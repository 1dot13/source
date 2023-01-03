#pragma once
#ifndef _LBT_PALETTEDB__H_
#define _LBT_PALETTEDB__H_

#include <map>
#include "LogicalBodyTypes/Singleton.h"
#include "LogicalBodyTypes/AbstractXMLLoader.h"
#include "LogicalBodyTypes/PaletteTable.h"

namespace LogicalBodyTypes {

class PaletteDB : public AbstractXMLLoader, public Singleton<PaletteDB> {

public:
	using Singleton <PaletteDB>::Instance;
	typedef std::map<std::string, PaletteTable*> PaletteTableMap;

private:
	enum ParseState {
		E_ELEMENT_PALETTES = FIRST_SUBCLASS_STATE,
		E_ELEMENT_PALETTE
	};
	PaletteTableMap paletteTables;

public:
	PaletteDB(void);
	~PaletteDB(void);
	bool AddPaletteTable(std::string name, PaletteTable* paletteTable);
	PaletteTable* FindPaletteTable(std::string name);

private:
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static void XMLCALL ExternalEntityHandler(XML_Parser parser, const XML_Char* context, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);

};

}

#endif
