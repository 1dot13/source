#ifndef _VFS_VFILE_H_
#define _VFS_VFILE_H_

#include "vfs_profile.h"
#include <vector>

#define VFILE_BLOCK_CREATE

namespace vfs
{
	class CVirtualFile
	{
	public:
		enum ESearchFile
		{
			SF_TOP,
			SF_FIRST_WRITEABLE,
			SF_STOP_ON_WRITEABLE_PROFILE,
		};
	public:
		~CVirtualFile();
		void Destroy();
		static CVirtualFile* Create(vfs::Path const& sFilePath, CProfileStack& rPStack);

		vfs::Path const&	Path();
		void				Add(vfs::IBaseFile *pFile, utf8string sProfileName, bool bReplace = false);
		bool				Remove(vfs::IBaseFile *pFile);
		//////////////////////////////////////////////////
		vfs::IBaseFile*		File(ESearchFile eSearch);
		vfs::IBaseFile*		File(utf8string const& sProfileName);
		//////////////////////////////////////////////////
	private:
		friend class std::vector<vfs::CVirtualFile>;
		CVirtualFile();
	private:
		vfs::Path			_path;
		utf8string			_top_pname;
		vfs::IBaseFile*		_top_file;
		CProfileStack*		_pstack;
	private:
		unsigned int		_myID;
#ifdef VFILE_BLOCK_CREATE
		static ObjBlockAllocator<CVirtualFile>* _vfile_pool;
#endif
	};
} // end namspace

#endif // _VFS_VFILE_H_
