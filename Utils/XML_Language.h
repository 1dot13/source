#ifndef __XML_LANGUAGE_H
#define __XML_LANGUAGE_H

#include "Types.h"

#define MAX_MESSAGE_NAMES_CHARS		400

typedef struct
{
	UINT32	uiIndex; 
	CHAR16	Message[MAX_MESSAGE_NAMES_CHARS];

} LANGUAGE_LOCATION;

extern LANGUAGE_LOCATION zlanguageText[1000];

#endif
