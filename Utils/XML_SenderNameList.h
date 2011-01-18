#ifndef _XML_SENDERNAMELIST_H
#define _XML_SENDERNAMELIST_H

#include "Types.h"

#define MAX_SENDER_NAMES_CHARS 128

typedef struct
{
	UINT16	uiIndex; 
	CHAR16	Name[MAX_SENDER_NAMES_CHARS];

} SENDER_NAMES_VALUES;

extern SENDER_NAMES_VALUES zSenderNameList[500];


#endif
