#include "XML.h"

const XML_Char* GetAttribute(const XML_Char* name, const XML_Char** atts) {
	for (const XML_Char** pIter = atts; *pIter != NULL; pIter++) {
		const XML_Char* key = *pIter++;
		if (strcmp(key, name) == 0) return *pIter;
	}
	return NULL;
}
