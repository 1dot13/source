#ifndef __CAMPAIGNHISTORY_SUMMARY_H
#define __CAMPAIGNHISTORY_SUMMARY_H

/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

// webpage displaying a summary of events
void GameInitCampaignHistorySummary();
BOOLEAN EnterCampaignHistorySummary();
void ExitCampaignHistorySummary();
void HandleCampaignHistorySummary();
void RenderCampaignHistorySummary();

// webpage displaying most important incidents
void GameInitCampaignHistory_MostImportant();
BOOLEAN EnterCampaignHistory_MostImportant();
void ExitCampaignHistory_MostImportant();
void HandleCampaignHistory_MostImportant();
void RenderCampaignHistory_MostImportant();

// webpage displaying most recent incidents	(not useable for the time being)
void GameInitCampaignHistory_News();
BOOLEAN EnterCampaignHistory_News();
void ExitCampaignHistory_News();
void HandleCampaignHistory_News();
void RenderCampaignHistory_News();

#endif	// __CAMPAIGNHISTORY_SUMMARY_H