/* 
 * bfVFS : vfs/Core/File/vfs_buffer_file.h
 *  - Buffer in RAM, implements File interface to unify usage of file and memory
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

#ifndef _VFS_MEMORY_FILE_H_
#define _VFS_MEMORY_FILE_H_

#include <vfs/Core/Interface/vfs_file_interface.h>
#include <sstream>

namespace vfs
{
	class VFS_API CBufferFile : public vfs::TFileTemplate<vfs::IReadable,vfs::IWritable>
	{
		typedef vfs::TFileTemplate<vfs::IReadable,vfs::IWritable> tBaseClass;
	public :
		CBufferFile();
		CBufferFile(vfs::Path const& filename);
		virtual ~CBufferFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual void		close();
		virtual vfs::size_t	getSize();

		virtual bool		isOpenRead();
		virtual bool		openRead();
		virtual vfs::size_t	read(vfs::Byte* data, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition();
		virtual void		setReadPosition(vfs::size_t positionInBytes);
		virtual void		setReadPosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual bool		isOpenWrite();
		virtual bool		openWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);
		virtual vfs::size_t	write(const vfs::Byte* data, vfs::size_t bytesToWrite);

		virtual vfs::size_t	getWritePosition();
		virtual void		setWritePosition(vfs::size_t positionInBytes);
		virtual void		setWritePosition(vfs::offset_t	offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual bool		deleteFile();

		// convenience method
		void				copyToBuffer(vfs::tReadableFile& rFile);
	protected:
		std::stringstream	m_buffer;
		bool				m_isOpen_read, m_isOpen_write;
	};

} // end namespace

#endif // _VFS_MEMORY_FILE_H_

