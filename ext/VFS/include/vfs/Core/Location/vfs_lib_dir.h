/* 
 * bfVFS : vfs/Core/Location/vfs_lib_dir.h
 *  - class for readonly (sub)directories in archives/libraries
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

#ifndef _VFS_LIB_DIR_H_
#define _VFS_LIB_DIR_H_

#include <vfs/Core/Interface/vfs_directory_interface.h>

namespace vfs
{

	class CLibDirectory : public vfs::TDirectory<vfs::IWriteType>
	{
		typedef vfs::TDirectory<vfs::IWriteType> tBaseClass;
		typedef std::map<vfs::Path, tFileType*, vfs::Path::Less> tFileCatalogue;

		class IterImpl;
	public:
		CLibDirectory(vfs::Path const& sLocalPath, vfs::Path const& sRealPath);
		virtual ~CLibDirectory();

		/** 
		 *  TDirectory interface
		 */
		virtual tFileType*		addFile(vfs::Path const& filename, bool deleteOldFile=false);
		virtual bool			addFile(tFileType* file, bool deleteOldFile=false);
		virtual bool			deleteFileFromDirectory(vfs::Path const& filename);
		virtual bool			createSubDirectory(vfs::Path const& subDirPath);
		virtual bool			deleteDirectory(vfs::Path const& dirPath);

		/** 
		 *  TLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& filename);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& filename);
		virtual tFileType*		getFileTyped(vfs::Path const& filename);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tFileCatalogue m_files;
	};

} // -end- namespace

#endif // _VFS_LIB_DIR_H_
