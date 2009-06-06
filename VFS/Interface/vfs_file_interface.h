#ifndef _VFS_FILE_INTERFACE_H_
#define _VFS_FILE_INTERFACE_H_

#include "../vfs_types.h"

namespace vfs
{
	/**
	 *  IBaseFile
	 */
	class IBaseFile
	{
	public:
		enum ESeekDir
		{
			SD_BEGIN,
			SD_CURRENT,
			SD_END,
		};
	public:
		IBaseFile(vfs::Path const& sFileName)
			: m_sFileName(sFileName)
		{};
		virtual ~IBaseFile()
		{};

		vfs::Path const&	GetFileName()
		{
			return m_sFileName;
		};
		virtual vfs::Path	GetFullPath()
		{
			return GetFileName();
		};

		virtual bool	IsWriteable() = 0;
		virtual bool	IsReadable() = 0;

		virtual bool	Close() = 0;
		virtual bool	GetFileSize(UInt32& uiFileSize) = 0;
		UInt32			GetFileSize()
		{
			UInt32 size;
			if(GetFileSize(size))
			{
				return size;
			}
			return 0;
		};

		virtual bool _getRealPath(vfs::Path& rPath)
		{
			return false;
		}
	protected:
		vfs::Path		m_sFileName;
	};

	/**
	 *  IReadType , IReadable
	 */
	class IReadType{};
	class IReadable : public vfs::IReadType
	{
	public:
		virtual bool	IsOpenRead() = 0;
		virtual bool	OpenRead() = 0;
		virtual bool	Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead) = 0;	

		virtual UInt32	GetReadLocation() = 0;
		virtual bool	SetReadLocation(UInt32 uiPositionInBytes) = 0;
		virtual bool	SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir) = 0;
	};
	//class NonReadable : public IReadType{};

	/**
	 *  IWriteType , IWriteable
	 */
	class IWriteType{};
	class IWriteable : public vfs::IWriteType
	{
	public:
		virtual bool	IsOpenWrite() = 0;
		virtual bool	OpenWrite(bool bCreateWhenNotExist = false, bool bTruncate = false) = 0;
		virtual bool	Write(const Byte* pData, UInt32 uiBytesToWrite, UInt32& uiBytesWritten) = 0;	

		virtual UInt32	GetWriteLocation() = 0;
		virtual bool	SetWriteLocation(Int32 uiPositionInBytes) = 0;
		virtual bool	SetWriteLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir) = 0;

		virtual bool	Delete() = 0;
	};
	//class NonWriteable: public IWriteType{};

	/******************************************************************/
	/******************************************************************/

	/**
	 *  IFileTemplate 
	 */
	template<typename ReadType=vfs::IReadType, typename WriteType=vfs::IWriteType>
	class IFileTemplate : public vfs::IBaseFile, public ReadType, public WriteType
	{
	public:
		typedef ReadType read_type;
		typedef WriteType write_type;
	public:
		IFileTemplate(vfs::Path const& sFileName) 
			: vfs::IBaseFile(sFileName), ReadType(), WriteType()
		{};
		virtual ~IFileTemplate()
		{};

		virtual bool	IsWriteable()
		{
			return typeid(write_type) == typeid(vfs::IWriteable);
		}
		virtual bool	IsReadable()
		{
			return typeid(read_type) == typeid(vfs::IReadable);
		}
		virtual vfs::IFileTemplate<read_type,vfs::IWriteable>* GetWriteable()
		{
			//if(IsWriteable()) TODO: test how this affects compatibility
			{
				return reinterpret_cast<vfs::IFileTemplate<read_type,vfs::IWriteable>*>(this);
			}
			return NULL;
		}
		virtual vfs::IFileTemplate<vfs::IReadable,write_type>* GetReadable()
		{
			if(IsReadable())
			{
				return reinterpret_cast<vfs::IFileTemplate<vfs::IReadable,write_type>*>(this);
			}
			return NULL;
		}
	};

	/**
	 *  IReadableFile 
	 */
	template<class WriteType=vfs::IWriteType>
	class IReadableFile : public vfs::IFileTemplate<IReadable,WriteType>
	{
	public:
		template<typename T>
		static IReadableFile* Cast(T* t)
		{
			vfs::IBaseFile* bf = t;
			return Cast<IBaseFile>(bf);
		}
		template<>
		static IReadableFile* Cast<IBaseFile>(IBaseFile* bf)
		{
			if(bf && bf->IsReadable())
			{
				return static_cast<IReadableFile*>(bf);
			}
			return NULL;
		}

		template<class T>
		IReadableFile* operator=(T const& t)
		{
			return IReadableFile::Cast(t);
		}
	public:
		IReadableFile(vfs::Path const& sFileName)
			: vfs::IFileTemplate<vfs::IReadable,WriteType>(sFileName)
		{};
		virtual ~IReadableFile(){};
	protected:
		IReadableFile();
	};

	/**
	 *  IWriteableFile 
	 */
	template<class ReadType=IReadType>
	class IWriteableFile : public vfs::IFileTemplate<ReadType,vfs::IWriteable>
	{
	public:
		template<class T>
		static IWriteableFile* Cast(T* t)
		{
			vfs::IBaseFile* bf = t;
			return Cast<IBaseFile>(bf);
		}
		template<>
		static IWriteableFile* Cast<IBaseFile>(IBaseFile* bf)
		{
			if(bf && bf->IsWriteable())
			{
				return static_cast<IWriteableFile*>(bf);
			}
			return NULL;
		}
		template<class T>
		IWriteableFile& operator=(T const& t)
		{
			return *IWriteableFile::Cast(t);
		}
	public:
		IWriteableFile(vfs::Path const& sFileName)
			: vfs::IFileTemplate<ReadType,vfs::IWriteable>(sFileName)
		{};
		virtual ~IWriteableFile(){};
	protected:
		IWriteableFile();
	};

	/******************************************************************/
	/******************************************************************/
	
	/**
	 *  typedef's
	 */
	typedef vfs::IReadableFile<vfs::IWriteType> tReadableFile;
	typedef vfs::IWriteableFile<vfs::IReadable> tWriteableFile;
	//typedef vfs::IWriteableFile<vfs::IReadType> tWriteableFile;
	

} // end namespace

#endif // _VFS_FILE_INTERFACE_H_
