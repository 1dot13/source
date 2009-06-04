#ifndef _VFS_LIB_FILE_H_
#define _VFS_LIB_FILE_H_

#include "../Interface/vfs_file_interface.h"
#include "../Interface/vfs_location_interface.h"
#include "../Interface/vfs_location_aware_file_interface.h"
#include "../Interface/vfs_library_interface.h"

namespace vfs
{

	class ILibrary;

	class CLibFile : public vfs::IFileTemplate<vfs::IReadable,vfs::IWriteType>, public vfs::ILocationAware<vfs::IReadable,vfs::IWriteType>
	{
		typedef vfs::IFileTemplate<vfs::IReadable,vfs::IWriteType> tBaseType;
	public:
		static CLibFile* Create(vfs::Path const& sFileName, 
			vfs::IVFSLocation<vfs::IReadable,vfs::IWriteType> *pLocation, 
			ILibrary *pLibrary,
			ObjBlockAllocator<CLibFile>* allocator = NULL);
		// don't delete objects that YOU have not created with 'new'
		// dtor has to remain public to be usable at all
		virtual ~CLibFile();

		virtual bool Close();

		virtual vfs::Path	GetFullPath();

		virtual bool		IsOpenRead();
		virtual bool		OpenRead();
		virtual bool		Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead);

		virtual UInt32		GetReadLocation();
		virtual bool		SetReadLocation(UInt32 uiPositionInBytes);
		virtual bool		SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir);

		virtual bool		GetFileSize(UInt32& uiFileSize);
	protected:
		bool		m_bIsOpen_read;
		ILibrary*	m_pLibrary;
	private:
		friend class std::vector<CLibFile>;
		CLibFile();
		static ObjBlockAllocator<CLibFile>*	_lfile_pool;
	};

} // end namespace

#endif // _VFS_LIB_FILE_H_

