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

void	GiveIMPSelectedGear( MERCPROFILESTRUCT *pProfile );
INT32	GetIMPGearCost();

#endif
