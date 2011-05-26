/* 
 * bfVFS : vfs/Tools/vfs_hp_timer.cpp
 *  - high performance/precision timer, used by profiler
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <vfs/Tools/vfs_hp_timer.h>

vfs::HPTimer::HPTimer() : is_running(false)
{
#ifdef WIN32
	QueryPerformanceFrequency(&ticksPerSecond);
#endif
}

vfs::HPTimer::~HPTimer()
{
}

void vfs::HPTimer::startTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick);
#elif __linux__
	gettimeofday(&t1,0);
#endif
	is_running = true;
}

long long vfs::HPTimer::ticks()
{
	if(is_running)
	{
#ifdef WIN32
		QueryPerformanceCounter(&tick2);
		return tick2.QuadPart - tick.QuadPart;
#elif __linux__
		gettimeofday(&t2,0);
		return t2.tv_usec - t1.tv_usec;
#endif
	}
	return 0;
}
double vfs::HPTimer::running()
{
	if(is_running)
	{
#ifdef WIN32
		QueryPerformanceCounter(&tick2);
		return (double)(tick2.QuadPart - tick.QuadPart)/(double)ticksPerSecond.QuadPart;
#elif __linux__
		gettimeofday(&t2,0);
		return (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
#endif
	}
	return 0;
}

void vfs::HPTimer::stopTimer()
{
#ifdef WIN32
	QueryPerformanceCounter(&tick2);
#elif __linux__
	gettimeofday(&t2,0);
#endif
	is_running = false;
}

double vfs::HPTimer::getElapsedTimeInSeconds()
{
#ifdef WIN32
	return (double)(tick2.QuadPart - tick.QuadPart)/(double)ticksPerSecond.QuadPart;
#elif __linux__
	return (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
#endif
}
