/* 
 * bfVFS : vfs/Core/Location/vfs_directory_tree.h
 *  - class for directories in a File System, implements Directory interface
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

#ifndef _VFS_DIRECTORY_H_
#define _VFS_DIRECTORY_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/Interface/vfs_directory_interface.h>

#include <map>
#include <vector>

namespace vfs
{

	/**
	 *  IDirectory<read,write>
	 */
	template<typename WriteType>
	class TDirectoryTree : public vfs::TDirectory<WriteType>
	{
		typedef std::map<vfs::Path, vfs::TDirectory<typename TDirectoryTree<WriteType>::tWriteType>*, vfs::Path::Less> tDirCatalogue;

		class IterImpl;
	public:
		typedef vfs::TDirectory<WriteType> tBaseClass;
		typedef typename tBaseClass::tWriteType tWriteType;
		typedef typename tBaseClass::tFileType tFileType;

		typedef TIterator<vfs::IBaseFile> Iterator;
		TDirectoryTree(vfs::Path const& sMountPoint, vfs::Path const& sRealPath);
		virtual ~TDirectoryTree();

		bool					init();

		/** 
		 *  TDirectory interface
		 */
		virtual tFileType*		addFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			addFile(tFileType* pFile, bool bDeleteOldFile=false);

		virtual bool			createSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			deleteDirectory(vfs::Path const& sDirPath);
		virtual bool			deleteFileFromDirectory(vfs::Path const& sFileName);

		/** 
		 *  TLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& sFileName);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& sFileName);
		virtual tFileType*		getFileTyped(vfs::Path const& sFileName);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tDirCatalogue m_catDirs;
	};

	typedef TDirectoryTree<vfs::IWritable>		CDirectoryTree;
	typedef TDirectoryTree<vfs::IWriteType>		CReadOnlyDirectoryTree;

} // end namespace

#endif // _VFS_DIRECTORY_H_
