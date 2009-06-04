#include "vfs_7z_library.h"
#include "vfs_lib_dir.h"
#include "../File/vfs_lib_file.h"
#include "../vfs_file_raii.h"

namespace sz
{
extern "C"
{
#include "7zCrc.h"
#include "Archive/7z/7zAlloc.h"
#include "Archive/7z/7zExtract.h"
#include "Archive/7z/7zIn.h"
}
}

/********************************************************************************************/
/***                                   my 7z extensions                                   ***/
/********************************************************************************************/

namespace szExt
{
	typedef struct CSzVfsFile
	{
		vfs::tReadableFile*	file;
	} CSzVfsFile;

	typedef struct CVfsFileInStream
	{
		sz::ISeekInStream	s;
		CSzVfsFile			file;
	} CVfsFileInStream;


	static sz::SRes VfsFileInStream_Read(void *pp, void *buf, size_t *size)
	{
		CVfsFileInStream *p = (CVfsFileInStream *)pp;
		vfs::UInt32 to_read = *size;
		vfs::UInt32 has_read = 0;
		sz::SRes res;
		if(p->file.file->Read((vfs::Byte*)buf,to_read, has_read))
		{
			res = SZ_OK;
		}
		else
		{
			res = SZ_ERROR_READ;
		}
		*size = has_read;
		return res;
	}

	static sz::SRes VfsFileInStream_Seek(void *pp, sz::Int64 *pos, sz::ESzSeek origin)
	{
		CVfsFileInStream *p = (CVfsFileInStream *)pp;

		vfs::IBaseFile::ESeekDir eSD;
		switch (origin)
		{
			case sz::SZ_SEEK_SET:
				eSD = vfs::IBaseFile::SD_BEGIN;
				break;
			case sz::SZ_SEEK_CUR:
				eSD = vfs::IBaseFile::SD_CURRENT;
				break;
			case sz::SZ_SEEK_END:
				eSD = vfs::IBaseFile::SD_END;
				break;
			default: 
				return ERROR_INVALID_PARAMETER;
		}
		vfs::Int32 _pos = (vfs::Int32)(*pos); // only 32 bit 
		sz::SRes res;
		if(p->file.file->SetReadLocation(_pos,eSD))
		{
			*pos = p->file.file->GetReadLocation();
			res = SZ_OK;
		}
		else
		{
			res = SZ_ERROR_READ;
		}
		return res;
	}

	void VfsFileInStream_CreateVTable(CVfsFileInStream *p)
	{
		p->s.Read = VfsFileInStream_Read;
		p->s.Seek = VfsFileInStream_Seek;
	}
}; // end namespace szExt

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

#define k_Copy 0

sz::UInt64 GetSum(const sz::UInt64 *values, sz::UInt32 index)
{
	sz::UInt64 sum = 0;
	sz::UInt32 i;
	for (i = 0; i < index; i++)
	{
		sum += values[i];
	}
	return sum;
}

bool vfs::CUncompressed7zLibrary::Init()
{
	if(!m_pLibraryFile)
	{
		return false;
	}
	szExt::CVfsFileInStream		archiveStream;
	sz::CLookToRead				lookStream;
	sz::CSzArEx					db;
	sz::SRes					res;
	sz::ISzAlloc				allocImp;
	sz::ISzAlloc				allocTempImp;

	if(!m_pLibraryFile->OpenRead())
	{
		return false;
	}
	archiveStream.file.file = m_pLibraryFile;

	szExt::VfsFileInStream_CreateVTable(&archiveStream);
	sz::LookToRead_CreateVTable(&lookStream, False);

	lookStream.realStream = &archiveStream.s;
	sz::LookToRead_Init(&lookStream);

	allocImp.Alloc = sz::SzAlloc;
	allocImp.Free = sz::SzFree;

	allocTempImp.Alloc = sz::SzAllocTemp;
	allocTempImp.Free = sz::SzFreeTemp;

	sz::CrcGenerateTable();

	sz::SzArEx_Init(&db);
	if( SZ_OK != (res = sz::SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp)) )
	{
		std::stringstream wss;
		wss << "Could not open 7z archive [" << m_pLibraryFile->GetFullPath()().utf8() << "]";
		THROWEXCEPTION(wss.str().c_str());
	}

	vfs::IDirectory<ILibrary::tWriteType>* pLD = NULL;
	vfs::Path oDir, oFile;
	vfs::Path oDirPath;

	for(vfs::UInt32 i = 0; i < db.db.NumFiles; i++)
	{
		sz::CSzFileItem *f = db.db.Files + i;
		if (f->IsDir)
		{
			continue;
		}
		vfs::Path sPath(f->Name);
		sPath.SplitLast(oDir,oFile);
		oDirPath = m_sMountPoint;
		if(!oDir.empty())
		{
			oDirPath += oDir;
		}

		// determine offset and size
		sz::UInt32 folderIndex = db.FileIndexToFolderIndexMap[i];

		sz::CSzFolder *folder = db.db.Folders + folderIndex;
		sz::UInt64 unpackSizeSpec = sz::SzFolder_GetUnpackSize(folder);
		size_t unpackSize = (size_t)unpackSizeSpec;
		sz::UInt64 startOffset = sz::SzArEx_GetFolderStreamPos(&db, folderIndex, 0);

		const sz::UInt64 *packSizes = db.db.PackSizes + db.FolderStartPackStreamIndex[folderIndex];

		//CSzCoderInfo *coder = &folder->Coders[0];
		//if (coder->MethodID == k_Copy)
		//{
		//	UInt32 si = 0;
		//	UInt64 offset;
		//	UInt64 inSize;
		//	offset = GetSum(packSizes, si);
		//	inSize = packSizes[si];
		//}

		// get or create according directory object
		tDirCatalogue::iterator it = m_catDirs.find(oDirPath);
		if(it != m_catDirs.end())
		{
			pLD = it->second;
		}
		else
		{
			pLD = new CLibDirectory(oDir,oDirPath);
			m_catDirs.insert(std::make_pair(oDirPath,pLD));
		}
		// create file
		vfs::CLibFile *pFile = vfs::CLibFile::Create(oFile,pLD,this,_allocator);
		// add file to directory
		if(!pLD->AddFile(pFile))
		{
			// something is wrong
			m_pLibraryFile->Close();
			return false;
		}
		// link file data struct to file object
		m_mapLibData.insert(std::pair<tFileType*,sFileData>(pFile,sFileData(unpackSize, (UInt32)startOffset)));
	}
	return true;
}

