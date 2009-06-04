#ifndef _VFS_H_
#define _VFS_H_

#include "Interface/vfs_file_interface.h"
#include "Interface/vfs_location_interface.h"
#include "vfs_vloc.h"
#include "vfs_vfile.h"
#include "vfs_types.h"
#include <map>

#define USE_VFS


class CFileAllocator
{
public:
	static void RegisterAllocator(BasicAllocator* allocator);
	
	static void Clear();
private:
	static std::vector<BasicAllocator*> _valloc;
};

namespace vfs
{
	class CProfileStack;

	class CVirtualFileSystem
	{
		typedef std::map<vfs::Path,CVirtualLocation*,vfs::Path::Less> tVFS;
	public:
		/*****************************************************/
		class Iterator
		{
		public:
			//////////////////////////////
			class IImplemetation
			{
			public:
				virtual ~IImplemetation() {};
				virtual vfs::tReadableFile* value() = 0;
				virtual void next() = 0;
			};
			//////////////////////////////
		private:
			friend class CVirtualFileSystem;
			Iterator(IImplemetation* impl);
		public:
			Iterator();
			~Iterator();

			vfs::tReadableFile*	value();
			void				next();
			bool				end();
		private:
			IImplemetation*		_iter_impl;
			vfs::tReadableFile*	_file;
		};
		/*****************************************************/
		class CRegularIterator : public vfs::CVirtualFileSystem::Iterator::IImplemetation
		{
		public:
			CRegularIterator(vfs::CVirtualFileSystem& rVFS);
			virtual ~CRegularIterator();

			virtual vfs::tReadableFile*				value();
			virtual void							next();
		private:
			vfs::CVirtualFileSystem&				m_rVFS;
			vfs::CVirtualFileSystem::tVFS::iterator	_vloc_iter;
			CVirtualLocation::Iterator				_vfile_iter;
		};
		/*****************************************************/
		class CMatchingIterator : public vfs::CVirtualFileSystem::Iterator::IImplemetation
		{
		public:
			CMatchingIterator(vfs::Path const& sPattern, vfs::CVirtualFileSystem& rVFS);
			virtual ~CMatchingIterator();

			virtual vfs::tReadableFile*				value();
			virtual void							next();
		private:
			bool									nextLocationMatch();
			bool									nextFileMatch();
		private:
			vfs::Path								m_sLocPattern, m_sFilePattern;
			vfs::CVirtualFileSystem&				m_rVFS;
			vfs::CVirtualFileSystem::tVFS::iterator	_vloc_iter;
			CVirtualLocation::Iterator				_vfile_iter;
		};
		/*****************************************************/
	public:
		~CVirtualFileSystem();

		static CVirtualFileSystem*	GetVFS();
		static void					ShutdownVFS();

		CProfileStack*			GetProfileStack();

		vfs::CVirtualLocation*	GetVirtualLocation(vfs::Path const& sPath, bool bCreate = false);
		bool					AddLocation(vfs::IBaseLocation* pLocation, 
											utf8string const& sProfileName, 
											bool bIsWriteable = false);

		bool					FileExists(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		bool					FileExists(vfs::Path const& rLocalFilePath, std::string const& sProfileName);

		vfs::IBaseFile*			GetFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::IBaseFile*			GetFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName);

		vfs::tReadableFile*		GetRFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::tReadableFile*		GetRFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName);

		vfs::tWriteableFile*	GetWFile(vfs::Path const& rLocalFilePath, vfs::CVirtualFile::ESearchFile eSF = vfs::CVirtualFile::SF_TOP );
		vfs::tWriteableFile*	GetWFile(vfs::Path const& rLocalFilePath, utf8string const& sProfileName);

		bool					RemoveFileFromFS(vfs::Path const& sFilePath);
		bool					RemoveDirectoryFromFS(vfs::Path const& sDir);
		bool					CreateNewFile(vfs::Path const& sFileName);

		Iterator				begin();
		Iterator				begin(vfs::Path const& sPattern);
	private:
		CProfileStack			m_oProfileStack;
		tVFS					m_mapFS;
	private:
		CVirtualFileSystem();
		static CVirtualFileSystem* m_pSingleton;
	};

} // end namespace

vfs::CVirtualFileSystem* GetVFS();

#endif // _VFS_H_
