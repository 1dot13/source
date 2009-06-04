#ifndef _VFS_PROFILE_H_
#define _VFS_PROFILE_H_

#include "vfs_types.h"
#include "Interface/vfs_location_interface.h"
#include <map>
#include <set>

namespace vfs
{
	class CVirtualProfile
	{
		typedef std::map<vfs::Path,vfs::IBaseLocation*, vfs::Path::Less> tLocations;
		typedef std::set<vfs::IBaseLocation*> tUniqueLoc;
	public:
		//////////////////////////////////////////////
		class Iterator
		{
			friend class CVirtualProfile;
		private:
			Iterator(CVirtualProfile& rProf);
		public:
			Iterator();
			~Iterator();
			//////
			vfs::IBaseLocation*		value() const;
			void					next();
			bool					end() const;
		private:
			CVirtualProfile*		m_pProf;
			tUniqueLoc::iterator	_loc_iter;
		};
		//////////////////////////////////////////////
		friend class Iterator;
	public:
		CVirtualProfile(utf8string const& sProfileName, bool bWriteable = false);
		~CVirtualProfile();

		const utf8string		Name;
		const bool				Writeable;

		Iterator				begin();
		
		void					AddLocation(vfs::IBaseLocation* pLoc);
		vfs::IBaseLocation*		GetLocation(vfs::Path const& sPath) const;
		vfs::IBaseFile*			GetFile(vfs::Path const& sPath) const;
	private:
		tLocations				m_mapLocations;
		tUniqueLoc				m_setLocations;
	};

	class CProfileStack
	{
	public:
		//////////////////////////////////////////////
		class Iterator
		{
			friend class CProfileStack;
		private:
			Iterator(CProfileStack& rPStack);
		public:
			Iterator();
			~Iterator();
			//////
			CVirtualProfile*	value() const;
			void				next();
			bool				end() const;
		private:
			CProfileStack* m_pPStack;
			std::list<CVirtualProfile*>::iterator _prof_iter;
		};
		//////////////////////////////////////////////
		friend class Iterator;
	public:
		CProfileStack();
		~CProfileStack();

		CVirtualProfile*	GetWriteProfile();
		CVirtualProfile*	GetProfile(utf8string const& sName) const;

		CVirtualProfile*	TopProfile() const;
		/** 
		 *  All files from the top profile will be removed from the VFS and the profile object will be deleted.
		 */
		bool				PopProfile();
		void				PushProfile(CVirtualProfile* pProfile);

		Iterator			begin();
	private:
		std::list<CVirtualProfile*> m_lProfiles;
	};

} // end namespace

#endif
