#ifndef _VFS_7Z_LIBRARY_H_
#define _VFS_7Z_LIBRARY_H_

#include "vfs_uncompressed_lib_base.h"
#include "../File/vfs_lib_file.h"

namespace vfs
{
	class CUncompressed7zLibrary : public vfs::CUncompressedLibraryBase
	{
	public:
		CUncompressed7zLibrary(tReadableFile *pLibraryFile,
			vfs::Path const& sMountPoint,
			bool bOwnFile = false,
			ObjBlockAllocator<vfs::CLibFile>* allocator=NULL)
			: vfs::CUncompressedLibraryBase(pLibraryFile,sMountPoint,bOwnFile), _allocator(allocator)
		{};
		virtual ~CUncompressed7zLibrary()
		{};
		/**
		 *  ILibrary interface
		 */
		virtual bool Init();
	private:
		ObjBlockAllocator<vfs::CLibFile>* _allocator;
	};
} // end namespace

#endif // _VFS_7Z_LIBRARY_H_

