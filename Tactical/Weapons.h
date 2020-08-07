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
	WM_ATTACHED_UB,
	WM_ATTACHED_UB_BURST,
	WM_ATTACHED_UB_AUTO,
	WM_ATTACHED_BAYONET,
	NUM_WEAPON_MODES
} ;

enum ScopeMode
{
	USE_BEST_SCOPE = 0,
	USE_SCOPE_2,
	USE_SCOPE_3,
	USE_SCOPE_4,
	USE_SCOPE_5,
	USE_SCOPE_6,
	USE_SCOPE_7,
	USE_SCOPE_8,
	USE_SCOPE_9,
	USE_SCOPE_10,
	NUM_SCOPE_MODES
};
#define USE_ALT_WEAPON_HOLD -1 // SANDRO - using this for hip/onehandpistol fire

//ADB moved from Interface Panels.h
void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem );


// HEADROCK: Removed this and externalized to JA2_OPTIONS.INI as part of HAM project.
//#define MAXCHANCETOHIT 99
#define BAD_DODGE_POSITION_PENALTY 20

#define GUN_BARREL_RANGE_BONUS	100
#define SOLID_SLUG_RANGE_BONUS	65
#define DUCKBILL_RANGE_BONUS	40

// silversurfer: externalized this to gGameExternalOptions.gubMaxPercentNoiseSilencedSound
//#define MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND 35

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

// Flugente: this value is now externalised to gItemSettings.usBasicDeprecateChance
// NB this is arbitrary, chances in DG ranged from 1 in 6 to 1 in 20
//#define BASIC_DEPRECIATE_CHANCE	15

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
	AMMO_MAGAZINE = 0,
	AMMO_BULLET,
	AMMO_BOX,
	AMMO_CRATE
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

// -------- added by Flugente: various ammo flags --------
// flags used for various ammo properties (easier than adding 32 differently named variables). DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define AMMO_CRYO				0x00000001	//1			// this ammo shock-freezes target
#define AMMO_BLIND				0x00000002	//2			// this ammo will blind if it hits the head
#define AMMO_ANTIMATERIEL		0x00000004	//4			// this ammo is anti-materiel, bullets can destroy structures
/*#define CONCERTINA				0x00000008	//8

#define WATER_DRUM				0x00000010	//16		// water drums allow to refill canteens in the sector they are in
#define MEAT_BLOODCAT			0x00000020	//32		// retrieve this by gutting a bloodcat
#define COW_MEAT   				0x00000040	//64		// retrieve this by gutting a cow
#define BELT_FED				0x00000080	//128		// item can be fed externally

#define AMMO_BELT				0x00000100	//256		// this item can be used to feed externally
#define AMMO_BELT_VEST			0x00000200	//512		// this is a vest that can contain AMMO_BELT items in its medium slots
#define COVERT_OPS_KIT			0x00000400	//1024		// this kit can restore the covert ops disguise
#define RAG						0x00000800	//2048		// this item is a rag and can be used to remove camo

#define ENEMY_NET_1_LVL_4		0x00001000	//4096
#define ENEMY_NET_2_LVL_4       0x00002000	//8192
#define ENEMY_NET_3_LVL_4 		0x00004000	//16384
#define ENEMY_NET_4_LVL_4		0x00008000	//32768

#define PLAYER_NET_1_LVL_1		0x00010000	//65536
#define PLAYER_NET_2_LVL_1      0x00020000	//131072
#define PLAYER_NET_3_LVL_1		0x00040000	//262144
#define PLAYER_NET_4_LVL_1		0x00080000	//524288

#define PLAYER_NET_1_LVL_2		0x00100000	//1048576
#define PLAYER_NET_2_LVL_2		0x00200000	//2097152
#define PLAYER_NET_3_LVL_2		0x00400000	//4194304
#define PLAYER_NET_4_LVL_2		0x00800000	//8388608

#define PLAYER_NET_1_LVL_3		0x01000000	//16777216
#define PLAYER_NET_2_LVL_3		0x02000000	//33554432
#define PLAYER_NET_3_LVL_3		0x04000000	//67108864
#define PLAYER_NET_4_LVL_3		0x08000000	//134217728

#define PLAYER_NET_1_LVL_4		0x10000000	//268435456
#define PLAYER_NET_2_LVL_4		0x20000000	//536870912
#define PLAYER_NET_3_LVL_4		0x40000000	//1073741824
#define PLAYER_NET_4_LVL_4		0x80000000	//2147483648*/
// ----------------------------------------------------------------

#define MAX_ANIMATION_FILENAME_LEN 100

typedef struct
{
	UINT32 uiIndex;
	
	// Flugente: fontColour takes RGB values from a sti library. Why not take RGB values directly?
	UINT8 red;
	UINT8 green;
	UINT8 blue;

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
	UINT16 usPiercePersonChanceModifier;
	BOOLEAN standardIssue;
	UINT8 numberOfBullets;
	INT32 multipleBulletDamageMultiplier;
	INT32 multipleBulletDamageDivisor;
	UINT16 highExplosive;
	UINT8 explosionSize; //0=none, 1=small, 2=medium, 3=large
	BOOLEAN dart;
	BOOLEAN knife;
	UINT16  monsterSpit;
	BOOLEAN ignoreArmour;
	BOOLEAN acidic;
	INT16	lockBustingPower;
	BOOLEAN tracerEffect;
	FLOAT	temperatureModificator;	// Flugente: modificator for weapon temperature
	FLOAT	dirtModificator;	// Flugente: modificator for dirt generation

	//zilpin: pellet spread patterns externalized in XML
	INT32 spreadPattern;

	// Flugente: item flag for various properties. Way easier than 32 boolean flags
	UINT32	ammoflag;

	// Flugente: modifiers for life and breath damage, 1.0 being the unmodified value
	FLOAT dDamageModifierLife;
	FLOAT dDamageModifierBreath;
	FLOAT dDamageModifierTank;
	FLOAT dDamageModifierArmouredVehicle;
	FLOAT dDamageModifierCivilianVehicle;
	FLOAT dDamageModifierZombie;

	// sevenfm: shot animation
	CHAR8	shotAnimation[MAX_ANIMATION_FILENAME_LEN];
} AMMOTYPE;

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
//#define	HIGH_POWER_SCOPE			14 //Externalized

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
 INT8	bAccuracy;								// accuracy or penalty used by OCTH
 INT8	nAccuracy;								// accuracy or penalty used by NCTH
 UINT16	ubMagSize;
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
 BOOLEAN EasyUnjam; // Guns where each bullet has its own chamber (like revolvers) are easyer to unjam 

 FLOAT	bRecoilX;		// HEADROCK HAM 4: Recoil now measured in points of muzzle deviation X and Y.
 FLOAT	bRecoilY;		// Positive values indicated upwards (Y) and rightwards (X). Negatives are down (-Y) and left (-X).
							// Note that each value is an array. Each item in the array determines recoil
							// for a different bullet in the sequence. Not all values have to be filled,
							// but the last filled value will determine the recoil for longer volleys.
 UINT8	ubRecoilDelay;

 UINT8	ubAimLevels;		// HEADROCK HAM 4: Dictates how many aiming levels this gun supports. If 0, the program
							// chooses automatically based on the type of gun (see AllowedAimingLevels() ).

 UINT8	ubHandling;			// CHRISL HAM 4: This value replaces ubReadyTime for determining a weapons base handling characteristics.

 // Flugente
 FLOAT usOverheatingJamThreshold;				// if a gun's temperature is above this treshold, it is increasingly prone to jamming
 FLOAT usOverheatingDamageThreshold;			// if a gun is fired while its temperature is above this value, it degrades much faster
 FLOAT usOverheatingSingleShotTemperature;		// a single shot raises a gun's temperature by this amount
 
 BOOLEAN HeavyGun;	// SANDRO - a gun with this cannot be shouldered in standing position, part of shooting from hip feature

 // Flugente: if the wielder possesses the 'GUNSLINGER_NT' trait, is using alternate hold scope mode, and has the second hand free, this gun can be used in burst mode
 // this is intended for guns that normally don't possess burst mode, like revolvers
 BOOLEAN fBurstOnlyByFanTheHammer;

 // Flugente: multi-barrel weapons can fire a variety of barrels at once in all firemodes.
 // This vector stores the possible configurations
 std::vector<UINT8>	barrelconfigurations;

 // sevenfm: NWSS data
 UINT8 ubNWSSCase;
 UINT8 ubNWSSLast;
 CHAR8 szNWSSSound[MAX_WEAPON_NAME_LENGTH + 1];

} WEAPONTYPE;
typedef struct
{
	UINT8	ubCalibre;
	UINT16	ubMagSize;
	UINT8	ubAmmoType;
	UINT8   ubMagType;

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
	UINT8		ubMagSize;
	BOOLEAN		fExplodeOnImpact;	// HEADROCK HAM 5: Flag for impact explosives.
	UINT16		usNumFragments;		// HEADROCK HAM 5.1: Fragmenting Explosive Data
	UINT8		ubFragType;
	UINT16		ubFragDamage;
	UINT16		ubFragRange;
	UINT16		ubHorizontalDegree;	// Flugente: size of the horizontal arc into which fragments will be shot
	UINT16		ubVerticalDegree;	// Flugente: size of the vertical arc into which fragments will be shot
	FLOAT		bIndoorModifier; //DBrot: confined areas enhance effect
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

extern bool gbForceWeaponNotReady;
extern bool gbForceWeaponReady;

extern INT32 EffectiveArmour( OBJECTTYPE * pObj );
extern INT8 ArmourVersusExplosivesPercent( SOLDIERTYPE * pSoldier );
// Flugente: fire resistance
INT16 ArmourVersusFirePercent( SOLDIERTYPE * pSoldier );
extern BOOLEAN FireWeapon( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );
extern void WeaponHit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation );
extern void StructureHit( INT32 iBullet, UINT16 usWeaponIndex, INT16 bWeaponStatus, UINT8 ubAttackerID, UINT16 sXPos, INT16 sYPos, INT16 sZPos, UINT16 usStructureID, INT32 iImpact, BOOLEAN fStopped );
extern void WindowHit( INT32 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce );
// HEADROCK HAM 5.1: Moved to Bullets.h
extern BOOLEAN InRange( SOLDIERTYPE *pSoldier, INT32 sGridNo );
extern void ShotMiss( UINT8 ubAttackerID, INT32 iBullet );
extern UINT32 CalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );
extern UINT32 CalcNewChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );
extern UINT32 AICalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos, INT8 bTargetLevel, UINT16 usAnimState);//dnl ch59 180813
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
INT32 TotalArmourProtection( SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, UINT8 ubAmmoType, BOOLEAN fConsiderFlak );
extern INT32 ArmourPercent( SOLDIERTYPE * pSoldier );

extern void GetTargetWorldPositions( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo, FLOAT *pdXPos, FLOAT *pdYPos, FLOAT *pdZPos );

extern BOOLEAN	OKFireWeapon( SOLDIERTYPE *pSoldier );
extern BOOLEAN CheckForGunJam( SOLDIERTYPE * pSoldier );
extern FLOAT   GetGunOverheatDamagePercentage( FLOAT usTemperature, UINT16 usIndx );	// Flugente: Get percentage: temperature/damagethreshold
extern FLOAT   GetGunOverheatJamPercentage( FLOAT usTemperature, UINT16 usIndx );		// Flugente: Get percentage: temperature/jamthreshold

extern INT32 CalcMaxTossRange( SOLDIERTYPE * pSoldier, UINT16 usItem, BOOLEAN fArmed, OBJECTTYPE* pObject =NULL );
extern UINT32 CalcThrownChanceToHit(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos );

extern void ChangeWeaponMode( SOLDIERTYPE * pSoldier );
extern void ChangeScopeMode( SOLDIERTYPE * pSoldier, INT32 iTrgGridNo );		// Flugente: use different scope

extern BOOLEAN UseHandToHand( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo, BOOLEAN fStealing );

void DishoutQueenSwipeDamage( SOLDIERTYPE *pQueenSoldier );

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack );

UINT16 GunRange( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier ); // SANDRO - added argument
BOOLEAN IsWeapon ( UINT16 itemIndex );
UINT8 GetDamage ( OBJECTTYPE *pObj );
// HEADROCK HAM 4: Same function as above, but without modifiers from attached items.
UINT8 GetBasicDamage ( OBJECTTYPE *pObj );
UINT8 GetAutofireShotsPerFiveAPs( OBJECTTYPE *pObj );
UINT8 GetBaseAutoFireCost( OBJECTTYPE *pObj );
UINT8 GetBurstPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance = FALSE );
UINT8 GetAutoPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance = FALSE );
UINT8 GetShotsPerBurst( OBJECTTYPE *pObj );
UINT16 GetMagSize( OBJECTTYPE *pObj, UINT8 subObject = 0 );
UINT16 GetExpMagSize( OBJECTTYPE *pObj );
UINT8 GetAmmoType( OBJECTTYPE *pObj );
bool WeaponReady(SOLDIERTYPE * pSoldier);
INT16 GetAPsToReload( OBJECTTYPE *pObj );

// HEADROCK HAM 3.4: Estimate bullets left in gun. Returns an "errorcode" telling the calling function if the check
// was successful and to what degree.
void EstimateBulletsLeft( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );
extern CHAR16 gBulletCount[10];

// HEADROCK HAM 4: This function generates a mag-factor bar percentage.
void CalcMagFactorSimple( SOLDIERTYPE *pSoldier, FLOAT d2DDistance, INT16 bAimTime, INT32 iGridNo );
// HEADROCK HAM 4: This gets the Z of a target regardless of what's there.
FLOAT GetTargetZPos( SOLDIERTYPE *pShooter, INT32 sTargetGridNo );

// Flugente: Overheating Weapons
void GunIncreaseHeat( OBJECTTYPE *pObj, SOLDIERTYPE* pSoldier );	// adding pSoldier allows soldier-specific modification of generated heat
FLOAT GetTemperatureModifier( OBJECTTYPE *pObj );
FLOAT GetSingleShotTemperature( OBJECTTYPE *pObj );
FLOAT GetGunOverheatDamagePercentage( OBJECTTYPE * pObj );	// Flugente: Get percentage: temperature/damagethreshold
FLOAT GetGunOverheatJamPercentage( OBJECTTYPE * pObj );		// Flugente: Get percentage: temperature/jamthreshold
FLOAT GetGunOverheatDisplayPercentage( OBJECTTYPE * pObj );	// Flugente: Get displyed overheat percentage - either GetGunOverheatDamagePercentage or GetGunOverheatJamPercentage
FLOAT GetOverheatJamThresholdModifier( OBJECTTYPE *pObj );
FLOAT GetOverheatJamThreshold( OBJECTTYPE *pObj );
FLOAT GetOverheatDamageThresholdModifier( OBJECTTYPE *pObj );
FLOAT GetOverheatDamageThreshold( OBJECTTYPE *pObj );

BOOLEAN ArtilleryStrike( UINT16 usItem, UINT8 ubOwnerID, UINT32 usStartingGridNo, UINT32 usTargetMapPos );

// Flugente: functions for using several barrels at once
UINT8 GetNextBarrelMode( UINT16 usItem, UINT8 aBarrelMode );
UINT8 GetFittingBarrelMode( UINT16 usItem, UINT8 aBarrelMode );	// return a number of barrels this gun can fire, equal or below aBarrelMode 
bool HasSeveralBarrelConfigurations( UINT16 usItem );

#endif
