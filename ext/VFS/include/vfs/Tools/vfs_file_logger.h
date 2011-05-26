/* 
 * bfVFS : vfs/Tools/vfs_file_logger.h
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

#ifndef _VFS_FILE_LOGGER_H_
#define _VFS_FILE_LOGGER_H_

#include <vfs/Core/vfs_path.h>
#include <vfs/Tools/vfs_log.h>
#include <vfs/Tools/vfs_hp_timer.h>
#include <vfs/Aspects/vfs_logging.h>

namespace vfs
{
	class VFS_API FileLogger : public vfs::Aspects::ILogger
	{
	public:
		FileLogger(vfs::Path const& log_file, bool append = false, vfs::Log::EFlushMode flush_mode = vfs::Log::FLUSH_ON_ENDL);
		FileLogger(vfs::tWritableFile* file, bool append = false, vfs::Log::EFlushMode flush_mode = vfs::Log::FLUSH_ON_ENDL);
		~FileLogger();

		virtual void Msg(const wchar_t* msg);
		virtual void Msg(const char* msg);

		void Msg(vfs::String const& msg);
	private:
		Log&		m_log;
		HPTimer		m_clock;
	};
}

#endif // _VFS_FILE_LOGGER_H_
