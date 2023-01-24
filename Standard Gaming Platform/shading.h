#ifndef _SHADING_H_
#define _SHADING_H_


#include "himage.h"		// For SGPPaletteEntry
#include "vobject.h"	// For HVOBJECT_SHADE_TABLES
#include "vsurface.h"	// For

#ifdef __cplusplus
extern "C" {
#endif

BOOLEAN ShadesCalculateTables(SGPPaletteEntry *p8BPPPalette);

void BuildShadeTable(void);
void BuildIntensityTable(void);
void SetShadeTablePercent( FLOAT uiShadePercent );

void Init8BitTables(void);
BOOLEAN Set8BitModePalette(SGPPaletteEntry *pPal);

extern SGPPaletteEntry Shaded8BPPPalettes[HVOBJECT_SHADE_TABLES+3][256];
extern UINT8 ubColorTables[HVOBJECT_SHADE_TABLES+3][256];

extern UINT16	IntensityTable[65536];
extern UINT16	ShadeTable[65536];
extern UINT16	White16BPPPalette[ 256 ];
extern FLOAT	guiShadePercent;
extern FLOAT	guiBrightPercent;

#ifdef __cplusplus
}
#endif

#define DEFAULT_SHADE_LEVEL		4

#endif
