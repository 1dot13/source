/*********************************************************************************
* SGP Digital Sound Module
*
*		This module handles the playing of digital samples, preloaded or streamed.
*
* Derek Beland, May 28, 1997
*
*********************************************************************************/
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <string.h>
	#include "soundman.h"
	#include "FileMan.h"
	#include "debug.h"
	#include "MemMan.h"
	#include "random.h"
	#include "fmod.h"
	#include "fmod_errors.h"
	#include "sgp_logger.h"
#endif

// Uncomment this to disable the startup of sound hardware
//#define SOUND_DISABLE

// global settings
#define		SOUND_MAX_CACHED		128						// number of cache slots

#ifdef JA2
#define		SOUND_MAX_CHANNELS		16						// number of mixer channels
#else
#define		SOUND_MAX_CHANNELS		32						// number of mixer channels
#endif

// default memory limit
#define		SOUND_DEFAULT_MEMORY	(8048*1024)

// size for sample to be double-buffered
#define		SOUND_DEFAULT_THRESH	(256*8024)

// playing/random value to indicate default
#define		SOUND_PARMS_DEFAULT		0xffffffff

// Sound status flags
#define		SOUND_CALLBACK			0x00000008

// Max volume
#define	 MAX_VOLUME	(127)

// Initialization parameters
#define AUDIO_BUFFER_LEN	100
#define STREAM_BUFFER_LEN	100

// Lesh modifications
// Sound debug

CHAR8 SndDebugFileName[]="sound.log";
FILE *SndDebug;

// Debug logging
void SoundLog(CHAR8 *strMessage);
void InitLogging();

// Cache system
BOOLEAN		SoundInitCache(void);
BOOLEAN		SoundShutdownCache(void);
UINT32		SoundLoadSample(STR pFilename);
UINT32		SoundFreeSample(STR pFilename);
UINT32		SoundGetCached(STR pFilename);
UINT32		SoundLoadDisk(STR pFilename);
BOOLEAN		SoundCleanCache(void);
UINT32		SoundFreeSampleIndex(UINT32 uiSample);

// Low level
// Init, de-init
BOOLEAN		SoundInitHardware(void);
BOOLEAN		SoundShutdownHardware(void);

// Playing
UINT32		SoundStartSample(UINT32 uiSample, UINT32 uiChannel, SOUNDPARMS *pParms);
UINT32		SoundStartStream(STR pFilename, UINT32 uiChannel, SOUNDPARMS *pParms);
BOOLEAN		SoundPlayStreamed(STR pFilename);
UINT32		SoundStartRandom(UINT32 uiSample);
BOOLEAN	 SoundRandomShouldPlay(UINT32 uiSample);

// Stopping
BOOLEAN		SoundStopIndex(UINT32 uiSound);

// Checks
BOOLEAN		SoundSampleIsPlaying(UINT32 uiSample);
BOOLEAN		SoundIndexIsPlaying(UINT32 uiSound);
BOOLEAN	 SoundSampleIsInUse(UINT32 uiSample);

// Volume operations
UINT32		SoundGetVolumeIndex(UINT32 uiChannel);
BOOLEAN		SoundSetVolumeIndex(UINT32 uiChannel, UINT32 uiVolume);

// Cleaning
UINT32		SoundGetEmptySample(void);

// Index, ID, other...
UINT32		SoundGetIndexByID(UINT32 uiSoundID);
UINT32		SoundGetFreeChannel(void);
UINT32		SoundGetUniqueID(void);

// Callbacks
void * F_CALLBACKAPI	SoundFileOpen (const STR8 pName);
void F_CALLBACKAPI		SoundFileClose(void *uiHandle);
INT F_CALLBACKAPI		SoundFileRead (void *pBuffer, INT iSize, void *uiHandle);
INT F_CALLBACKAPI		SoundFileSeek (void *uiHandle, INT iPos, signed char cMode);
INT F_CALLBACKAPI		SoundFileTell (void *uiHandle);

// Global variables
UINT32		guiSoundDefaultVolume = 127;
UINT32		guiSoundMemoryLimit=SOUND_DEFAULT_MEMORY;		// Maximum memory used for sounds
UINT32		guiSoundMemoryUsed=0;							// Memory currently in use
UINT32		guiSoundCacheThreshold=SOUND_DEFAULT_THRESH;	// Double-buffered threshold

BOOLEAN	 hSoundDriver;		// Sound driver ready ?
BOOLEAN		fDirectSound=TRUE;	// Using Direct Sound

// Local module variables
BOOLEAN		fSoundSystemInit=FALSE;		// Startup called T/F
BOOLEAN		gfEnableStartup=TRUE;		// Allow hardware to starup

// Struct definition for sample slots in the cache
//		Holds the regular sample data, as well as the
//		data for the random samples

typedef struct {
				CHAR8		pName[128];						// Sample path
				UINT32		uiSize;							// Sample size
				UINT32		uiFlags;						// Status flags
				PTR			pData;							// Pointer to loaded sample
				UINT32		uiCacheHits;					// Cache hits for this sample

				UINT32		uiTimeNext;						// Random sound data
				UINT32		uiTimeMin, uiTimeMax;
				UINT32		uiVolMin, uiVolMax;
				UINT32		uiPanMin, uiPanMax;
				UINT32		uiPriority;					 // Priority
				UINT32		uiInstances;					// Instances of sample sounding at this moment
				UINT32		uiMaxInstances;				 // Max allowable instances of sample
				} SAMPLETAG;

// Structure definition for slots in the sound output
//		These are used for both the cached and double-buffered
//		streams
typedef struct {
				UINT32			uiSample;		// Sample slot in cache
				FSOUND_STREAM*	hStream;		// Stream fmod handler
				UINT32		 uiFMODChannel;	// Fmod channel
				UINT32			uiFlags;		// Sample flags
				UINT32			uiSoundID;		// Sound unique ID
				UINT32			uiPriority;	 // Priority
				void			(*EOSCallback)(void *);
				void			*pCallbackData;
				UINT32			uiTimeStamp;	// Time stamp
				BOOLEAN			fLooping;		// Loop flag
				BOOLEAN			fStopAtZero;	// Stop at zero volume
				UINT32			uiFadeVolume;
				UINT32			uiFadeRate;
				UINT32			uiFadeTime;
				} SOUNDTAG;

// Sample cache list for files loaded
SAMPLETAG	pSampleList[SOUND_MAX_CACHED];
// Sound channel list for output channels
SOUNDTAG	pSoundList[SOUND_MAX_CHANNELS];

//*******************************************************************************
// High Level Interface
//*******************************************************************************

//*******************************************************************************
// SoundEnableSound
//
//	Allows or disallows the startup of the sound hardware.
//
//	Returns:	Nothing.
//
//*******************************************************************************
void SoundEnableSound(BOOLEAN fEnable)
{
	gfEnableStartup=fEnable;
}

//*******************************************************************************
// SoundGetDriverHandle
//
//	Returns:	Pointer to the system level output device module.
//
//*******************************************************************************
void *SoundGetDriverHandle( void )
{
	if(fSoundSystemInit)
		return(FSOUND_GetOutputHandle());
	else
		return(NULL);
}

//*******************************************************************************
// InitializeSoundManager
//
//	Zeros out the structs for the system info, and initializes the cache.
//
//	Returns:	TRUE always
//
//*******************************************************************************
BOOLEAN InitializeSoundManager(void)
{
	UINT32 uiCount;

	InitLogging();

	if(fSoundSystemInit)
	{
		SoundLog("Reopening JA2 sound manager");
		ShutdownSoundManager();
	}
	else
		SoundLog("Initialising JA2 sound manager");

	SoundLog((CHAR8 *)String("	Using %d channels", SOUND_MAX_CHANNELS));

	for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
	{
		memset(&pSoundList[uiCount], 0, sizeof(SOUNDTAG));
	}

	// Setup callbacks
	FSOUND_File_SetCallbacks(SoundFileOpen, SoundFileClose, SoundFileRead, SoundFileSeek, SoundFileTell);

#ifndef SOUND_DISABLE
	if(gfEnableStartup && SoundInitHardware())
		fSoundSystemInit=TRUE;
#endif

	SoundInitCache();

	SoundLog((CHAR8 *)String("	Sound memory limit = %i", SOUND_DEFAULT_MEMORY));
	SoundLog((CHAR8 *)String("	Cache threshold = %i", SOUND_DEFAULT_THRESH));

	guiSoundMemoryLimit=SOUND_DEFAULT_MEMORY;
	guiSoundMemoryUsed=0;
	guiSoundCacheThreshold=SOUND_DEFAULT_THRESH;

	return(TRUE);
}

//*******************************************************************************
// ShutdownSoundManager
//
//		Silences all currently playing sound, deallocates any memory allocated,
//	and releases the sound hardware.
//
//*******************************************************************************
void ShutdownSoundManager(void)
{
	SoundLog("Closing sound system...");

	SoundStopAll();
	SoundShutdownCache();
	Sleep(1000);
	SoundShutdownHardware();
	fSoundSystemInit=FALSE;
	SoundLog("JA2 sound manager shutdown");
}

//*******************************************************************************
// SoundPlay
//
//		Starts a sample playing. If the sample is not loaded in the cache, it will
//	be found and loaded. The pParms structure is used to
//	override the attributes of the sample such as playback speed, and to specify
//	a volume. Any entry containing SOUND_PARMS_DEFAULT will be set by the system.
//
//	Returns:	If the sound was started, it returns a sound ID unique to that
//						instance of the sound
//						If an error occured, SOUND_ERROR will be returned
//
//
//	!!Note:	Can no longer play streamed files
//
//*******************************************************************************

UINT32 SoundPlay(STR pFilename, SOUNDPARMS *pParms)
{
	UINT32 uiSample, uiChannel;

	if( fSoundSystemInit )
	{
		if( !SoundPlayStreamed(pFilename) )
		{
			if((uiSample=SoundLoadSample(pFilename))!=NO_SAMPLE)
			{
				if((uiChannel=SoundGetFreeChannel())!=SOUND_ERROR)
				{
					return(SoundStartSample(uiSample, uiChannel, pParms));
				}
			}
		}
		else
		{
			//Trying to play a sound which is bigger then the 'guiSoundCacheThreshold'
			FastDebugMsg(String("SoundPlay: ERROR: Trying to play %s sound is too lardge to load into cache, use SoundPlayStreamedFile() instead\n", pFilename ) );
		}
	}

	return(SOUND_ERROR);
}


//*******************************************************************************
// SoundPlayStreamedFile
//
//		The sample will
//	be played as a double-buffered sample. The pParms structure is used to
//	override the attributes of the sample such as playback speed, and to specify
//	a volume. Any entry containing SOUND_PARMS_DEFAULT will be set by the system.
//
//	Returns:	If the sound was started, it returns a sound ID unique to that
//						instance of the sound
//						If an error occured, SOUND_ERROR will be returned
//
//*******************************************************************************
UINT32	SoundPlayStreamedFile( STR pFilename, SOUNDPARMS *pParms )
{
	UINT32	uiChannel;
	HWFILE	hFile;

	if( fSoundSystemInit )
	{
		if((uiChannel=SoundGetFreeChannel())!=SOUND_ERROR)
		{
			hFile = FileOpen( pFilename, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
			if( !hFile )
			{							
				SoundLog((CHAR8 *)String("	ERROR in SoundPlayStreamedFile():	Couldnt open '%s'", pFilename ) );
				return( SOUND_ERROR );
			}

			UINT32 uiSize = FileGetSize(hFile);
			FileClose(hFile);

			if (uiSize == 0)
				return ( NO_SAMPLE);
			
			return(SoundStartStream( pFilename, uiChannel, pParms));
		}
	}
	return(SOUND_ERROR);
}

//*******************************************************************************
// SoundPlayRandom
//
//		Registers a sample to be played randomly within the specified parameters.
//	Parameters are passed in through pParms. Any parameter containing
//	SOUND_PARMS_DEFAULT will be set by the system. Only the uiTimeMin entry may
//	NOT be defaulted.
//
//	* Samples designated "random" are ALWAYS loaded into the cache, and locked
//	in place. They are never double-buffered, and this call will fail if they
//	cannot be loaded. *
//
//	Returns:	If successful, it returns the sample index it is loaded to, else
//						SOUND_ERROR is returned.
//
//*******************************************************************************
UINT32 SoundPlayRandom(STR pFilename, RANDOMPARMS *pParms)
{
	UINT32 uiSample;

	if(fSoundSystemInit)
	{
		if((uiSample=SoundLoadSample(pFilename))!=NO_SAMPLE)
		{
			// Sample loaded - marking slot
			pSampleList[uiSample].uiFlags|=(SAMPLE_RANDOM|SAMPLE_LOCKED);

			// Setup time intervals
			if(pParms->uiTimeMin==SOUND_PARMS_DEFAULT)
				return(SOUND_ERROR);
			else
				pSampleList[uiSample].uiTimeMin=pParms->uiTimeMin;

			if(pParms->uiTimeMax==SOUND_PARMS_DEFAULT)
				pSampleList[uiSample].uiTimeMax=pParms->uiTimeMin;
			else
				pSampleList[uiSample].uiTimeMax=pParms->uiTimeMax;

			// Volume
			if(pParms->uiVolMin==SOUND_PARMS_DEFAULT)
				pSampleList[uiSample].uiVolMin=guiSoundDefaultVolume;
			else
				pSampleList[uiSample].uiVolMin=pParms->uiVolMin;

			if(pParms->uiVolMax==SOUND_PARMS_DEFAULT)
				pSampleList[uiSample].uiVolMax=guiSoundDefaultVolume;
			else
				pSampleList[uiSample].uiVolMax=pParms->uiVolMax;

			// Panning
			if(pParms->uiPanMin==SOUND_PARMS_DEFAULT)
			{
				pSampleList[uiSample].uiPanMin=128;
				pSampleList[uiSample].uiPanMax=128;
			}
			else
			{
				pSampleList[uiSample].uiPanMin=pParms->uiPanMin;
				pSampleList[uiSample].uiPanMax=pParms->uiPanMax;
			}

			// Max instances
			if(pParms->uiMaxInstances==SOUND_PARMS_DEFAULT)
				pSampleList[uiSample].uiMaxInstances=1;
			else
				pSampleList[uiSample].uiMaxInstances=pParms->uiMaxInstances;

			// Priority
			if(pParms->uiPriority==SOUND_PARMS_DEFAULT)
				pSampleList[uiSample].uiPriority=PRIORITY_RANDOM;
			else
				pSampleList[uiSample].uiPriority=pParms->uiPriority;

			pSampleList[uiSample].uiInstances=0;

			// Time stamp
			pSampleList[uiSample].uiTimeNext=GetTickCount()+pSampleList[uiSample].uiTimeMin+Random(pSampleList[uiSample].uiTimeMax-pSampleList[uiSample].uiTimeMin);

			return(uiSample);
		}
		else
		{
			SoundLog((CHAR8 *)String("	ERROR in SoundPlayRandom():	Couldnt open '%s'", pFilename ) );
		}
	}

	return(SOUND_ERROR);
}

//*******************************************************************************
// SoundIsPlaying
//
//		Returns TRUE/FALSE that an instance of a sound is still playing.
//
//*******************************************************************************
BOOLEAN SoundIsPlaying(UINT32 uiSoundID)
{
UINT32 uiSound;

	if(fSoundSystemInit)
	{
		uiSound=SoundGetIndexByID(uiSoundID);
		if(uiSound!=NO_SAMPLE)
		{
			return(SoundIndexIsPlaying(uiSound));
		}
	}

	return(FALSE);
}

//*****************************************************************************************
// SoundIndexIsPlaying
//
// Returns TRUE/FALSE whether a sound channel's sample is currently playing.
//
// Returns BOOLEAN			- TRUE = playing, FALSE = stopped or nothing allocated
//
// UINT32 uiSound			 - Channel number of sound
//
// Created:	2/24/00 Derek Beland
//*****************************************************************************************
BOOLEAN SoundIndexIsPlaying(UINT32 uiSound)
{
	if(fSoundSystemInit)
	{
		if( pSoundList[uiSound].hStream!=NULL )
			return(FSOUND_IsPlaying(pSoundList[uiSound].uiFMODChannel));
	}

	return(FALSE);
}

//*******************************************************************************
// SoundStop
//
//		Stops the playing of a sound instance, if still playing.
//
//	Returns:	TRUE if the sample was actually stopped, FALSE if it could not be
//						found, or was not playing.
//
//*******************************************************************************
BOOLEAN SoundStop(UINT32 uiSoundID)
{
UINT32 uiSound;

	if(fSoundSystemInit)
	{
		if(SoundIsPlaying(uiSoundID))
		{
			uiSound=SoundGetIndexByID(uiSoundID);
			if(uiSound!=NO_SAMPLE)
			{
				SoundStopIndex(uiSound);
				return(TRUE);
			}
		}
	}

	return(FALSE);
}

//*******************************************************************************
// SoundSetMemoryLimit
//
//		Specifies how much memory the sound system is allowed to dynamically
// allocate. Once this limit is reached, the cache code will start dropping the
// least-used samples. You should always set the limit higher by a good margin
// than your actual memory requirements, to give the cache some elbow room.
//
//	Returns:	TRUE if the limit was set, or FALSE if the memory already used is
//						greater than the limit requested.
//
//*******************************************************************************
BOOLEAN SoundSetMemoryLimit(UINT32 uiLimit)
{
	if(guiSoundMemoryLimit < guiSoundMemoryUsed)
		return(FALSE);

	guiSoundMemoryLimit=uiLimit;
	return(TRUE);
}

//*******************************************************************************
// SoundGetSystemInfo
//
//		Returns information about the capabilities of the hardware. Currently does
//	nothing.
//
//	Returns:	FALSE, always
//
//*******************************************************************************
BOOLEAN SoundGetSystemInfo(void)
{
	return(FALSE);
}

//*****************************************************************************************
// SoundSetDefaultVolume
//
// Sets the volume to use when a default is not chosen.
//
// Returns BOOLEAN			-
//
// UINT32 uiVolume			-
//
// Created:	3/28/00 Derek Beland
//*****************************************************************************************
void SoundSetDefaultVolume(UINT32 uiVolume)
{
	guiSoundDefaultVolume=__min(uiVolume, MAX_VOLUME);
}

//*****************************************************************************************
// SoundGetDefaultVolume
//
//
//
// Returns UINT32			 -
//
// UINT32 uiVolume			-
//
// Created:	3/28/00 Derek Beland
//*****************************************************************************************
UINT32 SoundGetDefaultVolume(void)
{
	return(guiSoundDefaultVolume);
}

//*******************************************************************************
// SoundStopAll
//
//		Stops all currently playing sounds.
//
//	Returns:	TRUE, always
//
//*******************************************************************************
BOOLEAN SoundStopAll(void)
{
UINT32 uiCount;

	SoundLog("	Stopping all sounds");

	if(fSoundSystemInit)
	{
		for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
				SoundStopIndex(uiCount);
	}

	return(TRUE);
}


//*******************************************************************************
// SoundSetVolume
//
//		Sets the volume on a currently playing sound.
//
//	Returns:	TRUE if the volume was actually set on the sample, FALSE if the
//						sample had already expired or couldn't be found
//
//*******************************************************************************
BOOLEAN SoundSetVolume(UINT32 uiSoundID, UINT32 uiVolume)
{
UINT32 uiSound, uiVolCap;

	if(fSoundSystemInit)
	{
		uiVolCap=__min(uiVolume, MAX_VOLUME);

		if((uiSound=SoundGetIndexByID(uiSoundID))!=NO_SAMPLE)
		{
			pSoundList[uiSound].uiFadeVolume = uiVolume;
			return(SoundSetVolumeIndex(uiSound, uiVolume));
		}
	}

	return(FALSE);
}


//*****************************************************************************************
// SoundSetVolumeIndex
//
// Sounds the volume on a sound channel.
//
// Returns BOOLEAN			- TRUE if the volume was set
//
// UINT32 uiChannel			- Sound channel
// UINT32 uiVolume			- New volume 0-127
//
// Created:	3/17/00 Derek Beland
//*****************************************************************************************
BOOLEAN SoundSetVolumeIndex(UINT32 uiChannel, UINT32 uiVolume)
{
UINT32 uiVolCap;

	if(fSoundSystemInit)
	{
		uiVolCap=__min(uiVolume, MAX_VOLUME);

		if( pSoundList[uiChannel].hStream!=NULL )
			FSOUND_SetVolume(pSoundList[uiChannel].uiFMODChannel, uiVolCap * 2);

		return(TRUE);
	}

	return(FALSE);
}

//*******************************************************************************
// SoundSetPan
//
//		Sets the pan on a currently playing sound.
//
//	Returns:	TRUE if the pan was actually set on the sample, FALSE if the
//						sample had already expired or couldn't be found
//
//*******************************************************************************
BOOLEAN SoundSetPan(UINT32 uiSoundID, UINT32 uiPan)
{
UINT32 uiSound, uiPanCap;

	if(fSoundSystemInit)
	{
		uiPanCap=__min(uiPan, 255);

		if((uiSound=SoundGetIndexByID(uiSoundID))!=NO_SAMPLE)
		{
			if( pSoundList[uiSound].hStream!=NULL )
				FSOUND_SetPan(pSoundList[uiSound].uiFMODChannel, uiPanCap);

			return(TRUE);
		}
	}

	return(FALSE);
}


//*******************************************************************************
// SoundGetVolume
//
//		Returns the current volume setting of a sound that is playing. If the sound
//	has expired, or could not be found, SOUND_ERROR is returned.
//
//*******************************************************************************
UINT32 SoundGetVolume(UINT32 uiSoundID)
{
UINT32 uiSound;

	if(fSoundSystemInit)
	{
		if((uiSound=SoundGetIndexByID(uiSoundID))!=NO_SAMPLE)
			return(SoundGetVolumeIndex(uiSound));
	}

	return(SOUND_ERROR);
}


//*****************************************************************************************
// SoundGetVolumeIndex
//
// Returns the current volume of a sound channel.
//
// Returns UINT32			 - Volume 0-127
//
// UINT32 uiChannel			- Channel
//
// Created:	3/17/00 Derek Beland
//*****************************************************************************************
UINT32 SoundGetVolumeIndex(UINT32 uiChannel)
{
	if(fSoundSystemInit)
	{
		if( pSoundList[uiChannel].hStream!=NULL )
			return((UINT32)FSOUND_GetVolume(pSoundList[uiChannel].uiFMODChannel) / 2);
	}

	return(SOUND_ERROR);
}

//*******************************************************************************
// SoundServiceRandom
//
//		This function should be polled by the application if random samples are
//	used. The time marks on each are checked and if it is time to spawn a new
//	instance of the sound, the number already in existance are checked, and if
//	there is room, a new one is made and the count updated.
//		If random samples are not being used, there is no purpose in polling this
//	function.
//
//	Returns:	TRUE if a new random sound was created, FALSE if nothing was done.
//
//*******************************************************************************
BOOLEAN SoundServiceRandom(void)
{
UINT32 uiCount;
BOOLEAN fRandomSoundWasCreated=FALSE;

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
	{
		if(!(pSampleList[uiCount].uiFlags&SAMPLE_RANDOM_MANUAL) && SoundRandomShouldPlay(uiCount))
			fRandomSoundWasCreated |= SoundStartRandom(uiCount);
	}

	return(fRandomSoundWasCreated);
}

//*******************************************************************************
// SoundRandomShouldPlay
//
//	Determines whether a random sound is ready for playing or not.
//
//	Returns:	TRUE if a the sample should be played.
//
//*******************************************************************************
BOOLEAN SoundRandomShouldPlay(UINT32 uiSample)
{
	if(pSampleList[uiSample].uiFlags&SAMPLE_RANDOM)
		if(pSampleList[uiSample].uiTimeNext <= GetTickCount())
			if(pSampleList[uiSample].uiInstances < pSampleList[uiSample].uiMaxInstances)
			{
				return(TRUE);
			}

	return(FALSE);
}

//*******************************************************************************
// SoundStartRandom
//
//	Starts an instance of a random sample.
//
//	Returns:	TRUE if a new random sound was created, FALSE if nothing was done.
//
//*******************************************************************************
UINT32 SoundStartRandom(UINT32 uiSample)
{
UINT32 uiChannel, uiSoundID;
SOUNDPARMS spParms;

	if((uiChannel=SoundGetFreeChannel())!=SOUND_ERROR)
	{
		memset(&spParms, 0xff, sizeof(SOUNDPARMS));

		spParms.uiVolume=pSampleList[uiSample].uiVolMin+Random(pSampleList[uiSample].uiVolMax-pSampleList[uiSample].uiVolMin);
		spParms.uiPan=pSampleList[uiSample].uiPanMin+Random(pSampleList[uiSample].uiPanMax-pSampleList[uiSample].uiPanMin);
		spParms.uiLoop=1;
		spParms.uiPriority=pSampleList[uiSample].uiPriority;

		if((uiSoundID=SoundStartSample(uiSample, uiChannel, &spParms))!=SOUND_ERROR)
		{
			pSampleList[uiSample].uiTimeNext=GetTickCount()+pSampleList[uiSample].uiTimeMin+Random(pSampleList[uiSample].uiTimeMax-pSampleList[uiSample].uiTimeMin);
			pSampleList[uiSample].uiInstances++;
			//SoundLog((CHAR8 *)String("	SoundPlayRandom():	Sample #%d = '%s'", uiSample, pFilename ) );
			return(TRUE);
		}
		else
			SoundLog((CHAR8 *)String("	ERROR in SoundStartRandom(): Sample #%d start error - %s", uiSample, FMOD_ErrorString(FSOUND_GetError())));
	}
	else
		SoundLog("	ERROR in SoundStartRandom(): Failed to get free channel");
	return(FALSE);
}

//*******************************************************************************
// SoundStopAllRandom
//
//		This function should be polled by the application if random samples are
//	used. The time marks on each are checked and if it is time to spawn a new
//	instance of the sound, the number already in existance are checked, and if
//	there is room, a new one is made and the count updated.
//		If random samples are not being used, there is no purpose in polling this
//	function.
//
//	Returns:	TRUE if a new random sound was created, FALSE if nothing was done.
//
//*******************************************************************************
BOOLEAN SoundStopAllRandom(void)
{
UINT32 uiChannel, uiSample;

	// Stop all currently playing random sounds
	for(uiChannel=0; uiChannel < SOUND_MAX_CHANNELS; uiChannel++)
	{
		if( pSoundList[uiChannel].hStream!=NULL )
		{
			uiSample=pSoundList[uiChannel].uiSample;

			// if this was a random sample, decrease the iteration count
			if ( (uiSample != -1) && (pSampleList[uiSample].uiFlags&SAMPLE_RANDOM) )
				SoundStopIndex(uiChannel);
		}
	}

	// Unlock all random sounds so they can be dumped from the cache, and
	// take the random flag off so they won't be serviced/played
	for(uiSample=0; uiSample < SOUND_MAX_CACHED; uiSample++)
	{
		if(pSampleList[uiSample].uiFlags & SAMPLE_RANDOM)
			pSampleList[uiSample].uiFlags &= (~(SAMPLE_RANDOM | SAMPLE_LOCKED));
	}

	return(TRUE);
}

//*******************************************************************************
// SoundServiceStreams
//
//		Can be polled in tight loops where sound buffers might starve due to heavy
//	hardware use, etc. Streams DO NOT normally need to be serviced manually, but
//	in some cases (heavy file loading) it might be desirable.
//
//		If you are using the end of sample callbacks, you must call this function
//	periodically to check the sample's status.
//
//	Returns:	TRUE always.
//
//*******************************************************************************
BOOLEAN SoundServiceStreams(void)
{
UINT32 uiCount;

	if(fSoundSystemInit)
	{
		for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
		{
			if( (pSoundList[uiCount].hStream!=NULL) && (pSoundList[uiCount].uiSample==-1) )
			{
				// If a sound has a handle, but isn't playing, stop it and free up the handle
				if(!SoundIsPlaying(pSoundList[uiCount].uiSoundID))
				{
					SoundStopIndex(uiCount);
				}
				else
				{ // Check the volume fades on currently playing sounds
					UINT32 uiVolume = SoundGetVolumeIndex(uiCount);
					UINT32 uiTime = GetTickCount();

					if((uiVolume != pSoundList[uiCount].uiFadeVolume) && (uiTime >= (pSoundList[uiCount].uiFadeTime + pSoundList[uiCount].uiFadeRate)) )
					{
						if(uiVolume < pSoundList[uiCount].uiFadeVolume)
							SoundSetVolumeIndex(uiCount, ++uiVolume);
						else if(uiVolume > pSoundList[uiCount].uiFadeVolume)
						{
							uiVolume--;
							if(!uiVolume && pSoundList[uiCount].fStopAtZero)
							{
								SoundStopIndex(uiCount);
								SoundLog((CHAR8 *)String("	SoundServiceStreams(): Stop at zero volume on channel %d", uiCount));
							}
							else
								SoundSetVolumeIndex(uiCount, uiVolume);
						}

						pSoundList[uiCount].uiFadeTime = uiTime;
					}
				}
			}
		}
	}

	return(TRUE);
}


//*******************************************************************************
// SoundGetPosition
//
//	Reports the current time position of the sample.
//
//	Note: You should be checking SoundIsPlaying very carefully while
//	calling this function.
//
//	Returns:	The current time of the sample in milliseconds.
//
//*******************************************************************************
UINT32 SoundGetPosition(UINT32 uiSoundID)
{
UINT32 uiSound, uiTime, uiPosition;

	if(fSoundSystemInit)
	{
		if((uiSound=SoundGetIndexByID(uiSoundID))!=NO_SAMPLE)
		{
			uiTime=GetTickCount();
			// check for rollover
			if(uiTime < pSoundList[uiSound].uiTimeStamp)
				uiPosition=(0-pSoundList[uiSound].uiTimeStamp)+uiTime;
			else
				uiPosition=(uiTime-pSoundList[uiSound].uiTimeStamp);

			return(uiPosition);
		}
	}
	return(0);
}


//*******************************************************************************
// Cacheing Subsystem
//*******************************************************************************

//*******************************************************************************
// SoundInitCache
//
//		Zeros out the structures of the sample list.
//
//*******************************************************************************
BOOLEAN SoundInitCache(void)
{
	UINT32 uiCount;

	SoundLog("Init cache...");
	SoundLog((CHAR8 *)String("	Using %d cache slots", SOUND_MAX_CACHED));

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
	{
		memset(&pSampleList[uiCount], 0, sizeof(SAMPLETAG));
	}

	return(TRUE);
}

//*******************************************************************************
// SoundShutdownCache
//
//		Empties out the cache.
//
//	Returns: TRUE, always
//
//*******************************************************************************
BOOLEAN SoundShutdownCache(void)
{
	SoundEmptyCache();
	SoundLog("Cache shutdown");
	return(TRUE);
}

//*******************************************************************************
// SoundSetCacheThreshold
//
//		Sets the sound size above which samples will be played double-buffered,
// below which they will be loaded into the cache.
//
//	Returns: TRUE, always
//
//*******************************************************************************
BOOLEAN SoundSetCacheThreshhold(UINT32 uiThreshold)
{
	if(uiThreshold==0)
		guiSoundCacheThreshold=SOUND_DEFAULT_THRESH;
	else
		guiSoundCacheThreshold=uiThreshold;

	return(TRUE);
}

//*******************************************************************************
// SoundEmptyCache
//
//		Frees up all samples in the cache.
//
//	Returns: TRUE, always
//
//*******************************************************************************
BOOLEAN SoundEmptyCache(void)
{
UINT32 uiCount;

	SoundLog("Cleaning cache");
	SoundStopAll();

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
		SoundFreeSampleIndex(uiCount);

	return(TRUE);
}


//*******************************************************************************
// SoundLoadSample
//
//		Loads a sample into cache.
//
//	Returns: Sample index, if OK. NO_SAMPLE - if error.
//
//*******************************************************************************
UINT32 SoundLoadSample(STR pFilename)
{
UINT32 uiSample=NO_SAMPLE;

	if((uiSample=SoundGetCached(pFilename))!=NO_SAMPLE)
		return(uiSample);

	return(SoundLoadDisk(pFilename));
}

//*******************************************************************************
// SoundLockSample
//
//		Locks a sample into cache memory, so the cacheing system won't release it
//	when it needs room.
//
//	Returns: The sample index if successful, NO_SAMPLE if the file wasn't found
//						in the cache.
//
//*******************************************************************************
UINT32 SoundLockSample(STR pFilename)
{
UINT32 uiSample;

	if((uiSample=SoundGetCached(pFilename))!=NO_SAMPLE)
	{
		pSampleList[uiSample].uiFlags|=SAMPLE_LOCKED;
		return(uiSample);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundUnlockSample
//
//		Removes the lock on a sample so the cache is free to dump it when necessary.
//
//	Returns: The sample index if successful, NO_SAMPLE if the file wasn't found
//						in the cache.
//
//*******************************************************************************
UINT32 SoundUnlockSample(STR pFilename)
{
UINT32 uiSample;

	if((uiSample=SoundGetCached(pFilename))!=NO_SAMPLE)
	{
		pSampleList[uiSample].uiFlags&=(~SAMPLE_LOCKED);
		return(uiSample);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundFreeSample
//
//		Releases the resources associated with a sample from the cache.
//
//	Returns: The sample index if successful, NO_SAMPLE if the file wasn't found
//						in the cache.
//
//*******************************************************************************
UINT32 SoundFreeSample(STR pFilename)
{
UINT32 uiSample;

	if((uiSample=SoundGetCached(pFilename))!=NO_SAMPLE)
	{
		if(!SoundSampleIsPlaying(uiSample))
		{
			SoundFreeSampleIndex(uiSample);
			return(uiSample);
		}
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundGetCached
//
//		Tries to locate a sound by looking at what is currently loaded in the
//	cache.
//
//	Returns: The sample index if successful, NO_SAMPLE if the file wasn't found
//						in the cache.
//
//*******************************************************************************
UINT32 SoundGetCached(STR pFilename)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
	{
		if(_stricmp(pSampleList[uiCount].pName, pFilename)==0)
			return(uiCount);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundLoadDisk
//
//		Loads a sound file from disk into the cache, allocating memory and a slot
//	for storage.
//
//
//	Returns: The sample index if successful, NO_SAMPLE if the file wasn't found
//						in the cache.
//
//*******************************************************************************
UINT32 SoundLoadDisk(STR pFilename)
{
HWFILE	hFile;
UINT32	uiSize, uiSample;
BOOLEAN fRemoved;

	if((hFile=FileOpen(pFilename, FILE_ACCESS_READ, FALSE))!=0)
	{
		uiSize=FileGetSize(hFile);
		if(uiSize == 0)
		{
			return NO_SAMPLE;
		}

		// if insufficient memory, start unloading old samples until either
		// there's nothing left to unload, or we fit
		fRemoved=TRUE;
		while(((uiSize + guiSoundMemoryUsed) > guiSoundMemoryLimit) && (fRemoved))
			fRemoved=SoundCleanCache();

		// if we still don't fit
		if((uiSize + guiSoundMemoryUsed) > guiSoundMemoryLimit)
		{
			SoundLog((CHAR8 *)String("	ERROR in SoundLoadDisk():	trying to play '%s', not enough memory", pFilename ) );
			FileClose(hFile);
			return(NO_SAMPLE);
		}

		// if all the sample slots are full, unloading one
		if((uiSample=SoundGetEmptySample())==NO_SAMPLE)
		{
			SoundCleanCache();
			uiSample=SoundGetEmptySample();
		}

		// if we still don't have a sample slot
		if(uiSample==NO_SAMPLE)
		{
			SoundLog((CHAR8 *)String("	ERROR in SoundLoadDisk(): Trying to play '%s', cache slots are full", pFilename ) );
			FileClose(hFile);
			return(NO_SAMPLE);
		}

		memset(&pSampleList[uiSample], 0, sizeof(SAMPLETAG));

		if((pSampleList[uiSample].pData=MemAlloc(uiSize))==NULL)
		{
			SoundLog((CHAR8 *)String("	ERROR in SoundLoadDisk(): Trying to play '%s', memory allocation failed", pFilename ) );
			FileClose(hFile);
			return(NO_SAMPLE);
		}

		guiSoundMemoryUsed+=uiSize;

		FileRead(hFile, pSampleList[uiSample].pData, uiSize, NULL);
		FileClose(hFile);

		strcpy(pSampleList[uiSample].pName, pFilename);
		_strupr(pSampleList[uiSample].pName);
		pSampleList[uiSample].uiSize=uiSize;
		pSampleList[uiSample].uiFlags|=SAMPLE_ALLOCATED;
		return(uiSample);
	}

	else
	{
		SoundLog((CHAR8 *)String("	ERROR in SoundLoadDisk(): Failed to open '%s'", pFilename));
	}
	return(NO_SAMPLE);
}


//*******************************************************************************
// SoundCleanCache
//
//		Removes the least-used sound from the cache to make room.
//
//	Returns:	TRUE if a sample was freed, FALSE if none
//
//*******************************************************************************
BOOLEAN SoundCleanCache(void)
{
UINT32 uiCount, uiLowestHits=NO_SAMPLE, uiLowestHitsCount=0;

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
	{
		if((pSampleList[uiCount].uiFlags&SAMPLE_ALLOCATED) &&
			!(pSampleList[uiCount].uiFlags&SAMPLE_LOCKED))
		{
			if((uiLowestHits==NO_SAMPLE) || (uiLowestHitsCount < pSampleList[uiCount].uiCacheHits))
			{
				if(!SoundSampleIsPlaying(uiCount))
				{
					uiLowestHits=uiCount;
					uiLowestHitsCount=pSampleList[uiCount].uiCacheHits;
				}
			}
		}
	}

	if(uiLowestHits!=NO_SAMPLE)
	{
		SoundFreeSampleIndex(uiLowestHits);
		return(TRUE);
	}

	return(FALSE);
}

//*******************************************************************************
// Low Level Interface (Local use only)
//*******************************************************************************

//*******************************************************************************
// SoundSampleIsPlaying
//
//		Returns TRUE/FALSE that a sample is currently in use for playing a sound.
//
//*******************************************************************************
BOOLEAN SoundSampleIsPlaying(UINT32 uiSample)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
	{
		if(pSoundList[uiCount].uiSample==uiSample)
			return(TRUE);
	}

	return(FALSE);
}

//*******************************************************************************
// SoundGetEmptySample
//
//		Returns the slot number of an available sample index.
//
//	Returns:	A free sample index, or NO_SAMPLE if none are left.
//
//*******************************************************************************
UINT32 SoundGetEmptySample(void)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CACHED; uiCount++)
	{
		if(!(pSampleList[uiCount].uiFlags&SAMPLE_ALLOCATED))
			return(uiCount);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundFreeSampleIndex
//
//		Frees up a sample referred to by it's index slot number.
//
//	Returns:	Slot number if something was free, NO_SAMPLE otherwise.
//
//*******************************************************************************
UINT32 SoundFreeSampleIndex(UINT32 uiSample)
{
	if(pSampleList[uiSample].uiFlags&SAMPLE_ALLOCATED)
	{
		if(pSampleList[uiSample].pData!=NULL)
		{
			guiSoundMemoryUsed-=pSampleList[uiSample].uiSize;
			MemFree(pSampleList[uiSample].pData);
		}

		memset(&pSampleList[uiSample], 0, sizeof(SAMPLETAG));
		return(uiSample);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundGetIndexByID
//
//		Searches out a sound instance referred to by it's ID number.
//
//	Returns:	If the instance was found, the slot number. NO_SAMPLE otherwise.
//
//*******************************************************************************
UINT32 SoundGetIndexByID(UINT32 uiSoundID)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
	{
		if(pSoundList[uiCount].uiSoundID==uiSoundID)
			return(uiCount);
	}

	return(NO_SAMPLE);
}

//*******************************************************************************
// SoundInitHardware
//
//		Initializes the sound hardware through Windows/DirectX. THe highest possible
//	mixing rate and capabilities set are searched out and used.
//
//	Returns:	TRUE if the hardware was initialized, FALSE otherwise.
//
//*******************************************************************************
BOOLEAN SoundInitHardware(void)
{
	UINT32 uiCaps;

	SoundLog("Init hardware...");

	// Try to start up the FMOD Sound System
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
	FSOUND_SetBufferSize(AUDIO_BUFFER_LEN);
	SoundLog((CHAR8 *)String("	Using DirectSound driver: %s", FSOUND_GetDriverName(FSOUND_GetDriver())));
	SoundLog("	Driver capabilities:");
	FSOUND_GetDriverCaps( FSOUND_GetDriver(), &uiCaps );

	if ( uiCaps & FSOUND_CAPS_HARDWARE )
		SoundLog("		- supports hardware accelerated 3d sound");

	if ( uiCaps & FSOUND_CAPS_EAX2 )
		SoundLog("		- supports EAX 2 reverb");

	if ( uiCaps & FSOUND_CAPS_EAX3 )
		SoundLog("		- supports EAX 3 reverb");

	if( !FSOUND_Init(44100, SOUND_MAX_CHANNELS, FSOUND_INIT_GLOBALFOCUS|FSOUND_INIT_DONTLATENCYADJUST) )
	{
		SoundLog((CHAR8 *)String("	ERROR in SoundInitHardware(): %s", FMOD_ErrorString(FSOUND_GetError())));
		return(FALSE);
	}

	SoundLog("	FMOD started");
	SoundLog((CHAR8 *)String("	Mixing rate: %d", FSOUND_GetOutputRate()));

	// Driver is ready
	hSoundDriver = TRUE;
	fDirectSound = TRUE;

	return(hSoundDriver);
}

//*******************************************************************************
// SoundShutdownHardware
//
//		Shuts down the system hardware.
//
//	Returns:	TRUE always.
//
//*******************************************************************************
BOOLEAN SoundShutdownHardware(void)
{
	if(fSoundSystemInit)
		FSOUND_Close();

	SoundLog("	FMOD closed");
	return(TRUE);
}

//*******************************************************************************
// SoundGetFreeChannel
//
//		Finds an unused sound channel in the channel list.
//
//	Returns:	Index of a sound channel if one was found, SOUND_ERROR if not.
//
//*******************************************************************************
UINT32 SoundGetFreeChannel(void)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
	{
		if(!SoundIsPlaying(pSoundList[uiCount].uiSoundID))
		{
			SoundStopIndex(uiCount);
		}

		if( pSoundList[uiCount].hStream==NULL )
			return(uiCount);
	}

	return(SOUND_ERROR);
}

//*******************************************************************************
// SoundStartSample
//
//		Starts up a sample on the specified channel. Override parameters are passed
//	in through the structure pointer pParms. Any entry with a value of 0xffffffff
//	will be filled in by the system.
//
//	Returns:	Unique sound ID if successful, SOUND_ERROR if not.
//
//*******************************************************************************
UINT32 SoundStartSample(UINT32 uiSample, UINT32 uiChannel, SOUNDPARMS *pParms)
{
UINT32 uiSoundID;

	if(!fSoundSystemInit)
		return(SOUND_ERROR);

	// Setting up stream buffer
	FSOUND_Stream_SetBufferSize(STREAM_BUFFER_LEN);

	// Creating stream
	pSoundList[uiChannel].hStream=FSOUND_Stream_Open((STR8) pSampleList[uiSample].pData, FSOUND_LOADMEMORY|FSOUND_LOOP_NORMAL|FSOUND_2D, 0, pSampleList[uiSample].uiSize);
	if(pSoundList[uiChannel].hStream==NULL)
	{
		SoundLog((CHAR8 *)String(" ERROR in SoundStartSample(): %s", FMOD_ErrorString(FSOUND_GetError()) ));
		return(SOUND_ERROR);
	}

	// Setup sample params
	// Loop
	if((pParms!=NULL) && (pParms->uiLoop!=SOUND_PARMS_DEFAULT))
	{
		// If looping infinately, lock the sample so it can't be unloaded
		// and mark it as a looping sound
		if(pParms->uiLoop==0)
		{
			pSampleList[uiSample].uiFlags|=SAMPLE_LOCKED;
			pSoundList[uiChannel].fLooping=TRUE;
		}
		else
			FSOUND_Stream_SetLoopCount(pSoundList[uiChannel].hStream, pParms->uiLoop-1);
	}

	// Starting stream in pause
	pSoundList[uiChannel].uiFMODChannel = FSOUND_Stream_PlayEx(uiChannel, pSoundList[uiChannel].hStream, NULL, TRUE);
	if(pSoundList[uiChannel].uiFMODChannel==-1)
	{
		FSOUND_Stream_Close(pSoundList[uiChannel].hStream);
		SoundLog((CHAR8 *)String(" ERROR in SoundStartSample(): %s", FMOD_ErrorString(FSOUND_GetError()) ));
		return(SOUND_ERROR);
	}

	// Speed and pitchbend don't use

	// Volume
	if((pParms!=NULL) && (pParms->uiVolume!=SOUND_PARMS_DEFAULT))
		FSOUND_SetVolume(pSoundList[uiChannel].uiFMODChannel, pParms->uiVolume * 2);
	else
		FSOUND_SetVolume(pSoundList[uiChannel].uiFMODChannel, guiSoundDefaultVolume);

	// Panning
	if((pParms!=NULL) && (pParms->uiPan!=SOUND_PARMS_DEFAULT))
		FSOUND_SetPan(pSoundList[uiChannel].uiFMODChannel, pParms->uiPan);
	else
		FSOUND_SetPan(pSoundList[uiChannel].uiFMODChannel, 128);

	// Start sound!
	FSOUND_SetPaused(pSoundList[uiChannel].uiFMODChannel, FALSE);

	// Priority
	if((pParms!=NULL) && (pParms->uiPriority!=SOUND_PARMS_DEFAULT))
		pSoundList[uiChannel].uiPriority=pParms->uiPriority;
	else
		pSoundList[uiChannel].uiPriority=PRIORITY_MAX;

	// Callback at end of playback
	if((pParms!=NULL) && ((UINT32)pParms->EOSCallback!=SOUND_PARMS_DEFAULT))
	{
		pSoundList[uiChannel].EOSCallback=pParms->EOSCallback;
		pSoundList[uiChannel].pCallbackData=pParms->pCallbackData;
	}
	else
	{
		pSoundList[uiChannel].EOSCallback=NULL;
		pSoundList[uiChannel].pCallbackData=NULL;
	}

	// Other stuff...
	uiSoundID=SoundGetUniqueID();
	pSoundList[uiChannel].uiSoundID=uiSoundID;
	pSoundList[uiChannel].uiSample=uiSample;
	pSoundList[uiChannel].uiTimeStamp=GetTickCount();
	pSoundList[uiChannel].uiFadeVolume = SoundGetVolumeIndex(uiChannel);

	pSampleList[uiSample].uiCacheHits++;

	return(uiSoundID);
}

//*******************************************************************************
// SoundStartStream
//
//		Starts up a stream on the specified channel. Override parameters are passed
//	in through the structure pointer pParms. Any entry with a value of 0xffffffff
//	will be filled in by the system.
//
//	Returns:	Unique sound ID if successful, SOUND_ERROR if not.
//
//*******************************************************************************
UINT32 SoundStartStream(STR pFilename, UINT32 uiChannel, SOUNDPARMS *pParms)
{
UINT32 uiSoundID;

	if(!fSoundSystemInit)
		return(SOUND_ERROR);

	// Setting up stream buffer
	FSOUND_Stream_SetBufferSize(STREAM_BUFFER_LEN);

	// Creating stream
	pSoundList[uiChannel].hStream=FSOUND_Stream_Open(pFilename, FSOUND_LOOP_NORMAL|FSOUND_2D, 0, 0);
	if(pSoundList[uiChannel].hStream==NULL)
	{
		SoundLog((CHAR8 *)String(" ERROR in SoundStartStream(): %s ('%s')", FMOD_ErrorString(FSOUND_GetError()), pFilename));
		return(SOUND_ERROR);
	}

	// Setup params
	// Loop
	if( (pParms!=NULL) && (pParms->uiLoop!=SOUND_PARMS_DEFAULT ) )
	{
		if(pParms->uiLoop>0)
			FSOUND_Stream_SetLoopCount(pSoundList[uiChannel].hStream, pParms->uiLoop-1);
	}

	// Starting stream in pause
	pSoundList[uiChannel].uiFMODChannel = FSOUND_Stream_PlayEx(uiChannel, pSoundList[uiChannel].hStream, NULL, TRUE);
	if(pSoundList[uiChannel].uiFMODChannel==-1)
	{
		FSOUND_Stream_Close(pSoundList[uiChannel].hStream);
		SoundLog((CHAR8 *)String(" ERROR in SoundStartStream(): %s ('%s')", FMOD_ErrorString(FSOUND_GetError()), pFilename));
		return(SOUND_ERROR);
	}

	// Speed and pitchbend don't use

	// Volume
	if((pParms!=NULL) && (pParms->uiVolume!=SOUND_PARMS_DEFAULT))
		FSOUND_SetVolume(pSoundList[uiChannel].uiFMODChannel, pParms->uiVolume * 2);
	else
		FSOUND_SetVolume(pSoundList[uiChannel].uiFMODChannel, guiSoundDefaultVolume);

	// Panning
	if((pParms!=NULL) && (pParms->uiPan!=SOUND_PARMS_DEFAULT))
		FSOUND_SetPan(pSoundList[uiChannel].uiFMODChannel, pParms->uiPan);

	// Start sound!
	FSOUND_SetPaused(pSoundList[uiChannel].uiFMODChannel, FALSE);

	// Get ID
	uiSoundID=SoundGetUniqueID();
	pSoundList[uiChannel].uiSoundID=uiSoundID;

	// Priority
	if(pParms)
		pSoundList[uiChannel].uiPriority=pParms->uiPriority;
	else
		pSoundList[uiChannel].uiPriority=SOUND_PARMS_DEFAULT;

	// Callback at end of playback
	if((pParms!=NULL) && ((UINT32)pParms->EOSCallback!=SOUND_PARMS_DEFAULT))
	{
		pSoundList[uiChannel].EOSCallback=pParms->EOSCallback;
		pSoundList[uiChannel].pCallbackData=pParms->pCallbackData;
	}
	else
	{
		pSoundList[uiChannel].EOSCallback=NULL;
		pSoundList[uiChannel].pCallbackData=NULL;
	}

	// Other stuff
	pSoundList[uiChannel].uiTimeStamp=GetTickCount();
	pSoundList[uiChannel].uiFadeVolume = SoundGetVolumeIndex(uiChannel);
	pSoundList[uiChannel].uiSample = -1;	// it's streaming directly from file !!!!

	return(uiSoundID);
}

// ------------------------
// Callbacks implementation
// ========================
static void * F_CALLBACKAPI SoundFileOpen(const STR8 pName)
{
	return((void*)FileOpen(pName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE));
}

static void F_CALLBACKAPI SoundFileClose(void *uiHandle)
{
	FileClose((UINT32)uiHandle);
}

static INT F_CALLBACKAPI SoundFileRead(void *pBuffer, INT iSize, void *uiHandle)
{
	UINT32 uiActuallyRead;

	FileRead((UINT32)uiHandle, pBuffer, iSize, &uiActuallyRead);
	return(uiActuallyRead);
}

static INT F_CALLBACKAPI SoundFileSeek(void *uiHandle, INT iPos, signed char cMode)
{
	UINT8	uiHow;

	switch(cMode)
	{
	case SEEK_END:
		uiHow = FILE_SEEK_FROM_END;
		break;
	case SEEK_CUR:
		uiHow = FILE_SEEK_FROM_CURRENT;
		break;
	default:
		uiHow = FILE_SEEK_FROM_START;
	}

	return(!FileSeek((UINT32)uiHandle, iPos, uiHow));
}

static INT F_CALLBACKAPI SoundFileTell(void *uiHandle)
{
	return(FileGetPos((UINT32)uiHandle));
}

//*******************************************************************************
// SoundGetUniqueID
//
//		Returns a unique ID number with every call. Basically it's just a 32-bit
// static value that is incremented each time.
//
//*******************************************************************************
UINT32 SoundGetUniqueID(void)
{
static UINT32 uiNextID=0;

	if(uiNextID==NO_SAMPLE)
		uiNextID++;

	return(uiNextID++);
}

//*******************************************************************************
// SoundPlayStreamed
//
//		Returns TRUE/FALSE whether a sound file should be played as a streamed
//	sample, or loaded into the cache. The decision is based on the size of the
//	file compared to the guiSoundCacheThreshold.
//
//	Returns:	TRUE if it should be streamed, FALSE if loaded.
//
//*******************************************************************************
BOOLEAN SoundPlayStreamed(STR pFilename)
{
HWFILE hDisk;
UINT32 uiFilesize;

	if((hDisk=FileOpen(pFilename, FILE_ACCESS_READ, FALSE))!=0)
	{
		uiFilesize=FileGetSize(hDisk);
		FileClose(hDisk);
		return(uiFilesize >= guiSoundCacheThreshold);
	}

	return(FALSE);
}

//*******************************************************************************
// SoundStopIndex
//
//		Stops a sound referred to by it's slot number. This function is the only
//	one that should be deallocating sample handles. The random sounds have to have
//	their counters maintained, and using this as the central function ensures
//	that they stay in sync.
//
//	Returns:	TRUE if the sample was stopped, FALSE if it could not be found.
//
//*******************************************************************************
BOOLEAN SoundStopIndex(UINT32 uiChannel)
{
UINT32 uiSample;

	if(fSoundSystemInit)
	{
		if(uiChannel!=NO_SAMPLE)
		{
			if(pSoundList[uiChannel].hStream!=NULL)
			{
				FSOUND_Stream_Stop(pSoundList[uiChannel].hStream);
				FSOUND_Stream_Close(pSoundList[uiChannel].hStream);

			 uiSample=pSoundList[uiChannel].uiSample;

				// if this was a random sample, decrease the iteration count
				if ( (uiSample != -1) && (pSampleList[uiSample].uiFlags&SAMPLE_RANDOM) )
					pSampleList[uiSample].uiInstances--;

				if(pSoundList[uiChannel].EOSCallback!=NULL)
					pSoundList[uiChannel].EOSCallback(pSoundList[uiChannel].pCallbackData);

				if(!pSoundList[uiChannel].fLooping && !SoundSampleIsInUse(uiChannel))
					SoundRemoveSampleFlags(uiSample, SAMPLE_LOCKED);

				memset(&pSoundList[uiChannel], 0, sizeof(SOUNDTAG));
			}

			return(TRUE);
		}
	}

	return(FALSE);
}

// FUNCTIONS TO SET / RESET SAMPLE FLAGS
void SoundSetSampleFlags( UINT32 uiSample, UINT32 uiFlags )
{
	// CHECK FOR VALUE SAMPLE
	if((pSampleList[ uiSample ].uiFlags&SAMPLE_ALLOCATED) )
	{
		// SET
		pSampleList[uiSample].uiFlags |= uiFlags;
	}
}

void SoundRemoveSampleFlags( UINT32 uiSample, UINT32 uiFlags )
{
	// CHECK FOR VALID SAMPLE
	if((pSampleList[ uiSample ].uiFlags&SAMPLE_ALLOCATED) )
	{
		//REMOVE
		pSampleList[uiSample].uiFlags &= (~uiFlags);
	}
}

//*******************************************************************************
// SoundSampleIsInUse
//
//	Returns:	TRUE if the sample index is currently being played by the system.
//
//*******************************************************************************
BOOLEAN SoundSampleIsInUse(UINT32 uiSample)
{
UINT32 uiCount;

	for(uiCount=0; uiCount < SOUND_MAX_CHANNELS; uiCount++)
	{
		if((pSoundList[uiCount].uiSample==uiSample) && SoundIsPlaying(uiCount))
			return(TRUE);
	}

	return(FALSE);
}

// Lesh modifications
// Sound debug
static struct SoundLog {
	sgp::Logger_ID id;
	SoundLog() {
		id = sgp::Logger::instance().createLogger();
		sgp::Logger::instance().connectFile(id, SndDebugFileName, true, sgp::Logger::FLUSH_ON_DELETE);
	}
} s_SoundLog;

//*****************************************************************************************
// SoundLog
//	Writes string into log file
//
// Returns nothing
//
// Created:	10.12.2005 Lesh
//*****************************************************************************************
void SoundLog(CHAR8 *strMessage)
{
#ifndef USE_VFS
	if ((SndDebug = fopen(SndDebugFileName, "a+t")) != NULL)
	{
	 fprintf(SndDebug, "%s\n", strMessage);
		fclose(SndDebug);
	}
#else
	SGP_LOG(s_SoundLog.id, vfs::String::widen(strMessage,strlen(strMessage)));
#endif
}

//*****************************************************************************************
// InitLogging
//	Creates empty log file
//
// Returns nothing
//
// Created:	10.12.2005 Lesh
//*****************************************************************************************
void InitLogging()
{
#ifndef USE_VFS
	if ((SndDebug = fopen(SndDebugFileName, "wt")) != NULL)
	{
		fclose(SndDebug);
	}
#endif
}
