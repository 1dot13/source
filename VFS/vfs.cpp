#include "vfs_types.h"
#include "vfs.h"

#include "Interface/vfs_file_interface.h"
#include "File/vfs_file.h"
#include "File/vfs_dir_file.h"
#include "File/vfs_lib_file.h"
#include "vfs_file_raii.h"
#include "vfs_vfile.h"

#include "PropertyContainer.h"

#include <stack>


std::vector<BasicAllocator*> CFileAllocator::_valloc;

void CFileAllocator::RegisterAllocator(BasicAllocator* allocator)
{
	_valloc.push_back(allocator);
}
void CFileAllocator::Clear()
{
	std::vector<BasicAllocator*>::iterator it = _valloc.begin();
	for(; it != _valloc.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	_valloc.clear();
}


/********************************************************************/
/********************************************************************/

vfs::CVirtualFileSystem::CRegularIterator::CRegularIterator(vfs::CVirtualFileSystem& rVFS)
: vfs::CVirtualFileSystem::Iterator::IImplemetation(), m_rVFS(rVFS)
{
	_vloc_iter = m_rVFS.m_mapFS.begin();
	if(_vloc_iter != m_rVFS.m_mapFS.end())
	{
		_vfile_iter = _vloc_iter->second->iterate();
	}
}

vfs::CVirtualFileSystem::CRegularIterator::~CRegularIterator()
{
}

vfs::tReadableFile* vfs::CVirtualFileSystem::CRegularIterator::value()
{
	bool bExclusiveVLoc = false;
	if(_vloc_iter != m_rVFS.m_mapFS.end())
	{
		bExclusiveVLoc = _vloc_iter->second->GetIsExclusive();
	}
	if(!_vfile_iter.end())
	{
		vfs::CVirtualFile* pVFile = _vfile_iter.value();
		if(pVFile)
		{
			if(bExclusiveVLoc)
			{
				return vfs::tReadableFile::Cast(pVFile->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE));
			}
			else
			{
				return vfs::tReadableFile::Cast(pVFile->File(vfs::CVirtualFile::SF_TOP));
			}
		}
	}
	return NULL;
}
void vfs::CVirtualFileSystem::CRegularIterator::next()
{
	if(!_vfile_iter.end())
	{
		_vfile_iter.next();
	}
	while(_vfile_iter.end())
	{
		if(_vloc_iter != m_rVFS.m_mapFS.end())
		{
			_vloc_iter++;
			if(_vloc_iter != m_rVFS.m_mapFS.end())
			{
				_vfile_iter = _vloc_iter->second->iterate();
			}
		}
		else
		{
			return;
		}
	}
}


/********************************************************************/
/********************************************************************/

vfs::CVirtualFileSystem::CMatchingIterator::CMatchingIterator(vfs::Path const& sPattern, vfs::CVirtualFileSystem& rVFS)
: vfs::CVirtualFileSystem::Iterator::IImplemetation(), m_rVFS(rVFS)
{
	if(sPattern() == vfs::Const::STAR())
	{
		m_sLocPattern =  vfs::Path(vfs::Const::STAR());
		m_sFilePattern = vfs::Path(vfs::Const::STAR());
	}
	else
	{
		sPattern.SplitLast(m_sLocPattern,m_sFilePattern);
	}

	_vloc_iter = m_rVFS.m_mapFS.begin();
	while(_vloc_iter != m_rVFS.m_mapFS.end())
	{
		if( MatchPattern(m_sLocPattern(),_vloc_iter->second->Path()) )
		{
			bool bExclusiveVLoc = _vloc_iter->second->GetIsExclusive();
			_vfile_iter = _vloc_iter->second->iterate();
			while(!_vfile_iter.end())
			{
				vfs::IBaseFile* pFile = NULL;
				if(bExclusiveVLoc)
				{
					pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
				}
				else
				{
					pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_TOP);
				}
				if(pFile)
				{
					vfs::Path const& filename = pFile->GetFileName();
					if( MatchPattern(m_sFilePattern(),filename()) )
					{
						return;
					}
				}
				_vfile_iter.next();
			}
		}
		_vloc_iter++;
	}
}

vfs::CVirtualFileSystem::CMatchingIterator::~CMatchingIterator()
{
}

vfs::tReadableFile* vfs::CVirtualFileSystem::CMatchingIterator::value()
{
	bool bExclusiveVLoc = false;
	if( _vloc_iter != m_rVFS.m_mapFS.end() )
	{
		bExclusiveVLoc = _vloc_iter->second->GetIsExclusive();
	}
	if(!_vfile_iter.end())
	{
		vfs::CVirtualFile* pVFile = _vfile_iter.value();
		if(pVFile)
		{
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_TOP);
			}
			return vfs::tReadableFile::Cast(pFile);
		}
	}
	return NULL;
}

bool vfs::CVirtualFileSystem::CMatchingIterator::nextLocationMatch()
{
	while(_vloc_iter != m_rVFS.m_mapFS.end())
	{
		_vloc_iter++;
		if(_vloc_iter != m_rVFS.m_mapFS.end())
		{
			if(MatchPattern(m_sLocPattern(),_vloc_iter->second->Path()))
			{
				return true;
			}
		}
	}
	return false;
}
bool vfs::CVirtualFileSystem::CMatchingIterator::nextFileMatch()
{
	bool bExclusiveVLoc = false;
	if( _vloc_iter != m_rVFS.m_mapFS.end() )
	{
		bExclusiveVLoc = _vloc_iter->second->GetIsExclusive();
	}
	while(!_vfile_iter.end())
	{
		_vfile_iter.next();
		if(!_vfile_iter.end())
		{
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_TOP);
			}
			if(pFile)
			{
				vfs::Path const& filename = pFile->GetFileName();
				if(MatchPattern(m_sFilePattern(),filename()))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void vfs::CVirtualFileSystem::CMatchingIterator::next()
{
	if(nextFileMatch())
	{
		return;
	}
	while(nextLocationMatch())
	{
		_vfile_iter = _vloc_iter->second->iterate();
		if(!_vfile_iter.end())
		{
			bool bExclusiveVLoc = _vloc_iter->second->GetIsExclusive();
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->File(vfs::CVirtualFile::SF_TOP);
			}
			if(pFile && MatchPattern(m_sFilePattern(),pFile->GetFileName()()))
			{
				return;
			}
			else if(nextFileMatch())
			{
				return;
			}
		}
	}
}

/********************************************************************/
/********************************************************************/

vfs::CVirtualFileSystem::Iterator::Iterator()
: _iter_impl(NULL), _file(NULL)
{};

vfs::CVirtualFileSystem::Iterator::Iterator(vfs::CVirtualFileSystem::Iterator::IImplemetation* impl)
: _iter_impl(impl), _file(NULL)
{
	THROWIFFALSE(_iter_impl, L"EXCEPTION");
	_file = _iter_impl->value();
}

vfs::CVirtualFileSystem::Iterator::~Iterator()
{
}

vfs::tReadableFile* vfs::CVirtualFileSystem::Iterator::value()
{
	return _file;
};
void vfs::CVirtualFileSystem::Iterator::next()
{
	if(_iter_impl)
	{
		_iter_impl->next();
		_file = _iter_impl->value();
		if(!_file)
		{
			delete _iter_impl;
			_iter_impl = NULL;
		}
	}
}
bool vfs::CVirtualFileSystem::Iterator::end()
{
	return _file == NULL;
}

/********************************************************************/
/********************************************************************/

vfs::CVirtualFileSystem* GetVFS()
{
	return vfs::CVirtualFileSystem::GetVFS();
}

vfs::CVirtualFileSystem* vfs::CVirtualFileSystem::m_pSingleton = NULL;

vfs::CVirtualFileSystem* vfs::CVirtualFileSystem::GetVFS()
{
	if(!m_pSingleton)
	{
		m_pSingleton = new CVirtualFileSystem();
	}
	return m_pSingleton;
}

void vfs::CVirtualFileSystem::ShutdownVFS()
{
	if(m_pSingleton)
	{
		delete m_pSingleton;
		m_pSingleton = NULL;
	}
}


vfs::CVirtualFileSystem::CVirtualFileSystem()
{
}

vfs::CVirtualFileSystem::~CVirtualFileSystem()
{
	tVFS::iterator it = m_mapFS.begin();
	for(; it != m_mapFS.end(); ++it)
	{
		delete it->second;
	}
	m_mapFS.clear();
}

vfs::CProfileStack* vfs::CVirtualFileSystem::GetProfileStack()
{
	return &m_oProfileStack;
}

vfs::CVirtualFileSystem::Iterator vfs::CVirtualFileSystem::begin()
{
	return Iterator(new vfs::CVirtualFileSystem::CRegularIterator(*this));
}

vfs::CVirtualFileSystem::Iterator vfs::CVirtualFileSystem::begin(vfs::Path const& sPattern)
{
	return Iterator(new vfs::CVirtualFileSystem::CMatchingIterator(sPattern,*this));
}

bool vfs::CVirtualFileSystem::AddLocation(vfs::IBaseLocation* pLocation, utf8string const& sProfileName, bool bIsWriteable)
{
	vfs::CVirtualProfile *pProf = m_oProfileStack.GetProfile(sProfileName);
	if(!pProf)
	{
		pProf = new vfs::CVirtualProfile(sProfileName, bIsWriteable);
		m_oProfileStack.PushProfile(pProf);
	}
	pProf->AddLocation(pLocation);

	std::list<vfs::Path> lSubDirs;
	pLocation->GetSubDirList(lSubDirs);
	std::list<vfs::Path>::const_iterator sd_cit = lSubDirs.begin();
	for(;sd_cit != lSubDirs.end(); ++sd_cit)
	{
		this->GetVirtualLocation(*sd_cit, true);
	}

	vfs::IBaseLocation::Iterator it = pLocation->begin();
	for(; !it.end(); it.next())
	{
		vfs::IBaseFile *pFile = it.value();
		vfs::Path const& sPath = pFile->GetFullPath();
		vfs::Path dir,file;
		sPath.SplitLast(dir,file);
		
		CVirtualLocation* pLoc = this->GetVirtualLocation(dir,true);
		pLoc->AddFile(pFile,sProfileName);
	}
	return true;
}

vfs::tReadableFile* vfs::CVirtualFileSystem::GetRFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return vfs::tReadableFile::Cast(this->GetFile(rLocalFilePath,eSF));
}

vfs::tWriteableFile* vfs::CVirtualFileSystem::GetWFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return vfs::tWriteableFile::Cast(this->GetFile(rLocalFilePath,eSF));
}

vfs::IBaseFile* vfs::CVirtualFileSystem::GetFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::Path sDir,sFile;
	rLocalFilePath.SplitLast(sDir,sFile);

	vfs::CVirtualLocation* pVLoc = this->GetVirtualLocation(sDir);
	if(pVLoc)
	{
		vfs::CVirtualFile *pVFile = pVLoc->GetVFile(sFile);
		if(pVFile)
		{
			if(pVLoc->GetIsExclusive())
			{
				return pVFile->File(vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
			}
			return pVFile->File(eSF);
		}
	}
	return NULL;
}

vfs::tReadableFile* vfs::CVirtualFileSystem::GetRFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName)
{
	return vfs::tReadableFile::Cast(this->GetFile(rLocalFilePath, sProfileName));
}

vfs::tWriteableFile* vfs::CVirtualFileSystem::GetWFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName)
{
	return vfs::tWriteableFile::Cast(this->GetFile(rLocalFilePath, sProfileName));
}

vfs::IBaseFile* vfs::CVirtualFileSystem::GetFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName)
{
	vfs::Path sDir,sFile;
	rLocalFilePath.SplitLast(sDir,sFile);

	tVFS::iterator it_loc = m_mapFS.find(sDir);
	if(it_loc == m_mapFS.end())
	{
		return NULL;
	}
	vfs::CVirtualLocation* pVLoc = it_loc->second;
	if(pVLoc)
	{
		return pVLoc->GetFile(sFile,sProfileName);
	}
	return NULL;
}

bool vfs::CVirtualFileSystem::FileExists(vfs::Path const& rLocalFilePath, std::string const& sProfileName)
{
	return GetFile(rLocalFilePath, sProfileName) != NULL;
}

bool vfs::CVirtualFileSystem::FileExists(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return GetFile(rLocalFilePath, eSF) != NULL;
}

vfs::CVirtualLocation* vfs::CVirtualFileSystem::GetVirtualLocation(vfs::Path const& sPath, bool bCreate)
{
	tVFS::iterator it = m_mapFS.find(sPath);
	if(it == m_mapFS.end())
	{
		if(bCreate)
		{
			vfs::CVirtualLocation* pVLoc = new vfs::CVirtualLocation(sPath);
			m_mapFS.insert(std::make_pair(sPath,pVLoc));
			return pVLoc;
		}
		return NULL;
	}
	return it->second;
}

bool vfs::CVirtualFileSystem::RemoveDirectoryFromFS(vfs::Path const& sDir)
{
	vfs::Path pattern = sDir + "*";
	std::list<vfs::Path> files;

	Iterator it = this->begin(pattern);
	for(; !it.end(); it.next())
	{
		if(it.value()->IsWriteable())
		{
			files.push_back(it.value()->GetFullPath());
		}
	}
	bool success = true;
	std::list<vfs::Path>::iterator fit = files.begin();
	for(; fit != files.end(); ++fit)
	{
		success &= this->RemoveFileFromFS(*fit);
	}
	return success;
}

bool vfs::CVirtualFileSystem::RemoveFileFromFS(vfs::Path const& sFilePath)
{
	vfs::Path sPath,sFile;
	sFilePath.SplitLast(sPath,sFile);
	vfs::CVirtualProfile *pProf = m_oProfileStack.GetWriteProfile();
	if(pProf)
	{
		vfs::IBaseLocation *pBL = pProf->GetLocation(sPath);
		if(pBL && pBL->IsWriteable())
		{
			vfs::IDirectory<vfs::IWriteable> *pDir = pBL->Cast<vfs::IDirectory<vfs::IWriteable> >();
			if(pDir)
			{
				bool bSuccess = false;
				// remove file from virtual structures first
				vfs::IBaseFile* file = pDir->GetFile(sFilePath);
				if(file)
				{
					vfs::Path sDir,sFile;
					sFilePath.SplitLast(sDir,sFile);
					vfs::CVirtualLocation *pVLoc = this->GetVirtualLocation(sDir);
					if(pVLoc)
					{
						bSuccess = pVLoc->RemoveFile(file);
					}
				}
				// delete actual file
				return bSuccess && pDir->DeleteFileFromDirectory(sFilePath);
			}
		}
	}
	return false;
}

bool vfs::CVirtualFileSystem::CreateNewFile(vfs::Path const& sFileName)
{
	vfs::Path sPath,sFile;
	sFileName.SplitLast(sPath,sFile);
	vfs::CVirtualProfile *pProf = m_oProfileStack.GetWriteProfile();
	if(pProf)
	{
		bool bIsExclusive = false;
		bool bNewLocation = false;
		vfs::IBaseLocation *pProfLoc = pProf->GetLocation(sPath);
		if(!pProfLoc)
		{
			// try to find closest match
			vfs::Path sTemp, sCreateDir, sRight, sLeft = sPath;
			while(!pProfLoc && sLeft.SplitLast(sTemp,sRight))
			{
				sLeft = sTemp;
				sCreateDir = sRight + sCreateDir;
				pProfLoc = pProf->GetLocation(sLeft);
			}
			// see if the closest match is exclusive
			// if yes, then the the new path is a subdirectory and has to be exclusive too
			vfs::CVirtualLocation *pVLoc = this->GetVirtualLocation(sLeft,true);
			if(pVLoc)
			{
				bIsExclusive = pVLoc->GetIsExclusive();
			}
			//else
			//{
			//	THROWEXCEPTION(L"location (closest match) should exist");
			//}
			bNewLocation = true;
		}
		if(pProfLoc && pProfLoc->IsWriteable())
		{
			// create file and add to location
			vfs::IDirectory<vfs::IWriteable> *pDir = pProfLoc->Cast<vfs::IDirectory<vfs::IWriteable> >();
			vfs::IBaseFile* pFile = pDir->AddFile(sFileName);
			if(bNewLocation)
			{
				pProf->AddLocation(pProfLoc);
			}
			if(pFile)
			{
				CVirtualLocation* pLoc = this->GetVirtualLocation(sPath,true);
				if(bIsExclusive)
				{
					pLoc->SetIsExclusive(bIsExclusive);
				}
				pLoc->AddFile(pFile,pProf->Name);
				return true;
			}
		}
	}
	// throw ?
	return false;
}

/************************************************************************************************/
