#pragma once
#ifndef _LBT_FILTERDB__H_
#define _LBT_FILTERDB__H_

#include "LogicalBodyTypes/AbstractXMLLoader.h"
#include "Animation Data.h"
#include "Weapons.h"
#include "LogicalBodyTypes/EnumeratorDB.h"
#include "LogicalBodyTypes/Filter.h"
#include <map>

namespace LogicalBodyTypes {

class FilterDB : public AbstractXMLLoader, public Singleton<FilterDB> {

public:
	using Singleton <FilterDB>::Instance;
	typedef std::map<std::string, Filter*> FilterMap;
	typedef FilterMap::const_iterator iterator;

private:
	enum ParseState {
		E_ELEMENT_FILTERS = FIRST_SUBCLASS_STATE,
		E_ELEMENT_FILTER,
		E_OPERATION,
		E_CRITERION
	};
	struct ParseData : public AbstractXMLLoader::ParseData { // shadow ParseData type and also extend the ParseData stuct of the base class
		ParseData(XML_Parser* parser) : AbstractXMLLoader::ParseData(parser) {
			currentFilter = NULL;
			operationFlags = 0;
			criterionName = "";
			criterionType = 0;
		};
		~ParseData() {
			// don't like this one bit, but we have to handle this here in case an exception is thrown
			// remember to set the pointer to NULL after the filter object has been added to the DB!
			if (currentFilter != NULL) delete currentFilter;
		};
		Filter* currentFilter;
		std::string filterName;
		UINT32 operationFlags;
		std::string criterionName;
		UINT32 criterionType;
	} typedef ParseData;
	FilterMap filters;

public:
	FilterDB(void);
	~FilterDB(void);
	FilterMap::const_iterator begin();
	FilterMap::const_iterator end();
	Filter* FindFilter(std::string name);

private:
	bool AddFilter(std::string name, Filter* filter);
	static void RegisterEnumerators();
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static void XMLCALL ExternalEntityHandler(XML_Parser parser, const XML_Char* context, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);
	static AbstractXMLLoader::ParseData* MakeParseData(XML_Parser* parser); // shadow factory function for ParseData instances

};

}

#endif
