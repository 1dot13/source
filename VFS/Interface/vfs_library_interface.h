#ifndef _VFS_LIBRARY_INTERFACE_H_
#define _VFS_LIBRARY_INTERFACE_H_

#include "vfs_location_interface.h"


namespace vfs
{

	class ILibrary : public vfs::IVFSLocation<vfs::IReadable,vfs::IWriteType>
	{
	public:
		ILibrary(tReadableFile *pLibraryFile, vfs::Path const& sMountPoint, bool bOwnFile = false)
			: vfs::IVFSLocation<vfs::IReadable,vfs::IWriteType>(sMountPoint), m_pLibraryFile(pLibraryFile), m_bOwnLibFile(bOwnFile)
		{};
		virtual ~ILibrary()
		{
			if(m_pLibraryFile && m_bOwnLibFile)
			{
				m_pLibraryFile->Close();
				delete m_pLibraryFile;
				m_pLibraryFile = NULL;
			}
		};
	
		virtual bool	Init() = 0;
		virtual bool	CloseLibrary() = 0;
	
		virtual bool	Close(tFileType *pFileHandle) = 0;
		virtual bool	OpenRead(tFileType *pFileHandle) = 0;
		virtual bool	Read(tFileType *pFileHandle, Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead) = 0;
		
		virtual UInt32	GetReadLocation(tFileType *pFileHandle) = 0;
		virtual bool	SetReadLocation(tFileType *pFileHandle, UInt32 uiPositionInBytes) = 0;
		virtual bool	SetReadLocation(tFileType *pFileHandle, Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir) = 0;
	
		virtual bool	GetFileSize(tFileType *pFileHandle, UInt32& uiFileSize) = 0;

		vfs::Path const&	GetLibName()
		{
			return m_pLibraryFile->GetFileName();
		}
	protected:
		vfs::tReadableFile*	m_pLibraryFile;
		bool				m_bOwnLibFile;
	};	

}

#endif // _VFS_LIBRARY_INTERFACE_H_
