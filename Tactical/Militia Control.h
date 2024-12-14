#ifndef __MILITIA_CONTROL_H
#define __MILITIA_CONTROL_H

#include "Types.h"
#include "Soldier Control.h"
#include "mousesystem.h"
#include "Strategic Movement.h"

void PrepareMilitiaForTactical(BOOLEAN fPrepareAll);
void RemoveMilitiaFromTactical();
void ResetMilitia();
void HandleMilitiaPromotions();

extern BOOLEAN gfStrategicMilitiaChangesMade;

extern INT32 ghMilitiaControlBox;
//extern INT32 ghTalkToAllBox;

extern BOOLEAN fFirstClickInMilitiaControlScreenMask;

// figure out the assignment menu pop up box positions
void DetermineMilitiaControlBoxPositions( void );

// set x,y position in tactical 
void SetTacticalPopUpMilitiaControlBoxXY( SOLDIERTYPE *pSoldier );

BOOLEAN CreateDestroyMilitiaControlPopUpBoxes( void );

BOOLEAN CheckIfRadioIsEquipped( void );

#endif
