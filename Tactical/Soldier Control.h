#ifndef __SOLDER_CONTROL_H
#define __SOLDER_CONTROL_H

//dnl ch33 200909
// In the future MAXPATROLDGRIDS could be externalized but his value must always be >= OLD_MAXPATROLGRIDS
#define OLD_MAXPATROLGRIDS	10
#define MAXPATROLGRIDS		OLD_MAXPATROLGRIDS

#include "Animation Cache.h"
#include "Timer Control.h"
#include "vobject.h"
#include "Overhead Types.h"
#include "Item Types.h"
#include "worlddef.h"
#include <vector>
#include <iterator>

#define PTR_CIVILIAN	(pSoldier->bTeam == CIV_TEAM)
#define PTR_CROUCHED	(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH)
#define PTR_STANDING	(gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND)
#define PTR_PRONE	 (gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_PRONE)


// TEMP VALUES FOR NAMES
#define MAXCIVLASTNAMES		30
extern UINT16 CivLastNames[MAXCIVLASTNAMES][10];
 

// ANDREW: these are defines for OKDestanation usage - please move to approprite file
#define IGNOREPEOPLE	0
#define PEOPLETOO		1
#define ALLPEOPLE		2
#define FALLINGTEST	 3

#define	LOCKED_NO_NEWGRIDNO			2

#define	NO_PROFILE			200

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
#define HAS_SKILL_TRAIT( s, t ) (s->stats.ubSkillTrait1 == t || s->stats.ubSkillTrait2 == t)
#define NUM_SKILL_TRAITS( s, t ) ( (s->stats.ubSkillTrait1 == t) ? ( (s->stats.ubSkillTrait2 == t) ? 2 : 1 ) : ( (s->stats.ubSkillTrait2 == t) ? 1 : 0 ) )

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
#define SOLDIER_QUOTE_SAID_EXT_MIKE										0x0010
#define SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT						0x0020
#define SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED					0x0040
#define SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED					0x0080
#define SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED					0x0100


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
	BATTLE_SOUND_OK2,
	BATTLE_SOUND_COOL1,
	BATTLE_SOUND_CURSE1,
	BATTLE_SOUND_HIT1,
	BATTLE_SOUND_HIT2,
	BATTLE_SOUND_LAUGH1,
	BATTLE_SOUND_ATTN1,
	BATTLE_SOUND_DIE1,
	BATTLE_SOUND_HUMM,
	BATTLE_SOUND_NOTHING,
	BATTLE_SOUND_GOTIT,
	BATTLE_SOUND_LOWMARALE_OK1,
	BATTLE_SOUND_LOWMARALE_OK2,
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
};

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

// enum of uniform pieces
typedef struct
{
	PaletteRepID vest;
	PaletteRepID pants;
}UNIFORMCOLORS;

// HEADROCK HAM 3.6: Uniform colors for the different soldier classes
extern UNIFORMCOLORS gUniformColors[NUM_UNIFORMS];

// This macro should be used whenever we want to see if someone is neutral
// IF WE ARE CONSIDERING ATTACKING THEM.	Creatures & bloodcats will attack neutrals
// but they can't attack empty vehicles!!
#define CONSIDERED_NEUTRAL( me, them ) ( (them->aiData.bNeutral) && ( me->bTeam != CREATURE_TEAM || (them->flags.uiStatusFlags & SOLDIER_VEHICLE) ) )

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
	BOOLEAN											fGoBackToAimAfterHit;
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
	INT8			bFutureDrugEffect[2];						// value to represent effect of a needle
	INT8			bDrugEffectRate[2];							// represents rate of increase and decrease of effect	
	INT8			bDrugEffect[2];								// value that affects AP & morale calc ( -ve is poorly )
	INT8			bDrugSideEffectRate[2];					// duration of negative AP and morale effect
	INT8			bDrugSideEffect[2];							// duration of negative AP and morale effect
	INT8			bTimesDrugUsedSinceSleep[2];			
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

class STRUCT_Statistics//last edited at version 102
{
public:
	void				ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src);
	INT8												bLife;				// current life (hit points or health)
	INT8												bLifeMax;			// maximum life for this merc
	INT8												bExpLevel;		// general experience level
	INT8												bAgility;			// agility (speed) value
	INT8												bStrength;
	INT8												bMechanical;
	INT8												bMarksmanship;
	INT8												bExplosive;
	UINT8												ubSkillTrait1;
	UINT8												ubSkillTrait2;
	INT8												bDexterity;		// dexterity (hand coord) value
	INT8												bWisdom;
	INT8												bMedical;
	INT8												bScientific;	
	INT8						bLeadership;
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
	// Copy Constructor
	SOLDIERTYPE(const SOLDIERTYPE&);
	// Assignment operator
	SOLDIERTYPE& operator=(const SOLDIERTYPE&);
	// Destructor
	~SOLDIERTYPE();

	BOOLEAN Load(HWFILE hFile);
	BOOLEAN Save(HWFILE hFile);
	UINT32	GetChecksum();

	// Initialize the soldier.	
	//	Use this instead of the old method of calling memset.
	//	Note that the constructor does this automatically.
	void initialize();

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
	INT16												sBreathRed;			// current breath value
	

	UINT8												ubWaitActionToDo;
	INT8												ubInsertionDirection;
	INT8												bGunType;
	UINT8												ubOppNum;
	INT8												bLastRenderVisibleValue;
	UINT8												ubAttackingHand;
	INT16												sWeightCarriedAtTurnStart;
	

	//NEW MOVEMENT INFORMATION for Strategic Movement
	UINT8												ubGroupID;		//the movement group the merc is currently part of.
																								//sSectorX and sSectorY will reflect the sector the
																								//merc was at last.

	UINT8												ubMovementNoiseHeard;// 8 flags by direction


	// WORLD POSITION STUFF
	FLOAT											dXPos;
	FLOAT											dYPos;
	FLOAT											dOldXPos;
	FLOAT											dOldYPos;
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

	UINT8												ubSoldierClass;									//admin, elite, troop (creature types?)
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
	INT8												bRegenerationCounter;
	INT8												bRegenBoostersUsedToday;
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

	char endOfPOD;	// marker for end of POD (plain old data)

	// Note: Place all non-POD items at the end (after endOfPOD)
	// The format of this structure affects what is written into and read from various
	// files (maps, save files, etc.).	If you change it then that code will not work 
	// properly until it is all fixed and the files updated.

	Inventory inv;

	//data from version 101 wrapped into structs
	STRUCT_AIData									aiData;
	STRUCT_Flags									flags;
	STRUCT_TimeChanges								timeChanges;
	STRUCT_TimeCounters								timeCounters;
	STRUCT_Drugs									drugs;
	STRUCT_Statistics								stats;
	STRUCT_Pathing									pathing;

	// Debugging data - not saved
	INT32 sPlotSrcGrid;


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
	void EVENT_SoldierBeginDropBomb( void );
	void EVENT_SoldierEnterVehicle( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginGiveItem( void );
	void EVENT_SetSoldierPositionAndMaybeFinalDest( FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateFinalDest );
	void EVENT_SetSoldierPositionForceDelete( FLOAT dNewXPos, FLOAT dNewYPos );
	void EVENT_SoldierBeginReloadRobot( INT32 sGridNo, UINT8 ubDirection, UINT8 ubMercSlot );
	void EVENT_SoldierBeginTakeBlood( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_SoldierBeginAttachCan( INT32 sGridNo, UINT8 ubDirection );
	void EVENT_BeginMercTurn( BOOLEAN fFromRealTime, INT32 iRealTimeCounter );

	BOOLEAN EVENT_InternalGetNewSoldierPath( INT32 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestart );
	void EVENT_InternalSetSoldierDestination( UINT16	usNewDirection, BOOLEAN fFromMove, UINT16 usAnimState );
	void EVENT_InternalSetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos ,BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest, BOOLEAN fForceDelete );


	// Soldier Management functions called by Overhead.c
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
	void HandleAnimationProfile( UINT16	usAnimState, BOOLEAN fRemove );
	void HandleSoldierTakeDamageFeedback( void );
	void ChangeToFlybackAnimation( UINT8 flyBackDirection );
	void ChangeToFallbackAnimation( UINT8 fallBackDirection );


	void UpdateRobotControllerGivenController( void );
	void UpdateRobotControllerGivenRobot( void );
	SOLDIERTYPE *GetRobotController( void );
	BOOLEAN CanRobotBeControlled( void );
	BOOLEAN ControllingRobot( void );




	BOOLEAN SoldierReadyWeapon( INT16 sTargetXPos, INT16 sTargetYPos, BOOLEAN fEndReady );
	BOOLEAN SoldierReadyWeapon( void );
	BOOLEAN InternalSoldierReadyWeapon( UINT8 sFacingDir, BOOLEAN fEndReady );

	BOOLEAN CheckSoldierHitRoof( void );
	BOOLEAN CheckForBreathCollapse( void );
	BOOLEAN PlayerSoldierStartTalking( UINT8 ubTargetID, BOOLEAN fValidate );
	BOOLEAN InternalIsValidStance( INT8 bDirection, INT8 bNewStance );
	BOOLEAN IsValidSecondHandShot( void );
	BOOLEAN IsValidSecondHandShotForReloadingPurposes( void );
	BOOLEAN SoldierCarriesTwoHandedWeapon( void );

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
		memcpy(this, &src, SIZEOF_OLDSOLDIERTYPE_101_POD);
		inv = src.inv;
		bNewItemCount = src.bNewItemCount;
		bNewItemCycleCount = src.bNewItemCycleCount;
	};

	// Assignment operator
	OLDSOLDIERTYPE_101& operator=(const OLDSOLDIERTYPE_101& src)
	{
		if (this != &src) {
			memcpy(this, &src, SIZEOF_OLDSOLDIERTYPE_101_POD);
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
		memset( this, 0, SIZEOF_OLDSOLDIERTYPE_101_POD);
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
	FLOAT											dOldXPos;
	FLOAT											dOldYPos;
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
	
	INT8												bFutureDrugEffect[2];						// value to represent effect of a needle
	INT8												bDrugEffectRate[2];							// represents rate of increase and decrease of effect	
	INT8												bDrugEffect[2];									// value that affects AP & morale calc ( -ve is poorly )
	INT8												bDrugSideEffectRate[2];					// duration of negative AP and morale effect
	INT8												bDrugSideEffect[2];							// duration of negative AP and morale effect
	INT8												bTimesDrugUsedSinceSleep[2];			

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

#endif




