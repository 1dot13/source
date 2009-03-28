/// \file
/// \brief If _USE_RAK_MEMORY_OVERRIDE is defined, memory allocations go through rakMalloc, rakRealloc, and rakFree
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __RAK_MEMORY_H
#define __RAK_MEMORY_H

#include "Export.h"
#include "RakNetDefines.h"
#include <new>

#if defined(_XBOX) || defined(X360)
#elif defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
// Causes linker errors
// #include <stdlib.h>
typedef unsigned int size_t;
#elif defined ( __APPLE__ ) || defined ( __APPLE_CC__ )
#include <malloc/malloc.h>
#elif defined(_WIN32)
#include <malloc.h>
#else
#if !defined ( __FreeBSD__ )
#include <alloca.h>
#endif
#include <stdlib.h>
#endif

#if defined(_USE_RAK_MEMORY_OVERRIDE)
	#if defined(new)
		#pragma push_macro("new")
		#undef new
		#define RMO_NEW_UNDEF
	#endif
#endif

// These pointers are statically and globally defined in RakMemoryOverride.cpp
// Change them to point to your own allocators if you want.
extern "C" {
extern void* (*rakMalloc) (size_t size);
extern void* (*rakRealloc) (void *p, size_t size);
extern void (*rakFree) (void *p);
extern void (*notifyOutOfMemory) (const char *file, const long line);
}

namespace RakNet
{

	template <class Type>
	Type* OP_NEW(void)
	{
#if defined(_USE_RAK_MEMORY_OVERRIDE)
		char *buffer = (char *) rakMalloc(sizeof(Type));
		Type *t = new (buffer) Type;
		return t;
#else
		return new Type;
#endif
	}

	template <class Type>
	Type* OP_NEW_ARRAY(const int count)
	{
#if defined(_USE_RAK_MEMORY_OVERRIDE)
		Type *t;
		char *buffer = (char *) rakMalloc(sizeof(int)+sizeof(Type)*count);
		((int*)buffer)[0]=count;
		for (int i=0; i<count; i++)
		{
			t = new(buffer+sizeof(int)+i*sizeof(Type)) Type;
		}
		return (Type *) (buffer+sizeof(int));
#else
		return new Type[count];
#endif

	}

	template <class Type>
	void OP_DELETE(Type *buff)
	{
#if defined(_USE_RAK_MEMORY_OVERRIDE)
		if (buff==0) return;
		buff->~Type();
		rakFree((char*)buff);
#else
		delete buff;
#endif

	}

	template <class Type>
	void OP_DELETE_ARRAY(Type *buff)
	{
#if defined(_USE_RAK_MEMORY_OVERRIDE)
		if (buff==0) return;
		int count = ((int*)((char*)buff-sizeof(int)))[0];
		Type *t;
		for (int i=0; i<count; i++)
		{
			t = buff+i;
			t->~Type();
		}
		rakFree((char*)buff-sizeof(int));
#else
		delete [] buff;
#endif

	}

void* _RakMalloc (size_t size);
void* _RakRealloc (void *p, size_t size);
void _RakFree (void *p);

}

#if defined(_USE_RAK_MEMORY_OVERRIDE)
	#if defined(RMO_NEW_UNDEF)
	#pragma pop_macro("new")
	#undef RMO_NEW_UNDEF
	#endif
#endif

#endif
