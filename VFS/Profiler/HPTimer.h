#ifndef _HPTIMER_
#define _HPTIMER_

#ifdef WIN32
	#include <Windows.h>
#elif __linux__
  #include <sys/time.h>
#endif

class CHPTimer
{
public:
	CHPTimer();
	~CHPTimer();

	void startTimer();
	void stopTimer();

	double getElapsedTimeInSeconds();
protected:
#ifdef WIN32
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER tick,tick2;
#elif __linux__
	timeval t1,t2;
#endif
};

#endif // _HPTIMER_
