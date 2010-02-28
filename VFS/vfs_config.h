#ifndef _VFS_CONFIG_H_
#define _VFS_CONFIG_H_

#ifdef WIN32
#ifdef _MSC_VER
	#ifndef PRINT_DLL_INTERFACE_WARNING
		#pragma warning ( disable : 4251 )
	#endif
#endif
#endif

#ifdef WIN32
// VFS_STATIC overrides VFS_EXPORT
#ifdef VFS_STATIC
	#define VFS_API
#endif

#ifndef VFS_STATIC
#ifdef VFS_EXPORT
	#define VFS_API		__declspec(dllexport)
#else
	#define VFS_API		__declspec(dllimport)
#endif
#endif
#else
	#define VFS_API
#endif

#endif // _VFS_CONFIG_H_
