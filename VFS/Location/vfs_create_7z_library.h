#ifndef _VFS_CREATE_7Z_LIBRARY_H_
#define _VFS_CREATE_7Z_LIBRARY_H_

//#include "Interface/vfs_library_interface.h"
//#include "Interface/vfs_directory_interface.h"
#include "../Interface/vfs_file_interface.h"
#include <sstream>
#include <map>
#include <list>

namespace vfs
{
	class CCreateUncompressed7zLibrary
	{
	public:
		CCreateUncompressed7zLibrary();
		virtual ~CCreateUncompressed7zLibrary();

		bool AddFile(vfs::tReadableFile* pFile);

		bool WriteLibrary(vfs::Path const& sLibName);
		bool WriteLibrary(vfs::tWriteableFile* pFile);
	protected:
		bool WriteSignatureHeader(std::ostream& out);
		bool WriteNextHeader(std::ostream& out);
		bool WriteMainStreamsInfo(std::ostream &out);
		bool WritePackInfo(std::ostream& out);
		bool WriteUnPackInfo(std::ostream& out);
		bool WriteSubStreamsInfo(std::ostream& out);
		bool WriteFolder(std::ostream& out);
		bool WriteFilesInfo(std::ostream& out);
	private:
		unsigned int WriteFileName(std::ostream& out, utf8string const& filename);

	protected:
		vfs::tWriteableFile*	m_pLibFile;
		
		struct SFileInfo
		{
			SFileInfo() 
				: name(L""), CRC(0), offset(0), size(0), time_creation(0), time_last_access(0), time_write(0)
			{};
			//////
			utf8string::str_t name;
			UInt32		CRC;
			UInt64		offset;
			UInt64		size;
			UInt64		time_creation,time_last_access,time_write;
		};
		typedef std::map<utf8string::str_t,SFileInfo> tDirInfo;

		std::list<SFileInfo>	m_lFileInfo;
		tDirInfo				m_mapDirInfo;

		// keeping pointers to files to write their contents lates is not enough,
		// as the file may only exist for a short time
		std::stringstream		m_ssFileStream;

		std::stringstream		m_ssInfoStream;
	};
} // end namespace

#endif // _VFS_CREATE_7Z_LIBRARY_H_

