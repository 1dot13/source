/* 
 * bfVFS : vfs/Core/Location/vfs_lib_dir.cpp
 *  - class for readonly (sub)directories in archives/libraries
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

#include <vfs/Core/Location/vfs_lib_dir.h>
#include <vfs/Tools/vfs_log.h>

class vfs::CLibDirectory::IterImpl : public vfs::IBaseLocation::Iterator::IImplementation
{
	typedef vfs::IBaseLocation::Iterator::IImplementation tBaseClass;
public:
	IterImpl(CLibDirectory& lib);
	virtual ~IterImpl();
	virtual vfs::CLibDirectory::tFileType*			value();
	virtual void									next();
protected:
	virtual tBaseClass*								clone();
private:
	void operator=(vfs::CLibDirectory::IterImpl const& iter);

	vfs::CLibDirectory&								_lib;
	vfs::CLibDirectory::tFileCatalogue::iterator	_iter;
};

vfs::CLibDirectory::IterImpl::IterImpl(CLibDirectory& lib)
: tBaseClass(), _lib(lib)
{
	_iter = _lib.m_files.begin();
}

vfs::CLibDirectory::IterImpl::~IterImpl()
{
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::IterImpl::value()
{
	if(_iter != _lib.m_files.end())
	{
		return _iter->second;
	}
	return NULL;
}

void vfs::CLibDirectory::IterImpl::next()
{
	if(_iter != _lib.m_files.end())
	{
		_iter++;
	}
}

vfs::CLibDirectory::IterImpl::tBaseClass* vfs::CLibDirectory::IterImpl::clone()
{
	IterImpl* iter = new IterImpl(_lib);
	iter->_iter = _iter;
	return iter;
}

/***************************************************************************/
/***************************************************************************/

vfs::CLibDirectory::CLibDirectory(vfs::Path const& sLocalPath, vfs::Path const& sRealPath)
: tBaseClass(sLocalPath,sRealPath)
{
}

vfs::CLibDirectory::~CLibDirectory()
{
	tFileCatalogue::iterator it = m_files.begin();
	for(; it != m_files.end(); ++it)
	{
		// don't delete objects here
		//delete it->second;
	}
	m_files.clear();
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::addFile(vfs::Path const& filename, bool deleteOldFile)
{
	return NULL;
}

bool vfs::CLibDirectory::addFile(tFileType* file, bool deleteOldFile)
{
	if(!file)
	{
		return false;
	}
	vfs::Path const& name = file->getName();
	tFileType* oldFile = m_files[name];
	if(oldFile && (oldFile != file) )
	{
		if(deleteOldFile)
		{
			delete oldFile;
			m_files[name] = file;
		}
		else
		{
			return false;
		}
	}
	m_files[name] = file;
	return true;
}

bool vfs::CLibDirectory::deleteDirectory(vfs::Path const& dirPath)
{
	//if( !(m_mountPoint == dirPath) )
	//{
	//	return false;
	//}
	//if(implementsWritable())
	//{
	//	tFileCatalogue::iterator it = m_files.begin();
	//	for(; it != m_files.end(); ++it)
	//	{
	//		//delete it->second;
	//	}
	//	m_files.clear();
	//	return true;
	//}
	VFS_LOG_ERROR(L"called 'deleteDirectory', 'vfs::CLibDirectory' doesn't implement the IWritable interface");
	return false;
}

bool vfs::CLibDirectory::deleteFileFromDirectory(vfs::Path const& filename)
{
	//if(implementsWritable())
	//{
	//	tFileCatalogue::iterator it = m_files.find(filename);
	//	if(it != m_files.end())
	//	{
	//		delete it->second;
	//		m_files.erase(it);
	//		return true;
	//	}
	//}
	VFS_LOG_ERROR(L"called 'deleteFileFromDirectory', 'vfs::CLibDirectory' doesn't implement the IWritable interface");
	return false;
}

bool vfs::CLibDirectory::fileExists(vfs::Path const& filename)
{
	return (m_files[filename] != NULL);
}

vfs::IBaseFile*	vfs::CLibDirectory::getFile(vfs::Path const& filename)
{
	return getFileTyped(filename);
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::getFileTyped(vfs::Path const& filename)
{
	tFileCatalogue::iterator it =  m_files.find(filename);
	if(it != m_files.end())
	{
		return it->second;
	}
	return NULL;
}

bool vfs::CLibDirectory::createSubDirectory(vfs::Path const& subDirPath)
{
	// libraries are readonly
	return false;
}

void vfs::CLibDirectory::getSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	// nothing
}

vfs::CLibDirectory::Iterator vfs::CLibDirectory::begin()
{
	return Iterator(new IterImpl(*this));
}

/***************************************************************************/
/***************************************************************************/
