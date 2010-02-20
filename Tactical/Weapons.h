#ifndef __WEAPONS_H
#define __WEAPONS_H

#include "Soldier Control.h"

//ADB moved from Soldier Control.h
enum WeaponMode
{
	WM_NORMAL = 0,
	WM_BURST,
	WM_AUTOFIRE,
	WM_ATTACHED_GL,
	WM_ATTACHED_GL_BURST,
	WM_ATTACHED_GL_AUTO,
	NUM_WEAPON_MODES
} ;

//ADB moved from Interface Panels.h
void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem );


// HEADROCK: Removed this and externalized to JA2_OPTIONS.INI as part of HAM project.
//#define MAXCHANCETOHIT 99
#define BAD_DODGE_POSITION_PENALTY 20

#define GUN_BARREL_RANGE_BONUS	100
#define SOLID_SLUG_RANGE_BONUS	65
#define DUCKBILL_RANGE_BONUS	40

#define MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND 35

// JA2 GOLD: for weapons and attachments, give penalties only for status values below 85
#define WEAPON_STATUS_MOD( x ) ( (x) >= 85 ? 100 : (((x) * 100) / 85) )

// Special deaths can only occur within a limited distance to the target
// Madd
#define MAX_DISTANCE_FOR_MESSY_DEATH 7
#define MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH 23
// If you do a lot of damage with a close-range shot, instant kill
#define MIN_DAMAGE_FOR_INSTANT_KILL 55
// If you happen to kill someone with a close-range shot doing a lot of damage to the head, head explosion
#define MIN_DAMAGE_FOR_HEAD_EXPLOSION 45
// If you happen to kill someone with a close-range shot doing a lot of damage to the chest, chest explosion
// This value is lower than head because of the damage bonus for shooting the head
#define MIN_DAMAGE_FOR_BLOWN_AWAY 30
// If you happen to hit someone in the legs for enough damage, REGARDLESS of distance, person falls down
// Leg damage is halved for these purposes
#define MIN_DAMAGE_FOR_AUTO_FALL_OVER 20

// short range at which being prone provides to hit penalty when shooting standing people
#define MIN_PRONE_RANGE					50

// can't miss at this range?
#define POINT_BLANK_RANGE 16

#define BODY_IMPACT_ABSORPTION 20

#define BUCKSHOT_SHOTS 9

#define MIN_MORTAR_RANGE				150			// minimum range of a mortar

// WEAPON CLASSES
enum
{
	NOGUNCLASS,
	HANDGUNCLASS,
	SMGCLASS,
	RIFLECLASS,
	MGCLASS,
	SHOTGUNCLASS,
	KNIFECLASS,
	MONSTERCLASS,
	NUM_WEAPON_CLASSES
};

// exact gun types
enum
{
	NOT_GUN = 0,
	GUN_PISTOL,
	GUN_M_PISTOL,
	GUN_SMG,
	GUN_RIFLE,
	GUN_SN_RIFLE,
	GUN_AS_RIFLE,
	GUN_LMG,
	GUN_SHOTGUN
};


// ARMOUR CLASSES
enum 
{
	ARMOURCLASS_HELMET,
	ARMOURCLASS_VEST,
	ARMOURCLASS_LEGGINGS,
	ARMOURCLASS_PLATE,
	ARMOURCLASS_MONST,
	ARMOURCLASS_VEHICLE
};

// Warning: There is a table in weapons.c that indexes using these enumberations...
// BurstSndStrings[]....
// MADD MARKER
enum
{
	NOAMMO = 0,
	AMMO38,
	AMMO9,
	AMMO45,
	AMMO357,
	AMMO12G,
	AMMOCAWS,
	AMMO545,
	AMMO556,
	AMMO762N,
	AMMO762W,
	AMMO47,
	AMMO57,
	AMMOMONST,
	AMMOROCKET,
	AMMODART,
	AMMOFLAME,
	AMMO50,
	AMMO9H
};

enum
{
	AMMO_REGULAR = 0,
	AMMO_HP,
	AMMO_AP,
	AMMO_SUPER_AP,
	AMMO_BUCKSHOT,
	AMMO_FLECHETTE,
	AMMO_GRENADE,
	AMMO_MONSTER, // index referenced in code to create ammo type
	AMMO_KNIFE, // index referenced in code to create ammo type
	AMMO_HE,
	AMMO_HEAT,
	AMMO_SLEEP_DART,
	AMMO_FLAME,
};

typedef struct
{
	UINT32 uiIndex;

	// on interface
	UINT8 fontColour;
	// in detail screen (interface\infobox.sti)
	INT32 grayed;
	INT32 offNormal;
	INT32 onNormal;

	// bullet contact with structure
	INT32 structureImpactReductionMultiplier;
	INT32 structureImpactReductionDivisor;

	// bullet contact with armour
	INT32 armourImpactReductionMultiplier;
	INT32 armourImpactReductionDivisor;

	// bullet damage multiplier before/after penetrating armour
	INT32 beforeArmourDamageMultiplier;
	INT32 beforeArmourDamageDivisor;
	INT32 afterArmourDamageMultiplier;
	INT32 afterArmourDamageDivisor;

	// minimum damage can be zero
	BOOLEAN zeroMinimumDamage;
	BOOLEAN canGoThrough;
	BOOLEAN standardIssue;
	UINT8 numberOfBullets;
	INT32 multipleBulletDamageMultiplier;
	INT32 multipleBulletDamageDivisor;
	BOOLEAN highExplosive;
	UINT8 explosionSize; //0=none, 1=small, 2=medium, 3=large
	BOOLEAN antiTank;
	BOOLEAN dart;
	BOOLEAN knife;
	UINT16  monsterSpit;
	BOOLEAN ignoreArmour;
	BOOLEAN acidic;
	INT16	lockBustingPower;
	BOOLEAN tracerEffect;

	//zilpin: pellet spread patterns externalized in XML
	INT32 spreadPattern;
	
} AMMOTYPE;

enum
{
	EXPLOSV_NORMAL,
	EXPLOSV_STUN,
	EXPLOSV_TEARGAS,
	EXPLOSV_MUSTGAS,
	EXPLOSV_FLARE,
	EXPLOSV_NOISE,
	EXPLOSV_SMOKE,
	EXPLOSV_CREATUREGAS,
	EXPLOSV_BURNABLEGAS,
	EXPLOSV_FLASHBANG,
};

#define AMMO_DAMAGE_ADJUSTMENT_BUCKSHOT( x ) (x / 4)
#define NUM_BUCKSHOT_PELLETS 9

// hollow point bullets do lots of damage to people
#define AMMO_DAMAGE_ADJUSTMENT_HP( x ) ( (x * 17) / 10 )
// but they SUCK at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_HP( x ) ( (x * 3) / 2 )
// armour piercing bullets are good at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_AP( x ) ((x * 3) / 4)
// "super" AP bullets are great at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_SAP( x ) (x / 2)

// high explosive damage value (PRIOR to armour subtraction)
#define AMMO_DAMAGE_ADJUSTMENT_HE( x ) ((x * 4) / 3)

// but they SUCK at penetrating armour
#define AMMO_STRUCTURE_ADJUSTMENT_HP( x ) (x * 2)
// armour piercing bullets are good at penetrating structure
#define AMMO_STRUCTURE_ADJUSTMENT_AP( x ) ( (x * 3 ) / 4)
// "super" AP bullets are great at penetrating structures
#define AMMO_STRUCTURE_ADJUSTMENT_SAP( x ) ( x / 2 )

// one quarter of punching damage is "real" rather than breath damage
#define PUNCH_REAL_DAMAGE_PORTION 4 

#define AIM_BONUS_SAME_TARGET		10		// chance-to-hit bonus (in %)
#define AIM_BONUS_PER_AP			10		// chance-to-hit bonus (in %) for aim
#define AIM_BONUS_CROUCHING		 10
#define AIM_BONUS_PRONE						20
#define AIM_BONUS_TWO_HANDED_PISTOL 5 
#define AIM_BONUS_FIRING_DOWN				15
#define AIM_PENALTY_ONE_HANDED_PISTOL 5 
#define AIM_PENALTY_DUAL_PISTOLS		20
#define AIM_PENALTY_SMG				5
#define AIM_PENALTY_GASSED			50
#define AIM_PENALTY_GETTINGAID		20
#define AIM_PENALTY_PER_SHOCK		5		// 5% penalty per point of shock
#define AIM_BONUS_TARGET_HATED		20
#define AIM_BONUS_PSYCHO			15
#define AIM_PENALTY_TARGET_BUDDY	20
#define AIM_PENALTY_BURSTING		10
#define AIM_PENALTY_GENTLEMAN		15
#define AIM_PENALTY_TARGET_CROUCHED 20
#define AIM_PENALTY_TARGET_PRONE	40
#define AIM_PENALTY_BLIND			80
#define AIM_PENALTY_FIRING_UP		25
#define	HIGH_POWER_SCOPE			14

#define MAX_WEAPON_NAME_LENGTH		20

typedef struct
{
 UINT8	ubWeaponClass;			 // handgun/shotgun/rifle/knife
 UINT8	ubWeaponType;							// exact type (for display purposes)
 UINT8	ubCalibre;				// type of ammunition needed
 UINT8	ubReadyTime;				// APs to ready/unready weapon
 //CHRISL: Changed to a float to allow for more control over actual ap costs
 //UINT8	ubShotsPer4Turns;			// maximum (mechanical) firing rate
 FLOAT	ubShotsPer4Turns;			// maximum (mechanical) firing rate
 UINT8	ubShotsPerBurst;
 UINT8	ubBurstPenalty;						// % penalty per shot after first
 UINT8	ubBulletSpeed;			 // bullet's travelling speed
 UINT8	ubImpact;					// weapon's max damage impact (size & speed)
 UINT8	ubDeadliness;							// comparative ratings of guns
 INT8	bAccuracy;								// accuracy or penalty
 UINT8	ubMagSize;
 UINT16	usRange;
 UINT16	usReloadDelay;
 UINT8	ubAttackVolume;
 UINT8	ubHitVolume;
 UINT16	sSound;
 UINT16	sBurstSound;
 UINT16	sSilencedBurstSound;
 UINT16	sReloadSound;
 UINT16	sLocknLoadSound;
 UINT8	bBurstAP;										// Snap: Burst AP cost replaces bBaseAutofireCost
 UINT8	bAutofireShotsPerFiveAP;

 CHAR8	szWeaponName[MAX_WEAPON_NAME_LENGTH+1]; //+1 for the null terminator
 UINT32	uiIndex;

 BOOLEAN swapClips;
 UINT16	silencedSound;
 UINT8	APsToReload;
 UINT8	maxdistformessydeath;
 BOOLEAN NoSemiAuto;
 UINT8	AutoPenalty;
 INT16	sAniDelay;			// Lesh: for burst animation delay
 UINT8	APsToReloadManually;
 UINT16 ManualReloadSound;

} WEAPONTYPE;
typedef struct
{
	UINT8	ubCalibre;
	UINT16	ubMagSize;
	UINT8	ubAmmoType;

	UINT32 uiIndex;
} MAGTYPE;

typedef struct
{
	UINT8	ubArmourClass;
	UINT16	ubProtection;
	UINT8	ubCoverage;	// Captain J
	UINT8	ubDegradePercent;
	UINT32		uiIndex;
} ARMOURTYPE;

typedef struct
{
	UINT16		ubType;					// type of explosive
	UINT16		ubDamage;				// damage value
	UINT16		ubStunDamage;		// stun amount / 100
	UINT16		ubRadius;				// radius of effect
	UINT16		ubVolume;				// sound radius of explosion
	UINT16		ubVolatility;		// maximum chance of accidental explosion
	UINT16		ubAnimationID;	// Animation enum to use
	UINT32		uiIndex;
	UINT16		ubDuration;	
	UINT16		ubStartRadius;	
} EXPLOSIVETYPE;

//GLOBALS

extern WEAPONTYPE Weapon[ MAXITEMS ]; 
extern ARMOURTYPE Armour[MAXITEMS+1];
extern MAGTYPE Magazine[MAXITEMS+1];
extern EXPLOSIVETYPE Explosive[MAXITEMS+1];
extern CHAR8 gzBurstSndStrings[MAXITEMS*2][128];	// Lesh: changed this
extern AMMOTYPE AmmoTypes[MAXITEMS];

extern BOOLEAN ReadInWeaponStats(STR fileName);
extern BOOLEAN WriteWeaponStats();

extern INT32 EffectiveArmour( OBJECTTYPE * pObj );
extern INT8 ArmourVersusExplosivesPercent( SOLDIERTYPE * pSoldier );
extern BOOLEAN FireWeapon( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo );
extern void WeaponHit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation );
extern void StructureHit( INT32 iBullet, UINT16 usWeaponIndex, INT16 bWeaponStatus, UINT8 ubAttackerID, UINT16 sXPos, INT16 sYPos, INT16 sZPos, UINT16 usStructureID, INT32 iImpact, BOOLEAN fStopped );
extern void WindowHit( INT16 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce );
extern INT32 BulletImpact( SOLDIERTYPE *pFirer, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, INT16 sHitBy, UINT8 * pubSpecial );
extern BOOLEAN InRange( SOLDIERTYPE *pSoldier, INT16 sGridNo );
extern void ShotMiss( UINT8 ubAttackerID, INT32 iBullet );
extern UINT32 CalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT16 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );
extern UINT32 AICalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT16 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );
extern UINT32 CalcChanceToPunch(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, INT16 ubAimTime);
extern UINT32 CalcChanceToStab(SOLDIERTYPE * pAttacker,SOLDIERTYPE *pDefender, INT16 ubAimTime);
UINT32 CalcChanceToSteal(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, INT16 ubAimTime);
extern void ReloadWeapon( SOLDIERTYPE *pSoldier, UINT8 ubHandPos );
// Changed by ADB, rev 1513
//extern BOOLEAN IsGunWeaponModeCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , UINT8 bWpnMode );
//extern BOOLEAN IsGunBurstCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , BOOLEAN fNotify );
//extern BOOLEAN IsGunAutofireCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos );
extern BOOLEAN IsGunWeaponModeCapable( OBJECTTYPE* pObject, WeaponMode weaponMode, SOLDIERTYPE *pSoldier = NULL );
extern BOOLEAN IsGunBurstCapable( OBJECTTYPE* pObject, BOOLEAN fNotify, SOLDIERTYPE *pSoldier = NULL );
extern BOOLEAN IsGunAutofireCapable( OBJECTTYPE* pObject );
extern INT32 CalcBodyImpactReduction( UINT8 ubAmmoType, UINT8 ubHitLocation );
extern INT32 TotalArmourProtection( SOLDIERTYPE *pFirer, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, UINT8 ubAmmoType );
extern INT32 ArmourPercent( SOLDIERTYPE * pSoldier );

extern void GetTargetWorldPositions( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, FLOAT *pdXPos, FLOAT *pdYPos, FLOAT *pdZPos );

extern BOOLEAN	OKFireWeapon( SOLDIERTYPE *pSoldier );
extern BOOLEAN CheckForGunJam( SOLDIERTYPE * pSoldier );

extern INT32 CalcMaxTossRange( SOLDIERTYPE * pSoldier, UINT16 usItem, BOOLEAN fArmed );
extern UINT32 CalcThrownChanceToHit(SOLDIERTYPE *pSoldier, INT16 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );

extern void ChangeWeaponMode( SOLDIERTYPE * pSoldier );

extern BOOLEAN UseHandToHand( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo, BOOLEAN fStealing );

void DishoutQueenSwipeDamage( SOLDIERTYPE *pQueenSoldier );

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack );

UINT16 GunRange( OBJECTTYPE * pObj );
BOOLEAN IsWeapon ( UINT16 itemIndex );
UINT8 GetDamage ( OBJECTTYPE *pObj );
UINT8 GetAutofireShotsPerFiveAPs( OBJECTTYPE *pObj );
UINT8 GetBaseAutoFireCost( OBJECTTYPE *pObj );
UINT8 GetBurstPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance = FALSE );
UINT8 GetAutoPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance = FALSE );
UINT8 GetShotsPerBurst( OBJECTTYPE *pObj );
UINT16 GetMagSize( OBJECTTYPE *pObj );
UINT8 GetAmmoType( OBJECTTYPE *pObj );
bool WeaponReady(SOLDIERTYPE * pSoldier);
INT8 GetAPsToReload( OBJECTTYPE *pObj );

// HEADROCK HAM 3.4: Estimate bullets left in gun. Returns an "errorcode" telling the calling function if the check
// was successful and to what degree.
void EstimateBulletsLeft( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );
extern CHAR16 gBulletCount[10];


#endif
