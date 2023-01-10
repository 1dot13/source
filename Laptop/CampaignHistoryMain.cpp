/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

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
	#include "CampaignHistoryMain.h"


#define		BACKGROUND_WIDTH				125
#define		CAMPAIGN_HISTORY_BACKGROUND_HEIGHT					100

#define		CAMPAIGN_HISTORY_BIG_TITLE_X								115 + LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_BIG_TITLE_Y								10 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_SUBTITLE_X								CAMPAIGN_HISTORY_BIG_TITLE_X
#define		CAMPAIGN_HISTORY_SUBTITLE_Y								CAMPAIGN_HISTORY_BIG_TITLE_Y + 20


extern UINT32	guiInsuranceBackGround;
extern UINT32	guiInsuranceSmallTitleImage;
extern UINT32	guiInsuranceBigRedLineImage;
UINT32	guiCampaignBulletImage;
UINT32	guiCampaignLogoImage;


//link to the various pages
MOUSE_REGION	gCampaignHistoryLinkRegion[4];
void SelectCampaignHistoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


void GameInitCampaignHistory()
{

}

BOOLEAN EnterCampaignHistory()
{
	VOBJECT_DESC	VObjectDesc;

	InitCampaignHistoryDefaults();

	// load the Insurance bullet graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\bullet.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCampaignBulletImage));
	
	RenderCampaignHistory();

	return(TRUE);
}

void ExitCampaignHistory()
{
	RemoveCampaignHistoryDefaults();

	DeleteVideoObjectFromIndex( guiCampaignBulletImage );
	DeleteVideoObjectFromIndex( guiCampaignLogoImage );
}

void HandleCampaignHistory()
{

}

void RenderCampaignHistory()
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;
	HVOBJECT	hPixHandle;

	//Get the bullet
	GetVideoObject(&hPixHandle, guiCampaignBulletImage );

	DisplayCampaignHistoryDefaults();

	SetFontShadow( CAMPHIS_FONT_SHADOW );
		
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 80;

	swprintf(sText, szCampaignHistoryWebSite[TEXT_CAMPAIGNHISTORY_DESCRIPTION_1], pCountryNames[COUNTRY_NAME]);
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, CAMPHIS_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0);

	for(int i = TEXT_CAMPAIGNHISTORY_NAME_MINISTRY; i < TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION_SUBTITLE; ++i)
	{
		// display bullet
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

		switch( i )
		{
			case TEXT_CAMPAIGNHISTORY_NAME_REBEL:
			case TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION_SUBTITLE:
				swprintf(sText, szCampaignHistoryWebSite[i], pCountryNames[COUNTRY_NAME] );
				break;
			case TEXT_CAMPAIGNHISTORY_NAME_MINISTRY:
				swprintf(sText, szCampaignHistoryWebSite[i], pCountryNames[COUNTRY_NOUN] );
				break;
			default:
				swprintf(sText, szCampaignHistoryWebSite[i] );
		}
		
		DrawTextToScreen( sText, usPosX + 25, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_MED, CAMPHIS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 15;
	}

	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}


BOOLEAN InitCampaignHistoryDefaults()
{
	VOBJECT_DESC	VObjectDesc;

	// load the Flower Account Box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BackGroundTile.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsuranceBackGround));
	
	// load the red bar on the side of the page and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\LargeBar.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsuranceBigRedLineImage));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\PressLogos.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCampaignLogoImage));

	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for(int i=0; i<4; ++i)
	{
		MSYS_DefineRegion( &gCampaignHistoryLinkRegion[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY+CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectCampaignHistoryRegionCallBack);
		MSYS_AddRegion(&gCampaignHistoryLinkRegion[i]);
		MSYS_SetRegionUserData( &gCampaignHistoryLinkRegion[i], 0, i );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;
	}

	return( TRUE );
}

void DisplayCampaignHistoryDefaults()
{
	HVOBJECT	hPixHandle;
	GetVideoObject(&hPixHandle, guiCampaignLogoImage );

	SetFontShadow( CAMPHIS_FONT_SHADOW );

	CHAR16 sText[800];
	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;

	WebPageTileBackground( 4, 4, BACKGROUND_WIDTH, CAMPAIGN_HISTORY_BACKGROUND_HEIGHT, guiInsuranceBackGround );

	//Display the title slogan
	GetCampaignHistoryText( TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION, sText );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_BIG_TITLE_X, CAMPAIGN_HISTORY_BIG_TITLE_Y, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, CAMPHIS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	//Display the subtitle slogan
	GetCampaignHistoryText( TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION_SUBTITLE, sText );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_SUBTITLE_X, CAMPAIGN_HISTORY_SUBTITLE_Y, 0, CAMPHIS_FONT_BIG, CAMPHIS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for(int i=0; i<4; ++i)
	{
		GetCampaignHistoryText( TEXT_CAMPAIGNHISTORY_LINK_CONFLICTSUMMARY + i, sText );
		DisplayWrappedString( usPosX, usPosY, CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, 2, CAMPHIS_FONT_MED, CAMPHIS_FONT_COLOR,	sText, FONT_MCOLOR_BLACK, FALSE, 0);

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;

		//Display the red bar under the link at the bottom.	and the text
		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_UL_X + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, usPosY - 2 );
	}

	// closing line that separates header from individual page
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_LR_X, usPosY - 2 );

	usPosX = LAPTOP_SCREEN_LR_X - 50;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

	SetFontShadow( DEFAULT_SHADOW );
}

void RemoveCampaignHistoryDefaults()
{
	DeleteVideoObjectFromIndex( guiInsuranceBackGround );
	DeleteVideoObjectFromIndex( guiInsuranceBigRedLineImage );
	
	for(int i=0; i<4; ++i)
		MSYS_RemoveRegion( &gCampaignHistoryLinkRegion[i]);
}

void GetCampaignHistoryText( UINT8 ubNumber, STR16 pString )
{
	UINT32	uiStartLoc=0;
	CHAR16		sText[800];

	if ( ubNumber >= TEXT_CAMPAIGNHISTORY_MAX )
		wcscpy(	pString, L"bla" );
		
	switch( ubNumber )
	{
		case TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION:
		case TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION_SUBTITLE:
			swprintf( sText, szCampaignHistoryWebSite[ ubNumber ], pCountryNames[COUNTRY_NAME] );
			wcscpy(	pString, sText );
			break;
		default:
			wcscpy(	pString, szCampaignHistoryWebSite[ubNumber] );
	}
}


void SelectCampaignHistoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		if( uiLink == 0 )
			guiCurrentLaptopMode = LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY;
		else if( uiLink == 1 )
			guiCurrentLaptopMode = LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT;
		else if( uiLink == 2 )
			guiCurrentLaptopMode = LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS;
		else if( uiLink == 3 )
			guiCurrentLaptopMode = LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}
