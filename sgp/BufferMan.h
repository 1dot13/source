#ifndef SAVELOAD_BUFFER_H
#define SAVELOAD_BUFFER_H

#include "types.h"
#include "DEBUG.H"

#include <memory.h>
//A macro that actually memcpy's over data and increments the pointer automatically
//based on the size.  Works like a FileRead except with a buffer instead of a file pointer.
#define  LOADDATA( dst, src, size ) { memcpy( dst, src, size ); src += size; }
#define  SKIPDATA( dst, src, size ) { dst = src + size; }//dnl ch81 051213

extern BOOLEAN gfBufferLoadFailed;
extern CHAR8* gpLoadBufferStart;
extern INT32  giLoadBufferSize;

BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, CHAR8* bufStart, INT32 bufSize, const CHAR8* label);
BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, const CHAR8* label);

#endif
