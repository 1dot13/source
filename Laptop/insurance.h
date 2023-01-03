#ifndef __INSURANCE_H
#define __INSURANCE_H


#define		INS_FONT_COLOR											2
#define		INS_FONT_COLOR_RED									FONT_MCOLOR_RED
#define		INS_FONT_BIG												FONT14ARIAL
#define		INS_FONT_MED												FONT12ARIAL
#define		INS_FONT_SMALL											FONT10ARIAL

#define		INS_FONT_BTN_COLOR									FONT_MCOLOR_WHITE
#define		INS_FONT_BTN_SHADOW_COLOR						2

#define		INS_FONT_SHADOW											FONT_MCOLOR_WHITE

#define		INSURANCE_BULLET_TEXT_OFFSET_X			21

#define		INS_INFO_LEFT_ARROW_BUTTON_X			71 + LAPTOP_SCREEN_UL_X
#define		INS_INFO_LEFT_ARROW_BUTTON_Y			354 + LAPTOP_SCREEN_WEB_UL_Y

#define		INS_INFO_RIGHT_ARROW_BUTTON_X			409 + LAPTOP_SCREEN_UL_X
#define		INS_INFO_RIGHT_ARROW_BUTTON_Y			INS_INFO_LEFT_ARROW_BUTTON_Y



void GameInitInsurance();
BOOLEAN EnterInsurance();
void ExitInsurance();
void HandleInsurance();
void RenderInsurance();

BOOLEAN InitInsuranceDefaults();
void DisplayInsuranceDefaults();
void RemoveInsuranceDefaults();
void DisplaySmallColouredLineWithShadow( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 EndY, UINT32 colour = FROMRGB( 255, 0, 0 ) );
void GetInsuranceText( UINT8 ubNumber, STR16 pString );



#endif



 


