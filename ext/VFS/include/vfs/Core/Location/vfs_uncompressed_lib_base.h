/* 
 * bfVFS : vfs/Core/Location/vfs_uncompressed_lib_base.h 
 *  - partially implements library interface for uncompressed archive files
 *  - initialization is done in format-specific sub-classes
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

#ifndef _VFS_UNCOMPRESSED_LIB_BASE_H_
#define _VFS_UNCOMPRESSED_LIB_BASE_H_

#include <vfs/Core/Interface/vfs_library_interface.h>
#include <vfs/Core/Interface/vfs_directory_interface.h>
#include <sstream>

namespace vfs
{

	class VFS_API CUncompressedLibraryBase : public vfs::ILibrary
	{
	protected:
		typedef std::map<vfs::Path, vfs::TDirectory<vfs::ILibrary::tWriteType>*, vfs::Path::Less> tDirCatalogue;
		struct SFileData
		{
			SFileData(vfs::size_t const& fileSize, vfs::size_t const& fileOffset)
				: _fileSize(fileSize), _fileOffset(fileOffset), _currentReadPosition(0)
			{};
			vfs::size_t _fileSize, _fileOffset, _currentReadPosition;
		};
		typedef std::map<tFileType*, SFileData> tFileData;
		
		class IterImpl;
	public:
		CUncompressedLibraryBase(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile = false);
		virtual ~CUncompressedLibraryBase();

		/**
		 *  TLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& filename);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& filename);
		virtual tFileType*		getFileTyped(vfs::Path const& filename);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		/**
		 *  ILibrary interface
		 */
		virtual bool		init() = 0;
		virtual void		closeLibrary();

		virtual void		close(tFileType *fileHandle);
		virtual bool		openRead(tFileType *fileHandle);
		virtual vfs::size_t	read(tFileType *fileHandle, vfs::Byte* data, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition(tFileType *fileHandle);
		virtual void		setReadPosition(tFileType *fileHandle, vfs::size_t positionInBytes);
		virtual void		setReadPosition(tFileType *fileHandle, vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir);

		virtual vfs::size_t	getSize(tFileType *fileHandle);

		virtual Iterator	begin();
	protected:
		tDirCatalogue		m_dirs;
		tFileData			m_fileData;
		vfs::UInt32			m_numberOfOpenedFiles;
	private:
		SFileData&			_fileDataFromHandle(tFileType* handle);
	};
} // end namespace

#endif // _VFS_UNCOMPRESSED_LIB_BASE_H_

