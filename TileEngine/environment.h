#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#define		ENV_TIME_00		0
#define		ENV_TIME_01		1
#define		ENV_TIME_02		2
#define		ENV_TIME_03		3
#define		ENV_TIME_04		4
#define		ENV_TIME_05		5
#define		ENV_TIME_06		6
#define		ENV_TIME_07		7
#define		ENV_TIME_08		8
#define		ENV_TIME_09		9
#define		ENV_TIME_10		10
#define		ENV_TIME_11		11
#define		ENV_TIME_12		12
#define		ENV_TIME_13		13
#define		ENV_TIME_14		14
#define		ENV_TIME_15		15
#define		ENV_TIME_16		16
#define		ENV_TIME_17		17
#define		ENV_TIME_18		18
#define		ENV_TIME_19		19
#define		ENV_TIME_20		20
#define		ENV_TIME_21		21
#define		ENV_TIME_22		22
#define		ENV_TIME_23		23

#define		ENV_NUM_TIMES				24

//Make sure you use 24 for end time hours and 0 for start time hours if
//midnight is the hour you wish to use.
#define		NIGHT_TIME_LIGHT_START_HOUR		21
#define		NIGHT_TIME_LIGHT_END_HOUR			7
#define		PRIME_TIME_LIGHT_START_HOUR		21
#define		PRIME_TIME_LIGHT_END_HOUR			24

enum
{
	WEATHER_FORECAST_NORMAL,
	WEATHER_FORECAST_RAIN,
	WEATHER_FORECAST_THUNDERSHOWERS,
	WEATHER_FORECAST_SANDSTORM,
	WEATHER_FORECAST_SNOW,

	WEATHER_FORECAST_MAX
};

// higher is darker, remember
#define NORMAL_LIGHTLEVEL_NIGHT 12
#define NORMAL_LIGHTLEVEL_DAY 3

void ForecastDayEvents( );

void EnvironmentController( BOOLEAN fCheckForLights );
void EnvEnableTOD(void);
void EnvDisableTOD(void);

void BuildDayAmbientSounds( ); 
void BuildDayLightLevels();
UINT8 GetTimeOfDayAmbientLightLevel();

void	ChangeWeather( UINT8 aSector, UINT8 aType );

UINT8	GetWeatherInCurrentSector();

extern UINT8		gubEnvLightValue;
extern BOOLEAN		gfDoLighting;

void TurnOnNightLights();
void TurnOffNightLights();
void TurnOnPrimeLights();
void TurnOffPrimeLights();



//affects whether or not time of day affects the lighting.  Underground
//maps have an ambient light level that is saved in the map, and doesn't change.
extern BOOLEAN gfCaves;
extern BOOLEAN gfBasement;

INT8 SectorTemperature( UINT32 uiTime, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void UpdateTemperature( UINT8 ubTemperatureCode );

// Flugente: the environment can interact with soldiers
void HandleEnvironmentHazard( );

#endif
