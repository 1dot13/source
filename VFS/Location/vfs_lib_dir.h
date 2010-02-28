#ifndef _VFS_LIB_DIR_H_
#define _VFS_LIB_DIR_H_

#include "../Interface/vfs_directory_interface.h"

namespace vfs
{

	class CLibDirectory : public vfs::TDirectory<vfs::IWriteType>
	{
		typedef vfs::TDirectory<vfs::IWriteType> tBaseClass;
		typedef std::map<vfs::Path, tFileType*, vfs::Path::Less> tFileCatalogue;

		class IterImpl;
	public:
		CLibDirectory(vfs::Path const& sLocalPath, vfs::Path const& sRealPath);
		virtual ~CLibDirectory();

		/** 
		 *  TDirectory interface
		 */
		virtual tFileType*		addFile(vfs::Path const& filename, bool deleteOldFile=false);
		virtual bool			addFile(tFileType* file, bool deleteOldFile=false);
		virtual bool			deleteFileFromDirectory(vfs::Path const& filename);
		virtual bool			createSubDirectory(vfs::Path const& subDirPath);
		virtual bool			deleteDirectory(vfs::Path const& dirPath);

		/** 
		 *  TVFSLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& filename);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& filename);
		virtual tFileType*		getFileTyped(vfs::Path const& filename);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tFileCatalogue m_files;
	};

} // -end- namespace

#endif // _VFS_LIB_DIR_H_
