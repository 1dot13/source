#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include <string.h>

	#include "types.h"
	#include "wcheck.h"
	#include "stdlib.h"
	#include "debug.h"
	#include "Animation Cache.h"
	#include "Animation Data.h"
	#include "Animation Control.h"
	#include "Debug Control.h"
#endif

#define EMPTY_CACHE_ENTRY		65000

UINT32 guiCacheSize		= MIN_CACHE_SIZE;

void DetermineOptimumAnimationCacheSize( )
{
	// If we have lots-a memory, adjust accordingly!
	guiCacheSize = MIN_CACHE_SIZE;
}

BOOLEAN InitAnimationCache( UINT16 usSoldierID, AnimationSurfaceCacheType *pAnimCache )
{
	UINT32 cnt;

	// Allocate entries
	AnimDebugMsg( String( "*** Initializing anim cache surface for soldier %d", usSoldierID ) );
	pAnimCache->usCachedSurfaces = (UINT16 *) MemAlloc( sizeof( UINT16 ) * guiCacheSize );
	CHECKF( pAnimCache->usCachedSurfaces!= NULL );

	AnimDebugMsg( String( "*** Initializing anim cache hit counter for soldier %d", usSoldierID ) );
	pAnimCache->sCacheHits = (INT16 *) MemAlloc( sizeof( UINT16) * guiCacheSize );
	CHECKF( pAnimCache->sCacheHits!= NULL );

	// Zero entries
	for ( cnt = 0; cnt < guiCacheSize; cnt++ )
	{
		pAnimCache->usCachedSurfaces[ cnt ] = EMPTY_CACHE_ENTRY;
		pAnimCache->sCacheHits[ cnt ] = 0;
	}
	pAnimCache->ubCacheSize = 0;

	// Zero surface databse history for this soldeir
	ClearAnimationSurfacesUsageHistory( usSoldierID );

	return( TRUE );
}

void DeleteAnimationCache( UINT16 usSoldierID, AnimationSurfaceCacheType *pAnimCache )
{
	// Allocate entries
	if ( pAnimCache->usCachedSurfaces!= NULL )
	{
		AnimDebugMsg( String( "*** Removing Anim Cache surface for soldier %d", usSoldierID ) );
		MemFree( pAnimCache->usCachedSurfaces );
	}

	if ( pAnimCache->sCacheHits != NULL )
	{
		AnimDebugMsg( String( "*** Removing Anim Cache hit counter for soldier %d", usSoldierID ) );
		MemFree( pAnimCache->sCacheHits );
	}
}


BOOLEAN GetCachedAnimationSurface( UINT16 usSoldierID, AnimationSurfaceCacheType *pAnimCache, UINT16 usSurfaceIndex, UINT16 usCurrentAnimation )
{
	UINT8			cnt;
	UINT8			ubLowestIndex = 0;
	INT16		sMostHits = (INT16)32000;
	UINT16		usCurrentAnimSurface;

	// Check to see if surface exists already
	for ( cnt = 0; cnt < pAnimCache->ubCacheSize; cnt++ )
	{
		if ( pAnimCache->usCachedSurfaces[ cnt ] == usSurfaceIndex )
		{
			// Found surface, return
			AnimDebugMsg( String( "Anim Cache: Hit %d ( Soldier %d )", usSurfaceIndex, usSoldierID ) );
			pAnimCache->sCacheHits[cnt]++;
			return( TRUE );
		}
	}

	// Check if max size has been reached
	if ( pAnimCache->ubCacheSize == guiCacheSize )
	{
		AnimDebugMsg( String( "Anim Cache: Determining Bump Candidate ( Soldier %d )", usSoldierID ) );

		// Determine exisiting surface used by merc
		usCurrentAnimSurface = DetermineSoldierAnimationSurface( MercPtrs[ usSoldierID ], usCurrentAnimation );
		// If the surface we are going to bump is our existing animation, reject it as a candidate

		// If we get here, we need to remove an animation, pick the best one
		// Loop through and pick one with lowest cache hits
		for ( cnt = 0; cnt < pAnimCache->ubCacheSize; cnt++ )
		{
			AnimDebugMsg( String( "Anim Cache: Slot %d Hits %d ( Soldier %d )", cnt, pAnimCache->sCacheHits[ cnt ], usSoldierID ) );

			if ( pAnimCache->usCachedSurfaces[ cnt ] == usCurrentAnimSurface )
			{
				AnimDebugMsg( String( "Anim Cache: REJECTING Slot %d EXISTING ANIM SURFACE ( Soldier %d )", cnt, usSoldierID ) );
			}
			else
			{
				if ( pAnimCache->sCacheHits[ cnt ] < sMostHits )
				{
						sMostHits = pAnimCache->sCacheHits[ cnt ];
						ubLowestIndex = cnt;
				}
			}
		}

		// Bump off lowest index
		AnimDebugMsg( String( "Anim Cache: Bumping %d ( Soldier %d )", ubLowestIndex, usSoldierID ) );
		UnLoadAnimationSurface( usSoldierID, pAnimCache->usCachedSurfaces[ ubLowestIndex ] );

		// Decrement
		pAnimCache->sCacheHits[ ubLowestIndex ] = 0;
		pAnimCache->usCachedSurfaces[ ubLowestIndex ] = EMPTY_CACHE_ENTRY;
		pAnimCache->ubCacheSize--;

	}

	// If here, Insert at an empty slot
	// Find an empty slot
	for ( cnt = 0; cnt < guiCacheSize; cnt++ )
	{
		if ( pAnimCache->usCachedSurfaces[ cnt ] == EMPTY_CACHE_ENTRY )
		{
			AnimDebugMsg( String( "Anim Cache: Loading Surface %d ( Soldier %d )", usSurfaceIndex, usSoldierID ) );

			// Insert here
			CHECKF( LoadAnimationSurface( usSoldierID, usSurfaceIndex, usCurrentAnimation ) != FALSE );
			pAnimCache->sCacheHits[ cnt ] = 0;
			pAnimCache->usCachedSurfaces[ cnt ] = usSurfaceIndex;
			pAnimCache->ubCacheSize++;

			break;
		}
	}

	return( TRUE );
	}



void UnLoadCachedAnimationSurfaces( UINT16 usSoldierID, AnimationSurfaceCacheType *pAnimCache )
{
	UINT8			cnt;

	// Check to see if surface exists already
	for ( cnt = 0; cnt < pAnimCache->ubCacheSize; cnt++ )
	{
		if ( pAnimCache->usCachedSurfaces[ cnt ] != EMPTY_CACHE_ENTRY )
		{
			UnLoadAnimationSurface( usSoldierID, pAnimCache->usCachedSurfaces[ cnt ] );
		}
	}

}
