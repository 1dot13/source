#ifndef _VFS_VLOC_H_
#define _VFS_VLOC_H_

#include "vfs_types.h"
#include "Interface/vfs_file_interface.h"

#include <map>

namespace vfs
{
	class CVirtualFile;

	class CVirtualLocation
	{
		typedef std::map<vfs::Path, CVirtualFile*, vfs::Path::Less> tVFiles;
	public:
		class Iterator
		{
			friend class CVirtualLocation;
			Iterator(CVirtualLocation* pLoc);
		public:
			Iterator();
			~Iterator();

			CVirtualFile*		value();
			void				next();
			bool				end();
		private:
			CVirtualLocation*	m_pLoc;
			tVFiles::iterator	_vfile_iter;
		};
	public:
		CVirtualLocation(vfs::Path const& sPath);
		~CVirtualLocation();

		const vfs::Path		Path;

		void				SetIsExclusive(bool bExclusive);
		bool				GetIsExclusive();

		void				AddFile(vfs::IBaseFile* pFile, utf8string const& sProfileName);
		vfs::IBaseFile*		GetFile(vfs::Path const& sFilename, utf8string const& sProfileName = "") const;
		vfs::CVirtualFile*	GetVFile(vfs::Path const& sFilename);

		bool				RemoveFile(vfs::IBaseFile* pFile);
		
		Iterator			iterate();
	private:
		bool				m_bExclusive;
		tVFiles				m_mapVFiles;
	};
} // end namespace

#endif // _VFS_VLOC_H_
