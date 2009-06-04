#ifndef _VFS_FILE_H_
#define _VFS_FILE_H_

#include "../vfs_types.h"
#include "../Interface/vfs_file_interface.h"

#include <fstream>

typedef std::basic_fstream<wchar_t> wfstream;

namespace vfs
{
	/******************************************************************/
	/******************************************************************/

	class CFile : public vfs::IFileTemplate<IReadable,IWriteable>
	{
	public :
		CFile(vfs::Path const& sFileName);
		virtual ~CFile();

		virtual bool		Close();
		virtual bool		GetFileSize(UInt32& uiFileSize);

		virtual bool		IsOpenRead();
		virtual bool		OpenRead();
		virtual bool		Read(vfs::Byte* pData, vfs::UInt32 uiBytesToRead, vfs::UInt32& uiBytesRead);

		virtual vfs::UInt32	GetReadLocation();
		virtual bool		SetReadLocation(UInt32 uiPositionInBytes);
		virtual bool		SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool		IsOpenWrite();
		virtual bool		OpenWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);
		virtual bool		Write(const vfs::Byte* pData, vfs::UInt32 uiBytesToWrite, vfs::UInt32& uiBytesWritten);

		virtual vfs::UInt32	GetWriteLocation();
		virtual bool		SetWriteLocation(Int32 uiPositionInBytes);
		virtual bool		SetWriteLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool		Delete();
	protected:
		std::fstream	m_oFile;
		bool			m_bIsOpen_read, m_bIsOpen_write;
	};

	/******************************************************************/
	/******************************************************************/

	class CTextFile : public CFile
	{
	public:
		CTextFile(vfs::Path const& sFileName)
			: CFile(sFileName)
		{};
		virtual ~CTextFile()
		{};

		virtual bool OpenRead();
		virtual bool OpenWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);

		virtual bool Read(Byte* pData, vfs::UInt32 uiBytesToRead, vfs::UInt32& uiBytesRead);
		virtual bool ReadLine(std::string &sLine, vfs::UInt32 uiMaxNumChars);

		virtual bool Write(const vfs::Byte* pData, vfs::UInt32 uiBytesToWrite, vfs::UInt32& uiBytesWritten);
		virtual bool WriteLine(std::string const& sLine);
	};

} // end namespace

#endif // _VFS_FILE_H_

