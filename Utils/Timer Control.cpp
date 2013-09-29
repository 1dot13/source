#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "interface control.h"
#else
	#include <windows.h>
	#include <mmsystem.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "Timer Control.h"
	#include "overhead.h"
	#include "handle items.h"
	#include "worlddef.h"
	#include "renderworld.h"
	#include "interface control.h"
	#include "keymap.h"

#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include "Soldier Control.h"
#include "connect.h"

// Base resolution of callback timer
static INT32 BASETIMESLICE = 10;
const INT32 FASTFORWARDTIMESLICE = 1000;
const UINT32 FREQUENCY_CONST = 1000000;
static INT32 MIN_NOTIFY_TIME = 16000;
static INT32 UPDATETIMESLICE = 10000;


INT32	giClockTimer = -1;
INT32	giTimerDiag = 0;

UINT32	guiBaseJA2Clock = 0;
UINT32	guiBaseJA2NoPauseClock = 0;

BOOLEAN	gfPauseClock = FALSE;

BOOLEAN  gfHispeedClockMode = FALSE;

const inline UINT32 TIME_US_TO_MS(UINT32 value) { return value / 1000; }
const inline UINT32 TIME_MS_TO_US(UINT32 value) { return value * 1000; }

UINT32   giFastForwardPeriod = FASTFORWARDTIMESLICE;
BOOLEAN giFastForwardMode = FALSE;
INT32   giFastForwardKey = 0;
UINT32  guiTimeSlice = 0;
FLOAT gfClockSpeedPercent = 1.0;
LARGE_INTEGER gliPerfFreq = {0};
LARGE_INTEGER gliPerfCount = {0};
LARGE_INTEGER gliPerfCountNext = {0};


INT32		giTimerIntervals[ NUMTIMERS ] =
{
	5,					// Tactical Overhead
	20,					// NEXTSCROLL
	200,				// Start Scroll
	200,				// Animate tiles
	1000,				// FPS Counter
	80,					// PATH FIND COUNTER
	150,				// CURSOR TIMER
	250,				// RIGHT CLICK FOR MENU
	300,				// LEFT
	30,					// SLIDING TEXT
	200,				// TARGET REFINE TIMER
	150,					// CURSOR/AP FLASH
	60,					// FADE MERCS OUT
	160,				// PANEL SLIDE
	1000,				// CLOCK UPDATE DELAY
	20,					// PHYSICS UPDATE
	100,				// FADE ENEMYS
	20,					// STRATEGIC OVERHEAD
	40,
	500,				// NON GUN TARGET REFINE TIMER
	250,				// IMPROVED CURSOR FLASH
	500,				// 2nd CURSOR FLASH
	400,					// RADARMAP BLINK AND OVERHEAD MAP BLINK SHOUDL BE THE SAME
	400,
	10,					// Music Overhead
	100,				// Rubber band start delay
};

// TIMER COUNTERS
INT32		giTimerCounters[ NUMTIMERS ];

INT32		giTimerAirRaidQuote				= 0;
INT32		giTimerAirRaidDiveStarted = 0;
INT32		giTimerAirRaidUpdate			= 0;
INT32		giTimerCustomizable				= 0;
INT32		giTimerTeamTurnUpdate			= 0;

CUSTOMIZABLE_TIMER_CALLBACK gpCustomizableTimerCallback = NULL;

// Clock Callback event ID
MMRESULT	gTimerID;

TIMECAPS	gtc;

HANDLE		ghClockThread;
DWORD		gdwClockThreadId;
HANDLE		ghClockThreadShutdown;

HANDLE		ghNotifyThread;
DWORD		gdwNotifyThreadId;
HANDLE		ghNotifyThreadEvent;
HANDLE		ghNotifyThreadShutdownComplete;

// GLOBALS FOR CALLBACK
UINT32				gCNT;
SOLDIERTYPE		*gPSOLDIER;

// GLobal for displaying time diff ( DIAG )
UINT32		guiClockDiff = 0;
UINT32		guiClockStart = 0;


extern UINT32 guiCompressionStringBaseTime;
extern INT32 giFlashHighlightedItemBaseTime;
//extern INT32 giCompatibleItemBaseTime;//Moa:removed (see HandleMouseInCompatableItemForMapSectorInventory)
extern INT32 giAnimateRouteBaseTime;
extern INT32 giPotHeliPathBaseTime;
extern INT32 giClickHeliIconBaseTime;
extern INT32 giExitToTactBaseTime;
extern UINT32 guiSectorLocatorBaseTime;
extern INT32 giCommonGlowBaseTime;
extern INT32 giFlashAssignBaseTime;
extern INT32 giFlashContractBaseTime;
extern UINT32 guiFlashCursorBaseTime;
extern INT32 giPotCharPathBaseTime;


typedef void (*TIMER_NOTIFY_CALLBACK) ( INT32 timer, PTR state );
struct TIMER_NOTIFY_ITEM
{
	TIMER_NOTIFY_CALLBACK callback;
	PTR state;
};
typedef std::list<TIMER_NOTIFY_ITEM> TIMER_NOTIFY_ITEM_LIST;
typedef TIMER_NOTIFY_ITEM_LIST::iterator TIMER_NOTIFY_ITEM_ITERATOR;
static TIMER_NOTIFY_ITEM_LIST glNotifyCallbacks;
static CRITICAL_SECTION gcsNotifyLock;

static bool HasTimerNotifyCallbacks( );
static void BroadcastTimerNotify(INT32 );
static BOOLEAN UpdateTimeCounter( INT32 &counter, INT32 &iTimeLeft );
static BOOLEAN UpdateCounter( INT32 counter, INT32 &iTimeLeft);
static void UpdateTimer();


UINT32 InitializeJA2TimerCallback( UINT32 uiDelay, LPTIMECALLBACK TimerProc, UINT32 uiUser );

// CALLBACKS
void CALLBACK FlashItem( UINT uiID, UINT uiMsg, DWORD uiUser, DWORD uiDw1, DWORD uiDw2 );


void CALLBACK TimeProc( UINT uID,	UINT uMsg, DWORD dwUser, DWORD dw1,	DWORD dw2	)
{
	static BOOLEAN fInFunction = FALSE;
	//SOLDIERTYPE		*pSoldier;

	if ( !fInFunction )
	{
		fInFunction = TRUE;

		BOOLEAN timerDone = FALSE;
		BOOLEAN tickTime = FALSE;
		INT32 iTimeLeft = 0;

		// Use QPC to check if BASETIMESLICE (in ms) has passed
		if (IsHiSpeedClockMode())
		{
			// Only advance time when sufficient time has passed to exceed next time
			QueryPerformanceCounter(&gliPerfCount);
			if (gliPerfCount.QuadPart > gliPerfCountNext.QuadPart)
			{
				INT32 iNext = IsFastForwardMode() ? giFastForwardPeriod : UPDATETIMESLICE;
				gliPerfCountNext.QuadPart = gliPerfCount.QuadPart + (iNext * gliPerfFreq.QuadPart) / FREQUENCY_CONST;
				iTimeLeft = iNext;
				timerDone = IsFastForwardMode();
				tickTime = TRUE;
			}
		}
		else
		{
			// When using millisecond timer, advance time everytime this function is called
			tickTime = TRUE;
			timerDone = !IsFastForwardMode();
		}
		if (tickTime)
		{
			guiBaseJA2NoPauseClock += BASETIMESLICE;

			if ( !gfPauseClock )
			{
				guiBaseJA2Clock += BASETIMESLICE;

				for ( gCNT = 0; gCNT < NUMTIMERS; gCNT++ )
				{
					timerDone |= UpdateCounter( gCNT, iTimeLeft  );
				}

				// Update some specialized countdown timers...
				timerDone |= UpdateTimeCounter( giTimerAirRaidQuote, iTimeLeft );
				timerDone |= UpdateTimeCounter( giTimerAirRaidDiveStarted, iTimeLeft );
				timerDone |= UpdateTimeCounter( giTimerAirRaidUpdate, iTimeLeft );
				timerDone |= UpdateTimeCounter( giTimerTeamTurnUpdate, iTimeLeft );

				if ( gpCustomizableTimerCallback )
				{
					timerDone |= UpdateTimeCounter( giTimerCustomizable, iTimeLeft );
				}

#ifndef BOUNDS_CHECKER

				// If mapscreen...
				if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
				{
					// IN Mapscreen, loop through player's team.....
					for ( gCNT = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; gCNT <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; gCNT++ )
					{
						gPSOLDIER = MercPtrs[ gCNT ];
						timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.PortraitFlashCounter, iTimeLeft );
						timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.PanelAnimateCounter, iTimeLeft );
					}
				}
				else
				{
					// Set update flags for soldiers
					////////////////////////////
					for ( gCNT = 0; gCNT < guiNumMercSlots; gCNT++ )
					{
						gPSOLDIER = MercSlots[ gCNT ];

						if ( gPSOLDIER != NULL )
						{
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.UpdateCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.DamageCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.ReloadCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.FlashSelCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.BlinkSelCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.PortraitFlashCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.AICounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.FadeCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.NextTileCounter, iTimeLeft );
							timerDone |= UpdateTimeCounter( gPSOLDIER->timeCounters.PanelAnimateCounter, iTimeLeft );
#ifdef JA2UB
							timerDone |= UpdateTimeCounter( gPSOLDIER->GetupFromJA25StartCounter, iTimeLeft );
#endif
						}
					}
				}
#endif
			}
		}

		if (timerDone)
			SetEvent(ghNotifyThreadEvent);
		fInFunction = FALSE;
	}
}



static UINT32 MIN_TIMER(UINT32 timer, UINT32 other)
{
	UINT32 value = TIME_MS_TO_US(timer);
	return ( value && value < other ? value : other );
}

// Returns the smallest time interval for a counter currently in use
UINT32 GetNextCounterDoneTime(void)
{
	QueryPerformanceCounter(&gliPerfCount);
	INT32 time = (INT32)(((gliPerfCountNext.QuadPart - gliPerfCount.QuadPart) * FREQUENCY_CONST) / gliPerfFreq.QuadPart);
	return (UINT32)((time > 0) ? time : 0);
}

// Function to test if there are any outstanding timers.  Used in fast forward routines
BOOLEAN IsTimerActive(void)
{
	return GetNextCounterDoneTime() <= FASTFORWARDTIMESLICE ? TRUE : FALSE;
}

DWORD WINAPI JA2ClockThread( LPVOID lpParam ) 
{
	__try
	{
		for(;;) 
		{
			TimeProc(0, 0, 0, 0, 0);

			DWORD dwResult = WaitForSingleObject(ghClockThreadShutdown, 0);
			if (dwResult == WAIT_OBJECT_0 || dwResult == WAIT_ABANDONED)
				break;
			YieldProcessor();

			// Sleep for a couple of milliseconds if not in fast forward mode
			if (!IsFastForwardMode())
				Sleep( TIME_US_TO_MS( GetNextCounterDoneTime() ) );
		} 
	}
	__except( EXCEPTION_EXECUTE_HANDLER  )
	{
		// Unhandled exception just exit
	}
	return 0L;
}

DWORD WINAPI JA2NotifyThread( LPVOID lpParam ) 
{
	HANDLE waitHandles[] = {ghClockThreadShutdown, ghNotifyThreadEvent};
	for(;;) 
	{
		DWORD dwResult = WaitForSingleObject(ghClockThreadShutdown, 0);
		if (dwResult == WAIT_OBJECT_0 || dwResult == WAIT_ABANDONED)
			break;

		DWORD waitTime = (!IsFastForwardMode()) ? max(TIME_US_TO_MS(MIN_NOTIFY_TIME), TIME_US_TO_MS( GetNextCounterDoneTime() ) ) : 0;
		dwResult = WaitForMultipleObjectsEx(_countof(waitHandles), waitHandles, FALSE, waitTime, FALSE);
		if (dwResult == WAIT_OBJECT_0)
			break;
		if (dwResult >= WAIT_ABANDONED_0 && dwResult <= (WAIT_ABANDONED_0 + _countof(waitHandles)))
			break;
		if ( dwResult == WAIT_FAILED || dwResult == WAIT_TIMEOUT || (dwResult-WAIT_OBJECT_0) == 1)
		{
			if (HasTimerNotifyCallbacks())
			{
				BroadcastTimerNotify(-1);
			}
		}
		else
		{
			// unexpected failure
		}
	} 
	SetEvent(ghNotifyThreadShutdownComplete);
	return 0L;
}



BOOLEAN InitializeJA2Clock()
{
#ifdef CALLBACKTIMER
	MMRESULT	mmResult;
	INT32			cnt;

	// Init timer delays
	for ( cnt = 0; cnt < NUMTIMERS; cnt++ )
	{
		giTimerCounters[ cnt ] = giTimerIntervals[ cnt ];
	}


	// First get timer resolutions
	mmResult = timeGetDevCaps( &gtc, sizeof( gtc ) );

	if ( mmResult != TIMERR_NOERROR )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not get timer properties");
	}

	if ( !QueryPerformanceFrequency(&gliPerfFreq) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not get performance frequency");
	}
	if ( !QueryPerformanceCounter(&gliPerfCount) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not get performance frequency");
	}

	timeBeginPeriod(gtc.wPeriodMin);

	InitializeCriticalSection(&gcsNotifyLock);

	if (IsHiSpeedClockMode())
	{
		ghClockThreadShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
		ghNotifyThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		ghNotifyThreadShutdownComplete = CreateEvent(NULL, TRUE, FALSE, NULL);
		ghClockThread = CreateThread( 
			NULL,              // default security attributes
			0,                 // use default stack size  
			JA2ClockThread,    // thread function 
			NULL,              // argument to thread function 
			0,                 // use default creation flags 
			&gdwClockThreadId);// returns the thread identifier 
		ghNotifyThread = CreateThread( 
			NULL,              // default security attributes
			0,                 // use default stack size  
			JA2NotifyThread,    // thread function 
			NULL,              // argument to thread function 
			0,                 // use default creation flags 
			&gdwNotifyThreadId);// returns the thread identifier 
	}
	else
	{
		UpdateTimer();
	}
#endif

	return TRUE;
}


void	ShutdownJA2Clock(void)
{
	if (IsHiSpeedClockMode())
	{
		SetEvent(ghClockThreadShutdown);
		WaitForSingleObject(ghNotifyThreadShutdownComplete, 2000);
		HANDLE waitHandles[] = {ghClockThread, ghNotifyThread};
		WaitForMultipleObjects(_countof(waitHandles), waitHandles, TRUE, 1000);
		CloseHandle(ghClockThreadShutdown);
		CloseHandle(ghClockThread);
		CloseHandle(ghNotifyThreadEvent);
		CloseHandle(ghNotifyThread);
		// During ungraceful shutdowns notify lock may be in use in notify thread
		if (TryEnterCriticalSection(&gcsNotifyLock))
		{
			LeaveCriticalSection(&gcsNotifyLock);
			DeleteCriticalSection(&gcsNotifyLock);
		}
	}
	else
	{
		// Make sure we kill the timer
#ifdef CALLBACKTIMER
		timeKillEvent( gTimerID );
#endif
	}

	timeEndPeriod(gtc.wPeriodMin);
}


UINT32 InitializeJA2TimerCallback( UINT32 uiDelay, LPTIMECALLBACK TimerProc, UINT32 uiUser )
{
	MMRESULT	mmResult;
	MMRESULT	TimerID;


	// First get timer resolutions
	mmResult = timeGetDevCaps( &gtc, sizeof( gtc ) );

	if ( mmResult != TIMERR_NOERROR )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not get timer properties");
	}

	// Set timer at lowest resolution. Could use middle of lowest/highest, we'll see how this performs first
	TimerID = timeSetEvent( (UINT)uiDelay, (UINT)uiDelay, TimerProc, (DWORD)uiUser, TIME_PERIODIC );

	if ( !TimerID )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not create timer callback");
	}

	return ( (UINT32)TimerID );
}

void RemoveJA2TimerCallback( UINT32 uiTimer )
{
	timeKillEvent( uiTimer );
}


UINT32 InitializeJA2TimerID( UINT32 uiDelay, UINT32 uiCallbackID, UINT32 uiUser )
{
	switch( uiCallbackID )
	{
	case ITEM_LOCATOR_CALLBACK:

		return( InitializeJA2TimerCallback( uiDelay, FlashItem, uiUser ) );
		break;

	}

	// invalid callback id
	Assert( FALSE );
	return( 0 );
}


//////////////////////////////////////////////////////////////////////////////////////////////
// TIMER CALLBACK S
//////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK FlashItem( UINT uiID, UINT uiMsg, DWORD uiUser, DWORD uiDw1, DWORD uiDw2 )
{

}


void PauseTime( BOOLEAN fPaused )
{
	gfPauseClock = fPaused;
}

void SetCustomizableTimerCallbackAndDelay( INT32 iDelay, CUSTOMIZABLE_TIMER_CALLBACK pCallback, BOOLEAN fReplace )
{
	if ( gpCustomizableTimerCallback )
	{
		if ( !fReplace )
		{
			// replace callback but call the current callback first
			gpCustomizableTimerCallback();
		}
	}

	RESETTIMECOUNTER( giTimerCustomizable, iDelay );
	gpCustomizableTimerCallback = pCallback;
}

void CheckCustomizableTimer( void )
{
	if ( gpCustomizableTimerCallback )
	{
		if ( TIMECOUNTERDONE( giTimerCustomizable, 0 ) )
		{
			// set the callback to a temp variable so we can reset the global variable
			// before calling the callback, so that if the callback sets up another
			// instance of the timer, we don't reset it afterwards
			CUSTOMIZABLE_TIMER_CALLBACK pTempCallback;

			pTempCallback = gpCustomizableTimerCallback;
			gpCustomizableTimerCallback = NULL;
			pTempCallback();
		}
	}
}



void ResetJA2ClockGlobalTimers( void )
{
	UINT32 uiCurrentTime = GetJA2Clock();


	guiCompressionStringBaseTime = uiCurrentTime;
	giFlashHighlightedItemBaseTime = uiCurrentTime;
	//giCompatibleItemBaseTime = uiCurrentTime;//Moa: removed (see HandleMouseInCompatableItemForMapSectorInventory)
	giAnimateRouteBaseTime = uiCurrentTime;
	giPotHeliPathBaseTime = uiCurrentTime;
	giClickHeliIconBaseTime = uiCurrentTime;
	giExitToTactBaseTime = uiCurrentTime;
	guiSectorLocatorBaseTime = uiCurrentTime;

	giCommonGlowBaseTime = uiCurrentTime;
	giFlashAssignBaseTime = uiCurrentTime;
	giFlashContractBaseTime = uiCurrentTime;
	guiFlashCursorBaseTime = uiCurrentTime;
	giPotCharPathBaseTime = uiCurrentTime;
}

void SetTileAnimCounter( INT32 iTime )
{
	giTimerIntervals[ ANIMATETILES ] = iTime;
}

void SetFastForwardPeriod(DOUBLE value)
{
	giFastForwardPeriod = (UINT32)(value);
	if (giFastForwardPeriod <= 1)
		giFastForwardPeriod = 1;
}

void SetFastForwardKey(INT32 key)
{
	giFastForwardKey = key;
}

BOOLEAN IsFastForwardKeyPressed()
{
	// WANNE: In a multiplayer game it is not allowed for the "pure" client to do fast forward
	// Only the server is allowed to do, because the AI is generated on the server
	if (is_networked)
	{
		if (!is_server)				// It is not allowed when we are not the server
			return false;
		else if (gTacticalStatus.ubCurrentTeam != 1)	// It is not allowed, when it is not the enemy turn!
			return false;
	}

	return giFastForwardKey && IsKeyPressed(giFastForwardKey);
}

void SetFastForwardMode(BOOLEAN enable)
{
	giFastForwardMode = enable;
	UpdateTimer();
}

BOOLEAN IsFastForwardMode()
{
	return giFastForwardMode || IsFastForwardKeyPressed();
}

LONGLONG GetJA2Microseconds()
{
	return gliPerfCount.QuadPart * FREQUENCY_CONST / gliPerfFreq.QuadPart;
}

void AddTimerNotifyCallback( TIMER_NOTIFY_CALLBACK callback, PTR state )
{
	EnterCriticalSection(&gcsNotifyLock);
	BOOL addItem = TRUE;
	for (TIMER_NOTIFY_ITEM_ITERATOR itr = glNotifyCallbacks.begin(); itr != glNotifyCallbacks.end(); ++itr) {
		if ( callback == (*itr).callback && state == (*itr).state ){
			addItem = FALSE;
			break;
		}
	}
	if (addItem)
	{
		TIMER_NOTIFY_ITEM item;
		item.callback = callback;
		item.state = state;
		glNotifyCallbacks.push_back(item);
	}
	LeaveCriticalSection(&gcsNotifyLock);
}

void RemoveTimerNotifyCallback( TIMER_NOTIFY_CALLBACK callback, PTR state )
{
	EnterCriticalSection(&gcsNotifyLock);
	for ( TIMER_NOTIFY_ITEM_ITERATOR itr = glNotifyCallbacks.begin(); itr != glNotifyCallbacks.end(); ) 
	{
		if ( callback == (*itr).callback && state == (*itr).state )
			itr = glNotifyCallbacks.erase(itr);
		else
			++itr;
	}
	LeaveCriticalSection(&gcsNotifyLock);
}

void ClearTimerNotifyCallbacks()
{
	// If we cannot get the lock it is likely due to exception while handling notification and we are shutting down
	if ( TryEnterCriticalSection(&gcsNotifyLock) )
	{
		glNotifyCallbacks.clear();
		LeaveCriticalSection(&gcsNotifyLock);
	}
}

static bool HasTimerNotifyCallbacks( )
{
	return !glNotifyCallbacks.empty();
}


// Call timer notify routine
//   Separate the callback notifies with normal try/catch
//   as SEH __try/__except are incompatible with C++ exceptions
static void InnerTimerNotify(INT32 timer)
{
   try
   {
	  for (TIMER_NOTIFY_ITEM_ITERATOR itr = glNotifyCallbacks.begin(); itr != glNotifyCallbacks.end(); ++itr)
	  {
		 if ( NULL != (*itr).callback)
			(*itr).callback( timer, (*itr).state );
	  }
   }
   catch (...)   {}
}
static void BroadcastTimerNotify(INT32 timer)
{
	EnterCriticalSection(&gcsNotifyLock);
	__try
	{
		__try { InnerTimerNotify(timer); }
		__except( EXCEPTION_EXECUTE_HANDLER  )
		{ /*  Not sure.  exit? */ }
	}
	__finally
	{
		LeaveCriticalSection(&gcsNotifyLock);
	}
}

BOOLEAN UpdateTimeCounter( INT32 &counter, INT32 &iTimeLeft)
{
	if (counter == 0) {
		return FALSE;
	} else if ( ( counter - BASETIMESLICE ) < 0 ) {
		counter = 0;
		return TRUE;
	} else {
		counter -= BASETIMESLICE;
		if ( counter < iTimeLeft )
			iTimeLeft = counter;
		return FALSE;
	}
	return FALSE;
}

BOOLEAN UpdateCounter( INT32 counterIdx, INT32 &iTimeLeft )
{
	INT32& counter = giTimerCounters[ counterIdx ];
	return UpdateTimeCounter(counter, iTimeLeft);
}

BOOLEAN UpdateCounter( INT32 counterIdx )
{
	INT32 iDummy = 0;
	return UpdateCounter(counterIdx, iDummy);
}

void ResetCounter(INT32 counterIdx)
{
	giTimerCounters[ counterIdx ] = giTimerIntervals[ counterIdx ];
}

BOOLEAN CounterDone(INT32 counterIdx)
{
	return ( giTimerCounters[ counterIdx ] == 0 ) ? TRUE : FALSE;
}

void ResetTimerCounter(INT32 &timer, INT32 value)
{
	timer = value;
}

BOOLEAN TimeCounterDone(INT32 timer)
{
	return ( timer == 0 ) ? TRUE : FALSE;
}

void ZeroTimeCounter(INT32& timer)
{
	timer = 0;
}

BOOLEAN IsJA2TimerThread()
{
	return (GetCurrentThreadId() == gdwClockThreadId);
}

#ifndef GetJA2Clock
UINT32	GetJA2Clock()
{
	return guiBaseJA2Clock;
}
#endif

#ifndef GetJA2NoPauseClock
UINT32	GetJA2NoPauseClock()
{
	return guiBaseJA2NoPauseClock;
}
#endif

void SetHiSpeedClockMode(BOOLEAN enable)
{
	gfHispeedClockMode = enable;
}

BOOLEAN IsHiSpeedClockMode()
{
	return gfHispeedClockMode;
}

void SetNotifyFrequencyKey(INT32 value)
{
	MIN_NOTIFY_TIME = value;
}

void SetClockSpeedPercent(FLOAT value)
{
	gfClockSpeedPercent = value;
	UPDATETIMESLICE = (UINT32)((FLOAT)TIME_MS_TO_US(BASETIMESLICE) * 100.0f / value);
	UpdateTimer();
}

void UpdateTimer()
{
	// Set timer at lowest resolution. Could use middle of lowest/highest, we'll see how this performs first
	if (!IsHiSpeedClockMode())
	{
		UINT uiTimeSlice = giFastForwardMode ? gtc.wPeriodMin : max(gtc.wPeriodMin, TIME_US_TO_MS(UPDATETIMESLICE));
		if (uiTimeSlice != guiTimeSlice)
		{
			guiTimeSlice = uiTimeSlice;
			if (gTimerID != 0) timeKillEvent(gTimerID);
			gTimerID = timeSetEvent( uiTimeSlice, uiTimeSlice, TimeProc, (DWORD)0, TIME_PERIODIC );
			if ( !gTimerID )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not create timer callback");
			}
		}
	}
}

