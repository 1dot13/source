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
#include "PMC.h"
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
#include "Animation Data.h"

#include "SaveLoadGame.h"
#include "GameVersion.h"
#include "message.h"
#include "Game Event Hook.h"
#include "militia control.h"
#include "Town Militia.h"
#include "Strategic Town Loyalty.h"
#include "MilitiaIndividual.h"
#endif

std::vector<PMCReinforcesHireEvent> gPMCHiringEvents;
PMCGlobalData gPMCData;

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
MOUSE_REGION	gLinkRegion_PMC[NUM_LINKS];

void SelectLinkRegionCallBack_PMC( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		if ( uiLink == 0 )
			guiCurrentLaptopMode = LAPTOP_MODE_PMC_MAIN;
		else if ( uiLink == 1 )
			guiCurrentLaptopMode = LAPTOP_MODE_PMC_CONTRACT_MILITIA;
		else if ( uiLink == 2 )
			guiCurrentLaptopMode = LAPTOP_MODE_BROKEN_LINK;
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}

void InitDefaults_PMC( )
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
		MSYS_DefineRegion( &gLinkRegion_PMC[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY + CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						   CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack_PMC );
		MSYS_AddRegion( &gLinkRegion_PMC[i] );
		MSYS_SetRegionUserData( &gLinkRegion_PMC[i], 0, i );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;
	}
}

void DisplayDefaults_PMC( )
{
	HVOBJECT	hPixHandle;
	GetVideoObject( &hPixHandle, guiMercCompareLogoImage );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	CHAR16 sText[800];
	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;

	WebPageTileBackground( 4, 4, BACKGROUND_WIDTH, CAMPAIGN_HISTORY_BACKGROUND_HEIGHT, guiInsuranceBackGround );

	//Display the title slogan
	swprintf( sText, szPMCWebSite[TEXT_PMC_WEBSITENAME] );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_BIG_TITLE_X, CAMPAIGN_HISTORY_BIG_TITLE_Y, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	//Display the subtitle slogan
	swprintf( sText, szPMCWebSite[TEXT_PMC_SLOGAN] );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_SUBTITLE_X, CAMPAIGN_HISTORY_SUBTITLE_Y, 0, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for ( int i = 0; i<NUM_LINKS; ++i )
	{
		swprintf( sText, szPMCWebSite[TEXT_PMC_SUBSITE1 + i] );
		DisplayWrappedString( usPosX, usPosY, CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;

		//Display the red bar under the link at the bottom.	and the text
		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_UL_X + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, usPosY - 2, FROMRGB( 240, 240, 20 ) );
	}

	// closing line that separates header from individual page
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_LR_X, usPosY - 2, FROMRGB( 240, 240, 20 ) );

	usPosX = LAPTOP_SCREEN_LR_X - 110;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	BltVideoObject( FRAME_BUFFER, hPixHandle, 3, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	SetFontShadow( DEFAULT_SHADOW );
}

void RemoveDefaults_PMC( )
{
	DeleteVideoObjectFromIndex( guiInsuranceBackGround );
	DeleteVideoObjectFromIndex( guiInsuranceBigRedLineImage );
	DeleteVideoObjectFromIndex( guiMercCompareBulletImage );
	DeleteVideoObjectFromIndex( guiMercCompareLogoImage );

	for ( int i = 0; i<NUM_LINKS; ++i )
		MSYS_RemoveRegion( &gLinkRegion_PMC[i] );
}

////////////////////////// MAIN PAGE ////////////////////////////////
BOOLEAN EnterPMCMain( )
{
	InitDefaults_PMC( );

	RenderPMCMain( );

	return(TRUE);
}

void ExitPMCMain( )
{
	RemoveDefaults_PMC( );
}

void HandlePMCMain( )
{

}

void RenderPMCMain( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayDefaults_PMC( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	for ( int i = TEXT_PMC_MAIN1; i < TEXT_PMC_CONTRACT_TEAM_INTRO; ++i )
	{
		swprintf( sText, szPMCWebSite[i] );
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
BOOLEAN fPMCContractRedraw = FALSE;

// two dropdowns, which we'll use to toggle between our mercs
template<>	void	DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::SetRefresh( )	{
	fPMCContractRedraw = TRUE;
}

INT32 gPMCButtonImage[4];
INT32 gPMCButton[4] = {-1, -1, -1, -1};
BOOLEAN gPMCButtonCreated[4] = {FALSE, FALSE, FALSE, FALSE};

INT32 gPMCHireButton = -1;
BOOLEAN gPMCHireButtonCreated = FALSE;

typedef struct
{
	UINT8	usBodyType;
	UINT8	usHair;
	UINT8	usSkin;
	UINT8	usVest;
	UINT8	usPants;

	UINT8	usToHire;

	UINT16	usCostEach;
} PMCPersonnelData;

PMCPersonnelData pmcdata[2];

// how much does it cost to hire a militia from the PMC?
UINT16 GetMilitiaCostPMC( UINT8 aSoldierClass )
{
	// Flugente: if individual militia is on, the upfront payment is much smaller (a single day's wage). In exchange, we have to pay higher wages for these guys
	if ( gGameExternalOptions.fIndividualMilitia )
	{
		return gMilitiaOriginData[MO_PMC].dailycost[aSoldierClass];
	}
	else
	{
		switch ( aSoldierClass )
		{
		case REGULAR_MILITIA:
			return (4 * gGameExternalOptions.iMilitiaTrainingCost * gGameExternalOptions.iRegularCostModifier) / gGameExternalOptions.iTrainingSquadSize + 7 * gGameExternalOptions.usDailyCostTown[REGULAR_MILITIA];
			break;

		case ELITE_MILITIA:
			// elites are upgraded regulars, so we pay for the basic + special training
			return ((4 * gGameExternalOptions.iMilitiaTrainingCost * gGameExternalOptions.iRegularCostModifier) + (2 * gGameExternalOptions.iMilitiaTrainingCost * gGameExternalOptions.iVeteranCostModifier)) / gGameExternalOptions.iTrainingSquadSize + 7 * gGameExternalOptions.usDailyCostTown[ELITE_MILITIA];
			break;
		}
	}

	return 0;
}

void InitPMCData( )
{
	pmcdata[0].usBodyType = Random( ADULTFEMALEMONSTER );
	pmcdata[0].usHair = Random( 5 );
	pmcdata[0].usSkin = Random( 4 );
	pmcdata[0].usVest = 7;
	pmcdata[0].usPants = 4;

	pmcdata[0].usToHire = 0;
	
	pmcdata[1].usBodyType = Random( ADULTFEMALEMONSTER );
	pmcdata[1].usHair = Random( 5 );
	pmcdata[1].usSkin = Random( 4 );
	pmcdata[1].usVest = 6;
	pmcdata[1].usPants = 4;

	pmcdata[1].usToHire = 0;
}

CHAR16 gPMCSectorNamesstr[10][64];

BOOLEAN EnterPMCContract( )
{
	InitDefaults_PMC( );

	InitPMCData();
		
	UINT8 count = 0;
	std::vector<std::pair<INT16, STR16> > dropdownvector;

	for ( INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( INT16 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			if ( SectorOursAndPeaceful( sX, sY, 0 ) )
			{
				// if militia can land in this sector, add it to our list
				for ( UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt )
				{
					// Is this facility here?
					if ( gFacilityLocations[SECTOR( sX, sY )][cnt].fFacilityHere )
					{
						// we determine wether this is a prison by checking for usPrisonBaseLimit
						if ( gFacilityTypes[cnt].usFacilityFlags & FACILITYTYPE_PMCENTRYPOINT )
						{
							GetSectorIDString( sX, sY, 0, gPMCSectorNamesstr[count], TRUE );
							gPMCSectorNamesstr[count][63] = '/0';
						
							dropdownvector.push_back( std::make_pair( SECTOR( sX, sY ), gPMCSectorNamesstr[count] ) );

							++count;

							break;
						}
					}
				}
			}

			if ( count >= 10 )
				break;
		}

		if ( count >= 10 )
			break;
	}

	if ( !dropdownvector.empty( ) )
	{
		DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).SetEntries( dropdownvector );
		DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).SetHelpText( szPMCWebSite[TEXT_PMC_CONTRACT_DROPDOWNTEXT] );
		DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).Create( LAPTOP_SCREEN_UL_X + 200, MCA_START_CONTENT_Y + 150 );
	}
	else
	{
		DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).ClearEntries( );
	}

	return(TRUE);
}

void ExitPMCContract( )
{
	for ( int i = 0; i < 4; ++i )
	{
		if ( gPMCButtonCreated[i] )
		{
			// delete militia panel bottom
			RemoveButton( gPMCButton[i] );
			UnloadButtonImage( gPMCButtonImage[i] );

			gPMCButtonCreated[i] = FALSE;
		}
	}

	DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).Destroy( );

	if ( gPMCHireButtonCreated )
	{
		RemoveButton( gPMCHireButton );
		gPMCHireButtonCreated = FALSE;
	}
	
	RemoveDefaults_PMC( );
}

void HandlePMCContract( )
{
	if ( fPMCContractRedraw )
	{
		RenderPMCContract( );
		fPMCContractRedraw = FALSE;
	}
}


BOOLEAN RenderMilitiaExamples( )
{
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT hHandle;
	UINT32 uiGraphicHandle;

	// regular militia
	UINT16 usPosX = LAPTOP_SCREEN_UL_X;
	UINT16 usPosY = MCA_START_CONTENT_Y + 20;

	// load it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	// Pick images based on body type
	switch ( pmcdata[0].usBodyType )
	{
	case REGMALE:		FilenameForBPP( "Laptop\\IMPSmallMercImages.sti", VObjectDesc.ImageFile );	break;
	case BIGMALE:		FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
	case STOCKYMALE:	FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
	case REGFEMALE:		FilenameForBPP( "Laptop\\IMPFemaleMercImages.sti", VObjectDesc.ImageFile ); break;
	default:
		return FALSE;
		break;
	}

	// safety check if loaded
	CHECKF( AddVideoObject( &VObjectDesc, &uiGraphicHandle ) );

	// Get it
	GetVideoObject( &hHandle, uiGraphicHandle );

	// show background first
	BltVideoObject( FRAME_BUFFER, hHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	// show skin (1 - 4)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[0].usSkin + 1), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show head (5 - 9)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[0].usHair + 5), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show vest (10 - 20)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[0].usVest + 10), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show pants (21 - 26)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[0].usPants + 21), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	DeleteVideoObjectFromIndex( uiGraphicHandle );

	// veteran militia
	usPosX = LAPTOP_SCREEN_UL_X + 250;

	// load it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	// Pick images based on body type
	switch ( pmcdata[1].usBodyType )
	{
	case REGMALE:		FilenameForBPP( "Laptop\\IMPSmallMercImages.sti", VObjectDesc.ImageFile );	break;
	case BIGMALE:		FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
	case STOCKYMALE:	FilenameForBPP( "Laptop\\IMPBigMercImages.sti", VObjectDesc.ImageFile );	break;
	case REGFEMALE:		FilenameForBPP( "Laptop\\IMPFemaleMercImages.sti", VObjectDesc.ImageFile ); break;
	default:
		return FALSE;
		break;
	}

	// safety check if loaded
	CHECKF( AddVideoObject( &VObjectDesc, &uiGraphicHandle ) );

	// Get it
	GetVideoObject( &hHandle, uiGraphicHandle );

	// show background first
	BltVideoObject( FRAME_BUFFER, hHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	// show skin (1 - 4)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[1].usSkin + 1), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show head (5 - 9)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[1].usHair + 5), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show vest (10 - 20)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[1].usVest + 10), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
	// show pants (21 - 26)
	BltVideoObject( FRAME_BUFFER, hHandle, (pmcdata[1].usPants + 21), usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	DeleteVideoObjectFromIndex( uiGraphicHandle );

	return (TRUE);
}

void PMCButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			if ( btn->IDNum == gPMCButton[0] )
				pmcdata[0].usToHire = min( pmcdata[0].usToHire + 1, gPMCData.usRegularsAvailable );
			else if ( btn->IDNum == gPMCButton[1] )
				pmcdata[0].usToHire = max( pmcdata[0].usToHire - 1, 0 );
			else if ( btn->IDNum == gPMCButton[2] )
				pmcdata[1].usToHire = min( pmcdata[1].usToHire + 1, gPMCData.usVeteransAvailable );
			else if ( btn->IDNum == gPMCButton[3] )
				pmcdata[1].usToHire = max( pmcdata[1].usToHire - 1, 0 );

			RenderPMCContract();
		}
	}
}

void PMCConfirmationCallback( UINT8 val )
{
	fPMCContractRedraw = TRUE;
}

void PMCTeamHireCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// only proceed if we selected people
		if ( pmcdata[0].usToHire || pmcdata[1].usToHire )
		{
			AddTransactionToPlayersBook( PMC_CONTRACT, 0, GetWorldTotalMin( ), (-1) * (INT32)(pmcdata[0].usToHire * GetMilitiaCostPMC( REGULAR_MILITIA ) + pmcdata[1].usToHire * GetMilitiaCostPMC( ELITE_MILITIA )) );
		
			PMCReinforcesHireEvent hiringevent;

			// look for a free ID
			UINT8 id = 0;
			BOOLEAN found = FALSE;
			for ( UINT8 i = 0; i < 255; ++i )
			{
				found = TRUE;

				std::vector<PMCReinforcesHireEvent>::iterator itend = gPMCHiringEvents.end();
				for ( std::vector<PMCReinforcesHireEvent>::iterator it = gPMCHiringEvents.begin( ); it != itend; ++it )
				{
					if ( (*it).usId == id )
					{
						++id;
						found = FALSE;
						break;
					}
				}

				if ( found == TRUE )
					break;
			}

			hiringevent.usId = id;
			hiringevent.usRegulars = pmcdata[0].usToHire;
			hiringevent.usVeterans = pmcdata[1].usToHire;
			hiringevent.usSectorToArrive = (UINT8)DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).GetSelectedEntryKey( );
			hiringevent.usTimeToArrive = GetWorldTotalMin() + 1440;

			gPMCData.usRegularsHired += hiringevent.usRegulars;
			gPMCData.usVeteransHired += hiringevent.usVeterans;
			gPMCData.usTotalMoneyEarned += pmcdata[0].usToHire * GetMilitiaCostPMC( REGULAR_MILITIA ) + pmcdata[1].usToHire * GetMilitiaCostPMC( ELITE_MILITIA );

			gPMCHiringEvents.push_back( hiringevent );

			// The ETA is, well, just an ETA - randomise it a bit
			UINT32 realarrivaltime = hiringevent.usTimeToArrive - 60 + Random(180);
			AddStrategicEvent( EVENT_PMC_REINFORCEMENT_ARRIVAL, realarrivaltime, hiringevent.usId );
		
			gPMCData.usRegularsAvailable -= pmcdata[0].usToHire;
			pmcdata[0].usToHire = 0;

			gPMCData.usVeteransAvailable -= pmcdata[1].usToHire;
			pmcdata[1].usToHire = 0;		
				
			CHAR16 sString[256];
			swprintf( sString, szPMCWebSite[TEXT_PMC_CONFIRMATION], ((GetWorldTotalMin( ) + 1440) % 1440) / 60, ((GetWorldTotalMin( ) + 1440) % 1440) % 60 );

			DoLowerScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, PMCConfirmationCallback );
		}
	}
}

void RenderPMCContract( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayDefaults_PMC( );

	SetFontShadow( MERCOMP_FONT_SHADOW );
	
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	swprintf( sText, szPMCWebSite[TEXT_PMC_CONTRACT_TEAM_INTRO] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	// show example pics of militia
	RenderMilitiaExamples( );

	for ( int i = 0; i < 4; ++i )
	{
		if ( gPMCButtonCreated[i] )
		{
			// delete militia panel bottom
			RemoveButton( gPMCButton[i] );
			UnloadButtonImage( gPMCButtonImage[i] );

			gPMCButtonCreated[i] = FALSE;
		}
	}

	gPMCButtonImage[0] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 0, -1, 0, -1 );
	gPMCButtonImage[1] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 1, -1, 1, -1 );
	gPMCButtonImage[2] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 0, -1, 0, -1 );
	gPMCButtonImage[3] = LoadButtonImage( "INTERFACE\\plusminusbuttons.sti", -1, 1, -1, 1, -1 );

	// regulars
	usPosX = LAPTOP_SCREEN_UL_X + 120;
	usPosY = MCA_START_CONTENT_Y + 20;
	
	swprintf( sText, szPMCWebSite[TEXT_PMC_REGULAR] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosY += 10;

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL], gPMCData.usRegularsAvailable, GetMilitiaCostPMC( REGULAR_MILITIA ) );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	gPMCButton[0] = QuickCreateButton( gPMCButtonImage[0], usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PMCButtonCallback );
	gPMCButtonCreated[0] = TRUE;
	gPMCButton[1] = QuickCreateButton( gPMCButtonImage[1], usPosX + 20, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PMCButtonCallback );
	gPMCButtonCreated[1] = TRUE;

	usPosY += 20;

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL + 1], pmcdata[0].usToHire );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL + 2], pmcdata[0].usToHire * GetMilitiaCostPMC( REGULAR_MILITIA ) );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		
	// veterans
	usPosX = LAPTOP_SCREEN_UL_X + 370;
	usPosY = MCA_START_CONTENT_Y + 20;

	swprintf( sText, szPMCWebSite[TEXT_PMC_VETERAN] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosY += 10;

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL], gPMCData.usVeteransAvailable, GetMilitiaCostPMC( ELITE_MILITIA ) );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	gPMCButton[2] = QuickCreateButton( gPMCButtonImage[2], usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PMCButtonCallback );
	gPMCButtonCreated[2] = TRUE;
	gPMCButton[3] = QuickCreateButton( gPMCButtonImage[3], usPosX + 20, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1, BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PMCButtonCallback );
	gPMCButtonCreated[3] = TRUE;

	usPosY += 20;

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL + 1], pmcdata[1].usToHire );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	swprintf( sText, szPMCWebSite[TEXT_PMC_DETAIL + 2], pmcdata[1].usToHire * GetMilitiaCostPMC( ELITE_MILITIA ) );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y + 158;

	swprintf( sText, szPMCWebSite[TEXT_PMC_SELECTAREA] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosY += 10;

	swprintf( sText, szPMCWebSite[TEXT_PMC_TOTALCOST], pmcdata[0].usToHire * GetMilitiaCostPMC( REGULAR_MILITIA ) + pmcdata[1].usToHire * GetMilitiaCostPMC( ELITE_MILITIA ) );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		
	swprintf( sText, szPMCWebSite[TEXT_PMC_ETA], ((GetWorldTotalMin( ) + 1440) % 1440) / 60, ((GetWorldTotalMin( ) + 1440) % 1440) % 60 );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	if ( gPMCHireButtonCreated )
	{
		RemoveButton( gPMCHireButton );
		gPMCHireButtonCreated = FALSE;
	}

	if ( DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance().HasEntries() )
	{
		gPMCHireButton = CreateTextButton( szPMCWebSite[TEXT_PMC_CONTRACTBUTTON], CAMPHIS_FONT_MED, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
														usPosX, usPosY, 100, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, PMCTeamHireCallback );
		gPMCHireButtonCreated = TRUE;
	}
	else
	{
		swprintf( sText, szPMCWebSite[TEXT_PMC_NODROPOFF] );
		usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}
	
	usPosY += 50;

	// if we hired personnel and they are inbound, show the next deployment
	if ( !gPMCHiringEvents.empty() )
	{
		CHAR16 sectorname[64];
		GetSectorIDString( SECTORX( gPMCHiringEvents[0].usSectorToArrive ), SECTORY( gPMCHiringEvents[0].usSectorToArrive ), 0, sectorname, TRUE );
		swprintf( sText, szPMCWebSite[TEXT_PMC_NEXTDEPLOYMENT], gPMCHiringEvents[0].usRegulars, gPMCHiringEvents[0].usVeterans, sectorname, (gPMCHiringEvents[0].usTimeToArrive % 1440) / 60, (gPMCHiringEvents[0].usTimeToArrive % 1440) % 60, 1 + (gPMCHiringEvents[0].usTimeToArrive / 1440) );
		usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	DropDownTemplate<DROPDOWNNR_PMC_LOCATION>::getInstance( ).Display( );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// CONTRACT PAGE ////////////////////////////////


void GameInitPMC()
{
	gPMCData.usTotalMoneyEarned = 0;
	gPMCData.usRegularsHired = 0;
	gPMCData.usVeteransHired = 0;
	gPMCData.usRegularsAvailable = Random( gGameExternalOptions.usPMCMaxRegulars );
	gPMCData.usVeteransAvailable = Random( gGameExternalOptions.usPMCMaxVeterans );
}

// the PMC replenishes personnel
void HourlyUpdatePMC( )
{
	if ( Chance( 50 ) )
		gPMCData.usRegularsAvailable = min( gPMCData.usRegularsAvailable + 1, gGameExternalOptions.usPMCMaxRegulars );

	if ( Chance( 30 ) )
		gPMCData.usVeteransAvailable = min( gPMCData.usVeteransAvailable + 1, gGameExternalOptions.usPMCMaxVeterans );
}

void HandlePMCArrival( UINT8 usId )
{
	std::vector<PMCReinforcesHireEvent>::iterator itend = gPMCHiringEvents.end( );
	for ( std::vector<PMCReinforcesHireEvent>::iterator it = gPMCHiringEvents.begin( ); it != itend; ++it )
	{
		if ( (*it).usId == usId )
		{
			// the militia we ordered have arrived!
			StrategicAddMilitiaToSector( SECTORX( (*it).usSectorToArrive ), SECTORY( (*it).usSectorToArrive ), REGULAR_MILITIA, (*it).usRegulars );
			StrategicAddMilitiaToSector( SECTORX( (*it).usSectorToArrive ), SECTORY( (*it).usSectorToArrive ), ELITE_MILITIA, (*it).usVeterans );

			// Flugente: create individual militia			
			for ( int i = 0; i < (*it).usRegulars; ++i )
				CreateNewIndividualMilitia( REGULAR_MILITIA, MO_PMC, (*it).usSectorToArrive );

			for ( int i = 0; i < (*it).usVeterans; ++i )
				CreateNewIndividualMilitia( ELITE_MILITIA, MO_PMC, (*it).usSectorToArrive );

			// if this is in the current sector, militia will be updated
			ResetMilitia( );

			// notify the player that his reinforcements have arrived
			GetSectorIDString( SECTORX( (*it).usSectorToArrive ), SECTORY( (*it).usSectorToArrive ), 0, gPMCSectorNamesstr[0], TRUE );

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPMCWebSite[TEXT_PMC_ARRIVAL], gPMCSectorNamesstr[0] );

			// there is a chance that loyalty in cities will be lowered - townsfolk might dislike massive rise in foreign mercenaries
			// note: this is the decrease in hundreths of loyalty points, and one basic town loyalty point is worth 500 hundreths
			DecrementTownLoyaltyEverywhere( 50 * (*it).usRegulars + 75 * (*it).usVeterans );

			// delete event, we're done here
			gPMCHiringEvents.erase( it );

			return;
		}
	}
}

// it could happen that we load events incorrectly. To detect that early, we write an extra eyecacher befoer and after the list of events. Should we fail to read it correctly, we will knowm where to look. 
UINT32 gPMCSaveCheck = 0xF123456F;
BOOLEAN SavePMC( HWFILE hwFile )
{
	UINT32 uiNumBytesWritten = 0;

	if ( !FileWrite( hwFile, &gPMCData, sizeof(PMCGlobalData), &uiNumBytesWritten ) )
		return(FALSE);

	// in order to save a vector, we first save its size and then its content
	UINT32 size = gPMCHiringEvents.size( );
	if ( !FileWrite( hwFile, &size, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	if ( !FileWrite( hwFile, &gPMCSaveCheck, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	for ( UINT32 i = 0; i < size; ++i )
	{
		if ( !FileWrite( hwFile, &gPMCHiringEvents[i], sizeof(PMCReinforcesHireEvent), &uiNumBytesWritten ) )
			return(FALSE);
	}

	if ( !FileWrite( hwFile, &gPMCSaveCheck, sizeof(UINT32), &uiNumBytesWritten ) )
		return(FALSE);

	return(TRUE);
}

BOOLEAN LoadPMC( HWFILE hwFile )
{
	if ( guiCurrentSaveGameVersion >= PMC_WEBSITE )
	{
		gPMCHiringEvents.clear();

		UINT32 numBytesRead = 0;

		numBytesRead = ReadFieldByField( hwFile, &gPMCData, sizeof(gPMCData), sizeof(PMCGlobalData), numBytesRead );

		UINT32 size = 0;
		numBytesRead = ReadFieldByField( hwFile, &size, sizeof(size), sizeof(UINT32), numBytesRead );

		UINT32 safetycheck1 = 0;
		numBytesRead = ReadFieldByField( hwFile, &safetycheck1, sizeof(safetycheck1), sizeof(UINT32), numBytesRead );

		for ( UINT32 i = 0; i < size; ++i )
		{
			PMCReinforcesHireEvent event;
			numBytesRead = ReadFieldByField( hwFile, &event, sizeof(event), sizeof(PMCReinforcesHireEvent), 0 );

			gPMCHiringEvents.push_back( event );
		}

		UINT32 safetycheck2 = 0;
		numBytesRead = ReadFieldByField( hwFile, &safetycheck2, sizeof(safetycheck2), sizeof(UINT32), numBytesRead );

		if ( safetycheck1 != gPMCSaveCheck || safetycheck2 != gPMCSaveCheck )
		{
			ScreenMsg( FONT_MCOLOR_LTGREEN, MSG_INTERFACE, L"Eyecatcher corruption detected while loading PMC data!" );
			return FALSE;
		}
	}
	else
	{
		// if loading an older savegame, initialise the PMC
		GameInitPMC();
	}

	return TRUE;
}