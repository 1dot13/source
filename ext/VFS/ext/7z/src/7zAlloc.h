/* 7zAlloc.h -- Allocation functions
2009-02-07 : Igor Pavlov : Public domain */

#ifndef __7Z_ALLOC_H
#define __7Z_ALLOC_H

#include <stddef.h>
#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

SZIP_API void *SzAlloc(void *p, size_t size);
SZIP_API void SzFree(void *p, void *address);

SZIP_API void *SzAllocTemp(void *p, size_t size);
SZIP_API void SzFreeTemp(void *p, void *address);

#ifdef __cplusplus
}
#endif

#endif
