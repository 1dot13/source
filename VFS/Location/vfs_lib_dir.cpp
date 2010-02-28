#include "vfs_lib_dir.h"
#include "../Tools/Log.h"

class vfs::CLibDirectory::IterImpl : public vfs::IBaseLocation::Iterator::IImplementation
{
	typedef vfs::IBaseLocation::Iterator::IImplementation tBaseClass;
public:
	IterImpl(CLibDirectory& lib);
	virtual ~IterImpl();
	virtual vfs::CLibDirectory::tFileType*			value();
	virtual void									next();
protected:
	virtual tBaseClass*								clone();
private:
	void operator=(vfs::CLibDirectory::IterImpl const& iter);

	vfs::CLibDirectory&								_lib;
	vfs::CLibDirectory::tFileCatalogue::iterator	_iter;
};

vfs::CLibDirectory::IterImpl::IterImpl(CLibDirectory& lib)
: tBaseClass(), _lib(lib)
{
	_iter = _lib.m_files.begin();
}

vfs::CLibDirectory::IterImpl::~IterImpl()
{
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::IterImpl::value()
{
	if(_iter != _lib.m_files.end())
	{
		return _iter->second;
	}
	return NULL;
}

void vfs::CLibDirectory::IterImpl::next()
{
	if(_iter != _lib.m_files.end())
	{
		_iter++;
	}
}

vfs::CLibDirectory::IterImpl::tBaseClass* vfs::CLibDirectory::IterImpl::clone()
{
	IterImpl* iter = new IterImpl(_lib);
	iter->_iter = _iter;
	return iter;
}

/***************************************************************************/
/***************************************************************************/

vfs::CLibDirectory::CLibDirectory(vfs::Path const& sLocalPath, vfs::Path const& sRealPath)
: tBaseClass(sLocalPath,sRealPath)
{
}

vfs::CLibDirectory::~CLibDirectory()
{
	tFileCatalogue::iterator it = m_files.begin();
	for(; it != m_files.end(); ++it)
	{
		// don't delete objects here
		//delete it->second;
	}
	m_files.clear();
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::addFile(vfs::Path const& filename, bool deleteOldFile)
{
	return NULL;
}

bool vfs::CLibDirectory::addFile(tFileType* file, bool deleteOldFile)
{
	if(!file)
	{
		return false;
	}
	vfs::Path const& name = file->getName();
	tFileType* oldFile = m_files[name];
	if(oldFile && (oldFile != file) )
	{
		if(deleteOldFile)
		{
			delete oldFile;
			m_files[name] = file;
		}
		else
		{
			return false;
		}
	}
	m_files[name] = file;
	return true;
}

bool vfs::CLibDirectory::deleteDirectory(vfs::Path const& dirPath)
{
	//if( !(m_mountPoint == dirPath) )
	//{
	//	return false;
	//}
	//if(implementsWritable())
	//{
	//	tFileCatalogue::iterator it = m_files.begin();
	//	for(; it != m_files.end(); ++it)
	//	{
	//		//delete it->second;
	//	}
	//	m_files.clear();
	//	return true;
	//}
	CLog log(L"errors.log");
	log << "called 'vfs::CLibDirectory::deleteDirectory' which doesn't implement the IWritable interface" << CLog::ENDL;
	return false;
}

bool vfs::CLibDirectory::deleteFileFromDirectory(vfs::Path const& filename)
{
	//if(implementsWritable())
	//{
	//	tFileCatalogue::iterator it = m_files.find(filename);
	//	if(it != m_files.end())
	//	{
	//		delete it->second;
	//		m_files.erase(it);
	//		return true;
	//	}
	//}
	CLog log(L"errors.log");
	log << "called 'vfs::CLibDirectory::deleteFileFromDirectory' which doesn't implement the IWritable interface" << CLog::ENDL;
	return false;
}

bool vfs::CLibDirectory::fileExists(vfs::Path const& filename)
{
	return (m_files[filename] != NULL);
}

vfs::IBaseFile*	vfs::CLibDirectory::getFile(vfs::Path const& filename)
{
	return getFileTyped(filename);
}

vfs::CLibDirectory::tFileType* vfs::CLibDirectory::getFileTyped(vfs::Path const& filename)
{
	tFileCatalogue::iterator it =  m_files.find(filename);
	if(it != m_files.end())
	{
		return it->second;
	}
	return NULL;
}

bool vfs::CLibDirectory::createSubDirectory(vfs::Path const& subDirPath)
{
	// libraries are readonly
	return false;
}

void vfs::CLibDirectory::getSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	// nothing
}

vfs::CLibDirectory::Iterator vfs::CLibDirectory::begin()
{
	return Iterator(new IterImpl(*this));
}

/***************************************************************************/
/***************************************************************************/
