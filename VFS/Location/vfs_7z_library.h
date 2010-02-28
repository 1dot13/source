#ifndef _VFS_7Z_LIBRARY_H_
#define _VFS_7Z_LIBRARY_H_

#include "vfs_uncompressed_lib_base.h"
#include "../File/vfs_lib_file.h"

namespace vfs
{
	class CUncompressed7zLibrary : public vfs::CUncompressedLibraryBase
	{
	public:
		CUncompressed7zLibrary(tReadableFile *libraryFile,
			vfs::Path const& mountPoint,
			bool ownFile = false,
			ObjBlockAllocator<vfs::CLibFile>* allocator=NULL);		
		virtual ~CUncompressed7zLibrary();

		virtual bool init();
	private:
		ObjBlockAllocator<vfs::CLibFile>* _allocator;
	};
} // end namespace

#endif // _VFS_7Z_LIBRARY_H_

