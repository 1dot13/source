/* 
 * bfVFS : vfs/Tools/vfs_profiler.cpp
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

#define NOMINMAX
#include <vfs/Tools/vfs_profiler.h>

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/File/vfs_file.h>
#include <sstream>

namespace vfs
{
	class CProfileStarter
	{
	public:
		CProfileStarter()
		{
			Profiler::getProfiler();
		}
	};
	static CProfileStarter starter;
}

vfs::Profiler& vfs::Profiler::getProfiler()
{
	static Profiler* _prof = new Profiler;
	return *_prof;
}

vfs::Profiler::Profiler()
{
	m_vMarker.resize(1024);
	_nextMarker = 0;
}

void vfs::Profiler::clear()
{
	for(unsigned int i = 0; i < _nextMarker; ++i)
	{
		m_vMarker[i].markername = "";
		m_vMarker[i].time = 0;
		m_vMarker[i].call_count = 0;
		m_vMarker[i].success_count = 0;
		m_vMarker[i].fail_count = 0;
	}
	_nextMarker = 0;
}


vfs::Profiler::tMarkerID vfs::Profiler::registerMarker(const char *marker)
{
	m_vMarker[_nextMarker].markername = marker;
	return _nextMarker++;
}

void vfs::Profiler::startMarker(tMarkerID id)
{
	m_vMarker[id].timer.startTimer();
}

void vfs::Profiler::stopMarker(tMarkerID id, bool success)
{
	m_vMarker[id].timer.stopTimer();
	m_vMarker[id].time += m_vMarker[id].timer.getElapsedTimeInSeconds();
	m_vMarker[id].call_count++;
	if(success) m_vMarker[id].success_count++;
	else m_vMarker[id].fail_count++;
}

inline std::string multChar(std::string::value_type c, unsigned int multiplicity)
{
	std::string s;
	s.resize(multiplicity);
	for(unsigned int i=0; i<multiplicity; ++i)
	{
		s[i] = c;
	}
	return s;
}

inline long double perCent(unsigned long value, unsigned long ref)
{
	return 100.0 * ((double)(value)/double(ref));
}

inline long double oneDigit(long double number)
{
	unsigned long temp = (unsigned long)(number * 10);
	return (temp / 10.0);
}

bool vfs::Profiler::printProfilerState(vfs::Path const& file)
{
	vfs::CFile oFile(file);
	if(!oFile.openWrite(true,true))
	{
		return false;
	}
	// get largest value
	long double max_time = 0;
	std::string::size_type max_prefix = 0;
	for(unsigned int i=0; i<m_vMarker.size(); ++i)
	{
		if(m_vMarker[i].time > max_time)
		{
			max_time = m_vMarker[i].time;
		}
		std::string::size_type prefix_length = m_vMarker[i].markername.length();
		if(prefix_length > max_prefix)
		{
			max_prefix = prefix_length;
		}
	}
	const unsigned int WIDTH = 40;
	std::stringstream line;
	long double ld_success, ld_failure;
	for(unsigned int i=0; i<m_vMarker.size(); ++i)
	{
		if(m_vMarker[i].markername.empty())
		{
			break;
		}
		if(m_vMarker[i].markername.length() < WIDTH)
		{
			unsigned int space = WIDTH - m_vMarker[i].markername.length();
			line << m_vMarker[i].markername << multChar(' ',space) << " | ";
		}
		else
		{
			line << m_vMarker[i].markername.substr(0,WIDTH) << " | ";
		}
		if(max_time != 0)
		{
			unsigned int num_stars = (unsigned int)(WIDTH * (m_vMarker[i].time / max_time));
			ld_success = perCent(m_vMarker[i].success_count,m_vMarker[i].call_count);
			ld_failure = perCent(m_vMarker[i].fail_count,m_vMarker[i].call_count);
			line << "[" << oneDigit(ld_success)	 << "|" << oneDigit(ld_failure) << "] " 
				 << multChar('*', num_stars) << multChar(' ', WIDTH - num_stars) << " | ";
		}
		line << "C: " << m_vMarker[i].call_count << ", T: " << m_vMarker[i].time << std::endl;
		//line << std::endl;
	}
	std::string useless_copy = line.str();
	oFile.write(useless_copy.c_str(), (vfs::size_t)(useless_copy.length()*sizeof(std::string::value_type)));
	oFile.close();
	return true;
}

void vfs::DumpProfileState(vfs::Path const& path)
{
	vfs::Profiler::getProfiler().printProfilerState(path);
}

