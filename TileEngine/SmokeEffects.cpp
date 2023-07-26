#include "builddefines.h"

	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "weapons.h"
	#include "handle items.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "animation control.h"
	#include "tile animation.h"
	#include "handle items.h"
	#include "smokeeffects.h"
	#include "message.h"
	#include "isometric utils.h"
	#include "renderworld.h"
	#include "explosion control.h"
	#include "Random.h"
	#include "Game Clock.h"
	#include "opplist.h"
	#include "Campaign Types.h"
	#include "Tactical Save.h"
	#include "strategicmap.h"
	// sevenfm
	#include "environment.h"
	#include "Render Fun.h"

#include "SaveLoadGame.h"
#include "debug control.h"

#include "connect.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;



INT8 FromWorldFlagsToSmokeType( UINT16 ubWorldFlags );
UINT16 FromSmokeTypeToWorldFlags( INT8 bType );



// GLOBAL FOR SMOKE LISTING
SMOKEEFFECT				gSmokeEffectData[ NUM_SMOKE_EFFECT_SLOTS ];
UINT32						guiNumSmokeEffects = 0;


INT32 GetFreeSmokeEffect( void );
void RecountSmokeEffects( void );



INT32 GetFreeSmokeEffect( void )
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumSmokeEffects; uiCount++)
	{
		if(( gSmokeEffectData[uiCount].fAllocated==FALSE ) )
			return( (INT32)uiCount );
	}

	if( guiNumSmokeEffects < NUM_SMOKE_EFFECT_SLOTS )
		return( (INT32) guiNumSmokeEffects++ );

	return( -1 );
}

void RecountSmokeEffects( void )
{
	INT32 uiCount;

	for(uiCount=guiNumSmokeEffects-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gSmokeEffectData[uiCount].fAllocated ) )
		{
			guiNumSmokeEffects=(UINT32)(uiCount+1);
			break;
		}
	}
}




// Returns NO_SMOKE_EFFECT if none there...
INT8 GetSmokeEffectOnTile( INT32 sGridNo, INT8 bLevel )
{
	UINT16		ubExtFlags;

	ubExtFlags = gpWorldLevelData[ sGridNo ].ubExtFlags[ bLevel ];

	// Look at worldleveldata to find flags..
	if ( ubExtFlags & ANY_SMOKE_EFFECT )
	{
		// Which smoke am i?
		return( FromWorldFlagsToSmokeType( ubExtFlags ) );
	}

	return( NO_SMOKE_EFFECT );
}


INT8 FromWorldFlagsToSmokeType( UINT16 ubWorldFlags )
{
	if ( ubWorldFlags & MAPELEMENT_EXT_SMOKE )
	{
		return( NORMAL_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_TEARGAS )
	{
		return( TEARGAS_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_MUSTARDGAS )
	{
		return( MUSTARDGAS_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_CREATUREGAS )
	{
		return( CREATURE_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_BURNABLEGAS )
	{
		return( BURNABLEGAS_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_SIGNAL_SMOKE )
	{
		return( SIGNAL_SMOKE_EFFECT );
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_DEBRIS_SMOKE )
	{
		return(DEBRIS_SMOKE_EFFECT);
	}
	else if ( ubWorldFlags & MAPELEMENT_EXT_FIRERETARDANT_SMOKE )
	{
		return FIRERETARDANT_SMOKE_EFFECT;
	}
	
	return(NO_SMOKE_EFFECT);
}


UINT16 FromSmokeTypeToWorldFlags( INT8 bType )
{
	switch( bType )
	{
		case NORMAL_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_SMOKE );
			break;

		case TEARGAS_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_TEARGAS );
			break;

		case MUSTARDGAS_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_MUSTARDGAS );
			break;

		case BURNABLEGAS_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_BURNABLEGAS );
			break;

		case CREATURE_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_CREATUREGAS );
			break;

		case SIGNAL_SMOKE_EFFECT:

			return( MAPELEMENT_EXT_SIGNAL_SMOKE );
			break;

		case DEBRIS_SMOKE_EFFECT:
			return(MAPELEMENT_EXT_DEBRIS_SMOKE);
			break;

		case FIRERETARDANT_SMOKE_EFFECT:
			return MAPELEMENT_EXT_FIRERETARDANT_SMOKE;
			break;

		default:

			return( 0 );
	}
}



INT32 NewSmokeEffect( INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT16 ubOwner, BOOL fFromRemoteClient )
{
	SMOKEEFFECT *pSmoke;
	INT32		iSmokeIndex;
	INT8		bSmokeEffectType=0;

	if( ( iSmokeIndex = GetFreeSmokeEffect() )==(-1) )
		return(-1);

	// OJW - 20091027 - Syncronising smoke effect start for multiplayer
	if (is_networked && is_client)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[ubOwner];
		if (pSoldier != NULL)
		{
			if (pSoldier->bTeam == 0 || (pSoldier->bTeam == 1 && is_server))
			{
				// let all the other clients know we are spawning this effect
				// and align them with our random number generator
				send_newsmokeeffect(sGridNo,usItem,ubOwner,bLevel,iSmokeIndex);
			}
			else if (!fFromRemoteClient)
			{
				// skip executing locally because we want the random number generator to be aligned
				// with the client that spawns set off the smoke effect
				return -1;
			}
		}
#ifdef JA2BETAVERSION
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"NewSmokeEffect ( sGridNo : %i ,  usItem : %i , ubOwner : %i , bLevel : %i , iSmokeEffectID : %i )\n",sGridNo, usItem , ubOwner , bLevel , iSmokeIndex );
		MPDebugMsg(tmpMPDbgString);
		gfMPDebugOutputRandoms = true;
#endif
	}

	memset( &gSmokeEffectData[ iSmokeIndex ], 0, sizeof( SMOKEEFFECT ) );

	pSmoke = &gSmokeEffectData[ iSmokeIndex ];

	// Set some values...
	pSmoke->sGridNo									= sGridNo;
	pSmoke->usItem									= usItem;
	pSmoke->uiTimeOfLastUpdate			= GetWorldTotalSeconds( );

	// Are we indoors?
	// 0verhaul: Are we on the floor level?
	if ( GetTerrainType( sGridNo ) == FLAT_FLOOR && bLevel)
	{
		pSmoke->bFlags |= SMOKE_EFFECT_INDOORS;
	}


	switch( Explosive[Item[usItem].ubClassIndex].ubType )
	{
		case EXPLOSV_MUSTGAS:

			bSmokeEffectType	=	MUSTARDGAS_SMOKE_EFFECT;
			break;

		case EXPLOSV_BURNABLEGAS:

			bSmokeEffectType	=	BURNABLEGAS_SMOKE_EFFECT;
			break;

		case EXPLOSV_TEARGAS:
			bSmokeEffectType	=	TEARGAS_SMOKE_EFFECT;
			break;

		case EXPLOSV_SMOKE:

			bSmokeEffectType	=	NORMAL_SMOKE_EFFECT;
			break;

	// case SMALL_CREATURE_GAS:
			//bSmokeEffectType	=	CREATURE_SMOKE_EFFECT;
			//ubDuration				= 3;
			//ubStartRadius			= 1;
			//break;

		case EXPLOSV_CREATUREGAS:

			bSmokeEffectType	=	CREATURE_SMOKE_EFFECT;
			break;

	// case VERY_SMALL_CREATURE_GAS:

			//bSmokeEffectType	=	CREATURE_SMOKE_EFFECT;
			//ubDuration				= 2;
			//ubStartRadius			= 0;
			//break;

		case EXPLOSV_SIGNAL_SMOKE:

			bSmokeEffectType = SIGNAL_SMOKE_EFFECT;
			break;

		case EXPLOSV_SMOKE_DEBRIS:
			bSmokeEffectType = DEBRIS_SMOKE_EFFECT;
			break;

		case EXPLOSV_SMOKE_FIRERETARDANT:
			bSmokeEffectType = FIRERETARDANT_SMOKE_EFFECT;
			break;
	}

	pSmoke->ubDuration	= (UINT8)Explosive[ Item[ usItem ].ubClassIndex ].ubDuration;
	pSmoke->ubRadius	= (UINT8)Explosive[ Item[ usItem ].ubClassIndex ].ubStartRadius;
	pSmoke->bAge				= 0;
	pSmoke->fAllocated	= TRUE;
	pSmoke->bType				= bSmokeEffectType;
	pSmoke->ubOwner	 = ubOwner;

	if ( pSmoke->bFlags & SMOKE_EFFECT_INDOORS )
	{
		// Duration is increased by 2 turns...indoors
		pSmoke->ubDuration += 3;
	}
	else
	{
		switch ( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather )
		{
		case WEATHER_FORECAST_RAIN:				pSmoke->ubDuration = max( 0, pSmoke->ubDuration - 2 ); break;
		case WEATHER_FORECAST_THUNDERSHOWERS:	pSmoke->ubDuration = max( 0, pSmoke->ubDuration - 3 ); break;
		case WEATHER_FORECAST_SANDSTORM:		pSmoke->ubDuration = max( 0, pSmoke->ubDuration - 1 ); break;
		case WEATHER_FORECAST_SNOW:				pSmoke->ubDuration = max( 0, pSmoke->ubDuration - 1 ); break;
		}
	}

	if ( bLevel )
	{
		pSmoke->bFlags |= SMOKE_EFFECT_ON_ROOF;
	}

	// ATE: FALSE into subsequent-- it's the first one!
	SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, FALSE, bLevel, iSmokeIndex, fFromRemoteClient, TRUE );

	return( iSmokeIndex );
}

INT32 NewSmokeEffect(INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT16 ubOwner, BOOLEAN fFromRemoteClient, UINT8 ubDuration, UINT8 ubRadius, UINT8 ubGeneration)
{
	SMOKEEFFECT *pSmoke;
	INT32		iSmokeIndex;
	INT8		bSmokeEffectType = 0;

	iSmokeIndex = GetFreeSmokeEffect();

	if (iSmokeIndex == -1)
	{
		return -1;
	}	

	// OJW - 20091027 - Synchronizing smoke effect start for multiplayer
	if (is_networked && is_client)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[ubOwner];
		if (pSoldier != NULL)
		{
			if (pSoldier->bTeam == 0 || (pSoldier->bTeam == 1 && is_server))
			{
				// let all the other clients know we are spawning this effect
				// and align them with our random number generator
				send_newsmokeeffect(sGridNo, usItem, ubOwner, bLevel, iSmokeIndex);
			}
			else if (!fFromRemoteClient)
			{
				// skip executing locally because we want the random number generator to be aligned
				// with the client that spawns set off the smoke effect
				return -1;
			}
		}
#ifdef JA2BETAVERSION
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString, "NewSmokeEffect ( sGridNo : %i ,  usItem : %i , ubOwner : %i , bLevel : %i , iSmokeEffectID : %i )\n", sGridNo, usItem, ubOwner, bLevel, iSmokeIndex);
		MPDebugMsg(tmpMPDbgString);
		gfMPDebugOutputRandoms = true;
#endif
	}

	memset(&gSmokeEffectData[iSmokeIndex], 0, sizeof(SMOKEEFFECT));
	pSmoke = &gSmokeEffectData[iSmokeIndex];

	// Set some values...
	pSmoke->sGridNo = sGridNo;
	pSmoke->usItem = usItem;
	pSmoke->uiTimeOfLastUpdate = GetWorldTotalSeconds();	

	switch (Explosive[Item[usItem].ubClassIndex].ubType)
	{
	case EXPLOSV_MUSTGAS:
		bSmokeEffectType = MUSTARDGAS_SMOKE_EFFECT;
		break;
	case EXPLOSV_BURNABLEGAS:
		bSmokeEffectType = BURNABLEGAS_SMOKE_EFFECT;
		break;
	case EXPLOSV_TEARGAS:
		bSmokeEffectType = TEARGAS_SMOKE_EFFECT;
		break;
	case EXPLOSV_SMOKE:
		bSmokeEffectType = NORMAL_SMOKE_EFFECT;
		break;
	case EXPLOSV_CREATUREGAS:
		bSmokeEffectType = CREATURE_SMOKE_EFFECT;
		break;
	case EXPLOSV_SIGNAL_SMOKE:
		bSmokeEffectType = SIGNAL_SMOKE_EFFECT;
		break;
	case EXPLOSV_SMOKE_DEBRIS:
		bSmokeEffectType = DEBRIS_SMOKE_EFFECT;
		break;
	case EXPLOSV_SMOKE_FIRERETARDANT:
		bSmokeEffectType = FIRERETARDANT_SMOKE_EFFECT;
		break;
	}

	// sevenfm: use custom duration/radius
	if (ubDuration)
		pSmoke->ubDuration = ubDuration;
	else
		pSmoke->ubDuration = (UINT8)Explosive[Item[usItem].ubClassIndex].ubDuration;

	if (ubRadius)
		pSmoke->ubRadius = ubRadius;
	else
		pSmoke->ubRadius = (UINT8)Explosive[Item[usItem].ubClassIndex].ubStartRadius;

	pSmoke->bAge = 0;
	pSmoke->fAllocated = TRUE;
	pSmoke->bType = bSmokeEffectType;
	pSmoke->ubOwner = ubOwner;
	pSmoke->ubGeneration = ubGeneration;

	// Are we indoors?
	// sevenfm: check also roof above
	if (GetTerrainType(sGridNo) == FLAT_FLOOR && bLevel == 0 && FindStructure(sGridNo, STRUCTURE_ROOF) != NULL)
	{
		pSmoke->bFlags |= SMOKE_EFFECT_INDOORS;
	}

	if (pSmoke->bFlags & SMOKE_EFFECT_INDOORS)
	{
		// Duration is increased by 2 turns...indoors
		pSmoke->ubDuration += 2;
	}
	else
	{
		// limit smoke effects in the rain
		switch (SectorInfo[SECTOR(gWorldSectorX, gWorldSectorY)].usWeather)
		{
		case WEATHER_FORECAST_RAIN:				pSmoke->ubDuration = max(0, pSmoke->ubDuration - 2); break;
		case WEATHER_FORECAST_THUNDERSHOWERS:	pSmoke->ubDuration = max(0, pSmoke->ubDuration - 3); break;
		case WEATHER_FORECAST_SANDSTORM:		pSmoke->ubDuration = max(0, pSmoke->ubDuration - 1); break;
		case WEATHER_FORECAST_SNOW:				pSmoke->ubDuration = max(0, pSmoke->ubDuration - 1); break;
		}
	}

	if (bLevel)
	{
		pSmoke->bFlags |= SMOKE_EFFECT_ON_ROOF;
	}

	// ATE: FALSE into subsequent-- it's the first one!
	SpreadEffect(pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, FALSE, bLevel, iSmokeIndex, fFromRemoteClient, TRUE);

	return(iSmokeIndex);
}

// Add smoke to gridno
// ( Replacement algorithm uses distance away )
void AddSmokeEffectToTile( INT32 iSmokeEffectID, INT8 bType, INT32 sGridNo, INT8 bLevel )
{
#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"AddSmokeEffectToTile ( iSmokeEffectID : %i , bType : %i , sGridNo : %i , bLevel : %i )\n", iSmokeEffectID, bType , sGridNo , bLevel );
	MPDebugMsg(tmpMPDbgString);
#endif

	ANITILE_PARAMS	AniParams;
	ANITILE					*pAniTile;
	SMOKEEFFECT	 *pSmoke;
	BOOLEAN		 fDissipating = FALSE;

	pSmoke = &gSmokeEffectData[ iSmokeEffectID ];

	if ( ( pSmoke->ubDuration - pSmoke->bAge ) < 2 )
	{
	fDissipating = TRUE;
	// Remove old one...
	RemoveSmokeEffectFromTile( sGridNo, bLevel );
	}


	// If smoke effect exists already.... stop
	// silversurfer: Why? What if we throw another grenade of different type? Set at least the flag for the gas type...
	if ( gpWorldLevelData[ sGridNo ].ubExtFlags[ bLevel ] & ANY_SMOKE_EFFECT )
	{
		// Set world flags
		gpWorldLevelData[ sGridNo ].ubExtFlags[ bLevel ] |= FromSmokeTypeToWorldFlags( bType );
		return;
	}

	// OK,	Create anitile....
	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
	AniParams.sGridNo							= sGridNo;

	if ( bLevel == 0 )
	{
	AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
	}
	else
	{
	AniParams.ubLevelID						= ANI_ONROOF_LEVEL;
	}


	AniParams.sDelay							= (INT16)( 300 + Random( 300 ) );

	if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
	{
	AniParams.sStartFrame					= (INT16)0;
	}
	else
	{
	AniParams.sStartFrame					= (INT16)Random( 5 );
	}


	// Bare bones flags are...
//	AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_OPTIMIZEFORSMOKEEFFECT | ANITILE_SMOKE_EFFECT | ANITILE_LOOPING;
	//AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_SMOKE_EFFECT | ANITILE_LOOPING;


	if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
	{
	AniParams.uiFlags	= ANITILE_PAUSED | ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_SMOKE_EFFECT | ANITILE_LOOPING;
	}
	else
	{
	AniParams.uiFlags	= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_SMOKE_EFFECT | ANITILE_LOOPING | ANITILE_ALWAYS_TRANSLUCENT;
	}

	AniParams.sX									= CenterX( sGridNo );
	AniParams.sY									= CenterY( sGridNo );
	AniParams.sZ									= (INT16)0;

	// Use the right graphic based on type..
	switch( bType )
	{
		case NORMAL_SMOKE_EFFECT:

			if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\smkechze.STI" );
			}
			else
			{
				if ( fDissipating )
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\smalsmke.STI" );
				}
				else
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\SMOKE.STI" );
				}
			}
			break;

		case TEARGAS_SMOKE_EFFECT:

			if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\tearchze.STI" );
			}
			else
			{
				if ( fDissipating )
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\smaltear.STI" );
				}
				else
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\TEARGAS.STI" );
				}
			}
			break;

		case MUSTARDGAS_SMOKE_EFFECT:

			if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\mustchze.STI" );
			}
			else
			{
				if ( fDissipating )
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\smalmust.STI" );
				}
				else
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\MUSTARD2.STI" );
				}
			}
			break;

		case BURNABLEGAS_SMOKE_EFFECT:

			if ( !( gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\FLAMCHZE.STI" );
			}
			else
			{
				if ( fDissipating )
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\smalflam.STI" );
				}
				else
				{
					 strcpy( AniParams.zCachedFile, "TILECACHE\\FLAMETH2.STI" );
				}
			}
			break;

		case CREATURE_SMOKE_EFFECT:
			strcpy( AniParams.zCachedFile, "TILECACHE\\spit_gas.STI" );
			break;

		case SIGNAL_SMOKE_EFFECT:
			strcpy( AniParams.zCachedFile, "TILECACHE\\signal_gas.STI" );
			break;

		case DEBRIS_SMOKE_EFFECT:
			strcpy( AniParams.zCachedFile, "TILECACHE\\debris_smoke.STI" );
			break;

		case FIRERETARDANT_SMOKE_EFFECT:
			strcpy( AniParams.zCachedFile, "TILECACHE\\white_smoke.STI" );
			break;
	}

	// Create tile...
	pAniTile = CreateAnimationTile( &AniParams );

	// Set world flags
	gpWorldLevelData[ sGridNo ].ubExtFlags[ bLevel ] |= FromSmokeTypeToWorldFlags( bType );

	// All done...

	// Re-draw..... :(
	SetRenderFlags(RENDER_FLAG_FULL);
}

void RemoveSmokeEffectFromTile( INT32 sGridNo, INT8 bLevel )
{
#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"RemoveSmokeEffectFromTile ( sGridNo : %i , bLevel : %i  )\n", sGridNo, bLevel );
	MPDebugMsg(tmpMPDbgString);
#endif

	ANITILE *pAniTile;
	UINT8		ubLevelID;

	// Get ANI tile...
	if ( bLevel == 0 )
	{
		ubLevelID = ANI_STRUCT_LEVEL;
	}
	else
	{
		ubLevelID = ANI_ONROOF_LEVEL;
	}

	pAniTile = GetCachedAniTileOfType( sGridNo, ubLevelID, ANITILE_SMOKE_EFFECT );

	if ( pAniTile != NULL )
	{
		DeleteAniTile( pAniTile );

		SetRenderFlags( RENDER_FLAG_FULL );
	}

	// Unset flags in world....
	// ( // check to see if we are the last one....
	if ( GetCachedAniTileOfType( sGridNo, ubLevelID, ANITILE_SMOKE_EFFECT ) == NULL )
	{
		gpWorldLevelData[ sGridNo ].ubExtFlags[ bLevel ] &= ( ~ANY_SMOKE_EFFECT );
	}
}


void DecaySmokeEffects( UINT32 uiTime )
{
	SMOKEEFFECT *pSmoke;
	UINT32	cnt, cnt2;
	BOOLEAN fUpdate = FALSE;
	BOOLEAN fSpreadEffect;
	INT8	bLevel;
	UINT16	usNumUpdates = 1;

	for ( cnt = 0; cnt < guiNumMercSlots; ++cnt )
	{
		if ( MercSlots[ cnt ] )
		{
			// reset 'hit by gas' flags
			MercSlots[ cnt ]->flags.fHitByGasFlags = 0;
		}
	}

	// ATE: 1 ) make first pass and delete/mark any smoke effect for update
	// all the deleting has to be done first///

	// age all active tear gas clouds, deactivate those that are just dispersing
	for ( cnt = 0; cnt < guiNumSmokeEffects; ++cnt )
	{
		fSpreadEffect = TRUE;

		pSmoke = &gSmokeEffectData[ cnt ];

		if ( pSmoke->fAllocated )
		{
			if ( pSmoke->bFlags & SMOKE_EFFECT_ON_ROOF )
			{
				bLevel = 1;
			}
			else
			{
				bLevel = 0;
			}

			// Do things differently for combat /vs realtime
			// always try to update during combat
			if (gTacticalStatus.uiFlags & INCOMBAT )
			{
				fUpdate = TRUE;
			}
			else
			{
				// ATE: Do this every so ofte, to acheive the effect we want...
				if ( ( uiTime - pSmoke->uiTimeOfLastUpdate ) >= 10 )
				{
					fUpdate = TRUE;
					usNumUpdates = ( UINT16 ) ( ( uiTime - pSmoke->uiTimeOfLastUpdate ) / 10 );
				}
			}

			// Flugente: if tile has a fire retardant effect, don't create new fire
			if ( Explosive[Item[pSmoke->usItem].ubClassIndex].ubType == EXPLOSV_BURNABLEGAS )
			{
				if ( gpWorldLevelData[pSmoke->sGridNo].ubExtFlags[bLevel] & MAPELEMENT_EXT_FIRERETARDANT_SMOKE )
				{
					pSmoke->bAge = pSmoke->ubDuration + 1;
				}
			}

			if ( fUpdate )
			{
				pSmoke->uiTimeOfLastUpdate = uiTime;

				for ( cnt2 = 0; cnt2 < usNumUpdates; ++cnt2 )
				{
					pSmoke->bAge++;
	
					if ( pSmoke->bAge == 1 )
					{
						// ATE: At least mark for update!
						pSmoke->bFlags |= SMOKE_EFFECT_MARK_FOR_UPDATE;
						fSpreadEffect = FALSE;
					}
					else
					{
						fSpreadEffect = TRUE;
					}

					if ( fSpreadEffect )
					{
						// if this cloud remains effective (duration not reached)
						if ( pSmoke->bAge <= pSmoke->ubDuration)
						{
							// ATE: Only mark now and increse radius - actual drawing is done
							// in another pass cause it could
							// just get erased...
							pSmoke->bFlags |= SMOKE_EFFECT_MARK_FOR_UPDATE;

							// calculate the new cloud radius
							// cloud expands by 1 every turn outdoors, and every other turn indoors

							// ATE: If radius is < maximun, increase radius, otherwise keep at max
							if ( pSmoke->ubRadius < Explosive[ Item[ pSmoke->usItem ].ubClassIndex ].ubRadius )
							{
								pSmoke->ubRadius++;
							}
						}
						else
						{
							// deactivate tear gas cloud (use last known radius)
							SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, ERASE_SPREAD_EFFECT, bLevel, cnt );
							pSmoke->fAllocated = FALSE;
							break;
						}
					}
				}
			}
			else
			{
				// damage anyone standing in cloud
				SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, REDO_SPREAD_EFFECT, bLevel, cnt );
			}
		}
	}

	for ( cnt = 0; cnt < guiNumSmokeEffects; ++cnt )
	{
		pSmoke = &gSmokeEffectData[ cnt ];

		if ( pSmoke->fAllocated )
		{
			// if this cloud remains effective (duration not reached)
			if ( pSmoke->bFlags & SMOKE_EFFECT_MARK_FOR_UPDATE )
			{
				if ( pSmoke->bFlags & SMOKE_EFFECT_ON_ROOF )
				{
					bLevel = 1;
				}
				else
				{
					bLevel = 0;
				}

				SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, TRUE, bLevel, cnt );
				pSmoke->bFlags &= (~SMOKE_EFFECT_MARK_FOR_UPDATE);
			}
		}
	}

	AllTeamsLookForAll( TRUE );
}



BOOLEAN SaveSmokeEffectsToSaveGameFile( HWFILE hFile )
{
/*
	UINT32	uiNumBytesWritten;
	UINT32	uiCnt=0;
	UINT32	uiNumSmokeEffects=0;


	//loop through and count the number of smoke effects
	for( uiCnt=0; uiCnt<guiNumSmokeEffects; uiCnt++)
	{
		if( gSmokeEffectData[ uiCnt ].fAllocated )
			uiNumSmokeEffects++;
	}


	//Save the Number of Smoke Effects
	FileWrite( hFile, &uiNumSmokeEffects, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}


	if( uiNumSmokeEffects != 0 )
	{
		//loop through and save the number of smoke effects
		for( uiCnt=0; uiCnt < guiNumSmokeEffects; uiCnt++)
		{
			//if the smoke is active
			if( gSmokeEffectData[ uiCnt ].fAllocated )
			{
				//Save the Smoke effect Data
				FileWrite( hFile, &gSmokeEffectData[ uiCnt ], sizeof( SMOKEEFFECT ), &uiNumBytesWritten );
				if( uiNumBytesWritten != sizeof( SMOKEEFFECT ) )
				{
					return( FALSE );
				}
			}
		}
	}
*/
	return( TRUE );
}


BOOLEAN LoadSmokeEffectsFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT32	uiCount;
	UINT32	uiCnt=0;
	INT8	bLevel;

	//no longer need to load smoke effects.	They are now in temp files
	if( guiCurrentSaveGameVersion < 75 )
	{
		//Clear out the old list
		memset( gSmokeEffectData, 0, sizeof( SMOKEEFFECT ) * NUM_SMOKE_EFFECT_SLOTS );

		//Load the Number of Smoke Effects
		FileRead( hFile, &guiNumSmokeEffects, sizeof( UINT32 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT32 ) )
		{
			return( FALSE );
		}

		//This is a TEMP hack to allow us to use the saves
		if( guiCurrentSaveGameVersion < 37 && guiNumSmokeEffects == 0 )
		{
			//Load the Smoke effect Data
			FileRead( hFile, gSmokeEffectData, sizeof( SMOKEEFFECT ), &uiNumBytesRead );
			if( uiNumBytesRead != sizeof( SMOKEEFFECT ) )
			{
				return( FALSE );
			}
		}


		//loop through and load the list
		for( uiCnt=0; uiCnt<guiNumSmokeEffects;uiCnt++)
		{
			//Load the Smoke effect Data
			FileRead( hFile, &gSmokeEffectData[ uiCnt ], sizeof( SMOKEEFFECT ), &uiNumBytesRead );
			if( uiNumBytesRead != sizeof( SMOKEEFFECT ) )
			{
				return( FALSE );
			}
			//This is a TEMP hack to allow us to use the saves
			if( guiCurrentSaveGameVersion < 37 )
				break;
		}


		//loop through and apply the smoke effects to the map
		for(uiCount=0; uiCount < guiNumSmokeEffects; uiCount++)
		{
			//if this slot is allocated
			if( gSmokeEffectData[uiCount].fAllocated )
			{
		if ( gSmokeEffectData[uiCount].bFlags & SMOKE_EFFECT_ON_ROOF )
		{
			bLevel = 1;
		}
		else
		{
			bLevel = 0;
		}

				SpreadEffect( gSmokeEffectData[uiCount].sGridNo, gSmokeEffectData[uiCount].ubRadius, gSmokeEffectData[uiCount].usItem, gSmokeEffectData[uiCount].ubOwner, TRUE, bLevel, uiCount );
			}
		}
	}

	return( TRUE );
}


BOOLEAN SaveSmokeEffectsToMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiNumSmokeEffects=0;
	HWFILE	hFile;
	UINT32	uiNumBytesWritten=0;
	CHAR8		zMapName[ 128 ];
	UINT32	uiCnt;

	//get the name of the map
	GetMapTempFileName( SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//delete file the file.
	FileDelete( zMapName );

	//loop through and count the number of smoke effects
	for( uiCnt=0; uiCnt<guiNumSmokeEffects; uiCnt++)
	{
		if( gSmokeEffectData[ uiCnt ].fAllocated )
			uiNumSmokeEffects++;
	}

	//if there are no smoke effects
	if( uiNumSmokeEffects == 0 )
	{
		//set the fact that there are no smoke effects for this sector
		ReSetSectorFlag( sMapX, sMapY, bMapZ, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS );

		return( TRUE );
	}

	//Open the file for writing
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening file
		return( FALSE );
	}


	//Save the Number of Smoke Effects
	FileWrite( hFile, &uiNumSmokeEffects, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		//Close the file
		FileClose( hFile );

		return( FALSE );
	}


	//loop through and save the number of smoke effects
	for( uiCnt=0; uiCnt < guiNumSmokeEffects; uiCnt++)
	{
		//if the smoke is active
		if( gSmokeEffectData[ uiCnt ].fAllocated )
		{
			//Save the Smoke effect Data
			FileWrite( hFile, &gSmokeEffectData[ uiCnt ], sizeof( SMOKEEFFECT ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( SMOKEEFFECT ) )
			{
				//Close the file
				FileClose( hFile );

				return( FALSE );
			}
		}
	}

	//Close the file
	FileClose( hFile );

	SetSectorFlag( sMapX, sMapY, bMapZ, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS );

	return( TRUE );
}



BOOLEAN LoadSmokeEffectsFromMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiNumBytesRead;
	UINT32	uiCount;
	UINT32	uiCnt=0;
	HWFILE	hFile;
	CHAR8		zMapName[ 128 ];
	INT8	bLevel;

	GetMapTempFileName( SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//Open the file for reading, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}

	//Clear out the old list
	ResetSmokeEffects();


	//Load the Number of Smoke Effects
	FileRead( hFile, &guiNumSmokeEffects, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}

	//loop through and load the list
	for( uiCnt=0; uiCnt<guiNumSmokeEffects;uiCnt++)
	{
		//Load the Smoke effect Data
		FileRead( hFile, &gSmokeEffectData[ uiCnt ], sizeof( SMOKEEFFECT ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( SMOKEEFFECT ) )
		{
			FileClose( hFile );
			return( FALSE );
		}
	}


	//loop through and apply the smoke effects to the map
	for(uiCount=0; uiCount < guiNumSmokeEffects; uiCount++)
	{
		//if this slot is allocated
		if( gSmokeEffectData[uiCount].fAllocated )
		{
		if ( gSmokeEffectData[uiCount].bFlags & SMOKE_EFFECT_ON_ROOF )
		{
		bLevel = 1;
		}
		else
		{
		bLevel = 0;
		}

			SpreadEffect( gSmokeEffectData[uiCount].sGridNo, gSmokeEffectData[uiCount].ubRadius, gSmokeEffectData[uiCount].usItem, gSmokeEffectData[uiCount].ubOwner, TRUE, bLevel, uiCount );
		}
	}

	FileClose( hFile );

	return( TRUE );
}


void ResetSmokeEffects()
{
	//Clear out the old list
	memset( gSmokeEffectData, 0, sizeof( SMOKEEFFECT ) * NUM_SMOKE_EFFECT_SLOTS );
	guiNumSmokeEffects = 0;
}


void UpdateSmokeEffectGraphics( )
{
	UINT32		uiCnt;
	SMOKEEFFECT *pSmoke;
	INT8		bLevel;

	//loop through and save the number of smoke effects
	for( uiCnt=0; uiCnt < guiNumSmokeEffects; uiCnt++)
	{
		pSmoke = &gSmokeEffectData[ uiCnt ];

		//if the smoke is active
		if( gSmokeEffectData[ uiCnt ].fAllocated )
		{
			if ( gSmokeEffectData[uiCnt].bFlags & SMOKE_EFFECT_ON_ROOF )
			{
				bLevel = 1;
			}
			else
			{
				bLevel = 0;
			}

			SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, ERASE_SPREAD_EFFECT, bLevel, uiCnt );

			SpreadEffect( pSmoke->sGridNo, pSmoke->ubRadius, pSmoke->usItem, pSmoke->ubOwner, TRUE, bLevel, uiCnt );
		}
	}
}

BOOL GetRandomSignalSmokeGridNo(INT32* psGridNo)
{
	UINT32		uiCnt;
	//SMOKEEFFECT *pSmoke;
	//INT8		bLevel;

	INT32 smokearray[50];
	for (UINT8 i = 0; i < 50; ++i)
		smokearray[i] = -1;

	UINT8 cnt = 0;

	//loop through and save the number of smoke effects
	for( uiCnt=0; uiCnt < guiNumSmokeEffects; uiCnt++)
	{
		if ( gSmokeEffectData[ uiCnt ].fAllocated && gSmokeEffectData[ uiCnt ].bType == SIGNAL_SMOKE_EFFECT )
		{
			smokearray[cnt++] = uiCnt;

			if ( cnt >= 50 )
				break;
		}
	}

	if ( !cnt )
		return FALSE;

	UINT8 target = Random(cnt);

	(*psGridNo) = gSmokeEffectData[ smokearray[target] ].sGridNo;

	return TRUE;
}

// sevenfm: check smoke effect of certain type at gridno
BOOLEAN CheckSmokeEffect(INT32 sGridNo, INT8 bLevel, INT8 bType)
{
	UINT32	uiCnt;
	INT8	bSmokeEffectLevel;

	//loop through all smoke effects
	for (uiCnt = 0; uiCnt < guiNumSmokeEffects; uiCnt++)
	{
		if (gSmokeEffectData[uiCnt].fAllocated &&
			gSmokeEffectData[uiCnt].sGridNo == sGridNo &&
			gSmokeEffectData[uiCnt].bType == bType)
		{
			if (gSmokeEffectData[uiCnt].bFlags & SMOKE_EFFECT_ON_ROOF)
				bSmokeEffectLevel = 1;
			else
				bSmokeEffectLevel = 0;

			if (bSmokeEffectLevel == bLevel)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOLEAN FindVisibleSmokeEffect(INT8 bType)
{
	UINT32	uiCnt;
	INT8	bSmokeEffectLevel;

	//loop through all smoke effects
	for (uiCnt = 0; uiCnt < guiNumSmokeEffects; uiCnt++)
	{
		if (gSmokeEffectData[uiCnt].fAllocated &&
			gSmokeEffectData[uiCnt].bType == bType)
		{
			if (gSmokeEffectData[uiCnt].bFlags & SMOKE_EFFECT_ON_ROOF)
				bSmokeEffectLevel = 1;
			else
				bSmokeEffectLevel = 0;

			if (!TileIsOutOfBounds(gSmokeEffectData[uiCnt].sGridNo) &&
				GridNoOnScreen(gSmokeEffectData[uiCnt].sGridNo))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
