/* 
 * bfVFS : vfs/Tools/vfs_profiler.h
 *  - basic profiler class and macros to measure execution time of code blocks
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

#ifndef _VFS_PROFILER_H_
#define _VFS_PROFILER_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs_path.h>
#include <vfs/Tools/vfs_hp_timer.h>

#include <string>
#include <vector>

#define DO_PROFILE 1
#if DO_PROFILE
#	define REGISTERMARKER(id,name)			static vfs::Profiler::tMarkerID id = vfs::Profiler::getProfiler().registerMarker(name) 
#	define STARTMARKER(id)					(vfs::Profiler::getProfiler().startMarker(id))
#	define STOPMARKER(id,success)			(vfs::Profiler::getProfiler().stopMarker(id,success))

#	define DUMPPROFILERSTATSTOFILE(file)	vfs::Profiler::getProfiler().printProfilerState(file)
#else
#	define REGISTERMARKER(id,name)
#	define STARTMARKER(id)
#	define STOPMARKER(id,success)

#	define DUMPPROFILERSTATSTOFILE(file)
#endif

namespace vfs
{
	class VFS_API Profiler
	{
	public:
		typedef unsigned int tMarkerID;
		static Profiler& getProfiler();

		void clear();

		tMarkerID registerMarker(const char *marker);

		void startMarker(tMarkerID id);
		void stopMarker(tMarkerID id, bool success);

		bool printProfilerState(vfs::Path const& file);
	private:
		Profiler();

		struct MARKER
		{
			MARKER() : call_count(0), success_count(0), fail_count(0), time(0.0) {};
			std::string markername;
			unsigned long call_count;
			unsigned long success_count;
			unsigned long fail_count;
			long double time;
			HPTimer timer;
		};
		std::vector<MARKER> m_vMarker;
		unsigned int _nextMarker;
	};

	class VFS_API ProfileMarker
	{
	public:
		ProfileMarker(Profiler::tMarkerID id, bool default_exit=true)
			: _id(id), _exit_success(default_exit)
		{
			STARTMARKER(_id);
		}
		~ProfileMarker()
		{
			STOPMARKER(_id,_exit_success);
		}
		void exit(bool success)
		{
			_exit_success = success;
		}
	private:
		Profiler::tMarkerID _id;
		bool _exit_success;
	};

	VFS_API void DumpProfileState(vfs::Path const& path);

} // namespace vfs

#endif // _VFS_PROFILER_H_
