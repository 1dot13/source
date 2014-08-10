#ifndef __CAMPAIGNHISTORY_MAIN_H
#define __CAMPAIGNHISTORY_MAIN_H

/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#define		CAMPHIS_FONT_COLOR								2
#define		CAMPHIS_FONT_COLOR_RED							FONT_MCOLOR_RED
#define		CAMPHIS_FONT_BIG								FONT14ARIAL
#define		CAMPHIS_FONT_MED								FONT12ARIAL
#define		CAMPHIS_FONT_SMALL								FONT10ARIAL

#define		CAMPHIS_FONT_SHADOW								FONT_MCOLOR_WHITE

#define		CAMPAIGN_HISTORY_LINK_START_X					LAPTOP_SCREEN_UL_X
#define		CAMPAIGN_HISTORY_LINK_START_Y					LAPTOP_SCREEN_WEB_UL_Y + 5
#define		CAMPAIGN_HISTORY_LINK_TEXT_WIDTH				107
#define		CAMPAIGN_HISTORY_LINK_STEP_Y					14

void GameInitCampaignHistory();
BOOLEAN EnterCampaignHistory();
void ExitCampaignHistory();
void HandleCampaignHistory();
void RenderCampaignHistory();

BOOLEAN InitCampaignHistoryDefaults();
void DisplayCampaignHistoryDefaults();
void RemoveCampaignHistoryDefaults();
void GetCampaignHistoryText( UINT8 ubNumber, STR16 pString );

#endif //__CAMPAIGNHISTORY_MAIN_H
