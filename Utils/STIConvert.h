#ifndef __STCICONVERT_H
#define __STCICONVERT_H


#define CONVERT_ETRLE_COMPRESS							0x0020
#define CONVERT_TO_8_BIT								0x1000

void WriteSTIFile( INT8 *pData, SGPPaletteEntry *pPalette, INT16 sWidth, INT16 sHeight,	STR cOutputName, UINT32 fFlags, UINT32 uiAppDataSize );

UINT32 ETRLECompressSubImage( UINT8 * pDest, UINT32 uiDestLen, UINT8 * p8BPPBuffer, UINT16 usWidth, UINT16 usHeight, STCISubImage * pSubImage );

#endif