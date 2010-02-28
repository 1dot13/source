#ifndef _VFS_VLOC_H_
#define _VFS_VLOC_H_

#include "vfs_types.h"
#include "Interface/vfs_file_interface.h"
#include "Interface/vfs_iterator_interface.h"

#include <map>

namespace vfs
{
	class CVirtualFile;

	class CVirtualLocation
	{
		class VFileIterator;
		typedef std::map<vfs::Path, CVirtualFile*, vfs::Path::Less> tVFiles;
	public:
		typedef vfs::TIterator<CVirtualFile> Iterator;

		CVirtualLocation(vfs::Path const& sPath);
		~CVirtualLocation();

		const vfs::Path		cPath;

		void				setIsExclusive(bool exclusive);
		bool				getIsExclusive();

		void				addFile(vfs::IBaseFile* pile, utf8string const& profileName);
		vfs::IBaseFile*		getFile(vfs::Path const& filename, utf8string const& profileName = "") const;
		vfs::CVirtualFile*	getVirtualFile(vfs::Path const& filename);

		bool				removeFile(vfs::IBaseFile* file);
		
		Iterator			iterate();
	private:
		void				operator=(vfs::CVirtualLocation const& vloc);

		bool				m_exclusive;
		tVFiles				m_VFiles;
	};
} // end namespace

#endif // _VFS_VLOC_H_
