#ifndef _LBT_SURFACE__H_
#define _LBT_SURFACE__H_
#pragma once

#include "LogicalBodyTypes/AbstractXMLLoader.h"
#include "Animation Data.h"
#include "Utilities.h"
#include "Sys Globals.h"
#include "structure.h"

extern void FreeStructureFileRef(STRUCTURE_FILE_REF* pFileRef);

namespace LogicalBodyTypes {

// Singleton serving as container for AnimationSurfaceType instances.
// Implements loading said instances from an XML files.
class SurfaceDB : public AbstractXMLLoader, public Singleton<SurfaceDB> {

public:
	using Singleton <SurfaceDB>::Instance;
	typedef std::map <std::string, AnimationSurfaceType*> AnimSurfaceTypeMap;
	typedef std::map <std::string, STRUCTURE_FILE_REF*> StructTypeFileMap;
	typedef std::map <AnimationSurfaceType*, STRUCTURE_FILE_REF*> Surface2StructMap;
	typedef AnimSurfaceTypeMap::const_iterator iterator;

private:
	enum ParseState {
		E_ELEMENT_ANIMSURFACES = FIRST_SUBCLASS_STATE,
		E_ELEMENT_ANIMSURFACE
	};
	AnimSurfaceTypeMap animSurfaceTypes;
	StructTypeFileMap structTypeFiles;
	Surface2StructMap surfaceStructs;

public:
	SurfaceDB(void);
	~SurfaceDB(void);
	AnimSurfaceTypeMap::const_iterator begin();
	AnimSurfaceTypeMap::const_iterator end();
	AnimationSurfaceType* GetAnimSurfaceType(std::string name);
	STRUCTURE_FILE_REF* GetStructFile(AnimationSurfaceType* animSurfaceType);

private:
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static void XMLCALL ExternalEntityHandler(XML_Parser parser, const XML_Char* context, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);
	bool AddAnimSurfaceType(std::string name, AnimationSurfaceType* animSurfaceType, STRUCTURE_FILE_REF* pStructFile);
	STRUCTURE_FILE_REF* LoadStructFile(STR fileName);

};

}

#endif
