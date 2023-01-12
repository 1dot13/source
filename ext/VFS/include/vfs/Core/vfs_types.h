/* 
 * bfVFS : vfs/Core/vfs_types.h
 *  - basic integer types, "incomplete" list of useful constant strings
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

#ifndef _VFS_TYPES_H_
#define _VFS_TYPES_H_

#include <vfs/Core/vfs_string.h>

#ifdef __linux__
#	include <stdint.h>
#endif

namespace vfs
{
#ifdef WIN32
	typedef unsigned __int64	UInt64;
	typedef unsigned __int32	UInt32;
	typedef unsigned __int16	UInt16;
	typedef unsigned __int8		UInt8;
	typedef unsigned __int8		UByte;

	typedef __int64				Int64;
	typedef __int32				Int32;
	typedef __int16				Int16;
	typedef __int8				Int8;
	typedef __int8				Byte;
#elif __linux__
	typedef uint64_t			UInt64;
	typedef uint32_t  			UInt32;
	typedef uint16_t			UInt16;
	typedef uint8_t				UInt8;
	typedef uint8_t				UByte;

	typedef int64_t				Int64;
	typedef int32_t				Int32;
	typedef int16_t				Int16;
	typedef int8_t				Int8;
	typedef char				Byte;
#endif

	typedef ::size_t			size_t;
	typedef ::off_t				offset_t;

	extern const vfs::size_t	npos;
}

namespace vfs
{
	namespace Const
	{
		inline const vfs::String::str_t		EMPTY()				{ return L""; };
		//inline const vfs::String::char_t	EMPTY_CHAR()		{ return L''; };

		inline const vfs::String::str_t		DOT()				{ return L"."; };
		inline const vfs::String::char_t	DOT_CHAR()			{ return L'.'; };

		inline const vfs::String::str_t		DOTDOT()			{ return L".."; };
		inline const vfs::String::str_t		DOTSVN()			{ return L".svn"; };

		inline const vfs::String::str_t		STAR()				{ return L"*"; };
		inline const vfs::String::str_t		DSTAR()				{ return L"**"; };

#ifdef WIN32
		inline const vfs::String::str_t		SEPARATOR()			{ return L"\\"; };
		inline const vfs::String::char_t	SEPARATOR_CHAR()	{ return L'\\'; };
#else
		inline const vfs::String::str_t		SEPARATOR()			{ return L"/"; };
		inline const vfs::String::char_t	SEPARATOR_CHAR()	{ return L'/'; };
#endif
	}
}

#endif // _VFS_TYPES_H_
