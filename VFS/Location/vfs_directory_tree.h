#ifndef _VFS_DIRECTORY_H_
#define _VFS_DIRECTORY_H_

#include "../vfs_types.h"
//#include "Interface/vfs_location_interface.h"
#include "../Interface/vfs_file_interface.h"
#include "../Interface/vfs_directory_interface.h"

#include <map>
#include <vector>

namespace vfs
{

	/**
	 *  IDirectory<read,write>
	 */
	class CDirectoryTree : public vfs::IDirectory<vfs::IWriteable>
	{
		typedef std::map<vfs::Path, vfs::IDirectory<CDirectoryTree::tWriteType>*, vfs::Path::Less> tDirCatalogue;

		class IterImpl : public tClassType::Iterator::IImplemetation
		{
		public:
			IterImpl(CDirectoryTree& tree);
			virtual ~IterImpl();
			virtual tFileType* value();
			virtual void next();
		private:
			CDirectoryTree&			_tree;
			tDirCatalogue::iterator _subdir_iter;
			tClassType::Iterator	_file_iter;
		};
	public: 
		CDirectoryTree(vfs::Path const& sMountPoint, vfs::Path const& sRealPath)
			: vfs::IDirectory<vfs::IWriteable>(sMountPoint,sRealPath)
		{};
		virtual ~CDirectoryTree();

		bool Init();

		/** 
		 *  IDirectory interface
		 */
		virtual tFileType*		AddFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			AddFile(tFileType* pFile, bool bDeleteOldFile=false);

		virtual bool			CreateSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			DeleteDirectory(vfs::Path const& sDirPath);
		virtual bool			DeleteFileFromDirectory(vfs::Path const& sFileName);

		/** 
		 *  IVFSLocation interface
		 */
		virtual bool			FileExists(vfs::Path const& sFileName);
		virtual vfs::IBaseFile*	GetFile(vfs::Path const& sFileName);
		virtual tFileType*		GetFileTyped(vfs::Path const& sFileName);
		virtual void			GetSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tDirCatalogue m_catDirs;
	};




} // end namespace

#endif // _VFS_DIRECTORY_H_
