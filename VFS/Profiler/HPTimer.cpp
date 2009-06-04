#include "HPTimer.h"

CHPTimer::CHPTimer()
{
#ifdef WIN32
	QueryPerformanceFrequency(&ticksPerSecond);
#endif
}

CHPTimer::~CHPTimer()
{
}

void CHPTimer::StartTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick);
#elif LINUX
	gettimeofday(&t1,0);
#endif

}

void CHPTimer::StopTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick2);
#elif LINUX
	gettimeofday(&t2,0);
#endif
}

double CHPTimer::GetElapsedTimeInSeconds()
{
#ifdef WIN32
	return (double)(tick2.QuadPart - tick.QuadPart)/(double)ticksPerSecond.QuadPart;
#elif LINUX
	return (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
#endif
}
