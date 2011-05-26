/* 
 * bfVFS : vfs/Core/File/vfs_buffer_file.cpp
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

#include <vfs/Core/File/vfs_buffer_file.h>
#include <vfs/Core/vfs_file_raii.h>

#include <vector>

#define ERROR_FILE(msg)			(_BS(L"[") << this->getPath() << L"] - " << (msg) << _BS::wget)

#ifdef _DEBUG
	#define IS_FILE_VALID()		VFS_THROW_IFF( m_buffer.good(), ERROR_FILE(L"invalid file object") );
#else
	#define IS_FILE_VALID()		VFS_THROW_IFF( m_buffer.good(), ERROR_FILE(L"invalid file object") );
#endif

static inline std::ios::seekdir _seekDir(vfs::IBaseFile::ESeekDir seekDir)
{
	if(seekDir == vfs::IBaseFile::SD_BEGIN)
	{
		return std::ios::beg;
	}
	else if(seekDir == vfs::IBaseFile::SD_CURRENT)
	{
		return std::ios::cur;
	}
	else if(seekDir == vfs::IBaseFile::SD_END)
	{
		return std::ios::end;
	}
	VFS_THROW(_BS(L"Unknown seek direction [") << seekDir << L"]" << _BS::wget);
}

///////////////////////////////////////////////////

vfs::CBufferFile::CBufferFile()
: tBaseClass(vfs::Path()), m_isOpen_read(false), m_isOpen_write(false)
{
	m_buffer.str("");
}

vfs::CBufferFile::CBufferFile(vfs::Path const& fileName)
: tBaseClass(fileName), m_isOpen_read(false), m_isOpen_write(false)
{
	m_buffer.str("");
}

vfs::CBufferFile::~CBufferFile()
{
	m_buffer.str("");
	m_buffer.clear();
}

vfs::FileAttributes vfs::CBufferFile::getAttributes()
{
	return vfs::FileAttributes(vfs::FileAttributes::ATTRIB_NORMAL, vfs::FileAttributes::LT_NONE);
}

void vfs::CBufferFile::close()
{
	m_buffer.clear();
	m_isOpen_read = false;
	m_isOpen_write = false;
	IS_FILE_VALID();
}
vfs::size_t vfs::CBufferFile::getSize()
{
	IS_FILE_VALID();

	std::streampos size = 0;
	if(!m_buffer.str().empty())
	{
		std::streampos current_position = m_buffer.tellg();
		m_buffer.seekg(0,std::ios::end);
		size = m_buffer.tellg();
		m_buffer.seekg(current_position,std::ios::beg);
	}

	IS_FILE_VALID();
	return (vfs::size_t)size;
}

bool vfs::CBufferFile::isOpenRead()
{
	return m_isOpen_read;
}

bool vfs::CBufferFile::openRead()
{
	if(!m_buffer.good())
	{
		return false;
	}
	return m_isOpen_read = true;
}
vfs::size_t vfs::CBufferFile::read(vfs::Byte* data, vfs::size_t bytesToRead)
{
	if(!m_buffer.eof())
	{
		IS_FILE_VALID();
		VFS_THROW_IFF(m_isOpen_read || this->openRead(), ERROR_FILE(L"open error"));

		m_buffer.read(static_cast<Byte*>(data), bytesToRead);
		std::streamsize bytesRead = m_buffer.gcount();

		VFS_THROW_IFF( m_buffer.good() || m_buffer.eof(), ERROR_FILE(L"read error") );
		return (vfs::size_t)bytesRead;
	}
	return 0;
}

vfs::size_t vfs::CBufferFile::getReadPosition()
{
	IS_FILE_VALID();
	VFS_THROW_IFF(m_isOpen_read || this->openRead(), ERROR_FILE(L"open error"));

	return (vfs::size_t)m_buffer.tellg();
}

void vfs::CBufferFile::setReadPosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_read || this->openRead(), ERROR_FILE(L"open error") );

	m_buffer.seekg((std::streamoff)positionInBytes);

	IS_FILE_VALID();
}
void vfs::CBufferFile::setReadPosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_read || this->openRead(), ERROR_FILE(L"open error") );

	std::ios::seekdir ioSeekDir;
	VFS_TRYCATCH_RETHROW(ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error"));
	m_buffer.seekg(offsetInBytes, ioSeekDir);

	IS_FILE_VALID();
}

bool vfs::CBufferFile::isOpenWrite()
{
	return m_isOpen_write;
}

bool vfs::CBufferFile::openWrite(bool bCreateWhenNotExist, bool bTruncate)
{
	if( !m_buffer.good() )
		return false;
	if( m_isOpen_write )
		return true;

	if(bTruncate)
	{
		m_buffer.str("");
		m_buffer.clear();
	}
	m_isOpen_write = m_buffer.good();
	return m_isOpen_write;
}

vfs::size_t vfs::CBufferFile::write(const vfs::Byte* data, vfs::size_t bytesToWrite)
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	std::streampos start = 0;
	if(!m_buffer.str().empty())
	{
		start = m_buffer.tellp();
	}
	VFS_THROW_IFF( m_buffer.write(data, bytesToWrite), ERROR_FILE(L"write error") );
	std::streampos bytesWritten = m_buffer.tellp() - start;

	IS_FILE_VALID();

	return (vfs::size_t)bytesWritten;
}

vfs::size_t vfs::CBufferFile::getWritePosition()
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	return (vfs::size_t)m_buffer.tellp();
}

void vfs::CBufferFile::setWritePosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	m_buffer.seekp((std::streamoff)positionInBytes);

	IS_FILE_VALID();
}

void vfs::CBufferFile::setWritePosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	VFS_THROW_IFF( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	std::ios::seekdir ioSeekDir;
	VFS_TRYCATCH_RETHROW( ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error") );
	m_buffer.seekp(offsetInBytes, ioSeekDir);

	IS_FILE_VALID();
}

void vfs::CBufferFile::copyToBuffer(vfs::tReadableFile& rFile)
{
	try
	{
		bool needToClose = !rFile.isOpenRead();
		vfs::COpenReadFile readfile(&rFile);
		if(!needToClose)
		{
			readfile.release();
		}

		typedef std::vector<vfs::Byte> tByteVector;

		vfs::size_t size = rFile.getSize();
		if(size > 0)
		{
			tByteVector vBuffer(size);

			rFile.read(&vBuffer[0], size);
			this->write(&vBuffer[0], size);
		}
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(ERROR_FILE(L""), ex);
	}
}


bool vfs::CBufferFile::deleteFile()
{
	m_buffer.clear();
	m_buffer.str("");
	return m_buffer.good();
}
