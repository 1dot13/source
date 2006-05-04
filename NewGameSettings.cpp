#include "types.h"
#include "NewGameSettings.h"
#include "FileMan.h"

#include <windows.h>
#include <stdio.h>

#define GAME_INI_FILE "\\Ja2.ini"


#define SET_SYSTEM "JA2 System Settings"
#define SET_RESOLUTION "JA2 Screen Resolution Settings"
#define SET_RAIN "JA2 Rain Settings"
#define SET_THUNDER "JA2 Thunder Settings"
#define SET_TURN_SPEED "JA2 Turnbased Animation Speed Settings"
#define SET_STRATEGIC "JA2 Strategic Settings"
#define SET_SPECIAL "JA2 Special Settings"
#define SET_TACTICAL "JA2 Tactical Settings"
#define SET_TACTICAL_AI "JA2 Tactical AI Settings"
#define SET_GRAPHIC "JA2 Graphic Settings"
#define SET_GAMEPLAY "JA2 Gameplay Settings"
#define SET_SOUND "JA2 Sound Settings"


UINT8 gubDeadLockDelay = 15;
UINT8 ubStraightSightRange = 13;

BOOLEAN gfVSync = FALSE;
BOOLEAN gfAllowRain = FALSE;

UINT16 gusRainChancePerDay = 100, gusRainMinLength = 60, gusRainMaxLength = 180;


extern UINT8 gubPlayerTurnSpeedUpFactor;
extern UINT8 gubEnemyTurnSpeedUpFactor;
extern UINT8 gubCreatureTurnSpeedUpFactor;
extern UINT8 gubMilitiaTurnSpeedUpFactor;
extern UINT8 gubCivTurnSpeedUpFactor;

extern UINT32 guiMinLightningInterval;
extern UINT32 guiMaxLightningInterval;
extern UINT32 guiMinDLInterval;
extern UINT32 guiMaxDLInterval;
extern UINT32 guiProlongLightningIfSeenSomeone;
extern UINT32 guiChanceToDoLightningBetweenTurns;

extern UINT32 guiMaxRainDrops;

BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents = FALSE;
BOOLEAN gfAllowMilitiaGroups = TRUE;
BOOLEAN gfAllowReinforcments = TRUE;
BOOLEAN gfAllowReinforcmentsOnlyInCity = FALSE;
UINT32	guiBaseQueenPoolIncrement = 60;

BOOLEAN fAllowTacticalMilitiaCommand = TRUE;

UINT32 guiWeaponSoundEffectsVolume = 0;

typedef struct
{
	CHAR8* strParamName;
	CHAR8* strSectionName;
	LPVOID pVar;
	UINT8 ubVarType;
}TSetting;


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

TSetting gpSettings[] = 
{	
	//Video settings
	{"fVSync", SET_RESOLUTION, &gfVSync, VT_BOOLEAN},
	
	{"PlayerTurnSpeedUpFactor", SET_TURN_SPEED, (LPVOID)&gubPlayerTurnSpeedUpFactor, VT_UINT8},
	{"EnemyTurnSpeedUpFactor", SET_TURN_SPEED, (LPVOID)&gubEnemyTurnSpeedUpFactor, VT_UINT8},
	{"CreatureTurnSpeedUpFactor", SET_TURN_SPEED, (LPVOID)&gubCreatureTurnSpeedUpFactor, VT_UINT8},
	{"MilitiaTurnSpeedUpFactor", SET_TURN_SPEED, (LPVOID)&gubMilitiaTurnSpeedUpFactor, VT_UINT8},
	{"CivTurnSpeedUpFactor", SET_TURN_SPEED, (LPVOID)&gubCivTurnSpeedUpFactor, VT_UINT8},

	//Sound settings
	{"WeaponSoundEffectsVolume", SET_SOUND, &guiWeaponSoundEffectsVolume, VT_UINT32},

	// Militia Settings
	{"AllowTacticalMilitiaCommand", SET_TACTICAL, &fAllowTacticalMilitiaCommand, VT_BOOLEAN},
	
	{"AllowMilitiaMobileGroups", SET_GAMEPLAY, &gfAllowMilitiaGroups, VT_BOOLEAN},
	{"AllowReinforcments", SET_GAMEPLAY, &gfAllowReinforcments, VT_BOOLEAN},
	{"AllowReinforcmentsOnlyInCities", SET_GAMEPLAY, &gfAllowReinforcmentsOnlyInCity, VT_BOOLEAN},
	{"QueenPoolIncrementPerDifficultyLevel", SET_GAMEPLAY, &guiBaseQueenPoolIncrement, VT_UINT32},

	
	//Sight range
	{"BaseSightRange", SET_TACTICAL, &ubStraightSightRange, VT_UINT8},
	

	// Maximal search distance for grenades
	//{"MaxTossSearchDist", SET_TACTICAL_AI, &guiMaxTossSearchDist, VT_UINT32},

	//{"fEnableEmergencyButton_NumLock_ToSkipStrategicEvents", SET_SPECIAL, &gfEnableEmergencyButton_SkipStrategicEvents, VT_BOOLEAN},



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