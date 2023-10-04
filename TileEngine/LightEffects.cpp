#include "builddefines.h"

	#include "debug.h"
	#include "animation control.h"
	#include "lighteffects.h"
	#include "isometric utils.h"
	#include "lighting.h"
	#include "Game Clock.h"
	#include "opplist.h"
	#include "Tactical Save.h"

#include "SaveLoadGame.h"
#include "GameVersion.h"	// added by Flugente


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define		NUM_LIGHT_EFFECT_SLOTS					500 // 25	// Flugente: raised to 500  - lets see what happens


// GLOBAL FOR LIGHT LISTING
LIGHTEFFECT				gLightEffectData[ NUM_LIGHT_EFFECT_SLOTS ];
UINT32						guiNumLightEffects = 0;


INT32 GetFreeLightEffect( void );
void RecountLightEffects( void );


INT32 GetFreeLightEffect( void )
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumLightEffects; uiCount++)
	{
		if(( gLightEffectData[uiCount].fAllocated==FALSE ) )
			return( (INT32)uiCount );
	}

	if( guiNumLightEffects < NUM_LIGHT_EFFECT_SLOTS )
		return( (INT32) guiNumLightEffects++ );

	return( -1 );
}

void RecountLightEffects( void )
{
	INT32 uiCount;

	for(uiCount=guiNumLightEffects-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gLightEffectData[uiCount].fAllocated ) )
		{
			guiNumLightEffects=(UINT32)(uiCount+1);
			break;
		}
	}
}


void UpdateLightingSprite( LIGHTEFFECT *pLight )
{
	CHAR8 LightName[20];
	// Build light....

	sprintf( LightName, "Light%d", pLight->bRadius );

	// Delete old one if one exists...
	if( pLight->iLight!=(-1) )
	{
		LightSpriteDestroy( pLight->iLight );
		pLight->iLight = -1;
	}

	// Effect light.....
	if( ( pLight->iLight = LightSpriteCreate( LightName, 0 ) )==(-1))
	{
		// Could not light!
		return;
	}

	LightSpritePower( pLight->iLight, TRUE );
//	LightSpriteFake( pLight->iLight );
	INT16 sX, sY;
	ConvertGridNoToCenterCellXY(pLight->sGridNo, &sX, &sY);

	LightSpritePosition( pLight->iLight, (INT16)( sX / CELL_X_SIZE ), (INT16)( sY / CELL_Y_SIZE ) );
}

// Flugente: create a pure light, worry about updating sight in other functions
INT32 NewLightEffectInternal( INT32 sGridNo, UINT8 ubDuration, UINT8 ubStartRadius )
{
	LIGHTEFFECT *pLight;
	INT32				iLightIndex;

	if( ( iLightIndex = GetFreeLightEffect() )==(-1) )
		return(-1);

	memset( &gLightEffectData[ iLightIndex ], 0, sizeof( LIGHTEFFECT ) );

	pLight = &gLightEffectData[ iLightIndex ];

	// Set some values...
	pLight->sGridNo									= sGridNo;
	pLight->iLight									= -1;
	pLight->uiTimeOfLastUpdate			= GetWorldTotalSeconds( );

	pLight->ubDuration	= ubDuration;
	pLight->bRadius	 = ubStartRadius;
	pLight->bAge				= 0;
	pLight->fAllocated	= TRUE;

	UpdateLightingSprite( pLight );

	return iLightIndex;
}

INT32 NewLightEffect( INT32 sGridNo, UINT8 ubDuration, UINT8 ubStartRadius )
{
	INT32 iLightIndex = NewLightEffectInternal( sGridNo, ubDuration, ubStartRadius );

	// Handle sight here....
	AllTeamsLookForAll( FALSE );

	//Play the breaklight sound
//	PlayJA2Sample( BREAK_LIGHT_IGNITING, RATE_11025, SoundVolume( LOWVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
// MAdd:	for some reason this crashes the game!

	if ( iLightIndex > -1 )
	{
		LIGHTEFFECT *pLight = &gLightEffectData[ iLightIndex ];
		return( pLight->iLight );
	}

	return -1;
}



void RemoveLightEffectFromTile( INT32 sGridNo )
{
	LIGHTEFFECT *pLight;
	UINT32 cnt;

	// Set to unallocated....
	for ( cnt = 0; cnt < guiNumLightEffects; cnt++ )
	{
		pLight = &gLightEffectData[ cnt ];

		if ( pLight->fAllocated )
		{
			if ( pLight->sGridNo == sGridNo )
			{
				pLight->fAllocated = FALSE;

				// Remove light....
				if( pLight->iLight != (-1) )
				{
					LightSpriteDestroy( pLight->iLight );
				}
				break;
			}
		}
	}

}

BOOLEAN IsLightEffectAtTile( INT32 sGridNo )
{
	LIGHTEFFECT *pLight;
	UINT32 cnt;

	// Set to unallocated....
	for ( cnt = 0; cnt < guiNumLightEffects; cnt++ )
	{
		pLight = &gLightEffectData[ cnt ];

		if ( pLight->fAllocated )
		{
			if ( pLight->sGridNo == sGridNo )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void DecayLightEffects( UINT32 uiTime )
{
	LIGHTEFFECT *pLight;
	UINT32 cnt, cnt2;
	BOOLEAN fUpdate = FALSE;
	BOOLEAN	fDelete = FALSE;
	BOOLEAN fAnyUpdate = FALSE;
	UINT16	usNumUpdates = 1;

	// age all active light effects, deactivate those that are just dispersing
	for ( cnt = 0; cnt < guiNumLightEffects; ++cnt )
	{
		pLight = &gLightEffectData[ cnt ];

		fDelete = FALSE;

		if ( pLight->fAllocated )
		{
			usNumUpdates = 1;

			// Do things differently for combat /vs realtime
			// always try to update during combat
			if ( gTacticalStatus.uiFlags & INCOMBAT )
			{
				fUpdate = TRUE;
			}
			// ATE: Do this every so often, to achieve the effect we want...
			else if ( (uiTime - pLight->uiTimeOfLastUpdate) >= 10 )
			{
				fUpdate = TRUE;
				usNumUpdates = (UINT16)((uiTime - pLight->uiTimeOfLastUpdate) / 10);
			}

			if ( fUpdate )
			{
				pLight->uiTimeOfLastUpdate = uiTime;

				for ( cnt2 = 0; cnt2 < usNumUpdates; ++cnt2 )
				{
					++pLight->bAge;

					if ( pLight->ubDuration <= 0 )
						fDelete = TRUE;

					// if this light remains effective (duration not reached)
					if ( pLight->bAge < pLight->ubDuration)
					{
						// calculate the new cloud radius
						/*if ( ( pLight->bAge % 2 ) )
						{
							--pLight->bRadius;
						}*/
						if (pLight->ubDuration - pLight->bAge < pLight->bRadius)
						{
							pLight->bRadius--;
						}

						if ( pLight->bRadius == 0 )
						{
							// Delete...
							fDelete = TRUE;
							break;
						}
						else
						{
							UpdateLightingSprite( pLight );
						}
					}
					else
					{
						fDelete = TRUE;
						break;
					}
				}

				if ( fDelete )
				{
					pLight->fAllocated = FALSE;

					if( pLight->iLight != (-1) )
					{
						LightSpriteDestroy( pLight->iLight );
					}
				}

				// we have to update sight later on if any light was updated
				fAnyUpdate = TRUE;
			}
		}
	}

	if ( fAnyUpdate )
	{
		// Handle sight here....
		AllTeamsLookForAll( FALSE );
	}
}



BOOLEAN SaveLightEffectsToSaveGameFile( HWFILE hFile )
{
	/*
	UINT32	uiNumBytesWritten;
	UINT32	uiNumberOfLights=0;
	UINT32	uiCnt;

	//loop through and count the number of active slots
	for( uiCnt=0; uiCnt<guiNumLightEffects; uiCnt++)
	{
		if( gLightEffectData[ uiCnt ].fAllocated )
		{
			uiNumberOfLights++;
		}
	}

	//Save the Number of Light Effects
	FileWrite( hFile, &uiNumberOfLights, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}


	//if there are lights to save
	if( uiNumberOfLights != 0 )
	{
		//loop through and save each active slot
		for( uiCnt=0; uiCnt < guiNumLightEffects; uiCnt++)
		{
			if( gLightEffectData[ uiCnt ].fAllocated )
			{
				//Save the Light effect Data
				FileWrite( hFile, &gLightEffectData[ uiCnt ], sizeof( LIGHTEFFECT ), &uiNumBytesWritten );
				if( uiNumBytesWritten != sizeof( LIGHTEFFECT ) )
				{
					return( FALSE );
				}
			}
		}
	}
*/
	return( TRUE );
}


BOOLEAN LoadLightEffectsFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT32	uiCount;

	//no longer need to load Light effects.	They are now in temp files
	if( guiCurrentSaveGameVersion < 76 )
	{
		memset( gLightEffectData, 0, sizeof( LIGHTEFFECT ) *	NUM_LIGHT_EFFECT_SLOTS );

		//Load the Number of Light Effects
		FileRead( hFile, &guiNumLightEffects, sizeof( UINT32 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT32 ) )
		{
			return( FALSE );
		}

		//if there are lights saved.
		if( guiNumLightEffects != 0 )
		{
			//loop through and apply the light effects to the map
			for(uiCount=0; uiCount < guiNumLightEffects; uiCount++)
			{
				//Load the Light effect Data
				FileRead( hFile, &gLightEffectData[ uiCount ], sizeof( LIGHTEFFECT ), &uiNumBytesRead );
				if( uiNumBytesRead != sizeof( LIGHTEFFECT ) )
				{
					return( FALSE );
				}
			}
		}


		//loop through and apply the light effects to the map
		for(uiCount=0; uiCount < guiNumLightEffects; uiCount++)
		{
			if( gLightEffectData[uiCount].fAllocated )
				UpdateLightingSprite( &( gLightEffectData[uiCount] ) );
		}
	}

	return( TRUE );
}



BOOLEAN SaveLightEffectsToMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiNumLightEffects=0;
	HWFILE	hFile;
	UINT32	uiNumBytesWritten=0;
	CHAR8		zMapName[ 128 ];
	UINT32	uiCnt;

	//get the name of the map
	GetMapTempFileName( SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//delete file the file.
	FileDelete( zMapName );

	//loop through and count the number of Light effects
	for( uiCnt=0; uiCnt<guiNumLightEffects; uiCnt++)
	{
		if( gLightEffectData[ uiCnt ].fAllocated )
			uiNumLightEffects++;
	}

	//if there are no Light effects
	if( uiNumLightEffects == 0 )
	{
		//set the fact that there are no Light effects for this sector
		ReSetSectorFlag( sMapX, sMapY, bMapZ, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS );

		return( TRUE );
	}

	//Open the file for writing
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}


	//Save the Number of Light Effects
	FileWrite( hFile, &uiNumLightEffects, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		//Close the file
		FileClose( hFile );

		return( FALSE );
	}


	//loop through and save the number of Light effects
	for( uiCnt=0; uiCnt < guiNumLightEffects; uiCnt++)
	{
		//if the Light is active
		if( gLightEffectData[ uiCnt ].fAllocated )
		{
			//Save the Light effect Data
			FileWrite( hFile, &gLightEffectData[ uiCnt ], sizeof( LIGHTEFFECT ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( LIGHTEFFECT ) )
			{
				//Close the file
				FileClose( hFile );

				return( FALSE );
			}
		}
	}

	//Close the file
	FileClose( hFile );

	SetSectorFlag( sMapX, sMapY, bMapZ, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS );

	return( TRUE );
}



BOOLEAN LoadLightEffectsFromMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiNumBytesRead;
	UINT32	uiCount;
	UINT32	uiCnt=0;
	HWFILE	hFile;
	CHAR8		zMapName[ 128 ];

	GetMapTempFileName( SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//Open the file for reading, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening file
		return( FALSE );
	}

	//Clear out the old list
	ResetLightEffects();


	//Load the Number of Light Effects
	FileRead( hFile, &guiNumLightEffects, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	//loop through and load the list
	for( uiCnt=0; uiCnt<guiNumLightEffects;uiCnt++)
	{
		if(guiCurrentSaveGameVersion >= DYNAMIC_FLASHLIGHTS)
		{
			//Load the Light effect Data
			FileRead( hFile, &gLightEffectData[ uiCnt ], sizeof( LIGHTEFFECT ), &uiNumBytesRead );
			if( uiNumBytesRead != sizeof( LIGHTEFFECT ) )
			{
				return( FALSE );
			}
		}
		else
		{
			UINT16 bla = sizeof( LIGHTEFFECT );

			//Load the Light effect Data
			FileRead( hFile, &gLightEffectData[ uiCnt ], LIGHTEFFECT_OLDSIZE, &uiNumBytesRead );
			if( uiNumBytesRead != LIGHTEFFECT_OLDSIZE )
			{
				return( FALSE );
			}
		}
	}


	//loop through and apply the light effects to the map
	for(uiCount=0; uiCount < guiNumLightEffects; uiCount++)
	{
		if( gLightEffectData[uiCount].fAllocated )
			UpdateLightingSprite( &( gLightEffectData[uiCount] ) );
	}

	FileClose( hFile );

	return( TRUE );
}


void ResetLightEffects()
{
	//Clear out the old list
	memset( gLightEffectData, 0, sizeof( LIGHTEFFECT ) * NUM_LIGHT_EFFECT_SLOTS );
	guiNumLightEffects = 0;
}

// Flugente: create and destroy light sources tied to a person
void CreatePersonalLight( INT32 sGridNo, UINT8 ubID )
{
	INT32 iLightIndex = NewLightEffectInternal( sGridNo, 0, 1 );

	if ( iLightIndex > -1 )
	{
		gLightEffectData[ iLightIndex ].ubOwner  = ubID;
		gLightEffectData[ iLightIndex ].flags	|= LIGHTEFFECT_FLASHLIGHT;
	}
}

void RemovePersonalLights( UINT8 ubID )
{
	LIGHTEFFECT *pLight;

	// Set to unallocated....
	for ( UINT32 cnt = 0; cnt < guiNumLightEffects; ++cnt )
	{
		pLight = &gLightEffectData[ cnt ];

		if ( pLight->iLight != (-1) && pLight->flags & LIGHTEFFECT_FLASHLIGHT && pLight->ubOwner == ubID && pLight->fAllocated )			
		{
			pLight->fAllocated = FALSE;
			pLight->flags &= ~LIGHTEFFECT_FLASHLIGHT;
			
			LightSpriteDestroy( pLight->iLight );
		}
	}
}
