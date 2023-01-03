/* 
 * bfVFS : vfs/Tools/vfs_file_logger.cpp
 *  - implements Logging iterface in the Aspects module by using the vfs::Log class
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

#include <vfs/Tools/vfs_file_logger.h>

vfs::FileLogger::FileLogger(vfs::Path const& log_file, bool append, vfs::Log::EFlushMode flush_mode)
: m_log( *vfs::Log::create(log_file, append, flush_mode) )
{
	m_log.Reserve();
	m_clock.startTimer();
}

vfs::FileLogger::FileLogger(vfs::tWritableFile* file, bool append, vfs::Log::EFlushMode flush_mode)
: m_log( *vfs::Log::create(file,append, flush_mode) ) 
{
	m_log.Reserve();
	m_clock.startTimer();
}


vfs::FileLogger::~FileLogger()
{
	m_log.destroy();
}

void vfs::FileLogger::Msg(const wchar_t* msg)
{
	m_log << "[" << m_clock.running() << "] : " << vfs::String::as_utf8(msg) << vfs::Log::endl;
}

void vfs::FileLogger::Msg(const char* msg)
{
	m_log << "[" << m_clock.running() << "] : " << msg << vfs::Log::endl;
}

void vfs::FileLogger::Msg(vfs::String const& msg)
{
	this->Msg(msg.c_str());
}
