#include "Layers.h"
#include <iostream>
#include <fstream>

namespace LogicalBodyTypes {

Layers::Layers() : AbstractXMLLoader(StartElementHandle, EndElementHandle, CharacterDataHandle, MakeParseData) {
	directionLayerGraphs.resize(NUM_WORLD_DIRECTIONS);
}

Layers::~Layers(void) {
	for (LayerPropertiesVector::size_type i = 0; i < layers.size(); i++) {
		delete[] layers[i].identifier;
	}
}

Layers::LayerGraphIterator Layers::GetIterator(WorldDirections direction) {
	return directionLayerGraphs[direction].begin();
}

Layers::LayerGraphIterator Layers::GetIterationEnd(WorldDirections direction) {
	return directionLayerGraphs[direction].end();
}

Layers::LayerGraphIterator Layers::GetIterator(INT8 direction) {
	return directionLayerGraphs[direction].begin();
}

Layers::LayerGraphIterator Layers::GetIterationEnd(INT8 direction) {

	return directionLayerGraphs[direction].end();
}

AbstractXMLLoader::ParseData* Layers::MakeParseData(XML_Parser* parser) {
	return new ParseData(parser);
}

void Layers::AddLayerGraphNode(WorldDirections dir, std::string identifier, bool render, bool renderShadows, INT32 zIndex) {
	// create layer property object and push it into the layers properties container - if layer not already there
	Layers::LayerPropertiesVector::size_type index = GetIndex(identifier);
	if (index == -1) {
		LayerProperties props;
		props.identifier = new char[identifier.size() + 1];
		strcpy(props.identifier, identifier.c_str());
		props.render = render;
		props.renderShadows = renderShadows;
		layers.push_back(props);
		index = GetIndex(identifier);
	}

	// create node with zindex and a pointer to the layer properties object
	LayerGraphNode node;
	node.zIndex = zIndex;
	node.index = index;
	node.pLayerProperties = NULL;	// since the pointers would be invalided once the vector has to be reallocated,
									// we set the pointers after the layer configuration is finalized (see UpdateConfiguration)

	// add node to direction specific layer graph
	LayerGraph::iterator i = directionLayerGraphs[dir].begin();
	for (; i != directionLayerGraphs[dir].end(); i++) {
		if (i->zIndex > zIndex) break;
	}
	directionLayerGraphs[dir].insert(i, node);
}

void Layers::UpdateConfiguration() {
	for (int d = NORTH; d < NUM_WORLD_DIRECTIONS; ++d) {
		WorldDirections dir = static_cast<WorldDirections>(d);
		LayerGraph::iterator i = directionLayerGraphs[dir].begin();
		for (; i != directionLayerGraphs[dir].end(); i++) {
			i->pLayerProperties = &layers[i->index];
		}
	}
}

Layers::LayerPropertiesVector::size_type Layers::GetCount() {
	return layers.size();
}

Layers::LayerPropertiesVector::size_type Layers::GetIndex(std::string& identifier) {
	for (LayerPropertiesVector::size_type i = 0; i < layers.size(); i++) {
		if (strcmp(layers[i].identifier, identifier.c_str()) == 0) return i;
	}
	return -1;
}

const Layers::LayerProperties& Layers::GetLayerProperties(LayerPropertiesVector::size_type index) {
	return layers[index];
}

void Layers::StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_NONE:
			if (strcmp(name, "Layers") == 0) {
				data->level = 0;
				data->state = E_ELEMENT_LAYERS;
				break;
			}
		case E_ELEMENT_LAYERS:
		case E_ELEMENT_LAYER:
			if (strcmp(name, "Layer") == 0) {
				data->level++;
				data->state = E_ELEMENT_LAYER;
				XML_Char const* attrName = GetAttribute("name", atts);
				XML_Char const* attrRender = GetAttribute("render", atts);
				XML_Char const* attrShadow = GetAttribute("shadow", atts);
				XML_Char const* attrDirNorth = GetAttribute("zindex_north", atts);
				XML_Char const* attrDirNorthEast = GetAttribute("zindex_northeast", atts);
				XML_Char const* attrDirEast = GetAttribute("zindex_east", atts);
				XML_Char const* attrDirSouthEast = GetAttribute("zindex_southeast", atts);
				XML_Char const* attrDirSouth = GetAttribute("zindex_south", atts);
				XML_Char const* attrDirSouthWest = GetAttribute("zindex_southwest", atts);
				XML_Char const* attrDirWest = GetAttribute("zindex_west", atts);
				XML_Char const* attrDirNorthWest = GetAttribute("zindex_northwest", atts);
				if (attrName == NULL || attrRender == NULL || attrShadow == NULL) {
					throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				}
				BOOLEAN fRender;
				if (!ConvertStringToBOOLEAN(attrRender, &fRender)) throw XMLParseException("Attribute 'value' doesn't have a valid value!", name, data->pParser);
				BOOLEAN fShadow;
				if (!ConvertStringToBOOLEAN(attrShadow, &fShadow)) throw XMLParseException("Attribute 'shadow' doesn't have a valid value!", name, data->pParser);
				INT32 value;
				if (attrDirNorth != NULL) {
					if (!ConvertStringToINT32(attrDirNorth, &value)) throw XMLParseException("Attribute 'zindex_north' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[NORTH].push(ZLevelPair(data->level, value));
				}
				if (attrDirNorthEast != NULL) {
					if (!ConvertStringToINT32(attrDirNorthEast, &value)) throw XMLParseException("Attribute 'zindex_northeast' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[NORTHEAST].push(ZLevelPair(data->level, value));
				}
				if (attrDirEast != NULL) {
					if (!ConvertStringToINT32(attrDirEast, &value)) throw XMLParseException("Attribute 'zindex_east' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[EAST].push(ZLevelPair(data->level, value));
				}
				if (attrDirSouthEast != NULL) {
					if (!ConvertStringToINT32(attrDirSouthEast, &value)) throw XMLParseException("Attribute 'zindex_southeast' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[SOUTHEAST].push(ZLevelPair(data->level, value));
				}
				if (attrDirSouth != NULL) {
					if (!ConvertStringToINT32(attrDirSouth, &value)) throw XMLParseException("Attribute 'zindex_south' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[SOUTH].push(ZLevelPair(data->level, value));
				}
				if (attrDirSouthWest != NULL) {
					if (!ConvertStringToINT32(attrDirSouthWest, &value)) throw XMLParseException("Attribute 'zindex_southwest' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[SOUTHWEST].push(ZLevelPair(data->level, value));
				}
				if (attrDirWest != NULL) {
					if (!ConvertStringToINT32(attrDirWest, &value)) throw XMLParseException("Attribute 'zindex_west' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[WEST].push(ZLevelPair(data->level, value));
				}
				if (attrDirNorthWest != NULL) {
					if (!ConvertStringToINT32(attrDirNorthWest, &value)) throw XMLParseException("Attribute 'zindex_northwest' doesn't have a valid value!", name, data->pParser);
					data->directionZStacks[NORTHWEST].push(ZLevelPair(data->level, value));
				}

				for (int d = NORTH; d < NUM_WORLD_DIRECTIONS; ++d) {
					WorldDirections dir = static_cast<WorldDirections>(d);
					INT32 value = 0;
					if (!data->directionZStacks[dir].empty()) {
						value = data->directionZStacks[dir].top().second;
					}
					Instance().AddLayerGraphNode(dir, attrName, fRender == TRUE, fShadow == TRUE, value);
				}
				break;
			}
		default:
			throw XMLParseException("Unexpected element!", name, data->pParser);
	}
}

void Layers::EndElementHandle(void* userData, const XML_Char* name) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_ELEMENT_LAYER:
			if (strcmp(name, "Layer") == 0) {
				for (int d = NORTH; d < NUM_WORLD_DIRECTIONS; ++d) {
					if (!data->directionZStacks[static_cast<WorldDirections>(d)].empty()) {
						if (data->directionZStacks[static_cast<WorldDirections>(d)].top().first == data->level) {
							data->directionZStacks[d].pop();
						}
					}
				}
				if (--data->level == 0) data->state = E_ELEMENT_LAYERS;
				break;
			}
		case E_ELEMENT_LAYERS:
			if (strcmp(name, "Layers") == 0) {
				data->state = E_NONE;
				Instance().UpdateConfiguration();
				break;
			}
		default:
			throw XMLParseException("Unexpected element! Closing tag missing?", name, data->pParser);
			return;
	}
}

void Layers::CharacterDataHandle(void* userData, const XML_Char* str, int len) {
}

}
