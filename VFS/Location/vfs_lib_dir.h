#ifndef _VFS_LIB_DIR_H_
#define _VFS_LIB_DIR_H_

#include "../Interface/vfs_directory_interface.h"

namespace vfs
{

	class CLibDirectory : public vfs::IDirectory<vfs::IWriteType>
	{
		typedef std::map<vfs::Path, tFileType*, vfs::Path::Less> tFileCatalogue;

		class IterImpl : public tClassType::Iterator::IImplemetation
		{
		public:
			IterImpl(CLibDirectory& lib);
			virtual ~IterImpl();
			virtual tFileType* value();
			virtual void next();
		private:
			CLibDirectory& _lib;
			tFileCatalogue::iterator _iter;
		};
	public:
		CLibDirectory(vfs::Path const& sLocalPath, vfs::Path const& sRealPath)
			: vfs::IDirectory<vfs::IWriteType>(sLocalPath,sRealPath)
		{};
		virtual ~CLibDirectory();

		/** 
		 *  IDirectory interface
		 */
		virtual tFileType*		AddFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			AddFile(tFileType* pFile, bool bDeleteOldFile=false);
		virtual bool			DeleteFileFromDirectory(vfs::Path const& sFileName);
		virtual bool			CreateSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			DeleteDirectory(vfs::Path const& sDirPath);

		/** 
		 *  IVFSLocation interface
		 */
		virtual bool			FileExists(vfs::Path const& sFileName);
		virtual vfs::IBaseFile*	GetFile(vfs::Path const& sFileName);
		virtual tFileType*		GetFileTyped(vfs::Path const& sFileName);
		virtual void			GetSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tFileCatalogue m_mapFiles;
	};

} // -end- namespace

#endif // _VFS_LIB_DIR_H_
