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

// WANNE NEW
INT32 GetFreeIMPSlot(INT32 iIMPId, INT32 iDefaultIMPId);

// WDS: Allow flexible numbers of IMPs of each sex
INT32 CountFilledIMPSlots( INT8 iSex );
INT32 CountEmptyIMPSlots( INT8 iSex );

// Return a count of how many IMP slots there are
INT32 CountIMPSlots();
INT32 CountMaleIMPSlots();
INT32 CountFemaleIMPSlots();

// Get the first/last slots
INT32 GetFirstSlot(INT8 iSex);
INT32 GetLastSlot(INT8 iSex);
INT32 GetFirstFreeSlot(INT8 iSex);
INT32 GetFirstMaleSlot();
INT32 GetLastMaleSlot();
INT32 GetFirstFemaleSlot();
INT32 GetLastFemaleSlot();

// This returns a count of how any slots along we are in the potential
// slot list.  Usable for displaying on the "Voice" button in the interface
// and probably not much else.
INT32 GetVoiceCountFromVoiceSlot(INT32 iSlot);

// Given an IMP slot # (0 to NUM_PROFILES-1) return MALE or FEMALE
INT32 GetSexOfIMP(INT32 iIMPId);

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