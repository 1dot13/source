#ifndef _VFS_PROFILE_H_
#define _VFS_PROFILE_H_

#include "vfs_types.h"
#include "Interface/vfs_location_interface.h"
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

		CVirtualProfile(utf8string const& sProfileName, bool bWritable = false);
		~CVirtualProfile();

		const utf8string		cName;
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
		CVirtualProfile*	getProfile(utf8string const& sName) const;

		CVirtualProfile*	topProfile() const;
		/** 
		 *  All files from the top profile will be removed from the VFS and the profile object will be deleted.
		 */
		bool				popProfile();
		void				pushProfile(CVirtualProfile* pProfile);

		Iterator			begin();
	private:
		std::list<vfs::CVirtualProfile*> m_profiles;
	};

} // end namespace

#endif
