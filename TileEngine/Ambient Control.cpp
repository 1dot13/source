#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "stdio.h"
	#include "sgp.h"
	#include "Ambient types.h"
	#include "fileman.h"
	#include "environment.h"
	#include "Sound Control.h"
	#include "Game Events.h"
	#include "Ambient Control.h"
	#include "lighting.h"
	#include "Random.h"	
	#include "SmokeEffects.h"	// sevenfm
#endif

AMBIENTDATA_STRUCT		gAmbData[ MAX_AMBIENT_SOUNDS ];
INT16					gsNumAmbData = 0;

// sevenfm
UINT32					guiAmbientFire = NO_SAMPLE;
UINT8					gubAmbientFutureFireVolume = 0;
UINT32					guiFireAmbientLastUpdate = 0;

UINT8					gubCurrentSteadyStateAmbience = SSA_NONE;
UINT8					gubCurrentSteadyStateSound	= 0;
UINT32					guiCurrentSteadyStateSoundHandle = NO_SAMPLE;
STEADY_STATE_AMBIENCE	gSteadyStateAmbientTable[ NUM_STEADY_STATE_AMBIENCES ] =
{
	// NONE
	"",
	"",
	"",
	"",
	// NIGHT
	"",
	"",
	"",
	"",
	// COUNTRYSIZE
	// DAY
	"SOUNDS\\SSA\\insects Day 01.wav",
	"",
	"",
	"",
	// NIGHT
	"SOUNDS\\SSA\\night_crickets_01D.wav",
	"SOUNDS\\SSA\\night_crickets_01B.wav",
	"SOUNDS\\SSA\\night_crickets_01C.wav",
	"SOUNDS\\SSA\\night_crickets_01A.wav",
	// NEAR WATER
	// DAY
	"SOUNDS\\SSA\\swamp_day_01a.wav",
	"SOUNDS\\SSA\\swamp_day_01b.wav",
	"SOUNDS\\SSA\\swamp_day_01c.wav",
	"SOUNDS\\SSA\\swamp_day_01d.wav",
	//NIGHT
	"SOUNDS\\SSA\\marsh_at_night_01a.wav",
	"SOUNDS\\SSA\\marsh_at_night_01b.wav",
	"SOUNDS\\SSA\\marsh_at_night_01c.wav",
	"SOUNDS\\SSA\\marsh_at_night_01d.wav",
	//INWATER
	//DAY
	"SOUNDS\\SSA\\middle_of_water_01d.wav",
	"SOUNDS\\SSA\\middle_of_water_01c.wav",
	"SOUNDS\\SSA\\middle_of_water_01b.wav",
	"SOUNDS\\SSA\\middle_of_water_01a.wav",
	// night
	"SOUNDS\\SSA\\middle_of_water_01d.wav",
	"SOUNDS\\SSA\\middle_of_water_01c.wav",
	"SOUNDS\\SSA\\middle_of_water_01b.wav",
	"SOUNDS\\SSA\\middle_of_water_01a.wav",
	// HEAVY FOREST
	// day
	"SOUNDS\\SSA\\JUNGLE_DAY_01a.wav",
	"SOUNDS\\SSA\\JUNGLE_DAY_01b.wav",
	"SOUNDS\\SSA\\JUNGLE_DAY_01c.wav",
	"SOUNDS\\SSA\\JUNGLE_DAY_01d.wav",
	// night
	"SOUNDS\\SSA\\night_crickets_03a.wav",
	"SOUNDS\\SSA\\night_crickets_03b.wav",
	"SOUNDS\\SSA\\night_crickets_03c.wav",
	"SOUNDS\\SSA\\night_crickets_03d.wav",
	// PINE FOREST
	// DAY
	"SOUNDS\\SSA\\pine_forest_01a.wav",
	"SOUNDS\\SSA\\pine_forest_01b.wav",
	"SOUNDS\\SSA\\pine_forest_01c.wav",
	"SOUNDS\\SSA\\pine_forest_01d.wav",
	// NIGHT
	"SOUNDS\\SSA\\night_crickets_02a.wav",
	"SOUNDS\\SSA\\night_crickets_02b.wav",
	"SOUNDS\\SSA\\night_crickets_02c.wav",
	"SOUNDS\\SSA\\night_crickets_02d.wav",
	// ABANDANDED
	// DAY
	"SOUNDS\\SSA\\metal_wind_01a.wav",
	"SOUNDS\\SSA\\metal_wind_01b.wav",
	"SOUNDS\\SSA\\metal_wind_01c.wav",
	"SOUNDS\\SSA\\metal_wind_01d.wav",
	// NIGHT
	"SOUNDS\\SSA\\night_insects_01a.wav",
	"SOUNDS\\SSA\\night_insects_01b.wav",
	"SOUNDS\\SSA\\night_insects_01c.wav",
	"SOUNDS\\SSA\\night_insects_01d.wav",
	// AIRPORT
	// DAY
	"SOUNDS\\SSA\\rotating radar dish.wav",
	"",
	"",
	"",
	// NIGHT
	"SOUNDS\\SSA\\rotating radar dish.wav",
	"",
	"",
	"",
	// WASTE LAND
	// DAY
	"SOUNDS\\SSA\\gentle_wind.wav",
	"",
	"",
	"",
	// NIGHT
	"SOUNDS\\SSA\\insects_at_night_04.wav",
	"",
	"",
	"",
	// UNDERGROUND
	// DAY
	"SOUNDS\\SSA\\low ominous ambience.wav",
	"",
	"",
	"",
	// NIGHT
	"SOUNDS\\SSA\\low ominous ambience.wav",
	"",
	"",
	"",
	// OCEAN
	// DAY
	"SOUNDS\\SSA\\sea_01a.wav",
	"SOUNDS\\SSA\\sea_01b.wav",
	"SOUNDS\\SSA\\sea_01c.wav",
	"SOUNDS\\SSA\\sea_01d.wav",
	// NIGHT
	"SOUNDS\\SSA\\ocean_waves_01a.wav",
	"SOUNDS\\SSA\\ocean_waves_01b.wav",
	"SOUNDS\\SSA\\ocean_waves_01c.wav",
	"SOUNDS\\SSA\\ocean_waves_01d.wav",
};


BOOLEAN LoadAmbientControlFile( UINT8 ubAmbientID )
{
	SGPFILENAME						zFilename;
	HWFILE								hFile;
	INT32								cnt;



	// BUILD FILENAME
	sprintf( zFilename, "AMBIENT\\%d.bad", ubAmbientID );

	// OPEN, LOAD
	hFile = FileOpen( zFilename, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		return( FALSE );
	}

	// READ #
	if( !FileRead( hFile, &gsNumAmbData, sizeof( INT16 ), NULL ) )
	{
		return( FALSE );
	}

	// LOOP FOR OTHERS
	for ( cnt = 0; cnt < gsNumAmbData; cnt++ )
	{
		if( !FileRead( hFile, &(gAmbData[ cnt ]), sizeof( AMBIENTDATA_STRUCT ), NULL ) )
		{
			return( FALSE );
		}

		sprintf( zFilename, "AMBIENT\\%s", gAmbData[ cnt ].zFilename );
		strcpy( gAmbData[ cnt ].zFilename, zFilename );
	}

	FileClose( hFile );

	return( TRUE );
}

void GetAmbientDataPtr( AMBIENTDATA_STRUCT **ppAmbData, UINT16 *pusNumData )
{
	*ppAmbData		= gAmbData;
	*pusNumData		= gsNumAmbData;
}


void StopAmbients( )
{
	SoundStopAllRandom( );
}

void HandleNewSectorAmbience( UINT8 ubAmbientID )
{
	// OK, we could have just loaded a sector, erase all ambient sounds from queue, shutdown all ambient groupings
	SoundStopAllRandom( );

	DeleteAllStrategicEventsOfType( EVENT_AMBIENT );

	if( !gfBasement && !gfCaves )
	{
		if(	LoadAmbientControlFile( ubAmbientID ) )
		{
			// OK, load them up!
			BuildDayAmbientSounds( );
		}
		else
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_0, String("Cannot load Ambient data for tileset" ) );
		}
	}
}

void DeleteAllAmbients()
{
	// JA2Gold: it seems that ambient sounds don't get unloaded when we exit a sector!?
	SoundStopAllRandom();
	DeleteAllStrategicEventsOfType( EVENT_AMBIENT );
}

UINT32 SetupNewAmbientSound( UINT32 uiAmbientID )
{
	RANDOMPARMS rpParms;

	//SoundLog((CHAR8 *)String("	SetupNewAmbientSound()1:	uiAmbientID: '%d'", uiAmbientID ) );

	memset(&rpParms, 0xff, sizeof(RANDOMPARMS));

	rpParms.uiTimeMin		=	gAmbData[ uiAmbientID ].uiMinTime;
	rpParms.uiTimeMax		=	gAmbData[ uiAmbientID ].uiMaxTime;
	rpParms.uiVolMin		= CalculateSoundEffectsVolume( gAmbData[ uiAmbientID ].uiVol );
	rpParms.uiVolMax		= CalculateSoundEffectsVolume( gAmbData[ uiAmbientID ].uiVol );
	rpParms.uiPriority	=	GROUP_AMBIENT;

	//SoundLog((CHAR8 *)String("	SetupNewAmbientSound()2:	gAmbData[ uiAmbientID ].zFilename: '%s',	Params: '%s'", gAmbData[ uiAmbientID ].zFilename, &rpParms ) );

	return SoundPlayRandom( gAmbData[ uiAmbientID ].zFilename, &rpParms );	//bug Nr14
}


UINT32 StartSteadyStateAmbient( UINT32 ubVolume, UINT32 ubLoops)
{
SOUNDPARMS spParms;

	memset(&spParms, 0xff, sizeof(SOUNDPARMS));

	spParms.uiVolume = CalculateSoundEffectsVolume( ubVolume );
	spParms.uiLoop = ubLoops;
	spParms.uiPriority=GROUP_AMBIENT;

	return(SoundPlay( gSteadyStateAmbientTable[ gubCurrentSteadyStateAmbience ].zSoundNames[ gubCurrentSteadyStateSound ], &spParms ) );
}



BOOLEAN SetSteadyStateAmbience( UINT8 ubAmbience )
{
	BOOLEAN fInNight = FALSE;
	INT32	 cnt;
	UINT8	 ubNumSounds = 0;
	UINT8	 ubChosenSound;

	// Stop all ambients...
	if ( guiCurrentSteadyStateSoundHandle != NO_SAMPLE )
	{
		SoundStop( guiCurrentSteadyStateSoundHandle );
		guiCurrentSteadyStateSoundHandle = NO_SAMPLE;
	}

	// Determine what time of day we are in ( day/night)
	if( gubEnvLightValue >= LIGHT_DUSK_CUTOFF)
	{
	fInNight = TRUE;
	}

	// loop through listing to get num sounds...
	for ( cnt = ( fInNight * 4 ); cnt < ( NUM_SOUNDS_PER_TIMEFRAME / 2 ); cnt++ )
	{
	if ( gSteadyStateAmbientTable[ ubAmbience ].zSoundNames[ cnt ][ 0 ] == 0 )
	{
		break;
	}

	ubNumSounds++;
	}

	if ( ubNumSounds == 0 )
	{
	return( FALSE );
	}

	// Pick one
	ubChosenSound = (UINT8) Random( ubNumSounds );

	// Set!
	gubCurrentSteadyStateAmbience = ubAmbience;
	gubCurrentSteadyStateSound	= ubChosenSound;

	guiCurrentSteadyStateSoundHandle =	StartSteadyStateAmbient( LOWVOLUME, 0 );

	return( TRUE );
}

void UpdateFireAmbient(void)
{
	INT32	iVolume;
	UINT32	uiClock;

	if (!gGameExternalOptions.fEnableTA)
	{
		return;
	}

	if (guiCurrentScreen != GAME_SCREEN)
	{
		return;
	}

	uiClock = GetJA2Clock();

	if (uiClock - guiFireAmbientLastUpdate < 50)
	{
		return;
	}

	guiFireAmbientLastUpdate = uiClock;

	if (FindVisibleSmokeEffect(BURNABLEGAS_SMOKE_EFFECT))
	{
		gubAmbientFutureFireVolume = gGameExternalOptions.ubVolumeTA;

		if (guiAmbientFire == NO_SAMPLE)
		{
			StartFireAmbient();
		}

		// check if successfully started
		if (guiAmbientFire != NO_SAMPLE)
		{
			iVolume = SoundGetVolume(guiAmbientFire);

			if (iVolume != gubAmbientFutureFireVolume)
			{
				// increase volume until it reaches desired level
				iVolume = min((INT32)gubAmbientFutureFireVolume, iVolume + 4);
				SoundSetVolume(guiAmbientFire, iVolume);
			}
		}
	}
	else
	{
		gubAmbientFutureFireVolume = 0;

		// check if ambient sound is playing
		if (guiAmbientFire != NO_SAMPLE)
		{
			iVolume = SoundGetVolume(guiAmbientFire);

			if (iVolume != gubAmbientFutureFireVolume)
			{
				// increase volume until it reaches desired level
				iVolume = max((INT32)gubAmbientFutureFireVolume, iVolume - 4);
				SoundSetVolume(guiAmbientFire, iVolume);
			}

			if (iVolume == 0)
			{
				StopFireAmbient();
			}
		}
	}
}

void StopFireAmbient(void)
{
	if (guiAmbientFire != NO_SAMPLE)
	{
		SoundStop(guiAmbientFire);
		guiAmbientFire = NO_SAMPLE;
	}
}

void StartFireAmbient(void)
{
	CHAR8	filename[1024];
	SOUNDPARMS spParms;

	// Stop ambient
	if (guiAmbientFire != NO_SAMPLE)
	{
		return;
	}

	strcpy(filename, "Sounds\\ambient\\fire.ogg");
	//gubAmbientFireVolume = LOWVOLUME;

	// start sound
	memset(&spParms, 0xff, sizeof(SOUNDPARMS));
	//spParms.uiVolume = CalculateSoundEffectsVolume( LOWVOLUME );
	spParms.uiVolume = 0;
	spParms.uiLoop = 0;
	spParms.uiPriority = GROUP_AMBIENT;

	//guiAmbientFire = SoundPlay( zFileName, &spParms );
	guiAmbientFire = SoundPlayStreamedFile(filename, &spParms);
}
