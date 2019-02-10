#ifndef __SOLDER_CONTROL_H
#define __SOLDER_CONTROL_H

//dnl ch33 200909
// In the future MAXPATROLDGRIDS could be externalized but his value must always be >= OLD_MAXPATROLGRIDS
#define OLD_MAXPATROLGRIDS	10
#define MAXPATROLGRIDS		OLD_MAXPATROLGRIDS

// WANNE: Yes I know, we support up to 254 profiles, but because of compatibility, profile Id = 200
// is not a valid profil. We in MercProfiles.xml, the profile id = 200 should not be used!
#define	NO_PROFILE			200

#include "Animation Cache.h"
#include "Timer Control.h"
#include "vobject.h"
#include "Overhead Types.h"
#include "Item Types.h"
#include "worlddef.h"
#include <vector>
#include <iterator>
#include "GameSettings.h"	// added by Flugente
#include "Disease.h"		// added by Flugente

#define PTR_CIVILIAN	(pSoldier->bTeam == CIV_TEAM)
#define PTR_CROUCHED	(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH)
#define PTR_STANDING	(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND)
#define PTR_PRONE	 (gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_PRONE)

#define DRUG_TYPE_MAX	32
#define FOOD_TYPE_MAX	500

// TEMP VALUES FOR NAMES
#define MAXCIVLASTNAMES		30
extern UINT16 CivLastNames[MAXCIVLASTNAMES][10];

#ifdef JA2UB
//ja25ub
#define	NUM_ASSIST_SLOTS				156				//used for when the player asssists in killing the enemty
#endif

// ANDREW: these are defines for OKDestanation usage - please move to approprite file
#define IGNOREPEOPLE	0
#define PEOPLETOO		1
#define ALLPEOPLE		2
#define FALLINGTEST	 3

#define	LOCKED_NO_NEWGRIDNO			2

#define	BATTLE_SND_LOWER_VOLUME		1

#define	TAKE_DAMAGE_GUNFIRE				1
#define	TAKE_DAMAGE_BLADE					2
#define	TAKE_DAMAGE_HANDTOHAND		3
#define TAKE_DAMAGE_FALLROOF			4
#define TAKE_DAMAGE_BLOODLOSS			5
#define TAKE_DAMAGE_EXPLOSION			6
#define TAKE_DAMAGE_ELECTRICITY		7
#define TAKE_DAMAGE_GAS						8
#define TAKE_DAMAGE_TENTACLES			9
#define TAKE_DAMAGE_STRUCTURE_EXPLOSION 10
#define TAKE_DAMAGE_OBJECT		11
#define TAKE_DAMAGE_VEHICLE_TRAUMA		12


#define SOLDIER_UNBLIT_SIZE			(75*75*2)

#define	SOLDIER_IS_TACTICALLY_VALID					0x00000001
#define SOLDIER_SHOULD_BE_TACTICALLY_VALID	0x00000002
#define SOLDIER_MULTI_SELECTED							0x00000004
#define SOLDIER_PC													0x00000008
#define SOLDIER_ATTACK_NOTICED							0x00000010
#define SOLDIER_PCUNDERAICONTROL						0x00000020
#define SOLDIER_UNDERAICONTROL							0x00000040
#define SOLDIER_DEAD												0x00000080
#define SOLDIER_GREEN_RAY										0x00000100
#define SOLDIER_LOOKFOR_ITEMS								0x00000200
#define SOLDIER_ENEMY												0x00000400
#define SOLDIER_ENGAGEDINACTION							0x00000800
#define SOLDIER_ROBOT												0x00001000
#define SOLDIER_MONSTER											0x00002000
#define SOLDIER_ANIMAL											0x00004000
#define SOLDIER_VEHICLE											0x00008000
#define SOLDIER_MULTITILE_NZ								0x00010000
#define SOLDIER_Z								0x00010000
#define SOLDIER_MULTITILE_Z									0x00020000
#define SOLDIER_MULTITILE										( SOLDIER_MULTITILE_Z | SOLDIER_MULTITILE_NZ )
#define SOLDIER_RECHECKLIGHT								0x00040000
#define SOLDIER_TURNINGFROMHIT							0x00080000
#define SOLDIER_BOXER												0x00100000
#define SOLDIER_LOCKPENDINGACTIONCOUNTER		0x00200000
#define SOLDIER_COWERING										0x00400000
#define SOLDIER_MUTE												0x00800000
#define SOLDIER_GASSED											0x01000000
#define SOLDIER_OFF_MAP											0x02000000
#define SOLDIER_PAUSEANIMOVE								0x04000000
#define SOLDIER_DRIVER											0x08000000
#define SOLDIER_PASSENGER										0x10000000
#define SOLDIER_NPC_DOING_PUNCH							0x20000000
#define SOLDIER_NPC_SHOOTING								0x40000000
#define SOLDIER_LOOK_NEXT_TURNSOLDIER				0x80000000


/*
#define	SOLDIER_TRAIT_LOCKPICKING		0x0001
#define	SOLDIER_TRAIT_HANDTOHAND		0x0002
#define	SOLDIER_TRAIT_ELECTRONICS		0x0004
#define	SOLDIER_TRAIT_NIGHTOPS			0x0008
#define	SOLDIER_TRAIT_THROWING			0x0010
#define	SOLDIER_TRAIT_TEACHING			0x0020
#define	SOLDIER_TRAIT_HEAVY_WEAPS		0x0040
#define	SOLDIER_TRAIT_AUTO_WEAPS		0x0080
#define	SOLDIER_TRAIT_STEALTHY			0x0100
#define	SOLDIER_TRAIT_AMBIDEXT			0x0200
#define	SOLDIER_TRAIT_THIEF					0x0400
#define	SOLDIER_TRAIT_MARTIALARTS		0x0800
#define	SOLDIER_TRAIT_KNIFING				0x1000
*/
// SANDRO was here, messed this..
//#define HAS_SKILL_TRAIT( s, t ) (s->stats.ubSkillTrait1 == t || s->stats.ubSkillTrait2 == t)
//#define NUM_SKILL_TRAITS( s, t ) ( (s->stats.ubSkillTrait1 == t) ? ( (s->stats.ubSkillTrait2 == t) ? 2 : 1 ) : ( (s->stats.ubSkillTrait2 == t) ? 1 : 0 ) )
BOOLEAN HAS_SKILL_TRAIT( SOLDIERTYPE * pSoldier, UINT8 uiSkillTraitNumber );
INT8 NUM_SKILL_TRAITS( SOLDIERTYPE * pSoldier, UINT8 uiSkillTraitNumber );

#define	SOLDIER_QUOTE_SAID_IN_SHIT										0x0001
#define	SOLDIER_QUOTE_SAID_LOW_BREATH									0x0002
#define	SOLDIER_QUOTE_SAID_BEING_PUMMELED							0x0004
#define	SOLDIER_QUOTE_SAID_NEED_SLEEP									0x0008
#define	SOLDIER_QUOTE_SAID_LOW_MORAL									0x0010
#define	SOLDIER_QUOTE_SAID_MULTIPLE_CREATURES					0x0020
#define SOLDIER_QUOTE_SAID_ANNOYING_MERC							0x0040
#define SOLDIER_QUOTE_SAID_LIKESGUN										0x0080
#define SOLDIER_QUOTE_SAID_DROWNING										0x0100
#define SOLDIER_QUOTE_SAID_ROTTINGCORPSE							0x0200
#define SOLDIER_QUOTE_SAID_SPOTTING_CREATURE_ATTACK		0x0400
#define SOLDIER_QUOTE_SAID_SMELLED_CREATURE						0x0800
#define SOLDIER_QUOTE_SAID_ANTICIPATING_DANGER				0x1000
#define SOLDIER_QUOTE_SAID_WORRIED_ABOUT_CREATURES		0x2000
#define SOLDIER_QUOTE_SAID_PERSONALITY								0x4000
#define SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE				0x8000

#define SOLDIER_QUOTE_SAID_EXT_HEARD_SOMETHING				0x0001
#define SOLDIER_QUOTE_SAID_EXT_SEEN_CREATURE_ATTACK		0x0002
#define SOLDIER_QUOTE_SAID_EXT_USED_BATTLESOUND_HIT		0x0004
#define SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL							0x0008

//Ja25: no mike
#ifdef JA2UB
#define SOLDIER_QUOTE_SAID_EXT_MORRIS									0x0010 //Ja25 UB
#else
#define SOLDIER_QUOTE_SAID_EXT_MIKE										0x0010
#endif

#define SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT						0x0020
#define SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED					0x0040
#define SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED					0x0080
#define SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED					0x0100
#define SOLDIER_QUOTE_SAID_BUDDY_4_WITNESSED					0x0400
#define SOLDIER_QUOTE_SAID_BUDDY_5_WITNESSED					0x0800
#define SOLDIER_QUOTE_SAID_BUDDY_6_WITNESSED					0x1000

#ifdef JA2UB
#define	SOLDIER_QUOTE_SAID_THOUGHT_KILLED_YOU					0x0200
#endif


#define	SOLDIER_CONTRACT_RENEW_QUOTE_NOT_USED					0
#define	SOLDIER_CONTRACT_RENEW_QUOTE_89_USED					1
#define	SOLDIER_CONTRACT_RENEW_QUOTE_115_USED					2


#define SOLDIER_MISC_HEARD_GUNSHOT										0x01
// make sure soldiers (esp tanks) are not hurt multiple times by explosions
#define SOLDIER_MISC_HURT_BY_EXPLOSION								0x02
// should be revealed due to xrays
#define SOLDIER_MISC_XRAYED														0x04

#define MAXBLOOD										40
#define NOBLOOD											MAXBLOOD
#define BLOODTIME										5
#define FOOTPRINTTIME								2
#define MIN_BLEEDING_THRESHOLD			12		// you're OK while <4 Yellow life bars

#define BANDAGED( s ) (s->stats.bLifeMax - s->stats.bLife - s->bBleeding)

// amount of time a stats is to be displayed differently, due to change
#define CHANGE_STAT_RECENTLY_DURATION		60000

// MACROS
// #######################################################

#define NO_PENDING_ACTION			255
#define NO_PENDING_ANIMATION	32001
#define NO_PENDING_DIRECTION	253
#define NO_PENDING_STANCE			254
#define NO_DESIRED_HEIGHT			255

#define MAX_FULLTILE_DIRECTIONS 3

//DIGICRAB: Burst UnCap
//move the size of the spread target buffer to a define
//so that we can increase it if we can stand breaking the saves
#define MAX_BURST_SPREAD_TARGETS 6

#define		TURNING_FROM_PRONE_OFF						0
#define		TURNING_FROM_PRONE_ON						1	
#define		TURNING_FROM_PRONE_START_UP_FROM_MOVE		2
#define		TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE		3
#define		TURNING_FROM_PRONE_FOR_PUNCH_OR_STAB		4//dnl ch73 290913

//ENUMERATIONS FOR ACTIONS
enum
{
	MERC_OPENDOOR,
	MERC_OPENSTRUCT,
	MERC_PICKUPITEM,
	MERC_PUNCH,
	MERC_KNIFEATTACK,
	MERC_GIVEAID,
	MERC_GIVEITEM,
	MERC_WAITFOROTHERSTOTRIGGER,
	MERC_CUTFFENCE,
	MERC_DROPBOMB,
	MERC_STEAL,
	MERC_TALK,
	MERC_ENTER_VEHICLE,
	MERC_REPAIR,
	MERC_RELOADROBOT,
	MERC_TAKEBLOOD,
	MERC_ATTACH_CAN,
	MERC_FUEL_VEHICLE,
	MERC_BUILD_FORTIFICATION,
	MERC_HANDCUFF_PERSON,
	MERC_APPLYITEM,
	MERC_INTERACTIVEACTION,
	MERC_FILLBLOODBAG,
};

// ENUMERATIONS FOR THROW ACTIONS
enum
{
	NO_THROW_ACTION,
	THROW_ARM_ITEM,
	THROW_TARGET_MERC_CATCH,
};

// An enumeration for playing battle sounds
enum
{
	BATTLE_SOUND_OK1,
	BATTLE_SOUND_COOL1,
	BATTLE_SOUND_CURSE1,
	BATTLE_SOUND_HIT1,
	BATTLE_SOUND_LAUGH1,
	BATTLE_SOUND_ATTN1,
	BATTLE_SOUND_DIE1,
	BATTLE_SOUND_HUMM,
	BATTLE_SOUND_NOTHING,
	BATTLE_SOUND_GOTIT,
	BATTLE_SOUND_LOWMARALE_OK1,
	BATTLE_SOUND_LOWMARALE_ATTN1,
	BATTLE_SOUND_LOCKED,
	BATTLE_SOUND_ENEMY,
	NUM_MERC_BATTLE_SOUNDS
};


//different kinds of merc
enum
{
	MERC_TYPE__PLAYER_CHARACTER,
	MERC_TYPE__AIM_MERC,
	MERC_TYPE__MERC,
	MERC_TYPE__NPC,
	MERC_TYPE__EPC,
	MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT,
	MERC_TYPE__VEHICLE,
};

// SANDRO - added for healing damaged stats
enum
{
	DAMAGED_STAT_HEALTH,
	DAMAGED_STAT_DEXTERITY,
	DAMAGED_STAT_AGILITY,
	DAMAGED_STAT_STRENGTH,
	DAMAGED_STAT_WISDOM,
	DAMAGED_STAT_LEADERSHIP,
	DAMAGED_STAT_MARKSMANSHIP,
	DAMAGED_STAT_MECHANICAL,
	DAMAGED_STAT_EXPLOSIVES,
	DAMAGED_STAT_MEDICAL,
	NUM_DAMAGABLE_STATS,
};

// SANDRO - this is for determining what stance to go back after being hit
enum
{
	NO_SPEC_STANCE_AFTER_HIT,
	GO_TO_AIM_AFTER_HIT,
	GO_TO_ALTERNATIVE_AIM_AFTER_HIT,
	GO_TO_HTH_BREATH_AFTER_HIT,
	GO_TO_COWERING_AFTER_HIT,
};

// vehicle/human path structure
struct path
{
	UINT32 uiSectorId;
	UINT32 uiEta;
	BOOLEAN fSpeed;
	struct path *pNext;
	struct path *pPrev;
};



typedef struct path PathSt;
typedef PathSt *PathStPtr;

//used for color codes, but also shows the enemy type for debugging purposes
enum
{
	SOLDIER_CLASS_NONE,
	SOLDIER_CLASS_ADMINISTRATOR,
	SOLDIER_CLASS_ELITE,
	SOLDIER_CLASS_ARMY,
	SOLDIER_CLASS_GREEN_MILITIA,
	SOLDIER_CLASS_REG_MILITIA,
	SOLDIER_CLASS_ELITE_MILITIA,
	SOLDIER_CLASS_CREATURE,
	SOLDIER_CLASS_MINER,
	SOLDIER_CLASS_ZOMBIE,
	SOLDIER_CLASS_TANK,
	SOLDIER_CLASS_JEEP,
	SOLDIER_CLASS_BANDIT,
	SOLDIER_CLASS_MAX,
};

// Flugente: there are now separate gun choices, depending on a soldier's class
#define SOLDIER_GUN_CHOICE_SELECTIONS SOLDIER_CLASS_CREATURE

#define SOLDIER_CLASS_ENEMY( bSoldierClass )		( ( bSoldierClass >= SOLDIER_CLASS_ADMINISTRATOR ) && ( bSoldierClass <= SOLDIER_CLASS_ARMY ) )
#define SOLDIER_CLASS_MILITIA( bSoldierClass )	( ( bSoldierClass >= SOLDIER_CLASS_GREEN_MILITIA ) && ( bSoldierClass <= SOLDIER_CLASS_ELITE_MILITIA ) )

// Types of uniforms available
enum
{
	UNIFORM_ENEMY_ADMIN = 0,
	UNIFORM_ENEMY_TROOP,
	UNIFORM_ENEMY_ELITE,
	UNIFORM_MILITIA_ROOKIE,
	UNIFORM_MILITIA_REGULAR,
	UNIFORM_MILITIA_ELITE,
	NUM_UNIFORMS,
};

// -------- added by Flugente: various flags for soldiers --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define SOLDIER_DRUGGED						0x00000001	//1			// Soldier is on (non-alcoholic) drugs
#define SOLDIER_NO_AP						0x00000002	//2			// Soldier has no APs this turn (fix for reinforcement bug)
#define SOLDIER_COVERT_CIV					0x00000004	//4			// Soldier is currently disguised as a civilian
#define SOLDIER_COVERT_SOLDIER				0x00000008	//8			// Soldier is currently disguised as an enemy soldier

#define SOLDIER_DAMAGED_VEST				0x00000010	//16		// Soldier's vest is damaged (and thus can't be taken off)
#define SOLDIER_COVERT_NPC_SPECIAL			0x00000020	//32		// Special flag for NPCs when recruited (used for covert stuff)
#define SOLDIER_NEW_VEST   					0x00000040	//64		// Soldier is wearing new vest. if having both vest and pants, he can disguise
#define SOLDIER_NEW_PANTS					0x00000080	//128		// Soldier is wearing new pants

#define SOLDIER_DAMAGED_PANTS				0x00000100	//256		// Soldier's vest is damaged (and thus can't be taken off)
#define SOLDIER_HEADSHOT					0x00000200	//512		// last hit received was a headshot (attack to the head, so knifes/punches also work)
#define SOLDIER_POW							0x00000400	//1024		// we are a prisoner of war
#define SOLDIER_ASSASSIN					0x00000800	//2048		// we are an enemy assassin, and thus we will behave very different from normal enemies (not set on Kingpin's assassins intentionally)

#define SOLDIER_POW_PRISON					0x00001000	//4096		// this guy is a prisoner of war in a prison sector. SOLDIER_POW refers to people we capture, this refers to people we hold captive
#define SOLDIER_EQUIPMENT_DROPPED			0x00002000	//8192		// under certain circumstances, militia can be ordered to drop their gear twice. Thus we set a marker to avoid that.
#define SOLDIER_ACCESSTEAMMEMBER			0x00004000	//16384		// this merc is accessing another team member'S inventory (via abusing the stealing mechanic)
#define SOLDIER_REDOFLASHLIGHT				0x00008000	//32768		// this flag signifies that we somehow interacted with the items in our hands. Thus we have to possible redo lighting from flashlights

#define SOLDIER_LIGHT_OWNER					0x00010000	//65536		// we 'own' at least one light source (via flashlights)
#define SOLDIER_AIRDROP_TURN				0x00020000	//131072	// we are entering a sector via airdrop this turn
#define SOLDIER_ASSAULT_BONUS				0x00040000	//262144	// backgrounds: our first turn in an assault
#define SOLDIER_RADIO_OPERATOR_LISTENING	0x00080000	//524288	// radio operator is listening with his set

#define SOLDIER_RADIO_OPERATOR_JAMMING		0x00100000	//1048576	// radio operator is jamming frequencies
#define SOLDIER_RADIO_OPERATOR_SCANNING		0x00200000	//2097152	// radio operator is scanning for jammers
#define SOLDIER_AIRDROP						0x00400000	//4194304	// soldier is entering the sector via airdrop from a helicopter. Slightly different from SOLDIER_AIRDROP_TURN
#define SOLDIER_FRESHWOUND					0x00800000	//8388608	// campaign stats: soldier was wounded in this battle

#define SOLDIER_BATTLE_PARTICIPATION		0x01000000	//16777216	// campaign stats: soldier took part in this battle
#define SOLDIER_RAISED_REDALERT				0x02000000	//33554432	// this (AI) soldier has raised red alert. Don't allow him to do so again this turn - either it already worked, or the signal is blocked
#define SOLDIER_ENEMY_OFFICER				0x04000000	//67108864	// soldier is an enemy officer
#define SOLDIER_ENEMY_OBSERVEDTHISTURN		0x08000000	//134217728 // enemy soldier was seen by the player this turn

#define SOLDIER_VIP							0x10000000	//268435456	// soldier is a VIP - the player will likely try to assassinate him
#define SOLDIER_BODYGUARD					0x20000000	//536870912 // soldier is a bodyguard for a VIP
#define SOLDIER_COVERT_TEMPORARY_OVERT		0x40000000	//1073741824	// we are covert, but just performed a obviously suspicious task. For a short time, we can be uncovered more easily
#define SOLDIER_MOVEITEM_RESTRICTED			0x80000000	//2147483648	// when moving item, this soldier will not pick up equipment the militia might use
// ----------------------------------------------------------------

// ------------------- more flags for soldiers --------------------
#define SOLDIER_SNITCHING_OFF				0x00000001	//1				// isn't allowed to snitch
#define SOLDIER_PREVENT_MISBEHAVIOUR_OFF	0x00000002	//2				// isn't allowed to prevent misbehaviour
#define SOLDIER_RAM_THROUGH_OBSTACLES		0x00000004	//4				// vehicle
#define SOLDIER_INTERROGATE_ADMIN			0x00000008	//8				// interrogate admins. Flags might not be the best solution, but I won't add an extra variable for this

#define SOLDIER_INTERROGATE_TROOP			0x00000010	//16			// interrogate troops
#define SOLDIER_INTERROGATE_ELITE			0x00000020	//32			// interrogate elites
#define SOLDIER_INTERROGATE_OFFICER			0x00000040	//64			// interrogate officers
#define SOLDIER_INTERROGATE_GENERAL			0x00000080	//128			// interrogate generals

#define SOLDIER_INTERROGATE_CIVILIAN		0x00000100	//256			// interrogate civilian
#define SOLDIER_POTENTIAL_VOLUNTEER			0x00000200	//512			// this civilian _might_ join us as a volunteer if conditions are right
#define SOLDIER_HUNGOVER					0x00000400	//1024			// we drank alcohol recently, and are now hungover
#define SOLDIER_TAKEN_LARGE_HIT				0x00000800					// we recently received a lot of damage in a single hit

#define SOLDIER_COVERT_NOREDISGUISE			0x00001000					// this soldier does not want to be redisguised
#define SOLDIER_TRAIT_FOCUS					0x00002000					// 'focus' skill is active
#define SOLDIER_BAYONET_RUNBONUS			0x00004000					// we are performing a bayonet attack after transitioning from running, giving our attack extra force
#define SOLDIER_CONCEALINSERTION			0x00008000					// we enteri a sector by transition from concealed state (which causes us to spawn at the location we left the sector in)

#define SOLDIER_CONCEALINSERTION_DISCOVERED	0x00010000					// we enter a sector by transition from concealed state, but as we were 'discovered', set red alert
#define SOLDIER_MERC_POW_LOCATIONKNOWN		0x00020000					// we are a POW, but the player has discovered our location
#define SOLDIER_SURGERY_BOOSTED				0x00040000					// we are a boosted performing surgery (e.g. by using up a blood bag)

#define SOLDIER_INTERROGATE_ALL				0x000001F8					// all interrogation flags
// ----------------------------------------------------------------

// -------- added by Flugente: disease property flags --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// these flags describe how a disease has affected us so far
#define SOLDIERDISEASE_DIAGNOSED			0x00000001	//1				// it is now known that we have this disease - either a doctor diagnosed it, or it broke out an we are currently suffering
#define SOLDIERDISEASE_OUTBREAK				0x00000002	//2				// disease has broken out - we suffer the effects now. Without this flag, it is active but does not do any damage to us
#define SOLDIERDISEASE_REVERSEAL			0x00000004	//4				// disease is reversing - every hour we receive negative points. This is used to simulate a disease healing itself



// -------- added by Flugente: background property flags --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// a merc's background info reveals data about his previous life, like former regiments. These backgrounds add small abilities/disabilities. Nothing substantial, just small bits do
// diversify your mercs and add more personality
#define BACKGROUND_DRUGUSE						0x0000000000000001	//1				// might use drugs on his own (the 'Larry'-effect)
#define BACKGROUND_XENOPHOBIC					0x0000000000000002	//2				// arrogant towards others without this background
#define BACKGROUND_EXP_UNDERGROUND				0x0000000000000004	//4				// extra level in underground sectors
#define BACKGROUND_SCROUNGING					0x0000000000000008	//8				// might pick up valuable items on his own

#define BACKGROUND_TRAPLEVEL					0x0000000000000010	//16			// trap level +1
#define BACKGROUND_CORRUPTIONSPREAD				0x0000000000000020	//32			// spreads corruption to others	- not used in trunk!
#define BACKGROUND_NO_MALE   					0x0000000000000040	//64			// background cannot be selected by males (IMP creation)
#define BACKGROUND_NO_FEMALE					0x0000000000000080	//128			// background cannot be selected by females (IMP creation)

#define BACKGROUND_GLOBALOYALITYLOSSONDEATH		0x0000000000000100	//256			// if character dies, huge loyalty loss in entire country

#define BACKGROUND_FLAG_MAX	9					// number of flagged backgrounds - keep this updated, or properties will get lost!

// some properties are hidden (forbid background in MP creation)
// corruption property is not relevant in 1.13
#define BACKGROUND_HIDDEN_FLAGS					(BACKGROUND_NO_MALE|BACKGROUND_NO_FEMALE|BACKGROUND_CORRUPTIONSPREAD)

// anv: externalised taunts
// taunt properties
// attitudes
#define TAUNT_A_CUNNING_SOLO						0x0000000000000001	//1
#define TAUNT_A_CUNNING_AID							0x0000000000000002	//2
#define TAUNT_A_BRAVE_SOLO							0x0000000000000004	//4
#define TAUNT_A_BRAVE_AID							0x0000000000000008	//8

#define TAUNT_A_AGGRESSIVE							0x0000000000000010	//16
#define TAUNT_A_DEFENSIVE							0x0000000000000020	//32

// situations
// actions
#define TAUNT_S_FIRE_GUN		   					0x0000000000000040	//64
#define TAUNT_S_FIRE_LAUNCHER						0x0000000000000080	//128
#define TAUNT_S_ATTACK_BLADE						0x0000000000000100	//256
#define TAUNT_S_ATTACK_HTH							0x0000000000000200	//512

#define TAUNT_S_THROW_KNIFE							0x0000000000000400	//1024
#define TAUNT_S_THROW_GRENADE						0x0000000000000800	//2048

#define TAUNT_S_OUT_OF_AMMO							0x0000000000001000	//4096
#define TAUNT_S_RELOAD								0x0000000000002000	//8192

#define TAUNT_S_STEAL								0x0000000000004000	//16384

// AI routines
#define TAUNT_S_CHARGE_BLADE						0x0000000000008000	//32768
#define TAUNT_S_CHARGE_HTH							0x0000000000010000	//65536
#define TAUNT_S_RUN_AWAY							0x0000000000020000	//131072
#define TAUNT_S_SEEK_NOISE							0x0000000000040000	//262144
#define TAUNT_S_ALERT								0x0000000000080000	//...
#define TAUNT_S_SUSPICIOUS							0x0000000000100000
#define TAUNT_S_NOTICED_UNSEEN						0x0000000000200000	//
#define TAUNT_S_SAY_HI								0x0000000000400000	//
#define	TAUNT_S_INFORM_ABOUT						0x0000000000800000

// got_hit_xxx
#define TAUNT_S_GOT_HIT								0x0000000001000000	//
#define TAUNT_S_GOT_HIT_GUNFIRE						0x0000000002000000	//
#define TAUNT_S_GOT_HIT_BLADE						0x0000000004000000	//
#define TAUNT_S_GOT_HIT_HTH							0x0000000008000000	//
#define TAUNT_S_GOT_HIT_FALLROOF					0x0000000010000000	//
#define TAUNT_S_GOT_HIT_BLOODLOSS					0x0000000020000000	//
#define TAUNT_S_GOT_HIT_EXPLOSION					0x0000000040000000	//
#define TAUNT_S_GOT_HIT_GAS							0x0000000080000000	//
#define TAUNT_S_GOT_HIT_TENTACLES					0x0000000100000000	//
#define TAUNT_S_GOT_HIT_STRUCTURE_EXPLOSION			0x0000000200000000	//
#define TAUNT_S_GOT_HIT_OBJECT						0x0000000400000000	//
#define TAUNT_S_GOT_HIT_THROWING_KNIFE				0x0000000800000000	//

#define TAUNT_S_GOT_DEAFENED						0x0000001000000000	//
#define TAUNT_S_GOT_BLINDED							0x0000002000000000	//

#define TAUNT_S_GOT_ROBBED							0x0000004000000000	//

// got_missed_xxx
#define TAUNT_S_GOT_MISSED							0x0000008000000000	//
#define TAUNT_S_GOT_MISSED_GUNFIRE					0x0000010000000000	//
#define TAUNT_S_GOT_MISSED_BLADE					0x0000020000000000	//
#define TAUNT_S_GOT_MISSED_HTH						0x0000040000000000	//
#define TAUNT_S_GOT_MISSED_THROWING_KNIFE			0x0000080000000000	//

// hit_xxx
#define TAUNT_S_HIT									0x0000100000000000	//
#define TAUNT_S_HIT_GUNFIRE							0x0000200000000000	//
#define TAUNT_S_HIT_BLADE							0x0000400000000000	//
#define TAUNT_S_HIT_HTH								0x0000800000000000	//
#define TAUNT_S_HIT_EXPLOSION						0x0001000000000000	//
#define TAUNT_S_HIT_THROWING_KNIFE					0x0002000000000000	//

// kill_xxx
#define TAUNT_S_KILL								0x0004000000000000	//
#define TAUNT_S_KILL_GUNFIRE						0x0008000000000000	//
#define TAUNT_S_KILL_BLADE							0x0010000000000000	//
#define TAUNT_S_KILL_HTH							0x0020000000000000	//
#define TAUNT_S_KILL_THROWING_KNIFE					0x0040000000000000	//
#define TAUNT_S_HEAD_POP							0x0080000000000000	//

// miss_xxx
#define TAUNT_S_MISS								0x0100000000000000	//
#define TAUNT_S_MISS_GUNFIRE						0x0200000000000000	//
#define TAUNT_S_MISS_BLADE							0x0400000000000000	//
#define TAUNT_S_MISS_HTH							0x0800000000000000	//
#define TAUNT_S_MISS_THROWING_KNIFE					0x1000000000000000	//

// NEW FLAGS, starting from the beginning (UINT128 is redundant? yeah, right)

// class
#define TAUNT_C_ADMIN		   						0x0000000000000004	//4
#define TAUNT_C_ARMY		   						0x0000000000000008	//8
#define TAUNT_C_ELITE								0x0000000000000010	//16
#define TAUNT_C_GREEN								0x0000000000000020	//32
#define TAUNT_C_REGULAR		   						0x0000000000000040	//64
#define TAUNT_C_VETERAN								0x0000000000000080	//128

// sex
#define TAUNT_G_MALE								0x0000000000000100	//256
#define TAUNT_G_FEMALE								0x0000000000000200	//512

// target
#define TAUNT_T_MALE								0x0000000000000400	//1024
#define TAUNT_T_FEMALE								0x0000000000000800	//2048

#define TAUNT_T_ZOMBIE								0x0000000000001000	//4096

#define TAUNT_FLAG_1_MAX	64
#define TAUNT_FLAG_2_MAX	13
#define TAUNT_FLAG_MAX	TAUNT_FLAG_1_MAX + TAUNT_FLAG_2_MAX

// Flugente: types of multi-turn actions
enum
{
	MTA_NONE = 0,
	MTA_FORTIFY,
	MTA_REMOVE_FORTIFY,
	MTA_HACK,
	NUM_MTA,
};

//Flugente skills from traits and other sources
enum{
	// first skill
	SKILLS_FIRST = 0,
	
	// radio operator
	SKILLS_RADIO_FIRST = SKILLS_FIRST,
	SKILLS_RADIO_ARTILLERY = SKILLS_RADIO_FIRST,
	SKILLS_RADIO_JAM,
	SKILLS_RADIO_SCAN_FOR_JAM,
	SKILLS_RADIO_LISTEN,
	SKILLS_RADIO_CALLREINFORCEMENTS,
	SKILLS_RADIO_TURNOFF,
	SKILLS_RADIO_LAST = SKILLS_RADIO_TURNOFF,

	// spy
	SKILLS_INTEL_FIRST,
	SKILLS_INTEL_CONCEAL = SKILLS_INTEL_FIRST,	// assignment: spy hides among the population
	SKILLS_INTEL_GATHERINTEL,					// assignment: spy gathers information while disguised
	SKILLS_INTEL_LAST = SKILLS_INTEL_GATHERINTEL,

	// various
	SKILLS_VARIOUS_FIRST,
	SKILLS_SPOTTER = SKILLS_VARIOUS_FIRST,
	SKILLS_FOCUS,
	SKILLS_DRAG,
	SKILLS_VARIOUS_LAST = SKILLS_DRAG,

	SKILLS_MAX,
};

// Flugente: certain skills/traits/taints add effects that require a counter, here are enums for these
enum {
	SOLDIER_COUNTER_RADIO_ARTILLERY,		// there is actually no need for an artillery timer, but we use one to forbid the same AI guy ordering multiple strikes at once
	SOLDIER_COUNTER_SPOTTER,				// used to determine wether we are a spotter
	SOLDIER_COUNTER_ROLE_OBSERVED,			// every turn that the player observes an enemy, the enemies counter is increased. If it is high enough, we know his role
	
	SOLDIER_COUNTER_MAX = 20,				// enough space for fillers
};

// Flugente: certain skills/traits/taints require a cooldown timer 
enum {
	SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_SECONDS = 0,
	SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_APS,
	SOLDIER_COOLDOWN_CRYO,					// counts how many turns character will be frozen
	SOLDIER_COOLDOWN_INTEL_PENALTY,			// after being discovered, we can't gain intel from the assignment for this many hours
	SOLDIER_COOLDOWN_DRUGUSER_COMBAT,		// after a drug user deliberately took drugs, he will not do so on his own for a while

	SOLDIER_COOLDOWN_MAX = 20,				// enough space for fillers
};

// enum of uniform pieces
typedef struct
{
	PaletteRepID vest;
	PaletteRepID pants;
}UNIFORMCOLORS;

// HEADROCK HAM 3.6: Uniform colors for the different soldier classes
extern UNIFORMCOLORS gUniformColors[NUM_UNIFORMS];

// Flugente: a structure for clothing items
typedef struct
{
	UINT16			uiIndex;
	CHAR16			szName[80];				// name of these clothes
	PaletteRepID	vest;
	PaletteRepID	pants;
} CLOTHES_STRUCT;

#define CLOTHES_MAX	50

extern CLOTHES_STRUCT Clothes[CLOTHES_MAX];

// This macro should be used whenever we want to see if someone is neutral
// IF WE ARE CONSIDERING ATTACKING THEM.	Creatures & bloodcats will attack neutrals
// but they can't attack empty vehicles!!
// the_bob: also, creatures won't attack crows, because it seems to confuse the AI and cause freezes
#define CONSIDERED_NEUTRAL( me, them )  (\
										( them->aiData.bNeutral || them->usSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER|SOLDIER_POW) || (them->flags.uiStatusFlags & SOLDIER_PC && them->stats.bLife < OKLIFE) ) \
										&& ( me->bTeam != CREATURE_TEAM || ( (them->flags.uiStatusFlags & SOLDIER_VEHICLE) || (them->ubBodyType == CROW) ) ) \
										&& !( me->flags.uiStatusFlags & SOLDIER_BOXER && them->flags.uiStatusFlags & SOLDIER_BOXER ) \
										)

typedef struct
{
	UINT8			ubKeyID;
	UINT8			ubNumber;
} KEY_ON_RING;

typedef struct
{
	float				dX;
	float				dY;
	float				dZ;
	float				dForceX;
	float				dForceY;
	float				dForceZ;
	float				dLifeSpan;
	UINT8				ubActionCode;
	UINT32			uiActionData;

} THROW_PARAMS;

#define DELAYED_MOVEMENT_FLAG_PATH_THROUGH_PEOPLE 0x01

// reasons for being unable to continue movement
enum
{
	REASON_STOPPED_NO_APS,
	REASON_STOPPED_SIGHT,
};


enum
{
	HIT_BY_TEARGAS = 0x01,
	HIT_BY_MUSTARDGAS = 0x02,
	HIT_BY_CREATUREGAS = 0x04,
	HIT_BY_BURNABLEGAS = 0x08,
	HIT_BY_SMOKEGAS = 0x10,//dnl ch40 200909
};


struct LEVELNODE;

//ADB makes the code clearer, used like "thisSoldier->foo();"
//CHRISL: Not sure if it make the code easier to read or not, but it does make it harder to debug
//#define thisSoldier this

//forward declarations for versioning, it's so long I want it at the end
class OLDSOLDIERTYPE_101;

//ADB inventory needs a little work, for instance, how to get objects and counts to agree on sizes?
//also makes things more bloated when saving
class Inventory {
public:
	// Constructors
	// Create an inventory with a fixed maximum number of slots
	Inventory(int slotCount = NUM_INV_SLOTS);

	// Copy Constructor
	Inventory(const Inventory&);

	// Assignment operator
	Inventory& operator=(const Inventory&);

	// Destructor
	~Inventory();

	// Index operator
	OBJECTTYPE& operator [] (unsigned int idx);

	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion);
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	// Removes all items from the inventory
	void clear();

	// How any slots are there in this inventory?
	unsigned int size() const;

	//temporarily? public
	std::vector<int>			bNewItemCount;
	std::vector<int>			bNewItemCycleCount;
private:
	std::vector<OBJECTTYPE>	inv;
};
	// Added for new inventory system to work
class STRUCT_AIData//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
public:
	// AI STUFF from before the changes to the memory structure
	INT8												bOppList[MAX_NUM_SOLDIERS]; // AI knowledge database
	INT8												bLastAction;
	INT8												bAction;	
	INT32												usActionData;
	INT8												bNextAction;
	INT32												usNextActionData;
	INT8												bActionInProgress;
	INT8												bAlertStatus;
	INT8												bOppCnt;
	INT8												bNeutral;
	INT8												bNewSituation;
	INT8												bNextTargetLevel;
	INT8												bOrders;
	INT8												bAttitude;
	INT8												bUnderFire;
	INT8												bShock;
	INT8												bUnderEscort;
	INT8												bBypassToGreen;
	UINT8												ubLastMercToRadio;
	INT8												bDominantDir;				// AI main direction to face...
	INT8												bPatrolCnt;					// number of patrol gridnos
	INT8												bNextPatrolPnt;			// index to next patrol gridno
	INT32											sPatrolGrid[MAXPATROLGRIDS];// AI list for ptr->orders==PATROL
	INT32												sNoiseGridno;
	UINT8												ubNoiseVolume;
	INT8												bLastAttackHit;
	UINT8												ubXRayedBy;
	FLOAT												dHeightAdjustment;
	INT8												bMorale;
	INT8												bTeamMoraleMod;
	INT8												bTacticalMoraleMod;
	INT8												bStrategicMoraleMod;
	INT8												bAIMorale;
	UINT8												ubPendingAction;
	UINT8												ubPendingActionAnimCount;
	UINT32												uiPendingActionData1;
	INT32												sPendingActionData2;
	INT8												bPendingActionData3;
	INT8												ubDoorHandleCode;
	UINT32												uiPendingActionData4;
	INT8												bInterruptDuelPts;
	INT8												bPassedLastInterrupt;
	INT16												bIntStartAPs;	//100AP
	INT8												bMoved;
	INT8												bHunting;
	UINT8												ubLastCall;
	UINT8												ubCaller;
	INT32												sCallerGridNo;
	UINT8												bCallPriority;
	INT8												bCallActedUpon;
	INT8												bFrenzied;
	INT8												bNormalSmell;
	INT8												bMonsterSmell;
	INT8												bMobility;
	INT8												bRTPCombat;
	INT8												fAIFlags;
	INT16												bAimTime;	//100AP
	INT8												bShownAimTime;
	UINT8												ubInterruptCounter[MAX_NUM_SOLDIERS]; // SANDRO - interrupt counter added
};

class STRUCT_Flags//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);

public:
	// flags from before the changes to the memory structure
	INT8												bHasKeys;			// allows AI controlled dudes to open locked doors
	BOOLEAN											fDelayedMovement;
	BOOLEAN											fTurnInProgress;
	BOOLEAN											fBeginFade;
	INT8											bTurningFromPronePosition;
	BOOLEAN											fDontChargeReadyAPs;
	BOOLEAN											fPrevInWater;
	INT8											bGoBackToAimAfterHit;
	BOOLEAN											fForceRenderColor;
	BOOLEAN											fForceNoRenderPaletteCycle;
	BOOLEAN											fStopPendingNextTile;
	BOOLEAN											fUIMovementFast;
	BOOLEAN											fForceShade;
	BOOLEAN											fDeadSoundPlayed;
	BOOLEAN											fClosePanel;
	BOOLEAN											fClosePanelToDie;
	BOOLEAN											fDeadPanel;
	BOOLEAN											fOpenPanel;
	BOOLEAN											fIntendedTarget; // intentionally shot?
	BOOLEAN											fPauseAllAnimation;
	BOOLEAN											fContinueMoveAfterStanceChange;
	BOOLEAN											fHoldAttackerUntilDone;
	BOOLEAN											fWarnedAboutBleeding;
	BOOLEAN											fDyingComment;
	BOOLEAN											fTurningToShoot;
	BOOLEAN											fTurningToFall;
	BOOLEAN											fTurningUntilDone;
	BOOLEAN											fGettingHit;
	BOOLEAN											fInNonintAnim;
	BOOLEAN											fFlashLocator;
	BOOLEAN											fShowLocator;
	BOOLEAN											fFlashPortrait;
	BOOLEAN											fNoAPToFinishMove;
	BOOLEAN											fPausedMove;
	BOOLEAN											fUIdeadMerc;				// UI Flags for removing a newly dead merc
	BOOLEAN											fUInewMerc;					// UI Flags for adding newly created merc ( panels, etc )
	BOOLEAN											fUICloseMerc;				// UI Flags for closing panels
	BOOLEAN											fUIFirstTimeNOAP;		// UI Flag for diming guys when no APs ( dirty flags )
	BOOLEAN											fUIFirstTimeUNCON;	// UI FLAG For unconscious dirty		
	BOOLEAN											fReloading;
	BOOLEAN											fPauseAim;
	BOOLEAN											fInMissionExitNode;
	BOOLEAN											fBetweenSectors;	//set when the group isn't actually in a sector.
	BOOLEAN											fReactingFromBeingShot;
	BOOLEAN											fCheckForNewlyAddedItems;
	BOOLEAN											fBlockedByAnotherMerc;
	BOOLEAN											fContractPriceHasIncreased;
	BOOLEAN											fFixingSAMSite;
	BOOLEAN											fFixingRobot;
	BOOLEAN											fSignedAnotherContract; 
	BOOLEAN											fDontChargeTurningAPs;
	BOOLEAN											fChangingStanceDueToSuppression;
	BOOLEAN											fForcedToStayAwake;				// forced by player to stay awake, reset to false, the moment they are set to rest or sleep
	BOOLEAN											fDoSpread;
	BOOLEAN											fIsSoldierMoving;							// ie.	Record time is on
	BOOLEAN											fIsSoldierDelayed;						//Is the soldier delayed Soldier 
	BOOLEAN											fSoldierUpdatedFromNetwork;
	BOOLEAN											fSayAmmoQuotePending;
	BOOLEAN											fMuzzleFlash;
	BOOLEAN											fMercCollapsedFlag;
	BOOLEAN											fDoneAssignmentAndNothingToDoFlag;
	BOOLEAN											fMercAsleep;
	BOOLEAN											fDontChargeAPsForStanceChange;
	BOOLEAN											fSoldierWasMoving;
	BOOLEAN											fDontUnsetLastTargetFromTurn;
	BOOLEAN											fUseMoverrideMoveSpeed;
	BOOLEAN											fDieSoundUsed;
	BOOLEAN											fUseLandingZoneForArrival;
 	BOOLEAN											fComplainedThatTired;
	BOOLEAN											fRTInNonintAnim;

	UINT8												fHitByGasFlags;						// flags 
	INT8												fDisplayDamage;
	INT8												fCloseCall;
	INT8												fTryingToFall;
	INT8												fPastXDest;
	INT8												fPastYDest;
 BOOLEAN					 fFallClockwise;
	BOOLEAN					 fDoingExternalDeath;
	BOOLEAN					autofireLastStep;
	BOOLEAN lastFlankLeft;
	UINT32											uiStatusFlags;

	//LBE node stuff
	BOOLEAN			ZipperFlag;
	BOOLEAN			DropPackFlag;

};

class STRUCT_TimeChanges//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
		// time changes...when a stat was changed according to GetJA2Clock();
	UINT32											uiChangeLevelTime;
	UINT32											uiChangeHealthTime;
	UINT32											uiChangeStrengthTime;
	UINT32											uiChangeDexterityTime;
	UINT32											uiChangeAgilityTime;
	UINT32											uiChangeWisdomTime;
	UINT32											uiChangeLeadershipTime;
	UINT32											uiChangeMarksmanshipTime;
	UINT32											uiChangeExplosivesTime;
	UINT32											uiChangeMedicalTime;
	UINT32											uiChangeMechanicalTime;
};

class STRUCT_Drugs//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
	INT8			bFutureDrugEffect[DRUG_TYPE_MAX];						// value to represent effect of a needle
	INT8			bDrugEffectRate[DRUG_TYPE_MAX];							// represents rate of increase and decrease of effect	
	INT8			bDrugEffect[DRUG_TYPE_MAX];								// value that affects AP & morale calc ( -ve is poorly )
	INT8			bDrugSideEffectRate[DRUG_TYPE_MAX];					// duration of negative AP and morale effect
	INT8			bDrugSideEffect[DRUG_TYPE_MAX];							// duration of negative AP and morale effect
	INT8			bTimesDrugUsedSinceSleep[DRUG_TYPE_MAX];			
};

// Flugente: everything drug-related has been redone
enum {
	DRUG_EFFECT_HP = 0,
	DRUG_EFFECT_BP,
	DRUG_EFFECT_AP,
	DRUG_EFFECT_MORALE,
	DRUG_EFFECT_PHYS_RES,
	DRUG_EFFECT_STR,
	DRUG_EFFECT_AGI,
	DRUG_EFFECT_DEX,
	DRUG_EFFECT_WIS,

	DRUG_EFFECT_MAX = 20,
};


struct DRUGS
{
	UINT16			duration[DRUG_EFFECT_MAX];
	INT16			size[DRUG_EFFECT_MAX];

	UINT8			drugpersonality;
	UINT16			drugpersonality_duration;

	UINT8			drugdisability;
	UINT16			drugdisability_duration;

	FLOAT			drinkstaken;			// number of alcoholic drinks we habe in our system, lowered by 1 every hour
};

class STRUCT_TimeCounters//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
	TIMECOUNTER									UpdateCounter;
	TIMECOUNTER									DamageCounter;
	TIMECOUNTER									ReloadCounter;
	TIMECOUNTER									FlashSelCounter;
	TIMECOUNTER									AICounter;
	TIMECOUNTER									FadeCounter;
	TIMECOUNTER									PanelAnimateCounter;
	TIMECOUNTER									BlinkSelCounter;
	TIMECOUNTER									PortraitFlashCounter;
	TIMECOUNTER									NextTileCounter;
};

// forward declaration for modularized tactical ai
namespace AI
{
    namespace tactical
    {
        class Plan;
    }
}

class STRUCT_Statistics//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
	INT8												bExpLevel;		// general experience level
	INT8												bLife;				// current life (hit points or health)
	INT8												bLifeMax;			// maximum life for this merc
	INT8												bStrength;
	INT8												bAgility;			// agility (speed) value
	INT8												bDexterity;		// dexterity (hand coord) value
	INT8												bWisdom;
	INT8												bLeadership;
	INT8												bMarksmanship;
	INT8												bMechanical;
	INT8												bExplosive;
	INT8												bMedical;
	INT8												bScientific;	
	UINT8												ubSkillTraits[30];
	//UINT8												ubSkillTrait2;
	//UINT8												ubSkillTrait3; // added by SANDRO
};

class STRUCT_Pathing//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
	// WALKING STUFF
	INT8												bDesiredDirection;
	INT16												sDestXPos;
	INT16												sDestYPos;
	//INT32												sDesiredDest;//apparently not used
	INT32												sDestination;
	INT32												sFinalDestination;
	INT8												bLevel;
	INT8												bStopped;
	INT8												bNeedToLook;
	// PATH STUFF
	UINT16											usPathingData[ MAX_PATH_LIST_SIZE ];
	UINT16											usPathDataSize;
	UINT16											usPathIndex;	
	INT32												sBlackList;
	INT8												bPathStored;	// good for AI to reduct redundancy
};

class SOLDIERTYPE//last edited at version 102
{
public:
	// Conversion operator
	SOLDIERTYPE& operator=(const OLDSOLDIERTYPE_101&);

	// Constructor
	SOLDIERTYPE();
	// Destructor
	~SOLDIERTYPE();

	BOOLEAN Load(HWFILE hFile);
	BOOLEAN Save(HWFILE hFile);
	UINT32	GetChecksum();

	// Initialize the soldier.	
	//	Use this instead of the old method of calling memset.
	//	Note that the constructor does this automatically.
	void initialize();
	bool	exists();

	// Note: Place all non-POD items at the end (after endOfPOD)
	// The format of this structure affects what is written into and read from various
	// files (maps, save files, etc.).	If you change it then that code will not work 
	// properly until it is all fixed and the files updated.
public:
	// ID
	UINT8												ubID;
	UINT8												bReserved1;
	CHAR16											name[ 10 ];

	INT16	GetMaxDistanceVisible(INT32 sGridNo = -1, INT8 bLevel = -1, int calcAsType = -1);

	// DESCRIPTION / STATS, ETC
	UINT8												ubBodyType;
	INT16												bActionPoints;	//100AP
	INT16												bInitialActionPoints;	//100AP
	INT8												bOldLife;			// life at end of last turn, recorded for monster AI
	INT8												bVisible;			// to render or not to render...
	INT8												bActive;
	INT8												bTeam;				// Team identifier


	OBJECTTYPE									*pTempObject;
	KEY_ON_RING									*pKeyRing;

	UINT8												bInSector;
	INT8												bFlashPortraitFrame;
	INT16												sFractLife;		// fraction of life pts (in hundreths)	
	INT8												bBleeding;		// blood loss control variable
	INT8												bBreath;			// current breath value
	INT8												bBreathMax;	// max breath, affected by fatigue/sleep
	INT8												bStealthMode;
	INT16												sBreathRed;			// current breath loss value
	

	UINT8												ubWaitActionToDo;
	INT8												ubInsertionDirection;
	INT8												bGunType;
	UINT8												ubOppNum;
	INT8												bLastRenderVisibleValue;
	UINT8												ubAttackingHand;
	INT16												sWeightCarriedAtTurnStart;
	
	/////////////////////////////////////////////////////////////////////////////////
	// SANDRO - added following
	// values for surgery feature
	INT32												iHealableInjury; 
	BOOLEAN												fDoingSurgery; 
	// value for unregainable breath feature (for Martial Arts)
	signed long											lUnregainableBreath;
	// this stores possible stats lost due to critical hits
	UINT8												ubCriticalStatDamage[ NUM_DAMAGABLE_STATS ];
	/////////////////////////////////////////////////////////////////////////////////

	//NEW MOVEMENT INFORMATION for Strategic Movement
	UINT8												ubGroupID;		//the movement group the merc is currently part of.
																								//sSectorX and sSectorY will reflect the sector the
																								//merc was at last.

	UINT8												ubMovementNoiseHeard;// 8 flags by direction


	// WORLD POSITION STUFF
	FLOAT											dXPos;
	FLOAT											dYPos;
	// HEADROCK HAM 4: These two vars are appropriated for the new Shooting Mechanism.
	// They represent the soldier's position at the start of his turn.
	//FLOAT											dOldXPos;
	//FLOAT											dOldYPos; 
	INT16											sOldXPos;
	INT16											sOldYPos;
	INT32												sInitialGridNo;
	INT32												sGridNo;
	UINT8												ubDirection;
	INT16												sHeightAdjustment;
	INT16												sDesiredHeight;
	INT32												sTempNewGridNo;					// New grid no for advanced animations
	INT16												sRoomNo;
	INT8												bOverTerrainType;
	INT8												bOldOverTerrainType;

	INT8												bCollapsed;					// collapsed due to being out of APs
	INT8												bBreathCollapsed;					// collapsed due to being out of APs

	
	UINT8												ubDesiredHeight;
	UINT16											usPendingAnimation;
	UINT8												ubPendingStanceChange;
	UINT16											usAnimState;


	

	UINT32											uiAIDelay;
	INT16												sReloadDelay;
	UINT8												ubAttackerID;
	UINT8												ubPreviousAttackerID;

	INT32												sInsertionGridNo;


	AnimationSurfaceCacheType		AnimCache; // will be 9 bytes once changed to pointers

	UINT8												bSide;
	UINT8												bViewRange;
	INT8 												bNewOppCnt;
	INT8												bService;		// first aid, or other time consuming process
	
	UINT16											usAniCode;
	UINT16											usAniFrame;
	INT16											sAniDelay;

	// MOVEMENT TO NEXT TILE HANDLING STUFF
	UINT8												ubDelayedMovementCauseMerc;
	INT32												sDelayedMovementCauseGridNo;
	INT32												sReservedMovementGridNo;


	// Weapon Stuff
	INT32												sTargetGridNo;
	INT8												bTargetLevel;
	INT8												bTargetCubeLevel;
	INT32												sLastTarget;
	// HEADROCK HAM 4: the muzzle offset of the shooter's previous bullet. (NCTH)
	FLOAT												dPrevMuzzleOffsetX[2];
	FLOAT												dPrevMuzzleOffsetY[2];
	// HEADROCK HAM 4: Two more values. These record the shooter's previous Counter Force applied on the gun.
	FLOAT												dPrevCounterForceX[2];
	FLOAT												dPrevCounterForceY[2];
	// CHRISL: Track initial offsets for autofire
	FLOAT												dInitialMuzzleOffsetX;
	FLOAT												dInitialMuzzleOffsetY;

	INT8												bTilesMoved;
	FLOAT												dNextBleed;

	UINT8												ubTilesMovedPerRTBreathUpdate;
	UINT16											usLastMovementAnimPerRTBreathUpdate;
	
	INT16												sLocatorFrame;

	INT32												iFaceIndex;

	// FULL 3-d TILE STUFF ( keep records of three tiles infront )
	UINT16										usFrontArcFullTileList[ MAX_FULLTILE_DIRECTIONS ];
	INT32												usFrontArcFullTileGridNos[ MAX_FULLTILE_DIRECTIONS ];
	

	// PALETTE MANAGEMENT STUFF
	PaletteRepID								HeadPal;	// 30 
	PaletteRepID								PantsPal;	// 30
	PaletteRepID								VestPal;	// 30
	PaletteRepID								SkinPal;	// 30
	PaletteRepID								MiscPal;	// 30




	SGPPaletteEntry							*p8BPPPalette; // 4
	UINT16											*p16BPPPalette;
	UINT16											*pShades[ NUM_SOLDIER_SHADES ]; // Shading tables
	UINT16											*pGlowShades[ 20 ]; // 
	UINT16											*pCurrentShade;
	UINT8												ubFadeLevel;
	UINT8												ubServiceCount;
	UINT8												ubServicePartner;
	THROW_PARAMS								*pThrowParams;
	INT8												bReverse;
	LEVELNODE				*pLevelNode;
	LEVELNODE				*pExternShadowLevelNode;
	LEVELNODE				*pRoofUILevelNode;




	// UNBLIT BACKGROUND
	UINT16											*pBackGround;
	UINT16											*pZBackground;
	UINT16											usUnblitX, usUnblitY;
	UINT16											usUnblitWidth, usUnblitHeight;

	UINT8												ubStrategicInsertionCode;
	INT32											usStrategicInsertionData;

	
	INT32												iLight;
	INT32												iMuzFlash;
	INT8												bMuzFlashCount;


	INT16												sX;
	INT16												sY;

	UINT16											usOldAniState;
	INT16												sOldAniCode;

	INT8												bBulletsLeft;
	UINT8												ubSuppressionPoints;

	// STUFF FOR RANDOM ANIMATIONS
	UINT32											uiTimeOfLastRandomAction;
	INT16												usLastRandomAnim;



	UINT16											usAnimSurface;
	UINT16											sZLevel;

	INT32												sWalkToAttackGridNo;
	INT16												sWalkToAttackWalkToCost;


	INT16												sLocatorOffX;
	INT16												sLocatorOffY;
	UINT16											*pForcedShade;

	INT8												bDisplayDamageCount;
	INT16												sDamage;
	INT16												sDamageX;
	INT16												sDamageY;
	INT8												bDamageDir;
	INT8												bDoBurst;
	INT16												usUIMovementMode;
	INT8												bUIInterfaceLevel;

	UINT8												ubProfile;
	UINT8												ubQuoteRecord;
	UINT8												ubQuoteActionID;
	UINT8												ubBattleSoundID;

	UINT8												ubClosePanelFrame;
	UINT8												ubDeadPanelFrame;
	INT8												bOpenPanelFrame;

	INT16												sPanelFaceX;
	INT16												sPanelFaceY;

	// QUOTE STUFF
	INT8												bNumHitsThisTurn;
	UINT16											usQuoteSaidFlags;
	INT8												bLastSkillCheck;
	INT8												ubSkillCheckAttempts;

	INT8												bVocalVolume;	// verbal sounds need to differ in volume

	INT8												bStartFallDir;

	UINT8												ubPendingDirection;
	UINT32											uiAnimSubFlags;

	UINT8												bAimShotLocation;
	UINT8												ubHitLocation;

	UINT16											*pEffectShades[ NUM_SOLDIER_EFFECTSHADES ]; // Shading tables for effects

	UINT8												ubPlannedUIAPCost;
	INT16												sPlannedTargetX;
	INT16												sPlannedTargetY;

	INT32											sSpreadLocations[ MAX_BURST_SPREAD_TARGETS ];
	INT32												sStartGridNo;	
	INT32												sEndGridNo;	
	INT32												sForcastGridno;
	INT16												sZLevelOverride;
	INT8												bMovedPriorToInterrupt;
	INT32												iEndofContractTime;				// time, in global time(resolution, minutes) that merc will leave, or if its a M.E.R.C. merc it will be set to -1.	-2 for NPC and player generated
	INT32												iStartContractTime;		
	INT32						iTotalContractLength;			// total time of AIM mercs contract	or the time since last paid for a M.E.R.C. merc
	INT32												iNextActionSpecialData;		// AI special action data record for the next action
	UINT8												ubWhatKindOfMercAmI;			//Set to the type of character it is
	INT8						bAssignment;							// soldiers current assignment 
	INT8												bOldAssignment;						// old assignment, for autosleep purposes
	INT8						bTrainStat;								// current stat soldier is training
	INT16						sSectorX;									// X position on the Stategic Map
	INT16						sSectorY;									// Y position on the Stategic Map
	INT8												bSectorZ;									// Z sector location
	INT32						iVehicleId;								// the id of the vehicle the char is in
	PathStPtr					pMercPath;								//Path Structure
	UINT16											usMedicalDeposit;		 // is there a medical deposit on merc 
	UINT16											usLifeInsurance;			// is there life insurance taken out on merc	


	//DEF:	Used for the communications
	UINT32												uiStartMovementTime;				// the time since the merc first started moving 
	UINT32												uiOptimumMovementTime;			// everytime in ececute overhead the time for the current ani will be added to this total
	UINT32												usLastUpdateTime ;					// The last time the soldier was in ExecuteOverhead
	
	UINT32											uiSoldierUpdateNumber;
	BYTE												ubSoldierUpdateType;	
	INT32												sScheduledStop; //hayden, used for scheduling a grid to stop
	//END

	INT32												iStartOfInsuranceContract;
	UINT32											uiLastAssignmentChangeMin;		// timestamp of last assignment change in minutes
	INT32												iTotalLengthOfInsuranceContract;

	UINT8												ubSoldierClass;									//admin, elite, troop (creature types?) Nav: 2 seems to mean elite, 3 troop so admin is 1
	UINT8												ubAPsLostToSuppression;
	UINT8												ubSuppressorID;

	//Squad merging vars
	UINT8												ubDesiredSquadAssignment;
	UINT8												ubNumTraversalsAllowedToMerge;

	UINT16											usPendingAnimation2;
	UINT8												ubCivilianGroup;


	UINT32											uiUniqueSoldierIdValue; // the unique value every instance of a soldier gets - 1 is the first valid value

	INT8												bEndDoorOpenCode;

	UINT8												ubScheduleID;
	INT32												sEndDoorOpenCodeData;//dnl ch53 121009
	INT8												bBlockedByAnotherMercDirection;
	UINT16											usAttackingWeapon;
	INT8												bWeaponMode;
	UINT8												ubTargetID;
	INT8												bAIScheduleProgress;
	INT32												sOffWorldGridNo;
	struct TAG_anitile					*pAniTile;	
	INT8												bCamo;
	INT32												sAbsoluteFinalDestination;
	UINT8												ubHiResDirection;
	UINT8												ubHiResDesiredDirection;
	UINT8												ubLastFootPrintSound;
	INT8												bVehicleID;
	INT8												bMovementDirection;
	INT32												sOldGridNo;
	UINT16											usDontUpdateNewGridNoOnMoveAnimChange;
	INT16												sBoundingBoxWidth;
	INT16												sBoundingBoxHeight;
	INT16												sBoundingBoxOffsetX;
	INT16												sBoundingBoxOffsetY;
	UINT32											uiTimeSameBattleSndDone;
	INT8												bOldBattleSnd;
	INT32												iBurstSoundID;
	INT8												bSlotItemTakenFrom;
	UINT8												ubAutoBandagingMedic;
	UINT8												ubRobotRemoteHolderID;
	UINT32											uiTimeOfLastContractUpdate;
	INT8												bTypeOfLastContract;
	INT8												bTurnsCollapsed;
	INT8												bSleepDrugCounter;
	UINT8												ubMilitiaKills;
	

	INT8												bBlindedCounter;

	UINT8												ubHoursOnAssignment;						// used for assignments handled only every X hours

	UINT8												ubMercJustFired;	// the merc was just fired..there may be dialogue events occuring, this flag will prevent any interaction with contracts
																								// until after the merc leaves	
	UINT8												ubTurnsUntilCanSayHeardNoise;
	UINT16											usQuoteSaidExtFlags;

	INT32											sContPathLocation;//dnl ch53 was UINT16
	INT8												bGoodContPath;
	UINT8												ubPendingActionInterrupted;
	INT8												bNoiseLevel;
	INT8												bRegenerationCounter;					// Flugente: not used anymore!
	INT8												bRegenBoostersUsedToday;				// Flugente: not used anymore!
	INT8												bNumPelletsHitBy;
	INT32												sSkillCheckGridNo;
	UINT8												ubLastEnemyCycledID;

	UINT8												ubPrevSectorID;
	UINT8												ubNumTilesMovesSinceLastForget;
	INT8												bTurningIncrement;
	UINT32											uiBattleSoundID;

	UINT16											usValueGoneUp;

	UINT8												ubNumLocateCycles;
	UINT8												ubDelayedMovementFlags;
	UINT8												ubCTGTTargetID;

	UINT32											uiMercChecksum;

	INT8												bCurrentCivQuote;
	INT8												bCurrentCivQuoteDelta;
	UINT8												ubMiscSoldierFlags;
	UINT8												ubReasonCantFinishMove;

	INT32												sLocationOfFadeStart;
	UINT8												bUseExitGridForReentryDirection;

	UINT32											uiTimeSinceLastSpoke;
	UINT8												ubContractRenewalQuoteCode;
	INT32												sPreTraversalGridNo;
	UINT32											uiXRayActivatedTime;
	INT8												bTurningFromUI;
	INT8												bPendingActionData5;

	INT8												bDelayedStrategicMoraleMod;
	UINT8												ubDoorOpeningNoise;

	struct GROUP								*pGroup;
	UINT8												ubLeaveHistoryCode;
	INT8												bOverrideMoveSpeed;

	UINT32											uiTimeSoldierWillArrive;


	INT8												bVehicleUnderRepairID;
	INT32						iTimeCanSignElsewhere;
	INT8												bHospitalPriceModifier;
	UINT32											uiStartTimeOfInsuranceContract;
	INT8						bCorpseQuoteTolerance;
	INT8						bDeafenedCounter;
	INT32						iPositionSndID;	
	INT32						iTuringSoundID;
	UINT8						ubLastDamageReason;
	INT32												sLastTwoLocations[2];
	INT32						uiTimeSinceLastBleedGrunt;
	UINT8						ubNextToPreviousAttackerID;
	UINT8						bDoAutofire;
	INT8 numFlanks;
	INT32 lastFlankSpot;
	INT8 sniper;
	INT16 origDir;
	INT8	wornCamo;

	INT8	urbanCamo;	// Madd: new camo types
	INT8	wornUrbanCamo;

	INT8	desertCamo;	
	INT8	wornDesertCamo;

	INT8	snowCamo;	
	INT8	wornSnowCamo;

	// HEADROCK HAM 3.6: Added integer tracking the facility this character is using.
	INT16 sFacilityTypeOperated;
	// HEADROCK HAM 3.6: I'm removing this filler to make room for the above variable. I'm very worried though,
	// I don't know if this is a good idea at all...
	//INT16	filler;

	// Flugente: Is this the correct position?
	INT8												bScopeMode;

	///////////////////////////////////////////////////////
	// Flugente: this was the location of required variables required for the now removed poison feature. They can be used again
	UINT8	ubMilitiaAssists;		// Flugente: stores militia assists
	INT8	sNonNPCTraderID;		// Flugente: we can set up non-NPC soldiers to be merchants, we store their dealer id here (value > 0 means arms dealer entry x)
	//INT8	bUnusedINT8_3;
	UINT8	usDragPersonID;			// Flugente: id of person we are dragging

	//INT16	bUnusedINT16_4;
	INT16	sDragCorpseID;			// Flugente: id of corpse we are dragging

	UINT16	usChatPartnerID;		// Flugente: the id of another merc we are 'chatting' with
	
	// Flugente: new variables for extra stats	
	INT16	bExtraStrength;			// additional strength gained via power armor
	INT16	bExtraDexterity;		// additional dexterity gained via drugs
	INT16	bExtraAgility;			// additional agility gained via drugs
	INT16	bExtraWisdom;			// additional wisdom gained via drugs
	INT8	bExtraExpLevel;			// corruption can temporarily alter our ExpLevel without messing up our stats this way

	UINT32	usSoldierFlagMask;		// for various soldier-related flags (Illusion, Kill streak, etc.). Easier than adding 32 bool variables

	// Flugente: food system
	INT32	bFoodLevel;				// current level of food saturation
	INT32	bDrinkLevel;			// current level of drink saturation

	UINT8	usStarveDamageHealth;	// damage to health due to starvation. Can be cured by surgery, but only if nutrition level is high enough again
	UINT8	usStarveDamageStrength;	// damage to strength due to starvation. Can be cured by surgery, but only if nutrition level is high enough again

	// Flugente: multi-turn actions	
	INT16	bOverTurnAPS;			// remaining AP cost for the next turns	(allows actions to be performed for more than one turn)
	INT32	sMTActionGridNo;		// gridno on which we perfrom our multi-turn action
	UINT8	usMultiTurnAction;		// specifies which multi-turn action we are currently performing, 0: none

	INT16	bAIIndex;			    // feynman: PlanFactory from the modularized tactical AI that shall be used

	UINT16	usSoldierProfile;		// Flugente: allow linking to a xml-based profile specifiying name, visuals, traits etc.

	// Flugente: sector ID for move items assignment
	UINT8	usItemMoveSectorID;

	// Flugente: skill stuff
	UINT8	usAISkillUse;							// this variable allows the AI to remember which skill it wants to use
	UINT16	usSkillCounter[SOLDIER_COUNTER_MAX];	// counters used for various skill/trait/taint effects
	UINT32	usSkillCooldown[SOLDIER_COOLDOWN_MAX];	// cooldown used for various skill/trait/taint effects

	// Flugente: diseases
	INT16	sDiseasePoints[NUM_DISEASES];			// we store the state of our diseases here
	UINT8	sDiseaseFlag[NUM_DISEASES];				// we need to store some special flags for every disease
		
	// Flugente: Decrease this filler by 1 for each new UINT8 / BOOLEAN variable, so we can maintain savegame compatibility!!
	// Note that we also have to account for padding, so you might need to substract more than just the size of the new variables
	UINT8	ubFiller[12];
	UINT8	ubFiller1;
	
	// Flugente: modifiers to fire modes
	UINT8	usGLDelayMode;			// if > 0, delay GL grenade explosions
	UINT8	usBarrelMode;			// states how many barrels we are currently using as modifier for our fire mode
	UINT8	usBarrelCounter;		// states which barrel we are currently firing from (used during the firing process)

	// Flugente: focus skill gridno
	INT32	sFocusGridNo;

	UINT32	usSoldierFlagMask2;		// anv: another usSoldierFlagMask

	UINT32	usIndividualMilitiaID;	// Flugente: if this is a militia, this is the ID of the militia data

	
#ifdef JA2UB
	//ja25
	BOOLEAN											fIgnoreGetupFromCollapseCheck;
	TIMECOUNTER									GetupFromJA25StartCounter;
	BOOLEAN											fWaitingToGetupFromJA25Start;

	UINT8												ubPercentDamageInflictedByTeam[NUM_ASSIST_SLOTS];			//The percent of damage inflicted by the player team.  Each element corresponds to the Soldier ID.  Each element contains the percent damage inflicted by that merc
#endif
	
	char endOfPOD;	// marker for end of POD (plain old data)

	// Note: Place all non-POD items at the end (after endOfPOD)
	// The format of this structure affects what is written into and read from various
	// files (maps, save files, etc.).	If you change it then that code will not work 
	// properly until it is all fixed and the files updated.

	Inventory inv;
    AI::tactical::Plan*                             ai_masterplan_; // Interface object for ModularizedTacticalAI

	//data from version 101 wrapped into structs
	STRUCT_AIData									aiData;
	STRUCT_Flags									flags;
	STRUCT_TimeChanges								timeChanges;
	STRUCT_TimeCounters								timeCounters;
	//STRUCT_Drugs									drugs;			// Flugente: drug values are now in newdrugs
	DRUGS											newdrugs;
	STRUCT_Statistics								stats;
	STRUCT_Pathing									pathing;

	// Debugging data - not saved
	INT32 sPlotSrcGrid;
	//std::vector<UINT32>	CTH;

	// sevenfm: remember suppression points, shock from last attack
	// these counters are used only for showing suppression values above soldier (similar to damage counter)
	// these values are not saved
	UINT8	ubLastShock;
	UINT8	ubLastSuppression;
	UINT8	ubLastAP;
	UINT8	ubLastMorale;
	UINT8	ubLastShockFromHit;
	UINT8	ubLastAPFromHit;
	UINT8	ubLastMoraleFromHit;
	UINT8	iLastBulletImpact;
	UINT8	iLastArmourProtection;
	
	UINT16	usQuickItemId;
	UINT8	ubQuickItemSlot;

public:
	// CREATION FUNCTIONS
	BOOLEAN DeleteSoldier( void );
	BOOLEAN CreateSoldierLight( void );
	BOOLEAN DeleteSoldierLight( void );

	BOOLEAN CreateSoldierCommon( UINT8 ubBodyType, UINT16 usSoldierID, UINT16 usState );


	// Soldier Management functions, called by Event Pump.c
	BOOLEAN EVENT_InitNewSoldierAnim( UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce );

	BOOLEAN ChangeSoldierState( UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce );
	void EVENT_SetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos );
	void EVENT_SetSoldierDestination( UINT8	ubNewDirection );
	void EVENT_GetNewSoldierPath( INT32 sDestGridNo, UINT16 usMovementAnim );

	void EVENT_SetSoldierDirection( UINT16	usNewDirection );
	void EVENT_SetSoldierDesiredDirection( UINT16	usNewDirection );
	void EVENT_FireSoldierWeapon( INT32 sTargetGridNo );
	void EVENT_SoldierGotHit( UINT16 usWeaponIndex, INT16 ubDamage, INT16 sBreathLoss, UINT16 bDirection , UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation, INT16 sSubsequent, INT32 sLocationGridNo );
	void EVENT_SoldierBeginBladeAttack( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginPunchAttack( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginFirstAid( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_StopMerc( INT32 sGridNo, INT8 bDirection );
	void EVENT_SoldierBeginCutFence( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginRepair( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginRefuel( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginKnifeThrowAttack( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginUseDetonator( void );
	void EVENT_SoldierBeginDropBomb( );
	void EVENT_SoldierDefuseTripwire( INT32 sGridNo, INT32 sItem );
	void EVENT_SoldierEnterVehicle( INT32 sGridNo, UINT8 ubDirection, UINT8 ubSeatIndex = 0 );
	void EVENT_SoldierBeginGiveItem( void );
	void EVENT_SetSoldierPositionAndMaybeFinalDest( FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateFinalDest );
	void EVENT_SetSoldierPositionForceDelete( FLOAT dNewXPos, FLOAT dNewYPos );
	void EVENT_SoldierBeginReloadRobot( INT32 sGridNo, UINT8 ubDirection, UINT8 ubMercSlot );
	void EVENT_SoldierBeginTakeBlood( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginAttachCan( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_BeginMercTurn( BOOLEAN fFromRealTime, INT32 iRealTimeCounter );

	void EVENT_SoldierBuildStructure( INT32 sGridNo, UINT8 ubDirection );		// added by Flugente
	void EVENT_SoldierHandcuffPerson( INT32 sGridNo, UINT8 ubDirection );		// added by Flugente
	void EVENT_SoldierApplyItemToPerson( INT32 sGridNo, UINT8 ubDirection );	// added by Flugente
	void EVENT_SoldierTakeBloodFromPerson( INT32 sGridNo, UINT8 ubDirection );	// added by Flugente
	void EVENT_SoldierInteractiveAction( INT32 sGridNo, UINT16 usActionType );					// added by Flugente

	BOOLEAN EVENT_InternalGetNewSoldierPath( INT32 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestart );
	void EVENT_InternalSetSoldierDestination( UINT16	usNewDirection, BOOLEAN fFromMove, UINT16 usAnimState );
	void EVENT_InternalSetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos ,BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest, BOOLEAN fForceDelete );


	// Soldier Management functions called by Overhead.c
	// Flugente: frozen soldiers do not move. We simulate this by using fixed animation frames, which we determine here 
	UINT16 CryoAniFrame();
	BOOLEAN ConvertAniCodeToAniFrame( UINT16 usAniFrame );
	void TurnSoldier( void );
	void ChangeSoldierStance( UINT8 ubDesiredStance );
	void StopSoldier( void );
	void ReviveSoldier( void );
	UINT8 SoldierTakeDamage( INT8 bHeight, INT16 sLifeDeduct, INT16 sBreathDeduct, UINT8 ubReason, UINT8 ubAttacker, INT32 sSourceGrid, INT16 sSubsequent, BOOLEAN fShowDamage );


	// Palette functions for soldiers
	BOOLEAN CreateSoldierPalettes( void );

	// UTILITY FUNCTUIONS
	void MoveMerc( FLOAT dMovementChange, FLOAT dAngle, BOOLEAN fCheckRange );
	INT16 CalcActionPoints( void );
	BOOLEAN IsActionInterruptable( void );
	// This function is now obsolete.	Call ReduceAttackBusyCount instead.
	// void ReleaseSoldiersAttacker( SOLDIERTYPE *pSoldier );
	BOOLEAN MercInWater( void );
	BOOLEAN MercInShallowWater( void );
	BOOLEAN MercInDeepWater( void );
	BOOLEAN MercInHighWater( void );
	UINT16 GetNewSoldierStateFromNewStance( UINT8 ubDesiredStance );
	UINT16 GetMoveStateBasedOnStance( UINT8 ubStanceHeight );
	void SoldierGotoStationaryStance( void );
	BOOLEAN ReCreateSoldierLight( void );
	void RemoveSoldierFromGridNo( void );
	void InternalRemoveSoldierFromGridNo( BOOLEAN fForce );


	void CalcNewActionPoints( void );
	void AdjustNoAPToFinishMove( BOOLEAN fSet );




	UINT32 SoldierDressWound( SOLDIERTYPE *pVictim, INT16 sKitPts, INT16 sStatus );
	void ReceivingSoldierCancelServices( void );
	void GivingSoldierCancelServices( void );
	void InternalReceivingSoldierCancelServices( BOOLEAN fPlayEndAnim );
	void InternalGivingSoldierCancelServices( BOOLEAN fPlayEndAnim );


	void PositionSoldierLight( void );
	void SetCheckSoldierLightFlag( void );
	void SetSoldierCowerState( BOOLEAN fOn );
	void SetSoldierAsUnderAiControl( void );
	void ResetSoldierChangeStatTimer( void );
	void SetSoldierGridNo( INT32 sNewGridNo, BOOLEAN fForceRemove );
	void SetSoldierHeight( FLOAT dNewHeight );
	void InternalSetSoldierHeight( FLOAT dNewHeight, BOOLEAN fUpdateLevel );//this function did not have a forward declaration


	BOOLEAN DoMercBattleSound( UINT8 ubBattleSoundID );
	BOOLEAN InternalDoMercBattleSound( UINT8 ubBattleSoundID, INT8 bSpecialCode );
	BOOLEAN GetProfileFlagsFromGridno( UINT16 usAnimState, INT32 sTestGridNo, UINT16 *usFlags );
	void HaultSoldierFromSighting( BOOLEAN fFromSightingEnemy );
	void ReLoadSoldierAnimationDueToHandItemChange( UINT16 usOldItem, UINT16 usNewItem );
	void	DoNinjaAttack( void );
	void PickDropItemAnimation( void );


	void BeginSoldierGetup( void );
	void BeginSoldierClimbUpRoof( void );
	void BeginSoldierClimbDownRoof( void );
	void BeginSoldierClimbFence( void );
	void BeginTyingToFall( void );
	
	//legion by Jazz
	void BeginSoldierClimbWall( void  ); 
	void BeginSoldierClimbWindow( void );
	void BeginSoldierClimbWallUp( void  ); 
	
	void HandleAnimationProfile( UINT16	usAnimState, BOOLEAN fRemove );
	void HandleSoldierTakeDamageFeedback( void );
	void ChangeToFlybackAnimation( UINT8 flyBackDirection );
	void ChangeToFallbackAnimation( UINT8 fallBackDirection );


	void UpdateRobotControllerGivenController( void );
	void UpdateRobotControllerGivenRobot( void );
	SOLDIERTYPE *GetRobotController( void );
	BOOLEAN CanRobotBeControlled( void );
	BOOLEAN ControllingRobot( void );




	BOOLEAN SoldierReadyWeapon( INT16 sTargetXPos, INT16 sTargetYPos, BOOLEAN fEndReady, BOOLEAN fRaiseToHipOnly );
	BOOLEAN SoldierReadyWeapon( void );
	BOOLEAN InternalSoldierReadyWeapon( UINT8 sFacingDir, BOOLEAN fEndReady, BOOLEAN fRaiseToHipOnly );

	BOOLEAN CheckSoldierHitRoof( void );
	BOOLEAN CheckForBreathCollapse( void );
	BOOLEAN PlayerSoldierStartTalking( UINT8 ubTargetID, BOOLEAN fValidate );
	BOOLEAN InternalIsValidStance( INT8 bDirection, INT8 bNewStance );
	BOOLEAN IsValidSecondHandShot( void );
	BOOLEAN IsValidSecondHandBurst( void );
	BOOLEAN IsValidSecondHandShotForReloadingPurposes( void );
	BOOLEAN IsValidAlternativeFireMode( INT16 bAimTime, INT32 iTrgGridNo );
	BOOLEAN IsValidShotFromHip( INT16 bAimTime, INT32 iTrgGridNo );
	BOOLEAN IsValidPistolFastShot( INT16 bAimTime, INT32 iTrgGridNo );
	BOOLEAN SoldierCarriesTwoHandedWeapon( void );
	void	SoldierInventoryCoolDown( void );		// Flugente: Cool down/decay all items in inventory
	BOOLEAN	IsWeaponMounted( void );				// determine if we receive a bonus for mounting our weapon on something
	OBJECTTYPE* GetUsedWeapon( OBJECTTYPE * pObj );		// if in an underbarrel fire mode, return underbarrel weapon
	UINT16  GetUsedWeaponNumber( OBJECTTYPE * pObj );	// if in an underbarrel fire mode, return number of underbarrel weapon

	// returns damage resistance in percent
	INT32	GetDamageResistance( BOOLEAN fAutoResolve = FALSE, BOOLEAN fCalcBreathLoss = FALSE);
	INT8	GetHearingBonus();
	INT16	GetSightRangeBonus();

	INT16	GetSoldierCriticalDamageBonus( void );	// Flugente: determines critical damage bonus depending on class, skill, etc.

	// Flugente: Zombies
	BOOLEAN IsZombie( void );
			
	// reset the extra stat variables
	void	ResetExtraStats();

	// Flugente: inventory bombs can ignite while in mapscreen. Workaround: Damage items and health
	void	InventoryExplosion( void );

	// Flugente: do we currently provide ammo (pAmmoSlot) for someone else's (pubId) gun (pGunSlot)?
	BOOLEAN		IsFeedingExternal(UINT8* pubId1, UINT16* pGunSlot1, UINT16* pAmmoSlot1, UINT8* pubId2, UINT16* pGunSlot2, UINT16* pAmmoSlot2);

	// Flugente: return first found object with a specific flag from our inventory
	OBJECTTYPE* GetObjectWithFlag( UINT64 aFlag );

	// Flugente: functions for the covert ops trait

	// do we look like a civilian?
	BOOLEAN		LooksLikeACivilian( void );

	// do we look like a soldier?
	BOOLEAN		LooksLikeASoldier( void );

	// what kind of uniform are we wearing? returns -1 if none is worn
	INT8		GetUniformType();

	// is our equipment too good for a soldier?
	BOOLEAN		EquipmentTooGood( BOOLEAN fCloselook );
	
	// does soldier ubObserverID recognize us as his enemy?
	BOOLEAN		SeemsLegit( UINT8 ubObserverID );

	// do we recognize someone else as a combatant?
	BOOLEAN		RecognizeAsCombatant(UINT8 ubTargetID);

	// loose covert property
	void		LooseDisguise( void );	

	// sevenfm: auto disguise if have correct clothes
	void		Disguise();

	void		ApplyCovert(BOOLEAN aWithMessage);

	// lose disguise or take off any clothes item and switch back to original clothes
	void		Strip();

	// check wether our disguise is any good
	void		SpySelfTest();

	// Flugente: prisoner system
	BOOLEAN		CanProcessPrisoners();
	UINT32		GetSurrenderStrength();
	BOOLEAN		FreePrisoner();					// used for an enemy liberating fellow prisoners 
	BOOLEAN		CanBeCaptured();				// can this guy be captured (by handcuffing or asking him to surrender)?

	// Flugente: scuba gear
	BOOLEAN		UsesScubaGear();

	// Flugente: are we an assassin?
	BOOLEAN		IsAssassin();

	// Flugente: multi-turn actions
	UINT8		GetMultiTurnAction();
	void		StartMultiTurnAction( UINT8 usActionType, INT32 asGridNo );
	void		CancelMultiTurnAction(BOOLEAN fFinished);
	BOOLEAN		UpdateMultiTurnAction();

	void		DropSectorEquipment();

	// sevenfm: Take new bomb with id = usItem from iventory to HANDPOS
	void 		TakeNewBombFromInventory(UINT16 usItem);
	void		TakeNewItemFromInventory(UINT16 usItem);

	// Flugente: switch hand item for gunsling weapon, or pistol, or knife
	void		SwitchWeapons( BOOLEAN fKnife = FALSE, BOOLEAN fSideArm = FALSE );

	// Flugente: return a soldier's name. This allows for very easy manipulation of a soldier's name with pre- an suffixes, ranks etc.
	STR16		GetName();

	INT8		GetTraitCTHModifier( UINT16 usItem, INT16 ubAimTime, UINT8 ubTargetProfile );

	//void		AddDrugValues(UINT8 uDrugType, UINT8 usEffect, UINT8 usTravelRate, UINT8 usSideEffect );

	void		HandleFlashLights();
	UINT8		GetBestEquippedFlashLightRange();

	// Flugente: soldier profiles
	INT8		GetSoldierProfileType(UINT8 usTeam);		// retrieves the correct sub-array

	// Flugente: do we have a specific background flag?
	BOOLEAN		HasBackgroundFlag( UINT64 aFlag );
	INT16		GetBackgroundValue( UINT16 aNr );

	INT8		GetSuppressionResistanceBonus();			// bonus to resistance against suppression
	INT16		GetMeleeDamageBonus();
	INT16		GetAPBonus();
	INT8		GetFearResistanceBonus();					// fear resistance lowers shock and morale damage from horror
	UINT8		GetMoraleThreshold();
	FLOAT		GetMoraleModifier();
	INT16		GetInterruptModifier( UINT8 usDistance );

	void		SoldierPropertyUpkeep();					// update functions for various properties (updating counters, resetting flags etc.)

	// Flugente: functions for skill usage
	// traits can allow use of certain skills
	// check if Soldier can use the spell skillwise, with fAPCheck = TRUE also check current APs
	BOOLEAN CanUseSkill( INT8 iSkill, BOOLEAN fAPCheck = TRUE, INT32 sGridNo = -1 );

	// use a skill. For safety reasons, this calls CanUseSkill again (it is possible to switch the soldier while the menu is open)
	BOOLEAN UseSkill( UINT8 iSkill, INT32 usMapPos, UINT32 ID );

	// is the AI allowed to use a skill? we have to check how much breath and life using this skill would cost, as otherwise the AI might commit suicide by casting
	BOOLEAN IsAIAllowedtoUseSkill( INT8 iSkill );

	// print a small description of the skill if we can use it, or its requirements if we cannot
	STR16	PrintSkillDesc( INT8 iSkill);

	// Flugente: functions for the radio operator trait
	BOOLEAN CanUseRadio(BOOLEAN fCheckForAP = TRUE);							// can we use radio, if we even have one?
	BOOLEAN UseRadio();								// simply drain the batteries
	BOOLEAN HasMortar();
	BOOLEAN GetSlotOfSignalShellIfMortar(UINT8* pbLoop); // if we have a mortar and a fitting signal shell, return the inventory slot it is in
	BOOLEAN CanAnyArtilleryStrikeBeOrdered(UINT32* pSectorID);		// can any artillery strikes be ordered by this guy's team from the neighbouring sectors?
	BOOLEAN OrderArtilleryStrike( UINT32 usSectorNr, INT32 sTargetGridNo, UINT8 bTeam );
	BOOLEAN IsJamming();							// are we currently jamming communication frequencies?
	BOOLEAN JamCommunications();
	BOOLEAN IsScanning();
	BOOLEAN ScanForJam();
	BOOLEAN IsRadioListening();
	BOOLEAN RadioListen();
	BOOLEAN RadioCallReinforcements( UINT32 usSector, UINT16 sNumber );
	BOOLEAN SwitchOffRadio();
	void	RadioFail();							// display and error sound used either when the radio set fails or the sector is jammed - the player knows of the error, but cannot be sure of the cause

	// Flugente: spotter
	BOOLEAN IsSpotting();
	BOOLEAN CanSpot( INT32 sTargetGridNo = -1 );
	BOOLEAN BecomeSpotter( INT32 sTargetGridNo );

	// Flugente: enemy roles
	BOOLEAN HasSniper();
	BOOLEAN CanMedicAI();						// AI-only: can we heal a wounded ally? Do NOT, repeat, NOT use this with mercs!
	BOOLEAN	AIDoctorFriend();					// AI-only: heal a wounded friend. Do NOT, repeat, NOT use this with mercs!
	BOOLEAN AIDoctorSelf();						// AI-only: heal self. Do NOT, repeat, NOT use this with mercs!

	// Flugente: boxing fix: this shall be the only location where the boxing flag gets removed (easier debugging)
	void	DeleteBoxingFlag();

	// Flugente: disease
	void	Infect( UINT8 aDisease );
	void	AddDiseasePoints( UINT8 aDisease, INT32 aVal  );
	void	AnnounceDisease( UINT8 aDisease );

	// do we have any disease?
	// fDiagnosedOnly: check for wether we know of this infection
	// fHealableOnly: check wether it can be healed
	// fSymbolOnly: only show if symbol should be shown
	BOOLEAN HasDisease(BOOLEAN fDiagnosedOnly, BOOLEAN fHealableOnly, BOOLEAN fSymbolOnly = FALSE);
	BOOLEAN HasDiseaseWithFlag(UINT32 aFlag);
	FLOAT	GetDiseaseMagnitude( UINT8 aDisease );			// get the magnitude of a disease we might have, used to determine wether there are any effects
	void	PrintDiseaseDesc( CHAR16* apStr, BOOLEAN fFullDesc = FALSE );
	void	PrintFoodDesc( CHAR16* apStr, BOOLEAN fFullDesc = FALSE );
	void	PrintSleepDesc( CHAR16* apStr );
	FLOAT   GetDiseaseContactProtection();		// get percentage protection from infections via contact
	INT16	GetDiseaseResistance();
	FLOAT	GetBurialPoints( UINT16* apCorpses );

	// Flugente: hourly breath regen calculation
	INT8	GetSleepBreathRegeneration();

	// Flugente: assumed character weight (without any items)
	FLOAT	GetBodyWeight();

	// Flugente: are we crouched against cover from a specific direction? WARNING: This does not suffice to determine our cover!
	BOOLEAN	IsCrouchedAgainstCoverFromDir( UINT8 aDirection );

	// Flugente: fortification
	FLOAT	GetConstructionPoints( );

	// Flugente: do we have a specific item in our inventory?
	BOOLEAN HasItem( UINT16 usItem );
	BOOLEAN SelfDetonate();						// AI-only: blow up explosive in own inventory. Do NOT, repeat, NOT use this with mercs!

	// Flugente: chance to defeat a water snake instead of being hit by it
	UINT8	GetWaterSnakeDefenseChance( );

	// Flugente: interactive actions
	UINT16	GetInteractiveActionSkill( INT32 sGridNo, UINT8 usLevel, UINT16 usType );

	// Flugente: riot shields
	OBJECTTYPE*	GetEquippedRiotShield();
	BOOLEAN		IsRiotShieldEquipped();
	void		DestroyEquippedRiotShield();
	void		RiotShieldTakeDamage(INT32 sDamage);

	// Flugente: drag people
	BOOLEAN		CanDragInPrinciple();
	BOOLEAN		CanDragPerson( UINT8 usID );
	BOOLEAN		CanDragCorpse( UINT16 usCorpseNum );
	BOOLEAN		IsDraggingSomeone();
	void		SetDragOrderPerson( UINT16 usID );
	void		SetDragOrderCorpse( UINT32 usID );
	void		CancelDrag();

	// Flugente: spy assignments
	UINT8		GetUncoverRisk();
	FLOAT		GetIntelGain();

	void		StopChatting();
	void		DrugAutoUse();

	OBJECTTYPE*	GetObjectWithItemFlag( UINT64 aFlag );
	void		DestroyOneObjectWithItemFlag( UINT64 aFlag );

	// Flugente: can we fill a blood bag from this guy ?
	BOOLEAN		IsValidBloodDonor();
	//////////////////////////////////////////////////////////////////////////////

}; // SOLDIERTYPE;	

#define SIZEOF_SOLDIERTYPE_POD offsetof( SOLDIERTYPE, endOfPOD )
#define SIZEOF_OLDSOLDIERTYPE_101_POD offsetof( OLDSOLDIERTYPE_101, endOfPOD )

#define HEALTH_INCREASE			0x0001
#define STRENGTH_INCREASE		0x0002
#define	DEX_INCREASE				0x0004
#define AGIL_INCREASE				0x0008
#define WIS_INCREASE				0x0010
#define LDR_INCREASE				0x0020

#define MRK_INCREASE				0x0040
#define MED_INCREASE				0x0080
#define EXP_INCREASE				0x0100
#define MECH_INCREASE				0x0200

#define LVL_INCREASE				0x0400




// Moved to weapons.h by ADB, rev 1513
/*enum WeaponModes
{
	WM_NORMAL = 0,
	WM_BURST,
	WM_AUTOFIRE,
	WM_ATTACHED_GL,
	WM_ATTACHED_GL_BURST,
	WM_ATTACHED_GL_AUTO,
	NUM_WEAPON_MODES
} ;
*/
// TYPEDEFS FOR ANIMATION PROFILES
typedef struct
{
	UINT16	usTileFlags;
	INT8		bTileX;
	INT8		bTileY;

} ANIM_PROF_TILE;

typedef struct
{
	UINT8							ubNumTiles;
	ANIM_PROF_TILE		*pTiles;

}	ANIM_PROF_DIR;

typedef struct ANIM_PROF
{
	
	ANIM_PROF_DIR		Dirs[8];

} ANIM_PROF;



// Globals
//////////

// VARIABLES FOR PALETTE REPLACEMENTS FOR HAIR, ETC
extern UINT32					guiNumPaletteSubRanges;
extern UINT8					*gubpNumReplacementsPerRange;
extern PaletteSubRangeType		*gpPaletteSubRanges;
extern UINT32					guiNumReplacements;
extern PaletteReplacementType	*gpPalRep;

extern UINT8					bHealthStrRanges[];


// Functions
////////////

// Soldier Management functions called by Overhead.c
void RevivePlayerTeam( );

BOOLEAN ReCreateSelectedSoldierLight(	);

// Palette functions for soldiers
BOOLEAN GetPaletteRepIndexFromID( PaletteRepID aPalRep, UINT8 *pubPalIndex );
BOOLEAN	SetPaletteReplacement( SGPPaletteEntry *p8BPPPalette, PaletteRepID aPalRep );
BOOLEAN LoadPaletteData( );
BOOLEAN DeletePaletteData( );

// UTILITY FUNCTUIONS
void MoveMercFacingDirection( SOLDIERTYPE *pSoldier, BOOLEAN fReverse, FLOAT dMovementDist );
UINT8 GetDirectionFromXY( INT16 sXPos, INT16 sYPos, SOLDIERTYPE *pSoldier );
BOOLEAN GetDirectionChangeAmount( INT32 sGridNo, SOLDIERTYPE *pSoldier, UINT8 uiTurnAmount);
UINT8 GetDirectionFromGridNo( INT32 sGridNo, SOLDIERTYPE *pSoldier );
UINT8 atan8( INT16 sXPos, INT16 sYPos, INT16 sXPos2, INT16 sYPos2 );
UINT8 atan8FromAngle( DOUBLE dAngle );
INT16 GetDirectionToGridNoFromGridNo( INT32 sGridNoDest, INT32 sGridNoSrc );
// This function is now obsolete.	Call ReduceAttackBusyCount instead.
// void ReleaseSoldiersAttacker( SOLDIERTYPE *pSoldier );



// WRAPPERS FOR SOLDIER EVENTS
void SendSoldierPositionEvent( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos );
void SendSoldierDestinationEvent( SOLDIERTYPE *pSoldier, UINT16 usNewDestination );
void SendGetNewSoldierPathEvent( SOLDIERTYPE *pSoldier, INT32 sDestGridNo, UINT16 usMovementAnim );
void SendSoldierSetDirectionEvent( SOLDIERTYPE *pSoldier, UINT16 usNewDirection );
void SendSoldierSetDesiredDirectionEvent( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection );
void SendChangeSoldierStanceEvent( SOLDIERTYPE *pSoldier, UINT8 ubNewStance );
void SendBeginFireWeaponEvent( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo );



void HandleAnimationProfile( SOLDIERTYPE *pSoldier, UINT16	usAnimState, BOOLEAN fRemove );
BOOLEAN GetProfileFlagsFromGridno( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sTestGridNo, UINT16 *usFlags );
BOOLEAN PreloadSoldierBattleSounds( SOLDIERTYPE *pSoldier, BOOLEAN fRemove );
void CrowsFlyAway( UINT8 ubTeam );
void DebugValidateSoldierData( );
void HandlePlayerTogglingLightEffects( BOOLEAN fToggleValue );

// added by SANDRO
UINT8 GetSquadleadersCountInVicinity( SOLDIERTYPE * pSoldier, BOOLEAN fWithHigherLevel, BOOLEAN fDontCheckDistance );
UINT16 NumberOfDamagedStats( SOLDIERTYPE * pSoldier );
UINT8 RegainDamagedStats( SOLDIERTYPE * pSoldier, UINT16 usAmountRegainedHundredths );
BOOLEAN ResolvePendingInterrupt( SOLDIERTYPE * pSoldier, UINT8 ubInterruptType );
BOOLEAN AIDecideHipOrShoulderStance( SOLDIERTYPE * pSoldier, INT32 iGridNo );
BOOLEAN DecideAltAnimForBigMerc( SOLDIERTYPE * pSoldier );

// added by Flugente
BOOLEAN TwoStagedTrait( UINT8 uiSkillTraitNumber );						// determine if this (new) trait has two stages
BOOLEAN MajorTrait( UINT8 uiSkillTraitNumber );							// determine if this is a major trait
BOOLEAN GetRadioOperatorSignal(UINT8 usOwner, INT32* psTargetGridNo);	// retrieve the gridno of a radio operator who (or whose team) ordered an artillery strike
BOOLEAN IsValidArtilleryOrderSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 bTeam );		// can an artillery strike be ordered FROM this sector
BOOLEAN SectorJammed();
BOOLEAN PlayerTeamIsScanning();
UINT16	GridNoSpotterCTHBonus( SOLDIERTYPE* pSniper, INT32 sGridNo, UINT bTeam);				// bonus for snipers firing at this location (we get this if there are spotters)
UINT16	GetSuspiciousAnimationAPDuration( UINT16 usAnimation );			// get overt penalty duration in AP for using an animation

//typedef struct
class OLDSOLDIERTYPE_101
{
public:
	UINT32 GetChecksum();
	OLDSOLDIERTYPE_101() {
		bNewItemCount.reserve(inv.size());
		bNewItemCycleCount.reserve(inv.size());
		for (int idx=0; idx < (int)inv.size(); ++idx) {
			bNewItemCount.push_back(0);
			bNewItemCycleCount.push_back(0);
		}
		initialize();
	};

	// Copy Constructor
	OLDSOLDIERTYPE_101(const OLDSOLDIERTYPE_101& src) {
		memcpy(this, &src, SIZEOF_OLDSOLDIERTYPE_101_POD); // FIXME: memcpy std::vector
		inv = src.inv;
		bNewItemCount = src.bNewItemCount;
		bNewItemCycleCount = src.bNewItemCycleCount;
	};

	// Assignment operator
	OLDSOLDIERTYPE_101& operator=(const OLDSOLDIERTYPE_101& src)
	{
		if (this != &src) {
			memcpy(this, &src, SIZEOF_OLDSOLDIERTYPE_101_POD); // FIXME: memcpy std::vector
			inv = src.inv;
			bNewItemCount = src.bNewItemCount;
			bNewItemCycleCount = src.bNewItemCycleCount;
		}
		return *this;
	};

	// Destructor
	~OLDSOLDIERTYPE_101() {
	};

	// Initialize the soldier.	
	//	Use this instead of the old method of calling memset!
	//	Note that the constructor does this automatically.
	void initialize() {
		memset( this, 0, SIZEOF_OLDSOLDIERTYPE_101_POD); // FIXME: memcpy std::vector
		inv.clear();
		for (int idx=0; idx < (int)inv.size(); ++idx) {
			bNewItemCount[idx] = 0;
			bNewItemCycleCount[idx] = 0;
		}
	};

	// Ugly temporary solution
	void CopyOldInventoryToNew();

public:
	// ID
	UINT8												ubID;
	UINT8												bReserved1;

	// DESCRIPTION / STATS, ETC
	UINT8												ubBodyType;
	INT8												bActionPoints;
	INT8												bInitialActionPoints;

	UINT32											uiStatusFlags;

	OLD_OBJECTTYPE_101									DO_NOT_USE_Inv[ OldInventory::NUM_INV_SLOTS ];
public:
	OBJECTTYPE									*pTempObject;
	KEY_ON_RING									*pKeyRing;

	INT8												bOldLife;			// life at end of last turn, recorded for monster AI
	// attributes
	UINT8												bInSector;
	INT8												bFlashPortraitFrame;
	INT16												sFractLife;		// fraction of life pts (in hundreths)	
	INT8												bBleeding;		// blood loss control variable
	INT8												bBreath;			// current breath value
	INT8												bBreathMax;	// max breath, affected by fatigue/sleep
	INT8												bStealthMode;
	
	INT16												sBreathRed;			// current breath value
	BOOLEAN											fDelayedMovement;

	BOOLEAN											fReloading;
	UINT8												ubWaitActionToDo;
	BOOLEAN											fPauseAim;
	INT8												ubInsertionDirection;
	INT8												bGunType;
	// skills
	UINT8												ubOppNum;
	INT8												bLastRenderVisibleValue;
	BOOLEAN											fInMissionExitNode;
	UINT8												ubAttackingHand;
	INT8												bScientific;	
	// traits	
	INT16												sWeightCarriedAtTurnStart;
	CHAR16											name[ 10 ];
	
	INT8												bVisible;			// to render or not to render...

	
	INT8												bActive;

	INT8												bTeam;				// Team identifier

	//NEW MOVEMENT INFORMATION for Strategic Movement
	UINT8												ubGroupID;		//the movement group the merc is currently part of.
	BOOLEAN											fBetweenSectors;	//set when the group isn't actually in a sector.
																								//sSectorX and sSectorY will reflect the sector the
																								//merc was at last.

	UINT8												ubMovementNoiseHeard;// 8 flags by direction

	// 23 bytes so far 	

	// WORLD POSITION STUFF
	FLOAT											dXPos;
	FLOAT											dYPos;
	//FLOAT											dOldXPos; // HEADROCK HAM 4: These values have been reappropriated.
	//FLOAT											dOldYPos; // not sure what's going to happen with this though.
	INT16											sOldXPos;
	INT16											sOldYPos;
	INT32												sInitialGridNo;
	INT32												sGridNo;
	UINT8												ubDirection;
	INT16												sHeightAdjustment;
	INT16												sDesiredHeight;
	INT32												sTempNewGridNo;					// New grid no for advanced animations
	INT16												sRoomNo;
	INT8												bOverTerrainType;
	INT8												bOldOverTerrainType;

	INT8												bCollapsed;					// collapsed due to being out of APs
	INT8												bBreathCollapsed;					// collapsed due to being out of APs
	// 50 bytes so far

	
	UINT8												ubDesiredHeight;
	UINT16											usPendingAnimation;
	UINT8												ubPendingStanceChange;
	UINT16											usAnimState;
	BOOLEAN											fNoAPToFinishMove;
	BOOLEAN											fPausedMove;
	BOOLEAN											fUIdeadMerc;				// UI Flags for removing a newly dead merc
	BOOLEAN											fUInewMerc;					// UI Flags for adding newly created merc ( panels, etc )
	BOOLEAN											fUICloseMerc;				// UI Flags for closing panels
	BOOLEAN											fUIFirstTimeNOAP;		// UI Flag for diming guys when no APs ( dirty flags )
	BOOLEAN											fUIFirstTimeUNCON;	// UI FLAG For unconscious dirty		

	

	TIMECOUNTER									UpdateCounter;
	TIMECOUNTER									DamageCounter;
	TIMECOUNTER									ReloadCounter;
	TIMECOUNTER									FlashSelCounter;
	TIMECOUNTER									AICounter;
	TIMECOUNTER									FadeCounter;
	
	UINT8												ubSkillTrait1;
	UINT8												ubSkillTrait2;
	UINT8												ubSkillTrait3; // added by SANDRO

	UINT32											uiAIDelay;
	INT8												bDexterity;		// dexterity (hand coord) value
	INT8												bWisdom;
	INT16												sReloadDelay;
	UINT8												ubAttackerID;
	UINT8												ubPreviousAttackerID;
	BOOLEAN											fTurnInProgress;

	BOOLEAN											fIntendedTarget; // intentionally shot?
	BOOLEAN											fPauseAllAnimation;

	INT8												bExpLevel;		// general experience level
	INT32												sInsertionGridNo;

	BOOLEAN											fContinueMoveAfterStanceChange;

	// 60
	AnimationSurfaceCacheType		AnimCache; // will be 9 bytes once changed to pointers

	INT8												bLife;				// current life (hit points or health)
	UINT8												bSide;
	UINT8												bViewRange;
	INT8 												bNewOppCnt;
	INT8												bService;		// first aid, or other time consuming process
	
	UINT16											usAniCode;
	UINT16											usAniFrame;
	INT16											sAniDelay;

	// MOVEMENT TO NEXT TILE HANDLING STUFF
	INT8												bAgility;			// agility (speed) value
	UINT8												ubDelayedMovementCauseMerc;
	INT32												sDelayedMovementCauseGridNo;
	INT32												sReservedMovementGridNo;

	INT8												bStrength;

	// Weapon Stuff
	BOOLEAN											fHoldAttackerUntilDone;
	INT32 sTargetGridNo;
	INT8												bTargetLevel;
	INT8												bTargetCubeLevel;
	INT32												sLastTarget;
	INT8												bTilesMoved;
	INT8						bLeadership;
	FLOAT												dNextBleed;
	BOOLEAN											fWarnedAboutBleeding;
	BOOLEAN											fDyingComment;

	UINT8												ubTilesMovedPerRTBreathUpdate;
	UINT16											usLastMovementAnimPerRTBreathUpdate;
	
	BOOLEAN											fTurningToShoot;
	BOOLEAN											fTurningToFall;
	BOOLEAN											fTurningUntilDone;
	BOOLEAN											fGettingHit;
	BOOLEAN											fInNonintAnim;
	BOOLEAN											fFlashLocator;
	INT16												sLocatorFrame;
	BOOLEAN											fShowLocator;
	BOOLEAN											fFlashPortrait;
	INT8												bMechanical;
	INT8												bLifeMax;			// maximum life for this merc

	INT32												iFaceIndex;
	

	// PALETTE MANAGEMENT STUFF
	PaletteRepID								HeadPal;	// 30 
	PaletteRepID								PantsPal;	// 30
	PaletteRepID								VestPal;	// 30
	PaletteRepID								SkinPal;	// 30
	PaletteRepID								MiscPal;	// 30


	// FULL 3-d TILE STUFF ( keep records of three tiles infront )
	UINT16										usFrontArcFullTileList[ MAX_FULLTILE_DIRECTIONS ];
	INT32												usFrontArcFullTileGridNos[ MAX_FULLTILE_DIRECTIONS ];


	SGPPaletteEntry							*p8BPPPalette; // 4
	UINT16											*p16BPPPalette;
	UINT16											*pShades[ NUM_SOLDIER_SHADES ]; // Shading tables
	UINT16											*pGlowShades[ 20 ]; // 
	UINT16											*pCurrentShade;
	INT8												bMedical;
	BOOLEAN											fBeginFade;
	UINT8												ubFadeLevel;
	UINT8												ubServiceCount;
	UINT8												ubServicePartner;
	INT8												bMarksmanship;
	INT8												bExplosive;
	THROW_PARAMS								*pThrowParams;
	INT8											bTurningFromPronePosition;
	INT8												bReverse;
	LEVELNODE				*pLevelNode;
	LEVELNODE				*pExternShadowLevelNode;
	LEVELNODE				*pRoofUILevelNode;

	// WALKING STUFF
	INT8												bDesiredDirection;
	INT16												sDestXPos;
	INT16												sDestYPos;
	INT32												sDesiredDest;
	INT32												sDestination;
	INT32												sFinalDestination;
	INT8												bLevel;
	INT8												bStopped;
	INT8												bNeedToLook;




	// PATH STUFF
	UINT16											usPathingData[ MAX_PATH_LIST_SIZE ];
	UINT16											usPathDataSize;
	UINT16											usPathIndex;	
	INT32												sBlackList;
	INT8												bAimTime;
	INT8												bShownAimTime;
	INT8												bPathStored;	// good for AI to reduct redundancy
	INT8												bHasKeys;			// allows AI controlled dudes to open locked doors


	// UNBLIT BACKGROUND
	UINT16											*pBackGround;
	UINT16											*pZBackground;
	UINT16											usUnblitX, usUnblitY;
	UINT16											usUnblitWidth, usUnblitHeight;

	UINT8												ubStrategicInsertionCode;
	INT32											usStrategicInsertionData;

	
	INT32												iLight;
	INT32												iMuzFlash;
	INT8												bMuzFlashCount;


	INT16												sX;
	INT16												sY;

	UINT16											usOldAniState;
	INT16												sOldAniCode;

	INT8												bBulletsLeft;
	UINT8												ubSuppressionPoints;

	// STUFF FOR RANDOM ANIMATIONS
	UINT32											uiTimeOfLastRandomAction;
	INT16												usLastRandomAnim;


	// AI STUFF
	INT8												bOppList[MAX_NUM_SOLDIERS]; // AI knowledge database
	INT8												bLastAction;
	INT8												bAction;	
	INT32											usActionData;
	INT8												bNextAction;
	INT32											usNextActionData;
	INT8												bActionInProgress;
	INT8												bAlertStatus;
	INT8												bOppCnt;
	INT8												bNeutral;
	INT8												bNewSituation;
	INT8												bNextTargetLevel;
	INT8												bOrders;
	INT8												bAttitude;
	INT8												bCharacterTrait;	// Added by SANDRO
	INT8												bUnderFire;
	INT8												bShock;
	INT8												bUnderEscort;
	INT8												bBypassToGreen;
	UINT8												ubLastMercToRadio;
	INT8												bDominantDir;				// AI main direction to face...
	INT8												bPatrolCnt;					// number of patrol gridnos
	INT8												bNextPatrolPnt;			// index to next patrol gridno
	INT16												usPatrolGrid[OLD_MAXPATROLGRIDS];//dnl ch27 230909
	INT16												sNoiseGridNo;
	UINT8												ubNoiseVolume;
	INT8												bLastAttackHit;
	UINT8												ubXRayedBy;
	FLOAT												dHeightAdjustment;
	INT8												bMorale;
	INT8												bTeamMoraleMod;
	INT8												bTacticalMoraleMod;
	INT8												bStrategicMoraleMod;
	INT8												bAIMorale;
	UINT8												ubPendingAction;
	UINT8												ubPendingActionAnimCount;
	UINT32											uiPendingActionData1;
	INT32												sPendingActionData2;
	INT8												bPendingActionData3;
	INT8												ubDoorHandleCode;
	UINT32											uiPendingActionData4;
	INT8												bInterruptDuelPts;
	INT8												bPassedLastInterrupt;
	INT8												bIntStartAPs;
	INT8												bMoved;
	INT8												bHunting;
	UINT8												ubLastCall;
	UINT8												ubCaller;
	INT32												sCallerGridNo;
	UINT8												bCallPriority;
	INT8												bCallActedUpon;
	INT8												bFrenzied;
	INT8												bNormalSmell;
	INT8												bMonsterSmell;
	INT8												bMobility;
	INT8												bRTPCombat;
	INT8												fAIFlags;

	BOOLEAN											fDontChargeReadyAPs;
	UINT16											usAnimSurface;
	UINT16											sZLevel;
	BOOLEAN											fPrevInWater;
	BOOLEAN											fGoBackToAimAfterHit;

	INT32												sWalkToAttackGridNo;
	INT16												sWalkToAttackWalkToCost;

	BOOLEAN											fForceRenderColor;
	BOOLEAN											fForceNoRenderPaletteCycle;

	INT16												sLocatorOffX;
	INT16												sLocatorOffY;
	BOOLEAN											fStopPendingNextTile;

	BOOLEAN											fForceShade;
	UINT16											*pForcedShade;

	INT8												bDisplayDamageCount;
	INT8												fDisplayDamage;
	INT16												sDamage;
	INT16												sDamageX;
	INT16												sDamageY;
	INT8												bDamageDir;
	INT8												bDoBurst;
	INT16												usUIMovementMode;
	INT8												bUIInterfaceLevel;
	BOOLEAN											fUIMovementFast;

	TIMECOUNTER									BlinkSelCounter;
	TIMECOUNTER									PortraitFlashCounter;
	BOOLEAN											fDeadSoundPlayed;
	UINT8												ubProfile;
	UINT8												ubQuoteRecord;
	UINT8												ubQuoteActionID;
	UINT8												ubBattleSoundID;

	BOOLEAN											fClosePanel;
	BOOLEAN											fClosePanelToDie;
	UINT8												ubClosePanelFrame;
	BOOLEAN											fDeadPanel;
	UINT8												ubDeadPanelFrame;
	BOOLEAN											fOpenPanel;
	INT8												bOpenPanelFrame;

	INT16												sPanelFaceX;
	INT16												sPanelFaceY;

	// QUOTE STUFF
	INT8												bNumHitsThisTurn;
	UINT16											usQuoteSaidFlags;
	INT8												fCloseCall;
	INT8												bLastSkillCheck;
	INT8												ubSkillCheckAttempts;

	INT8												bVocalVolume;	// verbal sounds need to differ in volume

	INT8												bStartFallDir;
	INT8												fTryingToFall;

	UINT8												ubPendingDirection;
	UINT32											uiAnimSubFlags;

	UINT8												bAimShotLocation;
	UINT8												ubHitLocation;

	UINT16											*pEffectShades[ NUM_SOLDIER_EFFECTSHADES ]; // Shading tables for effects

	UINT8												ubPlannedUIAPCost;
	INT16												sPlannedTargetX;
	INT16												sPlannedTargetY;

	INT32											  sSpreadLocations[ MAX_BURST_SPREAD_TARGETS ];
	BOOLEAN											fDoSpread;
	INT32												sStartGridNo;	
	INT32												sEndGridNo;	
	INT32												sForcastGridNo;
	INT16												sZLevelOverride;
	INT8												bMovedPriorToInterrupt;
	INT32												iEndofContractTime;				// time, in global time(resolution, minutes) that merc will leave, or if its a M.E.R.C. merc it will be set to -1.	-2 for NPC and player generated
	INT32												iStartContractTime;		
	INT32						iTotalContractLength;			// total time of AIM mercs contract	or the time since last paid for a M.E.R.C. merc
	INT32												iNextActionSpecialData;		// AI special action data record for the next action
	UINT8												ubWhatKindOfMercAmI;			//Set to the type of character it is
	INT8						bAssignment;							// soldiers current assignment 
	INT8												bOldAssignment;						// old assignment, for autosleep purposes
	BOOLEAN											fForcedToStayAwake;				// forced by player to stay awake, reset to false, the moment they are set to rest or sleep
	INT8						bTrainStat;								// current stat soldier is training
	INT16						sSectorX;									// X position on the Stategic Map
	INT16						sSectorY;									// Y position on the Stategic Map
	INT8												bSectorZ;									// Z sector location
	INT32						iVehicleId;								// the id of the vehicle the char is in
	PathStPtr					pMercPath;								//Path Structure
	UINT8												fHitByGasFlags;						// flags 
	UINT16											usMedicalDeposit;		 // is there a medical deposit on merc 
	UINT16											usLifeInsurance;			// is there life insurance taken out on merc	


	//DEF:	Used for the communications
	UINT32												uiStartMovementTime;				// the time since the merc first started moving 
	UINT32												uiOptimumMovementTime;			// everytime in ececute overhead the time for the current ani will be added to this total
	UINT32												usLastUpdateTime ;					// The last time the soldier was in ExecuteOverhead
	
	BOOLEAN											fIsSoldierMoving;							// ie.	Record time is on
	BOOLEAN											fIsSoldierDelayed;						//Is the soldier delayed Soldier 
	BOOLEAN											fSoldierUpdatedFromNetwork;
	UINT32											uiSoldierUpdateNumber;
	BYTE												ubSoldierUpdateType;	
	//END

	INT32												iStartOfInsuranceContract;
	UINT32											uiLastAssignmentChangeMin;		// timestamp of last assignment change in minutes
	INT32												iTotalLengthOfInsuranceContract;

	UINT8												ubSoldierClass;									//admin, elite, troop (creature types?)
	UINT8												ubAPsLostToSuppression;
	BOOLEAN											fChangingStanceDueToSuppression;
	UINT8												ubSuppressorID;

	//Squad merging vars
	UINT8												ubDesiredSquadAssignment;
	UINT8												ubNumTraversalsAllowedToMerge;

	UINT16											usPendingAnimation2;
	UINT8												ubCivilianGroup;

	// time changes...when a stat was changed according to GetJA2Clock();
	UINT32											uiChangeLevelTime;
	UINT32											uiChangeHealthTime;
	UINT32											uiChangeStrengthTime;
	UINT32											uiChangeDexterityTime;
	UINT32											uiChangeAgilityTime;
	UINT32											uiChangeWisdomTime;
	UINT32											uiChangeLeadershipTime;
	UINT32											uiChangeMarksmanshipTime;
	UINT32											uiChangeExplosivesTime;
	UINT32											uiChangeMedicalTime;
	UINT32											uiChangeMechanicalTime;

	UINT32											uiUniqueSoldierIdValue; // the unique value every instance of a soldier gets - 1 is the first valid value
	INT8											UNUSED1; // This is unused at present and can be used for something else

//private:
	INT8											DO_NOT_USE_bNewItemCount[ OldInventory::NUM_INV_SLOTS ];
	INT8											DO_NOT_USE_bNewItemCycleCount[ OldInventory::NUM_INV_SLOTS ];
public:
	BOOLEAN											fCheckForNewlyAddedItems;
	INT8												bEndDoorOpenCode;

	UINT8												ubScheduleID;
	INT16												sEndDoorOpenCodeData;
	TIMECOUNTER									NextTileCounter;
	BOOLEAN											fBlockedByAnotherMerc;
	INT8												bBlockedByAnotherMercDirection;
	UINT16											usAttackingWeapon;
	INT8												bWeaponMode;
	UINT8												ubTargetID;
	INT8												bAIScheduleProgress;
	INT32												sOffWorldGridNo;
	struct TAG_anitile					*pAniTile;	
	INT8												bCamo;
	INT32												sAbsoluteFinalDestination;
	UINT8												ubHiResDirection;
	UINT8												ubHiResDesiredDirection;
	UINT8												ubLastFootPrintSound;
	INT8												bVehicleID;
	INT8												fPastXDest;
	INT8												fPastYDest;
	INT8												bMovementDirection;
	INT32												sOldGridNo;
	UINT16											usDontUpdateNewGridNoOnMoveAnimChange;
	INT16												sBoundingBoxWidth;
	INT16												sBoundingBoxHeight;
	INT16												sBoundingBoxOffsetX;
	INT16												sBoundingBoxOffsetY;
	UINT32											uiTimeSameBattleSndDone;
	INT8												bOldBattleSnd;
	BOOLEAN											fReactingFromBeingShot;
	BOOLEAN											fContractPriceHasIncreased;
	INT32												iBurstSoundID;
	BOOLEAN											fFixingSAMSite;
	BOOLEAN											fFixingRobot;
	INT8												bSlotItemTakenFrom;
	BOOLEAN											fSignedAnotherContract; 
	UINT8												ubAutoBandagingMedic;
	BOOLEAN											fDontChargeTurningAPs;
	UINT8												ubRobotRemoteHolderID;
	UINT32											uiTimeOfLastContractUpdate;
	INT8												bTypeOfLastContract;
	INT8												bTurnsCollapsed;
	INT8												bSleepDrugCounter;
	UINT8												ubMilitiaKills;
	
	INT8												bFutureDrugEffect[DRUG_TYPE_MAX];						// value to represent effect of a needle
	INT8												bDrugEffectRate[DRUG_TYPE_MAX];							// represents rate of increase and decrease of effect	
	INT8												bDrugEffect[DRUG_TYPE_MAX];									// value that affects AP & morale calc ( -ve is poorly )
	INT8												bDrugSideEffectRate[DRUG_TYPE_MAX];					// duration of negative AP and morale effect
	INT8												bDrugSideEffect[DRUG_TYPE_MAX];							// duration of negative AP and morale effect
	INT8												bTimesDrugUsedSinceSleep[DRUG_TYPE_MAX];			

	INT8												bBlindedCounter;
	BOOLEAN											fMercCollapsedFlag;
	BOOLEAN											fDoneAssignmentAndNothingToDoFlag;
	BOOLEAN											fMercAsleep;
	BOOLEAN											fDontChargeAPsForStanceChange;

	UINT8												ubHoursOnAssignment;						// used for assignments handled only every X hours

	UINT8												ubMercJustFired;	// the merc was just fired..there may be dialogue events occuring, this flag will prevent any interaction with contracts
																								// until after the merc leaves	
	UINT8												ubTurnsUntilCanSayHeardNoise;
	UINT16											usQuoteSaidExtFlags;

	UINT32											sContPathLocation;	
	INT8												bGoodContPath;
	UINT8												ubPendingActionInterrupted;
	INT8												bNoiseLevel;
	INT8												bRegenerationCounter;
	INT8												bRegenBoostersUsedToday;
	INT8												bNumPelletsHitBy;
	INT32												sSkillCheckGridNo;
	UINT8												ubLastEnemyCycledID;

	UINT8												ubPrevSectorID;
	UINT8												ubNumTilesMovesSinceLastForget;
	INT8												bTurningIncrement;
	UINT32											uiBattleSoundID;

	BOOLEAN											fSoldierWasMoving;
	BOOLEAN											fSayAmmoQuotePending;
	UINT16											usValueGoneUp;

	UINT8												ubNumLocateCycles;
	UINT8												ubDelayedMovementFlags;
	BOOLEAN											fMuzzleFlash;
	UINT8												ubCTGTTargetID;

	TIMECOUNTER									PanelAnimateCounter;
	UINT32											uiMercChecksum;

	INT8												bCurrentCivQuote;
	INT8												bCurrentCivQuoteDelta;
	UINT8												ubMiscSoldierFlags;
	UINT8												ubReasonCantFinishMove;

	INT32												sLocationOfFadeStart;
	UINT8												bUseExitGridForReentryDirection;

	UINT32											uiTimeSinceLastSpoke;
	UINT8												ubContractRenewalQuoteCode;
	INT32												sPreTraversalGridNo;
	UINT32											uiXRayActivatedTime;
	INT8												bTurningFromUI;
	INT8												bPendingActionData5;

	INT8												bDelayedStrategicMoraleMod;
	UINT8												ubDoorOpeningNoise;

	struct GROUP								*pGroup;
	UINT8												ubLeaveHistoryCode;
	BOOLEAN											fDontUnsetLastTargetFromTurn;
	INT8												bOverrideMoveSpeed;
	BOOLEAN											fUseMoverrideMoveSpeed;

	UINT32											uiTimeSoldierWillArrive;
	BOOLEAN											fDieSoundUsed;
	BOOLEAN											fUseLandingZoneForArrival;
	BOOLEAN					 fFallClockwise;
	INT8												bVehicleUnderRepairID;
	INT32						iTimeCanSignElsewhere;
	INT8												bHospitalPriceModifier;
	INT8												bFillerBytes[3];
	UINT32											uiStartTimeOfInsuranceContract;
	BOOLEAN											fRTInNonintAnim;
	BOOLEAN					 fDoingExternalDeath;
	INT8						bCorpseQuoteTolerance;
	INT8						bDeafenedCounter;
	INT32						iPositionSndID;	
	INT32						iTuringSoundID;
	UINT8						ubLastDamageReason;
	BOOLEAN											fComplainedThatTired;
	INT32												sLastTwoLocations[2];
	INT16						bFillerDude;
	INT32						uiTimeSinceLastBleedGrunt;
	UINT8						ubNextToPreviousAttackerID;
	UINT8						bDoAutofire;
	BOOLEAN					autofireLastStep;
	INT8 numFlanks;
	INT32 lastFlankSpot;
	BOOLEAN lastFlankLeft;
	INT8 sniper;
	INT16 origDir;
	INT8	wornCamo;

	INT8	urbanCamo;	// Madd: new camo types
	INT8	wornUrbanCamo;

	INT8	desertCamo;	
	INT8	wornDesertCamo;

	INT8	snowCamo;	
	INT8	wornSnowCamo;
	
#ifdef JA2UB	
	BOOLEAN											fIgnoreGetupFromCollapseCheck;
	TIMECOUNTER									GetupFromJA25StartCounter;
	BOOLEAN											fWaitingToGetupFromJA25Start;

	UINT8												ubPercentDamageInflictedByTeam[NUM_ASSIST_SLOTS];			//The percent of damage inflicted by the player team.  Each element corresponds to the Soldier ID.  Each element contains the percent damage inflicted by that merc
#endif
	UINT8					bFiller[ 36 ];

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//
	char ef1,ef2,ef3;	// Extra filler to get "offsetof(endOfPOD)" to match SIZEOF(oldstruct)

	char endOfPOD;	// marker for end of POD (plain old data)

	Inventory inv;

	std::vector<int>	bNewItemCount;
	std::vector<int> bNewItemCycleCount;

	// Debugging data - not saved
	INT32 sPlotSrcGrid;
}; // OLDSOLDIERTYPE_101;	


void HandleTakeDamageDeath( SOLDIERTYPE *pSoldier, UINT8 bOldLife, UINT8 ubReason );

void SetDamageDisplayCounter(SOLDIERTYPE* pSoldier);

// SANDRO - This whole procedure was merged with the surgery ability of the doctor trait
UINT32 VirtualSoldierDressWound( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVictim, OBJECTTYPE *pKit, INT16 sKitPts, INT16 sStatus, BOOLEAN fOnSurgery );

// Flugente: decide whether pRecruiter can successfully recruit pTarget to be a volunteer
void HandleVolunteerRecruitment( SOLDIERTYPE* pRecruiter, SOLDIERTYPE* pTarget );

// Flugente: apply a consumable item on a soldier. Returns true if item was successfully interacted with
BOOLEAN ApplyConsumable( SOLDIERTYPE* pSoldier, OBJECTTYPE *pObject, BOOLEAN fForce, BOOLEAN fUseAPs );

#endif


