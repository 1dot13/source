#ifndef JA25UPDATE__C_
#define JA25UPDATE__C_

#include "Types.h"

#ifdef JA2UB

//Used in the editor to compensate for the 2_<map name>
#define	JA25_EXP_MAP_NAME_PREFIX						L""
#define	JA25_EXP_MAP_NAME_SIZE_OFFSET				0


//The starting sector of the Expanion pack
extern INT16 JA2_5_START_SECTOR_X;
extern INT16 JA2_5_START_SECTOR_Y;
//#define JA2_5_START_SECTOR_X			7
//#define JA2_5_START_SECTOR_Y			8

#define 	JA25_PROF_DAT "BINARYDATA\\Ja25Prof.dat"

/*
BOOLEAN ConvertJa2SaveToJa2_5();

BOOLEAN ImportOldProfilesAndProcessThem();

void GivePlayerStartingJa25Money();

UINT32	HandleDifficultyModifierForImportedGames();

BOOLEAN DoesJa25ImportSaveExistAndSeemValid();
*/
#endif

#endif
