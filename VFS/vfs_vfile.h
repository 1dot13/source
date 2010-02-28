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
			SF_FIRST_WRITABLE,
			SF_STOP_ON_WRITABLE_PROFILE,
		};
	public:
		CVirtualFile();
		~CVirtualFile();
		void destroy();
		static CVirtualFile* create(vfs::Path const& sFilePath, vfs::CProfileStack& rPStack);

		vfs::Path const&	path();
		void				add(vfs::IBaseFile *pFile, utf8string sProfileName, bool bReplace = false);
		bool				remove(vfs::IBaseFile *pFile);
		//////////////////////////////////////////////////
		vfs::IBaseFile*		file(ESearchFile eSearch);
		vfs::IBaseFile*		file(utf8string const& sProfileName);
		//////////////////////////////////////////////////
	private:
		vfs::Path			_path;
		utf8string			_top_pname;
		vfs::IBaseFile*		_top_file;
		CProfileStack*		_pstack;
	private:
		vfs::UInt32			_myID;
#ifdef VFILE_BLOCK_CREATE
		static ObjBlockAllocator<vfs::CVirtualFile>* _vfile_pool;
#endif
	};
} // end namspace

#endif // _VFS_VFILE_H_
