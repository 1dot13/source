#ifndef _VFS_SLF_LIBRARY_H_
#define _VFS_SLF_LIBRARY_H_

#include "vfs_uncompressed_lib_base.h"

namespace vfs
{
	class VFS_API CSLFLibrary : public vfs::CUncompressedLibraryBase
	{
	public:
		CSLFLibrary(tReadableFile *pLibraryFile, vfs::Path const& sMountPoint, bool bOwnFile = false);
		virtual ~CSLFLibrary();

		virtual bool init();
	};

} // end namespace

#endif // _VFS_SLF_LIBRARY_H_

