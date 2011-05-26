/* 
 * bfVFS : vfs/Core/File/vfs_lib_file.cpp
 *  - read/read-write files for usage in vfs locations (libraries)
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

#include <vfs/Core/File/vfs_lib_file.h>
#include <vfs/Core/vfs.h>


#define ERROR_FILE(msg)		(_BS(L"[") << this->getPath()() << L"] - " << msg << _BS::wget)

vfs::ObjBlockAllocator<vfs::CLibFile>* vfs::CLibFile::_lfile_pool = NULL;

vfs::CLibFile* vfs::CLibFile::create(vfs::Path const& filename, 
									 tLocation *location, 
									 ILibrary *library,
									 vfs::ObjBlockAllocator<vfs::CLibFile>* allocator)
{
#if 0
	vfs::CLibFile* pFile = new vfs::CLibFile();
#else
	vfs::CLibFile* pFile;
	if(allocator)
	{
		pFile = allocator->New();
	}
	else
	{
		if(!_lfile_pool)
		{
			_lfile_pool = new vfs::ObjBlockAllocator<vfs::CLibFile>();
			vfs::ObjectAllocator::registerAllocator(_lfile_pool);
		}
		pFile = _lfile_pool->New();
	}
#endif
	pFile->m_filename = filename;
	pFile->m_location = location;
	pFile->m_library = library;
	return pFile;
}

vfs::CLibFile::CLibFile()
: tBaseClass(L""), 
	m_isOpen_read(false),
	m_library(NULL),
	m_location(NULL)
{
};

vfs::CLibFile::~CLibFile()
{
}

void vfs::CLibFile::close()
{
	if(m_isOpen_read)
	{
		m_library->close(this);
		m_isOpen_read = false;
	}
}

vfs::FileAttributes vfs::CLibFile::getAttributes()
{
	return vfs::FileAttributes(vfs::FileAttributes::ATTRIB_NORMAL | vfs::FileAttributes::ATTRIB_READONLY,
		vfs::FileAttributes::LT_LIBRARY);
}

vfs::Path vfs::CLibFile::getPath()
{
	if(m_location)
	{
		return m_location->getPath() + m_filename;
	}
	else
	{
		return m_filename;
	}
}
bool vfs::CLibFile::isOpenRead()
{
	return m_isOpen_read;
}

bool vfs::CLibFile::openRead()
{
	if(!m_isOpen_read)
	{
		VFS_TRYCATCH_RETHROW(m_isOpen_read = m_library->openRead(this), ERROR_FILE(L"read open error"));
	}
	return m_isOpen_read;
}

vfs::size_t vfs::CLibFile::read(vfs::Byte* data, vfs::size_t bytesToRead)
{
	VFS_THROW_IFF( m_isOpen_read, ERROR_FILE(L"file not opened") );
	try
	{
		return m_library->read(this, data, bytesToRead);
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(ERROR_FILE(L"read error"), ex);
	}
}	

vfs::size_t vfs::CLibFile::getReadPosition()
{
	VFS_THROW_IFF( m_isOpen_read, ERROR_FILE(L"file not opened") );
	try
	{
		return m_library->getReadPosition(this);
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(ERROR_FILE(L"library error"), ex);
	}
}


void vfs::CLibFile::setReadPosition(vfs::size_t uiPositionInBytes)
{
	VFS_THROW_IFF( m_isOpen_read, ERROR_FILE(L"file not opened") );
	VFS_TRYCATCH_RETHROW(m_library->setReadPosition(this,uiPositionInBytes), ERROR_FILE(L"library error") );
}

void vfs::CLibFile::setReadPosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	VFS_THROW_IFF( m_isOpen_read, ERROR_FILE(L"file not opened") );
	VFS_TRYCATCH_RETHROW(m_library->setReadPosition(this, offsetInBytes, seekDir), ERROR_FILE(L"library error"));
}

vfs::size_t vfs::CLibFile::getSize()
{
	VFS_THROW_IFF( m_isOpen_read || this->openRead(), ERROR_FILE(L"could not open file") );
	try
	{
		return m_library->getSize(this);
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(ERROR_FILE(L"library error"), ex);
	}
}
