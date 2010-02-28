#ifndef _VFS_LOCATION_INTERFACE_H_
#define _VFS_LOCATION_INTERFACE_H_

#include "../vfs_types.h"
#include "../vfs_debug.h"
#include "vfs_file_interface.h"
#include "vfs_iterator_interface.h"

#include <map>
#include <list>
#include <typeinfo>

namespace vfs
{
	class VFS_API IBaseLocation
	{
	public:
		typedef TIterator<vfs::IBaseFile> Iterator;

		virtual ~IBaseLocation()
		{};

		virtual bool				implementsWritable() = 0;
		virtual	bool				implementsReadable() = 0;

		virtual vfs::Path const&	getPath() = 0;
		virtual bool				fileExists(vfs::Path const& sFileName) = 0;
		virtual vfs::IBaseFile*		getFile(vfs::Path const& sFileName) = 0;

		virtual Iterator			begin() = 0;
		virtual void				getSubDirList(std::list<vfs::Path>& rlSubDirs) = 0;
	};

	/**
	 *  TVFSLocation
	 */
	template<typename ReadType, typename WriteType>
	class VFS_API TLocationTemplate : public IBaseLocation
	{
	public:
		typedef vfs::TLocationTemplate<ReadType,WriteType>	tLocationType;
		typedef vfs::TFileTemplate<ReadType,WriteType>		tFileType;
		typedef ReadType									tReadType;
		typedef WriteType									tWriteType;
		typedef std::list<std::pair<tFileType*,vfs::Path> >	tListFilesWithPath;
	public:
		TLocationTemplate(vfs::Path const& mountPoint)
			: m_mountPoint(mountPoint)
		{};
		virtual ~TLocationTemplate()
		{};

		// has to be virtual , or the types of the caller (not the real object) will be tested
		virtual bool implementsWritable()
		{
			return typeid(tWriteType) == typeid(vfs::IWritable);
		}
		virtual bool implementsReadable()
		{
			return typeid(tReadType) == typeid(vfs::IReadable);
		}

		vfs::Path const&			getMountPoint()
		{
			return m_mountPoint;
		}

		/** 
		 *  TLocationTemplate interface
		 */
		virtual vfs::Path const&	getPath()
		{
			return m_mountPoint;
		}

		virtual bool				fileExists(vfs::Path const& sFileName) = 0;
		virtual vfs::IBaseFile*		getFile(vfs::Path const& sFileName) = 0;
		virtual tFileType*			getFileTyped(vfs::Path const& rFileName) = 0;
	protected:
		vfs::Path m_mountPoint;
	};

/**************************************************************************************/
/**************************************************************************************/

	template<typename WriteType=vfs::IWriteType>
	class TReadLocation : public TLocationTemplate<IReadable,WriteType>
	{
	public:
		typedef TReadLocation<WriteType> tLocationType;

		static tLocationType* cast(vfs::IBaseLocation* bl)
		{
			if(bl && bl->implementsReadable())
			{
				return static_cast<tLocationType*>(bl);
			}
			return NULL;
		}
	public:
		TReadLocation(vfs::Path const& sLocalPath) 
			: vfs::TLocationTemplate<IReadable,WriteType>(sLocalPath)
		{};
		virtual ~TReadLocation(){};
	};

	template<typename ReadType=vfs::IReadType>
	class TWriteLocation : public vfs::TLocationTemplate<ReadType,vfs::IWritable>
	{
	public:
		typedef TWriteLocation<ReadType> tLocationType;

		static tLocationType* cast(vfs::IBaseLocation* bl)
		{
			if(bl && bl->implementsWritable())
			{
				return static_cast<tLocationType*>(bl);
			}
			return NULL;
		}
	public:
		TWriteLocation(vfs::Path const& sLocalPath) 
			: vfs::TLocationTemplate<ReadType,vfs::IWritable>(sLocalPath)
		{};
		virtual ~TWriteLocation(){};
	};

	/**************************************************************************************/
	/**************************************************************************************/

	typedef TReadLocation<vfs::IWriteType> tReadLocation;
	typedef TWriteLocation<vfs::IReadType> tWriteLocation;
	
} // end namespace

#endif // _VFS_LOCATION_INTERFACE_H_

