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
#include "MilitiaWebsite.h"
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

#include "BaseTable.h"
#include "Soldier Profile.h"
#include "Auto Resolve.h"
#include "MilitiaIndividual.h"
#include "Animation Data.h"
#include "MilitiaInterface.h"
#include "Interface Items.h"
#include "InterfaceItemImages.h"
#include "CampaignStats.h"
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

extern UINT32	gusMostImportantPage;

UINT32 gARFacesLib = 0;

void InitDefaults_MilitiaWebsite( )
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

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "Interface\\SmFaces.sti", VObjectDesc.ImageFile );
	CHECKV( AddVideoObject( &VObjectDesc, &gARFacesLib ) );
}

void DisplayDefaults_MilitiaWebsite( )
{
	HVOBJECT	hPixHandle;
	GetVideoObject( &hPixHandle, guiMercCompareLogoImage );

	SetFontShadow( NO_SHADOW );

	CHAR16 sText[800];
	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;

	WebPageTileBackground( 4, 4, BACKGROUND_WIDTH, CAMPAIGN_HISTORY_BACKGROUND_HEIGHT, guiInsuranceBackGround );

	//Display the title slogan
	swprintf( sText, szMilitiaWebSite[TEXT_MILITIAWEBSITE_WEBSITENAME] );
	DrawTextToScreen( sText, usPosX, CAMPAIGN_HISTORY_BIG_TITLE_Y, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	//Display the subtitle slogan
	swprintf( sText, szMilitiaWebSite[TEXT_MILITIAWEBSITE_SLOGAN] );
	DrawTextToScreen( sText, usPosX, CAMPAIGN_HISTORY_SUBTITLE_Y, 0, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	usPosY = CAMPAIGN_HISTORY_SUBTITLE_Y + CAMPAIGN_HISTORY_LINK_STEP_Y;

	// closing line that separates header from individual page
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_LR_X, usPosY - 2, FROMRGB( 255, 127, 0 ) );

	usPosX = LAPTOP_SCREEN_LR_X - 60;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	BltVideoObject( FRAME_BUFFER, hPixHandle, 4, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	SetFontShadow( NO_SHADOW );
}

void RemoveDefaults_MilitiaWebsite( )
{
	DeleteVideoObjectFromIndex( guiInsuranceBackGround );
	DeleteVideoObjectFromIndex( guiInsuranceBigRedLineImage );
	DeleteVideoObjectFromIndex( guiMercCompareBulletImage );
	DeleteVideoObjectFromIndex( guiMercCompareLogoImage );
}

////////////////////////// MAIN PAGE ////////////////////////////////
BOOLEAN gfMilitiaWebsiteMainRedraw = FALSE;

BOOLEAN gfMilitiaDetailsOpen = FALSE;

// filter data
BOOLEAN gfMilitiaWebsiteRedoFilters = FALSE;

enum InidivualMilitiaLifeState
{
	IMLS_ALL,
	IMLS_DEAD,
	IMLS_ALIVE,
	IMLS_FIRED
};

enum InidivualMilitiaRankState
{
	IMRS_ALL,
	IMRS_GREEN,
	IMRS_REGULAR,
	IMRS_ELITE
};

enum InidivualMilitiaOriginState
{
	IMOS_ALL,
	IMOS_ARULCO,
	IMOS_PMC,
	IMOS_DEFECTOR
};

CHAR16 gInidividualMilitiaWebsiteSectorNamesstr[256][128];

std::vector<std::pair<INT16, STR16> > GetMilitiaSectorDropVector( )
{
	std::vector<std::pair<INT16, STR16> > sectorvector;

	// 'Everywhere' is always an option
	sectorvector.push_back( std::make_pair( -1, szIdividualMilitiaWebsiteFilterText_Sector[0] ) );

	std::set<UINT8> sectorset;

	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != gIndividualMilitiaVector.end( ); ++it )
	{
		sectorset.insert( (*it).sector );
	}

	for ( std::set<UINT8>::iterator it = sectorset.begin( ); it != sectorset.end(); ++it )
	{
		GetSectorIDString( SECTORX( (*it) ), SECTORY( (*it) ), 0, gInidividualMilitiaWebsiteSectorNamesstr[(*it)], FALSE );

		sectorvector.push_back( std::make_pair( (INT16)(*it), gInidividualMilitiaWebsiteSectorNamesstr[(*it)] ) );
	}

	return sectorvector;
}

// later on, make this an array with a vector for each sector
std::vector<UINT32> gIndividualMilitiaFilteredIdsVector;

void FilterIndividualMilitia( InidivualMilitiaLifeState aLifeState, InidivualMilitiaRankState aRankState, InidivualMilitiaOriginState aOriginState, INT16 aSector )
{
	gIndividualMilitiaFilteredIdsVector.clear();

	std::vector<MILITIA>::iterator itend = gIndividualMilitiaVector.end( );
	for ( std::vector<MILITIA>::iterator it = gIndividualMilitiaVector.begin( ); it != itend; ++it )
	{
		if ( ( aLifeState == IMLS_DEAD && !((*it).flagmask & MILITIAFLAG_DEAD) ) 
			 || (aLifeState == IMLS_ALIVE && ((*it).flagmask & MILITIAFLAG_DEAD)) )
			continue;

		if ( (aLifeState == IMLS_FIRED && !((*it).flagmask & MILITIAFLAG_FIRED))
			 || (aLifeState != IMLS_FIRED && aLifeState != IMLS_ALL && ((*it).flagmask & MILITIAFLAG_FIRED)) )
			 continue;

		if ( (aRankState == IMRS_GREEN && (*it).militiarank != GREEN_MILITIA) ||
			 (aRankState == IMRS_REGULAR && (*it).militiarank != REGULAR_MILITIA) ||
			 (aRankState == IMRS_ELITE && (*it).militiarank != ELITE_MILITIA) )
			 continue;

		if ( (aOriginState == IMOS_ARULCO && (*it).origin != MO_ARULCO) ||
			 (aOriginState == IMOS_PMC && (*it).origin != MO_PMC) ||
			 (aOriginState == IMOS_DEFECTOR && (*it).origin != MO_DEFECTOR) )
			 continue;

		if ( aSector > 0 && (UINT8)aSector != (*it).sector )
			continue;

		gIndividualMilitiaFilteredIdsVector.push_back( (*it).id );
	}
}

// on dropdown change, redraw and redo filter selection
template<>	void	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; gfMilitiaWebsiteRedoFilters = TRUE; }
template<>	void	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; gfMilitiaWebsiteRedoFilters = TRUE; }
template<>	void	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; gfMilitiaWebsiteRedoFilters = TRUE; }
template<>	void	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; gfMilitiaWebsiteRedoFilters = TRUE; }

BOOLEAN EnterMilitiaWebsiteMain( )
{
	InitDefaults_MilitiaWebsite( );

	if ( !gGameExternalOptions.fIndividualMilitia )
		return TRUE;
	
	// we add several filter dropdowns here (otherwise this list will become tedious to use later on when there are hundreds of militia)
	{
		UINT16 usDropX = LAPTOP_SCREEN_UL_X + 4;
		{
			std::vector<std::pair<INT16, STR16> > filtervector;
			filtervector.push_back( std::make_pair( (INT16)IMLS_ALL, szIdividualMilitiaWebsiteFilterText_Dead[0] ) );
			filtervector.push_back( std::make_pair( (INT16)IMLS_DEAD, szIdividualMilitiaWebsiteFilterText_Dead[1] ) );
			filtervector.push_back( std::make_pair( (INT16)IMLS_ALIVE, szIdividualMilitiaWebsiteFilterText_Dead[2] ) );
			filtervector.push_back( std::make_pair( (INT16)IMLS_FIRED, szIdividualMilitiaWebsiteFilterText_Dead[3] ) );

			if ( !filtervector.empty( ) )
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).SetEntries( filtervector );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance().SetSelectedEntryKey( IMLS_ALIVE );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).Create( usDropX, MCA_START_CONTENT_Y + 4 );
				usDropX = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).GetLastX( ) + 10;
			}
			else
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance().ClearEntries();
			}
		}

		{
			std::vector<std::pair<INT16, STR16> > filtervector;
			filtervector.push_back( std::make_pair( (INT16)IMRS_ALL, szIdividualMilitiaWebsiteFilterText_Rank[0] ) );
			filtervector.push_back( std::make_pair( (INT16)IMRS_GREEN, szIdividualMilitiaWebsiteFilterText_Rank[1] ) );
			filtervector.push_back( std::make_pair( (INT16)IMRS_REGULAR, szIdividualMilitiaWebsiteFilterText_Rank[2] ) );
			filtervector.push_back( std::make_pair( (INT16)IMRS_ELITE, szIdividualMilitiaWebsiteFilterText_Rank[3] ) );

			if ( !filtervector.empty( ) )
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).SetEntries( filtervector );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).SetSelectedEntryKey( IMRS_ALL );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).Create( usDropX, MCA_START_CONTENT_Y + 4 );
				usDropX = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).GetLastX( ) + 10;
			}
			else
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).ClearEntries( );
			}
		}

		{
			std::vector<std::pair<INT16, STR16> > filtervector;
			filtervector.push_back( std::make_pair( (INT16)IMOS_ALL, szIdividualMilitiaWebsiteFilterText_Origin[0] ) );
			filtervector.push_back( std::make_pair( (INT16)IMOS_ARULCO, szIdividualMilitiaWebsiteFilterText_Origin[1] ) );
			filtervector.push_back( std::make_pair( (INT16)IMOS_PMC, szIdividualMilitiaWebsiteFilterText_Origin[2] ) );
			filtervector.push_back( std::make_pair( (INT16)IMOS_DEFECTOR, szIdividualMilitiaWebsiteFilterText_Origin[3] ) );

			if ( !filtervector.empty( ) )
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).SetEntries( filtervector );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).SetSelectedEntryKey( IMOS_ALL );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).Create( usDropX, MCA_START_CONTENT_Y + 4 );
				usDropX = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).GetLastX( ) + 10;
			}
			else
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).ClearEntries( );
			}
		}

		{
			std::vector<std::pair<INT16, STR16> > filtervector = GetMilitiaSectorDropVector();
			
			if ( !filtervector.empty( ) )
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).SetEntries( filtervector );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).SetSelectedEntryKey( -1 );
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).Create( usDropX, MCA_START_CONTENT_Y + 4 );
				usDropX = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).GetLastX( ) + 10;
			}
			else
			{
				DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).ClearEntries( );
			}
		}
	}

	INT16 key_dead = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).GetSelectedEntryKey( );
	INT16 key_rank = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).GetSelectedEntryKey( );
	INT16 key_origin = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).GetSelectedEntryKey( );
	INT16 key_sector = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).GetSelectedEntryKey( );

	// as we might filter out militia, we use a vector that contains all currently used militia IDs, and access data via that
	FilterIndividualMilitia( (InidivualMilitiaLifeState)key_dead, (InidivualMilitiaRankState)key_rank, (InidivualMilitiaOriginState)key_origin, key_sector );
			
	gTestPanel3.Create( LAPTOP_SCREEN_UL_X, MCA_START_CONTENT_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
	gTestPanel3.SetColorLine( Get16BPPColor( FROMRGB( 128, 128, 128 ) ) );
	gTestPanel3.SetColorLineShadow( Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );
	gTestPanel3.SetColorMarked( Get16BPPColor( FROMRGB( 200, 169, 87 ) ) );
	gTestPanel3.SetColorHighLight( Get16BPPColor( FROMRGB( 235, 222, 171 ) ) );

	TestTableTemplate<3>::getInstance( ).Create( LAPTOP_SCREEN_UL_X + 4, MCA_START_CONTENT_Y + 44, LAPTOP_SCREEN_LR_X - 4, LAPTOP_SCREEN_WEB_LR_Y - 4 );

	gTestPanel3.RegisterMemberThingy( &TestTableTemplate<3>::getInstance( ) );

	// militia details (opens upon interacting with militia list)
	militiaindividualmainwidget.Create( LAPTOP_SCREEN_UL_X + 50, MCA_START_CONTENT_Y + 40, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
		
	gfMilitiaDetailsOpen = FALSE;

	militiaindividualmainwidget.SetMouseRegionActive( FALSE );
	TestTableTemplate<3>::getInstance( ).SetMouseRegionActive( TRUE );

	return(TRUE);
}

void ExitMilitiaWebsiteMain( )
{
	militiaindividualmainwidget.Destroy( );

	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).Destroy( );
	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).Destroy( );
	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).Destroy( );
	DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).Destroy( );

	gTestPanel3.Destroy( );

	RemoveDefaults_MilitiaWebsite( );

	DeleteVideoObjectFromIndex( gARFacesLib );
}

void HandleMilitiaWebsiteMain( )
{
	if ( gfMilitiaWebsiteMainRedraw )
	{
		if ( gGameExternalOptions.fIndividualMilitia )
		{
			if ( gfMilitiaWebsiteRedoFilters )
			{
				INT16 key_dead = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).GetSelectedEntryKey( );
				INT16 key_rank = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).GetSelectedEntryKey( );
				INT16 key_origin = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).GetSelectedEntryKey( );
				INT16 key_sector = DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).GetSelectedEntryKey( );

				// as we might filter out militia, we use a vector that contains all currently used militia IDs, and access data via that
				FilterIndividualMilitia( (InidivualMilitiaLifeState)key_dead, (InidivualMilitiaRankState)key_rank, (InidivualMilitiaOriginState)key_origin, key_sector );

				TestTableTemplate<3>::getInstance( ).Create( TestTableTemplate<3>::getInstance( ).GetX( ), TestTableTemplate<3>::getInstance( ).GetY( ),
														   TestTableTemplate<3>::getInstance( ).GetX( ) + TestTableTemplate<3>::getInstance( ).GetWidth( ), 
														   TestTableTemplate<3>::getInstance( ).GetY( ) + TestTableTemplate<3>::getInstance( ).GetHeight() );

				gfMilitiaWebsiteRedoFilters = FALSE;
			}
		}

		RenderMilitiaWebsiteMain();

		gfMilitiaWebsiteMainRedraw = FALSE;
	}
}

void RenderMilitiaWebsiteMain( )
{
	DisplayDefaults_MilitiaWebsite( );

	SetFontShadow( NO_SHADOW );
	
	if ( gGameExternalOptions.fIndividualMilitia )
	{
		gTestPanel3.Display( );

		DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_DEAD>::getInstance( ).Display( );
		DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_RANK>::getInstance( ).Display( );
		DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_ORIGIN>::getInstance( ).Display( );
		DropDownTemplate<DROPDOWN_MILTIAWEBSITE_FILTER_SECTOR>::getInstance( ).Display( );

		if ( gfMilitiaDetailsOpen )
			militiaindividualmainwidget.Display( );
	}

	SetFontShadow( NO_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// MAIN PAGE ////////////////////////////////


BOOLEAN gMilitiaDetailOn = FALSE;

STR16 Profilenamegetter( UINT32 aNum )
{
	if ( aNum < gIndividualMilitiaFilteredIdsVector.size( ) )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[aNum];

		MILITIA militia;
		if ( GetMilitia( militiaid, &militia ) )
			return militia.GetName( );
	}

	return L"No entry found in database";
}

// a militia's name is written in a different colour, depending on their rank and whether they are dead
UINT8 MilitiaNameTextColourGetter( UINT32 aNum )
{
	if ( aNum < gIndividualMilitiaFilteredIdsVector.size() )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[aNum];

		MILITIA militia;
		if ( GetMilitia( militiaid, &militia ) )
		{
			if ( militia.flagmask & MILITIAFLAG_DEAD )
				return 243;

			switch ( militia.militiarank )
			{
			case GREEN_MILITIA:		return 184; break;
			case REGULAR_MILITIA:	return 201; break;
			case ELITE_MILITIA:		return 203; break;
			}
		}
	}

	return 2;
}

STR16 Sectornamegetter( UINT32 aNum )
{
	if ( aNum < gIndividualMilitiaFilteredIdsVector.size( ) )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[aNum];

		MILITIA militia;
		if ( GetMilitia( militiaid, &militia ) )
			return militia.GetSector( );
	}

	return L"No entry found in database";
}

UINT32 gusCurrentMilitia = 0;
CHAR16 gMilitiaOperationText[100];
STR16 OperationText( UINT32 aNum )
{
	MILITIA militia;
	if ( GetMilitia( gusCurrentMilitia, &militia ) && aNum < militia.history.size( ) )
	{
		MILITIA_BATTLEREPORT battlereport = militia.history[aNum];
		
		UINT32 day		= battlereport.id / (1440);
		UINT32 hours	= (battlereport.id - day * 1440) / 60;
		UINT32 minutes  = battlereport.id - day * 1440 - hours * 60;

		STR16 operationame = GetIncidentName( battlereport.id );

		if ( battlereport.flagmask & (MILITIA_BATTLEREPORT_FLAG_RECRUITED| MILITIA_BATTLEREPORT_FLAG_RECRUITED_TURNCOAT) )
		{
			STR16 originsectorstr = militia.GetOriginSector( );

			if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_RECRUITED_TURNCOAT )
				swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[11], day, hours, minutes, originsectorstr );
			else if ( militia.origin == MO_PMC )
				swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[8], day, hours, minutes, originsectorstr );
			else if ( militia.origin == MO_DEFECTOR )
				swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[9], day, hours, minutes, originsectorstr );
			else
				swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[1], day, hours, minutes, originsectorstr );
		}
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_PROMOTED )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[2], day, hours, minutes );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_DIED )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[3], operationame );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_WOUNDED_COMA )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[6], operationame );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[7], operationame );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_WOUNDED_HEAVY )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[5], operationame );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_WOUNDED_SMALL )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[4], operationame );
		else if ( battlereport.flagmask & MILITIA_BATTLEREPORT_FLAG_FIRED )
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[10], day, hours, minutes );
		else
			swprintf( gMilitiaOperationText, szIndividualMilitiaBattleReportText[0], operationame );

		return gMilitiaOperationText;
	}

	return L"No entry found in database";
}

CHAR16	gItemLongNamegetterText[800];
STR16 ItemLongNamegetter( UINT32 aNum )
{
	if ( aNum < gIndividualMilitiaVector.size( ) )
	{
		swprintf( gItemLongNamegetterText, L"%s", Item[aNum].szLongItemName );

		return gItemLongNamegetterText;
	}

	return L"unarmed";
}

void GetFaceData( UINT32 aMilitiaId, UINT32& arImageLib, UINT16& arImage )
{
	arImageLib = gARFacesLib;

	arImage = HUMAN_SKULL;

	MILITIA militia;
	if ( GetMilitia( aMilitiaId, &militia ) )
	{
		if ( militia.flagmask & MILITIAFLAG_DEAD )
			arImage = HUMAN_SKULL;
		else if ( militia.militiarank == GREEN_MILITIA)
			arImage = militia.bodytype == REGFEMALE ? MILITIA1F_FACE : MILITIA1_FACE;
		else if ( militia.militiarank == REGULAR_MILITIA)
			arImage = militia.bodytype == REGFEMALE ? MILITIA2F_FACE : MILITIA2_FACE;
		else if ( militia.militiarank == ELITE_MILITIA)
			arImage = militia.bodytype == REGFEMALE ? MILITIA3F_FACE : MILITIA3_FACE;
	}
}

void AutoResolveFaces( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )
{
	arImageLib = gARFacesLib;

	arImage = HUMAN_SKULL;

	if ( aNum < gIndividualMilitiaFilteredIdsVector.size( ) )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[aNum];

		GetFaceData( militiaid, arImageLib, arImage );
	}
}

/*void ItemImage( UINT32 usItem, UINT32& arImageLib, UINT16& arImage )
{
	if ( usItem != NONE )
	{
		INVTYPE* pItem = &Item[usItem];

		arImage = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;

		arImageLib = GetInterfaceGraphicForItem( pItem );
	}
}*/

/*extern UINT32 guTraitImages;

void TraitImage1( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )
{
	MILITIA& militia = GetMilitiabyNumber( aNum );

	arImageLib = guTraitImages;

	arImage = 3 * militia.traits[0];
}

void TraitImage2( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )
{
	MILITIA& militia = GetMilitiabyNumber( aNum );

	arImageLib = guTraitImages;

	arImage = 3 * militia.traits[1];
}

void TraitImage3( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )
{
	MILITIA& militia = GetMilitiabyNumber( aNum );

	arImageLib = guTraitImages;

	arImage = 3 * militia.traits[2];
}*/

void MilitiaHealthBar( UINT32 aId, UINT16& arCol1, UINT16& arVal1, UINT16& arCol2, UINT16& arVal2, UINT16& arCol3, UINT16& arVal3, UINT16& arCol4, UINT16& arVal4 )
{
	if ( aId < gIndividualMilitiaFilteredIdsVector.size() )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[aId];

		MILITIA militia;
		if ( GetMilitia( militiaid, &militia ) )
		{
			// we create a 'health bar' here
			// first a black background
			arCol1 = Get16BPPColor( FROMRGB( 0, 0, 0 ) );
			arVal1 = 100;

			// we might be bleeding - draw the potential life here
			arCol2 = Get16BPPColor( FROMRGB( 255, 255, 0 ) );
			arVal2 = 100;//militia.stats[HEALTH];

			// we might be wounded - draw the potential life here
			arCol3 = Get16BPPColor( FROMRGB( 255, 125, 125 ) );
			arVal3 = 100;//militia.stats[HEALTH];

			// the red life part that we really have
			arCol4 = Get16BPPColor( FROMRGB( 255, 0, 0 ) );
			arVal4 = UINT16( militia.healthratio );
		}
	}
}


void callbackmilitialist( INT32 usId )
{
	if ( usId < gIndividualMilitiaFilteredIdsVector.size() )
	{
		UINT32 militiaid = gIndividualMilitiaFilteredIdsVector[usId];

		militiaindividualmainwidget.SetMouseRegionActive( TRUE );
		militiaindividualmainwidget.SetId( (UINT32)militiaid );

		TestTableTemplate<3>::getInstance( ).SetMouseRegionActive( FALSE );
		TestTableTemplate<3>::getInstance( ).DestroyMouseRegions( );

		gfMilitiaDetailsOpen = TRUE;

		TestTableTemplate<3>::getInstance( ).SetRefresh( );
	}
}

void callbackmilitiadetail( INT32 usId )
{
	MILITIA militia;
	if ( GetMilitia( gusCurrentMilitia, &militia ) && usId < militia.history.size( ) )
	{
		MILITIA_BATTLEREPORT battlereport = militia.history[usId];

		// if this is a valid ID of an incident, go to its website, otherwise, do nothing
		INT32 incidentposition = GetPositionOfIncident( battlereport.id );
		if ( incidentposition >= 0 )
		{
			militiaindividualmainwidget.SetMouseRegionActive( FALSE );
			militiaindividualmainwidget.DestroyMouseRegions( );

			TestTableTemplate<3>::getInstance( ).SetMouseRegionActive( TRUE );
			TestTableTemplate<3>::getInstance( ).SetRefresh( );

			guiCurrentLaptopMode = LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT;
			gusMostImportantPage = incidentposition;
		}
	}
}

template<>	void	TestTableTemplate<1>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; }
template<>	void	TestTableTemplate<2>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; }
template<>	void	TestTableTemplate<3>::SetRefresh( )	{ gfMilitiaWebsiteMainRedraw = TRUE; }

template<>  void	TestTableTemplate<1>::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	ClearColumnDataProvider( );

	ColumnDataProvider imagecol( L"" );
	imagecol.SetRequiredHeigth( 27 );
	imagecol.SetRequiredLength( 40 );
	imagecol.SetMethodImage( AutoResolveFaces );
	imagecol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	imagecol.SetCallBackType( ColumnDataProvider::CDP_MILITIADETAIL );

	AddColumnDataProvider( imagecol );

	TestTable::Init( sX, sY, sX_End, sY_End );
}

template<>  void	TestTableTemplate<2>::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	ClearColumnDataProvider( );

	ColumnDataProvider namcol( szIdividualMilitiaWebsiteText[0] );
	namcol.SetMethodString( OperationText );
	
	MILITIA militia;
	if ( GetMilitia( gusCurrentMilitia, &militia ) )
		namcol.SetNumberOfEntries( militia.history.size( ) );
	else
		namcol.SetNumberOfEntries( 0 );

	namcol.SetCallBackType( ColumnDataProvider::CDP_MILITIADETAIL );

	AddColumnDataProvider( namcol );

	TestTable::Init( sX, sY, sX_End, sY_End );
}

template<>  void	TestTableTemplate<3>::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End )
{
	ClearColumnDataProvider( );

	if ( gGameExternalOptions.fIndividualMilitia_ManageHealth )
	{
		ColumnDataProvider healthbarcol( L"" );
		healthbarcol.SetMethodStatusBar( MilitiaHealthBar );
		healthbarcol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
		//healthbarcol.SetRequiredHeigth( 29 );
		healthbarcol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

		AddColumnDataProvider( healthbarcol );
	}

	/*// face
	ColumnDataProvider imagecol( L"" );
	imagecol.SetMethodImage( AutoResolveFaces );
	imagecol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	imagecol.SetRequiredHeigth( 27 );
	imagecol.SetRequiredLength( 35 );
	imagecol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( imagecol );*/

	// name
	ColumnDataProvider namcol( szIdividualMilitiaWebsiteText[12] );
	namcol.SetMethodString( Profilenamegetter );
	namcol.SetMethodTextColour( MilitiaNameTextColourGetter );
	namcol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	namcol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( namcol );

	// sector
	ColumnDataProvider sectorcol( szIdividualMilitiaWebsiteText[13] );
	sectorcol.SetMethodString( Sectornamegetter );
	sectorcol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	sectorcol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( sectorcol );

	/*// trait 1,2,3
	ColumnDataProvider traitcol1( L"" );
	traitcol1.SetMethodImage( TraitImage1 );
	traitcol1.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	traitcol1.SetRequiredHeigth( 20 );
	traitcol1.SetRequiredLength( 22 );
	traitcol1.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( traitcol1 );

	ColumnDataProvider traitcol2( L"" );
	traitcol2.SetMethodImage( TraitImage2 );
	traitcol2.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	traitcol2.SetRequiredHeigth( 20 );
	traitcol2.SetRequiredLength( 26 );
	traitcol2.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( traitcol2 );

	ColumnDataProvider traitcol3( L"" );
	traitcol3.SetMethodImage( TraitImage3 );
	traitcol3.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	traitcol3.SetRequiredHeigth( 20 );
	traitcol3.SetRequiredLength( 22 );
	traitcol3.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( traitcol3 );*/

	/*// weapon
	ColumnDataProvider itemimagecol( szIdividualMilitiaWebsiteText[14] );
	itemimagecol.SetMethodImage( ItemImage );
	itemimagecol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size( ) );
	itemimagecol.SetRequiredHeigth( 29 );
	itemimagecol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( itemimagecol );*/

	// name of weapon
	/*ColumnDataProvider itemnamecol( L"" );
	itemnamecol.SetMethodString( ItemLongNamegetter );
	itemnamecol.SetNumberOfEntries( gIndividualMilitiaFilteredIdsVector.size() );
	itemnamecol.SetCallBackType( ColumnDataProvider::CDP_MILITIA_LIST );

	AddColumnDataProvider( itemnamecol );*/

	TestTable::Init( sX, sY, sX_End, sY_End );
}