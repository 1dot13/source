#ifndef __EXIT_GUI_H
#define __EXIT_GUI_H

#include "types.h"

enum
{
	LEAVE_SECTOR_TYPE_GOTO_SECTOR,	//Uses "Go to Sector" text for check box button
	LEAVE_SECTOR_TYPE_GOTO_MAP,			//Uses "Go to map" text for check box button
};

enum
{
	LEAVE_SECTOR_GOTO_SECTOR_NOT_DISABLED, //Shows helptext for checkbox explaining what will happen if button is checked
	LEAVE_SECTOR_GOTO_MAP_NOT_DISABLED,		//Same as previous, but uses "map" instead of "sector".
	LEAVE_SECTOR_DISABLED_BECAUSE_MULTIPLE_SQUADS_IN_HOSTILE_SECTOR, //Self-explanatory
};


extern BOOLEAN gfInSectorExitMenu;

BOOLEAN InitSectorExitMenu( UINT8 ubDirection, INT32 sAdditionalData );//dnl ch56 151009
void RenderSectorExitMenu( );
BOOLEAN HandleSectorExitMenu( );
void RemoveSectorExitMenu( BOOLEAN fOK );


#endif