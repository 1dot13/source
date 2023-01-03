/* 
 * bfVFS : vfs/Core/Interface/vfs_library_interface.h
 *  - partially implements Location interface for archive files
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

#ifndef _VFS_LIBRARY_INTERFACE_H_
#define _VFS_LIBRARY_INTERFACE_H_

#include <vfs/Core/Interface/vfs_location_interface.h>

namespace vfs
{
	class VFS_API ILibrary : public vfs::TLocationTemplate<vfs::IReadable,vfs::IWriteType>
	{
		typedef vfs::TLocationTemplate<vfs::IReadable,vfs::IWriteType> tBaseClass;
	public:
		ILibrary(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile = false);
		virtual ~ILibrary();
	
		virtual bool		init() = 0;
		virtual void		closeLibrary() = 0;
	
		virtual void		close(tFileType *fileHandle) = 0;
		virtual bool		openRead(tFileType *fileHandle) = 0;
		virtual vfs::size_t	read(tFileType *fileHandle, vfs::Byte* data, vfs::size_t bytesToRead) = 0;
		
		virtual vfs::size_t	getReadPosition(tFileType *fileHandle) = 0;
		virtual void		setReadPosition(tFileType *fileHandle, vfs::size_t positionInBytes) = 0;
		virtual void		setReadPosition(tFileType *fileHandle, vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir) = 0;
	
		virtual vfs::size_t	getSize(tFileType *pFileHandle) = 0;

		vfs::Path const&	getName();
	protected:
		bool				m_ownLibFile;
		vfs::tReadableFile*	m_libraryFile;
	};	

}

#endif // _VFS_LIBRARY_INTERFACE_H_
