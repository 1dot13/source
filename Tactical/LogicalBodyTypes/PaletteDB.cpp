#include "PaletteDB.h"

namespace LogicalBodyTypes {

PaletteDB::PaletteDB(void) : AbstractXMLLoader(StartElementHandle, EndElementHandle, CharacterDataHandle) {
}

PaletteDB::~PaletteDB(void) {
	PaletteTableMap::iterator i = paletteTables.begin();
	for (; i != paletteTables.end(); i++) delete i->second;
}

bool PaletteDB::AddPaletteTable(std::string name, PaletteTable* paletteTable) {
	PaletteTableMap::iterator i = paletteTables.find(name);
	if (i != paletteTables.end()) return false;
	paletteTables[name] = paletteTable;
	return true;
}

PaletteTable* PaletteDB::FindPaletteTable(std::string name) {
	PaletteTableMap::iterator i = paletteTables.find(name);
	if (i == paletteTables.end()) return NULL;
	return i->second;
}

void XMLCALL PaletteDB::StartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_NONE:
			if (strcmp(name, "Palettes") == 0) {
				data->state = E_ELEMENT_PALETTES;
				break;
			}
		case E_ELEMENT_PALETTES:
			if (strcmp(name, "Palette") == 0) {
				data->state = E_ELEMENT_PALETTE;
				XML_Char const* aName = GetAttribute("name", atts);
				XML_Char const* aFileName = GetAttribute("filename", atts);
				if (aFileName == NULL || aName == NULL) throw XMLParseException("Mandatory attribute missing!", name, data->pParser);
				PaletteTable* paletteTable = new PaletteTable();
				if (!paletteTable->Load(aFileName)) throw XMLParseException("Palette table could not be loaded from the specified file!", name, data->pParser);
				Instance().AddPaletteTable(aName, paletteTable);
				break;
			}
		default:
			throw XMLParseException("Unexpected element!", name, data->pParser);
	}
}

void XMLCALL PaletteDB::EndElementHandle(void* userData, const XML_Char* name) {
	ParseData* data = (ParseData*)userData;
	switch (data->state) {
		case E_ELEMENT_PALETTE:
			if (strcmp(name, "Palette") == 0) {
				data->state = E_ELEMENT_PALETTES;
				break;
			}
		case E_ELEMENT_PALETTES:
			if (strcmp(name, "Palettes") == 0) {
				data->state = E_NONE;
				break;
			}
		default:
			throw XMLParseException("Unexpected element! Closing tag missing?", name, data->pParser);
			return;
	}
}

void XMLCALL PaletteDB::CharacterDataHandle(void* userData, const XML_Char* str, int len) {
}


/*
BOOLEAN SOLDIERTYPE::CreateSoldierPalettes( void )
{
	CHAR8 zColFilename[100];
	INT32 iWhich;
	INT32 cnt;
	SGPPaletteEntry Temp8BPPPalette[ 256 ];

	if (this->p8BPPPalette != NULL) {
		MemFree(this->p8BPPPalette);
		this->p8BPPPalette = NULL;
	}

	// Allocate mem for new palette
	this->p8BPPPalette = (SGPPaletteEntry *) MemAlloc( sizeof( SGPPaletteEntry ) * 256 );
	memset(this->p8BPPPalette, 0, sizeof(SGPPaletteEntry) * 256);
	CHECKF(this->p8BPPPalette != NULL);

	// Use col file
	if (CreateSGPPaletteFromCOLFile(Temp8BPPPalette, zColFilename)) {
		// Copy into palette
		memcpy(this->p8BPPPalette, Temp8BPPPalette, sizeof( this->p8BPPPalette ) * 256);
	} else {
	}

	if (this->p16BPPPalette != NULL) {
		MemFree(this->p16BPPPalette);
		this->p16BPPPalette = NULL;
	}

	// -- BUILD 16BPP Palette from this
	this->p16BPPPalette = Create16BPPPalette(this->p8BPPPalette);

	for (iWhich = 0; iWhich < NUM_SOLDIER_SHADES; iWhich++) {
		if (this->pShades[iWhich] != NULL) {
			MemFree(this->pShades[iWhich]);
			this->pShades[iWhich] = NULL;
		}
	}

	for (iWhich = 0; iWhich < NUM_SOLDIER_EFFECTSHADES; iWhich++) {
		if (this->pEffectShades[iWhich] != NULL) {
			MemFree( this->pEffectShades[ iWhich ] );
			this->pEffectShades[iWhich] = NULL;
		}
	}

	for (iWhich = 0; iWhich < 20; iWhich++) {
		if (this->pGlowShades[iWhich] != NULL) {
			MemFree( this->pGlowShades[ iWhich ]);
			this->pGlowShades[iWhich] = NULL;
		}
	}

	CreateSoldierPaletteTables( this, HVOBJECT_GLOW_GREEN );

	this->pEffectShades[0] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	this->pEffectShades[1] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 150, 100, TRUE);

	// Build shades for glowing visible bad guy

	// First do visible guy
	this->pGlowShades[0] = Create16BPPPaletteShaded(this->p8BPPPalette, 255, 255, 255, FALSE);
	for (cnt = 1; cnt < 10; cnt++) {
		this->pGlowShades[cnt] = CreateEnemyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[cnt], 255, FALSE);
	}

	// Now for gray guy...
	this->pGlowShades[10] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	for (cnt = 11; cnt < 19; cnt++) {
		this->pGlowShades[cnt] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[cnt], 0, FALSE);
	}
	this->pGlowShades[19] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gRedGlowR[18], 0, FALSE);


	// ATE: OK, piggyback on the shades we are not using for 2 colored lighting....
	// ORANGE, VISIBLE GUY
	this->pShades[20] = Create16BPPPaletteShaded(this->p8BPPPalette, 255, 255, 255, FALSE);
	for (cnt = 21; cnt < 30; cnt++) {
		this->pShades[cnt] = CreateEnemyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[( cnt - 20 )], gOrangeGlowG[( cnt - 20 )], TRUE);
	}

	// ORANGE, GREY GUY
	this->pShades[30] = Create16BPPPaletteShaded(this->p8BPPPalette, 100, 100, 100, TRUE);
	for (cnt = 31; cnt < 39; cnt++) {
		this->pShades[cnt] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[(cnt - 20)], gOrangeGlowG[(cnt - 20 )], TRUE);
	}
	this->pShades[39] = CreateEnemyGreyGlow16BPPPalette(this->p8BPPPalette, gOrangeGlowR[18], gOrangeGlowG[18], TRUE);

	return(TRUE);
}
*/


}
