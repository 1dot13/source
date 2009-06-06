#ifndef _VFS_DIR_FILE_H_
#define _VFS_DIR_FILE_H_

#include "vfs_file.h"

#include "../Interface/vfs_file_interface.h"
#include "../Interface/vfs_location_aware_file_interface.h"
#include "../Interface/vfs_location_interface.h"

namespace vfs
{

	class CVFSFile : public vfs::CFile, public vfs::ILocationAware<vfs::IReadable, vfs::IWriteable>
	{
	public:
		CVFSFile(vfs::Path const& sFileName, vfs::IDirectory<vfs::CFile::write_type> *pDirectory)
			: vfs::CFile(sFileName), vfs::ILocationAware<vfs::IReadable, vfs::IWriteable>(pDirectory)
		{};
		virtual ~CVFSFile()
		{};

		virtual vfs::Path	GetFullPath();
		virtual bool		Delete();

		virtual bool		OpenRead();
		virtual bool		OpenWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);

		virtual bool		_getRealPath(vfs::Path& rPath);
	};

	class CVFSTextFile : public vfs::CTextFile, public vfs::ILocationAware<vfs::CFile::read_type, vfs::CFile::write_type>
	{
	public:
		CVFSTextFile(vfs::Path const& sFileName, vfs::IDirectory<vfs::CFile::write_type> *pDirectory)
			: vfs::CTextFile(sFileName), vfs::ILocationAware<vfs::CFile::read_type, vfs::CFile::write_type>(pDirectory)
		{};
		virtual ~CVFSTextFile()
		{};

		virtual vfs::Path	GetFullPath();
		virtual bool		OpenRead();
	};

} // end namespace

#endif // _VFS_DIR_FILE_H_

