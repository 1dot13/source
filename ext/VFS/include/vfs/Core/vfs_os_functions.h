/* 
 * bfVFS : vfs/Core/os_functions.h
 *  - abstractions for OS dependant code
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

#ifndef _VFS_OS_FUNCTIONS_H_
#define _VFS_OS_FUNCTIONS_H_

#ifdef WIN32
	#include <windows.h>
#else 
	#include <sys/types.h>
	#include <sys/dir.h>
	#include <unistd.h>
	#include <stdio.h>
#endif

#include <vfs/vfs_config.h>
#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs_path.h>

namespace vfs
{
	namespace OS
	{
		class VFS_API CIterateDirectory
		{
		public:
			enum EFileAttribute
			{
				FA_DIRECTORY,
				FA_FILE
			};
		public:
			CIterateDirectory(vfs::Path const& sPath, vfs::String const& searchPattern);
			~CIterateDirectory();

			bool nextFile(vfs::String &fileName, CIterateDirectory::EFileAttribute &attrib);
		private:
		#ifdef WIN32
			HANDLE fSearchHandle;
			union
			{
				WIN32_FIND_DATAA	fFileInfoA;
				WIN32_FIND_DATAW	fFileInfoW;
			};
		#else
			struct direct **files;
			int count, current_pos;
		#endif
			bool fFirstRequest;
		};

		class FileAttributes
		{
		public:
			enum Attributes
			{
				ATTRIB_INVALID		= 0,
				ATTRIB_ARCHIVE		= 1,
				ATTRIB_DIRECTORY	= 2,
				ATTRIB_HIDDEN		= 4,
				ATTRIB_NORMAL		= 8,
				ATTRIB_READONLY		= 16,
				ATTRIB_SYSTEM		= 32,
				ATTRIB_TEMPORARY	= 64,
				ATTRIB_COMPRESSED	= 128,
				ATTRIB_OFFLINE		= 256,
			};
			bool getFileAttributes(vfs::Path const& sDir, vfs::UInt32& uiAttribs);
		};


		VFS_API bool checkRealDirectory(vfs::Path const& sDir);
		VFS_API bool createRealDirectory(vfs::Path const& sDir);

		VFS_API bool deleteRealFile(vfs::Path const& sDir);

		VFS_API void getExecutablePath(vfs::Path& sDir, vfs::Path& sFile);
		VFS_API void getCurrentDirectory(vfs::Path& sDir);
		VFS_API void setCurrectDirectory(vfs::Path const& sPath);

		VFS_API bool getEnv(vfs::String const& key, vfs::String& value);
	}; // end namespace OS

} // end namespace vfs

#endif // _VFS_OS_FUNCTIONS_H_

