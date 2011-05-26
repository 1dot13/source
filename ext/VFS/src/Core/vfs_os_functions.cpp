/* 
 * bfVFS : vfs/Core/os_functions.cpp
 *  - abstractions for OS dependant code
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

#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/vfs_string.h>
#include <vfs/Tools/vfs_log.h>
#include <vfs/Aspects/vfs_settings.h>

#include <cstdlib>
#include <cstring>
#include <sstream>

#ifndef WIN32
#	include "errno.h"
#	include "sys/stat.h"
#endif

vfs::OS::CIterateDirectory::CIterateDirectory(vfs::Path const& sPath, vfs::String const& searchPattern)
{
#ifdef WIN32
	if(!vfs::Settings::getUseUnicode())
	{
		std::string s;
		vfs::String::narrow((sPath+searchPattern).c_wcs(), s);
		fSearchHandle = FindFirstFileA(s.c_str(), &fFileInfoA);
	}
	else
	{
		fSearchHandle = FindFirstFileW((sPath+searchPattern).c_wcs().c_str(), &fFileInfoW);
	}
	if (fSearchHandle == INVALID_HANDLE_VALUE) 
	{
		DWORD error = GetLastError();
		VFS_THROW(_BS(L"Error accessing path [") << (sPath+searchPattern) << L"],  error code : " << error << _BS::wget);
	}
#else
	count = scandir(vfs::String::as_utf8(sPath()).c_str(),&files,NULL,NULL);
	if(count == -1)
	{
		vfs::String err = strerror(errno);
		VFS_THROW(err);
	}
	current_pos = 0;
#endif
	fFirstRequest = true;
}

vfs::OS::CIterateDirectory::~CIterateDirectory() 
{ 
#ifdef WIN32
	FindClose(fSearchHandle);
#else
#endif
}

bool vfs::OS::CIterateDirectory::nextFile(vfs::String &fileName, CIterateDirectory::EFileAttribute &attrib)
{
#ifdef WIN32
	VFS_THROW_IFF(fSearchHandle != INVALID_HANDLE_VALUE, L"Invalid Handle Value");
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
			fileName.r_wcs().assign( vfs::String::widen( fFileInfoA.cFileName, strlen(fFileInfoA.cFileName) ) );
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
		fileName = vfs::String(entry->d_name);
		attrib = (entry->d_type == DT_DIR) ? CIterateDirectory::FA_DIRECTORY : CIterateDirectory::FA_FILE;
		current_pos++;
		return true;
	}
	return false;
#endif
}

bool vfs::OS::checkRealDirectory(vfs::Path const& sDir)
{
#ifdef WIN32
	bool bDirExists = false;
	if(!vfs::Settings::getUseUnicode())
	{
		WIN32_FIND_DATAA fd;
		memset(&fd,0,sizeof(WIN32_FIND_DATAA));

		std::string s;
		vfs::String::narrow(sDir.c_wcs(), s);

		HANDLE hFile = FindFirstFileA( s.c_str(), &fd);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();
			VFS_LOG_ERROR(_BS(L"Directory [") << sDir << L"] does not exist : " << error << _BS::wget);
		}

		bDirExists = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;

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
			VFS_LOG_ERROR(_BS(L"Directory [") << sDir << L"] does not exist : " << error << _BS::wget);
		}

		bDirExists = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;

		FindClose(hFile);
	}
	return bDirExists;
#else
	int result = access(sDir.to_string().c_str(), F_OK /*0400*/);
	if(result == -1)
	{
		vfs::String err = strerror(errno);
		VFS_LOG_ERROR(err.c_str());
		return false;
	}
	return true;
#endif
}

bool vfs::OS::createRealDirectory(vfs::Path const& sDir)
{
#ifdef WIN32
	BOOL success;
	vfs::String::str_t const& str = sDir.c_wcs();
	success = vfs::Settings::getUseUnicode() ? 
		CreateDirectoryW(sDir.c_str(),NULL) :
		CreateDirectoryA( vfs::String::narrow( str.c_str(), str.length() ).c_str(), NULL );
	if(success == 0)
	{
		DWORD error = GetLastError();
		if(error == ERROR_ALREADY_EXISTS)
		{
			return true;
		}

		VFS_LOG_ERROR((_BS("Could not create directory [") << sDir << L"] : " << error).get());

		return false;
	}
	return true;
#else
	int result = mkdir(sDir.to_string().c_str(), S_IRWXU | S_IRGRP /*0777*/);
	if(result == -1 && errno != EEXIST)
	{
		vfs::String err = strerror(errno);
		VFS_THROW(err);
	}
	return true;
#endif
}

bool vfs::OS::FileAttributes::getFileAttributes(vfs::Path const& sDir, vfs::UInt32& uiAttribs)
{
#ifdef WIN32
	DWORD attribs = vfs::Settings::getUseUnicode() ?
		GetFileAttributesW(sDir.c_str()) :
		GetFileAttributesA(vfs::String::narrow(sDir.c_str(), sDir.length()).c_str());

	if(attribs == INVALID_FILE_ATTRIBUTES)
	{
		DWORD error = GetLastError();
		VFS_LOG_ERROR(_BS(L"Invalid File Attributes : ") << error << _BS::wget);
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

bool vfs::OS::deleteRealFile(vfs::Path const& sDir)
{
#ifdef WIN32
	BOOL del = vfs::Settings::getUseUnicode() ? 
		DeleteFileW( sDir.c_str() ) :
		DeleteFileA( vfs::String::narrow(sDir.c_str(), sDir.length()).c_str() );
	if(!del)
	{
		DWORD err = GetLastError();
		if(err != NO_ERROR)
		{
			VFS_LOG_ERROR(L"Could not delete file");
		}
	}
	return (del != FALSE);
#else
	return (remove( vfs::String::as_utf8(sDir()).c_str() ) == 0);
#endif
}

void vfs::OS::getExecutablePath(vfs::Path& sDir, vfs::Path& sFile)
{
#ifdef WIN32
	DWORD error;
	if(!vfs::Settings::getUseUnicode())
	{
		char path[256];
		if( 0 != (error = ::GetModuleFileNameA(NULL, path, 256)) )
		{
			vfs::Path(vfs::String::widen(path,256)).splitLast(sDir, sFile);
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
		VFS_THROW(_BS(L"Could not get current directory [") << 
			(!vfs::Settings::getUseUnicode() ? L"no unicode" : L"unicode") <<
			L"],  error code : " << code << _BS::wget);
	}
#else
	char buf[256];
	ssize_t size = readlink("/proc/self/exe", buf, 256);
	if(size == -1)
	{
		vfs::String err = strerror(errno);
		VFS_THROW(err);
	}
	buf[size] = 0;
	vfs::Path exedir(buf);
	exedir.splitLast(sDir, sFile);
#endif
}

void vfs::OS::getCurrentDirectory(vfs::Path& sPath)
{
#ifdef WIN32
	DWORD error;
	vfs::Path path;
	if( !vfs::Settings::getUseUnicode() )
	{
		char path[256];
		if( 0 != (error = ::GetCurrentDirectoryA(256, path)) )
		{
			sPath = vfs::Path(vfs::String::widen(path,256));
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
		VFS_THROW(_BS(L"Could not determine current directory ") <<
			(!vfs::Settings::getUseUnicode() ? L"[no unicode]" : L"[unicode]") <<
			L",  error code : " << _BS::wget);
	}
#else
	char* cwd = getcwd(NULL,0);
	if(!cwd)
	{
		vfs::String err = strerror(errno);
		VFS_THROW(err);
	}
	sPath = cwd;
	free(cwd);
#endif
}

void vfs::OS::setCurrectDirectory(vfs::Path const& sPath)
{
#ifdef WIN32
	if(!vfs::Settings::getUseUnicode())
	{
		std::string str;
		vfs::String::narrow( sPath.c_wcs(), str );
		VFS_THROW_IFF( ::SetCurrentDirectoryA( str.c_str() ) == TRUE,
			_BS(L"Could not set current directory [no unicode] : ") << sPath << _BS::wget );
	}
	else
	{
		VFS_THROW_IFF( ::SetCurrentDirectoryW( sPath.c_str() ) == TRUE,
			_BS(L"Could not set current directory [unicode] : ") << sPath <<_BS::wget );
	}
#else
	if(chdir(sPath.to_string().c_str()) != 0)
	{
		vfs::String err = strerror(errno);
		VFS_THROW(err);
	}
#endif
}

bool vfs::OS::getEnv(vfs::String const& key, vfs::String& value)
{
#ifdef _MSC_VER
	wchar_t *val_buf = NULL;
	::size_t buf_len; 
	errno_t err = _wdupenv_s(&val_buf,&buf_len, key.c_str());
	if(err == 0 && val_buf)
	{
		// success
		value = val_buf;
		free(val_buf);
	}
	return err == 0;
#else
	char* val_buf = getenv(key.utf8().c_str());
	if(val_buf)
	{
		value = val_buf;
		return true;
	}
	return false;
#endif
}
