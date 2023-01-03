/* 
 * bfVFS : vfs/Core/Location/vfs_directory_tree.cpp
 *  - class for directories in a File System, implements Directory interface
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

#include <vfs/Core/Location/vfs_directory_tree.h>
#include <vfs/Core/File/vfs_dir_file.h>
#include <vfs/Core/vfs_os_functions.h>

#include <queue>
#include <list>
#include <set>

#define ERROR_FILE(msg) (_BS(msg) << L" : " << pFile->getPath() << _BS::wget)

namespace vfs
{
	template<typename WriteType>
	class TSubDir : public vfs::TDirectory<typename vfs::TDirectoryTree<WriteType>::tWriteType>
	{
		typedef vfs::TDirectory<typename vfs::TDirectoryTree<WriteType>::tWriteType> tBaseClass;
		typedef typename tBaseClass::tFileType tFileType;

		typedef std::map<vfs::Path, tFileType*, vfs::Path::Less> tFileCatalogue;
	public:
		typedef vfs::IBaseLocation::Iterator Iterator;

		/////////////////////////////////////////////////////////////////////
		class IterImpl : public vfs::IBaseLocation::Iterator::IImplementation
		{
			friend class TSubDir<WriteType>;
			typedef vfs::IBaseLocation::Iterator::IImplementation tBaseClass;

			IterImpl(TSubDir<WriteType>* dir): _dir(dir)
			{
				VFS_THROW_IFF(_dir, L"");
				_iter = _dir->m_mapFiles.begin();
			}
		public:
			IterImpl() : tBaseClass(), _dir(NULL)
			{};
			virtual ~IterImpl()
			{};

			virtual tFileType*	value()
			{
				if(_iter != _dir->m_mapFiles.end())
				{
					return _iter->second;
				}
				return NULL;
			}
			virtual void		next()
			{
				if(_iter != _dir->m_mapFiles.end())
				{
					_iter++;
				}
			}
		protected:
			tBaseClass*	clone()
			{
				IterImpl* iter = new IterImpl();
				iter->_dir = _dir;
				iter->_iter = _iter;
				return iter;
			}
		private:
			TSubDir<WriteType>*					_dir;
			typename tFileCatalogue::iterator	_iter;
		};
		/////////////////////////////////////////////////////////////////////
	public:
		TSubDir(vfs::Path const& sMountPoint, vfs::Path const& sRealPath) 
			: tBaseClass(sMountPoint,sRealPath)
		{};

		virtual ~TSubDir();

		virtual bool			fileExists(vfs::Path const& rFileName);
		virtual vfs::IBaseFile* getFile(vfs::Path const& rFileName);
		virtual tFileType*		getFileTyped(vfs::Path const& rFileName);
		
		virtual tFileType*		addFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			addFile(tFileType* pFile, bool bDeleteOldFile=false);

		virtual bool			createSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			deleteDirectory(vfs::Path const& sDirPath);
		virtual bool			deleteFileFromDirectory(vfs::Path const& sFileName);

		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator 		begin();
	private:
		tFileCatalogue m_mapFiles;
	};

	template<>
	vfs::TSubDir<vfs::IWriteType>::tFileType* vfs::TSubDir<vfs::IWriteType>::addFile(vfs::Path const& sFilename, bool bDeleteOldFile);
	template<>
	vfs::TSubDir<vfs::IWritable>::tFileType* vfs::TSubDir<vfs::IWritable>::addFile(vfs::Path const& sFilename, bool bDeleteOldFile);

	template<>
	bool vfs::TSubDir<vfs::IWriteType>::deleteDirectory(vfs::Path const& sDirPath);
	template<>
	bool vfs::TSubDir<vfs::IWritable>::deleteDirectory(vfs::Path const& sDirPath);

	template<>
	bool vfs::TSubDir<vfs::IWriteType>::deleteFileFromDirectory(vfs::Path const& rFileName);
	template<>
	bool vfs::TSubDir<vfs::IWritable>::deleteFileFromDirectory(vfs::Path const& rFileName);
}

/********************************************************/

template<typename WriteType>
vfs::TSubDir<WriteType>::~TSubDir()
{
	typename tFileCatalogue::iterator it = m_mapFiles.begin();
	for(; it != m_mapFiles.end(); ++it)
	{
		if(it->second)
		{
			it->second->close();
			delete it->second;
		}
	}
	m_mapFiles.clear();
}

template<typename WriteType>
bool vfs::TSubDir<WriteType>::fileExists(vfs::Path const& sFileName)
{
	typename tFileCatalogue::iterator it = m_mapFiles.find(sFileName);
	bool success = (it != m_mapFiles.end()) && (it->second != NULL);
	return success;
}

template<typename WriteType>
vfs::IBaseFile* vfs::TSubDir<WriteType>::getFile(vfs::Path const& sFileName)
{
	return getFileTyped(sFileName);
}

template<typename WriteType>
typename vfs::TSubDir<WriteType>::tFileType* vfs::TSubDir<WriteType>::getFileTyped(vfs::Path const& sFileName)
{
	typename tFileCatalogue::iterator it = m_mapFiles.find(sFileName);
	if(it != m_mapFiles.end())
	{
		return it->second;
	}
	return NULL;
}

template<>
vfs::TSubDir<vfs::IWriteType>::tFileType* vfs::TSubDir<vfs::IWriteType>::addFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
	tFileType* pFile = m_mapFiles[sFilename];
	if(pFile)
	{
		if(!bDeleteOldFile)
		{
			// not allowed to replace old file
			return NULL;
		}
		delete pFile;
	}
	pFile = new vfs::CReadOnlyDirFile(sFilename,this);
	m_mapFiles[sFilename] = pFile;

	return pFile;
}

template<>
vfs::TSubDir<vfs::IWritable>::tFileType* vfs::TSubDir<vfs::IWritable>::addFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
	tFileType* pFile = m_mapFiles[sFilename];
	if(pFile)
	{
		if(!bDeleteOldFile)
		{
			// not allowed to replace old file
			return NULL;
		}
		delete pFile;
	}
	pFile = new vfs::CDirFile(sFilename,this);
	m_mapFiles[sFilename] = pFile;

	return pFile;
}

template<typename WriteType>
bool vfs::TSubDir<WriteType>::addFile(tFileType* pFile, bool bDeleteOldFile)
{
	if(!pFile)
	{
		return false;
	}
	tFileType* pOldFile = m_mapFiles[pFile->getName()];
	if( pOldFile && (pOldFile != pFile) )
	{
		if(bDeleteOldFile)
		{
			pOldFile->close();
			delete pOldFile;
		}
	}
	m_mapFiles[pFile->getName()] = pFile;
	return true;
}

template<>
bool vfs::TSubDir<vfs::IWritable>::deleteDirectory(vfs::Path const& sDirPath)
{
	if( !(this->m_mountPoint == sDirPath) )
	{
		return false;
	}
	tFileCatalogue::iterator it = m_mapFiles.begin();
	for(; it != m_mapFiles.end(); ++it)
	{
		tFileType* pFile = it->second;
		if(pFile)
		{
			pFile->close();
			if(!pFile->deleteFile())
			{
				VFS_THROW( ERROR_FILE(L"Could not delete file") );
			}
			delete pFile;
		}
	}
	m_mapFiles.clear();
	return true;
}

template<>
bool vfs::TSubDir<vfs::IWriteType>::deleteDirectory(vfs::Path const& sDirPath)
{
	return false;
}

template<>
bool vfs::TSubDir<vfs::IWritable>::deleteFileFromDirectory(vfs::Path const& rFileName)
{
	tFileCatalogue::iterator it = m_mapFiles.find(rFileName);
	if( it != m_mapFiles.end() )
	{
		tFileType* pFile = it->second;
		if(pFile)
		{
			pFile->close();
			VFS_THROW_IFF(pFile->deleteFile(), ERROR_FILE(L"Could not delete file"));
			delete pFile;
		}
		m_mapFiles.erase(it);
		return true;
	}
	return false;
}

template<>
bool vfs::TSubDir<vfs::IWriteType>::deleteFileFromDirectory(vfs::Path const& rFileName)
{
	return false;
}

template<typename WriteType>
bool vfs::TSubDir<WriteType>::createSubDirectory(vfs::Path const& sSubDirPath)
{
	return false;
}

template<typename WriteType>
void vfs::TSubDir<WriteType>::getSubDirList(std::list<vfs::Path>& rlSubDirs)
{
}

template<typename WriteType>
typename vfs::TSubDir<WriteType>::Iterator vfs::TSubDir<WriteType>::begin()
{
	return Iterator(new IterImpl(this));
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

template<typename WriteType>
class vfs::TDirectoryTree<WriteType>::IterImpl : public vfs::IBaseLocation::Iterator::IImplementation
{
	typedef vfs::IBaseLocation::Iterator::IImplementation tBaseClass;
	typedef vfs::TDirectoryTree<WriteType> tDirTree;
public:
	IterImpl(tDirTree& tree);
	virtual ~IterImpl();

	virtual typename tDirTree::tFileType*		value();
	virtual void								next();
protected:
	virtual tBaseClass*							clone()
	{
		IterImpl* iter = new IterImpl(_tree);
		iter->_subdir_iter = _subdir_iter;
		iter->_file_iter = _file_iter;
		return iter;
	}
private:
	void operator=(typename vfs::TDirectoryTree<WriteType>::IterImpl const& tree);
	tDirTree&									_tree;
	typename tDirTree::tDirCatalogue::iterator	_subdir_iter;
	typename tDirTree::tLocationType::Iterator	_file_iter;
};

template<typename WriteType>
vfs::TDirectoryTree<WriteType>::IterImpl::IterImpl(vfs::TDirectoryTree<WriteType>& tree)
: tBaseClass(), _tree(tree)
{
	_subdir_iter = _tree.m_catDirs.begin();
	if(_subdir_iter != _tree.m_catDirs.end())
	{
		TSubDir<WriteType> *sdir = dynamic_cast<TSubDir<WriteType>*>(_subdir_iter->second);
		typename TSubDir<WriteType>::Iterator it = sdir->begin();
		_file_iter = it;
		if(_file_iter.end())
		{
			next();
		}
	}
}

template<typename WriteType>
vfs::TDirectoryTree<WriteType>::IterImpl::~IterImpl()
{
}

template<typename WriteType>
typename vfs::TDirectoryTree<WriteType>::tFileType* vfs::TDirectoryTree<WriteType>::IterImpl::value()
{
	if(!_file_iter.end())
	{
		return static_cast<typename vfs::TDirectoryTree<WriteType>::tFileType*>(_file_iter.value());		
	}
	return NULL;
}

template<typename WriteType>
void vfs::TDirectoryTree<WriteType>::IterImpl::next()
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

/*****************************************************************************/
/*****************************************************************************/

template<typename WriteType>
vfs::TDirectoryTree<WriteType>::TDirectoryTree(vfs::Path const& sMountPoint, vfs::Path const& sRealPath)
: tBaseClass(sMountPoint,sRealPath)
{};


template<typename WriteType>
vfs::TDirectoryTree<WriteType>::~TDirectoryTree()
{
	typename tDirCatalogue::iterator it = m_catDirs.begin();
	for(;it != m_catDirs.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_catDirs.clear();
}

template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::init()
{
	// contains local path
	typedef vfs::TSubDir<WriteType> tSubDir;
	typedef std::pair<vfs::Path,tSubDir*> tDirs;
	std::queue<tDirs> qSubDirs;
	qSubDirs.push(tDirs(vfs::Path(vfs::Const::EMPTY()),new tSubDir(this->m_mountPoint, this->m_realPath)));
	
	m_catDirs[this->m_mountPoint] = qSubDirs.front().second;
	
	vfs::String sFilename;
	tSubDir *pCurrentDir;
	vfs::Path oCurDir;
	while(!qSubDirs.empty())
	{
		pCurrentDir = qSubDirs.front().second;
		oCurDir = this->m_realPath;
		if( !qSubDirs.front().first.empty())
		{
			oCurDir += qSubDirs.front().first;
		}

		try
		{
			vfs::OS::CIterateDirectory::EFileAttribute eFA;
			vfs::OS::CIterateDirectory iterFS(oCurDir, vfs::Const::STAR());
			while ( iterFS.nextFile(sFilename, eFA) ) 
			{
				if (StrCmp::Equal(vfs::Const::DOT(),sFilename) || StrCmp::Equal(vfs::Const::DOTDOT(),sFilename) || StrCmp::Equal(vfs::Const::DOTSVN(),sFilename) )
				{
					continue;
				}
				if (eFA == vfs::OS::CIterateDirectory::FA_DIRECTORY) 
				{
					vfs::Path sLocal = qSubDirs.front().first + sFilename;

					vfs::Path temp = this->m_mountPoint+sLocal;
					tSubDir *pNewDir = new tSubDir(sLocal, this->m_realPath+sLocal);
					qSubDirs.push(tDirs(sLocal,pNewDir));
					m_catDirs[temp] = pNewDir;
				}
				else 
				{
					pCurrentDir->addFile(vfs::Path(sFilename));
				}
			}
		}
		catch(std::exception &ex)
		{
			// probably directory doesn't exist. abort or continue???
			// -> abort AND continue
			VFS_LOG_WARNING(ex.what());
			return false;
		}
		qSubDirs.pop();
	}
	return true;
}

template<typename WriteType>
typename vfs::TDirectoryTree<WriteType>::tFileType* vfs::TDirectoryTree<WriteType>::addFile(vfs::Path const& sFilename, bool bDeleteOldFile)
{
	vfs::Path sDir,sFile;
	sFilename.splitLast(sDir,sFile);
	typename tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		vfs::Path sTemp,sCreateDir,sLeft,sRight = sDir;
		while(!sRight.empty())
		{
			sRight.splitFirst(sLeft,sTemp);
			sRight = sTemp;
			sCreateDir += sLeft;
			if(!this->createSubDirectory(sCreateDir))
			{
				VFS_THROW(_BS(L"could not create directory : ") << sCreateDir << _BS::wget);
			}
		}
		it = m_catDirs.find(sDir);
		if(it == m_catDirs.end())
		{
			return NULL;
		}
	}
	if(it->second)
	{
		return it->second->addFile(sFile,bDeleteOldFile);
	}
	return NULL;
}

template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::addFile(tFileType* pFile, bool bDeleteOldFile)
{
	// no files from outside
	// these files are not connected with the correct directory object
	return false;
}

template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::deleteDirectory(vfs::Path const& sDirPath)
{
	typename tDirCatalogue::iterator it = m_catDirs.find(sDirPath);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		return it->second->deleteDirectory(sDirPath);
	}
	return false;
}

template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::deleteFileFromDirectory(vfs::Path const& sFileName)
{
	vfs::Path sDir,sFile;
	sFileName.splitLast(sDir,sFile);
	typename tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		return it->second->deleteFileFromDirectory(sFile);
	}
	return false;
}

/** 
 *  IVFSLocation interface
 */
template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::fileExists(vfs::Path const& sFileName)
{
	vfs::Path sDir, sFile;
	sFileName.splitLast(sDir, sFile);
	typename tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return false;
	}
	if(it->second)
	{
		return it->second->fileExists(sFile);
	}
	return false;
}

template<typename WriteType>
vfs::IBaseFile* vfs::TDirectoryTree<WriteType>::getFile(vfs::Path const& sFileName)
{
	return getFileTyped(sFileName);
}

template<typename WriteType>
typename vfs::TDirectoryTree<WriteType>::tFileType* vfs::TDirectoryTree<WriteType>::getFileTyped(vfs::Path const& sFileName)
{
	vfs::Path sDir, sFile;
	sFileName.splitLast(sDir, sFile);
	typename tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it == m_catDirs.end())
	{
		// no such directory
		return NULL;
	}
	if(it->second)
	{
		return it->second->getFileTyped(sFile);
	}
	return NULL;
}

template<typename WriteType>
bool vfs::TDirectoryTree<WriteType>::createSubDirectory(vfs::Path const& sSubDirPath)
{
	if(vfs::OS::createRealDirectory( this->m_realPath + sSubDirPath ))
	{
		if( m_catDirs[sSubDirPath] == NULL)
		{
			m_catDirs[sSubDirPath] = new vfs::TSubDir<WriteType>(sSubDirPath, this->m_realPath + sSubDirPath);
		}
		return true;
	}
	return false;
}

template<typename WriteType>
void vfs::TDirectoryTree<WriteType>::getSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	typename tDirCatalogue::iterator it = m_catDirs.begin();
	for(;it != m_catDirs.end(); ++it)
	{
		rlSubDirs.push_back(it->first);
	}
}

template<typename WriteType>
typename vfs::TDirectoryTree<WriteType>::Iterator vfs::TDirectoryTree<WriteType>::begin()
{
	return Iterator(new IterImpl(*this));
}

/*****************************************************************************/
/*****************************************************************************/

template class vfs::TDirectoryTree<vfs::IWritable>;		// explicit template class instantiation
template class vfs::TDirectoryTree<vfs::IWriteType>;	// explicit template class instantiation

