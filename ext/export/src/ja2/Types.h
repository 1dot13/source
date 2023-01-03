#ifndef __TYPES_
#define __TYPES_

#include <wchar.h>			// for wide-character strings

// *** SIR-TECH TYPE DEFINITIONS ***

// These two types are defined by VC6 and were causing redefinition
// problems, but JA2 is compiled with VC5

// HEY WIZARDRY DUDES, JA2 ISN'T THE ONLY PROGRAM WE COMPILE! :-)

typedef unsigned int	UINT32;
typedef signed int		INT32;

// integers
typedef unsigned char	UINT8;
typedef signed char		INT8;
typedef unsigned short	UINT16;
typedef signed short	INT16;
// floats
typedef float			FLOAT;
typedef double			DOUBLE;
// strings
typedef char			CHAR8;
typedef wchar_t			CHAR16;
typedef CHAR8 * 		STR;
typedef CHAR8 *			STR8;
typedef CHAR16 *		STR16;
// flags (individual bits used)
typedef unsigned char	FLAGS8;
typedef unsigned short	FLAGS16;
typedef unsigned long	FLAGS32;
// other
typedef unsigned char	BOOLEAN;
typedef void *			PTR;
typedef unsigned short	HNDL;
typedef UINT8			BYTE;
typedef CHAR8			STRING512[512];
typedef UINT32			HWFILE;

#define SGPFILENAME_LEN 100
typedef CHAR8 SGPFILENAME[SGPFILENAME_LEN];	

#ifndef TRUE
#define	TRUE			1
#endif

#ifndef FALSE
#define	FALSE			0
#endif

#define	BAD_INDEX		-1

#define NULL_HANDLE		65535

#define					PI 3.1415926


#ifndef NULL
#define NULL			0
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


typedef FLOAT			VECTOR2[2];		// 2d vector (2x1 matrix)
typedef FLOAT			VECTOR3[3];		// 3d vector (3x1 matrix)
typedef FLOAT			VECTOR4[4];		// 4d vector (4x1 matrix)

typedef INT32			IVECTOR2[2];	// 2d vector (2x1 matrix)
typedef INT32			IVECTOR3[3];	// 3d vector (3x1 matrix)
typedef INT32			IVECTOR4[4];	// 4d vector (4x1 matrix)

typedef VECTOR3			MATRIX3[3];		// 3x3 matrix
typedef VECTOR4			MATRIX4[4];		// 4x4 matrix

//typedef VECTOR3		ANGLE;			// angle return array //lal removed
typedef	VECTOR4			COLOR;			// rgba color array


#endif