#ifndef __IMP_BACKGROUND__H_
#define __IMP_BACKGROUND__H_

#include "Types.h"
#include "mousesystem.h"

void	EnterIMPBackground( void );
void	RenderIMPBackground( void );
void	ExitIMPBackground( void );
void	HandleIMPBackground( void );

BOOLEAN IsBackGroundAllowed( UINT8 ubNumber );

void	AssignBackgroundHelpText( UINT8 ubNumber, MOUSE_REGION* pMouseregion );

#endif
