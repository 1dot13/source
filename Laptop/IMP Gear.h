#ifndef __IMP_GEAR__H_
#define __IMP_GEAR__H_

/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "Types.h"
#include "soldier profile type.h"

void	EnterIMPGear( void );
void	RenderIMPGear( void );
void	ExitIMPGear( void );
void	HandleIMPGear( void );

void	DisplayGear( UINT16 usItem, UINT16 usPosX, UINT16 usPosY, BOOLEAN fWithBackGround, UINT8 aNumber, BOOLEAN fDisplayNumber );

void	GiveIMPSelectedGear( MERCPROFILESTRUCT *pProfile );
INT32	GetIMPGearCost();

#endif
