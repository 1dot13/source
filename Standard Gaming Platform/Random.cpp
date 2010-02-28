#include "Random.h"
#include <time.h>
#include "debug control.h"

extern bool is_client;
extern bool is_server;
extern bool is_networked;

bool gfMPDebugOutputRandoms = false;

// OJW - 091024 - MP random syncing
// need to syncronise the randomness of events in a multiplayer game on all clients
UINT32 MPPreRandom( UINT32 uiRange )
{
	UINT32 uiNum;
	if( uiRange == 0 )
		return 0;
	//Extract the current pregenerated number
	uiNum = guiPreRandomNums[ guiPreRandomIndex ] * uiRange / RAND_MAX % uiRange;

	if (gfMPDebugOutputRandoms)
	{
		char tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"MPPreRandom ( uiRange : %i , uiPreRandomIndex : %i , uiResult : %i  )\n",uiRange, guiPreRandomIndex , uiNum );
		MPDebugMsg(tmpMPDbgString);
	}

	//Go to the next index.
	guiPreRandomIndex++;

	if (guiPreRandomIndex >= MAX_PREGENERATED_NUMS)
		guiPreRandomIndex = 0;

	return uiNum;
}

#ifdef BMP_RANDOM//dnl ch55 111009

#include <windows.h>

UINT32 guiPreRandomIndex;
UINT32 guiPreRandomNums[MAX_PREGENERATED_NUMS];

UINT32 GetRndNum(UINT32 maxnum)
{
	if (is_networked && is_client)
		return MPPreRandom(maxnum);

	static UINT32 rnd=0, cnt=0;
	POINT pt;

	if(!(cnt++%RAND_MAX))
	{
		GetCursorPos(&pt);// Get cursor location
		srand(maxnum ^ rnd ^ pt.x ^ pt.y ^ GetTickCount());
//SendFmtMsg("Random Number Generator Reinitialized.");
//for(int l=0;l<100;l++)SendFmtMsg("%2d", Random(100));
	}
	if(maxnum == 0)
		return(0);
	rnd = rand();
	rnd <<= 11;
	rnd ^= rand();
	rnd <<= 7;
	rnd ^= rand();
	return(rnd % maxnum);
}

void InitializeRandom(void)
{
	// Pregenerate all of the random numbers.
	for(guiPreRandomIndex = 0; guiPreRandomIndex<MAX_PREGENERATED_NUMS; guiPreRandomIndex++)
		guiPreRandomNums[guiPreRandomIndex] = GetRndNum(0xFFFFFFFF);
	guiPreRandomIndex = 0;
}

#else

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

#endif