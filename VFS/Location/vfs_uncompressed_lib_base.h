#ifndef _VFS_UNCOMPRESSED_LIB_BASE_H_
#define _VFS_UNCOMPRESSED_LIB_BASE_H_

#include "../Interface/vfs_library_interface.h"
#include "../Interface/vfs_directory_interface.h"
#include <sstream>

namespace vfs
{

	class CUncompressedLibraryBase : public vfs::ILibrary
	{
	protected:
		typedef std::map<vfs::Path, vfs::IDirectory<ILibrary::tWriteType>*, vfs::Path::Less> tDirCatalogue;
		struct sFileData
		{
			sFileData(UInt32 const& fileSize, UInt32 const& fileOffset)
				: uiFileSize(fileSize), uiFileOffset(fileOffset), uiCurrentReadPosition(0)
			{};
			UInt32 uiFileSize,uiFileOffset,uiCurrentReadPosition;
		};
		typedef std::map<tFileType*, sFileData> tLibData;
		
		class IterImpl : public tClassType::Iterator::IImplemetation
		{
		public:
			IterImpl(CUncompressedLibraryBase& lib);
			virtual ~IterImpl();
			virtual tFileType*			value();
			virtual void				next();
		private:
			CUncompressedLibraryBase*	_lib;
			tLibData::iterator			_iter;
		};
	public:
		CUncompressedLibraryBase(tReadableFile *pLibraryFile, vfs::Path const& sMountPoint, bool bOwnFile = false) 
			: vfs::ILibrary(pLibraryFile,sMountPoint,bOwnFile), m_uiNumberOfOpenedFiles(0)
		{};
		virtual ~CUncompressedLibraryBase();

		/**
		 *  IVFSLocation interface
		 */
		virtual bool			FileExists(vfs::Path const& sFileName);
		virtual vfs::IBaseFile*	GetFile(vfs::Path const& sFileName);
		virtual tFileType*		GetFileTyped(vfs::Path const& sFileName);
		virtual void			GetSubDirList(std::list<vfs::Path>& rlSubDirs);

		/**
		 *  ILibrary interface
		 */
		virtual bool			Init() = 0;
		virtual bool			CloseLibrary();

		virtual bool			Close(tFileType *pFileHandle);
		virtual bool			OpenRead(tFileType *pFileHandle);
		virtual bool			Read(tFileType *pFileHandle, Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead);

		virtual UInt32			GetReadLocation(tFileType *pFileHandle);
		virtual bool			SetReadLocation(tFileType *pFileHandle, UInt32 uiPositionInBytes);
		virtual bool			SetReadLocation(tFileType *pFileHandle, Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool			GetFileSize(tFileType *pFileHandle, UInt32& uiFileSize);

		virtual Iterator		begin();
	protected:
		tDirCatalogue			m_catDirs;
		tLibData				m_mapLibData;
		UInt32					m_uiNumberOfOpenedFiles;
	};
} // end namespace

#endif // _VFS_UNCOMPRESSED_LIB_BASE_H_

