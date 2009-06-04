//
// Snap: Declaration of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#ifndef _ITERATEDIR_H_
#define _ITERATEDIR_H_

#ifdef WIN32
	#include <windows.h>
#else 
	#include <sys/types.h>
	#include <sys/dir.h>
	#include <unistd.h>
	#include <stdio.h>
#endif

#include "vfs_types.h"

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

		bool NextFile(utf8string &fileName, CIterateDirectory::EFileAttribute &attrib);
	private:
	#ifdef WIN32
		HANDLE fSearchHandle;
		WIN32_FIND_DATAW	fFileInfo;
	#else
		struct direct **files;
		int count, current_pos;
	#endif
		bool fFirstRequest;
	};

	bool CreateRealDirecory(vfs::Path &sDir, bool bDoNotCreate=false);

	bool DeleteRealFile(vfs::Path &sDir);
}; // end namespace

#endif // #ifndef _ITERATEDIR_H_

