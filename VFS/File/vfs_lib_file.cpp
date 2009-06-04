#include "vfs_lib_file.h"
#include "../vfs.h"

ObjBlockAllocator<vfs::CLibFile>* vfs::CLibFile::_lfile_pool = NULL;

vfs::CLibFile* vfs::CLibFile::Create(vfs::Path const& sFileName, 
									 vfs::IVFSLocation<vfs::IReadable,vfs::IWriteType> *pLocation, 
									 ILibrary *pLibrary,
									 ObjBlockAllocator<vfs::CLibFile>* allocator)
{
#if 0
	vfs::CLibFile* pFile = new vfs::CLibFile();
#else
	vfs::CLibFile* pFile;
	if(allocator)
	{
		pFile = allocator->New();
	}
	else
	{
		if(!_lfile_pool)
		{
			_lfile_pool = new ObjBlockAllocator<vfs::CLibFile>();
			CFileAllocator::RegisterAllocator(_lfile_pool);
		}
		pFile = _lfile_pool->New();
	}
#endif
	pFile->m_sFileName = sFileName;
	pFile->_pLoc_ = pLocation;
	pFile->m_pLibrary = pLibrary;
	return pFile;
}

vfs::CLibFile::CLibFile()
: vfs::IFileTemplate<IReadable,IWriteType>(L""), 
	vfs::ILocationAware<vfs::IReadable,vfs::IWriteType>(NULL),
	m_bIsOpen_read(false),
	m_pLibrary(NULL)
{
};

vfs::CLibFile::~CLibFile()
{
}

bool vfs::CLibFile::Close()
{
	if(m_bIsOpen_read)
	{
		m_bIsOpen_read = !m_pLibrary->Close(this);
		return m_bIsOpen_read;
	}
	return false;
}

vfs::Path vfs::CLibFile::GetFullPath()
{
	if(_pLoc_)
	{
		return _pLoc_->GetFullPath() + m_sFileName;
	}
	else
	{
		return m_sFileName;
	}
}
bool vfs::CLibFile::IsOpenRead()
{
	return m_bIsOpen_read;
}

bool vfs::CLibFile::OpenRead()
{
	if(!m_bIsOpen_read)
	{
		if(!(m_bIsOpen_read = m_pLibrary->OpenRead(this)))
		{
			return false;
		}
	}
	return true;
}

bool vfs::CLibFile::Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead)
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	bool success = m_pLibrary->Read(this,pData,uiBytesToRead,uiBytesRead);
	return success;
}	

vfs::UInt32 vfs::CLibFile::GetReadLocation()
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return -1;
	}
	return m_pLibrary->GetReadLocation(this);
}


bool vfs::CLibFile::SetReadLocation(vfs::UInt32 uiPositionInBytes)
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	bool success = m_pLibrary->SetReadLocation(this,uiPositionInBytes);
	return success;
}

bool vfs::CLibFile::SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir)
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	return m_pLibrary->SetReadLocation(this,uiOffsetInBytes,eSeekDir);
}

bool vfs::CLibFile::GetFileSize(vfs::UInt32& uiFileSize)
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	return m_pLibrary->GetFileSize(this,uiFileSize);
}
