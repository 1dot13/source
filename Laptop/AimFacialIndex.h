#ifndef	__AIMFACIALINDEX_H_
#define	__AIMFACIALINDEX_H_

#define	AIM_FI_LEFT_CLICK_TEXT_X					(SCREEN_WIDTH - 640) / 2 + 116
#define	AIM_FI_LEFT_CLICK_TEXT_Y					(SCREEN_HEIGHT - 480) / 2 + 35 + LAPTOP_SCREEN_WEB_DELTA_Y

#define		AIM_FI_CLICK_DESC_TEXT_Y_OFFSET		16

#define	AIM_FI_RIGHT_CLICK_TEXT_X					(SCREEN_WIDTH - 640) / 2 + 500

#define		AIM_FI_CLICK_TEXT_WIDTH						110

#define		AIM_FI_HELP_FONT									FONT10ARIAL
#define		AIM_FI_HELP_TITLE_FONT						FONT12ARIAL	
#define	IM_FI_HELP_COLOR							FONT_MCOLOR_WHITE

void GameInitAimFacialIndex();
BOOLEAN EnterAimFacialIndex();
void ExitAimFacialIndex();
void HandleAimFacialIndex();
BOOLEAN RenderAimFacialIndex();

BOOLEAN DisplayAimFIMugShot();

#endif




 


