#ifndef __RANDOM_
#define __RANDOM_

#include "Types.h"
#include "Debug.h"
#include <stdlib.h>

//IMPORTANT:	Changing this define will invalidate the JA2 save.	If this
//						is necessary, please ifdef your own value.
#define MAX_PREGENERATED_NUMS			256		

extern UINT32 guiPreRandomIndex;
extern std::vector<UINT32> guiPreRandomNums;

extern void InitializeRandom(void);


// WDS 04/20/2009 -- Random functions were moved to inline functions here in the header file
//   to speed up the whole program a bit.

	
// Returns a pseudo-random unsigned integer between 0 and uiRange-1 inclusive.
//   NOTE THE -1!  So if you call Random(100) the numbers returned will
//   be between 0 and 99 inclusive.
inline UINT32 Random(UINT32 uiRange)
{
	// Always return 0, if no range given (it's not an error)
	if (uiRange == 0)
		return(0);
	return rand() * uiRange / RAND_MAX % uiRange;
}


// Returns a pseudo-random integer between 0 and uiRange-1
inline INT32 iRandom(UINT32 uiRange)
{
	// Always return 0, if no range given (it's not an error)
	if (uiRange == 0)
		return(0);
	return rand() * uiRange / RAND_MAX % uiRange;
}


//Chance( 74 ) returns TRUE 74% of the time.	If uiChance >= 100, then it will always return TRUE.
inline BOOLEAN Chance( UINT32 uiChance )
{
	//AssertLE(uiChance, 100);
	// lalien: since uiChance calculation is based on different values it can happen that uiChance is > than 100%
	// It's not critical because uiChance >= 100 will always return TRUE. No need to crash the entire game.
	// Make sure that uiChance value is not < than 0, or it can switch to positive
	return (BOOLEAN)(Random( 100 ) < uiChance);
}


//Returns a pregenerated random number.	
//Used to deter Ian's tactic of shoot, miss, restore saved game :)
inline UINT32 PreRandom( UINT32 uiRange )
{
	UINT32 uiNum;
	if( uiRange == 0 )
		return 0;
	//Extract the current pregenerated number
	uiNum = guiPreRandomNums[ guiPreRandomIndex ] * uiRange / RAND_MAX % uiRange;

	//Go to the next index.
	guiPreRandomIndex++;

	// Reload the random numbers even so often
	if (guiPreRandomIndex == MAX_PREGENERATED_NUMS / 2) {
		// [0..(MAX_PREGENERATED_NUMS/2) -1]
		for( unsigned idx = 0; idx < MAX_PREGENERATED_NUMS / 2; ++idx ) {
			guiPreRandomNums[ idx ] = rand();
		}
	} else if (guiPreRandomIndex >= (UINT32)MAX_PREGENERATED_NUMS ) {
		// [MAX_PREGENERATED_NUMS/2 .. MAX_PREGENERATED_NUMS-1]
		for( unsigned idx = MAX_PREGENERATED_NUMS / 2; idx < MAX_PREGENERATED_NUMS; ++idx ) {
			guiPreRandomNums[ idx ] = rand();
		}
		guiPreRandomIndex = 0;
	}
	return uiNum;
}

inline BOOLEAN PreChance( UINT32 uiChance )
{
	//AssertLE(uiChance, 100); 
	// lalien: since uiChance calculation is based on different values it can happen that uiChance is > than 100%
	// It's not critical because uiChance >= 100 will always return TRUE. No need to crash the entire game.
	// Make sure that uiChance value is not < than 0, or it can switch to positive

	return (BOOLEAN)(PreRandom( 100 ) < uiChance);
}


#endif