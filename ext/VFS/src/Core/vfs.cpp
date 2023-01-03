/* 
 * bfVFS : vfs/Core/vfs.cpp
 *  - primary interface for the using program, get files from the VFS internal storage
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

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs.h>

#include <vfs/Core/Interface/vfs_directory_interface.h>
#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Core/File/vfs_dir_file.h>
#include <vfs/Core/File/vfs_lib_file.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs_vfile.h>

#include <vfs/Tools/vfs_property_container.h>
#include <vfs/Tools/vfs_parser_tools.h>

#include <stack>

template class vfs::TIterator<vfs::tReadableFile>; // explicit instantiation

/********************************************************************/
/********************************************************************/

class vfs::CVirtualFileSystem::CRegularIterator : public vfs::CVirtualFileSystem::Iterator::IImplementation
{
	friend class vfs::CVirtualFileSystem;
	typedef vfs::CVirtualFileSystem::Iterator::IImplementation tBaseClass;

	CRegularIterator(vfs::CVirtualFileSystem* pVFS);
public:
	CRegularIterator() : tBaseClass(), m_VFS(NULL)
	{};
	virtual ~CRegularIterator()
	{};

	virtual vfs::tReadableFile*				value();
	virtual void							next();
protected:
	virtual tBaseClass* clone()
	{
		CRegularIterator* iter = new CRegularIterator();
		iter->m_VFS = m_VFS;
		iter->_vloc_iter = _vloc_iter;
		iter->_vfile_iter = _vfile_iter;
		return iter;
	}
private:
	vfs::CVirtualFileSystem*				m_VFS;
	vfs::CVirtualFileSystem::tVFS::iterator	_vloc_iter;
	vfs::CVirtualLocation::Iterator			_vfile_iter;
};

vfs::CVirtualFileSystem::CRegularIterator::CRegularIterator(vfs::CVirtualFileSystem* pVFS)
: tBaseClass(), m_VFS(pVFS)
{
	_vloc_iter = m_VFS->m_mapFS.begin();
	if(_vloc_iter != m_VFS->m_mapFS.end())
	{
		_vfile_iter = _vloc_iter->second->iterate();
	}
}

vfs::tReadableFile* vfs::CVirtualFileSystem::CRegularIterator::value()
{
	bool bExclusiveVLoc = false;
	if(_vloc_iter != m_VFS->m_mapFS.end())
	{
		bExclusiveVLoc = _vloc_iter->second->getIsExclusive();
	}
	if(!_vfile_iter.end())
	{
		vfs::CVirtualFile* pVFile = _vfile_iter.value();
		if(pVFile)
		{
			if(bExclusiveVLoc)
			{
				return vfs::tReadableFile::cast(pVFile->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE));
			}
			else
			{
				return vfs::tReadableFile::cast(pVFile->file(vfs::CVirtualFile::SF_TOP));
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
		if(_vloc_iter != m_VFS->m_mapFS.end())
		{
			_vloc_iter++;
			if(_vloc_iter != m_VFS->m_mapFS.end())
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

class vfs::CVirtualFileSystem::CMatchingIterator : public vfs::CVirtualFileSystem::Iterator::IImplementation
{
	friend class vfs::CVirtualFileSystem;
	typedef vfs::CVirtualFileSystem::Iterator::IImplementation tBaseClass;

	CMatchingIterator(vfs::Path const& sPattern, vfs::CVirtualFileSystem* pVFS);
public:
	CMatchingIterator() : tBaseClass(), m_VFS(NULL)
	{};
	virtual ~CMatchingIterator()
	{};

	virtual vfs::tReadableFile*				value();
	virtual void							next();
protected:
	virtual tBaseClass* clone()
	{
		CMatchingIterator* iter = new CMatchingIterator();
		iter->m_sLocPattern = m_sLocPattern;
		iter->m_sFilePattern = m_sFilePattern;
		iter->m_VFS = m_VFS;
		iter->_vloc_iter = _vloc_iter;
		iter->_vfile_iter = _vfile_iter;
		return iter;
	}
private:
	bool									nextLocationMatch();
	bool									nextFileMatch();
private:
	vfs::Path								m_sLocPattern, m_sFilePattern;
	vfs::CVirtualFileSystem*				m_VFS;
	vfs::CVirtualFileSystem::tVFS::iterator	_vloc_iter;
	vfs::CVirtualLocation::Iterator			_vfile_iter;
};

vfs::CVirtualFileSystem::CMatchingIterator::CMatchingIterator(vfs::Path const& sPattern, vfs::CVirtualFileSystem* pVFS)
: tBaseClass(), m_VFS(pVFS)
{
	if(sPattern() == vfs::Const::STAR())
	{
		m_sLocPattern =  vfs::Path(vfs::Const::STAR());
		m_sFilePattern = vfs::Path(vfs::Const::STAR());
	}
	else
	{
		sPattern.splitLast(m_sLocPattern,m_sFilePattern);
	}

	_vloc_iter = m_VFS->m_mapFS.begin();
	while(_vloc_iter != m_VFS->m_mapFS.end())
	{
		if( matchPattern(m_sLocPattern(),_vloc_iter->second->cPath()) )
		{
			bool bExclusiveVLoc = _vloc_iter->second->getIsExclusive();
			_vfile_iter = _vloc_iter->second->iterate();
			while(!_vfile_iter.end())
			{
				vfs::IBaseFile* pFile = NULL;
				if(bExclusiveVLoc)
				{
					pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
				}
				else
				{
					pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_TOP);
				}
				if(pFile)
				{
					vfs::Path const& filename = pFile->getName();
					if( matchPattern(m_sFilePattern(),filename()) )
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

vfs::tReadableFile* vfs::CVirtualFileSystem::CMatchingIterator::value()
{
	bool bExclusiveVLoc = false;
	if( _vloc_iter != m_VFS->m_mapFS.end() )
	{
		bExclusiveVLoc = _vloc_iter->second->getIsExclusive();
	}
	if(!_vfile_iter.end())
	{
		vfs::CVirtualFile* pVFile = _vfile_iter.value();
		if(pVFile)
		{
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_TOP);
			}
			return vfs::tReadableFile::cast(pFile);
		}
	}
	return NULL;
}

bool vfs::CVirtualFileSystem::CMatchingIterator::nextLocationMatch()
{
	while(_vloc_iter != m_VFS->m_mapFS.end())
	{
		_vloc_iter++;
		if(_vloc_iter != m_VFS->m_mapFS.end())
		{
			if(matchPattern(m_sLocPattern(),_vloc_iter->second->cPath()))
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
	if( _vloc_iter != m_VFS->m_mapFS.end() )
	{
		bExclusiveVLoc = _vloc_iter->second->getIsExclusive();
	}
	while(!_vfile_iter.end())
	{
		_vfile_iter.next();
		if(!_vfile_iter.end())
		{
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_TOP);
			}
			if(pFile)
			{
				vfs::Path const& filename = pFile->getName();
				if(matchPattern(m_sFilePattern(),filename()))
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
			bool bExclusiveVLoc = _vloc_iter->second->getIsExclusive();
			vfs::IBaseFile* pFile = NULL;
			if(bExclusiveVLoc)
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			}
			else
			{
				pFile = _vfile_iter.value()->file(vfs::CVirtualFile::SF_TOP);
			}
			if(pFile && matchPattern(m_sFilePattern(),pFile->getName()()))
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

bool vfs::canWrite()
{
	vfs::CVirtualProfile *prof = getVFS()->getProfileStack()->topProfile();
	return prof && prof->cWritable;
}

vfs::CVirtualFileSystem* getVFS()
{
	return vfs::CVirtualFileSystem::getVFS();
}

vfs::CVirtualFileSystem* vfs::CVirtualFileSystem::m_pSingleton = NULL;

vfs::CVirtualFileSystem* vfs::CVirtualFileSystem::getVFS()
{
	if(!m_pSingleton)
	{
		m_pSingleton = new CVirtualFileSystem();
	}
	return m_pSingleton;
}

void vfs::CVirtualFileSystem::shutdownVFS()
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

vfs::CProfileStack* vfs::CVirtualFileSystem::getProfileStack()
{
	return &m_oProfileStack;
}

vfs::CVirtualFileSystem::Iterator vfs::CVirtualFileSystem::begin()
{
	return Iterator(new vfs::CVirtualFileSystem::CRegularIterator(this));
}

vfs::CVirtualFileSystem::Iterator vfs::CVirtualFileSystem::begin(vfs::Path const& sPattern)
{
	return Iterator(new vfs::CVirtualFileSystem::CMatchingIterator(sPattern,this));
}

bool vfs::CVirtualFileSystem::addLocation(vfs::IBaseLocation* pLocation, vfs::CVirtualProfile *pProfile)
{
	VFS_THROW_IFF(pLocation != NULL, L"Invalid location object");
	VFS_THROW_IFF(pProfile!= NULL, L"Invalid location object");

	std::list<vfs::Path> lSubDirs;
	pLocation->getSubDirList(lSubDirs);
	std::list<vfs::Path>::const_iterator sd_cit = lSubDirs.begin();
	for(;sd_cit != lSubDirs.end(); ++sd_cit)
	{
		this->getVirtualLocation(*sd_cit, true);
	}

	vfs::IBaseLocation::Iterator it = pLocation->begin();
	for(; !it.end(); it.next())
	{
		vfs::IBaseFile *pFile = it.value();
		vfs::Path const& sPath = pFile->getPath();
		vfs::Path dir,file;
		sPath.splitLast(dir,file);
		
		CVirtualLocation* pLoc = this->getVirtualLocation(dir,true);
		pLoc->addFile(pFile, pProfile->cName);
	}
	return true;
}

vfs::tReadableFile* vfs::CVirtualFileSystem::getReadFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return vfs::tReadableFile::cast(this->getFile(rLocalFilePath,eSF));
}

vfs::tWritableFile* vfs::CVirtualFileSystem::getWriteFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return vfs::tWritableFile::cast(this->getFile(rLocalFilePath,eSF));
}

vfs::IBaseFile* vfs::CVirtualFileSystem::getFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	VFS_LOG_DEBUG( (L"Get file : " + rLocalFilePath()).c_str() );
	vfs::Path sDir,sFile;
	rLocalFilePath.splitLast(sDir,sFile);

	vfs::CVirtualLocation* pVLoc = this->getVirtualLocation(sDir);
	if(pVLoc)
	{
		vfs::CVirtualFile *pVFile = pVLoc->getVirtualFile(sFile);
		if(pVFile)
		{
			if(pVLoc->getIsExclusive())
			{
				return pVFile->file(vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			}
			return pVFile->file(eSF);
		}
	}
	VFS_LOG_DEBUG( _BS(L"Could not find file : ") << rLocalFilePath << _BS::wget );
	return NULL;
}

vfs::tReadableFile* vfs::CVirtualFileSystem::getReadFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName)
{
	return vfs::tReadableFile::cast(this->getFile(rLocalFilePath, sProfileName));
}

vfs::tWritableFile* vfs::CVirtualFileSystem::getWriteFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName)
{
	return vfs::tWritableFile::cast(this->getFile(rLocalFilePath, sProfileName));
}

vfs::IBaseFile* vfs::CVirtualFileSystem::getFile(vfs::Path const& rLocalFilePath, vfs::String const& sProfileName)
{
	vfs::Path sDir,sFile;
	rLocalFilePath.splitLast(sDir,sFile);

	tVFS::iterator it_loc = m_mapFS.find(sDir);
	if(it_loc == m_mapFS.end())
	{
		return NULL;
	}
	vfs::CVirtualLocation* pVLoc = it_loc->second;
	if(pVLoc)
	{
		return pVLoc->getFile(sFile,sProfileName);
	}
	return NULL;
}

bool vfs::CVirtualFileSystem::fileExists(vfs::Path const& rLocalFilePath, std::string const& sProfileName)
{
	return this->getFile(rLocalFilePath, sProfileName) != NULL;
}

bool vfs::CVirtualFileSystem::fileExists(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF)
{
	return this->getFile(rLocalFilePath, eSF) != NULL;
}

vfs::CVirtualLocation* vfs::CVirtualFileSystem::getVirtualLocation(vfs::Path const& sPath, bool bCreate)
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

bool vfs::CVirtualFileSystem::removeDirectoryFromFS(vfs::Path const& sDir)
{
	vfs::Path pattern = sDir + "*";
	std::list<vfs::Path> files;

	Iterator it = this->begin(pattern);
	for(; !it.end(); it.next())
	{
		if(it.value()->implementsWritable())
		{
			files.push_back(it.value()->getPath());
		}
	}
	bool success = true;
	std::list<vfs::Path>::iterator fit = files.begin();
	for(; fit != files.end(); ++fit)
	{
		success &= this->removeFileFromFS(*fit);
	}
	return success;
}

bool vfs::CVirtualFileSystem::removeFileFromFS(vfs::Path const& sFilePath)
{
	vfs::Path sPath,sFile;
	sFilePath.splitLast(sPath,sFile);
	vfs::CVirtualProfile *pProf = m_oProfileStack.getWriteProfile();
	if(pProf)
	{
		vfs::IBaseLocation *pBL = pProf->getLocation(sPath);
		if(pBL && pBL->implementsWritable())
		{
			vfs::TDirectory<vfs::IWritable> *pDir = dynamic_cast<vfs::TDirectory<vfs::IWritable>*>(pBL);
			if(pDir)
			{
				bool bSuccess = false;
				// remove file from virtual structures first
				vfs::IBaseFile* file = pDir->getFile(sFilePath);
				if(file)
				{
					vfs::Path sDir,sFile;
					sFilePath.splitLast(sDir,sFile);
					vfs::CVirtualLocation *pVLoc = this->getVirtualLocation(sDir);
					if(pVLoc)
					{
						bSuccess = pVLoc->removeFile(file);
					}
				}
				// delete actual file
				return bSuccess && pDir->deleteFileFromDirectory(sFilePath);
			}
		}
	}
	return false;
}

bool vfs::CVirtualFileSystem::createNewFile(vfs::Path const& sFilename)
{
	vfs::Path sPath,sFile;
	sFilename.splitLast(sPath,sFile);
	vfs::CVirtualProfile *pProf = m_oProfileStack.getWriteProfile();
	if(pProf)
	{
		bool bIsExclusive = false;
		bool bNewLocation = false;
		vfs::IBaseLocation *pProfLoc = pProf->getLocation(sPath);
		if(!pProfLoc)
		{
			// try to find closest match
			vfs::Path sTemp, sCreateDir, sRight, sLeft = sPath;
			while(!pProfLoc && !sLeft.empty())
			{
				sLeft.splitLast(sTemp,sRight);
				sLeft = sTemp;
				sCreateDir = sRight + sCreateDir;
				pProfLoc = pProf->getLocation(sLeft);
			}
			// see if the closest match is exclusive
			// if yes, then the the new path is a subdirectory and has to be exclusive too
			vfs::CVirtualLocation *pVLoc = this->getVirtualLocation(sLeft,true);
			if(pVLoc)
			{
				bIsExclusive = pVLoc->getIsExclusive();
			}
			//else
			//{
			//	VFS_THROW(L"location (closest match) should exist");
			//}
			bNewLocation = true;
		}
		if(pProfLoc && pProfLoc->implementsWritable())
		{
			// create file and add to location
			vfs::TDirectory<vfs::IWritable> *pDir = dynamic_cast<vfs::TDirectory<vfs::IWritable>*>(pProfLoc);
			vfs::IBaseFile* pFile = pDir->addFile(sFilename);
			if(bNewLocation)
			{
				pProf->addLocation(pProfLoc);
			}
			if(pFile)
			{
				CVirtualLocation* pLoc = this->getVirtualLocation(sPath,true);
				if(bIsExclusive)
				{
					pLoc->setIsExclusive(bIsExclusive);
				}
				pLoc->addFile(pFile,pProf->cName);
				return true;
			}
		}
	}
	// throw ?
	return false;
}

/************************************************************************************************/
