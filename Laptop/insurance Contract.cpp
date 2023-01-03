#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "insurance.h"
	#include "insurance Contract.h"
	#include "WCheck.h"
	#include "Utilities.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "Insurance Text.h"
	#include "stdio.h"
	#include "Soldier Profile.h"
	#include "Overhead.h"
	#include "Text Input.h"
	#include "Soldier Add.h"
	#include "Game Clock.h"
	#include "finances.h"
	#include "History.h"
	#include "Game Event Hook.h"
	#include "LaptopSave.h"
	#include "english.h"
	#include "Text.h"
	#include "Random.h"
	#include "Strategic Status.h"
	#include "Assignments.h"
	#include "Map Screen Interface.h"
	#include "Interface.h"				// added by Flugente
#endif
#include <vector>

#ifdef JA2UB
#include "ub_config.h"
#include "quests.h"
#endif

#define		INS_CTRCT_ORDER_GRID_WIDTH					132
#define		INS_CTRCT_ORDER_GRID_HEIGHT					216
#define		INS_CTRCT_ORDER_GRID_OFFSET_X				INS_CTRCT_ORDER_GRID_WIDTH + 2


#define		INS_CTRCT_ORDER_GRID1_X							(76 + LAPTOP_SCREEN_UL_X)
#define		INS_CTRCT_ORDER_GRID1_Y							(126 + LAPTOP_SCREEN_WEB_UL_Y)

#define		INS_CTRCT_ORDER_GRID2_X							INS_CTRCT_ORDER_GRID1_X + INS_CTRCT_ORDER_GRID_OFFSET_X

#define		INS_CTRCT_ORDER_GRID3_X							INS_CTRCT_ORDER_GRID2_X + INS_CTRCT_ORDER_GRID_OFFSET_X

#define		INS_CTRCT_OG_FACE_OFFSET_X					5
#define		INS_CTRCT_OG_FACE_OFFSET_Y					4


#define		INS_CTRCT_OG_NICK_NAME_OFFSET_X			57
#define		INS_CTRCT_OG_NICK_NAME_OFFSET_Y			13

#define		INS_CTRCT_OG_HAS_CONTRACT_OFFSET_X	INS_CTRCT_OG_NICK_NAME_OFFSET_X
#define		INS_CTRCT_OG_HAS_CONTRACT_OFFSET_Y	INS_CTRCT_OG_NICK_NAME_OFFSET_Y + 13

#define		INS_CTRCT_TITLE_Y										(48 + LAPTOP_SCREEN_WEB_UL_Y)//52 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_CTRCT_FIRST_BULLET_TEXT_X				86 + LAPTOP_SCREEN_UL_X
#define		INS_CTRCT_FIRST_BULLET_TEXT_Y				65 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_CTRCT_SECOND_BULLET_TEXT_X			INS_CTRCT_FIRST_BULLET_TEXT_X
#define		INS_CTRCT_SECOND_BULLET_TEXT_Y			93 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_CTRCT_INTSRUCTION_TEXT_WIDTH		375

#define		INS_CTRCT_RED_BAR_UNDER_INSTRUCTION_TEXT_Y	123 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_X			4
#define		INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_Y			54

#define		INS_CTRCT_LENGTH_OFFSET_X										INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_X
#define		INS_CTRCT_LENGTH_OFFSET_Y										71

#define		INS_CTRCT_DAYS_REMAINING_OFFSET_Y						87

#define		INS_CTRCT_INSURANCE_CNTRCT_OFFSET_Y					108

#define		INS_CTRCT_PREMIUM_OWING_OFFSET_Y						160


#define		INS_CTRCT_OG_BOX_OFFSET_X										92
#define		INS_CTRCT_OG_BOX_WIDTH											35

#define		INS_CTRCT_ACCEPT_BTN_X											( 132 / 2 - 43 / 2 ) //6
#define		INS_CTRCT_ACCEPT_BTN_Y											193

#define		INS_CTRCT_CLEAR_BTN_X												86

#define		INS_CTRCT_BOTTON_LINK_Y										351 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_CTRCT_BOTTOM_LINK_RED_BAR_X						171 + LAPTOP_SCREEN_UL_X
#define		INS_CTRCT_BOTTON_LINK_RED_BAR_Y						INS_CTRCT_BOTTON_LINK_Y + 41


#define		INS_CTRCT_BOTTOM_LINK_RED_BAR_OFFSET			117

#define		INS_CTRCT_BOTTOM_LINK_RED_WIDTH						97

#define		INS_CTRCT_CONTRACT_STATUS_TEXT_WIDTH			74

// this is the percentage of daily salary used as a base to calculate daily insurance premiums
#define		INSURANCE_PREMIUM_RATE										5

#define		INS_CTRCT_SKILL_BASE											42
#define		INS_CTRCT_FITNESS_BASE										85
#define		INS_CTRCT_EXP_LEVEL_BASE									3
#define		INS_CTRCT_SURVIVAL_BASE										90


UINT32	guiInsOrderGridImage;
UINT32	guiInsOrderBulletImage;

INT16		gsForm1InsuranceLengthNumber;
INT16		gsForm2InsuranceLengthNumber;
INT16		gsForm3InsuranceLengthNumber;

UINT8		gubMercIDForMercInForm1;
UINT8		gubMercIDForMercInForm2;
UINT8		gubMercIDForMercInForm3;

UINT8		gubNumberofDisplayedInsuranceGrids;

BOOLEAN	gfChangeInsuranceFormButtons = FALSE;

// WDS - make number of mercenaries, etc. be configurable
std::vector<UINT8> gubInsuranceMercArray (CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS, 0);
INT16		gsCurrentInsuranceMercIndex;
INT16		gsMaxPlayersOnTeam;


//link to the varios pages
MOUSE_REGION	gSelectedInsuranceContractLinkRegion[2];
void SelectInsuranceContractRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

INT32		guiInsContractPrevButtonImage;
void		BtnInsContractPrevButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiInsContractPrevBackButton;

INT32		guiInsContractNextButtonImage;
void		BtnInsContractNextButtonCallBack(GUI_BUTTON *btn,INT32 reason);
UINT32	guiInsContractNextBackButton;


//Graphic for Accept, Clear button for form 1
INT32		guiInsuranceAcceptClearForm1ButtonImage;
void		BtnInsuranceAcceptClearForm1ButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiInsuranceAcceptClearForm1Button;

//Graphic for Accept, Clear button for form 2
void		BtnInsuranceAcceptClearForm2ButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiInsuranceAcceptClearForm2Button;

//Graphic for Accept, Clear button for form 3
void		BtnInsuranceAcceptClearForm3ButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiInsuranceAcceptClearForm3Button;




//
//	Function Prototypes
//
BOOLEAN		DisplayOrderGrid( UINT8 ubGridNumber, UINT8 ubMercID );
INT8			GetNumberOfHireMercsStartingFromID( UINT8 ubStartMercID );
//INT32			CalculateInsuranceCost( SOLDIERTYPE *pSoldier, BOOLEAN fHaveInsurance );
void			InsuranceContractUserTextFieldCallBack( UINT8 ubID, BOOLEAN fEntering );
INT8			CountInsurableMercs();
void			DisableInsuranceContractNextPreviousbuttons();
void			CreateDestroyInsuranceContractFormButtons( BOOLEAN fCreate);
void			HandleAcceptButton( UINT8 ubSoldierID, UINT8 ubFormID );
FLOAT			DiffFromNormRatio( INT16 sThisValue, INT16 sNormalValue );
void			InsContractNoMercsPopupCallBack( UINT8 bExitValue );
void			BuildInsuranceArray();
BOOLEAN		MercIsInsurable( SOLDIERTYPE *pSoldier );
//UINT32		GetContractLengthForFormNumber( UINT8 ubFormID );
void			EnableDisableInsuranceContractAcceptButtons();
UINT32		GetTimeRemainingOnSoldiersContract( SOLDIERTYPE *pSoldier );
UINT32		GetTimeRemainingOnSoldiersInsuranceContract( SOLDIERTYPE *pSoldier );
void			EnableDisableIndividualInsuranceContractButton( UINT8 ubMercIDForMercInForm1, UINT32 *puiAcceptButton );
BOOLEAN		CanSoldierExtendInsuranceContract( SOLDIERTYPE *pSoldier );
INT32			CalculateSoldiersInsuranceContractLength( SOLDIERTYPE *pSoldier );
INT32			CalcStartDayOfInsurance( SOLDIERTYPE *pSoldier );

BOOLEAN		AreAnyAimMercsOnTeam( );
//ppp




void GameInitInsuranceContract()
{
	gsCurrentInsuranceMercIndex = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
}


void EnterLaptopInitInsuranceContract()
{
	CHAR16		zTextField[14];

	swprintf( zTextField, L"%d", 0 );
	SetInputFieldStringWith16BitString( 1, zTextField );
	SetInputFieldStringWith16BitString( 2, zTextField );
	SetInputFieldStringWith16BitString( 3, zTextField );

}



BOOLEAN EnterInsuranceContract()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16					usPosX,i;

	//build the list of mercs that are can be displayed
	BuildInsuranceArray();

	gubNumberofDisplayedInsuranceGrids = GetNumberOfHireMercsStartingFromID( (UINT8) gsCurrentInsuranceMercIndex );
	if( gubNumberofDisplayedInsuranceGrids > 3 )
		gubNumberofDisplayedInsuranceGrids = 3;

	InitInsuranceDefaults();



	// load the Insurance title graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\InsOrderGrid.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsOrderGridImage));

	// load the Insurance bullet graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\bullet.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsOrderBulletImage));


	usPosX = INS_CTRCT_BOTTOM_LINK_RED_BAR_X;
	for(i=0; i<2; i++)
	{
		MSYS_DefineRegion( &gSelectedInsuranceContractLinkRegion[i], usPosX, INS_CTRCT_BOTTON_LINK_RED_BAR_Y-37, (UINT16)(usPosX + INS_CTRCT_BOTTOM_LINK_RED_WIDTH), INS_CTRCT_BOTTON_LINK_RED_BAR_Y+2, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectInsuranceContractRegionCallBack);
		MSYS_AddRegion(&gSelectedInsuranceContractLinkRegion[i]);
		MSYS_SetRegionUserData( &gSelectedInsuranceContractLinkRegion[i], 0, i );

		usPosX += INS_CTRCT_BOTTOM_LINK_RED_BAR_OFFSET;
	}


	//left arrow
	guiInsContractPrevButtonImage	= LoadButtonImage("LAPTOP\\InsLeftButton.sti", 2,0,-1,1,-1 );
	guiInsContractPrevBackButton = CreateIconAndTextButton( guiInsContractPrevButtonImage, InsContractText[INS_CONTRACT_PREVIOUS], INS_FONT_BIG,
													INS_FONT_COLOR, INS_FONT_SHADOW,
													INS_FONT_COLOR, INS_FONT_SHADOW,
													TEXT_CJUSTIFIED,
													INS_INFO_LEFT_ARROW_BUTTON_X, INS_INFO_LEFT_ARROW_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnInsContractPrevButtonCallback);
	SetButtonCursor( guiInsContractPrevBackButton, CURSOR_WWW );
	SpecifyButtonTextOffsets( guiInsContractPrevBackButton, 17, 16, FALSE );


	//Right arrow
	guiInsContractNextButtonImage	= LoadButtonImage("LAPTOP\\InsRightButton.sti", 2,0,-1,1,-1 );
	guiInsContractNextBackButton = CreateIconAndTextButton( guiInsContractNextButtonImage, InsContractText[INS_CONTRACT_NEXT], INS_FONT_BIG,
													INS_FONT_COLOR, INS_FONT_SHADOW,
													INS_FONT_COLOR, INS_FONT_SHADOW,
													TEXT_CJUSTIFIED,
													INS_INFO_RIGHT_ARROW_BUTTON_X, INS_INFO_RIGHT_ARROW_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnInsContractNextButtonCallBack);
	SetButtonCursor( guiInsContractNextBackButton, CURSOR_WWW );
	SpecifyButtonTextOffsets( guiInsContractNextBackButton, 18, 16, FALSE );

	//create the new set of buttons
	CreateDestroyInsuranceContractFormButtons( TRUE );

//	RenderInsuranceContract();
	return(TRUE);
}

void ExitInsuranceContract()
{
	UINT8 i;

	RemoveInsuranceDefaults();

	DeleteVideoObjectFromIndex( guiInsOrderGridImage );

	DeleteVideoObjectFromIndex( guiInsOrderBulletImage );

	for(i=0; i<2; i++)
		MSYS_RemoveRegion( &gSelectedInsuranceContractLinkRegion[i]);

	//the previous button
	UnloadButtonImage( guiInsContractPrevButtonImage );
	RemoveButton( guiInsContractPrevBackButton );

	//the next button
	UnloadButtonImage( guiInsContractNextButtonImage );
	RemoveButton( guiInsContractNextBackButton );

	CreateDestroyInsuranceContractFormButtons( FALSE );
}



void HandleInsuranceContract()
{
	if( gfChangeInsuranceFormButtons )
	{
		//remove the old buttons from the page
		CreateDestroyInsuranceContractFormButtons( FALSE );

		//Get the new number of displayed insurance grids
		gubNumberofDisplayedInsuranceGrids = GetNumberOfHireMercsStartingFromID( (UINT8) gsCurrentInsuranceMercIndex );
		if( gubNumberofDisplayedInsuranceGrids > 3 )
			gubNumberofDisplayedInsuranceGrids = 3;

		//create the new set of buttons
		CreateDestroyInsuranceContractFormButtons( TRUE );

		//reset the flag
		gfChangeInsuranceFormButtons = FALSE;

		//force a redraw of the screen to erase the old buttons
		fPausedReDrawScreenFlag = TRUE;
		RenderInsuranceContract();

		MarkButtonsDirty();
	}

	EnableDisableInsuranceContractAcceptButtons();
}



void RenderInsuranceContract()
{
	HVOBJECT	hPixHandle;
	CHAR16		sText[800];
	UINT8			ubCount=0;
	INT16			sMercID;
	INT16			sNextMercID;
	UINT16		usPosX;
	SOLDIERTYPE *pSoldier = NULL;


	SetFontShadow( INS_FONT_SHADOW );

	DisplayInsuranceDefaults();

	//disable the next or previous button depending on how many more mercs we have to display
	DisableInsuranceContractNextPreviousbuttons();

	usPosX = INS_CTRCT_BOTTOM_LINK_RED_BAR_X;

	//Display the red bar under the link at the bottom.	and the text
	DisplaySmallColouredLineWithShadow( usPosX, INS_CTRCT_BOTTON_LINK_RED_BAR_Y, (UINT16)(usPosX + INS_CTRCT_BOTTOM_LINK_RED_WIDTH), INS_CTRCT_BOTTON_LINK_RED_BAR_Y );
	swprintf( sText, L"%s", pMessageStrings[ MSG_HOMEPAGE ] );
	DisplayWrappedString( usPosX, INS_CTRCT_BOTTON_LINK_Y+18, INS_CTRCT_BOTTOM_LINK_RED_WIDTH, 2, INS_FONT_MED, INS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	usPosX += INS_CTRCT_BOTTOM_LINK_RED_BAR_OFFSET;

	//Display the red bar under the link at the bottom.	and the text
	DisplaySmallColouredLineWithShadow( usPosX, INS_CTRCT_BOTTON_LINK_RED_BAR_Y, (UINT16)(usPosX + INS_CTRCT_BOTTOM_LINK_RED_WIDTH), INS_CTRCT_BOTTON_LINK_RED_BAR_Y );
	GetInsuranceText( INS_SNGL_HOW_DOES_INS_WORK, sText );
	DisplayWrappedString( usPosX, INS_CTRCT_BOTTON_LINK_Y+12, INS_CTRCT_BOTTOM_LINK_RED_WIDTH, 2, INS_FONT_MED, INS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);


	//Display the title slogan
	GetInsuranceText( INS_SNGL_ENTERING_REVIEWING_CLAIM, sText );
	DrawTextToScreen( sText, LAPTOP_SCREEN_UL_X, INS_CTRCT_TITLE_Y, LAPTOP_SCREEN_LR_X-(LAPTOP_SCREEN_UL_X)/*-iScreenWidthOffset*/, INS_FONT_BIG, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );


	//Get and display the insurance bullet
	GetVideoObject(&hPixHandle, guiInsOrderBulletImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, INS_CTRCT_FIRST_BULLET_TEXT_X, INS_CTRCT_FIRST_BULLET_TEXT_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//Display the first instruction sentence
	GetInsuranceText( INS_MLTI_TO_PURCHASE_INSURANCE, sText );
	DisplayWrappedString( INS_CTRCT_FIRST_BULLET_TEXT_X+INSURANCE_BULLET_TEXT_OFFSET_X, INS_CTRCT_FIRST_BULLET_TEXT_Y, INS_CTRCT_INTSRUCTION_TEXT_WIDTH, 2, INS_FONT_MED, INS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);


	//Get and display the insurance bullet
	GetVideoObject(&hPixHandle, guiInsOrderBulletImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, INS_CTRCT_FIRST_BULLET_TEXT_X, INS_CTRCT_SECOND_BULLET_TEXT_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//Display the second instruction sentence
	GetInsuranceText( INS_MLTI_ONCE_SATISFIED_CLICK_ACCEPT, sText );
	DisplayWrappedString( INS_CTRCT_FIRST_BULLET_TEXT_X+INSURANCE_BULLET_TEXT_OFFSET_X, INS_CTRCT_SECOND_BULLET_TEXT_Y, INS_CTRCT_INTSRUCTION_TEXT_WIDTH, 2, INS_FONT_MED, INS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	//Display the red bar under the instruction text
	DisplaySmallColouredLineWithShadow( INS_CTRCT_FIRST_BULLET_TEXT_X, INS_CTRCT_RED_BAR_UNDER_INSTRUCTION_TEXT_Y, INS_CTRCT_FIRST_BULLET_TEXT_X + INS_CTRCT_INTSRUCTION_TEXT_WIDTH, INS_CTRCT_RED_BAR_UNDER_INSTRUCTION_TEXT_Y );


	sNextMercID =	gsCurrentInsuranceMercIndex;
	while( ( ubCount < gubNumberofDisplayedInsuranceGrids ) && ( sNextMercID <= gTacticalStatus.Team[ gbPlayerNum ].bLastID ) )
	{
		sMercID = gubInsuranceMercArray[ sNextMercID ];

		pSoldier = &Menptr[ GetSoldierIDFromMercID( (UINT8) sMercID ) ];

		if( ( sMercID != -1 ) && MercIsInsurable( pSoldier ) )
		{
			DisplayOrderGrid( ubCount, (UINT8)sMercID );
			ubCount++;
		}

		sNextMercID++;
	}

	//if there are no valid mercs to insure
	if( ubCount == 0 )
	{
		//if there where AIM mercs ( on short contract )
		if( AreAnyAimMercsOnTeam( ) )
		{
			//Display Error Message, all aim mercs are on short contract
			GetInsuranceText( INS_MLTI_ALL_AIM_MERCS_ON_SHORT_CONTRACT, sText );
			DoLapTopMessageBox( MSG_BOX_RED_ON_WHITE, sText, LAPTOP_SCREEN, MSG_BOX_FLAG_OK, InsContractNoMercsPopupCallBack);
		}
		else
		{
			//Display Error Message, no valid mercs
			GetInsuranceText( INS_MLTI_NO_QUALIFIED_MERCS, sText );
			DoLapTopMessageBox( MSG_BOX_RED_ON_WHITE, sText, LAPTOP_SCREEN, MSG_BOX_FLAG_OK, InsContractNoMercsPopupCallBack);
		}
	}



	SetFontShadow(DEFAULT_SHADOW);
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}





void BtnInsContractPrevButtonCallback(GUI_BUTTON *btn,INT32 reason)
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

			if( gsCurrentInsuranceMercIndex > 2 )
				gsCurrentInsuranceMercIndex -= 3;

			//signal that we want to change the number of forms on the page
			gfChangeInsuranceFormButtons = TRUE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}



void BtnInsContractNextButtonCallBack(GUI_BUTTON *btn,INT32 reason)
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

			gsCurrentInsuranceMercIndex += 3;

			//signal that we want to change the number of forms on the page
			gfChangeInsuranceFormButtons = TRUE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}




BOOLEAN DisplayOrderGrid( UINT8 ubGridNumber, UINT8 ubMercID )
{
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT hPixHandle;
	UINT16	usPosX, usPosY;
	UINT32	uiInsMercFaceImage;
	INT32		iCostOfContract=0;
	char			sTemp[100];
	CHAR16		sText[800];
	BOOLEAN		fDisplayMercContractStateTextColorInRed = FALSE;

	SOLDIERTYPE	*pSoldier = &Menptr[ GetSoldierIDFromMercID( ubMercID ) ];


	usPosX=usPosY=0;

	switch( ubGridNumber )
	{
		case 0:
			usPosX = INS_CTRCT_ORDER_GRID1_X;
			gubMercIDForMercInForm1 = ubMercID;
			gsForm1InsuranceLengthNumber = (INT16) pSoldier->iTotalLengthOfInsuranceContract;
			break;

		case 1:
			usPosX = INS_CTRCT_ORDER_GRID2_X;
			gubMercIDForMercInForm2 = ubMercID;
			gsForm2InsuranceLengthNumber = (INT16) pSoldier->iTotalLengthOfInsuranceContract;
			break;

		case 2:
			usPosX = INS_CTRCT_ORDER_GRID3_X;
			gubMercIDForMercInForm3 = ubMercID;
			gsForm3InsuranceLengthNumber = (INT16) pSoldier->iTotalLengthOfInsuranceContract;
			break;

		default:
			//should never get in here
			Assert(0);
			break;
	}

	//Get and display the insurance order grid #1
	GetVideoObject(&hPixHandle, guiInsOrderGridImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, usPosX, INS_CTRCT_ORDER_GRID1_Y, VO_BLT_SRCTRANSPARENCY,NULL);


	// load the mercs face graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	sprintf(sTemp, "FACES\\%02d.sti", gMercProfiles[pSoldier->ubProfile].ubFaceIndex );
	FilenameForBPP( sTemp, VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &uiInsMercFaceImage));

	//Get the merc's face
	GetVideoObject(&hPixHandle, uiInsMercFaceImage );

	//if the merc is dead, shade the face red
	if( IsMercDead( pSoldier->ubProfile ) )
	{
		//if the merc is dead
		//shade the face red, (to signify that he is dead)
		hPixHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hPixHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );

		//set the red pallete to the face
		SetObjectHandleShade( uiInsMercFaceImage, 0 );
	}

	//Get and display the mercs face
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, usPosX+INS_CTRCT_OG_FACE_OFFSET_X, INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_OG_FACE_OFFSET_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// the face images isn't needed anymore so delete it
	DeleteVideoObjectFromIndex( uiInsMercFaceImage );

	//display the mercs nickname
	DrawTextToScreen(gMercProfiles[ ubMercID ].zNickname, (UINT16)(usPosX + INS_CTRCT_OG_NICK_NAME_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y + INS_CTRCT_OG_NICK_NAME_OFFSET_Y, 0, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

	//Get the text to display the mercs current insurance contract status
	if( IsMercDead( pSoldier->ubProfile ) )
	{
		//if the merc has a contract
		if( pSoldier->usLifeInsurance )
		{
			//Display the contract text
			GetInsuranceText( INS_SNGL_DEAD_WITH_CONTRACT, sText );
		}
		else
		{
			//Display the contract text
			GetInsuranceText( INS_SNGL_DEAD_NO_CONTRACT, sText );
		}
		DisplayWrappedString( (UINT16)(usPosX+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_Y, INS_CTRCT_CONTRACT_STATUS_TEXT_WIDTH, 2, INS_FONT_SMALL, INS_FONT_COLOR_RED,	sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
	else
	{
		//if the merc has a contract
		if( pSoldier->usLifeInsurance )
		{
			//if the soldier can extend their insurance
			if( CanSoldierExtendInsuranceContract( pSoldier ) )
			{
				//Display the contract text
				GetInsuranceText( INS_SNGL_PARTIALLY_INSURED, sText );
				fDisplayMercContractStateTextColorInRed = TRUE;
			}
			else
			{
				//Display the contract text
				GetInsuranceText( INS_SNGL_CONTRACT, sText );
				fDisplayMercContractStateTextColorInRed = FALSE;
			}
		}
		else
		{
			//Display the contract text
			GetInsuranceText( INS_SNGL_NOCONTRACT, sText );
			fDisplayMercContractStateTextColorInRed = TRUE;
		}
		if( fDisplayMercContractStateTextColorInRed )
			DisplayWrappedString( (UINT16)(usPosX+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_Y, INS_CTRCT_CONTRACT_STATUS_TEXT_WIDTH, 2, INS_FONT_SMALL, INS_FONT_COLOR_RED,	sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
		else
			DisplayWrappedString( (UINT16)(usPosX+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_OG_HAS_CONTRACT_OFFSET_Y, INS_CTRCT_CONTRACT_STATUS_TEXT_WIDTH, 2, INS_FONT_SMALL, INS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}



	//Display the Emplyment contract text
	GetInsuranceText( INS_SNGL_EMPLOYMENT_CONTRACT, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_Y, INS_CTRCT_ORDER_GRID_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the merc contract Length text
	GetInsuranceText( INS_SNGL_LENGTH, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_LENGTH_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_LENGTH_OFFSET_Y, 0, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Display the mercs contract length
	swprintf( sText, L"%d", pSoldier->iTotalContractLength );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_OG_BOX_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_LENGTH_OFFSET_Y, INS_CTRCT_OG_BOX_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );


	//Display the days remaining for the emplyment contract text
	GetInsuranceText( INS_SNGL_DAYS_REMAINING, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_LENGTH_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_DAYS_REMAINING_OFFSET_Y, 0, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );


	//display the amount of time the merc has left on their Regular contract
	if( IsMercDead( pSoldier->ubProfile ) )
		swprintf( sText, L"%s", pMessageStrings[ MSG_LOWERCASE_NA ] );
	else
		swprintf( sText, L"%d", GetTimeRemainingOnSoldiersContract( pSoldier ) );

	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_OG_BOX_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_DAYS_REMAINING_OFFSET_Y, INS_CTRCT_OG_BOX_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );

	//Display the Insurqance contract
	GetInsuranceText( INS_SNGL_INSURANCE_CONTRACT, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_INSURANCE_CNTRCT_OFFSET_Y, INS_CTRCT_ORDER_GRID_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );


	GetInsuranceText( INS_SNGL_LENGTH, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_LENGTH_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_LENGTH_OFFSET_Y+54, 0, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Display the insurance days remaining text
	GetInsuranceText( INS_SNGL_DAYS_REMAINING, sText );
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_LENGTH_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_DAYS_REMAINING_OFFSET_Y+54, 0, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );



	//
	//display the amount of time the merc has left on the insurance contract
	//

	//if the soldier has insurance, disply the length of time the merc has left
	if( IsMercDead( pSoldier->ubProfile ) )
		swprintf( sText, L"%s", pMessageStrings[ MSG_LOWERCASE_NA ] );

	else if( pSoldier->usLifeInsurance != 0 )
		swprintf( sText, L"%d", GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) );

	else
		swprintf( sText, L"%d", 0 );

	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_OG_BOX_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_DAYS_REMAINING_OFFSET_Y+54, INS_CTRCT_OG_BOX_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );


	//
	// Calculate the insurance cost
	//

	//if the soldier can get insurance, calculate a new cost
	if( CanSoldierExtendInsuranceContract( pSoldier ) )
	{
		iCostOfContract =CalculateInsuranceContractCost( CalculateSoldiersInsuranceContractLength( pSoldier ), pSoldier->ubProfile );
	}

	else
	{
		iCostOfContract = 0;
	}

	if( iCostOfContract < 0 )
	{
		//shouldnt get in here now since we can longer give refunds
		Assert( 0 );
	}
	else
	{
		//Display the premium owing text
		GetInsuranceText( INS_SNGL_PREMIUM_OWING, sText );
		DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_EMPLYMNT_CNTRCT_TEXT_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_PREMIUM_OWING_OFFSET_Y, INS_CTRCT_ORDER_GRID_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

		//display the amount of refund
		swprintf( sText, L"%d", iCostOfContract );
		InsertCommasForDollarFigure( sText );
		InsertDollarSignInToString( sText );
	}


	if( IsMercDead( ubMercID ) )
	{
		swprintf( sText, L"0");
		InsertDollarSignInToString( sText );
	}
	//display the amount owing
	DrawTextToScreen( sText, (UINT16)(usPosX+32), INS_CTRCT_ORDER_GRID1_Y+179, 72, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );



	//
	// Get the insurance contract length for the merc
	//
	swprintf( sText, L"%d", CalculateSoldiersInsuranceContractLength( pSoldier ) );


	//Display the length of time the player can get for the insurance contract
	DrawTextToScreen( sText, (UINT16)(usPosX+INS_CTRCT_OG_BOX_OFFSET_X), INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_LENGTH_OFFSET_Y+52+2, INS_CTRCT_OG_BOX_WIDTH, INS_FONT_MED, INS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );

	return( TRUE );
}



void BtnInsuranceAcceptClearForm1ButtonCallback(GUI_BUTTON *btn,INT32 reason)
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
			UINT8	ubButton = (UINT8) MSYS_GetBtnUserData( btn, 0 );
			UINT8	ubSoldierID = (UINT8) GetSoldierIDFromMercID( gubMercIDForMercInForm1 );

			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			//the accept button
			if( ubButton == 0 )
			{
				//handle the accept button, (global to all accept button
				HandleAcceptButton( ubSoldierID, 1 );

				//specify the length of the insurance contract
				Menptr[ ubSoldierID ].iTotalLengthOfInsuranceContract = gsForm1InsuranceLengthNumber;

				//reset the insurance length
				gsForm1InsuranceLengthNumber = 0;
			}

			//redraw the screen
			fPausedReDrawScreenFlag = TRUE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}



void BtnInsuranceAcceptClearForm2ButtonCallback(GUI_BUTTON *btn,INT32 reason)
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
			UINT8	ubButton = (UINT8) MSYS_GetBtnUserData( btn, 0 );
			UINT8	ubSoldierID = (UINT8) GetSoldierIDFromMercID( gubMercIDForMercInForm2 );

			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			//the accept button
			if( ubButton == 0 )
			{
				//handle the accept button, (global to all accept button
				HandleAcceptButton( ubSoldierID, 2 );

				//specify the length of the insurance contract
				Menptr[ ubSoldierID ].iTotalLengthOfInsuranceContract = gsForm2InsuranceLengthNumber;

				//reset the insurance length
				gsForm2InsuranceLengthNumber = 0;
			}

			//redraw the screen
			fPausedReDrawScreenFlag = TRUE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}




void BtnInsuranceAcceptClearForm3ButtonCallback(GUI_BUTTON *btn,INT32 reason)
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
			UINT8	ubButton = (UINT8) MSYS_GetBtnUserData( btn, 0 );
			UINT8	ubSoldierID = (UINT8) GetSoldierIDFromMercID( gubMercIDForMercInForm3 );

			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			//the accept button
			if( ubButton == 0 )
			{
				//handle the accept button, (global to all accept button
				HandleAcceptButton( ubSoldierID, 3 );

				//specify the length of the insurance contract
				Menptr[ ubSoldierID ].iTotalLengthOfInsuranceContract = gsForm3InsuranceLengthNumber;

				//reset the insurance length
				gsForm3InsuranceLengthNumber = 0;
			}

			//redraw the screen
			fPausedReDrawScreenFlag = TRUE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}



INT8 GetNumberOfHireMercsStartingFromID( UINT8 ubStartMercID )
{
	UINT8	i;
	UINT8	ubCount=0;

	for( i=0; i<gsMaxPlayersOnTeam; i++)
	{
		if( i>= ubStartMercID )
		{
			ubCount++;
		}
	}

	return( ubCount );
}


/*
INT32 CalculateInsuranceCost( SOLDIERTYPE *pSoldier, BOOLEAN fHaveInsurance )
{
	INT32			iAmount=0;
	UINT32		uiInsuranceContractLength = 0;

	uiInsuranceContractLength = CalculateSoldiersInsuranceContractLength( pSoldier );

	//If the soldier already has life insurance, then the user is changing the length of the contract
	if( pSoldier->usLifeInsurance )
	{
		//if the user is changing the contract length
		if( uiInsuranceContractLength != 0 )
		{
			iAmount = CalculateInsuranceContractCost( uiInsuranceContractLength, pSoldier->ubProfile);
		}
		//else we are just calculating the new figure
		else
		{
			iAmount = 0;
		}
	}
	//else the merc doesn't have life insurance
	else
	{
		iAmount = CalculateInsuranceContractCost( uiInsuranceContractLength, pSoldier->ubProfile);
	}

	return( iAmount );
}
*/

void SelectInsuranceContractRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32 uiInsuranceLink = MSYS_GetRegionUserData( pRegion, 0 );

		if( uiInsuranceLink == 0 )
			guiCurrentLaptopMode = LAPTOP_MODE_INSURANCE;
		else if( uiInsuranceLink == 1 )
			guiCurrentLaptopMode = LAPTOP_MODE_INSURANCE_INFO;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}

INT8 CountInsurableMercs()
{
	INT16					cnt;
	SOLDIERTYPE		*pSoldier;
	INT16					bLastTeamID;
	INT8					bCount=0;

	// Set locator to first merc
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	for ( pSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pSoldier++)
	{
		if (MercIsInsurable(pSoldier))
		{
			bCount++;
		}
	}

	return( bCount );
}

void DisableInsuranceContractNextPreviousbuttons()
{
	//disable the next button if there is no more mercs to display
	if( ( gsCurrentInsuranceMercIndex + gubNumberofDisplayedInsuranceGrids ) < CountInsurableMercs() )
	{
		EnableButton( guiInsContractNextBackButton );
	}
	else
		DisableButton( guiInsContractNextBackButton );

	//if we are currently displaying the first set of mercs, disable the previous button
	if( gsCurrentInsuranceMercIndex < 3 )
	{
		DisableButton( guiInsContractPrevBackButton );
	}
	else
		EnableButton( guiInsContractPrevBackButton );

}

void CreateDestroyInsuranceContractFormButtons( BOOLEAN fCreate)
{
	static BOOLEAN	fButtonsCreated = FALSE;

	if( fCreate && !fButtonsCreated )
	{
		//place the 3 accept buttons for the different forms

		//The accept button image
		guiInsuranceAcceptClearForm1ButtonImage	= LoadButtonImage("LAPTOP\\AcceptClearBox.sti", -1,0,-1,1,-1 );

		if( gubNumberofDisplayedInsuranceGrids >= 1 )
		{
			//the accept button for form 1
			guiInsuranceAcceptClearForm1Button = CreateIconAndTextButton( guiInsuranceAcceptClearForm1ButtonImage, InsContractText[INS_CONTRACT_ACCEPT], INS_FONT_MED,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															TEXT_CJUSTIFIED,
															INS_CTRCT_ORDER_GRID1_X+INS_CTRCT_ACCEPT_BTN_X, INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_ACCEPT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															DEFAULT_MOVE_CALLBACK, BtnInsuranceAcceptClearForm1ButtonCallback);
			SetButtonCursor( guiInsuranceAcceptClearForm1Button, CURSOR_LAPTOP_SCREEN );
			MSYS_SetBtnUserData( guiInsuranceAcceptClearForm1Button, 0, 0);
		}



		if( gubNumberofDisplayedInsuranceGrids >= 2 )
		{
			//the accept button for form 2
			guiInsuranceAcceptClearForm2Button = CreateIconAndTextButton( guiInsuranceAcceptClearForm1ButtonImage, InsContractText[INS_CONTRACT_ACCEPT], INS_FONT_MED,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															TEXT_CJUSTIFIED,
															INS_CTRCT_ORDER_GRID2_X+INS_CTRCT_ACCEPT_BTN_X, INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_ACCEPT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															DEFAULT_MOVE_CALLBACK, BtnInsuranceAcceptClearForm2ButtonCallback);
			SetButtonCursor(guiInsuranceAcceptClearForm2Button, CURSOR_LAPTOP_SCREEN );
			MSYS_SetBtnUserData( guiInsuranceAcceptClearForm2Button, 0, 0);

		}




		if( gubNumberofDisplayedInsuranceGrids >= 3 )
		{
			//the accept button for form 3
			guiInsuranceAcceptClearForm3Button = CreateIconAndTextButton( guiInsuranceAcceptClearForm1ButtonImage, InsContractText[INS_CONTRACT_ACCEPT], INS_FONT_MED,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															INS_FONT_BTN_COLOR, INS_FONT_BTN_SHADOW_COLOR,
															TEXT_CJUSTIFIED,
															INS_CTRCT_ORDER_GRID3_X+INS_CTRCT_ACCEPT_BTN_X, INS_CTRCT_ORDER_GRID1_Y+INS_CTRCT_ACCEPT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															DEFAULT_MOVE_CALLBACK, BtnInsuranceAcceptClearForm3ButtonCallback);
			SetButtonCursor( guiInsuranceAcceptClearForm3Button, CURSOR_LAPTOP_SCREEN );
			MSYS_SetBtnUserData( guiInsuranceAcceptClearForm3Button, 0, 0);
		}

		fButtonsCreated = TRUE;
	}

	if( fButtonsCreated && ! fCreate )
	{
		//the accept image
		UnloadButtonImage( guiInsuranceAcceptClearForm1ButtonImage );

		if( gubNumberofDisplayedInsuranceGrids >= 1 )
		{
			//the accept for the first form
			RemoveButton( guiInsuranceAcceptClearForm1Button );
		}

		if( gubNumberofDisplayedInsuranceGrids >= 2 )
		{
			//the accept clear for the second form
			RemoveButton( guiInsuranceAcceptClearForm2Button );
		}

		if( gubNumberofDisplayedInsuranceGrids >= 3 )
		{
			//the accept clear for the third form
			RemoveButton( guiInsuranceAcceptClearForm3Button );
		}

		fButtonsCreated = FALSE;
	}
}



void HandleAcceptButton( UINT8 ubSoldierID, UINT8 ubFormID )
{
	//passed in either 1,2,3 should be 0,1,2
	ubFormID--;

	PurchaseOrExtendInsuranceForSoldier( &Menptr[ ubSoldierID ], CalculateSoldiersInsuranceContractLength( &Menptr[ ubSoldierID ] ) );

	RenderInsuranceContract();
}









// determines if a merc will run out of their insurance contract
void DailyUpdateOfInsuredMercs()
{
	INT16		cnt;
	INT16		bLastTeamID;
	SOLDIERTYPE		*pSoldier;

	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	for ( pSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pSoldier++)
	{
		//if the soldier is in the team array
		if( pSoldier->bActive )
		{
			//if the merc has life insurance
			if( pSoldier->usLifeInsurance )
			{
				//if the merc wasn't just hired
				if( (INT16)GetWorldDay() != pSoldier->iStartOfInsuranceContract )
				{
					//if the contract has run out of time
					if( GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) <= 0 )
					{
						//if the soldier isn't dead
						if( !IsMercDead( pSoldier->ubProfile ) )
						{
							pSoldier->usLifeInsurance = 0;
							pSoldier->iTotalLengthOfInsuranceContract = 0;
							pSoldier->iStartOfInsuranceContract = 0;
						}
					}
				}
			}
		}
	}
}


#define MIN_INSURANCE_RATIO		0.1f
#define MAX_INSURANCE_RATIO		10.0f


INT32	CalculateInsuranceContractCost( INT32 iLength, UINT8 ubMercID )
{
	MERCPROFILESTRUCT * pProfile;
	INT16	sTotalSkill=0;
	FLOAT flSkillFactor, flFitnessFactor, flExpFactor, flSurvivalFactor;
	FLOAT flRiskFactor;
	UINT32 uiDailyInsurancePremium;
	UINT32 uiTotalInsurancePremium;
	SOLDIERTYPE	*pSoldier;


	pSoldier = &Menptr[ GetSoldierIDFromMercID( ubMercID ) ];


	// only mercs with at least 2 days to go on their employment contract are insurable
	// def: 2/5/99.	However if they already have insurance is SHOULD be ok
	if( GetTimeRemainingOnSoldiersContract( pSoldier ) < 2 && !( pSoldier->usLifeInsurance != 0 && GetTimeRemainingOnSoldiersContract( pSoldier ) >= 1 ) )
	{
		return( 0 );
	}

	//If the merc is currently being held captive, get out
	if (pSoldier->bAssignment == ASSIGNMENT_POW)
	{
		return( 0 );
	}

/*
	replaced with the above check

	if (iLength < 2)
	{
		return(0);
	}
	*/

	pProfile = &gMercProfiles[ ubMercID ];

	// calculate the degree of training
	sTotalSkill = (pProfile->bMarksmanship + pProfile->bMedical + pProfile->bMechanical + pProfile->bExplosive + pProfile->bLeadership) / 5;
	flSkillFactor = DiffFromNormRatio( sTotalSkill, INS_CTRCT_SKILL_BASE );

	// calc relative fitness level
	flFitnessFactor = DiffFromNormRatio( pProfile->bLife, INS_CTRCT_FITNESS_BASE );

	// calc relative experience
	flExpFactor = DiffFromNormRatio( pProfile->bExpLevel, INS_CTRCT_EXP_LEVEL_BASE );

	// calc player's survival rate (death rate subtracted from 100)
	flSurvivalFactor = DiffFromNormRatio( (INT16) (100 - CalcDeathRate()), INS_CTRCT_SURVIVAL_BASE );

	// calculate the overall insurability risk factor for this merc by combining all the subfactors
	flRiskFactor = flSkillFactor * flFitnessFactor * flExpFactor * flSurvivalFactor;

	// Flugente: backgrounds
	flRiskFactor = flRiskFactor * (100 + pSoldier->GetBackgroundValue(BG_PERC_INSURANCE)) / 100;

	// restrict the overall factor to within reasonable limits
	if (flRiskFactor < MIN_INSURANCE_RATIO)
	{
		flRiskFactor = MIN_INSURANCE_RATIO;
	}
	else
	if (flRiskFactor > MAX_INSURANCE_RATIO)
	{
		flRiskFactor = MAX_INSURANCE_RATIO;
	}

	// premium depend on merc's salary, the base insurance rate, and the individual's risk factor at this time
	uiDailyInsurancePremium = (UINT32) (pProfile->sSalary * INSURANCE_PREMIUM_RATE * flRiskFactor / 100.0f + 0.5f);
	// multiply by the insurance contract length
	uiTotalInsurancePremium = uiDailyInsurancePremium * iLength;

	return( uiTotalInsurancePremium );
}


// values passed in must be such that exceeding the normal value REDUCES insurance premiums
FLOAT DiffFromNormRatio( INT16 sThisValue, INT16 sNormalValue )
{
	FLOAT flRatio;

	if (sThisValue > 0)
	{
		flRatio = (FLOAT) sNormalValue / sThisValue;

		// restrict each ratio to within a reasonable range
		if (flRatio < MIN_INSURANCE_RATIO)
		{
			flRatio = MIN_INSURANCE_RATIO;
		}
		else
		if (flRatio > MAX_INSURANCE_RATIO)
		{
			flRatio = MAX_INSURANCE_RATIO;
		}
	}
	else
	{
		// use maximum allowable ratio
		flRatio = MAX_INSURANCE_RATIO;
	}

	return( flRatio );
}


void InsContractNoMercsPopupCallBack( UINT8 bExitValue )
{
	// yes, so start over, else stay here and do nothing for now
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		guiCurrentLaptopMode = LAPTOP_MODE_INSURANCE;
	}

	return;
}

void BuildInsuranceArray()
{
	INT16					cnt;
	SOLDIERTYPE		*pSoldier;
	INT16					bLastTeamID;

	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;
	gsMaxPlayersOnTeam = 0;

	// store profile #s of all insurable mercs in an array
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pSoldier++)
	{
		if( MercIsInsurable(pSoldier) )
		{
			gubInsuranceMercArray[ gsMaxPlayersOnTeam ] = pSoldier->ubProfile;
			gsMaxPlayersOnTeam++;
		}
	}
}


BOOLEAN AddLifeInsurancePayout( SOLDIERTYPE *pSoldier )
{
	UINT8	ubPayoutID;
	UINT32 uiTimeInMinutes;
	MERCPROFILESTRUCT *pProfile;
	UINT32 uiCostPerDay;
	UINT32 uiDaysToPay;


	Assert(pSoldier != NULL);
	Assert(pSoldier->ubProfile != NO_PROFILE);

	pProfile = &(gMercProfiles[ pSoldier->ubProfile ]);

	//if we need to add more array elements
	if( LaptopSaveInfo.ubNumberLifeInsurancePayouts <= LaptopSaveInfo.ubNumberLifeInsurancePayoutUsed )
	{
		LaptopSaveInfo.ubNumberLifeInsurancePayouts++;
		LaptopSaveInfo.pLifeInsurancePayouts = (LIFE_INSURANCE_PAYOUT *) MemRealloc( LaptopSaveInfo.pLifeInsurancePayouts, sizeof( LIFE_INSURANCE_PAYOUT ) * LaptopSaveInfo.ubNumberLifeInsurancePayouts	);
		if( LaptopSaveInfo.pLifeInsurancePayouts == NULL )
			return( FALSE );

		memset( &LaptopSaveInfo.pLifeInsurancePayouts[ LaptopSaveInfo.ubNumberLifeInsurancePayouts - 1 ], 0, sizeof( LIFE_INSURANCE_PAYOUT ) );
	}

	for( ubPayoutID = 0; ubPayoutID < LaptopSaveInfo.ubNumberLifeInsurancePayouts; ubPayoutID++ )
	{
		//get an empty element in the array
		if( !LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].fActive )
			break;
	}

	LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubSoldierID = pSoldier->ubID;
	LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID = pSoldier->ubProfile;
	LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].fActive = TRUE;

	// This uses the merc's latest salaries, ignoring that they may be higher than the salaries paid under the current
	// contract if the guy has recently gained a level.	We could store his daily salary when he was last contracted,
	// and use that, but it still doesn't easily account for the fact that renewing a leveled merc early means that the
	// first part of his contract is under his old salary and the second part is under his new one.	Therefore, I chose
	// to ignore this wrinkle, and let the player awlays get paid out using the higher amount.	ARM

	// figure out which of the 3 salary rates the merc has is the cheapest, and use it to calculate the paid amount, to
	// avoid getting back more than the merc cost if he was on a 2-week contract!

	// start with the daily salary
	uiCostPerDay = pProfile->sSalary;

	// consider weekly salary / day
	if ((pProfile->uiWeeklySalary / 7) < uiCostPerDay)
	{
		uiCostPerDay = (pProfile->uiWeeklySalary / 7);
	}

	// consider biweekly salary / day
	if ((pProfile->uiBiWeeklySalary / 14) < uiCostPerDay)
	{
		uiCostPerDay = (pProfile->uiBiWeeklySalary / 14);
	}

	// calculate how many full, insured days of work the merc is going to miss
	uiDaysToPay = pSoldier->iTotalLengthOfInsuranceContract - ( GetWorldDay() + 1 - pSoldier->iStartOfInsuranceContract );

	// calculate & store how much is to be paid out
	LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice = uiDaysToPay * uiCostPerDay;

	// 4pm next day
	uiTimeInMinutes = GetMidnightOfFutureDayInMinutes( 1 ) + 16 * 60;

	// if the death was suspicious, or he's already been investigated twice or more
	if (pProfile->ubSuspiciousDeath || (gStrategicStatus.ubInsuranceInvestigationsCnt >= 2))
	{
		// fraud suspected, claim will be investigated first
		AddStrategicEvent( EVENT_INSURANCE_INVESTIGATION_STARTED, uiTimeInMinutes, ubPayoutID );
	}
	else
	{
		// is ok, make a prompt payment
		AddStrategicEvent( EVENT_PAY_LIFE_INSURANCE_FOR_DEAD_MERC, uiTimeInMinutes, ubPayoutID );
	}

	LaptopSaveInfo.ubNumberLifeInsurancePayoutUsed++;

	return( TRUE );
}


void StartInsuranceInvestigation( UINT8	ubPayoutID )
{
	UINT8 ubDays;

	// send an email telling player an investigation is taking place
	if (gStrategicStatus.ubInsuranceInvestigationsCnt == 0)
	{
		// first offense
#ifdef JA2UB
// no UB
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			AddEmailWithSpecialData( 173, INSUR_SUSPIC_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT, TYPE_E_INSURANCE_L4 );
	}
#else
		AddEmailWithSpecialData( INSUR_SUSPIC, INSUR_SUSPIC_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif
	}
	else
	{
		// subsequent offense
#ifdef JA2UB
// no UB
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			AddEmailWithSpecialData( 179, INSUR_SUSPIC_2_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT, TYPE_E_INSURANCE_L5 );
	}
#else
		AddEmailWithSpecialData( INSUR_SUSPIC_2, INSUR_SUSPIC_2_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif
	}

	if ( gMercProfiles[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID ].ubSuspiciousDeath == VERY_SUSPICIOUS_DEATH )
	{
		// the fact that you tried to cheat them gets realized very quickly. :-)
		ubDays = 1;
	}
	else
	{
		// calculate how many days the investigation will take
		ubDays = (UINT8) (2 + gStrategicStatus.ubInsuranceInvestigationsCnt + Random(3));		// 2-4 days, +1 for every previous investigation
	}

	// post an event to end the investigation that many days in the future (at 4pm)
	AddStrategicEvent( EVENT_INSURANCE_INVESTIGATION_OVER, GetMidnightOfFutureDayInMinutes( ubDays ) + 16 * 60, ubPayoutID );

	// increment counter of all investigations
	gStrategicStatus.ubInsuranceInvestigationsCnt++;
}


void EndInsuranceInvestigation( UINT8	ubPayoutID )
{
	// send an email telling player the investigation is over
	if ( gMercProfiles[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID ].ubSuspiciousDeath == VERY_SUSPICIOUS_DEATH )
	{
		// fraud, no payout!
#ifdef JA2UB
// no UB
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			AddEmailWithSpecialData( INSUR_1HOUR_FRAUD, INSUR_1HOUR_FRAUD_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT, TYPE_E_INSURANCE_L2 );
	}		
#else
		AddEmailWithSpecialData( INSUR_1HOUR_FRAUD, INSUR_1HOUR_FRAUD_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif
	}
	// Flugente: also don't pay out if the death was suspicious. I mean, we get this if there were no enemies of the player straight up shot the guy...
	else if ( gMercProfiles[LaptopSaveInfo.pLifeInsurancePayouts[ubPayoutID].ubMercID].ubSuspiciousDeath == SUSPICIOUS_DEATH )
	{
#ifdef JA2UB
		// WANNE: I really don't know if we should call something here. At least it fixed the compilation error when compiling UB version.
#else
		// fraud, no payout!
		AddEmailWithSpecialData( INSUR_CHEAT_FRAUD, INSUR_CHEAT_FRAUD_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ubPayoutID].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ubPayoutID].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif 
	}
	else
	{
#ifdef JA2UB
// No UB
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			AddEmailWithSpecialData( 176, INSUR_INVEST_OVER_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT, TYPE_E_INSURANCE_L6 );
	}
#else
		AddEmailWithSpecialData( INSUR_INVEST_OVER, INSUR_INVEST_OVER_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif

		// only now make a payment (immediately)
		InsuranceContractPayLifeInsuranceForDeadMerc( ubPayoutID );
	}
}


//void InsuranceContractPayLifeInsuranceForDeadMerc( LIFE_INSURANCE_PAYOUT *pPayoutStruct )
void InsuranceContractPayLifeInsuranceForDeadMerc( UINT8 ubPayoutID )
{
	//if the mercs id number is the same what is in the soldier array
	if( LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubSoldierID == Menptr[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubSoldierID ].ubID )
	{
		// and if the soldier is still active ( player hasn't removed carcass yet ), reset insurance flag
		if( Menptr[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubSoldierID ].bActive )
			Menptr[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubSoldierID ].usLifeInsurance = 0;
	}

	//add transaction to players account
	AddTransactionToPlayersBook( INSURANCE_PAYOUT, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice );

	//add to the history log the fact that the we paid the insurance claim
	AddHistoryToPlayersLog( HISTORY_INSURANCE_CLAIM_PAYOUT, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, GetWorldTotalMin(), -1, -1 );

	//if there WASNT an investigation
	if( gMercProfiles[ LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID ].ubSuspiciousDeath == 0 )
	{
		//Add an email telling the user that he received an insurance payment
#ifdef JA2UB
// no UB
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			AddEmailWithSpecialData( 170, INSUR_PAYMENT_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT, TYPE_E_INSURANCE_L3 );
	}
#else
		AddEmailWithSpecialData( INSUR_PAYMENT, INSUR_PAYMENT_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].iPayOutPrice, LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].ubMercID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif
	}

	LaptopSaveInfo.ubNumberLifeInsurancePayoutUsed --;
	LaptopSaveInfo.pLifeInsurancePayouts[ ubPayoutID ].fActive = FALSE;
//	MemFree( pPayoutStruct );
}


//Gets called at the very end of the game
void InsuranceContractEndGameShutDown()
{
	//Free up the memory allocated to the insurance payouts
	if( LaptopSaveInfo.pLifeInsurancePayouts )
	{
		MemFree( LaptopSaveInfo.pLifeInsurancePayouts );
		LaptopSaveInfo.pLifeInsurancePayouts = NULL;
	}
}


BOOLEAN MercIsInsurable( SOLDIERTYPE *pSoldier )
{
	// only A.I.M. mercs currently on player's team
	if( ( pSoldier->bActive ) && ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) )
	{
		// with more than one day left on their employment contract are eligible for insurance
		// the second part is because the insurance doesn't pay for any working day already started at time of death
//		if( ( (pSoldier->iEndofContractTime - GetWorldTotalMin()) > 24 * 60) || pSoldier->usLifeInsurance )
		if( CanSoldierExtendInsuranceContract( pSoldier ) || pSoldier->usLifeInsurance )
		{
			// who aren't currently being held POW
			// POWs are also uninsurable - if already insured, that insurance IS valid but no new contracts or extension allowed
			if (pSoldier->bAssignment != ASSIGNMENT_POW)
			{
				return(TRUE);
			}
		}
	}

	return(FALSE);
}


void EnableDisableInsuranceContractAcceptButtons()
{
	//If it is the first grid
	if( gubNumberofDisplayedInsuranceGrids >= 1 )
	{
		EnableDisableIndividualInsuranceContractButton( gubMercIDForMercInForm1, &guiInsuranceAcceptClearForm1Button );
	}

	//If it is the 2nd grid
	if( gubNumberofDisplayedInsuranceGrids >= 2 )
	{
		EnableDisableIndividualInsuranceContractButton( gubMercIDForMercInForm2, &guiInsuranceAcceptClearForm2Button );
	}

	//If it is the 3rd grid
	if( gubNumberofDisplayedInsuranceGrids >= 3 )
	{
		EnableDisableIndividualInsuranceContractButton( gubMercIDForMercInForm3, &guiInsuranceAcceptClearForm3Button );
	}
}


void EnableDisableIndividualInsuranceContractButton( UINT8 ubMercIDForMercInForm, UINT32 *puiAcceptButton )
{
	INT16	sSoldierID = 0;

	sSoldierID = GetSoldierIDFromMercID( ubMercIDForMercInForm );
	if( sSoldierID == - 1)
		return;

	// if the soldiers contract can be extended, enable the button
	if( CanSoldierExtendInsuranceContract( &Menptr[ sSoldierID ] ) )
		EnableButton( *puiAcceptButton );

	// else the soldier cant extend their insurance contract, disable the button
	else
		DisableButton( *puiAcceptButton );

	//if the merc is dead, disable the button
	if( IsMercDead( ubMercIDForMercInForm ) )
		DisableButton( *puiAcceptButton );
}


/*
UINT32	GetContractLengthForFormNumber( UINT8 ubFormID )
{
	UINT8	ubMercID;
	SOLDIERTYPE	*pSoldier;

	switch( ubFormID )
	{
		case 0:
			ubMercID = gubMercIDForMercInForm1;
			break;
		case 1:
			ubMercID = gubMercIDForMercInForm1;
			break;
		case 2:
			ubMercID = gubMercIDForMercInForm1;
			break;

		default:
			Assert( 0 );
			break;
	}

	pSoldier = &Menptr[ GetSoldierIDFromMercID( ubMercID ) ];

	return( pSoldier->iTotalContractLength );
}
*/

UINT32	GetTimeRemainingOnSoldiersInsuranceContract( SOLDIERTYPE *pSoldier )
{
	//if the soldier has life insurance
	if( pSoldier->usLifeInsurance )
	{
		//if the insurance contract hasnt started yet
		if( (INT32)GetWorldDay() < pSoldier->iStartOfInsuranceContract )
			return( pSoldier->iTotalLengthOfInsuranceContract );
		else
			return( ( pSoldier->iTotalLengthOfInsuranceContract - ( GetWorldDay() - pSoldier->iStartOfInsuranceContract ) ) );
	}
	else
		return( 0 );
}

UINT32	GetTimeRemainingOnSoldiersContract( SOLDIERTYPE *pSoldier )
{
	INT32 iDayMercLeaves = ( pSoldier->iEndofContractTime / 1440 ) - 1;

	//Since the merc is leaving in the afternoon, we must adjust since the time left would be different if we did the calc
	//at 11:59 or 12:01 ( noon )
	if( pSoldier->iEndofContractTime % 1440 )
		iDayMercLeaves++;

	// Subtract todays day number
	iDayMercLeaves = iDayMercLeaves - GetWorldDay();

	if( iDayMercLeaves > pSoldier->iTotalContractLength )
		iDayMercLeaves = pSoldier->iTotalContractLength;

	return( iDayMercLeaves );
//	return( ( pSoldier->iEndofContractTime - (INT32)GetWorldTotalMin( ) ) / 1440 );
}


void PurchaseOrExtendInsuranceForSoldier( SOLDIERTYPE *pSoldier, UINT32 uiInsuranceLength )
{
	INT32	iAmountOfMoneyTransfer = -1;

	if( pSoldier == NULL )
		AssertMsg( 0, "Soldier pointer is NULL!" );

	//if the user doesnt have insruance already,
	if( !(pSoldier->usLifeInsurance ) )
	{
		//specify the start date of the contract
		pSoldier->iStartOfInsuranceContract = CalcStartDayOfInsurance( pSoldier );
		pSoldier->uiStartTimeOfInsuranceContract = GetWorldTotalMin();
	}

	//transfer money
	iAmountOfMoneyTransfer = CalculateInsuranceContractCost( uiInsuranceLength, pSoldier->ubProfile );

	//if the user did have insruance already,
	if( pSoldier->usLifeInsurance )
	{
		//specify the start date of the contract
		pSoldier->iStartOfInsuranceContract = CalcStartDayOfInsurance( pSoldier );
	}

	//add transaction to finaces page
	//if the player has life insurance
	if( pSoldier->usLifeInsurance )
	{
		//if the player is extending the contract
		if( iAmountOfMoneyTransfer > 0 )
			AddTransactionToPlayersBook(	EXTENDED_INSURANCE, pSoldier->ubProfile, GetWorldTotalMin(), -( iAmountOfMoneyTransfer ) );
		else
			Assert(0);
	}
	else
	{
		//if the player doesnt have enough money, tell him
		if( LaptopSaveInfo.iCurrentBalance < iAmountOfMoneyTransfer )
		{
			CHAR16		sText[800];

			GetInsuranceText( INS_MLTI_NOT_ENOUGH_FUNDS, sText );
			if( guiCurrentScreen == LAPTOP_SCREEN )
				DoLapTopMessageBox( MSG_BOX_RED_ON_WHITE, sText, LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
			else
				DoMapMessageBox( MSG_BOX_RED_ON_WHITE, sText, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		}
		else
		{
			//else if the player has enought to cover the bill, let him

			//the player just purchased life insurance
			AddTransactionToPlayersBook(	PURCHASED_INSURANCE, pSoldier->ubProfile, GetWorldTotalMin(), -( iAmountOfMoneyTransfer ) );

			//add an entry in the history page for the purchasing of life insurance
			AddHistoryToPlayersLog(HISTORY_PURCHASED_INSURANCE, pSoldier->ubProfile, GetWorldTotalMin(), -1, -1 );

			//Set that we have life insurance
			pSoldier->usLifeInsurance = 1;
		}
	}

	pSoldier->iTotalLengthOfInsuranceContract += uiInsuranceLength;

	//make sure the length doesnt exceed the contract length
	if( ( GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) ) > GetTimeRemainingOnSoldiersContract( pSoldier ) )
	{
		pSoldier->iTotalLengthOfInsuranceContract -= GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) - GetTimeRemainingOnSoldiersContract( pSoldier );
	}
}

BOOLEAN	CanSoldierExtendInsuranceContract( SOLDIERTYPE *pSoldier )
{
	if( CalculateSoldiersInsuranceContractLength( pSoldier ) != 0 )
		return( TRUE );
	else
		return( FALSE );
}


INT32 CalculateSoldiersInsuranceContractLength( SOLDIERTYPE *pSoldier )
{
	INT32 iInsuranceContractLength=0;
	UINT32 uiTimeRemainingOnSoldiersContract = GetTimeRemainingOnSoldiersContract( pSoldier );


	//if the merc is dead
	if( IsMercDead( pSoldier->ubProfile ) )
		return( 0 );


	// only mercs with at least 2 days to go on their employment contract are insurable
	// def: 2/5/99.	However if they already have insurance is SHOULD be ok
	if( uiTimeRemainingOnSoldiersContract < 2 && !( pSoldier->usLifeInsurance != 0 && uiTimeRemainingOnSoldiersContract >= 1 ) )
	{
		return( 0 );
	}

	//
	//Calculate the insurance contract length
	//

	//if the soldier has an insurance contract, dont deduct a day
	if( pSoldier->usLifeInsurance || DidGameJustStart() )
		iInsuranceContractLength = uiTimeRemainingOnSoldiersContract - GetTimeRemainingOnSoldiersInsuranceContract( pSoldier );

	//else deduct a day
	else
		iInsuranceContractLength = uiTimeRemainingOnSoldiersContract - GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) - 1;

	//make sure the length doesnt exceed the contract length
	if( ( GetTimeRemainingOnSoldiersInsuranceContract( pSoldier ) + iInsuranceContractLength ) > uiTimeRemainingOnSoldiersContract )
	{
		iInsuranceContractLength = uiTimeRemainingOnSoldiersContract - GetTimeRemainingOnSoldiersInsuranceContract( pSoldier );
	}

	//Is the mercs insurace contract is less then a day, set it to 0
	if( iInsuranceContractLength < 0 )
		iInsuranceContractLength = 0;

	if( pSoldier->usLifeInsurance && pSoldier->iStartOfInsuranceContract >= (INT32)GetWorldDay() && iInsuranceContractLength < 2 )
		iInsuranceContractLength = 0;

	return( iInsuranceContractLength );
}

INT32	CalcStartDayOfInsurance( SOLDIERTYPE *pSoldier )
{
	UINT32	uiDayToStartInsurance=0;

	//if the soldier was just hired ( in transit ), and the game didnt just start
	if( pSoldier->bAssignment == IN_TRANSIT && !DidGameJustStart() )
	{
		uiDayToStartInsurance = GetWorldDay( );
	}
	else
	{
		//Get tomorows date ( and convert it to days )
		uiDayToStartInsurance = GetMidnightOfFutureDayInMinutes( 1 ) / 1440;
	}

	return( uiDayToStartInsurance );
}

BOOLEAN AreAnyAimMercsOnTeam( )
{
	SOLDIERTYPE *pSoldier = NULL;

	for( int cnt = 0; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++cnt )
	{
		pSoldier = MercPtrs[cnt];

		//check to see if any of the mercs are AIM mercs
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
		{
			return TRUE;
		}
	}

	return FALSE;
}
