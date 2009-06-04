#ifndef _VFS_MEMORY_FILE_H_
#define _VFS_MEMORY_FILE_H_

#include "../Interface/vfs_file_interface.h"
#include <sstream>

namespace vfs
{
	class CMemoryFile : public vfs::IFileTemplate<vfs::IReadable,vfs::IWriteable>
	{
	public :
		CMemoryFile();
		CMemoryFile(vfs::Path const& sFileName);
		virtual ~CMemoryFile();

		virtual bool	Close();
		virtual bool	GetFileSize(UInt32& uiFileSize);

		virtual bool	IsOpenRead();
		virtual bool	OpenRead();
		virtual bool	Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead);

		virtual UInt32	GetReadLocation();
		virtual bool	SetReadLocation(UInt32 uiPositionInBytes);
		virtual bool	SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool	IsOpenWrite();
		virtual bool	OpenWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);
		virtual bool	Write(const Byte* pData, UInt32 uiBytesToWrite, UInt32& uiBytesWritten);

		virtual UInt32	GetWriteLocation();
		virtual bool	SetWriteLocation(Int32 uiPositionInBytes);
		virtual bool	SetWriteLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool	Delete();

		// convenience method
		bool			CopyToBuffer(vfs::tReadableFile& rFile);
	protected:
		std::stringstream	m_ssBuffer;
		bool				m_bIsOpen_read, m_bIsOpen_write;
	};

} // end namespace

#endif // _VFS_MEMORY_FILE_H_

