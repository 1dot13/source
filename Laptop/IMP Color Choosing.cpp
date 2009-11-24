#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Color Choosing Skin Hair.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Color Choosing.h"
	#include "Button System.h"
	#include "utilities.h"
	#include "Debug.h"
	#include "Text.h"
	#include "Font Control.h"
	#include "font.h"
	#include "laptop.h"
	#include "cursors.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "_Ja25EnglishText.h"
	#include "wordwrap.h"
	#include "CharProfile.h"
	#include "soldier profile type.h"
	#include "IMP Compile Character.h"
	#include "IMP Portraits.h"
	#include "IMP Begin Screen.h"
	#include "WCheck.h"
	#include "Animation Data.h"
#endif

enum
{ 
	PINKSKIN, 
	TANSKIN, 
	DARKSKIN, 
	BLACKSKIN, 
	NUMSKINS 
};
enum
{ 
	BROWNHEAD, 
	BLACKHEAD,
	WHITEHEAD,
	BLONDHEAD, 
	REDHEAD,
	NUMHEADS
};

enum
{
	WHITEVEST,
	GYELLOWSHIRT,
	YELLOWVEST,
	GREYVEST,
	BROWNVEST,
	PURPLESHIRT,
	BLUEVEST,
	JEANVEST,
	GREENVEST,
	REDVEST,
	BLACKSHIRT,
	NUMSHIRTS
};

enum
{
	BLUEPANTS,
	BLACKPANTS,
	JEANPANTS,
	TANPANTS,
	BEIGEPANTS,
	GREENPANTS,
	NUMPANTS
};

// Skin colors
/*STR16 sSkinTexts[]={
	L"Pink Skin",
	L"Tan Skin",
	L"Dark Skin",
	L"Black Skin",
	L"none",
};
 
// Hair colors
STR16 sHairTexts[]={
	L"Brown Head",
	L"Black Head",
	L"White Head",
	L"Blond Head",
	L"Red Head",
	L"none",
};

// Shirt colors
STR16 sShirtTexts[]={
	L"White Shirt",
	L"Green-Yellow Shirt",
	L"Yellow Shirt",
	L"Grey Shirt",
	L"Brown Shirt",
	L"Purple Shirt",
	L"Blue Shirt",
	L"Jean Shirt",
	L"Green Shirt",
	L"Red Shirt",
	L"Black Shirt",
	L"none",
};

// Pant colors
STR16 sPantsTexts[]={
	L"Blue Pants",
	L"Black Pants",
	L"Jean Pants",
	L"Tan Pants",
	L"Beige Pants",
	L"Green Pants",
	L"none",
};

STR16 sExplanationTexts[]=
{
	L"Hair Color",
	L"Skin Color",
	L"Shirt Color",
	L"Pants Color",
	L"Normal Body",
	L"Big Body",
};*/

//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define			IMP_COLOR_CHOOSING__TITLE_FONT				FONT14ARIAL

#define			IMP_COLOR_CHOOSING__FONT							FONT12ARIAL
#define			IMP_COLOR_CHOOSING__COLOR						FONT_MCOLOR_WHITE


/*enum
{
	IMP_COLOR_CHOOSING__TITLE_TEXT=NUMSKINS,
};*/


//#define	IMP_COLORS_COLUMN_CENTER_X								( LAPTOP_SCREEN_UL_X + 220 )
//#define	IMP_COLORS_COLUMN_CENTER_Y								( LAPTOP_SCREEN_UL_Y + 130 )

#define	IMP_COLORS_BOX_POS_X								( LAPTOP_SCREEN_UL_X + 194 )
#define	IMP_COLORS_BOX_POS_Y								( LAPTOP_SCREEN_UL_Y + 120 )

#define	IMP_BODY_TYPE_BTN_POS_X									( LAPTOP_SCREEN_UL_X + 123 )
#define	IMP_BODY_TYPE_BTN_POS_Y									( LAPTOP_SCREEN_UL_Y + 268 )


//*******************************************************************
//
// Global Variables
//
//*******************************************************************

INT32 iCurrentSkin = 0;
INT32 iCurrentHair = 0;
INT32 iCurrentShirt = 0;
INT32 iCurrentPants = 0;

BOOLEAN gfIST_Redraw4=FALSE;

BOOLEAN fReDrawColorScreenFlag;

BOOLEAN bBigBody;
BOOLEAN bBadAss;

// these are the buttons for the arrows
INT32 giIMPColorChoiceButton[ 11 ];
INT32 giIMPColorChoiceButtonImage[ 11 ];

// this is the Done	buttons
INT32 giIMPColorChoiceFinsihButton;
INT32 giIMPColorChoiceFinsihButtonImage;

//image handle
UINT32	guiIST_GreyGoldBox4;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPColorChoiceFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		AddIMPColorChoiceButtons();
void		IMPColorChoiceDisplayBoxes();
void		BtnIMPSkinChoiceNextCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPSkinChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPHairChoiceNextCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPHairChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPShirtChoiceNextCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPShirtChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPPantsChoiceNextCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPPantsChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason);
void		FixRanges( void );
void		HandleIMPNormalAndBigBodyButtons( BOOLEAN bPressedBigBody );
void		BtnIMPNormalBodyCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPBigBodyCallback(GUI_BUTTON *btn,INT32 reason);
void		BtnIMPToggleBadAssOption(GUI_BUTTON *btn,INT32 reason);
void		CreateBadAssToggleButton();
void		DextroyBadAssToggleButton();
BOOLEAN		RenderFigure();

//*******************************************************************
//
// Functions
//
//*******************************************************************



void EnterIMPColorChoice( void )
{
//	UINT32 uiCnt;
	VOBJECT_DESC	VObjectDesc;
	bBigBody = FALSE;
	bBadAss = FALSE;

	// Based on the portrait chosen, set the default colors, when colors
	switch( iPortraitNumber )
	{
		case( 0 ):
			iCurrentSkin = 3;
			iCurrentHair = 0;
			iCurrentShirt = 7;
			iCurrentPants = 2;
			bBigBody = TRUE;
		break;
		case( 1 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 4;
			iCurrentPants = 3;
		break;
		case( 2 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 0;
			iCurrentPants = 0;
		break;
		case( 3 ):
			iCurrentSkin = 2;
			iCurrentHair = 0;
			iCurrentShirt = 0;
			iCurrentPants = 1;
		break;
		case( 4 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 3;
			iCurrentPants = 5;
		break;
		case( 5 ):
			iCurrentSkin = 2;
			iCurrentHair = 1;
			iCurrentShirt = 3;
			iCurrentPants = 1;
			break;
		case( 6 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 4;
			iCurrentPants = 0;
			bBigBody = TRUE;
			break;
		case( 7 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 10;
			iCurrentPants = 1;
			bBigBody = TRUE;
			break;
		case( 8 ):
			iCurrentSkin = 1;
			iCurrentHair = 0;
			iCurrentShirt = 4;
			iCurrentPants = 3;
		break;
		case( 9 ):
			iCurrentSkin = 0;
			iCurrentHair = 0;
			iCurrentShirt = 4;
			iCurrentPants = 4;
		break;
		case( 10 ):
			iCurrentSkin = 1;
			iCurrentHair = 1;
			iCurrentShirt = 4;
			iCurrentPants = 1;
		break;
		case( 11 ):
			iCurrentSkin = 1;
			iCurrentHair = 1;
			iCurrentShirt = 10;
			iCurrentPants = 1;
		break;
		case( 12 ):
			iCurrentSkin = 0;
			iCurrentHair = 0;
			iCurrentShirt = 0;
			iCurrentPants = 5;
		break;
		case( 13 ):
			iCurrentSkin = 3;
			iCurrentHair = 0;
			iCurrentShirt = 3;
			iCurrentPants = 1;
			break;
		case( 14 ):
			iCurrentSkin = 1;
			iCurrentHair = 4;
			iCurrentShirt = 0;
			iCurrentPants = 2;
			break;
		case( 15 ):
			iCurrentSkin = 1;
			iCurrentHair = 3;
			iCurrentShirt = 4;
			iCurrentPants = 5;
			break;
	}

	//add the buttons
	AddIMPColorChoiceButtons();

	// load the stats graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\SkillTraitSmallGreyIdent.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiIST_GreyGoldBox4 ) )
	{
		Assert( 0 );
		return;
	}



	giIMPColorChoiceFinsihButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPColorChoiceFinsihButton = CreateIconAndTextButton( giIMPColorChoiceFinsihButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPColorChoiceFinishCallback );



	SetButtonCursor( giIMPColorChoiceFinsihButton, CURSOR_WWW);

}


void RenderIMPColorChoice( void )
{
	

	//render the metal background graphic
	RenderProfileBackGround();

	IMPColorChoiceDisplayBoxes();

	RenderAttrib1IndentFrame( 115, 42);

	// text - different for males and females
	if ( fCharacterIsMale )
	{
		DisplayWrappedString( LAPTOP_SCREEN_UL_X + 127, LAPTOP_SCREEN_WEB_UL_Y + 50,  ( 426 - 200 ), 2, FONT10ARIAL, 142, gzIMPColorChoosingText[2],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

		// title
		DrawTextToScreen( gzIMPColorChoosingText[0], LAPTOP_SCREEN_UL_X - 111, iScreenHeightOffset + 53, ( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X ), FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	else
	{
		DisplayWrappedString( LAPTOP_SCREEN_UL_X + 127, LAPTOP_SCREEN_WEB_UL_Y + 50,  ( 426 - 200 ), 2, FONT10ARIAL, 142, gzIMPColorChoosingText[3],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

		// title
		DrawTextToScreen( gzIMPColorChoosingText[1], LAPTOP_SCREEN_UL_X - 111, iScreenHeightOffset + 53, ( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X ), FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}

	// figure and frame
	RenderFigure();
	// text in the frame
	DrawTextToScreen( sColorChoiceExplanationTexts[ 0 ], IMP_COLORS_BOX_POS_X - 101, IMP_COLORS_BOX_POS_Y + 23, ( 100 ), FONT12ARIAL, FONT_WHITE, FONT_BLACK, FALSE, RIGHT_JUSTIFIED );
	DrawTextToScreen( sColorChoiceExplanationTexts[ 1 ], IMP_COLORS_BOX_POS_X - 101, IMP_COLORS_BOX_POS_Y + 50, ( 100 ), FONT12ARIAL, FONT_WHITE, FONT_BLACK, FALSE, RIGHT_JUSTIFIED );
	DrawTextToScreen( sColorChoiceExplanationTexts[ 2 ], IMP_COLORS_BOX_POS_X - 101, IMP_COLORS_BOX_POS_Y + 77, ( 100 ), FONT12ARIAL, FONT_WHITE, FONT_BLACK, FALSE, RIGHT_JUSTIFIED );
	DrawTextToScreen( sColorChoiceExplanationTexts[ 3 ], IMP_COLORS_BOX_POS_X - 101, IMP_COLORS_BOX_POS_Y + 105, ( 100 ), FONT12ARIAL, FONT_WHITE, FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

}
BOOLEAN RenderFigure()
{
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT hHandle;
	UINT32 uiGraphicHandle;
	UINT16 usPosX, usPosY;

	usPosX = IMP_COLORS_BOX_POS_X + 31;
	usPosY = IMP_COLORS_BOX_POS_Y + 13;

	// load it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	// Pick images based on body type
	if ( fCharacterIsMale )
	{
		if ( bBigBody == TRUE )
			FilenameForBPP( "Laptop\\IMPBigMercImages.sti" , VObjectDesc.ImageFile);
		else
			FilenameForBPP( "Laptop\\IMPSmallMercImages.sti" , VObjectDesc.ImageFile);
	}
	else
	{
		FilenameForBPP( "Laptop\\IMPFemaleMercImages.sti" , VObjectDesc.ImageFile);
	}

	// safety check if loaded
	CHECKF(AddVideoObject(&VObjectDesc, &uiGraphicHandle));

	// Get it
	GetVideoObject(&hHandle, uiGraphicHandle);

	// show background first
	BltVideoObject(FRAME_BUFFER, hHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

	// show skin (1 - 4)
	BltVideoObject(FRAME_BUFFER, hHandle, (iCurrentSkin + 1), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	// show head (5 - 9)
	BltVideoObject(FRAME_BUFFER, hHandle, (iCurrentHair + 5 ), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	// show vest (10 - 20)
	BltVideoObject(FRAME_BUFFER, hHandle, (iCurrentShirt + 10 ), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);
	// show pants (21 - 26)
	BltVideoObject(FRAME_BUFFER, hHandle, (iCurrentPants + 21 ), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

	DeleteVideoObjectFromIndex( uiGraphicHandle );

	// Add the frame around
	RenderColorChoiceFrame( (usPosX - 114), (usPosY - 13) );

	return ( TRUE );
}

void ExitIMPColorChoice( void )
{
	INT32 iCnt;

	DeleteVideoObjectFromIndex( guiIST_GreyGoldBox4 );

	//remove the skin buttons
	for(iCnt = 0; iCnt < ( fCharacterIsMale ? 11 : 8); iCnt++) // fix of bug, removing unasigned buttons when female
	{
		//if there is a button allocated
		if( giIMPColorChoiceButton[iCnt] != -1 )
		{
			RemoveButton(giIMPColorChoiceButton[ iCnt ] );
			UnloadButtonImage(giIMPColorChoiceButtonImage[ iCnt ] );
		}
	}

	RemoveButton( giIMPColorChoiceFinsihButton );
	UnloadButtonImage( giIMPColorChoiceFinsihButtonImage );
}


void HandleIMPColorChoice( void )
{
	if( fReDrawColorScreenFlag )
	{
		RenderIMPColorChoice( );
		fReDrawColorScreenFlag = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void AddIMPColorChoiceButtons()
{
	INT32 iCnt = 0;
	UINT16 usNextPosX, usNextPosY, usPrevPosX, usPrevPosY;

	usNextPosX = IMP_COLORS_BOX_POS_X + 137;
	usNextPosY = IMP_COLORS_BOX_POS_Y + 18;
	usPrevPosX = IMP_COLORS_BOX_POS_X + 10;
	usPrevPosY = IMP_COLORS_BOX_POS_Y + 18;

//************************************************************************************************************************
// HAIR BUTTONS
///////////////////////////////////////

	// next button HAIR
	giIMPColorChoiceButtonImage[ 0 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,3,-1,4,-1 );

	giIMPColorChoiceButton[ 0 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 0 ], usNextPosX, usNextPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPHairChoiceNextCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[0], 0, 0 );
	SetButtonCursor( giIMPColorChoiceButton[0], CURSOR_WWW);

	// previous button HAIR
	giIMPColorChoiceButtonImage[ 1 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,0,-1,1,-1 );

	giIMPColorChoiceButton[ 1 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 1 ], usPrevPosX, usPrevPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPHairChoicePreviousCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[1], 0, 1 );
	SetButtonCursor( giIMPColorChoiceButton[1], CURSOR_WWW);


	usNextPosY += 27;
	usPrevPosY += 27;

//************************************************************************************************************************
// SKIN BUTTONS
///////////////////////////////////////

	// Next button SKIN
	giIMPColorChoiceButtonImage[ 2 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,3,-1,4,-1 );

	giIMPColorChoiceButton[ 2 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 2 ], usNextPosX, usNextPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPSkinChoiceNextCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[2], 0, 2 );
	SetButtonCursor( giIMPColorChoiceButton[2], CURSOR_WWW);

	// previous button SKIN
	giIMPColorChoiceButtonImage[ 3 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,0,-1,1,-1 );

	giIMPColorChoiceButton[ 3 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 3 ], usPrevPosX, usPrevPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPSkinChoicePreviousCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[3], 0, 3 );
	SetButtonCursor( giIMPColorChoiceButton[3], CURSOR_WWW);

	usNextPosY += 27;
	usPrevPosY += 27;


//************************************************************************************************************************
// SHIRT BUTTONS
///////////////////////////////////////

	// next button SHIRT
	giIMPColorChoiceButtonImage[ 4 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,3,-1,4,-1 );

	giIMPColorChoiceButton[ 4 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 4 ], usNextPosX, usNextPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPShirtChoiceNextCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[4], 0, 4 );
	SetButtonCursor( giIMPColorChoiceButton[4], CURSOR_WWW);


	// previous button SHIRT
	giIMPColorChoiceButtonImage[ 5 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,0,-1,1,-1 );

	giIMPColorChoiceButton[ 5 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 5 ], usPrevPosX, usPrevPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPShirtChoicePreviousCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[5], 0, 5 );
	SetButtonCursor( giIMPColorChoiceButton[5], CURSOR_WWW);

	usNextPosY += 27;
	usPrevPosY += 27;

//************************************************************************************************************************
// PANTS BUTTONS
///////////////////////////////////////

	// next button PANTS
	giIMPColorChoiceButtonImage[ 6 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,3,-1,4,-1 );

	giIMPColorChoiceButton[ 6 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 6 ], usNextPosX, usNextPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPantsChoiceNextCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[6], 0, 6 );
	SetButtonCursor( giIMPColorChoiceButton[6], CURSOR_WWW);

	// previous button PANTS
	giIMPColorChoiceButtonImage[ 7 ]=	LoadButtonImage( "LAPTOP\\IMPARROWSSANDRO.sti" ,-1,0,-1,1,-1 );

	giIMPColorChoiceButton[ 7 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 7 ], usPrevPosX, usPrevPosY,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPantsChoicePreviousCallback );

	//Set the button data
	MSYS_SetBtnUserData( giIMPColorChoiceButton[7], 0, 7 );
	SetButtonCursor( giIMPColorChoiceButton[7], CURSOR_WWW);


//************************************************************************************************************************
// BODY TYPE BUTTONS
///////////////////////////////////////

	// only do this if we are a male character
	if (fCharacterIsMale)
	{
		// Normal body button
		giIMPColorChoiceButtonImage[ 8 ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );

		giIMPColorChoiceButton[ 8 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 8 ], IMP_BODY_TYPE_BTN_POS_X, IMP_BODY_TYPE_BTN_POS_Y,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPNormalBodyCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPColorChoiceButton[8], 0, 8 );
		SetButtonCursor( giIMPColorChoiceButton[8], CURSOR_WWW);

		// Big body button
		giIMPColorChoiceButtonImage[ 9 ] =	LoadButtonImage( "LAPTOP\\button_6.sti", -1,0,-1,1,-1 );

		giIMPColorChoiceButton[ 9 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 9 ], IMP_BODY_TYPE_BTN_POS_X, IMP_BODY_TYPE_BTN_POS_Y + 38,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPBigBodyCallback);

		//Set the button data
		MSYS_SetBtnUserData( giIMPColorChoiceButton[9], 0, 9 );
		SetButtonCursor( giIMPColorChoiceButton[9], CURSOR_WWW);

		// Badass toggle button
		giIMPColorChoiceButtonImage[ 10 ] =	LoadButtonImage( "EDITOR//SmCheckbox.sti", 1,0,-1,2,-1 );

		giIMPColorChoiceButton[ 10 ] = QuickCreateButton( giIMPColorChoiceButtonImage[ 10 ], IMP_BODY_TYPE_BTN_POS_X + 230, IMP_BODY_TYPE_BTN_POS_Y + 49,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
									MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPToggleBadAssOption );

		MSYS_SetBtnUserData( giIMPColorChoiceButton[10], 0, 10 );
		SetButtonCursor( giIMPColorChoiceButton[10], CURSOR_WWW);
		SetButtonFastHelpText( giIMPColorChoiceButton[ 10 ], gzIMPColorChoosingText[4]);


		// Make respective button initialy pressed
		if ( bBigBody )
		{
			ButtonList[ giIMPColorChoiceButton[ 9 ] ]->uiFlags |= BUTTON_CLICKED_ON;

			// enable badass option
			EnableButton( giIMPColorChoiceButton[ 10 ] );
		}
		else
		{
			ButtonList[ giIMPColorChoiceButton[ 8 ] ]->uiFlags |= BUTTON_CLICKED_ON;

			// disable badass option
			DisableButton( giIMPColorChoiceButton[ 10 ] );
		}
	}

}

void BtnIMPToggleBadAssOption(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if ( bBadAss || (btn->uiFlags & BUTTON_CLICKED_ON))
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
			bBadAss = FALSE;
		}
		else if ( !bBadAss && !(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			btn->uiFlags |= (BUTTON_CLICKED_ON);
			bBadAss = TRUE;
		}
	}
}
void BtnIMPSkinChoiceNextCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentSkin++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentSkin++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnIMPSkinChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentSkin--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentSkin--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}
void BtnIMPHairChoiceNextCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentHair++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentHair++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnIMPHairChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentHair--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentHair--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}
void BtnIMPShirtChoiceNextCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentShirt++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentShirt++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnIMPShirtChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentShirt--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentShirt--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}
void BtnIMPPantsChoiceNextCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentPants++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentPants++;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnIMPPantsChoicePreviousCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		iCurrentPants--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		iCurrentPants--;
		FixRanges();

		fReDrawColorScreenFlag = TRUE;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void FixRanges()
{
	// fix skin number
	if (iCurrentSkin >= NUMSKINS )
	{
		iCurrentSkin = 0;
	}
	else if (iCurrentSkin < 0 )
	{
		iCurrentSkin = NUMSKINS - 1;
	}

	// fix hair number
	if (iCurrentHair >= NUMHEADS )
	{
		iCurrentHair = 0;
	}
	else if (iCurrentHair < 0 )
	{
		iCurrentHair = NUMHEADS - 1;
	}

	// fix shirt number
	if (iCurrentShirt >= NUMSHIRTS )
	{
		iCurrentShirt = 0;
	}
	else if (iCurrentShirt < 0 )
	{
		iCurrentShirt = NUMSHIRTS - 1;
	}

	// fix pants number
	if (iCurrentPants >= NUMPANTS )
	{
		iCurrentPants = 0;
	}
	else if (iCurrentPants < 0 )
	{
		iCurrentPants = NUMPANTS - 1;
	}
}
void HandleIMPNormalAndBigBodyButtons( BOOLEAN bPressedBigBody )
{
	if ( bPressedBigBody )
	{
		if( bBigBody )
		{
			// play sound
			PlayButtonSound( giIMPColorChoiceButton[ 9 ], BUTTON_SOUND_DISABLED_CLICK );
		}
		else
		{
			bBigBody = TRUE;

			// Deselect normal body button
			ButtonList[ giIMPColorChoiceButton[ 8 ] ]->uiFlags &= ~BUTTON_CLICKED_ON;

			// Select big body button
			ButtonList[ giIMPColorChoiceButton[ 9 ] ]->uiFlags |= BUTTON_CLICKED_ON;

			// play sound
			PlayButtonSound( giIMPColorChoiceButton[ 9 ], BUTTON_SOUND_CLICKED_OFF );

			// Enable badass option
			EnableButton( giIMPColorChoiceButton[ 10 ] );

			fReDrawColorScreenFlag = TRUE;
		}
	}
	else
	{
		if( bBigBody )
		{
			bBigBody = FALSE;

			// Select normal body button
			ButtonList[ giIMPColorChoiceButton[ 8 ] ]->uiFlags |= BUTTON_CLICKED_ON;

			// Deselect big body button
			ButtonList[ giIMPColorChoiceButton[ 9 ] ]->uiFlags &= ~BUTTON_CLICKED_ON;

			// play sound
			PlayButtonSound( giIMPColorChoiceButton[ 8 ], BUTTON_SOUND_CLICKED_OFF );

			// Disable Badass option
			DisableButton( giIMPColorChoiceButton[ 10 ] );

			fReDrawColorScreenFlag = TRUE;
		}
		else
		{
			// play sound
			PlayButtonSound( giIMPColorChoiceButton[ 8 ], BUTTON_SOUND_DISABLED_CLICK );
		}
	}

}
void BtnIMPNormalBodyCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		HandleIMPNormalAndBigBodyButtons( FALSE );
	}
}

void BtnIMPBigBodyCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		HandleIMPNormalAndBigBodyButtons( TRUE );
	}
}

void IMPColorChoiceDisplayBoxes()
{
	//UINT16 usPosX, usPosY;
	HVOBJECT	hImageHandle;

	// Stats
	GetVideoObject(&hImageHandle, guiIST_GreyGoldBox4 );

	if ( fCharacterIsMale )
	{
		if( bBigBody )
		{
			//Display the gold background box on NORMAL BODY
			BltVideoObject(FRAME_BUFFER, hImageHandle, 0, IMP_BODY_TYPE_BTN_POS_X + 65, IMP_BODY_TYPE_BTN_POS_Y + 5, VO_BLT_SRCTRANSPARENCY,NULL);
			BltVideoObject(FRAME_BUFFER, hImageHandle, 1, IMP_BODY_TYPE_BTN_POS_X + 65, IMP_BODY_TYPE_BTN_POS_Y + 5 + 38, VO_BLT_SRCTRANSPARENCY,NULL);
		}
		else
		{
			//Display the grey background box on BIG BODY
			BltVideoObject(FRAME_BUFFER, hImageHandle, 1, IMP_BODY_TYPE_BTN_POS_X + 65, IMP_BODY_TYPE_BTN_POS_Y + 5, VO_BLT_SRCTRANSPARENCY,NULL);
			BltVideoObject(FRAME_BUFFER, hImageHandle, 0, IMP_BODY_TYPE_BTN_POS_X + 65, IMP_BODY_TYPE_BTN_POS_Y + 5 + 38, VO_BLT_SRCTRANSPARENCY,NULL);
		}

		//draw the text to the screenx
		DrawTextToScreen( sColorChoiceExplanationTexts[4], IMP_BODY_TYPE_BTN_POS_X + 7 + 65, IMP_BODY_TYPE_BTN_POS_Y + 12, 0, IMP_COLOR_CHOOSING__FONT, IMP_COLOR_CHOOSING__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		DrawTextToScreen( sColorChoiceExplanationTexts[5], IMP_BODY_TYPE_BTN_POS_X + 7 + 65, IMP_BODY_TYPE_BTN_POS_Y + 12 + 38, 0, IMP_COLOR_CHOOSING__FONT, IMP_COLOR_CHOOSING__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	}

}


void BtnIMPColorChoiceFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		// finished here
		iCurrentImpPage = IMP_MAIN_PAGE;
		//fButtonPendingFlag = TRUE;
		if( iCurrentProfileMode != IMP__FINISH )
		{
			iCurrentProfileMode = IMP__VOICE; // CHANGED BY SANDRO ("VOICE" IS NOW CHARACTER)
		}
		else
		{
			iCurrentProfileMode = IMP__FINISH;
		}
	}
}
INT8 iChosenSkin()
{
	if ( iCurrentSkin >= 0 && iCurrentSkin < NUMSKINS )
		return( iCurrentSkin );
	else
		return( 0 );
}

INT8 iChosenHair()
{
	if ( iCurrentHair >= 0 && iCurrentHair < NUMHEADS )
		return( iCurrentHair );
	else
		return( 0 );
}
INT8 iChosenShirt()
{
	if ( iCurrentShirt >= 0 && iCurrentShirt < NUMSHIRTS )
		return( iCurrentShirt );
	else
		return( 0 );
}
INT8 iChosenPants()
{
	if ( iCurrentPants >= 0 && iCurrentPants < NUMPANTS )
		return( iCurrentPants );
	else
		return( 0 );
}

BOOLEAN bBigBodySelected()
{
	if ( fCharacterIsMale )
		return( bBigBody );
	else
		return( FALSE );
}

BOOLEAN bBadAssSelected()
{
	if ( fCharacterIsMale && bBigBody)
		return( bBadAss );
	else
		return( FALSE );
}