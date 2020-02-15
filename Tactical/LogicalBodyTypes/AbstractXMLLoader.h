#pragma once

#ifndef _LBT_ABSTRACTXMLLOADER__H_
#define _LBT_ABSTRACTXMLLOADER__H_

#include <string>
#include <map>
#include "expat.h"
#include "Types.h"
#include "XML.h"
#include "TopicIDs.h"
#include "TopicOps.h"
#include "DEBUG.H"
#include "FileMan.h"
#include "MemMan.h"
#include "Debug Control.h"
#include "Sys Globals.h"
#include "LogicalBodyTypes/Singleton.h"
#include "LogicalBodyTypes/XMLParseException.h"

namespace LogicalBodyTypes {

class AbstractXMLLoader : public ISingleton<AbstractXMLLoader>
{

protected:
	enum ParseState {
		E_NONE,
		FIRST_SUBCLASS_STATE
	};
	class ParseData {
	public:
		ParseData(XML_Parser* parser = NULL);
		XML_Parser* pParser;
		int state; // we sacrifice type safety for the enumerators so subclasses can define their own states
		UINT32 level;
		CHAR8 szCharData[MAX_CHAR_DATA_LENGTH + 1];
		CHAR8 szErrorTxt[MAX_PATH + 1];
	} typedef ParseData;
	struct ExternalEntityArgs {
		const char* directoryName;
		const char* fileName;
		XML_Parser* pParser;
	} typedef ExternalEntityArgs;
	typedef ParseData* (*ParseDataFactoryFunc) (XML_Parser* parser);

private:
	XML_StartElementHandler startElementHandler;
	XML_EndElementHandler endElementHandler;
	XML_CharacterDataHandler characterDataHandler;
	ParseDataFactoryFunc parseDataFactFuncPntr;
	char directoryName[MAX_PATH + 1];
	char fileName[MAX_PATH + 1];

public:
	AbstractXMLLoader(XML_StartElementHandler startHandler, XML_EndElementHandler endHandler, XML_CharacterDataHandler charHandler, ParseDataFactoryFunc parseDataFactF = MakeParseData);
	~AbstractXMLLoader(void);
	bool LoadFromFile(const char* directoryName, const char* fileName);
	const char* GetFileName();
	const char* GetDirectoryName();
	void SetFileName(const char* fileName);
	void SetDirectoryName(const char* directoryName);

protected:
	static const XML_Char* GetAttribute(const XML_Char* name, const XML_Char** atts);
	static bool ConvertStringToINT8(const XML_Char* num, INT8* uInt8);
	static bool ConvertStringToUINT8(const XML_Char* num, UINT8* uInt8);
	static bool ConvertStringToINT32(const XML_Char* num, INT32* int32);
	static bool ConvertStringToUINT32(const XML_Char* num, UINT32* uInt32);
	static bool ConvertStringToBOOLEAN(const XML_Char* num, BOOLEAN* fBoolean);
	static ParseData* MakeParseData(XML_Parser* parser);

private:
	static AbstractXMLLoader& Instance();
	static void XMLCALL StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts);
	static void XMLCALL EndElementHandle(void* userData, const XML_Char* name);
	static void XMLCALL CharacterDataHandle(void* userData, const XML_Char* str, int len);
	static int XMLCALL ExternalEntityHandler(XML_Parser args, const XML_Char *context, const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId);

};

}

#endif
