 /* 
 * bfVFS : vfs/Core/vfs_dfebug.h
 *  - Exception class and throw macros, used to notify the using program of unexpected situations
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

#ifndef _VFS_DEBUG_H_
#define _VFS_DEBUG_H_

#include <vfs/Aspects/vfs_logging.h>

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs_path.h>
#include <list>

#ifdef _MSC_VER
class VFS_API std::exception;
#endif

namespace vfs
{
	class VFS_API Exception : public std::exception
	{
	public:
		Exception(vfs::String const& text, vfs::String const& function, int line, const char* file);
		Exception(vfs::String const& text, vfs::String const& function, int line, const char* file, std::exception& ex);

		virtual ~Exception() throw();

		virtual const char* what() const throw();

		vfs::String getLastEntryString() const;
		vfs::String getExceptionString() const;

		struct SEntry
		{
			vfs::String		time;
			vfs::String		message;
			vfs::String		function;
			int				line;
			vfs::String		file;
		};
		typedef std::list<SEntry> CALLSTACK;
		CALLSTACK m_CallStack;
	};
}

#ifdef WIN32
#define _FUNCTION_FORMAT_		__FUNCTION__
#else
//#define _FUNCTION_FORMAT_		__FUNCTION__
#define _FUNCTION_FORMAT_		__PRETTY_FUNCTION__
#endif

#define VFS_THROW(message) throw vfs::Exception((message), _FUNCTION_FORMAT_, __LINE__, __FILE__)
#define VFS_RETHROW(message,ex) throw vfs::Exception((message), _FUNCTION_FORMAT_, __LINE__, __FILE__, (ex))

#define VFS_THROW_IFF(boolexpr,message) if(!(boolexpr)){VFS_THROW((message));}

#define VFS_TRYCATCH_RETHROW(expr,message) \
{ \
	try { (expr); } \
	catch(std::exception &ex){ throw vfs::Exception((message),_FUNCTION_FORMAT_,__LINE__,__FILE__, ex); } \
}

#define VFS_IGNOREEXCEPTION(expr, log) \
{ \
	try{ (expr); } \
	catch(std::exception& ex){ \
	if(log) VFS_LOG_ERROR(ex.what()); } \
}

#endif // _VFS_DEBUG_H_
