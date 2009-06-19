#include "vfs_file_raii.h"
#include "vfs.h"

#include <sstream>
/********************************************************************************************/
/********************************************************************************************/

vfs::COpenReadFile::COpenReadFile(vfs::Path const& sPath, vfs::CVirtualFile::ESearchFile eSF)
{
	vfs::IBaseFile *pFile = GetVFS()->GetFile(sPath,eSF);
	THROWIFFALSE(pFile, (L"file \"" + sPath().c_wcs() + L"\" does not exist").c_str());
	m_pFile = vfs::tReadableFile::Cast(pFile);
	THROWIFFALSE(m_pFile, L"not readable");
	THROWIFFALSE(m_pFile->OpenRead(), L"open read failed");
}

vfs::COpenReadFile::COpenReadFile(vfs::tReadableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		THROWIFFALSE(m_pFile, L"no file");
		THROWIFFALSE(m_pFile->OpenRead(), L"not open");
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
		m_pFile->Close();
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
	vfs::IBaseFile *pFile = GetVFS()->GetFile(sPath,eSF);
	if(!pFile && bCreate)
	{
		if(GetVFS()->CreateNewFile(sPath))
		{
			pFile = GetVFS()->GetFile(sPath,eSF);
		}
		else
		{
			std::wstringstream wss;
			wss << L"Could not create VFS file \"" << sPath().c_wcs() << L"\"";
			THROWEXCEPTION(wss.str().c_str());
		}
	}
	if(!pFile)
	{
		std::wstringstream wss;
		wss << L"File \"" << sPath().c_wcs() << L"\" not found";
		THROWEXCEPTION(wss.str().c_str());
	}
	m_pFile = vfs::tWriteableFile::Cast(pFile);
	if(!m_pFile)
	{
		std::wstringstream wss;
		wss << L"File \"" << sPath().c_wcs() << L"\" exists, but is not writeable";
		THROWEXCEPTION(wss.str().c_str());
	}
	if(!m_pFile->OpenWrite(bCreate,bTruncate))
	{
		std::wstringstream wss;
		wss << L"File \"" << sPath().c_wcs() << L"\" could not be opened for writing";
		THROWEXCEPTION(wss.str().c_str());
	}
}
vfs::COpenWriteFile::COpenWriteFile(vfs::tWriteableFile *pFile)
{
	try
	{
		m_pFile = pFile;
		THROWIFFALSE(m_pFile, L"no file");
		THROWIFFALSE(m_pFile->OpenWrite(true,false), (L"File not open : " + m_pFile->GetFullPath()().c_wcs()).c_str());
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
		m_pFile->Close();
		m_pFile = NULL;
	}
}

vfs::tWriteableFile& vfs::COpenWriteFile::file()
{
	return *m_pFile;
}

void vfs::COpenWriteFile::release()
{
	m_pFile = NULL;
}

