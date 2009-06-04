#include "vfs_slf_library.h"
#include "../Interface/vfs_directory_interface.h"
#include "vfs_lib_dir.h"
#include "../File/vfs_lib_file.h"

namespace slf
{
	// copy from WinDef.h
	typedef unsigned long DWORD;
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

vfs::CSLFLibrary::~CSLFLibrary() 
{
}

bool vfs::CSLFLibrary::Init()
{
	if(m_pLibraryFile)
	{
		if(!m_pLibraryFile->OpenRead())
		{
			return false;
		}
		slf::LIBHEADER LibFileHeader;
		UInt32 uiNumBytesRead;
		if(!m_pLibraryFile->Read((Byte*)&LibFileHeader,sizeof( slf::LIBHEADER ), uiNumBytesRead))
		{
			m_pLibraryFile->Close();
			return false;
		}
		if( uiNumBytesRead != sizeof( slf::LIBHEADER ) )
		{
			//Error Reading the file database header.
			m_pLibraryFile->Close();
			return false;
		}

		vfs::Path oLibPath;
		//if the library has a path
		if( strlen( LibFileHeader.sPathToLibrary ) != 0 )
		{
			oLibPath = vfs::Path( LibFileHeader.sPathToLibrary );
		}
		else
		{
			//else the library name does not contain a path ( most likely either an error or it is the default path )
			oLibPath = vfs::Path( vfs::Const::EMPTY() );
		}
		if(m_sMountPoint.empty())
		{
			m_sMountPoint = oLibPath;
		}
		else
		{
			m_sMountPoint += oLibPath;
		}

		//place the file pointer at the begining of the file headers ( they are at the end of the file )
		m_pLibraryFile->SetReadLocation(-( LibFileHeader.iEntries * (Int32)sizeof(slf::DIRENTRY) ), vfs::IBaseFile::SD_END); 

		//loop through the library and determine the number of files that are FILE_OK
		//ie.  so we dont load the old or deleted files
		slf::DIRENTRY DirEntry;
		vfs::Path oDir, oFile;
		vfs::Path oDirPath;
		for(UInt32 uiLoop=0; uiLoop < (UInt32)LibFileHeader.iEntries; uiLoop++ )
		{
			//read in the file header
			if(!m_pLibraryFile->Read((Byte*)&DirEntry, sizeof( slf::DIRENTRY ), uiNumBytesRead))
			{
				m_pLibraryFile->Close();
				return false;
			}

			if( DirEntry.ubState == slf::FILE_OK )
			{
				vfs::Path sPath(utf8string::as_utf16(DirEntry.sFileName));
				sPath.SplitLast(oDir,oFile);
				oDirPath = m_sMountPoint;
				if(!oDir.empty())
				{
					oDirPath += oDir;
				}

				// get or create according directory object
				vfs::IDirectory<ILibrary::tWriteType>* pLD = NULL;
				tDirCatalogue::iterator it = m_catDirs.find(oDirPath);
				if(it != m_catDirs.end())
				{
					pLD = it->second;
				}
				else
				{
					pLD = new vfs::CLibDirectory(oDirPath,oDirPath);
					m_catDirs.insert(std::make_pair(oDirPath,pLD));
				}
				// create file
				vfs::CLibFile *pFile = vfs::CLibFile::Create(oFile,pLD,this);
				// add file to directory
				if(!pLD->AddFile(pFile))
				{
					m_pLibraryFile->Close();
					return false;
				}
				// link file data struct to file object
				m_mapLibData.insert(std::make_pair(pFile,sFileData(DirEntry.uiLength, DirEntry.uiOffset)));
			} // end if
		} // end for
		m_pLibraryFile->Close();
		return true;
	} // end if
	// no library file
	return false;
}

