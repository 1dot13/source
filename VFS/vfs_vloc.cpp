#include "vfs_vloc.h"
#include "vfs_vfile.h"
#include "vfs_profile.h"
#include "vfs.h"

/************************************************************************/

vfs::CVirtualLocation::Iterator::Iterator(CVirtualLocation* pLoc)
: m_pLoc(pLoc)
{
	_vfile_iter = m_pLoc->m_mapVFiles.begin();
}

vfs::CVirtualLocation::Iterator::Iterator()
: m_pLoc(NULL)
{
}

vfs::CVirtualLocation::Iterator::~Iterator()
{
}

vfs::CVirtualFile* vfs::CVirtualLocation::Iterator::value()
{
	if(m_pLoc && _vfile_iter != m_pLoc->m_mapVFiles.end())
	{
		return _vfile_iter->second;
	}
	return NULL;
}

void vfs::CVirtualLocation::Iterator::next()
{
	if(m_pLoc && _vfile_iter != m_pLoc->m_mapVFiles.end())
	{
		_vfile_iter++;
	}
}

bool vfs::CVirtualLocation::Iterator::end()
{
	if(m_pLoc)
	{
		return _vfile_iter == m_pLoc->m_mapVFiles.end();
	}
	return true;
}

/************************************************************************/

vfs::CVirtualLocation::CVirtualLocation(vfs::Path const& sPath)
: Path(sPath), m_bExclusive(false)
{};

vfs::CVirtualLocation::~CVirtualLocation()
{
	tVFiles::iterator it = m_mapVFiles.begin();
	for(; it != m_mapVFiles.end(); ++it)
	{
		it->second->Destroy();
	}
	m_mapVFiles.clear();
}

void vfs::CVirtualLocation::SetIsExclusive(bool bExclusive)
{
	m_bExclusive = bExclusive;
}
bool vfs::CVirtualLocation::GetIsExclusive()
{
	return m_bExclusive;
}

void vfs::CVirtualLocation::AddFile(vfs::IBaseFile* pFile, utf8string const& sProfileName)
{
	tVFiles::iterator it = m_mapVFiles.find(pFile->GetFileName());
	CVirtualFile *pVFile = NULL;
	if(it == m_mapVFiles.end())
	{
		vfs::Path& fp = pFile->GetFullPath();
		vfs::CProfileStack& stack = *(GetVFS()->GetProfileStack());
		pVFile = vfs::CVirtualFile::Create(fp,stack);
		it = m_mapVFiles.insert(m_mapVFiles.end(), std::pair<vfs::Path,vfs::CVirtualFile*>(pFile->GetFileName(),pVFile));
	}
	it->second->Add(pFile,sProfileName,true);
}

vfs::IBaseFile* vfs::CVirtualLocation::GetFile(vfs::Path const& sFilename, utf8string const& sProfileName) const
{
	tVFiles::const_iterator cit = m_mapVFiles.find(sFilename);
	if(cit != m_mapVFiles.end() && cit->second)
	{
		if(sProfileName.empty())
		{
			if(m_bExclusive)
			{
				return cit->second->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
			}
			else
			{
				return cit->second->File(vfs::CVirtualFile::SF_TOP);
			}
		}
		else
		{
			// you know what you are doing
			return cit->second->File(sProfileName);
		}
	}
	return NULL;
}
vfs::CVirtualFile* vfs::CVirtualLocation::GetVFile(vfs::Path const& sFilename)
{
	tVFiles::const_iterator cit = m_mapVFiles.find(sFilename);
	if(cit != m_mapVFiles.end())
	{
		return cit->second;
	}
	return NULL;
}

bool vfs::CVirtualLocation::RemoveFile(vfs::IBaseFile* pFile)
{
	if(pFile)
	{
		vfs::Path sDir,sFile;
		pFile->GetFullPath().SplitLast(sDir,sFile);
		tVFiles::iterator it = m_mapVFiles.find(sFile);
		if(it != m_mapVFiles.end())
		{
			if(!it->second->Remove(pFile))
			{
				CVirtualFile* vfile = it->second;
				//delete vfile;
				m_mapVFiles.erase(it);
			}
			return true;
		}
	}
	return false;
}


vfs::CVirtualLocation::Iterator vfs::CVirtualLocation::iterate()
{
	return Iterator(this);
}

