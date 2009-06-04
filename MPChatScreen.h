#ifndef _MP_CHAT_SCREEN_H_
#define _MP_CHAT_SCREEN_H_

#include "MessageBoxScreen.h"

UINT32	MPChatScreenInit( void );
UINT32	MPChatScreenHandle( void );
UINT32	MPChatScreenShutdown( void );

extern CHAR16 gszChatBoxInputString[255];
extern BOOLEAN gbChatSendToAll;
extern MESSAGE_BOX_STRUCT	gChatBox;
extern BOOLEAN	gfInChatBox;

#define CHAT_FONT_COLOR		208 // WHITE

////////////////////////////////
// ubStyle:				Determines the look of graphics including buttons
// zString:				16-bit string
// uiExitScreen		The screen to exit to
// ubFlags				Some flags for button style
// ReturnCallback	Callback for return. Can be NULL. Returns any above return value
// pCenteringRect	Rect to send if MSG_BOX_FLAG_USE_CENTERING_RECT set. Can be NULL 
////////////////////////////////

INT32 DoChatBox( bool bIncludeChatLog, const STR16 zString, UINT32 uiExitScreen, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );
void ChatLogMessage( UINT16 usColor, UINT8 ubPriority, STR16 pStringA, ...);

#endif