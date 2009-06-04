#include "vfs_profile.h"
#include "vfs.h"
#include "Location/vfs_lib_dir.h"
#include "Location/vfs_directory_tree.h"

#include "PropertyContainer.h"

#include <sstream>

vfs::CVirtualProfile::Iterator::Iterator(vfs::CVirtualProfile& rProf)
: m_pProf(&rProf)
{
	// only unique locations
	_loc_iter = m_pProf->m_setLocations.begin();
}

vfs::CVirtualProfile::Iterator::Iterator()
: m_pProf(NULL)
{}

vfs::CVirtualProfile::Iterator::~Iterator()
{}

vfs::IBaseLocation* vfs::CVirtualProfile::Iterator::value() const
{
	if(_loc_iter != m_pProf->m_setLocations.end())
	{
		return *_loc_iter;
	}
	THROWEXCEPTION(L"End of map");
}

void vfs::CVirtualProfile::Iterator::next()
{
	if(_loc_iter != m_pProf->m_setLocations.end())
	{
		_loc_iter++;
	}
	// silently ignore error
}
bool vfs::CVirtualProfile::Iterator::end() const
{
	return _loc_iter == m_pProf->m_setLocations.end();
}

/***************************************************************************/
/***************************************************************************/

vfs::CVirtualProfile::CVirtualProfile(utf8string const& sProfileName, bool bWriteable)
: Name(sProfileName), Writeable(bWriteable)
{};

vfs::CVirtualProfile::~CVirtualProfile()
{
	tUniqueLoc::iterator it = m_setLocations.begin();
	for(; it != m_setLocations.end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}
	m_setLocations.clear();
	m_mapLocations.clear();
};

vfs::CVirtualProfile::Iterator vfs::CVirtualProfile::begin()
{
	return Iterator(*this);
}
		
void vfs::CVirtualProfile::AddLocation(vfs::IBaseLocation* pLoc)
{
	if(pLoc)
	{
		m_setLocations.insert(pLoc);
		std::list<vfs::Path> lDirs;
		pLoc->GetSubDirList(lDirs);
		std::list<vfs::Path>::const_iterator cit = lDirs.begin();
		for(;cit != lDirs.end(); ++cit)
		{
			vfs::IBaseLocation *pNewLoc = m_mapLocations[*cit];
			if(!pNewLoc)
			{
				m_mapLocations[*cit] = pLoc;
			}
			else if(pNewLoc = pLoc)
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

vfs::IBaseLocation* vfs::CVirtualProfile::GetLocation(vfs::Path const& sPath) const
{
	tLocations::const_iterator it = m_mapLocations.find(sPath);
	if(it != m_mapLocations.end())
	{
		return it->second;
	}
	return NULL;
}

vfs::IBaseFile* vfs::CVirtualProfile::GetFile(vfs::Path const& sPath) const
{
	vfs::Path sDir,sFile;
	sPath.SplitLast(sDir,sFile);
	tLocations::const_iterator it = m_mapLocations.find(sDir);
	if(it != m_mapLocations.end())
	{
		return it->second->GetFile(sPath);
	}
	return NULL;
}


/***************************************************************************/
/***************************************************************************/

vfs::CProfileStack::Iterator::Iterator(CProfileStack& rPStack)
: m_pPStack(&rPStack)
{
	_prof_iter = m_pPStack->m_lProfiles.begin();
};

vfs::CProfileStack::Iterator::Iterator()
: m_pPStack(NULL)
{};

vfs::CProfileStack::Iterator::~Iterator()
{};

vfs::CVirtualProfile* vfs::CProfileStack::Iterator::value() const
{
	if(_prof_iter != m_pPStack->m_lProfiles.end())
	{
		return *_prof_iter;
	}
	THROWEXCEPTION(L"end of container");
}

void vfs::CProfileStack::Iterator::next()
{
	if(_prof_iter != m_pPStack->m_lProfiles.end())
	{
		_prof_iter++;
	}
	// silently ignore that we already are at the end of the list
}
bool vfs::CProfileStack::Iterator::end() const
{
	return _prof_iter == m_pPStack->m_lProfiles.end();
}

/***************************************************************************/
/***************************************************************************/
vfs::CProfileStack::CProfileStack()
{
}

vfs::CProfileStack::~CProfileStack()
{
	std::list<CVirtualProfile*>::iterator it = m_lProfiles.begin();
	for(; it != m_lProfiles.end(); ++it)
	{
		delete (*it);
		(*it) = NULL;
	}
	m_lProfiles.clear();
}

vfs::CVirtualProfile* vfs::CProfileStack::GetProfile(utf8string const& sName) const
{
	std::list<CVirtualProfile*>::const_iterator it = m_lProfiles.begin();
	for(;it != m_lProfiles.end(); ++it)
	{
		if( StrCmp::EqualCase((*it)->Name, sName) )
		{
			return *it;
		}
	}
	return NULL;
}

vfs::CVirtualProfile* vfs::CProfileStack::GetWriteProfile()
{
	std::list<CVirtualProfile*>::const_iterator it = m_lProfiles.begin();
	for(;it != m_lProfiles.end(); ++it)
	{
		if((*it)->Writeable)
		{
			return *it;
		}
	}
	return NULL;
}

vfs::CVirtualProfile* vfs::CProfileStack::TopProfile() const
{
	if(!m_lProfiles.empty())
	{
		return m_lProfiles.front();
	}
	return NULL;
}

bool vfs::CProfileStack::PopProfile()
{
	// there might be some files in this profile that are referenced in a CLog object
	// we need to it to release the file
	CLog::FlushAll();
	// an observer pattern would probably be the better solution,
	// but for now lets do it this way 

	bool bSuccess = true;
	vfs::CVirtualProfile* prof = this->TopProfile();
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
					file->GetFullPath().SplitLast(sDir,sFile);
					vfs::CVirtualLocation* vloc = GetVFS()->GetVirtualLocation(sDir);
					if(vloc)
					{
						if( !(bSuccess &= vloc->RemoveFile(file)) )
						{
							std::wstringstream wss;
							wss << L"Could not remove file ["
								<< file->GetFullPath()()
								<< L"] in Profile ["
								<< prof->Name << L"]";
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
					wss << L"File is NULL during iteration over files in location [" << loc->GetFullPath()() << L"]";
					THROWEXCEPTION(wss.str().c_str());
				}
			}
		}
		if(bSuccess)
		{
			// delete only when nothing went wrong
			this->m_lProfiles.pop_front();
			delete prof;
		}
	}
	return bSuccess;
}

void vfs::CProfileStack::PushProfile(CVirtualProfile* pProfile)
{
	if(!GetProfile(pProfile->Name))
	{
		m_lProfiles.push_front(pProfile);
		return;
	}
	THROWEXCEPTION(L"A profile with this name already exists");
}

vfs::CProfileStack::Iterator vfs::CProfileStack::begin()
{
	return Iterator(*this);
}

