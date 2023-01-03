#ifndef __IMP_MAINPAGE_H
#define __IMP_MAINPAGE_H
#include "types.h"

void RenderIMPMainPage( void );
void ExitIMPMainPage( void );
void EnterIMPMainPage( void );
void HandleIMPMainPage( void );
void NextProfilingMode( void );
void ShadeUnSelectableButtons( void );

BOOLEAN IsIMPSlotFree(INT32 iIMPId);
INT32 GetFreeIMPSlot(INT32 iDefaultIMPId);

// WDS: Allow flexible numbers of IMPs
INT32 CountFilledIMPSlots();
INT32 CountEmptyIMPSlots();

extern INT32 iCurrentProfileMode;

//enums for the various Profile modes
enum
{
	IMP__REGISTRY,
	IMP__PERSONALITY,
	IMP__ATTRIBUTES,
	IMP__PORTRAIT,
	IMP__VOICE,
	IMP__FINISH,
};

#endif