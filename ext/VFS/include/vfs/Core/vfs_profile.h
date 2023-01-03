/* 
 * bfVFS : vfs/Core/vfs_profile.h
 *  - Virtual Profile, container for real file system locations or archives
 *  - Profile Stack, orders profiles in a linear fashion (top-bottom)
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

#ifndef _VFS_PROFILE_H_
#define _VFS_PROFILE_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/Interface/vfs_location_interface.h>
#include <map>
#include <set>

namespace vfs
{
	class VFS_API CVirtualProfile
	{
		typedef std::map<vfs::Path,vfs::IBaseLocation*, vfs::Path::Less> tLocations;
		typedef std::set<vfs::IBaseLocation*> tUniqueLoc;

		class IterImpl;
		class FileIterImpl;
	public:
		typedef TIterator<vfs::IBaseLocation>	Iterator;
		typedef TIterator<vfs::IBaseFile>		FileIterator;

		CVirtualProfile(vfs::String const& profile_name, vfs::Path profile_root, bool writable = false);
		~CVirtualProfile();

		const vfs::String		cName;
		const vfs::Path			cRoot;
		const bool				cWritable;

		Iterator				begin();
		FileIterator			files(vfs::Path const& sPattern);
		
		void					addLocation(vfs::IBaseLocation* pLoc);
		vfs::IBaseLocation*		getLocation(vfs::Path const& sPath) const;
		vfs::IBaseFile*			getFile(vfs::Path const& sPath) const;
	private:
		void					operator=(vfs::CVirtualProfile const& vprof);
		tLocations				m_mapLocations;
		tUniqueLoc				m_setLocations;
	};

	class VFS_API CProfileStack
	{
		class IterImpl;
	public:
		typedef vfs::TIterator<CVirtualProfile> Iterator;

		CProfileStack();
		~CProfileStack();

		CVirtualProfile*	getWriteProfile();
		CVirtualProfile*	getProfile(vfs::String const& sName) const;

		CVirtualProfile*	topProfile() const;
		/** 
		 *  All files from the top profile will be removed from the VFS and the profile object will be deleted.
		 */
		bool				popProfile();
		void				pushProfile(CVirtualProfile* pProfile);

		Iterator			begin();
	private:
		typedef std::list<vfs::CVirtualProfile*> t_profiles;
		t_profiles			m_profiles;
	};

} // end namespace

#endif
