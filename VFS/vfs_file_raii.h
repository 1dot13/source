#ifndef _VFS_FILE_RAII_H_
#define _VFS_FILE_RAII_H_

#include "Interface/vfs_file_interface.h"
#include "vfs_vfile.h"

namespace vfs
{
	class COpenReadFile
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

	class COpenWriteFile
	{
	public:
		COpenWriteFile(	vfs::Path const& sPath,
						bool bCreate = false,
						bool bTruncate = false,
						vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_STOP_ON_WRITEABLE_PROFILE);
		COpenWriteFile(vfs::tWriteableFile *pFile);
		~COpenWriteFile();

		vfs::tWriteableFile&	file();
		void					release();
	private:
		vfs::tWriteableFile*	m_pFile;
	};

} // end namespace

#endif // _VFS_FILE_RAII_H_
