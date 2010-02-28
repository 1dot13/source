#ifndef _VFS_DIR_FILE_H_
#define _VFS_DIR_FILE_H_

#include "vfs_file.h"

#include "../Interface/vfs_file_interface.h"
#include "../Interface/vfs_location_aware_file_interface.h"
#include "../Interface/vfs_location_interface.h"
#include "../Interface/vfs_directory_interface.h"

namespace vfs
{
	class VFS_API CReadOnlyDirFile : public vfs::CReadOnlyFile
	{
	protected:
		typedef vfs::TDirectory<vfs::CReadOnlyDirFile::write_type> tLocation;
	public:
		CReadOnlyDirFile(vfs::Path const& filename, tLocation *directory);
		virtual ~CReadOnlyDirFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual vfs::Path	getPath();

		virtual bool		openRead();

		virtual bool		_getRealPath(vfs::Path& path);
	private:
		tLocation*			_location;
	};

	class VFS_API CDirFile : public vfs::CFile
	{
		typedef vfs::TDirectory<vfs::CFile::write_type> tLocation;
	public:
		CDirFile(vfs::Path const& filename, tLocation *directory);
		virtual ~CDirFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual vfs::Path	getPath();
		virtual bool		deleteFile();

		virtual bool		openRead();
		virtual bool		openWrite(bool createWhenNotExist = false, bool truncate = false);

		virtual bool		_getRealPath(vfs::Path& path);
	private:
		tLocation*			_location;
	};
} // end namespace

#endif // _VFS_DIR_FILE_H_

