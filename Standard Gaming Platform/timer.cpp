	#include "types.h"
	#include <windows.h>
		#include "video.h"
	#include "timer.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

UINT32 guiStartupTime;
UINT32 guiCurrentTime;

void CALLBACK Clock( HWND hWindow, UINT uMessage, UINT idEvent, DWORD dwTime )
{
	guiCurrentTime = GetTickCount();
	if (guiCurrentTime < guiStartupTime)
	{ // Adjust guiCurrentTime because of loopback on the timer value
	guiCurrentTime = guiCurrentTime + (0xffffffff - guiStartupTime);
	}
	else
	{ // Adjust guiCurrentTime because of loopback on the timer value
	guiCurrentTime = guiCurrentTime - guiStartupTime;
	}
}

BOOLEAN InitializeClockManager(void)
{

	// Register the start time (use WIN95 API call)
	guiCurrentTime = guiStartupTime = GetTickCount();
	SetTimer(ghWindow, MAIN_TIMER_ID, 10, (TIMERPROC)Clock);


	return TRUE;
}

void	ShutdownClockManager(void)
{

	// Make sure we kill the timer
	KillTimer(ghWindow, MAIN_TIMER_ID);

}

TIMER	GetClock(void)
{
	return guiCurrentTime;
}

TIMER	SetCountdownClock(UINT32 uiTimeToElapse)
{
	return (guiCurrentTime + uiTimeToElapse);
}

UINT32 ClockIsTicking(TIMER uiTimer)
{
	if (uiTimer > guiCurrentTime)
	{ // Well timer still hasn't elapsed
	return (uiTimer - guiCurrentTime);
	}
	// Time's up
	return 0;
}
