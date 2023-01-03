#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include "vobject.h"
	#include "video.h"
	#include "font.h"
	#include "Font Control.h"
	#include "messagebox.h"
	#include "input.h"
	#include "english.h"
#endif

//internal variables.
INT32 iMsgBoxNum;
INT32 iMsgBoxOkImg, iMsgBoxCancelImg;
INT32 iMsgBoxBgrnd,iMsgBoxOk,iMsgBoxCancel;
SGPRect MsgBoxRect;

BOOLEAN gfMessageBoxResult = FALSE;
UINT8 gubMessageBoxStatus = MESSAGEBOX_NONE;

void MsgBoxOkClkCallback( GUI_BUTTON *butn, INT32 reason );
void MsgBoxCnclClkCallback( GUI_BUTTON *butn, INT32 reason );

void CreateMessageBox( STR16 wzString )
{
	INT16 sPixLen;
	INT16 sStartX, sStartY;

	sPixLen = StringPixLength ( wzString, (UINT16)gpLargeFontType1 ) + 10;
	if ( sPixLen > iScreenWidthOffset + 600 )
		sPixLen = iScreenWidthOffset + 600;

	sStartX = (SCREEN_WIDTH - sPixLen) / 2;
	sStartY = ((SCREEN_HEIGHT - 96) / 2);

	gfMessageBoxResult = FALSE;

	// Fake button for background w/ text
	iMsgBoxBgrnd = CreateTextButton( wzString, (UINT16)gpLargeFontType1,
									FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT, sStartX, sStartY,
									sPixLen, 96, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 2,
									BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK);
	DisableButton( iMsgBoxBgrnd );
	SpecifyDisabledButtonStyle( iMsgBoxBgrnd, DISABLED_STYLE_NONE );

	iMsgBoxOkImg = LoadButtonImage("EDITOR//ok.sti",0,1,2,3,4);
	iMsgBoxCancelImg = LoadButtonImage("EDITOR//cancel.sti",0,1,2,3,4);

	iMsgBoxOk = QuickCreateButton(iMsgBoxOkImg, (INT16)(sStartX + (sPixLen/2) - 35), (INT16)(sStartY + 58),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BUTTON_NO_CALLBACK, (GUI_CALLBACK)MsgBoxOkClkCallback);

	iMsgBoxCancel = QuickCreateButton(iMsgBoxCancelImg, (INT16)(sStartX + (sPixLen/2) + 5), (INT16)(sStartY + 58),
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BUTTON_NO_CALLBACK, (GUI_CALLBACK)MsgBoxCnclClkCallback);

	MsgBoxRect.iLeft = sStartX;
	MsgBoxRect.iTop = sStartY;
	MsgBoxRect.iRight = sStartX + sPixLen;
	MsgBoxRect.iBottom = sStartY + 96;

	RestrictMouseCursor( &MsgBoxRect );

	gubMessageBoxStatus = MESSAGEBOX_WAIT;

}

BOOLEAN MessageBoxHandled()
{
	InputAtom DummyEvent;

	while (DequeueSpecificEvent(&DummyEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if ( DummyEvent.usEvent == KEY_DOWN )
		{
			switch( DummyEvent.usParam )
			{
				case ENTER:
				case 'y':
				case 'Y':
					gubMessageBoxStatus = MESSAGEBOX_DONE;
					gfMessageBoxResult = TRUE;
					break;
				case ESC:
				case 'n':
				case 'N':
					gubMessageBoxStatus = MESSAGEBOX_DONE;
					gfMessageBoxResult = FALSE;
					break;
			}
		}
	}

	if( gubMessageBoxStatus == MESSAGEBOX_DONE )
	{
		while( DequeueEvent( &DummyEvent ) )
			continue;
	}
	MarkButtonsDirty();
	RenderButtons( );
//	InvalidateScreen( );
//	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender( );
	return gubMessageBoxStatus == MESSAGEBOX_DONE;
}

void RemoveMessageBox( )
{
	FreeMouseCursor( FALSE );
	RemoveButton( iMsgBoxCancel );
	RemoveButton( iMsgBoxOk );
	RemoveButton( iMsgBoxBgrnd );
	UnloadButtonImage( iMsgBoxOkImg );
	UnloadButtonImage( iMsgBoxCancelImg );
	gubMessageBoxStatus = MESSAGEBOX_NONE;
}


//----------------------------------------------------------------------------------------------
//	Quick Message Box button callback functions.
//----------------------------------------------------------------------------------------------

void MsgBoxOkClkCallback( GUI_BUTTON *butn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		butn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gubMessageBoxStatus = MESSAGEBOX_DONE;
		gfMessageBoxResult = TRUE;
	}
}

void MsgBoxCnclClkCallback( GUI_BUTTON *butn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		butn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gubMessageBoxStatus = MESSAGEBOX_DONE;
		gfMessageBoxResult = FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	End of the quick message box callback functions
//----------------------------------------------------------------------------------------------


#endif







