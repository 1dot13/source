#include "os_functions.h"
#include "vfs_debug.h"
#include <sstream>
#include "Tools/Log.h"
#include "utf8string.h"
#include "vfs_settings.h"

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
	return 1;
}
#endif


os::CIterateDirectory::CIterateDirectory(vfs::Path const& sPath, utf8string const& searchPattern)
{
#ifdef WIN32
	if(!vfs::Settings::getUseUnicode())
	{
		std::string s;
		utf8string::narrow((sPath+searchPattern).c_wcs(), s);
		fSearchHandle = FindFirstFileA(s.c_str(), &fFileInfoA);
	}
	else
	{
		fSearchHandle = FindFirstFileW((sPath+searchPattern).c_wcs().c_str(), &fFileInfoW);
	}
	if (fSearchHandle == INVALID_HANDLE_VALUE) 
	{
		DWORD error = GetLastError();
		std::wstringstream wss;
		wss << L"Error accessing path [" << (sPath+searchPattern)() << L"],  error code : " << error;
		THROWEXCEPTION(wss.str().c_str());
	}
#else
	count = scandir(utf8string::as_utf8(sPath()).c_str(),&files,NULL,NULL);
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

bool os::CIterateDirectory::nextFile(utf8string &fileName, CIterateDirectory::EFileAttribute &attrib)
{
#ifdef WIN32
	THROWIFFALSE(fSearchHandle != INVALID_HANDLE_VALUE, L"Invalid Handle Value");
	if (fFirstRequest)
	{
		fFirstRequest = false;
	}
	//else
	{
		if(!vfs::Settings::getUseUnicode())
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
		fileName = utf8string(entry->d_name);
		attrib = (entry->d_type == DT_DIR) ? CIterateDirectory::FA_DIRECTORY : CIterateDirectory::FA_FILE;
		current_pos++;
		return true;
	}
	return false;
#endif
}

bool os::createRealDirectory(vfs::Path const& sDir, bool bDoNotCreate)
{
#if WIN32
	if(bDoNotCreate)
	{
		bool bDirExists = false;
		if(!vfs::Settings::getUseUnicode())
		{
			WIN32_FIND_DATAA fd;
			memset(&fd,0,sizeof(WIN32_FIND_DATAA));

			std::string s;
			utf8string::narrow(sDir.c_wcs(), s);

			HANDLE hFile = FindFirstFileA( s.c_str(), &fd);
			if(hFile == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				CLog log(L"errors.log", true);
				log << L"Directory [" << sDir() << L"] does not exist : " << (int)error << CLog::ENDL;
			}

			// WANNE - MP: This should fix the missing sync folder event if it exists (by Frostregen)
			bDirExists = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
			//bDirExists = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;

			FindClose(hFile);
		}
		else
		{
			WIN32_FIND_DATAW fd;
			memset(&fd,0,sizeof(WIN32_FIND_DATAW));
			HANDLE hFile = FindFirstFileW( sDir.c_str(), &fd);
			if(hFile == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				CLog log(L"errors.log", true);
				log << L"Directory [" << sDir() << L"] does not exist : " << (int)error << CLog::ENDL;
			}

			// WANNE - MP: This should fix the missing sync folder event if it exists (by Frostregen)
			bDirExists = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
			//bDirExists = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;

			FindClose(hFile);
		}
		return bDirExists;
	}
	BOOL success;
	utf8string::str_t const& str = sDir.c_wcs();
	success = vfs::Settings::getUseUnicode() ? 
		CreateDirectoryW(sDir.c_str(),NULL) :
		CreateDirectoryA( utf8string::narrow( str.c_str(), str.length() ).c_str(), NULL );
	if(success == 0)
	{
		DWORD error = GetLastError();
		if(error == ERROR_ALREADY_EXISTS)
		{
			return true;
		}

		CLog log(L"errors.log", true);
		log << L"Could not create directory [" << sDir() << L"] : " << (int)error << CLog::ENDL;

		return false;
	}
	return true;
#else
	return false;
#endif
}

bool os::FileAttributes::getFileAttributes(vfs::Path const& sDir, vfs::UInt32& uiAttribs)
{
#ifdef WIN32
	DWORD attribs = vfs::Settings::getUseUnicode() ?
		GetFileAttributesW(sDir.c_str()) :
		GetFileAttributesA(utf8string::narrow(sDir.c_str(), sDir.length()).c_str());

	if(attribs == INVALID_FILE_ATTRIBUTES)
	{
		DWORD error = GetLastError();
		CLog log(L"errors.log");
		log << L"Invalid File Attributes : " << (int)error << CLog::ENDL;
		return false;
	}

	for(vfs::UInt32 attribMask = 0x80000000; attribMask > 0; attribMask >>= 1)
	{
		switch(attribs & attribMask)
		{
			case FILE_ATTRIBUTE_ARCHIVE:
				uiAttribs |= ATTRIB_ARCHIVE;
				break;
			case FILE_ATTRIBUTE_DIRECTORY:
				uiAttribs |= ATTRIB_DIRECTORY;
				break;
			case FILE_ATTRIBUTE_HIDDEN:
				uiAttribs |= ATTRIB_HIDDEN;
				break;
			case FILE_ATTRIBUTE_NORMAL:	
				uiAttribs |= ATTRIB_NORMAL;
				break;
			case FILE_ATTRIBUTE_READONLY:
				uiAttribs |= ATTRIB_READONLY;
				break;
			case FILE_ATTRIBUTE_SYSTEM:
				uiAttribs |= ATTRIB_SYSTEM;
				break;
			case FILE_ATTRIBUTE_TEMPORARY:
				uiAttribs |= ATTRIB_TEMPORARY;
				break;
			case FILE_ATTRIBUTE_COMPRESSED:
				uiAttribs |= ATTRIB_COMPRESSED;
				break;
			case FILE_ATTRIBUTE_OFFLINE:
				uiAttribs |= ATTRIB_OFFLINE;
				break;
		}
	}
#else
#endif
	return true;
}

bool os::deleteRealFile(vfs::Path const& sDir)
{
#ifdef WIN32
	BOOL del = vfs::Settings::getUseUnicode() ? 
		DeleteFileW( sDir.c_str() ) :
		DeleteFileA( utf8string::narrow(sDir.c_str(), sDir.length()).c_str() );
	return (del != FALSE);
#else
	return (remove( utf8string::as_utf8(sDir()).c_str() ) == 0);
#endif
}

void os::getExecutablePath(vfs::Path& sDir, vfs::Path& sFile)
{
#ifdef WIN32
	DWORD error;
	if(!vfs::Settings::getUseUnicode())
	{
		char path[256];
		if( 0 != (error = ::GetModuleFileNameA(NULL, path, 256)) )
		{
			vfs::Path(utf8string::widen(path,256)).splitLast(sDir, sFile);
		}
	}
	else
	{
		wchar_t path[256];
		if( 0 != (error = ::GetModuleFileNameW(NULL, path, 256)) )
		{
			vfs::Path(path).splitLast(sDir, sFile);
		}
	}
	if(error == 0)
	{
		DWORD code = GetLastError();
		THROWEXCEPTION( BuildString().add(L"Could not get current directory [").add(
			(!vfs::Settings::getUseUnicode()) ? L"[no unicode]" : L"[unicode]").add(L"],  error code : ").add(code).get());
	}
#else
#endif
}

void os::getCurrentDirectory(vfs::Path& sPath)
{
#ifdef WIN32
	DWORD error;
	vfs::Path path;
	if( !vfs::Settings::getUseUnicode() )
	{
		char path[256];
		if( 0 != (error = ::GetCurrentDirectoryA(256, path)) )
		{
			sPath = vfs::Path(utf8string::widen(path,256));
		}
	}
	else
	{
		wchar_t path[256];
		if( 0 != (error = ::GetCurrentDirectoryW(256, path)) )
		{
			sPath = vfs::Path(path);
		}
	}
	if(error == 0)
	{
		DWORD code = GetLastError();
		THROWEXCEPTION(BuildString().add(L"Could not get current directory [").add(
			(!vfs::Settings::getUseUnicode()) ? L"[no unicode]" : L"[unicode]").add(L"],  error code : ").add(code).get());
	}
#else
#endif
}

void os::setCurrectDirectory(vfs::Path const& sPath)
{
#ifdef WIN32
	if(!vfs::Settings::getUseUnicode())
	{
		std::string str;
		utf8string::narrow( sPath.c_wcs(), str );
		THROWIFFALSE( ::SetCurrentDirectoryA( str.c_str() ) == TRUE,
			BuildString().add(L"Could not set current directory [no unicode] : ").add(sPath).get() );
	}
	else
	{
		THROWIFFALSE( ::SetCurrentDirectoryW( sPath.c_str() ) == TRUE,
			BuildString().add(L"Could not set current directory [unicode] : ").add(sPath).get() );
	}
#else
#endif
}


