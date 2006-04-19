#include "types.h"
#include "NewGameSettings.h"
#include "FileMan.h"

#include <windows.h>
#include <stdio.h>

#define GAME_INI_FILE "\\Ja2.ini"

BOOLEAN gfStretch = FALSE;
BOOLEAN gfStretchWinGDI = FALSE;
BOOLEAN gfVSync = FALSE;

BOOLEAN gfAllowRain = FALSE;

UINT16 gusRainChancePerDay = 100, gusRainMinLength = 60, gusRainMaxLength = 180;

extern UINT32 guiMinLightningInterval;
extern UINT32 guiMaxLightningInterval;
extern UINT32 guiMinDLInterval;
extern UINT32 guiMaxDLInterval;
extern UINT32 guiProlongLightningIfSeenSomeone;
extern UINT32 guiChanceToDoLightningBetweenTurns;

extern UINT32 guiMaxRainDrops;

typedef struct
{
	CHAR8* strParamName;
	CHAR8* strSectionName;
	LPVOID pVar;
	UINT8 ubVarType;
}TSetting;

#define SET_RAIN "JA2 Rain Settings"
#define SET_THUNDER "JA2 Thunder Settings"

enum
{
	VT_UINT32,
	VT_UINT16,
	VT_UINT8,
	VT_INT32,
	VT_INT16,
	VT_INT8,
	VT_BOOLEAN
};

TSetting gpSettings[] = {
	
	// Rain settings
	{"ALLOW_RAIN", SET_RAIN, &gfAllowRain, VT_BOOLEAN},
	{"RAIN_CHANCE_PER_DAY", SET_RAIN, &gusRainChancePerDay, VT_UINT16},
	{"RAIN_MIN_LENGTH_IN_MINUTES", SET_RAIN, &gusRainMinLength, VT_UINT16},
	{"RAIN_MAX_LENGTH_IN_MINUTES", SET_RAIN, &gusRainMaxLength, VT_UINT16},
	{"MAX_RAIN_DROPS", SET_RAIN, &guiMaxRainDrops, VT_UINT32},

	// Thunder settings
	{"MIN_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS", SET_THUNDER, &guiMinLightningInterval, VT_UINT32},
	{"MAX_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS", SET_THUNDER, &guiMaxLightningInterval, VT_UINT32},
	{"MIN_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS", SET_THUNDER, &guiMinDLInterval, VT_UINT32},
	{"MAX_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS", SET_THUNDER, &guiMaxDLInterval, VT_UINT32},
	{"PROLOGNE_DELAY_IF_SEEN_SOMEONE_DURING_LIGHTNING_IN_TURNBASED_IN_SECONDS", SET_THUNDER, &guiProlongLightningIfSeenSomeone, VT_UINT32},
	{"CHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS", SET_THUNDER, &guiChanceToDoLightningBetweenTurns, VT_UINT32},

	{0,0,0,VT_UINT8}
};


void NSSaveSettings()
{
	CHAR8 zStr[ 256 ];
	TSetting* pCurr = gpSettings;
	INT32 iBuf;
	BOOLEAN fFileCreated = !FileExists( GAME_INI_FILE );

	FileDelete( GAME_INI_FILE );

	while( pCurr->pVar )
	{
			switch( pCurr->ubVarType )
			{
			case VT_UINT32:
				iBuf = *((UINT32*)pCurr->pVar);
				break;
			case VT_UINT16:
				iBuf = *((UINT16*)pCurr->pVar);
				break;
			case VT_UINT8:
				iBuf = *((UINT8*)pCurr->pVar);
				break;
			case VT_INT32:
				iBuf = *((INT32*)pCurr->pVar);
				break;
			case VT_INT16:
				iBuf = *((INT16*)pCurr->pVar);
				break;
			case VT_INT8:
				iBuf = *((INT8*)pCurr->pVar);
				break;
			case VT_BOOLEAN:
				iBuf = *((BOOLEAN*)pCurr->pVar);
				break;
			}

		sprintf( zStr, "%d", iBuf );

		WritePrivateProfileString( pCurr->strSectionName, pCurr->strParamName, zStr, GAME_INI_FILE );

		++pCurr;
	}
}

void NSLoadSettings()
{
	CHAR8 zStr[ 256 ];
	UINT32	uiRetVal=0;
	TSetting* pCurr = gpSettings;
	INT32 iBuf;
	STRING512		INIFile;		// Path to the ini file

	// Get Executable Directory
	GetExecutableDirectory( INIFile );

	strcat(INIFile, "\\Ja2.ini");

	while( pCurr->pVar )
	{
		uiRetVal = GetPrivateProfileString( pCurr->strSectionName, pCurr->strParamName, "", zStr, 256, INIFile );
		if( uiRetVal && strlen( zStr ) )
		{
			sscanf( zStr, "%d", &iBuf );
			switch( pCurr->ubVarType )
			{
			case VT_UINT32:
				*((UINT32*)pCurr->pVar) = iBuf;
				break;
			case VT_UINT16:
				*((UINT16*)pCurr->pVar) = iBuf;
				break;
			case VT_UINT8:
				*((UINT8*)pCurr->pVar) = iBuf;
				break;
			case VT_INT32:
				*((INT32*)pCurr->pVar) = iBuf;
				break;
			case VT_INT16:
				*((INT16*)pCurr->pVar) = iBuf;
				break;
			case VT_INT8:
				*((INT8*)pCurr->pVar) = iBuf;
				break;
			case VT_BOOLEAN:
				*((BOOLEAN*)pCurr->pVar) = iBuf;
				break;
			}
		}
		pCurr++;
	}
}