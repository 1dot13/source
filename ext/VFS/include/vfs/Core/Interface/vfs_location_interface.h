/* 
 * bfVFS : vfs/Core/Interface/vfs_location_interface.h
 *  - generic Location interface that allows retrieval of a file from a real location
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _VFS_LOCATION_INTERFACE_H_
#define _VFS_LOCATION_INTERFACE_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/Interface/vfs_iterator_interface.h>

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
	 *  TLocation
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

