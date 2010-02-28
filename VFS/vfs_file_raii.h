#ifndef _VFS_FILE_RAII_H_
#define _VFS_FILE_RAII_H_

#include "Interface/vfs_file_interface.h"
#include "vfs_vfile.h"

namespace vfs
{
	class VFS_API COpenReadFile
	{
	public:
		COpenReadFile(vfs::Path const& sPath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP);
		COpenReadFile(vfs::tReadableFile *pFile);
		~COpenReadFile();

		vfs::tReadableFile&		file();
		void					release();
	private:
		vfs::tReadableFile*		m_pFile;
	};

	class VFS_API COpenWriteFile
	{
	public:
		COpenWriteFile(	vfs::Path const& sPath,
						bool bCreate = false,
						bool bTruncate = false,
						vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
		COpenWriteFile(vfs::tWritableFile *pFile);
		~COpenWriteFile();

		vfs::tWritableFile&	file();
		void					release();
	private:
		vfs::tWritableFile*	m_pFile;
	};

} // end namespace

#endif // _VFS_FILE_RAII_H_
