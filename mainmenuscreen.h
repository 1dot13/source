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
	CHAR8	FileName800x600[MAX_MAIN_MENU_CHARS];
	CHAR8	FileName1024x768[MAX_MAIN_MENU_CHARS];
	INT32	ImagePositionX;
	INT32	ImagePositionY;
	INT32	ImagePosition800x600X;
	INT32	ImagePosition800x600Y;
	INT32	ImagePosition1024x768X;
	INT32	ImagePosition1024x768Y;
	
	UINT32	MAINMENU_Y;
	UINT32	MAINMENU_800x600Y;
	UINT32	MAINMENU_1024x768Y;
	UINT32	MAINMENU_Y_SPACE;
	
	UINT32	MAINMENU_X;
	UINT32	MAINMENU_800x600X;
	UINT32	MAINMENU_1024x768X;

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