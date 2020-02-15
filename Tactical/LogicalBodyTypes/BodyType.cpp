#include "BodyType.h"

namespace LogicalBodyTypes {

BodyType::BodyType(UINT32 cacheSize, Filter* filterP) {
	filter = filterP;
	hasSurfaces = false;
	cache = new SurfaceCache(cacheSize);
	animstates.resize(NUMANIMATIONSTATES);
	physAnimSurfaces.resize(NUMANIMATIONSURFACETYPES);
	for (int i = 0; i < NUMANIMATIONSTATES; i++) {
		animstates[i].resize(Layers::Instance().GetCount());
	}
	for (int i = 0; i < NUMANIMATIONSURFACETYPES; i++) {
		physAnimSurfaces[i].resize(Layers::Instance().GetCount());
	}
	// initialize body type specific layer properties with the layer specific layer properties.
	Layers::LayerPropertiesVector::size_type numLayers = Layers::Instance().GetCount();
	layerProperties.resize(numLayers);
	for (Layers::LayerPropertiesVector::size_type layer = 0; layer < numLayers; layer++) {
		layerProperties[layer] = Layers::Instance().GetLayerProperties(layer);
	}
}

BodyType::~BodyType(void) {
	delete cache;
}

void BodyType::AddSurfaceForAnimState(UINT16 usAnimState, Layers::LayerPropertiesVector::size_type layer, AnimationSurfaceType* animSurfaceType, AnimationSurfaceType* alphaSurfaceType, Filter* filter, PaletteTable* paletteTable) {
	STRUCTURE_FILE_REF* pStructFile = SurfaceDB::Instance().GetStructFile(animSurfaceType);
	UINT32 idx = cache->AddSurface(animSurfaceType, pStructFile);
	UINT32 idxAlpha = 0;
	if (alphaSurfaceType != NULL) {
		idxAlpha = cache->AddSurface(alphaSurfaceType, pStructFile);
	}
	animstates[usAnimState][layer].push_back(FilterSurfaceTypePair(filter, FilterSurfaceTypePair::second_type(idx, LogicalSurfaceType(animSurfaceType, alphaSurfaceType, idxAlpha, paletteTable))));
	hasSurfaces = true;
}

void BodyType::AddSurfaceForPhysSurface(UINT16 usAnimSurface, Layers::LayerPropertiesVector::size_type layer, AnimationSurfaceType* animSurfaceType, AnimationSurfaceType* alphaSurfaceType, Filter* filter, PaletteTable* paletteTable) {
	STRUCTURE_FILE_REF* pStructFile = SurfaceDB::Instance().GetStructFile(animSurfaceType);
	UINT32 idx = cache->AddSurface(animSurfaceType, pStructFile);
	UINT32 idxAlpha = 0;
	if (alphaSurfaceType != NULL) {
		idxAlpha = cache->AddSurface(alphaSurfaceType, pStructFile);
	}
	physAnimSurfaces[usAnimSurface][layer].push_back(FilterSurfaceTypePair(filter, FilterSurfaceTypePair::second_type(idx, LogicalSurfaceType(animSurfaceType, alphaSurfaceType, idxAlpha, paletteTable))));
	hasSurfaces = true;
}

void BodyType::SetLayerProperties(Layers::LayerPropertiesVector::size_type index, Layers::LayerProperties properties) {
	layerProperties[index] = properties; // TODO range check?
}

const Layers::LayerProperties* BodyType::GetLayerProperties(Layers::LayerPropertiesVector::size_type index) {
	return &layerProperties[index]; // TODO range check?
}

}
