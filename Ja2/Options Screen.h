#ifndef _OPTIONS_SCREEN__H_
#define _OPTIONS_SCREEN__H_

#include "MessageBoxScreen.h"

/////////////////////////////////
//
//	Defines
//
/////////////////////////////////
#define	OPT_MAIN_FONT							FONT10ARIAL
#define	OPT_SLIDER_FONT							FONT12ARIAL

#define	OPT_MAIN_COLOR							OPT_BUTTON_ON_COLOR
#define	OPT_HIGHLIGHT_COLOR						FONT_MCOLOR_WHITE

// these both could be established at run time, then never worry about it again
// derived from "OPTIONSCREENBASE.sti"
#define	OPTIONS_SCREEN_WIDTH					638
#define	OPTIONS_SCREEN_HEIGHT					478

#define	OPTIONS__TOP_LEFT_X						iScreenWidthOffset
#define	OPTIONS__TOP_LEFT_Y						iScreenHeightOffset
#define	OPTIONS__BOTTOM_RIGHT_X					OPTIONS__TOP_LEFT_X + OPTIONS_SCREEN_WIDTH
#define	OPTIONS__BOTTOM_RIGHT_Y					OPTIONS__TOP_LEFT_Y + OPTIONS_SCREEN_HEIGHT

#define	OPT_SAVE_BTN_X							iScreenWidthOffset + 20
#define	OPT_SAVE_BTN_Y							iScreenHeightOffset + 443

#define	OPT_LOAD_BTN_X							OPT_SAVE_BTN_X + 70
#define	OPT_LOAD_BTN_Y							OPT_SAVE_BTN_Y

#define	OPT_QUIT_BTN_X							OPT_LOAD_BTN_X + 70
#define	OPT_QUIT_BTN_Y							OPT_SAVE_BTN_Y

#define OPT_SWAP_BTN_X					OPT_QUIT_BTN_X + 70
#define OPT_SWAP_BTN_Y					OPT_SAVE_BTN_Y

// ary-05/05/2009 : need more option screen toggles : Add in buttons that allow for options column paging
#define	OPT_PREV_BTN_X							OPT_SWAP_BTN_X + 100
#define	OPT_PREV_BTN_Y							OPT_SAVE_BTN_Y 

#define	OPT_PAGE_X								OPT_PREV_BTN_X + 54
#define	OPT_PAGE_Y								OPT_SAVE_BTN_Y + 7

#define	OPT_NEXT_BTN_X							OPT_PREV_BTN_X + 110
#define	OPT_NEXT_BTN_Y							OPT_SAVE_BTN_Y 

#define	OPT_DONE_BTN_X							OPT_NEXT_BTN_X + 110
#define	OPT_DONE_BTN_Y							OPT_SAVE_BTN_Y

#define	OPT_GAP_BETWEEN_TOGGLE_BOXES			18

//Text
#define	OPT_TOGGLE_BOX_FIRST_COL_TEXT_X			OPT_TOGGLE_BOX_FIRST_COLUMN_X + OPT_SPACE_BETWEEN_TEXT_AND_TOGGLE_BOX
#define	OPT_TOGGLE_BOX_FIRST_COL_TEXT_Y			OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y

#define	OPT_TOGGLE_BOX_SECOND_TEXT_X			OPT_TOGGLE_BOX_SECOND_COLUMN_X + OPT_SPACE_BETWEEN_TEXT_AND_TOGGLE_BOX
#define	OPT_TOGGLE_BOX_SECOND_TEXT_Y			OPT_TOGGLE_BOX_SECOND_COLUMN_START_Y

//toggle boxes
#define	OPT_SPACE_BETWEEN_TEXT_AND_TOGGLE_BOX	30
#define	OPT_TOGGLE_TEXT_OFFSET_Y				2

#define	OPT_TOGGLE_BOX_FIRST_COLUMN_X			iScreenWidthOffset + 260
#define	OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y		iScreenHeightOffset + 82

#define	OPT_TOGGLE_BOX_SECOND_COLUMN_X			iScreenWidthOffset + 435
#define	OPT_TOGGLE_BOX_SECOND_COLUMN_START_Y	OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y

#define	OPT_TOGGLE_BOX_TEXT_WIDTH				OPT_TOGGLE_BOX_SECOND_COLUMN_X - OPT_TOGGLE_BOX_FIRST_COLUMN_X - 20

// Slider bar defines
#define	OPT_GAP_BETWEEN_SLIDER_BARS				60
#define	OPT_SLIDER_BAR_SIZE						258

#define	OPT_SLIDER_TEXT_WIDTH					45

#define	OPT_SOUND_FX_TEXT_X						iScreenWidthOffset + 38
#define	OPT_SOUND_FX_TEXT_Y						iScreenHeightOffset + 87

#define	OPT_SPEECH_TEXT_X						iScreenWidthOffset + 85
#define	OPT_SPEECH_TEXT_Y						OPT_SOUND_FX_TEXT_Y

#define	OPT_MUSIC_TEXT_X						iScreenWidthOffset + 137
#define	OPT_MUSIC_TEXT_Y						OPT_SOUND_FX_TEXT_Y

#define	OPT_TEXT_TO_SLIDER_OFFSET_Y				25

#define	OPT_SOUND_EFFECTS_SLIDER_X				iScreenWidthOffset + 56
#define	OPT_SOUND_EFFECTS_SLIDER_Y				iScreenHeightOffset + 126

#define	OPT_SPEECH_SLIDER_X						iScreenWidthOffset + 107
#define	OPT_SPEECH_SLIDER_Y						OPT_SOUND_EFFECTS_SLIDER_Y

#define	OPT_MUSIC_SLIDER_X						iScreenWidthOffset + 158
#define	OPT_MUSIC_SLIDER_Y						OPT_SOUND_EFFECTS_SLIDER_Y

#define	OPT_MUSIC_SLIDER_PLAY_SOUND_DELAY		75

#define	OPT_FIRST_COLUMN_TOGGLE_CUT_OFF			18
#define	MAX_NUMBER_OF_OPTION_TOGGLES			(OPT_FIRST_COLUMN_TOGGLE_CUT_OFF * 2)
#define		OPT_BUTTON_FONT												FONT14ARIAL
#define		OPT_BUTTON_FONT2											FONT10ARIAL //smaller button on options screen
#define		OPT_BUTTON_ON_COLOR										73//FONT_MCOLOR_WHITE
#define		OPT_BUTTON_OFF_COLOR									73//FONT_MCOLOR_WHITE



//Record the previous screen the user was in.
extern UINT32		guiPreviousOptionScreen;



UINT32	OptionsScreenShutdown( void );
UINT32	OptionsScreenHandle( void );
UINT32	OptionsScreenInit( void );



void SetOptionsScreenToggleBoxes();
void GetOptionsScreenToggleBoxes();
// OJW - moved this here so can exit options screen on client disconnect
void			SetOptionsExitScreen( UINT32 uiExitScreen );
void			SetOptionsPreviousScreen( UINT32 uiPrevScreen, BOOLEAN setFeaturesToo = TRUE );


BOOLEAN		DoOptionsMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );


#endif
