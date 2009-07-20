//
// Snap: Implementation of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#include "iteratedir.h"
#include "vfs_debug.h"
#include <sstream>

#include "utf8string.h"

#ifndef WIN32
int file_select(struct direct *entry)
{
/*	if((strcmp(entry->d_name,".") == 0) || ( strcmp(entry->d_name,"..") == 0))
	{
		return (FALSE);
	}
	else
	{
		return (TRUE);
	}*/
	return (TRUE);
}
#endif


extern bool g_VFS_NO_UNICODE;

os::CIterateDirectory::CIterateDirectory(vfs::Path const& sPath, utf8string const& searchPattern)
{
#ifdef WIN32
	if(g_VFS_NO_UNICODE)
	{
		std::string s;
		utf8string::narrow((sPath+searchPattern)().c_wcs(), s);
		fSearchHandle = FindFirstFileA(s.c_str(), &fFileInfoA);
	}
	else
	{
		fSearchHandle = FindFirstFileW((sPath+searchPattern)().c_wcs().c_str(), &fFileInfoW);
	}
	if (fSearchHandle == INVALID_HANDLE_VALUE) 
	{
		DWORD error = GetLastError();
		std::wstringstream wss;
		wss << L"Error accessing path [" << (sPath+searchPattern)() << L"],  error code : " << error;
		THROWEXCEPTION(wss.str().c_str());
	}
#else
	count = scandir(sPath().utf8().c_str(),&files,NULL,NULL);
	current_pos = 0;
#endif
	fFirstRequest = true;
}

os::CIterateDirectory::~CIterateDirectory() 
{ 
#ifdef WIN32
	FindClose(fSearchHandle);
#else
#endif
}

bool os::CIterateDirectory::NextFile(utf8string &fileName, CIterateDirectory::EFileAttribute &attrib)
{
#ifdef WIN32
	THROWIFFALSE(fSearchHandle != INVALID_HANDLE_VALUE, L"Invalid Handle Value");
	if (fFirstRequest)
	{
		fFirstRequest = false;
	}
	//else
	{
		if(g_VFS_NO_UNICODE)
		{
			if( !FindNextFileA(fSearchHandle, &fFileInfoA) ) 
			{
				return false;
			}
			fileName.r_wcs().assign( utf8string::widen( fFileInfoA.cFileName, strlen(fFileInfoA.cFileName) ) );
			attrib = (fFileInfoA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? CIterateDirectory::FA_DIRECTORY : CIterateDirectory::FA_FILE;
		}
		else
		{
			if ( !FindNextFileW(fSearchHandle, &fFileInfoW) )
			{
				return false;
			}
			fileName.r_wcs().assign(fFileInfoW.cFileName);
			attrib = (fFileInfoW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? CIterateDirectory::FA_DIRECTORY : CIterateDirectory::FA_FILE;
		}
	}
	return true;
#else
	if(current_pos < count)
	{
		struct dirent* entry = files[current_pos];
		fileName.assign(VfsString(entry->d_name)());
		attrib = (entry->d_type == DT_DIR) ? CIterateDir::FA_DIRECTORY : CIterateDir::FA_FILE;
		current_pos++;
		return true;
	}
	return false;
#endif
}

bool os::CreateRealDirecory(vfs::Path& sDir, bool bDoNotCreate)
{
#if WIN32
	if(bDoNotCreate)
	{
		bool bDirExists = false;
		if(g_VFS_NO_UNICODE)
		{
			WIN32_FIND_DATAA fd;
			memset(&fd,0,sizeof(WIN32_FIND_DATAA));

			std::string s;
			utf8string::narrow(sDir().c_wcs(), s);

			HANDLE hFile = FindFirstFileA( s.c_str(), &fd);
			bDirExists = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;
			FindClose(hFile);
		}
		else
		{
			WIN32_FIND_DATAW fd;
			memset(&fd,0,sizeof(WIN32_FIND_DATAW));
			HANDLE hFile = FindFirstFileW( sDir().c_wcs().c_str(), &fd);
			bDirExists = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;
			FindClose(hFile);
		}
		return bDirExists;
	}
	BOOL success;
	utf8string::str_t const& str = sDir().c_wcs();
	success = g_VFS_NO_UNICODE ? 
		CreateDirectoryA( utf8string::narrow( str.c_str(), str.length() ).c_str(), NULL ) :
		CreateDirectoryW(sDir().c_wcs().c_str(),NULL);
	if(success == 0)
	{
		DWORD error = GetLastError();
		if(error == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
		return false;
	}
	return true;
#else
#endif
}

bool os::DeleteRealFile(vfs::Path &sDir)
{
#ifdef WIN32
	utf8string::str_t const& str = sDir().c_wcs();
	BOOL del = g_VFS_NO_UNICODE ? 
		DeleteFileA( utf8string::narrow(str.c_str(), str.length()).c_str() ) :
		DeleteFileW( str.c_str() );
	return (del != FALSE);
#else
	return (remove( sDir().utf8().c_str() ) == 0);
#endif
}

