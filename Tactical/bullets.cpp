#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include "math.h"
	#include <stdio.h>
	#include <errno.h>

	#include "worlddef.h"
	#include "renderworld.h"
	#include "vsurface.h"
	#include "sysutil.h"
	#include "container.h"
	#include "wcheck.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "faces.h"
	#include "utilities.h"
	#include "overhead.h"
	#include "Soldier Profile.h"
	#include "Bullets.h"
	#include "los.h"
	#include "worldman.h"
	#include "random.h"
	#include "GameSettings.h"
	#include "FileMan.h"
	#include "lighting.h"
	#include "Buildings.h"
#endif

// Defines
// HEADROCK HAM 5: Increasing... with the hope of making spectacular fragmenting explosives.
#define		NUM_BULLET_SLOTS					1000		// Flugente: 200 -> 100


// GLOBAL FOR FACES LISTING
BULLET	gBullets[ NUM_BULLET_SLOTS ];
UINT32	guiNumBullets = 0;
BOOLEAN	fTracer = FALSE;

//afp-start
int gXPATH[BULLET_TRACER_MAX_LENGTH]; // positions between bullet
int gYPATH[BULLET_TRACER_MAX_LENGTH]; // positions between bullet
//afp-end

INT32 GetFreeBullet(void)
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumBullets; uiCount++)
	{
		if((gBullets[uiCount].fAllocated==FALSE) )
			return((INT32)uiCount);
	}

	if(guiNumBullets < NUM_BULLET_SLOTS )
		return((INT32)guiNumBullets++);

	return(-1);
}


void RecountBullets(void)
{
	INT32 uiCount;

	for(uiCount=guiNumBullets-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gBullets[uiCount].fAllocated ) )
		{
			guiNumBullets=(UINT32)(uiCount+1);
			return;
		}
	}
	guiNumBullets = 0;
}


INT32	CreateBullet( UINT8 ubFirerID, BOOLEAN fFake, UINT16 usFlags,UINT16 fromItem )
{
	INT32			iBulletIndex;
	BULLET		*pBullet;

	if( ( iBulletIndex = GetFreeBullet() )==(-1) )
		return(-1);

	memset(&gBullets[ iBulletIndex ], 0, sizeof( BULLET ) );

	pBullet = &gBullets[ iBulletIndex ];

	pBullet->iBullet = iBulletIndex;
	pBullet->fAllocated = TRUE;
	pBullet->fLocated		= FALSE;
	pBullet->ubFirerID	= ubFirerID;
	pBullet->usFlags		= usFlags;
	pBullet->usLastStructureHit = 0;
	pBullet->fromItem = fromItem;

	if (fFake)
	{
		pBullet->fReal = FALSE;
	}
	else
	{
		pBullet->fReal = TRUE;
//		gBullets[ iBullet ].pFirer->bBulletsLeft++;
		// HEADROCK HAM 5: Do not create for explosives.
		if (!(Item[fromItem].usItemClass & IC_EXPLOSV))
		{
			gTacticalStatus.ubAttackBusyCount++;
		}
		DebugAttackBusy( String( "Creating a new bullet for %d.	ABC now %d\n", ubFirerID, gTacticalStatus.ubAttackBusyCount) );
	}

	//afp-start each bullet carry its tail
	for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
		pBullet->pNodes[i] = NULL;
	//afp-end
	return( iBulletIndex );
}


void HandleBulletSpecialFlags( INT32 iBulletIndex )
{
	BULLET		*pBullet;
	ANITILE_PARAMS	AniParams;
	FLOAT		dX, dY;
	UINT8		ubDirection;

	pBullet = &( gBullets[ iBulletIndex ] );

	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );

	if ( pBullet->fReal )
	{
		// Create ani tile if this is a spit!
		if ( pBullet->usFlags & ( BULLET_FLAG_KNIFE ) )
		{
			AniParams.sGridNo							= pBullet->sGridNo;
			AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
			AniParams.sDelay							= 100;
			AniParams.sStartFrame					= 3;
			AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_LOOPING | ANITILE_USE_DIRECTION_FOR_START_FRAME;
			AniParams.sX									= FIXEDPT_TO_INT32( pBullet->qCurrX );
			AniParams.sY									= FIXEDPT_TO_INT32( pBullet->qCurrY );
			AniParams.sZ									= CONVERT_HEIGHTUNITS_TO_PIXELS( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );

			if ( pBullet->usFlags & ( BULLET_FLAG_CREATURE_SPIT ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\SPIT2.STI" );
			}
			else if ( pBullet->usFlags & ( BULLET_FLAG_KNIFE ) )
			{
				strcpy( AniParams.zCachedFile, "TILECACHE\\KNIFING.STI" );

				if ( pBullet->ubFirerID != NOBODY )
					pBullet->ubItemStatus = pBullet->pFirer->inv[ HANDPOS ][0]->data.objectStatus;
				else
					pBullet->ubItemStatus = 100;
			}

			// Get direction to use for this guy....
			dX = ( (FLOAT)( pBullet->qIncrX ) / FIXEDPT_FRACTIONAL_RESOLUTION );
			dY = ( (FLOAT)( pBullet->qIncrY ) / FIXEDPT_FRACTIONAL_RESOLUTION );

			ubDirection = atan8( 0, 0, (INT16)( dX * 100 ), (INT16)( dY * 100 ) );

			AniParams.uiUserData3					= ubDirection;

			pBullet->pAniTile = CreateAnimationTile( &AniParams );

			// IF we are anything that needs a shadow.. set it here....
			if ( pBullet->usFlags & ( BULLET_FLAG_KNIFE ) )
			{
				AniParams.ubLevelID						= ANI_SHADOW_LEVEL;
				AniParams.sZ									= 0;
				pBullet->pShadowAniTile				= CreateAnimationTile( &AniParams );
			}

		}
	}
}


void RemoveBullet( INT32 iBullet )
{
	CHECKV( iBullet < NUM_BULLET_SLOTS );

	//afp-start
	// remove any tail first if exists
	for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
		if (gBullets[iBullet].pNodes[i] != NULL)
		{
			RemoveStructFromLevelNode(gBullets[ iBullet ].sGridNo, gBullets[iBullet].pNodes[i]);
			gBullets[iBullet].pNodes[i] = NULL;
		}
	//afp-end
	// decrease soldier's bullet count

	if (gBullets[ iBullet ].fReal)
	{
		// set to be deleted at next update
		gBullets[ iBullet ].fToDelete = TRUE;

		// decrement reference to bullet in the firer
//		gBullets[ iBullet ].pFirer->bBulletsLeft--;
//		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Ending bullet, bullets left %d", gBullets[ iBullet ].pFirer->bBulletsLeft ) );
//		DebugAttackBusy( String( "Deleting a bullet for %d.	Total count now %d\n", gBullets[ iBullet].ubFirerID, gBullets[ iBullet ].pFirer->bBulletsLeft) );
		// Nah, just decrement the attack busy count and be done with it
		DebugAttackBusy( String( "Deleting a bullet for %d.\n", gBullets[ iBullet].ubFirerID ) );
		// HEADROCK HAM 5: Fragments do not need reducing.
		if (gBullets[iBullet].fFragment == false)
		{
			ReduceAttackBusyCount( );
		}

		// if ( gBullets[ iBullet ].usFlags & ( BULLET_FLAG_KNIFE ) )
		// {
		// Delete ani tile
		if ( gBullets[ iBullet ].pAniTile != NULL )
		{
			DeleteAniTile( gBullets[ iBullet ].pAniTile );
			gBullets[ iBullet ].pAniTile = NULL;
		}

		// Delete shadow
		// if ( gBullets[ iBullet ].usFlags & ( BULLET_FLAG_KNIFE ) )
		// {
		if ( gBullets[ iBullet ].pShadowAniTile != NULL )
		{
			DeleteAniTile( gBullets[ iBullet ].pShadowAniTile );
			gBullets[ iBullet ].pShadowAniTile = NULL;
		}
//	}
//		}
	}
	else
	{
		// delete this fake bullet right away!
		gBullets[ iBullet ].fAllocated = FALSE;
		RecountBullets();
	}
}

void LocateBullet( INT32 iBulletIndex )
{
	// HEADROCK HAM 5: Do not track fragments. There are too many of them.
	if ( gGameSettings.fOptions[ TOPTION_SHOW_MISSES ] && gBullets[ iBulletIndex ].fFragment == false)
	{
	// Check if a bad guy fired!
	if ( gBullets[ iBulletIndex ].ubFirerID != NOBODY )
	{
		if ( MercPtrs[ gBullets[ iBulletIndex ].ubFirerID ]->bSide == gbPlayerNum )
		{
			if ( !gBullets[ iBulletIndex ].fLocated )
			{
				gBullets[ iBulletIndex ].fLocated = TRUE;

				//Only if we are in turnbased and noncombat
				if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
				{
					  LocateGridNo( gBullets[ iBulletIndex ].sGridNo );
				}
			}
		}
	}
	}
}


void UpdateBullets( )
{
	UINT32					uiCount;
	LEVELNODE				*pNode;
	BOOLEAN					fDeletedSome = FALSE;

	for ( uiCount = 0; uiCount < guiNumBullets; uiCount++ )
	{
		if ( gBullets[ uiCount ].fAllocated)
		{
			if (gBullets[ uiCount ].fReal && !( gBullets[ uiCount ].usFlags & BULLET_STOPPED ) )
			{
				// there are duplicate checks for deletion in case the bullet is deleted by shooting
				// someone at point blank range, in the first MoveBullet call in the FireGun code
				if ( gBullets[ uiCount ].fToDelete )
				{
					// Remove from old position
					gBullets[ uiCount ].fAllocated = FALSE;
					fDeletedSome = TRUE;
					continue;
				}

				//if ( !( gGameSettings.fOptions[ TOPTION_HIDE_BULLETS ] ) )
				{
					// ALRIGHTY, CHECK WHAT TYPE OF BULLET WE ARE

					if ( gBullets[ uiCount ].usFlags & ( BULLET_FLAG_CREATURE_SPIT | BULLET_FLAG_KNIFE | BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME | BULLET_FLAG_WHITESMOKE /*| BULLET_FLAG_TRACER*/ ) )
					{
					}
					else
					{
						RemoveStruct( gBullets[ uiCount ].sGridNo, BULLETTILE1 );
						RemoveStruct( gBullets[ uiCount ].sGridNo, BULLETTILE2 );
					}
				}
				//afp-start
				// remove old tail first if exists
				FIXEDPT lastX = gBullets[uiCount].qCurrX;
				FIXEDPT lastY = gBullets[uiCount].qCurrY;
				FIXEDPT lastZ = gBullets[uiCount].qCurrZ;
				for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
					if (gBullets[uiCount].pNodes[i] != NULL)
					{
						RemoveStructFromLevelNode(gBullets[ uiCount ].sGridNo, gBullets[uiCount].pNodes[i]);
						gBullets[uiCount].pNodes[i] = NULL;
					}
				//afp-end

				MoveBullet( uiCount );
				if ( gBullets[ uiCount ].fToDelete )
				{
					// Remove from old position
					gBullets[ uiCount ].fAllocated = FALSE;
					fDeletedSome = TRUE;
					continue;
				}

				if ( gBullets[ uiCount ].usFlags & BULLET_STOPPED )
				{
					continue;
				}

				// Display bullet
				//if ( !( gGameSettings.fOptions[ TOPTION_HIDE_BULLETS ] ) )
				{
					if ( gBullets[ uiCount ].usFlags & ( BULLET_FLAG_KNIFE ) )
					{
						if ( gBullets[ uiCount ].pAniTile != NULL )
						{
							gBullets[ uiCount ].pAniTile->sRelativeX	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrX );
							gBullets[ uiCount ].pAniTile->sRelativeY	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrY );
							gBullets[ uiCount ].pAniTile->pLevelNode->sRelativeZ	= (INT16) CONVERT_HEIGHTUNITS_TO_PIXELS( FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrZ ) );

							if ( gBullets[ uiCount ].usFlags & ( BULLET_FLAG_KNIFE ) )
							{
								gBullets[ uiCount ].pShadowAniTile->sRelativeX	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrX );
								gBullets[ uiCount ].pShadowAniTile->sRelativeY	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrY );
							}

						}
					}
					// Are we a missle?
					else if ( gBullets[ uiCount ].usFlags & ( BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME | BULLET_FLAG_WHITESMOKE | BULLET_FLAG_CREATURE_SPIT ) )
					{
					}
					/*
					else if ( gBullets[ uiCount ].usFlags & ( BULLET_FLAG_TRACER ) )
					{
						ManLooksForOtherTeams(gBullets[ uiCount ].pFirer);
					}
					*/
					/*
					else if (fTracer == TRUE)
					{
						ManLooksForOtherTeams(gBullets[ uiCount ].pFirer);
					}
					*/
					
					else
					{
						pNode = AddStructToTail( gBullets[ uiCount ].sGridNo, BULLETTILE1 );
						pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->uiFlags |= ( LEVELNODE_USEABSOLUTEPOS | LEVELNODE_IGNOREHEIGHT );
						pNode->sRelativeX	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrX );
						pNode->sRelativeY	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrY );
						pNode->sRelativeZ = (INT16) CONVERT_HEIGHTUNITS_TO_PIXELS( FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrZ ) );

						//afp-start - add new tail /tracer
						// HEADROCK HAM 5: No tail for fragments.				
						if (gGameSettings.fOptions[TOPTION_ALTERNATE_BULLET_GRAPHICS] && gBullets[ uiCount ].fFragment == false)	
						{
  							if ((lastX != 0)  || (lastY != 0))
							{
								// qIncrX can be used to calculate slope and make the tracer longer if necessary
								PointsPath((INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrX),
									(INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrY), 
									(INT16) FIXEDPT_TO_INT32( lastX), 
									(INT16) FIXEDPT_TO_INT32( lastY));

								// compute valid points allong the fire line
								int pointsCount = 0;
								for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
								{	
									pointsCount = i;
									if (gXPATH[i] == 0)
										if (gYPATH[i] == 0)
											break;
								}
								if (pointsCount <= 0)
									pointsCount = 30;


								for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
								{
									if (gXPATH[i] == 0)
										if (gYPATH[i] == 0)
											break;
									
									// add all points along the path between bullets as bullets
									pNode = AddStructToTail( gBullets[ uiCount ].sGridNo, BULLETTILE1 );
									pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
									pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
									pNode->uiFlags |= ( LEVELNODE_USEABSOLUTEPOS | LEVELNODE_IGNOREHEIGHT );
									pNode->sRelativeX	= gXPATH[i];
									pNode->sRelativeY	= gYPATH[i];
									FIXEDPT relativeZ = lastZ - ((i + 1) * ((gBullets[ uiCount ].qCurrZ - lastZ) / (pointsCount)));
									pNode->sRelativeZ = (INT16) CONVERT_HEIGHTUNITS_TO_PIXELS( FIXEDPT_TO_INT32( relativeZ));
									
									// store structure pointer to clear image at the next bullet position
									gBullets[uiCount].pNodes[i] = pNode;
								}
							}
						}
						//afp-end
						// Display shadow
						// afp - no more shadow if tracer enabled
						if (!gGameSettings.fOptions[TOPTION_ALTERNATE_BULLET_GRAPHICS])	
						{
							pNode = AddStructToTail( gBullets[ uiCount ].sGridNo, BULLETTILE2 );
							pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
							pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
							pNode->uiFlags |= ( LEVELNODE_USEABSOLUTEPOS | LEVELNODE_IGNOREHEIGHT );
							pNode->sRelativeX	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrX );
							pNode->sRelativeY	= (INT16) FIXEDPT_TO_INT32( gBullets[ uiCount ].qCurrY );
							pNode->sRelativeZ = (INT16)gpWorldLevelData[ gBullets[ uiCount ].sGridNo ].sHeight;
						}
					}
				}
			}
			else
			{
				if ( gBullets[ uiCount ].fToDelete )
				{
					gBullets[ uiCount ].fAllocated = FALSE;
					fDeletedSome = TRUE;
				}
			}
		}
	}

	if ( fDeletedSome )
	{
		RecountBullets( );
	}
}



BULLET *GetBulletPtr( INT32 iBullet )
{
	BULLET	*pBullet;

	CHECKN( iBullet < NUM_BULLET_SLOTS );

	pBullet = &gBullets[ iBullet ];

	return( pBullet );
}


void AddMissileTrail( BULLET *pBullet, FIXEDPT qCurrX, FIXEDPT qCurrY, FIXEDPT qCurrZ )
{
	ANITILE_PARAMS	AniParams;

	// If we are a small missle, don't show
	if ( pBullet->usFlags & ( BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_FLAME | BULLET_FLAG_CREATURE_SPIT | BULLET_FLAG_WHITESMOKE /*| BULLET_FLAG_TRACER*/ ) )
	{
		if ( pBullet->iLoop < 5 )
		{
			return;
		}
	}

	// HEADROCK HAM B2.5: Created new bullet flag that tells us whether this specific bullet is a tracer.
	// The condition now reads that flag and creates a lightshow only for tracer bullets. This flag is only
	// used if the new Tracer System is on.
	//if (fTracer == TRUE)
	if ((gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && pBullet->fTracer == TRUE) || (gGameExternalOptions.ubRealisticTracers == 0 && fTracer == TRUE))
	{
		if ( pBullet->iLoop < 5 )
		{
			return;
		}
	}

	// If we are a small missle, don't show
	if ( pBullet->usFlags & ( BULLET_FLAG_TANK_CANNON ) )
	{
		//if ( pBullet->iLoop < 40 )
		//{
			return;
		//}
	}


	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
	AniParams.sGridNo							= pBullet->sGridNo;
	AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
	AniParams.sDelay							= (INT16)( 100 + Random( 100 ) );
	AniParams.sStartFrame					= 0;
	AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
	AniParams.sX									= FIXEDPT_TO_INT32( qCurrX );
	AniParams.sY									= FIXEDPT_TO_INT32( qCurrY );
	AniParams.sZ									= CONVERT_HEIGHTUNITS_TO_PIXELS( FIXEDPT_TO_INT32( qCurrZ ) );


	if ( pBullet->usFlags & ( BULLET_FLAG_MISSILE | BULLET_FLAG_TANK_CANNON ) )
	{
		strcpy( AniParams.zCachedFile, "TILECACHE\\MSLE_SMK.STI" );
	}
	else if ( pBullet->usFlags & ( BULLET_FLAG_SMALL_MISSILE ) )
	{
		strcpy( AniParams.zCachedFile, "TILECACHE\\MSLE_SMA.STI" );
	}
	else if ( pBullet->usFlags & ( BULLET_FLAG_CREATURE_SPIT ) )
	{
		strcpy( AniParams.zCachedFile, "TILECACHE\\MSLE_SPT.STI" );
	}
	else if ( pBullet->usFlags & ( BULLET_FLAG_FLAME ) )
	{
		strcpy( AniParams.zCachedFile, "TILECACHE\\FLMTHR2.STI" );
		AniParams.sDelay							= (INT16)( 100 );
	}
	else if ( pBullet->usFlags & BULLET_FLAG_WHITESMOKE )
	{
		strcpy( AniParams.zCachedFile, "TILECACHE\\white_smoke_trail.sti" );
		AniParams.sDelay = (INT16)( 100 );
	}
	//else if ( pBullet->usFlags & ( BULLET_FLAG_TRACER ) )
	// HEADROCK HAM B2.5: Created new bullet flag that tells us whether this specific bullet is a tracer.
	// The condition now reads that flag and creates a lightshow only for tracer bullets. This flag is only
	// used if the new Tracer System is on.
	// else if (fTracer == TRUE)
	else if ((gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && pBullet->fTracer == TRUE) || (gGameExternalOptions.ubRealisticTracers == 0 && fTracer == TRUE))
	{
		INT16 sXPos, sYPos;

		strcpy( AniParams.zCachedFile, "TILECACHE\\BULLET_TRACER.STI" );
		AniParams.uiFlags |= ANITILE_LIGHT;
		AniParams.sDelay							= 10000; // Test this out

		if (!pBullet->pAniTile)
		{
			pBullet->pAniTile = CreateAnimationTile( &AniParams );
		}

		ConvertGridNoToCenterCellXY( pBullet->sGridNo, &sXPos, &sYPos );
		LightSpritePosition( pBullet->pAniTile->lightSprite, (INT16)(sXPos/CELL_X_SIZE), (INT16)(sYPos/CELL_Y_SIZE));

#if 0
		if ( pBullet->pFirer->pathing.bLevel > 0 ) // if firer on roof then
		{
			if ( FindBuilding(AniParams.sGridNo) != NULL ) // if this spot is still within the building's grid area
			{
				LightSpritePower( pBullet->pAniTile->lightSprite, FALSE);
			}
		}
#endif

		return;
	}

	CreateAnimationTile( &AniParams );

}


BOOLEAN SaveBulletStructureToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	UINT16	usCnt;
	UINT32	uiBulletCount=0;

	//loop through and count the number of bullets
	for( usCnt=0; usCnt<NUM_BULLET_SLOTS; usCnt++ )
	{
		//if the bullet is active, save it
		if( gBullets[ usCnt ].fAllocated )
		{
			uiBulletCount++;
		}
	}

	//Save the number of Bullets in the array
	FileWrite( hFile, &uiBulletCount, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	if( uiBulletCount != 0 )
	{
		for( usCnt=0; usCnt<NUM_BULLET_SLOTS; usCnt++ )
		{
			//if the bullet is active, save it
			if( gBullets[ usCnt ].fAllocated )
			{
				//Save the the Bullet structure
				FileWrite( hFile, &gBullets[usCnt], sizeof( BULLET ), &uiNumBytesWritten );
				if( uiNumBytesWritten != sizeof( BULLET ) )
				{
					return( FALSE );
				}
			}
		}
	}

	return( TRUE );
}


BOOLEAN LoadBulletStructureFromSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT16	usCnt;

	//make sure the bullets are not allocated
	memset( gBullets, 0, NUM_BULLET_SLOTS * sizeof( BULLET ) );

	//Load the number of Bullets in the array
	FileRead( hFile, &guiNumBullets, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	for( usCnt=0; usCnt<guiNumBullets; usCnt++ )
	{
		//Load the the Bullet structure
		FileRead( hFile, &gBullets[usCnt], sizeof( BULLET ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( BULLET ) )
		{
			return( FALSE );
		}

		//Set some parameters
		gBullets[usCnt].uiLastUpdate = 0;
		if( gBullets[usCnt].ubFirerID != NOBODY )
			gBullets[usCnt].pFirer = &Menptr[ gBullets[usCnt].ubFirerID ];
		else
			gBullets[usCnt].pFirer = NULL;

		gBullets[usCnt].pAniTile = NULL;
		gBullets[usCnt].pShadowAniTile = NULL;
		gBullets[usCnt].iBullet = usCnt;

		HandleBulletSpecialFlags( gBullets[usCnt].iBullet );
	}

	return( TRUE );
}

void StopBullet( INT32 iBullet )
{
	gBullets[ iBullet ].usFlags |= BULLET_STOPPED;

	RemoveStruct( gBullets[ iBullet ].sGridNo, BULLETTILE1 );
	RemoveStruct( gBullets[ iBullet ].sGridNo, BULLETTILE2 );
}


void DeleteAllBullets( )
{
	UINT32					uiCount;

	for ( uiCount = 0; uiCount < guiNumBullets; uiCount++ )
	{
		if ( gBullets[ uiCount ].fAllocated)
		{
			// Remove from old position
			RemoveBullet( uiCount );
			gBullets[ uiCount ].fAllocated = FALSE;
		}
	}

	RecountBullets( );

}
//afp-start adapted function, no much time for cosmetics 
void PointsPath(int sx1, int sy1, int ex2, int ey2) 
{ 
	for (int i = 0; i < BULLET_TRACER_MAX_LENGTH; i++)
	{
		gXPATH[i] = 0;
		gYPATH[i] = 0;
	}

	int counter = 0;
	gXPATH[counter] = sx1;
	gYPATH[counter] = sy1;

	int x0 = sx1; 
	int y0 = sy1; 
	int x1 = ex2; 
	int y1 = ey2; 
	int dy = y1 - y0; 
	int dx = x1 - x0; 
	int stepx, stepy; 
	int gridX, gridY; 

	if (dy < 0) 
	{ 
		dy = -dy; 
		stepy = -1; 
	} 
	else 
		stepy = 1; 

	if (dx < 0) 
	{ 
		dx = -dx; 
		stepx = -1; 
	} 
	else 
		stepx = 1; 

	dy <<= 1; 
	dx <<= 1; 
	gridX = x0 >> 3; 
	gridY = y0 >> 3; 

	if ((sx1==ex2) &&(sy1=ey2))
		return;

	if (dx > dy) 
	{ 
		int fraction = dy - (dx >> 1); 
		while (x0 != x1) 
		{ 
			if (fraction >= 0) 
			{ 
				y0 += stepy; 
				fraction -= dx; 
			} 
			x0 += stepx; 
			fraction += dy; 

			counter++;
			if (counter >= BULLET_TRACER_MAX_LENGTH)
				return;
			gXPATH[counter] = x0;
			gYPATH[counter] = y0;
		} 
	} 
	else 
	{ 
		int fraction = dx - (dy >> 1); 
		while (y0 != y1) 
		{ 
			if (fraction >= 0) 
			{ 
				x0 += stepx; 
				fraction -= dy; 
			} 
			y0 += stepy; 
			fraction += dx; 
			counter++;
			if (counter >= BULLET_TRACER_MAX_LENGTH)
				return;
			gXPATH[counter] = x0;
			gYPATH[counter] = y0;
		} 
	} 
	return;
}
//afp-end
