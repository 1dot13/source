#pragma once

#ifndef _LBT_PALETTETABLE__H_
#define _LBT_PALETTETABLE__H_

#include "soldier control.h"
#include "Utilities.h"
#include "lighting.h"

extern UINT8 gRedGlowR[];
extern UINT8 gOrangeGlowR[];
extern UINT8 gOrangeGlowG[];
extern UINT16* CreateEnemyGlow16BPPPalette(SGPPaletteEntry* pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen);
extern UINT16* CreateEnemyGreyGlow16BPPPalette(SGPPaletteEntry* pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen);

namespace LogicalBodyTypes {

class PaletteTable : public SOLDIERTYPE {

public:
	PaletteTable();
	~PaletteTable(void);
	bool Load(std::string fileName);

private:
	bool CreateSGPPaletteFromActFile(SGPPaletteEntry *pPalette, std::string fileName);

};

}

#endif
