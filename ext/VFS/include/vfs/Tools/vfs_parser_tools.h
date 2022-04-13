/* 
 * bfVFS : vfs/Tools/vfs_parser_tools.h
 *  - read file line-wise,
 *  - split string into tokens,
 *  - simple pattern matching
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

#ifndef _VFS_PARSER_TOOLS_H_
#define _VFS_PARSER_TOOLS_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <list>

namespace vfs
{
	class VFS_API CReadLine
	{
		static const vfs::size_t BUFFER_SIZE = 1024;
	public:
		CReadLine(vfs::tReadableFile& rFile, bool autoControlFile = true);
		~CReadLine();

		bool fillBuffer(bool refill = false);
		bool fromBuffer(std::string& line);
		bool getLine(std::string& line);
	private:
		vfs::Byte			_buffer[BUFFER_SIZE+1];
		vfs::tReadableFile& _file;
		vfs::size_t			_bytes_left;
		vfs::size_t			_buffer_pos;
		vfs::size_t			_buffer_last;
		bool				_auto_ctrl_file;  // if false the object user checks for EOF and closes file

		void operator=(CReadLine const& rl);
	};

	/**************************************************************/
	/**************************************************************/

	class VFS_API CTokenizer
	{
	public:
		CTokenizer(vfs::String const& str);
		~CTokenizer();

		bool next(vfs::String& token, vfs::String::char_t delimeter = L',');
	private:
		const vfs::String	m_list;
		vfs::String::size_t	m_current, m_next;

		void operator=(CTokenizer const& str);
	};

	VFS_API bool matchPattern(vfs::String const& sPattern, vfs::String const& sStr);
	VFS_API bool matchPattern(vfs::String const& sPattern, vfs::String::str_t const& sStr);

} // namespace vfs

#endif // _VFS_PARSER_TOOLS_H_

