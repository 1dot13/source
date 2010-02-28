#ifndef _VFS_MEMORY_FILE_H_
#define _VFS_MEMORY_FILE_H_

#include "../Interface/vfs_file_interface.h"
#include <sstream>

namespace vfs
{
	class VFS_API CMemoryFile : public vfs::TFileTemplate<vfs::IReadable,vfs::IWritable>
	{
		typedef vfs::TFileTemplate<vfs::IReadable,vfs::IWritable> tBaseClass;
	public :
		CMemoryFile();
		CMemoryFile(vfs::Path const& filename);
		virtual ~CMemoryFile();

		virtual vfs::FileAttributes	getAttributes();

		virtual void		close();
		virtual vfs::size_t	getSize();

		virtual bool		isOpenRead();
		virtual bool		openRead();
		virtual vfs::size_t	read(vfs::Byte* data, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition();
		virtual void		setReadPosition(vfs::size_t positionInBytes);
		virtual void		setReadPosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual bool		isOpenWrite();
		virtual bool		openWrite(bool bCreateWhenNotExist = false, bool bTruncate = false);
		virtual vfs::size_t	write(const vfs::Byte* data, vfs::size_t bytesToWrite);

		virtual vfs::size_t	getWritePosition();
		virtual void		setWritePosition(vfs::size_t positionInBytes);
		virtual void		setWritePosition(vfs::offset_t	offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual bool		deleteFile();

		// convenience method
		void				copyToBuffer(vfs::tReadableFile& rFile);
	protected:
		std::stringstream	m_buffer;
		bool				m_isOpen_read, m_isOpen_write;
	};

} // end namespace

#endif // _VFS_MEMORY_FILE_H_

