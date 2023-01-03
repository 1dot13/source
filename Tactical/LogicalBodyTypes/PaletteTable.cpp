#include "PaletteTable.h"

namespace LogicalBodyTypes {

PaletteTable::PaletteTable() {
}

PaletteTable::~PaletteTable() {
	// FREE PALETTES
	if (this->p8BPPPalette != NULL)
	{
		MemFree(this->p8BPPPalette);
		this->p8BPPPalette = NULL;
	}

	if (this->p16BPPPalette != NULL)
	{
		MemFree(this->p16BPPPalette);
		this->p16BPPPalette = NULL;
	}

	for (int cnt = 0; cnt < NUM_SOLDIER_SHADES; cnt++)
	{
		if (this->pShades[cnt] != NULL)
		{
			MemFree(this->pShades[cnt]);
			this->pShades[cnt] = NULL;
		}
	}

	for (int cnt = 0; cnt < NUM_SOLDIER_EFFECTSHADES; cnt++)
	{
		if (this->pEffectShades[cnt] != NULL)
		{
			MemFree(this->pEffectShades[cnt]);
			this->pEffectShades[cnt] = NULL;
		}
	}

	// Delete glows
	for (int cnt = 0; cnt < 20; cnt++)
	{
		if (this->pGlowShades[cnt] != NULL)
		{
			MemFree(this->pGlowShades[cnt]);
			this->pGlowShades[cnt] = NULL;
		}

	}
}

bool PaletteTable::CreateSGPPaletteFromActFile(SGPPaletteEntry *pPalette, std::string fileName) {
	HWFILE hFileHandle;
	UINT32 cnt;
	char* colFileName = new char[fileName.size() + 1];
	strcpy(colFileName, fileName.c_str());
	if (!FileExists(colFileName)) {
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot find COL file");
		return false;
	}
	if ((hFileHandle = FileOpen(colFileName, FILE_ACCESS_READ, FALSE)) == 0) {
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Cannot open COL file");
		return false;
	}
	for (cnt = 0; cnt < 256; cnt++) {
		FileRead(hFileHandle, &pPalette[cnt].peRed, sizeof(UINT8), NULL);
		FileRead(hFileHandle, &pPalette[cnt].peGreen, sizeof(UINT8), NULL);
		FileRead(hFileHandle, &pPalette[cnt].peBlue, sizeof(UINT8), NULL);
	}
	FileClose(hFileHandle);
	delete[] colFileName;
	return true;
}

bool PaletteTable::Load(std::string fileName) {
	INT32 iWhich;
	INT32 cnt;
	SGPPaletteEntry	Temp8BPPPalette[256];

	if (this->p8BPPPalette != NULL) {
		MemFree(this->p8BPPPalette);
		this->p8BPPPalette = NULL;
	}
	this->p8BPPPalette = (SGPPaletteEntry*)MemAlloc(sizeof(SGPPaletteEntry) * 256);
	memset(this->p8BPPPalette, 0, sizeof(SGPPaletteEntry) * 256);
	if (CreateSGPPaletteFromActFile(Temp8BPPPalette, (CHAR8*)fileName.c_str())) {
		memcpy(this->p8BPPPalette, Temp8BPPPalette, sizeof(this->p8BPPPalette) * 256);
	} else {
		return false;
	}

	if (this->p16BPPPalette != NULL ) {
		MemFree(this->p16BPPPalette);
		this->p16BPPPalette = NULL;
	}
	this->p16BPPPalette = Create16BPPPalette(this->p8BPPPalette);

	for (iWhich = 0; iWhich < NUM_SOLDIER_SHADES; iWhich++) {
		if (this->pShades[ iWhich ] != NULL) {
			MemFree(this->pShades[iWhich]);
			this->pShades[ iWhich ] = NULL;
		}
	}
	for (iWhich = 0; iWhich < NUM_SOLDIER_EFFECTSHADES; iWhich++) {
		if (this->pEffectShades[iWhich] != NULL) {
			MemFree(this->pEffectShades[iWhich]);
			this->pEffectShades[iWhich] = NULL;
		}
	}
	for (iWhich = 0; iWhich < 20; iWhich++) {
		if (this->pGlowShades[iWhich] != NULL) {
			MemFree(this->pGlowShades[iWhich]);
			this->pGlowShades[iWhich] = NULL;
		}
	}

	CreateSoldierPaletteTables(this, HVOBJECT_GLOW_GREEN);

	this->pEffectShades[0] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	this->pEffectShades[1] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 150, 100, TRUE);
	this->pGlowShades[0] = Create16BPPPaletteShaded(this->p8BPPPalette, 255, 255, 255, FALSE);
	for (cnt = 1; cnt < 10; cnt++) {
		this->pGlowShades[cnt] = CreateEnemyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[cnt], 255, FALSE);
	}
	this->pGlowShades[10] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	for (cnt = 11; cnt < 19; cnt++) {
		this->pGlowShades[cnt] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[cnt], 0, FALSE);
	}
	this->pGlowShades[19] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[18], 0, FALSE);
	this->pShades[20] = Create16BPPPaletteShaded(this->p8BPPPalette, 255, 255, 255, FALSE);
	for (cnt = 21; cnt < 30; cnt++) {
		this->pShades[cnt] = CreateEnemyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[(cnt - 20)], gOrangeGlowG[(cnt - 20)], TRUE);
	}
	this->pShades[30] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	for (cnt = 31; cnt < 39; cnt++) {
		this->pShades[cnt] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[( cnt - 20)], gOrangeGlowG[(cnt - 20)], TRUE);
	}
	this->pShades[39] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[18], gOrangeGlowG[18], TRUE);

	return true;
}

}
