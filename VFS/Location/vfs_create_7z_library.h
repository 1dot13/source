#ifndef _VFS_CREATE_7Z_LIBRARY_H_
#define _VFS_CREATE_7Z_LIBRARY_H_

#include "../Interface/vfs_file_interface.h"
#include <sstream>
#include <map>
#include <list>

namespace vfs
{
	class VFS_API CCreateUncompressed7zLibrary
	{
	public:
		CCreateUncompressed7zLibrary();
		virtual ~CCreateUncompressed7zLibrary();

		bool addFile(vfs::tReadableFile* pFile);

		bool writeLibrary(vfs::Path const& sLibName);
		bool writeLibrary(vfs::tWritableFile* pFile);
	protected:
		bool writeSignatureHeader(std::ostream& out);
		bool writeNextHeader(std::ostream& out);
		bool writeMainStreamsInfo(std::ostream &out);
		bool writePackInfo(std::ostream& out);
		bool writeUnPackInfo(std::ostream& out);
		bool writeSubStreamsInfo(std::ostream& out);
		bool writeFolder(std::ostream& out);
		bool writeFilesInfo(std::ostream& out);
	private:
		vfs::size_t writeFileName(std::ostream& out, utf8string const& filename);

	protected:
		vfs::tWritableFile*	m_pLibFile;
		
		struct SFileInfo
		{
			SFileInfo() 
				: name(L""), CRC(0), offset(0), size(0), time_creation(0), time_last_access(0), time_write(0)
			{};
			//////
			utf8string::str_t name;
			vfs::UInt32		CRC;
			vfs::UInt64		offset;
			vfs::UInt64		size;
			vfs::UInt64		time_creation,time_last_access,time_write;
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

