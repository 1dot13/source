#pragma once

#ifndef _LBT_BODYTYPE__H_
#define _LBT_BODYTYPE__H_

#include "LogicalBodyTypes/Layers.h"
#include "LogicalBodyTypes/Filter.h"
#include "LogicalBodyTypes/SurfaceCache.h"
#include "LogicalBodyTypes/SurfaceDB.h"
#include "LogicalBodyTypes/PaletteDB.h"
#include "LogicalBodyTypes/PaletteTable.h"
#include "Animation Data.h"
#include "Animation Control.h"

namespace LogicalBodyTypes {

class BodyType {

public:
	struct LogicalSurfaceType {
		LogicalSurfaceType(AnimationSurfaceType* physicalSurfaceType, AnimationSurfaceType* alphaSurfaceType, UINT32 alphaSurfaceIdx, PaletteTable* paletteTable)
			: paletteTable(paletteTable), physicalSurfaceType(physicalSurfaceType), alphaSurfaceType(alphaSurfaceType), alphaSurfaceIdx(alphaSurfaceIdx) {};
		const PaletteTable* paletteTable;
		const AnimationSurfaceType* physicalSurfaceType;
		const AnimationSurfaceType* alphaSurfaceType;
		UINT32 alphaSurfaceIdx;
	} typedef LogicalSurfaceType;

private:
	typedef std::pair<Filter*, std::pair<UINT32, LogicalSurfaceType> > FilterSurfaceTypePair;
	typedef std::list<FilterSurfaceTypePair> FilterSurfaceTypeList;
	typedef std::vector<FilterSurfaceTypeList> LayerVector;
	typedef std::vector<LayerVector> AnimStateVector;
	typedef std::vector<LayerVector> PhysAnimSurfaceVector;
	AnimStateVector animstates;
	PhysAnimSurfaceVector physAnimSurfaces;
	Layers::LayerPropertiesVector layerProperties;
	Filter* filter;
	bool hasSurfaces;
	UINT32 numCacheNodes;
	UINT32 maxCacheNodes;
	SurfaceCache* cache;

public:
	BodyType(UINT32 cacheSize, Filter* filterP = NULL);
	~BodyType(void);
	void AddSurfaceForAnimState(UINT16 usAnimState, Layers::LayerPropertiesVector::size_type layer, AnimationSurfaceType* animSurfaceType, AnimationSurfaceType* alphaSurfaceType = NULL, Filter* filter = NULL, PaletteTable* paletteTable = NULL);
	void AddSurfaceForPhysSurface(UINT16 usAnimSurface, Layers::LayerPropertiesVector::size_type layer, AnimationSurfaceType* animSurfaceType, AnimationSurfaceType* alphaSurfaceType = NULL, Filter* filter = NULL, PaletteTable* paletteTable = NULL);
	void SetLayerProperties(Layers::LayerPropertiesVector::size_type index, Layers::LayerProperties properties);
	const Layers::LayerProperties* GetLayerProperties(Layers::LayerPropertiesVector::size_type index);

	inline bool Match(SOLDIERTYPE* pSoldier) {
		if (filter != NULL) return filter->Match(pSoldier);
		return false; // no filter specified - matches none
	};

	inline bool HasSurfaces() {
		return hasSurfaces;
	}

	inline BodyType::LogicalSurfaceType* GetLogicalSurfaceType(Layers::LayerPropertiesVector::size_type layer, SOLDIERTYPE* pSoldier) {
		FilterSurfaceTypeList::iterator end = animstates[pSoldier->usAnimState][layer].end();
		for (FilterSurfaceTypeList::iterator i = animstates[pSoldier->usAnimState][layer].begin(); i != end; i++) {
			Filter* f = (*i).first;
			if (f != NULL) {
				if (f->Match(pSoldier)) {
					bool isLoaded = cache->GetLoadedSurface((*i).second.first) != NULL;
					if ((*i).second.second.alphaSurfaceType != NULL) {
						isLoaded = isLoaded && (cache->GetLoadedSurface((*i).second.second.alphaSurfaceIdx) != NULL);
					}
					return (isLoaded) ? &(*i).second.second : NULL;
				}
			} else { // no filter specified - matches all
				bool isLoaded = cache->GetLoadedSurface((*i).second.first) != NULL;
				if ((*i).second.second.alphaSurfaceType != NULL) {
					isLoaded = isLoaded && (cache->GetLoadedSurface((*i).second.second.alphaSurfaceIdx) != NULL);
				}
				return (isLoaded) ? &(*i).second.second : NULL;
			}
		}
		end = physAnimSurfaces[pSoldier->usAnimSurface][layer].end();
		for (FilterSurfaceTypeList::iterator i = physAnimSurfaces[pSoldier->usAnimSurface][layer].begin(); i != end; i++) {
			Filter* f = (*i).first;
			if (f != NULL) {
				if (f->Match(pSoldier)) {
					bool isLoaded = cache->GetLoadedSurface((*i).second.first) != NULL;
					if ((*i).second.second.alphaSurfaceType != NULL) {
						isLoaded = isLoaded && (cache->GetLoadedSurface((*i).second.second.alphaSurfaceIdx) != NULL);
					}
					return (isLoaded) ? &(*i).second.second : NULL;
				}
			} else { // no filter specified - matches all
				bool isLoaded = cache->GetLoadedSurface((*i).second.first) != NULL;
				if ((*i).second.second.alphaSurfaceType != NULL) {
					isLoaded = isLoaded && (cache->GetLoadedSurface((*i).second.second.alphaSurfaceIdx) != NULL);
				}
				return (isLoaded) ? &(*i).second.second : NULL;
			}
		}
		return NULL;
	};

};

}

#endif
