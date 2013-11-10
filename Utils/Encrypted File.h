#ifndef __ENCRYPTED_H_
#define __ENCRYPTED_H_

#include "types.h"

BOOLEAN LoadEncryptedDataFromFile(STR pFileName, STR16 pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount);
BOOLEAN LoadEncryptedDataFromFileRandomLine(STR pFileName, STR16 pDestString, UINT32 uiSeekAmount);
void DecodeString(STR16 pDestString, UINT32 uiSeekAmount);

#endif