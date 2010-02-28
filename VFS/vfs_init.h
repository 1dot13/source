#ifndef _VFS_INIT_H_
#define _VFS_INIT_H_

#include "vfs_types.h"
#include "vfs_profile.h"
#include "PropertyContainer.h"

VFS_API bool initWriteProfile(vfs::CVirtualProfile &rProf, vfs::Path const& profileRoot);
VFS_API bool initVirtualFileSystem(vfs::Path const& vfs_ini);
VFS_API bool initVirtualFileSystem(std::list<vfs::Path> const& vfs_ini_list);
VFS_API bool initVirtualFileSystem(CPropertyContainer& props);

#endif // _VFS_INIT_H_
