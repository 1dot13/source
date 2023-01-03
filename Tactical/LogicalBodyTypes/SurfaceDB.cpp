#include "SurfaceDB.h"

namespace LogicalBodyTypes {

SurfaceDB::SurfaceDB(void) : AbstractXMLLoader(StartElementHandle, EndElementHandle, CharacterDataHandle) {
}

SurfaceDB::~SurfaceDB(void) {
	// Note: these handles are already freed on shutdown by the game (instances are recorded with
	// a double linked list see FreeAllStructureFiles). So no action required.
	/*for (StructTypeFileMap::iterator i = structTypeFiles.begin(); i != structTypeFiles.end(); i++) {
		FreeStructureFileRef(i->second);
	}*/
	for (AnimSurfaceTypeMap::iterator i = animSurfaceTypes.begin(); i != animSurfaceTypes.end(); i++) {
		// guess we have to take care of this here instead of SurfaceCache
		if (i->second->hVideoObject != NULL) {
			// on exit access violation in DestroyObjectPaletteTables
			// the video object seems to have been deleted already, only the memory is not null
			// we are not the master, surfacecache ought to be...
			// TODO figure something out. deactivated for now (memory leak)
			DeleteVideoObject(i->second->hVideoObject);
		}
		delete i->second;
	}
}

bool SurfaceDB::AddAnimSurfaceType(std::string name, AnimationSurfaceType* animSurfaceType, STRUCTURE_FILE_REF* pStructFile) {
	iterator i = animSurfaceTypes.find(name);
	if (i != animSurfaceTypes.end()) {
		return false;
	}
	surfaceStructs[animSurfaceType] = pStructFile;
	animSurfaceTypes[name] = animSurfaceType;

	return true;
}

STRUCTURE_FILE_REF* SurfaceDB::LoadStructFile(STR fileName) {
	StructTypeFileMap::iterator i = structTypeFiles.find(fileName);
	if (i == structTypeFiles.end()) {
		//CHECKF(LoadAnimationStateInstructions()); // should not need to call this here. Effects some global objects, but the function will have been invoked already.
		//LoadAnimationProfiles(); // should already be loaded
		STRUCTURE_FILE_REF* pStructureFileRef = LoadStructureFile(fileName);
		if (pStructureFileRef == NULL) {
			SET_ERROR("Animation structure file load failed - %s", fileName);
			return NULL;
		}
		structTypeFiles[fileName] = pStructureFileRef;
	}
	return structTypeFiles[fileName];
}

STRUCTURE_FILE_REF* SurfaceDB::GetStructFile(AnimationSurfaceType* animSurfaceType) {
	Surface2StructMap::iterator i = surfaceStructs.find(animSurfaceType);
	if (i == surfaceStructs.end()) return NULL;
	return i->second;
}

SurfaceDB::AnimSurfaceTypeMap::const_iterator SurfaceDB::begin() {
	return animSurfaceTypes.begin();
}

SurfaceDB::AnimSurfaceTypeMap::const_iterator SurfaceDB::end() {
	return animSurfaceTypes.end();
}

AnimationSurfaceType* SurfaceDB::GetAnimSurfaceType(std::string name) {
	iterator i = animSurfaceTypes.find(name);
	if (i != animSurfaceTypes.end()) {
		return i->second;
	}
	return NULL;
}

void XMLCALL SurfaceDB::StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_NONE:
			if (strcmp(name, "AnimSurfaces") == 0) {
				data->state = E_ELEMENT_ANIMSURFACES;
				break;
			}
		case E_ELEMENT_ANIMSURFACES:
			if (strcmp(name, "AnimSurface") == 0) {
				data->state = E_ELEMENT_ANIMSURFACE;
				XML_Char const* aName = GetAttribute("name", atts);
				XML_Char const* aFile = GetAttribute("file", atts);
				XML_Char const* aFlgs = GetAttribute("flags", atts);
				//XML_Char const* aType = GetAttribute("datatype", atts);
				XML_Char const* aType = GetAttribute("structdata", atts);
				XML_Char const* aDirs = GetAttribute("directions", atts);
				XML_Char const* aFrms = GetAttribute("framesperdir", atts);
				XML_Char const* aProf = GetAttribute("profile", atts);
				XML_Char const* aAlpha = GetAttribute("alpha", atts);
				if (aName == NULL || aFile == NULL || aFlgs == NULL || aDirs == NULL || aFrms == NULL || aProf == NULL) {
					throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				}
				AnimationSurfaceType* sType = new AnimationSurfaceType;
				memset(sType, 0, sizeof(AnimationSurfaceType));
				// note that ubName will not be used for lookup (we use a hashmap)
				// so we don't have to set it to anything sensible.
				sType->ubName = 0;
				if (strlen(aFile) > sizeof(sType->Filename) - 1) throw XMLParseException("Filename must not me longer than 49 characters!", name, data->pParser);
				strcpy(sType->Filename, aFile);
				if (!ConvertStringToUINT8(aFlgs, &(sType->ubFlags))) throw XMLParseException("Invalid flags attribute!", name, data->pParser);
				if (!ConvertStringToINT8(aFrms, &(sType->bProfile))) throw XMLParseException("Attribute 'profile' doesn't have a valid value!", name, data->pParser);
				if (!ConvertStringToUINT32(aDirs, &(sType->uiNumDirections))) throw XMLParseException("Attribute 'directions' doesn't have a valid value!", name, data->pParser);
				if (!ConvertStringToUINT32(aFrms, &(sType->uiNumFramesPerDir))) throw XMLParseException("Attribute 'framesperdir' doesn't have a valid value!", name, data->pParser);
				if (!FileExists(sType->Filename)) {
					std::string msg = "Animation surface file does not exist: ";
					msg += sType->Filename;
					throw XMLParseException(msg.c_str(), name, data->pParser);
				}
				STRUCTURE_FILE_REF* pStructFile = NULL;
				if (aType != NULL && strlen(aType) > 0 && strcmp(aType, "NONE") != 0) {
					char* sFileName = new char[strlen(aType) + 1];
					strcpy(sFileName, aType);
					if (!FileExists(sFileName)) {
						std::string msg = "Structure data file does not exist: ";
						msg += sFileName;
						throw XMLParseException(msg.c_str(), name, data->pParser); 
					}
					pStructFile = Instance().LoadStructFile(sFileName);
					if (!pStructFile) {
						delete sType;
						delete[] sFileName;
						throw XMLParseException("Could not load animation structure file!", name, data->pParser);
					}
					delete[] sFileName;
				}
				if (!Instance().AddAnimSurfaceType(aName, sType, pStructFile)) {
					delete sType;
					throw XMLParseException("AnimationSurfaceType defined twice, or invalid name!", name, data->pParser);
				}
				if (aAlpha != NULL && strcmp(aAlpha, "1") == 0) {
					std::string fileNameAlpha = aFile;
					std::string::size_type pos = fileNameAlpha.find_last_of(".");
					if (pos == std::string::npos) throw XMLParseException("Can't load alpha AnimationSurfaceType. Invalid file name!", name, data->pParser);
					fileNameAlpha.insert(pos, "_A");
					if (fileNameAlpha.size() > sizeof(sType->Filename) - 1) throw XMLParseException("Filename must not be longer than 49 characters!", name, data->pParser);
					AnimationSurfaceType* sTypeAlpha = new AnimationSurfaceType;
					memset(sTypeAlpha, 0, sizeof(AnimationSurfaceType));
					sTypeAlpha->ubName = 0;
					strcpy(sTypeAlpha->Filename, fileNameAlpha.c_str());
					std::string nameAlpha = aName;
					nameAlpha += "###ALPHA";
					if (!Instance().AddAnimSurfaceType(nameAlpha.c_str(), sTypeAlpha, pStructFile)) {
						delete sTypeAlpha;
						throw XMLParseException("AnimationSurfaceType defined twice, or invalid name!", name, data->pParser);
					}
				}
				break;
			}
		default:
			throw XMLParseException("Unexpected element!", name, data->pParser);
	}
}

void XMLCALL SurfaceDB::EndElementHandle(void* userData, const XML_Char* name) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_ELEMENT_ANIMSURFACE:
			if (strcmp(name, "AnimSurface") == 0) {
				data->state = E_ELEMENT_ANIMSURFACES;
				break;
			}
		case E_ELEMENT_ANIMSURFACES:
			if (strcmp(name, "AnimSurfaces") == 0) {
				data->state = E_NONE;
				break;
			}
		default:
			throw XMLParseException("Unexpected element! Closing tag missing?", name, data->pParser);
			return;
	}
}

void XMLCALL SurfaceDB::CharacterDataHandle(void* userData, const XML_Char* str, int len) {
}

}
