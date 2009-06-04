#ifndef __IMP_COLOR_CHOOSING_SKIN_HAIR__H_
#define __IMP_COLOR_CHOOSING_SKIN_HAIR__H_

#include "Types.h"

void EnterIMPColorChoice( void );
void RenderIMPColorChoice( void );
void ExitIMPColorChoice( void );
void HandleIMPColorChoice( void );


INT8 iChosenSkin();
INT8 iChosenHair();
INT8 iChosenShirt();
INT8 iChosenPants();

BOOLEAN bBigBodySelected();
BOOLEAN bBadAssSelected();

#endif
