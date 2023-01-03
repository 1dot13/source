/* 
 * bfVFS : vfs/Core/Interface/vfs_directory_interface.h
 *  - partially implements Location interface for file system directories
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

#ifndef _VFS_DIRECTORY_INTERFACE_H_
#define _VFS_DIRECTORY_INTERFACE_H_

#include <vfs/Core/Interface/vfs_location_interface.h>

namespace vfs
{
	template<class WriteType>
	class TDirectory : public vfs::TLocationTemplate<vfs::IReadable, WriteType>
	{
	public:
		typedef typename vfs::TLocationTemplate<vfs::IReadable, WriteType> tBaseClass;
		typedef typename tBaseClass::tFileType tFileType;
		typedef typename tBaseClass::tWriteType tWriteType;

		TDirectory(vfs::Path const& mountPoint, vfs::Path const& realPath)
			: tBaseClass(mountPoint), m_realPath(realPath)
		{};
		virtual ~TDirectory()
		{};
	
		vfs::Path const&	getRealPath()
		{
			return m_realPath;
		}
	
		virtual tFileType*	addFile(vfs::Path const& sFilename, bool bDeleteOldFile=false) = 0;
		virtual bool		addFile(typename tBaseClass::tFileType* pFile, bool bDeleteOldFile=false) = 0;
	
		virtual bool		createSubDirectory(vfs::Path const& sSubDirPath) = 0;
		virtual bool		deleteDirectory(vfs::Path const& sDirPath) = 0;
		virtual bool		deleteFileFromDirectory(vfs::Path const& sFileName) = 0;
	protected:
		const vfs::Path 	m_realPath;
	private:
		void				operator=(TDirectory<WriteType> const& t);
	};
}

#endif // _VFS_DIRECTORY_INTERFACE_H_
