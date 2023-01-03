/* 
 * bfVFS : vfs/Ext/slf/vfs_slf_library.cpp
 *  - implements Library interface, creates library object from SLF archive files
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

#ifdef VFS_WITH_SLF

#include <vfs/Ext/slf/vfs_slf_library.h>
#include <vfs/Core/Location/vfs_lib_dir.h>
#include <vfs/vfs_config.h>
#include <vfs/Core/Interface/vfs_directory_interface.h>
#include <vfs/Core/File/vfs_lib_file.h>
#include <vfs/Core/vfs_file_raii.h>

#include <cstring>

namespace slf
{
	typedef vfs::UInt32 DWORD;
	// copy from WinDef.h
	typedef struct _FILETIME {
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;

	typedef void* HANDLE;

	const vfs::UInt32 FILENAME_SIZE		= 256;
	const vfs::UInt32 PATH_SIZE			= 80;

	const vfs::UInt32 FILE_OK			= 0;
	const vfs::UInt32 FILE_DELETED		= 0xff;
	const vfs::UInt32 FILE_OLD			= 1;
	const vfs::UInt32 FILE_DOESNT_EXIST	= 0xfe;

	struct  LIBHEADER
	{
		vfs::Byte		sLibName[ FILENAME_SIZE ];
		vfs::Byte		sPathToLibrary[ FILENAME_SIZE ];
		vfs::Int32		iEntries;
		vfs::Int32		iUsed;
		vfs::UInt16		iSort;
		vfs::UInt16		iVersion;
		vfs::UByte		fContainsSubDirectories;
		vfs::Int32		iReserved;
	};

	struct  DIRENTRY
	{
		vfs::Byte		sFileName[ FILENAME_SIZE ];
		vfs::UInt32		uiOffset;
		vfs::UInt32		uiLength;
		vfs::UInt8		ubState;
		vfs::UInt8		ubReserved;
		FILETIME		sFileTime; 
		vfs::UInt16		usReserved2;
	};
}; // end namespace slf

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

vfs::CSLFLibrary::CSLFLibrary(tReadableFile *pLibraryFile, vfs::Path const& sMountPoint, bool bOwnFile)
: vfs::CUncompressedLibraryBase(pLibraryFile,sMountPoint,bOwnFile)
{};

vfs::CSLFLibrary::~CSLFLibrary() 
{
}

bool vfs::CSLFLibrary::init()
{
	if(!m_libraryFile)
	{
		return false;
	}
	try
	{
		vfs::COpenReadFile rfile(m_libraryFile);

		slf::LIBHEADER LibFileHeader;
		vfs::size_t bytesRead = m_libraryFile->read((vfs::Byte*)&LibFileHeader, sizeof( slf::LIBHEADER ));
		VFS_THROW_IFF(bytesRead == sizeof( slf::LIBHEADER ), L"");

		vfs::Path oLibPath;
		//if the library has a path
		if( strlen( (char*)LibFileHeader.sPathToLibrary ) != 0 )
		{
			oLibPath = vfs::Path( LibFileHeader.sPathToLibrary );
		}
		else
		{
			//else the library name does not contain a path ( most likely either an error or it is the default path )
			oLibPath = vfs::Path( vfs::Const::EMPTY() );
		}
		if(m_mountPoint.empty())
		{
			m_mountPoint = oLibPath;
		}
		else
		{
			m_mountPoint += oLibPath;
		}

		//place the file pointer at the begining of the file headers ( they are at the end of the file )
		m_libraryFile->setReadPosition(-( LibFileHeader.iEntries * (vfs::Int32)sizeof(slf::DIRENTRY) ), vfs::IBaseFile::SD_END); 

		//loop through the library and determine the number of files that are FILE_OK
		//ie.  so we dont load the old or deleted files
		slf::DIRENTRY DirEntry;
		vfs::Path oDir, oFile;
		vfs::Path oDirPath;
		for(vfs::UInt32 uiLoop=0; uiLoop < (vfs::UInt32)LibFileHeader.iEntries; uiLoop++ )
		{
			//read in the file header
			//memset(&DirEntry,0,sizeof(DirEntry));
			bytesRead = m_libraryFile->read((Byte*)&DirEntry, sizeof( slf::DIRENTRY ));
			VFS_THROW_IFF(bytesRead == sizeof( slf::DIRENTRY ), L"");

			if( DirEntry.ubState == slf::FILE_OK )
			{
				vfs::Path sPath(vfs::String::as_utf16(DirEntry.sFileName));
				sPath.splitLast(oDir,oFile);
				oDirPath = m_mountPoint;
				if(!oDir.empty())
				{
					oDirPath += oDir;
				}

				// get or create according directory object
				vfs::TDirectory<ILibrary::tWriteType>* pLD = NULL;
				tDirCatalogue::iterator it = m_dirs.find(oDirPath);
				if(it != m_dirs.end())
				{
					pLD = it->second;
				}
				else
				{
					pLD = new vfs::CLibDirectory(oDirPath,oDirPath);
					m_dirs.insert(std::make_pair(oDirPath,pLD));
				}
				// create file
				vfs::CLibFile *pFile = vfs::CLibFile::create(oFile,pLD,this);
				// add file to directory
				VFS_THROW_IFF(pLD->addFile(pFile), L"");

				// link file data struct to file object
				m_fileData.insert(std::make_pair(pFile, SFileData(DirEntry.uiLength, DirEntry.uiOffset)));
			} // end if
		} // end for
		return true;
	}
	catch(std::exception& ex)
	{
		VFS_LOG_ERROR(ex.what());
		return false;
	}
}

#endif // VFS_WITH_SLF
