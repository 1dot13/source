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

	template<typename WriteType=vfs::IWriteType>
	class VFS_API TFile : public vfs::TFileTemplate<vfs::IReadable,WriteType>
	{
		typedef vfs::TFileTemplate<vfs::IReadable,WriteType> tBaseClass;
	public :
		TFile(vfs::Path const& filename);
		virtual ~TFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual void		close();
		virtual vfs::size_t	getSize();

		virtual bool		isOpenRead();
		virtual bool		openRead();
		virtual vfs::size_t	read(vfs::Byte* data, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition();
		virtual void		setReadPosition(vfs::size_t positionInBytes);
		virtual void		setReadPosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

	protected:
		bool				_internalOpenRead(vfs::Path const& path);
	protected:
		bool				m_isOpen_read;
		std::fstream		m_file;
	};

	/******************************************************************/
	/******************************************************************/

	// implements the IWritable interface for TFile
	class VFS_API CFile : public vfs::TFile<vfs::IWritable>
	{
		typedef vfs::TFile<vfs::IWritable>	tBaseClass;
	public :
		CFile(vfs::Path const& filename);
		virtual ~CFile();

		virtual void		close();

		virtual bool		isOpenWrite();
		virtual bool		openWrite(bool createWhenNotExist = false, bool truncate = false);
		virtual vfs::size_t	write(const vfs::Byte* data, vfs::size_t bytesToWrite);

		virtual vfs::size_t	getWritePosition();
		virtual void		setWritePosition(vfs::size_t positionInBytes);
		virtual void		setWritePosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual bool		deleteFile();
	protected:
		bool				_internalOpenWrite(vfs::Path const& path, bool createWhenNotExist = false, bool truncate = false);
	protected:
		bool				m_isOpen_write;
	};

	/******************************************************************/
	/******************************************************************/

	typedef vfs::TFile<vfs::IWriteType>		CReadOnlyFile; // needs explicit template instantiation

} // end namespace

#endif // _VFS_FILE_H_

