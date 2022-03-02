#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "physics.h"
	#include "wcheck.h"
	#include "isometric utils.h"
	#include "worldman.h"
	#include "Sound Control.h"
	#include "interface.h"
	#include "interface items.h"
	#include "explosion control.h"
	#include "Debug Control.h"
	#include "message.h"
	#include "structure wrap.h"
	#include "animation control.h"
	#include "text.h"
	#include "Random.h"
	#include "lighteffects.h"
	#include "opplist.h"
	#include "Buildings.h"
	#include "Dialogue Control.h"	// added by Flugente
	#include "Map Information.h"	// added by Shadooow
#endif
#include "connect.h"
#include "PATHAI.H"


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
extern INT16 EffectiveDexterity(SOLDIERTYPE* pSoldier, BOOLEAN fTrainer);
extern bool GridNoOnWalkableWorldTile(INT32 sGridNo);

#define NO_TEST_OBJECT												0
#define TEST_OBJECT_NO_COLLISIONS							1
#define TEST_OBJECT_ANY_COLLISION							2
#define TEST_OBJECT_NOTWALLROOF_COLLISIONS		3

#define OUTDOORS_START_ANGLE									(FLOAT)( PI/4 )
#define INDOORS_START_ANGLE										(FLOAT)( PI/30 )
//#define INDOORS_START_ANGLE									(FLOAT)( 0 )
#define	GLAUNCHER_START_ANGLE									(FLOAT)( PI/8 )
#define	GLAUNCHER_HIGHER_LEVEL_START_ANGLE						(FLOAT)( PI/6 )

#define GET_THROW_HEIGHT( l )						(INT16)( ( l * 256 ) )
//#define GET_SOLDIER_THROW_HEIGHT( l )		(INT16)( ( l * 256 ) + STANDING_HEIGHT )
// ATE: Commented out part to provide a more accurate start height
#define GET_SOLDIER_THROW_HEIGHT( l )		(INT16)( ( l * 256 ) + CROUCHED_HEIGHT )

#define	GET_OBJECT_LEVEL( z )						( (INT8)( ( z + 10 ) / HEIGHT_UNITS ) )
//#define	OBJECT_DETONATE_ON_IMPACT( object )	( ( object->Obj.usItem == MORTAR_SHELL ) ) // && ( object->ubActionCode == THROW_ARM_ITEM || pObject->fTestObject ) )
// HEADROCK HAM 5: Enabled "Explode on Impact" flag for explosive items
#define	OBJECT_DETONATE_ON_IMPACT( object )	((Item[object->Obj.usItem].usItemClass == IC_BOMB) || ((Item[object->Obj.usItem].usItemClass & IC_EXPLOSV) && Explosive[Item[object->Obj.usItem].ubClassIndex].fExplodeOnImpact))

#define	MAX_INTEGRATIONS				8

#define					TIME_MULTI			1.8

//#define					TIME_MULTI			2.2


#define DELTA_T		( 1.0 * TIME_MULTI )


#define					GRAVITY						( 9.8 * 2.5 )
//#define					GRAVITY						( 9.8 * 2.8 )


REAL_OBJECT			ObjectSlots[ NUM_OBJECT_SLOTS ];
UINT32					guiNumObjectSlots = 0;
BOOLEAN					fDampingActive = FALSE;
//real						Kdl	= (float)0.5;					// LINEAR DAMPENING ( WIND RESISTANCE )
real						Kdl	= (float)( 0.1 * TIME_MULTI );					// LINEAR DAMPENING ( WIND RESISTANCE )

#define					EPSILONV		0.5
#define					EPSILONP		(real)0.01
#define					EPSILONPZ		3

#define					CALCULATE_OBJECT_MASS( m )	( (float)( m * 2 ) )
#define					SCALE_VERT_VAL_TO_HORZ( f )				( ( f / HEIGHT_UNITS ) * CELL_X_SIZE )
#define					SCALE_HORZ_VAL_TO_VERT( f )				( ( f / CELL_X_SIZE ) * HEIGHT_UNITS )

void SimulateObject( REAL_OBJECT *pObject, real deltaT );

void CheckForObjectHittingMerc( REAL_OBJECT *pObject, UINT16 usStructureID );
extern void DoGenericHit( SOLDIERTYPE *pSoldier, UINT8 ubSpecial, INT16 bDirection );


BOOLEAN					PhysicsUpdateLife( REAL_OBJECT *pObject, real DeltaTime );
BOOLEAN					PhysicsComputeForces( REAL_OBJECT *pObject );
BOOLEAN					PhysicsIntegrate( REAL_OBJECT *pObject, real DeltaTime );
BOOLEAN					PhysicsMoveObject( REAL_OBJECT *pObject );
BOOLEAN					PhysicsCheckForCollisions( REAL_OBJECT *pObject, INT32 *piCollisionID );
void					PhysicsResolveCollision( REAL_OBJECT *pObject, vector_3 *pVelocity, vector_3 *pNormal, real CoefficientOfRestitution );
void					PhysicsDeleteObject( REAL_OBJECT *pObject );
BOOLEAN					PhysicsHandleCollisions( REAL_OBJECT *pObject, INT32 *piCollisionID, real DeltaTime );
FLOAT					CalculateForceFromRange(UINT16 usItem, INT16 sRange, FLOAT dDegrees, INT32 sTargetSpot, UINT8 ubTargetLevel);

INT32          RandomGridFromRadius( INT32 sSweetGridNo, INT8 ubMinRadius, INT8 ubMaxRadius );

// Lesh: needed to fix item throwing through window
extern INT16 DirIncrementer[8];

void						HandleArmedObjectImpact( REAL_OBJECT *pObject );
void ObjectHitWindow( INT32 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce );
FLOAT CalculateObjectTrajectory( INT16 sTargetZ, OBJECTTYPE *pItem, vector_3 *vPosition, vector_3 *vForce, INT32 *psFinalGridNo );
vector_3 FindBestForceForTrajectory( INT32 sSrcGridNo, INT32 sGridNo,INT16 sStartZ, INT16 sEndZ, real dzDegrees, OBJECTTYPE *pItem, INT32 *psGridNo, FLOAT *pzMagForce );
INT32 ChanceToGetThroughObjectTrajectory( INT16 sTargetZ, OBJECTTYPE *pItem, vector_3 *vPosition, vector_3 *vForce, INT32 *psFinalGridNo, INT8 *pbLevel, BOOLEAN fFromUI );
FLOAT CalculateSoldierMaxForce(SOLDIERTYPE *pSoldier, FLOAT dDegrees, OBJECTTYPE *pObject, BOOLEAN fArmed, INT32 sTargetSpot, UINT8 ubTargetLevel);
BOOLEAN AttemptToCatchObject( REAL_OBJECT *pObject );
BOOLEAN CheckForCatchObject( REAL_OBJECT *pObject );
BOOLEAN DoCatchObject( REAL_OBJECT *pObject );
BOOLEAN CheckForCatcher( REAL_OBJECT *pObject, UINT16 usStructureID );


void REAL_OBJECT::initialize()
{
	memset(this, 0, SIZEOF_REAL_OBJECT_POD);
	this->Obj.initialize();
}

REAL_OBJECT& REAL_OBJECT::operator =(OLD_REAL_OBJECT_101 &src)
{
	this->Obj = src.oldObj;

	this->fAllocated = src.fAllocated;
	this->fAlive = src.fAlive;
	this->fApplyFriction = src.fApplyFriction;
	this->fColliding = src.fColliding;
	this->fZOnRest = src.fZOnRest;
	this->fVisible = src.fVisible;
	this->fInWater = src.fInWater;
	this->fTestObject = src.fTestObject;
	this->fTestEndedWithCollision = src.fTestEndedWithCollision;
	this->fTestPositionNotSet = src.fTestPositionNotSet;

	this->TestZTarget = src.TestZTarget;
	this->OneOverMass = src.OneOverMass;
	this->AppliedMu = src.AppliedMu;

	this->Position = src.Position;
	this->TestTargetPosition = src.TestTargetPosition;
	this->OldPosition = src.OldPosition;
	this->Velocity = src.Velocity;
	this->OldVelocity = src.OldVelocity;
	this->InitialForce = src.InitialForce;
	this->Force = src.Force;
	this->CollisionNormal = src.CollisionNormal;
	this->CollisionVelocity = src.CollisionVelocity;
	this->CollisionElasticity = src.CollisionElasticity;

	this->sGridNo = src.sGridNo;
	this->iID = src.iID;
	this->pNode = src.pNode;
	this->pShadow = src.pShadow;

	this->sConsecutiveCollisions = src.sConsecutiveCollisions;
	this->sConsecutiveZeroVelocityCollisions = src.sConsecutiveZeroVelocityCollisions;
	this->iOldCollisionCode = src.iOldCollisionCode;

	this->dLifeLength = src.dLifeLength;
	this->dLifeSpan = src.dLifeSpan;
	this->fFirstTimeMoved = src.fFirstTimeMoved;
	this->sFirstGridNo = src.sFirstGridNo;
	this->ubOwner = src.ubOwner;
	this->ubActionCode = src.ubActionCode;
	this->uiActionData = src.uiActionData;
	this->fDropItem = src.fDropItem;
	this->uiNumTilesMoved = src.uiNumTilesMoved;
	this->fCatchGood = src.fCatchGood;
	this->fAttemptedCatch = src.fAttemptedCatch;
	this->fCatchAnimOn = src.fCatchAnimOn;
	this->fCatchCheckDone = src.fCatchCheckDone;
	this->fEndedWithCollisionPositionSet = src.fEndedWithCollisionPositionSet;
	this->EndedWithCollisionPosition = src.EndedWithCollisionPosition;
	this->fHaveHitGround = src.fHaveHitGround;
	this->fPotentialForDebug = src.fPotentialForDebug;
	this->sLevelNodeGridNo = src.sLevelNodeGridNo;
	this->iSoundID = src.iSoundID;
	this->ubLastTargetTakenDamage = src.ubLastTargetTakenDamage;
	return *this;
}

/// OBJECT POOL FUNCTIONS
INT32 GetFreeObjectSlot(void)
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumObjectSlots; uiCount++)
	{
		if(( ObjectSlots[uiCount].fAllocated == FALSE ) )
			return((INT32)uiCount);
	}

	if(guiNumObjectSlots < NUM_OBJECT_SLOTS )
		return((INT32)guiNumObjectSlots++);

	return(-1);
}


void RecountObjectSlots(void)
{
	INT32 uiCount;

	for(uiCount=guiNumObjectSlots-1; (uiCount >=0) ; uiCount--)
	{
		if( ( ObjectSlots[uiCount].fAllocated ) )
		{
			guiNumObjectSlots=(UINT32)(uiCount+1);
			return;
		}
	}

	guiNumObjectSlots = 0;
}


INT32	CreatePhysicalObject( OBJECTTYPE *pGameObj, real dLifeLength, real xPos, real yPos, real zPos, real xForce, real yForce, real zForce, UINT8 ubOwner, UINT8 ubActionCode, UINT32 uiActionData, BOOLEAN fTestObject )
{
	INT32			iObjectIndex;
	FLOAT			mass;
	REAL_OBJECT		*pObject;

	if( ( iObjectIndex = GetFreeObjectSlot() )==(-1) )
		return(-1);

	pObject = &( ObjectSlots[ iObjectIndex ] );

	pObject->initialize();

	// OK, GET OBJECT DATA AND COPY
	pObject->Obj = *pGameObj;

	// Get mass
	mass =	CALCULATE_OBJECT_MASS( Item[pGameObj->usItem ].ubWeight );

	// If mass is z, make it something!
	if ( mass == 0 )
	{
		mass = 10;
	}

	// OK, mass determines the smoothness of the physics integration
	// For gameplay, we will use mass for maybe max throw distance
	mass = 60;

	// Set lifelength
	pObject->dLifeLength = dLifeLength;

	pObject->fAllocated		= TRUE;
	pObject->fAlive					= TRUE;
	pObject->fApplyFriction	= FALSE;
	pObject->iSoundID		= NO_SAMPLE;

	// Set values
	pObject->OneOverMass = 1 / mass;
	pObject->Position.x	= xPos;
	pObject->Position.y	= yPos;
	pObject->Position.z	= zPos;
	pObject->fVisible		= TRUE;
	pObject->fTestObject	= fTestObject;
	pObject->ubOwner	= ubOwner;
	pObject->ubActionCode = ubActionCode;
	pObject->uiActionData = uiActionData;
	pObject->fDropItem		= TRUE;
	pObject->ubLastTargetTakenDamage = NOBODY;

	pObject->fFirstTimeMoved = TRUE;

	pObject->InitialForce.x	= SCALE_VERT_VAL_TO_HORZ( xForce );
	pObject->InitialForce.y	= SCALE_VERT_VAL_TO_HORZ( yForce );
	pObject->InitialForce.z	= zForce ;

	pObject->InitialForce = VDivScalar( &(pObject->InitialForce), (float)TIME_MULTI );
	pObject->InitialForce = VMultScalar( &(pObject->InitialForce), 1.5 );


	// Calculate gridNo
	pObject->sGridNo = MAPROWCOLTOPOS( ( (INT16)yPos / CELL_Y_SIZE ), ( (INT16)xPos / CELL_X_SIZE ) );
	pObject->iID = iObjectIndex;
	pObject->pNode = NULL;
	pObject->pShadow = NULL;

	// If gridno not equal to NOWHERE, use sHeight of alnd....	
	if (!TileIsOutOfBounds(pObject->sGridNo))
	{
		pObject->Position.z += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pObject->sGridNo ].sHeight );
		pObject->EndedWithCollisionPosition.z += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pObject->sGridNo ].sHeight );
	}

	PhysicsDebugMsg( String( "NewPhysics Object")	);

	// If this is a real object being created, then increase the ABC
	if (!fTestObject)
	{
		gTacticalStatus.ubAttackBusyCount++;
		DebugAttackBusy( String( "@@@@@@@ Increasing attacker busy count..., PHYSICS OBJECT effect started.	Now %d\n", gTacticalStatus.ubAttackBusyCount) );
	}

	return( iObjectIndex );
}


BOOLEAN RemoveObjectSlot( INT32 iObject )
{
	CHECKF( iObject < NUM_OBJECT_SLOTS );

	ObjectSlots[iObject].fAllocated=FALSE;

	RecountObjectSlots();

	return( TRUE );
}


void SimulateWorld(	)
{
	UINT32					cnt;
	REAL_OBJECT		*pObject;


	if ( COUNTERDONE( PHYSICSUPDATE ) )
	{
		RESETCOUNTER( PHYSICSUPDATE );

		for( cnt = 0;cnt < guiNumObjectSlots; cnt++)
		{
			// CHECK FOR ALLOCATED
			if( ObjectSlots[ cnt ].fAllocated )
			{
				// Get object
				pObject = &( ObjectSlots[ cnt ] );

				SimulateObject( pObject, (real)DELTA_T );
			}
		}
	}
}

void RemoveAllPhysicsObjects( )
{
	UINT32					cnt;

	for( cnt = 0;cnt < guiNumObjectSlots; cnt++)
	{
		// CHECK FOR ALLOCATED
		if( ObjectSlots[ cnt ].fAllocated )
		{
			PhysicsDeleteObject( &(ObjectSlots[ cnt ]) );
		}
	}
}


void SimulateObject( REAL_OBJECT *pObject, real deltaT )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject");

	if ( !PhysicsUpdateLife( pObject, (float)deltaT ) )
	{
		return;
	}

	//	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: check life");
	if ( pObject->fAlive )
	{
		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: do subtime");
		// Do subtime here....

		if ( !PhysicsComputeForces( pObject ) )
		{
			//			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: couldn't compute forces");
			return;
		}

		real CurrentTime = 0;
		real DeltaTime = (float)deltaT / 10.0f;
		real TargetTime = (float)deltaT;
		INT32			iCollisionID;
		BOOLEAN		fEndThisObject = FALSE;

		while( CurrentTime < TargetTime )
		{
			//MADD TO KAIDEN: HERE IS THE PROBLEM:	For some reason fEndThisObject is never set to true
			//			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("SimulateObject: in loop while CurrentTime < TargetTime: %d < %d",CurrentTime,TargetTime));

			if ( !PhysicsIntegrate( pObject, DeltaTime ) )
			{
				fEndThisObject = TRUE;
				break;
			}

			if ( !PhysicsHandleCollisions( pObject, &iCollisionID, DeltaTime	) )
			{
				fEndThisObject = TRUE;
				break;
			}

			if ( iCollisionID != COLLISION_NONE )
			{
				break;
			}

			CurrentTime += DeltaTime;
		}

		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: check if ending this object");
		if ( fEndThisObject )
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: object ended, returning");
			return;
		}

		//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: try moving the object");
		if ( !PhysicsMoveObject( pObject ) )
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"SimulateObject: couldn't move object, returning");
			return;
		}

	}
}


BOOLEAN	PhysicsComputeForces( REAL_OBJECT *pObject )
{
	vector_3			vTemp;

	// Calculate forces
	pObject->Force = VSetEqual( &(pObject->InitialForce ) );

	// Note: Only apply gravity if we are not resting on some structure surface
	if ( !pObject->fZOnRest )
	{
		pObject->Force.z -= (real)GRAVITY;
	}

	// Set intial force to zero
	pObject->InitialForce = VMultScalar( &(pObject->InitialForce ), 0 );

	if ( pObject->fApplyFriction )
	{
		vTemp = VMultScalar( &(pObject->Velocity), -pObject->AppliedMu );
		pObject->Force = VAdd( &(vTemp), &(pObject->Force) );

		pObject->fApplyFriction = FALSE;
	}

	if( fDampingActive )
	{
		vTemp = VMultScalar( &(pObject->Velocity), -Kdl );
		pObject->Force = VAdd( &(vTemp), &(pObject->Force) );

	}

	return( TRUE );
}

BOOLEAN	PhysicsUpdateLife( REAL_OBJECT *pObject, real DeltaTime )
{
	UINT8 bLevel = 0;

	pObject->dLifeSpan += DeltaTime;

	// End life if time has ran out or we are stationary
	if ( pObject->dLifeLength != -1 )
	{
		if ( pObject->dLifeSpan > pObject->dLifeLength )
		{
			pObject->fAlive = FALSE;
		}

	}

	// WANNE.BMP: Only do this check on normal maps.
	// When we do this check on Big Maps it always returns false and we got wrong physics when trowing items!
	if (WORLD_COLS <= 160 && WORLD_ROWS <= 160)
	{
		// End life if we are out of bounds....
		if ( !GridNoOnVisibleWorldTile( pObject->sGridNo ) )
		{
			pObject->fAlive = FALSE;
		}
	}

	if ( !pObject->fAlive )
	{
		// ATE: OK, adjust gridno based on where we ended and if we hit any walls....
		{
			// Check for SW wall...
			real remainder;

			remainder = (float)fmod( pObject->Position.x, CELL_X_SIZE );

			if ( remainder >= 9 )
			{
				// DO we have a wall here?
				if ( WallOrClosedDoorExistsOfTopRightOrientation( pObject->sGridNo ) )
				{
					// Adjust forward 1!
					pObject->sGridNo += 1;
				}
			}

			// Now SE wall
			remainder = (float)fmod( pObject->Position.y,	CELL_X_SIZE );

			if ( remainder >= 9 )
			{
				// DO we have a wall here?
				if ( WallOrClosedDoorExistsOfTopLeftOrientation( pObject->sGridNo ) )
				{
					// Adjust forward 1!
					pObject->sGridNo += WORLD_COLS;
				}
			}
		}

		pObject->fAlive = FALSE;

		if ( !pObject->fTestObject )
		{
			if ( pObject->iSoundID != NO_SAMPLE )
			{
				SoundStop( pObject->iSoundID );
			}

			if ( pObject->ubActionCode == THROW_ARM_ITEM && !pObject->fInWater )
			{
				HandleArmedObjectImpact( pObject );
			}
			else
			{
				// If we are in water, and we are a sinkable item...
				//				if ( !pObject->fInWater || !( Item[ pObject->Obj.usItem ].fFlags & ITEM_SINKS ) )
				if ( !pObject->fInWater || !( Item[ pObject->Obj.usItem ].sinks	) )
				{
					if ( pObject->fDropItem )
					{
						// ATE: If we have collided with roof last...
						if ( pObject->iOldCollisionCode == COLLISION_ROOF )
						{
							bLevel = 1;
						}

						// ATE: Yet another hack here, to make sure things don't fall through roofs
						if ( pObject->ubLastTargetTakenDamage != NOBODY )
						{
							SOLDIERTYPE *pSoldier;

							pSoldier = MercPtrs[ pObject->ubLastTargetTakenDamage ];

							bLevel = pSoldier->pathing.bLevel;
						}

						// ATE; If an armed object, don't add....
						if ( pObject->ubActionCode != THROW_ARM_ITEM )
						{
							AddItemToPool( pObject->sGridNo, &( pObject->Obj ), 1, bLevel, 0, -1 );
						}
					}
				}
			}

			// Make impact noise....
			if ( Item[pObject->Obj.usItem].rock )
			{
				MakeNoise( pObject->ubOwner, pObject->sGridNo, 0, gpWorldLevelData[ pObject->sGridNo ].ubTerrainID, (UINT8) (9 + PreRandom( 9 ) ), NOISE_ROCK_IMPACT );
			}
			else if ( Item[ pObject->Obj.usItem ].usItemClass & IC_GRENADE )
			{
				MakeNoise( pObject->ubOwner, pObject->sGridNo, 0, gpWorldLevelData[ pObject->sGridNo ].ubTerrainID, (UINT8) (9 + PreRandom( 9 ) ), NOISE_GRENADE_IMPACT );
			}

			if ( !pObject->fTestObject && pObject->iOldCollisionCode == COLLISION_GROUND )
			{
				PlayJA2Sample( THROW_IMPACT_2, RATE_11025, SoundVolume( MIDVOLUME, pObject->sGridNo ), 1, SoundDir( pObject->sGridNo ) );
			}

			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., PHYSICS OBJECT DONE effect gone off") );
			DebugAttackBusy( "@@@@@@@ PHYSICS OBJECT effect finished\n" );
			ReduceAttackBusyCount( );

			// ATE: Handle end of animation...
			if ( pObject->fCatchAnimOn )
			{
				SOLDIERTYPE *pSoldier;

				pObject->fCatchAnimOn = FALSE;

				// Get intended target
				pSoldier = MercPtrs[ pObject->uiActionData ];

				// Catch anim.....
				switch( gAnimControl[ pSoldier->usAnimState ].ubHeight )
				{
				case ANIM_STAND:

					pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
					pSoldier->EVENT_InitNewSoldierAnim( END_CATCH, 0 , FALSE );
					break;

				case ANIM_CROUCH:

					pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
					pSoldier->EVENT_InitNewSoldierAnim( END_CROUCH_CATCH, 0 , FALSE );
					break;
				}

				PlayJA2Sample( CATCH_OBJECT, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

			}
		}

		PhysicsDeleteObject( pObject );
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN	PhysicsIntegrate( REAL_OBJECT *pObject, real DeltaTime )
{
	vector_3			vTemp;

	// Save old position
	pObject->OldPosition = VSetEqual( &(pObject->Position) );
	pObject->OldVelocity = VSetEqual( &(pObject->Velocity) );

	vTemp = VMultScalar( &(pObject->Velocity), DeltaTime );
	pObject->Position = VAdd( &(pObject->Position), &vTemp );

	// Save test TargetPosition
	if ( pObject->fTestPositionNotSet )
	{
		pObject->TestTargetPosition	= VSetEqual( &(pObject->Position) );
	}

	vTemp = VMultScalar( &(pObject->Force), ( DeltaTime * pObject->OneOverMass ) );
	pObject->Velocity = VAdd( &(pObject->Velocity), &vTemp );

	if ( pObject->fPotentialForDebug )
	{
		PhysicsDebugMsg( String( "Object %d: Force		%f %f %f", pObject->iID, pObject->Force.x, pObject->Force.y, pObject->Force.z ) );
		PhysicsDebugMsg( String( "Object %d: Velocity %f %f %f", pObject->iID, pObject->Velocity.x, pObject->Velocity.y, pObject->Velocity.z ) );
		PhysicsDebugMsg( String( "Object %d: Position %f %f %f", pObject->iID, pObject->Position.x, pObject->Position.y, pObject->Position.z ) );
		PhysicsDebugMsg( String( "Object %d: Delta Pos %f %f %f", pObject->iID, (pObject->OldPosition.x - pObject->Position.x ), (pObject->OldPosition.y - pObject->Position.y ), ( pObject->OldPosition.z - pObject->Position.z ) ) );
	}

	//	if ( pObject->Obj.usItem == MORTAR_SHELL && !pObject->fTestObject && pObject->ubActionCode == THROW_ARM_ITEM )
	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 524, usItemClass=%d",Item[pObject->Obj.usItem].usItemClass));

	if ( Item[pObject->Obj.usItem].usItemClass == IC_BOMB && !pObject->fTestObject && pObject->ubActionCode == THROW_ARM_ITEM )
	{
		// Start soud if we have reached our max height
		if ( pObject->OldVelocity.z >= 0 && pObject->Velocity.z < 0 )
		{
			if ( pObject->iSoundID == NO_SAMPLE )
			{
				pObject->iSoundID =	PlayJA2Sample( MORTAR_WHISTLE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			}
		}
	}

	return( TRUE );
}


BOOLEAN	PhysicsHandleCollisions( REAL_OBJECT *pObject, INT32 *piCollisionID, real DeltaTime )
{
	FLOAT					dDeltaX, dDeltaY, dDeltaZ;


	if ( PhysicsCheckForCollisions( pObject, piCollisionID ) )
	{

		dDeltaX = pObject->Position.x - pObject->OldPosition.x;
		dDeltaY = pObject->Position.y - pObject->OldPosition.y;
		dDeltaZ = pObject->Position.z - pObject->OldPosition.z;

		if ( dDeltaX <= EPSILONV && dDeltaX >= -EPSILONV &&
			dDeltaY <= EPSILONV && dDeltaY >= -EPSILONV )
		{
			pObject->sConsecutiveZeroVelocityCollisions++;
		}

		if ( pObject->sConsecutiveZeroVelocityCollisions > 3 )
		{
			// We will continue with our Z velocity
			pObject->Velocity.x = 0;
			pObject->Velocity.y = 0;

			// Check that we are not colliding with structure z
			//if ( *piCollisionID == COLLISION_STRUCTURE_Z || *piCollisionID == COLLISION_ROOF )
			if ( *piCollisionID == COLLISION_STRUCTURE_Z || *piCollisionID == COLLISION_ROOF || *piCollisionID == COLLISION_GROUND )
			{
				pObject->Velocity.z = 0;

				// Set us not alive!
				pObject->fAlive = FALSE;
			}

			*piCollisionID = COLLISION_NONE;
		}
		else
		{
			// Set position back to before collision
			pObject->Position = VSetEqual( &(pObject->OldPosition) );
			// Set old position!
			pObject->OldPosition.x = pObject->Position.y - dDeltaX;
			pObject->OldPosition.y = pObject->Position.x - dDeltaY;
			pObject->OldPosition.z = pObject->Position.z - dDeltaZ;

			PhysicsResolveCollision( pObject, &(pObject->CollisionVelocity), &(pObject->CollisionNormal), pObject->CollisionElasticity );
		}

		if ( pObject->Position.z < 0 )
		{
			pObject->Position.z = 0;
		}
		//otherwise, continue falling downwards!

		// TO STOP?

		// Check for delta position values
		if ( dDeltaZ <= EPSILONP && dDeltaZ >= -EPSILONP &&
			dDeltaY <= EPSILONP && dDeltaY >= -EPSILONP &&
			dDeltaX <= EPSILONP && dDeltaX >= -EPSILONP )
		{
			//pObject->fAlive = FALSE;
			//return( FALSE );
		}

		// Check for repeated collisions...
		//if ( pObject->iOldCollisionCode == COLLISION_ROOF || pObject->iOldCollisionCode == COLLISION_GROUND || pObject->iOldCollisionCode == COLLISION_WATER )
		{
			// ATE: This is a safeguard
			if ( pObject->sConsecutiveCollisions > 30 )
			{
				pObject->fAlive = FALSE;
				return( FALSE );
			}
		}

		// Check for -ve velocity still...
		/*if ( pObject->Velocity.z <= EPSILONV && pObject->Velocity.z >= -EPSILONV &&
		pObject->Velocity.y <= EPSILONV && pObject->Velocity.y >= -EPSILONV &&
		pObject->Velocity.x <= EPSILONV && pObject->Velocity.x >= -EPSILONV )
		{
		PhysicsDeleteObject( pObject );
		pObject->fAlive = FALSE;
		return( FALSE );
		}*/
	}

	return( TRUE );
}

void PhysicsDeleteObject( REAL_OBJECT *pObject )
{
	if ( pObject->fAllocated )
	{
		if ( pObject->pNode != NULL )
		{
			RemoveStructFromLevelNode( pObject->sLevelNodeGridNo, pObject->pNode );
		}

		if ( pObject->pShadow != NULL )
		{
			RemoveShadowFromLevelNode( pObject->sLevelNodeGridNo, pObject->pShadow );
		}


		RemoveObjectSlot( pObject->iID );
	}
}

INT16 gsWaterSplashSoundNum = -1;

void PlaySplashSound(INT32 sGridNo)
{
	// sevenfm: also play sound
	CHAR8	zFilename[512];
	// prepare water splash sound
	if (gsWaterSplashSoundNum < 0)
	{
		gsWaterSplashSoundNum = 0;
		do
		{
			gsWaterSplashSoundNum++;
			sprintf(zFilename, "sounds\\misc\\Splash%d.ogg", gsWaterSplashSoundNum);
		} while (FileExists(zFilename));
		gsWaterSplashSoundNum--;
	}
	if (gsWaterSplashSoundNum > 0)
	{
		sprintf(zFilename, "sounds\\misc\\Splash%d.ogg", Random(gsWaterSplashSoundNum) + 1);
		if (FileExists(zFilename))
		{
			PlayJA2SampleFromFile(zFilename, RATE_11025, SoundVolume(MIDVOLUME, sGridNo), 1, SoundDir(sGridNo));
		}
	}
}

BOOLEAN	PhysicsCheckForCollisions( REAL_OBJECT *pObject, INT32 *piCollisionID )
{
	vector_3			vTemp;
	FLOAT					dDeltaX, dDeltaY, dDeltaZ, dX, dY, dZ;
	INT32					iCollisionCode = COLLISION_NONE;
	BOOLEAN				fDoCollision = FALSE;
	FLOAT					dElasity = 1;
	UINT16				usStructureID = -1;
	FLOAT					dNormalX = 0.0, dNormalY = 0.0, dNormalZ = 1.0;
	INT32					sGridNo = NOWHERE;

	// Checkf for collisions
	dX = pObject->Position.x;
	dY = pObject->Position.y;
	dZ = pObject->Position.z;

	vTemp.x = 0;
	vTemp.y = 0;
	vTemp.z = 0;

	dDeltaX = dX - pObject->OldPosition.x;
	dDeltaY = dY - pObject->OldPosition.y;
	dDeltaZ = dZ - pObject->OldPosition.z;

	//Round delta pos to nearest 0.01
	//dDeltaX = (float)( (int)dDeltaX * 100 ) / 100;
	//dDeltaY = (float)( (int)dDeltaY * 100 ) / 100;
	//dDeltaZ = (float)( (int)dDeltaZ * 100 ) / 100;

	// SKIP FIRST GRIDNO, WE'LL COLLIDE WITH OURSELVES....
	if ( pObject->fTestObject != TEST_OBJECT_NO_COLLISIONS )
	{
		iCollisionCode = CheckForCollision( dX, dY, dZ, dDeltaX, dDeltaY, dDeltaZ, (INT16 *)&usStructureID, &dNormalX, &dNormalY, &dNormalZ );
	}
	else if ( pObject->fTestObject == TEST_OBJECT_NO_COLLISIONS )
	{
		iCollisionCode = COLLISION_NONE;

		// Are we on a downward slope?
		if ( dZ < pObject->TestZTarget && dDeltaZ < 0 )
		{
			if (pObject->fTestPositionNotSet )
			{
				if ( pObject->TestZTarget > 32 )
				{
					pObject->fTestPositionNotSet = FALSE;
					pObject->TestZTarget		 = 0;
				}
				else
				{
					iCollisionCode = COLLISION_GROUND;
				}
			}
			else
			{
				iCollisionCode = COLLISION_GROUND;
			}
		}
	}


	// If a test object and we have collided with something ( should only be ground ( or roof? ) )
	// Or destination?
	if ( pObject->fTestObject == TEST_OBJECT_ANY_COLLISION )
	{
		if ( iCollisionCode != COLLISION_GROUND && iCollisionCode != COLLISION_ROOF && iCollisionCode != COLLISION_WATER && iCollisionCode != COLLISION_NONE )
		{
			pObject->fTestEndedWithCollision = TRUE;
			pObject->fAlive = FALSE;
			return( FALSE );
		}
	}

	if ( pObject->fTestObject == TEST_OBJECT_NOTWALLROOF_COLLISIONS )
	{
		// So we don't collide with ourselves.....
		if ( iCollisionCode != COLLISION_WATER && iCollisionCode != COLLISION_GROUND && iCollisionCode != COLLISION_NONE &&
			iCollisionCode != COLLISION_ROOF && iCollisionCode != COLLISION_INTERIOR_ROOF &&
			iCollisionCode != COLLISION_WALL_SOUTHEAST && iCollisionCode != COLLISION_WALL_SOUTHWEST &&
			iCollisionCode != COLLISION_WALL_NORTHEAST && iCollisionCode != COLLISION_WALL_NORTHWEST )
		{
			if ( pObject->fFirstTimeMoved || pObject->sFirstGridNo == pObject->sGridNo )
			{
				iCollisionCode = COLLISION_NONE;
			}

			// If we are NOT a wall or window, ignore....
			if ( pObject->uiNumTilesMoved < 4 )
			{
				switch( iCollisionCode )
				{
				case COLLISION_MERC:
				case COLLISION_STRUCTURE:
				case COLLISION_STRUCTURE_Z:

					// Set to no collision ( we shot past )
					iCollisionCode = COLLISION_NONE;
					break;
				}
			}
		}


		switch( iCollisionCode )
		{
			// End test with any collision NOT a wall, roof...
		case COLLISION_STRUCTURE:
		case COLLISION_STRUCTURE_Z:

			// OK, if it's mercs... don't stop
			if ( usStructureID >= INVALID_STRUCTURE_ID )
			{
				pObject->fTestEndedWithCollision = TRUE;

				if ( !pObject->fEndedWithCollisionPositionSet )
				{
					pObject->fEndedWithCollisionPositionSet = TRUE;
					pObject->EndedWithCollisionPosition = VSetEqual( &(pObject->Position) );
				}
				iCollisionCode = COLLISION_NONE;
			}
			else
			{
				if ( !pObject->fEndedWithCollisionPositionSet )
				{
					pObject->fEndedWithCollisionPositionSet = TRUE;
					pObject->EndedWithCollisionPosition = VSetEqual( &(pObject->Position) );
				}
			}
			break;

		case COLLISION_ROOF:

			if ( !pObject->fEndedWithCollisionPositionSet )
			{
				pObject->fEndedWithCollisionPositionSet = TRUE;
				pObject->EndedWithCollisionPosition = VSetEqual( &(pObject->Position) );
			}
			break;

		case COLLISION_WATER:
		case COLLISION_GROUND:
		case COLLISION_MERC:
		case COLLISION_INTERIOR_ROOF:
		case COLLISION_NONE:
		case COLLISION_WINDOW_SOUTHEAST:
		case COLLISION_WINDOW_SOUTHWEST:
		case COLLISION_WINDOW_NORTHEAST:
		case COLLISION_WINDOW_NORTHWEST:

			// Here we just keep going..
			break;

		default:

			// THis is for walls, windows, etc
			// here, we set test ended with collision, but keep going...
			pObject->fTestEndedWithCollision = TRUE;
			break;
		}
	}


	if ( pObject->fTestObject != TEST_OBJECT_NOTWALLROOF_COLLISIONS )
	{
		if ( iCollisionCode != COLLISION_WATER && iCollisionCode != COLLISION_GROUND && iCollisionCode != COLLISION_NONE &&
			iCollisionCode != COLLISION_ROOF && iCollisionCode != COLLISION_INTERIOR_ROOF &&
			iCollisionCode != COLLISION_WALL_SOUTHEAST && iCollisionCode != COLLISION_WALL_SOUTHWEST &&
			iCollisionCode != COLLISION_WALL_NORTHEAST && iCollisionCode != COLLISION_WALL_NORTHWEST )
		{
			// So we don't collide with ourselves.....
			if ( pObject->fFirstTimeMoved || pObject->sFirstGridNo == pObject->sGridNo )
			{
				iCollisionCode = COLLISION_NONE;
			}

			// If we are NOT a wall or window, ignore....
			if ( pObject->uiNumTilesMoved < 4 )
			{
				switch( iCollisionCode )
				{
				case COLLISION_MERC:
				case COLLISION_STRUCTURE:
				case COLLISION_STRUCTURE_Z:

					// Set to no collision ( we shot past )
					iCollisionCode = COLLISION_NONE;
					break;
				}
			}

		}
	}

	*piCollisionID = iCollisionCode;

	// If We hit the ground
	if ( iCollisionCode > COLLISION_NONE )
	{
		if ( pObject->iOldCollisionCode == iCollisionCode )
		{
			pObject->sConsecutiveCollisions++;
		}
		else
		{
			pObject->sConsecutiveCollisions = 1;
		}

		if ( iCollisionCode == COLLISION_WINDOW_NORTHWEST || iCollisionCode == COLLISION_WINDOW_NORTHEAST || iCollisionCode == COLLISION_WINDOW_SOUTHWEST || iCollisionCode == COLLISION_WINDOW_SOUTHEAST )
		{
			// sevenfm: added requirements for object to break window
			if (Item[pObject->Obj.usItem].ubWeight >= 4 &&
				Item[pObject->Obj.usItem].sinks &&
				!Item[pObject->Obj.usItem].unaerodynamic &&
				(Item[pObject->Obj.usItem].metal || Item[pObject->Obj.usItem].rock))
			{
				if (!pObject->fTestObject)
				{
					// Break window!
					PhysicsDebugMsg(String("Object %d: Collision Window", pObject->iID));

					sGridNo = MAPROWCOLTOPOS(((INT16)pObject->Position.y / CELL_Y_SIZE), ((INT16)pObject->Position.x / CELL_X_SIZE));

					ObjectHitWindow(sGridNo, usStructureID, FALSE, TRUE);
				}
				*piCollisionID = COLLISION_NONE;
				return(FALSE);
			}
			else
			{
				// A wall, do stuff
				vTemp.x = dNormalX;
				vTemp.y = dNormalY;
				vTemp.z = dNormalZ;

				fDoCollision = TRUE;
				dElasity = (float)1.1;
			}
		}

		// ATE: IF detonate on impact, stop now!
		if ( OBJECT_DETONATE_ON_IMPACT( pObject ) )
		{
			pObject->fAlive = FALSE;
			return( TRUE );
		}

		if ( iCollisionCode == COLLISION_GROUND )
		{
			vTemp.x = 0;
			vTemp.y = 0;
			vTemp.z = -1;

			pObject->fApplyFriction = TRUE;
			//pObject->AppliedMu			= (float)(0.54 * TIME_MULTI );
			pObject->AppliedMu			= (float)(0.34 * TIME_MULTI );

			//dElasity = (float)1.5;
			dElasity = (float)1.3;

			fDoCollision = TRUE;

			if ( !pObject->fTestObject && !pObject->fHaveHitGround )
			{
				PlayJA2Sample( THROW_IMPACT_2, RATE_11025, SoundVolume( MIDVOLUME, pObject->sGridNo ), 1, SoundDir( pObject->sGridNo ) );
			}

			pObject->fHaveHitGround = TRUE;
		}
		else if ( iCollisionCode == COLLISION_WATER )
		{
			ANITILE_PARAMS	AniParams;
			ANITILE						*pNode;

			// Continue going...
			pObject->fApplyFriction = TRUE;
			pObject->AppliedMu			= (float)(1.54 * TIME_MULTI );

			sGridNo = MAPROWCOLTOPOS( ( (INT16)pObject->Position.y / CELL_Y_SIZE ), ( (INT16)pObject->Position.x / CELL_X_SIZE ) );

			// Make thing unalive...
			pObject->fAlive = FALSE;

			// If first time...
			if ( pObject->fVisible )
			{
				if ( pObject->fTestObject == NO_TEST_OBJECT )
				{
					// Make invisible
					pObject->fVisible = FALSE;

					// JA25 CJC Oct 13 1999 - if node pointer is null don't try to set flags inside it!
					if( pObject->pNode )
					{
						pObject->pNode->uiFlags |= LEVELNODE_HIDDEN;
					}

					pObject->fInWater	= TRUE;

					// Make ripple
					memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
					AniParams.sGridNo							= sGridNo;
					AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
					AniParams.usTileType				= THIRDMISS;
					AniParams.usTileIndex					= THIRDMISS1;
					AniParams.sDelay							= 50;
					AniParams.sStartFrame					= 0;
					AniParams.uiFlags							= ANITILE_FORWARD;

					// sevenfm: explosion animation only for normal, stun and flashbang type explosives
					if (pObject->ubActionCode == THROW_ARM_ITEM &&
						(Item[pObject->Obj.usItem].usItemClass & IC_EXPLOSV) &&
						(Explosive[Item[pObject->Obj.usItem].ubClassIndex].ubType == EXPLOSV_NORMAL ||
						Explosive[Item[pObject->Obj.usItem].ubClassIndex].ubType == EXPLOSV_STUN ||
						Explosive[Item[pObject->Obj.usItem].ubClassIndex].ubType == EXPLOSV_FLASHBANG))
					{
						gTacticalStatus.ubAttackBusyCount++;
						DebugAttackBusy( String( "Incrementing attack busy because of delayed water explosion. Now %d\n", gTacticalStatus.ubAttackBusyCount ) );
						AniParams.ubKeyFrame1					= 11;
						AniParams.uiKeyFrame1Code			= ANI_KEYFRAME_CHAIN_WATER_EXPLOSION;
						AniParams.uiUserData					= pObject->Obj.usItem;
						AniParams.ubUserData2					= pObject->ubOwner;
					}

					pNode = CreateAnimationTile( &AniParams );

					// Adjust for absolute positioning
					pNode->pLevelNode->uiFlags |= LEVELNODE_USEABSOLUTEPOS;

					pNode->pLevelNode->sRelativeX	= (INT16)pObject->Position.x;
					pNode->pLevelNode->sRelativeY	= (INT16)pObject->Position.y;
					pNode->pLevelNode->sRelativeZ	= (INT16)CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16)pObject->Position.z );

					// sevenfm: also play sound
					UINT16 usItem = pObject->Obj.usItem;
					INT32 sGridNo = pObject->sGridNo;

					if (HasItemFlag(usItem, CORPSE))
						PlayJA2Sample(ENTER_DEEP_WATER_1, RATE_11025, SoundVolume(MIDVOLUME, sGridNo), 1, SoundDir(sGridNo));
					else if (Item[usItem].ubWeight > 10)
						PlayJA2Sample(ENTER_WATER_1, RATE_11025, SoundVolume(MIDVOLUME, sGridNo), 1, SoundDir(sGridNo));
					else
						PlaySplashSound(sGridNo);
				}
			}

		}
		else if ( iCollisionCode == COLLISION_ROOF || iCollisionCode == COLLISION_INTERIOR_ROOF )
		{
			vTemp.x = 0;
			vTemp.y = 0;
			vTemp.z = -1;

			pObject->fApplyFriction = TRUE;
			pObject->AppliedMu			= (float)(0.54 * TIME_MULTI );

			dElasity = (float)1.4;

			fDoCollision = TRUE;

		}
		//else if ( iCollisionCode == COLLISION_INTERIOR_ROOF )
		//{
		//	vTemp.x = 0;
		//	vTemp.y = 0;
		//		vTemp.z = 1;

		//	pObject->fApplyFriction = TRUE;
		//	pObject->AppliedMu			= (float)(0.54 * TIME_MULTI );

		//	dElasity = (float)1.4;

		//	fDoCollision = TRUE;

		//}
		else if ( iCollisionCode == COLLISION_STRUCTURE_Z )
		{
			if ( CheckForCatcher( pObject, usStructureID ) )
			{
				return( FALSE );
			}

			CheckForObjectHittingMerc( pObject, usStructureID );

			vTemp.x = 0;
			vTemp.y = 0;
			vTemp.z = -1;

			pObject->fApplyFriction = TRUE;
			pObject->AppliedMu			= (float)(0.54 * TIME_MULTI );

			dElasity = (float)1.2;

			fDoCollision = TRUE;

		}
		else if ( iCollisionCode == COLLISION_WALL_SOUTHEAST || iCollisionCode == COLLISION_WALL_SOUTHWEST ||
			iCollisionCode == COLLISION_WALL_NORTHEAST || iCollisionCode == COLLISION_WALL_NORTHWEST )
		{
			// A wall, do stuff
			vTemp.x = dNormalX;
			vTemp.y = dNormalY;
			vTemp.z = dNormalZ;

			fDoCollision = TRUE;

			dElasity = (float)1.1;
		}
		else
		{
			vector_3 vIncident;

			if ( CheckForCatcher( pObject, usStructureID ) )
			{
				return( FALSE );
			}

			CheckForObjectHittingMerc( pObject, usStructureID );

			vIncident.x = dDeltaX;
			vIncident.y = dDeltaY;
			vIncident.z = 0;
			// Nomralize

			vIncident = VGetNormal( &vIncident );

			//vTemp.x = -1;
			//vTemp.y = 0;
			//vTemp.z = 0;
			vTemp.x = -1 * vIncident.x;
			vTemp.y = -1 * vIncident.y;
			vTemp.z = 0;

			fDoCollision = TRUE;

			dElasity = (float)1.1;
		}

		if ( fDoCollision )
		{
			pObject->CollisionNormal.x		= vTemp.x;
			pObject->CollisionNormal.y		= vTemp.y;
			pObject->CollisionNormal.z		= vTemp.z;
			pObject->CollisionElasticity	= dElasity;
			pObject->iOldCollisionCode	= iCollisionCode;

			// Save collision velocity
			pObject->CollisionVelocity = VSetEqual( &(pObject->OldVelocity) );

			if ( pObject->fPotentialForDebug )
			{
				PhysicsDebugMsg( String( "Object %d: Collision %d", pObject->iID, iCollisionCode ) );
				PhysicsDebugMsg( String( "Object %d: Collision Normal %f %f %f", pObject->iID, vTemp.x, vTemp.y, vTemp.z ) );
				PhysicsDebugMsg( String( "Object %d: Collision OldPos %f %f %f", pObject->iID, pObject->Position.x, pObject->Position.y, pObject->Position.z ) );
				PhysicsDebugMsg( String( "Object %d: Collision Velocity %f %f %f", pObject->iID, pObject->CollisionVelocity.x, pObject->CollisionVelocity.y, pObject->CollisionVelocity.z ) );
			}

			pObject->fColliding = TRUE;

		}
		else
		{
			pObject->fColliding = FALSE;
			pObject->sConsecutiveCollisions = 0;
			pObject->sConsecutiveZeroVelocityCollisions = 0;
			pObject->fHaveHitGround = FALSE;
		}
	}

	return( fDoCollision );
}


void PhysicsResolveCollision( REAL_OBJECT *pObject, vector_3 *pVelocity, vector_3 *pNormal, real CoefficientOfRestitution )
{
	real ImpulseNumerator, Impulse;
	vector_3			vTemp;

	ImpulseNumerator = -1 * CoefficientOfRestitution * VDotProduct( pVelocity , pNormal );

	Impulse = ImpulseNumerator;

	vTemp = VMultScalar( pNormal, Impulse );

	pObject->Velocity = VAdd( &(pObject->Velocity), &vTemp );

}

BOOLEAN PhysicsMoveObject( REAL_OBJECT *pObject )
{
	LEVELNODE *pNode;
	INT32 sNewGridNo, sTileIndex;
	ETRLEObject		*pTrav;
	HVOBJECT			hVObject;


	//Determine new gridno
	sNewGridNo = MAPROWCOLTOPOS( ( (INT16)pObject->Position.y / CELL_Y_SIZE ), ( (INT16)pObject->Position.x / CELL_X_SIZE ) );

	if ( pObject->fFirstTimeMoved )
	{
		pObject->fFirstTimeMoved = FALSE;
		pObject->sFirstGridNo		= sNewGridNo;
	}

	// CHECK FOR RANGE< IF INVALID, REMOVE!
	if ( sNewGridNo == -1 )
	{
		PhysicsDeleteObject( pObject );
		return( FALSE );
	}

	// Look at old gridno
	if ( sNewGridNo != pObject->sGridNo || pObject->pNode == NULL )
	{
		if ( pObject->fVisible )
		{
			if ( CheckForCatchObject( pObject ) )
			{
				pObject->fVisible = FALSE;
			}
		}

		if ( pObject->fVisible )
		{
			// Add smoke trails...
			//			if ( pObject->Obj.usItem == MORTAR_SHELL && pObject->uiNumTilesMoved > 2 && pObject->ubActionCode == THROW_ARM_ITEM )
			//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 1147"));

			if ( Item[pObject->Obj.usItem].usItemClass == IC_BOMB && pObject->uiNumTilesMoved > 2 && pObject->ubActionCode == THROW_ARM_ITEM )
			{
				if ( sNewGridNo != pObject->sGridNo )
				{
					ANITILE_PARAMS	AniParams;

					AniParams.sGridNo							= sNewGridNo;
					AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
					AniParams.sDelay							= (INT16)( 100 + PreRandom( 100 ) );
					AniParams.sStartFrame					= 0;
					AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
					AniParams.sX									= (INT16)pObject->Position.x;
					AniParams.sY									= (INT16)pObject->Position.y;
					AniParams.sZ									= (INT16)CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16)pObject->Position.z );

					strcpy( AniParams.zCachedFile, "TILECACHE\\MSLE_SMK.STI" );

					CreateAnimationTile( &AniParams );
				}
			}
			else if ( pObject->uiNumTilesMoved > 0 )
			{
				if ( sNewGridNo != pObject->sGridNo )
				{
					// We're at a new gridno!
					if ( pObject->pNode != NULL )
					{
						RemoveStructFromLevelNode( pObject->sLevelNodeGridNo, pObject->pNode );
					}

					// We're at a new gridno!
					if ( pObject->pShadow != NULL )
					{
						RemoveShadowFromLevelNode( pObject->sLevelNodeGridNo, pObject->pShadow );
					}

					// Now get graphic index
					sTileIndex = GetTileGraphicForItem( &(Item[ pObject->Obj.usItem ] ) );
					//sTileIndex = BULLETTILE1;

					// Set new gridno, add
					pNode = AddStructToTail( sNewGridNo, sTileIndex );
					pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
					pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					pNode->uiFlags |= ( LEVELNODE_USEABSOLUTEPOS | LEVELNODE_IGNOREHEIGHT | LEVELNODE_PHYSICSOBJECT | LEVELNODE_DYNAMIC );

					// Set levelnode
					pObject->pNode = pNode;

					// Add shadow
					AddShadowToHead( sNewGridNo, sTileIndex );
					pNode = gpWorldLevelData[ sNewGridNo ].pShadowHead;
					pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
					pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					pNode->uiFlags |= ( LEVELNODE_USEABSOLUTEPOS | LEVELNODE_IGNOREHEIGHT | LEVELNODE_PHYSICSOBJECT | LEVELNODE_DYNAMIC );

					// Set levelnode
					pObject->pShadow = pNode;

					pObject->sLevelNodeGridNo = sNewGridNo;
				}
			}
		}
		else
		{
			// Remove!
			if ( pObject->pNode != NULL )
			{
				RemoveStructFromLevelNode( pObject->sLevelNodeGridNo, pObject->pNode );
			}

			// We're at a new gridno!
			if ( pObject->pShadow != NULL )
			{
				RemoveShadowFromLevelNode( pObject->sLevelNodeGridNo, pObject->pShadow );
			}

			pObject->pNode		= NULL;
			pObject->pShadow	= NULL;
		}

		if ( sNewGridNo != pObject->sGridNo )
		{
			pObject->uiNumTilesMoved++;
		}

		pObject->sGridNo = sNewGridNo;

		if ( pObject->fPotentialForDebug )
		{
			PhysicsDebugMsg( String( "Object %d: uiNumTilesMoved: %d", pObject->iID, pObject->uiNumTilesMoved ) );
		}
	}

	if ( pObject->fVisible )
	{
		//		if ( pObject->Obj.usItem != MORTAR_SHELL || pObject->ubActionCode != THROW_ARM_ITEM )
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 1238"));

		if ( !(Item[pObject->Obj.usItem].usItemClass == IC_BOMB) || pObject->ubActionCode != THROW_ARM_ITEM )
		{
			if ( pObject->pNode != NULL )
			{
				// OK, get offsets
				hVObject = gTileDatabase[ pObject->pNode->usIndex ].hTileSurface;
				pTrav = &(hVObject->pETRLEObject[ gTileDatabase[ pObject->pNode->usIndex ].usRegionIndex ] );

				// Add new object / update position
				// Update position data
				pObject->pNode->sRelativeX	= (INT16)pObject->Position.x; // + pTrav->sOffsetX;
				pObject->pNode->sRelativeY	= (INT16)pObject->Position.y; // + pTrav->sOffsetY;
				pObject->pNode->sRelativeZ	= (INT16)CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16)pObject->Position.z );

				// Update position data
				pObject->pShadow->sRelativeX	= (INT16)pObject->Position.x; // + pTrav->sOffsetX;
				pObject->pShadow->sRelativeY	= (INT16)pObject->Position.y; // + pTrav->sOffsetY;
				pObject->pShadow->sRelativeZ	= (INT16)gpWorldLevelData[ pObject->sGridNo ].sHeight;
			}
		}
	}

	return( TRUE );
}



#if 0
{
	LEVELNODE *pNode;
	INT32 sNewGridNo;

	//Determine new gridno
	sNewGridNo = MAPROWCOLTOPOS( ( pObject->Position.y / CELL_Y_SIZE ), ( pObject->Position.x / CELL_X_SIZE ) );

	// Look at old gridno
	if ( sNewGridNo != pObject->sGridNo )
	{
		// We're at a new gridno!

		// First find levelnode of our object and delete
		pNode = gpWorldLevelData[ pObject->sGridNo ].pStructHead;

		// LOOP THORUGH OBJECT LAYER
		while( pNode != NULL )
		{
			if ( pNode->uiFlags & LEVELNODE_PHYSICSOBJECT )
			{
				if ( pNode->iPhysicsObjectID == pObject->iID )
				{
					RemoveStruct( pObject->sGridNo, pNode->usIndex );
					break;
				}
			}

			pNode = ppNode->pNext;
		}

		// Set new gridno, add
	}
	// Add new object / update position

}
#endif

void ObjectHitWindow( INT32 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce )
{
	EV_S_WINDOWHIT	SWindowHit;
	SWindowHit.sGridNo = sGridNo;
	SWindowHit.usStructureID = usStructureID;
	SWindowHit.fBlowWindowSouth = fBlowWindowSouth;
	SWindowHit.fLargeForce = fLargeForce;
	//AddGameEvent( S_WINDOWHIT, 0, &SWindowHit );

	WindowHit( sGridNo, usStructureID, fBlowWindowSouth, fLargeForce );

}


vector_3 FindBestForceForTrajectory( INT32 sSrcGridNo, INT32 sGridNo,INT16 sStartZ, INT16 sEndZ, real dzDegrees, OBJECTTYPE *pItem, INT32 *psGridNo, real *pdMagForce )
{
	vector_3		vDirNormal, vPosition, vForce;
	INT16				sDestX, sDestY, sSrcX, sSrcY;
	real				dForce		= 20;
	real				dRange;
	real				dPercentDiff = 0;
	real				dTestRange, dTestDiff;
	INT32				iNumChecks = 0;

	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( sSrcGridNo, &sSrcX, &sSrcY );

	// Set position
	vPosition.x = sSrcX;
	vPosition.y = sSrcY;
	vPosition.z = sStartZ;

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// NOmralize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dzDegrees );

	// Get range
	dRange = (float)GetRangeInCellCoordsFromGridNoDiff( sGridNo, sSrcGridNo );

	//calculate force needed
	dForce = (float)( 12.0 * ( sqrt( ( GRAVITY * dRange ) / sin( 2.0 * dzDegrees ) ) ) );

	do
	{
		// This first force is just an estimate...
		// now di a binary search to find best value....
		iNumChecks++;


		// Now use a force
		vForce.x = dForce * vDirNormal.x;
		vForce.y = dForce * vDirNormal.y;
		vForce.z = dForce * vDirNormal.z;

		dTestRange = CalculateObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, psGridNo );

		// What's the diff?
		dTestDiff = dTestRange - dRange;

		// How have we done?
		// < 5% off...
		if ( fabs( ( dTestDiff / dRange ) ) < .01 )
		{
			break;
		}

		if ( iNumChecks > MAX_INTEGRATIONS )
		{
			break;
		}

		// What is the Percentage difference?
		dPercentDiff = dForce * ( dTestDiff / dRange );

		// Adjust force accordingly
		dForce = dForce - ( ( dPercentDiff ) / 2 );

	} while( TRUE );

	if ( pdMagForce )
	{
		(*pdMagForce) = dForce;
	}

#ifdef JA2TESTVERSION
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Number of integration: %d", iNumChecks );
#endif

	return( vForce );
}


INT32 FindFinalGridNoGivenDirectionGridNoForceAngle( INT32 sSrcGridNo, INT32 sGridNo, INT16 sStartZ, INT16 sEndZ, real dForce, real dzDegrees, OBJECTTYPE *pItem )
{
	vector_3		vDirNormal, vPosition, vForce;
	INT16				sDestX, sDestY, sSrcX, sSrcY;
	real				dRange;
	INT32				sEndGridNo;


	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( sSrcGridNo, &sSrcX, &sSrcY );

	// Set position
	vPosition.x = sSrcX;
	vPosition.y = sSrcY;
	vPosition.z = sStartZ;

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// NOmralize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dzDegrees );

	// Get range
	dRange = (float)GetRangeInCellCoordsFromGridNoDiff( sGridNo, sSrcGridNo );

	// Now use a force
	vForce.x = dForce * vDirNormal.x;
	vForce.y = dForce * vDirNormal.y;
	vForce.z = dForce * vDirNormal.z;

	CalculateObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, &sEndGridNo );

	return( sEndGridNo );
}


real FindBestAngleForTrajectory( INT32 sSrcGridNo, INT32 sGridNo,INT16 sStartZ, INT16 sEndZ, real dForce, OBJECTTYPE *pItem, INT32 *psGridNo )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory");

	vector_3		vDirNormal, vPosition, vForce;
	INT16				sDestX, sDestY, sSrcX, sSrcY;
	real				dRange;
	real				dzDegrees = ( (float)PI/8 );
	real				dPercentDiff = 0;
	real				dTestRange, dTestDiff;
	INT32				iNumChecks = 0;


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: get range and direction");
	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( sSrcGridNo, &sSrcX, &sSrcY );

	// Set position
	vPosition.x = sSrcX;
	vPosition.y = sSrcY;
	vPosition.z = sStartZ;

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// NOmralize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dzDegrees );

	// Get range
	dRange = (float)GetRangeInCellCoordsFromGridNoDiff( sGridNo, sSrcGridNo );

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: find best direction");
	do
	{
		// This first direction is just an estimate...
		// now do a binary search to find best value....
		iNumChecks++;

		// Now use a force
		vForce.x = dForce * vDirNormal.x;
		vForce.y = dForce * vDirNormal.y;
		vForce.z = dForce * vDirNormal.z;

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: CalculateObjectTrajectory");
		dTestRange = CalculateObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, psGridNo );

		// What's the diff?
		dTestDiff = dTestRange - dRange;

		// How have we done?
		// < 5% off...
		if ( fabs( (FLOAT)( dTestDiff / dRange ) ) < .05 )
		{
			break;
		}

		if ( iNumChecks > MAX_INTEGRATIONS )
		{
			break;
		}

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: get percentage difference");
		// What is the Percentage difference?
		dPercentDiff = dzDegrees * ( dTestDiff / dRange );

		// Adjust degrees accordingly
		dzDegrees = dzDegrees - ( dPercentDiff / 2 );

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: check angle");
		// OK, If our angle is too far either way, giveup!
		if ( fabs( dzDegrees ) >= ( PI / 2 ) || fabs( dzDegrees ) <= 0.005 )
		{
			// Use 0.....
			dzDegrees = 0;
			// From degrees, calculate Z portion of normal
			vDirNormal.z	= (float)sin( dzDegrees );
			// Now use a force
			vForce.x = dForce * vDirNormal.x;
			vForce.y = dForce * vDirNormal.y;
			vForce.z = dForce * vDirNormal.z;
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: giving up CalculateObjectTrajectory");
			dTestRange = CalculateObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, psGridNo );
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory: giving up returning 0");
			return( (FLOAT)( dzDegrees ) );
		}


		// From degrees, calculate Z portion of normal
		vDirNormal.z	= (float)sin( dzDegrees );

	} while( TRUE );

	// OK, we have our force, calculate change to get through without collide
	//if ( ChanceToGetThroughObjectTrajectory( sEndZ, pItem, &vPosition, &vForce ) == 0 )
	//{
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Chance to get through throw is 0." );
	//}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FindBestAngleForTrajectory done");
	return( dzDegrees );
}


void FindTrajectory( INT32 sSrcGridNo, INT32 sGridNo, INT16 sStartZ, INT16 sEndZ, real dForce, real dzDegrees, OBJECTTYPE *pItem, INT32 *psGridNo )
{
	vector_3		vDirNormal, vPosition, vForce;
	INT16				sDestX, sDestY, sSrcX, sSrcY;

	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( sSrcGridNo, &sSrcX, &sSrcY );

	// Set position
	vPosition.x = sSrcX;
	vPosition.y = sSrcY;
	vPosition.z = sStartZ;

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// NOmralize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dzDegrees );

	// Now use a force
	vForce.x = dForce * vDirNormal.x;
	vForce.y = dForce * vDirNormal.y;
	vForce.z = dForce * vDirNormal.z;

	CalculateObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, psGridNo );
}



// OK, this will, given a target Z, INVTYPE, source, target gridnos, initial force vector, will
// return range

FLOAT CalculateObjectTrajectory( INT16 sTargetZ, OBJECTTYPE *pItem, vector_3 *vPosition, vector_3 *vForce, INT32 *psFinalGridNo )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory");
	INT32 iID;
	REAL_OBJECT *pObject;
	FLOAT	dDiffX, dDiffY;
	INT32 sGridNo;
	//int cnt=0;

	if ( psFinalGridNo )
	{
		(*psFinalGridNo) = NOWHERE;
	}


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory: createphysicalobject");
	// OK, create a physics object....
	iID = CreatePhysicalObject( pItem, -1, vPosition->x, vPosition->y, vPosition->z, vForce->x, vForce->y, vForce->z, NOBODY, NO_THROW_ACTION, 0, TEST_OBJECT_NO_COLLISIONS );

	if ( iID == -1 )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory: returning -1");
		return( -1 );
	}

	pObject = &( ObjectSlots[ iID ] );

	// Set some special values...
	pObject->fTestObject = TEST_OBJECT_NO_COLLISIONS;
	pObject->TestZTarget = sTargetZ;
	pObject->fTestPositionNotSet = TRUE;
	pObject->fVisible		= FALSE;

	// Alrighty, move this beast until it dies....
	while( pObject->fAlive )
	{
		//cnt = cnt + 1;
		//		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory: calling simulateobject - this object never fucking dies!!!!");
		SimulateObject( pObject, (float)DELTA_T );
	}

	// Calculate gridno from last position
	sGridNo = MAPROWCOLTOPOS( ( (INT16)pObject->Position.y / CELL_Y_SIZE ), ( (INT16)pObject->Position.x / CELL_X_SIZE ) );

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory: physicsdeleteobject");
	PhysicsDeleteObject( pObject );

	// get new x, y, z values
	dDiffX = ( pObject->TestTargetPosition.x - vPosition->x );
	dDiffY = ( pObject->TestTargetPosition.y - vPosition->y );

	if ( psFinalGridNo )
	{
		(*psFinalGridNo) = sGridNo;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateObjectTrajectory: done");
	return( (FLOAT)sqrt( ( dDiffX * dDiffX ) + ( dDiffY * dDiffY ) ) );

}


INT32 ChanceToGetThroughObjectTrajectory( INT16 sTargetZ, OBJECTTYPE *pItem, vector_3 *vPosition, vector_3 *vForce, INT32 *psNewGridNo, INT8 *pbLevel, BOOLEAN fFromUI )
{
	INT32 iID;
	REAL_OBJECT *pObject;

	// OK, create a physics object....
	iID = CreatePhysicalObject( pItem, -1, vPosition->x, vPosition->y, vPosition->z, vForce->x, vForce->y, vForce->z, NOBODY, NO_THROW_ACTION, 0, TEST_OBJECT_NOTWALLROOF_COLLISIONS );

	if ( iID == -1 )
	{
		return( -1 );
	}

	pObject = &( ObjectSlots[ iID ] );

	// Set some special values...
	pObject->fTestObject = TEST_OBJECT_NOTWALLROOF_COLLISIONS;
	pObject->fTestPositionNotSet = TRUE;
	pObject->TestZTarget = sTargetZ;
	pObject->fVisible		= FALSE;
	//pObject->fPotentialForDebug = TRUE;

	// Alrighty, move this beast until it dies....
	while( pObject->fAlive )
	{
		SimulateObject( pObject, (float)DELTA_T );
	}


	if ( psNewGridNo != NULL )
	{
		// Calculate gridno from last position

		// If NOT from UI, use exact collision position
		// JA25 comment from CJC:	testing breaklights, we ran into a situation where EndedWithCollisionPosition was not
		// set due to breaklight stopping on flat ground.	It makes sense that if EndedWithCollisionPosition is
		// unset, at least the Position value is of SOME use...
		if ( fFromUI || (pObject->EndedWithCollisionPosition.x == 0.0f && pObject->EndedWithCollisionPosition.y == 0.0f) )
		{
			(*psNewGridNo) = MAPROWCOLTOPOS( ( (INT16)pObject->Position.y / CELL_Y_SIZE ), ( (INT16)pObject->Position.x / CELL_X_SIZE ) );
		}
		else
		{
			(*psNewGridNo) = MAPROWCOLTOPOS( ( (INT16)pObject->EndedWithCollisionPosition.y / CELL_Y_SIZE ), ( (INT16)pObject->EndedWithCollisionPosition.x / CELL_X_SIZE ) );
		}

		(*pbLevel) = GET_OBJECT_LEVEL( pObject->EndedWithCollisionPosition.z - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ (*psNewGridNo) ].sHeight ) );
	}

	PhysicsDeleteObject( pObject );

	// See If we collided
	if ( pObject->fTestEndedWithCollision )
	{
		return( 0 );
	}
	return( 100 );

}




FLOAT CalculateLaunchItemAngle( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubHeight, real dForce, OBJECTTYPE *pItem, INT32 *psGridNo )
{
	real				dAngle;
	INT16				sSrcX, sSrcY;

	ConvertGridNoToCenterCellXY( pSoldier->sGridNo, &sSrcX, &sSrcY );

	dAngle = FindBestAngleForTrajectory( pSoldier->sGridNo, sGridNo, GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel ), ubHeight, dForce, pItem, psGridNo );

	// new we have defaut angle value...
	return( dAngle );
}



void CalculateLaunchItemBasicParams( SOLDIERTYPE *pSoldier, OBJECTTYPE *pItem, INT32 sGridNo, UINT8 ubLevel, INT16 sEndZ,  FLOAT *pdMagForce, FLOAT *pdDegrees, INT32 *psFinalGridNo, BOOLEAN fArmed )
{
	INT32		sInterGridNo = NOWHERE;
	INT16		sStartZ;
	FLOAT		dMagForce, dMaxForce, dMinForce;
	FLOAT		dDegrees, dNewDegrees;
	BOOLEAN	fThroughIntermediateGridNo = FALSE;
	UINT16	usLauncher;
	BOOLEAN	fIndoors = FALSE;
	BOOLEAN	fLauncher = FALSE;
	BOOLEAN	fMortar		= FALSE;
	BOOLEAN	fGLauncher = FALSE;
	INT16		sMinRange = 0;

	// Start with default degrees/ force
	dDegrees = OUTDOORS_START_ANGLE;
	sStartZ	= GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel );

	// Are we armed, and are we throwing a LAUNCHABLE?

	//MM: Again, this only works if the launchers are all the same
	//usLauncher = GetLauncherFromLaunchable( pItem->usItem );
	//MM: Replacement:
	OBJECTTYPE *pObj = NULL;
	usLauncher = 0;
	pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );
	if ( pObj != NULL )
	{
		if (Item[pObj->usItem].usItemClass == IC_LAUNCHER)
			usLauncher = pObj->usItem;
		else if (Item[pObj->usItem].usItemClass == IC_GUN)
		{
			usLauncher = GetAttachedGrenadeLauncher(pObj);
		}
	}
	if ( usLauncher == 0) // fail back to the original
		usLauncher = GetLauncherFromLaunchable( pItem->usItem );


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 1741"));
	if ( fArmed && ( Item[usLauncher].mortar || Item[pItem->usItem].mortar ) )
	{
		// Start at 0....
		sStartZ = ( pSoldier->pathing.bLevel * 256 );
		fMortar = TRUE;
		sMinRange = MIN_MORTAR_RANGE;
		//fLauncher = TRUE;
	}
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 1750"));

	if ( fArmed && ( Item[usLauncher].grenadelauncher || Item[pItem->usItem].grenadelauncher ) )
	{
		// OK, look at target level and decide angle to use...
		if ( ubLevel == 1 )
		{
			dDegrees	= GLAUNCHER_HIGHER_LEVEL_START_ANGLE;
		}
		else
		{
			dDegrees	= GLAUNCHER_START_ANGLE;
		}
		fGLauncher = TRUE;
		sMinRange	= MIN_MORTAR_RANGE/2;
		if ( (gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE] && pSoldier->bTeam == OUR_TEAM) || ( pSoldier->bTeam == ENEMY_TEAM && ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD || gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE ) ) )
	//	if ( (gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE] && pSoldier->bTeam == OUR_TEAM) || ( pSoldier->bTeam == ENEMY_TEAM && gGameOptions.ubDifficultyLevel >= DIF_LEVEL_HARD ) )
			dDegrees *= 2;
		//fLauncher = TRUE;
	}

	// CHANGE DEGREE VALUES BASED ON IF WE ARE INSIDE, ETC
	// ARE WE INSIDE?

	if ( gfCaves || gfBasement	)
	{
		// Adjust angle....
		dDegrees = INDOORS_START_ANGLE;
		fIndoors = TRUE;
	}

	if ( ( IsRoofPresentAtGridNo( pSoldier->sGridNo ) ) && pSoldier->pathing.bLevel == 0 )
	{
		// Adjust angle....
		dDegrees = INDOORS_START_ANGLE;
		fIndoors = TRUE;
	}

	// IS OUR TARGET INSIDE?
	if ( IsRoofPresentAtGridNo( sGridNo ) && ubLevel == 0 )
	{
		// Adjust angle....
		dDegrees = INDOORS_START_ANGLE;
		fIndoors = TRUE;
	}


	// OK, look if we can go through a windows here...
	if ( ubLevel == 0 )
	{
		sInterGridNo = SoldierToLocationWindowTest( pSoldier, sGridNo );

		// Lesh: fixing nasty freeze when throwing item through window
		// sometimes SoldierToLocationWindowTest returns same value as pSoldier->sGridNo
		// this leads to fault in FindBestAngleForTrajectory function and it subroutines loops
		// supplied by invalid data
		if ( pSoldier->sGridNo == sInterGridNo )
		{
			// bad news - i can't throw item at myself
			// so use dir incrementer
			UINT8	ubDir = atan8( CenterX(pSoldier->sGridNo), CenterY(pSoldier->sGridNo), CenterX(sGridNo), CenterY(sGridNo) );
			sInterGridNo += DirIncrementer[ubDir];
		}
	}
	else
	{
		sInterGridNo = NOWHERE;
	}
	
	if (!TileIsOutOfBounds(sInterGridNo))
	{
		// IF so, adjust target height, gridno....
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Through a window!" );

		fThroughIntermediateGridNo = TRUE;
	}

	if ( !fLauncher )
	{
		// Find force for basic
		FindBestForceForTrajectory( pSoldier->sGridNo, sGridNo, sStartZ, sEndZ, dDegrees, pItem, psFinalGridNo, &dMagForce );

		// Adjust due to max range....
		dMaxForce = CalculateSoldierMaxForce(pSoldier, dDegrees, pItem, fArmed, sGridNo, ubLevel);

		if ( fIndoors )
		{
			dMaxForce = dMaxForce * 2;
		}

		if ( dMagForce > dMaxForce )
		{
			dMagForce = dMaxForce;
		}

		// ATE: If we are a mortar, make sure we are at min.
		if ( fMortar || fGLauncher )
		{
			// find min force
			dMinForce = CalculateForceFromRange(pItem->usItem, (INT16)(sMinRange / 10), (FLOAT)(PI / 4), sGridNo, ubLevel);

			if ( dMagForce < dMinForce )
			{
				dMagForce = dMinForce;
			}
		}

		if ( fThroughIntermediateGridNo )
		{
			// Given this power, now try and go through this window....
			dDegrees = FindBestAngleForTrajectory( pSoldier->sGridNo, sInterGridNo, GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel ), 150, dMagForce, pItem, psFinalGridNo );
		}
	}
	else
	{
		// Use MAX force, vary angle....
		dMagForce = CalculateSoldierMaxForce(pSoldier, dDegrees, pItem, fArmed, sGridNo, ubLevel);

		if ( ubLevel == 0 )
		{
			dMagForce		=	(float)( dMagForce * 1.25 );
		}

		FindTrajectory( pSoldier->sGridNo, sGridNo, sStartZ, sEndZ, dMagForce, dDegrees, pItem, psFinalGridNo );

		if ( ubLevel == 1 && !fThroughIntermediateGridNo )
		{
			// Is there a guy here...?
			if ( WhoIsThere2( sGridNo, ubLevel ) != NOBODY )
			{
				dMagForce		=	(float)( dMagForce * 0.85 );

				// Yep, try to get angle...
				dNewDegrees = FindBestAngleForTrajectory( pSoldier->sGridNo, sGridNo, GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel ), 150, dMagForce, pItem, psFinalGridNo );

				if ( dNewDegrees != 0 )
				{
					dDegrees = dNewDegrees;
				}
			}
		}

		if ( fThroughIntermediateGridNo )
		{
			dDegrees = FindBestAngleForTrajectory( pSoldier->sGridNo, sInterGridNo, GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel ), 150, dMagForce, pItem, psFinalGridNo );
		}
	}


	(*pdMagForce) = dMagForce;
	(*pdDegrees )	= dDegrees;
}

BOOLEAN GrenadeRollingPossible(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 *sXPos, INT16 *sYPos)
{
	if (!(pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO))
	{		
		UINT8 ubDirection = GetDirectionFromGridNo(sGridNo, pSoldier);
		if (ubDirection % 2 == 1)//diagonal direction is disabled
		{
			return FALSE;
		}
		INT32 sTestGridNo = NewGridNo(pSoldier->sGridNo, DirectionInc(ubDirection));

		if (gubWorldMovementCosts[sTestGridNo][ubDirection][pSoldier->pathing.bLevel] == TRAVELCOST_WALL)
		{
			BOOLEAN obstacle = FALSE;
			INT16 newDir = (ubDirection != 2 && ubDirection != 6) ? EAST : SOUTH;
			INT32 newLoc = NewGridNo(pSoldier->sGridNo, DirectionInc(newDir));
			STRUCTURE *pStruct;
			pStruct = gpWorldLevelData[newLoc].pStructureHead;
			while (pStruct)
			{
				if ((pStruct->fFlags & STRUCTURE_ANYDOOR) && (pStruct->fFlags & STRUCTURE_OPEN))
				{
					if (gubWorldMovementCosts[newLoc][newDir][pSoldier->pathing.bLevel] >= 220)//doors are opened in a way they make an obstacle
						return FALSE;
					ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
					return TRUE;
				}
				pStruct = pStruct->pNext;
			}
			newLoc = NewGridNo(sTestGridNo, DirectionInc(newDir));
			pStruct = gpWorldLevelData[newLoc].pStructureHead;
			while (pStruct)
			{
				if ((pStruct->fFlags & STRUCTURE_ANYDOOR) && (pStruct->fFlags & STRUCTURE_OPEN))
				{
					ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
					return TRUE;
				}
				else if ((pStruct->fFlags & STRUCTURE_OBSTACLE))
				{
					obstacle = TRUE;
				}
				pStruct = pStruct->pNext;
			}
			if (!obstacle)
			{
				ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
				return TRUE;
			}
			obstacle = FALSE;
			newDir = (ubDirection != 2 && ubDirection != 6) ? WEST : NORTH;
			newLoc = NewGridNo(pSoldier->sGridNo, DirectionInc(newDir));
			pStruct = gpWorldLevelData[newLoc].pStructureHead;
			while (pStruct)
			{
				if ((pStruct->fFlags & STRUCTURE_ANYDOOR) && (pStruct->fFlags & STRUCTURE_OPEN))
				{
					if (gubWorldMovementCosts[newLoc][newDir][pSoldier->pathing.bLevel] >= 220)//doors are opened in a way they make an obstacle
						return FALSE;
					ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
					return TRUE;
				}
				pStruct = pStruct->pNext;
			}
			newLoc = NewGridNo(sTestGridNo, DirectionInc(newDir));
			pStruct = gpWorldLevelData[newLoc].pStructureHead;
			while (pStruct)
			{
				if ((pStruct->fFlags & STRUCTURE_ANYDOOR) && (pStruct->fFlags & STRUCTURE_OPEN))
				{
					ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
					return TRUE;
				}
				else if ((pStruct->fFlags & STRUCTURE_OBSTACLE))
				{
					obstacle = TRUE;
				}
				pStruct = pStruct->pNext;
			}
			if (!obstacle)
			{
				ConvertGridNoToCenterCellXY(newLoc, sXPos, sYPos);
				return TRUE;
			}
		}
	}
	return FALSE;
}


BOOLEAN CalculateLaunchItemChanceToGetThrough( SOLDIERTYPE *pSoldier, OBJECTTYPE *pItem, INT32 sGridNo, UINT8 ubLevel, INT16 sEndZ,	INT32 *psFinalGridNo, BOOLEAN fArmed, INT8 *pbLevel, BOOLEAN fFromUI )
{
	FLOAT				dForce, dDegrees;
	INT16				sDestX, sDestY, sSrcX, sSrcY;
	vector_3		vForce, vPosition, vDirNormal;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateLaunchItemChanceToGetThrough");

	if ( pSoldier->sGridNo == sGridNo )
	{
		printf("Warning! Soldier #%d attempted to launch item at himself\n", pSoldier->ubID);
		return FALSE;
	}
	// Ge7t basic launch params...
	CalculateLaunchItemBasicParams( pSoldier, pItem, sGridNo, ubLevel, sEndZ, &dForce, &dDegrees, psFinalGridNo, fArmed );

	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( pSoldier->sGridNo, &sSrcX, &sSrcY );

	if (GrenadeRollingPossible(pSoldier, sGridNo, &sSrcX, &sSrcY))
	{
		dForce /= 2;
	}

	// Set position
	vPosition.x = sSrcX;
	vPosition.y = sSrcY;
	vPosition.z = GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel );

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// NOmralize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dDegrees );

	// Do force....
	vForce.x = dForce * vDirNormal.x;
	vForce.y = dForce * vDirNormal.y;
	vForce.z = dForce * vDirNormal.z;

	// OK, we have our force, calculate change to get through without collide
	if ( ChanceToGetThroughObjectTrajectory( sEndZ, pItem, &vPosition, &vForce, psFinalGridNo, pbLevel, fFromUI ) == 0 )
	{
		return( FALSE );
	}

	if ( (*pbLevel) != ubLevel )
	{
		return( FALSE );
	}

	if ( !fFromUI && (*psFinalGridNo) != sGridNo )
	{
		return( FALSE );
	}

	return( TRUE );
}



FLOAT CalculateForceFromRange(UINT16 usItem, INT16 sRange, FLOAT dDegrees, INT32 sTargetSpot, UINT8 ubTargetLevel)
{
	FLOAT				dMagForce;
	INT32 sSrcGridNo, sDestGridNo;
	INT32 sFinalGridNo;

	// WANNE: This does not work, so I reverted back!
	// SB: Now this works on all map sizes, I've tested it!
	// OK, use a fake gridno, find the new gridno based on range, use height of merc, end height of ground,
	// 45 degrees
	sSrcGridNo	= INT32(WORLD_COLS/2+double(WORLD_COLS)*WORLD_ROWS/5.914);
	//shadooow: so it looks like it doesn't work in all maps afterall...
	//note that my hack will not work in custom made maps where neither 4408 and center GridNo aren't walkable, if such map exists then we need to solve this in some other way
	if (!GridNoOnWalkableWorldTile(sSrcGridNo))
	{
		sSrcGridNo = gMapInformation.sCenterGridNo;
	}
	sDestGridNo = sSrcGridNo + ( sRange * WORLD_COLS );

//	sSrcGridNo	= 4408;
//	sDestGridNo = 4408 + ( sRange * WORLD_COLS );

	// Buggler: impact explosives requiring larger force to reach desired range due to no bounce
	// Please change the if conditions too when definition of OBJECT_DETONATE_ON_IMPACT( object ) changes
	if ((Item[usItem].usItemClass == IC_BOMB) ||
		((Item[usItem].usItemClass & IC_EXPLOSV) && Explosive[Item[usItem].ubClassIndex].fExplodeOnImpact) ||
		Water(sTargetSpot, ubTargetLevel))
		// Use a mortar shell objecttype to simulate impact explosives
		CreateItem( MORTAR_SHELL, 100, &gTempObject );
	else
		// Use a grenade objecttype to simulate bouncy explosives
		CreateItem( HAND_GRENADE, 100, &gTempObject );

	FindBestForceForTrajectory( sSrcGridNo, sDestGridNo, GET_SOLDIER_THROW_HEIGHT( 0 ), 0, dDegrees, &gTempObject, &sFinalGridNo, &dMagForce );

	// WANNE: Problem is the FORCE!!
	// dMagForce = 460.00;

	return( dMagForce );
}


FLOAT CalculateSoldierMaxForce(SOLDIERTYPE *pSoldier, FLOAT dDegrees, OBJECTTYPE *pItem, BOOLEAN fArmed, INT32 sTargetSpot, UINT8 ubTargetLevel)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateSoldierMaxForce");

	INT32 uiMaxRange;
	FLOAT	dMagForce;

	dDegrees = (FLOAT)( PI/4 );

	uiMaxRange = CalcMaxTossRange( pSoldier, pItem->usItem, fArmed, pItem );

	dMagForce = CalculateForceFromRange(pItem->usItem, (INT16)uiMaxRange, dDegrees, sTargetSpot, ubTargetLevel);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateSoldierMaxForce: done");
	return( dMagForce );
}


void CalculateLaunchItemParamsForThrow( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubLevel, INT16 sEndZ, OBJECTTYPE *pItem, UINT32 uiHitChance, UINT8 ubActionCode, UINT32 uiActionData, UINT16 usItemNum )
{
	FLOAT				dForce, dDegrees;
	INT16				sDestX, sDestY, sSrcX, sSrcY;
	vector_3		vForce, vDirNormal;
	INT32 sFinalGridNo;
	BOOLEAN			fArmed = FALSE;
	UINT16			usLauncher;
	INT16				sStartZ;
	INT8		bMinMissRadius, bMaxMissRadius, bMaxRadius;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateLaunchItemParamsForThrow");

	// Set target ID if anyone
	pSoldier->ubTargetID = WhoIsThere2( sGridNo, ubLevel );

	if ( ubActionCode == THROW_ARM_ITEM )
	{
		fArmed = TRUE;
	}

	// set the max miss radius
	if ( Item[usItemNum].mortar )
	{
		bMaxRadius = gItemSettings.usMissMaxRadiusMortar;
	}
	else
	{
		bMaxRadius = gItemSettings.usMissMaxRadiusGrenade;
	}

	// calculate the actual min and max miss radius based on CtH
	bMinMissRadius = (INT8)((FLOAT)bMaxRadius * (10.0f - sqrt((FLOAT)uiHitChance)) / 10.0f);
	bMaxMissRadius = bMaxRadius * ( 100 - uiHitChance ) / 100;

	// modify by a bit of luck
	bMinMissRadius = __max( (bMinMissRadius - (INT8)Random(2)), 0);
	bMinMissRadius = __min( (bMinMissRadius + (INT8)Random(2)), bMaxRadius);

	bMaxMissRadius = __max( (bMaxMissRadius - (INT8)Random(2)), 0);
	bMaxMissRadius = __min( (bMaxMissRadius + (INT8)Random(2)), bMaxRadius);

	// min miss radius cannot be greater than max miss radius
	bMinMissRadius = __min( bMinMissRadius, bMaxMissRadius);

	sGridNo = RandomGridFromRadius( sGridNo, bMinMissRadius, bMaxMissRadius );

	// Get basic launch params...
	CalculateLaunchItemBasicParams( pSoldier, pItem, sGridNo, ubLevel, sEndZ, &dForce, &dDegrees, &sFinalGridNo, fArmed );

	// Get XY from gridno
	ConvertGridNoToCenterCellXY( sGridNo, &sDestX, &sDestY );
	ConvertGridNoToCenterCellXY( pSoldier->sGridNo, &sSrcX, &sSrcY );

	if (GrenadeRollingPossible(pSoldier, sGridNo, &sSrcX, &sSrcY))
	{
		dForce /= 2;
	}

	// OK, get direction normal
	vDirNormal.x = (float)(sDestX - sSrcX);
	vDirNormal.y = (float)(sDestY - sSrcY);
	vDirNormal.z = 0;

	// Normalize
	vDirNormal = VGetNormal( &vDirNormal );

	// From degrees, calculate Z portion of normal
	vDirNormal.z	= (float)sin( dDegrees );

	// Do force....
	vForce.x = dForce * vDirNormal.x;
	vForce.y = dForce * vDirNormal.y;
	vForce.z = dForce * vDirNormal.z;


	// Allocate Throw Parameters
	pSoldier->pThrowParams = (THROW_PARAMS *) MemAlloc( sizeof( THROW_PARAMS ) );
	memset( pSoldier->pThrowParams, 0, sizeof( THROW_PARAMS ) );

	OBJECTTYPE::CopyToOrCreateAt( &pSoldier->pTempObject, pItem);
	pSoldier->pThrowParams->dX = (float)sSrcX;
	pSoldier->pThrowParams->dY = (float)sSrcY;


	sStartZ = GET_SOLDIER_THROW_HEIGHT( pSoldier->pathing.bLevel );

	//MM: Again, this only works if the launchers are all the same
	//usLauncher = GetLauncherFromLaunchable( pItem->usItem );
	//MM: Replacement:
	OBJECTTYPE *pObj = NULL;
	usLauncher = 0;
	pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );
	if ( pObj != NULL )
	{
		if (Item[pObj->usItem].usItemClass == IC_LAUNCHER)
			usLauncher = pObj->usItem;
		else if (Item[pObj->usItem].usItemClass == IC_GUN)
		{
			usLauncher = GetAttachedGrenadeLauncher(pObj);
		}
	}
	if ( usLauncher == 0) // fail back to the original
		usLauncher = GetLauncherFromLaunchable( pItem->usItem );


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 2103"));

	if ( fArmed && Item[usLauncher].mortar )
	{
		// Start at 0....
		sStartZ = ( pSoldier->pathing.bLevel * 256 ) + 50;
	}

	pSoldier->pThrowParams->dZ = (float)sStartZ;
	pSoldier->pThrowParams->dForceX = vForce.x;
	pSoldier->pThrowParams->dForceY = vForce.y;
	pSoldier->pThrowParams->dForceZ = vForce.z;
	pSoldier->pThrowParams->dLifeSpan = -1;
	pSoldier->pThrowParams->ubActionCode = ubActionCode;
	pSoldier->pThrowParams->uiActionData = uiActionData;

	// Dirty interface
	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CalculateLaunchItemParamsForThrow done");
}


BOOLEAN CheckForCatcher( REAL_OBJECT *pObject, UINT16 usStructureID )
{
	// Do we want to catch?
	if ( pObject->fTestObject ==	NO_TEST_OBJECT )
	{
		if ( pObject->ubActionCode == THROW_TARGET_MERC_CATCH )
		{
			// Is it a guy?
			if ( usStructureID < INVALID_STRUCTURE_ID )
			{
				//Is it the same guy?
				if ( usStructureID == pObject->uiActionData )
				{
					if ( DoCatchObject( pObject ) )
					{
						pObject->fAlive = FALSE;
						return( TRUE );
					}
				}
			}
		}
	}
	return( FALSE );
}


void CheckForObjectHittingMerc( REAL_OBJECT *pObject, UINT16 usStructureID )
{
	SOLDIERTYPE *pSoldier;
	INT16		sDamage, sBreath;

	// Do we want to catch?
	if ( pObject->fTestObject ==	NO_TEST_OBJECT )
	{
		// Is it a guy?
		if ( usStructureID < INVALID_STRUCTURE_ID )
		{
			if ( pObject->ubLastTargetTakenDamage != (UINT8)usStructureID )
			{
				// Flugente: if this fails, something is very wrong indeed
				Assert(usStructureID<TOTAL_SOLDIERS);

				pSoldier = MercPtrs[ usStructureID ];

				// silversurfer: Don't hurt civilians. Throwing objects at civilians to kill them is a lame exploit.
				if ( pSoldier->aiData.bNeutral )
					sDamage = 0;
				else
					sDamage = 1;

				sBreath = 0;

				pSoldier->EVENT_SoldierGotHit( NOTHING, sDamage, sBreath, pSoldier->ubDirection, 0, pObject->ubOwner, FIRE_WEAPON_TOSSED_OBJECT_SPECIAL, 0, 0, NOWHERE );

				pObject->ubLastTargetTakenDamage = (UINT8)( usStructureID );
			}
		}
	}
}


BOOLEAN CheckForCatchObject( REAL_OBJECT *pObject )
{
	SOLDIERTYPE *pSoldier;
	UINT32			uiSpacesAway;

	// Do we want to catch?
	if ( pObject->fTestObject ==	NO_TEST_OBJECT )
	{
		if ( pObject->ubActionCode == THROW_TARGET_MERC_CATCH )
		{
			pSoldier = MercPtrs[ pObject->uiActionData ];

			// Is it a guy?
			// Are we close to this guy?
			uiSpacesAway = PythSpacesAway( pObject->sGridNo, pSoldier->sGridNo );

			if ( uiSpacesAway < 4 && !pObject->fAttemptedCatch )
			{
				if ( pSoldier->usAnimState != CATCH_STANDING &&
					pSoldier->usAnimState != CATCH_CROUCHED &&
					pSoldier->usAnimState != LOWER_RIFLE )
				{
					if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND )
					{
						pSoldier->EVENT_InitNewSoldierAnim( CATCH_STANDING, 0 , FALSE );
					}
					else if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH )
					{
						pSoldier->EVENT_InitNewSoldierAnim( CATCH_CROUCHED, 0 , FALSE );
					}

					pObject->fCatchAnimOn = TRUE;
				}
			}

			pObject->fAttemptedCatch = TRUE;

			if ( uiSpacesAway <= 1 && !pObject->fCatchCheckDone )
			{
				if ( AttemptToCatchObject( pObject ) )
				{
					return( TRUE );
				}
			}
		}
	}
	return( FALSE );
}




BOOLEAN AttemptToCatchObject( REAL_OBJECT *pObject )
{
	SOLDIERTYPE *pSoldier;
	UINT8				ubChanceToCatch;

	// Get intended target
	pSoldier = MercPtrs[ pObject->uiActionData ];

	// OK, get chance to catch
	// base it on...? CC? Dexterity?
	ubChanceToCatch = 50 + EffectiveDexterity( pSoldier, FALSE ) / 2;

#ifdef JA2TESTVERSION
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Chance To Catch: %d", ubChanceToCatch );
#endif

	pObject->fCatchCheckDone = TRUE;

	if ( PreRandom( 100 ) > ubChanceToCatch )
	{
		return( FALSE );
	}

	pObject->fCatchGood = TRUE;

	return( TRUE );
}


BOOLEAN DoCatchObject( REAL_OBJECT *pObject )
{
	SOLDIERTYPE *pSoldier;
	BOOLEAN			fGoodCatch = FALSE;
	UINT16			usItem;

	// Get intended target
	pSoldier = MercPtrs[ pObject->uiActionData ];

	// Catch anim.....
	switch( gAnimControl[ pSoldier->usAnimState ].ubHeight )
	{
	case ANIM_STAND:

		pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
		pSoldier->EVENT_InitNewSoldierAnim( END_CATCH, 0 , FALSE );
		break;

	case ANIM_CROUCH:

		pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
		pSoldier->EVENT_InitNewSoldierAnim( END_CROUCH_CATCH, 0 , FALSE );
		break;
	}

	PlayJA2Sample( CATCH_OBJECT, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

	pObject->fCatchAnimOn = FALSE;

	if ( !pObject->fCatchGood )
	{
		return( FALSE );
	}

	// Get item
	usItem = pObject->Obj.usItem;

	// Transfer object
	fGoodCatch = AutoPlaceObject( pSoldier, &(pObject->Obj), TRUE );

	// Report success....
	if ( fGoodCatch )
	{
		pObject->fDropItem = FALSE;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_MERC_CAUGHT_ITEM ], pSoldier->GetName(), ShortItemNames[ usItem ] );
	}

	return( TRUE );
}


void HandleArmedObjectImpact( REAL_OBJECT *pObject )
{
	INT16		sZ;
	BOOLEAN		fDoImpact = FALSE;
	BOOLEAN		fCheckForDuds = FALSE;
	bool		fIsDud = FALSE;
	OBJECTTYPE	*pObj;
	INT32		iTrapped = 0;
	UINT16		usFlags = 0;
	INT8		bLevel = 0;

	// sevenfm: can delay explosion for normal, stun and flashbang type grenades
	BOOLEAN		fCanDelayExplosion = FALSE;
	BOOLEAN		fGoodStatus = FALSE;
	BOOLEAN		fDelayedExplosion = FALSE;

	if (is_networked && is_client)
	{
		if (pObject->mpIsFromRemoteClient && !pObject->mpHaveClientResult)
		{
			return;
		}
	}

	// Calculate pixel position of z
	sZ = (INT16)CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16)( pObject->Position.z ) ) - gpWorldLevelData[ pObject->sGridNo ].sHeight;

	// get OBJECTTYPE
	pObj = &(pObject->Obj);

	// ATE: Make sure number of objects is 1...
	pObj->ubNumberOfObjects = 1;

	if (Item[pObj->usItem].usItemClass & IC_GRENADE)
	{
		fCheckForDuds = TRUE;

		if (CanDelayGrenadeExplosion(pObj->usItem) && (Item[pObj->usItem].ubCursor == TOSSCURS || Item[pObj->usItem].glgrenade))
		{
			fCanDelayExplosion = TRUE;
		}

		if ((*pObj)[0]->data.sObjectFlag & DELAYED_GRENADE_EXPLOSION)
		{
			fDelayedExplosion = TRUE;
		}
	}

	//	if ( pObj->usItem == MORTAR_SHELL )
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("physics.cpp line 2337"));

	if (Item[ pObj->usItem ].usItemClass == IC_BOMB)
	{
		fCheckForDuds = TRUE;
	}

	if ( Item[ pObj->usItem ].usItemClass & IC_THROWN	)
	{
		AddItemToPool( pObject->sGridNo, pObj, INVISIBLE, bLevel, usFlags, 0 );
	}

	if ( fCheckForDuds && (*pObj)[0]->data.misc.bDetonatorType != BOMB_TIMED )
	{
		if( (*pObj)[0]->data.objectStatus >= USABLE && 
			PreRandom(100) < (UINT32) (*pObj)[0]->data.objectStatus + PreRandom( 50 ) )
		{
			fGoodStatus = TRUE;
		}

		// OJW - 20021002 - MP Explosives
		if (is_networked && is_client && pObject->mpIsFromRemoteClient && pObject->mpHaveClientResult)
		{
			fIsDud = pObject->mpWasDud;
		}
		else
		{
			fDoImpact = TRUE;
			fIsDud = false;

			// Flugente: explosion does not happen instantly if grenade
			// is not in water and
			// either has a bad status or is a delayed grenade
			if (!pObject->fInWater &&
				(!fGoodStatus || fCanDelayExplosion && (gGameExternalOptions.fDelayedGrenadeExplosion || fDelayedExplosion)))
			{
				// didn't go off!
				fDoImpact = FALSE;
				fIsDud = true;
			}
		}

		if (fIsDud)
		{
			// Start timed bomb...
			usFlags |= WORLD_ITEM_ARMED_BOMB;

			(*pObj)[0]->data.misc.bDetonatorType = BOMB_TIMED;
			if( !fGoodStatus )
			{
				(*pObj)[0]->data.misc.bDelay = (INT8)( 1 + PreRandom( 2 ) );
			}
			else
			{
				(*pObj)[0]->data.misc.bDelay = 1;

				// for non-player grenades, add turn so player could disarm grenade or run away
				if ( pObject->ubOwner != NOBODY && MercPtrs[pObject->ubOwner]->bTeam != gbPlayerNum )
				{
					(*pObj)[0]->data.misc.bDelay++;
				}
			}
			
			(*pObj)[0]->data.misc.usBombItem = pObj->usItem;

			(*pObj).fFlags |= OBJECT_ARMED_BOMB;
			(*pObj).fFlags |= OBJECT_KNOWN_TO_BE_TRAPPED;

			// set high trap level
			(*pObj)[0]->data.bTrap = 10;

			// ATE: If we have collided with roof last...
			if ( pObject->iOldCollisionCode == COLLISION_ROOF )
			{
				bLevel = 1;
			}

			// Add item to pool....
			AddItemToPool( pObject->sGridNo, pObj, VISIBLE, bLevel, usFlags, 0 );

			// All teams look for this...
			NotifySoldiersToLookforItems( );

			if ( pObject->ubOwner != NOBODY && !fGoodStatus )
			{
				MercPtrs[ pObject->ubOwner ]->DoMercBattleSound( (INT8)( BATTLE_SOUND_CURSE1 ) );
			}
		}
	}
	else
	{
		fDoImpact = TRUE;
	}

	// Flugente: additional dialogue
	// any merc nearby that can see this can warn us
	if ( ( *pObj )[0]->data.misc.bDelay > 0 && pObject->ubOwner != NOBODY )//&& MercPtrs[pObject->ubOwner]->bTeam != gbPlayerNum )
	{
		AdditionalTacticalCharacterDialogue_AllInSectorRadiusCall( NO_PROFILE, ADE_GRENADEWARNING, pObj->usItem, 0, 0, pObject->sGridNo, 5 );
	}

	if ( fDoImpact )
	{
		if ( Item[pObject->Obj.usItem].flare )
		{
			//if the light object will be created OFF the ground
			if( pObject->Position.z > 0 && FindBuilding(pObject->sGridNo) )
			{
				//we cannot create the light source above the ground, or on a roof.	The system doesnt support it.
				AddItemToPool( pObject->sGridNo, &( pObject->Obj ), 1, 1, 0, -1 );
			}
			else
			{
				// Add a light effect...
				NewLightEffect( pObject->sGridNo, (UINT8)Explosive[Item[pObject->Obj.usItem].ubClassIndex].ubDuration , (UINT8)Explosive[Item[pObject->Obj.usItem].ubClassIndex].ubStartRadius );
			}
		}
		else if ( Item[ pObject->Obj.usItem ].usItemClass & IC_GRENADE	)
		{
			if( (*pObj)[0]->data.misc.bDetonatorType != BOMB_TIMED )
			{
				IgniteExplosion( pObject->ubOwner, (INT16)pObject->Position.x, (INT16)pObject->Position.y, sZ, pObject->sGridNo, pObject->Obj.usItem, GET_OBJECT_LEVEL( pObject->Position.z - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pObject->sGridNo ].sHeight ) ), DIRECTION_IRRELEVANT, &pObject->Obj );
			}
		}
		else if ( Item[ pObject->Obj.usItem ].usItemClass == IC_BOMB	)
		{
			sZ = (INT16)CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16)pObject->Position.z );

			IgniteExplosion( pObject->ubOwner, (INT16)pObject->Position.x, (INT16)pObject->Position.y, sZ, pObject->sGridNo, pObject->Obj.usItem, GET_OBJECT_LEVEL( pObject->Position.z - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pObject->sGridNo ].sHeight ) ) );
		}
	}

	// OJW - 20021002 - MP Explosives
	if (is_networked && is_client && !pObject->mpIsFromRemoteClient)
	{
		// send results to other clients
		send_grenade_result( (INT16)pObject->Position.x, (INT16)pObject->Position.y, sZ , pObject->sGridNo , pObject->ubOwner, pObject->iID, fIsDud);
	}

}

BOOLEAN	SavePhysicsTableToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten=0;
	UINT16	usCnt=0;
	UINT32	usPhysicsCount=0;

	for( usCnt=0; usCnt<NUM_OBJECT_SLOTS; usCnt++ )
	{
		//if the REAL_OBJECT is active, save it
		if( ObjectSlots[ usCnt ].fAllocated )
		{
			usPhysicsCount++;
		}
	}


	//Save the number of REAL_OBJECTs in the array
	FileWrite( hFile, &usPhysicsCount, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	if( usPhysicsCount != 0 )
	{
		for( usCnt=0; usCnt<NUM_OBJECT_SLOTS; usCnt++ )
		{
			//if the REAL_OBJECT is active, save it
			if( ObjectSlots[ usCnt ].fAllocated )
			{
				//Save the the REAL_OBJECT structure
				if ( !ObjectSlots[usCnt].Save(hFile) )
				{
					return( FALSE );
				}
			}
		}
	}

	return( TRUE );
}


BOOLEAN	LoadPhysicsTableFromSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead=0;
	UINT16	usCnt=0;

	//make sure the objects are not allocated
	for (int x = 0; x < NUM_OBJECT_SLOTS; ++x)
	{
		ObjectSlots[x].initialize();
	}

	//Load the number of REAL_OBJECTs in the array
	FileRead( hFile, &guiNumObjectSlots, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}

	//loop through and add the objects
	for( usCnt=0; usCnt<guiNumObjectSlots; usCnt++ )
	{
		//Load the the REAL_OBJECT structure
		if ( !ObjectSlots[usCnt].Load(hFile) )
		{
			return( FALSE );
		}

		ObjectSlots[usCnt].pNode = NULL;
		ObjectSlots[usCnt].pShadow = NULL;
		ObjectSlots[usCnt].iID = usCnt;
	}

	return( TRUE );
}


INT32 RandomGridFromRadius( INT32 sSweetGridNo, INT8 ubMinRadius, INT8 ubMaxRadius )
{
	INT16		sX, sY;
	INT32		sGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	UINT32		cnt = 0;

	// just a precaution...
	ubMaxRadius = __max( ubMaxRadius, 0 );
	ubMinRadius = __max( __min( ubMinRadius, ubMaxRadius ), 0 );

	if ( ubMaxRadius == 0)
	{
		return( sSweetGridNo );
	}

	do
	{
		sX = (UINT16)PreRandom( ubMaxRadius + 1 );
		sY = (UINT16)PreRandom( ubMaxRadius + 1 );

		if ( ( sX < ubMinRadius || sY < ubMinRadius ) && ubMaxRadius != ubMinRadius )
		{
			continue;
		}

		if ( PreRandom( 2 ) == 0 )
		{
			sX = sX * -1;
		}

		if ( PreRandom( 2 ) == 0 )
		{
			sY = sY * -1;
		}

		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * sY ) )/ WORLD_COLS ) * WORLD_COLS;

		sGridNo = sSweetGridNo + ( WORLD_COLS * sY ) + sX;

		if ( sGridNo == sSweetGridNo )
		{
			continue;
		}

		if ( sGridNo >=0 && sGridNo < WORLD_MAX &&
			sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
		{
			fFound = TRUE;
		}

		cnt++;

		if ( cnt > 50 )
		{
			return( NOWHERE );
		}

	} while( !fFound );

	return( sGridNo );
}

UINT32 GetArtilleryTargetGridNo( UINT32 sTargetGridNo, INT8 bRadius )
{
	return RandomGridFromRadius( sTargetGridNo, 1, bRadius );
}

BOOLEAN GetArtilleryLaunchParams(UINT32 sStartingGridNo, UINT32 sTargetGridNo, UINT8 ubTargetLevel, INT16 sStartZ, INT16 sEndZ, UINT16 usLauncher, OBJECTTYPE* pObj, FLOAT* pdForce, FLOAT* pdDegrees)
{
	FLOAT		dMagForce, dMaxForce, dMinForce;
	FLOAT		dDegrees		= OUTDOORS_START_ANGLE;
	INT16		sMinRange		= MIN_MORTAR_RANGE;

	sStartZ			= 256;

	// Find force for basic
	INT32 sFinalGridNo = 0;
	FindBestForceForTrajectory( sStartingGridNo, sTargetGridNo, sStartZ, sEndZ, dDegrees, pObj, &sFinalGridNo, &dMagForce );

	INT32 uiMaxRange = GetModifiedGunRange(usLauncher) / CELL_X_SIZE;

	dMaxForce = CalculateForceFromRange(NULL, (INT16)uiMaxRange, (FLOAT)(PI / 4), sTargetGridNo, ubTargetLevel);
			
	if ( dMagForce > dMaxForce )
		dMagForce = dMaxForce;

	dMinForce = CalculateForceFromRange(NULL, (INT16)(sMinRange / 10), (FLOAT)(PI / 4), sTargetGridNo, ubTargetLevel);

	if ( dMagForce < dMinForce )
		dMagForce = dMinForce;
		
	(*pdForce)		= dMagForce;
	(*pdDegrees) 	= dDegrees;

	return TRUE;
}
