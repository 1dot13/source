#ifndef _VFS_UNCOMPRESSED_LIB_BASE_H_
#define _VFS_UNCOMPRESSED_LIB_BASE_H_

#include "../Interface/vfs_library_interface.h"
#include "../Interface/vfs_directory_interface.h"
#include <sstream>

namespace vfs
{

	class VFS_API CUncompressedLibraryBase : public vfs::ILibrary
	{
	protected:
		typedef std::map<vfs::Path, vfs::TDirectory<vfs::ILibrary::tWriteType>*, vfs::Path::Less> tDirCatalogue;
		struct SFileData
		{
			SFileData(vfs::size_t const& fileSize, vfs::size_t const& fileOffset)
				: _fileSize(fileSize), _fileOffset(fileOffset), _currentReadPosition(0)
			{};
			vfs::size_t _fileSize, _fileOffset, _currentReadPosition;
		};
		typedef std::map<tFileType*, SFileData> tFileData;
		
		class IterImpl;
	public:
		CUncompressedLibraryBase(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile = false);
		virtual ~CUncompressedLibraryBase();

		/**
		 *  TVFSLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& filename);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& filename);
		virtual tFileType*		getFileTyped(vfs::Path const& filename);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		/**
		 *  ILibrary interface
		 */
		virtual bool		init() = 0;
		virtual void		closeLibrary();

		virtual void		close(tFileType *fileHandle);
		virtual bool		openRead(tFileType *fileHandle);
		virtual vfs::size_t	read(tFileType *fileHandle, vfs::Byte* data, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition(tFileType *fileHandle);
		virtual void		setReadPosition(tFileType *fileHandle, vfs::size_t positionInBytes);
		virtual void		setReadPosition(tFileType *fileHandle, vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir);

		virtual vfs::size_t	getSize(tFileType *fileHandle);

		virtual Iterator	begin();
	protected:
		tDirCatalogue		m_dirs;
		tFileData			m_fileData;
		vfs::UInt32			m_numberOfOpenedFiles;
	private:
		SFileData&			_fileDataFromHandle(tFileType* handle);
	};
} // end namespace

#endif // _VFS_UNCOMPRESSED_LIB_BASE_H_

