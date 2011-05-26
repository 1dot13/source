/* 
 * bfVFS : vfs/Tools/vfs_tools.h
 *  - simple from/to string (list) conversion functions, 
 *  - remove leading/trailing whitspace characters in a string
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

#ifndef _VFS_TOOLS_H_
#define _VFS_TOOLS_H_

#include <vfs/vfs_config.h>

#include <vfs/Core/vfs_types.h>
#include <list>

namespace vfs
{
	template<typename CharType, typename ValueType>
	std::basic_string<CharType> toString(ValueType const& rVal) 
	{
		std::basic_stringstream<CharType> tss;
		if( !(tss << std::fixed << rVal))
		{
			return std::basic_string<CharType>();
		}
		return tss.str();
	}

	template<typename T_>
	bool convertTo(vfs::String const& sStr, T_ &rVal)
	{
		std::wstringstream ss;
		ss.str(sStr.c_wcs());
		if(!(ss >> rVal))
		{
			return false;
		}
		return true;
	}

	template<typename T_>
	vfs::String toStringList(std::list<T_> const& rValList)
	{
		std::wstringstream ss;
		typename std::list<T_>::const_iterator cit = rValList.begin();
		if(cit != rValList.end())
		{
			ss << *cit;
			cit++;
			for(;cit != rValList.end(); ++cit)
			{
				ss << L" , " << *cit;
			}
		}
		if(!ss)
		{
			return L"";
		}
		return ss.str();
	}

	template<>
	VFS_API vfs::String toStringList<vfs::String>(std::list<vfs::String> const& rValList);

	////////////////////////////////////////////////////////////////////////////////////////////

	// remove leading and trailing white characters;
	template<typename StringType>
	VFS_API StringType trimString(StringType const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	// explicit instantiations : std::string, std::wstring, vfs::String
	template<>
	VFS_API std::string trimString<std::string>(std::string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	template<>
	VFS_API std::wstring trimString<std::wstring>(std::wstring const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	template<>
	VFS_API vfs::String trimString<vfs::String>(vfs::String const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);


} // namespace vfs

#endif // _TOOLS_H_
