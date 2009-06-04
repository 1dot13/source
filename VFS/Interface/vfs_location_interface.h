#ifndef _VFS_LOCATION_INTERFACE_H_
#define _VFS_LOCATION_INTERFACE_H_

#include "../vfs_types.h"
#include "../vfs_debug.h"
#include "vfs_file_interface.h"

#include <map>
#include <list>

namespace vfs
{

	class IBaseLocation
	{
	public:
		class Iterator
		{
		public:
			class IImplemetation
			{
			public:
				virtual ~IImplemetation() {};
				virtual vfs::IBaseFile* value() = 0;
				virtual void next() = 0;
			};
		public:
			Iterator() : _iter_impl(NULL), _file(NULL) {};
			Iterator(IImplemetation* impl) : _iter_impl(impl), _file(NULL)
			{
				THROWIFFALSE(_iter_impl, L"EXCEPTION");
				_file = _iter_impl->value();
			}
			~Iterator()
			{
			}
			//////////////////////////////
			vfs::IBaseFile* value()
			{
				return _file;
			};
			void next()
			{
				if(_iter_impl)
				{
					_iter_impl->next();
					_file = _iter_impl->value();
					if(!_file)
					{
						delete _iter_impl;
						_iter_impl = NULL;
					}
				}
			}
			bool end()
			{
				return _file == NULL;
			}
			//////////////////////////////
		private:
			IImplemetation*	_iter_impl;
			vfs::IBaseFile*	_file;
		};
	public:
		virtual ~IBaseLocation()
		{};

		template<typename T>
		T* Cast()
		{
			return dynamic_cast<T*>(this);
		}
		virtual bool			IsWriteable() = 0;
		virtual	bool			IsReadable() = 0;

		virtual vfs::Path const&	GetFullPath() = 0;
		virtual bool			FileExists(vfs::Path const& sFileName) = 0;
		virtual vfs::IBaseFile*	GetFile(vfs::Path const& sFileName) = 0;

		virtual Iterator		begin() = 0;
		virtual void			GetSubDirList(std::list<vfs::Path>& rlSubDirs) = 0;
	};
	/**
	 *  IVFSLocation
	 */
	//template<typename ReadType=vfs::IReadType, typename WriteType=vfs::IWriteType>
	template<typename ReadType, typename WriteType>
	class IVFSLocation : public IBaseLocation
	{
	public:
		typedef vfs::IVFSLocation<ReadType,WriteType>		tClassType;
		typedef vfs::IFileTemplate<ReadType,WriteType>		tFileType;
		typedef ReadType									tReadType;
		typedef WriteType									tWriteType;
		typedef std::list<std::pair<tFileType*,vfs::Path> >	tListFilesWithPath;
	public:
		IVFSLocation(vfs::Path const& sMountPoint)
			: m_sMountPoint(sMountPoint)
		{};
		virtual ~IVFSLocation()
		{};

		// has to be virtual , or the types of the caller (not the real object) will be tested
		virtual bool IsWriteable()
		{
			return typeid(tWriteType) == typeid(vfs::IWriteable);
		}
		virtual bool IsReadable()
		{
			return typeid(tReadType) == typeid(vfs::IReadable);
		}
		virtual IVFSLocation<IReadType,IWriteable>* GetWriteable()
		{
			//if(IsWriteable()) TODO: test how this affects compatibility
			{
				return reinterpret_cast<IVFSLocation<vfs::IReadType,vfs::IWriteable>*>(this);
			}
			return NULL;
		}
		vfs::Path const& GetMountPoint()
		{
			return m_sMountPoint;
		};

		/** 
		 *  IVFSLocation interface
		 */
		virtual vfs::Path const&	GetFullPath()
		{
			return m_sMountPoint;
		}
		virtual bool			FileExists(vfs::Path const& sFileName) = 0;
		virtual vfs::IBaseFile*	GetFile(vfs::Path const& sFileName) = 0;
		virtual tFileType*		GetFileTyped(vfs::Path const& rFileName) = 0;
	protected:
		vfs::Path m_sMountPoint;
	};

/**************************************************************************************/
/**************************************************************************************/

	template<typename WriteType=vfs::IWriteType>
	class IReadLocation : public IVFSLocation<IReadable,WriteType>
	{
	public:
		template<typename T>
		static IReadLocation* Cast(T* t)
		{
			// which file type does the to tested location contain
			typename T::tFileType* _F=NULL;
			// is it readable?
			vfs::IReadable *rf = _F;
			// is the write type compatible (probably redundant)
			WriteType *wf = _F;
			// it is readable and the write type fits
			return reinterpret_cast<IReadLocation*>(t);
		}
		template<>
		static IReadLocation* Cast<IBaseLocation>(IBaseLocation* bl)
		{
			return dynamic_cast<IReadLocation*>(bl);
		}
	public:
		IReadLocation(vfs::Path const& sLocalPath) 
			: vfs::IVFSLocation<IReadable,WriteType>(sLocalPath)
		{};
		virtual ~IReadLocation(){};
	};
	typedef IReadLocation<vfs::IWriteType> tReadLocation;
	
	template<typename ReadType=vfs::IReadType>
	class IWriteLocation : public vfs::IVFSLocation<ReadType,vfs::IWriteable>
	{
	public:
		template<class T>
		static IWriteLocation* Cast(T* t)
		{
			// which file type does the to tested location contain
			typename T::tFileType* _F=NULL;
			// is it writeable?
			vfs::IWriteable *wf = _F;
			// is the read type compatible (probably redundant)
			ReadType *rf = _F;
			// is writeable and the read type fits
			return reinterpret_cast<IWriteLocation*>(t);
		}
		template<>
		static IWriteLocation* Cast<IBaseLocation>(IBaseLocation* bl)
		{
			return dynamic_cast<IWriteLocation*>(bl);
		}
	public:
		IWriteLocation(vfs::Path const& sLocalPath) 
			: vfs::IVFSLocation<ReadType,vfs::IWriteable>(sLocalPath)
		{};
		virtual ~IWriteLocation(){};
	};
	typedef IWriteLocation<vfs::IReadType> tWriteLocation;
	
	
	/**************************************************************************************/
	/**************************************************************************************/

	template<class WriteType>
	class IDirectory : public vfs::IVFSLocation<vfs::IReadable, WriteType>
	{
		typedef typename vfs::IVFSLocation<vfs::IReadable, WriteType> tBaseType;
	public:
		IDirectory(vfs::Path const& sMountPoint, vfs::Path const& sRealPath)
			: vfs::IVFSLocation<vfs::IReadable, WriteType>(sMountPoint), m_sRealPath(sRealPath)
		{};
		virtual ~IDirectory()
		{};
	
		vfs::Path const&					GetRealPath()
		{
			return m_sRealPath;
		}
	
		virtual tBaseType::tFileType*	AddFile(vfs::Path const& sFilename, bool bDeleteOldFile=false) = 0;
		virtual bool					AddFile( typename vfs::IVFSLocation<vfs::IReadable, WriteType>::tFileType* pFile, bool bDeleteOldFile=false) = 0;
	
		virtual bool					CreateSubDirectory(vfs::Path const& sSubDirPath) = 0;
		virtual bool					DeleteDirectory(vfs::Path const& sDirPath) = 0;
		virtual bool					DeleteFileFromDirectory(vfs::Path const& sFileName) = 0;
	protected:
		const vfs::Path m_sRealPath;
	};
	
	/**************************************************************************************/
	/**************************************************************************************/
	
} // end namespace

#endif // _VFS_LOCATION_INTERFACE_H_

