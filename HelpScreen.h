#ifndef _HELP_SCREEN__H_
#define _HELP_SCREEN__H_


//enum used for the different help screens that can come up
enum
{
	HELP_SCREEN_LAPTOP,
	HELP_SCREEN_MAPSCREEN,
	HELP_SCREEN_MAPSCREEN_NO_ONE_HIRED,
	HELP_SCREEN_MAPSCREEN_NOT_IN_ARULCO,
	HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY,
	HELP_SCREEN_TACTICAL,
	HELP_SCREEN_OPTIONS,
	HELP_SCREEN_LOAD_GAME,

	HELP_SCREEN_NUMBER_OF_HELP_SCREENS,
};




typedef struct
{
	INT8		bCurrentHelpScreen;
	UINT32	uiFlags;

	UINT16	usHasPlayerSeenHelpScreenInCurrentScreen;

	UINT8		ubHelpScreenDirty;

	UINT16	usScreenLocX;
	UINT16	usScreenLocY;
	UINT16	usScreenWidth;
	UINT16	usScreenHeight;

	INT32		iLastMouseClickY;			//last position the mouse was clicked ( if != -1 )

	INT8		bCurrentHelpScreenActiveSubPage;	//used to keep track of the current page being displayed

	INT8		bNumberOfButtons;

	//used so if the user checked the box to show the help, it doesnt automatically come up every frame
	BOOLEAN	fHaveAlreadyBeenInHelpScreenSinceEnteringCurrenScreen;

	INT8		bDelayEnteringHelpScreenBy1FrameCount;
	UINT16	usLeftMarginPosX;

	UINT16	usCursor;

	BOOLEAN	fWasTheGamePausedPriorToEnteringHelpScreen;



	//scroll variables
	UINT16	usTotalNumberOfPixelsInBuffer;
	INT32		iLineAtTopOfTextBuffer;
	UINT16	usTotalNumberOfLinesInBuffer;
	BOOLEAN	fForceHelpScreenToComeUp;

} HELP_SCREEN_STRUCT;

extern HELP_SCREEN_STRUCT gHelpScreen;




BOOLEAN ShouldTheHelpScreenComeUp( UINT8 ubScreenID, BOOLEAN fForceHelpScreenToComeUp );
void HelpScreenHandler();
void InitHelpScreenSystem();
void NewScreenSoResetHelpScreen( );
INT8 HelpScreenDetermineWhichMapScreenHelpToShow();


#endif