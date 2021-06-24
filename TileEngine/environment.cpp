#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "sgp.h"
	#include "environment.h"
	#include "renderworld.h"
	#include "Sound Control.h"
	#include "overhead.h"
	#include "Game Clock.h"
	#include "Ambient Control.h"
	#include "Game Events.h"
	#include "message.h"
	#include "opplist.h"
	#include "Random.h"
	#include "Points.h"					// added by Flugente
	#include "Campaign.h"				// added by Flugente
	#include "Isometric Utils.h"		// added by Flugente
	#include "worldman.h"				// added by Flugente
	#include "Dialogue Control.h"		// added by Flugente
#endif

#include "Text.h"
#include "connect.h"

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

//effects whether or not time of day effects the lighting.	Underground
//maps have an ambient light level that is saved in the map, and doesn't change.
BOOLEAN			gfBasement = FALSE;
BOOLEAN			gfCaves = FALSE;


#define	ENV_TOD_FLAGS_DAY										0x00000001
#define	ENV_TOD_FLAGS_DAWN									0x00000002
#define	ENV_TOD_FLAGS_DUSK									0x00000004
#define	ENV_TOD_FLAGS_NIGHT									0x00000008

/*
#define		DAWNLIGHT_START											( 5 * 60 )
#define		DAWN_START													( 6 * 60 )
#define	DAY_START														( 8 * 60 )
#define		TWILLIGHT_START											( 19 * 60 )
#define		DUSK_START													( 20 * 60 )
#define	NIGHT_START													( 22 * 60 )
*/
#define		DAWN_START													( 6 * 60 + 47 )		//6:47AM
#define	DAY_START														( 7 * 60 + 5 )		//7:05AM
#define		DUSK_START													( 20 * 60 + 57 )	//8:57PM
#define	NIGHT_START													( 21 * 60 + 15 )	//9:15PM

#define		DAWN_TO_DAY													(DAY_START-DAWN_START)
#define		DAY_TO_DUSK													(DUSK_START-DAY_START)
#define		DUSK_TO_NIGHT												(NIGHT_START-DUSK_START)
#define		NIGHT_TO_DAWN												(24*60-NIGHT_START+DAWN_START)

//UINT32										guiEnvWeather	= 0;		// Flugente: this was used as a flagmask, but will now be used as an enum - we can't have several types of weather at the same time anyway
UINT32										guiRainLoop	= NO_SAMPLE;


// frame cues for lightning
UINT8 ubLightningTable[3][10][2]=
																{	{	{0,	15},
																		{1, 0},
																		{2, 0},
																		{3, 6},
																		{4, 0},
																		{5, 0},
																		{6, 0},
																		{7, 0},
																		{8, 0},
																		{9, 0}	},

																	{	{0,	15},
																		{1, 0},
																		{2, 0},
																		{3, 6},
																		{4, 0},
																		{5, 15},
																		{6, 0},
																		{7, 6},
																		{8, 0},
																		{9, 0}	},

																	{	{0,	15},
																		{1, 0},
																		{2, 15},
																		{3, 0},
																		{4, 0},
																		{5, 0},
																		{6, 0},
																		{7, 0},
																		{8, 0},
																		{9, 0}	}	};

// CJC: I don't think these are used anywhere!
UINT8			guiTODFlags[ENV_NUM_TIMES] = {
																			ENV_TOD_FLAGS_NIGHT,		// 00
																			ENV_TOD_FLAGS_NIGHT,		// 01
																			ENV_TOD_FLAGS_NIGHT,		// 02
																			ENV_TOD_FLAGS_NIGHT,		// 03
																			ENV_TOD_FLAGS_NIGHT,		// 04
																			ENV_TOD_FLAGS_DAWN,			// 05
																			ENV_TOD_FLAGS_DAWN,			// 06
																			ENV_TOD_FLAGS_DAWN,			// 07
																			ENV_TOD_FLAGS_DAY,		// 08
																			ENV_TOD_FLAGS_DAY,		// 09
																			ENV_TOD_FLAGS_DAY,		// 10
																			ENV_TOD_FLAGS_DAY,		// 11
																			ENV_TOD_FLAGS_DAY,			// 12
																			ENV_TOD_FLAGS_DAY,		// 13
																			ENV_TOD_FLAGS_DAY,			// 14
																			ENV_TOD_FLAGS_DAY,			// 15
																			ENV_TOD_FLAGS_DAY,			// 16
																			ENV_TOD_FLAGS_DAY,			// 17
																			ENV_TOD_FLAGS_DAY,			// 18
																			ENV_TOD_FLAGS_DUSK,			// 19
																			ENV_TOD_FLAGS_DUSK,			// 20
																			ENV_TOD_FLAGS_DUSK,			// 21
																			ENV_TOD_FLAGS_NIGHT,		// 22
																			ENV_TOD_FLAGS_NIGHT};		// 23

typedef enum
{
	COOL,
	WARM,
	HOT
} Temperatures;

typedef enum
{
	TEMPERATURE_DESERT_COOL,
	TEMPERATURE_DESERT_WARM,
	TEMPERATURE_DESERT_HOT,
	TEMPERATURE_GLOBAL_COOL,
	TEMPERATURE_GLOBAL_WARM,
	TEMPERATURE_GLOBAL_HOT,
} TemperatureEvents;

#define DESERT_WARM_START		( 8 * 60 )
#define DESERT_HOT_START		( 9 * 60 )
#define DESERT_HOT_END			(17 * 60 )
#define DESERT_WARM_END			(19 * 60 )

#define GLOBAL_WARM_START		( 9 * 60 )
#define GLOBAL_HOT_START		(12 * 60 )
#define GLOBAL_HOT_END			(14 * 60 )
#define GLOBAL_WARM_END			(17 * 60 )

#define HOT_DAY_LIGHTLEVEL 2

BOOLEAN		fTimeOfDayControls=TRUE;
UINT32		guiEnvTime=0;
UINT32		guiEnvDay=0;
UINT8			gubEnvLightValue = 0;
BOOLEAN		gfDoLighting		= FALSE;

UINT8		gubDesertTemperature = 0;
UINT8		gubGlobalTemperature = 0;

// local prototypes
void EnvDoLightning(void);

// polled by the game to handle time/atmosphere changes from gamescreen
void EnvironmentController( BOOLEAN fCheckForLights )
{
	UINT32			uiOldWorldHour;
	UINT8			ubLightAdjustFromWeather = 0;
	
	// do none of this stuff in the basement or caves
	if( gfBasement || gfCaves )
	{
		if (gGameSettings.fOptions[ TOPTION_RAIN_SOUND ] == TRUE)
		{
			if ( guiRainLoop != NO_SAMPLE )
			{
				SoundStop( guiRainLoop );
				guiRainLoop = NO_SAMPLE;
			}
		}

		return;
	}

	if(fTimeOfDayControls )
	{
		uiOldWorldHour = GetWorldHour();

		// If hour is different
		if ( uiOldWorldHour != guiEnvTime )
		{
			// Hour change....
			guiEnvTime=uiOldWorldHour;
		}

		//ExecuteStrategicEventsUntilTimeStamp( (UINT16)GetWorldTotalMin( ) );

		// Polled weather stuff...
		// ONly do indooors
		if( !gfBasement && !gfCaves )
		{
			if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_RAIN 
				 || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS 
				 || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SANDSTORM )
			{
				if (gGameSettings.fOptions[ TOPTION_RAIN_SOUND ] == TRUE)
				{
					if ( guiRainLoop == NO_SAMPLE )
					{
						if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS )
						{
							guiRainLoop	= PlayJA2Ambient( RAIN_1, 140, 0 );
						}
						else if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SANDSTORM )
						{
							guiRainLoop = PlayJA2Ambient( WIND, 105, 0 );
						}
						else
						{
							guiRainLoop	= PlayJA2Ambient( RAIN_1, 70, 0 );
						}
					}
				}

				// Do lightning if we want...
				if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS )
				{
					EnvDoLightning( );
				}
			}
			else
			{
				if (gGameSettings.fOptions[ TOPTION_RAIN_SOUND ] == TRUE)
				{
					if ( guiRainLoop != NO_SAMPLE )
					{
						SoundStop( guiRainLoop );
						guiRainLoop = NO_SAMPLE;
					}
				}
			}
		}

		if ( gfDoLighting && fCheckForLights )
		{
			// Adjust light level based on weather...
			ubLightAdjustFromWeather = GetTimeOfDayAmbientLightLevel();

			// ONly do indooors
			if( !gfBasement && !gfCaves )
			{
				// Rain storms....
				// Thunder showers.. make darker
				if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS )
				{
					ubLightAdjustFromWeather = (UINT8)(__min( gubEnvLightValue+2, NORMAL_LIGHTLEVEL_NIGHT ));
				}
				else if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_RAIN )
				{
					ubLightAdjustFromWeather = (UINT8)(__min( gubEnvLightValue+1, NORMAL_LIGHTLEVEL_NIGHT ));
				}
				else if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SANDSTORM )
				{
					ubLightAdjustFromWeather = (UINT8)(__min( gubEnvLightValue + 5, NORMAL_LIGHTLEVEL_NIGHT ));
				}
				else if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SNOW )
				{
					ubLightAdjustFromWeather = (UINT8)(__min( gubEnvLightValue + 1, NORMAL_LIGHTLEVEL_NIGHT ));
				}
			}

			LightSetBaseLevel( ubLightAdjustFromWeather );

			//Update Merc Lights since the above function modifies it.
			HandlePlayerTogglingLightEffects( FALSE );

			// Make teams look for all
			// AllTeamsLookForAll( FALSE );

			// Set global light value
			SetRenderFlags(RENDER_FLAG_FULL);
			gfDoLighting = FALSE;
		}
	}
}

void BuildDayLightLevels()
{
	UINT32 uiLoop, uiHour;

	/*
	// Dawn; light 12
	AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, DAWNLIGHT_START, NORMAL_LIGHTLEVEL_NIGHT - 1 );

	// loop from light 12 down to light 4
	for (uiLoop = 1; uiLoop < 8; uiLoop++)
	{
		AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, DAWN_START + 15 * uiLoop,	NORMAL_LIGHTLEVEL_NIGHT - 1 - uiLoop );
	}
	*/

	//Transition from night to day
	for( uiLoop = 0; uiLoop < 9; ++uiLoop )
	{
		AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, DAWN_START + 2 * uiLoop,	NORMAL_LIGHTLEVEL_NIGHT - 1 - uiLoop );
	}

	// Add events for hot times
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, DESERT_WARM_START, TEMPERATURE_DESERT_WARM );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, DESERT_HOT_START, TEMPERATURE_DESERT_HOT );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, DESERT_HOT_END, TEMPERATURE_DESERT_WARM );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, DESERT_WARM_END, TEMPERATURE_DESERT_COOL );

	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, GLOBAL_WARM_START, TEMPERATURE_GLOBAL_WARM );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, GLOBAL_HOT_START, TEMPERATURE_GLOBAL_HOT );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, GLOBAL_HOT_END, TEMPERATURE_GLOBAL_WARM );
	AddEveryDayStrategicEvent( EVENT_TEMPERATURE_UPDATE, GLOBAL_WARM_END, TEMPERATURE_GLOBAL_COOL );

/*
	// Twilight; light 5
	AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, TWILLIGHT_START, NORMAL_LIGHTLEVEL_DAY + 1 );

	// Dusk; loop from light 5 up to 12
	for (uiLoop = 1; uiLoop < 8; uiLoop++)
	{
		AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, DUSK_START + 15 * uiLoop, NORMAL_LIGHTLEVEL_DAY + 1 + uiLoop );
	}
*/

	//Transition from day to night
	for( uiLoop = 0; uiLoop < 9; ++uiLoop )
	{
		AddEveryDayStrategicEvent( EVENT_CHANGELIGHTVAL, DUSK_START + 2 * uiLoop,	NORMAL_LIGHTLEVEL_DAY + 1 + uiLoop );
	}

	//Set up the scheduling for turning lights on and off based on the various types.
	uiHour = NIGHT_TIME_LIGHT_START_HOUR == 24 ? 0 : NIGHT_TIME_LIGHT_START_HOUR;
	AddEveryDayStrategicEvent( EVENT_TURN_ON_NIGHT_LIGHTS, uiHour * 60, 0 );
	uiHour = NIGHT_TIME_LIGHT_END_HOUR == 24 ? 0 : NIGHT_TIME_LIGHT_END_HOUR;
	AddEveryDayStrategicEvent( EVENT_TURN_OFF_NIGHT_LIGHTS, uiHour * 60, 0 );
	uiHour = PRIME_TIME_LIGHT_START_HOUR == 24 ? 0 : PRIME_TIME_LIGHT_START_HOUR;
	AddEveryDayStrategicEvent( EVENT_TURN_ON_PRIME_LIGHTS, uiHour * 60, 0 );
	uiHour = PRIME_TIME_LIGHT_END_HOUR == 24 ? 0 : PRIME_TIME_LIGHT_END_HOUR;
	AddEveryDayStrategicEvent( EVENT_TURN_OFF_PRIME_LIGHTS, uiHour * 60, 0 );
}

void BuildDayAmbientSounds( )
{
	// Add events!
	for ( INT32 cnt = 0; cnt < gsNumAmbData; ++cnt )
	{
		switch( gAmbData[ cnt ].ubTimeCatagory )
		{
			case AMB_TOD_DAWN:
				AddSameDayRangedStrategicEvent( EVENT_AMBIENT, DAWN_START, DAWN_TO_DAY, cnt );
				break;
			case AMB_TOD_DAY:
				AddSameDayRangedStrategicEvent( EVENT_AMBIENT, DAY_START, DAY_TO_DUSK, cnt );
				break;
			case AMB_TOD_DUSK:
				AddSameDayRangedStrategicEvent( EVENT_AMBIENT, DUSK_START, DUSK_TO_NIGHT, cnt );
				break;
			case AMB_TOD_NIGHT:
				AddSameDayRangedStrategicEvent( EVENT_AMBIENT, NIGHT_START, NIGHT_TO_DAWN, cnt );
				break;
		}
	}

	//guiRainLoop = NO_SAMPLE;

	//rain
	if (gGameSettings.fOptions[ TOPTION_RAIN_SOUND ] == TRUE)
	{
		if ( guiRainLoop != NO_SAMPLE )
		{
			SoundStop( guiRainLoop );
			guiRainLoop = NO_SAMPLE;
		}
	}
	//end rain
}

// Flugente: weather can now be localized, meaning that different sectors can have different types of weather at the same time
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// we use this structure to set up weather. For a given weather type, we put the chance of the weather happening in here, and later note whether a sector has been checked, and if weather does occur
struct WeatherHelperStruct
{
	WeatherHelperStruct( ) : weatherhappens( FALSE ), handled( FALSE ), chance( 0 ) {}

	BOOLEAN weatherhappens;
	BOOLEAN handled;
	UINT8 chance;
};

void DetermineLocalizedWeatherGivenHelperArray( WeatherHelperStruct helperstruct[256] )
{
	UINT32 totalweatherpoints = 0;
	for ( UINT8 sector = 0; sector < 255; ++sector )
	{
		totalweatherpoints += helperstruct[sector].chance;
	}

	if ( totalweatherpoints )
	{
		INT32 chosenweatherpoint = Random( totalweatherpoints );
		UINT8 chosensector = 0;

		for ( UINT8 sector = 0; sector < 255; ++sector )
		{
			chosenweatherpoint -= helperstruct[sector].chance;

			if ( chosenweatherpoint < 0 )
			{
				chosensector = sector;

				break;
			}
		}

		// we now have a starting sector. We now loop over all sectors several times. If a sector is adjacent to a sector with new weather, determine whether the weather will also happen there
		helperstruct[chosensector].weatherhappens = TRUE;
		helperstruct[chosensector].handled = TRUE;

		BOOLEAN weatherchangehappened = TRUE;

		while ( weatherchangehappened )
		{
			weatherchangehappened = FALSE;

			for ( UINT8 sector = 0; sector < 255; ++sector )
			{
				if ( !helperstruct[sector].handled )
				{
					// if it can't rain in a sector anyway, screw this
					if ( !helperstruct[sector].chance )
					{
						helperstruct[sector].handled = TRUE;
						continue;
					}

					INT8 x = SECTORX( sector );
					INT8 y = SECTORY( sector );

					// check adjacent sectors;
					INT16 adjacentsectors[4] = {-1, -1, -1, -1};

					if ( x > 0 )	adjacentsectors[0] = SECTOR( x - 1, y );
					if ( x < 15 )	adjacentsectors[1] = SECTOR( x + 1, y );
					if ( y > 0 )	adjacentsectors[2] = SECTOR( x, y - 1 );
					if ( y < 15 )	adjacentsectors[3] = SECTOR( x, y + 1 );

					for ( int i = 0; i < 4; ++i )
					{
						if ( adjacentsectors[i] > 0 )
						{
							UINT8 adjacentsector = (UINT8)(adjacentsectors[i]);

							// if weather happens in the adjacent sector, determine whether it also happens here
							if ( helperstruct[adjacentsector].handled && helperstruct[adjacentsector].weatherhappens )
							{
								helperstruct[sector].weatherhappens = Chance( helperstruct[sector].chance );
								helperstruct[sector].handled = TRUE;

								// if weather spreads, we will continue with this function
								if ( helperstruct[sector].weatherhappens )
								{
									weatherchangehappened = TRUE;
									continue;
								}
							}
						}
					}
				}
			}
		}

		// loop over all sectors again. If a sector doesn't have the new weather, but most or all adjacent sectors do, consider adding it here too
		for ( UINT8 sector = 0; sector < 255; ++sector )
		{
			if ( !helperstruct[sector].weatherhappens && helperstruct[sector].chance )
			{
				INT8 x = SECTORX( sector );
				INT8 y = SECTORY( sector );

				// check adjacent sectors;
				INT16 adjacentsectors[4] = {-1, -1, -1, -1};

				if ( x > 0 )	adjacentsectors[0] = SECTOR( x - 1, y );
				if ( x < 15 )	adjacentsectors[1] = SECTOR( x + 1, y );
				if ( y > 0 )	adjacentsectors[2] = SECTOR( x, y - 1 );
				if ( y < 15 )	adjacentsectors[3] = SECTOR( x, y + 1 );

				UINT8 adjacentweathercount = 0;
				for ( int i = 0; i < 4; ++i )
				{
					if ( adjacentsectors[i] > 0 )
					{
						UINT8 adjacentsector = (UINT8)(adjacentsectors[i]);

						if ( helperstruct[adjacentsector].weatherhappens )
						{
							++adjacentweathercount;
						}
					}
				}

				// no weather 'holes'
				if ( adjacentweathercount >= 4 )
				{
					helperstruct[sector].weatherhappens = TRUE;
				}
				// if weather is on 3 sides, at least consider awarding it
				else if ( adjacentweathercount == 3 && Chance( 66 ) )
				{
					helperstruct[sector].weatherhappens = TRUE;
				}
			}
		}
	}
}

void ForecastDayEvents( )
{
	UINT32 uiOldDay;
	UINT32 uiStartTime, uiEndTime;

	// Get current day and see if different
	if ( ( uiOldDay = GetWorldDay() ) != guiEnvDay )
	{
		// It's a new day, forecast weather
		guiEnvDay = uiOldDay;

		// Set light level changes
		//BuildDayLightLevels();

		// Build ambient sound queues
		BuildDayAmbientSounds( );

		// ATE: Don't forecast if start of game...
		if ( guiEnvDay > 1 )
		{
			//rain
			if ( gGameExternalOptions.gfAllowRain )
			{
				for ( int i = 0; i < gGameExternalOptions.gusWeatherPerDayRain; ++i )
				{
					if ( Chance( gGameExternalOptions.gusRainChancePerDay ) )
					{
						uiStartTime = GetWorldTotalMin( ) + (UINT32)(Random( 1440 - 1 - gGameExternalOptions.gusRainMaxLength ));

						UINT32 length = gGameExternalOptions.gusRainMinLength + Random( max(1, gGameExternalOptions.gusRainMaxLength - gGameExternalOptions.gusRainMinLength ) );

						uiEndTime = uiStartTime + length;

						// We determine the 'start point' of the weather via xml chances. Weather can then spread to adjacent sectors according to chances
						WeatherHelperStruct helperstruct[256];

						for ( UINT8 sector = 0; sector < 255; ++sector )
							helperstruct[sector].chance = SectorExternalData[sector][0].rainchance;

						DetermineLocalizedWeatherGivenHelperArray( helperstruct );

						// now that we know in which sectors the new weather will happen, set up timed events
						BOOLEAN dothunderstorm = (gGameExternalOptions.gfAllowLightning && Chance( 50 ));

						for ( UINT8 sector = 0; sector < 255; ++sector )
						{
							if ( helperstruct[sector].weatherhappens )
							{
								AddStrategicEvent( dothunderstorm ? EVENT_WEATHER_THUNDERSHOWERS : EVENT_WEATHER_SHOWERS, uiStartTime, sector );
								AddStrategicEvent( EVENT_WEATHER_NORMAL, uiEndTime, sector );
							}
						}
					}
				}
			}

			// sandstorm
			if ( gGameExternalOptions.gfAllowSandStorms )
			{
				for ( int i = 0; i < gGameExternalOptions.gusWeatherPerDaySandstorm; ++i )
				{
					if ( Chance( gGameExternalOptions.gusSandStormsChancePerDay ) )
					{
						uiStartTime = GetWorldTotalMin( ) + (UINT32)(Random( 1440 - 1 - gGameExternalOptions.gusSandStormsMaxLength ));
						
						UINT32 length = gGameExternalOptions.gusSandStormsMinLength + Random( max( 1, gGameExternalOptions.gusSandStormsMaxLength - gGameExternalOptions.gusSandStormsMinLength ) );

						uiEndTime = uiStartTime + length;

						// We determine the 'start point' of the weather via xml chances. Weather can then spread to adjacent sectors according to chances
						WeatherHelperStruct helperstruct[256];

						for ( UINT8 sector = 0; sector < 255; ++sector )
							helperstruct[sector].chance = SectorExternalData[sector][0].sandstormchance;

						DetermineLocalizedWeatherGivenHelperArray( helperstruct );
				
						for ( UINT8 sector = 0; sector < 255; ++sector )
						{
							if ( helperstruct[sector].weatherhappens )
							{
								AddStrategicEvent( EVENT_WEATHER_SANDSTORM, uiStartTime, sector );
								AddStrategicEvent( EVENT_WEATHER_NORMAL, uiEndTime, sector );
							}
						}
					}
				}
			}

			// snow
			if ( gGameExternalOptions.gfAllowSnow && Chance( gGameExternalOptions.gusSnowChancePerDay ) )
			{
				for ( int i = 0; i < gGameExternalOptions.gusWeatherPerDaySnow; ++i )
				{
					if ( Chance( gGameExternalOptions.gusSnowChancePerDay ) )
					{
						uiStartTime = GetWorldTotalMin( ) + (UINT32)(Random( 1440 - 1 - gGameExternalOptions.gusSnowMaxLength ));
						
						UINT32 length = gGameExternalOptions.gusSnowMinLength + Random( max( 1, gGameExternalOptions.gusSnowMaxLength - gGameExternalOptions.gusSnowMinLength ) );

						uiEndTime = uiStartTime + length;

						// We determine the 'start point' of the weather via xml chances. Weather can then spread to adjacent sectors according to chances
						WeatherHelperStruct helperstruct[256];

						for ( UINT8 sector = 0; sector < 255; ++sector )
							helperstruct[sector].chance = SectorExternalData[sector][0].snowchance;

						DetermineLocalizedWeatherGivenHelperArray( helperstruct );

						for ( UINT8 sector = 0; sector < 255; ++sector )
						{
							if ( helperstruct[sector].weatherhappens )
							{
								AddStrategicEvent( EVENT_WEATHER_SNOW, uiStartTime, sector );
								AddStrategicEvent( EVENT_WEATHER_NORMAL, uiEndTime, sector );
							}
						}
					}
				}
			}
		}
	}
}

void EnvEnableTOD(void)
{
	fTimeOfDayControls=TRUE;
}

void EnvDisableTOD(void)
{
	fTimeOfDayControls=FALSE;
}



//rain
UINT32 guiMinLightningInterval = 5;
UINT32 guiMaxLightningInterval = 15;

UINT32 guiMinDLInterval = 1;
UINT32 guiMaxDLInterval = 5;

UINT32 guiProlongLightningIfSeenSomeone = 5;
UINT32 guiChanceToDoLightningBetweenTurns = 20;


 // 60 = 1 second
#define MIN_LIGHTNING_INTERVAL ( 60 * guiMinLightningInterval )
#define MAX_LIGHTNING_INTERVAL ( 60 * guiMaxLightningInterval )

#define MAX_DELAYED_SOUNDS 10
#define NO_DL_SOUND 0xFFFFFFFF

// 1000 = 1 second
#define MIN_DL_INTERVAL ( 1000 * guiMinDLInterval )
#define MAX_DL_INTERVAL ( 1000 * guiMaxDLInterval )

#define EXTRA_ADD_VIS_DIST_IF_SEEN_SOMEONE ( 1000 * guiProlongLightningIfSeenSomeone	)
#define CHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS guiChanceToDoLightningBetweenTurns

BOOLEAN gfLightningInProgress = FALSE;
BOOLEAN gfHaveSeenSomeone = FALSE;

UINT8 ubRealAmbientLightLevel = 0;
BOOLEAN gfTurnBasedDoLightning = FALSE;
BOOLEAN gfTurnBasedLightningEnd = FALSE;

BOOLEAN gfWasTurnBasedWhenLightningBegin = FALSE;

UINT8 gubLastTeamLightning;

#define IS_TURNBASED ( gTacticalStatus.uiFlags & TURNBASED &&	gTacticalStatus.uiFlags & INCOMBAT )

void BeginTeamTurn( UINT8 ubTeam );


void EnvDoLightning(void)
{
	static UINT32 uiCount=10, uiIndex=0, uiStrike=0, uiFrameNext=0;
	static UINT8 ubLevel=0, ubLastLevel=0;
	static UINT32 uiLastUpdate = 0xFFFFFFFF;
	static UINT32 uiTurnOffExtraVisDist = 0xFFFFFFFF;
	static UINT32 pDelayedSounds[ MAX_DELAYED_SOUNDS ];
	UINT32 uiDSIndex;

	if( GetJA2Clock() < uiLastUpdate )
	{
		uiLastUpdate = 0;
		memset( pDelayedSounds, NO_DL_SOUND, sizeof( UINT32 ) * MAX_DELAYED_SOUNDS );
	}

	if( GetJA2Clock() < uiLastUpdate + 1000 / 60 )
		return;
	else
		uiLastUpdate = GetJA2Clock();

	if( GetJA2Clock() > uiTurnOffExtraVisDist )
	{
		AllTeamsLookForAll( FALSE );
		uiTurnOffExtraVisDist = 0xFFFFFFFF;

		if( gfTurnBasedLightningEnd )
		{
			BeginTeamTurn( gubLastTeamLightning );
			gfTurnBasedLightningEnd = FALSE;
		}
	}

	for ( uiDSIndex = 0; uiDSIndex < MAX_DELAYED_SOUNDS; ++uiDSIndex )
	{
		if( GetJA2Clock() > pDelayedSounds[ uiDSIndex ] )
		{
			PlayJA2Ambient(LIGHTNING_1+Random(2), HIGHVOLUME, 1);
			pDelayedSounds[ uiDSIndex ] = NO_DL_SOUND;
		}
	}

	if ( gfPauseDueToPlayerGamePause )
	{
		return;
	}

	if( IS_TURNBASED && !gfLightningInProgress )
	{
		if( !gfTurnBasedDoLightning )
		{
			return;
		}
		else
		{
			gfTurnBasedDoLightning = FALSE;
			uiFrameNext = 1;
			uiCount = 0;
			gfTurnBasedLightningEnd = TRUE;
		}
	}

	++uiCount;

	if(uiCount >= (uiFrameNext+10))
	{
		if( gfHaveSeenSomeone && gfWasTurnBasedWhenLightningBegin	)
			uiTurnOffExtraVisDist = GetJA2Clock() + EXTRA_ADD_VIS_DIST_IF_SEEN_SOMEONE;
		else
			uiTurnOffExtraVisDist = GetJA2Clock();

		gfLightningInProgress = FALSE;
		gfHaveSeenSomeone = FALSE;

		uiCount=0;
		uiIndex=0;
		ubLevel=0;
		ubLastLevel=0;

		uiStrike=Random(3);
		uiFrameNext=MIN_LIGHTNING_INTERVAL+Random(MAX_LIGHTNING_INTERVAL - MIN_LIGHTNING_INTERVAL);
	}
	else if(uiCount >= uiFrameNext)
	{
		if(uiCount == uiFrameNext)
		{
			//EnvStopCrickets();
//			PlayJA2Ambient(LIGHTNING_1+Random(2), HIGHVOLUME, 1);
			for( uiDSIndex = 0; uiDSIndex < MAX_DELAYED_SOUNDS; ++uiDSIndex )
				if( pDelayedSounds[ uiDSIndex ] == NO_DL_SOUND )
				{
					pDelayedSounds[ uiDSIndex ] = GetJA2Clock() + MIN_DL_INTERVAL+Random(MAX_DL_INTERVAL - MIN_DL_INTERVAL);
					break;
				}

			ubRealAmbientLightLevel = ubAmbientLightLevel;

			gfWasTurnBasedWhenLightningBegin = IS_TURNBASED;

			gfLightningInProgress = TRUE;
			AllTeamsLookForAll( FALSE );
		}

		while(uiCount > ((UINT32)ubLightningTable[uiStrike][uiIndex][0] + uiFrameNext))
			++uiIndex;

		ubLastLevel=ubLevel;
		ubLevel=min( ubRealAmbientLightLevel - 1, ubLightningTable[uiStrike][uiIndex][1] );

/*		// ATE: Don't modify if scrolling!
		if ( gfScrollPending || gfScrollInertia )
		{
		}
		else*/
		{
 			if(ubLastLevel!=ubLevel)
			{
				if(ubLevel > ubLastLevel)
				{
					LightAddBaseLevel(0, (UINT8)(ubLevel-ubLastLevel));
					if(ubLevel > 0)
						RenderSetShadows(TRUE);
				}
				else
				{
	//				LightSubtractBaseLevel(0, (UINT8)(ubLastLevel-ubLevel));
					LightSetBaseLevel( ubRealAmbientLightLevel - ubLevel );
					if(ubLevel > 0)
						RenderSetShadows(TRUE);
				}

				SetRenderFlags(RENDER_FLAG_FULL);
			}
		}
	}
}

BOOLEAN LightningEndOfTurn( UINT8 ubTeam )
{
	if ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather != WEATHER_FORECAST_THUNDERSHOWERS )
		return TRUE;

	if ( Random(100) >= CHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS )
		return TRUE;

	if( !gfTurnBasedLightningEnd )
	{
		gfTurnBasedDoLightning = TRUE;
		gubLastTeamLightning = ubTeam;
		EnvDoLightning();
		return FALSE;
	}
	
	return TRUE;
}

UINT8 GetTimeOfDayAmbientLightLevel()
{
	if (!is_networked)
	{
		if ( SectorTemperature( GetWorldMinutesInDay(), gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == HOT )
		{
			return( HOT_DAY_LIGHTLEVEL );
		}
		else
		{
			return( gubEnvLightValue );
		}
	}
	else
	{
		return ( gubEnvLightValue );
	}
}

void	ChangeWeather( UINT8 aSector, UINT8 aType )
{
	if ( aType >= WEATHER_FORECAST_MAX )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Error in ChangeWeather(): Invalid weather" );
		return;
	}

	if ( SectorInfo[aSector].usWeather != aType )
	{
		if ( SECTOR( gWorldSectorX, gWorldSectorY ) == aSector && !gbWorldSectorZ )
		{
			if ( !gfBasement && !gfCaves )
				gfDoLighting = TRUE;

			CHAR16 pStrSectorName[128];
			GetSectorIDString( SECTORX( aSector ), SECTORY( aSector ), 0, pStrSectorName, TRUE );

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: Changed weather from %s to %s", pStrSectorName, szWeatherTypeText[SectorInfo[aSector].usWeather], szWeatherTypeText[aType] );
		}

		// Flugente: additional dialogue
		AdditionalTacticalCharacterDialogue_AllInSector( SECTORX( aSector ), SECTORY( aSector ), 0, NO_PROFILE, ADE_WEATHERCHANGE, aType );

		// First turn off whatever weather it is, then turn on the requested weather
		SectorInfo[aSector].usWeather = aType;
	}
}

UINT8	GetWeatherInCurrentSector()
{
	// no weather underground!
	if ( gbWorldSectorZ )
		return WEATHER_FORECAST_NORMAL;
	
	return SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather;
}

//end rain





/*
void EnvDoLightning(void)
{
static UINT32 uiCount=0, uiIndex=0, uiStrike=0, uiFrameNext=1000;
static UINT8 ubLevel=0, ubLastLevel=0;

	if ( gfPauseDueToPlayerGamePause )
	{
	return;
	}

	uiCount++;
	if(uiCount >= (uiFrameNext+10))
	{
		uiCount=0;
		uiIndex=0;
		ubLevel=0;
		ubLastLevel=0;

		uiStrike=Random(3);
		uiFrameNext=1000+Random(1000);
	}
	else if(uiCount >= uiFrameNext)
	{
		if(uiCount == uiFrameNext)
		{
			//EnvStopCrickets();
			PlayJA2Ambient(LIGHTNING_1+Random(2), HIGHVOLUME, 1);
		}

		while(uiCount > ((UINT32)ubLightningTable[uiStrike][uiIndex][0] + uiFrameNext))
			uiIndex++;

		ubLastLevel=ubLevel;
		ubLevel=ubLightningTable[uiStrike][uiIndex][1];

	// ATE: Don't modify if scrolling!
	if ( gfScrollPending || gfScrollInertia )
	{
	}
	else
	{
 		if(ubLastLevel!=ubLevel)
		{
			if(ubLevel > ubLastLevel)
			{
				LightAddBaseLevel(0, (UINT8)(ubLevel-ubLastLevel));
				if(ubLevel > 0)
					RenderSetShadows(TRUE);
			}
			else
			{
				LightSubtractBaseLevel(0, (UINT8)(ubLastLevel-ubLevel));
				if(ubLevel > 0)
					RenderSetShadows(TRUE);
			}
			SetRenderFlags(RENDER_FLAG_FULL);
		}
	}
	}
}


UINT8 GetTimeOfDayAmbientLightLevel()
{
	if ( SectorTemperature( GetWorldMinutesInDay(), gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == HOT )
	{
		return( HOT_DAY_LIGHTLEVEL );
	}
	else
	{
		return( gubEnvLightValue );
	}
}
*/


void TurnOnNightLights()
{
	for ( INT32 i = 0; i < MAX_LIGHT_SPRITES; ++i )
	{
		if( LightSprites[ i ].uiFlags & LIGHT_SPR_ACTIVE &&
			LightSprites[ i ].uiFlags & LIGHT_NIGHTTIME &&
				!(LightSprites[ i ].uiFlags & (LIGHT_SPR_ON|MERC_LIGHT) ) )
		{
			LightSpritePower( i, TRUE );
		}
	}
}

void TurnOffNightLights()
{
	for ( INT32 i = 0; i < MAX_LIGHT_SPRITES; ++i )
	{
		if( LightSprites[ i ].uiFlags & LIGHT_SPR_ACTIVE &&
			LightSprites[ i ].uiFlags & LIGHT_NIGHTTIME &&
				LightSprites[ i ].uiFlags & LIGHT_SPR_ON &&
				!(LightSprites[ i ].uiFlags & MERC_LIGHT) )
		{
			LightSpritePower( i, FALSE );
		}
	}
}

void TurnOnPrimeLights()
{
	for ( INT32 i = 0; i < MAX_LIGHT_SPRITES; ++i )
	{
		if( LightSprites[ i ].uiFlags & LIGHT_SPR_ACTIVE &&
			LightSprites[ i ].uiFlags & LIGHT_PRIMETIME &&
				!(LightSprites[ i ].uiFlags & (LIGHT_SPR_ON|MERC_LIGHT) ) )
		{
			LightSpritePower( i, TRUE );
		}
	}
}

void TurnOffPrimeLights()
{
	for ( INT32 i = 0; i < MAX_LIGHT_SPRITES; ++i )
	{
		if( LightSprites[ i ].uiFlags & LIGHT_SPR_ACTIVE &&
			LightSprites[ i ].uiFlags & LIGHT_PRIMETIME &&
				LightSprites[ i ].uiFlags & LIGHT_SPR_ON &&
				!(LightSprites[ i ].uiFlags & MERC_LIGHT) )
		{
			LightSpritePower( i, FALSE );
		}
	}
}

void UpdateTemperature( UINT8 ubTemperatureCode )
{
	switch( ubTemperatureCode )
	{
		case TEMPERATURE_DESERT_COOL:
			gubDesertTemperature = 0;
			break;
		case TEMPERATURE_DESERT_WARM:
			gubDesertTemperature = 1;
			break;
		case TEMPERATURE_DESERT_HOT:
			gubDesertTemperature = 2;
			break;
		case TEMPERATURE_GLOBAL_COOL:
			gubGlobalTemperature = 0;
			break;
		case TEMPERATURE_GLOBAL_WARM:
			gubGlobalTemperature = 1;
			break;
		case TEMPERATURE_GLOBAL_HOT:
			gubGlobalTemperature = 2;
			break;
	}
	gfDoLighting = TRUE;
}

INT8 SectorTemperature( UINT32 uiTime, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	// SANDRO - updated this code
	if (bSectorZ > 0)
	{
		// cool underground
		return( 0 );
	}
	else if ( IsSectorDesert( sSectorX, sSectorY ) && !(SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_RAIN || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS) ) // is desert
	{
		return( gubDesertTemperature );
	}
	else
	{
		return( gubGlobalTemperature );
	}
}


// Flugente: fun with snakes
void AddSnakeAmim( INT32 sGridno, UINT8 usDirection )
{
	if ( !TileIsOutOfBounds( sGridno ) )
	{
		ANITILE_PARAMS	AniParams;

		memset( &AniParams, 0, sizeof(ANITILE_PARAMS) );
		
		AniParams.sGridNo = sGridno;
		AniParams.ubLevelID = ANI_OBJECT_LEVEL;
		AniParams.sDelay = 100;
		AniParams.sStartFrame = 0;
		AniParams.uiFlags = ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_USE_DIRECTION_FOR_START_FRAME;//| ANITILE_LOOPING;
		AniParams.sX = CenterX( sGridno );
		AniParams.sY = CenterY( sGridno );
		AniParams.sZ = 0;
		strcpy( AniParams.zCachedFile, "TILECACHE\\WATERSNAKE_MOVE.sti" );

		AniParams.uiUserData3 = gOneCCDirection[usDirection];

		CreateAnimationTile( &AniParams );
	}
}

// Flugente: the environment can interact with soldiers
void HandleEnvironmentHazard( )
{
	UINT8 sector = (UINT8)SECTOR( gWorldSectorX, gWorldSectorY );
	UINT8 ubTraverseType = SectorInfo[sector].ubTraversability[THROUGH_STRATEGIC_MOVE];

	if ( gGameExternalOptions.gfAllowSnakes && !gbWorldSectorZ && Chance( SectorExternalData[sector][0].snakechance ) )
	{
		// the number of snake attacks is randomized
		int actionstodo = 1 + Random( SectorExternalData[sector][0].numsnakes );

		BOOLEAN soundplayed = FALSE;

		SOLDIERTYPE *pSoldier = NULL;
		UINT32 uiCnt = 0;
		for ( uiCnt = 0, pSoldier = MercPtrs[uiCnt]; uiCnt < TOTAL_SOLDIERS; ++uiCnt, ++pSoldier )
		{
			if ( pSoldier->bActive && !pSoldier->bSectorZ && pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->stats.bLife > 0 )
			{
				if ( TERRAIN_IS_WATER( pSoldier->bOverTerrainType ) && pSoldier->pathing.bLevel <= 0 )
				{
					// there is a chance to be attacked, it is increased if we have open wounds
					UINT16 chancetobeattacked = 20 + pSoldier->bBleeding + 20 * TERRAIN_IS_DEEP_WATER( pSoldier->bOverTerrainType );

					if ( Chance( chancetobeattacked ) )
					{
						// there is a chance we defeat the snake and do not take damage, but get a bit of exp
						if ( Chance( pSoldier->GetWaterSnakeDefenseChance( ) ) )
						{
							StatChange( pSoldier, EXPERAMT, 10, FROM_SUCCESS );

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szSnakeText[0], pSoldier->GetName( ) );
						}
						// otherwise, the snake damages us
						else
						{
							INT16 damage = 10 + Random( 11 );

							pSoldier->EVENT_SoldierGotHit(47, damage, damage * 100, pSoldier->ubDirection, 0, NOBODY, FIRE_WEAPON_NO_SPECIAL, AIM_SHOT_LEGS, 0, pSoldier->sGridNo);

							// if this is a swamp, handle possible extra infection
							if ( ubTraverseType == SWAMP || ubTraverseType == SWAMP_ROAD )
							{
								HandlePossibleInfection( pSoldier, NULL, INFECTION_TYPE_SWAMP );
							}

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szSnakeText[1], pSoldier->GetName( ) );
						}

						// in any case, play animation and lose APs
						DeductPoints( pSoldier, 20, 200, DISABLED_INTERRUPT );

						AddSnakeAmim( pSoldier->sGridNo, Random( NUM_WORLD_DIRECTIONS ) );

						if ( !soundplayed )
						{
							PlayJA2SampleFromFile( Chance( 50 ) ? "Sounds\\WATERSNAKE_ATTACK_01.WAV" : "Sounds\\WATERSNAKE_ATTACK_02.WAV", RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

							soundplayed = TRUE;
						}

						--actionstodo;
					}

					if ( actionstodo <= 0 )
						break;
				}
			}
		}

		// if there are actions left, just have the snakes travel ominously in random locations
		int cnt = 0;
		while ( actionstodo > 0 && cnt < 1000 )
		{
			INT32 sGridNo = RandomGridNo( );

			if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
			{
				AddSnakeAmim( sGridNo, Random( NUM_WORLD_DIRECTIONS ) );

				--actionstodo;
			}

			++cnt;
		}

		if ( !actionstodo && !soundplayed )
		{
			PlayJA2SampleFromFile( Chance( 50 ) ? "Sounds\\WATERSNAKE_MOVE_01.WAV" : "Sounds\\WATERSNAKE_MOVE_02.WAV", RATE_11025, LOWVOLUME, 1, MIDDLEPAN );
		}
	}
}
