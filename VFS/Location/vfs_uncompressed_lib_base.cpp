#include "vfs_uncompressed_lib_base.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

vfs::CUncompressedLibraryBase::~CUncompressedLibraryBase() 
{
	this->CloseLibrary();
	// delete sub dirs from catalogue
	tDirCatalogue::iterator it = m_catDirs.begin();
	for(; it != m_catDirs.end(); ++it)
	{
		delete it->second;
	}
	// LibData is invalid 
	// just clear it, since the file handles were deleted before
	m_mapLibData.clear(); 
	m_catDirs.clear();
}

bool vfs::CUncompressedLibraryBase::CloseLibrary()
{
	bool success = true;
	tLibData::iterator it = m_mapLibData.begin();
	for(; it != m_mapLibData .end(); ++it)
	{
		success &= it->first->Close();
		// what if closing of (at least) one file fails?? continue or not??
		// in the end, these are not real files!
	}
	return success;
}

bool vfs::CUncompressedLibraryBase::FileExists(vfs::Path const& sFileName)
{
	vfs::Path sDir,sFile;
	sFileName.SplitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it != m_catDirs.end())
	{
		return it->second->FileExists(sFile);
	}
	return false;
}

vfs::IBaseFile*	vfs::CUncompressedLibraryBase::GetFile(vfs::Path const& sFileName)
{
	return GetFileTyped(sFileName);
}

vfs::CUncompressedLibraryBase::tFileType* vfs::CUncompressedLibraryBase::GetFileTyped(vfs::Path const& sFileName)
{
	vfs::Path sDir,sFile;
	sFileName.SplitLast(sDir,sFile);
	tDirCatalogue::iterator it = m_catDirs.find(sDir);
	if(it != m_catDirs.end())
	{
		return it->second->GetFileTyped(sFile);
	}
	return NULL;
}

bool vfs::CUncompressedLibraryBase::Close(tFileType *pFileHandle)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return false;
	}
	// reset read position 
	// can't do this when opening file, 
	// because you could try to open a file when it is already open and would so reset the read position
	it->second.uiCurrentReadPosition = 0;
	if(m_uiNumberOfOpenedFiles > 0)
	{
		m_uiNumberOfOpenedFiles--;
		if(m_uiNumberOfOpenedFiles == 0)
		{
			m_pLibraryFile->Close();
		}
	}
	return true;
}
bool vfs::CUncompressedLibraryBase::OpenRead(tFileType *pFileHandle)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return false;
	}
	m_uiNumberOfOpenedFiles++;
	if(m_uiNumberOfOpenedFiles == 1)
	{
		if(!m_pLibraryFile->IsOpenRead() && !m_pLibraryFile->OpenRead())
		{
			return false;
		}
	}
	// already open
	return true;
}

bool vfs::CUncompressedLibraryBase::Read(tFileType *pFileHandle, Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return false;
	}
	// if(m_pLibraryFile->IsOpen()) ... we could check it (*AGAIN*), 
	//                                  but the file implementation does it already,
	//                                  and the user probably called 'OpenRead' too,
	//                                  so why do it over and over again
	if( (it->second.uiCurrentReadPosition + uiBytesToRead) > it->second.uiFileSize )
	{
		// original implementation
		uiBytesRead = 0;
		return false;
		// or you could adjust the number of bytes to read
		uiBytesToRead = it->second.uiFileSize - it->second.uiCurrentReadPosition; // +-1 ???
	}
	// set lib-file's read-location to match location of virtual-file
	m_pLibraryFile->SetReadLocation(it->second.uiFileOffset + it->second.uiCurrentReadPosition,IBaseFile::SD_BEGIN);
	bool success = m_pLibraryFile->Read(pData,uiBytesToRead,uiBytesRead) 
		&& (uiBytesToRead == uiBytesRead);
	if(success)
	{
		it->second.uiCurrentReadPosition += uiBytesRead;
	}
	// false if read operation failed
	return success;
}

vfs::UInt32 vfs::CUncompressedLibraryBase::GetReadLocation(tFileType *pFileHandle)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return -1;
	}
	return it->second.uiCurrentReadPosition;
}

bool vfs::CUncompressedLibraryBase::SetReadLocation(tFileType *pFileHandle, vfs::UInt32 uiPositionInBytes)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return false;
	}
	if( (uiPositionInBytes < 0) || (uiPositionInBytes >= (vfs::Int32)(it->second.uiFileSize)) )
	{
		return false;
	}
	// uiCurrentReadPosition is offset to file-offset
	it->second.uiCurrentReadPosition = uiPositionInBytes;
	return true;
}

bool vfs::CUncompressedLibraryBase::SetReadLocation(tFileType *pFileHandle, Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		return false;
	}
	if( abs(uiOffsetInBytes) > (Int32)(it->second.uiFileSize) ) 
	{
		// cannot be corrent in any case
		return false;
	}
	if(eSeekDir == IBaseFile::SD_BEGIN)
	{
		if(uiOffsetInBytes < 0)
		{
			return false;
		}
		it->second.uiCurrentReadPosition = uiOffsetInBytes;
		return true;
	}
	else if(eSeekDir == IBaseFile::SD_CURRENT)
	{
		vfs::Int32 temp = it->second.uiCurrentReadPosition + uiOffsetInBytes;
		if( (temp < 0) || (temp > (Int32)(it->second.uiFileSize)) )
		{
			return false;
		}
		it->second.uiCurrentReadPosition = temp;
		return true;
	}
	else if(eSeekDir == IBaseFile::SD_END)
	{
		if(uiOffsetInBytes > 0)
		{
			return false;
		}
		it->second.uiCurrentReadPosition = it->second.uiFileSize - uiOffsetInBytes;
		return true;
	}
	return false;
}

bool vfs::CUncompressedLibraryBase::GetFileSize(tFileType *pFileHandle, UInt32& uiFileSize)
{
	tLibData::iterator it = m_mapLibData.find(pFileHandle);
	if(it == m_mapLibData.end())
	{
		// wrong file handle
		uiFileSize = 0;
		return false;
	}
	uiFileSize = it->second.uiFileSize;
	return true;
}

void vfs::CUncompressedLibraryBase::GetSubDirList(std::list<vfs::Path>& rlSubDirs)
{
	tDirCatalogue::iterator it = m_catDirs.begin();
	for(;it != m_catDirs.end(); ++it)
	{
		rlSubDirs.push_back(it->first);
	}
}

vfs::CUncompressedLibraryBase::Iterator vfs::CUncompressedLibraryBase::begin()
{
	return Iterator(new IterImpl(*this));
}

/************************************************************************/
/************************************************************************/

vfs::CUncompressedLibraryBase::IterImpl::IterImpl(vfs::CUncompressedLibraryBase &lib)
: _lib(&lib)
{
	_iter = _lib->m_mapLibData.begin();
}
vfs::CUncompressedLibraryBase::IterImpl::~IterImpl()
{
}

vfs::CUncompressedLibraryBase::tFileType* vfs::CUncompressedLibraryBase::IterImpl::value()
{
	if(_iter != _lib->m_mapLibData.end())
	{
		return _iter->first;
	}
	return NULL;
}

void vfs::CUncompressedLibraryBase::IterImpl::next()
{
	if(_iter != _lib->m_mapLibData.end())
	{
		_iter++;
	}
}



