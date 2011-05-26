/* 
 * bfVFS : vfs/Tools/vfs_hp_timer.h
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

#ifndef _VFS_HP_TIMER_
#define _VFS_HP_TIMER_

#include <vfs/vfs_config.h>

#ifdef WIN32
	#include <Windows.h>
#elif __linux__
  #include <sys/time.h>
#endif

namespace vfs
{
	class VFS_API HPTimer
	{
	public:
		HPTimer();
		~HPTimer();

		void		startTimer();
		void		stopTimer();
		
		long long	ticks();
		double		running();

		double		getElapsedTimeInSeconds();
	protected:
		bool			is_running;
	#ifdef WIN32
		LARGE_INTEGER	ticksPerSecond;
		LARGE_INTEGER	tick,tick2;
	#elif __linux__
		timeval			t1,t2;
	#endif
	};
}

#endif // _VFS_HP_TIMER_
