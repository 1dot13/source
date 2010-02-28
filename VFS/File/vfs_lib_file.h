#ifndef _VFS_LIB_FILE_H_
#define _VFS_LIB_FILE_H_

#include "../Interface/vfs_file_interface.h"
#include "../Interface/vfs_location_interface.h"
#include "../Interface/vfs_location_aware_file_interface.h"
#include "../Interface/vfs_library_interface.h"

namespace vfs
{

	class ILibrary;

	class CLibFile : public vfs::TFileTemplate<vfs::IReadable,vfs::IWriteType>
	{
		typedef vfs::TFileTemplate<vfs::IReadable,vfs::IWriteType> tBaseClass;
		typedef vfs::TLocationTemplate<vfs::IReadable,vfs::IWriteType> tLocation;
	public:
		CLibFile();
		static CLibFile* create(vfs::Path const& filename, 
			tLocation *location, 
			vfs::ILibrary *library,
			ObjBlockAllocator<CLibFile>* allocator = NULL);

		// don't delete objects that YOU have not created with 'new'
		// dtor has to remain public to be usable at all
		virtual ~CLibFile();

		virtual vfs::FileAttributes getAttributes();

		virtual void		close();

		virtual vfs::Path	getPath();

		virtual bool		isOpenRead();
		virtual bool		openRead();
		virtual vfs::size_t	read(vfs::Byte* pData, vfs::size_t bytesToRead);

		virtual vfs::size_t	getReadPosition();
		virtual void		setReadPosition(vfs::size_t positionInBytes);
		virtual void		setReadPosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir);

		virtual vfs::size_t	getSize();
	protected:
		bool				m_isOpen_read;
		ILibrary*			m_library;
		tLocation*			m_location;
	private:
		static ObjBlockAllocator<CLibFile>*	_lfile_pool;
	};

} // end namespace

#endif // _VFS_LIB_FILE_H_

