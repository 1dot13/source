#ifndef _VFS_LIBRARY_INTERFACE_H_
#define _VFS_LIBRARY_INTERFACE_H_

#include "vfs_location_interface.h"

namespace vfs
{
	class VFS_API ILibrary : public vfs::TLocationTemplate<vfs::IReadable,vfs::IWriteType>
	{
		typedef vfs::TLocationTemplate<vfs::IReadable,vfs::IWriteType> tBaseClass;
	public:
		ILibrary(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile = false);
		virtual ~ILibrary();
	
		virtual bool		init() = 0;
		virtual void		closeLibrary() = 0;
	
		virtual void		close(tFileType *fileHandle) = 0;
		virtual bool		openRead(tFileType *fileHandle) = 0;
		virtual vfs::size_t	read(tFileType *fileHandle, vfs::Byte* data, vfs::size_t bytesToRead) = 0;
		
		virtual vfs::size_t	getReadPosition(tFileType *fileHandle) = 0;
		virtual void		setReadPosition(tFileType *fileHandle, vfs::size_t positionInBytes) = 0;
		virtual void		setReadPosition(tFileType *fileHandle, vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir) = 0;
	
		virtual vfs::size_t	getSize(tFileType *pFileHandle) = 0;

		vfs::Path const&	getName();
	protected:
		bool				m_ownLibFile;
		vfs::tReadableFile*	m_libraryFile;
	};	

}

#endif // _VFS_LIBRARY_INTERFACE_H_
