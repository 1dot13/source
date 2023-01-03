#include "SurfaceCache.h"

namespace LogicalBodyTypes {

SurfaceCache::SurfaceCache(UINT32 size) {
	loadedElementsMax = size;
	loadedElements = 0;
	oldest = newest = NULL;
};

SurfaceCache::~SurfaceCache() {
	for (UINT32 i = 0; i < surfaceVector.size(); i++) {
		delete surfaceVector[i];
	}
}

AnimationSurfaceType* SurfaceCache::GetLoadedSurface(UINT32 idx) {
	if (idx >= surfaceVector.size()) {
		// TODO more sensible handling of errors
		SET_ERROR("Error: surface index out of range!");
		return NULL;
	}
	CacheEntry* node = surfaceVector[idx];
	if (node == newest) {
		return node->surface; // nothing to be done here...
	}
	if (loadedElements < loadedElementsMax) {
		if (!oldest) oldest = node;
		if (!node->loaded) {
			node->loaded = LoadSurface(node->surface, node->structFile);
			// TODO more sensible handling of errors
			if (!node->loaded) {
				SET_ERROR("Error: a surface could not be loaded! Surface file: %s", node->surface->Filename);
				return NULL;
			}
			loadedElements++;
		} else {
			if (node->next) node->next->prev = node->prev;
			if (node->prev) node->prev->next = node->next;
		}
		if (newest) {
			newest->next = node;
			node->prev = newest;
		}
		newest = node;
	} else {
		if (node->loaded) {
			if (node == oldest) {
				oldest = oldest->next;
			}
			if (node->next) node->next->prev = node->prev;
			if (node->prev) node->prev->next = node->next;
		} else {
			CacheEntry* oldNext = oldest->next;
			while (node == oldNext) {
				oldNext = oldNext->next;
			}
			oldest->prev = NULL;
			oldest->next = NULL;
			oldest->loaded = !UnloadSurface(oldest->surface);
			oldest = oldNext;
			node->loaded = LoadSurface(node->surface, node->structFile);
			// TODO more sensible handling of errors
			if (!node->loaded) {
				SET_ERROR("Error: a surface could not be loaded!");
				return NULL;
			}
		}
		newest->next = node;
		node->prev = newest;
		newest = node;
	}
	return node->surface;
}

UINT32 SurfaceCache::AddSurface(AnimationSurfaceType* surface, STRUCTURE_FILE_REF* pStructFile) {
	surface->hVideoObject = NULL;
	CacheEntry* node = new CacheEntry(surface, pStructFile);
	surfaceVector.push_back(node);
	return surfaceVector.size() - 1;
}

bool SurfaceCache::UnloadSurface(AnimationSurfaceType* animSurfaceType) {
	if (animSurfaceType->hVideoObject != NULL) {
		DeleteVideoObject(animSurfaceType->hVideoObject);
		animSurfaceType->hVideoObject = NULL;
	}
	return true;
}

bool SurfaceCache::LoadSurface(AnimationSurfaceType* animSurfaceType, STRUCTURE_FILE_REF* pStructureFileRef) {
	AuxObjectData *pAuxData;
	VOBJECT_DESC VObjectDesc;
	HVOBJECT hVObject;
	HIMAGE hImage;
	CHAR8 sFilename[48];
	// Create video object
	FilenameForBPP(animSurfaceType->Filename, sFilename);
	AnimDebugMsg(String("SurfaceCache::LoadSurface, %s", sFilename));
	hImage = CreateImage(sFilename, IMAGE_ALLDATA);
	if (hImage == NULL) return TRUE == SET_ERROR("Error: Could not load animation file %s", sFilename);
	
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMHIMAGE;
	VObjectDesc.hImage = hImage;
	hVObject = CreateVideoObject( &VObjectDesc );
	if ( hVObject == NULL ) {
		SET_ERROR("Error: Could not load animation file: %s", sFilename);
		// Video Object will set error conition.
		DestroyImage( hImage );
		return false;
	}
	// Get aux data
	if (hImage->uiAppDataSize == hVObject->usNumberOfObjects * sizeof(AuxObjectData)) {
		// Valid auxiliary data, so get # od frames from data
		pAuxData = (AuxObjectData*) hImage->pAppData;
		animSurfaceType->uiNumFramesPerDir = pAuxData->ubNumberOfFrames;
	}
	else {
		SET_ERROR("Invalid # of animations given");
		DestroyImage(hImage);
		return false;
	}
	if (pStructureFileRef != NULL) {
		INT16 sStartFrame = 0;
		// removed exception coding. Do we even need the next block without that? See original global function
		if (AddZStripInfoToVObject(hVObject, pStructureFileRef, TRUE, sStartFrame) == FALSE) {
			DestroyImage(hImage);
			DeleteVideoObject(hVObject);
			AnimDebugMsg(String("Animation structure ZStrip creation error: %s", sFilename));
			SET_ERROR(String("Animation structure ZStrip creation error: %s", sFilename));
			return false;
		}
	}
	// the hImage is no longer needed
	DestroyImage(hImage);
	// Set video object index
	animSurfaceType->hVideoObject = hVObject;
	// Determine if we have a problem with #frames + directions ( ie mismatch )
	if ((animSurfaceType->uiNumDirections * animSurfaceType->uiNumFramesPerDir ) != animSurfaceType->hVideoObject->usNumberOfObjects) {
		AnimDebugMsg(String("Surface Database: WARNING!!! Surface %d has #frames mismatch. Filename %s", animSurfaceType->ubName, animSurfaceType->Filename));
	}
	return true;
}

}
