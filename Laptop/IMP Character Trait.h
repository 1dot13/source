#ifndef __IMP_CHARACTER_TRAIT__H_
#define __IMP_CHARACTER_TRAIT__H_

#include "Types.h"

void EnterIMPCharacterTrait( void );
void RenderIMPCharacterTrait( void );
void ExitIMPCharacterTrait( void );
void HandleIMPCharacterTrait( void );

INT8 iChosenCharacterTrait();

STR16 gzIMPCharacterTraitText[];
STR16 gzIMPAttitudesText[];

#endif
