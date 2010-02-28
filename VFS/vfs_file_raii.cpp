#include "vfs_file_raii.h"
#include "vfs.h"

#include <sstream>
/********************************************************************************************/
/********************************************************************************************/

vfs::COpenReadFile::COpenReadFile(vfs::Path const& sPath, vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::IBaseFile *pFile = getVFS()->getFile(sPath,eSF);
	THROWIFFALSE(pFile,
		BuildString().add(L"file \"").add(sPath).add(L"\" does not exist").get());
	
	m_pFile = vfs::tReadableFile::cast(pFile);
	
	THROWIFFALSE(m_pFile,
		BuildString().add(L"File \"").add(sPath).add(L"\" is not readable").get());
	
	THROWIFFALSE(m_pFile->openRead(),
		BuildString().add(L"Could not open file : ").add(m_pFile->getPath()).get());
}

vfs::COpenReadFile::COpenReadFile(vfs::tReadableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		THROWIFFALSE(m_pFile,
			BuildString().add(L"Invalid file object").get());

		THROWIFFALSE(m_pFile->openRead(),
			BuildString().add(L"Could not open file : ").add(pFile->getPath()).get());
	}
	catch(CBasicException &ex)
	{
		RETHROWEXCEPTION(L"",&ex);
	}
}
vfs::COpenReadFile::~COpenReadFile()
{
	if(m_pFile)
	{
		m_pFile->close();
		m_pFile = NULL;
	}
}

vfs::tReadableFile& vfs::COpenReadFile::file()
{
	return *m_pFile;
}

void vfs::COpenReadFile::release()
{
	m_pFile = NULL;
}

/**************************************************************************/

vfs::COpenWriteFile::COpenWriteFile(vfs::Path const& sPath, 
									bool bCreate, 
									bool bTruncate, 
									vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::IBaseFile *pFile = getVFS()->getFile(sPath,eSF);
	if(!pFile && bCreate)
	{
		if(getVFS()->createNewFile(sPath))
		{
			pFile = getVFS()->getFile(sPath,eSF);
		}
		else
		{
			THROWEXCEPTION(BuildString().add(L"Could not create VFS file \"").add(sPath).add(L"\"").get());
		}
	}
	THROWIFFALSE(pFile,
		BuildString().add(L"File \"").add(sPath).add(L"\" not found").get());

	m_pFile = vfs::tWritableFile::cast(pFile);

	THROWIFFALSE(m_pFile,
		BuildString().add(L"File \"").add(sPath).add(L"\" exists, but is not writable").get());
	
	THROWIFFALSE(m_pFile->openWrite(bCreate,bTruncate),
		BuildString().add(L"File \"").add(sPath).add(L"\" could not be opened for writing").get());
}
vfs::COpenWriteFile::COpenWriteFile(vfs::tWritableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		THROWIFFALSE(m_pFile,
			BuildString().add(L"Invalid file object").get());
		THROWIFFALSE(m_pFile->openWrite(true,false),
			BuildString().add(L"Could not open file : ").add(m_pFile->getPath()).get());
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(L"",&ex);
	};
}
vfs::COpenWriteFile::~COpenWriteFile()
{
	if(m_pFile)
	{
		m_pFile->close();
		m_pFile = NULL;
	}
}

vfs::tWritableFile& vfs::COpenWriteFile::file()
{
	return *m_pFile;
}

void vfs::COpenWriteFile::release()
{
	m_pFile = NULL;
}

