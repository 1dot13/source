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
	template<typename WriteType>
	class TDirectoryTree : public vfs::TDirectory<WriteType>
	{
		typedef std::map<vfs::Path, vfs::TDirectory<typename TDirectoryTree<WriteType>::tWriteType>*, vfs::Path::Less> tDirCatalogue;

		class IterImpl;
	public:
		typedef vfs::TDirectory<WriteType> tBaseClass;
		typedef typename tBaseClass::tWriteType tWriteType;
		typedef typename tBaseClass::tFileType tFileType;

		typedef TIterator<vfs::IBaseFile> Iterator;
		TDirectoryTree(vfs::Path const& sMountPoint, vfs::Path const& sRealPath);
		virtual ~TDirectoryTree();

		bool					init();

		/** 
		 *  TDirectory interface
		 */
		virtual tFileType*		addFile(vfs::Path const& sFilename, bool bDeleteOldFile=false);
		virtual bool			addFile(tFileType* pFile, bool bDeleteOldFile=false);

		virtual bool			createSubDirectory(vfs::Path const& sSubDirPath);
		virtual bool			deleteDirectory(vfs::Path const& sDirPath);
		virtual bool			deleteFileFromDirectory(vfs::Path const& sFileName);

		/** 
		 *  TVFSLocation interface
		 */
		virtual bool			fileExists(vfs::Path const& sFileName);
		virtual vfs::IBaseFile*	getFile(vfs::Path const& sFileName);
		virtual tFileType*		getFileTyped(vfs::Path const& sFileName);
		virtual void			getSubDirList(std::list<vfs::Path>& rlSubDirs);

		virtual Iterator begin();
	protected:
		tDirCatalogue m_catDirs;
	};

	typedef TDirectoryTree<vfs::IWritable>		CDirectoryTree;
	typedef TDirectoryTree<vfs::IWriteType>		CReadOnlyDirectoryTree;

} // end namespace

#endif // _VFS_DIRECTORY_H_
