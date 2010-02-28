#ifndef _OS_FUNCTIONS_H_
#define _OS_FUNCTIONS_H_

#ifdef WIN32
	#include <windows.h>
#else 
	#include <sys/types.h>
	#include <sys/dir.h>
	#include <unistd.h>
	#include <stdio.h>
#endif

#include "vfs_config.h"
#include "vfs_types.h"
#include "vfs_path.h"

namespace os
{

	class CIterateDirectory
	{
	public:
		enum EFileAttribute
		{
			FA_DIRECTORY,
			FA_FILE
		};
	public:
		CIterateDirectory(vfs::Path const& sPath, utf8string const& searchPattern);
		~CIterateDirectory();

		bool nextFile(utf8string &fileName, CIterateDirectory::EFileAttribute &attrib);
	private:
	#ifdef WIN32
		HANDLE fSearchHandle;
		union
		{
			WIN32_FIND_DATAA	fFileInfoA;
			WIN32_FIND_DATAW	fFileInfoW;
		};
	#else
		struct direct **files;
		int count, current_pos;
	#endif
		bool fFirstRequest;
	};

	class FileAttributes
	{
	public:
		enum Attributes
		{
			ATTRIB_INVALID		= 0,
			ATTRIB_ARCHIVE		= 1,
			ATTRIB_DIRECTORY	= 2,
			ATTRIB_HIDDEN		= 4,
			ATTRIB_NORMAL		= 8,
			ATTRIB_READONLY		= 16,
			ATTRIB_SYSTEM		= 32,
			ATTRIB_TEMPORARY	= 64,
			ATTRIB_COMPRESSED	= 128,
			ATTRIB_OFFLINE		= 256,
		};
		bool getFileAttributes(vfs::Path const& sDir, vfs::UInt32& uiAttribs);
	};


	VFS_API bool createRealDirectory(vfs::Path const& sDir, bool bDoNotCreate=false);

	bool deleteRealFile(vfs::Path const& sDir);

	VFS_API void getExecutablePath(vfs::Path& sDir, vfs::Path& sFile);
	VFS_API void getCurrentDirectory(vfs::Path& sDir);
	VFS_API void setCurrectDirectory(vfs::Path const& sPath);

}; // end namespace

#endif // _OS_FUNCTIONS_H_

