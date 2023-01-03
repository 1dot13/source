#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "aim.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "timer control.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "email.h"
	#include "Game Clock.h"
	#include "Text.h"
	#include "LaptopSave.h"
	#include "Multi Language Graphic Utils.h"
	#include "GameSettings.h"
	#include "english.h"
	#include "sysutil.h"
#endif

#include "LocalizedStrings.h"
#include "Soldier Profile.h"

UINT8 MAX_NUMBER_MERCS = 0;

UINT8			AimMercArray[ NUM_PROFILES ]; //MAX_NUMBER_MERCS ];

AIM_AVAILABLE_TEMP gAimAvailabilityTemp[ NUM_PROFILES ];
AIM_AVAILABLE gAimAvailability[ NUM_PROFILES ];

UINT8			gCurrentAimPage[ NUM_AIM_SCREENS ] =
{
	LAPTOP_MODE_AIM,
	LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES,
	LAPTOP_MODE_AIM_MEMBERS_ARCHIVES,
	LAPTOP_MODE_AIM_POLICIES,
	LAPTOP_MODE_AIM_HISTORY,
	LAPTOP_MODE_AIM_LINKS
};


//
//***	Defines **
//


#define			BOBBYR_UNDER_CONSTRUCTION_AD_FONT							FONT14HUMANIST//FONT16ARIAL
#define			BOBBYR_UNDER_CONSTRUCTION_AD_COLOR						FONT_MCOLOR_DKRED//FONT_MCOLOR_WHITE


// Link Images
#define			LINK_SIZE_X							101
#define			LINK_SIZE_Y							76

#define			MEMBERCARD_X						IMAGE_OFFSET_X + 118
#define			MEMBERCARD_Y						IMAGE_OFFSET_Y + 190

#define			POLICIES_X							IMAGE_OFFSET_X + 284
#define			POLICIES_Y							MEMBERCARD_Y

#define			HISTORY_X								MEMBERCARD_X
#define			HISTORY_Y								IMAGE_OFFSET_Y + 279

#define			LINKS_X									POLICIES_X
#define			LINKS_Y									HISTORY_Y

#define			WARNING_X								IMAGE_OFFSET_X + 126
#define			WARNING_Y							IMAGE_OFFSET_Y + 79

#define			MEMBERS_TEXT_Y					MEMBERCARD_Y + 77
#define			HISTORY_TEXT_Y					HISTORY_Y + 77
#define			POLICIES_TEXT_Y					MEMBERS_TEXT_Y
#define			LINK_TEXT_Y							HISTORY_TEXT_Y

#define			AIM_WARNING_TEXT_X			WARNING_X + 15
#define			AIM_WARNING_TEXT_Y			WARNING_Y + 46
#define			AIM_WARNING_TEXT_WIDTH	220

#define			AIM_FLOWER_LINK_TEXT_Y	AIM_WARNING_TEXT_Y + 25

#define			AIM_BOBBYR1_LINK_TEXT_X			WARNING_X + 20
#define			AIM_BOBBYR1_LINK_TEXT_Y			WARNING_Y + 20

#define			AIM_BOBBYR2_LINK_TEXT_X			WARNING_X + 50
#define			AIM_BOBBYR2_LINK_TEXT_Y			WARNING_Y + 58

#define			AIM_BOBBYR3_LINK_TEXT_X			WARNING_X + 20
#define			AIM_BOBBYR3_LINK_TEXT_Y			WARNING_Y + 20

#define			AIM_AD_TOP_LEFT_X				WARNING_X
#define			AIM_AD_TOP_LEFT_Y				WARNING_Y
#define			AIM_AD_BOTTOM_RIGHT_X		AIM_AD_TOP_LEFT_X + 248
#define			AIM_AD_BOTTOM_RIGHT_Y		AIM_AD_TOP_LEFT_Y + 110

#define			AIM_COPYRIGHT_X						iScreenWidthOffset + 160
#define			AIM_COPYRIGHT_Y						iScreenHeightOffset + 415 // + LAPTOP_SCREEN_WEB_DELTA_Y
#define			AIM_COPYRIGHT_WIDTH			400
#define			AIM_COPYRIGHT_GAP				9

#define			AIM_WARNING_TIME				10000
#define			AIM_ADVERTISING_DELAY		500

#define			AIM_FLOWER_AD_DELAY					150
#define			AIM_FLOWER_NUM_SUBIMAGES		16

#define			AIM_AD_FOR_ADS_DELAY					150
#define			AIM_AD_FOR_ADS__NUM_SUBIMAGES	13

#define			AIM_AD_INSURANCE_AD_DELAY					150
#define			AIM_AD_INSURANCE_AD__NUM_SUBIMAGES	10

#define			AIM_AD_FUNERAL_AD_DELAY					250
#define			AIM_AD_FUNERAL_AD__NUM_SUBIMAGES	9

#define			AIM_AD_BOBBYR_AD_STARTS								2
#define			AIM_AD_DAY_FUNERAL_AD_STARTS					4
#define			AIM_AD_DAY_FLOWER_AD_STARTS						7
#define			AIM_AD_DAY_INSURANCE_AD_STARTS				12

#define			AIM_AD_BOBBYR_AD_DELAY								300
#define			AIM_AD_BOBBYR_AD__NUM_SUBIMAGES				21
#define			AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES		6


//#define

enum
{
	AIM_AD_NOT_DONE,
	AIM_AD_DONE,
	AIM_AD_WARNING_BOX,
	AIM_AD_FOR_ADS,
	AIM_AD_BOBBY_RAY_AD,
	AIM_AD_FUNERAL_ADS,
	AIM_AD_FLOWER_SHOP,
	AIM_AD_INSURANCE_AD,
	AIM_AD_LAST_AD
};


// Aim Screen Handle
UINT32	guiAimSymbol;
UINT32		guiRustBackGround;
UINT32		guiMemberCard;
UINT32		guiPolicies;
UINT32		guiHistory;
UINT32		guiLinks;
UINT32		guiWarning;
extern UINT32		guiBottomButton; // symbol already declared globally in AimPolicies.cpp (jonathanl)
extern UINT32		guiBottomButton2; // symbol already declared globally in AimPolicies.cpp (jonathanl)
UINT32		guiFlowerAdvertisement;
UINT32		guiAdForAdsImages;
UINT32		guiInsuranceAdImages;
UINT32		guiFuneralAdImages;
UINT32		guiBobbyRAdImages;


UINT8			gubAimMenuButtonDown=255;
UINT32		gubWarningTimer;
UINT8			gubCurrentAdvertisment;

BOOLEAN		gfInitAdArea;

// MemberCard
MOUSE_REGION	gSelectedMemberCardRegion;
void SelectMemberCardRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


// Policies
MOUSE_REGION	gSelectedPoliciesRegion;
void SelectPoliciesRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//History
MOUSE_REGION	gSelectedHistoryRegion;
void SelectHistoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//Links
MOUSE_REGION	gSelectedLinksRegion;
void SelectLinksRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//Bottom Buttons
void BtnAimBottomButtonsCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiBottomButtons[ NUM_AIM_SCREENS ];
INT32		guiBottomButtonImage;

//Banner Area
MOUSE_REGION	gSelectedBannerRegion;
void SelectBannerRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//Aim logo click
MOUSE_REGION	gSelectedAimLogo;
void SelectAimLogoRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );



BOOLEAN	DrawWarningBox( BOOLEAN fInit, BOOLEAN fRedraw );
BOOLEAN DisplayFlowerAd( BOOLEAN fInit, BOOLEAN fRedraw );
void		HandleAdAndWarningArea( BOOLEAN fInit, BOOLEAN fRedraw );
void		LaptopInitAim();
BOOLEAN DisplayAd( BOOLEAN fInit, BOOLEAN fRedraw, UINT16 usDelay, UINT16 usNumberOfSubImages, UINT32 uiAdImageIdentifier );
void		HandleTextOnAimAdd( UINT8 ubCurSubImage );
BOOLEAN DisplayBobbyRAd( BOOLEAN fInit, BOOLEAN fRedraw );
UINT8		GetNextAimAd( UINT8 ubCurrentAd );



BOOLEAN		fFirstTimeIn=TRUE;

//Hotkey Assignment
void HandleAimKeyBoardInput();


//
// ***********************	Functions		*************************
//

void GameInitAIM()
{
	LaptopInitAim();
}



BOOLEAN EnterAIM()
{
	VOBJECT_DESC	VObjectDesc;

	gubWarningTimer = 0;
	gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
	LaptopInitAim();

	InitAimDefaults();

	// load the MemberShipcard graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\membercard.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMemberCard));

	// load the Policies graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Policies.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiPolicies));

	// load the Links graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Links.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiLinks));

	// load the History graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_HISTORY );
	CHECKF(AddVideoObject(&VObjectDesc, &guiHistory));

	// load the Wanring graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_WARNING );
	CHECKF(AddVideoObject(&VObjectDesc, &guiWarning));

	// load the flower advertisment and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\flowerad_16.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiFlowerAdvertisement));

	// load the your ad advertisment and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_YOURAD13 );
	CHECKF(AddVideoObject(&VObjectDesc, &guiAdForAdsImages));

	// load the insurance advertisment and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_INSURANCEAD10 );
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsuranceAdImages));

	// load the funeral advertisment and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_FUNERALAD9 );
	CHECKF(AddVideoObject(&VObjectDesc, &guiFuneralAdImages));

	// load the funeral advertisment and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_BOBBYRAYAD21 );
	CHECKF(AddVideoObject(&VObjectDesc, &guiBobbyRAdImages ));



	//** Mouse Regions **

	//Mouse region for the MebershipCard
	MSYS_DefineRegion( &gSelectedMemberCardRegion, MEMBERCARD_X, MEMBERCARD_Y , (MEMBERCARD_X + LINK_SIZE_X), (MEMBERCARD_Y + LINK_SIZE_Y), MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectMemberCardRegionCallBack );
	MSYS_AddRegion(&gSelectedMemberCardRegion);

	//Mouse region for the Policies
	MSYS_DefineRegion( &gSelectedPoliciesRegion, POLICIES_X, POLICIES_Y , (POLICIES_X + LINK_SIZE_X), (POLICIES_Y + LINK_SIZE_Y), MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectPoliciesRegionCallBack );
	MSYS_AddRegion(&gSelectedPoliciesRegion);

	//Mouse region for the History
	MSYS_DefineRegion( &gSelectedHistoryRegion, HISTORY_X, HISTORY_Y , (HISTORY_X + LINK_SIZE_X), (HISTORY_Y + LINK_SIZE_Y), MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectHistoryRegionCallBack );
	MSYS_AddRegion(&gSelectedHistoryRegion);

	//Mouse region for the Links
	MSYS_DefineRegion( &gSelectedLinksRegion, LINKS_X, LINKS_Y , (LINKS_X + LINK_SIZE_X), (LINKS_Y + LINK_SIZE_Y), MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinksRegionCallBack );
	MSYS_AddRegion(&gSelectedLinksRegion);

	//Mouse region for the Links
	MSYS_DefineRegion( &gSelectedBannerRegion, AIM_AD_TOP_LEFT_X, AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X, AIM_AD_BOTTOM_RIGHT_Y, MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectBannerRegionCallBack);
	MSYS_AddRegion(&gSelectedBannerRegion);

	// disable the region because only certain banners will be 'clickable'
	MSYS_DisableRegion(&gSelectedBannerRegion);

	gubAimMenuButtonDown=255;

	fFirstTimeIn = FALSE;
	RenderAIM();

	return( TRUE );
}

void LaptopInitAim()
{
	gfInitAdArea = TRUE;
}

void ExitAIM()
{
	RemoveAimDefaults();

	DeleteVideoObjectFromIndex(guiMemberCard);
	DeleteVideoObjectFromIndex(guiPolicies);
	DeleteVideoObjectFromIndex(guiLinks);
	DeleteVideoObjectFromIndex(guiHistory);
	DeleteVideoObjectFromIndex(guiWarning);
	DeleteVideoObjectFromIndex(guiFlowerAdvertisement);
	DeleteVideoObjectFromIndex(guiAdForAdsImages);
	DeleteVideoObjectFromIndex(guiInsuranceAdImages);
	DeleteVideoObjectFromIndex(guiFuneralAdImages);
	DeleteVideoObjectFromIndex(guiBobbyRAdImages);


	// Remove Mouse Regions
	MSYS_RemoveRegion( &gSelectedMemberCardRegion);
	MSYS_RemoveRegion( &gSelectedPoliciesRegion);
	MSYS_RemoveRegion( &gSelectedLinksRegion);
	MSYS_RemoveRegion( &gSelectedHistoryRegion);
	MSYS_RemoveRegion( &gSelectedBannerRegion);
}

void HandleAIM()
{
	HandleAdAndWarningArea( gfInitAdArea, FALSE );
	gfInitAdArea = FALSE;

	HandleAimKeyBoardInput();
}

void RenderAIM()
{
	HVOBJECT hMemberCardHandle;
	HVOBJECT hPoliciesHandle;
	HVOBJECT hLinksHandle;
	HVOBJECT hHistoryHandle;
//	UINT16	x,y, uiPosX, uiPosY;

	DrawAimDefaults();

	//MemberCard
	GetVideoObject(&hMemberCardHandle, guiMemberCard);
	BltVideoObject(FRAME_BUFFER, hMemberCardHandle, 0,MEMBERCARD_X, MEMBERCARD_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//Policies
	GetVideoObject(&hPoliciesHandle, guiPolicies);
	BltVideoObject(FRAME_BUFFER, hPoliciesHandle, 0,POLICIES_X, POLICIES_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//Links
	GetVideoObject(&hLinksHandle, guiLinks);
	BltVideoObject(FRAME_BUFFER, hLinksHandle, 0,LINKS_X, LINKS_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//History
	GetVideoObject(&hHistoryHandle, guiHistory);
	BltVideoObject(FRAME_BUFFER, hHistoryHandle, 0,HISTORY_X, HISTORY_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Draw the aim slogan under the symbol
	DisplayAimSlogan();

	DisplayAimCopyright();

	//Draw text under boxes
	// members
	DrawTextToScreen(AimBottomMenuText[AIM_MEMBERS], MEMBERCARD_X, MEMBERS_TEXT_Y, LINK_SIZE_X, FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	// Policies
	DrawTextToScreen(AimBottomMenuText[AIM_POLICIES], POLICIES_X, POLICIES_TEXT_Y, LINK_SIZE_X, FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	// History
	DrawTextToScreen(AimBottomMenuText[AIM_HISTORY], HISTORY_X, HISTORY_TEXT_Y, LINK_SIZE_X, FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	// Links
	DrawTextToScreen(AimBottomMenuText[AIM_LINKS], LINKS_X, LINK_TEXT_Y, LINK_SIZE_X, FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	HandleAdAndWarningArea( gfInitAdArea, TRUE );

	RenderWWWProgramTitleBar( );

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}



void SelectMemberCardRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if(!fFirstTimeIn)
			guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}


void SelectPoliciesRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_POLICIES;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}


void SelectHistoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_HISTORY;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}



void SelectLinksRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_LINKS;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}



BOOLEAN InitAimDefaults()
{
	VOBJECT_DESC	VObjectDesc;

	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustBackGround));

	// load the Aim Symbol graphic and add it
	if(gGameExternalOptions.gfUseNewStartingGearInterface)
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		GetMLGFilename( VObjectDesc.ImageFile, MLG_AIMSYMBOL_SMALL );
		CHECKF(AddVideoObject(&VObjectDesc, &guiAimSymbol));
		//Mouse region for the Links
		MSYS_DefineRegion( &gSelectedAimLogo, AIM_SYMBOL_SMALL_X, AIM_SYMBOL_SMALL_Y, AIM_SYMBOL_SMALL_X+AIM_SYMBOL_SMALL_WIDTH, AIM_SYMBOL_SMALL_Y+AIM_SYMBOL_SMALL_HEIGHT, MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectAimLogoRegionCallBack);
		MSYS_AddRegion(&gSelectedAimLogo);
	}
	else
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		GetMLGFilename( VObjectDesc.ImageFile, MLG_AIMSYMBOL );
		CHECKF(AddVideoObject(&VObjectDesc, &guiAimSymbol));
		//Mouse region for the Links
		MSYS_DefineRegion( &gSelectedAimLogo, AIM_SYMBOL_X, AIM_SYMBOL_Y, AIM_SYMBOL_X+AIM_SYMBOL_WIDTH, AIM_SYMBOL_Y+AIM_SYMBOL_HEIGHT, MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectAimLogoRegionCallBack);
		MSYS_AddRegion(&gSelectedAimLogo);
	}



	return(TRUE);
}

BOOLEAN RemoveAimDefaults()
{
	DeleteVideoObjectFromIndex(guiRustBackGround);
	DeleteVideoObjectFromIndex(guiAimSymbol);
	MSYS_RemoveRegion( &gSelectedAimLogo);

	return(TRUE);
}

BOOLEAN DrawAimDefaults()
{
	HVOBJECT hRustBackGroundHandle;
	HVOBJECT hAimSymbolHandle;
	UINT16	x,y, uiPosX, uiPosY;

	// Blt the rust background
	GetVideoObject(&hRustBackGroundHandle, guiRustBackGround);

	uiPosY = RUSTBACKGROUND_1_Y;
	for(y=0; y<4; y++)
	{
		uiPosX = RUSTBACKGROUND_1_X;
		for(x=0; x<4; x++)
		{
		BltVideoObject(FRAME_BUFFER, hRustBackGroundHandle, 0,uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += RUSTBACKGROUND_SIZE_X;
		}
		uiPosY += RUSTBACKGROUND_SIZE_Y;
	}

	// Aim Symbol
	if(gGameExternalOptions.gfUseNewStartingGearInterface)
	{
		GetVideoObject(&hAimSymbolHandle, guiAimSymbol);
		BltVideoObject(FRAME_BUFFER, hAimSymbolHandle, 0,AIM_SYMBOL_SMALL_X, AIM_SYMBOL_SMALL_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	}
	else
	{
		GetVideoObject(&hAimSymbolHandle, guiAimSymbol);
		BltVideoObject(FRAME_BUFFER, hAimSymbolHandle, 0,AIM_SYMBOL_X, AIM_SYMBOL_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	return(TRUE);
}


void SelectAimLogoRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}




BOOLEAN DisplayAimSlogan()
{
	CHAR16	sSlogan[400];
	if(!g_bUseXML_Strings)
	{
		LoadEncryptedDataFromFile(AIMHISTORYFILE, sSlogan, 0, AIM_HISTORY_LINE_SIZE);
	}
	else
	{
		Loc::GetString(Loc::AIM_HISTORY, L"Line", 0, sSlogan, 400);
	}
	//Display Aim Text under the logo
	DisplayWrappedString(AIM_LOGO_TEXT_X, AIM_LOGO_TEXT_Y, AIM_LOGO_TEXT_WIDTH, 2, AIM_LOGO_FONT, AIM_FONT_MCOLOR_WHITE, sSlogan, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	return(TRUE);
}


BOOLEAN DisplayAimCopyright()
{
	CHAR16	sSlogan[400];

	//Load and Display the copyright notice
	if(!g_bUseXML_Strings)
	{
		UINT32	uiStartLoc=0;
		uiStartLoc = AIM_HISTORY_LINE_SIZE * AIM_COPYRIGHT_1;
		LoadEncryptedDataFromFile(AIMHISTORYFILE, sSlogan, uiStartLoc, AIM_HISTORY_LINE_SIZE);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

		uiStartLoc = AIM_HISTORY_LINE_SIZE * AIM_COPYRIGHT_2;
		LoadEncryptedDataFromFile(AIMHISTORYFILE, sSlogan, uiStartLoc, AIM_HISTORY_LINE_SIZE);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y + AIM_COPYRIGHT_GAP, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

		uiStartLoc = AIM_HISTORY_LINE_SIZE * AIM_COPYRIGHT_3;
		LoadEncryptedDataFromFile(AIMHISTORYFILE, sSlogan, uiStartLoc, AIM_HISTORY_LINE_SIZE);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y + AIM_COPYRIGHT_GAP*2, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}
	else
	{
		Loc::GetString(Loc::AIM_HISTORY, L"Line", AIM_COPYRIGHT_1, sSlogan, 400);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

		Loc::GetString(Loc::AIM_HISTORY, L"Line", AIM_COPYRIGHT_2, sSlogan, 400);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y + AIM_COPYRIGHT_GAP, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

		Loc::GetString(Loc::AIM_HISTORY, L"Line", AIM_COPYRIGHT_3, sSlogan, 400);
		DrawTextToScreen(sSlogan, AIM_COPYRIGHT_X, AIM_COPYRIGHT_Y + AIM_COPYRIGHT_GAP*2, AIM_COPYRIGHT_WIDTH, AIM_COPYRIGHT_FONT, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}
	return(TRUE);
}

// Buttons
BOOLEAN InitAimMenuBar(void)
{
	UINT8	i;
	UINT16	usPosX;

	guiBottomButtonImage =	LoadButtonImage("LAPTOP\\BottomButtons2.sti", -1,0,-1,1,-1 );

	usPosX = BOTTOM_BUTTON_START_X;
	for(i=0; i<BOTTOM_BUTTON_AMOUNT; i++)
	{
		guiBottomButtons[i] = CreateIconAndTextButton( guiBottomButtonImage, AimBottomMenuText[i], FONT10ARIAL,
														AIM_BUTTON_ON_COLOR, DEFAULT_SHADOW,
														AIM_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, BOTTOM_BUTTON_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, BtnAimBottomButtonsCallback);
		SetButtonCursor(guiBottomButtons[i], CURSOR_LAPTOP_SCREEN);


		MSYS_SetBtnUserData( guiBottomButtons[i], 0, gCurrentAimPage[i]);
		MSYS_SetBtnUserData( guiBottomButtons[i], 1, i);

		usPosX += BOTTOM_BUTTON_START_WIDTH;
	}
	return(TRUE);
}
BOOLEAN ExitAimMenuBar(void)
{
	UINT8	i;

	UnloadButtonImage( guiBottomButtonImage );

	for(i=0; i<BOTTOM_BUTTON_AMOUNT; i++)
	{
		RemoveButton( guiBottomButtons[i] );
	}
	return(TRUE);
}


void BtnAimBottomButtonsCallback(GUI_BUTTON *btn,INT32 reason)
{
	UINT32		bNewValue;

	bNewValue = MSYS_GetBtnUserData( btn, 0 );
	gubAimMenuButtonDown = 255;
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;

		gubAimMenuButtonDown = (UINT8)MSYS_GetBtnUserData( btn, 1 );
		InvalidateRegion(BOTTOM_BUTTON_START_X,BOTTOM_BUTTON_START_Y,BOTTOM_BUTTON_END_X,BOTTOM_BUTTON_END_Y);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			ResetAimButtons(guiBottomButtons, NUM_AIM_BOTTOMBUTTONS);

			guiCurrentLaptopMode = (UINT8)MSYS_GetBtnUserData( btn, 0 );

			InvalidateRegion(BOTTOM_BUTTON_START_X,BOTTOM_BUTTON_START_Y,BOTTOM_BUTTON_END_X,BOTTOM_BUTTON_END_Y);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(BOTTOM_BUTTON_START_X,BOTTOM_BUTTON_START_Y,BOTTOM_BUTTON_END_X,BOTTOM_BUTTON_END_Y);
	}
	DisableAimButton();
}


void ResetAimButtons(UINT32 *Buttons, UINT16 uNumberOfButtons)
{
	UINT32 cnt;

	for ( cnt = 0; cnt < uNumberOfButtons; cnt++ )
	{
		ButtonList[ Buttons[ cnt ] ]->uiFlags &= ~(BUTTON_CLICKED_ON);
	}
}

void DisableAimButton()
{
	int i=0;

	for(i=0; i<NUM_AIM_BOTTOMBUTTONS; i++)
	{
		if( gCurrentAimPage[i] == guiCurrentLaptopMode)
			ButtonList[ guiBottomButtons[i] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void HandleAdAndWarningArea( BOOLEAN fInit, BOOLEAN fRedraw )
{
	static UINT8 ubPreviousAdvertisment;

	if( fInit )
		gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
	else
	{
		if( ubPreviousAdvertisment == AIM_AD_DONE )
		{
			gubCurrentAdvertisment = GetNextAimAd( gubCurrentAdvertisment );

			fInit = TRUE;

/*
			UINT32	uiDay = GetWorldDay();
			BOOLEAN	fSkip=FALSE;
			gubCurrentAdvertisment++;

			//if the add should be for Bobby rays
			if( gubCurrentAdvertisment == AIM_AD_BOBBY_RAY_AD )
			{
				//if the player has NOT ever been to drassen
				if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
				{
					//advance to the next add
					gubCurrentAdvertisment++;
				}
				else
				{
					fSkip = TRUE;
					fInit = TRUE;
				}
			}
			else
				fSkip = FALSE;


			if( !fSkip )
			{
				//if the current ad is not supposed to be available, loop back to the first ad
				switch( gubCurrentAdvertisment )
				{
					case AIM_AD_FUNERAL_ADS:
						if( uiDay < AIM_AD_DAY_FUNERAL_AD_STARTS )
							gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
						break;

					case AIM_AD_FLOWER_SHOP:
						if( uiDay < AIM_AD_DAY_FLOWER_AD_STARTS )
							gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
						break;

					case AIM_AD_INSURANCE_AD:
						if( uiDay < AIM_AD_DAY_INSURANCE_AD_STARTS )
							gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
						break;
				}
				fInit = TRUE;
			}
*/
		}

		if( gubCurrentAdvertisment >= AIM_AD_LAST_AD)
		{
			gubCurrentAdvertisment = AIM_AD_WARNING_BOX;
		}
	}

	switch( gubCurrentAdvertisment )
	{
		case 	AIM_AD_WARNING_BOX:
			MSYS_DisableRegion(&gSelectedBannerRegion);
			ubPreviousAdvertisment = DrawWarningBox( fInit, fRedraw );
			break;

		case AIM_AD_FLOWER_SHOP:
			ubPreviousAdvertisment = DisplayFlowerAd( fInit, fRedraw );
			break;

		case AIM_AD_FOR_ADS:
			// disable the region because only certain banners will be 'clickable'
			MSYS_DisableRegion(&gSelectedBannerRegion);
			ubPreviousAdvertisment = DisplayAd( fInit, fRedraw, AIM_AD_FOR_ADS_DELAY, AIM_AD_FOR_ADS__NUM_SUBIMAGES, guiAdForAdsImages );
			break;

		case AIM_AD_INSURANCE_AD:
		MSYS_EnableRegion(&gSelectedBannerRegion);
			ubPreviousAdvertisment = DisplayAd( fInit, fRedraw, AIM_AD_INSURANCE_AD_DELAY, AIM_AD_INSURANCE_AD__NUM_SUBIMAGES, guiInsuranceAdImages );
			break;

		case AIM_AD_FUNERAL_ADS:
		MSYS_EnableRegion(&gSelectedBannerRegion);
			ubPreviousAdvertisment = DisplayAd( fInit, fRedraw, AIM_AD_FUNERAL_AD_DELAY, AIM_AD_FUNERAL_AD__NUM_SUBIMAGES, guiFuneralAdImages );
			break;

		case AIM_AD_BOBBY_RAY_AD:
		MSYS_EnableRegion(&gSelectedBannerRegion);
//			ubPreviousAdvertisment = DisplayAd( fInit, fRedraw, AIM_AD_BOBBYR_AD_DELAY, AIM_AD_BOBBYR_AD__NUM_SUBIMAGES, guiBobbyRAdImages );
			ubPreviousAdvertisment = DisplayBobbyRAd( fInit, fRedraw );
			break;
	}
}



BOOLEAN DisplayFlowerAd( BOOLEAN fInit, BOOLEAN fRedraw )
{
	static UINT32 uiLastTime;
	static UINT8	ubSubImage=0;
	static UINT8	ubCount=0;
	UINT32 uiCurTime = GetJA2Clock();

	if( fInit )
	{
		uiLastTime = 0;
		ubSubImage = 0;
		ubCount = 0;
	MSYS_EnableRegion(&gSelectedBannerRegion);
	}

	if( ((uiCurTime - uiLastTime) > AIM_FLOWER_AD_DELAY) || fRedraw)
	{
		HVOBJECT hAdHandle;

		if( ubSubImage == AIM_FLOWER_NUM_SUBIMAGES)
		{
			if(ubCount == 0 || fRedraw)
			{
				//Blit the blue sky frame with text on top
				GetVideoObject(&hAdHandle, guiFlowerAdvertisement);
				BltVideoObject(FRAME_BUFFER, hAdHandle, 0,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

				// redraw new mail warning, and create new mail button, if nessacary
				fReDrawNewMailFlag = TRUE;

				//Display Aim Warning Text
				DisplayWrappedString(AIM_WARNING_TEXT_X, AIM_WARNING_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, FONT14ARIAL, FONT_GREEN, AimScreenText[AIM_INFO_6], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

				//Display Aim Warning Text
				SetFontShadow( FONT_MCOLOR_WHITE );
				DisplayWrappedString(AIM_WARNING_TEXT_X, AIM_FLOWER_LINK_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, FONT12ARIAL, 2, AimScreenText[AIM_INFO_7], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
				SetFontShadow( DEFAULT_SHADOW );

				InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
			}

			uiLastTime = GetJA2Clock();

			ubCount++;
			if( ubCount > 40 )
			{
				return( AIM_AD_DONE );
			}
			else
				return( AIM_AD_NOT_DONE );

		}
		else
		{
			GetVideoObject(&hAdHandle, guiFlowerAdvertisement);
			BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			// redraw new mail warning, and create new mail button, if nessacary
			fReDrawNewMailFlag = TRUE;

			ubSubImage++;
		}

		uiLastTime = GetJA2Clock();
	InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
	}
	return( AIM_AD_NOT_DONE );
}


BOOLEAN	DrawWarningBox( BOOLEAN fInit, BOOLEAN fRedraw )
{
	static UINT32	uiLastTime;
	static UINT8	ubSubImage = 0;
	UINT32			uiCurTime = GetJA2Clock();


	if( fInit || fRedraw)
	{
		CHAR16		sText[400];
		//UINT16	usLocY = AIM_WARNING_TEXT_Y + (GetFontHeight(AIM_WARNING_FONT) + 2) * 2;
		HVOBJECT	hWarningHandle;

		//Warning
		GetVideoObject(&hWarningHandle, guiWarning);
		BltVideoObject(FRAME_BUFFER, hWarningHandle, 0,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

		if(!g_bUseXML_Strings)
		{
			UINT32 uiStartLoc = AIM_HISTORY_LINE_SIZE * AIM_WARNING_1;
			LoadEncryptedDataFromFile(AIMHISTORYFILE, sText, uiStartLoc, AIM_HISTORY_LINE_SIZE);
		}
		else
		{
			Loc::GetString(Loc::AIM_HISTORY, L"Line", AIM_WARNING_1, sText, 400);
		}

		//Display Aim Warning Text
		DisplayWrappedString(AIM_WARNING_TEXT_X, AIM_WARNING_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, AIM_WARNING_FONT, FONT_RED, sText, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

		InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);

		// redraw new mail warning, and create new mail button, if nessacary
		fReDrawNewMailFlag = TRUE;

		if( fInit )
			uiLastTime = uiCurTime;
	}

	if( (uiCurTime - uiLastTime) > AIM_WARNING_TIME)
		return(AIM_AD_DONE);
	else
	{
		return(AIM_AD_NOT_DONE);
	}
}

void SelectBannerRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( gubCurrentAdvertisment == AIM_AD_FLOWER_SHOP )
			GoToWebPage( FLORIST_BOOKMARK );
		else if( gubCurrentAdvertisment == AIM_AD_INSURANCE_AD )
			GoToWebPage( INSURANCE_BOOKMARK );
		else if( gubCurrentAdvertisment == AIM_AD_FUNERAL_ADS )
			GoToWebPage( FUNERAL_BOOKMARK );
		else if( gubCurrentAdvertisment == AIM_AD_BOBBY_RAY_AD )
			GoToWebPage( BOBBYR_BOOKMARK );


	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}




BOOLEAN DisplayAd( BOOLEAN fInit, BOOLEAN fRedraw, UINT16 usDelay, UINT16 usNumberOfSubImages, UINT32 uiAdImageIdentifier )
{
	static UINT32 uiLastTime;
	static UINT8	ubSubImage=0;
	static UINT8	ubCount=0;
	UINT32 uiCurTime = GetJA2Clock();
	UINT8	ubRetVal = 0;

	if( fInit )
	{
		uiLastTime = 0;
		ubSubImage = 0;
		ubCount = 0;
	}

	if( ((uiCurTime - uiLastTime) > usDelay) || fRedraw)
	{
		HVOBJECT hAdHandle;

		if( ubSubImage == 0 )
		{
			if(ubCount == 0 || fRedraw)
			{
				//Blit the ad
				GetVideoObject(&hAdHandle, uiAdImageIdentifier);
				BltVideoObject(FRAME_BUFFER, hAdHandle, 0,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

				// redraw new mail warning, and create new mail button, if nessacary
				fReDrawNewMailFlag = TRUE;

				InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
			}

			uiLastTime = GetJA2Clock();

			//display first frame longer then rest
			ubCount++;
			if( ubCount > 12 )
			{
				ubCount=0;
				ubSubImage++;
			}

			ubRetVal = AIM_AD_NOT_DONE;

		}
		else if( ubSubImage == usNumberOfSubImages-1 )
		{
			if(ubCount == 0 || fRedraw)
			{
				//Blit the ad
				GetVideoObject(&hAdHandle, uiAdImageIdentifier);
				BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

				// redraw new mail warning, and create new mail button, if nessacary
				fReDrawNewMailFlag = TRUE;

				InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
			}

			uiLastTime = GetJA2Clock();

			//display first frame longer then rest
			ubCount++;
			if( ubCount > 12 )
			{
				ubRetVal = AIM_AD_DONE;
			}
		}
		else
		{
			GetVideoObject(&hAdHandle, uiAdImageIdentifier);
			BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			// redraw new mail warning, and create new mail button, if nessacary
			fReDrawNewMailFlag = TRUE;

			ubSubImage++;
		}

		//if the add it to have text on it, then put the text on it.
		HandleTextOnAimAdd( ubSubImage );

		uiLastTime = GetJA2Clock();
	InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
	}
	return( ubRetVal );
}

void HandleTextOnAimAdd( UINT8 ubCurSubImage )
{
	switch( gubCurrentAdvertisment )
	{
		case 	AIM_AD_WARNING_BOX:
			break;

		case AIM_AD_FLOWER_SHOP:
			break;

		case AIM_AD_FOR_ADS:
			break;

		case AIM_AD_INSURANCE_AD:
			break;

		case AIM_AD_FUNERAL_ADS:
			break;

		case AIM_AD_BOBBY_RAY_AD:

			//if the subimage is the first couple
			if( ubCurSubImage <= AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES )
			{
				//Display Aim Warning Text
				SetFontShadow( 2 );
				DisplayWrappedString(AIM_BOBBYR1_LINK_TEXT_X, AIM_BOBBYR1_LINK_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, BOBBYR_UNDER_CONSTRUCTION_AD_FONT, BOBBYR_UNDER_CONSTRUCTION_AD_COLOR, AimScreenText[AIM_BOBBYR_ADD1], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED | INVALIDATE_TEXT );
				SetFontShadow( DEFAULT_SHADOW );
			}


			else if( ubCurSubImage >= AIM_AD_BOBBYR_AD__NUM_SUBIMAGES-5 )
			{
				//Display Aim Warning Text
				SetFontShadow( 2 );
				DisplayWrappedString(AIM_BOBBYR2_LINK_TEXT_X, AIM_BOBBYR2_LINK_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, BOBBYR_UNDER_CONSTRUCTION_AD_FONT, BOBBYR_UNDER_CONSTRUCTION_AD_COLOR, AimScreenText[AIM_BOBBYR_ADD2], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED | INVALIDATE_TEXT ) ;
				SetFontShadow( DEFAULT_SHADOW );
			}
/*
			else
			{
				//Display Aim Warning Text
				SetFontShadow( 2 );
//				DisplayWrappedString( AIM_BOBBYR3_LINK_TEXT_X, AIM_BOBBYR3_LINK_TEXT_Y, AIM_WARNING_TEXT_WIDTH, 2, BOBBYR_UNDER_CONSTRUCTION_AD_FONT, FONT_MCOLOR_WHITE, AimScreenText[AIM_BOBBYR_ADD3], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED | INVALIDATE_TEXT );
				SetFontShadow( DEFAULT_SHADOW );
			}
*/
			break;
	}
}



BOOLEAN DisplayBobbyRAd( BOOLEAN fInit, BOOLEAN fRedraw )
{
	static UINT32 uiLastTime;
	static UINT8	ubSubImage=0;
	static UINT8	ubDuckCount=0;
	static UINT8	ubCount=0;
	UINT32 uiCurTime = GetJA2Clock();
	UINT8	ubRetVal = 0;
	UINT16	usDelay = AIM_AD_BOBBYR_AD_DELAY;


	if( fInit )
	{
		ubDuckCount = 0;
		uiLastTime = 0;
		ubSubImage = 0;
		ubCount = 0;
	}

	if( ((uiCurTime - uiLastTime) > usDelay) || fRedraw)
	{
		HVOBJECT hAdHandle;

		//Loop through the first 6 images twice, then start into the later ones
		GetVideoObject(&hAdHandle, guiBobbyRAdImages);

		//if we are still looping through the first 6 animations
		if( ubDuckCount < 2 )
		{
			BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage, WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			ubSubImage ++;

			InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);

			// if we do the first set of images
			if( ubSubImage > AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES )
			{
				ubDuckCount++;

				if( ubDuckCount < 2 )
					ubSubImage = 0;
				else
					ubSubImage = AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES+1;
			}
			ubRetVal = AIM_AD_NOT_DONE;
		}

		else
		{
			//Blit the ad
			BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			ubSubImage ++;

			if( ubSubImage >= AIM_AD_BOBBYR_AD__NUM_SUBIMAGES-1 )
			{
				//display last frame longer then rest
				ubCount++;
				if( ubCount > 12 )
				{
					ubRetVal = AIM_AD_DONE;
				}

				ubSubImage = AIM_AD_BOBBYR_AD__NUM_SUBIMAGES - 1;
			}

			// redraw new mail warning, and create new mail button, if nessacary
			fReDrawNewMailFlag = TRUE;

			InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
		}

		//if the add it to have text on it, then put the text on it.
		HandleTextOnAimAdd( ubSubImage );


		uiLastTime = GetJA2Clock();
	InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
	}

/*

		if( ubDuckImage < AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES )
		{
			ubDuckImage++;
		}

		GetVideoObject(&hAdHandle, guiBobbyRAdImages);

		if( ubDuckImage < AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES * 2 )
		{
			if( ubDuckImage >= AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES )
				BltVideoObject(FRAME_BUFFER, hAdHandle, (UINT16)(ubDuckImage-AIM_AD_BOBBYR_AD_NUM_DUCK_SUBIMAGES), WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);
			else
				BltVideoObject(FRAME_BUFFER, hAdHandle, ubDuckImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			ubDuckImage++;
		}
		else
			ubSubImage = 5;


		if( ubSubImage == 5 )
		{
			if(ubCount == 0 || fRedraw)
			{
				//Blit the ad
				BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

				// redraw new mail warning, and create new mail button, if nessacary
				fReDrawNewMailFlag = TRUE;

				InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
			}

			uiLastTime = GetJA2Clock();


			ubRetVal = AIM_AD_NOT_DONE;

		}
		else if( ubSubImage == AIM_AD_BOBBYR_AD__NUM_SUBIMAGES-1 )
		{
			if(ubCount == 0 || fRedraw)
			{
				//Blit the ad
				GetVideoObject(&hAdHandle, guiBobbyRAdImages);
				BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

				// redraw new mail warning, and create new mail button, if nessacary
				fReDrawNewMailFlag = TRUE;

				InvalidateRegion(AIM_AD_TOP_LEFT_X,AIM_AD_TOP_LEFT_Y, AIM_AD_BOTTOM_RIGHT_X	,AIM_AD_BOTTOM_RIGHT_Y);
			}

			uiLastTime = GetJA2Clock();

			//display last frame longer then rest
			ubCount++;
			if( ubCount > 12 )
			{
				ubRetVal = AIM_AD_DONE;
			}
		}
		else
		{
			GetVideoObject(&hAdHandle, guiBobbyRAdImages);
			BltVideoObject(FRAME_BUFFER, hAdHandle, ubSubImage,WARNING_X, WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);

			// redraw new mail warning, and create new mail button, if nessacary
			fReDrawNewMailFlag = TRUE;

			ubSubImage++;
		}
*/

	return( ubRetVal );
}


UINT8 GetNextAimAd( UINT8 ubCurrentAd )
{
	UINT8 ubNextAd;
	UINT32	uiDay = GetWorldDay();
	if( ubCurrentAd == AIM_AD_WARNING_BOX )
	{
		if( uiDay < AIM_AD_BOBBYR_AD_STARTS )
		{
			//if the player has NOT ever been to drassen
			if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
			{
				ubNextAd = AIM_AD_FOR_ADS;
			}
			else
			{
				ubNextAd = AIM_AD_BOBBY_RAY_AD;
			}
		}

		else if( uiDay < AIM_AD_DAY_FUNERAL_AD_STARTS )
			ubNextAd = AIM_AD_FUNERAL_ADS;

		else if( uiDay < AIM_AD_DAY_FLOWER_AD_STARTS )
			ubNextAd = AIM_AD_FLOWER_SHOP;

		else //if( uiDay < AIM_AD_DAY_INSURANCE_AD_STARTS )
			ubNextAd = AIM_AD_INSURANCE_AD;
	}
	else
	{
		ubNextAd = AIM_AD_WARNING_BOX;
	}

	return( ubNextAd );
}



void HandleAimKeyBoardInput()
{
	InputAtom					InputEvent;

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{//!HandleTextInput( &InputEvent ) &&
		if( InputEvent.usEvent == KEY_DOWN )
		{
			switch (InputEvent.usParam)
			{
				case 'h':
					guiCurrentLaptopMode = LAPTOP_MODE_AIM_HISTORY;
					break;
				case 'l':
					guiCurrentLaptopMode = LAPTOP_MODE_AIM_LINKS;
					break;
				case ENTER:
				case 'e':
				case 'm':
					if(!fFirstTimeIn)
						guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES;
					break;
				case 'p':
					guiCurrentLaptopMode = LAPTOP_MODE_AIM_POLICIES;
					break;
				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
					break;
			}
		}
	}
}


