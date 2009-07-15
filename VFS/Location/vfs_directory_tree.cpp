#include "vfs_directory_tree.h"
#include "../File/vfs_dir_file.h"
#include "../Interface/vfs_location_aware_file_interface.h"

#include "../iteratedir.h"

#include <queue>
#include <list>
#include <set>

const utf8string::str_t CONST_EXT_TXT = L".TXT";
const utf8string::str_t CONST_EXT_LOG = L".LOG";
const utf8string::str_t CONST_EXT_INI = L".INI";

namespace vfs
{
	class CSubDir : public vfs::IDirectory<vfs::CDirectoryTree::tWriteType>
	{
		typedef std::map<vfs::Path, tFileType*, vfs::Path::Less> tFileCatalogue;

		class IterImpl : public tClassType::Iterator::IImplemetation
		{
		public:
			IterImpl(CSubDir& dir);
			virtual ~IterImpl();
			virtual tFileType* value();
			virtual void next();
		private:
			CSubDir& _dir;
			tFileCatalogue::iterator _iter;
		};
	public:
		CSubDir(vfs::Path const& sMountPoint, vfs::Path const& sRealPath) 
			: vfs::IDirectory<vfs::CDirectoryTree::tWriteType>(sMountPoint,sRealPath)
		{};
		virtual ~CSubDir();

		virtual bool			FileExists(vfs::Path const& rFileName);
		virtual vfs::IBaseFile* GetFile(vfs::Path const& rFileName);
		virtual tFileType*		GetFileTyped(vfs::Path const& rFileName);
		
		virtual tFileType*		AddFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			AddFile(tFileType* pFile, bool bDeleteOldFile=false);

		virtual bool			CreateSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			DeleteDirectory(vfs::Path const& sDirPath);
		virtual bool			DeleteFileFromDirectory(vfs::Path const& sFileName);

		virtual void			GetSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	private:
		tFileCatalogue m_mapFiles;
	};
}

/********************************************************/
vfs::CSubDir::IterImpl::IterImpl(CSubDir& dir)
: _dir(dir)
{
	_iter = _dir.m_mapFiles.begin();
}
vfs::CSubDir::IterImpl::~IterImpl()
{
}
vfs::CSubDir::tFileType* vfs::CSubDir::IterImpl::value()
{
	if(_iter != _dir.m_mapFiles.end())
	{
		return _iter->second;
	}
	return NULL;
}
void vfs::CSubDir::IterImpl::next()
{
	if(_iter != _dir.m_mapFiles.end())
	{
		_iter++;
	}
}
/********************************************************/

vfs::CSubDir::~CSubDir()
{
	tFileCatalogue::iterator it = m_mapFiles.begin();
	for(; it != m_mapFiles.end(); ++it)
	{
		if(it->second)
		{
			it->second->Close();
			delete it->second;
		}
	}
	m_mapFiles.clear();
}

bool vfs::CSubDir::FileExists(vfs::Path const& sFileName)
{
	tFileCatalogue::iterator it = m_mapFiles.find(sFileName);
	bool success = (it != m_mapFiles.end()) && (it->second != NULL);
	return success;
}

vfs::IBaseFile* vfs::CSubDir::GetFile(vfs::Path const& sFileName)
{
	return GetFileTyped(sFileName);
}

vfs::CSubDir::tFileType* vfs::CSubDir::GetFileTyped(vfs::Path const& sFileName)
{
	tFileCatalogue::iterator it = m_mapFiles.find(sFileName);
	if(it != m_mapFiles.end())
	{
		CSubDir::tFileType* file = it->second;
		return file;
	}
	return NULL;
}


vfs::CSubDir::tFileType* vfs::CSubDir::AddFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
#if 1
	tFileType* pFile = m_mapFiles[sFilename];
#else
	CSubDir::tFileCatalogue::iterator it;
	tFileType* pFile = ((it=m_mapFiles.find(sFilename)) != m_mapFiles.end()) ? it->second : NULL;
#endif
	if(pFile)
	{
		if(!bDeleteOldFile)
		{
			// not allowed to replace old file
			return NULL;
		}
		delete pFile;
	}
	utf8string sExtension;
	sFilename.Extension(sExtension);
	if(sExtension == CONST_EXT_TXT || sExtension == CONST_EXT_LOG || sExtension == CONST_EXT_INI)
	{
		pFile = new vfs::CVFSTextFile(sFilename,this);
	}
	else
	{
		pFile = new vfs::CVFSFile(sFilename,this);
	}
#if 1
	m_mapFiles[sFilename] = pFile;
#else
	m_mapFiles.insert(std::make_pair(sFilename,pFile));
#endif

	return pFile;
}

bool vfs::CSubDir::AddFile(tFileType* pFile, bool bDeleteOldFile)
{
	if(!pFile)
	{
		return false;
	}
	tFileType * pOldFile = m_mapFiles[pFile->GetFileName()];
	if( pOldFile && (pOldFile != pFile) )
	{
		if(bDeleteOldFile)
		{
			pOldFile->Close();
			delete pOldFile;
		}
	}
	m_mapFiles[pFile->GetFileName()] = pFile;
	return true;
}

bool vfs::CSubDir::DeleteDirectory(vfs::Path const& sDirPath)
{
	if( !(m_sMountPoint == sDirPath) )
	{
		return false;
	}
	tFileCatalogue::iterator it = m_mapFiles.begin();
	for(; it != m_mapFiles.end(); ++it)
	{
		tFileType* pFile = it->second;
		if(pFile)
		{
			pFile->Close();
			if(!pFile->Delete())
			{
				std::wstringstream wss;
				wss << L"Could not delete file \"" << pFile->GetFullPath()() << L"\"";
				THROWEXCEPTION(wss.str().c_str());
			}
			delete pFile;
		}
	}
	m_mapFiles.clear();
	return true;
}

bool vfs::CSubDir::DeleteFileFromDirectory(vfs::Path const& rFileName)
{
	tFileCatalogue::iterator it = m_mapFiles.find(rFileName);
	if( it != m_mapFiles.end() )
	{
		tFileType* pFile = it->second;
		if(pFile)
		{
			pFile->Close();
			THROWIFFALSE(pFile->Delete(), L"Could not delete file");
			delete pFile;
		}
		m_mapFiles.erase(it);
		return true;
	}
	return false;
}


bool vfs::CSubDir::CreateSubDirectory(vfs::Path const& sSubDirPath)
{
	return false;
}

void vfs::CSubDir::GetSubDirList(std::list<vfs::Path>& rlSubDirs)
{
}

vfs::CSubDir::Iterator vfs::CSubDir::begin()
{
	Iterator it;
	{
		it = Iterator(new IterImpl(*this));
	}
	return it;
}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

vfs::CDirectoryTree::~CDirectoryTree()
{
	tDirCatalogue::iterator it = m_catDirs.begin();
	for(;it != m_catDirs.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_catDirs.clear();
}


bool vfs::CDirectoryTree::Init()
{
	// contains local path
	typedef std::pair<vfs::Path,CSubDir*> tDirs;
	std::queue<tDirs> qSubDirs;
	qSubDirs.push(tDirs(vfs::Path(vfs::Const::EMPTY()),new CSubDir(m_sMountPoint,m_sRealPath)));
	
	m_catDirs[m_sMountPoint] = qSubDirs.front().second;
	
	utf8string sFilename;
	CSubDir *pCurrentDir;
	vfs::Path oCurDir;
	while(!qSubDirs.empty())
	{
		pCurrentDir = qSubDirs.front().second;
		oCurDir = m_sRealPath;
		if( !qSubDirs.front().first.empty())
		{
			oCurDir += qSubDirs.front().first;
		}

		try
		{
			os::CIterateDirectory::EFileAttribute eFA;
			os::CIterateDirectory iterFS(oCurDir, vfs::Const::STAR());
			while ( iterFS.NextFile(sFilename, eFA) ) 
			{
				if (StrCmp::Equal(vfs::Const::DOT(),sFilename) || StrCmp::Equal(vfs::Const::DOTDOT(),sFilename) || StrCmp::Equal(vfs::Const::DOTSVN(),sFilename) )
				{
					continue;
				}
				if (eFA == os::CIterateDirectory::FA_DIRECTORY) 
				{
					vfs::Path sLocal = qSubDirs.front().first + sFilename;

					vfs::Path temp = m_sMountPoint+sLocal;
					CSubDir *pNewDir = new CSubDir(sLocal, m_sRealPath+sLocal);
					qSubDirs.push(tDirs(sLocal,pNewDir));
					m_catDirs[temp] = pNewDir;
				}
				else 
				{
					pCurrentDir->AddFile(vfs::Path(sFilename));
				}
			}
		}
		catch(CBasicException &ex)
		{
			// probably directory doesn't exist. abort or continue???
			// -> abort AND continue
			LogException(ex);
			return false;
		}
		qSubDirs.pop();
	}
	return true;
}

vfs::CDirectoryTree::tFileType* vfs::CDirectoryTree::AddFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
	vfs::Path sDir,sFile;
	sFilename.SplitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		vfs::Path sTemp,sCreateDir,sLeft,sRight = sDir;
		while(sRight.SplitFirst(sLeft,sTemp))
		{
			sRight = sTemp;
			sCreateDir += sLeft;
			CreateSubDirectory(sCreateDir);
		}
		it = m_catDirs.find(sDir);
		if(it == m_catDirs.end())
		{
			return NULL;
		}
	}
	if(it->second)
	{
		vfs::CDirectoryTree::tFileType* file = it->second->AddFile(sFile,bDeleteOldFile);
		return file;
	}
	return NULL;
}

bool vfs::CDirectoryTree::AddFile(tFileType* pFile, bool bDeleteOldFile)
{
	// no files from outside
	// these files are not connected with the correct directory object
	return false;
}

bool vfs::CDirectoryTree::DeleteDirectory(vfs::Path const& sDirPath)
{
	tDirCatalogue::iterator it = m_catDirs.find(sDirPath);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		return it->second->DeleteDirectory(sDirPath);
	}
	return false;
}

bool vfs::CDirectoryTree::DeleteFileFromDirectory(vfs::Path const& sFileName)
{
	vfs::Path sDir,sFile;
	sFileName.SplitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		return it->second->DeleteFileFromDirectory(sFile);
	}
	return false;
}

/** 
 *  IVFSLocation interface
 */
bool vfs::CDirectoryTree::FileExists(vfs::Path const& sFileName)
{
	vfs::Path sDir, sFile;
	sFileName.SplitLast(sDir, sFile);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		bool success = it->second->FileExists(sFile);
		return success;
	}
	return false;
}

vfs::IBaseFile* vfs::CDirectoryTree::GetFile(vfs::Path const& sFileName)
{
	return GetFileTyped(sFileName);
}

vfs::CDirectoryTree::tFileType* vfs::CDirectoryTree::GetFileTyped(vfs::Path const& sFileName)
{
	vfs::Path sDir, sFile;
	sFileName.SplitLast(sDir, sFile);
	//tDirCatalogue::iterator it = m_catDirs.find(sDir);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return NULL;
	}
	if(it->second)
	{
		vfs::CDirectoryTree::tFileType* file = it->second->GetFileTyped(sFile);
		return file;
	}
	return NULL;
}

bool vfs::CDirectoryTree::CreateSubDirectory(vfs::Path const& sSubDirPath)
{
	if(os::CreateRealDirecory( m_sRealPath + sSubDirPath ))
	{
		if( m_catDirs[sSubDirPath] == NULL)
		{
			CSubDir *pNewDir = new CSubDir(sSubDirPath, m_sRealPath + sSubDirPath);
			m_catDirs[sSubDirPath] = pNewDir;
		}
		return true;
	}
	return false;
}

void vfs::CDirectoryTree::GetSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	tDirCatalogue::iterator it = m_catDirs.begin();
	for(;it != m_catDirs.end(); ++it)
	{
		rlSubDirs.push_back(it->first);
	}
}

vfs::CDirectoryTree::Iterator vfs::CDirectoryTree::begin()
{
	return Iterator(new IterImpl(*this));
}


/*****************************************************************************/
/*****************************************************************************/

vfs::CDirectoryTree::IterImpl::IterImpl(CDirectoryTree& tree)
: _tree(tree)
{
	_subdir_iter = _tree.m_catDirs.begin();
	if(_subdir_iter != _tree.m_catDirs.end())
	{
		CSubDir *sdir = dynamic_cast<CSubDir*>(_subdir_iter->second);
		CSubDir::Iterator it = sdir->begin();
		_file_iter = it;
		if(_file_iter.end())
		{
			next();
		}
	}
}

vfs::CDirectoryTree::IterImpl::~IterImpl()
{
}

vfs::CDirectoryTree::tFileType* vfs::CDirectoryTree::IterImpl::value()
{
	if(!_file_iter.end())
	{
		return static_cast<tFileType*>(_file_iter.value());
	}
	return NULL;
}

void vfs::CDirectoryTree::IterImpl::next()
{
	if(!_file_iter.end())
	{
		_file_iter.next();
	}
	// need to loop for the case when one or many sub directories are empty
	while(_file_iter.end())
	{
		if(_subdir_iter == _tree.m_catDirs.end())
		{
			break;
		}
		else
		{
			_subdir_iter++;
			if(_subdir_iter != _tree.m_catDirs.end())
			{
				_file_iter = _subdir_iter->second->begin();
			}
		}
	}
	return;
}



