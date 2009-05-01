#include "Random.h"
#include <time.h>

UINT32 guiPreRandomIndex = 0;
std::vector<UINT32> guiPreRandomNums(MAX_PREGENERATED_NUMS, 0);

void InitializeRandom()
{
	// Seed the random-number generator with current time so that
	// the numbers will be different every time we run.
	srand( (unsigned) time(NULL) );

	//Pregenerate all of the random numbers.
	for( guiPreRandomIndex = 0; guiPreRandomIndex < MAX_PREGENERATED_NUMS; guiPreRandomIndex++ )
	{
		guiPreRandomNums[ guiPreRandomIndex ] = rand();
	}
	guiPreRandomIndex = 0;
}
