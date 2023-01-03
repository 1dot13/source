#ifndef __AIM_H
#define __AIM_H

#include "Cursors.h"
#include "Soldier Profile.h"

//
// AIM defines
//


#define	AIMHISTORYFILE		"BINARYDATA\\AimHist.edt"
#define AIM_HISTORY_LINE_SIZE 400 * 2

//#define		MAX_NUMBER_MERCS							40

extern UINT8 MAX_NUMBER_MERCS;

extern UINT8			AimMercArray[ NUM_PROFILES ];//MAX_NUMBER_MERCS ];

typedef struct
{
	UINT8   ubAimArrayID;
	UINT8   uiIndex;
	UINT8   ProfilId;
	UINT8   AimBio;
	
}	AIM_AVAILABLE;

typedef struct
{
	UINT8   ubAimArrayID;
	UINT8   uiIndex;
	UINT8   ProfilId;
	UINT8   AimBio;
	
}	AIM_AVAILABLE_TEMP;

extern AIM_AVAILABLE_TEMP gAimAvailabilityTemp[ NUM_PROFILES ];
extern AIM_AVAILABLE gAimAvailability[ NUM_PROFILES ];

#define		NUM_AIM_SCREENS								6

#define		IMAGE_OFFSET_X							LAPTOP_SCREEN_UL_X
#define		IMAGE_OFFSET_Y							LAPTOP_SCREEN_WEB_UL_Y

#define		AIM_LOGO_TEXT_X							iScreenWidthOffset + 175
#define		AIM_LOGO_TEXT_Y							iScreenHeightOffset + 101
#define			AIM_LOGO_TEXT_WIDTH					360

// Aim Symbol 203, 51
#define			AIM_SYMBOL_SIZE_X						203
#define			AIM_SYMBOL_SIZE_Y						51

//262, 28
#define			AIM_SYMBOL_X								IMAGE_OFFSET_X + 149
#define			AIM_SYMBOL_Y								IMAGE_OFFSET_Y + 3
#define			AIM_SYMBOL_WIDTH						203
#define			AIM_SYMBOL_HEIGHT						51

//tais: small logo for nsgi
#define			AIM_SYMBOL_SMALL_X						IMAGE_OFFSET_X + 3
#define			AIM_SYMBOL_SMALL_Y						IMAGE_OFFSET_Y + 3
#define			AIM_SYMBOL_SMALL_WIDTH					102
#define			AIM_SYMBOL_SMALL_HEIGHT					26


// RustBackGround
#define			RUSTBACKGROUND_SIZE_X				125
#define			RUSTBACKGROUND_SIZE_Y				100

#define			RUSTBACKGROUND_1_X					IMAGE_OFFSET_X
#define			RUSTBACKGROUND_1_Y					IMAGE_OFFSET_Y

//Bottom Buttons
#define			NUM_AIM_BOTTOMBUTTONS				6
#define			BOTTOM_BUTTON_START_WIDTH		75
#define			BOTTOM_BUTTON_START_HEIGHT	18
#define			BOTTOM_BUTTON_START_X				LAPTOP_SCREEN_UL_X + 25

//#define		BOTTOM_BUTTON_START_Y					LAPTOP_SCREEN_WEB_LR_Y - BOTTOM_BUTTON_START_HEIGHT - 57
#define		BOTTOM_BUTTON_START_Y					(LAPTOP_SCREEN_WEB_LR_Y - BOTTOM_BUTTON_START_HEIGHT - 3)
#define			BOTTOM_BUTTON_AMOUNT				NUM_AIM_SCREENS
#define			BOTTOM_BUTTON_GAP						0
#define			BOTTOM_BUTTON_TEXT_Y				BOTTOM_BUTTON_START_Y + 6
#define			BOTTOM_BUTTON_END_X					BOTTOM_BUTTON_START_X + BOTTOM_BUTTON_START_WIDTH * 6
#define			BOTTOM_BUTTON_END_Y					BOTTOM_BUTTON_START_Y + BOTTOM_BUTTON_START_HEIGHT

#define			AIM_FONT10ROMAN							FONT10ROMAN
#define			AIM_FONT12ROMAN							FONT12ROMAN
#define			AIM_FONT14SANSERIF					FONT14SANSERIF
#define			AIM_LOGO_FONT								FONT10ARIAL
#define			AIM_COPYRIGHT_FONT					FONT10ARIAL
#define			AIM_WARNING_FONT						FONT12ARIAL
#define			AIM_FONT12ARIAL							FONT12ARIAL
#define			AIM_FONT_MCOLOR_WHITE				FONT_MCOLOR_WHITE
#define		IM_FONT_MCOLOR_DKRED					FONT_MCOLOR_RED
#define			AIM_GREEN										157
#define			AIM_MAINTITLE_COLOR					AIM_GREEN
#define			AIM_MAINTITLE_FONT					FONT14ARIAL
#define			AIM_BUTTON_ON_COLOR					FONT_MCOLOR_DKWHITE
#define			AIM_BUTTON_OFF_COLOR				138	
#define			AIM_BUTTON_DISABLED_COLOR		141	
#define			AIM_CONTENTBUTTON_WIDTH			205
#define			AIM_CONTENTBUTTON_HEIGHT		19
#define			AIM_FONT_GOLD								170

enum
{
	AIM_SLOGAN,
	AIM_WARNING_1,
	AIM_WARNING_2,
	AIM_COPYRIGHT_1,
	AIM_COPYRIGHT_2,
	AIM_COPYRIGHT_3
};

void GameInitAIM();
BOOLEAN EnterAIM();
void ExitAIM();
void HandleAIM();
void RenderAIM();

BOOLEAN ExitAimMenuBar(void);
BOOLEAN InitAimMenuBar(void);

BOOLEAN RemoveAimDefaults();
BOOLEAN InitAimDefaults();
BOOLEAN DrawAimDefaults();

BOOLEAN DisplayAimSlogan();
BOOLEAN DisplayAimCopyright();

void ResetAimButtons(UINT32 *Buttons, UINT16 uNumberOfButtons);
void DisableAimButton();

#endif
