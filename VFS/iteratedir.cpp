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


os::CIterateDirectory::CIterateDirectory(vfs::Path const& sPath, utf8string const& searchPattern)
{
#ifdef WIN32
	fSearchHandle = FindFirstFileW((sPath+searchPattern)().c_wcs().c_str(), &fFileInfo);
	if (fSearchHandle == INVALID_HANDLE_VALUE) 
	{
		std::wstringstream wss;
		wss << L"Path [" << (sPath+searchPattern)() << L"] does not exist";
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
	else if ( !FindNextFileW(fSearchHandle, &fFileInfo) )
	{
		return false;
	}
	fileName.r_wcs().assign(fFileInfo.cFileName);
	attrib = (fFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? CIterateDirectory::FA_DIRECTORY : CIterateDirectory::FA_FILE;
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
		WIN32_FIND_DATAW fd;
		memset(&fd,0,sizeof(WIN32_FIND_DATAW));
		HANDLE hFile = FindFirstFileW( sDir().c_wcs().c_str(), &fd);
		bDirExists = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;
		FindClose(hFile);
		return bDirExists;
	}
	BOOL success;
	success = CreateDirectoryW(sDir().c_wcs().c_str(),NULL);
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
	return (DeleteFileW( sDir().c_wcs().c_str() ) != FALSE);
#else
	return (remove( sDir().utf8().c_str() ) == 0);
#endif
}

