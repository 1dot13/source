#ifndef _MSGBOX_H
#define _MSGBOX_H

#include "mousesystem.h"

// Message box flags
#define		MSG_BOX_FLAG_USE_CENTERING_RECT			0x00000001		// Pass in a rect to center in
#define		MSG_BOX_FLAG_OK							0x00000002		// Displays OK button
#define		MSG_BOX_FLAG_YESNO						0x00000004		// Displays YES NO buttons
#define		MSG_BOX_FLAG_CANCEL						0x00000008		// Displays YES NO buttons
#define		MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS		0x00000010		// Displays four numbered buttons, 1-4
#define		MSG_BOX_FLAG_YESNOCONTRACT				0x00000020		// yes no and contract buttons
#define		MSG_BOX_FLAG_OKCONTRACT					0x00000040		// ok and contract buttons
#define		MSG_BOX_FLAG_YESNOLIE					0x00000080		// ok and contract buttons
#define		MSG_BOX_FLAG_CONTINUESTOP				0x00000100		// continue stop box
#define		MSG_BOX_FLAG_OKSKIP						0x00000200		// Displays ok or skip (meanwhile) buttons
#define		MSG_BOX_FLAG_GENERICCONTRACT			0x00000400		// displays contract buttoin + 2 user-defined text buttons
#define		MSG_BOX_FLAG_GENERIC_TWO_BUTTONS		0x00000800		// 2 user-defined text buttons
// OJW - Adding text chatbox 
#define		MSG_BOX_FLAG_INPUTBOX					0x00001000		// has a text input field
// Flugente - added boxes with multiple buttons that can be renamed easily
#define		MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS		0x00002000		// Displays four numbered buttons with definable labels
#define		MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS		0x00004000		// Displays eight numbered buttons with definable labels
#define		MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS	0x00008000		// Displays sixteen numbered buttons with definable labels

#define		MSG_BOX_FLAG_DROPDOWN_1					0x00010000		// Displays an additional dropdown box
#define		MSG_BOX_FLAG_DROPDOWN_2					0x00020000		// Displays another additional dropdown box
#define		MSG_BOX_FLAG_IMAGE						0x00040000		// Displays an image

// message box return codes
#define		MSG_BOX_RETURN_OK						1				// ENTER or on OK button
#define		MSG_BOX_RETURN_YES						2				// ENTER or YES button
#define		MSG_BOX_RETURN_NO						3				// ESC, Right Click or NO button
#define		MSG_BOX_RETURN_CONTRACT					4				// contract button
#define		MSG_BOX_RETURN_LIE						5				// LIE BUTTON

// message box style flags
enum
{
	MSG_BOX_BASIC_STYLE = 0,														// We'll have other styles, like in laptop, etc
																											// Graphics are all that are required here...
	MSG_BOX_RED_ON_WHITE,
	MSG_BOX_BLUE_ON_GREY,
	MSG_BOX_BASIC_SMALL_BUTTONS,
	MSG_BOX_IMP_STYLE,
	MSG_BOX_LAPTOP_DEFAULT,
	MSG_BOX_BASIC_MEDIUM_BUTTONS,
};

enum
{
	MSG_BOX_DEFAULT_BUTTON_NONE = 0,

	MSG_BOX_DEFAULT_BUTTON_1,
	MSG_BOX_DEFAULT_BUTTON_2,
	MSG_BOX_DEFAULT_BUTTON_3,
	MSG_BOX_DEFAULT_BUTTON_4,
	MSG_BOX_DEFAULT_BUTTON_5,
	MSG_BOX_DEFAULT_BUTTON_6,
	MSG_BOX_DEFAULT_BUTTON_7,
	MSG_BOX_DEFAULT_BUTTON_8,
	MSG_BOX_DEFAULT_BUTTON_9,
	MSG_BOX_DEFAULT_BUTTON_10,
	MSG_BOX_DEFAULT_BUTTON_11,
	MSG_BOX_DEFAULT_BUTTON_12,
	MSG_BOX_DEFAULT_BUTTON_13,
	MSG_BOX_DEFAULT_BUTTON_14,
	MSG_BOX_DEFAULT_BUTTON_15,
	MSG_BOX_DEFAULT_BUTTON_16,
};

typedef void (*MSGBOX_CALLBACK)( UINT8 bExitValue );	

// Flugente: made an array for user-defined buttons
#define		NUM_CUSTOM_BUTTONS	16

extern		CHAR16		gzUserDefinedButton[ NUM_CUSTOM_BUTTONS ][ 128 ];
// sevenfm: color for buttons
extern UINT16	gzUserDefinedButtonColor[ NUM_CUSTOM_BUTTONS ];

typedef struct
{
	UINT32						usFlags;
	UINT32						uiExitScreen;
	MSGBOX_CALLBACK		ExitCallback;
	INT16							sX;
	INT16							sY;
	UINT32						uiSaveBuffer;
	MOUSE_REGION			BackRegion;
	UINT16						usWidth;
	UINT16						usHeight;
	INT32							iButtonImages;
	union
	{
		struct
		{
			UINT32						uiOKButton;
			UINT32						uiYESButton;
			UINT32						uiNOButton;
			UINT32						uiUnusedButton;
		};
		struct
		{
			UINT32						uiButton[16];
		};
	};
	BOOLEAN						fRenderBox;
	INT8							bHandled;
	INT32							iBoxId;

	BOOLEAN						fWasPaused;
} MESSAGE_BOX_STRUCT;




extern		MESSAGE_BOX_STRUCT	gMsgBox;
extern		BOOLEAN							fRestoreBackgroundForMessageBox;

//this variable can be unset if ur in a non gamescreen and DONT want the msg box to use the save buffer
extern		BOOLEAN							gfDontOverRideSaveBuffer;

//OJW - 20090208
extern		CHAR16							gszMsgBoxInputString[255];
////////////////////////////////
// ubStyle:				Determines the look of graphics including buttons
// zString:				16-bit string
// uiExitScreen		The screen to exit to
// ubFlags				Some flags for button style
// ReturnCallback	Callback for return. Can be NULL. Returns any above return value
// pCenteringRect	Rect to send if MSG_BOX_FLAG_USE_CENTERING_RECT set. Can be NULL 
////////////////////////////////

INT32 DoMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect, UINT8 ubDefaultButton = MSG_BOX_DEFAULT_BUTTON_NONE );
void DoScreenIndependantMessageBox( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void DoUpperScreenIndependantMessageBox( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );

void DoLowerScreenIndependantMessageBox( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void DoScreenIndependantMessageBoxWithRect( const STR16 zString, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );

//wrappers for other screens
BOOLEAN DoLapTopSystemMessageBoxWithRect( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );
INT32 DoMapMessageBoxWithRect( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );
BOOLEAN DoOptionsMessageBoxWithRect( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );
BOOLEAN DoSaveLoadMessageBoxWithRect( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect );


#endif