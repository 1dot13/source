/* 
 * bfVFS : vfs/Core/vfs_vloc.cpp
 *  - Virtual Location, stores Virtual Files
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

#include <vfs/Core/vfs_vloc.h>
#include <vfs/Core/vfs_vfile.h>
#include <vfs/Core/vfs_profile.h>
#include <vfs/Core/vfs.h>

/************************************************************************/

class vfs::CVirtualLocation::VFileIterator : public vfs::CVirtualLocation::Iterator::IImplementation
{
	friend class vfs::CVirtualLocation;
	typedef vfs::CVirtualLocation::Iterator::IImplementation tBaseClass;

	VFileIterator(vfs::CVirtualLocation* pLoc): tBaseClass(), m_pLoc(pLoc)
	{
		VFS_THROW_IFF(pLoc, L"");
		_vfile_iter = m_pLoc->m_VFiles.begin();
	}
public:
	VFileIterator() : tBaseClass(), m_pLoc(NULL)
	{};
	virtual ~VFileIterator()
	{};
	virtual vfs::CVirtualFile*	value()
	{
		if(m_pLoc && _vfile_iter != m_pLoc->m_VFiles.end())
		{
			return _vfile_iter->second;
		}
		return NULL;
	}
	virtual void				next()
	{
		if(m_pLoc && _vfile_iter != m_pLoc->m_VFiles.end())
		{
			_vfile_iter++;
		}
	}
protected:
	virtual tBaseClass* clone()
	{
		VFileIterator* iter = new VFileIterator(m_pLoc);
		iter->_vfile_iter = _vfile_iter;
		return iter;
	}
private:
	vfs::CVirtualLocation*						m_pLoc;
	vfs::CVirtualLocation::tVFiles::iterator	_vfile_iter;
};

/************************************************************************/

vfs::CVirtualLocation::CVirtualLocation(vfs::Path const& path)
: cPath(path), m_exclusive(false)
{};

vfs::CVirtualLocation::~CVirtualLocation()
{
	tVFiles::iterator it = m_VFiles.begin();
	for(; it != m_VFiles.end(); ++it)
	{
		it->second->destroy();
	}
	m_VFiles.clear();
}

void vfs::CVirtualLocation::setIsExclusive(bool exclusive)
{
	m_exclusive = exclusive;
}
bool vfs::CVirtualLocation::getIsExclusive()
{
	return m_exclusive;
}

void vfs::CVirtualLocation::addFile(vfs::IBaseFile* file, vfs::String const& profileName)
{
	vfs::CVirtualFile *pVFile = NULL;
	tVFiles::iterator it = m_VFiles.find(file->getName());
	if(it == m_VFiles.end())
	{
		vfs::Path fp = file->getPath();
		vfs::CProfileStack& stack = *(getVFS()->getProfileStack());
		pVFile = vfs::CVirtualFile::create(fp,stack);
		it = m_VFiles.insert(m_VFiles.end(), std::pair<vfs::Path,vfs::CVirtualFile*>(file->getName(),pVFile));
	}
	it->second->add(file,profileName,true);
}

vfs::IBaseFile* vfs::CVirtualLocation::getFile(vfs::Path const& filename, vfs::String const& profileName) const
{
	tVFiles::const_iterator cit = m_VFiles.find(filename);
	if(cit != m_VFiles.end() && cit->second)
	{
		if(profileName.empty())
		{
			if(m_exclusive)
			{
				return cit->second->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			}
			else
			{
				return cit->second->file(vfs::CVirtualFile::SF_TOP);
			}
		}
		else
		{
			// you know what you are doing
			return cit->second->file(profileName);
		}
	}
	return NULL;
}
vfs::CVirtualFile* vfs::CVirtualLocation::getVirtualFile(vfs::Path const& filename)
{
	tVFiles::const_iterator cit = m_VFiles.find(filename);
	if(cit != m_VFiles.end())
	{
		return cit->second;
	}
	return NULL;
}

bool vfs::CVirtualLocation::removeFile(vfs::IBaseFile* file)
{
	if(file)
	{
		vfs::Path sDir,sFile;
		file->getPath().splitLast(sDir,sFile);
		tVFiles::iterator it = m_VFiles.find(sFile);
		if(it != m_VFiles.end())
		{
			if(!it->second->remove(file))
			{
				//CVirtualFile* vfile = it->second;
				//delete vfile;
				m_VFiles.erase(it);
			}
			return true;
		}
	}
	return false;
}


vfs::CVirtualLocation::Iterator vfs::CVirtualLocation::iterate()
{
	return Iterator(new VFileIterator(this));
}

