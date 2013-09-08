#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"

	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Soldier macros.h"
	#include "SmokeEffects.h"
	#include "lighting.h"
	#include "Auto Resolve.h"
	#include "Soldier Functions.h" // added by SANDRO
	#include "Drugs And Alcohol.h" // HEADROCK HAM 4: Get drunk level
	#include "LOS.h" // HEADROCK HAM 4: Required for new shooting mechanism. Alternately, maybe move the functions to LOS.h.
	#include "Campaign Types.h"	// added by Flugente
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
#include "connect.h"

//rain
//#define WEAPON_RELIABILITY_REDUCTION_PER_RAIN_INTENSITY 0
extern INT8 gbCurrentRainIntensity;
//end rain

extern SECTOR_EXT_DATA	SectorExternalData[256][4];	// added by Flugente

// HEADROCK HAM B1: Externalized both values to INI
//#define MINCHANCETOHIT          1
//#define MAXCHANCETOHIT          99

#define NORMAL_RANGE            90      // # world units considered an 'avg' shot
#define MIN_SCOPE_RANGE         60     // # world units after which scope's useful

#define MIN_TANK_RANGE         120      // range at which tank starts really having trouble aiming

// percent reduction in sight range per point of aiming
#define SNIPERSCOPE_AIM_BONUS   20
// bonus to hit with working laser scope
#define LASERSCOPE_BONUS				20
// percent reduction in sight range per point of aiming
#define REFLEXSCOPE_AIM_BONUS   10

#define MANDATORY_WEAPON_DELAY		1200
#define NO_WEAPON_SOUND						0

#define HEAD_DAMAGE_ADJUSTMENT( x ) ((x * 3) / 2)
#define LEGS_DAMAGE_ADJUSTMENT( x ) (x / 2)

#define CRITICAL_HIT_THRESHOLD 30

#define HTH_MODE_PUNCH 1
#define HTH_MODE_STAB 2
#define HTH_MODE_STEAL 3

extern void TeamChangesSides( UINT8 ubTeam, INT8 bSide );

extern BOOLEAN	gfNextFireJam;

BOOLEAN WillExplosiveWeaponFail( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj );

BOOLEAN UseGun( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );
BOOLEAN UseGunNCTH( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );
BOOLEAN UseBlade( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );
BOOLEAN UseThrown( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );
BOOLEAN UseLauncher( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo );

// new subroutines for NCTH calculation
FLOAT CalcNewChanceToHitBaseAttributeBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitBaseEffectBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitBaseWeaponBonus(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, FLOAT fGunBaseDifficulty, UINT8 stance);
FLOAT CalcNewChanceToHitBaseSpecialBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitBaseTargetBonus(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget, INT32 sGridNo, INT32 iRange, UINT8 ubAimPos, BOOLEAN fCantSeeTarget);
FLOAT CalcNewChanceToHitAimAttributeBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitAimEffectBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitAimWeaponBonus(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, FLOAT fGunAimDifficulty, UINT8 stance);
FLOAT CalcNewChanceToHitAimSpecialBonus(SOLDIERTYPE *pSoldier);
FLOAT CalcNewChanceToHitAimTargetBonus(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget, INT32 sGridNo, INT32 iRange, UINT8 ubAimPos, BOOLEAN fCantSeeTarget);
FLOAT CalcNewChanceToHitAimTraitBonus(SOLDIERTYPE *pSoldier, FLOAT fAimCap, FLOAT fDifference, INT32 sGridNo, INT16 ubAimTime, FLOAT fScopeMagFactor, UINT32 uiBestScopeRange);

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack );

BOOLEAN gfNextShotKills = FALSE;
BOOLEAN gfReportHitChances = FALSE;

// HEADROCK HAM B2.5:Using this boolean to tell the CTH formula that it is being called by UseGun(), rather than
// by other functions. This is mostly to assist the new Tracer Fire system...
BOOLEAN fCalculateCTHDuringGunfire = FALSE;

//GLOBALS

// TODO: Move strings to extern file


#define PISTOL( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd)		{ HANDGUNCLASS, GUN_PISTOL,		ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_PISTOL, S_LNL_PISTOL }
#define M_PISTOL( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd)	{ HANDGUNCLASS, GUN_M_PISTOL,	ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_PISTOL, S_LNL_PISTOL }
#define SMG( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd )			{ SMGCLASS,			GUN_SMG,			ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_SMG, S_LNL_SMG }
#define SN_RIFLE( ammo, update, impact, rt, rof, burstrof, deadl, acc, clip, range, av, hv, sd, bsd )							{ RIFLECLASS,		GUN_SN_RIFLE,	ammo,				rt,								rof, burstrof, 0,					 update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_RIFLE, S_LNL_RIFLE }
#define RIFLE( ammo, update, impact, rt, rof, burstrof, deadl, acc, clip, range, av, hv, sd, bsd )								{ RIFLECLASS,		GUN_RIFLE,		ammo,				rt,								rof, burstrof, 0,					 update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_RIFLE, S_LNL_RIFLE }
#define ASRIFLE( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd )	{ RIFLECLASS,		GUN_AS_RIFLE,	ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_RIFLE, S_LNL_RIFLE }
#define SHOTGUN( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd )	{ SHOTGUNCLASS,	GUN_SHOTGUN,	ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_SHOTGUN, S_LNL_SHOTGUN }
#define LMG( ammo, update, impact, rt, rof, burstrof, burstpenal, deadl, acc, clip, range, av, hv, sd, bsd )			{ MGCLASS,			GUN_LMG,			ammo,				rt,								rof, burstrof, burstpenal, update, (UINT8) (impact), deadl, acc, clip, range, 200, av, hv, sd, bsd, S_RELOAD_LMG, S_LNL_LMG }
#define BLADE( impact, rof, deadl, range, av, sd )																																{ KNIFECLASS,		NOT_GUN,			0,					APBPConstants[AP_READY_KNIFE],		rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 range, 200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define THROWINGBLADE( impact, rof, deadl, range, av, sd )																												{ KNIFECLASS,		NOT_GUN,			0,					APBPConstants[AP_READY_KNIFE],		rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 range, 200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define PUNCHWEAPON( impact, rof, deadl, av, sd )																																	{ KNIFECLASS,		NOT_GUN,			0,					0,								rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 10,		200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define LAUNCHER( update, rt, rof, deadl, acc, range, av, hv, sd)																									{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 1,								 deadl, acc, 0,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define LAW( update, rt, rof, deadl, acc, range, av, hv, sd)																											{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 80,							 deadl, acc, 1,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define CANNON( update, rt, rof, deadl, acc, range, av, hv, sd)																										{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 80,							 deadl, acc, 1,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define MONSTSPIT( impact, rof, deadl, clip, range, av, hv, sd)																										{ MONSTERCLASS, NOT_GUN,			AMMOMONST,	APBPConstants[AP_READY_KNIFE],		rof, 0,				 0,					 250,		 (UINT8) (impact), deadl, 0,	 clip, range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }

// ranges are in world units, calculated by:
// 100 + real-range-in-metres/10
// then I scaled them down... I forget how much by!

// Accuracy is based on probability of shot being within 10cm of bullseye target on chest at 25m
// from Compendium of Modern Firearms (Edge of the Sword Vol 1)

// JA2 GOLD: reduced pistol ready time to 0, tweaked sniper rifle values and G11 range
WEAPONTYPE Weapon[ MAXITEMS ];

// MADD MARKER

MAGTYPE Magazine[MAXITEMS+1];
// =
//{
//	// calibre,			 mag size,			ammo type
//	{ AMMO9,						15,					AMMO_REGULAR },
//	{ AMMO9,						30,					AMMO_REGULAR },
//	{ AMMO9,						15,					AMMO_AP },
//	{ AMMO9,						30,					AMMO_AP },
//	{ AMMO9,						15,					AMMO_HP },
//	{ AMMO9,						30,					AMMO_HP },
//	{ AMMO38,						 6,					AMMO_REGULAR },
//	{ AMMO38,						 6,					AMMO_AP },
//	{ AMMO38,						 6,					AMMO_HP },
//	{ AMMO45,						 7,					AMMO_REGULAR },
//	{ AMMO45,						30,					AMMO_REGULAR },
//	{ AMMO45,						 7,					AMMO_AP },
//	{ AMMO45,						30,					AMMO_AP },
//	{ AMMO45,						 7,					AMMO_HP },
//	{ AMMO45,						30,					AMMO_HP },
//	{ AMMO357,					 6,					AMMO_REGULAR },
//	{ AMMO357,					 9,					AMMO_REGULAR },
//	{ AMMO357,					 6,					AMMO_AP },
//	{ AMMO357,					 9,					AMMO_AP },
//	{ AMMO357,					 6,					AMMO_HP },
//	{ AMMO357,					 9,					AMMO_HP },
//	{ AMMO545,					30,					AMMO_AP },
//	{ AMMO545,					30,					AMMO_HP },
//	{ AMMO556,					30,					AMMO_AP },
//	{ AMMO556,					30,					AMMO_HP },
//	{ AMMO762W,					10,					AMMO_AP },
//	{ AMMO762W,					30,					AMMO_AP },
//	{ AMMO762W,					10,					AMMO_HP },
//	{ AMMO762W,					30,					AMMO_HP },
//	{ AMMO762N,					 5,					AMMO_AP },
//	{ AMMO762N,					20,					AMMO_AP },
//	{ AMMO762N,					 5,					AMMO_HP },
//	{ AMMO762N,					20,					AMMO_HP },
//	{ AMMO47,						50,					AMMO_SUPER_AP },
//	{ AMMO57,						50,					AMMO_AP },
//	{ AMMO57,						50,					AMMO_HP },
//	{ AMMO12G,					7,					AMMO_REGULAR },
//	{ AMMO12G,					7,					AMMO_BUCKSHOT },
//	{ AMMOCAWS,					10,					AMMO_SUPER_AP },
//	{ AMMOCAWS,					10,					AMMO_BUCKSHOT },
//	{ AMMOROCKET,				5,					AMMO_SUPER_AP },
//	{ AMMOROCKET,				5,					AMMO_HE },
//	{ AMMOROCKET,				5,					AMMO_HEAT },
//	{ AMMODART,					1,					AMMO_SLEEP_DART },
//	{ AMMOFLAME,				5,					AMMO_BUCKSHOT },
//// MADD MARKER
//	{ AMMO50,				11,					AMMO_SUPER_AP },
//	{ AMMO9H,				20,					AMMO_SUPER_AP },
//	{ AMMO9,				50,					AMMO_REGULAR },
//	{ AMMO9,				50,					AMMO_AP },
//	{ AMMO9,				50,					AMMO_HP },
//	{ AMMO545,				75,					AMMO_AP },
//	{ AMMO545,				75,					AMMO_HP },
//	{ AMMO556,				200,					AMMO_AP },
//	{ AMMO556,				200,					AMMO_HP },
//	{ AMMO762N,				100,					AMMO_AP },
//	{ AMMO762N,				100,					AMMO_HP },
//	{ AMMO57,				20,					AMMO_AP },
//	{ AMMO57,				20,					AMMO_HP },
//	{ NOAMMO,						0,					0 }
//};

ARMOURTYPE Armour[MAXITEMS+1];// =
//{
//	//	Class					      Protection	Degradation%			Description
//	//  -------------       ----------  ------------      ----------------
//	{		ARMOURCLASS_VEST,				10,					25			}, /* Flak jacket     */
//	{		ARMOURCLASS_VEST,				13,					20			}, /* Flak jacket w X */
//	{		ARMOURCLASS_VEST,				16,					15			}, /* Flak jacket w Y */
//	{		ARMOURCLASS_VEST,				15,					20			}, /* Kevlar jacket   */
//	{		ARMOURCLASS_VEST,				19,					15			}, /* Kevlar jack w X */
//	{		ARMOURCLASS_VEST,				24,					10			}, /* Kevlar jack w Y */
//	{		ARMOURCLASS_VEST,				30,					15			}, /* Spectra jacket  */
//	{		ARMOURCLASS_VEST,				36,					10			}, /* Spectra jack w X*/
//	{		ARMOURCLASS_VEST,				42,					 5			}, /* Spectra jack w Y*/
//	{		ARMOURCLASS_LEGGINGS,		15,					20			}, /* Kevlar leggings */
//	{		ARMOURCLASS_LEGGINGS,		19,					15			}, /* Kevlar legs w X */
//
//	{		ARMOURCLASS_LEGGINGS,		24,					10			}, /* Kevlar legs w Y */
//	{		ARMOURCLASS_LEGGINGS,		30,					15			}, /* Spectra leggings*/
//	{		ARMOURCLASS_LEGGINGS,		36,					10			}, /* Spectra legs w X*/
//	{		ARMOURCLASS_LEGGINGS,		42,					 5			}, /* Spectra legs w Y*/
//	{		ARMOURCLASS_HELMET,			10,					 5			}, /* Steel helmet    */
//	{		ARMOURCLASS_HELMET,			15,					20			}, /* Kevlar helmet   */
//	{		ARMOURCLASS_HELMET,			19,					15			}, /* Kevlar helm w X */
//	{		ARMOURCLASS_HELMET,			24,					10			}, /* Kevlar helm w Y */
//	{		ARMOURCLASS_HELMET,			30,					15			}, /* Spectra helmet  */
//	{		ARMOURCLASS_HELMET,			36,					10			}, /* Spectra helm w X*/
//
//	{		ARMOURCLASS_HELMET,			42,					 5			}, /* Spectra helm w Y*/
//	{		ARMOURCLASS_PLATE,			15,					200			}, /* Ceramic plates  */
//	{		ARMOURCLASS_MONST,			 3,					 0	 		}, /* Infant creature hide */
//	{		ARMOURCLASS_MONST,			 5,					 0			}, /* Young male creature hide  */
//	{		ARMOURCLASS_MONST,			 6,					 0			}, /* Male creature hide  */
//	{		ARMOURCLASS_MONST,			20,					 0			}, /* Queen creature hide  */
//	{		ARMOURCLASS_VEST,				 2,					25			}, /* Leather jacket    */
//	{		ARMOURCLASS_VEST,				12,					30			}, /* Leather jacket w kevlar */
//	{		ARMOURCLASS_VEST,				16,					25			}, /* Leather jacket w kevlar & compound 18 */
//	{		ARMOURCLASS_VEST,				19,					20			}, /* Leather jacket w kevlar & queen blood */
//
//	{		ARMOURCLASS_MONST,			 7,					 0			}, /* Young female creature hide */
//	{		ARMOURCLASS_MONST,			 8,					 0			}, /* Old female creature hide  */
//	{		ARMOURCLASS_VEST,				 1,					25			}, /* T-shirt */
//	{		ARMOURCLASS_VEST,				22,					20			}, /* Kevlar 2 jacket   */
//	{		ARMOURCLASS_VEST,				27,					15			}, /* Kevlar 2 jack w X */
//	{		ARMOURCLASS_VEST,				32,					10			}, /* Kevlar 2 jack w Y */
//};

EXPLOSIVETYPE Explosive[MAXITEMS+1];// =
//{
//	//	Type							Yield		Yield2		Radius		Volume		Volatility	Animation			Description
//	//										-----		-------		------		------		----------	--------- 		------------------
//	{		EXPLOSV_STUN,			    1,			70*2,				4,				0,				0,					STUN_BLAST		/* stun grenade       */},
//	{		EXPLOSV_TEARGAS,	    0,			20*2,				4,				0,				0,					TARGAS_EXP		/* tear gas grenade   */},
//	{ 	EXPLOSV_MUSTGAS,	    15,			40,				4,				0,				0,					MUSTARD_EXP		/* mustard gas grenade*/},
//	{ 	EXPLOSV_NORMAL,		    15,			7,				3,				15,				1,					BLAST_1				/* mini hand grenade  */},
//	{ 	EXPLOSV_NORMAL,		    25,			10,				4,				25,				1,					BLAST_1				/* reg hand grenade   */},
//	{ 	EXPLOSV_NORMAL,		    40,			12,				5,				20,				10,					BLAST_2				/* RDX                */},
//	{ 	EXPLOSV_NORMAL,		    50,			15,				5,				50,				2,					BLAST_2				/* TNT (="explosives")*/},
//	{ 	EXPLOSV_NORMAL,		    60,			15,				6,				60,				2,					BLAST_2				/* HMX (=RDX+TNT)     */},
//	{ 	EXPLOSV_NORMAL,		    55,			15,				6,				55,				0,					BLAST_2				/* C1  (=RDX+min oil) */},
//	{ 	EXPLOSV_NORMAL,		    50,			22,				6,				50,				2,					BLAST_2				/* mortar shell       */},
//
//	{ 	EXPLOSV_NORMAL,		    30,			30,				2,				30,				2,					BLAST_1				/* mine               */},
//	{		EXPLOSV_NORMAL,		    65,			30,				7,				65,				0,					BLAST_1				/* C4  ("plastique")  */},
//	{		EXPLOSV_FLARE,		    0,			0,				10,				0,				0,					BLAST_1				/* trip flare				  */},
//	{		EXPLOSV_NOISE,		    0,			0,				50,				50,				0,					BLAST_1				/* trip klaxon        */},
//	{		EXPLOSV_NORMAL,		    20,			0,				1,				20,				0,					BLAST_1				/* shaped charge      */},
//	{		EXPLOSV_FLARE,		    0,			0,				10,				0,				0,					BLAST_1,			/* break light        */},
//	{		EXPLOSV_NORMAL,		    25,			5,				4,				25,				1,					BLAST_1,			/* GL grenade					*/},
//	{		EXPLOSV_TEARGAS,	    0,			20*2,				3,				0,				0,					TARGAS_EXP,		/* GL tear gas grenade*/},
//	{		EXPLOSV_STUN,			    1,			60*2,				4,				0,				0,					STUN_BLAST,	  /* GL stun grenade		*/},
//	{		EXPLOSV_SMOKE,		    0,			0,				3,				0,				0,					SMOKE_EXP,		/* GL smoke grenade		*/},
//
//	{		EXPLOSV_SMOKE,		    0,			0,				4,				0,				0,					SMOKE_EXP,		/* smoke grenade			*/},
//	{ 	EXPLOSV_NORMAL,		    60,			20,				6,				60,				2,					BLAST_2,			/* Tank Shell         */},
//	{ 	EXPLOSV_NORMAL,		    100,		0,				0,				0,				0,					BLAST_1,			/* Fake structure igniter*/},
//	{ 	EXPLOSV_NORMAL,		    100,		0,				1,				0,				0,					BLAST_1,			/* creature cocktail */},
//	{ 	EXPLOSV_NORMAL,		    50,			10,				5,				50,				2,					BLAST_2,			/* fake struct explosion*/},
//	{ 	EXPLOSV_NORMAL,		    50,			10,				5,				50,				2,					BLAST_3,			/* fake vehicle explosion*/},
//	{		EXPLOSV_TEARGAS,	    0,			40*2,				4,				0,				0,					TARGAS_EXP		/* big tear gas */},
//	{ 	EXPLOSV_CREATUREGAS,	5,			0,				1,				0,				0,					NO_BLAST		  /* small creature gas*/},
//	{ 	EXPLOSV_CREATUREGAS,	8,			0,				3,				0,				0,					NO_BLAST  		/* big creature gas*/},
//	{ 	EXPLOSV_CREATUREGAS,	0,			0,				0,				0,				0,					NO_BLAST  		/* vry sm creature gas*/},
//	{ 	EXPLOSV_NORMAL,		    55,			15,				6,				55,				0,					BLAST_2				/* RPG_HE_ROCKET */},
//	{ 	EXPLOSV_NORMAL,		    100,		10,				3,				75,				0,					BLAST_1				/* RPG_AP_ROCKET */},
//	{ 	EXPLOSV_NORMAL,		    25,			200,			8,				65,				0,					BLAST_2				/* RPG_FRAG_ROCKET */},
//};


CHAR8 gzBurstSndStrings[MAXITEMS*2][128];// =
//{
//	"",													// NOAMMO
//	"",													// 38
//	"9mm Burst ",								// 9mm
//	"45 Caliber Burst ",				// 45
//	"",													// 357
//	"",						              // 12G
//	"Shotgun Burst ",						// CAWS
//	"5,45 Burst ",							// 5.45
//	"5,56 Burst ",							// 5.56
//	"7,62 NATO Burst ",					// 7,62 N
//	"7,62 WP Burst ",						// 7,62 W
//	"4,7 Caliber Burst ",				// 4.7
//	"5,7 Burst ",								// 5,7
//	"",													// Monster
//	"RL Automatic ",            // Rocket
//	"",													// Dart
//	"",													// Flame (unused)
//	"",													// 50 Cal
//	"",													// 9MM HEavy
//};

AMMOTYPE AmmoTypes[MAXITEMS];

// the amount of momentum reduction for the head, torso, and legs
// used to determine whether the bullet will go through someone
UINT8 BodyImpactReduction[4] = { 0, 15, 30, 23 };

bool gbForceWeaponNotReady = false;
bool gbForceWeaponReady = false;


enum
{
	WEAPON_ELEMENT_NONE = 0,
	WEAPON_ELEMENT_WEAPONLIST,
	WEAPON_ELEMENT_WEAPON,
	WEAPON_ELEMENT_WEAPON_PROPERY,
}
typedef WEP_PARSE_STAGE;

struct
{
	WEP_PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	WEAPONTYPE		curWeapon;
	WEAPONTYPE *	curWeaponList;
	UINT32			maxWeapons;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef weaponParseData;

static void XMLCALL
weaponStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "WEAPONLIST") == 0 && pData->curElement == WEAPON_ELEMENT_NONE)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPONLIST;

			memset(pData->curWeaponList,0,sizeof(WEAPONTYPE)*pData->maxWeapons);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "WEAPON") == 0 && pData->curElement == WEAPON_ELEMENT_WEAPONLIST)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;

			memset(&pData->curWeapon,0,sizeof(WEAPONTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == WEAPON_ELEMENT_WEAPON &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szWeaponName") == 0 ||
				strcmp(name, "ubWeaponClass") == 0 ||
				strcmp(name, "ubWeaponType") == 0 ||
				strcmp(name, "ubCalibre") == 0 ||
				strcmp(name, "ubReadyTime") == 0 ||
				strcmp(name, "ubShotsPer4Turns") == 0 ||
				strcmp(name, "ubShotsPerBurst") == 0 ||
				strcmp(name, "ubBurstPenalty") == 0 ||
				strcmp(name, "ubBulletSpeed") == 0 ||
				strcmp(name, "ubImpact") == 0 ||
				strcmp(name, "ubDeadliness") == 0 ||
				strcmp(name, "bAccuracy") == 0 ||
				strcmp(name, "nAccuracy") == 0 ||
				strcmp(name, "ubMagSize") == 0 ||
				strcmp(name, "usRange")	 == 0 ||
				strcmp(name, "usReloadDelay") == 0 ||
				strcmp(name, "ubAttackVolume") == 0 ||
				strcmp(name, "ubHitVolume") == 0 ||
				strcmp(name, "sSound") == 0 ||
				strcmp(name, "sBurstSound") == 0 ||
				strcmp(name, "sSilencedBurstSound") == 0 ||
				strcmp(name, "sReloadSound") == 0 ||
				strcmp(name, "sLocknLoadSound") == 0 ||
				strcmp(name, "bBurstAP") == 0 ||
				strcmp(name, "bAutofireShotsPerFiveAP") == 0 ||
				strcmp(name, "APsToReload") == 0 ||
				strcmp(name, "SwapClips") == 0 ||
				strcmp(name, "AutoPenalty") == 0 ||
				strcmp(name, "NoSemiAuto") == 0 ||
				strcmp(name, "EasyUnjam") == 0 ||
				strcmp(name, "MaxDistForMessyDeath") == 0 ||
				strcmp(name, "SilencedSound") == 0 || // Lesh: add new field (OR operand)
                strcmp(name, "BurstAniDelay") == 0 || // Lesh: add new field (field itself)
				strcmp(name, "APsToReloadManually") == 0 ||
				strcmp(name, "ManualReloadSound") == 0 ||
				strcmp(name, "ubAimLevels") == 0  ||// HEADROCK HAM 4: Allowed aiming levels for this gun.
				strcmp(name, "bRecoilX") == 0 || // HEADROCK HAM 4:
				strcmp(name, "bRecoilY") == 0 || // HEADROCK HAM 4:
				strcmp(name, "ubRecoilDelay") == 0 || // HEADROCK HAM 4:
				strcmp(name, "Handling") == 0 || // CHRISL HAM 4
				strcmp(name, "usOverheatingJamThreshold") == 0 || // Flugente
				strcmp(name, "usOverheatingDamageThreshold") == 0 || // Flugente
				strcmp(name, "usOverheatingSingleShotTemperature") == 0 || // Flugente
				strcmp(name, "HeavyGun") == 0)) // SANDRO - cannot be shouldered while standing
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON_PROPERY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
weaponCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
weaponEndElementHandle(void *userData, const XML_Char *name)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "WEAPONLIST") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_NONE;
		}
		else if(strcmp(name, "WEAPON") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPONLIST;

			if(pData->curWeapon.uiIndex < pData->maxWeapons)
			{
				pData->curWeaponList[pData->curWeapon.uiIndex] = pData->curWeapon; //write the weapon into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.uiIndex = atol(pData->szCharData);
		}
		else if(strcmp(name, "szWeaponName") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;

			if(MAX_WEAPON_NAME_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curWeapon.szWeaponName,pData->szCharData);
			else
			{
				strncpy(pData->curWeapon.szWeaponName,pData->szCharData,MAX_WEAPON_NAME_LENGTH);
				pData->curWeapon.szWeaponName[MAX_WEAPON_NAME_LENGTH] = '\0';
			}
		}
		else if(strcmp(name, "ubWeaponClass") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubWeaponClass = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubWeaponType") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubWeaponType = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubCalibre") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubCalibre = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubReadyTime") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubReadyTime = (UINT8) atol(pData->szCharData);
			pData->curWeapon.ubReadyTime = (UINT8)DynamicAdjustAPConstants(pData->curWeapon.ubReadyTime, pData->curWeapon.ubReadyTime);
		}
		else if(strcmp(name, "ubShotsPer4Turns") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubShotsPer4Turns = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "ubShotsPerBurst") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubShotsPerBurst = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubBurstPenalty") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubBurstPenalty = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubBulletSpeed") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubBulletSpeed = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubImpact") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubImpact = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDeadliness") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubDeadliness = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bAccuracy") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bAccuracy = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "nAccuracy") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.nAccuracy = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMagSize") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubMagSize = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usRange")	 == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usRange = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usReloadDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usReloadDelay = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubAttackVolume") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubAttackVolume = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubHitVolume") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubHitVolume = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "sSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sBurstSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sBurstSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sSilencedBurstSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sSilencedBurstSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sReloadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sReloadSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sLocknLoadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sLocknLoadSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bBurstAP") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bBurstAP = (UINT8) atol(pData->szCharData);
			pData->curWeapon.bBurstAP = (UINT8)DynamicAdjustAPConstants(pData->curWeapon.bBurstAP, pData->curWeapon.bBurstAP);
		}
		else if(strcmp(name, "bAutofireShotsPerFiveAP") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bAutofireShotsPerFiveAP = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "APsToReload") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.APsToReload = (UINT8) atol(pData->szCharData);
			pData->curWeapon.APsToReload = (UINT8)DynamicAdjustAPConstants(pData->curWeapon.APsToReload, pData->curWeapon.APsToReload);
		}
		else if(strcmp(name, "SilencedSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.silencedSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "SwapClips") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.swapClips = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "MaxDistForMessyDeath") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.maxdistformessydeath = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "AutoPenalty") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.AutoPenalty = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "NoSemiAuto") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.NoSemiAuto = (BOOLEAN) atol(pData->szCharData);
		}
        // Lesh: xmlreader must handle new field
   		else if(strcmp(name, "BurstAniDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.sAniDelay = (INT16) atol(pData->szCharData);
		}
        // Lesh: end
   		else if(strcmp(name, "APsToReloadManually") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.APsToReloadManually = (UINT8) atol(pData->szCharData);
			pData->curWeapon.APsToReloadManually = (UINT8)DynamicAdjustAPConstants(pData->curWeapon.APsToReloadManually, pData->curWeapon.APsToReloadManually);
		}
   		else if(strcmp(name, "ManualReloadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.ManualReloadSound = (UINT16) atol(pData->szCharData);
		}
   		else if(strcmp(name, "EasyUnjam") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.EasyUnjam = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubAimLevels") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubAimLevels = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bRecoilX") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bRecoilX = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bRecoilY") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bRecoilY = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubRecoilDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubRecoilDelay = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Handling") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubHandling = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usOverheatingJamThreshold") == 0)			// Flugente FTW 1
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usOverheatingJamThreshold = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "usOverheatingDamageThreshold") == 0)			// Flugente FTW 1
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usOverheatingDamageThreshold = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "usOverheatingSingleShotTemperature") == 0)			// Flugente FTW 1
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usOverheatingSingleShotTemperature = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "HeavyGun") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.HeavyGun = (BOOLEAN) atof(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInWeaponStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	weaponParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading weapons.xml" );

	// Open weapons file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );


	XML_SetElementHandler(parser, weaponStartElementHandle, weaponEndElementHandle);
	XML_SetCharacterDataHandler(parser, weaponCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curWeaponList = Weapon;
	pData.maxWeapons = MAXITEMS;

	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Weapons.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteWeaponStats()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Weapons out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			CHAR8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<WEAPON>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									cnt);//Weapon[cnt].uiIndex);

			FilePrintf(hFile,"\t\t<szWeaponName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				INT8 invChar = szRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder);
					szRemainder[uiCharLoc] = invChar;
				}

				szRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						szRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						szRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						szRemainder++;
					break;

					case '\'':
						FilePrintf(hFile,"&apos;");
						szRemainder++;
					break;

					case '\"':
						FilePrintf(hFile,"&quot;");
						szRemainder++;
					break;
				}
			}
			FilePrintf(hFile,"</szWeaponName>\r\n");

			FilePrintf(hFile,"\t\t<ubWeaponClass>%d</ubWeaponClass>\r\n",						Weapon[cnt].ubWeaponClass);
			FilePrintf(hFile,"\t\t<ubWeaponType>%d</ubWeaponType>\r\n",							Weapon[cnt].ubWeaponType);
			FilePrintf(hFile,"\t\t<ubCalibre>%d</ubCalibre>\r\n",								Weapon[cnt].ubCalibre);
			FilePrintf(hFile,"\t\t<ubReadyTime>%d</ubReadyTime>\r\n",							Weapon[cnt].ubReadyTime);
			FilePrintf(hFile,"\t\t<ubShotsPer4Turns>%d</ubShotsPer4Turns>\r\n",					Weapon[cnt].ubShotsPer4Turns);
			FilePrintf(hFile,"\t\t<ubShotsPerBurst>%d</ubShotsPerBurst>\r\n",					Weapon[cnt].ubShotsPerBurst);
			FilePrintf(hFile,"\t\t<ubBurstPenalty>%d</ubBurstPenalty>\r\n",						Weapon[cnt].ubBurstPenalty);
			FilePrintf(hFile,"\t\t<bRecoilX>%d<\bRecoilX>",										Weapon[cnt].bRecoilX);
			FilePrintf(hFile,"\t\t<bRecoilY>%d<\bRecoilY>",										Weapon[cnt].bRecoilY);
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
			FilePrintf(hFile,"\t\t<nAccuracy>%d</nAccuracy>\r\n",								Weapon[cnt].nAccuracy);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Weapon[cnt].ubMagSize);
			FilePrintf(hFile,"\t\t<usRange>%d</usRange>\r\n",									Weapon[cnt].usRange);
			FilePrintf(hFile,"\t\t<usReloadDelay>%d</usReloadDelay>\r\n",						Weapon[cnt].usReloadDelay);
			FilePrintf(hFile,"\t\t<BurstAniDelay>%d</BurstAniDelay>\r\n",						Weapon[cnt].sAniDelay);
			FilePrintf(hFile,"\t\t<ubAttackVolume>%d</ubAttackVolume>\r\n",						Weapon[cnt].ubAttackVolume);
			FilePrintf(hFile,"\t\t<ubHitVolume>%d</ubHitVolume>\r\n",							Weapon[cnt].ubHitVolume);
			FilePrintf(hFile,"\t\t<sSound>%d</sSound>\r\n",										Weapon[cnt].sSound);
			FilePrintf(hFile,"\t\t<sBurstSound>%d</sBurstSound>\r\n",							Weapon[cnt].sBurstSound);
			FilePrintf(hFile,"\t\t<sSilencedBurstSound>%d</sSilencedBurstSound>\r\n",							Weapon[cnt].sSilencedBurstSound);
			FilePrintf(hFile,"\t\t<sReloadSound>%d</sReloadSound>\r\n",							Weapon[cnt].sReloadSound);
			FilePrintf(hFile,"\t\t<sLocknLoadSound>%d</sLocknLoadSound>\r\n",					Weapon[cnt].sLocknLoadSound);
			FilePrintf(hFile,"\t\t<SilencedSound>%d</SilencedSound>\r\n",						Weapon[cnt].silencedSound  );
			FilePrintf(hFile,"\t\t<bBurstAP>%d</bBurstAP>\r\n",				Weapon[cnt].bBurstAP);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t\t<APsToReload>%d</APsToReload>\r\n",							Weapon[cnt].APsToReload);
			FilePrintf(hFile,"\t\t<SwapClips>%d</SwapClips>\r\n",								Weapon[cnt].swapClips );
			FilePrintf(hFile,"\t\t<MaxDistForMessyDeath>%d</MaxDistForMessyDeath>\r\n",			Weapon[cnt].maxdistformessydeath);
			FilePrintf(hFile,"\t\t<AutoPenalty>%d</AutoPenalty>\r\n",			Weapon[cnt].AutoPenalty);
			FilePrintf(hFile,"\t\t<NoSemiAuto>%d</NoSemiAuto>\r\n",			Weapon[cnt].NoSemiAuto);
			FilePrintf(hFile,"\t\t<ubAimLevels>%d</ubAimLevels>\r\n",							Weapon[cnt].ubAimLevels );
			FilePrintf(hFile,"\t\t<EasyUnjam>%d</EasyUnjam>\r\n",			Weapon[cnt].EasyUnjam);
			FilePrintf(hFile,"\t\t<Handling>%d</Handling>\r\n",			Weapon[cnt].ubHandling);
			FilePrintf(hFile,"\t\t<usOverheatingJamThreshold>%4.2f</usOverheatingJamThreshold>\r\n",			Weapon[cnt].usOverheatingJamThreshold); // Flugente FTW 1
			FilePrintf(hFile,"\t\t<usOverheatingDamageThreshold>%4.2f</usOverheatingDamageThreshold>\r\n",			Weapon[cnt].usOverheatingDamageThreshold);
			FilePrintf(hFile,"\t\t<usOverheatingSingleShotTemperature>%4.2f</usOverheatingSingleShotTemperature>\r\n",			Weapon[cnt].usOverheatingSingleShotTemperature);
			FilePrintf(hFile,"\t\t<HeavyGun>%d</HeavyGun>\r\n",			Weapon[cnt].HeavyGun);
			

			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}

UINT16 GunRange( OBJECTTYPE * pObj, SOLDIERTYPE * pSoldier ) // SANDRO - added argument
{
	//INT8 bAttachPos;
	UINT16 rng;

	if ( Item[ pObj->usItem ].usItemClass & IC_WEAPON )
	{


		//bAttachPos = FindAttachment( pObj, GUN_BARREL_EXTENDER );

		//if ( bAttachPos == ITEM_NOT_FOUND )
		//{
		//	rng = ( Weapon[ pObj->usItem ].usRange );
		//	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GunRange: base rng=%d",rng));
		//}
		//else
		//{
		//	rng = ( Weapon[ pObj->usItem ].usRange + (GUN_BARREL_RANGE_BONUS * WEAPON_STATUS_MOD(pObj->bAttachStatus[ bAttachPos ]) / 100 ) );
		//	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GunRange: Found extender rng=%d",rng));
		//}

		//if ( Weapon [ pObj->usItem ].ubWeaponType == GUN_SHOTGUN && (*pObj)[0]->data.gun.ubGunAmmoType != AMMO_BUCKSHOT )
		//{
		//	rng += SOLID_SLUG_RANGE_BONUS;
		//	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GunRange: Found solid slug rng=%d",rng));
		//}
		//else
		//{
		//	bAttachPos = FindAttachment( pObj, DUCKBILL );
		//	if ( bAttachPos != ITEM_NOT_FOUND )
		//	{
		//		rng += DUCKBILL_RANGE_BONUS * (WEAPON_STATUS_MOD(pObj->bAttachStatus[ bAttachPos ]) / 100 );
		//		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("GunRange: Found duckbill rng=%d",rng));
		//	}
		//}


		UINT16 usRange = GetModifiedGunRange(pObj->usItem);

		// Snap: attachment status is factored into the range bonus calculation
		rng = (usRange * GetPercentRangeBonus(pObj))/10000; 
		rng += GetRangeBonus(pObj);

		// SANDRO - STOMP traits - Gunslinger bonus range with pistols
		if ( pSoldier != NULL && Item[ pObj->usItem ].usItemClass & IC_GUN )
		{
			if ((Weapon[ pObj->usItem ].ubWeaponType == GUN_PISTOL || Weapon[ pObj->usItem ].ubWeaponType == GUN_M_PISTOL) &&
				gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) )
			{
				rng += (rng * gSkillTraitValues.ubGSEffectiveRangeBonusPistols * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ) / 100 );
			}
		}

		return rng;
	}
	else
	{
		// return a minimal value of 1 tile
		return( CELL_X_SIZE );
	}
}

INT32 EffectiveArmour( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_ARMOUR)
	{
		return( 0 );
	}
	iValue = Armour[ Item[pObj->usItem].ubClassIndex ].ubProtection;
	iValue = iValue * (*pObj)[0]->data.objectStatus * Armour[ Item[pObj->usItem].ubClassIndex ].ubCoverage / 10000;

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 && iter->exists())
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 = iValue2 * (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,1) );
}

//zwwooooo - IoV: Lbe can be bulletproof after adding bulletproof plate into it.(Like CRIAS, MBSS, HSGI WASATCH...)
INT32 EffectiveArmourLBE( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_LBEGEAR)
	{
		return( 0 );
	}
	
	iValue = 0;
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 = iValue2 * (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,0) );
}

//zwwooooo - IoV: Lbe can be bulletproof after adding bulletproof plate into it.(Like CRIAS, MBSS, HSGI WASATCH...)
INT32 ExplosiveEffectiveArmourLBE( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_LBEGEAR)
	{
		return( 0 );
	}
	
	iValue = 0;
	
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 *= (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,0) );
}

INT32 ArmourPercent( SOLDIERTYPE * pSoldier )
{
	INT32 iVest, iHelmet, iLeg;
	INT32 iDivideValue;

	if (pSoldier->inv[VESTPOS].exists() == true)
	{
		iVest = EffectiveArmour( &(pSoldier->inv[VESTPOS]) );
		iDivideValue = ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) );

		// WANNE: Just to be on the save side
		if (iDivideValue > 0)
		{
			// convert to % of best; ignoring bug-treated stuff
			iVest = 6500 * iVest / iDivideValue;
		}
		else
		{
			iVest = 65 * iVest / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
		}
	}
	else
	{
		iVest = 0;
	}

	if (pSoldier->inv[HELMETPOS].exists() == true)
	{
		iHelmet = EffectiveArmour( &(pSoldier->inv[HELMETPOS]) );
		iDivideValue = ( Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubCoverage );

		// WANNE: Just to be on the save side
		if (iDivideValue > 0)
		{
			// convert to % of best; ignoring bug-treated stuff
			iHelmet = 1500 * iHelmet / iDivideValue;
		}
		else
		{
			iHelmet = 15 * iHelmet / Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection;
		}
	}
	else
	{
		iHelmet = 0;
	}

	if (pSoldier->inv[LEGPOS].exists() == true)
	{
		iLeg = EffectiveArmour( &(pSoldier->inv[LEGPOS]) );
		iDivideValue = ( Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubCoverage );

		// WANNE: Just to be on the save side
		if (iDivideValue > 0)
		{
			// convert to % of best; ignoring bug-treated stuff
			iLeg = 2500 * iLeg / iDivideValue;
		}
		else
		{
			iLeg = 25 * iLeg / Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection;
		}
	}
	else
	{
		iLeg = 0;
	}

	//zwwooooo - IoV: Add iVestPack
	INT32 iVestPack;
	if (pSoldier->inv[VESTPOCKPOS].exists() == true)
	{
		iVestPack = EffectiveArmourLBE( &(pSoldier->inv[VESTPOCKPOS]) );
		iDivideValue = ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) );

		// WANNE: Just to be on the save side
		if (iDivideValue > 0)
		{
			// convert to % of best; ignoring bug-treated stuff
			iVestPack = 6500 * iVestPack / iDivideValue;
}
		else
		{
			iVestPack = 65 * iVestPack / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
		}
	}
	else
	{
		iVestPack = 0;
	}

	//return( (iHelmet + iVest + iLeg) );
	//zwwooooo - IoV: Add iVestPack
	return( (iHelmet + iVest + iLeg + iVestPack) );
}

INT32 ExplosiveEffectiveArmour( OBJECTTYPE * pObj )
{
	INT32		iValue;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_ARMOUR)
	{
		return( 0 );
	}
	iValue = Armour[ Item[pObj->usItem].ubClassIndex ].ubProtection;
	iValue = iValue * (*pObj)[0]->data.objectStatus * Armour[ Item[pObj->usItem].ubClassIndex ].ubCoverage / 10000;
	if ( Item[pObj->usItem].flakjacket )
	{
		// increase value for flak jackets!
		iValue *= 3;
	}

	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 && iter->exists())
		{
			INT32 iValue2;

			iValue2 = Armour[ Item[ iter->usItem ].ubClassIndex ].ubProtection;
			iValue2 *= (*iter)[0]->data.objectStatus * Armour[ Item[ iter->usItem ].ubClassIndex ].ubCoverage / 10000;

			iValue += iValue2;
		}
	}
	return( max(iValue,1) );
}

INT8 ArmourVersusExplosivesPercent( SOLDIERTYPE * pSoldier )
{
	// returns the % damage reduction from grenades
	INT32 iVest, iHelmet, iLeg;

	if (pSoldier->inv[VESTPOS].exists() == true)
	{
		iVest = ExplosiveEffectiveArmour( &(pSoldier->inv[VESTPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iVest = __min( 65, 6500 * iVest / ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) ) );
	}
	else
	{
		iVest = 0;
	}

	if (pSoldier->inv[HELMETPOS].exists() == true)
	{
		iHelmet = ExplosiveEffectiveArmour( &(pSoldier->inv[HELMETPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iHelmet = __min( 15, 1500 * iHelmet / ( Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubCoverage ) );
	}
	else
	{
		iHelmet = 0;
	}

	if (pSoldier->inv[LEGPOS].exists() == true)
	{
		iLeg = ExplosiveEffectiveArmour( &(pSoldier->inv[LEGPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iLeg = __min( 25, 2500 * iLeg / ( Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubCoverage ) );
	}
	else
	{
		iLeg = 0;
	}

	//zwwooooo - IoV: Add iVestPack
	INT32 iVestPack;
	if (pSoldier->inv[VESTPOCKPOS].exists() == true)
	{
		iVestPack = ExplosiveEffectiveArmourLBE( &(pSoldier->inv[VESTPOCKPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iVestPack = __min( 65, 6500 * iVestPack / ( ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection * Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubCoverage ) + ( Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection * Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubCoverage ) ) );
}
	else
	{
		iVestPack = 0;
	}

	//return( (INT8) (iHelmet + iVest + iLeg) );
	//zwwooooo - IoV: Add iVestPack
	return( (INT8) (iHelmet + iVest + iLeg + iVestPack) );
}

void AdjustImpactByHitLocation( INT32 iImpact, UINT8 ubHitLocation, INT32 * piNewImpact, INT32 * piImpactForCrits )
{
	switch( ubHitLocation )
	{
		case AIM_SHOT_HEAD:
			// 1.5x damage from successful hits to the head!
			//*piImpactForCrits = HEAD_DAMAGE_ADJUSTMENT( iImpact ); //comm by ddd
			*piImpactForCrits = INT32(gGameExternalOptions.fShotHeadDivisor*iImpact);
			*piNewImpact = *piImpactForCrits;
			break;
		case AIM_SHOT_LEGS:
			// half damage for determining critical hits
			// quarter actual damage
			*piImpactForCrits = LEGS_DAMAGE_ADJUSTMENT( iImpact );
			*piNewImpact = LEGS_DAMAGE_ADJUSTMENT( *piImpactForCrits );
			break;
		default:
			*piImpactForCrits = iImpact;
			*piNewImpact = iImpact;
			break;
	}
}


// #define	TESTGUNJAM

//rain
extern INT8 gbCurrentRainIntensity;
//end rain
BOOLEAN CheckForGunJam( SOLDIERTYPE * pSoldier ) 
{ 
	OBJECTTYPE * pObj; 
	// INT32 iChance, iResult; 
 
	// should jams apply to enemies? 
	if (pSoldier->flags.uiStatusFlags & SOLDIER_PC) 
	{ 
		if ( Item[pSoldier->usAttackingWeapon].usItemClass == IC_GUN && !EXPLOSIVE_GUN( pSoldier->usAttackingWeapon ) && !(pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO) ) 
		{ 
			pObj = pSoldier->GetUsedWeapon(&pSoldier->inv[pSoldier->ubAttackingHand]);

			if ((*pObj)[0]->data.gun.bGunAmmoStatus > 0) 
			{ 
				// Algorithm for jamming 
				int maxJamChance = 50; // Externalize this? 
				int reliability =  GetReliability( pObj ); 
				int condition = (*pObj)[0]->data.gun.bGunStatus; 
				int invertedBaseJamChance = condition + (reliability * 2) - 
					gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity * gbCurrentRainIntensity; 

				// Flugente: If overheating is allowed, a gun will be prone to more overheating if its temperature is high
				if ( gGameExternalOptions.fWeaponOverheating )
				{
					FLOAT overheatjampercentage = GetGunOverheatJamPercentage( pObj );	// how much above the gun's usOverheatingJamThreshold are we? ...

					int overheatjamfactor = (int)(100* overheatjampercentage);			// We need an integer value and rough percentages

					overheatjamfactor = max(0, overheatjamfactor - 100);				// If we haven't reached the OverheatJamThreshold, no increased chance of jamming because of overheating

					invertedBaseJamChance -= overheatjamfactor;							// lower invertedBaseJamChance	(thereby increasing jamChance later on)
				}

				// Flugente: dirt can also influence a gun's jamming behaviour
				if ( gGameExternalOptions.fDirtSystem )
				{
					FLOAT dirtpercentage = (*pObj)[0]->data.bDirtLevel / OVERHEATING_MAX_TEMPERATURE;
					
					int dirtjamfactor = (int)(100 * dirtpercentage*dirtpercentage);
					
					invertedBaseJamChance -= dirtjamfactor;	
				}

				if (invertedBaseJamChance < 0) 
					invertedBaseJamChance = 0; 
				else if (invertedBaseJamChance > 100) 
					invertedBaseJamChance = 100; 
				int jamChance = 100;
				if ( pSoldier->ubAttackingHand == SECONDHANDPOS && pSoldier->IsValidSecondHandBurst() ) 
					jamChance -= (int)sqrt((double)invertedBaseJamChance * ((75.0-(int)((pSoldier->bDoBurst/2)>1)*15) + (double)invertedBaseJamChance / 2.0)); 
				else
					jamChance -= (int)sqrt((double)invertedBaseJamChance * ((75.0-(int)(pSoldier->bDoBurst>1)*15) + (double)invertedBaseJamChance / 2.0)); 
				if (jamChance < 0) 
					jamChance = 0; 
				else if (jamChance > maxJamChance - reliability) 
					jamChance = maxJamChance - reliability; 
			 
				/* Old jam code 
				// gun might jam, figure out the chance 
				//iChance = (80 - pObj->bGunStatus); 
			 
				//rain 
				iChance = (80 - pObj->ItemData.Gun.bGunStatus) + gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity * gbCurrentRainIntensity; 
				//end rain 
			 
				// CJC: removed reliability from formula... 
			 
				// jams can happen to unreliable guns "earlier" than normal or reliable ones. 
				//iChance = iChance - Item[pObj->usItem].bReliability * 2; 
			 
				// decrease the chance of a jam by 20% per point of reliability; 
				// increased by 20% per negative point... 
				//iChance = iChance * (10 - Item[pObj->usItem].bReliability * 2) / 10; 
			 
				//rain 
				// iChance = iChance * (10 - Item[pObj->usItem].bReliability * 2) / 10; // Madd: took it back out 
				//end rain 
			 
				if (pSoldier->bDoBurst > 1) 
				{ 
				// if at bullet in a burst after the first, higher chance 
				iChance -= PreRandom( 80 ); 
				} 
				else 
				{ 
				iChance -= PreRandom( 100 ); 
				} 
			*/ 
#ifdef TESTGUNJAM 
				if ( 1 ) 
#else 
				if ((INT32) PreRandom( 100 ) < jamChance || gfNextFireJam ) 
#endif 
				{ 
					gfNextFireJam = FALSE; 
				 
					// jam! negate the gun ammo status. 
					(*pObj)[0]->data.gun.bGunAmmoStatus *= -1; 
				 
					// Deduct AMMO! 
					DeductAmmo( pSoldier, pSoldier->ubAttackingHand ); 
				 
					TacticalCharacterDialogue( pSoldier, QUOTE_JAMMED_GUN ); 
					return( TRUE ); 
				} 
			} 
			else if ((*pObj)[0]->data.gun.bGunAmmoStatus < 0) 
			{ 
			// try to unjam gun 
				if(EnoughPoints(pSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM], FALSE))
				{
					DeductPoints(pSoldier, APBPConstants[AP_UNJAM], APBPConstants[BP_UNJAM] );
					INT8 bChanceMod;
					
					if ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].EasyUnjam )
						bChanceMod = 100;
					else
						bChanceMod = (INT8) (GetReliability( pObj )* 4);
					
					int iResult = SkillCheck( pSoldier, UNJAM_GUN_CHECK, bChanceMod); 
					
					if (iResult > 0) 
					{ 
						// yay! unjammed the gun 
						(*pObj)[0]->data.gun.bGunAmmoStatus *= -1; 
					 
						// MECHANICAL/DEXTERITY GAIN: Unjammed a gun 
						
						if (bChanceMod < 100) // don't give exp for unjamming an easily unjammable gun
						{
							StatChange( pSoldier, MECHANAMT, 5, FALSE ); 
							StatChange( pSoldier, DEXTAMT, 5, FALSE ); 
						}
					 
						DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 ); 
					 
						// We unjammed gun, return appropriate value! 
						return( 255 ); 
					} 
					else 
					{ 
						return( TRUE ); 
					} 
				}
				else
					return( TRUE );
			} 
		} 
	} 
	return( FALSE ); 
} 


BOOLEAN	OKFireWeapon( SOLDIERTYPE *pSoldier )
{
	BOOLEAN bGunJamVal;

	// 1) Are we attacking with our second hand?
	if ( pSoldier->ubAttackingHand == SECONDHANDPOS )
	{
		//if ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].NoSemiAuto ) // Madd: no autofire w/second hand (yet)
		//{
		//	return( FALSE );
		//}

		if ( !EnoughAmmo( pSoldier, FALSE, pSoldier->ubAttackingHand ) )
		{
			if ( pSoldier->bTeam == gbPlayerNum	 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_2ND_CLIP_DEPLETED ] );
				return( FALSE );
			}

		}
	}

	bGunJamVal = CheckForGunJam( pSoldier );

	if ( bGunJamVal == 255 )
	{
		return( 255 );
	}

	if ( bGunJamVal )
	{
		return( FALSE );
	}

	return( TRUE );
}


// Flugente FTW 1: Get percentage: temperature/damagethreshold
FLOAT   GetGunOverheatDamagePercentage( FLOAT usTemperature, UINT16 usIndx )
{
	FLOAT damagethreshold = Weapon[Item[ usIndx ].ubClassIndex].usOverheatingDamageThreshold;

	return usTemperature/ damagethreshold ;
}

// Flugente FTW 1: Get percentage: temperature/jamthreshold
FLOAT   GetGunOverheatJamPercentage( FLOAT usTemperature, UINT16 usIndx )
{
	FLOAT jamthreshold = Weapon[Item[ usIndx ].ubClassIndex].usOverheatingJamThreshold;

	return usTemperature/ jamthreshold ;
}

BOOLEAN FireWeapon( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo )
{
	// ignore passed in target gridno for now
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("FireWeapon"));
	// If realtime and we are reloading - do not fire until counter is done!
	if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) && !pSoldier->bDoBurst )
	{
		if ( pSoldier->flags.fReloading )
		{
			return( FALSE );
		}
	}

	// if target gridno is the same as ours, do not fire!
	if (sTargetGridNo == pSoldier->sGridNo)
	{
		// FREE UP NPC!
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - attack on own gridno!") );
		FreeUpAttacker( );
		return( FALSE );
	}

	// SET ATTACKER TO NOBODY, WILL GET SET EVENTUALLY
	pSoldier->ubOppNum = NOBODY ;

	UINT32 usItemClass = Item[ pSoldier->usAttackingWeapon ].usItemClass;
	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
		usItemClass = IC_LAUNCHER;
	if ( pSoldier->bWeaponMode == WM_ATTACHED_BAYONET )
		usItemClass = IC_BLADE;

	switch( usItemClass )
	{
		case IC_THROWING_KNIFE:
		case IC_GUN:

			if ( pSoldier->bWeaponMode == WM_ATTACHED_GL )
				UseLauncher ( pSoldier, sTargetGridNo );
			else
			{
				// ATE: PAtch up - bookkeeping for spreading done out of whak
				if ( pSoldier->flags.fDoSpread)
				{

					// 0verhaul:  This check does not work!  All auto-fire has bDoBurst turned on.  And only allowing a spread
					// for a single-shot mode is useless.
					//if (pSoldier->flags.fDoBurst )
					//{
					//	pSoldier->flags.fDoSpread = FALSE;
					//}
					//else 
					// 0verhaul:  The original code seemed brain damaged:  If the current spread target was 0 it would shoot at the
					// non-spread target grid # instead.  Also fDoSpread is used as a counter from 1 to MAX_BURST_SPREAD_TARGETS,
					// but was actually reset before it got there.  So the final spread target would never be shot at.  Hopefully this
					// will work better.
					if (pSoldier->flags.fDoSpread > MAX_BURST_SPREAD_TARGETS ||
						pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] == 0)
					{
						if (pSoldier->flags.fDoSpread == 1)
						{
							// If no spread locations are defined, don't spread
							pSoldier->flags.fDoSpread = 0;
						}
						else
						{
							// If we hit the end of the array, either by finding a 0 or by exceeding its size, reset
							pSoldier->flags.fDoSpread = 1;
						}
					}
				}


				if ( pSoldier->flags.fDoSpread )
				{
					UseGun( pSoldier, pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] );
					pSoldier->flags.fDoSpread++;
				}
				else
				{
					UseGun( pSoldier, sTargetGridNo );
				}
			}
			break;
		case IC_BLADE:

			UseBlade( pSoldier, sTargetGridNo );
			break;
		case IC_PUNCH:
			UseHandToHand( pSoldier, sTargetGridNo, FALSE );
			break;

		case IC_LAUNCHER:
			if ( Item[pSoldier->usAttackingWeapon].rocketlauncher ){
				UseGun( pSoldier, sTargetGridNo );
			} else {
				// ATE: PAtch up - bookkeeping for spreading done out of whak
				if ( pSoldier->flags.fDoSpread)
				{
					if (!pSoldier->bDoBurst )
					{
						pSoldier->flags.fDoSpread = FALSE;
					}

					// 0verhaul:  The original code seemed brain damaged:  If the current spread target was 0 it would shoot at the
					// non-spread target grid # instead.  Also fDoSpread is used as a counter from 1 to MAX_BURST_SPREAD_TARGETS,
					// but was actually reset before it got there.  So the final spread target would never be shot at.  Hopefully this
					// will work better.
					else if ( pSoldier->flags.fDoSpread > MAX_BURST_SPREAD_TARGETS ||
						pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] == 0)
					{
						if (pSoldier->flags.fDoSpread == 1)
						{
							// If no spread locations are defined, don't spread
							pSoldier->flags.fDoSpread = 0;
						}
						else
						{
							// If we hit the end of the array, either by finding a 0 or by exceeding its size, reset
							pSoldier->flags.fDoSpread = 1;
						}
					}
				}

				if ( pSoldier->flags.fDoSpread )
				{
					UseLauncher( pSoldier, pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] );
					pSoldier->flags.fDoSpread++;
				}
				else
				{
					UseLauncher( pSoldier, sTargetGridNo );
				}
			}

			break;

		default:
			// attempt to throw
			UseThrown( pSoldier, sTargetGridNo );
			break;
	}
	return( TRUE );
}


void GetTargetWorldPositions( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo, FLOAT *pdXPos, FLOAT *pdYPos, FLOAT *pdZPos )
{
	FLOAT								dTargetX;
	FLOAT								dTargetY;
	FLOAT								dTargetZ;
	SOLDIERTYPE					*pTargetSoldier;
	INT8								bStructHeight;
	INT16								sXMapPos, sYMapPos;
	UINT32							uiRoll;

	pTargetSoldier = SimpleFindSoldier( sTargetGridNo, pSoldier->bTargetLevel );
	if ( pTargetSoldier )
	{
		// SAVE OPP ID
		pSoldier->ubOppNum = pTargetSoldier->ubID;
		dTargetX = (FLOAT) CenterX( pTargetSoldier->sGridNo );
		dTargetY = (FLOAT) CenterY( pTargetSoldier->sGridNo );
		if (pSoldier->bAimShotLocation == AIM_SHOT_RANDOM)
		{
			uiRoll = PreRandom( 100 );
			if (uiRoll < 15)
			{
				pSoldier->bAimShotLocation = AIM_SHOT_LEGS;
			}
			else if (uiRoll > 94)
			{
				pSoldier->bAimShotLocation = AIM_SHOT_HEAD;
			}
			else
			{
				pSoldier->bAimShotLocation = AIM_SHOT_TORSO;
			}
			if ( pSoldier->bAimShotLocation != AIM_SHOT_HEAD )
			{
				UINT32 uiChanceToGetThrough = SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, pTargetSoldier, pSoldier->bAimShotLocation );

				if ( uiChanceToGetThrough < 25 )
				{
					if ( SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, pTargetSoldier, AIM_SHOT_HEAD ) > uiChanceToGetThrough * 2 )
					{
						// try for a head shot then
						pSoldier->bAimShotLocation = AIM_SHOT_HEAD;
					}
				}
			}

		}

		switch( pSoldier->bAimShotLocation )
		{
			case AIM_SHOT_HEAD:
				CalculateSoldierZPos( pTargetSoldier, HEAD_TARGET_POS, &dTargetZ );
				break;
			case AIM_SHOT_TORSO:
				CalculateSoldierZPos( pTargetSoldier, TORSO_TARGET_POS, &dTargetZ );
				break;
			case AIM_SHOT_LEGS:
				CalculateSoldierZPos( pTargetSoldier, LEGS_TARGET_POS, &dTargetZ );
				break;
			default:
				// %)@#&(%?
				CalculateSoldierZPos( pTargetSoldier, TARGET_POS, &dTargetZ );
				break;
		}
	}
	else
	{

		// GET TARGET XY VALUES
		ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

		// fire at centre of tile
		dTargetX = (FLOAT) sXMapPos;
		dTargetY = (FLOAT) sYMapPos;
		if (pSoldier->bTargetCubeLevel)
		{
			// fire at the centre of the cube specified
			dTargetZ = ( (FLOAT) (pSoldier->bTargetCubeLevel + pSoldier->bTargetLevel * PROFILE_Z_SIZE) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
		}
		else
		{
			bStructHeight = GetStructureTargetHeight( sTargetGridNo, (BOOLEAN) (pSoldier->bTargetLevel == 1) );
			if (bStructHeight > 0)
			{
				// fire at the centre of the cube *one below* the tallest of the tallest structure
				if (bStructHeight > 1)
				{
					// reduce target level by 1
					bStructHeight--;
				}
				dTargetZ = ((FLOAT) (bStructHeight + pSoldier->bTargetLevel * PROFILE_Z_SIZE) - 0.5f) * HEIGHT_UNITS_PER_INDEX;
			}
			else
			{
				// fire at 1 unit above the level of the ground
				dTargetZ = (FLOAT) (pSoldier->bTargetLevel * PROFILE_Z_SIZE) * HEIGHT_UNITS_PER_INDEX + 1;
			}
		}
		// adjust for terrain height
		dTargetZ += CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[sTargetGridNo].sHeight );
	}

	*pdXPos = dTargetX;
	*pdYPos = dTargetY;
	*pdZPos = dTargetZ;
}

// HEADROCK HAM 4: The CTH formula has been radically altered, and the value it returns is used very differently.
// This function now no longer rolls any random variables - that is relegated to other functions. However, this
// function is now responsible for altering the "center point" for the attack, given target movement and previous
// bullets fired in a volley.
BOOLEAN UseGunNCTH( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo )
{
	// CTH is now used as a Muzzle Sway value. That is, it determines how wide our shot can go off the "center point"
	// of the attack. Later on, we'll randomize just how far the shot actually goes within that sway radius.
	UINT32							uiMuzzleSway;
	INT16								sXMapPos, sYMapPos;
	INT16								sAPCost;
	INT32								iBPCost;
	FLOAT								dTargetX;
	FLOAT								dTargetY;
	FLOAT								dTargetZ;
	UINT16							usItemNum;
	BOOLEAN							fBuckshot;
	UINT8								ubVolume;
	CHAR8								zBurstString[512];
	UINT8								ubDirection;
	INT32 sNewGridNo;
	UINT8								ubMerc;
	BOOLEAN							fGonnaHit = FALSE;
	FLOAT							dExpGain = 0;
	UINT32							uiDepreciateTest;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("UseGun") );
	
	// Calculate total APs required to complete the entire attack, including all relevant weapon AP costs
	// with possible turning and gun-raising costs as well.
 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bAimTime );
	// SANDRO - get BP cost for weapon manipulating
	if ( gGameExternalOptions.ubEnergyCostForWeaponWeight && !IsAutoResolveActive() ) // rather not in atuoresolve, since we can't choose stance there
		iBPCost = sAPCost * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;
	else 
		iBPCost = 0;

	usItemNum = pSoldier->usAttackingWeapon;

	// Flugente: check for underbarrel weapons and use that object if necessary
	OBJECTTYPE* pObjHand = pSoldier->GetUsedWeapon( &(pSoldier->inv[HANDPOS]) );
	OBJECTTYPE* pObjAttHand = pSoldier->GetUsedWeapon( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
	UINT16 usUBItem = pSoldier->GetUsedWeaponNumber( &pSoldier->inv[ pSoldier->ubAttackingHand ] );

	// CALC MUZZLE SWAY
	if ( Item[ usUBItem ].usItemClass == IC_THROWING_KNIFE )
	{
	    uiMuzzleSway = 100 - CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, pSoldier->bAimShotLocation );
	}
	else
	{
		uiMuzzleSway = 100 - CalcChanceToHitGun( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, pSoldier->bAimShotLocation );
	}

	//ATE: Added if we are in meanwhile, we always hit...
	if ( AreInMeanwhile( ) )
	{
		uiMuzzleSway = 0; // HEADROCK HAM 4: This ensures a direct shot at the target.
	}

	#ifdef JA2BETAVERSION
	if ( gfReportHitChances )
	{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Hit chance was %ld, roll %ld (range %d)", uiHitChance, uiDiceRoll, PythSpacesAway( pSoldier->sGridNo, pSoldier->sTargetGridNo ) );
	}
	#endif

	if ( pSoldier->bDoBurst )
	{
		// Only deduct points once for Burst and Autofire (on firing the first bullet).
		if ( pSoldier->bDoBurst == 1 )
		{
			INT8 bShotsToFire = pSoldier->bDoAutofire ?	pSoldier->bDoAutofire :  GetShotsPerBurst(pObjHand);
			//if (pSoldier->IsValidSecondHandBurst())
			//	bShotsToFire = bShotsToFire*2;
			
			if ( Weapon[ usUBItem ].sBurstSound != NO_WEAPON_SOUND )
			{
				UINT16 noisefactor;
				// IF we are silenced?
				noisefactor = GetPercentNoiseVolume( pObjAttHand );

				if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usUBItem ].ubAttackVolume <= 10 )
				{
					// Pick sound file baed on how many bullets we are going to fire...
					sprintf( zBurstString, gzBurstSndStrings[ Weapon[ usUBItem ].sSilencedBurstSound ], bShotsToFire );

					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile( zBurstString, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else
				{
					// Pick sound file baed on how many bullets we are going to fire...
                    // Lesh: changed next line
					sprintf( zBurstString, gzBurstSndStrings[ Weapon[ usUBItem ].sBurstSound ], bShotsToFire );

					INT8 volume = HIGHVOLUME;
					if ( noisefactor < 100 ) volume = (INT8) ((volume * noisefactor) / 100);
					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile(  zBurstString, RATE_11025, SoundVolume( (INT8) volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}

				/*
				//DIGICRAB: We don't need this anymore, because of the burst sound modification
				//	If we don't have the burst sound, a normal shot will be played for each shot
				if ( pSoldier->iBurstSoundID == NO_SAMPLE )
				{
					// If failed, play normal default....
					pSoldier->iBurstSoundID = PlayJA2Sample( Weapon[ usItemNum ].sBurstSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				*/
			}

			DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
		}

	}
	else
	{
		// ONLY DEDUCT FOR THE FIRST HAND when doing two-pistol attacks
		if ( pSoldier->IsValidSecondHandShot( ) && (*pObjHand)[0]->data.gun.bGunStatus >= USABLE && (*pObjHand)[0]->data.gun.bGunAmmoStatus > 0 )
		{
			// only deduct APs when the main gun fires
			if ( pSoldier->ubAttackingHand == HANDPOS )
			{
				DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
			}
		}
		else
		{
			DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
		}

		//PLAY SOUND
		// ( For throwing knife.. it's earlier in the animation
		if ( Weapon[ usUBItem ].sSound != NO_WEAPON_SOUND && Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )
		{
			// Switch on silencer...
			UINT16 noisefactor = GetPercentNoiseVolume( pObjAttHand );
			if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usUBItem ].ubAttackVolume <= 10 )
			{
				INT32 uiSound;

				uiSound = Weapon [ usUBItem ].silencedSound ;
				//if ( Weapon[ usItemNum ].ubCalibre == AMMO9 || Weapon[ usItemNum ].ubCalibre == AMMO38 || Weapon[ usItemNum ].ubCalibre == AMMO57 )
				//{
				//	uiSound = S_SILENCER_1;
				//}
				//else
				//{
				//	uiSound = S_SILENCER_2;
				//}

				PlayJA2Sample( uiSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

			}
			else
			{
				INT8 volume = HIGHVOLUME;
				if ( noisefactor < 100 ) volume = (volume * noisefactor) / 100;
				PlayJA2Sample( Weapon[ usUBItem ].sSound, RATE_11025, SoundVolume( volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
			}
		}
	}

	// SANDRO - pay BP cost for weapon recoil kick
	if ( gGameExternalOptions.ubEnergyCostForWeaponRecoilKick && !IsAutoResolveActive() ) // rather not in atuoresolve, since we can't choose stance there
	{
		iBPCost = GetBPCostForRecoilkick( pSoldier ); 
		DeductPoints( pSoldier, 0, iBPCost );
	}
	
	if ( (Item[ usUBItem ].usItemClass == IC_GUN) && gGameExternalOptions.bAllowWearSuppressor )
	{
		if ( IsFlashSuppressor(pObjAttHand, pSoldier ) )
		{
			for (attachmentList::iterator iter = (*pObjAttHand)[0]->attachments.begin(); iter != (*pObjAttHand)[0]->attachments.end(); ++iter) 
			{
				if (Item[iter->usItem].hidemuzzleflash  ) 
				{
					OBJECTTYPE* pA=	&(*iter);
					if ( (*pA)[0]->data.objectStatus >=USABLE)
					{
						INT16 ammoReliability = Item[(*pObjAttHand)[0]->data.gun.usGunAmmoItem].bReliability;
						// HEADROCK HAM 5: Variable base chance
						if ( UsingNewCTHSystem() == true)
						{	
							UINT16 usBaseChance = gGameCTHConstants.BASIC_RELIABILITY_ODDS;
							FLOAT dReliabilityRatio = 3.0f * ((FLOAT)usBaseChance / (FLOAT)BASIC_DEPRECIATE_CHANCE); // Compare original odds to new odds.
							uiDepreciateTest = usBaseChance + (INT16)( dReliabilityRatio * (Item[ iter->usItem ].bReliability + ammoReliability) );
							uiDepreciateTest = max(0, uiDepreciateTest);
						}
						else
						{
							uiDepreciateTest = max(0,BASIC_DEPRECIATE_CHANCE + 3 * (Item[ iter->usItem ].bReliability + ammoReliability));
						}
						if ( !PreRandom( uiDepreciateTest ) && ( (*pObjAttHand)[0]->data.objectStatus > 1) )
						{
							(*pA)[0]->data.objectStatus--;
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"rel =%d ",Item[ iter->usItem ].bReliability );

							if ( Random(100) < Item[pA->usItem].usDamageChance )
							{
								(*pA)[0]->data.sRepairThreshold--;
							}
						}
					}
				}
			}
		}
	} //if ( Item[ usItemNum ].usItemClass == IC_GUN )
//ddd}

	//DIGICRAB: Barrel extender wear code
	// Relocated from CalcChanceToHitGun
	if ( Item[ usUBItem ].usItemClass == IC_GUN )
	{
		// lalien: search for barrel extender not for any item with range bonus. (else barrel extender will fall off even when none is attached)
		OBJECTTYPE* pAttachment = FindAttachment( pObjHand, GUN_BARREL_EXTENDER );

		if ( pAttachment )
		{
			// reduce status and see if it falls off
			INT8 reduction = (INT8) Random( 2 );
			(*pAttachment)[0]->data.objectStatus -= reduction;

			// Flugente: reduce repair threshold
			if ( reduction > 0 && Random(100) < Item[pAttachment->usItem].usDamageChance )
			{
				(*pAttachment)[0]->data.sRepairThreshold--;
			}

			if ( (*pAttachment)[0]->data.objectStatus - Random( 35 ) - Random( 35 ) < USABLE )
			{
				//CHRISL: This setup assumes that remove() will work successfully, but if it doesn't we'll duplicate the item.
				// barrel extender falls off!
				// drop it to ground
				//AddItemToPool( pSoldier->sGridNo, pAttachment, 1, pSoldier->pathing.bLevel, 0, -1 );

				// since barrel extenders are not removable we cannot call RemoveAttachment here
				// and must create the item by hand
				//(*pInHand)[0]->attachments.remove(*pAttachment);
				//ADB ubWeight has been removed, see comments in OBJECTTYPE
				//pInHand->ubWeight = CalculateObjectWeight( pInHand );

				//CHRISL: Instead of the above, use this function which is basially redundant to what remove() does, but includes
				//	a failsafe so we don't cause an item duplication.
				for(std::list<OBJECTTYPE>::iterator iter = (*pObjHand)[0]->attachments.begin();
					iter != (*pObjHand)[0]->attachments.end(); ++iter){
						if(*iter == *pAttachment)
						{
							AddItemToPool( pSoldier->sGridNo, pAttachment, 1, pSoldier->pathing.bLevel, 0, -1 );

							iter = (*pObjHand)[0]->RemoveAttachmentAtIter(iter);

							break;
						}
				}

				// big penalty to hit
				// HEADROCK HAM 4: Muzzle sway INCREASES when a barrel extender falls off.
				if(uiMuzzleSway > 70)
				{
					uiMuzzleSway = 100;
				}
				else
				{
					uiMuzzleSway += 30;
				}

				// curse!
				if ( pSoldier->bTeam == OUR_TEAM )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 46 ], pSoldier->GetName() );
				}
			}
		}
	}

	// Some things we don't do for knives...
	if ( Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )
	{
		// If realtime - set counter to freeup from attacking once done
		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{

			// Set delay based on stats, weapon type, etc
			pSoldier->sReloadDelay	= (INT16)( Weapon[ usUBItem ].usReloadDelay + MANDATORY_WEAPON_DELAY );

			// If a bad guy, double the delay!
			if ( (pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY ) )
			{
				pSoldier->sReloadDelay	= ( pSoldier->sReloadDelay * 2 );
			}


			// slow down demo mode!
			if ( gTacticalStatus.uiFlags & DEMOMODE )
			{
				pSoldier->sReloadDelay *= 2;
			}

			//pSoldier->flags.fReloading		= TRUE;
			//RESETTIMECOUNTER( pSoldier->timeCounters.ReloadCounter, pSoldier->sReloadDelay );
		}

		// Deduct AMMO!
		DeductAmmo( pSoldier, pSoldier->ubAttackingHand );

		// ATE: Check if we should say quote...
		if ( (*pObjHand)[0]->data.gun.ubGunShotsLeft == 0 && !Item[usUBItem].rocketlauncher )
		{
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->flags.fSayAmmoQuotePending = TRUE;
			}
		}
	}
	else	//  throwing knife
	{
		fBuckshot = FALSE;
		pSoldier->flags.fMuzzleFlash = FALSE;

		// Deduct knife from inv! (not here, later?)
	}

	if ( Item[usUBItem].rocketlauncher  )
	{
		if ( WillExplosiveWeaponFail( pSoldier, pObjHand ) )
		{
			if ( Item[usUBItem].singleshotrocketlauncher  )
			{
				CreateItem( Item[usItemNum].discardedlauncheritem , (*pObjHand)[0]->data.objectStatus, pObjHand );
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, C1, pSoldier->pathing.bLevel );
			}
			else
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d", usUBItem , (*pObjHand)[0]->data.gun.usGunAmmoItem ) );

				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, (*pObjHand)[0]->data.gun.usGunAmmoItem, pSoldier->pathing.bLevel );
				pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem = NONE;
			}
		  // Reduce again for attack end 'cause it has been incremented for a normal attack
		  //
			// Not anymore.  Only the attack animation was increased, and it will decrease itself.
			  DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK, Now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
			  DebugAttackBusy( String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK\n", gAnimControl[ pSoldier->usAnimState ].zAnimStr ) );
	//		  ReduceAttackBusyCount( pSoldier->ubID, FALSE );

		return( FALSE );
		}
	}

	// GET TARGET XY VALUES
	ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

	// ATE; Moved a whole blotch if logic code for finding target positions to a function
	// so other places can use it
	// If the target contains a soldier, this function determines the x,y,z coordinates of the center of the
	// targeted bodypart.
	GetTargetWorldPositions( pSoldier, sTargetGridNo, &dTargetX, &dTargetY, &dTargetZ );
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: New Shooting Mechanism
	//
	// This single function is now responsible for determining the point where the bullet will go. It adjusts
	// the target x/y/z coordinates autonomously, based on the various effects of the new shooting mechanism.
	//
	// Therefore, once we reach FireBulletGivenTarget, the coordinates passed to that function are the ABSOLUTE
	// final coordinates of the bullet, so FireBullet doesn't actually need to calculate anything beyond that except
	// the angle increments required to put the bullet through those coordinates.
	
	INT16 sApertureRatio = 0;
	if ( !AreInMeanwhile() )
	{
		AdjustTargetCenterPoint( pSoldier, sTargetGridNo, &dTargetX, &dTargetY, &dTargetZ, pObjAttHand, uiMuzzleSway, &sApertureRatio );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: Basic Experience Gain
	//
	// This part gives out basic experience gain for taking a shot or throwing a knife. It gives a few number of
	// "free" points for any attack, and then an additional number of points based on how difficult the shot is,
	// and how well the gun is being aimed.
	// All points given here are FAILURE type, and cannot cause level gain.
	// In addition, once the target is HIT, the shooter receives an additional number of points.

	if ( Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )
	{
		// NB bDoBurst will be 2 at this point for the first shot since it was incremented
		// above
		// HEADROCK: Actually, it's 1 for the first shot. Works fine regardless though.
		// HEADROCK HAM 4: Extra experience gain now given when the target is hit. This part only gives basic points
		// for the attack (FAILURE type).
		if ( PTR_OURTEAM && pSoldier->ubTargetID != NOBODY && (!pSoldier->bDoBurst || pSoldier->bDoBurst == 2 ) && (gTacticalStatus.uiFlags & INCOMBAT ) && ( SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, MercPtrs[ pSoldier->ubTargetID ], pSoldier->bAimShotLocation ) > 0 ) )
		{
			// add base pts for taking a shot, whether it hits or misses
			dExpGain = 2.0f;

			if ( pSoldier->IsValidSecondHandShot( ) && (*pObjHand)[0]->data.gun.bGunStatus >= USABLE && (*pObjHand)[0]->data.gun.bGunAmmoStatus > 0 )
			{
				// reduce exp gain for two pistol shooting since both shots give xp
				dExpGain = (dExpGain * 2) / 3;
			}

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				dExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				dExpGain = 0;
			}

			UINT16 usDextGain = (UINT16)(dExpGain / 2);
			UINT16 usAgilGain = (UINT16)(dExpGain / 2);
			UINT16 usMrksGain = 0;

			// Extra marksmanship/dexterity bonus for aiming.
			if (pSoldier->aiData.bAimTime)
			{
				dExpGain += (3 * sApertureRatio) / 100; // At this point, usExpGain could equal up to 5.
				usDextGain += (UINT16)(dExpGain / 3);
				usMrksGain += (UINT16)((dExpGain * 2) / 3);
			}

			// DEXTERITY/AGILITY/MARKSMANSHIP GAIN: gun attack
			StatChange( pSoldier, AGILAMT, usAgilGain, FROM_FAILURE );
			StatChange( pSoldier, DEXTAMT, usDextGain, FROM_FAILURE );
			StatChange( pSoldier, MARKAMT, usMrksGain, FROM_FAILURE );
		}
	}
	else
	{
		// HEADROCK HAM 4: Extra experience gain now done when the bullet hits the target. This part only
		// gives basic experience for the attack (FAILURE type).
		// Improve for using a throwing knife....
		if (PTR_OURTEAM && pSoldier->ubTargetID != NOBODY)
		{
			// add base pts for taking a shot, whether it hits or misses
			dExpGain = 5.0f;

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				dExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				dExpGain = 0;
			}

			UINT16 usDextGain = (UINT16)(dExpGain / 2);
			UINT16 usAgilGain = (UINT16)(dExpGain / 2);
			UINT16 usMrksGain = 0;

			// Extra marksmanship/dexterity bonus for aiming.
			if (pSoldier->aiData.bAimTime)
			{
				dExpGain += (5 * sApertureRatio) / 100; // At this point, usExpGain could equal up to 10.
				usDextGain += (UINT16)(dExpGain / 3);
				usMrksGain += (UINT16)((dExpGain * 2) / 3);
			}

			// DEXTERITY/AGILITY/MARKSMANSHIP GAIN: gun attack
			StatChange( pSoldier, AGILAMT, usAgilGain, FROM_FAILURE );
			StatChange( pSoldier, DEXTAMT, usDextGain, FROM_FAILURE );
			StatChange( pSoldier, MARKAMT, usMrksGain, FROM_FAILURE );
		}
	}

	/////////////////////////////// Fire bullet at the target coordinates.
	//hayden
	if((is_server && pSoldier->ubID<120) || (!is_server && is_client && pSoldier->ubID<20) || (!is_server && !is_client) )
	{
		FireBulletGivenTarget( pSoldier, dTargetX, dTargetY, dTargetZ, usUBItem, sApertureRatio, fBuckshot, FALSE );
	}
	else
	{
		FireBulletGivenTarget( pSoldier, dTargetX, dTargetY, dTargetZ, usUBItem, sApertureRatio, fBuckshot, TRUE );
	}
	//bottom one is fake (ie not in my control)

	ubVolume = Weapon[ usUBItem ].ubAttackVolume;

	if ( Item[ usItemNum ].usItemClass == IC_THROWING_KNIFE )
	{
		// Here, remove the knife...	or (for now) rocket launcher
		pSoldier->inv[ HANDPOS ].RemoveObjectsFromStack(1);
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
	else if ( Item[usUBItem].rocketlauncher )
	{
		if ( Item[usUBItem].singleshotrocketlauncher )
		{
			CreateItem( Item[usUBItem].discardedlauncheritem, (*pObjHand)[0]->data.objectStatus, pObjHand );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}

		// Direction to center of explosion
		ubDirection = gOppositeDirection[ pSoldier->ubDirection ];
		sNewGridNo  = NewGridNo( pSoldier->sGridNo, (UINT16)(1 * DirectionInc( ubDirection ) ) );

		// Check if a person exists here and is not prone....
		ubMerc = WhoIsThere2( sNewGridNo, pSoldier->pathing.bLevel );

		if ( ubMerc != NOBODY )
		{
			if ( gAnimControl[ MercPtrs[ ubMerc ]->usAnimState ].ubHeight != ANIM_PRONE )
			{
				// Increment attack counter...
//				gTacticalStatus.ubAttackBusyCount++;
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Exaust from LAW", gTacticalStatus.ubAttackBusyCount ) );
				DebugAttackBusy( "Incrementing Attack: Exaust from LAW\n" );

				MercPtrs[ ubMerc ]->EVENT_SoldierGotHit( MINI_GRENADE, 10, 200, pSoldier->ubDirection, 0, pSoldier->ubID, 0, ANIM_CROUCH, 0, sNewGridNo );
			}
		}
	}
	else
	{
		// Snap: get cumulative noise reduction from the weapon and its attachments
		UINT16 noisefactor = GetPercentNoiseVolume( pObjAttHand );
		if ( ubVolume * noisefactor > 25000 )
		{ // Snap: hack this to prevent overflow (damn miserly programmers!)
			ubVolume = 250;
		}
		else
		{
			ubVolume = __max( 1, ( ubVolume * GetPercentNoiseVolume( pObjAttHand ) ) / 100 );
		}
	}

	MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_GUNFIRE );

	if ( pSoldier->bDoBurst )
	{
		// done, if bursting, increment
		pSoldier->bDoBurst++;
	}

	INT16 iOverheatReliabilityMalus = 0;
	// Flugente: Increase Weapon Temperature
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		FLOAT overheatjampercentage = GetGunOverheatDamagePercentage( pObjAttHand );		// ... how much above the gun's usOverheatingDamageThreshold are we? ...

		if ( overheatjampercentage > 1.0 )
			iOverheatReliabilityMalus = (INT16)floor(overheatjampercentage*overheatjampercentage);
	}

	GunIncreaseHeat( pObjAttHand, pSoldier );

	// CJC: since jamming is no longer affected by reliability, increase chance of status going down for really unreliabile guns
 	//INT16 ammoReliability = 0; // Madd: ammo reliability affects gun

	//if ( Item[usItemNum].usItemClass == IC_GUN )
	//{
	//	OBJECTTYPE *pGun = &(pSoldier->inv[pSoldier->ubAttackingHand]);
	//	ammoReliability = Item[(*pGun)[0]->data.gun.usGunAmmoItem].bReliability;
	//}

	// Flugente FTW 1: Added a malus to reliability for overheated guns
	// HEADROCK HAM 5: Variable NCTH base change
	if ( UsingNewCTHSystem() == true)
	{
		UINT16 usBaseChance = gGameCTHConstants.BASIC_RELIABILITY_ODDS;
		FLOAT dReliabilityRatio = 3.0f * ((FLOAT)usBaseChance / (FLOAT)BASIC_DEPRECIATE_CHANCE); // Compare original odds to new odds.
		uiDepreciateTest = usBaseChance + (INT16)( dReliabilityRatio * GetReliability( &(pSoldier->inv[pSoldier->ubAttackingHand]) ) - iOverheatReliabilityMalus);
		uiDepreciateTest = max(0, uiDepreciateTest);
	}
	else
	{
		uiDepreciateTest = max( BASIC_DEPRECIATE_CHANCE + 3 * GetReliability( pObjAttHand ) - iOverheatReliabilityMalus, 0);
	}
	if ( !PreRandom( uiDepreciateTest ) && ( (*pObjAttHand)[0]->data.objectStatus > 1) )
	{
		(*pObjAttHand)[0]->data.objectStatus--;

		// Flugente: reduce repair threshold
		if ( Random(100) < Item[pObjAttHand->usItem].usDamageChance )
		{
			(*pObjAttHand)[0]->data.sRepairThreshold--;
		}
	}

	// reduce monster smell (gunpowder smell)
	if ( pSoldier->aiData.bMonsterSmell > 0 && Random( 2 ) == 0 )
	{
		pSoldier->aiData.bMonsterSmell--;
	}

//<SB> manual recharge
	if (Weapon[Item[usUBItem].ubClassIndex].APsToReloadManually > 0)
		(*pObjAttHand)[0]->data.gun.ubGunState &= ~GS_CARTRIDGE_IN_CHAMBER;
//<SB>
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("UseGun: done"));
	return( TRUE );
}

BOOLEAN UseGun( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo )
{
	if(UsingNewCTHSystem() == true)
		return UseGunNCTH(pSoldier, sTargetGridNo);

	UINT32							uiHitChance, uiDiceRoll;
	INT16								sXMapPos, sYMapPos;
	INT16								sAPCost;
	INT32								iBPCost;
	FLOAT								dTargetX;
	FLOAT								dTargetY;
	FLOAT								dTargetZ;
	UINT16							usItemNum;
	BOOLEAN							fBuckshot;
	UINT8								ubVolume;
	CHAR8								zBurstString[512];
	UINT8								ubDirection;
	INT32 sNewGridNo;
	UINT8								ubMerc;
	BOOLEAN							fGonnaHit = FALSE;
	UINT16							usExpGain = 0;
	UINT32							uiDepreciateTest;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("UseGun") );
	// Deduct points!
 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bAimTime );
	// SANDRO: get BP cost for weapon manipulating
	if ( gGameExternalOptions.ubEnergyCostForWeaponWeight && !IsAutoResolveActive() ) // rather not in atuoresolve, since we can't choose stance there
		iBPCost = sAPCost * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;
	else 
		iBPCost = 0;

	// Flugente: check for underbarrel weapons and use that object if necessary
	OBJECTTYPE* pObjUsed = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );
	UINT16 usUBItem = pSoldier->GetUsedWeaponNumber( &pSoldier->inv[pSoldier->ubAttackingHand] );

	usItemNum = pSoldier->usAttackingWeapon;

	// DEDUCT APs
	if ( pSoldier->bDoBurst )
	{
		// ONly deduct points once
		if ( pSoldier->bDoBurst == 1 )
		{
			INT8 bShotsToFire = pSoldier->bDoAutofire ?	pSoldier->bDoAutofire : GetShotsPerBurst(pObjUsed);
			//if (pSoldier->IsValidSecondHandBurst())
			//	bShotsToFire = bShotsToFire*2;
						
			if ( Weapon[ usUBItem ].sBurstSound != NO_WEAPON_SOUND )
			{
				// IF we are silenced?
				UINT16 noisefactor = GetPercentNoiseVolume( pObjUsed );

				if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usUBItem ].ubAttackVolume <= 10 )
				{
					// Pick sound file baed on how many bullets we are going to fire...
					sprintf( zBurstString, gzBurstSndStrings[ Weapon[ usUBItem ].sSilencedBurstSound ], bShotsToFire );

					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile( zBurstString, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				else
				{
					// Pick sound file baed on how many bullets we are going to fire...
                    // Lesh: changed next line
					sprintf( zBurstString, gzBurstSndStrings[ Weapon[ usUBItem ].sBurstSound ], bShotsToFire );

					INT8 volume = HIGHVOLUME;
					if ( noisefactor < 100 ) volume = (INT8) ((volume * noisefactor) / 100);
					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile(  zBurstString, RATE_11025, SoundVolume( (INT8) volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}

				/*
				//DIGICRAB: We don't need this anymore, because of the burst sound modification
				//	If we don't have the burst sound, a normal shot will be played for each shot
				if ( pSoldier->iBurstSoundID == NO_SAMPLE )
				{
					// If failed, play normal default....
					pSoldier->iBurstSoundID = PlayJA2Sample( Weapon[ usItemNum ].sBurstSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				}
				*/
			}

			DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
		}

	}
	else
	{
		// ONLY DEDUCT FOR THE FIRST HAND when doing two-pistol attacks
		if ( pSoldier->IsValidSecondHandShot( ) && (*pObjUsed)[0]->data.gun.bGunStatus >= USABLE && (*pObjUsed)[0]->data.gun.bGunAmmoStatus > 0 )
		{
			// only deduct APs when the main gun fires
			if ( pSoldier->ubAttackingHand == HANDPOS )
			{
				DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
			}
		}
		else
		{
			DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
		}

		//PLAY SOUND
		// ( For throwing knife.. it's earlier in the animation
		if ( Weapon[ usUBItem ].sSound != NO_WEAPON_SOUND && Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )
		{
			// Switch on silencer...
			UINT16 noisefactor = GetPercentNoiseVolume( pObjUsed );
			if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usUBItem ].ubAttackVolume <= 10 )
			{
				INT32 uiSound;

				uiSound = Weapon [ usUBItem ].silencedSound ;
				//if ( Weapon[ usItemNum ].ubCalibre == AMMO9 || Weapon[ usItemNum ].ubCalibre == AMMO38 || Weapon[ usItemNum ].ubCalibre == AMMO57 )
				//{
				//	uiSound = S_SILENCER_1;
				//}
				//else
				//{
				//	uiSound = S_SILENCER_2;
				//}

				PlayJA2Sample( uiSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

			}
			else
			{
				INT8 volume = HIGHVOLUME;
				if ( noisefactor < 100 ) volume = (volume * noisefactor) / 100;
				PlayJA2Sample( Weapon[ usUBItem ].sSound, RATE_11025, SoundVolume( volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
			}
		}
	}

	// SANDRO: pay BP cost for weapon recoil kick
	if ( gGameExternalOptions.ubEnergyCostForWeaponRecoilKick && !IsAutoResolveActive() ) // rather not in atuoresolve, since we can't choose stance there
	{
		iBPCost = GetBPCostForRecoilkick( pSoldier ); 
		DeductPoints( pSoldier, 0, iBPCost );
	}		

	// CALC CHANCE TO HIT
	if ( Item[ usUBItem ].usItemClass == IC_THROWING_KNIFE )
	{
	  uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, pSoldier->bAimShotLocation );
	}
	else
	{
		fCalculateCTHDuringGunfire = TRUE;
		uiHitChance = CalcChanceToHitGun( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, pSoldier->bAimShotLocation );
		//CHRISL:
		//pSoldier->CTH.push_back(uiHitChance);
		//if(pSoldier->bDoAutofire == pSoldier->bDoBurst && pSoldier->CTH.size() > 0)
		//	pSoldier->CTH.clear();
	}
	fCalculateCTHDuringGunfire = FALSE;

//ddd{износ объектов с муззле флаш. silencer
	if ( (Item[ usUBItem ].usItemClass == IC_GUN) && gGameExternalOptions.bAllowWearSuppressor )
	{
		if ( IsFlashSuppressor(pObjUsed, pSoldier ) )
		{
			for (attachmentList::iterator iter = (*pObjUsed)[0]->attachments.begin(); iter != (*pObjUsed)[0]->attachments.end(); ++iter) 
			{
				if (Item[iter->usItem].hidemuzzleflash  ) 
				{
					OBJECTTYPE* pA=	&(*iter);
					if ( (*pA)[0]->data.objectStatus >=USABLE)
					{
						INT16 ammoReliability = Item[(*pObjUsed)[0]->data.gun.usGunAmmoItem].bReliability;
						// HEADROCK HAM 5: Variable base chance
						if ( UsingNewCTHSystem() == true )
						{
							UINT16 usBaseChance = gGameCTHConstants.BASIC_RELIABILITY_ODDS;
							FLOAT dReliabilityRatio = 3.0f * ((FLOAT)usBaseChance / (FLOAT)BASIC_DEPRECIATE_CHANCE); // Compare original odds to new odds.
							uiDepreciateTest = usBaseChance + (INT16)( dReliabilityRatio * (Item[ iter->usItem ].bReliability + ammoReliability) );
							uiDepreciateTest = __max(0, uiDepreciateTest);
						}
						else
						{
							uiDepreciateTest = __max(0,BASIC_DEPRECIATE_CHANCE + 3 * (Item[ iter->usItem ].bReliability + ammoReliability));
						}
						if ( !PreRandom( uiDepreciateTest ) && ( (*pObjUsed)[0]->data.objectStatus > 1) )
						{
							(*pA)[0]->data.objectStatus--;
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"rel =%d ",Item[ iter->usItem ].bReliability );

							// Flugente: reduce repair threshold
							if ( Random(100) < Item[pA->usItem].usDamageChance )
							{
								(*pA)[0]->data.sRepairThreshold--;
							}
						}
					}
				}
			}
		}
	} //if ( Item[ usItemNum ].usItemClass == IC_GUN )
//ddd}

	//DIGICRAB: Barrel extender wear code
	// Relocated from CalcChanceToHitGun
	if ( Item[ usUBItem ].usItemClass == IC_GUN )
	{
		// lalien: search for barrel extender not for any item with range bonus. (else barrel extender will fall off even when none is attached)
		OBJECTTYPE* pAttachment = FindAttachment( pObjUsed, GUN_BARREL_EXTENDER );

		if ( pAttachment->exists() )
		{
			// reduce status and see if it falls off
			INT8 reduction = (INT8) Random( 2 );
			(*pAttachment)[0]->data.objectStatus -= reduction;

			// Flugente: reduce repair threshold
			if ( reduction > 0 && Random(100) < Item[pAttachment->usItem].usDamageChance )
			{
				(*pAttachment)[0]->data.sRepairThreshold--;
			}

			if ( (*pAttachment)[0]->data.objectStatus - Random( 35 ) - Random( 35 ) < USABLE )
			{
				//CHRISL: This setup assumes that remove() will work successfully, but if it doesn't we'll duplicate the item.
				// barrel extender falls off!
				// drop it to ground
				//AddItemToPool( pSoldier->sGridNo, pAttachment, 1, pSoldier->pathing.bLevel, 0, -1 );

				// since barrel extenders are not removable we cannot call RemoveAttachment here
				// and must create the item by hand
				//(*pInHand)[0]->attachments.remove(*pAttachment);
				//ADB ubWeight has been removed, see comments in OBJECTTYPE
				//pInHand->ubWeight = CalculateObjectWeight( pInHand );

				//CHRISL: Instead of the above, use this function which is basially redundant to what remove() does, but includes
				//	a failsafe so we don't cause an item duplication.
				for(std::list<OBJECTTYPE>::iterator iter = (*pObjUsed)[0]->attachments.begin();
					iter != (*pObjUsed)[0]->attachments.end(); ++iter){
						if(*iter == *pAttachment)
						{
							AddItemToPool( pSoldier->sGridNo, pAttachment, 1, pSoldier->pathing.bLevel, 0, -1 );

							iter = (*pObjUsed)[0]->RemoveAttachmentAtIter(iter);

							break;
						}
				}

				// big penalty to hit
				// WDS 07/06/2008 fix randoms
				//if(uiHitChance < 30)
				// HEADROCK: Altered formula to accept variable MINIMUM CTH.
				//if(uiHitChance <= 30)
				//	uiHitChance = MINCHANCETOHIT;
				//else
				//	uiHitChance -= 30;
				if(uiHitChance <= (UINT16)(__max(30, gGameExternalOptions.ubMinimumCTH + 30)))
				{
					uiHitChance = gGameExternalOptions.ubMinimumCTH ;
				}
				else
				{
					uiHitChance -= 30;
				}

				// curse!
				if ( pSoldier->bTeam == OUR_TEAM )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 46 ], pSoldier->GetName() );
				}
			}
		}
	}


	//ATE: Added if we are in meanwhile, we always hit...
	if ( AreInMeanwhile( ) )
	{
		uiHitChance = 100;
	}

	// ROLL DICE
	uiDiceRoll = PreRandom( 100 );

	#ifdef JA2BETAVERSION
	if ( gfReportHitChances )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Hit chance was %ld, roll %ld (range %d)", uiHitChance, uiDiceRoll, PythSpacesAway( pSoldier->sGridNo, pSoldier->sTargetGridNo ) );
	}
	#endif

	// WDS 07/06/2008 fix randoms
	//fGonnaHit = uiDiceRoll <= uiHitChance;
	fGonnaHit = uiDiceRoll < uiHitChance;
	if(uiDiceRoll == uiHitChance)//dnl ch60 011109 Fix uiDiceRoll==uiHitChance which always result in hit regard fGonnaHit is FALSE
		++uiDiceRoll;

	// GET TARGET XY VALUES
    ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

	// ATE; Moved a whole blotch if logic code for finding target positions to a function
	// so other places can use it
	GetTargetWorldPositions( pSoldier, sTargetGridNo, &dTargetX, &dTargetY, &dTargetZ );

	// Some things we don't do for knives...
	if ( Item[ usUBItem ].usItemClass != IC_THROWING_KNIFE )
	{
		// If realtime - set counter to freeup from attacking once done
		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{

			// Set delay based on stats, weapon type, etc
			pSoldier->sReloadDelay	= (INT16)( Weapon[ usUBItem ].usReloadDelay + MANDATORY_WEAPON_DELAY );

			// If a bad guy, double the delay!
			if ( (pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY ) )
			{
				pSoldier->sReloadDelay	= ( pSoldier->sReloadDelay * 2 );
			}


			// slow down demo mode!
			if ( gTacticalStatus.uiFlags & DEMOMODE )
			{
				pSoldier->sReloadDelay *= 2;
			}

			//pSoldier->flags.fReloading		= TRUE;
			//RESETTIMECOUNTER( pSoldier->timeCounters.ReloadCounter, pSoldier->sReloadDelay );
		}

		// Deduct AMMO!
		DeductAmmo( pSoldier, pSoldier->ubAttackingHand );

		// ATE: Check if we should say quote...		
		if ( (*pObjUsed)[0]->data.gun.ubGunShotsLeft == 0 && !Item[usUBItem].rocketlauncher )
		{
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->flags.fSayAmmoQuotePending = TRUE;
			}
		}
		// NB bDoBurst will be 2 at this point for the first shot since it was incremented
		// above
		if ( PTR_OURTEAM && pSoldier->ubTargetID != NOBODY && (!pSoldier->bDoBurst || pSoldier->bDoBurst == 2 ) && (gTacticalStatus.uiFlags & INCOMBAT ) && ( SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, MercPtrs[ pSoldier->ubTargetID ], pSoldier->bAimShotLocation ) > 0 ) )
		{
			if ( fGonnaHit )
			{
				// grant extra exp for hitting a difficult target
				usExpGain += (UINT8) (100 - uiHitChance) / 25;

				if ( pSoldier->aiData.bAimTime && !pSoldier->bDoBurst )
				{
					// gain extra exp for aiming, up to the amount from
					// the difficulty of the shot
					usExpGain += __min( pSoldier->aiData.bAimTime, usExpGain );
				}

				// base pts extra for hitting
				usExpGain	+= 3;
			}

			// add base pts for taking a shot, whether it hits or misses
			usExpGain += 3;

			if ( pSoldier->IsValidSecondHandShot( ) && pSoldier->inv[ HANDPOS ][0]->data.gun.bGunStatus >= USABLE && (*pObjUsed)[0]->data.gun.bGunAmmoStatus > 0 )
			{
				// reduce exp gain for two pistol shooting since both shots give xp
				usExpGain = (usExpGain * 2) / 3;
			}

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				usExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				usExpGain = 0;
			}

			// MARKSMANSHIP GAIN: gun attack
			StatChange( pSoldier, MARKAMT, usExpGain, ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
		}

		// set buckshot and muzzle flash
		fBuckshot = FALSE;
		if (!CREATURE_OR_BLOODCAT( pSoldier ) )
		{
			if ( IsFlashSuppressor( pObjUsed, pSoldier ) )
				pSoldier->flags.fMuzzleFlash = FALSE;
			else
				pSoldier->flags.fMuzzleFlash = TRUE;

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("UseGun: Muzzle flash = %d",pSoldier->flags.fMuzzleFlash));
						
			if ( AmmoTypes[(*pObjUsed)[0]->data.gun.ubGunAmmoType].numberOfBullets > 1 )
				fBuckshot = TRUE;

			//switch ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunAmmoType )
			//{
			//	case AMMO_BUCKSHOT:
			//		fBuckshot = TRUE;
			//		break;
			//	case AMMO_SLEEP_DART:
			//		pSoldier->flags.fMuzzleFlash = FALSE;
			//		break;
			//	default:
			//		break;
			//}
		}
	}
	else	//  throwing knife
	{
		fBuckshot = FALSE;
		pSoldier->flags.fMuzzleFlash = FALSE;

		// Deduct knife from inv! (not here, later?)

		// Improve for using a throwing knife....
		if (PTR_OURTEAM && pSoldier->ubTargetID != NOBODY)
		{
			if ( fGonnaHit )
			{
				// grant extra exp for hitting a difficult target
				usExpGain += (UINT8) (100 - uiHitChance) / 10;

				if (pSoldier->aiData.bAimTime)
				{
					// gain extra exp for aiming, up to the amount from
					// the difficulty of the throw
					usExpGain += ( 2 * __min( pSoldier->aiData.bAimTime, usExpGain ) );
				}

				// base pts extra for hitting
				usExpGain	+= 10;
			}

			// add base pts for taking a shot, whether it hits or misses
			usExpGain += 10;

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				usExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				usExpGain = 0;
			}

			// MARKSMANSHIP/DEXTERITY GAIN: throwing knife attack
			StatChange( pSoldier, MARKAMT, ( UINT16 )( usExpGain / 2 ), ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
			StatChange( pSoldier, DEXTAMT, ( UINT16 )( usExpGain / 2 ), ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
		}
	}

	if ( Item[usUBItem].rocketlauncher  )
	{
		if ( WillExplosiveWeaponFail( pSoldier, pObjUsed ) )
		{
			if ( Item[usUBItem].singleshotrocketlauncher  )
			{
				CreateItem( Item[usUBItem].discardedlauncheritem , (*pObjUsed)[0]->data.objectStatus, pObjUsed );
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, C1, pSoldier->pathing.bLevel );
			}
			else
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d", usUBItem , (*pObjUsed)[0]->data.gun.usGunAmmoItem ) );

				IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, (*pObjUsed)[0]->data.gun.usGunAmmoItem, pSoldier->pathing.bLevel );
			
				OBJECTTYPE * pLaunchable = FindLaunchableAttachment( pObjUsed, usUBItem );
				if(pLaunchable)
				{
					(*pObjUsed)[0]->data.gun.usGunAmmoItem = pLaunchable->usItem;
				} 
				else 
				{
					(*pObjUsed)[0]->data.gun.usGunAmmoItem = NONE;
				}
			}
		    // Reduce again for attack end 'cause it has been incremented for a normal attack
		    //
			// Not anymore.  Only the attack animation was increased, and it will decrease itself.
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK, Now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
			DebugAttackBusy( String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK\n", gAnimControl[ pSoldier->usAnimState ].zAnimStr ) );
			//ReduceAttackBusyCount( pSoldier->ubID, FALSE );

		  return( FALSE );
		}
	}
	//hayden
	if((is_server && pSoldier->ubID<120) || (!is_server && is_client && pSoldier->ubID<20) || (!is_server && !is_client) )
	{
		FireBulletGivenTarget( pSoldier, dTargetX, dTargetY, dTargetZ, usUBItem, (UINT16) (uiHitChance - uiDiceRoll), fBuckshot, FALSE );
	}
	else
	{
		FireBulletGivenTarget( pSoldier, dTargetX, dTargetY, dTargetZ, usUBItem, (UINT16) (uiHitChance - uiDiceRoll), fBuckshot, TRUE );
	}
	//bottom one is fake (ie not in my control)

	ubVolume = Weapon[ usUBItem ].ubAttackVolume;

	if ( Item[ usUBItem ].usItemClass == IC_THROWING_KNIFE )
	{
		// Here, remove the knife...	or (for now) rocket launcher
		pSoldier->inv[ HANDPOS ].RemoveObjectsFromStack(1);
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
	else if ( Item[usUBItem].rocketlauncher )
	{
		if ( Item[usUBItem].singleshotrocketlauncher )
		{
			CreateItem( Item[usUBItem].discardedlauncheritem, (*pObjUsed)[0]->data.objectStatus, pObjUsed );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}

		// Direction to center of explosion
		ubDirection = gOppositeDirection[ pSoldier->ubDirection ];
		sNewGridNo  = NewGridNo( pSoldier->sGridNo, (UINT16)(1 * DirectionInc( ubDirection ) ) );

		// Check if a person exists here and is not prone....
		ubMerc = WhoIsThere2( sNewGridNo, pSoldier->pathing.bLevel );

		if ( ubMerc != NOBODY )
		{
			if ( gAnimControl[ MercPtrs[ ubMerc ]->usAnimState ].ubHeight != ANIM_PRONE )
			{
				// Increment attack counter...
//				gTacticalStatus.ubAttackBusyCount++;
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Exaust from LAW", gTacticalStatus.ubAttackBusyCount ) );
				DebugAttackBusy( "Incrementing Attack: Exaust from LAW\n" );

				MercPtrs[ ubMerc ]->EVENT_SoldierGotHit( MINI_GRENADE, 10, 200, pSoldier->ubDirection, 0, pSoldier->ubID, 0, ANIM_CROUCH, 0, sNewGridNo );
			}
		}
	}
	else
	{
		// Snap: get cumulative noise reduction from the weapon and its attachments
		UINT16 noisefactor = GetPercentNoiseVolume( pObjUsed );
		if ( ubVolume * noisefactor > 25000 )
		{ // Snap: hack this to prevent overflow (damn miserly programmers!)
			ubVolume = 250;
		}
		else
		{
			ubVolume = __max( 1, ( ubVolume * noisefactor ) / 100 );
		}				
	}

	MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_GUNFIRE );

	if ( pSoldier->bDoBurst )
	{
		// done, if bursting, increment
		pSoldier->bDoBurst++;
	}

	INT16 iOverheatReliabilityMalus = 0;
	// Flugente: Increase Weapon Temperature
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		FLOAT overheatjampercentage = GetGunOverheatDamagePercentage( pObjUsed );		// ... how much above the gun's usOverheatingDamageThreshold are we? ...

		if ( overheatjampercentage > 1.0 )
			iOverheatReliabilityMalus = (INT16)floor(overheatjampercentage*overheatjampercentage);
	}

	GunIncreaseHeat( pObjUsed, pSoldier );

	/* //WarmSteel - Replaced with GetReliability( pObj )
	// CJC: since jamming is no longer affected by reliability, increase chance of status going down for really unreliabile guns
 	INT16 ammoReliability = 0; // Madd: ammo reliability affects gun


	if ( Item[usItemNum].usItemClass == IC_GUN )
	{
		OBJECTTYPE *pGun = &(pSoldier->inv[pSoldier->ubAttackingHand]);
		ammoReliability = Item[(*pGun)[0]->data.gun.usGunAmmoItem].bReliability;
	}
	*/

	// Flugente FTW 1: Added a malus to reliability for overheated guns
	if ( UsingNewCTHSystem() == true )
	{
		UINT16 usBaseChance = gGameCTHConstants.BASIC_RELIABILITY_ODDS;
		FLOAT dReliabilityRatio = 3.0f * ((FLOAT)usBaseChance / (FLOAT)BASIC_DEPRECIATE_CHANCE); // Compare original odds to new odds.
		uiDepreciateTest = usBaseChance + (INT16)( dReliabilityRatio * GetReliability( &(pSoldier->inv[ pSoldier->ubAttackingHand ])) - iOverheatReliabilityMalus);
		uiDepreciateTest = max(0, uiDepreciateTest);
	}
	else
	{
		uiDepreciateTest = max( BASIC_DEPRECIATE_CHANCE + 3 * ( GetReliability( pObjUsed ) ) - iOverheatReliabilityMalus, 0);
	}

	if ( !PreRandom( uiDepreciateTest ) && ( (*pObjUsed)[0]->data.objectStatus > 1) )
	{
		(*pObjUsed)[0]->data.objectStatus--;

		// Flugente: reduce repair threshold
		if ( Random(100) < Item[pObjUsed->usItem].usDamageChance )
		{
			(*pObjUsed)[0]->data.sRepairThreshold--;
		}
	}

	// reduce monster smell (gunpowder smell)
	if ( pSoldier->aiData.bMonsterSmell > 0 && Random( 2 ) == 0 )
	{
		pSoldier->aiData.bMonsterSmell--;
	}

//<SB> manual recharge
	if (Weapon[Item[usUBItem].ubClassIndex].APsToReloadManually > 0)
	{
		(*pObjUsed)[0]->data.gun.ubGunState &= ~GS_CARTRIDGE_IN_CHAMBER;
	}
//<SB>
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("UseGun: done"));
	return( TRUE );
}

BOOLEAN UseBlade( SOLDIERTYPE *pSoldier , INT32 sTargetGridNo )
{
	SOLDIERTYPE *				pTargetSoldier;
	INT32								iHitChance, iDiceRoll;
	INT16								sXMapPos, sYMapPos;
	INT16								sAPCost;
	EV_S_WEAPONHIT			SWeaponHit;
	INT32								iImpact, iImpactForCrits;
	BOOLEAN							fGonnaHit = FALSE;
	UINT16							usExpGain = 0;
	INT8								bMaxDrop;
	BOOLEAN							fSurpriseAttack;

	// Deduct points!
 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bAimTime );

	DeductPoints( pSoldier, sAPCost, 0, AFTERACTION_INTERRUPT );

	// GET TARGET XY VALUES
	ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

	// See if a guy is here!
	pTargetSoldier = SimpleFindSoldier( sTargetGridNo, pSoldier->bTargetLevel );
	if ( pTargetSoldier )
	{
		// set target as noticed attack
		pSoldier->flags.uiStatusFlags |= SOLDIER_ATTACK_NOTICED;
		pTargetSoldier->flags.fIntendedTarget = TRUE;

		// SAVE OPP ID
		pSoldier->ubOppNum = pTargetSoldier->ubID;

		// CHECK IF BUDDY KNOWS ABOUT US
		if ( pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN || pTargetSoldier->stats.bLife < OKLIFE || pTargetSoldier->bCollapsed )
		{
			iHitChance = 100;
			fSurpriseAttack = TRUE;
		}
		else
		{
			iHitChance = CalcChanceToStab( pSoldier, pTargetSoldier, pSoldier->aiData.bAimTime );
			fSurpriseAttack = FALSE;
		}

		// ROLL DICE
		iDiceRoll = (INT32) PreRandom( 100 );
		//sprintf( gDebugStr, "Hit Chance: %d %d", (int)uiHitChance, uiDiceRoll );

		// SANDRO - new mercs' records 
		if ( pSoldier->bTeam == gbPlayerNum && pSoldier->ubProfile != NO_PROFILE )
		{
			gMercProfiles[ pSoldier->ubProfile ].records.usBladeAttacks++;
			if ( iDiceRoll < iHitChance )
			{
				gMercProfiles[ pSoldier->ubProfile ].records.usShotsHit++;
			}
		}

		// WDS 07/19/2008 - Random number use fix
		if ( iDiceRoll < iHitChance )
		{
			fGonnaHit = TRUE;

			// CALCULATE DAMAGE!
			// attack HITS, calculate damage (base damage is 1-maximum knife sImpact)
			iImpact = HTHImpact( pSoldier, pTargetSoldier, (iHitChance - iDiceRoll), TRUE );

			// Flugente: check for underbarrel weapons and use that object if necessary (think of bayonets)
			OBJECTTYPE* pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );

			// modify this by the knife's condition (if it's dull, not much good)
			iImpact = ( iImpact * WEAPON_STATUS_MOD( (*pObj)[0]->data.objectStatus) ) / 100;
			
			// modify by hit location
			AdjustImpactByHitLocation( iImpact, pSoldier->bAimShotLocation, &iImpact, &iImpactForCrits );

			// bonus for surprise
			if ( fSurpriseAttack )
			{
				iImpact = (iImpact * 3) / 2;
			}

			// any successful hit does at LEAST 1 pt minimum damage
			if (iImpact < 1)
			{
				iImpact = 1;
			}
			
			if ( (*pObj)[0]->data.objectStatus > USABLE )
			{
				bMaxDrop = (iImpact / 20);

				// the duller they get, the slower they get any worse...
				bMaxDrop = __min( bMaxDrop, (*pObj)[0]->data.objectStatus / 10 );

				// as long as its still > USABLE, it drops another point 1/2 the time
				bMaxDrop = __max( bMaxDrop, 2 );

				(*pObj)[0]->data.objectStatus -= (INT8) Random( bMaxDrop );     // 0 to (maxDrop - 1)

				// Flugente: reduce repair threshold
				if ( bMaxDrop > 0 && Random(100) < Item[pObj->usItem].usDamageChance )
				{
					(*pObj)[0]->data.sRepairThreshold = max((*pObj)[0]->data.objectStatus, (*pObj)[0]->data.sRepairThreshold - 1);
				}
			}

			// SANDRO - new merc records - times wounded (stabbed)
			if ( pTargetSoldier->ubProfile != NO_PROFILE )
				gMercProfiles[ pTargetSoldier->ubProfile ].records.usTimesWoundedStabbed++;

			// Send event for getting hit
			memset( &(SWeaponHit), 0, sizeof( SWeaponHit ) );
			SWeaponHit.usSoldierID			= pTargetSoldier->ubID;
			SWeaponHit.uiUniqueId       = pTargetSoldier->uiUniqueSoldierIdValue;
			SWeaponHit.usWeaponIndex		= pSoldier->usAttackingWeapon;
			SWeaponHit.sDamage					= (INT16) iImpact;
			SWeaponHit.usDirection			= (UINT8)GetDirectionFromGridNo( pSoldier->sGridNo, pTargetSoldier );
			SWeaponHit.sXPos						= (INT16)pTargetSoldier->dXPos;
			SWeaponHit.sYPos						= (INT16)pTargetSoldier->dYPos;
			SWeaponHit.sZPos						= 20;
			SWeaponHit.sRange						= 1;
			SWeaponHit.ubAttackerID			= pSoldier->ubID;
			SWeaponHit.fHit							= TRUE;
			SWeaponHit.ubSpecial				= FIRE_WEAPON_NO_SPECIAL;
			AddGameEvent( S_WEAPONHIT, (UINT16) 20, &SWeaponHit );
		}
		else
		{
			// if it was another team shooting at someone under our control
			if ( (pSoldier->bTeam != Menptr[ pTargetSoldier->ubID ].bTeam ) )
			{
				if (pTargetSoldier->bTeam == gbPlayerNum)
				{
				 // AGILITY GAIN (10):  Target avoids a knife attack
				 // Snap: stat gains should match stat requirements
				 StatChange( MercPtrs[ pTargetSoldier->ubID ], DEXTAMT, 15, FALSE);
				 StatChange( MercPtrs[ pTargetSoldier->ubID ], AGILAMT, 5, FALSE);
				 StatChange( MercPtrs[ pTargetSoldier->ubID ], STRAMT, 5, FALSE);
				}
			}
			// 0verhaul:  Another case that is handled by the animation transition system.
			// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in knife attack") );
			// FreeUpAttacker( (UINT8) pSoldier->ubID );
		}

		if ( PTR_OURTEAM && pSoldier->ubTargetID != NOBODY)
		{
			if ( fGonnaHit )
			{
				// grant extra exp for hitting a difficult target
				usExpGain += (UINT8) (100 - iHitChance) / 10;

				if (pSoldier->aiData.bAimTime)
				{
					// gain extra exp for aiming, up to the amount from
					// the difficulty of the attack
					usExpGain += ( 2 * __min( pSoldier->aiData.bAimTime, usExpGain ) );
				}

				// base pts extra for hitting
				usExpGain	+= 10;
			}

			// add base pts for taking a shot, whether it hits or misses
			usExpGain += 10;

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				usExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move!
				usExpGain = 0;
			}

			// DEXTERITY GAIN:  Made a knife attack, successful or not
			StatChange( pSoldier, DEXTAMT, usExpGain, ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
			StatChange( pSoldier, AGILAMT, usExpGain/3, ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
			StatChange( pSoldier, STRAMT, usExpGain/3, ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );
		}
	}
	// 0verhaul:  This case is also handled by the animation translations
	// else
	// {
	//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in knife attack") );
	//	FreeUpAttacker( (UINT8) pSoldier->ubID );
	// }

	// possibly reduce monster smell
	if ( pSoldier->aiData.bMonsterSmell > 0 && Random( 5 ) == 0 )
	{
		pSoldier->aiData.bMonsterSmell--;
	}


	return( TRUE );
}


BOOLEAN UseHandToHand( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo, BOOLEAN fStealing )
{
	SOLDIERTYPE				*	pTargetSoldier;
	INT32								iHitChance, iDiceRoll;
	INT16								sXMapPos, sYMapPos;
	INT16								sAPCost;
	EV_S_WEAPONHIT			SWeaponHit;
	INT32								iImpact;
	UINT16							usOldItem, usNewItem;
	UINT8								ubExpGain;
	UINT8					ubIndexRet;
	BOOLEAN					fFailure;		// no stealing occured
	BOOLEAN					fNoMoreItems = FALSE;	// The enemy has no more items to steal!
	BOOLEAN					fNoMoreItemInHand = FALSE;
	BOOLEAN					fSoldierCollapsed = FALSE;
	BOOLEAN					fStealAttempt = FALSE; // added - SANDRO
	// Deduct points!
	// August 13 2002: unless stealing - APs already deducted elsewhere

	// Punch the enemy
	if (!fStealing)
	{
 		sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bAimTime );
		DeductPoints( pSoldier, sAPCost, 0, AFTERACTION_INTERRUPT );
	}
	// Steal from the enemy
	else
	{
		// APs were already reduced!
		//SANDRO - actually we will redure them here, but lower and based on number of items stolen.
	}

	// See if a guy is here!
	pTargetSoldier = SimpleFindSoldier( sTargetGridNo, pSoldier->bTargetLevel );
	if ( pTargetSoldier )
	{
		// set target as noticed attack
		pSoldier->flags.uiStatusFlags |= SOLDIER_ATTACK_NOTICED;
		pTargetSoldier->flags.fIntendedTarget = TRUE;

		// SAVE OPP ID
		pSoldier->ubOppNum = pTargetSoldier->ubID;

		if (fStealing)
		{
			// Calculate the possible chance to steal!
			// Flugente: if we are on the same team, allow guaranteed full access
			if ( AllowedToStealFromTeamMate(pSoldier->ubID, pTargetSoldier->ubID) )
			{
				iHitChance = 100;
			}
			else if ( AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) || CREATURE_OR_BLOODCAT( pTargetSoldier ) || TANK( pTargetSoldier ) || (SOLDIER_CLASS_MILITIA(pTargetSoldier->ubSoldierClass) && (gGameExternalOptions.ubMilitiaDropEquipment != 2)) ) // added militia here - SANDRO
			{
				iHitChance = 0;
			}
			else if ( pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN )
			{
				// give bonus for surprise, but not so much as struggle would still occur
				iHitChance = CalcChanceToSteal( pSoldier, pTargetSoldier, pSoldier->aiData.bAimTime ) + 20;
			}
			else if ( pTargetSoldier->stats.bLife < OKLIFE || pTargetSoldier->bCollapsed )
			{
				iHitChance = 100;
				fSoldierCollapsed = TRUE;
			}
			else
			{
				iHitChance = CalcChanceToSteal( pSoldier, pTargetSoldier, pSoldier->aiData.bAimTime );
			}
		}
		else
		{
			if ( pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN || pTargetSoldier->stats.bLife < OKLIFE || pTargetSoldier->bCollapsed )
			{
				iHitChance = 100;
			}
			else
			{
				iHitChance = CalcChanceToPunch( pSoldier, pTargetSoldier, pSoldier->aiData.bAimTime );
			}
		}

		// ROLL DICE
		iDiceRoll = (INT32) PreRandom( 100 );
		//sprintf( gDebugStr, "Hit Chance: %d %d", (int)uiHitChance, uiDiceRoll );

		#ifdef JA2BETAVERSION
		if ( gfReportHitChances )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Hit chance was %ld, roll %ld", iHitChance, iDiceRoll );
		}
		#endif

		// GET TARGET XY VALUES
		ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

		// -----------------------------------
		// Steal from the Enemy
		// -----------------------------------
		if (fStealing )
		{
			// SANDRO - unable to steal from militia if they are not allowed to drop equipment
			if (SOLDIER_CLASS_MILITIA(pTargetSoldier->ubSoldierClass) && (gGameExternalOptions.ubMilitiaDropEquipment != 2) )
			{
				DeductPoints( pSoldier, (APBPConstants[AP_STEAL_ITEM] / 5), 0, AFTERACTION_INTERRUPT );
				pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
				return ( TRUE );
			}

			fFailure=FALSE;

			// Flugente: if we are disguised and try to steal from a conscious enemy, there is a chance that he notices us and we lose our disguise. If he can see us this always happens
			if ( !fSoldierCollapsed && pSoldier->bSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER) )
			{
				BOOLEAN fUncovered = FALSE;

				// if he sees us, always loose disguise
				if ( pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == SEEN_CURRENTLY )
					fUncovered = TRUE;
				else
				{
					UINT8 chance = 10;

					if ( pTargetSoldier->aiData.bOppList[ pSoldier->ubID ] == SEEN_LAST_TURN )
						chance += 40;

					chance += pSoldier->aiData.bAlertStatus * 10;

					if ( Chance(chance) )
						fUncovered = TRUE;
				}

				if ( fUncovered )
				{
					pSoldier->LooseDisguise();
					pSoldier->Strip();

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_STEAL_FAIL], pSoldier->GetName()  );
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_UNCOVERED], pTargetSoldier->GetName(), pSoldier->GetName()  );
				}
			}

						// WDS 07/19/2008 - Random number use fix
			// Do we have the chance to steal more than 1 item?
			// SANDRO - taking items from collapsed soldiers is treated differently
			// Flugente: if we are on the same team, allow guaranteed full access
			if ( AllowedToStealFromTeamMate(pSoldier->ubID, pTargetSoldier->ubID) || ( fSoldierCollapsed || (!gGameExternalOptions.fEnhancedCloseCombatSystem && iDiceRoll < (iHitChance * 2 / 3))) && pSoldier->bTeam == gbPlayerNum )
			{
				// first, charge extra Aps, because it's difficlut to pickup from other soldier
				if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					DeductPoints( pSoldier, (GetBasicAPsToPickupItem( pSoldier ) * 2), 0, AFTERACTION_INTERRUPT );

				// The item that the enemy holds in his hand before the stealing
				usOldItem = pTargetSoldier->inv[HANDPOS].usItem;

				INT16 sNumStolenItems = StealItems(pSoldier, pTargetSoldier,&ubIndexRet);

				// We have only stolen 1 item, because the enemy has not more than one item.
				if ( sNumStolenItems == 1)
				{
					// charge Aps
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
						DeductPoints( pSoldier, GetBasicAPsToPickupItem( pSoldier ), 0, AFTERACTION_INTERRUPT );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_STOLE_SOMETHING ], pSoldier->GetName(), ShortItemNames[ pTargetSoldier->inv[ubIndexRet].usItem ] );
					if (pTargetSoldier->inv[ubIndexRet].MoveThisObjectTo(gTempObject, 1) == 0) {

						// Try to place the item in the merc inventory
						if (!AutoPlaceObject( pSoldier, &gTempObject, TRUE ))
						{
							// Place the item on the ground
							AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
						}
					}

					// The item that the enemy holds in his hand before the stealing
					usNewItem = pTargetSoldier->inv[HANDPOS].usItem;

					if (usOldItem != usNewItem)
					{
						pTargetSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItem, usNewItem );
					}
				}
				// The enemy has no more items to steal
				else if (sNumStolenItems == 0)
				{
					fNoMoreItems = TRUE;
				}
			}
			// WDS 07/19/2008 - Random number use fix
			// Do we have luck on stealing?
			else if ( iDiceRoll < iHitChance && iHitChance > 0 )
			{
				if ( pTargetSoldier->inv[HANDPOS].exists() == true )
				{
					UINT16 usOldItemTarget = pTargetSoldier->inv[HANDPOS].usItem;

					fStealAttempt = TRUE; // added for check

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_STOLE_SOMETHING ], pSoldier->GetName(), ShortItemNames[ pTargetSoldier->inv[HANDPOS].usItem ] );

					// if successful - place the item to our hands
					if (iDiceRoll < (iHitChance * 2 / 3) && gGameExternalOptions.fEnhancedCloseCombatSystem)
					{
						usOldItem = pSoldier->inv[HANDPOS].usItem;
						OBJECTTYPE oTempObjectSecondHand;
						INT8 bEmptySl;
						oTempObjectSecondHand.usItem = 0;

						// if we have something in main hand, place it into inventory or on the ground
						if (pSoldier->inv[HANDPOS].exists() == true )
						{
							if( pSoldier->bTeam != gbPlayerNum )  // for enemies, ignore rules and place it anywhere
							{
								bEmptySl = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, NUM_INV_SLOTS );
								SwapObjs( pSoldier, HANDPOS, bEmptySl, TRUE );
							}
							else if (!PlaceInAnyPocket( pSoldier, &pSoldier->inv[HANDPOS], FALSE, 5))
							{
								if (!AutoPlaceObject( pSoldier, &pSoldier->inv[HANDPOS], FALSE, 5))
									AddItemToPool( pSoldier->sGridNo, &pSoldier->inv[HANDPOS], 1, pSoldier->pathing.bLevel, 0, -1 );
							}
						}
						// if twohanded item, place the secondhand item if any to temp space
						if (pSoldier->inv[SECONDHANDPOS].exists() == true && Item[pTargetSoldier->inv[HANDPOS].usItem].twohanded)
						{
							if( pSoldier->bTeam != gbPlayerNum )  // for enemies, ignore rules and place it anywhere
							{
								bEmptySl = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, NUM_INV_SLOTS );
								SwapObjs( pSoldier, SECONDHANDPOS, bEmptySl, TRUE );
							}
							else
							{
								pSoldier->inv[SECONDHANDPOS].MoveThisObjectTo(oTempObjectSecondHand, 1);
							}
						}
						// now take the item
						if (pTargetSoldier->inv[HANDPOS].MoveThisObjectTo(gTempObject, 1) == 0)
						{
							if (PlaceObject( pSoldier, HANDPOS, &gTempObject ))
							{
								pSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItem, gTempObject.usItem );
								if( pSoldier->bTeam != gbPlayerNum )  // make sure item is droppable now, so we don't lose our gun if killed this enemy
								{
									pSoldier->inv[HANDPOS].fFlags &= ~OBJECT_UNDROPPABLE;
								}
							}
							else
							{
								AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
							}
						}
						// if second hand item was removed place it now somewhere
						if (oTempObjectSecondHand.usItem != 0 )
						{
							if (!PlaceInAnyPocket( pSoldier, &oTempObjectSecondHand, FALSE, 5))
							{
								if (!AutoPlaceObject( pSoldier, &oTempObjectSecondHand, FALSE ))
								{
									AddItemToPool( pSoldier->sGridNo, &oTempObjectSecondHand, 1, pSoldier->pathing.bLevel, 0, -1 );
								}
							}
						}
						// add to merc records
						if ( pSoldier->ubProfile != NO_PROFILE )
							gMercProfiles[ pSoldier->ubProfile ].records.usItemsStolen++;

					}
					// Item dropped somewhere... roll based on the same chance to determine where!
					else
					{
						if ( pSoldier->bTeam == gbPlayerNum )
						{
							pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
						}
						if (pTargetSoldier->inv[HANDPOS].MoveThisObjectTo(gTempObject, 1) == 0) 
						{
							iDiceRoll = (INT32) PreRandom( 100 );
							if (iDiceRoll < iHitChance)
							{
								// Drop item in the our tile
								AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
							}
							else
							{
								// Drop item in the target's tile
								AddItemToPool( pTargetSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
							}
						}
						// add to merc records
						if ( pSoldier->ubProfile != NO_PROFILE )
							gMercProfiles[ pSoldier->ubProfile ].records.usItemsStolen++;
					}
					// Reload buddy's animation...
					pTargetSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItemTarget, NOTHING );
				}
				// Enemy has no item in his hand.
				else
				{
					fNoMoreItemInHand = TRUE;
				}
				// Reload buddy's animation...
				//pTargetSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItem, NOTHING );
			}
			// We could not steal from the enemy, we had no luck
			else
			{
				fFailure=TRUE;
			}

			// SANDRO - Enhanced Close Combat System 
			// Deduct APs for stealing now (moved from Soldier Control.cpp) - SANDRO
			// If stolen something or fail to steal, reduce APs by the full amount
			if (gGameExternalOptions.fEnhancedCloseCombatSystem)
			{
				if (fSoldierCollapsed)
				{
					// APs are reduced in Handle Items.cpp in "SoldierStealItemFromSoldier"
				}
				else if ( fStealAttempt || (fFailure == TRUE))
				{
					if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && ( gGameOptions.fNewTraitSystem ))
					{
						DeductPoints( pSoldier, max( 1, (INT16)((APBPConstants[AP_STEAL_ITEM] *  (100 - gSkillTraitValues.ubMAReducedAPsToSteal * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/ 100) + 0.5)), 200, AFTERACTION_INTERRUPT );
					}
					else
					{
						DeductPoints( pSoldier, APBPConstants[AP_STEAL_ITEM], 200, AFTERACTION_INTERRUPT );
					}
				}
				// Only 1/7 of original AP cost, if the enemy has nothing to steal
				else if ((fNoMoreItems == TRUE) || (fNoMoreItemInHand == TRUE))
				{
					DeductPoints( pSoldier, (APBPConstants[AP_STEAL_ITEM] / 7), 0, AFTERACTION_INTERRUPT );
				}
			}
			else
			{
				if (HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && ( gGameOptions.fNewTraitSystem ))
				{
					DeductPoints( pSoldier, max( 1, (INT16)((APBPConstants[AP_STEAL_ITEM] *  (100 - gSkillTraitValues.ubMAReducedAPsToSteal * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ))/ 100) + 0.5)), 0, AFTERACTION_INTERRUPT );
				}
				else
				{
					DeductPoints( pSoldier, APBPConstants[AP_STEAL_ITEM], 0, AFTERACTION_INTERRUPT );
				}
			}
						
			// We failed to steal something!
			if (fFailure)
			{
				// Only report if it was not the Nada item!
				if (pTargetSoldier->inv[HANDPOS].exists() == true)
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
						Message[ STR_FAILED_TO_STEAL_SOMETHING ],
						pSoldier->GetName(), ShortItemNames[ pTargetSoldier->inv[HANDPOS].usItem ] );

					if ( pSoldier->bTeam == gbPlayerNum )
					{
						pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					}
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
					Message[ STR_NO_MORE_ITEM_IN_HAND ]);
				}
			}
			// Enemy had no item in its hand
			else if (fNoMoreItemInHand == TRUE)
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
					Message[ STR_NO_MORE_ITEM_IN_HAND ]);
			}

			// The enemy has no more items to steal
			else if (fNoMoreItems == TRUE)
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE,
					Message[ STR_NO_MORE_ITEMS_TO_STEAL ]);
			}

			// Give some experience
			if ( iHitChance > 0 && pSoldier->bTeam == gbPlayerNum && pTargetSoldier->bTeam != gbPlayerNum && !(pTargetSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( pTargetSoldier ) && !TANK( pTargetSoldier ) )
			{
				if (fFailure == FALSE)
				{
					// SANDRO - Enhanced Close Combat System - experience for stealing changed
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					{
						// stealing from unconsious opponents is not rewarded much
						if (fSoldierCollapsed)
						{
							StatChange( pSoldier, DEXTAMT, 1, FALSE );
						}
						else
						{
							// We were successfull in stealing. Give some experience
							StatChange( pSoldier, STRAMT, 6, FALSE );
							StatChange( pSoldier, DEXTAMT, 10, FALSE );
							StatChange( pSoldier, AGILAMT, 10, FALSE );
						}
					}
					else
					{
						// We were successfull in stealing. Give some experience
						StatChange( pSoldier, STRAMT, 8, FALSE );
						StatChange( pSoldier, DEXTAMT, 3, FALSE );
						StatChange( pSoldier, AGILAMT, 3, FALSE );
					}
				}
				else
				{
					// SANDRO - Enhanced Close Combat System - experience for stealing changed
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					{
						// failed a steal; give some experience - slightly changed by SANDRO
						StatChange( pSoldier, STRAMT, 2, FROM_FAILURE );
						StatChange( pSoldier, DEXTAMT, 4, FROM_FAILURE );
						StatChange( pSoldier, AGILAMT, 4, FROM_FAILURE );
					}
					else
					{
						// failed a steal; give some experience
						StatChange( pSoldier, STRAMT, 4, FROM_FAILURE );
						StatChange( pSoldier, DEXTAMT, 1, FROM_FAILURE );
						StatChange( pSoldier, AGILAMT, 1, FROM_FAILURE );
					}
				}
			}

			// SANDRO - Enhanced Close Combat System - Notice merc after stealing
			if (gGameExternalOptions.fEnhancedCloseCombatSystem)
				pTargetSoldier->EVENT_SetSoldierDesiredDirection( GetDirectionFromGridNo( pSoldier->sGridNo, pTargetSoldier ) );

			// 0verhaul:  Also handled in the animation transition
			// #ifdef JA2BETAVERSION
			//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - steal") );
			// #endif
			// FreeUpAttacker( (UINT8) pSoldier->ubID );
		}

		// -----------------------------------
		// Punch, Kick the Enemy
		// -----------------------------------
		else
		{
			// SANDRO - new mercs' records 
			if ( pSoldier->bTeam == gbPlayerNum && pSoldier->ubProfile != NO_PROFILE )
			{
				gMercProfiles[ pSoldier->ubProfile ].records.usHtHAttacks++;
				if ( iDiceRoll < iHitChance )
				{
					gMercProfiles[ pSoldier->ubProfile ].records.usShotsHit++;
				}
			}

			// SANDRO - Enhanced Close Combat System - spinning kick is not automatic but just more damaging
			if (!gGameExternalOptions.fEnhancedCloseCombatSystem)
			{
				// ATE/CC: if doing ninja spin kick (only), automatically make it a hit 
				if ( pSoldier->usAnimState == NINJA_SPINKICK)
				{
					// Let him to succeed by a random amount
					iDiceRoll = PreRandom( iHitChance );				
				}
			}

			if ( pSoldier->bTeam == gbPlayerNum && pTargetSoldier->bTeam != gbPlayerNum )
			{
				// WDS 07/19/2008 - Random number use fix
				// made an HTH attack; give experience
				if ( iDiceRoll < iHitChance )
				{
					ubExpGain = 8;

					// SANDRO - Enhanced Close Combat System - EXP given based on type of attack
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					{ 
						// Increased EXP for head strike
						if (pSoldier->bAimShotLocation == AIM_SHOT_HEAD)
						{
							ubExpGain += 2;
						}
						// Decreased EXP for leg strike
						else if (pSoldier->bAimShotLocation == AIM_SHOT_LEGS)
						{
							ubExpGain -= 4;
						}
						
						// Double EXP for focused attack
						if ( (pSoldier->usAnimState == NINJA_SPINKICK || pSoldier->aiData.bAimTime >= (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : 6)) &&
							((HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT)) && gGameOptions.fNewTraitSystem ))
						{
							ubExpGain *= 2;
						}
					}

					if ( pTargetSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) )
					{
						ubExpGain = 0;
					}
					else if ( pTargetSoldier->ubBodyType == COW || pTargetSoldier->ubBodyType == CROW )
					{
						ubExpGain /= 2;
					}

					// SANDRO - Enhanced Close Combat System
					if ( gGameExternalOptions.fEnhancedCloseCombatSystem && pTargetSoldier->bCollapsed)
					{
						// beating unconscious enemy is a matter of brute strength, so give exp mostly to Stregnth 
						StatChange( pSoldier, DEXTAMT, (ubExpGain+1)/3, FALSE );
						StatChange( pSoldier, STRAMT,  ubExpGain + 1, FALSE );
						StatChange( pSoldier, AGILAMT, (ubExpGain+1)/3, FALSE );
					}
					else
					{
						StatChange( pSoldier, DEXTAMT, ubExpGain, FALSE );
						StatChange( pSoldier, STRAMT,  (ubExpGain+1)/3, FALSE );
						StatChange( pSoldier, AGILAMT, (ubExpGain+1)/3, FALSE );
					}
				}
				else
				{
					ubExpGain = 4;

					// SANDRO - Enhanced Close Combat System - EXP given based on type of attack
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					{ 
						// Increased EXP for head strike
						if (pSoldier->bAimShotLocation == AIM_SHOT_HEAD)
						{
							ubExpGain += 1;
						}
						// Decreased EXP for leg strike
						else if (pSoldier->bAimShotLocation == AIM_SHOT_LEGS)
						{
							ubExpGain -= 2;
						}
						
						// Double EXP for focused attack
						if ( (pSoldier->usAnimState == NINJA_SPINKICK || pSoldier->aiData.bAimTime >= (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : 6)) &&
							((HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT)) && gGameOptions.fNewTraitSystem ) )
						{
							ubExpGain *= 2;
						}
					}

					if ( pTargetSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) )
					{
						ubExpGain = 0;
					}
					else if ( pTargetSoldier->ubBodyType == COW || pTargetSoldier->ubBodyType == CROW )
					{
						ubExpGain /= 2;
					}

					StatChange( pSoldier, DEXTAMT, ubExpGain, FALSE );
					StatChange( pSoldier, STRAMT,  (ubExpGain+1)/3, FALSE );
					StatChange( pSoldier, AGILAMT, (ubExpGain+1)/3, FALSE );
				}
			}
			else if ( pSoldier->bTeam != gbPlayerNum && pTargetSoldier->bTeam == gbPlayerNum )
			{
				// being attacked... if successfully dodged, give experience
				if ( iDiceRoll > iHitChance )
				{
					// SANDRO - Enhanced Close Combat System - More logical - give EXP for dodging to Agility most 
					if (gGameExternalOptions.fEnhancedCloseCombatSystem)
					{ 
						StatChange( pTargetSoldier, DEXTAMT, 5, FALSE ); // 4 istead of 8
						StatChange( pTargetSoldier, STRAMT, 2, FALSE ); // 2 instead of 3
						StatChange( pTargetSoldier, AGILAMT, 8, FALSE ); // 8 instead of 3
					}
					else
					{
						StatChange( pTargetSoldier, DEXTAMT, 8, FALSE );
						StatChange( pTargetSoldier, STRAMT, 3, FALSE );
						StatChange( pTargetSoldier, AGILAMT, 3, FALSE );
					}
				}
			}

			// SANDRO - Enhanced Close Combat System - Add dodge animation of target if missed ninja spin kick
			//if ( (iDiceRoll >= iHitChance) && pSoldier->usAnimState == NINJA_SPINKICK && gGameExternalOptions.fEnhancedCloseCombatSystem
			//	&& gAnimControl[ pTargetSoldier->usAnimState ].ubHeight == ANIM_STAND && IS_MERC_BODY_TYPE( pTargetSoldier ))
			//{
			//	pTargetSoldier->ChangeSoldierState( DODGE_ONE, 0 , FALSE ); 
			//	// after dodging melee attack go to apropriate stance
			//	if ( (gAnimControl[ pTargetSoldier->usAnimState ].ubHeight == ANIM_STAND) && (Item[pTargetSoldier->inv[HANDPOS].usItem].usItemClass & (IC_NONE | IC_PUNCH)) && pTargetSoldier->stats.bLife > 30 && pTargetSoldier->bBreath > 25 )
			//	{
			//		if ((((NUM_SKILL_TRAITS( pTargetSoldier, MARTIAL_ARTS_NT ) >= ((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA) ? 2 : 1 )) && gGameOptions.fNewTraitSystem ) ||
			//			(HAS_SKILL_TRAIT( pTargetSoldier, MARTIALARTS_OT ) && !gGameOptions.fNewTraitSystem ) ) &&
			//			 pTargetSoldier->ubBodyType == REGMALE )
			//		{
			//			//pTSoldier->usPendingAnimation = NINJA_GOTOBREATH;
			//			pTargetSoldier->usPendingAnimation = NINJA_BREATH ;
			//		}
			//		else
			//		{
			//			pTargetSoldier->usPendingAnimation = PUNCH_BREATH ;
			//		}
			//	}
			//	else if ( (gAnimControl[ pTargetSoldier->usAnimState ].ubHeight == ANIM_STAND) && (Item[pTargetSoldier->inv[HANDPOS].usItem].usItemClass == IC_BLADE))
			//	{
			//		//pTSoldier->usPendingAnimation = KNIFE_GOTOBREATH;
			//		pTargetSoldier->usPendingAnimation = KNIFE_BREATH ;
			//	}
			//}
			// WDS 07/19/2008 - Random number use fix
#if (defined JA2UB) 
//Ja25 no meanwhiles
	        if ( iDiceRoll < iHitChance )
#else
			if ( iDiceRoll < iHitChance || AreInMeanwhile( ) )
#endif
			{
				// CALCULATE DAMAGE!
				iImpact = HTHImpact( pSoldier, pTargetSoldier, (iHitChance - iDiceRoll), FALSE );

				// SANDRO - new merc records - times wounded (punched)
				if ( pTargetSoldier->ubProfile != NO_PROFILE )
					gMercProfiles[ pTargetSoldier->ubProfile ].records.usTimesWoundedPunched++;

				// Send event for getting hit
				memset( &(SWeaponHit), 0, sizeof( SWeaponHit ) );
				SWeaponHit.usSoldierID			= pTargetSoldier->ubID;
				SWeaponHit.usWeaponIndex		= pSoldier->usAttackingWeapon;
				SWeaponHit.sDamage					= (INT16) iImpact;
				SWeaponHit.usDirection			= (UINT8)GetDirectionFromGridNo( pSoldier->sGridNo, pTargetSoldier );
				SWeaponHit.sXPos						= (INT16)pTargetSoldier->dXPos;
				SWeaponHit.sYPos						= (INT16)pTargetSoldier->dYPos;
				SWeaponHit.sZPos						= 20;
				SWeaponHit.sRange						= 1;
				SWeaponHit.ubAttackerID			= pSoldier->ubID;
				SWeaponHit.fHit							= TRUE;
				SWeaponHit.ubSpecial				= FIRE_WEAPON_NO_SPECIAL;
				AddGameEvent( S_WEAPONHIT, (UINT16) 20, &SWeaponHit );
			}
			// 0verhaul:  And this too
			// else
			// {
			//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in HTH attack") );
			//	FreeUpAttacker( (UINT8) pSoldier->ubID );
			// }
		}
	}

	// possibly reduce monster smell (gunpowder smell)
	if ( pSoldier->aiData.bMonsterSmell > 0 && Random( 5 ) == 0 )
	{
		pSoldier->aiData.bMonsterSmell--;
	}


	return( TRUE );
}

BOOLEAN UseThrown( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo )
{
	UINT32			uiHitChance, uiDiceRoll;
	INT8			bLoop;
	UINT8			ubTargetID;
	SOLDIERTYPE	*	pTargetSoldier;
	INT16			sAPCost = 0;

	uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, AIM_SHOT_TORSO );

	uiDiceRoll = PreRandom( 100 );

	#ifdef JA2BETAVERSION
	if ( gfReportHitChances )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Hit chance was %ld, roll %ld (range %d)", uiHitChance, uiDiceRoll, PythSpacesAway( pSoldier->sGridNo, sTargetGridNo ) );
	}
	#endif

	if ( pSoldier->bTeam == gbPlayerNum && gTacticalStatus.uiFlags & INCOMBAT )
	{
		// check target gridno
		ubTargetID = WhoIsThere2( pSoldier->sTargetGridNo, pSoldier->bTargetLevel );
		if ( ubTargetID == NOBODY )
		{
			pTargetSoldier = NULL;
		}
		else
		{
			pTargetSoldier = MercPtrs[ ubTargetID ];
		}

		if ( pTargetSoldier && pTargetSoldier->bTeam == pSoldier->bTeam )
		{
			// ignore!
			pTargetSoldier = NULL;
		}

		// Snap: Find the number of real targets for grenade attack
		UINT16 usNumTargets = 0;
		if ( pTargetSoldier != NULL )
		{
			usNumTargets++;
		}
			// search for an opponent near the target gridno
			for ( bLoop = 0; bLoop < NUM_WORLD_DIRECTIONS; bLoop++ )
			{
				ubTargetID = WhoIsThere2( NewGridNo( pSoldier->sTargetGridNo, DirectionInc( bLoop ) ), pSoldier->bTargetLevel );
				pTargetSoldier = NULL;
				if ( ubTargetID != NOBODY )
				{
					pTargetSoldier = MercPtrs[ ubTargetID ];
					if ( pTargetSoldier->bTeam != pSoldier->bTeam )
					{
						usNumTargets++;
				}
			}
		}

		if ( usNumTargets )
		{
			// ok this is a real attack on someone, grant experience
			// Snap: calculate experience points for strength: weight x distance
			UINT16 usHandItem = pSoldier->inv[HANDPOS].usItem;
			// calculate actual range (in world units)
			INT32 iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sTargetGridNo );
			// 10 points for a regular grenade thrown at 100 units (1 cell = 10 units)
			UINT16 usExpGain = ( Item[usHandItem].ubWeight * iRange + 30) / 60;
			StatChange( pSoldier, STRAMT, usExpGain, FALSE );

			// Snap: calculate experience points for dexterity and marksmanship
			usExpGain = 5     // For at least one target
				+ 2*(usNumTargets-1);   // For every additional target
			BOOLEAN fGonnaHit = uiDiceRoll < uiHitChance;
			if ( fGonnaHit ) usExpGain *= 2;   // For actually hitting the target :)
			usExpGain += (UINT16) (100 - uiHitChance) / 10; // Extra exp for a difficult target
			StatChange( pSoldier, DEXTAMT, usExpGain / 2, ( fGonnaHit ? FALSE : FROM_FAILURE ) );
			StatChange( pSoldier, MARKAMT, usExpGain / 2, ( fGonnaHit ? FALSE : FROM_FAILURE ) );

			/*if ( uiDiceRoll < uiHitChance )
			{
				usExpGain *= 2;
				usExpGain += (UINT16) ( 100 - uiHitChance ) / 10;
				StatChange( pSoldier, DEXTAMT, usExpGain / 2, FALSE );
				StatChange( pSoldier, MARKAMT, usExpGain / 2, FALSE );
			}
			else
			{
				StatChange( pSoldier, DEXTAMT, usExpGain / 2, FROM_FAILURE );
				StatChange( pSoldier, MARKAMT, usExpGain / 2, FROM_FAILURE );
			}*/
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - new mercs' records
	if ( Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass == IC_GRENADE || Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass == IC_BOMB || Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass == IC_THROWN)
	{
		gMercProfiles[ pSoldier->ubProfile ].records.usGrenadesThrown++;			
	}
	/////////////////////////////////////////////////////////////////////////////////////

	CalculateLaunchItemParamsForThrow( pSoldier, sTargetGridNo, pSoldier->bTargetLevel, (INT16)(pSoldier->bTargetLevel * 256 ), &(pSoldier->inv[ HANDPOS ] ), (INT8)(uiDiceRoll - uiHitChance), THROW_ARM_ITEM, 0 );

	//AXP 25.03.2007: Cleaned up throwing AP costs. Now only turning + stance change AP
	//	costs are deducted. Final throw cost is deducted on creating the grenade object
	if ( (UINT8)GetDirectionFromGridNo( sTargetGridNo, pSoldier ) != pSoldier->ubDirection )
			sAPCost += (INT16)GetAPsToLook( pSoldier );
	sAPCost += GetAPsToChangeStance( pSoldier, ANIM_STAND );

	HandleSoldierThrowItem( pSoldier, pSoldier->sTargetGridNo );
	DeductPoints( pSoldier, sAPCost, 0, AFTERSHOT_INTERRUPT );
	pSoldier->inv[ HANDPOS ].RemoveObjectsFromStack(1);

	/*
	// Madd: Next 2 lines added: Deduct points!

	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bAimTime );
    // Kaiden: Deducting points too early, moving the line down
	//DeductPoints( pSoldier, sAPCost, 0 );

	// OK, goto throw animation
	HandleSoldierThrowItem( pSoldier, pSoldier->sTargetGridNo );
    // Kaiden: Deducting points too early, moving the line down
	DeductPoints( pSoldier, sAPCost, 0 );
	RemoveObjs( &(pSoldier->inv[ HANDPOS ] ), 1 );
	*/
	return( TRUE );
}


BOOLEAN UseLauncher( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo )
{
	UINT32			uiHitChance, uiDiceRoll;
	INT16				sAPCost = 0;
	INT32				iBPCost = 0;
	OBJECTTYPE	Launchable;
	OBJECTTYPE * pObj;
	UINT16			usItemNum;
	INT32       iID;
	REAL_OBJECT *pObject;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("UseLauncher, target = %d", sTargetGridNo) );
	usItemNum = pSoldier->usAttackingWeapon;

	if ( !EnoughAmmo( pSoldier, TRUE, pSoldier->ubAttackingHand ) )
	{
		return( FALSE );
	}

	pObj = &(pSoldier->inv[HANDPOS]);
	attachmentList::iterator iter;
	for (iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
		if ( Item[ iter->usItem ].usItemClass & IC_EXPLOSV && iter->exists())
		{
			break;
		}
	}
	if (iter == (*pObj)[0]->attachments.end())
	{
		// this should not happen!!
		return( FALSE );
	}

	//CHRISL: Added a magsize value to Explosives.xml which is checked by GetExpMagSize.  This will allow OAS to load individual grenades into
	//	multi-launch grenades and fire correctly
	//if ( Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize > 1 || ( Item[pObj->usItem].grenadelauncher && GetMagSize(pObj) > 1 ))
	if(GetExpMagSize(&(*iter)) > 1)
		CreateItem( iter->usItem, 100, &Launchable );
	else
		CreateItem( iter->usItem, (*iter)[0]->data.objectStatus, &Launchable );
	//if ( pSoldier->usAttackingWeapon == pObj->usItem )
	//{
		DeductAmmo( pSoldier, HANDPOS );
	//}
	//else
	//{
	//	// Firing an attached grenade launcher... the attachment we found above
	//	// is the one to remove!
	//	RemoveAttachment( pObj, bAttachPos);
	//}

	// Flugente fix: until now, pobj was always used to determine wether the explosive weapon will fail - but in the case of underbarrel weapons that wasn't the gun fired
	// we now determine the real weapon being fired
	OBJECTTYPE* pgunobj = pObj;
	if ( pSoldier->bWeaponMode & (WM_ATTACHED_GL|WM_ATTACHED_GL_BURST|WM_ATTACHED_GL_AUTO) )
	{
		// firing an attached weapon 
		attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
		for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
		{
			if ( iter->exists() && Item[ iter->usItem ].usItemClass & IC_LAUNCHER )
			{
				pgunobj = &(*iter);
				break;
			}
		}
	}
	
  // ATE: Check here if the launcher should fail 'cause of bad status.....
  if ( WillExplosiveWeaponFail( pSoldier, pgunobj ) )
  {
	GunIncreaseHeat( pgunobj, pSoldier );
    // Explode dude!

    // So we still should have ABC > 0
    // Begin explosion due to failure...
		IgniteExplosion( pSoldier->ubID, CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, Launchable.usItem, pSoldier->pathing.bLevel );

    // Reduce again for attack end 'cause it has been incremented for a normal attack
    // Nope, not anymore.
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK, Now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
		DebugAttackBusy( String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK\n", gAnimControl[ pSoldier->usAnimState ].zAnimStr ) );
		// ReduceAttackBusyCount( pSoldier->ubID, FALSE );

    // So all's well, should be good from here....
    return( FALSE );
  }

	GunIncreaseHeat( pgunobj, pSoldier );

	// Flugente: if we are using a rifle grenade, we also use up one of the gun's bullets
	if ( IsAttachmentClass(pgunobj->usItem, AC_RIFLEGRENADE) )
	{
		if ( (*pObj)[0]->data.gun.ubGunShotsLeft> 0 )
			(*pObj)[0]->data.gun.ubGunShotsLeft--;

		// increase heat, as we 'fired' a bullet
		GunIncreaseHeat( pObj, pSoldier );
	}

	if ( Weapon[ usItemNum ].sSound != NO_WEAPON_SOUND  )
	{
		if (Item[ usItemNum ].usItemClass == IC_GUN  )
			PlayJA2Sample( Weapon[ UNDER_GLAUNCHER ].sSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		else
			PlayJA2Sample( Weapon[ usItemNum ].sSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
	}

	uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->aiData.bAimTime, AIM_SHOT_TORSO );

	uiDiceRoll = PreRandom( 100 );

	#ifdef JA2BETAVERSION
	if ( gfReportHitChances )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Hit chance was %ld, roll %ld (range %d)", uiHitChance, uiDiceRoll, PythSpacesAway( pSoldier->sGridNo, sTargetGridNo ) );
	}
	#endif


	if ( Item[ usItemNum ].usItemClass == IC_LAUNCHER )
	{
		// Preserve gridno!
		//pSoldier->sLastTarget = sTargetGridNo;

		//sAPCost = MinAPsToAttack( pSoldier, sTargetGridNo, TRUE );

	 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->aiData.bShownAimTime );
		// SANDRO: get BP cost for weapon manipulating
		if ( gGameExternalOptions.ubEnergyCostForWeaponWeight )
			iBPCost = sAPCost * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;
		else 
			iBPCost = 0;

	}
	else
	{
		// Throw....
		sAPCost = MinAPsToThrow( pSoldier, sTargetGridNo, FALSE );
		// SANDRO: get BP cost for weapon manipulating
		if ( gGameExternalOptions.ubEnergyCostForWeaponWeight )
			iBPCost = sAPCost * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;
		else 
			iBPCost = 0;
	}

	if ( pSoldier->bDoBurst )
	{
		// ONly deduct points once
		if ( pSoldier->bDoBurst == 1 )
		{
			DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
		}
	}
	else
	{
		DeductPoints( pSoldier, sAPCost, iBPCost, AFTERSHOT_INTERRUPT );
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - new mercs' records
	if ( Item[ usItemNum ].usItemClass == IC_LAUNCHER || Item[usItemNum].grenadelauncher || 
		Item[usItemNum].rocketlauncher || Item[usItemNum].singleshotrocketlauncher || Item[usItemNum].mortar )
	{
		gMercProfiles[ pSoldier->ubProfile ].records.usMissilesLaunched++;			
	}
	/////////////////////////////////////////////////////////////////////////////////////

	CalculateLaunchItemParamsForThrow( pSoldier, sTargetGridNo, pSoldier->bTargetLevel, 0, &Launchable, (INT8)(uiDiceRoll - uiHitChance), THROW_ARM_ITEM, 0 );

	iID = CreatePhysicalObject( pSoldier->pTempObject, pSoldier->pThrowParams->dLifeSpan,  pSoldier->pThrowParams->dX, pSoldier->pThrowParams->dY, pSoldier->pThrowParams->dZ, pSoldier->pThrowParams->dForceX, pSoldier->pThrowParams->dForceY, pSoldier->pThrowParams->dForceZ, pSoldier->ubID, pSoldier->pThrowParams->ubActionCode, pSoldier->pThrowParams->uiActionData, FALSE );

	// OJW - 20091002 - Explosives
	if (is_networked && is_client)
	{
		if (pSoldier->bTeam == 0 || (pSoldier->bTeam == 1 && is_server))
		{
			send_grenade( pSoldier->pTempObject , pSoldier->pThrowParams->dLifeSpan,	pSoldier->pThrowParams->dX, pSoldier->pThrowParams->dY, pSoldier->pThrowParams->dZ, pSoldier->pThrowParams->dForceX, pSoldier->pThrowParams->dForceY, pSoldier->pThrowParams->dForceZ, sTargetGridNo, pSoldier->ubID, pSoldier->pThrowParams->ubActionCode, pSoldier->pThrowParams->uiActionData, iID , false);
		}
	}

	pObject = &( ObjectSlots[ iID ] );
  //pObject->fPotentialForDebug = TRUE;


	OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );

	MemFree( pSoldier->pThrowParams );
	pSoldier->pThrowParams = NULL;

	if ( pSoldier->bDoBurst )
	{
		// done, if bursting, increment
		pSoldier->bDoBurst++;
	}

	return( TRUE );
}

BOOLEAN DoSpecialEffectAmmoMiss( UINT8 ubAttackerID, INT32 sGridNo, INT16 sXPos, INT16 sYPos, INT16 sZPos, BOOLEAN fSoundOnly, BOOLEAN fFreeupAttacker, INT32 iBullet )
{
	ANITILE_PARAMS	AniParams;
	UINT8						ubAmmoType;
	UINT16          usItem;

	// Flugente: check for underbarrel weapons and use that object if necessary
	OBJECTTYPE* pObj = MercPtrs[ ubAttackerID ]->GetUsedWeapon( &MercPtrs[ ubAttackerID ]->inv[ MercPtrs[ ubAttackerID ]->ubAttackingHand ] );

	ubAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;
	usItem     = (*pObj).usItem;

	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );

	if ( AmmoTypes[ubAmmoType].explosionSize == 1 )
	{
		if ( !fSoundOnly )
		{
			AniParams.sGridNo							= sGridNo;
			AniParams.ubLevelID						= ANI_TOPMOST_LEVEL;
			AniParams.sDelay							= (INT16)( 100 );
			AniParams.sStartFrame					= 0;
			AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
			AniParams.sX									= sXPos;
			AniParams.sY									= sYPos;
			AniParams.sZ									= sZPos;

			strcpy( AniParams.zCachedFile, "TILECACHE\\MINIBOOM.STI" );

			CreateAnimationTile( &AniParams );

			if ( fFreeupAttacker )
			{
				RemoveBullet( iBullet );
				// 0verhaul:  No longer necessary
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				// FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}
		
		if (!TileIsOutOfBounds(sGridNo))
		{
			PlayJA2Sample( SMALL_EXPLODE_1 , RATE_11025, SoundVolume( (INT8)HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
		}
		else
		{
			PlayJA2Sample( SMALL_EXPLODE_1 , RATE_11025, MIDVOLUME, 1, MIDDLE );
		}

		return( TRUE );
	}
	else if ( AmmoTypes[ubAmmoType].explosionSize == 2 )
	{
		if ( !fSoundOnly )
		{
			AniParams.sGridNo							= sGridNo;
			AniParams.ubLevelID						= ANI_TOPMOST_LEVEL;
			AniParams.sDelay							= (INT16)( 100 );
			AniParams.sStartFrame					= 0;
			AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
			AniParams.sX									= sXPos;
			AniParams.sY									= sYPos;
			AniParams.sZ									= sZPos;

			strcpy( AniParams.zCachedFile, "TILECACHE\\ZGRAV_D.STI" );

			CreateAnimationTile( &AniParams );

			if ( fFreeupAttacker )
			{
				RemoveBullet( iBullet );
				// 0verhaul:  No longer necessary.
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				// FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}
		
		if (!TileIsOutOfBounds(sGridNo))
		{
			PlayJA2Sample( EXPLOSION_1 , RATE_11025, SoundVolume( (INT8)HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
		}
		else
		{
			PlayJA2Sample( EXPLOSION_1 , RATE_11025, MIDVOLUME, 1, MIDDLE );
		}

		return( TRUE );
	}
	else if ( AmmoTypes[ubAmmoType].explosionSize == 3 )
	{
		if ( !fSoundOnly )
		{
			AniParams.sGridNo							= sGridNo;
			AniParams.ubLevelID						= ANI_TOPMOST_LEVEL;
			AniParams.sDelay							= (INT16)( 100 );
			AniParams.sStartFrame					= 0;
			AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_ALWAYS_TRANSLUCENT;
			AniParams.sX									= sXPos;
			AniParams.sY									= sYPos;
			AniParams.sZ									= sZPos;

			strcpy( AniParams.zCachedFile, "TILECACHE\\ZGRAV_C.STI" );

			CreateAnimationTile( &AniParams );

			if ( fFreeupAttacker )
			{
				RemoveBullet( iBullet );
				// 0verhaul:  No longer necessary.
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				// FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}
		
		if (!TileIsOutOfBounds(sGridNo))
		{
			PlayJA2Sample( EXPLOSION_BLAST_2 , RATE_11025, SoundVolume( (INT8)HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
		}
		else
		{
			PlayJA2Sample( EXPLOSION_BLAST_2 , RATE_11025, MIDVOLUME, 1, MIDDLE );
		}

		return( TRUE );
	}
	else if ( AmmoTypes[ubAmmoType].monsterSpit )
	{
    // Increment attack busy...
	  // gTacticalStatus.ubAttackBusyCount++;
	  // DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Explosion gone off, COunt now %d", gTacticalStatus.ubAttackBusyCount ) );

		PlayJA2Sample( CREATURE_GAS_NOISE, RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
		// 0verhaul:  Not ready to simplify this to a single call yet.  What we need in order to
		// fix this correctly is to detach 'alien spit' as a special caliber and instead add 3 new calibers for actual "ammo":
		// very small spit, small spit, and large spit.  Then we need to add a special effect miss index to point to the
		// appropriate smoke effect to disperse.
		//NewSmokeEffect( sGridNo, usItem, 0, ubAttackerID);

	    // Do Spread effect.......
		switch( usItem )
	    {
  			case CREATURE_YOUNG_MALE_SPIT:
			case CREATURE_INFANT_SPIT:

		  	NewSmokeEffect( sGridNo, VERY_SMALL_CREATURE_GAS, 0, ubAttackerID );
	        break;
	      case CREATURE_OLD_MALE_SPIT:
		  	NewSmokeEffect( sGridNo, SMALL_CREATURE_GAS, 0, ubAttackerID );
			break;

	      case CREATURE_QUEEN_SPIT:
		  	NewSmokeEffect( sGridNo, LARGE_CREATURE_GAS, 0, ubAttackerID );
			break;
	    }
	}

	return( FALSE );
}


void WeaponHit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	SOLDIERTYPE				*pTargetSoldier, *pSoldier;
	OBJECTTYPE				*pObj;
	// Get attacker
	pSoldier				= MercPtrs[ ubAttackerID ];

	// Flugente: check for underbarrel weapons and use that object if necessary
	pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );

	// Get Target
	pTargetSoldier	= MercPtrs[ usSoldierID ];

	MakeNoise( ubAttackerID, pTargetSoldier->sGridNo, pTargetSoldier->pathing.bLevel, gpWorldLevelData[pTargetSoldier->sGridNo].ubTerrainID, Weapon[ usWeaponIndex ].ubHitVolume, NOISE_BULLET_IMPACT );

	// CALLAHAN START BUGFIX
	// Provisions for Fragments, which are resulting from a different weapon than the one we are holding in our hand.
	UINT8 ubAmmoType = 0;
	if ( pObj->usItem == usWeaponIndex || EXPLOSIVE_GUN( usWeaponIndex ))	// WANNE: This fixes the bug, that ONE shot LAWs do not explode on a direct target hit!
	{
		ubAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;
	}
	else
	{
		// Must be a fragment.
		ubAmmoType = Explosive[Item[usWeaponIndex].ubClassIndex].ubFragType;
		usWeaponIndex = 1; // Set to default gun.
	}
	
	if ( EXPLOSIVE_GUN( usWeaponIndex ) || AmmoTypes[ubAmmoType].explosionSize > 1)
	// CALLAHAN END BUGFIX
	{
		// Reduce attacker count!
		//TODO: Madd --- I don't think this code will ever get called for the HE ammo -- the EXPLOSIVE_GUN check filters out regular guns
		// marke test mag ammo type: pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.ubGunAmmoType
                // 2cond 'or' added
		if ( Item[usWeaponIndex].rocketlauncher || AmmoTypes[ubAmmoType].explosionSize > 1 )
		{
			if ( Item[usWeaponIndex].singleshotrocketlauncher )
			{
				IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos ), C1, pTargetSoldier->pathing.bLevel );
			}
			// changed rpg type to work only with two flags matching
			else if ( !Item[usWeaponIndex].singleshotrocketlauncher && Item[usWeaponIndex].rocketlauncher)
				//we shouldn't be able to have an underbarrel firing mode in this step, so we keep the original code :JMich
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("WeaponHit: RPG7 item: %d, Ammo: %d",pSoldier->inv[HANDPOS].usItem , pSoldier->inv[HANDPOS][0]->data.gun.usGunAmmoItem ) );

				IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos ), pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem, pTargetSoldier->pathing.bLevel );
				
				//This is just to make multishot launchers work in semi auto. It's not really a permanent solution because it still doesn't allow autofire, but it will do for now.
				OBJECTTYPE * pLaunchable = FindLaunchableAttachment( &(pSoldier->inv[pSoldier->ubAttackingHand ]), pSoldier->inv[pSoldier->ubAttackingHand ].usItem );
				if(pLaunchable){
					pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem = pLaunchable->usItem;
				} else {
					pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem = NONE;
				}
			}
		    else if ( AmmoTypes[ubAmmoType].explosionSize > 1)
			{
				// re-routed the Highexplosive value to define exposion type
				IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos ), AmmoTypes[ubAmmoType].highExplosive , pTargetSoldier->pathing.bLevel );
				// pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem = NONE;
			}
		}
		else // tank cannon
		{
			// HEADROCK HAM 5 TODO: TANK_SHELL!
			IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos ), TANK_SHELL, pTargetSoldier->pathing.bLevel );
		}

		// 0verhaul:  No longer necessary
		// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of LAW fire") );
		// FreeUpAttacker( ubAttackerID );
		return;
	}

	DoSpecialEffectAmmoMiss( ubAttackerID, pTargetSoldier->sGridNo, sXPos, sYPos, sZPos, FALSE, FALSE, 0 );

	// OK, SHOT HAS HIT, DO THINGS APPROPRIATELY
	// ATE: This is 'cause of that darn smoke effect that could potentially kill
	// the poor bastard .. so check
	if ( !pTargetSoldier->flags.fDoingExternalDeath )
	{
		pTargetSoldier->EVENT_SoldierGotHit(	usWeaponIndex, sDamage, sBreathLoss, usDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation, 0, NOWHERE );
	}
	// else
	// {
	//	Buddy had died from additional damage - free up attacker here...
	//	ReduceAttackBusyCount( pTargetSoldier->ubAttackerID, FALSE );
	//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Special effect killed before bullet impact, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
	//	OutputDebugString( "Special effect killed before bullet impact\n" );
	// }
}


void StructureHit( INT32 iBullet, UINT16 usWeaponIndex, INT16 bWeaponStatus, UINT8 ubAttackerID, UINT16 sXPos, INT16 sYPos, INT16 sZPos, UINT16 usStructureID, INT32 iImpact, BOOLEAN fStopped )
{
	BOOLEAN						fDoMissForGun = FALSE;
	ANITILE						*pNode;
	INT32 sGridNo;
	ANITILE_PARAMS	AniParams;
	UINT16					usMissTileIndex, usMissTileType;
	STRUCTURE				*pStructure = NULL;
	UINT32					uiMissVolume = MIDVOLUME;
	BOOLEAN					fHitSameStructureAsBefore;
	BULLET *				pBullet;
	SOLDIERTYPE *		pAttacker = NULL;
	OBJECTTYPE * pObj;

	pBullet = GetBulletPtr( iBullet );
    Assert(pBullet);

	// Flugente: check for underbarrel weapons and use that object if necessary
	pObj = MercPtrs[ ubAttackerID ]->GetUsedWeapon( &MercPtrs [ ubAttackerID ]->inv[MercPtrs[ubAttackerID]->ubAttackingHand] );

	// Get attacker
	if (ubAttackerID != NOBODY)
	{
		pAttacker = MercPtrs[ ubAttackerID ];
	}

	// HEADROCK HAM 5: Define differently for fragments
	UINT8 ubHitVolume = 0;
	UINT8 ubAmmoType = 0;

	if (pBullet->fFragment)
	{
		ubHitVolume = 1;
		ubAmmoType = Explosive[Item[pBullet->fromItem].ubClassIndex].ubFragType;
	}
	else
	{
		ubHitVolume = Weapon[usWeaponIndex].ubHitVolume;
		ubAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;
	}

	if ( fStopped && ubAttackerID != NOBODY )
	{
		if ( pAttacker->ubOppNum != NOBODY )
		{
			// if it was another team shooting at someone under our control
			if ( (pAttacker->bTeam != Menptr[ pAttacker->ubOppNum ].bTeam ) )
			{
				// if OPPONENT is under our control
				if (Menptr[ pAttacker->ubOppNum ].bTeam == gbPlayerNum )
				{
					// AGILITY GAIN: Opponent "dodged" a bullet shot at him (it missed)
					StatChange( MercPtrs[ pAttacker->ubOppNum ], AGILAMT, 5, FROM_FAILURE );
				}
			}
		}
	}

	if ( pBullet )
	{
		fHitSameStructureAsBefore = ( usStructureID == pBullet->usLastStructureHit );
	}
	else
	{
		// WTF?
		fHitSameStructureAsBefore = FALSE;
	}

	sGridNo = MAPROWCOLTOPOS( (sYPos/CELL_Y_SIZE), (sXPos/CELL_X_SIZE) );
	if ( !fHitSameStructureAsBefore )
	{


		if (sZPos > WALL_HEIGHT)
		{
			MakeNoise( ubAttackerID, sGridNo, 1, gpWorldLevelData[sGridNo].ubTerrainID, ubHitVolume, NOISE_BULLET_IMPACT );
		}
		else
		{
			MakeNoise( ubAttackerID, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, ubHitVolume, NOISE_BULLET_IMPACT );
		}

	}

	if (fStopped)
	{
		// marke need another attacker id assignment
		SOLDIERTYPE				*pSoldier;

	    // Get attacker
	    pSoldier				= MercPtrs[ ubAttackerID ];
                 // marke added one 'or' to get this working with HE ammo
		if ( Item[usWeaponIndex].rocketlauncher || AmmoTypes[ (*pObj)[0]->data.gun.ubGunAmmoType].explosionSize > 1)
		{
			// Reduce attacker count!
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of LAW fire") );
			// FreeUpAttacker( ubAttackerID );
			// HEADROCK HAM 5: Fragments fired by such weapons should not explode. 
			if ( pBullet->fFragment == false)
			{
				if ( Item[usWeaponIndex].singleshotrocketlauncher )
				{
					// HEADROCK HAM 5 TODO: C1!!!
					IgniteExplosion( ubAttackerID, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, C1, (INT8)( sZPos >= WALL_HEIGHT ) );
				}
				// changed too to use 2 flag to determine
				else if ( !Item[usWeaponIndex].singleshotrocketlauncher && Item[usWeaponIndex].rocketlauncher)
					//there shouldn't be a way to enter here with an UnderBarrel weapon, so retaining original code :JMich
				{
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d",pAttacker->inv[HANDPOS].usItem , pAttacker->inv[HANDPOS][0]->data.gun.usGunAmmoItem ) );
					IgniteExplosion( ubAttackerID, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, pAttacker->inv[pAttacker->ubAttackingHand ][0]->data.gun.usGunAmmoItem , (INT8)( sZPos >= WALL_HEIGHT ) );
					
					//This is just to make multishot launchers work in semi auto. It's not really a permanent solution because it still doesn't allow autofire, but it will do for now.
					OBJECTTYPE * pLaunchable = FindLaunchableAttachment( &(pAttacker->inv[pAttacker->ubAttackingHand ]), pAttacker->inv[pAttacker->ubAttackingHand ].usItem );
					if(pLaunchable){
						pAttacker->inv[pAttacker->ubAttackingHand ][0]->data.gun.usGunAmmoItem = pLaunchable->usItem;
					} else {
						pAttacker->inv[pAttacker->ubAttackingHand ][0]->data.gun.usGunAmmoItem = NONE;
					}
				}
				else if ( AmmoTypes[(*pObj)[0]->data.gun.ubGunAmmoType].explosionSize > 1)
				{
					// re-routed the Highexplosive value to define exposion type
					IgniteExplosion( ubAttackerID, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, AmmoTypes[ (*pObj)[0]->data.gun.ubGunAmmoType].highExplosive , (INT8)( sZPos >= WALL_HEIGHT ) );
					// pSoldier->inv[pSoldier->ubAttackingHand ][0]->data.gun.usGunAmmoItem = NONE;
				}
			}

			// Moved here to make sure ABC stays >0 until everything done
			RemoveBullet( iBullet );

			return;
		}

		if ( Item[usWeaponIndex].cannon )
		{
			// Reduce attacker count!
			//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of TANK fire") );
			//FreeUpAttacker( ubAttackerID );

			// HEADROCK HAM 5 TODO: Tank shell!!
			IgniteExplosion( ubAttackerID, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, TANK_SHELL, (INT8)( sZPos >= WALL_HEIGHT ) );
			//FreeUpAttacker( (UINT8) ubAttackerID );

			// Moved here to keep ABC >0 as long as possible
			RemoveBullet( iBullet );

			return;
		}
	}

	// Get Structure pointer and damage it!
	if ( usStructureID != INVALID_STRUCTURE_ID )
	{
		pStructure = FindStructureByID( sGridNo, usStructureID );

		DamageStructure( pStructure, (UINT8)iImpact, STRUCTURE_DAMAGE_GUNFIRE, sGridNo, sXPos, sYPos, ubAttackerID );
	}

	// HEADROCK HAM 5: Fragments are not fired from guns, so they need a special case.
	if (pBullet->fFragment == false)
	{
		switch(  Weapon[ usWeaponIndex ].ubWeaponClass )
		{
			case HANDGUNCLASS:
			case RIFLECLASS:
			case SHOTGUNCLASS:
			case SMGCLASS:
			case MGCLASS:

				// Guy has missed, play random sound
				if (  MercPtrs[ ubAttackerID ]->bTeam == gbPlayerNum )
				{
					if ( !MercPtrs[ ubAttackerID ]->bDoBurst )
					{
						if ( Random( 40 ) == 0 )
						{
							MercPtrs[ ubAttackerID ]->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
						}
					}
				}
				//fDoMissForGun = TRUE;
				//break;
				fDoMissForGun = TRUE;
				break;

			case MONSTERCLASS:

				DoSpecialEffectAmmoMiss( ubAttackerID, sGridNo, sXPos, sYPos, sZPos, FALSE, TRUE, iBullet );

				RemoveBullet( iBullet );
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - monster attack hit structure") );
				// FreeUpAttacker( (UINT8) ubAttackerID );

				//PlayJA2Sample( SPIT_RICOCHET , RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );
				break;

			case KNIFECLASS:

				// When it hits the ground, leave on map...
				if ( Item[ usWeaponIndex ].usItemClass == IC_THROWING_KNIFE )
				{
					//dnl ch67 080913
					INT8 bMaxLeft, bMaxRight, bMaxUp, bMaxDown, bXOffset, bYOffset, ubSearchRange;
					INT32 iGridNo = NOWHERE;
					for(ubSearchRange = 0; ubSearchRange < 4 && iGridNo == NOWHERE; ubSearchRange++)
					{
						bMaxLeft = bMaxRight = ubSearchRange;// determine maximum horizontal limits
						bMaxUp = bMaxDown = ubSearchRange;// determine maximum vertical limits
						for(bYOffset = -bMaxUp; bYOffset <= bMaxDown && iGridNo == NOWHERE; bYOffset++)// evaluate every tile until find first reachable one
							for(bXOffset = -bMaxLeft; bXOffset <= bMaxRight; bXOffset++)
							{
								iGridNo = pBullet->sGridNo + bXOffset + (MAXCOL * bYOffset);// calculate the next potential gridno near this opponent
								if(!TileIsOutOfBounds(iGridNo) && !FindStructure(iGridNo, STRUCTURE_BLOCKSMOVES))
									break;
								iGridNo = NOWHERE;
							}
					}
					if(iGridNo != NOWHERE)
					{
						UINT16 usItem = pBullet->fromItem;
						if(usStructureID == INVALID_STRUCTURE_ID)
						{
							for(int i=0; i<MAXITEMS; i++)
								if(Item[i].bloodieditem == pBullet->fromItem)
								{
									usItem = Item[i].uiIndex;// clean the blood from knife, actually this should be done during repair
									break;
								}
						}
						CreateItem(usItem, (pBullet->ubItemStatus>1 ? pBullet->ubItemStatus-Random(2) : pBullet->ubItemStatus), &gTempObject);
						AddItemToPool(iGridNo, &gTempObject, -1, 0, 0, -1);
						NotifySoldiersToLookforItems();
					}
/*
					// OK, have we hit ground?
					if ( usStructureID == INVALID_STRUCTURE_ID )
					{
						// Add item
						CreateItem( usWeaponIndex, bWeaponStatus, &gTempObject );

						AddItemToPool( sGridNo, &gTempObject, -1 , 0, 0, -1 );

						// Make team look for items
						NotifySoldiersToLookforItems( );
					}
*/
					if ( !fHitSameStructureAsBefore )
					{
						PlayJA2Sample( MISS_KNIFE, RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );
					}

					RemoveBullet( iBullet );
					// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - knife attack hit structure") );
					// FreeUpAttacker( (UINT8) ubAttackerID );
				}
		}
	}
	else
	{
		if ( sZPos == 0 )
		{
			PlayJA2Sample( MISS_G2 , RATE_11025, 5, 1, SoundDir( sGridNo ) );
		}
		else
		{
			PlayJA2Sample( MISS_1 + Random(8), RATE_11025, 5, 1, SoundDir( sGridNo ) );
		}
		RemoveBullet( iBullet );
	}

	if ( fDoMissForGun )
	{
		// OK, are we a shotgun, if so , make sounds lower...
		if ( Weapon[ usWeaponIndex ].ubWeaponClass == SHOTGUNCLASS )
		{
			uiMissVolume = LOWVOLUME;
		}

		// Free guy!
		//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure") );
		//FreeUpAttacker( (UINT8) ubAttackerID );


		// PLAY SOUND AND FLING DEBRIS
		// RANDOMIZE SOUND SYSTEM

		// IF WE HIT THE GROUND

		if ( fHitSameStructureAsBefore )
		{
			if ( fStopped )
			{
				RemoveBullet( iBullet );
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit same structure twice") );
				// FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}
		else
		{

			if ( !fStopped || !DoSpecialEffectAmmoMiss( ubAttackerID, sGridNo, sXPos, sYPos, sZPos, FALSE, TRUE, iBullet ) )
			{
				if ( sZPos == 0 )
				{
					PlayJA2Sample( MISS_G2 , RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );
				}
				else
				{
					PlayJA2Sample( MISS_1 + Random(8), RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );
				}

				// Default hit is the ground
				usMissTileIndex = FIRSTMISS1;
				usMissTileType	= FIRSTMISS;

				// Check if we are in water...
				if ( TERRAIN_IS_WATER( gpWorldLevelData[ sGridNo ].ubTerrainID) )
				{
					usMissTileIndex = SECONDMISS1;
					usMissTileType	= SECONDMISS;

					// Add ripple
					memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
					AniParams.sGridNo							= sGridNo;
					AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
					AniParams.usTileType				  = THIRDMISS;
					AniParams.usTileIndex					= THIRDMISS1;
					AniParams.sDelay							= 50;
					AniParams.sStartFrame					= 0;
					AniParams.uiFlags							= ANITILE_FORWARD;

					pNode = CreateAnimationTile( &AniParams );

					// Adjust for absolute positioning
					pNode->pLevelNode->uiFlags |= LEVELNODE_USEABSOLUTEPOS;
					pNode->pLevelNode->sRelativeX	= sXPos;
					pNode->pLevelNode->sRelativeY	= sYPos;
					pNode->pLevelNode->sRelativeZ = sZPos;

				}

				memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
				AniParams.sGridNo							= sGridNo;
				AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
				AniParams.usTileType				  = usMissTileType;
				AniParams.usTileIndex					= usMissTileIndex;
				AniParams.sDelay							= 80;
				AniParams.sStartFrame					= 0;
				if (fStopped)
				{
					AniParams.uiFlags							= ANITILE_FORWARD | ANITILE_RELEASE_ATTACKER_WHEN_DONE;
				}
				else
				{
					AniParams.uiFlags							= ANITILE_FORWARD;
				}
				// Save bullet ID!
				AniParams.uiUserData3					= iBullet;

				pNode = CreateAnimationTile( &AniParams );

				// Set attacker ID
				pNode->usMissAnimationPlayed = usMissTileType;
				pNode->ubAttackerMissed			 = ubAttackerID;
				// Adjust for absolute positioning
				pNode->pLevelNode->uiFlags |= LEVELNODE_USEABSOLUTEPOS;
				pNode->pLevelNode->sRelativeX	= sXPos;
				pNode->pLevelNode->sRelativeY	= sYPos;
				pNode->pLevelNode->sRelativeZ = sZPos;

				// ATE: Show misses...( if our team )
				if ( gGameSettings.fOptions[ TOPTION_SHOW_MISSES ] )
				{
					if ( ubAttackerID != NOBODY )
					{
						if ( MercPtrs[ ubAttackerID ]->bTeam == gbPlayerNum )
						{
							LocateGridNo( sGridNo );
						}
					}
				}
			}

			pBullet->usLastStructureHit = usStructureID;

		}
	}
}

void WindowHit( INT32 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce )
{
	STRUCTURE *			pWallAndWindow;
	DB_STRUCTURE *	pWallAndWindowInDB;
	INT32						sShatterGridNo;
	UINT16					usTileIndex;
	ANITILE *			pNode;
	ANITILE_PARAMS	AniParams;


	// ATE: Make large force always for now ( feel thing )
	fLargeForce = TRUE;

	// we have to do two things here: swap the window structure
	// (right now just using the partner stuff in a chain from
	// intact to cracked to shattered) and display the
	// animation if we've reached shattered

	// find the wall structure, and go one length along the chain
	pWallAndWindow = FindStructureByID( sGridNo, usStructureID );
	if (pWallAndWindow == NULL)
	{
		return;
	}

	pWallAndWindow = SwapStructureForPartner( sGridNo, pWallAndWindow);
	if (pWallAndWindow == NULL)
	{
		return;
	}

	// record window smash
	AddWindowHitToMapTempFile( sGridNo );

	pWallAndWindowInDB = pWallAndWindow->pDBStructureRef->pDBStructure;

	if ( fLargeForce )
	{
		// Force to destruction animation!
		if (pWallAndWindowInDB->bPartnerDelta != NO_PARTNER_STRUCTURE  )
		{
			pWallAndWindow = SwapStructureForPartner( sGridNo, pWallAndWindow);
			if ( pWallAndWindow )
			{
				// record 2nd window smash
				AddWindowHitToMapTempFile( sGridNo );

				pWallAndWindowInDB = pWallAndWindow->pDBStructureRef->pDBStructure;
			}
		}
	}

	SetRenderFlags( RENDER_FLAG_FULL );

	if (pWallAndWindowInDB->ubArmour == MATERIAL_THICKER_METAL_WITH_SCREEN_WINDOWS)
	{
		// don't play any sort of animation or sound
		return;
	}

	if (pWallAndWindowInDB->bPartnerDelta != NO_PARTNER_STRUCTURE  )
	{ // just cracked; don't display the animation
		MakeNoise( NOBODY, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, WINDOW_CRACK_VOLUME, NOISE_BULLET_IMPACT );
		return;
	}
	MakeNoise( NOBODY, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, WINDOW_SMASH_VOLUME, NOISE_BULLET_IMPACT );
	if (pWallAndWindowInDB->ubWallOrientation == INSIDE_TOP_RIGHT || pWallAndWindowInDB->ubWallOrientation == OUTSIDE_TOP_RIGHT)
	{
	/*
		sShatterGridNo = sGridNo + 1;
		// check for wrapping around edge of map
		if (sShatterGridNo % WORLD_COLS == 0)
		{
			// in which case we don't play the animation!
			return;
		}*/
		if (fBlowWindowSouth)
		{
			usTileIndex = WINDOWSHATTER1;
			sShatterGridNo = sGridNo + 1;
		}
		else
		{
			usTileIndex = WINDOWSHATTER11;
			sShatterGridNo = sGridNo;
		}

	}
	else
	{
	/*
		sShatterGridNo = sGridNo + WORLD_COLS;
		// check for wrapping around edge of map
		if (sShatterGridNo % WORLD_ROWS == 0)
		{
			// in which case we don't play the animation!
			return;
		}*/
		if (fBlowWindowSouth)
		{
			usTileIndex = WINDOWSHATTER6;
			sShatterGridNo = sGridNo + WORLD_COLS;
		}
		else
		{
			usTileIndex = WINDOWSHATTER16;
			sShatterGridNo = sGridNo;
		}
	}

	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
	AniParams.sGridNo							= sShatterGridNo;
	AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
	AniParams.usTileType				  = WINDOWSHATTER;
	AniParams.usTileIndex					= usTileIndex;
	AniParams.sDelay							= 50;
	AniParams.sStartFrame					= 0;
	AniParams.uiFlags							= ANITILE_FORWARD;

	pNode = CreateAnimationTile( &AniParams );
	//dddokno{
CompileWorldMovementCosts();
//dddokno}
	PlayJA2Sample( GLASS_SHATTER1 + Random(2), RATE_11025, MIDVOLUME, 1, SoundDir( sGridNo ) );

}


BOOLEAN InRange( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	 INT32								sRange;	
	 UINT16								usInHand;
	 OBJECTTYPE							*pObj;
	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("InRange"));

	 // Flugente: check for underbarrel weapons and use that object if necessary
	 pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[HANDPOS] );

	 usInHand = (*pObj).usItem;
	 INVTYPE* pItemInHand = &Item[ usInHand ];

	 if ( pItemInHand->usItemClass == IC_GUN || pItemInHand->usItemClass == IC_THROWING_KNIFE || (pItemInHand->rocketlauncher && !pItemInHand->singleshotrocketlauncher))
	 {
		 // Determine range
		 sRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

		 if ( pItemInHand->usItemClass == IC_THROWING_KNIFE )
		 {
			 // NB CalcMaxTossRange returns range in tiles, not in world units
		 	 if ( sRange <= CalcMaxTossRange( pSoldier, usInHand, TRUE ) * CELL_X_SIZE )
			 {
				 return( TRUE );
			 }
		 }
		 else
		 {
			 // For given weapon, check range
			 if ( sRange <= GunRange( pObj, pSoldier ) ) // SANDRO - added argument
			 {
					return( TRUE );
			 }
		 }
	 }
	 return( FALSE );
}

UINT32 CalcNewChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos )
{
//////////////////////////////////////////////////////////////////////////////////
// START COMMON PART OF CLEANED UP AND OLD VERSION OF NCTH CALCULATION
// When the old code is removed some of the old variables can be removed too
// because then they are not used anymore.
// iChance, bBandaged, iTraitModifier
//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 4: 
	//
	// The CTH/Firing system has been completely revamped. CTH is now the result of
	// two completely separate calculations. One takes into account skill and conditions,
	// the other relies on gun accuracy.
	// See also UseGun() for further notes.
	//
	//////////////////////////////////////////////////////////////////////////////////

	// Basic defines
	SOLDIERTYPE * pTarget;
	INT32 iChance, iRange, iSightRange; //, minRange;

	UINT16	usInHand;
	OBJECTTYPE * pInHand;
	INT8 bBandaged, iTraitModifier;
	INT16	sDistVis, sDistVisNoScope;
	UINT8 ubTargetID;
//	bool	highPowerScope = false;
//	INT16 sTotalAutofirePenalty = 0;
	bool	fCantSeeTarget = false;
	FLOAT	scopeRangeMod = 0.0f;
	
	// make sure the guy's actually got a weapon in his hand!
	pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);

	// Flugente: check for underbarrel weapons and use that object if necessary
	OBJECTTYPE* pObjAttHand = pSoldier->GetUsedWeapon( &(pSoldier->inv[pSoldier->ubAttackingHand]) );
	UINT16 usItemAttHand    = pSoldier->GetUsedWeaponNumber( &(pSoldier->inv[pSoldier->ubAttackingHand]) );

	usInHand = pSoldier->usAttackingWeapon;
	gCTHDisplay.fMaxAimReached = FALSE;

	// calculate actual range (in units, 10 units = 1 tile)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );
	FLOAT dDeltaX = (FLOAT)(CenterX( pSoldier->sGridNo ) - CenterX( sGridNo ));
	FLOAT dDeltaY = (FLOAT)(CenterY( pSoldier->sGridNo ) - CenterY( sGridNo ));
	FLOAT d2DDistance = sqrt((dDeltaX*dDeltaX)+(dDeltaY*dDeltaY));

	// Find a target in the tile
	ubTargetID = WhoIsThere2( sGridNo, pSoldier->bTargetLevel ); // Target ubID
	pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel ); // Target Pointer

	UINT8 stance = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pSoldier->IsWeaponMounted() )
		stance = ANIM_PRONE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// CALCULATE LINE OF SIGHT
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//CHRISL: We have a new way to do this using a global variable
	if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) ) // ignore scopes when firing from hip or on fast shot with pistol
		gbForceWeaponReady = true;

	sDistVis = pSoldier->GetMaxDistanceVisible(sGridNo, pSoldier->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;

	iSightRange = 0;

	if (ubTargetID != NOBODY && pSoldier->aiData.bOppList[ubTargetID] == SEEN_CURRENTLY || gbPublicOpplist[pSoldier->bTeam][ubTargetID] == SEEN_CURRENTLY)
		iSightRange = SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pSoldier->bAimShotLocation, false );
	if (iSightRange == 0) {	// didn't do a bodypart-based test or can't see specific body part aimed at
		iSightRange = SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pSoldier->bTargetLevel, pSoldier->bTargetCubeLevel, TRUE, NO_DISTANCE_LIMIT, false );
	}
	if (iSightRange == 0) {	// Can't see the target but we still need to know what the sight range would be if we could so we can deal with cover penalties
		iSightRange = SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pSoldier->bAimShotLocation, false, true );
		fCantSeeTarget = true;
	}

	gbForceWeaponReady = false;

	//CHRIS: This next value needs to be determined with no vision modifiers from our weapon
	gbForceWeaponNotReady = true;
	sDistVisNoScope = pSoldier->GetMaxDistanceVisible(sGridNo, pSoldier->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;
	gbForceWeaponNotReady = false;

	// Flugente: blind soldiers have sDistVisNoScope = 0...
	if ( sDistVisNoScope )
		scopeRangeMod = (float)sDistVis / (float)sDistVisNoScope;	// percentage DistVis has been enhanced due to an attached scope

	if ( scopeRangeMod )
		iSightRange = (INT32)(iSightRange / scopeRangeMod);

	if(iSightRange > 0){
		//CHRISL: The LOS system, which determines whether to display an enemy unit, does not factor in the AimBonus tag during it's calculations.  So having
		//	the CTH system use that tag to adjust iSightRange for AimBonus applied from armor might not be the best option.  Especially as it can sometimes
		//	result in 0% CTH when everything looks like you could actually hit the target.  Let's try applying this penalty to CTH at the same point we would
		//	apply the "invisible target" penalty.
		//iSightRange -= GetGearAimBonus ( pSoldier, iSightRange, ubAimTime ) * iSightRange / 100;
		if ( gGameOptions.fNewTraitSystem ) {
			if ( HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) ) {
				iSightRange -= ((gSkillTraitValues.ubSNEffRangeToTargetReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )) * iSightRange) /100;
			}
		}
		else if ( HAS_SKILL_TRAIT( pSoldier, PROF_SNIPER_OT ) ) {
			iSightRange -= ((gbSkillTraitBonus[ PROF_SNIPER_OT ] * NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )) * iSightRange) /100;
		}
		if (iSightRange < 1) {
			iSightRange = 1;
		}
	}
	// CHRISL: There are conditions where iSightRange can still return 0.  If that happens, the result is that "impossible" shots are actually easier then
	//	simply "really hard" shots.  As a result, if we can't see the target and we have a 0 sight range, we need to create an iSightRange based on iRange
	if(iSightRange == 0)
		iSightRange = __max((INT16)(sDistVis*1.5), iRange*2);
	if(iSightRange > sDistVis)
		fCantSeeTarget = true;
//////////////////////////////////////////////////////////////////////////////////
// END COMMON PART OF CLEANED UP AND OLD VERSION OF NCTH CALCULATION
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// START CLEANED UP VERSION OF NCTH CALCULATION
//////////////////////////////////////////////////////////////////////////////////
// silversurfer: This is a cleaned up version of the NCTH calculation and hopefully easier to read.
// The if clause and the rest of the old code below its else can be removed if the new version is accepted.
// The "if (gGameExternalOptions.fUseNewCTHCalculation)" is only there for easy switching in Ja2_Options.ini.
//////////////////////////////////////////////////////////////////////////////////
if (gGameExternalOptions.fUseNewCTHCalculation)
{
	FLOAT fBaseChance = 0, fBaseModifier = 0, fAimChance = 0, fAimModifier = 0, fFinalChance = 0;

	//////////////////////////////////////////////////////////////////////////////////
	// First step: Calculate a base CTH value
	//
	// This "Base" CTH is a percentage value describing how well our gun's muzzle is aimed
	// when not spending any extra effort on aiming. In other words, it's the most basic state,
	// where the shooter only brings up the gun into the general direction of the target, and
	// pulls the trigger.
	//
	// This is calculated first by factoring in skills, and later by adding up various conditional
	// factors into a percentage which is then applied to those skills.
	//
	// First calculate the basic value for BASE CTH by factoring in all the character's skills.
	// The most important skill here is EXPERIENCE.
	fBaseChance = CalcNewChanceToHitBaseAttributeBonus(pSoldier);

	// this shooter will never hit anything
	if ( fBaseChance <= gGameExternalOptions.ubMinimumCTH )
		return gGameExternalOptions.ubMinimumCTH;

	// Add a flat Base bonus from the item and its attachments.
	INT32 imoda = GetFlatBaseModifier( pInHand, stance );
	INT32 imodb = GetFlatBaseModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
	fBaseChance += (FLOAT)((gGameExternalOptions.ubProneModifierPercentage * imoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * imodb)/100); 

	// get bonus from effects lasting on the shooter (morale, injury, shock etc.)
	fBaseModifier += CalcNewChanceToHitBaseEffectBonus(pSoldier);

	// Calculate how easy it is to handle the weapon
	FLOAT fGunDifficulty = (FLOAT)(( Weapon[ usInHand ].ubHandling * (100 - GetPercentReadyTimeAPReduction(pInHand) )) / 100);
	fGunDifficulty *= (FLOAT)(100 / APBPConstants[AP_MAXIMUM]); // Adjust for 100AP/25AP
	FLOAT fGunBaseDifficulty = fGunDifficulty;
	FLOAT fGunAimDifficulty = fGunDifficulty;
	
	// get bonus from weapon handling
	fBaseModifier += CalcNewChanceToHitBaseWeaponBonus(pSoldier, sGridNo, ubAimTime, fGunBaseDifficulty, stance);

	// get special bonus (enemy/militia bonus, special characters, game difficulty bonus etc.)
	fBaseModifier += CalcNewChanceToHitBaseSpecialBonus(pSoldier);

	// get target specific bonus
	fBaseModifier += CalcNewChanceToHitBaseTargetBonus(pSoldier, pTarget, sGridNo, iRange, ubAimPos, fCantSeeTarget);

	//CHRISL: Applying the Gear AimBonus (penalty) here, and directly to fBaseModifier as a flat penalty, instead of altering iSightRange above.  For now
	//	I'm just applying this to the BaseModifier which means that aiming can overcome the Gear AimBonus (penalty).
	fBaseModifier += GetGearAimBonus ( pSoldier, iSightRange, ubAimTime );

	////////////////////////////////////
	// Finish BASE CTH calculation:
	fBaseChance = (fBaseChance * (100+fBaseModifier) / 100);
	fBaseChance = __max( fBaseChance, 0 );
	fBaseChance = __min( fBaseChance, 100 );

	fFinalChance = fBaseChance;
	//////////////////////////////////////////////////////////////////////////////////
	// Second step: Calculate bonuses from aiming
	//
	// In this step, we figure out how much CTH we get from aiming. Naturally, we
	// need extra aiming (bAimTime) to be applied.
	//
	// CTH is capped by a value determined by the shooter's skills. The better
	// your skills, the higher CTH can go. A steady hand (DEX) and training
	// (MRK) are paramount here.
	// After that, the cap is raised or lowered by other factors, like fatigue
	// or the size of the gun.
	//
	// Finally, we determine how much each Aim Click will give us. Returns are
	// diminishing, so the more clicks we put in, the less CTH for each additional
	// click. However, the last few points can be absolutely crucial for long-range
	// shooting.
	//
	// Scope magnification can be important here, but only if the target is closer
	// than the scope's minimum aiming range. If it is closer, the shooter begins
	// to suffer aiming penalties which can render aiming almost useless.

	if (ubAimTime > 0)
	{
		// Are we using a scope? If so, what's the range factor?
		FLOAT fScopeMagFactor, fRangeModifier;
		//CHRISL: This does make sense but it effectively makes high powered scopes worthless if a target is actually visible.  As an example, a Battle Scope
		//	is going to have a iScopeMagFactor of 7.  With a "NORMAL_SHOOTING_DISTANCE" also of 7, we're going to end up with uiBestScopeRange of 49.  That's
		//	effectilvey saying that any target within 490m is "too close" for the scope to be effective.  That by itself isn't realistic.  But in JA2 it's also
		//	a big drawback.  Consider that normally (excluding terrain, lighting and environment) you can see 26 tiles.  The Battle Scope increases that by 50%
		//	in bright light, meaning you can see up to 39 tiles away.  A target standing 49 tiles is considered "invisible" to you because the target is beyond
		//	your 39 tile vision range.  Sniper scopes (10 iScopeMagFactor) are even worse (uiBestScopeRange = 70 but visible range limited to 49 tiles).  So the
		//	only way to get the full benefit of a higher powered scope is to have a spotter AND to suffer the various "TARGET_INVISIBLE" penalties.  This makes
		//	no sense.  Admittedly, higher powered scopes should be less efficienct at "close range" but is 490m (700m for sniper scope) really "close range"?
		//	Also, we can't simply change NORMAL_SHOOTING_DISTANCE because this value also effects shooting without a scope.  We don't want ot make it harder for
		//	"scopeless" shooting.  So to try and counter some of this, while still showing that scopes are less effecient at "close range", I'm going to cut the
		//	uiBestScopeRange value in half.  This should allow a Battle Scope to reach full effeciency at 24 tiles and a Sniper scope will be fully effecient at
		//	35 tiles.  ACOG becomes fully effecient at 14 tiles and 2x is fully effeciency at 7 tiles (compared to 28 and 14 respectively).  This does mean that a
		//	2x scope reaches full effeciency at the same point as "scopeless" shooting, but I don't think this will be a serious problem.

		if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) ) // ignore scopes when firing from hip/fast shot from pistol
		{
			fScopeMagFactor = GetBestScopeMagnificationFactor( pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand]), d2DDistance );
			fRangeModifier = GetScopeRangeMultiplier(pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand]), (FLOAT)iRange);
		}
		else
		{
			fScopeMagFactor = 1.0f;
			// we are shooting from hip so SCOPE_RANGE_MULTIPLIER cannot be applied
			fRangeModifier = 1.0f;
		}

		UINT32 uiBestScopeRange = (UINT32)(fScopeMagFactor * gGameCTHConstants.NORMAL_SHOOTING_DISTANCE * fRangeModifier);

		// get attribute based aiming value. 
		// This is also the CTH cap, the absolute maximum CTH a shooter can get.
		fAimChance = CalcNewChanceToHitAimAttributeBonus(pSoldier);		

		// get direct AimChance bonus for traits (throwing, sniper etc.)
		FLOAT fDifference = 99 - fAimChance;
		fAimChance += CalcNewChanceToHitAimTraitBonus(pSoldier, fAimChance, fDifference, sGridNo, ubAimTime, fScopeMagFactor, uiBestScopeRange);

		// Add percent-based modifier from the gun and its attachments
		FLOAT moda = (FLOAT)(fAimChance * GetPercentCapModifier( pInHand, stance ) / 100);
		FLOAT modb = (FLOAT)(fAimChance * GetPercentCapModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) / 100);
		fAimChance += (FLOAT)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100);

		// get aimbonus from effects lasting on the shooter
		fAimModifier += CalcNewChanceToHitAimEffectBonus(pSoldier);

		// get aimbonus from weapon handling
		fAimModifier += CalcNewChanceToHitAimWeaponBonus(pSoldier, sGridNo, ubAimTime, fGunAimDifficulty, stance);

		// get special aimbonus (game difficulty etc.)
		fAimModifier += CalcNewChanceToHitAimSpecialBonus(pSoldier);

		// apply bonus from traits
		// Flugente: moved trait modifiers into a member function
			UINT8 targetprofile = NOBODY;
			if ( pTarget && pTarget->ubProfile != NOBODY )
				targetprofile = pTarget->ubProfile;

			fAimModifier += pSoldier->GetTraitCTHModifier( usInHand, ubAimTime, targetprofile );

		// Flugente: backgrounds
		if ( pTarget && pTarget->bTeam == CREATURE_TEAM )
			fAimModifier += pSoldier->GetBackgroundValue(BG_PERC_CTH_CREATURE);

		// get aimbonus from target
		fAimModifier += CalcNewChanceToHitAimTargetBonus(pSoldier, pTarget, sGridNo, iRange, ubAimPos, fCantSeeTarget);

		// silversurfer: this doesn't make sense. We always apply a penalty when we can see the target?
		// invisible targets are already taken into account one step above in aimbonus from target
		// VISIBILITY
/*		if (iRange > 0 && iSightRange > iRange)
		{
			FLOAT fTempPenalty = (FLOAT)((FLOAT)iSightRange / (FLOAT)iRange);
			fTempPenalty = (FLOAT)(100 / fTempPenalty);
			fAimModifier += ((100-fTempPenalty) * gGameCTHConstants.AIM_VISIBILITY)/100;
			fAimModifier = __max( gGameCTHConstants.AIM_TARGET_INVISIBLE, fAimModifier );
		}*/

		// factor in scopes under their range
		if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
		{
			if (fScopeMagFactor > 1.0 && iRange < (INT32)uiBestScopeRange)
			{
				// Calculate how much penalty this scope should give at this range
				FLOAT dScopePenaltyRatio = ((FLOAT)uiBestScopeRange / (FLOAT)iRange);
				FLOAT iScopePenalty = (FLOAT)(dScopePenaltyRatio * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE * (fScopeMagFactor /2));
				fAimModifier += iScopePenalty;
			}
			else if (fScopeMagFactor == 1.0f && GetHighestScopeMagnificationFactor( &(pSoldier->inv[pSoldier->ubAttackingHand]) ) > 1.0f )
			{
				// Not using a scope, but it's still there. Give half the penalty based on the size of the scope.
				FLOAT iScopePenalty = (FLOAT)(((GetHighestScopeMagnificationFactor( &(pSoldier->inv[pSoldier->ubAttackingHand]) )/2) * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE)/2);
				fAimModifier += iScopePenalty;
			}
		}

		// Make sure cap is within limits
		fAimChance = __max(fAimChance, __max(0,(UINT32)fBaseChance));
		fAimChance = __min(fAimChance, gGameExternalOptions.ubMaximumCTH);

		// Now figure out the distance between the Base CTH and the CTH Cap. This is the distance we'll potentially
		// cover when applying the maximum number of aiming clicks for this gun.
		FLOAT fMaxAimBonus = (FLOAT)(fAimChance - fBaseChance);

		fMaxAimBonus = (FLOAT)((fMaxAimBonus * (100+fAimModifier)) / 100);
		fMaxAimBonus = __max(0, fMaxAimBonus); // can't get less than 0 points for aiming...
		
		// Now let's find out how many CTH points we get per aiming level.
		UINT8 ubAllowedAimingLevels = AllowedAimingLevels( pSoldier, sGridNo );
		if(ubAimTime == ubAllowedAimingLevels)
			gCTHDisplay.fMaxAimReached = TRUE;
		FLOAT dAimFractionsDivisor = 0.0;
		for (UINT8 cnt = 0; cnt < ubAllowedAimingLevels; cnt++)
		{
			dAimFractionsDivisor += (cnt+1);
		}			

		FLOAT fAimPoints = 0;
		FLOAT fAimPointFraction = (FLOAT)((FLOAT)fMaxAimBonus / dAimFractionsDivisor);
		for (UINT8 x = 0; x < ubAimTime; x++)
		{
			fAimPoints += fAimPointFraction * (ubAllowedAimingLevels-x);
			// Add Flat Modifier from the weapon and its attachments
			INT32 moda = GetFlatAimModifier( pInHand, stance );
			INT32 modb = GetFlatAimModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
			fAimPoints += (FLOAT)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 
		}

		// Finally, add the appropriate number of CTH points to our chance-to-hit, and limit it into good values.
		fFinalChance = __max(fBaseChance + (INT32)fAimPoints, fBaseChance);
		fFinalChance = __min(fFinalChance, (INT32)fAimChance);
	}
		
	// Impose global limits.	
	// Flugente: backgrounds
	fFinalChance = __min(fFinalChance, min(100, gGameExternalOptions.ubMaximumCTH + (UINT8)(pSoldier->GetBackgroundValue(BG_PERC_CTH_MAX))) );
	fFinalChance = __max(fFinalChance, gGameExternalOptions.ubMinimumCTH);
	

	return ((INT32)fFinalChance);
}
else
{
//////////////////////////////////////////////////////////////////////////////////
// END CLEANED UP VERSION OF NCTH CALCULATION
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// START OLD VERSION OF NCTH CALCULATION
//////////////////////////////////////////////////////////////////////////////////

	// Calculate how easy it is to handle this gun.
	FLOAT iGunDifficulty = (FLOAT)(( Weapon[ usInHand ].ubHandling * (100 - GetPercentReadyTimeAPReduction(pInHand) )) / 100);
	iGunDifficulty *= (FLOAT)(100 / APBPConstants[AP_MAXIMUM]); // Adjust for 100AP/25AP
	FLOAT iGunBaseDifficulty = iGunDifficulty;
	FLOAT iGunAimDifficulty = iGunDifficulty;

	//////////////////////////////////////////////////////////////////////////////////
	// First step: Calculate a base CTH value
	//
	// This "Base" CTH is a percentage value describing how well our gun's muzzle is aimed
	// when not spending any extra effort on aiming. In other words, it's the most basic state,
	// where the shooter only brings up the gun into the general direction of the target, and
	// pulls the trigger.
	//
	// This is calculated first by factoring in skills, and later by adding up various conditional
	// factors into a percentage which is then applied to those skills.
	//
	// The most important skill here is EXPERIENCE.

	FLOAT iBaseModifier = 0;

	INT8 bExperience = EffectiveExpLevel( pSoldier );
	INT8 bMarksmanship = EffectiveMarksmanship( pSoldier );
	INT16 bDexterity = EffectiveDexterity( pSoldier, FALSE );
	INT16 bWisdom = EffectiveWisdom( pSoldier );

	if ( bMarksmanship == 0 || bDexterity == 0 )
	{
		return( gGameExternalOptions.ubMinimumCTH );
	}

	// Calculate the basic value for BASE CTH by factoring in all the character's skills.
	FLOAT iCombinedSkill = (FLOAT)(gGameCTHConstants.BASE_EXP*(bExperience*10));
	iCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_MARKS * bMarksmanship);
	iCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_WIS * bWisdom);
	iCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_DEX * bDexterity);

	// Divide by the total coefficient value.
    iCombinedSkill = (FLOAT)(iCombinedSkill / 
			(gGameCTHConstants.BASE_EXP+
			gGameCTHConstants.BASE_MARKS+
			gGameCTHConstants.BASE_WIS+
			gGameCTHConstants.BASE_DEX));

	// At this point we have a value of 1 to 100. Divide by 3.
	iCombinedSkill /= 3;
	
	// iCombinedSkill is now our basic CTH. Its range is 0 to 33.
	iChance = (INT32)iCombinedSkill;

	// Add a flat Base bonus from the item and its attachments.
	INT32 moda = GetFlatBaseModifier( pInHand, stance );
	INT32 modb = GetFlatBaseModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
	iChance += (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 
	
	// We now begin adding up factors that may increase or decrease Base CTH. They are pooled together to form a percentage
	// value.
	
	// FIRING FROM ALTERNATIVE WEAPON HOLDING (faster shot, less accuracy)
	if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
	{
		iBaseModifier -= gGameExternalOptions.ubAltWeaponHoldingCtHPenaly;
	}

	// MORALE
	INT8 iCurrentMorale = GetMoraleModifier(pSoldier);
	if (iCurrentMorale > 0)
	{
		iBaseModifier += (FLOAT)(iCurrentMorale * gGameCTHConstants.BASE_HIGH_MORALE);
	}
	else if (iCurrentMorale < 0)
	{
		iBaseModifier += (FLOAT)(iCurrentMorale * gGameCTHConstants.BASE_LOW_MORALE);
	}

	// Flugente: moved trait modifiers into a member function
	UINT8 targetprofile = NOBODY;
	if ( pTarget && pTarget->ubProfile != NOBODY )
		targetprofile = pTarget->ubProfile;

	iTraitModifier = pSoldier->GetTraitCTHModifier( usInHand, ubAimTime, targetprofile );

	// Flugente: backgrounds
	if ( pTarget && pTarget->bTeam == CREATURE_TEAM )
		iTraitModifier += pSoldier->GetBackgroundValue(BG_PERC_CTH_CREATURE);

	// SHOOTING AT SAME TARGET AGAIN
	if (sGridNo == pSoldier->sLastTarget )
		iBaseModifier += (FLOAT)gGameCTHConstants.BASE_SAME_TARGET;

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Bonus CtH for Militia
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sGreenMilitiaCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sRegularMilitiaCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sVeteranMilitiaCtHBonusPercent;
	// bonus for enemy
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sEnemyAdminCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sEnemyRegularCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		iBaseModifier += gGameExternalOptions.sEnemyEliteCtHBonusPercent;
	// SANDRO - option to make special NPCs stronger - chance to hit
	if (gGameExternalOptions.usSpecialNPCStronger > 0)
	{
		switch( pSoldier->ubProfile )
		{
			case CARMEN:
			case QUEEN:
			case JOE:
			case ANNIE:
			case CHRIS:
			case KINGPIN:
			case TIFFANY:
			case T_REX:
			case DRUGGIST:
			case GENERAL:
			case JIM:
			case JACK:
			case OLAF:
			case RAY:
			case OLGA:
			case TYRONE:
			case MIKE:
				iBaseModifier += gGameExternalOptions.usSpecialNPCStronger;
				break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////
	// Gun Difficulty Modifiers
		
	// FIRING 1-HANDED WEAPONS
	if ( !(Item[ usInHand ].twohanded ) )	//JMich todo: underbarrel
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() != false)
		{
			if ( pSoldier->IsValidSecondHandShot( ) )
			{
				// Penalty for shooting two pistols. Ambidextrous trait halves this, once per skill level.
				// SANDRO - Ambidextrous trait has only one level
				FLOAT iTempPenalty = (gGameCTHConstants.BASE_TWO_GUNS * iGunBaseDifficulty) - iGunBaseDifficulty;
				//INT8	traitLoop = (gGameOptions.fNewTraitSystem?NUM_SKILL_TRAITS( pSoldier, AMBIDEXTROUS_NT ):NUM_SKILL_TRAITS( pSoldier, AMBIDEXT_OT ));
				if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
				{
					iTempPenalty = iTempPenalty * (100 - gSkillTraitValues.ubAMPenaltyDoubleReduction) / 100;
				}
				else if (!gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
				{
					iTempPenalty = 0;
				}
				//for (INT32 x = 0; x < traitLoop; x++)
				//{
				//	iTempPenalty /= 2;
				//}
				
				iGunBaseDifficulty += iTempPenalty;
			}
			else
			{
				// Penalty for shooting a pistol with just one hand
				iGunBaseDifficulty *= gGameCTHConstants.BASE_ONE_HANDED;
			}
		}
	}

	// HEAVY WEAPON
	if (Item[usInHand].rocketlauncher && !(gGameOptions.fNewTraitSystem) ) 
	{
		// Penalty for shooting heavy weapons (launchers). Heavy Weapons skill halves this, once per skill level.
		FLOAT iTempPenalty = (gGameCTHConstants.BASE_HEAVY_WEAPON * iGunBaseDifficulty) - iGunBaseDifficulty;

		for (INT32 x=0; x < NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT ); x++)
		{
			iTempPenalty /= 2;
		}

		iGunBaseDifficulty += iTempPenalty;
	}

	// STANCE
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
		case ANIM_CROUCH:
			iGunBaseDifficulty *= gGameCTHConstants.BASE_CROUCHING_STANCE;
			break;
		case ANIM_PRONE:
			iGunBaseDifficulty *= gGameCTHConstants.BASE_PRONE_STANCE;
			break;
	}

	// Percent based modifier from the gun and its attachments
	FLOAT umoda = (iGunBaseDifficulty * GetPercentHandlingModifier( pInHand, stance )) / 100;
	FLOAT umodb = (iGunBaseDifficulty * GetPercentHandlingModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
	iGunBaseDifficulty += ((gGameExternalOptions.ubProneModifierPercentage * umoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * umodb)/100); 

	// End gun difficulty modifiers
	/////////////////////////////////////////////

	// Now apply Gun Difficulty to the Base Modifier.
	iBaseModifier -= iGunBaseDifficulty * gGameCTHConstants.BASE_DRAW_COST;

	// INJURY
	if (pSoldier->stats.bLife < pSoldier->stats.bLifeMax)
	{
		FLOAT iTempPenalty = gGameCTHConstants.BASE_INJURY;

		// Bleeding damage is used as a percentage off the Max Life, giving a penalty of up to BASE_INJURY.
		iBaseModifier += (pSoldier->bBleeding * iTempPenalty) / pSoldier->stats.bLifeMax;

		// Bandage damage is used similarly, but is only 1/3 as bad.
		bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
		iBaseModifier += ((bBandaged * iTempPenalty) / pSoldier->stats.bLifeMax) / 3;
	}

	// FATIGUE
	iBaseModifier += (gGameCTHConstants.BASE_FATIGUE * (100 - pSoldier->bBreath)) / 100;

	// DRUNKNESS
	INT8 iDrunkness = GetDrunkLevel(pSoldier);
	if (iDrunkness > 0)
	{
		iDrunkness -= 1;
		iBaseModifier += gGameCTHConstants.BASE_DRUNK[iDrunkness];
	}

	// GASSED
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		iBaseModifier += gGameCTHConstants.BASE_GASSED;
	}

	// BEING BANDAGED
	if (pSoldier->ubServiceCount > 0)
	{
		iBaseModifier += gGameCTHConstants.BASE_BEING_BANDAGED;
	}

	// SHOCK (SHOOTER)
	if (pSoldier->aiData.bShock)
	{
		FLOAT iTempPenalty = gGameCTHConstants.BASE_SHOCK;
		INT32 iShockPercentage = (pSoldier->aiData.bShock * 100) / gGameExternalOptions.ubMaxSuppressionShock;

		iBaseModifier += (iTempPenalty * iShockPercentage) / 100;
	}

	/////////////
	// Target-specific tests

	INT32 iHeightDifference = 0;
	if ( pTarget == NULL )
	{
		// Shooting to roof.
		if ( pSoldier->bTargetLevel > pSoldier->pathing.bLevel )
		{
			iHeightDifference = 3 * pSoldier->bTargetLevel;
		}
	}
	else
	{
		// HEIGHT DIFFERENCE

		UINT32 uiShooterHeight = 0;
		UINT32 uiTargetHeight = 0;

		if ( pSoldier->pathing.bLevel > 0 )
		{
			uiShooterHeight += 3 * pSoldier->pathing.bLevel;
		}
		
		switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				uiShooterHeight += 2;
				break;
			case ANIM_CROUCH:
				uiShooterHeight += 1;
				break;
		}

		if (pTarget->pathing.bLevel > 0)
		{
			uiTargetHeight += 3 * pTarget->pathing.bLevel;
		}

		switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 2;
						break;
					case AIM_SHOT_TORSO:
					case AIM_SHOT_RANDOM:
					case AIM_SHOT_GLAND:
						uiTargetHeight += 1;
						break;
				}
				break;
			case ANIM_CROUCH:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 1;
						break;
				}
				break;
		}

		iHeightDifference = uiShooterHeight - uiTargetHeight;
		if (iHeightDifference < 0)
		{
			iHeightDifference *= -1;
		}
		else
		{
			iHeightDifference = 0;
		}

		// AGILITY OR EXPERIENCE
		FLOAT iTempPenalty = (FLOAT)__max((pTarget->stats.bExpLevel*10), pTarget->stats.bAgility);

		iBaseModifier += (iTempPenalty * gGameCTHConstants.BASE_AGILE_TARGET) / 100;
	}

	// Height difference is mitigated by range. A LONGER range reduces this penalty!
	if (iRange > 0 && iHeightDifference > 0)
	{
		FLOAT iTempPenalty = gGameCTHConstants.BASE_SHOOTING_UPWARDS * iHeightDifference;
		iTempPenalty /= iRange;

		iBaseModifier += iTempPenalty;
	}

	// INVIS TARGET
	if (fCantSeeTarget)
	{
		iBaseModifier += gGameCTHConstants.BASE_TARGET_INVISIBLE;
	}
	//CHRISL: Applying the Gear AimBonus (penalty) here, and directly to iBaseModifier as a flat penalty, instead of altering iSightRange above.  For now
	//	I'm just applying this to the BaseModifier which means that aiming can overcome the Gear AimBonus (penalty).
	iBaseModifier += GetGearAimBonus ( pSoldier, iSightRange, ubAimTime );

	// GAME DIFFICULTY
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) && (pSoldier->bSide != gbPlayerNum) )
	{
		iBaseModifier += gGameCTHConstants.BASE_DIFFICULTY[gGameOptions.ubDifficultyLevel];
	}

	// Percentage based-modifier from the weapon and its attachments
	umoda = (iGunBaseDifficulty * GetPercentBaseModifier( pInHand, stance )) / 100;
	umodb = (iGunBaseDifficulty * GetPercentBaseModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
	iBaseModifier += ((gGameExternalOptions.ubProneModifierPercentage * umoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * umodb)/100); 
	

	////////////////////////////////////
	// Finish BASE CTH calculation:



	iChance = (INT32)(iChance * (100+iBaseModifier)) / 100;
	iChance = __max( iChance, 0 );
	iChance = __min( iChance, 100 );


	//////////////////////////////////////////////////////////////////////////////////
	// Second step: Calculate bonuses from aiming
	//
	// In this step, we figure out how much CTH we get from aiming. Naturally, we
	// need extra aiming (bAimTime) to be applied.
	//
	// CTH is capped by a value determined by the shooter's skills. The better
	// your skills, the higher CTH can go. A steady hand (DEX) and training
	// (MRK) are paramount here.
	// After that, the cap is raised or lowered by other factors, like fatigue
	// or the size of the gun.
	//
	// Finally, we determine how much each Aim Click will give us. Returns are
	// diminishing, so the more clicks we put in, the less CTH for each additional
	// click. However, the last few points can be absolutely crucial for long-range
	// shooting.
	//
	// Scope magnification can be important here, but only if the target is closer
	// than the scope's minimum aiming range. If it is closer, the shooter begins
	// to suffer aiming penalties which can render aiming almost useless.

	if (ubAimTime > 0)
	{

		// Are we using a scope? If so, what's the range factor?
		FLOAT iScopeMagFactor, rangeModifier;
		//CHRISL: This does make sense but it effectively makes high powered scopes worthless if a target is actually visible.  As an example, a Battle Scope
		//	is going to have a iScopeMagFactor of 7.  With a "NORMAL_SHOOTING_DISTANCE" also of 7, we're going to end up with uiBestScopeRange of 49.  That's
		//	effectilvey saying that any target within 490m is "too close" for the scope to be effective.  That by itself isn't realistic.  But in JA2 it's also
		//	a big drawback.  Consider that normally (excluding terrain, lighting and environment) you can see 26 tiles.  The Battle Scope increases that by 50%
		//	in bright light, meaning you can see up to 39 tiles away.  A target standing 49 tiles is considered "invisible" to you because the target is beyond
		//	your 39 tile vision range.  Sniper scopes (10 iScopeMagFactor) are even worse (uiBestScopeRange = 70 but visible range limited to 49 tiles).  So the
		//	only way to get the full benefit of a higher powered scope is to have a spotter AND to suffer the various "TARGET_INVISIBLE" penalties.  This makes
		//	no sense.  Admittedly, higher powered scopes should be less efficienct at "close range" but is 490m (700m for sniper scope) really "close range"?
		//	Also, we can't simply change NORMAL_SHOOTING_DISTANCE because this value also effects shooting without a scope.  We don't want ot make it harder for
		//	"scopeless" shooting.  So to try and counter some of this, while still showing that scopes are less effecient at "close range", I'm going to cut the
		//	uiBestScopeRange value in half.  This should allow a Battle Scope to reach full effeciency at 24 tiles and a Sniper scope will be fully effecient at
		//	35 tiles.  ACOG becomes fully effecient at 14 tiles and 2x is fully effeciency at 7 tiles (compared to 28 and 14 respectively).  This does mean that a
		//	2x scope reaches full effeciency at the same point as "scopeless" shooting, but I don't think this will be a serious problem.

		if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) ) // ignore scopes when firing from hip/fast shot from pistol
		{
			iScopeMagFactor = GetBestScopeMagnificationFactor( pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand]), d2DDistance );
			rangeModifier = GetScopeRangeMultiplier(pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand]), (FLOAT)iRange);
		}
		else
		{
			iScopeMagFactor = 1.0f;
			rangeModifier = gGameCTHConstants.SCOPE_RANGE_MULTIPLIER;
		}
		
		UINT32 uiBestScopeRange = (UINT32)(iScopeMagFactor * gGameCTHConstants.NORMAL_SHOOTING_DISTANCE * rangeModifier);

		FLOAT iAimModifier = 0;

		// "AIMING" FROM ALTERNATIVE WEAPON HOLDING (faster shots, no scopes, less accuracy)
		if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ))
		{
			//due to the way aiming levels are handled in NCTH, this penalty is increased here by 1/3 (it is harmonized by reduced aiming clicks)
			iAimModifier -= gGameExternalOptions.ubAltWeaponHoldingAimingPenaly * 4 / 3;
		}

		// WEAPON CONDITION
		if ( (*pInHand)[0]->data.objectStatus < 50 )
		{
			iAimModifier += gGameCTHConstants.AIM_GUN_CONDITION * (50 - (*pInHand)[0]->data.objectStatus);
		}

		// MORALE
		INT32 iTempModifier = GetMoraleModifier( pSoldier );
		if ( iTempModifier > 0 )
		{
			iAimModifier += gGameCTHConstants.AIM_HIGH_MORALE * iTempModifier;
		}
		else if ( iTempModifier < 0 )
		{
			iAimModifier += gGameCTHConstants.AIM_LOW_MORALE * iTempModifier;
		}

		if(gGameOptions.fNewTraitSystem)
		{
			//Give 60% the normal stomp modifier as the aim mod
			iAimModifier += (FLOAT)iTraitModifier * 0.6f;
		}
		//else
		//{
			// Flugente: drugs can temporarily cause a merc to go psycho
			if ( pSoldier->ubProfile != NO_PROFILE && (gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO || MercUnderTheInfluence(pSoldier, DRUG_TYPE_PSYCHO) ) )
			{
				iAimModifier += gGameCTHConstants.AIM_PSYCHO;
			}
		//}

		//////////////////////////////////////////
		// Gun Handling modifiers

		switch (gAnimControl[ pSoldier->usAnimState ].ubEndHeight)
		{
			case ANIM_STAND:
				iGunAimDifficulty *= gGameCTHConstants.AIM_STANDING_STANCE;
				break;

			case ANIM_CROUCH:
				iGunAimDifficulty *= gGameCTHConstants.AIM_CROUCHING_STANCE;
				break;

			case ANIM_PRONE:
				iGunAimDifficulty *= gGameCTHConstants.AIM_PRONE_STANCE;
				break;
		}

		// FIRING 1-HANDED WEAPONS
		if ( !(Item[ usInHand ].twohanded ) ) //JMich Todo: fix for UnderBarrel firing
		{
			if (pSoldier->inv[SECONDHANDPOS].exists() != false)
			{
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					// Penalty for shooting two pistols. Ambidextrous trait halves this, once per skill level.
					// SANDRO - Ambidextrous trait has only one level
					FLOAT iTempPenalty = (gGameCTHConstants.AIM_TWO_GUNS * iGunAimDifficulty) - iGunAimDifficulty;
					//INT8	traitLoop = (gGameOptions.fNewTraitSystem?NUM_SKILL_TRAITS( pSoldier, AMBIDEXTROUS_NT ):NUM_SKILL_TRAITS( pSoldier, AMBIDEXT_OT ));
					if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
					{
						iTempPenalty = iTempPenalty * (100 - gSkillTraitValues.ubAMPenaltyDoubleReduction) / 100;
					}
					else if (!gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
					{
						iTempPenalty = 0;
					}
					//for (INT32 x = 0; x < traitLoop; x++)
					//{
					//	iTempPenalty /= 2;
					//}
					
					iGunAimDifficulty += iTempPenalty;
				}
				else
				{
					// Penalty for shooting a pistol with just one hand
					iGunAimDifficulty *= gGameCTHConstants.AIM_ONE_HANDED;
				}
			}
		}

		// HEAVY WEAPON
		if (Item[usInHand].rocketlauncher && !(gGameOptions.fNewTraitSystem) ) 
		{
			// Penalty for shooting heavy weapons (launchers). Heavy Weapons skill halves this, once per skill level.
			FLOAT iTempPenalty = (gGameCTHConstants.AIM_HEAVY_WEAPON * iGunAimDifficulty) - iGunAimDifficulty;

			for (INT32 x=0; x < NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT ); x++)
			{
				iTempPenalty /= 2;
			}

			iGunAimDifficulty += iTempPenalty;
		}

		FLOAT umoda = (iGunAimDifficulty * GetPercentHandlingModifier( pInHand, stance )) / 100;
		FLOAT umodb = (iGunAimDifficulty * GetPercentHandlingModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
		iGunAimDifficulty += (gGameExternalOptions.ubProneModifierPercentage * umoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * umodb)/100; 

		// End Gun Handling modifiers
		//////////////////////////////////////////

		iAimModifier -= gGameCTHConstants.AIM_DRAW_COST * iGunAimDifficulty;

		// VISIBILITY
		if (iRange > 0 && iSightRange > iRange)
		{
			FLOAT iTempPenalty = (FLOAT)((FLOAT)iSightRange / (FLOAT)iRange);
			iTempPenalty = (FLOAT)(100 / iTempPenalty);
			iAimModifier += ((100-iTempPenalty) * gGameCTHConstants.AIM_VISIBILITY)/100;
			iAimModifier = __max( gGameCTHConstants.AIM_TARGET_INVISIBLE, iAimModifier );
		}
		//if (iSightRange == -1)
		if(fCantSeeTarget)
		{
			iAimModifier += gGameCTHConstants.AIM_TARGET_INVISIBLE;
		}

		// INJURY
		if (pSoldier->stats.bLife < pSoldier->stats.bLifeMax)
		{
			FLOAT iTempPenalty = gGameCTHConstants.AIM_INJURY;

			// Bleeding damage is used as a percentage off the Max Life, giving a penalty of up to BASE_INJURY.
			iAimModifier += (pSoldier->bBleeding * iTempPenalty) / pSoldier->stats.bLifeMax;

			// Bandage damage is used similarly, but is only 1/3 as bad.
			bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
			iAimModifier += ((bBandaged * iTempPenalty) / pSoldier->stats.bLifeMax) / 3;
		}

		// FATIGUE
		iAimModifier += (gGameCTHConstants.AIM_FATIGUE * (100 - pSoldier->bBreath)) / 100;

		// DRUNKNESS
		INT8 iDrunkness = GetDrunkLevel(pSoldier);
		if (iDrunkness > 0)
		{
			iDrunkness -= 1;
			iAimModifier += gGameCTHConstants.AIM_DRUNK[iDrunkness];
		}

		// GASSED
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		{
			iAimModifier += gGameCTHConstants.AIM_GASSED;
		}

		// BEING BANDAGED
		if (pSoldier->ubServiceCount > 0)
		{
			iAimModifier += gGameCTHConstants.AIM_BEING_BANDAGED;
		}

		// SHOCK (SHOOTER)
		if (pSoldier->aiData.bShock)
		{
			FLOAT iTempPenalty = gGameCTHConstants.AIM_SHOCK;
			INT32 iShockPercentage = (pSoldier->aiData.bShock * 100) / gGameExternalOptions.ubMaxSuppressionShock;

			iAimModifier += (iTempPenalty * iShockPercentage) / 100;
		}
		
		/////////////
		// Target-specific tests

		INT32 iHeightDifference = 0;
		if ( pTarget == NULL )
		{
			// Shooting to roof.
			if ( pSoldier->bTargetLevel > pSoldier->pathing.bLevel )
			{
				iHeightDifference = 3 * pSoldier->bTargetLevel;
			}
		}
		else
		{
			// HEIGHT DIFFERENCE

			UINT32 uiShooterHeight = 0;
			UINT32 uiTargetHeight = 0;

			if ( pSoldier->pathing.bLevel > 0 )
			{
				uiShooterHeight += 3 * pSoldier->pathing.bLevel;
			}
			
			switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
			{
				case ANIM_STAND:
					uiShooterHeight += 2;
					break;
				case ANIM_CROUCH:
					uiShooterHeight += 1;
					break;
			}

			if (pTarget->pathing.bLevel > 0)
			{
				uiTargetHeight += 3 * pTarget->pathing.bLevel;
			}

			switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
			{
				case ANIM_STAND:
					switch (ubAimPos)
					{
						case AIM_SHOT_HEAD:
							uiTargetHeight += 2;
							break;
						case AIM_SHOT_TORSO:
						case AIM_SHOT_RANDOM:
						case AIM_SHOT_GLAND:
							uiTargetHeight += 1;
							break;
					}
					break;
				case ANIM_CROUCH:
					switch (ubAimPos)
					{
						case AIM_SHOT_HEAD:
							uiTargetHeight += 1;
							break;
					}
					break;
			}

			iHeightDifference = uiShooterHeight - uiTargetHeight;
			if (iHeightDifference < 0)
			{
				iHeightDifference *= -1;
			}
			else
			{
				iHeightDifference = 0;
			}
		}

		// Height difference is mitigated by range. A LONGER range reduces this penalty!
		if (iRange > 0 && iHeightDifference > 0)
		{
			FLOAT iTempPenalty = gGameCTHConstants.AIM_SHOOTING_UPWARDS * iHeightDifference;
			iTempPenalty /= iRange;

			iAimModifier += iTempPenalty;
		}

		// GAME DIFFICULTY
		if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) && (pSoldier->bSide != gbPlayerNum) )
		{
			iAimModifier += gGameCTHConstants.AIM_DIFFICULTY[gGameOptions.ubDifficultyLevel];
		}
				
		// Percent modifier from the weapon and its attachments
		INT32 moda = GetPercentAimModifier( pInHand, stance );
		INT32 modb = GetPercentAimModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
		iAimModifier += (gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100; 

		// Calculate final max aiming
		

		//////////////////////////////////////////
		// Start by calculating the CTH cap, the absolute maximum CTH a shooter can get.
		iCombinedSkill = ( gGameCTHConstants.AIM_EXP * bExperience * 10 );
		iCombinedSkill += ( gGameCTHConstants.AIM_MARKS * bMarksmanship );
		iCombinedSkill += ( gGameCTHConstants.AIM_WIS * bWisdom );
		iCombinedSkill += ( gGameCTHConstants.AIM_DEX * bDexterity );
		
		iCombinedSkill /= (gGameCTHConstants.AIM_EXP + gGameCTHConstants.AIM_MARKS + gGameCTHConstants.AIM_WIS + gGameCTHConstants.AIM_DEX);

		INT32 uiCap = (INT32)iCombinedSkill;
		// Add percent-based modifier from the gun and its attachments
		INT32 uimoda = (INT32)(uiCap * GetPercentCapModifier( pInHand, stance )) / 100;
		INT32 uimodb = (INT32)(uiCap * GetPercentCapModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
		uiCap += (INT32)((gGameExternalOptions.ubProneModifierPercentage * uimoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * uimodb)/100); 

		// Add bonuses from Sniper Skill. Applies only when using a scope at or above its "best" range.
		INT16 sDifference = 99 - uiCap;
		INT16 sSniperSkillBonus = 0;
		// SANDRO - added support to Throwing trait
		if ( uiCap < gGameExternalOptions.ubMaximumCTH && Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
		{
			if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) )
			{
				uiCap += (INT16)((sDifference * gSkillTraitValues.ubTHBladesCtHBonusPerClick*3) / gGameExternalOptions.ubMaximumCTH);
				//uiCap += gSkillTraitValues.ubTHBladesCtHBonusPerClick*2;
			}
		}
		else if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
		{
			if (uiCap < gGameExternalOptions.ubMaximumCTH && iScopeMagFactor > 1.0 && iRange >= (INT32)uiBestScopeRange )
			{
				INT8	loop;
				if(gGameOptions.fNewTraitSystem)
				{
					if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
						loop = NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT );
					else
						loop = NUM_SKILL_TRAITS( pSoldier, SNIPER_NT );
				}
				else
					loop = NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT );

				for (INT8 cnt = 0; cnt < loop; cnt++)
				{
					// For each level of the sniper skill, cover a defined distance between the current CTH Cap and 99 (the
					// maximum...
					// SANDRO - this results in giving us total bonus on uiCap of lame 3-8%... compared to 10-20% bonus *PER AIM CLICK* in OCTH
					// ..as an artificial solution multiply the bonus by at least 2 to achieve some meaningful results
					if(gGameOptions.fNewTraitSystem)
					{
						if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
							sSniperSkillBonus += (INT16)((sDifference * gSkillTraitValues.ubGSAimingBonusPerClick * 2) / gGameExternalOptions.ubMaximumCTH);
						else
							sSniperSkillBonus += (INT16)((sDifference * gSkillTraitValues.ubSNAimingBonusPerClick * 2) / gGameExternalOptions.ubMaximumCTH);
					}
					else
						sSniperSkillBonus += (INT16)((sDifference * gGameCTHConstants.AIM_SNIPER_SKILL * 2) / gGameExternalOptions.ubMaximumCTH);
					sDifference -= sSniperSkillBonus;
				}

				uiCap += sSniperSkillBonus;
			}
		}
		// Make sure cap is within limits
		uiCap = __max(uiCap, __max(0,(UINT32)iChance));
		uiCap = __min(uiCap, gGameExternalOptions.ubMaximumCTH);

		// Now figure out the distance between the Base CTH and the CTH Cap. This is the distance we'll potentially
		// cover when applying the maximum number of aiming clicks for this gun.
		INT32 iMaxAimBonus = uiCap - iChance;

		iMaxAimBonus = (INT32)((iMaxAimBonus * (100+iAimModifier)) / 100);
		iMaxAimBonus = __max(0, iMaxAimBonus); // can't get less than 0 points for aiming...
		
		// factor in scopes under their range
		if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
		{
			if (iScopeMagFactor > 1.0 && iRange < (INT32)uiBestScopeRange)
			{
				// Calculate how much penalty this scope should give at this range
				FLOAT dScopePenaltyRatio = ((FLOAT)uiBestScopeRange / (FLOAT)iRange);
				INT32 iScopePenalty = (INT32)(dScopePenaltyRatio * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE * (iScopeMagFactor /2));
				iMaxAimBonus += iScopePenalty;
			}
			else if (iScopeMagFactor == 1.0f && GetHighestScopeMagnificationFactor( &(pSoldier->inv[pSoldier->ubAttackingHand]) ) > 1.0f )
			{
				// Not using a scope, but it's still there. Give half the penalty based on the size of the scope.
				INT32 iScopePenalty = (INT32)(((GetHighestScopeMagnificationFactor( &(pSoldier->inv[pSoldier->ubAttackingHand]) )/2) * gGameCTHConstants.AIM_TOO_CLOSE_SCOPE)/2);
				iMaxAimBonus += iScopePenalty;
			}

			iMaxAimBonus = __max(0, iMaxAimBonus); // can't get less than 0 points for aiming...
		}
		// Now let's find out how many CTH points we get per aiming level.
		UINT8 ubAllowedAimingLevels = AllowedAimingLevels( pSoldier, sGridNo );
		if(ubAimTime == ubAllowedAimingLevels)
			gCTHDisplay.fMaxAimReached = TRUE;
		FLOAT dAimFractionsDivisor = 0.0;
		for (UINT8 cnt = 0; cnt < ubAllowedAimingLevels; cnt++)
		{
			dAimFractionsDivisor += (cnt+1);
		}			

		FLOAT iAimPoints = 0;
		FLOAT iAimPointFraction = (FLOAT)((FLOAT)iMaxAimBonus / dAimFractionsDivisor);
		for (UINT8 x = 0; x < ubAimTime; x++)
		{
			iAimPoints += iAimPointFraction * (ubAllowedAimingLevels-x);
			// Add Flat Modifier from the weapon and its attachments
			INT32 moda = GetFlatAimModifier( pInHand, stance );
			INT32 modb = GetFlatAimModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
			iAimPoints += (gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100; 
		}

		// Finally, add the appropriate number of CTH points to our chance-to-hit, and limit it into good values.
		iChance = __max(iChance + (INT32)iAimPoints, iChance);
		iChance = __min(iChance, (INT32)uiCap);
	}
		
	// Impose global limits.	
	// Flugente: backgrounds
	iChance =  min(iChance, min(100, gGameExternalOptions.ubMaximumCTH + (UINT8)(pSoldier->GetBackgroundValue(BG_PERC_CTH_MAX))) );
	iChance = __max(iChance, gGameExternalOptions.ubMinimumCTH);
	
	return (iChance);
}
//////////////////////////////////////////////////////////////////////////////////
// END OLD VERSION OF NCTH CALCULATION
//////////////////////////////////////////////////////////////////////////////////
}

/*

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun"));


	if ( AM_A_ROBOT( pSoldier ) )
	{
		SOLDIERTYPE * pSoldier2;
		pSoldier2 = pSoldier->GetRobotController( );
		if ( pSoldier2 )
		{
			uiExperience = EffectiveExpLevel( pSoldier2 );
			uiMarksmanship = EffectiveMarksmanship( pSoldier2 );
			uiDexterity = EffectiveDexterity( pSoldier2 );;
			uiWisdom = EffectiveWisdom( pSoldier2 );
		}
	}

	// calculate actual range (in units, 10 units = 1 tile)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

	// If in burst mode, deduct points for change to hit for each shot after the first
	if ( pSoldier->bDoBurst && pSoldier->bDoAutofire == 0 )
	{
		// Snap: bipod may reduce burst penalty
		iPenalty = GetBurstPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			* (pSoldier->bDoBurst - 1);

		// halve the penalty for people with the autofire trait
		if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS ) )
		{
			iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS );
		}
		iChance -= iPenalty;
	}
	else if ( pSoldier->bDoAutofire > 0 )
	{
		// Snap: bipod may reduce auto penalty
		iPenalty = GetAutoPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)
			* (pSoldier->bDoBurst - 1);

		// halve the penalty for people with the autofire trait
		if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS ) )
		{
			iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS );
		}
		iChance -= iPenalty;
		// HEADROCK HAM 3.5: Store the penalty
		sTotalAutofirePenalty = iPenalty;
	}


	// WANNE: Changed this, because RPGs are not in the calculation, only guns
	//if ( Item[ usInHand ].usItemClass == IC_GUN )
	if ( Item[ usInHand ].usItemClass == IC_GUN || Item[ usInHand ].usItemClass == IC_LAUNCHER)
	{
		iMaxRange = GunRange( pInHand );
	}
	else
	{
		iMaxRange = CELL_X_SIZE; // one tile
	}


		// HEADROCK HAM B2.1 : The TARGET's shock now also affects aiming. If the target is prone
		// and in shock, they are harder to hit! This represents a target that's cowering as close
		// to the ground (and as close to any possible cover, like a small dune or a fold of earth
		// or anything like that).
		if ( gGameExternalOptions.ubAimPenaltyPerTargetShock > 0 )
		{
			// HEADROCK HAM B2.1 : This value determines how much penalty the target's shock-value gives the shooter.
			// As of HAM B2.3: There's a maximum range at which 100% penalty is given.
			// As of HAM B2.8: Target's stance and the targetted bodypart will affect the end result.
			
			INT8 AIM_PENALTY_PER_TARGET_SHOCK;
			INT16 sCoweringPenalty = 0;
			UINT8 ubCoweringDivisor;
			UINT16 MIN_RANGE_FOR_FULL_COWER;
			UINT16 MAX_TARGET_COWERING_PENALTY;

			AIM_PENALTY_PER_TARGET_SHOCK = gGameExternalOptions.ubAimPenaltyPerTargetShock;
			MIN_RANGE_FOR_FULL_COWER = gGameExternalOptions.usMinRangeForFullCoweringPenalty; 
			MAX_TARGET_COWERING_PENALTY = gGameExternalOptions.usMaxTargetCoweringPenalty;

			pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
			if (pTarget != NULL)
			{
				if (pTarget->aiData.bShock )
				{
					if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
					{
						ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorProne;

						sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
						sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
						sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

						if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
						{
							if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
								sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
						}
						if ( sCoweringPenalty < 1 )
							sCoweringPenalty = 1;

						iChance -= sCoweringPenalty;
					}
					else if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_CROUCH) 
					{
						switch ( ubAimPos )
						{
							case AIM_SHOT_HEAD:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedHead;
								break;
							case AIM_SHOT_TORSO:
							case AIM_SHOT_RANDOM:
							case AIM_SHOT_GLAND:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedTorso;
								break;
							case AIM_SHOT_LEGS:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedLegs;
								break;
						}

						sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
						sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
						sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

						if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
						{
							if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
								sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
						}
						if ( sCoweringPenalty < 1 )
							sCoweringPenalty = 1;

						// HEADROCK HAM B2.8.1: Added formula to make sure that cowering target penalties
						// are not given when on the roof.

						if (pSoldier->pathing.bLevel == pTarget->pathing.bLevel && pSoldier->pathing.bLevel > 0)
							sCoweringPenalty = 0; // No cowering penalties when both are on the roof!
						else if (pSoldier->pathing.bLevel < pSoldier->pathing.bLevel && gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
							sCoweringPenalty *= 2; // Much harder to shoot at anyone cowering above you.
						else if (pSoldier->pathing.bLevel > pSoldier->pathing.bLevel)
							sCoweringPenalty /= 2; // Much easier to shoot at cowerers below you.
						iChance -= sCoweringPenalty;
					}
				}
			}
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after scope bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));

		UINT8 bLightLevel = LightTrueLevel(sGridNo, pSoldier->bTargetLevel);
		// Apply flat to-hit bonus (e.g. from laser scope)
		// Snap: If prone and range is good, apply bipod bonus here as well
		iChance += GetToHitBonus( pInHand, iSightRange, bLightLevel,
			gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && iRange > MIN_PRONE_RANGE );
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after to hit bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));
	}

	if ( TANK( pSoldier ) && ( iRange / CELL_X_SIZE < MaxNormalDistanceVisible() ) )
	{
		// tank; penalize at close range!
		// 2 percent per tile closer than max visible distance
		iChance -= 2 * ( MaxNormalDistanceVisible() - (iRange / CELL_X_SIZE) );
	}

	if (iSightRange == 0)
	{
		// firing blind!
		iChance -= AIM_PENALTY_BLIND;
	}

	// HEADROCK HAM 3.5: Moved this here for now.
	// HEADROCK HAM B2.5: One of every X bullets in a tracer magazine is a tracer round, which will
	// bump the CTH up by a certain amount.

	if (AmmoTypes[(*pInHand)[0]->data.gun.ubGunAmmoType].tracerEffect == 1 && gGameExternalOptions.ubRealisticTracers > 0 )
	{
		UINT16 iBulletsLeft, iTracersFired, iBulletsPerTracer, iBulletsSinceLastTracer;
		UINT8 cnt;
		//UINT16 iAutoPenaltySinceLastTracer;
		iTracersFired = 0;
		iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;

		// Calculate number of bullets left right before firing this bullet
		if (fCalculateCTHDuringGunfire)
		{
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft + (pSoldier->bDoBurst - 1);
		}
		else
		{
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft;
		}

		iBulletsSinceLastTracer = 0;
		for (cnt=0;cnt<pSoldier->bDoBurst;cnt++)
		{
			iBulletsSinceLastTracer++;
			if ((( iBulletsLeft - (cnt - 1) ) / iBulletsPerTracer) - ((iBulletsLeft - cnt) / iBulletsPerTracer) == 1)
			{
				iBulletsSinceLastTracer = 0;
			}
		}

		iTracersFired = ((iBulletsLeft+1) / iBulletsPerTracer) - (((iBulletsLeft+1) - (pSoldier->bDoBurst)) / iBulletsPerTracer);

		if ( iTracersFired > 0 )
		{
			// HEADROCK HAM 3.5: I'm going to revise this - my current system makes no sense at all. What was I
			// thinking?!

			// Correct all autofire penalty so far
			//iBonus = iPenalty;
			
			// Add Tracer Bump if previous bullet was a tracer
			//iBonus += (gGameExternalOptions.ubCTHBumpPerTracer * iTracersFired);
			iBonus = (gGameExternalOptions.ubCTHBumpPerTracer * iTracersFired);

			// Calculate penalty since last tracer was fired
			UINT8 ubAutoPenaltySinceLastTracer = GetAutoPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * iBulletsSinceLastTracer;
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS ) )
			{
				ubAutoPenaltySinceLastTracer /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS );
			}			
			// Add penalty to bonus.
			//iBonus -= iAutoPenaltySinceLastTracer;
			
			///////////////////////////////////////////////////
			// HEADROCK HAM 3.5: Limit maximum bonus by range.
			
			INT16 sBaseChance = iChance + sTotalAutofirePenalty;

			// We don't want to enforce a limit unless the tracers have actually put us over the original CtH.
			if (sBaseChance <= iChance+iBonus) // Base_Chance without AutoPen, less or equal to Current_Chance plus tracer bumps
			{
				// store lowest: Chance+Tracer bumps, or Range-enforced limit
				INT16 sChanceLimit = __min(iChance+iBonus, sBaseChance+(((iRange-100) / CELL_X_SIZE) * gGameExternalOptions.ubRangeDifficultyAimingWithTracers));
				// store highest: Chance Delta or base CtH
				//sChanceDelta = __max(sChanceDelta, sBaseChance);

				// iBonus is the distance between the enforced limit (if any) and the current chance with all penalties so far.
				// But it can't be negative, 'cause it's a bonus.
				iBonus = __max(0,sChanceLimit - iChance);
				// Add autopenalty since last tracer
				if (iBulletsSinceLastTracer < iBulletsPerTracer)
				{
					iBonus -= ubAutoPenaltySinceLastTracer;
				}

			}
			iChance += iBonus;
		}
	}


	pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
	if (pTarget != NULL)
	{
		// targeting a merc
		// adjust for crouched/prone target
		switch( gAnimControl[ pTarget->usAnimState ].ubHeight )
		{
			case ANIM_CROUCH:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 13% penalty per tile closer than min range
					iChance -= 13 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE + 10 * (AIM_PENALTY_TARGET_CROUCHED / 3) )
					{
						iChance -= AIM_PENALTY_TARGET_CROUCHED;
					}
					else if (iRange > POINT_BLANK_RANGE)
					{
						// at close range give same bonus as prone, up to maximum of AIM_PENALTY_TARGET_CROUCHED
						iChance -= 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
					}
				}
				break;
			case ANIM_PRONE:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 25% penalty per tile closer than min range
					iChance -= 25 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE)
					{
						// reduce chance to hit with distance to the prone/immersed target
						iPenalty = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
						iPenalty = __min( iPenalty, AIM_PENALTY_TARGET_PRONE );

						iChance -= iPenalty;
					}
				}
				break;
			case ANIM_STAND:
				// if we are prone and at close range, then penalize shots to the torso or head!
				if ( iRange <= MIN_PRONE_RANGE && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
				{
					if ( ubAimPos == AIM_SHOT_RANDOM || ubAimPos == AIM_SHOT_GLAND )
					{
						ubAdjAimPos = AIM_SHOT_TORSO;
					}
					else
					{
						ubAdjAimPos = ubAimPos;
					}
					// lose 10% per height difference, lessened by distance
					// e.g. 30% to aim at head at range 1, only 10% at range 3
					// or 20% to aim at torso at range 1, no penalty at range 3
					// NB torso aim position is 2, so (5-aimpos) is 3, for legs it's 2, for head 4
					iChance -= (5 - ubAdjAimPos - iRange / CELL_X_SIZE) * 10;
				}
				break;
			default:
				break;
		}

		// penalty for amount that enemy has moved
		// HEADROCK HAM B2.6: Externalized the value
		iPenalty = __min( (UINT16)((float)pTarget->bTilesMoved * (float)gGameExternalOptions.iMovementEffectOnAiming), 30 );
		iChance -= iPenalty;

			// reduce dodge ability by the attacker's stats
			iBonus = ( EffectiveDexterity( pSoldier ) / 5 + EffectiveExpLevel( pSoldier ) * 2);
			if ( TANK( pTarget ) || (pSoldier->ubBodyType != QUEENMONSTER) )
			{
				// reduce ability to track shots
				iBonus = iBonus / 2;
			}

			if ( iPenalty > iBonus )
			{
				iChance -= (iPenalty - iBonus);
			}
		}
	}



	// CHECK IF TARGET IS WITHIN GUN'S EFFECTIVE MAXIMUM RANGE
	if ( iRange > iMaxRange )
	{
		// a bullet WILL travel that far if not blocked, but it's NOT accurate,
		// because beyond maximum range, the bullet drops rapidly

		// This won't cause the bullet to be off to the left or right, only make it
		// drop in flight.
		iChance /= 2;
	}

   DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: ichance = %d",iChance));
  return (iChance);
}*/

UINT32 CalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos )
{
	if(UsingNewCTHSystem() == true)
		return CalcNewChanceToHitGun(pSoldier, sGridNo, ubAimTime, ubAimPos);

	//CHRISL: I've tried to sort and reorganize this function to make it a little easier to follow.  I've removed most of the unused code fragments and removed
	//	alot of the notes.  I've also tried to sort the various portions of the function so that similar actions orrucr at the same place in the code.  The
	//	following is the newly written function
	SOLDIERTYPE * pTarget;
	OBJECTTYPE * pInHand;
	INT32	iChance;	//CTH
	INT32	iRange;		//Actual range to target
	INT32	iSightRange = 0;	//LOS range
	INT32	iCoverRange;	//Amount sight range is modified due to cover
	INT32	iMaxRange;	//Weapon maximum range
	INT32	iMinRange;	//Minimum effective range
	INT32	iGunCondition;	//Weapons effective condition
	INT32	iMarksmanship;	//Mercs effective marksmanship
	INT32	iAccRangeMod;	//Range modifier granted from weapon accuracy
	INT32	iBonus, iPenalty;
	INT16	sDistVis, sDistVisNoScope, iMaxNormRange, iScopeVisionRangeBonus, sCoweringPenalty = 0, sTotalAutofirePenalty = 0;
	INT16	bonusProgression[8] = {500,500,600,600,750,750,750,1000};	//Aiming progression
	UINT16	usInHand, usShockPenalty, MIN_RANGE_FOR_FULL_COWER, MAX_TARGET_COWERING_PENALTY;
	UINT16	iBulletsLeft, iTracersFired = 0, iBulletsPerTracer, iBulletsSinceLastTracer=0, iRoundsFiredPreviously;
	INT8	bBandaged, maxClickBonus = 10, AIM_PENALTY_PER_TARGET_SHOCK;
	UINT8	ubAdjAimPos, ubTargetID, bLightLevel, ubCoweringDivisor, ubAutoPenaltySinceLastTracer=0;
	FLOAT	maxBonus, aimTimeBonus, scopeRangeMod = 0.0f, iAimBonus;
	bool	fCantSeeTarget = false, fCoverObscured = false;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun"));
	if ( pSoldier->stats.bMarksmanship == 0 ) {
		return( gGameExternalOptions.ubMinimumCTH );
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Assign basic variables
	// Flugente: check for underbarrel weapons and use that object if necessary
	pInHand = pSoldier->GetUsedWeapon( &(pSoldier->inv[pSoldier->ubAttackingHand]) );
	UINT16 usItemUsed =  pSoldier->GetUsedWeaponNumber( &(pSoldier->inv[pSoldier->ubAttackingHand]) );

	pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
	iGunCondition = WEAPON_STATUS_MOD( (*pInHand)[0]->data.gun.bGunStatus );
	usInHand = pSoldier->usAttackingWeapon;
	ubTargetID = WhoIsThere2( sGridNo, pSoldier->bTargetLevel );
	bLightLevel = LightTrueLevel(sGridNo, pSoldier->bTargetLevel);
	usShockPenalty = pSoldier->aiData.bShock * AIM_PENALTY_PER_SHOCK;
	iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;
	AIM_PENALTY_PER_TARGET_SHOCK = gGameExternalOptions.ubAimPenaltyPerTargetShock;
	MIN_RANGE_FOR_FULL_COWER = gGameExternalOptions.usMinRangeForFullCoweringPenalty; 
	MAX_TARGET_COWERING_PENALTY = gGameExternalOptions.usMaxTargetCoweringPenalty;
	/////////////////////////////////////////////////////////////////////////////////////

	UINT8 stance = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;

	// Flugente: new feature: if the next tile in our sight direction has a height so that we could rest our weapon on it, we do that, thereby gaining the prone boni instead. This includes bipods
	if ( gGameExternalOptions.fWeaponResting && pSoldier->IsWeaponMounted() )
		stance = ANIM_PRONE;

	/////////////////////////////////////////////////////////////////////////////////////
	// Assign range variables -- all range values are in units (10 units = 1 tile)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );	// calculate actual range

	if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) ) // ignore scopes when firing from hip
		gbForceWeaponReady = true;

	sDistVis = pSoldier->GetMaxDistanceVisible(sGridNo, pSoldier->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;
	iScopeVisionRangeBonus = GetTotalVisionRangeBonus(pSoldier, bLightLevel);	// not an actual range value, simply a modifier for range calculations
	if (ubTargetID != NOBODY && pSoldier->aiData.bOppList[ubTargetID] == SEEN_CURRENTLY || gbPublicOpplist[pSoldier->bTeam][ubTargetID] == SEEN_CURRENTLY)
		iSightRange = SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pSoldier->bAimShotLocation, false );
	if (iSightRange == 0) {	// didn't do a bodypart-based test or can't see specific body part aimed at
		iSightRange = SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pSoldier->bTargetLevel, pSoldier->bTargetCubeLevel, TRUE, NO_DISTANCE_LIMIT, false );
		fCoverObscured = true;
	}
	if (iSightRange == 0) {	// Can't see the target but we still need to know what the sight range would be if we could so we can deal with cover penalties
		iSightRange = SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pSoldier->bAimShotLocation, false, true );
		fCantSeeTarget = true;
		fCoverObscured = false;
	}
		gbForceWeaponReady = false;
		gbForceWeaponNotReady = true;
	sDistVisNoScope = pSoldier->GetMaxDistanceVisible(sGridNo, pSoldier->bTargetLevel, CALC_FROM_ALL_DIRS ) * CELL_X_SIZE;
		gbForceWeaponNotReady = false;

	// Flugente: blind soldiers have sDistVisNoScope = 0...
	if ( sDistVisNoScope )
		scopeRangeMod = (float)sDistVis / (float)sDistVisNoScope;	// percentage DistVis has been enhanced due to an attached scope

	iMaxNormRange = MaxNormalDistanceVisible() * CELL_X_SIZE;
	if ( Item[ usItemUsed ].usItemClass == IC_GUN || Item[ usItemUsed ].usItemClass == IC_LAUNCHER)
		iMaxRange = GunRange( pInHand, pSoldier ); // SANDRO - added argument
	else
		iMaxRange = CELL_X_SIZE; // one tile
	iCoverRange = __max(0,iSightRange - iRange);
	iMinRange = iMaxRange / 10;

	// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
	FLOAT accuracyheatmultiplicator = 1.0;
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		FLOAT overheatjampercentage = GetGunOverheatJamPercentage( pInHand );
		FLOAT accuracymalus = (FLOAT)((max(1.0, overheatjampercentage) - 1.0) * 0.1);
		accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
	}

	iAccRangeMod = (INT32)(iRange * accuracyheatmultiplicator * Weapon[usItemUsed].bAccuracy / 100);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// Modify Sight and Physical Range

	if ( scopeRangeMod )
		iSightRange = (INT32)(iSightRange / scopeRangeMod);

	if(iSightRange > 0 && !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) ){
		//CHRISL: The LOS system, which determines whether to display an enemy unit, does not factor in the AimBonus tag during it's calculations.  So having
		//	the CTH system use that tag to adjust iSightRange for AimBonus applied from armor might not be the best option.  Especially as it can sometimes
		//	result in 0% CTH when everything looks like you could actually hit the target.  Let's try applying this penalty to CTH at the same point we would
		//	apply the "invisible target" penalty.
		//iSightRange -= GetGearAimBonus ( pSoldier, iSightRange, ubAimTime ) * iSightRange / 100;
		if ( gGameOptions.fNewTraitSystem ) {
			if ( HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) ) {
				iSightRange -= ((gSkillTraitValues.ubSNEffRangeToTargetReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )) * iSightRange) /100;
			}
		}
		else if ( HAS_SKILL_TRAIT( pSoldier, PROF_SNIPER_OT ) ) {
			iSightRange -= ((gbSkillTraitBonus[ PROF_SNIPER_OT ] * NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )) * iSightRange) /100;
		}
		if (iRange < GetMinRangeForAimBonus(pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand])) && iScopeVisionRangeBonus > 50){	// iSightRange penalty for using a high power scope within min range due to poor focus
			iPenalty = 0;
			for(UINT8 loop = 0; loop < ((GetMinRangeForAimBonus(pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand])) - iRange)/CELL_X_SIZE); loop++){
				iPenalty += iSightRange * iScopeVisionRangeBonus / 100;
			}
			iSightRange += iPenalty;
		}
		if (iSightRange < 1) {
			iSightRange = 1;
		}
	}
	if(iSightRange > sDistVis)
		fCantSeeTarget = true;
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Determine iMarksmanship and Base CTH
	if (Item[usItemUsed].rocketlauncher ){
		// use the same calculation as for mechanical thrown weapons
		iMarksmanship = ( EffectiveDexterity( pSoldier, FALSE ) + EffectiveMarksmanship( pSoldier ) + EffectiveWisdom( pSoldier ) + (10 * EffectiveExpLevel( pSoldier ) )) / 4;
		// heavy weapons trait helps out
		if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPS_OT ) && !( gGameOptions.fNewTraitSystem )) // SANDRO - old/new traits
			iMarksmanship += gbSkillTraitBonus[HEAVY_WEAPS_OT] * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT );
	} else {
		iMarksmanship = EffectiveMarksmanship( pSoldier );
		if ( AM_A_ROBOT( pSoldier ) ) {
			SOLDIERTYPE * pSoldier2;
			pSoldier2 = pSoldier->GetRobotController( );
			if ( pSoldier2 )
				iMarksmanship = __max( iMarksmanship, EffectiveMarksmanship( pSoldier2 ) );
		}
	}
	// modify chance to hit by morale
	iMarksmanship += GetMoraleModifier( pSoldier );
	// penalize marksmanship for fatigue
	iMarksmanship -= GetSkillCheckPenaltyForFatigue( pSoldier, iMarksmanship );
	if (iGunCondition >= iMarksmanship)	// base chance is equal to the shooter's marksmanship skill
		iChance = iMarksmanship;
	else	// base chance is equal to the average of marksmanship & gun's condition!
		iChance = (iMarksmanship + iGunCondition) / 2;
	// if shooting same target as the last shot
	if (sGridNo == pSoldier->sLastTarget )	// give a bonus to hit
		iChance += AIM_BONUS_SAME_TARGET;
	// if shooting from alternative weapon holding, apply the preset penalty
	if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
		iChance -= gGameExternalOptions.ubAltWeaponHoldingCtHPenaly;
		
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Range and Size modifiers
	
	// Effects of actual gun max range... the numbers are based on wanting -40%
	// at range 26for a pistol with range 13, and -0 for a sniper rifle with range 80
	// The result is about 1.5% penalty per tile but only after about 30% of max range.  Within 30% max range, there is no modifier
	iPenalty = ((iMaxRange - (iRange-iAccRangeMod) * 3) * 10) / (17 * CELL_X_SIZE);
	if ( iPenalty < 0 )
		iChance += iPenalty;
	if ( TANK( pSoldier ) && ( iRange / CELL_X_SIZE < MaxNormalDistanceVisible() ) )
		iChance -= 2 * ( MaxNormalDistanceVisible() - (iRange / CELL_X_SIZE) );
	// Like the above modifier, only this applies to long range weapons trying to be used in close quater.  Penalty will result when within 10% of max range
	iPenalty = (((iRange - iMinRange - iAccRangeMod) * 12) * 10) / (17 * CELL_X_SIZE);
	if ( iPenalty < 0 )
		iChance += iPenalty;
	
	// Effects of visual range
	// From for JA2.5:  3% bonus/penalty for each tile different from range NORMAL_RANGE.
	if (!TANK(pSoldier))	// WANNE: No penalty on the tank
		iPenalty = 3 * ( NORMAL_RANGE - iSightRange ) / CELL_X_SIZE;
	if ( fCantSeeTarget ){
		// CHRISL: There are conditions where iSightRange can still return 0.  If that happens, the result is that "impossible" shots are actually easier then
		//	simply "really hard" shots.  As a result, if we can't see the target and we have a 0 sight range, we should reevaluate the above penalty but use
		//	iRange instead of iSightRange, then include the unseen penalty.
		if(iSightRange == 0)
			iPenalty = (3 * ( NORMAL_RANGE - iRange ) / CELL_X_SIZE) - gGameExternalOptions.iPenaltyShootUnSeen;
		iPenalty = min(iPenalty, -gGameExternalOptions.iPenaltyShootUnSeen);
	}
	iChance += iPenalty;
	//CHRISL: Applying the Gear AimBonus (penalty) here, and directly to iChance as a flat penalty, instead of altering iSightRange above.
	if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
		iChance += GetGearAimBonus ( pSoldier, iSightRange, ubAimTime );

	//CHRISL: We should probably include these target size penalties even if we can't see the target so that shooting a "hidden" head is harder then a "hidden" body
	// if aiming at the head, reduce chance to hit
	if (ubAimPos == AIM_SHOT_HEAD)
	{
		// penalty of 3% per tile
		//iPenalty = 3 * iSightRange / 10; //comm by ddd
		iPenalty = INT32(gGameExternalOptions.uShotHeadPenalty * iSightRange / 10);
		iChance -= iPenalty;
	}
	else if (ubAimPos == AIM_SHOT_LEGS)
	{
		// penalty of 1% per tile
		iPenalty = iSightRange / 10;
		iChance -= iPenalty;
	}
	//CHRISL: A target's stance should have no impact on an aimed, headshot.  The head doesn't get any smaller just because the target is crouching down.
	if (pTarget != NULL && ubAimPos != AIM_SHOT_HEAD)
	{
		// targeting a merc
		// adjust for crouched/prone target
		switch( gAnimControl[ pTarget->usAnimState ].ubHeight )
		{
			case ANIM_CROUCH:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 13% penalty per tile closer than min range
					iChance -= (INT32)(13 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE ));
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE + 10 * (AIM_PENALTY_TARGET_CROUCHED / 3) )
					{
						iChance -= (INT32)(AIM_PENALTY_TARGET_CROUCHED);
					}
					else if (iRange > POINT_BLANK_RANGE)
					{
						// at close range give same bonus as prone, up to maximum of AIM_PENALTY_TARGET_CROUCHED
						iChance -= (INT32)(3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE));	 // penalty -3%/tile
					}
				}
				break;
			case ANIM_PRONE:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 25% penalty per tile closer than min range
					iChance -= (INT32)(25 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE ) * iPenalty);
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE)
					{
						// reduce chance to hit with distance to the prone/immersed target
						iPenalty = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
						iPenalty = __min( iPenalty, AIM_PENALTY_TARGET_PRONE );

						iChance -= iPenalty;
					}
				}
				break;
				// silversurfer: this doesn't make any sense at all. iPenalty is defined nowhere and for 3 to 5 tiles range
				// we actually get a bonus to iChance most of the time because the formula produces a negative value. 
				// Headshots are not considered at all because of the above if statement. So we will consider shooting upwards separately below.
/*			case ANIM_STAND:
				// if we are prone and at close range, then penalize shots to the torso or head!
				if ( iRange <= MIN_PRONE_RANGE && stance == ANIM_PRONE )
				{
					if ( ubAimPos == AIM_SHOT_RANDOM || ubAimPos == AIM_SHOT_GLAND )
					{
						ubAdjAimPos = AIM_SHOT_TORSO;
					}
					else
					{
						ubAdjAimPos = ubAimPos;
					}
					// lose 10% per height difference, lessened by distance
					// e.g. 30% to aim at head at range 1, only 10% at range 3
					// or 20% to aim at torso at range 1, no penalty at range 3
					// NB torso aim position is 2, so (5-aimpos) is 3, for legs it's 2, for head 4
					iChance -= (INT32)((5 - ubAdjAimPos - iRange / CELL_X_SIZE) * 10 * iPenalty);
				}
				break;*/
			default:
				break;
		}
	}
	// start of new functionality for shooting upwards (thanks to NCTH)
	INT32 iHeightDifference = 0;

	// SHOOTING AT A TARGET AT DIFFERENT HEIGHT?
	if ( pTarget == NULL )
	{
		// Shooting to roof.
		if ( pSoldier->bTargetLevel > pSoldier->pathing.bLevel )
		{
			iHeightDifference = 3 * pSoldier->bTargetLevel;
		}
	}
	else
	{
		// HEIGHT DIFFERENCE
		UINT32 uiShooterHeight = 0;
		UINT32 uiTargetHeight = 0;

		if ( pSoldier->pathing.bLevel > 0 )
		{
			uiShooterHeight += 3 * pSoldier->pathing.bLevel;
		}
		
		switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				uiShooterHeight += 2;
				break;
			case ANIM_CROUCH:
				uiShooterHeight += 1;
				break;
		}

		if (pTarget->pathing.bLevel > 0)
		{
			uiTargetHeight += 3 * pTarget->pathing.bLevel;
		}

		switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 2;
						break;
					case AIM_SHOT_TORSO:
					case AIM_SHOT_RANDOM:
					case AIM_SHOT_GLAND:
						uiTargetHeight += 1;
						break;
				}
				break;
			case ANIM_CROUCH:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 1;
						break;
				}
				break;
		}

		iHeightDifference = uiShooterHeight - uiTargetHeight;
		if (iHeightDifference < 0)
		{
			iHeightDifference *= -1;
		}
		else
		{
			iHeightDifference = 0;
		}
	}

	// Height difference is mitigated by range. A LONGER range reduces this penalty!
	if (iRange > 0 && iHeightDifference > 0)
	{
		FLOAT fTempPenalty = -100 * iHeightDifference;
		fTempPenalty /= iRange;
		iChance += fTempPenalty;
	}
	// end of new functionality for shooting upwards

	if (pTarget != NULL)
	{
		// penalty for amount that enemy has moved
		// HEADROCK HAM B2.6: Externalized the value
		iPenalty = __min( (UINT16)((float)pTarget->bTilesMoved * (float)gGameExternalOptions.iMovementEffectOnAiming), 30 );
		///////////////////////////////////////////////////////////////////////////////////
		// SANDRO - fearless characters do not even take their head down no matter what
		if ( gGameOptions.fNewTraitSystem && pTarget->ubProfile != NO_PROFILE )
		{
			if ( gMercProfiles[ pTarget->ubProfile ].bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
				iPenalty -= (iPenalty * 2 / 3);	// two thirds only
		}
		///////////////////////////////////////////////////////////////////////////////////
		iChance -= iPenalty;

		// if target sees us, he may have a chance to dodge before the gun goes off
		// but ability to dodge is reduced if crouched or prone!
		if (pTarget->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY && !TANK( pTarget ) && !(pSoldier->ubBodyType != QUEENMONSTER) )
		{
			iPenalty = ( EffectiveAgility( pTarget, FALSE ) / 5 + EffectiveExpLevel( pTarget ) * 2);
			switch( gAnimControl[ pTarget->usAnimState ].ubHeight )
			{
				case ANIM_CROUCH:
					iPenalty = iPenalty * 2 / 3;
					break;
				case ANIM_PRONE:
					iPenalty /= 3;
					break;
			}

			// reduce dodge ability by the attacker's stats
			iBonus = ( EffectiveDexterity( pSoldier, FALSE ) / 5 + EffectiveExpLevel( pSoldier ) * 2);
			if ( TANK( pTarget ) || (pSoldier->ubBodyType != QUEENMONSTER) )
			{
				// reduce ability to track shots
				iBonus = iBonus / 2;
			}

			if ( iPenalty > iBonus )
			{
				iChance -= (iPenalty - iBonus);
			}
		}
		if(fCoverObscured){	// If we can't see the body part but can see the tile include a -1%/tile penalty
			iPenalty = iCoverRange / CELL_X_SIZE;
			iChance -= iPenalty;
		}
	}
	else if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
	{
		// 25% penalty per tile closer than min range
		iChance -= 25 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
	}

	attachmentList::iterator iter_end = (*&(pSoldier->inv[pSoldier->ubAttackingHand]))[0]->attachments.end();
	for(attachmentList::iterator iter = (*&(pSoldier->inv[pSoldier->ubAttackingHand]))[0]->attachments.begin(); iter != iter_end; ++iter)
	{
		if(iter->exists() && !IsAttachmentClass(iter->usItem, AC_SCOPE|AC_SIGHT|AC_IRONSIGHT ) && Item[iter->usItem].aimbonus >= gGameExternalOptions.sHighPowerScope && iRange > Item[iter->usItem].minrangeforaimbonus)
		{
			iPenalty = (Item[iter->usItem].aimbonus * (iRange - Item[iter->usItem].minrangeforaimbonus)) / 1000;
			iPenalty = min(AIM_BONUS_PRONE, iPenalty);
			iChance -= iPenalty;
		}
	}

	// Flugente: check for scope mode
	if ( Item[(&(pSoldier->inv[pSoldier->ubAttackingHand]))->usItem].usItemClass == IC_GUN && !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
	{
		std::map<INT8, OBJECTTYPE*> ObjList;
		GetScopeLists((&(pSoldier->inv[pSoldier->ubAttackingHand])), ObjList);
		
		if ( ObjList[pSoldier->bScopeMode] != NULL )
		{
			iPenalty = (Item[ObjList[pSoldier->bScopeMode]->usItem].aimbonus * (iRange - Item[ObjList[pSoldier->bScopeMode]->usItem].minrangeforaimbonus)) / 1000;
			iPenalty = min(AIM_BONUS_PRONE, iPenalty);
			iChance -= iPenalty;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Modify for traits
	// Flugente: moved trait modifiers into a member function
	UINT8 targetprofile = NOBODY;
	if ( pTarget && pTarget->ubProfile != NOBODY )
		targetprofile = pTarget->ubProfile;

	iChance += pSoldier->GetTraitCTHModifier( usInHand, ubAimTime, targetprofile );
		
	// Flugente: backgrounds
	if ( pTarget && pTarget->bTeam == CREATURE_TEAM )
		iChance += pSoldier->GetBackgroundValue(BG_PERC_CTH_CREATURE);
		
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Modify for stance and weapon
	// if shooter is crouched, he aims slightly better (to max of AIM_BONUS_CROUCHING)
	if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH )
	{
		iBonus = iRange / 10;
		if (iBonus > AIM_BONUS_CROUCHING)
		{
			iBonus = AIM_BONUS_CROUCHING;
		}

		// if our weapon is rested, factor in the prone position
		if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != stance )
		{
			INT32 pronebonus = iRange / 10;
			if (pronebonus > AIM_BONUS_PRONE)
			{
				pronebonus = AIM_BONUS_PRONE;
			}

			iBonus = (INT32)((gGameExternalOptions.ubProneModifierPercentage * pronebonus + (100 - gGameExternalOptions.ubProneModifierPercentage) * iBonus)/100); 
		}

		iChance += iBonus;
	}
	else if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND )
	{
		// if our weapon is rested, factor in the prone position
		if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != stance )
		{
			iBonus = iRange / 10;
			if (iBonus > AIM_BONUS_PRONE)
			{
				iBonus = AIM_BONUS_PRONE;
			}

			iChance += (INT32)((gGameExternalOptions.ubProneModifierPercentage * iBonus )/100); 
		}
	}
	// if shooter is prone, he aims even better, except at really close range
	else if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
	{
		if (iRange > MIN_PRONE_RANGE)
		{
			iBonus = iRange / 10;
			if (iBonus > AIM_BONUS_PRONE)
			{
				iBonus = AIM_BONUS_PRONE;
			}
			iChance += iBonus;
		}
	}
	
	//Madd: inherent weapon accuracy bonus
	//iChance += Weapon[usInHand].bAccuracy;
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Modify for using one hand
	if ( !(Item[ usInHand ].twohanded ) )
		//check for 2weapon, retaining original code :JMich
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() == false)
		{
			// firing with gun in right hand, and second hand empty (ie: no grenade, med kit or anything there)
			// Madd: easier to fire pistol/smg w/one hand free, essentially this will make pistols a little bit more accurate, and hopefully still useful later in the game
			if (Weapon[usInHand].ubWeaponClass == HANDGUNCLASS)
				iChance += AIM_BONUS_TWO_HANDED_PISTOL;
		}
		else
		{
			// Madd: harder to fire smgs w/something in other hand
			// SMGs are treated as pistols for these purpose except there is a -5 penalty;
			if (Weapon[usInHand].ubWeaponClass == SMGCLASS)
			{
				iChance -= AIM_PENALTY_SMG;
			}
			if (gGameOptions.fNewTraitSystem)
			{
				// Okay, we will shoot from both weapons, calculate penalty to CtH
				if ( pSoldier->IsValidSecondHandShot() )
				{					
					// penaly to shoot from dual weapons
					iChance -= (INT32)(gSkillTraitValues.ubCtHPenaltyDualShot * (HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) ? ((100 - gSkillTraitValues.ubAMPenaltyDoubleReduction)/100) : 1 ));
				}
			}
			else // original code
			{
				if ( !HAS_SKILL_TRAIT( pSoldier, AMBIDEXT_OT ) )
				{
					if ( pSoldier->IsValidSecondHandShot( ) )
					{
						// penalty to aim when firing two pistols
						iChance -= AIM_PENALTY_DUAL_PISTOLS;
					}
					/*
					else
					{
						// penalty to aim with pistol being fired one-handed
						iChance -= AIM_PENALTY_ONE_HANDED_PISTOL;
					}
					*/
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Modify for Recoil and Tracer effects
	if ( pSoldier->bDoBurst && pSoldier->bDoAutofire == 0 )
	{
		/*CHRISL: At this point in the calculation, Bipods, Foregrips and other "recoil stabalizing" bonuses should reduce the weapons "recoil penalty"
			(BurstPenalty) by a percentage.  This reduction should apply to the recoil per shot and not as a "flat" initial reduction.  Later in the code,
			Bipods also grant their flat CTH bonus so we don't need to mess this that here*/
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
		{
			INT16 moda = GetBurstToHitBonus(pInHand, stance == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			INT16 modb = GetBurstToHitBonus(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			iBonus = (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 
			
			iPenalty = Weapon[pInHand->usItem].ubBurstPenalty * (pSoldier->bDoBurst - 1) * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
			iPenalty = max(0, (iPenalty * (100 - iBonus))/100 );
		}
		else
		{
			// Snap: bipod may reduce burst penalty
			INT16 moda = GetBurstPenalty(pInHand, stance == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			INT16 modb = GetBurstPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			iPenalty = (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 

			if(gGameExternalOptions.bAimedBurstEnabled)
				iPenalty += Weapon[usInHand].ubBurstPenalty * (pSoldier->bDoBurst - 1) * gGameExternalOptions.uAimedBurstPenalty;
		}

		// SAMDRO - shooting dual bursts is somehow harder to control
		if ( pSoldier->IsValidSecondHandBurst() )
		{
			iPenalty = iPenalty*6/5; // +10% 
		}

		// halve the penalty for people with the autofire trait
		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
			{
				iPenalty = max(0, ((iPenalty * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
			{
				iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
			}
		}
		iChance -= iPenalty;
	}
	else if ( pSoldier->bDoAutofire > 0 )
	{
		/*CHRISL: At this point in the calculation, Bipods, Foregrips and other "recoil stabalizing" bonuses should reduce the weapons "recoil penalty"
			(AutoPenalty) by a percentage.  This reduction should apply the the recoil per shot and not as a "flat" initial reduction.  Later in the code,
			Bipods also grant their flat CTH bonus so we don't need to mess this that here*/
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
		{
			INT16 moda = GetAutoToHitBonus(pInHand, stance == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			INT16 modb = GetAutoToHitBonus(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			iBonus = (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 

			iPenalty = Weapon[pInHand->usItem].AutoPenalty * (pSoldier->bDoBurst - 1) * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
			iPenalty = max(0, (iPenalty * (100 - iBonus))/100 );
		}
		else
		{
			// Snap: bipod may reduce auto penalty
			INT16 moda = GetAutoPenalty(pInHand, stance == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			INT16 modb = GetAutoPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			iPenalty = (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 

			if(gGameExternalOptions.bAimedBurstEnabled)
				iPenalty += Weapon[usInHand].AutoPenalty * (pSoldier->bDoBurst - 1) * gGameExternalOptions.uAimedBurstPenalty;
		}

		// SAMDRO - shooting dual bursts is somehow harder to control
		if ( pSoldier->IsValidSecondHandBurst() )
		{
			iPenalty = iPenalty*6/5; // +10% 
		}

		// halve the penalty for people with the autofire trait
		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
			{
				iPenalty = max(0, ((iPenalty * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
			{
				iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
			}
		}
		iChance -= iPenalty;
		// HEADROCK HAM 3.5: Store the penalty
		sTotalAutofirePenalty = iPenalty;
	}
	if (AmmoTypes[(*pInHand)[0]->data.gun.ubGunAmmoType].tracerEffect == 1 && gGameExternalOptions.ubRealisticTracers > 0 )
	{
		/*CHRISL: We want to know here is how many tracers have been fired during this autofire sequence.  But we also want to figure that based on how
			much ammo we've used from the current mag.  If every 3rd round is a tracer, and we previously fired 5 rounds from our "mag", then the first
			round of this burst is a tracer, the 2nd & 3rd are normal, the 4th is a tracer, 5th & 6th are normal, etc.*/
		if (fCalculateCTHDuringGunfire)	// Calculate number of bullets left right before firing this burst
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft + (pSoldier->bDoBurst - 1);
		else
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft;
		iRoundsFiredPreviously = GetMagSize(pInHand) - iBulletsLeft;	//How many rounds had been fired before this autofire sequence
		for( UINT8 cnt = 1; cnt <= pSoldier->bDoBurst; cnt++ ){	//How many tracers have been fired prior to this bullet in this autofire sequence
			if((iRoundsFiredPreviously+cnt)%iBulletsPerTracer == 0)
				iTracersFired++;
		}
		if ( iTracersFired > 0 ) {
			if(sTotalAutofirePenalty > 0){
				INT16 sChanceLimit = min(gGameExternalOptions.ubCTHBumpPerTracer, ((iRange-(gGameExternalOptions.ubMinRangeTracerEffect*CELL_X_SIZE)) / CELL_X_SIZE) * gGameExternalOptions.ubRangeDifficultyAimingWithTracers);
				sChanceLimit = max(0, sChanceLimit);
				iBonus = sChanceLimit * iTracersFired;
				iBonus = min(iBonus, sTotalAutofirePenalty);
			}

			iChance += iBonus;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Modify for Aim
	if(ubAimTime)
	{
		// CHRISL: Rather then a flat +10/click bonus, we're going to try a bonus that's based on MRK and Lvl which gets
		//	progressivly less the more we aim.  Everything is based on the maxBonus that a merc can possibly get which
		//	uses the equation: 20+(MRK/20*LVL)+Accuracy+(Sniper trait * 10).  This value is then split between the 8
		//	possible AimTime's using a max aimTime bonus of 10.

		// Flugente: If overheating is allowed, an overheated gun receives a slight malus to accuracy
		FLOAT accuracyheatmultiplicator = 1.0;
		if ( gGameExternalOptions.fWeaponOverheating )
		{
			FLOAT overheatdamagepercentage = GetGunOverheatDamagePercentage( pInHand );
			FLOAT accuracymalus = (FLOAT)((max(1.0, overheatdamagepercentage) - 1.0) * 0.1);
			accuracyheatmultiplicator = (FLOAT)max(0.0, 1.0 - accuracymalus);
		}

		if ( gGameOptions.fNewTraitSystem ) 
		{
			// bonus to snipers and gunslingers
			if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
				maxBonus = 20+((FLOAT)iMarksmanship/20*(EffectiveExpLevel(pSoldier)))+(accuracyheatmultiplicator * Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT )*gSkillTraitValues.ubGSAimingBonusPerClick);
			else
				maxBonus = 20+((FLOAT)iMarksmanship/20*(EffectiveExpLevel(pSoldier)))+(accuracyheatmultiplicator * Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )*gSkillTraitValues.ubSNAimingBonusPerClick);
		} 
		else 
		{			
			if(gGameExternalOptions.bAltAimEnabled) 
				maxBonus = (20*(iMarksmanship/100))+((FLOAT)iMarksmanship/20*pSoldier->stats.bExpLevel)+(accuracyheatmultiplicator * Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )*10);
			else 
				maxBonus = 20+((FLOAT)iMarksmanship/20*pSoldier->stats.bExpLevel)+(accuracyheatmultiplicator * Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )*10);
		}
		iAimBonus = (float)GetAimBonus( pSoldier, pInHand, 100, 1 );

		// penalty when aiming from alternative weapon holding
		if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
			iAimBonus = iAimBonus * (100 - gGameExternalOptions.ubAltWeaponHoldingAimingPenaly) / 100;

		for(int i = 0; i < ubAimTime; i++) {
			aimTimeBonus = __min((maxBonus*bonusProgression[i]/1000),maxClickBonus);
			maxBonus -= aimTimeBonus;
			iChance += (INT32)floor(aimTimeBonus+.5);
			//CHRISL: This is a slight extra bonus granted from a scopes AimBonus score
			aimTimeBonus = __min(ceil((float)(iAimBonus * bonusProgression[i] / 1000)), 3);
			iAimBonus -= aimTimeBonus;
			iChance+= (INT32)floor(aimTimeBonus+.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// AI Modifiers
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) )	// if this is a computer AI controlled enemy
	{
		if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		{
			// On easy, penalize all enemies by 5%
			iChance -= 5;
		}
		else
		{
			iChance += __min( 0, gbDiff[ DIFF_ENEMY_TO_HIT_MOD ][ SoldierDifficultyLevel( pSoldier ) ] );
		}
	}
	// SANDRO - Bonus CtH for Militia
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sGreenMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sRegularMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sVeteranMilitiaCtHBonusPercent) /100);
	// bonus for enemy
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyAdminCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyRegularCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyEliteCtHBonusPercent) /100);
	// SANDRO - option to make special NPCs stronger - chance to hit
	if (gGameExternalOptions.usSpecialNPCStronger > 0)
	{
		switch( pSoldier->ubProfile )
		{
			case CARMEN:
			case QUEEN:
			case JOE:
			case ANNIE:
			case CHRIS:
			case KINGPIN:
			case TIFFANY:
			case T_REX:
			case DRUGGIST:
			case GENERAL:
			case JIM:
			case JACK:
			case OLAF:
			case RAY:
			case OLGA:
			case TYRONE:
			case MIKE:
				iChance += (iChance * gGameExternalOptions.usSpecialNPCStronger / 100);
				break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Situational modifiers
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	  iChance -= AIM_PENALTY_GASSED;
	if (pSoldier->ubServiceCount > 0)
	  iChance -= AIM_PENALTY_GETTINGAID;
	if (pSoldier->aiData.bShock)
	{
		if (gGameExternalOptions.usMaxShooterCoweringPenalty > 0)
		{
			if ( usShockPenalty > gGameExternalOptions.usMaxShooterCoweringPenalty )
				usShockPenalty = gGameExternalOptions.usMaxShooterCoweringPenalty;
		}
		if ( usShockPenalty < 1 )
			usShockPenalty = 1;

		iChance -= usShockPenalty;
	}
	// adjust for roof/not on roof
	if ( pSoldier->pathing.bLevel == 0 )
	{
		if ( pSoldier->bTargetLevel > 0 )
		{
			// penalty for firing up
			iChance -= AIM_PENALTY_FIRING_UP;
		}
	}
	else // pSoldier->pathing.bLevel > 0 )
	{
		if ( pSoldier->bTargetLevel == 0 )
		{
			iChance += AIM_BONUS_FIRING_DOWN;
		}
	}
	// IF CHANCE EXISTS, BUT SHOOTER IS INJURED
	if ((iChance > 0) && (pSoldier->stats.bLife < pSoldier->stats.bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

		// injury penalty is based on % damage taken (max 2/3rds chance)
		iPenalty = (iChance * 2 * (pSoldier->stats.bLifeMax - pSoldier->stats.bLife + (bBandaged / 2))) /
						(3 * pSoldier->stats.bLifeMax);

		// reduce injury penalty due to merc's experience level (he can take it!)
		iChance -= (iPenalty * (100 - (10 * ( EffectiveExpLevel( pSoldier ) - 1)))) / 100;
	}

	// IF CHANCE EXISTS, BUT SHOOTER IS LOW ON BREATH
	if ((iChance > 0) && (pSoldier->bBreath < 100))
	{
		// breath penalty is based on % breath missing (max 1/2 chance)
		iPenalty = (iChance * (100 - pSoldier->bBreath)) / 200;
		// reduce breath penalty due to merc's dexterity (he can compensate!)
		iChance -= (iPenalty * (100 - ( EffectiveDexterity( pSoldier, FALSE ) - 10))) / 100;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Equipment Modifiers
	iChance += GetGearToHitBonus ( pSoldier );

	INT16 moda = GetToHitBonus( pInHand, iRange, bLightLevel, stance && iRange > MIN_PRONE_RANGE );
	INT16 modb = GetToHitBonus( pInHand, iRange, bLightLevel, gAnimControl[ pSoldier->usAnimState ].ubEndHeight && iRange > MIN_PRONE_RANGE );
	iChance += (INT32)((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Suppression modifiers
	if ( gGameExternalOptions.ubAimPenaltyPerTargetShock > 0 )
	{
		if (pTarget != NULL)
		{
			if (pTarget->aiData.bShock )
			{
				if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
				{
					ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorProne;

					sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
					sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
					sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

					if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
					{
						if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
							sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
					}
					if ( sCoweringPenalty < 1 )
						sCoweringPenalty = 1;

					iChance -= sCoweringPenalty;
				}
				else if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_CROUCH) 
				{
					switch ( ubAimPos )
					{
						case AIM_SHOT_HEAD:
							ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedHead;
							break;
						case AIM_SHOT_TORSO:
						case AIM_SHOT_RANDOM:
						case AIM_SHOT_GLAND:
							ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedTorso;
							break;
						case AIM_SHOT_LEGS:
							ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedLegs;
							break;
					}

					sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
					sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
					sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

					if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
					{
						if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
							sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
					}
					if ( sCoweringPenalty < 1 )
						sCoweringPenalty = 1;

					// HEADROCK HAM B2.8.1: Added formula to make sure that cowering target penalties
					// are not given when on the roof.

					if (pSoldier->pathing.bLevel == pTarget->pathing.bLevel && pSoldier->pathing.bLevel > 0)
						sCoweringPenalty = 0; // No cowering penalties when both are on the roof!
					else if (pSoldier->pathing.bLevel < pSoldier->pathing.bLevel && gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
						sCoweringPenalty *= 2; // Much harder to shoot at anyone cowering above you.
					else if (pSoldier->pathing.bLevel > pSoldier->pathing.bLevel)
						sCoweringPenalty /= 2; // Much easier to shoot at cowerers below you.
					iChance -= sCoweringPenalty;
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Max range modifier.  This needs to be here, near the end of the function to
	// guarantee that the max range penalty is properly calculated.
	/////////////////////////////////////////////////////////////////////////////////////
	if ( iRange > iMaxRange )
	{
		// a bullet WILL travel that far if not blocked, but it's NOT accurate,
		// because beyond maximum range, the bullet drops rapidly

		// This won't cause the bullet to be off to the left or right, only make it
		// drop in flight.
		//iChance /= 2; //ddd
		iChance = (INT32)(iChance / gGameExternalOptions.fOutOfGunRangeOrSight);
	}
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// Adjust for Max and Min values
	// HEADROCK: (HAM) Altered so called "Defined Limits" to accept external arguments.
	// The divisor argument only works when the minimum is set to 0. It has a chance of 1 in X to
	// bump the minimum back to 1, where X = the Divisor value. So a divisor value of 50 gives a 1/50
	// chance of getting some actual chance to hit despite the 0 minimum. The overall total would then
	// be an effective CTH of only 1/5000 (50 chances to get a 1 out of 100 CTH, hehehe)
	if (iChance <= gGameExternalOptions.ubMinimumCTH)
	{
		if ( TANK( pSoldier ) )
		{
			// allow absolute minimums
			iChance = 0;
		}
		else
		{
			iChance = gGameExternalOptions.ubMinimumCTH;
			if ( gGameExternalOptions.ubMinimumCTH == 0 )
			{
				if ( PreRandom( gGameExternalOptions.usMinimumCTHDivisor ) == (gGameExternalOptions.usMinimumCTHDivisor - 1) && fCalculateCTHDuringGunfire == TRUE )//dnl ch58 271009 Any Random function must not be used during calculation, only allow when you actually FireGun because AI became confused and sometimes do nothing
				{
					iChance = 1;
				}
			}
		}
	}
	else
	{
		// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
		// if (iChance > MAXCHANCETOHIT)
		// iChance = MAXCHANCETOHIT;

		// Flugente: backgrounds
		iChance =  min(iChance, min(100, gGameExternalOptions.ubMaximumCTH + (UINT8)(pSoldier->GetBackgroundValue(BG_PERC_CTH_MAX))) );
	}
	/////////////////////////////////////////////////////////////////////////////////////
	
   DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: ichance = %d",iChance));
   return (iChance);
	/*CHRISL: I've tried to sort and reorganize this function to make it a little easier to follow.  All of this is the original function
  //SOLDIERTYPE *vicpSoldier;
	SOLDIERTYPE * pTarget;
	INT32	iChance;	//CTH
	INT32	iRange;		//Actual range to target
	INT32	iSightRange;	//LOS range
	INT32	iTileRange;		//LOS range to the tile target is in - in case we can't see the target
	INT32	iVisionRange;	//Effective visual range
	INT32	iMaxRange;	//Weapon maximum range
	INT32	iGunCondition;	//Weapons effective condition
	INT32	iMarksmanship;	//Mercs effective marksmanship
	INT32	iBonus, iPenalty;
	//rain
	//INT32 iScopeBonus;
	//end rain
	UINT16	usInHand;
	OBJECTTYPE * pInHand;
//	INT8 bAttachPos;
	INT8	bBandaged;
	INT16	sDistVis;
	UINT8	ubAdjAimPos;
	UINT8	ubTargetID;
	bool	highPowerScope = false;
	UINT32	pScope;
	UINT8	bLightLevel = LightTrueLevel(sGridNo, pSoldier->bTargetLevel);

	// HEADROCK HAM 3.5: Variable holds total autofire penalty.
	INT16 sTotalAutofirePenalty = 0;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun"));

	if ( pSoldier->stats.bMarksmanship == 0 )
	{
		// HEADROCK: (HAM) Altered to accept external arguments
		// return( MINCHANCETOHIT );
		return( gGameExternalOptions.ubMinimumCTH );
	}

	// make sure the guy's actually got a weapon in his hand!
	pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);
	usInHand = pSoldier->usAttackingWeapon;
	//CHRISL: We have new modifiers when using a high power scope so find out if we have one from the start
	for(attachmentList::iterator iter = (*pInHand)[0]->attachments.begin(); iter != (*pInHand)[0]->attachments.end(); iter++)
	{
		if(Item[iter->usItem].aimbonus >= gGameExternalOptions.sHighPowerScope && iter->exists())
		{
			highPowerScope = true;
			pScope = iter->usItem;
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// DETERMINE BASE CHANCE OF HITTING
	iGunCondition = WEAPON_STATUS_MOD( (*pInHand)[0]->data.gun.bGunStatus );

	if (Item[usInHand].rocketlauncher )
	{
		// use the same calculation as for mechanical thrown weapons
		iMarksmanship = ( EffectiveDexterity( pSoldier ) + EffectiveMarksmanship( pSoldier ) + EffectiveWisdom( pSoldier ) + (10 * EffectiveExpLevel( pSoldier ) )) / 4;
		// heavy weapons trait helps out
		if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPS_OT ) && !( gGameOptions.fNewTraitSystem )) // SANDRO - old/new traits
		{
			iMarksmanship += gbSkillTraitBonus[HEAVY_WEAPS_OT] * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT );
		}
	}
	else
	{
		iMarksmanship = EffectiveMarksmanship( pSoldier );

		if ( AM_A_ROBOT( pSoldier ) )
		{
			SOLDIERTYPE * pSoldier2;

			pSoldier2 = pSoldier->GetRobotController( );
			if ( pSoldier2 )
			{
				iMarksmanship = __max( iMarksmanship, EffectiveMarksmanship( pSoldier2 ) );
			}
		}
	}

	// modify chance to hit by morale
	iMarksmanship += GetMoraleModifier( pSoldier );

	// penalize marksmanship for fatigue
	iMarksmanship -= GetSkillCheckPenaltyForFatigue( pSoldier, iMarksmanship );

	//if ( PTR_OURTEAM )
	//{
	//	iChance = ( iMarksmanship * iGunCondition ) / 100;
	//}
	//else
	{
		if (iGunCondition >= iMarksmanship)
			// base chance is equal to the shooter's marksmanship skill
			iChance = iMarksmanship;
		else
			// base chance is equal to the average of marksmanship & gun's condition!
			iChance = (iMarksmanship + iGunCondition) / 2;
	}
	/////////////////////////////////////////////////////////////////////////////////////

	// if shooting same target as the last shot
	if (sGridNo == pSoldier->sLastTarget )
		iChance += AIM_BONUS_SAME_TARGET;		// give a bonus to hit

	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - STOMP traits
	////////////////////////////////////////////////////
	// For every type of gun, there was added a penalty for untrained mercs. This penalty 
	// is rather small and is often compensated by single trait of that type
	if( gGameOptions.fNewTraitSystem )
	{
		// Bonus for heavy weapons moved here from above to get instant CtH bonus and not marksmanship bonus, 
		// which is supressed by weapon condition
		if (Item[usInHand].rocketlauncher || Item[usInHand].singleshotrocketlauncher)
		{
			iChance += gSkillTraitValues.bCtHModifierRocketLaunchers; // -25% for untrained mercs !!!

			if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ))
				iChance += gSkillTraitValues.ubHWBonusCtHRocketLaunchers * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ); // +25% per trait
		}
		// Added CtH bonus for Gunslinger trait on pistols and machine-pistols
		else if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL )
		{
			iChance += gSkillTraitValues.bCtHModifierPistols; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) && pSoldier->bDoBurst == 0 && pSoldier->bDoAutofire == 0 )
				iChance += gSkillTraitValues.ubGSBonusCtHPistols * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ); // +10% per trait
		}
		else if ( Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
		{
			iChance += gSkillTraitValues.bCtHModifierMachinePistols; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( pSoldier, GUNSLINGER_NT ) && ((pSoldier->bDoBurst == 0 && pSoldier->bDoAutofire == 0) || !gSkillTraitValues.ubGSCtHMPExcludeAuto))
				iChance += gSkillTraitValues.ubGSBonusCtHMachinePistols * NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT ); // +5% per trait
		}
		// Added CtH bonus for Machinegunner skill on assault rifles, SMGs and LMGs
		else if ( Weapon[usInHand].ubWeaponType == GUN_AS_RIFLE )
		{
			iChance += gSkillTraitValues.bCtHModifierAssaultRifles; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
				iChance += gSkillTraitValues.ubAWBonusCtHAssaultRifles * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT ); // +5% per trait
		}
		else if ( Weapon[usInHand].ubWeaponType == GUN_SMG ) 
		{
			iChance += gSkillTraitValues.bCtHModifierSMGs; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
				iChance += gSkillTraitValues.ubAWBonusCtHSMGs * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT ); // +5% per trait
		}
		else if ( Weapon[usInHand].ubWeaponType == GUN_LMG )
		{
			iChance += gSkillTraitValues.bCtHModifierLMGs; // -10% for untrained mercs.

			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
				iChance += gSkillTraitValues.ubAWBonusCtHLMGs * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT ); // +5% per trait
		}
		// Added CtH bonus for Gunslinger trait on pistols and machine-pistols
		else if ( Weapon[usInHand].ubWeaponType == GUN_SN_RIFLE )
		{
			iChance += gSkillTraitValues.bCtHModifierSniperRifles; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) && pSoldier->bDoBurst == 0 && pSoldier->bDoAutofire == 0 )
				iChance += gSkillTraitValues.ubSNBonusCtHSniperRifles * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ); // +5% per trait
		}
		// Added CtH bonus for Ranger skill on rifles and shotguns
		else if ( Weapon[usInHand].ubWeaponType == GUN_RIFLE ) 
		{
			iChance += gSkillTraitValues.bCtHModifierRifles; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) && pSoldier->bDoBurst == 0 && pSoldier->bDoAutofire == 0 )
				iChance += gSkillTraitValues.ubRABonusCtHRifles * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ); // +5% per trait
			//CHRISL: Why wouldn't sniper training include standard rifles which are often used as "poor-man sniper rifles"
			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) && pSoldier->bDoBurst == 0 && pSoldier->bDoAutofire == 0 )
				iChance += gSkillTraitValues.ubSNBonusCtHRifles * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT ); // +5% per trait
		}
		else if ( Weapon[usInHand].ubWeaponType == GUN_SHOTGUN )
		{
			iChance += gSkillTraitValues.bCtHModifierShotguns; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
				iChance += gSkillTraitValues.ubRABonusCtHShotguns * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ); // +10% per trait
		}

		// Added small CtH penalty for robot if controller hasn't the Technician trait
		if( AM_A_ROBOT( pSoldier ) )
		{
			iChance += gSkillTraitValues.bCtHModifierRobot; // -10% 

			if ( HAS_SKILL_TRAIT( pSoldier->GetRobotController(), TECHNICIAN_NT ) )
				iChance += gSkillTraitValues.ubTECtHControlledRobotBonus * NUM_SKILL_TRAITS( pSoldier->GetRobotController(), TECHNICIAN_NT ); // +10% per trait
		}

		// Added character traits influence
		if ( pSoldier->ubProfile != NO_PROFILE )
		{		
			// Sociable - better performance in groups
			if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_SOCIABLE )
			{	
				INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_SOCIABLE );
				if ( bNumMercs > 2 )
					iChance += 5;
				else if ( bNumMercs > 0 )
					iChance += 2;
			}
			// Loner - better performance when alone
			else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_LONER )
			{	
				INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_LONER );
				if ( bNumMercs == 0 )
					iChance += 5;
				else if ( bNumMercs <= 1 )
					iChance += 2;
			}
			// Aggressive - bonus on bursts/autofire
			else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
			{	
				if (( pSoldier->bDoBurst || pSoldier->bDoAutofire ) && !ubAimTime )
					iChance += 5;
			}
			// Show-off - better performance if some babes around to impress
			else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_SHOWOFF )
			{	
				INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_SHOWOFF );
				if ( bNumMercs > 1 )
					iChance += 5;
				else if ( bNumMercs > 0 )
					iChance += 2;
			}
			// Added disabilities
			if ( pSoldier->ubProfile != NO_PROFILE )
			{
				// Heat intolerant penalty
				if ( MercIsHot( pSoldier ) )
				{
					iChance -= 15;
				}
				// Small penalty for fear of insects in tropical sectors
				else if ( (gMercProfiles[ pSoldier->ubProfile ].bDisability == FEAR_OF_INSECTS) && MercIsInTropicalSector( pSoldier ) )
				{
					// fear of insects, and we are in tropical sector
					iChance -= 5;
				}
			}
		}
		// Dauntless - penalty for not taking proper cover
		pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
		if (pTarget != NULL)
		{
			if ( pTarget->ubProfile != NO_PROFILE )
			{
				if ( gMercProfiles[ pTarget->ubProfile ].bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
					iChance += 5;
			}
		}
	}
	else
	{
		// this rather unlogical bonus for psychotic characters applies only with old traits
		if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO )
		{
			iChance += AIM_BONUS_PSYCHO;
		}
	}

	// calculate actual range (in units, 10 units = 1 tile)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

	//CHRISL: Incur a penalty if using a highPowerScope
	if(highPowerScope == true && iRange > Item[pScope].minrangeforaimbonus)
	{
		iPenalty = (Item[pScope].aimbonus * (iRange-Item[pScope].minrangeforaimbonus)) / 1000;
		if (iPenalty > AIM_BONUS_PRONE)
		{
			iPenalty = AIM_BONUS_PRONE;
		}
		iChance -= iPenalty;
	}
	// if shooter is crouched, he aims slightly better (to max of AIM_BONUS_CROUCHING)
	if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH )
	{
		iBonus = iRange / 10;
		if (iBonus > AIM_BONUS_CROUCHING)
		{
			iBonus = AIM_BONUS_CROUCHING;
		}
		iChance += iBonus;
	}
	// if shooter is prone, he aims even better, except at really close range
	else if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
	{
		if (iRange > MIN_PRONE_RANGE)
		{
			iBonus = iRange / 10;
			if (iBonus > AIM_BONUS_PRONE)
			{
				iBonus = AIM_BONUS_PRONE;
			}
			// Snap: bipod bonus is now calculated in GetToHitBonus
			//bAttachPos = FindAttachment( pInHand, BIPOD );
			//if (bAttachPos != ITEM_NOT_FOUND)
			//{	// extra bonus to hit for a bipod, up to half the prone bonus itself
			//	iBonus += (iBonus * WEAPON_STATUS_MOD(GetBipodStatus(pInHand)) / 100) / 2;
			//}
			iChance += iBonus;
		}
	}

	//Madd: inherent weapon accuracy bonus
	iChance += Weapon[usInHand].bAccuracy;

//	if ( !(Item[ usInHand ].fFlags & ITEM_TWO_HANDED) )
	if ( !(Item[ usInHand ].twohanded ) )
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() == false)
		{
			// firing with gun in right hand, and second hand empty (ie: no grenade, med kit or anything there)
			// Madd: easier to fire pistol/smg w/one hand free, essentially this will make pistols a little bit more accurate, and hopefully still useful later in the game
			if (Weapon[usInHand].ubWeaponClass == HANDGUNCLASS)
				iChance += AIM_BONUS_TWO_HANDED_PISTOL;
		}
		else
		{
			// Madd: harder to fire smgs w/something in other hand
			// SMGs are treated as pistols for these purpose except there is a -5 penalty;
			if (Weapon[usInHand].ubWeaponClass == SMGCLASS)
			{
				iChance -= AIM_PENALTY_SMG;
			}

			//////////////////////////////////////////////////////////////////
			// SANDRO - old/new traits
			if (gGameOptions.fNewTraitSystem)
			{
				// Okay, we will shoot from both weapons, calculate penalty to CtH
				if ( pSoldier->IsValidSecondHandShot() )
				{					
					// penaly to shoot from dual weapons
					iChance -= (INT32)(gSkillTraitValues.ubCtHPenaltyDualShot * (HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ) ? ((100 - gSkillTraitValues.ubAMPenaltyDoubleReduction)/100) : 1 ));
				}
			}
			else // original code
			{
				if ( !HAS_SKILL_TRAIT( pSoldier, AMBIDEXT_OT ) )
				{
					if ( pSoldier->IsValidSecondHandShot( ) )
					{
						// penalty to aim when firing two pistols
						iChance -= AIM_PENALTY_DUAL_PISTOLS;
					}
//					else
//					{
//						// penalty to aim with pistol being fired one-handed
//						iChance -= AIM_PENALTY_ONE_HANDED_PISTOL;
//					}
				}
			}
			//////////////////////////////////////////////////////////////////
		}
	}

	// If in burst mode, deduct points for chance to hit for each shot after the first
	if ( pSoldier->bDoBurst && pSoldier->bDoAutofire == 0 )
	{
//		CHRISL: At this point in the calculation, Bipods, Foregrips and other "recoil stabalizing" bonuses should reduce the weapons "recoil penalty"
//			(BurstPenalty) by a percentage.  This reduction should apply the the recoil per shot and not as a "flat" initial reduction.  Later in the code,
//			Bipods also grant their flat CTH bonus so we don't need to mess this that here
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
		{
			iBonus = GetBurstToHitBonus(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			iPenalty = Weapon[pInHand->usItem].ubBurstPenalty * (pSoldier->bDoBurst - 1) * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
			iPenalty = max(0, (iPenalty * (100 - iBonus))/100 );
		}
		else
		{
			// Snap: bipod may reduce burst penalty
			iPenalty = GetBurstPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			if(gGameExternalOptions.bAimedBurstEnabled)
				iPenalty += Weapon[usInHand].ubBurstPenalty * (pSoldier->bDoBurst - 1) * gGameExternalOptions.uAimedBurstPenalty;
		}

		// halve the penalty for people with the autofire trait
		//////////////////////////////////////////////////////////////////
		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
			{
				iPenalty = max(0, ((iPenalty * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
			{
				iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
			}
		}
		//////////////////////////////////////////////////////////////////
		iChance -= iPenalty;
	}
	else if ( pSoldier->bDoAutofire > 0 )
	{
//		CHRISL: At this point in the calculation, Bipods, Foregrips and other "recoil stabalizing" bonuses should reduce the weapons "recoil penalty"
//			(AutoPenalty) by a percentage.  This reduction should apply the the recoil per shot and not as a "flat" initial reduction.  Later in the code,
//			Bipods also grant their flat CTH bonus so we don't need to mess this that here
		if(gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier)
		{
			iBonus = GetAutoToHitBonus(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * gGameExternalOptions.ubFlatAFTHBtoPrecentMultiplier;
			iPenalty = Weapon[pInHand->usItem].AutoPenalty * (pSoldier->bDoBurst - 1) * (gGameExternalOptions.bAimedBurstEnabled?gGameExternalOptions.uAimedBurstPenalty:1);
			iPenalty = max(0, (iPenalty * (100 - iBonus))/100 );
		}
		else
		{
			// Snap: bipod may reduce auto penalty
			iPenalty = GetAutoPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * (pSoldier->bDoBurst - 1);
			if(gGameExternalOptions.bAimedBurstEnabled)
				iPenalty += Weapon[usInHand].AutoPenalty * (pSoldier->bDoBurst - 1) * gGameExternalOptions.uAimedBurstPenalty;
		}

		// halve the penalty for people with the autofire trait
		//////////////////////////////////////////////////////////////////
		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
			{
				iPenalty = max(0, ((iPenalty * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
			{
				iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
			}
		}
		//////////////////////////////////////////////////////////////////

		iChance -= iPenalty;
		// HEADROCK HAM 3.5: Store the penalty
		sTotalAutofirePenalty = iPenalty;
	}

	//ADB we need to calculate the distance visible and SoldierTo...LOSTests that we want to
	//calculate it when firing, so the scope can be accounted for when the weapon is raised
	//I consider this a hack to change the global, but passing a var alllll the way down to GetVisionBonus was worse
	//if anyone wants to change it go for it, I don't know what usAnimState should be set

	//store old flag
	UINT32 oldFlag = gAnimControl[ pSoldier->usAnimState ].uiFlags;
	//add the necessary bits to the flag
	gAnimControl[ pSoldier->usAnimState ].uiFlags |= (ANIM_FIREREADY | ANIM_FIRE);

	// 0verhaul:  Changed to take expanded range from shooting at different levels into account
	//ADB this change does nothing - either way it is random - we don't know what level we are shooting to, which is
	//what the last parameter is, and the soldier's current level is as good a guess as ground level.
	//so if you really want to fix this, pass in a value
	sDistVis = pSoldier->GetMaxDistanceVisible(sGridNo, pSoldier->bTargetLevel, CALC_FROM_ALL_DIRS );

	// CJC August 13 2002:  Wow, this has been wrong the whole time.  bTargetCubeLevel seems to be generally set to 2 -
	// but if a character is shooting at an enemy in a particular spot, then we should be using the target position on the body.

	// CJC August 13, 2002
	// If the start soldier has a body part they are aiming at, and know about the person in the tile, then use that height instead
	iSightRange = -1;

	// give some leeway to allow people to spot for each other...
	//so make the range to calculate the bullet at 255+
	ubTargetID = WhoIsThere2( sGridNo, pSoldier->bTargetLevel );
	// best to use team knowledge as well, in case of spotting for someone else
	// 0verhaul:  Why not use the distance visible as the max for line of sight testing?
	//ADB because A) the bullet can travel farther than I can see and B) I might have a spotter

	//We are firing a gun, and so the gun will be pointed and the scope will be used, even if it isn't now, so don't forget that we are in a firing animation
	if (ubTargetID != NOBODY && pSoldier->aiData.bOppList[ubTargetID] == SEEN_CURRENTLY || gbPublicOpplist[pSoldier->bTeam][ubTargetID] == SEEN_CURRENTLY)
	{
		iSightRange = SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[ubTargetID], TRUE, NO_DISTANCE_LIMIT, pSoldier->bAimShotLocation, false );
	}
	//CHRISL: This is useful if we're trying to target an opponent we can't actually see
	iTileRange = SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pSoldier->bTargetLevel, pSoldier->bTargetCubeLevel, TRUE, NO_DISTANCE_LIMIT, false );
	if (iSightRange == -1) // didn't do a bodypart-based test
	{
		iSightRange = iTileRange;
	}
	// WANNE: Changed this, because RPGs are not in the calculation, only guns
	//if ( Item[ usInHand ].usItemClass == IC_GUN )
	if ( Item[ usInHand ].usItemClass == IC_GUN || Item[ usInHand ].usItemClass == IC_LAUNCHER)
	{
		iMaxRange = GunRange( pInHand, pSoldier ); // SANDRO - added argument
	}
	else
	{
		iMaxRange = CELL_X_SIZE; // one tile
	}

	if(iSightRange > 0){
		//Madd: Some gear can affect the to-hit bonus and the bonus from aiming
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: before bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));
		//CHRISL: This is supposed to be a percentage increase/decrease
		//iSightRange -= GetGearAimBonus ( pSoldier, iRange, ubAimTime );
		iSightRange -= GetGearAimBonus ( pSoldier, iRange, ubAimTime ) * iSightRange / 100;
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after gear bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));

		//CHRISL: We want our AimBonus to be based on our visual range, not our actual range
		// Reduce effective range by the aiming bonus (e.g. from sniper scope)
		//iSightRange -= GetAimBonus( pInHand, iRange, ubAimTime );
		iSightRange -= GetAimBonus( pInHand, iSightRange, ubAimTime );
		//CHRISL: If we have the Sniper skill, reduce effective range
		//////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if ( gGameOptions.fNewTraitSystem )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, SNIPER_NT ) )
			{
				iSightRange -= ((gSkillTraitValues.ubSNEffRangeToTargetReduction * NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )) * iSightRange) /100;
			}
		}
		else if ( HAS_SKILL_TRAIT( pSoldier, PROF_SNIPER_OT ) )
		{
			iSightRange -= ((gbSkillTraitBonus[ PROF_SNIPER_OT ] * NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )) * iSightRange) /100;
		}
		//////////////////////////////////////////////////////////////////
		if (iSightRange < 1)
		{
			iSightRange = 1;
		}
	}

	//restore old flag
	gAnimControl[ pSoldier->usAnimState ].uiFlags = oldFlag;

	//CHRISL: Because of the changes I've made to how scopes modify iSightRange, this penalty makes it basically impossible
	//	to shot at targets you can't see yourself.  While this isn't an issue for most weapons, it overly restricts the
	//	effectiveness of sniper rifles.  So pull this penalty but leave the one that comes later in the code.
	//if ( iSightRange > (sDistVis * CELL_X_SIZE) )
	//{
		// shooting beyond max normal vision... penalize such distance at double (also later we halve the remaining chance)
		//iSightRange += (iSightRange - sDistVis * CELL_X_SIZE);
	//}

	//CHRISL: Not being able to actually see the target should not restrict out ability to aim a weapon.  Aiming is more then just sighting a specific target.  It's
	//	also steadying the weapon and aiming where you think the target is.  We will eventually get hit with a serious "Can't See the Target" penalty and aiming is
	//	the only way to even attempt to counter that.  This is one reason for "Kevlar Grass"
	// if shooter spent some extra time aiming and can see the target
	//if (iSightRange > 0 && ubAimTime && !pSoldier->bDoBurst ) //comm by ddd
	//if (iSightRange > 0 && ubAimTime )
	//////////////////////////////////////////////////////////////////
	if(ubAimTime)
	{
		// CHRISL: Rather then a flat +10/click bonus, we're going to try a bonus that's based on MRK and Lvl which gets
		//	progressivly less the more we aim.  Everything is based on the maxBonus that a merc can possibly get which
		//	uses the equation: 20+(MRK/20*LVL)+Accuracy+(Sniper trait * 10).  This value is then split between the 8
		//	possible AimTime's using a max aimTime bonus of 10.
		INT16	bonusProgression[8] = {500,500,600,600,750,750,750,1000};
		//FLOAT	maxBonus = 20+((FLOAT)iMarksmanship/20*pSoldier->stats.bExpLevel)+(Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER )*10);

		//////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		FLOAT	maxBonus;
		if ( gGameOptions.fNewTraitSystem )
		{
			// bonus to snipers and gunslingers
			if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_PISTOL )
				maxBonus = 20+((FLOAT)iMarksmanship/20*(EffectiveExpLevel(pSoldier)))+(Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT )*gSkillTraitValues.ubGSAimingBonusPerClick);
			else
				maxBonus = 20+((FLOAT)iMarksmanship/20*(EffectiveExpLevel(pSoldier)))+(Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, SNIPER_NT )*gSkillTraitValues.ubSNAimingBonusPerClick);
		}
		else
		{			
			if(gGameExternalOptions.bAltAimEnabled) 
				maxBonus = (20*(iMarksmanship/100))+((FLOAT)iMarksmanship/20*pSoldier->stats.bExpLevel)+(Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )*10);
			else 
				maxBonus = 20+((FLOAT)iMarksmanship/20*pSoldier->stats.bExpLevel)+(Weapon[Item[pInHand->usItem].ubClassIndex].bAccuracy*2)+(NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT )*10);
		}
		//}ddd
		//////////////////////////////////////////////////////////////////
		INT8	maxClickBonus = 10;
		FLOAT	aimTimeBonus;
		for(int i = 0; i < ubAimTime; i++)
 		{
			aimTimeBonus = __min((maxBonus*bonusProgression[i]/1000),maxClickBonus);
			maxBonus -= aimTimeBonus;
			iChance += (INT32)floor(aimTimeBonus+.5);
		}
		///ddd{
		//zilpin: aimed burst
		//Reduce the aim time bonus based on weapon burst penalty, after the first shot.
		//The burst penalty was already applied, but it also reduces the benefit of aiming.
//		CHRISL: This is actaully applying the weapons AutoPenalty a second time.  We already apply AutoPenalty/round (after the first) in the autofire
//			section earlier in this code.  We shouldn't be applying this again here.  Plus, this modification should occur regardless of whether we aim
//			or not.  I've moved this condition to the burst/autofire portion of the code above.
//		if(gGameExternalOptions.bAimedBurstEnabled)
//		{
//			//if( pSoldier->bDoAutofire )
//			//	iChance -= Weapon[usInHand].AutoPenalty * pSoldier->bDoBurst * gGameExternalOptions.uAimedBurstPenalty;
//			//else if( pSoldier->bDoBurst>1 )
//			//	iChance -= Weapon[usInHand].ubBurstPenalty * pSoldier->bDoBurst * gGameExternalOptions.uAimedBurstPenalty;
//			if( pSoldier->bDoAutofire>0 && pSoldier->bDoBurst>1)
//				iChance -= Weapon[usInHand].AutoPenalty * pSoldier->bDoBurst * gGameExternalOptions.uAimedBurstPenalty;
//			//if( pSoldier->bDoAutofire>10 )
//			//	iChance -= Weapon[usInHand].AutoPenalty * pSoldier->bDoBurst * gGameExternalOptions.uAimedBurstPenalty;
//			//else 
//				if( pSoldier->bDoBurst>1  && pSoldier->bDoAutofire==0)
//				iChance -= Weapon[usInHand].ubBurstPenalty * pSoldier->bDoBurst * gGameExternalOptions.uAimedBurstPenalty;
//
//		}
		///ddd}

//		if(highPowerScope == true)
//		{
//		{
//			if ( NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER ) == 2 )
//				aimTimeBonus = -((__max(0,75-pSoldier->stats.bMarksmanship))/(pSoldier->stats.bExpLevel*2));
//			else if( NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER ) == 1 )
//				aimTimeBonus = -((__max(0,85-pSoldier->stats.bMarksmanship))/(pSoldier->stats.bExpLevel*2));
//			else
//				aimTimeBonus = -((__max(0,100-pSoldier->stats.bMarksmanship))/(pSoldier->stats.bExpLevel));
//			aimTimeBonus = __max(-9, aimTimeBonus);
//			aimTimeBonus = __min(0, aimTimeBonus);
//			aimTimeBonus += AIM_BONUS_PER_AP;
//		}
//		else
//			aimTimeBonus = AIM_BONUS_PER_AP;
//		iChance += (aimTimeBonus * ubAimTime); // bonus for every pt of aiming
	}
	//////////////////////////////////////////////////////////////////

	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) )	// if this is a computer AI controlled enemy
	{
		if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		{
			// On easy, penalize all enemies by 5%
			iChance -= 5;
		}
		else
		{
			// max with 0 to prevent this being a bonus, for JA2 it's just a penalty to make early enemies easy
			// CJC note: IDIOT!  This should have been a min.  It's kind of too late now...
			// CJC 2002-05-17: changed the max to a min to make this work.
			iChance += __min( 0, gbDiff[ DIFF_ENEMY_TO_HIT_MOD ][ SoldierDifficultyLevel( pSoldier ) ] );
		}
	}

	// if shooter is being affected by gas
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
	  iChance -= AIM_PENALTY_GASSED;
	}

	// if shooter is being bandaged at the same time, his concentration is off
	if (pSoldier->ubServiceCount > 0)
	  iChance -= AIM_PENALTY_GETTINGAID;

	// if shooter is still in shock
	if (pSoldier->aiData.bShock)
	{
		// HEADROCK HAM B2.8: Placed a maximum here, as shock is now also used in suppression.
		UINT16 usShockPenalty;

		usShockPenalty = pSoldier->aiData.bShock * AIM_PENALTY_PER_SHOCK;

		if (gGameExternalOptions.usMaxShooterCoweringPenalty > 0)
		{
			if ( usShockPenalty > gGameExternalOptions.usMaxShooterCoweringPenalty )
				usShockPenalty = gGameExternalOptions.usMaxShooterCoweringPenalty;
		}
		if ( usShockPenalty < 1 )
			usShockPenalty = 1;

		iChance -= usShockPenalty;
	}

	//CHRISL: Just because we can't see a target, doesn't mean these armor/helmet modifiers should be ignored
	iChance += GetGearToHitBonus ( pSoldier );

	if ( iSightRange > 0 )
	{
		//CHRISL: Moved iSightRange modifications to the range section above
		// SANDRO - DELETED POINTLESS UNUSED PARTS OF CODE

		// HEADROCK HAM B2.1 : The TARGET's shock now also affects aiming. If the target is prone
		// and in shock, they are harder to hit! This represents a target that's cowering as close
		// to the ground (and as close to any possible cover, like a small dune or a fold of earth
		// or anything like that).
		if ( gGameExternalOptions.ubAimPenaltyPerTargetShock > 0 )
		{
			// HEADROCK HAM B2.1 : This value determines how much penalty the target's shock-value gives the shooter.
			// As of HAM B2.3: There's a maximum range at which 100% penalty is given.
			// As of HAM B2.8: Target's stance and the targetted bodypart will affect the end result.
			
			INT8 AIM_PENALTY_PER_TARGET_SHOCK;
			INT16 sCoweringPenalty = 0;
			UINT8 ubCoweringDivisor;
			UINT16 MIN_RANGE_FOR_FULL_COWER;
			UINT16 MAX_TARGET_COWERING_PENALTY;

			AIM_PENALTY_PER_TARGET_SHOCK = gGameExternalOptions.ubAimPenaltyPerTargetShock;
			MIN_RANGE_FOR_FULL_COWER = gGameExternalOptions.usMinRangeForFullCoweringPenalty; 
			MAX_TARGET_COWERING_PENALTY = gGameExternalOptions.usMaxTargetCoweringPenalty;

			pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
			if (pTarget != NULL)
			{
				if (pTarget->aiData.bShock )
				{
					if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
					{
						ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorProne;

						sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
						sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
						sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

						if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
						{
							if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
								sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
						}
						if ( sCoweringPenalty < 1 )
							sCoweringPenalty = 1;

						iChance -= sCoweringPenalty;
					}
					else if (gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_CROUCH) 
					{
						switch ( ubAimPos )
						{
							case AIM_SHOT_HEAD:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedHead;
								break;
							case AIM_SHOT_TORSO:
							case AIM_SHOT_RANDOM:
							case AIM_SHOT_GLAND:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedTorso;
								break;
							case AIM_SHOT_LEGS:
								ubCoweringDivisor = gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedLegs;
								break;
						}

						sCoweringPenalty = (pTarget->aiData.bShock * AIM_PENALTY_PER_TARGET_SHOCK);
						sCoweringPenalty = sCoweringPenalty / ubCoweringDivisor;
						sCoweringPenalty = (sCoweringPenalty * __min(iSightRange,MIN_RANGE_FOR_FULL_COWER)) / MIN_RANGE_FOR_FULL_COWER;

						if (gGameExternalOptions.usMaxTargetCoweringPenalty > 0)
						{
							if ( sCoweringPenalty > gGameExternalOptions.usMaxTargetCoweringPenalty )
								sCoweringPenalty = gGameExternalOptions.usMaxTargetCoweringPenalty;
						}
						if ( sCoweringPenalty < 1 )
							sCoweringPenalty = 1;

						// HEADROCK HAM B2.8.1: Added formula to make sure that cowering target penalties
						// are not given when on the roof.

						if (pSoldier->pathing.bLevel == pTarget->pathing.bLevel && pSoldier->pathing.bLevel > 0)
							sCoweringPenalty = 0; // No cowering penalties when both are on the roof!
						else if (pSoldier->pathing.bLevel < pSoldier->pathing.bLevel && gAnimControl[ pTarget->usAnimState ].ubHeight == ANIM_PRONE)
							sCoweringPenalty *= 2; // Much harder to shoot at anyone cowering above you.
						else if (pSoldier->pathing.bLevel > pSoldier->pathing.bLevel)
							sCoweringPenalty /= 2; // Much easier to shoot at cowerers below you.
						iChance -= sCoweringPenalty;
					}
				}
			}
		}
	}

	//CHRISL: This should effect us regardless of whether we can see the target or not
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after scope bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));
	// Apply flat to-hit bonus (e.g. from laser scope)
	// Snap: If prone and range is good, apply bipod bonus here as well
	iChance += GetToHitBonus( pInHand, iRange, bLightLevel,gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && iRange > MIN_PRONE_RANGE );
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after to hit bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));

	//NumMessage("EFFECTIVE RANGE = ",range);

	// ADJUST FOR RANGE
	// bonus if range is less than normal range, penalty if it's more
	//iChance += (NORMAL_RANGE - iRange) / (CELL_X_SIZE / 5);	// 5% per tile

	// Effects of actual gun max range... the numbers are based on wanting -40%
	// at range 26for a pistol with range 13, and -0 for a sniper rifle with range 80
	iPenalty = ((iMaxRange - iRange * 3) * 10) / (17 * CELL_X_SIZE);
	if ( iPenalty < 0 )
	{
		// No penalty on tanks
		//if (!TANK(pSoldier))
			iChance += iPenalty;
	}
	//iChance -= 20 * iRange / iMaxRange;

	if ( TANK( pSoldier ) && ( iRange / CELL_X_SIZE < MaxNormalDistanceVisible() ) )
	{
		// tank; penalize at close range!
		// 2 percent per tile closer than max visible distance
		iChance -= 2 * ( MaxNormalDistanceVisible() - (iRange / CELL_X_SIZE) );
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// Target Size Penalties
	//if (iSightRange == 0)
	//{
		// firing blind!
		//iChance -= AIM_PENALTY_BLIND;
	//}
	if ( pSoldier->aiData.bOppList[ubTargetID] != SEEN_CURRENTLY &&(pSoldier->flags.uiStatusFlags & SOLDIER_PC) 	)
		iChance -=gGameExternalOptions.iPenaltyShootUnSeen;
	else
	{
		// Effects based on aiming & sight
		// From for JA2.5:  3% bonus/penalty for each tile different from range NORMAL_RANGE.
		// This doesn't provide a bigger bonus at close range, but stretches it out, making medium
		// range less penalized, and longer range more penalized

		// WANNE: No penalty on the tank
		if (!TANK(pSoldier))
			iChance += 3 * ( NORMAL_RANGE - iSightRange ) / CELL_X_SIZE;
		// if aiming at the head, reduce chance to hit
		if (ubAimPos == AIM_SHOT_HEAD)
		{
			// penalty of 3% per tile
			//iPenalty = 3 * iSightRange / 10; //comm by ddd
			iPenalty = INT32(gGameExternalOptions.uShotHeadPenalty * iSightRange / 10);
			iChance -= iPenalty;
		}
		else if (ubAimPos == AIM_SHOT_LEGS)
		{
			// penalty of 1% per tile
			iPenalty = iSightRange / 10;
			iChance -= iPenalty;
		}

//		if (iSightRange < NORMAL_RANGE)
//		{
//			// bonus to hit of 20% at point blank (would be 25% at range 0);
//			//at NORMAL_RANGE, bonus is 0
//			iChance += 25 * (NORMAL_RANGE - iSightRange) / NORMAL_RANGE;
//		}
//		else
//		{
//			// penalty of 2% / tile
//			iChance -= (iSightRange - NORMAL_RANGE) / 5;
//		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////
	// Tracer effects
	// HEADROCK HAM 3.5: Moved this here for now.
	// HEADROCK HAM B2.5: One of every X bullets in a tracer magazine is a tracer round, which will
	// bump the CTH up by a certain amount.

	if (AmmoTypes[(*pInHand)[0]->data.gun.ubGunAmmoType].tracerEffect == 1 && gGameExternalOptions.ubRealisticTracers > 0 )
	{
		UINT16 iBulletsLeft, iTracersFired, iBulletsPerTracer, iBulletsSinceLastTracer=0, iRoundsFiredPreviously;
		UINT8 cnt;
		UINT8 ubAutoPenaltySinceLastTracer=0;
		//UINT16 iAutoPenaltySinceLastTracer;
		iTracersFired = 0;
		iBulletsPerTracer = gGameExternalOptions.ubNumBulletsPerTracer;

		// Calculate number of bullets left right before firing this burst
		if (fCalculateCTHDuringGunfire)
		{
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft + (pSoldier->bDoBurst - 1);
		}
		else
		{
			iBulletsLeft = (*pInHand)[0]->data.gun.ubGunShotsLeft;
		}

//		CHRISL: What we want to know here is how many tracers have been fired during this autofire sequence.  But we also want to figure that based on how
//			much ammo we've used from the current load.  If every 3rd round is a tracer, and we previously fired 5 rounds from our "mag", then the first
//			round of this burst is a tracer, the 2nd & 3rd are normal, the 4th is a tracer, 5th & 6th are normal, etc.
		iRoundsFiredPreviously = GetMagSize(pInHand) - iBulletsLeft;	//How many rounds had been fired before this autofire sequence
		for( cnt = 1; cnt <= pSoldier->bDoBurst; cnt++ ){
			if((iRoundsFiredPreviously+cnt)%iBulletsPerTracer == 0)
				iTracersFired++;
		}

//		iBulletsSinceLastTracer = 0;
//		for (cnt=0;cnt<pSoldier->bDoBurst;cnt++)
//		{
//			iBulletsSinceLastTracer++;
//			if ((( iBulletsLeft - (cnt - 1) ) / iBulletsPerTracer) - ((iBulletsLeft - cnt) / iBulletsPerTracer) == 1)
//			{
//				iBulletsSinceLastTracer = 0;
//			}
//		}
//
//		iTracersFired = ((iBulletsLeft+1) / iBulletsPerTracer) - (((iBulletsLeft+1) - (pSoldier->bDoBurst)) / iBulletsPerTracer);

		if ( iTracersFired > 0 )
		{
			// HEADROCK HAM 3.5: I'm going to revise this - my current system makes no sense at all. What was I
			// thinking?!
//			 CHRISL: Headrock may come around later and revise this again, but for the time being, I'm making some adjustments.  With the current code
//				the number of tracer rounds fired doesn't really matter.  In most cases, the range is going to limit the max bonus so much that 1 tracer or
//				20 will have the same effect because we currently look at the "bump" value times the number of rouns and THEN modify for range.  What we
//				should be doing is modify the bump for range, multiply that by the number of tracer rounds fired, then make sure the bonus isn't more then
//				our combined autofire penalty.

			// Correct all autofire penalty so far
			//iBonus = iPenalty;
			
			// Add Tracer Bump if previous bullet was a tracer
			//iBonus += (gGameExternalOptions.ubCTHBumpPerTracer * iTracersFired);
			//iBonus = (gGameExternalOptions.ubCTHBumpPerTracer * iTracersFired);

			// Calculate penalty since last tracer was fired
			// CHRISL: We already look at GetAutoPenalty earlier in the code, and that result was stored as sTotalAutofirePenalty so why this wonky recalculation?
			//UINT8 ubAutoPenaltySinceLastTracer = GetAutoPenalty(pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) * iBulletsSinceLastTracer;
			//////////////////////////////////////////////////////////////////
//			if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
//			{
//				if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
//				{
//					ubAutoPenaltySinceLastTracer = max(0, ((ubAutoPenaltySinceLastTracer * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
//				}
//			}
//			else
//			{
//				if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
//				{
//					ubAutoPenaltySinceLastTracer /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
//				}
//			}	
//			//////////////////////////////////////////////////////////////////	
//			// Add penalty to bonus.
//			//iBonus -= iAutoPenaltySinceLastTracer;
			
			///////////////////////////////////////////////////
			// HEADROCK HAM 3.5: Limit maximum bonus by range.
//			 CHRISL: We want to limit the maximum bonus "per round" by range.  The way we have been doing it, it's max bonus by range, regardless of the
//				number of rounds fired.  This means firing 1 tracer or 20 makes no difference.  And we can run this calculation directly again
//				sTotalAutofirePenalty.

			if(sTotalAutofirePenalty > 0){
				INT16 sChanceLimit = min(gGameExternalOptions.ubCTHBumpPerTracer, ((iRange-(gGameExternalOptions.ubMinRangeTracerEffect*CELL_X_SIZE)) / CELL_X_SIZE) * gGameExternalOptions.ubRangeDifficultyAimingWithTracers);
				sChanceLimit = max(0, sChanceLimit);
				iBonus = sChanceLimit * iTracersFired;
				iBonus = min(iBonus, sTotalAutofirePenalty);
			}

			//INT16 sChanceLimit = __min(0, sTotalAutofirePenalty+(((iRange-100) / CELL_X_SIZE) * gGameExternalOptions.ubRangeDifficultyAimingWithTracers));
			
			//INT16 sBaseChance = iChance + sTotalAutofirePenalty;

			// We don't want to enforce a limit unless the tracers have actually put us over the original CtH.
//			if (sBaseChance <= iChance+iBonus) // Base_Chance without AutoPen, less or equal to Current_Chance plus tracer bumps
//			{
//				// store lowest: Chance+Tracer bumps, or Range-enforced limit
//				INT16 sChanceLimit = __min(iChance+iBonus, sBaseChance+(((iRange-100) / CELL_X_SIZE) * gGameExternalOptions.ubRangeDifficultyAimingWithTracers));
//				// store highest: Chance Delta or base CtH
//				//sChanceDelta = __max(sChanceDelta, sBaseChance);
//
//				// iBonus is the distance between the enforced limit (if any) and the current chance with all penalties so far.
//				// But it can't be negative, 'cause it's a bonus.
//				iBonus = __max(0,sChanceLimit - iChance);
//				// Add autopenalty since last tracer
//				if (iBulletsSinceLastTracer < iBulletsPerTracer)
//				{
//					iBonus -= ubAutoPenaltySinceLastTracer;
//				}
//
//			}
			iChance += iBonus;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	// adjust for roof/not on roof
	if ( pSoldier->pathing.bLevel == 0 )
	{
		if ( pSoldier->bTargetLevel > 0 )
		{
			// penalty for firing up
			iChance -= AIM_PENALTY_FIRING_UP;
		}
	}
	else // pSoldier->pathing.bLevel > 0 )
	{
		if ( pSoldier->bTargetLevel == 0 )
		{
			iChance += AIM_BONUS_FIRING_DOWN;
		}
	}


	pTarget = SimpleFindSoldier( sGridNo, pSoldier->bTargetLevel );
	//CHRISL: A target's stance should have no impact on an aimed, headshot.  The head doesn't get any smaller just because the target is crouching down.
	if (pTarget != NULL && ubAimPos != AIM_SHOT_HEAD)
	{
		// targeting a merc
		// adjust for crouched/prone target
		switch( gAnimControl[ pTarget->usAnimState ].ubHeight )
		{
			case ANIM_CROUCH:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 13% penalty per tile closer than min range
					iChance -= 13 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE + 10 * (AIM_PENALTY_TARGET_CROUCHED / 3) )
					{
						iChance -= AIM_PENALTY_TARGET_CROUCHED;
					}
					else if (iRange > POINT_BLANK_RANGE)
					{
						// at close range give same bonus as prone, up to maximum of AIM_PENALTY_TARGET_CROUCHED
						iChance -= 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
					}
				}
				break;
			case ANIM_PRONE:
				if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
				{
					// 25% penalty per tile closer than min range
					iChance -= 25 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
				}
				else
				{
					// at anything other than point-blank range
					if (iRange > POINT_BLANK_RANGE)
					{
						// reduce chance to hit with distance to the prone/immersed target
						iPenalty = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
						iPenalty = __min( iPenalty, AIM_PENALTY_TARGET_PRONE );

						iChance -= iPenalty;
					}
				}
				break;
			case ANIM_STAND:
				// if we are prone and at close range, then penalize shots to the torso or head!
				if ( iRange <= MIN_PRONE_RANGE && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
				{
					if ( ubAimPos == AIM_SHOT_RANDOM || ubAimPos == AIM_SHOT_GLAND )
					{
						ubAdjAimPos = AIM_SHOT_TORSO;
					}
					else
					{
						ubAdjAimPos = ubAimPos;
					}
					// lose 10% per height difference, lessened by distance
					// e.g. 30% to aim at head at range 1, only 10% at range 3
					// or 20% to aim at torso at range 1, no penalty at range 3
					// NB torso aim position is 2, so (5-aimpos) is 3, for legs it's 2, for head 4
					iChance -= (5 - ubAdjAimPos - iRange / CELL_X_SIZE) * 10;
				}
				break;
			default:
				break;
		}

		// penalty for amount that enemy has moved
		// HEADROCK HAM B2.6: Externalized the value
		iPenalty = __min( (UINT16)((float)pTarget->bTilesMoved * (float)gGameExternalOptions.iMovementEffectOnAiming), 30 );
		///////////////////////////////////////////////////////////////////////////////////
		// SANDRO - fearless characters do not even take their head down no matter what
		if ( gGameOptions.fNewTraitSystem && pTarget->ubProfile != NO_PROFILE )
		{
			if ( gMercProfiles[ pTarget->ubProfile ].bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
				iPenalty -= (iPenalty * 2 / 3);	// two thirds only
		}
		///////////////////////////////////////////////////////////////////////////////////
		iChance -= iPenalty;

		// if target sees us, he may have a chance to dodge before the gun goes off
		// but ability to dodge is reduced if crouched or prone!
		if (pTarget->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY && !TANK( pTarget ) && !(pSoldier->ubBodyType != QUEENMONSTER) )
		{
			iPenalty = ( EffectiveAgility( pTarget ) / 5 + EffectiveExpLevel( pTarget ) * 2);
			switch( gAnimControl[ pTarget->usAnimState ].ubHeight )
			{
				case ANIM_CROUCH:
					iPenalty = iPenalty * 2 / 3;
					break;
				case ANIM_PRONE:
					iPenalty /= 3;
					break;
			}

			// reduce dodge ability by the attacker's stats
			iBonus = ( EffectiveDexterity( pSoldier ) / 5 + EffectiveExpLevel( pSoldier ) * 2);
			if ( TANK( pTarget ) || (pSoldier->ubBodyType != QUEENMONSTER) )
			{
				// reduce ability to track shots
				iBonus = iBonus / 2;
			}

			if ( iPenalty > iBonus )
			{
				iChance -= (iPenalty - iBonus);
			}
		}
	}
	else if ( TANK( pSoldier ) && iRange < MIN_TANK_RANGE )
	{
		// 25% penalty per tile closer than min range
		iChance -= 25 * ( ( MIN_TANK_RANGE - iRange ) / CELL_X_SIZE );
	}


	// add camo effects

#if 0
	if ((victim = WhoIsThere(sGridNo)) < TOTAL_SOLDIERS)
	 {

		// if victim is 5 or more tiles away and camouflaged, reduce
		// chance to hit by 5%  (ALREADY HAVE THIS INFO)
		if (range > 75 && vicpSoldier->camouflage)
		 {
			switch(vicpSoldier->terrtype)
			{
			 case GROUNDTYPE:
			 case SANDTYPE  :
			 case GRASSTYPE :
			 case TGRASSTYPE:
			 case DGRASSTYPE:
			 case ROUGHTYPE : iChance += CAMOUFLAGE_TO_HIT_PENALTY;
						break;

			 case FLOORTYPE :
			 case LAKETYPE  :
			 case OCEANTYPE : break;

#ifdef BETAVERSION
			 default        : NumMessage("CHANCE TO HIT ERROR: Unknown camo terrtype ",vicpSoldier->terrtype);
#endif
			}
		 }
	 }
#endif

	// IF CHANCE EXISTS, BUT SHOOTER IS INJURED
	if ((iChance > 0) && (pSoldier->stats.bLife < pSoldier->stats.bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

		// injury penalty is based on % damage taken (max 2/3rds chance)
		iPenalty = (iChance * 2 * (pSoldier->stats.bLifeMax - pSoldier->stats.bLife + (bBandaged / 2))) /
						(3 * pSoldier->stats.bLifeMax);

		// reduce injury penalty due to merc's experience level (he can take it!)
		iChance -= (iPenalty * (100 - (10 * ( EffectiveExpLevel( pSoldier ) - 1)))) / 100;
	}

	// IF CHANCE EXISTS, BUT SHOOTER IS LOW ON BREATH
	if ((iChance > 0) && (pSoldier->bBreath < 100))
	{
		// breath penalty is based on % breath missing (max 1/2 chance)
		iPenalty = (iChance * (100 - pSoldier->bBreath)) / 200;
		// reduce breath penalty due to merc's dexterity (he can compensate!)
		iChance -= (iPenalty * (100 - ( EffectiveDexterity( pSoldier ) - 10))) / 100;
	}


	// CHECK IF TARGET IS WITHIN GUN'S EFFECTIVE MAXIMUM RANGE
	if ( iRange > iMaxRange )
	{
		// a bullet WILL travel that far if not blocked, but it's NOT accurate,
		// because beyond maximum range, the bullet drops rapidly

		// This won't cause the bullet to be off to the left or right, only make it
		// drop in flight.
		//iChance /= 2; //ddd
		iChance /= gGameExternalOptions.fOutOfGunRangeOrSight;
	}
	if ( iSightRange > (sDistVis * CELL_X_SIZE) )
	{
		// penalize out of sight shots, cumulative to effective range penalty
		iChance /= 2;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Bonus CtH for Militia
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sGreenMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sRegularMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sVeteranMilitiaCtHBonusPercent) /100);
	// bonus for enemy
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyAdminCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyRegularCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyEliteCtHBonusPercent) /100);
	//////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - option to make special NPCs stronger - chance to hit
	if (gGameExternalOptions.usSpecialNPCStronger > 0)
	{
		switch( pSoldier->ubProfile )
		{
			case CARMEN:
			case QUEEN:
			case JOE:
			case ANNIE:
			case CHRIS:
			case KINGPIN:
			case TIFFANY:
			case T_REX:
			case DRUGGIST:
			case GENERAL:
			case JIM:
			case JACK:
			case OLAF:
			case RAY:
			case OLGA:
			case TYRONE:
			case MIKE:
				iChance += (iChance * gGameExternalOptions.usSpecialNPCStronger / 100);
				break;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////

  // MAKE SURE CHANCE TO HIT IS WITHIN DEFINED LIMITS
	// HEADROCK: (HAM) Altered so called "Defined Limits" to accept external arguments.
	// The divisor argument only works when the minimum is set to 0. It has a chance of 1 in X to
	// bump the minimum back to 1, where X = the Divisor value. So a divisor value of 50 gives a 1/50
	// chance of getting some actual chance to hit despite the 0 minimum. The overall total would then
	// be an effective CTH of only 1/5000 (50 chances to get a 1 out of 100 CTH, hehehe)
	if (iChance <= gGameExternalOptions.ubMinimumCTH)
	{
		if ( TANK( pSoldier ) )
		{
			// allow absolute minimums
			iChance = 0;
		}
		else
		{
			iChance = gGameExternalOptions.ubMinimumCTH;
			if ( gGameExternalOptions.ubMinimumCTH == 0 )
			{
				if ( PreRandom( gGameExternalOptions.usMinimumCTHDivisor ) == (gGameExternalOptions.usMinimumCTHDivisor - 1) )
				{
					iChance = 1;
				}
			}
		}
	}
	else
	{
		// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
		// if (iChance > MAXCHANCETOHIT)
		// iChance = MAXCHANCETOHIT;
		if (iChance > gGameExternalOptions.ubMaximumCTH)
			iChance = gGameExternalOptions.ubMaximumCTH;
	}

//  NumMessage("ChanceToHit = ",chance);
   DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: ichance = %d",iChance));
  return (iChance);*/
}

UINT32 AICalcChanceToHitGun(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos, INT8 bTargetLevel, UINT16 usAnimState)//dnl ch59 180813
{
	INT8 bTrueLevel;//dnl ch59 180813 if target is above ground CalcChanceToHitGun will return 0 because pSoldier->bTargetLevel contains some old values from previous target which was on ground level
	UINT16	usTrueState;
	UINT32	uiChance;

	// same as CCTHG but fakes the attacker always standing
	usTrueState = pSoldier->usAnimState;
	bTrueLevel = pSoldier->bTargetLevel;
	pSoldier->bTargetLevel = bTargetLevel;
	pSoldier->usAnimState = usAnimState;
	uiChance = CalcChanceToHitGun( pSoldier, sGridNo, ubAimTime, ubAimPos );
	pSoldier->usAnimState = usTrueState;
	pSoldier->bTargetLevel = bTrueLevel;
	if(UsingNewCTHSystem() == true)
	{
		////////////////////////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: NCTH calculation
		//
		// In the new CTH system, the value returned by CalcChanceToHitGun is not an actual
		// chance to hit the target. In fact, it is simply a measure of how well the gun is
		// aimed. To get a good idea of whether or not a shot is likely to hit the target,
		// we need to take other factors into consideration.

		FLOAT d2DDistance = (FLOAT) PythSpacesAway( pSoldier->sGridNo, sGridNo ) * 10.0f;
		FLOAT dMagFactor = CalcMagFactor( pSoldier, &(pSoldier->inv[pSoldier->ubAttackingHand]), d2DDistance, sGridNo, (UINT8)ubAimTime );
		FLOAT dDistanceFactor = (d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE);

		uiChance = (UINT32)((uiChance * dMagFactor) / dDistanceFactor);

		FLOAT dBasicAperture = CalcBasicAperture( );
		FLOAT dAperture = (dBasicAperture * (100-uiChance)) / 100.0f;

		if (dAperture == 0)
		{
			return 100;
		}
		else
		{
			FLOAT dTargetArea = 28.26f; // Calculated area of a target given known homan body size.
			FLOAT dApertureArea = (FLOAT)(PI * (dAperture * dAperture));

			uiChance = (UINT32)__min(100, (dTargetArea / dApertureArea) * 100);
		}
		FLOAT dGunRange;

		// Flugente: check for underbarrel weapons and use that object if necessary
		OBJECTTYPE* pObjAttHand = pSoldier->GetUsedWeapon( &(pSoldier->inv[pSoldier->ubAttackingHand]) );

		dGunRange = (FLOAT)(GunRange( pObjAttHand, pSoldier ) );

		FLOAT dMaxGunRange = dGunRange * gGameCTHConstants.MAX_EFFECTIVE_RANGE_MULTIPLIER;
		if ( dMaxGunRange < d2DDistance)
		{
			// Weapon out of conceivable hit range. Reduce chance to hit to 0!
			return (0);
		}
		else if ( dGunRange < d2DDistance)
		{
			FLOAT dChance = (FLOAT)uiChance;
			FLOAT dMaxChanceReduction = (dChance * gGameCTHConstants.MAX_EFFECTIVE_RANGE_REDUCTION);
			if (gGameCTHConstants.MAX_EFFECTIVE_USE_GRADIENT)
			{
				// Just outside range. Reduce considerably!
				return min(uiChance, (UINT)(dChance - (dMaxChanceReduction * ((d2DDistance - dGunRange) / (dMaxGunRange - dGunRange)))));
			}
			else
			{
				return (UINT)(dChance - dMaxChanceReduction);
			}
		}
	}
	return( uiChance );
}

INT32 CalcBodyImpactReduction( UINT8 ubAmmoType, UINT8 ubHitLocation )
{
	// calculate how much bullets are slowed by passing through someone
	INT32 iReduction = BodyImpactReduction[ubHitLocation];

	iReduction = (INT32)(iReduction * AmmoTypes[ubAmmoType].armourImpactReductionMultiplier / max(1,AmmoTypes[ubAmmoType].armourImpactReductionDivisor));
	//switch (ubAmmoType)
	//{
	//	case AMMO_HP:
	//		iReduction = AMMO_ARMOUR_ADJUSTMENT_HP( iReduction );
	//		break;
	//	case AMMO_AP:
	//	case AMMO_HEAT:
	//		iReduction = AMMO_ARMOUR_ADJUSTMENT_AP( iReduction );
	//		break;
	//	case AMMO_SUPER_AP:
	//		iReduction = AMMO_ARMOUR_ADJUSTMENT_SAP( iReduction );
	//		break;
	//	default:
	//		break;
	//}
	return( iReduction );
}

INT32 ArmourProtection( SOLDIERTYPE * pTarget, UINT16 ubArmourType, INT16 * pbStatus, INT32 iImpact, UINT8 ubAmmoType, BOOLEAN *plateHit)
{
	INT32		iProtection, iAppliedProtection, iFailure, iCoverage;

	iProtection = Armour[ ubArmourType ].ubProtection;
	iCoverage = Armour [ ubArmourType ].ubCoverage;
	if ( *plateHit ) iCoverage = 100;

	if ( !AM_A_ROBOT( pTarget ) )
	{
		// check for the bullet missing armor due to coverage
		iFailure = PreRandom( 100 ) + 1 - iCoverage;
		if (iFailure > 0 )
		{
			if (Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_VEST && !AmmoTypes[ubAmmoType].ignoreArmour)
			{
			 	return ( iImpact/2 );
			}
			else return ( 0 );
		}
		// check for the bullet hitting a weak spot in the armour
		iFailure = PreRandom( 100 ) + 1 - *pbStatus;
		if (iFailure > 0)
		{
			iProtection -= iFailure;
			if (iProtection < 0)
			{
				if (Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_PLATE ) *plateHit=true;
				return( 0 );
			}
		}
	}

	// adjust protection of armour due to different ammo types
	iProtection = (INT32)(iProtection * AmmoTypes[ubAmmoType].armourImpactReductionMultiplier / max(1,AmmoTypes[ubAmmoType].armourImpactReductionDivisor) );

	//switch (ubAmmoType)
	//{
	//	case AMMO_HP:
	//		iProtection = AMMO_ARMOUR_ADJUSTMENT_HP( iProtection );
	//		break;
	//	case AMMO_AP:
	//	case AMMO_HEAT:
	//		iProtection = AMMO_ARMOUR_ADJUSTMENT_AP( iProtection );
	//		break;
	//	case AMMO_SUPER_AP:
	//		iProtection = AMMO_ARMOUR_ADJUSTMENT_SAP( iProtection );
	//		break;
	//	default:
	//		break;
	//}

	// figure out how much of the armour's protection value is necessary
	// in defending against this bullet
	if (iProtection > iImpact )
	{
		iAppliedProtection = iImpact;
	}
	else
	{
		// applied protection is the full strength of the armour, before AP/HP changes
		iAppliedProtection = Armour[ ubArmourType ].ubProtection;
	}

	// reduce armour condition

	if ( AmmoTypes[ubAmmoType].acidic )
	{
		// creature spit damages armour a lot! an extra 3x for a total of 4x normal
		*pbStatus -= 3 * (iAppliedProtection * Armour[ubArmourType].ubDegradePercent) / 100;

		// reduce amount of protection from armour
		iProtection /= 2;
	}
	else if ( AmmoTypes[ubAmmoType].ignoreArmour )
	{
		// knives and darts damage armour but are not stopped by kevlar
		if ( Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_VEST || Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_LEGGINGS )
		{
			iProtection = 0;
		}
	}

	if ( !AM_A_ROBOT( pTarget ) )
	{
		*pbStatus -= (iAppliedProtection * Armour[ubArmourType].ubDegradePercent) / 100;
	}

	// return armour protection
	if (Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_PLATE ) *plateHit=true;
	return( iProtection );
}


INT32 TotalArmourProtection( SOLDIERTYPE *pFirer, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, UINT8 ubAmmoType )
{
	INT32					iTotalProtection = 0, iSlot;
	OBJECTTYPE *	pArmour;
	BOOLEAN					plateHit = false;

	if (pTarget->flags.uiStatusFlags & SOLDIER_VEHICLE)
	{
		INT16 bDummyStatus = 100;
		BOOLEAN dummyCoverage = true;

		//bDummyStatus = pVehicleList[ pTarget->bVehicleID ].sExternalArmorLocationsStatus[ ubHitLocation ];

		iTotalProtection += ArmourProtection( pTarget, (UINT8) pVehicleList[ pTarget->bVehicleID ].sArmourType, &bDummyStatus, iImpact, ubAmmoType, &dummyCoverage );

		//pVehicleList[ pTarget->bVehicleID ].sExternalArmorLocationsStatus[ ubHitLocation ] = bDummyStatus;

	}
	else
	{
		switch( ubHitLocation )
		{
			case AIM_SHOT_GLAND:
				// creature hit in the glands!!! no armour there!
				return( 0 );
			case AIM_SHOT_HEAD:
				iSlot = HELMETPOS;
				break;
			case AIM_SHOT_LEGS:
				iSlot = LEGPOS;
				break;
			case AIM_SHOT_TORSO:
			default:
				iSlot = VESTPOS;
				break;

		}

		//zwwooooo - IoV: It can plates for the LBE bulletproof. Like CRIAS, MBSS, HSGI WASATCH...
		OBJECTTYPE *	pVestPack;
		pVestPack = &(pTarget->inv[VESTPOCKPOS]);
		if (iSlot == VESTPOS && pVestPack->exists() == true)
		{
			for (attachmentList::iterator iter = (*pVestPack)[0]->attachments.begin(); iter != (*pVestPack)[0]->attachments.end(); ++iter) {
				if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 )
				{
					iTotalProtection += ArmourProtection( pTarget, Item[iter->usItem].ubClassIndex, &((*iter)[0]->data.objectStatus), iImpact, ubAmmoType, &plateHit );
					if ( (*iter)[0]->data.objectStatus < USABLE )
					{
						pVestPack->RemoveAttachment(&(*iter));
						DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
						if ( pTarget->bTeam == gbPlayerNum )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[61], pTarget->name );
						}
					}
					break;
				}
			}
		}
				
		if ( iImpact > iTotalProtection )
		{
		pArmour = &(pTarget->inv[ iSlot ]);
		if (pArmour->exists() == true)
		{
			for (attachmentList::iterator iter = (*pArmour)[0]->attachments.begin(); iter != (*pArmour)[0]->attachments.end(); ++iter) {
				if (Item[iter->usItem].usItemClass == IC_ARMOUR && (*iter)[0]->data.objectStatus > 0 && iter->exists())
				{
					// bullet got through jacket; apply ceramic plate armour
					iTotalProtection += ArmourProtection( pTarget, Item[iter->usItem].ubClassIndex, &((*iter)[0]->data.objectStatus), iImpact, ubAmmoType, &plateHit );
					if ( (*iter)[0]->data.objectStatus < USABLE )
					{
						// destroy plates!
						pArmour->RemoveAttachment(&(*iter));
						DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
//#ifdef ENGLISH
						if ( pTarget->bTeam == gbPlayerNum )
						{
							// report plates destroyed!
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[61], pTarget->name );
						}
//#endif
					}
					break;//original code only used the first ceramic plate
				}
			}


			// if the plate didn't stop the bullet...
			if ( iImpact > iTotalProtection )
			{
				iTotalProtection += ArmourProtection( pTarget, Item[pArmour->usItem].ubClassIndex, &((*pArmour)[0]->data.objectStatus), iImpact, ubAmmoType, &plateHit );
				if ( (*pArmour)[0]->data.objectStatus < USABLE )
				{
					//Madd: put any attachments that someone might have added to the armour in the merc's inventory
					for (attachmentList::iterator iter = (*pArmour)[0]->attachments.begin(); iter != (*pArmour)[0]->attachments.end(); ++iter) {
						if ( !AutoPlaceObject( pTarget, &(*iter), FALSE ) && iter->exists())
						{   // put it on the ground
							AddItemToPool( pTarget->sGridNo, &(*iter), 1, pTarget->pathing.bLevel, 0 , -1 );
						}
					}

					DeleteObj( pArmour );//takes care of attachments, no need to remove() or erase()
					DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
				}
			}
		}
	}
	}
	return( iTotalProtection );
}

// HEADROCK HAM 5: Added argument the bullet!
INT32 BulletImpact( SOLDIERTYPE *pFirer, BULLET *pBullet, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iOrigImpact, INT16 sHitBy, UINT8 * pubSpecial )
{
	UINT16 usAttackingWeapon = 0;
	INT32 sOrigGridNo = 0;
	BOOLEAN fFragment = FALSE;
	if (pBullet == NULL)
	{
		usAttackingWeapon = pFirer->inv[pFirer->ubAttackingHand][0]->data.gun.ubGunAmmoType;
		sOrigGridNo = pFirer->sGridNo;
	}
	else
	{
		usAttackingWeapon = pBullet->fromItem;
		sOrigGridNo = pBullet->sOrigGridNo;
		fFragment = pBullet->fFragment;
	}

	INT32					iImpact, iFluke, iBonus, iImpactForCrits = 0;
	INT8					bStatLoss = 0;
	UINT8					ubAmmoType;

#ifdef ENABLE_ZOMBIES
	if ( pTarget->IsZombie() )
	{
		// if bullet does not hits anything other than the head, it doesn't do any damage
		if ( gGameExternalOptions.fZombieOnlyHeadshotsWork && ubHitLocation != AIM_SHOT_HEAD )
			return 0;

		// set a flag if this was a headshot, unset if it wasn't. Thus we can determine if this was a headshot kill (only if life > 0, ignore if already dead)
		if ( gGameExternalOptions.fZombieOnlyHeadShotsPermanentlyKill && pTarget->stats.bLife > 0 )
		{
			if ( ubHitLocation == AIM_SHOT_HEAD  )
				pTarget->bSoldierFlagMask |= SOLDIER_HEADSHOT;
			else
				pTarget->bSoldierFlagMask &= ~SOLDIER_HEADSHOT;
		}
	}
#endif

	// NOTE: reduction of bullet impact due to range and obstacles is handled
	// in MoveBullet.

	// Set a few things up:
	if ( Item[ usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE )
	{
		ubAmmoType = AMMO_KNIFE;
	}
	// HEADROCK HAM 5: Added provisions for fragments, which are not fired from the weapon in your hand.
	else if ( fFragment == FALSE )
	{
		// Flugente: check for underbarrel weapons and use that object if necessary
		OBJECTTYPE* pObj = pFirer->GetUsedWeapon( &pFirer->inv[pFirer->ubAttackingHand] );
		ubAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;
	}
	else
	{
		ubAmmoType = Explosive[Item[usAttackingWeapon].ubClassIndex].ubFragType;
	}

	if ( TANK( pTarget ) )
	{
		if ( !AmmoTypes[ubAmmoType].antiTank )
		{
			// ping!
			return( 0 );
		}
	}

	// plus/minus up to 25% due to "random" factors (major organs hit or missed,
	// lucky lighter in breast pocket, divine intervention on behalf of "Rev"...)
	iFluke = PreRandom(51) - 25;		// gives (0 to 50 -25)->-25% to +25%
	//NumMessage("Fluke = ",fluke);

	// up to 50% extra impact for making particularly accurate successful shots
	iBonus = sHitBy / 2;
	//NumMessage("Bonus = ",bonus);

	iOrigImpact = iOrigImpact * (100 + iFluke + iBonus) / 100;

	// at very long ranges (1.5x maxRange and beyond) impact could go negative
	if (iOrigImpact < 1)
	{
		iOrigImpact = 1;		// raise impact to a minimum of 1 for any hit
	}

	// adjust for HE rounds
	if ( AmmoTypes[ubAmmoType].highExplosive )
	{
//		iOrigImpact = AMMO_DAMAGE_ADJUSTMENT_HE( iOrigImpact );
		iOrigImpact = (INT32)(iOrigImpact * AmmoTypes[ubAmmoType].beforeArmourDamageMultiplier / max(1,AmmoTypes[ubAmmoType].beforeArmourDamageDivisor) );

		if ( TANK( pTarget ) )
		{
			// HEAT round on tank, divide by 3 for damage
			iOrigImpact /= 2;
		}
	}

	if (pubSpecial && (*pubSpecial == FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL || *pubSpecial == FIRE_WEAPON_BLINDED_SPECIAL) )
	{
		iImpact = iOrigImpact;
	}
	else
	{
		iImpact = iOrigImpact - TotalArmourProtection( pFirer, pTarget, ubHitLocation, iOrigImpact, ubAmmoType );
	}

	// calc minimum damage
	if ( AmmoTypes[ubAmmoType].zeroMinimumDamage )
	{
		if (iImpact < 0)
		{
			iImpact = 0;
		}
	}
	else
	{
		if (iImpact < ((iOrigImpact + 5) / 10) )
		{
			iImpact = (iOrigImpact + 5) / 10;
		}

		if ( (AmmoTypes[ubAmmoType].numberOfBullets > 1 ) && (pTarget->bNumPelletsHitBy > 0) )
		{
			iImpact += (pTarget->bNumPelletsHitBy - 1)  / 2;
		}

	}

	if (gfNextShotKills)
	{
		// big time cheat key effect!
		iImpact = 100;
		gfNextShotKills = FALSE;
	}

	if ( iImpact > 0 && !TANK( pTarget ) )
	{
		// Flugente: ammo can now add the lifedamage drug effect. This will kill the target in a few turns.
		// this is intended to work on darts, but it is possible on any ammo
		if ( AmmoTypes[ubAmmoType].ammoflag & AMMO_NEUROTOXIN )
		{			
			pTarget->bSoldierFlagMask |= SOLDIER_DRUGGED;

			// Add lifedamage effects
			pTarget->AddDrugValues( DRUG_TYPE_LIFEDAMAGE, Drug[DRUG_TYPE_LIFEDAMAGE].ubDrugEffect, Drug[DRUG_TYPE_LIFEDAMAGE].ubDrugTravelRate, Drug[DRUG_TYPE_LIFEDAMAGE].ubDrugSideEffect );
		}
		else if ( AmmoTypes[ubAmmoType].dart && sHitBy > 20 )
		{
			if (pubSpecial)
			{
				*pubSpecial = FIRE_WEAPON_SLEEP_DART_SPECIAL;
			}
			return( iImpact );
		}

		iImpact = (INT32)(iImpact * AmmoTypes[ubAmmoType].afterArmourDamageMultiplier / max(1,AmmoTypes[ubAmmoType].afterArmourDamageDivisor) ) ;
		//if (ubAmmoType == AMMO_HP)
		//{ // good solid hit with a hollow-point bullet, which got through armour!
		//	iImpact = AMMO_DAMAGE_ADJUSTMENT_HP( iImpact );
		//}

		////////////////////////////////////////////////////////////////////////////////////
		if ( gGameOptions.fNewTraitSystem ) // new traits bonuses - SANDRO
		{
			// Throwing skill increases damage of knives here
			if ( (ubAmmoType == AMMO_KNIFE) && HAS_SKILL_TRAIT( pFirer, THROWING_NT ))
			{
				iImpact += (INT32)(iImpact * (gSkillTraitValues.ubTHBladesDamageBonus + (gSkillTraitValues.ubTHBladesDamageBonusPerClick * pFirer->aiData.bAimTime))/100);
			}
	
			// Sniper trait adds bonus damage per aim click
			if (HAS_SKILL_TRAIT( pFirer, SNIPER_NT ) && (pFirer->aiData.bAimTime >= gSkillTraitValues.ubSNDamageBonusFromNumClicks) && !fFragment)
			{	
				iImpact += (INT32)(iImpact * (pFirer->aiData.bAimTime - gSkillTraitValues.ubSNDamageBonusFromNumClicks + 1) * gSkillTraitValues.ubSNDamageBonusPerClick * NUM_SKILL_TRAITS( pFirer, SNIPER_NT ))/100; // +5% per trait
			}
		}
				
		// Flugente: moved the damage calculation into a separate function
		BOOLEAN autoresolve = IsAutoResolveActive();	
		// HEADROCK HAM 5.1: Oh sandro, you rendered zerominimumdamage moot...
		if ( AmmoTypes[ubAmmoType].zeroMinimumDamage )
		{
			iImpact = __max( 0, (INT32)(iImpact * (100 - pTarget->GetDamageResistance(autoresolve, FALSE)) / 100 ) );
		}
		else
		{
			iImpact = __max( 1, (INT32)(iImpact * (100 - pTarget->GetDamageResistance(autoresolve, FALSE)) / 100 ) );
		}


		AdjustImpactByHitLocation( iImpact, ubHitLocation, &iImpact, &iImpactForCrits );
		
		switch( ubHitLocation )
		{
			case AIM_SHOT_HEAD:
				// is the blow deadly enough for an instant kill?
				// HEADROCK HAM 3.6: Reattached "Max Distance For Messy Death" tag from the XML! God knows why it wasn't attached when they MADE THAT TAG.
				//if ( PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && pFirer->usAttackingWeapon  == BARRETT ))
				// HEADROCK HAM 5.1: Using usAttackingWeapon
				if ( PythSpacesAway( sOrigGridNo, pTarget->sGridNo ) <= Weapon[ usAttackingWeapon ].maxdistformessydeath )
				{
					if (iImpactForCrits > MIN_DAMAGE_FOR_INSTANT_KILL && iImpactForCrits < pTarget->stats.bLife)
					{
						// blow to the head is so deadly that it causes instant death;
						// the target has more life than iImpact so we increase it
						if( iImpact < pTarget->stats.bLife ) // Added check here to see the real impact in game if bigger than life - SANDRO
							iImpact = pTarget->stats.bLife + Random( 10 );
						else
							iImpact += Random( 10 );

						iImpactForCrits = iImpact;
					}

					if (pubSpecial)
					{
						// is the blow deadly enough to cause a head explosion?
						if ( iImpactForCrits >= pTarget->stats.bLife )
						{
							if (iImpactForCrits > MIN_DAMAGE_FOR_HEAD_EXPLOSION )
							{
								*pubSpecial = FIRE_WEAPON_HEAD_EXPLODE_SPECIAL;
							}
							else if ( iImpactForCrits > (MIN_DAMAGE_FOR_HEAD_EXPLOSION / 2) && ( PreRandom( MIN_DAMAGE_FOR_HEAD_EXPLOSION / 2 ) < (UINT32)(iImpactForCrits - MIN_DAMAGE_FOR_HEAD_EXPLOSION / 2) ) )
							{
								*pubSpecial = FIRE_WEAPON_HEAD_EXPLODE_SPECIAL;
							}
						}

					}
				}
				break;
			case AIM_SHOT_LEGS:
				// is the damage enough to make us fall over?
				if ( pubSpecial && IS_MERC_BODY_TYPE( pTarget ) && gAnimControl[ pTarget->usAnimState ].ubEndHeight == ANIM_STAND && !pTarget->MercInWater() )
				{
					if (iImpactForCrits > MIN_DAMAGE_FOR_AUTO_FALL_OVER )
					{
						*pubSpecial = FIRE_WEAPON_LEG_FALLDOWN_SPECIAL;
					}
					// else ramping up chance from 1/2 the automatic value onwards
					else if ( iImpactForCrits > (MIN_DAMAGE_FOR_AUTO_FALL_OVER / 2) && ( PreRandom( MIN_DAMAGE_FOR_AUTO_FALL_OVER / 2 ) < (UINT32)(iImpactForCrits - MIN_DAMAGE_FOR_AUTO_FALL_OVER / 2) ) )
					{
						*pubSpecial = FIRE_WEAPON_LEG_FALLDOWN_SPECIAL;
					}
				}
				break;
			case AIM_SHOT_TORSO:
				// normal damage to torso
				// is the blow deadly enough for an instant kill?
				// since this value is much lower than the others, it only applies at short range...
				// HEADROCK HAM 3.6: Reattached "Max Distance For Messy Death" tag from the XML! God knows why it wasn't attached when they MADE THAT TAG.
				//if ( PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && pFirer->usAttackingWeapon  == BARRETT ))
				// HEADROCK HAM 5.1: Using usAttackingWeapon
				if ( PythSpacesAway( sOrigGridNo, pTarget->sGridNo ) <= Weapon[ usAttackingWeapon ].maxdistformessydeath )
				{
					if (iImpact > MIN_DAMAGE_FOR_INSTANT_KILL && iImpact < pTarget->stats.bLife)
					{
						// blow to the chest is so deadly that it causes instant death;
						// the target has more life than iImpact so we increase it
						if( iImpact < pTarget->stats.bLife ) // Added check here to see the real impact in game if bigger than life - SANDRO
							iImpact = pTarget->stats.bLife + Random( 10 );
						else
							iImpact += Random( 10 );

						iImpactForCrits = iImpact;
					}
					// special thing for hitting chest - allow cumulative damage to count
					else if ( (iImpact + pTarget->sDamage) > (MIN_DAMAGE_FOR_BLOWN_AWAY + MIN_DAMAGE_FOR_INSTANT_KILL) )
					{
						if( iImpact < pTarget->stats.bLife ) // Added check here to see the real impact in game if bigger than life - SANDRO
							iImpact = pTarget->stats.bLife + Random( 10 );
						else
							iImpact += Random( 10 );

						iImpactForCrits = iImpact;
					}

					// is the blow deadly enough to cause a chest explosion?
					if (pubSpecial)
					{
						if (iImpact > MIN_DAMAGE_FOR_BLOWN_AWAY && iImpact >= pTarget->stats.bLife)
						{
							*pubSpecial = FIRE_WEAPON_CHEST_EXPLODE_SPECIAL;
						}
					}
				}
				break;
		}
	}

	if ( AM_A_ROBOT( pTarget ) )
	{
		iImpactForCrits = 0;
	}

	// don't do critical hits against people who are gonna die!
	if( !IsAutoResolveActive() )
	{

		if ( AmmoTypes[ubAmmoType].knife && pFirer->aiData.bOppList[ pTarget->ubID ] == SEEN_CURRENTLY && !fFragment )
		{
			// is this a stealth attack?
			if ( pTarget->aiData.bOppList[ pFirer->ubID ] == NOT_HEARD_OR_SEEN && !CREATURE_OR_BLOODCAT( pTarget ) 
				&& (ubHitLocation == AIM_SHOT_HEAD || ubHitLocation == AIM_SHOT_TORSO ) && (pTarget->aiData.bAlertStatus < STATUS_RED || !gGameExternalOptions.fEnhancedCloseCombatSystem) ) // Added check if enemy is alerted - SANDRO
			{
				// HEADROCK HAM 4: With the original CTH system, the HitBy value represented a randomal margin by
				// which the shooter surpassed his CTH value. Therefore, only a high CTH shot which randomly succeeded
				// by a wide margin could reach values of 80 and above, and even then they were rare.
				// In the new system, HitBy represents a shooter's effort to make his shot more accurate, so shots
				// done with large amounts of aiming and skill would always result in high HitBy values (up to ~70-80%
				// regularly for the average shooting using maximum aiming). This means that high HitBy values are now
				// somewhat more common.
				// With regards to this bit of code, an increase in average HitBy means that its effect on the formula
				// should be lessened a little to maintain the same ratio of instant-deaths as in JA2. However, I am
				// reluctant to make any changes, mainly because I think there's no problem with stealth knife hits
				// being deadlier. If, however, it is discovered that stealth insta-kills are too common and too powerful,
				// reduce the effect of this sHitBy, possibly by dividing it here.

				//////////////////////////////////////////////////////////////////////////////////
				// SANDRO - old/new traits
				if( !gGameOptions.fNewTraitSystem )
				{
					if ( PreRandom( 100 ) < (UINT32)(sHitBy + 10 * NUM_SKILL_TRAITS( pFirer, THROWING_OT )) )
					{
						// instant death!
						if( iImpact < pTarget->stats.bLife ) // Added check here to see the real impact in game if bigger than life - SANDRO
							iImpact = pTarget->stats.bLife + Random( 10 );
						else
							iImpact += Random( 10 );

						iImpactForCrits = iImpact;
					}
				}
				else
				{
					// Throwing trait increases chance to silent critical hit
					if ( PreRandom( 100 ) < (UINT32)(sHitBy + (HAS_SKILL_TRAIT( pFirer, THROWING_NT ) ? gSkillTraitValues.ubTHBladesSilentCriticalHitChance : 0)) )
					{
						// No no no, we don't want instant kill in this case, 
						// but rather tripled damage, so it also always matters what knife we use - SANDRO
						iImpact *= ( 3 + gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus);

						iImpactForCrits = iImpact;
					}
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
		}

		if (iImpactForCrits > 0 && iImpactForCrits < pTarget->stats.bLife && !(pTarget->flags.uiStatusFlags & SOLDIER_MONSTER) ) // not to monsters - SANDRO
		{
			UINT32 uiCritChance = 0;
			if (fFragment)
			{
				uiCritChance = (iImpactForCrits / 2);
			}
			else
			{
				uiCritChance = (iImpactForCrits / 2) + (pFirer->aiData.bAimTime * 5);
			}
			BOOLEAN fMaliciousHit = FALSE;
			// SANDRO - Malicious characters inflict stat loss more often
			if ( gGameOptions.fNewTraitSystem && pFirer->ubProfile != NO_PROFILE && !fFragment)
			{
				if ( gMercProfiles[ pFirer->ubProfile ].bCharacterTrait == CHAR_TRAIT_MALICIOUS )
				{
					uiCritChance += 8;
					fMaliciousHit = TRUE;
				}
			}
			// SANDRO - with new traits, the chance for stat loss is higher as we are now able to repair it
			if ((PreRandom( uiCritChance ) + 1) > (UINT8)(gGameOptions.fNewTraitSystem ? gSkillTraitValues.ubDamageNeededToLoseStats : CRITICAL_HIT_THRESHOLD))
			{
				bStatLoss = (INT8) PreRandom( iImpactForCrits / 2 ) + 1;
				// SANDRO - malicious hit
				if ( fMaliciousHit && Chance( max( 15, uiCritChance )) && ( ubHitLocation == AIM_SHOT_TORSO || ubHitLocation == AIM_SHOT_LEGS ) && 
					( sHitBy >= 20 ) && ( pTarget->ubBodyType <= STOCKYMALE ) && ( gAnimControl[ pTarget->usAnimState ].ubHeight != ANIM_PRONE ) )
				{
					// no stats to lose, but drain breath a lot
					DeductPoints( pTarget, 0, (5 * APBPConstants[BP_GET_HIT]) );
					SoldierCollapse( pTarget );

					if (pTarget->name[0] && pTarget->bVisible == TRUE)
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SOLDIER_HIT_TO_GROIN], pTarget->GetName() );
					}
					// Gain morale for inflicting critical hit
					HandleMoraleEvent( pFirer, MORALE_MALICIOUS_HIT, pFirer->sSectorX, pFirer->sSectorY, pFirer->bSectorZ );

				}
				else
				{
					switch( ubHitLocation )
					{
						case AIM_SHOT_HEAD:
							if (bStatLoss >= pTarget->stats.bWisdom)
							{
								bStatLoss = pTarget->stats.bWisdom - 1;
							}
							if ( bStatLoss > 0 )
							{
								pTarget->stats.bWisdom -= bStatLoss;
								// SANDRO - added this for healing lost stats feature
								pTarget->ubCriticalStatDamage[DAMAGED_STAT_WISDOM] += bStatLoss;

								if (pTarget->ubProfile != NO_PROFILE)
								{
									gMercProfiles[ pTarget->ubProfile ].bWisdom = pTarget->stats.bWisdom;
								}

								if (pTarget->name[0] && pTarget->bVisible == TRUE)
								{
									// make stat RED for a while...
									pTarget->timeChanges.uiChangeWisdomTime = GetJA2Clock();
									pTarget->usValueGoneUp &= ~( WIS_INCREASE );

									if (bStatLoss == 1)
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_WISDOM], pTarget->GetName() );
									}
									else
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_WISDOM], pTarget->GetName(), bStatLoss );
									}
								}
							}
							else if ( pTarget->bNumPelletsHitBy == 0 )
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_HEAD_HIT], pTarget->GetName() );
							}

							// HEADROCK HAM 3.2: Critical headshots may now cause blindness, based on shot damage.
							if (gGameExternalOptions.ubChanceBlindedByHeadshot)
							{
								if (PreRandom(gGameExternalOptions.ubChanceBlindedByHeadshot) == 0)
								{
									if (pTarget->bBlindedCounter < iImpact / 10 )
										pTarget->bBlindedCounter = iImpact / 10;
								}
							}

							break;
						case AIM_SHOT_TORSO:
							// SANDRO - added a 20% chance to loose maximum health
							if ( PreRandom( 5 ) == 0 )
							{
								if (bStatLoss >= (pTarget->stats.bLifeMax - OKLIFE))
								{
									bStatLoss = pTarget->stats.bLifeMax - OKLIFE - 1;
								}
								if ( bStatLoss > iImpact)
								{
									bStatLoss = iImpact;
								}
								if ( bStatLoss > 0 )
								{
									pTarget->stats.bLifeMax -= bStatLoss;
									// SANDRO - added this for healing lost stats feature
									pTarget->ubCriticalStatDamage[DAMAGED_STAT_HEALTH] += bStatLoss;

									if (pTarget->ubProfile != NO_PROFILE)
									{
										gMercProfiles[ pTarget->ubProfile ].bLifeMax = pTarget->stats.bLifeMax;
									}

									if (pTarget->name[0] && pTarget->bVisible == TRUE)
									{
										// make stat RED for a while...
										pTarget->timeChanges.uiChangeDexterityTime = GetJA2Clock();
										pTarget->usValueGoneUp &= ~( HEALTH_INCREASE );

										if (bStatLoss == 1)
										{
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_LOSES_ONE_POINT_MAX_HEALTH], pTarget->GetName() );
										}
										else
										{
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_LOSES_X_POINTS_MAX_HEALTH], pTarget->GetName(), bStatLoss );
										}
									}
								}
							}
							else
							{
								// 50:50 to lose strength or dexterity
								if (PreRandom( 3 ) < 4 )
								{
									if (bStatLoss >= pTarget->stats.bDexterity)
									{
										bStatLoss = pTarget->stats.bDexterity - 1;
									}
									if ( bStatLoss > 0 )
									{
										pTarget->stats.bDexterity -= bStatLoss;
										// SANDRO - added this for healing lost stats feature
										pTarget->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY] += bStatLoss;

										if (pTarget->ubProfile != NO_PROFILE)
										{
											gMercProfiles[ pTarget->ubProfile ].bDexterity = pTarget->stats.bDexterity;
										}

										if (pTarget->name[0] && pTarget->bVisible == TRUE)
										{
											// make stat RED for a while...
											pTarget->timeChanges.uiChangeDexterityTime = GetJA2Clock();
											pTarget->usValueGoneUp &= ~( DEX_INCREASE );

											if (bStatLoss == 1)
											{
												ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_DEX], pTarget->GetName() );
											}
											else
											{
												ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_DEX], pTarget->GetName(), bStatLoss );
											}
										}
									}
								}
								else
								{
									if (bStatLoss >= pTarget->stats.bStrength)
									{
										bStatLoss = pTarget->stats.bStrength - 1;
									}
									if ( bStatLoss > 0 )
									{
										pTarget->stats.bStrength -= bStatLoss;
										// SANDRO - added this for healing lost stats feature
										pTarget->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH] += bStatLoss;

										if (pTarget->ubProfile != NO_PROFILE)
										{
											gMercProfiles[ pTarget->ubProfile ].bStrength = pTarget->stats.bStrength;
										}

										if (pTarget->name[0] && pTarget->bVisible == TRUE)
										{
											// make stat RED for a while...
											pTarget->timeChanges.uiChangeStrengthTime = GetJA2Clock();
											pTarget->usValueGoneUp &= ~( STRENGTH_INCREASE );

											if (bStatLoss == 1)
											{
												ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_STRENGTH], pTarget->GetName() );
											}
											else
											{
												ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_STRENGTH], pTarget->GetName(), bStatLoss );
											}
										}
									}
								}
							}
							break;
						case AIM_SHOT_LEGS:
							if (bStatLoss >= pTarget->stats.bAgility)
							{
								bStatLoss = pTarget->stats.bAgility - 1;
							}
							if ( bStatLoss > 0 )
							{
								pTarget->stats.bAgility -= bStatLoss;
								// SANDRO - added this for healing lost stats feature
								pTarget->ubCriticalStatDamage[DAMAGED_STAT_AGILITY] += bStatLoss;

								if (pTarget->ubProfile != NO_PROFILE)
								{
									gMercProfiles[ pTarget->ubProfile ].bAgility = pTarget->stats.bAgility;
								}

								if (pTarget->name[0] && pTarget->bVisible == TRUE)
								{
									// make stat RED for a while...
									pTarget->timeChanges.uiChangeAgilityTime = GetJA2Clock();
									pTarget->usValueGoneUp &= ~( AGIL_INCREASE );

									if (bStatLoss == 1)
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_AGIL], pTarget->GetName() );
									}
									else
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_AGIL], pTarget->GetName(), bStatLoss );
									}
								}
							}
							break;
					}

					// SANDRO - Gain morale for inflicting critical hit if malicious character
					if ( fMaliciousHit )
					{
						HandleMoraleEvent( pFirer, MORALE_MALICIOUS_HIT, pFirer->sSectorX, pFirer->sSectorY, pFirer->bSectorZ );
					}

					// SANDRO - new merc records - times stat damaged
					if ( bStatLoss > 0 && pTarget->ubProfile != NO_PROFILE )
						gMercProfiles[ pTarget->ubProfile ].records.usTimesStatDamaged++;
				}
			}
			else if ( ubHitLocation == AIM_SHOT_HEAD && pTarget->bNumPelletsHitBy == 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_HEAD_HIT], pTarget->GetName() );
			}
		}
	}

	return( iImpact );
}

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack )
{
	////////////////////////////////////////////
	// SANDRO - this all was somehow messed up
	////////////////////////////////////////////

	INT32 iImpact, iFluke, iBonus;

	// Flugente: check for underbarrel weapons and use that object if necessary (think of bayonets)
	OBJECTTYPE* pObj = pSoldier->GetUsedWeapon( &(pSoldier->inv[HANDPOS]) );

	if (fBladeAttack)
	{
		iImpact = ( EffectiveExpLevel( pSoldier ) / 2); // 0 to 4 for level
				
		iImpact += GetDamage(pObj);
		
		iImpact += EffectiveStrength( pSoldier, FALSE ) / 20; // 0 to 5 for strength, adjusted by damage taken

		if ( AM_A_ROBOT( pTarget ) )
		{
			iImpact /= 4;
		}
	}
	else
	{
		iImpact = ( EffectiveExpLevel( pSoldier ) / 2); // 0 to 4 for level
		iImpact += EffectiveStrength( pSoldier, FALSE ) / 5; // 0 to 20 for strength, adjusted by damage taken

		// NB martial artists don't get a bonus for using brass knuckles! - oh, they do in STOMP - SANDRO
		if (pSoldier->usAttackingWeapon )
		{
			if ( gGameOptions.fNewTraitSystem )
			{
				iImpact += GetDamage(&pSoldier->inv[HANDPOS]);

				if ( AM_A_ROBOT( pTarget ) )
				{
					iImpact /= 2;
				}
			}
			else
			{
				if (!HAS_SKILL_TRAIT( pSoldier, MARTIALARTS_OT ))
				{
					iImpact += GetDamage(&pSoldier->inv[HANDPOS]);
				}
				if ( AM_A_ROBOT( pTarget ) )
				{
					iImpact /= 2;
				}
			}
		}
		else
		{
			// base HTH damage
			// Enhanced Close Combat System - Slightly reduced for we can now attack to head for bigger damage
			if (gGameExternalOptions.fEnhancedCloseCombatSystem)
			{
				iImpact += 4;
			}
			else
			{
				iImpact += 5;
			}

			// Add melee damage multiplier to HtH attacks as well - SANDRO
			// actually I make the influence a little lesser, because to the blades and so,
			// only the item impact is multiplied, not the level and strength bonus, but here it does
			iImpact = (INT32)(iImpact * gGameExternalOptions.iMeleeDamageModifier / 100 ); 

			if ( AM_A_ROBOT( pTarget ) )
			{
				iImpact = 0;
			}
		}
	}

	iFluke = PreRandom( 51 ) - 25; // +/-25% bonus due to random factors
	iBonus = iHitBy / 2;				// up to 50% extra impact for accurate attacks

	iImpact = iImpact * (100 + iFluke + iBonus) / 100;

	iBonus = 0; 

	if (!fBladeAttack)
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			if (!pSoldier->usAttackingWeapon || Item[pSoldier->inv[HANDPOS].usItem].brassknuckles)
			{
				if ( gGameExternalOptions.fEnhancedCloseCombatSystem )
				{
					// add bonus for martial arts
					if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) )
					{
						iBonus += ( gSkillTraitValues.ubMABonusDamageHandToHand * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) );
					}			
					// add bonus for focused punch
					if (pSoldier->usAnimState == NINJA_SPINKICK || pSoldier->usAnimState == FOCUSED_PUNCH || pSoldier->usAnimState == FOCUSED_HTH_KICK )
					{
						iBonus += 50;

						// ...and martial arts
						if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) )
						{
							//iImpact = (INT32)((iImpact * (100 + gSkillTraitValues.usMAAimedPunchDamageBonus * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) ) / 100) + 0.5); // +75% damage per trait
							iBonus += ( gSkillTraitValues.usMAAimedPunchDamageBonus * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) );
						}
					}
				}
				else
				{
					// add bonuses for martial arts
					if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) )
					{
						iBonus += ( gSkillTraitValues.ubMABonusDamageHandToHand * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) );

						// The Spinning kicks or aimed punch bonus - SANDRO
						if (pSoldier->usAnimState == NINJA_SPINKICK || (pSoldier->aiData.bAimTime >= (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : 6)))
						{
							//iImpact = (INT32)((iImpact * (100 + gSkillTraitValues.usMAAimedPunchDamageBonus * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) ) / 100) + 0.5); // +75% damage per trait
							iBonus += ( (gSkillTraitValues.ubMABonusDamageHandToHand + 25 )* NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) );
						}
					}
				}
			}
			else
			{
				// bonus damage of blunt weapons for melee character
				if (HAS_SKILL_TRAIT( pSoldier, MELEE_NT ))
				{
					iBonus += gSkillTraitValues.ubMEDamageBonusBlunt;

					// bonus for aimed attack
					if ( pSoldier->aiData.bAimTime >= (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedBladedAttackes : 6) )
					{
							iBonus += gSkillTraitValues.usMEAimedMeleeAttackDamageBonus;  // incresed damage if focused melee attack
					}
				}
			}
		}
		else // original code
		{
			if ( gGameExternalOptions.fEnhancedCloseCombatSystem && (pSoldier->usAnimState == NINJA_SPINKICK || pSoldier->usAnimState == FOCUSED_PUNCH || pSoldier->usAnimState == FOCUSED_HTH_KICK ))
			{
				iBonus += 50;
			}
			// add bonuses for hand-to-hand and martial arts
			if ( HAS_SKILL_TRAIT( pSoldier, MARTIALARTS_OT ) )
			{
				iBonus += gbSkillTraitBonus[MARTIALARTS_OT] * NUM_SKILL_TRAITS( pSoldier, MARTIALARTS_OT );
				if (pSoldier->usAnimState == NINJA_SPINKICK)
				{
					iBonus += 100;
				}
			}
			if ( HAS_SKILL_TRAIT( pSoldier, HANDTOHAND_OT ) )
			{
				// SPECIAL  - give TRIPLE bonus for damage for hand-to-hand trait
				// because the HTH bonus is half that of martial arts, and gets only 1x for to-hit bonus
				iBonus += 3 * gbSkillTraitBonus[HANDTOHAND_OT] * NUM_SKILL_TRAITS( pSoldier, HANDTOHAND_OT );
			}
		}

		// Enhanced Close Combat System - aiming at body parts makes difference
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			if ( pSoldier->bAimShotLocation == AIM_SHOT_HEAD )
			{
				iBonus += 25; // Punch or kick to head deal more damage (vs -20% accuracy)
			}
			else if	( pSoldier->bAimShotLocation == AIM_SHOT_LEGS )
			{
				// Punch or kick to legs deal less damage but takes some AP per damage out of target adjusted by AP_MAXIMUM
				// NO, better to make hit to legs have a much better chance to hit, while only 50% damage is dealt
				iBonus -= 50; 
				//DeductPoints( pTarget, (APBPConstants[AP_MAXIMUM] / 10), 0 );
			}
			else if (gAnimControl[ pTarget->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				iBonus += 30; // 30% incresed damage to lying characters
			}

			// Surprising attack deals more damage
			if (pTarget->aiData.bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN && !CREATURE_OR_BLOODCAT( pTarget ) )
			{
				iBonus += 50; // 50% incresed damage on suprising attacks
			}						
		}

		// Flugente: power armour reduces damage taken
		// SANDRO - huh?!
		/*INT8 iSlot = VESTPOS;
		switch( pSoldier->bAimShotLocation )
		{
			case AIM_SHOT_HEAD:
				iSlot = HELMETPOS;
				break;
			case AIM_SHOT_LEGS:
				iSlot = LEGPOS;
				break;
			default:
				iSlot = VESTPOS;
				break;
		}*/
	}
	// DAMAGE BONUS TO KNIFE ATTACK WITH MELEE SKILL
	else 
	{
		if ( HAS_SKILL_TRAIT( pSoldier, MELEE_NT ) && ( gGameOptions.fNewTraitSystem ))
		{
			iBonus += gSkillTraitValues.ubMEDamageBonusBlades; // +30% damage

			if (pSoldier->usAnimState == FOCUSED_STAB)
			{
				iBonus += gSkillTraitValues.usMEAimedMeleeAttackDamageBonus;  // 50% incresed damage if focused melee attack
			}
		}
		// Enhanced Close Combat System
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			if (gAnimControl[ pTarget->usAnimState ].ubEndHeight == ANIM_PRONE)
			{
				iBonus += 30;  // incresed damage to lying characters
			}
		}

		// Flugente: power armour reduces damage taken
		// SANDRO - huh?!
		/*INT8 iSlot = VESTPOS;
		switch( pSoldier->bAimShotLocation )
		{
			case AIM_SHOT_HEAD:
				iSlot = HELMETPOS;
				break;
			case AIM_SHOT_LEGS:
				iSlot = LEGPOS;
				break;
			default:
				iSlot = VESTPOS;
				break;
		}*/
	}

	// bonus damage for aggressive characters
	if ( gGameOptions.fNewTraitSystem && gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
	{
		iBonus += 10; // +10%
	}

	// Flugente: if we are using a garotte, there is a chance that we score an instakill
	// our level in covert ops and wether the target is aware of us has a huge impact
	if ( pObj && HasItemFlag(pObj->usItem, GAROTTE) )
	{
		INT32 instakillchance = 0;
		INT32 resistchance = 20;

		if ( !SoldierTo3DLocationLineOfSightTest( pSoldier, pTarget->sGridNo, pTarget->pathing.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
			instakillchance += 30;

		UINT8 skilllevel = NUM_SKILL_TRAITS( pSoldier, COVERT_NT );
		instakillchance += skilllevel * gSkillTraitValues.sCoMeleeInstakillBonus;

		if ( pTarget->aiData.bAlertStatus == STATUS_YELLOW )
			resistchance += 20;
		else if ( pTarget->aiData.bAlertStatus >= STATUS_RED )
			resistchance += 50;

		if ( pTarget->bCollapsed )
			resistchance = 0;

		// killchance gets lowered if garotte is in bad shape
		instakillchance *= ( (*pObj)[0]->data.objectStatus / 100 );

		if ( Random(instakillchance) >= Random(resistchance) )
			iImpact += 500;
	}

	// apply all bonuses
	iImpact = (iImpact * (100 + iBonus) + 50) / 100; // round it properly

	// Flugente: moved the damage calculation into a separate function
	BOOLEAN autoresolve = IsAutoResolveActive();		
	iImpact = max( 1, (INT32)(iImpact * (100 - pTarget->GetDamageResistance(autoresolve, FALSE)) / 100 ) );

#ifdef ENABLE_ZOMBIES
	// Flugente: if the target is a zombie, any melee attack, regardless of hit location, will set the headshot flag. Thus any zombie killed in melee will stay dead (if you play with that option)
	if ( pTarget->IsZombie() )
	{
		// set a flag if this was a headshot, unset if it wasn't. Thus we can determine if this was a headshot kill (only if life > 0, ignore if already dead)
		if ( gGameExternalOptions.fZombieOnlyHeadShotsPermanentlyKill && pTarget->stats.bLife > 0 )
		{
			pTarget->bSoldierFlagMask |= SOLDIER_HEADSHOT;
		}
	}
#endif

	return( iImpact );
}

void ShotMiss( UINT8 ubAttackerID, INT32 iBullet )
{
	BOOLEAN						fDoMissForGun = FALSE;
	SOLDIERTYPE				*pAttacker;
	BULLET						*pBullet;

	pAttacker = MercPtrs[ ubAttackerID ];

	if ( pAttacker->ubOppNum != NOBODY )
	{
		// if it was another team shooting at someone under our control
		if ( (pAttacker->bTeam != Menptr[ pAttacker->ubOppNum ].bTeam ) )
		{
			// if OPPONENT is under our control
      if (Menptr[ pAttacker->ubOppNum ].bTeam == gbPlayerNum )
			{
				// AGILITY GAIN: Opponent "dodged" a bullet shot at him (it missed)
				StatChange( MercPtrs[ pAttacker->ubOppNum ], AGILAMT, 5, FROM_FAILURE );
			}
		}
	}

	switch(  Weapon[ MercPtrs[ ubAttackerID ]->usAttackingWeapon ].ubWeaponClass )
	{
		case HANDGUNCLASS:
		case RIFLECLASS:
		case SHOTGUNCLASS:
		case SMGCLASS:
		case MGCLASS:

			// Guy has missed, play random sound
			if (  MercPtrs[ ubAttackerID ]->bTeam == gbPlayerNum )
			{
				if ( Random(40) == 0 )
				{
					MercPtrs[ ubAttackerID ]->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
				}
			}
			fDoMissForGun = TRUE;
			break;

		case MONSTERCLASS:
			PlayJA2Sample( SPIT_RICOCHET, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			break;

		case KNIFECLASS://dnl ch67 080913
			if((pBullet=GetBulletPtr(iBullet)) != NULL)
			{
				INT8 bMaxLeft, bMaxRight, bMaxUp, bMaxDown, bXOffset, bYOffset, ubSearchRange;
				INT32 iGridNo = NOWHERE;
				for(ubSearchRange = 0; ubSearchRange < 4 && iGridNo == NOWHERE; ubSearchRange++)
				{
					bMaxLeft = bMaxRight = ubSearchRange;// determine maximum horizontal limits
					bMaxUp = bMaxDown = ubSearchRange;// determine maximum vertical limits
					for(bYOffset = -bMaxUp; bYOffset <= bMaxDown && iGridNo == NOWHERE; bYOffset++)// evaluate every tile until find first reachable one
						for(bXOffset = -bMaxLeft; bXOffset <= bMaxRight; bXOffset++)
						{
							iGridNo = pBullet->sGridNo + bXOffset + (MAXCOL * bYOffset);// calculate the next potential gridno near this opponent
							if(!TileIsOutOfBounds(iGridNo) && !FindStructure(iGridNo, STRUCTURE_BLOCKSMOVES))
								break;
							iGridNo = NOWHERE;
						}
				}
				if(iGridNo != NOWHERE)
				{
					CreateItem(pBullet->fromItem, (pBullet->ubItemStatus>1 ? pBullet->ubItemStatus-Random(2) : pBullet->ubItemStatus), &gTempObject);
					AddItemToPool(iGridNo, &gTempObject, -1, 0, 0, -1);
					NotifySoldiersToLookforItems();
				}
			}
			break;
	}

	if ( fDoMissForGun )
	{
		// PLAY SOUND AND FLING DEBRIS
		// RANDOMIZE SOUND SYSTEM

		if ( !DoSpecialEffectAmmoMiss( ubAttackerID, NOWHERE, 0, 0, 0, TRUE, TRUE, 0 ) )
		{
			PlayJA2Sample( MISS_1 + Random(8), RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}

		// ATE: Show misses...( if our team )
		if ( gGameSettings.fOptions[ TOPTION_SHOW_MISSES ] )
		{
			pBullet = GetBulletPtr( iBullet );

			if ( pAttacker->bTeam == gbPlayerNum )
			{
				LocateGridNo( pBullet->sGridNo );
			}
		}

	}

	// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet missed") );
	// FreeUpAttacker( ubAttackerID );
}

UINT32 CalcChanceHTH( SOLDIERTYPE * pAttacker,SOLDIERTYPE *pDefender, INT16 ubAimTime, UINT8 ubMode )
{
	UINT16 usInHand;
	UINT8 ubBandaged;
	INT32 iAttRating, iDefRating;
	INT32 iChance;

	usInHand = pAttacker->usAttackingWeapon;

	// Flugente: we might be using a bayonet, we should check that
	if ( pAttacker->bWeaponMode == WM_ATTACHED_BAYONET )
		usInHand =  pAttacker->GetUsedWeaponNumber( &(pAttacker->inv[HANDPOS]) );

	if ( (usInHand != CREATURE_QUEEN_TENTACLES ) && (pDefender->stats.bLife < OKLIFE || pDefender->bBreath < OKBREATH) )
	{
		// there is NO way to miss
		return( 100 );
	}

	if (ubMode == HTH_MODE_STAB)
	{
		// safety check
		if (Weapon[usInHand].ubWeaponClass != KNIFECLASS)
		 {
			#ifdef BETAVERSION
			NumMessage("CalcChanceToStab: ERROR - Attacker isn't holding a knife, usInHand = ",usInHand);
			#endif
			return(0);
		 }
	}
	else
	{
		if ( Item[ usInHand ].usItemClass != IC_PUNCH )
		{
			return(0);
		}
	}

	// CALCULATE ATTACKER'S CLOSE COMBAT RATING (1-100)
	if (ubMode == HTH_MODE_STEAL)
	{
		///////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - Enhanced Close Combat System - different calculation for stealing
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			// We need to be agile and dexterous
			iAttRating = ( 2 * EffectiveDexterity( pAttacker, FALSE ) + // coordination, accuracy  *
					 2 * EffectiveAgility( pAttacker, FALSE ) +    // speed & reflexes
				     pAttacker->stats.bStrength +    // physical strength 
					 pDefender->bExtraStrength +    // additional strength from power armour
					 (10 * EffectiveExpLevel( pAttacker ) ) );  // experience, knowledge
		}
		else
		{
					// this is more of a brute force strength-vs-strength check
			iAttRating = ( EffectiveDexterity( pAttacker, FALSE ) + // coordination, accuracy
					 EffectiveAgility( pAttacker, FALSE ) +    // speed & reflexes
					 3 * pAttacker->stats.bStrength +    // physical strength (TRIPLED!)
					 3 * pDefender->bExtraStrength +    // additional strength from power armour
					 (10 * EffectiveExpLevel( pAttacker ) ) );  // experience, knowledge
		}
		///////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		iAttRating = (3 * EffectiveDexterity( pAttacker, FALSE ) + // coordination, accuracy (TRIPLED!)
				 EffectiveAgility( pAttacker, FALSE ) +    // speed & reflexes
				 pAttacker->stats.bStrength +    // physical strength
				 pDefender->bExtraStrength +    // additional strength from power armour
				 (10 * EffectiveExpLevel( pAttacker ) ) );  // experience, knowledge
	}

	iAttRating /= 6;  // convert from 6-600 to 1-100

	// psycho bonus - only with old traits - SANDRO
	// Flugente: drugs can temporarily cause a merc to go psycho
	if ( !( gGameOptions.fNewTraitSystem ) && pAttacker->ubProfile != NO_PROFILE && (gMercProfiles[ pAttacker->ubProfile ].bDisability == PSYCHO || MercUnderTheInfluence(pAttacker, DRUG_TYPE_PSYCHO) ) )
	{
		iAttRating += AIM_BONUS_PSYCHO;
	}

	// modify chance to hit by morale
	iAttRating += GetMoraleModifier( pAttacker );

	// modify for fatigue
	iAttRating -= GetSkillCheckPenaltyForFatigue( pAttacker, iAttRating );

	// if attacker spent some extra time aiming
	if (ubAimTime)
	{
		// use only HALF of the normal aiming bonus for knife aiming.
		// since there's no range penalty, the bonus is otherwise too generous
		// SANDRO - Enhanced Close Combat System - Because you can only aim once, I've rather put here a flat number
		// SANDRO again - changed this to behave differenty... on focused punches we go for DAMAGE, but the hit may be more predictable, so don't add any CtH
		// if we want more CtH, just use the APs to try to punch again, while the focused punch is good either on weakened or unaware enemies
		// on focused stabs however, we go for the opposite, we want to hit at all cost, since the knife deals good damage always
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			if (ubMode == HTH_MODE_STAB)
				iAttRating += 30;
			//else
				//iAttRating += 20;
		}
		else
		{
			iAttRating += ((AIM_BONUS_PER_AP * ubAimTime) / 2);    //bonus for aiming
		}
	}

	if (! (pAttacker->flags.uiStatusFlags & SOLDIER_PC) )   // if attacker is a computer AI controlled enemy
	{
		iAttRating += gbDiff[ DIFF_ENEMY_TO_HIT_MOD ][ SoldierDifficultyLevel( pAttacker ) ];
	}

	// if attacker is being affected by gas
	if ( pAttacker->flags.uiStatusFlags & SOLDIER_GASSED )
		iAttRating -= AIM_PENALTY_GASSED;

	// if attacker is being bandaged at the same time, his concentration is off
	if (pAttacker->ubServiceCount > 0)
		iAttRating -= AIM_PENALTY_GETTINGAID;

	// if attacker is still in shock
	if (pAttacker->aiData.bShock)
		iAttRating -= (pAttacker->aiData.bShock * AIM_PENALTY_PER_SHOCK);

	// if the attacker is an A.I.M. mercenary
	//if (pAttacker->characternum < MAX_AIM_MERCS)	// exclude Gus
	//	iAttRating += AdjustChanceForProfile(pAttacker,pDefender);

	// If attacker injured, reduce chance accordingly (by up to 2/3rds)
	if ((iAttRating > 0) && (pAttacker->stats.bLife < pAttacker->stats.bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		ubBandaged = pAttacker->stats.bLifeMax - pAttacker->stats.bLife - pAttacker->bBleeding;

		iAttRating -= (2 * iAttRating * (pAttacker->stats.bLifeMax - pAttacker->stats.bLife + (ubBandaged / 2))) / (3 * pAttacker->stats.bLifeMax);
	}

	// If attacker tired, reduce chance accordingly (by up to 1/2)
	if ((iAttRating > 0) && (pAttacker->bBreath < 100))
		iAttRating -= (iAttRating * (100 - pAttacker->bBreath)) / 200;

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - old/new traits
	if (ubMode == HTH_MODE_STAB)
	{
		// add bonus for melee/knifing
		if ( gGameOptions.fNewTraitSystem )
		{
			iAttRating += gSkillTraitValues.bCtHModifierKnifeAttack; // Make melee attacks a little more problematic for untrained mercs

			if (HAS_SKILL_TRAIT( pAttacker, MELEE_NT ))
			{
				iAttRating += gSkillTraitValues.ubMECtHBladesBonus;
			}
		}
		else if (HAS_SKILL_TRAIT( pAttacker, KNIFING_OT ))
		{
			iAttRating += gbSkillTraitBonus[KNIFING_OT] * NUM_SKILL_TRAITS( pAttacker, KNIFING_OT );
		}
	}
	else
	{
		// add bonus for martial arts/melee
		if ( gGameOptions.fNewTraitSystem )
		{
			iAttRating += gSkillTraitValues.bCtHModifierHtHAttack; // Make HtH attacks a little more problematic for untrained mercs

			// Flugente: if we are using a garotte, alter the attack rating on wether we are used in to this weapon, and wether our target can see us
			if ( HasItemFlag(usInHand, GAROTTE) )
			{
				// using a garotte is pretty hard, we get a malus as default value
				INT32 garottemodifier = -30;

				UINT8 skilllevel = NUM_SKILL_TRAITS( pAttacker, COVERT_NT );
				garottemodifier += skilllevel * gSkillTraitValues.sCOMeleeCTHBonus;

				if ( pDefender->bCollapsed )
					garottemodifier += 80;

				// if this guy can see us, get a big malus!
				else if ( SoldierTo3DLocationLineOfSightTest( pAttacker, pDefender->sGridNo, pDefender->pathing.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
					garottemodifier -= 80;

				iAttRating += garottemodifier;
			}

			// bare hands - bonus for Martial arts
			if (!pAttacker->usAttackingWeapon && HAS_SKILL_TRAIT( pAttacker, MARTIAL_ARTS_NT ))
			{
				iAttRating += gSkillTraitValues.ubMABonusCtHBareHands * NUM_SKILL_TRAITS( pAttacker, MARTIAL_ARTS_NT );
			}
			// brass knuckles - bonus for Martial Arts (but a little lesser)
			else if (Item[pAttacker->inv[HANDPOS].usItem].brassknuckles && HAS_SKILL_TRAIT( pAttacker, MARTIAL_ARTS_NT ))
			{
				iAttRating += gSkillTraitValues.ubMABonusCtHBrassKnuckles * NUM_SKILL_TRAITS( pAttacker, MARTIAL_ARTS_NT );
			}
			// blunt melee weapons - bonus for Melee
			else if (HAS_SKILL_TRAIT( pAttacker, MELEE_NT ))
			{
				iAttRating += gSkillTraitValues.ubMECtHBluntBonus;
			}
		}
		else // original code
		{
			if (HAS_SKILL_TRAIT( pAttacker, MARTIALARTS_OT ))
			{
				iAttRating += gbSkillTraitBonus[MARTIALARTS_OT] * NUM_SKILL_TRAITS( pAttacker, MARTIALARTS_OT );
			}
			if ( HAS_SKILL_TRAIT( pAttacker, HANDTOHAND_OT ) )
			{
				iAttRating += gbSkillTraitBonus[HANDTOHAND_OT] * NUM_SKILL_TRAITS( pAttacker, HANDTOHAND_OT );
			}
		}
	}
	
	// Added character traits influence
	if ( gGameOptions.fNewTraitSystem && pAttacker->ubProfile != NO_PROFILE )
	{
		// Sociable - better performance in groups
		if ( gMercProfiles[ pAttacker->ubProfile ].bCharacterTrait == CHAR_TRAIT_SOCIABLE )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pAttacker->ubProfile, CHAR_TRAIT_SOCIABLE );
			if ( bNumMercs > 2 )
				iAttRating += 5;
			else if ( bNumMercs > 0 )
				iAttRating += 2;
		}
		// Loner - better performance when alone
		else if ( gMercProfiles[ pAttacker->ubProfile ].bCharacterTrait == CHAR_TRAIT_LONER )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pAttacker->ubProfile, CHAR_TRAIT_LONER );
			if ( bNumMercs == 0 )
				iAttRating += 5;
			else if ( bNumMercs <= 1 )
				iAttRating += 2;
		}
		// Show-off - better performance if some babes around to impress
		else if ( gMercProfiles[ pAttacker->ubProfile ].bCharacterTrait == CHAR_TRAIT_SHOWOFF )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pAttacker->ubProfile, CHAR_TRAIT_SHOWOFF );
			if ( bNumMercs > 1 )
				iAttRating += 5;
			else if ( bNumMercs > 0 )
				iAttRating += 2;
		}
	}		

	// Flugente: backgrounds
	if (ubMode == HTH_MODE_STAB)
	{
		iAttRating = (iAttRating * (100 + pAttacker->GetBackgroundValue(BG_PERC_CTH_BLADE))) / 100;
	}
	////////////////////////////////////////////////////////////////////////////////////

	if (iAttRating < 1)
		iAttRating = 1;


	// CALCULATE DEFENDER'S CLOSE COMBAT RATING (0-100)
	if (ubMode == HTH_MODE_STEAL)
	{
		// SANDRO - Enhanced Close Combat System - stealing defence based on dexterity and strength
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			iDefRating = ( EffectiveAgility( pDefender, FALSE )) +   // speed & reflexes
			   2 * EffectiveDexterity( pDefender, FALSE ) +  // coordination, accuracy
			   2 * pDefender->stats.bStrength +    // physical strength 
			   2 * pDefender->bExtraStrength +    // additional strength from power armour
			   (10 * EffectiveExpLevel( pDefender ) );  // experience, knowledge
		}
		else
		{
			iDefRating = (EffectiveAgility( pDefender, FALSE )) +   // speed & reflexes
			   EffectiveDexterity( pDefender, FALSE ) +  // coordination, accuracy
			   3 * pDefender->stats.bStrength +    // physical strength (TRIPLED!)
			   3 * pDefender->bExtraStrength +    // additional strength from power armour
			   (10 * EffectiveExpLevel( pDefender ) );  // experience, knowledge
		}
	}
	else
	{
		iDefRating = (3 * EffectiveAgility( pDefender, FALSE ) ) +   // speed & reflexes (TRIPLED!)
		   EffectiveDexterity( pDefender, FALSE ) +  // coordination, accuracy
		   pDefender->stats.bStrength +    // physical strength
		   pDefender->bExtraStrength +    // additional strength from power armour
	     (10 * EffectiveExpLevel( pDefender ) );  // experience, knowledge
	}

	iDefRating /= 6;  // convert from 6-600 to 1-100

	// modify chance to dodge by morale
	iDefRating += GetMoraleModifier( pDefender );

	// modify for fatigue
	iDefRating -= GetSkillCheckPenaltyForFatigue( pDefender, iDefRating );

	// if attacker is being affected by gas
	if ( pDefender->flags.uiStatusFlags & SOLDIER_GASSED )
		iDefRating -= AIM_PENALTY_GASSED;

	// if defender is being bandaged at the same time, his concentration is off
	if (pDefender->ubServiceCount > 0)
		iDefRating -= AIM_PENALTY_GETTINGAID;

	// if defender is still in shock
	if (pDefender->aiData.bShock)
		iDefRating -= (pDefender->aiData.bShock * AIM_PENALTY_PER_SHOCK);

	// If defender injured, reduce chance accordingly (by up to 2/3rds)
	if ((iDefRating > 0) && (pDefender->stats.bLife < pDefender->stats.bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		ubBandaged = pDefender->stats.bLifeMax - pDefender->stats.bLife - pDefender->bBleeding;

		iDefRating -= (2 * iDefRating * (pDefender->stats.bLifeMax - pDefender->stats.bLife + (ubBandaged / 2))) / (3 * pDefender->stats.bLifeMax);
	}

	// If defender tired, reduce chance accordingly (by up to 1/2)
	if ((iDefRating > 0) && (pDefender->bBreath < 100))
		iDefRating -= (iDefRating * (100 - pDefender->bBreath)) / 200;

	if ( usInHand == CREATURE_QUEEN_TENTACLES && pDefender->ubBodyType == LARVAE_MONSTER || pDefender->ubBodyType == INFANT_MONSTER )
	{
		// try to prevent queen from killing the kids, ever!
		iDefRating += 10000;
	}

	if (gAnimControl[ pDefender->usAnimState ].ubEndHeight < ANIM_STAND)
	{
		if (usInHand == CREATURE_QUEEN_TENTACLES)
		{
			if ( gAnimControl[ pDefender->usAnimState ].ubEndHeight == ANIM_PRONE )
			{
				// make it well-nigh impossible to hit someone who is prone!
				iDefRating += 1000;
			}
			else
			{
				iDefRating += BAD_DODGE_POSITION_PENALTY * 2;
			}
		}
		else
		{
			// if defender crouched, reduce chance accordingly (harder to dodge)
			iDefRating -= BAD_DODGE_POSITION_PENALTY;
			// If our target is prone, double the penalty!
			if ( gAnimControl[ pDefender->usAnimState ].ubEndHeight == ANIM_PRONE )
			{
				iDefRating -= BAD_DODGE_POSITION_PENALTY;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - old/new traits
	if (ubMode == HTH_MODE_STAB)
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			iDefRating += gSkillTraitValues.bModifierDodgeKnifeChance; // Make HtH dodging a little more problematic for untrained mercs

			// Give bonus to dodge melee blade to MA and/or Melee trait - SANDRO
			// Note that it is no longer dependant on what you have in hands
			if (HAS_SKILL_TRAIT( pDefender, MARTIAL_ARTS_NT ))
			{
				iDefRating += ( gSkillTraitValues.ubMAChanceToDodgeMelee * NUM_SKILL_TRAITS( pDefender, MARTIAL_ARTS_NT ) );
			}
			if (HAS_SKILL_TRAIT( pDefender, MELEE_NT ))
			{
				iDefRating += gSkillTraitValues.ubMEDodgeBladesBonus;

				if (Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE)
				{
					// knife-to-knife combat, we are the best around at this
					iDefRating += gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands;
				}
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pDefender, KNIFING_OT ) )
			{
				if (Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE)
				{
					// good with knives, got one, so we're good at parrying
					iDefRating += gbSkillTraitBonus[KNIFING_OT] * NUM_SKILL_TRAITS( pDefender, KNIFING_OT );
				}
				else
				{
					// good with knives, don't have one, but we know a bit about dodging
					iDefRating += ( gbSkillTraitBonus[KNIFING_OT] * NUM_SKILL_TRAITS( pDefender, KNIFING_OT ) ) / 3;
				}
			}
			if (HAS_SKILL_TRAIT( pDefender, MARTIALARTS_OT ))
			{
				// bonus for dodging knives
				iDefRating += ( gbSkillTraitBonus[MARTIALARTS_OT] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS_OT ) )/ 2;
			}
		}
	}
	else
	{	// punch/hand-to-hand/martial arts attack/steal
		if ( gGameOptions.fNewTraitSystem )
		{
			iDefRating += gSkillTraitValues.bModifierDodgeHtHChance; // Make HtH dodging a little more problematic for untrained mercs

			if ( !pAttacker->usAttackingWeapon || Item[pAttacker->inv[HANDPOS].usItem].brassknuckles )
			{
				if (HAS_SKILL_TRAIT( pDefender, MARTIAL_ARTS_NT ))
				{
					// Bonus to dodge to MA trait - SANDRO
					// Note that it no longer depends on whatever having in hands, because before 
					// you would have unlogicaly lesser chance to dodge with a knife in hands than with a gun
					iDefRating += gSkillTraitValues.ubMAChanceToDodgeHtH * NUM_SKILL_TRAITS( pDefender, MARTIAL_ARTS_NT ); // +35% per trait

					if ( !pDefender->inv[HANDPOS].exists() )
					{
						iDefRating += ( gSkillTraitValues.ubMAOnTopCTDHtHBareHanded * NUM_SKILL_TRAITS( pDefender, MARTIAL_ARTS_NT ) );
					}
					else if ( Item[pDefender->inv[HANDPOS].usItem].brassknuckles )
					{
						iDefRating += ( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * NUM_SKILL_TRAITS( pDefender, MARTIAL_ARTS_NT ) );
					}
				}
			}
			else
			{
				if (HAS_SKILL_TRAIT( pDefender, MARTIAL_ARTS_NT ))
				{
					iDefRating += gSkillTraitValues.ubMAChanceToDodgeMelee * NUM_SKILL_TRAITS( pDefender, MARTIAL_ARTS_NT ); // +35% per trait
				}
				if (HAS_SKILL_TRAIT( pDefender, MELEE_NT ))
				{
					iDefRating += gSkillTraitValues.ubMEDodgeBluntBonus;

					if (Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE)
					{
						// bonus, when we have our knife in hands
						iDefRating += gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands;
					}
				}
			}
		}
		else
		{
			// SANDRO - this is so idiotic, it actiually means, that martial artists and HtH 
			// soldiers do not get their dodging bonus if they have a knife in hands, but do
			// have it if having anything else! I just had to change it, for it is a bug more
			// than an intended feature
			if (HAS_SKILL_TRAIT( pDefender, KNIFING_OT ) && Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE && ubMode != HTH_MODE_STEAL)
			{
				// with our knife, we get some bonus at defending from HTH attacks
				iDefRating += ( gbSkillTraitBonus[KNIFING_OT] * NUM_SKILL_TRAITS( pDefender, KNIFING_OT ) ) / 2;

			}
			if (HAS_SKILL_TRAIT( pDefender, MARTIALARTS_OT ))
			{
				// now the bonus depends on what we have in hands
				if ( pDefender->inv[HANDPOS].exists() )
				{
					// if we have something in hands, the chance is reduced
					iDefRating += gbSkillTraitBonus[MARTIALARTS_OT] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS_OT )*2/3;
				}
				else
				{
					// else we are unbeatable
					iDefRating += gbSkillTraitBonus[MARTIALARTS_OT] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS_OT );
				}
			}
			if (HAS_SKILL_TRAIT( pDefender, HANDTOHAND_OT ))
			{
				iDefRating += gbSkillTraitBonus[HANDTOHAND_OT] * NUM_SKILL_TRAITS( pDefender, HANDTOHAND_OT );
			}
		}
	}

	// Added character traits influence
	if ( gGameOptions.fNewTraitSystem && pDefender->ubProfile != NO_PROFILE )
	{
		// Sociable - better performance in groups
		if ( gMercProfiles[ pDefender->ubProfile ].bCharacterTrait == CHAR_TRAIT_SOCIABLE )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pDefender->ubProfile, CHAR_TRAIT_SOCIABLE );
			if ( bNumMercs > 2 )
				iDefRating += 5;
			else if ( bNumMercs > 0 )
				iDefRating += 2;
		}
		// Loner - better performance when alone
		else if ( gMercProfiles[ pDefender->ubProfile ].bCharacterTrait == CHAR_TRAIT_LONER )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pDefender->ubProfile, CHAR_TRAIT_LONER );
			if ( bNumMercs == 0 )
				iDefRating += 5;
			else if ( bNumMercs <= 1 )
				iDefRating += 2;
		}
		// Show-off - better performance if some babes around to impress
		else if ( gMercProfiles[ pDefender->ubProfile ].bCharacterTrait == CHAR_TRAIT_SHOWOFF )
		{	
			INT8 bNumMercs = CheckMercsNearForCharTraits( pDefender->ubProfile, CHAR_TRAIT_SHOWOFF );
			if ( bNumMercs > 1 )
				iDefRating += 5;
			else if ( bNumMercs > 0 )
				iDefRating += 2;
		}
		else if ( gMercProfiles[ pDefender->ubProfile ].bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
		{
			iDefRating -= 5;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	if (iDefRating < 1)
		iDefRating = 1;


	//NumMessage("CalcChanceToStab - Attacker's Rating = ",iAttRating);
	//NumMessage("CalcChanceToStab - Defender's Rating = ",iDefRating);

	// calculate chance to hit by comparing the 2 opponent's ratings
	// iChance = (100 * iAttRating) / (iAttRating + iDefRating);


	if (ubMode == HTH_MODE_STEAL)
	{
		// make this more extreme so that weak people have a harder time stealing from
		// the stronger
		iChance = 50 * iAttRating / iDefRating;
	}
	else
	{
		// Changed from DG by CJC to give higher chances of hitting with a stab or punch
		iChance = 67 + (iAttRating - iDefRating) / 3;

		// SANDRO - Enhanced Close Combat System - chances to hit for punches aimed at body parts
		if (gGameExternalOptions.fEnhancedCloseCombatSystem)
		{
			if( gGameOptions.fNewTraitSystem  && HAS_SKILL_TRAIT( pAttacker, MARTIAL_ARTS_NT ) && 
				((pAttacker->usAnimState == NINJA_SPINKICK) || (pAttacker->aiData.bAimTime >= (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : 6))) )
			{
				// spinning kick or focused punch modifier
				iChance += gSkillTraitValues.bMAAimedPunchCtHModifier * NUM_SKILL_TRAITS( pAttacker, MARTIAL_ARTS_NT );
			}

			if ( pAttacker->bAimShotLocation == AIM_SHOT_HEAD )
			{
				// Harder to hit head (but higher damage there)
				iChance -= 20;
			}
			else if( pAttacker->bAimShotLocation == AIM_SHOT_LEGS )
			{
				// easy to hit legs (but low damage there)
				iChance += 25;
			}
		}
		else
		{
			if ( pAttacker->bAimShotLocation == AIM_SHOT_HEAD )
			{
				// make this harder!
				iChance -= 20;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Bonus CtH for Militia
	if (pAttacker->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sGreenMilitiaCtHBonusPercent) /100);
	else if (pAttacker->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sRegularMilitiaCtHBonusPercent) /100);
	else if (pAttacker->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sVeteranMilitiaCtHBonusPercent) /100);
	// bonus for enemy
	else if (pAttacker->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyAdminCtHBonusPercent) /100);
	else if (pAttacker->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyRegularCtHBonusPercent) /100);
	else if (pAttacker->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyEliteCtHBonusPercent) /100);
	//////////////////////////////////////////////////////////////////////////////////////

  // MAKE SURE CHANCE TO HIT IS WITHIN DEFINED LIMITS
  // HEADROCK: I urinate on your Defined Limits! Power Rangers, Externalize!
  // Disclaimer: No offense meant, all in good fun ;)
  if (iChance < gGameExternalOptions.ubMinimumCTH)
	{
    iChance = gGameExternalOptions.ubMinimumCTH;
	}
  else
	{
		// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
		//if (iChance > MAXCHANCETOHIT)
		//	iChance = MAXCHANCETOHIT;
		if (iChance > gGameExternalOptions.ubMaximumCTH)
			iChance = gGameExternalOptions.ubMaximumCTH;
	}

  //NumMessage("ChanceToStab = ",chance);

  return (iChance);
}

UINT32 CalcChanceToStab(SOLDIERTYPE * pAttacker,SOLDIERTYPE *pDefender, INT16 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_STAB ) );
}

UINT32 CalcChanceToPunch(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, INT16 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_PUNCH ) );
}

UINT32 CalcChanceToSteal(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, INT16 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_STEAL ) );
}


void ReloadWeapon( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
{
	// NB this is a cheat function, don't award experience

	if ( pSoldier->inv[ ubHandPos ].exists() == true )
	{
		pSoldier->inv[ ubHandPos ][0]->data.gun.ubGunShotsLeft = GetMagSize(&pSoldier->inv[ ubHandPos ]);
		// Dirty Bars
		if ( IsWeaponAttached(&pSoldier->inv[ ubHandPos ], IC_GUN) )
		{
			(*FindAttachedWeapon(&pSoldier->inv[ ubHandPos ], IC_GUN))[0]->data.gun.ubGunShotsLeft = GetMagSize(FindAttachedWeapon(&pSoldier->inv[ ubHandPos ], IC_GUN));
		}
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL1 );
	}
}

// Changed by ADB, rev 1513
//BOOLEAN IsGunWeaponModeCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , UINT8 bWpnMode)
BOOLEAN IsGunWeaponModeCapable( OBJECTTYPE* pObject, WeaponMode bWpnMode, SOLDIERTYPE *pSoldier )
{
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("IsGunWeaponModeCapable: weapon mode=%d",bWpnMode));

	// Flugente: if have a riflegrenade device attached, and that thing has a launchable grenade, block all other firing modes
	if ( HasAttachmentOfClass(pObject, AC_RIFLEGRENADE) )
	{
		OBJECTTYPE* pRifleGrenadeDeviceObj = FindAttachment_GrenadeLauncher(pObject);

		if ( pRifleGrenadeDeviceObj && FindLaunchableAttachment( pObject, pRifleGrenadeDeviceObj->usItem) )
		{
			if ( bWpnMode == WM_ATTACHED_GL )
			{				
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	switch(bWpnMode)
	{
		case WM_NORMAL:
		//return (pSoldier->inv[ ubHandPos ].exists() == true && Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON && !Weapon[ pSoldier->inv[ ubHandPos ].usItem ].NoSemiAuto );  // Check for being a weapon....
		return (pObject->exists() == true && Item[ pObject->usItem ].usItemClass & IC_WEAPON && !Weapon[ pObject->usItem ].NoSemiAuto );  // Check for being a weapon....

		case WM_BURST:
		//return IsGunBurstCapable(pSoldier, ubHandPos, FALSE);
		return IsGunBurstCapable(pObject, FALSE, pSoldier);

		case WM_AUTOFIRE:
		//return ((IsGunAutofireCapable(pSoldier, ubHandPos) || Weapon[ pSoldier->inv[ ubHandPos ].usItem ].NoSemiAuto )&& !Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher );
		return ((IsGunAutofireCapable(pObject) || Weapon[ pObject->usItem ].NoSemiAuto )&& !Item[pObject->usItem].grenadelauncher );

		case WM_ATTACHED_GL:
//		return (FindAttachment( &(pSoldier->inv[ubHandPos]), UNDER_GLAUNCHER ) != 0 && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), UNDER_GLAUNCHER ) != 0 );

		//return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher &&  IsGrenadeLauncherAttached( &(pSoldier->inv[ubHandPos]) ) && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]) )) != 0 );
		return ( (!Item[pObject->usItem].grenadelauncher && !IsAttachmentClass(pObject->usItem, AC_RIFLEGRENADE) ) &&  IsGrenadeLauncherAttached( pObject ) && FindLaunchableAttachment( pObject, GetAttachedGrenadeLauncher( pObject )) != 0 );

		case WM_ATTACHED_GL_BURST:
			//return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher && IsGrenadeLauncherAttached( &(pSoldier->inv[ubHandPos]) ) && Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[ubHandPos])].ubShotsPerBurst > 0 && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]))) != 0 );
			return ( (!Item[pObject->usItem].grenadelauncher && !HasAttachmentOfClass( pObject, AC_RIFLEGRENADE ) ) && IsGrenadeLauncherAttached( pObject ) && Weapon[GetAttachedGrenadeLauncher(pObject)].ubShotsPerBurst > 0 && FindLaunchableAttachment( pObject, GetAttachedGrenadeLauncher( pObject)) != 0 );

		case WM_ATTACHED_GL_AUTO:
			return FALSE;
//			return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher && Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[ubHandPos])].bAutofireShotsPerFiveAP > 0 && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]) ) != ITEM_NOT_FOUND );

		case WM_ATTACHED_UB:
			return (IsWeaponAttached( pObject, IC_GUN ) && !Weapon[FindAttachedWeapon(pObject, IC_GUN)->usItem].NoSemiAuto );

		case WM_ATTACHED_UB_BURST:
			return (IsWeaponAttached( pObject, IC_GUN ) && IsGunBurstCapable(FindAttachedWeapon(pObject, IC_GUN), FALSE, pSoldier));

		case WM_ATTACHED_UB_AUTO:
			return (IsWeaponAttached( pObject, IC_GUN ) && (IsGunAutofireCapable(FindAttachedWeapon(pObject, IC_GUN)) || Weapon[FindAttachedWeapon(pObject, IC_GUN)->usItem].NoSemiAuto));

		case WM_ATTACHED_BAYONET:
			return IsWeaponAttached( pObject, IC_BLADE );

		default:
		return FALSE;
	}
}

// Changed by ADB, rev 1513
//BOOLEAN IsGunAutofireCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
BOOLEAN IsGunAutofireCapable( OBJECTTYPE* pObject )
{
	BOOLEAN fCapable = FALSE;

	//if ( pSoldier->inv[ ubHandPos ].exists() == true )
	if ( pObject->exists() == true )
	{
		// ATE: Check for being a weapon....
		//if ( Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON )
		if ( Item[ pObject->usItem ].usItemClass & IC_WEAPON )
		{
			//if ( GetAutofireShotsPerFiveAPs(&pSoldier->inv[ ubHandPos ]) > 0 )
			if ( GetAutofireShotsPerFiveAPs(pObject) > 0 )
			{
				fCapable = TRUE;
			}
		}
	}

	return( fCapable );
}

// Changed by ADB, rev 1513
//BOOLEAN IsGunBurstCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , BOOLEAN fNotify )
BOOLEAN IsGunBurstCapable(OBJECTTYPE* pObject, BOOLEAN fNotify, SOLDIERTYPE* pSoldier )
{
	BOOLEAN fCapable = FALSE;

	//if ( pSoldier->inv[ ubHandPos ].exists() == true )
	if ( pObject->exists() == true )
	{
		// ATE: Check for being a weapon....
		//if ( Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON )
		if ( Item[ pObject->usItem ].usItemClass & IC_WEAPON )
		{
			//if ( GetShotsPerBurst(&pSoldier->inv[ ubHandPos ]) > 0 )
			if ( GetShotsPerBurst(pObject) > 0 )
			{
				fCapable = TRUE;
			}
		}
	}

	//ADB fNotify is always false, so notify and pSoldier are not necessary parameters
	if ( fNotify && !fCapable && pSoldier != NULL )
	{
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_NOT_BURST_CAPABLE ], pSoldier->GetName() );
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_NOT_BURST_CAPABLE ], pSoldier->GetName() );
	}

	return( fCapable );
}

void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem )
{
#ifdef ADB_TODO
	SetBurstAndAutoFireMode(pSoldier, GetWeaponMode(&pSoldier->inv[uiInvPos]));
#endif
	// if in attached weapon mode and don't have weapon with GL attached in hand, reset weapon mode
	if ( ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO ) && !IsGrenadeLauncherAttached( &(pSoldier->inv[ HANDPOS ] ) ) ) ||
		 ( (pSoldier->bWeaponMode == WM_ATTACHED_UB || pSoldier->bWeaponMode == WM_ATTACHED_UB_BURST || pSoldier->bWeaponMode == WM_ATTACHED_UB_AUTO ) && !IsWeaponAttached( &(pSoldier->inv[ HANDPOS ]), IC_GUN   ) ) ||
		 ( (pSoldier->bWeaponMode == WM_ATTACHED_BAYONET )																							   && !IsWeaponAttached( &(pSoldier->inv[ HANDPOS ]), IC_BLADE ) ) ) 
	{
		if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
		{
			pSoldier->bWeaponMode = WM_NORMAL;
			pSoldier->bDoBurst = FALSE;
			pSoldier->bDoAutofire = 0;			
		}
		else
		{
			pSoldier->bWeaponMode = WM_AUTOFIRE;
			pSoldier->bDoBurst = TRUE;
			pSoldier->bDoAutofire = 1;
		}
		if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].twohanded && Weapon[ pSoldier->inv[ HANDPOS ].usItem ].HeavyGun && gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
			pSoldier->bScopeMode = USE_ALT_WEAPON_HOLD;
		else
			pSoldier->bScopeMode = USE_BEST_SCOPE;
	}

	// if he is loaded tactically
	if ( pSoldier->bInSector )
	{
		// If this is our main hand
		if ( uiInvPos == HANDPOS || uiInvPos == SECONDHANDPOS )
		{
			// check if we need to change animation!
			pSoldier->ReLoadSoldierAnimationDueToHandItemChange( usOldItem, usNewItem );
		}

		// if this is head gear
		if ( uiInvPos == HEAD1POS || uiInvPos == HEAD2POS )
		{
			// Could be because of GOGGLES change...	Re-create light...
			pSoldier->DeleteSoldierLight( );
			pSoldier->PositionSoldierLight( );
		}
	}
	else
	{
		// as a minimum
		if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass & IC_WEAPON) && GetShotsPerBurst(&pSoldier->inv[ HANDPOS ]) == 0 )
		{
			if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
			{
				pSoldier->bWeaponMode = WM_NORMAL;
				pSoldier->bDoBurst = FALSE;
				pSoldier->bDoAutofire = 0;				
			}
			else
			{
				pSoldier->bWeaponMode = WM_AUTOFIRE;
				pSoldier->bDoAutofire = 1;
				pSoldier->bDoBurst = TRUE;
			}
			if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].twohanded && Weapon[ pSoldier->inv[ HANDPOS ].usItem ].HeavyGun && gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
				pSoldier->bScopeMode = USE_ALT_WEAPON_HOLD;
			else
				pSoldier->bScopeMode = USE_BEST_SCOPE;
		}
	}

	// Flugente: if we are using dual weapons, only allow iron sights (we can't look through two scopes simultaneously, can we?)
	if ( uiInvPos == HANDPOS || uiInvPos == SECONDHANDPOS )
	{
		if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass & IC_WEAPON) && (Item[ pSoldier->inv[ SECONDHANDPOS ].usItem ].usItemClass & IC_WEAPON) )
		{
			std::map<INT8, OBJECTTYPE*> ObjList;
			GetScopeLists(&pSoldier->inv[ HANDPOS ], ObjList);

			std::map<INT8, OBJECTTYPE*>::iterator itend = ObjList.end();
			for (std::map<INT8, OBJECTTYPE*>::iterator it = ObjList.begin(); it != itend; ++it)
			{
				if ( (*it).second != NULL )
				{
					pSoldier->bScopeMode = (*it).first;
				}
				else
					break;
			}
		}
	}
}


INT32 CalcMaxTossRange( SOLDIERTYPE * pSoldier, UINT16 usItem, BOOLEAN fArmed )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange");
	INT32 iRange = 0;
	UINT16	usSubItem = NOTHING;

	if ( EXPLOSIVE_GUN( usItem ) )
	{
		// oops! return value in weapons table
		return( GetModifiedGunRange(usItem) / CELL_X_SIZE );
	}

	// if item's fired mechanically
	// ATE: If we are sent in a LAUNCHABLE, get the LAUCNHER, and sub ONLY if we are armed...
	//MM: this only works if every launcher for a given launchable has the same range!!
	//usSubItem = GetLauncherFromLaunchable( usItem );
	//MM: So instead, let's look at the soldier's hand, and check his gun for an underbarrel GL
	if ( fArmed )
	{
		OBJECTTYPE *pObj = NULL;
		pObj = pSoldier->GetUsedWeapon( &pSoldier->inv[pSoldier->ubAttackingHand] );
		if ( pObj != NULL )
		{
			if (Item[pObj->usItem].usItemClass == IC_LAUNCHER)
				usSubItem = pObj->usItem;
			else if (Item[pObj->usItem].usItemClass == IC_GUN)
			{
				usSubItem = GetAttachedGrenadeLauncher(pObj);
			}
		}
	}

	if ( fArmed && usSubItem != NOTHING )
	{
		usItem = usSubItem;
	}

	if ( Item[ usItem ].usItemClass == IC_LAUNCHER && fArmed )
	{
		// this function returns range in tiles so, stupidly, we have to divide by 10 here
		iRange = GetModifiedGunRange(usItem) / CELL_X_SIZE;
	}
	else
	{
//		if ( Item[ usItem ].fFlags & ITEM_UNAERODYNAMIC )
		if ( Item[ usItem ].unaerodynamic )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange: not aerodynamic");
			iRange = 1;
		}
		else if ( Item[ usItem ].usItemClass == IC_GRENADE )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange: grenade");
			// start with the range based on the soldier's strength and the item's weight
			// Altered by Digicrab on 14 March, 2004
			// Reversed a Ja2Gold change that made grenades of weight 3 or more have the same throw distance as those of weight 3.
			INT32 iThrowingStrength = ( EffectiveStrength( pSoldier, FALSE ) * 2 + 100 ) / 3;
			iRange = 2 + ( iThrowingStrength / (3 + (Item[usItem].ubWeight) / 3 ));
		}
		else
		{	// not as aerodynamic!
			// start with the range based on the soldier's strength and the item's weight
			iRange = 2 + ( ( EffectiveStrength( pSoldier, FALSE ) / ( 5 + Item[usItem].ubWeight) ) );
		}

		// adjust for thrower's remaining breath (lose up to 1/2 of range)
		iRange -= (iRange * (100 - pSoldier->bBreath)) / 200;

		////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if( gGameOptions.fNewTraitSystem ) 
		{
			if ( (Item[ usItem ].usItemClass == IC_THROWING_KNIFE) && (HAS_SKILL_TRAIT( pSoldier, THROWING_NT )) )
			{
				// better max range due to expertise
				iRange += ((iRange * gSkillTraitValues.ubTHBladesMaxRange ) / 100);
			}
			else if ( (Item[ usItem ].usItemClass == IC_GRENADE) && (HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT )) )
			{
				// better max range due to expertise
				iRange += ((iRange * gSkillTraitValues.ubDEMaxRangeToThrowGrenades) / 100);
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, THROWING_OT ) )
			{
				// better max range due to expertise
				iRange = iRange * (100 + gbSkillTraitBonus[THROWING_OT] * NUM_SKILL_TRAITS( pSoldier, THROWING_OT ) ) / 100;
			}		
		}
		////////////////////////////////////////////////////////////////////////////////////////////////

		// Adjust for thrower's stance
		if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight < ANIM_STAND)
		{
			// For now we just assume the thrower is crouched since we don't allow prone tossing at the moment
			// So dock 30% from the distance
			iRange = (iRange * 70) / 100;
		}
	}

	if (iRange < 1)
	{
		iRange = 1;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange done");
	return( iRange );
}


UINT32 CalcThrownChanceToHit(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, UINT8 ubAimPos )
{
	INT32 iChance, iMaxRange, iRange;
	UINT16	usHandItem;
	INT8 bPenalty, bBandaged;

	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		usHandItem = GetAttachedGrenadeLauncher (&pSoldier->inv[HANDPOS]);// UNDER_GLAUNCHER;
	}
	else
	{
		usHandItem = pSoldier->inv[HANDPOS].usItem;
	}

/*
	// CJC: Grenade Launchers don't fire in a straight line!
	#ifdef BETAVERSION
	if (usHandItem == GLAUNCHER)
	{
		PopMessage("CalcThrownChanceToHit: DOESN'T WORK ON GLAUNCHERs!");
		return(0);
	}
	#endif
*/

	if ( Item[ usHandItem ].usItemClass != IC_LAUNCHER && pSoldier->bWeaponMode != WM_ATTACHED_GL && pSoldier->bWeaponMode != WM_ATTACHED_GL_BURST && pSoldier->bWeaponMode != WM_ATTACHED_GL_AUTO )
	{
		// PHYSICALLY THROWN arced projectile (ie. grenade)
		// for lack of anything better, base throwing accuracy on dex & marskmanship
		iChance = ( EffectiveDexterity( pSoldier, FALSE ) + EffectiveMarksmanship( pSoldier ) ) / 2;
		////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if ( gGameOptions.fNewTraitSystem )
		{
			// throwing trait helps out on grenades and knives
			if ( Item[ usHandItem ].usItemClass == IC_THROWING_KNIFE )
			{
				iChance += gSkillTraitValues.bCtHModifierThrowingKnives; // -15% for untrained mercs

				if ( HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) )
					iChance += (gSkillTraitValues.ubTHBladesCtHBonus + (gSkillTraitValues.ubTHBladesCtHBonusPerClick * pSoldier->aiData.bAimTime) );
			}
			else
			{
				iChance += gSkillTraitValues.bCtHModifierThrowingGrenades; // -10% for untrained mercs

				if ( HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ) )
					iChance += gSkillTraitValues.ubDECtHWhenThrowingGrenades; // +30% chance
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, THROWING_OT ) )
				iChance += gbSkillTraitBonus[THROWING_OT] * NUM_SKILL_TRAITS( pSoldier, THROWING_OT );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{

		// MECHANICALLY FIRED arced projectile (ie. mortar), need brains & know-how
		iChance = ( EffectiveDexterity( pSoldier, FALSE ) + EffectiveMarksmanship( pSoldier ) + EffectiveWisdom( pSoldier ) + pSoldier->stats.bExpLevel ) / 4;

		////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if ( gGameOptions.fNewTraitSystem ) 
		{
			if ( Item[ usHandItem ].mortar )
			{
				if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ))
					iChance += (gSkillTraitValues.sCtHModifierMortar * max( 0, ((100 - gSkillTraitValues.ubHWMortarCtHPenaltyReduction * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ))/100)));
				else
					iChance += gSkillTraitValues.sCtHModifierMortar; // -60% for untrained mercs
			}
			else
			{
				iChance += gSkillTraitValues.bCtHModifierGrenadeLaunchers; // -25% for untrained mercs

				if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPONS_NT ))
					iChance += gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPONS_NT ); // +25% per trait - SANDRO
			}
		}
		else
		{
			// This feature is available only if not having new traits on - SANDRO
			// Also.. this was moved here before the Heavy Weapons bonus
			// HEADROCK HAM 3.2: External divisor for CTH with mortars, now that they are more prevalent in the battlefield.
			if ( Item[ usHandItem ].mortar )
			{
				iChance = iChance / gGameExternalOptions.ubMortarCTHDivisor;
			}

			// heavy weapons trait helps out
			if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPS_OT ))
			{
				iChance += gbSkillTraitBonus[HEAVY_WEAPS_OT] * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT );
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if ( UsingNewCTHSystem() == false && pSoldier->bDoBurst )
	{
		int iPenalty = ( Weapon[usHandItem].ubBurstPenalty
			- GetBurstToHitBonus(&pSoldier->inv[HANDPOS], gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) )
			* (pSoldier->bDoBurst - 1);

		// SAMDRO - shooting dual bursts is somehow harder to control
		if ( pSoldier->IsValidSecondHandBurst() )
		{
			iPenalty = iPenalty*6/5; // +10% 
		}
		// halve the penalty for people with the autofire trait
		/////////////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if ( gGameOptions.fNewTraitSystem ) 
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPONS_NT ) )
			{
				iPenalty = max(0, ((iPenalty * (100 - gSkillTraitValues.ubAWAutoFirePenaltyReduction * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/100));
			}
		}
		else
		{
			if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS_OT ) )
			{
				iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT );
			}
		}
		/////////////////////////////////////////////////////////////////////////
		iChance -= iPenalty;
	}

	// modify based on morale
	iChance += GetMoraleModifier( pSoldier );

	// modify by fatigue
	iChance -= GetSkillCheckPenaltyForFatigue( pSoldier, iChance );

	// if shooting same target from same position as the last shot
	if (sGridNo == pSoldier->sLastTarget)
	{
		iChance += AIM_BONUS_SAME_TARGET;		// give a bonus to hit
	}

	// ADJUST FOR EXTRA AIMING TIME
	if (ubAimTime)
	{
		iChance += (AIM_BONUS_PER_AP * ubAimTime); // bonus for every pt of aiming
	}

/*
	if (!pSoldier->human)	// if this is a computer AI controlled enemy
	{
		iChance += Diff[DIFF_ENEMY_TO_HIT_MOD][GameOption[ENEMYDIFFICULTY]];
	}
*/

	// if shooter is being affected by gas
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		iChance -= AIM_PENALTY_GASSED;
	}

	// if shooter is being bandaged at the same time, his concentration is off
	if (pSoldier->ubServiceCount > 0)
	{
	  iChance -= AIM_PENALTY_GETTINGAID;
	}

	// if shooter is still in shock
	if (pSoldier->aiData.bShock)
	{
		iChance -= (pSoldier->aiData.bShock * AIM_PENALTY_PER_SHOCK);
	}

	// calculate actual range (in world units)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

	//NumMessage("ACTUAL RANGE = ",range);

	// Snap: Use tohitbonus or aimbonus instead
	/*if ( GetDayVisionRangeBonus (pSoldier) >0) // pSoldier->inv[HEAD1POS].usItem == SUNGOGGLES || pSoldier->inv[HEAD2POS].usItem == SUNGOGGLES)
	{
		// decrease effective range by 10% when using sungoggles (w or w/o scope)
		iRange -= iRange / 10;	//basically, +1% to hit per every 2 squares
	}*/

	//NumMessage("EFFECTIVE RANGE = ",range);

	// ADJUST FOR RANGE

	if ( Item[usHandItem].mortar && iRange < MIN_MORTAR_RANGE)
	{
		return(0);
	}
	else
	{
		iMaxRange = CalcMaxTossRange( pSoldier, usHandItem , TRUE ) * CELL_X_SIZE;

		//NumMessage("MAX RANGE = ",maxRange);

		// bonus if range is less than 1/2 maximum range, penalty if it's more

		// bonus is 50% at range 0, -50% at maximum range

		iChance += 50 * 2 * ( (iMaxRange / 2) - iRange ) / iMaxRange;
		//iChance += ((iMaxRange / 2) - iRange);		// increments of 1% per pixel

		// IF TARGET IS BEYOND MAXIMUM THROWING RANGE
		if (iRange > iMaxRange)
		{
			// the object CAN travel that far if not blocked, but it's NOT accurate!
			iChance /= 2;
		}
	}

	// IF CHANCE EXISTS, BUT ATTACKER IS INJURED
	if ((iChance > 0) && (pSoldier->stats.bLife < pSoldier->stats.bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

		// injury penalty is based on % damage taken (max 2/3rds iChance)
		bPenalty = (2 * iChance * (pSoldier->stats.bLifeMax - pSoldier->stats.bLife + (bBandaged / 2))) /
			 (3 * pSoldier->stats.bLifeMax);

		// for mechanically-fired projectiles, reduce penalty in half
		if ( Item[ usHandItem ].usItemClass == IC_LAUNCHER )
		{
			bPenalty /= 2;
		}

		// reduce injury penalty due to merc's experience level (he can take it!)
		iChance -= (bPenalty * (100 - (10 * ( EffectiveExpLevel( pSoldier ) - 1)))) / 100;
	}

	// IF CHANCE EXISTS, BUT ATTACKER IS LOW ON BREATH
	if ((iChance > 0) && (pSoldier->bBreath < 100))
	{
		// breath penalty is based on % breath missing (max 1/2 iChance)
		bPenalty = (iChance * (100 - pSoldier->bBreath)) / 200;

		// for mechanically-fired projectiles, reduce penalty in half
		if ( Item[ usHandItem ].usItemClass == IC_LAUNCHER )
			bPenalty /= 2;

		// reduce breath penalty due to merc's dexterity (he can compensate!)
		iChance -= (bPenalty * (100 - ( EffectiveDexterity( pSoldier, FALSE ) - 10))) / 100;
	}

	// if iChance exists, but it's a mechanical item being used
	if ((iChance > 0) && (Item[ usHandItem ].usItemClass == IC_LAUNCHER ))
		// reduce iChance to hit DIRECTLY by the item's working condition
		iChance = (iChance * WEAPON_STATUS_MOD(pSoldier->inv[HANDPOS][0]->data.objectStatus)) / 100;

	// HEADROCK HAM 3.2: External divisor for CTH with mortars, now that they are more prevalent in the battlefield.
	// SANDRO - moved this up
	//if ( Item[ usHandItem ].mortar )
	//{
	//	iChance = iChance / gGameExternalOptions.ubMortarCTHDivisor;
	//}

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Bonus CtH for Militia
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sGreenMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sRegularMilitiaCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sVeteranMilitiaCtHBonusPercent) /100);
	// bonus for enemy
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyAdminCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyRegularCtHBonusPercent) /100);
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		iChance += ((iChance * gGameExternalOptions.sEnemyEliteCtHBonusPercent) /100);
	//////////////////////////////////////////////////////////////////////////////////////

	// What's with all these defined limits? Let's think out of the box for a minute, shall we?
	// HEADROCK (HAM): externalized, effective immediately.
	if (iChance < gGameExternalOptions.ubMinimumCTH)
		iChance = gGameExternalOptions.ubMinimumCTH;
	else
	{
		// HEADROCK (HAM): Externalized maximum to JA2_OPTIONS.INI
		//if (iChance > MAXCHANCETOHIT)
		//	iChance = MAXCHANCETOHIT;
		if (iChance > gGameExternalOptions.ubMaximumCTH)
			iChance = gGameExternalOptions.ubMaximumCTH;
	}


	//NumMessage("ThrownChanceToHit = ",iChance);
	return (iChance);
}


void ChangeWeaponMode( SOLDIERTYPE * pSoldier )
{
	// ATE: Don't do this if in a fire amimation.....
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE )
	{
		return;
	}

	do
	{
		pSoldier->bWeaponMode++;
		if(pSoldier->bWeaponMode == NUM_WEAPON_MODES)
		{
			if ( Weapon[pSoldier->inv[HANDPOS].usItem].NoSemiAuto )
				pSoldier->bWeaponMode = WM_AUTOFIRE;
			else
				pSoldier->bWeaponMode = WM_NORMAL;

			if ( HasAttachmentOfClass( &(pSoldier->inv[HANDPOS]), AC_RIFLEGRENADE) )
			{
				OBJECTTYPE* pRifleGrenadeDeviceObj = FindAttachment_GrenadeLauncher( &(pSoldier->inv[HANDPOS]) );

				if ( pRifleGrenadeDeviceObj && FindLaunchableAttachment( &(pSoldier->inv[HANDPOS]), pRifleGrenadeDeviceObj->usItem) )
				{
					pSoldier->bWeaponMode = WM_ATTACHED_GL;
				}
			}
		}
	}
	// Changed by ADB, rev 1513
	//while(IsGunWeaponModeCapable( pSoldier, HANDPOS, pSoldier->bWeaponMode ) == FALSE && pSoldier->bWeaponMode != WM_NORMAL);
	while(IsGunWeaponModeCapable( &pSoldier->inv[HANDPOS], static_cast<WeaponMode>(pSoldier->bWeaponMode), pSoldier ) == FALSE && pSoldier->bWeaponMode != WM_NORMAL);
	
	if (pSoldier->bWeaponMode == WM_AUTOFIRE || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO || pSoldier->bWeaponMode == WM_ATTACHED_UB_AUTO)
	{
		pSoldier->bDoAutofire = 1;
		pSoldier->bDoBurst = 1;
	}
	else if(pSoldier->bWeaponMode == WM_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_UB_BURST)
	{
		pSoldier->bDoAutofire = 0;
		pSoldier->bDoBurst = 1;
	}
	else
	{
		pSoldier->bDoBurst = 0;
		pSoldier->bDoAutofire = 0;	
	}

	// Changed by ADB, rev 1513
	//pSoldier->flags.fDoSpread = 0;
	
	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	gfUIForceReExamineCursorData = TRUE;
}

// Flugente: use a different scope
void ChangeScopeMode( SOLDIERTYPE * pSoldier, INT32 iTrgGridNo )
{
	// just to be safe...
	if ( !pSoldier )
		return;

	// ATE: Don't do this if in a fire animation.....
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE )
		return;

	if ( (&pSoldier->inv[HANDPOS])->exists() != true )
		return;

	INT8 bOldAimTime = pSoldier->aiData.bShownAimTime;

	// SANDRO - if the gun is flagged as HeavyGun, then we can only fire it from hip, thus no scopes to use at all, not even iron sights
	if ( Item[pSoldier->inv[HANDPOS].usItem].twohanded && Weapon[pSoldier->inv[HANDPOS].usItem].HeavyGun && gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
	{
		pSoldier->bScopeMode = USE_ALT_WEAPON_HOLD;
	}
	else
	{
		std::map<INT8, OBJECTTYPE*> ObjList;
		GetScopeLists(&pSoldier->inv[HANDPOS], ObjList);

		do
		{
			pSoldier->bScopeMode++;
			if(pSoldier->bScopeMode == NUM_SCOPE_MODES)
			{
				// SANDRO - this is where we offer firing from alternative weapon hold
				pSoldier->bScopeMode = USE_ALT_WEAPON_HOLD; // USE_BEST_SCOPE;
			}
		}
		while( ObjList[pSoldier->bScopeMode] == NULL && pSoldier->bScopeMode != USE_ALT_WEAPON_HOLD); //USE_BEST_SCOPE);
		
		// If this mode is not allowed, or we are not standing, or holding something weird, skip it
		if ( pSoldier->bScopeMode == USE_ALT_WEAPON_HOLD && ( gGameExternalOptions.ubAllowAlternativeWeaponHolding != 3 || gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND || AM_A_ROBOT(pSoldier) || Item[pSoldier->inv[HANDPOS].usItem].rocketlauncher || Item[pSoldier->inv[HANDPOS].usItem].singleshotrocketlauncher ) )
			pSoldier->bScopeMode = USE_BEST_SCOPE;
	}
	// changing scope mode can change number of aiming levels, we should reset the aiming variable here
	// for the luxury of the payer, I've made it so if he had aiming set to a higher value, we give him the maximum value for the new scope mode,
	// assuming he wants to go for the most accurate shot
	if ( iTrgGridNo != 0 )
	{
		if ( bOldAimTime > AllowedAimingLevels(pSoldier, iTrgGridNo) )
		{
			pSoldier->aiData.bShownAimTime = AllowedAimingLevels(pSoldier, iTrgGridNo);
		}
	}
	else // wtf, reset to zero just to be sure
	{
		pSoldier->aiData.bShownAimTime = 0;
	}

	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	gfUIForceReExamineCursorData = TRUE;
}

void DishoutQueenSwipeDamage( SOLDIERTYPE *pQueenSoldier )
{
	INT8 bValidDishoutDirs[3][3] = { NORTH, NORTHEAST, -1,
																	EAST, SOUTHEAST, -1,
																	SOUTH, -1, -1 };

	UINT32                  cnt, cnt2;
	SOLDIERTYPE							*pSoldier;
	INT8										bDir;
	INT32										iChance;
	INT32										iImpact;
	INT32										iHitBy;

	// Loop through all mercs and make go
	for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
	{
		pSoldier = MercSlots[ cnt ];

		if (pSoldier != NULL )
		{
			if ( pSoldier->ubID != pQueenSoldier->ubID )
			{
				// ATE: Ok, lets check for some basic things here!				
				if ( pSoldier->stats.bLife >= OKLIFE && !TileIsOutOfBounds(pSoldier->sGridNo) && pSoldier->bActive && pSoldier->bInSector )
				{
					UINT16 usRange = GetModifiedGunRange(CREATURE_QUEEN_TENTACLES);

					// Get Pyth spaces away....
					if ( GetRangeInCellCoordsFromGridNoDiff( pQueenSoldier->sGridNo, pSoldier->sGridNo ) <= usRange )
					{
						// get direction
						bDir = (INT8)GetDirectionFromGridNo( pSoldier->sGridNo, pQueenSoldier );

						//
						for ( cnt2 = 0; cnt2 < 2; cnt2++ )
						{
							if ( bValidDishoutDirs[ pQueenSoldier->aiData.uiPendingActionData1 ][ cnt2 ] == bDir )
							{
								iChance = CalcChanceToStab( pQueenSoldier, pSoldier, 0 );

								// CC: Look here for chance to hit, damage, etc...
								// May want to not hit if target is prone, etc....
								iHitBy = iChance - (INT32) PreRandom( 100 );
								if ( iHitBy > 0 )
								{
									// Hit!
									iImpact = HTHImpact( pQueenSoldier, pSoldier, iHitBy, TRUE );
									// SANDRO - new merc records - well, crepitus queen's tentackles are brutal, count them as "stabbed"
									if ( pSoldier->ubProfile != NO_PROFILE )
										gMercProfiles[ pSoldier->ubProfile ].records.usTimesWoundedStabbed++;

			 						pSoldier->EVENT_SoldierGotHit( CREATURE_QUEEN_TENTACLES, (INT16) iImpact, (INT16) iImpact, gOppositeDirection[ bDir ], 50, pQueenSoldier->ubID, 0, ANIM_CROUCH, 0, 0 );
								}
							}
						}
					}
				}
			}
		}
	}

	pQueenSoldier->aiData.uiPendingActionData1++;
}


BOOLEAN WillExplosiveWeaponFail( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{
  if ( pSoldier->bTeam == gbPlayerNum || pSoldier->bVisible == 1 )
  {
	// malus for overheating
	INT16 iOverheatMalus = 0;	
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		FLOAT overheatjampercentage = GetGunOverheatDamagePercentage( pObj);

		if ( overheatjampercentage > 1.0 )
			iOverheatMalus = (INT16)floor(5*overheatjampercentage*overheatjampercentage);

		iOverheatMalus = max(0, iOverheatMalus - 1);
	}

    if ( (INT8)(PreRandom( 40 ) + PreRandom( 40 ) + iOverheatMalus) > (*pObj)[0]->data.objectStatus )
    {
      // Do second dice roll
      if ( PreRandom( 2 ) == 1 )
      {
        // Fail
        return( TRUE );
      }
    }
  }
  return( FALSE );
}


BOOLEAN IsWeapon ( UINT16 itemIndex )
{
	UINT32 usItemClass = Item[itemIndex].usItemClass;
	return (usItemClass == IC_GUN || usItemClass == IC_PUNCH  || usItemClass == IC_BLADE   ||
		usItemClass == IC_LAUNCHER  || usItemClass == IC_THROWING_KNIFE  || usItemClass == IC_THROWN  ||
		usItemClass == IC_GRENADE  || usItemClass == IC_TENTACLES);
}

UINT8 GetDamage ( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetDamage"));

	if ( Item[pObj->usItem].usItemClass == IC_BLADE || Item[pObj->usItem].usItemClass == IC_PUNCH || Item[pObj->usItem].usItemClass == IC_TENTACLES )
	{
		UINT8 ubDamage;
		// HEADROCK HAM 3.6: Can now take a negative modifier 
		if(UsingNewCTHSystem() == true)
		{
			ubDamage = Weapon[ pObj->usItem ].ubImpact;
			ubDamage += GetMeleeDamageBonus(pObj);
			ubDamage = (UINT8)GetModifiedMeleeDamage( ubDamage );
		}
		else
		{
			ubDamage = (UINT8)GetModifiedMeleeDamage( Weapon[ pObj->usItem ].ubImpact );
			ubDamage += GetMeleeDamageBonus(pObj);
		}
		//return min(255, (UINT8)( (ubDamage) + ( (double)ubDamage / 100) * gGameExternalOptions.iMeleeDamageModifier ) );
		return min(255, (UINT8)ubDamage);
	}
	else
	{
		UINT8 ubDamage;
		// HEADROCK HAM 3.6: Can now take a negative modifier 
		if(UsingNewCTHSystem() == true)
		{
			ubDamage = Weapon[ pObj->usItem ].ubImpact;
			// HEADROCK HAM 4: I've decided to remove this condition. It makes no friggin sense.
			//if (FitsInSmallPocket(pObj) == true)
			//{
				ubDamage += GetDamageBonus(pObj);
			//}
			ubDamage = (UINT8)GetModifiedGunDamage( ubDamage );
		}
		else
		{
			ubDamage = (UINT8)GetModifiedGunDamage( Weapon[ pObj->usItem ].ubImpact );

			// WTF? Why do only small weapons get their damage bonus?!
			if (FitsInSmallPocket(pObj) == true)
			{
				ubDamage += GetDamageBonus(pObj);
			}
		}
		return min(255, (UINT8)ubDamage );
	}
}

// HEADROCK HAM 4: Same function as above, but without modifiers from attached items.
UINT8 GetBasicDamage ( OBJECTTYPE *pObj )
{
	if ( Item[pObj->usItem].usItemClass == IC_BLADE || Item[pObj->usItem].usItemClass == IC_PUNCH || Item[pObj->usItem].usItemClass == IC_TENTACLES )
	{
		// HEADROCK HAM 3.6: Can now take a negative modifier 
		UINT8 ubDamage = (UINT8)GetModifiedMeleeDamage( Weapon[ pObj->usItem ].ubImpact );
		return ubDamage;
	}
	else
	{
		// HEADROCK HAM 3.6: Can now take a negative modifier 
		UINT8 ubDamage = (UINT8)GetModifiedGunDamage( Weapon[ pObj->usItem ].ubImpact );
		return ubDamage;
	}
}

UINT8 GetShotsPerBurst( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetShotsPerBurst"));

	return Weapon[ pObj->usItem ].ubShotsPerBurst  + GetBurstSizeBonus(pObj);

}
UINT8 GetBurstPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance )
{
	// Snap: Make sure burst bonus does not exceed burst penalty!
	INT16 bns = GetBurstToHitBonus(pObj, fProneStance);

	if ( bns > Weapon[ pObj->usItem ].ubBurstPenalty ) {
		return 0;
	}

	return Weapon[ pObj->usItem ].ubBurstPenalty - bns;
}
UINT8 GetAutoPenalty( OBJECTTYPE *pObj, BOOLEAN fProneStance )
{
	// Snap: Make sure burst bonus does not exceed burst penalty!
	INT16 bns = GetAutoToHitBonus(pObj, fProneStance);

	if ( bns > Weapon[ pObj->usItem ].AutoPenalty ) {
		return 0;
	}

	return Weapon[ pObj->usItem ].AutoPenalty - bns;
}

// HEADROCK HAM 4: This determines whether the previous bullet in a volley was a tracer bullet.
BOOLEAN WasPrevBulletATracer( SOLDIERTYPE *pSoldier, OBJECTTYPE *pWeapon )
{

	if (!AmmoTypes[ (*pWeapon)[0]->data.gun.ubGunAmmoType ].tracerEffect)
	{
		// Not a tracer magazine
		return (FALSE);
	}
	if (pSoldier->bDoBurst == 0)
	{
		// No volley? Tracers are irrelevant
		return (FALSE);
	}

	if (gGameExternalOptions.ubRealisticTracers == 0 || gGameExternalOptions.ubNumBulletsPerTracer == 1)
	{
		// All bullets are tracers anyway.
		return (TRUE);
	}

	UINT16 usShotsLeft = (*pWeapon)[0]->data.gun.ubGunShotsLeft;
	UINT16 usMagSize = GetMagSize(pWeapon);
	UINT16 usBulletsFired = usMagSize-usShotsLeft;
	UINT16 usBulletsSinceTracer = (usBulletsFired+1)%gGameExternalOptions.ubNumBulletsPerTracer;
	if (usBulletsSinceTracer == 1)
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
 }

UINT8 GetAutofireShotsPerFiveAPs( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetAutofireShotsPerFiveAPs"));

//	HEADROCK HAM B2.6: Added overall modifier
	if (Weapon[ pObj->usItem ].bAutofireShotsPerFiveAP > 0)
	{
		// WANNE: Fix by Headrock
		// Weapons shouldn't ever lose their Bp5AP due to this modifier.
		return __max((Weapon[ pObj->usItem ].bAutofireShotsPerFiveAP + gGameExternalOptions.bAutofireBulletsPer5APModifier), 1);
		
		//return __max((Weapon[ pObj->usItem ].bAutofireShotsPerFiveAP + gGameExternalOptions.bAutofireBulletsPer5APModifier), 0);
	}
	else
		return 0;

}
UINT16 GetMagSize( OBJECTTYPE *pObj, UINT8 subObject )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetMagSize"));

	return Weapon[ pObj->usItem ].ubMagSize + GetMagSizeBonus(pObj, subObject);
}

UINT16 GetExpMagSize( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetMagSize"));
	UINT16	magSize = 0;
	if(Item[pObj->usItem].usItemClass & IC_EXPLOSV)
		magSize = Explosive[Item[pObj->usItem].ubClassIndex].ubMagSize;
	else
		magSize = Weapon[pObj->usItem].ubMagSize;

	//return Weapon[ pObj->usItem ].ubMagSize + GetMagSizeBonus(pObj);
	return magSize;
}

// HEADROCK HAM 3.3: Function to get a weapon's current ammotype.
UINT8 GetAmmoType( OBJECTTYPE *pObj )
{
	return (*pObj)[0]->data.gun.ubGunAmmoType;
}

bool WeaponReady(SOLDIERTYPE * pSoldier)
{
#ifdef ROBOT_ALWAYS_READY
	if ( AM_A_ROBOT( pSoldier) )
		return true;
#endif
	if (gbForceWeaponNotReady)
		return false;
	if (gbForceWeaponReady)
		return true;
	if (( (gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY ) ||
		(gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIRE ) ) &&
		!(gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ALT_WEAPON_HOLDING ) )
		return true;
	else
		return false;
}

INT8 GetAPsToReload( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetAPsToReload"));

	return ( Weapon[ pObj->usItem ].APsToReload *
		( 100 - GetPercentReloadTimeAPReduction(pObj) ) ) / 100;

}

// HEADROCK HAM 3.4: Estimates the number of bullets left in the gun. For use during combat.

CHAR16 gBulletCount[10]; // This is a global containing the bullet count string
void EstimateBulletsLeft( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{

	UINT16 usExpLevel;
	UINT32 usDexterity;
	UINT32 usWisdom;
	UINT16 ubMagSize = Weapon[pObj->usItem].ubMagSize;
	UINT16 usRealBulletCount = (*pObj)[0]->data.gun.ubGunShotsLeft;
	UINT16 i = 0;
	BOOLEAN fPsycho = FALSE;
	INT16 sEffectiveSkill;
	INT8 bDeviation = 0;

	// HEADROCK HAM 3.5: Bugfix, failsafe
	if ( pSoldier == NULL )
	{
		// No soldier... Return true count.
		swprintf(gBulletCount, L"%d", usRealBulletCount);
		return;
	}

	usExpLevel = EffectiveExpLevel(pSoldier);
	usDexterity = EffectiveDexterity(pSoldier, FALSE);
	usWisdom = EffectiveWisdom(pSoldier);

	if ( gGameExternalOptions.usBulletHideIntensity <= 0 )
	{
		// Feature is disabled. Print the real bullet count.
		swprintf(gBulletCount, L"%d", usRealBulletCount);
		return;
	}

	// Is this Soldier a psycho?
	// Flugente: drugs can temporarily cause a merc to go psycho
	if ( pSoldier->ubProfile != NO_PROFILE && (gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO || MercUnderTheInfluence(pSoldier, DRUG_TYPE_PSYCHO) ) )
	{
		fPsycho = TRUE;
	}

	// High EXP Level, Wisdom and Dexterity required for any estimation to be possible.
	// When Experience goes up, the required WIS+DEX goes down. 
	// At ExpLevel 1 -> WIS+DEX must be > 180. A high requirement!
	// At ExpLevel 2 -> WIS+DEX must be > 160.
	// ...
	// At ExpLevel 5 -> WIS+DEX must be > 100. Most characters have already attained estimation ability by now.
	// At ExpLevel 10 -> WIS+DEX must be > 0, which is always.
	if ( (usWisdom + usDexterity) < (200 - (usExpLevel * 20)) )
	{
		// Soldier is not skilled enough to know how many bullets are left in the gun. Print a "??" indicating that
		// the real bullet count may be anywhere between empty and full.
		swprintf(gBulletCount, L"%s", L"??");
		return;
	}
	else // Soldier good enough for at least a rough estimate.
	{
		// HEADROCK HAM 3.5: Moved this here. If the character fails the above requirement, he shouldn't even know if the
		// gun is empty or full.
		//-------------------------
		// If the magazine is empty or full, he knows it automatically.
		if (usRealBulletCount == ubMagSize)
		{
			// Magazine is fresh. Let the soldier know this.
			swprintf(gBulletCount, L"%d", usRealBulletCount);
			return;
		}
		if (usRealBulletCount == 0)
		{
			// Magazine is empty, so it will also show as empty.
			swprintf(gBulletCount, L"%d", usRealBulletCount);
			return;
		}
		//-------------------------

		// Let's see by how much we've beaten the requirement!
		sEffectiveSkill = ( (usWisdom + usDexterity) - (200 - (usExpLevel * 20)) ) / 2;
	}
	// So from now on we've got the sEffectiveSkill value, which can go from 0 to 100
	// This value represents getting better in all three stats (WIS,DEX,EXP), because as
	// they go up the result of the calculation above also goes up. Having a particularly high
	// value in any of the three stats, in fact, reduces the need to have high values in others.
	//
	// The higher your EXP level, the less WIS+DEX you need for an accurate estimate, so even
	// inept characters will eventually be able to make a fair estimate, provided the EXP
	// level goes high enough.
	//
	// On the other hand, WIS and DEX work together. In fact, it is the average of these
	// skills that determines how soon you can start estimating, and how good your estimation gets
	// as you gain levels. So while one high skill can bring better estimates sooner, it requires 
	// both skills to be improved for a really good estimate.
	//
	// The range of sEffectiveSkill is 0 to 100
	//``````````````````````````````````````````

	// Psychos are eligible for an estimate just the same as any other character. But the trait reduces 
	// effective skill by 10, so It'll take them longer before they can get a good estimate.
	sEffectiveSkill -= (fPsycho * (10));

	sEffectiveSkill = __max(0, sEffectiveSkill);
	sEffectiveSkill = __min(100, sEffectiveSkill);
	
	// Now, we invert the effective skill
	bDeviation = 100-sEffectiveSkill; // range is still 0-100, but lower is better
	// Use this as a percentage, and figure out the deviation based on magazine size and current bullet. The nearer
	// you get to the bottom of a magazine, the harder it is to accurately estimate how many bullets are in there.
	bDeviation = ((ubMagSize - usRealBulletCount) * (bDeviation * bDeviation)) / 10000;

	// Add externalized difficulty modifier
	bDeviation = (bDeviation * gGameExternalOptions.usBulletHideIntensity) / 100;

	// If the deviation surpasses the character's EXP level, then he/she is not able to know how many bullets are
	// left in the gun, but can still give a rough estimate. There are currently three estimate stages - High, Mid, 
	// and Low.
	if (bDeviation > usExpLevel)
	{
		if (usRealBulletCount >= (ubMagSize*2)/3)
		{
			swprintf(gBulletCount, L"%s", L"?H");
		}
		else if (usRealBulletCount < (ubMagSize*2)/3 && usRealBulletCount >= ubMagSize/3)
		{
			swprintf(gBulletCount, L"%s", L"?M");
		}
		else if (usRealBulletCount < ubMagSize/3)
		{
			swprintf(gBulletCount, L"%s", L"?L");
		}
		return;
	}

	// Default - return true count.
	swprintf(gBulletCount, L"%d", usRealBulletCount);
	return;
	
}

// HEADROCK HAM 4: Calculate the ratio between current Mag Factor and Target Factor. Used for the CTH bars.
void CalcMagFactorSimple( SOLDIERTYPE *pSoldier, FLOAT d2DDistance, INT16 bAimTime, INT32 iGridNo )
{
	OBJECTTYPE *pWeapon = &(pSoldier->inv[pSoldier->ubAttackingHand]);
		
	FLOAT iActualMagFactor = 0;
	FLOAT iHighestMagFactor = 0;
	FLOAT iScopeFactor = 0;
	FLOAT iProjectionFactor = 0;
		
	FLOAT iTargetMagFactor = d2DDistance / gGameCTHConstants.NORMAL_SHOOTING_DISTANCE;
	FLOAT	 rangeModifier = GetScopeRangeMultiplier(pSoldier, pWeapon, d2DDistance);

	// Flugente: when using scope modes, use scopes
	if ( gGameExternalOptions.fScopeModes || bAimTime > 0 )
	{
		if ( !pSoldier->IsValidAlternativeFireMode( bAimTime, iGridNo ) )
			iScopeFactor = GetBestScopeMagnificationFactor( pSoldier, pWeapon, d2DDistance );
		else			
			iScopeFactor = 1.0f;
		// Set a display variable
		gCTHDisplay.ScopeMagFactor = iScopeFactor;

		iScopeFactor = __min(iScopeFactor, __max(1.0f, iTargetMagFactor/rangeModifier));

		// With the reworked NCTH code we don't want to use iProjectionFactor anymore. 
		// Instead we use the performance bonus if at least one bonus is != 0. Otherwise -> continue using Projection Factor.
		if (gGameExternalOptions.fUseNewCTHCalculation 
			&& ( gGameCTHConstants.LASER_PERFORMANCE_BONUS_HIP + gGameCTHConstants.LASER_PERFORMANCE_BONUS_IRON + gGameCTHConstants.LASER_PERFORMANCE_BONUS_SCOPE != 0 ))
			iProjectionFactor = 1.0;
		else
			iProjectionFactor = CalcProjectionFactor(pSoldier, pWeapon, d2DDistance, (UINT8)bAimTime);

		// Set a display variable
		gCTHDisplay.ProjectionFactor = iProjectionFactor;

		// Set a display variable
		gCTHDisplay.iBestLaserRange = GetBestLaserRange( pWeapon );

		// The final factor is the largest of the two.
		iHighestMagFactor = __max( iScopeFactor, iProjectionFactor );
	}
	else
	{
		gCTHDisplay.ScopeMagFactor = 1.0;
		gCTHDisplay.ProjectionFactor = 1.0;
		gCTHDisplay.iBestLaserRange = GetBestLaserRange( pWeapon );
		iHighestMagFactor = 1.0;
	}

	gCTHDisplay.FinalMagFactor = iHighestMagFactor;

}

// Flugente: Increase temperature/dirt of gun in ubAttackingHand due to firing a shot
void GunIncreaseHeat( OBJECTTYPE *pObj, SOLDIERTYPE* pSoldier )
{
	if ( gGameExternalOptions.fWeaponOverheating )
	{
		if ( Item[pObj->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )							// if item is a gun pr launcher...
		{
		  FLOAT guntemperature = (*pObj)[0]->data.bTemperature;									// ... get current temperature ...

		  FLOAT singleshottemperature = GetSingleShotTemperature( pObj );						// ... get temperature rise ...

		  FLOAT newguntemperature = min(guntemperature + singleshottemperature, OVERHEATING_MAX_TEMPERATURE );					// ... calculate new temperature ...

		  (*pObj)[0]->data.bTemperature = newguntemperature;									// ... apply new temperature

#ifdef JA2TESTVERSION
		  ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Gun temperature increased from %4.2f to %4.2f", guntemperature, newguntemperature );
#endif
		}
	}

	// firing a gun also increases dirt
	if ( gGameExternalOptions.fDirtSystem )
	{
		FLOAT dirtincreasefactor = GetItemDirtIncreaseFactor(pObj, TRUE);			// ... get dirt increase factor ...

		// get sector-specific dirt threshold
		UINT16 sectormod = 0;
		UINT8 ubSectorId = SECTOR(gWorldSectorX, gWorldSectorY);	
		if ( gbWorldSectorZ > 0 )
			sectormod = 100;
		else if ( ubSectorId >= 0 && ubSectorId < 256  )
		{
			sectormod = SectorExternalData[ubSectorId][gbWorldSectorZ].usNaturalDirt;
		}

		// the current sector determines how much dirt increases
		dirtincreasefactor *= (sectormod)/100;

		if ( dirtincreasefactor > 0.0f )									// ... item can get dirtier ...
		{
			(*pObj)[0]->data.bDirtLevel = min((*pObj)[0]->data.bDirtLevel + dirtincreasefactor, OVERHEATING_MAX_TEMPERATURE );	// dirt and overheating use the same threshold
		}
	}
}

// Flugente: Overheating Weapons
FLOAT GetTemperatureModifier( OBJECTTYPE *pObj )
{
	// determine modificator according to attachments
	FLOAT modificator = Item[pObj->usItem].overheatTemperatureModificator;

	attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if (iter->exists())
		{
			modificator += Item[iter->usItem].overheatTemperatureModificator;
		}
	}

	return modificator;
}

FLOAT GetSingleShotTemperature( OBJECTTYPE *pObj )
{
	FLOAT singleshottemperature = 0.0;

	if ( Item[pObj->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		singleshottemperature = Weapon[ pObj->usItem ].usOverheatingSingleShotTemperature;

		// determine modificator according to attachments
		FLOAT modificator = 1.0f + GetTemperatureModifier(pObj);

		// determine modificator according to ammo
		modificator += AmmoTypes[(*pObj)[0]->data.gun.ubGunAmmoType].temperatureModificator;

		singleshottemperature *= modificator;

		// multiply again for global modifer
		singleshottemperature *= gGameExternalOptions.iOverheatTemperatureGlobalModfier;
	}

	return singleshottemperature;
}

// Flugente: Get percentage: temperature/damagethreshold
FLOAT   GetGunOverheatDamagePercentage( OBJECTTYPE * pObj )
{
	FLOAT damagethreshold = GetOverheatDamageThreshold(pObj);

	FLOAT temperature = (*pObj)[0]->data.bTemperature;

	// to avert nonsense...
	damagethreshold = max( (FLOAT)(1.0), damagethreshold);

	return temperature/ damagethreshold ;
}

// Flugente: Get percentage: temperature/jamthreshold
FLOAT   GetGunOverheatJamPercentage( OBJECTTYPE * pObj )
{
	FLOAT jamthreshold = GetOverheatJamThreshold(pObj);

	FLOAT temperature = (*pObj)[0]->data.bTemperature;

	// to avert nonsense...
	jamthreshold = max( (FLOAT)(1.0), jamthreshold);

	return temperature/ jamthreshold ;
}

FLOAT GetOverheatJamThresholdModifier( OBJECTTYPE *pObj )
{
	// determine modificator according to attachments
	FLOAT modificator = Item[pObj->usItem].overheatJamThresholdModificator;

	attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if (iter->exists())
		{
			modificator += Item[iter->usItem].overheatJamThresholdModificator;
		}
	}

	return modificator;
}

FLOAT GetOverheatJamThreshold( OBJECTTYPE *pObj )
{
	FLOAT jamthreshold = OVERHEATING_MAX_TEMPERATURE / 4.0f;

	if ( Item[pObj->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		jamthreshold = Weapon[ pObj->usItem ].usOverheatingJamThreshold;

		// determine modificator according to attachments
		FLOAT modificator = 1.0f + GetOverheatJamThresholdModifier(pObj);

		jamthreshold *= modificator;
	}

	return jamthreshold;
}

FLOAT GetOverheatDamageThresholdModifier( OBJECTTYPE *pObj )
{
	// determine modificator according to attachments
	FLOAT modificator = Item[pObj->usItem].overheatDamageThresholdModificator;

	attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
	for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if (iter->exists())
		{
			modificator += Item[iter->usItem].overheatDamageThresholdModificator;
		}
	}

	return modificator;
}

FLOAT GetOverheatDamageThreshold( OBJECTTYPE *pObj )
{
	FLOAT damagethreshold = OVERHEATING_MAX_TEMPERATURE / 4.0f;

	if ( Item[pObj->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		damagethreshold = Weapon[ pObj->usItem ].usOverheatingDamageThreshold;

		// determine modificator according to attachments
		FLOAT modificator = 1.0f + GetOverheatDamageThresholdModifier(pObj);

		damagethreshold *= modificator;
	}

	return damagethreshold;
}

FLOAT CalcNewChanceToHitBaseAttributeBonus(SOLDIERTYPE *pSoldier)
{
	INT8 bExperience = EffectiveExpLevel( pSoldier );
	INT8 bMarksmanship = EffectiveMarksmanship( pSoldier );
	INT16 bDexterity = EffectiveDexterity( pSoldier, FALSE );
	INT16 bWisdom = EffectiveWisdom( pSoldier );

	// this shooter will never hit anything
	if ( bMarksmanship == 0 || bDexterity == 0 )
	{
		return( gGameExternalOptions.ubMinimumCTH );
	}
	
	FLOAT fCombinedSkill = (FLOAT)(gGameCTHConstants.BASE_EXP*(bExperience*10));
	fCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_MARKS * bMarksmanship);
	fCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_DEX * bDexterity);
	fCombinedSkill += (FLOAT)(gGameCTHConstants.BASE_WIS * bWisdom);

	// Divide by the total coefficient value.
    fCombinedSkill /= (FLOAT)(gGameCTHConstants.BASE_EXP + gGameCTHConstants.BASE_MARKS + gGameCTHConstants.BASE_DEX + gGameCTHConstants.BASE_WIS);

	// At this point we have a value of 1 to 100. Divide by 3.
	fCombinedSkill /= 3;

	return fCombinedSkill;
}

FLOAT CalcNewChanceToHitBaseEffectBonus(SOLDIERTYPE *pSoldier)
{
	FLOAT fBaseModifier = 0;
	
	// MORALE
	INT8 iCurrentMorale = GetMoraleModifier(pSoldier);
	if (iCurrentMorale > 0)
	{
		fBaseModifier += (FLOAT)(iCurrentMorale * gGameCTHConstants.BASE_HIGH_MORALE);
	}
	else if (iCurrentMorale < 0)
	{
		fBaseModifier += (FLOAT)(iCurrentMorale * gGameCTHConstants.BASE_LOW_MORALE);
	}

	// INJURY
	if (pSoldier->stats.bLife < pSoldier->stats.bLifeMax)
	{
		FLOAT fTempPenalty = gGameCTHConstants.BASE_INJURY;

		// Bleeding damage is used as a percentage off the Max Life, giving a penalty of up to BASE_INJURY.
		fBaseModifier += (FLOAT)(pSoldier->bBleeding * fTempPenalty) / pSoldier->stats.bLifeMax;

		// Bandage damage is used similarly, but is only 1/3 as bad.
		INT8 bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
		fBaseModifier += ((bBandaged * fTempPenalty) / pSoldier->stats.bLifeMax) / 3;
	}

	// FATIGUE
	fBaseModifier += (gGameCTHConstants.BASE_FATIGUE * (100 - pSoldier->bBreath)) / 100;

	// DRUNKNESS
	INT8 iDrunkness = GetDrunkLevel(pSoldier);
	if (iDrunkness > 0)
	{
		iDrunkness -= 1;
		fBaseModifier += gGameCTHConstants.BASE_DRUNK[iDrunkness];
	}

	// GASSED
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		fBaseModifier += gGameCTHConstants.BASE_GASSED;
	}

	// BEING BANDAGED
	if (pSoldier->ubServiceCount > 0)
	{
		fBaseModifier += gGameCTHConstants.BASE_BEING_BANDAGED;
	}

	// SHOCK (SHOOTER)
	if (pSoldier->aiData.bShock)
	{
		FLOAT fTempPenalty = gGameCTHConstants.BASE_SHOCK;
		FLOAT fShockPercentage = (FLOAT)(pSoldier->aiData.bShock * 100) / gGameExternalOptions.ubMaxSuppressionShock;

		fBaseModifier += (fTempPenalty * fShockPercentage) / 100;
	}

	return fBaseModifier;
}

FLOAT CalcNewChanceToHitBaseWeaponBonus(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, FLOAT fGunBaseDifficulty, UINT8 stance)
{
	FLOAT fBaseModifier = 0;
	UINT16 usInHand = pSoldier->usAttackingWeapon;
	OBJECTTYPE * pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);

	// FIRING FROM ALTERNATIVE WEAPON HOLDING (faster shot, less accuracy)
	if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
	{
		fBaseModifier -= gGameExternalOptions.ubAltWeaponHoldingCtHPenaly;
	}
		
	// FIRING 1-HANDED WEAPONS
	if ( !(Item[ usInHand ].twohanded ) )	//JMich todo: underbarrel
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() != false)
		{
			if ( pSoldier->IsValidSecondHandShot( ) )
			{
				// Penalty for shooting two pistols. Ambidextrous trait halves this.
				FLOAT fTempPenalty = (gGameCTHConstants.BASE_TWO_GUNS * fGunBaseDifficulty) - fGunBaseDifficulty;
				if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
				{
					fTempPenalty = fTempPenalty * (100 - gSkillTraitValues.ubAMPenaltyDoubleReduction) / 100;
				}
				else if (!gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXT_OT ))
				{
					fTempPenalty = 0;
				}
				
				fGunBaseDifficulty += fTempPenalty;
			}
			else
			{
				// Penalty for shooting a pistol with just one hand
				fGunBaseDifficulty *= gGameCTHConstants.BASE_ONE_HANDED;
			}
		}
	}

	// HEAVY WEAPON in OLD trait system
	if (Item[usInHand].rocketlauncher && !(gGameOptions.fNewTraitSystem) ) 
	{
		// Penalty for shooting heavy weapons (launchers). Heavy Weapons skill halves this, once per skill level.
		FLOAT fTempPenalty = (gGameCTHConstants.BASE_HEAVY_WEAPON * fGunBaseDifficulty) - fGunBaseDifficulty;

		for (INT32 x=0; x < NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT ); x++)
		{
			fTempPenalty /= 2;
		}

		fGunBaseDifficulty += fTempPenalty;
	}

	// STANCE
	switch ( stance )
	{
		case ANIM_CROUCH:
			fGunBaseDifficulty *= gGameCTHConstants.BASE_CROUCHING_STANCE;
			break;

		case ANIM_PRONE:
			fGunBaseDifficulty *= gGameCTHConstants.BASE_PRONE_STANCE;
			break;
	}

	// Percentage based handling modifier from the gun and its attachments
	FLOAT moda = (fGunBaseDifficulty * GetPercentHandlingModifier( pInHand, stance )) / 100;
	FLOAT modb = (fGunBaseDifficulty * GetPercentHandlingModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
	fGunBaseDifficulty += ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 
	
	// Now apply Gun Difficulty to the Base Modifier.
	fBaseModifier -= fGunBaseDifficulty * gGameCTHConstants.BASE_DRAW_COST;

	// Percentage based modifier from the weapon and its attachments
	moda = (fGunBaseDifficulty * GetPercentBaseModifier( pInHand, stance )) / 100;
	modb = (fGunBaseDifficulty * GetPercentBaseModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
	fBaseModifier += ((gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100); 

	return fBaseModifier;
}

FLOAT CalcNewChanceToHitBaseSpecialBonus(SOLDIERTYPE *pSoldier)
{
	FLOAT fBaseModifier = 0;
	
		/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Bonus CtH for Militia
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sGreenMilitiaCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sRegularMilitiaCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sVeteranMilitiaCtHBonusPercent;
	// bonus for enemy
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sEnemyAdminCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sEnemyRegularCtHBonusPercent;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteCtHBonusPercent != 0)
		fBaseModifier += gGameExternalOptions.sEnemyEliteCtHBonusPercent;
		
	// SANDRO - option to make special NPCs stronger - chance to hit
	if (gGameExternalOptions.usSpecialNPCStronger > 0)
	{
		switch( pSoldier->ubProfile )
		{
			case CARMEN:
			case QUEEN:
			case JOE:
			case ANNIE:
			case CHRIS:
			case KINGPIN:
			case TIFFANY:
			case T_REX:
			case DRUGGIST:
			case GENERAL:
			case JIM:
			case JACK:
			case OLAF:
			case RAY:
			case OLGA:
			case TYRONE:
			case MIKE:
				fBaseModifier += gGameExternalOptions.usSpecialNPCStronger;
				break;
		}
	}

	// GAME DIFFICULTY
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) && (pSoldier->bSide != gbPlayerNum) )
	{
		fBaseModifier += gGameCTHConstants.BASE_DIFFICULTY[gGameOptions.ubDifficultyLevel];
	}

	return fBaseModifier;
}

FLOAT CalcNewChanceToHitBaseTargetBonus(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget, INT32 sGridNo, INT32 iRange, UINT8 ubAimPos, BOOLEAN fCantSeeTarget)
{
	FLOAT fBaseModifier = 0;
	INT32 iHeightDifference = 0;

	// SHOOTING AT SAME TARGET AGAIN
	if (sGridNo == pSoldier->sLastTarget )
		fBaseModifier += (FLOAT)gGameCTHConstants.BASE_SAME_TARGET;

	// SHOOTING AT A TARGET AT DIFFERENT HEIGHT?
	if ( pTarget == NULL )
	{
		// Shooting to roof.
		if ( pSoldier->bTargetLevel > pSoldier->pathing.bLevel )
		{
			iHeightDifference = 3 * pSoldier->bTargetLevel;
		}
	}
	else
	{
		// HEIGHT DIFFERENCE

		UINT32 uiShooterHeight = 0;
		UINT32 uiTargetHeight = 0;

		if ( pSoldier->pathing.bLevel > 0 )
		{
			uiShooterHeight += 3 * pSoldier->pathing.bLevel;
		}
		
		switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				uiShooterHeight += 2;
				break;
			case ANIM_CROUCH:
				uiShooterHeight += 1;
				break;
		}

		if (pTarget->pathing.bLevel > 0)
		{
			uiTargetHeight += 3 * pTarget->pathing.bLevel;
		}

		switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 2;
						break;
					case AIM_SHOT_TORSO:
					case AIM_SHOT_RANDOM:
					case AIM_SHOT_GLAND:
						uiTargetHeight += 1;
						break;
				}
				break;
			case ANIM_CROUCH:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 1;
						break;
				}
				break;
		}

		iHeightDifference = uiShooterHeight - uiTargetHeight;
		if (iHeightDifference < 0)
		{
			iHeightDifference *= -1;
		}
		else
		{
			iHeightDifference = 0;
		}

	}

	// Height difference is mitigated by range. A LONGER range reduces this penalty!
	if (iRange > 0 && iHeightDifference > 0)
	{
		FLOAT fTempPenalty = gGameCTHConstants.BASE_SHOOTING_UPWARDS * iHeightDifference;
		fTempPenalty /= iRange;

		fBaseModifier += fTempPenalty;
	}
	
	// Flugente: there needs to be a target...
	if ( pTarget )
	{
		// Target has high Agility or Experience and is therefore harder to hit
		FLOAT fTempPenalty = (FLOAT)__max((pTarget->stats.bExpLevel*10), pTarget->stats.bAgility);
		fBaseModifier += (fTempPenalty * gGameCTHConstants.BASE_AGILE_TARGET) / 100;
	}

	// INVIS TARGET
	if (fCantSeeTarget)
	{
		fBaseModifier += gGameCTHConstants.BASE_TARGET_INVISIBLE;
	}

	return fBaseModifier;
}

FLOAT CalcNewChanceToHitAimAttributeBonus(SOLDIERTYPE *pSoldier)
{
	INT8 bExperience = EffectiveExpLevel( pSoldier );
	INT8 bMarksmanship = EffectiveMarksmanship( pSoldier );
	INT16 bDexterity = EffectiveDexterity( pSoldier, FALSE );
	INT16 bWisdom = EffectiveWisdom( pSoldier );

	// Start by calculating the CTH cap, the absolute maximum CTH a shooter can get.
	FLOAT fCombinedSkill = (FLOAT)( gGameCTHConstants.AIM_EXP * bExperience * 10 );
	fCombinedSkill += (FLOAT)( gGameCTHConstants.AIM_MARKS * bMarksmanship );
	fCombinedSkill += (FLOAT)( gGameCTHConstants.AIM_DEX * bDexterity );
	fCombinedSkill += (FLOAT)( gGameCTHConstants.AIM_WIS * bWisdom );

	// Divide by the total coefficient value.
	fCombinedSkill /= (FLOAT)(gGameCTHConstants.AIM_EXP + gGameCTHConstants.AIM_MARKS + gGameCTHConstants.AIM_DEX + gGameCTHConstants.AIM_WIS);
	
	return fCombinedSkill;
}

FLOAT CalcNewChanceToHitAimEffectBonus(SOLDIERTYPE *pSoldier)
{
	FLOAT fAimModifier = 0;
	
	// MORALE
	INT32 iTempModifier = GetMoraleModifier( pSoldier );
	if ( iTempModifier > 0 )
	{
		fAimModifier += gGameCTHConstants.AIM_HIGH_MORALE * iTempModifier;
	}
	else if ( iTempModifier < 0 )
	{
		fAimModifier += gGameCTHConstants.AIM_LOW_MORALE * iTempModifier;
	}

	// Flugente: drugs can temporarily cause a merc to go psycho
	if ( pSoldier->ubProfile != NO_PROFILE && (gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO || MercUnderTheInfluence(pSoldier, DRUG_TYPE_PSYCHO) ) )
	{
		fAimModifier += gGameCTHConstants.AIM_PSYCHO;
	}
	
	// INJURY
	if (pSoldier->stats.bLife < pSoldier->stats.bLifeMax)
	{
		FLOAT fTempPenalty = gGameCTHConstants.AIM_INJURY;

		// Bleeding damage is used as a percentage off the Max Life, giving a penalty of up to BASE_INJURY.
		fAimModifier += (pSoldier->bBleeding * fTempPenalty) / pSoldier->stats.bLifeMax;

		// Bandage damage is used similarly, but is only 1/3 as bad.
		INT8 bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;
		fAimModifier += ((bBandaged * fTempPenalty) / pSoldier->stats.bLifeMax) / 3;
	}

	// FATIGUE
	fAimModifier += (gGameCTHConstants.AIM_FATIGUE * (100 - pSoldier->bBreath)) / 100;

	// DRUNKNESS
	INT8 iDrunkness = GetDrunkLevel(pSoldier);
	if (iDrunkness > 0)
	{
		iDrunkness -= 1;
		fAimModifier += gGameCTHConstants.AIM_DRUNK[iDrunkness];
	}

	// GASSED
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		fAimModifier += gGameCTHConstants.AIM_GASSED;
	}

	// BEING BANDAGED
	if (pSoldier->ubServiceCount > 0)
	{
		fAimModifier += gGameCTHConstants.AIM_BEING_BANDAGED;
	}

	// SHOCK (SHOOTER)
	if (pSoldier->aiData.bShock)
	{
		FLOAT fTempPenalty = gGameCTHConstants.AIM_SHOCK;
		FLOAT fShockPercentage = (FLOAT)(pSoldier->aiData.bShock * 100) / gGameExternalOptions.ubMaxSuppressionShock;
			fAimModifier += (fTempPenalty * fShockPercentage) / 100;
	}
		
	return fAimModifier;
}

FLOAT CalcNewChanceToHitAimWeaponBonus(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 ubAimTime, FLOAT fGunAimDifficulty, UINT8 stance)
{
	FLOAT fAimModifier = 0;
	UINT16 usInHand = pSoldier->usAttackingWeapon;
	OBJECTTYPE * pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);

	// "AIMING" FROM ALTERNATIVE WEAPON HOLDING (faster shots, no scopes, less accuracy)
	if ( pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ))
	{
		//due to the way aiming levels are handled in NCTH, this penalty is increased here by 1/3 (it is harmonized by reduced aiming clicks)
		fAimModifier -= gGameExternalOptions.ubAltWeaponHoldingAimingPenaly * 4 / 3;
	}
		
	// WEAPON CONDITION
	if ( (*pInHand)[0]->data.objectStatus < 50 )
	{
		fAimModifier += gGameCTHConstants.AIM_GUN_CONDITION * (50 - (*pInHand)[0]->data.objectStatus);
	}

	// FIRING 1-HANDED WEAPONS
	if ( !(Item[ usInHand ].twohanded ) ) //JMich Todo: fix for UnderBarrel firing
	{
		if (pSoldier->inv[SECONDHANDPOS].exists() != false)
		{
			if ( pSoldier->IsValidSecondHandShot( ) )
			{
				// Penalty for shooting two pistols. Ambidextrous trait halves this.
				FLOAT fTempPenalty = (gGameCTHConstants.AIM_TWO_GUNS * fGunAimDifficulty) - fGunAimDifficulty;
				if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXTROUS_NT ))
				{
					fTempPenalty = fTempPenalty * (100 - gSkillTraitValues.ubAMPenaltyDoubleReduction) / 100;
				}
				else if (!gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, AMBIDEXT_OT ))
				{
					fTempPenalty = 0;
				}
				
				fGunAimDifficulty += fTempPenalty;
			}
			else
			{
				// Penalty for shooting a pistol with just one hand
				fGunAimDifficulty *= gGameCTHConstants.AIM_ONE_HANDED;
			}
		}
	}

	// HEAVY WEAPON in OLD trait system
	if (Item[usInHand].rocketlauncher && !(gGameOptions.fNewTraitSystem) ) 
	{
		// Penalty for shooting heavy weapons (launchers). Heavy Weapons skill halves this, once per skill level.
		FLOAT fTempPenalty = (gGameCTHConstants.AIM_HEAVY_WEAPON * fGunAimDifficulty) - fGunAimDifficulty;
			for (INT32 x=0; x < NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS_OT ); x++)
		{
			fTempPenalty /= 2;
		}
			fGunAimDifficulty += fTempPenalty;
	}

	// STANCE
	switch (stance)
	{
		case ANIM_STAND:
			fGunAimDifficulty *= gGameCTHConstants.AIM_STANDING_STANCE;
			break;

		case ANIM_CROUCH:
			fGunAimDifficulty *= gGameCTHConstants.AIM_CROUCHING_STANCE;
			break;

		case ANIM_PRONE:
			fGunAimDifficulty *= gGameCTHConstants.AIM_PRONE_STANCE;
			break;
	}

	// Percentage based handling modifier from the gun and its attachments
	FLOAT moda = (fGunAimDifficulty * GetPercentHandlingModifier( pInHand, stance )) / 100;
	FLOAT modb = (fGunAimDifficulty * GetPercentHandlingModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight )) / 100;
	fGunAimDifficulty += (gGameExternalOptions.ubProneModifierPercentage * moda + (100 - gGameExternalOptions.ubProneModifierPercentage) * modb)/100; 

	// Now apply Gun Difficulty to the Aim Modifier
	fAimModifier -= gGameCTHConstants.AIM_DRAW_COST * fGunAimDifficulty;

	// Percent modifier from the weapon and its attachments
	INT32 imoda = GetPercentAimModifier( pInHand, stance );
	INT32 imodb = GetPercentAimModifier( pInHand, gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
	fAimModifier += (FLOAT)(gGameExternalOptions.ubProneModifierPercentage * imoda + (100 - gGameExternalOptions.ubProneModifierPercentage) * imodb)/100; 
	
	return fAimModifier;
}

FLOAT CalcNewChanceToHitAimSpecialBonus(SOLDIERTYPE *pSoldier)
{
	FLOAT fAimModifier = 0;

	// GAME DIFFICULTY
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC ) && (pSoldier->bSide != gbPlayerNum) )
	{
		fAimModifier += gGameCTHConstants.AIM_DIFFICULTY[gGameOptions.ubDifficultyLevel];
	}
	
	return fAimModifier;
}

FLOAT CalcNewChanceToHitAimTargetBonus(SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget, INT32 sGridNo, INT32 iRange, UINT8 ubAimPos, BOOLEAN fCantSeeTarget)
{
	FLOAT fAimModifier = 0;
	INT32 iHeightDifference = 0;

	// SHOOTING AT A TARGET AT DIFFERENT HEIGHT?
	if ( pTarget == NULL )
	{
		// Shooting to roof.
		if ( pSoldier->bTargetLevel > pSoldier->pathing.bLevel )
		{
			iHeightDifference = 3 * pSoldier->bTargetLevel;
		}
	}
	else
	{
		// HEIGHT DIFFERENCE
		UINT32 uiShooterHeight = 0;
		UINT32 uiTargetHeight = 0;

		if ( pSoldier->pathing.bLevel > 0 )
		{
			uiShooterHeight += 3 * pSoldier->pathing.bLevel;
		}
		
		switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				uiShooterHeight += 2;
				break;
			case ANIM_CROUCH:
				uiShooterHeight += 1;
				break;
		}

		if (pTarget->pathing.bLevel > 0)
		{
			uiTargetHeight += 3 * pTarget->pathing.bLevel;
		}

		switch ( gAnimControl[ pTarget->usAnimState ].ubEndHeight )
		{
			case ANIM_STAND:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 2;
						break;
					case AIM_SHOT_TORSO:
					case AIM_SHOT_RANDOM:
					case AIM_SHOT_GLAND:
						uiTargetHeight += 1;
						break;
				}
				break;
			case ANIM_CROUCH:
				switch (ubAimPos)
				{
					case AIM_SHOT_HEAD:
						uiTargetHeight += 1;
						break;
				}
				break;
		}

		iHeightDifference = uiShooterHeight - uiTargetHeight;
		if (iHeightDifference < 0)
		{
			iHeightDifference *= -1;
		}
		else
		{
			iHeightDifference = 0;
		}
	}

	// Height difference is mitigated by range. A LONGER range reduces this penalty!
	if (iRange > 0 && iHeightDifference > 0)
	{
		FLOAT fTempPenalty = gGameCTHConstants.AIM_SHOOTING_UPWARDS * iHeightDifference;
		fTempPenalty /= iRange;
		fAimModifier += fTempPenalty;
	}

	// target is invisible to the shooter
	if(fCantSeeTarget)
	{
		fAimModifier += gGameCTHConstants.AIM_TARGET_INVISIBLE;
	}

	return fAimModifier;
}

FLOAT CalcNewChanceToHitAimTraitBonus(SOLDIERTYPE *pSoldier, FLOAT fAimCap, FLOAT fDifference, INT32 sGridNo, INT16 ubAimTime, FLOAT fScopeMagFactor, UINT32 uiBestScopeRange)
{
	FLOAT fAimChance = 0;
	FLOAT fSniperSkillBonus = 0;
	UINT16 usInHand = pSoldier->usAttackingWeapon;
	INT32 iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

	// SANDRO - added support to Throwing trait
	if ( fAimCap < gGameExternalOptions.ubMaximumCTH && Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
	{
		if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, THROWING_NT ) )
		{
			fAimChance += (FLOAT)((fDifference * gSkillTraitValues.ubTHBladesCtHBonusPerClick*3) / gGameExternalOptions.ubMaximumCTH);
		}
	}
	// Add bonuses from Sniper Skill. Applies only when using a scope at or above its "best" range.
	else if ( !pSoldier->IsValidAlternativeFireMode( ubAimTime, sGridNo ) )
	{
		if (fAimCap < gGameExternalOptions.ubMaximumCTH && fScopeMagFactor > 1.0 && iRange >= (INT32)uiBestScopeRange )
		{
			INT8	loop;
			if(gGameOptions.fNewTraitSystem)
			{
				if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
					loop = NUM_SKILL_TRAITS( pSoldier, GUNSLINGER_NT );
				else
					loop = NUM_SKILL_TRAITS( pSoldier, SNIPER_NT );
			}
			else
				loop = NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT );

			for (INT8 cnt = 0; cnt < loop; cnt++)
			{
				// For each level of the sniper skill, cover a defined distance between the current CTH Cap and 99 (the
				// maximum...
				// SANDRO - this results in giving us total bonus on fAimChance of lame 3-8%... compared to 10-20% bonus *PER AIM CLICK* in OCTH
				// ..as an artificial solution multiply the bonus by at least 2 to achieve some meaningful results
				if(gGameOptions.fNewTraitSystem)
				{
					if ( Weapon[usInHand].ubWeaponType == GUN_PISTOL || Weapon[usInHand].ubWeaponType == GUN_M_PISTOL )
						fSniperSkillBonus += (FLOAT)((fDifference * gSkillTraitValues.ubGSAimingBonusPerClick * 2) / gGameExternalOptions.ubMaximumCTH);
					else
						fSniperSkillBonus += (FLOAT)((fDifference * gSkillTraitValues.ubSNAimingBonusPerClick * 2) / gGameExternalOptions.ubMaximumCTH);
				}
				else
					fSniperSkillBonus += (FLOAT)((fDifference * gGameCTHConstants.AIM_SNIPER_SKILL * 2) / gGameExternalOptions.ubMaximumCTH);
				fDifference -= fSniperSkillBonus;
			}

			fAimChance += fSniperSkillBonus;
		}
	}
	
	return fAimChance;
}