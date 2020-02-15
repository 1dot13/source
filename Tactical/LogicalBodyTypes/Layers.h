#pragma once
#ifndef _LBT_LAYERS__H_
#define _LBT_LAYERS__H_

#include "LogicalBodyTypes/AbstractXMLLoader.h"
#include <stack>

namespace LogicalBodyTypes {

// Layers configuration implemented as singleton and loadable from XML.
class Layers : public AbstractXMLLoader, public Singleton<Layers> {

public:
	using Singleton <Layers>::Instance;
	struct LayerProperties {
		char* identifier;
		bool render;
		bool renderShadows;
	} typedef LayerProperties;
	typedef std::vector <LayerProperties> LayerPropertiesVector;

private:
	enum ParseState {
		E_ELEMENT_LAYERS = FIRST_SUBCLASS_STATE,
		E_ELEMENT_LAYER
	};
	typedef std::pair <UINT32, INT32> ZLevelPair;
	typedef std::stack <ZLevelPair> ZStack;
	typedef std::vector <ZStack> ZStackVector;
	struct ParseData : public AbstractXMLLoader::ParseData { // shadow ParseData type and also extend the ParseData stuct of the base class
		ParseData(XML_Parser* parser) : AbstractXMLLoader::ParseData(parser)
		{
			directionZStacks.resize(NUM_WORLD_DIRECTIONS);
		};
		ZStackVector directionZStacks;
	} typedef ParseData;
	struct LayerGraphNode {
		INT32 zIndex;
		LayerPropertiesVector::size_type index;
		LayerProperties* pLayerProperties;
	} typedef LayerGraphNode;
	typedef std::list <LayerGraphNode> LayerGraph;
	typedef std::vector <LayerGraph> LayerGraphsVector;
	LayerPropertiesVector layers;
	LayerGraphsVector directionLayerGraphs;

public:
	typedef LayerGraph::iterator LayerGraphIterator;

public:
	Layers(void);
	~Layers(void);
	LayerPropertiesVector::size_type GetCount();
	// returns -1 if layer doesn't exist
	LayerPropertiesVector::size_type GetIndex(std::string& identifier);
	const Layers::LayerProperties& GetLayerProperties(LayerPropertiesVector::size_type index);
	LayerGraphIterator GetIterator(WorldDirections direction);
	LayerGraphIterator GetIterationEnd(WorldDirections direction);
	LayerGraphIterator GetIterator(INT8 direction);
	LayerGraphIterator GetIterationEnd(INT8 direction);

private:
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	void AddLayerGraphNode(WorldDirections dir, std::string identifier, bool render, bool renderShadows, INT32 zIndex);
	static AbstractXMLLoader::ParseData* MakeParseData(XML_Parser* parser); // shadow factory function for ParseData instances
	void UpdateConfiguration();

};

}

#endif
