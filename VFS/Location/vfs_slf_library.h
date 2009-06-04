#ifndef _VFS_SLF_LIBRARY_H_
#define _VFS_SLF_LIBRARY_H_

//#include "Interface/vfs_library_interface.h"
//#include "Interface/vfs_directory_interface.h"
#include "vfs_uncompressed_lib_base.h"

namespace vfs
{
	class CSLFLibrary : public vfs::CUncompressedLibraryBase
	{
	public:
		CSLFLibrary(tReadableFile *pLibraryFile, vfs::Path const& sMountPoint, bool bOwnFile = false) 
			: vfs::CUncompressedLibraryBase(pLibraryFile,sMountPoint,bOwnFile)
		{};
		virtual ~CSLFLibrary();

		/**
		 *  ILibrary interface
		 */
		virtual bool Init();
	};

} // end namespace

#endif // _VFS_SLF_LIBRARY_H_

