/* 
 * bfVFS : vfs/Core/Location/vfs_uncompressed_lib_base.cpp
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

#include <vfs/Core/Location/vfs_uncompressed_lib_base.h>

/********************************************************************************************/
vfs::CUncompressedLibraryBase::SFileData& vfs::CUncompressedLibraryBase::_fileDataFromHandle(tFileType* handle)
{
	tFileData::iterator it = m_fileData.find(handle);
	if(it != m_fileData.end())
	{
		return it->second;
	}
	VFS_THROW(L"Invalid file handle");
}
/********************************************************************************************/
/********************************************************************************************/

class vfs::CUncompressedLibraryBase::IterImpl : public vfs::IBaseLocation::Iterator::IImplementation
{
	typedef vfs::IBaseLocation::Iterator::IImplementation tBaseClass;
public:
	IterImpl(CUncompressedLibraryBase& lib);
	virtual ~IterImpl();
	virtual tFileType*									value();
	virtual void										next();
protected:
	virtual tBaseClass* clone()
	{
		IterImpl* iter = new IterImpl(*_lib);
		iter->_iter = _iter;
		return iter;
	}
private:
	vfs::CUncompressedLibraryBase*						_lib;
	vfs::CUncompressedLibraryBase::tFileData::iterator	_iter;
};


vfs::CUncompressedLibraryBase::IterImpl::IterImpl(vfs::CUncompressedLibraryBase &lib)
: tBaseClass(), _lib(&lib)
{
	_iter = _lib->m_fileData.begin();
}
vfs::CUncompressedLibraryBase::IterImpl::~IterImpl()
{
}

vfs::CUncompressedLibraryBase::tFileType* vfs::CUncompressedLibraryBase::IterImpl::value()
{
	if(_iter != _lib->m_fileData.end())
	{
		return _iter->first;
	}
	return NULL;
}

void vfs::CUncompressedLibraryBase::IterImpl::next()
{
	if(_iter != _lib->m_fileData.end())
	{
		_iter++;
	}
}

/************************************************************************/
/************************************************************************/

vfs::CUncompressedLibraryBase::CUncompressedLibraryBase(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile) 
: vfs::ILibrary(libraryFile,mountPoint,ownFile), m_numberOfOpenedFiles(0)
{
}

vfs::CUncompressedLibraryBase::~CUncompressedLibraryBase() 
{
	this->closeLibrary();
	// delete sub dirs from catalogue
	tDirCatalogue::iterator it = m_dirs.begin();
	for(; it != m_dirs.end(); ++it)
	{
		delete it->second;
	}
	// LibData is invalid 
	// just clear it, since the file handles were deleted before
	m_fileData.clear(); 
	m_dirs.clear();
}

void vfs::CUncompressedLibraryBase::closeLibrary()
{
	tFileData::iterator it = m_fileData.begin();
	for(; it != m_fileData .end(); ++it)
	{
		// what if closing of (at least) one file fails?? continue or not??
		// in the end, these are not real files!
		VFS_IGNOREEXCEPTION(it->first->close(), true);
	}
}

bool vfs::CUncompressedLibraryBase::fileExists(vfs::Path const& filename)
{
	vfs::Path sDir,sFile;
	filename.splitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_dirs.find(sDir);
	if(it != m_dirs.end())
	{
		return it->second->fileExists(sFile);
	}
	return false;
}

vfs::IBaseFile*	vfs::CUncompressedLibraryBase::getFile(vfs::Path const& filename)
{
	return getFileTyped(filename);
}

vfs::CUncompressedLibraryBase::tFileType* vfs::CUncompressedLibraryBase::getFileTyped(vfs::Path const& filename)
{
	vfs::Path sDir,sFile;
	filename.splitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_dirs.find(sDir);
	if(it != m_dirs.end())
	{
		return it->second->getFileTyped(sFile);
	}
	return NULL;
}

void vfs::CUncompressedLibraryBase::close(tFileType *fileHandle)
{
	try
	{
		SFileData& file = _fileDataFromHandle(fileHandle);

		// reset read position 
		// can't do this when opening file, 
		// because you could try to open a file when it is already open and would so reset the read position
		file._currentReadPosition = 0;
		if(m_numberOfOpenedFiles > 0)
		{
			m_numberOfOpenedFiles--;
			if(m_numberOfOpenedFiles == 0)
			{
				m_libraryFile->close();
			}
		}
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}
bool vfs::CUncompressedLibraryBase::openRead(tFileType *fileHandle)
{
	try
	{
		_fileDataFromHandle(fileHandle);
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}

	m_numberOfOpenedFiles++;
	if(m_numberOfOpenedFiles == 1)
	{
		if(!m_libraryFile->isOpenRead() && !m_libraryFile->openRead())
		{
			return false;
		}
	}
	// already open
	return true;
}

vfs::size_t vfs::CUncompressedLibraryBase::read(tFileType *fileHandle, vfs::Byte* data, vfs::size_t bytesToRead)
{
	try
	{
		SFileData& file = _fileDataFromHandle(fileHandle);

		if( (file._currentReadPosition + bytesToRead) > file._fileSize )
		{
			bytesToRead = file._fileSize - file._currentReadPosition;
		}
		if(bytesToRead == 0)
		{
			// eof
			return 0;
		}
		// set lib-file's read-location to match location of lib-file
		m_libraryFile->setReadPosition(file._fileOffset + file._currentReadPosition, IBaseFile::SD_BEGIN);

		vfs::size_t bytesRead = m_libraryFile->read(data, bytesToRead);
		VFS_THROW_IFF( bytesToRead == bytesRead, L"Number of bytes doesn't match" );
		file._currentReadPosition += bytesRead;
		return bytesRead;
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}

vfs::size_t vfs::CUncompressedLibraryBase::getReadPosition(tFileType *fileHandle)
{
	try
	{
		return _fileDataFromHandle(fileHandle)._currentReadPosition;
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}

void vfs::CUncompressedLibraryBase::setReadPosition(tFileType *fileHandle, vfs::size_t positionInBytes)
{
	try
	{
		SFileData& file = _fileDataFromHandle(fileHandle);
		if( positionInBytes > file._fileSize )
		{
			positionInBytes = file._fileSize;
		}

		// positionInBytes is offset to file-offset
		file._currentReadPosition = positionInBytes;
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}

static inline vfs::offset_t clampReadPosition(vfs::offset_t const& off, vfs::size_t const& size)
{
	return ( off < 0 ) ? ( 0 ) : ( (vfs::size_t)off > size ? size : off );
}

void vfs::CUncompressedLibraryBase::setReadPosition(tFileType *fileHandle, vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	try
	{
		SFileData& file = _fileDataFromHandle(fileHandle);

		if(seekDir == IBaseFile::SD_BEGIN)
		{
			file._currentReadPosition = clampReadPosition(offsetInBytes, file._fileSize);
		}
		else if(seekDir == IBaseFile::SD_CURRENT)
		{
			vfs::offset_t pos = file._currentReadPosition + offsetInBytes;
			file._currentReadPosition = clampReadPosition(pos, file._fileSize);
		}
		else if(seekDir == IBaseFile::SD_END)
		{
			vfs::offset_t pos = file._currentReadPosition + offsetInBytes;
			file._currentReadPosition = clampReadPosition(pos, file._fileSize);
		}
		else
		{
			VFS_THROW(L"Unknown seek direction");
		}
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}

vfs::size_t vfs::CUncompressedLibraryBase::getSize(tFileType *fileHandle)
{
	try
	{
		return _fileDataFromHandle(fileHandle)._fileSize;
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}
}

void vfs::CUncompressedLibraryBase::getSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	tDirCatalogue::iterator it = m_dirs.begin();
	for(;it != m_dirs.end(); ++it)
	{
		rlSubDirs.push_back(it->first);
	}
}

vfs::CUncompressedLibraryBase::Iterator vfs::CUncompressedLibraryBase::begin()
{
	return Iterator(new IterImpl(*this));
}



