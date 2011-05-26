/* 
 * bfVFS : vfs/Core/File/vfs_dir_file.h
 *  - read/read-write files for usage in vfs locations
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

#ifndef _VFS_DIR_FILE_H_
#define _VFS_DIR_FILE_H_

#include <vfs/Core/File/vfs_file.h>

#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/Interface/vfs_location_interface.h>
#include <vfs/Core/Interface/vfs_directory_interface.h>

namespace vfs
{
	class VFS_API CReadOnlyDirFile : public vfs::CReadOnlyFile
	{
	protected:
		typedef vfs::TDirectory<vfs::CReadOnlyDirFile::write_type> tLocation;
	public:
		CReadOnlyDirFile(vfs::Path const& filename, tLocation *directory);
		virtual ~CReadOnlyDirFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual vfs::Path	getPath();

		virtual bool		openRead();

		virtual bool		_getRealPath(vfs::Path& path);
	private:
		tLocation*			_location;
	};

	class VFS_API CDirFile : public vfs::CFile
	{
		typedef vfs::TDirectory<vfs::CFile::write_type> tLocation;
	public:
		CDirFile(vfs::Path const& filename, tLocation *directory);
		virtual ~CDirFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual vfs::Path	getPath();
		virtual bool		deleteFile();

		virtual bool		openRead();
		virtual bool		openWrite(bool createWhenNotExist = false, bool truncate = false);

		virtual bool		_getRealPath(vfs::Path& path);
	private:
		tLocation*			_location;
	};
} // end namespace

#endif // _VFS_DIR_FILE_H_

