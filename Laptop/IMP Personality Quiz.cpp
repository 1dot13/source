#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "CharProfile.h"
	#include "IMP Personality Quiz.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "Utilities.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "laptop.h"
	#include "IMP Compile Character.h"
	#include "soldier profile type.h"
	#include "IMP Text System.h"
	#include "input.h"
	#include "english.h"
#endif


// Kaiden this line was commented before I screwed with it
//extern struct POINT;

UINT32 giIMPPersonalityQuizButton[ 2 ];
UINT32 giIMPPersonalityQuizButtonImage[ 2 ];


// these are the buttons for the current question
INT32 giIMPPersonalityQuizAnswerButton[ 10 ];
INT32 giIMPPersonalityQuizAnswerButtonImage[ 10 ];

INT32 giPreviousQuestionButton;
INT32 giNextQuestionButton;

INT32 giPreviousQuestionButtonImage;
INT32 giNextQuestionButtonImage;

// this the currently highlighted answer
INT32 iCurrentAnswer = -1;

// the current quiz question
INT32 giCurrentPersonalityQuizQuestion = 0;
INT32 giPreviousPersonalityQuizQuestion = -1;
INT32 giMaxPersonalityQuizQuestion = 0;

// start over flag
BOOLEAN fStartOverFlag = FALSE;


#define BTN_FIRST_COLUMN_X 15
#define BTN_SECOND_COLUMN_X 256

#define INDENT_OFFSET 55

// number of IMP questions
#define MAX_NUMBER_OF_IMP_QUESTIONS 16

// answer list
INT32 iQuizAnswerList[MAX_NUMBER_OF_IMP_QUESTIONS];
// current number of buttons being shown
INT32 iNumberOfPersonaButtons = 0;

// function definitions
void DestroyIMPPersonalityQuizAnswerButtons(INT32 iNumberOfButtons );
void CreateIMPPersonalityQuizAnswerButtons( void );
void CreateIMPPersonalityQuizButtons( void );
void DestroyIMPersonalityQuizButtons( void );
void AddIMPPersonalityQuizAnswerButtons( INT32 iNumberOfButtons );
void DestroyPersonalityQuizButtons( void );
void ResetQuizAnswerButtons( );
void CompileQuestionsInStatsAndWhatNot( void );
void PrintQuizQuestionNumber( void );
void CheckStateOfTheConfirmButton( void );
void HandleIMPQuizKeyBoard( void );
void ToggleQuestionNumberButtonOn( INT32 iAnswerNumber );
void MoveBackAQuestion( void );
void MoveAheadAQuestion( void );
void CheckAndUpdateNextPreviousIMPQuestionButtonStates( void );


// answer button callbacks
void BtnIMPPersonalityQuizAnswer0Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer1Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer2Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer3Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer4Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer5Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer6Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer7Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer8Callback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizAnswer9Callback(GUI_BUTTON *btn,INT32 reason);

// other callbacks
void BtnIMPPersonalityQuizAnswerConfirmCallback(GUI_BUTTON *btn,INT32 reason);
void BtnIMPPersonalityQuizStartOverCallback(GUI_BUTTON *btn,INT32 reason);
void PreviousQuestionButtonCallback( GUI_BUTTON *btn, INT32 iReason );
void NextQuestionButtonCallback( GUI_BUTTON *btn, INT32 iReason );









void EnterIMPPersonalityQuiz( void )
{

	// void answers out the quiz
	memset( &iQuizAnswerList, -1, sizeof( INT32 ) * MAX_NUMBER_OF_IMP_QUESTIONS );

	// if we are entering for first time, reset
	if( giCurrentPersonalityQuizQuestion == MAX_NUMBER_OF_IMP_QUESTIONS )
	{
	giCurrentPersonalityQuizQuestion = 0;
	}
	// reset previous
	giPreviousPersonalityQuizQuestion = -1;

	// reset skills, attributes and personality
	ResetSkillsAttributesAndPersonality( );

	// create/destroy buttons for	questions, if needed
	CreateIMPPersonalityQuizAnswerButtons( );

	// now reset them
	ResetQuizAnswerButtons( );

	// create other buttons
	CreateIMPPersonalityQuizButtons( );

	return;
}

void RenderIMPPersonalityQuiz( void )
{
	// the background
	RenderProfileBackGround( );

	// highlight answer
	PrintImpText( );

	// indent for current and last page numbers
	//RenderAttrib2IndentFrame(BTN_FIRST_COLUMN_X + 2, 365 );

	// the current and last question numbers
	PrintQuizQuestionNumber( );



	return;
}

void ExitIMPPersonalityQuiz( void )
{


	// set previous to current, we want it's buttons gone!
	giPreviousPersonalityQuizQuestion = giCurrentPersonalityQuizQuestion;


	// destroy regular quiz buttons: the done and start over buttons
	DestroyIMPersonalityQuizButtons( );

	// destroy the buttons used for answers
	DestroyPersonalityQuizButtons( );

	if( fStartOverFlag )
	{
		fStartOverFlag = FALSE;
		giCurrentPersonalityQuizQuestion = 0;
	}
	return;
}


void HandleIMPPersonalityQuiz( void )
{

	// create/destroy buttons for	questions, if needed
	CreateIMPPersonalityQuizAnswerButtons( );

	// handle keyboard input
	HandleIMPQuizKeyBoard( );

	if( iCurrentAnswer == -1)
	{
		DisableButton(	giIMPPersonalityQuizButton[0] );
	}

	return;
}


void CreateIMPPersonalityQuizButtons( void )
{
	// this function will create the buttons needed for the IMP personality quiz Page





	// ths Done button
	giIMPPersonalityQuizButtonImage[0]=	LoadButtonImage( "LAPTOP\\button_7.sti" ,-1,0,-1,1,-1 );

// giIMPPersonalityQuizButton[0] = QuickCreateButton( giIMPPersonalityQuizButtonImage[0], LAPTOP_SCREEN_UL_X +	( 197 ), LAPTOP_SCREEN_WEB_UL_Y + ( 310 ),
//										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
//										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswerConfirmCallback);
//
	giIMPPersonalityQuizButton[0] = CreateIconAndTextButton( giIMPPersonalityQuizButtonImage[0], pImpButtonText[ 8 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 197 ), LAPTOP_SCREEN_WEB_UL_Y + ( 302 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswerConfirmCallback);

	// start over
	giIMPPersonalityQuizButtonImage[ 1 ]=	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );

	// giIMPPersonalityQuizButton[ 1 ] = QuickCreateButton( giIMPPersonalityQuizButtonImage[1], LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 310 ),
//										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
//										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPersonalityQuizStartOverCallback);
//

	giIMPPersonalityQuizButton[ 1 ] = CreateIconAndTextButton( giIMPPersonalityQuizButtonImage[ 1 ], pImpButtonText[ 7 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 302 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPersonalityQuizStartOverCallback);

	giPreviousQuestionButtonImage = LoadButtonImage( "LAPTOP\\button_3.sti" ,-1,0,-1,1,-1 );
	giPreviousQuestionButton = CreateIconAndTextButton( giPreviousQuestionButtonImage, pImpButtonText[ 12 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 197 ), LAPTOP_SCREEN_WEB_UL_Y + ( 361 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PreviousQuestionButtonCallback);

	giNextQuestionButtonImage = LoadButtonImage( "LAPTOP\\button_3.sti" ,-1,0,-1,1,-1 );
	giNextQuestionButton = CreateIconAndTextButton( giNextQuestionButtonImage, pImpButtonText[ 13 ], FONT12ARIAL,
														FONT_WHITE, DEFAULT_SHADOW,
														FONT_WHITE, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														LAPTOP_SCREEN_UL_X +	( 417 ), LAPTOP_SCREEN_WEB_UL_Y + ( 361 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)NextQuestionButtonCallback);


	SpecifyButtonTextSubOffsets( giNextQuestionButton, 0, -1, FALSE );
	SpecifyButtonTextSubOffsets( giPreviousQuestionButton, 0, -1, FALSE );

	DisableButton( giPreviousQuestionButton );
	DisableButton( giNextQuestionButton );

	SetButtonCursor( giIMPPersonalityQuizButton[0], CURSOR_WWW );
	SetButtonCursor( giIMPPersonalityQuizButton[1], CURSOR_WWW );
	SetButtonCursor( giPreviousQuestionButton, CURSOR_WWW );
	SetButtonCursor( giNextQuestionButton, CURSOR_WWW );

	return;
}

void DestroyIMPersonalityQuizButtons( void )
{
	// this function will destroy the buttons needed for the IMP personality quiz page

	// the done button
	RemoveButton(giIMPPersonalityQuizButton[ 0 ] );
	UnloadButtonImage(giIMPPersonalityQuizButtonImage[ 0 ] );

	// the start over button
	RemoveButton(giIMPPersonalityQuizButton[ 1 ] );
	UnloadButtonImage(giIMPPersonalityQuizButtonImage[ 1 ] );

	// previosu button
	RemoveButton( giPreviousQuestionButton );
	UnloadButtonImage( giPreviousQuestionButtonImage );

	// next button
	RemoveButton( giNextQuestionButton );
	UnloadButtonImage( giNextQuestionButtonImage );


	return;
}

void CreateIMPPersonalityQuizAnswerButtons( void )
{
	// this function will create the buttons for the personality quiz answer selections

	if( IMP_PERSONALITY_QUIZ != iCurrentImpPage )
	{
		// not valid pagre, get out
		return;
	}


	if( giCurrentPersonalityQuizQuestion == giPreviousPersonalityQuizQuestion )
	{
		// mode has not changed, return;
		return;
	}

	// destroy old screens buttons
	DestroyPersonalityQuizButtons( );

	// re-render screen
	RenderProfileBackGround( );

	switch( giCurrentPersonalityQuizQuestion )
	{
		case ( -1 ):
		// do nothing
		break;
		case ( 0 ):
		// 6 buttons
		iNumberOfPersonaButtons = 6;
		break;
		case ( 3 ):
			// 5 buttons
			iNumberOfPersonaButtons = 5;
	break;
		case ( 5 ):
			// 5 buttons
			iNumberOfPersonaButtons = 5;
	break;
		case ( 10 ):
			// 5 buttons
			iNumberOfPersonaButtons = 5;
	break;
		case ( 11 ):
			// 5 buttons
			iNumberOfPersonaButtons = 8;

	break;
		default:
		// 4 buttons
			iNumberOfPersonaButtons = 4;

		break;
	}

	AddIMPPersonalityQuizAnswerButtons( iNumberOfPersonaButtons );

	ToggleQuestionNumberButtonOn( iQuizAnswerList[ giCurrentPersonalityQuizQuestion ] );

	// re render text
	PrintImpText( );

	// the current and last question numbers
	PrintQuizQuestionNumber( );

	// title bar
	RenderWWWProgramTitleBar( );

	return;
}


void DestroyPersonalityQuizButtons( void )
{

	// this function will destroy the buttons used in the previous personality question
	// destroy old buttons
	switch( giPreviousPersonalityQuizQuestion	)
	{
		case ( -1 ):
			// do nothing
			break;
			case ( 0 ):
		// 6 buttons
	DestroyIMPPersonalityQuizAnswerButtons( 6 );
		break;
		case ( 3 ):
			// 5 buttons
		DestroyIMPPersonalityQuizAnswerButtons( 5 );
	break;
		case ( 5 ):
			// 5 buttons
		DestroyIMPPersonalityQuizAnswerButtons( 5 );
	break;
		case ( 10 ):
			// 5 buttons
		DestroyIMPPersonalityQuizAnswerButtons( 5 );
	break;
		case ( 11 ):
			// 5 buttons
		DestroyIMPPersonalityQuizAnswerButtons( 8 );
	break;
		default:
		// 4 buttons
		DestroyIMPPersonalityQuizAnswerButtons( 4 );
		break;
	}

	return;
}

void AddIMPPersonalityQuizAnswerButtons( INT32 iNumberOfButtons )
{
	// will add iNumberofbuttons to the answer button list
	INT32 iCounter = 0;
	CHAR16 sString[ 32 ];

	for(iCounter = 0; iCounter < iNumberOfButtons; iCounter++)
	{
	switch( iCounter )
		{
		case ( 0 ):
			giIMPPersonalityQuizAnswerButtonImage[0]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[0] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[0], LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 97 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer0Callback);

			break;
		case ( 1 ):
			giIMPPersonalityQuizAnswerButtonImage[1]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[1] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[1], LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 147 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer1Callback);

			break;
		case ( 2 ):
			giIMPPersonalityQuizAnswerButtonImage[2]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[2] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[2], LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 197 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer2Callback);

			break;
		case ( 3 ):
			giIMPPersonalityQuizAnswerButtonImage[3]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[3] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[3], LAPTOP_SCREEN_UL_X +	( BTN_FIRST_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 247 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer3Callback);

			break;
		case ( 4 ):
			giIMPPersonalityQuizAnswerButtonImage[4]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[4] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[4], LAPTOP_SCREEN_UL_X +	( BTN_SECOND_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 97 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer4Callback);

			break;
		case ( 5 ):
			giIMPPersonalityQuizAnswerButtonImage[5]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[5] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[5], LAPTOP_SCREEN_UL_X +	( BTN_SECOND_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 147 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer5Callback);

			break;
		case ( 6 ):
			giIMPPersonalityQuizAnswerButtonImage[6]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[6] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[6], LAPTOP_SCREEN_UL_X +	( BTN_SECOND_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 197 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer6Callback);

			break;
		case ( 7 ):
			giIMPPersonalityQuizAnswerButtonImage[7]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[7] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[7], LAPTOP_SCREEN_UL_X +	( BTN_SECOND_COLUMN_X ), LAPTOP_SCREEN_WEB_UL_Y + ( 247 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer7Callback);

			break;
		case ( 8 ):
			giIMPPersonalityQuizAnswerButtonImage[8]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[8] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[8], LAPTOP_SCREEN_UL_X +	( ( BTN_SECOND_COLUMN_X )	), LAPTOP_SCREEN_WEB_UL_Y + ( 268 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer8Callback);

			break;
		case ( 9 ):
			giIMPPersonalityQuizAnswerButtonImage[9]=	LoadButtonImage( "LAPTOP\\button_6.sti" ,-1,0,-1,1,-1 );
		giIMPPersonalityQuizAnswerButton[9] = QuickCreateButton( giIMPPersonalityQuizAnswerButtonImage[9], LAPTOP_SCREEN_UL_X +	( ( 276 - 46 ) / 2 ), LAPTOP_SCREEN_WEB_UL_Y + ( 147 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 3,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIMPPersonalityQuizAnswer9Callback);

			break;
		}
		swprintf( sString, L"%d", iCounter + 1 );
		SpecifyButtonUpTextColors( giIMPPersonalityQuizAnswerButton[ iCounter ], FONT_WHITE, FONT_BLACK );
		SpecifyButtonDownTextColors( giIMPPersonalityQuizAnswerButton[ iCounter ], FONT_WHITE, FONT_BLACK	);
		SpecifyButtonTextOffsets( giIMPPersonalityQuizAnswerButton[ iCounter ], +23, +12, TRUE );
		SpecifyButtonFont( giIMPPersonalityQuizAnswerButton[ iCounter ], FONT12ARIAL );
		SpecifyButtonText( giIMPPersonalityQuizAnswerButton[ iCounter ], sString );
	SetButtonCursor( giIMPPersonalityQuizAnswerButton[ iCounter ], CURSOR_WWW);
	}

	// previous is current
	giPreviousPersonalityQuizQuestion = giCurrentPersonalityQuizQuestion;
	return;
}


void DestroyIMPPersonalityQuizAnswerButtons(INT32 iNumberOfButtons )
{
	INT32 iCounter = 0;
	for(iCounter = 0; iCounter < iNumberOfButtons; iCounter++)
	{
	 RemoveButton(giIMPPersonalityQuizAnswerButton[ iCounter ] );
	 UnloadButtonImage(giIMPPersonalityQuizAnswerButtonImage[ iCounter ] );
		giIMPPersonalityQuizAnswerButton[ iCounter ] = -1;
	}


	return;
}


void BtnIMPPersonalityQuizAnswer0Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

 if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 0;
			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);



			// highlight answer
		PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;
		}
	}
}


void BtnIMPPersonalityQuizAnswer1Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 1;

			// highlight answer
		PrintImpText( );

			// the current and last question numbers
		PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;
		}
	}
}

void BtnIMPPersonalityQuizAnswer2Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 2;

				// highlight answer
		PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer3Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 3;


				// highlight answer
		PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer4Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 4;

				// highlight answer
		PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer5Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 5;


				// highlight answer
		PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer6Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 6;

				// highlight answer
		PrintIMPPersonalityQuizQuestionAndAnsers( );

				// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer7Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 7;

			// redraw text
			PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;
		}
	}
}

void BtnIMPPersonalityQuizAnswer8Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 8;

			PrintImpText( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

			fReDrawCharProfile = TRUE;

		}
	}
}

void BtnIMPPersonalityQuizAnswer9Callback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		// reset buttons
			ResetQuizAnswerButtons( );

			// now set this button on
		btn->uiFlags|=(BUTTON_CLICKED_ON);

			// ok, check to see if button was disabled, if so, re enable
		CheckStateOfTheConfirmButton( );

			iCurrentAnswer = 9;

				// highlight answer
		PrintIMPPersonalityQuizQuestionAndAnsers( );

			// the current and last question numbers
	 PrintQuizQuestionNumber( );

		}
	}
}






void BtnIMPPersonalityQuizAnswerConfirmCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{

		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			if( iCurrentAnswer != -1)
			{

				// reset all the buttons
				ResetQuizAnswerButtons( );

				// copy the answer into the list
		 iQuizAnswerList[ giCurrentPersonalityQuizQuestion ] = iCurrentAnswer;

				// reset answer for next question
				iCurrentAnswer = -1;

				// next question, JOHNNY!
				if( giCurrentPersonalityQuizQuestion == giMaxPersonalityQuizQuestion )
				{
					giMaxPersonalityQuizQuestion++;
				}

		giCurrentPersonalityQuizQuestion++;
				CheckAndUpdateNextPreviousIMPQuestionButtonStates();

				// OPPS!, done..time to finish up
				if( giCurrentPersonalityQuizQuestion > 15)
				{
					iCurrentImpPage = IMP_PERSONALITY_FINISH;
					// process
					CompileQuestionsInStatsAndWhatNot( );
				}
			}
		}
	}
}



void BtnIMPPersonalityQuizStartOverCallback(GUI_BUTTON *btn,INT32 reason)
{

	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{

		btn->uiFlags&=~(BUTTON_CLICKED_ON);
			giPreviousPersonalityQuizQuestion = giCurrentPersonalityQuizQuestion;
			giMaxPersonalityQuizQuestion = 0;
		fStartOverFlag = TRUE;

			iCurrentImpPage = IMP_PERSONALITY;
			fButtonPendingFlag = TRUE;
			iCurrentAnswer = -1;
		}
	}
}

void ResetQuizAnswerButtons( void )
{

	INT32 iCounter = 0;
	INT32 iCnt = 0;

	// how many buttons to reset?
	switch( giCurrentPersonalityQuizQuestion	)
	{
		case ( -1 ):
			// do nothing
			return;
			break;
		case ( 0 ):
		// 6 buttons
	iCounter =6;
		break;
		case ( 3 ):
			// 5 buttons
	 iCounter = 5;
	break;
		case ( 5 ):
			// 6 buttons
			iCounter = 5;
		break;
		case ( 10 ):
			// 6 buttons
			iCounter = 5;
		break;
		case ( 11 ):
			// 9 buttons
		iCounter = 8;
		break;
		default:
		// 4 buttons
		iCounter = 4;
		break;
	}

	// now run through and reset the buttons
	for(iCnt = 0; iCnt < iCounter; iCnt++)
	{
		ButtonList[ giIMPPersonalityQuizAnswerButton[ iCnt ] ]->uiFlags &= ~(BUTTON_CLICKED_ON);
	}

	return;
}


void CompileQuestionsInStatsAndWhatNot( void )
{
	// one BIG case/switch statement to determine what values are added where
	INT32 iCurrentQuestion = 0;

	for( iCurrentQuestion= 0; iCurrentQuestion < MAX_NUMBER_OF_IMP_QUESTIONS; iCurrentQuestion++ )
	{
		switch( iCurrentQuestion )
		{

		// ok, run throught he list of questions
		case ( 0 ):
			switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						if( fCharacterIsMale )
						{
							// Martial arts
							AddSkillToSkillList( MARTIALARTS_OT );
						}
						else
						{
							// for women, ambidexterious
							AddSkillToSkillList( AMBIDEXT_OT );
						}
				break;
			case( 1 ):
						AddAnAttitudeToAttitudeList(	ATT_LONER );
					break;
					case( 2 ):
							// hand to hand
						AddSkillToSkillList( HANDTOHAND_OT );
					break;
			case( 3 ):
					// lock picking
			AddSkillToSkillList( LOCKPICKING_OT );
					break;
					case( 4 ):
					// throwing
						AddSkillToSkillList( THROWING_OT );
					break;
					case( 5 ):
					// optimist
						AddAnAttitudeToAttitudeList( ATT_OPTIMIST );
					break;
				}
		break;
	 case ( 1 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// teaching
			 AddSkillToSkillList( TEACHING_OT );
				break;
			case( 1 ):
					AddSkillToSkillList( STEALTHY_OT );
					break;
					case( 2 ):
					// psycho
						AddAPersonalityToPersonalityList( PSYCHO );
					break;
			case( 3 ):
				AddAnAttitudeToAttitudeList(	ATT_FRIENDLY );
					break;
				}
		break;
	 case ( 2 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// lock picking
			AddSkillToSkillList( LOCKPICKING_OT );
				break;
			case( 1 ):
					// arrogant
						AddAnAttitudeToAttitudeList( ATT_ARROGANT );
					break;
					case( 2 ):
						AddSkillToSkillList( STEALTHY_OT );
					break;
			case( 3 ):
							// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;

				}
		break;
	 case ( 3 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// automatic weapons
			AddSkillToSkillList( AUTO_WEAPS_OT );
				break;
			case( 1 ):
						// friendly
						AddAnAttitudeToAttitudeList(	ATT_FRIENDLY );

					break;
					case( 2 ):
							// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;
			case( 3 ):
						// asshole
						AddAnAttitudeToAttitudeList( ATT_ASSHOLE );
					break;
					case( 4 ):
					AddAnAttitudeToAttitudeList(	ATT_LONER );
					break;

				}
		break;
		case ( 4 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
					// coward
						AddAnAttitudeToAttitudeList( ATT_COWARD );
				break;
			case( 1 ):
						// none
					break;
					case( 2 ):
							// aggressive
						AddAnAttitudeToAttitudeList( ATT_AGGRESSIVE );
					break;
			case( 3 ):
						// none
					break;
				}
		break;
		case ( 5 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
					// coward
						AddAnAttitudeToAttitudeList( ATT_COWARD );
				break;
			case( 1 ):
						AddSkillToSkillList( NIGHTOPS_OT );
					break;
					case( 2 ):
						// dont like boxes much
			AddAPersonalityToPersonalityList( CLAUSTROPHOBIC );
					break;
			case( 3 ):
						// none
					break;
					case( 4 ):
					// none
					break;
				}
		break;
		case ( 6 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// electronics
			AddSkillToSkillList( ELECTRONICS_OT );
				break;
			case( 1 ):
							//knifing
			AddSkillToSkillList( KNIFING_OT );
					break;
					case( 2 ):
					AddSkillToSkillList( NIGHTOPS_OT );
					break;
			case( 3 ):
						// none
					break;
				}
		break;
		case ( 7 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// ambidexterous
						AddSkillToSkillList( AMBIDEXT_OT );
				break;
			case( 1 ):
			// none
					break;
					case( 2 ):
			// optimist
					AddAnAttitudeToAttitudeList( ATT_OPTIMIST );
					break;
			case( 3 ):
			// psycho
						AddAPersonalityToPersonalityList( PSYCHO );
					break;
				}
		break;
		case ( 8 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
					// forgetful
						AddAPersonalityToPersonalityList( FORGETFUL );
				break;
			case( 1 ):
						// none
					case( 2 ):
							// pessimist
						AddAnAttitudeToAttitudeList( ATT_PESSIMIST );
					break;
			case( 3 ):
					// nervous
						AddAPersonalityToPersonalityList( NERVOUS );
					break;
				}
		break;
		case ( 9 ):

				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// none
				break;
			case( 1 ):
						// pessimist
						AddAnAttitudeToAttitudeList( ATT_PESSIMIST );
					break;
					case( 2 ):
						// asshole
						AddAnAttitudeToAttitudeList( ATT_ASSHOLE );
					break;
			case( 3 ):
					// nervous
						AddAPersonalityToPersonalityList( NERVOUS );
					break;
				}
		break;
		case ( 10 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
					case( 0 ):
					// none
				break;
			case( 1 ):
					// teaching
			 AddSkillToSkillList( TEACHING_OT );
					break;
					case( 2 ):
						// aggressive
						AddAnAttitudeToAttitudeList( ATT_AGGRESSIVE );
					break;
			case( 3 ):
						// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;
					case( 4 ):
						// none
					break;
				}
		break;
		case ( 11 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						if( fCharacterIsMale )
						{
							// Martial arts
							AddSkillToSkillList( MARTIALARTS_OT );
						}
						else
						{
							// for women, ambidexterious
							AddSkillToSkillList( AMBIDEXT_OT );
						}
				break;
			case( 1 ):
							//knifing
			AddSkillToSkillList( KNIFING_OT );
					break;
					case( 2 ):
					// none
					break;
			case( 3 ):
						// automatic weapons
			AddSkillToSkillList( AUTO_WEAPS_OT );
					break;
					case( 4 ):
							// hand to hand
						AddSkillToSkillList( HANDTOHAND_OT );
					break;
					case( 5 ):
					// electronics
			AddSkillToSkillList( ELECTRONICS_OT );
					break;
					case( 6 ):
					// ashole
					break;
					case( 7 ):
					// none
					break;
				}
		break;
		case ( 12 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
			// forgetful
						AddAPersonalityToPersonalityList( FORGETFUL );
					break;
			case( 1 ):
						// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;
					case( 2 ):
							// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;
			case( 3 ):
					// heat problems
			AddAPersonalityToPersonalityList( HEAT_INTOLERANT );
					break;
				}
		break;
		case ( 13 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// dont like boxes much
			AddAPersonalityToPersonalityList( CLAUSTROPHOBIC );
				break;
			case( 1 ):
							// normal
						AddAnAttitudeToAttitudeList( ATT_NORMAL );
					break;
					case( 2 ):
					// heat problems
			AddAPersonalityToPersonalityList( HEAT_INTOLERANT );
					break;
			case( 3 ):
					// none
					break;
				}
		break;
		case ( 14 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// throwing
			AddSkillToSkillList( THROWING_OT );
				break;
			case( 1 ):
						// ambidexterous
						AddSkillToSkillList( AMBIDEXT_OT );
					break;
					case( 3 ):
					// none
					break;
			case( 2 ):
					AddAnAttitudeToAttitudeList( ATT_ARROGANT );
					break;

				}
		break;
		case ( 15 ):
				switch( iQuizAnswerList[ iCurrentQuestion ] )
				{
				case( 0 ):
						// none !
				break;
			case( 1 ):
						// none !
					break;
					case( 2 ):
						// none !
					break;
			case( 3 ):
						// none !
					break;
				}
		break;
		}
	}
}


void BltAnswerIndents( INT32 iNumberOfIndents )
{
	INT32 iCounter = 0;


	// the question indent
	RenderQtnIndentFrame( 15, 20 );


	// the answers

	for( iCounter = 0; iCounter < iNumberOfIndents; iCounter++)
	{
		switch( iCounter )
		{
		case( 0 ):
		if( iNumberOfIndents < 5 )
				{
			RenderQtnLongIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 93);

					if( iCurrentAnswer == iCounter )
					{
						RenderQtnLongIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 93 );
					}
				}
		else
				{
			RenderQtnShortIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 93);

					if( iCurrentAnswer == iCounter )
					{
						RenderQtnShortIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 93 );
					}
				}
		break;
		case( 1 ):
		if( iNumberOfIndents < 5 )
				{
			RenderQtnLongIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 143);

					if( iCurrentAnswer == iCounter )
					{
						RenderQtnLongIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 143 );
					}
				}
		else
				{
			RenderQtnShortIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 143);

					if( iCurrentAnswer == iCounter )
					{
						RenderQtnShortIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 143 );
					}
				}
		break;
		case( 2 ):
		if( iNumberOfIndents < 5 )
			{
		RenderQtnLongIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 193);

				if( iCurrentAnswer == iCounter )
				{
				RenderQtnLongIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 193 );
				}
			}
		else
			{
		RenderQtnShortIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 193);

				if( iCurrentAnswer == iCounter )
				{
				RenderQtnShortIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 193 );
				}
			}
		break;
		case( 3 ):

				// is this question # 6 ..if so, need longer answer box
				if( ( giCurrentPersonalityQuizQuestion == 5) )
				{
					// render longer frame
					RenderQtnShort2IndentFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243 );

						// is this answer currently selected?
					if( iCurrentAnswer == iCounter )
					{
						// need to highlight
 				 RenderQtnShort2IndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243 );

					}
					// done
					break;
				}

		if( iNumberOfIndents < 5 )
				{
			RenderQtnLongIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243);

					if( iCurrentAnswer == iCounter )
					{
				 RenderQtnLongIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243 );
					}
				}
		else
				{
			RenderQtnShortIndentFrame(BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243);

					if( iCurrentAnswer == iCounter )
					{
				 RenderQtnShortIndentHighFrame( BTN_FIRST_COLUMN_X + INDENT_OFFSET, 243 );
					}
				}
		break;
		case( 4 ):

				//is this question # 14 or 21?..if so, need longer answer box
				if( ( giCurrentPersonalityQuizQuestion == 10)||( giCurrentPersonalityQuizQuestion == 5 ) )
					{
					// render longer frame
					RenderQtnShort2IndentFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 93 );

						// is this answer currently selected?
					if( iCurrentAnswer == iCounter )
						{
						// need to highlight
					RenderQtnShort2IndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 93 );

						}
					// done
					break;
					}

		 RenderQtnShortIndentFrame(BTN_SECOND_COLUMN_X + INDENT_OFFSET, 93);

				if( iCurrentAnswer == iCounter )
				{
				 RenderQtnShortIndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 93 );
				}
		break;
		case( 5 ):

				// special case?..longer frame needed if so
				if( ( giCurrentPersonalityQuizQuestion == 19) )
				{
					// render longer frame
					RenderQtnShort2IndentFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 143 );

						// is this answer currently selected?
					if( iCurrentAnswer == iCounter )
						{
						// need to highlight
				RenderQtnShort2IndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 143 );
						}
					// done
					break;
					}
		 RenderQtnShortIndentFrame(BTN_SECOND_COLUMN_X + INDENT_OFFSET, 143);
				if( iCurrentAnswer == iCounter )
				{
				 RenderQtnShortIndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 143 );
				}
		break;
		case( 6 ):
		 RenderQtnShortIndentFrame(BTN_SECOND_COLUMN_X + INDENT_OFFSET, 193);
				if( iCurrentAnswer == iCounter )
				{
				 RenderQtnShortIndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 193 );
				}
		break;
		case( 7 ):
		 RenderQtnShortIndentFrame(BTN_SECOND_COLUMN_X + INDENT_OFFSET, 243);
				if( iCurrentAnswer == iCounter )
				{
				 RenderQtnShortIndentHighFrame( BTN_SECOND_COLUMN_X + INDENT_OFFSET, 243 );
				}
		break;
		case( 8 ):
		break;
		}
	}
}

void PrintQuizQuestionNumber( void )
{
	// this function will print the number of the current question and the numebr of questions

	CHAR16 sString[ 10 ];


	// setup font
	SetFont( FONT12ARIAL);
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// get current question number into a string
	swprintf(sString, L"%d", giCurrentPersonalityQuizQuestion + 1);

	// print current question number
	mprintf( LAPTOP_SCREEN_UL_X + 345 , LAPTOP_SCREEN_WEB_UL_Y + 370 ,sString);

	// total number of questions
	mprintf( LAPTOP_SCREEN_UL_X + 383 , LAPTOP_SCREEN_WEB_UL_Y + 370 , L"16");
	return;
}



void CheckStateOfTheConfirmButton( void )
{
	// will check the state of the confirm button, should it be enabled or disabled?
	if( iCurrentAnswer == -1 )
	{
		// was disabled, enable
		EnableButton( giIMPPersonalityQuizButton[0] );
	}

	return;
}

void HandleIMPQuizKeyBoard( void )
{
	InputAtom					InputEvent;
	POINT	MousePos;
	BOOLEAN fSkipFrame = FALSE;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while( ( DequeueEvent(&InputEvent) == TRUE )	)
	{
	if( fSkipFrame == FALSE )
	{
		// HOOK INTO MOUSE HOOKS


		//
		//if( (InputEvent.usEvent == KEY_DOWN ) && ( InputEvent.usParam >= '1' ) && ( InputEvent.usParam <= '9') )
		//{
		//	if( ( UINT16 )( iNumberOfPersonaButtons ) >= InputEvent.usParam - '0' )
		//	{
				// reset buttons
		//		ResetQuizAnswerButtons( );

				// ok, check to see if button was disabled, if so, re enable
		//		CheckStateOfTheConfirmButton( );

				// toggle this button on
		//		ButtonList[ giIMPPersonalityQuizAnswerButton[ InputEvent.usParam - '1' ] ]->uiFlags |= (BUTTON_CLICKED_ON);

		//		iCurrentAnswer = InputEvent.usParam - '1';

		//		PrintImpText( );

				// the current and last question numbers
		//		PrintQuizQuestionNumber( );

		//		fReDrawCharProfile = TRUE;
		//		fSkipFrame = TRUE;
		//	}
//		}
//		else if( ( iCurrentAnswer != -1 ) && ( InputEvent.usEvent == KEY_DOWN ) && ( InputEvent.usParam == ENTER ) )
//		{
			// reset all the buttons
//			ResetQuizAnswerButtons( );

			// copy the answer into the list
//			iQuizAnswerList[ giCurrentPersonalityQuizQuestion ] = iCurrentAnswer;

			// reset answer for next question
//			iCurrentAnswer = -1;

			// next question, JOHNNY!
//			giCurrentPersonalityQuizQuestion++;
//			giMaxPersonalityQuizQuestion++;


			// OPPS!, done..time to finish up
//			if( giCurrentPersonalityQuizQuestion > 15)
//			{
//				iCurrentImpPage = IMP_PERSONALITY_FINISH;
				// process
//				CompileQuestionsInStatsAndWhatNot( );
//			}

//			fSkipFrame = TRUE;
//		}
//		else if( ( InputEvent.usEvent == KEY_DOWN ) && ( InputEvent.usParam == '=' ) )
//		{
//			MoveAheadAQuestion( );
//			fSkipFrame = TRUE;
//		}
//		else if( ( InputEvent.usEvent == KEY_DOWN ) && ( InputEvent.usParam == '-' ) )
//		{
//			MoveBackAQuestion( );
//			fSkipFrame = TRUE;
//		}
//		else
//		{

			switch(InputEvent.usEvent)
			{
				case LEFT_BUTTON_DOWN:
					MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);

					break;
				case LEFT_BUTTON_UP:
					MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);

					break;
				case RIGHT_BUTTON_DOWN:
					MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);

					break;
				case RIGHT_BUTTON_UP:
					MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);

					break;
				case RIGHT_BUTTON_REPEAT:
					MouseSystemHook(RIGHT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);

					break;
				case LEFT_BUTTON_REPEAT:
					MouseSystemHook(LEFT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);

					break;
				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
					break;
//			}
		}
		}
	}
	return;
}


void CheckAndUpdateNextPreviousIMPQuestionButtonStates( void )
{
	if( giCurrentPersonalityQuizQuestion >= giMaxPersonalityQuizQuestion )
	{
		DisableButton( giNextQuestionButton );
	}
	else
	{
		EnableButton( giNextQuestionButton );
	}

	if( giCurrentPersonalityQuizQuestion == 0 )
	{
		DisableButton( giPreviousQuestionButton );
	}
	else
	{
		EnableButton( giPreviousQuestionButton );
	}
}


void MoveAheadAQuestion( void )
{

	// move ahead a question in the personality question list
	if( giCurrentPersonalityQuizQuestion < giMaxPersonalityQuizQuestion )
	{
		giCurrentPersonalityQuizQuestion++;

		iCurrentAnswer = -1;
		CheckStateOfTheConfirmButton( );

		iCurrentAnswer = iQuizAnswerList[ giCurrentPersonalityQuizQuestion ];
	}

	CheckAndUpdateNextPreviousIMPQuestionButtonStates( );


//	EnableButton( giPreviousQuestionButton );

//	if( giCurrentPersonalityQuizQuestion >= giMaxPersonalityQuizQuestion )
//	{
//		DisableButton( giNextQuestionButton );
//		iCurrentAnswer = -1;
//	}
//	else
//	{
//		EnableButton( giNextQuestionButton );
//	}

	return;
}

void MoveBackAQuestion( void )
{
	if( giCurrentPersonalityQuizQuestion > 0 )
	{
		giCurrentPersonalityQuizQuestion--;

		iCurrentAnswer = -1;
		CheckStateOfTheConfirmButton( );

		iCurrentAnswer = iQuizAnswerList[ giCurrentPersonalityQuizQuestion ];
	}

	EnableButton( giNextQuestionButton );

	CheckAndUpdateNextPreviousIMPQuestionButtonStates( );

//	if( giCurrentPersonalityQuizQuestion == 0 )
//	{
//		DisableButton( giPreviousQuestionButton );
//	}
//	else
//	{
//		EnableButton( giPreviousQuestionButton );
//	}

	return;
}

void ToggleQuestionNumberButtonOn( INT32 iAnswerNumber )
{
	if( ( giCurrentPersonalityQuizQuestion <= giMaxPersonalityQuizQuestion ) && ( iAnswerNumber != -1 ) )
	{
		// reset buttons
		ResetQuizAnswerButtons( );

		// toggle this button on
		ButtonList[ giIMPPersonalityQuizAnswerButton[ iAnswerNumber ] ]->uiFlags |= (BUTTON_CLICKED_ON);
		iCurrentAnswer = iAnswerNumber;
	}

	return;

}

void PreviousQuestionButtonCallback( GUI_BUTTON *btn, INT32 iReason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
	return;

	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			MoveBackAQuestion( );
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
	return;
}


void NextQuestionButtonCallback( GUI_BUTTON *btn, INT32 iReason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
	return;

	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			MoveAheadAQuestion( );
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
	return;
}
