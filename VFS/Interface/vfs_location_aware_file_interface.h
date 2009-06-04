#ifndef _VFS_LOCATION_AWARE_FILE_INTERFACE_H_
#define _VFS_LOCATION_AWARE_FILE_INTERFACE_H_

#include "vfs_file_interface.h"

namespace vfs
{

	template<typename ReadType, typename WriteType>
	class IVFSLocation;

	template<class WriteType> class IDirectory;

	class ILibrary;


	/**
	 *  ILocationAware
	 */
	template<typename ReadType, typename WriteType>
	class ILocationAware
	{
	public:
		typedef vfs::IVFSLocation<ReadType,WriteType> tLocationType;
	public:
		ILocationAware(vfs::IVFSLocation<ReadType,WriteType> *pLocation)
			: _pLoc_(pLocation)
		{};
		virtual ~ILocationAware()
		{};
	protected:
		tLocationType* _pLoc_;
	};

	/******************************************************************/
	/******************************************************************/



}

#endif // _VFS_LOCATION_AWARE_FILE_INTERFACE_H_

