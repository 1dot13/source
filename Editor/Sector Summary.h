#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __SECTOR_SUMMARY_H
#define __SECTOR_SUMMARY_H

//This function can be very time consuming as it loads every map file with a valid
//coordinate name, analyses it, and builds a new global summary file.
void CreateGlobalSummary();

void CreateSummaryWindow();
void ReleaseSummaryWindow();
void DestroySummaryWindow();
void RenderSummaryWindow();
void LoadWorldInfo();

void UpdateSectorSummary( STR16 gszFilename, BOOLEAN fUpdate );

void SaveGlobalSummary();
void LoadGlobalSummary();

extern BOOLEAN gfGlobalSummaryExists;

extern BOOLEAN gfSummaryWindowActive;

extern BOOLEAN gSectorExists[16][16];

extern UINT16 gusNumEntriesWithOutdatedOrNoSummaryInfo;

extern BOOLEAN gfUpdateSummaryInfo;

extern UINT8 GetCurrentSummaryVersion();

void GetSectorFromFileName(STR16 szFileName, INT16& sSectorX, INT16& sSectorY, INT8& bSectorZ, BOOLEAN& fAltMap);//dnl ch31 140909
void ResetCustomFileSectorSummary(void);//dnl ch30 150909

#endif
#endif