/* 
 * bfVFS : vfs/Core/vfs_path.h
 *  - Path class, stores and validates a file/directory path string, offers meaningful path operations
 *  - path comparison functions (operator overloading)
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

#ifndef _VFS_PATH_H_
#define _VFS_PATH_H_

#include <vfs/vfs_config.h>
#include <vfs/Core/vfs_types.h>

namespace vfs
{
	class VFS_API Path
	{
	public:
		class VFS_API Less{
		public:
			bool operator()(vfs::Path const& s1, vfs::Path const& s2) const;
		};
		class Equal{
		public:
			bool operator()(vfs::Path const& s1, vfs::Path const& s2) const;
		};
	public:
		Path();
		Path(const char* sPath);
		Path(std::string const& sPath);
		Path(const wchar_t* sPath);
		Path(vfs::String const& sPath);

		const vfs::String::char_t*	c_str() const;
		const vfs::String::str_t&	c_wcs() const;
		const vfs::String::str_t&	operator()() const;	

		std::string			to_string() const;

		Path&				operator+=(Path const& p);
		Path&				operator+=(vfs::String const& p);

		bool				empty() const;

		vfs::String::size_t	length() const;

		void				doCheck();
		bool				expandEnv();

		void				splitLast(vfs::Path &rsHead, vfs::Path &rsLast) const;
		void				splitFirst(vfs::Path &rsFirst, vfs::Path &rsTail) const;

		bool				extension(vfs::String &sExt) const;

		bool				operator==(vfs::Path const& p2);
	private:
		vfs::String::str_t	_path;
		struct SeparatorPosition
		{
			SeparatorPosition() : first(vfs::npos), last(vfs::npos) {}
			vfs::size_t		first, last;
		} _sep;
	};
}

template<>
VFS_API BuildString&	BuildString::add<vfs::Path>(vfs::Path const& value);
template<>
VFS_API BuildString&	BuildString::operator<< <vfs::Path>(vfs::Path const& value);

// add only valid Path objects
VFS_API vfs::Path		operator+(vfs::Path const& p1, vfs::Path const& p2);

VFS_API bool			operator==(vfs::Path const& p1, vfs::Path const& p2);
// compare path with string (that can be an invalid path)
// use with care as these strings can be different from the internal representation although they seem to be equal
VFS_API bool			operator==(vfs::Path const& p1, vfs::String const& p2);
VFS_API bool			operator==(vfs::Path const& p1, vfs::String::str_t const& p2);
VFS_API bool			operator==(vfs::Path const& p1, const vfs::String::char_t* p2);

#endif // _VFS_PATH_H_

