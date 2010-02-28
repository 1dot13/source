#ifndef _VFS_FILE_INTERFACE_H_
#define _VFS_FILE_INTERFACE_H_

#include "../vfs_types.h"
#include "../vfs_path.h"

#include <typeinfo>

namespace vfs
{
	/**
	 *  FileAttributes
	 */
	class FileAttributes
	{
	public:
		enum Attributes {
			ATTRIB_INVALID			= 0,
			ATTRIB_ARCHIVE			= 1,
			ATTRIB_DIRECTORY		= 2,
			ATTRIB_HIDDEN			= 4,
			ATTRIB_NORMAL			= 8,
			ATTRIB_READONLY			= 16,
			ATTRIB_SYSTEM			= 32,
			ATTRIB_TEMPORARY		= 64,
			ATTRIB_COMPRESSED		= 128,
			ATTRIB_OFFLINE			= 256,
		};
		enum LocationType {
			LT_NONE					= 0,
			LT_LIBRARY				= 1,
			LT_DIRECTORY			= 2,
			LT_READONLY_DIRECTORY	= 4,
		};
	public:
		FileAttributes();
		FileAttributes(vfs::UInt32 attribs, LocationType location);

		vfs::UInt32	getAttrib() const;
		vfs::UInt32	getLocation() const;

		bool		isAttribSet(vfs::UInt32 attribs) const;
		bool		isAttribNotSet(vfs::UInt32 attribs) const;

		bool		isLocation(vfs::UInt32 location) const;
	private:
		void		operator=(vfs::FileAttributes const& attr);

		const vfs::UInt32	_attribs;
		const vfs::UInt32	_location;
	};

	/**
	 *  IBaseFile
	 */
	class VFS_API IBaseFile
	{
	public:
		enum ESeekDir
		{
			SD_BEGIN,
			SD_CURRENT,
			SD_END,
		};
	public:
		IBaseFile(vfs::Path const& filename);
		virtual ~IBaseFile();

		virtual vfs::FileAttributes	getAttributes() = 0;

		vfs::Path const&	getName();
		virtual vfs::Path	getPath();

		virtual bool		implementsWritable() = 0;
		virtual bool		implementsReadable() = 0;

		virtual void		close() = 0;
		virtual vfs::size_t	getSize() = 0;

		virtual bool		_getRealPath(vfs::Path& path);
	protected:
		vfs::Path			m_filename;
	};

	/**
	 *  IReadType , IReadable
	 */
	class IReadType{};
	class IReadable : public vfs::IReadType
	{
	public:
		virtual bool		isOpenRead() = 0;
		virtual bool		openRead() = 0;
		virtual vfs::size_t	read(vfs::Byte* data, vfs::size_t bytesToRead) = 0;	

		virtual vfs::size_t	getReadPosition() = 0;
		virtual void		setReadPosition(vfs::size_t positionInBytes) = 0;
		virtual void		setReadPosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir) = 0;
	};
	//class NonReadable : public IReadType{};

	/**
	 *  IWriteType , IWritable
	 */
	class IWriteType{};
	class IWritable : public vfs::IWriteType
	{
	public:
		virtual bool		isOpenWrite() = 0;
		virtual bool		openWrite(bool createWhenNotExist = false, bool truncate = false) = 0;
		virtual vfs::size_t	write(const vfs::Byte* data, vfs::size_t bytesToWrite) = 0;	

		virtual vfs::size_t	getWritePosition() = 0;
		virtual void		setWritePosition(vfs::size_t positionInBytes) = 0;
		virtual void		setWritePosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir) = 0;

		virtual bool		deleteFile() = 0;
	};
	//class NonWritable: public IWriteType{};

	/******************************************************************/
	/******************************************************************/

	/**
	 *  IFileTemplate 
	 */
	template<typename ReadType=vfs::IReadType, typename WriteType=vfs::IWriteType>
	class VFS_API TFileTemplate : public vfs::IBaseFile, public ReadType, public WriteType
	{
	public:
		typedef ReadType read_type;
		typedef WriteType write_type;

		typedef vfs::TFileTemplate<read_type,vfs::IWritable> write_file_type;
		typedef vfs::TFileTemplate<vfs::IReadable,write_type> read_file_type;
	public:
		TFileTemplate(vfs::Path const& fileName) 
			: vfs::IBaseFile(fileName), ReadType(), WriteType()
		{};
		virtual ~TFileTemplate()
		{};
		virtual bool	implementsWritable()
		{
			return typeid(write_type) == typeid(vfs::IWritable);
		}
		virtual bool	implementsReadable()
		{
			return typeid(read_type) == typeid(vfs::IReadable);
		}
	};

	/**
	 *  TReadableFile 
	 */
	template<class WriteType=vfs::IWriteType>
	class TReadableFile : public vfs::TFileTemplate<vfs::IReadable,WriteType>
	{
		typedef vfs::TFileTemplate<vfs::IReadable,WriteType> tBaseClass;
	public:
		typedef vfs::TReadableFile<WriteType> read_file_type;

		/////////////////////////////////////////
		TReadableFile(vfs::Path const& sFilename)
			: tBaseClass(sFilename)
		{};
		virtual ~TReadableFile(){};

		/////////////////////////////////////////
		read_file_type* operator=(vfs::IBaseFile const& t)
		{
			return read_file_type::cast(t);
		}

		/////////////////////////////////////////
		static read_file_type* cast(vfs::IBaseFile* bf)
		{
			if(bf && bf->implementsReadable())
			{
				return static_cast<read_file_type*>(bf);
			}
			return NULL;
		}
	protected:
		TReadableFile();
	};

	/**
	 *  TWritableFile 
	 */
	template<class ReadType=vfs::IReadType>
	class TWritableFile : public vfs::TFileTemplate<ReadType,vfs::IWritable>
	{
		typedef vfs::TFileTemplate<ReadType,vfs::IWritable> tBaseClass;
	public:
		typedef vfs::TWritableFile<ReadType> write_file_type;

		/////////////////////////////////////////
		TWritableFile(vfs::Path const& sFilename)
			: tBaseClass(sFilename)
		{};
		virtual ~TWritableFile(){};

		/////////////////////////////////////////
		write_file_type& operator=(vfs::IBaseFile const& t)
		{
			return *write_file_type::cast(t);
		}

		/////////////////////////////////////////
		static write_file_type* cast(IBaseFile* bf)
		{
			if(bf && bf->implementsWritable())
			{
				return static_cast<write_file_type*>(bf);
			}
			return NULL;
		}
	protected:
		TWritableFile();
	};


	/******************************************************************/
	/******************************************************************/
	
	/**
	 *  typedef's
	 */
	typedef vfs::TReadableFile<vfs::IWriteType> tReadableFile;
	typedef vfs::TWritableFile<vfs::IReadable>	tWritableFile;

} // end namespace

#endif // _VFS_FILE_INTERFACE_H_
