/**
* @file
* @author Flugente (bears-pit.com)
*/

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
#include "Line.h"
#include "Insurance Text.h"
#include "Encrypted File.h"
#include "Text.h"
#include "Multi Language Graphic Utils.h"
#include "WHO.h"
#include "random.h"
#include "Interface.h"
#include "Soldier Add.h"
#include "Soldier Profile.h"
#include "DropDown.h"
#include "Overhead.h"
#include "Map Screen Interface.h"
#include "Quests.h"
#include "finances.h"
#include "Game Clock.h"
#endif


#define		MERCOMP_FONT_COLOR								2
#define		CAMPHIS_FONT_COLOR_RED							FONT_MCOLOR_RED
#define		CAMPHIS_FONT_BIG								FONT14ARIAL
#define		CAMPHIS_FONT_MED								FONT12ARIAL
#define		CAMPHIS_FONT_SMALL								FONT10ARIAL

#define		MERCOMP_FONT_SHADOW								FONT_MCOLOR_WHITE

#define		CAMPAIGN_HISTORY_LINK_START_X					LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_LINK_START_Y					LAPTOP_SCREEN_WEB_UL_Y + 5
#define		CAMPAIGN_HISTORY_LINK_TEXT_WIDTH				107
#define		CAMPAIGN_HISTORY_LINK_STEP_Y					14

#define		BACKGROUND_WIDTH								125
#define		CAMPAIGN_HISTORY_BACKGROUND_HEIGHT				100

#define		CAMPAIGN_HISTORY_BIG_TITLE_X					115 + LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_BIG_TITLE_Y					10 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_SUBTITLE_X						CAMPAIGN_HISTORY_BIG_TITLE_X
#define		CAMPAIGN_HISTORY_SUBTITLE_Y						CAMPAIGN_HISTORY_BIG_TITLE_Y + 20

#define		NUM_LINKS										3
#define		MCA_START_CONTENT_Y								(LAPTOP_SCREEN_WEB_UL_Y + NUM_LINKS * 17)


extern UINT32	guiInsuranceBackGround;
extern UINT32	guiInsuranceSmallTitleImage;
extern UINT32	guiInsuranceBigRedLineImage;
extern UINT32	guiMercCompareBulletImage;
extern UINT32	guiMercCompareLogoImage;

//link to the various pages
MOUSE_REGION	gLinkRegion_WHO[NUM_LINKS];

void SelectLinkRegionCallBack_WHO( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		if ( uiLink == 0 )
			guiCurrentLaptopMode = LAPTOP_MODE_WHO_MAIN;
		else if ( uiLink == 1 )
			guiCurrentLaptopMode = LAPTOP_MODE_WHO_CONTRACT;
		else if ( uiLink == 2 )
			guiCurrentLaptopMode = LAPTOP_MODE_WHO_TIPS;
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

void InitDefaults_WHO( )
{
	VOBJECT_DESC	VObjectDesc;

	// load the Insurance bullet graphic and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\bullet.sti", VObjectDesc.ImageFile );
	CHECKV( AddVideoObject( &VObjectDesc, &guiMercCompareBulletImage ) );

	// load the Flower Account Box graphic and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\BackGroundTile.sti", VObjectDesc.ImageFile );
	CHECKV( AddVideoObject( &VObjectDesc, &guiInsuranceBackGround ) );

	// load the red bar on the side of the page and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\LargeBar.sti", VObjectDesc.ImageFile );
	CHECKV( AddVideoObject( &VObjectDesc, &guiInsuranceBigRedLineImage ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\PressLogos.sti", VObjectDesc.ImageFile );
	CHECKV( AddVideoObject( &VObjectDesc, &guiMercCompareLogoImage ) );

	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for ( int i = 0; i<NUM_LINKS; ++i )
	{
		MSYS_DefineRegion( &gLinkRegion_WHO[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY + CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						   CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack_WHO );
		MSYS_AddRegion( &gLinkRegion_WHO[i] );
		MSYS_SetRegionUserData( &gLinkRegion_WHO[i], 0, i );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;
	}
}

void DisplayDefaults_WHO( )
{
	HVOBJECT	hPixHandle;
	GetVideoObject( &hPixHandle, guiMercCompareLogoImage );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	CHAR16 sText[800];
	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;

	WebPageTileBackground( 4, 4, BACKGROUND_WIDTH, CAMPAIGN_HISTORY_BACKGROUND_HEIGHT, guiInsuranceBackGround );

	//Display the title slogan
	swprintf( sText, szWHOWebSite[TEXT_WHO_WEBSITENAME] );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_BIG_TITLE_X, CAMPAIGN_HISTORY_BIG_TITLE_Y, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	//Display the subtitle slogan
	swprintf( sText, szWHOWebSite[TEXT_WHO_SLOGAN] );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_SUBTITLE_X, CAMPAIGN_HISTORY_SUBTITLE_Y, 0, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for ( int i = 0; i<NUM_LINKS; ++i )
	{
		swprintf( sText, szWHOWebSite[TEXT_WHO_SUBSITE1 + i] );
		DisplayWrappedString( usPosX, usPosY, CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;

		//Display the red bar under the link at the bottom.	and the text
		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_UL_X + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, usPosY - 2, FROMRGB( 0, 30, 180 ) );
	}

	// closing line that separates header from individual page
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_LR_X, usPosY - 2, FROMRGB( 0, 30, 180 ) );

	usPosX = LAPTOP_SCREEN_LR_X - 60;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	BltVideoObject( FRAME_BUFFER, hPixHandle, 2, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	SetFontShadow( DEFAULT_SHADOW );
}

void RemoveDefaults_WHO( )
{
	DeleteVideoObjectFromIndex( guiInsuranceBackGround );
	DeleteVideoObjectFromIndex( guiInsuranceBigRedLineImage );
	DeleteVideoObjectFromIndex( guiMercCompareBulletImage );
	DeleteVideoObjectFromIndex( guiMercCompareLogoImage );

	for ( int i = 0; i<NUM_LINKS; ++i )
		MSYS_RemoveRegion( &gLinkRegion_WHO[i] );
}

////////////////////////// MAIN PAGE ////////////////////////////////
BOOLEAN EnterWHOMain( )
{
	InitDefaults_WHO( );

	RenderWHOMain( );

	return(TRUE);
}

void ExitWHOMain( )
{
	RemoveDefaults_WHO( );
}

void HandleWHOMain( )
{

}

void RenderWHOMain( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayDefaults_WHO( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;
		
	for ( int i = TEXT_WHO_MAIN1; i < TEXT_WHO_CONTRACT1; ++i )
	{
		swprintf( sText, szWHOWebSite[i] );
		usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 10;
	}
	
	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// MAIN PAGE ////////////////////////////////

////////////////////////// CONTRACT PAGE ////////////////////////////////
MOUSE_REGION	gLinkRegion_WHO_Contract;
BOOLEAN			gLinkRegionDefined = FALSE;

void SelectLinkRegionCallBack_WHO_Contract( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED] = !gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED];

		// if we've just subscribed, set data
		if ( gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED] && !gubFact[FACT_DISEASE_WHODATA_ACCESS] )
		{
			gubFact[FACT_DISEASE_WHODATA_ACCESS] = TRUE;

			AddTransactionToPlayersBook( WHO_SUBSCRIPTION, 0, GetWorldTotalMin( ), (-1) * (INT32)gGameExternalOptions.sDiseaseWHOSubscriptionCost );
		}
		
		RenderWHOContract( );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

BOOLEAN EnterWHOContract( )
{
	InitDefaults_WHO( );
	
	RenderWHOContract( );

	return(TRUE);
}

void ExitWHOContract( )
{
	if ( gLinkRegionDefined )
	{
		MSYS_RemoveRegion( &gLinkRegion_WHO_Contract );
		gLinkRegionDefined = FALSE;
	}

	RemoveDefaults_WHO( );
}

void HandleWHOContract( )
{

}

void RenderWHOContract( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayDefaults_WHO( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;
	
	swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT1] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT1 + 1] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT1 + 2], gGameExternalOptions.sDiseaseWHOSubscriptionCost );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosY += 10;

	swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT1 + 3] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosY += 10;

	if ( gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED] )
		swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT_ACQUIRED] );
	else
		swprintf( sText, szWHOWebSite[TEXT_WHO_CONTRACT_ACQUIRED_NOT] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	usPosY += 10;

	if ( !gLinkRegionDefined )
	{
		MSYS_DefineRegion( &gLinkRegion_WHO_Contract, usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY + CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						   CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack_WHO_Contract );
		MSYS_AddRegion( &gLinkRegion_WHO_Contract );
		MSYS_SetRegionUserData( &gLinkRegion_WHO_Contract, 0, 1 );
		gLinkRegionDefined = TRUE;
	}

	swprintf( sText, gubFact[FACT_DISEASE_WHODATA_SUBSCRIBED] ? szWHOWebSite[TEXT_WHO_CONTRACT_BUTTON_UNSUBSCRIBE] : szWHOWebSite[TEXT_WHO_CONTRACT_BUTTON_SUBSCRIBE] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	//Display the red bar under the link at the bottom.	and the text
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_UL_X + 140, usPosY - 2, FROMRGB( 0, 30, 180 ) );

	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// CONTRACT PAGE ////////////////////////////////

////////////////////////// HELPFUL TIPS PAGE ////////////////////////////////
BOOLEAN EnterWHOTips( )
{
	InitDefaults_WHO( );

	RenderWHOTips( );

	return(TRUE);
}

void ExitWHOTips( )
{
	RemoveDefaults_WHO( );
}

void HandleWHOTips( )
{

}

void RenderWHOTips( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;
	HVOBJECT	hPixHandle;

	//Get the bullet
	GetVideoObject( &hPixHandle, guiMercCompareBulletImage );
	
	DisplayDefaults_WHO( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	for ( int i = TEXT_WHO_TIPS1; i < TEXT_WHO_MAX; ++i )
	{
		// display bullet
		BltVideoObject( FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

		swprintf( sText, szWHOWebSite[ i ] );
		usPosY += DisplayWrappedString( usPosX + 25, usPosY, LAPTOP_SCREEN_LR_X - (LAPTOP_SCREEN_UL_X + 25), 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// HELPFUL TIPS PAGE ////////////////////////////////
