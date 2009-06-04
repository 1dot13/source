#ifndef _HPTIMER_
#define _HPTIMER_

#ifdef WIN32
	#include <Windows.h>
#elif LINUX
  #include <sys/time.h>
#endif

class CHPTimer
{
public:
	CHPTimer();
	~CHPTimer();

	void StartTimer();
	void StopTimer();

	double GetElapsedTimeInSeconds();
protected:
#ifdef WIN32
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER tick,tick2;
#elif LINUX
	timeval t1,t2;
#endif
};

#endif // _HPTIMER_
