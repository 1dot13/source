/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "Insurance Text.h"
	#include "insurance.h"
	#include "insurance Comments.h"
	#include "WCheck.h"
	#include "Utilities.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "Text.h"
	#include "CampaignHistory_Summary.h"
	#include "CampaignHistoryMain.h"
	#include "CampaignStats.h"
	#include "Campaign Types.h"
	#include "Game Clock.h"
	#include "random.h"
	#include "strategicmap.h"
#endif

#define		CAMPHIS_SUM_TITLE_Y								52 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPHIS_SUM_FIRST_BULLET_X					82 + LAPTOP_SCREEN_UL_X
#define		CAMPHIS_SUM_FIRST_BULLET_Y					75 + LAPTOP_SCREEN_WEB_UL_Y

#define		CAMPHIS_SUM_REDLINE_WIDTH					384

#define		CAMPHIS_SUM_COMMENT_OFFSET_Y				20

#define		CAMPHIS_SUM_NEXT_COMMENT_OFFSET_Y	65

#define		CAMPHIS_SUM_COMMENT_TEXT_WIDTH			364

#define		CAMPHIS_SUM_LINK_Y									357 + LAPTOP_SCREEN_WEB_UL_Y
#define		CAMPHIS_SUM_LINK_WIDTH							90
#define		CAMPHIS_SUM_LINK_HEIGHT						35
#define		CAMPHIS_SUM_LINK_OFFSET_X					166

#define		CAMPAIGN_HISTORY_PAGEBTN_X					LAPTOP_SCREEN_UL_X + 150
#define		CAMPAIGN_HISTORY_PAGEBTN_Y					LAPTOP_SCREEN_LR_Y - 4
#define		CAMPAIGN_HISTORY_PAGEBTN_STEP_X				50

#define		CAMPAIGN_HISTORY_TABLE_STEP_X				60

#define		CAMPAIGN_HISTORY_FONT_COLOR_REGULAR			2
#define		CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE			FONT_MCOLOR_DKGRAY

#define		CAMPAIGN_HISTORY_DETAIL_STEP_Y				15

extern UINT32	guiInsCmntBulletImage;

extern BOOLEAN DisplayComment( UINT8 ubCommentorsName, UINT8 ubComment, UINT16 usPosY );

////////////////////////// SUMMARY PAGE //////////////////////////////////
MOUSE_REGION	gCampaignHistorySummaryRegion[1];
void SelectCampaignHistorySummaryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

UINT8	gusSummaryMode = 0;	// 0: Kills etc., 1: Consumption etc.

void GameInitCampaignHistorySummary()
{

}

BOOLEAN EnterCampaignHistorySummary()
{
	InitCampaignHistoryDefaults();

	// previous/next buttons
	UINT16 usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	for(int i=0; i<1; ++i)
	{
		MSYS_DefineRegion( &gCampaignHistorySummaryRegion[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY+CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectCampaignHistorySummaryRegionCallBack);
		MSYS_AddRegion(&gCampaignHistorySummaryRegion[i]);
		MSYS_SetRegionUserData( &gCampaignHistorySummaryRegion[i], 0, i );

		usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	}
	
	RenderCampaignHistorySummary();

	return(TRUE);
}

void ExitCampaignHistorySummary()
{
	// previous/next buttons
	for(int i=0; i<1; ++i)
		MSYS_RemoveRegion( &gCampaignHistorySummaryRegion[i]);

	RemoveCampaignHistoryDefaults();
}

void HandleCampaignHistorySummary()
{

}

void RenderCampaignHistorySummary()
{
//	HVOBJECT hPixHandle;
	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayCampaignHistoryDefaults();

	SetFontShadow( CAMPHIS_FONT_SHADOW );
	
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 80;

	if ( gusSummaryMode == 0 )
	{
		swprintf(sText, L"");
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_KILLED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_WOUNDED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_PRISONERS]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_SHOTSFIRED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX = LAPTOP_SCREEN_UL_X;
		// closing line that separates header from individual page
		DisplaySmallRedLineWithShadow( usPosX, usPosY+15, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, usPosY+15);

		usPosY += 20;

		for(int i = CAMPAIGNHISTORY_SD_MERC; i < CAMPAIGNHISTORY_SD_MAX; ++i)
		{
			// if faction was not involved at all, ignore
			if ( !gCampaignStats.usKills[i] && !gCampaignStats.usWounds[i] && !gCampaignStats.usPrisoners[i] && !gCampaignStats.usShots[i] )
				continue;

			usPosX = LAPTOP_SCREEN_UL_X;
			swprintf(sText, L"%s", szSoldierClassName[i]);
			DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", gCampaignStats.usKills[i]);
			if ( gCampaignStats.usKills[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", gCampaignStats.usWounds[i]);
			if ( gCampaignStats.usWounds[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", gCampaignStats.usPrisoners[i]);
			if ( gCampaignStats.usPrisoners[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", gCampaignStats.usShots[i]);
			if ( gCampaignStats.usShots[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosY += 20;
		}
	}
	else if ( gusSummaryMode == 1 )
	{
		usPosX = LAPTOP_SCREEN_UL_X;
		usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_MONEYEARNED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 20;

		for(int i = 0; i < CAMPAIGN_MONEY_MAX; ++i)
		{
			usPosX = LAPTOP_SCREEN_UL_X;
			swprintf(sText, L"%s", szCampaignHistoryMoneyTypeString[i]);
			if ( gCampaignStats.sMoneyEarned[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d $", gCampaignStats.sMoneyEarned[i]);
			if ( gCampaignStats.sMoneyEarned[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosY += 20;
		}

		usPosX = LAPTOP_SCREEN_UL_X;
		usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_CONSUMPTION]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosY += 20;

		for(int i = 0; i < CAMPAIGN_CONSUMED_MAX; ++i)
		{
			usPosX = LAPTOP_SCREEN_UL_X;
			swprintf(sText, L"%s", szCampaignHistoryConsumptionTypeString[i]);
			if ( gCampaignStats.usConsumed[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%3.3f kg", gCampaignStats.usConsumed[i] / 10.0 );
			if ( gCampaignStats.usConsumed[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosY += 20;
		}
	}

	// previous/next buttons
	usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	if ( gusSummaryMode )
		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_LOSSES]);
	else
		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_CONSUMPTION]);
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	SetFontShadow( DEFAULT_SHADOW );
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);

	SetFontShadow( DEFAULT_SHADOW );
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectCampaignHistorySummaryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );
				
		//if( uiLink == 0 )
		{
			++gusSummaryMode;

			if ( gusSummaryMode >= 2)
				gusSummaryMode = 0;
		}

		RenderCampaignHistorySummary();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}
////////////////////////// SUMMARY PAGE //////////////////////////////////

////////////////////////// MOST IMPORTANT PAGE //////////////////////////////////
//link to the various pages
MOUSE_REGION	gCampaignHistoryMostImportantLinkRegion[3];
void SelectCampaignHistoryMostImportantRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

UINT8	gusMostImportantPage = 0;
UINT8	gusMostImportantMode = 0;	// 0: Summary, 1: Detail

// special pics for one-time-events
UINT32	guiCampaignEventImage;

// in order to display a fitting picture, we need a lot of libraries.
// There are several folders according to the terrain. Each has subfolders for
// - player wins battle
// - player loses battle
// - tank battle
// - military installation (SAM site, barracks etc.)
// - airdrop
// The image number displayed is then determined as 'id of incident % number of pics in library'. That way each incident keeps its picture, while pictures don't repeat that often
// Each library needs at least one picture


UINT32	guiCampaignHistoryPicture[CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MAX][CAMPAINGHISTORY_PICLIBRARY_TYPE_MAX];


void GameInitCampaignHistory_MostImportant()
{
	gusMostImportantPage = 0;
}

BOOLEAN EnterCampaignHistory_MostImportant()
{
	InitCampaignHistoryDefaults();

	// previous/next buttons
	UINT16 usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	for(int i=0; i<3; ++i)
	{
		MSYS_DefineRegion( &gCampaignHistoryMostImportantLinkRegion[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY+CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectCampaignHistoryMostImportantRegionCallBack);
		MSYS_AddRegion(&gCampaignHistoryMostImportantLinkRegion[i]);
		MSYS_SetRegionUserData( &gCampaignHistoryMostImportantLinkRegion[i], 0, i );

		usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	}

	VOBJECT_DESC	VObjectDesc;

	// picture libraries. Do NOT translate this!
	STR terrainfolders[] =
	{
		"Coastal",
		"Desert",
		"Forest",
		"Mountain",
		"Polar",
		"River",
		"Sparse",
		"Swamp",
		"Tropical",
		"Underground",
		"Urban",
	};

	STR typenames[] =
	{
		"win",
		"loss",
		"installation",
		"airdrop",
		"tanks",
	};

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\CampaignStats\\events.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCampaignEventImage));

	UINT8 i, j;
	for(i = 0; i < CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MAX; ++i)
	{
		for(j = 0; j < CAMPAINGHISTORY_PICLIBRARY_TYPE_MAX; ++j)
		{
			CHAR8 sString[100];
			sprintf(sString, "LAPTOP\\CampaignStats\\%s\\%s.sti", terrainfolders[i], typenames[j]);
			
			VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
			FilenameForBPP((sString), VObjectDesc.ImageFile);

			// if a picture library dos not exist, use the default library instead
			if ( !AddVideoObject(&VObjectDesc, &guiCampaignHistoryPicture[i][j]) )
			{
				guiCampaignHistoryPicture[i][j] = guiCampaignEventImage;
			}
		}
	}

	
	RenderCampaignHistorySummary();

	return(TRUE);
}

void ExitCampaignHistory_MostImportant()
{
	RemoveCampaignHistoryDefaults();

	// previous/next buttons
	for(int i=0; i<3; ++i)
		MSYS_RemoveRegion( &gCampaignHistoryMostImportantLinkRegion[i]);

	DeleteVideoObjectFromIndex( guiCampaignEventImage );

	for(int i = 0; i < CAMPAINGHISTORY_PICLIBRARY_TERRAIN_MAX; ++i)
	{
		for(int j = 0; j < CAMPAINGHISTORY_PICLIBRARY_TYPE_MAX; ++j)
		{
			DeleteVideoObjectFromIndex( guiCampaignHistoryPicture[i][j] );
		}
	}
}

void HandleCampaignHistory_MostImportant()
{

}

void RenderCampaignHistory_MostImportant()
{
	HVOBJECT hPixHandle;
	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayCampaignHistoryDefaults();

	SetFontShadow( CAMPHIS_FONT_SHADOW );
	
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 80;

	// return if there are no incidents yet
	if ( !gCampaignStats.usNumIncidents )
		return;

	Incident_Stats incident = gCampaignStats.mIncidentVector[ gusMostImportantPage ];

	CHAR16 wSectorName_Target[ 100 ];
	GetSectorIDString( SECTORX(incident.usSector), SECTORY(incident.usSector), incident.usLevel, wSectorName_Target, TRUE );

	INT8 bTownId = GetTownIdForSector( SECTORX(incident.usSector), SECTORY(incident.usSector) );

	//Calculate the day, hour, and minutes.
	UINT32 day		= ( incident.usTime / NUM_SEC_IN_DAY );
	UINT32 hour		= ( incident.usTime - ( day * NUM_SEC_IN_DAY ) ) / NUM_SEC_IN_HOUR;
	UINT32 minute	= ( incident.usTime - ( ( day * NUM_SEC_IN_DAY ) + ( hour * NUM_SEC_IN_HOUR ) ) ) / NUM_SEC_IN_MIN;

	UINT8 importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_MOMENTOUS;
	if ( incident.usInterestRating < 500  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_IRRELEVANT;
	else if ( incident.usInterestRating < 1000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_INSIGNIFICANT;
	else if ( incident.usInterestRating < 2000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_NOTABLE;
	else if ( incident.usInterestRating < 3000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_NOTEWORTHY;
	else if ( incident.usInterestRating < 4000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_SIGNIFICANT;
	else if ( incident.usInterestRating < 5000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_INTERESTING;
	else if ( incident.usInterestRating < 6000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_IMPORTANT;
	else if ( incident.usInterestRating < 7000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_VERYIMPORTANT;
	else if ( incident.usInterestRating < 8000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_GRAVE;
	else if ( incident.usInterestRating < 10000  )
		importancenr = TEXT_CAMPAIGNHISTORY_IMPORTANCE_MAJOR;

	swprintf(sText, L"%s %s #%d - %s, %s %d, %02d:%02d", szCampaignHistoryImportanceString[importancenr], szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_INCIDENT], incident.usID, wSectorName_Target, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_DAY], day, hour, minute);
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, CAMPHIS_FONT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 110;

	if ( gusMostImportantMode == 0 )
	{
		usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_KILLED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_WOUNDED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_PRISONERS]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_SHOTSFIRED]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_PARTICIPANTS]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

		usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_PROMOTIONS]);
		DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
				
		usPosX = LAPTOP_SCREEN_UL_X;
		// closing line that separates header from individual page
		DisplaySmallRedLineWithShadow( usPosX, usPosY+15, LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X, usPosY+15);

		usPosY += 20;

		for(int i = CAMPAIGNHISTORY_SD_MERC; i < CAMPAIGNHISTORY_SD_MAX; ++i)
		{
			// if faction was not involved at all, ignore
			if ( !incident.usKills[i] && !incident.usWounds[i] && !incident.usPrisoners[i] && !incident.usShots[i] && !incident.usParticipants[i] && !incident.usPromotions[i] )
				continue;

			usPosX = LAPTOP_SCREEN_UL_X;
			swprintf(sText, L"%s", szSoldierClassName[i]);
			DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += 2 * CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usKills[i]);
			if ( incident.usKills[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usWounds[i]);
			if ( incident.usWounds[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usPrisoners[i]);
			if ( incident.usPrisoners[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );

			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usShots[i]);
			if ( incident.usShots[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usParticipants[i]);
			if ( incident.usParticipants[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosX += CAMPAIGN_HISTORY_TABLE_STEP_X;

			swprintf(sText, L"%d", incident.usPromotions[i]);
			if ( incident.usPromotions[i] )
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
			else
				DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_PASSIVE, FONT_MCOLOR_BLACK, FALSE, 0 );
		
			usPosY += 20;
		}
	}
	else
	{
		UINT8 terrain, type;
		incident.GetTerrainandType(terrain, type);

		// draw text
		// if this is a one-time event, display a special text
		if ( incident.usOneTimeEventFlags & 0xFFFFFFFF )
		{
			UINT8 eventnr = 0;
			if ( incident.usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_OMERTA )
				eventnr = 0;
			else if ( incident.usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_DEATH_KINGPIN )
				eventnr = 1;
			else if ( incident.usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_MASSACRE_HICKS )
				eventnr = 2;
			else if ( incident.usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_MASSACRE_BLOODCATS )
				eventnr = 3;
			else if ( incident.usOneTimeEventFlags & INCIDENT_ONETIMEEVENT_CITY_RETAKEN )
				eventnr = 4;
			else
			{
				for(int i = 0; i < NUM_CAMPAIGNSTATSEVENTS; ++i)
				{
					if ( bTownId == zCampaignStatsEvent[i].usCityTaken )
					{
						eventnr = i;
						break;
					}
				}
			}

			// special picture - use eventnr
			GetVideoObject(&hPixHandle, guiCampaignEventImage );

			BltVideoObject(FRAME_BUFFER, hPixHandle, eventnr, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY, NULL);

			UINT16 picend = usPosY + hPixHandle->pETRLEObject[ eventnr ].usHeight + 3;
			usPosX += hPixHandle->pETRLEObject[ eventnr ].usWidth + 3;

			for (int i = 0; i < MAX_CAMPAIGNSTATSEVENTS_TEXTS; ++i)
			{
				swprintf(sText, zCampaignStatsEvent[eventnr].szText[i] );
				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}
		}
		else
		{
			GetVideoObject(&hPixHandle, guiCampaignHistoryPicture[terrain][type] );

			UINT16 picnum = incident.usID % hPixHandle->usNumberOfObjects;

			BltVideoObject(FRAME_BUFFER, hPixHandle, picnum, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

			UINT16 picend = usPosY + hPixHandle->pETRLEObject[ picnum ].usHeight + 3;
			usPosX += hPixHandle->pETRLEObject[ picnum ].usWidth + 3;

			// time string
			CHAR16	timestring[ 50 ];
			if ( hour < 3 || hour > 23 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_DEEPNIGHT]);
			else if ( hour < 6 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_DAWN]);
			else if ( hour < 8 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_EARLYMORNING]);
			else if ( hour < 11 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_MORNING]);
			else if ( hour < 14 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_NOON]);
			else if ( hour < 18 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_AFTERNOON]);
			else if ( hour < 21 )
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_EVENING]);
			else
				swprintf(timestring, szCampaignHistoryTimeString[TEXT_CAMPAIGNHISTORY_TIME_NIGHT]);

			CHAR16	aggressor[ 50 ];
			CHAR16	defender[ 50 ];
			if ( incident.usIncidentFlags & INCIDENT_ATTACK_ENEMY )
			{
				swprintf(aggressor, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ARMY]);
				swprintf(defender, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REBELFORCES]);
			}
			else
			{
				swprintf(aggressor, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REBELFORCES]);
				swprintf(defender, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ARMY]);
			}

			CHAR16	rebel[ 50 ];
			CHAR16	enemy[ 50 ];
			swprintf(enemy, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ARMY]);
			swprintf(rebel, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REBELFORCES]);

			CHAR16	attacktype[ 50 ];
			if ( incident.usIncidentFlags & INCIDENT_AIRDROP )
				swprintf(attacktype, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_AIRDROPPED]);
			else if ( incident.usIncidentFlags & INCIDENT_AMBUSH )
				swprintf(attacktype, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_AMBUSHED]);
			else
				swprintf(attacktype, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKED]);

			swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SETTING], timestring, aggressor, attacktype, defender, wSectorName_Target);
		
			usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
			if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;

			if ( incident.usIncidentFlags & (INCIDENT_PLAYER_ALLDIRS|INCIDENT_ENEMY_ALLDIRS) )
			{
				if ( incident.usIncidentFlags & INCIDENT_PLAYER_ALLDIRS && incident.usIncidentFlags & INCIDENT_ENEMY_ALLDIRS )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKERANDDEFENDERDIR], incident.GetAttackerDirString(TRUE), defender, incident.GetAttackerDirString(FALSE) );
				else if ( ( incident.usIncidentFlags & INCIDENT_PLAYER_ALLDIRS && incident.usIncidentFlags & INCIDENT_ATTACK_PLAYERSIDE ) || ( incident.usIncidentFlags & INCIDENT_ENEMY_ALLDIRS && incident.usIncidentFlags & INCIDENT_ATTACK_ENEMY ) )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKERDIR], incident.GetAttackerDirString(TRUE) );
				else
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_DEFENDERDIR], defender, incident.GetAttackerDirString(FALSE) );
			
				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			if ( incident.usIncidentFlags & INCIDENT_BUILDINGS_DAMAGED )
			{
				swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_BUILDINGDAMAGE], incident.usKills[CAMPAIGNHISTORY_SD_CIV], incident.usWounds[CAMPAIGNHISTORY_SD_CIV] );
				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			if ( incident.usIncidentFlags & (INCIDENT_REINFORCEMENTS_ENEMY|INCIDENT_REINFORCEMENTS_PLAYERSIDE) )
			{
				if ( incident.usIncidentFlags & INCIDENT_REINFORCEMENTS_ENEMY && incident.usIncidentFlags & INCIDENT_REINFORCEMENTS_PLAYERSIDE )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REINFORCE_BOTH], aggressor, defender );
				else if ( incident.usIncidentFlags & INCIDENT_REINFORCEMENTS_ENEMY )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REINFORCE], enemy );
				else
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_REINFORCE], rebel );

				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			if ( incident.usIncidentFlags & (INCIDENT_MUSTARDGAS_ENEMY|INCIDENT_MUSTARDGAS_PLAYERSIDE) )
			{
				if ( incident.usIncidentFlags & INCIDENT_MUSTARDGAS_ENEMY && incident.usIncidentFlags & INCIDENT_MUSTARDGAS_PLAYERSIDE )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_CHEMICAL_BOTH] );
				else if ( incident.usIncidentFlags & INCIDENT_MUSTARDGAS_ENEMY )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_CHEMICAL], enemy );
				else
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_CHEMICAL], rebel );

				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			if ( incident.usIncidentFlags & (INCIDENT_TANKS_ENEMY|INCIDENT_TANKS_PLAYERSIDE) )
			{
				if ( incident.usIncidentFlags & INCIDENT_TANKS_ENEMY && incident.usIncidentFlags & INCIDENT_TANKS_PLAYERSIDE )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_TANKS_BOTH] );
				else if ( incident.usIncidentFlags & INCIDENT_TANKS_ENEMY )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_TANKS], incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_TANK], enemy, incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_TANK] );
				else
					// TODO
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_TANKS], incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_TANK], rebel, incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_TANK] );

				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			if ( incident.usIncidentFlags & (INCIDENT_SNIPERS_ENEMY|INCIDENT_SNIPERS_PLAYERSIDE) )
			{
				if ( incident.usIncidentFlags & INCIDENT_SNIPERS_ENEMY && incident.usIncidentFlags & INCIDENT_SNIPERS_PLAYERSIDE )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SNIPERS_BOTH] );
				else if ( incident.usIncidentFlags & INCIDENT_SNIPERS_ENEMY )
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SNIPERS], enemy );
				else
					swprintf(sText, szCampaignHistoryDetail[TEXT_CAMPAIGNHISTORY_DETAIL_SNIPERS], rebel );

				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}

			{
				// team losses
				UINT16 rebellosses = incident.usKills[CAMPAIGNHISTORY_SD_MERC] + incident.usKills[CAMPAIGNHISTORY_SD_MILITIA_GREEN] + incident.usKills[CAMPAIGNHISTORY_SD_MILITIA_REGULAR] + incident.usKills[CAMPAIGNHISTORY_SD_MILITIA_ELITE];
				UINT16 armylosses  = incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_ADMIN] + incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_ARMY] + incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_ELITE] + incident.usKills[CAMPAIGNHISTORY_SD_ENEMY_TANK];
				UINT16 armypows    = incident.usPrisoners[CAMPAIGNHISTORY_SD_ENEMY_ADMIN] + incident.usPrisoners[CAMPAIGNHISTORY_SD_ENEMY_ARMY] + incident.usPrisoners[CAMPAIGNHISTORY_SD_ENEMY_ELITE] + incident.usPrisoners[CAMPAIGNHISTORY_SD_ENEMY_TANK];

				// team sizes
				UINT16 rebelsize   = max(1, incident.usParticipants[CAMPAIGNHISTORY_SD_MERC] + incident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_GREEN] + incident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_REGULAR] + incident.usParticipants[CAMPAIGNHISTORY_SD_MILITIA_ELITE]);
				UINT16 armysize    = max(1, incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_ADMIN] + incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_ARMY] + incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_ELITE] + incident.usParticipants[CAMPAIGNHISTORY_SD_ENEMY_TANK]);
			
				// if rebels won...
				if ( incident.usIncidentFlags & INCIDENT_WIN )
				{
					FLOAT ratio = (FLOAT)(rebellosses / rebelsize);
					if ( ratio < 0.1 )
						swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_REBEL] );
					else if ( ratio < 0.3 )
					{
						if ( armypows )
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_EASY_REBEL_PRISONER] );
						else
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_EASY_REBEL] );
					}
					else if ( ratio < 0.6 )
					{
						if ( armypows )
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_REBEL] );
						else
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_REBEL_PRISONER] );
					}
					else
						swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_HARD_REBEL] );
				}
				else	// army won...
				{
					FLOAT ratio = (FLOAT)(armylosses / armysize);
					BOOLEAN armyhadmore = (armysize > rebelsize);

					if ( ratio < 0.1 )
					{
						if ( armyhadmore )
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_ARMY_NUMBERS] );
						else
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_ARMY_TRAINING] );
					}
					else if ( ratio < 0.3 )
					{
						if ( armyhadmore )
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_EASY_ARMY_NUMBERS] );
						else
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_EASY_ARMY_TRAINING] );
					}
					else if ( ratio < 0.6 )
					{
						if ( armyhadmore )
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_ARMY_NUMBERS] );
						else
							swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_ARMY_TRAINING] );
					}
					else
						swprintf(sText, szCampaignHistoryResultString[TEXT_CAMPAIGNHISTORY_RESULT_HARD_ARMY] );
				}

				usPosY += DisplayWrappedString( usPosX, usPosY, LAPTOP_SCREEN_LR_X - usPosX, 2, CAMPHIS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR,sText, FONT_MCOLOR_BLACK, FALSE, 0);
				if ( picend < usPosY )	usPosX = LAPTOP_SCREEN_UL_X;
			}
		}
	}

	// previous/next buttons
	usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	if ( gusMostImportantMode )
		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_SUMMARY]);
	else
		swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_DETAIL]);
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_PREVIOUS]);
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	swprintf(sText, szCampaignHistoryWebpageString[WEBPAGE_CAMPAIGNHISTORY_NEXT]);
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );
	
	SetFontShadow( DEFAULT_SHADOW );
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectCampaignHistoryMostImportantRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		// only display existing incidents
		UINT8 numpages = gCampaignStats.usNumIncidents;
		/*for(int i = 0; i < CAMPAIGNSTATS_NUM_MOST_IMPORTANT; ++i)
		{
			if ( gCampaignStats.importantIncidents[i].usID )
				++numpages;
		}*/

		if( uiLink == 0 )
		{
			++gusMostImportantMode;

			if ( gusMostImportantMode >= 2)
				gusMostImportantMode = 0;
		}
		else if( uiLink == 1 )
		{
			if ( gusMostImportantPage == 0 )
				gusMostImportantPage = numpages;

			--gusMostImportantPage;
		}
		else if( uiLink == 2 )
		{
			++gusMostImportantPage;

			if ( gusMostImportantPage >= numpages )
				gusMostImportantPage = 0;
		}

		RenderCampaignHistory_MostImportant();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}
////////////////////////// MOST IMPORTANT PAGE //////////////////////////////////

////////////////////////// NEWS PAGE //////////////////////////////////
//link to the various pages
MOUSE_REGION	gCampaignHistoryNewsLinkRegion[4];
void SelectCampaignHistoryNewsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

UINT8	gusNewsPage = 0;

void GameInitCampaignHistory_News()
{
	gusNewsPage = 0;
}

BOOLEAN EnterCampaignHistory_News()
{
	InitCampaignHistoryDefaults();

	// previous/next buttons
	UINT16 usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	UINT16 usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	for(int i=0; i<4; ++i)
	{
		MSYS_DefineRegion( &gCampaignHistoryNewsLinkRegion[i], usPosX, usPosY, (UINT16)(usPosX + CAMPAIGN_HISTORY_LINK_TEXT_WIDTH), usPosY+CAMPAIGN_HISTORY_LINK_STEP_Y, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectCampaignHistoryNewsRegionCallBack);
		MSYS_AddRegion(&gCampaignHistoryNewsLinkRegion[i]);
		MSYS_SetRegionUserData( &gCampaignHistoryNewsLinkRegion[i], 0, i );

		usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	}
	
	RenderCampaignHistorySummary();

	return(TRUE);
}

void ExitCampaignHistory_News()
{
	RemoveCampaignHistoryDefaults();

	// previous/next buttons
	for(int i=0; i<4; ++i)
		MSYS_RemoveRegion( &gCampaignHistoryNewsLinkRegion[i]);
}

void HandleCampaignHistory_News()
{

}

void RenderCampaignHistory_News()
{
//	HVOBJECT hPixHandle;
	CHAR16	sText[800];
	swprintf( sText, L"" );
	UINT16	usPosX, usPosY;

	DisplayCampaignHistoryDefaults();

	SetFontShadow( CAMPHIS_FONT_SHADOW );
	
	usPosX = LAPTOP_SCREEN_UL_X;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 80;

	// return if there are no incidents yet
	/*if ( !gCampaignStats.usNumIncidents )
		return;

	Incident_Stats incident = gCampaignStats.mIncidentVector[ gusNewsPage ];

	CHAR16 wSectorName_Target[ 100 ];
	GetSectorIDString( SECTORX(incident.usSector), SECTORY(incident.usSector), incident.usLevel, wSectorName_Target, TRUE );

	//Calculate the day, hour, and minutes.
	UINT32 day		= ( incident.usTime / NUM_SEC_IN_DAY );
	UINT32 hour		= ( incident.usTime - ( day * NUM_SEC_IN_DAY ) ) / NUM_SEC_IN_HOUR;
	UINT32 minute	= ( incident.usTime - ( ( day * NUM_SEC_IN_DAY ) + ( hour * NUM_SEC_IN_HOUR ) ) ) / NUM_SEC_IN_MIN;*/

	//swprintf(sText, L"Incident #%d - %s, Day %d, %02d:%02d", incident.usID, wSectorName_Target, day, hour, minute);
	swprintf(sText, L"--- currently under construction ---");
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, CAMPHIS_FONT_BIG, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	
	

	// previous/next buttons
	/*usPosX = CAMPAIGN_HISTORY_PAGEBTN_X;
	usPosY = CAMPAIGN_HISTORY_PAGEBTN_Y;
	swprintf(sText, L"Summary");
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	swprintf(sText, L"Detail");
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	swprintf(sText, L"Previous");
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );

	usPosX += CAMPAIGN_HISTORY_PAGEBTN_STEP_X;
	swprintf(sText, L"Next");
	DrawTextToScreen( sText, usPosX, usPosY, LAPTOP_SCREEN_LR_X-LAPTOP_SCREEN_UL_X, INS_FONT_SMALL, CAMPAIGN_HISTORY_FONT_COLOR_REGULAR, FONT_MCOLOR_BLACK, FALSE, 0 );*/


	SetFontShadow( DEFAULT_SHADOW );
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectCampaignHistoryNewsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32 uiLink = MSYS_GetRegionUserData( pRegion, 0 );

		// only display existing incidents
		UINT8 numpages = gCampaignStats.usNumIncidents;
		/*for(int i = 0; i < CAMPAIGNSTATS_NUM_LAST_INCIDENTS; ++i)
		{
			if ( gCampaignStats.lastIncidents[i].usID )
				++numpages;
		}*/

		if( uiLink == 0 )
		{
			
		}
		else if( uiLink == 1 )
		{
			
		}
		else if( uiLink == 2 )
		{
			if ( gusNewsPage == 0 )
				gusNewsPage = numpages;

			--gusNewsPage;
		}
		else if( uiLink == 3 )
		{
			++gusNewsPage;

			if ( gusNewsPage >= numpages )
				gusNewsPage = 0;
		}

		RenderCampaignHistory_News();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}
////////////////////////// NEWS PAGE //////////////////////////////////
