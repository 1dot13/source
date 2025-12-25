#ifndef _MAINMENU_H
#define _MAINMENU_H

//MainMenu by Jazz
#define MAX_MAIN_MENU_IMAGE		200	
#define MAX_MAIN_MENU_CHARS		500	
#define MAX_ELEMENT		20	

typedef struct
{
	UINT32	uiIndex; // add
	BOOLEAN	Visible;
	CHAR8	Description[MAX_MAIN_MENU_CHARS];
	CHAR8	FileName[MAX_MAIN_MENU_CHARS];
	BOOLEAN FitToScreen;	
	BOOLEAN StretchImage;
	BOOLEAN CenterImage;
	BOOLEAN ImagePositionRelative;
	INT32	ImagePositionX;
	INT32	ImagePositionY;
	
	UINT32	MAINMENU_Y;
	UINT32	MAINMENU_Y_SPACE;
	UINT32	MAINMENU_X;

} MAIN_MENU_VALUES;

//Main Menu layout by Jazz
extern MAIN_MENU_VALUES gMainMenulayout[MAX_MAIN_MENU_IMAGE];

extern UINT32		MAINMENU_Y; 
extern UINT32		MAINMENU_X; 
extern UINT32		MAINMENU_Y_SPACE;

BOOLEAN InitMainMenu( );
void ClearMainMenu( );

void InitDependingGameStyleOptions();

#endif
