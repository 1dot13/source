#ifndef _VFS_DIRECTORY_INTERFACE_H_
#define _VFS_DIRECTORY_INTERFACE_H_

//#include "vfs_file_interface.h"
#include "vfs_location_interface.h"

namespace vfs
{
	/**
	 * NOTE: declaration of 'IDirectory' is in file 'vfs_location_interface' because
	 *       of msvc and gcc incompatibilities
	 *  
	  
	template<class WriteType>
	class IDirectory : public IVFSLocation<vfs::IReadable, WriteType>

	*
	*/
}

#endif // _VFS_DIRECTORY_INTERFACE_H_
