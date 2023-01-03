/* 
 * bfVFS : vfs/Core/vfs_file_raii.cpp
 *  - RAII classes to open files for reading/writing
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

#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs.h>

#include <sstream>
/********************************************************************************************/
/********************************************************************************************/

vfs::COpenReadFile::COpenReadFile(vfs::Path const& sPath, vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::IBaseFile *pFile = getVFS()->getFile(sPath,eSF);
	VFS_THROW_IFF(pFile, _BS(L"file \"") << sPath << L"\" does not exist" << _BS::wget);
	
	m_pFile = vfs::tReadableFile::cast(pFile);
	
	VFS_THROW_IFF(m_pFile, _BS(L"File \"") << sPath << L"\" is not readable" << _BS::wget);
	
	VFS_THROW_IFF(m_pFile->openRead(), _BS(L"Could not open file : ") << m_pFile->getPath() << _BS::wget);
}

vfs::COpenReadFile::COpenReadFile(vfs::tReadableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		VFS_THROW_IFF(m_pFile, L"Invalid file object");

		VFS_THROW_IFF(m_pFile->openRead(), _BS(L"Could not open file : ") << pFile->getPath() << _BS::wget);
	}
	catch(std::exception &ex)
	{
		VFS_RETHROW(L"",ex);
	}
}
vfs::COpenReadFile::~COpenReadFile()
{
	if(m_pFile)
	{
		m_pFile->close();
		m_pFile = NULL;
	}
}

vfs::tReadableFile* vfs::COpenReadFile::operator->()
{
	return m_pFile;
}

vfs::tReadableFile& vfs::COpenReadFile::file()
{
	return *m_pFile;
}

void vfs::COpenReadFile::release()
{
	m_pFile = NULL;
}

/**************************************************************************/

vfs::COpenWriteFile::COpenWriteFile(vfs::Path const& sPath, 
									bool bCreate, 
									bool bTruncate, 
									vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::IBaseFile *pFile = getVFS()->getFile(sPath,eSF);
	if(!pFile && bCreate)
	{
		if(getVFS()->createNewFile(sPath))
		{
			pFile = getVFS()->getFile(sPath,eSF);
		}
		else
		{
			VFS_THROW(_BS(L"Could not create VFS file \"") << sPath << L"\"" << _BS::wget);
		}
	}
	VFS_THROW_IFF(pFile, _BS(L"File \"") << sPath << L"\" not found" << _BS::wget);

	m_pFile = vfs::tWritableFile::cast(pFile);

	VFS_THROW_IFF(m_pFile, _BS(L"File \"") << sPath << L"\" exists, but is not writable" << _BS::wget);
	
	VFS_THROW_IFF(m_pFile->openWrite(bCreate,bTruncate), _BS(L"File \"") << sPath << L"\" could not be opened for writing" << _BS::wget);
}
vfs::COpenWriteFile::COpenWriteFile(vfs::tWritableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		VFS_THROW_IFF(m_pFile, L"Invalid file object");
		VFS_THROW_IFF(m_pFile->openWrite(true,false), _BS(L"Could not open file : ") << m_pFile->getPath() << _BS::wget);
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"",ex);
	};
}
vfs::COpenWriteFile::~COpenWriteFile()
{
	if(m_pFile)
	{
		m_pFile->close();
		m_pFile = NULL;
	}
}

vfs::tWritableFile* vfs::COpenWriteFile::operator->()
{
	return m_pFile;
}

vfs::tWritableFile& vfs::COpenWriteFile::file()
{
	return *m_pFile;
}

void vfs::COpenWriteFile::release()
{
	m_pFile = NULL;
}

