#ifndef __IMP_DISABILITY_TRAIT__H_
#define __IMP_DISABILITY_TRAIT__H_

#include "Types.h"

void EnterIMPDisabilityTrait( void );
void RenderIMPDisabilityTrait( void );
void ExitIMPDisabilityTrait( void );
void HandleIMPDisabilityTrait( void );

//enum
//{
typedef enum
{
	IMP_NO_DISABILITY = 0,
	IMP_DISABILITY_HEAT_INTOLERANT,
	IMP_DISABILITY_NERVOUS,
	IMP_DISABILITY_CLAUSTROPHOBIC,
	IMP_DISABILITY_NONSWIMMER,
	IMP_DISABILITY_FEAR_OF_INSECTS,
	IMP_DISABILITY_FORGETFUL,
	IMP_DISABILITY_PSYCHO,
	IMP_DISABILITIES_NUMBER,
};

INT8 iChosenDisabilityTrait();
INT8 iPlayersAttributePointsBonusForDisabilitySelected();

STR16 gzIMPDisabilityText[];

#endif
