#include "connect.h"
#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include "builddefines.h"
#include <stdio.h>

#include <memory.h>
#include <math.h>
#include "wcheck.h"
#include "Isometric Utils.h"
#include "debug.h"
#include "los.h"
#include "animation control.h"
#include "Random.h"
//#include "soldier control.h"
#include "Event Pump.h"
#include "overhead.h"
#include "weapons.h"

#include "opplist.h"
#include "bullets.h"

#include "lighting.h"
#include "phys math.h"
#include "items.h"
#include "Soldier Profile.h"
#include "worldman.h"
#include "rotting corpses.h"
#include "GameSettings.h"
#include "keys.h"
#include "message.h"
#include "Structure Wrap.h"
#include "campaign.h"
#include "environment.h"
#include "Pathai.h"
#include "Soldier macros.h"
#include "strategicmap.h"
#include "Interface.h"
#include "points.h"
#include "Smell.h"
#include "Text.h"
#include "Quests.h"
#include "items.h"
#include "Item Types.h"
#endif
#include "fresh_header.h"
#include "WorldDat.h"
// HEADROCK HAM 3.6: This must be included, for testing whether Bloodcats and Enemies can see one another.
#include "Campaign Types.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define		STEPS_FOR_BULLET_MOVE_TRAILS					10
#define		STEPS_FOR_BULLET_MOVE_SMALL_TRAILS		5
#define		STEPS_FOR_BULLET_MOVE_FIRE_TRAILS			5

#define ALWAYS_CONSIDER_HIT (STRUCTURE_WALLSTUFF | STRUCTURE_CAVEWALL | STRUCTURE_FENCE)


extern UINT32 guiSoldierFlags;
extern INT16 DirIncrementer[8];

extern BOOLEAN fTracer;


static FIXEDPT gqStandardWallHeight = INT32_TO_FIXEDPT( WALL_HEIGHT_UNITS );
static FIXEDPT gqStandardWindowBottomHeight = INT32_TO_FIXEDPT( WINDOW_BOTTOM_HEIGHT_UNITS );
static FIXEDPT gqStandardWindowTopHeight = INT32_TO_FIXEDPT( WINDOW_TOP_HEIGHT_UNITS );

#define FIXEDPT_MULTIPLY( a, b ) ( (a / 256) * (b / 256) )



//ADB When a merc calcs CTGT for a thrown item he uses a GLOCK temp item
//but we don't want to recreate it every single time CTGT is called, so init the GLOCK in InitializeJA2()
OBJECTTYPE GLOCK_17_ForUseWithLOS;



UINT32 FPMult32(UINT32 uiA, UINT32 uiB)
{
	UINT32 uiResult;

	__asm {
		// Load the 32-bit registers with the two values
		mov		eax, uiA
			mov		ebx, uiB

			// Multiply them
			// Top 32 bits (whole portion) goes into edx
			// Bottom 32 bits (fractional portion) goes into eax
			imul	ebx

			// Shift the fractional portion back to (lower) 16 bits
			shr		eax, 16
			// Shift the whole portion to 16 bits, in the upper word
			shl		edx, 16

			// At this point, we have edx xxxx0000 and eax 0000xxxx
			// Combine the two words into a dword
			or		eax, edx

			// Put the result into a returnable variable
			mov		uiResult, eax
	}

	return(uiResult);
}


//zilpin: pellet spread patterns externalized in XML
t_SpreadPattern  *gpSpreadPattern=NULL;
INT32 giSpreadPatternCount=0;
char * gSpreadPatternMethodNames[] = {"RECTANGLE","DIAMOND","ELLIPSE",NULL};
int giSpreadPatternMethod_Default = SPREADPATTERNMETHOD_ELLIPSE;
//zilpin: ddShotgunSpread is only used as a backup if the XML file does not exist.
static DOUBLE ddShotgunSpread[3][BUCKSHOT_SHOTS][2] =
{
	{
		// spread of about 2 degrees in all directions
		// Horiz,	Vert
		{	0.0,		0.0 },
		{ -0.012,	0.0 },
		{ +0.012,	0.0 },
		{	0.0,		-0.012},
		{	0.0,		+0.012},
		{	-0.008,	-0.008},
		{ -0.008,	+0.008},
		{ +0.008,	-0.008},
		{ +0.008,	+0.008}
	},
	{
		// duckbill flattens the spread and makes it wider horizontally (5 degrees)
		// Horiz,	Vert
		{	0.0,		0.0 },
		{	-0.008,	0.0 },
		{	+0.008,	0.0 },
		{	-0.016,	0.0 },
		{	+0.016,	0.0 },
		{	-0.024,	0.0 },
		{	+0.024,	0.0 },
		{	-0.032,	0.0 },
		{	+0.032,	0.0 },
	},
	{
		// flamethrower more spread out
		// Horiz,	Vert
		{	0.0,		0.0 },
		{ -0.120,	0.0 },
		{ +0.120,	0.0 },
		{	0.0,		-0.120},
		{	0.0,		+0.120},
		{	-0.080,	-0.080},
		{ -0.080,	+0.080},
		{ +0.080,	-0.080},
		{ +0.080,	+0.080}
		},

};
//zilpin: pellet spread patterns externalized in XML
//Decide what spread pattern to use for an object in inventory.
//Attachment trumps Weapon trumps Ammo trumps AmmoType
//If multiple attachments, then the first one encountered is used.
//This will work for non-weapons, but ammo and ammo type are only checked for guns.
INT32 GetSpreadPattern( OBJECTTYPE * pObj )
{
	int i=0,n=0;
	if( !pObj || !pObj->usItem || !pObj->exists() ) return 0;

	//If there are attachments, check them.  Stop on the first one with something defined.
	//Dear God, I hate C++ iterators.  What a fugly mess. //WarmSteel - I made it even messier ;3
	for (attachmentList::iterator iter = pObj[0][0]->attachments.begin(); iter != pObj[0][0]->attachments.end(); ++iter){
		if( n=Item[ iter->usItem ].spreadPattern && iter->exists()){
				//An attachment has it, and it trumps everything, so return it's value.
			return n;}
	}

	//The object's own spread pattern, if it has one.
	if( n=Item[pObj->usItem].spreadPattern )
		return n;

	//If a gun, and don't have a pattern yet, check it's ammo.
	if( IC_GUN & Item[pObj->usItem].usItemClass ){
		//Check for the ammo first.
		if( n=Item[ pObj[0][0]->data.gun.usGunAmmoItem ].spreadPattern )
			return n;
		//If no pattern for the ammo, try the ammo type.
		if( n=AmmoTypes[pObj[0][0]->data.gun.ubGunAmmoType].spreadPattern )
			return n;
	}


	/*
	This is the old code to iterate through all the attachments.
	No longer works, since the NewInv project object-orientified everything.
	*
	//If there are attachments, check them.  Stop on the first one with something defined.
	for (i=0; i<MAX_ATTACHMENTS; i++)
		if (pObj->usAttachItem[i])
			if( n=Item[ pObj->usAttachItem[i] ].spreadPattern )
				//The attachment has it, and it trumps everything, so return it's value.
				return n;

	//The object's own spread pattern, if it has one.
	if( n=Item[pObj->usItem].spreadPattern )
		return n;

	//If a gun, and don't have a pattern yet, check it's ammo.
	if( IC_GUN & Item[pObj->usItem].usItemClass ){
		//Check for the ammo first.
		if( n=Item[ pObj->ItemData.Gun.usGunAmmoItem ].spreadPattern )
			return n;
		//If no pattern for the ammo, try the ammo type.
		if( n=AmmoTypes[pObj->ItemData.Gun.ubGunAmmoType].spreadPattern )
			return n;
	}
	*/

	//No pattern for anything found, so use the default.
	return 0;
}



static UINT8 gubTreeSightReduction[ANIM_STAND + 1] =
{
	0,
	8, // prone
	0,
	7, // crouched
	0,
	0,
	6 // standing
};

#define MAX_DIST_FOR_LESS_THAN_MAX_CHANCE_TO_HIT_STRUCTURE 25

#define MAX_CHANCE_OF_HITTING_STRUCTURE 90

static UINT32 guiStructureHitChance[ MAX_DIST_FOR_LESS_THAN_MAX_CHANCE_TO_HIT_STRUCTURE + 1] =
{
	0,	// 0 tiles
	0,
	0,
	2,
	4,
	7,	// 5 tiles
	10,
	14,
	18,
	23,
	28,	// 10 tiles
	34,
	40,
	47,
	54,
	60,	// 15 tiles
	66,
	71,
	74,
	76,
	78,// 20 tiles
	80,
	82,
	84,
	86,
	88,// 25 tiles
};


#define PERCENT_BULLET_SLOWED_BY_RANGE 25

#define MIN_DIST_FOR_HIT_FRIENDS 30
#define MIN_DIST_FOR_HIT_FRIENDS_UNAIMED 15
#define MIN_CHANCE_TO_ACCIDENTALLY_HIT_SOMEONE 3

#define RADIANS_IN_CIRCLE 6.283
#define DEGREES_22_5 (RADIANS_IN_CIRCLE * 22.5 / 360)
#define DEGREES_45 (RADIANS_IN_CIRCLE * 45 / 360)
// note: these values are in RADIANS!!
// equal to 15 degrees
#define MAX_AIMING_SCREWUP (RADIANS_IN_CIRCLE * 15 / 360)
// min aiming screwup is X degrees, gets divided by distance in tiles
#define MIN_AIMING_SCREWUP (RADIANS_IN_CIRCLE * 22 / 360)
//#define MAX_AIMING_SCREWUP 0.2618
// equal to 10 degrees
//#define MAX_AIMING_SCREWUP_VERTIC 0.1745

#define SMELL_REDUCTION_FOR_NEARBY_OBSTACLE 20 //80

#define STANDING_CUBES 3

// MoveBullet and ChanceToGetThrough use this array to maintain which
// of which structures in a tile might be hit by a bullet.

#define MAX_LOCAL_STRUCTURES 100

STRUCTURE * gpLocalStructure[MAX_LOCAL_STRUCTURES];
UINT32			guiLocalStructureCTH[MAX_LOCAL_STRUCTURES];
UINT8				gubLocalStructureNumTimesHit[MAX_LOCAL_STRUCTURES];

BOOLEAN CalculateLOSNormal( 	STRUCTURE *pStructure, INT8 bLOSX, INT8 bLOSY, INT8 bLOSZ, FLOAT dDeltaX, FLOAT dDeltaY, FLOAT dDeltaZ, FLOAT *pdNormalX, FLOAT *pdNormalY, FLOAT *pdNormalZ );


extern UINT8 gubMaterialArmour[];

#ifdef LOS_DEBUG
LOSResults gLOSTestResults = {0};
#endif

FIXEDPT FloatToFixed( FLOAT dN )
{
	FIXEDPT		qN;
	// verify that dN is within the range storable by FIXEDPT?
	// first get the whole part

	qN = (dN * FIXEDPT_FRACTIONAL_RESOLUTION);

	//qN = INT32_TO_FIXEDPT( (INT32)dN );
	// now add the fractional part
	//qN += (INT32)(((dN - (INT32) dN)) * FIXEDPT_FRACTIONAL_RESOLUTION);

	return( qN );
}


FLOAT FixedToFloat( FIXEDPT qN )
{
	return( ((FLOAT) qN)	/ FIXEDPT_FRACTIONAL_RESOLUTION );
}

//
// fixed-point arithmetic stuff ends here
//

FLOAT Distance3D( FLOAT dDeltaX, FLOAT dDeltaY, FLOAT dDeltaZ )
{
	return( (FLOAT) sqrt( (DOUBLE) (dDeltaX * dDeltaX + dDeltaY * dDeltaY + dDeltaZ * dDeltaZ) ));
}

FLOAT Distance2D( FLOAT dDeltaX, FLOAT dDeltaY )
{
	return( (FLOAT) sqrt( (DOUBLE) (dDeltaX * dDeltaX + dDeltaY * dDeltaY )));
}

//#define DEBUGLOS

#if defined( JA2BETAVERSION ) && defined( DEBUGLOS )
void DebugLOS( STR szOutput )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,szOutput);
	FILE *		DebugFile;

	if ((DebugFile = fopen( "losdebug.txt", "a+t" )) != NULL)
	{
		fputs( szOutput, DebugFile );
		fputs( "\n", DebugFile );
		fclose( DebugFile );
	}

}
#else
#define DebugLOS( a )
#endif

enum
{
	LOC_OTHER,
	LOC_0_4,
	LOC_3_4,
	LOC_4_0,
	LOC_4_3,
	LOC_4_4
}
LocationCode;

/**
* Calculates the height and gives it back as one of the ANIM_* defines
*
* @param pSoldier
* @return ANIM_STAND or ANIM_CROUCH or ANIM_PRONE
*/
inline UINT8 GetCurrentHeightOfSoldier( SOLDIERTYPE* pSoldier )
{
	return gAnimControl[ pSoldier->usAnimState ].ubHeight;
}

// why not functions? because these are typeless
#define MINMAX(iMin, iMax, iVal) min(iMax, max(iMin, iVal))
#define MINMAX100(iVal) MINMAX(0, 100, iVal)
#define MINMAX100N(iVal) MINMAX(-100, 100, iVal)

/*
* Calculates the total camouflage for the corresponding type.
*
* @comment we don't need the snow camo as we don't have a terrain type -> camouflage association for snow
*          (meaning we would need externalisation for specifing camo effectivness based on terrain textures and not terrain types)
*          (unfortunatly that's not only programming but alot of content-heavy work, which doesn't seem probable to happen)
* @precondition camo on soldiertypes are correctly set
*
* @param pSoldier 
* @return a value generally between 0 and 100 (but who knows if someone wants to use negative values, the basic types allow for that)
*/
inline INT8 GetJungleCamouflage( SOLDIERTYPE* pSoldier )
{
	// SANDRO - the Camouflage trait has been solved differently
	//if (HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED )) {
	//	return 100;
	//}
	
	return MINMAX100N(pSoldier->bCamo + pSoldier->wornCamo);
}
inline INT8 GetDesertCamouflage( SOLDIERTYPE* pSoldier )
{
	// SANDRO - the Camouflage trait has been solved differently
	//if (HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_DESERT )) {
	//	return 100;
	//}
	
	return MINMAX100N(pSoldier->desertCamo + pSoldier->wornDesertCamo);
}
inline INT8 GetUrbanCamouflage( SOLDIERTYPE* pSoldier )
{
	// SANDRO - the Camouflage trait has been solved differently
	//if (HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_URBAN )) {
	//	return 100;
	//}
	
	return MINMAX100N(pSoldier->urbanCamo + pSoldier->wornUrbanCamo);
}

// should be in lightning, 0-100 definition of brightness
inline UINT8 GetBrightness(const UINT8& ubLightLevel)
{
	return 100 * (ubLightLevel-SHADE_MAX)/(SHADE_MIN-SHADE_MAX);
}

/**
* Calculates the total stealth value of a player.
*
* @param pSoldier
* @return ANIM_STAND or ANIM_CROUCH or ANIM_PRONE
*/
inline INT8 GetStealth( SOLDIERTYPE* pSoldier )
{
	INT16 stealth = GetWornStealth( pSoldier );
	// SANDRO - this only counts with old traits
	if (HAS_SKILL_TRAIT( pSoldier, STEALTHY_OT ) && !gGameOptions.fNewTraitSystem)
	{
		stealth += gGameExternalOptions.ubStealthTraitCoverValue * NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
	}

	return MINMAX100N(stealth);
}

/**
* Gives back a valid terrainId for a specified GridNo and bLevel
* 
* @param pSoldier
*/
INT8 GetTerrainTypeForGrid( const INT32& sGridNo, const INT16& bLevel )
{
	LEVELNODE* pNode;
	if (bLevel == I_ROOF_LEVEL)
	{
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
	}
	else
	{
		pNode = gpWorldLevelData[ sGridNo ].pObjectHead;
	}

	// Check if we have anything in object layer which has a terrain modifier
	if ( pNode != NULL )
	{
		if ( gTileDatabase[ pNode->usIndex ].ubTerrainID != NO_TERRAIN )
		{
			return gTileDatabase[ pNode->usIndex ].ubTerrainID ;
		}
	}

	// map element layer
	if (gpWorldLevelData[sGridNo].ubTerrainID != NO_TERRAIN)
	{
		return gpWorldLevelData[sGridNo].ubTerrainID;
	}

	// Now try terrain special
	pNode = gpWorldLevelData[ sGridNo ].pLandHead;

	return gTileDatabase[ pNode->usIndex ].ubTerrainID;
}

// the following functions should return percentage values which can be easily added/substracted from
// the sight reduction variable inside the Soldier-sight-test.
// This means we specify here how much each test counts towards the total.
// e.g. camouflage of 100 at stance prone on perfect terrain will give back "-50" as value, indicating a 50% sight reduction

/**
* Calculates a percentage value to be added (or substracted) from sight. it's based on the camouflage and stance of the soldier, as well as the type of terrain.
* - usually it should return something smaller than 0 but we might give a bonus in a later version if the target wears a red-glowing hat
*
* @param pSoldier the target needed to get the correct camouflage info
* @param ubTerrainType terrain type defined by TerrainTypeDefines
* @return a negative value will indicate a reduction of sight, a positive one an addition to sight
*/
INT8 GetSightAdjustmentCamouflageOnTerrain( SOLDIERTYPE* pSoldier, const UINT8& ubStance, const UINT8& ubTerrainType )
{
	if (gGameExternalOptions.ubCamouflageEffectiveness == 0) {
		return 0;
	}

	INT8 scaler = -(ANIM_STAND+1 - ubStance); // stand = 7-6 => 10%, crouch = 7-3 => 66%, prone = 7-1 => 100%;

	scaler = gGameExternalOptions.ubCamouflageEffectiveness * scaler / 6;

	switch(ubTerrainType) {
		case LOW_GRASS:
		case HIGH_GRASS:
			return GetJungleCamouflage(pSoldier) * scaler/100;
		case FLAT_FLOOR:
		case PAVED_ROAD:
			return GetUrbanCamouflage(pSoldier) * scaler/100;
		case DIRT_ROAD:
		case TRAIN_TRACKS:
			return GetDesertCamouflage(pSoldier) * scaler/100;
		case FLAT_GROUND:
			// here it would be best if we could have a terrain texture -> camo association instead of terrain type -> camo
			//  but we need to allow modders to specify that information in an xml file
			// this is because flat ground is used by both (it's irritating to see wood camo work perfectly on brown ground, at least ingame)
			return max( GetDesertCamouflage(pSoldier), GetJungleCamouflage(pSoldier) ) * scaler/100;
		default:
			return 0;
	}
}

/**
* Calculates a percentage value to be added (or substracted) from sight. it's based on the movement of a soldier and the brightness level.
* - this will be used to calculate the penalty (sight addition) which occurs on the soldier who is been spot-tested
* - it also will be used to reduce the ability to find others if the spotter moves
*
* @param pSoldier the target
* @return a negative value will indicate a reduction of sight, a positive one an addition to sight
*/
INT8 GetSightAdjustmentThroughMovement( SOLDIERTYPE* pSoldier, const INT8& bTilesMoved, const UINT8& ubLightLevel  )
{
	if (gGameExternalOptions.ubMovementEffectiveness == 0) {
		return 0;
	}

	INT8 stealth = GetStealth(pSoldier);

	INT8 bMovementAdjustment = bTilesMoved * ( 100 - stealth ) / 100;

	// SANDRO - added reduction of penalty for moving for Stealthy trait with new traits
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ))
		bMovementAdjustment = max(0, (bMovementAdjustment * (100 - gSkillTraitValues.ubSTStealthPenaltyForMovingReduction) / 100) );

	UINT8 ubBrightness = GetBrightness( ubLightLevel );

	return MINMAX100N( bMovementAdjustment * ubBrightness / 100 * gGameExternalOptions.ubMovementEffectiveness / 100 );
}

INT8 GetSightAdjustmentThroughStance( const UINT8& ubStance )
{
	if (gGameExternalOptions.ubStanceEffectiveness == 0) {
		return 0;
	}

	INT8 bStanceAdjustment = -(ANIM_STAND - ubStance) * 20; // stand = 6-6 => 0%, crouch = 6-3 => 60%, prone = 6-1 => 100%;

	return MINMAX100N( gGameExternalOptions.ubStanceEffectiveness * bStanceAdjustment / 100 );
}

/**
* TODO: Figure out how to find out your equipment cost. Big equipment will make you more visible! we could go after the load itself (kg)
* TODO: but we still want a backpack to count more than your vest
* 
* Calculates a percentage value to be added (or substracted) from sight. it's based on the amount of LBE.
*
* @param pSoldier the target
* @return a negative value will indicate a reduction of sight, a positive one an addition to sight
*/
INT8 GetSightAdjustmentBasedOnLBE( SOLDIERTYPE* pSoldier )
{
	if (gGameExternalOptions.ubLBEEffectiveness == 0) {
		return 0;
	}

	UINT8 ubScale = 0;

	if(UsingNewInventorySystem() == true)
	{
		if (pSoldier->inv[HANDPOS].exists())
		{
			ubScale += min( 10, CalculateItemSize( &pSoldier->inv[HANDPOS] ) );
		}

		if (pSoldier->inv[SECONDHANDPOS].exists())
		{
			ubScale += max( 0, min( 10, CalculateItemSize( &pSoldier->inv[SECONDHANDPOS] ) ) - 5 );
		}
		
		if (pSoldier->inv[CPACKPOCKPOS].exists())
		{
			ubScale += 5;
		}

		if (pSoldier->inv[BPACKPOCKPOS].exists())
		{
			ubScale += 15;
		}

		if (pSoldier->inv[GUNSLINGPOCKPOS].exists())
		{
			ubScale += max( 0, min( 10, CalculateItemSize( &pSoldier->inv[GUNSLINGPOCKPOS] ) ) - 5 );
		}
	}

	return gGameExternalOptions.ubLBEEffectiveness * ubScale / 50;
}

/**
* Calculates a percentage value to be added (or substracted) from sight. it's based on the stealth value and the brightness.
* - usually it should return something smaller than 0 but we might give a bonus if the target uses a lamp for better viewing (LAM-Lamp) which the spotter can see
*
* @param pSoldier the target
* @param ubLightLevel light level given back by the LightTrueLevel function
* @return a negative value will indicate a reduction of sight, a positive one an addition to sight
*/
INT8 GetSightAdjustmentStealthAtLightLevel( SOLDIERTYPE* pSoldier, const UINT8& ubLightLevel )
{
	if (gGameExternalOptions.ubStealthEffectiveness == 0) {
		return 0;
	}

	// set scaler to scale with light level (dark = 100%)
	UINT8 ubScaler = GetBrightness( ubLightLevel );

	// last term corresponds to the maximum of ubScaler before
	ubScaler *= gGameExternalOptions.ubStealthEffectiveness / 100;

	INT8 ibStealthInPercent = GetStealth( pSoldier );

	return MINMAX100N( - ibStealthInPercent * ubScaler / 100);
}

/**
* Calculates a percentage value used for trees that hide you. Will be used inside line of sight function.
* - It's based on the tree "height", fill and density.
* - As well as range and you point of intersection
* - can go up to 100% hiding
*
* @param iRange one tile = 10 range
* @param pTreeStructure a structure containing the tree you want to check
* @param ubHeightLevel between 1 and 4 of the current merc
* @return >=0 the percentage a tree will give a bonus to hiding / view range degradation
*/
INT8 GetSightAdjustmentBehindStructure( const INT16& iRange, STRUCTURE* pStructure, const UINT8& ubHeightLevel )
{
	if (gGameExternalOptions.ubTreeCoverEffectiveness == 0) {
		return 0;
	}

	// Assertions here because we might need this function in other line of sight tests
	Assert( ubHeightLevel >= 1 );
	Assert( ubHeightLevel <= 4 );
	Assert( pStructure != NULL );
	Assert( iRange >= 0 ); // ==0 for you being inside a passable tree

	// pStructure->fFlags & STRUCTURE_TREE
	UINT8 ubLevels[4];
	if (StructureDensity( pStructure, &ubLevels[0], &ubLevels[1], &ubLevels[2], &ubLevels[3] ) != TRUE)
	{
		return 0; // this structure has no density therefore no additional sight adjustment possible
	}
	
	//const UINT8& ubDensityInPercent = pStructure->pDBStructureRef->pDBStructure->ubDensity;
	UINT16 ubFillInPercent = ubLevels[ubHeightLevel-1];

	Assert( ubFillInPercent >= 0 );
	Assert( ubFillInPercent <= 100 );

	// these scalers are for backwards compatibility to mods
	UINT8 ubCloseRangeScaler = iRange > CLOSE_TO_FIRER; // zero or 1
	INT16 ubLongRangeScaler = iRange > 100 ? iRange / 100 : 1;

	//* ubDensityInPercent / 100 we assume densitiy is used for bullets
	// we make the fill less effective
	INT8 ubSolidityInPercent = ubFillInPercent * gGameExternalOptions.ubTreeCoverEffectiveness / 100;

	return MINMAX100N(- ubSolidityInPercent * ubCloseRangeScaler * ubLongRangeScaler); 
}

/*
* Easy way to get all sight adjustments into one that affect a soldier
*/
INT16 GetSightAdjustment( SOLDIERTYPE* pSoldier, INT32 sGridNo, INT16 bLevel, INT8 bStance )
{
	if (sGridNo == -1) {
		sGridNo = pSoldier->sGridNo;
	}

	if (bLevel == -1) {
		bLevel = gpWorldLevelData[pSoldier->sGridNo].sHeight;
	}

	if (bStance == -1) {
		bStance = GetCurrentHeightOfSoldier( pSoldier );
	}

	UINT8 ubTerrainType = GetTerrainTypeForGrid( sGridNo, bLevel );
	UINT8 ubLightLevel = LightTrueLevel( sGridNo, bLevel );

	INT16 iSightAdjustment = 0;

	// general stuff (independant of soldier)
	iSightAdjustment += GetSightAdjustmentThroughStance( bStance );

	// context sensitive (needs soldier)
	iSightAdjustment += GetSightAdjustmentBasedOnLBE( pSoldier );

	// context sensitive stuff with 2nd parameter (needs soldier for attributes but can be given a second parameter)
	iSightAdjustment += GetSightAdjustmentThroughMovement( pSoldier, pSoldier->bTilesMoved, ubLightLevel );
	iSightAdjustment += GetSightAdjustmentStealthAtLightLevel( pSoldier, ubLightLevel );
	iSightAdjustment += GetSightAdjustmentCamouflageOnTerrain( pSoldier, bStance, ubTerrainType );

	return MINMAX100N(iSightAdjustment);
}

BOOLEAN ResolveHitOnWall( STRUCTURE * pStructure, INT32 iGridNo, INT8 bLOSIndexX, INT8 bLOSIndexY, DOUBLE ddHorizAngle )
{
	BOOLEAN				fNorthSouth, fEastWest;
	BOOLEAN				fTopLeft, fTopRight;
	INT8					bLocation = LOC_OTHER;

	switch ( bLOSIndexX )
	{
	case 0:
		if ( bLOSIndexY == 4 )
		{
			bLocation = LOC_0_4;
		}
		break;
	case 3:
		if ( bLOSIndexY == 4 )
		{
			bLocation = LOC_3_4;
		}
		break;
	case 4:
		switch( bLOSIndexY )
		{
		case 0:
			bLocation = LOC_4_0;
			break;
		case 3:
			bLocation = LOC_4_3;
			break;
		case 4:
			bLocation = LOC_4_4;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	if ( bLocation == LOC_OTHER )
	{
		// these spots always block
		return( TRUE );
	}

	// use cartesian angles for god's sakes -CJC
	ddHorizAngle = -ddHorizAngle;

	fNorthSouth = ( ( ddHorizAngle < (0) && ddHorizAngle > ( -PI * 1 / 2) ) || ( ddHorizAngle > ( PI * 1 / 2 ) && ddHorizAngle < (PI) ) );
	fEastWest = ( ( ddHorizAngle > (0) && ddHorizAngle < (PI * 1 / 2) ) || ( ddHorizAngle < (-PI * 1 / 2) && ddHorizAngle > ( -PI ) ) );

	fTopLeft = ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT );
	fTopRight = ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT );

	if ( fNorthSouth )
	{
		// Check N-S at west corner:		4,4 4,3	0,4
		if ( bLocation == LOC_4_3 || bLocation == LOC_4_4 )
		{
			// if wall orientation is top-right, then check S of this location
			// if wall orientation is top-left, then check E of this location
			// if no wall of same orientation there, let bullet through
			if ( fTopRight )
			{
				if (!WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( SOUTH )) ) &&
						!WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo) ) &&
						!OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( (iGridNo + DirectionInc( SOUTH )) ) )
				{
					return( FALSE );
				}

			}

		}
		else if ( bLocation == LOC_0_4 )
		{
			if ( fTopLeft )
			{
				if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( WEST )) ) &&
						 !WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( SOUTHWEST ) ) ) &&
	 					 !OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (iGridNo + DirectionInc( WEST )) ) )
				{
					return( FALSE );
				}
			}

		}

		// Check N-S at east corner:		4,4 3,4	4,0
		if ( bLocation == LOC_4_4 || bLocation == LOC_3_4 )
		{
			if ( fTopLeft )
			{
				if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( EAST )) ) &&
						 !WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo) ) &&
	 					 !OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (iGridNo + DirectionInc( EAST )) ) )
				{
					return( FALSE );
				}
			}
		}
		else if ( bLocation == LOC_4_0 )
		{
			// if door is normal and OPEN and outside type then we let N-S pass
			if ( (pStructure->fFlags & STRUCTURE_DOOR) && (pStructure->fFlags & STRUCTURE_OPEN) )
			{
				if ( pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
				{
					return( FALSE );
				}
			}
			else if ( fTopRight )
			{
				if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( NORTHEAST )) ) &&
						 !WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( NORTH ) ) ) &&
	 					 !OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (iGridNo + DirectionInc( NORTHEAST )) ) )
				{
					return( FALSE );
				}
			}
		}
	}

	if ( fEastWest )
	{
		// Check E-W at north corner:	4,4	4,0		0,4
		if ( bLocation == LOC_4_4 )
		{
			if ( pStructure->ubWallOrientation == NO_ORIENTATION)
			{
				// very top north corner of building, and going (screenwise) west or east
				return( FALSE );
			}
		}
		else if ( bLocation == LOC_4_0 )
		{
			// maybe looking E-W at (screenwise) north corner of building
			// if wall orientation is top-right, then check N of this location
			// if no wall of same orientation there, let bullet through
			if ( fTopRight )
			{
				if (!WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( NORTH )) ) && 
						!WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( NORTH )) ) )
				{
					return( FALSE );
				}
			}
		}
		else if ( bLocation == LOC_0_4 )
		{
			// if normal door and OPEN and inside type then we let E-W pass
			if ( (pStructure->fFlags & STRUCTURE_DOOR) && (pStructure->fFlags & STRUCTURE_OPEN) )
			{
				if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == INSIDE_TOP_RIGHT )
				{
					return( FALSE );
				}
			}

			// if wall orientation is top-left, then check W of this location
			// if no wall of same orientation there, let bullet through
			if ( fTopLeft )
			{
				if (!WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( WEST )) ) && 
						!WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( WEST )) ) )
				{
					return( FALSE );
				}
			}

		}

		// Check E-W at south corner:	4,4 3,4 4,3
		if ( bLocation == LOC_3_4 || bLocation == LOC_4_4 || bLocation == LOC_4_3 )
		{
			if ( (bLocation == LOC_3_4 && fTopLeft) || (bLocation == LOC_4_3 && fTopRight) || (bLocation == LOC_4_4) )
			{
				if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (iGridNo + DirectionInc( EAST ) ) ) &&
						 !WallOrClosedDoorExistsOfTopRightOrientation( (iGridNo + DirectionInc( SOUTH ) ) ) &&						 
						 !OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (iGridNo + DirectionInc( EAST ) ) ) && 
						 !OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( (iGridNo + DirectionInc( SOUTH ) ) )
					)
				{
					return( FALSE );
				}
			}
		}

	}

	/*

	*/



	// currently handled:
	// E-W at north corner:	(4,4), (0,4), (4,0)
	// N-S at east corner: (4,4)
	// N-S at west corner: (4,4)

	// could add:
	// N-S at east corner: (3, 4), (4, 0)
	// N-S at west corner: (0, 4), (4, 3)
	// E-W at south corner: (4, 4), (3, 4), (4, 3) (completely new)

	/*

	// possibly shooting at corner in which case we should let it pass
	if ( bLOSIndexX == 0)
	{
	if ( bLOSIndexY == (PROFILE_Y_SIZE - 1))
	{
	// maybe looking E-W at (screenwise) north corner of building, or through open door
	if ( ( ddHorizAngle > (0) && ddHorizAngle < (PI * 1 / 2) ) || ( ddHorizAngle < (-PI * 1 / 2) && ddHorizAngle > ( -PI ) ) )
	{
	// if door is normal and OPEN and inside type then we let E-W pass
	if ( (pStructure->fFlags & STRUCTURE_DOOR) && (pStructure->fFlags & STRUCTURE_OPEN) )
	{
	if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == INSIDE_TOP_RIGHT )
	{
	fResolveHit = FALSE;
	}
	}

	// if wall orientation is top-left, then check W of this location
	// if no wall of same orientation there, let bullet through
	if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
	{
	if (!WallOrClosedDoorExistsOfTopLeftOrientation( (INT16) (iGridNo + DirectionInc( WEST )) ) &&
	!WallOrClosedDoorExistsOfTopRightOrientation( (INT16) (iGridNo + DirectionInc( WEST )) ) )
	{
	fResolveHit = FALSE;
	}
	}
	}
	else if ( ( ddHorizAngle < (0) && ddHorizAngle > ( -PI * 1 / 2) ) || ( ddHorizAngle > ( PI * 1 / 2 ) && ddHorizAngle < (PI) ) )
	{
	// maybe looking N-S at (screenwise) west corner of building

	// if wall orientation is top-left, then check W of this location
	// if no wall of same orientation there, let bullet through
	if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
	{
	if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (INT16) (iGridNo + DirectionInc( WEST )) ) &&
	!WallOrClosedDoorExistsOfTopRightOrientation( (INT16) (iGridNo + DirectionInc( SOUTHWEST ) ) ) &&
	!OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (INT16) (iGridNo + DirectionInc( WEST )) ) )
	{
	fResolveHit = FALSE;
	}
	}

	}

	}
	}
	else if (bLOSIndexX == (PROFILE_X_SIZE - 1))
	{
	if (bLOSIndexY == 0)
	{
	// maybe looking E-W at (screenwise) north corner of building
	if ( ( ddHorizAngle > (0) && ddHorizAngle < (PI * 1 / 2) ) || ( ddHorizAngle < (-PI * 1 / 2) && ddHorizAngle > ( -PI ) ) )
	{
	// if wall orientation is top-right, then check N of this location
	// if no wall of same orientation there, let bullet through
	if ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
	{
	if (!WallOrClosedDoorExistsOfTopRightOrientation( (INT16) (iGridNo + DirectionInc( NORTH )) ) &&
	!WallOrClosedDoorExistsOfTopLeftOrientation( (INT16) (iGridNo + DirectionInc( NORTH )) ) )
	{
	fResolveHit = FALSE;
	}
	}
	}
	else
	{
	// if door is normal and OPEN and outside type then we let N-S pass
	if ( (pStructure->fFlags & STRUCTURE_DOOR) && (pStructure->fFlags & STRUCTURE_OPEN) )
	{
	if ( pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
	{
	fResolveHit = FALSE;
	}
	}
	}
	}
	else if ( bLOSIndexY == (PROFILE_Y_SIZE - 1) || bLOSIndexY == (PROFILE_Y_SIZE - 2) )
	{
	// maybe (SCREENWISE) west or east corner of building and looking N
	if ( ( ddHorizAngle < (0) && ddHorizAngle > ( -PI * 1 / 2) ) || ( ddHorizAngle > ( PI * 1 / 2 ) && ddHorizAngle < (PI) ) )
	{
	// if wall orientation is top-right, then check S of this location
	// if wall orientation is top-left, then check E of this location
	// if no wall of same orientation there, let bullet through
	if ( pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT )
	{
	if ( !WallOrClosedDoorExistsOfTopLeftOrientation( (INT16) (iGridNo + DirectionInc( EAST )) ) &&
	!WallOrClosedDoorExistsOfTopRightOrientation( (INT16) (iGridNo) ) &&
	!OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( (INT16) (iGridNo + DirectionInc( EAST )) ) )
	{
	fResolveHit = FALSE;
	}
	}
	else if ( pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT )
	{
	if (!WallOrClosedDoorExistsOfTopRightOrientation( (INT16) (iGridNo + DirectionInc( SOUTH )) ) &&
	!WallOrClosedDoorExistsOfTopLeftOrientation( (INT16) (iGridNo) ) &&
	!OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( (INT16) (iGridNo + DirectionInc( SOUTH )) ) )
	{
	fResolveHit = FALSE;
	}

	}
	}
	// the following only at 4,4
	else if ( bLOSIndexY == (PROFILE_Y_SIZE - 1) )
	{
	if ( pStructure->ubWallOrientation == NO_ORIENTATION)
	{
	// very top north corner of building, and going (screenwise) west or east
	fResolveHit = FALSE;
	}
	}
	}
	}
	*/

	return( TRUE );
}



/*
*
* The line of sight code is now used to simulate smelling through the air (for monsters);
* It obeys the following rules:
* - ignores trees and vegetation
* - ignores people
* - should always start off with head height for both source and target, so that lying down makes no difference
* - stop at closed windows
* - stop for other obstacles
*
* Just for reference, normal sight obeys the following rules:
* - trees & vegetation reduce the maximum sighting distance
* - ignores people
* - starts at height relative to stance
* - ignores windows
* - stops at other obstacles
*
*/
INT32 LineOfSightTest( FLOAT dStartX, FLOAT dStartY, FLOAT dStartZ, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, int iTileSightLimit, INT8 bAware, BOOLEAN fSmell, INT32 * psWindowGridNo, bool adjustForSight = true, bool cthCalc = false )
{
	// Parameters...
	// the X,Y,Z triplets should be obvious
	// TileSightLimit is the max # of tiles of distance visible
	// TreeSightReduction is the reduction in 10ths of tiles in max visibility for each LOS cube (5th of a tile) of
	// vegetation hit
	// Aware is whether the looker is aware of the target
	// Smell is whether this is a sight or a smell test

	// Now returns not a boolean but the adjusted (by cover) distance to the target, or 0 for unseen

	FIXEDPT		qCurrX;
	FIXEDPT		qCurrY;
	FIXEDPT		qCurrZ;

	INT32		iGridNo;
	INT32		iCurrTileX;
	INT32		iCurrTileY;

	INT8		bLOSIndexX;
	INT8		bLOSIndexY;
	INT8		bOldLOSIndexX;
	INT8		bOldLOSIndexY;
	INT32		iOldCubesZ;

	INT32		iCurrCubesZ;

	FIXEDPT	qLandHeight;
	INT32		iCurrAboveLevelZ;
	INT32		iCurrCubesAboveLevelZ;
	INT32		iStartCubesAboveLevelZ;
	INT32		iEndCubesAboveLevelZ;
	INT32		iStartCubesZ;
	INT32		iEndCubesZ;

	INT16		sDesiredLevel;


	INT32		iOldTileX;
	INT32		iOldTileY;

	FLOAT		dDeltaX;
	FLOAT		dDeltaY;
	FLOAT		dDeltaZ;

	FIXEDPT		qIncrX;
	FIXEDPT		qIncrY;
	FIXEDPT		qIncrZ;

	FLOAT		dDistance;

	INT32		iDistance;
	INT32		iLoop;

	MAP_ELEMENT *		pMapElement;
	STRUCTURE *			pStructure;
	STRUCTURE *			pRoofStructure = NULL;

	BOOLEAN					fCheckForRoof;
	FIXEDPT					qLastZ;

	FIXEDPT					qDistToTravelX;
	FIXEDPT					qDistToTravelY;
	INT32						iStepsToTravelX;
	INT32						iStepsToTravelY;
	INT32						iStepsToTravel;
	BOOLEAN					fResolveHit;
	DOUBLE					ddHorizAngle;
	INT32						iStructureHeight;

	FIXEDPT					qWallHeight;
	BOOLEAN					fOpaque;
	INT8						bSmoke = 0;

	if ( gTacticalStatus.uiFlags & DISALLOW_SIGHT )
	{
		return( 0 );
	}

	//ADB see notes at bottom as to why there is this 255 here
	INT32		iSightLimit = iTileSightLimit * CELL_X_SIZE;
	if (iTileSightLimit >= 255) {
		iSightLimit = (iTileSightLimit - 255) * CELL_X_SIZE;
	}
	//for the CTGT calculation, clamp it to the average distance
	INT32		iAdjSightLimit = iSightLimit;
	if (iSightLimit == 0)
	{
		// blind!
		return( 0 );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LineOfSightTest: Thermal Optics = %d",fSmell));

	// verify start and end to make sure we'll always be inside the map

	// hack end location to the centre of the tile, because there was a problem
	// seeing a presumably off-centre merc...

	dStartX = (FLOAT) (((INT32)dStartX) / 10) * 10 + 5;
	dStartY = (FLOAT) (((INT32)dStartY) / 10) * 10 + 5;

	dEndX = (FLOAT) (((INT32)dEndX) / 10) * 10 + 5;
	dEndY = (FLOAT) (((INT32)dEndY) / 10) * 10 + 5;

	dDeltaX = dEndX - dStartX;
	dDeltaY = dEndY - dStartY;
	dDeltaZ = dEndZ - dStartZ;

	dDistance = Distance3D( dDeltaX, dDeltaY, CONVERT_HEIGHTUNITS_TO_DISTANCE( dDeltaZ ));
	iDistance = (INT32) dDistance;

	if ( iDistance == 0 )
	{
		return( FALSE );
	}

	if ( dDistance != (FLOAT) iDistance )
	{
		// add 1 step to account for fraction
		iDistance += 1;
	}

	if ( iDistance > iTileSightLimit * CELL_X_SIZE)
	{
		// out of visual range
		return( 0 );
	}

	ddHorizAngle = atan2( dDeltaY, dDeltaX );

#ifdef LOS_DEBUG
	memset( &gLOSTestResults, 0, sizeof( LOSResults ) );
	gLOSTestResults.fLOSTestPerformed = TRUE;
	gLOSTestResults.iStartX = (INT32) dStartX;
	gLOSTestResults.iStartY = (INT32) dStartY;
	gLOSTestResults.iStartZ = (INT32) dStartZ;
	gLOSTestResults.iEndX = (INT32) dEndX;
	gLOSTestResults.iEndY = (INT32) dEndY;
	gLOSTestResults.iEndZ = (INT32) dEndZ;
	gLOSTestResults.iMaxDistance = (INT32) iSightLimit;
	gLOSTestResults.iDistance = (INT32) dDistance;
#endif

	qIncrX = FloatToFixed( dDeltaX / (FLOAT)iDistance );
	qIncrY = FloatToFixed( dDeltaY / (FLOAT)iDistance );
	qIncrZ = FloatToFixed( dDeltaZ / (FLOAT)iDistance );

	fCheckForRoof = FALSE;

	// figure out starting and ending cubes
	iGridNo = GETWORLDINDEXFROMWORLDCOORDS( (INT32)dStartY, (INT32)dStartX );
	qCurrZ = FloatToFixed( dStartZ );
	qLandHeight = INT32_TO_FIXEDPT( CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ iGridNo ].sHeight ) );
	iCurrAboveLevelZ = FIXEDPT_TO_INT32( qCurrZ - qLandHeight );

	iStartCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );
	iStartCubesAboveLevelZ = iStartCubesZ;
	if (iStartCubesAboveLevelZ >= STRUCTURE_ON_GROUND_MAX)
	{
		iStartCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
	}

	// check to see if we need to check for roofs based on the starting gridno
	qWallHeight = gqStandardWallHeight + qLandHeight;
	if ( qCurrZ < qWallHeight )
	{
		// possibly going up through a roof on this level
		qCurrZ = FloatToFixed( dEndZ );

		if ( qCurrZ > qWallHeight )
		{
			fCheckForRoof = TRUE;
		}

	}
	else // >
	{
		// possibly going down through a roof on this level
		qCurrZ = FloatToFixed( dEndZ );

		if (qCurrZ < qWallHeight)
		{
			fCheckForRoof = TRUE;
		}
	}

	iGridNo = GETWORLDINDEXFROMWORLDCOORDS( (INT32)dEndY, (INT32)dEndX );
	qCurrZ = FloatToFixed( dEndZ );
	qLandHeight = INT32_TO_FIXEDPT( CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ iGridNo ].sHeight ) );
	iCurrAboveLevelZ = FIXEDPT_TO_INT32( qCurrZ - qLandHeight );
	iEndCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );
	iEndCubesAboveLevelZ = iEndCubesZ;
	if (iEndCubesAboveLevelZ >= STRUCTURE_ON_GROUND_MAX)
	{
		iEndCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
	}

	// check to see if we need to check for roofs based on the starting gridno
	qWallHeight = gqStandardWallHeight + qLandHeight;

	if ( qCurrZ < qWallHeight )
	{
		// possibly going down through a roof on this level
		qCurrZ = FloatToFixed( dStartZ );

		if ( qCurrZ > qWallHeight )
		{
			fCheckForRoof = TRUE;
		}

	}
	else // >
	{
		// possibly going up through a roof on this level
		qCurrZ = FloatToFixed( dStartZ );

		if (qCurrZ < qWallHeight)
		{
			fCheckForRoof = TRUE;
		}
	}

	// apply increments for first move

	// first move will be 1 step
	// plus a fractional part equal to half of the difference between the delta and
	// the increment times the distance

	qCurrX = FloatToFixed( dStartX ) + qIncrX + ( FloatToFixed( dDeltaX ) - qIncrX * iDistance ) / 2;
	qCurrY = FloatToFixed( dStartY ) + qIncrY + ( FloatToFixed( dDeltaY ) - qIncrY * iDistance ) / 2;
	qCurrZ = FloatToFixed( dStartZ ) + qIncrZ + ( FloatToFixed( dDeltaZ ) - qIncrZ * iDistance ) / 2;

	iCurrTileX = FIXEDPT_TO_TILE_NUM( qCurrX );
	iCurrTileY = FIXEDPT_TO_TILE_NUM( qCurrY );
	bLOSIndexX = FIXEDPT_TO_LOS_INDEX( qCurrX );
	bLOSIndexY = FIXEDPT_TO_LOS_INDEX( qCurrY );
	iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( qCurrZ ) );

	iLoop = 1;

	do
	{
		// check a particular tile

		// retrieve values from world for this particular tile
		iGridNo = iCurrTileX + iCurrTileY * WORLD_COLS;

		pMapElement = &(gpWorldLevelData[ iGridNo ]);
		qLandHeight = INT32_TO_FIXEDPT( CONVERT_PIXELS_TO_HEIGHTUNITS( pMapElement->sHeight ) );
		qWallHeight = gqStandardWallHeight + qLandHeight;

		if (fCheckForRoof)
		{
			pRoofStructure = FindStructure( iGridNo, STRUCTURE_ROOF );

			if ( pRoofStructure )
			{

				qLastZ = qCurrZ - qIncrZ;

				// if just on going to next tile we cross boundary, then roof stops sight here!
				if ( (qLastZ > qWallHeight && qCurrZ <= qWallHeight) || (qLastZ < qWallHeight && qCurrZ >= qWallHeight))
				{
					// hit a roof
					return( 0 );
				}

			}

		}

		// record old tile location for loop purposes
		iOldTileX = iCurrTileX;
		iOldTileY = iCurrTileY;
		do
		{
			// check a particular location within the tile

			// check for collision with the ground
			iCurrAboveLevelZ = FIXEDPT_TO_INT32( qCurrZ - qLandHeight );
			if (iCurrAboveLevelZ < 0)
			{
				// ground is in the way!
#ifdef LOS_DEBUG
				gLOSTestResults.iStoppedX = FIXEDPT_TO_INT32( qCurrX );
				gLOSTestResults.iStoppedY = FIXEDPT_TO_INT32( qCurrY );
				gLOSTestResults.iStoppedZ = FIXEDPT_TO_INT32( qCurrZ );
				// subtract one to compensate for rounding up when negative
				gLOSTestResults.iCurrCubesZ = 0;
#endif
				return( 0 );
			}
			// check for the existence of structures
			pStructure = pMapElement->pStructureHead;
			if (pStructure == NULL)
			{	// no structures in this tile, AND THAT INCLUDES ROOFS! :-)

				// new system; figure out how many steps until we cross the next edge
				// and then fast forward that many steps.

				iOldTileX = iCurrTileX;
				iOldTileY = iCurrTileY;
				iOldCubesZ = iCurrCubesZ;

				if (qIncrX > 0)
				{
					qDistToTravelX = INT32_TO_FIXEDPT( CELL_X_SIZE ) - (qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE ));
					iStepsToTravelX = qDistToTravelX / qIncrX;
				}
				else if (qIncrX < 0)
				{
					qDistToTravelX = qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE );
					iStepsToTravelX = qDistToTravelX / -qIncrX;
				}
				else
				{
					// make sure we don't consider X a limit :-)
					iStepsToTravelX = 1000000;
				}

				if (qIncrY > 0)
				{
					qDistToTravelY = INT32_TO_FIXEDPT( CELL_Y_SIZE ) - (qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE ));
					iStepsToTravelY = qDistToTravelY / qIncrY;
				}
				else if (qIncrY < 0)
				{
					qDistToTravelY = qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE );
					iStepsToTravelY = qDistToTravelY / -qIncrY;
				}
				else
				{
					// make sure we don't consider Y a limit :-)
					iStepsToTravelY = 1000000;
				}

				iStepsToTravel = __min( iStepsToTravelX, iStepsToTravelY ) + 1;

				/*
				if (qIncrX > 0)
				{
				qDistToTravelX = INT32_TO_FIXEDPT( CELL_X_SIZE ) - (qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE ));
				iStepsToTravelX = qDistToTravelX / qIncrX;
				}
				else if (qIncrX < 0)
				{
				qDistToTravelX = qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE );
				iStepsToTravelX = qDistToTravelX / (-qIncrX);
				}
				else
				{
				// make sure we don't consider X a limit :-)
				iStepsToTravelX = 1000000;
				}

				if (qIncrY > 0)
				{
				qDistToTravelY = INT32_TO_FIXEDPT( CELL_Y_SIZE ) - (qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE ));
				iStepsToTravelY = qDistToTravelY / qIncrY;
				}
				else if (qIncrY < 0)
				{
				qDistToTravelY = qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE );
				iStepsToTravelY = qDistToTravelY / (-qIncrY);
				}
				else
				{
				// make sure we don't consider Y a limit :-)
				iStepsToTravelY = 1000000;
				}

				// add 1 to the # of steps to travel to go INTO the next tile
				iStepsToTravel = __min( iStepsToTravelX, iStepsToTravelY ) + 1;
				//iStepsToTravel = 1;
				*/

				qCurrX += qIncrX * iStepsToTravel;
				qCurrY += qIncrY * iStepsToTravel;
				qCurrZ += qIncrZ * iStepsToTravel;
				iLoop += iStepsToTravel;

				// check for ground collision
				if (qCurrZ < qLandHeight && iLoop < iDistance)
				{
					// ground is in the way!
#ifdef LOS_DEBUG
					gLOSTestResults.iStoppedX = FIXEDPT_TO_INT32( qCurrX );
					gLOSTestResults.iStoppedY = FIXEDPT_TO_INT32( qCurrY );
					gLOSTestResults.iStoppedZ = FIXEDPT_TO_INT32( qCurrZ );
					// subtract one to compensate for rounding up when negative
					gLOSTestResults.iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrCubesAboveLevelZ ) - 1;
#endif
					return( 0 );
				}

				// figure out the new tile location
				iCurrTileX = FIXEDPT_TO_TILE_NUM( qCurrX );
				iCurrTileY = FIXEDPT_TO_TILE_NUM( qCurrY );
				iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( qCurrZ ) );
				bLOSIndexX = FIXEDPT_TO_LOS_INDEX( qCurrX );
				bLOSIndexY = FIXEDPT_TO_LOS_INDEX( qCurrY );
			}
			else
			{	// there are structures in this tile

				iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );
				// figure out the LOS cube level of the current point

				if (iCurrCubesAboveLevelZ < STRUCTURE_ON_ROOF_MAX)
				{
					if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
					{
						// check objects on the ground
						sDesiredLevel = STRUCTURE_ON_GROUND;
					}
					else
					{
						// check objects on roofs
						sDesiredLevel = STRUCTURE_ON_ROOF;
						iCurrCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
					}
					// check structures for collision
					while (pStructure != NULL)
					{
						// transparent structures should be skipped
						// normal roof structures should be skipped here because their only bits are roof lips
						// and those should act as transparent
						fOpaque = (pStructure->fFlags & STRUCTURE_TRANSPARENT) == 0;
						if ( pStructure->fFlags & STRUCTURE_ROOF )
						{
							// roof lip; allow sighting if person on roof is near
							if ( (iLoop < 2 * CELL_X_SIZE || (iDistance - iLoop) < 2 * CELL_X_SIZE ) )
							{
								if ( iLoop <= CELL_X_SIZE + 1 || (iDistance - iLoop ) <= CELL_X_SIZE + 1 )
								{
									// right near edge, allow sighting at 3 tiles from roof edge if prone
									// less if standing, and we can tell that with iStartCubesZ and iEndCubesZ
									if ( iStartCubesZ < iEndCubesZ )
									{
										// looking up, so reduce for the target stance-height according to iEndCubesZ
										if ( iDistance >= (3 - iEndCubesAboveLevelZ) * CELL_X_SIZE )
										{
											fOpaque = FALSE;
										}
									}
									else
									{
										if ( iDistance >= (3 - iStartCubesAboveLevelZ) * CELL_X_SIZE )
										{
											fOpaque = FALSE;
										}
									}

								}
								else
								{
									if ( iDistance >= 12 * CELL_X_SIZE )
									{
										fOpaque = FALSE;
									}
								}
							}
						}

						if ( fOpaque )
						{
							if (pStructure->sCubeOffset == sDesiredLevel)
							{
								if (((*(pStructure->pShape))[bLOSIndexX][bLOSIndexY] & AtHeight[iCurrCubesAboveLevelZ]) > 0)
								{
									if (fSmell)
									{
										if (pStructure->fFlags & STRUCTURE_TREE)
										{
											// smell not stopped by vegetation
										}
										else if ((pStructure->fFlags & STRUCTURE_WALLNWINDOW))// && (pStructure->fFlags & STRUCTURE_OPEN))
										{
											// open/closed window, smell not stopped
										}
										else
										{
											if (pStructure->fFlags & STRUCTURE_WALLSTUFF)
											{
												DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LineOfSightTest: checking thru wall, thermal = %d",fSmell));
												// possibly at corner in which case we should let it pass
												fResolveHit = ResolveHitOnWall( pStructure, iGridNo, bLOSIndexX, bLOSIndexY, ddHorizAngle );
											}
											else
											{
												fResolveHit = TRUE;
											}
											if (fResolveHit)
											{
												// CJC, May 30:	smell reduced by obstacles but not stopped
												// if obstacle within 10 tiles
												iAdjSightLimit -= SMELL_REDUCTION_FOR_NEARBY_OBSTACLE;
												DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LineOfSightTest: hit a wall, thermal = %d, sight limit = %d, distance = %d",fSmell,iAdjSightLimit,iDistance));
												if (iLoop > 100 || iDistance > iAdjSightLimit)
												{
													DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LineOfSightTest: couldn't see thru wall, thermal = %d",fSmell));
													// out of visual range
#ifdef LOS_DEBUG
													gLOSTestResults.fOutOfRange = TRUE;
													gLOSTestResults.iCurrCubesZ = iCurrCubesZ;
#endif
													return( 0 );
												}

												/*
												// smell-line stopped by obstacle!
												#ifdef LOS_DEBUG
												gLOSTestResults.iStoppedX = FIXEDPT_TO_INT32( qCurrX );
												gLOSTestResults.iStoppedY = FIXEDPT_TO_INT32( qCurrY );
												gLOSTestResults.iStoppedZ = FIXEDPT_TO_INT32( qCurrZ );
												gLOSTestResults.iCurrCubesZ = iCurrCubesAboveLevelZ;
												#endif
												return( 0 );
												*/
											}
										}
									}
									else
									{
										if (pStructure->fFlags & STRUCTURE_TREE)
										{
											INT8 adjustment = GetSightAdjustmentBehindStructure( iLoop, pStructure, iCurrCubesAboveLevelZ+1 );
											iAdjSightLimit = iAdjSightLimit + iAdjSightLimit * adjustment/100;

#ifdef LOS_DEBUG
												gLOSTestResults.ubTreeSpotsHit++;
												gLOSTestResults.iMaxDistance = iSightLimit;
#endif
												if (iDistance > iAdjSightLimit && !cthCalc)
												{
													// out of visual range
#ifdef LOS_DEBUG
													gLOSTestResults.fOutOfRange = TRUE;
													gLOSTestResults.iCurrCubesZ = iCurrCubesZ;
#endif
													return( 0 );
												}
											}
										else if ( (pStructure->fFlags & STRUCTURE_WALLNWINDOW) && !(pStructure->fFlags & STRUCTURE_SPECIAL) && qCurrZ >= (gqStandardWindowBottomHeight + qLandHeight) && qCurrZ <= (gqStandardWindowTopHeight + qLandHeight))
										{
											// do nothing; windows are transparent (except ones marked as special)
											if (psWindowGridNo != NULL)
											{
												// we're supposed to note the location of this window!
												// but if a location has already been set then there are two windows, in which case
												// we abort												
												if (TileIsOutOfBounds(*psWindowGridNo))
												{
													*psWindowGridNo = iGridNo;
													return( iLoop );
												}
												else
												{
													//*psWindowGridNo = NOWHERE;
													//return( iLoop );
												}
											}
										}
										else
										{
											if (pStructure->fFlags & STRUCTURE_WALLSTUFF)
											{
												// possibly shooting at corner in which case we should let it pass
												fResolveHit = ResolveHitOnWall( pStructure, iGridNo, bLOSIndexX, bLOSIndexY, ddHorizAngle );
											}
											else
											{
												if (iCurrCubesAboveLevelZ < (STANDING_CUBES - 1) )
												{
													if ( (iLoop <= CLOSE_TO_FIRER) && (iCurrCubesAboveLevelZ <= iStartCubesAboveLevelZ) )
													{
														// if we are in the same vertical cube as the start,
														// and this is the height of the structure, then allow sight to go through
														// NB cubes are 0 based, heights 1 based
														iStructureHeight = StructureHeight( pStructure );
														fResolveHit = ( iCurrCubesAboveLevelZ != (iStructureHeight - 1) );
													}
													else if ( (iLoop >= (iDistance - CLOSE_TO_FIRER) ) && (iCurrCubesAboveLevelZ <= iEndCubesZ) && bAware )
													{
														// if we are in the same vertical cube as our destination,
														// and this is the height of the structure, and we are aware
														// then allow sight to go through
														// NB cubes are 0 based, heights 1 based
														iStructureHeight = StructureHeight( pStructure );
														fResolveHit = ( iCurrCubesAboveLevelZ != (iStructureHeight - 1) );
													}
													else
													{
														fResolveHit = TRUE;
													}
												}
												else
												{
													fResolveHit = TRUE;
												}
											}
											if (fResolveHit)
											{
												// hit the obstacle!
#ifdef LOS_DEBUG
												gLOSTestResults.iStoppedX = FIXEDPT_TO_INT32( qCurrX );
												gLOSTestResults.iStoppedY = FIXEDPT_TO_INT32( qCurrY );
												gLOSTestResults.iStoppedZ = FIXEDPT_TO_INT32( qCurrZ );
												gLOSTestResults.iCurrCubesZ = iCurrCubesAboveLevelZ;
#endif
												return( 0 );
											}
										}
									}
								}
							}
						}
						pStructure = pStructure->pNext;
					}
				}
				// got past all structures; go to next location within
				// tile, horizontally or vertically
				bOldLOSIndexX = bLOSIndexX;
				bOldLOSIndexY = bLOSIndexY;
				iOldCubesZ = iCurrCubesZ;
				do
				{
					qCurrX += qIncrX;
					qCurrY += qIncrY;
					if (pRoofStructure)
					{
						qLastZ = qCurrZ;
						qCurrZ += qIncrZ;
						if ( (qLastZ > qWallHeight && qCurrZ <= qWallHeight) || (qLastZ < qWallHeight && qCurrZ >= qWallHeight))
						{
							// hit a roof
							return( 0 );
						}
					}
					else
					{
						qCurrZ += qIncrZ;
					}

					iLoop++;
					bLOSIndexX = FIXEDPT_TO_LOS_INDEX( qCurrX );
					bLOSIndexY = FIXEDPT_TO_LOS_INDEX( qCurrY );
					iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( qCurrZ ) );
					// shouldn't need to check whether we are not at maximum range because
					// that will be caught below and this loop shouldn't go for more than a
					// couple of iterations.
				}
				while( (bLOSIndexX == bOldLOSIndexX) && (bLOSIndexY == bOldLOSIndexY) && (iCurrCubesZ == iOldCubesZ));
				iCurrTileX = FIXEDPT_TO_TILE_NUM( qCurrX );
				iCurrTileY = FIXEDPT_TO_TILE_NUM( qCurrY );
			}
		} while( (iCurrTileX == iOldTileX) && (iCurrTileY == iOldTileY) && (iLoop < iDistance));

		// leaving a tile, check to see if it had gas in it
		//		if ( pMapElement->ubExtFlags[0] & (MAPELEMENT_EXT_SMOKE | MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS) )
		if ( pMapElement->ubExtFlags[0] & (MAPELEMENT_EXT_SMOKE | MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS | MAPELEMENT_EXT_BURNABLEGAS) )
		{
			if ( (pMapElement->ubExtFlags[0] & MAPELEMENT_EXT_SMOKE) && !fSmell )
			{
				bSmoke++;

				// we can only see 3 tiles in smoke
				// (2 if we're IN smoke)

				if ( bSmoke >= 3 )
				{
					iAdjSightLimit = 0;
				}
				// unpopular
				/*
				else
				{
				// losing 1/3rd results in chances to hit which are WAY too low when firing from out of
				// two tiles of smoke... changing this to a 1/6 penalty

				iAdjSightLimit -= iSightLimit / 6;
				}
				*/
			}
			else
			{
				// reduce by 2 tiles per tile of tear gas or mustard gas
				iAdjSightLimit -= 2 * CELL_X_SIZE;
			}

			if ( iAdjSightLimit <= 0 )
			{
				// can't see, period!
				return( 0 );
			}
		}

	} while( iLoop < iDistance );
	// unless the distance is integral, after the loop there will be a
	// fractional amount of distance remaining which is unchecked
	// but we shouldn't(?) need to check it because the target is there!
#ifdef LOS_DEBUG
	gLOSTestResults.fLOSClear = TRUE;
#endif
	//ADB this was the original code and comments:
	// this somewhat complicated formula does the following:
	// it starts with the distance to the target
	// it adds the difference between the original and adjusted sight limit, = the amount of cover
	// it then scales the value based on the difference between the original sight limit and the
	//	very maximum possible in best lighting conditions
	//return( (iDistance + (iSightLimit - iAdjSightLimit)) * (MaxDistanceVisible() * CELL_X_SIZE) / iSightLimit );

	//in the original code, iSightLimit could vary depending exactly on what I can see
	//so assuming there was no cover, the returned value is altered exactly on iSightLimit
	//if my sight is low, then the function returns a value higher than MaxNormalDistanceVisible
	//this simulates a target farther away and harder to hit
	//if my sight is higher than the max distance, via sniper scope etc, then the function returns a lower value
	//this makes the target seem like it is closer and easier to hit


	//ADB this is the new code
	//the problem with the new code is there needs to be a way to calculate a line
	//without stopping when my eyes stop, but also including my sight limit!
	//so iTileSightLimit has 255 added to it if the line is supposed to be infinite

	int distanceWithCover = (iDistance + (iSightLimit - iAdjSightLimit));
	if (adjustForSight == false) {
		//I know I am doing a CTGT calc, so don't get carried away
		//just because I can see 999 tiles doesn't mean I can shoot it!
		//this preserves the original intent of the scalar, but makes it less likely iAdjSightLimit
		//is less than zero, which if it were the function would return, which we don't want if our sight is good
		return distanceWithCover;
	}

	//I know that my sight could be over the average distance, but I am not going to clamp it
	//because I do not think the actual value matters, and if it does,
	//then using the real value is closer to the original intent
	//AFAIK all instances that aren't CTGT just use this as a boolean test
	return( distanceWithCover * (MaxNormalDistanceVisible() * CELL_X_SIZE) / iSightLimit );
}

BOOLEAN CalculateSoldierZPos( SOLDIERTYPE * pSoldier, UINT8 ubPosType, FLOAT * pdZPos )
{
	UINT8		ubHeight;

	if ( pSoldier->ubBodyType == CROW )
	{
		// Crow always as prone...
		ubHeight = ANIM_PRONE;
	}
	else if ( pSoldier->MercInDeepWater( ) )
	{
		// treat as prone
		ubHeight = ANIM_PRONE;
	}
	else if ( pSoldier->MercInShallowWater( ) )
	{
		// treat as crouched
		ubHeight = ANIM_CROUCH;
	}
	else
	{
		if ( CREATURE_OR_BLOODCAT( pSoldier ) || pSoldier->ubBodyType == COW )
		{
			// this if statement is to avoid the 'creature weak spot' target
			// spot for creatures
			if ( ubPosType == HEAD_TARGET_POS || ubPosType == LEGS_TARGET_POS )
			{
				// override!
				ubPosType = TORSO_TARGET_POS;
			}
		}
		else if ( TANK( pSoldier ) )
		{
			// high up!
			ubPosType = HEAD_TARGET_POS;
		}

		ubHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;
	}

	switch( ubPosType )
	{
	case LOS_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_LOS_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_LOS_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_LOS_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case FIRING_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_FIRING_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_FIRING_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_FIRING_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case TARGET_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_TARGET_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_TARGET_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_TARGET_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case HEAD_TARGET_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_HEAD_TARGET_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_HEAD_TARGET_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_HEAD_TARGET_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case TORSO_TARGET_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_TORSO_TARGET_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_TORSO_TARGET_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_TORSO_TARGET_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case LEGS_TARGET_POS:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_LEGS_TARGET_POS;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_LEGS_TARGET_POS;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_LEGS_TARGET_POS;
			break;
		default:
			return( FALSE );
		}
		break;
	case HEIGHT:
		switch ( ubHeight )
		{
		case ANIM_STAND:
			*pdZPos = STANDING_HEIGHT;
			break;
		case ANIM_CROUCH:
			*pdZPos = CROUCHED_HEIGHT;
			break;
		case ANIM_PRONE:
			*pdZPos = PRONE_HEIGHT;
			break;
		default:
			return( FALSE );
		}
		break;
	}
	if ( pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV )
	{
		// reduce value for kids who are 2/3 the height of regular people
		*pdZPos = (*pdZPos * 2) / 3;
	}
	else if ( pSoldier->ubBodyType == ROBOTNOWEAPON || pSoldier->ubBodyType == LARVAE_MONSTER || pSoldier->ubBodyType == INFANT_MONSTER || pSoldier->ubBodyType == BLOODCAT )
	{
		// robot is 1/3 the height of regular people
		*pdZPos = *pdZPos / 3;
	}
	else if ( TANK ( pSoldier ) )
	{
		*pdZPos = (*pdZPos * 4) / 3;
	}

	if (pSoldier->pathing.bLevel > 0)
	{ // on a roof
		*pdZPos += WALL_HEIGHT_UNITS;
	}

	// IF this is a plane, strafe!
	// ATE: Don;t panic - this is temp - to be changed to a status flag....
	if ( pSoldier->ubID == MAX_NUM_SOLDIERS )
	{
		*pdZPos = ( WALL_HEIGHT_UNITS * 2 ) - 1;
	}

	*pdZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[pSoldier->sGridNo].sHeight );
	return( TRUE );
}

INT32 SoldierToSoldierLineOfSightTest( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier, INT8 bAware, int iTileSightLimit, UINT8 ubAimLocation, bool adjustForSight, bool cthCalc )
{
	FLOAT			dStartZPos, dEndZPos;
	BOOLEAN		fOk;
	BOOLEAN		fSmell;
	UINT8			ubPosType;

	// TO ADD: if in tear gas, reduce sight limit to 2 tiles
	CHECKF( pStartSoldier );
	CHECKF( pEndSoldier );
	fOk = CalculateSoldierZPos( pStartSoldier, LOS_POS, &dStartZPos );
	CHECKF( fOk );

	// HEADROCK HAM 3.6: Location of static sectors externalized, and there can be more than one. Also, modders can
	// determine whether bloodcats really are blind to enemies here at all.
	UINT8 ubSectorID = SECTOR(gWorldSectorX,gWorldSectorY);
	UINT8 PlacementType = gBloodcatPlacements[ ubSectorID ][ 0 ].PlacementType;
	// Does sector contain a Bloodcat Garrison?
	if (PlacementType == BLOODCAT_PLACEMENT_STATIC)
	{
		// Are bloodcats set to forgo attacking enemies?
		if (gBloodcatPlacements[ ubSectorID ][ gGameOptions.ubDifficultyLevel-1 ].ubFactionAffiliation == QUEENS_CIV_GROUP)
		{
			// skip sight between army & bloodcats
			if ( pStartSoldier->bTeam == ENEMY_TEAM && pEndSoldier->bTeam == CREATURE_TEAM && pEndSoldier->ubBodyType == BLOODCAT )
			{
				return( 0 );
			}
			if ( pStartSoldier->bTeam == CREATURE_TEAM && pStartSoldier->ubBodyType == BLOODCAT && pEndSoldier->bTeam == ENEMY_TEAM )
			{
				return( 0 );
			}
		}
		else if (gBloodcatPlacements[ ubSectorID ][ gGameOptions.ubDifficultyLevel-1 ].ubFactionAffiliation > NON_CIV_GROUP)
		{
			// Bloodcats in this sector belong to a faction. They adhere to certain rules as a result.
			if ( pEndSoldier->bTeam == CREATURE_TEAM && pEndSoldier->ubBodyType == BLOODCAT && pStartSoldier->bSide != gbPlayerNum)
			{
				// Target is a bloodcat. He can't be spotted by civilians no matter what.
				{
					return ( 0 );
				}
			}
			else if ( pStartSoldier->bTeam == CREATURE_TEAM && pStartSoldier->ubBodyType == BLOODCAT )
			{
				// Source is a bloodcat. He can only spot player-side soldiers, and only if hostile.
				if ( pEndSoldier->bSide != gbPlayerNum || pStartSoldier->aiData.bNeutral )
				{
					return ( 0 );
				}
			}
		}
	}

	if (pStartSoldier->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		// monsters use smell instead of sight!
		dEndZPos = STANDING_LOS_POS; // should avoid low rocks etc
		if (pEndSoldier->pathing.bLevel > 0)
		{ // on a roof
			dEndZPos += WALL_HEIGHT_UNITS;
		}
		dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[pEndSoldier->sGridNo].sHeight );
		fSmell = TRUE;
	}
	else
	{
		switch( ubAimLocation )
		{
		case AIM_SHOT_HEAD:
			ubPosType = HEAD_TARGET_POS;
			break;
		case AIM_SHOT_TORSO:
			ubPosType = TORSO_TARGET_POS;
			break;
		case AIM_SHOT_LEGS:
			ubPosType = LEGS_TARGET_POS;
			break;
		default:
			ubPosType = TARGET_POS;
			break;
		}

		fOk = CalculateSoldierZPos( pEndSoldier, ubPosType, &dEndZPos );
		if (!fOk)
		{
			return( FALSE );
		}
		CHECKF( fOk );
		fSmell = HasThermalOptics( pStartSoldier);
	}

	if ( TANK( pStartSoldier ) )
	{
		INT16		sDistance;

		sDistance = PythSpacesAway( pStartSoldier->sGridNo, pEndSoldier->sGridNo );

		if ( sDistance <= 8 )
		{
			// blind spot?
			if ( dEndZPos <= PRONE_LOS_POS )
			{
				return( FALSE );
			}
			else if ( sDistance <= 4 && dEndZPos <= CROUCHED_LOS_POS )
			{
				return( FALSE );
			}
		}
	}

	// needed for sight limit calculation
	if (iTileSightLimit == CALC_FROM_ALL_DIRS || iTileSightLimit == CALC_FROM_WANTED_DIR) {
		iTileSightLimit = pStartSoldier->GetMaxDistanceVisible( pEndSoldier->sGridNo, pEndSoldier->pathing.bLevel, iTileSightLimit );
		iTileSightLimit += iTileSightLimit * GetSightAdjustment(pEndSoldier) / 100;
	}

	// needed for gun hit calculation (can you even hit him)
	else if (iTileSightLimit == NO_DISTANCE_LIMIT) {
		iTileSightLimit = pStartSoldier->GetMaxDistanceVisible( pEndSoldier->sGridNo, pEndSoldier->pathing.bLevel, CALC_FROM_ALL_DIRS );
		iTileSightLimit += iTileSightLimit * GetSightAdjustment(pEndSoldier) / 100;
		iTileSightLimit += 255; // this shifts the limit for something special (we don't know yet)
	}

	// we assume that if we are given a limit it doesn't include stealth or similar stuff
	// for other function we assume the opposite but not this one, as we here are given the needed target soldier information to calculate sight adjustment
	else {
		iTileSightLimit += iTileSightLimit * GetSightAdjustment(pEndSoldier) / 100;
	}

	return( LineOfSightTest( (FLOAT) CenterX( pStartSoldier->sGridNo ), (FLOAT) CenterY( pStartSoldier->sGridNo ), dStartZPos, (FLOAT) CenterX( pEndSoldier->sGridNo ), (FLOAT) CenterY( pEndSoldier->sGridNo ), dEndZPos, iTileSightLimit, bAware, fSmell, NULL, adjustForSight, cthCalc ) );
	}

INT32 SoldierToLocationWindowTest( SOLDIERTYPE * pStartSoldier, INT32 sEndGridNo )
{
	// figure out if there is a SINGLE window between the looker and target
	FLOAT			dStartZPos, dEndZPos;
	INT16			sXPos, sYPos;
	INT32			sWindowGridNo = NOWHERE;
	INT32			iRet;

	CHECKF( pStartSoldier );
	dStartZPos = FixedToFloat( ((gqStandardWindowTopHeight + gqStandardWindowBottomHeight) / 2) );
	if (pStartSoldier->pathing.bLevel > 0)
	{ // on a roof
		dStartZPos += WALL_HEIGHT_UNITS;
	}
	dStartZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[pStartSoldier->sGridNo].sHeight );
	dEndZPos = dStartZPos;

	ConvertGridNoToXY( sEndGridNo, &sXPos, &sYPos );
	sXPos = sXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
	sYPos = sYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

	//ADB changed from 255 to 511 to handle new LOS test
	// We don't want to consider distance limits here so pass in tile sight limit of 255( + 256)
	// and consider trees as little as possible
	iRet = LineOfSightTest( (FLOAT) CenterX( pStartSoldier->sGridNo ), (FLOAT) CenterY( pStartSoldier->sGridNo ), dStartZPos, (FLOAT) sXPos, (FLOAT) sYPos, dEndZPos, 511, TRUE, FALSE, &sWindowGridNo );

	return( sWindowGridNo );
}

INT32 SoldierTo3DLocationLineOfSightTest( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel, INT8 bAware, int iTileSightLimit, bool adjustForSight )
{
	FLOAT						dStartZPos, dEndZPos;
	INT16						sXPos, sYPos;
	UINT8						ubTargetID;
	BOOLEAN					fOk;

	CHECKF( pStartSoldier );

	fOk = CalculateSoldierZPos( pStartSoldier, LOS_POS, &dStartZPos );
	CHECKF( fOk );


	if (bCubeLevel > 0)
	{
		dEndZPos = ((FLOAT) (bCubeLevel + bLevel * PROFILE_Z_SIZE ) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
		dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ sGridNo ].sHeight );
	}
	else
	{
		ubTargetID = WhoIsThere2( sGridNo, bLevel );
		if (ubTargetID != NOBODY)
		{
			// there's a merc there; do a soldier-to-soldier test
			return( SoldierToSoldierLineOfSightTest( pStartSoldier, MercPtrs[ubTargetID], bAware, iTileSightLimit, LOS_POS, adjustForSight) );
		}
		// else... assume standing height
		dEndZPos = STANDING_LOS_POS + bLevel * HEIGHT_UNITS;
		// add in ground height
		dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ sGridNo ].sHeight );
	}

	ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );
	sXPos = sXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
	sYPos = sYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

	if (iTileSightLimit == CALC_FROM_ALL_DIRS || iTileSightLimit == CALC_FROM_WANTED_DIR) {
		iTileSightLimit = pStartSoldier->GetMaxDistanceVisible( sGridNo, bLevel, iTileSightLimit );
	}
	else if (iTileSightLimit == NO_DISTANCE_LIMIT) {
		iTileSightLimit = 255 + pStartSoldier->GetMaxDistanceVisible( sGridNo, bLevel, CALC_FROM_ALL_DIRS );
	}

	return( LineOfSightTest( (FLOAT) CenterX( pStartSoldier->sGridNo ), (FLOAT) CenterY( pStartSoldier->sGridNo ), dStartZPos, (FLOAT) sXPos, (FLOAT) sYPos, dEndZPos, iTileSightLimit, bAware, HasThermalOptics( pStartSoldier), NULL, adjustForSight ) );
}

INT32 SoldierToVirtualSoldierLineOfSightTest( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bStance, INT8 bAware, int iTileSightLimit )
{
	FLOAT						dStartZPos, dEndZPos;
	INT16						sXPos, sYPos;
	BOOLEAN					fOk;

	CHECKF( pStartSoldier );

	fOk = CalculateSoldierZPos( pStartSoldier, LOS_POS, &dStartZPos );
	CHECKF( fOk );

	// manually calculate destination Z position.
	switch( bStance )
	{
	case ANIM_STAND:
		dEndZPos = STANDING_LOS_POS;
		break;
	case ANIM_CROUCH:
		dEndZPos = CROUCHED_LOS_POS;
		break;
	case ANIM_PRONE:
		dEndZPos = PRONE_LOS_POS;
		break;
	default:
		return( FALSE );
	}
	dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[sGridNo].sHeight );
	if (bLevel > 0)
	{ // on a roof
		dEndZPos += WALL_HEIGHT_UNITS;
	}


	ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );
	sXPos = sXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
	sYPos = sYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

	if (iTileSightLimit == CALC_FROM_ALL_DIRS || iTileSightLimit == CALC_FROM_WANTED_DIR) {
		iTileSightLimit = pStartSoldier->GetMaxDistanceVisible( sGridNo, bLevel, iTileSightLimit );
	}
	else if (iTileSightLimit == NO_DISTANCE_LIMIT) {
		iTileSightLimit = 255 + pStartSoldier->GetMaxDistanceVisible( sGridNo, bLevel, CALC_FROM_ALL_DIRS );
	}

	return( LineOfSightTest( (FLOAT) CenterX( pStartSoldier->sGridNo ), (FLOAT) CenterY( pStartSoldier->sGridNo ), dStartZPos, (FLOAT) sXPos, (FLOAT) sYPos, dEndZPos, iTileSightLimit, bAware, HasThermalOptics( pStartSoldier), NULL, false ) );
}

INT32 LocationToLocationLineOfSightTest( INT32 sStartGridNo, INT8 bStartLevel, INT32 sEndGridNo, INT8 bEndLevel, INT8 bAware, int iTileSightLimit )
{
	FLOAT						dStartZPos, dEndZPos;
	INT16						sStartXPos, sStartYPos, sEndXPos, sEndYPos;
	UINT8						ubStartID;

	ubStartID = WhoIsThere2( sStartGridNo, bStartLevel );
	if ( ubStartID != NOBODY )
	{
		return( SoldierTo3DLocationLineOfSightTest( MercPtrs[ ubStartID ], sEndGridNo, bEndLevel, 0, bAware, iTileSightLimit ) );
	}

	// else... assume standing heights
	dStartZPos = STANDING_LOS_POS + bStartLevel * HEIGHT_UNITS;
	// add in ground height
	dStartZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ sStartGridNo ].sHeight );

	ConvertGridNoToXY( sStartGridNo, &sStartXPos, &sStartYPos );
	sStartXPos = sStartXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
	sStartYPos = sStartYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

	dEndZPos = STANDING_LOS_POS + bEndLevel * HEIGHT_UNITS;
	// add in ground height
	dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ sEndGridNo ].sHeight );

	ConvertGridNoToXY( sEndGridNo, &sEndXPos, &sEndYPos );
	sEndXPos = sEndXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
	sEndYPos = sEndYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

	if (iTileSightLimit == CALC_FROM_ALL_DIRS || iTileSightLimit == CALC_FROM_WANTED_DIR) {
		iTileSightLimit = MaxNormalDistanceVisible();
	}
	else if (iTileSightLimit == NO_DISTANCE_LIMIT) {
		iTileSightLimit = 255 + MaxNormalDistanceVisible();
	}
	return( LineOfSightTest( (FLOAT)sStartXPos, (FLOAT)sStartYPos, dStartZPos, (FLOAT) sEndXPos, (FLOAT) sEndYPos, dEndZPos, iTileSightLimit, bAware, FALSE, NULL ) );
}

/*
INT32 BulletImpactReducedByRange( INT32 iImpact, INT32 iDistanceTravelled, INT32 iRange )
{
// for now, don't reduce, because did weird stuff to AI!
return( iImpact );

// only start reducing impact at distances greater than one range
//return( __max( 1, iImpact * ( 100 - ( PERCENT_BULLET_SLOWED_BY_RANGE * iDistanceTravelled ) / iRange ) / 100 ) );

}
*/

BOOLEAN BulletHitMerc( BULLET * pBullet, STRUCTURE * pStructure, BOOLEAN fIntended )
{
	INT32								iImpact, iDamage;
	EV_S_WEAPONHIT			SWeaponHit;
	INT32								sRange;
	SOLDIERTYPE *				pFirer = pBullet->pFirer;
	FLOAT								dZPosRelToMerc;
	UINT8								ubHitLocation = AIM_SHOT_RANDOM;
	UINT8								ubAttackDirection;
	UINT8								ubAmmoType;
	UINT32							uiChanceThrough;
	UINT8								ubSpecial = FIRE_WEAPON_NO_SPECIAL;
	INT16								sHitBy;
	BOOLEAN							fStopped = TRUE;
	INT8								bSlot;
	INT8								bHeadSlot = NO_SLOT;
	SOLDIERTYPE *				pTarget;
  INT32 sNewGridNo;
	BOOLEAN			 fCanSpewBlood = FALSE;
	INT8				bSpewBloodLevel;

	// HEADROCK HAM 5: Fragments read attacking weapon from explosive, not from firer. Theoretically,
	// all bullets should read this value...
	UINT16 usAttackingWeapon = 0;
	if (pBullet->fFragment)
	{
		usAttackingWeapon = pBullet->fromItem;
	}
	else
	{
		usAttackingWeapon = pFirer->usAttackingWeapon;
	}

	// structure IDs for mercs match their merc IDs
	pTarget = MercPtrs[ pStructure->usStructureID ];

	if (pBullet->usFlags & BULLET_FLAG_KNIFE)
	{
		// Place knife on guy....

		// See if they have room ( and make sure it's not in hand pos?
		// CHRISL:
		bSlot = FindEmptySlotWithin( pTarget, BIGPOCKSTART, NUM_INV_SLOTS );
		if (bSlot == NO_SLOT)
		{
			// Add item

			CreateItem( pBullet->fromItem, (INT8) pBullet->ubItemStatus, &gTempObject );

			AddItemToPool( pTarget->sGridNo, &gTempObject, -1 , pTarget->pathing.bLevel, 0, 0 );

			// Make team look for items
			NotifySoldiersToLookforItems( );
		}
		else
		{
			if ( Item[pBullet->fromItem].bloodieditem > 0 )
				CreateItem( Item[pBullet->fromItem].bloodieditem, (INT8) pBullet->ubItemStatus, &(pTarget->inv[bSlot]) );
			else
				CreateItem( pBullet->fromItem, (INT8) pBullet->ubItemStatus, &(pTarget->inv[bSlot]) );
		}

		ubAmmoType = AMMO_KNIFE;
	}
	else if (pBullet->fFragment)
	{
		// Read ammo type from the explosive data.
		ubAmmoType = Explosive[Item[pBullet->fromItem].ubClassIndex].ubFragType;
	}
	else
	{
		ubAmmoType = pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType;
	}

	// HEADROCK HAM 5.1: This is an utter hack, but it may be necessary. This soldier is hit by a fragment,
	// so he needs to have his animations be interruptible. This takes care of soldiers hitting themselves
	// with their own fragments.
	if (pBullet->fFragment && pTarget == pBullet->pFirer)
	{
		if (pTarget->usAniCode >1)
		{
			pTarget->flags.fInNonintAnim = FALSE;
			pTarget->flags.fRTInNonintAnim = FALSE;
		}
	}

	// at least partly compensate for "near miss" increases for this guy, after all, the bullet
	// actually hit him!
	// take this out for now at least... no longer certain that he was awarded a suppression pt
	// when the bullet got near him
	//pTarget->ubSuppressionPoints--;

	if ( pTarget->flags.uiStatusFlags & SOLDIER_VEHICLE || (pTarget->ubBodyType == COW || pTarget->ubBodyType == CROW || pTarget->ubBodyType == BLOODCAT) )
	{
		//ubHitLocation = pStructure->ubVehicleHitLocation;
		ubHitLocation = AIM_SHOT_TORSO;
	}
	else
	{
		// Determine where the person was hit...

		if ( CREATURE_OR_BLOODCAT( pTarget )	)
		{
			ubHitLocation = AIM_SHOT_TORSO;

			// adult monster types have a weak spot
			if ( (pTarget->ubBodyType >= ADULTFEMALEMONSTER) && (pTarget->ubBodyType <=	YAM_MONSTER) )
			{
				// HEADROCK HAM 5.1: Bullet data now contains its original coordinates.
				ubAttackDirection = (UINT8) GetDirectionToGridNoFromGridNo( pBullet->sOrigGridNo, pTarget->sGridNo );
				if ( ubAttackDirection == pTarget->ubDirection || ubAttackDirection == gOneCCDirection[ pTarget->ubDirection ] || ubAttackDirection == gOneCDirection[ pTarget->ubDirection ] )
				{
					// may hit weak spot!
					if (0) // check fact
					{
						uiChanceThrough = 30;
					}
					else
					{
						uiChanceThrough = 1;
					}

					if (PreRandom( 100 ) < uiChanceThrough)
					{
						ubHitLocation = AIM_SHOT_GLAND;
					}
				}
			}
		}

		if (ubHitLocation == AIM_SHOT_RANDOM) // i.e. if not set yet
		{

			if (pTarget->MercInDeepWater( ) )
			{
				// automatic head hit!
				ubHitLocation = AIM_SHOT_HEAD;
			}
			else
			{
				switch (gAnimControl[ pTarget->usAnimState ].ubEndHeight)
				{
				case ANIM_STAND:
					// Fall through to crouch if in shallow or medium water
					if ( !pTarget->MercInShallowWater( ) )
					{
						dZPosRelToMerc = FixedToFloat( pBullet->qCurrZ ) - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[pBullet->sGridNo].sHeight );
						if ( dZPosRelToMerc > HEIGHT_UNITS )
						{
							dZPosRelToMerc -= HEIGHT_UNITS;
						}
						if (dZPosRelToMerc > STANDING_HEAD_BOTTOM_POS)
						{
							ubHitLocation = AIM_SHOT_HEAD;
						}
						else if (dZPosRelToMerc < STANDING_TORSO_BOTTOM_POS )
						{
							ubHitLocation = AIM_SHOT_LEGS;
						}
						else
						{
							ubHitLocation = AIM_SHOT_TORSO;
						}
						break;
					}
				case ANIM_CROUCH:
					dZPosRelToMerc = FixedToFloat( pBullet->qCurrZ ) - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[pBullet->sGridNo].sHeight );
					if ( dZPosRelToMerc > HEIGHT_UNITS )
					{
						dZPosRelToMerc -= HEIGHT_UNITS;
					}
					if (dZPosRelToMerc > CROUCHED_HEAD_BOTTOM_POS)
					{
						ubHitLocation = AIM_SHOT_HEAD;
					}
					else if ( dZPosRelToMerc < CROUCHED_TORSO_BOTTOM_POS )
					{
						// prevent targets in water from being hit in legs
						ubHitLocation = AIM_SHOT_LEGS;
					}
					else
					{
						ubHitLocation = AIM_SHOT_TORSO;
					}
					break;
				case ANIM_PRONE:
					ubHitLocation = AIM_SHOT_TORSO;
					break;

				}
			}
		}

		if ( ( AmmoTypes[ubAmmoType].monsterSpit ) && (ubHitLocation == AIM_SHOT_HEAD) && ( ! (pTarget->flags.uiStatusFlags & SOLDIER_MONSTER) ) )
		{
			UINT8			ubOppositeDirection;

			// HEADROCK HAM 5.1: Bullet data now contains its original GridNo
			ubAttackDirection = (UINT8) GetDirectionToGridNoFromGridNo( pBullet->sOrigGridNo, pTarget->sGridNo );
			ubOppositeDirection = gOppositeDirection[ ubAttackDirection ];

			if ( ! ( ubOppositeDirection == pTarget->ubDirection || ubAttackDirection == gOneCCDirection[ pTarget->ubDirection ] || ubAttackDirection == gOneCDirection[ pTarget->ubDirection ] ) )
			{
				// lucky bastard was facing away!
			}
			//			else if ( ( (pTarget->inv[HEAD1POS].usItem == NIGHTGOGGLES) || (pTarget->inv[HEAD1POS].usItem == SUNGOGGLES) || (pTarget->inv[HEAD1POS].usItem == GASMASK) ) && ( PreRandom( 100 ) < (UINT32) (pTarget->inv[HEAD1POS][0]->data.objectStatus) ) )
			else if ( ( (pTarget->inv[HEAD1POS].exists() == true) ) && ( PreRandom( 100 ) < (UINT32) (pTarget->inv[HEAD1POS][0]->data.objectStatus) ) )
			{
				// lucky bastard was wearing protective stuff
				bHeadSlot = HEAD1POS;
			}
			else if ( ( (pTarget->inv[HEAD2POS].exists() == true) ) && ( PreRandom( 100 ) < (UINT32) (pTarget->inv[HEAD2POS][0]->data.objectStatus) ) )
			{
				// lucky bastard was wearing protective stuff
				bHeadSlot = HEAD2POS;
			}
			else
			{
				// splat!!
				ubSpecial = FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL;
			}

		}

	}

	// Determine damage, checking guy's armour, etc
	sRange = GetRangeInCellCoordsFromGridNoDiff( pBullet->sOrigGridNo, pTarget->sGridNo );

	if ( gTacticalStatus.uiFlags & GODMODE	&& !(pFirer->flags.uiStatusFlags & SOLDIER_PC))
	{
		// in god mode, and firer is computer controlled
		iImpact = 0;
		iDamage = 0;
	}
	else if (fIntended)
	{
		if (pFirer->aiData.bOppList[pTarget->ubID] == SEEN_CURRENTLY)
		{
			sHitBy = pBullet->sHitBy;
		}
		else
		{
			// hard to aim at something far away being reported by someone else!
			sHitBy = pBullet->sHitBy / 2;
		}
		// hit the intended target which was in our LOS
		// reduce due to range
		iImpact = pBullet->iImpact; //BulletImpactReducedByRange( pBullet->iImpact, pBullet->iLoop, pBullet->iRange );
		iImpact -= pBullet->iImpactReduction;
		if (iImpact < 0)
		{
			// shouldn't happen but
			iImpact = 0;
		}
		// HEADROCK HAM 5: Added argument
		iDamage = BulletImpact( pFirer, pBullet, pTarget, ubHitLocation, iImpact, sHitBy, &ubSpecial );
		// handle hit here...
		// HEADROCK HAM 5: Fragments not counted as shots.
		if( ( pFirer->bTeam == 0 ) && !(pBullet->fFragment) )
		{
			// SANDRO - new mercs' records 
			// if we shoot with buckshot or similar, do not count a hit for every pellet
			if ( pBullet->usFlags & BULLET_FLAG_BUCKSHOT ) 
			{
				// if just one pellet hits the target, record it as successful hit, ignore the rest
				if ( pTarget->bNumPelletsHitBy == 0 )
					gMercProfiles[ pFirer->ubProfile ].records.usShotsHit++;
			}
			else
			{
				gMercProfiles[ pFirer->ubProfile ].records.usShotsHit++;
			}
		}

		// intentionally shot
		pTarget->flags.fIntendedTarget = TRUE;

		if ( (pBullet->usFlags & BULLET_FLAG_BUCKSHOT) && ( pTarget->ubID == pFirer->ubTargetID ) )
		{
			pTarget->bNumPelletsHitBy++;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: Experience Gain
		//
		// The old CTH system gave extra experience points for extra aiming and difficult shots. However,
		// it relied upon knowing BEFOREHAND that the target was going to be hit by the bullet (thanks
		// to how the OCTH value operated).
		// In NCTH, we have no idea how accurate our bullet is until it hits the target. We know how carefully
		// we aimed though, so now that the bullet hits the intended target, we can calculate extra experience
		// based on the aiming value.
		
		if (UsingNewCTHSystem() == true && pFirer->bTeam == gbPlayerNum && pBullet->fAimed) // Only for single shot, first bullet in volley, or first bullet in spread.
		{
			UINT16 usExpGain = 10;
			usExpGain = (usExpGain * pBullet->sHitBy) / 100;

			if ( pTarget->ubBodyType == COW || pTarget->ubBodyType == CROW )
			{
				usExpGain /= 2;
			}
			else if ( pTarget->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pTarget ) || TANK( pTarget ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				usExpGain = 0;
			}

			if (!(pBullet->usFlags & BULLET_FLAG_KNIFE))
			{
				// MARKSMANSHIP/DEXTERITY GAIN: gun attack
				StatChange( pFirer, MARKAMT, ( UINT16 )( (usExpGain * 2) / 3 ), FROM_SUCCESS );
				StatChange( pFirer, DEXTAMT, ( UINT16 )( usExpGain / 3 ), FROM_SUCCESS );
			}
			else
			{
				// MARKSMANSHIP/DEXTERITY/AGILITY GAIN: throwing knife attack
				StatChange( pFirer, MARKAMT, ( UINT16 )( usExpGain / 5 ), FROM_SUCCESS );
				StatChange( pFirer, DEXTAMT, ( UINT16 )( (usExpGain * 2) / 5 ), FROM_SUCCESS );
				StatChange( pFirer, AGILAMT, ( UINT16 )( (usExpGain * 2) / 5 ), FROM_SUCCESS );
			}
		}
	}
	else
	{
		// if an accidental target was hit, don't give a bonus for good aim!
		sHitBy = 0;
		iImpact = pBullet->iImpact;
		//iImpact = BulletImpactReducedByRange( pBullet->iImpact, pBullet->iLoop, pBullet->iRange );
		iImpact -= pBullet->iImpactReduction;
		if (iImpact < 0)
		{
			// shouldn't happen but
			iImpact = 0;
		}
		// HEADROCK HAM 5: Added argument
		iDamage = BulletImpact( pFirer, pBullet, pTarget, ubHitLocation, iImpact, sHitBy, &ubSpecial );

		// accidentally shot
		pTarget->flags.fIntendedTarget = FALSE;
	}

	if ( AmmoTypes[ubAmmoType].monsterSpit )
	{
		if (bHeadSlot != NO_SLOT)
		{
			pTarget->inv[ bHeadSlot ][0]->data.objectStatus -= (INT8) ( (iImpact / 2) + Random( (iImpact / 2) ) );
			if ( pTarget->inv[ bHeadSlot ][0]->data.objectStatus <= USABLE )
			{
				if ( pTarget->inv[ bHeadSlot ][0]->data.objectStatus <= 0 )
				{
					DeleteObj( &(pTarget->inv[ bHeadSlot ]) );
					DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
				}
				// say curse?
			}
		}
	}
	else if (ubHitLocation == AIM_SHOT_HEAD)
	{
		// bullet to the head may damage any head item
		bHeadSlot = HEAD1POS + (INT8) Random( 2 );
		if ( pTarget->inv[ bHeadSlot ].exists() == true )
		{
			pTarget->inv[ bHeadSlot ][0]->data.objectStatus -= (INT8) ( Random( iImpact / 2 ) );
			if ( pTarget->inv[ bHeadSlot ][0]->data.objectStatus < 0 )
			{
				// just break it...
				pTarget->inv[ bHeadSlot ][0]->data.objectStatus = 1;
			}
		}
	}

	// check to see if the guy is a friendly?..if so, up the number of times wounded
	if( ( pTarget->bTeam == gbPlayerNum ) )
	{
		if ( pBullet->usFlags & BULLET_FLAG_BUCKSHOT ) 
		{
			// SANDRO - if just one pellet hits the target, record it as being wounded, ignore the rest
			if ( pTarget->bNumPelletsHitBy == 0 )
				gMercProfiles[ pTarget->ubProfile ].records.usTimesWoundedShot++;
		}
		else if ( iDamage > 1 ) // damage of 1 is just a scratch, not a real wound
		{
			if ( pBullet->usFlags & BULLET_FLAG_KNIFE )
				gMercProfiles[ pTarget->ubProfile ].records.usTimesWoundedStabbed++;
			else
				gMercProfiles[ pTarget->ubProfile ].records.usTimesWoundedShot++;
		}
	}

	// check to see if someone was accidentally hit when no target was specified by the player
	if ( pFirer->bTeam == gbPlayerNum && pFirer->ubTargetID == NOBODY && pTarget->aiData.bNeutral	)
	{
		if ( pTarget->ubCivilianGroup == KINGPIN_CIV_GROUP || pTarget->ubCivilianGroup == HICKS_CIV_GROUP )
		{
			// hicks and kingpin are touchy!
			pFirer->ubTargetID = pTarget->ubID;
		}
		else if ( Random( 100 ) < 60 )
		{
			// get touchy
			pFirer->ubTargetID = pTarget->ubID;
		}

	}


	// Send event for getting hit
	memset( &(SWeaponHit), 0, sizeof( SWeaponHit ) );
	SWeaponHit.usSoldierID			= pTarget->ubID;
	SWeaponHit.uiUniqueId				= pTarget->uiUniqueSoldierIdValue;
	// HEADROCK HAM 5: Already defined.
	SWeaponHit.usWeaponIndex		= usAttackingWeapon;
	SWeaponHit.sDamage					= (INT16) iDamage;
	// breath loss is based on original impact of bullet
	SWeaponHit.sBreathLoss			= (INT16) ( (iImpact * APBPConstants[BP_GET_WOUNDED] * (pTarget->bBreathMax * 100 - pTarget->sBreathRed)) / 10000 );
	// HEADROCK HAM 5.1: Bullet data contains the correct original GridNo
	SWeaponHit.usDirection			= GetDirectionFromGridNo( pBullet->sOrigGridNo, pTarget );
	SWeaponHit.sXPos						= (INT16)pTarget->dXPos;
	SWeaponHit.sYPos						= (INT16)pTarget->dYPos;
	SWeaponHit.sZPos						= 20;
	SWeaponHit.sRange						= sRange;
	SWeaponHit.ubAttackerID			= pFirer->ubID;
	SWeaponHit.fHit							= TRUE;
	SWeaponHit.ubLocation				= ubHitLocation;

	if ( (pFirer->bDoBurst) && (ubSpecial == FIRE_WEAPON_NO_SPECIAL) )
	{
		// the animation required by the bullet hit (head explosion etc) overrides the
		// hit-by-a-burst animation
		ubSpecial = FIRE_WEAPON_BURST_SPECIAL;
	}
	SWeaponHit.ubSpecial = ubSpecial;

	// now check to see if the bullet goes THROUGH this person! (not vehicles)
	// HEADROCK HAM 5: Attacking weapon already defined above.
	if ( !(pTarget->flags.uiStatusFlags & SOLDIER_VEHICLE) && (AmmoTypes[ubAmmoType].canGoThrough) && !EXPLOSIVE_GUN( usAttackingWeapon ) )
	{
		// if we do more damage than expected, then the bullet will be more likely
		// to be lodged in the body

		// if we do less than expected, then the bullet has been slowed and less
		// likely to continue

		// higher chance for bigger guns, because they'll go through the back armour

		// reduce impact to match damage, if damage wasn't more than the impact
		// due to good aim, etc.
		if (iDamage < iImpact)
		{
			iImpact = iDamage;
		}
		uiChanceThrough = (UINT8) __max( 0, ( iImpact - 20 ) );
		if (PreRandom( 100 ) < uiChanceThrough )
		{
			// bullet MAY go through
			// adjust for bullet going through person
			iImpact -= CalcBodyImpactReduction( ubAmmoType, ubHitLocation );
			// adjust for other side of armour!
			iImpact -= TotalArmourProtection( pFirer, pTarget, ubHitLocation, iImpact, ubAmmoType );
			if (iImpact > 0)
			{
				pBullet->iImpact = (INT8) iImpact;
				// bullet was NOT stopped
				fStopped = FALSE;
			}
		}
	}

	if ( gTacticalStatus.ubCurrentTeam != OUR_TEAM && pTarget->bTeam == gbPlayerNum )
	{
		// someone has been hit so no close-call quotes
		gTacticalStatus.fSomeoneHit = TRUE;
	}

	// handle hit!	Handle damage before deleting the bullet though
	//hayden
	if(SWeaponHit.ubAttackerID < 20 ||(is_server && SWeaponHit.ubAttackerID < 120)|| !is_client ) // 124 last possible ai 
	{
		if(is_client) 
			SWeaponHit.sDamage=(INT16)(SWeaponHit.sDamage * cDamageMultiplier); // adjust damage from external variable //hayden
		
		WeaponHit( SWeaponHit.usSoldierID, SWeaponHit.usWeaponIndex, SWeaponHit.sDamage, SWeaponHit.sBreathLoss, SWeaponHit.usDirection, SWeaponHit.sXPos, SWeaponHit.sYPos, SWeaponHit.sZPos, SWeaponHit.sRange, SWeaponHit.ubAttackerID, SWeaponHit.fHit, SWeaponHit.ubSpecial, SWeaponHit.ubLocation );
		
		
		if(is_server || (is_client && SWeaponHit.ubAttackerID <20) ) 
		{
			SWeaponHit.fStopped=fStopped;
			SWeaponHit.iBullet=pBullet->iBullet;

			send_hit( &SWeaponHit );
		}
	}

	if (fStopped)
	{
		RemoveBullet( pBullet->iBullet );
	}
	else
	{
		// ATE: I'm in enemy territory again, evil CC's world :)
		// This looks like the place I should add code to spew blood on the ground
		// The algorithm I'm going to use is given the current gridno of bullet,
		// get a new gridno based on direction it was moving.	Check to see if we're not
		// going through walls, etc by testing for a path, unless on the roof, in which case it would always
		// be legal, but the bLevel May change...
  	sNewGridNo = NewGridNo( pBullet->sGridNo, DirectionInc( gOppositeDirection[ SWeaponHit.usDirection ] ) );

		bSpewBloodLevel = MercPtrs[ SWeaponHit.usSoldierID ]->pathing.bLevel;
		fCanSpewBlood	= TRUE;

		// If on anything other than bLevel of 0, we can pretty much freely spew blood
		if ( bSpewBloodLevel == 0 )
		{
			if ( gubWorldMovementCosts[ sNewGridNo ][ gOppositeDirection[ SWeaponHit.usDirection ] ][ 0 ] >= TRAVELCOST_BLOCKED )
			{
				fCanSpewBlood = FALSE;
			}
		}
		else
		{
			// If a roof does not exist here, make level = 0
      if ( !IsRoofPresentAtGridNo( sNewGridNo ) )
			{
				bSpewBloodLevel = 0;
			}
		}

		if ( fCanSpewBlood )
		{
			// Drop blood dude!
			InternalDropBlood( sNewGridNo, bSpewBloodLevel, 0, (UINT8)(MAXBLOODQUANTITY), 1 );
		}
	}

	return( fStopped );
}

void BulletHitStructure( BULLET * pBullet, UINT16 usStructureID, INT32 iImpact, SOLDIERTYPE * pFirer, FIXEDPT qCurrX, FIXEDPT qCurrY, FIXEDPT qCurrZ, BOOLEAN fStopped )
{
	EV_S_STRUCTUREHIT		SStructureHit;

	SStructureHit.sXPos = (INT16) FIXEDPT_TO_INT32( qCurrX + FloatToFixed( 0.5f ) ); // + 0.5);
	SStructureHit.sYPos = (INT16) FIXEDPT_TO_INT32( qCurrY + FloatToFixed( 0.5f ) ); // (dCurrY + 0.5);
	SStructureHit.sZPos = CONVERT_HEIGHTUNITS_TO_PIXELS( (INT16) FIXEDPT_TO_INT32( qCurrZ + FloatToFixed( 0.5f )) );// dCurrZ + 0.5) );
	// HEADROCK HAM 5.1: Bullet already contains this data...
	SStructureHit.usWeaponIndex = pBullet->fromItem;
	SStructureHit.bWeaponStatus = pBullet->ubItemStatus;
	SStructureHit.ubAttackerID	= pFirer->ubID;
	SStructureHit.usStructureID = usStructureID;
	SStructureHit.iImpact				= iImpact;
	SStructureHit.iBullet				= pBullet->iBullet;
	
	if (is_networked)
		SStructureHit.fStopped = fStopped;

	//hayden
	if(is_client)
		send_hitstruct(&SStructureHit);

	StructureHit( SStructureHit.iBullet, SStructureHit.usWeaponIndex, SStructureHit.bWeaponStatus, SStructureHit.ubAttackerID, SStructureHit.sXPos, SStructureHit.sYPos, SStructureHit.sZPos, SStructureHit.usStructureID, SStructureHit.iImpact, fStopped );
}

void BulletHitWindow( BULLET *pBullet, INT32 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth )
{
	if (is_networked)
	{
		EV_S_WINDOWHIT	SWindowHit;
		SWindowHit.sGridNo = sGridNo;
		SWindowHit.usStructureID = usStructureID;
		SWindowHit.fBlowWindowSouth = fBlowWindowSouth;
		SWindowHit.fLargeForce = FALSE;
		SWindowHit.iBullet = pBullet->iBullet;
		SWindowHit.ubAttackerID=pBullet->pFirer->ubID;
		//hayden
		if(is_client)
			send_hitwindow(&SWindowHit);
	}

	WindowHit( sGridNo, usStructureID, fBlowWindowSouth, FALSE );
}

void BulletMissed( BULLET *pBullet, SOLDIERTYPE * pFirer )
{
	if (is_networked)
	{
		EV_S_MISS SMiss;
		SMiss.iBullet=pBullet->iBullet;
		SMiss.ubAttackerID=pFirer->ubID;
		//hayden
		if(is_client)
			send_miss(&SMiss);
	}

	ShotMiss( pFirer->ubID, pBullet->iBullet );
}

UINT32 ChanceOfBulletHittingStructure( INT32 iDistance, INT32 iDistanceToTarget, INT16 sHitBy )
{
	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ChanceOfBulletHittingStructure"));
	INT32 iCloseToCoverPenalty;

	if (iDistance / CELL_X_SIZE > MAX_DIST_FOR_LESS_THAN_MAX_CHANCE_TO_HIT_STRUCTURE)
	{
		return( MAX_CHANCE_OF_HITTING_STRUCTURE );
	}
	else
	{
		iCloseToCoverPenalty = iDistance / 5 - (iDistanceToTarget - iDistance);
		if (iCloseToCoverPenalty < 0)
		{
			iCloseToCoverPenalty = 0;
		}
		// HEADROCK HAM 4: HitBy is not longer handled this way, and will never drop below 0 anyway.
		// A new method might be devised to handle this, but in practice there's really no reason to.
		// I'm commenting this part out for now. If we discover that bullets are far too likely to go
		// through objects for any reason, we'll put it back in somehow.
		if (UsingNewCTHSystem() == false && sHitBy < 0)
		{
			// add 20% to distance so that misses hit nearer obstacles a bit more
			iDistance += iDistance / 5;
		}
		if ( ( (iDistance + iCloseToCoverPenalty) / CELL_X_SIZE ) > MAX_DIST_FOR_LESS_THAN_MAX_CHANCE_TO_HIT_STRUCTURE )
		{
			//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ChanceOfBulletHittingStructure done"));
			return( MAX_CHANCE_OF_HITTING_STRUCTURE );
		}
		else
		{
			//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ChanceOfBulletHittingStructure done"));
			return( guiStructureHitChance[ (iDistance + iCloseToCoverPenalty) / CELL_X_SIZE ] );
		}
	}
}

INT32 StructureResistanceIncreasedByRange( INT32 iImpactReduction, INT32 iGunRange, INT32 iDistance )
{
	return( iImpactReduction * ( 100 + PERCENT_BULLET_SLOWED_BY_RANGE * (iDistance - iGunRange) / iGunRange ) / 100 );
	/*
	if ( iDistance > iGunRange )
	{
	return( iImpactReduction * ( 100 + PERCENT_BULLET_SLOWED_BY_RANGE * (iDistance - iGunRange) / iGunRange ) / 100 );
	}
	else
	{
	return( iImpactReduction );
	}
	*/
}


INT32 HandleBulletStructureInteraction( BULLET * pBullet, STRUCTURE * pStructure, BOOLEAN * pfHit )
{
	DOOR		*pDoor;
	INT16		sLockDamage;

	// returns remaining impact amount

	// HEADROCK HAM 5.1: Define differently for fragments
	UINT8 ubAmmoType = 0;
	if (pBullet->fFragment)
	{
		ubAmmoType = Explosive[Item[pBullet->fromItem].ubClassIndex].ubFragType;
	}
	else
	{
		ubAmmoType = pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType;
	}

	INT32 iCurrImpact;
	INT32 iImpactReduction;

	*pfHit = FALSE;

	if (pBullet->usFlags & BULLET_FLAG_KNIFE || pBullet->usFlags & BULLET_FLAG_MISSILE || pBullet->usFlags & BULLET_FLAG_TANK_CANNON || pBullet->usFlags & BULLET_FLAG_FLAME )
	{
		// stops!
		*pfHit = TRUE;
		return( 0 );
	}
	//else if ( pBullet->usFlags & BULLET_FLAG_SMALL_MISSILE )
	//{
	// stops if using HE ammo
	else if ( AmmoTypes[ubAmmoType].highExplosive && !AmmoTypes[ubAmmoType].antiTank )
	{
		*pfHit = TRUE;
		return( 0 );
	}
	//	}

	// ATE: Alrighty, check for shooting door locks...
	// First check this is a type of struct that can handle locks...
	// HEADROCK HAM 5.1: Fragments have radical target gridnos, which would prevent them from opening a lock otherwise.
	if ( pStructure->fFlags & ( STRUCTURE_DOOR | STRUCTURE_OPENABLE ) && (PythSpacesAway( (INT16) pBullet->sTargetGridNo, pStructure->sGridNo ) <= 2 || pBullet->fFragment) )
	{
		// lookup lock table to see if we have a lock,
		// and then remove lock if enough damage done....
		pDoor = FindDoorInfoAtGridNo( pBullet->sGridNo );

		// Does it have a lock?
		INT16 lockBustingPower = AmmoTypes[ubAmmoType].lockBustingPower;

		// WANNE: bugfix: No door returned, so the game crashes!
		if (pDoor)
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Door info: damage = %d, pick difficulty = %d, smash difficulty = %d, lockbuster power = %d",pDoor->bLockDamage,LockTable[ pDoor->ubLockID ].ubPickDifficulty,LockTable[ pDoor->ubLockID ].ubSmashDifficulty,lockBustingPower) );

			if ( pDoor && (( LockTable[ pDoor->ubLockID ].ubPickDifficulty < 50 && LockTable[ pDoor->ubLockID ].ubSmashDifficulty < 70 ) || lockBustingPower*2 >= LockTable[ pDoor->ubLockID ].ubSmashDifficulty ) )
			{
				// Yup.....

				// Chance that it hit the lock....
				// HEADROCK HAM 5.1: Fragments are numerous, so reduce their chance of hitting the lock.
				UINT8 ubRandomChanceHitLock = 2;
				if (pBullet->fFragment)
				{
					ubRandomChanceHitLock = 10;
				}
				if ( PreRandom( ubRandomChanceHitLock ) == 0 || lockBustingPower > 0 )
				{
					// Adjust damage-- CC adjust this based on gun type, etc.....
					//sLockDamage = (INT16)( 35 + Random( 35 ) );
					sLockDamage = (INT16) (pBullet->iImpact - pBullet->iImpactReduction );
					sLockDamage += (INT16) PreRandom( sLockDamage );
					sLockDamage += lockBustingPower;

					sLockDamage = min(sLockDamage,127);

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ LOCK_HAS_BEEN_HIT ] );

					//Madd: catch the overflow
					if ( sLockDamage + pDoor->bLockDamage > 127 )
						pDoor->bLockDamage = 127;
					else
						pDoor->bLockDamage+= sLockDamage;

					// Check if it has been shot!
					if ( pDoor->bLockDamage > LockTable[ pDoor->ubLockID ].ubSmashDifficulty || sLockDamage > LockTable[ pDoor->ubLockID ].ubSmashDifficulty )
					{
						// Display message!
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ LOCK_HAS_BEEN_DESTROYED ] );

						// succeeded! door can never be locked again, so remove from door list...
						RemoveDoorInfoFromTable( pDoor->sGridNo );

						// MARKSMANSHIP GAIN (marksPts): Opened/Damaged a door
						// HEADROCK HAM 5.1: Fragments don't increase marksmanship, but can increase Explosives!
						if (pBullet->fFragment == FALSE)
						{
							StatChange( pBullet->pFirer, MARKAMT, 10, FALSE );
						}
						else
						{
							StatChange( pBullet->pFirer, EXPLODEAMT, 3, FALSE );
						}

						// SANDRO - merc records - locks breached
						if ( pBullet->pFirer->ubProfile != NO_PROFILE )
							gMercProfiles[ pBullet->pFirer->ubProfile ].records.usLocksBreached++;
					}
				}
			}
		}
	}

	// okay, this seems pretty weird, so here's the comment to explain it:
	// iImpactReduction is the reduction in impact due to the structure
	// pBullet->iImpactReduction is the accumulated reduction in impact
	//	for all bullets encountered thus far
	// iCurrImpact is the original impact value of the bullet reduced due to
	//	range.	To avoid problems involving multiple multiplication
	//	( (1 - X) * (1 - Y) != (1 - X - Y) ! ), this is calculated from
	//	scratch at each collision with an obstacle
	//	reduction due to range is 25% per "max range"
	if (PreRandom( 100 ) < pStructure->pDBStructureRef->pDBStructure->ubDensity)
	{
		iCurrImpact = pBullet->iImpact;
		//iCurrImpact = BulletImpactReducedByRange( pBullet->iImpact, pBullet->iLoop, pBullet->iRange );
		iImpactReduction = gubMaterialArmour[ pStructure->pDBStructureRef->pDBStructure->ubArmour ];
		iImpactReduction = StructureResistanceIncreasedByRange( iImpactReduction, pBullet->iRange, pBullet->iLoop );

		iImpactReduction = (INT32) (iImpactReduction * AmmoTypes[ubAmmoType].structureImpactReductionMultiplier / max(1,AmmoTypes[ubAmmoType].structureImpactReductionDivisor));

		//switch (pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType)
		//{
		//	case AMMO_HP:
		//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_HP( iImpactReduction );
		//		break;
		//	case AMMO_AP:
		//	case AMMO_HEAT:
		//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_AP( iImpactReduction );
		//		break;
		//	case AMMO_SUPER_AP:
		//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_SAP( iImpactReduction );
		//		break;
		//	default:
		//		break;
		//}

		pBullet->iImpactReduction += iImpactReduction;

		// really weak stuff like grass should never *stop* a bullet, maybe slow it though
		if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_LIGHT_VEGETATION )
		{
			// just return a +ve value to indicate the bullet wasn't stopped
			*pfHit = FALSE;
			return( 1 );
		}

		*pfHit = TRUE;
		return( iCurrImpact - pBullet->iImpactReduction );
	}
	else
	{
		// just return a +ve value to indicate the bullet wasn't stopped
		*pfHit = FALSE;
		return( 1 );
	}
}

INT32 CTGTHandleBulletStructureInteraction( BULLET * pBullet, STRUCTURE * pStructure )
{
	// returns reduction in impact for summing in CTGT

	INT32 iCurrImpact;
	INT32 iImpactReduction;

	if (pBullet->usFlags & BULLET_FLAG_KNIFE || pBullet->usFlags & BULLET_FLAG_MISSILE || pBullet->usFlags & BULLET_FLAG_FLAME || pBullet->usFlags & BULLET_FLAG_TANK_CANNON )
	{
		// knife/rocket stops when it hits anything, and people block completely
		return( pBullet->iImpact );
	}
	//else if ( pBullet->usFlags & BULLET_FLAG_SMALL_MISSILE )
	//{
	// stops if using HE ammo
	else if ( AmmoTypes[pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType].highExplosive && !AmmoTypes[pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType].antiTank )
	{
		return( pBullet->iImpact );
	}
	//	}
	else if ( pStructure->fFlags & STRUCTURE_PERSON )
	{
		if ( pStructure->usStructureID != pBullet->ubFirerID && pStructure->usStructureID != pBullet->ubTargetID )
		{
		}
	}

	// okay, this seems pretty weird, so here's the comment to explain it:
	// iImpactReduction is the reduction in impact due to the structure
	// pBullet->iImpactReduction is the accumulated reduction in impact
	//	for all bullets encountered thus far
	// iCurrImpact is the original impact value of the bullet reduced due to
	//	range.	To avoid problems involving multiple multiplication
	//	( (1 - X) * (1 - Y) != (1 - X - Y) ! ), this is calculated from
	//	scratch at each collision with an obstacle
	//	reduction due to range is 25% per "max range"
	//iCurrImpact = BulletImpactReducedByRange( pBullet->iImpact, pBullet->iLoop, pBullet->iRange );
	iCurrImpact = pBullet->iImpact;
	// multiply impact reduction by 100 to retain fractions for a bit...
	iImpactReduction = gubMaterialArmour[ pStructure->pDBStructureRef->pDBStructure->ubArmour ] * pStructure->pDBStructureRef->pDBStructure->ubDensity / 100;
	iImpactReduction = StructureResistanceIncreasedByRange( iImpactReduction, pBullet->iRange, pBullet->iLoop );

	iImpactReduction = (INT32)(iImpactReduction * AmmoTypes[pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType].structureImpactReductionMultiplier / max(1,AmmoTypes[pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType].structureImpactReductionDivisor));

	//switch (pBullet->pFirer->inv[ pBullet->pFirer->ubAttackingHand ][0]->data.gun.ubGunAmmoType)
	//{
	//	case AMMO_HP:
	//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_HP( iImpactReduction );
	//		break;
	//	case AMMO_AP:
	//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_AP( iImpactReduction );
	//		break;
	//	case AMMO_SUPER_AP:
	//		iImpactReduction = AMMO_STRUCTURE_ADJUSTMENT_SAP( iImpactReduction );
	//		break;
	//	default:
	//		break;
	//}
	return( iImpactReduction );
}


UINT8 CalcChanceToGetThrough( BULLET * pBullet )
{
	FIXEDPT	qLandHeight;
	INT32		iCurrAboveLevelZ;
	INT32		iCurrCubesAboveLevelZ;
	INT16		sDesiredLevel;

	INT32		iOldTileX;
	INT32		iOldTileY;
	INT32		iOldCubesZ;

	MAP_ELEMENT *		pMapElement;
	STRUCTURE *			pStructure;
	STRUCTURE *			pRoofStructure = NULL;

	FIXEDPT					qLastZ;

	BOOLEAN					fIntended;
	INT8						bOldLOSIndexX;
	INT8						bOldLOSIndexY;

	INT32						iChanceToGetThrough = 100;

	FIXEDPT					qDistToTravelX;
	FIXEDPT					qDistToTravelY;
	INT32						iStepsToTravelX;
	INT32						iStepsToTravelY;
	INT32						iStepsToTravel;
	INT32						iNumLocalStructures;
	INT32						iStructureLoop;
	UINT32					uiChanceOfHit;
	INT32						iGridNo;
	INT32						iTotalStructureImpact;
	BOOLEAN					fResolveHit;

	FIXEDPT					qWallHeight;
	FIXEDPT					qWindowBottomHeight;
	FIXEDPT					qWindowTopHeight;

	DebugLOS( "Starting CalcChanceToGetThrough" );

	do
	{
		// check a particular tile
		// retrieve values from world for this particular tile
		iGridNo = pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS;
		DebugLOS( String( "CTGT now at %ld", iGridNo ) );
		pMapElement = &(gpWorldLevelData[ iGridNo ] );
		qLandHeight = INT32_TO_FIXEDPT( CONVERT_PIXELS_TO_HEIGHTUNITS( pMapElement->sHeight ) );
		qWallHeight = gqStandardWallHeight + qLandHeight;
		qWindowBottomHeight = gqStandardWindowBottomHeight + qLandHeight;
		qWindowTopHeight = gqStandardWindowTopHeight + qLandHeight;

		// Assemble list of structures we might hit!
		iNumLocalStructures = 0;
		pStructure = pMapElement->pStructureHead;
		// calculate chance of hitting each structure
		uiChanceOfHit = ChanceOfBulletHittingStructure( pBullet->iLoop, pBullet->iDistanceLimit, 0 );

		// reset roof structure pointer each tile
		pRoofStructure = NULL;

		if (iGridNo == (INT32) pBullet->sTargetGridNo)
		{
			fIntended = TRUE;
			// if in the same tile as our destination, we WANT to hit the structure!
			uiChanceOfHit = 100;
		}
		else
		{
			fIntended = FALSE;
		}

		iCurrAboveLevelZ = FIXEDPT_TO_INT32( pBullet->qCurrZ - qLandHeight );
		if (iCurrAboveLevelZ < 0)
		{
			// ground is in the way!
			DebugLOS(String("CalcChanceToGetThrough done ground in way #1"));
			return( 0 );
		}
		iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );

		DebugLOS(String("CalcChanceToGetThrough: while pStructure"));
		while( pStructure )
		{
			if (pStructure->fFlags & ALWAYS_CONSIDER_HIT)
			{
				// ALWAYS add walls
				gpLocalStructure[iNumLocalStructures] = pStructure;
				// fence is special
				//(iCurrCubesAboveLevelZ <= iStartCubesAboveLevelZ)
				if ( pStructure->fFlags & STRUCTURE_ANYFENCE )
				{
					if (pStructure->pDBStructureRef->pDBStructure->ubDensity < 100)
					{
						guiLocalStructureCTH[iNumLocalStructures] = uiChanceOfHit;
					}
					else if ( (pBullet->iLoop <= CLOSE_TO_FIRER) && (iCurrCubesAboveLevelZ <= pBullet->bStartCubesAboveLevelZ) && (pBullet->bEndCubesAboveLevelZ >= iCurrCubesAboveLevelZ) && iCurrCubesAboveLevelZ == (StructureHeight( pStructure ) - 1) )
					{
						guiLocalStructureCTH[iNumLocalStructures] = uiChanceOfHit;
					}
					else if ( (pBullet->iLoop + pBullet->iDistanceLimit / 50 >= pBullet->iDistanceLimit - CLOSE_TO_FIRER) && (iCurrCubesAboveLevelZ <= pBullet->bEndCubesAboveLevelZ) && iCurrCubesAboveLevelZ == (StructureHeight( pStructure ) - 1) )
					{
						guiLocalStructureCTH[iNumLocalStructures] = uiChanceOfHit;
					}
					else
					{
						guiLocalStructureCTH[iNumLocalStructures] = 100;
					}
				}
				else
				{
					guiLocalStructureCTH[iNumLocalStructures] = 100;
				}
				gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
				iNumLocalStructures++;
			}
			else if (pStructure->fFlags & STRUCTURE_ROOF)
			{
				// only consider roofs if the flag is set; don't add them to the array since they
				// are a special case
				if (pBullet->fCheckForRoof)
				{
					pRoofStructure = pStructure;

					if ( pRoofStructure )
					{
						qLastZ = pBullet->qCurrZ - pBullet->qIncrZ;

						// if just on going to next tile we cross boundary, then roof stops bullet here!
						if ( (qLastZ > qWallHeight && pBullet->qCurrZ <= qWallHeight) || (qLastZ < qWallHeight && pBullet->qCurrZ >= qWallHeight))
						{
							// hit a roof
							DebugLOS(String("ChanceChanceToGetThrough done hit roof #1" ));

							return( 0 );
						}
					}

				}
			}
			else if (pStructure->fFlags & STRUCTURE_PERSON)
			{
				if ( (pStructure->usStructureID != pBullet->ubFirerID) && (pStructure->usStructureID != pBullet->ubTargetID ) )
				{
					// ignore intended target since we will get closure upon reaching the center
					// of the destination tile

					// ignore intervening target if not visible; PCs are always visible so AI will never skip them on that
					// basis
					if ( !fIntended && (MercPtrs[ pStructure->usStructureID ]->bVisible == TRUE) )
					{
						// in actually moving the bullet, we consider only count friends as targets if the bullet is unaimed
						// (buckshot), if they are the intended target, or beyond the range of automatic friendly fire hits
						// OR a 1 in 30 chance occurs
						if (gAnimControl[ MercPtrs[pStructure->usStructureID]->usAnimState ].ubEndHeight == ANIM_STAND &&
							( (pBullet->fAimed && pBullet->iLoop > MIN_DIST_FOR_HIT_FRIENDS) ||
							(!pBullet->fAimed && pBullet->iLoop > MIN_DIST_FOR_HIT_FRIENDS_UNAIMED)
							)
							)
						{
							// could hit this person!
							gpLocalStructure[iNumLocalStructures] = pStructure;
							// CJC commented this out because of tank trying to shoot through another tank
							//guiLocalStructureCTH[iNumLocalStructures] = uiChanceOfHit;
							guiLocalStructureCTH[iNumLocalStructures] = 100;
							gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
							iNumLocalStructures++;
						}
						else
						{
							// minimal chance of hitting this person
							gpLocalStructure[iNumLocalStructures] = pStructure;
							guiLocalStructureCTH[iNumLocalStructures] = MIN_CHANCE_TO_ACCIDENTALLY_HIT_SOMEONE;
							gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
							iNumLocalStructures++;
						}
					}
				}
			}
			else if ( pStructure->fFlags & STRUCTURE_CORPSE )
			{
				if ( iGridNo == (INT32) pBullet->sTargetGridNo || (pStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles >= 10) )
				{
					// could hit this corpse!
					// but we should ignore the corpse if there is someone standing there
					if ( FindStructure( iGridNo, STRUCTURE_PERSON ) == NULL )
					{
						gpLocalStructure[iNumLocalStructures] = pStructure;
						iNumLocalStructures++;
					}
				}
			}
			else
			{
				if ( pBullet->iLoop > CLOSE_TO_FIRER && !fIntended )
				{
					// could hit it

					gpLocalStructure[iNumLocalStructures] = pStructure;
					guiLocalStructureCTH[iNumLocalStructures] = uiChanceOfHit;
					gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
					iNumLocalStructures++;
				}
			}
			pStructure = pStructure->pNext;
		}

		// record old tile location for loop purposes
		iOldTileX = pBullet->iCurrTileX;
		iOldTileY = pBullet->iCurrTileY;

		DebugLOS(String("CalcChanceToGetThrough: checking for location within the tile"));
		do
		{
			// check a particular location within the tile

			// check for collision with the ground
			iCurrAboveLevelZ = FIXEDPT_TO_INT32( pBullet->qCurrZ - qLandHeight );
			if (iCurrAboveLevelZ < 0)
			{
				// ground is in the way!
				DebugLOS(String("ChanceOfBulletHittingStructure done ground in way #2"));

				return( 0 );
			}
			// check for the existence of structures
			pStructure = pMapElement->pStructureHead;
			if (pStructure == NULL)
			{	// no structures in this tile, and THAT INCLUDES ROOFS AND PEOPLE! :-)
				// new system; figure out how many steps until we cross the next edge
				// and then fast forward that many steps.

				iOldTileX = pBullet->iCurrTileX;
				iOldTileY = pBullet->iCurrTileY;
				iOldCubesZ = pBullet->iCurrCubesZ;

				if (pBullet->qIncrX > 0)
				{
					qDistToTravelX = INT32_TO_FIXEDPT( CELL_X_SIZE ) - (pBullet->qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE ));
					iStepsToTravelX = qDistToTravelX / pBullet->qIncrX;
				}
				else if (pBullet->qIncrX < 0)
				{
					qDistToTravelX = pBullet->qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE );
					iStepsToTravelX = qDistToTravelX / (-pBullet->qIncrX);
				}
				else
				{
					// make sure we don't consider X a limit :-)
					iStepsToTravelX = 1000000;
				}

				if (pBullet->qIncrY > 0)
				{
					qDistToTravelY = INT32_TO_FIXEDPT( CELL_Y_SIZE ) - (pBullet->qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE ));
					iStepsToTravelY = qDistToTravelY / pBullet->qIncrY;
				}
				else if (pBullet->qIncrY < 0)
				{
					qDistToTravelY = pBullet->qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE );
					iStepsToTravelY = qDistToTravelY / (-pBullet->qIncrY);
				}
				else
				{
					// make sure we don't consider Y a limit :-)
					iStepsToTravelY = 1000000;
				}

				// add 1 to the # of steps to travel to go INTO the next tile
				iStepsToTravel = __min( iStepsToTravelX, iStepsToTravelY ) + 1;

				pBullet->qCurrX += pBullet->qIncrX * iStepsToTravel;
				pBullet->qCurrY += pBullet->qIncrY * iStepsToTravel;
				pBullet->qCurrZ += pBullet->qIncrZ * iStepsToTravel;
				pBullet->iLoop += iStepsToTravel;

				// check for ground collision
				if (pBullet->qCurrZ < qLandHeight && pBullet->iLoop < pBullet->iDistanceLimit)
				{
					// ground is in the way!
					DebugLOS(String("ChanceChanceToGetThrough done ground in way #3" ));
					return( 0 );
				}

				// figure out the new tile location
				pBullet->iCurrTileX = FIXEDPT_TO_TILE_NUM( pBullet->qCurrX );
				pBullet->iCurrTileY = FIXEDPT_TO_TILE_NUM( pBullet->qCurrY );
				pBullet->iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );
				pBullet->bLOSIndexX = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrX );
				pBullet->bLOSIndexY = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrY );

				DebugLOS( String( "	CTGT at %ld %ld after traversing empty tile", pBullet->bLOSIndexX, pBullet->bLOSIndexY ) );

			}
			else
			{
				// there are structures in this tile

				iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );

				// figure out the LOS cube level of the current point
				if (iCurrCubesAboveLevelZ < STRUCTURE_ON_ROOF_MAX)
				{
					if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
					{
						// check objects on the ground
						sDesiredLevel = STRUCTURE_ON_GROUND;
					}
					else
					{
						// check objects on roofs
						sDesiredLevel = STRUCTURE_ON_ROOF;
						iCurrCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
					}
					// check structures for collision
					for ( iStructureLoop = 0; iStructureLoop < iNumLocalStructures; iStructureLoop++)
					{
						pStructure = gpLocalStructure[iStructureLoop];
						if (pStructure && pStructure->sCubeOffset == sDesiredLevel)
						{
							if (((*(pStructure->pShape))[pBullet->bLOSIndexX][pBullet->bLOSIndexY] & AtHeight[iCurrCubesAboveLevelZ]) > 0)
							{
								if (pStructure->fFlags & STRUCTURE_PERSON)
								{
									// hit someone?
									if (fIntended)
									{	// gotcha! ... return chance to get through
										iChanceToGetThrough = iChanceToGetThrough * (pBullet->iImpact - pBullet->iImpactReduction) / pBullet->iImpact;
										DebugLOS(String("ChanceChanceToGetThrough done, hit intended, returning %d", iChanceToGetThrough ));
										return( (UINT8) iChanceToGetThrough );
									}
									else
									{
										gubLocalStructureNumTimesHit[iStructureLoop]++;
									}
								}
								else if (pStructure->fFlags & STRUCTURE_WALLNWINDOW && pBullet->qCurrZ >= qWindowBottomHeight && pBullet->qCurrZ <= qWindowTopHeight)
								{
									fResolveHit = ResolveHitOnWall( pStructure, iGridNo, pBullet->bLOSIndexX, pBullet->bLOSIndexY, pBullet->ddHorizAngle );

									if (fResolveHit)
									{
										// the bullet would keep on going!	unless we're considering a knife...
										if (pBullet->usFlags & BULLET_FLAG_KNIFE)
										{
											gubLocalStructureNumTimesHit[iStructureLoop]++;
										}
									}
								}
								else if (pBullet->iLoop > CLOSE_TO_FIRER || (pStructure->fFlags & ALWAYS_CONSIDER_HIT))
								{
									if (pStructure->fFlags & STRUCTURE_WALLSTUFF)
									{
										// possibly shooting at corner in which case we should let it pass
										fResolveHit = ResolveHitOnWall( pStructure, iGridNo, pBullet->bLOSIndexX, pBullet->bLOSIndexY, pBullet->ddHorizAngle );
									}
									else
									{
										fResolveHit = TRUE;
									}
									if (fResolveHit)
									{
										gubLocalStructureNumTimesHit[iStructureLoop]++;
									}
								}
							}
						}
					}
				}

				// got past everything; go to next LOS location within
				// tile, horizontally or vertically
				bOldLOSIndexX = pBullet->bLOSIndexX;
				bOldLOSIndexY = pBullet->bLOSIndexY;
				iOldCubesZ = pBullet->iCurrCubesZ;
				do
				{
					pBullet->qCurrX += pBullet->qIncrX;
					pBullet->qCurrY += pBullet->qIncrY;
					if (pRoofStructure)
					{
						qLastZ = pBullet->qCurrZ;
						pBullet->qCurrZ += pBullet->qIncrZ;
						if ( (qLastZ > qWallHeight && pBullet->qCurrZ < qWallHeight) || (qLastZ < qWallHeight && pBullet->qCurrZ > qWallHeight))
						{
							// hit roof!
							//pBullet->iImpactReduction += CTGTHandleBulletStructureInteraction( pBullet, pRoofStructure );
							//if (pBullet->iImpactReduction >= pBullet->iImpact)
							{
								DebugLOS(String("ChanceChanceToGetThrough done hit roof #2" ));
								return( 0 );
							}

						}
					}
					else
					{
						pBullet->qCurrZ += pBullet->qIncrZ;
					}
					pBullet->iLoop++;
					pBullet->bLOSIndexX = CONVERT_WITHINTILE_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrX ) % CELL_X_SIZE );
					pBullet->bLOSIndexY = CONVERT_WITHINTILE_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrY ) % CELL_Y_SIZE );
					pBullet->iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );
				}
				while( (pBullet->bLOSIndexX == bOldLOSIndexX) && (pBullet->bLOSIndexY == bOldLOSIndexY) && (pBullet->iCurrCubesZ == iOldCubesZ));

				DebugLOS( String( "	CTGT at %ld %ld %ld after moving in nonempty tile from %ld %ld %ld", pBullet->bLOSIndexX, pBullet->bLOSIndexY, pBullet->iCurrCubesZ, bOldLOSIndexX, bOldLOSIndexY, iOldCubesZ ) );
				pBullet->iCurrTileX = FIXEDPT_TO_INT32( pBullet->qCurrX ) / CELL_X_SIZE;
				pBullet->iCurrTileY = FIXEDPT_TO_INT32( pBullet->qCurrY ) / CELL_Y_SIZE;
			}
		} while( (pBullet->iLoop < pBullet->iDistanceLimit) && (pBullet->iCurrTileX == iOldTileX) && (pBullet->iCurrTileY == iOldTileY));

		if ( pBullet->iCurrTileX < 0 || pBullet->iCurrTileX >= WORLD_COLS || pBullet->iCurrTileY < 0 || pBullet->iCurrTileY >= WORLD_ROWS )
		{
			DebugLOS(String("CalcChanceToGetThrough done invalid position"));

			return( 0 );
		}

		DebugLOS(String("CalcChanceToGetThrough check for bullet drop"));
		pBullet->sGridNo = MAPROWCOLTOPOS( pBullet->iCurrTileY , pBullet->iCurrTileX );

		// HEADROCK HAM 4: Using new gravity constant from INI. See also a second change below.
		if ((UsingNewCTHSystem() == false && pBullet->iLoop > pBullet->iRange * 2) ||
			(UsingNewCTHSystem() == true && pBullet->iLoop > (INT32)(pBullet->iRange * gGameCTHConstants.RANGE_COEFFICIENT)))
		{
			// beyond max effective range, bullet starts to drop!
			// since we're doing an increment based on distance, not time, the
			// decrement is scaled down depending on how fast the bullet is (effective range)
			if ( pBullet->iRange <= 0 )
			{
				pBullet->iRange = 1;
			}


			pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * (UsingNewCTHSystem()?gGameCTHConstants.GRAVITY_COEFFICIENT:2));
		}

		// end of the tile...
		DebugLOS(String("CalcChanceToGetThrough end of tile"));
		if (iNumLocalStructures > 0)
		{
			for ( iStructureLoop = 0; iStructureLoop < iNumLocalStructures; iStructureLoop++)
			{
				// Calculate the total impact based on the number of points in the structure that were hit
				if (gubLocalStructureNumTimesHit[iStructureLoop] > 0)
				{
					iTotalStructureImpact = CTGTHandleBulletStructureInteraction( pBullet, gpLocalStructure[iStructureLoop] ) * gubLocalStructureNumTimesHit[iStructureLoop];

					// reduce the impact reduction of a structure tile to that of the bullet, since it can't do MORE than stop it.
					iTotalStructureImpact = __min( iTotalStructureImpact, pBullet->iImpact );

					// add to "impact reduction" based on strength of structure weighted by probability of hitting it
					pBullet->iImpactReduction += (iTotalStructureImpact * guiLocalStructureCTH[iStructureLoop]) / 100;
				}
			}
			if (pBullet->iImpactReduction >= pBullet->iImpact)
			{
				DebugLOS(String("CalcChanceToGetThrough done impact reduced"));
				return( 0 );
			}
		}
	} while( pBullet->iLoop < pBullet->iDistanceLimit );
	// unless the distance is integral, after the loop there will be a
	// fractional amount of distance remaining which is unchecked
	// but we shouldn't(?) need to check it because the target is there!

	DebugLOS(String("CalcChanceToGetThrough simple calc, %d * (%d - %d) / %d",
		iChanceToGetThrough, pBullet->iImpact, pBullet->iImpactReduction, (max(pBullet->iImpact,1))));
	// try simple chance to get through, ignoring range effects
	iChanceToGetThrough = iChanceToGetThrough * (pBullet->iImpact - pBullet->iImpactReduction) / (max(pBullet->iImpact,1));

	if (iChanceToGetThrough < 0)
	{
		iChanceToGetThrough = 0;
	}
	DebugLOS(String("CalcChanceToGetThrough done, returning %d", iChanceToGetThrough));

	return( (UINT8) iChanceToGetThrough );
}

UINT8 SoldierToSoldierChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier )
{
	FLOAT			dEndZPos;
	BOOLEAN		fOk;

	if (pStartSoldier == pEndSoldier)
	{
		return( 0 );
	}
	CHECKF( pStartSoldier );
	CHECKF( pEndSoldier );
	fOk = CalculateSoldierZPos( pEndSoldier, TARGET_POS, &dEndZPos );
	if (!fOk)
	{
		return( FALSE );
	}

	// set startsoldier's target ID ... need an ID stored in case this
	// is the AI calculating cover to a location where he might not be any more
	pStartSoldier->ubCTGTTargetID = pEndSoldier->ubID;
	return( ChanceToGetThrough( pStartSoldier, (FLOAT) CenterX( pEndSoldier->sGridNo ), (FLOAT) CenterY( pEndSoldier->sGridNo ), dEndZPos ) );
}

UINT8 SoldierToSoldierBodyPartChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier, UINT8 ubAimLocation )
{
	// does like StS-CTGT but with a particular body part in mind
	FLOAT			dEndZPos;
	BOOLEAN		fOk;
	UINT8			ubPosType;

	if (pStartSoldier == pEndSoldier)
	{
		return( 0 );
	}
	CHECKF( pStartSoldier );
	CHECKF( pEndSoldier );
	switch( ubAimLocation )
	{
	case AIM_SHOT_HEAD:
		ubPosType = HEAD_TARGET_POS;
		break;
	case AIM_SHOT_TORSO:
		ubPosType = TORSO_TARGET_POS;
		break;
	case AIM_SHOT_LEGS:
		ubPosType = LEGS_TARGET_POS;
		break;
	default:
		ubPosType = TARGET_POS;
		break;
	}

	fOk = CalculateSoldierZPos( pEndSoldier, ubPosType, &dEndZPos );
	if (!fOk)
	{
		return( FALSE );
	}

	// set startsoldier's target ID ... need an ID stored in case this
	// is the AI calculating cover to a location where he might not be any more
	pStartSoldier->ubCTGTTargetID = pEndSoldier->ubID;
	return( ChanceToGetThrough( pStartSoldier, (FLOAT) CenterX( pEndSoldier->sGridNo ), (FLOAT) CenterY( pEndSoldier->sGridNo ), dEndZPos ) );
}

UINT8 SoldierToLocationChanceToGetThrough( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel, UINT8 ubTargetID )
{
	FLOAT			dEndZPos;
	INT16			sXPos;
	INT16			sYPos;
	INT8			bStructHeight;
	SOLDIERTYPE * pEndSoldier;

	if (pStartSoldier->sGridNo == sGridNo)
	{
		return( 0 );
	}
	CHECKF( pStartSoldier );

	pEndSoldier = SimpleFindSoldier( sGridNo, bLevel );
	if (pEndSoldier != NULL)
	{
		return( SoldierToSoldierChanceToGetThrough( pStartSoldier, pEndSoldier ) );
	}
	else
	{
		if (bCubeLevel)
		{
			// fire at the centre of the cube specified
			dEndZPos = ( ( (FLOAT) (bCubeLevel + bLevel * PROFILE_Z_SIZE) ) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
		}
		else
		{
			bStructHeight = GetStructureTargetHeight( sGridNo, (BOOLEAN) (bLevel == 1) );
			if (bStructHeight > 0)
			{
				// fire at the centre of the cube of the tallest structure
				dEndZPos = ((FLOAT) (bStructHeight + bLevel * PROFILE_Z_SIZE) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
			}
			else
			{
				// fire at 1 unit above the level of the ground
				dEndZPos = (FLOAT) ((bLevel * PROFILE_Z_SIZE) * HEIGHT_UNITS_PER_INDEX + 1);
			}
		}

		dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[sGridNo].sHeight );
		ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );
		sXPos = sXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
		sYPos = sYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

		// set startsoldier's target ID ... need an ID stored in case this
		// is the AI calculating cover to a location where he might not be any more
		pStartSoldier->ubCTGTTargetID = ubTargetID;
		return( ChanceToGetThrough( pStartSoldier, (FLOAT) sXPos, (FLOAT) sYPos, dEndZPos ) );
	}
}

UINT8 AISoldierToSoldierChanceToGetThrough( SOLDIERTYPE * pStartSoldier, SOLDIERTYPE * pEndSoldier )
{
	// Like a standard CTGT algorithm BUT fakes the start soldier at standing height
	FLOAT			dEndZPos;
	BOOLEAN		fOk;
	UINT8			ubChance;
	UINT16		usTrueState;

	if (pStartSoldier == pEndSoldier)
	{
		return( 0 );
	}
	CHECKF( pStartSoldier );
	CHECKF( pEndSoldier );
	fOk = CalculateSoldierZPos( pEndSoldier, TARGET_POS, &dEndZPos );
	if (!fOk)
	{
		return( FALSE );
	}
	usTrueState = pStartSoldier->usAnimState;
	pStartSoldier->usAnimState = STANDING;

	// set startsoldier's target ID ... need an ID stored in case this
	// is the AI calculating cover to a location where he might not be any more
	pStartSoldier->ubCTGTTargetID = NOBODY;

	ubChance = ChanceToGetThrough( pStartSoldier, (FLOAT) CenterX( pEndSoldier->sGridNo ), (FLOAT) CenterY( pEndSoldier->sGridNo ), dEndZPos );
	pStartSoldier->usAnimState = usTrueState;
	return( ubChance );
}

UINT8 AISoldierToLocationChanceToGetThrough( SOLDIERTYPE * pStartSoldier, INT32 sGridNo, INT8 bLevel, INT8 bCubeLevel )
{
	FLOAT			dEndZPos;
	INT16			sXPos;
	INT16			sYPos;
	INT8			bStructHeight;
	SOLDIERTYPE * pEndSoldier;

	UINT16		usTrueState;
	UINT8			ubChance;

	if (pStartSoldier->sGridNo == sGridNo)
	{
		return( 0 );
	}
	CHECKF( pStartSoldier );

	pEndSoldier = SimpleFindSoldier( sGridNo, bLevel );
	if (pEndSoldier != NULL)
	{
		return( AISoldierToSoldierChanceToGetThrough( pStartSoldier, pEndSoldier ) );
	}
	else
	{
		if (bCubeLevel)
		{
			// fire at the centre of the cube specified
			dEndZPos = ( (FLOAT) (bCubeLevel + bLevel * PROFILE_Z_SIZE) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
		}
		else
		{
			bStructHeight = GetStructureTargetHeight( sGridNo, (BOOLEAN) (bLevel == 1) );
			if (bStructHeight > 0)
			{
				// fire at the centre of the cube of the tallest structure
				dEndZPos = ((FLOAT) (bStructHeight + bLevel * PROFILE_Z_SIZE) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
			}
			else
			{
				// fire at 1 unit above the level of the ground
				dEndZPos = (FLOAT) ((bLevel * PROFILE_Z_SIZE) * HEIGHT_UNITS_PER_INDEX + 1);
			}
		}

		dEndZPos += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[sGridNo].sHeight );
		ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );
		sXPos = sXPos * CELL_X_SIZE + (CELL_X_SIZE / 2);
		sYPos = sYPos * CELL_Y_SIZE + (CELL_Y_SIZE / 2);

		// set startsoldier's target ID ... need an ID stored in case this
		// is the AI calculating cover to a location where he might not be any more
		pStartSoldier->ubCTGTTargetID = NOBODY;

		usTrueState = pStartSoldier->usAnimState;
		pStartSoldier->usAnimState = STANDING;

		ubChance = ChanceToGetThrough( pStartSoldier, (FLOAT) sXPos, (FLOAT) sYPos, dEndZPos );

		pStartSoldier->usAnimState = usTrueState;

		return( ubChance );
	}
}


void CalculateFiringIncrements( DOUBLE ddHorizAngle, DOUBLE ddVerticAngle, DOUBLE dd2DDistance, BULLET * pBullet, DOUBLE * pddNewHorizAngle, DOUBLE * pddNewVerticAngle )
{
	INT32 iMissedBy = - pBullet->sHitBy;
	DOUBLE ddVerticPercentOfMiss;
	DOUBLE ddAbsVerticAngle;
	DOUBLE ddScrewupAdjustmentLimit;
	UINT32 uiChanceOfMissAbove;
	DOUBLE ddMinimumMiss;
	DOUBLE ddMaximumMiss;
	DOUBLE ddAmountOfMiss;

	if (UsingNewCTHSystem() == false && iMissedBy > 0)
	{
		ddVerticPercentOfMiss = PreRandom( 50 );

		ddAbsVerticAngle = ddVerticAngle;
		if (ddAbsVerticAngle < 0)
		{
			ddAbsVerticAngle *= -1.0;
		}

		// chance of shooting over target is 60 for horizontal shots, up to 80% for shots at 22.5 degrees,
		// and then down again to 50% for shots at 45+%.
		if (ddAbsVerticAngle < DEGREES_22_5)
		{
			uiChanceOfMissAbove = 60 + (INT32) (20 * (ddAbsVerticAngle) / DEGREES_22_5);
		}
		else if (ddAbsVerticAngle < DEGREES_45)
		{
			uiChanceOfMissAbove = 80 - (INT32) (30.0 * (ddAbsVerticAngle - DEGREES_22_5) / DEGREES_22_5);
		}
		else
		{
			uiChanceOfMissAbove = 50;
		}
		// figure out change in horizontal and vertical angle due to shooter screwup
		// the more the screwup, the greater the angle;
		// for the look of things, the further away, reduce the angle a bit.
		ddScrewupAdjustmentLimit = (dd2DDistance / CELL_X_SIZE) / 200;
		if (ddScrewupAdjustmentLimit > MAX_AIMING_SCREWUP / 2)
		{
			ddScrewupAdjustmentLimit = MAX_AIMING_SCREWUP / 2;
		}
		ddMaximumMiss = MAX_AIMING_SCREWUP - ddScrewupAdjustmentLimit;

		// Want to make sure that not too many misses actually hit the target after all
		// to miss a target at 1 tile is about 30 degrees off, at 5 tiles, 6 degrees off
		// at 15 tiles, 2 degrees off.	Thus 30 degrees divided by the # of tiles distance.
		ddMinimumMiss = MIN_AIMING_SCREWUP / (dd2DDistance / CELL_X_SIZE );

		if (ddMinimumMiss > ddMaximumMiss)
		{
			ddMinimumMiss = ddMaximumMiss;
		}

		ddAmountOfMiss = ( (ddMaximumMiss - ddMinimumMiss) * (DOUBLE) iMissedBy) / 100.0 + ddMinimumMiss;

		// miss to the left or right
		if (PreRandom( 2 ) )
		{
			ddHorizAngle += ddAmountOfMiss * (100.0 - ddVerticPercentOfMiss) / 100.0;
		}
		else
		{
			ddHorizAngle -= ddAmountOfMiss * (100.0 - ddVerticPercentOfMiss) / 100.0;
		}

		// miss up or down
		if (PreRandom( 100 ) < uiChanceOfMissAbove)
		{
			ddVerticAngle += ddAmountOfMiss * ddVerticPercentOfMiss / 100.0;
		}
		else
		{
			ddVerticAngle -= ddAmountOfMiss * ddVerticPercentOfMiss / 100.0;
		}
	}

	*pddNewHorizAngle = ddHorizAngle;
	*pddNewVerticAngle = ddVerticAngle;

	pBullet->qIncrX = FloatToFixed( (FLOAT) cos( ddHorizAngle ) );
	pBullet->qIncrY = FloatToFixed( (FLOAT) sin( ddHorizAngle ) );

	// this is the same as multiplying the X and Y increments by the projection of the line in
	// 3-space onto the horizontal plane, without reducing the X/Y increments and thus slowing
	// the LOS code
	pBullet->qIncrZ = FloatToFixed( (FLOAT) ( sin( ddVerticAngle ) / sin( (PI/2) - ddVerticAngle ) * 2.56 ) );
}

//zilpin: pellet spread patterns externalized in XML
//New firing increments calculation, without any adjustment.
void CalculateFiringIncrementsSimple( DOUBLE ddHorizAngle, DOUBLE ddVerticAngle, BULLET * pBullet)
{
	pBullet->qIncrX = FloatToFixed( (FLOAT) cos( ddHorizAngle ) );
	pBullet->qIncrY = FloatToFixed( (FLOAT) sin( ddHorizAngle ) );
	pBullet->qIncrZ = FloatToFixed( (FLOAT) ( sin( ddVerticAngle ) / sin( (PI/2) - ddVerticAngle ) * 2.56 ) );
}
INT8 FireBullet( SOLDIERTYPE * pFirer, BULLET * pBullet, BOOLEAN fFake )
{
	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBullet"));

	pBullet->iCurrTileX = FIXEDPT_TO_INT32( pBullet->qCurrX ) / CELL_X_SIZE;
	pBullet->iCurrTileY = FIXEDPT_TO_INT32( pBullet->qCurrY ) / CELL_Y_SIZE;
	pBullet->bLOSIndexX = CONVERT_WITHINTILE_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrX ) % CELL_X_SIZE );
	pBullet->bLOSIndexY = CONVERT_WITHINTILE_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrY ) % CELL_Y_SIZE );
	pBullet->iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );
	pBullet->iLoop = 1;
	pBullet->pFirer = pFirer;
	pBullet->iImpactReduction = 0;
	pBullet->sGridNo = MAPROWCOLTOPOS( pBullet->iCurrTileY, pBullet->iCurrTileX );

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBullet: bullet info set"));

	if (fFake)
	{
		pBullet->ubTargetID = pFirer->ubCTGTTargetID;
		return( CalcChanceToGetThrough( pBullet ) );
	}
	else
	{
		pBullet->ubTargetID = pFirer->ubTargetID;
		//if ( gGameSettings.fOptions[ TOPTION_HIDE_BULLETS ] )
		//{
		//	pBullet->uiLastUpdate = 0;
		//	pBullet->ubTilesPerUpdate	= 4;
		//}
		//else
		//{
		pBullet->uiLastUpdate = 0;
		pBullet->ubTilesPerUpdate	= 1;
		//}

		// increment shots fired if shooter has a merc profile
		// SANDRO - this was moved elsewhere
		//if( ( pFirer->ubProfile != NO_PROFILE ) && ( pFirer->bTeam == gbPlayerNum ) )
		//{
		//	// another shot fired
		//	/////////////////////////////////////////////////////////////////////////////////////
		//	// SANDRO - new mercs' records
		//	if ( Item[ pFirer->usAttackingWeapon ].usItemClass == IC_LAUNCHER || Item[pFirer->usAttackingWeapon].grenadelauncher || Item[pFirer->usAttackingWeapon].rocketlauncher || Item[pFirer->usAttackingWeapon].singleshotrocketlauncher || Item[pFirer->usAttackingWeapon].mortar)
		//		gMercProfiles[ pFirer->ubProfile ].records.usMissilesLaunched++;
		//	else if ( Item[ pFirer->usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE )
		//		gMercProfiles[ pFirer->ubProfile ].records.usKnivesThrown++;
		//	else 
		//		gMercProfiles[ pFirer->ubProfile ].records.usShotsFired++;
		//	
		//	/////////////////////////////////////////////////////////////////////////////////////
		//}

		if ( Item[ pFirer->usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE )
		{
			pBullet->usClockTicksPerUpdate = 30;
		}
		// HEADROCK HAM 5: Bomb Fragments
		else if (pBullet->fFragment)
		{
			pBullet->usClockTicksPerUpdate = 30;
		}
		else
		{
			//afp-start  //always a fast bullet 
			if ( pBullet->usFlags & ( BULLET_FLAG_CREATURE_SPIT | BULLET_FLAG_KNIFE | BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME /*| BULLET_FLAG_TRACER*/ ) )
				pBullet->usClockTicksPerUpdate = (Weapon[ pFirer->usAttackingWeapon ].ubBulletSpeed + GetBulletSpeedBonus(&pFirer->inv[pFirer->ubAttackingHand]) ) / 10;
			else
				if (gGameSettings.fOptions[TOPTION_ALTERNATE_BULLET_GRAPHICS])				
					pBullet->usClockTicksPerUpdate = (Weapon[ pFirer->usAttackingWeapon ].ubBulletSpeed + GetBulletSpeedBonus(&pFirer->inv[pFirer->ubAttackingHand]) ) / 10;
				else
					pBullet->usClockTicksPerUpdate = 1;
			//afp-end
		}

		//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBullet: handle flags"));
		HandleBulletSpecialFlags( pBullet->iBullet );

		//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBullet: move bullet"));
		MoveBullet( pBullet->iBullet );

		//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBullet done"));

		return( TRUE );
	}
}

/*
DOUBLE CalculateVerticalAngle( SOLDIERTYPE * pFirer, SOLDIERTYPE * pTarget )
{
DOUBLE dStartZ, dEndZ;

CalculateSoldierZPos( pFirer, FIRING_POS, &dStartZ );
CalculateSoldierZPos( pTarget, TARGET_POS, &dEndZ );

dDeltaX = (FLOAT) CenterX( pTarget->sGridNo ) - (FLOAT) CenterX( pFirer->sGridNo );
dDeltaY = (FLOAT) CenterY( pTarget->sGridNo ) - (FLOAT) CenterY( pFirer->sGridNo );
dDeltaZ = dEndZ - dStartZ;

d2DDistance = Distance2D( dDeltaX, dDeltaY );

ddOrigHorizAngle = atan2( dDeltaY, dDeltaX );
}
*/

// HEADROCK HAM 4: This formula no longer causes the bullet to deviate. That is handled by AdjustCenterPoint(). This
// formula is now only responsible for setting the bullet to go where it's told to, and create spread patterns around
// the bullet.
INT8 FireBulletGivenTargetNCTH( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usHandItem, INT16 sApertureRatio, BOOLEAN fBuckshot, BOOLEAN fFake )
{
	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"FireBulletGivenTarget");
	// fFake indicates that we should set things up for a call to ChanceToGetThrough
	FLOAT		dStartZ=0;

	FLOAT		d2DDistance=0;
	FLOAT		dDeltaX=0;
	FLOAT		dDeltaY=0;
	FLOAT		dDeltaZ=0;

	FLOAT		dStartX=0;
	FLOAT		dStartY=0;

	DOUBLE	ddOrigHorizAngle=0;
	DOUBLE	ddOrigVerticAngle=0;
	DOUBLE	ddHorizAngle=0;
	DOUBLE	ddVerticAngle=0;
	DOUBLE	ddAdjustedHorizAngle=0;
	DOUBLE	ddAdjustedVerticAngle=0;
	DOUBLE	ddDummyHorizAngle=0;
	DOUBLE	ddDummyVerticAngle=0;

	BULLET *				pBullet=NULL;
	INT32						iBullet=0;

	INT32		iDistance=0;

	UINT8		ubLoop=0;
	UINT8		ubShots=0;
	UINT8		ubImpact=0;
	INT8		bCTGT;
	UINT8		ubSpreadIndex = 0;
	UINT16	usBulletFlags = 0;
	int n=0;

	OBJECTTYPE* pObjAttHand = pFirer->GetUsedWeapon( &(pFirer->inv[pFirer->ubAttackingHand]) );

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTarget"));

	CalculateSoldierZPos( pFirer, FIRING_POS, &dStartZ );

	dStartX = (FLOAT) CenterX( pFirer->sGridNo );
	dStartY = (FLOAT) CenterY( pFirer->sGridNo );

	dDeltaX = dEndX - dStartX;
	dDeltaY = dEndY - dStartY;
	dDeltaZ = dEndZ - dStartZ;

	//lal bugfix
	if( dDeltaZ > 0 )
		d2DDistance = Distance3D( dDeltaX, dDeltaY, dDeltaZ );
	else
		d2DDistance = Distance2D( dDeltaX, dDeltaY );

	//d2DDistance = Distance2D( dDeltaX, dDeltaY );

	iDistance = (INT32) d2DDistance;

	if ( d2DDistance != iDistance )
	{
		iDistance += 1;
		d2DDistance = (FLOAT) ( iDistance);
	}

	ddOrigHorizAngle = atan2( dDeltaY, dDeltaX );
	ddOrigVerticAngle = atan2( dDeltaZ, (d2DDistance * 2.56f) );
	ddAdjustedHorizAngle = ddOrigHorizAngle;
	ddAdjustedVerticAngle = ddOrigVerticAngle;

	ubShots = 1;
	fTracer = FALSE;

	// Check if we have spit as a weapon!
	if ( Weapon[ usHandItem ].ubWeaponClass == MONSTERCLASS )
	{
		usBulletFlags |= BULLET_FLAG_CREATURE_SPIT;
	}
	else if ( Item[ usHandItem ].usItemClass == IC_THROWING_KNIFE )
	{
		usBulletFlags |= BULLET_FLAG_KNIFE;
	}
	else if (	Item[usHandItem].rocketlauncher )
	{
		usBulletFlags |= BULLET_FLAG_MISSILE;
	}
	else if ( Item[usHandItem].cannon )
	{
		usBulletFlags |= BULLET_FLAG_TANK_CANNON;
	}
	else if ( Item[usHandItem].rocketrifle )
	{
		usBulletFlags |= BULLET_FLAG_SMALL_MISSILE;
	}
	else if ( usHandItem == FLAMETHROWER )
	{
		usBulletFlags |= BULLET_FLAG_FLAME;
		ubSpreadIndex = 2;
	}
	// HEADROCK HAM B2.5: Set tracer effect on/off for individual bullets in a Tracer Magazine, as part of the
	// New Tracer System.
	else if (gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && (pFirer->bDoAutofire > 0 || pFirer->bDoBurst > 0)
		&& AmmoTypes[ (*pObjAttHand)[0]->data.gun.ubGunAmmoType ].tracerEffect )
	{
		UINT16 iBulletsLeft, iBulletsPerTracer;
		iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;
		iBulletsLeft = (*pObjAttHand)[0]->data.gun.ubGunShotsLeft + pFirer->bDoBurst;

		if ((((iBulletsLeft - (pFirer->bDoBurst - 1)) / iBulletsPerTracer) - ((iBulletsLeft - pFirer->bDoBurst) / iBulletsPerTracer)) == 1)
		{
			fTracer = TRUE;
		}
		else
		{
			fTracer = FALSE;
		}
	}
	else if ( AmmoTypes[ (*pObjAttHand)[0]->data.gun.ubGunAmmoType ].tracerEffect && (pFirer->bDoBurst || gGameSettings.fOptions[ TOPTION_TRACERS_FOR_SINGLE_FIRE ]) )
	{
		//usBulletFlags |= BULLET_FLAG_TRACER;
		fTracer = TRUE;
	}

	ubImpact =(UINT8) GetDamage(&pFirer->inv[pFirer->ubAttackingHand]);
	//zilpin: Begin new code block for spread patterns, number of projectiles, impact adjustment, etc.
	{
		ObjectData *weapon = &((*pObjAttHand)[0]->data);
		ubShots = AmmoTypes[ weapon->gun.ubGunAmmoType].numberOfBullets;
		ubSpreadIndex = GetSpreadPattern(  pObjAttHand  );
		if( ubShots>1 && !fFake )
		{
			fBuckshot = true;
			usBulletFlags |= BULLET_FLAG_BUCKSHOT;
			ubImpact = (UINT8) (ubImpact * AmmoTypes[weapon->gun.ubGunAmmoType].multipleBulletDamageMultiplier / max(1,AmmoTypes[weapon->gun.ubGunAmmoType].multipleBulletDamageDivisor) );
			if (pFirer->ubTargetID != NOBODY)
			{
				MercPtrs[ pFirer->ubTargetID ]->bNumPelletsHitBy = 0;
			}
		}
		weapon=NULL;
	}
	//zilpin: End of new code block.

	// GET BULLET
	for (ubLoop = 0; ubLoop < ubShots; ubLoop++)
	{
		iBullet = CreateBullet( pFirer->ubID, fFake, usBulletFlags,usHandItem );
		if (iBullet == -1)
		{
			//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Failed to create bullet") );

			return( FALSE );
		}
		pBullet = GetBulletPtr( iBullet );

		// HEADROCK HAM 4: The HitBy value now holds the ratio between the Distance Aperture and the Final Aperture.
		// Basically, this represents by how much our shooter has managed to make the shot more accurate than it would
		// be without any extra aiming or optical equipment. HitBy can now only be positive, and equals 100 when the
		// shot is perfectly on-target, or 0 when it is as inaccurate as the shooting system can possibly allow.
		// In this sense, sApertureRatio is the closest "relative" that the NCTH system has to the old system. it tells
		// us, on a scale of 0-100, how accurate we are compared to "Best" and "Worst" accuracy.
		// Note also that references to this value have been changed accordingly. Most imporantly, functions that alter
		// the path of the bullet, or determine whether collisions are more likely thanks to low HitBy, have been
		// altered to _not use this value at all_.
		// At the moment, HitBy is used only for determining extra damage (0-50% extra), and for determining
		// how much extra experience is gained by the shooter if the target is actually hit by the bullet.
		pBullet->sHitBy	= sApertureRatio;

		// HEADROCK HAM 4: TODO: Determine whether this value is required anymore. Why ever let bullets
		// pass through the roof at all??
		if (dStartZ < WALL_HEIGHT_UNITS)
		{
			if (dEndZ > WALL_HEIGHT_UNITS)
			{
				pBullet->fCheckForRoof = TRUE;
			}
			else
			{
				pBullet->fCheckForRoof = FALSE;
			}
		}
		else // dStartZ >= WALL_HEIGHT_UNITS; presumably >
		{
			if (dEndZ < WALL_HEIGHT_UNITS)
			{
				pBullet->fCheckForRoof = TRUE;
			}
			else
			{
				pBullet->fCheckForRoof = FALSE;
			}
		}

		//zilpin: pellet spread patterns externalized in XML
		//Now the first shot will use the pattern, as well.
		//Single shot weapons will still be stuck with straight-ahead only, though.
		//if ( ubLoop == 0 )
		if( ubShots == 1 )
		{
			// CHRISL: If we don't set the ddHorizAngle, at the very least, shooting by corners is impossible.  Unfortunately, I don't know what other
			//	impacts these two lines will have.  Headrock didn't include them when he originally wrote NCTH but they are in a similar location in the
			//	OCTH code.  Hopefully no issues will result from this change.
			ddHorizAngle = ddOrigHorizAngle;
			ddVerticAngle = ddOrigVerticAngle;
			// HEADROCK HAM 4: Firing increments no longer required here (NCTH)
			// calculate by hand (well, without angles) to match LOS
			pBullet->qIncrX = FloatToFixed( dDeltaX / (FLOAT)iDistance );
			pBullet->qIncrY = FloatToFixed( dDeltaY / (FLOAT)iDistance );
			pBullet->qIncrZ = FloatToFixed( dDeltaZ / (FLOAT)iDistance );
		}
		else if( ubShots > 1 )
		{
			//Calculate the deviation due to to-hit, but only for the first shot.
			//The bullet is only used temporarily.  It gets reset later.
			//Should this use d3DDistance?
			//if( ubLoop<1 ){
			//	CalculateFiringIncrements( ddOrigHorizAngle, ddOrigVerticAngle, d2DDistance, pBullet, &ddAdjustedHorizAngle, &ddAdjustedVerticAngle );
			//}

			// temporarily set bullet's sHitBy value to 0 to get unadjusted angles
			//Note: this no longer needs to be done since the CalculateFiringIncrementsSimple is being used below.
			//pBullet->sHitBy = 0;

			//Also prevent out-of-bounds overflows.
			//Rotates through the array if more pellets than expected, and uses pattern index 0 if requested pattern does not exist.
			if( gpSpreadPattern == NULL || giSpreadPatternCount<1 ){
				//XML file missing, or empty, or error while loading.  Use hard-coded defaults.
				//This is from the original code.
				if( ubSpreadIndex > 2 || ubSpreadIndex<0 ) ubSpreadIndex=0;
				n = ubLoop % BUCKSHOT_SHOTS;
				ddHorizAngle  = ddAdjustedHorizAngle  + ddShotgunSpread[ubSpreadIndex][n][0];
				ddVerticAngle = ddAdjustedVerticAngle + ddShotgunSpread[ubSpreadIndex][n][1];
			}else{
				double d=0,r=0,xspread=0,yspread=0;
				int n=0;

				//Use spread patterns loaded from XML.
				if( ubSpreadIndex >= giSpreadPatternCount || ubSpreadIndex < 0)
					ubSpreadIndex=0;

				xspread=gpSpreadPattern[ubSpreadIndex].xspread;
				yspread=gpSpreadPattern[ubSpreadIndex].yspread;

				//Only use randomized spread pattern if the random spread is defined AND each static angle already fired once.
				if( ubLoop >= gpSpreadPattern[ubSpreadIndex].iCount && (xspread + yspread) ){
					//Create random angle within range, positive and negative.
					switch( gpSpreadPattern[ubSpreadIndex].method )
					{
					case SPREADPATTERNMETHOD_RECT:  //Rectangle Method. (Simple)
						//Applying a new random number to each angle results in a rectangular spread pattern, rather than an oval one.
						ddHorizAngle  = (double)rand() * 2 * xspread / RAND_MAX - xspread;
						ddVerticAngle = (double)rand() * 2 * yspread / RAND_MAX - yspread;
						break;
					case SPREADPATTERNMETHOD_DIAMOND:  //Diamond Method. (Kinda Simple)
						//Angles are generated within a diamond shaped region.
						//This is more natural than the rectangular pattern, but still not optimal.
						//The first random number is in a range of 0 to (x+y).
						d = (xspread+yspread) * (double)rand() / (double)RAND_MAX;
						//The second random number determines the percentage of that value to use on x.  The rest is spent on y.
						r = (double)rand() / (double)RAND_MAX;
						ddHorizAngle = r*d;
						ddVerticAngle = (1-r)*d;
						//Positive and negative are then randomly determined.  Otherwise, everthing would always shoot to the high right (+,+).
						n = rand()%4;
						if(n&1) ddHorizAngle *= -1;
						if(n&2) ddVerticAngle *= -1;
						break;
					case SPREADPATTERNMETHOD_ELLIPSE:  //Ellipse Method.
						//Angles are generated within an ellipse.
						//This is getting close to true spread behaviour, and may be the preferred general purpose method.
						//However, due to the distribution of random numbers for r, the pattern generated tends to have
						//more pellets land along the axis, making the pattern look a bit like the Swiss cross sometimes.
						//(It only becomes noticable in simulations I do in an external program. In-game it looks fine.)
						//
						//First, get our random range of -pi to pi. This could be recalculated for every use, but doesn't need to be.
						r=(double)rand()*2*PI/RAND_MAX - PI;
						//Which axis is our major axis?
						if( xspread > yspread )
						{
							//The Ellipse.
							//  Any point on an Ellipse border line = (x,y)
							//  where x = m * cos(r) and y = n * sin(r)
							//  where m = major axis radius and n = minor axis radius and r = all values -pi <= r <= pi
							//We use any random value between -pi and pi, but vary the radius to get points inside the ellipse as well.
							ddHorizAngle  = ( (double)rand()*xspread / RAND_MAX ) * cos(r);
							ddVerticAngle = ( (double)rand()*yspread / RAND_MAX ) * sin(r);
						}else{
							//Reverse sine and cosine if our major axis is y.
							ddHorizAngle  = ( (double)rand()*xspread / RAND_MAX ) * sin(r);
							ddVerticAngle = ( (double)rand()*yspread / RAND_MAX ) * cos(r);
						}
						break;
					case -1:  //Optimal Method.  (Most realistic)
						//Not yet implemented.
						//Using the random distribution above, normal distrubution causes more pellets to end up toward the middle.
						//This is generally not noticable, and in fact feels more natural to some people (not zilpin),
						//but is not _even_ distribution.
						//In addition, since it is random, it is possible for all of the pellets to vear off center,
						//resulting in rare unnatural freak shots, which would never occur in a real shotgun.
						//This may add to gameplay, but it may bother some people (like zilpin).
						//Should anyone devise a way to adjust the ellipse distribution to account for this, here is where to put it.
						//This would probably be done by adjusting the random value of 'r' above.
						//Contact zilpin for real life shotgun spread pattern data, if you happen to be one of those mathematicians
						//who can extrapolate a succinct algorithm from raw data.  But keep in mind that there's not much data to go on.
						//
						//Also note that in the real world, shot spread tends to take the shape of a sagging funnel, rather than a cone.
						//That is to say, the path of each pellet follows a mild curve, not a straight line.
						//I'm really not going to worry about that here.  Ever.
						break;
					default:
						//If an invalid method is set in the structure, no randomized pellets are shot.
						//Note that there still may have been static angle pellets fired.
						//This should never happen, since the XML loading function will set them to giSpreadPatternMethod_Default.
						break;
					}
				}else if( gpSpreadPattern[ubSpreadIndex].iCount>0 ){
					//Use static angle, if they exist.
					n = ubLoop % gpSpreadPattern[ubSpreadIndex].iCount;
					ddHorizAngle  = gpSpreadPattern[ubSpreadIndex].x[n];
					ddVerticAngle = gpSpreadPattern[ubSpreadIndex].y[n];
				}else{
					//No angles defined, so just fire straight.
					ddHorizAngle = ddVerticAngle = 0.0;
				}


				//#ifdef JA2TESTVERSION
				DOUBLE ddRawHorizAngle = ddHorizAngle;
				DOUBLE ddRawVerticAngle = ddVerticAngle;
				//#endif

				//Adjust based on the to-hit deviation calculated when the first pellet was fired.
				ddHorizAngle  += ddAdjustedHorizAngle;
				ddVerticAngle += ddAdjustedVerticAngle;

				//Logging for debugging
				//#ifdef JA2TESTVERSION
				if(!fFake)
				{
					FILE      *OutFile;
					if ((OutFile = fopen("SpreadPatternLog.txt", "a+t")) != NULL)
					{ 
						//To easily cut-and-paste these values from the log into a C/C++ source file for later analysis
						//Lots of reference debug info in a comment.
						fprintf(OutFile, "{ % 9.8f , % 9.8f , % 9.8f , % 9.8f }, //DEBUG: merc %4d fired pellet %4d of %4d using method %4d %12s with SpreadPattern %4d %s\n",
							ddRawHorizAngle, ddRawVerticAngle,
							ddHorizAngle, ddVerticAngle,
							pFirer->ubID, ubLoop, ubShots,
							gpSpreadPattern[ubSpreadIndex].method, gSpreadPatternMethodNames[gpSpreadPattern[ubSpreadIndex].method],
							ubSpreadIndex, gpSpreadPattern[ubSpreadIndex].Name,
							NULL
						);
						fclose(OutFile);
					}
				}
				//#endif
			}

			//Just calculate the increments the bullet will use, not any of the to-hit adjustments, because we already did.
			CalculateFiringIncrementsSimple( ddHorizAngle, ddVerticAngle, pBullet );
		}


		pBullet->ddHorizAngle = ddHorizAngle;

		if (ubLoop == 0 && pFirer->bDoBurst < 2)
		{
			pBullet->fAimed = TRUE;
		}
		else
		{
			// buckshot pellets after the first can hit friendlies even at close range
			pBullet->fAimed = FALSE;
		}

		if ( pBullet->usFlags & BULLET_FLAG_KNIFE )
		{
			pBullet->ubItemStatus = (*pObjAttHand)[0]->data.objectStatus;
		}

		// apply increments for first move

		//zilpin: pellet spread patterns externalized in XML
		//This is a bugfix for strange behavior when firing, such as the projectile hitting walls behind a merc.
		//The bullet should start it's journey at the beginning, not down range.
		//Commented out the qIncr adjustments.
		pBullet->qCurrX = FloatToFixed( dStartX ) ; //+ pBullet->qIncrX;
		pBullet->qCurrY = FloatToFixed( dStartY ) ; //+ pBullet->qIncrY;
		pBullet->qCurrZ = FloatToFixed( dStartZ ) ; //+ pBullet->qIncrZ;

		//zilpin: pellet spread patterns externalized in XML
		//Also bugfix.  I understand why this was added, but it causes problems.
		/*
		// NB we can only apply correction for leftovers if the bullet is going to hit
		// because otherwise the increments are not right for the calculations!
		if ( pBullet->sHitBy >= 0 )
		{
			pBullet->qCurrX += ( FloatToFixed( dDeltaX ) - pBullet->qIncrX * iDistance ) / 2;
			pBullet->qCurrY += ( FloatToFixed( dDeltaY ) - pBullet->qIncrY * iDistance ) / 2;
			pBullet->qCurrZ += ( FloatToFixed( dDeltaZ ) - pBullet->qIncrZ * iDistance ) / 2;
		}
		*/

		pBullet->iImpact = ubImpact;

		pBullet->iRange = GunRange( pObjAttHand, pFirer ); // SANDRO - added argument
		// HEADROCK HAM 5.1: Define original point.
		pBullet->sOrigGridNo = ((INT32)dStartX) / CELL_X_SIZE + ((INT32)dStartY) / CELL_Y_SIZE * WORLD_COLS;
		pBullet->sTargetGridNo = ((INT32)dEndX) / CELL_X_SIZE + ((INT32)dEndY) / CELL_Y_SIZE * WORLD_COLS;

		pBullet->bStartCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dStartZ - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pFirer->sGridNo ].sHeight ) );
		pBullet->bEndCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dEndZ - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pBullet->sTargetGridNo ].sHeight ) );

		// this distance limit only applies in a "hard" sense to fake bullets for chance-to-get-through,
		// but is used for determining structure hits by the regular code
		pBullet->iDistanceLimit = iDistance;
		// HEADROCK HAM BETA2.5: New method for signifying whether a bullet is a tracer or not, using an individual
		// bullet structure flag. Hehehehe, I think this is kind of reverting to old code, isn't it?
		if (gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && (pFirer->bDoAutofire > 0 || pFirer->bDoBurst > 0)
			&& AmmoTypes[ (*pObjAttHand)[0]->data.gun.ubGunAmmoType ].tracerEffect )
		{
			UINT16 iBulletsLeft, iBulletsPerTracer;
			iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;
			iBulletsLeft = (*pObjAttHand)[0]->data.gun.ubGunShotsLeft + pFirer->bDoBurst;

			// Is this specific bullet a tracer? - based on how many tracers there are per regular bullets in
			// a tracer magazine (INI-settable).
			if ((((iBulletsLeft - (pFirer->bDoBurst - 1)) / iBulletsPerTracer) - ((iBulletsLeft - pFirer->bDoBurst) / iBulletsPerTracer)) == 1)
			{
				pBullet->fTracer = TRUE;
			}
			else
			{
				pBullet->fTracer = FALSE;
			}
		}
		if (fFake)
		{
			bCTGT = FireBullet( pFirer, pBullet, TRUE );
			RemoveBullet( iBullet );
			return( bCTGT );
		}
		else
		{
			//			if (ubLoop + 1 > pFirer->bBulletsLeft)
			//			{
			//				// this is an error!!
			//				ubLoop = ubLoop;
			//			}
			//hayden
			if(is_client)send_bullet( pBullet, usHandItem );
			FireBullet( pFirer, pBullet, FALSE );
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - new mercs' records
	if( !fFake && ( pFirer->ubProfile != NO_PROFILE ) && ( pFirer->bTeam == gbPlayerNum ) )
	{
		// another shot fired
		if ( Item[usHandItem].usItemClass == IC_LAUNCHER || Item[usHandItem].grenadelauncher || Item[usHandItem].rocketlauncher || Item[usHandItem].singleshotrocketlauncher || Item[usHandItem].mortar)
			gMercProfiles[ pFirer->ubProfile ].records.usMissilesLaunched++;
		else if ( Item[usHandItem].usItemClass == IC_THROWING_KNIFE )
			gMercProfiles[ pFirer->ubProfile ].records.usKnivesThrown++;
		else 
			gMercProfiles[ pFirer->ubProfile ].records.usShotsFired++;
	}
	/////////////////////////////////////////////////////////////////////////////////////

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTargetDone"));

	return( TRUE );
}

INT8 FireBulletGivenTarget( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usHandItem, INT16 sHitBy, BOOLEAN fBuckshot, BOOLEAN fFake )
{
	if(UsingNewCTHSystem() == true)
		return FireBulletGivenTargetNCTH(pFirer, dEndX, dEndY, dEndZ, usHandItem, sHitBy, fBuckshot, fFake);
	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"FireBulletGivenTarget");
	// fFake indicates that we should set things up for a call to ChanceToGetThrough
	FLOAT		dStartZ=0;

	FLOAT		d2DDistance=0;
	FLOAT		dDeltaX=0;
	FLOAT		dDeltaY=0;
	FLOAT		dDeltaZ=0;

	FLOAT		dStartX=0;
	FLOAT		dStartY=0;

	DOUBLE	ddOrigHorizAngle=0;
	DOUBLE	ddOrigVerticAngle=0;
	DOUBLE	ddHorizAngle=0;
	DOUBLE	ddVerticAngle=0;
	DOUBLE	ddAdjustedHorizAngle=0;
	DOUBLE	ddAdjustedVerticAngle=0;
	DOUBLE	ddDummyHorizAngle=0;
	DOUBLE	ddDummyVerticAngle=0;

	BULLET *				pBullet=NULL;
	INT32						iBullet=0;

	INT32		iDistance=0;

	UINT8		ubLoop=0;
	UINT8		ubShots=0;
	UINT8		ubImpact=0;
	INT8		bCTGT;
	UINT8		ubSpreadIndex = 0;
	UINT16	usBulletFlags = 0;
	int n=0;

	OBJECTTYPE* pObjAttHand = pFirer->GetUsedWeapon( &(pFirer->inv[pFirer->ubAttackingHand]) );

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTarget"));

	CalculateSoldierZPos( pFirer, FIRING_POS, &dStartZ );

	dStartX = (FLOAT) CenterX( pFirer->sGridNo );
	dStartY = (FLOAT) CenterY( pFirer->sGridNo );

	dDeltaX = dEndX - dStartX;
	dDeltaY = dEndY - dStartY;
	dDeltaZ = dEndZ - dStartZ;

	//lal bugfix
	if( dDeltaZ > 0 )
		d2DDistance = Distance3D( dDeltaX, dDeltaY, dDeltaZ );
	else
		d2DDistance = Distance2D( dDeltaX, dDeltaY );

	//d2DDistance = Distance2D( dDeltaX, dDeltaY );

	iDistance = (INT32) d2DDistance;

	if ( d2DDistance != iDistance )
	{
		iDistance += 1;
		d2DDistance = (FLOAT) ( iDistance);
	}

	ddOrigHorizAngle = atan2( dDeltaY, dDeltaX );
	ddOrigVerticAngle = atan2( dDeltaZ, (d2DDistance * 2.56f) );
	ddAdjustedHorizAngle = ddOrigHorizAngle;
	ddAdjustedVerticAngle = ddOrigVerticAngle;

	ubShots = 1;
	fTracer = FALSE;
		
	// Check if we have spit as a weapon!
	if ( Weapon[ usHandItem ].ubWeaponClass == MONSTERCLASS )
	{
		usBulletFlags |= BULLET_FLAG_CREATURE_SPIT;
	}
	else if ( Item[ usHandItem ].usItemClass == IC_THROWING_KNIFE )
	{
		usBulletFlags |= BULLET_FLAG_KNIFE;
	}
	else if (	Item[usHandItem].rocketlauncher )
	{
		usBulletFlags |= BULLET_FLAG_MISSILE;
	}
	else if ( Item[usHandItem].cannon )
	{
		usBulletFlags |= BULLET_FLAG_TANK_CANNON;
	}
	else if ( Item[usHandItem].rocketrifle )
	{
		usBulletFlags |= BULLET_FLAG_SMALL_MISSILE;
	}
	else if ( usHandItem == FLAMETHROWER )
	{
		usBulletFlags |= BULLET_FLAG_FLAME;
		ubSpreadIndex = 2;
	}
	// HEADROCK HAM B2.5: Set tracer effect on/off for individual bullets in a Tracer Magazine, as part of the
	// New Tracer System.
	else if (gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && (pFirer->bDoAutofire > 0 || pFirer->bDoBurst > 0)
		&& AmmoTypes[ (*pObjAttHand)[0]->data.gun.ubGunAmmoType ].tracerEffect )
	{
		UINT16 iBulletsLeft, iBulletsPerTracer;
		iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;
		iBulletsLeft = (*pObjAttHand)[0]->data.gun.ubGunShotsLeft + pFirer->bDoBurst;

		if ((((iBulletsLeft - (pFirer->bDoBurst - 1)) / iBulletsPerTracer) - ((iBulletsLeft - pFirer->bDoBurst) / iBulletsPerTracer)) == 1)
		{
			fTracer = TRUE;
		}
		else
		{
			fTracer = FALSE;
		}
	}
	else if ( AmmoTypes[ pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType ].tracerEffect && (pFirer->bDoBurst || gGameSettings.fOptions[ TOPTION_TRACERS_FOR_SINGLE_FIRE ]) )
	{
		//usBulletFlags |= BULLET_FLAG_TRACER;
		fTracer = TRUE;
	}

	ubImpact =(UINT8) GetDamage(pObjAttHand);
	//zilpin: pellet spread patterns externalized in XML
	/* zilpin: The section below, including line comments, is the original adjustment made to multiple projectile stats.
	           Left in comments for reference, but the new handling is after it.
	           The fBuckshot flag is now ignored, so it can probably be taken out of the function definition.
	//	if (!fFake)
	{
		if (fBuckshot)
		{
			// shotgun pellets fire 9 bullets doing 1/4 damage each
			if (!fFake)
			{
				ubShots = AmmoTypes[pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType].numberOfBullets;
				// but you can't really aim the damn things very well!
				if (sHitBy > 0)
				{
					sHitBy = sHitBy / 2;
				}
				if ( IsDuckbill( &(pFirer->inv[pFirer->ubAttackingHand])) )
				{
					ubSpreadIndex = 1;
				}
				if (pFirer->ubTargetID != NOBODY)
				{
					MercPtrs[ pFirer->ubTargetID ]->bNumPelletsHitBy = 0;
				}
				usBulletFlags |= BULLET_FLAG_BUCKSHOT;

			}
			//			ubImpact = AMMO_DAMAGE_ADJUSTMENT_BUCKSHOT( ubImpact );
			ubImpact = (UINT8) (ubImpact * AmmoTypes[pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType].multipleBulletDamageMultiplier / max(1,AmmoTypes[pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType].multipleBulletDamageDivisor) );
		}
	}
	*/
	//zilpin: Begin new code block for spread patterns, number of projectiles, impact adjustment, etc.
	{
		ObjectData *weapon = &((*pObjAttHand)[0]->data);
		ubShots = AmmoTypes[ weapon->gun.ubGunAmmoType].numberOfBullets;
		ubSpreadIndex = GetSpreadPattern(  pObjAttHand  );
		if( ubShots>1 && !fFake )
		{
			fBuckshot = true;
			usBulletFlags |= BULLET_FLAG_BUCKSHOT;
			ubImpact = (UINT8) (ubImpact * AmmoTypes[weapon->gun.ubGunAmmoType].multipleBulletDamageMultiplier / max(1,AmmoTypes[weapon->gun.ubGunAmmoType].multipleBulletDamageDivisor) );
			if (pFirer->ubTargetID != NOBODY)
			{
				MercPtrs[ pFirer->ubTargetID ]->bNumPelletsHitBy = 0;
			}
		}
		weapon=NULL;
	}
	//zilpin: End of new code block.

	// GET BULLET
	for (ubLoop = 0; ubLoop < ubShots; ubLoop++)
	{
		iBullet = CreateBullet( pFirer->ubID, fFake, usBulletFlags,usHandItem );
		if (iBullet == -1)
		{
			//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Failed to create bullet") );

			return( FALSE );
		}
		pBullet = GetBulletPtr( iBullet );
		pBullet->sHitBy	= sHitBy;

		if (dStartZ < WALL_HEIGHT_UNITS)
		{
			if (dEndZ > WALL_HEIGHT_UNITS)
			{
				pBullet->fCheckForRoof = TRUE;
			}
			else
			{
				pBullet->fCheckForRoof = FALSE;
			}
		}
		else // dStartZ >= WALL_HEIGHT_UNITS; presumably >
		{
			if (dEndZ < WALL_HEIGHT_UNITS)
			{
				pBullet->fCheckForRoof = TRUE;
			}
			else
			{
				pBullet->fCheckForRoof = FALSE;
			}
		}

		//zilpin: pellet spread patterns externalized in XML
		//Now the first shot will use the pattern, as well.
		//Single shot weapons will still be stuck with straight-ahead only, though.
		//if ( ubLoop == 0 )
		if( ubShots == 1 )
		{
			ddHorizAngle = ddOrigHorizAngle;
			ddVerticAngle = ddOrigVerticAngle;

			// first bullet, roll to hit...
			if ( sHitBy >= 0 )
			{
				// calculate by hand (well, without angles) to match LOS
				pBullet->qIncrX = FloatToFixed( dDeltaX / (FLOAT)iDistance );
				pBullet->qIncrY = FloatToFixed( dDeltaY / (FLOAT)iDistance );
				pBullet->qIncrZ = FloatToFixed( dDeltaZ / (FLOAT)iDistance );
				ddAdjustedHorizAngle = ddHorizAngle;
				ddAdjustedVerticAngle = ddVerticAngle;
			}
			else
			{
				CalculateFiringIncrements( ddHorizAngle, ddVerticAngle, d2DDistance, pBullet, &ddAdjustedHorizAngle, &ddAdjustedVerticAngle );
			}
		}
		else if( ubShots > 1 )
		{
			//Calculate the deviation due to to-hit, but only for the first shot.
			//The bullet is only used temporarily.  It gets reset later.
			//Should this use d3DDistance?
			if( ubLoop<1 ){
				CalculateFiringIncrements( ddOrigHorizAngle, ddOrigVerticAngle, d2DDistance, pBullet, &ddAdjustedHorizAngle, &ddAdjustedVerticAngle );
			}

			// temporarily set bullet's sHitBy value to 0 to get unadjusted angles
			//Note: this no longer needs to be done since the CalculateFiringIncrementsSimple is being used below.
			//pBullet->sHitBy = 0;

			//Also prevent out-of-bounds overflows.
			//Rotates through the array if more pellets than expected, and uses pattern index 0 if requested pattern does not exist.
			if( gpSpreadPattern == NULL || giSpreadPatternCount<1 ){
				//XML file missing, or empty, or error while loading.  Use hard-coded defaults.
				//This is from the original code.
				if( ubSpreadIndex > 2 || ubSpreadIndex<0 ) ubSpreadIndex=0;
				n = ubLoop % BUCKSHOT_SHOTS;
				ddHorizAngle  = ddAdjustedHorizAngle  + ddShotgunSpread[ubSpreadIndex][n][0];
				ddVerticAngle = ddAdjustedVerticAngle + ddShotgunSpread[ubSpreadIndex][n][1];
			}else{
				double d=0,r=0,xspread=0,yspread=0;
				int n=0;

				//Use spread patterns loaded from XML.
				if( ubSpreadIndex >= giSpreadPatternCount || ubSpreadIndex < 0)
					ubSpreadIndex=0;

				xspread=gpSpreadPattern[ubSpreadIndex].xspread;
				yspread=gpSpreadPattern[ubSpreadIndex].yspread;

				//Only use randomized spread pattern if the random spread is defined AND each static angle already fired once.
				if( ubLoop >= gpSpreadPattern[ubSpreadIndex].iCount && (xspread + yspread) ){
					//Create random angle within range, positive and negative.
					switch( gpSpreadPattern[ubSpreadIndex].method )
					{
					case SPREADPATTERNMETHOD_RECT:  //Rectangle Method. (Simple)
						//Applying a new random number to each angle results in a rectangular spread pattern, rather than an oval one.
						ddHorizAngle  = (double)rand() * 2 * xspread / RAND_MAX - xspread;
						ddVerticAngle = (double)rand() * 2 * yspread / RAND_MAX - yspread;
						break;
					case SPREADPATTERNMETHOD_DIAMOND:  //Diamond Method. (Kinda Simple)
						//Angles are generated within a diamond shaped region.
						//This is more natural than the rectangular pattern, but still not optimal.
						//The first random number is in a range of 0 to (x+y).
						d = (xspread+yspread) * (double)rand() / (double)RAND_MAX;
						//The second random number determines the percentage of that value to use on x.  The rest is spent on y.
						r = (double)rand() / (double)RAND_MAX;
						ddHorizAngle = r*d;
						ddVerticAngle = (1-r)*d;
						//Positive and negative are then randomly determined.  Otherwise, everthing would always shoot to the high right (+,+).
						n = rand()%4;
						if(n&1) ddHorizAngle *= -1;
						if(n&2) ddVerticAngle *= -1;
						break;
					case SPREADPATTERNMETHOD_ELLIPSE:  //Ellipse Method.
						//Angles are generated within an ellipse.
						//This is getting close to true spread behaviour, and may be the preferred general purpose method.
						//However, due to the distribution of random numbers for r, the pattern generated tends to have
						//more pellets land along the axis, making the pattern look a bit like the Swiss cross sometimes.
						//(It only becomes noticable in simulations I do in an external program. In-game it looks fine.)
						//
						//First, get our random range of -pi to pi. This could be recalculated for every use, but doesn't need to be.
						r=(double)rand()*2*PI/RAND_MAX - PI;
						//Which axis is our major axis?
						if( xspread > yspread )
						{
							//The Ellipse.
							//  Any point on an Ellipse border line = (x,y)
							//  where x = m * cos(r) and y = n * sin(r)
							//  where m = major axis radius and n = minor axis radius and r = all values -pi <= r <= pi
							//We use any random value between -pi and pi, but vary the radius to get points inside the ellipse as well.
							ddHorizAngle  = ( (double)rand()*xspread / RAND_MAX ) * cos(r);
							ddVerticAngle = ( (double)rand()*yspread / RAND_MAX ) * sin(r);
						}else{
							//Reverse sine and cosine if our major axis is y.
							ddHorizAngle  = ( (double)rand()*xspread / RAND_MAX ) * sin(r);
							ddVerticAngle = ( (double)rand()*yspread / RAND_MAX ) * cos(r);
						}
						break;
					case -1:  //Optimal Method.  (Most realistic)
						//Not yet implemented.
						//Using the random distribution above, normal distrubution causes more pellets to end up toward the middle.
						//This is generally not noticable, and in fact feels more natural to some people (not zilpin),
						//but is not _even_ distribution.
						//In addition, since it is random, it is possible for all of the pellets to vear off center,
						//resulting in rare unnatural freak shots, which would never occur in a real shotgun.
						//This may add to gameplay, but it may bother some people (like zilpin).
						//Should anyone devise a way to adjust the ellipse distribution to account for this, here is where to put it.
						//This would probably be done by adjusting the random value of 'r' above.
						//Contact zilpin for real life shotgun spread pattern data, if you happen to be one of those mathematicians
						//who can extrapolate a succinct algorithm from raw data.  But keep in mind that there's not much data to go on.
						//
						//Also note that in the real world, shot spread tends to take the shape of a sagging funnel, rather than a cone.
						//That is to say, the path of each pellet follows a mild curve, not a straight line.
						//I'm really not going to worry about that here.  Ever.
						break;
					default:
						//If an invalid method is set in the structure, no randomized pellets are shot.
						//Note that there still may have been static angle pellets fired.
						//This should never happen, since the XML loading function will set them to giSpreadPatternMethod_Default.
						break;
					}
				}else if( gpSpreadPattern[ubSpreadIndex].iCount>0 ){
					//Use static angle, if they exist.
					n = ubLoop % gpSpreadPattern[ubSpreadIndex].iCount;
					ddHorizAngle  = gpSpreadPattern[ubSpreadIndex].x[n];
					ddVerticAngle = gpSpreadPattern[ubSpreadIndex].y[n];
				}else{
					//No angles defined, so just fire straight.
					ddHorizAngle = ddVerticAngle = 0.0;
				}


				//#ifdef JA2TESTVERSION
				DOUBLE ddRawHorizAngle = ddHorizAngle;
				DOUBLE ddRawVerticAngle = ddVerticAngle;
				//#endif

				//Adjust based on the to-hit deviation calculated when the first pellet was fired.
				ddHorizAngle  += ddAdjustedHorizAngle;
				ddVerticAngle += ddAdjustedVerticAngle;

				//Logging for debugging
				//#ifdef JA2TESTVERSION
				if(!fFake)
				{
					FILE      *OutFile;
					if ((OutFile = fopen("SpreadPatternLog.txt", "a+t")) != NULL)
					{ 
						//To easily cut-and-paste these values from the log into a C/C++ source file for later analysis
						//Lots of reference debug info in a comment.
						fprintf(OutFile, "{ % 9.8f , % 9.8f , % 9.8f , % 9.8f }, //DEBUG: merc %4d fired pellet %4d of %4d using method %4d %12s with SpreadPattern %4d %s\n",
							ddRawHorizAngle, ddRawVerticAngle,
							ddHorizAngle, ddVerticAngle,
							pFirer->ubID, ubLoop, ubShots,
							gpSpreadPattern[ubSpreadIndex].method, gSpreadPatternMethodNames[gpSpreadPattern[ubSpreadIndex].method],
							ubSpreadIndex, gpSpreadPattern[ubSpreadIndex].Name,
							NULL
						);
						fclose(OutFile);
					}
				}
				//#endif
			}

			//Just calculate the increments the bullet will use, not any of the to-hit adjustments, because we already did.
			CalculateFiringIncrementsSimple( ddHorizAngle, ddVerticAngle, pBullet );
		}


		pBullet->ddHorizAngle = ddHorizAngle;

		if (ubLoop == 0 && pFirer->bDoBurst < 2)
		{
			pBullet->fAimed = TRUE;
		}
		else
		{
			// buckshot pellets after the first can hit friendlies even at close range
			pBullet->fAimed = FALSE;
		}

		if ( pBullet->usFlags & BULLET_FLAG_KNIFE )
		{
			pBullet->ubItemStatus = (*pObjAttHand)[0]->data.objectStatus;
		}

		// apply increments for first move

		//zilpin: pellet spread patterns externalized in XML
		//This is a bugfix for strange behavior when firing, such as the projectile hitting walls behind a merc.
		//The bullet should start it's journey at the beginning, not down range.
		//Commented out the qIncr adjustments.
		pBullet->qCurrX = FloatToFixed( dStartX ) ; //+ pBullet->qIncrX;
		pBullet->qCurrY = FloatToFixed( dStartY ) ; //+ pBullet->qIncrY;
		pBullet->qCurrZ = FloatToFixed( dStartZ ) ; //+ pBullet->qIncrZ;

		//zilpin: pellet spread patterns externalized in XML
		//Also bugfix.  I understand why this was added, but it causes problems.
		/*
		// NB we can only apply correction for leftovers if the bullet is going to hit
		// because otherwise the increments are not right for the calculations!
		if ( pBullet->sHitBy >= 0 )
		{
			pBullet->qCurrX += ( FloatToFixed( dDeltaX ) - pBullet->qIncrX * iDistance ) / 2;
			pBullet->qCurrY += ( FloatToFixed( dDeltaY ) - pBullet->qIncrY * iDistance ) / 2;
			pBullet->qCurrZ += ( FloatToFixed( dDeltaZ ) - pBullet->qIncrZ * iDistance ) / 2;
		}
		*/

		pBullet->iImpact = ubImpact;

		pBullet->iRange = GunRange( pObjAttHand, pFirer ); // SANDRO - added argument
		// HEADROCK HAM 5.1: Define original point.
		pBullet->sOrigGridNo = ((INT32)dStartX) / CELL_X_SIZE + ((INT32)dStartY) / CELL_Y_SIZE * WORLD_COLS;
		pBullet->sTargetGridNo = ((INT32)dEndX) / CELL_X_SIZE + ((INT32)dEndY) / CELL_Y_SIZE * WORLD_COLS;

		pBullet->bStartCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dStartZ - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pFirer->sGridNo ].sHeight ) );
		pBullet->bEndCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dEndZ - CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[ pBullet->sTargetGridNo ].sHeight ) );

		// this distance limit only applies in a "hard" sense to fake bullets for chance-to-get-through,
		// but is used for determining structure hits by the regular code
		pBullet->iDistanceLimit = iDistance;
		// HEADROCK HAM BETA2.5: New method for signifying whether a bullet is a tracer or not, using an individual
		// bullet structure flag. Hehehehe, I think this is kind of reverting to old code, isn't it?
		if (gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && (pFirer->bDoAutofire > 0 || pFirer->bDoBurst > 0)
			&& AmmoTypes[ (*pObjAttHand)[0]->data.gun.ubGunAmmoType ].tracerEffect )
		{
			UINT16 iBulletsLeft, iBulletsPerTracer;
			iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;
			iBulletsLeft = (*pObjAttHand)[0]->data.gun.ubGunShotsLeft + pFirer->bDoBurst;

			// Is this specific bullet a tracer? - based on how many tracers there are per regular bullets in
			// a tracer magazine (INI-settable).
			if ((((iBulletsLeft - (pFirer->bDoBurst - 1)) / iBulletsPerTracer) - ((iBulletsLeft - pFirer->bDoBurst) / iBulletsPerTracer)) == 1)
			{
				pBullet->fTracer = TRUE;
			}
			else
			{
				pBullet->fTracer = FALSE;
			}
		}
		if (fFake)
		{
			bCTGT = FireBullet( pFirer, pBullet, TRUE );
			RemoveBullet( iBullet );
			return( bCTGT );
		}
		else
		{
			//			if (ubLoop + 1 > pFirer->bBulletsLeft)
			//			{
			//				// this is an error!!
			//				ubLoop = ubLoop;
			//			}
			//hayden
			if(is_client)
				send_bullet( pBullet, usHandItem );

			FireBullet( pFirer, pBullet, FALSE );
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - new mercs' records
	if( !fFake && ( pFirer->ubProfile != NO_PROFILE ) && ( pFirer->bTeam == gbPlayerNum ) )
	{
		// another shot fired
		if ( Item[usHandItem].usItemClass == IC_LAUNCHER || Item[usHandItem].grenadelauncher || Item[usHandItem].rocketlauncher || Item[usHandItem].singleshotrocketlauncher || Item[usHandItem].mortar)
			gMercProfiles[ pFirer->ubProfile ].records.usMissilesLaunched++;
		else if ( Item[usHandItem].usItemClass == IC_THROWING_KNIFE )
			gMercProfiles[ pFirer->ubProfile ].records.usKnivesThrown++;
		else 
			gMercProfiles[ pFirer->ubProfile ].records.usShotsFired++;
	}
	/////////////////////////////////////////////////////////////////////////////////////

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTargetDone"));

	return( TRUE );
}

// HEADROCK HAM 5: This function is similar to FireBulletGivenTargetNCTH above, except it is used for firing fragments
// out of an explosion. The method is the same: a bullet is created and propelled towards an X/Y/Z location. The difference
// is that there is no firing weapon - the bullet needs to receive details from the explosive item, and supply the 
// thrower's pointer instead of the "shooter"'s, to ensure proper experience distribution and hostility checks go to the
// thrower.
// Note that this function does not make provisions for Fake Firing. There is no need for it, and it would needlessly
// complicate things anyway.
// Also note we receive start coordinates from the bomb itself, because there's nowhere else to get them.
INT8 FireFragmentGivenTarget( SOLDIERTYPE * pThrower, FLOAT dStartX, FLOAT dStartY, FLOAT dStartZ, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ, UINT16 usExplosiveItem )
{
	// Artificial...
	dStartZ++;

	FLOAT		d2DDistance=0;
	FLOAT		dDeltaX=0;
	FLOAT		dDeltaY=0;
	FLOAT		dDeltaZ=0;

	DOUBLE	ddOrigHorizAngle=0;
	DOUBLE	ddOrigVerticAngle=0;
	DOUBLE	ddHorizAngle=0;
	DOUBLE	ddVerticAngle=0;
	DOUBLE	ddAdjustedHorizAngle=0;
	DOUBLE	ddAdjustedVerticAngle=0;
	DOUBLE	ddDummyHorizAngle=0;
	DOUBLE	ddDummyVerticAngle=0;

	BULLET *				pBullet=NULL;
	INT32						iBullet=0;

	INT32		iDistance=0;

	UINT8		ubLoop=0;
	UINT8		ubShots=0;
	UINT8		ubImpact=0;
	UINT16	usBulletFlags = 0;
	int n=0;

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTarget"));

	dDeltaX = dEndX - dStartX;
	dDeltaY = dEndY - dStartY;
	dDeltaZ = dEndZ - dStartZ;

	//lal bugfix
	if( dDeltaZ > 0 )
		d2DDistance = Distance3D( dDeltaX, dDeltaY, dDeltaZ );
	else
		d2DDistance = Distance2D( dDeltaX, dDeltaY );

	iDistance = (INT32) d2DDistance;

	if ( d2DDistance != iDistance )
	{
		iDistance += 1;
		d2DDistance = (FLOAT) ( iDistance);
	}

	ddOrigHorizAngle = atan2( dDeltaY, dDeltaX );
	ddOrigVerticAngle = atan2( dDeltaZ, (d2DDistance * 2.56f) );
	ddAdjustedHorizAngle = ddOrigHorizAngle;
	ddAdjustedVerticAngle = ddOrigVerticAngle;

	ubShots = 1;
	fTracer = FALSE;

	// HEADROCK HAM B2.5: Set tracer effect on/off for individual bullets in a Tracer Magazine, as part of the
	// New Tracer System.

	ubImpact =(UINT8) Explosive[Item[usExplosiveItem].ubClassIndex].ubFragDamage;

	iBullet = CreateBullet( pThrower->ubID, false, usBulletFlags, usExplosiveItem );
	if (iBullet == -1)
	{
		//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Failed to create bullet") );

		return( FALSE );
	}
	pBullet = GetBulletPtr( iBullet );

	pBullet->fFragment = true;

	// Set HitBy to 1, these are completely unaimed after all.
	pBullet->sHitBy	= 100;

	// HEADROCK HAM 4: TODO: Determine whether this value is required anymore. Why ever let bullets
	// pass through the roof at all??
	if (dStartZ < WALL_HEIGHT_UNITS)
	{
		if (dEndZ > WALL_HEIGHT_UNITS)
		{
			pBullet->fCheckForRoof = TRUE;
		}
		else
		{
			pBullet->fCheckForRoof = FALSE;
		}
	}
	else // dStartZ >= WALL_HEIGHT_UNITS; presumably >
	{
		if (dEndZ < WALL_HEIGHT_UNITS)
		{
			pBullet->fCheckForRoof = TRUE;
		}
		else
		{
			pBullet->fCheckForRoof = FALSE;
		}
	}

	// CHRISL: If we don't set the ddHorizAngle, at the very least, shooting by corners is impossible.  Unfortunately, I don't know what other
	//	impacts these two lines will have.  Headrock didn't include them when he originally wrote NCTH but they are in a similar location in the
	//	OCTH code.  Hopefully no issues will result from this change.
	ddHorizAngle = ddOrigHorizAngle;
	ddVerticAngle = ddOrigVerticAngle;
	// HEADROCK HAM 4: Firing increments no longer required here (NCTH)
	// calculate by hand (well, without angles) to match LOS
	pBullet->qIncrX = FloatToFixed( dDeltaX / ((FLOAT)iDistance ));
	pBullet->qIncrY = FloatToFixed( dDeltaY / ((FLOAT)iDistance ));
	pBullet->qIncrZ = FloatToFixed( dDeltaZ / ((FLOAT)iDistance ));

	pBullet->ddHorizAngle = ddHorizAngle;

	pBullet->fAimed = FALSE;

	pBullet->qCurrX = FloatToFixed( dStartX ) ;
	pBullet->qCurrY = FloatToFixed( dStartY ) ;
	pBullet->qCurrZ = FloatToFixed( dStartZ ) ;

	pBullet->iImpact = ubImpact;

	pBullet->iRange = Explosive[Item[usExplosiveItem].ubClassIndex].ubFragRange;
	// HEADROCK HAM 5.1: Define original point.
	pBullet->sOrigGridNo = ((INT32)dStartX) / CELL_X_SIZE + ((INT32)dStartY) / CELL_Y_SIZE * WORLD_COLS;
	pBullet->sTargetGridNo = ((INT32)dEndX) / CELL_X_SIZE + ((INT32)dEndY) / CELL_Y_SIZE * WORLD_COLS;

	pBullet->bStartCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dStartZ );
	pBullet->bEndCubesAboveLevelZ = (INT8) CONVERT_HEIGHTUNITS_TO_INDEX( (INT32)dEndZ );

	// this distance limit only applies in a "hard" sense to fake bullets for chance-to-get-through,
	// but is used for determining structure hits by the regular code
	pBullet->iDistanceLimit = iDistance;
	
	pBullet->fTracer = false;

	if(is_client)send_bullet( pBullet, usExplosiveItem );
	FireBullet( pThrower, pBullet, FALSE );

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - new mercs' records
	/* Make a new record for these if you want, but right now it's disabled for fragments.
	if( !fFake && ( pFirer->ubProfile != NO_PROFILE ) && ( pFirer->bTeam == gbPlayerNum ) )
	{
		// another shot fired
		if ( Item[usHandItem].usItemClass == IC_LAUNCHER || Item[usHandItem].grenadelauncher || Item[usHandItem].rocketlauncher || Item[usHandItem].singleshotrocketlauncher || Item[usHandItem].mortar)
			gMercProfiles[ pFirer->ubProfile ].records.usMissilesLaunched++;
		else if ( Item[usHandItem].usItemClass == IC_THROWING_KNIFE )
			gMercProfiles[ pFirer->ubProfile ].records.usKnivesThrown++;
		else 
			gMercProfiles[ pFirer->ubProfile ].records.usShotsFired++;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	*/

	//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireBulletGivenTargetDone"));

	return( TRUE );
}

INT8 ChanceToGetThrough( SOLDIERTYPE * pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ )
{
	if ( Item[pFirer->usAttackingWeapon].usItemClass == IC_GUN || Item[ pFirer->usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE || Item[pFirer->usAttackingWeapon].rocketlauncher )
	{
		BOOLEAN fBuckShot = FALSE;

		OBJECTTYPE* pObjHand = pFirer->GetUsedWeapon( &pFirer->inv[HANDPOS] );

		// if shotgun, shotgun would have to be in main hand
		if ( pObjHand->usItem == pFirer->usAttackingWeapon )
		{
			if ( AmmoTypes[ (*pObjHand)[0]->data.gun.ubGunAmmoType].numberOfBullets > 1 )
			{
				fBuckShot = TRUE;
			}
		}

		// HEADROCK HAM 4: Changed argument from "hitBy"->"Aperture". Value 0->100.
		return( FireBulletGivenTarget( pFirer, dEndX, dEndY, dEndZ, pFirer->usAttackingWeapon, (UsingNewCTHSystem()?100:0), fBuckShot, TRUE ) );
	}
	else
	{
		// fake it

		//store the current item then replace it with a Glock 17
		OBJECTTYPE oldItemInHand (pFirer->inv[ HANDPOS ]);
		UINT16 oldAttackingWeapon = pFirer->usAttackingWeapon;

		//GLOCK_17_ForUseWithLOS is init in InitializeJA2()
		pFirer->inv[ HANDPOS ] = GLOCK_17_ForUseWithLOS;
		pFirer->usAttackingWeapon = GLOCK_17;

		// HEADROCK HAM 4: Changed argument from "Hitby"->"Aperture". Value 0->100.
		INT8 retVal = FireBulletGivenTarget( pFirer, dEndX, dEndY, dEndZ, GLOCK_17, (UsingNewCTHSystem()?100:0), FALSE, TRUE );

		pFirer->inv[ HANDPOS ] = oldItemInHand;
		pFirer->usAttackingWeapon = oldAttackingWeapon;
		return retVal;
	}
}

void MoveBullet( INT32 iBullet )
{
	BULLET	*pBullet;

	FIXEDPT	qLandHeight;
	INT32		iCurrAboveLevelZ;
	INT32		iCurrCubesAboveLevelZ;
	INT16		sDesiredLevel;

	INT32		iOldTileX;
	INT32		iOldTileY;
	INT32		iOldCubesZ;

	MAP_ELEMENT *		pMapElement;
	STRUCTURE *			pStructure;
	STRUCTURE *			pRoofStructure = NULL;

	FIXEDPT					qLastZ;

	SOLDIERTYPE *		pTarget;
	UINT8						ubTargetID;
	BOOLEAN					fIntended;
	BOOLEAN					fStopped;
	INT8						bOldLOSIndexX;
	INT8						bOldLOSIndexY;

	UINT32					uiTileInc = 0;
	UINT32					uiTime;

	INT8						bDir;
	INT32						iGridNo, iAdjGridNo;

	INT32						iRemainingImpact;

	FIXEDPT					qDistToTravelX;
	FIXEDPT					qDistToTravelY;
	INT32						iStepsToTravelX;
	INT32						iStepsToTravelY;
	INT32						iStepsToTravel;

	INT32						iNumLocalStructures;
	INT32						iStructureLoop;
	UINT32					uiChanceOfHit;

	BOOLEAN					fResolveHit;

	INT32						i;
	BOOLEAN					fGoingOver = FALSE;
	BOOLEAN					fHitStructure;

	FIXEDPT					qWallHeight;
	FIXEDPT					qWindowBottomHeight;
	FIXEDPT					qWindowTopHeight;

	pBullet = GetBulletPtr( iBullet );

	// CHECK MIN TIME ELAPSED
	uiTime = GetJA2Clock( );

	if ( ( uiTime - pBullet->uiLastUpdate ) < pBullet->usClockTicksPerUpdate )
	{
		return;
	}

	pBullet->uiLastUpdate = uiTime;

	do
	{
		// check a particular tile
		// retrieve values from world for this particular tile
		iGridNo = pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS;
		if (!GridNoOnVisibleWorldTile( iGridNo ) || (pBullet->iCurrCubesZ > PROFILE_Z_SIZE * 2 && FIXEDPT_TO_INT32( pBullet->qIncrZ ) > 0 ) )
		{
			// bullet outside of world!
			// NB remove bullet only flags a bullet for deletion; we still have access to the
			// information in the structure
			RemoveBullet( pBullet->iBullet );

			if(ENABLE_COLLISION)
					BulletMissed( pBullet, pBullet->pFirer );//only if local origin
			
			return;
		}

		pMapElement = &(gpWorldLevelData[ iGridNo ]);
		qLandHeight = INT32_TO_FIXEDPT( CONVERT_PIXELS_TO_HEIGHTUNITS( pMapElement->sHeight ) );
		qWallHeight = gqStandardWallHeight + qLandHeight;
		qWindowBottomHeight = gqStandardWindowBottomHeight + qLandHeight;
		qWindowTopHeight = gqStandardWindowTopHeight + qLandHeight;


		// calculate which level bullet is on for suppression and close call purposes
		// figure out the LOS cube level of the current point
		iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ - qLandHeight) );
		// figure out the level
		if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
		{
			// check objects on the ground
			sDesiredLevel = 0;
		}
		else
		{
			// check objects on roofs
			sDesiredLevel = 1;
		}

		// assemble list of structures we might hit!
		iNumLocalStructures = 0;
		pStructure = pMapElement->pStructureHead;
		// calculate chance of hitting each structure
		// HEADROCK HAM 4: This function no longer makes use of the HitBy argument, thanks to its nature having
		// been changed by the NCTH system. I've left the argument in the function however, in case we need to
		// undo that change later (I.E. find some way to increase collisions based on the new HitBy).
		uiChanceOfHit = ChanceOfBulletHittingStructure( pBullet->iLoop, pBullet->iDistanceLimit, pBullet->sHitBy );
		if (iGridNo == (INT32) pBullet->sTargetGridNo)
		{
			fIntended = TRUE;
			// if in the same tile as our destination, we WANT to hit the structure!
			if ( fIntended )
			{
				uiChanceOfHit = 100;
			}
		}
		else
		{
			fIntended = FALSE;
		}
		// HEADROCK HAM 5: Fragments have 100% Chance to Hit.
		if (pBullet->fFragment == TRUE)
		{
			fIntended = TRUE;
			uiChanceOfHit = 100;
		}
		//check for structures at target, and calc chance to hit
		//hayden, disable any structure t avoid collision
		if(ENABLE_COLLISION)
		{
		while( pStructure )
		{
			if (pStructure->fFlags & ALWAYS_CONSIDER_HIT)
			{
				// ALWAYS add walls
				// fence is special
				if ( pStructure->fFlags & STRUCTURE_ANYFENCE )
				{
					// If the density of the fence is less than 100%, or this is the top of the fence, then roll the dice
					// NB cubes are 0 based, heights 1 based
					if ( pStructure->pDBStructureRef->pDBStructure->ubDensity < 100 )
					{
						// requires roll
						if ( PreRandom( 100 ) < uiChanceOfHit )
						{
							gpLocalStructure[iNumLocalStructures] = pStructure;
							gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
							iNumLocalStructures++;
						}
					}
					else if ( (pBullet->iLoop <= CLOSE_TO_FIRER) && (iCurrCubesAboveLevelZ <= pBullet->bStartCubesAboveLevelZ) && (pBullet->bEndCubesAboveLevelZ >= iCurrCubesAboveLevelZ) && iCurrCubesAboveLevelZ == (StructureHeight( pStructure ) - 1) )
					{
						// near firer and at top of structure and at same level as bullet's start
						// requires roll
						if ( PreRandom( 100 ) < uiChanceOfHit )
						{
							gpLocalStructure[iNumLocalStructures] = pStructure;
							gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
							iNumLocalStructures++;
						}
					}
					else if ( (pBullet->iLoop + pBullet->iDistanceLimit / 50 >= pBullet->iDistanceLimit - CLOSE_TO_FIRER) && (iCurrCubesAboveLevelZ <= pBullet->bEndCubesAboveLevelZ) && iCurrCubesAboveLevelZ == (StructureHeight( pStructure ) - 1) )
					{
						// near target and at top of structure and at same level as bullet's end
						// requires roll
						if ( PreRandom( 100 ) < uiChanceOfHit )
						{
							gpLocalStructure[iNumLocalStructures] = pStructure;
							gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
							iNumLocalStructures++;
						}
					}
					else
					{
						// always add
						gpLocalStructure[iNumLocalStructures] = pStructure;
						gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
						iNumLocalStructures++;
					}

					/*
					if ( !( (pStructure->pDBStructureRef->pDBStructure->ubDensity < 100 || iCurrCubesAboveLevelZ == (StructureHeight( pStructure ) - 1) )	) && (PreRandom( 100 ) >= uiChanceOfHit) )
					{
					gpLocalStructure[iNumLocalStructures] = pStructure;
					gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
					iNumLocalStructures++;
					}
					*/
				}
				else
				{
					gpLocalStructure[iNumLocalStructures] = pStructure;
					gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
					iNumLocalStructures++;
				}
			}
			else if (pStructure->fFlags & STRUCTURE_ROOF)
			{
				// only consider roofs if the flag is set; don't add them to the array since they
				// are a special case
				if (pBullet->fCheckForRoof)
				{
					pRoofStructure = pStructure;

					qLastZ = pBullet->qCurrZ - pBullet->qIncrZ;

					// if just on going to next tile we cross boundary, then roof stops bullet here!
					if ( (qLastZ > qWallHeight && pBullet->qCurrZ <= qWallHeight) || (qLastZ < qWallHeight && pBullet->qCurrZ >= qWallHeight))
					{
						// hit a roof
						StopBullet( pBullet->iBullet );
						BulletHitStructure( pBullet, 0, 0, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
						return;
					}

				}
			}
			else if (pStructure->fFlags & STRUCTURE_PERSON)
			{
				// HEADROCK HAM 5: Fragments can hit the shooter.
				if ( MercPtrs[ pStructure->usStructureID ] != pBullet->pFirer || pBullet->fFragment == TRUE )
				{
					// in actually moving the bullet, we consider only count friends as targets if the bullet is unaimed
					// (buckshot), if they are the intended target, or beyond the range of automatic friendly fire hits
					// OR a 1 in 30 chance occurs


					// ignore *intervening* target if not visible; PCs are always visible so AI will never skip them on that
					// basis
					// HEADROCK HAM 5: Fragments should also hit whatever they encounter.
					if (fIntended || pBullet->fFragment == TRUE )
					{
						// could hit this person!
						gpLocalStructure[iNumLocalStructures] = pStructure;
						iNumLocalStructures++;
					}
					else if ( pBullet->pFirer->flags.uiStatusFlags & SOLDIER_MONSTER )
					{
						// monsters firing will always accidentally hit people but never accidentally hit each other.
						if ( !(MercPtrs[ pStructure->usStructureID ]->flags.uiStatusFlags & SOLDIER_MONSTER) )
						{
							gpLocalStructure[iNumLocalStructures] = pStructure;
							iNumLocalStructures++;
						}
					}
					else if ( MercPtrs[ pStructure->usStructureID ]->bVisible == TRUE &&
						gAnimControl[ MercPtrs[pStructure->usStructureID]->usAnimState ].ubEndHeight == ANIM_STAND &&
						( (pBullet->fAimed && pBullet->iLoop > MIN_DIST_FOR_HIT_FRIENDS) ||
						(!pBullet->fAimed && pBullet->iLoop > MIN_DIST_FOR_HIT_FRIENDS_UNAIMED) ||
						PreRandom( 100 ) < MIN_CHANCE_TO_ACCIDENTALLY_HIT_SOMEONE
						)
						)
					{
						// could hit this person!
						gpLocalStructure[iNumLocalStructures] = pStructure;
						iNumLocalStructures++;
					}

					// this might be a close call
					if ( MercPtrs[ pStructure->usStructureID ]->bTeam == gbPlayerNum && pBullet->pFirer->bTeam != gbPlayerNum && sDesiredLevel == MercPtrs[ pStructure->usStructureID ]->pathing.bLevel )
					{
						MercPtrs[ pStructure->usStructureID ]->flags.fCloseCall = TRUE;
					}

					if ( IS_MERC_BODY_TYPE( MercPtrs[pStructure->usStructureID] ) )
					{
						// HEADROCK HAM 3.3: Externalized distance at which characters suffer from friendly suppression.
						// previously relied on minimum distance at which characters may suffer from friendly fire HITS.
						UINT16 MIN_DIST_FOR_SCARE_FRIENDS = gGameExternalOptions.usMinDistanceFriendlySuppression;

						// apply suppression, regardless of friendly or enemy
						// except if friendly, not within a few tiles of shooter
						if ( MercPtrs[ pStructure->usStructureID ]->bSide != pBullet->pFirer->bSide || pBullet->iLoop > MIN_DIST_FOR_SCARE_FRIENDS )
						{
							// buckshot has only a 1 in 2 chance of applying a suppression point
							// HEADROCK HAM 5: For NCTH, make pellets as effective as any other bullet.
							BOOLEAN fInflictSuppression = FALSE;
							if (UsingNewCTHSystem() == true)
							{
								fInflictSuppression = TRUE;
							}
							else if ( !(pBullet->usFlags & BULLET_FLAG_BUCKSHOT) || Random( 2 ) )
							{
								fInflictSuppression = TRUE;
							}
							
							if (fInflictSuppression)
							{
								// bullet goes whizzing by this guy!
								switch ( gAnimControl[ MercPtrs[pStructure->usStructureID]->usAnimState ].ubEndHeight )
								{
								case ANIM_PRONE:
									// two 1/4 chances of avoiding suppression pt - one below
									if (PreRandom( 4 ) == 0)
									{
										break;
									}
									// else fall through
								case ANIM_CROUCH:
									// 1/4 chance of avoiding suppression pt
									if (PreRandom( 4 ) == 0)
									{
										break;
									}
									// else fall through
								default:
									MercPtrs[pStructure->usStructureID]->ubSuppressionPoints++;
									MercPtrs[pStructure->usStructureID]->ubSuppressorID = pBullet->pFirer->ubID;
									break;
								}
							}
						}
					}
				}

			}
			else if ( pStructure->fFlags & STRUCTURE_CORPSE )
			{
				if ( iGridNo == (INT32) pBullet->sTargetGridNo || (pStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles >= 10) )
				{
					// could hit this corpse!
					// but ignore if someone is here
					if ( FindStructure( iGridNo, STRUCTURE_PERSON ) == NULL )
					{
						gpLocalStructure[iNumLocalStructures] = pStructure;
						iNumLocalStructures++;
					}
				}
			}
			else
			{
				if (pBullet->iLoop > CLOSE_TO_FIRER || ( fIntended ) )
				{
					// calculate chance of hitting structure
					if (PreRandom( 100 ) < uiChanceOfHit)
					{
						// could hit it
						gpLocalStructure[iNumLocalStructures] = pStructure;
						gubLocalStructureNumTimesHit[iNumLocalStructures] = 0;
						iNumLocalStructures++;
					}
				}
			}
			pStructure = pStructure->pNext;
		}

		}
		// check to see if any soldiers are nearby; those soldiers
		// have their near-miss value incremented
		if (pMapElement->ubAdjacentSoldierCnt > 0)
		{
			// cube level now calculated above!
			// figure out the LOS cube level of the current point
			//iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ - qLandHeight) );
			// figure out what level to affect...
			if (iCurrCubesAboveLevelZ < STRUCTURE_ON_ROOF_MAX)
			{
				/*
				if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
				{
				// check objects on the ground
				sDesiredLevel = 0;
				}
				else
				{
				// check objects on roofs
				sDesiredLevel = 1;
				}
				*/

				for( bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++)
				{
					iAdjGridNo = iGridNo + DirIncrementer[bDir];

					if ( gubWorldMovementCosts[ iAdjGridNo ][ bDir ][ sDesiredLevel ] < TRAVELCOST_BLOCKED)
					{
						ubTargetID = WhoIsThere2( iAdjGridNo, (INT8) sDesiredLevel );
						if (ubTargetID != NOBODY)
						{
							pTarget = MercPtrs[ ubTargetID ];
							if ( IS_MERC_BODY_TYPE( pTarget ) && pBullet->pFirer->bSide != pTarget->bSide )
							{
								// HEADROCK HAM 5: For NCTH, make pellets as effective as any other bullet.
								BOOLEAN fInflictSuppression = FALSE;
								if (UsingNewCTHSystem() == true)
								{
									fInflictSuppression = TRUE;
								}
								else if ( !(pBullet->usFlags & BULLET_FLAG_BUCKSHOT) || Random( 2 ) )
								{
									fInflictSuppression = TRUE;
								}
								
								if (fInflictSuppression)
								{
									// bullet goes whizzing by this guy!
									switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
									{
									case ANIM_PRONE:
										// two 1/4 chances of avoiding suppression pt - one below
										if (PreRandom( 4 ) == 0)
										{
											break;
										}
										// else fall through
									case ANIM_CROUCH:
										// 1/4 chance of avoiding suppression pt
										if (PreRandom( 4 ) == 0)
										{
											break;
										}
										// else fall through
									default:
										pTarget->ubSuppressionPoints++;
										pTarget->ubSuppressorID = pBullet->pFirer->ubID;
										break;
									}
								}

								/*
								// this could be a close call
								if ( pTarget->bTeam == gbPlayerNum && pBullet->pFirer->bTeam != gbPlayerNum )
								{
								pTarget->flags.fCloseCall = TRUE;
								}
								*/

							}
						}
					}
				}
			}
		}

		// record old tile location for loop purposes
		iOldTileX = pBullet->iCurrTileX;
		iOldTileY = pBullet->iCurrTileY;

		do
		{
			// check a particular location within the tile

			// check for collision with the ground
			iCurrAboveLevelZ = FIXEDPT_TO_INT32( pBullet->qCurrZ - qLandHeight );
//hayden	not hit	ground
			if(ENABLE_COLLISION)	
			{
			if (iCurrAboveLevelZ < 0)
			{
				// ground is in the way!
				StopBullet( pBullet->iBullet );
				BulletHitStructure( pBullet, INVALID_STRUCTURE_ID, 0, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
				return;
			}
			}
			// check for the existence of structures
			if (iNumLocalStructures == 0 && !pRoofStructure)
			{	// no structures in this tile, AND THAT INCLUDES ROOFS! :-)
				// new system; figure out how many steps until we cross the next edge
				// and then fast forward that many steps.

				iOldTileX = pBullet->iCurrTileX;
				iOldTileY = pBullet->iCurrTileY;
				iOldCubesZ = pBullet->iCurrCubesZ;

				if (pBullet->qIncrX > 0)
				{
					qDistToTravelX = INT32_TO_FIXEDPT( CELL_X_SIZE ) - (pBullet->qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE ));
					iStepsToTravelX = qDistToTravelX / pBullet->qIncrX;
				}
				else if (pBullet->qIncrX < 0)
				{
					qDistToTravelX = pBullet->qCurrX % INT32_TO_FIXEDPT( CELL_X_SIZE );
					iStepsToTravelX = qDistToTravelX / (-pBullet->qIncrX);
				}
				else
				{
					// make sure we don't consider X a limit :-)
					iStepsToTravelX = 1000000;
				}

				if (pBullet->qIncrY > 0)
				{
					qDistToTravelY = INT32_TO_FIXEDPT( CELL_Y_SIZE ) - (pBullet->qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE ));
					iStepsToTravelY = qDistToTravelY / pBullet->qIncrY;
				}
				else if (pBullet->qIncrY < 0)
				{
					qDistToTravelY = pBullet->qCurrY % INT32_TO_FIXEDPT( CELL_Y_SIZE );
					iStepsToTravelY = qDistToTravelY / (-pBullet->qIncrY);
				}
				else
				{
					// make sure we don't consider Y a limit :-)
					iStepsToTravelY = 1000000;
				}

				// add 1 to the # of steps to travel to go INTO the next tile
				iStepsToTravel = __min( iStepsToTravelX, iStepsToTravelY ) + 1;

				// special coding (compared with other versions above) to deal with
				// bullets hitting the ground
//hayden, stop hit ground
				if(ENABLE_COLLISION)
				{
				if (pBullet->qCurrZ + pBullet->qIncrZ * iStepsToTravel < qLandHeight)
				{
					// WANNE - BMP: BUGFIX: Had to change abs to _abs64 because FIXEDPT is now INT64
					//iStepsToTravel = __min( iStepsToTravel, abs( (pBullet->qCurrZ - qLandHeight) / pBullet->qIncrZ ) );
					iStepsToTravel = __min( iStepsToTravel, _abs64( (pBullet->qCurrZ - qLandHeight) / pBullet->qIncrZ ) );

					pBullet->qCurrX += pBullet->qIncrX * iStepsToTravel;
					pBullet->qCurrY += pBullet->qIncrY * iStepsToTravel;
					pBullet->qCurrZ += pBullet->qIncrZ * iStepsToTravel;

					StopBullet( pBullet->iBullet );
					BulletHitStructure( pBullet, INVALID_STRUCTURE_ID, 0, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
					return;
				}
				}

				// HEADROCK HAM B2.5: Changed condition to read fTracer flag directly from bullet's struct.
				// This is for the New Tracer System.
				if (( pBullet->usFlags & ( BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME | BULLET_FLAG_CREATURE_SPIT /*| BULLET_FLAG_TRACER*/	) ) 
					|| ((gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && pBullet->fTracer == TRUE) || (gGameExternalOptions.ubRealisticTracers == 0 && fTracer == TRUE)))
				{
					INT8 bStepsPerMove = STEPS_FOR_BULLET_MOVE_TRAILS;

					if ( pBullet->usFlags & ( BULLET_FLAG_SMALL_MISSILE ) )
					{
						bStepsPerMove = STEPS_FOR_BULLET_MOVE_SMALL_TRAILS;
					}
					else if ( pBullet->usFlags & ( BULLET_FLAG_FLAME ) )
					{
						bStepsPerMove = STEPS_FOR_BULLET_MOVE_FIRE_TRAILS;
					}

					for ( i = 0; i < iStepsToTravel; i++ )
					{
						if ( ( ( pBullet->iLoop + i ) % bStepsPerMove ) == 0 )
						{
							fGoingOver = TRUE;
							break;
						}
					}

					if ( fGoingOver )
					{
						FIXEDPT	qCurrX, qCurrY, qCurrZ;

						qCurrX = pBullet->qCurrX + pBullet->qIncrX * i;
						qCurrY = pBullet->qCurrY + pBullet->qIncrY * i;
						qCurrZ = pBullet->qCurrZ + pBullet->qIncrZ * i;

						AddMissileTrail( pBullet, qCurrX, qCurrY, qCurrZ );
					}
				}

				pBullet->qCurrX += pBullet->qIncrX * iStepsToTravel;
				pBullet->qCurrY += pBullet->qIncrY * iStepsToTravel;
				pBullet->qCurrZ += pBullet->qIncrZ * iStepsToTravel;
				pBullet->iLoop += iStepsToTravel;


				// figure out the new tile location
				pBullet->iCurrTileX = FIXEDPT_TO_TILE_NUM( pBullet->qCurrX );
				pBullet->iCurrTileY = FIXEDPT_TO_TILE_NUM( pBullet->qCurrY );
				pBullet->iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );
				pBullet->bLOSIndexX = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrX );
				pBullet->bLOSIndexY = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrY );
			}
			else
			{
				// there are structures in this tile
				iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );
				// figure out the LOS cube level of the current point

				if (iCurrCubesAboveLevelZ < STRUCTURE_ON_ROOF_MAX)
				{
					if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
					{
						// check objects on the ground
						sDesiredLevel = STRUCTURE_ON_GROUND;
					}
					else
					{
						// check objects on roofs
						sDesiredLevel = STRUCTURE_ON_ROOF;
						iCurrCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
					}
					// check structures for collision
					for ( iStructureLoop = 0; iStructureLoop < iNumLocalStructures; iStructureLoop++)
					{
						pStructure = gpLocalStructure[iStructureLoop];
						if (pStructure && pStructure->sCubeOffset == sDesiredLevel)
						{
							if (((*(pStructure->pShape))[pBullet->bLOSIndexX][pBullet->bLOSIndexY] & AtHeight[iCurrCubesAboveLevelZ]) > 0)
							{
								if (pStructure->fFlags & STRUCTURE_PERSON)
								{
									// hit someone!
									fStopped = BulletHitMerc( pBullet, pStructure, fIntended );
									if (fStopped)
									{
										// remove bullet function now called from within BulletHitMerc, so just quit
										return;
									}
									else
									{
										// set pointer to null so that we don't consider hitting this person again
										gpLocalStructure[iStructureLoop] = NULL;
									}
								}
								else if (pStructure->fFlags & STRUCTURE_WALLNWINDOW && pBullet->qCurrZ >= qWindowBottomHeight && pBullet->qCurrZ <= qWindowTopHeight)
								{
									fResolveHit = ResolveHitOnWall( pStructure, iGridNo, pBullet->bLOSIndexX, pBullet->bLOSIndexY, pBullet->ddHorizAngle );

									if (fResolveHit)
									{

										if (pBullet->usFlags & BULLET_FLAG_KNIFE)
										{
											// knives do get stopped by windows!

											iRemainingImpact = HandleBulletStructureInteraction( pBullet, pStructure, &fHitStructure );
											if ( iRemainingImpact <= 0 )
											{
												// check angle of knife and place on ground appropriately
												OBJECTTYPE Object;
												INT32	iKnifeGridNo;

												CreateItem( pBullet->fromItem, (INT8) pBullet->ubItemStatus, &Object );

												// by default knife at same tile as window
												iKnifeGridNo = iGridNo;

												if (pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT)
												{
													if ( pBullet->qIncrX > 0)
													{
														// heading east so place knife on west, in same tile
													}
													else
													{
														// place to east of window
														iKnifeGridNo += 1;
													}
												}
												else
												{
													if (pBullet->qIncrY > 0)
													{
														// heading south so place wall to north, in same tile of window
													}
													else
													{
														iKnifeGridNo += WORLD_ROWS;
													}
												}

												if ( sDesiredLevel == STRUCTURE_ON_GROUND )
												{
													AddItemToPool( iKnifeGridNo, &Object, -1, 0, 0, 0 );
												}
												else
												{
													AddItemToPool( iKnifeGridNo, &Object, -1, 0, 1, 0 );
												}

												// Make team look for items
												NotifySoldiersToLookforItems( );

												// bullet must end here!
												StopBullet( pBullet->iBullet );
												BulletHitStructure( pBullet, pStructure->usStructureID, 1, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
												return;
											}
										}
										else
										{
											if (pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT)
											{
												if ( pBullet->qIncrX > 0)
												{
													BulletHitWindow( pBullet, (pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS), pStructure->usStructureID, TRUE );
													LocateBullet( pBullet->iBullet );
													// have to remove this window from future hit considerations so the deleted structure data can't be referenced!
													gpLocalStructure[ iStructureLoop ] = NULL;
												}
												else
												{
													BulletHitWindow( pBullet, (pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS), pStructure->usStructureID, FALSE );
													LocateBullet( pBullet->iBullet );
													gpLocalStructure[ iStructureLoop ] = NULL;
												}
											}
											else
											{
												if (pBullet->qIncrY > 0)
												{
													BulletHitWindow( pBullet, (pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS), pStructure->usStructureID, TRUE );
													LocateBullet( pBullet->iBullet );
													gpLocalStructure[ iStructureLoop ] = NULL;
												}
												else
												{
													BulletHitWindow( pBullet, (pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS), pStructure->usStructureID, FALSE );
													LocateBullet( pBullet->iBullet );
													gpLocalStructure[ iStructureLoop ] = NULL;
												}
											}
											// but the bullet keeps on going!!!
										}

									}
								}
								else if ( pBullet->iLoop > CLOSE_TO_FIRER || (pStructure->fFlags & ALWAYS_CONSIDER_HIT) || (pBullet->iLoop > CLOSE_TO_FIRER) || (fIntended) )
								{
									if (pStructure->fFlags & STRUCTURE_WALLSTUFF)
									{
										// possibly shooting at corner in which case we should let it pass
										fResolveHit = ResolveHitOnWall( pStructure, iGridNo, pBullet->bLOSIndexX, pBullet->bLOSIndexY, pBullet->ddHorizAngle );
									}
									else
									{
										fResolveHit = TRUE;
									}

									if (fResolveHit)
									{

										iRemainingImpact = HandleBulletStructureInteraction( pBullet, pStructure, &fHitStructure );
										if (fHitStructure)
										{
											// ATE: NOT if we are a special bullet like a LAW trail...
											if (pStructure->fFlags & STRUCTURE_CORPSE && !( pBullet->usFlags & ( BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME | BULLET_FLAG_CREATURE_SPIT ) ) )
											{
												// ATE: In enemy territory here... ;)
												// Now that we have hit a corpse, make the bugger twich!

												CorpseHit( pBullet->sGridNo, pStructure->usStructureID );
												DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., CORPSE HIT") );

												// Moved here to keep ABC >0 as long as possible
												RemoveBullet( iBullet );
												// ReduceAttackBusyCount( );
												return;
											}
											else if ( iRemainingImpact <= 0 )
											{
												StopBullet( pBullet->iBullet );
												BulletHitStructure( pBullet, pStructure->usStructureID, 1, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
												return;
											}
											else if (fHitStructure && (gubLocalStructureNumTimesHit[iStructureLoop] == 0) )
											{
												// play animation to indicate structure being hit
												BulletHitStructure( pBullet, pStructure->usStructureID, 1, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, FALSE );
												gubLocalStructureNumTimesHit[iStructureLoop] = 1;
											}
										}
									}
								}
							}
						}
					}
				}
				// got past everything; go to next LOS location within
				// tile, horizontally or vertically
				bOldLOSIndexX = pBullet->bLOSIndexX;
				bOldLOSIndexY = pBullet->bLOSIndexY;
				iOldCubesZ = pBullet->iCurrCubesZ;
				do
				{
					pBullet->qCurrX += pBullet->qIncrX;
					pBullet->qCurrY += pBullet->qIncrY;
//no hit on lan//pRoofStructure//hayden
					if (is_networked && pRoofStructure)
					{
						qLastZ = pBullet->qCurrZ;
						pBullet->qCurrZ += pBullet->qIncrZ;
						if ( (qLastZ > qWallHeight && pBullet->qCurrZ <= qWallHeight) || (qLastZ < qWallHeight && pBullet->qCurrZ >= qWallHeight))
						{
							// generate roof-hitting event
							// always stop with roofs

							if ( 1 /*HandleBulletStructureInteraction( pBullet, pRoofStructure, &fHitStructure ) <= 0 */)
							{
								StopBullet( pBullet->iBullet );
								BulletHitStructure( pBullet, 0, 0, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ, TRUE );
								return;
							}
							/*
							else
							{
							// ATE: Found this: Should we be calling this because if we do, it will
							// delete a bullet that was not supposed to be deleted....
							//BulletHitStructure( pBullet, 0, 0, pBullet->pFirer, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ );
							}
							*/
						}
					}
					else
					{
						pBullet->qCurrZ += pBullet->qIncrZ;
					}
					pBullet->bLOSIndexX = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrX );
					pBullet->bLOSIndexY = FIXEDPT_TO_LOS_INDEX( pBullet->qCurrY );
					pBullet->iCurrCubesZ = CONVERT_HEIGHTUNITS_TO_INDEX( FIXEDPT_TO_INT32( pBullet->qCurrZ ) );
					pBullet->iLoop++;

					// HEADROCK HAM B2.5: Changed condition to read fTracer flag directly from bullet's struct.
					// This is for the New Tracer System.
					if (( pBullet->usFlags & ( BULLET_FLAG_MISSILE | BULLET_FLAG_SMALL_MISSILE | BULLET_FLAG_TANK_CANNON | BULLET_FLAG_FLAME | BULLET_FLAG_CREATURE_SPIT /*| BULLET_FLAG_TRACER */) ) 
						|| ((gGameExternalOptions.ubRealisticTracers > 0 && gGameExternalOptions.ubNumBulletsPerTracer > 0 && pBullet->fTracer == TRUE) || (gGameExternalOptions.ubRealisticTracers == 0 && fTracer == TRUE)))
					{
						INT8 bStepsPerMove = STEPS_FOR_BULLET_MOVE_TRAILS;

						if (( pBullet->usFlags & ( BULLET_FLAG_SMALL_MISSILE /*| BULLET_FLAG_TRACER*/ ) ) || fTracer == TRUE)
						{
							bStepsPerMove = STEPS_FOR_BULLET_MOVE_SMALL_TRAILS;
						}
						else if ( pBullet->usFlags & ( BULLET_FLAG_FLAME ) )
						{
							bStepsPerMove = STEPS_FOR_BULLET_MOVE_FIRE_TRAILS;
						}

						if ( pBullet->iLoop % bStepsPerMove == 0 )
						{
							// add smoke trail
							AddMissileTrail( pBullet, pBullet->qCurrX, pBullet->qCurrY, pBullet->qCurrZ );
						}
					}

				}
				while( (pBullet->bLOSIndexX == bOldLOSIndexX) && (pBullet->bLOSIndexY == bOldLOSIndexY) && (pBullet->iCurrCubesZ == iOldCubesZ));
				pBullet->iCurrTileX = FIXEDPT_TO_INT32( pBullet->qCurrX ) / CELL_X_SIZE;
				pBullet->iCurrTileY = FIXEDPT_TO_INT32( pBullet->qCurrY ) / CELL_Y_SIZE;
			}
		} while( (pBullet->iCurrTileX == iOldTileX) && (pBullet->iCurrTileY == iOldTileY));

		if ( !GridNoOnVisibleWorldTile( (pBullet->iCurrTileX + pBullet->iCurrTileY * WORLD_COLS) ) || (pBullet->iCurrCubesZ > PROFILE_Z_SIZE * 2 && FIXEDPT_TO_INT32( pBullet->qIncrZ ) > 0 ) )
		{
			// bullet outside of world!
			RemoveBullet( pBullet->iBullet );
			if(ENABLE_COLLISION)BulletMissed( pBullet, pBullet->pFirer );//hayden: only play event if local
			return;
		}

		pBullet->sGridNo = MAPROWCOLTOPOS( pBullet->iCurrTileY , pBullet->iCurrTileX );
		uiTileInc++;

		// HEADROCK HAM 5: Ignore if moving a fragment.
		if(UsingNewCTHSystem() && pBullet->fFragment == false ){
			// HEADROCK HAM 4: This is kind of a hack. I'm measuring the distance the tile has moved in 2D space,
			// for purposes of determining whether gravity should take effect.
			FLOAT dDistanceMoved = PythSpacesAway( pBullet->pFirer->sGridNo, pBullet->sGridNo ) * 10.0f;

			// HEADROCK HAM 4: Now using an INI=set Gravity Constant.
			if ( (dDistanceMoved > (FLOAT)pBullet->iRange*gGameCTHConstants.RANGE_COEFFICIENT) )
			{
				// beyond max effective range, bullet starts to drop!
				// since we're doing an increment based on distance, not time, the
				// decrement is scaled down depending on how fast the bullet is (effective range)
				//pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * 2);
				pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * gGameCTHConstants.GRAVITY_COEFFICIENT); 
			}
			else if ( (pBullet->usFlags & BULLET_FLAG_FLAME) && (dDistanceMoved > pBullet->iRange) )
			{
				//pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * 2);
				pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * (gGameCTHConstants.GRAVITY_COEFFICIENT / 2) );
			}
		} else {
			if ( (pBullet->iLoop > pBullet->iRange * 2) )
			{
				// beyond max effective range, bullet starts to drop!
				// since we're doing an increment based on distance, not time, the
				// decrement is scaled down depending on how fast the bullet is (effective range)
				pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * 2);
			}
			else if ( (pBullet->usFlags & BULLET_FLAG_FLAME) && ( pBullet->iLoop > pBullet->iRange ) )
			{
				pBullet->qIncrZ -= INT32_TO_FIXEDPT( 100 ) / (pBullet->iRange * 2);
			}
		}

		// check to see if bullet is close to target
		if ( pBullet->pFirer->ubTargetID != NOBODY && !(pBullet->pFirer->flags.uiStatusFlags & SOLDIER_ATTACK_NOTICED) && PythSpacesAway( pBullet->sGridNo, pBullet->sTargetGridNo ) <= 3 )
		{
			pBullet->pFirer->flags.uiStatusFlags |= SOLDIER_ATTACK_NOTICED;
		}
	} while( uiTileInc < pBullet->ubTilesPerUpdate );
	// unless the distance is integral, after the loop there will be a
	// fractional amount of distance remaining which is unchecked
	// but we shouldn't(?) need to check it because the target is there!

}




INT32	CheckForCollision( FLOAT dX, FLOAT dY, FLOAT dZ, FLOAT dDeltaX, FLOAT dDeltaY, FLOAT dDeltaZ, INT16 *pusStructureID, FLOAT *pdNormalX, FLOAT *pdNormalY, FLOAT *pdNormalZ )
{

	INT32		iLandHeight;
	INT32		iCurrAboveLevelZ;
	INT32		iCurrCubesAboveLevelZ;
	INT16		sDesiredLevel;

	MAP_ELEMENT *		pMapElement;
	STRUCTURE *			pStructure, *pTempStructure;

	SOLDIERTYPE *		pTarget;
	FLOAT						dTargetX;
	FLOAT						dTargetY;
	FLOAT						dTargetZMin;
	FLOAT						dTargetZMax;
	BOOLEAN					fIntended;

	INT32						sX, sY, sZ;

	FLOAT						dOldZUnits, dZUnits;

	INT8						bLOSIndexX, bLOSIndexY;
	INT32						iCurrCubesZ;


	sX = (INT32)( dX / CELL_X_SIZE );
	sY = (INT32)( dY / CELL_Y_SIZE );
	sZ = (INT32)dZ;

	// Check if gridno is in bounds....
	if ( !GridNoOnVisibleWorldTile( sX + sY * WORLD_COLS ) )
	{
		//	return( COLLISION_NONE );
	}

	if ( sX < 0 || sX > WORLD_COLS || sY < 0 || sY > WORLD_COLS )
	{
		//		return( COLLISION_NONE );
	}

	// check a particular tile
	// retrieve values from world for this particular tile
	pMapElement = &(gpWorldLevelData[ sX + sY * WORLD_COLS] );
	iLandHeight = CONVERT_PIXELS_TO_HEIGHTUNITS( pMapElement->sHeight );

	// Calculate old height and new hieght in pixels
	dOldZUnits = (dZ - dDeltaZ );
	dZUnits	=		dZ;

	//if (pBullet->fCheckForRoof)
	//{
	//	if (pMapElement->pRoofHead != NULL)
	//	{
	//		fRoofPresent = TRUE;
	//	}
	//	else
	//	{
	//		fRoofPresent = FALSE;
	//	}
	//}

	//if (pMapElement->pMercHead != NULL && pBullet->iLoop != 1)
	if (pMapElement->pMercHead != NULL )
	{ // a merc! that isn't us :-)
		pTarget = pMapElement->pMercHead->pSoldier;
		dTargetX = pTarget->dXPos;
		dTargetY = pTarget->dYPos;
		dTargetZMin = 0.0f;
		CalculateSoldierZPos( pTarget, HEIGHT, &dTargetZMax );
		if (pTarget->pathing.bLevel > 0)
		{
			// on roof
			dTargetZMin += WALL_HEIGHT_UNITS;
		}
		if ( sX + sY * WORLD_COLS == pTarget->sGridNo)
		{
			fIntended = TRUE;
		}
		else
		{
			fIntended = FALSE;
		}
	}
	else
	{
		pTarget = NULL;
	}

	// record old tile location for loop purposes

	// check for collision with the ground
	iCurrAboveLevelZ = (INT32) dZ - iLandHeight;
	if (iCurrAboveLevelZ < 0)
	{
		// ground is in the way!
		if ( TERRAIN_IS_WATER( pMapElement->ubTerrainID) )
		{
			return ( COLLISION_WATER );
		}
		else
		{
			return ( COLLISION_GROUND );
		}
	}
	// check for the existence of structures
	pStructure = pMapElement->pStructureHead;
	if (pStructure == NULL)
	{	// no structures in this tile

		// we can go as far as we like vertically (so long as we don't hit
		// the ground), but want to stop when we get to the next tile or
		// the end of the LOS path

		// move 1 unit along the bullet path
		//if (fRoofPresent)
		//{
		//	dLastZ = pBullet->dCurrZ;
		//	(pBullet->dCurrZ) += pBullet->dIncrZ;
		//	if ( (dLastZ > WALL_HEIGHT && pBullet->dCurrZ < WALL_HEIGHT) || (dLastZ < WALL_HEIGHT && pBullet->dCurrZ > WALL_HEIGHT))
		//	{
		//		// generate roof-hitting event
		//		BulletHitStructure( pBullet->pFirer, pBullet->dCurrX, pBullet->dCurrY, pBullet->dCurrZ );
		//		RemoveBullet( pBullet->iBullet );
		//		return;
		//	}
		//}
		//else
		//{
		//	(pBullet->dCurrZ) += pBullet->dIncrZ;
		//}

		// check for ground collision
		if ( dZ < iLandHeight)
		{
			// ground is in the way!
			if ( TERRAIN_IS_WATER( pMapElement->ubTerrainID) )
			{
				return ( COLLISION_WATER );
			}
			else
			{
				return ( COLLISION_GROUND );
			}
		}

		if ( gfCaves || gfBasement )
		{
			if ( dOldZUnits > HEIGHT_UNITS && dZUnits	< HEIGHT_UNITS )
			{
				return( COLLISION_ROOF );
			}
			if ( dOldZUnits < HEIGHT_UNITS && dZUnits	> HEIGHT_UNITS )
			{
				return( COLLISION_INTERIOR_ROOF );
			}
		}

		// check to see if we hit someone
		//if (pTarget && Distance2D( dX - dTargetX, dY - dTargetY ) < HIT_DISTANCE )
		//{
		// well, we're in the right area; it's possible that
		// we're firing over or under them though
		//	if ( dZ < dTargetZMax && dZ > dTargetZMin)
		//	{
		//		return( COLLISION_MERC );
		//	}
		//}

	}
	else
	{
		// there are structures in this tile
		iCurrCubesAboveLevelZ = CONVERT_HEIGHTUNITS_TO_INDEX( iCurrAboveLevelZ );
		// figure out the LOS cube level of the current point

		// CALCULAT LOS INDEX
		bLOSIndexX = CONVERT_WITHINTILE_TO_INDEX( ((INT32)dX) % CELL_X_SIZE );
		bLOSIndexY = CONVERT_WITHINTILE_TO_INDEX( ((INT32)dY) % CELL_Y_SIZE );
		iCurrCubesZ = (INT32) CONVERT_HEIGHTUNITS_TO_INDEX( dZ );

		if (iCurrCubesAboveLevelZ < STRUCTURE_ON_ROOF_MAX)
		{
			if (iCurrCubesAboveLevelZ < STRUCTURE_ON_GROUND_MAX)
			{
				// check objects on the ground
				sDesiredLevel = STRUCTURE_ON_GROUND;
			}
			else
			{
				// check objects on roofs
				sDesiredLevel = STRUCTURE_ON_ROOF;
				iCurrCubesAboveLevelZ -= STRUCTURE_ON_ROOF;
			}

			// check structures for collision
			while (pStructure != NULL)
			{

				if (pStructure->fFlags & STRUCTURE_ROOF || gfCaves || gfBasement )
				{
					if ( dOldZUnits > HEIGHT_UNITS && dZUnits	< HEIGHT_UNITS )
					{
						return( COLLISION_ROOF );
					}
					if ( dOldZUnits < HEIGHT_UNITS && dZUnits	> HEIGHT_UNITS )
					{
						return( COLLISION_INTERIOR_ROOF );
					}
				}

				if (pStructure->sCubeOffset == sDesiredLevel)
				{

					if (((*(pStructure->pShape))[bLOSIndexX][bLOSIndexY] & AtHeight[iCurrCubesAboveLevelZ]) > 0)
					{
						*pusStructureID = pStructure->usStructureID;

						if (pStructure->fFlags & STRUCTURE_WALLNWINDOW && dZ >= WINDOW_BOTTOM_HEIGHT_UNITS && dZ <= WINDOW_TOP_HEIGHT_UNITS)
						{
							if (pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT)
							{

								if (dDeltaX > 0)
								{
									return( COLLISION_WINDOW_SOUTHWEST );
								}
								else
								{
									return( COLLISION_WINDOW_NORTHWEST );
								}
							}
							else
							{
								if ( dDeltaY > 0)
								{
									return( COLLISION_WINDOW_SOUTHEAST );
								}
								else
								{
									return( COLLISION_WINDOW_NORTHEAST );
								}
							}
						}

						if (pStructure->fFlags & STRUCTURE_WALLSTUFF )
						{
							//if ( !CalculateLOSNormal( pStructure, bLOSIndexX, bLOSIndexY, (INT8)iCurrCubesAboveLevelZ, dDeltaX, dDeltaY, dDeltaZ, pdNormalX, pdNormalY, pdNormalZ ) )
							//{
							//	return( COLLISION_NONE );
							//}
							*pdNormalX = 0;
							*pdNormalY = 0;
							*pdNormalZ = 0;

							if (pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT)
							{

								if (dDeltaX > 0)
								{
									*pdNormalX = -1;
									return( COLLISION_WALL_SOUTHEAST );
								}
								else
								{
									*pdNormalX = 1;
									return( COLLISION_WALL_NORTHEAST );
								}
							}
							else
							{
								if ( dDeltaY > 0)
								{
									*pdNormalY = -1;
									return( COLLISION_WALL_SOUTHWEST );
								}
								else
								{
									*pdNormalY = 1;
									return( COLLISION_WALL_NORTHWEST );
								}
							}

						}
						else
						{
							// Determine if we are on top of this struct
							// If we are a tree, not dense enough to stay!
							if ( !( pStructure->fFlags & STRUCTURE_TREE ) && !( pStructure->fFlags & STRUCTURE_CORPSE ) )
							{
								if ( iCurrCubesAboveLevelZ < PROFILE_Z_SIZE-1 )
								{
									if ( !((*(pStructure->pShape))[bLOSIndexX][bLOSIndexY] & AtHeight[ iCurrCubesAboveLevelZ + 1 ])	)
									{
										if ( ( pStructure->fFlags & STRUCTURE_ROOF ) )
										{
											return( COLLISION_ROOF );
										}
										else
										{
											return( COLLISION_STRUCTURE_Z );
										}
									}
								}
								else
								{
									// Search next level ( if we are ground )
									if ( sDesiredLevel == STRUCTURE_ON_GROUND )
									{
										pTempStructure = pMapElement->pStructureHead;

										// LOOK at ALL structs on roof
										while ( pTempStructure != NULL )
										{
											if (pTempStructure->sCubeOffset == STRUCTURE_ON_ROOF )
											{
												if ( !((*(pTempStructure->pShape))[bLOSIndexX][bLOSIndexY] & AtHeight[ 0 ]) )
												{
													return( COLLISION_STRUCTURE_Z );
												}

											}

											pTempStructure = pTempStructure->pNext;
										}
									}
									else
									{
										// We are very high!
										return( COLLISION_STRUCTURE_Z );
									}
								}
							}

							// Check armour rating.....
							// ATE; not if small vegitation....
							if ( pStructure->pDBStructureRef->pDBStructure->ubArmour != MATERIAL_LIGHT_VEGETATION )
							{
								if ( !(pStructure->fFlags & STRUCTURE_CORPSE ) )
								{
									return( COLLISION_STRUCTURE );
								}
							}
						}
					}
				}
				pStructure = pStructure->pNext;
			}

		}

		// check to see if we hit someone
		//if (pTarget && Distance2D( dX - dTargetX, dY - dTargetY ) < HIT_DISTANCE )
		//{
		// well, we're in the right area; it's possible that
		// we're firing over or under them though
		//	if ( dZ < dTargetZMax && dZ > dTargetZMin)
		//		{
		//			return( COLLISION_MERC );
		//		}
		//	}

	}

	return( COLLISION_NONE );
}

INT16 gsLOSDirLUT[3][3] =
{
	315,		0,		45,
	270,		0,		90,
	225,	180,		135
};


BOOLEAN CalculateLOSNormal( 	STRUCTURE *pStructure, INT8 bLOSX, INT8 bLOSY, INT8 bLOSZ, FLOAT dDeltaX, FLOAT dDeltaY, FLOAT dDeltaZ, FLOAT *pdNormalX, FLOAT *pdNormalY, FLOAT *pdNormalZ )
{
	INT32		cntx, cnty;
	INT8		bX, bY, tX, tY;
	INT8		bNumNormals = 0;
	BOOLEAN fParimeter;


	vector_3		vZ, vTemp2, vNormal, vAveNormal, vTemp, vIncident;

	vZ.x = 0;
	vZ.y = 0;
	vZ.z = 2;

	vIncident.x = -1 * dDeltaX;
	vIncident.y = dDeltaY;
	vIncident.z = 0;
	// Nomralize
	vIncident = VGetNormal( &vIncident );

	vAveNormal.x = 0;
	vAveNormal.y = 0;
	vAveNormal.z = 0;

	//OK, center on xy and goforit!
	for ( cntx = 0; cntx < 3; cntx++ )
	{
		bX = bLOSX + ( cntx - 1 );

		// Check for boundry conditions, use same as one before boundary
		if ( bX < 0 || bX > 4 )
		{
			continue;
		}

		for ( cnty = 0; cnty < 3; cnty++ )
		{
			bY = bLOSY + ( cnty - 1 );

			if ( bY < 0 || bY > 4 )
			{
				continue;
			}

			if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0)
			{
				fParimeter = FALSE;
				// THIS MUST BE THE POLYGONAL SURFACE, CHECK!
				do
				{
					tX = (bX - 1);
					tY = bY;
					if ( tX >= 0 )
					{
						if ( ((*(pStructure->pShape))[tX][tY] & AtHeight[ bLOSZ ]) <= 0)
						{
							fParimeter = TRUE;
							break;
						}
					}

					tX = (bX + 1);
					tY = bY;
					if ( tX <=4 )
					{
						if (((*(pStructure->pShape))[tX][tY] & AtHeight[ bLOSZ ]) > 0)
						{

						}
						else
						{
							fParimeter = TRUE;
							break;
						}
					}

					tX = bX;
					tY = bY - 1;
					if ( tX >= 0 )
					{
						if (((*(pStructure->pShape))[tX][tY] & AtHeight[ bLOSZ ]) > 0)
						{

						}
						else
						{
							fParimeter = TRUE;
							break;
						}
					}

					tX = bX;
					tY = bY + 1;
					if ( tX >=4 )
					{
						if (((*(pStructure->pShape))[tX][tY] & AtHeight[ bLOSZ ]) > 0)
						{
						}
						else
						{
							fParimeter = TRUE;
							break;
						}
					}

				} while( FALSE );

				// OK, now add angles, but not the center!
				if (	cntx == 1 && cnty == 1 )
				{

				}
				else //if ( fParimeter )
				{
					// OK< Calcluate normal using cross-product
					// 1) Calculate Vector2
					vTemp2.x = (float)(bX - bLOSX);
					vTemp2.y = (float)(bY - bLOSY);
					vTemp2.z = 1;

					// Skip ones ||l to incident vector
					//if ( vTemp2.x == vIncident.x && vTemp2.y == vIncident.y )
					//{
					//	continue;
					//}

					//2) Calculate Normal from cross product
					vNormal =	VCrossProduct( &vTemp2, &vZ );

					if ( VGetLength( &vNormal ) > 0 )
					{
						// Nomralize
						vNormal = VGetNormal( &vNormal );

						// CHECK ANGLE BRTWEEN INCIDENNCE AND NORMAL
						//if ( VDotProduct( &vNormal, &vIncident ) > 0 )
						{
							bNumNormals++;

							//Average normal!
							vTemp =	VAdd( &vNormal, &vAveNormal );
							vAveNormal = VSetEqual( &vTemp );
							vAveNormal = VDivScalar( &vAveNormal, bNumNormals );
							// Nomralize
							vAveNormal = VGetNormal( &vAveNormal );

						}

					}
				}
			}
		}
	}

	*pdNormalX = 0;
	*pdNormalY = 0;
	*pdNormalZ = 0;

	if ( bLOSZ < 4 )
	{
		if (((*(pStructure->pShape))[bLOSX][bLOSY] & AtHeight[ bLOSZ + 1 ]) > 0)
		{
			//*pdNormalZ = -1;
		}
	}


	// Average angle
	if ( VGetLength( &vAveNormal ) > 0 )
	{
		*pdNormalX	= vAveNormal.x;
		*pdNormalY	= vAveNormal.y;


		// OK done, now determine direction
		if ( dDeltaX > 0 )
		{
			*pdNormalX *= -1;
		}

		if ( dDeltaY < 0 )
		{
			*pdNormalY *= -1;
		}

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}

}

//////////////////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 4: New Shooting Mechanism
//
// The Shooting Mechanism has been significantly altered in HAM 4. With the new system,
// any shot has a certain random deviation off the target center. The possible (max)
// size of this deviation is affected by many factors, including primarily the shooter's
// skill and aiming, the scope used, the gun's inherent properties, and so on. This is
// calculated in several distinct steps, each of which is responsible for one type of
// effect, such as the sway of the muzzle or tracking the target's movement.
//
// This function, which basically drives the whole thing, plays with the location of the 
// "center point", the point where the gun would shoot if aimed and fired with 100% accuracy. 
// The idea is to make gunplay more randomal and less exploitable.
//
// Related changes include an overhaul of the CalcChanceToHit() formula, as well as 
// restructuring of UseGun() and its sub-functions.
 
void AdjustTargetCenterPoint( SOLDIERTYPE *pShooter, INT32 iTargetGridNo, FLOAT *dEndX, FLOAT *dEndY, FLOAT *dEndZ, OBJECTTYPE *pWeapon, UINT32 uiMuzzleSway, INT16 *sApertureRatio )
{
	SOLDIERTYPE *pTarget = SimpleFindSoldier( iTargetGridNo, pShooter->bTargetLevel );

	///////////////////////////////////////////
	// Calculate shooter's coordinates

	FLOAT		dStartX=0;
	FLOAT		dStartY=0;
	FLOAT		dStartZ=0;

	// Find start Z for the shooter given his stance and location. This actually marks the point where the gun's
	// muzzle is located.
	CalculateSoldierZPos( pShooter, FIRING_POS, &dStartZ );

	// Locate absolute center X,Y of the shooter
	dStartX = (FLOAT) CenterX( pShooter->sGridNo );
	dStartY = (FLOAT) CenterY( pShooter->sGridNo );

	////////////////////////////////////////////////////////////////////////////
	// Calculate difference (Delta) between start and end point of bullet flight

	FLOAT		dDeltaX=0;
	FLOAT		dDeltaY=0;
	FLOAT		dDeltaZ=0;

	dDeltaX = *dEndX - dStartX;
	dDeltaY = *dEndY - dStartY;
	dDeltaZ = *dEndZ - dStartZ;

	///////////////////////////////////////////////////////////////////////
	// Find distance between shooter and target, in points.
	FLOAT		d2DDistance=0;

	d2DDistance = Distance3D( dDeltaX, dDeltaY, CONVERT_HEIGHTUNITS_TO_DISTANCE( dDeltaZ ) );

	// Round it upwards.
	INT32 iDistance = (INT32) d2DDistance;
	if ( d2DDistance != iDistance )
	{
		iDistance += 1;
		d2DDistance = (FLOAT) ( iDistance);
	}

	////////////////////////////////////////////////////////////////////////
	// Calculate the "shot angle". This is the angle of a "perfect" shot, from the end of the muzzle straight
	// to the center of the target. The resulting angle, in Radians, is relative to the Positive X axis.
	DOUBLE	ddOrigHorizAngle=0;
	DOUBLE	ddOrigVerticAngle=0;

	ddOrigHorizAngle = atan2( dDeltaY, dDeltaX );
	ddOrigVerticAngle = atan2( dDeltaZ, (d2DDistance * 2.56f) );

	//////////////////////////////////////////////////////////////////////////////
	//
	// Now that we have the perfect shot coordinates and angle, we can start applying various movements to
	// the gun's muzzle. We take into account both the Muzzle Sway factor calculated by the CTH formula, as well
	// as compensation for things like firing out of range or firing at a moving target.
	//
	// Movements are recorded as relative to the center of the target, either left/right, up/down, or both.
	// Imagine the center of the target being at 0,0.
	// Positive X movement is to the right of the target, Positive Y movement is above the target.
	// We'll use these values later to determine where the bullet should actually go.

	// Muzzle Offset values track movements of the gun's barrel. They'll be fed to various formulae here, which
	// will alter and return them as necessary.
	FLOAT dMuzzleOffsetX = 0.0;
	FLOAT dMuzzleOffsetY = 0.0;

	//////////////////////////////////////////////////////////////////////////////////////
	// STEP 1: Shot Aperture
	//
	// Here we calculate the maximum radius of movement possible for our gun's muzzle, given the Muzzle Sway
	// value which was fed into this function. The larger the Muzzle Sway, the larger the radius, and thus the
	// more our muzzle can randomly sway away from the target's center.
	// We'll later use Shot Aperture to limit the final Muzzle Offset values.

	FLOAT iBasicAperture = 0.0;
	FLOAT iDistanceAperture = 0.0;
	FLOAT iMaxAperture = 0.0;
	FLOAT iAperture = 0.0;

	// First, let's calculate the basic Aperture. This is the size of an unmodified aperture at 1x Normal Distance.
	iBasicAperture = CalcBasicAperture( );
	
	// Next, find out how large the aperture can be around the target, given range. The further the target is, the
	// larger the aperture can be.
	iDistanceAperture = iBasicAperture * (d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);

	///////////////////////////////////////////////////////////////
	// To make things easier for us, we now calculate the Magnification Factor for this shot. A Mag Factor
	// is a divisor to the sway of the muzzle. It's about the same as multiplying CTH by a certain amount.
	// Note that both optical magnification devices (like scopes) and dot-projection devices (like lasers and 
	// reflex sights) provide this sort of bonus.

	FLOAT iMagFactor = CalcMagFactor( pShooter, pWeapon, d2DDistance, (UINT8)pShooter->aiData.bAimTime );

	// Next step is to apply scope/projection factor to decrease the size of the aperture. This gives us the "Max
	// Aperture" value - the size of the shooting circle if the gun is as unstable as possible.
	iMaxAperture = iDistanceAperture / iMagFactor;

	// We now use the Muzzle Sway value, calculated by the CTH formula, to decrease the size of the shot aperture.
	// It is used as a percentage: a 50% muzzle sway value gives a cone with half the maximum radius. A cone with
	// 0% Muzzle Sway is a single line with no width (meaning all shots will fly right down the center, and all will
	// hit the target), while a cone with 100% muzzle sway is as wide as possible.
	iAperture = (FLOAT)((iMaxAperture * uiMuzzleSway) / 100);

	// Aperture ratio below is used for Experience Gain. The smaller the aperture, compared to the size of the
	// Distance Aperture, the more experience we get.
	// Since we're using the Distance Aperture for this calculation, the final result is much smaller (better)
	// when scopes are in play.
	// Note that we calculate experience gain in the Bullet Impact formula now, to allow increasing experience
	// gain based on whether the target was actually hit or not.
	*sApertureRatio = (INT16)(100.0f - ((iAperture / iDistanceAperture) * 100.0f));

	///////////////////////////////////////////////////////////////////////////////////////
	// MUZZLE MOVEMENTS
	//
	// We begin by calculating effects that change the direction of the muzzle. Every change in muzzle direction
	// may lead to "missing" the center of the target, although that doesn't mean the target won't be hit anyway.
	//
	// The calculations in this section are only done for single-shot bullets, or the first bullet in a burst/auto
	// volley. If a volley continues, subsequent bullets will "inherit" past muzzle movements from one another.
	// As you'll see, this allows the shooter to readjust the weapon WHILE it is firing, rather than randomize
	// muzzle movements every time.
	
	if (pShooter->bDoBurst <= 1)
	{

		///////////////////////////////////////////////////////////////////////////////////////////
		// STEP 2: Random Muzzle Sway
		//
		// Our "Shot Aperture" determines a radius around the target's center, within which our
		// muzzle might be pointed when the trigger is squeezed. By reducing Muzzle Sway (through
		// aiming and skill), we make this radius smaller.
		// Here, we pick a random point within the circle drawn by this radius. This is where
		// the muzzle will be pointed, relative to the center of the target, when the trigger is pulled.

		CalcMuzzleSway( pShooter, &dMuzzleOffsetX, &dMuzzleOffsetY, iAperture );

		/////////////////////////////////////////////////////////////////////////////////////////////
		// STEP 3: Lateral Offset due to Target Movement
		//
		// Moving targets are harder to hit, because the shooter needs to anticipate motion and point
		// his gun accordingly ahead of the target. This is called "Leading". The following function
		// measures how much the target has moved, and adjusts the gun to point BEHIND the target.
		// Depending on the size of the Shot Aperture, as well as the aptitude of the shooter himself,
		// this could mean that shots will all (or most) go behind the target, as though firing at where
		// it WAS rather than where it IS.
		// Note that the Target Tracking formula only adjusts the muzzle along the X-Axis, I.E. moving the
		// shot behind the target, never up or down.

		if (pTarget)
		{
			CalcTargetMovementOffset( pShooter, pTarget, pWeapon, &dMuzzleOffsetX, ddOrigHorizAngle, (INT32)iAperture );
		}

		pShooter->dInitialMuzzleOffsetX = dMuzzleOffsetX;
		pShooter->dInitialMuzzleOffsetY = dMuzzleOffsetY;

		/////////////////////////////////////////////
		// First shot in a burst/auto volley

		if (pShooter->bDoBurst == 1 || pShooter->bDoAutofire > 1)
		{
			// A shooter does not get to exercise counter-force on the first few bullets in a volley.
			// These commands reset the counter-force.
			pShooter->dPrevCounterForceX = 0.0;
			pShooter->dPrevCounterForceY = 0.0;

			////////////////////////////////////////////////////////////////////////////////////////////
			// STEP 4: Pre-Compensating for Future Recoil
			//
			// Since soldiers can't use counter-force to work against the recoil of the first few bullets
			// in a burst (it just happens too fast), they can instead adjust the muzzle of the gun
			// BEFORE pulling the trigger. They take the gun's recoil into consideration for this.
			// If the muzzle is adjusted by the exact correct amount (and assuming no other deviation from
			// the other factors), the muzzle will end up pointed at the center of the target when the
			// shooter gets his first chance to apply counter force. This dramatically increases the
			// chance to hit the target with those first few bullets, again assuming no other deviation
			// was applied.
			CalcPreRecoilOffset( pShooter, pWeapon, &dMuzzleOffsetX, &dMuzzleOffsetY, (UINT32)d2DDistance );
		}
	}

	// The first bullet in a burst/autofire volley works the same as a single-shot (see above). However, once
	// it's time to fire the second bullet, things change radically. Our shooter now knows (broadly) where his
	// muzzle is pointing, and can use the time spent firing the volley to adjust his aim. This may or may not
	// allow the shooter to actually compensate for aiming errors made with the first bullet! Of course,
	// while doing this, the shooter also has to combat the weapon's recoil - a difficult job with some weapons.
	else
	{
		// In a Burst/Autofire volley, we do not need to recalculate muzzle sway or target movement offsets and
		// other related muzzle movements. Instead, we inherit the muzzle offset variables from the previous bullet
		// in the volley.
		dMuzzleOffsetX = pShooter->dPrevMuzzleOffsetX;
		dMuzzleOffsetY = pShooter->dPrevMuzzleOffsetY;

		////////////////////////////////////////////////////////////////////////////////////////////
		// STEP 4: Recoil Offset
		//
		// Guns in NCTH have a defined recoil strength and direction. Different guns will "kick" differently.
		// The shooter will need to apply force to counter the effects of recoil while the gun is firing,
		// or else end up with bullets being fired up into the sky or down into the earth. Skilled shooters
		// can actually productively handle their weapons, using this extra time to improve their aim by
		// following the impact of the bullets and adjusting the muzzle direction accordingly. Of course,
		// this is made difficult by having to fight the gun's recoil at the same time.
		// This function alters the muzzle direction based on the gun's recoil, and the amount of counter-force
		// our shooter applies to fight that recoil.
		CalcRecoilOffset( pShooter, &dMuzzleOffsetX, &dMuzzleOffsetY, pWeapon, (UINT32)d2DDistance );
	}

	// At this point, basic muzzle deviation is fully calculated. For Autofire/Burst volleys, the next bullet
	// in the volley will actually inherit these values and behave accordingly. For this purpose, we store
	// the Muzzle Offset values in our shooter's data.
	if ( pShooter->bDoBurst > 0 )
	{
		// If this is the first shot in a burst/autofire volley, subsequent shots will inherit the same Muzzle
		// Direction from this shot. In other words, when we reach the second (or later) bullet in the volley,
		// we will no longer calculate the above movements like movement tracking or muzzle sway. Instead, we'll
		// just remember were our muzzle was pointed last, and proceed from there. This allows volleys to be adjusted
		// while they're being fired, with the hope of redirecting the muzzle towards the center of the target if
		// the first shot was off-center.
		pShooter->dPrevMuzzleOffsetX = dMuzzleOffsetX;
		pShooter->dPrevMuzzleOffsetY = dMuzzleOffsetY;
	}
	else
	{
		// Single-shot fire does not require storing Muzzle Offset for later use. The next fired bullet will calculate
		// its own offsets. To be on the safe side, we reset the Muzzle Offset values stored in the shooter's data.
		pShooter->dPrevMuzzleOffsetX = 0;
		pShooter->dPrevMuzzleOffsetY = 0;
	}

	UINT16 uiRange = GunRange(pWeapon, pShooter);
	// Is the target beyond the gun's range?
	if (d2DDistance > ((FLOAT)uiRange*gGameCTHConstants.RANGE_COEFFICIENT))
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		// STEP 5: Bullet Drop Compensation
		//
		// Bullets reaching their maximum flight range will begin to drop towards the ground. Shooters wishing
		// to fire beyond a gun's range can use the simple ballistic principle of raising the muzzle upwards to
		// have the bullets fly in a higher angle - allowing the bullet to avoid hitting the ground for a little
		// longer, and as a result having more actual range. However, this is tricky, and only the most skilled 
		// shooters will be able to do this reliably. In any case, this can't extend the range of the gun by more 
		// than a handful of tiles.

		CalcRangeCompensationOffset( pShooter, &dMuzzleOffsetY, (INT32)d2DDistance, pWeapon );
	}

	// We have now completed altering the direction of the muzzle. From here on in, the muzzle direction is fixed.
	// However this doesn't mean that the shot will fly where the muzzle is pointed! We still have a few more
	// steps to go.
	FLOAT dShotOffsetX = dMuzzleOffsetX;
	FLOAT dShotOffsetY = dMuzzleOffsetY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// STEP 6: Bullet Deviation
	//
	// In the old JA2 system, a gun's Accuracy value was treated a simple bonus to CTH. NCTH uses
	// this value in a completely different fasion: It simulates a weapon's maximum "effective" range,
	// the range at which a weapon can reliably hit the target it was aimed at (assuming perfect aim).
	// Like Muzzle Sway (see above), the program "draws" a circle over the target whose radius is based
	// on the Accuracy value. Higher accuracy = smaller circle. Then, it picks a completely random point
	// within that circle, and sends the bullet that way. Over greater distances, this can amount to
	// several penalties, thus making accurate weapons essential for any long-range shot.

	FLOAT iBulletDev = CalcBulletDeviation( pShooter, &dShotOffsetX, &dShotOffsetY, pWeapon, (UINT32)d2DDistance );

	/////////////////////////////////////////////////////////////////////////////////////////////
	// STEP 7: Limit Shot Angle
	//
	// The system provides an INI-defined constant that tells us the maximum angle to which our
	// shot might deviate, given all above factors combined. This is done mainly to avoid shots
	// flying off in unrealistic directions, such as sideways or upwards. That can happen as a
	// result of too many modifiers by the previous functions. In addition, the INI value also
	// affects accuracy of ALL shots in the game, and is enforced here a second time.
	
	LimitImpactPointByFacing( pShooter, pTarget, &dShotOffsetX, &dShotOffsetY, dEndX, dEndY );
	LimitImpactPointToMaxAperture( &dShotOffsetX, &dShotOffsetY, iDistanceAperture );				


	// DEBUGGING: Remove this!
	if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
	{
		CHAR16 szLeftRight[10];
		CHAR16 szUpDown[10];

		if (dShotOffsetX > 0)
		{
			swprintf(szLeftRight, L"right");
		}
		else
		{
			swprintf(szLeftRight, L"left");
		}
		if (dShotOffsetY > 0)
		{
			swprintf(szUpDown, L"up");
		}
		else
		{
			swprintf(szUpDown, L"down");
		}	

		if (pShooter->bDoBurst == 0 || pShooter->bDoBurst == 1)
		{
			ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, L"%d. Shot aperture %2.1f, Accuracy %2.1f, goes %2.1f %s and %2.1f %s", pShooter->bDoBurst, iAperture, iBulletDev, dShotOffsetX, szLeftRight, dShotOffsetY, szUpDown );
		}
		else
		{
			if ((pShooter->bDoBurst-1)%3 == 0)
			{
				ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, L"%d. MO: %2.1f, %2.1f - SO: %2.1f, %2.1f - CF: %2.1f, %2.1f", pShooter->bDoBurst, dMuzzleOffsetX, dMuzzleOffsetY, dShotOffsetX, dShotOffsetY, pShooter->dPrevCounterForceX, pShooter->dPrevCounterForceY );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d. MO: %2.1f, %2.1f - SO: %2.1f, %2.1f - CF: %2.1f, %2.1f", pShooter->bDoBurst, dMuzzleOffsetX, dMuzzleOffsetY, dShotOffsetX, dShotOffsetY, pShooter->dPrevCounterForceX, pShooter->dPrevCounterForceY  );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// STEP 8: 2D to 3D
	//
	// So far, we've been accumulating the adjustment of muzzle/shot angles in a very peculiar way:
	// by moving the point of impact either above/below the target or to its left/right. So now, we have
	// a final intended impact point, but only relative to the target. To fire the shot at the intended
	// point, we need to find out its 3D location.
	// This is not as complicated as it may seem. Several times during the formula, I noted that
	// plotting muzzle/shot movement can be imagined as drawing a circle on top of the target, and selecting
	// a point within that circle that corresponds to how much we want to move the muzzle. That imaginary
	// circle lies on a plane that is 90-degree perpenticular to the angle of the "perfect" shot as we
	// calculated at the very start of this function. Therefore, we can use sinus and cosinus to figure
	// out the location of any point on that target.
	//
	// Note that for Z-Elevation (moving the muzzle up/down), we already have the value we require. We only
	// need to plot the X/Y coordinates out of what was previously the X-Shot-Offset.
	//
	// Here we figure out the X/Y coordinates in relation to the target's center:

	FLOAT dSecondDeltaX = (FLOAT)(sin(ddOrigHorizAngle) * dShotOffsetX)*(-1);
	FLOAT dSecondDeltaY = (FLOAT)(cos(ddOrigHorizAngle) * dShotOffsetX);

	// Finally, we add our deltas to the coordinates of the target's center.
	*dEndX += dSecondDeltaX;
	*dEndY += dSecondDeltaY;
	*dEndZ = __max(-127, *dEndZ + (dShotOffsetY * 11.3f)); // Y, which represented up/down offset, is actually the Z axis in 3d game terms...

	// These values are returned to the calling function, in this case UseGunNCTH(). That function then fires the bullet
	// directly at the intended coordinates.

}

FLOAT CalcProjectionFactor( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT d2DDistance, UINT8 ubAimTime )
{
	FLOAT iProjectionFactor = 1.0f;
	FLOAT iTargetMagFactor = d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE;

	if (ubAimTime > 0)
	{
		// Flugente: if scope modes are allowed, player team uses them
		if ( gGameExternalOptions.fScopeModes && pShooter && pShooter->bTeam == gbPlayerNum && pWeapon->exists() == true && Item[pWeapon->usItem].usItemClass == IC_GUN )
		{
			// Flugente: check for scope mode
			std::map<INT8, OBJECTTYPE*> ObjList;
			GetScopeLists(pWeapon, ObjList);
		
			iProjectionFactor = Item[ObjList[pShooter->bScopeMode]->usItem].projectionfactor;
		}
		else
			iProjectionFactor = GetProjectionFactor( pWeapon );

		if (floor(iTargetMagFactor*10) > floor(iProjectionFactor*10.001))
		{
			iProjectionFactor -= (iTargetMagFactor - iProjectionFactor);
			iProjectionFactor = __max(iProjectionFactor, 1.0f);
		}
	}

	return iProjectionFactor;
}

FLOAT CalcMagFactor( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT d2DDistance, UINT8 ubAimTime )
{
	FLOAT iFinalMagFactor = 1.0;
	FLOAT iScopeFactor = 0;
	FLOAT iProjectionFactor = 0;

	// Flugente: if scope modes are allowed, player team uses them. We either use a scope or we don't, so the magnification factor isn't fitted to range (this is actually bad)
	if ( gGameExternalOptions.fScopeModes && pShooter && pShooter->bTeam == gbPlayerNum && pWeapon->exists() == true && Item[pWeapon->usItem].usItemClass == IC_GUN )
	{
		iScopeFactor = GetBestScopeMagnificationFactor( pShooter, pWeapon, d2DDistance );
		iProjectionFactor = CalcProjectionFactor(pShooter, pWeapon, d2DDistance, ubAimTime);
		iFinalMagFactor = __max(iScopeFactor, iProjectionFactor);
	}
	else
	{
		FLOAT iTargetMagFactor = d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE;
		FLOAT rangeModifier = GetScopeRangeMultiplier(pShooter, pWeapon, d2DDistance);

		if (ubAimTime > 0)
		{
			iScopeFactor = GetBestScopeMagnificationFactor( pShooter, pWeapon, d2DDistance );
			iScopeFactor = __min(iScopeFactor, __max(1.0f, iTargetMagFactor/rangeModifier));
			iProjectionFactor = CalcProjectionFactor(pShooter, pWeapon, d2DDistance, ubAimTime);
			iFinalMagFactor = __max(iScopeFactor, iProjectionFactor);
		}
	}

	return iFinalMagFactor;
}

// HEADROCK HAM 4: This function calculates and returns the maximum shot aperture as a radius to which the muzzle of
// the gun might sway off the target center when the trigger is pulled.
FLOAT CalcBasicAperture()
{
	FLOAT iBasicAperture = 0.0;

	// Maximum aperture is the radius of a shot performed with 100% muzzle sway (a completely unstabled gun).
	// It is the radius of a circle "drawn" at 1xNormal Shooting Distance, at the end of a cone starting at the
	// tip ofour muzzle.
	// To get this value, we get a constant read from the INI file which defines the spread angle of the cone - in
	// other words, the angle between a line running down the center of the cone and a line running down its side.
	// By default, the angle is 22.5 degrees. This gives a cone with a 45-degree spread, covering the entire area
	// in front of the shooter. This is done to ensure that shots don't ever appear to go sideways or upwards, 
	// making weird visuals. 
	// Decreasing the value of this angle will make ALL shots tighter, so use discretion.

	// Calculate the angle in radians
	DOUBLE ddMaxAngleRadians = (gGameCTHConstants.DEGREES_MAXIMUM_APERTURE * RADIANS_IN_CIRCLE) / 360;

	// Calculate the radius of the aperture circle at a distance of 1xNormal from the muzzle's end.
	iBasicAperture = (FLOAT)(sin(ddMaxAngleRadians) * gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);
	
	// Return this as the Shot Aperture value to be used here on in.
	return iBasicAperture;
}

// HEADROCK HAM 4: This function causes an offset in x/y/z shooting coordinated due to under-compensation
// for target movement.
void CalcTargetMovementOffset( SOLDIERTYPE *pShooter, SOLDIERTYPE *pTarget, OBJECTTYPE *pWeapon, FLOAT *dMuzzleOffsetX, DOUBLE ddShootingAngle, INT32 iAperture )
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// HEADROCK HAM 4: New Shooting Mechanism, Target Movement
	//
	// The effect of Target Movement on shooting tries to mimic real-life. It does so by requiring the shooter
	// to track the target as it moves.
	//
	// This is done by measuring the distance and angle that the target has moved since the start of its most recent
	// turn. Once this is calculated, we compare this to the angle where the shot will come from, to see how much it's
	// going to affect the shooter's ability to hit the target.
	//
	// Once we know the size of the penalty, we apply it by moving the "center point" for the attack a little behind
	// the target. This means that more shots will go behind the target, less in front of it, and generally reduce
	// our ability to hit that target properly. Aiming the gun well does help, but can't completely eliminate the
	// penalty.
	// 
	// In addition, skilled shooters can figure out the target's speed and heading if they have sufficient time. That
	// means that after the target has moved a certain number of tiles, any FURTHER tiles it moves will actually
	// give the shooter time to adjust, lowering the penalty somewhat. Skilled shooters can start to adjust very
	// quickly, while unskilled ones will find the target very hard to hit unless it runs a long distance, and even then
	// can't compensate very much.


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We start by figuring out how much distance the target has moved, and at what angle (compared to positive x).
	// We do this by comparing the target's position at the start of its recent turn (sStartPos) with its current
	// position.

	if (gGameCTHConstants.MOVEMENT_TRACKING_DIFFICULTY == 0)
	{
		// Movement tracking is defined as nonexistent, and thus doesn't take place.
		return;
	}

	if (pTarget->sOldXPos == 0 && pTarget->sOldYPos == 0)
	{
		// HEADROCK HAM 4: Hopefully the right spot for this: This soldier has no "old" coordinates, so just set them
		// to wherever he/she is currently standing.
		pTarget->sOldXPos = CenterX( pTarget->sGridNo );
		pTarget->sOldYPos = CenterY( pTarget->sGridNo );
		// Since movement is now nonexistent, break the formula here without adjusting coordinates.
		return;
	}

	// Find previous position of this soldier, recorded when his/her last turn started.
	INT16 sStartPosX = pTarget->sOldXPos;
	INT16 sStartPosY = pTarget->sOldYPos;

	// Find the current position of this soldier.
	INT16 sCurPosX = 0;
	INT16 sCurPosY = 0;
	ConvertGridNoToCellXY(pTarget->sGridNo, &sCurPosX, &sCurPosY);

	// Figure out the difference between the two coordinates.
	INT32 sDeltaX = sStartPosX - sCurPosX;
	INT32 sDeltaY = sStartPosY - sCurPosY;

	// Pythagorian distance calculation.
	INT16 sDistanceMoved = (INT16)sqrt((double) (sDeltaX * sDeltaX) + (sDeltaY * sDeltaY));
	sDistanceMoved /= CELL_X_SIZE; // convert to Tiles.

	//CHRISL: The above all assumes that the target is actually still possibly moving at the same rate
	//	he was moving at before the end of his turn.  But what happens if a target moves, stops and 
	//	changes stance?  We use usAnimState to figure out what stance we're currently in by looking at
	//	out current animation state.  And I think usUIMovementMode tells us what animation state we
	//	last moved in.  So if these two values are different, we should be able to assume that the target
	//	has stopped moving.
	// HEADROCK HAM 5: Oh god Chris, that's not how it's supposed to work. Changing stance doesn't miraculously
	// erase all the ground you've covered. The target has still moved some distance since its previous turn,
	// and we ARE taking this into account now regardless of what the target did at the end of the move. We
	// are essentially assuming that turns overlap, so the shot isn't being taken at the exact moment after the
	// guy has crouched down, it's taking place simultaneously with his sprint. Commented out.
	/*
	if (pTarget->usAnimState != pTarget->usUIMovementMode)
		sDistanceMoved = 0;
	*/

	if (sDistanceMoved == 0)
	{
		//Target hasn't moved. Simply abort the formula, as there will be no movement penalty anyway.
		return;
	}

	// Figure out the angle of movement compared to positive X axis of the tile grid. The result is in RADIANS.
	DOUBLE ddMovementAngle = atan2( (DOUBLE)sDeltaY, (DOUBLE)sDeltaX );
	
	///////////////////////////////////////////////////////////////////////////////////////
	// Now we need to figure out the relation between the Shooting Angle and the Movement Angle. This
	// will tell us whether the target is moving perpendicular to our shot, directly at/away from us,
	// or anywhere in between.

	DOUBLE ddRelativeAngle = ddMovementAngle - ddShootingAngle;

	// The penalty coefficient is a sinus of the relative angle. That means that if the shooting angle and the movement 
	// angle are the same or 180-degree opposite, there will be no movement penalty (0%). If the shooting angle is 
	// perpendicular to the movement angle (90 or 270 degrees), there will be full movement penalty (100%). 
	// Other relative angles fall in between, according to the sinus curve.
	FLOAT iPenaltyCoeff = (FLOAT)sin(ddRelativeAngle);

	if (iPenaltyCoeff == 0.0)
	{
		// Parallel angle (0 or 180) gives no penalty, so stop here without adjusting coordinates.
		return;
	}

	// Now let's figure out how competent our shooter is.
	FLOAT iCombinedSkill = gGameCTHConstants.MOVEMENT_MRK * EffectiveMarksmanship( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_WIS * EffectiveWisdom( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_DEX * EffectiveDexterity( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_EXP_LEVEL * (EffectiveExpLevel( pShooter ) * 10);

	// Average
	FLOAT iDivisor = gGameCTHConstants.MOVEMENT_MRK + gGameCTHConstants.MOVEMENT_WIS + gGameCTHConstants.MOVEMENT_DEX + gGameCTHConstants.MOVEMENT_EXP_LEVEL;
	iCombinedSkill /= iDivisor;

	// Limit to a 0-100 scale.
	UINT32 uiCombinedSkill = (UINT32)__max(__min(iCombinedSkill,100), 0);

	// Here we define the number of tiles the target must move to give our shooter the worst possible penalty.
	// Again, if the shooter is skilled, this will occur after fewer tiles have been moved. 
	// If the target moves *more* tiles than this, the movement penalty will begin to diminish. That's our shooter
	// beginning to compensate for the target's speed, pointing the gun ahead of the target ("Leading the target").
	UINT8 uiTilesForMaxPenalty = (100-uiCombinedSkill) / (100 / gGameCTHConstants.MOVEMENT_TRACKING_DIFFICULTY);
	
	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	// Add a percentage-based modifier from the weapon and its attachments. Movement tracking devices will
	// provide faster compensation for target movement, allowing the shooter to begin adjusting the muzzle
	// after the target moves a smaller number of tiles.
	INT32 moda = (INT32)(uiTilesForMaxPenalty * GetTargetTrackingModifier( pWeapon, stance )) / 100;
	INT32 modb = (INT32)(uiTilesForMaxPenalty * GetTargetTrackingModifier( pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight )) / 100;
	uiTilesForMaxPenalty += (UINT8)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);

	if (uiTilesForMaxPenalty == 0)
	{
		// Shooter is skilled enough to avoid ANY movement penalty. Return without adjusting.
		return;
	}

	/////////////////////////////////////////////////////////////////
	// Now we start adding up the penalty for each tile of movement:
	UINT16 usNumPenaltiesAdded;
	FLOAT iFinalPenalty = 0;

	// Up to a certain number of tiles, the shooter is disoriented by the target's movement, and the penalty
	// increases for each tile moved. This simulates targets moving at various speeds: The more tiles a target has
	// moved, the faster it is seen to be going, hence the gun's muzzle will be pointing more behind the target.
	for (usNumPenaltiesAdded = 0; usNumPenaltiesAdded < __min(uiTilesForMaxPenalty,sDistanceMoved); usNumPenaltiesAdded++)
	{
		// Deteriorate aim by one fraction. The size of the fractions is defined in the INI file.
		iFinalPenalty += gGameCTHConstants.MOVEMENT_PENALTY_PER_TILE;
	}

	// By now, we've hit the end of the penalty zone. Either the target has only moved enough tiles to confuse us,
	// or we're simply done being confused and can now compensate.
	// If we can start compensating, each further tile of movement will in fact return some of our aim back towards
	// the center of the target, indicating that the shooter is trying to "lead". 
	// Again, skilled shooters will reach the cutoff point earlier, which translates into receiving more of their
	// penalty back as the target moves further. In the same situation, a less-experienced shooter will still be
	// confused by movement, suffering from extra tiles rather than compensating.

	if (usNumPenaltiesAdded < sDistanceMoved)
	{
		// First let's add one step where the penalty doesn't change. The target has moved an extra
		// tile beyond the cutoff range, but our shooter doesn't suffer extra penalty for that tile.
		// If the target keeps moving, the shooter will then begin compensating.
		usNumPenaltiesAdded ++;

		// If there's any distance left, each tile moved will actually DECREASE the penalty, thus
		// moving the shooting point back towards the target. The more tiles the target moves now,
		// the easier it is to hit.
		for (usNumPenaltiesAdded; usNumPenaltiesAdded < sDistanceMoved; usNumPenaltiesAdded++ )
		{
			// Improve aim by 1/2 the specified penalty per tile moved. Yes, the penalty diminishes SLOWER than
			// it was gained. The target needs to move 3x as much as the uiTileForMaxPenaltyValue for our shooter
			// to actually compensate for the entire penalty. If it has moved only 2x the required amount,
			// our shooter has only compensated for HALF the penalty.
			iFinalPenalty -= (gGameCTHConstants.MOVEMENT_PENALTY_PER_TILE / 2);

			// Given enough time, the shooter can completely compensate for movement.
		}
	}

	// Now we have a Final Penalty value. This is the distance our muzzle is trailing behind the
	// target.
	
	// Let's make sure that the penalty isn't negative. We don't want to over-compensate. That wouldn't be nice.
	iFinalPenalty = __max(0, iFinalPenalty);

	// Now we need to figure out exactly how much our muzzle has moved in actual distance points. We do this
	// by applying it as a percentage to the size of our Shot Aperture. Since Shot Aperture is an indicator of 
	// how well our gun is aimed, this means that better-aimed shots will "trail" less than poorly-aimed shots.
	// In other words, aiming will effectively reduce the size of the penalty, making it easier to hit the target
	// despite its movement. Note that with an appropriately-sized movement penalty, and an inexperienced shooter,
	// it's quite possible that even a very-well-aimed shot will stand no chance to hit the target if it is
	// moving fast enough.
	iFinalPenalty = ((iFinalPenalty * iAperture)/100);

	// Next, we apply the Penalty Coefficient calculated earlier based on direction of movement. Again, if the
	// target is moving perpendicular to the angle of the shot, the shooter receives the maximum (100%) size of the
	// penalty. Sharper angles provide less.
	// Also note that thanks to the use of a sinus in calculating the coefficient, we are also now telling the
	// formula whether the shot should lag to the left or the right. A positive coefficient means the shot will head
	// right of the target. A negative coefficient will make the shot deviate left of the target.
	iFinalPenalty *= iPenaltyCoeff;

	// We now have our final movement penalty value, which represents the distance our muzzle is lagging behind the
	// target. We apply this directly to the lateral offset of our muzzle:
	*dMuzzleOffsetX += iFinalPenalty;

}

void CalcRangeCompensationOffset( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetY, INT32 iRangeToTarget, OBJECTTYPE *pWeapon )
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// HEADROCK HAM 4: New Shooting Mechanism, Bullet Drop Compensation
	//
	// Different guns have different ranges. In the real world, a gun is usually measured by its "effective" range,
	// the range at which it can reliably put shots on a man-sized target. However, guns also have a certain
	// "maximum" range, the range at which the bullet loses enough velocity to begin tumbling, lose aerodynamic 
	// stability, and as a result come crashing into the ground. The initial speed of the bullet, its shape and size,
	// and how fast it is spinning along its axis will all determine how far it can go before this happens.
	//
	// The JA2 Bullet Mechanism already simulates this effect, by decreasing a bullet's vertical angle repeatedly
	// once it crosses its "maximum range" (as defined by twice the weapon's Range property). The bullet falls to the
	// ground with increasing rapidity, until it collides with the ground.
	//
	// NCTH uses this principle in reverse - allowing shooters to compensate for this effect and thereby increase
	// the range of their guns artificially. They do this by raising the muzzle of the gun upwards, effectively
	// aiming *above* the target. This gives the bullet higher vertical velocity, and as a result prolongs the amount
	// of time it can fall once it reaches Maximum Range, before colliding with the ground. A shot placed properly
	// this way will arch upwards and then fall directly at the target despite being outside maximum range. 
	//
	// In effect, this can only add a small fraction to the gun's maximum range, but in some cases can be crucial.
	// However, it is a remarkable feat to be able to guess the correct angle for compensation, and only the most
	// skilled shooters can do this reliably (hitting the target with reasonable frequency). Attachments can also
	// provide bonuses for doing exactly this sort of thing.

	UINT16 usGunRange = GunRange( pWeapon, pShooter );
	if (usGunRange == 0)
	{
		// Avoid division by 0.
		return;
	}
	if ((usGunRange*gGameCTHConstants.RANGE_COEFFICIENT) >= iRangeToTarget)
	{
		return;
	}

	// To begin, we calculate something called a Gravity Constant. Once a bullet reaches its maximum flight distance,
	// it will lose exactly this much vertical movement for every step. In reality, this would be related to the
	// speed the bullet is traveling in, but since bullet speed is not actually simulated by JA2, we instead use the 
	// gun's Range as a measure for how fast the bullet is. After all, guns with longer range almost invariably fire
	// faster bullets.
	FLOAT dGravity = (FLOAT)(100.0 / (usGunRange * gGameCTHConstants.GRAVITY_COEFFICIENT));

	// We now take the Range to Target and the Maximum Range for the bullet, and find the distance between them.
	// This is how much distance the bullet has to cover while dropping out of the sky. In other words, we need to
	// make the bullet fly this much longer than it would normally have.
	INT32 iRangeDiff = (iRangeToTarget-(UINT32)(usGunRange*gGameCTHConstants.RANGE_COEFFICIENT)) / 10;

	// To know how much we need to raise our muzzle, what we do is calculate how far below the target the bullet would
	// hit, assuming no muzzle adjustment, and pretending there's no ground to collide with. The result is equal to
	// exactly the amount of extra upwards angle we need to add to the muzzle to make the bullet hit the target!
	FLOAT dOptimalAddY = iRangeDiff * (iRangeDiff+1) * (dGravity/2);
	if ( pShooter->pathing.bLevel && pShooter->bTargetLevel == 0 )
	{
		dOptimalAddY -= 25.6f;
		if (dOptimalAddY < 0)
		{
			return;
		}
	}

	// The next step is to figure out how skilled our shooter is. Correctly compensating for gravity is not a simple
	// matter, and a skilled shooter will have a far greater chance of guessing the extra muzzle angle required.
	FLOAT iCombinedSkill = gGameCTHConstants.MOVEMENT_MRK * EffectiveMarksmanship( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_WIS * EffectiveWisdom( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_DEX * EffectiveDexterity( pShooter );
	iCombinedSkill += gGameCTHConstants.MOVEMENT_EXP_LEVEL * (EffectiveExpLevel( pShooter ) * 10);

	// Average
	FLOAT iDivisor = gGameCTHConstants.MOVEMENT_MRK + gGameCTHConstants.MOVEMENT_WIS + gGameCTHConstants.MOVEMENT_DEX + gGameCTHConstants.MOVEMENT_EXP_LEVEL;
	iCombinedSkill /= iDivisor;

	// The weapon and/or its attachments can increase our skill by a certain percentage. Windage sights on long-range
	// weapons (like sniper rifles and launchers) are sometimes designed specifically for this purpose.
	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	FLOAT moda = (iCombinedSkill * GetDropCompensationModifier( pWeapon, stance )) / 100;
	FLOAT modb = (iCombinedSkill * GetDropCompensationModifier( pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight )) / 100;
	iCombinedSkill += ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);

	// Limit this to a scale of 0-100.
	UINT32 uiCombinedSkill = (UINT32)__max(__min(iCombinedSkill,100), 0);
	
	// Invert the value. Now 100=worst, 0=best.
	uiCombinedSkill = 100-uiCombinedSkill;

	// Now, let's randomly decide how far "off" our shooter is from the perfect angle:

	// Randomize a number within the combined skill limit.
	FLOAT iRandom = (FLOAT)(PreRandom(uiCombinedSkill) / 100.0);

	// Randomly decide whether we're overcompensating (aiming too high) or undercompensating (aiming too low).
	INT8 uiUpDown = PreRandom(2)?(1):(-1);

	// We add the randomal deviation to the perfect angle thusly:
	dOptimalAddY += (iRandom * dOptimalAddY * uiUpDown);

	// Finally, we add this to our current Vertical Muzzle Offset value. In essence, we are now aiming at a point
	// a certain distance directly above the target.
	*dMuzzleOffsetY += dOptimalAddY;

}


void CalcMuzzleSway( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, FLOAT iAperture )
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: New Shooting Mechanism, Muzzle Sway
	//
	// In the new system, our "CTH" is replaced with a value called "Muzzle Sway". It represents
	// a randomal sway of our gun's muzzle as we pull the trigger. The better our aim, the less our
	// muzzle might sway from the target's center when we take the shot.
	//
	// To determine how far a shot can go from the center, we have the Shot Aperture value, calculated earlier
	// in the calling function and fed to us here. Imagine it as a circle drawn around the target, with a specific
	// radius, within which our muzzle may point anywhere.
	// This only tells us how far our muzzle MIGHT deviate from the center. How much it actually deviates is 
	// completely randomal, allowing perfect (0 deviation) shots to occur even if the Shot Aperture circle is 
	// gigantic. Of course, the smaller the Aperture, the more likely we are to deviate only a small distance (or none).

	// Start by randomizing the general distance of the muzzle point from the center of the target.
	// HEADROCK HAM 5: Using a better distribution system for random values.
	FLOAT RandomMuzzleSway = sqrt((FLOAT)PreRandom( 1000 ) / (FLOAT)1000);
	
	// Randomize an angle of deviation, in Radians.
	FLOAT dRandomAngleRadians = (FLOAT)(PreRandom(6283) / 1000.0f);

	// Trigonometry!
	FLOAT dDeltaX = (sin(dRandomAngleRadians) * RandomMuzzleSway) * iAperture;
	FLOAT dDeltaY = (cos(dRandomAngleRadians) * RandomMuzzleSway) * iAperture;

	// The new Vertical Bias constant turns circular deviation into an ellipse. This can be used to reduce the distance
	// of up/down deviation. To balance things out, lateral (left/right) deviation is increased by the same proportion.
	// Vertical bias is applied based on stance. You get a flatter ellipse when prone, and no ellipse (a circle) when standing.
	FLOAT dVerticalBias = gGameCTHConstants.VERTICAL_BIAS;
	switch (gAnimControl[ pShooter->usAnimState ].ubEndHeight)
	{
		case ANIM_STAND:
			dVerticalBias = 1.0;
			break;
		case ANIM_PRONE:
			// No change - full bias.
			break;
		case ANIM_CROUCH:
			dVerticalBias = 1.0f + ((dVerticalBias - 1.0f) * 0.66f);
			break;
	}

	// Now that we have the distance on both axes, we can apply it to the Muzzle Offset value which we were fed
	// from the calling function.

	*dMuzzleOffsetX += dDeltaX;
	*dMuzzleOffsetY += dDeltaY * dVerticalBias;
}

FLOAT CalcBulletDeviation( SOLDIERTYPE *pShooter, FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, OBJECTTYPE *pWeapon, UINT32 uiRange )
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: New Shooting Mechanism, Bullet Deviation
	//
	// The new shooting system separates CTH into two parts - one where the shooter tries to perfect his aim
	// using extra aiming and a scope, and relying heavily on skills and shooter's condition. This determines
	// how far off the target the muzzle of the gun might point when the trigger is pulled.
	//
	// The second part, which is calculated here, is a second, marginal deviation caused by the gun itself.
	// In other words, the gun's accuracy determines how far the bullet might fly off the point at which
	// it was fired.
	//
	// This Bullet Deviation is largely static - it depends only on the type of gun we're using, its
	// condition, and possibly the type of ammo or attachments we're using. For the most part, there's nothing
	// the player can do to reduce bullet deviation, so it acts as a sort of uncontrollable extra element
	// in shot randomness. In other words, unless you switch to a different gun, you're stuck with some random
	// factor.
	//
	// Longer rifles have higher accuracy, and therefore will put the bullet closer to where it was aimed.
	// Shorter pistols and SMGs will cause a relatively large deviation of the bullet, making it harder to
	// properly hit the target at a distance, even when properly aiming the gun.
	//
	// Attachments or ammo that increase/decrease the gun's accuracy value will have a direct effect here.
	// Guns with bad condition will also tend to send bullets in unwanted directions. Other than this,
	// there's very little the player can do at all about it.

	// We start by reading the gun's Accuracy value. We'll use that as the basis for everything else.

	// Flugente: determine used gun
	OBJECTTYPE* pObjAttHand = pShooter->GetUsedWeapon( &pShooter->inv[ pShooter->ubAttackingHand ] );

	INT16 sAccuracy = GetGunAccuracy( pWeapon );
	UINT16 sEffRange = Weapon[Item[pObjAttHand->usItem].ubClassIndex].usRange + GetRangeBonus( pObjAttHand );

	// WANNE: I got a CTD in a multiplayer test game, because sEffRange was 0 (division to zero).
    // I don't know why this happend?
	FLOAT iRangeRatio = 1.0f;
	if (sEffRange > 0)
	{
		iRangeRatio = __max(1.0f, (FLOAT)(uiRange / sEffRange));
	}
	
	// This value can be anywhere between 0 and 100. Let's make sure.
	sAccuracy = __max(0, sAccuracy);
	sAccuracy = __min(100, sAccuracy);

	// Now we figure out a radius for our deviation circle. Similar to Muzzle Sway, shots will be
	// able to go anywhere within the circle drawn by this radius. The maximum possible radius
	// is defined by an INI setting. We then take the inverted accuracy value and use that as a
	// percentage to increase or decrease the radius. A gun with accuracy 0 will produce a fully-sized
	// deviation circle (100% radius), while a gun with accuracy 100 produces a minimal sized
	// circle (actually, a dot with 0% radius).
	FLOAT iBulletDev = (gGameCTHConstants.MAX_BULLET_DEV * (100-sAccuracy)) / 100;
	if (gGameCTHConstants.RANGE_EFFECTS_DEV == TRUE)
		iBulletDev *= iRangeRatio;
	iBulletDev /= 2;	// This compensates for the difference between CellXY and ScreenXY 

	// If the radius is 0, this gun's bullets are flawless, and will never deviate from where
	// they were fired.
	if (iBulletDev == 0)
	{
		return (iBulletDev);
	}

	// So now, iBulletDev is a maximum deviation for any bullet coming out of this weapon.
	// Next we randomly determine how far away this bullet will fly.

	// HEADROCK HAM 5: Using a better formula to distribute shots evenly within the deviation
	// aperture.

	// Randomize a deviation as a value between 0.0 and 1.0, then sqrt to give an even dispersal.
	FLOAT dRandomDeviation = (FLOAT)((FLOAT)PreRandom(1000) / 1000.f);

	// Randomize an angle of deviation, in Radians.
	FLOAT dRandomAngleRadians = (FLOAT)(PreRandom(6283) / 1000.0f);

	// Trigonometry!
	FLOAT dDeltaX = (sin(dRandomAngleRadians) * dRandomDeviation * iBulletDev);
	FLOAT dDeltaY = (cos(dRandomAngleRadians) * dRandomDeviation * iBulletDev);

	// Over increasing range, a bullet fired with the same deviation should actually end up
	// further away from the target than if the target was closer. We use the constant
	// "Normal Shooting Distance" to determine a "normal" range at which deviation is not multiplied
	// or divided. At twice that range, bullets will end up twice further from the center of the target. 
	// At half this range, they'll end up half as far from the center of the target.
	FLOAT iDistanceRatio = (FLOAT)((FLOAT)uiRange / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);
	dDeltaX *= iDistanceRatio;
	dDeltaY *= iDistanceRatio;

	// Finally, adjust the center point of our shot to correspond with the deviation.
	*dShotOffsetX += dDeltaX;
	*dShotOffsetY += dDeltaY;

	return (iBulletDev);
}

void LimitImpactPointByFacing( SOLDIERTYPE *pShooter, SOLDIERTYPE *pTarget, FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, FLOAT *dEndX, FLOAT *dEndY )
{
	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// CHRISL HAM 4: New Shooting Mechanism, Angle Limits by Facing
	//
	// One drawback of the new shooting mechanism is that it depends on the the JSD files being accurate.  These
	// files determine how an obstruction (like an enemy soldier) takes up space in a 3D tile.  This is done by
	// splitting a tile into 125 "cubes", stacked 5 x 5 x 5.  Each "cube" is 2 units wide, 2 units deep and about
	// 6 units tall.  A standing merc in the JSD files is represented by a "plus sign" shaped construct that is
	// 3 "cubes" tall, 3 "cubes" wide and 1 "cube" deep.  A crouching soldier is 2 x 3 x 2 and a prone soldier is
	// 1 x 3 x 5.  This means if we're shooting at a standind target from the side, we effectively have 1/3 the
	// chance to hit as we would a target that was facing us.  Unfortunately, the graphical cursor can't display
	// this information so it will appear as though you have the same chance of hitting regardless of the targets
	// facing.
	//
	// As a result of the above, this function determines the targets facing compared to the shooters, and will
	// reduce the Shot Offsets if the target isn't facing directly towards or away from the shooter.  This isn't
	// the most accurate solution but it's the best solution I can come up with at this time.  The alternative is
	// to let shots that appear to have no chance of missing (shooting aperture within the targets displayed body)
	// actually miss an average of 2 in every 3 shots.
	//
	// I'm going to assume for the moment that the 1/3rd chance only occurs when a target is facing directly perpendicular
	// to your facing.  I don't know for sure if this is true or not, but it's the assumption I'm going to make.
	// 
	////////////////////////////////////////////////////////////////////////////////////////////

	UINT8	iShooterFacing = pShooter->ubDirection;
	UINT8	iTargetFacing = pShooter->ubDirection;
	FLOAT	iDivisor = gGameCTHConstants.SIDE_FACING_DIVISOR;

	if(pTarget)
		iTargetFacing = pTarget->ubDirection;

	switch (iTargetFacing)
	{
		case 0:
			if(iShooterFacing == 2 || iShooterFacing == 6){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 1:
			if(iShooterFacing == 3 || iShooterFacing == 7){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 2:
			if(iShooterFacing == 0 || iShooterFacing == 4){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 3:
			if(iShooterFacing == 1 || iShooterFacing == 5){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 4:
			if(iShooterFacing == 2 || iShooterFacing == 6){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 5:
			if(iShooterFacing == 3 || iShooterFacing == 7){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 6:
			if(iShooterFacing == 0 || iShooterFacing == 4){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		case 7:
			if(iShooterFacing == 1 || iShooterFacing == 5){
				*dShotOffsetX /= iDivisor;
				*dShotOffsetY /= iDivisor;
			}
			break;
		default:
			break;
	}
}

void LimitImpactPointToMaxAperture( FLOAT *dShotOffsetX, FLOAT *dShotOffsetY, FLOAT dDistanceAperture )
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// HEADROCK HAM 4: New Shooting Mechanism, Angle Limits
	//
	// The new Shooting Mechanism works primarily by (virtually) never firing the bullet directly at the target's
	// center. It does so by moving the "target point" for the bullet a certain distance from the target's
	// center, as dictated by the functions preceding this one in the formula. However, this has the potential
	// of making particularly-bad shots go off in very awkward directions, such as sideways or upwards, given
	// that the center point can be adjusted considerably.
	//
	// To make sure this doesn't happen, this function limits all shots so that they never have neither a horizontal
	// nor vertical angle greater than X (defined by the INI file). By default, this angle is 22.5 - the maximum
	// possible angle to avoid a bullet flying outside the 45-degree cone in front of the shooter.
	//
	// If this INI-defined angle is decreased, it will proportionally make all shots more accurate, so take that
	// under advisement. If that occurs, this formula will also make sure to keep shots within that smaller cone.
	//
	// Thankfully, by this point we already have the value called "Shot Aperture" calculated. This is the MAXIMUM
	// possible deviation of any shot at Normal Distance. We just need to make sure that our current shot doesn't
	// have a greater deviation than this.

	FLOAT dReductionRatio = 1.0;

	// Calculate how far from the target center will our shot impact. This is done by simple pythagorean formula.
	FLOAT dTotalShotOffset = sqrt((*dShotOffsetX * *dShotOffsetX)+(*dShotOffsetY * *dShotOffsetY));

	// If our shot deviates more than the aperture allows, at the target's distance...
	if (dTotalShotOffset > dDistanceAperture )
	{
		// Find out the ratio between the deviation and the limit.
		dReductionRatio = dDistanceAperture / dTotalShotOffset;

		// Multiply both offsets by this ratio. This will effectively move them back into the aperture radius
		// without altering the ratio between vertical and horizontal offsets.
		*dShotOffsetX *= dReductionRatio;
		*dShotOffsetY *= dReductionRatio;
	}
}

UINT32 CalcCounterForceFrequency(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon)
{
	INT8	traitLoop;

	FLOAT iCounterForceFrequency = gGameCTHConstants.RECOIL_COUNTER_FREQUENCY_AGI * EffectiveAgility(pShooter);
	iCounterForceFrequency += gGameCTHConstants.RECOIL_COUNTER_FREQUENCY_EXP_LEVEL * EffectiveExpLevel(pShooter) * 10;

	// Average
	FLOAT iDivisor = gGameCTHConstants.RECOIL_COUNTER_FREQUENCY_AGI + 
						gGameCTHConstants.RECOIL_COUNTER_FREQUENCY_EXP_LEVEL;
	iCounterForceFrequency /= iDivisor;

	// Bridge the gap to 100 with the help of the AUTO-WEAPONS skill
	if(gGameOptions.fNewTraitSystem)
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPONS_NT );
	else
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPS_OT );
	for (INT32 x = 0; x < traitLoop; x++)
	{
		INT8 bDifference = (INT8)(100-iCounterForceFrequency);
		bDifference = (INT8)(bDifference / gGameCTHConstants.RECOIL_COUNTER_FREQUENCY_AUTO_WEAPONS_DIVISOR);
		iCounterForceFrequency += bDifference;
	}

	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	// Percent Modifier from weapon and its attachments
	FLOAT moda = (iCounterForceFrequency * GetCounterForceFrequencyModifier( pWeapon, stance )) / 100;
	FLOAT modb = (iCounterForceFrequency * GetCounterForceFrequencyModifier( pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight )) / 100;
	iCounterForceFrequency += ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);

	// Limit to 1-100.
	iCounterForceFrequency = __min(100, iCounterForceFrequency);
	iCounterForceFrequency = __max(1, iCounterForceFrequency);

	// Invert value. Now 100 is bad, 1 is good.
	iCounterForceFrequency = 100 - iCounterForceFrequency;

	// Calculate frequency
	UINT32 uiBulletsPer5AP = max(1,GetAutofireShotsPerFiveAPs( pWeapon ));
	UINT32 uiCounterForceFrequency = __max(1, (UINT32)ceil((uiBulletsPer5AP * iCounterForceFrequency) / 100));

	//CHRISL: For the time being, this is going to override the CFF function
	uiBulletsPer5AP = GetAutofireShotsPerFiveAPs( pWeapon );
	if(uiBulletsPer5AP < 1)
		uiBulletsPer5AP = 3;
	uiCounterForceFrequency = (UINT32)ceil((FLOAT)uiBulletsPer5AP / 2.0f);

	return uiCounterForceFrequency;
}

FLOAT CalcCounterForceMax(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, UINT8 uiStance)
{
	if(uiStance == 0)
		uiStance = gAnimControl[ pShooter->usAnimState ].ubHeight;

	FLOAT iCounterForceMax = gGameCTHConstants.RECOIL_MAX_COUNTER_STR * EffectiveStrength(pShooter);
	iCounterForceMax += gGameCTHConstants.RECOIL_MAX_COUNTER_AGI * EffectiveAgility(pShooter);
	iCounterForceMax += gGameCTHConstants.RECOIL_MAX_COUNTER_EXP_LEVEL * EffectiveExpLevel(pShooter) * 10;

	FLOAT iDivisor = gGameCTHConstants.RECOIL_MAX_COUNTER_STR + 
						gGameCTHConstants.RECOIL_MAX_COUNTER_AGI + 
						gGameCTHConstants.RECOIL_MAX_COUNTER_EXP_LEVEL;
	iCounterForceMax /= iDivisor;

	// Add the effects from the weapon and its attachments (foregrip, for instance). This is applied as a percentage.
	INT32 iModifier = GetCounterForceMaxModifier( pWeapon, uiStance );
	iCounterForceMax += (iCounterForceMax * iModifier) / 100;

	// Limit to 0-100.
	//iCounterForceMax = __max(__min(iCounterForceMax, 100), 0);

	// Use the result as a percentage of the maximum possible applicable force, as defined in the INI.
	iCounterForceMax = (FLOAT)((iCounterForceMax * gGameCTHConstants.RECOIL_MAX_COUNTER_FORCE) / 100);

	return iCounterForceMax;
}

UINT32 CalcCounterForceAccuracy(SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, UINT32 uiRange, BOOLEAN fTracer, bool fAnticipate)
{
	INT8	traitLoop;

	FLOAT iCounterForceAccuracy = gGameCTHConstants.RECOIL_COUNTER_ACCURACY_DEX * EffectiveDexterity(pShooter);
	iCounterForceAccuracy += gGameCTHConstants.RECOIL_COUNTER_ACCURACY_WIS * EffectiveWisdom(pShooter);
	iCounterForceAccuracy += gGameCTHConstants.RECOIL_COUNTER_ACCURACY_AGI * EffectiveAgility(pShooter);
	iCounterForceAccuracy += gGameCTHConstants.RECOIL_COUNTER_ACCURACY_EXP_LEVEL * EffectiveExpLevel(pShooter) * 10;

	FLOAT iDivisor = gGameCTHConstants.RECOIL_COUNTER_ACCURACY_DEX + 
						gGameCTHConstants.RECOIL_COUNTER_ACCURACY_WIS + 
						gGameCTHConstants.RECOIL_COUNTER_ACCURACY_AGI + 
						gGameCTHConstants.RECOIL_COUNTER_ACCURACY_EXP_LEVEL;
	iCounterForceAccuracy /= iDivisor;

	// Add the effects from the weapon and its attachments. A foregrip or bipod are very useful for this.
	// Attachment bonuses are applied as a percentage to the accuracy of the shooter.

	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	INT32 moda = GetCounterForceAccuracyModifier( pWeapon, stance );
	INT32 modb = GetCounterForceAccuracyModifier( pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight );
	INT32 iModifier = (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);

	UINT32 uiCounterForceAccuracy = (UINT32)(iCounterForceAccuracy + ((iCounterForceAccuracy * iModifier) / 100));

	// Now add the effect of the AutoWeapons skill. It "bridges" a portion of the gap between shooter's actual accuracy 
	// and 100% accuracy. For instance, if the divisor is set to 2 in the INI, the first skill level will close 50% of 
	// the gap, the second skill level closes another 25%.
	if(gGameOptions.fNewTraitSystem)
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPONS_NT );
	else
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPS_OT );
	for (INT32 x = 0; x < traitLoop; x++)
	{
		INT8 bDifference = 100-uiCounterForceAccuracy;
		bDifference = (INT8)(bDifference / gGameCTHConstants.RECOIL_COUNTER_ACCURACY_AUTO_WEAPONS_DIVISOR);
		uiCounterForceAccuracy += bDifference;
	}

	////////////////////////////////////////////////////////////////////
	// Sight-range effects
	// If we can't see the target, but buddies can see it, CF-Accuracy drops by 50%
	// If we can't see the target and neither can buddies, CF-Accuracy drops by 75%

	UINT8 ubTargetID = WhoIsThere2( pShooter->sTargetGridNo, pShooter->bTargetLevel ); // Target ubID
	INT16 sDistVis = pShooter->GetMaxDistanceVisible(pShooter->sTargetGridNo, pShooter->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;
	gbForceWeaponNotReady = true;
	INT16 sDistVisNoScope = pShooter->GetMaxDistanceVisible(pShooter->sTargetGridNo, pShooter->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;
	gbForceWeaponNotReady = false;
	FLOAT scopeRangeMod = (float)sDistVis / (float)sDistVisNoScope;

	INT32 iSightRange = 0;
	if (ubTargetID != NOBODY)
		iSightRange = SoldierToSoldierLineOfSightTest( pShooter, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pShooter->bAimShotLocation, false );
	if (iSightRange == 0) {	// didn't do a bodypart-based test or can't see specific body part aimed at
		iSightRange = SoldierTo3DLocationLineOfSightTest( pShooter, pShooter->sTargetGridNo, pShooter->bTargetLevel, pShooter->bTargetCubeLevel, TRUE, NO_DISTANCE_LIMIT, false );
	}
	if (iSightRange == 0) {	// Can't see the target but we still need to know what the sight range would be if we could so we can deal with cover penalties
		iSightRange = SoldierToSoldierLineOfSightTest( pShooter, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pShooter->bAimShotLocation, false, true );
	}

	// Modify iSightRange for scope use
	iSightRange = (INT32)(iSightRange / scopeRangeMod);

	if (pShooter->aiData.bOppList[ubTargetID] != SEEN_CURRENTLY && gbPublicOpplist[pShooter->bTeam][ubTargetID] == SEEN_CURRENTLY)
		iSightRange *= 2;
	else if(pShooter->aiData.bOppList[ubTargetID] != SEEN_CURRENTLY && gbPublicOpplist[pShooter->bTeam][ubTargetID] != SEEN_CURRENTLY)
		iSightRange *= 4;

	// CHRISL: Divide physical range by sight range.  Not the other way around.  We wan't CFA to go down if someone appears to be further
	//	then they are because of terrain or the fact that we can't see them.
	uiCounterForceAccuracy = (UINT32)(uiCounterForceAccuracy * (FLOAT)((FLOAT)uiRange / iSightRange));

	// Was last bullet a tracer?
	if (fTracer)
	{
		// Augment by a certain amount, mitigated by range
		uiCounterForceAccuracy += (UINT32)gGameCTHConstants.RECOIL_COUNTER_ACCURACY_TRACER_BONUS;
	}

	if(fAnticipate)
	{
		uiCounterForceAccuracy -= (UINT32)(uiCounterForceAccuracy * gGameCTHConstants.RECOIL_COUNTER_ACCURACY_ANTICIPATION / 100);
	}

	// Limit to 0 to 100.
	uiCounterForceAccuracy = __max(__min(uiCounterForceAccuracy, 100), 0);

	// Invert. 0=good, 100-bad.
	uiCounterForceAccuracy = 100-uiCounterForceAccuracy;

	return uiCounterForceAccuracy;
}

void CalcPreRecoilOffset( SOLDIERTYPE *pShooter, OBJECTTYPE *pWeapon, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, UINT32 uiRange )
{
	//////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: New Shooting Mechanism, Pre-Recoil Calculation
	//
	// The new recoil system works on the premise that each gun has its own recoil force
	// and direction. After X shots with enough recoil, the muzzle of the gun will be pointing
	// a certain distance off the center of the target. After a few bullets have been fired,
	// mercs get a chance to begin applying counter-force to act against recoil, and given
	// enough bullets might even be able to push their gun back towards the target's center.
	// However, by this point several bullets will have been wasted, and the muzzle may already
	// be pointing very far from the target's center.
	//
	// To compensate for this, experienced characters can anticipate the recoil of their gun
	// before it even starts firing. Instead of aiming the first shot at the center of the
	// target, these characters will aim it at a location which would, once recoil kicks in,
	// rake the gun over the target. This minimizes the distance by which the last shot will
	// be offset from the target's center, making it easier to return the muzzle towards the
	// center once the character gets the chance to start applying Counter-Force.
	//
	// To visualize this, imagine a gun like an AK-47, which pulls very strongly upwards.
	// Experienced soldiers know that if they aim at a target's legs, the first bullets will
	// then "rake" upwards, hitting the target in the legs, abdomen, torso, and possibly the
	// head. By this point, if they continue autofiring, they'll be able to get control
	// of the gun and keep it pointed at least close to the torso. If their volley had started
	// while pointed at the torso, they'd be firing above the target by now...

	FLOAT dOffsetX = 0.0f;
	FLOAT dOffsetY = 0.0f;
	FLOAT dCounterForceX = 0.0f;
	FLOAT dCounterForceY = 0.0f;
	
	UINT32 uiIntendedBullets;
	if ( pShooter->bDoAutofire > 0 )
	{
		// Autofire. The number of bullets to be fired equals to the value of bDoAutofire
		uiIntendedBullets = pShooter->bDoAutofire;
	}
	else
	{
		// Burst. Get the number of shots-per-burst from the weapon.
		uiIntendedBullets = Weapon[ pWeapon->usItem ].ubShotsPerBurst + GetBurstSizeBonus(pWeapon);
	}
	if (uiIntendedBullets == 1)
	{
		// Only one bullet being fired. Forget the rest.
		return;
	}

	// Calculating Pre-Recoil Offset is fairly simple. We begin by determining the chance of
	// this soldier to actually consider using pre-recoil offset at all. This depends entirely
	// on wisdom and experience.

	// We can also anticipate the recoil of the weapon, which we should set here.

	INT8 bGunRecoilX;
	INT8 bGunRecoilY;

	// Get the gun's recoil values for our first bullet.
	GetRecoil( pShooter, pWeapon, &bGunRecoilX, &bGunRecoilY, 1 );

	// Calculate the Distance Ratio for later use.
	FLOAT dDistanceRatio = (FLOAT)uiRange / (FLOAT)gGameCTHConstants.NORMAL_RECOIL_DISTANCE;

	// Calculate the various counter-force related values for our shooter.
	FLOAT dCounterForceMax = CalcCounterForceMax(pShooter, pWeapon);

	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	FLOAT moda = CalcCounterForceMax(pShooter, pWeapon, stance);
	FLOAT modb = CalcCounterForceMax(pShooter, pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight);
	FLOAT iCounterForceMax = ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);
	
	UINT32 uiCounterForceAccuracy = CalcCounterForceAccuracy(pShooter, pWeapon, uiRange, FALSE, true);

	////////////////////////////////////////////////////////////
	// Calculate the soldier's expertise to determine whether he's smart enough to compensate
	// for recoil before it begins at all!
	INT8	traitLoop;
	FLOAT iCombinedSkill = gGameCTHConstants.PRE_RECOIL_WIS * EffectiveWisdom(pShooter);
	iCombinedSkill += gGameCTHConstants.PRE_RECOIL_EXP_LEVEL * EffectiveExpLevel(pShooter) * 10;

	// Average
	FLOAT iDivisor = gGameCTHConstants.PRE_RECOIL_WIS + 
						gGameCTHConstants.PRE_RECOIL_EXP_LEVEL;
	iCombinedSkill /= iDivisor;

	// AUTO-WEAPONS skill is used to bridge the gap between Combined Skill and 100%. Each level of the skill
	// adds a fraction of the distance, as defined by the INI file.
	if(gGameOptions.fNewTraitSystem)
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPONS_NT );
	else
		traitLoop = NUM_SKILL_TRAITS( pShooter, AUTO_WEAPS_OT );
	for (INT32 x = 0; x < traitLoop; x++)
	{
		INT8 bDifference = (INT8)(100-iCombinedSkill);
		bDifference = (INT8)(bDifference / gGameCTHConstants.PRE_RECOIL_AUTO_WEAPONS_SKILL);
		iCombinedSkill += bDifference;
	}

	// Limit to a 0-100 scale, just in case.
	UINT32 uiPreRecoilAbility = (UINT32)__min(100, __max(iCombinedSkill, 0));

	// End skill calculation.
	/////////////////////////////////////////////////////////

	// HEADROCK HAM 5: We'll begin by checking whether the soldier was smart/experienced enough
	// to aim AWAY from the target's center before firing his volley. By aiming low and to the left, for
	// example, the soldier should have an easier time leading his volley into the target with
	// subsequent bullets (assuming the gun is pulling up and right, of course). 
	// We can adjust by up to one recoil-length away - not much, but helpful.

	// Randomly determine whether the soldier has compensated by moving his muzzle.
	/*
	if (PreRandom(100) < uiPreRecoilAbility)
	{
		// The soldier is smart enough to compensate for recoil by not aiming for the center of the target.
		// He will attempt to adjust his aim by one recoil's length, causing the second bullet to impact
		// with the target's center. This will increase controllability of the remainder of the volley.
	
		FLOAT dIdealOffsetX = (FLOAT)-bGunRecoilX;
		FLOAT dIdealOffsetY = (FLOAT)-bGunRecoilY;

		// Naturally, getting it right is remarkably tricky. We might undercompensate or overcompensate!
		// Our pre-recoil ability will help us do this more accurately.
		FLOAT dMaxError = 0.0f;
		FLOAT dError = 0.0f;
		INT8 bErrorDirection = 0;

		// FOR X:
			dMaxError = (dIdealOffsetX * (100-uiPreRecoilAbility)) / 100;
			dError = ((FLOAT)Random((INT32)(abs(dMaxError) * 1000))) / 1000.0f;
			bErrorDirection = Random(2) ? 1 : (-1);
			// Set final muzzle compensation
			dOffsetX = dIdealOffsetX + (dError * bErrorDirection);
		// FOR Y:
			dMaxError = (dIdealOffsetY * (100-uiPreRecoilAbility)) / 100;
			dError = ((FLOAT)Random((INT32)(abs(dMaxError) * 1000))) / 1000.0f;
			bErrorDirection = Random(2) ? 1 : (-1);
			// Set final muzzle compensation
			dOffsetY = dIdealOffsetY + (dError * bErrorDirection);
	}
	*/

	// Randomly determine whether the soldier has compensated by applying counter-force.
	if (PreRandom(100) < uiPreRecoilAbility)
	{
		// HEADROCK HAM 5: Since we can now alter CF with every shot, but only by a small amount, we can allow
		// our shooter to "anticipate" recoil by applying at least part of the required CF to stabilize the
		// muzzle.

		// First lets figure out what the "ideal" CF would be. Ideal CF application would result in the muzzle
		// moving towards the 0,0 coordinate (target center) with some velocity, but not more velocity than it
		// would take to stabilize the muzzle for our next shot.

		// The basic ideal is CF = -Recoil. If we can achieve this, then our muzzle will be completely stationary
		// after the first bullet is fired, allowing us to adjust CF as we please during the following CF check.
		FLOAT dIdealCounterForceX = (FLOAT)-bGunRecoilX;
		FLOAT dIdealCounterForceY = (FLOAT)-bGunRecoilY;

		// But we also want the volley to be moving at least somewhat toward the target. Therefore, lets figure
		// out where our muzzle is, and adjust in that direction by as much force as could be undone by the next
		// bullet.

		// Maximum velocity change. This is how fast the muzzle can move while still being easy to stop moving
		// within one bullet cycle.
		FLOAT dMaxChange = gGameCTHConstants.RECOIL_COUNTER_INCREMENT;
		
		// For X:
		INT8 bMuzzleDirectionX = (INT8)(*dMuzzleOffsetX / abs(*dMuzzleOffsetX));
		dIdealCounterForceX += (-1) * bMuzzleDirectionX * dMaxChange;
		// For Y:
		INT8 bMuzzleDirectionY = (INT8)(*dMuzzleOffsetY / abs(*dMuzzleOffsetY));
		dIdealCounterForceY += (-1) * bMuzzleDirectionY * dMaxChange;
		
		// Lets figure out how close we come to actually applying this exact amount of CF?
		FLOAT dMaxError = 0.0f;
		FLOAT dError = 0.0f;
		INT8 bErrorDirection = 0;

		// FOR X:
			dMaxError = (dIdealCounterForceX * (100-uiPreRecoilAbility)) / 100;
			dError = ((FLOAT)Random((INT32)(abs(dMaxError) * 1000))) / 1000.0f;
			bErrorDirection = Random(2) ? 1 : (-1);
			// Set final counter-force
			dCounterForceX = dIdealCounterForceX + (dError * bErrorDirection);
		// FOR Y:
			dMaxError = (dIdealCounterForceY * (100-uiPreRecoilAbility)) / 100;
			dError = ((FLOAT)Random((INT32)(abs(dMaxError) * 1000))) / 1000.0f;
			bErrorDirection = Random(2) ? 1 : (-1);
			// Set final counter-force
			dCounterForceY = dIdealCounterForceY + (dError * bErrorDirection);		

		// We obviously can't apply more than our shooter's Max Counter-Force, so lets use Pythagorean Theorem
		// to "nerf" counterforce on both axes to the given limit.
		FLOAT dTotalCounterForce = sqrt((abs(dCounterForceX)*abs(dCounterForceX)) + (abs(dCounterForceY)*abs(dCounterForceY)));
		if (dTotalCounterForce > dCounterForceMax)
		{
			FLOAT dRatio = dTotalCounterForce / dCounterForceMax;
			if (dRatio != 0)
			{
				dCounterForceX /= dRatio;
				dCounterForceY /= dRatio;
			}
		}
	}

	// So far we've been calculating everything at NORMAL distance, so lets apply distance to the target
	// to convert this into actual muzzle point deviation.
	dOffsetX *= dDistanceRatio;
	dOffsetY *= dDistanceRatio;

	// Adjust our muzzle point!
	*dMuzzleOffsetX += dOffsetX;
	*dMuzzleOffsetY += dOffsetY;

	// Also, lets set our shooter's Counter Force variables. These will apply for the next bullet in the
	// volley.
	pShooter->dPrevCounterForceX = dCounterForceX;
	pShooter->dPrevCounterForceY = dCounterForceY;

}

// Calculate Recoil Offset
void CalcRecoilOffset( SOLDIERTYPE *pShooter, FLOAT *dMuzzleOffsetX, FLOAT *dMuzzleOffsetY, OBJECTTYPE *pWeapon, UINT32 uiRange )
{
	//////////////////////////////////////////////////////////////////////////////////////
	// Recoil Calculation
	//
	// Where the old CTH system used recoil as a flat penalty to CTH after each bullet, the new system
	// handles things completely differently. It actually gives each bullet an extra deviation in a specific
	// direction away from the center of the target, which is based entirely on the parameters of the gun itself.
	// Various guns can "pull" in different directions and with different force, all defined in the XML entries.
	// Ammo and Attachments, such as bipods, can reduce this offset.
	//
	// In addition, the shooter's skills allow him or her to compensate for the recoil by pulling the gun back
	// towards the target. This is done by applying a certain amount of counter-force. The shooter has to guesstimate
	// the amount of force that will be required to put the muzzle back on the target, and is limited in how much
	// counter force he/she can apply per bullet. Better shooters will be able to compensate correctly, while bad
	// shooters might be overwhelmed and start firing towards the sky.

	// Get X/Y recoil parameters from the gun.
	INT8 bGunRecoilX;
	INT8 bGunRecoilY;

	GetRecoil( pShooter, pWeapon, &bGunRecoilX, &bGunRecoilY, pShooter->bDoBurst-1 );

	// If no recoil, then we shouldn't be here anyway.
	if(bGunRecoilX == 0 && bGunRecoilY == 0)
		return;

	FLOAT dDistanceRatio = (FLOAT)uiRange / (FLOAT)gGameCTHConstants.NORMAL_RECOIL_DISTANCE;

	// These variables will hold the amount of X/Y force our shooter exerts to try to fight recoil.
	FLOAT dAppliedCounterForceX = pShooter->dPrevCounterForceX;
	FLOAT dAppliedCounterForceY = pShooter->dPrevCounterForceY;

	BOOLEAN fTracer = WasPrevBulletATracer( pShooter, pWeapon );
	
	///////////////////////////////////////////////////////////////////////////////////
	// STEP 2:
	//
	// Our shooter got the chance to change the amount of force he's using to counter recoil.

	// The "Next Offset" here signifies how far off the center of the target our next bullet will be, in X/Y terms.
	// The optimal of course would be if both are equal to 0, meaning the shot will be directed towards the center.
	// However, it's up to the shooter to make sure that happens, by applying counter-force.
	// Our shooter wants to apply just as much counter-force as necessary to keep the next offset as close to 0 on both
	// axes.

	// Instead of assuming we're on a uiCounterForceFrequency round, we need to verify exactly where we are in the uiCounterForceFrequency
	//	sequence.
	INT32 uiIntendedBullets;
	if ( pShooter->bDoAutofire > 0 )
	{
		// Autofire. The number of bullets to be fired equals to the value of bDoAutofire
		uiIntendedBullets = pShooter->bDoAutofire;
	}
	else
	{
		// Burst. Get the number of shots-per-burst from the weapon.
		uiIntendedBullets = Weapon[ pWeapon->usItem ].ubShotsPerBurst + GetBurstSizeBonus(pWeapon);
	}

	////////////////////////////////////////////////////////////////////////////
	// Calculating recoil counter-force.
	//
	// The shooter needs to apply a specific amount of force in order to counter-act recoil, not just for this
	// bullet, but for all others that came before.
	//
	// To do this, the shooter needs skills. One set of skills allows the shooter to exert more counter-force. A
	// shooter with insufficient strength will be unable to counter-act the recoil from a powerful gun. A shooter
	// with high strength can potentially hold the gun steady even while its firing.
	//
	// However, the shooter also needs a second set of skills to help him predict exactly how much force is required
	// between every bullet, and the ability to actually exert the exact amount of force required. Exerting too little
	// force leads to under-compensation: The shooter will need to exert even MORE force for the next bullet! Exerting
	// too MUCH force is also a potential problem, as the gun may miss the target on the OTHER side. Of course,
	// it's generally better to exert too much force than too little, assuming you don't repeatedly over-compensate,
	// in which case you'll be shooting at the ground.

	// STEP 1: determining how much counter-force the shooter can apply if necessary. This is the absolute
	// maximum counter-force that can be applied. By default, it is based primarily on the strength of the shooter,
	// although agility is also helpful.

	FLOAT dCounterForceMax = CalcCounterForceMax(pShooter, pWeapon);

	UINT8 stance = gAnimControl[ pShooter->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pShooter->IsWeaponMounted() )
		stance = ANIM_PRONE;

	FLOAT moda = CalcCounterForceMax(pShooter, pWeapon, stance);
	FLOAT modb = CalcCounterForceMax(pShooter, pWeapon, gAnimControl[ pShooter->usAnimState ].ubEndHeight);
	FLOAT iCounterForceMax = ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);
		
	// iCounterForceMax is now the absolute limit.

	// STEP 2: Now we need to determine how accurate the shooter is when applying counter-force. He won't always apply
	// as much as necessary, and may sometimes apply too much. The ability to apply exactly (or close to exactly) the
	// amount of force required is based on various skills, especially agility and dexterity. Wisdom and experience also
	// help, and the AUTO_WEAPONS skill is invaluable. In addition, tracers (when actually fired) will boost the
	// ability to compensate correctly by a certain amount.

	UINT32 uiCounterForceAccuracy = CalcCounterForceAccuracy(pShooter, pWeapon, uiRange, fTracer);

	// STEP 3: Now let's determine how much counter-force would be ideal. If the gun isn't kicking too powerfully,
	// and/or not too much recoil has built up from previous bullets, the shooter should potentially be able to compensate
	// for ALL of it. However, in some cases, the shooter simply can't exert enough force to counter ALL recoil.
	
	// HEADROCK HAM 5: Unfortunately, In an attempt to correct a conceptual problem with calculating ideal CF,
	// ChrisL created a formula that was impossible to decypher. Supposedly, the formula would cause autofire to
	// move towards a different point than the center, then slowly correct that point. I got three different coders
	// to look at it and they couldn't see how it works. 
	// Unfortunately, short of asking Chris to redo it, I couldn't see any other way to rewrite the
	// formula so that it is legible. Instead of bothering Chris about it I decided to just rewrite the entire system
	// for additional realism, based on a superior idea I had before work on NCTH began.
	
	// Instead of being forced to use the same CF for several consecutive shots, the shooter's CF will actually 
	// increase or decrease as required with every bullet. In other words, with each bullet fired, CF can 
	// be changed. However, we enforce a given cap which is significantly lower than Max CF or any common recoil values,
	// meaning that the shooter needs to make many adjustments in order to keep the volley going in the right,
	// with inherent errors causing it to be much more difficult overall to keep the gun trained directly at the
	// target regardless of CF accuracy.

	// For more explanation read the comments inside the following function:

	// CALCULATE FOR X
	FLOAT dCounterForceChangeX = CalcCounterForceChange( pShooter, uiCounterForceAccuracy, dCounterForceMax, *dMuzzleOffsetX / dDistanceRatio, bGunRecoilX, pShooter->dPrevCounterForceX, uiIntendedBullets ); 
	// CALCULATE FOR Y
	FLOAT dCounterForceChangeY = CalcCounterForceChange( pShooter, uiCounterForceAccuracy, dCounterForceMax, *dMuzzleOffsetY / dDistanceRatio, bGunRecoilY, pShooter->dPrevCounterForceY, uiIntendedBullets ); 

	dAppliedCounterForceX += dCounterForceChangeX;
	dAppliedCounterForceY += dCounterForceChangeY;

	/////////////////////////////
	// Use pythagorean to scale this to the max force allowed.
	// We want to use pythagorean here so that we calculate the total "vector length" of recoil and compare that to CFM
	FLOAT dTotalAppliedCounterForce = sqrt((dAppliedCounterForceX*dAppliedCounterForceX) + (dAppliedCounterForceY*dAppliedCounterForceY));
	if (dTotalAppliedCounterForce > dCounterForceMax)
	{
		FLOAT dRatio = dTotalAppliedCounterForce / dCounterForceMax;
		if (dRatio != 0)
		{
			dAppliedCounterForceX /= dRatio;
			dAppliedCounterForceY /= dRatio;
		}
	}
	// This works because sqrt((iACFX^2)+(iACFY^2)) should result in being less then or equal to CFM

	// Record how much counter force was applied this time. It will be used for the next few shots until the
	// shooter can recalculate.
	pShooter->dPrevCounterForceX = dAppliedCounterForceX;
	pShooter->dPrevCounterForceY = dAppliedCounterForceY;

	dAppliedCounterForceX += (FLOAT)bGunRecoilX;
	dAppliedCounterForceY += (FLOAT)bGunRecoilY;

	// So far we've been calculating everything at NORMAL distance, so we need to apply a distance ratio to
	// multiply all muzzle movements as distance increases or decreases.
	dAppliedCounterForceX *= dDistanceRatio;
	dAppliedCounterForceY *= dDistanceRatio;

	// Offset the bullet.
	*dMuzzleOffsetX += dAppliedCounterForceX;
	*dMuzzleOffsetY += dAppliedCounterForceY;

	
}

FLOAT CalcCounterForceChange( SOLDIERTYPE * pShooter, UINT32 uiCounterForceAccuracy, FLOAT dCounterForceMax, FLOAT dMuzzleOffset, INT8 bRecoil, FLOAT dPrevCounterForce, UINT32 uiIntendedBullets )
{
	/////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 5: New Counter-Force Calculation
	//
	// This function, called from the main NCTH ballistic formula, handles the adjustment of
	// Counter Force between each and every bullet during an Autofire or Burst volley.
	//
	// Counter-Force (CF) is the power used by the shooter to control his gun as it jerks back
	// (Recoil, or Muzzle Climb). CF can be exerted to reduce recoil, it can be loosened up to let recoil
	// move the gun on its own, or it can be used to push the muzzle of the gun in any necessary direction.
	// The goal is to bring the muzzle towards that "perfect" trajectory needed to fire subsequent 
	// bullets into the exact center of the target (coordinate 0,0).
	//
	// After each bullet is fired, the muzzle moves a certain amount either towards or away
	// from the target's center - depending on the difference between Recoil and Counter Force
	// at the time. Positive movement on the X axis means the muzzle is moving towards the right,
	// negative means it is moving towards the left. For the Y axis this is up and down respectively.
	// Again, the shooter's goal is to move the muzzle from whereever it currently is pointing towards 
	// coordinates 0,0.
	// Recoil (usually) remains consistent throughout the volley, so to move the muzzle correctly to 0,0
	// our shooter must constantly adjust the amount of counter-force he is applying - affecting the
	// difference between Recoil and CF and thus generating movement in the desired direction.
	//
	// After the first bullet is fired, the shooter is already applying some counter-force, generally
	// in the direction of the target center - helping to overcome some initial recoil. From that point
	// onwards, counter-force must be adjusted after every bullet, changing the muzzle's angular velocity
	// to get the muzzle going in the right direction or slowing it down when it's getting close to the target.
	//
	// The trick is that these changes in velocity must be made in relatively small amounts - no sudden
	// changes from pushing the barrel one way to pushing it the other way, but rather gradual
	// changes that could take several "bullets" to complete. In other words, several bullets might be
	// fired while the shooter slowly changes the muzzle velocity towards the desired direction. Visually,
	// this would seem like the volley's movements decelerate, then start moving in another direction
	// (hopefully back towards the target).
	//
	// Getting the muzzle to coordinates 0,0 or thereabouts is not sufficient on its own. If the muzzle
	// is moving fast towards 0,0, it may sweep over that coordinate. Optimally, the shooter wants to
	// reach 0,0 while at the same time having the right amount of CF to hold the muzzle there against
	// the gun's recoil - resulting in the muzzle being both pointed directly at the target AND completely
	// stationary. Doing so is very hard, especially due to the program enforcing "mandatory errors" in 
	// CF application - mimicing the virtual impossibility for the human body to fully stabilize an 
	// autofiring gun. Radical errors could send the muzzle flying in some direction, wasting several 
	// more bullets on empty space while the shooter struggles to regain control.
	//
	// The formula below handles the majority of this process. It calculates how much CHANGE must be 
	// made to our current Counter-Force before the next bullet is fired. It factors in the acceleration and 
	// deceleration required to finesse the muzzle back to coordinate 0,0. It calculates how many 
	// rounds it would take to get there. It also enforces both a maximum amount of change per 
	// bullet as well as the mandatory ERROR in shooter CF application.
	// Finally, the function returns the amount of CF Change to be applied this cycle.
	//
	// NOTE: This function has to be run TWICE: once for the X axis, the other for the Y axis. Feed
	// different arguments in to get one or the other.

	///////////////// MAXIMUM INCREMENT

	// This INI value defines the maximum allowed change of velocity between any two bullets in the
	// volley. This is used to limit any attempted change, and also helps measure the potential movements
	// of the muzzle.
	// Note: The limit applies BEFORE mandatory aiming errors are taken into account.
	FLOAT dMaxIncrement = gGameCTHConstants.RECOIL_COUNTER_INCREMENT;

	///////////////// IDEAL COUNTER-FORCE

	// To begin, we're going to calculate an "IDEAL" CF. In other words, we're checking to see
	// how much CF is currently required to bring the muzzle exactly to coordinate 0,0 in one
	// movement - ignoring current CF or any limit on changing CF.

	// First calculate how far our muzzle currently is from the target's center. Positive is
	// right (or up), negative is left (or down).
	FLOAT dIdealCF = dMuzzleOffset;

	// Add the gun's Recoil to this, as that's how far the muzzle will end up if we let
	// recoil go unopposed.
	dIdealCF += bRecoil;

	// The remainder is how much help we need from CF to get our muzzle exactly on the target.
	// We need to invert this now, because CF must work TOWARDS 0,0 , i.e. decreasing a right/up-offset
	// and increasing a left/down-offset.
	dIdealCF *= -1;

	// Lets also record the direction of the target.
	INT8 bDirectionTarget = (-1) * (INT8)(dMuzzleOffset / abs(dMuzzleOffset));

	// Lets track a separate value for the actual direction of the target
	INT8 bDirectionIdeal = (INT8)(dIdealCF / abs(dIdealCF));

	// We now know how much CF we need to get back to the target's center. Let's subtract
	// the amount of CF we're already applying from the previous bullet. This tells us by how much we 
	// need to change the current CF in order to get the muzzle all the way back to 0,0 with our
	// current bullet.
	FLOAT dIdealDelta = dIdealCF - dPrevCounterForce;

	///////////////// SITUATIONS

	// So now we know how much CF we would need, and how much change we need to make to get to that CF.
	// However, since we are enforcing a maximum amount of change with every bullet, any change
	// has to be carefully considered. If we change CF too radically towards the target, we may end
	// up sweeping across it. In other words, when we get to 0,0, our CF may cause us to keep
	// moving the muzzle in that direction afterwards, causing us to end up on the other side of
	// 0,0 with the muzzle speeding off further away! Ideally, we want to get to 0,0 and simultaneously
	// also reach CF = -Recoil (in other words, balancing them out so that the muzzle doesn't move
	// AWAY from 0,0). This is tricky, again, because we can only change CF by a small amount each
	// time we run this function.
	// The section below handles this, by actively measuring what would happen if we altered CF, whether
	// reducing it or increasing it, and trying to figure out the best way to reach 0,0 coordinates
	// with exactly CF = -Recoil.

	// Start with something easy: is the muzzle currently motionless? ( CF = -Recoil )
	if (dPrevCounterForce == -bRecoil)
	{
		///////////////// STATIONARY MUZZLE

		// We are currently applying exactly as much CF as we need to keep the muzzle stationary.
		// Ok, are we at the target center?
		if (dMuzzleOffset == 0)
		{
			//Perfect! This is where we want to be! Keep the muzzle steady by simply not changing anything.
			dIdealDelta = 0;
			if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
			{
				ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Muzzle stable :: No change required.");
			}
		}
		else
		{
			// We need to accelerate in the direction of the target center. Because we are currently motionless,
			// then no matter how much acceleration we add right now we'll be able to undo it during the next
			// step - i.e. one step accelerating, next step we could easily decelerate back to a stationary
			// state, or keep accelerating if necessary. In either case, it's safe to accelerate towards
			// 0,0.

			// To do this we don't have to change anything. dIdealChangeX already tells us which direction
			// to accelerate in, and also indicates the "IDEAL" amount we need to apply. If that ideal exceeds
			// the delta limit, that's fine - it'll be reduced appropriately later in this function anyway.
			if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
			{
				ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Muzzle stationary :: Increasing CF towards target.");
			}
		}
	}
	else
	{
		///////////////// MOVING MUZZLE

		// The muzzle is currently in motion, which complicates things.
		// Let's find out what exactly we want to do based on the current situation.
		if (dMuzzleOffset == 0)
		{
			// Our muzzle is ON TOP OF the target - but is still in motion. Our only sane option at this
			// point is to decelerate muzzle movement. The change, therefore, should attempt to cancel
			// out recoil and no more. To do so, calculate our current speed, then invert it.
			dIdealDelta = bRecoil + dPrevCounterForce;
			dIdealDelta *= -1; 
			if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
			{
				ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Muzzle at target but in motion :: Reversing CF.");
			}
		}
		else
		{
			// If we got here, it means that our muzzle is NOT pointing at the target, and is NOT stationary
			// either. This means we're going to have to think carefully about any change we make to CF.
			// Is it better to fight recoil right now, or better to let it work? Or maybe even help it
			// along. Perhaps muzzle movement needs to be reduced to avoid overshooting the 0,0 target,
			// this is what we're going to figure out now.

			// Calculate current muzzle velocity by summing up Recoil and Counter Force.
			FLOAT dVelocity = bRecoil + dPrevCounterForce;

			// The sign tells us which way we are going.
			INT8 bDirectionMuzzle = (INT8)(dVelocity / abs(dVelocity));
			
			// Is the muzzle heading AWAY from the target at the moment?
			if (bDirectionMuzzle != bDirectionTarget)
			{
				// Naturally, moving away from 0,0 is the opposite of what we want. Immediately
				// decelerate! We do this simply be reversing speed.
				dIdealDelta = dVelocity * (-1);
				if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
				{
					ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Muzzle moving away from target :: Reversing CF.");
				}
			}
			else
			{
				// Our muzzle is heading towards the 0,0 coordinate. This is good, but we must still be
				// careful. If we accelerate too much, we will end up overshooting. If we slow down too
				// much, we would take too many bullets to reach the target. Lets figure out what then
				// best move is.

				// First lets count the number of steps it would take to stop all muzzle movement. This assumes
				// that we will be able to change muzzle movement by a set amount after each bullet (I.E. not
				// accounting for mandatory errors). Essentially, we're calculating how fast the muzzle is moving
				// right now, and dividing it by the maximum deceleration that can be applied. The result is
				// the number of bullets that will elapse before we could stop the muzzle from moving, assuming
				// we start decelerating immediately and keep at it until we stop.
				UINT32 uiStepsToStopMovement = (INT32)abs((dPrevCounterForce + bRecoil) / dMaxIncrement);

				// Now we count something else entirely: the number of steps it would take to reach target 0,0.
				// To do this we actually simulate the remaining bullets in the volley, using current and future
				// speed as a guide. For this calculation, we will be assuming that on each step we're going to
				// decelerate as much as possible while still remaining in motion.
				UINT32 uiStepsToReachTargetWhileDecelerating = 0;
				FLOAT dDistanceTraveled = 0;
				// We'll be working upwards to make things easy.
				dVelocity = abs(dVelocity);

				// Start a loop to measure the distance our muzzle will travel after
				// each bullet. With each cycle, we will decrease our speed by the 
				// maximum allowed amount, and add the distance traveled to a counter.
				// We only stop once we've arrived at or overpassed the target 0,0.
				// As we go, we'll count the number of steps we've taken.
				while (dDistanceTraveled < abs(dMuzzleOffset) )
				{
					// Count a step
					uiStepsToReachTargetWhileDecelerating++;

					// Reduce speed by one full increment.
					dVelocity = dVelocity - dMaxIncrement;
					// But, make sure we stay in motion by enforcing a minimum velocity. 
					// This minimum is equal to the maximum allowed delta. While traveling at this 
					// speed we can stop muzzle movement in exactly one step. In other words, it 
					// is the highest speed at which we are simultaneously both in motion and in 
					// total control of the gun.
					dVelocity = __max(dVelocity, dMaxIncrement);

					// Measure the distance we've traveled so far based on the current speed.
					dDistanceTraveled += dVelocity; 
				}
		
				// We now know how many steps it would take to stop the muzzle from moving, and how
				// many steps it will take to reach 0,0 if we decelerate constantly.

				// However, before using these values we need to ask: are we actually intending to
				// fire that many bullets?
				UINT32 uiRoundsRemaining = uiIntendedBullets - (pShooter->bDoBurst - 1);
				BOOLEAN fEnoughBullets = true;
				if (uiRoundsRemaining < uiStepsToReachTargetWhileDecelerating)
				{
					// Setting this flag to false means we don't intend to fire enough bullets
					// to use a "smooth and controllable" deceleration as seen above. The flag will 
					// then signal to the code below that we're in a hurry. As a result, the program
					// will force MORE acceleration towards 0,0, increasing the chance of sweeping 
					// across it before the volley ends.
					fEnoughBullets = false;
				}
			
				// We are now ready to compare the two Step Counts. By determining whether it would
				// take more steps to stop the muzzle or more steps to reach 0,0, we can decide whether
				// acceleration or deceleration are preferable at this time.

				// If it will take longer to stop movement than it would to reach 0,0 (and assuming
				// we have enough bullets to reach 0,0 while slowing down...)
				if (uiStepsToReachTargetWhileDecelerating <= uiStepsToStopMovement && fEnoughBullets )
				{
					// We are already traveling too fast. Even if we decelerate constantly, we will 
					// reach 0,0 before the muzzle can be fully stopped. Therefore, we have to start 
					// slowing down immediately. We're going to overshoot the target, but at least lets
					// not overshoot too much...

					dIdealDelta = (-1) * bDirectionTarget * __min(dMaxIncrement, abs(dIdealDelta));
					if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
					{
						ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Going to overshoot target :: Decreasing CF.");
					}
				}
				else
				{
					// It will take more steps to reach the 0,0 point than it would to stop the muzzle
					// from moving. That means we are traveling at a "safe" speed. In other words, we
					// can afford at least one more step without decelerating.
					// So the question now is - can we afford to actually ACCELERATE at least once, or do
					// we need to keep our current speed to avoid overshooting?

					// To do this we'll run basically the same check we did before - simulating muzzle
					// movement over the new few bullets. Except now we'll be checking what happens if
					// we accelerate once (this turn) and THEN start decelerating. If the check goes ok,
					// that means we can afford to accelerate this turn.

					// First, increase the number of steps to stop by two (one extra step to accelerate, 
					// one extra step to decelerate).
					uiStepsToStopMovement += 2;

					// Now let's see how many steps it would take to reach the target with the new speed.
					UINT32 uiStepsToReachTarget = 0;
					// Calculate current muzzle velocity again
					dVelocity = bRecoil + dPrevCounterForce;
					// Working only with positives...
					dVelocity = abs(dVelocity);
					// We're testing to see what happens if we accelerate, so increase velocity by the
					// maximum allowed amount.
					dVelocity += dMaxIncrement;
					dDistanceTraveled = 0;

					// Simulate!
					while (dDistanceTraveled < abs(dMuzzleOffset) )
					{
						// Count a step
						uiStepsToReachTarget++;
						// Reduce speed, but keep up moving at least fast enough that we could stop whenever we want.
						dVelocity = __max(dVelocity - dMaxIncrement, dMaxIncrement);
						// Measure the distance we've traveled so far.
						dDistanceTraveled += dVelocity; 
					}

					// Check whether we have enough bullets to reach the target while decelerating.
					// If we don't, we're going to want to force acceleration regardless of what the
					// test above says. Accelerating will increase our chance to reach the target AT ALL,
					// rather than be forced to stop short of the target for lack of bullets.
					BOOLEAN fEnoughBullets = true;
					if (uiRoundsRemaining < uiStepsToReachTarget)
					{
						fEnoughBullets = false;
					}

					// Ok, so now lets ask: Are we still moving slowly enough that we can stop the muzzle any time
					// before reaching 0,0, if we want to?
					if (uiStepsToReachTarget > uiStepsToStopMovement || fEnoughBullets)
					{
						// Yes! This means that acceleration right now will not prevent us from slowing down later
						// and reaching 0,0 with no muzzle movement. Therefore, acceleration is favoured.

						// Create the maximum allowed acceleration in the target's direction.
						dIdealDelta = bDirectionTarget * dMaxIncrement;
						if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
						{
							ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Advancing to target :: Increasing CF.");
						}
					}
					else
					{
						// Accelerating now will inevitably force us to overshoot the target later on.
						// Deceleration, as we discovered earlier, is not necessary at this step.
						// Therefore, we are content with our current speed, and will likely begin decelerating
						// on our next cycle.

						// No change in current muzzle velocity.
						dIdealDelta = 0;
						if (gGameSettings.fOptions[TOPTION_REPORT_MISS_MARGIN])
						{
							ScreenMsg( FONT_ORANGE, MSG_INTERFACE, L"CF CHANGE: Muzzle advancing at acceptable speed :: No change to CF.");
						}
					}
				}
			}
		}
	}

	// Phew! We now have our orders regarding how much velocity change is ideal given
	// the muzzle's current velocity and its position compared to the target.
	
	// However, we're not done yet. First we need to enforce the maximum possible velocity
	// change per round, as dictated by the INI file. Then we need to apply a minimum amount
	// of shooter error, to add some randomality to this formula - otherwise we're going
	// to get shooters who can fill a target with bullets very easily.

	///////////////// MAXIMUM DELTA

	// Lets start by enforcing a maximum change. The acceptable delta is between dMaxIncrement
	// and -dMaxIncrement.
	FLOAT dDelta = __max(-dMaxIncrement, __min(dIdealDelta, dMaxIncrement));
	INT8 bDeltaSign = (INT8)(dDelta / abs(dDelta));

	///////////////// MANDATORY ERRORS

	// First of all, lets calculate together the forces at work. The more forces are being
	// applied, both by the gun and the shooter, the more prone-to-errors we will be.
	FLOAT dTotalForcesAtWork = abs(bRecoil) + abs(dPrevCounterForce) + abs(dDelta);

	// Compare this to the shooter's computed strength. Stronger shooters will have much less
	// trouble correctly applying their force than weaker ones.
	FLOAT dRatio = dTotalForcesAtWork / dCounterForceMax;

	// This value from the INI defines the minimum amount of error we can have, as a percentage
	// of the total change we're allowed to make per cycle.
	FLOAT dMinCounterForceChangeError = gGameCTHConstants.RECOIL_COUNTER_ACCURACY_MIN_ERROR;
	FLOAT dMinError = dMinCounterForceChangeError * dMaxIncrement;
	dMinError = __max(0.1f, dMinError); //We always want a little. Always.

	// The maximum error is calculated based on how much change we're trying to make, how
	// powerful the forces at work are compared to our strength, and of course how much
	// counter-force accuracy our shooter has.
	FLOAT dMaxError = dRatio * abs(dDelta);
	dMaxError = (dMaxError * (100-uiCounterForceAccuracy)) / 100;
	// It can never be less than the minimum enforced error.
	dMaxError = __max(dMinError, dMaxError);

	// What's the randomality range?
	FLOAT dErrorRange = dMaxError - dMinError;

	// Compute the error!
	FLOAT dError = (FLOAT)(Random( (UINT32)(dMaxError * 1000) )) / 1000.0f;
	INT8 bErrorSign = (Random(2) ? 1 : (-1));
	
	dDelta += (dMinError + dError) * bErrorSign;
	if (bDeltaSign == 1)
	{
		dDelta = __max(0, dDelta);
	}
	else
	{
		dDelta = __min(0, dDelta);
	}

	return dDelta;
}


#if 0
{
	INT32		cntx, cnty;
	INT8		bX, bY;
	INT16		sAngleSum = 0;
	INT8		bNumSums = 0;
	INT16		sAngleAv;
	BOOLEAN fOkX, fOkY;
	BOOLEAN	fGoodX, fGoodY;

	//OK, center on xy and goforit!
	for ( cntx = 0; cntx < 3; cntx++ )
	{
		bX = bLOSX + ( cntx - 1 );

		// Check for boundry conditions, use same as one before boundary
		if ( bX < 0 || bX > 4 )
		{
			fOkX = 0;
		}
		else
		{
			fOkX = 1;
		}

		for ( cnty = 0; cnty < 3; cnty++ )
		{
			fGoodX = fGoodY = TRUE;

			bY = bLOSY + ( cnty - 1 );

			if ( bY < 0 || bY > 4 )
			{
				fOkY = 0;
			}
			else
			{
				fOkY = 1;
			}

			if ( bY < 0 )
			{
				if ( fOkX )
				{
					bY = 0;
				}
				else
				{
					fGoodY = FALSE;
				}
			}
			if ( bY > 4 )
			{
				if ( fOkX )
				{
					bY = 4;
				}
				else
				{
					fGoodY = FALSE;
				}
			}

			if ( bX < 0 )
			{
				if ( fOkY )
				{
					bX = 0;
				}
				else
				{
					fGoodX = FALSE;
				}
			}
			if ( bX > 4 )
			{
				if ( fOkX )
				{
					bX = 4;
				}
				else
				{
					fGoodX = FALSE;
				}
			}

			if ( !fGoodX || !fGoodY )
			{
				continue;
			}

			if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0)
			{
				// OK, now add angles, but not the center!
				if (	cntx == 1 && cnty == 1 )
				{

				}
				else
				{
					sAngleSum += gsLOSDirLUT[ cntx ][cnty];
					bNumSums++;
				}
			}
		}
	}

	// Average angle
	if ( bNumSums > 0)
	{
		sAngleAv = sAngleSum / bNumSums;

		// This is our normal in XY plane!
		// Get unit vector from this
		*pdNormalX = cos( ( (float)sAngleAv / 180 ) * PI );
		*pdNormalY = sin( ( (float)sAngleAv / 180 ) * PI );

		// OK done, now determine direction
		if ( dDeltaX > 0 )
		{
			*pdNormalX *= -1;
		}

		if ( dDeltaY > 0 )
		{
			*pdNormalY *= -1;
		}

		if ( bLOSZ == 3 )
		{
			// Prependicular
			*pdNormalX = 0;
			*pdNormalY = 0;
			*pdNormalZ = -1;
		}
		else if ( bLOSZ > 0 && (((*(pStructure->pShape))[bLOSX][bLOSY] & AtHeight[ bLOSZ +1 ]) ) == 0)
		{
			*pdNormalX = 0;
			*pdNormalY = 0;
			*pdNormalZ = -1;
		}
		else
		{
			*pdNormalZ = 0;
		}

		return( TRUE );
	}
	else
	{
		*pdNormalX = 0;
		*pdNormalY = 0;

		return( FALSE );
	}

}

{
	INT32		cntx, cnty;
	INT8		bX, bY;
	INT16		sAngleSum = 0;
	INT8		bNumSums = 0;
	INT16		sAngleAv;
	BOOLEAN	fAlongX, fAlongY;


	fAlongX = fAlongY = FALSE;

	//1) LOOK ALONG X
	bX = bLOSX - 1;
	bY = bLOSY;

	if ( bX >= 0 && bX <= 4 )
	{
		if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0 )
		{
			fAlongX = TRUE;
		}
	}

	bX = bLOSX + 1;
	bY = bLOSY;

	if ( bX >= 0 && bX <= 4 )
	{
		if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0 )
		{
			fAlongX = TRUE;
		}
	}

	//1) LOOK ALONG Y
	bX = bLOSX;
	bY = bLOSY - 1;

	if ( bY >= 0 && bY <= 4 )
	{
		if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0 )
		{
			fAlongY = TRUE;
		}
	}

	bX = bLOSX;
	bY = bLOSY + 1;

	if ( bY >= 0 && bY <= 4 )
	{
		if (((*(pStructure->pShape))[bX][bY] & AtHeight[ bLOSZ ]) > 0 )
		{
			fAlongY = TRUE;
		}
	}


	if ( fAlongX )
	{
		*pdNormalY = 1;
	}
	else
	{
		*pdNormalY = 0;
	}

	if ( fAlongY )
	{
		*pdNormalX = 1;
	}
	else
	{
		*pdNormalX = 0;
	}

	// OK done, now determine direction
	if ( dDeltaX > 0 )
	{
		*pdNormalX *= -1;
	}

	if ( dDeltaY < 0 )
	{
		*pdNormalY *= -1;
	}

	if ( bLOSZ < 4 && (((*(pStructure->pShape))[bLOSX][bLOSY] & AtHeight[ bLOSZ +1 ]) ) == 0)
	{
		*pdNormalX = 0;
		*pdNormalY = 0;
		*pdNormalZ = -1;
	}
	else
	{
		*pdNormalZ = 0;
	}

	if ( *pdNormalX == 0 && *pdNormalY == 0 && *pdNormalZ == 0 )
	{
		return( FALSE );
	}
	else
	{
		return( TRUE );
	}

}




#endif
