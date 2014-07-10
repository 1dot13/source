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
#include "merccompare.h"
#include "random.h"
#include "Interface.h"
#include "Soldier Add.h"
#include "Soldier Profile.h"
#include "DropDown.h"
#include "Overhead.h"
#include "Map Screen Interface.h"
#include "DynamicDialogue.h"	// added by Flugente
#endif


#define		MERCOMP_FONT_COLOR									2
#define		CAMPHIS_FONT_COLOR_RED								FONT_MCOLOR_RED
#define		CAMPHIS_FONT_BIG									FONT14ARIAL
#define		CAMPHIS_FONT_MED									FONT12ARIAL
#define		CAMPHIS_FONT_SMALL									FONT10ARIAL

#define		CAMPHIS_FONT_BTN_COLOR								FONT_MCOLOR_WHITE
#define		CAMPHIS_FONT_BTN_SHADOW_COLOR						2

#define		MERCOMP_FONT_SHADOW									FONT_MCOLOR_WHITE

#define		CAMPAIGN_HISTORY_BULLET_TEXT_OFFSET_X			21

#define		CAMPAIGN_HISTORY_LINK_START_X					LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_LINK_START_Y					LAPTOP_SCREEN_WEB_UL_Y + 5
#define		CAMPAIGN_HISTORY_LINK_TEXT_WIDTH				107
#define		CAMPAIGN_HISTORY_LINK_STEP_Y					14

#define		CAMPAIGN_HISTORY_BACKGROUND_WIDTH				125
#define		CAMPAIGN_HISTORY_BACKGROUND_HEIGHT					100

#define		CAMPAIGN_HISTORY_BIG_TITLE_X								115 + LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_BIG_TITLE_Y								10 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_RED_BAR_X									LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_RED_BAR_Y									LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_TOP_RED_BAR_X							LAPTOP_SCREEN_UL_X + 66
#define		CAMPAIGN_HISTORY_TOP_RED_BAR_Y							109 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPAIGN_HISTORY_TOP_RED_BAR_Y1						31 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_BOTTOM_RED_BAR_Y					345 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_X			77 + LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_Y			392 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_WIDTH	107
#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_OFFSET	148
#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_X_2		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_X + CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_OFFSET
#define		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_X_3		CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_X_2 + CAMPAIGN_HISTORY_BOTTOM_LINK_RED_BAR_OFFSET

#define		CAMPAIGN_HISTORY_SUBTITLE_X								CAMPAIGN_HISTORY_BIG_TITLE_X
#define		CAMPAIGN_HISTORY_SUBTITLE_Y								CAMPAIGN_HISTORY_BIG_TITLE_Y + 20

#define		CAMPAIGN_HISTORY_BULLET_TEXT_1_Y						188 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPAIGN_HISTORY_BULLET_TEXT_2_Y						215 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPAIGN_HISTORY_BULLET_TEXT_3_Y						242 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_BOTTOM_SLOGAN_X						CAMPAIGN_HISTORY_SUBTITLE_X
#define		CAMPAIGN_HISTORY_BOTTOM_SLOGAN_Y						285 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPAIGN_HISTORY_BOTTOM_SLOGAN_WIDTH				370

#define		CAMPAIGN_HISTORY_SMALL_TITLE_X							64 + LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_SMALL_TITLE_Y							5 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPAIGN_HISTORY_SMALL_TITLE_WIDTH					434 - 170
#define		CAMPAIGN_HISTORY_SMALL_TITLE_HEIGHT				40 - 10

#define		MERCCOMPARE_NUM_LINKS									4
#define		MCA_START_CONTENT_Y										(LAPTOP_SCREEN_WEB_UL_Y + MERCCOMPARE_NUM_LINKS * 17)


extern UINT32	guiInsuranceBackGround;
extern UINT32	guiInsuranceSmallTitleImage;
extern UINT32	guiInsuranceBigRedLineImage;
UINT32	guiMercCompareBulletImage;
UINT32	guiMercCompareLogoImage;

//link to the various pages
MOUSE_REGION	gMercCompareLinkRegion[MERCCOMPARE_NUM_LINKS];
void SelectMercCompareRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason );


void GameInitMercCompare( )
{

}

BOOLEAN EnterMercCompareMain( )
{
	InitMercCompareDefaults( );

	RenderMercCompareMain( );

	return(TRUE);
}

void ExitMercCompareMain( )
{
	RemoveMercCompareDefaults( );
}

void HandleMercCompareMain( )
{

}

void RenderMercCompareMain( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;
	HVOBJECT	hPixHandle;

	//Get the bullet
	GetVideoObject( &hPixHandle, guiMercCompareBulletImage );

	DisplayMercCompareDefaults( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_INTRO1] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	for ( int i = TEXT_MERCCOMPARE_BULLET1; i < TEXT_MERCCOMPARE_BULLET4; ++i )
	{
		// display bullet
		BltVideoObject( FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );
				
		swprintf( sText, szMercCompareWebSite[i] );
		DrawTextToScreen( sText, usPosX + 25, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 15;
	}

	usPosY += 15;

	swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_INTRO2] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}


void InitMercCompareDefaults( )
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
	for ( int i = 0; i<MERCCOMPARE_NUM_LINKS; ++i )
	{
		MSYS_DefineRegion( &gMercCompareLinkRegion[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY + CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
			CURSOR_WWW, MSYS_NO_CALLBACK, SelectMercCompareRegionCallBack );
		MSYS_AddRegion( &gMercCompareLinkRegion[i] );
		MSYS_SetRegionUserData( &gMercCompareLinkRegion[i], 0, i );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;
	}
}

void DisplayMercCompareDefaults( )
{
	HVOBJECT	hPixHandle;
	GetVideoObject( &hPixHandle, guiMercCompareLogoImage );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	CHAR16 sText[800];
	UINT16 usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_LINK_START_Y;

	WebPageTileBackground( 4, 4, CAMPAIGN_HISTORY_BACKGROUND_WIDTH, CAMPAIGN_HISTORY_BACKGROUND_HEIGHT, guiInsuranceBackGround );

	//Display the title slogan
	GetMercCompareText( TEXT_MERCCOMPARE_WEBSITENAME, sText );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_BIG_TITLE_X, CAMPAIGN_HISTORY_BIG_TITLE_Y, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	//Display the subtitle slogan
	GetMercCompareText( TEXT_MERCCOMPARE_SLOGAN, sText );
	DrawTextToScreen( sText, CAMPAIGN_HISTORY_SUBTITLE_X, CAMPAIGN_HISTORY_SUBTITLE_Y, 0, CAMPHIS_FONT_BIG, MERCOMP_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX = CAMPAIGN_HISTORY_LINK_START_X;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	for ( int i = 0; i<MERCCOMPARE_NUM_LINKS; ++i )
	{
		GetMercCompareText( TEXT_MERCCOMPARE_SUBSITE1 + i, sText );
		DisplayWrappedString( usPosX, usPosY, CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += CAMPAIGN_HISTORY_LINK_STEP_Y;

		//Display the red bar under the link at the bottom.	and the text
		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_UL_X + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH, usPosY - 2, FROMRGB( 0, 255, 0 ) );
	}

	// closing line that separates header from individual page
	DisplaySmallColouredLineWithShadow( usPosX, usPosY - 2, LAPTOP_SCREEN_LR_X, usPosY - 2, FROMRGB( 0, 255, 0 ) );

	usPosX = LAPTOP_SCREEN_LR_X - 110;
	usPosY = CAMPAIGN_HISTORY_LINK_START_Y;
	BltVideoObject( FRAME_BUFFER, hPixHandle, 1, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

	SetFontShadow( DEFAULT_SHADOW );
}

void RemoveMercCompareDefaults( )
{
	DeleteVideoObjectFromIndex( guiInsuranceBackGround );
	DeleteVideoObjectFromIndex( guiInsuranceBigRedLineImage );
	DeleteVideoObjectFromIndex( guiMercCompareBulletImage );
	DeleteVideoObjectFromIndex( guiMercCompareLogoImage );

	for ( int i = 0; i<MERCCOMPARE_NUM_LINKS; ++i )
		MSYS_RemoveRegion( &gMercCompareLinkRegion[i] );
}

void GetMercCompareText( UINT8 ubNumber, STR16 pString )
{
	if ( ubNumber >= TEXT_MERCCOMPARE_MAX )
		wcscpy( pString, L"???" );

	wcscpy( pString, szMercCompareWebSite[ubNumber] );
}


void SelectMercCompareRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		if ( uiLink == 0 )
			guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_MAIN;
		else if ( uiLink == 1 )
			guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_MATRIX;
		else if ( uiLink == 2 )
			guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_ANALYZE;
		else if ( uiLink == 3 )
			guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_CUSTOMERS;
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}


////////////////////////// MERC COMPARE CUSTOMERS ////////////////////////////////

BOOLEAN EnterMercCompareCustomers( )
{	
	InitMercCompareDefaults( );
	
	RenderMercCompareCustomers( );

	return(TRUE);
}

void ExitMercCompareCustomers( )
{
	RemoveMercCompareDefaults( );
}

void HandleMercCompareCustomers( )
{

}

void RenderMercCompareCustomers( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;
	HVOBJECT	hPixHandle;

	//Get the bullet
	GetVideoObject( &hPixHandle, guiMercCompareBulletImage );

	DisplayMercCompareDefaults( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_QUOTEINTRO] );
	usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	// choose 3 random customer quotes out of the pool of all quotes
	std::set<UINT8> quoteset;
	UINT8 safetycounter = 0;
	while ( quoteset.size( ) < min( 3, TEXT_MERCCOMPARE_CUSTOMERSTATEMENTS ) && safetycounter < 30 )
	{
		quoteset.insert( Random( TEXT_MERCCOMPARE_CUSTOMERSTATEMENTS ) );
		++safetycounter;
	}
	
	std::set<UINT8>::iterator itend = quoteset.end();
	for ( std::set<UINT8>::iterator it = quoteset.begin(); it != itend; ++it )
	{
		// display bullet
		BltVideoObject( FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL );

		swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_QUOTE1 + 2 * (*it)] );
		usPosY += DisplayWrappedString( usPosX + 25, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X - 25, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 10;

		swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_QUOTE1 + 2 * (*it) + 1] );
		usPosY += DisplayWrappedString( usPosX + 180, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X - 25, 2, FONT12ROMAN, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		usPosY += 30;
	}
	
	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// MERC COMPARE CUSTOMERS ////////////////////////////////

////////////////////////// MERC COMPARE ANALYZE ////////////////////////////////
UINT8 gMercCompareProfile1 = NO_PROFILE;
UINT8 gMercCompareProfile2 = NO_PROFILE;
BOOLEAN fMercCompareRedraw = FALSE;

#define MCA_DROPDOWN_X				(LAPTOP_SCREEN_UL_X + 60)
#define MCA_SIDEOFFSET				((LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X) /2)
#define MCA_NUMBEROFFSET			((LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X) *4/9)

// two dropdowns, which we'll use to toggle between our mercs
template<>	void	DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::SetRefresh( )	{ fMercCompareRedraw = TRUE; }
template<>	void	DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::SetRefresh( )	{ fMercCompareRedraw = TRUE; }

BOOLEAN EnterMercCompareAnalyze()
{
	InitMercCompareDefaults( );

	// We fill two dropdowns with all mercs on our team
	std::vector<std::pair<INT16, STR16> > mercvector;

	SOLDIERTYPE* pSoldier = NULL;
	UINT16 id = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16 lastid  = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pSoldier = MercPtrs[id]; id <= lastid; ++id, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE )
			mercvector.push_back( std::make_pair( pSoldier->ubProfile, gMercProfiles[pSoldier->ubProfile].zNickname ) );
	}
	
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).SetEntries( mercvector );
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).SetHelpText( szMercCompareWebSite[TEXT_MERCCOMPARE_DROPDOWNTEXT] );
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).Create( MCA_DROPDOWN_X, MCA_START_CONTENT_Y );

	DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).SetEntries( mercvector );
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).SetHelpText( szMercCompareWebSite[TEXT_MERCCOMPARE_DROPDOWNTEXT] );
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).Create( MCA_DROPDOWN_X + MCA_SIDEOFFSET, MCA_START_CONTENT_Y );

	// if a profile was already set, use it
	if ( DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance().HasEntries() )
	{
		if ( gMercCompareProfile1 != NO_PROFILE )
			DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).SetSelectedEntryKey( gMercCompareProfile1 );

		if ( gMercCompareProfile2 != NO_PROFILE )
			DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).SetSelectedEntryKey( gMercCompareProfile2 );
	}

	RenderMercCompareAnalyze( );
			
	return(TRUE);
}

void ExitMercCompareAnalyze()
{
	RemoveMercCompareDefaults( );

	DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).Destroy( );
	DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).Destroy( );
}

void HandleMercCompareAnalyze()
{
	if ( fMercCompareRedraw )
	{
		RenderMercCompareAnalyze( );
		fMercCompareRedraw = FALSE;
	}
}

void RenderMercCompareAnalyze()
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;
	HVOBJECT	hPixHandle;

	//Get the bullet
	GetVideoObject( &hPixHandle, guiMercCompareBulletImage );

	DisplayMercCompareDefaults( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;
		
	if ( DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance().HasEntries() )
	{
		gMercCompareProfile1 = (UINT8)DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).GetSelectedEntryKey( );
		gMercCompareProfile2 = (UINT8)DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).GetSelectedEntryKey( );
	
		DisplayMercData( gMercCompareProfile1, gMercCompareProfile2 );

		DropDownTemplate<DROPDOWNNR_MERCCOMPARE1>::getInstance( ).Display( );
		DropDownTemplate<DROPDOWNNR_MERCCOMPARE2>::getInstance( ).Display( );
	}
	else
	{
		swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_ERROR_NOBODYTHERE] );
		usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}
	
	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}
////////////////////////// MERC COMPARE ANALYZE ////////////////////////////////


BOOLEAN DisplayMercData( UINT8 usProfileA, UINT8 usProfileB )
{
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT		hPixHandle;
	UINT16			usPosX, usPosY, usPosY2;
	UINT32			uiInsMercFaceImage;
	INT32			iCostOfContract = 0;
	char			sTemp[100];
	CHAR16			sText[800];
	BOOLEAN			fDisplayMercContractStateTextColorInRed = FALSE;
	
	// we need a valid ID here
	if ( !OKToCheckOpinion( usProfileA ) || !OKToCheckOpinion( usProfileB ) )
		return FALSE;

	MERCPROFILESTRUCT*	pProfileA = &(gMercProfiles[usProfileA]);
	MERCPROFILESTRUCT*	pProfileB = &(gMercProfiles[usProfileB]);

	if ( !pProfileA || !pProfileB )
		return FALSE;

	INT16 idA = GetSoldierIDFromMercID( usProfileA );
	INT16 idB = GetSoldierIDFromMercID( usProfileB );

	if ( idA < 0 || idB < 0 )
		return FALSE;

	SOLDIERTYPE* pSoldierA = MercPtrs[idA];
	SOLDIERTYPE* pSoldierB = MercPtrs[idB];

	if ( !pSoldierA || !pSoldierB )
		return FALSE;

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;
		
	// load the mercs face graphic and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	// face 1
	// IMP faces are stored elsewhere
	if ( gProfilesIMP[usProfileA].ProfilId == usProfileA )
	{
		sprintf( sTemp, "IMPFACES\\%02d.sti", gMercProfiles[usProfileA].ubFaceIndex );
		FilenameForBPP( sTemp, VObjectDesc.ImageFile );
		CHECKF( AddVideoObject( &VObjectDesc, &uiInsMercFaceImage ) );
	}
	else
	{
		sprintf( sTemp, "FACES\\%02d.sti", usProfileA );
		FilenameForBPP( sTemp, VObjectDesc.ImageFile );
		CHECKF( AddVideoObject( &VObjectDesc, &uiInsMercFaceImage ) );
	}

	//Get the merc's face
	GetVideoObject( &hPixHandle, uiInsMercFaceImage );

	//if the merc is dead, shade the face red
	if ( IsMercDead( usProfileA ) )
	{
		//if the merc is dead
		//shade the face red, (to signify that he is dead)
		hPixHandle->pShades[0] = Create16BPPPaletteShaded( hPixHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );

		//set the red pallete to the face
		SetObjectHandleShade( uiInsMercFaceImage, 0 );
	}

	//Get and display the mercs face
	BltVideoObject( FRAME_BUFFER, hPixHandle, 0, usPosX + 5, usPosY + 4, VO_BLT_SRCTRANSPARENCY, NULL );

	usPosX += MCA_SIDEOFFSET;

	// face 2
	// IMP faces are stored elsewhere
	if ( gProfilesIMP[usProfileB].ProfilId == usProfileB )
	{
		sprintf( sTemp, "IMPFACES\\%02d.sti", gMercProfiles[usProfileB].ubFaceIndex );
		FilenameForBPP( sTemp, VObjectDesc.ImageFile );
		CHECKF( AddVideoObject( &VObjectDesc, &uiInsMercFaceImage ) );
	}
	else
	{
		sprintf( sTemp, "FACES\\%02d.sti", usProfileB );
		FilenameForBPP( sTemp, VObjectDesc.ImageFile );
		CHECKF( AddVideoObject( &VObjectDesc, &uiInsMercFaceImage ) );
	}

	//Get the merc's face
	GetVideoObject( &hPixHandle, uiInsMercFaceImage );

	//if the merc is dead, shade the face red
	if ( IsMercDead( usProfileB ) )
	{
		//if the merc is dead
		//shade the face red, (to signify that he is dead)
		hPixHandle->pShades[0] = Create16BPPPaletteShaded( hPixHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );

		//set the red pallete to the face
		SetObjectHandleShade( uiInsMercFaceImage, 0 );
	}

	//Get and display the mercs face
	BltVideoObject( FRAME_BUFFER, hPixHandle, 0, usPosX + 5, usPosY + 4, VO_BLT_SRCTRANSPARENCY, NULL );

	// the face images isn't needed anymore so delete it
	DeleteVideoObjectFromIndex( uiInsMercFaceImage );

	usPosX -= MCA_SIDEOFFSET;
	usPosY += 50;

	SetFontShadow( MERCOMP_FONT_SHADOW );

	// base opinions
	swprintf( sText, L"Base opinion:" );
	DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	INT8 val = pProfileA->bMercOpinion[usProfileB];
	swprintf( sText, L"%d", val );
	DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

	val = pProfileB->bMercOpinion[usProfileA];
	swprintf( sText, L"%d", val );
	usPosY += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	// refinement
	BOOLEAN fRefinementfoundA = FALSE;
	// if we don't care, doesn't matter
	if ( pProfileA->bRefinementCareLevel == CARELEVEL_NONE )
	{
		// nothing to do...
	}
	// if we care somewhat, malus on slob/snob
	else if ( pProfileA->bRefinementCareLevel == CARELEVEL_SOME )
	{
		if ( pProfileA->bRefinement * pProfileB->bRefinement == 2 )
		{
			val = -gGameExternalOptions.sMoraleModRefinement;
			fRefinementfoundA = TRUE;
		}
	}
	// if we care extremely, reward for similarity, malus otherwise
	else //if( pProfile->bRefinementCareLevel == CARELEVEL_EXTREME )
	{
		if ( pProfileA->bRefinement * pProfileB->bRefinement == 2 )
			val = -2 * gGameExternalOptions.sMoraleModRefinement;
		else if ( pProfileA->bRefinement * pProfileB->bRefinement == 0 )
			val = -gGameExternalOptions.sMoraleModRefinement;
		else
			val = gGameExternalOptions.sMoraleModRefinement;

		fRefinementfoundA = TRUE;
	}
	
	if ( fRefinementfoundA )
	{
		swprintf( sText, (val < 0) ? L"Dislikes %s %s" : L"Likes %s %s", szRefinementTextTypes[pProfileB->bRefinement], szCareLevelText[pProfileA->bRefinementCareLevel] );
		DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	BOOLEAN fRefinementfoundB = FALSE;
	// if we don't care, doesn't matter
	if ( pProfileB->bRefinementCareLevel == CARELEVEL_NONE )
	{
		// nothing to do...
	}
	// if we care somewhat, malus on slob/snob
	else if ( pProfileB->bRefinementCareLevel == CARELEVEL_SOME )
	{
		if ( pProfileA->bRefinement * pProfileB->bRefinement == 2 )
		{
			val = -gGameExternalOptions.sMoraleModRefinement;
			fRefinementfoundB = TRUE;
		}
	}
	// if we care extremely, reward for similarity, malus otherwise
	else //if( pProfile->bRefinementCareLevel == CARELEVEL_EXTREME )
	{
		if ( pProfileA->bRefinement * pProfileB->bRefinement == 2 )
			val = -2 * gGameExternalOptions.sMoraleModRefinement;
		else if ( pProfileA->bRefinement * pProfileB->bRefinement == 0 )
			val = -gGameExternalOptions.sMoraleModRefinement;
		else
			val = gGameExternalOptions.sMoraleModRefinement;

		fRefinementfoundB = TRUE;
	}

	if ( fRefinementfoundB )
	{
		swprintf( sText, (val < 0) ? L"Dislikes %s %s" : L"Likes %s %s", szRefinementTextTypes[pProfileA->bRefinement], szCareLevelText[pProfileB->bRefinementCareLevel] );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	if ( fRefinementfoundA || fRefinementfoundB )
		usPosY += 12;

	// some people hate other nationalities (do not mix up with racism, which uses bRace)
	if ( (pProfileA->bHatedNationality > -1 && pProfileB->bNationality == pProfileA->bHatedNationality) || (pProfileB->bHatedNationality > -1 && pProfileA->bNationality == pProfileB->bHatedNationality) )
	{
		if ( pProfileA->bHatedNationality > -1 && pProfileB->bNationality == pProfileA->bHatedNationality )
		{
			if ( pProfileA->bHatedNationalityCareLevel == CARELEVEL_SOME )
				val = -gGameExternalOptions.sMoraleModHatedNationality;
			else if ( pProfileA->bHatedNationalityCareLevel == CARELEVEL_EXTREME )
				val = -gGameExternalOptions.sMoraleModHatedNationality * 2;
			else
				val = 0;

			swprintf( sText, pProfileA->bRacist == RACIST_VERY ? L"Strongly hates %s" : L"Hates %s", szNationalityTextAdjective[pProfileA->bHatedNationality] );
			DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}

		if ( pProfileB->bHatedNationality > -1 && pProfileA->bNationality == pProfileB->bHatedNationality )
		{
			if ( pProfileB->bHatedNationalityCareLevel == CARELEVEL_SOME )
				val = -gGameExternalOptions.sMoraleModHatedNationality;
			else if ( pProfileB->bHatedNationalityCareLevel == CARELEVEL_EXTREME )
				val = -gGameExternalOptions.sMoraleModHatedNationality * 2;
			else
				val = 0;

			swprintf( sText, pProfileB->bRacist == RACIST_VERY ? L"Strongly hates %s" : L"Hates %s", szNationalityTextAdjective[pProfileB->bHatedNationality] );
			DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}

		usPosY += 12;
	}

	// racism
	if ( (pProfileA->bRacist || pProfileB->bRacist) && pProfileA->bRace != pProfileB->bRace )
	{
		if ( pProfileA->bRacist )
		{
			if ( pProfileA->bRacist == RACIST_SOME )
				val = -gGameExternalOptions.sMoraleModRacism;
			else if ( pProfileA->bRacist == RACIST_VERY )
				val = -gGameExternalOptions.sMoraleModRacism * 2;
			else
				val = 0;
				
			swprintf( sText, pProfileA->bRacist == RACIST_VERY ? L"Deep racism against %s" : L"Racism against %s", szRaceText[pProfileB->bRace] );
			DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}

		if ( pProfileB->bRacist )
		{
			if ( pProfileB->bRacist == RACIST_SOME )
				val = -gGameExternalOptions.sMoraleModRacism;
			else if ( pProfileB->bRacist == RACIST_VERY )
				val = -gGameExternalOptions.sMoraleModRacism * 2;
			else
				val = 0;

			swprintf( sText, pProfileB->bRacist == RACIST_VERY ? L"Deep racism against %s" : L"Racism against %s", szRaceText[pProfileA->bRace] );
			DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}

		usPosY += 12;
	}

	// appearance
	BOOLEAN fAppearanceDone = FALSE;
	switch ( pProfileB->bAppearance )
	{
	case APPEARANCE_UGLY:		val = -gGameExternalOptions.sMoraleModAppearance * 2;	break;
	case APPEARANCE_HOMELY:		val = -gGameExternalOptions.sMoraleModAppearance;		break;
	case APPEARANCE_ATTRACTIVE:	val = gGameExternalOptions.sMoraleModAppearance;		break;
	case APPEARANCE_BABE:		val = gGameExternalOptions.sMoraleModAppearance * 2;	break;
	default:	val = 0;	break;
	}

	if ( pProfileA->bAppearanceCareLevel == CARELEVEL_EXTREME )		val *= 2;
	else if ( pProfileA->bAppearanceCareLevel == CARELEVEL_NONE )	val = 0;

	if ( val != 0 )
	{
		swprintf( sText, (pProfileA->bAppearanceCareLevel == CARELEVEL_EXTREME) ? L"Cares deeply about looks" : L"Cares about looks" );
		DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		fAppearanceDone = TRUE;
	}

	switch ( pProfileA->bAppearance )
	{
	case APPEARANCE_UGLY:		val = -gGameExternalOptions.sMoraleModAppearance * 2;	break;
	case APPEARANCE_HOMELY:		val = -gGameExternalOptions.sMoraleModAppearance;		break;
	case APPEARANCE_ATTRACTIVE:	val = gGameExternalOptions.sMoraleModAppearance;		break;
	case APPEARANCE_BABE:		val = gGameExternalOptions.sMoraleModAppearance * 2;	break;
	default:	val = 0;	break;
	}

	if ( pProfileB->bAppearanceCareLevel == CARELEVEL_EXTREME )		val *= 2;
	else if ( pProfileB->bAppearanceCareLevel == CARELEVEL_NONE )	val = 0;

	if ( val != 0 )
	{
		swprintf( sText, (pProfileB->bAppearanceCareLevel == CARELEVEL_EXTREME) ? L"Cares deeply about looks" : L"Cares about looks" );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		fAppearanceDone = TRUE;
	}

	if ( fAppearanceDone )
		usPosY += 12;

	// sexism
	BOOLEAN fSexismDone = FALSE;
	if ( (pProfileA->bSexist || pProfileB->bSexist) && pProfileA->bSex != pProfileB->bSex )
	{
		if ( pProfileA->bSexist )
		{
			switch ( pProfileB->bAppearance )
			{
			case APPEARANCE_UGLY:		val = -gGameExternalOptions.sMoraleModSexism * 2;	break;
			case APPEARANCE_HOMELY:		val = -gGameExternalOptions.sMoraleModSexism;		break;
			case APPEARANCE_ATTRACTIVE:	val = gGameExternalOptions.sMoraleModSexism;		break;
			case APPEARANCE_BABE:		val = gGameExternalOptions.sMoraleModSexism * 2;	break;
			default:	val = 0;	break;
			}

			if ( pProfileA->bSexist == VERY_SEXIST )	val *= 2;

			if ( val != 0 )
			{
				swprintf( sText, (pProfileA->bSexist == VERY_SEXIST) ? L"Very sexist" : L"Sexist" );
				DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				swprintf( sText, L"%d", val );
				DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				fSexismDone = TRUE;
			}
		}

		if ( pProfileB->bSexist )
		{
			switch ( pProfileA->bAppearance )
			{
			case APPEARANCE_UGLY:		val = -gGameExternalOptions.sMoraleModSexism * 2;	break;
			case APPEARANCE_HOMELY:		val = -gGameExternalOptions.sMoraleModSexism;		break;
			case APPEARANCE_ATTRACTIVE:	val = gGameExternalOptions.sMoraleModSexism;		break;
			case APPEARANCE_BABE:		val = gGameExternalOptions.sMoraleModSexism * 2;	break;
			default:	val = 0;	break;
			}

			if ( pProfileB->bSexist == VERY_SEXIST )	val *= 2;

			if ( val != 0 )
			{
				swprintf( sText, (pProfileB->bSexist == VERY_SEXIST) ? L"Very sexist" : L"Sexist" );
				DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				swprintf( sText, L"%d", val );
				DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				fSexismDone = TRUE;
			}
		}

		if ( fSexismDone )
			usPosY += 12;
	}

	// backgrounds
	if ( GetBackgroundValue( usProfileA, BG_DISLIKEBG ) && GetBackgroundValue( usProfileA, BG_DISLIKEBG ) == -GetBackgroundValue( usProfileB, BG_DISLIKEBG ) )
	{
		val = -2;

		swprintf( sText, L"Dislikes other background" );
		DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		
		swprintf( sText, L"Dislikes other background" );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		usPosY += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	// after this, the lines are no longer synchronous - we just list each property on its own side
	usPosY2 = usPosY;

	if ( pProfileA->usBackground != pProfileB->usBackground )
	{
		if ( HasBackgroundFlag( usProfileA, BACKGROUND_XENOPHOBIC ) )
		{
			val = -gGameExternalOptions.sMoraleModXenophobicBackGround;
			swprintf( sText, L"Dislikes other backgrounds" );
			DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			usPosY += DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}

		if ( HasBackgroundFlag( usProfileB, BACKGROUND_XENOPHOBIC ) )
		{
			val = -gGameExternalOptions.sMoraleModXenophobicBackGround;
			swprintf( sText, L"Dislikes other backgrounds" );
			DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			swprintf( sText, L"%d", val );
			usPosY2 += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		}
	}

	// dynamic opinions
	if ( gGameExternalOptions.fDynamicOpinions )
	{
		for ( UINT8 opinionevent = OPINIONEVENT_FRIENDLYFIRE; opinionevent < OPINIONEVENT_MAX; ++opinionevent )
		{
			val = GetDynamicOpinion( usProfileA, usProfileB, opinionevent );

			if ( val )
			{
				swprintf( sText, szMercCompareEventText[opinionevent], gMercProfiles[usProfileB].zNickname );
				DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				swprintf( sText, L"%d", val );
				usPosY += DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			}

			val = GetDynamicOpinion( usProfileB, usProfileA, opinionevent );

			if ( val )
			{
				swprintf( sText, szMercCompareEventText[opinionevent], gMercProfiles[usProfileA].zNickname );
				DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				swprintf( sText, L"%d", val );
				usPosY2 += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
			}
		}
	}

	// long-term memory
	val = gMercProfiles[usProfileA].sDynamicOpinionLongTerm[usProfileB];

	if ( val )
	{
		swprintf( sText, L"Past grievances" );
		DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		usPosY += DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	val = gMercProfiles[usProfileB].sDynamicOpinionLongTerm[usProfileA];

	if ( val )
	{
		swprintf( sText, L"Past grievances" );
		DisplayWrappedString( usPosX + MCA_SIDEOFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
		swprintf( sText, L"%d", val );
		usPosY2 += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}

	// draw the final verdict
	val = SoldierRelation( pSoldierA, pSoldierB );

	swprintf( sText, L"____" );
	DisplayWrappedString( usPosX + MCA_NUMBEROFFSET - 10, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	usPosY += 12;
	swprintf( sText, L"%d", val );
	usPosY += DisplayWrappedString( usPosX + MCA_NUMBEROFFSET, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	val = SoldierRelation( pSoldierB, pSoldierA );

	swprintf( sText, L"____" );
	DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET - 10, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	usPosY2 += 12;
	swprintf( sText, L"%d", val );
	usPosY2 += DisplayWrappedString( usPosX + MCA_SIDEOFFSET + MCA_NUMBEROFFSET, usPosY2, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	return TRUE;
}

////////////////////////// MERC COMPARE MATRIX ////////////////////////////////
UINT8 gSquadToShow = SQUAD_1;
BOOLEAN fMercCompareMatrixRedraw = FALSE;

//link to the various pages
MOUSE_REGION	gMercCompareMatrixLinkRegion[100];
BOOLEAN			gMercCompareMatrixLinkDefined[100] = { FALSE };
void SelectMercCompareMatrixRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason );

// two dropdowns, which we'll use to toggle between our mercs
template<>	void	DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::SetRefresh( )	{
	fMercCompareMatrixRedraw = TRUE;
}

BOOLEAN EnterMercCompareMatrix( )
{
	InitMercCompareDefaults( );

	// We fill two dropdowns with all mercs on our team
	std::vector<std::pair<INT16, STR16> > dropdownvector;

	// create a map of all current squads  with at least 2 members
	std::map<UINT8, UINT8>	squadmap;

	SOLDIERTYPE* pSoldier = NULL;
	UINT16 id = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16 lastid = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pSoldier = MercPtrs[id]; id <= lastid; ++id, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->bAssignment < ON_DUTY )
		{
			if ( squadmap.find( pSoldier->bAssignment ) == squadmap.end() )
				squadmap[pSoldier->bAssignment] = 1;
			else
				++squadmap[pSoldier->bAssignment];
		}
	}

	std::map<UINT8, UINT8>::iterator itend = squadmap.end();
	for ( std::map<UINT8, UINT8>::iterator it = squadmap.begin(); it != itend; ++it )
	{
		if ( (*it).second > 1 )
		{
			if ( gGameExternalOptions.fUseXMLSquadNames )
				dropdownvector.push_back( std::make_pair( (*it).first, SquadNames[(*it).first].squadname ) );
			else
				dropdownvector.push_back( std::make_pair( (*it).first, pSquadMenuStrings[(*it).first] ) );
		}
	}

	if ( !dropdownvector.empty() )
	{
		DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).SetEntries( dropdownvector );
		DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).SetHelpText( szMercCompareWebSite[TEXT_MERCCOMPARE_DROPDOWNTEXT_MATRIX] );
		DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).Create( LAPTOP_SCREEN_UL_X, MCA_START_CONTENT_Y );
	}

	RenderMercCompareMatrix( );

	return(TRUE);
}

void ExitMercCompareMatrix( )
{
	RemoveMercCompareDefaults( );

	DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).Destroy( );

	for ( int i = 0; i < 100; ++i )
	{
		if ( gMercCompareMatrixLinkDefined[i] )
		{
			MSYS_RemoveRegion( &gMercCompareMatrixLinkRegion[i] );
			gMercCompareMatrixLinkDefined[i] = FALSE;
		}
	}
}

void HandleMercCompareMatrix( )
{
	if ( fMercCompareMatrixRedraw )
	{
		RenderMercCompareMatrix( );
		fMercCompareMatrixRedraw = FALSE;
	}
}

void RenderMercCompareMatrix( )
{
	CHAR16		sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayMercCompareDefaults( );

	SetFontShadow( MERCOMP_FONT_SHADOW );

	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = MCA_START_CONTENT_Y;

	for ( int i = 0; i < 100; ++i )
	{
		if ( gMercCompareMatrixLinkDefined[i] )
		{
			MSYS_RemoveRegion( &gMercCompareMatrixLinkRegion[i] );
			gMercCompareMatrixLinkDefined[i] = FALSE;
		}
	}

	if ( DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance().HasEntries() )
	{
		gSquadToShow = ( UINT8 )DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).GetSelectedEntryKey( );
		DropDownTemplate<DROPDOWNNR_MERCCOMPARE_SQUADSELECTION>::getInstance( ).Display( );

		SetFontShadow( MERCOMP_FONT_SHADOW );

		usPosX = LAPTOP_SCREEN_UL_X;
		usPosY = MCA_START_CONTENT_Y + 30;

		// display a table with all squadmembers
		std::vector<UINT8> squadvector;
		SOLDIERTYPE* pSoldier = NULL;
		UINT16 id = gTacticalStatus.Team[gbPlayerNum].bFirstID;
		UINT16 lastid = gTacticalStatus.Team[gbPlayerNum].bLastID;
		for ( pSoldier = MercPtrs[id]; id <= lastid; ++id, pSoldier++ )
		{
			if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->bAssignment == gSquadToShow )
			{
				// remember squamember
				squadvector.push_back( pSoldier->ubProfile );
			}
		}

		UINT16 spacepermerc = (LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X) / (squadvector.size() + 1);
		spacepermerc = min( spacepermerc , 60);

		// now loop over the squadmembers and fill out the table
		std::vector<UINT8>::iterator itend = squadvector.end( );
		for ( std::vector<UINT8>::iterator it = squadvector.begin(); it != itend; ++it )
		{
			usPosX += spacepermerc;

			// write the names of all squamembers on top of the table
			swprintf( sText, gMercProfiles[(*it)].zNickname );
			DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

			DisplaySmallColouredLineWithShadow( usPosX - 10, usPosY - 6, usPosX - 10, usPosY + 17 * (squadvector.size( ) + 1) - 5, FROMRGB( 0, 255, 0 ) );
		}

		DisplaySmallColouredLineWithShadow( usPosX - 10, usPosY - 6, usPosX - 10, usPosY + 17 * (squadvector.size( ) + 1) - 5, FROMRGB( 0, 255, 0 ) );

		usPosX = LAPTOP_SCREEN_UL_X;

		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 5, usPosX + spacepermerc * (squadvector.size( ) + 1), usPosY - 5, FROMRGB( 0, 255, 0 ) );
	
		UINT16 currentmouseregion = 0;

		for ( std::vector<UINT8>::iterator it = squadvector.begin(); it != itend; ++it )
		{
			INT16 idA = GetSoldierIDFromMercID( (*it) );

			if ( idA < 0 )
				continue;

			SOLDIERTYPE* pSoldierA = MercPtrs[idA];

			if ( !pSoldierA )
				continue;

			usPosY += 17;
			usPosX = LAPTOP_SCREEN_UL_X;

			DisplaySmallColouredLineWithShadow( usPosX, usPosY - 5, usPosX + spacepermerc * (squadvector.size( ) + 1), usPosY - 5, FROMRGB( 0, 255, 0 ) );
		
			// write name on the left side
			swprintf( sText, gMercProfiles[pSoldierA->ubProfile].zNickname );
			DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosX += spacepermerc;

			for ( std::vector<UINT8>::iterator it2 = squadvector.begin( ); it2 != itend; ++it2 )
			{
				INT16 idB = GetSoldierIDFromMercID( (*it2) );

				if ( idB < 0 )
					continue;

				if ( idA == idB )
				{
					usPosX += spacepermerc;
					continue;
				}

				SOLDIERTYPE* pSoldierB = MercPtrs[idB];

				if ( !pSoldierB )
					continue;

				if ( !gMercCompareMatrixLinkDefined[currentmouseregion] )
				{
					MSYS_DefineRegion( &gMercCompareMatrixLinkRegion[currentmouseregion], usPosX - 5, usPosY, usPosX + spacepermerc - 5, usPosY + 17, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectMercCompareMatrixRegionCallBack );
					MSYS_AddRegion( &gMercCompareMatrixLinkRegion[currentmouseregion] );

					// both profilenumbers are combined to a single value which can later be reinterpreted.
					// Note: this will fail if the profile IDs are bigger than UINT16 (currently UINT8)
					MSYS_SetRegionUserData( &gMercCompareMatrixLinkRegion[currentmouseregion], 0, ((pSoldierA->ubProfile << 8) | pSoldierB->ubProfile) );
					gMercCompareMatrixLinkDefined[currentmouseregion] = TRUE;
					++currentmouseregion;
				}
						
				// write down both relations						
				INT8 val = SoldierRelation( pSoldierA, pSoldierB );
				swprintf( sText, L"%d", val );
				DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				usPosX += 15;

				swprintf( sText, L"/" );
				DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
				usPosX += 5;

				val = SoldierRelation( pSoldierB, pSoldierA );
				swprintf( sText, L"%d", val );
				DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, (val > 0) ? FONT_MCOLOR_LTGREEN : (val < 0) ? FONT_MCOLOR_LTRED : MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );

				usPosX += spacepermerc - 20;
			}
		}

		usPosY += 17;
		usPosX = LAPTOP_SCREEN_UL_X;

		DisplaySmallColouredLineWithShadow( usPosX, usPosY - 5, usPosX + spacepermerc * (squadvector.size( ) + 1), usPosY - 5, FROMRGB( 0, 255, 0 ) );
	}
	else
	{
		swprintf( sText, szMercCompareWebSite[TEXT_MERCCOMPARE_ERROR_NOBODYTHERE] );
		usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, 2, CAMPHIS_FONT_MED, MERCOMP_FONT_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, 0 );
	}
	
	SetFontShadow( DEFAULT_SHADOW );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion( LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y );
}

void SelectMercCompareMatrixRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( iReason & MSYS_CALLBACK_REASON_INIT )
	{
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		INT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		gMercCompareProfile1 = uiLink >> 8;
		gMercCompareProfile2 = uiLink & ~0xFFFFFF00;

		guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_ANALYZE;
	}
	else if ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
	}
}
////////////////////////// MERC COMPARE MATRIX ////////////////////////////////