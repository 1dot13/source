#ifndef LOS_H
#define LOS_H

//#define LOS_DEBUG
#include "WorldDat.h" // needed for MAX_TILESETS

// fixed-point arithmetic definitions start here

// WANNE - BMP: BUGFIX: Replaced INT32 with INT64, because otherwise we would get an overflow on big maps!
//typedef INT32 FIXEDPT;
typedef INT64 FIXEDPT;			



// rem 1 signed bit at the top
#define FIXEDPT_WHOLE_BITS 11
#define FIXEDPT_FRACTIONAL_BITS 20
#define FIXEDPT_FRACTIONAL_RESOLUTION 1048576

#define INT32_TO_FIXEDPT( n ) ( (n) << FIXEDPT_FRACTIONAL_BITS )
#define FIXEDPT_TO_INT32( n ) ( (n) / FIXEDPT_FRACTIONAL_RESOLUTION )

#define FIXEDPT_TO_TILE_NUM( n ) (FIXEDPT_TO_INT32( (n) ) / CELL_X_SIZE)
#define FIXEDPT_TO_LOS_INDEX( n ) (CONVERT_WITHINTILE_TO_INDEX( FIXEDPT_TO_INT32( (n) ) % CELL_X_SIZE))

// fixed-point arithmetic definitions end here

#define	OK_CHANCE_TO_GET_THROUGH		10

enum CollisionEnums
{
	COLLISION_NONE,
	COLLISION_GROUND,
	COLLISION_MERC,
	COLLISION_WINDOW_SOUTHEAST,
	COLLISION_WINDOW_SOUTHWEST,
	COLLISION_WINDOW_NORTHEAST,
	COLLISION_WINDOW_NORTHWEST,
	COLLISION_WINDOW_NORTH,
	COLLISION_WALL_SOUTHEAST,
	COLLISION_WALL_SOUTHWEST,
	COLLISION_WALL_NORTHEAST,
	COLLISION_WALL_NORTHWEST,
	COLLISION_STRUCTURE,
	COLLISION_ROOF,
	COLLISION_INTERIOR_ROOF,
	COLLISION_STRUCTURE_Z,
	COLLISION_WATER,

} ;

enum {
	LOS_POS,
	FIRING_POS,
	TARGET_POS,
	HEAD_TARGET_POS,
	TORSO_TARGET_POS,
	LEGS_TARGET_POS,
	HEIGHT
};


INT32	CheckForCollision( FLOAT dX, FLOAT dY, FLOAT dZ, FLOAT dDeltaX, FLOAT dDeltaY, FLOAT dDeltaZ, INT16 *pusStructureID, FLOAT *pdNormalX, FLOAT *pdNormalY, FLOAT *pdNormalZ );


INT8 ChanceToGetThrough( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ );
INT8 FireBulletGivenTarget( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usHandItem, INT16 sHitBy, BOOLEAN fBuckshot, BOOLEAN fFake );
// HEADROCK HAM 4: Changed the name of one argument to avoid confusion with the new CTH system.
INT8 FireBulletGivenTargetNCTH( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usHandItem, INT16 sApertureRatio, BOOLEAN fBuckshot, BOOLEAN fFake );
// HEADROCK HAM 5: Function for fragments ejected from an explosion.
INT8 FireFragmentGivenTarget( SOLDIERTYPE * pThrower, FLOAT dStartX, FLOAT dStartY, FLOAT dStartZ, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usExplosiveItem );

// Flugente: fire a shot from a gun that has no user (used for traps with attached guns)
INT8 FireBulletGivenTargetTrapOnly( SOLDIERTYPE* pThrower, OBJECTTYPE* pObj, INT32 gridno, FLOAT dStartZ, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, INT16 sHitBy);

#define CALC_FROM_ALL_DIRS -1
#define CALC_FROM_WANTED_DIR -2
#define NO_DISTANCE_LIMIT -3

INT32 SoldierToSoldierLineOfSightTest( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier, INT8 bAware, int iTileSightLimit = CALC_FROM_ALL_DIRS, UINT8 ubAimLocation = LOS_POS, bool adjustForSight = true, bool cthCalc = false );
INT32 SoldierTo3DLocationLineOfSightTest( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel, INT8 bAware, int ubSightLimit = CALC_FROM_ALL_DIRS, bool adjustForSight = true );
INT32 SoldierToVirtualSoldierLineOfSightTest( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bStance, INT8 bAware, int iTileSightLimit = CALC_FROM_ALL_DIRS );
UINT8 SoldierToSoldierChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier );
UINT8 SoldierToSoldierBodyPartChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier, UINT8 ubAimLocation );
UINT8 AISoldierToSoldierChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier );
UINT8 AISoldierToLocationChanceToGetThrough( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel );
UINT8 SoldierToLocationChanceToGetThrough( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel, UINT8 ubTargetID );
INT32 SoldierToLocationWindowTest( SOLDIERTYPE * pStartSoldier, INT32 sEndGridNo );
INT32 LocationToLocationLineOfSightTest( INT32 sStartGridNo, INT8 bStartLevel, INT32 sEndGridNo, INT8 bEndLevel, INT8 bAware, int iTileSightLimit = CALC_FROM_ALL_DIRS );

BOOLEAN CalculateSoldierZPos( SOLDIERTYPE * pSoldier, UINT8 ubPosType, FLOAT * pdZPos );

#define HEIGHT_UNITS 256
#define HEIGHT_UNITS_PER_INDEX (HEIGHT_UNITS / PROFILE_Z_SIZE)
#define MAX_STRUCTURE_HEIGHT 50
// 5.12 == HEIGHT_UNITS / MAX_STRUCTURE_HEIGHT
#define CONVERT_PIXELS_TO_HEIGHTUNITS( n ) ((n) * HEIGHT_UNITS / MAX_STRUCTURE_HEIGHT)
#define CONVERT_PIXELS_TO_INDEX( n ) ((n) * HEIGHT_UNITS / MAX_STRUCTURE_HEIGHT / HEIGHT_UNITS_PER_INDEX)
#define CONVERT_HEIGHTUNITS_TO_INDEX( n ) ((n) / HEIGHT_UNITS_PER_INDEX)
#define CONVERT_HEIGHTUNITS_TO_DISTANCE( n ) ((n) / (HEIGHT_UNITS / CELL_X_SIZE) )
#define CONVERT_HEIGHTUNITS_TO_PIXELS( n ) ((n) * MAX_STRUCTURE_HEIGHT / HEIGHT_UNITS )
#define CONVERT_WITHINTILE_TO_INDEX( n ) ((n) >> 1)
#define CONVERT_INDEX_TO_WITHINTILE( n ) ((n) << 1)
#define CONVERT_INDEX_TO_PIXELS( n ) ((n) * MAX_STRUCTURE_HEIGHT * HEIGHT_UNITS_PER_INDEX / HEIGHT_UNITS)


#define TREE_SIGHT_REDUCTION 6
#define NORMAL_TREES 10

// 191 is 6' (structures of height 3)
// 127 is 4' (structures of height 2)
//	63 is 2' (structures of height 1)


#define STANDING_HEIGHT 191.0f
#define STANDING_LOS_POS 175.0f
#define STANDING_FIRING_POS 175.0f
#define STANDING_HEAD_TARGET_POS 175.0f
#define STANDING_HEAD_BOTTOM_POS 159.0f
#define STANDING_TORSO_TARGET_POS 127.0f
#define STANDING_TORSO_BOTTOM_POS 95.0f
#define STANDING_LEGS_TARGET_POS 47.0f
#define STANDING_TARGET_POS STANDING_HEAD_TARGET_POS

#define CROUCHED_HEIGHT 130.0f
#define CROUCHED_LOS_POS 111.0f
#define CROUCHED_FIRING_POS 111.0f

#define CROUCHED_HEAD_TARGET_POS 111.0f
#define CROUCHED_HEAD_BOTTOM_POS 95.0f
#define CROUCHED_TORSO_TARGET_POS 71.0f
#define CROUCHED_TORSO_BOTTOM_POS 47.0f
#define CROUCHED_LEGS_TARGET_POS 31.0f
#define CROUCHED_TARGET_POS CROUCHED_HEAD_TARGET_POS

#define PRONE_HEIGHT 63.0f
#define PRONE_LOS_POS 31.0f
#define PRONE_FIRING_POS 31.0f
#define PRONE_TORSO_TARGET_POS 31.0f
#define PRONE_HEAD_TARGET_POS 31.0f
#define PRONE_LEGS_TARGET_POS 31.0f
#define PRONE_TARGET_POS PRONE_HEAD_TARGET_POS

#define WALL_HEIGHT_UNITS HEIGHT_UNITS
#define WINDOW_BOTTOM_HEIGHT_UNITS 87
#define WINDOW_TOP_HEIGHT_UNITS 220

#define CLOSE_TO_FIRER 25
#define VERY_CLOSE_TO_FIRER 21

#ifdef LOS_DEBUG
typedef struct LOSResults
{
	BOOLEAN			fLOSTestPerformed;
	BOOLEAN			fLOSClear;
	BOOLEAN			fOutOfRange;
	INT32				iDistance;
	INT32				iMaxDistance;
	UINT8				ubTreeSpotsHit;
	INT32				iStartX;
	INT32				iStartY;
	INT32				iStartZ;
	INT32				iEndX;
	INT32				iEndY;
	INT32				iEndZ;
	INT32				iStoppedX;
	INT32				iStoppedY;
	INT32				iStoppedZ;
	INT32				iCurrCubesZ;
	UINT8				ubChanceToGetThrough;
} LOSResults;

extern LOSResults gLOSTestResults;

#endif

void MoveBullet( INT32 iBullet );
//BOOLEAN FireBullet2( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, INT16 sHitBy );

INT8 GetTerrainTypeForGrid( const INT32& uGridNo, const INT16& bLevel );

// check the flat distance adjustment based on your stance
// use this to test your view on your surroundings based on your stance
INT8 GetSightAdjustmentThroughStance( const UINT8& ubStance );

// quick way to get all. should be used for all sight line tests with target soldier known
INT16 GetSightAdjustment( SOLDIERTYPE* pSoldier, INT32 sGridNo = -1, INT16 bLevel = -1, INT8 bStance = -1 );

//zilpin: pellet spread patterns externalized in XML
#define SPREADPATTERN_NAME_SIZE 32
enum SpreadPatternMethod_enum
{
	SPREADPATTERNMETHOD_RECT = 0,
	SPREADPATTERNMETHOD_DIAMOND,
	SPREADPATTERNMETHOD_ELLIPSE,
	SPREADPATTERNMETHOD_COUNT,
};
extern char *gSpreadPatternMethodNames[];
extern int giSpreadPatternMethod_Default;
typedef struct SpreadPattern_struct
{
	//Unique name.
	char Name[SPREADPATTERN_NAME_SIZE];
	//Max spread for randomized cone of fire.
	DOUBLE  xspread,yspread;
	//Method (i.e. algorithm) used to distribute randomized cone.
	int  method;
	//List of statically defined spread angles.
	//These get used first.
	INT32	iCount;					
	DOUBLE  *x;
	DOUBLE  *y;
} t_SpreadPattern;
extern t_SpreadPattern  *gpSpreadPattern;
extern INT32 giSpreadPatternCount;
extern INT32 GetSpreadPattern( OBJECTTYPE * pObj );

/////////////////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 4: The following functions are all part of the NCTH project,
// which completely redesigns the way we calculate and handle CTH for the purposes of firing weapons.
void AdjustTargetCenterPoint( SOLDIERTYPE *pShooter, INT32 iTargetGridNo, FLOAT *dEndX, FLOAT *dEndY, FLOAT *dEndZ, OBJECTTYPE *pWeapon, UINT32 uiMuzzleSway, INT16 *sApertureRatio );
FLOAT CalcMagFactor( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT d2DDistance, INT32 iTargetGridNo, UINT8 ubAimTime );
FLOAT CalcProjectionFactor( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT d2DDistance, UINT8 ubAimTime );
FLOAT CalcBasicAperture();
void CalcTargetMovementOffset( SOLDIERTYPE *pShooter, SOLDIERTYPE *pTarget, OBJECTTYPE *pWeapon, FLOAT *dMuzzleOffsetX, DOUBLE ddShootingAngle, INT32 iAperture );
void CalcRangeCompensationOffset( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetY, INT32 iRangeToTarget, OBJECTTYPE *pWeapon );
void CalcMuzzleSway( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, FLOAT iAperture );
FLOAT CalcBulletDeviation( SOLDIERTYPE *pShooter, FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, OBJECTTYPE *pWeapon, UINT32 uiRange );
void LimitImpactPointByFacing( SOLDIERTYPE *pShooter, SOLDIERTYPE *pTarget, FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, FLOAT *dEndX, FLOAT *dEndY );
void LimitImpactPointToMaxAperture( FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, FLOAT dDistanceAperture );
// HEADROCK HAM 5: Removed and replaced by continuously-variable CF
// UINT32 CalcCounterForceFrequency(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon);
FLOAT CalcCounterForceMax(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, UINT8 uiStance = 0);
UINT32 CalcCounterForceAccuracy(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, UINT32 uiRange, BOOLEAN fTracer, bool fAnticipate = false);
void CalcPreRecoilOffset( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, UINT32 uiRange );
void CalcRecoilOffset( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, OBJECTTYPE *pWeapon, UINT32 uiRange );
// HEADROCK HAM 5: New function, completely replaces the Counter Force Frequency check.
FLOAT CalcCounterForceChange( SOLDIERTYPE * pShooter, UINT32 uiCounterForceAccuracy, FLOAT dCounterForceMax, FLOAT dMuzzleOffset, INT8 bRecoil, FLOAT dPrevCounterForce, UINT32 uiIntendedBullets );

// HEADROCK HAM 4: Required for shooting mechanic
extern INT8 EffectiveMarksmanship( SOLDIERTYPE * pSoldier ); 
extern INT16 EffectiveWisdom( SOLDIERTYPE * pSoldier ); 
extern INT8 EffectiveExpLevel( SOLDIERTYPE * pSoldier ); 
extern INT16 EffectiveDexterity( SOLDIERTYPE * pSoldier, BOOLEAN fTrainer ); 
extern INT16 EffectiveStrength( SOLDIERTYPE * pSoldier, BOOLEAN fTrainer );
extern INT16 EffectiveAgility( SOLDIERTYPE * pSoldier, BOOLEAN fTrainer ); 

#endif