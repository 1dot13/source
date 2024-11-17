#ifndef __BULLETS_H
#define __BULLETS_H

#include "types.h"
#include "Soldier Control.h"
#include "los.h"
#include "tile animation.h"


#define		BULLET_FLAG_CREATURE_SPIT			0x0001
#define		BULLET_FLAG_KNIFE					0x0002
#define		BULLET_FLAG_MISSILE					0x0004
#define		BULLET_FLAG_SMALL_MISSILE			0x0008
#define		BULLET_STOPPED						0x0010
#define		BULLET_FLAG_TANK_CANNON				0x0020
#define		BULLET_FLAG_BUCKSHOT				0x0040
#define		BULLET_FLAG_FLAME					0x0080
#define		BULLET_FLAG_TRACER					0x0100
#define		BULLET_FLAG_ANTIMATERIEL			0x0200		// Flugente: bullet can destroy structures
#define		BULLET_FLAG_INFECTED				0x0400		// Flugente: bullet is infected
#define		BULLET_FLAG_WHITESMOKE				0x0800		// Flugente: bullet leaves a white smoke trail (for fire extinguisher)

//afp-start calculate line points between two point
#define BULLET_TRACER_MAX_LENGTH 60
void PointsPath(int sx1, int sy1, int ex2, int ey2) ;
//afp-end
typedef struct
{
	//afp-start this structure keeps tracers points
	LEVELNODE* pNodes[BULLET_TRACER_MAX_LENGTH];
	//afp-end
	INT32		iBullet;
	SoldierID	ubFirerID;
	SoldierID	ubTargetID;
	INT8			bStartCubesAboveLevelZ;
	INT8			bEndCubesAboveLevelZ;
	INT32		sGridNo;
	INT16		sUnused;
	UINT16		usLastStructureHit;
	FIXEDPT		qCurrX;
	FIXEDPT		qCurrY;
	FIXEDPT		qCurrZ;
	FIXEDPT		qIncrX;
	FIXEDPT		qIncrY;
	FIXEDPT		qIncrZ;
	DOUBLE		ddHorizAngle;
	INT32		iCurrTileX;
	INT32		iCurrTileY;
	INT8			bLOSIndexX;
	INT8			bLOSIndexY;
	BOOLEAN		fCheckForRoof;
	INT32		iCurrCubesZ;
	INT32		iLoop;
	BOOLEAN		fAllocated;
	BOOLEAN		fToDelete;
	BOOLEAN		fLocated;
	BOOLEAN		fReal;
	BOOLEAN		fAimed;
	UINT32		uiLastUpdate;
	UINT8		ubTilesPerUpdate;
	UINT16		usClockTicksPerUpdate;
	SOLDIERTYPE	*pFirer;			// Flugente: WARNING! We cannot assume this will be valid. Check wether ubFirerID is NOBODY first
	INT32		sOrigGridNo;	// HEADROCK HAM 5.1: Original tile does not necessarily have to be the pFirer's tile
	INT32		sTargetGridNo;
	INT16		sHitBy;
	INT32		iImpact;
	INT32		iImpactReduction;
	INT32		iRange;
	INT32		iDistanceLimit;
	UINT16		usFlags;
	ANITILE		*pAniTile;
	ANITILE		*pShadowAniTile;
	UINT16		ubItemStatus;
	UINT16		fromItem;
	INT32		flash;
	// HEADROCK HAM B2.5: Experimental bullet tracer info. If the new tracer system is activated, this flag tells
	// the bullet animation functions to create a lightpath for this bullet. This means now all bullets in a tracer
	// magazine will cause a lightshow (that's the intended result).
	BOOLEAN fTracer;
	// HEADROCK HAM 5: Fragments. They are handled slightly differently by the bullet functions.
	BOOLEAN fFragment;
} BULLET;

extern UINT32 guiNumBullets;

INT32	CreateBullet( SoldierID ubFirerID, BOOLEAN fFake, UINT16 usFlags,UINT16 fromItem );
void	RemoveBullet( INT32 iBullet );
void	StopBullet( INT32 iBullet );
void	UpdateBullets( );
BULLET *GetBulletPtr( INT32 iBullet );
INT32 BulletImpact( SOLDIERTYPE *pFirer, BULLET *pBullet, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, INT16 sHitBy, UINT8 * pubSpecial );


void DeleteAllBullets( );

void LocateBullet( INT32 iBulletIndex );

void HandleBulletSpecialFlags( INT32 iBulletIndex );

void AddMissileTrail( BULLET *pBullet, FIXEDPT qCurrX, FIXEDPT qCurrY, FIXEDPT qCurrZ );

//Save the bullet table to the saved game file
BOOLEAN SaveBulletStructureToSaveGameFile( HWFILE hFile );


//Load the bullet table from the saved game file
BOOLEAN LoadBulletStructureFromSavedGameFile( HWFILE hFile );


#endif
