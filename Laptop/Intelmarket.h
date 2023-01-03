#ifndef __INTELMARKET_H
#define __INTELMARKET_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// displays a few quotes of customers
void EnterIntelmarket();
void ExitIntelmarket();
void HandleIntelmarket();
void RenderIntelmarket();

void EnterIntelmarket_About();
void ExitIntelmarket_About();
void HandleIntelmarket_About();
void RenderIntelmarket_About();

void EnterIntelmarket_Sell();
void ExitIntelmarket_Sell();
void HandleIntelmarket_Sell();
void RenderIntelmarket_Sell();

enum
{
	PHOTOFACT_NONE = 0,
	PHOTOFACT_TAKEN,
	PHOTOFACT_UPLOADED,
	PHOTOFACT_VERIFIED,
};

#endif //__INTELMARKET_H
