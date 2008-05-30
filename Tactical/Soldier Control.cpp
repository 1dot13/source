#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include "builddefines.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "wcheck.h"
#include "Render Fun.h"
#include "stdlib.h"
#include "debug.h"
#include "MemMan.h"
#include "Overhead Types.h"
//#include "Soldier Control.h"
#include "Animation Cache.h"
#include "Animation Data.h"
#include "Animation Control.h"
#include "container.h"
#include <math.h>
#include "pathai.h"
#include "Random.h"
#include "worldman.h"
#include "Isometric Utils.h"
#include "Render Dirty.h"
#include "renderworld.h"
#include "sys globals.h"
#include "video.h"
#include "points.h"
#include "Win util.h"
#include "Sound Control.h"
#include "weapons.h"
#include "vobject_blitters.h"
#include "Handle UI.h"
#include "soldier ani.h"
#include "Event pump.h"
#include "opplist.h"
#include "ai.h"
#include "interface.h"
#include "lighting.h"
#include "faces.h"
#include "Soldier Profile.h"
#include "gap.h"
#include "interface panels.h"
#include "campaign.h"
#include "soldier macros.h"
#include "english.h"
#include "Squads.h"

#ifdef NETWORKED
#include "Networking.h"
#include "NetworkEvent.h"
#endif
#include "structure wrap.h"
#include "items.h"
#include "Soundman.h"
#include "utilities.h"
#include "Strategic.h"
#include "soldier tile.h"
#include "Smell.h"
#include "Keys.h"
#include "dialogue control.h"
#include "soldier functions.h"
#include "rt time defines.h"
#include "Exit Grids.h"
#include "gamescreen.h"
#include "Quests.h"
#include "message.h"
#include "NPC.h"
#include "SkillCheck.h"
#include "handle doors.h"
#include "interface dialogue.h"
#include "smokeeffects.h"
#include	"GameSettings.h"
#include "tile animation.h"
#include "ShopKeeper Interface.h"
#include "Arms Dealer Init.h"
#include "vehicles.h"
#include "rotting corpses.h"
#include "Interface Control.h"
#include "strategicmap.h"
#include "morale.h"
#include "meanwhile.h"
#include "drugs and alcohol.h"
#include "SkillCheck.h"
#include "boxing.h"
#include "overhead map.h"
#include "Map Information.h"
#include "environment.h"
#include "Game Clock.h"
#include "Explosion Control.h"
#include "Buildings.h"
#include "Text.h"
#include "Strategic Merc Handler.h"
#include "Campaign Types.h"
#include "Strategic Status.h"
#include "civ quotes.h"
#include "Strategic Pathing.h"
#endif

#include "fresh_header.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


//turnspeed
//UINT8 gubPlayerTurnSpeedUpFactor = 1;
//UINT8 gubEnemyTurnSpeedUpFactor = 1;
//UINT8 gubCreatureTurnSpeedUpFactor = 1;
//UINT8 gubMilitiaTurnSpeedUpFactor = 1;
//UINT8 gubCivilianTurnSpeedUpFactor = 1;
//turnspeed

//extern BOOLEAN fAllowTacticalMilitiaCommand; //lal

extern INT16 DirIncrementer[8];

#define		PALETTEFILENAME							"BINARYDATA\\ja2pal.dat"

#define		LOW_MORALE_BATTLE_SND_THREASHOLD	35


#define		MIN_SUBSEQUENT_SNDS_DELAY									2000
#include "connect.h"

// Enumerate extended directions
enum
{
	EX_NORTH									= 0,
	EX_NORTHEAST							= 4,
	EX_EAST										= 8,
	EX_SOUTHEAST							= 12,
	EX_SOUTH									= 16,
	EX_SOUTHWEST							= 20,
	EX_WEST										= 24,
	EX_NORTHWEST							= 28,
	EX_NUM_WORLD_DIRECTIONS		= 32,
	EX_DIRECTION_IRRELEVANT
} ExtendedWorldDirections;

// LUT for conversion from 8-direction to extended direction
UINT8 ubExtDirection[] =
{
	EX_NORTH,
	EX_NORTHEAST,
	EX_EAST,
	EX_SOUTHEAST,
	EX_SOUTH,
	EX_SOUTHWEST,
	EX_WEST,
	EX_NORTHWEST
};


UINT8 gExtOneCDirection[ EX_NUM_WORLD_DIRECTIONS ] =
{
	4,
	5,
	6,
	7,

	8,
	9,
	10,
	11,

	12,
	13,
	14,
	15,

	16,
	17,
	18,
	19,

	20,
	21,
	22,
	23,

	24,
	25,
	26,
	27,

	28,
	29,
	30,
	31,

	0,
	1,
	2,
	3,
};


typedef struct
{
	CHAR8				zName[20];
	UINT8				ubRandomVal;
	BOOLEAN			fPreload;
	BOOLEAN			fBadGuy;
	BOOLEAN			fDontAllowTwoInRow;
	BOOLEAN			fStopDialogue;

} BATTLESNDS_STRUCT;


BATTLESNDS_STRUCT	 gBattleSndsData[] =
{
	"ok1",			2,				1,			1,		1,		2,
	"ok2",			0,				1,			1,		1,		2,
	"cool",			0,				1,			0,		1,		0,
	"curse",		0,				1,			1,		1,		0,
	"hit1",			2,				1,			1,		1,		1,
	"hit2",			0,				1,			1,		1,		1,
	"laugh",		0,				1,			1,		1,		0,
	"attn",			0,				1,			0,		1,		0,
	"dying",		0,				1,			1,		1,		1,
	"humm",			0,				0,			0,		1,		1,
	"noth",			0,				0,			0,		1,		1,
	"gotit",		0,				0,			0,		1,		1,
	"lmok1",		2,				1,			0,		1,		2,
	"lmok2",		0,				1,			0,		1,		2,
	"lmattn",		0,				1,			0,		1,		0,
	"locked",		0,				0,			0,		1,		0,
	"enem",			0,				1,			1,		1,		0,
};

// CHRISL:
MERCPROFILEGEAR::MERCPROFILEGEAR()
{
	clearInventory();
	initialize();
}

// Assignment operator
MERCPROFILEGEAR& MERCPROFILEGEAR::operator=(const MERCPROFILEGEAR& src) {
    if (this != &src) {
		memcpy(this, &src, SIZEOF_MERCPROFILEGEAR_POD);
		inv = src.inv;
		iStatus = src.iStatus;
		iDrop = src.iDrop;
		iNumber = src.iNumber;
		lbe = src.lbe;
		lStatus = src.lStatus;
		invCnt = src.invCnt;
		lbeCnt = src.lbeCnt;
    }
	return *this;
}

// Destructor
MERCPROFILEGEAR::~MERCPROFILEGEAR() {
}

// Initialize the soldier.  
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILEGEAR::initialize() {
	memset( this, 0, SIZEOF_MERCPROFILEGEAR_POD);
	clearInventory();
}

void MERCPROFILEGEAR::clearInventory() {
	//ADB these really should be defines
	invCnt = 55;
	lbeCnt = 5;

	inv.clear();
	iStatus.clear();
	iDrop.clear();
	iNumber.clear();

	inv.resize(invCnt);
	iStatus.resize(invCnt);
	iDrop.resize(invCnt);
	iNumber.resize(invCnt);

	lbe.clear();
	lStatus.clear();

	lbe.resize(lbeCnt);
	lStatus.resize(lbeCnt);
}

// ----------------------------------------
// New inventory handling code.
// ----------------------------------------

Inventory::Inventory(int slotCount) {
	inv.resize(slotCount);//calls the OBJECTTYPE constructor, will work with non POD data
	bNewItemCount.resize(slotCount);
	bNewItemCycleCount.resize(slotCount);
};

Inventory::Inventory(const Inventory& src) {
	inv = src.inv;
	bNewItemCount = src.bNewItemCount;
	bNewItemCycleCount = src.bNewItemCycleCount;
}

unsigned int Inventory::size() const {
	return inv.size();
}

// Assignment operator
Inventory& Inventory::operator=(const Inventory& src)
{
	if (this != &src) {
		inv = src.inv;
		bNewItemCount = src.bNewItemCount;
 		bNewItemCycleCount = src.bNewItemCycleCount;
   }
    return *this;
}

Inventory::~Inventory() {
	inv.clear();
	bNewItemCount.clear();
	bNewItemCycleCount.clear();
};

OBJECTTYPE& Inventory::operator [] (unsigned int idx)
{
	if (idx >= inv.size()) {
		inv.resize(idx+1);
		bNewItemCount.resize(idx+1);
		bNewItemCycleCount.resize(idx+1);
		DebugBreakpoint();
	}
	return inv[idx];
};

void Inventory::clear() {
	int size = inv.size();
	inv.clear();
	bNewItemCount.clear();
	bNewItemCycleCount.clear();

	inv.resize(size);
	bNewItemCount.resize(size);
	bNewItemCycleCount.resize(size);
};

// ----------------------------------------

void STRUCT_TimeCounters::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	this->UpdateCounter = src.UpdateCounter;
	this->DamageCounter = src.DamageCounter;
	this->ReloadCounter = src.ReloadCounter;
	this->FlashSelCounter = src.FlashSelCounter;
	this->AICounter = src.AICounter;
	this->FadeCounter = src.FadeCounter;
	this->BlinkSelCounter = src.BlinkSelCounter;
	this->PortraitFlashCounter = src.PortraitFlashCounter;
	this->NextTileCounter = src.NextTileCounter;
	this->PanelAnimateCounter = src.PanelAnimateCounter;
}

void STRUCT_Drugs::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	for (int x = 0; x < 2; ++x)
	{
		this->bFutureDrugEffect[x] = src.bFutureDrugEffect[x];						// value to represent effect of a needle
		this->bDrugEffectRate[x] = src.bDrugEffectRate[x];							// represents rate of increase and decrease of effect
		this->bDrugEffect[x] = src.bDrugEffect[x];									// value that affects AP & morale calc ( -ve is poorly )
		this->bDrugSideEffectRate[x] = src.bDrugSideEffectRate[x];					// duration of negative AP and morale effect
		this->bDrugSideEffect[x] = src.bDrugSideEffect[x];							// duration of negative AP and morale effect
		this->bTimesDrugUsedSinceSleep[x] = src.bTimesDrugUsedSinceSleep[x];
	}
}

void STRUCT_TimeChanges::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	this->uiChangeLevelTime = src.uiChangeLevelTime;
	this->uiChangeHealthTime = src.uiChangeHealthTime;
	this->uiChangeStrengthTime = src.uiChangeStrengthTime;
	this->uiChangeDexterityTime = src.uiChangeDexterityTime;
	this->uiChangeAgilityTime = src.uiChangeAgilityTime;
	this->uiChangeWisdomTime = src.uiChangeWisdomTime;
	this->uiChangeLeadershipTime = src.uiChangeLeadershipTime;
	this->uiChangeMarksmanshipTime = src.uiChangeMarksmanshipTime;
	this->uiChangeExplosivesTime = src.uiChangeExplosivesTime;
	this->uiChangeMedicalTime = src.uiChangeMedicalTime;
	this->uiChangeMechanicalTime = src.uiChangeMechanicalTime;
}

void STRUCT_Flags::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	this->ZipperFlag = FALSE;
	this->DropPackFlag = FALSE;

	this->bHasKeys = src.bHasKeys;			// allows AI controlled dudes to open locked doors
	this->fHitByGasFlags = src.fHitByGasFlags;						// flags
	this->fIsSoldierMoving = src.fIsSoldierMoving;							// ie.  Record time is on
	this->fIsSoldierDelayed = src.fIsSoldierDelayed;						//Is the soldier delayed Soldier
	this->fSoldierUpdatedFromNetwork = src.fSoldierUpdatedFromNetwork;
	this->fChangingStanceDueToSuppression = src.fChangingStanceDueToSuppression;
	this->fCheckForNewlyAddedItems = src.fCheckForNewlyAddedItems;
	this->fCheckForNewlyAddedItems = src.bEndDoorOpenCode;
	this->fMuzzleFlash = src.fMuzzleFlash;
	this->fSoldierWasMoving = src.fSoldierWasMoving;
	this->fSayAmmoQuotePending = src.fSayAmmoQuotePending;
	this->fDontUnsetLastTargetFromTurn = src.fDontUnsetLastTargetFromTurn;
	this->fUseMoverrideMoveSpeed = src.fUseMoverrideMoveSpeed;
	this->fDieSoundUsed = src.fDieSoundUsed;
	this->fUseLandingZoneForArrival = src.fUseLandingZoneForArrival;
	this->fFallClockwise = src.fFallClockwise;
	this->fComplainedThatTired = src.fComplainedThatTired;
	this->fRTInNonintAnim = src.fRTInNonintAnim;
	this->fDoingExternalDeath = src.fDoingExternalDeath;
	this->fPastXDest = src.fPastXDest;
	this->fPastYDest = src.fPastYDest;
	this->fBlockedByAnotherMerc = src.fBlockedByAnotherMerc;
	this->fReactingFromBeingShot = src.fReactingFromBeingShot;
	this->fContractPriceHasIncreased = src.fContractPriceHasIncreased;
	this->fFixingSAMSite = src.fFixingSAMSite;
	this->fFixingRobot = src.fFixingRobot;
	this->fSignedAnotherContract = src.fSignedAnotherContract;
	this->fDontChargeTurningAPs = src.fDontChargeTurningAPs;
	this->fMercCollapsedFlag = src.fMercCollapsedFlag;
	this->fDoneAssignmentAndNothingToDoFlag = src.fDoneAssignmentAndNothingToDoFlag;
	this->fMercAsleep = src.fMercAsleep;
	this->fDontChargeAPsForStanceChange = src.fDontChargeAPsForStanceChange;
	this->fStopPendingNextTile = src.fStopPendingNextTile;
	this->fForceShade = src.fForceShade;
	this->fForceRenderColor = src.fForceRenderColor;
	this->fForceNoRenderPaletteCycle = src.fForceNoRenderPaletteCycle;
	this->bTurningFromPronePosition = src.bTurningFromPronePosition;
	this->fFlashPortrait = src.fFlashPortrait;
	this->fBeginFade = src.fBeginFade;
	this->fDontChargeReadyAPs = src.fDontChargeReadyAPs;
	this->fPrevInWater = src.fPrevInWater;
	this->fGoBackToAimAfterHit = src.fGoBackToAimAfterHit;
	this->fDisplayDamage = src.fDisplayDamage;
	this->fUIMovementFast = src.fUIMovementFast;
	this->fDeadSoundPlayed = src.fDeadSoundPlayed;
	this->fClosePanel = src.fClosePanel;
	this->fClosePanelToDie = src.fClosePanelToDie;
	this->fDeadPanel = src.fDeadPanel;
	this->fOpenPanel = src.fOpenPanel;
	this->fTryingToFall = src.fTryingToFall;
	this->fCloseCall = src.fCloseCall;
	this->fDoSpread = src.fDoSpread;
	this->fForcedToStayAwake = src.fForcedToStayAwake;				// forced by player to stay awake, reset to false, the moment they are set to rest or sleep
	this->fDelayedMovement = src.fDelayedMovement;
	this->fReloading = src.fReloading;
	this->fPauseAim = src.fPauseAim;
	this->fInMissionExitNode = src.fInMissionExitNode;
	this->fTurnInProgress = src.fTurnInProgress;
	this->fIntendedTarget = src.fIntendedTarget; // intentionally shot?
	this->fContinueMoveAfterStanceChange = src.fContinueMoveAfterStanceChange;
	this->fHoldAttackerUntilDone = src.fHoldAttackerUntilDone;
	this->fWarnedAboutBleeding = src.fWarnedAboutBleeding;
	this->fDyingComment = src.fDyingComment;
	this->fNoAPToFinishMove = src.fNoAPToFinishMove;
	this->fPausedMove = src.fPausedMove;
	this->fUIdeadMerc = src.fUIdeadMerc;				// UI Flags for removing a newly dead merc
	this->fUInewMerc = src.fUInewMerc;					// UI Flags for adding newly created merc ( panels, etc )
	this->fUICloseMerc = src.fUICloseMerc;				// UI Flags for closing panels
	this->fUIFirstTimeNOAP = src.fUIFirstTimeNOAP;		// UI Flag for diming guys when no APs ( dirty flags )
	this->fUIFirstTimeUNCON = src.fUIFirstTimeUNCON;	// UI FLAG For unconscious dirty
	this->fBetweenSectors = src.fBetweenSectors;	//set when the group isn't actually in a sector.
	this->fPauseAllAnimation = src.fPauseAllAnimation;
	this->fTurningToShoot = src.fTurningToShoot;
	this->fTurningToFall = src.fTurningToFall;
	this->fTurningUntilDone = src.fTurningUntilDone;
	this->fGettingHit = src.fGettingHit;
	this->fInNonintAnim = src.fInNonintAnim;
	this->fFlashLocator = src.fFlashLocator;
	this->fShowLocator = src.fShowLocator;
	this->autofireLastStep = src.autofireLastStep;
	this->lastFlankLeft = src.lastFlankLeft;
	this->uiStatusFlags = src.uiStatusFlags;
}

void STRUCT_Statistics::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	this->ubSkillTrait1 = src.ubSkillTrait1;
	this->ubSkillTrait2 = src.ubSkillTrait2;
	this->bDexterity = src.bDexterity;		// dexterity (hand coord) value
	this->bWisdom = src.bWisdom;
	this->bExpLevel = src.bExpLevel;		// general experience level
	this->bLife = src.bLife;				// current life (hit points or health)
	this->bAgility = src.bAgility;			// agility (speed) value
	this->bStrength = src.bStrength;
	this->bMechanical = src.bMechanical;
	this->bLifeMax = src.bLifeMax;			// maximum life for this merc
	this->bMedical = src.bMedical;
	this->bMarksmanship = src.bMarksmanship;
	this->bScientific = src.bScientific;
	this->bLeadership = src.bLeadership;
	this->bExplosive = src.bExplosive;
}

void STRUCT_AIData::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	memcpy( &(this->bOppList), &(src.bOppList), sizeof(INT8) * MAX_NUM_SOLDIERS ); // AI knowledge database
	memcpy( &(this->sPatrolGrid), &(src.usPatrolGrid), sizeof(INT16) * MAXPATROLGRIDS );// AI list for ptr->orders==PATROL
	this->bLastAction = src.bLastAction;
	this->bAction = src.bAction;
	this->usActionData = src.usActionData;
	this->bNextAction = src.bNextAction;
	this->usNextActionData = src.usNextActionData;
	this->bActionInProgress = src.bActionInProgress;
	this->bAlertStatus = src.bAlertStatus;
	this->bOppCnt = src.bOppCnt;
	this->bNeutral = src.bNeutral;
	this->bNewSituation = src.bNewSituation;
	this->bNextTargetLevel = src.bNextTargetLevel;
	this->bOrders = src.bOrders;
	this->bAttitude = src.bAttitude;
	this->bUnderFire = src.bUnderFire;
	this->bShock = src.bShock;
	this->bUnderEscort = src.bUnderEscort;
	this->bBypassToGreen = src.bBypassToGreen;
	this->ubLastMercToRadio = src.ubLastMercToRadio;
	this->bDominantDir = src.bDominantDir;				// AI main direction to face...
	this->bPatrolCnt = src.bPatrolCnt;					// number of patrol gridnos
	this->bNextPatrolPnt = src.bNextPatrolPnt;			// index to next patrol gridno
	this->sNoiseGridno = src.sNoiseGridno;
	this->ubNoiseVolume = src.ubNoiseVolume;
	this->bLastAttackHit = src.bLastAttackHit;
	this->ubXRayedBy = src.ubXRayedBy;
	this->dHeightAdjustment = src.dHeightAdjustment;
	this->bMorale = src.bMorale;
	this->bTeamMoraleMod = src.bTeamMoraleMod;
	this->bTacticalMoraleMod = src.bTacticalMoraleMod;
	this->bStrategicMoraleMod = src.bStrategicMoraleMod;
	this->bAIMorale = src.bAIMorale;
	this->ubPendingAction = src.ubPendingAction;
	this->ubPendingActionAnimCount = src.ubPendingActionAnimCount;
	this->uiPendingActionData1 = src.uiPendingActionData1;
	this->sPendingActionData2 = src.sPendingActionData2;
	this->bPendingActionData3 = src.bPendingActionData3;
	this->ubDoorHandleCode = src.ubDoorHandleCode;
	this->uiPendingActionData4 = src.uiPendingActionData4;
	this->bInterruptDuelPts = src.bInterruptDuelPts;
	this->bPassedLastInterrupt = src.bPassedLastInterrupt;
	this->bIntStartAPs = src.bIntStartAPs;
	this->bMoved = src.bMoved;
	this->bHunting = src.bHunting;
	this->ubLastCall = src.ubLastCall;
	this->ubCaller = src.ubCaller;
	this->sCallerGridNo = src.sCallerGridNo;
	this->bCallPriority = src.bCallPriority;
	this->bCallActedUpon = src.bCallActedUpon;
	this->bFrenzied = src.bFrenzied;
	this->bNormalSmell = src.bNormalSmell;
	this->bMonsterSmell = src.bMonsterSmell;
	this->bMobility = src.bMobility;
	this->bRTPCombat = src.bRTPCombat;
	this->fAIFlags = src.fAIFlags;
	this->bAimTime = src.bAimTime;
	this->bShownAimTime = src.bShownAimTime;
}

void STRUCT_Pathing::ConvertFrom_101_To_102(const OLDSOLDIERTYPE_101& src)
{
	memcpy( &(this->usPathingData), &(src.usPathingData), sizeof(UINT16) * MAX_PATH_LIST_SIZE );
	this->bDesiredDirection = src.bDesiredDirection;
	this->sDestXPos = src.sDestXPos;
	this->sDestYPos = src.sDestYPos;
	//this->sDesiredDest = src.sDesiredDest;//apparently not used
	this->sDestination = src.sDestination;
	this->sFinalDestination = src.sFinalDestination;
	this->bLevel = src.bLevel;
	this->bStopped = src.bStopped;
	this->bNeedToLook = src.bNeedToLook;
	this->usPathDataSize = src.usPathDataSize;
	this->usPathIndex = src.usPathIndex;
	this->sBlackList = src.sBlackList;
	this->bPathStored = src.bPathStored;	// good for AI to reduct redundancy
}

// Conversion operator
SOLDIERTYPE& SOLDIERTYPE::operator=(const OLDSOLDIERTYPE_101& src)
{
    if ((void*)this != (void*)&src) {
		//member classes
		aiData.ConvertFrom_101_To_102(src);
		flags.ConvertFrom_101_To_102(src);
		timeChanges.ConvertFrom_101_To_102(src);
		timeCounters.ConvertFrom_101_To_102(src);
		drugs.ConvertFrom_101_To_102(src);
		stats.ConvertFrom_101_To_102(src);
		pathing.ConvertFrom_101_To_102(src);
		inv = src.inv;

		//arrays
		memcpy( &(this->name), &(src.name), sizeof(CHAR16) * 10 );
		memcpy( &(this->pShades), &(src.pShades), sizeof(UINT16) * NUM_SOLDIER_SHADES ); // Shading tables
		memcpy( &(this->pGlowShades), &(src.pGlowShades), sizeof(UINT16) * 20 ); //
		memcpy( &(this->pEffectShades), &(src.pEffectShades), sizeof(UINT16) * NUM_SOLDIER_EFFECTSHADES ); // Shading tables for effects
		memcpy( &(this->sSpreadLocations), &(src.sSpreadLocations), sizeof(INT16) * MAX_BURST_SPREAD_TARGETS );
		memcpy( &(this->usFrontArcFullTileList), &(src.usFrontArcFullTileList), sizeof(UINT16) * MAX_FULLTILE_DIRECTIONS );
		memcpy( &(this->usFrontArcFullTileGridNos), &(src.usFrontArcFullTileGridNos), sizeof(INT16) * MAX_FULLTILE_DIRECTIONS );
		memcpy( &(this->HeadPal), &(src.HeadPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->PantsPal), &(src.PantsPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->VestPal), &(src.VestPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->SkinPal), &(src.SkinPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->MiscPal), &(src.MiscPal), sizeof(PaletteRepID) );	// 30
		this->sLastTwoLocations[0] = src.sLastTwoLocations[0];
		this->sLastTwoLocations[1] = src.sLastTwoLocations[1];

		this->ubID = src.ubID;
		this->bReserved1 = src.bReserved1;

		this->ubBodyType = src.ubBodyType;
		this->bActionPoints = src.bActionPoints;
		this->bInitialActionPoints = src.bInitialActionPoints;


		this->pKeyRing = src.pKeyRing;
		this->bOldLife = src.bOldLife;			// life at end of last turn, recorded for monster AI
		this->bInSector = src.bInSector;
		this->bFlashPortraitFrame = src.bFlashPortraitFrame;
		this->sFractLife = src.sFractLife;		// fraction of life pts (in hundreths)
		this->bBleeding = src.bBleeding;		// blood loss control variable
		this->bBreath = src.bBreath;			// current breath value
		this->bBreathMax = src.bBreathMax;   // max breath, affected by fatigue/sleep
		this->bStealthMode = src.bStealthMode;

		this->sBreathRed = src.sBreathRed;			// current breath value

		this->ubWaitActionToDo = src.ubWaitActionToDo;
		this->ubInsertionDirection = src.ubInsertionDirection;
		this->bGunType = src.bGunType;
		this->ubOppNum = src.ubOppNum;
		this->bLastRenderVisibleValue = src.bLastRenderVisibleValue;
		this->ubAttackingHand = src.ubAttackingHand;
		this->sWeightCarriedAtTurnStart = src.sWeightCarriedAtTurnStart;

		this->bVisible = src.bVisible;			// to render or not to render...

		this->bActive = src.bActive;
		this->bTeam = src.bTeam;				// Team identifier

		this->ubGroupID = src.ubGroupID;		//the movement group the merc is currently part of.
		this->ubMovementNoiseHeard = src.ubMovementNoiseHeard;// 8 flags by direction


		this->dXPos = src.dXPos;
		this->dYPos = src.dYPos;
		this->dOldXPos = src.dOldXPos;
		this->dOldYPos = src.dOldYPos;
		this->sInitialGridNo = src.sInitialGridNo;
		this->sGridNo = src.sGridNo;
		this->ubDirection = src.ubDirection;
		this->sHeightAdjustment = src.sHeightAdjustment;
		this->sDesiredHeight = src.sDesiredHeight;
		this->sTempNewGridNo = src.sTempNewGridNo;					// New grid no for advanced animations
		this->sRoomNo = src.sRoomNo;
		this->bOverTerrainType = src.bOverTerrainType;
		this->bOldOverTerrainType = src.bOldOverTerrainType;
		this->bCollapsed = src.bCollapsed;					// collapsed due to being out of APs
		this->bBreathCollapsed = src.bBreathCollapsed;					// collapsed due to being out of APs


		this->ubDesiredHeight = src.ubDesiredHeight;
		this->usPendingAnimation = src.usPendingAnimation;
		this->ubPendingStanceChange = src.ubPendingStanceChange;
		this->usAnimState = src.usAnimState;

		this->uiAIDelay = src.uiAIDelay;
		this->sReloadDelay = src.sReloadDelay;
		this->ubAttackerID = src.ubAttackerID;
		this->ubPreviousAttackerID = src.ubPreviousAttackerID;

		this->sInsertionGridNo = src.sInsertionGridNo;


		this->AnimCache = src.AnimCache; // will be 9 bytes once changed to pointers

		this->bSide = src.bSide;
		this->bViewRange = src.bViewRange;
		this->bNewOppCnt = src.bNewOppCnt;
		this->bService = src.bService;		// first aid, or other time consuming process

		this->usAniCode = src.usAniCode;
		this->usAniFrame = src.usAniFrame;
		this->sAniDelay = src.sAniDelay;

		this->ubDelayedMovementCauseMerc = src.ubDelayedMovementCauseMerc;
		this->sDelayedMovementCauseGridNo = src.sDelayedMovementCauseGridNo;
		this->sReservedMovementGridNo = src.sReservedMovementGridNo;


		this->sTargetGridNo = src.sTargetGridNo;
		this->bTargetLevel = src.bTargetLevel;
		this->bTargetCubeLevel = src.bTargetCubeLevel;
		this->sLastTarget = src.sLastTarget;
		this->bTilesMoved = src.bTilesMoved;
		this->dNextBleed = src.dNextBleed;

		this->ubTilesMovedPerRTBreathUpdate = src.ubTilesMovedPerRTBreathUpdate;
		this->usLastMovementAnimPerRTBreathUpdate = src.usLastMovementAnimPerRTBreathUpdate;

		this->sLocatorFrame = src.sLocatorFrame;
		this->iFaceIndex = src.iFaceIndex;


		this->p8BPPPalette = src.p8BPPPalette; // 4
		this->p16BPPPalette = src.p16BPPPalette;
		this->pCurrentShade = src.pCurrentShade;
		this->ubFadeLevel = src.ubFadeLevel;
		this->ubServiceCount = src.ubServiceCount;
		this->ubServicePartner = src.ubServicePartner;
		this->pThrowParams = src.pThrowParams;
		this->bReverse = src.bReverse;
		this->pLevelNode = src.pLevelNode;
		this->pExternShadowLevelNode = src.pExternShadowLevelNode;
		this->pRoofUILevelNode = src.pRoofUILevelNode;

		this->pBackGround = src.pBackGround;
		this->pZBackground = src.pZBackground;
		this->usUnblitX = src.usUnblitX;
		this->usUnblitY = src.usUnblitY;
		this->usUnblitWidth = src.usUnblitWidth;
		this->usUnblitHeight = src.usUnblitHeight;

		this->ubStrategicInsertionCode = src.ubStrategicInsertionCode;
		this->usStrategicInsertionData = src.usStrategicInsertionData;


		this->iLight = src.iLight;
		this->iMuzFlash = src.iMuzFlash;
		this->bMuzFlashCount = src.bMuzFlashCount;


		this->sX = src.sX;
		this->sY = src.sY;

		this->usOldAniState = src.usOldAniState;
		this->sOldAniCode = src.sOldAniCode;

		this->bBulletsLeft = src.bBulletsLeft;
		this->ubSuppressionPoints = src.ubSuppressionPoints;

		this->uiTimeOfLastRandomAction = src.uiTimeOfLastRandomAction;
		this->usLastRandomAnim = src.usLastRandomAnim;

		this->usAnimSurface = src.usAnimSurface;
		this->sZLevel = src.sZLevel;

		this->sWalkToAttackGridNo = src.sWalkToAttackGridNo;
		this->sWalkToAttackWalkToCost = src.sWalkToAttackWalkToCost;


		this->sLocatorOffX = src.sLocatorOffX;
		this->sLocatorOffY = src.sLocatorOffY;
		this->pForcedShade = src.pForcedShade;

		this->bDisplayDamageCount = src.bDisplayDamageCount;
		this->sDamage = src.sDamage;
		this->sDamageX = src.sDamageX;
		this->sDamageY = src.sDamageY;
		this->bDamageDir = src.bDamageDir;
		this->bDoBurst = src.bDoBurst;
		this->usUIMovementMode = src.usUIMovementMode;
		this->bUIInterfaceLevel = src.bUIInterfaceLevel;

		this->ubProfile = src.ubProfile;
		this->ubQuoteRecord = src.ubQuoteRecord;
		this->ubQuoteActionID = src.ubQuoteActionID;
		this->ubBattleSoundID = src.ubBattleSoundID;

		this->ubClosePanelFrame = src.ubClosePanelFrame;
		this->ubDeadPanelFrame = src.ubDeadPanelFrame;
		this->bOpenPanelFrame = src.bOpenPanelFrame;

		this->sPanelFaceX = src.sPanelFaceX;
		this->sPanelFaceY = src.sPanelFaceY;

		this->bNumHitsThisTurn = src.bNumHitsThisTurn;
		this->usQuoteSaidFlags = src.usQuoteSaidFlags;
		this->bLastSkillCheck = src.bLastSkillCheck;
		this->ubSkillCheckAttempts = src.ubSkillCheckAttempts;

		this->bVocalVolume = src.bVocalVolume;	// verbal sounds need to differ in volume

		this->bStartFallDir = src.bStartFallDir;

		this->ubPendingDirection = src.ubPendingDirection;
		this->uiAnimSubFlags = src.uiAnimSubFlags;

		this->bAimShotLocation = src.bAimShotLocation;
		this->ubHitLocation = src.ubHitLocation;



		this->ubPlannedUIAPCost = src.ubPlannedUIAPCost;
		this->sPlannedTargetX = src.sPlannedTargetX;
		this->sPlannedTargetY = src.sPlannedTargetY;

		this->sStartGridNo = src.sStartGridNo;
		this->sEndGridNo = src.sEndGridNo;
		this->sForcastGridno = src.sForcastGridno;
		this->sZLevelOverride = src.sZLevelOverride;
		this->bMovedPriorToInterrupt = src.bMovedPriorToInterrupt;
		this->iEndofContractTime = src.iEndofContractTime;				// time, in global time(resolution, minutes) that merc will leave, or if its a M.E.R.C. merc it will be set to -1.  -2 for NPC and player generated
		this->iStartContractTime = src.iStartContractTime;
		this->iTotalContractLength = src.iTotalContractLength;			// total time of AIM mercs contract	or the time since last paid for a M.E.R.C. merc
		this->iNextActionSpecialData = src.iNextActionSpecialData;		// AI special action data record for the next action
		this->ubWhatKindOfMercAmI = src.ubWhatKindOfMercAmI;			//Set to the type of character it is
		this->bAssignment = src.bAssignment;							// soldiers current assignment
		this->bOldAssignment = src.bOldAssignment;						// old assignment, for autosleep purposes
		this->bTrainStat = src.bTrainStat;								// current stat soldier is training
		this->sSectorX = src.sSectorX;									// X position on the Stategic Map
		this->sSectorY = src. sSectorY;									// Y position on the Stategic Map
		this->bSectorZ = src.bSectorZ;									// Z sector location
		this->iVehicleId = src.iVehicleId;								// the id of the vehicle the char is in
		this->pMercPath = src.pMercPath;								//Path Structure
		this->usMedicalDeposit = src.usMedicalDeposit;         // is there a medical deposit on merc
		this->usLifeInsurance = src.usLifeInsurance;          // is there life insurance taken out on merc


		this->uiStartMovementTime = src.uiStartMovementTime;				// the time since the merc first started moving
		this->uiOptimumMovementTime = src.uiOptimumMovementTime;			// everytime in ececute overhead the time for the current ani will be added to this total
		this->usLastUpdateTime = src.usLastUpdateTime ;					// The last time the soldier was in ExecuteOverhead

		this->uiSoldierUpdateNumber = src.uiSoldierUpdateNumber;
		this->ubSoldierUpdateType = src.ubSoldierUpdateType;

		this->iStartOfInsuranceContract = src.iStartOfInsuranceContract;
		this->uiLastAssignmentChangeMin = src.uiLastAssignmentChangeMin;		// timestamp of last assignment change in minutes
		this->iTotalLengthOfInsuranceContract = src.iTotalLengthOfInsuranceContract;

		this->ubSoldierClass = src.ubSoldierClass;									//admin, elite, troop (creature types?)
		this->ubAPsLostToSuppression = src.ubAPsLostToSuppression;
		this->ubSuppressorID = src.ubSuppressorID;

		this->ubDesiredSquadAssignment = src.ubDesiredSquadAssignment;
		this->ubNumTraversalsAllowedToMerge = src.ubNumTraversalsAllowedToMerge;

		this->usPendingAnimation2 = src.usPendingAnimation2;
		this->ubCivilianGroup = src.ubCivilianGroup;

		this->uiUniqueSoldierIdValue = src.uiUniqueSoldierIdValue; // the unique value every instance of a soldier gets - 1 is the first valid value

		this->ubScheduleID = src.ubScheduleID;
		this->sEndDoorOpenCodeData = src.sEndDoorOpenCodeData;
		this->bBlockedByAnotherMercDirection = src.bBlockedByAnotherMercDirection;
		this->usAttackingWeapon = src.usAttackingWeapon;
		this->bWeaponMode = src.bWeaponMode;
		this->ubTargetID = src.ubTargetID;
		this->bAIScheduleProgress = src.bAIScheduleProgress;
		this->sOffWorldGridNo = src.sOffWorldGridNo;
		this->pAniTile = src.pAniTile;
		this->bCamo = src.bCamo;
		this->sAbsoluteFinalDestination = src.sAbsoluteFinalDestination;
		this->ubHiResDirection = src.ubHiResDirection;
		this->ubHiResDesiredDirection = src.ubHiResDesiredDirection;
		this->ubLastFootPrintSound = src.ubLastFootPrintSound;
		this->bVehicleID = src.bVehicleID;
		this->bMovementDirection = src.bMovementDirection;
		this->sOldGridNo = src.sOldGridNo;
		this->usDontUpdateNewGridNoOnMoveAnimChange = src.usDontUpdateNewGridNoOnMoveAnimChange;
		this->sBoundingBoxWidth = src.sBoundingBoxWidth;
		this->sBoundingBoxHeight = src.sBoundingBoxHeight;
		this->sBoundingBoxOffsetX = src.sBoundingBoxOffsetX;
		this->sBoundingBoxOffsetY = src.sBoundingBoxOffsetY;
		this->uiTimeSameBattleSndDone = src.uiTimeSameBattleSndDone;
		this->bOldBattleSnd = src.bOldBattleSnd;
		this->iBurstSoundID = src.iBurstSoundID;
		this->bSlotItemTakenFrom = src.bSlotItemTakenFrom;
		this->ubAutoBandagingMedic = src.ubAutoBandagingMedic;
		this->ubRobotRemoteHolderID = src.ubRobotRemoteHolderID;
		this->uiTimeOfLastContractUpdate = src.uiTimeOfLastContractUpdate;
		this->bTypeOfLastContract = src.bTypeOfLastContract;
		this->bTurnsCollapsed = src.bTurnsCollapsed;
		this->bSleepDrugCounter = src.bSleepDrugCounter;
		this->ubMilitiaKills = src.ubMilitiaKills;


		this->bBlindedCounter = src.bBlindedCounter;

		this->ubHoursOnAssignment = src.ubHoursOnAssignment;						// used for assignments handled only every X hours

		this->ubMercJustFired = src.ubMercJustFired;   // the merc was just fired..there may be dialogue events occuring, this flag will prevent any interaction with contracts
																								// until after the merc leaves
		this->ubTurnsUntilCanSayHeardNoise = src.ubTurnsUntilCanSayHeardNoise;
		this->usQuoteSaidExtFlags = src.usQuoteSaidExtFlags;

		this->sContPathLocation = src.sContPathLocation;
		this->bGoodContPath = src.bGoodContPath;
		this->ubPendingActionInterrupted = src.ubPendingActionInterrupted;
		this->bNoiseLevel = src.bNoiseLevel;
		this->bRegenerationCounter = src.bRegenerationCounter;
		this->bRegenBoostersUsedToday = src.bRegenBoostersUsedToday;
		this->bNumPelletsHitBy = src.bNumPelletsHitBy;
		this->sSkillCheckGridNo = src.sSkillCheckGridNo;
		this->ubLastEnemyCycledID = src.ubLastEnemyCycledID;

		this->ubPrevSectorID = src.ubPrevSectorID;
		this->ubNumTilesMovesSinceLastForget = src.ubNumTilesMovesSinceLastForget;
		this->bTurningIncrement = src.bTurningIncrement;
		this->uiBattleSoundID = src.uiBattleSoundID;


		this->usValueGoneUp = src.usValueGoneUp;
		this->ubNumLocateCycles = src.ubNumLocateCycles;
		this->ubDelayedMovementFlags = src.ubDelayedMovementFlags;
		this->ubCTGTTargetID = src.ubCTGTTargetID;

		this->uiMercChecksum = src.uiMercChecksum;

		this->bCurrentCivQuote = src.bCurrentCivQuote;
		this->bCurrentCivQuoteDelta = src.bCurrentCivQuoteDelta;
		this->ubMiscSoldierFlags = src.ubMiscSoldierFlags;
		this->ubReasonCantFinishMove = src.ubReasonCantFinishMove;

		this->sLocationOfFadeStart = src.sLocationOfFadeStart;
		this->bUseExitGridForReentryDirection = src.bUseExitGridForReentryDirection;

		this->uiTimeSinceLastSpoke = src.uiTimeSinceLastSpoke;
		this->ubContractRenewalQuoteCode = src.ubContractRenewalQuoteCode;
		this->sPreTraversalGridNo = src.sPreTraversalGridNo;
		this->uiXRayActivatedTime = src.uiXRayActivatedTime;
		this->bTurningFromUI = src.bTurningFromUI;
		this->bPendingActionData5 = src.bPendingActionData5;

		this->bDelayedStrategicMoraleMod = src.bDelayedStrategicMoraleMod;
		this->ubDoorOpeningNoise = src.ubDoorOpeningNoise;

		this->pGroup = src.pGroup;
		this->ubLeaveHistoryCode = src.ubLeaveHistoryCode;
		this->bOverrideMoveSpeed = src.bOverrideMoveSpeed;

		this->uiTimeSoldierWillArrive = src.uiTimeSoldierWillArrive;
		this->bVehicleUnderRepairID = src.bVehicleUnderRepairID;
		this->iTimeCanSignElsewhere = src.iTimeCanSignElsewhere;
		this->bHospitalPriceModifier = src.bHospitalPriceModifier;
		this->uiStartTimeOfInsuranceContract = src.uiStartTimeOfInsuranceContract;
		this->bCorpseQuoteTolerance = src.bCorpseQuoteTolerance;
		this->bDeafenedCounter = src.bDeafenedCounter;
		this->iPositionSndID = src.iPositionSndID;
		this->iTuringSoundID = src.iTuringSoundID;
		this->ubLastDamageReason = src.ubLastDamageReason;
		this->uiTimeSinceLastBleedGrunt = src.uiTimeSinceLastBleedGrunt;
		this->ubNextToPreviousAttackerID = src.ubNextToPreviousAttackerID;
		this->bDoAutofire = src.bDoAutofire;
		this->numFlanks = src.numFlanks;
		this->lastFlankSpot = src.lastFlankSpot;
		this->sniper = src.sniper;
		this->origDir = src.origDir;
		this->wornCamo = src.wornCamo;

		this->urbanCamo = src.urbanCamo;	// Madd: new camo types
		this->wornUrbanCamo = src.wornUrbanCamo;

		this->desertCamo = src.desertCamo;
		this->wornDesertCamo = src.wornDesertCamo;

		this->snowCamo = src.snowCamo;
		this->wornSnowCamo = src.wornSnowCamo;
    }
    return *this;
}

SOLDIERTYPE::~SOLDIERTYPE() {}

SOLDIERTYPE::SOLDIERTYPE() {
	initialize();
}

UINT32 SOLDIERTYPE::GetChecksum( )
{
	UINT32	uiChecksum = 1;
	UINT32	uiLoop;

	uiChecksum += (this->stats.bLife + 1);
	uiChecksum *= (this->stats.bLifeMax + 1);
	uiChecksum += (this->stats.bAgility + 1);
	uiChecksum *= (this->stats.bDexterity + 1);
	uiChecksum += (this->stats.bStrength + 1);
	uiChecksum *= (this->stats.bMarksmanship + 1);
	uiChecksum += (this->stats.bMedical + 1);
	uiChecksum *= (this->stats.bMechanical + 1);
	uiChecksum += (this->stats.bExplosive + 1);

	// put in some multipliers too!
	uiChecksum *= (this->stats.bExpLevel + 1);
	uiChecksum += (this->ubProfile + 1);

	for ( uiLoop = 0; uiLoop < this->inv.size(); uiLoop++ )
	{
		uiChecksum += this->inv[ uiLoop ].usItem;
		uiChecksum += this->inv[ uiLoop ].ubNumberOfObjects;
	}

	return( uiChecksum );
}


// Copy Constructor
SOLDIERTYPE::SOLDIERTYPE(const SOLDIERTYPE& src)
{
	memcpy(this, &src, SIZEOF_SOLDIERTYPE_POD);
	inv = src.inv;
	aiData = src.aiData;
	flags = src.flags;
	timeChanges = src.timeChanges;
	timeCounters = src.timeCounters;
	drugs = src.drugs;
	stats = src.stats;
	pathing = src.pathing;
}

// Assignment operator
SOLDIERTYPE& SOLDIERTYPE::operator=(const SOLDIERTYPE& src)
{
	if (this != &src) {
		memcpy(this, &src, SIZEOF_SOLDIERTYPE_POD);
		aiData = src.aiData;
		flags = src.flags;
		timeChanges = src.timeChanges;
		timeCounters = src.timeCounters;
		drugs = src.drugs;
		stats = src.stats;
		pathing = src.pathing;
		inv = src.inv;
    }
    return *this;
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void SOLDIERTYPE::initialize()
{
	memset( this, 0, SIZEOF_SOLDIERTYPE_POD);
	inv.clear();

	memset( &aiData, 0, sizeof(STRUCT_AIData) );
	memset( &flags, 0, sizeof(STRUCT_Flags) );
	memset( &timeChanges, 0, sizeof(STRUCT_TimeChanges) );
	memset( &timeCounters, 0, sizeof(STRUCT_TimeCounters) );
	memset( &drugs, 0, sizeof(STRUCT_Drugs) );
	memset( &stats, 0, sizeof(STRUCT_Statistics) );
	memset( &pathing, 0, sizeof(STRUCT_Pathing) );
}



// Ugly temporary solution
//
// These two functions map the "old" style inventory (fixed array of ) to the new (a flexibly sized vector).
// If you change names or eliminate some positions or such you need to change these.
// Eventually the need for these functions will disappear.

UINT32 OLDSOLDIERTYPE_101::GetChecksum()
{
	UINT32	uiChecksum = 1;
	UINT32	uiLoop;

	uiChecksum += (this->bLife + 1);
	uiChecksum *= (this->bLifeMax + 1);
	uiChecksum += (this->bAgility + 1);
	uiChecksum *= (this->bDexterity + 1);
	uiChecksum += (this->bStrength + 1);
	uiChecksum *= (this->bMarksmanship + 1);
	uiChecksum += (this->bMedical + 1);
	uiChecksum *= (this->bMechanical + 1);
	uiChecksum += (this->bExplosive + 1);

	// put in some multipliers too!
	uiChecksum *= (this->bExpLevel + 1);
	uiChecksum += (this->ubProfile + 1);

	for ( uiLoop = 0; uiLoop < NUM_ORIGINAL_INV_SLOTS; uiLoop++ )
	{
		uiChecksum += this->inv[ uiLoop ].usItem;
		uiChecksum += this->inv[ uiLoop ].ubNumberOfObjects;
	}

	return( uiChecksum );
}


void OLDSOLDIERTYPE_101::CopyOldInventoryToNew()
{
	// Do not use a loop in case the new inventory slots are arranged differently than the old
	inv[HELMETPOS] = DO_NOT_USE_Inv[OldInventory::HELMETPOS];
	inv[VESTPOS] = DO_NOT_USE_Inv[OldInventory::VESTPOS];
	inv[LEGPOS] = DO_NOT_USE_Inv[OldInventory::LEGPOS];
	inv[HEAD1POS] = DO_NOT_USE_Inv[OldInventory::HEAD1POS];
	inv[HEAD2POS] = DO_NOT_USE_Inv[OldInventory::HEAD2POS];
	inv[HANDPOS] = DO_NOT_USE_Inv[OldInventory::HANDPOS];
	inv[SECONDHANDPOS] = DO_NOT_USE_Inv[OldInventory::SECONDHANDPOS];
	inv[BIGPOCK1POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK1POS];
	inv[BIGPOCK2POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK2POS];
	inv[BIGPOCK3POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK3POS];
	inv[BIGPOCK4POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK4POS];
	inv[SMALLPOCK1POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK1POS];
	inv[SMALLPOCK2POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK2POS];
	inv[SMALLPOCK3POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK3POS];
	inv[SMALLPOCK4POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK4POS];
	inv[SMALLPOCK5POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK5POS];
	inv[SMALLPOCK6POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK6POS];
	inv[SMALLPOCK7POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK7POS];
	inv[SMALLPOCK8POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK8POS];

	bNewItemCount[HELMETPOS] = DO_NOT_USE_bNewItemCount[OldInventory::HELMETPOS];
	bNewItemCount[VESTPOS] = DO_NOT_USE_bNewItemCount[OldInventory::VESTPOS];
	bNewItemCount[LEGPOS] = DO_NOT_USE_bNewItemCount[OldInventory::LEGPOS];
	bNewItemCount[HEAD1POS] = DO_NOT_USE_bNewItemCount[OldInventory::HEAD1POS];
	bNewItemCount[HEAD2POS] = DO_NOT_USE_bNewItemCount[OldInventory::HEAD2POS];
	bNewItemCount[HANDPOS] = DO_NOT_USE_bNewItemCount[OldInventory::HANDPOS];
	bNewItemCount[SECONDHANDPOS] = DO_NOT_USE_bNewItemCount[OldInventory::SECONDHANDPOS];
	bNewItemCount[BIGPOCK1POS] = DO_NOT_USE_bNewItemCount[OldInventory::BIGPOCK1POS];
	bNewItemCount[BIGPOCK2POS] = DO_NOT_USE_bNewItemCount[OldInventory::BIGPOCK2POS];
	bNewItemCount[BIGPOCK3POS] = DO_NOT_USE_bNewItemCount[OldInventory::BIGPOCK3POS];
	bNewItemCount[BIGPOCK4POS] = DO_NOT_USE_bNewItemCount[OldInventory::BIGPOCK4POS];
	bNewItemCount[SMALLPOCK1POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK1POS];
	bNewItemCount[SMALLPOCK2POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK2POS];
	bNewItemCount[SMALLPOCK3POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK3POS];
	bNewItemCount[SMALLPOCK4POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK4POS];
	bNewItemCount[SMALLPOCK5POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK5POS];
	bNewItemCount[SMALLPOCK6POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK6POS];
	bNewItemCount[SMALLPOCK7POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK7POS];
	bNewItemCount[SMALLPOCK8POS] = DO_NOT_USE_bNewItemCount[OldInventory::SMALLPOCK8POS];

	bNewItemCycleCount[HELMETPOS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::HELMETPOS];
	bNewItemCycleCount[VESTPOS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::VESTPOS];
	bNewItemCycleCount[LEGPOS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::LEGPOS];
	bNewItemCycleCount[HEAD1POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::HEAD1POS];
	bNewItemCycleCount[HEAD2POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::HEAD2POS];
	bNewItemCycleCount[HANDPOS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::HANDPOS];
	bNewItemCycleCount[SECONDHANDPOS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SECONDHANDPOS];
	bNewItemCycleCount[BIGPOCK1POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::BIGPOCK1POS];
	bNewItemCycleCount[BIGPOCK2POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::BIGPOCK2POS];
	bNewItemCycleCount[BIGPOCK3POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::BIGPOCK3POS];
	bNewItemCycleCount[BIGPOCK4POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::BIGPOCK4POS];
	bNewItemCycleCount[SMALLPOCK1POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK1POS];
	bNewItemCycleCount[SMALLPOCK2POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK2POS];
	bNewItemCycleCount[SMALLPOCK3POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK3POS];
	bNewItemCycleCount[SMALLPOCK4POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK4POS];
	bNewItemCycleCount[SMALLPOCK5POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK5POS];
	bNewItemCycleCount[SMALLPOCK6POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK6POS];
	bNewItemCycleCount[SMALLPOCK7POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK7POS];
	bNewItemCycleCount[SMALLPOCK8POS] = DO_NOT_USE_bNewItemCycleCount[OldInventory::SMALLPOCK8POS];
}

UINT32 MERCPROFILESTRUCT::GetChecksum( )
{
	UINT32	uiChecksum = 1;
	UINT32	uiLoop;

	uiChecksum += (this->bLife + 1);
	uiChecksum *= (this->bLifeMax + 1);
	uiChecksum += (this->bAgility + 1);
	uiChecksum *= (this->bDexterity + 1);
	uiChecksum += (this->bStrength + 1);
	uiChecksum *= (this->bMarksmanship + 1);
	uiChecksum += (this->bMedical + 1);
	uiChecksum *= (this->bMechanical + 1);
	uiChecksum += (this->bExplosive + 1);

	// put in some multipliers too!
	uiChecksum *= (this->bExpLevel + 1);

	for ( uiLoop = 0; uiLoop < this->inv.size(); uiLoop++ )
	{
		uiChecksum += this->inv[ uiLoop ];
		uiChecksum += this->bInvNumber[ uiLoop ];
	}

	return( uiChecksum );
}

OLD_MERCPROFILESTRUCT_101::OLD_MERCPROFILESTRUCT_101()
{
	memset(this, 0, SIZEOF_OLD_MERCPROFILESTRUCT_101_POD);
}

MERCPROFILESTRUCT::MERCPROFILESTRUCT()
{
	initialize();
}

// Copy Constructor
MERCPROFILESTRUCT::MERCPROFILESTRUCT(const MERCPROFILESTRUCT& src)
{
	memcpy(this, &src, SIZEOF_MERCPROFILESTRUCT_POD);
	inv = src.inv;
	bInvStatus = src.bInvStatus;
	bInvNumber = src.bInvNumber;
}

// Conversion Constructor
MERCPROFILESTRUCT& MERCPROFILESTRUCT::operator=(const OLD_MERCPROFILESTRUCT_101& src)
{
	//Why do we need this if the inv is an array of ints?  Because some data will be lost otherwise!
    if ((void*)this != (void*)&src) {
		CopyOldInventoryToNew(src);

		//arrays
		memcpy( &(this->zName), &(src.zName), sizeof(CHAR16) * NAME_LENGTH );
		memcpy( &(this->zNickname), &(src.zNickname), sizeof(CHAR16) * NICKNAME_LENGTH );
		memcpy( &(this->PANTS), &(src.PANTS), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->VEST), &(src.VEST), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->SKIN), &(src.SKIN), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->HAIR), &(src.HAIR), sizeof(PaletteRepID) );	// 30

		memcpy( &(this->bBuddy), &(src.bBuddy), 5 * sizeof (INT8));
		memcpy( &(this->bHated), &(src.bHated), 5 * sizeof (INT8));
		memcpy( &(this->ubRoomRangeStart), &(src.ubRoomRangeStart), 2 * sizeof (UINT8));
		memcpy( &(this->bMercTownReputation), &(src.bMercTownReputation),  20  * sizeof (INT8));
		memcpy( &(this->usApproachFactor), &(src.usApproachFactor), 4 * sizeof (UINT16));

		memcpy( &(this->ubApproachVal), &(src.ubApproachVal), 4 * sizeof (UINT8));
		memcpy( &(this->ubApproachMod), &(src.ubApproachMod), 3 * 4 * sizeof (UINT8));
		memcpy( &(this->bMercOpinion), &(src.bMercOpinion), 75 * sizeof (INT8));

		memcpy( &(this->usStatChangeChances), &(src.usStatChangeChances),  12  * sizeof (UINT16));// used strictly for balancing, never shown!
		memcpy( &(this->usStatChangeSuccesses), &(src.usStatChangeSuccesses),  12  * sizeof (UINT16));// used strictly for balancing, never shown!
		memcpy( &(this->ubRoomRangeEnd), &(src.ubRoomRangeEnd), 2 * sizeof (UINT8));
		memcpy( &(this->bHatedTime), &(src.bHatedTime), 5 * sizeof (INT8));
		memcpy( &(this->bHatedCount), &(src.bHatedCount), 5 * sizeof (INT8));

		this->bLearnToLike = src.bLearnToLike;
		this->uiAttnSound = src.uiAttnSound;
		this->uiCurseSound = src.uiCurseSound;
		this->uiDieSound = src.uiDieSound;
		this->uiGoodSound = src.uiGoodSound;
		this->uiGruntSound = src.uiGruntSound;
		this->uiGrunt2Sound = src.uiGrunt2Sound;
		this->uiOkSound = src.uiOkSound;
		this->ubFaceIndex = src.ubFaceIndex;
		this->bSex = src.bSex;
		this->bArmourAttractiveness = src.bArmourAttractiveness;
		this->ubMiscFlags2 = src.ubMiscFlags2;
		this->bEvolution = src.bEvolution;
		this->ubMiscFlags = src.ubMiscFlags;
		this->bSexist = src.bSexist;
		this->bLearnToHate = src.bLearnToHate;

		// skills
		this->bStealRate = src.bStealRate;
		this->bVocalVolume = src.bVocalVolume;
		this->ubQuoteRecord = src.ubQuoteRecord;
		this->bDeathRate = src.bDeathRate;
		this->bScientific = src.bScientific;

		this->sExpLevelGain = src.sExpLevelGain;
		this->sLifeGain = src.sLifeGain;
		this->sAgilityGain = src.sAgilityGain;
		this->sDexterityGain = src.sDexterityGain;
		this->sWisdomGain = src.sWisdomGain;
		this->sMarksmanshipGain = src.sMarksmanshipGain;
		this->sMedicalGain = src.sMedicalGain;
		this->sMechanicGain = src.sMechanicGain;
		this->sExplosivesGain = src.sExplosivesGain;

		this->ubBodyType = src.ubBodyType;
		this->bMedical = src.bMedical;

		this->usEyesX = src.usEyesX;
		this->usEyesY = src.usEyesY;
		this->usMouthX = src.usMouthX;
		this->usMouthY = src.usMouthY;
		this->uiEyeDelay = src.uiEyeDelay;
		this->uiMouthDelay = src.uiMouthDelay;
		this->uiBlinkFrequency = src.uiBlinkFrequency;
		this->uiExpressionFrequency = src.uiExpressionFrequency;
		this->sSectorX = src.sSectorX;
		this->sSectorY = src.sSectorY;

		this->uiDayBecomesAvailable = src.uiDayBecomesAvailable;//day the merc will be available.used with the bMercStatus

		this->bStrength = src.bStrength;

		this->bLifeMax = src.bLifeMax;
		this->bExpLevelDelta = src.bExpLevelDelta;
		this->bLifeDelta = src.bLifeDelta;
		this->bAgilityDelta = src.bAgilityDelta;
		this->bDexterityDelta = src.bDexterityDelta;
		this->bWisdomDelta = src.bWisdomDelta;
		this->bMarksmanshipDelta = src.bMarksmanshipDelta;
		this->bMedicalDelta = src.bMedicalDelta;
		this->bMechanicDelta = src.bMechanicDelta;
		this->bExplosivesDelta = src.bExplosivesDelta;
		this->bStrengthDelta = src.bStrengthDelta;
		this->bLeadershipDelta = src.bLeadershipDelta;
		this->usKills = src.usKills;
		this->usAssists = src.usAssists;
		this->usShotsFired = src.usShotsFired;
		this->usShotsHit = src.usShotsHit;
		this->usBattlesFought = src.usBattlesFought;
		this->usTimesWounded = src.usTimesWounded;
		this->usTotalDaysServed = src.usTotalDaysServed;

		this->sLeadershipGain = src.sLeadershipGain;
		this->sStrengthGain = src.sStrengthGain;



		// BODY TYPE SUBSITUTIONS
		this->uiBodyTypeSubFlags = src.uiBodyTypeSubFlags;

		this->sSalary = src.sSalary;
		this->bLife = src.bLife;
		this->bDexterity = src.bDexterity;// dexterity (hand coord) value
		this->bPersonalityTrait = src.bPersonalityTrait;
		this->bSkillTrait = src.bSkillTrait;

		this->bReputationTolerance = src.bReputationTolerance;
		this->bExplosive = src.bExplosive;
		this->bSkillTrait2 = src.bSkillTrait2;
		this->bLeadership = src.bLeadership;

		this->bExpLevel = src.bExpLevel;// general experience level

		this->bMarksmanship = src.bMarksmanship;
		this->bMinService = src.bMinService;
		this->bWisdom = src.bWisdom;
		this->bResigned = src.bResigned;
		this->bActive = src.bActive;

		this->bMainGunAttractiveness = src.bMainGunAttractiveness;
		this->bAgility = src.bAgility;// agility (speed) value

		this->fUseProfileInsertionInfo = src.fUseProfileInsertionInfo;// Set to various flags, ( contained in TacticalSave.h )
		this->sGridNo = src.sGridNo;// The Gridno the NPC was in before leaving the sector
		this->ubQuoteActionID = src.ubQuoteActionID;
		this->bMechanical = src.bMechanical;

		this->ubInvUndroppable = src.ubInvUndroppable;

		this->ubStrategicInsertionCode = src.ubStrategicInsertionCode;


		this->ubLastQuoteSaid = src.ubLastQuoteSaid;

		this->bRace = src.bRace;
		this->bNationality = src.bNationality;
		this->bAppearance = src.bAppearance;
		this->bAppearanceCareLevel = src.bAppearanceCareLevel;
		this->bRefinement = src.bRefinement;
		this->bRefinementCareLevel = src.bRefinementCareLevel;
		this->bHatedNationality = src.bHatedNationality;
		this->bHatedNationalityCareLevel = src.bHatedNationalityCareLevel;
		this->bRacist = src.bRacist;
		this->uiWeeklySalary = src.uiWeeklySalary;
		this->uiBiWeeklySalary = src.uiBiWeeklySalary;
		this->bMedicalDeposit = src.bMedicalDeposit;
		this->bAttitude = src.bAttitude;
		this->bBaseMorale = src.bBaseMorale;
		this->sMedicalDepositAmount = src.sMedicalDepositAmount;

		this->bTown = src.bTown;
		this->bTownAttachment = src.bTownAttachment;
		this->usOptionalGearCost = src.usOptionalGearCost;
		this->bApproached = src.bApproached;
		this->bMercStatus = src.bMercStatus;//The status of the merc.If negative, see flags at the top of this file.Positive:The number of days the merc is away for.0:Not hired but ready to be.
		this->bLearnToLikeTime = src.bLearnToLikeTime;
		this->bLearnToHateTime = src.bLearnToHateTime;
		this->bLearnToLikeCount = src.bLearnToLikeCount;
		this->bLearnToHateCount = src.bLearnToHateCount;
		this->ubLastDateSpokenTo = src.ubLastDateSpokenTo;
		this->bLastQuoteSaidWasSpecial = src.bLastQuoteSaidWasSpecial;
		this->bSectorZ = src.bSectorZ;
		this->usStrategicInsertionData = src.usStrategicInsertionData;
		this->bFriendlyOrDirectDefaultResponseUsedRecently = src.bFriendlyOrDirectDefaultResponseUsedRecently;
		this->bRecruitDefaultResponseUsedRecently = src.bRecruitDefaultResponseUsedRecently;
		this->bThreatenDefaultResponseUsedRecently = src.bThreatenDefaultResponseUsedRecently;
		this->bNPCData = src.bNPCData;// NPC specific
		this->iBalance = src.iBalance;
		this->sTrueSalary = src.sTrueSalary; // for use when the person is working for us for free but has a positive salary value
		this->ubCivilianGroup = src.ubCivilianGroup;
		this->ubNeedForSleep = src.ubNeedForSleep;
		this->uiMoney = src.uiMoney;
		this->bNPCData2 = src.bNPCData2;// NPC specific

		this->ubMiscFlags3 = src.ubMiscFlags3;

		this->ubDaysOfMoraleHangover = src.ubDaysOfMoraleHangover;// used only when merc leaves team while having poor morale
		this->ubNumTimesDrugUseInLifetime = src.ubNumTimesDrugUseInLifetime;// The # times a drug has been used in the player's lifetime...

		// Flags used for the precedent to repeating oneself in Contract negotiations.Used for quote 80 -~107.Gets reset every day
		this->uiPrecedentQuoteSaid = src.uiPrecedentQuoteSaid;
		this->uiProfileChecksum = src.uiProfileChecksum;
		this->sPreCombatGridNo = src.sPreCombatGridNo;
		this->ubTimeTillNextHatedComplaint = src.ubTimeTillNextHatedComplaint;
		this->ubSuspiciousDeath = src.ubSuspiciousDeath;

		this->iMercMercContractLength = src.iMercMercContractLength;//Used for MERC mercs, specifies how many days the merc has gone since last page

		this->uiTotalCostToDate = src.uiTotalCostToDate;// The total amount of money that has been paid to the merc for their salary
	}
	return *this;
}

// Assignment operator
MERCPROFILESTRUCT& MERCPROFILESTRUCT::operator=(const MERCPROFILESTRUCT& src)
{
    if (this != &src) {
		memcpy(this, &src, SIZEOF_MERCPROFILESTRUCT_POD);
		inv = src.inv;
		bInvStatus = src.bInvStatus;
		bInvNumber = src.bInvNumber;
    }
	return *this;
}

// Destructor
MERCPROFILESTRUCT::~MERCPROFILESTRUCT()
{
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILESTRUCT::initialize()
{
	memset( this, 0, SIZEOF_MERCPROFILESTRUCT_POD);
	clearInventory();
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILESTRUCT::clearInventory()
{
	inv.clear();
	bInvStatus.clear();
	bInvNumber.clear();

	inv.resize(NUM_INV_SLOTS);
	bInvStatus.resize(NUM_INV_SLOTS);
	bInvNumber.resize(NUM_INV_SLOTS);
}

void MERCPROFILESTRUCT::CopyOldInventoryToNew(const OLD_MERCPROFILESTRUCT_101& src)
{
	// Do not use a loop in case the new inventory slots are arranged differently than the old
	inv[HELMETPOS] = src.DO_NOT_USE_inv[OldInventory::HELMETPOS];
	inv[VESTPOS] = src.DO_NOT_USE_inv[OldInventory::VESTPOS];
	inv[LEGPOS] = src.DO_NOT_USE_inv[OldInventory::LEGPOS];
	inv[HEAD1POS] = src.DO_NOT_USE_inv[OldInventory::HEAD1POS];
	inv[HEAD2POS] = src.DO_NOT_USE_inv[OldInventory::HEAD2POS];
	inv[HANDPOS] = src.DO_NOT_USE_inv[OldInventory::HANDPOS];
	inv[SECONDHANDPOS] = src.DO_NOT_USE_inv[OldInventory::SECONDHANDPOS];
	inv[BIGPOCK1POS] = src.DO_NOT_USE_inv[OldInventory::BIGPOCK1POS];
	inv[BIGPOCK2POS] = src.DO_NOT_USE_inv[OldInventory::BIGPOCK2POS];
	inv[BIGPOCK3POS] = src.DO_NOT_USE_inv[OldInventory::BIGPOCK3POS];
	inv[BIGPOCK4POS] = src.DO_NOT_USE_inv[OldInventory::BIGPOCK4POS];
	inv[SMALLPOCK1POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK1POS];
	inv[SMALLPOCK2POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK2POS];
	inv[SMALLPOCK3POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK3POS];
	inv[SMALLPOCK4POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK4POS];
	inv[SMALLPOCK5POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK5POS];
	inv[SMALLPOCK6POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK6POS];
	inv[SMALLPOCK7POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK7POS];
	inv[SMALLPOCK8POS] = src.DO_NOT_USE_inv[OldInventory::SMALLPOCK8POS];

	bInvStatus[HELMETPOS] = src.DO_NOT_USE_bInvStatus[OldInventory::HELMETPOS];
	bInvStatus[VESTPOS] = src.DO_NOT_USE_bInvStatus[OldInventory::VESTPOS];
	bInvStatus[LEGPOS] = src.DO_NOT_USE_bInvStatus[OldInventory::LEGPOS];
	bInvStatus[HEAD1POS] = src.DO_NOT_USE_bInvStatus[OldInventory::HEAD1POS];
	bInvStatus[HEAD2POS] = src.DO_NOT_USE_bInvStatus[OldInventory::HEAD2POS];
	bInvStatus[HANDPOS] = src.DO_NOT_USE_bInvStatus[OldInventory::HANDPOS];
	bInvStatus[SECONDHANDPOS] = src.DO_NOT_USE_bInvStatus[OldInventory::SECONDHANDPOS];
	bInvStatus[BIGPOCK1POS] = src.DO_NOT_USE_bInvStatus[OldInventory::BIGPOCK1POS];
	bInvStatus[BIGPOCK2POS] = src.DO_NOT_USE_bInvStatus[OldInventory::BIGPOCK2POS];
	bInvStatus[BIGPOCK3POS] = src.DO_NOT_USE_bInvStatus[OldInventory::BIGPOCK3POS];
	bInvStatus[BIGPOCK4POS] = src.DO_NOT_USE_bInvStatus[OldInventory::BIGPOCK4POS];
	bInvStatus[SMALLPOCK1POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK1POS];
	bInvStatus[SMALLPOCK2POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK2POS];
	bInvStatus[SMALLPOCK3POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK3POS];
	bInvStatus[SMALLPOCK4POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK4POS];
	bInvStatus[SMALLPOCK5POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK5POS];
	bInvStatus[SMALLPOCK6POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK6POS];
	bInvStatus[SMALLPOCK7POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK7POS];
	bInvStatus[SMALLPOCK8POS] = src.DO_NOT_USE_bInvStatus[OldInventory::SMALLPOCK8POS];

	bInvNumber[HELMETPOS] = src.DO_NOT_USE_bInvNumber[OldInventory::HELMETPOS];
	bInvNumber[VESTPOS] = src.DO_NOT_USE_bInvNumber[OldInventory::VESTPOS];
	bInvNumber[LEGPOS] = src.DO_NOT_USE_bInvNumber[OldInventory::LEGPOS];
	bInvNumber[HEAD1POS] = src.DO_NOT_USE_bInvNumber[OldInventory::HEAD1POS];
	bInvNumber[HEAD2POS] = src.DO_NOT_USE_bInvNumber[OldInventory::HEAD2POS];
	bInvNumber[HANDPOS] = src.DO_NOT_USE_bInvNumber[OldInventory::HANDPOS];
	bInvNumber[SECONDHANDPOS] = src.DO_NOT_USE_bInvNumber[OldInventory::SECONDHANDPOS];
	bInvNumber[BIGPOCK1POS] = src.DO_NOT_USE_bInvNumber[OldInventory::BIGPOCK1POS];
	bInvNumber[BIGPOCK2POS] = src.DO_NOT_USE_bInvNumber[OldInventory::BIGPOCK2POS];
	bInvNumber[BIGPOCK3POS] = src.DO_NOT_USE_bInvNumber[OldInventory::BIGPOCK3POS];
	bInvNumber[BIGPOCK4POS] = src.DO_NOT_USE_bInvNumber[OldInventory::BIGPOCK4POS];
	bInvNumber[SMALLPOCK1POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK1POS];
	bInvNumber[SMALLPOCK2POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK2POS];
	bInvNumber[SMALLPOCK3POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK3POS];
	bInvNumber[SMALLPOCK4POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK4POS];
	bInvNumber[SMALLPOCK5POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK5POS];
	bInvNumber[SMALLPOCK6POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK6POS];
	bInvNumber[SMALLPOCK7POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK7POS];
	bInvNumber[SMALLPOCK8POS] = src.DO_NOT_USE_bInvNumber[OldInventory::SMALLPOCK8POS];
}

BOOLEAN IsValidSecondHandShot( SOLDIERTYPE *pSoldier );

UINT8					*gubpNumReplacementsPerRange;
PaletteSubRangeType		*gpPaletteSubRanges;


UINT8	bHealthStrRanges[] =
{
	15,
	30,
	45,
	60,
	75,
	90,
	101
};


INT16 gsTerrainTypeSpeedModifiers[] =
{
	5,						// Flat ground
	5,						// Floor
	5,						// Paved road
	5,						// Dirt road
	10,						// LOW GRASS
	15,						// HIGH GRASS
	20,						// TRAIN TRACKS
	20,						// LOW WATER
	25,						// MID WATER
	30							// DEEP WATER
};

//Kris:
//Temporary for testing the speed of the translucency.  Pressing Ctrl+L in turn based
//input will toggle this flag.  When clear, the translucency checking is turned off to
//increase the speed of the game.
BOOLEAN gfCalcTranslucency = FALSE;


INT16		gsFullTileDirections[ MAX_FULLTILE_DIRECTIONS ] =
{
	-1, -WORLD_COLS - 1, -WORLD_COLS

};

PaletteReplacementType	*gpPalRep;

// Palette ranges
UINT32									guiNumPaletteSubRanges;
PaletteSubRangeType			*guipPaletteSubRanges = NULL;
// Palette replacements
UINT32									guiNumReplacements;
PaletteReplacementType	*guipPaletteReplacements = NULL;

extern BOOLEAN fReDrawFace;
extern UINT8 gubWaitingForAllMercsToExitCode;
BOOLEAN	gfGetNewPathThroughPeople = FALSE;

// LOCAL FUNCTIONS
// DO NOT CALL UNLESS THROUGH EVENT_SetSoldierPosition
UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady );
BOOLEAN CheckForFullStruct( INT16 sGridNo, UINT16 *pusIndex  );
void SetSoldierLocatorOffsets( SOLDIERTYPE *pSoldier );
void CheckForFullStructures( SOLDIERTYPE *pSoldier );
BOOLEAN InitNewSoldierState( SOLDIERTYPE *pSoldier, UINT8 ubNewState, UINT16 usStartingAniCode );
UINT16 GetNewSoldierStateFromNewStance( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void SetSoldierAniSpeed( SOLDIERTYPE *pSoldier );
void AdjustForFastTurnAnimation( SOLDIERTYPE *pSoldier );
UINT16 SelectFireAnimation( SOLDIERTYPE *pSoldier, UINT8 ubHeight );
void SelectFallAnimation( SOLDIERTYPE *pSoldier );
BOOLEAN FullStructAlone( INT16 sGridNo, UINT8 ubRadius );
void SoldierGotHitGunFire( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial , UINT8 ubHitLocation);
void SoldierGotHitBlade( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitPunch( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitExplosion( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
UINT8 CalcScreamVolume( SOLDIERTYPE * pSoldier, UINT8 ubCombinedLoss );
void PlaySoldierFootstepSound( SOLDIERTYPE *pSoldier );
void HandleSystemNewAISituation( SOLDIERTYPE *pSoldier, BOOLEAN fResetABC );

UINT16 *CreateEnemyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen );
UINT16 *CreateEnemyGreyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen );

void SoldierBleed( SOLDIERTYPE *pSoldier, BOOLEAN fBandagedBleed );
INT32 CheckBleeding( SOLDIERTYPE *pSoldier );

void EVENT_InternalSetSoldierDesiredDirection( SOLDIERTYPE *pSoldier, UINT8	ubNewDirection, BOOLEAN fInitalMove, UINT16 usAnimState );

#ifdef JA2BETAVERSION
extern void ValidatePlayersAreInOneGroupOnly();
extern void MapScreenDefaultOkBoxCallback( UINT8 bExitValue );
void SAIReportError( STR16 wErrorString );
#endif

UINT32 SleepDartSuccumbChance( SOLDIERTYPE * pSoldier );

void	EnableDisableSoldierLightEffects( BOOLEAN fEnableLights );
void	SetSoldierPersonalLightLevel( SOLDIERTYPE *pSoldier );


void HandleVehicleMovementSound( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	VEHICLETYPE *pVehicle = &( pVehicleList[ pSoldier->bVehicleID ] );

	if ( fOn )
	{
		if ( pVehicle->iMovementSoundID == NO_SAMPLE )
		{
			pVehicle->iMovementSoundID = PlayJA2Sample( pVehicle->iMoveSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		}
	}
	else
	{
		if ( pVehicle->iMovementSoundID != NO_SAMPLE )
		{
			SoundStop( pVehicle->iMovementSoundID );
			pVehicle->iMovementSoundID = NO_SAMPLE;
		}
	}
}


void SOLDIERTYPE::AdjustNoAPToFinishMove( BOOLEAN fSet )
{
	if ( this->ubBodyType == CROW )
	{
		return;
	}

	// Check if we are a vehicle first....
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// Turn off sound effects....
		if ( fSet )
		{
			HandleVehicleMovementSound( this, FALSE );
		}
	}

	// Turn off sound effects....
	if ( fSet )
	{
		// Position light....
		// this->SetCheckSoldierLightFlag( );
	}
	else
	{
		// this->DeleteSoldierLight( );
	}


	//send it on
		if (is_networked && this->ubID < 120)
	{
		//if(this->ubID>=120) 
		//	return;//hayden
		EV_S_STOP_MERC				SStopMerc;

		SStopMerc.sGridNo					= this->sGridNo;
		SStopMerc.ubDirection			= this->ubDirection;
		SStopMerc.usSoldierID			= this->ubID;
		SStopMerc.fset=fSet;
			SStopMerc.sXPos=this->sX;
		SStopMerc.sYPos=this->sY;

		//AddGameEvent( S_STOP_MERC, 0, &SStopMerc ); //hayden.
		if (!is_server && this->ubID >=20)return;

		if(is_client)
			send_stop(&SStopMerc);
	}







	this->flags.fNoAPToFinishMove = fSet;

	if ( !fSet )
	{
		// return reason to default value
		this->ubReasonCantFinishMove = REASON_STOPPED_NO_APS;
	}
}

void HandleCrowShadowVisibility( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->usAnimState == CROW_FLY )
		{
			if ( pSoldier->pAniTile != NULL )
			{
				if ( pSoldier->bLastRenderVisibleValue != -1 )
				{
					HideAniTile( pSoldier->pAniTile, FALSE );
				}
				else
				{
					HideAniTile( pSoldier->pAniTile, TRUE );
				}
			}
		}
	}
}

void HandleCrowShadowNewGridNo( SOLDIERTYPE *pSoldier )
{
	ANITILE_PARAMS	AniParams;

	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );

	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->pAniTile != NULL )
		{
			DeleteAniTile( pSoldier->pAniTile );
			pSoldier->pAniTile = NULL;
		}

		if ( pSoldier->sGridNo != NOWHERE )
		{
			if ( pSoldier->usAnimState == CROW_FLY )
			{
				AniParams.sGridNo							= (INT16)pSoldier->sGridNo;
				AniParams.ubLevelID						= ANI_SHADOW_LEVEL;
				AniParams.sDelay							= pSoldier->sAniDelay;
				AniParams.sStartFrame					= 0;
				AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_LOOPING | ANITILE_USE_DIRECTION_FOR_START_FRAME;
				AniParams.sX									= pSoldier->sX;
				AniParams.sY									= pSoldier->sY;
				AniParams.sZ									= 0;
				strcpy( AniParams.zCachedFile, "TILECACHE\\FLY_SHDW.STI" );

				AniParams.uiUserData3					= pSoldier->ubDirection;

				pSoldier->pAniTile = CreateAnimationTile( &AniParams );

				HandleCrowShadowVisibility( pSoldier );
			}
		}
	}
}


void HandleCrowShadowRemoveGridNo( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->usAnimState == CROW_FLY )
		{
			if ( pSoldier->pAniTile != NULL )
			{
				DeleteAniTile( pSoldier->pAniTile );
				pSoldier->pAniTile = NULL;
			}
		}
	}
}


void HandleCrowShadowNewDirection( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->usAnimState == CROW_FLY )
		{
			if ( pSoldier->pAniTile != NULL )
			{
				pSoldier->pAniTile->uiUserData3	= pSoldier->ubDirection;
			}
		}
	}
}

void HandleCrowShadowNewPosition( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->usAnimState == CROW_FLY )
		{
			if ( pSoldier->pAniTile != NULL )
			{
				pSoldier->pAniTile->sRelativeX	= pSoldier->sX;
				pSoldier->pAniTile->sRelativeY	= pSoldier->sY;
			}
		}
	}
}



INT8 SOLDIERTYPE::CalcActionPoints( void )
{
	UINT8 ubPoints,ubMaxAPs;
	INT8  bBandage;

	// dead guys don't get any APs (they shouldn't be here asking for them!)
	if (!this->stats.bLife)
		return(0);

	// people with sleep dart drug who have collapsed get no APs
	if ( (this->bSleepDrugCounter > 0) && this->bCollapsed )
		return( 0 );

	// Calculate merc's action points at 100% capability (range is 10 - 25)
	// round fractions of .5 up (that's why the +20 before the division!
	ubPoints = 5 + (((10 * EffectiveExpLevel( this ) +
		3 * EffectiveAgility( this )   +
		2 * this->stats.bLifeMax   +
		2 * EffectiveDexterity( this ) ) + 20) / 40);

	//if (GameOption[INCREASEDAP] % 2 == 1)
	//points += AP_INCREASE;

	//Madd: Add in AP Bonuses (Penalties) from worn gear
	ubPoints += GetGearAPBonus ( this );

	// Calculate bandage
	bBandage = this->stats.bLifeMax - this->stats.bLife - this->bBleeding;

	// If injured, reduce action points accordingly (by up to 2/3rds)
	if (this->stats.bLife < this->stats.bLifeMax)
	{
		ubPoints -= (2 * ubPoints * (this->stats.bLifeMax - this->stats.bLife + (bBandage / 2))) /
			(3 * this->stats.bLifeMax);
	}

	// If tired, reduce action points accordingly (by up to 1/2)
	if (this->bBreath < 100)
		ubPoints -= (ubPoints * (100 - this->bBreath)) / 200;

	if (this->sWeightCarriedAtTurnStart > 100)
	{
		ubPoints = (UINT8) ( ((UINT32)ubPoints) * 100 / this->sWeightCarriedAtTurnStart );
	}

	// If resulting APs are below our permitted minimum, raise them to it!
	if (ubPoints < AP_MINIMUM)
		ubPoints = AP_MINIMUM;

	// make sure action points doesn't exceed the permitted maximum
	ubMaxAPs = gubMaxActionPoints[ this->ubBodyType ];

	//if (GameOption[INCREASEDAP] % 2 == 1)
	// maxAPs += AP_INCREASE;


	// If resulting APs are below our permitted minimum, raise them to it!
	if (ubPoints > ubMaxAPs)
		ubPoints = ubMaxAPs;

	if ( this->ubBodyType == BLOODCAT )
	{
		// use same as young monsters
		ubPoints = (ubPoints * AP_YOUNG_MONST_FACTOR) / 10;
	}
	else if (this->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		// young monsters get extra APs
		if ( this->ubBodyType == YAF_MONSTER || this->ubBodyType == YAM_MONSTER || this->ubBodyType == INFANT_MONSTER )
		{
			ubPoints = (ubPoints * AP_YOUNG_MONST_FACTOR) / 10;
		}

		// if frenzied, female monsters get more APs! (for young females, cumulative!)
		if (this->aiData.bFrenzied)
		{

			ubPoints = (ubPoints * AP_MONST_FRENZY_FACTOR) / 10;
		}
	}

	// adjust APs for phobia situations
	if ( this->ubProfile != NO_PROFILE )
	{
		if ( (gMercProfiles[ this->ubProfile ].bPersonalityTrait == CLAUSTROPHOBIC) && (gbWorldSectorZ > 0) )
		{
			ubPoints = (ubPoints * AP_CLAUSTROPHOBE) / 10;
		}
		else if ( (gMercProfiles[ this->ubProfile ].bPersonalityTrait == FEAR_OF_INSECTS) && (MercSeesCreature( this ) ) )
		{
			ubPoints = (ubPoints * AP_AFRAID_OF_INSECTS) / 10;
		}
	}

	// Adjusat APs due to drugs...
	HandleAPEffectDueToDrugs( this, &ubPoints );

	// If we are a vehicle, adjust APS...
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		AdjustVehicleAPs( this, &ubPoints );
	}

	//Madd
	//	if ( this->bTeam != CIV_TEAM && this->bTeam != gbPlayerNum && gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
	//		ubPoints += 5; // everything and everyone moves a little faster against you on insane...


	//Kaiden: Took your idea a step further adding the bonus for each difficulty level
	// and then externalized it. AND added it to the Dont max out points section below.
	if ( this->bTeam != CIV_TEAM && this->bTeam != gbPlayerNum)
	{

		switch( gGameOptions.ubDifficultyLevel )
		{
		case DIF_LEVEL_EASY:

			ubPoints += gGameExternalOptions.iEasyAPBonus;
			break;

		case DIF_LEVEL_MEDIUM:

			ubPoints += gGameExternalOptions.iExperiencedAPBonus;
			break;

		case DIF_LEVEL_HARD:

			ubPoints += gGameExternalOptions.iExpertAPBonus;
			break;

		case DIF_LEVEL_INSANE:

			ubPoints += gGameExternalOptions.iInsaneAPBonus;
			break;

		default:
			ubPoints +=0;
		}
	}
	else
		ubPoints += gGameExternalOptions.iPlayerAPBonus;

	// if we are in boxing mode, adjust APs... THIS MUST BE LAST!
	if ( gTacticalStatus.bBoxingState == BOXING || gTacticalStatus.bBoxingState == PRE_BOXING )
	{
		ubPoints /= 2;
	}

	return (ubPoints);
}

void SOLDIERTYPE::CalcNewActionPoints( void )
{
	if ( gTacticalStatus.bBoxingState == BOXING || gTacticalStatus.bBoxingState == PRE_BOXING )
	{
		// if we are in boxing mode, carry 1/2 as many points
		if (this->bActionPoints > MAX_AP_CARRIED / 2)
		{
			this->bActionPoints = MAX_AP_CARRIED / 2;
		}
	}
	else
	{
		if (this->bActionPoints > MAX_AP_CARRIED)
		{
			this->bActionPoints = MAX_AP_CARRIED;
		}
	}

	this->bActionPoints					+= this->CalcActionPoints( );

	// Don't max out if we are drugged....
	if ( !GetDrugEffect( this, DRUG_TYPE_ADRENALINE ) )
	{  //Kaiden: If Enemy, they can max out, but their Max is NOW = MAX + diffAPBonus
		// No sense in giving them a bonus if some of the points are wasted because we
		// Didn't raise their cap by the same amount.
		if ( this->bTeam != CIV_TEAM && this->bTeam != gbPlayerNum)
		{
			switch( gGameOptions.ubDifficultyLevel )
			{
			case DIF_LEVEL_EASY:

				this->bActionPoints	= __min( this->bActionPoints, (gubMaxActionPoints[ this->ubBodyType ] + gGameExternalOptions.iEasyAPBonus));
				break;

			case DIF_LEVEL_MEDIUM:

				this->bActionPoints	= __min( this->bActionPoints, (gubMaxActionPoints[ this->ubBodyType ] + gGameExternalOptions.iExperiencedAPBonus));
				break;

			case DIF_LEVEL_HARD:

				this->bActionPoints	= __min( this->bActionPoints, (gubMaxActionPoints[ this->ubBodyType ] + gGameExternalOptions.iExpertAPBonus));
				break;

			case DIF_LEVEL_INSANE:

				this->bActionPoints	= __min( this->bActionPoints, (gubMaxActionPoints[ this->ubBodyType ] + gGameExternalOptions.iInsaneAPBonus));
				break;

			default:
				this->bActionPoints					= __min( this->bActionPoints, gubMaxActionPoints[ this->ubBodyType ] );
			}

		}
		else
		{ //Kaiden: Players just max out normally unless drugged
			this->bActionPoints	= __min( this->bActionPoints, (gubMaxActionPoints[ this->ubBodyType ] + gGameExternalOptions.iPlayerAPBonus) );
		}

	}

	this->bInitialActionPoints	= this->bActionPoints;
}


void	SOLDIERTYPE::DoNinjaAttack( void )
{
	//UINT32						uiMercFlags;
	UINT16						usSoldierIndex;
	SOLDIERTYPE				*pTSoldier;
	UINT8							ubTDirection;
	UINT8							ubTargetStance;


	usSoldierIndex = WhoIsThere2( this->sTargetGridNo, this->pathing.bLevel );
	if ( usSoldierIndex != NOBODY )
	{
		GetSoldier( &pTSoldier, usSoldierIndex );

		// Look at stance of target
		ubTargetStance = gAnimControl[ pTSoldier->usAnimState ].ubEndHeight;

		// Get his life...if < certain value, do finish!
		if ( (pTSoldier->stats.bLife <= 30 || pTSoldier->bBreath <= 30) && ubTargetStance != ANIM_PRONE )
		{
			// Do finish!
			this->ChangeSoldierState( NINJA_SPINKICK, 0 , FALSE );
		}
		else
		{
			if ( ubTargetStance != ANIM_PRONE )
			{
				if ( Random( 2 ) == 0 )
				{
					this->ChangeSoldierState( NINJA_LOWKICK, 0 , FALSE );
				}
				else
				{
					this->ChangeSoldierState( NINJA_PUNCH, 0 , FALSE );
				}

				// CHECK IF HE CAN SEE US, IF SO CHANGE DIRECTION
				if ( pTSoldier->aiData.bOppList[ this->ubID ] == 0 && pTSoldier->bTeam != this->bTeam )
				{
					if ( !( pTSoldier->flags.uiStatusFlags & ( SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE ) ) )
					{
						ubTDirection = (UINT8)GetDirectionFromGridNo( this->sGridNo, pTSoldier );
						SendSoldierSetDesiredDirectionEvent( pTSoldier, ubTDirection );
					}
				}
			}
			else
			{
				// CHECK OUR STANCE
				if ( gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_CROUCH )
				{
					// SET DESIRED STANCE AND SET PENDING ANIMATION
					SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
					this->usPendingAnimation = PUNCH_LOW;
				}
				else
				{
					// USE crouched one
					// NEED TO CHANGE STANCE IF NOT CROUCHD!
					this->EVENT_InitNewSoldierAnim( PUNCH_LOW, 0 , FALSE );
				}
			}
		}
	}

	if ( this->ubProfile == 33 )
	{
		UINT32 uiSoundID;
		SOUNDPARMS		spParms;
		INT32		iFaceIndex;

		// Play sound!
		memset(&spParms, 0xff, sizeof(SOUNDPARMS));

		spParms.uiSpeed = RATE_11025;
		spParms.uiVolume = (INT8)CalculateSpeechVolume( HIGHVOLUME );

		// If we are an enemy.....reduce due to volume
		if ( this->bTeam != gbPlayerNum )
		{
			spParms.uiVolume = SoundVolume( (UINT8)spParms.uiVolume, this->sGridNo );
		}
		spParms.uiLoop = 1;
		spParms.uiPan = SoundDir( this->sGridNo );
		spParms.uiPriority=GROUP_PLAYER;

		if ( this->usAnimState == NINJA_SPINKICK )
		{
			uiSoundID = SoundPlay( "BATTLESNDS\\033_CHOP2.WAV", &spParms );
		}
		else
		{
			if ( Random( 2 ) == 0 )
			{
				uiSoundID = SoundPlay( "BATTLESNDS\\033_CHOP3.WAV", &spParms );
			}
			else
			{
				uiSoundID = SoundPlay( "BATTLESNDS\\033_CHOP1.WAV", &spParms );
			}
		}


		if ( uiSoundID != SOUND_ERROR )
		{
			this->uiBattleSoundID = uiSoundID;

			if ( this->ubProfile != NO_PROFILE )
			{
				// Get soldier's face ID
				iFaceIndex = this->iFaceIndex;

				// Check face index
				if ( iFaceIndex != -1 )
				{
					ExternSetFaceTalking( iFaceIndex, uiSoundID );
				}
			}
		}
	}

}


BOOLEAN SOLDIERTYPE::CreateSoldierCommon( UINT8 ubBodyType, UINT16 usSoldierID, UINT16 usState )
{
	BOOLEAN fSuccess = FALSE;
	INT32 iCounter = 0;

	//if we are loading a saved game, we DO NOT want to reset the opplist, look for enemies, or say a dying commnet
	if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		// Set initial values for opplist!
		InitSoldierOppList( this );
		HandleSight( this, SIGHT_LOOK );

		// Set some quote flags
		if ( this->stats.bLife >= OKLIFE )
		{
			this->flags.fDyingComment = FALSE;
		}
		else
		{
			this->flags.fDyingComment = TRUE;
		}
	}

	// ATE: Reset some timer flags...
	this->uiTimeSameBattleSndDone = 0;
	// ATE: Reset every time.....
	this->flags.fSoldierWasMoving				= TRUE;
	this->iTuringSoundID          = NO_SAMPLE;
	this->uiTimeSinceLastBleedGrunt = 0;

	if ( this->ubBodyType == QUEENMONSTER )
	{
		this->iPositionSndID = NewPositionSnd( NOWHERE, POSITION_SOUND_FROM_SOLDIER, (UINT32)this, QUEEN_AMBIENT_NOISE );
	}


	// ANYTHING AFTER HERE CAN FAIL
	do
	{

		if (usSoldierID <= gTacticalStatus.Team[ OUR_TEAM ].bLastID)
		{
			this->pKeyRing = (KEY_ON_RING *) MemAlloc( NUM_KEYS * sizeof( KEY_ON_RING ) );
			memset(  this->pKeyRing , 0, NUM_KEYS * sizeof( KEY_ON_RING ) );

			for( iCounter = 0; iCounter < NUM_KEYS; iCounter++ )
			{
				this->pKeyRing[ iCounter ].ubKeyID = INVALID_KEY_NUMBER;
			}
		}
		else
		{
			this->pKeyRing = NULL;
		}
		// Create frame cache
		if( InitAnimationCache( usSoldierID, &( this->AnimCache) ) == FALSE )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed animation cache creation" ) );
			break;
		}

		if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			// Init new soldier state
			// OFFSET FIRST ANIMATION FRAME FOR NEW MERCS
			if ( usState != STANDING )
			{
				this->EVENT_InitNewSoldierAnim( usState, (UINT8)0, TRUE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( usState, (UINT8)Random( 10 ), TRUE );
			}
		}
		else
		{
			/// if we don't have a world loaded, and are in a bad anim, goto standing.
			// bad anims are: HOPFENCE,
			// CLIMBDOWNROOF, FALLFORWARD_ROOF,FALLOFF, CLIMBUPROOF
			if( !gfWorldLoaded &&
				( usState == HOPFENCE ||
				usState == CLIMBDOWNROOF ||
				usState == FALLFORWARD_ROOF ||
				usState == FALLOFF ||
				usState == CLIMBUPROOF ) )
			{
				this->EVENT_InitNewSoldierAnim( STANDING, 0, TRUE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( usState, this->usAniCode, TRUE );
			}

		}


		//if ( this->pBackGround != NULL )
		//	MemFree( this->pBackGround );

		// INIT ANIMATION DATA
		//if((this->pBackGround=MemAlloc(SOLDIER_UNBLIT_SIZE))==NULL)
		//{
		//	DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed unblit memory allocation" ) );
		//	break;
		//}
		//memset(this->pBackGround, 0, SOLDIER_UNBLIT_SIZE);

		//if((this->pZBackground=MemAlloc(SOLDIER_UNBLIT_SIZE))==NULL)
		//{
		//	DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed unblit memory allocation" ) );
		//	break;
		//}
		//memset(this->pZBackground, 0, SOLDIER_UNBLIT_SIZE);

		// Init palettes
		if( this->CreateSoldierPalettes( ) == FALSE )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed in creating soldier palettes" ) );
			break;
		}

		fSuccess = TRUE;

	} while( FALSE );

	if ( !fSuccess )
	{
		this->DeleteSoldier( );
	}

	return( fSuccess );

}



BOOLEAN SOLDIERTYPE::DeleteSoldier( void )
{
	UINT32		cnt;
	INT32			iGridNo;
	INT8			bDir;
	BOOLEAN		fRet;

	if ( this != NULL )
	{
		//if(this->pBackGround!=NULL)
		//MemFree(this->pBackGround);

		//if(this->pZBackground!=NULL)
		//MemFree(this->pZBackground);

		if( this->sGridNo != NOWHERE )
		{
			// Remove adjacency records
			for (bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++)
			{
				iGridNo = this->sGridNo + DirIncrementer[ bDir ];
				if( iGridNo >= 0 && iGridNo < WORLD_MAX )
				{
					gpWorldLevelData[ iGridNo ].ubAdjacentSoldierCnt--;
				}
			}
		}

		// Delete key ring
		if (this->pKeyRing)
		{
			MemFree( this->pKeyRing );
			this->pKeyRing = NULL;
		}

		// Delete faces
		DeleteSoldierFace( this );

		// FREE PALETTES
		if ( this->p8BPPPalette != NULL )
		{
			MemFree( this->p8BPPPalette );
			this->p8BPPPalette = NULL;
		}

		if ( this->p16BPPPalette != NULL )
		{
			MemFree( this->p16BPPPalette );
			this->p16BPPPalette = NULL;
		}

		for ( cnt = 0; cnt < NUM_SOLDIER_SHADES; cnt++ )
		{
			if ( this->pShades[ cnt ] != NULL )
			{
				MemFree( this->pShades[ cnt ] );
				this->pShades[ cnt ] = NULL;
			}
		}
		for ( cnt = 0; cnt < NUM_SOLDIER_EFFECTSHADES; cnt++ )
		{
			if ( this->pEffectShades[ cnt ] != NULL )
			{
				MemFree( this->pEffectShades[ cnt ] );
				this->pEffectShades[ cnt ] = NULL;
			}
		}

		// Delete glows
		for ( cnt = 0; cnt < 20; cnt++ )
		{
			if ( this->pGlowShades[ cnt ] != NULL )
			{
				MemFree( this->pGlowShades[ cnt ] );
				this->pGlowShades[ cnt ] = NULL;
			}

		}


		if ( this->ubBodyType == QUEENMONSTER )
		{
			DeletePositionSnd( this->iPositionSndID );
		}

		// Free any animations we may have locked...
		UnLoadCachedAnimationSurfaces( this->ubID, &( this->AnimCache) );

		// Free Animation cache
		DeleteAnimationCache( this->ubID, &( this->AnimCache) );

		// Soldier is not active
		this->bActive = FALSE;

		// Remove light
		this->DeleteSoldierLight( );

		// Remove reseved movement value
		UnMarkMovementReserved( this );

	}

	// REMOVE SOLDIER FROM SLOT!
	fRet = RemoveMercSlot( this );

	if (!fRet)
	{
		RemoveAwaySlot( this );
	}

	return( TRUE );
}

BOOLEAN SOLDIERTYPE::CreateSoldierLight( void )
{
	if ( this->bTeam != gbPlayerNum )
	{
		return( FALSE );
	}

	// DO ONLY IF WE'RE AT A GOOD LEVEL
	if ( this->iLight == -1 )
	{
		// ATE: Check for goggles in headpos....
		if ( GetTotalVisionRangeBonus( this, NORMAL_LIGHTLEVEL_NIGHT ) >= UVGOGGLES_BONUS )
		{
			if( ( this->iLight=LightSpriteCreate("Light4", 0 ) )==(-1))
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return( FALSE );
			}
			else
			{
				LightSprites[ this->iLight ].uiFlags |= MERC_LIGHT;
			}
		}
		else if ( GetTotalVisionRangeBonus( this, NORMAL_LIGHTLEVEL_NIGHT ) >= NIGHTSIGHTGOGGLES_BONUS )
		{
			if( ( this->iLight=LightSpriteCreate("Light3", 0 ) )==(-1))
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return( FALSE );
			}
			else
			{
				LightSprites[ this->iLight ].uiFlags |= MERC_LIGHT;
			}
		}
		else
		{
			if( ( this->iLight=LightSpriteCreate("Light2", 0 ) )==(-1))
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return( FALSE );
			}
			else
			{
				LightSprites[ this->iLight ].uiFlags |= MERC_LIGHT;
			}
		}

		if ( this->pathing.bLevel != 0 )
		{
			LightSpriteRoofStatus(this->iLight, TRUE );
		}
	}

	return( TRUE );
}

BOOLEAN SOLDIERTYPE::ReCreateSoldierLight( void  )
{
	if ( this->bTeam != gbPlayerNum )
	{
		return( FALSE );
	}

	if ( !this->bActive )
	{
		return( FALSE );
	}

	if ( !this->bInSector )
	{
		return( FALSE );
	}

	// Delete Light!
	this->DeleteSoldierLight( );

	if ( this->iLight == -1 )
	{
		this->CreateSoldierLight( );
	}

	return( TRUE );
}


BOOLEAN ReCreateSelectedSoldierLight(  )
{
	SOLDIERTYPE *pSoldier;

	if ( gusSelectedSoldier == NOBODY )
	{
		return( FALSE );
	}

	pSoldier = MercPtrs[ gusSelectedSoldier ];

	return( pSoldier->ReCreateSoldierLight( ) );
}


BOOLEAN SOLDIERTYPE::DeleteSoldierLight( void )
{
	if( this->iLight!=(-1) )
	{
		LightSpriteDestroy( this->iLight );
		this->iLight = -1;
	}

	return( TRUE );
}

// FUNCTIONS CALLED BY EVENT PUMP
/////////////////////////////////

BOOLEAN SOLDIERTYPE::ChangeSoldierState( UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce )
{
	EV_S_CHANGESTATE	SChangeState;

	// Send message that we have changed states
	SChangeState.usNewState				 = usNewState;
	SChangeState.usSoldierID			 = this->ubID;
	SChangeState.uiUniqueId				 = this->uiUniqueSoldierIdValue;
	SChangeState.usStartingAniCode = usStartingAniCode;
	SChangeState.sXPos						 = this->sX;
	SChangeState.sYPos						 = this->sY;
	SChangeState.fForce						 = fForce;
	SChangeState.usNewDirection = this->ubDirection;


	//AddGameEvent( S_CHANGESTATE, 0, &SChangeState );
	if((is_server && this->ubID < 120)||(is_client && this->ubID < 20))
	{
		send_changestate(&SChangeState);
	}
	//else if((is_client && !is_server) && (this->ubID < 20 || (this->ubID < 120 && gTacticalStatus.ubCurrentTeam == OUR_TEAM)))
	//{
	//	this->EVENT_InitNewSoldierAnim( SChangeState.usNewState, SChangeState.usStartingAniCode, SChangeState.fForce );
	//	send_changestate(&SChangeState);
	//}
	//else if (!is_client)
	//{
		this->EVENT_InitNewSoldierAnim( SChangeState.usNewState, SChangeState.usStartingAniCode, SChangeState.fForce );
	//}
	return( TRUE );
}


// This function reevaluates the stance if the guy sees us!
BOOLEAN ReevaluateEnemyStance( SOLDIERTYPE *pSoldier, UINT16 usAnimState )
{
	INT32		cnt, iClosestEnemy = NOBODY;
	INT16		sTargetXPos, sTargetYPos;
	BOOLEAN	fReturnVal = FALSE;
	INT16		sDist, sClosestDist = 10000;

	// make the chosen one not turn to face us
	if ( OK_ENEMY_MERC( pSoldier ) && pSoldier->ubID != gTacticalStatus.ubTheChosenOne && gAnimControl[ usAnimState ].ubEndHeight == ANIM_STAND && !( pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL) )
	{
		if ( pSoldier->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_OFF )
		{
			// If we are a queen and see enemies, goto ready
			if ( pSoldier->ubBodyType == QUEENMONSTER )
			{
				if ( gAnimControl[ usAnimState ].uiFlags & ( ANIM_BREATH ) )
				{
					if ( pSoldier->aiData.bOppCnt > 0 )
					{
						pSoldier->EVENT_InitNewSoldierAnim( QUEEN_INTO_READY, 0 , TRUE );
						return( TRUE );
					}
				}
			}

			// ATE: Don't do this if we're not a merc.....
			if ( !IS_MERC_BODY_TYPE( pSoldier ) )
			{
				return( FALSE );
			}

			if ( gAnimControl[ usAnimState ].uiFlags & ( ANIM_MERCIDLE | ANIM_BREATH ) )
			{
				if ( pSoldier->aiData.bOppCnt > 0 )
				{
					// Pick a guy this buddy sees and turn towards them!
					for ( cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++ )
					{
						if ( pSoldier->aiData.bOppList[ cnt ] == SEEN_CURRENTLY )
						{
							sDist = PythSpacesAway( pSoldier->sGridNo, MercPtrs[ cnt ]->sGridNo );
							if (sDist < sClosestDist)
							{
								sClosestDist = sDist;
								iClosestEnemy = cnt;
							}
						}
					}

					if (iClosestEnemy != NOBODY)
					{
						// Change to fire ready animation
						ConvertGridNoToXY( MercPtrs[ iClosestEnemy ]->sGridNo, &sTargetXPos, &sTargetYPos );

						pSoldier->flags.fDontChargeReadyAPs = TRUE;

						// Ready weapon
						fReturnVal = pSoldier->SoldierReadyWeapon( sTargetXPos, sTargetYPos, FALSE );

						return( fReturnVal );
					}

				}
			}
		}
	}
	return( FALSE );

}


void CheckForFreeupFromHit( SOLDIERTYPE *pSoldier, UINT32 uiOldAnimFlags, UINT32 uiNewAnimFlags, UINT16 usOldAniState, UINT16 usNewState )
{
	// THIS COULD POTENTIALLY CALL EVENT_INITNEWAnim() if the GUY was SUPPRESSED
	// CHECK IF THE OLD ANIMATION WAS A HIT START THAT WAS NOT FOLLOWED BY A HIT FINISH
	// IF SO, RELEASE ATTACKER FROM ATTACKING

	// If old and new animations are the same, do nothing!
	if ( usOldAniState == QUEEN_HIT && usNewState == QUEEN_HIT )
	{
		return;
	}

	if ( usOldAniState != usNewState && ( uiOldAnimFlags & ANIM_HITSTART ) && !( uiNewAnimFlags & ANIM_HITFINISH ) && !( uiNewAnimFlags & ANIM_IGNOREHITFINISH ) && !(pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT ) )
	{
		// 0verhaul:  Yet again, this is handled by the state transition code.
		// Release attacker
		// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, normal hit animation ended NEW: %s ( %d ) OLD: %s ( %d )", gAnimControl[ usNewState ].zAnimStr, usNewState, gAnimControl[ usOldAniState ].zAnimStr, pSoldier->usOldAniState ) );
		// ReleaseSoldiersAttacker( pSoldier );

		//FREEUP GETTING HIT FLAG
		// pSoldier->flags.fGettingHit = FALSE;

		// ATE: if our guy, have 10% change of say damn, if still conscious...
		if ( pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE )
		{
			if ( Random( 10 ) == 0 )
			{
				pSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_CURSE1 ) );
			}
		}
	}

	// CHECK IF WE HAVE FINSIHED A HIT WHILE DOWN
	// OBLY DO THIS IF 1 ) We are dead already or 2 ) We are alive still
	if ( ( uiOldAnimFlags & ANIM_HITWHENDOWN ) && ( ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) || pSoldier->stats.bLife != 0 ) )
	{
		// 0verhaul:  Ditto
		// Release attacker
		// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, animation of kill on the ground ended") );
		// ReleaseSoldiersAttacker( pSoldier );

		//FREEUP GETTING HIT FLAG
		// pSoldier->flags.fGettingHit = FALSE;

		if ( pSoldier->stats.bLife == 0 )
		{
			//ATE: Set previous attacker's value!
			// This is so that the killer can say their killed quote....
			pSoldier->ubAttackerID = pSoldier->ubPreviousAttackerID;
		}
	}
}


// THIS IS CALLED FROM AN EVENT ( S_CHANGESTATE )!
BOOLEAN SOLDIERTYPE::EVENT_InitNewSoldierAnim( UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"EVENT_InitNewSoldierAnim");
	INT16  sNewGridNo = 0;
	INT16		sAPCost = 0;
	INT16		sBPCost = 0;
	UINT32	uiOldAnimFlags;
	UINT32  uiNewAnimFlags;
	UINT16	usSubState;
	UINT16	usItem;
	BOOLEAN	fTryingToRestart = FALSE;

	CHECKF( usNewState < NUMANIMATIONSTATES );

	///////////////////////////////////////////////////////////////////////
	//			DO SOME CHECKS ON OUR NEW ANIMATION!
	/////////////////////////////////////////////////////////////////////

	// If we are NOT loading a game, continue normally
	if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		// CHECK IF WE ARE TRYING TO INTURRUPT A SCRIPT WHICH WE DO NOT WANT INTERRUPTED!
		if ( this->flags.fInNonintAnim )
		{
			return( FALSE );
		}

		if ( this->flags.fRTInNonintAnim )
		{
			if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
			{
				return( FALSE );
			}
			else
			{
				this->flags.fRTInNonintAnim = FALSE;
			}
		}


		// Check if we can restart this animation if it's the same as our current!
		if ( usNewState == this->usAnimState )
		{
			if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_NORESTART ) && !fForce )
			{
				fTryingToRestart = TRUE;
			}
		}

		// Check state, if we are not at the same height, set this ani as the pending one and
		// change stance accordingly
		// ATE: ONLY IF WE ARE STARTING AT START OF ANIMATION!
		if ( usStartingAniCode == 0 )
		{
			if ( gAnimControl[ usNewState ].ubHeight != gAnimControl[ this->usAnimState ].ubEndHeight &&
				!( gAnimControl[ usNewState ].uiFlags & ( ANIM_STANCECHANGEANIM | ANIM_IGNORE_AUTOSTANCE ) ) )
			{

				// Check if we are going from crouched height to prone height, and adjust fast turning accordingly
				// Make guy turn while crouched THEN go into prone
				if ( ( gAnimControl[ usNewState ].ubEndHeight == ANIM_PRONE && gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_CROUCH ) && !( gTacticalStatus.uiFlags & INCOMBAT ) )
				{
					this->flags.fTurningUntilDone = TRUE;
					this->ubPendingStanceChange = gAnimControl[ usNewState ].ubEndHeight;
					this->usPendingAnimation		= usNewState;
					return( TRUE );
				}
				// Check if we are in realtime and we are going from stand to crouch
				else if ( gAnimControl[ usNewState ].ubEndHeight == ANIM_CROUCH && gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_STAND && ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_MOVING ) && ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
				{
					this->ubDesiredHeight = gAnimControl[ usNewState ].ubEndHeight;
					// Continue with this course of action IE: Do animation and skip from stand to crouch
				}
				// Check if we are in realtime and we are going from crouch to stand
				else if ( gAnimControl[ usNewState ].ubEndHeight == ANIM_STAND && gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_CROUCH && ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_MOVING ) && ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) && this->usAnimState != HELIDROP )
				{
					this->ubDesiredHeight = gAnimControl[ usNewState ].ubEndHeight;
					// Continue with this course of action IE: Do animation and skip from stand to crouch
				}
				else
				{
					// ONLY DO FOR EVERYONE BUT PLANNING GUYS
					if ( this->ubID < MAX_NUM_SOLDIERS )
					{
						// Set our next moving animation to be pending, after
						this->usPendingAnimation = usNewState;
						// Set new state to be animation to move to new stance
						SendChangeSoldierStanceEvent( this, gAnimControl[ usNewState ].ubHeight );
						return( TRUE );
					}
				}
			}
		}

		if ( usNewState == ADJACENT_GET_ITEM )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				EVENT_InternalSetSoldierDesiredDirection( this, this->ubPendingDirection, FALSE, this->usAnimState );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = ADJACENT_GET_ITEM;
				this->flags.fTurningUntilDone	 = TRUE;
				this->SoldierGotoStationaryStance( );
				return( TRUE );
			}
		}


		if ( usNewState == CLIMBUPROOF )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = CLIMBUPROOF;
				this->flags.fTurningUntilDone	 = TRUE;
				this->SoldierGotoStationaryStance( );
				return( TRUE );
			}
		}

		if ( usNewState == CLIMBDOWNROOF )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = CLIMBDOWNROOF;
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
				this->flags.fTurningUntilDone	 = TRUE;
				this->SoldierGotoStationaryStance( );
				return( TRUE );
			}
		}

		// ATE: Don't raise/lower automatically if we are low on health,
		// as our gun looks lowered anyway....
		//if ( this->stats.bLife > INJURED_CHANGE_THREASHOLD )
		{
			// Don't do some of this if we are a monster!
			// ATE: LOWER AIMATION IS GOOD, RAISE ONE HOWEVER MAY CAUSE PROBLEMS FOR AI....
			if ( !(this->flags.uiStatusFlags & SOLDIER_MONSTER ) && this->ubBodyType != ROBOTNOWEAPON && this->bTeam == gbPlayerNum )
			{
				// If this animation is a raise_weapon animation
				if ( ( gAnimControl[ usNewState ].uiFlags & ANIM_RAISE_WEAPON ) && !( gAnimControl[ this->usAnimState ].uiFlags & ( ANIM_RAISE_WEAPON | ANIM_NOCHANGE_WEAPON ) ) )
				{
					// We are told that we need to rasie weapon
					// Do so only if
					// 1) We have a rifle in hand...
					usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[ HANDPOS ].exists() == true && (Item[ usItem ].twohanded ) && !Item[usItem].rocketlauncher )
					{
						// Switch on height!
						switch( gAnimControl[ this->usAnimState ].ubEndHeight )
						{
						case ANIM_STAND:

							// 2) OK, all's fine... lower weapon first....
							this->usPendingAnimation = usNewState;
							// Set new state to be animation to move to new stance
							usNewState = RAISE_RIFLE;
						}
					}
				}

				// If this animation is a lower_weapon animation
				if ( ( gAnimControl[ usNewState ].uiFlags & ANIM_LOWER_WEAPON ) && !( gAnimControl[ this->usAnimState ].uiFlags & ( ANIM_LOWER_WEAPON | ANIM_NOCHANGE_WEAPON ) ) )
				{
					// We are told that we need to rasie weapon
					// Do so only if
					// 1) We have a rifle in hand...
					usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[ HANDPOS ].exists() == true && (Item[ usItem ].twohanded ) && !Item[usItem].rocketlauncher )
					{
						// Switch on height!
						switch( gAnimControl[ this->usAnimState ].ubEndHeight )
						{
						case ANIM_STAND:

							// 2) OK, all's fine... lower weapon first....
							this->usPendingAnimation = usNewState;
							// Set new state to be animation to move to new stance
							usNewState = LOWER_RIFLE;
						}
					}
				}
			}
		}

		// Are we cowering and are tyring to move, getup first...
		if ( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING && this->usAnimState == COWERING && gAnimControl[ usNewState ].ubEndHeight == ANIM_STAND )
		{
			this->usPendingAnimation = usNewState;
			// Set new state to be animation to move to new stance
			usNewState = END_COWER;
		}

		// If we want to start swatting, put a pending animation
		if( this->usAnimState != START_SWAT && usNewState == SWATTING )
		{
			// Set new state to be animation to move to new stance
			usNewState = START_SWAT;
		}

		if( this->usAnimState == SWATTING && usNewState == CROUCHING )
		{
			// Set new state to be animation to move to new stance
			usNewState = END_SWAT;
		}

		if( this->usAnimState == WALKING && usNewState == STANDING && this->stats.bLife < INJURED_CHANGE_THREASHOLD && this->ubBodyType <= REGFEMALE && !this->MercInWater( ) )
		{
			// Set new state to be animation to move to new stance
			usNewState = END_HURT_WALKING;
		}

		// Check if we are an enemy, and we are in an animation what should be overriden
		// by if he sees us or not.
		if ( ReevaluateEnemyStance( this, usNewState ) )
		{
			return( TRUE );
		}

		// OK.......
		if ( this->ubBodyType > REGFEMALE )
		{
			if ( this->stats.bLife < INJURED_CHANGE_THREASHOLD )
			{
				if ( usNewState == READY_RIFLE_STAND )
				{
					//	this->usPendingAnimation2 = usNewState;
					//	usNewState = FROM_INJURED_TRANSITION;
				}
			}
		}

		// Alrighty, check if we should free buddy up!
		if ( usNewState == GIVING_AID )
		{
			UnSetUIBusy( this->ubID );
		}


		// SUBSTITUDE VARIOUS REG ANIMATIONS WITH ODD BODY TYPES
		if ( SubstituteBodyTypeAnimation( this, usNewState, &usSubState ) )
		{
			usNewState = usSubState;
		}

		// CHECK IF WE CAN DO THIS ANIMATION!
		if ( IsAnimationValidForBodyType( this, usNewState ) == FALSE )
		{
			return( FALSE );
		}

		// OK, make guy transition if a big merc...
		if ( this->uiAnimSubFlags & SUB_ANIM_BIGGUYTHREATENSTANCE )
		{
			if ( usNewState == KNEEL_DOWN && this->usAnimState != BIGMERC_CROUCH_TRANS_INTO )
			{
				UINT16 usItem;

				// Do we have a rifle?
				usItem = this->inv[ HANDPOS ].usItem;

				if ( this->inv[ HANDPOS ].exists() == true )
				{
					if ( Item[ usItem ].usItemClass == IC_GUN && !Item[usItem].rocketlauncher )
					{
						//						if ( (Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
						if ( (Item[ usItem ].twohanded ) )
						{
							usNewState = BIGMERC_CROUCH_TRANS_INTO;
						}
					}
				}
			}

			if ( usNewState == KNEEL_UP && this->usAnimState != BIGMERC_CROUCH_TRANS_OUTOF )
			{
				UINT16 usItem;

				// Do we have a rifle?
				usItem = this->inv[ HANDPOS ].usItem;

				if ( this->inv[ HANDPOS ].exists() == true )
				{
					if ( Item[ usItem ].usItemClass == IC_GUN && !Item[usItem].rocketlauncher )
					{
						//						if ( (Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
						if ( (Item[ usItem ].twohanded ) )
						{
							usNewState = BIGMERC_CROUCH_TRANS_OUTOF;
						}
					}
				}
			}
		}

		// OK, if we have reverse set, do the side step!
		if ( this->bReverse )
		{
			if ( usNewState == WALKING || usNewState == RUNNING || usNewState == SWATTING )
			{
				// CHECK FOR SIDEWAYS!
				if ( this->ubDirection == gPurpendicularDirection[ this->ubDirection ][ this->pathing.usPathingData[ this->pathing.usPathIndex ] ] )
				{
					// We are perpendicular!
					usNewState = SIDE_STEP;
				}
				else
				{
					if ( gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_CROUCH )
					{
						usNewState = SWAT_BACKWARDS;
					}
					else
					{
						// Here, change to  opposite direction
						usNewState = WALK_BACKWARDS;
					}
				}
			}
		}

		// ATE: Patch hole for breath collapse for roofs, fences
		if ( usNewState == CLIMBUPROOF || usNewState == CLIMBDOWNROOF || usNewState == HOPFENCE )
		{
			// Check for breath collapse if a given animation like
			if ( this->CheckForBreathCollapse( ) || this->bCollapsed )
			{
				// UNset UI
				UnSetUIBusy( this->ubID );

				SoldierCollapse( this );

				this->bBreathCollapsed = FALSE;

				return( FALSE );

			}
		}

		// If we are in water.....and trying to run, change to run
		if ( this->MercInWater() )
		{
			// Check animation
			// Change to walking
			if ( usNewState == RUNNING )
			{
				usNewState = WALKING;
			}
		}

		// Turn off anipause flag for any anim!
		this->flags.uiStatusFlags &= (~SOLDIER_PAUSEANIMOVE);

		// Unset paused for no APs.....
		this->AdjustNoAPToFinishMove( FALSE );

#if 0
		// 0verhaul:  This is a test.  The only time I have been able to make this code hit is when
		// the player goes prone while moving.  And that is not what this part is intended for.  I 
		// have seen the soldier in the middle of crawling, get up, turn, and then go prone again to
		// continue along his path.  But this code was not hit for that part.  And this code seems
		// to be made for that part.  So apparently they found another way to deal with it.  So
		// I disabled the "locked" code for usDontUpdateNewGridNoOnMoveAnimChange since it can cause
		// problems of its own.  Now we see if we can do without this part too.
		if ( usNewState == CRAWLING && this->usDontUpdateNewGridNoOnMoveAnimChange == 1 )
		{
			if ( this->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE )
			{
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_START_UP_FROM_MOVE;
			}

			// ATE: IF we are starting to crawl, but have to getup to turn first......
			if ( this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_START_UP_FROM_MOVE )
			{
				usNewState = PRONE_UP;
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE;
			}
		}
#endif

		// We are about to start moving
		// Handle buddy beginning to move...
		// check new gridno, etc
		// ATE: Added: Make check that old anim is not a moving one as well
		if ( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING && !( gAnimControl[ this->usAnimState ].uiFlags & ANIM_MOVING ) || ( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING && fForce ) )
		{
			BOOLEAN fKeepMoving;

			if ( usNewState == CRAWLING && this->usDontUpdateNewGridNoOnMoveAnimChange == LOCKED_NO_NEWGRIDNO )
			{
				// Turn off lock once we are crawling once...
				this->usDontUpdateNewGridNoOnMoveAnimChange = 1;
			}

			// ATE: Additional check here if we have just been told to update animation ONLY, not goto gridno stuff...
			if ( !this->usDontUpdateNewGridNoOnMoveAnimChange  )
			{
				if ( usNewState != SWATTING  )
				{
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Handling New gridNo for %d: Old %s, New %s", this->ubID, gAnimControl[ this->usAnimState ].zAnimStr , gAnimControl[ usNewState ].zAnimStr)  );

					if ( !( gAnimControl[ usNewState ].uiFlags & ANIM_SPECIALMOVE ) )
					{
						// Handle goto new tile...
						if ( HandleGotoNewGridNo( this, &fKeepMoving, TRUE, usNewState ) )
						{
							if ( !fKeepMoving )
							{
								return( FALSE );
							}

							// Make sure desy = zeroed out...
							// this->flags.fPastXDest = this->flags.fPastYDest = FALSE;
						}
						else
						{
							if ( this->bBreathCollapsed )
							{
								// UNset UI
								UnSetUIBusy( this->ubID );

								SoldierCollapse( this );

								this->bBreathCollapsed = FALSE;
							}
							return( FALSE );
						}
					}
					else
					{
						// Change desired direction
						// Just change direction
						this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[ this->pathing.usPathIndex ], FALSE, this->usAnimState );
					}

					//check for services
					this->ReceivingSoldierCancelServices( );
					this->GivingSoldierCancelServices( );


					// Check if we are a vehicle, and start playing noise sound....
					if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
					{
						HandleVehicleMovementSound( this, TRUE );
					}
				}
			}
		}
		else
		{
			// Check for stopping movement noise...
			if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
			{
				HandleVehicleMovementSound( this, FALSE );

				// If a vehicle, set hewight to 0
				this->SetSoldierHeight( (FLOAT)( 0 ) );
			}

		}

		// Reset to false always.....
		// ( Unless locked )
		if ( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING )
		{
			// 0verhaul:  **** Special hack!!!!
			//   If a merc begins to go prone while moving, the LOCKED_NO_NEWGRIDNO is set.  If the merc never finishes
			// going prone, either due to interrupting the stance change with a different stance change, or other possible
			// factors such as maybe getting shot (this is realtime so an enemy could see him), it stays on locked.  Once
			// it stays on locked, the soldier will be unable to navigate around obstacles but will simply stay put
			// twitching.  Since the LOCKED is only set when going prone, this unsets it.
			if ( this->usDontUpdateNewGridNoOnMoveAnimChange != LOCKED_NO_NEWGRIDNO || 
				(this->usDontUpdateNewGridNoOnMoveAnimChange == LOCKED_NO_NEWGRIDNO && this->usAnimState != PRONE_DOWN))
			{
				this->usDontUpdateNewGridNoOnMoveAnimChange = FALSE;
			}
		}

		if ( fTryingToRestart )
		{
			return( FALSE );
		}

	}


	// ATE: If this is an AI guy.. unlock him!
	if ( gTacticalStatus.fEnemySightingOnTheirTurn )
	{
		if ( gTacticalStatus.ubEnemySightingOnTheirTurnEnemyID == this->ubID )
		{
			this->flags.fPauseAllAnimation = FALSE;
			gTacticalStatus.fEnemySightingOnTheirTurn = FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////
	//			HERE DOWN - WE HAVE MADE A DESCISION!
	/////////////////////////////////////////////////////////////////////

	uiOldAnimFlags = gAnimControl[ this->usAnimState ].uiFlags;
	uiNewAnimFlags = gAnimControl[ usNewState ].uiFlags;

	sNewGridNo = NewGridNo( this->sGridNo, DirectionInc( (UINT8) this->pathing.usPathingData[ this->pathing.usPathIndex ] ) );


	// CHECKING IF WE HAVE A HIT FINISH BUT NO DEATH IS DONE WITH A SPECIAL ANI CODE
	// IN THE HIT FINSIH ANI SCRIPTS

	// CHECKING IF WE HAVE FINISHED A DEATH ANIMATION IS DONE WITH A SPECIAL ANI CODE
	// IN THE DEATH SCRIPTS


	// CHECK IF THIS NEW STATE IS NON-INTERRUPTABLE
	// IF SO - SET NON-INT FLAG
	// 0verhaul:  Okay, here is a question:  Is the "non-interrupt" supposed to be transferrable to other anims?
	// That is, if one anim is not interruptable but it chains to another anim, should the "not interruptable" flag
	// remain?  I'm going to try out the theory that new animations should reset the "don't interrupt" flag.
#if 0
	if ( uiNewAnimFlags & ANIM_NONINTERRUPT )
	{
		this->flags.fInNonintAnim = TRUE;
	}

	if ( uiNewAnimFlags & ANIM_RT_NONINTERRUPT )
	{
		this->flags.fRTInNonintAnim = TRUE;
	}
#else
	this->flags.fInNonintAnim = (uiNewAnimFlags & ANIM_NONINTERRUPT) != 0;
	this->flags.fRTInNonintAnim = (uiNewAnimFlags & ANIM_RT_NONINTERRUPT) != 0;
#endif

	// CHECK IF WE ARE NOT AIMING, IF NOT, RESET LAST TAGRET!
	if ( !(gAnimControl[ this->usAnimState ].uiFlags & ANIM_FIREREADY ) && !(gAnimControl[ usNewState ].uiFlags & ANIM_FIREREADY ) )
	{
		// ATE: Also check for the transition anims to not reset this
		// this should have used a flag but we're out of them....
		if ( usNewState != READY_RIFLE_STAND && usNewState != READY_RIFLE_PRONE && usNewState != READY_RIFLE_CROUCH && usNewState != ROBOT_SHOOT )
		{
			this->sLastTarget = NOWHERE;
		}
	}

	// If a special move state, release np aps
	if ( ( gAnimControl[ usNewState ].uiFlags & ANIM_SPECIALMOVE ) )
	{
		this->AdjustNoAPToFinishMove( FALSE );
	}

	if ( gAnimControl[ usNewState ].uiFlags & ANIM_UPDATEMOVEMENTMODE )
	{
		if ( this->bTeam == gbPlayerNum )
		{
			// this->usUIMovementMode =  GetMoveStateBasedOnStance( this, gAnimControl[ usNewState ].ubEndHeight );
		}
	}

	// ATE: If not a moving animation - turn off reverse....
	if ( !( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING ) )
	{
		this->bReverse = FALSE;
	}

	// ONLY DO FOR EVERYONE BUT PLANNING GUYS
	if ( this->ubID < MAX_NUM_SOLDIERS )
	{

		// Do special things based on new state
		// CHRISL: Make changes so that we charge extra APs while wearing a backpack while using new inventory system
		switch( usNewState )
		{
		case STANDING:

			// Update desired height
			this->ubDesiredHeight		 = ANIM_STAND;
			break;

		case CROUCHING:

			// Update desired height
			this->ubDesiredHeight		 = ANIM_CROUCH;
			break;

		case PRONE:

			// Update desired height
			this->ubDesiredHeight		 = ANIM_PRONE;
			break;

		case READY_RIFLE_STAND:
		case READY_RIFLE_PRONE:
		case READY_RIFLE_CROUCH:
		case READY_DUAL_STAND:
		case READY_DUAL_CROUCH:
		case READY_DUAL_PRONE:

			// OK, get points to ready weapon....
			if ( !this->flags.fDontChargeReadyAPs )
			{
				sAPCost = GetAPsToReadyWeapon( this, usNewState );
				DeductPoints( this, sAPCost, sBPCost );
			}
			else
			{
				this->flags.fDontChargeReadyAPs = FALSE;
			}
			break;

		case WALKING:

			this->usPendingAnimation = NO_PENDING_ANIMATION;
			this->aiData.ubPendingActionAnimCount = 0;
			break;

		case SWATTING:

			this->usPendingAnimation = NO_PENDING_ANIMATION;
			this->aiData.ubPendingActionAnimCount = 0;
			break;

		case CRAWLING:

			// Turn off flag...
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
			this->aiData.ubPendingActionAnimCount = 0;
			this->usPendingAnimation = NO_PENDING_ANIMATION;
			break;

		case RUNNING:

			// Only if our previous is not running
			if ( this->usAnimState != RUNNING )
			{
				// CHRISL
				if((UsingNewInventorySystem() == true) && this->inv[BPACKPOCKPOS].exists() == true)
				{
					sAPCost = AP_START_RUN_COST + 2;
					sBPCost += 2;
				}
				else
					sAPCost = AP_START_RUN_COST;
				DeductPoints( this, sAPCost, sBPCost );
			}
			// Set pending action count to 0
			this->aiData.ubPendingActionAnimCount = 0;
			this->usPendingAnimation = NO_PENDING_ANIMATION;
			break;

		case ADULTMONSTER_WALKING:
			this->aiData.ubPendingActionAnimCount = 0;
			break;

		case ROBOT_WALK:
			this->aiData.ubPendingActionAnimCount = 0;
			break;

		case KNEEL_UP:
		case KNEEL_DOWN:
		case BIGMERC_CROUCH_TRANS_INTO:
		case BIGMERC_CROUCH_TRANS_OUTOF:

			if ( !this->flags.fDontChargeAPsForStanceChange )
			{
				// CHRISL
				if((UsingNewInventorySystem() == true) && this->inv[BPACKPOCKPOS].exists() == true && !this->flags.ZipperFlag)
				{
					if(usNewState == KNEEL_UP || usNewState == BIGMERC_CROUCH_TRANS_OUTOF)
					{
						sAPCost=AP_CROUCH+2;
						sBPCost=BP_CROUCH+2;
					}
					else if(usNewState == KNEEL_DOWN || usNewState == BIGMERC_CROUCH_TRANS_INTO)
					{
						sAPCost=AP_CROUCH+1;
						sBPCost=BP_CROUCH+1;
					}
					else
					{
						sAPCost=AP_CROUCH;
						sBPCost=BP_CROUCH;
					}
				}
				else
				{
					sAPCost=AP_CROUCH;
					sBPCost=BP_CROUCH;
				}
				DeductPoints( this, sAPCost, sBPCost );
			}
			this->flags.fDontChargeAPsForStanceChange = FALSE;
			break;

		case PRONE_UP:
		case PRONE_DOWN:

			// ATE: If we are NOT waiting for prone down...
			if ( this->flags.bTurningFromPronePosition < TURNING_FROM_PRONE_START_UP_FROM_MOVE && !this->flags.fDontChargeAPsForStanceChange )
			{
				// ATE: Don't do this if we are still 'moving'....
				if ( this->sGridNo == this->pathing.sFinalDestination || this->pathing.usPathIndex == 0 )
				{
					// CHRISL
					if((UsingNewInventorySystem() == true) && this->inv[BPACKPOCKPOS].exists() == true && !this->flags.ZipperFlag)
					{
						if(usNewState == PRONE_UP)
						{
						sAPCost=AP_PRONE+2;
						sBPCost=BP_PRONE+2;
						}
						else
						{
						sAPCost=AP_PRONE+1;
						sBPCost=BP_PRONE+1;
						}
					}
					else
					{
					sAPCost=AP_PRONE;
					sBPCost=BP_PRONE;
					}
					DeductPoints( this, sAPCost, sBPCost );
				}
			}
			this->flags.fDontChargeAPsForStanceChange = FALSE;
			break;

			//Deduct points for stance change
			//sAPCost = GetAPsToChangeStance( this, gAnimControl[ usNewState ].ubEndHeight );
			//DeductPoints( this, sAPCost, 0 );
			//break;

		case START_AID:

			DeductPoints( this, AP_START_FIRST_AID, BP_START_FIRST_AID );
			break;

		case CUTTING_FENCE:
			DeductPoints( this, AP_USEWIRECUTTERS, BP_USEWIRECUTTERS );
			break;

		case PLANT_BOMB:

			DeductPoints( this, AP_DROP_BOMB, 0 );
			break;

		case STEAL_ITEM:

			DeductPoints( this, AP_STEAL_ITEM, 0 );
			break;

		case CROW_DIE:

			// Delete shadow of crow....
			if ( this->pAniTile != NULL )
			{
				DeleteAniTile( this->pAniTile );
				this->pAniTile = NULL;
			}
			break;

		case CROW_FLY:

			// Ate: startup a shadow ( if gridno is set )
			HandleCrowShadowNewGridNo( this );
			break;

		case CROW_EAT:

			// ATE: Make sure height level is 0....
			this->SetSoldierHeight( (FLOAT)(0) );
			HandleCrowShadowRemoveGridNo( this );
			break;

		case USE_REMOTE:

			DeductPoints( this, AP_USE_REMOTE, 0 );
			break;

			//case PUNCH:

			//Deduct points for punching
			//sAPCost = MinAPsToAttack( this, this->sGridNo, FALSE );
			//DeductPoints( this, sAPCost, 0 );
			//break;

		case HOPFENCE:

			// CHRISL
			if((UsingNewInventorySystem() == true) && this->inv[BPACKPOCKPOS].exists() == true)
				DeductPoints( this, AP_JUMPFENCEBPACK, BP_JUMPFENCEBPACK );
			else
				DeductPoints( this, AP_JUMPFENCE, BP_JUMPFENCE );
			break;

			// Deduct aps for falling down....
		case FALLBACK_HIT_STAND:
		case FALLFORWARD_FROMHIT_STAND:

			DeductPoints( this, AP_FALL_DOWN, BP_FALL_DOWN );
			break;

		case FALLFORWARD_FROMHIT_CROUCH:

			DeductPoints( this, (AP_FALL_DOWN/2), (BP_FALL_DOWN/2) );
			break;

		case QUEEN_SWIPE:

			// ATE: set damage counter...
			this->aiData.uiPendingActionData1 = 0;
			break;

		case CLIMBDOWNROOF:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, AP_CLIMBOFFROOF, BP_CLIMBOFFROOF );
			break;

		case CLIMBUPROOF:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, AP_CLIMBROOF, BP_CLIMBROOF );
			break;

		case JUMP_OVER_BLOCKING_PERSON:

			// Set path....
			{
				INT16 sNewGridNo;

				DeductPoints( this, AP_JUMP_OVER, BP_JUMP_OVER );

				sNewGridNo = NewGridNo( (INT16)this->sGridNo, DirectionInc( this->ubDirection ) );
				sNewGridNo = NewGridNo( (INT16)sNewGridNo, DirectionInc( this->ubDirection ) );

				this->sPlotSrcGrid = this->sGridNo;
				this->flags.fPastXDest = FALSE;
				this->flags.fPastYDest = FALSE;
				this->pathing.usPathDataSize = 0;
				this->pathing.usPathIndex    = 0;
				this->pathing.usPathingData[ this->pathing.usPathDataSize ] = this->ubDirection;
				this->pathing.usPathDataSize++;
				this->pathing.usPathingData[ this->pathing.usPathDataSize ] = this->ubDirection;
				this->pathing.usPathDataSize++;
				this->pathing.sFinalDestination = sNewGridNo;
				// Set direction
				this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[ this->pathing.usPathIndex ], FALSE, JUMP_OVER_BLOCKING_PERSON );
			}
			break;


		case GENERIC_HIT_STAND:
		case GENERIC_HIT_CROUCH:
		case STANDING_BURST_HIT:
		case ADULTMONSTER_HIT:
		case ADULTMONSTER_DYING:
		case COW_HIT:
		case COW_DYING:
		case BLOODCAT_HIT:
		case BLOODCAT_DYING:
		case WATER_HIT:
		case WATER_DIE:
		case DEEP_WATER_HIT:
		case DEEP_WATER_DIE:
		case RIFLE_STAND_HIT:
		case LARVAE_HIT:
		case LARVAE_DIE:
		case QUEEN_HIT:
		case QUEEN_DIE:
		case INFANT_HIT:
		case INFANT_DIE:
		case CRIPPLE_HIT:
		case CRIPPLE_DIE:
		case CRIPPLE_DIE_FLYBACK:
		case ROBOTNW_HIT:
		case ROBOTNW_DIE:

			// Set getting hit flag to TRUE
			this->flags.fGettingHit = TRUE;
			break;

		case CHARIOTS_OF_FIRE:
		case BODYEXPLODING:

			// Merc on fire!
			this->aiData.uiPendingActionData1 = PlaySoldierJA2Sample( this->ubID, ( FIRE_ON_MERC ), RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 5, SoundDir( this->sGridNo ), TRUE );
			break;
		}
	}

	// Remove old animation profile
	this->HandleAnimationProfile( this->usAnimState, TRUE );


	// From animation control, set surface
	if ( SetSoldierAnimationSurface( this, usNewState ) == FALSE )
	{
		return( FALSE );
	}


	// Set state
	this->usOldAniState = this->usAnimState;
	this->sOldAniCode = this->usAniCode;

	// Change state value!
	this->usAnimState = usNewState;
	// Set current frame
	this->usAniCode  = usStartingAniCode;

	// Handle cleanup stuff for getting hit.  Shouldn't this be part of the animation script?
	CheckForFreeupFromHit( this, uiOldAnimFlags, uiNewAnimFlags, this->usOldAniState, usNewState );

	// Perform attack busy stuff
	if (this->usOldAniState != this->usAnimState)
	{
		if ( uiNewAnimFlags & ANIM_ATTACK ) {
			gTacticalStatus.ubAttackBusyCount++;
			DebugAttackBusy( String( "**** Attack animation transfer to %s for %d.\nABC now %d\n", gAnimControl[ usNewState ].zAnimStr, this->ubID, gTacticalStatus.ubAttackBusyCount ) );
		} else if (uiOldAnimFlags & ANIM_ATTACK || this->flags.fChangingStanceDueToSuppression ) {
			DebugAttackBusy( String( "**** Transfer to %s for %d.\n", gAnimControl[ usNewState ].zAnimStr, this->ubID ) );
		}
		if (uiOldAnimFlags & ANIM_ATTACK ) {
			DebugAttackBusy( String( "**** Attack animation transfer from %s for %d.  Reducing ABC.\n", gAnimControl[ this->usOldAniState ].zAnimStr, this->ubID ) );
			ReduceAttackBusyCount( );
		} else if (uiNewAnimFlags & ANIM_ATTACK  || this->flags.fChangingStanceDueToSuppression ) {
			DebugAttackBusy( String( "**** Transfer from %s for %d\n", gAnimControl[ this->usOldAniState ].zAnimStr, this->ubID ) );
		}
	}

	if (this->flags.fChangingStanceDueToSuppression)
	{
		this->flags.fChangingStanceDueToSuppression = FALSE;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of suppression stance change") );
		DebugAttackBusy( String( "@@@@@@@ Freeing up attacker - end of suppression stance change for %d\n", this->ubID ) );
		ReduceAttackBusyCount( );
	}

	this->sZLevelOverride = -1;

	if ( !( this->flags.uiStatusFlags & SOLDIER_LOCKPENDINGACTIONCOUNTER ) )
	{
		//ATE Cancel ANY pending action...
		if ( this->aiData.ubPendingActionAnimCount > 0 && ( gAnimControl[ this->usOldAniState ].uiFlags & ANIM_MOVING ) )
		{
			// Do some special things for some actions
			switch( this->aiData.ubPendingAction )
			{
			case MERC_GIVEITEM:

				// Unset target as enaged
				MercPtrs[ this->aiData.uiPendingActionData4 ]->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION);
				break;
			}
			this->aiData.ubPendingAction		 = NO_PENDING_ACTION;
		}
		else
		{
			// Increment this for almost all animations except some movement ones...
			// That's because this represents ANY animation other than the one we began when the pending action was started
			// ATE: Added to ignore this count if we are waiting for someone to move out of our way...
			if ( usNewState != START_SWAT && usNewState != END_SWAT && !( gAnimControl[ usNewState ].uiFlags & ANIM_NOCHANGE_PENDINGCOUNT ) && !this->flags.fDelayedMovement && !( this->flags.uiStatusFlags & SOLDIER_ENGAGEDINACTION ) )
			{
				this->aiData.ubPendingActionAnimCount++;
			}
		}
	}

	// Set new animation profile
	this->HandleAnimationProfile( usNewState, FALSE );

	// Reset some animation values
	this->flags.fForceShade = FALSE;

	// CHECK IF WE ARE AT AN IDLE ACTION
#if 0
	if ( gAnimControl[ usNewState ].uiFlags & ANIM_IDLE )
	{
		this->aiData.bAction = ACTION_DONE;
	}
	else
	{
		this->aiData.bAction = ACTION_BUSY;
	}
#endif

	// ATE; For some animations that could use some variations, do so....
	if (usNewState == CHARIOTS_OF_FIRE || usNewState == BODYEXPLODING )
	{
		this->usAniCode = (UINT16)( Random( 10 ) );
	}

	// ATE: Default to first frame....
	// Will get changed ( probably ) by AdjustToNextAnimationFrame()
	this->ConvertAniCodeToAniFrame( (INT16)( 0 ) );

	// Set delay speed
	SetSoldierAniSpeed( this );

	// Reset counters
	RESETTIMECOUNTER( this->timeCounters.UpdateCounter, this->sAniDelay );

	// Adjust to new animation frame ( the first one )
	AdjustToNextAnimationFrame( this );

	// Setup offset information for UI above guy
	SetSoldierLocatorOffsets( this );

	// Lesh: test fix visibility after raising gun
	if ( ( gAnimControl[ this->usOldAniState ].uiFlags & ANIM_RAISE_WEAPON) && (gAnimControl[ this->usAnimState ].uiFlags & ANIM_FIREREADY) )
		//equivalent if ( (this->usAnimState == AIM_RIFLE_PRONE) || (this->usAnimState == AIM_RIFLE_CROUCH) || (this->usAnimState == AIM_RIFLE_STAND) )
	{
		if ( (this->usOldAniState == READY_RIFLE_STAND) || (this->usOldAniState == READY_RIFLE_CROUCH) || (this->usOldAniState == READY_RIFLE_PRONE) )
		{
			HandleSight(this,SIGHT_LOOK);
		}
	}

	// If our own guy...
	if ( this->bTeam == gbPlayerNum )
	{
		// Are we stationary?
		if ( gAnimControl[ usNewState ].uiFlags & ANIM_STATIONARY )
		{
			// Position light....
			// this->SetCheckSoldierLightFlag( );
		}
		else
		{
			// Hide light.....
			// this->DeleteSoldierLight( );
		}
	}

	// If we are certain animations, reload palette
	if ( usNewState == VEHICLE_DIE || usNewState == CHARIOTS_OF_FIRE || usNewState == BODYEXPLODING )
	{
		this->CreateSoldierPalettes( );
	}

	// ATE: if the old animation was a movement, and new is not, play sound...
	// OK, play final footstep sound...
	if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_STATIONARY ) &&
			( gAnimControl[ this->usOldAniState ].uiFlags & ANIM_MOVING ) )
		{
			PlaySoldierFootstepSound( this );
		}
	}

	// Free up from stance change
	FreeUpNPCFromStanceChange( this );

	return( TRUE );
}


void SOLDIERTYPE::InternalRemoveSoldierFromGridNo( BOOLEAN fForce )
{
	INT8 bDir;
	INT32 iGridNo;

	if((this->sGridNo!=NOWHERE) )
	{
		if ( this->bInSector || fForce )
		{
			// Remove from world ( old pos )
			RemoveMerc( this->sGridNo, this, FALSE );
			this->HandleAnimationProfile( this->usAnimState, TRUE );

			// Remove records of this guy being adjacent
			for (bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++)
			{
				iGridNo = this->sGridNo + DirIncrementer[ bDir ];
				if( iGridNo >= 0 && iGridNo < WORLD_MAX )
				{
					gpWorldLevelData[ iGridNo ].ubAdjacentSoldierCnt--;
				}
			}

			HandlePlacingRoofMarker( this, this->sGridNo, FALSE , FALSE );

			// Remove reseved movement value
			UnMarkMovementReserved( this );

			HandleCrowShadowRemoveGridNo( this );

			// Reset gridno...
			this->sGridNo = NOWHERE;
		}
	}
}

void SOLDIERTYPE::RemoveSoldierFromGridNo( void )
{
	this->InternalRemoveSoldierFromGridNo( FALSE );
}


void SOLDIERTYPE::EVENT_InternalSetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos ,BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest, BOOLEAN fForceRemove )
{
	INT16 sNewGridNo;

	// Not if we're dead!
	if ( ( this->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		return;
	}

	// Set new map index
	sNewGridNo = GETWORLDINDEXFROMWORLDCOORDS(dNewYPos, dNewXPos );

	if ( fUpdateDest )
	{
		this->pathing.sDestination = sNewGridNo;
	}

	if ( fUpdateFinalDest )
	{
		this->pathing.sFinalDestination = sNewGridNo;
	}

	// Copy old values
	this->dOldXPos = this->dXPos;
	this->dOldYPos = this->dYPos;

	// Set New pos
	this->dXPos = dNewXPos;
	this->dYPos = dNewYPos;

	this->sX = (INT16)dNewXPos;
	this->sY = (INT16)dNewYPos;

	HandleCrowShadowNewPosition( this );

	this->SetSoldierGridNo( sNewGridNo, fForceRemove );

	if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
	{
		if ( gGameSettings.fOptions[ TOPTION_MERC_ALWAYS_LIGHT_UP ] )
		{
			this->SetCheckSoldierLightFlag( );
		}
	}

	// ATE: Mirror calls if we are a vehicle ( for all our passengers )
	UpdateAllVehiclePassengersGridNo( this );

}

void SOLDIERTYPE::EVENT_SetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos ,TRUE, TRUE, FALSE );
}

void SOLDIERTYPE::EVENT_SetSoldierPositionForceDelete( FLOAT dNewXPos, FLOAT dNewYPos )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos ,TRUE, TRUE, TRUE );
}

void SOLDIERTYPE::EVENT_SetSoldierPositionAndMaybeFinalDest( FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateFinalDest )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos ,TRUE, fUpdateFinalDest, FALSE );
}

void EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateDest,  BOOLEAN fUpdateFinalDest )
{
	pSoldier->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos ,fUpdateDest, fUpdateFinalDest, FALSE );
}


void SOLDIERTYPE::InternalSetSoldierHeight( FLOAT dNewHeight, BOOLEAN fUpdateLevel )
{

	INT8	bOldLevel = this->pathing.bLevel;

	this->aiData.dHeightAdjustment = dNewHeight;
	this->sHeightAdjustment = (INT16)this->aiData.dHeightAdjustment;

	if ( !fUpdateLevel )
	{
		return;
	}

	// 0verhaul:  Changed this to half the wall height.  During a climb up, a soldier's height increases to about 8, then falls
	// to near 0 before being set to 50 at the end.  The animation offsets should probably be changed to make this unnecessary
	// but this is good enough to keep him from bouncing between level 1 and level 0 (and also triggering weird sight bugs).
	if ( this->sHeightAdjustment > 25 )
	{
		this->pathing.bLevel = SECOND_LEVEL;

		ApplyTranslucencyToWalls((INT16)(this->dXPos/CELL_X_SIZE), (INT16)(this->dYPos/CELL_Y_SIZE));
		//LightHideTrees((INT16)(this->dXPos/CELL_X_SIZE), (INT16)(this->dYPos/CELL_Y_SIZE));
		//ConcealAllWalls();

		//this->pLevelNode->ubShadeLevel=gpWorldLevelData[this->sGridNo].pRoofHead->ubShadeLevel;
		//this->pLevelNode->ubSumLights=gpWorldLevelData[this->sGridNo].pRoofHead->ubSumLights;
		//this->pLevelNode->ubMaxLights=gpWorldLevelData[this->sGridNo].pRoofHead->ubMaxLights;
		//this->pLevelNode->ubNaturalShadeLevel=gpWorldLevelData[this->sGridNo].pRoofHead->ubNaturalShadeLevel;
	}
	else
	{
		this->pathing.bLevel = FIRST_LEVEL;

		//this->pLevelNode->ubShadeLevel=gpWorldLevelData[this->sGridNo].pLandHead->ubShadeLevel;
		//this->pLevelNode->ubSumLights=gpWorldLevelData[this->sGridNo].pLandHead->ubSumLights;
		//this->pLevelNode->ubMaxLights=gpWorldLevelData[this->sGridNo].pLandHead->ubMaxLights;
		//this->pLevelNode->ubNaturalShadeLevel=gpWorldLevelData[this->sGridNo].pLandHead->ubNaturalShadeLevel;


	}

	if ( bOldLevel == 0 && this->pathing.bLevel == 0 )
	{

	}
	else
	{
		// Show room at new level
		//HideRoom( this->sGridNo, this );
	}
}



void SOLDIERTYPE::SetSoldierHeight( FLOAT dNewHeight )
{
	this->InternalSetSoldierHeight( dNewHeight, TRUE );
}


void SOLDIERTYPE::SetSoldierGridNo( INT16 sNewGridNo, BOOLEAN fForceRemove )
{
	BOOLEAN	fInWaterValue;
	INT8		bDir;
	INT32		cnt;
	SOLDIERTYPE * pEnemy;

	//INT16	sX, sY, sWorldX, sZLevel;

	// Not if we're dead!
	if ( ( this->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		return;
	}

	if ( sNewGridNo != this->sGridNo || this->pLevelNode == NULL )
	{
		// Check if we are moving AND this is our next dest gridno....
		if ( gAnimControl[ this->usAnimState ].uiFlags & ( ANIM_MOVING  | ANIM_SPECIALMOVE ) )
		{
			if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
			{
				if ( sNewGridNo != this->pathing.sDestination )
				{
					// THIS MUST be our new one......MAKE IT SO
					sNewGridNo = this->pathing.sDestination;
				}

				// Now check this baby....
				if ( sNewGridNo == this->sGridNo )
				{
					return;
				}
			}
		}

		this->sOldGridNo = this->sGridNo;

		if ( this->ubBodyType == QUEENMONSTER )
		{
			SetPositionSndGridNo( this->iPositionSndID, sNewGridNo );
		}

		if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
		{
			this->InternalRemoveSoldierFromGridNo( fForceRemove );
		}

		// CHECK IF OUR NEW GIRDNO IS VALID,IF NOT DONOT SET!
		if ( !GridNoOnVisibleWorldTile( sNewGridNo ) )
		{
			this->sGridNo = sNewGridNo;
			return;
		}

		// Alrighty, update UI for this guy, if he's the selected guy...
		if ( gusSelectedSoldier == this->ubID )
		{
			if ( guiCurrentEvent == C_WAIT_FOR_CONFIRM )
			{
				// Update path!
				gfPlotNewMovement = TRUE;
			}
		}


		// Reset some flags for optimizations..
		this->sWalkToAttackGridNo = NOWHERE;

		// ATE: Make sure!
		// RemoveMerc( this->sGridNo, this, FALSE );

		this->sGridNo = sNewGridNo;

		// OK, check for special code to close door...
		if ( this->bEndDoorOpenCode == 2 )
		{
			this->bEndDoorOpenCode = 0;

			HandleDoorChangeFromGridNo( this, this->sEndDoorOpenCodeData, FALSE );
		}

		// OK, Update buddy's strategic insertion code....
		this->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		this->usStrategicInsertionData = sNewGridNo;


		// Remove this gridno as a reserved place!
		if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
		{
			UnMarkMovementReserved( this );
		}

		if ( this->sInitialGridNo == 0 )
		{
			this->sInitialGridNo = sNewGridNo;
			this->aiData.sPatrolGrid[0] = sNewGridNo;
		}

		// Add records of this guy being adjacent
		for (bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++)
		{
			gpWorldLevelData[ this->sGridNo + DirIncrementer[ bDir ] ].ubAdjacentSoldierCnt++;
		}

		if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
		{
			DropSmell( this );
		}

		// HANDLE ANY SPECIAL RENDERING SITUATIONS
		this->sZLevelOverride = -1;
		// If we are over a fence ( hopping ), make us higher!

		if ( IsJumpableFencePresentAtGridno( sNewGridNo ) )
		{
			//sX = MapX( sNewGridNo );
			//sY = MapY( sNewGridNo );
			//GetWorldXYAbsoluteScreenXY( sX, sY, &sWorldX, &sZLevel);
			//this->sZLevelOverride = (sZLevel*Z_SUBLAYERS)+ROOF_Z_LEVEL;
			this->sZLevelOverride = TOPMOST_Z_LEVEL;
		}


		// Add/ remove tree if we are near it
		// CheckForFullStructures( this );

		// Add merc at new pos
		if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
		{
			AddMercToHead( this->sGridNo, this, TRUE );

			// If we are in the middle of climbing the roof!
			if ( this->usAnimState == CLIMBUPROOF )
			{
				if(this->iLight!=(-1))
					LightSpriteRoofStatus(this->iLight, TRUE );
			}
			else if ( this->usAnimState == CLIMBDOWNROOF )
			{
				if(this->iLight!=(-1))
					LightSpriteRoofStatus(this->iLight, FALSE );
			}

			//JA2Gold:
			//if the player wants the merc to cast the fake light AND it is night
			if( this->bTeam != OUR_TEAM || gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] && NightTime() )
			{
				if ( this->pathing.bLevel > 0 && gpWorldLevelData[this->sGridNo].pRoofHead != NULL )
				{
					gpWorldLevelData[this->sGridNo].pMercHead->ubShadeLevel=gpWorldLevelData[this->sGridNo].pRoofHead->ubShadeLevel;
					gpWorldLevelData[this->sGridNo].pMercHead->ubSumLights=gpWorldLevelData[this->sGridNo].pRoofHead->ubSumLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubMaxLights=gpWorldLevelData[this->sGridNo].pRoofHead->ubMaxLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubNaturalShadeLevel=gpWorldLevelData[this->sGridNo].pRoofHead->ubNaturalShadeLevel;
				}
				else
				{
					gpWorldLevelData[this->sGridNo].pMercHead->ubShadeLevel=gpWorldLevelData[this->sGridNo].pLandHead->ubShadeLevel;
					gpWorldLevelData[this->sGridNo].pMercHead->ubSumLights=gpWorldLevelData[this->sGridNo].pLandHead->ubSumLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubMaxLights=gpWorldLevelData[this->sGridNo].pLandHead->ubMaxLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubNaturalShadeLevel=gpWorldLevelData[this->sGridNo].pLandHead->ubNaturalShadeLevel;
				}
			}

			///HandlePlacingRoofMarker( this, this->sGridNo, TRUE, FALSE );

			this->HandleAnimationProfile( this->usAnimState, FALSE );

			HandleCrowShadowNewGridNo( this );
		}

		this->bOldOverTerrainType = this->bOverTerrainType;
		this->bOverTerrainType = GetTerrainType( this->sGridNo );

		// OK, check that our animation is up to date!
		// Check our water value

		if ( !( this->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
		{
			fInWaterValue = this->MercInWater( );

			// ATE: If ever in water MAKE SURE WE WALK AFTERWOODS!
			if ( fInWaterValue )
			{
				this->usUIMovementMode = WALKING;
			}

			if ( fInWaterValue != this->flags.fPrevInWater )
			{
				//Update Animation data
				SetSoldierAnimationSurface( this, this->usAnimState );

				// Update flag
				this->flags.fPrevInWater = fInWaterValue;

				// Update sound...
				if ( fInWaterValue )
				{
					PlaySoldierJA2Sample( this->ubID, ENTER_WATER_1, RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ), TRUE );
				}
				else
				{
					// ATE: Check if we are going from water to land - if so, resume
					// with regular movement mode...
					this->EVENT_InitNewSoldierAnim( this->usUIMovementMode, 0 , FALSE );
				}

			}


			// OK, If we were not in deep water but we are now, handle deep animations!
			if ( TERRAIN_IS_DEEP_WATER( this->bOverTerrainType) && !TERRAIN_IS_DEEP_WATER( this->bOldOverTerrainType) )
			{
				// Based on our current animation, change!
				switch( this->usAnimState )
				{
				case WALKING:
				case RUNNING:

					// IN deep water, swim!

					// Make transition from low to deep
					this->EVENT_InitNewSoldierAnim( LOW_TO_DEEP_WATER, 0 , FALSE );
					this->usPendingAnimation = DEEP_WATER_SWIM;
					this->usDontUpdateNewGridNoOnMoveAnimChange = 1;
					PlayJA2Sample( ENTER_DEEP_WATER_1, RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

				}
			}

			// Damage water if in deep water....
			if ( this->MercInHighWater( ) )
			{
				WaterDamage( this );
			}

			// OK, If we were in deep water but we are NOT now, handle mid animations!
			if ( !TERRAIN_IS_DEEP_WATER( this->bOverTerrainType) && TERRAIN_IS_DEEP_WATER( this->bOldOverTerrainType) )
			{
				// Make transition from low to deep
				this->EVENT_InitNewSoldierAnim( DEEP_TO_LOW_WATER, 0 , FALSE );
				this->usDontUpdateNewGridNoOnMoveAnimChange = 1;
				this->usPendingAnimation = this->usUIMovementMode;
			}
		}

		// are we now standing in tear gas without a decently working gas mask?
		if ( GetSmokeEffectOnTile( sNewGridNo, this->pathing.bLevel ) > 1 ) //lal: removed normal smoke
		{
			BOOLEAN fSetGassed = TRUE;

			// If we have a functioning gas mask...
			if ( FindGasMask ( this ) != NO_SLOT && this->inv[ HEAD1POS ][0]->data.objectStatus >= GASMASK_MIN_STATUS )
			{
				fSetGassed = FALSE;
			}
			//   if ( this->inv[ HEAD1POS ].usItem == GASMASK && this->inv[ HEAD1POS ][0]->data.objectStatus >= GASMASK_MIN_STATUS )
			//{
			//	fSetGassed = FALSE;
			//}
			//   if ( this->inv[ HEAD2POS ].usItem == GASMASK && this->inv[ HEAD2POS ][0]->data.objectStatus >= GASMASK_MIN_STATUS )
			//{
			//	fSetGassed = FALSE;
			//}

			if ( fSetGassed )
			{
				this->flags.uiStatusFlags |= SOLDIER_GASSED;
			}
		}

		if ( this->bTeam == gbPlayerNum && this->bStealthMode )
		{
			// Merc got to a new tile by "sneaking". Did we theoretically sneak
			// past an enemy?

			if ( this->aiData.bOppCnt > 0 )		// opponents in sight
			{
				// check each possible enemy
				for ( cnt = 0; cnt < MAX_NUM_SOLDIERS; cnt++ )
				{
					pEnemy = MercPtrs[ cnt ];
					// if this guy is here and alive enough to be looking for us
					if ( pEnemy->bActive && pEnemy->bInSector && ( pEnemy->stats.bLife >= OKLIFE ) )
					{
						// no points for sneaking by the neutrals & friendlies!!!
						if ( !pEnemy->aiData.bNeutral && ( this->bSide != pEnemy->bSide ) && (pEnemy->ubBodyType != COW && pEnemy->ubBodyType != CROW) )
						{
							// if we SEE this particular oppponent, and he DOESN'T see us... and he COULD see us...
							if ( (this->aiData.bOppList[ cnt ] == SEEN_CURRENTLY) &&
								pEnemy->aiData.bOppList[ this->ubID ] != SEEN_CURRENTLY &&
								PythSpacesAway( this->sGridNo, pEnemy->sGridNo ) < pEnemy->GetMaxDistanceVisible(this->sGridNo, this->pathing.bLevel ) )
							{
								// AGILITY (5):  Soldier snuck 1 square past unaware enemy
								StatChange( this, AGILAMT, 5, FALSE );
								// Keep looping, we'll give'em 1 point for EACH such enemy!
							}
						}
					}
				}
			}
		}

		// Adjust speed based on terrain, etc
		SetSoldierAniSpeed( this );

	}
	else
	{
		//int breakpoint = 0;
	}
}


void SOLDIERTYPE::EVENT_FireSoldierWeapon( INT16 sTargetGridNo )
{
	INT16 sTargetXPos, sTargetYPos;
	BOOLEAN		fDoFireRightAway = FALSE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("EVENT_FireSoldierWeapon"));

	// CANNOT BE SAME GRIDNO!
	if ( this->sGridNo == sTargetGridNo )
	{
		return;
	}

	//switch ( this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunAmmoType )
	//{
	//	case AMMO_SLEEP_DART:
	//		this->flags.fMuzzleFlash = FALSE;
	//		break;
	//	default:
	//		this->flags.fMuzzleFlash = TRUE;
	//		break;
	//}

#if 0
	// 0verhaul:  This does not go here!  In spite of this function's name, it is not the actual "fire" function.
	// In fact this sets the muzzle flash even while the soldier may be turning to shoot, which can cause
	// problems for real-time shooting.

	// The correct place for this is UseGun, which already has code to set or reset the flash.

	if ( IsFlashSuppressor (&this->inv[ this->ubAttackingHand ], this ) )
		this->flags.fMuzzleFlash = FALSE;
	else
		this->flags.fMuzzleFlash = TRUE;
#endif

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("EVENT_FireSoldierWeapon: Muzzle flash = %d",this->flags.fMuzzleFlash));

	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
	// Nah, just let the animations speak for themselves
	//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
	DebugAttackBusy( String("!!!!!!! Starting fire weapon attack, attack count now %d\n", gTacticalStatus.ubAttackBusyCount) );
	//}

	// Set soldier's target gridno
	// This assignment was redundent because it's already set in
	// the actual event call
	this->sTargetGridNo = sTargetGridNo;
	//this->sLastTarget = sTargetGridNo;
	this->ubTargetID = WhoIsThere2( sTargetGridNo, this->bTargetLevel );

#if 0
	//	if (Item[this->inv[HANDPOS].usItem].usItemClass & IC_GUN)
	{
		if (this->bDoBurst)
		{
			// This is NOT the bullets to fire.  That is done as a check of bDoBurst against the weapon burst count or
			// bDoAutofire, or single-fire.  So let the bullet count be managed by the firing code.
			// Set the TOTAL number of bullets to be fired
			// Can't shoot more bullets than we have in our magazine!
			if(this->bDoAutofire)
				this->bBulletsLeft = __min( this->bDoAutofire, this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunShotsLeft );
			else
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"EVENT_FireSoldierWeapon: do burst");
				if ( this->bWeaponMode == WM_ATTACHED_GL_BURST )
					this->bBulletsLeft = __min( Weapon[GetAttachedGrenadeLauncher(&this->inv[this->ubAttackingHand])].ubShotsPerBurst, this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunShotsLeft );
				else
					this->bBulletsLeft = __min( GetShotsPerBurst(&this->inv[ this->ubAttackingHand ]), this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunShotsLeft );
			}
		}
		else if ( IsValidSecondHandShot( this ) )
		{
			// two-pistol attack - two bullets!
			this->bBulletsLeft = 2;
		}
		else
		{
			this->bBulletsLeft = 1;
		}

		if ( AmmoTypes[this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunAmmoType].numberOfBullets > 1 )
		{
			this->bBulletsLeft *= AmmoTypes[this->inv[ this->ubAttackingHand ][0]->data.gun.ubGunAmmoType].numberOfBullets;
		}
	}
#endif
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack, bullets left %d", this->bBulletsLeft) );

	// Convert our grid-not into an XY
	ConvertGridNoToXY( sTargetGridNo, &sTargetXPos, &sTargetYPos );


	// Change to fire animation
	// Ready weapon
	this->SoldierReadyWeapon( sTargetXPos, sTargetYPos, FALSE );

	// IF WE ARE AN NPC, SLIDE VIEW TO SHOW WHO IS SHOOTING
	{
		//if ( this->flags.fDoSpread )
		//{
		// If we are spreading burst, goto right away!
		//this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[ this->usAnimState ].ubEndHeight ), 0, FALSE );

		//}

		// else
		{
			if (this->flags.uiStatusFlags & SOLDIER_MONSTER )
			{
				// Force our direction!
				this->EVENT_SetSoldierDirection( this->pathing.bDesiredDirection );
				this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[ this->usAnimState ].ubEndHeight ), 0, FALSE );
			}
			else
			{
				// IF WE ARE IN REAl-TIME, FIRE IMMEDIATELY!
				if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
				{
					//fDoFireRightAway = TRUE;
				}

				// Check if our weapon has no intermediate anim...
				if (Item[this->inv[ HANDPOS ].usItem].rocketlauncher || Item[this->inv[ HANDPOS ].usItem].grenadelauncher || Item[this->inv[ HANDPOS ].usItem].mortar )
					///*			switch( this->inv[ HANDPOS ].usItem )
					//			{
					//case RPG7:
					//case ROCKET_LAUNCHER:
					//case MORTAR:
					//case GLAUNCHER:*/

					fDoFireRightAway = TRUE;
				//		break;
				//}

				if ( fDoFireRightAway )
				{
					// Set to true so we don't get toasted twice for APs..
					this->flags.fDontUnsetLastTargetFromTurn = TRUE;

					// Make sure we don't try and do fancy prone turning.....
					this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;

					// Force our direction!
					this->EVENT_SetSoldierDirection( this->pathing.bDesiredDirection );

					this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[ this->usAnimState ].ubEndHeight ), 0, FALSE );
				}
				else
				{
					// Set flag indicating we are about to shoot once destination direction is hit
					this->flags.fTurningToShoot = TRUE;

					if ( this->bTeam != gbPlayerNum  && this->bVisible != -1)
					{
						LocateSoldier( this->ubID, DONTSETLOCATOR );
					}
				}
			}
		}
	}
}

//gAnimControl[ pSoldier->usAnimState ].ubEndHeight
//					pSoldier->ChangeSoldierState( SHOOT_RIFLE_STAND, 0 , FALSE );

UINT16 SelectFireAnimation( SOLDIERTYPE *pSoldier, UINT8 ubHeight )
{
	INT16 sDist;
	UINT16 usItem;
	FLOAT		dTargetX;
	FLOAT		dTargetY;
	FLOAT		dTargetZ;
	BOOLEAN	fDoLowShot = FALSE;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("SelectFireAnimation"));


	//Do different things if we are a monster
	if (pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		switch( pSoldier->ubBodyType )
		{
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:
		case YAF_MONSTER:
		case YAM_MONSTER:

			return( MONSTER_SPIT_ATTACK );
			break;

		case LARVAE_MONSTER:

			break;

		case INFANT_MONSTER:

			return( INFANT_ATTACK );
			break;

		case QUEENMONSTER:

			return( QUEEN_SPIT );
			break;

		}
		return( TRUE );
	}

	if ( pSoldier->ubBodyType == ROBOTNOWEAPON )
	{
		if ( pSoldier->bDoBurst > 0 )
		{
			return( ROBOT_BURST_SHOOT );
		}
		else
		{
			return( ROBOT_SHOOT );
		}
	}

	// Check for rocket laucncher....
	if ( Item[pSoldier->inv[ HANDPOS ].usItem].rocketlauncher )
	{
		return( SHOOT_ROCKET );
	}

	// Check for mortar....
	if ( Item[pSoldier->inv[ HANDPOS ].usItem].mortar )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"SelectFireAnimation: shoot_mortar");
		return( SHOOT_MORTAR );
	}

	// Check for tank cannon
	if ( Item[pSoldier->inv[ HANDPOS ].usItem].cannon )
	{
		return( TANK_SHOOT );
	}

	if ( pSoldier->ubBodyType == TANK_NW || pSoldier->ubBodyType == TANK_NE )
	{
		return( TANK_BURST );
	}

	// Determine which animation to do...depending on stance and gun in hand...
	switch ( ubHeight )
	{
	case ANIM_STAND:

		usItem = pSoldier->inv[ HANDPOS ].usItem;

		// CHECK 2ND HAND!
		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			// Increment the number of people busy doing stuff because of an attack
			//gTacticalStatus.ubAttackBusyCount++;
			//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack with 2 guns, attack count now %d", gTacticalStatus.ubAttackBusyCount) );

			return( SHOOT_DUAL_STAND );
		}
		else
		{
			// OK, while standing check distance away from target, and shoot low if we should!
			sDist = PythSpacesAway( pSoldier->sGridNo, pSoldier->sTargetGridNo );

			//ATE: OK, SEE WERE WE ARE TARGETING....
			GetTargetWorldPositions( pSoldier, pSoldier->sTargetGridNo, &dTargetX, &dTargetY, &dTargetZ );

			//CalculateSoldierZPos( pSoldier, FIRING_POS, &dFirerZ );

			if ( sDist <= 2 && dTargetZ <= 100 )
			{
				fDoLowShot = TRUE;
			}

			// ATE: Made distence away long for psitols such that they never use this....
			//if ( !(Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
			//{
			//	fDoLowShot = FALSE;
			//}

			// Don't do any low shots if in water
			if ( pSoldier->MercInWater( ) )
			{
				fDoLowShot = FALSE;
			}


			if ( pSoldier->bDoBurst > 0 )
			{
				if ( fDoLowShot )
				{
					return( FIRE_BURST_LOW_STAND );
				}
				else
				{
					return( STANDING_BURST );
				}
			}
			else
			{
				if ( fDoLowShot )
				{
					return( FIRE_LOW_STAND );
				}
				else
				{
					return( SHOOT_RIFLE_STAND );
				}
			}
		}
		break;

	case ANIM_PRONE:

		if ( pSoldier->bDoBurst > 0 )
		{
			//				pSoldier->flags.fBurstCompleted = FALSE;
			return( PRONE_BURST );
		}
		else
		{
			if ( pSoldier->IsValidSecondHandShot( ) )
			{
				return( SHOOT_DUAL_PRONE );
			}
			else
			{
				return( SHOOT_RIFLE_PRONE );
			}
		}
		break;

	case ANIM_CROUCH:

		if ( pSoldier->IsValidSecondHandShot( ) )
		{
			// Increment the number of people busy doing stuff because of an attack
			//gTacticalStatus.ubAttackBusyCount++;
			//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting attack with 2 guns, attack count now %d", gTacticalStatus.ubAttackBusyCount) );

			return( SHOOT_DUAL_CROUCH );
		}
		else
		{
			if ( pSoldier->bDoBurst > 0 )
			{
				//				pSoldier->flags.fBurstCompleted = FALSE;
				return( CROUCHED_BURST );
			}
			else
			{
				return( SHOOT_RIFLE_CROUCH );
			}
		}
		break;

	default:
		AssertMsg( FALSE, String( "SelectFireAnimation: ERROR - Invalid height %d", ubHeight ) );
		break;
	}


	// If here, an internal error has occured!
	Assert( FALSE );
	return ( 0 );
}


UINT16 SOLDIERTYPE::GetMoveStateBasedOnStance( UINT8 ubStanceHeight )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( ubStanceHeight )
	{
	case ANIM_STAND:
		if ( this->flags.fUIMovementFast && !( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			return( RUNNING );
		}
		else
		{
			return( WALKING );
		}
		break;

	case ANIM_PRONE:
		if ( this->flags.fUIMovementFast )
		{
			return( CRAWLING );
		}
		else
		{
			return( CRAWLING );
		}
		break;

	case ANIM_CROUCH:
		if ( this->flags.fUIMovementFast )
		{
			return( SWATTING );
		}
		else
		{
			return( SWATTING );
		}
		break;


	default:
		AssertMsg( FALSE, String( "GetMoveStateBasedOnStance: ERROR - Invalid height %d", ubStanceHeight ) );
		break;
	}


	// If here, an internal error has occured!
	Assert( FALSE );
	return ( 0 );
}


void SelectFallAnimation( SOLDIERTYPE *pSoldier )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:
		pSoldier->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0 , FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0 , FALSE );
		break;
	}

}

BOOLEAN SOLDIERTYPE::SoldierReadyWeapon( void )
{
	return( this->InternalSoldierReadyWeapon( (INT8)this->ubDirection, FALSE ) );
}

BOOLEAN SOLDIERTYPE::SoldierReadyWeapon( INT16 sTargetXPos, INT16 sTargetYPos, BOOLEAN fEndReady )
{
	INT16								sFacingDir;

	sFacingDir = GetDirectionFromXY( sTargetXPos , sTargetYPos, this );

	return( this->InternalSoldierReadyWeapon( (INT8)sFacingDir, fEndReady ) );
}


BOOLEAN SOLDIERTYPE::InternalSoldierReadyWeapon( UINT8 sFacingDir, BOOLEAN fEndReady )
{
	UINT16 usAnimState;
	BOOLEAN	fReturnVal = FALSE;

	// Handle monsters differently
	if (this->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		if ( !fEndReady )
		{
			this->EVENT_SetSoldierDesiredDirection( sFacingDir );
		}
		return( FALSE );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("InternalSoldierReadyWeapon: PickingAnimation"));
	usAnimState = PickSoldierReadyAnimation( this, fEndReady );

	if ( usAnimState != INVALID_ANIMATION )
	{
		if(is_networked)
		{
			ChangeSoldierState( usAnimState, 0 , FALSE );//this passes it to an area where it gets sent over the network.
		}
		else
		{
		this->EVENT_InitNewSoldierAnim( usAnimState, 0 , FALSE );
		}
		fReturnVal = TRUE;
		
	}

	if ( !fEndReady )
	{
		// Ready direction for new facing direction
		if ( usAnimState == INVALID_ANIMATION )
		{
			usAnimState = this->usAnimState;
		}

		EVENT_InternalSetSoldierDesiredDirection( this, sFacingDir, FALSE, usAnimState );

		// Check if facing dir is different from ours and change direction if so!
		//if ( sFacingDir != this->ubDirection )
		//{
		//	DeductPoints( this, AP_CHANGE_FACING, 0 );
		//}//

	}

	return( fReturnVal );
}

UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("PickSoldierReadyAnimation"));

	// Invalid animation if nothing in our hands
	if ( pSoldier->inv[ HANDPOS ].exists() == false )
	{
		return( INVALID_ANIMATION );
	}

	if ( pSoldier->MercInDeepWater( ) )
	{
		return( INVALID_ANIMATION );
	}

	if ( pSoldier->ubBodyType == ROBOTNOWEAPON )
	{
		return( INVALID_ANIMATION );
	}

	// Check if we have a gun.....
	if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass != IC_GUN && !Item[pSoldier->inv[ HANDPOS ].usItem].grenadelauncher )
	{
		return( INVALID_ANIMATION );
	}

	if ( Item[pSoldier->inv[ HANDPOS ].usItem].rocketlauncher )
	{
		return( INVALID_ANIMATION );
	}

	if ( pSoldier->ubBodyType == TANK_NW || pSoldier->ubBodyType == TANK_NE )
	{
		return( INVALID_ANIMATION );
	}

	if ( fEndReady )
	{
		// IF our gun is already drawn, do not change animation, just direction
		if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_FIREREADY | ANIM_FIRE ))
		{

			switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
			{
			case ANIM_STAND:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return( END_DUAL_STAND );
				}
				else
				{
					return( END_RIFLE_STAND );
				}
				break;

			case ANIM_PRONE:

				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return( END_DUAL_PRONE );
				}
				else
				{
					return( END_RIFLE_PRONE );
				}
				break;

			case ANIM_CROUCH:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return( END_DUAL_CROUCH );
				}
				else
				{
					return( END_RIFLE_CROUCH );
				}
				break;

			}

		}
	}
	else
	{

		// IF our gun is already drawn, do not change animation, just direction
		if ( !(gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_FIREREADY | ANIM_FIRE ) ) )
		{

			{
				switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
				{
				case ANIM_STAND:

					// CHECK 2ND HAND!
					if ( pSoldier->IsValidSecondHandShot( ) )
					{
						return( READY_DUAL_STAND );
					}
					else
					{
						return( READY_RIFLE_STAND );
					}
					break;

				case ANIM_PRONE:
					// Go into crouch, turn, then go into prone again
					//pSoldier->ChangeSoldierStance( ANIM_CROUCH );
					//pSoldier->ubDesiredHeight = ANIM_PRONE;
					//pSoldier->ChangeSoldierState( PRONE_UP );
					if ( pSoldier->IsValidSecondHandShot( ) )
					{
						return( READY_DUAL_PRONE );
					}
					else
					{
						return( READY_RIFLE_PRONE );
					}
					break;

				case ANIM_CROUCH:

					// CHECK 2ND HAND!
					if ( pSoldier->IsValidSecondHandShot( ) )
					{
						return( READY_DUAL_CROUCH );
					}
					else
					{
						return( READY_RIFLE_CROUCH );
					}
					break;

				}
			}

		}
	}

	return( INVALID_ANIMATION );
}

// 0verhaul:  These routines are obsolete.  Just call ReduceAttackBusyCount to reduce the ABC or
// FreeUpAttacker to abort the current action.
// extern SOLDIERTYPE * FreeUpAttackerGivenTarget( UINT8 ubID, UINT8 ubTargetID );
// extern SOLDIERTYPE * ReduceAttackBusyGivenTarget( UINT8 ubID, UINT8 ubTargetID );


// ATE: THIS FUNCTION IS USED FOR ALL SOLDIER TAKE DAMAGE FUNCTIONS!
void SOLDIERTYPE::EVENT_SoldierGotHit( UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation, INT16 sSubsequent, INT16 sLocationGrid )
{
	UINT8		ubCombinedLoss, ubVolume, ubReason;
//	SOLDIERTYPE * pNewSoldier;

	ubReason = 0;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3 , "EVENT_SoldierGotHit");

#if 0
	// 0verhaul: Under the new ABC system this is no longer necessary.
	// ATE: If we have gotten hit, but are still in our attack animation, reduce count!
	switch ( this->usAnimState )
	{
		case SHOOT_ROCKET:
		case SHOOT_MORTAR:
		case THROW_ITEM:
		// <SB> crouch throwing
		case THROW_ITEM_CROUCHED:
		// <SB> crouch throwing
		case LOB_ITEM:

			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - ATTACK ANIMATION %s ENDED BY HIT ANIMATION, Now %d", gAnimControl[ this->usAnimState ].zAnimStr, gTacticalStatus.ubAttackBusyCount ) );
			ReduceAttackBusyCount( this->ubID, FALSE );
			break;
	}
#endif

	// DO STUFF COMMON FOR ALL TYPES
	if (	ubAttackerID != NOBODY)
	{
		MercPtrs[ubAttackerID]->aiData.bLastAttackHit = TRUE;
	}

	// Set attacker's ID
	this->ubAttackerID = ubAttackerID;

#if 0
	// 0verhaul:  Slashing out more unnecessary and reworked code
	if ( !( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		// Increment  being attacked count
		this->bBeingAttackedCount++;
	}

	// if defender is a vehicle, there will be no hit animation played!
	if ( !( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		// Increment the number of people busy doing stuff because of an attack (busy doing hit anim!)
		gTacticalStatus.ubAttackBusyCount++;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Person got hit, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
	}
#endif

	// ATE; Save hit location info...( for later anim determination stuff )
	this->ubHitLocation = ubHitLocation;

	// handle morale for heavy damage attacks
	if ( sDamage > 25 )
	{
		if ( this->ubAttackerID != NOBODY && MercPtrs[ this->ubAttackerID ]->bTeam == gbPlayerNum )
		{
			HandleMoraleEvent( MercPtrs[ this->ubAttackerID ], MORALE_DID_LOTS_OF_DAMAGE, MercPtrs[ this->ubAttackerID ]->sSectorX, MercPtrs[ this->ubAttackerID ]->sSectorY, MercPtrs[ this->ubAttackerID ]->bSectorZ );
		}
		if (this->bTeam == gbPlayerNum)
		{
			HandleMoraleEvent( this, MORALE_TOOK_LOTS_OF_DAMAGE, this->sSectorX, this->sSectorY, this->bSectorZ );
		}
	}

	// SWITCH IN TYPE OF WEAPON
	if ( ubSpecial == FIRE_WEAPON_TOSSED_OBJECT_SPECIAL )
	{
		ubReason = TAKE_DAMAGE_OBJECT;
	}
	else if ( Item[ usWeaponIndex ].usItemClass & IC_TENTACLES )
	{
		ubReason = TAKE_DAMAGE_TENTACLES;
	}
	// marke take out gunfire if ammotype is explosive

	// callahan update start
	// setting new func to intercept testhit
	else if ( Item[ usWeaponIndex ].usItemClass & ( IC_GUN | IC_THROWING_KNIFE ) && ubAttackerID == NOBODY)
	{
		sBreathLoss += BP_GET_HIT;
		ubReason = TAKE_DAMAGE_GUNFIRE;
	}
	// callahan update end

	else if ( Item[ usWeaponIndex ].usItemClass & ( IC_GUN | IC_THROWING_KNIFE ) && AmmoTypes[MercPtrs[ubAttackerID]->inv[MercPtrs[ubAttackerID]->ubAttackingHand ][0]->data.gun.ubGunAmmoType].explosionSize <= 1)
	{
		if ( ubSpecial == FIRE_WEAPON_SLEEP_DART_SPECIAL )
		{
			UINT32	uiChance;

			// put the drug in!
			this->bSleepDrugCounter = 10;

			uiChance = SleepDartSuccumbChance( this );

			if ( PreRandom( 100 ) < uiChance )
			{
				// succumb to the drug!
				sBreathLoss = (INT16)( this->bBreathMax * 100 );
			}

		}
		else if ( ubSpecial == FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL )
		{
			// blinded!!
			if ( (this->bBlindedCounter == 0)  )
			{
				// say quote
				if (this->flags.uiStatusFlags & SOLDIER_PC)
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
				DecayIndividualOpplist( this );
			}
			// will always increase counter by at least 1
			this->bBlindedCounter += (sDamage / 8) + 1;

			// Dirty panel
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
		sBreathLoss += BP_GET_HIT;
		ubReason = TAKE_DAMAGE_GUNFIRE;
	}
	else if ( Item[ usWeaponIndex ].usItemClass & IC_BLADE )
	{
		sBreathLoss = BP_GET_HIT;
		ubReason = TAKE_DAMAGE_BLADE;
	}
	else if ( Item[ usWeaponIndex ].usItemClass & IC_PUNCH )
	{
		// damage from hand-to-hand is 1/4 normal, 3/4 breath.. the sDamage value
		// is actually how much breath we'll take away
		sBreathLoss = sDamage * 100;
		sDamage = sDamage / PUNCH_REAL_DAMAGE_PORTION;
		if ( AreInMeanwhile() && gCurrentMeanwhileDef.ubMeanwhileID == INTERROGATION )
		{
			sBreathLoss = 0;
			sDamage /= 2;
		}
		ubReason = TAKE_DAMAGE_HANDTOHAND;
	}
	// marke added one 'or' for explosive ammo. variation of: AmmoTypes[this->inv[this->ubAttackingHand ][0]->data.gun.ubGunAmmoType].explosionSize > 1
	//  extracting attacker´s ammo type
	else if ( Item[ usWeaponIndex ].usItemClass & IC_EXPLOSV || AmmoTypes[MercPtrs[ubAttackerID]->inv[MercPtrs[ubAttackerID]->ubAttackingHand ][0]->data.gun.ubGunAmmoType].explosionSize > 1)
	{
		INT8 bDeafValue;

		bDeafValue = Explosive[ Item[ usWeaponIndex ].ubClassIndex ].ubVolume / 10;
		if ( bDeafValue == 0 )
			bDeafValue = 1;

		// Lesh: flashbang does damage
		switch ( ubSpecial )
		{
		case FIRE_WEAPON_BLINDED_AND_DEAFENED:
			this->bDeafenedCounter = bDeafValue;
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Soldier is blinded and deafened" );

			// if soldier in building OR underground
			if ( InBuilding(sLocationGrid) || (gbWorldSectorZ) )
			{
				// deal max special damage
				this->bBlindedCounter = (INT8)Explosive[ Item[ usWeaponIndex ].ubClassIndex ].ubDuration;
				// say quote
				if (this->flags.uiStatusFlags & SOLDIER_PC)
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
			}
			else if ( NightTime() ) // if soldier outside at night
			{
				// halve effect
				this->bBlindedCounter = (INT8)Explosive[ Item[ usWeaponIndex ].ubClassIndex ].ubDuration / 2;
				if ( this->bBlindedCounter == 0 )
					this->bBlindedCounter = 1;
				this->bDeafenedCounter /= 2;
				// say quote
				if (this->flags.uiStatusFlags & SOLDIER_PC)
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
			}
			DecayIndividualOpplist( this );
			break;

		case FIRE_WEAPON_BLINDED:
			break;

		case FIRE_WEAPON_DEAFENED:
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Soldier is deafened" );
			this->bDeafenedCounter = bDeafValue;
			break;
		};

		if ( usWeaponIndex == STRUCTURE_EXPLOSION )
		{
			ubReason = TAKE_DAMAGE_STRUCTURE_EXPLOSION;
		}
		else
		{
			ubReason = TAKE_DAMAGE_EXPLOSION;
		}
	}
	else
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Control: Weapon class not handled in SoldierGotHit( ) %d", usWeaponIndex ) );
	}


#if 0
	// 0verhaul:  None of this hairyness is necessary anymore!  Hazaa!
	// CJC: moved to after SoldierTakeDamage so that any quotes from the defender
	// will not be said if they are knocked out or killed
	if ( ubReason != TAKE_DAMAGE_TENTACLES && ubReason != TAKE_DAMAGE_OBJECT )
	{
		// OK, OK: THis is hairy, however, it's ness. because the normal freeup call uses the
		// attckers intended target, and here we want to use thier actual target....

		// ATE: If it's from GUNFIRE damage, keep in mind bullets...
		if ( Item[ usWeaponIndex ].usItemClass & IC_GUN )
		{
			pNewSoldier = FreeUpAttackerGivenTarget( this->ubAttackerID, this->ubID );
		}
		else
		{
			pNewSoldier = ReduceAttackBusyGivenTarget( this->ubAttackerID, this->ubID );
		}

		if (pNewSoldier != NULL)
		{
			//warning, if this code is ever uncommented, rename all this
			//to this in this function, then init this to this
			this = pNewSoldier;
		}
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Tried to free up attacker, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
	}
#endif


	// OK, If we are a vehicle.... damage vehicle...( people inside... )
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		this->SoldierTakeDamage( ANIM_CROUCH, sDamage, sBreathLoss, ubReason, this->ubAttackerID, NOWHERE, FALSE, TRUE );
		return;
	}

	// DEDUCT LIFE
	ubCombinedLoss = this->SoldierTakeDamage( ANIM_CROUCH, sDamage, sBreathLoss, ubReason, this->ubAttackerID, NOWHERE, FALSE, TRUE );

	// ATE: OK, Let's check our ASSIGNMENT state,
	// If anything other than on a squad or guard, make them guard....
	if ( this->bTeam == gbPlayerNum )
	{
		if ( this->bAssignment >= ON_DUTY && this->bAssignment != ASSIGNMENT_POW )
		{
			if( this->flags.fMercAsleep )
			{
				this->flags.fMercAsleep = FALSE;
				this->flags.fForcedToStayAwake = FALSE;

				// refresh map screen
				fCharacterInfoPanelDirty = TRUE;
				fTeamPanelDirty = TRUE;
			}

			AddCharacterToAnySquad( this );
		}
	}


	// SCREAM!!!!
	ubVolume = CalcScreamVolume( this, ubCombinedLoss );

	// IF WE ARE AT A HIT_STOP ANIMATION
	// DO APPROPRIATE HITWHILE DOWN ANIMATION
	if ( !( gAnimControl[ this->usAnimState ].uiFlags & ANIM_HITSTOP ) || this->usAnimState != JFK_HITDEATH_STOP )
	{
		MakeNoise( this->ubID, this->sGridNo, this->pathing.bLevel, this->bOverTerrainType, ubVolume, NOISE_SCREAM);
	}

	// IAN ADDED THIS SAT JUNE 14th : HAVE TO SHOW VICTIM!
	if (gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) && this->bVisible != -1 && this->bTeam == gbPlayerNum )
		LocateSoldier(this->ubID,DONTSETLOCATOR);


	if ( Item[ usWeaponIndex ].usItemClass & IC_BLADE )
	{
		PlayJA2Sample( (UINT32)( KNIFE_IMPACT ), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
	}
	else
	{
		PlayJA2Sample( (UINT32)( BULLET_IMPACT_1 + Random(3) ), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
	}

	// PLAY RANDOM GETTING HIT SOUND
	// ONLY IF WE ARE CONSCIOUS!
	if ( this->stats.bLife >= CONSCIOUSNESS )
	{
		if ( this->ubBodyType == CROW )
		{
			// Exploding crow...
			PlayJA2Sample( CROW_EXPLODE_1, RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
		}
		else
		{
			// ATE: This is to disallow large amounts of smaples being played which is load!
			if ( this->flags.fGettingHit && this->usAniCode != STANDING_BURST_HIT )
			{

			}
			else
			{
				this->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );
			}
		}
	}

	// CHECK FOR DOING HIT WHILE DOWN
	if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_HITSTOP ) )
	{
		switch( this->usAnimState )
		{
		case FLYBACKHIT_STOP:
			this->ChangeSoldierState( FALLBACK_DEATHTWICH, 0, FALSE );
			break;

		case STAND_FALLFORWARD_STOP:
			this->ChangeSoldierState( GENERIC_HIT_DEATHTWITCHNB, 0, FALSE );
			break;

		case JFK_HITDEATH_STOP:
			this->ChangeSoldierState( JFK_HITDEATH_TWITCHB, 0, FALSE );
			break;

		case FALLBACKHIT_STOP:
			this->ChangeSoldierState( FALLBACK_HIT_DEATHTWITCHNB, 0, FALSE );
			break;

		case PRONE_LAYFROMHIT_STOP:
			this->ChangeSoldierState( PRONE_HIT_DEATHTWITCHNB, 0, FALSE );
			break;

		case PRONE_HITDEATH_STOP:
			this->ChangeSoldierState( PRONE_HIT_DEATHTWITCHB, 0 , FALSE );
			break;

		case FALLFORWARD_HITDEATH_STOP:
			this->ChangeSoldierState( GENERIC_HIT_DEATHTWITCHB, 0 , FALSE );
			break;

		case FALLBACK_HITDEATH_STOP:
			this->ChangeSoldierState( FALLBACK_HIT_DEATHTWITCHB, 0 , FALSE );
			break;

		case FALLOFF_DEATH_STOP:
			this->ChangeSoldierState( FALLOFF_TWITCHB, 0 , FALSE );
			break;

		case FALLOFF_STOP:
			this->ChangeSoldierState( FALLOFF_TWITCHNB, 0 , FALSE );
			break;

		case FALLOFF_FORWARD_DEATH_STOP:
			this->ChangeSoldierState( FALLOFF_FORWARD_TWITCHB, 0 , FALSE );
			break;

		case FALLOFF_FORWARD_STOP:
			this->ChangeSoldierState( FALLOFF_FORWARD_TWITCHNB, 0 , FALSE );
			break;

		default:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Control: Death state %d has no death hit", this->usAnimState ) );

		}
		return;
	}

	// Set goback to aim after hit flag!
	// Only if we were aiming!
	if ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_FIREREADY )
	{
		this->flags.fGoBackToAimAfterHit = TRUE;
	}

	// IF COWERING, PLAY SPECIFIC GENERIC HIT STAND...
	if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
	{
		if ( this->stats.bLife == 0 || IS_MERC_BODY_TYPE( this ) )
		{
			this->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0 , FALSE );
		}
		else
		{
			this->EVENT_InitNewSoldierAnim( CIV_COWER_HIT, 0 , FALSE );
		}
		return;
	}

	// Change based on body type
	switch( this->ubBodyType )
	{
	case COW:
		this->EVENT_InitNewSoldierAnim( COW_HIT, 0 , FALSE );
		return;
		break;

	case BLOODCAT:
		this->EVENT_InitNewSoldierAnim( BLOODCAT_HIT, 0 , FALSE );
		return;
		break;

	case ADULTFEMALEMONSTER:
	case AM_MONSTER:
	case YAF_MONSTER:
	case YAM_MONSTER:

		this->EVENT_InitNewSoldierAnim( ADULTMONSTER_HIT, 0 , FALSE );
		return;
		break;

	case LARVAE_MONSTER:
		this->EVENT_InitNewSoldierAnim( LARVAE_HIT, 0 , FALSE );
		return;
		break;

	case QUEENMONSTER:
		this->EVENT_InitNewSoldierAnim( QUEEN_HIT, 0 , FALSE );
		return;
		break;

	case CRIPPLECIV:

		{
			// OK, do some code here to allow the fact that poor buddy can be thrown back if it's a big enough hit...
			this->EVENT_InitNewSoldierAnim( CRIPPLE_HIT, 0 , FALSE );

			//this->stats.bLife = 0;
			//this->EVENT_InitNewSoldierAnim( CRIPPLE_DIE_FLYBACK, 0 , FALSE );


		}
		return;
		break;

	case ROBOTNOWEAPON:
		this->EVENT_InitNewSoldierAnim( ROBOTNW_HIT, 0 , FALSE );
		return;
		break;


	case INFANT_MONSTER:
		this->EVENT_InitNewSoldierAnim( INFANT_HIT, 0 , FALSE );
		return;

	case CROW:

		this->EVENT_InitNewSoldierAnim( CROW_DIE, 0 , FALSE );
		return;

		//case FATCIV:
	case MANCIV:
	case MINICIV:
	case DRESSCIV:
	case HATKIDCIV:
	case KIDCIV:

		// OK, if life is 0 and not set as dead ( this is a death hit... )
		if ( !( this->flags.uiStatusFlags & SOLDIER_DEAD ) && this->stats.bLife == 0 )
		{
			// Randomize death!
			if ( Random( 2 ) )
			{
				this->EVENT_InitNewSoldierAnim( CIV_DIE2, 0 , FALSE );
				return;
			}
		}

		// IF here, go generic hit ALWAYS.....
		this->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0 , FALSE );
		return;
		break;
	}

	// If here, we are a merc, check if we are in water
	if ( this->MercInShallowWater( ) )
	{
		this->EVENT_InitNewSoldierAnim( WATER_HIT, 0 , FALSE );
		return;
	}
	if ( this->MercInDeepWater( ) )
	{
		this->EVENT_InitNewSoldierAnim( DEEP_WATER_HIT, 0 , FALSE );
		return;
	}


	// SWITCH IN TYPE OF WEAPON
	if ( Item[ usWeaponIndex ].usItemClass & ( IC_GUN | IC_THROWING_KNIFE ) )
	{
		SoldierGotHitGunFire( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
	}
	if ( Item[ usWeaponIndex ].usItemClass & IC_BLADE )
	{
		SoldierGotHitBlade( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
	}
	// marke setting ammo explosions included here with 3rd 'or' including ubReason
	if ( Item[ usWeaponIndex ].usItemClass & IC_EXPLOSV || Item[ usWeaponIndex ].usItemClass & IC_TENTACLES || ubReason == TAKE_DAMAGE_EXPLOSION )
	{
		SoldierGotHitExplosion( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
	}
	if ( Item[ usWeaponIndex ].usItemClass & IC_PUNCH )
	{
		SoldierGotHitPunch( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
	}

}

UINT8 CalcScreamVolume( SOLDIERTYPE * pSoldier, UINT8 ubCombinedLoss )
{
	// NB explosions are so loud they should drown out screams
	UINT8 ubVolume;

	if (ubCombinedLoss < 1)
	{
		ubVolume = 1;
	}
	else
	{
		ubVolume = ubCombinedLoss;
	}

	// Victim yells out in pain, making noise.  Yelps are louder from greater
	// wounds, but softer for more experienced soldiers.

	if (ubVolume > (10 - EffectiveExpLevel( pSoldier ) ))
	{
		ubVolume = 10 - EffectiveExpLevel( pSoldier );
	}

	/*
	// the "Speck factor"...  He's a whiner, and extra-sensitive to pain!
	if (ptr->trait == NERVOUS)
	ubVolume += 2;
	*/

	if (ubVolume < 0)
	{
		ubVolume = 0;
	}

	return( ubVolume );
}


void DoGenericHit( SOLDIERTYPE *pSoldier, UINT8 ubSpecial, INT16 bDirection )
{
	// Based on stance, select generic hit animation
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:
		// For now, check if we are affected by a burst
		// For now, if the weapon was a gun, special 1 == burst
		// ATE: Only do this for mercs!
		if ( ubSpecial == FIRE_WEAPON_BURST_SPECIAL && pSoldier->ubBodyType <= REGFEMALE )
		{
			//SetSoldierDesiredDirection( pSoldier, bDirection );
			pSoldier->EVENT_SetSoldierDirection( (INT8)bDirection );
			pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

			pSoldier->EVENT_InitNewSoldierAnim( STANDING_BURST_HIT, 0 , FALSE );
		}
		else
		{
			// Check in hand for rifle
			if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
			{
				pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0 , FALSE );
			}
			else
			{
				pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0 , FALSE );
			}
		}
		break;

	case ANIM_PRONE:

		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0 , FALSE );
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0 , FALSE );
		break;

	}
}


void SoldierGotHitGunFire( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	INT16	sNewGridNo;
	BOOLEAN	fBlownAway = FALSE;
	BOOLEAN	fHeadHit = FALSE;
	BOOLEAN	fFallenOver = FALSE;

	// MAYBE CHANGE TO SPECIAL ANIMATION BASED ON VALUE SET BY DAMAGE CALCULATION CODE
	// ALL THESE ONLY WORK ON STANDING PEOPLE
	if (!(pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER) && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND)
	{
		if (gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND )
		{
			if (ubSpecial == FIRE_WEAPON_HEAD_EXPLODE_SPECIAL)
			{
				if ( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
				{
					if (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && usWeaponIndex == BARRETT ))
					{
						sNewGridNo = NewGridNo( (INT16)pSoldier->sGridNo, (INT8)( DirectionInc( pSoldier->ubDirection ) ) );

						// CHECK OK DESTINATION!
						if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, JFK_HITDEATH ) )
						{
							sNewGridNo = NewGridNo( (INT16)sNewGridNo, (INT8)( DirectionInc( pSoldier->ubDirection ) ) );

							if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, pSoldier->usAnimState ) )
							{
								fHeadHit = TRUE;
							}
						}
					}
				}
			}
			else if (ubSpecial == FIRE_WEAPON_CHEST_EXPLODE_SPECIAL)
			{
				if ( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
				{
					if (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_DISTANCE_FOR_MESSY_DEATH || (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && usWeaponIndex == BARRETT ))
					{

						// possibly play torso explosion anim!
						if (pSoldier->ubDirection == bDirection)
						{
							sNewGridNo = NewGridNo( (INT16)pSoldier->sGridNo, DirectionInc( gOppositeDirection[ pSoldier->ubDirection ] ) );

							if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ bDirection ], FLYBACK_HIT ) )
							{
								sNewGridNo = NewGridNo( (INT16)sNewGridNo, DirectionInc( gOppositeDirection[ bDirection ] ) );

								if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ bDirection ], pSoldier->usAnimState ) )
								{
									fBlownAway = TRUE;
								}
							}
						}
					}
				}
			}
			else if (ubSpecial == FIRE_WEAPON_LEG_FALLDOWN_SPECIAL)
			{
				// possibly play fall over anim!
				// this one is NOT restricted by distance
				if (IsValidStance( pSoldier, ANIM_PRONE ) )
				{
					// Can't be in water, or not standing
					if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND && !pSoldier->MercInWater( ) )
					{
						fFallenOver = TRUE;
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 20 ], pSoldier->name );
					}
				}
			}
		}
	}

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		// 0verhaul:  Handled in the soldier state change code
		// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker,Dead soldier hit" ) );
		// ReleaseSoldiersAttacker( pSoldier );
		return;
	}

	if ( fFallenOver )
	{
		SoldierCollapse( pSoldier );
		return;
	}

	if ( fBlownAway )
	{
		// Only for mercs...
		if ( pSoldier->ubBodyType < 4 )
		{
			pSoldier->ChangeToFlybackAnimation( (UINT8)bDirection );
			return;
		}
	}

	if ( fHeadHit )
	{
		// Only for mercs ( or KIDS! )
		if ( pSoldier->ubBodyType < 4 || pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV )
		{
			pSoldier->EVENT_InitNewSoldierAnim( JFK_HITDEATH, 0 , FALSE );
			return;
		}
	}

	DoGenericHit( pSoldier, ubSpecial, bDirection );

}

void SoldierGotHitExplosion( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	INT16 sNewGridNo;

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}

	//check for services
	pSoldier->ReceivingSoldierCancelServices( );
	pSoldier->GivingSoldierCancelServices( );


	if ( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
	{
		if ( Explosive[ Item[ usWeaponIndex ].ubClassIndex ].ubRadius >= 3 && pSoldier->stats.bLife == 0 && gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_PRONE )
		{
			if ( sRange >= 2 && sRange <= 4 )
			{
				pSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );

				pSoldier->EVENT_InitNewSoldierAnim( CHARIOTS_OF_FIRE, 0 , FALSE );
				return;
			}
			else if ( sRange <= 1 )
			{
				pSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );

				pSoldier->EVENT_InitNewSoldierAnim( BODYEXPLODING, 0 , FALSE );
				return;
			}
		}
	}

	// If we can't fal back or such, so generic hit...
	if ( pSoldier->ubBodyType >= 4 )
	{
		DoGenericHit( pSoldier, 0, bDirection );
		return;
	}

	// Lesh: possible soldier behavior when affected by flashbang
	// Soldier can:
	//   1. stand as if there was no explosion at all
	//   2. crouch. represent that soldier didn't expect such blow and instinctively
	//      made defensive movement to protect his body
	//   3. fall forward. again, he didn't expect that something will explode behind
	//      him and deafens him
	//   4. fall backward. unexpected blast, fear, clumsy moves and soldier flies backward.

	// Based on stance, select generic hit animation
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:
		if ( ubSpecial == FIRE_WEAPON_DEAFENED )
		{
			switch( Random(10) )
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				// 6 of 10 - crouch
				pSoldier->ChangeSoldierStance( ANIM_CROUCH );
				break;
			case 6:
			case 7:
			case 8:
				// 3 of 10 - fall forward
				pSoldier->BeginTyingToFall( );
				pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
				break;
			case 9:
				// 1 of 10 - still standing
				DoGenericHit( pSoldier, 0, bDirection );
				break;
			};
			break;
		}
		else if ( ubSpecial == FIRE_WEAPON_BLINDED_AND_DEAFENED )
		{
			switch( Random(10) )
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				// 5 of 10 - crouch
				pSoldier->ChangeSoldierStance( ANIM_CROUCH );
				break;
			case 5:
			case 6:
			case 7:
			case 8:
				// 4 of 10 - fall backward (if possible) either forward
				// Check behind us!
				sNewGridNo = NewGridNo( (INT16)pSoldier->sGridNo, DirectionInc( gOppositeDirection[ bDirection ] ) );
				if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ bDirection ], FLYBACK_HIT ) )
				{
					pSoldier->EVENT_SetSoldierDirection( (INT8)bDirection );
					pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
					pSoldier->ChangeToFallbackAnimation( (UINT8)bDirection );
				}
				else
				{
					pSoldier->BeginTyingToFall( );
					pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
				}
				break;
			case 9:
				// 1 of 10 - still standing
				DoGenericHit( pSoldier, 0, bDirection );
				break;
			};
			break;
		}
		else if ( ubSpecial == FIRE_WEAPON_BLINDED )
		{
		}

	case ANIM_CROUCH:

		if ( ubSpecial == FIRE_WEAPON_BLINDED ||
			ubSpecial == FIRE_WEAPON_BLINDED_AND_DEAFENED ||
			ubSpecial == FIRE_WEAPON_DEAFENED )
		{
			DoGenericHit( pSoldier, 0, bDirection );
			break;
		}

		pSoldier->EVENT_SetSoldierDirection( (INT8)bDirection );
		pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

		// Check behind us!
		sNewGridNo = NewGridNo( (INT16)pSoldier->sGridNo, DirectionInc( gOppositeDirection[ bDirection ] ) );

		if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ bDirection ], FLYBACK_HIT ) )
		{
			pSoldier->ChangeToFallbackAnimation( (UINT8)bDirection );
		}
		else
		{
			if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND )
			{
				pSoldier->BeginTyingToFall( );
				pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
			}
			else
			{
				SoldierCollapse( pSoldier );
			}
		}
		break;

	case ANIM_PRONE:

		SoldierCollapse( pSoldier );
		break;
	}

}


void SoldierGotHitBlade( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}


	// Based on stance, select generic hit animation
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:

		// Check in hand for rifle
		if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0 , FALSE );
		}
		else
		{
			pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0 , FALSE );
		}
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0 , FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0 , FALSE );
		break;
	}

}


void SoldierGotHitPunch( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}

	// Based on stance, select generic hit animation
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:
		// Check in hand for rifle
		if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0 , FALSE );
		}
		else
		{
			pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0 , FALSE );
		}
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0 , FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0 , FALSE );
		break;

	}

}


BOOLEAN SOLDIERTYPE::EVENT_InternalGetNewSoldierPath( INT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim )
{
	INT32	iDest;
	INT16	sNewGridNo;
	BOOLEAN fContinue;
	UINT32	uiDist;
	UINT16	usAnimState;
	UINT16	usMoveAnimState = usMovementAnim;
	INT16							sMercGridNo;
	UINT16						usPathingData[ MAX_PATH_LIST_SIZE ];
	UINT8							ubPathingMaxDirection;
	BOOLEAN						fAdvancePath = TRUE;
	UINT8							fFlags = 0;

	// Ifd this code, make true if a player
	if ( fFromUI == 3 )
	{
		if ( this->bTeam == gbPlayerNum )
		{
			fFromUI = 1;
		}
		else
		{
			fFromUI = 0;
		}
	}

	// ATE: if a civ, and from UI, and were cowering, remove from cowering
	if ( AM_AN_EPC( this ) && fFromUI )
	{
		if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
		{
			this->SetSoldierCowerState( FALSE );
			usMoveAnimState = WALKING;
		}
	}


	this->bGoodContPath			= FALSE;

	if ( this->flags.fDelayedMovement )
	{
		if ( this->ubDelayedMovementFlags & DELAYED_MOVEMENT_FLAG_PATH_THROUGH_PEOPLE )
		{
			fFlags = PATH_THROUGH_PEOPLE;
		}
		else
		{
			fFlags = PATH_IGNORE_PERSON_AT_DEST;
		}
		this->flags.fDelayedMovement = FALSE;
	}

	if ( gfGetNewPathThroughPeople )
	{
		fFlags = PATH_THROUGH_PEOPLE;
	}

	// ATE: Some stuff here for realtime, going through interface....
	if ( ( !( gTacticalStatus.uiFlags & INCOMBAT ) && ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_MOVING ) && fFromUI == 1 ) || fFromUI == 2 )
	{
		if ( this->bCollapsed )
		{
			return( FALSE );
		}

		sMercGridNo = this->sGridNo;
		this->sGridNo = this->pathing.sDestination;

		// Check if path is good before copying it into guy's path...
		if ( !(uiDist = FindBestPath( this, sDestGridNo, this->pathing.bLevel, this->usUIMovementMode, COPYROUTE, fFlags ) ) )
		{
			// Set to old....
			this->sGridNo = sMercGridNo;

			return( FALSE );
		}

		//uiDist =  FindBestPath( this, sDestGridNo, this->pathing.bLevel, this->usUIMovementMode, COPYROUTE, fFlags );

		this->sGridNo = sMercGridNo;
		this->pathing.sFinalDestination = sDestGridNo;

		if ( uiDist > 0 )
		{
			// Add one to path data size....
			if ( fAdvancePath )
			{
				memcpy( usPathingData, this->pathing.usPathingData, sizeof( usPathingData ) );
				ubPathingMaxDirection = (UINT8)usPathingData[ MAX_PATH_LIST_SIZE -1 ];
				memcpy( &(this->pathing.usPathingData[1]), usPathingData, sizeof( usPathingData ) - sizeof( UINT16 ) );

				// If we have reach the max, go back one sFinalDest....
				if ( this->pathing.usPathDataSize == MAX_PATH_LIST_SIZE )
				{
					//this->pathing.sFinalDestination = NewGridNo( (INT16)this->pathing.sFinalDestination, DirectionInc( gOppositeDirection[ ubPathingMaxDirection ] ) );
				}
				else
				{
					this->pathing.usPathDataSize++;
				}
			}

			usMoveAnimState = this->usUIMovementMode;

			if ( this->MercInDeepWater( ) )
			{
				usMoveAnimState = DEEP_WATER_SWIM;
			}

			// Change animation only.... set value to NOT call any goto new gridno stuff.....
			if ( usMoveAnimState != this->usAnimState )
			{
				//
				this->usDontUpdateNewGridNoOnMoveAnimChange = TRUE;

				this->EVENT_InitNewSoldierAnim( usMoveAnimState, 0, FALSE );
				if(is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usMoveAnimState , 0 , FALSE );

				return( TRUE );
			}
			if(is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, this->usAnimState , 255 , FALSE );

			return( TRUE );
		}

		return( FALSE );
	}

	// we can use the soldier's level here because we don't have pathing across levels right now...
#if 0
	// Uhhmmmm, the name of this function has "NEWPath" in it.
	if (this->pathing.bPathStored)
	{
		fContinue = TRUE;
	}
	else
	{
#endif
		iDest = FindBestPath( this, sDestGridNo, this->pathing.bLevel, usMovementAnim, COPYROUTE, fFlags );
		fContinue = (iDest != 0);
//	}

	// Only if we can get a path here
	if ( fContinue )
	{
		// Debug messages
		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier %d: Get new path", this->ubID ) );

		// Set final destination
		this->pathing.sFinalDestination = sDestGridNo;
		this->flags.fPastXDest = 0;
		this->flags.fPastYDest = 0;


		// CHECK IF FIRST TILE IS FREE
		sNewGridNo = NewGridNo( (INT16)this->sGridNo, DirectionInc( (UINT8)this->pathing.usPathingData[ this->pathing.usPathIndex ] ) );

		// If true, we're OK, if not, WAIT for a guy to pass!
		// If we are in deep water, we can only swim!
		if ( this->MercInDeepWater( ) )
		{
			usMoveAnimState = DEEP_WATER_SWIM;
		}
		// Can't forget shallow water!  AI will sometimes attempt to swat through it, which is not legal either
		else if ( this->MercInWater( ) )
		{
			usMoveAnimState = WALKING;
		}

		// If we were aiming, end aim!
		usAnimState = PickSoldierReadyAnimation( this, TRUE );

		// Add a pending animation first!
		// Only if we were standing!
		if ( usAnimState != INVALID_ANIMATION && gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_STAND )
		{
			this->EVENT_InitNewSoldierAnim( usAnimState, 0, FALSE );
			this->usPendingAnimation = usMoveAnimState;
			if(is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usAnimState , 0 , FALSE );
		}
		else
		{
			// Call local copy for change soldier state!
			this->EVENT_InitNewSoldierAnim( usMoveAnimState, 0, fForceRestartAnim );
			if(is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usMovementAnim , 0 , fForceRestartAnim );

		}

		// Change desired direction
		// ATE: Here we have a situation where in RT, we may have
		// gotten a new path, but we are alreayd moving.. so
		// at leasty change new dest. This will be redundent if the ANI is a totaly new one

		return( TRUE );
	}

	return( FALSE );
}

void SOLDIERTYPE::EVENT_GetNewSoldierPath( INT16 sDestGridNo, UINT16 usMovementAnim )
{
	// ATE: Default restart of animation to TRUE
	this->EVENT_InternalGetNewSoldierPath( sDestGridNo, usMovementAnim, FALSE, TRUE );
}

// Change our state based on stance, to stop!
void SOLDIERTYPE::StopSoldier( void )
{
	this->ReceivingSoldierCancelServices( );
	this->GivingSoldierCancelServices( );

	if ( !( gAnimControl[ this->usAnimState ].uiFlags & ANIM_STATIONARY ) )
	{
		//this->SoldierGotoStationaryStance( );
		this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
	}

	// Set desination
	this->pathing.sFinalDestination = this->sGridNo;

}

void SOLDIERTYPE::SoldierGotoStationaryStance( void )
{
	// ATE: This is to turn off fast movement, that us used to change movement mode
	// for ui display on stance changes....
	if ( this->bTeam == gbPlayerNum )
	{
		//this->flags.fUIMovementFast = FALSE;
	}

	// The queen, if she sees anybody, goes to ready, not normal breath....
	if ( this->ubBodyType == QUEENMONSTER )
	{
		if ( this->aiData.bOppCnt > 0 || this->bTeam == gbPlayerNum )
		{
			this->EVENT_InitNewSoldierAnim( QUEEN_READY, 0 , TRUE );
			return;
		}
	}

	// Check if we are in deep water!
	if ( this->MercInDeepWater( ) )
	{
		// IN deep water, tred!
		this->EVENT_InitNewSoldierAnim( DEEP_WATER_TRED, 0 , FALSE );
	}
	else if ( this->ubServicePartner != NOBODY && this->stats.bLife >= OKLIFE && this->bBreath > 0  )
	{
		if(!is_networked)this->EVENT_InitNewSoldierAnim( GIVING_AID, 0 , FALSE );
		else this->ChangeSoldierState( GIVING_AID, 0, 0 );			
		
	}
	else
	{
		// Change state back to stationary state for given height
		switch( gAnimControl[ this->usAnimState ].ubEndHeight )
		{
		case ANIM_STAND:

			// If we are cowering....goto cower state
			if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
			{
				this->EVENT_InitNewSoldierAnim( START_COWER, 0 , FALSE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( STANDING, 0 , FALSE );
			}
			break;

		case ANIM_CROUCH:

			// If we are cowering....goto cower state
			if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
			{
				this->EVENT_InitNewSoldierAnim( COWERING, 0 , FALSE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( CROUCHING, 0 , FALSE );
			}
			break;

		case ANIM_PRONE:
			this->EVENT_InitNewSoldierAnim( PRONE, 0 , FALSE );
			break;
		}

	}

}


void SOLDIERTYPE::ChangeSoldierStance( UINT8 ubDesiredStance )
{
	UINT16 usNewState;

	// Check if they are the same!
	if ( ubDesiredStance == gAnimControl[ this->usAnimState ].ubEndHeight )
	{
		// Free up from stance change
		FreeUpNPCFromStanceChange( this );
		return;
	}

	if (!IsValidStance( this, ubDesiredStance))
	{
		//ADB this was recently put in as an assert, but that would make the game crash in certain circumstances, including the example below
		//you can comment out the breakpoint if it annoys you, but the return should remain for the indefinate future
		//if it's removed you'll have stuff happen like when out of breath and lying collapsed on the ground
		//and trying to climb onto a roof you'll crouch, stand, reach for the roof, and then collapse again, which looks really weird.
		DebugBreakpoint();
		return;
	}

	// Set UI Busy
	SetUIBusy( this->ubID );

	// ATE: If we are an NPC, cower....
	if ( this->ubBodyType >= FATCIV && this->ubBodyType <= KIDCIV )
	{
		if ( ubDesiredStance == ANIM_STAND )
		{
			this->SetSoldierCowerState( FALSE );
		}
		else
		{
			this->SetSoldierCowerState( TRUE );
		}
	}
	else
	{
		usNewState = this->GetNewSoldierStateFromNewStance( ubDesiredStance );

		// Set desired stance
		this->ubDesiredHeight = ubDesiredStance;

		// Now change to appropriate animation
		this->EVENT_InitNewSoldierAnim( usNewState, 0 , FALSE );
		}
	}

void SOLDIERTYPE::EVENT_InternalSetSoldierDestination( UINT16	usNewDirection, BOOLEAN fFromMove, UINT16 usAnimState )
{
	INT16	sNewGridNo;
	INT16		sXPos, sYPos;

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( (INT16)this->sGridNo, DirectionInc( (UINT8)usNewDirection ) );

	ConvertMapPosToWorldTileCenter( sNewGridNo, &sXPos, &sYPos );

	// Save new dest gridno, x, y
	this->pathing.sDestination = sNewGridNo;
	this->pathing.sDestXPos = sXPos;
	this->pathing.sDestYPos = sYPos;

	this->bMovementDirection = (INT8)usNewDirection;


	// OK, ATE: If we are side_stepping, calculate a NEW desired direction....
	if ( this->bReverse && usAnimState == SIDE_STEP )
	{
		UINT8 ubPerpDirection;

		// Get a new desired direction,
		ubPerpDirection = gPurpendicularDirection[ this->ubDirection ][ usNewDirection ];

		// CHange actual and desired direction....
		this->EVENT_SetSoldierDirection( ubPerpDirection );
		this->pathing.bDesiredDirection = this->ubDirection;
	}
	else
	{
		if ( !( gAnimControl[ usAnimState ].uiFlags & ANIM_SPECIALMOVE ) )
		{
			EVENT_InternalSetSoldierDesiredDirection( this, (UINT8)usNewDirection, fFromMove, usAnimState );
		}
	}
}

void SOLDIERTYPE::EVENT_SetSoldierDestination( UINT8	ubNewDirection )
{
	this->EVENT_InternalSetSoldierDestination( ubNewDirection, FALSE, this->usAnimState );
}


// function to determine which direction a creature can turn in
INT8 MultiTiledTurnDirection( SOLDIERTYPE * pSoldier, INT8 bStartDirection, INT8 bDesiredDirection )
{
	INT8										bTurningIncrement;
	INT8										bCurrentDirection;
	INT8										bLoop;
	UINT16									usStructureID, usAnimSurface;
	STRUCTURE_FILE_REF *		pStructureFileRef;
	BOOLEAN									fOk = FALSE;

	// start by trying to turn in quickest direction
	bTurningIncrement = (INT8) QuickestDirection( bStartDirection, bDesiredDirection );

	usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, pSoldier->usUIMovementMode );

	pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, pSoldier->usUIMovementMode );
	if ( !pStructureFileRef )
	{
		// without structure data, well, assume quickest direction
		return( bTurningIncrement );
	}

	// ATE: Only if we have a levelnode...
	if ( pSoldier->pLevelNode != NULL && pSoldier->pLevelNode->pStructureData != NULL )
	{
		usStructureID = pSoldier->pLevelNode->pStructureData->usStructureID;
	}
	else
	{
		usStructureID = INVALID_STRUCTURE_ID;
	}

	bLoop = 0;
	bCurrentDirection = bStartDirection;

	while( bLoop < 2 )
	{
		while( bCurrentDirection != bDesiredDirection )
		{
			bCurrentDirection += bTurningIncrement;

			// did we wrap directions?
			if ( bCurrentDirection < 0 )
			{
				bCurrentDirection = (MAXDIR - 1);
			}
			else if ( bCurrentDirection >= MAXDIR )
			{
				bCurrentDirection = 0;
			}

			// check to see if we can add creature in that direction
			fOk = OkayToAddStructureToWorld( pSoldier->sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[ gOneCDirection[ bCurrentDirection	] ]), usStructureID );
			if (!fOk)
			{
				break;
			}
		}

		if ( (bCurrentDirection == bDesiredDirection) && fOk )
		{
			// success!!
			return( bTurningIncrement );
		}

		bLoop++;
		//if ( bLoop < 2 )
		{
			// change direction of loop etc
			bCurrentDirection = bStartDirection;
			bTurningIncrement *= -1;
		}
	}
	// nothing found... doesn't matter much what we return
	return( bTurningIncrement );
}



void EVENT_InternalSetSoldierDesiredDirection( SOLDIERTYPE *pSoldier, UINT8	ubNewDirection, BOOLEAN fInitalMove, UINT16 usAnimState )
{
	//if ( usAnimState == WALK_BACKWARDS )
	if ( pSoldier->bReverse && usAnimState != SIDE_STEP )
	{
		// OK, check if we are going to go in the exact opposite than our facing....
		ubNewDirection = gOppositeDirection[ ubNewDirection ];
	}


	pSoldier->pathing.bDesiredDirection = (INT8)ubNewDirection;

	// If we are prone, goto crouched first!
	// ONly if we are stationary, and only if directions are differnet!

	// ATE: If we are fNoAPsToFinnishMove, stop what we were doing and
	// reset flag.....
	if ( pSoldier->flags.fNoAPToFinishMove && ( gAnimControl[ usAnimState ].uiFlags & ANIM_MOVING ) )
	{
		// ATE; Commented this out: NEVER, EVER, start a new anim from this function, as an eternal loop will result....
		//pSoldier->SoldierGotoStationaryStance( );
		// Reset flag!
		pSoldier->AdjustNoAPToFinishMove( FALSE );
	}

	if ( pSoldier->pathing.bDesiredDirection != pSoldier->ubDirection )
	{
		if ( gAnimControl[ usAnimState ].uiFlags & ( ANIM_BREATH | ANIM_OK_CHARGE_AP_FOR_TURN | ANIM_FIREREADY ) && !fInitalMove && !pSoldier->flags.fDontChargeTurningAPs )
		{
			// Deduct points for initial turn!
			switch( gAnimControl[ usAnimState ].ubEndHeight )
			{
				// Now change to appropriate animation
			case ANIM_STAND:
				DeductPoints( pSoldier, AP_LOOK_STANDING, 0 );
				break;

			case ANIM_CROUCH:
				DeductPoints( pSoldier, AP_LOOK_CROUCHED, 0 );
				break;

			case ANIM_PRONE:
				DeductPoints( pSoldier, AP_LOOK_PRONE, 0 );
				break;
			}

		}

		pSoldier->flags.fDontChargeTurningAPs = FALSE;

		if ( fInitalMove )
		{
			if ( gAnimControl[ usAnimState ].ubHeight == ANIM_PRONE  )
			{
				if ( pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE )
				{
					pSoldier->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_START_UP_FROM_MOVE;
				}
			}
		}

		if ( gAnimControl[ usAnimState ].uiFlags & ANIM_STATIONARY || pSoldier->flags.fNoAPToFinishMove || fInitalMove )
		{
			if ( gAnimControl[ usAnimState ].ubHeight == ANIM_PRONE )
			{
				// Set this beasty of a flag to allow us to go back down to prone if we choose!
				// ATE: Alrighty, set flag to go back down only if we are not moving anywhere
				//if ( pSoldier->pathing.sDestination == pSoldier->sGridNo )
				if ( !fInitalMove )
				{
					pSoldier->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_ON;

					// Set a pending animation to change stance first...
					SendChangeSoldierStanceEvent( pSoldier, ANIM_CROUCH );

				}
			}
		}
	}

	// Set desired direction for the extended directions...
	pSoldier->ubHiResDesiredDirection = ubExtDirection[ pSoldier->pathing.bDesiredDirection ];

	if ( pSoldier->pathing.bDesiredDirection != pSoldier->ubDirection )
	{
		if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_VEHICLE ) || CREATURE_OR_BLOODCAT( pSoldier ) )
		{
			pSoldier->flags.uiStatusFlags |= SOLDIER_PAUSEANIMOVE;
		}
	}


	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		pSoldier->bTurningIncrement = (INT8) ExtQuickestDirection( pSoldier->ubHiResDirection, pSoldier->ubHiResDesiredDirection );
	}
	else
	{
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE )
		{
			pSoldier->bTurningIncrement = (INT8) MultiTiledTurnDirection( pSoldier, pSoldier->ubDirection, pSoldier->pathing.bDesiredDirection );
		}
		else
		{
			pSoldier->bTurningIncrement = (INT8) QuickestDirection( pSoldier->ubDirection, pSoldier->pathing.bDesiredDirection );
		}
	}

}


void SOLDIERTYPE::EVENT_SetSoldierDesiredDirection( UINT16	usNewDirection )
{
	EVENT_InternalSetSoldierDesiredDirection( this, (UINT8)usNewDirection, FALSE, this->usAnimState );
}


void SOLDIERTYPE::EVENT_SetSoldierDirection( UINT16	usNewDirection )
{
	// Remove old location data
	this->HandleAnimationProfile( this->usAnimState, TRUE );

	this->ubDirection = (INT8)usNewDirection;

	// Updated extended direction.....
	this->ubHiResDirection = ubExtDirection[ this->ubDirection ];

	// Add new stuff
	this->HandleAnimationProfile( this->usAnimState, FALSE );

	// If we are turning, we have chaanged our aim!
	if ( !this->flags.fDontUnsetLastTargetFromTurn )
	{
		this->sLastTarget = NOWHERE;
	}

	AdjustForFastTurnAnimation( this );

	// Update structure info!
	//	 if ( this->flags.uiStatusFlags & SOLDIER_MULTITILE )
	{
		UpdateMercStructureInfo( this );
	}

	// Handle Profile data for hit locations
	this->HandleAnimationProfile( this->usAnimState, TRUE );

	HandleCrowShadowNewDirection( this );

	// Change values!
	SetSoldierLocatorOffsets( this );

}


void SOLDIERTYPE::EVENT_BeginMercTurn( BOOLEAN fFromRealTime, INT32 iRealTimeCounter )
{
	// NB realtimecounter is not used, always passed in as 0 now!

	INT32 iBlood;

	if (this->aiData.bUnderFire)
	{
		// UnderFire now starts at 2 for "under fire this turn",
		// down to 1 for "under fire last turn", to 0.
		this->aiData.bUnderFire--;
	}

	// ATE: Add decay effect sfor drugs...
	if ( fFromRealTime  ) //&& iRealTimeCounter % 300 )
	{
		HandleEndTurnDrugAdjustments( this );
	}
	else
	{
		HandleEndTurnDrugAdjustments( this );
	}

	// ATE: Don't bleed if in AUTO BANDAGE!
	if ( !gTacticalStatus.fAutoBandageMode )
	{
		// Blood is not for the weak of heart, or mechanical
		if ( !( this->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_ROBOT ) ) )
		{
			if ( this->bBleeding || this->stats.bLife < OKLIFE ) // is he bleeding or dying?
			{
				iBlood = CheckBleeding( this );	// check if he might lose another life point

				// ATE: Only if in sector!
				if ( this->bInSector )
				{
					if ( iBlood != NOBLOOD )
					{
						DropBlood( this, (INT8)iBlood, this->bVisible );
					}
				}
			}
		}
	}

	// survived bleeding, but is he out of breath?
	if ( this->stats.bLife && !this->bBreath && this->MercInWater( ) )
	{
		// Drowning...
	}

	// if he is still alive (didn't bleed to death)
	if ( this->stats.bLife )
	{
		// reduce the effects of any residual shock from past injuries by half
		this->aiData.bShock /= 2;

		// if this person has heard a noise that hasn't been investigated
		if (this->aiData.sNoiseGridno != NOWHERE)
		{
			if (this->aiData.ubNoiseVolume)	// and the noise volume is still positive
			{
				this->aiData.ubNoiseVolume--;	// the volume of the noise "decays" by 1 point

				if (!this->aiData.ubNoiseVolume)	// if the volume has reached zero
				{
					this->aiData.sNoiseGridno = NOWHERE;		// forget about the noise!
				}
			}
		}

		// save unused action points up to a maximum
		/*
		if ((savedPts = this->aiData.bActionPts) > MAX_AP_CARRIED)
		savedPts = MAX_AP_CARRIED;
		*/
		if ( this->flags.uiStatusFlags & SOLDIER_GASSED )
		{
			// then must get a gas mask or leave the gassed area to get over it
			//			 if ( ( this->inv[ HEAD1POS ].usItem == GASMASK || this->inv[ HEAD2POS ].usItem == GASMASK ) || !( GetSmokeEffectOnTile( this->sGridNo, this->pathing.bLevel ) ) )
			if ( FindGasMask( this ) != NO_SLOT || !( GetSmokeEffectOnTile( this->sGridNo, this->pathing.bLevel ) ) )
			{
				// Turn off gassed flag....
				this->flags.uiStatusFlags &= (~SOLDIER_GASSED );
			}
		}

		if ( this->bBlindedCounter > 0 )
		{
			this->bBlindedCounter--;
			if (this->bBlindedCounter == 0)
			{
				// we can SEE!!!!!
				HandleSight( this, SIGHT_LOOK );
				// Dirty panel
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}


		if ( this->bDeafenedCounter > 0 )
		{
			this->bDeafenedCounter--;
		}

		// ATE: To get around a problem...
		// If an AI guy, and we have 0 life, and are still at higher hieght,
		// Kill them.....


		this->sWeightCarriedAtTurnStart = (INT16) CalculateCarriedWeight( this );

		UnusedAPsToBreath( this );

		// Set flag back to normal, after reaching a certain statge
		if ( this->bBreath > 80 )
		{
			this->usQuoteSaidFlags &= ( ~SOLDIER_QUOTE_SAID_LOW_BREATH );
		}
		if ( this->bBreath > 50 )
		{
			this->usQuoteSaidFlags &= ( ~SOLDIER_QUOTE_SAID_DROWNING );
		}


		if ( this->ubTurnsUntilCanSayHeardNoise > 0)
		{
			this->ubTurnsUntilCanSayHeardNoise--;
		}

		if ( this->bInSector )
		{
			this->CheckForBreathCollapse( );
		}

		this->CalcNewActionPoints( );

		this->bTilesMoved						= 0;

		if ( this->bInSector )
		{
			this->BeginSoldierGetup( );

			// CJC Nov 30: handle RT opplist decaying in another function which operates less often
			if ( gTacticalStatus.uiFlags & INCOMBAT )
			{
				VerifyAndDecayOpplist( this );

				// turn off xray
				if ( this->uiXRayActivatedTime )
				{
					TurnOffXRayEffects( this );
				}
			}

			if ( (this->bTeam == gbPlayerNum) && (this->ubProfile != NO_PROFILE) )
			{
				switch( gMercProfiles[ this->ubProfile ].bPersonalityTrait )
				{
				case FEAR_OF_INSECTS:
					if ( MercSeesCreature( this ) )
					{
						HandleMoraleEvent( this, MORALE_INSECT_PHOBIC_SEES_CREATURE, this->sSectorX, this->sSectorY, this->bSectorZ );
						if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
						{
							TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
							this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;
						}
					}
					break;
				case CLAUSTROPHOBIC:
					if ( gbWorldSectorZ > 0 && Random( 6 - gbWorldSectorZ ) == 0 )
					{
						// underground!
						HandleMoraleEvent( this, MORALE_CLAUSTROPHOBE_UNDERGROUND, this->sSectorX, this->sSectorY, this->bSectorZ );
						if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
						{
							TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
							this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;
						}

					}
					break;
				case NERVOUS:
					if ( DistanceToClosestFriend( this ) > NERVOUS_RADIUS )
					{
						// augh!!
						if ( this->aiData.bMorale < 50 )
						{
							HandleMoraleEvent( this, MORALE_NERVOUS_ALONE, this->sSectorX, this->sSectorY, this->bSectorZ );
							if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
							{
								TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
								this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;
							}
						}
					}
					else
					{
						if ( this->aiData.bMorale > 45 )
						{
							// turn flag off, so that we say it every two turns
							this->usQuoteSaidFlags &= ~SOLDIER_QUOTE_SAID_PERSONALITY;
						}
					}
					break;
				}
			}
		}

		// Reset quote flags for under heavy fire and close call!
		this->usQuoteSaidFlags &= ( ~SOLDIER_QUOTE_SAID_BEING_PUMMELED );
		this->usQuoteSaidExtFlags &= ( ~SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL );
		this->bNumHitsThisTurn = 0;
		this->ubSuppressionPoints = 0;
		this->flags.fCloseCall = FALSE;

		this->ubMovementNoiseHeard = 0;

		// If soldier has new APs, reset flags!
		if ( this->bActionPoints > 0 )
		{
			this->flags.fUIFirstTimeNOAP = FALSE;
			this->aiData.bMoved = FALSE;
			this->aiData.bPassedLastInterrupt = FALSE;
		}
	}
}

// UTILITY FUNCTIONS CALLED BY OVERHEAD.H
UINT8		gDirectionFrom8to2[ ] = { 0, 0, 1, 1, 0, 1, 1, 0 };


BOOLEAN SOLDIERTYPE::ConvertAniCodeToAniFrame( UINT16 usAniFrame )
{
	UINT16	usAnimSurface;
	UINT8		ubTempDir;
	// Given ani code, adjust for facing direction

	// get anim surface and determine # of frames
	usAnimSurface = GetSoldierAnimationSurface( this, this->usAnimState );

	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	// COnvert world direction into sprite direction
	ubTempDir = gOneCDirection[ this->ubDirection ];

	//If we are only one frame, ignore what the script is telling us!
	if ( gAnimSurfaceDatabase[ usAnimSurface ].ubFlags & ANIM_DATA_FLAG_NOFRAMES )
	{
		usAniFrame = 0;
	}

	if ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumDirections == 32 )
	{
		ubTempDir = gExtOneCDirection[ this->ubHiResDirection ];
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumDirections == 4 )
	{
		ubTempDir = ubTempDir / 2;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumDirections == 1 )
	{
		ubTempDir = 0;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumDirections == 3 )
	{
		if ( this->ubDirection == NORTHWEST )
		{
			ubTempDir = 1;
		}
		if ( this->ubDirection == WEST )
		{
			ubTempDir = 0;
		}
		if ( this->ubDirection == EAST )
		{
			ubTempDir = 2;
		}
	}
	else if ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumDirections == 2 )
	{
		ubTempDir = gDirectionFrom8to2[ this->ubDirection ];
	}

	this->usAniFrame = usAniFrame + (UINT16) ( ( gAnimSurfaceDatabase[ usAnimSurface ].uiNumFramesPerDir * ubTempDir ) );

	if ( gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject == NULL )
	{
		this->usAniFrame = 0;
		return( TRUE );
	}

	if ( this->usAniFrame >= gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->usNumberOfObjects )
	{
		// Debug msg here....
		//		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Soldier Animation: Wrong Number of frames per number of objects: %d vs %d, %S",  gAnimSurfaceDatabase[ usAnimSurface ].uiNumFramesPerDir, gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->usNumberOfObjects, gAnimControl[ this->usAnimState ].zAnimStr );

		this->usAniFrame = 0;
	}

	return( TRUE );
}


void SOLDIERTYPE::TurnSoldier( void )
{
	INT16		sDirection;
	BOOLEAN	fDoDirectionChange = TRUE;
	INT32		cnt;

	// If we are a vehicle... DON'T TURN!
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		if ( this->ubBodyType != TANK_NW && this->ubBodyType != TANK_NE )
		{
			return;
		}
	}
	else	// Lesh: patch for "Bug: Enemy turns around in turn based mode!"
	{
		// in case of errors in turning tasks
		if ( this->pathing.bDesiredDirection > 7 || this->pathing.bDesiredDirection < 0)
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("TurnSoldier() Warinig: Invalid desired direction for non-vehicle unit") );
			this->pathing.bDesiredDirection = this->ubDirection;
		}
	}
	// Lesh: patch ended

	// We handle sight now....
	if ( this->flags.uiStatusFlags & SOLDIER_LOOK_NEXT_TURNSOLDIER )
	{
		if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_STATIONARY && this->usAnimState != CLIMBUPROOF && this->usAnimState != CLIMBDOWNROOF ) )
		{
			// HANDLE SIGHT!
			HandleSight( this,SIGHT_LOOK | SIGHT_RADIO );
		}
		// Turn off!
		this->flags.uiStatusFlags &= (~SOLDIER_LOOK_NEXT_TURNSOLDIER );

		HandleSystemNewAISituation( this, FALSE );
	}


	if ( this->flags.fTurningToShoot )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			if ( ( (gAnimControl[ this->usAnimState ].uiFlags & ANIM_FIREREADY ) && 
				this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_OFF) || 
				this->ubBodyType == ROBOTNOWEAPON || 
				this->ubBodyType == TANK_NW || 
				this->ubBodyType == TANK_NE  )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("TurnSoldier: EVENT_InitNewSoldierAnim") );
				this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[ this->usAnimState ].ubEndHeight ), 0, FALSE );
				this->flags.fTurningToShoot = FALSE;

				// Save last target gridno!
				//this->sLastTarget = this->sTargetGridNo;

			}
			// Else check if we are trying to shoot and once was prone, but am now crouched because we needed to turn...
			else if ( this->flags.bTurningFromPronePosition )
			{
				if ( IsValidStance( this, ANIM_PRONE ) )
				{
					SendChangeSoldierStanceEvent( this, ANIM_PRONE );
					this->usPendingAnimation = SelectFireAnimation( this, ANIM_PRONE );
				}
				else
				{
					this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, ANIM_CROUCH ), 0, FALSE );
				}
				this->flags.fTurningToShoot = FALSE;
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
			}
		}
	}

	if ( this->flags.fTurningToFall )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			SelectFallAnimation( this );
			this->flags.fTurningToFall = FALSE;
		}
	}

	if ( this->flags.fTurningUntilDone && ( this->ubPendingStanceChange != NO_PENDING_STANCE ) )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			SendChangeSoldierStanceEvent( this, this->ubPendingStanceChange );
			this->ubPendingStanceChange = NO_PENDING_STANCE;
			this->flags.fTurningUntilDone = FALSE;
		}
	}

	if ( this->flags.fTurningUntilDone && ( this->usPendingAnimation != NO_PENDING_ANIMATION ) )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			UINT16 usPendingAnimation;

			usPendingAnimation = this->usPendingAnimation;
			this->usPendingAnimation = NO_PENDING_ANIMATION;

			this->EVENT_InitNewSoldierAnim( usPendingAnimation, 0 , FALSE );
			this->flags.fTurningUntilDone = FALSE;
		}
	}

	// Don't do anything if we are at dest direction!
	if ( this->ubDirection == this->pathing.bDesiredDirection )
	{
		if ( this->ubBodyType == TANK_NW || this->ubBodyType == TANK_NE )
		{
			if ( this->iTuringSoundID != NO_SAMPLE )
			{
				SoundStop( this->iTuringSoundID );
				this->iTuringSoundID = NO_SAMPLE;

				PlaySoldierJA2Sample( this->ubID, TURRET_STOP, RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ), TRUE );
			}
		}

		// Turn off!
		this->flags.uiStatusFlags &= (~SOLDIER_LOOK_NEXT_TURNSOLDIER );
		this->flags.fDontUnsetLastTargetFromTurn = FALSE;

		// Unset ui busy if from ui
		if ( this->bTurningFromUI && 
			( this->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE ) && 
			( this->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ON ) )
		{
			UnSetUIBusy( this->ubID );
			this->bTurningFromUI = FALSE;
		}

		if ( this->flags.uiStatusFlags & ( SOLDIER_VEHICLE ) || CREATURE_OR_BLOODCAT( this ) )
		{
			this->flags.uiStatusFlags &= (~SOLDIER_PAUSEANIMOVE);
		}

		FreeUpNPCFromTurning( this, LOOK);

		// Undo our flag for prone turning...
		// Else check if we are trying to shoot and once was prone, but am now crouched because we needed to turn...
		if ( this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_ON )
		{
			// ATE: Don't do this if we have something in our hands we are going to throw!
			if ( IsValidStance( this, ANIM_PRONE ) && this->pTempObject == NULL )
			{
				SendChangeSoldierStanceEvent( this, ANIM_PRONE );
			}
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
		}

		// If a special code, make guy crawl after stance change!
		if ( this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE && 
			this->usAnimState != PRONE_UP && 
			this->usAnimState != PRONE_DOWN )
		{
			if ( IsValidStance( this, ANIM_PRONE ) )
			{
				this->EVENT_InitNewSoldierAnim( CRAWLING, 0, FALSE );
			}
			// Else swat for a tile so that there's room to resume prone
			else
			{
				this->EVENT_InitNewSoldierAnim( this->usUIMovementMode, 0 , FALSE );
			}
		}

		if ( this->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT )
		{
			// This section seems problem-prone.  It relies on all animations happening without interruption.  There must be a more
			// foolproof method.
			if ( this->flags.fGettingHit == 1 )
			{
				if ( this->usPendingAnimation != FALLFORWARD_ROOF && this->usPendingAnimation != FALLOFF && this->usAnimState != FALLFORWARD_ROOF && this->usAnimState != FALLOFF )
				{
					// Go back to original direction
					this->EVENT_SetSoldierDesiredDirection( (INT8)this->aiData.uiPendingActionData1 );

					//SETUP GETTING HIT FLAG TO 2
					this->flags.fGettingHit = 2;
				}
				else
				{
					this->flags.uiStatusFlags &= (~SOLDIER_TURNINGFROMHIT );
					this->flags.fGettingHit = FALSE;
				}
			}
			else if ( this->flags.fGettingHit == 2 )
			{
				// Turn off
				this->flags.uiStatusFlags &= (~SOLDIER_TURNINGFROMHIT );


				// Release attacker
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, turning from hit animation ended") );
				// ReleaseSoldiersAttacker( this );
				// 0verhaul:  Since I disabled the turn from hit ABC increase, I need to disable the turn from hit decrease too.
				DebugAttackBusy( "Finished turning from hit.  Not Reducing attack busy.\n");
				//ReduceAttackBusyCount( );

				//FREEUP GETTING HIT FLAG
				this->flags.fGettingHit = FALSE;
			}
		}

		return;
	}

	// IF WE ARE HERE, WE ARE IN THE PROCESS OF TURNING

	// DOUBLE CHECK TO UNSET fNOAPs...
	if ( this->flags.fNoAPToFinishMove )
	{
		this->AdjustNoAPToFinishMove( FALSE );
	}

	// Do something different for vehicles....
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		fDoDirectionChange = FALSE;

		// Get new direction
		/*
		sDirection = this->ubHiResDirection + ExtQuickestDirection( this->ubHiResDirection, this->ubHiResDesiredDirection );
		*/
		sDirection = this->ubHiResDirection + this->bTurningIncrement;
		if (sDirection > 31)
		{
			sDirection = 0;
		}
		else
		{
			if ( sDirection < 0 )
			{
				sDirection = 31;
			}
		}
		this->ubHiResDirection = (UINT8)sDirection;

		// Are we at a multiple of a 'cardnal' direction?
		for ( cnt = 0; cnt < 8; cnt++ )
		{
			if ( sDirection == ubExtDirection[ cnt ] )
			{
				fDoDirectionChange = TRUE;

				sDirection = (INT16)cnt;

				break;
			}
		}

		if ( this->ubBodyType == TANK_NW || this->ubBodyType == TANK_NE )
		{
			if ( this->iTuringSoundID == NO_SAMPLE )
			{
				this->iTuringSoundID = PlaySoldierJA2Sample( this->ubID, TURRET_MOVE, RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 100, SoundDir( this->sGridNo ), TRUE );
			}
		}
	}
	else
	{
		// Get new direction
		//sDirection = this->ubDirection + QuickestDirection( this->ubDirection, this->pathing.bDesiredDirection );
		sDirection = this->ubDirection + this->bTurningIncrement;
		if (sDirection > 7)
		{
			sDirection = 0;
		}
		else
		{
			if ( sDirection < 0 )
			{
				sDirection = 7;
			}
		}
	}


	// CHECK FOR A VALID TURN DIRECTION
	// This is needed for prone animations as well as any multi-tiled structs
	if ( fDoDirectionChange )
	{
		// If the soldier is not crawling or multi-tiled, he should be allowed to turn in place.  Even if there is some
		// obstacle he shouldn't be standing on.
		if ( OKToAddMercToWorld( this, (INT8)sDirection ) )
		{
			if ( gubWaitingForAllMercsToExitCode == WAIT_FOR_MERCS_TO_WALKOFF_SCREEN || gubWaitingForAllMercsToExitCode == WAIT_FOR_MERCS_TO_WALK_TO_GRIDNO )
			{
				// Don't do this if we are walking off screen...

			}
			else
			{
				// ATE: We should only do this if we are STATIONARY!
				if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_STATIONARY ) )
				{
					this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;
				}
				// otherwise, it's handled next tile...
			}

			this->EVENT_SetSoldierDirection( sDirection );

			if ( this->ubBodyType != LARVAE_MONSTER && !this->MercInWater( ) && this->bOverTerrainType != DIRT_ROAD && this->bOverTerrainType != PAVED_ROAD )
			{
				PlaySoldierFootstepSound( this );
			}
		}
		// Are we prone crawling?
		else if ( this->usAnimState == CRAWLING )
		{
			// OK, we want to getup, turn and go prone again....
			SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE;
		}
		// If we are a creature, or multi-tiled, cancel AI action.....?
		else if ( this->flags.uiStatusFlags & SOLDIER_MULTITILE )
		{
			this->pathing.bDesiredDirection = this->ubDirection;
		}
	}
}


UINT8	gRedGlowR[]=
{
	0,			// Normal shades
	25,
	50,
	75,
	100,
	125,
	150,
	175,
	200,
	225,

	0,		// For gray palettes
	25,
	50,
	75,
	100,
	125,
	150,
	175,
	200,
	225,

};


#if 0
UINT8	gOrangeGlowR[]=
{
	0,			// Normal shades
	20,
	40,
	60,
	80,
	100,
	120,
	140,
	160,
	180,

	0,		// For gray palettes
	20,
	40,
	60,
	80,
	100,
	120,
	140,
	160,
	180,
};
#endif

UINT8	gOrangeGlowR[]=
{
	0,			// Normal shades
	25,
	50,
	75,
	100,
	125,
	150,
	175,
	200,
	225,

	0,		// For gray palettes
	25,
	50,
	75,
	100,
	125,
	150,
	175,
	200,
	225,

};


#if 0
UINT8	gOrangeGlowG[]=
{
	0,			// Normal shades
	5,
	10,
	25,
	30,
	35,
	40,
	45,
	50,
	55,

	0,		// For gray palettes
	5,
	10,
	25,
	30,
	35,
	40,
	45,
	50,
	55,
};
#endif

UINT8	gOrangeGlowG[]=
{
	0,			// Normal shades
	20,
	40,
	60,
	80,
	100,
	120,
	140,
	160,
	180,

	0,		// For gray palettes
	20,
	40,
	60,
	80,
	100,
	120,
	140,
	160,
	180,

};


BOOLEAN SOLDIERTYPE::CreateSoldierPalettes( void )
{
	UINT16 usAnimSurface, usPaletteAnimSurface;
	CHAR8	zColFilename[ 100 ];
	INT32 iWhich;
	INT32 cnt;
	INT8	bBodyTypePalette;
	SGPPaletteEntry							Temp8BPPPalette[ 256 ];

	//NT32 uiCount;
	//PPaletteEntry Pal[256];

	if ( this->p8BPPPalette != NULL )
	{
		MemFree( this->p8BPPPalette );
		this->p8BPPPalette = NULL;
	}


	// Allocate mem for new palette
	this->p8BPPPalette = (SGPPaletteEntry *) MemAlloc( sizeof( SGPPaletteEntry ) * 256 );
	memset( this->p8BPPPalette, 0, sizeof( SGPPaletteEntry ) * 256 );

	CHECKF( this->p8BPPPalette != NULL );

	// --- TAKE FROM CURRENT ANIMATION HVOBJECT!
	usAnimSurface = GetSoldierAnimationSurface( this, this->usAnimState );

	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	if ( ( bBodyTypePalette = GetBodyTypePaletteSubstitutionCode( this, this->ubBodyType, zColFilename ) ) == -1 )
	{
		// ATE: here we want to use the breath cycle for the palette.....
		usPaletteAnimSurface = LoadSoldierAnimationSurface( this, STANDING );

		if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
		{
			// Use palette from HVOBJECT, then use substitution for pants, etc
			memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[ usPaletteAnimSurface ].hVideoObject->pPaletteEntry, sizeof( this->p8BPPPalette ) * 256 );

			// Substitute based on head, etc
			SetPaletteReplacement( this->p8BPPPalette, this->HeadPal );
			SetPaletteReplacement( this->p8BPPPalette, this->VestPal );
			SetPaletteReplacement( this->p8BPPPalette, this->PantsPal );
			SetPaletteReplacement( this->p8BPPPalette, this->SkinPal );
		}
	}
	else if ( bBodyTypePalette == 0 )
	{
		// Use palette from hvobject
		memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->pPaletteEntry, sizeof( this->p8BPPPalette ) * 256 );
	}
	else
	{
		// Use col file
		if ( CreateSGPPaletteFromCOLFile( Temp8BPPPalette, zColFilename ) )
		{
			// Copy into palette
			memcpy( this->p8BPPPalette,		Temp8BPPPalette, sizeof( this->p8BPPPalette ) * 256 );
		}
		else
		{
			// Use palette from hvobject
			memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->pPaletteEntry, sizeof( this->p8BPPPalette ) * 256 );
		}
	}


	if ( this->p16BPPPalette != NULL )
	{
		MemFree( this->p16BPPPalette );
		this->p16BPPPalette = NULL;
	}

	// -- BUILD 16BPP Palette from this
	this->p16BPPPalette = Create16BPPPalette( this->p8BPPPalette );

	for ( iWhich = 0; iWhich < NUM_SOLDIER_SHADES; iWhich++ )
	{
		if ( this->pShades[ iWhich ] != NULL )
		{
			MemFree( this->pShades[ iWhich ] );
			this->pShades[ iWhich ] = NULL;
		}
	}

	for ( iWhich = 0; iWhich < NUM_SOLDIER_EFFECTSHADES; iWhich++ )
	{
		if ( this->pEffectShades[ iWhich ] != NULL )
		{
			MemFree( this->pEffectShades[ iWhich ] );
			this->pEffectShades[ iWhich ] = NULL;
		}
	}

	for ( iWhich = 0; iWhich < 20; iWhich++ )
	{
		if ( this->pGlowShades[ iWhich ] != NULL )
		{
			MemFree( this->pGlowShades[ iWhich ] );
			this->pGlowShades[ iWhich ] = NULL;
		}
	}


	CreateSoldierPaletteTables( this, HVOBJECT_GLOW_GREEN );


	// Build a grayscale palette for testing grayout of mercs
	//for(uiCount=0; uiCount < 256; uiCount++)
	//{
	//	Pal[uiCount].peRed=(UINT8)(uiCount%128)+128;
	//	Pal[uiCount].peGreen=(UINT8)(uiCount%128)+128;
	//	Pal[uiCount].peBlue=(UINT8)(uiCount%128)+128;
	//}
	this->pEffectShades[ 0 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	this->pEffectShades[ 1 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 150, 100, TRUE );

	// Build shades for glowing visible bad guy

	// First do visible guy
	this->pGlowShades[ 0 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 255, 255, 255, FALSE );
	for ( cnt = 1; cnt < 10; cnt++ )
	{
		this->pGlowShades[ cnt ] = CreateEnemyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[cnt], 255, FALSE );
	}

	// Now for gray guy...
	this->pGlowShades[ 10 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	for ( cnt = 11; cnt < 19; cnt++ )
	{
		this->pGlowShades[ cnt ] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[cnt], 0, FALSE );
	}
	this->pGlowShades[ 19 ] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[18], 0, FALSE );


	// ATE: OK, piggyback on the shades we are not using for 2 colored lighting....
	// ORANGE, VISIBLE GUY
	this->pShades[ 20 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 255, 255, 255, FALSE );
	for ( cnt = 21; cnt < 30; cnt++ )
	{
		this->pShades[ cnt ] = CreateEnemyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[ ( cnt - 20 )], gOrangeGlowG[ ( cnt - 20 ) ], TRUE );
	}

	// ORANGE, GREY GUY
	this->pShades[ 30 ] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	for ( cnt = 31; cnt < 39; cnt++ )
	{
		this->pShades[ cnt ] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[ ( cnt - 20 ) ], gOrangeGlowG[ ( cnt - 20 ) ], TRUE );
	}
	this->pShades[ 39 ] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[18], gOrangeGlowG[18], TRUE );

	return( TRUE );
}



void AdjustAniSpeed( SOLDIERTYPE *pSoldier )
{
	if ( ( gTacticalStatus.uiFlags & SLOW_ANIMATION ) )
	{
		if ( gTacticalStatus.bRealtimeSpeed == -1 )
		{
			pSoldier->sAniDelay = 10000;
		}
		else
		{
			pSoldier->sAniDelay = pSoldier->sAniDelay * ( 1 * gTacticalStatus.bRealtimeSpeed / 2 );
		}
	}


	RESETTIMECOUNTER( pSoldier->timeCounters.UpdateCounter, pSoldier->sAniDelay );
}


void CalculateSoldierAniSpeed( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pStatsSoldier )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"CalculateSoldierAniSpeed");
	UINT32 uiTerrainDelay;
	UINT32 uiSpeed = 0;

	INT8 bBreathDef, bLifeDef, bAgilDef;
	INT8 bAdditional = 0;

	// for those animations which have a speed of zero, we have to calculate it
	// here. Some animation, such as water-movement, have an ADDITIONAL speed
	switch( pSoldier->usAnimState )
	{
		// Lesh: bursting animation delay control begins
		// Add your animation ID to control it
	case STANDING_BURST:
	case FIRE_STAND_BURST_SPREAD:
	case FIRE_BURST_LOW_STAND:
	case TANK_BURST:
	case CROUCHED_BURST:
	case PRONE_BURST:
		pSoldier->sAniDelay = Weapon[Item[pSoldier->inv[HANDPOS].usItem].ubClassIndex].sAniDelay;
		AdjustAniSpeed( pSoldier );
		return;
		// Lesh: end

	case PRONE:
	case STANDING:

		pSoldier->sAniDelay = ( pStatsSoldier->bBreath * 2 ) + (100 - pStatsSoldier->stats.bLife );

		// Limit it!
		if ( pSoldier->sAniDelay < 40 )
		{
			pSoldier->sAniDelay = 40;
		}
		AdjustAniSpeed( pSoldier );
		return;

	case CROUCHING:

		pSoldier->sAniDelay = ( pStatsSoldier->bBreath * 2 ) + ( (100 - pStatsSoldier->stats.bLife ) );

		// Limit it!
		if ( pSoldier->sAniDelay < 40 )
		{
			pSoldier->sAniDelay = 40;
		}
		AdjustAniSpeed( pSoldier );
		return;

	case WALKING:

		// Adjust based on body type
		bAdditional = (UINT8)( gubAnimWalkSpeeds[ pStatsSoldier->ubBodyType ].sSpeed );
		if ( bAdditional < 0 )
			bAdditional = 0;
		break;

	case RUNNING:

		// Adjust based on body type
		bAdditional = (UINT8)gubAnimRunSpeeds[ pStatsSoldier->ubBodyType ].sSpeed;
		if ( bAdditional < 0 )
			bAdditional = 0;
		break;

	case SWATTING:

		// Adjust based on body type
		if ( pStatsSoldier->ubBodyType <= REGFEMALE )
		{
			bAdditional = (UINT8)gubAnimSwatSpeeds[ pStatsSoldier->ubBodyType ].sSpeed;
			if ( bAdditional < 0 )
				bAdditional = 0;
		}
		break;

	case CRAWLING:

		// Adjust based on body type
		if ( pStatsSoldier->ubBodyType <= REGFEMALE )
		{
			bAdditional = (UINT8)gubAnimCrawlSpeeds[ pStatsSoldier->ubBodyType ].sSpeed;
			if ( bAdditional < 0 )
				bAdditional = 0;
		}
		break;

	case READY_RIFLE_STAND:

		// Raise rifle based on aim vs non-aim.
		if ( pSoldier->aiData.bAimTime == 0 )
		{
			// Quick shot
			pSoldier->sAniDelay = 70;
		}
		else
		{
			pSoldier->sAniDelay = 150;
		}
		AdjustAniSpeed( pSoldier );
		return;
	}


	// figure out movement speed (terrspeed)
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_MOVING )
	{
		uiSpeed = gsTerrainTypeSpeedModifiers[ pStatsSoldier->bOverTerrainType ];

		uiTerrainDelay = uiSpeed;
	}
	else
	{
		uiTerrainDelay = 40;			// standing still
	}

	bBreathDef = 50 - ( pStatsSoldier->bBreath / 2 );

	if ( bBreathDef > 30 )
		bBreathDef = 30;

	bAgilDef = 50 - ( EffectiveAgility( pStatsSoldier ) / 4 );
	bLifeDef = 50 - ( pStatsSoldier->stats.bLife / 2 );

	uiTerrainDelay += ( bLifeDef + bBreathDef + bAgilDef + bAdditional );

	pSoldier->sAniDelay = (INT16)uiTerrainDelay;

	// If a moving animation and w/re on drugs, increase speed....
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_MOVING )
	{
		if ( GetDrugEffect( pSoldier, DRUG_TYPE_ADRENALINE ) )
		{
			pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
		}
	}

	// MODIFTY NOW BASED ON REAL-TIME, ETC
	// Adjust speed, make twice as fast if in turn-based!
	if ( gTacticalStatus.uiFlags & TURNBASED && ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
	}

	// MODIFY IF REALTIME COMBAT
	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// ATE: If realtime, and stealth mode...
		if ( pStatsSoldier->bStealthMode )
		{
			pSoldier->sAniDelay = (INT16)( pSoldier->sAniDelay * 2 );
		}

		//pSoldier->sAniDelay = pSoldier->sAniDelay * ( 1 * gTacticalStatus.bRealtimeSpeed / 2 );
	}

}

UINT8 GetSpeedUpFactor( )
{
	switch(  gTacticalStatus.ubCurrentTeam )
	{
	case OUR_TEAM:
		return gGameExternalOptions.gubPlayerTurnSpeedUpFactor;
	case ENEMY_TEAM:
		return gGameExternalOptions.gubEnemyTurnSpeedUpFactor;
	case CREATURE_TEAM:
		return gGameExternalOptions.gubCreatureTurnSpeedUpFactor;
	case MILITIA_TEAM:
		return gGameExternalOptions.gubMilitiaTurnSpeedUpFactor;
	case CIV_TEAM:
		return gGameExternalOptions.gubCivilianTurnSpeedUpFactor;
	}

	return 1;
}

void SetSoldierAniSpeed( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pStatsSoldier;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"SetSoldierAniSpeed");

	// ATE: If we are an enemy and are not visible......
	// Set speed to 0
	if(!is_client)
	{
	if ( ( gTacticalStatus.uiFlags & TURNBASED && ( gTacticalStatus.uiFlags & INCOMBAT ) ) || gTacticalStatus.fAutoBandageMode )
	{
		if ( ( ( pSoldier->bVisible == -1 && pSoldier->bVisible == pSoldier->bLastRenderVisibleValue ) || gTacticalStatus.fAutoBandageMode ) && pSoldier->usAnimState != MONSTER_UP )
		{
			if ( pSoldier->bDoBurst && !PTR_OURTEAM )
			{
				pSoldier->sAniDelay = 50;
			}
			else
			{
				pSoldier->sAniDelay = 0;
			}
			RESETTIMECOUNTER( pSoldier->timeCounters.UpdateCounter, pSoldier->sAniDelay );
			return;
		}
	}
	}

	// Default stats soldier to same as normal soldier.....
	pStatsSoldier = pSoldier;

	if ( pSoldier->flags.fUseMoverrideMoveSpeed )
	{
		pStatsSoldier = MercPtrs[ pSoldier->bOverrideMoveSpeed ];
	}

	// Only calculate if set to zero
	if ( ( pSoldier->sAniDelay = gAnimControl[ pSoldier->usAnimState ].sSpeed ) == 0 )
	{
		CalculateSoldierAniSpeed( pSoldier, pStatsSoldier );
	}

	AdjustAniSpeed( pSoldier );

	if ( _KeyDown( SPACE ) )
	{
		//pSoldier->sAniDelay = 1000;
	}

	if ( gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT )
		if( GetSpeedUpFactor() )
			pSoldier->sAniDelay /= GetSpeedUpFactor();
		else
			pSoldier->sAniDelay = 0;
}


///////////////////////////////////////////////////////
//PALETTE REPLACEMENT FUNCTIONS
///////////////////////////////////////////////////////
BOOLEAN LoadPaletteData( )
{
	HWFILE		hFile;
	UINT32			cnt, cnt2;

	hFile = FileOpen( PALETTEFILENAME, FILE_ACCESS_READ, FALSE );

	// Read # of types
	if ( !FileRead( hFile, &guiNumPaletteSubRanges, sizeof( guiNumPaletteSubRanges ), (UINT32 *)NULL ) )
	{
		return( FALSE );
	}

	// Malloc!
	gpPaletteSubRanges = (PaletteSubRangeType *) MemAlloc( sizeof( PaletteSubRangeType ) * guiNumPaletteSubRanges );
	gubpNumReplacementsPerRange = (UINT8 *) MemAlloc( sizeof( UINT8 ) * guiNumPaletteSubRanges );

	// Read # of types for each!
	for ( cnt = 0; cnt < guiNumPaletteSubRanges; cnt++ )
	{
		if ( !FileRead( hFile, &gubpNumReplacementsPerRange[ cnt ], sizeof( UINT8 ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}
	}

	// Loop for each one, read in data
	for ( cnt = 0; cnt < guiNumPaletteSubRanges; cnt++ )
	{
		if ( !FileRead( hFile, &gpPaletteSubRanges[ cnt ].ubStart, sizeof( UINT8 ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}
		if ( !FileRead( hFile, &gpPaletteSubRanges[ cnt ].ubEnd, sizeof( UINT8 ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}
	}


	// Read # of palettes
	if ( !FileRead( hFile, &guiNumReplacements, sizeof( guiNumReplacements ), (UINT32 *)NULL ) )
	{
		return( FALSE );
	}

	// Malloc!
	gpPalRep = (PaletteReplacementType *) MemAlloc( sizeof( PaletteReplacementType ) * guiNumReplacements );

	// Read!
	for ( cnt = 0; cnt < guiNumReplacements; cnt++ )
	{
		// type
		if ( !FileRead( hFile, &gpPalRep[ cnt ].ubType, sizeof( gpPalRep[ cnt ].ubType ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}

		if ( !FileRead( hFile, &gpPalRep[ cnt ].ID, sizeof( gpPalRep[ cnt ].ID ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}

		// # entries
		if ( !FileRead( hFile, &gpPalRep[ cnt ].ubPaletteSize, sizeof( gpPalRep[ cnt ].ubPaletteSize ), (UINT32 *)NULL ) )
		{
			return( FALSE );
		}

		// Malloc
		gpPalRep[ cnt ].r = (UINT8 *) MemAlloc( gpPalRep[ cnt ].ubPaletteSize );
		CHECKF( gpPalRep[ cnt ].r != NULL );
		gpPalRep[ cnt ].g = (UINT8 *) MemAlloc( gpPalRep[ cnt ].ubPaletteSize );
		CHECKF( gpPalRep[ cnt ].g != NULL );
		gpPalRep[ cnt ].b = (UINT8 *) MemAlloc( gpPalRep[ cnt ].ubPaletteSize );
		CHECKF( gpPalRep[ cnt ].b != NULL );

		for( cnt2 = 0; cnt2 < gpPalRep[ cnt ].ubPaletteSize; cnt2++ )
		{
			if ( !FileRead( hFile, &gpPalRep[ cnt ].r[ cnt2 ], sizeof( UINT8 ), (UINT32 *)NULL ) )
			{
				return( FALSE );
			}
			if ( !FileRead( hFile, &gpPalRep[ cnt ].g[ cnt2 ], sizeof( UINT8 ), (UINT32 *)NULL ) )
			{
				return( FALSE );
			}
			if ( !FileRead( hFile, &gpPalRep[ cnt ].b[ cnt2 ], sizeof( UINT8 ), (UINT32 *)NULL ) )
			{
				return( FALSE );
			}
		}

	}

	FileClose( hFile );

	return( TRUE );
}

BOOLEAN	SetPaletteReplacement( SGPPaletteEntry *p8BPPPalette, PaletteRepID aPalRep )
{
	UINT32 cnt2;
	UINT8	 ubType;
	UINT8  ubPalIndex;

	CHECKF( GetPaletteRepIndexFromID( aPalRep, &ubPalIndex ) );

	// Get range type
	ubType = gpPalRep[ ubPalIndex ].ubType;

	for ( cnt2 = gpPaletteSubRanges[ ubType ].ubStart; cnt2 <= gpPaletteSubRanges[ ubType ].ubEnd; cnt2++ )
	{
		p8BPPPalette[ cnt2 ].peRed =  gpPalRep[ ubPalIndex ].r[ cnt2 - gpPaletteSubRanges[ ubType ].ubStart ];
		p8BPPPalette[ cnt2 ].peGreen = gpPalRep[ ubPalIndex ].g[ cnt2 - gpPaletteSubRanges[ ubType ].ubStart ];
		p8BPPPalette[ cnt2 ].peBlue = gpPalRep[ ubPalIndex ].b[ cnt2 - gpPaletteSubRanges[ ubType ].ubStart ];
	}

	return( TRUE );
}


BOOLEAN DeletePaletteData( )
{
	UINT32 cnt;

	// Free!
	if ( gpPaletteSubRanges != NULL )
	{
		MemFree( gpPaletteSubRanges );
		gpPaletteSubRanges = NULL;
	}

	if ( gubpNumReplacementsPerRange != NULL )
	{
		MemFree( gubpNumReplacementsPerRange );
		gubpNumReplacementsPerRange = NULL;
	}


	for ( cnt = 0; cnt < guiNumReplacements; cnt++ )
	{
		// Free
		if ( gpPalRep[ cnt ].r != NULL )
		{
			MemFree( gpPalRep[ cnt ].r );
			gpPalRep[ cnt ].r = NULL;
		}
		if ( gpPalRep[ cnt ].g != NULL )
		{
			MemFree( gpPalRep[ cnt ].g );
			gpPalRep[ cnt ].g = NULL;
		}
		if ( gpPalRep[ cnt ].b != NULL )
		{
			MemFree( gpPalRep[ cnt ].b );
			gpPalRep[ cnt ].b  = NULL;
		}
	}

	// Free
	if ( gpPalRep != NULL )
	{
		MemFree( gpPalRep );
		gpPalRep = NULL;
	}

	return( TRUE );
}


BOOLEAN GetPaletteRepIndexFromID( PaletteRepID aPalRep, UINT8 *pubPalIndex )
{
	UINT32 cnt;

	// Check if type exists
	for ( cnt = 0; cnt < guiNumReplacements; cnt++ )
	{
		if ( COMPARE_PALETTEREP_ID( aPalRep, gpPalRep[ cnt ].ID ) )
		{
			*pubPalIndex = ( UINT8 )cnt;
			return( TRUE );
		}
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Invalid Palette Replacement ID given");
	return( FALSE );
}

UINT16 SOLDIERTYPE::GetNewSoldierStateFromNewStance( UINT8 ubDesiredStance )
{
	UINT16 usNewState;
	INT8	bCurrentHeight;

	bCurrentHeight = ( ubDesiredStance - gAnimControl[ this->usAnimState ].ubEndHeight );

	// Now change to appropriate animation

	switch( bCurrentHeight )
	{
	case ANIM_STAND - ANIM_CROUCH:
		usNewState	= KNEEL_UP;
		break;
	case ANIM_CROUCH - ANIM_STAND:
		usNewState	= KNEEL_DOWN;
		break;

	case ANIM_STAND - ANIM_PRONE:
		usNewState	= PRONE_UP;
		break;
	case ANIM_PRONE - ANIM_STAND:
		usNewState	= KNEEL_DOWN;
		break;

	case ANIM_CROUCH - ANIM_PRONE:
		usNewState	= PRONE_UP;
		break;
	case ANIM_PRONE - ANIM_CROUCH:
		usNewState	= PRONE_DOWN;
		break;

	default:

		// Cannot get here unless ub desired stance is bogus
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "GetNewSoldierStateFromNewStance bogus ubDesiredStance value %d", ubDesiredStance ) );
		usNewState = this->usAnimState;

	}

	return( usNewState );
}


void MoveMercFacingDirection( SOLDIERTYPE *pSoldier, BOOLEAN fReverse, FLOAT dMovementDist )
{
	FLOAT					dAngle = (FLOAT)0;

	// Determine which direction we are in
	switch( pSoldier->ubDirection )
	{
	case NORTH:
		dAngle = (FLOAT)( -1 * PI );
		break;

	case NORTHEAST:
		dAngle = (FLOAT)( PI * .75 );
		break;

	case EAST:
		dAngle = (FLOAT)( PI / 2 );
		break;

	case SOUTHEAST:
		dAngle = (FLOAT)( PI / 4 );
		break;

	case SOUTH:
		dAngle = (FLOAT)0;
		break;

	case SOUTHWEST:
		//dAngle = (FLOAT)(  PI * -.25 );
		dAngle = (FLOAT)-0.786;
		break;

	case WEST:
		dAngle = (FLOAT) ( PI *-.5 );
		break;

	case NORTHWEST:
		dAngle = (FLOAT) ( PI * -.75 );
		break;

	}

	if ( fReverse )
	{
		dMovementDist = dMovementDist * -1;
	}

	pSoldier->MoveMerc( dMovementDist, dAngle, FALSE );

}

void SOLDIERTYPE::BeginSoldierClimbUpRoof( void )
{

	//CHRISL: Disable climbing up to a roof while wearing a backpack
	if((UsingNewInventorySystem() == true) && this->inv[BPACKPOCKPOS].exists() == true) {
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB] );
		return;
	}
	INT8							bNewDirection;
	UINT8							ubWhoIsThere;
	if(is_client)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[43] );
		return;//hayden disable climbing roof
	}
	if ( FindHeigherLevel( this, this->sGridNo, this->ubDirection, &bNewDirection ) && ( this->pathing.bLevel == 0 ) )
	{
		if ( EnoughPoints( this, GetAPsToClimbRoof( this, FALSE ), 0, TRUE ) )
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2( NewGridNo( (INT16)this->sGridNo, (UINT16)DirectionInc(bNewDirection ) ), 1 );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID )
		 {
			 return;
		 }
			else
		 {

			 if (this->bTeam == gbPlayerNum)
			 {
				 // OK, SET INTERFACE FIRST
				 SetUIBusy( this->ubID );
			 }

			 this->sTempNewGridNo = NewGridNo( (INT16)this->sGridNo, (UINT16)DirectionInc(bNewDirection ) );

			 this->ubPendingDirection = bNewDirection;
			 //this->usPendingAnimation = CLIMBUPROOF;
			 this->EVENT_InitNewSoldierAnim( CLIMBUPROOF, 0 , FALSE );

			 this->InternalReceivingSoldierCancelServices( FALSE );
			 this->InternalGivingSoldierCancelServices( FALSE );

			}

		}
	}
	else
	{
		DebugAttackBusy( String( "Soldier %d tried to climb where no roof is.\n", this->ubID ) );
		this->aiData.bAction = AI_ACTION_NONE;
	}
}

void SOLDIERTYPE::BeginSoldierClimbFence( void )
{
	INT8							bDirection;

	// Make sure we hop the correct fence to follow our path!
	if (this->pathing.usPathIndex < this->pathing.usPathDataSize)
	{
		bDirection = (INT8) this->pathing.usPathingData[ this->pathing.usPathIndex];
	}
	else
	{
		bDirection = this->ubDirection;
	}

	if ( FindFenceJumpDirection( this, this->sGridNo, bDirection, &bDirection ) )
	{
		this->sTempNewGridNo = NewGridNo( (INT16)this->sGridNo, (UINT16)DirectionInc(bDirection ) );
		this->flags.fDontChargeTurningAPs = TRUE;
		EVENT_InternalSetSoldierDesiredDirection( this, bDirection, FALSE, this->usAnimState );
		this->flags.fTurningUntilDone = TRUE;
		// ATE: Reset flag to go back to prone...
		this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
		this->usPendingAnimation = HOPFENCE;
	}

}

UINT32 SleepDartSuccumbChance( SOLDIERTYPE * pSoldier )
{
	UINT32		uiChance;
	INT8			bEffectiveStrength;

	// figure out base chance of succumbing,
	bEffectiveStrength = EffectiveStrength( pSoldier );

	if (bEffectiveStrength > 90)
	{
		uiChance = 110 - bEffectiveStrength;
	}
	else if (bEffectiveStrength > 80)
	{
		uiChance = 120 - bEffectiveStrength;
	}
	else if (bEffectiveStrength > 70)
	{
		uiChance = 130 - bEffectiveStrength;
	}
	else
	{
		uiChance = 140 - bEffectiveStrength;
	}

	// add in a bonus based on how long it's been since shot... highest chance at the beginning
	uiChance += (10 - pSoldier->bSleepDrugCounter);

	return( uiChance );
}

void SOLDIERTYPE::BeginSoldierGetup( void )
{
	// RETURN IF WE ARE BEING SERVICED
	if ( this->ubServiceCount > 0 )
	{
		return;
	}

	// ATE: Don't getup if we are in a meanwhile
	if ( AreInMeanwhile( ) )
	{
		return;
	}

	if ( this->bCollapsed )
	{
		if ( this->stats.bLife >= OKLIFE && this->bBreath >= OKBREATH && (this->bSleepDrugCounter == 0) )
		{
			// get up you hoser!

			this->bCollapsed = FALSE;
			this->bTurnsCollapsed = 0;

			if ( IS_MERC_BODY_TYPE( this ) )
			{
				switch( this->usAnimState )
				{
				case FALLOFF_FORWARD_STOP:
				case PRONE_LAYFROMHIT_STOP:
				case STAND_FALLFORWARD_STOP:
					this->ChangeSoldierStance( ANIM_CROUCH );
					break;

				case FALLBACKHIT_STOP:
				case FALLOFF_STOP:
				case FLYBACKHIT_STOP:
				case FALLBACK_HIT_STAND:
				case FALLOFF:
				case FLYBACK_HIT:

					// ROLL OVER
					this->EVENT_InitNewSoldierAnim( ROLLOVER, 0 , FALSE );
					break;

				default:

					this->ChangeSoldierStance( ANIM_CROUCH );
					break;
				}
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( END_COWER, 0 , FALSE );
			}
		}
		else
		{
			this->bTurnsCollapsed++;
			if ( (gTacticalStatus.bBoxingState == BOXING) && (this->flags.uiStatusFlags & SOLDIER_BOXER) )
			{
				if (this->bTurnsCollapsed > 1)
				{
					// We have a winnah!  But it isn't this boxer!
					EndBoxingMatch( this );
				}
			}
		}
	}
	else if ( this->bSleepDrugCounter > 0 )
	{
		UINT32 uiChance;

		uiChance = SleepDartSuccumbChance( this );

		if ( PreRandom( 100 ) < uiChance )
		{
			// succumb to the drug!
			DeductPoints( this, 0, (INT16)( this->bBreathMax * 100 ) );
			SoldierCollapse( this );
		}
	}

	if ( this->bSleepDrugCounter > 0 )
	{
		this->bSleepDrugCounter--;
	}
}


void HandleTakeDamageDeath( SOLDIERTYPE *pSoldier, UINT8 bOldLife, UINT8 ubReason )
{
	switch( ubReason )
	{
	case TAKE_DAMAGE_BLOODLOSS:
	case TAKE_DAMAGE_ELECTRICITY:
	case TAKE_DAMAGE_GAS:

		if ( pSoldier->bInSector )
		{
			if ( pSoldier->bVisible != -1 )
			{
				if ( ubReason != TAKE_DAMAGE_BLOODLOSS )
				{
					pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
					pSoldier->flags.fDeadSoundPlayed = TRUE;
				}
			}

			if ( ( ubReason == TAKE_DAMAGE_ELECTRICITY ) && pSoldier->stats.bLife < OKLIFE )
			{
				pSoldier->flags.fInNonintAnim = FALSE;
			}

			// Check for < OKLIFE
			if ( pSoldier->stats.bLife < OKLIFE && pSoldier->stats.bLife != 0 && !pSoldier->bCollapsed)
			{
				SoldierCollapse( pSoldier );
			}

			// THis is for the die animation that will be happening....
			if ( pSoldier->stats.bLife == 0 )
			{
				pSoldier->flags.fDoingExternalDeath = TRUE;
			}

			// Check if he is dead....
			CheckForAndHandleSoldierDyingNotFromHit( pSoldier );

		}

		//if( !( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
		{
			pSoldier->HandleSoldierTakeDamageFeedback( );
		}

		if(( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) || !pSoldier->bInSector )
		{
			if ( pSoldier->stats.bLife == 0 && !( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
			{
				StrategicHandlePlayerTeamMercDeath( pSoldier );

				// ATE: Here, force always to use die sound...
				pSoldier->flags.fDieSoundUsed = FALSE;
				pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
				pSoldier->flags.fDeadSoundPlayed = TRUE;

				// ATE: DO death sound
				PlayJA2Sample( (UINT8)DOORCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				PlayJA2Sample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			}
		}
		break;
	}

	// 0verhaul:  This is also already handled by the animation transitions
	// if ( ubReason == TAKE_DAMAGE_ELECTRICITY )
	// {
	//	if ( pSoldier->stats.bLife >= OKLIFE )
	//	{
	//		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Freeing up attacker from electricity damage") );
	//		ReleaseSoldiersAttacker( pSoldier );
	//	}
	// }
}


UINT8 SOLDIERTYPE::SoldierTakeDamage( INT8 bHeight, INT16 sLifeDeduct, INT16 sBreathLoss, UINT8 ubReason, UINT8 ubAttacker, INT16 sSourceGrid, INT16 sSubsequent, BOOLEAN fShowDamage )
{
	INT8		bOldLife;
	UINT8		ubCombinedLoss;
	INT8		bBandage;
	INT16		sAPCost;
	UINT8		ubBlood;
	UINT16		usItemFlags=0; // Kaiden: Needed for the reveal all items after combat code from UB.

	this->ubLastDamageReason = ubReason;


	// CJC Jan 21 99: add check to see if we are hurting an enemy in an enemy-controlled
	// sector; if so, this is a sign of player activity
	switch ( this->bTeam )
	{
	case ENEMY_TEAM:
		// if we're in the wilderness this always counts
		if ( StrategicMap[ CALCULATE_STRATEGIC_INDEX( gWorldSectorX, gWorldSectorY ) ].fEnemyControlled || SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] != TOWN )
		{
			// update current day of activity!
			UpdateLastDayOfPlayerActivity( (UINT16) GetWorldDay() );
		}
		break;
	case CREATURE_TEAM:
		// always a sign of activity?
		UpdateLastDayOfPlayerActivity( (UINT16) GetWorldDay() );
		break;
	case CIV_TEAM:
		if ( this->ubCivilianGroup == KINGPIN_CIV_GROUP && gubQuest[ QUEST_RESCUE_MARIA ] == QUESTINPROGRESS && gTacticalStatus.bBoxingState == NOT_BOXING )
		{
			SOLDIERTYPE * pMaria = FindSoldierByProfileID( MARIA, FALSE );
			if ( pMaria && pMaria->bActive && pMaria->bInSector )
			{
				SetFactTrue( FACT_MARIA_ESCAPE_NOTICED );
			}
		}
		break;
	default:
		break;
	}

	// Deduct life!, Show damage if we want!
	bOldLife = this->stats.bLife;

	// OK, If we are a vehicle.... damage vehicle...( people inside... )
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		if ( TANK( this ) )
		{
			//sLifeDeduct = (sLifeDeduct * 2) / 3;
		}
		else
		{
			if ( ubReason == TAKE_DAMAGE_GUNFIRE )
			{
				sLifeDeduct /= 3;
			}
			else if ( ubReason == TAKE_DAMAGE_EXPLOSION && sLifeDeduct > 50 )
			{
				// boom!
				sLifeDeduct *= 2;
			}
		}

		VehicleTakeDamage( this->bVehicleID, ubReason, sLifeDeduct, this->sGridNo, ubAttacker );
		HandleTakeDamageDeath( this, bOldLife, ubReason );
		return( 0 );
	}

	// ATE: If we are elloit being attacked in a meanwhile...
	if ( this->flags.uiStatusFlags & SOLDIER_NPC_SHOOTING )
	{
		// Almost kill but not quite.....
		sLifeDeduct = ( this->stats.bLife - 1 );
		// Turn off
		this->flags.uiStatusFlags &= ( ~SOLDIER_NPC_SHOOTING );
	}

	// CJC: make sure Elliot doesn't bleed to death!
	if ( ubReason == TAKE_DAMAGE_BLOODLOSS && AreInMeanwhile() )
	{
		return( 0 );
	}


	// Calculate bandage
	bBandage = this->stats.bLifeMax - this->stats.bLife - this->bBleeding;

	if( guiCurrentScreen == MAP_SCREEN )
	{
		fReDrawFace = TRUE;
	}

	if ( CREATURE_OR_BLOODCAT( this ) )
	{
		INT16 sReductionFactor = 0;

		if ( this->ubBodyType == BLOODCAT )
		{
			sReductionFactor = 2;
		}
		else if (this->flags.uiStatusFlags & SOLDIER_MONSTER)
		{
			switch( this->ubBodyType )
			{
			case LARVAE_MONSTER:
			case INFANT_MONSTER:
				sReductionFactor = 1;
				break;
			case YAF_MONSTER:
			case YAM_MONSTER:
				sReductionFactor = 4;
				break;
			case ADULTFEMALEMONSTER:
			case AM_MONSTER:
				sReductionFactor = 6;
				break;
			case QUEENMONSTER:
				// increase with range!
				if ( ubAttacker == NOBODY )
				{
					sReductionFactor = 8;
				}
				else
				{
					sReductionFactor = 4 + PythSpacesAway( MercPtrs[ ubAttacker ]->sGridNo, this->sGridNo ) / 2;
				}
				break;
			}
		}

		if ( ubReason == TAKE_DAMAGE_EXPLOSION )
		{
			sReductionFactor /= 4;
		}
		if ( sReductionFactor > 1 )
		{
			sLifeDeduct = (sLifeDeduct + (sReductionFactor / 2 ) ) / sReductionFactor;
		}
		else if (	ubReason == TAKE_DAMAGE_EXPLOSION )
		{
			// take at most 2/3rds
			sLifeDeduct = (sLifeDeduct * 2) / 3;
		}

		// reduce breath loss to a smaller degree, except for the queen...
		if ( this->ubBodyType == QUEENMONSTER )
		{
			// in fact, reduce breath loss by MORE!
			sReductionFactor = __min( sReductionFactor, 8 );
			sReductionFactor *= 2;
		}
		else
		{
			sReductionFactor /= 2;
		}
		if ( sReductionFactor > 1 )
		{
			sBreathLoss = (sBreathLoss + (sReductionFactor / 2 ) ) / sReductionFactor;
		}
	}

	if (sLifeDeduct > this->stats.bLife)
	{
		this->stats.bLife = 0;
	}
	else
	{
		// Decrease Health
		this->stats.bLife -= sLifeDeduct;
	}

	// ATE: Put some logic in here to allow enemies to die quicker.....
	// Are we an enemy?
	if ( this->bSide != gbPlayerNum && !this->aiData.bNeutral && this->ubProfile == NO_PROFILE )
	{
		// ATE: Give them a chance to fall down...
		if ( this->stats.bLife > 0 && this->stats.bLife < ( OKLIFE - 1 ) )
		{
			// Are we taking damage from bleeding?
			if ( ubReason == TAKE_DAMAGE_BLOODLOSS )
			{
				// Fifty-fifty chance to die now!
				if ( Random( 2 ) == 0 || gTacticalStatus.Team[ this->bTeam ].bMenInSector == 1 )
				{
					// Kill!
					this->stats.bLife = 0;
				}
			}
			else
			{
				// OK, see how far we are..
				if ( this->stats.bLife < ( OKLIFE - 3 ) )
				{
					// Kill!
					this->stats.bLife = 0;
				}
			}
		}
	}

	if ( fShowDamage )
	{
		this->sDamage += sLifeDeduct;
	}

	// Truncate life
	if ( this->stats.bLife < 0 )
	{
		this->stats.bLife = 0;
	}


	// Calculate damage to our items if from an explosion!
	if ( ubReason == TAKE_DAMAGE_EXPLOSION || ubReason == TAKE_DAMAGE_STRUCTURE_EXPLOSION)
	{
		CheckEquipmentForDamage( this, sLifeDeduct );
	}



	// Calculate bleeding
	if ( ubReason != TAKE_DAMAGE_GAS && !AM_A_ROBOT( this ) )
	{
		if ( ubReason == TAKE_DAMAGE_HANDTOHAND  )
		{
			if ( sLifeDeduct > 0 )
			{
				// HTH does 1 pt bleeding per hit
				this->bBleeding = this->bBleeding + 1;
			}
		}
		else
		{
			this->bBleeding = this->stats.bLifeMax - ( this->stats.bLife + bBandage );
		}

	}

	// Deduct breath AND APs!
	sAPCost = (sLifeDeduct / AP_GET_WOUNDED_DIVISOR); // + fallCost;

	// ATE: if the robot, do not deduct
	if ( !AM_A_ROBOT( this ) )
	{
		DeductPoints( this, sAPCost, sBreathLoss , FALSE);
	}

	ubCombinedLoss = (UINT8) sLifeDeduct / 10 + sBreathLoss / 2000;

	// Add shock
	if ( !AM_A_ROBOT( this ) )
	{
		this->aiData.bShock += ubCombinedLoss;
	}

	// start the stopwatch - the blood is gushing!
	this->dNextBleed = CalcSoldierNextBleed( this );

	if ( this->bInSector && this->bVisible != -1 )
	{
		// If we are already dead, don't show damage!
		if ( bOldLife != 0 && fShowDamage && sLifeDeduct != 0 && sLifeDeduct < 1000 )
		{
			// Display damage
			INT16 sOffsetX, sOffsetY;

			// Set Damage display counter
			this->flags.fDisplayDamage = TRUE;
			this->bDisplayDamageCount = 0;

			if ( this->ubBodyType == QUEENMONSTER )
			{
				this->sDamageX = 0;
				this->sDamageY = 0;
			}
			else
			{
				GetSoldierAnimOffsets( this, &sOffsetX, &sOffsetY );
				this->sDamageX = sOffsetX;
				this->sDamageY = sOffsetY;
			}
		}
	}

	// OK, if here, let's see if we should drop our weapon....
	if ( ubReason != TAKE_DAMAGE_BLOODLOSS && !(AM_A_ROBOT( this )) )
	{
		INT16 sTestOne, sTestTwo, sChanceToDrop;
		INT8	bVisible = -1;

		sTestOne = EffectiveStrength( this );
		sTestTwo = ( 2 * ( __max( sLifeDeduct, ( sBreathLoss / 100 ) ) ) );


		if (this->ubAttackerID != NOBODY && MercPtrs[ this->ubAttackerID ]->ubBodyType == BLOODCAT )
		{
			// bloodcat boost, let them make people drop items more
			sTestTwo += 20;
		}

		// If damage > effective strength....
		sChanceToDrop = ( __max( 0, ( sTestTwo - sTestOne ) ) );

		// ATE: Increase odds of NOT dropping an UNDROPPABLE OBJECT
		if ( ( this->inv[ HANDPOS ].fFlags & OBJECT_UNDROPPABLE ) )
		{
			sChanceToDrop -= 30;
		}

#ifdef JA2TESTVERSION
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Chance To Drop Weapon: str: %d Dam: %d Chance: %d", sTestOne, sTestTwo, sChanceToDrop );
#endif

		if ( Random( 100 ) < (UINT16) sChanceToDrop )
		{
			// OK, drop item in main hand...
			if ( this->inv[ HANDPOS ].exists() == true )
			{
				if ( !( this->inv[ HANDPOS ].fFlags & OBJECT_UNDROPPABLE ) )
				{
					// ATE: if our guy, make visible....
					if ( this->bTeam == gbPlayerNum )
					{
						bVisible = 1;
					}
					//if this soldier was an enemy
					// Kaiden Added for UB reveal All items after combat feature!
					else if( this->bTeam == ENEMY_TEAM )
					{
						//add a flag to the item so when all enemies are killed, we can run through and reveal all the enemies items
						usItemFlags |= WORLD_ITEM_DROPPED_FROM_ENEMY;
					}
					AddItemToPool( this->sGridNo, &(this->inv[ HANDPOS ]), bVisible, this->pathing.bLevel, usItemFlags, -1 ); //Madd: added usItemFlags to function arguments
					DeleteObj( &(this->inv[HANDPOS]) );
				}
			}
		}
	}

	// Drop some blood!
	// decide blood amt, if any
	ubBlood = ( sLifeDeduct / BLOODDIVISOR);
	if ( ubBlood > MAXBLOODQUANTITY )
	{
		ubBlood = MAXBLOODQUANTITY;
	}

	if ( !( this->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_ROBOT ) ) )
	{
		if ( ubBlood != 0 )
		{
			if ( this->bInSector )
			{
				DropBlood( this, ubBlood, this->bVisible );
			}
		}
	}

	//Set UI Flag for unconscious, if it's our own guy!
	if ( this->bTeam == gbPlayerNum  )
	{
		if ( this->stats.bLife < OKLIFE && this->stats.bLife > 0 && bOldLife >= OKLIFE )
		{
			this->flags.fUIFirstTimeUNCON = TRUE;
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
	}

	if ( this->bInSector )
	{
		this->CheckForBreathCollapse( );
	}

	// EXPERIENCE CLASS GAIN (combLoss): Getting wounded in battle

	DirtyMercPanelInterface( this, DIRTYLEVEL1 );


	if ( ubAttacker != NOBODY )
	{
		// don't give exp for hitting friends!
		if ( (MercPtrs[ ubAttacker ]->bTeam == gbPlayerNum) && (this->bTeam != gbPlayerNum) )
		{
			if ( ubReason == TAKE_DAMAGE_EXPLOSION )
			{
				// EXPLOSIVES GAIN (combLoss):  Causing wounds in battle
				StatChange( MercPtrs[ ubAttacker ], EXPLODEAMT, (UINT16)( 10 * ubCombinedLoss ), FROM_FAILURE );
			}
			/*
			else if ( ubReason == TAKE_DAMAGE_GUNFIRE )
			{
			// MARKSMANSHIP GAIN (combLoss):  Causing wounds in battle
			StatChange( MercPtrs[ ubAttacker ], MARKAMT, (UINT16)( 5 * ubCombinedLoss ), FALSE );
			}
			*/
		}
	}


	SOLDIERTYPE *pSoldier = this;
	if (PTR_OURTEAM)
	{
		// EXPERIENCE GAIN: Took some damage
		StatChange( this, EXPERAMT, ( UINT16 )( 5 * ubCombinedLoss ), FROM_FAILURE );

		// Check for quote
		if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_BEING_PUMMELED ) )
		{
			// Check attacker!
			if ( ubAttacker != NOBODY && ubAttacker != this->ubID )
			{
				this->bNumHitsThisTurn++;

				if ( (this->bNumHitsThisTurn >= 3) && ( this->stats.bLife - this->bOldLife > 20 ) )
				{
					if ( Random(100) < (UINT16)((40 * ( this->bNumHitsThisTurn - 2))))
					{
						DelayedTacticalCharacterDialogue( this, QUOTE_TAKEN_A_BREATING );
						this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_BEING_PUMMELED;
						this->bNumHitsThisTurn = 0;
					}
				}
			}
		}
	}

	if ((ubAttacker != NOBODY) && (Menptr[ubAttacker].bTeam == OUR_TEAM) && (this->ubProfile != NO_PROFILE) && (this->ubProfile >= FIRST_RPC && this->ubProfile < GASTON ))
	{
		gMercProfiles[this->ubProfile].ubMiscFlags |= PROFILE_MISC_FLAG_WOUNDEDBYPLAYER;
		if (this->ubProfile == 114)
		{
			SetFactTrue( FACT_PACOS_KILLED );
		}
	}

	HandleTakeDamageDeath( this, bOldLife, ubReason );

	// Check if we are < unconscious, and shutup if so! also wipe sight
	if ( this->stats.bLife < CONSCIOUSNESS )
	{
		ShutupaYoFace( this->iFaceIndex );
	}

	if ( this->stats.bLife < OKLIFE )
	{
		DecayIndividualOpplist( this );
	}


	return( ubCombinedLoss );
}


extern BOOLEAN IsMercSayingDialogue( UINT8 ubProfileID );


BOOLEAN SOLDIERTYPE::InternalDoMercBattleSound( UINT8 ubBattleSoundID, INT8 bSpecialCode )
{
	//in this function, pSoldier stands in for the this pointer, since
	//this soldier could be a vehicle, then the merc that makes the sound is inside
	SGPFILENAME		zFilename;
	SOUNDPARMS		spParms;
	UINT8					ubSoundID;
	UINT32				uiSoundID;
	UINT32				iFaceIndex;
	BOOLEAN				fDoSub = FALSE;
	INT32					uiSubSoundID = 0;
	BOOLEAN				fSpeechSound = FALSE;
	SOLDIERTYPE*		pSoldier = this;

	// DOUBLECHECK RANGE
	CHECKF ( ubBattleSoundID < NUM_MERC_BATTLE_SOUNDS );

	if ( ( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		// Pick a passenger from vehicle....
		pSoldier = PickRandomPassengerFromVehicle( this );

		if ( pSoldier == NULL )
		{
			return( FALSE );
		}

	}

	// If a death sound, and we have already done ours...
	if ( ubBattleSoundID == BATTLE_SOUND_DIE1 )
	{
		if ( pSoldier->flags.fDieSoundUsed )
		{
			return( TRUE );
		}
	}


	// Are we mute?
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_MUTE )
	{
		return( FALSE );
	}


	//	uiTimeSameBattleSndDone

	// If we are a creature, etc, pick a better sound...
	if ( ubBattleSoundID == BATTLE_SOUND_HIT1 || ubBattleSoundID == BATTLE_SOUND_HIT2 )
	{
		switch ( pSoldier->ubBodyType )
		{
		case COW:

			fDoSub = TRUE;
			uiSubSoundID = COW_HIT_SND;
			break;

		case YAF_MONSTER:
		case YAM_MONSTER:
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:

			fDoSub = TRUE;

			if ( Random( 2 ) == 0 )
			{
				uiSubSoundID = ACR_DIE_PART1;
			}
			else
			{
				uiSubSoundID = ACR_LUNGE;
			}
			break;

		case INFANT_MONSTER:

			fDoSub = TRUE;
			uiSubSoundID = BCR_SHRIEK;
			break;

		case QUEENMONSTER:

			fDoSub = TRUE;
			uiSubSoundID = LQ_SHRIEK;
			break;

		case LARVAE_MONSTER:

			fDoSub = TRUE;
			uiSubSoundID = BCR_SHRIEK;
			break;

		case BLOODCAT:

			fDoSub = TRUE;
			uiSubSoundID = BLOODCAT_HIT_1;
			break;

		case ROBOTNOWEAPON:

			fDoSub = TRUE;
			uiSubSoundID = (UINT32)( S_METAL_IMPACT1 + Random( 2 ) );
			break;
		}
	}

	if ( ubBattleSoundID == BATTLE_SOUND_DIE1 )
	{
		switch ( pSoldier->ubBodyType )
		{
		case COW:

			fDoSub = TRUE;
			uiSubSoundID = COW_DIE_SND;
			break;

		case YAF_MONSTER:
		case YAM_MONSTER:
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:

			fDoSub = TRUE;
			uiSubSoundID = CREATURE_FALL_PART_2;
			break;

		case INFANT_MONSTER:

			fDoSub = TRUE;
			uiSubSoundID = BCR_DYING;
			break;

		case LARVAE_MONSTER:

			fDoSub = TRUE;
			uiSubSoundID = LCR_RUPTURE;
			break;

		case QUEENMONSTER:

			fDoSub = TRUE;
			uiSubSoundID = LQ_DYING;
			break;

		case BLOODCAT:

			fDoSub = TRUE;
			uiSubSoundID = BLOODCAT_DIE_1;
			break;

		case ROBOTNOWEAPON:

			fDoSub = TRUE;
			uiSubSoundID = (UINT32)( EXPLOSION_1 );
			PlayJA2Sample( ROBOT_DEATH, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			break;

		}
	}

	// OK. any other sound, not hits, robot makes a beep
	if ( pSoldier->ubBodyType == ROBOTNOWEAPON && !fDoSub )
	{
		fDoSub = TRUE;
		if ( ubBattleSoundID == BATTLE_SOUND_ATTN1 )
		{
			uiSubSoundID = ROBOT_GREETING;
		}
		else
		{
			uiSubSoundID = ROBOT_BEEP;
		}
	}

	if ( fDoSub )
	{
		if( guiCurrentScreen != GAME_SCREEN )
		{
			PlayJA2Sample( uiSubSoundID, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}
		else
		{
			PlayJA2Sample( uiSubSoundID, RATE_11025, SoundVolume( (UINT8)CalculateSpeechVolume( HIGHVOLUME ), pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		}
		return( TRUE );
	}

	// Check if this is the same one we just played...
	if ( pSoldier->bOldBattleSnd == ubBattleSoundID && gBattleSndsData[ ubBattleSoundID ].fDontAllowTwoInRow )
	{
		// Are we below the min delay?
		if ( ( GetJA2Clock( ) - pSoldier->uiTimeSameBattleSndDone ) < MIN_SUBSEQUENT_SNDS_DELAY )
		{
			return( TRUE );
		}
	}

	// If a battle snd is STILL playing....
	if ( SoundIsPlaying( pSoldier->uiBattleSoundID ) )
	{
		// We can do a few things here....
		// Is this a crutial one...?
		if ( gBattleSndsData[ ubBattleSoundID ].fStopDialogue == 1 )
		{
			// Stop playing origonal
			SoundStop( pSoldier->uiBattleSoundID );
		}
		else
		{
			// Skip this one...
			return( TRUE );
		}
	}

	// If we are talking now....
	if ( IsMercSayingDialogue( pSoldier->ubProfile ) )
	{
		// We can do a couple of things now...
		if ( gBattleSndsData[ ubBattleSoundID ].fStopDialogue == 1 )
		{
			// Stop dialigue...
			DialogueAdvanceSpeech( );
		}
		else if ( gBattleSndsData[ ubBattleSoundID ].fStopDialogue == 2 )
		{
			// Skip battle snd...
			return( TRUE );
		}
	}


	// Save this one we're doing...
	pSoldier->bOldBattleSnd						 = ubBattleSoundID;
	pSoldier->uiTimeSameBattleSndDone	 = GetJA2Clock( );


	// Adjust based on morale...
	if ( ubBattleSoundID == BATTLE_SOUND_OK1 && pSoldier->aiData.bMorale < LOW_MORALE_BATTLE_SND_THREASHOLD )
	{
		ubBattleSoundID = BATTLE_SOUND_LOWMARALE_OK1;
	}
	if ( ubBattleSoundID == BATTLE_SOUND_ATTN1 && pSoldier->aiData.bMorale < LOW_MORALE_BATTLE_SND_THREASHOLD )
	{
		ubBattleSoundID = BATTLE_SOUND_LOWMARALE_ATTN1;
	}

	ubSoundID = ubBattleSoundID;

	//if the sound to be played is a confirmation, check to see if we are to play it
	if( ubSoundID == BATTLE_SOUND_OK1 )
	{
		if( gGameSettings.fOptions[ TOPTION_MUTE_CONFIRMATIONS ] )
			return( TRUE );
		//else a speech sound is to be played
		else
			fSpeechSound = TRUE;
	}

	// Randomize between sounds, if appropriate
	if ( gBattleSndsData[ ubSoundID ].ubRandomVal != 0 )
	{
		ubSoundID = ubSoundID + (UINT8)Random( gBattleSndsData[ ubSoundID ].ubRandomVal );

	}


	// OK, build file and play!
	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		sprintf( zFilename, "BATTLESNDS\\%03d_%s.ogg", pSoldier->ubProfile, gBattleSndsData[ ubSoundID ].zName );

		if ( !FileExists( zFilename ) )
		{
			sprintf( zFilename, "BATTLESNDS\\%03d_%s.wav", pSoldier->ubProfile, gBattleSndsData[ ubSoundID ].zName );
		}

		if ( !FileExists( zFilename ) )
		{
			// OK, temp build file...
			if ( pSoldier->ubBodyType == REGFEMALE )
			{
				sprintf( zFilename, "BATTLESNDS\\f_%s.ogg", gBattleSndsData[ ubSoundID ].zName );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\f_%s.wav", gBattleSndsData[ ubSoundID ].zName );
				}
			}
			else
			{
				sprintf( zFilename, "BATTLESNDS\\m_%s.ogg", gBattleSndsData[ ubSoundID ].zName );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\m_%s.wav", gBattleSndsData[ ubSoundID ].zName );
				}
			}
		}
	}
	else
	{
		// Check if we can play this!
		if ( !gBattleSndsData[ ubSoundID ].fBadGuy )
		{
			return( FALSE );
		}

		if ( pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV )
		{
			if ( ubSoundID == BATTLE_SOUND_DIE1 )
			{
				sprintf( zFilename, "BATTLESNDS\\kid%d_dying.ogg", pSoldier->ubBattleSoundID );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\kid%d_dying.wav", pSoldier->ubBattleSoundID );
				}
			}
			else
			{
				sprintf( zFilename, "BATTLESNDS\\kid%d_%s.ogg", pSoldier->ubBattleSoundID, gBattleSndsData[ ubSoundID ].zName );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\kid%d_%s.wav", pSoldier->ubBattleSoundID, gBattleSndsData[ ubSoundID ].zName );
				}
			}
		}
		else
		{
			if ( ubSoundID == BATTLE_SOUND_DIE1 )
			{
				sprintf( zFilename, "BATTLESNDS\\bad%d_die.ogg", pSoldier->ubBattleSoundID );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\bad%d_die.wav", pSoldier->ubBattleSoundID );
				}
			}
			else
			{
				sprintf( zFilename, "BATTLESNDS\\bad%d_%s.ogg", pSoldier->ubBattleSoundID, gBattleSndsData[ ubSoundID ].zName );
				if ( !FileExists( zFilename ) )
				{
					sprintf( zFilename, "BATTLESNDS\\bad%d_%s.wav", pSoldier->ubBattleSoundID, gBattleSndsData[ ubSoundID ].zName );
				}
			}
		}
	}

	// Play sound!
	memset(&spParms, 0xff, sizeof(SOUNDPARMS));

	spParms.uiSpeed = RATE_11025;
	//spParms.uiVolume = CalculateSpeechVolume( pSoldier->bVocalVolume );

	spParms.uiVolume = (INT8)CalculateSpeechVolume( HIGHVOLUME );

	// ATE: Reduce volume for OK sounds...
	// ( Only for all-moves or multi-selection cases... )
	if ( bSpecialCode == BATTLE_SND_LOWER_VOLUME )
	{
		spParms.uiVolume = (INT8)CalculateSpeechVolume( MIDVOLUME );
	}

	// If we are an enemy.....reduce due to volume
	if ( pSoldier->bTeam != gbPlayerNum )
	{
		spParms.uiVolume = SoundVolume( (UINT8)spParms.uiVolume, pSoldier->sGridNo );
	}

	spParms.uiLoop = 1;
	spParms.uiPan = SoundDir( pSoldier->sGridNo );
	spParms.uiPriority=GROUP_PLAYER;

	if ( ( uiSoundID = SoundPlay( zFilename, &spParms ) ) == SOUND_ERROR )
	{
		return( FALSE );
	}
	else
	{
		pSoldier->uiBattleSoundID = uiSoundID;

		if ( pSoldier->ubProfile != NO_PROFILE )
		{
			// Get soldier's face ID
			iFaceIndex = pSoldier->iFaceIndex;

			// Check face index
			if ( iFaceIndex != -1 )
			{
				ExternSetFaceTalking( iFaceIndex, uiSoundID );
			}
		}

		return( TRUE );
	}
}

BOOLEAN SOLDIERTYPE::DoMercBattleSound( UINT8 ubBattleSoundID )
{
	// We WANT to play some RIGHT AWAY.....
	if ( gBattleSndsData[ ubBattleSoundID ].fStopDialogue == 1 || ( this->ubProfile == NO_PROFILE ) || InOverheadMap( ) )
	{
		return( this->InternalDoMercBattleSound( ubBattleSoundID, 0 ) );
	}

	// So here, only if we were currently saying dialogue.....
	if ( !IsMercSayingDialogue( this->ubProfile ) )
	{
		return( this->InternalDoMercBattleSound( ubBattleSoundID, 0 ) );
	}

	// OK, queue it up otherwise!
	TacticalCharacterDialogueWithSpecialEvent( this, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, ubBattleSoundID,0 );

	return( TRUE );
}


BOOLEAN PreloadSoldierBattleSounds( SOLDIERTYPE *pSoldier, BOOLEAN fRemove )
{
	UINT32 cnt;

	CHECKF( pSoldier->bActive != FALSE );

	for ( cnt = 0; cnt < NUM_MERC_BATTLE_SOUNDS; cnt++ )
	{
		// OK, build file and play!
		if ( pSoldier->ubProfile != NO_PROFILE )
		{
			if ( gBattleSndsData[ cnt ].fPreload )
			{
				if ( fRemove )
				{
					SoundUnlockSample( gBattleSndsData[ cnt ].zName );
				}
				else
				{
					SoundLockSample( gBattleSndsData[ cnt ].zName );
				}
			}
		}
		else
		{
			if ( gBattleSndsData[ cnt ].fPreload && gBattleSndsData[ cnt ].fBadGuy )
			{
				if ( fRemove )
				{
					SoundUnlockSample( gBattleSndsData[ cnt ].zName );
				}
				else
				{
					SoundLockSample( gBattleSndsData[ cnt ].zName );
				}
			}
		}
	}

	return( TRUE );
}



BOOLEAN SOLDIERTYPE::CheckSoldierHitRoof( void )
{
	// Check if we are near a lower level
	INT8							bNewDirection;
	BOOLEAN						fReturnVal = FALSE;
	INT16							sNewGridNo;
	// Default to true
	BOOLEAN						fDoForwards = TRUE;

	if ( this->stats.bLife >= OKLIFE )
	{
		return( FALSE );
	}

	if ( FindLowerLevel( this, this->sGridNo, this->ubDirection, &bNewDirection ) && ( this->pathing.bLevel > 0 ) )
	{
		// ONly if standing!
		if ( gAnimControl[ this->usAnimState ].ubHeight == ANIM_STAND )
		{
			// We are near a lower level.
			// Use opposite direction
			bNewDirection = gOppositeDirection[ bNewDirection ];

			// Alrighty, let's not blindly change here, look at whether the dest gridno is good!
			sNewGridNo = NewGridNo( (INT16)this->sGridNo, DirectionInc( gOppositeDirection[ bNewDirection ] ) );
			if ( !NewOKDestination( this, sNewGridNo, TRUE, 0 ) )
			{
				return( FALSE );
			}
			sNewGridNo = NewGridNo( (INT16)sNewGridNo, DirectionInc( gOppositeDirection[ bNewDirection ] ) );
			if ( !NewOKDestination( this, sNewGridNo, TRUE, 0 ) )
			{
				return( FALSE );
			}

			// Are wee near enough to fall forwards....
			if ( this->ubDirection == gOneCDirection[ bNewDirection ] ||
				this->ubDirection == gTwoCDirection[ bNewDirection ] ||
				this->ubDirection == bNewDirection ||
				this->ubDirection == gOneCCDirection[ bNewDirection ] ||
				this->ubDirection == gTwoCCDirection[ bNewDirection ] )
			{
				// Do backwards...
				fDoForwards = FALSE;
			}

			// If we are facing the opposite direction, fall backwards
			// ATE: Make this more usefull...
			if ( fDoForwards )
			{
				this->sTempNewGridNo = NewGridNo( (INT16)this->sGridNo, (INT16)( -1 * DirectionInc(bNewDirection ) ) );
				this->sTempNewGridNo = NewGridNo( (INT16)this->sTempNewGridNo, (INT16)( -1 * DirectionInc( bNewDirection ) ) );
				this->EVENT_SetSoldierDesiredDirection( gOppositeDirection[ bNewDirection ] );
				this->flags.fTurningUntilDone = TRUE;
				this->usPendingAnimation = FALLFORWARD_ROOF;
				//this->EVENT_InitNewSoldierAnim( FALLFORWARD_ROOF, 0 , FALSE );

				// Deduct hitpoints/breath for falling!
				this->SoldierTakeDamage( ANIM_CROUCH, 100, 5000, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );

				fReturnVal = TRUE;

			}
			else
			{

				this->sTempNewGridNo = NewGridNo( (INT16)this->sGridNo, (INT16)( -1 * DirectionInc( bNewDirection ) ) );
				this->sTempNewGridNo = NewGridNo( (INT16)this->sTempNewGridNo, (INT16)( -1 * DirectionInc( bNewDirection ) ) );
				this->EVENT_SetSoldierDesiredDirection( bNewDirection );
				this->flags.fTurningUntilDone = TRUE;
				this->usPendingAnimation = FALLOFF;

				// Deduct hitpoints/breath for falling!
				this->SoldierTakeDamage( ANIM_CROUCH, 100, 5000, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );

				fReturnVal = TRUE;
			}
		}
	}

	return( fReturnVal );
}

void SOLDIERTYPE::BeginSoldierClimbDownRoof( void )
{
	INT8							bNewDirection;
	UINT8	ubWhoIsThere;


	if ( FindLowerLevel( this, this->sGridNo, this->ubDirection, &bNewDirection ) && ( this->pathing.bLevel > 0 ) )
	{
		if ( EnoughPoints( this, GetAPsToClimbRoof( this, TRUE ), 0, TRUE ) )
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2( NewGridNo( (INT16)this->sGridNo, (UINT16)DirectionInc(bNewDirection ) ), 0 );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID )
		 {
			 return;
		 }
			else
		 {

			 if (this->bTeam == gbPlayerNum)
			 {
				 // OK, SET INTERFACE FIRST
				 SetUIBusy( this->ubID );
			 }




			 this->sTempNewGridNo = NewGridNo( (INT16)this->sGridNo, (UINT16)DirectionInc(bNewDirection ) );

			 bNewDirection = gTwoCDirection[ bNewDirection ];

			 this->ubPendingDirection = bNewDirection;
			 this->EVENT_InitNewSoldierAnim( CLIMBDOWNROOF, 0 , FALSE );

			 this->InternalReceivingSoldierCancelServices( FALSE );
			 this->InternalGivingSoldierCancelServices( FALSE );

			}
		}
	}

}
/*
void BeginSoldierClimbDownRoof( SOLDIERTYPE *pSoldier )
{
INT8							bNewDirection;
UINT8	ubWhoIsThere;


if ( FindLowerLevel( pSoldier, pSoldier->sGridNo, pSoldier->ubDirection, &bNewDirection ) && ( pSoldier->pathing.bLevel > 0 ) )
{
if ( EnoughPoints( pSoldier, GetAPsToClimbRoof( pSoldier, TRUE ), 0, TRUE ) )
{
if (pSoldier->bTeam == gbPlayerNum)
{
// OK, SET INTERFACE FIRST
SetUIBusy( pSoldier->ubID );
}




pSoldier->sTempNewGridNo = NewGridNo( (INT16)pSoldier->sGridNo, (UINT16)DirectionInc(bNewDirection ) );

bNewDirection = gTwoCDirection[ bNewDirection ];

pSoldier->ubPendingDirection = bNewDirection;
pSoldier->EVENT_InitNewSoldierAnim( CLIMBDOWNROOF, 0 , FALSE );

InternalpSoldier->ReceivingSoldierCancelServices(, FALSE );
InternalpSoldier->GivingSoldierCancelServices(, FALSE );

}
}

}
*/



void SOLDIERTYPE::MoveMerc( FLOAT dMovementChange, FLOAT dAngle, BOOLEAN fCheckRange )
{
	//INT16					dDegAngle;
	FLOAT					dDeltaPos;
	FLOAT					dXPos , dYPos;
	BOOLEAN				fStop = FALSE;


	//dDegAngle = (INT16)( dAngle * 180 / PI );
	//sprintf( gDebugStr, "Move Angle: %d", (int)dDegAngle );

	// Find delta Movement for X pos
	dDeltaPos = (FLOAT) (dMovementChange * sin( dAngle ));

	// Find new position
	dXPos = this->dXPos + dDeltaPos;

	if ( fCheckRange )
	{
		fStop = FALSE;

		switch( this->bMovementDirection )
		{
		case NORTHEAST:
		case EAST:
		case SOUTHEAST:

			if ( dXPos >= this->pathing.sDestXPos )
			{
				fStop = TRUE;
			}
			break;

		case NORTHWEST:
		case WEST:
		case SOUTHWEST:

			if ( dXPos <= this->pathing.sDestXPos )
			{
				fStop = TRUE;
			}
			break;

		case NORTH:
		case SOUTH:

			fStop = TRUE;
			break;

		}

		if ( fStop )
		{
			//dXPos = this->pathing.sDestXPos;
			this->flags.fPastXDest = TRUE;

			if ( this->sGridNo == this->pathing.sFinalDestination )
			{
				dXPos = this->pathing.sDestXPos;
			}
		}
	}

	// Find delta Movement for Y pos
	dDeltaPos = (FLOAT) (dMovementChange * cos( dAngle ));

	// Find new pos
	dYPos = this->dYPos + dDeltaPos;

	if ( fCheckRange )
	{
		fStop = FALSE;

		switch( this->bMovementDirection )
		{
		case NORTH:
		case NORTHEAST:
		case NORTHWEST:

			if ( dYPos <= this->pathing.sDestYPos )
			{
				fStop = TRUE;
			}
			break;

		case SOUTH:
		case SOUTHWEST:
		case SOUTHEAST:

			if ( dYPos >= this->pathing.sDestYPos )
			{
				fStop = TRUE;
			}
			break;

		case EAST:
		case WEST:

			fStop = TRUE;
			break;

		}

		if ( fStop )
		{
			//dYPos = this->pathing.sDestYPos;
			this->flags.fPastYDest = TRUE;

			if ( this->sGridNo == this->pathing.sFinalDestination )
			{
				dYPos = this->pathing.sDestYPos;
			}
		}
	}

	// OK, set new position
	this->EVENT_InternalSetSoldierPosition( dXPos, dYPos, FALSE, FALSE, FALSE );

	//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("X: %f Y: %f", dXPos, dYPos ) );

}

UINT8 GetDirectionFromGridNo( INT16 sGridNo, SOLDIERTYPE *pSoldier )
{
	INT16 sXPos, sYPos;

	ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );

	return( GetDirectionFromXY( sXPos, sYPos, pSoldier ) );
}

INT16 GetDirectionToGridNoFromGridNo( INT16 sGridNoDest, INT16 sGridNoSrc )
{
	INT16 sXPos2, sYPos2;
	INT16 sXPos, sYPos;

	ConvertGridNoToXY( sGridNoSrc, &sXPos, &sYPos );
	ConvertGridNoToXY( sGridNoDest, &sXPos2, &sYPos2 );

	return( atan8( sXPos2, sYPos2, sXPos, sYPos ) );

}

UINT8 GetDirectionFromXY( INT16 sXPos, INT16 sYPos, SOLDIERTYPE *pSoldier )
{
	INT16 sXPos2, sYPos2;

	ConvertGridNoToXY( pSoldier->sGridNo, &sXPos2, &sYPos2 );

	return( atan8( sXPos2, sYPos2, sXPos, sYPos ) );
}

#if 0
UINT8  atan8( INT16 x1, INT16 y1, INT16 x2, INT16 y2 )
{
	static int trig[8] = { 2, 3, 4, 5, 6, 7, 8, 1 };
	// returned values are N=1, NE=2, E=3, SE=4, S=5, SW=6, W=7, NW=8
	double dx=(x2-x1);
	double dy=(y2-y1);
	double a;
	int i,k;
	if (dx==0)
		dx=0.00390625; // 1/256th
#define PISLICES (8)
	a=(atan2(dy,dx) + PI/PISLICES)/(PI/(PISLICES/2));
	i=(int)a;
	if (a>0)
		k=i; else
		if (a<0)
			k=i+(PISLICES-1); else
			k=0;
	return(trig[k]);
}
#endif

//#if 0
UINT8 atan8( INT16 sXPos, INT16 sYPos, INT16 sXPos2, INT16 sYPos2 )
{
	DOUBLE  test_x =  sXPos2 - sXPos;
	DOUBLE  test_y =  sYPos2 - sYPos;
	UINT8	  mFacing = WEST;
	//INT16					dDegAngle;
	DOUBLE angle;

	if ( test_x == 0 )
	{
		test_x = 0.04;
	}

	angle = atan2( test_x, test_y );


	//dDegAngle = (INT16)( angle * 180 / PI );
	//sprintf( gDebugStr, "Move Angle: %d", (int)dDegAngle );

	do
	{
		if ( angle >=-PI*.375 && angle <= -PI*.125 )
		{
			mFacing = SOUTHWEST;
			break;
		}

		if ( angle <= PI*.375 && angle >= PI*.125 )
		{
			mFacing = SOUTHEAST;
			break;
		}

		if ( angle >=PI*.623 && angle <= PI*.875 )
		{
			mFacing = NORTHEAST;
			break;
		}

		if ( angle <=-PI*.623 && angle >= -PI*.875 )
		{
			mFacing = NORTHWEST;
			break;
		}

		if ( angle >-PI*0.125 && angle < PI*0.125 )
		{
			mFacing = SOUTH;
		}
		if ( angle > PI*0.375 && angle < PI*0.623 )
		{
			mFacing = EAST;
		}
		if ( ( angle > PI*0.875 && angle <= PI ) || ( angle > -PI && angle < -PI*0.875 ) )
		{
			mFacing = NORTH;
		}
		if ( angle > -PI*0.623 && angle < -PI*0.375 )
		{
			mFacing = WEST;
		}

	} while( FALSE );

	return( mFacing );
}


UINT8 atan8FromAngle( DOUBLE angle )
{
	UINT8	  mFacing = WEST;

	if ( angle > PI )
	{
		angle = ( angle - PI ) - PI;
	}
	if ( angle < -PI )
	{
		angle = ( PI - ( fabs( angle ) - PI )  );
	}

	do
	{
		if ( angle >=-PI*.375 && angle <= -PI*.125 )
		{
			mFacing = SOUTHWEST;
			break;
		}

		if ( angle <= PI*.375 && angle >= PI*.125 )
		{
			mFacing = SOUTHEAST;
			break;
		}

		if ( angle >=PI*.623 && angle <= PI*.875 )
		{
			mFacing = NORTHEAST;
			break;
		}

		if ( angle <=-PI*.623 && angle >= -PI*.875 )
		{
			mFacing = NORTHWEST;
			break;
		}

		if ( angle >-PI*0.125 && angle < PI*0.125 )
		{
			mFacing = SOUTH;
		}
		if ( angle > PI*0.375 && angle < PI*0.623 )
		{
			mFacing = EAST;
		}
		if ( ( angle > PI*0.875 && angle <= PI ) || ( angle > -PI && angle < -PI*0.875 ) )
		{
			mFacing = NORTH;
		}
		if ( angle > -PI*0.623 && angle < -PI*0.375 )
		{
			mFacing = WEST;
		}

	} while( FALSE );

	return( mFacing );
}


void CheckForFullStructures( SOLDIERTYPE *pSoldier )
{
	// This function checks to see if we are near a specific structure type which requires us to blit a
	// small obscuring peice
	INT16 sGridNo;
	UINT16 usFullTileIndex;
	INT32		cnt;


	// Check in all 'Above' directions
	for ( cnt = 0; cnt < MAX_FULLTILE_DIRECTIONS; cnt++ )
	{
		sGridNo = pSoldier->sGridNo + gsFullTileDirections[ cnt ];

		if ( CheckForFullStruct( sGridNo, &usFullTileIndex ) )
		{
			// Add one for the item's obsuring part
			pSoldier->usFrontArcFullTileList[ cnt ] = usFullTileIndex + 1;
			pSoldier->usFrontArcFullTileGridNos[ cnt ] = sGridNo;
			AddTopmostToHead(  sGridNo, pSoldier->usFrontArcFullTileList[ cnt ] );
		}
		else
		{
			if ( pSoldier->usFrontArcFullTileList[ cnt ] != 0 )
			{
				RemoveTopmost( pSoldier->usFrontArcFullTileGridNos[ cnt ], pSoldier->usFrontArcFullTileList[ cnt ] );
			}
			pSoldier->usFrontArcFullTileList[ cnt ] = 0;
			pSoldier->usFrontArcFullTileGridNos[ cnt ] = 0;
		}
	}

}


BOOLEAN CheckForFullStruct( INT16 sGridNo, UINT16 *pusIndex  )
{
	LEVELNODE	*pStruct		 = NULL;
	LEVELNODE	*pOldStruct		 = NULL;
	UINT32				fTileFlags;

	pStruct = gpWorldLevelData[ sGridNo ].pStructHead;

	// Look through all structs and Search for type

	while( pStruct != NULL )
	{

		if ( pStruct->usIndex != NO_TILE && pStruct->usIndex < NUMBEROFTILES )
		{

			GetTileFlags( pStruct->usIndex, &fTileFlags );

			// Advance to next
			pOldStruct = pStruct;
			pStruct = pStruct->pNext;

			//if( (pOldStruct->pStructureData!=NULL) && ( pOldStruct->pStructureData->fFlags&STRUCTURE_TREE ) )
			if ( fTileFlags & FULL3D_TILE )
			{
				// CHECK IF THIS TREE IS FAIRLY ALONE!
				if ( FullStructAlone( sGridNo, 2 ) )
				{
					// Return true and return index
					*pusIndex = pOldStruct->usIndex;
					return( TRUE );
				}
				else
				{
					return( FALSE );
				}

			}

		}
		else
		{
			// Advance to next
			pOldStruct = pStruct;
			pStruct = pStruct->pNext;
		}

	}

	// Could not find it, return FALSE
	return( FALSE );

}


BOOLEAN FullStructAlone( INT16 sGridNo, UINT8 ubRadius )
{
	INT16  sTop, sBottom;
	INT16  sLeft, sRight;
	INT16  cnt1, cnt2;
	INT16	 iNewIndex;
	INT32	 leftmost;


	// Determine start end end indicies and num rows
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft   = - ubRadius;
	sRight  = ubRadius;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{

		leftmost = ( ( sGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = sGridNo + ( WORLD_COLS * cnt1 ) + cnt2;


			if ( iNewIndex >=0 && iNewIndex < WORLD_MAX &&
				iNewIndex >= leftmost && iNewIndex < ( leftmost + WORLD_COLS ) )
			{
				if ( iNewIndex != sGridNo )
				{
					if ( FindStructure( iNewIndex, STRUCTURE_TREE ) != NULL )
					{
						return( FALSE );
					}
				}
			}

		}
	}

	return( TRUE );
}


void AdjustForFastTurnAnimation( SOLDIERTYPE *pSoldier )
{

	// CHECK FOR FASTTURN ANIMATIONS
	// ATE: Mod: Only fastturn for OUR guys!
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_FASTTURN && pSoldier->bTeam == gbPlayerNum && !( pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT ) )
	{
		if ( pSoldier->ubDirection != pSoldier->pathing.bDesiredDirection )
		{
			pSoldier->sAniDelay = FAST_TURN_ANIM_SPEED;
		}
		else
		{
			SetSoldierAniSpeed( pSoldier );
			//	FreeUpNPCFromTurning( pSoldier, LOOK);
		}
	}

}

BOOLEAN SOLDIERTYPE::IsActionInterruptable( void )
{
	if ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_NONINTERRUPT )
	{
		return( FALSE );
	}
	return( TRUE );
}

// WRAPPER FUNCTIONS FOR SOLDIER EVENTS
void SendSoldierPositionEvent( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos )
{
	// Sent event for position update
	EV_S_SETPOSITION	SSetPosition;

	SSetPosition.usSoldierID = pSoldier->ubID;
	SSetPosition.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	SSetPosition.dNewXPos = dNewXPos;
	SSetPosition.dNewYPos = dNewYPos;

	AddGameEvent( S_SETPOSITION, 0, &SSetPosition );

}

void SendSoldierDestinationEvent( SOLDIERTYPE *pSoldier, UINT16 usNewDestination )
{
	// Sent event for position update
	EV_S_CHANGEDEST	SChangeDest;

	SChangeDest.usSoldierID = pSoldier->ubID;
	SChangeDest.usNewDestination = usNewDestination;
	SChangeDest.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_CHANGEDEST, 0, &SChangeDest );

}

void SendSoldierSetDirectionEvent( SOLDIERTYPE *pSoldier, UINT16 usNewDirection )
{
	// Sent event for position update
	EV_S_SETDIRECTION	SSetDirection;

	SSetDirection.usSoldierID = pSoldier->ubID;
	SSetDirection.usNewDirection = usNewDirection;
	SSetDirection.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_SETDIRECTION, 0, &SSetDirection );

}

void SendSoldierSetDesiredDirectionEvent( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection )
{
	// Sent event for position update
	EV_S_SETDESIREDDIRECTION	SSetDesiredDirection;

	SSetDesiredDirection.usSoldierID = pSoldier->ubID;
	SSetDesiredDirection.usDesiredDirection = usDesiredDirection;
	SSetDesiredDirection.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_SETDESIREDDIRECTION, 0, &SSetDesiredDirection );
	if(is_server || (is_client && pSoldier->ubID <20) ) send_dir( pSoldier, usDesiredDirection );

}

void SendGetNewSoldierPathEvent( SOLDIERTYPE *pSoldier, INT16 sDestGridNo, UINT16 usMovementAnim )
{
	EV_S_GETNEWPATH	SGetNewPath;

	SGetNewPath.usSoldierID				= pSoldier->ubID;
	SGetNewPath.sDestGridNo				= sDestGridNo;
	SGetNewPath.usMovementAnim		= usMovementAnim;
	SGetNewPath.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_GETNEWPATH, 0, &SGetNewPath );
}


void SendChangeSoldierStanceEvent( SOLDIERTYPE *pSoldier, UINT8 ubNewStance )
{
#if 0
	EV_S_CHANGESTANCE			SChangeStance;

#ifdef NETWORKED
	if( !IsTheSolderUnderMyControl( pSoldier->ubID) )
		return;
#endif

	SChangeStance.ubNewStance   = ubNewStance;
	SChangeStance.usSoldierID  = pSoldier->ubID;
	SChangeStance.sXPos				= pSoldier->sX;
	SChangeStance.sYPos				= pSoldier->sY;
	SChangeStance.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_CHANGESTANCE, 0, &SChangeStance );
#endif

	if(((pSoldier->ubID > 19 && !is_server) || (pSoldier->ubID > 119 && is_server) )&& is_networked)return;
						
	pSoldier->ChangeSoldierStance( ubNewStance );
	if(is_server || (is_client && pSoldier->ubID <20) ) send_stance( pSoldier, ubNewStance );
}


void SendBeginFireWeaponEvent( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("SendBeginFireWeaponEvent"));
	EV_S_BEGINFIREWEAPON		SBeginFireWeapon;

	SBeginFireWeapon.usSoldierID			= pSoldier->ubID;
	SBeginFireWeapon.sTargetGridNo		= sTargetGridNo;
	SBeginFireWeapon.bTargetLevel			= pSoldier->bTargetLevel;
	SBeginFireWeapon.bTargetCubeLevel	= pSoldier->bTargetCubeLevel;
	SBeginFireWeapon.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_BEGINFIREWEAPON, 0, &SBeginFireWeapon );

}

#if 0
// This function is now obsolete.  Just call ReduceAttackBusyCount.

// This function just encapolates the check for turnbased and having an attacker in the first place
void ReleaseSoldiersAttacker( SOLDIERTYPE *pSoldier )
{
	INT32 cnt;
	UINT8	ubNumToFree;

	//if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		// ATE: Removed...
		//if ( pSoldier->ubAttackerID != NOBODY )
		{
			// JA2 Gold
			// set next-to-previous attacker, so long as this isn't a repeat attack
			if (pSoldier->ubPreviousAttackerID != pSoldier->ubAttackerID)
			{
				pSoldier->ubNextToPreviousAttackerID = pSoldier->ubPreviousAttackerID;
			}

			// get previous attacker id
			pSoldier->ubPreviousAttackerID = pSoldier->ubAttackerID;

			// Copy BeingAttackedCount here....
			ubNumToFree = pSoldier->bBeingAttackedCount;
			// Zero it out BEFORE, as supression may increase it again...
			pSoldier->bBeingAttackedCount = 0;

			for ( cnt = 0; cnt < ubNumToFree; cnt++ )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker of %d (attacker is %d) - releasesoldierattacker num to free is %d", pSoldier->ubID, pSoldier->ubAttackerID, ubNumToFree ) );
				ReduceAttackBusyCount( pSoldier->ubAttackerID, FALSE );
			}

			// ATE: Set to NOBODY if this person is NOT dead
			// otherise, we keep it so the kill can be awarded!
			if ( pSoldier->stats.bLife != 0 && pSoldier->ubBodyType != QUEENMONSTER )
			{
				pSoldier->ubAttackerID = NOBODY;
			}
		}
	}
}
#endif

BOOLEAN SOLDIERTYPE::MercInWater( void )
{
	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_WATER( this->bOverTerrainType))
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN SOLDIERTYPE::MercInShallowWater( void )
{
	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_SHALLOW_WATER( this->bOverTerrainType))
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN SOLDIERTYPE::MercInDeepWater( void )
{
	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_DEEP_WATER( this->bOverTerrainType))
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN SOLDIERTYPE::MercInHighWater( void )
{
	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_HIGH_WATER( this->bOverTerrainType))
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}



void RevivePlayerTeam( )
{
	INT32 cnt;
	SOLDIERTYPE		*pSoldier;

	// End the turn of player charactors
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		pSoldier->ReviveSoldier( );
	}

}



// What?  A zombie function?
void SOLDIERTYPE::ReviveSoldier( void )
{
	INT16					sX, sY;

	if ( this->stats.bLife < OKLIFE  && this->bActive )
	{
		// If dead or unconscious, revive!
		this->flags.uiStatusFlags &= ( ~SOLDIER_DEAD );

		this->stats.bLife = this->stats.bLifeMax;
		this->bBleeding = 0;
		this->ubDesiredHeight = ANIM_STAND;

		AddManToTeam( this->bTeam );

		// Set to standing
		this->flags.fInNonintAnim = FALSE;
		this->flags.fRTInNonintAnim = FALSE;

		// Change to standing,unless we can getup with an animation
		this->EVENT_InitNewSoldierAnim( STANDING, 0, TRUE );
		this->BeginSoldierGetup( );

		// Makesure center of tile
		sX = CenterX( this->sGridNo );
		sY = CenterY( this->sGridNo );

		this->EVENT_SetSoldierPosition( (FLOAT) sX, (FLOAT) sY );

		// Dirty INterface
		fInterfacePanelDirty = DIRTYLEVEL2;

	}

}


void SOLDIERTYPE::HandleAnimationProfile( UINT16	usAnimState, BOOLEAN fRemove )
{
	//#if 0
	ANIM_PROF					*pProfile;
	ANIM_PROF_DIR			*pProfileDir;
	ANIM_PROF_TILE		*pProfileTile;
	INT8							bProfileID;
	UINT32						iTileCount;
	INT16							sGridNo;
	UINT16						usAnimSurface;

	// ATE

	// Get Surface Index
	usAnimSurface = DetermineSoldierAnimationSurface( this, usAnimState );

	CHECKV( usAnimSurface != INVALID_ANIMATION_SURFACE );

	bProfileID = gAnimSurfaceDatabase[ usAnimSurface ].bProfile;

	// Determine if this animation has a profile
	if ( bProfileID != -1 )
	{
		// Getprofile
		pProfile = &(gpAnimProfiles[ bProfileID ] );

		// Get direction
		pProfileDir = &( pProfile->Dirs[ this->ubDirection ] );

		// Loop tiles and set accordingly into world
		for( iTileCount = 0; iTileCount < pProfileDir->ubNumTiles; iTileCount++ )
		{
			pProfileTile = &( pProfileDir->pTiles[ iTileCount ] );

			sGridNo = this->sGridNo + ( ( WORLD_COLS * pProfileTile->bTileY ) + pProfileTile->bTileX );

			// Check if in bounds
			if ( !OutOfBounds( this->sGridNo, sGridNo ) )
			{
				if ( fRemove )
				{
					// Remove from world
					RemoveMerc( sGridNo, this, TRUE );
				}
				else
				{
					// PLace into world
					AddMercToHead( sGridNo, this, FALSE );
					//if ( pProfileTile->bTileY != 0 || pProfileTile->bTileX != 0 )
					{
						gpWorldLevelData[sGridNo].pMercHead->uiFlags |= LEVELNODE_MERCPLACEHOLDER;
						gpWorldLevelData[sGridNo].pMercHead->uiAnimHitLocationFlags = pProfileTile->usTileFlags;
					}
				}
			}

		}
	}

	//#endif

}


LEVELNODE *GetAnimProfileFlags( INT16 sGridNo, UINT16 *usFlags, SOLDIERTYPE **ppTargSoldier, LEVELNODE *pGivenNode )
{
	LEVELNODE				*pNode;

	(*ppTargSoldier) = NULL;
	(*usFlags )			 = 0;

	if ( pGivenNode == NULL )
	{
		pNode = gpWorldLevelData[sGridNo].pMercHead;
	}
	else
	{
		pNode = pGivenNode->pNext;
	}

	//#if 0

	if ( pNode != NULL )
	{
		if ( pNode->uiFlags & LEVELNODE_MERCPLACEHOLDER )
		{
			(*usFlags) = (UINT16)pNode->uiAnimHitLocationFlags;
			(*ppTargSoldier) = pNode->pSoldier;
		}
	}

	//#endif

	return( pNode );

}


BOOLEAN SOLDIERTYPE::GetProfileFlagsFromGridno( UINT16 usAnimState, UINT16 sTestGridNo, UINT16 *usFlags )
{
	ANIM_PROF					*pProfile;
	ANIM_PROF_DIR			*pProfileDir;
	ANIM_PROF_TILE		*pProfileTile;
	INT8							bProfileID;
	UINT32						iTileCount;
	INT16							sGridNo;
	UINT16						usAnimSurface;

	// Get Surface Index
	usAnimSurface = DetermineSoldierAnimationSurface( this, usAnimState );

	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	bProfileID = gAnimSurfaceDatabase[ usAnimSurface ].bProfile;

	*usFlags = 0;

	// Determine if this animation has a profile
	if ( bProfileID != -1 )
	{
		// Getprofile
		pProfile = &(gpAnimProfiles[ bProfileID ] );

		// Get direction
		pProfileDir = &( pProfile->Dirs[ this->ubDirection ] );

		// Loop tiles and set accordingly into world
		for( iTileCount = 0; iTileCount < pProfileDir->ubNumTiles; iTileCount++ )
		{
			pProfileTile = &( pProfileDir->pTiles[ iTileCount ] );

			sGridNo = this->sGridNo + ( ( WORLD_COLS * pProfileTile->bTileY ) + pProfileTile->bTileX );

			// Check if in bounds
			if ( !OutOfBounds( this->sGridNo, sGridNo ) )
			{
				if ( sGridNo == sTestGridNo )
				{
					*usFlags = pProfileTile->usTileFlags;
					return( TRUE );
				}
			}

		}
	}

	return( FALSE );
}


void SOLDIERTYPE::EVENT_SoldierBeginGiveItem( void )
{
	SOLDIERTYPE *pTSoldier;

	if ( VerifyGiveItem( this, &pTSoldier ) )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->pathing.bDesiredDirection = this->aiData.bPendingActionData3;
		this->ubDirection = this->aiData.bPendingActionData3;

		// begin animation
		this->EVENT_InitNewSoldierAnim( GIVE_ITEM, 0 , FALSE );

	}
	else
	{
		UnSetEngagedInConvFromPCAction( this );

		OBJECTTYPE::DeleteMe( &this->pTempObject );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginBladeAttack( INT16 sGridNo, UINT8 ubDirection )
{
	SOLDIERTYPE *pTSoldier;
	//UINT32 uiMercFlags;
	UINT16 usSoldierIndex;
	UINT8 ubTDirection;
	ROTTING_CORPSE *pCorpse;

	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Begin blade attack: ATB  %d", gTacticalStatus.ubAttackBusyCount) );
	DebugAttackBusy( String( "Begin blade attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount) );

	//}

	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	this->EVENT_SetSoldierDesiredDirection( ubDirection );
	this->EVENT_SetSoldierDirection( ubDirection );
	// CHANGE TO ANIMATION

	// DETERMINE ANIMATION TO PLAY
	// LATER BASED ON IF TAREGT KNOWS OF US, STANCE, ETC
	// GET POINTER TO TAREGT
	if (this->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		UINT8 ubTargetID;

		// Is there an unconscious guy at gridno......
		ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );

		if ( ubTargetID != NOBODY && ( ( MercPtrs[ ubTargetID ]->stats.bLife < OKLIFE && MercPtrs[ ubTargetID ]->stats.bLife > 0 ) || ( MercPtrs[ ubTargetID ]->bBreath < OKBREATH && MercPtrs[ ubTargetID ]->bCollapsed ) ) )
		{
			this->aiData.uiPendingActionData4 = ubTargetID;
			// add regen bonus
			this->bRegenerationCounter++;
			this->EVENT_InitNewSoldierAnim( MONSTER_BEGIN_EATTING_FLESH, 0, FALSE );
		}
		else
		{
			if ( PythSpacesAway( this->sGridNo, sGridNo ) <= 1 )
			{
				this->EVENT_InitNewSoldierAnim( MONSTER_CLOSE_ATTACK, 0, FALSE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( ADULTMONSTER_ATTACKING, 0, FALSE );
			}
		}
	}
	else if (this->ubBodyType == BLOODCAT)
	{
		// Check if it's a claws or teeth...
		if ( this->inv[ HANDPOS ].usItem == BLOODCAT_CLAW_ATTACK )
		{
			this->EVENT_InitNewSoldierAnim( BLOODCAT_SWIPE, 0, FALSE );
		}
		else
		{
			this->EVENT_InitNewSoldierAnim( BLOODCAT_BITE_ANIM, 0, FALSE );
		}
	}
	else
	{
		usSoldierIndex = WhoIsThere2( sGridNo, this->bTargetLevel );
		if ( usSoldierIndex != NOBODY )
		{
			GetSoldier( &pTSoldier, usSoldierIndex );

			// Look at stance of target
			switch( gAnimControl[ pTSoldier->usAnimState ].ubEndHeight	)
			{
			case ANIM_STAND:
			case ANIM_CROUCH:

				// CHECK IF HE CAN SEE US, IF SO RANDOMIZE
				if ( pTSoldier->aiData.bOppList[ this->ubID ] == 0 && pTSoldier->bTeam != this->bTeam )
				{
					// WE ARE NOT SEEN
					this->EVENT_InitNewSoldierAnim( STAB, 0 , FALSE );
				}
				else
				{
					// WE ARE SEEN
					if ( Random( 50 ) > 25 )
					{
						this->EVENT_InitNewSoldierAnim( STAB, 0 , FALSE );
					}
					else
					{
						this->EVENT_InitNewSoldierAnim( SLICE, 0 , FALSE );
					}

					// IF WE ARE SEEN, MAKE SURE GUY TURNS!
					// Get direction to target
					// IF WE ARE AN ANIMAL, CAR, MONSTER, DONT'T TURN
					if ( !( pTSoldier->flags.uiStatusFlags & ( SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE ) ) )
					{
						// OK, stop merc....
						pTSoldier->EVENT_StopMerc( pTSoldier->sGridNo, pTSoldier->ubDirection );

						if ( pTSoldier->bTeam != gbPlayerNum )
						{
							CancelAIAction( pTSoldier, TRUE );
						}

						ubTDirection = (UINT8)GetDirectionFromGridNo( this->sGridNo, pTSoldier );
						SendSoldierSetDesiredDirectionEvent( pTSoldier, ubTDirection );
					}
				}

				break;

			case ANIM_PRONE:

				// CHECK OUR STANCE
				if ( gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_CROUCH )
				{
					// SET DESIRED STANCE AND SET PENDING ANIMATION
					SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
					this->usPendingAnimation = CROUCH_STAB;
				}
				else
				{
					// USE crouched one
					// NEED TO CHANGE STANCE IF NOT CROUCHD!
					this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0 , FALSE );
				}
				break;
			}
		}
		else
		{
			// OK, SEE IF THERE IS AN OBSTACLE HERE...
			if ( !NewOKDestination( this, sGridNo, FALSE, this->pathing.bLevel ) )
			{
				this->EVENT_InitNewSoldierAnim( STAB, 0 , FALSE );
			}
			else
			{
				// Check for corpse!
				pCorpse = GetCorpseAtGridNo( sGridNo, this->pathing.bLevel );

				if ( pCorpse == NULL )
				{
					this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0 , FALSE );
				}
				else
				{
					if ( IsValidDecapitationCorpse( pCorpse ) )
					{
						this->EVENT_InitNewSoldierAnim( DECAPITATE, 0 , FALSE );
					}
					else
					{
						this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0 , FALSE );
					}
				}
			}
		}
	}

	// SET TARGET GRIDNO
	this->sTargetGridNo = sGridNo;
	this->bTargetLevel = this->pathing.bLevel;
	this->ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );
}


void SOLDIERTYPE::EVENT_SoldierBeginPunchAttack( INT16 sGridNo, UINT8 ubDirection )
{
	BOOLEAN			fMartialArtist = FALSE;
	SOLDIERTYPE *pTSoldier;
	//UINT32 uiMercFlags;
	UINT16 usSoldierIndex;
	UINT8 ubTDirection;
	BOOLEAN fChangeDirection = FALSE;
	UINT16	usItem;

	// Get item in hand...
	usItem = this->inv[ HANDPOS ].usItem;


	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Begin HTH attack: ATB  %d", gTacticalStatus.ubAttackBusyCount) );
	DebugAttackBusy( String( "Begin HTH attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount) );

	//}

	// get target.....
	usSoldierIndex = WhoIsThere2( this->sTargetGridNo, this->pathing.bLevel );
	if ( usSoldierIndex != NOBODY )
	{
		GetSoldier( &pTSoldier, usSoldierIndex );

		fChangeDirection = TRUE;
	}
	else
	{
		return;
	}

	if ( fChangeDirection )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );
	}

	// Are we a martial artist?
	if ( HAS_SKILL_TRAIT( this, MARTIALARTS ) )
	{
		fMartialArtist = TRUE;
	}


	if ( fMartialArtist && !AreInMeanwhile( ) && !Item[usItem].crowbar )
	{
		// Are we in attack mode yet?
		if ( this->usAnimState != NINJA_BREATH && gAnimControl[ this->usAnimState ].ubHeight == ANIM_STAND && gAnimControl[ pTSoldier->usAnimState ].ubHeight != ANIM_PRONE )
		{
			this->EVENT_InitNewSoldierAnim( NINJA_GOTOBREATH, 0 , FALSE );
		}
		else
		{
			this->DoNinjaAttack( );
		}
	}
	else
	{
		// Look at stance of target
		switch( gAnimControl[ pTSoldier->usAnimState ].ubEndHeight	)
		{
		case ANIM_STAND:
		case ANIM_CROUCH:

			if ( !Item[usItem].crowbar )
			{
				this->EVENT_InitNewSoldierAnim( PUNCH, 0 , FALSE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( CROWBAR_ATTACK, 0 , FALSE );
			}

			// CHECK IF HE CAN SEE US, IF SO CHANGE DIR
			if ( pTSoldier->aiData.bOppList[ this->ubID ] == 0 && pTSoldier->bTeam != this->bTeam )
			{
				// Get direction to target
				// IF WE ARE AN ANIMAL, CAR, MONSTER, DONT'T TURN
				if ( !( pTSoldier->flags.uiStatusFlags & ( SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE ) ) )
				{
					// OK, stop merc....
					pTSoldier->EVENT_StopMerc( pTSoldier->sGridNo, pTSoldier->ubDirection );

					if ( pTSoldier->bTeam != gbPlayerNum )
					{
						CancelAIAction( pTSoldier, TRUE );
					}

					ubTDirection = (UINT8)GetDirectionFromGridNo( this->sGridNo, pTSoldier );
					SendSoldierSetDesiredDirectionEvent( pTSoldier, ubTDirection );
				}
			}
			break;

		case ANIM_PRONE:

			// CHECK OUR STANCE
			// ATE: Added this for CIV body types 'cause of elliot
			if ( !IS_MERC_BODY_TYPE( this ) )
			{
				this->EVENT_InitNewSoldierAnim( PUNCH, 0 , FALSE );
			}
			else
			{
				if ( gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_CROUCH )
				{
					// SET DESIRED STANCE AND SET PENDING ANIMATION
					SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
					this->usPendingAnimation = PUNCH_LOW;
				}
				else
				{
					// USE crouched one
					// NEED TO CHANGE STANCE IF NOT CROUCHD!
					this->EVENT_InitNewSoldierAnim( PUNCH_LOW, 0 , FALSE );
				}
			}
			break;
		}
	}

	// SET TARGET GRIDNO
	this->sTargetGridNo = sGridNo;
	this->bTargetLevel = this->pathing.bLevel;
	this->sLastTarget		= sGridNo;
	this->ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );
}


void SOLDIERTYPE::EVENT_SoldierBeginKnifeThrowAttack( INT16 sGridNo, UINT8 ubDirection )
{
	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
//	gTacticalStatus.ubAttackBusyCount++;
	//}
//	this->bBulletsLeft = 1;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting knifethrow attack, bullets left %d", this->bBulletsLeft) );
	DebugAttackBusy( String( "Begin knife throwing attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount) );

	this->EVENT_InitNewSoldierAnim( THROW_KNIFE, 0 , FALSE );

	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	this->EVENT_SetSoldierDesiredDirection( ubDirection );
	this->EVENT_SetSoldierDirection( ubDirection );


	// SET TARGET GRIDNO
	this->sTargetGridNo = sGridNo;
	this->sLastTarget		= sGridNo;
	this->flags.bTurningFromPronePosition	= 0;
	// NB target level must be set by functions outside of here... but I think it
	// is already set in HandleItem or in the AI code - CJC
	this->ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );
}


void SOLDIERTYPE::EVENT_SoldierBeginDropBomb( void )
{
	// Increment the number of people busy doing stuff because of an attack
	switch( gAnimControl[ this->usAnimState ].ubHeight )
	{
	case ANIM_STAND:

		this->EVENT_InitNewSoldierAnim( PLANT_BOMB, 0 , FALSE );
		break;

	default:

		// Call hander for planting bomb...
		HandleSoldierDropBomb( this, this->aiData.sPendingActionData2 );
		this->SoldierGotoStationaryStance( );
		break;
	}

}


void SOLDIERTYPE::EVENT_SoldierBeginUseDetonator( void )
{
	// Increment the number of people busy doing stuff because of an attack
	switch( gAnimControl[ this->usAnimState ].ubHeight )
	{
	case ANIM_STAND:

		this->EVENT_InitNewSoldierAnim( USE_REMOTE, 0 , FALSE );
		break;

	default:

		// Call hander for planting bomb...
		HandleSoldierUseRemote( this, this->aiData.sPendingActionData2 );
		break;
	}
}

void SOLDIERTYPE::EVENT_SoldierBeginFirstAid( INT16 sGridNo, UINT8 ubDirection )
{
	SOLDIERTYPE *pTSoldier;
	//UINT32 uiMercFlags;
	UINT16 usSoldierIndex;
	BOOLEAN fRefused = FALSE;

	usSoldierIndex = WhoIsThere2( sGridNo, this->pathing.bLevel );
	if ( usSoldierIndex != NOBODY )
	{
		pTSoldier = MercPtrs[ usSoldierIndex ];

		// OK, check if we should play quote...
		if ( pTSoldier->bTeam != gbPlayerNum )
		{
			if ( pTSoldier->ubProfile != NO_PROFILE && pTSoldier->ubProfile >= FIRST_RPC && pTSoldier->ubProfile < GASTON && !RPC_RECRUITED( pTSoldier ) )
			{
				fRefused = PCDoesFirstAidOnNPC( pTSoldier->ubProfile );
			}

			if ( !fRefused )
			{
				if ( CREATURE_OR_BLOODCAT( pTSoldier ) )
				{
					// nope!!
					fRefused = TRUE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_REFUSE_FIRSTAID_FOR_CREATURE ] );
				}
				else if ( !pTSoldier->aiData.bNeutral && pTSoldier->stats.bLife >= OKLIFE && pTSoldier->bSide != this->bSide )
				{
					fRefused = TRUE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_REFUSE_FIRSTAID ] );
				}

			}
		}

		if ( fRefused )
		{
			UnSetUIBusy( this->ubID );
			return;
		}

		// ATE: We can only give firsty aid to one perosn at a time... cancel
		// any now...
		this->InternalGivingSoldierCancelServices( FALSE );

		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		// CHECK OUR STANCE AND GOTO CROUCH IF NEEDED
		//if ( gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_CROUCH )
		//{
		// SET DESIRED STANCE AND SET PENDING ANIMATION
		//	SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
		//	this->usPendingAnimation = START_AID;
		//}
		//else
		{
			// CHANGE TO ANIMATION
			if(!is_networked)this->EVENT_InitNewSoldierAnim( START_AID, 0 , FALSE );
			else this->ChangeSoldierState( START_AID, 0, 0 );

		}

		// SET TARGET GRIDNO
		this->sTargetGridNo = sGridNo;

		// SET PARTNER ID
		this->ubServicePartner = (UINT8)usSoldierIndex;

		// SET PARTNER'S COUNT REFERENCE
		pTSoldier->ubServiceCount++;

		// If target and doer are no the same guy...
		if ( pTSoldier->ubID != this->ubID && !pTSoldier->bCollapsed )
		{
			pTSoldier->SoldierGotoStationaryStance( );
		}
	}
}


void SOLDIERTYPE::EVENT_SoldierEnterVehicle( INT16 sGridNo, UINT8 ubDirection )
{
	SOLDIERTYPE *pTSoldier;
	UINT32 uiMercFlags;
	UINT16 usSoldierIndex;

	if ( FindSoldier( sGridNo, &usSoldierIndex, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
	{
		pTSoldier = MercPtrs[ usSoldierIndex ];

		// Enter vehicle...
		EnterVehicle( pTSoldier, this );
	}

	UnSetUIBusy( this->ubID );
}


UINT32 SOLDIERTYPE::SoldierDressWound( SOLDIERTYPE *pVictim, INT16 sKitPts, INT16 sStatus )
{
	UINT32 uiDressSkill, uiPossible, uiActual, uiMedcost, uiDeficiency, uiAvailAPs, uiUsedAPs;
	UINT8 ubBelowOKlife, ubPtsLeft;
	BOOLEAN	fRanOut = FALSE;

	if (pVictim->bBleeding < 1 && pVictim->stats.bLife >= OKLIFE )
	{
		return(0);		// nothing to do, shouldn't have even been called!
	}

	if ( pVictim->stats.bLife == 0 )
	{
		return(0);
	}

	// in case he has multiple kits in hand, limit influence of kit status to 100%!
	if (sStatus >= 100)
	{
		sStatus = 100;
	}

	// calculate wound-dressing skill (3x medical, 2x equip, 1x level, 1x dex)
	uiDressSkill = ( ( 3 * EffectiveMedical( this ) ) +					// medical knowledge
		( 2 * sStatus) + 																// state of medical kit
		(10 * EffectiveExpLevel( this ) ) +					// battle injury experience
		EffectiveDexterity( this ) )	/ 7;		// general "handiness"

	// try to use every AP that the merc has left
	uiAvailAPs = this->bActionPoints;

	// OK, If we are in real-time, use another value...
	if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		// Set to a value which looks good based on our tactical turns duration
		uiAvailAPs = RT_FIRST_AID_GAIN_MODIFIER;
	}

	// calculate how much bandaging CAN be done this turn
	uiPossible = ( uiAvailAPs * uiDressSkill ) / 50;	// max rate is 2 * fullAPs

	// if no healing is possible (insufficient APs or insufficient dressSkill)
	if (!uiPossible)
		return(0);


	if (Item[this->inv[ HANDPOS ].usItem].medicalkit )		// using the GOOD medic stuff
	{
		uiPossible += ( uiPossible / 2);			// add extra 50 %
	}

	uiActual = uiPossible;		// start by assuming maximum possible


	// figure out how far below OKLIFE the victim is
	if (pVictim->stats.bLife >= OKLIFE)
	{
		ubBelowOKlife = 0;
	}
	else
	{
		ubBelowOKlife = OKLIFE - pVictim->stats.bLife;
	}

	// figure out how many healing pts we need to stop dying (2x cost)
	uiDeficiency = (2 * ubBelowOKlife );

	// if, after that, the patient will still be bleeding
	if ( (pVictim->bBleeding - ubBelowOKlife ) > 0)
	{
		// then add how many healing pts we need to stop bleeding (1x cost)
		uiDeficiency += ( pVictim->bBleeding - ubBelowOKlife );
	}

	// now, make sure we weren't going to give too much
	if ( uiActual > uiDeficiency)	// if we were about to apply too much
		uiActual = uiDeficiency;	// reduce actual not to waste anything


	// now make sure we HAVE that much
	if (Item[this->inv[ HANDPOS ].usItem].medicalkit )
	{
		uiMedcost = (uiActual + 1) / 2;		// cost is only half, rounded up

		if ( uiMedcost > (UINT32)sKitPts )     		// if we can't afford this
		{
			fRanOut = TRUE;
			uiMedcost = sKitPts;		// what CAN we afford?
			uiActual = uiMedcost * 2;		// give double this as aid
		}
	}
	else
	{
		uiMedcost = uiActual;

		if ( uiMedcost > (UINT32)sKitPts)		// can't afford it
		{
			fRanOut = TRUE;
			uiMedcost = uiActual = sKitPts;   	// recalc cost AND aid
		}
	}

	ubPtsLeft = (UINT8)uiActual;


	// heal real life points first (if below OKLIFE) because we don't want the
	// patient still DYING if bandages run out, or medic is disabled/distracted!
	// NOTE: Dressing wounds for life below OKLIFE now costs 2 pts/life point!
	if ( ubPtsLeft && pVictim->stats.bLife < OKLIFE)
	{
		// if we have enough points to bring him all the way to OKLIFE this turn
		if ( ubPtsLeft >= (2 * ubBelowOKlife ) )
		{
			// raise life to OKLIFE
			pVictim->stats.bLife = OKLIFE;

			// reduce bleeding by the same number of life points healed up
			pVictim->bBleeding -= ubBelowOKlife;

			// use up appropriate # of actual healing points
			ubPtsLeft -= (2 * ubBelowOKlife);
		}
		else
		{
			pVictim->stats.bLife += ( ubPtsLeft / 2);
			pVictim->bBleeding -= ( ubPtsLeft / 2);

			ubPtsLeft = ubPtsLeft % 2;	// if ptsLeft was odd, ptsLeft = 1
		}

		// this should never happen any more, but make sure bleeding not negative
		if (pVictim->bBleeding < 0)
		{
			pVictim->bBleeding = 0;
		}

		// if this healing brought the patient out of the worst of it, cancel dying
		if (pVictim->stats.bLife >= OKLIFE )
		{
			//pVictim->dying = pVictim->dyingComment = FALSE;
			//pVictim->shootOn = TRUE;

			// turn off merc QUOTE flags
			pVictim->flags.fDyingComment = FALSE;

		}

		// update patient's entire panel (could have regained consciousness, etc.)
	}


	// if any healing points remain, apply that to any remaining bleeding (1/1)
	// DON'T spend any APs/kit pts to cure bleeding until merc is no longer dying
	//if ( ubPtsLeft && pVictim->bBleeding && !pVictim->dying)
	if ( ubPtsLeft && pVictim->bBleeding )
	{
		// if we have enough points to bandage all remaining bleeding this turn
		if (ubPtsLeft >= pVictim->bBleeding )
		{
			ubPtsLeft -= pVictim->bBleeding;
			pVictim->bBleeding = 0;
		}
		else		// bandage what we can
		{
			pVictim->bBleeding -= ubPtsLeft;
			ubPtsLeft = 0;
		}

		// update patient's life bar only
	}


	// if wound has been dressed enough so that bleeding won't occur, turn off
	// the "warned about bleeding" flag so merc tells us about the next bleeding
	if ( pVictim->bBleeding <= MIN_BLEEDING_THRESHOLD )
	{
		pVictim->flags.fWarnedAboutBleeding = FALSE;
	}


	// if there are any ptsLeft now, then we didn't actually get to use them
	uiActual -= ubPtsLeft;

	// usedAPs equals (actionPts) * (%of possible points actually used)
	uiUsedAPs = ( uiActual * uiAvailAPs ) / uiPossible;

	if (Item[this->inv[ HANDPOS ].usItem].medicalkit  )	// using the GOOD medic stuff
	{
		uiUsedAPs = ( uiUsedAPs * 2) / 3;	// reverse 50% bonus by taking 2/3rds
	}

	DeductPoints( this, (INT16)uiUsedAPs, (INT16)( ( uiUsedAPs * BP_PER_AP_LT_EFFORT) ) );


	SOLDIERTYPE* pSoldier = this;
	if ( PTR_OURTEAM )
	{
		// MEDICAL GAIN   (actual / 2):  Helped someone by giving first aid
		StatChange(this, MEDICALAMT, (UINT16)(uiActual / 2), FALSE);

		// DEXTERITY GAIN (actual / 6):  Helped someone by giving first aid
		StatChange(this, DEXTAMT,    (UINT16)(uiActual / 6), FALSE);
	}
	if(is_networked && pVictim->ubID > 19)send_heal(pVictim);

	return( uiMedcost );
}


void SOLDIERTYPE::InternalReceivingSoldierCancelServices( BOOLEAN fPlayEndAnim )
{
	SOLDIERTYPE	*pTSoldier;
	INT32		cnt;

	if ( this->ubServiceCount > 0 )
	{
		// Loop through guys who have us as servicing
		for ( pTSoldier = Menptr, cnt = 0; cnt < MAX_NUM_SOLDIERS; pTSoldier++, cnt++ )
		{
			if ( pTSoldier->bActive )
			{
				if ( pTSoldier->ubServicePartner == this->ubID )
				{
					// END SERVICE!
					this->ubServiceCount--;

					pTSoldier->ubServicePartner = NOBODY;

					if ( gTacticalStatus.fAutoBandageMode )
					{
						this->ubAutoBandagingMedic = NOBODY;

						ActionDone( pTSoldier );
					}
					else
					{
						// don't use end aid animation in autobandage
						if ( pTSoldier->stats.bLife >= OKLIFE && pTSoldier->bBreath > 0 && fPlayEndAnim )
						{
							if(!is_networked)pTSoldier->EVENT_InitNewSoldierAnim( END_AID, 0 , FALSE );
							else pTSoldier->ChangeSoldierState( END_AID, 0, 0 );
						}
					}


				}
			}
		}

	}

}


void SOLDIERTYPE::ReceivingSoldierCancelServices( void )
{
	this->InternalReceivingSoldierCancelServices( TRUE );
}


void SOLDIERTYPE::InternalGivingSoldierCancelServices( BOOLEAN fPlayEndAnim )
{
	SOLDIERTYPE	*pTSoldier;

	// GET TARGET SOLDIER
	if ( this->ubServicePartner != NOBODY )
	{
		pTSoldier = MercPtrs[ this->ubServicePartner ];

		// END SERVICE!
		pTSoldier->ubServiceCount--;

		this->ubServicePartner = NOBODY;

		if ( gTacticalStatus.fAutoBandageMode )
		{
			pTSoldier->ubAutoBandagingMedic = NOBODY;

			ActionDone( this );
		}
		else
		{
			if ( this->stats.bLife >= OKLIFE && this->bBreath > 0 && fPlayEndAnim )
			{
				// don't use end aid animation in autobandage
					if(!is_networked)this->EVENT_InitNewSoldierAnim( END_AID, 0 , FALSE );
					else this->ChangeSoldierState( END_AID, 0, 0 );	
			}
		}
	}

}

void SOLDIERTYPE::GivingSoldierCancelServices( void )
{
	this->InternalGivingSoldierCancelServices( TRUE );
}


void SOLDIERTYPE::HaultSoldierFromSighting( BOOLEAN fFromSightingEnemy )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("HaultSoldierFromSighting") );
	// SEND HUALT EVENT!
	EV_S_STOP_MERC				SStopMerc;

	SStopMerc.sGridNo					= this->sGridNo;
	SStopMerc.ubDirection			= this->ubDirection;
	SStopMerc.usSoldierID			= this->ubID;
	SStopMerc.fset=TRUE;
		SStopMerc.sXPos=this->sX;
		SStopMerc.sYPos=this->sY;
	//AddGameEvent( S_STOP_MERC, 0, &SStopMerc ); //hayden.
	if(this->ubID>=120 || (!is_server && this->ubID >=20)) return;//hayden
	if(is_client)send_stop(&SStopMerc);

	// If we are a 'specialmove... ignore...
	if ( ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_SPECIALMOVE ) )
	{
		return;
	}

	// OK, check if we were going to throw something, and give it back if so!
	if ( this->pTempObject != NULL && fFromSightingEnemy )
	{
		// Place it back into inv....
		AutoPlaceObject( this, this->pTempObject, FALSE );

		//AXP 25.03.2007: Not needed anymore, grenade costs are only deducted on throwing the object
		//AXP 24.03.2007: Give APs back if we wanted to throw grenade, but interrupt/spotting occured
		//if ( this->pThrowParams->ubActionCode == THROW_ARM_ITEM )
		//{
		//	DeductPoints( this, -MinAPsToAttack( this, this->sTargetGridNo, FALSE ), 0 );
		//}

		OBJECTTYPE::DeleteMe( &this->pTempObject );
		this->usPendingAnimation  = NO_PENDING_ANIMATION;
		this->usPendingAnimation2 = NO_PENDING_ANIMATION;

		// Decrement attack counter...
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending throw because saw something") );
		DebugAttackBusy("@@@@@@@ Reducing attacker busy count..., ending throw because saw something\n");
		FreeUpAttacker( );

		// ATE: Goto stationary stance......
		this->SoldierGotoStationaryStance( );

		DirtyMercPanelInterface( this, DIRTYLEVEL2 );
	}


	// Kaiden: Added fix from UB for seeing new enemies when throwing Knives.
	// ATE: Dave, don't kill me
	// Here, we need to handle the situation when we're throweing a knife and we see somebody
	// cause for some reason throwing a knie does not use the pTempObject stuff that all other stuff does...
	if ( this->usPendingAnimation == THROW_KNIFE )
	{
		// Decrement attack counter...
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending throw knife because saw something") );
		DebugAttackBusy("@@@@@@@ Reducing attacker busy count..., ending throw knife because saw something");
		FreeUpAttacker( );

		// ATE: Goto stationary stance......
		this->SoldierGotoStationaryStance( );

		DirtyMercPanelInterface( this, DIRTYLEVEL2 );
	}



	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
	}
	else
	{
		// Pause this guy from no APS
		this->AdjustNoAPToFinishMove( TRUE );

		this->ubReasonCantFinishMove = REASON_STOPPED_SIGHT;

		// ATE; IF turning to shoot, stop!
		// ATE: We want to do this only for enemies, not items....
		if ( this->flags.fTurningToShoot && fFromSightingEnemy )
		{
			this->flags.fTurningToShoot = FALSE;
			// Release attacker

			// OK - this is hightly annoying , but due to the huge combinations of
			// things that can happen - 1 of them is that sLastTarget will get unset
			// after turn is done - so set flag here to tell it not to...
			this->flags.fDontUnsetLastTargetFromTurn = TRUE;

			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending fire because saw something") );
			DebugAttackBusy("@@@@@@@ Reducing attacker busy count..., ending fire because saw something\n");
			FreeUpAttacker( );

		}

		// OK, if we are stopped at our destination, cancel pending action...
		if ( fFromSightingEnemy )
		{
			if ( this->aiData.ubPendingAction != NO_PENDING_ACTION && this->sGridNo == this->pathing.sFinalDestination )
			{
				this->aiData.ubPendingAction = NO_PENDING_ACTION;
			}

			// Stop pending animation....
			this->usPendingAnimation = NO_PENDING_ANIMATION;
			this->usPendingAnimation2 = NO_PENDING_ANIMATION;

			// Kaiden: Another UB Bug Fix.
			// ATE: Nasty bug - clear out any fence jumping that may be in progress
			this->flags.fContinueMoveAfterStanceChange = FALSE;
		}

		if ( !this->flags.fTurningToShoot )
		{
			this->flags.bTurningFromPronePosition = FALSE;
		}
	}

	// Unset UI!
	if ( fFromSightingEnemy || ( this->pTempObject == NULL && !this->flags.fTurningToShoot ) )
	{
		UnSetUIBusy( this->ubID );
	}

	this->bTurningFromUI = FALSE;

	UnSetEngagedInConvFromPCAction( this );
}


// HUALT EVENT IS USED TO STOP A MERC - NETWORKING SHOULD CHECK / ADJUST TO GRIDNO?
void SOLDIERTYPE::EVENT_StopMerc( INT16 sGridNo, INT8 bDirection )
{
	INT16 sX, sY;

	// MOVE GUY TO GRIDNO--- SHOULD BE THE SAME UNLESS IN MULTIPLAYER
	// Makesure center of tile
	sX = CenterX( sGridNo );
	sY = CenterY( sGridNo );


	//Cancel pending events
	if ( !this->flags.fDelayedMovement )
	{
		this->usPendingAnimation = NO_PENDING_ANIMATION;
		this->usPendingAnimation2 = NO_PENDING_ANIMATION;
		this->ubPendingDirection = NO_PENDING_DIRECTION;
		this->aiData.ubPendingAction		 = NO_PENDING_ACTION;
	}

	this->bEndDoorOpenCode				 = 0;
	this->flags.bTurningFromPronePosition	= 0;

	// Cancel path data!
	this->pathing.usPathIndex = this->pathing.usPathDataSize = 0;

	// Set ext tile waiting flag off!
	this->flags.fDelayedMovement = FALSE;

	// Turn off reverse...
	this->bReverse = FALSE;

	this->EVENT_SetSoldierPosition( (FLOAT) sX, (FLOAT) sY );
	this->pathing.sDestXPos = (INT16)this->dXPos;
	this->pathing.sDestYPos = (INT16)this->dYPos;
	this->EVENT_SetSoldierDirection( bDirection);

	if ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_MOVING )
	{
		this->SoldierGotoStationaryStance( );
	}

	// ATE; IF turning to shoot, stop!
	if ( this->flags.fTurningToShoot )
	{
		this->flags.fTurningToShoot = FALSE;
		// Release attacker
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending fire because saw something") );
		DebugAttackBusy("@@@@@@@ Reducing attacker busy count..., ending fire because saw something\n");
		FreeUpAttacker( );
	}

	// Turn off multi-move speed override....
	if ( this->sGridNo == this->pathing.sFinalDestination )
	{
		this->flags.fUseMoverrideMoveSpeed = FALSE;
	}

	// Unset UI!
	UnSetUIBusy( this->ubID );

	UnMarkMovementReserved( this );

}


void SOLDIERTYPE::ReLoadSoldierAnimationDueToHandItemChange( UINT16 usOldItem, UINT16 usNewItem )
{
	// DON'T continue aiming!
	// GOTO STANCE
	// CHECK FOR AIMING ANIMATIONS
	BOOLEAN	fOldRifle = FALSE;
	BOOLEAN	fNewRifle = FALSE;

	// Shutoff burst....
	// ( we could be on, then change gun that does not have burst )
	if ( Weapon[ usNewItem ].ubShotsPerBurst == 0 && !Weapon[this->inv[HANDPOS].usItem ].NoSemiAuto )
	{
		this->bDoBurst		= FALSE;
		this->bWeaponMode = WM_NORMAL;
		this->bDoAutofire = 0;
	}
	else if ( Weapon[usNewItem].NoSemiAuto )
	{
		this->bDoBurst		= TRUE;
		this->bWeaponMode = WM_AUTOFIRE;
		this->bDoAutofire = 1;
	}

	if ( gAnimControl[ this->usAnimState ].uiFlags & ANIM_FIREREADY )
	{
		// Stop aiming!
		this->SoldierGotoStationaryStance( );
	}

	// Cancel services...
	this->GivingSoldierCancelServices( );

	// Did we have a rifle and do we now not have one?
	if ( usOldItem != NOTHING )
	{
		if ( Item[ usOldItem ].usItemClass == IC_GUN )
		{
			//			if ( (Item[ usOldItem ].fFlags & ITEM_TWO_HANDED) && usOldItem != ROCKET_LAUNCHER )
			if ( (Item[ usOldItem ].twohanded ) && !Item[usOldItem].rocketlauncher )
			{
				fOldRifle = TRUE;
			}
		}
	}

	if ( usNewItem != NOTHING )
	{
		if ( Item[ usNewItem ].usItemClass == IC_GUN )
		{
			//			if ( (Item[ usNewItem ].fFlags & ITEM_TWO_HANDED) && usNewItem != ROCKET_LAUNCHER )
			if ( (Item[ usNewItem ].twohanded ) && !Item[usNewItem].rocketlauncher )
			{
				fNewRifle = TRUE;
			}
		}
	}

	// Switch on stance!
	switch( gAnimControl[ this->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:

		if ( fOldRifle && !fNewRifle )
		{
			// Put it away!
			this->EVENT_InitNewSoldierAnim( LOWER_RIFLE, 0 , FALSE );
		}
		else if ( !fOldRifle && fNewRifle )
		{
			// Bring it up!
			this->EVENT_InitNewSoldierAnim( RAISE_RIFLE, 0 , FALSE );
		}
		else
		{
			SetSoldierAnimationSurface( this, this->usAnimState );
		}
		break;

	case ANIM_CROUCH:
	case ANIM_PRONE:

		SetSoldierAnimationSurface( this, this->usAnimState );
		break;
	}

}



UINT16 *CreateEnemyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen )
{
	UINT16 *p16BPPPalette, r16, g16, b16, usColor;
	UINT32 cnt;
	UINT32 rmod, gmod, bmod;
	UINT8	 r,g,b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *) MemAlloc( sizeof( UINT16 ) * 256 );

	for ( cnt = 0; cnt < 256; cnt++ )
	{
		gmod = (pPalette[ cnt ].peGreen);
		bmod = (pPalette[ cnt ].peBlue);

		rmod = __max( rscale, (pPalette[ cnt ].peRed) );

		if ( fAdjustGreen )
		{
			gmod = __max( gscale, (pPalette[ cnt ].peGreen) );
		}

		r = (UINT8)__min(rmod, 255);
		g = (UINT8)__min(gmod, 255);
		b = (UINT8)__min(bmod, 255);

		if(gusRedShift < 0)
			r16=((UINT16)r>>(-gusRedShift));
		else
			r16=((UINT16)r<<gusRedShift);

		if(gusGreenShift < 0)
			g16=((UINT16)g>>(-gusGreenShift));
		else
			g16=((UINT16)g<<gusGreenShift);


		if(gusBlueShift < 0)
			b16=((UINT16)b>>(-gusBlueShift));
		else
			b16=((UINT16)b<<gusBlueShift);

		// Prevent creation of pure black color
		usColor	= (r16&gusRedMask)|(g16&gusGreenMask)|(b16&gusBlueMask);

		if((usColor==0) && ((r+g+b)!=0))
			usColor=0x0001;

		p16BPPPalette[ cnt ] = usColor;
	}
	return( p16BPPPalette );
}


UINT16 *CreateEnemyGreyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen )
{
	UINT16 *p16BPPPalette, r16, g16, b16, usColor;
	UINT32 cnt, lumin;
	UINT32 rmod, gmod, bmod;
	UINT8	 r,g,b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *) MemAlloc( sizeof( UINT16 ) * 256 );

	for ( cnt = 0; cnt < 256; cnt++ )
	{
		lumin=(pPalette[ cnt ].peRed*299/1000)+ (pPalette[ cnt ].peGreen*587/1000)+(pPalette[ cnt ].peBlue*114/1000);
		rmod=(100*lumin)/256;
		gmod=(100*lumin)/256;
		bmod=(100*lumin)/256;



		rmod = __max( rscale, rmod );

		if ( fAdjustGreen )
		{
			gmod = __max( gscale, gmod );
		}


		r = (UINT8)__min(rmod, 255);
		g = (UINT8)__min(gmod, 255);
		b = (UINT8)__min(bmod, 255);

		if(gusRedShift < 0)
			r16=((UINT16)r>>(-gusRedShift));
		else
			r16=((UINT16)r<<gusRedShift);

		if(gusGreenShift < 0)
			g16=((UINT16)g>>(-gusGreenShift));
		else
			g16=((UINT16)g<<gusGreenShift);


		if(gusBlueShift < 0)
			b16=((UINT16)b>>(-gusBlueShift));
		else
			b16=((UINT16)b<<gusBlueShift);

		// Prevent creation of pure black color
		usColor	= (r16&gusRedMask)|(g16&gusGreenMask)|(b16&gusBlueMask);

		if((usColor==0) && ((r+g+b)!=0))
			usColor=0x0001;

		p16BPPPalette[ cnt ] = usColor;
	}
	return( p16BPPPalette );
}


void ContinueMercMovement( SOLDIERTYPE *pSoldier )
{
	INT16		sAPCost;
	INT16		sGridNo;

	sGridNo = pSoldier->pathing.sFinalDestination;

	// Can we afford this?
	if ( pSoldier->bGoodContPath )
	{
		sGridNo = pSoldier->sContPathLocation;
	}
	else
	{
		// ATE: OK, don't cancel count, so pending actions are still valid...
		pSoldier->aiData.ubPendingActionAnimCount = 0;
	}

	// get a path to dest...
	if ( FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, pSoldier->usUIMovementMode, NO_COPYROUTE, 0 ) )
	{
		sAPCost = PtsToMoveDirection( pSoldier, (UINT8)guiPathingData[ 0 ] );

		if ( EnoughPoints( pSoldier, sAPCost, 0 , (BOOLEAN)( pSoldier->bTeam == gbPlayerNum )  ) )
		{
			// Acknowledge
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );

				// If we have a face, tell text in it to go away!
				if ( pSoldier->iFaceIndex != -1 )
				{
					gFacesData[ pSoldier->iFaceIndex ].fDisplayTextOver = FACE_ERASE_TEXT_OVER;
				}
			}

			pSoldier->AdjustNoAPToFinishMove( FALSE );

			SetUIBusy( pSoldier->ubID );

			// OK, try and get a path to out dest!
			pSoldier->EVENT_InternalGetNewSoldierPath( sGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
		}
	}
}


BOOLEAN SOLDIERTYPE::CheckForBreathCollapse( void )
{
	// Check if we are out of breath!
	// Only check if > 70
	if ( this->bBreathMax > 70 )
	{
		if ( this->bBreath < 20 && !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LOW_BREATH ) &&
			gAnimControl[ this->usAnimState ].ubEndHeight == ANIM_STAND )
		{
			// WARN!
			TacticalCharacterDialogue( this, QUOTE_OUT_OF_BREATH );

			// Set flag indicating we were warned!
			this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_LOW_BREATH;
		}
	}

	// Check for drowing.....
	//if ( this->bBreath < 10 && !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_DROWNING ) && this->bOverTerrainType == DEEP_WATER )
	//{
	// WARN!
	//	TacticalCharacterDialogue( this, QUOTE_DROWNING );

	// Set flag indicating we were warned!
	//	this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_DROWNING;

	// WISDOM GAIN (25):  Starting to drown
	//  StatChange( this, WISDOMAMT, 25, FALSE );

	//}

	if ( this->bBreath == 0 && !this->bCollapsed && !( this->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_ANIMAL | SOLDIER_MONSTER ) ) )
	{
		// Collapse!
		// OK, Set a flag, because we may still be in the middle of an animation what is not interruptable...
		this->bBreathCollapsed = TRUE;

		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN SOLDIERTYPE::InternalIsValidStance( INT8 bDirection, INT8 bNewStance )
{
	UINT16								usOKToAddStructID=0;
	STRUCTURE_FILE_REF		*pStructureFileRef;
	UINT16								usAnimSurface=0;
	UINT16								usAnimState;

	// Check, if dest is prone, we can actually do this!

	// If we are a vehicle, we can only 'stand'
	if ( ( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) && bNewStance != ANIM_STAND )
	{
		return( FALSE );
	}

	// Check if we are in water?
	if ( this->MercInWater( ) )
	{
		if ( bNewStance == ANIM_PRONE || bNewStance == ANIM_CROUCH )
		{
			return( FALSE );
		}
	}

	if ( this->ubBodyType == ROBOTNOWEAPON && bNewStance != ANIM_STAND )
	{
		return( FALSE );
	}

	// Check if we are in water?
	if ( AM_AN_EPC( this ) )
	{
		if ( bNewStance == ANIM_PRONE )
		{
			return( FALSE );
		}
		else
		{
			return( TRUE );
		}
	}


	if ( this->bCollapsed )
	{
		//CHRISL: Changes from ADB rev 1475.
		if ( bNewStance == ANIM_CROUCH )
		{
			return( FALSE );
		}
		//when civilians are collapsed and die they may change to stand in order to fall forward
		if ( bNewStance == ANIM_STAND && this->ubBodyType <= REGFEMALE )
		{
			//if we are trying to stand and we are a MERC
			return( FALSE );
		}
	}

	// Check if we can do this....
	if ( this->pLevelNode && this->pLevelNode->pStructureData != NULL )
	{
		usOKToAddStructID = this->pLevelNode->pStructureData->usStructureID;
	}
	else
	{
		usOKToAddStructID = INVALID_STRUCTURE_ID;
	}

	switch( bNewStance )
	{
	case ANIM_STAND:

		usAnimState = STANDING;
		break;

	case ANIM_CROUCH:

		usAnimState = CROUCHING;
		break;


	case ANIM_PRONE:

		usAnimState = PRONE;
		break;

	default:

		// Something gone funny here....
		usAnimState = this->usAnimState;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Wrong desired stance given: %d, %d.", bNewStance, this->usAnimState );
	}

	usAnimSurface = DetermineSoldierAnimationSurface( this,  usAnimState );

	// Get structure ref........
	pStructureFileRef = GetAnimationStructureRef( this->ubID, usAnimSurface, usAnimState );

	if ( pStructureFileRef != NULL )
	{
		// Can we add structure data for this stance...?
		if ( !OkayToAddStructureToWorld( this->sGridNo, this->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ bDirection ]]), usOKToAddStructID ) )
		{
			return( FALSE );
		}
	}

	return( TRUE );
}


BOOLEAN IsValidStance( SOLDIERTYPE *pSoldier, INT8 bNewStance )
{
	return( pSoldier->InternalIsValidStance( pSoldier->ubDirection, bNewStance ) );
}


BOOLEAN IsValidMovementMode( SOLDIERTYPE *pSoldier, INT16 usMovementMode )
{
	// Check, if dest is prone, we can actually do this!

	// Check if we are in water?
	if ( pSoldier->MercInWater( ) )
	{
		if ( usMovementMode == RUNNING || usMovementMode == SWATTING || usMovementMode == CRAWLING )
		{
			return( FALSE );
		}
	}

	return( TRUE );
}


void SelectMoveAnimationFromStance( SOLDIERTYPE *pSoldier )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:
		pSoldier->EVENT_InitNewSoldierAnim( WALKING, 0 , FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( CRAWLING, 0 , FALSE );
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( SWATTING, 0 , FALSE );
		break;

	}

}


void GetActualSoldierAnimDims( SOLDIERTYPE *pSoldier, INT16 *psHeight, INT16 *psWidth )
{
	UINT16		usAnimSurface;
	ETRLEObject *pTrav;

	usAnimSurface = GetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

	if ( usAnimSurface == INVALID_ANIMATION_SURFACE )
	{
		*psHeight					= (INT16)5;
		*psWidth					= (INT16)5;

		return;
	}

	if ( gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject == NULL )
	{
		*psHeight					= (INT16)5;
		*psWidth					= (INT16)5;
		return;
	}

	// OK, noodle here on what we should do... If we take each frame, it will be different slightly
	// depending on the frame and the value returned here will vary thusly. However, for the
	// uses of this function, we should be able to use just the first frame...

	if ( pSoldier->usAniFrame >= gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->usNumberOfObjects )
	{
		//int i = 0;
		return;
	}

	pTrav = &(gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->pETRLEObject[ pSoldier->usAniFrame ] );

	*psHeight					= (INT16)pTrav->usHeight;
	*psWidth					= (INT16)pTrav->usWidth;
}

void GetActualSoldierAnimOffsets( SOLDIERTYPE *pSoldier, INT16 *sOffsetX, INT16 *sOffsetY )
{
	UINT16											 usAnimSurface;
	ETRLEObject *pTrav;

	usAnimSurface = GetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

	if ( usAnimSurface == INVALID_ANIMATION_SURFACE )
	{
		*sOffsetX					= (INT16)0;
		*sOffsetY					= (INT16)0;

		return;
	}

	if ( gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject == NULL )
	{
		*sOffsetX					= (INT16)0;
		*sOffsetY					= (INT16)0;
		return;
	}

	pTrav = &(gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->pETRLEObject[ pSoldier->usAniFrame ] );

	*sOffsetX					= (INT16)pTrav->sOffsetX;
	*sOffsetY					= (INT16)pTrav->sOffsetY;
}


void SetSoldierLocatorOffsets( SOLDIERTYPE *pSoldier )
{
	INT16 sHeight, sWidth;
	INT16 sOffsetX, sOffsetY;


	// OK, from our animation, get height, width
	GetActualSoldierAnimDims( pSoldier, &sHeight, &sWidth );
	GetActualSoldierAnimOffsets( pSoldier, &sOffsetX, &sOffsetY );

	// OK, here, use the difference between center of animation ( sWidth/2 ) and our offset!
	//pSoldier->sLocatorOffX = ( abs( sOffsetX ) ) - ( sWidth / 2 );

	pSoldier->sBoundingBoxWidth		= sWidth;
	pSoldier->sBoundingBoxHeight  = sHeight;
	pSoldier->sBoundingBoxOffsetX	= sOffsetX;
	pSoldier->sBoundingBoxOffsetY	= sOffsetY;

}

BOOLEAN SOLDIERTYPE::SoldierCarriesTwoHandedWeapon( void )
{
	UINT16 usItem;

	usItem = this->inv[ HANDPOS ].usItem;

	if ( this->inv[ HANDPOS ].exists() == true && (Item[ usItem ].twohanded ) )
	{
		return( TRUE );
	}

	return( FALSE );

}



INT32 CheckBleeding( SOLDIERTYPE *pSoldier )
{
	INT8		bBandaged; //,savedOurTurn;
	INT32	iBlood = NOBLOOD;

	if ( pSoldier->stats.bLife != 0 )
	{
		// if merc is hurt beyond the minimum required to bleed, or he's dying
		if ( ( pSoldier->bBleeding > MIN_BLEEDING_THRESHOLD) || pSoldier->stats.bLife < OKLIFE )
		{
			// if he's NOT in the process of being bandaged or DOCTORed
			if ( ( pSoldier->ubServiceCount == 0 ) && ( AnyDoctorWhoCanHealThisPatient( pSoldier, HEALABLE_EVER ) == NULL ) )
			{
				// may drop blood whether or not any bleeding takes place this turn
				if ( pSoldier->bTilesMoved < 1 )
			 {
				 iBlood = ( ( pSoldier->bBleeding - MIN_BLEEDING_THRESHOLD ) / BLOODDIVISOR ); // + pSoldier->dying;
				 if ( iBlood > MAXBLOODQUANTITY )
				 {
					 iBlood = MAXBLOODQUANTITY;
				 }
			 }
				else
			 {
				 iBlood = NOBLOOD;
			 }

				// Are we in a different mode?
				if ( !(gTacticalStatus.uiFlags & TURNBASED ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
			 {
				 pSoldier->dNextBleed -= (FLOAT)RT_NEXT_BLEED_MODIFIER;
			 }
				else
			 {
				 // Do a single step descrease
				 pSoldier->dNextBleed--;
			 }

				// if it's time to lose some blood
				if ( pSoldier->dNextBleed <= 0)
			 {
				 // first, calculate if soldier is bandaged
				 bBandaged = pSoldier->stats.bLifeMax - pSoldier->bBleeding - pSoldier->stats.bLife;

				 // as long as he's bandaged and not "dying"
				 if ( bBandaged && pSoldier->stats.bLife >= OKLIFE )
				 {
					 // just bleeding through existing bandages
					 pSoldier->bBleeding++;

					 SoldierBleed( pSoldier, TRUE );
				 }
				 else	// soldier is either not bandaged at all or is dying
				 {
					 if ( pSoldier->stats.bLife < OKLIFE )		// if he's dying
					 {
						 // if he's conscious, and he hasn't already, say his "dying quote"
						 if ( ( pSoldier->stats.bLife >= CONSCIOUSNESS ) && !pSoldier->flags.fDyingComment )
						 {
							 TacticalCharacterDialogue( pSoldier, QUOTE_SERIOUSLY_WOUNDED );

							 pSoldier->flags.fDyingComment = TRUE;
						 }

						 // can't permit lifemax to ever bleed beneath OKLIFE, or that
						 // soldier might as well be dead!
						 if (pSoldier->stats.bLifeMax >= OKLIFE)
							{
								// bleeding while "dying" costs a PERMANENT point of life each time!
								pSoldier->stats.bLifeMax--;
								pSoldier->bBleeding--;
							}
						}
					}

				 // either way, a point of life (health) is lost because of bleeding
				 // This will also update the life bar

				 SoldierBleed( pSoldier, FALSE );


				 // if he's not dying (which includes him saying the dying quote just
				 // now), and he hasn't warned us that he's bleeding yet, he does so
				 // Also, not if they are being bandaged....
				 if ( ( pSoldier->stats.bLife >= OKLIFE ) && !pSoldier->flags.fDyingComment && !pSoldier->flags.fWarnedAboutBleeding && !gTacticalStatus.fAutoBandageMode && pSoldier->ubServiceCount == 0 )
					{
						TacticalCharacterDialogue( pSoldier, QUOTE_STARTING_TO_BLEED );

						// "starting to bleed" quote
						pSoldier->flags.fWarnedAboutBleeding = TRUE;
					}

				 pSoldier->dNextBleed = CalcSoldierNextBleed( pSoldier );

				}
			}
		}
	}
	return( iBlood );
}


void SoldierBleed( SOLDIERTYPE *pSoldier, BOOLEAN fBandagedBleed )
{
	INT8 bOldLife;

	// OK, here make some stuff happen for bleeding
	// A banaged bleed does not show damage taken , just through existing bandages

	// ATE: Do this ONLY if buddy is in sector.....
	if ( ( pSoldier->bInSector && guiCurrentScreen == GAME_SCREEN ) || guiCurrentScreen != GAME_SCREEN )
	{
		pSoldier->flags.fFlashPortrait = TRUE;
		pSoldier->bFlashPortraitFrame = FLASH_PORTRAIT_STARTSHADE;
		RESETTIMECOUNTER( pSoldier->timeCounters.PortraitFlashCounter, FLASH_PORTRAIT_DELAY );

		// If we are in mapscreen, set this person as selected
		if ( guiCurrentScreen == MAP_SCREEN )
		{
			SetInfoChar( pSoldier->ubID );
		}
	}

	bOldLife = pSoldier->stats.bLife;

	// If we are already dead, don't show damage!
	if ( !fBandagedBleed )
	{
		pSoldier->SoldierTakeDamage( ANIM_CROUCH, 1, 100, TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, TRUE );
	}

}


void SoldierCollapse( SOLDIERTYPE *pSoldier )
{
	BOOLEAN fMerc = FALSE;

	if ( pSoldier->ubBodyType <= REGFEMALE )
	{
		fMerc = TRUE;
	}

	// If we are an animal, etc, don't do anything....
	switch( pSoldier->ubBodyType )
	{
	case ADULTFEMALEMONSTER:
	case AM_MONSTER:
	case YAF_MONSTER:
	case YAM_MONSTER:
	case LARVAE_MONSTER:
	case INFANT_MONSTER:
	case QUEENMONSTER:

		// Give breath back....
		DeductPoints( pSoldier,0, (INT16)-5000 );
		return;
		break;
	}

	pSoldier->bCollapsed			 = TRUE;

	pSoldier->ReceivingSoldierCancelServices( );

	// CC has requested - handle sight here...
	HandleSight( pSoldier, SIGHT_LOOK );

	// Check height
	switch( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
	{
	case ANIM_STAND:

		if ( pSoldier->MercInDeepWater( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( DEEP_WATER_DIE, 0, FALSE );
		}
		else if ( pSoldier->MercInShallowWater( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( WATER_DIE, 0, FALSE );
		}
		else
		{
			pSoldier->BeginTyingToFall( );
			if(!is_networked)pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
			else pSoldier->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		}
		break;

	case ANIM_CROUCH:

		// Crouched or prone, only for mercs!
		pSoldier->BeginTyingToFall( );

		// 0verhaul:  No special case here!  First the FALLFORWARD_FROMHIT_CROUCH can be filled in to use the FALLFORWARD_FROMHIT_STAND anim
		// then when real anims come, use them instead.
		//if ( fMerc )
		//{
			if(!is_networked)pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_CROUCH, 0 , FALSE);
			else pSoldier->ChangeSoldierState( FALLFORWARD_FROMHIT_CROUCH, 0, FALSE );
		//}
		//else
		//{
		//	// For civs... use fall from stand...
		//	pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0 , FALSE);
		//}
		break;

	case ANIM_PRONE:

		switch( pSoldier->usAnimState )
		{
		case FALLFORWARD_FROMHIT_STAND:
		case ENDFALLFORWARD_FROMHIT_CROUCH:

			pSoldier->ChangeSoldierState( STAND_FALLFORWARD_STOP, 0, FALSE );
			break;

		case FALLBACK_HIT_STAND:
			pSoldier->ChangeSoldierState( FALLBACKHIT_STOP, 0, FALSE );
			break;

		default:
			if(!is_networked)pSoldier->EVENT_InitNewSoldierAnim( PRONE_LAY_FROMHIT, 0 , FALSE );
			else pSoldier->ChangeSoldierState( PRONE_LAY_FROMHIT, 0, FALSE );
			break;
		}
		break;
	}

	if (pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY)
	{

		if ( !(gTacticalStatus.bPanicTriggerIsAlarm) && (gTacticalStatus.ubTheChosenOne == pSoldier->ubID) )
		{
			// replace this guy as the chosen one!
			gTacticalStatus.ubTheChosenOne = NOBODY;
			MakeClosestEnemyChosenOne();
		}

		if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && (pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL))
		{
#ifdef TESTAICONTROL
			DebugAI( String("Ending turn for %d because of error from HandleItem", pSoldier->ubID ) );
#endif

			EndAIGuysTurn( pSoldier );
		}
	}

	// DON'T DE-SELECT GUY.....
	//else
	//{
	// Check if this is our selected guy...
	//	if ( pSoldier->ubID == gusSelectedSoldier )
	//	{
	//		SelectNextAvailSoldier( pSoldier );
	//		}
	//}
}


FLOAT CalcSoldierNextBleed( SOLDIERTYPE *pSoldier )
{
	INT8		bBandaged;

	// calculate how many turns before he bleeds again
	// bleeding faster the lower life gets, and if merc is running around
	//pSoldier->nextbleed = 2 + (pSoldier->life / (10 + pSoldier->tilesMoved));  // min = 2

	// if bandaged, give 1/2 of the bandaged life points back into equation
	bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	return( (FLOAT)1 + (FLOAT)( (pSoldier->stats.bLife + bBandaged / 2) / (10 + pSoldier->bTilesMoved) ) );  // min = 1
}

FLOAT CalcSoldierNextUnmovingBleed( SOLDIERTYPE *pSoldier )
{
	INT8		bBandaged;

	// calculate bleeding rate without the penalty for tiles moved

	// if bandaged, give 1/2 of the bandaged life points back into equation
	bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	return( (FLOAT)1 + (FLOAT)( (pSoldier->stats.bLife + bBandaged / 2) / 10 ) );  // min = 1
}

void HandlePlacingRoofMarker( SOLDIERTYPE *pSoldier, INT16 sGridNo, BOOLEAN fSet, BOOLEAN fForce )
{
	LEVELNODE *pRoofNode;
	LEVELNODE *pNode;

	if ( pSoldier->bVisible == -1 && fSet )
	{
		return;
	}

	if ( pSoldier->bTeam != gbPlayerNum )
	{
		//return;
	}

	// If we are on the roof, add roof UI peice!
	if ( pSoldier->pathing.bLevel == SECOND_LEVEL )
	{
		// Get roof node
		pRoofNode = gpWorldLevelData[ sGridNo ].pRoofHead;

		// Return if we are still climbing roof....
		if ( pSoldier->usAnimState == CLIMBUPROOF && !fForce )
		{
			return;
		}

		if ( pRoofNode != NULL )
		{
			if ( fSet )
			{
				if ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED )
				{
					// Set some flags on this poor thing
					//pRoofNode->uiFlags |= ( LEVELNODE_USEBESTTRANSTYPE | LEVELNODE_REVEAL | LEVELNODE_DYNAMIC  );
					//pRoofNode->uiFlags |= ( LEVELNODE_DYNAMIC );
					//pRoofNode->uiFlags &= ( ~LEVELNODE_HIDDEN );
					//ResetSpecificLayerOptimizing( TILES_DYNAMIC_ROOF );
				}
			}
			else
			{
				if ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED )
				{
					// Remove some flags on this poor thing
					//pRoofNode->uiFlags &= ~( LEVELNODE_USEBESTTRANSTYPE | LEVELNODE_REVEAL | LEVELNODE_DYNAMIC );

					//pRoofNode->uiFlags |= LEVELNODE_HIDDEN;
				}
			}

			if ( fSet )
			{
				// If it does not exist already....
				if ( !IndexExistsInRoofLayer( sGridNo, FIRSTPOINTERS11 ) )
				{
					pNode = AddRoofToTail( sGridNo, FIRSTPOINTERS11 );
					pNode->ubShadeLevel				=DEFAULT_SHADE_LEVEL;
					pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
				}
			}
			else
			{
				RemoveRoof( sGridNo, FIRSTPOINTERS11 );
			}
		}
	}
}

void SOLDIERTYPE::PositionSoldierLight( void )
{
	// DO ONLY IF WE'RE AT A GOOD LEVEL
	if ( ubAmbientLightLevel < MIN_AMB_LEVEL_FOR_MERC_LIGHTS )
	{
		return;
	}

	if ( !this->bInSector )
	{
		return;
	}

	if ( this->bTeam != gbPlayerNum )
	{
		return;
	}

	if ( this->stats.bLife < OKLIFE )
	{
		return;
	}

	//if the player DOESNT want the merc to cast light
	if( !gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] )
	{
		return;
	}

	if ( this->iLight == -1 )
	{
		this->CreateSoldierLight( );
	}

	//if ( this->ubID == gusSelectedSoldier )
	{
		LightSpritePower(this->iLight, TRUE);
		LightSpriteFake(this->iLight);

		LightSpritePosition( this->iLight, (INT16)(this->sX/CELL_X_SIZE), (INT16)(this->sY/CELL_Y_SIZE));
	}
}

void SOLDIERTYPE::SetCheckSoldierLightFlag( )
{
	this->PositionSoldierLight( );
	//this->flags.uiStatusFlags |= SOLDIER_RECHECKLIGHT;
}


void PickPickupAnimation( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT16 sGridNo, INT8 bZLevel )
{
	INT8				bDirection;
	STRUCTURE		*pStructure;
	BOOLEAN			fDoNormalPickup = TRUE;


	// OK, Given the gridno, determine if it's the same one or different....
	if ( sGridNo != pSoldier->sGridNo )
	{
		// Get direction to face....
		bDirection = (INT8)GetDirectionFromGridNo( sGridNo, pSoldier );
		pSoldier->ubPendingDirection = bDirection;

		// Change to pickup animation
		pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM, 0 , FALSE );

		if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
		{
			// set "pending action" value for AI so it will wait
			pSoldier->aiData.bAction = AI_ACTION_PENDING_ACTION;
		}

	}
	else
	{
		// If in water....
		if ( pSoldier->MercInWater( ) )
		{
			UnSetUIBusy( pSoldier->ubID );
			HandleSoldierPickupItem( pSoldier, iItemIndex, sGridNo, bZLevel );
			pSoldier->SoldierGotoStationaryStance( );
			if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
			{
				// reset action value for AI because we're done!
				ActionDone( pSoldier );
			}

		}
		else
		{
			// Don't show animation of getting item, if we are not standing
			switch ( gAnimControl[ pSoldier->usAnimState ].ubHeight )
			{
			case ANIM_STAND:

				// OK, if we are looking at z-level >0, AND
				// we have a strucxture with items in it
				// look for orientation and use angle accordingly....
				if ( bZLevel > 0 )
				{
					//#if 0
					// Get direction to face....
					if ( ( pStructure = FindStructure( (INT16)sGridNo, ( STRUCTURE_HASITEMONTOP | STRUCTURE_OPENABLE ) ) ) != NULL )
					{
						fDoNormalPickup = FALSE;

						// OK, look at orientation
						switch( pStructure->ubWallOrientation )
						{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:

							bDirection = (INT8)NORTH;
							break;

						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:

							bDirection = (INT8)WEST;
							break;

						default:

							bDirection = pSoldier->ubDirection;
							break;
						}

						//pSoldier->ubPendingDirection = bDirection;
						pSoldier->EVENT_SetSoldierDesiredDirection( bDirection );
						pSoldier->EVENT_SetSoldierDirection( bDirection );

						// Change to pickup animation
						pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM, 0 , FALSE );
					}
					//#endif
				}

				if ( fDoNormalPickup )
				{
					pSoldier->EVENT_InitNewSoldierAnim( PICKUP_ITEM, 0 , FALSE );
				}

				if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
				{
					// set "pending action" value for AI so it will wait
					pSoldier->aiData.bAction = AI_ACTION_PENDING_ACTION;
				}
				break;

			case ANIM_CROUCH:
			case ANIM_PRONE:

				UnSetUIBusy( pSoldier->ubID );
				HandleSoldierPickupItem( pSoldier, iItemIndex, sGridNo, bZLevel );
				pSoldier->SoldierGotoStationaryStance( );
				if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
				{
					// reset action value for AI because we're done!
					ActionDone( pSoldier );
				}
				break;
			}
		}
	}
}

void SOLDIERTYPE::PickDropItemAnimation( void )
{
	// Don't show animation of getting item, if we are not standing
	switch ( gAnimControl[ this->usAnimState ].ubHeight )
	{
	case ANIM_STAND:

		this->aiData.bAction = AI_ACTION_PENDING_ACTION;
		this->EVENT_InitNewSoldierAnim( DROP_ITEM, 0 , FALSE );
		break;

	case ANIM_CROUCH:
	case ANIM_PRONE:

		SoldierHandleDropItem( this );
		this->SoldierGotoStationaryStance( );
		ActionDone(this);
		break;
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginCutFence( INT16 sGridNo, UINT8 ubDirection )
{
	// Make sure we have a structure here....
	if ( IsCuttableWireFenceAtGridNo( sGridNo ) )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		//BOOLEAN CutWireFence( INT16 sGridNo )

		// SET TARGET GRIDNO
		this->sTargetGridNo = sGridNo;

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( CUTTING_FENCE, 0 , FALSE );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginRepair( INT16 sGridNo, UINT8 ubDirection )
{
	INT8 bRepairItem;
	UINT8	ubID;

	// Make sure we have a structure here....
	bRepairItem = IsRepairableStructAtGridNo( sGridNo, &ubID );

	if ( bRepairItem )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		//BOOLEAN CutWireFence( INT16 sGridNo )

		// SET TARGET GRIDNO
		//this->sTargetGridNo = sGridNo;

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( GOTO_REPAIRMAN, 0 , FALSE );
		// SET BUDDY'S ASSIGNMENT TO REPAIR...

		// Are we a SAM site? ( 3 == SAM )
		if ( bRepairItem == 3 )
		{
			SetSoldierAssignment( this, REPAIR, TRUE, FALSE, -1 );
		}
		else if ( bRepairItem == 2 ) // ( 2 == VEHICLE )
		{
			SetSoldierAssignment( this, REPAIR, FALSE, FALSE, ubID );
		}

	}
}

void SOLDIERTYPE::EVENT_SoldierBeginRefuel( INT16 sGridNo, UINT8 ubDirection )
{
	INT8 bRefuelItem;
	UINT8	ubID;

	// Make sure we have a structure here....
	bRefuelItem = IsRefuelableStructAtGridNo( sGridNo, &ubID );

	if ( bRefuelItem )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		//BOOLEAN CutWireFence( INT16 sGridNo )

		// SET TARGET GRIDNO
		//this->sTargetGridNo = sGridNo;

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( REFUEL_VEHICLE, 0 , FALSE );
		// SET BUDDY'S ASSIGNMENT TO REPAIR...
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginTakeBlood( INT16 sGridNo, UINT8 ubDirection )
{
	ROTTING_CORPSE *pCorpse;


	// See if these is a corpse here....
	pCorpse = GetCorpseAtGridNo( sGridNo , this->pathing.bLevel );

	if ( pCorpse != NULL )
	{
		this->aiData.uiPendingActionData4 = pCorpse->iID;

		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		this->EVENT_InitNewSoldierAnim( TAKE_BLOOD_FROM_CORPSE, 0 , FALSE );
	}
	else
	{
		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginAttachCan( INT16 sGridNo, UINT8 ubDirection )
{
	STRUCTURE *			pStructure;
	DOOR_STATUS *		pDoorStatus;

	// OK, find door, attach to door, do animation...., remove item....

	// First make sure we still have item in hand....
	if ( !Item[this->inv[ HANDPOS ].usItem].canandstring )
	{
		return;
	}

	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );

	if ( pStructure == NULL )
	{
		return;
	}

	// Modify door status to make sure one is created for this door
	// Use the current door state for this
	if ( !(pStructure->fFlags & STRUCTURE_OPEN) )
	{
		ModifyDoorStatus( sGridNo, FALSE, FALSE );
	}
	else
	{
		ModifyDoorStatus( sGridNo, TRUE, TRUE );
	}

	// Now get door status...
	pDoorStatus = GetDoorStatus( sGridNo );
	if ( pDoorStatus == NULL )
	{
		// SOmething wrong here...
		return;
	}

	// OK set flag!
	pDoorStatus->ubFlags |= DOOR_HAS_TIN_CAN;

	// Do animation
	this->EVENT_SetSoldierDesiredDirection( ubDirection );
	this->EVENT_SetSoldierDirection( ubDirection );

	this->EVENT_InitNewSoldierAnim( ATTACH_CAN_TO_STRING, 0 , FALSE );

	// Remove item...
	this->inv[ HANDPOS ].RemoveObjectsFromStack(1);
	fInterfacePanelDirty = DIRTYLEVEL2;

}


void SOLDIERTYPE::EVENT_SoldierBeginReloadRobot( INT16 sGridNo, UINT8 ubDirection, UINT8 ubMercSlot )
{
	UINT8 ubPerson;

	// Make sure we have a robot here....
	ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY && MercPtrs[ ubPerson ]->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		this->EVENT_SetSoldierDesiredDirection( ubDirection );
		this->EVENT_SetSoldierDirection( ubDirection );

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( RELOAD_ROBOT, 0 , FALSE );

	}
}



void SOLDIERTYPE::ResetSoldierChangeStatTimer( void )
{
	this->timeChanges.uiChangeLevelTime = 0;
	this->timeChanges.uiChangeHealthTime = 0;
	this->timeChanges.uiChangeStrengthTime= 0;
	this->timeChanges.uiChangeDexterityTime= 0;
	this->timeChanges.uiChangeAgilityTime= 0;
	this->timeChanges.uiChangeWisdomTime= 0;
	this->timeChanges.uiChangeLeadershipTime= 0;
	this->timeChanges.uiChangeMarksmanshipTime= 0;
	this->timeChanges.uiChangeExplosivesTime= 0;
	this->timeChanges.uiChangeMedicalTime= 0;
	this->timeChanges.uiChangeMechanicalTime= 0;


	return;
}


void SOLDIERTYPE::ChangeToFlybackAnimation( UINT8 flyBackDirection )
{
	UINT8 ubOppositeDir;
	INT16 sDirectionInc;
	INT16 sNewGridNo;

	ubOppositeDir = gOppositeDirection[ flyBackDirection ];
	sDirectionInc = DirectionInc( ubOppositeDir);

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( this->sGridNo, sDirectionInc );
	if ( gubWorldMovementCosts[ sNewGridNo ][ ubOppositeDir ][ this->pathing.bLevel ] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Pretend we hit the wall and fall forward instead
		this->BeginTyingToFall();
		this->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		return;
	}

	sNewGridNo = NewGridNo( sNewGridNo, sDirectionInc );
	if ( gubWorldMovementCosts[ sNewGridNo ][ ubOppositeDir ][ this->pathing.bLevel ] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Fall back instead
		this->BeginTyingToFall();
		this->ChangeSoldierState( FALLBACK_HIT_STAND, 0, FALSE );
	}


	// Remove any previous actions
	this->aiData.ubPendingAction		 = NO_PENDING_ACTION;

	this->sPlotSrcGrid = this->sGridNo;

	// Since we're manually setting our path, we have to reset these @#$@# flags too.  Otherwise we don't reach the
	// destination a lot of the time
	this->flags.fPastXDest = 0;
	this->flags.fPastYDest = 0;

	// Set path....
	this->pathing.usPathDataSize = 0;
	this->pathing.usPathIndex    = 0;
	this->pathing.usPathingData[ this->pathing.usPathDataSize ] = gOppositeDirection[ this->ubDirection ];
	this->pathing.usPathDataSize++;
	this->pathing.usPathingData[ this->pathing.usPathDataSize ] = gOppositeDirection[ this->ubDirection ];
	this->pathing.usPathDataSize++;
	this->pathing.sFinalDestination = sNewGridNo;
	this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[ this->pathing.usPathIndex ], FALSE, FLYBACK_HIT );

	// Get a new direction based on direction
	this->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0 , FALSE );
}

void SOLDIERTYPE::ChangeToFallbackAnimation( UINT8 fallBackDirection )
{
	INT16 sNewGridNo;
	UINT8 ubOppositeDir;
	INT16 sDirection;

	ubOppositeDir = gOppositeDirection[ fallBackDirection ];
	sDirection = DirectionInc( ubOppositeDir);

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( this->sGridNo, sDirection );
	if ( gubWorldMovementCosts[ sNewGridNo ][ ubOppositeDir ][ this->pathing.bLevel ] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Pretend we hit the wall and fall forward instead
		this->BeginTyingToFall();
		this->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		return;
	}

	// Get dest gridno, convert to center coords
	//sNewGridNo = NewGridNo( sNewGridNo, sDirection );
	//usNewGridNo = NewGridNo( (UINT16)usNewGridNo, (UINT16)(-1 * DirectionInc( bDirection ) ) );

	// Remove any previous actions
	this->aiData.ubPendingAction		 = NO_PENDING_ACTION;

	this->sPlotSrcGrid = this->sGridNo;

	// Since we're manually setting our path, we have to reset these @#$@# flags too.  Otherwise we don't reach the
	// destination a lot of the time
	this->flags.fPastXDest = 0;
	this->flags.fPastYDest = 0;

	// Set path....
	this->pathing.usPathDataSize = 0;
	this->pathing.usPathIndex    = 0;
	this->pathing.usPathingData[ this->pathing.usPathDataSize ] = gOppositeDirection[ this->ubDirection ];
	this->pathing.usPathDataSize++;
	this->pathing.sFinalDestination = sNewGridNo;
	this->EVENT_InternalSetSoldierDestination( this->pathing.usPathingData[ this->pathing.usPathIndex ], FALSE, FALLBACK_HIT_STAND );

	// Get a new direction based on direction
	this->EVENT_InitNewSoldierAnim( FALLBACK_HIT_STAND, 0 , FALSE );
}


void SOLDIERTYPE::SetSoldierCowerState( BOOLEAN fOn )
{
	// Robot's don't cower!
	if ( this->ubBodyType == ROBOTNOWEAPON )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("ERROR: Robot was told to cower!" ) );
		return;
	}

	// OK< set flag and do anim...
	if ( fOn )
	{
		if ( !( this->flags.uiStatusFlags & SOLDIER_COWERING ) )
		{
			this->EVENT_InitNewSoldierAnim( START_COWER, 0, FALSE );

			this->flags.uiStatusFlags |= SOLDIER_COWERING;

			this->ubDesiredHeight = ANIM_CROUCH;
		}
	}
	else
	{
		if ( (this->flags.uiStatusFlags & SOLDIER_COWERING) || (gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_STAND) )
		{
			this->EVENT_InitNewSoldierAnim( END_COWER, 0, FALSE );

			this->flags.uiStatusFlags &= (~SOLDIER_COWERING );

			this->ubDesiredHeight = ANIM_STAND;
		}
	}
}

void MercStealFromMerc( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget )
{
	INT16 sActionGridNo, sGridNo, sAdjustedGridNo;
	UINT8	ubDirection;


	// OK, find an adjacent gridno....
	sGridNo = pTarget->sGridNo;

	// See if we can get there to punch
	sActionGridNo =  FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
	if ( sActionGridNo != -1 )
	{
		// SEND PENDING ACTION
		pSoldier->aiData.ubPendingAction = MERC_STEAL;
		pSoldier->bTargetLevel = pTarget->pathing.bLevel; // Overhaul:  Update the level too!
		pSoldier->aiData.sPendingActionData2  = pTarget->sGridNo;
		pSoldier->aiData.bPendingActionData3  = ubDirection;
		pSoldier->aiData.ubPendingActionAnimCount = 0;

		// CHECK IF WE ARE AT THIS GRIDNO NOW
		if ( pSoldier->sGridNo != sActionGridNo )
		{
			// WALK UP TO DEST FIRST
			SendGetNewSoldierPathEvent( pSoldier, sActionGridNo, pSoldier->usUIMovementMode );
		}
		else
		{
			pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
			pSoldier->EVENT_InitNewSoldierAnim( STEAL_ITEM, 0 , FALSE );
		}

		// OK, set UI
//		gTacticalStatus.ubAttackBusyCount++;
		// reset attacking item (hand)
		pSoldier->usAttackingWeapon = 0;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting STEAL attack, attack count now %d", gTacticalStatus.ubAttackBusyCount) );
		DebugAttackBusy( String( "!!!!!!! Starting STEAL attack, attack count now %d\n", gTacticalStatus.ubAttackBusyCount ) );

		SetUIBusy( pSoldier->ubID );
	}
}

SOLDIERTYPE				*pTMilitiaSoldier;//global pointer
BOOLEAN SOLDIERTYPE::PlayerSoldierStartTalking( UINT8 ubTargetID, BOOLEAN fValidate )
{
	INT16							sFacingDir, sXPos, sYPos, sAPCost;
	SOLDIERTYPE				*pTSoldier;
	UINT32						uiRange;

	if ( ubTargetID == NOBODY )
	{
		return( FALSE );
	}

	pTSoldier = MercPtrs[ ubTargetID ];

	// Check distance again, to be sure
	if ( fValidate )
	{
		// OK, since we locked this guy from moving
		// we should be close enough, so talk ( unless he is now dead )
		if ( !IsValidTalkableNPC( (UINT8)ubTargetID, FALSE, FALSE, FALSE ) )
		{
			return( FALSE );
		}

		uiRange = GetRangeFromGridNoDiff( this->sGridNo, pTSoldier->sGridNo );

		if ( uiRange > ( NPC_TALK_RADIUS * 2 ) )
		{
			// Todo here - should we follow dude?
			return( FALSE );
		}


	}

	// Get APs...
	sAPCost = AP_TALK;

	if ( !(gTacticalStatus.uiFlags & INCOMBAT) || (gTacticalStatus.uiFlags & REALTIME) ) //lal
	{
		ConvertGridNoToXY( pTSoldier->sGridNo, &sXPos, &sYPos );

		// Get direction from mouse pos
		sFacingDir = GetDirectionFromXY( sXPos, sYPos, this );

		// Set our guy facing
		SendSoldierSetDesiredDirectionEvent( this, sFacingDir );

		// Set NPC facing
		SendSoldierSetDesiredDirectionEvent( pTSoldier, gOppositeDirection[ sFacingDir ] );

		// Stop our guys...
		this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
	}

	pTMilitiaSoldier = pTSoldier; //lal

	//lal
	// ATE; Check for normal civs...
	if ( GetCivType( pTSoldier ) != CIV_TYPE_NA )
	{
		//lal
		if ( ( pTSoldier->bTeam == MILITIA_TEAM ) && ( gGameExternalOptions.fAllowTacticalMilitiaCommand == TRUE ) && (this->bSide == pTSoldier->bSide) )
		{
			PopupMilitiaControlMenu( pTSoldier );
			return( FALSE );
		}
		else
		{
			// Deduct points from our guy....
			DeductPoints( this, sAPCost, 0 );

			StartCivQuote( pTSoldier );
			return( FALSE );
		}
	}




	// Are we an EPC that is being escorted?
	if ( pTSoldier->ubProfile != NO_PROFILE && pTSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		return( InitiateConversation( pTSoldier, this, APPROACH_EPC_WHO_IS_RECRUITED, 0 ) );
		//Converse( pTSoldier->ubProfile, this->ubProfile, APPROACH_EPC_WHO_IS_RECRUITED, 0 );
	}
	else if (pTSoldier->aiData.bNeutral)
	{
		switch( pTSoldier->ubProfile )
		{
		case JIM:
		case JACK:
		case OLAF:
		case RAY:
		case OLGA:
		case TYRONE:
			// Start combat etc
			DeleteTalkingMenu();
			CancelAIAction( pTSoldier, TRUE );
			AddToShouldBecomeHostileOrSayQuoteList( pTSoldier->ubID );
			break;
		default:
			// Start talking!
			return( InitiateConversation( pTSoldier, this, NPC_INITIAL_QUOTE, 0 ) );
			break;
		}
	}
	else
	{
		// Start talking with hostile NPC
		return( InitiateConversation( pTSoldier, this, APPROACH_ENEMY_NPC_QUOTE, 0 ) );
	}

	return( TRUE );
}


BOOLEAN SOLDIERTYPE::IsValidSecondHandShot( void )
{
	if ( Item[ this->inv[ SECONDHANDPOS ].usItem ].usItemClass == IC_GUN &&
		!(Item[ this->inv[SECONDHANDPOS ].usItem ].twohanded ) &&
		!this->bDoBurst &&
		!Item[this->inv[ HANDPOS ].usItem].grenadelauncher &&
		Item[ this->inv[HANDPOS].usItem ].usItemClass == IC_GUN &&
		this->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE &&
		this->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0 )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN SOLDIERTYPE::IsValidSecondHandShotForReloadingPurposes( void )
{
	// should be maintained as same as function above with line
	// about ammo taken out!
	if ( Item[ this->inv[ SECONDHANDPOS ].usItem ].usItemClass == IC_GUN &&
		!this->bDoBurst &&
		!Item[this->inv[ HANDPOS ].usItem].grenadelauncher &&
		Item[ this->inv[HANDPOS].usItem ].usItemClass == IC_GUN &&
		this->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE //&&
		//			 this->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0 &&
		//			 gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_PRONE )
		)
	{
		return( TRUE );
	}

	return( FALSE );
}



BOOLEAN SOLDIERTYPE::CanRobotBeControlled( void )
{
	SOLDIERTYPE *pController;

	if ( !( this->flags.uiStatusFlags & SOLDIER_ROBOT ) )
	{
		return( FALSE );
	}

	if ( this->ubRobotRemoteHolderID == NOBODY )
	{
		return( FALSE );
	}

	pController = MercPtrs[ this->ubRobotRemoteHolderID ];

	if ( pController->bActive )
	{
		if ( pController->ControllingRobot(  ) )
		{
			// ALL'S OK!
			return( TRUE );
		}
	}

	return( FALSE );
}


BOOLEAN SOLDIERTYPE::ControllingRobot( void )
{
	SOLDIERTYPE *pRobot;
	INT8				bPos;

	if ( !this->bActive )
	{
		return( FALSE );
	}

	// EPCs can't control the robot (no inventory to hold remote, for one)
	if ( AM_AN_EPC( this ) )
	{
		return( FALSE );
	}

	// Don't require this->bInSector here, it must work from mapscreen!

	// are we in ok shape?
	if ( this->stats.bLife < OKLIFE || ( this->bTeam != gbPlayerNum ) )
	{
		return( FALSE );
	}

	// allow control from within vehicles - allows strategic travel in a vehicle with robot!
	if ( ( this->bAssignment >= ON_DUTY ) && ( this->bAssignment != VEHICLE ) )
	{
		return( FALSE );
	}

	// is the soldier wearing a robot remote control?
	bPos = FindRemoteControl( this );
	if ( bPos == NO_SLOT )
	{
		return( FALSE );
	}

	// Find the robot
	pRobot = FindSoldierByProfileID( ROBOT, TRUE );
	if ( !pRobot )
	{
		return( FALSE );
	}

	if ( pRobot->bActive )
	{
		// Are we in the same sector....?
		// ARM: CHANGED TO WORK IN MAPSCREEN, DON'T USE WorldSector HERE
		if ( pRobot->sSectorX == this->sSectorX &&
			pRobot->sSectorY == this->sSectorY &&
			pRobot->bSectorZ == this->bSectorZ )
		{
			// they have to be either both in sector, or both on the road
			if ( pRobot->flags.fBetweenSectors == this->flags.fBetweenSectors )
			{
				// if they're on the road...
				if ( pRobot->flags.fBetweenSectors )
				{
					// they have to be in the same squad or vehicle
					if ( pRobot->bAssignment != this->bAssignment )
					{
						return( FALSE );
					}

					// if in a vehicle, must be the same vehicle
					if ( pRobot->bAssignment == VEHICLE && ( pRobot->iVehicleId != this->iVehicleId ) )
					{
						return( FALSE );
					}
				}

				// all OK!
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


SOLDIERTYPE *SOLDIERTYPE::GetRobotController( void )
{
	if ( this->ubRobotRemoteHolderID == NOBODY )
	{
		return( NULL );
	}
	else
	{
		return( MercPtrs[ this->ubRobotRemoteHolderID ] );
	}
}

void SOLDIERTYPE::UpdateRobotControllerGivenRobot( void )
{
	//the original function passed in pRobot, not pSoldier
	SOLDIERTYPE *pRobot = this;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt = 0;

	// Loop through guys and look for a controller!

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pTeamSoldier++)
	{
		if ( pTeamSoldier->bActive )
		{
			if ( pTeamSoldier->ControllingRobot(  ) )
			{
				pRobot->ubRobotRemoteHolderID = pTeamSoldier->ubID;
				return;
			}
		}
	}

	pRobot->ubRobotRemoteHolderID = NOBODY;
}


void SOLDIERTYPE::UpdateRobotControllerGivenController( void )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt = 0;

	// First see if are still controlling the robot
	if ( !this->ControllingRobot( ) )
	{
		return;
	}

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// Loop through guys to find the robot....
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pTeamSoldier++)
	{
		if ( pTeamSoldier->bActive && ( pTeamSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
		{
			pTeamSoldier->ubRobotRemoteHolderID = this->ubID;
		}
	}
}


void SOLDIERTYPE::HandleSoldierTakeDamageFeedback( void )
{
	// Do sound.....
	// if ( this->stats.bLife >= CONSCIOUSNESS )
	{
		// ATE: Limit how often we grunt...
		if ( ( GetJA2Clock( ) - this->uiTimeSinceLastBleedGrunt ) > 1000 )
		{
			this->uiTimeSinceLastBleedGrunt = GetJA2Clock( );

			this->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );
		}
	}

	// Flash portrait....
	this->flags.fFlashPortrait = TRUE;
	this->bFlashPortraitFrame = FLASH_PORTRAIT_STARTSHADE;
	RESETTIMECOUNTER( this->timeCounters.PortraitFlashCounter, FLASH_PORTRAIT_DELAY );
}


void HandleSystemNewAISituation( SOLDIERTYPE *pSoldier, BOOLEAN fResetABC )
{
	// Are we an AI guy?
	// 0verhaul:
	// This code will only stop a soldier if it is not the player's turn.  The problem here is that the soldier's
	// actions may have triggered an interrupt.  This code is called in order to cancel the soldier's movement 
	// after the interrupt is triggered, so if the AI causes an interrupt and it's the player's turn, he will
	// continue doing what he was going to do.  We need this function to work even when it's the player's turn,
	// at least in this case.
	//if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum && pSoldier->bTeam != gbPlayerNum )
	{
		if ( pSoldier->aiData.bNewSituation == IS_NEW_SITUATION )
		{
			// Cancel what they were doing....
			pSoldier->usPendingAnimation	= NO_PENDING_ANIMATION;
			pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;
			pSoldier->flags.bTurningFromPronePosition = FALSE;
			pSoldier->ubPendingDirection = NO_PENDING_DIRECTION;
			pSoldier->aiData.ubPendingAction		 = NO_PENDING_ACTION;
			pSoldier->bEndDoorOpenCode	 = 0;

			// if this guy isn't under direct AI control, WHO GIVES A FLYING FLICK?
			if ( pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL )
			{
				if ( pSoldier->flags.fTurningToShoot )
				{
					pSoldier->flags.fTurningToShoot = FALSE;
					// Release attacker
					// OK - this is hightly annoying , but due to the huge combinations of
					// things that can happen - 1 of them is that sLastTarget will get unset
					// after turn is done - so set flag here to tell it not to...
					pSoldier->flags.fDontUnsetLastTargetFromTurn = TRUE;
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending fire because saw something: DONE IN SYSTEM NEW SITUATION") );
					DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something: DONE IN SYSTEM NEW SITUATION\n" );
					FreeUpAttacker( );
				}

				if ( pSoldier->pTempObject != NULL )
				{
					// Place it back into inv....
					AutoPlaceObject( pSoldier, pSoldier->pTempObject, FALSE );
					OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
					pSoldier->usPendingAnimation  = NO_PENDING_ANIMATION;
					pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;

					// Decrement attack counter...
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., ending throw because saw something: DONE IN SYSTEM NEW SITUATION") );
					DebugAttackBusy("@@@@@@@ Reducing attacker busy count..., ending throw because saw something: DONE IN SYSTEM NEW SITUATION\n");
					FreeUpAttacker( );
				}

			}
		}
	}
}

void InternalPlaySoldierFootstepSound( SOLDIERTYPE * pSoldier )
{
	UINT8					ubRandomSnd;
	INT8					bVolume = MIDVOLUME;
	// Assume outside
	UINT32					ubSoundBase = WALK_LEFT_OUT;
	UINT8					ubRandomMax = 4;

	// Determine if we are on the floor
	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		if ( pSoldier->usAnimState == HOPFENCE )
		{
			bVolume = HIGHVOLUME;
		}

		if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
		{
			PlaySoldierJA2Sample( pSoldier->ubID, ROBOT_BEEP, RATE_11025, SoundVolume( bVolume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
			return;
		}

		//if ( SoldierOnScreen( pSoldier->ubID ) )
		{
			if ( pSoldier->usAnimState == CRAWLING )
			{
				ubSoundBase = CRAWL_1;
			}
			else
			{
				// Pick base based on terrain over....
				if ( pSoldier->bOverTerrainType == FLAT_FLOOR )
				{
					ubSoundBase = WALK_LEFT_IN;
				}
				else if ( pSoldier->bOverTerrainType == DIRT_ROAD || pSoldier->bOverTerrainType == PAVED_ROAD )
				{
					ubSoundBase = WALK_LEFT_ROAD;
				}
				else if ( pSoldier->MercInShallowWater( ) )
				{
					ubSoundBase = WATER_WALK1_IN;
					ubRandomMax = 2;
				}
				else if ( pSoldier->MercInDeepWater( ) )
				{
					ubSoundBase = SWIM_1;
					ubRandomMax = 2;
				}
			}

			// Pick a random sound...
			do
			{
				ubRandomSnd = (UINT8)Random( ubRandomMax );

			} while ( ubRandomSnd == pSoldier->ubLastFootPrintSound );

			pSoldier->ubLastFootPrintSound = ubRandomSnd;

			// OK, if in realtime, don't play at full volume, because too many people walking around
			// sounds don't sound good - ( unless we are the selected guy, then always play at reg volume )
			if ( ! ( gTacticalStatus.uiFlags & INCOMBAT ) && ( pSoldier->ubID != gusSelectedSoldier ) )
			{
				bVolume = LOWVOLUME;
			}

			PlaySoldierJA2Sample( pSoldier->ubID, ubSoundBase + pSoldier->ubLastFootPrintSound, RATE_11025, SoundVolume( bVolume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
		}
	}
}

void PlaySoldierFootstepSound( SOLDIERTYPE *pSoldier )
{
	// normally, not in stealth mode
	if ( !pSoldier->bStealthMode )
	{
		InternalPlaySoldierFootstepSound( pSoldier );
	}
}

void PlayStealthySoldierFootstepSound( SOLDIERTYPE *pSoldier )
{
	// even if in stealth mode
	InternalPlaySoldierFootstepSound( pSoldier );
}



void CrowsFlyAway( UINT8 ubTeam )
{
	UINT32				cnt;
	SOLDIERTYPE		*pTeamSoldier;

	for ( cnt = gTacticalStatus.Team[ ubTeam ].bFirstID, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ ubTeam ].bLastID; cnt++, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
		{
			if ( pTeamSoldier->ubBodyType == CROW && pTeamSoldier->usAnimState != CROW_FLY )
			{
				// fly away even if not seen!
				HandleCrowFlyAway( pTeamSoldier );
			}
		}
	}
}


#ifdef JA2BETAVERSION
void DebugValidateSoldierData( )
{
	UINT32 cnt;
	SOLDIERTYPE		*pSoldier;
	CHAR16 sString[ 1024 ];
	BOOLEAN fProblemDetected = FALSE;
	static UINT32 uiFrameCount = 0;


	// this function is too slow to run every frame, so do the check only every 50 frames
	if ( uiFrameCount++ < 50 )
	{
		return;
	}

	// reset frame counter
	uiFrameCount = 0;


	// Loop through our team...
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		if ( pSoldier->bActive )
		{
			// OK, first check for alive people
			// Don't do this check if we are a vehicle...
			if ( pSoldier->stats.bLife > 0  && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				// Alive -- now check for proper group IDs
				if ( pSoldier->ubGroupID == 0 && pSoldier->bAssignment != IN_TRANSIT && pSoldier->bAssignment != ASSIGNMENT_POW && !( pSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
				{
					// This is bad!
					swprintf( sString, L"Soldier Data Error: Soldier %d is alive but has a zero group ID.", cnt );
					fProblemDetected = TRUE;
				}
				else if ( ( pSoldier->ubGroupID != 0 ) && ( GetGroup( pSoldier->ubGroupID ) == NULL ) )
				{
					// This is bad!
					swprintf( sString, L"Soldier Data Error: Soldier %d has an invalid group ID of %d.", cnt, pSoldier->ubGroupID );
					fProblemDetected = TRUE;
				}
			}
			else
			{
				if ( pSoldier->ubGroupID != 0 && ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
				{
					// Dead guys should have 0 group IDs
					//swprintf( sString, L"GroupID Error: Soldier %d is dead but has a non-zero group ID.", cnt );
					//fProblemDetected = TRUE;
				}
			}

			// check for invalid sector data
			if ( ( pSoldier->bAssignment != IN_TRANSIT ) &&
				( ( pSoldier->sSectorX <= 0 ) || ( pSoldier->sSectorX >= 17 ) ||
				( pSoldier->sSectorY <= 0 ) || ( pSoldier->sSectorY >= 17 ) ||
				( pSoldier->bSectorZ  < 0 ) || ( pSoldier->bSectorZ >   3 ) ) )
			{
				swprintf( sString, L"Soldier Data Error: Soldier %d is located at %d/%d/%d.", cnt, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
				fProblemDetected = TRUE;
			}
		}

		if ( fProblemDetected )
		{
			SAIReportError( sString );
			/*
			if ( guiCurrentScreen == MAP_SCREEN )
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
			else
			DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
			*/
			break;
		}
	}


	// also do this
	ValidatePlayersAreInOneGroupOnly();
}
#endif



void SOLDIERTYPE::BeginTyingToFall( void )
{
	this->bStartFallDir = this->ubDirection;
	this->flags.fTryingToFall = TRUE;

	// Randomize direction
	if ( Random( 50 ) < 25 )
	{
		this->flags.fFallClockwise = TRUE;
	}
	else
	{
		this->flags.fFallClockwise = FALSE;
	}
}

void SOLDIERTYPE::SetSoldierAsUnderAiControl( void )
{
	SOLDIERTYPE *pSoldier=NULL;
	INT32 cnt;

	//this is silly, but left over from when pSoldierToSet was passed in as a parameter
	if ( this == NULL )
	{
		return;
	}

	// Loop through ALL teams...
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ LAST_TEAM ].bLastID; cnt++,pSoldier++)
	{
		if( pSoldier->bActive )
		{
			pSoldier->flags.uiStatusFlags &= ~SOLDIER_UNDERAICONTROL;
		}
	}

	this->flags.uiStatusFlags |= SOLDIER_UNDERAICONTROL;
}

void HandlePlayerTogglingLightEffects( BOOLEAN fToggleValue )
{
	if( fToggleValue )
	{
		//Toggle light status
		if ( gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] )
			gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] = FALSE;
		else
			gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] = TRUE;
	}

	//Update all the mercs in the sector
	EnableDisableSoldierLightEffects( gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ] );

	SetRenderFlags(RENDER_FLAG_FULL);
}


void EnableDisableSoldierLightEffects( BOOLEAN fEnableLights )
{
	SOLDIERTYPE *pSoldier=NULL;
	INT32 cnt;

	// Loop through player teams...
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{
		//if the soldier is in the sector
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE )
		{
			//if we are to enable the lights
			if( fEnableLights )
			{
				//Add the light around the merc
				pSoldier->PositionSoldierLight( );
			}
			else
			{
				//Delete the fake light the merc casts
				pSoldier->DeleteSoldierLight( );

				//Light up the merc though
				SetSoldierPersonalLightLevel( pSoldier );
			}
		}
	}
}

void SetSoldierPersonalLightLevel( SOLDIERTYPE *pSoldier )
{
	if( pSoldier == NULL )
	{
		return;
	}

	if( pSoldier->sGridNo == NOWHERE )
	{
		return;
	}

	//THe light level for the soldier
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubShadeLevel = 3;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubSumLights = 5;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubMaxLights = 5;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubNaturalShadeLevel = 5;
}
