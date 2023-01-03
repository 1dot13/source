/* 
 * bfVFS : vfs_config.h
 *  - define basic library macros, especially the DLL interface macros
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

#ifndef _VFS_CONFIG_H_
#define _VFS_CONFIG_H_

#ifdef WIN32
#ifdef _MSC_VER
#	ifndef PRINT_DLL_INTERFACE_WARNING
#		pragma warning ( disable : 4251 )
#	endif
#endif
#endif

#if defined(_WIN32) && defined(_MSC_VER)
	// VFS_STATIC overrides VFS_EXPORT
#	ifdef VFS_STATIC
#		define VFS_API
#	endif

#	ifndef VFS_STATIC
#		ifdef VFS_EXPORT
#			define VFS_API		__declspec(dllexport)
#		else
#			define VFS_API		__declspec(dllimport)
#		endif
#	endif
#else
#	define VFS_API
#endif

#define VFS_VERSION_MAJOR	1
#define VFS_VERSION_MINOR	0
#define VFS_VERSION_PATCH	0

#if defined VFS_NO_LOGGING
#	define LOG_INFO(x)
#	define LOG_WARNING(y)
#	define LOG_ERROR(z)
#endif

#endif // _VFS_CONFIG_H_
