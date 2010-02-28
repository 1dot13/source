#include "vfs_profile.h"
#include "vfs.h"
#include "Location/vfs_lib_dir.h"
#include "Location/vfs_directory_tree.h"

#include "Tools/Log.h"

#include <sstream>


class vfs::CVirtualProfile::IterImpl : public vfs::CVirtualProfile::Iterator::IImplementation
{
	friend class vfs::CVirtualProfile;
	typedef vfs::CVirtualProfile::Iterator::IImplementation tBaseClass;

	IterImpl(CVirtualProfile* profile) : tBaseClass(), m_profile(profile)
	{
		THROWIFFALSE(profile, L"");
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
	THROWIFFALSE(profile, L"");
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

vfs::CVirtualProfile::CVirtualProfile(utf8string const& sProfileName, bool bWritable)
: cName(sProfileName), cWritable(bWritable)
{};

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
			vfs::IBaseLocation *pNewLoc = m_mapLocations[*cit];
			if(!pNewLoc)
			{
				m_mapLocations[*cit] = pLoc;
			}
			else if(pNewLoc == pLoc)
			{
				// seems to be an update. do nothing
			}
			else
			{
				THROWEXCEPTION(L"Location already taken");
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
		THROWIFFALSE(m_pPStack, L"");
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
	std::list<CVirtualProfile*>::iterator it = m_profiles.begin();
	for(; it != m_profiles.end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}
	m_profiles.clear();
}

vfs::CVirtualProfile* vfs::CProfileStack::getProfile(utf8string const& sName) const
{
	std::list<CVirtualProfile*>::const_iterator it = m_profiles.begin();
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
	std::list<vfs::CVirtualProfile*>::const_iterator it = m_profiles.begin();
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
	// there might be some files in this profile that are referenced in a CLog object
	// we need to it to release the file
	CLog::flushAll();
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
							std::wstringstream wss;
							wss << L"Could not remove file ["
								<< file->getPath()()
								<< L"] in Profile ["
								<< prof->cName << L"]";
							THROWEXCEPTION(wss.str().c_str());
						}
					}
					else
					{
						std::wstringstream wss;
						wss << L"Virtual location [" << sDir() << L"] doesn't exist. Maybe the VFS was not properly setup.";
						THROWEXCEPTION(wss.str().c_str());
					}
				}
				else
				{
					std::wstringstream wss;
					wss << L"File is NULL during iteration over files in location [" << loc->getPath()() << L"]";
					THROWEXCEPTION(wss.str().c_str());
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
		m_profiles.push_front(pProfile);
		return;
	}
	THROWEXCEPTION(L"A profile with this name already exists");
}

vfs::CProfileStack::Iterator vfs::CProfileStack::begin()
{
	return Iterator(new IterImpl(this));
}

