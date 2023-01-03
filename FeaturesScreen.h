#ifndef _FEATURES_SCREEN__H_
#define _FEATURES_SCREEN__H_

#include "MessageBoxScreen.h"
#include "Options Screen.h"

namespace FeaturesScreen {

UINT32	Shutdown(void);
UINT32	Handle(void);
UINT32	Init(void);

BOOLEAN DoMessageBox(UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback);
BOOLEAN DoMessageBoxWithRect(UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect* pCenteringRect);

void SetPreviousScreen(UINT32 screenId, BOOLEAN setOptionsToo = TRUE);

}
#endif
