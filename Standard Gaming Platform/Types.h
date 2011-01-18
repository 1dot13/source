#ifndef __TYPES_
#define __TYPES_

#ifndef _SIRTECH_TYPES_
#define _SIRTECH_TYPES_

#ifdef	JA2
	#ifdef	RELEASE_WITH_DEBUG_INFO

		//For JA2 Release with debug info build, disable these warnigs messages
		#pragma warning( disable : 4201 4214 4057 4100 4514 4115 4711 4244 )

	#endif


#endif 


// build defines header....
#include "builddefines.h"
	



#include <wchar.h>			// for wide-character strings

// *** SIR-TECH TYPE DEFINITIONS ***

// These two types are defined by VC6 and were causing redefinition
// problems, but JA2 is compiled with VC5

// HEY WIZARDRY DUDES, JA2 ISN'T THE ONLY PROGRAM WE COMPILE! :-)

#if defined( JA2 ) || defined( UTILS )
typedef unsigned int	UINT32;
typedef signed __int64		 INT64;		// WANNE - BMP: Used for Big Maps
typedef signed int	 INT32;
typedef unsigned __int64	UINT64;
typedef unsigned long long	UINT128;
#else
typedef unsigned int	UINT32;
typedef int					INT32;
#endif

// integers
typedef unsigned char	UINT8;
typedef signed char	 INT8;
typedef unsigned short	UINT16;
typedef signed short	INT16;
// floats
typedef float			FLOAT;
typedef double			DOUBLE;
// strings
typedef char					CHAR8;
typedef wchar_t					CHAR16;
typedef CHAR8 * 				STR;
typedef CHAR8 *					STR8;
typedef CHAR16 *				STR16;
// flags (individual bits used)
typedef unsigned char		FLAGS8;
typedef unsigned short	FLAGS16;
typedef unsigned long		FLAGS32;
// other
typedef unsigned char		BOOLEAN;
typedef void *					PTR;
typedef unsigned short	HNDL;
typedef UINT8						BYTE;
typedef CHAR8						STRING512[512];
typedef UINT32					HWFILE;

#define SGPFILENAME_LEN 100
typedef CHAR8 SGPFILENAME[SGPFILENAME_LEN];	

// *** SIR-TECH TYPE DEFINITIONS ***

#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BAD_INDEX -1

#define NULL_HANDLE 65535

#define PI 3.1415926

#define ST_EPSILON 0.00001	// define a sir-tech epsilon value

#ifndef NULL
#define NULL 0
#endif

typedef struct
{ 
	INT32 iLeft;
	INT32 iTop;
	INT32 iRight;
	INT32 iBottom;

} SGPRect;

typedef struct
{
	INT32 	iX;
	INT32	iY;

} SGPPoint;

typedef struct
{
	INT32 Min;
	INT32 Max;

} SGPRange;


typedef FLOAT	 VECTOR2[2];		// 2d vector (2x1 matrix)
typedef FLOAT	 VECTOR3[3];		// 3d vector (3x1 matrix)
typedef FLOAT	 VECTOR4[4];		// 4d vector (4x1 matrix)

typedef INT32		IVECTOR2[2];		// 2d vector (2x1 matrix)
typedef INT32		IVECTOR3[3];		// 3d vector (3x1 matrix)
typedef INT32		IVECTOR4[4];		// 4d vector (4x1 matrix)

typedef VECTOR3	MATRIX3[3];		// 3x3 matrix
typedef VECTOR4	MATRIX4[4];		// 4x4 matrix

//typedef VECTOR3	ANGLE;			// angle return array //lal removed
typedef	VECTOR4	COLOR;			// rgba color array


#include <vfs/Aspects/vfs_settings.h>
#include <vfs/Core/vfs_string.h>

inline void convert_string(std::wstring const& str_in, std::string &str_out)
{
#if !defined(USE_VFS)
	const wchar_t* src = str_in.c_str();
	size_t len = wcstombs(NULL, src, str_in.length());
	if(len > 0)
	{
		str_out.resize(len);
		wcstombs(&str_out[0], src, len);
	}
#else
	if(vfs::Settings::getUseUnicode())
	{
		str_out = vfs::String::as_utf8(str_in);
	}
	else
	{
		vfs::String::narrow(str_in, str_out);
	}
#endif
}

inline void convert_string(std::string const& str_in, std::wstring &str_out)
{
#if !defined(USE_VFS)
	const char* src = str_in.c_str();
	size_t len = mbstowcs(NULL, src, str_in.length());
	if(len > 0)
	{
		str_out.resize(len);
		mbstowcs(&str_out[0], src, len);
	}
#else
	if(vfs::Settings::getUseUnicode())
	{
		vfs::String::as_utf16(str_in, str_out);
	}
	else
	{
		vfs::String::widen(str_in, str_out);
	}
#endif
}


#endif