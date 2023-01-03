#pragma once
#ifndef _LBT_SURFACECACHE__H_
#define _LBT_SURFACECACHE__H_

#include "Animation Data.h"
#include "Utilities.h"
#include "Sys Globals.h"
#include "Debug Control.h"

namespace LogicalBodyTypes {

class SurfaceCache {

private:
	struct CacheEntry {
		CacheEntry(AnimationSurfaceType* animSurface, STRUCTURE_FILE_REF* pStructFile) {
			surface = animSurface;
			structFile = pStructFile;
			loaded = false;
			prev = next = NULL;
		}
		bool loaded;
		AnimationSurfaceType* surface;
		STRUCTURE_FILE_REF* structFile;
		CacheEntry* prev;
		CacheEntry* next;
	} typedef CacheEntry;
	typedef std::vector<CacheEntry*> SurfaceVector;

public:
	SurfaceCache(UINT32 size);
	~SurfaceCache();
	AnimationSurfaceType* GetLoadedSurface(UINT32 idx);
	UINT32 AddSurface(AnimationSurfaceType* surface, STRUCTURE_FILE_REF* pStructFile);

private:
	UINT32 loadedElementsMax;
	UINT32 loadedElements;
	CacheEntry* newest;
	CacheEntry* oldest;
    SurfaceVector surfaceVector;
	static bool LoadSurface(AnimationSurfaceType* animSurfaceType, STRUCTURE_FILE_REF* pStructureFileRef);
	static bool SurfaceCache::UnloadSurface(AnimationSurfaceType* animSurfaceType);

};

}

#endif
