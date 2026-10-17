#ifndef SAVELOAD_BUFFER_H
#define SAVELOAD_BUFFER_H

#include "types.h"
#include "DEBUG.H"

extern BOOLEAN gfBufferLoadFailed;
extern CHAR8* gpLoadBufferStart;
extern INT32  giLoadBufferSize;

BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, CHAR8* bufStart, INT32 bufSize, const CHAR8* label);
BOOLEAN LoadDataFromBuffer(PTR dst, CHAR8*& src, INT32 size, const CHAR8* label);

#endif
