#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __EDITORMAPINFO_H
#define __EDITORMAPINFO_H

extern INT32 iNewMapWorldRows;
extern INT32 iNewMapWorldCols;

void SetupTextInputForOptions();
void UpdateOptions();
void ExtractAndUpdateOptions();
void SetupTextInputForMapInfo();
void UpdateMapInfo();
void ExtractAndUpdateMapInfo();
BOOLEAN ApplyNewExitGridValuesToTextFields();
void UpdateMapInfoFields();

extern SGPPaletteEntry gEditorLightColor;

extern BOOLEAN gfEditorForceShadeTableRebuild;

void LocateNextExitGrid();

enum
{
	PRIMETIME_LIGHT,
	NIGHTTIME_LIGHT,
	ALWAYSON_LIGHT,
	ANY_LIGHT//dnl ch86 210214
};

void ChangeLightDefault( INT8 bLightType );
extern INT8 gbDefaultLightType;

#endif
#endif



 


