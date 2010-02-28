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

void CHPTimer::startTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick);
#elif __linux__
	gettimeofday(&t1,0);
#endif

}

void CHPTimer::stopTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick2);
#elif __linux__
	gettimeofday(&t2,0);
#endif
}

double CHPTimer::getElapsedTimeInSeconds()
{
#ifdef WIN32
	return (double)(tick2.QuadPart - tick.QuadPart)/(double)ticksPerSecond.QuadPart;
#elif __linux__
	return (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
#endif
}
