#ifndef __IMP_CHARACTER_TRAIT__H_
#define __IMP_CHARACTER_TRAIT__H_

#include "Types.h"

void EnterIMPCharacterTrait( void );
void RenderIMPCharacterTrait( void );
void ExitIMPCharacterTrait( void );
void HandleIMPCharacterTrait( void );

//enum
//{
typedef enum
{
	IMP_CHARACTER_TRAIT_NORMAL = 0,
	IMP_CHARACTER_TRAIT_FRIENDLY,
	IMP_CHARACTER_TRAIT_LONER,
	IMP_CHARACTER_TRAIT_OPTIMIST,
	IMP_CHARACTER_TRAIT_PESSIMIST,
	IMP_CHARACTER_TRAIT_AGGRESSIVE,
	IMP_CHARACTER_TRAIT_ARROGANT,
	IMP_CHARACTER_TRAIT_BIGSHOT,
	IMP_CHARACTER_TRAIT_ASSHOLE,
	IMP_CHARACTER_TRAIT_COWARD,
	IMP_CHARACTER_TRAITS_NUMBER,
} ;

INT8 iChosenCharacterTrait();

STR16 gzIMPCharacterTraitText[];

#endif
