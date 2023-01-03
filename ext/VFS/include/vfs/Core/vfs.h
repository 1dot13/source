/* 
 * bfVFS : vfs/Core/vfs.h
 *  - primary interface for the using program, get files from the VFS internal storage
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

#ifndef _VFS_H_
#define _VFS_H_

#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/Interface/vfs_location_interface.h>
#include <vfs/Core/Interface/vfs_iterator_interface.h>
#include <vfs/Core/vfs_vloc.h>
#include <vfs/Core/vfs_vfile.h>

#include <map>

namespace vfs
{
	class CProfileStack;

	class VFS_API CVirtualFileSystem
	{
		typedef std::map<vfs::Path,CVirtualLocation*,vfs::Path::Less> tVFS;

		class CRegularIterator;
		class CMatchingIterator;
	public:
		typedef TIterator<vfs::tReadableFile> Iterator;

		~CVirtualFileSystem();

		static CVirtualFileSystem*	getVFS();
		static void					shutdownVFS();

		vfs::CProfileStack*		getProfileStack();

		vfs::CVirtualLocation*	getVirtualLocation(vfs::Path const& sPath, bool bCreate = false);
		bool					addLocation(vfs::IBaseLocation* pLocation, vfs::CVirtualProfile *pProfile);

		bool					fileExists(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		bool					fileExists(vfs::Path const& rLocalFilePath, std::string const& sProfileName);

		vfs::IBaseFile*			getFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::IBaseFile*			getFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName);

		vfs::tReadableFile*		getReadFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::tReadableFile*		getReadFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName);

		vfs::tWritableFile*		getWriteFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::tWritableFile*		getWriteFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName);

		bool					removeFileFromFS(vfs::Path const& sFilePath);
		bool					removeDirectoryFromFS(vfs::Path const& sDir);
		bool					createNewFile(vfs::Path const& sFileName);

		Iterator				begin();
		Iterator				begin(vfs::Path const& sPattern);
	private:
		vfs::CProfileStack		m_oProfileStack;
		tVFS					m_mapFS;
	private:
		CVirtualFileSystem();
		static CVirtualFileSystem* m_pSingleton;
	};

	VFS_API bool canWrite();
} // end namespace

VFS_API vfs::CVirtualFileSystem*  getVFS();


#endif // _VFS_H_
