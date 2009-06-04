#include "vfs_lib_dir.h"

vfs::CLibDirectory::~CLibDirectory()
{
	tFileCatalogue::iterator it = m_mapFiles.begin();
	for(; it != m_mapFiles.end(); ++it)
	{
		// don't delete objects here
		//delete it->second;
	}
	m_mapFiles.clear();
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::AddFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
	return NULL;
}

bool vfs::CLibDirectory::AddFile(tFileType* pFile, bool bDeleteOldFile)
{
	if(!pFile)
	{
		return false;
	}
	vfs::Path const& sName = pFile->GetFileName();
	tFileType* pFileOld = m_mapFiles[sName];
	if(pFileOld && (pFileOld != pFile) )
	{
		if(bDeleteOldFile)
		{
			delete pFileOld;
			m_mapFiles[sName] = pFile;
		}
		else
		{
			return false;
		}
	}
	m_mapFiles[sName] = pFile;
	return true;
}

bool vfs::CLibDirectory::DeleteDirectory(vfs::Path const& sDirPath)
{
	if( !(m_sMountPoint == sDirPath) )
	{
		return false;
	}
	if(IsWriteable())
	{
		tFileCatalogue::iterator it = m_mapFiles.begin();
		for(; it != m_mapFiles.end(); ++it)
		{
			delete it->second;
		}
		m_mapFiles.clear();
		return true;
	}
	return false;
}

bool vfs::CLibDirectory::DeleteFileFromDirectory(vfs::Path const& sFileName)
{
	if(IsWriteable())
	{
		tFileCatalogue::iterator it = m_mapFiles.find(sFileName);
		if(it != m_mapFiles.end())
		{
			delete it->second;
			m_mapFiles.erase(it);
			return true;
		}
	}
	return false;
}

bool vfs::CLibDirectory::FileExists(vfs::Path const& sFileName)
{
	bool success = (m_mapFiles[sFileName] != NULL);
	return success;
}

vfs::IBaseFile*	vfs::CLibDirectory::GetFile(vfs::Path const& sFileName)
{
	return GetFileTyped(sFileName);
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::GetFileTyped(vfs::Path const& sFileName)
{
	CLibDirectory::tFileType* file = m_mapFiles[sFileName];
	return file;
}

bool vfs::CLibDirectory::CreateSubDirectory(vfs::Path const& sSubDirPath)
{
	// libraries are readonly
	return false;
}

void vfs::CLibDirectory::GetSubDirList(std::list<vfs::Path>& rlSubDirs)
{
}

vfs::CLibDirectory::Iterator vfs::CLibDirectory::begin()
{
	return Iterator(new IterImpl(*this));
}

/***************************************************************************/
/***************************************************************************/

vfs::CLibDirectory::IterImpl::IterImpl(CLibDirectory& lib)
: _lib(lib)
{
	_iter = _lib.m_mapFiles.begin();
}

vfs::CLibDirectory::IterImpl::~IterImpl()
{
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::IterImpl::value()
{
	if(_iter != _lib.m_mapFiles.end())
	{
		return _iter->second;
	}
	return NULL;
}

void vfs::CLibDirectory::IterImpl::next()
{
	if(_iter != _lib.m_mapFiles.end())
	{
		_iter++;
	}
}

/***************************************************************************/
/***************************************************************************/

static void tesst()
{
	vfs::CLibDirectory *dir = new vfs::CLibDirectory(vfs::Path("test"), vfs::Path("test2"));
	vfs::CLibDirectory::Iterator it = dir->begin();
	while(!it.end())
	{
		vfs::CLibDirectory::tFileType* file = static_cast<vfs::CLibDirectory::tFileType*>(it.value());
		it.next();
	}
}