#ifndef _VFS_DIRECTORY_INTERFACE_H_
#define _VFS_DIRECTORY_INTERFACE_H_

#include "vfs_location_interface.h"

namespace vfs
{
	template<class WriteType>
	class TDirectory : public vfs::TLocationTemplate<vfs::IReadable, WriteType>
	{
	public:
		typedef typename vfs::TLocationTemplate<vfs::IReadable, WriteType> tBaseClass;
		typedef typename tBaseClass::tFileType tFileType;
		typedef typename tBaseClass::tWriteType tWriteType;

		TDirectory(vfs::Path const& mountPoint, vfs::Path const& realPath)
			: tBaseClass(mountPoint), m_realPath(realPath)
		{};
		virtual ~TDirectory()
		{};
	
		vfs::Path const&	getRealPath()
		{
			return m_realPath;
		}
	
		virtual tFileType*	addFile(vfs::Path const& sFilename, bool bDeleteOldFile=false) = 0;
		virtual bool		addFile(typename tBaseClass::tFileType* pFile, bool bDeleteOldFile=false) = 0;
	
		virtual bool		createSubDirectory(vfs::Path const& sSubDirPath) = 0;
		virtual bool		deleteDirectory(vfs::Path const& sDirPath) = 0;
		virtual bool		deleteFileFromDirectory(vfs::Path const& sFileName) = 0;
	protected:
		const vfs::Path 	m_realPath;
	private:
		void				operator=(TDirectory<WriteType> const& t);
	};
}

#endif // _VFS_DIRECTORY_INTERFACE_H_
