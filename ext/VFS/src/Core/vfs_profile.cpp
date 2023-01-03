/* 
 * bfVFS : vfs/Core/vfs_profile.cpp
 *  - Virtual Profile, container for real file system locations or archives
 *  - Profile Stack, orders profiles in a linear fashion (top-bottom)
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

#include <vfs/Core/vfs_profile.h>
#include <vfs/Core/vfs.h>
#include <vfs/Core/Location/vfs_lib_dir.h>
#include <vfs/Core/Location/vfs_directory_tree.h>
#include <vfs/Tools/vfs_log.h>
#include <vfs/Tools/vfs_parser_tools.h>

#include <sstream>


class vfs::CVirtualProfile::IterImpl : public vfs::CVirtualProfile::Iterator::IImplementation
{
	friend class vfs::CVirtualProfile;
	typedef vfs::CVirtualProfile::Iterator::IImplementation tBaseClass;

	IterImpl(CVirtualProfile* profile) : tBaseClass(), m_profile(profile)
	{
		VFS_THROW_IFF(profile, L"");
		// only unique locations
		_loc_iter = m_profile->m_setLocations.begin();
	}
public:
	IterImpl() : tBaseClass(), m_profile(NULL)
	{};
	virtual ~IterImpl()
	{};
	//////
	virtual vfs::IBaseLocation*		value()
	{
		if(_loc_iter != m_profile->m_setLocations.end())
		{
			return *_loc_iter;
		}
		return NULL;
	}
	virtual void					next()
	{
		if(_loc_iter != m_profile->m_setLocations.end())
		{
			_loc_iter++;
		}
	}
protected:
	virtual tBaseClass* clone()
	{
		IterImpl* iter = new IterImpl();
		iter->m_profile = m_profile;
		iter->_loc_iter = _loc_iter;
		return iter;
	}
private:
	vfs::CVirtualProfile*						m_profile;
	vfs::CVirtualProfile::tUniqueLoc::iterator	_loc_iter;
};

/***************************************************************************/
/***************************************************************************/

class vfs::CVirtualProfile::FileIterImpl : public vfs::CVirtualProfile::FileIterator::IImplementation
{
	friend class vfs::CVirtualProfile;
	typedef vfs::CVirtualProfile::FileIterator::IImplementation tBaseClass;
	FileIterImpl(vfs::Path const& sPattern, CVirtualProfile* profile);

public:
	FileIterImpl() : tBaseClass(), m_profile(NULL)
	{};
	virtual ~FileIterImpl()
	{};
	/////
	virtual vfs::IBaseFile*			value()
	{
		return file;
	}
	virtual void					next();
protected:
	virtual tBaseClass* clone()
	{
		FileIterImpl* iter2 = new FileIterImpl();
		iter2->m_pattern = m_pattern;
		iter2->m_profile = m_profile;
		iter2->iter = iter;
		iter2->fiter = fiter;
		iter2->file = file;
		return iter2;
	}
private:
	vfs::Path						m_pattern;
	vfs::CVirtualProfile*			m_profile;
	vfs::CVirtualProfile::Iterator	iter;
	vfs::IBaseLocation::Iterator	fiter;
	vfs::IBaseFile*					file;
};

vfs::CVirtualProfile::FileIterImpl::FileIterImpl(vfs::Path const& sPattern, CVirtualProfile* profile)
: tBaseClass(), m_pattern(sPattern), m_profile(profile)
{
	VFS_THROW_IFF(profile, L"");
	iter = m_profile->begin();
	while(!iter.end())
	{
		fiter = iter.value()->begin();
		while(!fiter.end())
		{
			file = fiter.value();
			if( matchPattern(m_pattern(), file->getPath()()) )
			{
				return;
			}
			fiter.next();
		}
		iter.next();
	}
	file = NULL;
}

void vfs::CVirtualProfile::FileIterImpl::next()
{
	if(!fiter.end())
	{
		fiter.next();
	}
	while(!iter.end())
	{
		while(!fiter.end())
		{
			file = fiter.value();
			if( matchPattern(m_pattern(), file->getPath()()) )
			{
				return;
			}
			fiter.next();
		}
		iter.next();
		if(!iter.end())
		{
			fiter = iter.value()->begin();
		}
	}
	file = NULL;
}

/***************************************************************************/
/***************************************************************************/

vfs::CVirtualProfile::CVirtualProfile(vfs::String const& profile_name, vfs::Path profile_root, bool writable)
: cName(profile_name), cRoot(profile_root), cWritable(writable) 
{
}

vfs::CVirtualProfile::~CVirtualProfile()
{
	tUniqueLoc::iterator it = m_setLocations.begin();
	for(; it != m_setLocations.end(); ++it)
	{
		delete (*it);
		//(*it) = NULL;
	}
	m_setLocations.clear();
	m_mapLocations.clear();
};

vfs::CVirtualProfile::Iterator vfs::CVirtualProfile::begin()
{
	return Iterator(new IterImpl(this));
}

vfs::CVirtualProfile::FileIterator vfs::CVirtualProfile::files(vfs::Path const& sPattern)
{
	return FileIterator( new FileIterImpl(sPattern, this));
}

void vfs::CVirtualProfile::addLocation(vfs::IBaseLocation* pLoc)
{
	if(pLoc)
	{
		m_setLocations.insert(pLoc);
		std::list<vfs::Path> lDirs;
		pLoc->getSubDirList(lDirs);
		std::list<vfs::Path>::const_iterator cit = lDirs.begin();
		for(;cit != lDirs.end(); ++cit)
		{
			vfs::IBaseLocation *pOldLoc = m_mapLocations[*cit];
			if(!pOldLoc)
			{
				m_mapLocations[*cit] = pLoc;
			}
			else if(pOldLoc == pLoc)
			{
				// seems to be an update. do nothing
			}
			else
			{
				VFS_LOG_WARNING((L"Another location is already mapped to '" + ((*cit)()) + L"' [keeping old location]").c_str());
				//VFS_THROW(L"Location already taken");
			}
		}
	}
}

vfs::IBaseLocation* vfs::CVirtualProfile::getLocation(vfs::Path const& sPath) const
{
	tLocations::const_iterator it = m_mapLocations.find(sPath);
	if(it != m_mapLocations.end())
	{
		return it->second;
	}
	return NULL;
}

vfs::IBaseFile* vfs::CVirtualProfile::getFile(vfs::Path const& sPath) const
{
	vfs::Path sDir,sFile;
	sPath.splitLast(sDir,sFile);
	tLocations::const_iterator it = m_mapLocations.find(sDir);
	if(it != m_mapLocations.end())
	{
		return it->second->getFile(sPath);
	}
	return NULL;
}


/***************************************************************************/
/***************************************************************************/

class vfs::CProfileStack::IterImpl : public vfs::CProfileStack::Iterator::IImplementation
{
	friend class CProfileStack;
	typedef CProfileStack::Iterator::IImplementation tBaseClass;

	IterImpl(CProfileStack* pPStack) : tBaseClass(), m_pPStack(pPStack)
	{
		VFS_THROW_IFF(m_pPStack, L"");
		_prof_iter = m_pPStack->m_profiles.begin();
	}
public:
	IterImpl() : tBaseClass(), m_pPStack(NULL)
	{};
	~IterImpl()
	{};
	//////
	virtual vfs::CVirtualProfile*	value()
	{
		if(_prof_iter != m_pPStack->m_profiles.end())
		{
			return *_prof_iter;
		}
		return NULL;
	}
	virtual void					next()
	{
		if(_prof_iter != m_pPStack->m_profiles.end())
		{
			_prof_iter++;
		}
	}
protected:
	virtual tBaseClass* clone()
	{
		IterImpl* iter = new IterImpl();
		iter->m_pPStack = m_pPStack;
		iter->_prof_iter = _prof_iter;
		return iter;
	}
private:
	vfs::CProfileStack*							m_pPStack;
	std::list<vfs::CVirtualProfile*>::iterator	_prof_iter;
};

/***************************************************************************/
/***************************************************************************/
vfs::CProfileStack::CProfileStack()
{
}

vfs::CProfileStack::~CProfileStack()
{
	t_profiles::iterator it = m_profiles.begin();
	for(; it != m_profiles.end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}
	m_profiles.clear();
}

vfs::CVirtualProfile* vfs::CProfileStack::getProfile(vfs::String const& sName) const
{
	t_profiles::const_iterator it = m_profiles.begin();
	for(;it != m_profiles.end(); ++it)
	{
		if( StrCmp::EqualCase((*it)->cName, sName) )
		{
			return *it;
		}
	}
	return NULL;
}

vfs::CVirtualProfile* vfs::CProfileStack::getWriteProfile()
{
	t_profiles::const_iterator it = m_profiles.begin();
	for(;it != m_profiles.end(); ++it)
	{
		if((*it)->cWritable)
		{
			return *it;
		}
	}
	return NULL;
}

vfs::CVirtualProfile* vfs::CProfileStack::topProfile() const
{
	if(!m_profiles.empty())
	{
		return m_profiles.front();
	}
	return NULL;
}

bool vfs::CProfileStack::popProfile()
{
	// there might be some files in this profile that are referenced in a Log object
	// we need to it to release the file
	vfs::Log::flushReleaseAll();
	// an observer pattern would probably be the better solution,
	// but for now lets do it this way 

	vfs::CVirtualProfile* prof = this->topProfile();
	if(prof)
	{
		vfs::CVirtualProfile::Iterator loc_it = prof->begin();
		for(; !loc_it.end(); loc_it.next())
		{
			vfs::IBaseLocation* loc = loc_it.value();
			vfs::IBaseLocation::Iterator f_it = loc->begin();
			for(; !f_it.end(); f_it.next())
			{
				vfs::IBaseFile* file = f_it.value();
				vfs::Path sDir, sFile;
				if(file)
				{
					file->getPath().splitLast(sDir,sFile);
					vfs::CVirtualLocation* vloc = getVFS()->getVirtualLocation(sDir);
					if(vloc)
					{
						if( !vloc->removeFile(file) )
						{
							VFS_THROW(_BS(L"Could not remove file [") << file->getPath()
								<< L"] in Profile [" << prof->cName << L"]" << _BS::wget);
						}
					}
					else
					{
						VFS_THROW(_BS(L"Virtual location [") << sDir
							<< L"] doesn't exist. Maybe the VFS was not properly setup." << _BS::wget);
					}
				}
				else
				{
					VFS_THROW(_BS(L"File is NULL during iteration over files in location [")
						<< loc->getPath() << L"]" << _BS::wget);
				}
			}
		}
		// delete only when nothing went wrong
		this->m_profiles.pop_front();
		delete prof;
	}
	return true;
}

void vfs::CProfileStack::pushProfile(CVirtualProfile* pProfile)
{
	if(!getProfile(pProfile->cName))
	{
		if(pProfile->cWritable)
		{
			m_profiles.push_front(pProfile);
		}
		else
		{
			t_profiles::iterator pit = m_profiles.begin();
			while(pit != m_profiles.end() && (*pit)->cWritable)
			{
				pit++;
			}
			//if(pit != m_profiles.end())
			{
				m_profiles.insert(pit,pProfile);
			}
			//else
			//{
			//	m_profiles.push_front(pProfile);
			//}
		}
		return;
	}
	VFS_THROW(L"A profile with this name already exists");
}

vfs::CProfileStack::Iterator vfs::CProfileStack::begin()
{
	return Iterator(new IterImpl(this));
}

