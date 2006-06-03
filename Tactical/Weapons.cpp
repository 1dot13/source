#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "sys globals.h"
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
#endif

//rain
//#define WEAPON_RELIABILITY_REDUCTION_PER_RAIN_INTENSITY 0
extern INT8 gbCurrentRainIntensity;
//end rain


#define MINCHANCETOHIT          1
#define MAXCHANCETOHIT          99

// NB this is arbitrary, chances in DG ranged from 1 in 6 to 1 in 20
#define BASIC_DEPRECIATE_CHANCE	15

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

BOOLEAN UseGun( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo );
BOOLEAN UseBlade( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo );
BOOLEAN UseThrown( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo );
BOOLEAN UseLauncher( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo );

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack );

BOOLEAN gfNextShotKills = FALSE;
BOOLEAN gfReportHitChances = FALSE;

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
#define BLADE( impact, rof, deadl, range, av, sd )																																{ KNIFECLASS,		NOT_GUN,			0,					AP_READY_KNIFE,		rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 range, 200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define THROWINGBLADE( impact, rof, deadl, range, av, sd )																												{ KNIFECLASS,		NOT_GUN,			0,					AP_READY_KNIFE,		rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 range, 200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define PUNCHWEAPON( impact, rof, deadl, av, sd )																																	{ KNIFECLASS,		NOT_GUN,			0,					0,								rof, 0,				 0,					 0,			 (UINT8) (impact), deadl, 0,	 0,		 10,		200, av, 0,  sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define LAUNCHER( update, rt, rof, deadl, acc, range, av, hv, sd)																									{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 1,								 deadl, acc, 0,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define LAW( update, rt, rof, deadl, acc, range, av, hv, sd)																											{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 80,							 deadl, acc, 1,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define CANNON( update, rt, rof, deadl, acc, range, av, hv, sd)																										{ RIFLECLASS,		NOT_GUN,			NOAMMO,			rt,								rof, 0,				 0,					 update, 80,							 deadl, acc, 1,		 range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }
#define MONSTSPIT( impact, rof, deadl, clip, range, av, hv, sd)																										{ MONSTERCLASS, NOT_GUN,			AMMOMONST,	AP_READY_KNIFE,		rof, 0,				 0,					 250,		 (UINT8) (impact), deadl, 0,	 clip, range, 200, av, hv, sd, NO_WEAPON_SOUND, NO_WEAPON_SOUND, NO_WEAPON_SOUND }

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


INT8 gzBurstSndStrings[MAXITEMS*2][128];// =
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

	INT8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	WEAPONTYPE		curWeapon;
	WEAPONTYPE *	curWeaponList;
	UINT32			maxWeapons;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef weaponParseData;


static void XMLCALL
weaponStartElementHandle(void *userData, const char *name, const char **atts)
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
				strcmp(name, "MaxDistForMessyDeath") == 0 ||
				strcmp(name, "SilencedSound") == 0 || // Lesh: add new field (OR operand)
                strcmp(name, "BurstAniDelay") == 0))   // Lesh: add new field (field itself)

		{
			pData->curElement = WEAPON_ELEMENT_WEAPON_PROPERY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
weaponCharacterDataHandle(void *userData, const char *str, int len)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
weaponEndElementHandle(void *userData, const char *name)
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
		}
		else if(strcmp(name, "ubShotsPer4Turns") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubShotsPer4Turns = (UINT8) atol(pData->szCharData);
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
		else if(strcmp(name, "ubMagSize") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubMagSize = (UINT8) atol(pData->szCharData);
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

#ifdef JA2TESTVERSION
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\~Weapons out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			INT8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<WEAPON>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									Weapon[cnt].uiIndex);

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
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Weapon[cnt].ubMagSize);
			FilePrintf(hFile,"\t\t<usRange>%d</usRange>\r\n",									Weapon[cnt].usRange);
			FilePrintf(hFile,"\t\t<usReloadDelay>%d</usReloadDelay>\r\n",						Weapon[cnt].usReloadDelay);
			FilePrintf(hFile,"\t\t<ubAttackVolume>%d</ubAttackVolume>\r\n",						Weapon[cnt].ubAttackVolume);
			FilePrintf(hFile,"\t\t<ubHitVolume>%d</ubHitVolume>\r\n",							Weapon[cnt].ubHitVolume);
			FilePrintf(hFile,"\t\t<sSound>%d</sSound>\r\n",										Weapon[cnt].sSound);
			FilePrintf(hFile,"\t\t<sBurstSound>%d</sBurstSound>\r\n",							Weapon[cnt].sBurstSound);
			FilePrintf(hFile,"\t\t<sSilencedBurstSound>%d</sSilencedBurstSound>\r\n",							Weapon[cnt].sSilencedBurstSound);
			FilePrintf(hFile,"\t\t<sReloadSound>%d</sReloadSound>\r\n",							Weapon[cnt].sReloadSound);
			FilePrintf(hFile,"\t\t<sLocknLoadSound>%d</sLocknLoadSound>\r\n",					Weapon[cnt].sLocknLoadSound);
			FilePrintf(hFile,"\t\t<bBurstAP>%d</bBurstAP>\r\n",				Weapon[cnt].bBurstAP);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );
#endif

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
			INT8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

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
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
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



			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}

UINT16 GunRange( OBJECTTYPE * pObj )
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

		//if ( Weapon [ pObj->usItem ].ubWeaponType == GUN_SHOTGUN && pObj->ubGunAmmoType != AMMO_BUCKSHOT )
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


		// Snap: attachment status is factored into the range bonus calculation
		rng = Weapon[ pObj->usItem ].usRange + GetRangeBonus(pObj);

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
	INT8		bPlate;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_ARMOUR)
	{
		return( 0 );
	}
	iValue = Armour[ Item[pObj->usItem].ubClassIndex ].ubProtection;
	iValue = iValue * pObj->bStatus[0] / 100;

//	bPlate = FindAttachment( pObj, CERAMIC_PLATES );
	bPlate = FindFirstArmourAttachment( pObj );
	if ( bPlate != ITEM_NOT_FOUND )
	{
		INT32 iValue2;

		iValue2 = Armour[ Item[ pObj->usAttachItem[bPlate] ].ubClassIndex ].ubProtection;
		iValue2 = iValue2 * pObj->bAttachStatus[ bPlate ] / 100;

		iValue += iValue2;
	}
	return( max(iValue,1) );
}

INT32 ArmourPercent( SOLDIERTYPE * pSoldier )
{
	INT32 iVest, iHelmet, iLeg;

	if (pSoldier->inv[VESTPOS].usItem)
	{
		iVest = EffectiveArmour( &(pSoldier->inv[VESTPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iVest = 65 * iVest / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection );
	}
	else
	{
		iVest = 0;
	}

	if (pSoldier->inv[HELMETPOS].usItem)
	{
		iHelmet = EffectiveArmour( &(pSoldier->inv[HELMETPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iHelmet = 15 * iHelmet / Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection;
	}
	else
	{
		iHelmet = 0;
	}

	if (pSoldier->inv[LEGPOS].usItem)
	{
		iLeg = EffectiveArmour( &(pSoldier->inv[LEGPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iLeg = 25 * iLeg / Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection;
	}
	else
	{
		iLeg = 0;
	}
	return( (iHelmet + iVest + iLeg) );
}

INT32 ExplosiveEffectiveArmour( OBJECTTYPE * pObj )
{
	INT32		iValue;
	INT8		bPlate;

	if (pObj == NULL || Item[pObj->usItem].usItemClass != IC_ARMOUR)
	{
		return( 0 );
	}
	iValue = Armour[ Item[pObj->usItem].ubClassIndex ].ubProtection;
	iValue = iValue * pObj->bStatus[0] / 100;
	if ( Item[pObj->usItem].flakjacket )
	{
		// increase value for flak jackets!
		iValue *= 3;
	}

	bPlate = FindFirstArmourAttachment( pObj );
	if ( bPlate != ITEM_NOT_FOUND )
	{
		INT32 iValue2;

		iValue2 = Armour[ Item[ pObj->usAttachItem[bPlate] ].ubClassIndex ].ubProtection;
		iValue2 = iValue2 * pObj->bAttachStatus[ bPlate ] / 100;

		iValue += iValue2;
	}
	return( max(iValue,1) );
}

INT8 ArmourVersusExplosivesPercent( SOLDIERTYPE * pSoldier )
{
	// returns the % damage reduction from grenades
	INT32 iVest, iHelmet, iLeg;

	if (pSoldier->inv[VESTPOS].usItem)
	{
		iVest = ExplosiveEffectiveArmour( &(pSoldier->inv[VESTPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iVest = __min( 65, 65 * iVest / ( Armour[ Item[ SPECTRA_VEST_18 ].ubClassIndex ].ubProtection + Armour[ Item[ CERAMIC_PLATES ].ubClassIndex ].ubProtection) );
	}
	else
	{
		iVest = 0;
	}

	if (pSoldier->inv[HELMETPOS].usItem)
	{
		iHelmet = ExplosiveEffectiveArmour( &(pSoldier->inv[HELMETPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iHelmet = __min( 15, 15 * iHelmet / Armour[ Item[ SPECTRA_HELMET_18 ].ubClassIndex ].ubProtection );
	}
	else
	{
		iHelmet = 0;
	}

	if (pSoldier->inv[LEGPOS].usItem)
	{
		iLeg = ExplosiveEffectiveArmour( &(pSoldier->inv[LEGPOS]) );
		// convert to % of best; ignoring bug-treated stuff
		iLeg = __min( 25, 25 * iLeg / Armour[ Item[ SPECTRA_LEGGINGS_18 ].ubClassIndex ].ubProtection );
	}
	else
	{
		iLeg = 0;
	}
	return( (INT8) (iHelmet + iVest + iLeg) );
}

void AdjustImpactByHitLocation( INT32 iImpact, UINT8 ubHitLocation, INT32 * piNewImpact, INT32 * piImpactForCrits )
{
	switch( ubHitLocation )
	{
		case AIM_SHOT_HEAD:
			// 1.5x damage from successful hits to the head!
			*piImpactForCrits = HEAD_DAMAGE_ADJUSTMENT( iImpact );
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
  OBJECTTYPE *	pObj;
  INT32		iChance, iResult;
  
  // should jams apply to enemies?
	if (pSoldier->uiStatusFlags & SOLDIER_PC)
	{
		if ( Item[pSoldier->usAttackingWeapon].usItemClass == IC_GUN && !EXPLOSIVE_GUN( pSoldier->usAttackingWeapon ) )
		{
			pObj = &(pSoldier->inv[pSoldier->ubAttackingHand]);
  			if (pObj->bGunAmmoStatus > 0)
			{
				// gun might jam, figure out the chance
				//iChance = (80 - pObj->bGunStatus);
				
				//rain
				iChance = (80 - 90 - pObj->bGunStatus) + gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity * gbCurrentRainIntensity;
				//end rain


				// CJC: removed reliability from formula...
				
				// jams can happen to unreliable guns "earlier" than normal or reliable ones.
				//iChance = iChance - Item[pObj->usItem].bReliability * 2;

				// decrease the chance of a jam by 20% per point of reliability; 
				// increased by 20% per negative point...
				//iChance = iChance * (10 - Item[pObj->usItem].bReliability * 2) / 10;
				
				//rain
//				iChance = iChance * (10 - Item[pObj->usItem].bReliability * 2) / 10; // Madd: took it back out
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

#ifdef TESTGUNJAM
				if ( 1 )
#else
				if ((INT32) PreRandom( 100 ) < iChance || gfNextFireJam )
#endif
				{
					gfNextFireJam = FALSE;

					// jam! negate the gun ammo status.
					pObj->bGunAmmoStatus *= -1;

					// Deduct AMMO!
					DeductAmmo( pSoldier, pSoldier->ubAttackingHand );

					TacticalCharacterDialogue( pSoldier, QUOTE_JAMMED_GUN );
					return( TRUE );
				}
			}
			else if (pObj->bGunAmmoStatus < 0)
			{
				// try to unjam gun
				iResult = SkillCheck( pSoldier, UNJAM_GUN_CHECK, (INT8) ((Item[pObj->usItem].bReliability + Item[pObj->usGunAmmoItem].bReliability)* 4) );
				if (iResult > 0)
				{
					// yay! unjammed the gun
					pObj->bGunAmmoStatus *= -1;

					// MECHANICAL/DEXTERITY GAIN: Unjammed a gun
					StatChange( pSoldier, MECHANAMT, 5, FALSE );
					StatChange( pSoldier, DEXTAMT, 5, FALSE );

					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

					// We unjammed gun, return appropriate value!
					return( 255 );
				}
				else
				{
					return( TRUE );
				}
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
		if ( Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].NoSemiAuto ) // Madd: no autofire w/second hand (yet)
		{
			return( FALSE );
		}

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


BOOLEAN FireWeapon( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo )
{
	// ignore passed in target gridno for now

	// If realtime and we are reloading - do not fire until counter is done!
	if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) && !pSoldier->bDoBurst )
	{
		if ( pSoldier->fReloading )
		{
			return( FALSE );
		}
	}

	// if target gridno is the same as ours, do not fire!
	if (sTargetGridNo == pSoldier->sGridNo)
	{
		// FREE UP NPC!
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - attack on own gridno!") );
		FreeUpAttacker( (UINT8) pSoldier->ubID );
		return( FALSE );
	}

	// SET ATTACKER TO NOBODY, WILL GET SET EVENTUALLY
	pSoldier->ubOppNum = NOBODY ;

	UINT16 usItemClass = Item[ pSoldier->usAttackingWeapon ].usItemClass;
	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
		usItemClass = IC_LAUNCHER;

	switch( usItemClass )
	{
		case IC_THROWING_KNIFE:
		case IC_GUN:

			if ( pSoldier->bWeaponMode == WM_ATTACHED_GL )
				UseLauncher ( pSoldier, sTargetGridNo );
			else
			{
				// ATE: PAtch up - bookkeeping for spreading done out of whak
				if ( pSoldier->fDoSpread && !pSoldier->bDoBurst )
				{
					pSoldier->fDoSpread = FALSE;
				}

				if ( pSoldier->fDoSpread >= MAX_BURST_SPREAD_TARGETS )
				{
					//If we have more than MAX_BURST_SPREAD_TARGETS bullets, loop over and start again from the first target location
					pSoldier->fDoSpread = 1;
				}


				if ( pSoldier->fDoSpread )
				{
					if ( pSoldier->sSpreadLocations[ pSoldier->fDoSpread - 1 ] != 0 )
					{
						UseGun( pSoldier, pSoldier->sSpreadLocations[ pSoldier->fDoSpread - 1 ] );
					}
					else
					{
						UseGun( pSoldier, sTargetGridNo );
					}
					pSoldier->fDoSpread++;
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
			if ( Item[pSoldier->usAttackingWeapon].rocketlauncher )
				UseGun( pSoldier, sTargetGridNo );
			else
				// ATE: PAtch up - bookkeeping for spreading done out of whak
				if ( pSoldier->fDoSpread && !pSoldier->bDoBurst )
				{
					pSoldier->fDoSpread = FALSE;
				}

				if ( pSoldier->fDoSpread >= MAX_BURST_SPREAD_TARGETS )
				{
					//If we have more than MAX_BURST_SPREAD_TARGETS bullets, loop over and start again from the first target location
					pSoldier->fDoSpread = 1;
				}


				if ( pSoldier->fDoSpread )
				{
					if ( pSoldier->sSpreadLocations[ pSoldier->fDoSpread - 1 ] != 0 )
					{
						UseLauncher( pSoldier, pSoldier->sSpreadLocations[ pSoldier->fDoSpread - 1 ] );
					}
					else
					{
						UseLauncher( pSoldier, sTargetGridNo );
					}
					pSoldier->fDoSpread++;
				}
				else
				{
					UseLauncher( pSoldier, sTargetGridNo );
				}
			
			break;

		default:
			// attempt to throw
			UseThrown( pSoldier, sTargetGridNo );
			break;
	}
	return( TRUE );
}


void GetTargetWorldPositions( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, FLOAT *pdXPos, FLOAT *pdYPos, FLOAT *pdZPos )
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


BOOLEAN UseGun( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo )
{
	UINT32							uiHitChance, uiDiceRoll;
	INT16								sXMapPos, sYMapPos;	
	INT16								sAPCost;
	FLOAT								dTargetX;
	FLOAT								dTargetY;
	FLOAT								dTargetZ;
	UINT16							usItemNum;
	BOOLEAN							fBuckshot;
	UINT8								ubVolume;
	INT8								zBurstString[50];
	UINT8								ubDirection;
	INT16								sNewGridNo;
	UINT8								ubMerc;
	BOOLEAN							fGonnaHit = FALSE;
	UINT16							usExpGain = 0;
	UINT32							uiDepreciateTest;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("UseGun") );
	// Deduct points!
 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->bAimTime );

	usItemNum = pSoldier->usAttackingWeapon;

	if ( pSoldier->bDoBurst )
	{
		// ONly deduct points once
		if ( pSoldier->bDoBurst == 1 )
		{
			if ( Weapon[ usItemNum ].sBurstSound != NO_WEAPON_SOUND )
			{
				// IF we are silenced?
				UINT16 noisefactor = GetPercentNoiseVolume( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
				if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usItemNum ].ubAttackVolume <= 10 )
				{
					// Pick sound file baed on how many bullets we are going to fire...
					sprintf( (char *)zBurstString, (const char*)gzBurstSndStrings[ Weapon[ usItemNum ].sSilencedBurstSound ], pSoldier->bBulletsLeft );

					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile( (STR8) zBurstString, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );			
				}
				else
				{
					// Pick sound file baed on how many bullets we are going to fire...
                    // Lesh: changed next line
					sprintf( (char *)zBurstString, (const char*)gzBurstSndStrings[ Weapon[ usItemNum ].sBurstSound ], pSoldier->bBulletsLeft );

					INT8 volume = HIGHVOLUME;
					if ( noisefactor < 100 ) volume = (volume * noisefactor) / 100;
					// Try playing sound...
					pSoldier->iBurstSoundID = PlayJA2SampleFromFile( (STR8) zBurstString, RATE_11025, SoundVolume( (INT8) volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );			
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

			DeductPoints( pSoldier, sAPCost, 0 );
		}

	}
	else
	{
		// ONLY DEDUCT FOR THE FIRST HAND when doing two-pistol attacks
		if ( IsValidSecondHandShot( pSoldier ) && pSoldier->inv[ HANDPOS ].bGunStatus >= USABLE && pSoldier->inv[HANDPOS].bGunAmmoStatus > 0 )
		{
			// only deduct APs when the main gun fires
			if ( pSoldier->ubAttackingHand == HANDPOS )
			{
				DeductPoints( pSoldier, sAPCost, 0 );
			}				
		}
		else
		{
			DeductPoints( pSoldier, sAPCost, 0 );
		}

		//PLAY SOUND
		// ( For throwing knife.. it's earlier in the animation
		if ( Weapon[ usItemNum ].sSound != NO_WEAPON_SOUND && Item[ usItemNum ].usItemClass != IC_THROWING_KNIFE )
		{
			// Switch on silencer...
			UINT16 noisefactor = GetPercentNoiseVolume( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
			if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usItemNum ].ubAttackVolume <= 10 )
			{
				INT32 uiSound;

				uiSound = Weapon [ usItemNum ].silencedSound ;
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
				PlayJA2Sample( Weapon[ usItemNum ].sSound, RATE_11025, SoundVolume( volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );			
			}
		}
	}


	// CALC CHANCE TO HIT
	if ( Item[ usItemNum ].usItemClass == IC_THROWING_KNIFE )
	{
	  uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->bAimTime, pSoldier->bAimShotLocation );
	}
	else
	{
	  uiHitChance = CalcChanceToHitGun( pSoldier, sTargetGridNo, pSoldier->bAimTime, pSoldier->bAimShotLocation );
	}

	//DIGICRAB: Barrel extender wear code
	// Relocated from CalcChanceToHitGun
	if ( Item[ usItemNum ].usItemClass == IC_GUN )
	{
		INT8 bAttachPos;
		OBJECTTYPE * pInHand;
		pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);
		UINT32 item;
		item = FindRangeBonusAttachment(pInHand);
		bAttachPos = FindAttachment( pInHand, item );
		if ( bAttachPos != ITEM_NOT_FOUND )
		{
			// reduce status and see if it falls off
			pInHand->bAttachStatus[ bAttachPos ] -= (INT8) Random( 2 );

			if ( pInHand->bAttachStatus[ bAttachPos ] - Random( 35 ) - Random( 35 ) < USABLE )
			{
				// barrel extender falls off!
				OBJECTTYPE Temp;

				// since barrel extenders are not removable we cannot call RemoveAttachment here
				// and must create the item by hand
				CreateItem( item, pInHand->bAttachStatus[ bAttachPos ], &Temp );
				pInHand->usAttachItem[ bAttachPos ] = NOTHING;
				pInHand->bAttachStatus[ bAttachPos ] = 0;

				// drop it to ground
				AddItemToPool( pSoldier->sGridNo, &Temp, 1, pSoldier->bLevel, 0, -1 );

				// big penalty to hit
				if(uiHitChance < 30)
					uiHitChance = MINCHANCETOHIT;
				else
					uiHitChance -= 30;

				// curse!
				if ( pSoldier->bTeam == OUR_TEAM )
				{
					DoMercBattleSound( pSoldier, BATTLE_SOUND_CURSE1 );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 46 ], pSoldier->name );
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

	fGonnaHit = uiDiceRoll <= uiHitChance;

	// GET TARGET XY VALUES
  ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

	// ATE; Moved a whole blotch if logic code for finding target positions to a function
	// so other places can use it
	GetTargetWorldPositions( pSoldier, sTargetGridNo, &dTargetX, &dTargetY, &dTargetZ );

	// Some things we don't do for knives...
	if ( Item[ usItemNum ].usItemClass != IC_THROWING_KNIFE )
	{
		// If realtime - set counter to freeup from attacking once done
		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{

			// Set delay based on stats, weapon type, etc
			pSoldier->sReloadDelay	= (INT16)( Weapon[ usItemNum ].usReloadDelay + MANDATORY_WEAPON_DELAY );

			// If a bad guy, double the delay!
			if ( (pSoldier->uiStatusFlags & SOLDIER_ENEMY ) )
			{
				pSoldier->sReloadDelay	= ( pSoldier->sReloadDelay * 2 );
			}

			
			// slow down demo mode!
			if ( gTacticalStatus.uiFlags & DEMOMODE )
			{
				pSoldier->sReloadDelay *= 2;
			}

			//pSoldier->fReloading		= TRUE;
			//RESETTIMECOUNTER( pSoldier->ReloadCounter, pSoldier->sReloadDelay );
		}

		// Deduct AMMO!
		DeductAmmo( pSoldier, pSoldier->ubAttackingHand );

		// ATE: Check if we should say quote...
		if ( pSoldier->inv[ pSoldier->ubAttackingHand ].ubGunShotsLeft == 0 && !Item[pSoldier->usAttackingWeapon].rocketlauncher ) 
		{
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->fSayAmmoQuotePending = TRUE;
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

				if ( pSoldier->bAimTime && !pSoldier->bDoBurst )
				{
					// gain extra exp for aiming, up to the amount from 
					// the difficulty of the shot
					usExpGain += __min( pSoldier->bAimTime, usExpGain );
				}

				// base pts extra for hitting
				usExpGain	+= 3;
			}

			// add base pts for taking a shot, whether it hits or misses
			usExpGain += 3;

			if ( IsValidSecondHandShot( pSoldier ) && pSoldier->inv[ HANDPOS ].bGunStatus >= USABLE && pSoldier->inv[HANDPOS].bGunAmmoStatus > 0 )
			{
				// reduce exp gain for two pistol shooting since both shots give xp
				usExpGain = (usExpGain * 2) / 3;
			}

			if ( MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == COW || MercPtrs[ pSoldier->ubTargetID ]->ubBodyType == CROW )
			{
				usExpGain /= 2;
			}
			else if ( MercPtrs[ pSoldier->ubTargetID ]->uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
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
			if ( IsFlashSuppressor(&pSoldier->inv[ pSoldier->ubAttackingHand ], pSoldier ) )
				pSoldier->fMuzzleFlash = FALSE;
			else
				pSoldier->fMuzzleFlash = TRUE;

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("UseGun: Muzzle flash = %d",pSoldier->fMuzzleFlash));

			if ( AmmoTypes[pSoldier->inv[ pSoldier->ubAttackingHand ].ubGunAmmoType].numberOfBullets > 1 )
				fBuckshot = TRUE;

			//switch ( pSoldier->inv[ pSoldier->ubAttackingHand ].ubGunAmmoType )
			//{
			//	case AMMO_BUCKSHOT:
			//		fBuckshot = TRUE;
			//		break;
			//	case AMMO_SLEEP_DART:
			//		pSoldier->fMuzzleFlash = FALSE;
			//		break;
			//	default:
			//		break;
			//}
		}
	}
	else	//  throwing knife
	{
		fBuckshot = FALSE;
		pSoldier->fMuzzleFlash = FALSE;

		// Deduct knife from inv! (not here, later?)

		// Improve for using a throwing knife....
		if (PTR_OURTEAM && pSoldier->ubTargetID != NOBODY)
		{
			if ( fGonnaHit )
			{
				// grant extra exp for hitting a difficult target
				usExpGain += (UINT8) (100 - uiHitChance) / 10;

				if (pSoldier->bAimTime)
				{
					// gain extra exp for aiming, up to the amount from 
					// the difficulty of the throw
					usExpGain += ( 2 * __min( pSoldier->bAimTime, usExpGain ) );
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
			else if ( MercPtrs[ pSoldier->ubTargetID ]->uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
			{
				// no exp from shooting a vehicle that you can't damage and can't move! 
				usExpGain = 0;
			}

			// MARKSMANSHIP/DEXTERITY GAIN: throwing knife attack
			StatChange( pSoldier, MARKAMT, ( UINT16 )( usExpGain / 2 ), ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );				
			StatChange( pSoldier, DEXTAMT, ( UINT16 )( usExpGain / 2 ), ( UINT8 )( fGonnaHit ? FALSE : FROM_FAILURE ) );				
		}
	}

	if ( Item[usItemNum].rocketlauncher  )
	{
    if ( WillExplosiveWeaponFail( pSoldier, &( pSoldier->inv[ HANDPOS ] ) ) )
    {
		if ( Item[usItemNum].singleshotrocketlauncher  )
		{
			CreateItem( Item[usItemNum].discardedlauncheritem , pSoldier->inv[ HANDPOS ].bStatus[ 0 ],&(pSoldier->inv[ HANDPOS ] ) );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			IgniteExplosion( pSoldier->ubID, (INT16)CenterX( pSoldier->sGridNo ), (INT16)CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, C1, pSoldier->bLevel );
		}
		else
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d",pSoldier->inv[HANDPOS].usItem , pSoldier->inv[HANDPOS].usGunAmmoItem ) );
			
			IgniteExplosion( pSoldier->ubID, (INT16)CenterX( pSoldier->sGridNo ), (INT16)CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, pSoldier->inv[pSoldier->ubAttackingHand ].usGunAmmoItem, pSoldier->bLevel );
			pSoldier->inv[pSoldier->ubAttackingHand ].usGunAmmoItem = NONE;
		}
      // Reduce again for attack end 'cause it has been incremented for a normal attack
      // 
		  DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK, Now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
		  ReduceAttackBusyCount( pSoldier->ubID, FALSE );

      return( FALSE );
    }
  }


	FireBulletGivenTarget( pSoldier, dTargetX, dTargetY, dTargetZ, pSoldier->usAttackingWeapon, (UINT16) (uiHitChance - uiDiceRoll), fBuckshot, FALSE );

	ubVolume = Weapon[ pSoldier->usAttackingWeapon ].ubAttackVolume;

	if ( Item[ usItemNum ].usItemClass == IC_THROWING_KNIFE )
	{
		// Here, remove the knife...	or (for now) rocket launcher	
		RemoveObjs( &(pSoldier->inv[ HANDPOS ] ), 1 );
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
	else if ( Item[usItemNum].rocketlauncher ) 
	{
		if ( Item[usItemNum].singleshotrocketlauncher )
		{
			CreateItem( Item[usItemNum].discardedlauncheritem, pSoldier->inv[ HANDPOS ].bStatus[ 0 ], &(pSoldier->inv[ HANDPOS ] ) );
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
		}

		// Direction to center of explosion
		ubDirection = gOppositeDirection[ pSoldier->bDirection ];
		sNewGridNo  = NewGridNo( (UINT16)pSoldier->sGridNo, (UINT16)(1 * DirectionInc( ubDirection ) ) );

		// Check if a person exists here and is not prone....
		ubMerc = WhoIsThere2( sNewGridNo, pSoldier->bLevel );

		if ( ubMerc != NOBODY )
		{
			if ( gAnimControl[ MercPtrs[ ubMerc ]->usAnimState ].ubHeight != ANIM_PRONE )
			{
				// Increment attack counter...
				gTacticalStatus.ubAttackBusyCount++;
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Exaust from LAW", gTacticalStatus.ubAttackBusyCount ) );

				EVENT_SoldierGotHit( MercPtrs[ ubMerc ], MINI_GRENADE, 10, 200, pSoldier->bDirection, 0, pSoldier->ubID, 0, ANIM_CROUCH, 0, sNewGridNo );
			}
		}
	}
	else
	{
		// Snap: get cumulative noise reduction from the weapon and its attachments
		UINT16 noisefactor = GetPercentNoiseVolume( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
		if ( ubVolume * noisefactor > 25000 )
		{ // Snap: hack this to prevent overflow (damn miserly programmers!)
			ubVolume = 250;
		}
		else
		{
			ubVolume = __max( 1, ( ubVolume * GetPercentNoiseVolume( &pSoldier->inv[ pSoldier->ubAttackingHand ] ) ) / 100 );
		}
	}

	MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_GUNFIRE );

	if ( pSoldier->bDoBurst )
	{
		// done, if bursting, increment
		pSoldier->bDoBurst++;
	}

	// CJC: since jamming is no longer affected by reliability, increase chance of status going down for really unreliabile guns
 	INT16 ammoReliability = 0; // Madd: ammo reliability affects gun
	if ( Item[usItemNum].usItemClass == IC_GUN )
		ammoReliability = Item[pSoldier->inv[pSoldier->ubAttackingHand].usItem].bReliability;

	uiDepreciateTest = BASIC_DEPRECIATE_CHANCE + 3 * (Item[ usItemNum ].bReliability + ammoReliability);

	if ( !PreRandom( uiDepreciateTest ) && ( pSoldier->inv[ pSoldier->ubAttackingHand ].bStatus[0] > 1) )
	{
		pSoldier->inv[ pSoldier->ubAttackingHand ].bStatus[ 0 ]--;		
	}

	// reduce monster smell (gunpowder smell)
	if ( pSoldier->bMonsterSmell > 0 && Random( 2 ) == 0 )
	{
		pSoldier->bMonsterSmell--;
	}


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("UseGun: done"));
	return( TRUE );
}

BOOLEAN UseBlade( SOLDIERTYPE *pSoldier , INT16 sTargetGridNo )
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
 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->bAimTime );

	DeductPoints( pSoldier, sAPCost, 0 );
	
	// GET TARGET XY VALUES
	ConvertGridNoToCenterCellXY( sTargetGridNo, &sXMapPos, &sYMapPos );

	// See if a guy is here!
	pTargetSoldier = SimpleFindSoldier( sTargetGridNo, pSoldier->bTargetLevel );
	if ( pTargetSoldier )
	{	
		// set target as noticed attack
		pSoldier->uiStatusFlags |= SOLDIER_ATTACK_NOTICED;
		pTargetSoldier->fIntendedTarget = TRUE;

		// SAVE OPP ID
		pSoldier->ubOppNum = pTargetSoldier->ubID;

		// CHECK IF BUDDY KNOWS ABOUT US
		if ( pTargetSoldier->bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN || pTargetSoldier->bLife < OKLIFE || pTargetSoldier->bCollapsed )
		{
			iHitChance = 100;
			fSurpriseAttack = TRUE;
		}
		else
		{
			iHitChance = CalcChanceToStab( pSoldier, pTargetSoldier, pSoldier->bAimTime );
			fSurpriseAttack = FALSE;
		}

		// ROLL DICE
		iDiceRoll = (INT32) PreRandom( 100 );
		//sprintf( gDebugStr, "Hit Chance: %d %d", (int)uiHitChance, uiDiceRoll );


		if ( iDiceRoll <= iHitChance )
		{
			fGonnaHit = TRUE;

			// CALCULATE DAMAGE!
			// attack HITS, calculate damage (base damage is 1-maximum knife sImpact)
			iImpact = HTHImpact( pSoldier, pTargetSoldier, (iHitChance - iDiceRoll), TRUE );

			// modify this by the knife's condition (if it's dull, not much good)
			iImpact = ( iImpact * WEAPON_STATUS_MOD(pSoldier->inv[pSoldier->ubAttackingHand].bStatus[0]) ) / 100;

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

			if ( pSoldier->inv[ pSoldier->ubAttackingHand ].bStatus[ 0 ] > USABLE )
			{
				bMaxDrop = (iImpact / 20);

				// the duller they get, the slower they get any worse...
				bMaxDrop = __min( bMaxDrop, pSoldier->inv[ pSoldier->ubAttackingHand ].bStatus[ 0 ] / 10 );

				// as long as its still > USABLE, it drops another point 1/2 the time
				bMaxDrop = __max( bMaxDrop, 2 );

				pSoldier->inv[ pSoldier->ubAttackingHand ].bStatus[ 0 ] -= (INT8) Random( bMaxDrop );     // 0 to (maxDrop - 1)
			}

			// Send event for getting hit
			memset( &(SWeaponHit), 0, sizeof( SWeaponHit ) );
			SWeaponHit.usSoldierID			= pTargetSoldier->ubID;
			SWeaponHit.uiUniqueId       = pTargetSoldier->uiUniqueSoldierIdValue;
			SWeaponHit.usWeaponIndex		= pSoldier->usAttackingWeapon;
			SWeaponHit.sDamage					= (INT16) iImpact;
			SWeaponHit.usDirection			= GetDirectionFromGridNo( pSoldier->sGridNo, pTargetSoldier );
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
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in knife attack") );
			FreeUpAttacker( (UINT8) pSoldier->ubID );
		}
	
		if ( PTR_OURTEAM && pSoldier->ubTargetID != NOBODY)
		{
			if ( fGonnaHit )
			{
				// grant extra exp for hitting a difficult target
				usExpGain += (UINT8) (100 - iHitChance) / 10;

				if (pSoldier->bAimTime)
				{
					// gain extra exp for aiming, up to the amount from 
					// the difficulty of the attack
					usExpGain += ( 2 * __min( pSoldier->bAimTime, usExpGain ) );
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
			else if ( MercPtrs[ pSoldier->ubTargetID ]->uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( MercPtrs[ pSoldier->ubTargetID ] ) || TANK( MercPtrs[ pSoldier->ubTargetID ] ) )
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
	else
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in knife attack") );
		FreeUpAttacker( (UINT8) pSoldier->ubID );
	}

	// possibly reduce monster smell
	if ( pSoldier->bMonsterSmell > 0 && Random( 5 ) == 0 )
	{
		pSoldier->bMonsterSmell--;
	}


	return( TRUE );
}


BOOLEAN UseHandToHand( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, BOOLEAN fStealing )
{
	SOLDIERTYPE				*	pTargetSoldier;
	INT32								iHitChance, iDiceRoll;
	INT16								sXMapPos, sYMapPos;	
	INT16								sAPCost;
	EV_S_WEAPONHIT			SWeaponHit;
	INT32								iImpact;
	UINT16							usOldItem;
	UINT8								ubExpGain;
	UINT8					ubIndexRet;
	BOOLEAN					fFailure;
	// Deduct points!
	// August 13 2002: unless stealing - APs already deducted elsewhere

//	if (!fStealing)
	{
 		sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->bAimTime );

		DeductPoints( pSoldier, sAPCost, 0 );
	}
	
	// See if a guy is here!
	pTargetSoldier = SimpleFindSoldier( sTargetGridNo, pSoldier->bTargetLevel );
	if ( pTargetSoldier )
	{	
		// set target as noticed attack
		pSoldier->uiStatusFlags |= SOLDIER_ATTACK_NOTICED;
		pTargetSoldier->fIntendedTarget = TRUE;

		// SAVE OPP ID
		pSoldier->ubOppNum = pTargetSoldier->ubID;

		if (fStealing)
		{
			if ( AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) || CREATURE_OR_BLOODCAT( pTargetSoldier ) || TANK( pTargetSoldier ) )
			{
				iHitChance = 0;
			}
			else if ( pTargetSoldier->bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN )
			{
				// give bonus for surprise, but not so much as struggle would still occur
				iHitChance = CalcChanceToSteal( pSoldier, pTargetSoldier, pSoldier->bAimTime ) + 20;
			}
			else if ( pTargetSoldier->bLife < OKLIFE || pTargetSoldier->bCollapsed )
			{
				iHitChance = 100;
			}
			else
			{
				iHitChance = CalcChanceToSteal( pSoldier, pTargetSoldier, pSoldier->bAimTime );
			}
		}
		else
		{
			if ( pTargetSoldier->bOppList[ pSoldier->ubID ] == NOT_HEARD_OR_SEEN || pTargetSoldier->bLife < OKLIFE || pTargetSoldier->bCollapsed )
			{
				iHitChance = 100;
			}
			else
			{
				iHitChance = CalcChanceToPunch( pSoldier, pTargetSoldier, pSoldier->bAimTime );
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

if (fStealing )
		{
			fFailure=FALSE;
			if ( iDiceRoll <= iHitChance )
			{
				if ( pSoldier->bTeam == gbPlayerNum && pTargetSoldier->bTeam != gbPlayerNum && !(pTargetSoldier->uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( pTargetSoldier ) && !TANK( pTargetSoldier ) )
				{
					// made a steal; give experience
					StatChange( pSoldier, STRAMT, 8, FALSE );
					StatChange( pSoldier, DEXTAMT, 3, FALSE );
					StatChange( pSoldier, AGILAMT, 3, FALSE );
				}
				if (( iDiceRoll <= iHitChance * 2 / 3) || (pTargetSoldier->bCollapsed))
				{
					if (StealItems(pSoldier, pTargetSoldier,&ubIndexRet) == 1)
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_STOLE_SOMETHING ], pSoldier->name, ShortItemNames[ pTargetSoldier->inv[ubIndexRet].usItem ] );
						if (!AutoPlaceObject( pSoldier, &(pTargetSoldier->inv[ubIndexRet]), TRUE ))
							AddItemToPool( pSoldier->sGridNo, &(pTargetSoldier->inv[HANDPOS]), 1, pSoldier->bLevel, 0, -1 );
						DeleteObj( &(pTargetSoldier->inv[ubIndexRet]) );
					}
				}
				else if ( pTargetSoldier->inv[HANDPOS].usItem != NOTHING )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_STOLE_SOMETHING ], pSoldier->name, ShortItemNames[ pTargetSoldier->inv[HANDPOS].usItem ] );
					usOldItem = pTargetSoldier->inv[HANDPOS].usItem;
					if ( pSoldier->bTeam == gbPlayerNum )
					{
						DoMercBattleSound( pSoldier, BATTLE_SOUND_CURSE1 );
					}

					// Item dropped somewhere... roll based on the same chance to determine where!
					iDiceRoll = (INT32) PreRandom( 100 );
					if (iDiceRoll < iHitChance)
					{
						// Drop item in the our tile
						AddItemToPool( pSoldier->sGridNo, &(pTargetSoldier->inv[HANDPOS]), 1, pSoldier->bLevel, 0, -1 );
					}
					else
					{
						// Drop item in the target's tile
						AddItemToPool( pTargetSoldier->sGridNo, &(pTargetSoldier->inv[HANDPOS]), 1, pSoldier->bLevel, 0, -1 );
					}
					DeleteObj( &(pTargetSoldier->inv[HANDPOS]) );
					// Reload buddy's animation...
					ReLoadSoldierAnimationDueToHandItemChange( pTargetSoldier, usOldItem, NOTHING );
				}
				else
				{
					fFailure=TRUE;
				}
				// Reload buddy's animation...
				//ReLoadSoldierAnimationDueToHandItemChange( pTargetSoldier, usOldItem, NOTHING );
			}
			else
			{
				fFailure=TRUE;
			}
			if (fFailure)
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, 
					Message[ STR_FAILED_TO_STEAL_SOMETHING ], 
					pSoldier->name, ShortItemNames[ pTargetSoldier->inv[HANDPOS].usItem ] );
				if ( pSoldier->bTeam == gbPlayerNum )
				{
					DoMercBattleSound( pSoldier, BATTLE_SOUND_CURSE1 );
				}

				if ( iHitChance > 0 && pSoldier->bTeam == gbPlayerNum && pTargetSoldier->bTeam != gbPlayerNum && !(pTargetSoldier->uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( pTargetSoldier ) && !TANK( pTargetSoldier ) )
				{	
					// failed a steal; give some experience
					StatChange( pSoldier, STRAMT, 4, FROM_FAILURE );
					StatChange( pSoldier, DEXTAMT, 1, FROM_FAILURE );
					StatChange( pSoldier, AGILAMT, 1, FROM_FAILURE );
				}

			}

			#ifdef JA2BETAVERSION
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - steal") );
			#endif
			FreeUpAttacker( (UINT8) pSoldier->ubID );

		}
		else
		{

			// ATE/CC: if doing ninja spin kick (only), automatically make it a hit
			if ( pSoldier->usAnimState == NINJA_SPINKICK)
			{
				// Let him to succeed by a random amount
				iDiceRoll = PreRandom( iHitChance );
			}

			if ( pSoldier->bTeam == gbPlayerNum && pTargetSoldier->bTeam != gbPlayerNum )
			{	
				// made an HTH attack; give experience
				if ( iDiceRoll <= iHitChance )
				{
					ubExpGain = 8;

					if ( pTargetSoldier->uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) )
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
				else
				{
					ubExpGain = 4;

					if ( pTargetSoldier->uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pTargetSoldier ) || TANK( pTargetSoldier ) )
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
					StatChange( pTargetSoldier, DEXTAMT, 8, FALSE );
					StatChange( pTargetSoldier, STRAMT, 3, FALSE );
					StatChange( pTargetSoldier, AGILAMT, 3, FALSE );
				}
			}

			if ( iDiceRoll <= iHitChance || AreInMeanwhile( ) )
			{
				// CALCULATE DAMAGE!
				iImpact = HTHImpact( pSoldier, pTargetSoldier, (iHitChance - iDiceRoll), FALSE );

				// Send event for getting hit
				memset( &(SWeaponHit), 0, sizeof( SWeaponHit ) );
				SWeaponHit.usSoldierID			= pTargetSoldier->ubID;
				SWeaponHit.usWeaponIndex		= pSoldier->usAttackingWeapon;
				SWeaponHit.sDamage					= (INT16) iImpact;
				SWeaponHit.usDirection			= GetDirectionFromGridNo( pSoldier->sGridNo, pTargetSoldier );
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
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - missed in HTH attack") );
				FreeUpAttacker( (UINT8) pSoldier->ubID );
			}
		}
	}

	// possibly reduce monster smell (gunpowder smell)
	if ( pSoldier->bMonsterSmell > 0 && Random( 5 ) == 0 )
	{
		pSoldier->bMonsterSmell--;
	}


	return( TRUE );
}

BOOLEAN UseThrown( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	UINT32			uiHitChance, uiDiceRoll;
	INT8			bLoop;
	UINT8			ubTargetID;
	SOLDIERTYPE	*	pTargetSoldier;
	INT16			sAPCost = 0;

	uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->bAimTime, AIM_SHOT_TORSO );

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

	
	CalculateLaunchItemParamsForThrow( pSoldier, sTargetGridNo, pSoldier->bTargetLevel, (INT16)(pSoldier->bTargetLevel * 256 ), &(pSoldier->inv[ HANDPOS ] ), (INT8)(uiDiceRoll - uiHitChance), THROW_ARM_ITEM, 0 );

	// Madd: Next 2 lines added: Deduct points!

	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, pSoldier->bAimTime );
    // Kaiden: Deducting points too early, moving the line down
	//DeductPoints( pSoldier, sAPCost, 0 );

	// OK, goto throw animation
	HandleSoldierThrowItem( pSoldier, pSoldier->sTargetGridNo );
    // Kaiden: Deducting points too early, moving the line down
	DeductPoints( pSoldier, sAPCost, 0 );
	RemoveObjs( &(pSoldier->inv[ HANDPOS ] ), 1 );

	return( TRUE );
}


BOOLEAN UseLauncher( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	UINT32			uiHitChance, uiDiceRoll;
	INT16				sAPCost = 0;
	INT8				bAttachPos;
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
	for (bAttachPos = 0; bAttachPos < MAX_ATTACHMENTS; bAttachPos++)
	{
		if (pObj->usAttachItem[ bAttachPos ] != NOTHING)
		{
			if ( Item[ pObj->usAttachItem[ bAttachPos ] ].usItemClass & IC_EXPLOSV )
			{
				break;
			}
		}
	}
	if (bAttachPos == MAX_ATTACHMENTS)
	{
		// this should not happen!!
		return( FALSE );
	}

	if ( Weapon[GetAttachedGrenadeLauncher(pObj)].ubMagSize > 1 || ( Item[pObj->usItem].grenadelauncher && GetMagSize(pObj) > 1 ))
		CreateItem( pObj->usAttachItem[ bAttachPos ], 100, &Launchable );
	else
		CreateItem( pObj->usAttachItem[ bAttachPos ], pObj->bAttachStatus[ bAttachPos ], &Launchable );
	//if ( pSoldier->usAttackingWeapon == pObj->usItem )
	//{
		DeductAmmo( pSoldier, HANDPOS );
	//}
	//else
	//{
	//	// Firing an attached grenade launcher... the attachment we found above
	//	// is the one to remove!
	//	RemoveAttachment( pObj, bAttachPos, NULL );
	//}

  // ATE: Check here if the launcher should fail 'cause of bad status.....
  if ( WillExplosiveWeaponFail( pSoldier, pObj ) )
  {
    // Explode dude!

    // So we still should have ABC > 0
    // Begin explosion due to failure...
		IgniteExplosion( pSoldier->ubID, (INT16)CenterX( pSoldier->sGridNo ), (INT16)CenterY( pSoldier->sGridNo ), 0, pSoldier->sGridNo, Launchable.usItem, pSoldier->bLevel );

    // Reduce again for attack end 'cause it has been incremented for a normal attack
    // 
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY BAD EXPLOSIVE CHECK, Now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
		ReduceAttackBusyCount( pSoldier->ubID, FALSE );

    // So all's well, should be good from here....
    return( FALSE );
  }

	if ( Weapon[ usItemNum ].sSound != NO_WEAPON_SOUND  )
	{
		if (Item[ usItemNum ].usItemClass == IC_GUN  )
			PlayJA2Sample( Weapon[ UNDER_GLAUNCHER ].sSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );			
		else
			PlayJA2Sample( Weapon[ usItemNum ].sSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );			
	}

	uiHitChance = CalcThrownChanceToHit( pSoldier, sTargetGridNo, pSoldier->bAimTime, AIM_SHOT_TORSO );

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

	 	sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, FALSE, 0 );

	}
	else
	{
		// Throw....
		sAPCost = MinAPsToThrow( pSoldier, sTargetGridNo, FALSE );
	}

	if ( pSoldier->bDoBurst )
	{
		// ONly deduct points once
		if ( pSoldier->bDoBurst == 1 )
		{
			DeductPoints( pSoldier, sAPCost, 0 );
		}
	}
	else
	{
		DeductPoints( pSoldier, sAPCost, 0 );
	}

	CalculateLaunchItemParamsForThrow( pSoldier, sTargetGridNo, pSoldier->bTargetLevel, 0, &Launchable, (INT8)(uiDiceRoll - uiHitChance), THROW_ARM_ITEM, 0 );

	iID = CreatePhysicalObject( pSoldier->pTempObject, pSoldier->pThrowParams->dLifeSpan,  pSoldier->pThrowParams->dX, pSoldier->pThrowParams->dY, pSoldier->pThrowParams->dZ, pSoldier->pThrowParams->dForceX, pSoldier->pThrowParams->dForceY, pSoldier->pThrowParams->dForceZ, pSoldier->ubID, pSoldier->pThrowParams->ubActionCode, pSoldier->pThrowParams->uiActionData );

	pObject = &( ObjectSlots[ iID ] );
  //pObject->fPotentialForDebug = TRUE;


	MemFree( pSoldier->pTempObject );
	pSoldier->pTempObject = NULL;

	MemFree( pSoldier->pThrowParams );
	pSoldier->pThrowParams = NULL;

	if ( pSoldier->bDoBurst )
	{
		// done, if bursting, increment
		pSoldier->bDoBurst++;
	}

	return( TRUE );
}

BOOLEAN DoSpecialEffectAmmoMiss( UINT8 ubAttackerID, INT16 sGridNo, INT16 sXPos, INT16 sYPos, INT16 sZPos, BOOLEAN fSoundOnly, BOOLEAN fFreeupAttacker, INT32 iBullet )
{
	ANITILE_PARAMS	AniParams;
	UINT8						ubAmmoType;
  UINT16          usItem;

	ubAmmoType = MercPtrs[ ubAttackerID ]->inv[ MercPtrs[ ubAttackerID ]->ubAttackingHand ].ubGunAmmoType;
	usItem     = MercPtrs[ ubAttackerID ]->inv[ MercPtrs[ ubAttackerID ]->ubAttackingHand ].usItem;

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
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}

		if ( sGridNo != NOWHERE )
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
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}

		if ( sGridNo != NOWHERE )
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
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit structure - explosive ammo") );
				FreeUpAttacker( (UINT8) ubAttackerID );
			}
		}

		if ( sGridNo != NOWHERE )
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

      	NewSmokeEffect( sGridNo, usItem, 0, ubAttackerID );
  //  // Do Spread effect.......
  //  switch( usItem )
  //  {
  //		case CREATURE_YOUNG_MALE_SPIT:
		//case CREATURE_INFANT_SPIT:

  //    	NewSmokeEffect( sGridNo, VERY_SMALL_CREATURE_GAS, 0, ubAttackerID );
  //      break;
  //    case CREATURE_OLD_MALE_SPIT:
  //    	NewSmokeEffect( sGridNo, SMALL_CREATURE_GAS, 0, ubAttackerID );
  //      break;

  //    case CREATURE_QUEEN_SPIT:
  //    	NewSmokeEffect( sGridNo, LARGE_CREATURE_GAS, 0, ubAttackerID );
  //      break;
  //  }
  }

	return( FALSE );
}


void WeaponHit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	SOLDIERTYPE				*pTargetSoldier, *pSoldier;

	// Get attacker
	pSoldier				= MercPtrs[ ubAttackerID ];
	
	// Get Target
	pTargetSoldier	= MercPtrs[ usSoldierID ];

	MakeNoise( ubAttackerID, pTargetSoldier->sGridNo, pTargetSoldier->bLevel, gpWorldLevelData[pTargetSoldier->sGridNo].ubTerrainID, Weapon[ usWeaponIndex ].ubHitVolume, NOISE_BULLET_IMPACT );

	if ( EXPLOSIVE_GUN( usWeaponIndex ) )
	{
		// Reduce attacker count!
		if ( Item[usWeaponIndex].rocketlauncher  )
		{
			if ( Item[usWeaponIndex].singleshotrocketlauncher )
			{
				IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, (INT16) (GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos )), C1, pTargetSoldier->bLevel );
			}
			else
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d",pSoldier->inv[HANDPOS].usItem , pSoldier->inv[HANDPOS].usGunAmmoItem ) );
				
				IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, (INT16) (GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos )), pSoldier->inv[pSoldier->ubAttackingHand ].usGunAmmoItem, pTargetSoldier->bLevel );
				pSoldier->inv[pSoldier->ubAttackingHand ].usGunAmmoItem = NONE;
			}
		}
		else // tank cannon
		{
			IgniteExplosion( ubAttackerID, sXPos, sYPos, 0, (INT16) (GETWORLDINDEXFROMWORLDCOORDS( sYPos, sXPos )), TANK_SHELL, pTargetSoldier->bLevel );
		}

		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of LAW fire") );
		FreeUpAttacker( ubAttackerID );
		return;
	}

	DoSpecialEffectAmmoMiss( ubAttackerID, pTargetSoldier->sGridNo, sXPos, sYPos, sZPos, FALSE, FALSE, 0 );

	// OK, SHOT HAS HIT, DO THINGS APPROPRIATELY
  // ATE: This is 'cause of that darn smoke effect that could potnetially kill
  // the poor bastard .. so check
  if ( !pTargetSoldier->fDoingExternalDeath )
  {
	  EVENT_SoldierGotHit( pTargetSoldier,	usWeaponIndex, sDamage, sBreathLoss, usDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation, 0, NOWHERE );
  }
  else
  {
    // Buddy had died from additional dammage - free up attacker here...
		ReduceAttackBusyCount( pTargetSoldier->ubAttackerID, FALSE );
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Special effect killed before bullet impact, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
  }
}


void StructureHit( INT32 iBullet, UINT16 usWeaponIndex, INT8 bWeaponStatus, UINT8 ubAttackerID, UINT16 sXPos, INT16 sYPos, INT16 sZPos, UINT16 usStructureID, INT32 iImpact, BOOLEAN fStopped )
{
	BOOLEAN						fDoMissForGun = FALSE;
	ANITILE						*pNode;
	INT16							sGridNo;
	ANITILE_PARAMS	AniParams;
	UINT16					usMissTileIndex, usMissTileType;
	STRUCTURE				*pStructure = NULL;
	UINT32					uiMissVolume = MIDVOLUME;
	BOOLEAN					fHitSameStructureAsBefore;
	BULLET *				pBullet;
	SOLDIERTYPE *		pAttacker;
	
	pBullet = GetBulletPtr( iBullet );

	if ( fStopped && ubAttackerID != NOBODY )
	{
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
			MakeNoise( ubAttackerID, sGridNo, 1, gpWorldLevelData[sGridNo].ubTerrainID, Weapon[ usWeaponIndex ].ubHitVolume, NOISE_BULLET_IMPACT );
		}
		else
		{
			MakeNoise( ubAttackerID, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, Weapon[ usWeaponIndex ].ubHitVolume, NOISE_BULLET_IMPACT );
		}

	}

	if (fStopped)
	{
		if ( Item[usWeaponIndex].rocketlauncher )
		{
			RemoveBullet( iBullet );

			// Reduce attacker count!
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of LAW fire") );
			FreeUpAttacker( ubAttackerID );
			if ( Item[usWeaponIndex].rocketlauncher )
			{
				IgniteExplosion( ubAttackerID, (INT16)CenterX( sGridNo ), (INT16)CenterY( sGridNo ), 0, sGridNo, C1, (INT8)( sZPos >= WALL_HEIGHT ) );
			}
			else
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("StructureHit: RPG7 item: %d, Ammo: %d",pAttacker->inv[HANDPOS].usItem , pAttacker->inv[HANDPOS].usGunAmmoItem ) );
				
				IgniteExplosion( ubAttackerID, (INT16)CenterX( sGridNo ), (INT16)CenterY( sGridNo ), 0, sGridNo, pAttacker->inv[pAttacker->ubAttackingHand ].usGunAmmoItem , (INT8)( sZPos >= WALL_HEIGHT ) );
				pAttacker->inv[pAttacker->ubAttackingHand ].usGunAmmoItem = NONE;
			}
			return;
		}

		if ( Item[usWeaponIndex].cannon )
		{
			RemoveBullet( iBullet );

			// Reduce attacker count!
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of TANK fire") );
			FreeUpAttacker( ubAttackerID );

			IgniteExplosion( ubAttackerID, (INT16)CenterX( sGridNo ), (INT16)CenterY( sGridNo ), 0, sGridNo, TANK_SHELL, (INT8)( sZPos >= WALL_HEIGHT ) );
			//FreeUpAttacker( (UINT8) ubAttackerID );

			return;
		}
	}

	// Get Structure pointer and damage it!
	if ( usStructureID != INVALID_STRUCTURE_ID )
	{
		pStructure = FindStructureByID( sGridNo, usStructureID );

		DamageStructure( pStructure, (UINT8)iImpact, STRUCTURE_DAMAGE_GUNFIRE, sGridNo, sXPos, sYPos, ubAttackerID );
	}

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
						DoMercBattleSound(  MercPtrs[ ubAttackerID ], BATTLE_SOUND_CURSE1 );
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
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - monster attack hit structure") );
			FreeUpAttacker( (UINT8) ubAttackerID );

			//PlayJA2Sample( SPIT_RICOCHET , RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );			
			break;

		case KNIFECLASS:

			// When it hits the ground, leave on map...
			if ( Item[ usWeaponIndex ].usItemClass == IC_THROWING_KNIFE )
			{
				OBJECTTYPE		Object;

				// OK, have we hit ground?
				if ( usStructureID == INVALID_STRUCTURE_ID )
				{	
					// Add item
					CreateItem( usWeaponIndex, bWeaponStatus, &Object );

					AddItemToPool( sGridNo, &Object, -1 , 0, 0, -1 );

					// Make team look for items
					NotifySoldiersToLookforItems( );
				}

				if ( !fHitSameStructureAsBefore )
				{
					PlayJA2Sample( MISS_KNIFE, RATE_11025, uiMissVolume, 1, SoundDir( sGridNo ) );			
				}

				RemoveBullet( iBullet );
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - knife attack hit structure") );
				FreeUpAttacker( (UINT8) ubAttackerID );
			}
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
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet hit same structure twice") );
				FreeUpAttacker( (UINT8) ubAttackerID );
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
				if ( gpWorldLevelData[ sGridNo ].ubTerrainID == LOW_WATER ||  gpWorldLevelData[ sGridNo ].ubTerrainID == DEEP_WATER )
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

void WindowHit( INT16 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth, BOOLEAN fLargeForce )
{
	STRUCTURE *			pWallAndWindow;
	DB_STRUCTURE *	pWallAndWindowInDB;
	INT16						sShatterGridNo;
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

	PlayJA2Sample( GLASS_SHATTER1 + Random(2), RATE_11025, MIDVOLUME, 1, SoundDir( sGridNo ) );			

}


BOOLEAN InRange( SOLDIERTYPE *pSoldier, INT16 sGridNo )
{
	 INT16								sRange;	
	 UINT16								usInHand;

	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("InRange"));
	 usInHand = pSoldier->inv[HANDPOS].usItem;

	 if ( Item[ usInHand ].usItemClass == IC_GUN || Item[ usInHand ].usItemClass == IC_THROWING_KNIFE || (Item[usInHand].rocketlauncher && !Item[usInHand].singleshotrocketlauncher))
	 {
		 // Determine range
		 sRange = (INT16)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

		 if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
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
			 if ( sRange <= GunRange( &(pSoldier->inv[HANDPOS]) ) )
			 {
					return( TRUE );
			 }
		 }
	 }
	 return( FALSE );
}

UINT32 CalcChanceToHitGun(SOLDIERTYPE *pSoldier, UINT16 sGridNo, UINT8 ubAimTime, UINT8 ubAimPos )
{
  //SOLDIERTYPE *vicpSoldier;
	SOLDIERTYPE * pTarget;
	INT32 iChance, iRange, iSightRange, iMaxRange, iBonus; //, minRange;
	//rain
	//INT32 iScopeBonus; 
	//end rain

  INT32 iGunCondition, iMarksmanship;
	INT32 iPenalty;
	UINT16	usInHand;
	OBJECTTYPE * pInHand;
//	INT8 bAttachPos;
	INT8 bBandaged;
	INT16	sDistVis;
	UINT8	ubAdjAimPos;
	UINT8 ubTargetID;

	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun"));

	 if ( pSoldier->bMarksmanship == 0 )
	{
		// always min chance
		return( MINCHANCETOHIT );
	}

  // make sure the guy's actually got a weapon in his hand!
	pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);
  usInHand = pSoldier->usAttackingWeapon;

	// DETERMINE BASE CHANCE OF HITTING
	iGunCondition = WEAPON_STATUS_MOD( pInHand->bGunStatus );

	if (Item[usInHand].rocketlauncher )
	{
		// use the same calculation as for mechanical thrown weapons
		iMarksmanship = ( EffectiveDexterity( pSoldier ) + EffectiveMarksmanship( pSoldier ) + EffectiveWisdom( pSoldier ) + (10 * EffectiveExpLevel( pSoldier ) )) / 4;
		// heavy weapons trait helps out
		if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPS ))
		{
			iMarksmanship += gbSkillTraitBonus[HEAVY_WEAPS] * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS );
		}
	}
	else
	{
		iMarksmanship = EffectiveMarksmanship( pSoldier );

		if ( AM_A_ROBOT( pSoldier ) )
		{
			SOLDIERTYPE * pSoldier2;

			pSoldier2 = GetRobotController( pSoldier );
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
	// if shooting same target as the last shot
	if (sGridNo == pSoldier->sLastTarget )
		iChance += AIM_BONUS_SAME_TARGET;		// give a bonus to hit

	if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[ pSoldier->ubProfile ].bPersonalityTrait == PSYCHO )
	{
		iChance += AIM_BONUS_PSYCHO;
	}

	// calculate actual range (in units, 10 units = 1 tile)
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

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
		if (pSoldier->inv[SECONDHANDPOS].usItem == NOTHING)
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

			if ( !HAS_SKILL_TRAIT( pSoldier, AMBIDEXT ) )
			{
				if ( IsValidSecondHandShot( pSoldier ) )
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

	// If in burst mode, deduct points for change to hit for each shot after the first
	if ( pSoldier->bDoBurst )
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
	}

	sDistVis = DistanceVisible( pSoldier, DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, sGridNo, 0, pSoldier );

	// give some leeway to allow people to spot for each other... 
	// use distance limitation for LOS routine of 2 x maximum distance EVER visible, so that we get accurate
	// calculations out to around 50 tiles.  Because we multiply max distance by 2, we must divide by 2 later

	// CJC August 13 2002:  Wow, this has been wrong the whole time.  bTargetCubeLevel seems to be generally set to 2 -
	// but if a character is shooting at an enemy in a particular spot, then we should be using the target position on the body.

	// CJC August 13, 2002
	// If the start soldier has a body part they are aiming at, and know about the person in the tile, then use that height instead
	iSightRange = -1;

	ubTargetID = WhoIsThere2( sGridNo, pSoldier->bTargetLevel );
	// best to use team knowledge as well, in case of spotting for someone else
	if (ubTargetID != NOBODY && pSoldier->bOppList[ubTargetID] == SEEN_CURRENTLY || gbPublicOpplist[pSoldier->bTeam][ubTargetID] == SEEN_CURRENTLY)
	{
		iSightRange = SoldierToBodyPartLineOfSightTest( pSoldier, sGridNo, pSoldier->bTargetLevel, pSoldier->bAimShotLocation, (UINT8) (MaxDistanceVisible() * 2), TRUE );	
	}
	
	if (iSightRange == -1) // didn't do a bodypart-based test
	{
		iSightRange = SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pSoldier->bTargetLevel, pSoldier->bTargetCubeLevel, (UINT8) (MaxDistanceVisible() * 2), TRUE );
	}
	
	iSightRange *= 2;

	if ( iSightRange > (sDistVis * CELL_X_SIZE) )
	{
		// shooting beyond max normal vision... penalize such distance at double (also later we halve the remaining chance)
		iSightRange += (iSightRange - sDistVis * CELL_X_SIZE);
	}

	// if shooter spent some extra time aiming and can see the target
	if (iSightRange > 0 && ubAimTime && !pSoldier->bDoBurst )
		iChance += (AIM_BONUS_PER_AP * ubAimTime); // bonus for every pt of aiming

	if ( !(pSoldier->uiStatusFlags & SOLDIER_PC ) )	// if this is a computer AI controlled enemy
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
	if ( pSoldier->uiStatusFlags & SOLDIER_GASSED )
	{
	  iChance -= AIM_PENALTY_GASSED;
	}

	// if shooter is being bandaged at the same time, his concentration is off
	if (pSoldier->ubServiceCount > 0)
	  iChance -= AIM_PENALTY_GETTINGAID;

	// if shooter is still in shock
	if (pSoldier->bShock)
		iChance -= (pSoldier->bShock * AIM_PENALTY_PER_SHOCK);

	if ( Item[ usInHand ].usItemClass == IC_GUN )
	{
		iMaxRange = GunRange( pInHand );
	}
	else
	{
		iMaxRange = CELL_X_SIZE; // one tile
	}

	if ( iSightRange > 0 )
	{

		// Madd: Spacific item bonuses are no longer hard-coded

		// Snap: use tohitbonus or aimbonus instead
		/*if (pSoldier->inv[HEAD1POS].usItem == SUNGOGGLES || pSoldier->inv[HEAD2POS].usItem == SUNGOGGLES)
		{
			// decrease effective range by 10% when using sungoggles (w or w/o scope)
			iSightRange -= iRange / 10;	//basically, +1% to hit per every 2 squares
		}*/

		//bAttachPos = FindAttachment( pInHand, SNIPERSCOPE );

		//// does gun have scope, long range recommends its use, and shooter's aiming?
		//if (bAttachPos != NO_SLOT && (iRange > MIN_SCOPE_RANGE) && (ubAimTime > 0))
		//{
		//	// reduce effective sight range by 20% per extra aiming time AP of the distance
		//	// beyond MIN_SCOPE_RANGE.  Max reduction is 80% of the range beyond.
		//	iScopeBonus = ((SNIPERSCOPE_AIM_BONUS * ubAimTime) * (iRange - MIN_SCOPE_RANGE)) / 100;

		//	// adjust for scope condition, only has full affect at 100%
		//	iScopeBonus = (iScopeBonus * WEAPON_STATUS_MOD(pInHand->bAttachStatus[bAttachPos])) / 100;

		//rain
		// reduce scope effectiveness when it's raining 
		// iScopeBonus /= 1 + gbCurrentRainIntensity; //madd: commented out -- didn't do anything but cause a warning :S
		//end rain

		//	// reduce effective range by the bonus obtained from the scope
		//	iSightRange -= iScopeBonus;
		//	if (iSightRange < 1)
		//	{
		//		iSightRange = 1;
		//	}
		//}

		//bAttachPos = FindAttachment( pInHand, LASERSCOPE );
		//if (usInHand == ROCKET_RIFLE || usInHand == AUTO_ROCKET_RIFLE || bAttachPos != NO_SLOT) // rocket rifle has one built in
		//{
		//	INT8 bLaserStatus;

		//	if ( usInHand == ROCKET_RIFLE || usInHand == AUTO_ROCKET_RIFLE )
		//	{
		//		bLaserStatus = WEAPON_STATUS_MOD(pInHand->bGunStatus);
		//	}
		//	else
		//	{
		//		bLaserStatus = WEAPON_STATUS_MOD(pInHand->bAttachStatus[ bAttachPos ]);
		//	}
		//	 
		//	// laser scope isn't of much use in high light levels; add something for that
		//	if (bLaserStatus > 50)
		//	{
		//		iScopeBonus = LASERSCOPE_BONUS * (bLaserStatus - 50) / 50;
		//	}
		//	else
		//	{
		//		// laser scope in bad condition creates aim penalty!
		//		iScopeBonus = - LASERSCOPE_BONUS * (50 - bLaserStatus) / 50;
		//	}
		//	
		//	iChance += iScopeBonus;

		//}

		//Madd: Some gear can affect the to-hit bonus and the bonus from aiming
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: before bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));
		iSightRange -= GetGearAimBonus ( pSoldier, iRange, ubAimTime );
		iChance += GetGearToHitBonus ( pSoldier );
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after gear bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));

		// Reduce effective range by the aiming bonus (e.g. from sniper scope)
		iSightRange -= GetAimBonus( pInHand, iRange, ubAimTime );
		if (iSightRange < 1)
		{
			iSightRange = 1;
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after scope bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));

		UINT8 bLightLevel = LightTrueLevel(sGridNo, pSoldier->bTargetLevel);
		// Apply flat to-hit bonus (e.g. from laser scope)
		// Snap: If prone and range is good, apply bipod bonus here as well
		iChance += GetToHitBonus( pInHand, iSightRange, bLightLevel,
			gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE && iRange > MIN_PRONE_RANGE );
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: after to hit bonus - ubAimTime = %d, iSightRange = %d, iChance = %d ", ubAimTime, iSightRange, iChance));
	}

	// if aiming at the head, reduce chance to hit
	if (ubAimPos == AIM_SHOT_HEAD)	
	{
		// penalty of 3% per tile
		iPenalty = 3 * iSightRange / 10;
		iChance -= iPenalty;
	}
	else if (ubAimPos == AIM_SHOT_LEGS)
	{
		// penalty of 1% per tile
		iPenalty = iSightRange / 10;
		iChance -= iPenalty;
	}

	//NumMessage("EFFECTIVE RANGE = ",range);

	// ADJUST FOR RANGE
	// bonus if range is less than normal range, penalty if it's more
	//iChance += (NORMAL_RANGE - iRange) / (CELL_X_SIZE / 5);	// 5% per tile

	// Effects of actual gun max range... the numbers are based on wanting -40%
	// at range 26for a pistol with range 13, and -0 for a sniper rifle with range 80
	iPenalty = ((iMaxRange - iRange * 3) * 10) / (17 * CELL_X_SIZE);
	if ( iPenalty < 0 )
	{
		iChance += iPenalty;
	}
	//iChance -= 20 * iRange / iMaxRange;

	if ( TANK( pSoldier ) && ( iRange / CELL_X_SIZE < MaxDistanceVisible() ) )
	{
		// tank; penalize at close range!
		// 2 percent per tile closer than max visible distance
		iChance -= 2 * ( MaxDistanceVisible() - (iRange / CELL_X_SIZE) );
	}

	if (iSightRange == 0)
	{
		// firing blind!
		iChance -= AIM_PENALTY_BLIND;
	}
	else
	{
		// Effects based on aiming & sight
		// From for JA2.5:  3% bonus/penalty for each tile different from range NORMAL_RANGE.
		// This doesn't provide a bigger bonus at close range, but stretches it out, making medium 
		// range less penalized, and longer range more penalized
		iChance += 3 * ( NORMAL_RANGE - iSightRange ) / CELL_X_SIZE;
		/*
		if (iSightRange < NORMAL_RANGE)
		{
			// bonus to hit of 20% at point blank (would be 25% at range 0);
			//at NORMAL_RANGE, bonus is 0
			iChance += 25 * (NORMAL_RANGE - iSightRange) / NORMAL_RANGE;
		}
		else
		{
			// penalty of 2% / tile
			iChance -= (iSightRange - NORMAL_RANGE) / 5;
		}
		*/
	}

	// adjust for roof/not on roof
	if ( pSoldier->bLevel == 0 )
	{
		if ( pSoldier->bTargetLevel > 0 )
		{
			// penalty for firing up
			iChance -= AIM_PENALTY_FIRING_UP;
		}
	}
	else // pSoldier->bLevel > 0 )
	{
		if ( pSoldier->bTargetLevel == 0 )
		{
			iChance += AIM_BONUS_FIRING_DOWN;
		}
		// if have roof trait, give bonus
		if ( HAS_SKILL_TRAIT( pSoldier, ONROOF ) )
		{
			iChance += gbSkillTraitBonus[ ONROOF ] * NUM_SKILL_TRAITS( pSoldier, ONROOF );
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
		iPenalty = __min( ((pTarget->bTilesMoved * 3) / 2), 30 );
		iChance -= iPenalty;

		// if target sees us, he may have a chance to dodge before the gun goes off
		// but ability to dodge is reduced if crouched or prone!
		if (pTarget->bOppList[pSoldier->ubID] == SEEN_CURRENTLY && !TANK( pTarget ) && !(pSoldier->ubBodyType != QUEENMONSTER) )
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
	if ((victim = WhoIsThere(sGridNo)) < NOBODY)
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
	if ((iChance > 0) && (pSoldier->bLife < pSoldier->bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		bBandaged = pSoldier->bLifeMax - pSoldier->bLife - pSoldier->bBleeding;

		// injury penalty is based on % damage taken (max 2/3rds chance)
		iPenalty = (iChance * 2 * (pSoldier->bLifeMax - pSoldier->bLife + (bBandaged / 2))) /
						(3 * pSoldier->bLifeMax);

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
		iChance /= 2;
	}
	if ( iSightRange > (sDistVis * CELL_X_SIZE) )
	{
		// penalize out of sight shots, cumulative to effective range penalty
		iChance /= 2;
	}

  // MAKE SURE CHANCE TO HIT IS WITHIN DEFINED LIMITS
  if (iChance < MINCHANCETOHIT)
	{
		if ( TANK( pSoldier ) )
		{
			// allow absolute minimums
			iChance = 0;
		}
		else
		{
			iChance = MINCHANCETOHIT;
		}
	}
  else
   {
    if (iChance > MAXCHANCETOHIT)
      iChance = MAXCHANCETOHIT;
   }

//  NumMessage("ChanceToHit = ",chance);
   DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcChanceToHitGun: ichance = %d",iChance));
  return (iChance);
}

UINT32 AICalcChanceToHitGun(SOLDIERTYPE *pSoldier, UINT16 sGridNo, UINT8 ubAimTime, UINT8 ubAimPos )
{
	UINT16	usTrueState;
	UINT32	uiChance;

	// same as CCTHG but fakes the attacker always standing
	usTrueState = pSoldier->usAnimState;
	pSoldier->usAnimState = STANDING;
	uiChance = CalcChanceToHitGun( pSoldier, sGridNo, ubAimTime, ubAimPos );
	pSoldier->usAnimState = usTrueState;
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

INT32 ArmourProtection( SOLDIERTYPE * pTarget, UINT16 ubArmourType, INT8 * pbStatus, INT32 iImpact, UINT8 ubAmmoType )
{
	INT32		iProtection, iAppliedProtection, iFailure;

	iProtection = Armour[ ubArmourType ].ubProtection;

	if ( !AM_A_ROBOT( pTarget ) )
	{
		// check for the bullet hitting a weak spot in the armour
		iFailure = PreRandom( 100 ) + 1 - *pbStatus;
		if (iFailure > 0)
		{
			iProtection -= iFailure;
			if (iProtection < 0)
			{
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
		if (Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_VEST || Armour[ ubArmourType ].ubArmourClass == ARMOURCLASS_LEGGINGS)
		{
			iProtection = 0;
		}
	}

	if ( !AM_A_ROBOT( pTarget ) )
	{
		*pbStatus -= (iAppliedProtection * Armour[ubArmourType].ubDegradePercent) / 100;
	}

	// return armour protection
	return( iProtection );
}


INT32 TotalArmourProtection( SOLDIERTYPE *pFirer, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iImpact, UINT8 ubAmmoType )
{
	INT32					iTotalProtection = 0, iSlot;
	OBJECTTYPE *	pArmour;
	INT8					bPlatePos = -1;

	if (pTarget->uiStatusFlags & SOLDIER_VEHICLE)
	{
		INT8 bDummyStatus = 100;

		//bDummyStatus = (INT8) pVehicleList[ pTarget->bVehicleID ].sExternalArmorLocationsStatus[ ubHitLocation ];

		iTotalProtection += ArmourProtection( pTarget, (UINT8) pVehicleList[ pTarget->bVehicleID ].sArmourType, &bDummyStatus, iImpact, ubAmmoType );

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

		pArmour = &(pTarget->inv[ iSlot ]);
		if (pArmour->usItem != NOTHING)
		{
			// check plates first
			//if ( iSlot == VESTPOS )
			//{
//				bPlatePos = FindAttachment( pArmour, CERAMIC_PLATES );
				bPlatePos = FindFirstArmourAttachment( pArmour);
				if (bPlatePos != -1)
				{
					// bullet got through jacket; apply ceramic plate armour
					iTotalProtection += ArmourProtection( pTarget, Item[pArmour->usAttachItem[bPlatePos]].ubClassIndex, &(pArmour->bAttachStatus[bPlatePos]), iImpact, ubAmmoType );
					if ( pArmour->bAttachStatus[bPlatePos] < USABLE )
					{
						// destroy plates!
						pArmour->usAttachItem[ bPlatePos ] = NOTHING;
						pArmour->bAttachStatus[ bPlatePos ] = 0;
						DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
#ifdef ENGLISH
						if ( pTarget->bTeam == gbPlayerNum )
						{
							// report plates destroyed!
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[61], pTarget->name );
						}
#endif
					}
				}
			//}


			// if the plate didn't stop the bullet...
			if ( iImpact > iTotalProtection )
			{
				iTotalProtection += ArmourProtection( pTarget, Item[pArmour->usItem].ubClassIndex, &(pArmour->bStatus[0]), iImpact, ubAmmoType );
				if ( pArmour->bStatus[ 0 ] < USABLE )
				{
					DeleteObj( pArmour );
					DirtyMercPanelInterface( pTarget, DIRTYLEVEL2 );
				}
			}
		}
	}
	return( iTotalProtection );
}

INT32 BulletImpact( SOLDIERTYPE *pFirer, SOLDIERTYPE * pTarget, UINT8 ubHitLocation, INT32 iOrigImpact, INT16 sHitBy, UINT8 * pubSpecial )
{
	INT32					iImpact, iFluke, iBonus, iImpactForCrits = 0;
	INT8					bStatLoss;
	UINT8					ubAmmoType;

	// NOTE: reduction of bullet impact due to range and obstacles is handled
	// in MoveBullet.

	// Set a few things up:
	if ( Item[ pFirer->usAttackingWeapon ].usItemClass == IC_THROWING_KNIFE )
	{
		ubAmmoType = AMMO_KNIFE;
	}
	else
	{
		ubAmmoType = pFirer->inv[pFirer->ubAttackingHand].ubGunAmmoType;
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
	iFluke = PreRandom(51) - 25;		// gives (0 to 50 -25) -> -25% to +25%
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

	if (pubSpecial && *pubSpecial == FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL)
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

		if ( AmmoTypes[ubAmmoType].dart && sHitBy > 20 )
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

		AdjustImpactByHitLocation( iImpact, ubHitLocation, &iImpact, &iImpactForCrits );

		switch( ubHitLocation )
		{
			case AIM_SHOT_HEAD:
				// is the blow deadly enough for an instant kill?
				if ( PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && pFirer->usAttackingWeapon  == BARRETT ))
				{
					if (iImpactForCrits > MIN_DAMAGE_FOR_INSTANT_KILL && iImpactForCrits < pTarget->bLife)
					{
						// blow to the head is so deadly that it causes instant death;
						// the target has more life than iImpact so we increase it
						iImpact = pTarget->bLife + Random( 10 );
						iImpactForCrits = iImpact;
					}

					if (pubSpecial)
					{
						// is the blow deadly enough to cause a head explosion?
						if ( iImpactForCrits >= pTarget->bLife )
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
				if ( pubSpecial && IS_MERC_BODY_TYPE( pTarget ) && gAnimControl[ pTarget->usAnimState ].ubEndHeight == ANIM_STAND && pTarget->bOverTerrainType != LOW_WATER && pTarget->bOverTerrainType != MED_WATER && pTarget->bOverTerrainType != DEEP_WATER  )
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
				if ( PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (PythSpacesAway( pFirer->sGridNo, pTarget->sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && pFirer->usAttackingWeapon  == BARRETT ))
				{
					if (iImpact > MIN_DAMAGE_FOR_INSTANT_KILL && iImpact < pTarget->bLife)
					{
						// blow to the chest is so deadly that it causes instant death;
						// the target has more life than iImpact so we increase it
						iImpact = pTarget->bLife + Random( 10 );
						iImpactForCrits = iImpact;
					}
					// special thing for hitting chest - allow cumulative damage to count
					else if ( (iImpact + pTarget->sDamage) > (MIN_DAMAGE_FOR_BLOWN_AWAY + MIN_DAMAGE_FOR_INSTANT_KILL) )
					{
						iImpact = pTarget->bLife + Random( 10 );
						iImpactForCrits = iImpact;
					}

					// is the blow deadly enough to cause a chest explosion?
					if (pubSpecial)
					{
						if (iImpact > MIN_DAMAGE_FOR_BLOWN_AWAY && iImpact >= pTarget->bLife)
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

		if ( AmmoTypes[ubAmmoType].knife && pFirer->bOppList[ pTarget->ubID ] == SEEN_CURRENTLY )
		{
			// is this a stealth attack?
			if ( pTarget->bOppList[ pFirer->ubID ] == NOT_HEARD_OR_SEEN && !CREATURE_OR_BLOODCAT( pTarget ) && (ubHitLocation == AIM_SHOT_HEAD || ubHitLocation == AIM_SHOT_TORSO ) )
			{
				if ( PreRandom( 100 ) < (UINT32)(sHitBy + 10 * NUM_SKILL_TRAITS( pFirer, THROWING )) )
				{
					// instant death!					
					iImpact = pTarget->bLife + Random( 10 );
					iImpactForCrits = iImpact;
				}
			}
		}

		if (iImpactForCrits > 0 && iImpactForCrits < pTarget->bLife )
		{
			if (PreRandom( iImpactForCrits / 2 + pFirer->bAimTime * 5) + 1 > CRITICAL_HIT_THRESHOLD)
			{
				bStatLoss = (INT8) PreRandom( iImpactForCrits / 2 ) + 1;
				switch( ubHitLocation )
				{
					case AIM_SHOT_HEAD:
						if (bStatLoss >= pTarget->bWisdom)
						{
							bStatLoss = pTarget->bWisdom - 1;
						}
						if ( bStatLoss > 0 )
						{
							pTarget->bWisdom -= bStatLoss;

							if (pTarget->ubProfile != NO_PROFILE)
							{
								gMercProfiles[ pTarget->ubProfile ].bWisdom = pTarget->bWisdom;
							}


							if (pTarget->name[0] && pTarget->bVisible == TRUE)
							{
								// make stat RED for a while...
								pTarget->uiChangeWisdomTime = GetJA2Clock();
								pTarget->usValueGoneUp &= ~( WIS_INCREASE );

								if (bStatLoss == 1)
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_WISDOM], pTarget->name );
								}
								else
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_WISDOM], pTarget->name, bStatLoss );
								}
							}
						}
						else if ( pTarget->bNumPelletsHitBy == 0 )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_HEAD_HIT], pTarget->name );
						}
						break;
					case AIM_SHOT_TORSO:
						if (PreRandom( 1 ) == 0 && !(pTarget->uiStatusFlags & SOLDIER_MONSTER) )
						{
							if (bStatLoss >= pTarget->bDexterity)
							{
								bStatLoss = pTarget->bDexterity - 1;
							}
							if ( bStatLoss > 0 )
							{
								pTarget->bDexterity -= bStatLoss;

								if (pTarget->ubProfile != NO_PROFILE)
								{
									gMercProfiles[ pTarget->ubProfile ].bDexterity = pTarget->bDexterity;
								}

								if (pTarget->name[0] && pTarget->bVisible == TRUE)
								{
									// make stat RED for a while...
									pTarget->uiChangeDexterityTime = GetJA2Clock();
									pTarget->usValueGoneUp &= ~( DEX_INCREASE );

									if (bStatLoss == 1)
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_DEX], pTarget->name );
									}
									else
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_DEX], pTarget->name, bStatLoss );
									}
								}
							}
						}
						else
						{
							if (bStatLoss >= pTarget->bStrength)
							{
								bStatLoss = pTarget->bStrength - 1;
							}
							if ( bStatLoss > 0 )
							{
								pTarget->bStrength -= bStatLoss;

								if (pTarget->ubProfile != NO_PROFILE)
								{
									gMercProfiles[ pTarget->ubProfile ].bStrength = pTarget->bStrength;
								}

								if (pTarget->name[0] && pTarget->bVisible == TRUE)
								{
									// make stat RED for a while...
									pTarget->uiChangeStrengthTime = GetJA2Clock();
									pTarget->usValueGoneUp &= ~( STRENGTH_INCREASE );

									if (bStatLoss == 1)
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_STRENGTH], pTarget->name );
									}
									else
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_STRENGTH], pTarget->name, bStatLoss );
									}
								}
							}
						}
						break;
					case AIM_SHOT_LEGS:
						if (bStatLoss >= pTarget->bAgility)
						{
							bStatLoss = pTarget->bAgility - 1;
						}	
						if ( bStatLoss > 0 )
						{				
							pTarget->bAgility -= bStatLoss;

							if (pTarget->ubProfile != NO_PROFILE)
							{
								gMercProfiles[ pTarget->ubProfile ].bAgility = pTarget->bAgility;
							}

							if (pTarget->name[0] && pTarget->bVisible == TRUE)
							{
								// make stat RED for a while...
								pTarget->uiChangeAgilityTime = GetJA2Clock();
								pTarget->usValueGoneUp &= ~( AGIL_INCREASE );

								if (bStatLoss == 1)
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_AGIL], pTarget->name );
								}
								else
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_AGIL], pTarget->name, bStatLoss );
								}
							}
						}
						break;
				}
			}
			else if ( ubHitLocation == AIM_SHOT_HEAD && pTarget->bNumPelletsHitBy == 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_HEAD_HIT], pTarget->name );			
			}
		}
	}

	return( iImpact );
}

INT32 HTHImpact( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pTarget, INT32 iHitBy, BOOLEAN fBladeAttack )
{
	INT32 iImpact, iFluke, iBonus;

	if (fBladeAttack)
	{
		iImpact = ( EffectiveExpLevel( pSoldier ) / 2); // 0 to 4 for level
		iImpact += GetDamage(&pSoldier->inv[HANDPOS]);
		iImpact += EffectiveStrength( pSoldier ) / 20; // 0 to 5 for strength, adjusted by damage taken

		if ( AM_A_ROBOT( pTarget ) )
		{
			iImpact /= 4;
		}
	}
	else
	{
		iImpact = ( EffectiveExpLevel( pSoldier ) / 2); // 0 to 4 for level
		iImpact += EffectiveStrength( pSoldier ) / 5; // 0 to 20 for strength, adjusted by damage taken

		// NB martial artists don't get a bonus for using brass knuckles!
		if (pSoldier->usAttackingWeapon && !( HAS_SKILL_TRAIT( pSoldier, MARTIALARTS ) ) )
		{
			iImpact += GetDamage(&pSoldier->inv[HANDPOS]);

			if ( AM_A_ROBOT( pTarget ) )
			{
				iImpact /= 2;
			}
		}
		else
		{
			// base HTH damage
			iImpact += 5;
			if ( AM_A_ROBOT( pTarget ) )
			{
				iImpact = 0;
			}
		}

	}
	

	iFluke = PreRandom( 51 ) - 25; // +/-25% bonus due to random factors
	iBonus = iHitBy / 2;				// up to 50% extra impact for accurate attacks
	
	iImpact = iImpact * (100 + iFluke + iBonus) / 100;

	if (!fBladeAttack)
	{
		// add bonuses for hand-to-hand and martial arts
		if ( HAS_SKILL_TRAIT( pSoldier, MARTIALARTS ) )
		{
			iImpact = iImpact * ( 100 + gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pSoldier, MARTIALARTS ) ) / 100;
			if (pSoldier->usAnimState == NINJA_SPINKICK)
			{
				iImpact *= 2;
			}
		}
		if ( HAS_SKILL_TRAIT( pSoldier, HANDTOHAND ) )			
		{
			// SPECIAL  - give TRIPLE bonus for damage for hand-to-hand trait
			// because the HTH bonus is half that of martial arts, and gets only 1x for to-hit bonus
			iImpact = iImpact * ( 100 + 3 * gbSkillTraitBonus[HANDTOHAND] * NUM_SKILL_TRAITS( pSoldier, HANDTOHAND ) ) / 100;
		}		
	}

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
					DoMercBattleSound(  MercPtrs[ ubAttackerID ], BATTLE_SOUND_CURSE1 );
				}
			}
			fDoMissForGun = TRUE;
			break;

		case MONSTERCLASS:
			PlayJA2Sample( SPIT_RICOCHET, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );			
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
				LocateGridNo( (INT16)pBullet->sGridNo );
			}
		}

	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - bullet missed") );
	FreeUpAttacker( ubAttackerID );
}

UINT32 CalcChanceHTH( SOLDIERTYPE * pAttacker,SOLDIERTYPE *pDefender, UINT8 ubAimTime, UINT8 ubMode )
{
  UINT16 usInHand;
	UINT8 ubBandaged;
  INT32 iAttRating, iDefRating;
	INT32 iChance;

  usInHand = pAttacker->usAttackingWeapon;

	if ( (usInHand != CREATURE_QUEEN_TENTACLES ) && (pDefender->bLife < OKLIFE || pDefender->bBreath < OKBREATH) )
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
		// this is more of a brute force strength-vs-strength check
		iAttRating = ( EffectiveDexterity( pAttacker ) + // coordination, accuracy
				 EffectiveAgility( pAttacker ) +    // speed & reflexes
				 3 * pAttacker->bStrength +    // physical strength (TRIPLED!)
				 (10 * EffectiveExpLevel( pAttacker ) ) );  // experience, knowledge
	}
	else
	{
		iAttRating = (3 * EffectiveDexterity( pAttacker ) + // coordination, accuracy (TRIPLED!)
				 EffectiveAgility( pAttacker ) +    // speed & reflexes
				 pAttacker->bStrength +    // physical strength
				 (10 * EffectiveExpLevel( pAttacker ) ) );  // experience, knowledge
	}

  iAttRating /= 6;  // convert from 6-600 to 1-100

	// psycho bonus
	if ( pAttacker->ubProfile != NO_PROFILE && gMercProfiles[ pAttacker->ubProfile ].bPersonalityTrait == PSYCHO )
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
    iAttRating += ((AIM_BONUS_PER_AP * ubAimTime) / 2);    //bonus for aiming
   }

  if (! (pAttacker->uiStatusFlags & SOLDIER_PC) )   // if attacker is a computer AI controlled enemy
	{
    iAttRating += gbDiff[ DIFF_ENEMY_TO_HIT_MOD ][ SoldierDifficultyLevel( pAttacker ) ];
	}

  // if attacker is being affected by gas
  if ( pAttacker->uiStatusFlags & SOLDIER_GASSED )
    iAttRating -= AIM_PENALTY_GASSED;

  // if attacker is being bandaged at the same time, his concentration is off
	if (pAttacker->ubServiceCount > 0)
	  iAttRating -= AIM_PENALTY_GETTINGAID;

  // if attacker is still in shock
  if (pAttacker->bShock)
    iAttRating -= (pAttacker->bShock * AIM_PENALTY_PER_SHOCK);

/*
  // if the attacker is an A.I.M. mercenary
  if (pAttacker->characternum < MAX_AIM_MERCS)	// exclude Gus
    iAttRating += AdjustChanceForProfile(pAttacker,pDefender);
*/

  // If attacker injured, reduce chance accordingly (by up to 2/3rds)
  if ((iAttRating > 0) && (pAttacker->bLife < pAttacker->bLifeMax))
   {
    // if bandaged, give 1/2 of the bandaged life points back into equation
    ubBandaged = pAttacker->bLifeMax - pAttacker->bLife - pAttacker->bBleeding;

    iAttRating -= (2 * iAttRating * (pAttacker->bLifeMax - pAttacker->bLife + (ubBandaged / 2))) /
		 (3 * pAttacker->bLifeMax);
   }

  // If attacker tired, reduce chance accordingly (by up to 1/2)
  if ((iAttRating > 0) && (pAttacker->bBreath < 100))
    iAttRating -= (iAttRating * (100 - pAttacker->bBreath)) / 200;

	if (pAttacker->ubProfile != NO_PROFILE)
	{
		if (ubMode == HTH_MODE_STAB)
		{
			if (HAS_SKILL_TRAIT( pAttacker, KNIFING ))			
			{
				iAttRating += gbSkillTraitBonus[KNIFING] * NUM_SKILL_TRAITS( pAttacker, KNIFING );
			}
		}
		else
		{
			// add bonuses for hand-to-hand and martial arts
			if (HAS_SKILL_TRAIT( pAttacker, MARTIALARTS ))
			{
				iAttRating += gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pAttacker, MARTIALARTS );
			}
			if ( HAS_SKILL_TRAIT( pAttacker, HANDTOHAND ) )
			{
				iAttRating += gbSkillTraitBonus[HANDTOHAND] * NUM_SKILL_TRAITS( pAttacker, HANDTOHAND );
			}
		}
	}


  if (iAttRating < 1)
    iAttRating = 1;


  // CALCULATE DEFENDER'S CLOSE COMBAT RATING (0-100)
	if (ubMode == HTH_MODE_STEAL)
	{
		iDefRating = (EffectiveAgility( pDefender )) +   // speed & reflexes
		   EffectiveDexterity( pDefender ) +  // coordination, accuracy
		   3 * pDefender->bStrength +    // physical strength (TRIPLED!)
	     (10 * EffectiveExpLevel( pDefender ) );  // experience, knowledge
	}
	else
	{
		iDefRating = (3 * EffectiveAgility( pDefender ) ) +   // speed & reflexes (TRIPLED!)
		   EffectiveDexterity( pDefender ) +  // coordination, accuracy
		   pDefender->bStrength +    // physical strength
	     (10 * EffectiveExpLevel( pDefender ) );  // experience, knowledge
	}
  
  iDefRating /= 6;  // convert from 6-600 to 1-100

	// modify chance to dodge by morale
	iDefRating += GetMoraleModifier( pDefender );

	// modify for fatigue
	iDefRating -= GetSkillCheckPenaltyForFatigue( pDefender, iDefRating );

  // if attacker is being affected by gas
  if ( pDefender->uiStatusFlags & SOLDIER_GASSED )
    iDefRating -= AIM_PENALTY_GASSED;

  // if defender is being bandaged at the same time, his concentration is off
	if (pDefender->ubServiceCount > 0)
	  iDefRating -= AIM_PENALTY_GETTINGAID;

  // if defender is still in shock
  if (pDefender->bShock)
    iDefRating -= (pDefender->bShock * AIM_PENALTY_PER_SHOCK);

/*
  // if the defender is an A.I.M. mercenary
  if (pDefender->characternum < MAX_AIM_MERCS)	// exclude Gus
    iDefRating += AdjustChanceForProfile(pDefender,pAttacker);
*/

  // If defender injured, reduce chance accordingly (by up to 2/3rds)
  if ((iDefRating > 0) && (pDefender->bLife < pDefender->bLifeMax))
   {
    // if bandaged, give 1/2 of the bandaged life points back into equation
    ubBandaged = pDefender->bLifeMax - pDefender->bLife - pDefender->bBleeding;

    iDefRating -= (2 * iDefRating * (pDefender->bLifeMax - pDefender->bLife + (ubBandaged / 2))) /
		 (3 * pDefender->bLifeMax);

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


	if (pDefender->ubProfile != NO_PROFILE)
	{
		if (ubMode == HTH_MODE_STAB)
		{
			if (Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE)
			{
				if ( HAS_SKILL_TRAIT( pDefender, KNIFING ) )
				{
					// good with knives, got one, so we're good at parrying
					iDefRating += gbSkillTraitBonus[KNIFING] * NUM_SKILL_TRAITS( pDefender, KNIFING );
				}
				if (HAS_SKILL_TRAIT( pDefender, MARTIALARTS ))
				{
					// the knife gets in the way but we're still better than nobody
					iDefRating += ( gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS ) ) / 3;
				}				
			}
			else
			{
				if ( HAS_SKILL_TRAIT( pDefender, KNIFING ) )
				{
					// good with knives, don't have one, but we know a bit about dodging
					iDefRating += ( gbSkillTraitBonus[KNIFING] * NUM_SKILL_TRAITS( pDefender, KNIFING ) ) / 3;
				}
				if (HAS_SKILL_TRAIT( pDefender, MARTIALARTS ))
				{
					// bonus for dodging knives
					iDefRating += ( gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS ) )/ 2;
				}
			}
		}
		else
		{	// punch/hand-to-hand/martial arts attack/steal
			if (Item[pDefender->inv[HANDPOS].usItem].usItemClass == IC_BLADE && ubMode != HTH_MODE_STEAL)
			{		
				if (HAS_SKILL_TRAIT( pDefender, KNIFING ))
				{
					// with our knife, we get some bonus at defending from HTH attacks
					iDefRating += ( gbSkillTraitBonus[KNIFING] * NUM_SKILL_TRAITS( pDefender, KNIFING ) ) / 2;
				}
			}
			else
			{
				if (HAS_SKILL_TRAIT( pDefender, MARTIALARTS ))
				{
					iDefRating += gbSkillTraitBonus[MARTIALARTS] * NUM_SKILL_TRAITS( pDefender, MARTIALARTS );
				}
				if (HAS_SKILL_TRAIT( pDefender, HANDTOHAND ))
				{
					iDefRating += gbSkillTraitBonus[HANDTOHAND] * NUM_SKILL_TRAITS( pDefender, HANDTOHAND );
				}
			}
		}
	}

  if (iDefRating < 1)
    iDefRating = 1;


  //NumMessage("CalcChanceToStab - Attacker's Rating = ",iAttRating);
  //NumMessage("CalcChanceToStab - Defender's Rating = ",iDefRating);

  // calculate chance to hit by comparing the 2 opponent's ratings
//  iChance = (100 * iAttRating) / (iAttRating + iDefRating);
	

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

		if ( pAttacker->bAimShotLocation == AIM_SHOT_HEAD )
		{
			// make this harder!
			iChance -= 20;
		}

	}


  // MAKE SURE CHANCE TO HIT IS WITHIN DEFINED LIMITS
  if (iChance < MINCHANCETOHIT)
	{
    iChance = MINCHANCETOHIT;
	}
  else
	{
		if (iChance > MAXCHANCETOHIT)
			iChance = MAXCHANCETOHIT;
	}

  //NumMessage("ChanceToStab = ",chance);

  return (iChance);
}

UINT32 CalcChanceToStab(SOLDIERTYPE * pAttacker,SOLDIERTYPE *pDefender, UINT8 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_STAB ) );
}

UINT32 CalcChanceToPunch(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, UINT8 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_PUNCH ) );
}

UINT32 CalcChanceToSteal(SOLDIERTYPE *pAttacker, SOLDIERTYPE * pDefender, UINT8 ubAimTime)
{
	return( CalcChanceHTH( pAttacker, pDefender, ubAimTime, HTH_MODE_STEAL ) );
}


void ReloadWeapon( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
{
	// NB this is a cheat function, don't award experience

	if ( pSoldier->inv[ ubHandPos ].usItem != NOTHING )
	{
		pSoldier->inv[ ubHandPos ].ubGunShotsLeft = GetMagSize(&pSoldier->inv[ ubHandPos ]);
		// Dirty Bars
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL1 );
	}
}



BOOLEAN IsGunWeaponModeCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , UINT8 bWpnMode)
{
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("IsGunWeaponModeCapable: weapon mode=%d",bWpnMode));

	switch(bWpnMode)
	{
		case WM_NORMAL:
		return (pSoldier->inv[ ubHandPos ].usItem != NOTHING && Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON && !Weapon[ pSoldier->inv[ ubHandPos ].usItem ].NoSemiAuto );  // Check for being a weapon....

		case WM_BURST:
		return IsGunBurstCapable(pSoldier, ubHandPos, FALSE);

		case WM_AUTOFIRE:
		return ((IsGunAutofireCapable(pSoldier, ubHandPos) || Weapon[ pSoldier->inv[ ubHandPos ].usItem ].NoSemiAuto )&& !Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher );

		case WM_ATTACHED_GL:
//		return (FindAttachment( &(pSoldier->inv[ubHandPos]), UNDER_GLAUNCHER ) != ITEM_NOT_FOUND && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), UNDER_GLAUNCHER ) != ITEM_NOT_FOUND );
		
		return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher &&  IsGrenadeLauncherAttached( &(pSoldier->inv[ubHandPos]) ) && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]) )) != ITEM_NOT_FOUND );
	
		case WM_ATTACHED_GL_BURST:
			return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher && IsGrenadeLauncherAttached( &(pSoldier->inv[ubHandPos]) ) && Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[ubHandPos])].ubShotsPerBurst > 0 && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]))) != ITEM_NOT_FOUND );

		case WM_ATTACHED_GL_AUTO:
			return FALSE;
//			return (!Item[pSoldier->inv[ubHandPos].usItem].grenadelauncher && Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[ubHandPos])].bAutofireShotsPerFiveAP > 0 && FindLaunchableAttachment( &(pSoldier->inv[ubHandPos]), GetAttachedGrenadeLauncher( &(pSoldier->inv[ubHandPos]) ) != ITEM_NOT_FOUND );

		default:
		return FALSE;
	}
}

BOOLEAN IsGunAutofireCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
{
	BOOLEAN fCapable = FALSE;

	if ( pSoldier->inv[ ubHandPos ].usItem != NOTHING )
	{
		// ATE: Check for being a weapon....
		if ( Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON )
		{
			if ( GetAutofireShotsPerFiveAPs(&pSoldier->inv[ ubHandPos ]) > 0 )
			{
				fCapable = TRUE;
			}
		}
	}

	return( fCapable );
}

BOOLEAN IsGunBurstCapable( SOLDIERTYPE *pSoldier, UINT8 ubHandPos , BOOLEAN fNotify )
{
	BOOLEAN fCapable = FALSE;

	if ( pSoldier->inv[ ubHandPos ].usItem != NOTHING )
	{
		// ATE: Check for being a weapon....
		if ( Item[ pSoldier->inv[ ubHandPos ].usItem ].usItemClass & IC_WEAPON )
		{
			if ( GetShotsPerBurst(&pSoldier->inv[ ubHandPos ]) > 0 )
			{
				fCapable = TRUE;
			}
		}
	}

	if ( fNotify && !fCapable )
	{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_NOT_BURST_CAPABLE ], pSoldier->name );
	}

	return( fCapable );
}


INT32 CalcMaxTossRange( SOLDIERTYPE * pSoldier, UINT16 usItem, BOOLEAN fArmed )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange");
	INT32 iRange;
	UINT16	usSubItem;

	if ( EXPLOSIVE_GUN( usItem ) )
	{
		// oops! return value in weapons table
		return( Weapon[ usItem ].usRange / CELL_X_SIZE );
	}

	// if item's fired mechanically
	// ATE: If we are sent in a LAUNCHABLE, get the LAUCNHER, and sub ONLY if we are armed...
	usSubItem = GetLauncherFromLaunchable( usItem );

	if ( fArmed && usSubItem != NOTHING )
	{
		usItem = usSubItem;
	}

	if ( Item[ usItem ].usItemClass == IC_LAUNCHER && fArmed )
	{
		// this function returns range in tiles so, stupidly, we have to divide by 10 here
		iRange = Weapon[usItem].usRange / CELL_X_SIZE;
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
			INT32 iThrowingStrength = ( EffectiveStrength( pSoldier ) * 2 + 100 ) / 3;
			iRange = 2 + ( iThrowingStrength / (3 + (Item[usItem].ubWeight) / 3 ));
		}
		else
		{	// not as aerodynamic!
			// start with the range based on the soldier's strength and the item's weight
			iRange = 2 + ( ( EffectiveStrength( pSoldier ) / ( 5 + Item[usItem].ubWeight) ) );
		}
	
		// adjust for thrower's remaining breath (lose up to 1/2 of range)
		iRange -= (iRange * (100 - pSoldier->bBreath)) / 200;

		if ( HAS_SKILL_TRAIT( pSoldier, THROWING ) )
		{
			// better max range due to expertise
			iRange = iRange * (100 + gbSkillTraitBonus[THROWING] * NUM_SKILL_TRAITS( pSoldier, THROWING ) ) / 100;	
		}
	}

	if (iRange < 1)
	{
		iRange = 1;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"calcmaxtossrange done");
	return( iRange );
}


UINT32 CalcThrownChanceToHit(SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubAimTime, UINT8 ubAimPos )
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
		iChance = ( EffectiveDexterity( pSoldier ) + EffectiveMarksmanship( pSoldier ) ) / 2;
		// throwing trait helps out
		if ( HAS_SKILL_TRAIT( pSoldier, THROWING ) )
		{
			iChance += gbSkillTraitBonus[THROWING] * NUM_SKILL_TRAITS( pSoldier, THROWING );
		}
	}
	else
	{

		// MECHANICALLY FIRED arced projectile (ie. mortar), need brains & know-how
		iChance = ( EffectiveDexterity( pSoldier ) + EffectiveMarksmanship( pSoldier ) + EffectiveWisdom( pSoldier ) + pSoldier->bExpLevel ) / 4;

		// heavy weapons trait helps out
		if (HAS_SKILL_TRAIT( pSoldier, HEAVY_WEAPS ))
		{
			iChance += gbSkillTraitBonus[HEAVY_WEAPS] * NUM_SKILL_TRAITS( pSoldier, HEAVY_WEAPS );
		}
	}

	if ( pSoldier->bDoBurst )
	{
		int iPenalty = ( Weapon[usHandItem].ubBurstPenalty
			- GetBurstToHitBonus(&pSoldier->inv[HANDPOS], gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE) )
			* (pSoldier->bDoBurst - 1);

		// halve the penalty for people with the autofire trait
		if ( HAS_SKILL_TRAIT( pSoldier, AUTO_WEAPS ) )
		{
			iPenalty /= 2 * NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS );
		}
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
	if ( pSoldier->uiStatusFlags & SOLDIER_GASSED )
	{
		iChance -= AIM_PENALTY_GASSED;
	}

	// if shooter is being bandaged at the same time, his concentration is off
	if (pSoldier->ubServiceCount > 0)
	{
	  iChance -= AIM_PENALTY_GETTINGAID;
	}

	// if shooter is still in shock
	if (pSoldier->bShock)
	{
		iChance -= (pSoldier->bShock * AIM_PENALTY_PER_SHOCK);
	}

	// calculate actual range (in world units)
	iRange = (INT16)GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

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
	if ((iChance > 0) && (pSoldier->bLife < pSoldier->bLifeMax))
	{
		// if bandaged, give 1/2 of the bandaged life points back into equation
		bBandaged = pSoldier->bLifeMax - pSoldier->bLife - pSoldier->bBleeding;

		// injury penalty is based on % damage taken (max 2/3rds iChance)
		bPenalty = (2 * iChance * (pSoldier->bLifeMax - pSoldier->bLife + (bBandaged / 2))) /
			 (3 * pSoldier->bLifeMax);

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
		iChance -= (bPenalty * (100 - ( EffectiveDexterity( pSoldier ) - 10))) / 100;
	}

	// if iChance exists, but it's a mechanical item being used
	if ((iChance > 0) && (Item[ usHandItem ].usItemClass == IC_LAUNCHER ))
		// reduce iChance to hit DIRECTLY by the item's working condition
		iChance = (iChance * WEAPON_STATUS_MOD(pSoldier->inv[HANDPOS].bStatus[0])) / 100;

	// MAKE SURE CHANCE TO HIT IS WITHIN DEFINED LIMITS
	if (iChance < MINCHANCETOHIT)
		iChance = MINCHANCETOHIT;
	else
	{
		if (iChance > MAXCHANCETOHIT)
			iChance = MAXCHANCETOHIT;
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
		}
	}
	while(IsGunWeaponModeCapable( pSoldier, HANDPOS, pSoldier->bWeaponMode ) == FALSE && pSoldier->bWeaponMode != WM_NORMAL);
	
	if (pSoldier->bWeaponMode == WM_AUTOFIRE || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )
	{
		pSoldier->bDoAutofire = 1;
		pSoldier->bDoBurst = 1;
	}
	else if(pSoldier->bWeaponMode == WM_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST )
	{
		pSoldier->bDoAutofire = 0;
		pSoldier->bDoBurst = 1;
	}
	else
	{
		pSoldier->bDoBurst = 0;
		pSoldier->bDoAutofire = 0;	
	}

	pSoldier->fDoSpread = 0;
	
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
				if ( pSoldier->bLife >= OKLIFE && pSoldier->sGridNo != NOWHERE && pSoldier->bActive && pSoldier->bInSector )
				{
					// Get Pyth spaces away....
					if ( GetRangeInCellCoordsFromGridNoDiff( pQueenSoldier->sGridNo, pSoldier->sGridNo ) <= Weapon[ CREATURE_QUEEN_TENTACLES].usRange )
					{
						// get direction
						bDir = (INT8)GetDirectionFromGridNo( pSoldier->sGridNo, pQueenSoldier );

						// 
						for ( cnt2 = 0; cnt2 < 2; cnt2++ )
						{
							if ( bValidDishoutDirs[ pQueenSoldier->uiPendingActionData1 ][ cnt2 ] == bDir )
							{
								iChance = CalcChanceToStab( pQueenSoldier, pSoldier, 0 );

								// CC: Look here for chance to hit, damage, etc...
								// May want to not hit if target is prone, etc....
								iHitBy = iChance - (INT32) PreRandom( 100 );
								if ( iHitBy > 0 )
								{
									// Hit!
									iImpact = HTHImpact( pQueenSoldier, pSoldier, iHitBy, TRUE );
			 						EVENT_SoldierGotHit( pSoldier, CREATURE_QUEEN_TENTACLES, (INT16) iImpact, (INT16) iImpact, gOppositeDirection[ bDir ], 50, pQueenSoldier->ubID, 0, ANIM_CROUCH, 0, 0 );
								}
							}
						}
					}
				}
			}
		}
	}

	pQueenSoldier->uiPendingActionData1++;
}


BOOLEAN WillExplosiveWeaponFail( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{
  if ( pSoldier->bTeam == gbPlayerNum || pSoldier->bVisible == 1 )
  {
    if ( (INT8)(PreRandom( 40 ) + PreRandom( 40 ) ) > pObj->bStatus[0] )
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
	if ( Item[itemIndex].usItemClass == IC_GUN || Item[itemIndex].usItemClass == IC_PUNCH  || Item[itemIndex].usItemClass == IC_BLADE   || 
		Item[itemIndex].usItemClass == IC_LAUNCHER  || Item[itemIndex].usItemClass == IC_THROWING_KNIFE  || Item[itemIndex].usItemClass == IC_THROWN  ||
		Item[itemIndex].usItemClass == IC_GRENADE  || Item[itemIndex].usItemClass == IC_TENTACLES )
		return true;

	return false;
}

UINT8 GetDamage ( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetDamage"));

	if ( Item[pObj->usItem].usItemClass == IC_BLADE || Item[pObj->usItem].usItemClass == IC_PUNCH || Item[pObj->usItem].usItemClass == IC_TENTACLES )
	{
		UINT8 ubDamage = Weapon[ pObj->usItem ].ubImpact + GetMeleeDamageBonus(pObj);
		return min(255, (UINT8)( (ubDamage) + ( (double)ubDamage / 100) * gGameExternalOptions.ubMeleeDamageMultiplier ) );
	}
	else
	{
		UINT8 ubDamage = Weapon[ pObj->usItem ].ubImpact + GetDamageBonus(pObj);
		return min(255, (UINT8)( (ubDamage) + ( (double)ubDamage / 100) * gGameExternalOptions.ubGunDamageMultiplier ) );
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
UINT8 GetAutofireShotsPerFiveAPs( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetAutofireShotsPerFiveAPs"));

	return Weapon[ pObj->usItem ].bAutofireShotsPerFiveAP;

}
UINT8 GetMagSize( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetMagSize"));

	return Weapon[ pObj->usItem ].ubMagSize + GetMagSizeBonus(pObj);

}
BOOLEAN WeaponReady(SOLDIERTYPE * pSoldier)
{
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FIREREADY )
		return TRUE;
	else
		return FALSE;

}
INT8 GetAPsToReload( OBJECTTYPE *pObj )
{
//	 DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GetAPsToReload"));

	return ( Weapon[ pObj->usItem ].APsToReload *
		( 100 - GetPercentReloadTimeAPReduction(pObj) ) ) / 100;

}

