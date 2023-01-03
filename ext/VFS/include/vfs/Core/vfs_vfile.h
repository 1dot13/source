/* 
 * bfVFS : vfs/Core/vfs_vfile.h
 *  - Virtual File, handles access to files from different VFS profiles
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

#ifndef _VFS_VFILE_H_
#define _VFS_VFILE_H_

#include <vfs/Core/vfs_profile.h>
#include <vfs/Tools/vfs_allocator.h>
#include <vector>

#define VFILE_BLOCK_CREATE

namespace vfs
{
	class VFS_API CVirtualFile
	{
	public:
		enum ESearchFile
		{
			SF_TOP,
			SF_FIRST_WRITABLE,
			SF_STOP_ON_WRITABLE_PROFILE,
		};
	public:
		CVirtualFile();
		~CVirtualFile();
		void destroy();
		static CVirtualFile* create(vfs::Path const& sFilePath, vfs::CProfileStack& rPStack);

		vfs::Path const&	path();
		void				add(vfs::IBaseFile *pFile, vfs::String sProfileName, bool bReplace = false);
		bool				remove(vfs::IBaseFile *pFile);
		//////////////////////////////////////////////////
		vfs::IBaseFile*		file(ESearchFile eSearch);
		vfs::IBaseFile*		file(vfs::String const& sProfileName);
		//////////////////////////////////////////////////
	private:
		vfs::Path			_path;
		vfs::String			_top_pname;
		vfs::IBaseFile*		_top_file;
		CProfileStack*		_pstack;
	private:
		vfs::UInt32			_myID;
#ifdef VFILE_BLOCK_CREATE
		static ObjBlockAllocator<vfs::CVirtualFile>* _vfile_pool;
#endif
	};
} // end namspace

#endif // _VFS_VFILE_H_
