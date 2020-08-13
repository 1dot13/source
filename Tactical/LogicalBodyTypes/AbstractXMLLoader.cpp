#include "AbstractXMLLoader.h"

namespace LogicalBodyTypes {

	AbstractXMLLoader::AbstractXMLLoader(XML_StartElementHandler startHandler, XML_EndElementHandler endHandler, XML_CharacterDataHandler charHandler, ParseDataFactoryFunc parseDataFactF) {
	startElementHandler = startHandler;
	endElementHandler = endHandler;
	characterDataHandler = charHandler;
	parseDataFactFuncPntr = parseDataFactF;
}

AbstractXMLLoader::~AbstractXMLLoader(void) {
}

AbstractXMLLoader::ParseData::ParseData(XML_Parser* parser) {
	pParser = parser;
	state = E_NONE;
	level = 0;
	szCharData[0] = '\0';
	szErrorTxt[0] = '\0';
}

AbstractXMLLoader::ParseData* AbstractXMLLoader::MakeParseData(XML_Parser* parser) {
	return new ParseData(parser);
}

bool AbstractXMLLoader::LoadFromFile(const char* directoryName, const char* fileName) {
	HWFILE hFile;
	UINT32 uiBytesRead;
	UINT32 uiFSize;
	CHAR8* lpcBuffer;
	char fileNameFull[MAX_PATH + 1];
	if (strlen(fileName) + strlen(directoryName) >= MAX_PATH) {
		LiveMessage("Can't load file. Concatinated filename too long for buffer!");
		return false;
	}
	SetDirectoryName(directoryName);
	SetFileName(fileName);
	strcpy(fileNameFull, directoryName);
	strcat(fileNameFull, fileName);
	XML_Parser parser = XML_ParserCreate(NULL);
	AbstractXMLLoader::ParseData* data = parseDataFactFuncPntr(&parser);

	std::string msg = "Loading ";
	msg += fileName;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, msg.c_str());
	hFile = FileOpen(fileNameFull, FILE_ACCESS_READ, FALSE);
	if (!hFile) {
		delete data;
		return false;
	}
	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8*)MemAlloc(uiFSize + 1);
	if (!FileRead(hFile, lpcBuffer, uiFSize, &uiBytesRead)) {
		MemFree(lpcBuffer);
		delete data;
		return false;
	}
	lpcBuffer[uiFSize] = 0;
	FileClose(hFile);

	ExternalEntityArgs eeArgs;
	eeArgs.directoryName = directoryName;
	eeArgs.fileName = fileName;
	eeArgs.pParser = &parser;

	XML_SetElementHandler(parser, startElementHandler, endElementHandler);
	XML_SetCharacterDataHandler(parser, characterDataHandler);
	XML_SetUserData(parser, data);
	XML_SetExternalEntityRefHandler(parser, ExternalEntityHandler);
	XML_SetExternalEntityRefHandlerArg(parser, (void*)&eeArgs);

	try {
		if (!XML_Parse(parser, lpcBuffer, uiFSize, TRUE)) {
			CHAR8 errorBuf[512];
			sprintf(errorBuf, "XML Parser Error in %s[%d]: %s", fileNameFull, XML_GetCurrentLineNumber(parser), XML_ErrorString(XML_GetErrorCode(parser)));
			LiveMessage(errorBuf);
			MemFree(lpcBuffer);
			delete data;
			return false;
		}
	} catch (XMLParseException e) {
		CHAR8 errorBuf[512];
		sprintf(errorBuf, "XML Parser Exception in %s[%d]: %s", fileNameFull, e._LINE, e.what());
		LiveMessage(errorBuf);
		MemFree(lpcBuffer);
		XML_ParserFree(parser);
		delete data;
		return false;
	}
	MemFree(lpcBuffer);
	XML_ParserFree(parser);
	delete data;
	return true;
}

int XMLCALL AbstractXMLLoader::ExternalEntityHandler(XML_Parser args, const XML_Char* context, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId) {
	ExternalEntityArgs* eArgs = (ExternalEntityArgs*)args;
	char fileNameFull[MAX_PATH + 1];
	UINT32 uiBytesRead;
	UINT32 uiFSize;
	CHAR8* lpcBuffer;
	HWFILE hFile;
	if (strlen(eArgs->directoryName) + strlen(systemId) >= MAX_PATH) {
		LiveMessage("Can't load file specified in external entity. Concatinated filename too long for buffer!");
		return XML_STATUS_ERROR;
	}
	strcpy(fileNameFull, eArgs->directoryName);
	strcat(fileNameFull, systemId);

	hFile = FileOpen(fileNameFull, FILE_ACCESS_READ, FALSE);
	if (!hFile) {
		LiveMessage("File specified in external entity can't be opened!");
		return XML_STATUS_ERROR;
	}
	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8*)MemAlloc(uiFSize + 1);
	if (!FileRead(hFile, lpcBuffer, uiFSize, &uiBytesRead)) {
		LiveMessage("File specified in external entity couldn't be read!");
		MemFree(lpcBuffer);
		return XML_STATUS_ERROR;
	}
	lpcBuffer[uiFSize] = 0;
	FileClose(hFile);
	
	XML_Parser extParser = XML_ExternalEntityParserCreate(*eArgs->pParser, context, NULL);
	ParseData* data = (ParseData*)XML_GetUserData(*eArgs->pParser);
	data->pParser = &extParser;
	try {
		if (!XML_Parse(extParser, lpcBuffer, uiFSize, TRUE)) {
			CHAR8 errorBuf[512];
			sprintf(errorBuf, "XML Parser Error in external entity %s[%d]: %s", systemId, XML_GetCurrentLineNumber(extParser), XML_ErrorString(XML_GetErrorCode(extParser)));
			LiveMessage(errorBuf);
			MemFree(lpcBuffer);
			return XML_STATUS_ERROR;
		}
	} catch (XMLParseException e) {
		CHAR8 errorBuf[512];
		sprintf(errorBuf, "XML Parser Exception in external entity %s[%d]: %s", systemId, e._LINE, e.what());
		LiveMessage(errorBuf);
		XML_ParserFree(extParser);
		MemFree(lpcBuffer);
		return XML_STATUS_ERROR;
	}
	data->pParser = eArgs->pParser;
	XML_ParserFree(extParser);
	MemFree(lpcBuffer);
	return XML_STATUS_OK;
};


const char* AbstractXMLLoader::GetFileName() {
	return fileName;
}

const char* AbstractXMLLoader::GetDirectoryName() {
	return directoryName;
}

void AbstractXMLLoader::SetFileName(const char* fileName) {
	strcpy(this->fileName, fileName);
}

void AbstractXMLLoader::SetDirectoryName(const char* directoryName) {
	strcpy(this->directoryName, directoryName);
}

const XML_Char* AbstractXMLLoader::GetAttribute(const XML_Char* name, const XML_Char** atts) {
	for (const XML_Char** pIter = atts; *pIter != NULL; pIter++) {
		const XML_Char* key = *pIter++;
		if (strcmp(key, name) == 0) return *pIter;
	}
	return NULL;
}

bool AbstractXMLLoader::ConvertStringToINT8(const XML_Char* num, INT8* int8) {
	long l = strtol(num, NULL, 10);
	if (l > 127 || l < -128) return false;
	if (l == 0 && strcmp(num, "0") != 0) return false;
	*int8 = (INT8)l;
	return true;
}

bool AbstractXMLLoader::ConvertStringToUINT8(const XML_Char* num, UINT8* uInt8) {
	unsigned long ul = strtoul(num, NULL, 10);
	if (ul > 255) return false;
	if (ul == 0 && strcmp(num, "0") != 0) return false;
	*uInt8 = (UINT8)ul;
	return true;
}

bool AbstractXMLLoader::ConvertStringToINT32(const XML_Char* num, INT32* int32) {
	long l = strtol(num, NULL, 10);
	// TODO: add range checks also for long types (and use the defined macros)
	// TODO: string should be trimmed (other convert functions also)
	if (l == 0 && strcmp(num, "0") != 0) return false;
	*int32 = (INT32)l;
	return true;
}

bool AbstractXMLLoader::ConvertStringToUINT32(const XML_Char* num, UINT32* uInt32) {
	unsigned long ul = strtoul(num, NULL, 10);
	if (ul == 0 && strcmp(num, "0") != 0) return false;
	*uInt32 = ul;
	return true;
}

bool AbstractXMLLoader::ConvertStringToBOOLEAN(const XML_Char* num, BOOLEAN* fBoolean) {
	unsigned long ul = strtoul(num, NULL, 10);
	if (ul == 0 && strcmp(num, "0") != 0) return false;
	*fBoolean = ul != 0;
	return true;
}

}
