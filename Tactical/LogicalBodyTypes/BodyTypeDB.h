#pragma once

#ifndef _LBT_BODYTYPEDB__H_
#define _LBT_BODYTYPEDB__H_

#include <list>
#include "LogicalBodyTypes/Singleton.h"
#include "LogicalBodyTypes/AbstractXMLLoader.h"
#include "LogicalBodyTypes/BodyType.h"
#include "LogicalBodyTypes/FilterDB.h"
#include "LogicalBodyTypes/SurfaceDB.h"

namespace LogicalBodyTypes {

class BodyTypeDB : public AbstractXMLLoader, public Singleton<BodyTypeDB> {

public:
	using Singleton <BodyTypeDB>::Instance;
	typedef std::list<BodyType*> BodyTypeList;
	//TOTAL_SOLDIERS
private:
	enum ParseState {
		E_ELEMENT_BODYTYPES = FIRST_SUBCLASS_STATE,
		E_ELEMENT_BODYTYPE,
		E_ELEMENT_LAYER_CONFIGS,
		E_ELEMENT_LAYER_CONFIG,
		E_ELEMENT_SURFACES,
		E_ELEMENT_LAYER,
		E_ELEMENT_PROP,
		E_ELEMENT_SURFACE
	};
	struct ParseData : public AbstractXMLLoader::ParseData { // shadow ParseData type and also extend the ParseData stuct of the base class
		ParseData(XML_Parser* parser) : AbstractXMLLoader::ParseData(parser) {
			currentBodyType = NULL;
			filter = NULL;
			paletteTable = NULL;
			layer = -1;
			layerPropertiesConfig.clear();			
		};
		~ParseData() {
			if (currentBodyType != NULL) delete currentBodyType;
		};
		BodyType* currentBodyType;
		Filter* filter;
		PaletteTable* paletteTable;
		Layers::LayerPropertiesVector::size_type layer;
		std::map<Layers::LayerPropertiesVector::size_type, Layers::LayerProperties> layerPropertiesConfig;
	} typedef ParseData;
	BodyTypeList bodyTypes;

public:
	BodyTypeDB(void);
	~BodyTypeDB(void);
	void Add(BodyType* bodyType);

	inline BodyType* Find(SOLDIERTYPE* pSoldier) {
		BodyTypeList::iterator end = bodyTypes.end();
		for (BodyTypeList::iterator i = bodyTypes.begin(); i != end; i++) {
			// if we find a bodytype, but it does not have any surfaces, it is meant to exclude objects
			// from being matched against bodytypes so we return NULL in that case.
			if ((*i)->Match(pSoldier)) return ((*i)->HasSurfaces()) ? *i : NULL;
		}
		return NULL;
	};

private:
	static void RegisterEnumerators();
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	static void XMLCALL ExternalEntityHandler(XML_Parser parser, const XML_Char* context, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);
	static AbstractXMLLoader::ParseData* MakeParseData(XML_Parser* parser); // shadow factory function for ParseData instances

};

}

#endif
