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
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include "pathai.h"
#include "Random.h"
#include "worldman.h"
#include "Isometric Utils.h"
#include "renderworld.h"
#include "video.h"
#include "points.h"
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
#include "Debug Control.h"
#include "LOS.h" // added by SANDRO
#include "CampaignStats.h"		// added by Flugente
#include "Interface Panels.h"
#include "Queen Command.h"		// added by Flugente
#include "Town Militia.h"		// added by Flugente
#include "Auto Bandage.h"		// added by Flugente
#include "Facilities.h"			// added by Flugente
#include "Cheats.h"				// added by Flugente
#include "MilitiaIndividual.h"	// added by Flugente
#include "Arms Dealer Init.h"	// added by Flugente for armsDealerInfo[]
#include "LuaInitNPCs.h"		// added by Flugente
#include "SaveLoadMap.h"		// added by Flugente
#include "qarray.h"				// added by Flugente

#include "ub_config.h"
#include "../ModularizedTacticalAI/include/Plan.h" // for plan destructor call

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

#include "GameInitOptionsScreen.h"

#include "fresh_header.h"

#include "Dialogue Control.h"

#include "IMP Skill Trait.h"	// added by Flugente
#include "Food.h"				// added by Flugente
#include "Tactical Save.h"		// added by Flugente for AddItemsToUnLoadedSector()
#include "LightEffects.h"		// added by Flugente for CreatePersonalLight()
#include "DynamicDialogue.h"	// added by Flugente for HandleDynamicOpinions()
#include "strategic town loyalty.h"		// added by Flugente for gTownLoyalty
#include "Rebel Command.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

UINT16 usForceAnimState = INVALID_ANIMATION;//dnl ch70 170913

//turnspeed
//UINT8 gubPlayerTurnSpeedUpFactor = 1;
//UINT8 gubEnemyTurnSpeedUpFactor = 1;
//UINT8 gubCreatureTurnSpeedUpFactor = 1;
//UINT8 gubMilitiaTurnSpeedUpFactor = 1;
//UINT8 gubCivilianTurnSpeedUpFactor = 1;
//turnspeed

//extern BOOLEAN fAllowTacticalMilitiaCommand; //lal

extern INT16 DirIncrementer[8];

// sevenfm: used in auto taking concertina/sandbag items from inventory
extern BOOLEAN gfShiftBombPlant;

#define		PALETTEFILENAME							"BINARYDATA\\ja2pal.dat"

#define		LOW_MORALE_BATTLE_SND_THREASHOLD	35


#define		MIN_SUBSEQUENT_SNDS_DELAY									2000
#include "connect.h"

extern void TeleportSelectedSoldier( void );
extern BOOLEAN AddSoldierToSectorNoCalculateDirectionUseAnimation( UINT16 ubID, UINT16 usAnimState, UINT16 usAnimCode );

// Flugente: external sector data
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// sevenfm: check availability of actions
extern BOOLEAN CheckAutoBandage(void);

// Enumerate extended directions
enum
{
	EX_NORTH = 0,
	EX_NORTHEAST = 4,
	EX_EAST = 8,
	EX_SOUTHEAST = 12,
	EX_SOUTH = 16,
	EX_SOUTHWEST = 20,
	EX_WEST = 24,
	EX_NORTHWEST = 28,
	EX_NUM_WORLD_DIRECTIONS = 32,
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


UINT8 gExtOneCDirection[EX_NUM_WORLD_DIRECTIONS] =
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
	BOOLEAN			fPreload;
	BOOLEAN			fBadGuy;
	BOOLEAN			fDontAllowTwoInRow;
	BOOLEAN			fStopDialogue;

} BATTLESNDS_STRUCT;


BATTLESNDS_STRUCT	 gBattleSndsData[] =
{
	"ok", 1, 1, 1, 2,
	"cool", 1, 0, 1, 0,
	"curse", 1, 1, 1, 0,
	"hit", 1, 1, 1, 1,
	"laugh", 1, 1, 1, 0,
	"attn", 1, 0, 1, 0,
	"dying", 1, 1, 1, 1,
	"humm", 0, 0, 1, 1,
	"noth", 0, 0, 1, 1,
	"gotit", 0, 0, 1, 1,
	"lmok", 1, 0, 1, 2,
	"lmattn", 1, 0, 1, 0,
	"locked", 0, 0, 1, 0,
	"enem", 1, 1, 1, 0,
	"punch", 1, 0, 0, 0,
	"knife", 1, 0, 0, 0,
};

// Flugente: a little helper struct for npc sounds
typedef struct
{
	CHAR8			zName[20];
} BATTLESNDS_NPC_HELPER_STRUCT;

BATTLESNDS_NPC_HELPER_STRUCT	 gBattleSndsNpcHelperData[] =
{
	"bad",
	"kid",
	"zombie",
};

extern void ReduceAttachmentsOnGunForNonPlayerChars( SOLDIERTYPE *pSoldier, OBJECTTYPE * pObj );

// CHRISL:
MERCPROFILEGEAR::MERCPROFILEGEAR( )
{
	clearInventory( );
	initialize( );
}

// Assignment operator
MERCPROFILEGEAR& MERCPROFILEGEAR::operator=(const MERCPROFILEGEAR& src) {
	if ( this != &src ) {
		memcpy( this, &src, SIZEOF_MERCPROFILEGEAR_POD );
		inv = src.inv;
		iStatus = src.iStatus;
		iDrop = src.iDrop;
		iNumber = src.iNumber;
		lbe = src.lbe;
		lStatus = src.lStatus;
		invCnt = src.invCnt;
		lbeCnt = src.lbeCnt;
		PriceModifier = src.PriceModifier;
		AbsolutePrice = src.AbsolutePrice;
	}
	return *this;
}

// Copy constructor
MERCPROFILEGEAR::MERCPROFILEGEAR(const MERCPROFILEGEAR& src)
{
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
		PriceModifier = src.PriceModifier;
		AbsolutePrice = src.AbsolutePrice;
	}
}

// Destructor
MERCPROFILEGEAR::~MERCPROFILEGEAR( ) {
}

// Initialize the soldier.  
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILEGEAR::initialize( ) {
	memset( this, 0, SIZEOF_MERCPROFILEGEAR_POD );
	clearInventory( );
}

void MERCPROFILEGEAR::clearInventory( ) {
	//ADB these really should be defines
	invCnt = 55;
	lbeCnt = 5;
	PriceModifier = 0;

	inv.clear( );
	iStatus.clear( );
	iDrop.clear( );
	iNumber.clear( );

	inv.resize( invCnt );
	iStatus.resize( invCnt );
	iDrop.resize( invCnt );
	iNumber.resize( invCnt );

	lbe.clear( );
	lStatus.clear( );

	lbe.resize( lbeCnt );
	lStatus.resize( lbeCnt );
}

// ----------------------------------------
// New inventory handling code.
// ----------------------------------------

Inventory::Inventory( int slotCount ) {
	inv.resize( slotCount );//calls the OBJECTTYPE constructor, will work with non POD data
	bNewItemCount.resize( slotCount );
	bNewItemCycleCount.resize( slotCount );
};

Inventory::Inventory( const Inventory& src ) {
	inv = src.inv;
	bNewItemCount = src.bNewItemCount;
	bNewItemCycleCount = src.bNewItemCycleCount;
}

unsigned int Inventory::size( ) const {
	return inv.size( );
}

auto Inventory::get() const -> const std::vector<OBJECTTYPE>& {
	return inv;
}

// Assignment operator
Inventory& Inventory::operator=(const Inventory& src)
{
	if ( this != &src ) {
		inv = src.inv;
		bNewItemCount = src.bNewItemCount;
		bNewItemCycleCount = src.bNewItemCycleCount;
	}
	return *this;
}

Inventory::~Inventory( ) {
	inv.clear( );
	bNewItemCount.clear( );
	bNewItemCycleCount.clear( );
};

OBJECTTYPE& Inventory::operator [] ( unsigned int idx )
{
	if ( idx >= inv.size( ) ) {
		inv.resize( idx + 1 );
		bNewItemCount.resize( idx + 1 );
		bNewItemCycleCount.resize( idx + 1 );
		DebugBreakpoint( );
	}
	return inv[idx];
};

void Inventory::clear( ) {
	int size = inv.size( );
	inv.clear( );
	bNewItemCount.clear( );
	bNewItemCycleCount.clear( );

	inv.resize( size );
	bNewItemCount.resize( size );
	bNewItemCycleCount.resize( size );
};

// ----------------------------------------

void STRUCT_TimeCounters::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
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

void STRUCT_Drugs::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
{
	int x = 0;
	for ( ; x < 2; ++x )
	{
		this->bFutureDrugEffect[x] = src.bFutureDrugEffect[x];						// value to represent effect of a needle
		this->bDrugEffectRate[x] = src.bDrugEffectRate[x];							// represents rate of increase and decrease of effect
		this->bDrugEffect[x] = src.bDrugEffect[x];									// value that affects AP & morale calc ( -ve is poorly )
		this->bDrugSideEffectRate[x] = src.bDrugSideEffectRate[x];					// duration of negative AP and morale effect
		this->bDrugSideEffect[x] = src.bDrugSideEffect[x];							// duration of negative AP and morale effect
		this->bTimesDrugUsedSinceSleep[x] = src.bTimesDrugUsedSinceSleep[x];
	}

	for ( ; x < DRUG_TYPE_MAX; ++x )
	{
		this->bFutureDrugEffect[x] = 0;							// value to represent effect of a needle
		this->bDrugEffectRate[x] = 0;							// represents rate of increase and decrease of effect
		this->bDrugEffect[x] = 0;								// value that affects AP & morale calc ( -ve is poorly )
		this->bDrugSideEffectRate[x] = 0;						// duration of negative AP and morale effect
		this->bDrugSideEffect[x] = 0;							// duration of negative AP and morale effect
		this->bTimesDrugUsedSinceSleep[x] = 0;
	}
}

void STRUCT_TimeChanges::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
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

void STRUCT_Flags::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
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
	//this->fCheckForNewlyAddedItems = src.bEndDoorOpenCode;//dnl ch33 130909
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
	this->bGoBackToAimAfterHit = src.fGoBackToAimAfterHit;
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

void STRUCT_Statistics::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
{
	this->ubSkillTraits[0] = src.ubSkillTrait1;
	this->ubSkillTraits[1] = src.ubSkillTrait2;
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

void STRUCT_AIData::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
{
	memcpy( &(this->bOppList), &(src.bOppList), sizeof(INT8)* MAX_NUM_SOLDIERS ); // AI knowledge database
	TranslateArrayFields( this->sPatrolGrid, src.usPatrolGrid, OLD_MAXPATROLGRIDS, INT16_INT32 );//dnl ch27 200909 AI list for ptr->orders==PATROL
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
	this->sNoiseGridno = src.sNoiseGridNo;
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

void STRUCT_Pathing::ConvertFrom_101_To_102( const OLDSOLDIERTYPE_101& src )
{
	memcpy( &(this->usPathingData), &(src.usPathingData), sizeof(UINT16)* MAX_PATH_LIST_SIZE );
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
	if ( (void*)this != (void*)&src ) {
		//member classes
		aiData.ConvertFrom_101_To_102( src );
		flags.ConvertFrom_101_To_102( src );
		timeChanges.ConvertFrom_101_To_102( src );
		timeCounters.ConvertFrom_101_To_102( src );
		//drugs.ConvertFrom_101_To_102( src );
		stats.ConvertFrom_101_To_102( src );
		pathing.ConvertFrom_101_To_102( src );
		inv = src.inv;

		//arrays
		memcpy( &(this->name), &(src.name), sizeof(CHAR16)* 10 );
		memcpy( &(this->pShades), &(src.pShades), sizeof(UINT16)* NUM_SOLDIER_SHADES ); // Shading tables
		memcpy( &(this->pGlowShades), &(src.pGlowShades), sizeof(UINT16)* 20 ); //
		memcpy( &(this->pEffectShades), &(src.pEffectShades), sizeof(UINT16)* NUM_SOLDIER_EFFECTSHADES ); // Shading tables for effects
		memcpy( &(this->sSpreadLocations), &(src.sSpreadLocations), sizeof(INT16)* MAX_BURST_SPREAD_TARGETS );
		memcpy( &(this->usFrontArcFullTileList), &(src.usFrontArcFullTileList), sizeof(UINT16)* MAX_FULLTILE_DIRECTIONS );
		memcpy( &(this->usFrontArcFullTileGridNos), &(src.usFrontArcFullTileGridNos), sizeof(INT16)* MAX_FULLTILE_DIRECTIONS );
		memcpy( &(this->HeadPal), &(src.HeadPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->PantsPal), &(src.PantsPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->VestPal), &(src.VestPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->SkinPal), &(src.SkinPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->MiscPal), &(src.MiscPal), sizeof(PaletteRepID) );	// 30
		this->sLastTwoLocations[0] = src.sLastTwoLocations[0];
		this->sLastTwoLocations[1] = src.sLastTwoLocations[1];

		this->ubID = src.ubID;
		//this->bReserved1 = src.bReserved1;

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
		// HEADROCK HAM 4: Changed from FLOAT to INT32 to record position at beginning of turn.
		this->sOldXPos = src.sOldXPos;
		this->sOldYPos = src.sOldYPos;
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

		// HEADROCK HAM 4: TODO: Added four new variables to soldiertype. MAke sure they don't screw it all up!
		this->dPrevMuzzleOffsetX[0] = 0.0;
		this->dPrevMuzzleOffsetX[1] = 0.0;
		this->dPrevMuzzleOffsetY[0] = 0.0;
		this->dPrevMuzzleOffsetY[1] = 0.0;
		this->dPrevCounterForceX[0] = 0.0;
		this->dPrevCounterForceX[1] = 0.0;
		this->dPrevCounterForceY[0] = 0.0;
		this->dPrevCounterForceY[1] = 0.0;

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
		this->sForcastGridno = src.sForcastGridNo;
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
		this->sSectorY = src.sSectorY;									// Y position on the Stategic Map
		this->bSectorZ = src.bSectorZ;									// Z sector location
		this->iVehicleId = src.iVehicleId;								// the id of the vehicle the char is in
		this->pMercPath = src.pMercPath;								//Path Structure
		this->usMedicalDeposit = src.usMedicalDeposit;         // is there a medical deposit on merc
		this->usLifeInsurance = src.usLifeInsurance;          // is there life insurance taken out on merc


		this->uiStartMovementTime = src.uiStartMovementTime;				// the time since the merc first started moving
		this->uiOptimumMovementTime = src.uiOptimumMovementTime;			// everytime in ececute overhead the time for the current ani will be added to this total
		this->usLastUpdateTime = src.usLastUpdateTime;					// The last time the soldier was in ExecuteOverhead

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

		this->bCamo = __min( gGameExternalOptions.bCamoKitArea, src.bCamo );
		this->wornCamo = __min( (100 - gGameExternalOptions.bCamoKitArea), src.wornCamo );

		this->urbanCamo = __min( gGameExternalOptions.bCamoKitArea, src.urbanCamo );	// Madd: new camo types
		this->wornUrbanCamo = __min( (100 - gGameExternalOptions.bCamoKitArea), src.wornUrbanCamo );

		this->desertCamo = __min( gGameExternalOptions.bCamoKitArea, src.desertCamo );
		this->wornDesertCamo = __min( (100 - gGameExternalOptions.bCamoKitArea), src.wornDesertCamo );

		this->snowCamo = __min( gGameExternalOptions.bCamoKitArea, src.snowCamo );
		this->wornSnowCamo = __min( (100 - gGameExternalOptions.bCamoKitArea), src.wornSnowCamo );

		this->bScopeMode = USE_BEST_SCOPE;

		this->ubMilitiaAssists = 0;

		this->CancelDrag();
		
		this->bFoodLevel = 0;
		this->bDrinkLevel = 0;
		this->usStarveDamageHealth = 0;
		this->usStarveDamageStrength = 0;
		this->bAIIndex = 0;
		this->usSoldierProfile = 0;
		this->usIndividualMilitiaID = 0;

		this->usAISkillUse = 0;
		for ( UINT8 i = 0; i < SOLDIER_COUNTER_MAX; ++i )		this->usSkillCounter[i] = 0;
		for ( UINT8 i = 0; i < SOLDIER_COOLDOWN_MAX; ++i )	this->usSkillCooldown[i] = 0;

		this->InitializeExtraData();
	}
	return *this;
}

SOLDIERTYPE::~SOLDIERTYPE( )
{
	if ( ai_masterplan_ )
		delete ai_masterplan_;
}

SOLDIERTYPE::SOLDIERTYPE( ) {
	initialize( );
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

	UINT32 invsize = this->inv.size( );
	for ( uiLoop = 0; uiLoop < invsize; ++uiLoop )
	{
		uiChecksum += this->inv[uiLoop].usItem;
		uiChecksum += this->inv[uiLoop].ubNumberOfObjects;
	}

	return(uiChecksum);
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void SOLDIERTYPE::initialize( )
{
	memset( this, 0, SIZEOF_SOLDIERTYPE_POD );
	inv.clear( );
	ai_masterplan_ = 0;
	this->iVehicleId = (-1);

	memset( &aiData, 0, sizeof(STRUCT_AIData) );
	memset( &flags, 0, sizeof(STRUCT_Flags) );
	memset( &timeChanges, 0, sizeof(STRUCT_TimeChanges) );
	memset( &timeCounters, 0, sizeof(STRUCT_TimeCounters) );
	//memset( &drugs, 0, sizeof(STRUCT_Drugs) );
	memset( &newdrugs, 0, sizeof(DRUGS) );
	memset( &stats, 0, sizeof(STRUCT_Statistics) );
	memset( &pathing, 0, sizeof(STRUCT_Pathing) );

	// sevenfm:initialize additional data
	this->InitializeExtraData();
}

bool SOLDIERTYPE::exists( )
{
	if ( this == NULL )
		return(FALSE);
	return (TRUE);
}

// Ugly temporary solution
//
// These two functions map the "old" style inventory (fixed array of ) to the new (a flexibly sized vector).
// If you change names or eliminate some positions or such you need to change these.
// Eventually the need for these functions will disappear.

UINT32 OLDSOLDIERTYPE_101::GetChecksum( )
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
		uiChecksum += this->inv[uiLoop].usItem;
		uiChecksum += this->inv[uiLoop].ubNumberOfObjects;
	}

	return(uiChecksum);
}


void OLDSOLDIERTYPE_101::CopyOldInventoryToNew( )
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

	UINT32 invsize = this->inv.size( );
	for ( uiLoop = 0; uiLoop < invsize; ++uiLoop )
	{
		uiChecksum += this->inv[uiLoop];
		uiChecksum += this->bInvNumber[uiLoop];
	}

	return(uiChecksum);
}

OLD_MERCPROFILESTRUCT_101::OLD_MERCPROFILESTRUCT_101( )
{
	memset( this, 0, SIZEOF_OLD_MERCPROFILESTRUCT_101_POD );
}

MERCPROFILESTRUCT::MERCPROFILESTRUCT( )
{
	initialize( );
}

// Copy Constructor
MERCPROFILESTRUCT::MERCPROFILESTRUCT( const MERCPROFILESTRUCT& src )
{
	memcpy( this, &src, SIZEOF_MERCPROFILESTRUCT_POD );
	inv = src.inv;
	bInvStatus = src.bInvStatus;
	bInvNumber = src.bInvNumber;
}

// Conversion Constructor
MERCPROFILESTRUCT& MERCPROFILESTRUCT::operator=(const OLD_MERCPROFILESTRUCT_101& src)
{
	//Why do we need this if the inv is an array of ints?  Because some data will be lost otherwise!
	if ( (void*)this != (void*)&src ) {
		CopyOldInventoryToNew( src );

		//arrays
		memcpy( &(this->zName), &(src.zName), sizeof(CHAR16)* NAME_LENGTH );
		memcpy( &(this->zNickname), &(src.zNickname), sizeof(CHAR16)* NICKNAME_LENGTH );
		memcpy( &(this->PANTS), &(src.PANTS), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->VEST), &(src.VEST), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->SKIN), &(src.SKIN), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->HAIR), &(src.HAIR), sizeof(PaletteRepID) );	// 30

		memcpy( &(this->bBuddy), &(src.bBuddy), 5 * sizeof (UINT8) );
		memcpy( &(this->bHated), &(src.bHated), 5 * sizeof (UINT8) );
		memcpy( &(this->usRoomRangeStart), &(src.ubRoomRangeStart), 2 * sizeof (UINT8) );
		memcpy( &(this->bMercTownReputation), &(src.bMercTownReputation), 20 * sizeof (INT8) );
		memcpy( &(this->usApproachFactor), &(src.usApproachFactor), 4 * sizeof (UINT16) );

		memcpy( &(this->ubApproachVal), &(src.ubApproachVal), 4 * sizeof (UINT8) );
		memcpy( &(this->ubApproachMod), &(src.ubApproachMod), 3 * 4 * sizeof (UINT8) );

		// Flugente: opinion has 255 entries now, old had 75 - only copy over the old
		memcpy( &(this->bMercOpinion), &(src.bMercOpinion), NUMBER_OF_OPINIONS_OLD * sizeof (INT8) );

		memcpy( &(this->usStatChangeChances), &(src.usStatChangeChances), 12 * sizeof (UINT16) );// used strictly for balancing, never shown!
		memcpy( &(this->usStatChangeSuccesses), &(src.usStatChangeSuccesses), 12 * sizeof (UINT16) );// used strictly for balancing, never shown!
		memcpy( &(this->usRoomRangeEnd), &(src.ubRoomRangeEnd), 2 * sizeof (UINT8) );
		memcpy( &(this->bHatedTime), &(src.bHatedTime), 5 * sizeof (INT8) );
		memcpy( &(this->bHatedCount), &(src.bHatedCount), 5 * sizeof (INT8) );

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
		this->fRegresses = src.bEvolution == 2; // formerly, 2 == CharacterEvolution::DEVOLVES
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
		/////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - new records
		this->records.usKillsElites = (src.usKills / 4);
		this->records.usKillsRegulars = (src.usKills / 2);
		this->records.usKillsAdmins = (src.usKills / 4);
		this->records.usKillsHostiles = 0;
		this->records.usKillsCreatures = 0;
		this->records.usKillsZombies = 0;
		this->records.usKillsTanks = 0;
		this->records.usKillsOthers = 0;
		this->records.usAssistsMercs = (src.usAssists * 3 / 4);
		this->records.usAssistsMilitia = (src.usAssists / 4);
		this->records.usAssistsOthers = 0;
		this->records.usShotsFired = src.usShotsFired;
		this->records.usMissilesLaunched = 0;
		this->records.usGrenadesThrown = 0;
		this->records.usKnivesThrown = 0;
		this->records.usBladeAttacks = 0;
		this->records.usHtHAttacks = 0;
		this->records.usShotsHit = src.usShotsHit;
		this->records.usBattlesTactical = (src.usBattlesFought * 3 / 4);
		this->records.usBattlesAutoresolve = src.usBattlesFought / 4;
		this->records.usBattlesRetreated = 0;
		this->records.usAmbushesExperienced = 0;
		this->records.usLargestBattleFought = 0;
		this->records.usTimesWoundedShot = src.usTimesWounded;
		this->records.usTimesWoundedStabbed = 0;
		this->records.usTimesWoundedPunched = 0;
		this->records.usTimesWoundedBlasted = 0;
		this->records.usTimesStatDamaged = 0;
		this->records.usTimesSurgeryUndergoed = 0;
		this->records.usFacilityAccidents = 0;
		this->records.usLocksPicked = 0;
		this->records.usLocksBreached = 0;
		this->records.usTrapsRemoved = 0;
		this->records.usExpDetonated = 0;
		this->records.usItemsRepaired = 0;
		this->records.usItemsCombined = 0;
		this->records.usItemsStolen = 0;
		this->records.usMercsBandaged = 0;
		this->records.usSurgeriesMade = 0;
		this->records.usNPCsDiscovered = 0;
		this->records.usSectorsDiscovered = 0;
		this->records.usMilitiaTrained = 0;
		//this->records.usFacilityEvents = 0;
		this->records.ubQuestsHandled = 0;

		/////////////////////////////////////////////////////////////////////////////////////
		this->usTotalDaysServed = src.usTotalDaysServed;

		this->sLeadershipGain = src.sLeadershipGain;
		this->sStrengthGain = src.sStrengthGain;



		// BODY TYPE SUBSITUTIONS
		this->uiBodyTypeSubFlags = src.uiBodyTypeSubFlags;

		this->sSalary = src.sSalary;
		this->bLife = src.bLife;
		this->bDexterity = src.bDexterity;// dexterity (hand coord) value
		this->bDisability = src.bDisability;

		this->bSkillTraits[0] = src.bSkillTrait;
		this->bSkillTraits[1] = src.bSkillTrait2;

		this->bReputationTolerance = src.bReputationTolerance;
		this->bExplosive = src.bExplosive;
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
	if ( this != &src ) {
		memcpy( this, &src, SIZEOF_MERCPROFILESTRUCT_POD );
		inv = src.inv;
		bInvStatus = src.bInvStatus;
		bInvNumber = src.bInvNumber;
	}
	return *this;
}

// Destructor
MERCPROFILESTRUCT::~MERCPROFILESTRUCT( )
{
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILESTRUCT::initialize( )
{
	memset( this, 0, SIZEOF_MERCPROFILESTRUCT_POD );
	clearInventory( );
	// SANDRO - added this
	memset( &records, 0, sizeof(STRUCT_Records) );
	memset( &usBackground, 0, sizeof(UINT16) );

	memset( &usDynamicOpinionFlagmask, 0, sizeof(usDynamicOpinionFlagmask) );
	memset( &sDynamicOpinionLongTerm, 0, sizeof(sDynamicOpinionLongTerm) );

	memset( &usVoiceIndex, 0, sizeof(UINT32) );
	memset( &Type, 0, sizeof( UINT32 ) );
}

// Initialize the soldier.
//  Use this instead of the old method of calling memset!
//  Note that the constructor does this automatically.
void MERCPROFILESTRUCT::clearInventory( )
{
	inv.clear( );
	bInvStatus.clear( );
	bInvNumber.clear( );

	inv.resize( NUM_INV_SLOTS );
	bInvStatus.resize( NUM_INV_SLOTS );
	bInvNumber.resize( NUM_INV_SLOTS );
}

void MERCPROFILESTRUCT::CopyOldInventoryToNew( const OLD_MERCPROFILESTRUCT_101& src )
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
	5,						// NO_TERRAIN // anv: that one was missing
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


INT16		gsFullTileDirections[MAX_FULLTILE_DIRECTIONS] =
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
UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady, BOOLEAN fAltWeaponHolding );
BOOLEAN CheckForFullStruct( INT32 sGridNo, UINT16 *pusIndex );
void SetSoldierLocatorOffsets( SOLDIERTYPE *pSoldier );
void CheckForFullStructures( SOLDIERTYPE *pSoldier );
UINT16 GetNewSoldierStateFromNewStance( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void SetSoldierAniSpeed( SOLDIERTYPE *pSoldier );
void AdjustForFastTurnAnimation( SOLDIERTYPE *pSoldier );
UINT16 SelectFireAnimation( SOLDIERTYPE *pSoldier, UINT8 ubHeight );
void SelectFallAnimation( SOLDIERTYPE *pSoldier );
BOOLEAN FullStructAlone( INT32 sGridNo, UINT8 ubRadius );
void SoldierGotHitGunFire( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT16 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitBlade( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitPunch( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitExplosion( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
void SoldierGotHitVehicle( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation );
UINT8 CalcScreamVolume( SOLDIERTYPE * pSoldier, UINT8 ubCombinedLoss );
void PlaySoldierFootstepSound( SOLDIERTYPE *pSoldier );
void HandleSystemNewAISituation( SOLDIERTYPE *pSoldier, BOOLEAN fResetABC );

UINT16 *CreateEnemyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen );
UINT16 *CreateEnemyGreyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen );

void SoldierBleed( SOLDIERTYPE *pSoldier, BOOLEAN fBandagedBleed );
INT32 CheckBleeding( SOLDIERTYPE *pSoldier );

void EVENT_InternalSetSoldierDesiredDirection( SOLDIERTYPE *pSoldier, UINT8	ubNewDirection, BOOLEAN fInitalMove, UINT16 usAnimState );

#ifdef JA2BETAVERSION
extern void ValidatePlayersAreInOneGroupOnly( );
extern void MapScreenDefaultOkBoxCallback( UINT8 bExitValue );
void SAIReportError( STR16 wErrorString );
#endif

UINT32 SleepDartSuccumbChance( SOLDIERTYPE * pSoldier );

void	EnableDisableSoldierLightEffects( BOOLEAN fEnableLights );
void	SetSoldierPersonalLightLevel( SOLDIERTYPE *pSoldier );


void HandleVehicleMovementSound( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	VEHICLETYPE *pVehicle = &(pVehicleList[pSoldier->bVehicleID]);

	if ( fOn )
	{
		if ( pVehicle->iMovementSoundID == NO_SAMPLE )
		{
			// anv: will be played in InternalPlaySoldierFootstepSound
			//pVehicle->iMovementSoundID = PlayJA2Sample( pVehicle->iMoveSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
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
	if ( is_networked && this->ubID < 120 )
	{
		//if(this->ubID>=120) 
		//	return;//hayden
		EV_S_STOP_MERC				SStopMerc;

		SStopMerc.sGridNo = this->sGridNo;
		SStopMerc.ubDirection = this->ubDirection;
		SStopMerc.usSoldierID = this->ubID;
		SStopMerc.fset = fSet;
		SStopMerc.sXPos = this->sX;
		SStopMerc.sYPos = this->sY;

		//AddGameEvent( S_STOP_MERC, 0, &SStopMerc ); //hayden.
		if ( !is_server && this->ubID >= 20 )return;

		if ( is_client )
			send_stop( &SStopMerc );
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

	memset( &AniParams, 0, sizeof(ANITILE_PARAMS) );

	if ( pSoldier->ubBodyType == CROW )
	{
		if ( pSoldier->pAniTile != NULL )
		{
			DeleteAniTile( pSoldier->pAniTile );
			pSoldier->pAniTile = NULL;
		}

		if ( !TileIsOutOfBounds( pSoldier->sGridNo ) )
		{
			if ( pSoldier->usAnimState == CROW_FLY )
			{
				AniParams.sGridNo = pSoldier->sGridNo;
				AniParams.ubLevelID = ANI_SHADOW_LEVEL;
				AniParams.sDelay = pSoldier->sAniDelay;
				AniParams.sStartFrame = 0;
				AniParams.uiFlags = ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_LOOPING | ANITILE_USE_DIRECTION_FOR_START_FRAME;
				AniParams.sX = pSoldier->sX;
				AniParams.sY = pSoldier->sY;
				AniParams.sZ = 0;
				strcpy( AniParams.zCachedFile, "TILECACHE\\FLY_SHDW.STI" );

				AniParams.uiUserData3 = pSoldier->ubDirection;

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
				pSoldier->pAniTile->uiUserData3 = pSoldier->ubDirection;
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
				pSoldier->pAniTile->sRelativeX = pSoldier->sX;
				pSoldier->pAniTile->sRelativeY = pSoldier->sY;
			}
		}
	}
}

extern INT16 DynamicAdjustAPConstants( INT16 iniReadValue, INT16 iniDefaultValue, BOOLEAN reverse );

INT16 SOLDIERTYPE::CalcActionPoints( void )
{
	INT16 ubPoints, ubMaxAPs;
	INT8  bBandage;

	// dead guys don't get any APs (they shouldn't be here asking for them!)
	if ( !this->stats.bLife )
		return(0);

	// people with sleep dart drug who have collapsed get no APs
	if ( (this->bSleepDrugCounter > 0) && this->bCollapsed )
		return(0);

	//CHRISL: Update this calucalation to give a default range of 40-100
	// Calculate merc's action points at 100% capability (range is 10 - 25)
	// round fractions of .5 up (that's why the +20 before the division!
	//ubPoints = 5 + (((10 * EffectiveExpLevel( this ) +
	//	3 * EffectiveAgility( this )   +
	//	2 * this->stats.bLifeMax   +
	//	2 * EffectiveDexterity( this ) ) + 20) / 40);
	ubPoints = 20 + (((10 * EffectiveExpLevel( this ) +
		3 * EffectiveAgility( this, FALSE ) +
		2 * this->stats.bLifeMax +
		2 * EffectiveDexterity( this, FALSE )) + 5) / 10);

	//if (GameOption[INCREASEDAP] % 2 == 1)
	//points += APBPConstants[AP_INCREASE];

	//Madd: Add in AP Bonuses (Penalties) from worn gear
	ubPoints += GetGearAPBonus( this );

	// Calculate bandage
	bBandage = this->stats.bLifeMax - this->stats.bLife - this->bBleeding;

	// If injured, reduce action points accordingly (by up to 2/3rds)
	if ( this->stats.bLife < this->stats.bLifeMax )
	{
		ubPoints -= (2 * ubPoints * (this->stats.bLifeMax - this->stats.bLife + (bBandage / 2))) /
			(3 * this->stats.bLifeMax);
	}

	// If tired, reduce action points accordingly (by up to 1/2)
	if ( this->bBreath < 100 && !(this->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT(this))
		ubPoints -= (ubPoints * (100 - this->bBreath)) / 200;

	if ( this->sWeightCarriedAtTurnStart > 100 )
	{
		ubPoints = (UINT8)(((UINT32)ubPoints) * 100 / this->sWeightCarriedAtTurnStart);
	}

	//CHRISL: Moved this down here so that all ubPoints adjustments can be made to the default 100AP system
	// Adjust ubPoints based on actual AP_MAXIMUM setting
	ubPoints = DynamicAdjustAPConstants( ubPoints, ubPoints );

	// If resulting APs are below our permitted minimum, raise them to it!
	// HEADROCK: Enforce new minimums due to suppression. I should've done this neater though.
	// HEADROCK HAM 3.6: This was the wrong place to put AP_MIN_LIMIT. The value here should be AP_MINIMUM, which is
	// the minimum amount of APs a character can GAIN each turn on top of what he had last turn. AP_MIN_LIMIT has been
	// moved to CalcNewActionPoints() where it belongs.
	if (ubPoints < APBPConstants[AP_MINIMUM])
		ubPoints = APBPConstants[AP_MINIMUM];
	
	// make sure action points doesn't exceed the permitted maximum
	ubMaxAPs = gubMaxActionPoints[ this->ubBodyType ];

	//if (GameOption[INCREASEDAP] % 2 == 1)
	// maxAPs += APBPConstants[AP_INCREASE];


	// If resulting APs are below our permitted minimum, raise them to it!
	if ( ubPoints > ubMaxAPs )
		ubPoints = ubMaxAPs;

	if ( this->ubBodyType == BLOODCAT )
	{
		// use same as young monsters
		ubPoints = (ubPoints * APBPConstants[AP_YOUNG_MONST_FACTOR]) / 10;
	}
	else if ( this->flags.uiStatusFlags & SOLDIER_MONSTER )
	{
		// young monsters get extra APs
		if ( this->ubBodyType == YAF_MONSTER || this->ubBodyType == YAM_MONSTER || this->ubBodyType == INFANT_MONSTER )
		{
			ubPoints = (ubPoints * APBPConstants[AP_YOUNG_MONST_FACTOR]) / 10;
		}

		// if frenzied, female monsters get more APs! (for young females, cumulative!)
		if ( this->aiData.bFrenzied )
		{

			ubPoints = (ubPoints * APBPConstants[AP_MONST_FRENZY_FACTOR]) / 10;
		}
	}
	// SANDRO - moved this here
	// If we are a vehicle, adjust APS...
	else if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		AdjustVehicleAPs( this, &ubPoints );
	}
	// SANDRO - squadleader's bonus to APs
	else if ( gGameOptions.fNewTraitSystem && IS_MERC_BODY_TYPE( this ) &&
			  (this->bTeam == ENEMY_TEAM || this->bTeam == MILITIA_TEAM || this->bTeam == gbPlayerNum) )
	{
		// +5% APs per trait (+15% is max) (as a default)
		ubPoints += (ubPoints * gSkillTraitValues.ubSLBonusAPsPercent * GetSquadleadersCountInVicinity( this, FALSE, FALSE ) / 100);
	}

	// adjust APs for phobia situations
	if ( this->ubProfile != NO_PROFILE )
	{
		// Flugente: drugs can temporarily cause a merc to be claustrophobic
		if ( DoesMercHaveDisability( this, CLAUSTROPHOBIC ) && gbWorldSectorZ > 0 )
		{
			ubPoints = (ubPoints * APBPConstants[AP_CLAUSTROPHOBE]) / 10;
		}
		else if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) && MercSeesCreature( this ) )
		{
			ubPoints = (ubPoints * APBPConstants[AP_AFRAID_OF_INSECTS]) / 10;
		}
		else if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) && MercIsInTropicalSector( this ) )
		{
			ubPoints = (ubPoints * 9) / 10;
		}
		else if ( DoesMercHaveDisability( this, AFRAID_OF_HEIGHTS ) && this->pathing.bLevel > 0 )
		{
			ubPoints = (ubPoints * 9) / 10;
		}
	}

	// Adjust APs due to drugs...
	HandleAPEffectDueToDrugs( this, &ubPoints );
	
	//Madd
	//	if ( this->bTeam != CIV_TEAM && this->bTeam != gbPlayerNum && gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
	//		ubPoints += 5; // everything and everyone moves a little faster against you on insane...


	//Kaiden: Took your idea a step further adding the bonus for each difficulty level
	// and then externalized it. AND added it to the Dont max out points section below.
	///////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - following code messed a bit
	//if ( this->bTeam != CIV_TEAM && this->bTeam != gbPlayerNum)
	if ( this->bTeam == ENEMY_TEAM )
	{
		ubPoints += zDiffSetting[gGameOptions.ubDifficultyLevel].iEnemyAPBonus;
	}
	// Bonus to Militia APs
	else if ( this->bTeam == MILITIA_TEAM )
	{
		// This is only affected from 2/5
		if ( this->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.bGreenMilitiaAPsBonus != 0 )
			ubPoints += gGameExternalOptions.bGreenMilitiaAPsBonus;
		else if ( this->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.bRegularMilitiaAPsBonus != 0 )
			ubPoints += gGameExternalOptions.bRegularMilitiaAPsBonus;
		else if ( this->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.bVeteranMilitiaAPsBonus != 0 )
			ubPoints += gGameExternalOptions.bVeteranMilitiaAPsBonus;
	}
	//Kaiden: Players just max out normally unless drugged
	else if ( this->bTeam == gbPlayerNum )
	{
		ubPoints += gGameExternalOptions.iPlayerAPBonus;
	}
	// nothing here
	else
	{
		//ubPoints += 0;
	}

	// Flugente: personal AP adjustments
	ubPoints = (INT16)((ubPoints * (100 + this->GetAPBonus( ))) / 100);

	// option to make special NPCs stronger - AP bonus
	if ( this->ubProfile != NO_PROFILE && gGameExternalOptions.usSpecialNPCStronger > 0 )
	{
		switch ( this->ubProfile )
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
			// Only 1/4 of the bonus
			ubPoints += (ubPoints * gGameExternalOptions.usSpecialNPCStronger / 400);
			break;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////

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
		if ( this->bActionPoints > APBPConstants[MAX_AP_CARRIED] / 2 )
		{
			this->bActionPoints = APBPConstants[MAX_AP_CARRIED] / 2;
		}
	}
	else
	{
		//POSSIBLE STRUCTURE CHANGE PROBLEM, NOT CURRENTLY CHANGED. GOTTHARD 7/14/08		
		if ( this->bActionPoints > APBPConstants[MAX_AP_CARRIED] )
		{
			this->bActionPoints = APBPConstants[MAX_AP_CARRIED];
		}
	}

	this->bActionPoints += this->CalcActionPoints( );
	// HEADROCK HAM 3.6: This should've been here all along. This enforces an absolute minimum limit on APs, which
	// can be negative.
	if ( this->bActionPoints < APBPConstants[AP_MIN_LIMIT] )
		this->bActionPoints = APBPConstants[AP_MIN_LIMIT];

	// Don't max out if we are drugged....
	if ( !this->newdrugs.size[DRUG_EFFECT_AP] && !this->newdrugs.size[DRUG_EFFECT_AGI] )
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - following code messed a bit
		// First get max APs we can have
		UINT16 usMaxActionPnts = gubMaxActionPoints[this->ubBodyType];
		//Kaiden: If Enemy, they can max out, but their Max is NOW = MAX + diffAPBonus
		// No sense in giving them a bonus if some of the points are wasted because we
		// Didn't raise their cap by the same amount.
		// squadleader's bonus to APs
		if ( gGameOptions.fNewTraitSystem  && IS_MERC_BODY_TYPE( this ) &&
			 (this->bTeam == ENEMY_TEAM || this->bTeam == MILITIA_TEAM || this->bTeam == gbPlayerNum) )
		{
			// +5% APs per trait (+15% is max)
			usMaxActionPnts += (usMaxActionPnts * gSkillTraitValues.ubSLBonusAPsPercent * GetSquadleadersCountInVicinity( this, FALSE, FALSE ) / 100);
		}

		if ( this->bTeam == ENEMY_TEAM )
		{
			usMaxActionPnts += zDiffSetting[gGameOptions.ubDifficultyLevel].iEnemyAPBonus;
		}
		// Bonus to Militia APs
		else if ( this->bTeam == MILITIA_TEAM )
		{
			if ( this->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.bGreenMilitiaAPsBonus != 0 )
				usMaxActionPnts += gGameExternalOptions.bGreenMilitiaAPsBonus;
			else if ( this->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.bRegularMilitiaAPsBonus != 0 )
				usMaxActionPnts += gGameExternalOptions.bRegularMilitiaAPsBonus;
			else if ( this->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.bVeteranMilitiaAPsBonus != 0 )
				usMaxActionPnts += gGameExternalOptions.bVeteranMilitiaAPsBonus;
		}
		//Kaiden: Players just max out normally unless drugged
		else if ( this->bTeam == gbPlayerNum )
		{
			usMaxActionPnts += gGameExternalOptions.iPlayerAPBonus;
		}
		else
		{
			// Nothing here
			//MaxActionPnts += 0;
		}

		// Flugente: personal AP adjustments
		usMaxActionPnts = (INT16)((usMaxActionPnts * (100 + this->GetAPBonus( ))) / 100);

		// option to make special NPCs stronger - AP bonus
		if ( this->ubProfile != NO_PROFILE && gGameExternalOptions.usSpecialNPCStronger > 0 )
		{
			switch ( this->ubProfile )
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
				// Only 1/4 of the bonus
				usMaxActionPnts += (usMaxActionPnts * gGameExternalOptions.usSpecialNPCStronger / 400);
				break;
			}
		}

		// Now repair
		if ( this->bActionPoints > usMaxActionPnts )
		{
			this->bActionPoints = usMaxActionPnts;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
	}

	this->bInitialActionPoints = this->bActionPoints;

	// Flugente: due to changes and bugs with enemy reinforcements, we now set a flag if a soldier should start with no APs, and act here accordingly
	if ( this->usSoldierFlagMask & SOLDIER_NO_AP )
	{
		this->usSoldierFlagMask &= ~SOLDIER_NO_AP;

		this->bInitialActionPoints = 0;
		this->bActionPoints = 0;
	}
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
		ubTargetStance = gAnimControl[pTSoldier->usAnimState].ubEndHeight;

		// Get his life...if < certain value, do finish!
		// SANDRO - Enhanced Close Combat System - Spinning kick is performed on focused attack
		if ( gGameExternalOptions.fEnhancedCloseCombatSystem && ((this->aiData.bAimTime > 0) && ubTargetStance != ANIM_PRONE) )
		{
			this->ChangeSoldierState( NINJA_SPINKICK, 0, FALSE );

		}
		else if ( !gGameExternalOptions.fEnhancedCloseCombatSystem && (pTSoldier->stats.bLife <= 30 || pTSoldier->bBreath <= 30) && ubTargetStance != ANIM_PRONE )
		{
			// Do finish!
			this->ChangeSoldierState( NINJA_SPINKICK, 0, FALSE );
		}
		else
		{
			if ( ubTargetStance != ANIM_PRONE )
			{
				if ( Random( 2 ) == 0 )
				{
					this->ChangeSoldierState( NINJA_LOWKICK, 0, FALSE );
				}
				else
				{
					this->ChangeSoldierState( NINJA_PUNCH, 0, FALSE );
				}

				// CHECK IF HE CAN SEE US, IF SO CHANGE DIRECTION
				if ( pTSoldier->aiData.bOppList[this->ubID] == 0 && pTSoldier->bTeam != this->bTeam )
				{
					if ( !(pTSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE)) )
					{
						ubTDirection = (UINT8)GetDirectionFromGridNo( this->sGridNo, pTSoldier );
						SendSoldierSetDesiredDirectionEvent( pTSoldier, ubTDirection );
					}
				}
			}
			else
			{
				// CHECK OUR STANCE
				if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH )
				{
					// SET DESIRED STANCE AND SET PENDING ANIMATION
					SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
					this->usPendingAnimation = PUNCH_LOW;
				}
				else
				{
					// USE crouched one
					// NEED TO CHANGE STANCE IF NOT CROUCHD!
					this->EVENT_InitNewSoldierAnim( PUNCH_LOW, 0, FALSE );
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
		memset( &spParms, 0xff, sizeof(SOUNDPARMS) );

		spParms.uiSpeed = RATE_11025;
		spParms.uiVolume = (INT8)CalculateSpeechVolume( HIGHVOLUME );

		// If we are an enemy.....reduce due to volume
		if ( this->bTeam != gbPlayerNum )
		{
			spParms.uiVolume = SoundVolume( (UINT8)spParms.uiVolume, this->sGridNo );
		}
		spParms.uiLoop = 1;
		spParms.uiPan = SoundDir( this->sGridNo );
		spParms.uiPriority = GROUP_PLAYER;

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
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
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
	this->flags.fSoldierWasMoving = TRUE;
	this->iTuringSoundID = NO_SAMPLE;
	this->uiTimeSinceLastBleedGrunt = 0;

	if ( this->ubBodyType == QUEENMONSTER )
	{
		this->iPositionSndID = NewPositionSnd( NOWHERE, POSITION_SOUND_FROM_SOLDIER, (UINT32)this, QUEEN_AMBIENT_NOISE );
	}


	// ANYTHING AFTER HERE CAN FAIL
	do
	{

		if ( usSoldierID <= gTacticalStatus.Team[OUR_TEAM].bLastID )
		{
			this->pKeyRing = (KEY_ON_RING *)MemAlloc( NUM_KEYS * sizeof(KEY_ON_RING) );
			memset( this->pKeyRing, 0, NUM_KEYS * sizeof(KEY_ON_RING) );

			for ( iCounter = 0; iCounter < NUM_KEYS; iCounter++ )
			{
				this->pKeyRing[iCounter].ubKeyID = INVALID_KEY_NUMBER;
			}
		}
		else
		{
			this->pKeyRing = NULL;
		}
		// Create frame cache
		if ( InitAnimationCache( usSoldierID, &(this->AnimCache) ) == FALSE )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed animation cache creation" ) );
			break;
		}

		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
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
			if ( !gfWorldLoaded &&
				 (usState == HOPFENCE || usState == JUMPWINDOWS ||
				 usState == CLIMBDOWNROOF ||

				 usState == JUMPDOWNWALL ||
				 usState == JUMPUPWALL ||

				 usState == FALLFORWARD_ROOF ||
				 usState == FALLOFF ||
				 usState == CLIMBUPROOF) )
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
		if ( this->CreateSoldierPalettes( ) == FALSE )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed in creating soldier palettes" ) );
			break;
		}

		fSuccess = TRUE;

	} while ( FALSE );

	if ( !fSuccess )
	{
		this->DeleteSoldier( );
	}

	return(fSuccess);

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

		if ( !TileIsOutOfBounds( this->sGridNo ) )
		{
			// Remove adjacency records
			for ( bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++ )
			{
				iGridNo = this->sGridNo + DirIncrementer[bDir];
				if ( iGridNo >= 0 && iGridNo < WORLD_MAX )
				{
					gpWorldLevelData[iGridNo].ubAdjacentSoldierCnt--;
				}
			}
		}

		// Delete key ring
		if ( this->pKeyRing )
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
			if ( this->pShades[cnt] != NULL )
			{
				MemFree( this->pShades[cnt] );
				this->pShades[cnt] = NULL;
			}
		}
		for ( cnt = 0; cnt < NUM_SOLDIER_EFFECTSHADES; cnt++ )
		{
			if ( this->pEffectShades[cnt] != NULL )
			{
				MemFree( this->pEffectShades[cnt] );
				this->pEffectShades[cnt] = NULL;
			}
		}

		// Delete glows
		for ( cnt = 0; cnt < 20; cnt++ )
		{
			if ( this->pGlowShades[cnt] != NULL )
			{
				MemFree( this->pGlowShades[cnt] );
				this->pGlowShades[cnt] = NULL;
			}

		}


		if ( this->ubBodyType == QUEENMONSTER )
		{
			DeletePositionSnd( this->iPositionSndID );
		}

		// Free any animations we may have locked...
		UnLoadCachedAnimationSurfaces( this->ubID, &(this->AnimCache) );

		// Free Animation cache
		DeleteAnimationCache( this->ubID, &(this->AnimCache) );

		// Soldier is not active
		this->bActive = FALSE;

		// Remove light
		this->DeleteSoldierLight( );

		// Remove reseved movement value
		UnMarkMovementReserved( this );

	}

	// REMOVE SOLDIER FROM SLOT!
	fRet = RemoveMercSlot( this );

	if ( !fRet )
	{
		RemoveAwaySlot( this );
	}

	return(TRUE);
}

BOOLEAN SOLDIERTYPE::CreateSoldierLight( void )
{
	if ( this->bTeam != gbPlayerNum )
	{
		return(FALSE);
	}

	// DO ONLY IF WE'RE AT A GOOD LEVEL
	if ( this->iLight == -1 )
	{
		INT16 visionrangebonus = GetTotalVisionRangeBonus( this, NORMAL_LIGHTLEVEL_NIGHT );

		// ATE: Check for goggles in headpos....
		if ( visionrangebonus >= UVGOGGLES_BONUS )
		{
			if ( (this->iLight = LightSpriteCreate( "Light4", 0 )) == (-1) )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return(FALSE);
			}
			else
			{
				LightSprites[this->iLight].uiFlags |= MERC_LIGHT;
			}
		}
		else if ( visionrangebonus >= NIGHTSIGHTGOGGLES_BONUS )
		{
			if ( (this->iLight = LightSpriteCreate( "Light3", 0 )) == (-1) )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return(FALSE);
			}
			else
			{
				LightSprites[this->iLight].uiFlags |= MERC_LIGHT;
			}
		}
		else
		{
			if ( (this->iLight = LightSpriteCreate( "Light2", 0 )) == (-1) )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Soldier: Failed loading light" ) );
				return(FALSE);
			}
			else
			{
				LightSprites[this->iLight].uiFlags |= MERC_LIGHT;
			}
		}

		if ( this->pathing.bLevel != 0 )
		{
			LightSpriteRoofStatus( this->iLight, TRUE );
		}
	}

	return(TRUE);
}

BOOLEAN SOLDIERTYPE::ReCreateSoldierLight( void )
{
	if ( this->bTeam != gbPlayerNum )
	{
		return(FALSE);
	}

	if ( !this->bActive )
	{
		return(FALSE);
	}

	if ( !this->bInSector )
	{
		return(FALSE);
	}

	// Delete Light!
	this->DeleteSoldierLight( );

	if ( this->iLight == -1 )
	{
		this->CreateSoldierLight( );
	}

	return(TRUE);
}


BOOLEAN ReCreateSelectedSoldierLight( )
{
	SOLDIERTYPE *pSoldier;

	if ( gusSelectedSoldier == NOBODY )
	{
		return(FALSE);
	}

	pSoldier = MercPtrs[gusSelectedSoldier];

	return(pSoldier->ReCreateSoldierLight( ));
}


BOOLEAN SOLDIERTYPE::DeleteSoldierLight( void )
{
	if ( this->iLight != (-1) )
	{
		LightSpriteDestroy( this->iLight );
		this->iLight = -1;
	}

	return(TRUE);
}

// FUNCTIONS CALLED BY EVENT PUMP
/////////////////////////////////

BOOLEAN SOLDIERTYPE::ChangeSoldierState( UINT16 usNewState, UINT16 usStartingAniCode, BOOLEAN fForce )
{
	EV_S_CHANGESTATE	SChangeState;

	// Send message that we have changed states
	SChangeState.usNewState = usNewState;
	SChangeState.usSoldierID = this->ubID;
	SChangeState.uiUniqueId = this->uiUniqueSoldierIdValue;
	SChangeState.usStartingAniCode = usStartingAniCode;
	SChangeState.sXPos = this->sX;
	SChangeState.sYPos = this->sY;
	SChangeState.fForce = fForce;
	SChangeState.usNewDirection = this->ubDirection;
	SChangeState.usTargetGridNo = this->sTargetGridNo;


	//AddGameEvent( S_CHANGESTATE, 0, &SChangeState );
	if ( (is_server && this->ubID < 120) || (is_client && this->ubID < 20) )
	{
		send_changestate( &SChangeState );
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
	return(TRUE);
}


// This function reevaluates the stance if the guy sees us!
BOOLEAN ReevaluateEnemyStance( SOLDIERTYPE *pSoldier, UINT16 usAnimState )
{
	INT32		cnt, iClosestEnemy = NOBODY;
	INT16		sTargetXPos, sTargetYPos;
	BOOLEAN	fReturnVal = FALSE;
	INT16		sDist, sClosestDist = 10000;

	// make the chosen one not turn to face us
	if ( OK_ENEMY_MERC( pSoldier ) && pSoldier->ubID != gTacticalStatus.ubTheChosenOne && gAnimControl[usAnimState].ubEndHeight == ANIM_STAND && !(pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL) )
	{
		if ( pSoldier->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_OFF )
		{
			// If we are a queen and see enemies, goto ready
			if ( pSoldier->ubBodyType == QUEENMONSTER )
			{
				if ( gAnimControl[usAnimState].uiFlags & (ANIM_BREATH) )
				{
					if ( pSoldier->aiData.bOppCnt > 0 )
					{
						pSoldier->EVENT_InitNewSoldierAnim( QUEEN_INTO_READY, 0, TRUE );
						return(TRUE);
					}
				}
			}

			// ATE: Don't do this if we're not a merc.....
			if ( !IS_MERC_BODY_TYPE( pSoldier ) )
			{
				return(FALSE);
			}

			if ( gAnimControl[usAnimState].uiFlags & (ANIM_MERCIDLE | ANIM_BREATH) )
			{
				if ( pSoldier->aiData.bOppCnt > 0 )
				{
					// Pick a guy this buddy sees and turn towards them!
					for ( cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; cnt++ )
					{
						if ( pSoldier->aiData.bOppList[cnt] == SEEN_CURRENTLY )
						{
							sDist = PythSpacesAway( pSoldier->sGridNo, MercPtrs[cnt]->sGridNo );
							if ( sDist < sClosestDist )
							{
								sClosestDist = sDist;
								iClosestEnemy = cnt;
							}
						}
					}

					if ( iClosestEnemy != NOBODY )
					{

						// SANDRO - do we want this to be happening at all? It is somehow unwelcomed in IIS and for alternative weapon holding, 
						// besides it is rather illogical... well, I've made an ini setting for it			
						if ( gGameExternalOptions.fNoEnemyAutoReadyWeapon == 0 )
						{
							// Change to fire ready animation
							ConvertGridNoToXY( MercPtrs[iClosestEnemy]->sGridNo, &sTargetXPos, &sTargetYPos );

							pSoldier->flags.fDontChargeReadyAPs = TRUE;

							// Ready weapon
							fReturnVal = pSoldier->SoldierReadyWeapon( sTargetXPos, sTargetYPos, FALSE, AIDecideHipOrShoulderStance( pSoldier, MercPtrs[iClosestEnemy]->sGridNo ) );

							return(fReturnVal);
						}
						// this makes the soldier to only turn towards our direction, instead of raising his weapon
						else if ( gGameExternalOptions.fNoEnemyAutoReadyWeapon == 2 )
						{
							//ConvertGridNoToXY( MercPtrs[ iClosestEnemy ]->sGridNo, &sTargetXPos, &sTargetYPos );
							//sFacingDir = GetDirectionFromXY( sXPos, sYPos, pSoldier );
							INT16 sFacingDir = GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, MercPtrs[iClosestEnemy]->sGridNo );

							if ( sFacingDir != pSoldier->ubDirection )
							{
								INT16 sAPCost = GetAPsToLook( pSoldier );

								// Check AP cost...
								if ( !EnoughPoints( pSoldier, sAPCost, 0, TRUE ) )
								{
									return(FALSE);
								}

								SendSoldierSetDesiredDirectionEvent( pSoldier, sFacingDir );
								//fReturnVal = MakeSoldierTurn( pSoldier, sXPos, sYPos );

								return(TRUE);
							}
						}
					}

				}
			}
		}
	}
	return(FALSE);

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

	//if ( usOldAniState != usNewState && ( uiOldAnimFlags & ANIM_HITSTART ) && !( uiNewAnimFlags & ANIM_HITFINISH ) && !( uiNewAnimFlags & ANIM_IGNOREHITFINISH ) && !(pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT ) )
	if ( usOldAniState != usNewState && (uiOldAnimFlags & ANIM_HITSTART) && !(uiNewAnimFlags & ANIM_HITFINISH) && !(pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT) )
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
				pSoldier->DoMercBattleSound( (INT8)(BATTLE_SOUND_CURSE1) );
			}
		}
	}

	// CHECK IF WE HAVE FINSIHED A HIT WHILE DOWN
	// OBLY DO THIS IF 1 ) We are dead already or 2 ) We are alive still
	if ( (uiOldAnimFlags & ANIM_HITWHENDOWN) && ((pSoldier->flags.uiStatusFlags & SOLDIER_DEAD) || pSoldier->stats.bLife != 0) )
	{
		// 0verhaul:  Ditto
		// Release attacker
		// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, animation of kill on the ground ended") );
		// ReleaseSoldiersAttacker( pSoldier );

		//FREEUP GETTING HIT FLAG
		// pSoldier->flags.fGettingHit = FALSE;

		if ( pSoldier->stats.bLife == 0 && pSoldier->ubPreviousAttackerID != NOBODY ) // SANDRO added check
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
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "EVENT_InitNewSoldierAnim" );
	INT32	usNewGridNo = 0;
	INT16	sAPCost = 0;
	INT32	sBPCost = 0;
	UINT32	uiOldAnimFlags;
	UINT32  uiNewAnimFlags;
	UINT16	usSubState;
	UINT16	usItem;
	BOOLEAN	fTryingToRestart = FALSE;

	CHECKF( usNewState < NUMANIMATIONSTATES );

	///////////////////////////////////////////////////////////////////////
	//			DO SOME CHECKS ON OUR NEW ANIMATION!
	/////////////////////////////////////////////////////////////////////

	if (usNewState == THROW_GRENADE_STANCE || usNewState == LOB_GRENADE_STANCE || usNewState == THROW_ITEM || usNewState == THROW_ITEM_CROUCHED)
	{
		UINT16 usItem = this->usGrenadeItem;
		UINT8 ubVolume = Weapon[usItem].ubAttackVolume;

		// play grenade pin sound
		if (usItem && Item[usItem].usItemClass == IC_GRENADE)
		{
			CHAR8	zFilename[512];
			sprintf(zFilename, "");

			BOOLEAN fDelay = FALSE;

			if (usNewState == THROW_GRENADE_STANCE && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && this->ubBodyType < REGFEMALE)
			{
				fDelay = TRUE;
			}

			// check if custom sound is set in Weapons.xml
			if (Weapon[usItem].sSound)
			{
				PlayJA2Sample(Weapon[Item[usItem].ubClassIndex].sSound, RATE_11025, SoundVolume(MIDVOLUME, this->sGridNo), 1, SoundDir(this->sGridNo));
			}
			else
			{
				if (ItemIsFlare(usItem) ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_SIGNAL_SMOKE ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_FLARE)
				{
					if (usItem == BREAK_LIGHT)
					{
						if (fDelay)
							sprintf(zFilename, "sounds\\grenade\\grenade_breaklight_delay.ogg");
						else
							sprintf(zFilename, "sounds\\grenade\\grenade_breaklight.ogg");
					}
					else
					{
						if (fDelay)
							sprintf(zFilename, "sounds\\grenade\\grenade_flare_delay.ogg");
						else
							sprintf(zFilename, "sounds\\grenade\\grenade_flare.ogg");
					}
				}
				else if (Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_NORMAL ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_STUN ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_FLASHBANG)
				{
					if (fDelay)
						sprintf(zFilename, "sounds\\grenade\\grenade_pin_delay.ogg");
					else
						sprintf(zFilename, "sounds\\grenade\\grenade_pin.ogg");
				}
				else if (Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_SMOKE ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_TEARGAS ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_MUSTGAS ||
					Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_SIGNAL_SMOKE)
				{
					if (fDelay)
						sprintf(zFilename, "sounds\\grenade\\grenade_gas_delay.ogg");
					else
						sprintf(zFilename, "sounds\\grenade\\grenade_gas.ogg");
				}
				else if (Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_BURNABLEGAS)
				{
					if (fDelay)
						sprintf(zFilename, "sounds\\grenade\\grenade_fire_delay.ogg");
					else
						sprintf(zFilename, "sounds\\grenade\\grenade_fire.ogg");
				}

				if (strlen(zFilename) > 0 && FileExists(zFilename))
				{
					PlayJA2SampleFromFile(zFilename, RATE_11025, SoundVolume(MIDVOLUME, this->sGridNo), 1, SoundDir(this->sGridNo));
				}
			}
		}
	}

	// If we are NOT loading a game, continue normally
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		usItem = this->inv[HANDPOS].usItem;

		// CHECK IF WE ARE TRYING TO INTURRUPT A SCRIPT WHICH WE DO NOT WANT INTERRUPTED!
		if ( this->flags.fInNonintAnim )
		{
			return(FALSE);
		}

		if ( this->flags.fRTInNonintAnim )
		{
			if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
			{
				return(FALSE);
			}
			else
			{
				this->flags.fRTInNonintAnim = FALSE;
			}
		}


		// Check if we can restart this animation if it's the same as our current!
		if ( usNewState == this->usAnimState )
		{
			if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_NORESTART) && !fForce )
			{
				fTryingToRestart = TRUE;
			}
		}

		// Check state, if we are not at the same height, set this ani as the pending one and
		// change stance accordingly
		// ATE: ONLY IF WE ARE STARTING AT START OF ANIMATION!
		if ( usStartingAniCode == 0 )
		{
			if ( gAnimControl[usNewState].ubHeight != gAnimControl[this->usAnimState].ubEndHeight &&
				 !(gAnimControl[usNewState].uiFlags & (ANIM_STANCECHANGEANIM | ANIM_IGNORE_AUTOSTANCE)) )
			{

				// Check if we are going from crouched height to prone height, and adjust fast turning accordingly
				// Make guy turn while crouched THEN go into prone
				if ( (gAnimControl[usNewState].ubEndHeight == ANIM_PRONE && gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH) && !(gTacticalStatus.uiFlags & INCOMBAT) )
				{
					this->flags.fTurningUntilDone = TRUE;
					this->ubPendingStanceChange = gAnimControl[usNewState].ubEndHeight;
					this->usPendingAnimation = usNewState;
					return(TRUE);
				}
				// Check if we are in realtime and we are going from stand to crouch
				else if ( gAnimControl[usNewState].ubEndHeight == ANIM_CROUCH && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && (gAnimControl[this->usAnimState].uiFlags & ANIM_MOVING) && ((gTacticalStatus.uiFlags & REALTIME) || !(gTacticalStatus.uiFlags & INCOMBAT)) )
				{
					this->ubDesiredHeight = gAnimControl[usNewState].ubEndHeight;
					// Continue with this course of action IE: Do animation and skip from stand to crouch
				}
				// Check if we are in realtime and we are going from crouch to stand
				else if ( gAnimControl[usNewState].ubEndHeight == ANIM_STAND && gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH && (gAnimControl[this->usAnimState].uiFlags & ANIM_MOVING) && ((gTacticalStatus.uiFlags & REALTIME) || !(gTacticalStatus.uiFlags & INCOMBAT)) && this->usAnimState != HELIDROP )
				{
					this->ubDesiredHeight = gAnimControl[usNewState].ubEndHeight;
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
						SendChangeSoldierStanceEvent( this, gAnimControl[usNewState].ubHeight );
						return(TRUE);
					}
				}
			}

			// Going from hip stance to shoulder stance, skip first 2 frames for smoother graphic look
			if ( usNewState == READY_RIFLE_STAND && (gAnimControl[this->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING)) )
			{
				if ( this->ubBodyType == BIGMALE )
					usStartingAniCode = 1; // this looks better for big mercs
				else
					usStartingAniCode = 2;
			}
			// Going from shoulder stance to hip stance
			else if ( usNewState == READY_ALTERNATIVE_STAND && (gAnimControl[this->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) )
			{
				if (ItemIsTwoHanded(this->inv[HANDPOS].usItem))
					usStartingAniCode = 1;
				else
					usStartingAniCode = 2;
			}
		}

		if ( usNewState == ADJACENT_GET_ITEM )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				EVENT_InternalSetSoldierDesiredDirection( this, this->ubPendingDirection, FALSE, this->usAnimState );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = ADJACENT_GET_ITEM;
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		if ( usNewState == ADJACENT_GET_ITEM_CROUCHED )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				EVENT_InternalSetSoldierDesiredDirection( this, this->ubPendingDirection, FALSE, this->usAnimState );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = ADJACENT_GET_ITEM_CROUCHED;
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		if ( usNewState == CLIMBUPROOF )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = CLIMBUPROOF;
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
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
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		if ( usNewState == JUMPUPWALL )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = JUMPUPWALL;
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		if ( usNewState == JUMPDOWNWALL )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = JUMPDOWNWALL;
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
				this->flags.fTurningUntilDone = TRUE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		if ( usNewState == START_AID_PRN )
		{
			if ( this->ubPendingDirection != NO_PENDING_DIRECTION )
			{
				this->EVENT_SetSoldierDesiredDirection( this->ubPendingDirection );
				this->ubPendingDirection = NO_PENDING_DIRECTION;
				this->usPendingAnimation = START_AID_PRN;
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_ON;
				this->flags.fTurningUntilDone = TRUE;
				this->ubPendingStanceChange = ANIM_PRONE;
				this->SoldierGotoStationaryStance( );
				return(TRUE);
			}
		}

		// ATE: Don't raise/lower automatically if we are low on health,
		// as our gun looks lowered anyway....
		//if ( this->stats.bLife > INJURED_CHANGE_THREASHOLD )
		if ( !this->MercInWater( ) )
		{
			// Don't do some of this if we are a monster!
			// ATE: LOWER AIMATION IS GOOD, RAISE ONE HOWEVER MAY CAUSE PROBLEMS FOR AI....
			if ( !(this->flags.uiStatusFlags & SOLDIER_MONSTER) && this->ubBodyType != ROBOTNOWEAPON && this->bTeam == gbPlayerNum )
			{
				// If this animation is a raise_weapon animation
				if ( (gAnimControl[usNewState].uiFlags & ANIM_RAISE_WEAPON) && !(gAnimControl[this->usAnimState].uiFlags & (ANIM_RAISE_WEAPON | ANIM_NOCHANGE_WEAPON)) )
				{
					// We are told that we need to rasie weapon
					// Do so only if
					// 1) We have a rifle in hand...
					//usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[HANDPOS].exists( ) == true && ItemIsTwoHanded(usItem) && !ItemIsRocketLauncher(usItem) )
					{
						// Switch on height!
						switch ( gAnimControl[this->usAnimState].ubEndHeight )
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
				if ( (gAnimControl[usNewState].uiFlags & ANIM_LOWER_WEAPON) && !(gAnimControl[this->usAnimState].uiFlags & (ANIM_LOWER_WEAPON | ANIM_NOCHANGE_WEAPON)) )
				{
					// We are told that we need to rasie weapon
					// Do so only if
					// 1) We have a rifle in hand...
					//usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[HANDPOS].exists( ) == true && ItemIsTwoHanded(usItem) && !ItemIsRocketLauncher(usItem) )
					{
						// Switch on height!
						switch ( gAnimControl[this->usAnimState].ubEndHeight )
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

		// Are we cowering and are trying to move, getup first...
		//if ( gAnimControl[ usNewState ].uiFlags & ANIM_MOVING && this->usAnimState == COWERING && gAnimControl[ usNewState ].ubEndHeight == ANIM_STAND )
		if ( this->usAnimState == COWERING )
		{
			if ( gAnimControl[usNewState].ubEndHeight == ANIM_STAND )
			{
				this->usPendingAnimation = usNewState;
				this->ubDesiredHeight = ANIM_STAND;
				usNewState = END_COWER;
			}
			else if ( gAnimControl[usNewState].ubEndHeight == ANIM_CROUCH )
			{
				this->usPendingAnimation = usNewState;
				this->ubDesiredHeight = ANIM_CROUCH;
				usNewState = END_COWER_CROUCHED;
			}
		}
		else if ( this->usAnimState == COWERING_PRONE )
		{
			if ( gAnimControl[usNewState].ubEndHeight == ANIM_PRONE )
			{
				this->usPendingAnimation = usNewState;
				this->ubDesiredHeight = ANIM_PRONE;
				usNewState = END_COWER_PRONE;
			}
		}

		// If we want to start swatting, put a pending animation
		if ( this->usAnimState != START_SWAT && usNewState == SWATTING )
		{
			// Set new state to be animation to move to new stance
			usNewState = START_SWAT;
		}

		if ( this->usAnimState == SWATTING && usNewState == CROUCHING )
		{
			// Set new state to be animation to move to new stance
			usNewState = END_SWAT;
		}
		///***ddd{
		if ( (this->usAnimState == SWATTING_WK || this->usAnimState == SWAT_BACKWARDS
			|| this->usAnimState == SWAT_BACKWARDS_NOTHING || this->usAnimState == SWAT_BACKWARDS_WK)
			&& usNewState == CROUCHING )
		{
			// Set new state to be animation to move to new stance
			usNewState = END_SWAT;
		}
		///***ddd}

		if ( this->usAnimState == WALKING && usNewState == STANDING && this->stats.bLife < INJURED_CHANGE_THREASHOLD && this->ubBodyType <= REGFEMALE && !this->MercInWater( ) )
		{
			// Set new state to be animation to move to new stance
			usNewState = END_HURT_WALKING;
		}

		// Check if we are an enemy, and we are in an animation what should be overriden
		// by if he sees us or not.
		if ( ReevaluateEnemyStance( this, usNewState ) )
		{
			return(TRUE);
		}

		// OK.......
		// SANDRO - removing unused code
		/*if ( this->ubBodyType > REGFEMALE )
		{
		if ( this->stats.bLife < INJURED_CHANGE_THREASHOLD )
		{
		if ( usNewState == READY_RIFLE_STAND )
		{
		//	this->usPendingAnimation2 = usNewState;
		//	usNewState = FROM_INJURED_TRANSITION;
		}
		}
		}*/

		// Alrighty, check if we should free buddy up!
		if ( usNewState == GIVING_AID || usNewState == GIVING_AID_PRN )
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
			return(FALSE);
		}

		// OK, make guy transition if a big merc...
		//if ( ( this->uiAnimSubFlags & SUB_ANIM_BIGGUYTHREATENSTANCE ) && !( this->uiAnimSubFlags & SUB_ANIM_BIGGUYSHOOT2 ) )
		if ( this->ubBodyType == BIGMALE )
		{
			// SANDRO - we are changing crouching animation here to the old vanilla one, don't do that if alt animations are used
			if ( !DecideAltAnimForBigMerc( this ) )
			{
				if ( usNewState == KNEEL_DOWN && this->usAnimState != BIGMERC_CROUCH_TRANS_INTO )
				{
					//UINT16 usItem;

					// Do we have a rifle?
					//usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[HANDPOS].exists( ) == true )
					{
						if ( Item[usItem].usItemClass == IC_GUN && !ItemIsRocketLauncher(usItem) )
						{
							//						if ( (Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
							if (ItemIsTwoHanded(usItem))
							{
								usNewState = BIGMERC_CROUCH_TRANS_INTO;
							}
						}
					}
				}

				if ( usNewState == KNEEL_UP && this->usAnimState != BIGMERC_CROUCH_TRANS_OUTOF )
				{
					//UINT16 usItem;

					// Do we have a rifle?
					//usItem = this->inv[ HANDPOS ].usItem;

					if ( this->inv[HANDPOS].exists( ) == true )
					{
						if ( Item[usItem].usItemClass == IC_GUN && !ItemIsRocketLauncher(usItem) )
						{
							//						if ( (Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
							if (ItemIsTwoHanded(usItem))
							{
								usNewState = BIGMERC_CROUCH_TRANS_OUTOF;
							}
						}
					}
				}
			}
		}

		// OK, if we have reverse set, do the side step!
		if ( this->bReverse )
		{
			if ( usNewState == WALKING || usNewState == RUNNING || usNewState == SWATTING
				 //*** ddd
				 || usNewState == SWATTING_WK || usNewState == START_SWAT )
			{
				// CHECK FOR SIDEWAYS!
				if ( !(this->flags.uiStatusFlags & SOLDIER_VEHICLE) && this->ubDirection == gPurpendicularDirection[this->ubDirection][this->pathing.usPathingData[this->pathing.usPathIndex]] )
				{
					// We are perpendicular!
					// SANDRO - wait wait wait!!! We need to determine if gonna sidestep with weapon raised
					if ( ((gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY) ||
						(gAnimControl[this->usAnimState].uiFlags & ANIM_FIRE)) && gGameExternalOptions.fAllowWalkingWithWeaponRaised )
					{
						if ( this->inv[HANDPOS].exists( ) == true && Item[usItem].usItemClass == IC_GUN && !ItemIsRocketLauncher(usItem) )
						{
							if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND )
							{
								if ( this->IsValidSecondHandShot() )
								{
									usNewState = SIDE_STEP_DUAL_RDY;
								}
								else if ( gAnimControl[this->usAnimState].uiFlags & ANIM_ALT_WEAPON_HOLDING )
								{
									usNewState = SIDE_STEP_ALTERNATIVE_RDY;
								}
								else
								{
									usNewState = SIDE_STEP_WEAPON_RDY;
								}
							}
						}
					}
					else
					{
						if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND )
						{
							usNewState = SIDE_STEP;
						}
						// comment in once animations are ready
						/*else if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH )
						{
							if ( this->IsValidSecondHandShot() )
							{
								usNewState = SIDE_STEP_CROUCH_DUAL;
							}
							else if ( !Item[this->inv[HANDPOS].usItem].twohanded )
							{
								usNewState = SIDE_STEP_CROUCH_PISTOL;
							}
							else
							{
								usNewState = SIDE_STEP_CROUCH_RIFLE;
							}
						}*/
					}
				}
				else
				{
					if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH )
					{
						if ( this->inv[HANDPOS].exists( ) == true && Item[usItem].usItemClass == IC_GUN && ItemIsTwoHanded(usItem) && !ItemIsRocketLauncher(usItem) )
							usNewState = SWAT_BACKWARDS;
						else
							usNewState = SWAT_BACKWARDS_NOTHING;
						// move backward crouching, with a knife
						if ( this->inv[HANDPOS].exists( ) == true &&
							 //(this->ubBodyType == BIGMALE || this->ubBodyType == REGFEMALE )&&
							 (Item[usItem].usItemClass == IC_BLADE || Item[usItem].usItemClass == IC_THROWING_KNIFE) )
							 usNewState = SWAT_BACKWARDS_WK;

					}
					else
					{
						// Here, change to  opposite direction
						usNewState = WALK_BACKWARDS;
					}
				}
			}
			//***08.12.2008*** added roll animation ;) ddd			
			else if ( usNewState == CRAWLING
					  && this->ubDirection ==
					  gPurpendicularDirection[this->ubDirection][this->pathing.usPathingData
					  [this->pathing.usPathIndex]] )
			{
				if ( QuickestDirection( this->ubDirection, this->pathing.usPathingData[this->pathing.usPathIndex] ) > 0 )
					usNewState = ROLL_PRONE_R;
				else if ( QuickestDirection( this->ubDirection, this->pathing.usPathingData[this->pathing.usPathIndex] ) < 0 )
					usNewState = ROLL_PRONE_L;

				if ( usNewState != CRAWLING )
				{
					if ( this->ubDirection % 2 == 0 )
						gAnimControl[usNewState].dMovementChange = (FLOAT)0.8;
					else
						gAnimControl[usNewState].dMovementChange = (FLOAT)1.1;
				}
			}///

		}

		// ATE: Patch hole for breath collapse for roofs, fences
		if ( usNewState == JUMPUPWALL || usNewState == JUMPDOWNWALL || usNewState == CLIMBUPROOF || usNewState == CLIMBDOWNROOF || usNewState == HOPFENCE || usNewState == JUMPWINDOWS )
		{
			// Check for breath collapse if a given animation like
			if ( this->CheckForBreathCollapse( ) || this->bCollapsed )
			{
				// UNset UI
				UnSetUIBusy( this->ubID );

				SoldierCollapse( this );

				this->bBreathCollapsed = FALSE;

				return(FALSE);

			}
		}

		// If we are in water.....and trying to run, change to run
		if ( this->MercInWater( ) )
		{
			// Check animation
			// Change to walking
			if ( usNewState == RUNNING )
			{
				usNewState = WALKING;
			}
		}
		// SANDRO - check if we are gonna move with weapon raised
		else if ( gGameExternalOptions.fAllowWalkingWithWeaponRaised && ( (gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY) || (gAnimControl[this->usAnimState].uiFlags & ANIM_FIRE) ) )
		{
			if ( this->inv[HANDPOS].exists( ) == true && Item[usItem].usItemClass == IC_GUN && !ItemIsRocketLauncher(usItem) )
			{
				if ( usNewState == WALKING )
				{
					if (this->IsValidSecondHandShot())
					{
						usNewState = WALKING_DUAL_RDY;
					}
					else if (gAnimControl[this->usAnimState].uiFlags & ANIM_ALT_WEAPON_HOLDING)
					{
						usNewState = WALKING_ALTERNATIVE_RDY;
					}
					else
					{
						usNewState = WALKING_WEAPON_RDY;
					}
				}
				else if (usNewState == SWATTING || usNewState == START_SWAT )
				{
					if ( this->IsValidSecondHandShot() )
					{
						usNewState = CROUCHEDMOVE_DUAL_READY;
					}
					else if (!ItemIsTwoHanded(this->inv[HANDPOS].usItem))
					{
						usNewState = CROUCHEDMOVE_PISTOL_READY;
					}
					else
					{
						usNewState = CROUCHEDMOVE_RIFLE_READY;
					}
				}
			}
		}
		
		// Turn off anipause flag for any anim!
		this->flags.uiStatusFlags &= (~SOLDIER_PAUSEANIMOVE);

		// Unset paused for no APs.....
		this->AdjustNoAPToFinishMove( FALSE );


		// We are about to start moving
		// Handle buddy beginning to move...
		// check new gridno, etc
		// ATE: Added: Make check that old anim is not a moving one as well
		if ( gAnimControl[usNewState].uiFlags & ANIM_MOVING && !(gAnimControl[this->usAnimState].uiFlags & ANIM_MOVING) || (gAnimControl[usNewState].uiFlags & ANIM_MOVING && fForce) )
		{
			BOOLEAN fKeepMoving;

			if ( usNewState == CRAWLING && this->usDontUpdateNewGridNoOnMoveAnimChange == LOCKED_NO_NEWGRIDNO )
			{
				// Turn off lock once we are crawling once...
				this->usDontUpdateNewGridNoOnMoveAnimChange = 1;
			}

			// ATE: Additional check here if we have just been told to update animation ONLY, not goto gridno stuff...
			if ( !this->usDontUpdateNewGridNoOnMoveAnimChange )
			{
				if ( usNewState != SWATTING )
				{
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Handling New gridNo for %d: Old %s, New %s", this->ubID, gAnimControl[this->usAnimState].zAnimStr, gAnimControl[usNewState].zAnimStr ) );

					if ( !(gAnimControl[usNewState].uiFlags & ANIM_SPECIALMOVE) )
					{
						// Handle goto new tile...
						if ( HandleGotoNewGridNo( this, &fKeepMoving, TRUE, usNewState ) )
						{
							if ( !fKeepMoving )
							{
								return(FALSE);
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
							return(FALSE);
						}
					}
					else
					{
						// Change desired direction
						// Just change direction
						this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[this->pathing.usPathIndex], FALSE, this->usAnimState );
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
				this->SetSoldierHeight( (FLOAT)(0) );
			}

		}

		// Reset to false always.....
		// ( Unless locked )
		if ( gAnimControl[usNewState].uiFlags & ANIM_MOVING )
		{
			// 0verhaul:  **** Special hack!!!!
			//   If a merc begins to go prone while moving, the LOCKED_NO_NEWGRIDNO is set.  If the merc never finishes
			// going prone, either due to interrupting the stance change with a different stance change, or other possible
			// factors such as maybe getting shot (this is realtime so an enemy could see him), it stays on locked.  Once
			// it stays on locked, the soldier will be unable to navigate around obstacles but will simply stay put
			// twitching.  Since the LOCKED is only set when going prone, this unsets it.
			if ( this->usDontUpdateNewGridNoOnMoveAnimChange != LOCKED_NO_NEWGRIDNO ||
				 (this->usDontUpdateNewGridNoOnMoveAnimChange == LOCKED_NO_NEWGRIDNO && this->usAnimState != PRONE_DOWN) )
			{
				this->usDontUpdateNewGridNoOnMoveAnimChange = FALSE;
			}
		}

		if ( fTryingToRestart )
		{
			return(FALSE);
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

	uiOldAnimFlags = gAnimControl[this->usAnimState].uiFlags;
	uiNewAnimFlags = gAnimControl[usNewState].uiFlags;

	usNewGridNo = NewGridNo( this->sGridNo, DirectionInc( (UINT8) this->pathing.usPathingData[this->pathing.usPathIndex] ) );


	// CHECKING IF WE HAVE A HIT FINISH BUT NO DEATH IS DONE WITH A SPECIAL ANI CODE
	// IN THE HIT FINSIH ANI SCRIPTS

	// CHECKING IF WE HAVE FINISHED A DEATH ANIMATION IS DONE WITH A SPECIAL ANI CODE
	// IN THE DEATH SCRIPTS


	// CHECK IF THIS NEW STATE IS NON-INTERRUPTABLE
	// IF SO - SET NON-INT FLAG
	// 0verhaul:  Okay, here is a question:  Is the "non-interrupt" supposed to be transferrable to other anims?
	// That is, if one anim is not interruptable but it chains to another anim, should the "not interruptable" flag
	// remain?  I'm going to try out the theory that new animations should reset the "don't interrupt" flag.
	this->flags.fInNonintAnim = (uiNewAnimFlags & ANIM_NONINTERRUPT) != 0;
	this->flags.fRTInNonintAnim = (uiNewAnimFlags & ANIM_RT_NONINTERRUPT) != 0;

	// CHECK IF WE ARE NOT AIMING, IF NOT, RESET LAST TAGRET!
	if ( !(gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY) && !(gAnimControl[usNewState].uiFlags & ANIM_FIREREADY) )
	{
		// ATE: Also check for the transition anims to not reset this
		// this should have used a flag but we're out of them....
		if ( usNewState != READY_ALTERNATIVE_STAND && usNewState != READY_RIFLE_STAND && usNewState != READY_RIFLE_PRONE && usNewState != READY_RIFLE_CROUCH && usNewState != ROBOT_SHOOT && usNewState != TANK_SHOOT && usNewState != TANK_BURST && usNewState != THROW_KNIFE && usNewState != THROW_KNIFE_SP_BM && this->usAnimState != THROW_KNIFE && this->usAnimState != THROW_KNIFE_SP_BM )//dnl ch64 300813 //dnl ch70 170913
		{
			this->sLastTarget = NOWHERE;
		}
	}

	// If a special move state, release np aps
	if ( (gAnimControl[usNewState].uiFlags & ANIM_SPECIALMOVE) )
	{
		this->AdjustNoAPToFinishMove( FALSE );
	}

	if ( gAnimControl[usNewState].uiFlags & ANIM_UPDATEMOVEMENTMODE )
	{
		if ( this->bTeam == gbPlayerNum )
		{
			// this->usUIMovementMode =  GetMoveStateBasedOnStance( this, gAnimControl[ usNewState ].ubEndHeight );
		}
	}

	// ATE: If not a moving animation - turn off reverse....
	if ( !(gAnimControl[usNewState].uiFlags & ANIM_MOVING) )
	{
		this->bReverse = FALSE;
	}

	// ONLY DO FOR EVERYONE BUT PLANNING GUYS
	if ( this->ubID < MAX_NUM_SOLDIERS )
	{
		// Do special things based on new state
		// CHRISL: Make changes so that we charge extra APs while wearing a backpack while using new inventory system
		switch ( usNewState )
		{
		case STANDING:

			// Update desired height
			this->ubDesiredHeight = ANIM_STAND;
			break;

		case CROUCHING:

			// Update desired height
			this->ubDesiredHeight = ANIM_CROUCH;
			break;

		case PRONE:

			// Update desired height
			this->ubDesiredHeight = ANIM_PRONE;
			break;

		case READY_RIFLE_STAND:
		case READY_RIFLE_PRONE:
		case READY_RIFLE_CROUCH:
		case READY_DUAL_STAND:
		case READY_DUAL_CROUCH:
		case READY_DUAL_PRONE:
		case READY_ALTERNATIVE_STAND:

			// OK, get points to ready weapon....
			if ( !this->flags.fDontChargeReadyAPs )
			{
				sAPCost = GetAPsToReadyWeapon( this, usNewState );
				// SANDRO - get BP cost for weapon manipulating
				if ( gGameExternalOptions.ubEnergyCostForWeaponWeight )
					sBPCost = sAPCost * GetBPCostPer10APsForGunHolding( this ) / 10;
				else
					sBPCost = 0;
				DeductPoints( this, sAPCost, sBPCost, BEFORESHOT_INTERRUPT );
			}
			else
			{
				this->flags.fDontChargeReadyAPs = FALSE;
			}
			break;

		case WALKING:
		case WALKING_WEAPON_RDY:
		case WALKING_DUAL_RDY:
		case WALKING_ALTERNATIVE_RDY:

			this->usPendingAnimation = NO_PENDING_ANIMATION;
			this->aiData.ubPendingActionAnimCount = 0;
			break;

		case SWATTING:
		case CROUCHEDMOVE_RIFLE_READY:
		case CROUCHEDMOVE_PISTOL_READY:
		case CROUCHEDMOVE_DUAL_READY:

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
			/* //shadooow: moved to ActionPointCost
			if ( this->usAnimState != RUNNING )
			{
				// CHRISL
				if ( (UsingNewInventorySystem( ) == true) && FindBackpackOnSoldier( this ) != ITEM_NOT_FOUND )
				{
					sAPCost = GetAPsStartRun( this ) + 2; // changed by SANDRO
					sBPCost += 2;
				}
				else
					sAPCost = GetAPsStartRun( this ); // changed by SANDRO
				DeductPoints( this, sAPCost, sBPCost, MOVEMENT_INTERRUPT );
			}
			*/
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
				if ( UsingNewInventorySystem( ) )
				{
					if ( usNewState == KNEEL_UP || usNewState == BIGMERC_CROUCH_TRANS_OUTOF )
					{
						sAPCost = GetAPsCrouch( this, TRUE * 2 );
						sBPCost = APBPConstants[BP_CROUCH] + 2;
					}
					else
					{
						sAPCost = GetAPsCrouch( this, TRUE );
						sBPCost = APBPConstants[BP_CROUCH] + 1;
					}
				}
				else
				{
					sAPCost = GetAPsCrouch( this, FALSE );
					sBPCost = APBPConstants[BP_CROUCH];
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
				if ( UsingNewInventorySystem( ) )
				{
					if ( usNewState == PRONE_UP )
					{
						sAPCost = GetAPsProne( this, TRUE * 2 );
						sBPCost = APBPConstants[BP_PRONE] + 2;
					}
					else
					{
						sAPCost = GetAPsProne( this, TRUE );
						sBPCost = APBPConstants[BP_PRONE] + 1;
					}
				}
				else
				{
					sAPCost = GetAPsProne( this, FALSE );
					sBPCost = APBPConstants[BP_PRONE];
				}
				DeductPoints( this, sAPCost, sBPCost );
			}
			this->flags.fDontChargeAPsForStanceChange = FALSE;
			break;

			//Deduct points for stance change
			//sAPCost = GetAPsToChangeStance( this, gAnimControl[ usNewState ].ubEndHeight );
			//DeductPoints( this, sAPCost, 0 );
			//break;

		case START_AID:
		case START_AID_PRN:

			DeductPoints( this, APBPConstants[AP_START_FIRST_AID], APBPConstants[BP_START_FIRST_AID] );
			break;

		case CUTTING_FENCE:
			DeductPoints( this, APBPConstants[AP_USEWIRECUTTERS], APBPConstants[BP_USEWIRECUTTERS], AFTERACTION_INTERRUPT );
			break;

		case PLANT_BOMB:

			if (ItemIsMine(this->inv[HANDPOS].usItem))	// bury a mine
				DeductPoints( this, GetAPsToPlantMine( this ), APBPConstants[BP_BURY_MINE] ); // changed by SANDRO
			else
				DeductPoints( this, GetAPsToDropBomb( this ), APBPConstants[BP_DROP_BOMB] ); // changed by SANDRO
			break;

		case STEAL_ITEM:

			// we will deduct APs for this elsewhere (see weapons.cpp) - SANDRO
			//DeductPoints( this, APBPConstants[AP_STEAL_ITEM], 0 );
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

			DeductPoints( this, APBPConstants[AP_USE_REMOTE], 0, AFTERACTION_INTERRUPT );
			break;

		case REFUEL_VEHICLE:

			DeductPoints(this, APBPConstants[AP_REFUEL_VEHICLE], 0, AFTERACTION_INTERRUPT);
			break;

		case GOTO_REPAIRMAN:

			DeductPoints(this, APBPConstants[AP_START_REPAIR], 0, AFTERACTION_INTERRUPT);
			break;

		case TAKE_BLOOD_FROM_CORPSE:

			DeductPoints(this, APBPConstants[AP_TAKE_BLOOD], 0, AFTERACTION_INTERRUPT);
			break;

			//case PUNCH:

			//Deduct points for punching
			//sAPCost = MinAPsToAttack( this, this->sGridNo, FALSE );
			//DeductPoints( this, sAPCost, 0 );
			//break;

		case HOPFENCE:

			// CHRISL
			// SANDRO - changed this a bit
			if ( (UsingNewInventorySystem( ) == true) && FindBackpackOnSoldier( this ) != ITEM_NOT_FOUND )
				DeductPoints( this, GetAPsToJumpFence( this, TRUE ), GetBPsToJumpFence( this, TRUE ), SP_MOVEMENT_INTERRUPT );
			else
				DeductPoints( this, GetAPsToJumpFence( this, FALSE ), GetBPsToJumpFence( this, FALSE ), SP_MOVEMENT_INTERRUPT );
			break;

		case JUMPWINDOWS:
			if ( (UsingNewInventorySystem( ) == true) && FindBackpackOnSoldier( this ) != ITEM_NOT_FOUND )
				DeductPoints( this, GetAPsToJumpThroughWindows( this, TRUE ), GetBPsToJumpThroughWindows( this, TRUE ), SP_MOVEMENT_INTERRUPT );
			else
				DeductPoints( this, GetAPsToJumpThroughWindows( this, FALSE ), GetBPsToJumpThroughWindows( this, FALSE ), SP_MOVEMENT_INTERRUPT );
			break;

			// Deduct aps for falling down....
		case FALLBACK_HIT_STAND:
		case FALLFORWARD_FROMHIT_STAND:

			DeductPoints( this, APBPConstants[AP_FALL_DOWN], APBPConstants[BP_FALL_DOWN], DISABLED_INTERRUPT );
			break;

		case FALLFORWARD_FROMHIT_CROUCH:

			DeductPoints( this, (APBPConstants[AP_FALL_DOWN] / 2), (APBPConstants[BP_FALL_DOWN] / 2), DISABLED_INTERRUPT );
			break;

		case QUEEN_SWIPE:

			// ATE: set damage counter...
			this->aiData.uiPendingActionData1 = 0;
			break;

		case CLIMBDOWNROOF:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, GetAPsToClimbRoof( this, TRUE ), GetBPsToClimbRoof( this, TRUE ), SP_MOVEMENT_INTERRUPT ); // changed by SANDRO
			break;

		case CLIMBUPROOF:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, GetAPsToClimbRoof( this, FALSE ), GetBPsToClimbRoof( this, FALSE ), SP_MOVEMENT_INTERRUPT ); // changed by SANDRO
			break;

		case JUMPDOWNWALL:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, GetAPsToJumpWall( this, TRUE ), GetBPsToJumpWall( this, TRUE ), SP_MOVEMENT_INTERRUPT );
			break;

		case JUMPUPWALL:

			// disable sight
			gTacticalStatus.uiFlags |= DISALLOW_SIGHT;

			DeductPoints( this, GetAPsToJumpWall( this, FALSE ), GetBPsToJumpWall( this, FALSE ), SP_MOVEMENT_INTERRUPT );
			break;

		case JUMP_OVER_BLOCKING_PERSON:
			// Set path....
		{
										  INT32 usNewGridNo;

										  DeductPoints( this, GetAPsToJumpOver( this ), APBPConstants[BP_JUMP_OVER], SP_MOVEMENT_INTERRUPT );

										  usNewGridNo = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );
										  usNewGridNo = NewGridNo( usNewGridNo, DirectionInc( this->ubDirection ) );

										  this->sPlotSrcGrid = this->sGridNo;
										  this->flags.fPastXDest = FALSE;
										  this->flags.fPastYDest = FALSE;
										  this->pathing.usPathDataSize = 0;
										  this->pathing.usPathIndex = 0;
										  this->pathing.usPathingData[this->pathing.usPathDataSize] = this->ubDirection;
										  this->pathing.usPathDataSize++;
										  this->pathing.usPathingData[this->pathing.usPathDataSize] = this->ubDirection;
										  this->pathing.usPathDataSize++;
										  this->pathing.sFinalDestination = usNewGridNo;
										  // Set direction
										  this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[this->pathing.usPathIndex], FALSE, JUMP_OVER_BLOCKING_PERSON );
		}
			break;

		case LONG_JUMP:
			// Set path....
		{
						  INT32 usNewGridNo;

						  DeductPoints( this, GetAPsToJumpOver( this ), APBPConstants[BP_JUMP_OVER], SP_MOVEMENT_INTERRUPT );

						  usNewGridNo = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );
						  usNewGridNo = NewGridNo( usNewGridNo, DirectionInc( this->ubDirection ) );
						  usNewGridNo = NewGridNo( usNewGridNo, DirectionInc( this->ubDirection ) );

						  this->sPlotSrcGrid = this->sGridNo;
						  this->flags.fPastXDest = FALSE;
						  this->flags.fPastYDest = FALSE;
						  this->pathing.usPathDataSize = 0;
						  this->pathing.usPathIndex = 0;
						  this->pathing.usPathingData[this->pathing.usPathDataSize] = this->ubDirection;
						  this->pathing.usPathDataSize++;
						  this->pathing.usPathingData[this->pathing.usPathDataSize] = this->ubDirection;
						  this->pathing.usPathDataSize++;
						  this->pathing.usPathingData[this->pathing.usPathDataSize] = this->ubDirection;
						  this->pathing.usPathDataSize++;
						  this->pathing.sFinalDestination = usNewGridNo;
						  // Set direction
						  this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[this->pathing.usPathIndex], FALSE, LONG_JUMP );
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
			this->aiData.uiPendingActionData1 = PlaySoldierJA2Sample( this->ubID, (FIRE_ON_MERC), RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 5, SoundDir( this->sGridNo ), TRUE );
			break;

		case CRYO_DEATH:
			PlayJA2StreamingSampleFromFile( "stsounds\\CryoBlast_1.ogg", RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 1, MIDDLEPAN, NULL );
			break;

		case CRYO_DEATH_CROUCHED:
			PlayJA2StreamingSampleFromFile( "stsounds\\CryoBlast_2.ogg", RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 1, MIDDLEPAN, NULL );
			break;
		}
	}

	// Remove old animation profile
	this->HandleAnimationProfile( this->usAnimState, TRUE );


	// From animation control, set surface
	if ( SetSoldierAnimationSurface( this, usNewState ) == FALSE )
	{
		return(FALSE);
	}


	// Set state
	this->usOldAniState = this->usAnimState;
	this->sOldAniCode = this->usAniCode;

	// Change state value!
	this->usAnimState = usNewState;
	// Set current frame
	this->usAniCode = usStartingAniCode;

	// Handle cleanup stuff for getting hit.  Shouldn't this be part of the animation script?
	CheckForFreeupFromHit( this, uiOldAnimFlags, uiNewAnimFlags, this->usOldAniState, usNewState );

	// Perform attack busy stuff
	if ( this->usOldAniState != this->usAnimState )
	{
		if ( uiNewAnimFlags & ANIM_ATTACK ) {
			gTacticalStatus.ubAttackBusyCount++;
			DebugAttackBusy( String( "**** Attack animation transfer to %s for %d.\nABC now %d\n", gAnimControl[usNewState].zAnimStr, this->ubID, gTacticalStatus.ubAttackBusyCount ) );
		} else if (uiOldAnimFlags & ANIM_ATTACK || this->flags.fChangingStanceDueToSuppression ) {
			DebugAttackBusy( String( "**** Transfer to %s for %d.\n", gAnimControl[usNewState].zAnimStr, this->ubID ) );
		}
		if ( uiOldAnimFlags & ANIM_ATTACK ) {
			DebugAttackBusy( String( "**** Attack animation transfer from %s for %d.  Reducing ABC.\n", gAnimControl[this->usOldAniState].zAnimStr, this->ubID ) );
			ReduceAttackBusyCount( );
		} else if (uiNewAnimFlags & ANIM_ATTACK  || this->flags.fChangingStanceDueToSuppression ) {
			DebugAttackBusy( String( "**** Transfer from %s for %d\n", gAnimControl[this->usOldAniState].zAnimStr, this->ubID ) );
		}
	}

	if ( this->flags.fChangingStanceDueToSuppression )
	{
		this->flags.fChangingStanceDueToSuppression = FALSE;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Freeing up attacker - end of suppression stance change" ) );
		DebugAttackBusy( String( "@@@@@@@ Freeing up attacker - end of suppression stance change for %d\n", this->ubID ) );
		ReduceAttackBusyCount( );
	}

	this->sZLevelOverride = -1;

	if ( !(this->flags.uiStatusFlags & SOLDIER_LOCKPENDINGACTIONCOUNTER) )
	{
		//ATE Cancel ANY pending action...
		if ( this->aiData.ubPendingActionAnimCount > 0 && (gAnimControl[this->usOldAniState].uiFlags & ANIM_MOVING) )
		{
			// Do some special things for some actions
			switch ( this->aiData.ubPendingAction )
			{
			case MERC_GIVEITEM:

				// Unset target as enaged
				MercPtrs[this->aiData.uiPendingActionData4]->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION);
				break;
			}
			this->aiData.ubPendingAction = NO_PENDING_ACTION;
		}
		else
		{
			// Increment this for almost all animations except some movement ones...
			// That's because this represents ANY animation other than the one we began when the pending action was started
			// ATE: Added to ignore this count if we are waiting for someone to move out of our way...
			if ( usNewState != START_SWAT && usNewState != END_SWAT && !(gAnimControl[usNewState].uiFlags & ANIM_NOCHANGE_PENDINGCOUNT) && !this->flags.fDelayedMovement && !(this->flags.uiStatusFlags & SOLDIER_ENGAGEDINACTION) )
			{
				this->aiData.ubPendingActionAnimCount++;
			}
		}
	}

	// Set new animation profile
	this->HandleAnimationProfile( usNewState, FALSE );

	// Reset some animation values
	this->flags.fForceShade = FALSE;

	// ATE; For some animations that could use some variations, do so....
	if ( usNewState == CHARIOTS_OF_FIRE || usNewState == BODYEXPLODING )
	{
		this->usAniCode = (UINT16)(Random( 10 ));
	}

	// ATE: Default to first frame....
	// Will get changed ( probably ) by AdjustToNextAnimationFrame()
	this->ConvertAniCodeToAniFrame( (INT16)(0) );

	// Set delay speed
	SetSoldierAniSpeed( this );

	// Reset counters
	RESETTIMECOUNTER( this->timeCounters.UpdateCounter, this->sAniDelay );

	// Adjust to new animation frame ( the first one )
	AdjustToNextAnimationFrame( this );

	// Setup offset information for UI above guy
	SetSoldierLocatorOffsets( this );

	// Lesh: test fix visibility after raising gun
	if ( (gAnimControl[this->usOldAniState].uiFlags & ANIM_RAISE_WEAPON) && (gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY) )
		//equivalent if ( (this->usAnimState == AIM_RIFLE_PRONE) || (this->usAnimState == AIM_RIFLE_CROUCH) || (this->usAnimState == AIM_RIFLE_STAND) )
	{
		if ( (this->usOldAniState == READY_RIFLE_STAND) || (this->usOldAniState == READY_RIFLE_CROUCH) || (this->usOldAniState == READY_RIFLE_PRONE) ||
			 (this->usOldAniState == READY_DUAL_STAND) || (this->usOldAniState == READY_DUAL_CROUCH) || (this->usOldAniState == READY_DUAL_PRONE) )
		{
			HandleSight( this, SIGHT_LOOK );
		}
	}

	// Flugente: if we are covert and perform a suspicious action, we will be easier to uncover for a short time
	if ( this->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER) )
	{
		// if e perform a suspicious action, we are easier to identify 
		UINT16 appenalty = GetSuspiciousAnimationAPDuration( this->usAnimState );

		if ( appenalty )
		{
			// mark us a easily identifiable
			this->usSoldierFlagMask |= SOLDIER_COVERT_TEMPORARY_OVERT;

			// in realtime mode, remember the second when this event happened. Once suspicion is checked, we are either uncovered or, if enough time has passed, no longer suspicious
			// in turnbase mode, remember our current APs. If a new turn has started or enough APs have been used, remove the flag
			this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_SECONDS] = GetWorldTotalSeconds( ) + max( 1, appenalty / 25 );
			this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_APS] = appenalty;
		}
	}

	// If our own guy...
	if ( this->bTeam == gbPlayerNum )
	{
		// Are we stationary?
		if ( gAnimControl[usNewState].uiFlags & ANIM_STATIONARY )
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
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_STATIONARY) &&
			 (gAnimControl[this->usOldAniState].uiFlags & ANIM_MOVING) )
		{
			PlaySoldierFootstepSound( this );
		}
	}

	// Free up from stance change
	FreeUpNPCFromStanceChange( this );

	return(TRUE);
}


void SOLDIERTYPE::InternalRemoveSoldierFromGridNo( BOOLEAN fForce )
{
	INT8 bDir;
	INT32 iGridNo;

	if ( !TileIsOutOfBounds( this->sGridNo ) )
	{
		if ( this->bInSector || fForce )
		{
			// Remove from world ( old pos )
			RemoveMerc( this->sGridNo, this, FALSE );
			this->HandleAnimationProfile( this->usAnimState, TRUE );

			// Remove records of this guy being adjacent
			for ( bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++ )
			{
				iGridNo = this->sGridNo + DirIncrementer[bDir];
				if ( iGridNo >= 0 && iGridNo < WORLD_MAX )
				{
					gpWorldLevelData[iGridNo].ubAdjacentSoldierCnt--;
				}
			}

			HandlePlacingRoofMarker( this, this->sGridNo, FALSE, FALSE );

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


void SOLDIERTYPE::EVENT_InternalSetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest, BOOLEAN fForceRemove )
{
	INT32 sNewGridNo;

	// Not if we're dead!
	if ( (this->flags.uiStatusFlags & SOLDIER_DEAD) )
	{
		return;
	}

	// Set new map index
	sNewGridNo = GETWORLDINDEXFROMWORLDCOORDS( dNewYPos, dNewXPos );

	if ( fUpdateDest )
	{
		this->pathing.sDestination = sNewGridNo;
	}

	if ( fUpdateFinalDest )
	{
		this->pathing.sFinalDestination = sNewGridNo;
	}

	// HEADROCK HAM 4: TODO: Figure out whether this has any influence in-game?
	// Copy old values
	//this->sOldXPos = this->dXPos;
	//this->sOldYPos = this->dYPos;

	// Set New pos
	this->dXPos = dNewXPos;
	this->dYPos = dNewYPos;

	this->sX = (INT16)dNewXPos;
	this->sY = (INT16)dNewYPos;

	HandleCrowShadowNewPosition( this );

	this->SetSoldierGridNo( sNewGridNo, fForceRemove );

	if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
	{
		this->SetCheckSoldierLightFlag( );
	}

	// ATE: Mirror calls if we are a vehicle ( for all our passengers )
	UpdateAllVehiclePassengersGridNo( this );

}

void SOLDIERTYPE::EVENT_SetSoldierPosition( FLOAT dNewXPos, FLOAT dNewYPos )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos, TRUE, TRUE, FALSE );
}

void SOLDIERTYPE::EVENT_SetSoldierPositionForceDelete( FLOAT dNewXPos, FLOAT dNewYPos )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos, TRUE, TRUE, TRUE );
}

void SOLDIERTYPE::EVENT_SetSoldierPositionAndMaybeFinalDest( FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateFinalDest )
{
	this->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos, TRUE, fUpdateFinalDest, FALSE );
}

void EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest )
{
	pSoldier->EVENT_InternalSetSoldierPosition( dNewXPos, dNewYPos, fUpdateDest, fUpdateFinalDest, FALSE );
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

		ApplyTranslucencyToWalls( (INT16)(this->dXPos / CELL_X_SIZE), (INT16)(this->dYPos / CELL_Y_SIZE) );
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


void SOLDIERTYPE::SetSoldierGridNo( INT32 sNewGridNo, BOOLEAN fForceRemove )
{
	BOOLEAN	fInWaterValue;
	INT8		bDir;
	INT32		cnt;
	SOLDIERTYPE * pEnemy;

	//INT16	sX, sY, sWorldX, sZLevel;

	// Not if we're dead!
	if ( (this->flags.uiStatusFlags & SOLDIER_DEAD) )
	{
		return;
	}

	if ( sNewGridNo != this->sGridNo || this->pLevelNode == NULL )
	{
		// Check if we are moving AND this is our next dest gridno....
		if ( gAnimControl[this->usAnimState].uiFlags & (ANIM_MOVING | ANIM_SPECIALMOVE) )
		{
			if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
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

		if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
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
		if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
		{
			UnMarkMovementReserved( this );
		}

		if ( this->sInitialGridNo == 0 )
		{
			this->sInitialGridNo = sNewGridNo;
			this->aiData.sPatrolGrid[0] = sNewGridNo;
		}

		// Add records of this guy being adjacent
		for ( bDir = 0; bDir < NUM_WORLD_DIRECTIONS; bDir++ )
		{
			gpWorldLevelData[this->sGridNo + DirIncrementer[bDir]].ubAdjacentSoldierCnt++;
		}

		if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
		{
			DropSmell( this );
		}

		// HANDLE ANY SPECIAL RENDERING SITUATIONS
		this->sZLevelOverride = -1;
		// If we are over a fence ( hopping ), make us higher!

		if ( IsJumpableFencePresentAtGridNo( sNewGridNo ) )
		{
			//sX = MapX( sNewGridNo );
			//sY = MapY( sNewGridNo );
			//GetWorldXYAbsoluteScreenXY( sX, sY, &sWorldX, &sZLevel);
			//this->sZLevelOverride = (sZLevel*Z_SUBLAYERS)+ROOF_Z_LEVEL;
			this->sZLevelOverride = TOPMOST_Z_LEVEL;
		}
		/*
		if ( IsJumpableWindowPresentAtGridNo( sNewGridNo ) )
		{
		//sX = MapX( sNewGridNo );
		//sY = MapY( sNewGridNo );
		//GetWorldXYAbsoluteScreenXY( sX, sY, &sWorldX, &sZLevel);
		//this->sZLevelOverride = (sZLevel*Z_SUBLAYERS)+ROOF_Z_LEVEL;
		this->sZLevelOverride = TOPMOST_Z_LEVEL;
		}
		*/

		//ddd window{ ???????
		//if ( IsOknoFencePresentAtGridno( sNewGridNo ) )
		//{
		//	this->sZLevelOverride = TOPMOST_Z_LEVEL;
		//}
		//ddd window}

		// Add/ remove tree if we are near it
		// CheckForFullStructures( this );

		// Add merc at new pos
		if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
		{
			AddMercToHead( this->sGridNo, this, TRUE );

			// If we are in the middle of climbing the roof!
			if ( this->usAnimState == CLIMBUPROOF )
			{
				if ( this->iLight != (-1) )
					LightSpriteRoofStatus( this->iLight, TRUE );
			}
			else if ( this->usAnimState == CLIMBDOWNROOF )
			{
				if ( this->iLight != (-1) )
					LightSpriteRoofStatus( this->iLight, FALSE );
			}

			if ( this->usAnimState == JUMPUPWALL )
			{
				if ( this->iLight != (-1) )
					LightSpriteRoofStatus( this->iLight, TRUE );
			}
			else if ( this->usAnimState == JUMPDOWNWALL )
			{
				if ( this->iLight != (-1) )
					LightSpriteRoofStatus( this->iLight, FALSE );
			}

			//JA2Gold:
			//if the player wants the merc to cast the fake light AND it is night
			if ( this->bTeam != OUR_TEAM || gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] && NightTime( ) )
			{
				if ( this->pathing.bLevel > 0 && gpWorldLevelData[this->sGridNo].pRoofHead != NULL )
				{
					gpWorldLevelData[this->sGridNo].pMercHead->ubShadeLevel = gpWorldLevelData[this->sGridNo].pRoofHead->ubShadeLevel;
					gpWorldLevelData[this->sGridNo].pMercHead->ubSumLights = gpWorldLevelData[this->sGridNo].pRoofHead->ubSumLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubMaxLights = gpWorldLevelData[this->sGridNo].pRoofHead->ubMaxLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubNaturalShadeLevel = gpWorldLevelData[this->sGridNo].pRoofHead->ubNaturalShadeLevel;
				}
				else
				{
					gpWorldLevelData[this->sGridNo].pMercHead->ubShadeLevel = gpWorldLevelData[this->sGridNo].pLandHead->ubShadeLevel;
					gpWorldLevelData[this->sGridNo].pMercHead->ubSumLights = gpWorldLevelData[this->sGridNo].pLandHead->ubSumLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubMaxLights = gpWorldLevelData[this->sGridNo].pLandHead->ubMaxLights;
					gpWorldLevelData[this->sGridNo].pMercHead->ubNaturalShadeLevel = gpWorldLevelData[this->sGridNo].pLandHead->ubNaturalShadeLevel;
				}
			}

			this->HandleFlashLights( );

			///HandlePlacingRoofMarker( this, this->sGridNo, TRUE, FALSE );

			this->HandleAnimationProfile( this->usAnimState, FALSE );

			HandleCrowShadowNewGridNo( this );
		}

		this->bOldOverTerrainType = this->bOverTerrainType;
		this->bOverTerrainType = GetTerrainType( this->sGridNo );

		// OK, check that our animation is up to date!
		// Check our water value
		INT16 usUIMovementModeToSet = this->usUIMovementMode;
		if ( !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
		{
			fInWaterValue = this->MercInWater( );

			// ATE: If ever in water MAKE SURE WE WALK AFTERWOODS!
			if ( fInWaterValue )
			{
				usUIMovementModeToSet = WALKING;
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
					this->EVENT_InitNewSoldierAnim( usUIMovementModeToSet, 0, FALSE );
				}

			}

			// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
			// This should fix "problems" for special modified maps			
			if ( (TERRAIN_IS_WATER( this->bOverTerrainType ) || TERRAIN_IS_WATER( this->bOldOverTerrainType )) && this->pathing.bLevel > 0 )
			{
				this->bOverTerrainType = FLAT_GROUND;
				this->bOldOverTerrainType = FLAT_GROUND;
			}

			// OK, If we were not in deep water but we are now, handle deep animations!
			if ( TERRAIN_IS_DEEP_WATER( this->bOverTerrainType ) && !TERRAIN_IS_DEEP_WATER( this->bOldOverTerrainType ) )
			{
				// Based on our current animation, change!
				switch ( this->usAnimState )
				{
				case WALKING:
				case WALKING_WEAPON_RDY:
				case WALKING_DUAL_RDY:
				case WALKING_ALTERNATIVE_RDY:
				case RUNNING:
					// IN deep water, swim!
					// Make transition from low to deep
					this->EVENT_InitNewSoldierAnim( LOW_TO_DEEP_WATER, 0, FALSE );
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
			if ( !TERRAIN_IS_DEEP_WATER( this->bOverTerrainType ) && TERRAIN_IS_DEEP_WATER( this->bOldOverTerrainType ) )
			{
				// Make transition from low to deep
				this->EVENT_InitNewSoldierAnim( DEEP_TO_LOW_WATER, 0, FALSE );
				this->usDontUpdateNewGridNoOnMoveAnimChange = 1;
				this->usPendingAnimation = usUIMovementModeToSet;
			}
		}

		// are we now standing in tear gas without a decently working gas mask?
		if ( GetSmokeEffectOnTile( sNewGridNo, this->pathing.bLevel ) > 1 ) //lal: removed normal smoke
		{
			BOOLEAN fSetGassed = TRUE;

			// If we have a functioning gas mask...
			if ( DoesSoldierWearGasMask( this ) && this->inv[FindGasMask( this )][0]->data.objectStatus >= GASMASK_MIN_STATUS )//dnl ch40 200909
				fSetGassed = FALSE;
			if ( fSetGassed )
			{
				this->flags.uiStatusFlags |= SOLDIER_GASSED;
			}
		}

		// Flugente: award agility stat increase if we sneak upon an enemy undetected
		// do NOT award this bonus if we are currently loading a game - otherwise one could increase agility by repeatedly saving and reloading the game
		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
		{
			if ( this->bTeam == gbPlayerNum && this->bStealthMode )
			{
				// Merc got to a new tile by "sneaking". Did we theoretically sneak
				// past an enemy?

				if ( this->aiData.bOppCnt > 0 )		// opponents in sight
				{
					// check each possible enemy
					for ( cnt = 0; cnt < MAX_NUM_SOLDIERS; ++cnt )
					{
						pEnemy = MercPtrs[cnt];
						// if this guy is here and alive enough to be looking for us
						if ( pEnemy->bActive && pEnemy->bInSector && (pEnemy->stats.bLife >= OKLIFE) )
						{
							// no points for sneaking by the neutrals & friendlies!!!
							if ( !pEnemy->aiData.bNeutral && (this->bSide != pEnemy->bSide) && (pEnemy->ubBodyType != COW && pEnemy->ubBodyType != CROW) )
							{
								// if we SEE this particular oppponent, and he DOESN'T see us... and he COULD see us...
								if ( (this->aiData.bOppList[cnt] == SEEN_CURRENTLY) &&
									 pEnemy->aiData.bOppList[this->ubID] != SEEN_CURRENTLY &&
									 PythSpacesAway( this->sGridNo, pEnemy->sGridNo ) < pEnemy->GetMaxDistanceVisible( this->sGridNo, this->pathing.bLevel ) )
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
		}

		// Adjust speed based on terrain, etc
		SetSoldierAniSpeed( this );
	}
}


void SOLDIERTYPE::EVENT_FireSoldierWeapon( INT32 sTargetGridNo )
{
	INT16 sTargetXPos, sTargetYPos;
	BOOLEAN		fDoFireRightAway = FALSE;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "EVENT_FireSoldierWeapon" ) );

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


	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "EVENT_FireSoldierWeapon: Muzzle flash = %d", this->flags.fMuzzleFlash ) );

	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
	// Nah, just let the animations speak for themselves
	//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "!!!!!!! Starting attack, attack count now %d", gTacticalStatus.ubAttackBusyCount ) );
	DebugAttackBusy( String( "!!!!!!! Starting fire weapon attack, attack count now %d\n", gTacticalStatus.ubAttackBusyCount ) );
	//}

	// Set soldier's target gridno
	// This assignment was redundent because it's already set in
	// the actual event call
	this->sTargetGridNo = sTargetGridNo;
	//this->sLastTarget = sTargetGridNo;
	this->ubTargetID = WhoIsThere2( sTargetGridNo, this->bTargetLevel );

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "!!!!!!! Starting attack, bullets left %d", this->bBulletsLeft ) );

	// Convert our grid-not into an XY
	ConvertGridNoToXY( sTargetGridNo, &sTargetXPos, &sTargetYPos );

	//dnl ch72 2509134
	UINT16 usItem;
	if ( this->bWeaponMode == WM_ATTACHED_GL || this->bWeaponMode == WM_ATTACHED_GL_BURST || this->bWeaponMode == WM_ATTACHED_GL_AUTO )
		usItem = GetAttachedGrenadeLauncher( &this->inv[HANDPOS] );
	else
		usItem = this->inv[HANDPOS].usItem;
	if (ItemIsRocketLauncher(usItem) || ItemIsGrenadeLauncher(usItem) || ItemIsMortar(usItem) )
	{
		if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE || ItemIsMortar(usItem) && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND )
			SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
		fDoFireRightAway = TRUE;
	}

	// Change to fire animation
	// Ready weapon
	//if (this->bTeam == gbPlayerNum)
	//{
	this->SoldierReadyWeapon( sTargetXPos, sTargetYPos, FALSE, IsValidAlternativeFireMode( this->aiData.bAimTime, sTargetGridNo ) );
	//}
	//else
	//{
	//	this->SoldierReadyWeapon( sTargetXPos, sTargetYPos, FALSE, AIDecideHipOrShoulderStance( this, sTargetGridNo ) );
	//}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - hack! - an interrupt pending before shot
	if ( UsingImprovedInterruptSystem() )
	{
		if ( ResolvePendingInterrupt( this, BEFORESHOT_INTERRUPT ) )
		{
			this->usPendingAnimation = NO_PENDING_ANIMATION;
			this->ubPendingDirection = NO_PENDING_DIRECTION;
			// "artificially" set lock ui flag
			if ( this->bTeam == gbPlayerNum )
			{
				//AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
				guiPendingOverrideEvent = LU_BEGINUILOCK;
				HandleTacticalUI( );
			}
			// return as we are not gonna shoot if interrupted
			return;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// IF WE ARE AN NPC, SLIDE VIEW TO SHOW WHO IS SHOOTING
	{
		//if ( this->flags.fDoSpread )
		//{
		// If we are spreading burst, goto right away!
		//this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[ this->usAnimState ].ubEndHeight ), 0, FALSE );

		//}

		// else
		{
			if ( this->flags.uiStatusFlags & SOLDIER_MONSTER )
			{
				// Force our direction!
				this->EVENT_SetSoldierDirection( this->pathing.bDesiredDirection );
				this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[this->usAnimState].ubEndHeight ), 0, FALSE );
			}
			else
			{
				if ( fDoFireRightAway )
				{
					// Set to true so we don't get toasted twice for APs..
					this->flags.fDontUnsetLastTargetFromTurn = TRUE;

					// Make sure we don't try and do fancy prone turning.....
					this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;

					// Force our direction!
					this->EVENT_SetSoldierDirection( this->pathing.bDesiredDirection );

					this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[this->usAnimState].ubEndHeight ), 0, FALSE );
				}
				else
				{
					// Set flag indicating we are about to shoot once destination direction is hit
					this->flags.fTurningToShoot = TRUE;

					if (this->bTeam != gbPlayerNum)
					{
						if (this->bVisible != -1)
						{
							LocateSoldier(this->ubID, DONTSETLOCATOR);
						}
						else if (!TileIsOutOfBounds(sTargetGridNo) && !GridNoOnScreen(sTargetGridNo))
						{
							INT16 sNewCenterWorldX;
							INT16 sNewCenterWorldY;
							ConvertGridNoToCenterCellXY(sTargetGridNo, &sNewCenterWorldX, &sNewCenterWorldY);

							SetRenderCenter(sNewCenterWorldX, sNewCenterWorldY);

							// Plot new path!
							gfPlotNewMovement = TRUE;
						}
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
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "SelectFireAnimation" ) );


	//Do different things if we are a monster
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER )
	{
		switch ( pSoldier->ubBodyType )
		{
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:
		case YAF_MONSTER:
		case YAM_MONSTER:

			return(MONSTER_SPIT_ATTACK);
			break;

		case LARVAE_MONSTER:

			break;

		case INFANT_MONSTER:

			return(INFANT_ATTACK);
			break;

		case QUEENMONSTER:

			return(QUEEN_SPIT);
			break;

		}
		return(TRUE);
	}

	if ( pSoldier->ubBodyType == ROBOTNOWEAPON )
	{
		if ( pSoldier->bDoBurst > 0 )
		{
			return(ROBOT_BURST_SHOOT);
		}
		else
		{
			return(ROBOT_SHOOT);
		}
	}

	// Check for rocket laucncher....
	if (ItemIsRocketLauncher(pSoldier->inv[HANDPOS].usItem))
	{
		//***ddd if shoot crouched
		if ( ubHeight == ANIM_STAND )
			return(SHOOT_ROCKET);
		if ( ubHeight == ANIM_CROUCH )
			return(SHOOT_ROCKET_CROUCHED);
	}

	// Check for mortar....
	if (ItemIsMortar(pSoldier->inv[HANDPOS].usItem))
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "SelectFireAnimation: shoot_mortar" );
		return(SHOOT_MORTAR);
	}

	// Check for tank cannon
	if (ItemIsCannon(pSoldier->inv[HANDPOS].usItem))
	{
		return(TANK_SHOOT);
	}

	if ( ARMED_VEHICLE( pSoldier ) )
	{
		return(TANK_BURST);
	}

	// Determine which animation to do...depending on stance and gun in hand...
	switch ( ubHeight )
	{
	case ANIM_STAND:

		usItem = pSoldier->inv[HANDPOS].usItem;

		// CHECK 2ND HAND!
		if ( pSoldier->IsValidSecondHandBurst( ) )
		{
			return(BURST_DUAL_STAND);
		}
		else if ( pSoldier->IsValidSecondHandShot( ) && !pSoldier->bDoBurst )
		{
			return(SHOOT_DUAL_STAND);
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

			// Don't do any low shots if in water
			if ( pSoldier->MercInWater( ) )
			{
				fDoLowShot = FALSE;
			}


			if ( pSoldier->bDoBurst > 0 )
			{
				if ( fDoLowShot )
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, pSoldier->sTargetGridNo ) )
					{
						return(LOW_BURST_ALTERNATIVE_STAND);
					}
					else
					{
						return(FIRE_BURST_LOW_STAND);
					}
				}
				else
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, pSoldier->sTargetGridNo ) )
					{
						return(BURST_ALTERNATIVE_STAND);
					}
					else
					{
						return(STANDING_BURST);
					}
				}
			}
			else
			{
				if ( fDoLowShot )
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, pSoldier->sTargetGridNo ) )
					{
						return(LOW_SHOT_ALTERNATIVE_STAND);
					}
					else
					{
						return(FIRE_LOW_STAND);
					}
				}
				else
				{
					if ( pSoldier->IsValidAlternativeFireMode( pSoldier->aiData.bAimTime, pSoldier->sTargetGridNo ) )
					{
						return(SHOOT_ALTERNATIVE_STAND);
					}
					else
					{
						return(SHOOT_RIFLE_STAND);
					}
				}
			}
		}
		break;

	case ANIM_CROUCH:

		if ( pSoldier->IsValidSecondHandShot( ) && pSoldier->bDoBurst > 0 && pSoldier->IsValidSecondHandBurst( ) )
		{
			return(BURST_DUAL_CROUCH);
		}
		else if ( pSoldier->IsValidSecondHandShot( ) && !pSoldier->bDoBurst )
		{
			return(SHOOT_DUAL_CROUCH);
		}
		else
		{
			if ( pSoldier->bDoBurst > 0 )
			{
				return(CROUCHED_BURST);
			}
			else
			{
				return(SHOOT_RIFLE_CROUCH);
			}
		}
		break;

	case ANIM_PRONE:

		if ( pSoldier->IsValidSecondHandShot( ) && pSoldier->bDoBurst > 0 && pSoldier->IsValidSecondHandBurst( ) )
		{
			return(BURST_DUAL_PRONE);
		}
		else if ( pSoldier->IsValidSecondHandShot( ) && !pSoldier->bDoBurst )
		{
			return(SHOOT_DUAL_PRONE);
		}
		else
		{
			if ( pSoldier->bDoBurst > 0 )
			{
				return(PRONE_BURST);
			}
			else
			{
				return(SHOOT_RIFLE_PRONE);
			}
		}
		break;

	default:
		AssertMsg( FALSE, String( "SelectFireAnimation: ERROR - Invalid height %d", ubHeight ) );
		break;
	}


	// If here, an internal error has occured!
	Assert( FALSE );
	return (0);
}


UINT16 SOLDIERTYPE::GetMoveStateBasedOnStance( UINT8 ubStanceHeight )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( ubStanceHeight )
	{
	case ANIM_STAND:
		//if ( this->flags.fUIMovementFast && !( this->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		if ( this->IsFastMovement() )
		{
			return(RUNNING);
		}
		else
		{
			return(WALKING);
		}
		break;

	case ANIM_PRONE:
		if ( this->IsFastMovement() )
		{
			return(CRAWLING);
		}
		else
		{
			return(CRAWLING);
		}
		break;

	case ANIM_CROUCH:
		if ( this->IsFastMovement() )
		{
			return(SWATTING);
		}
		else
		{
			//***ddd
			// only 1 bodytime is ready (drawn) currently, the rest need to be added
			UINT16 usItem = this->inv[HANDPOS].usItem;
			if ( this->inv[HANDPOS].exists( ) == true &&
				 //(this->ubBodyType == BIGMALE || this->ubBodyType == REGFEMALE )&&
				 (Item[usItem].usItemClass == IC_BLADE || Item[usItem].usItemClass == IC_THROWING_KNIFE) )
				 return(SWATTING_WK);
			else
				return(SWATTING);

		}
		break;


	default:
		AssertMsg( FALSE, String( "GetMoveStateBasedOnStance: ERROR - Invalid height %d", ubStanceHeight ) );
		break;
	}


	// If here, an internal error has occured!
	Assert( FALSE );
	return (0);
}


void SelectFallAnimation( SOLDIERTYPE *pSoldier )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:
		pSoldier->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0, FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0, FALSE );
		break;
	}

}

BOOLEAN SOLDIERTYPE::SoldierReadyWeapon( void )
{
	return(this->InternalSoldierReadyWeapon( (INT8)this->ubDirection, FALSE, FALSE ));
}

BOOLEAN SOLDIERTYPE::SoldierReadyWeapon( INT16 sTargetXPos, INT16 sTargetYPos, BOOLEAN fEndReady, BOOLEAN fRaiseToHipOnly )
{
	INT16								sFacingDir;

	sFacingDir = GetDirectionFromXY( sTargetXPos, sTargetYPos, this );

	return(this->InternalSoldierReadyWeapon( (INT8)sFacingDir, fEndReady, fRaiseToHipOnly ));
}


BOOLEAN SOLDIERTYPE::InternalSoldierReadyWeapon( UINT8 sFacingDir, BOOLEAN fEndReady, BOOLEAN fRaiseToHipOnly )
{
	UINT16 usAnimState;
	BOOLEAN	fReturnVal = FALSE;

	// Handle monsters differently
	if ( this->flags.uiStatusFlags & SOLDIER_MONSTER )
	{
		if ( !fEndReady )
		{
			this->EVENT_SetSoldierDesiredDirection( sFacingDir );
		}
		return(FALSE);
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "InternalSoldierReadyWeapon: PickingAnimation" ) );
	usAnimState = PickSoldierReadyAnimation( this, fEndReady, fRaiseToHipOnly );
	if ( !fEndReady && this->ubDirection != sFacingDir )//dnl ch71 170913
	{
		UINT16 usTrueAnimState = this->usAnimState;
		switch ( gAnimControl[this->usAnimState].ubEndHeight )
		{
		case ANIM_STAND:
			this->usAnimState = STANDING;
			if ( this->flags.fDontChargeTurningAPs && !this->flags.fDontChargeReadyAPs && usAnimState == INVALID_ANIMATION )
				usAnimState = PickSoldierReadyAnimation( this, FALSE, fRaiseToHipOnly );// someone introduced new cost system for turning and this means we must force gun raise
			break;
		case ANIM_CROUCH:
			this->usAnimState = CROUCHING;
			if ( this->flags.fDontChargeTurningAPs && !this->flags.fDontChargeReadyAPs && usAnimState == INVALID_ANIMATION )
				usAnimState = PickSoldierReadyAnimation( this, FALSE, FALSE );// force gun raise for proper APs deduction
			break;
		case ANIM_PRONE:
			usAnimState = INVALID_ANIMATION;// we are turning in prone so raise gun will be done after turning
			break;
		}
		this->usAnimState = usTrueAnimState;
	}
	if ( usAnimState != INVALID_ANIMATION )
	{
		if ( is_networked )
		{
			ChangeSoldierState( usAnimState, 0, FALSE );//this passes it to an area where it gets sent over the network.
		}
		else
		{
			this->EVENT_InitNewSoldierAnim( usAnimState, 0, FALSE );
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
		//dnl ch72 270913 ugly but fast fix for not charging turning APs as there is no fire ready animation for mortars and rocket launchers
		UINT16 usItem = this->inv[HANDPOS].usItem;
		if (ItemIsRocketLauncher(usItem) || ItemIsMortar(usItem) )
			usForceAnimState = this->usAnimState;
		EVENT_InternalSetSoldierDesiredDirection( this, sFacingDir, FALSE, usAnimState );
		usForceAnimState = INVALID_ANIMATION;
		// Check if facing dir is different from ours and change direction if so!
		//if ( sFacingDir != this->ubDirection )
		//{
		//	DeductPoints( this, APBPConstants[AP_CHANGE_FACING], 0 );
		//}//
	}

	// weapon raising sound
	if (fReturnVal &&
		!fEndReady &&
		this->bVisible >= 0 &&
		!WeaponReady(this) &&
		this->inv[HANDPOS].exists() &&
		this->inv[HANDPOS].usItem &&
		Item[this->inv[HANDPOS].usItem].usItemClass & IC_GUN)
	{
		CHAR8	zFilename[512];
		sprintf(zFilename, "");

		switch (Weapon[Item[this->inv[HANDPOS].usItem].ubClassIndex].ubWeaponType)
		{
		case GUN_PISTOL:
			sprintf(zFilename, "sounds\\equip\\Draw_Pistol.ogg");
			break;
		case GUN_M_PISTOL:
			sprintf(zFilename, "sounds\\equip\\Draw_MP.ogg");
			break;
		case GUN_SMG:
			sprintf(zFilename, "sounds\\equip\\Draw_SMG.ogg");
			break;
		case GUN_RIFLE:
			sprintf(zFilename, "sounds\\equip\\Draw_Rifle.ogg");
			break;
		case GUN_SN_RIFLE:
			sprintf(zFilename, "sounds\\equip\\Draw_Sniper.ogg");
			break;
		case GUN_AS_RIFLE:
			sprintf(zFilename, "sounds\\equip\\Draw_AR.ogg");
			break;
		case GUN_LMG:
			sprintf(zFilename, "sounds\\equip\\Draw_LMG.ogg");
			break;
		case GUN_SHOTGUN:
			sprintf(zFilename, "sounds\\equip\\Draw_Shotgun.ogg");
			break;
		default:
			sprintf(zFilename, "sounds\\equip\\Draw.ogg");
		}

		if (!FileExists(zFilename))
		{
			sprintf(zFilename, "sounds\\equip\\Draw.ogg");
		}

		if (FileExists(zFilename))
		{
			PlayJA2SampleFromFile(zFilename, RATE_11025, SoundVolume(HIGHVOLUME, this->sGridNo), 1, SoundDir(this->sGridNo));
		}
	}

	return(fReturnVal);
}

UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady, BOOLEAN fAltWeaponHolding )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "PickSoldierReadyAnimation" ) );

	// Invalid animation if nothing in our hands
	if ( pSoldier->inv[HANDPOS].exists( ) == false )
	{
		return(INVALID_ANIMATION);
	}

	if ( pSoldier->MercInDeepWater( ) )
	{
		return(INVALID_ANIMATION);
	}

	if ( pSoldier->ubBodyType == ROBOTNOWEAPON )
	{
		return(INVALID_ANIMATION);
	}

	// Check if we have a gun.....
	if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass != IC_GUN && !ItemIsGrenadeLauncher(pSoldier->inv[HANDPOS].usItem) )
	{
		return(INVALID_ANIMATION);
	}

	if (ItemIsRocketLauncher(pSoldier->inv[HANDPOS].usItem))
	{
		return(INVALID_ANIMATION);
	}

	if ( ARMED_VEHICLE( pSoldier ) )
	{
		return(INVALID_ANIMATION);
	}

	if ( fEndReady )
	{
		// IF our gun is already drawn, do not change animation, just direction
		if ( gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE) )
		{

			switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
			{
			case ANIM_STAND:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(END_DUAL_STAND);
				}
				else
				{
					if ( gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING) )//&& Item[ pSoldier->inv[HANDPOS].usItem ].twohanded)
					{
						return(UNREADY_ALTERNATIVE_STAND);
					}
					//else if (gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_ALT_WEAPON_HOLDING ) && !Item[ pSoldier->inv[HANDPOS].usItem ].twohanded)
					//{
					//	return( PISTOL_FASTSHOT_UNREADY );
					//}
					else
					{
						return(END_RIFLE_STAND);
					}
				}
				break;

			case ANIM_PRONE:

				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(END_DUAL_PRONE);
				}
				else
				{
					return(END_RIFLE_PRONE);
				}
				break;

			case ANIM_CROUCH:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(END_DUAL_CROUCH);
				}
				else
				{
					return(END_RIFLE_CROUCH);
				}
				break;

			}

		}
	}
	else
	{
		// if our gun is in alternative holding (hip rifle/one-hand pistol) and we are going to shoulder
		if ( (gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING)) && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND && !fAltWeaponHolding && !Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].HeavyGun )
		{
			return(READY_RIFLE_STAND);
		}
		// this is a specific situation when we have a gun in standard holding (shouldered rifle/two-hand pistol) and was told to go to alternative holding
		else if ( (gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) && !(gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING))
				  && fAltWeaponHolding && gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 && pSoldier->bScopeMode == -1 && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND
				  && ((!ItemIsTwoHanded(pSoldier->inv[HANDPOS].usItem) && !pSoldier->IsValidSecondHandShot( ) && !pSoldier->MercInWater( )) || ItemIsTwoHanded(pSoldier->inv[HANDPOS].usItem)) )
		{
			return(READY_ALTERNATIVE_STAND);
		}
		// IF our gun is already drawn, do not change animation, just direction
		else if ( !(gAnimControl[pSoldier->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) )
		{
			switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
			{
			case ANIM_STAND:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(READY_DUAL_STAND);
				}
				else
				{
					if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding )
					{
						if ( fAltWeaponHolding || (Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].HeavyGun && ItemIsTwoHanded(pSoldier->inv[HANDPOS].usItem)) )
						{
							return(READY_ALTERNATIVE_STAND);
						}
						else
						{
							return(READY_RIFLE_STAND);
						}
					}
					else
					{
						return(READY_RIFLE_STAND);
					}
				}
				break;

			case ANIM_PRONE:
				// Go into crouch, turn, then go into prone again
				//pSoldier->ChangeSoldierStance( ANIM_CROUCH );
				//pSoldier->ubDesiredHeight = ANIM_PRONE;
				//pSoldier->ChangeSoldierState( PRONE_UP );
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(READY_DUAL_PRONE);
				}
				else
				{
					return(READY_RIFLE_PRONE);
				}
				break;

			case ANIM_CROUCH:

				// CHECK 2ND HAND!
				if ( pSoldier->IsValidSecondHandShot( ) )
				{
					return(READY_DUAL_CROUCH);
				}
				else
				{
					return(READY_RIFLE_CROUCH);
				}
				break;

			}

		}
	}

	return(INVALID_ANIMATION);
}

// 0verhaul:  These routines are obsolete.  Just call ReduceAttackBusyCount to reduce the ABC or
// FreeUpAttacker to abort the current action.
// extern SOLDIERTYPE * FreeUpAttackerGivenTarget( UINT8 ubID, UINT8 ubTargetID );
// extern SOLDIERTYPE * ReduceAttackBusyGivenTarget( UINT8 ubID, UINT8 ubTargetID );

#pragma optimize("", off)
// ATE: THIS FUNCTION IS USED FOR ALL SOLDIER TAKE DAMAGE FUNCTIONS!
void SOLDIERTYPE::EVENT_SoldierGotHit( UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 bDirection, UINT16 sRange, UINT16 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation, INT16 sSubsequent, INT32 sLocationGrid )
{
	UINT8		ubCombinedLoss, ubVolume, ubReason;
	//	SOLDIERTYPE * pNewSoldier;

	if (gTacticalStatus.uiFlags & GODMODE && this->bTeam == OUR_TEAM)
	{
		sDamage = 0;
		ubSpecial = FIRE_WEAPON_NO_SPECIAL;
	}

	ubReason = 0;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "EVENT_SoldierGotHit" );


	// DO STUFF COMMON FOR ALL TYPES
	if ( ubAttackerID != NOBODY )
	{
		MercPtrs[ubAttackerID]->aiData.bLastAttackHit = TRUE;
	}

	// Set attacker's ID
	this->ubAttackerID = ubAttackerID;


	// ATE; Save hit location info...( for later anim determination stuff )
	this->ubHitLocation = ubHitLocation;

	// handle morale for heavy damage attacks
	if ( sDamage > 25 )
	{
		if ( this->ubAttackerID != NOBODY && MercPtrs[this->ubAttackerID]->bTeam == gbPlayerNum )
		{
			HandleMoraleEvent( MercPtrs[this->ubAttackerID], MORALE_DID_LOTS_OF_DAMAGE, MercPtrs[this->ubAttackerID]->sSectorX, MercPtrs[this->ubAttackerID]->sSectorY, MercPtrs[this->ubAttackerID]->bSectorZ );
			this->ubLastMoraleFromHit++;
		}
		if ( this->bTeam == gbPlayerNum )
		{
			HandleMoraleEvent( this, MORALE_TOOK_LOTS_OF_DAMAGE, this->sSectorX, this->sSectorY, this->bSectorZ );
			this->ubLastMoraleFromHit++;
		}
	}

	// SWITCH IN TYPE OF WEAPON
	if ( ubSpecial == FIRE_WEAPON_TOSSED_OBJECT_SPECIAL )
	{
		ubReason = TAKE_DAMAGE_OBJECT;
	}
	else if ( Item[usWeaponIndex].usItemClass & IC_TENTACLES )
	{
		ubReason = TAKE_DAMAGE_TENTACLES;
	}
	else if ( ubSpecial == FIRE_WEAPON_VEHICLE_TRAUMA )
	{
		ubReason = TAKE_DAMAGE_VEHICLE_TRAUMA;
	}
	// Flugente: check the ammo
	else if ( ubAttackerID != NOBODY
		&& MercPtrs[ubAttackerID]->inv[HANDPOS].exists()
		&& AmmoTypes[*&( MercPtrs[ubAttackerID]->inv[HANDPOS] )[0]->data.gun.ubGunAmmoType].ammoflag & AMMO_TRAIL_FIRE )
	{
		ubReason = TAKE_DAMAGE_GAS_FIRE;

		INT16 fireresistance = ArmourVersusFirePercent( this );

		sDamage = max( 0, sDamage  * ( 100 - fireresistance ) / 100 );
	}

	// marke take out gunfire if ammotype is explosive

	// callahan update start
	// setting new func to intercept testhit
	else if ( Item[usWeaponIndex].usItemClass & (IC_GUN | IC_THROWING_KNIFE) && ubAttackerID == NOBODY )
	{
		sBreathLoss += APBPConstants[BP_GET_HIT];
		ubReason = TAKE_DAMAGE_GUNFIRE;
	}
	// callahan update end

	else if ( Item[usWeaponIndex].usItemClass & (IC_GUN | IC_THROWING_KNIFE) && AmmoTypes[MercPtrs[ubAttackerID]->inv[MercPtrs[ubAttackerID]->ubAttackingHand][0]->data.gun.ubGunAmmoType].explosionSize <= 1 )
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
				sBreathLoss = (INT16)(this->bBreathMax * 100);
			}

		}
		else if ( ubSpecial == FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL )
		{
			// blinded!!
			if ( (this->bBlindedCounter == 0) )
			{
				// say quote
				if ( this->flags.uiStatusFlags & SOLDIER_PC )
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
		// Flugente: like FIRE_WEAPON_BLINDED_BY_SPIT_SPECIAL but without the damage dependency
		else if ( ubSpecial == FIRE_WEAPON_BLINDED_SPECIAL )
		{
			// blinded!!
			if ( (this->bBlindedCounter == 0) )
			{
				// say quote
				if ( this->flags.uiStatusFlags & SOLDIER_PC )
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
				DecayIndividualOpplist( this );
			}

			this->bBlindedCounter += 2 * Random( 3 ) + 2;

			// Dirty panel
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
		sBreathLoss += APBPConstants[BP_GET_HIT];
		ubReason = TAKE_DAMAGE_GUNFIRE;
	}
	else if ( Item[usWeaponIndex].usItemClass & IC_BLADE )
	{
		// SANDRO - slightly reduce breath damage of melee weapons, it is an issue for martial arts
		if ( gGameOptions.fNewTraitSystem )
			sBreathLoss = (APBPConstants[BP_GET_HIT] * (100 + gSkillTraitValues.bPercentModifierBladesBreathLoss) / 100);
		else
			sBreathLoss = APBPConstants[BP_GET_HIT];
		ubReason = TAKE_DAMAGE_BLADE;

		// Flugente: check wether we can make this blade bloody
		if ( ubAttackerID != NOBODY && MercPtrs[ubAttackerID]->inv[HANDPOS].exists( ) )
		{
			if ( Item[MercPtrs[ubAttackerID]->inv[HANDPOS].usItem].bloodieditem > 0 )
			{
				// magic happens
				MercPtrs[ubAttackerID]->inv[HANDPOS].usItem = Item[MercPtrs[ubAttackerID]->inv[HANDPOS].usItem].bloodieditem;
			}

			// Flugente: if the blade is infected, infect the victim
			if ( *&(MercPtrs[ubAttackerID]->inv[HANDPOS])[0]->data.sObjectFlag & INFECTED && gGameExternalOptions.fDiseaseContaminatesItems )
			{
				// infect us with the first disease
				this->Infect( 0 );
			}

			// if this guy has the disease, infect the blade
			if ( this->sDiseasePoints[0] > 0 )
				*&(MercPtrs[ubAttackerID]->inv[HANDPOS])[0]->data.sObjectFlag |= INFECTED;
		}
	}
	else if ( Item[usWeaponIndex].usItemClass & IC_PUNCH )
	{
		////////////////////////////////////////////////////////////////////////////
		// SANDRO - STOMP traits
		UINT16 sBreathRegainPenaltyMultiplier = 0;
		if ( gGameOptions.fNewTraitSystem )
		{
			if ( ubAttackerID != NOBODY )
			{
				if ( !(MercPtrs[ubAttackerID]->inv[HANDPOS].exists( )) || ItemIsBrassKnuckles(MercPtrs[ubAttackerID]->inv[HANDPOS].usItem) )
				{
					// with enhanced CCS, make the lost breath harder to regenerate, which makes CQC more usable
					if ( gGameExternalOptions.fEnhancedCloseCombatSystem )
						sBreathRegainPenaltyMultiplier = 10;

					sBreathLoss = sDamage * (100 + gSkillTraitValues.bPercentModifierHtHBreathLoss); // 80% only for untrained mercs

					// martial arts bonus for breath damage
					if ( HAS_SKILL_TRAIT( MercPtrs[ubAttackerID], MARTIAL_ARTS_NT ) )
					{
						sBreathLoss += sDamage * gSkillTraitValues.ubMABonusBreathDamageHandToHand * NUM_SKILL_TRAITS( MercPtrs[ubAttackerID], MARTIAL_ARTS_NT );

						sBreathRegainPenaltyMultiplier += (gSkillTraitValues.usMALostBreathRegainPenalty * NUM_SKILL_TRAITS( MercPtrs[ubAttackerID], MARTIAL_ARTS_NT ));
					}
				}
				else
				{
					// with enhanced CCS, make the lost breath harder to regenerate, which makes CQC more usable
					if ( gGameExternalOptions.fEnhancedCloseCombatSystem )
						sBreathRegainPenaltyMultiplier = 15;

					sBreathLoss = sDamage * (100 + gSkillTraitValues.bPercentModifierBluntBreathLoss); // 50% only for melee weapons
				}
			}
			else
			{
				sBreathLoss = sDamage * (100 + gSkillTraitValues.bPercentModifierHtHBreathLoss);
			}
			// bodybuilding reduces this to half
			if ( HAS_SKILL_TRAIT( this, BODYBUILDING_NT ) )
			{
				sBreathLoss = max( 10, (sBreathLoss * (100 - gSkillTraitValues.ubBBBreathLossForHtHImpactReduction) / 100) );
			}
		}
		else
		{
			// with enhanced CCS, make the lost breath harder to regenerate, which makes CQC more usable
			if ( gGameExternalOptions.fEnhancedCloseCombatSystem )
				sBreathRegainPenaltyMultiplier = 10;
			// damage from hand-to-hand is 1/4 normal, 3/4 breath.. the sDamage value
			// is actually how much breath we'll take away
			sBreathLoss = sDamage * 100;
		}
		if ( sBreathRegainPenaltyMultiplier > 0 )
		{
			// unregainable breath damage
			this->lUnregainableBreath += ((sBreathLoss * sBreathRegainPenaltyMultiplier) / 100);
		}
		////////////////////////////////////////////////////////////////////////////
		sDamage = sDamage / PUNCH_REAL_DAMAGE_PORTION;
#ifdef JA2UB
		//Ja25: No meanwhiles
#else
		if ( AreInMeanwhile( ) && gCurrentMeanwhileDef.ubMeanwhileID == INTERROGATION )
		{
			sBreathLoss = 0;
			sDamage /= 4;
			sDamage = max(1, sDamage);
		}
#endif
		ubReason = TAKE_DAMAGE_HANDTOHAND;

		// Flugente: if the weapon is a taser and has enough batteries, the damage will be 0, but the breathdamage will knock out anyone
		if ( HasItemFlag( usWeaponIndex, TASER ) )
		{
			// tasers need batteries, because I say so
			if (ItemNeedsBatteries(usWeaponIndex))
			{
				// check for batteries
				OBJECTTYPE* pBatteries = FindAttachedBatteries( &(MercPtrs[ubAttackerID]->inv[HANDPOS]) );
				if ( pBatteries )
				{
					sDamage = 0;
					sBreathLoss = 30000;
					ubReason = TAKE_DAMAGE_ELECTRICITY;

					// use up 8-12 percent of batteries
					if ( Item[pBatteries->usItem].percentstatusdrainreduction > 0 )
						(*pBatteries)[0]->data.objectStatus -= (INT8)((8 + Random( 5 )) * (100 - Item[(*pBatteries)[0]->data.objectStatus].percentstatusdrainreduction) / 100);
					else
						(*pBatteries)[0]->data.objectStatus -= (INT8)((8 + Random( 5 )));
					if ( (*pBatteries)[0]->data.objectStatus <= 0 )
					{
						// destroy batteries
						pBatteries->RemoveObjectsFromStack( 1 );
						if ( pBatteries->exists( ) == false ) {
							MercPtrs[ubAttackerID]->inv[HANDPOS].RemoveAttachment( pBatteries );
						}
					}

					// insert electrical sound effect here
					PlayJA2Sample( DOOR_ELECTRICITY, RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
				}
			}
		}
	}
	// marke added one 'or' for explosive ammo. variation of: AmmoTypes[this->inv[this->ubAttackingHand ][0]->data.gun.ubGunAmmoType].explosionSize > 1
	//  extracting attacker�s ammo type
	else if ( Item[usWeaponIndex].usItemClass & IC_EXPLOSV || AmmoTypes[MercPtrs[ubAttackerID]->inv[MercPtrs[ubAttackerID]->ubAttackingHand][0]->data.gun.ubGunAmmoType].explosionSize > 1 )
	{
		INT8 bDeafValue;

		bDeafValue = Explosive[Item[usWeaponIndex].ubClassIndex].ubVolume / 10;
		if ( bDeafValue == 0 )
			bDeafValue = 1;

		// Lesh: flashbang does damage
		switch ( ubSpecial )
		{
		case FIRE_WEAPON_BLINDED_AND_DEAFENED:
			this->bDeafenedCounter = bDeafValue;
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Soldier is blinded and deafened" );

			// if soldier in building OR underground
			if ( InBuilding( sLocationGrid ) || (gbWorldSectorZ) )
			{
				// deal max special damage
				this->bBlindedCounter = (INT8)Explosive[Item[usWeaponIndex].ubClassIndex].ubDuration;
				// say quote
				if ( this->flags.uiStatusFlags & SOLDIER_PC )
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
			}
			else if ( NightTime( ) ) // if soldier outside at night
			{
				// halve effect
				this->bBlindedCounter = (INT8)Explosive[Item[usWeaponIndex].ubClassIndex].ubDuration / 2;
				if ( this->bBlindedCounter == 0 )
					this->bBlindedCounter = 1;
				this->bDeafenedCounter /= 2;
				// say quote
				if ( this->flags.uiStatusFlags & SOLDIER_PC )
				{
					TacticalCharacterDialogue( this, QUOTE_BLINDED );
				}
			}
			DecayIndividualOpplist( this );
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_BLINDED, ubAttackerID );
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_DEAFENED, ubAttackerID );
			break;

		case FIRE_WEAPON_BLINDED:
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_BLINDED, ubAttackerID );
			break;

		case FIRE_WEAPON_DEAFENED:
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Soldier is deafened" );		
			this->bDeafenedCounter = bDeafValue;
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_DEAFENED, ubAttackerID );
			break;
		};

		if ( usWeaponIndex == STRUCTURE_EXPLOSION )
		{
			ubReason = TAKE_DAMAGE_STRUCTURE_EXPLOSION;
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_STRUCTURE_EXPLOSION, ubAttackerID );
		}
		else
		{
			ubReason = TAKE_DAMAGE_EXPLOSION;
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_EXPLOSION, ubAttackerID );
		}
	}
	else
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Control: Weapon class not handled in SoldierGotHit( ) %d", usWeaponIndex ) );
	}



	// Flugente: moved the damage calculation into a separate function
	sBreathLoss = max( 1, (INT16)(sBreathLoss * (100 - this->GetDamageResistance( FALSE, TRUE )) / 100) );

	// OK, If we are a vehicle.... damage vehicle...( people inside... )
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		this->SoldierTakeDamage( ANIM_CROUCH, sDamage, sBreathLoss, ubReason, this->ubAttackerID, NOWHERE, FALSE, TRUE );
		return;
	}

	// sevenfm: player mercs should not die instantly
	if (gGameExternalOptions.fReducedInstantDeath &&
		this->flags.uiStatusFlags & SOLDIER_PC &&
		this->stats.bLife >= OKLIFE &&
		ubSpecial != FIRE_WEAPON_HEAD_EXPLODE_SPECIAL &&
		ubSpecial != FIRE_WEAPON_CHEST_EXPLODE_SPECIAL &&
		Item[usWeaponIndex].usItemClass & (IC_GUN | IC_THROWING_KNIFE | IC_BLADE))
	{
		sDamage = __min(sDamage, this->stats.bLife - OKLIFE + 1 + sDamage / 10);
	}

	// DEDUCT LIFE
	ubCombinedLoss = this->SoldierTakeDamage( ANIM_CROUCH, sDamage, sBreathLoss, ubReason, this->ubAttackerID, NOWHERE, FALSE, TRUE );

	// ATE: OK, Let's check our ASSIGNMENT state,
	// If anything other than on a squad or guard, make them guard....
	if ( this->bTeam == gbPlayerNum )
	{
		if ( this->bAssignment >= ON_DUTY && this->bAssignment != ASSIGNMENT_POW && this->bAssignment != ASSIGNMENT_MINIEVENT && this->bAssignment != ASSIGNMENT_REBELCOMMAND)
		{
			if ( this->flags.fMercAsleep )
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
	if ( !(gAnimControl[this->usAnimState].uiFlags & ANIM_HITSTOP) || this->usAnimState != JFK_HITDEATH_STOP )
	{
		MakeNoise( this->ubID, this->sGridNo, this->pathing.bLevel, this->bOverTerrainType, ubVolume, NOISE_SCREAM );
	}

	// IAN ADDED THIS SAT JUNE 14th : HAVE TO SHOW VICTIM!
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) && this->bVisible != -1 && this->bTeam == gbPlayerNum )
		LocateSoldier( this->ubID, DONTSETLOCATOR );


	if ( Item[usWeaponIndex].usItemClass & IC_BLADE )
	{
		PlayJA2Sample( (UINT32)(KNIFE_IMPACT), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
	}
	else
	{
		PlayJA2Sample( (UINT32)(BULLET_IMPACT_1 + Random( 3 )), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
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
				this->DoMercBattleSound( BATTLE_SOUND_HIT1 );
			}
		}
	}

	// anv: soldier got rammed by vehicle
	if ( ubSpecial == FIRE_WEAPON_VEHICLE_TRAUMA )
	{
		SoldierGotHitVehicle( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
		return;
	}

	// CHECK FOR DOING HIT WHILE DOWN
	if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_HITSTOP) )
	{
		switch ( this->usAnimState )
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
			this->ChangeSoldierState( PRONE_HIT_DEATHTWITCHB, 0, FALSE );
			break;

		case FALLFORWARD_HITDEATH_STOP:
			this->ChangeSoldierState( GENERIC_HIT_DEATHTWITCHB, 0, FALSE );
			break;

		case FALLBACK_HITDEATH_STOP:
			this->ChangeSoldierState( FALLBACK_HIT_DEATHTWITCHB, 0, FALSE );
			break;

		case FALLOFF_DEATH_STOP:
			this->ChangeSoldierState( FALLOFF_TWITCHB, 0, FALSE );
			break;

		case FALLOFF_STOP:
			this->ChangeSoldierState( FALLOFF_TWITCHNB, 0, FALSE );
			break;

		case FALLOFF_FORWARD_DEATH_STOP:
			this->ChangeSoldierState( FALLOFF_FORWARD_TWITCHB, 0, FALSE );
			break;

		case FALLOFF_FORWARD_STOP:
			this->ChangeSoldierState( FALLOFF_FORWARD_TWITCHNB, 0, FALSE );
			break;

		default:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Control: Death state %d has no death hit", this->usAnimState ) );

		}
		return;
	}

	// Set goback to aim after hit flag!
	// SANDRO - added more cases, alternative weapon holding, go back to cowering, and go back to hth/blade stance 
	// If we were in hth or blade stance, and we were hit by HtH or blade attack, go back to the fighting stance (if we can still keep up)
	if ( (Item[usWeaponIndex].usItemClass & (IC_BLADE | IC_PUNCH)) && Item[this->inv[HANDPOS].usItem].usItemClass & (IC_NONE | IC_BLADE | IC_PUNCH) &&
		 (this->usAnimState == PUNCH_BREATH || this->usAnimState == KNIFE_BREATH || this->usAnimState == NINJA_BREATH) && (gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND) )
	{
		if ( this->stats.bLife > 30 && this->bBreath > 25 )
		{
			this->flags.bGoBackToAimAfterHit = GO_TO_HTH_BREATH_AFTER_HIT;
		}
	}
	// If we were aiming
	else if ( gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY )
	{
		if ( gAnimControl[this->usAnimState].uiFlags & ANIM_ALT_WEAPON_HOLDING ) // alternative weapon holding stance
			this->flags.bGoBackToAimAfterHit = GO_TO_ALTERNATIVE_AIM_AFTER_HIT;
		else // standard
			this->flags.bGoBackToAimAfterHit = GO_TO_AIM_AFTER_HIT;
	}
	// if we were cowering (this is different from the bellow, we don't use that status flag for this animation)
	else if ( this->usAnimState == COWERING )
	{
		this->flags.bGoBackToAimAfterHit = GO_TO_COWERING_AFTER_HIT;
	}
	else
	{
		this->flags.bGoBackToAimAfterHit = NO_SPEC_STANCE_AFTER_HIT;
	}

	// IF COWERING, PLAY SPECIFIC GENERIC HIT STAND...
	if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
	{
		if ( this->stats.bLife == 0 || IS_MERC_BODY_TYPE( this ) )
		{
			this->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0, FALSE );
		}
		else
		{
			this->EVENT_InitNewSoldierAnim( CIV_COWER_HIT, 0, FALSE );
		}
		return;
	}

	// Change based on body type
	switch ( this->ubBodyType )
	{
	case COW:
		this->EVENT_InitNewSoldierAnim( COW_HIT, 0, FALSE );
		return;
		break;

	case BLOODCAT:
		this->EVENT_InitNewSoldierAnim( BLOODCAT_HIT, 0, FALSE );

		if ( Item[usWeaponIndex].usItemClass & ( IC_EXPLOSV | IC_BOBBY_GUN ) )
			this->usSoldierFlagMask |= SOLDIER_DAMAGED_VEST;
		return;
		break;

	case ADULTFEMALEMONSTER:
	case AM_MONSTER:
	case YAF_MONSTER:
	case YAM_MONSTER:

		this->EVENT_InitNewSoldierAnim( ADULTMONSTER_HIT, 0, FALSE );
		return;
		break;

	case LARVAE_MONSTER:
		this->EVENT_InitNewSoldierAnim( LARVAE_HIT, 0, FALSE );
		return;
		break;

	case QUEENMONSTER:
		this->EVENT_InitNewSoldierAnim( QUEEN_HIT, 0, FALSE );
		return;
		break;

	case CRIPPLECIV:

	{
					   // OK, do some code here to allow the fact that poor buddy can be thrown back if it's a big enough hit...
					   this->EVENT_InitNewSoldierAnim( CRIPPLE_HIT, 0, FALSE );

					   //this->stats.bLife = 0;
					   //this->EVENT_InitNewSoldierAnim( CRIPPLE_DIE_FLYBACK, 0 , FALSE );


	}
		return;
		break;

	case ROBOTNOWEAPON:
		this->EVENT_InitNewSoldierAnim( ROBOTNW_HIT, 0, FALSE );
		return;
		break;


	case INFANT_MONSTER:
		this->EVENT_InitNewSoldierAnim( INFANT_HIT, 0, FALSE );
		return;

	case CROW:

		this->EVENT_InitNewSoldierAnim( CROW_DIE, 0, FALSE );
		return;

		//case FATCIV:
	case MANCIV:
	case MINICIV:
	case DRESSCIV:
	case HATKIDCIV:
	case KIDCIV:

		// OK, if life is 0 and not set as dead ( this is a death hit... )
		if ( !(this->flags.uiStatusFlags & SOLDIER_DEAD) && this->stats.bLife == 0 )
		{
			// Randomize death!
			if ( Random( 2 ) )
			{
				this->EVENT_InitNewSoldierAnim( CIV_DIE2, 0, FALSE );
				return;
			}
		}

		// IF here, go generic hit ALWAYS.....
		this->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0, FALSE );
		return;
		break;
	}

	// If here, we are a merc, check if we are in water
	if ( this->MercInShallowWater( ) )
	{
		this->EVENT_InitNewSoldierAnim( WATER_HIT, 0, FALSE );
		return;
	}
	if ( this->MercInDeepWater( ) )
	{
		this->EVENT_InitNewSoldierAnim( DEEP_WATER_HIT, 0, FALSE );
		return;
	}

	// Flugente: cryo death
	if (this->stats.bLife <= 0 && this->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
	{
		if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND )
			this->EVENT_InitNewSoldierAnim( CRYO_DEATH, 0, TRUE );
		else
			this->EVENT_InitNewSoldierAnim( CRYO_DEATH_CROUCHED, 0, TRUE );

		return;
	}

	// SWITCH IN TYPE OF WEAPON
	if ( Item[usWeaponIndex].usItemClass & (IC_GUN | IC_THROWING_KNIFE) )
	{
		SoldierGotHitGunFire( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
		if ( Item[usWeaponIndex].usItemClass & IC_GUN )
		{
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_GUNFIRE, ubAttackerID );
			if (ubAttackerID != NOBODY)
			{
				PossiblyStartEnemyTaunt( MercPtrs[ubAttackerID], TAUNT_HIT_GUNFIRE, this->ubID );
			}
		}
		else
		{
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_THROWING_KNIFE, ubAttackerID );
			if (ubAttackerID != NOBODY)
			{
				PossiblyStartEnemyTaunt(MercPtrs[ubAttackerID], TAUNT_HIT_THROWING_KNIFE, this->ubID);
			}
		}
	}
	if ( Item[usWeaponIndex].usItemClass & IC_BLADE )
	{
		SoldierGotHitBlade( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
		// anv: taunts are called from UseBlade()
	}
	// marke setting ammo explosions included here with 3rd 'or' including ubReason
	if ( Item[usWeaponIndex].usItemClass & IC_EXPLOSV || Item[usWeaponIndex].usItemClass & IC_TENTACLES || ubReason == TAKE_DAMAGE_EXPLOSION )
	{
		SoldierGotHitExplosion( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
		if ( Item[usWeaponIndex].usItemClass & IC_EXPLOSV || ubReason == TAKE_DAMAGE_EXPLOSION )
		{
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_EXPLOSION, ubAttackerID );
			//PossiblyStartEnemyTaunt( MercPtrs[ubAttackerID], TAUNT_HIT_EXPLOSION, this );
		}
		else if ( Item[usWeaponIndex].usItemClass & IC_TENTACLES )
		{
			PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_TENTACLES, ubAttackerID );
			//PossiblyStartEnemyTaunt( MercPtrs[ubAttackerID], TAUNT_HIT_TENTACLES, this );
		}
	}
	if ( Item[usWeaponIndex].usItemClass & IC_PUNCH )
	{
		SoldierGotHitPunch( this, usWeaponIndex, sDamage, bDirection, sRange, ubAttackerID, ubSpecial, ubHitLocation );
		// anv: taunts are called from UseHandToHand()
	}
}
#pragma optimize("", on)
UINT8 CalcScreamVolume( SOLDIERTYPE * pSoldier, UINT8 ubCombinedLoss )
{
	// NB explosions are so loud they should drown out screams
	UINT8 ubVolume;

	if ( ubCombinedLoss < 1 )
	{
		ubVolume = 1;
	}
	else
	{
		ubVolume = ubCombinedLoss;
	}

	// Victim yells out in pain, making noise.  Yelps are louder from greater
	// wounds, but softer for more experienced soldiers.

	if ( ubVolume > (10 - EffectiveExpLevel( pSoldier )) )
	{
		ubVolume = 10 - EffectiveExpLevel( pSoldier );
	}

	/*
	// the "Speck factor"...  He's a whiner, and extra-sensitive to pain!
	if (ptr->trait == NERVOUS)
	ubVolume += 2;
	*/

	if ( ubVolume < 0 )
	{
		ubVolume = 0;
	}

	return(ubVolume);
}


void DoGenericHit( SOLDIERTYPE *pSoldier, UINT8 ubSpecial, INT16 bDirection )
{
	// Based on stance, select generic hit animation
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
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

			pSoldier->EVENT_InitNewSoldierAnim( STANDING_BURST_HIT, 0, FALSE );
		}
		else
		{
			// Check in hand for rifle
			if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
			{
				pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0, FALSE );
			}
			else
			{
				pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0, FALSE );
			}
		}
		break;

	case ANIM_PRONE:

		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0, FALSE );
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0, FALSE );
		break;

	}
}


void SoldierGotHitGunFire( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT16 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	INT32	usNewGridNo;
	BOOLEAN	fBlownAway = FALSE;
	BOOLEAN	fHeadHit = FALSE;
	BOOLEAN	fFallenOver = FALSE;

	// MAYBE CHANGE TO SPECIAL ANIMATION BASED ON VALUE SET BY DAMAGE CALCULATION CODE
	// ALL THESE ONLY WORK ON STANDING PEOPLE
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER) && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND && (!(gTacticalStatus.uiFlags & GODMODE) || pSoldier->bTeam != OUR_TEAM))
	{
		if ( gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND )
		{
			if ( ubSpecial == FIRE_WEAPON_HEAD_EXPLODE_SPECIAL )
			{
				if ( gGameSettings.fOptions[TOPTION_BLOOD_N_GORE] )
				{
					// HEADROCK HAM 3.6: Reattached "Max Distance For Messy Death" tag from the XML! God knows why it wasn't attached when they MADE THAT TAG.
					//if (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= Weapon[usWeaponIndex].maxdistformessydeath || (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && usWeaponIndex == BARRETT ))

					UINT8 ubDistMessy = Weapon[usWeaponIndex].maxdistformessydeath;
					// modify by ini values
					if ( Item[usWeaponIndex].usItemClass == IC_GUN )
						ubDistMessy *= gItemSettings.fDistMessyModifierGun[Weapon[usWeaponIndex].ubWeaponType];

					if ( SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= ubDistMessy )
					{
						usNewGridNo = NewGridNo( pSoldier->sGridNo, (INT8)(DirectionInc( pSoldier->ubDirection )) );

						// CHECK OK DESTINATION!
						if ( OKFallDirection( pSoldier, usNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, JFK_HITDEATH ) )
						{
							usNewGridNo = NewGridNo( usNewGridNo, (INT8)(DirectionInc( pSoldier->ubDirection )) );

							if ( OKFallDirection( pSoldier, usNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, pSoldier->usAnimState ) )
							{
								fHeadHit = TRUE;
							}
						}
					}
				}
			}
			else if ( ubSpecial == FIRE_WEAPON_CHEST_EXPLODE_SPECIAL )
			{
				if ( gGameSettings.fOptions[TOPTION_BLOOD_N_GORE] )
				{
					// HEADROCK HAM 3.6: Reattached "Max Distance For Messy Death" tag from the XML! God knows why it wasn't attached when they MADE THAT TAG.
					//if (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= Weapon[usWeaponIndex].maxdistformessydeath || (SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= MAX_BARRETT_DISTANCE_FOR_MESSY_DEATH && usWeaponIndex == BARRETT ))

					UINT8 ubDistMessy = Weapon[usWeaponIndex].maxdistformessydeath;
					// modify by ini values
					if ( Item[usWeaponIndex].usItemClass == IC_GUN )
						ubDistMessy *= gItemSettings.fDistMessyModifierGun[Weapon[usWeaponIndex].ubWeaponType];

					if ( SpacesAway( pSoldier->sGridNo, Menptr[ubAttackerID].sGridNo ) <= ubDistMessy )
					{

						// possibly play torso explosion anim!
						if ( pSoldier->ubDirection == bDirection )
						{
							usNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( gOppositeDirection[pSoldier->ubDirection] ) );

							if ( OKFallDirection( pSoldier, usNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[bDirection], FLYBACK_HIT ) )
							{
								usNewGridNo = NewGridNo( usNewGridNo, DirectionInc( gOppositeDirection[bDirection] ) );

								if ( OKFallDirection( pSoldier, usNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[bDirection], pSoldier->usAnimState ) )
								{
									fBlownAway = TRUE;
								}
							}
						}
					}
				}
			}
			else if ( ubSpecial == FIRE_WEAPON_LEG_FALLDOWN_SPECIAL )
			{
				// possibly play fall over anim!
				// this one is NOT restricted by distance
				if ( IsValidStance( pSoldier, ANIM_PRONE ) )
				{
					// Can't be in water, or not standing
					if ( gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND && !pSoldier->MercInWater( ) )
					{
						fFallenOver = TRUE;
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[20], pSoldier->GetName( ) );
					}
				}
			}
		}
	}

	// Flugente: if hit in legs or torso, blood will be on our uniform - parts of the clothes cannot be worn anymore
	if ( ubHitLocation == AIM_SHOT_TORSO )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_VEST;
	else if ( ubHitLocation == AIM_SHOT_LEGS )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_PANTS;

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
		// HEADROCK HAM 3.2: Critical legshots cost an extra number of APs, based on shot damage.
		if ( gGameExternalOptions.fCriticalLegshotCausesAPLoss )
		{
			DeductPoints( pSoldier, APBPConstants[AP_LOSS_PER_LEGSHOT_DAMAGE] * sDamage, 0, DISABLED_INTERRUPT );
		}
		SoldierCollapse( pSoldier );
		return;
	}

	if ( fBlownAway )
	{
		// Only for mercs...
		if ( pSoldier->ubBodyType < 4 )
		{
			pSoldier->ChangeToFlybackAnimation( (UINT8)bDirection );

			// Flugente: dynamic opinions
			if (gGameExternalOptions.fDynamicOpinions && ubAttackerID != NOBODY )
				HandleDynamicOpinionChange( MercPtrs[ubAttackerID], OPINIONEVENT_BRUTAL_GOOD, TRUE, TRUE );

			return;
		}
	}

	if ( fHeadHit )
	{
		// Only for mercs ( or KIDS! )
		if ( pSoldier->ubBodyType < 4 || pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV )
		{
			pSoldier->EVENT_InitNewSoldierAnim( JFK_HITDEATH, 0, FALSE );

			// Flugente: dynamic opinions
			if (gGameExternalOptions.fDynamicOpinions && ubAttackerID != NOBODY )
				HandleDynamicOpinionChange( MercPtrs[ubAttackerID], OPINIONEVENT_BRUTAL_GOOD, TRUE, TRUE );

			return;
		}
	}

	DoGenericHit( pSoldier, ubSpecial, bDirection );
}

void SoldierGotHitExplosion( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	// Flugente: if hit in legs or torso, blood will be on our uniform - parts of the clothes cannot be worn anymore
	if ( ubHitLocation == AIM_SHOT_TORSO )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_VEST;
	else if ( ubHitLocation == AIM_SHOT_LEGS )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_PANTS;

	INT32 sNewGridNo;

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}

	//check for services
	pSoldier->ReceivingSoldierCancelServices( );
	pSoldier->GivingSoldierCancelServices( );


	if ( gGameSettings.fOptions[TOPTION_BLOOD_N_GORE] )
	{
		if ( Explosive[Item[usWeaponIndex].ubClassIndex].ubRadius >= 3 && pSoldier->stats.bLife == 0 && gAnimControl[pSoldier->usAnimState].ubEndHeight != ANIM_PRONE )
		{
			if ( sRange >= 2 && sRange <= 4 )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_HIT1 );

				pSoldier->EVENT_InitNewSoldierAnim( CHARIOTS_OF_FIRE, 0, FALSE );
				return;
			}
			else if ( sRange <= 1 )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_HIT1 );

				pSoldier->EVENT_InitNewSoldierAnim( BODYEXPLODING, 0, FALSE );
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
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:
		if ( ubSpecial == FIRE_WEAPON_DEAFENED )
		{
			switch ( Random( 10 ) )
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
			switch ( Random( 10 ) )
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
				sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( gOppositeDirection[bDirection] ) );
				if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[bDirection], FLYBACK_HIT ) )
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
		sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( gOppositeDirection[bDirection] ) );

		if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[bDirection], FLYBACK_HIT ) )
		{
			pSoldier->ChangeToFallbackAnimation( (UINT8)bDirection );
		}
		else
		{
			if ( gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND )
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
	// Flugente: if hit in legs or torso, blood will be on our uniform - parts of the clothes cannot be worn anymore
	if ( ubHitLocation == AIM_SHOT_TORSO )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_VEST;
	else if ( ubHitLocation == AIM_SHOT_LEGS )
		pSoldier->usSoldierFlagMask |= SOLDIER_DAMAGED_PANTS;

	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}


	// Based on stance, select generic hit animation
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:

		// Check in hand for rifle
		if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0, FALSE );
		}
		else
		{
			pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0, FALSE );
		}
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0, FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0, FALSE );
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
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:
		// Check in hand for rifle
		if ( pSoldier->SoldierCarriesTwoHandedWeapon( ) )
		{
			pSoldier->EVENT_InitNewSoldierAnim( RIFLE_STAND_HIT, 0, FALSE );
		}
		else
		{
			pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_STAND, 0, FALSE );
		}
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_CROUCH, 0, FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( GENERIC_HIT_PRONE, 0, FALSE );
		break;

	}

}

void SoldierGotHitVehicle( SOLDIERTYPE *pSoldier, UINT16 usWeaponIndex, INT16 sDamage, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID, UINT8 ubSpecial, UINT8 ubHitLocation )
{
	INT32 sNewGridNo = 0;
	// IF HERE AND GUY IS DEAD, RETURN!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )
	{
		return;
	}

	if ( pSoldier->flags.fTryingToFall )
	{
		return;
	}

	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:

		sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( bDirection ) );//DirectionInc( gOppositeDirection[ bDirection ] ) );
		if ( IS_MERC_BODY_TYPE( pSoldier ) && OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, bDirection, FLYBACK_HIT ) )
		{
			pSoldier->EVENT_SetSoldierDirection( (INT8)gOppositeDirection[bDirection] );
			pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
			pSoldier->ChangeToFallbackAnimation( (UINT8)gOppositeDirection[bDirection] );
		}
		else if ( IS_MERC_BODY_TYPE( pSoldier ) )
		{
			pSoldier->EVENT_SetSoldierDirection( bDirection );
			pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
			pSoldier->BeginTyingToFall( );
			pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		}
		else
		{
			SoldierCollapse( pSoldier );
		}
		break;


	case ANIM_CROUCH:

		pSoldier->EVENT_SetSoldierDirection( (INT8)gOppositeDirection[bDirection] );
		pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

		// Check behind us!
		sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( bDirection ) );

		if ( IS_MERC_BODY_TYPE( pSoldier ) && OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, gOppositeDirection[pSoldier->ubDirection], FLYBACK_HIT ) )
		{
			pSoldier->ChangeToFallbackAnimation( (UINT8)gOppositeDirection[bDirection] );
		}
		else
		{
			SoldierCollapse( pSoldier );
		}
		break;

	case ANIM_PRONE:

		SoldierCollapse( pSoldier );
		break;
	}

}

BOOLEAN SOLDIERTYPE::EVENT_InternalGetNewSoldierPath( INT32 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim )
{
	INT32	iDest;
	INT32 sNewGridNo;
	BOOLEAN fContinue;
	UINT32	uiDist;
	UINT16	usAnimState;
	UINT16	usMoveAnimState = usMovementAnim;
	INT32							sMercGridNo;
	UINT16						usPathingData[MAX_PATH_LIST_SIZE];
	UINT8							ubPathingMaxDirection;
	BOOLEAN						fAdvancePath = TRUE;
	UINT8							fFlags = 0;

	//shadooow: if collapsed and enough breath, get up first and wait for new input
	if (this->bCollapsed && this->bBreath >= OKBREATH)
	{
		this->BeginSoldierGetup();
		if(!this->bCollapsed) return FALSE;
	}

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

	// sevenfm: stop cowering for mercs
	if (fFromUI &&
		IS_MERC_BODY_TYPE(this) &&
		//!(this->flags.uiStatusFlags & SOLDIER_COWERING) &&
		this->IsCowering())
	{
		this->StopCoweringAnimation();
	}

	// sevenfm: set WALKING when sidestepping, this should fix running instead of sidestepping with weapon raised in turnbased mode
	if (usMoveAnimState == RUNNING && this->bReverse)
		usMoveAnimState = WALKING;

	this->bGoodContPath = FALSE;

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
	if ( (!(gTacticalStatus.uiFlags & INCOMBAT) && (gAnimControl[this->usAnimState].uiFlags & ANIM_MOVING) && fFromUI == 1) || fFromUI == 2 )
	{
		if ( this->bCollapsed )
		{
			return(FALSE);
		}

		sMercGridNo = this->sGridNo;
		this->sGridNo = this->pathing.sDestination;

		// Check if path is good before copying it into guy's path...
		if ( !(uiDist = FindBestPath( this, sDestGridNo, this->pathing.bLevel, this->usUIMovementMode, COPYROUTE, fFlags )) )
		{
			// Set to old....
			this->sGridNo = sMercGridNo;

			return(FALSE);
		}

		//uiDist =  FindBestPath( this, sDestGridNo, this->pathing.bLevel, this->usUIMovementMode, COPYROUTE, fFlags );

		this->sGridNo = sMercGridNo;
		this->pathing.sFinalDestination = sDestGridNo;

		if ( uiDist > 0 )
		{
			// Add one to path data size....
			if ( fAdvancePath )
			{
				memcpy( usPathingData, this->pathing.usPathingData, sizeof(usPathingData) );
				ubPathingMaxDirection = (UINT8)usPathingData[MAX_PATH_LIST_SIZE - 1];
				memcpy( &(this->pathing.usPathingData[1]), usPathingData, sizeof(usPathingData)-sizeof(UINT16) );

				// If we have reach the max, go back one sFinalDest....
				if ( this->pathing.usPathDataSize == MAX_PATH_LIST_SIZE )
				{
					//this->pathing.sFinalDestination = NewGridNo( (INT16)this->pathing.sFinalDestination, DirectionInc( gOppositeDirection[ ubPathingMaxDirection ] ) );
				}
				else
				{
					++this->pathing.usPathDataSize;
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
				if ( is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usMoveAnimState, 0, FALSE );

				return(TRUE);
			}
			if ( is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, this->usAnimState, 255, FALSE );

			return(TRUE);
		}

		return(FALSE);
	}

	{
		iDest = FindBestPath( this, sDestGridNo, this->pathing.bLevel, usMovementAnim, COPYROUTE, fFlags );
		fContinue = (iDest != 0);
	}

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
		sNewGridNo = NewGridNo( this->sGridNo, DirectionInc( (UINT8)this->pathing.usPathingData[this->pathing.usPathIndex] ) );

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
		// SANDRO - we may try to move with raised weapon, so don't end aim after
		if ( (gAnimControl[this->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) &&
			 (usMoveAnimState == WALKING || usMoveAnimState == SIDE_STEP) && !(this->MercInWater( )) )
		{
			usAnimState = INVALID_ANIMATION;
		}
		else
		{
			usAnimState = PickSoldierReadyAnimation( this, TRUE, FALSE );
		}

		// Add a pending animation first!
		// Only if we were standing!
		if ( usAnimState != INVALID_ANIMATION && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND )
		{
			this->EVENT_InitNewSoldierAnim( usAnimState, 0, FALSE );
			this->usPendingAnimation = usMoveAnimState;
			if ( is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usAnimState, 0, FALSE );
		}
		else
		{
			// Call local copy for change soldier state!
			this->EVENT_InitNewSoldierAnim( usMoveAnimState, 0, fForceRestartAnim );
			if ( is_server || (is_client && this->ubID <20) ) send_path( this, sDestGridNo, usMovementAnim, 0, fForceRestartAnim );

		}

		// Change desired direction
		// ATE: Here we have a situation where in RT, we may have
		// gotten a new path, but we are alreayd moving.. so
		// at leasty change new dest. This will be redundent if the ANI is a totaly new one

		return(TRUE);
	}

	return(FALSE);
}

void SOLDIERTYPE::EVENT_GetNewSoldierPath( INT32 sDestGridNo, UINT16 usMovementAnim )
{
	// ATE: Default restart of animation to TRUE
	this->EVENT_InternalGetNewSoldierPath( sDestGridNo, usMovementAnim, FALSE, TRUE );
}

// Change our state based on stance, to stop!
void SOLDIERTYPE::StopSoldier( void )
{
	this->ReceivingSoldierCancelServices( );
	this->GivingSoldierCancelServices( );

	if ( !(gAnimControl[this->usAnimState].uiFlags & ANIM_STATIONARY) )
	{
		//this->SoldierGotoStationaryStance( );
		this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
	}

	// Set destination
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
			this->EVENT_InitNewSoldierAnim( QUEEN_READY, 0, TRUE );
			return;
		}
	}

	// Check if we are in deep water!
	if ( this->MercInDeepWater( ) )
	{
		// IN deep water, tred!
		this->EVENT_InitNewSoldierAnim( DEEP_WATER_TRED, 0, FALSE );
	}
	else if ( this->ubServicePartner != NOBODY && this->stats.bLife >= OKLIFE && this->bBreath > 0 )
	{
		if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
		{
			if ( !is_networked )
				this->EVENT_InitNewSoldierAnim( GIVING_AID_PRN, 0, FALSE );
			else
				this->ChangeSoldierState( GIVING_AID_PRN, 0, 0 );
		}
		else
		{
			if ( !is_networked )
				this->EVENT_InitNewSoldierAnim( GIVING_AID, 0, FALSE );
			else
				this->ChangeSoldierState( GIVING_AID, 0, 0 );
		}

	}
	else
	{
		// Change state back to stationary state for given height
		switch ( gAnimControl[this->usAnimState].ubEndHeight )
		{
		case ANIM_STAND:

			// If we are cowering....goto cower state
			if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
			{
				this->EVENT_InitNewSoldierAnim( START_COWER, 0, FALSE );
			}
			// Flugente: if we walk with our gun raised, we should still have it raised once we stop walking
			else if ( this->usAnimState == WALKING_WEAPON_RDY || this->usAnimState == AIM_RIFLE_STAND )
			{
				this->EVENT_InitNewSoldierAnim( AIM_RIFLE_STAND, 0, FALSE );
			}
			else if ( this->usAnimState == WALKING_DUAL_RDY || this->usAnimState == AIM_DUAL_STAND )
			{
				this->EVENT_InitNewSoldierAnim( AIM_DUAL_STAND, 0, FALSE );
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( STANDING, 0, FALSE );
			}
			break;

		case ANIM_CROUCH:

			// If we are cowering....goto cower state
			if ( this->flags.uiStatusFlags & SOLDIER_COWERING )
			{
				this->EVENT_InitNewSoldierAnim( COWERING, 0, FALSE );
			}
			// Flugente: if we walk with our gun raised, we should still have it raised once we stop walking
			else if (this->usAnimState == CROUCHEDMOVE_RIFLE_READY )
			{
				this->EVENT_InitNewSoldierAnim(AIM_RIFLE_CROUCH, 0, FALSE);
			}
			else if (this->usAnimState == CROUCHEDMOVE_PISTOL_READY )
			{
				this->EVENT_InitNewSoldierAnim(AIM_RIFLE_CROUCH, 0, FALSE);
			}
			else if (this->usAnimState == CROUCHEDMOVE_DUAL_READY)
			{
				this->EVENT_InitNewSoldierAnim(AIM_DUAL_CROUCH, 0, FALSE);
			}
			else
			{
				this->EVENT_InitNewSoldierAnim( CROUCHING, 0, FALSE );
			}
			break;

		case ANIM_PRONE:
			this->EVENT_InitNewSoldierAnim( PRONE, 0, FALSE );
			break;
		}

	}

}


void SOLDIERTYPE::ChangeSoldierStance( UINT8 ubDesiredStance )
{	
	// Check if they are the same!
	if ( ubDesiredStance == gAnimControl[this->usAnimState].ubEndHeight )
	{
		// Free up from stance change
		FreeUpNPCFromStanceChange( this );
		return;
	}

	if ( !IsValidStance( this, ubDesiredStance ) )
	{
		//ADB this was recently put in as an assert, but that would make the game crash in certain circumstances, including the example below
		//you can comment out the breakpoint if it annoys you, but the return should remain for the indefinate future
		//if it's removed you'll have stuff happen like when out of breath and lying collapsed on the ground
		//and trying to climb onto a roof you'll crouch, stand, reach for the roof, and then collapse again, which looks really weird.
		DebugBreakpoint( );
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
		UINT16 usNewState = this->GetNewSoldierStateFromNewStance( ubDesiredStance );

		// Set desired stance
		this->ubDesiredHeight = ubDesiredStance;

		this->EVENT_InitNewSoldierAnim(usNewState, 0, FALSE);
	}

	this->usSoldierFlagMask |= SOLDIER_REDOFLASHLIGHT;
}

void SOLDIERTYPE::EVENT_InternalSetSoldierDestination( UINT16	usNewDirection, BOOLEAN fFromMove, UINT16 usAnimState )
{
	INT32	sNewGridNo;
	INT16		sXPos, sYPos;

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( this->sGridNo, DirectionInc( (UINT8)usNewDirection ) );

	ConvertGridNoToCenterCellXY( sNewGridNo, &sXPos, &sYPos );

	// Save new dest gridno, x, y
	this->pathing.sDestination = sNewGridNo;
	this->pathing.sDestXPos = sXPos;
	this->pathing.sDestYPos = sYPos;

	this->bMovementDirection = (INT8)usNewDirection;


	// OK, ATE: If we are side_stepping, calculate a NEW desired direction....
	if ( this->bReverse && 
		(usAnimState == SIDE_STEP ||
			usAnimState == ROLL_PRONE_R ||
			usAnimState == ROLL_PRONE_L ||
			usAnimState == SIDE_STEP_CROUCH_RIFLE ||
			usAnimState == SIDE_STEP_CROUCH_PISTOL ||
			usAnimState == SIDE_STEP_CROUCH_DUAL ||
			usAnimState == SIDE_STEP_ALTERNATIVE_RDY ||
			usAnimState == SIDE_STEP_WEAPON_RDY ||
			usAnimState == SIDE_STEP_DUAL_RDY) )
	{
		UINT8 ubPerpDirection;

		// Get a new desired direction,
		ubPerpDirection = gPurpendicularDirection[this->ubDirection][usNewDirection];

		// CHange actual and desired direction....
		this->EVENT_SetSoldierDirection( ubPerpDirection );
		this->pathing.bDesiredDirection = this->ubDirection;
	}
	else
	{
		if ( !(gAnimControl[usAnimState].uiFlags & ANIM_SPECIALMOVE) )
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
	bTurningIncrement = (INT8)QuickestDirection( bStartDirection, bDesiredDirection );

	usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, pSoldier->usUIMovementMode );

	pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, pSoldier->usUIMovementMode );
	if ( !pStructureFileRef )
	{
		// without structure data, well, assume quickest direction
		return(bTurningIncrement);
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

	while ( bLoop < 2 )
	{
		while ( bCurrentDirection != bDesiredDirection )
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
			fOk = OkayToAddStructureToWorld( pSoldier->sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[bCurrentDirection]]), usStructureID );
			if ( !fOk )
			{
				break;
			}
		}

		if ( (bCurrentDirection == bDesiredDirection) && fOk )
		{
			// success!!
			return(bTurningIncrement);
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
	return(bTurningIncrement);
}



void EVENT_InternalSetSoldierDesiredDirection( SOLDIERTYPE *pSoldier, UINT8	ubNewDirection, BOOLEAN fInitalMove, UINT16 usAnimState )
{
	INT16 sAPCost = 0;
	INT32 iBPCost = 0;

	//if ( usAnimState == WALK_BACKWARDS )
	if ( pSoldier->bReverse && 
		(usAnimState != SIDE_STEP && 
			usAnimState != ROLL_PRONE_R &&
			usAnimState != ROLL_PRONE_L	&&
			usAnimState != SIDE_STEP_CROUCH_RIFLE &&
			usAnimState != SIDE_STEP_CROUCH_PISTOL &&
			usAnimState != SIDE_STEP_CROUCH_DUAL &&
			usAnimState != SIDE_STEP_WEAPON_RDY &&
			usAnimState != SIDE_STEP_DUAL_RDY &&
			usAnimState != SIDE_STEP_ALTERNATIVE_RDY) )
	{
		// OK, check if we are going to go in the exact opposite than our facing....
		ubNewDirection = gOppositeDirection[ubNewDirection];
	}
	
	pSoldier->pathing.bDesiredDirection = (INT8)ubNewDirection;

	// If we are prone, goto crouched first!
	// ONly if we are stationary, and only if directions are differnet!

	// ATE: If we are fNoAPsToFinnishMove, stop what we were doing and
	// reset flag.....
	if ( pSoldier->flags.fNoAPToFinishMove && (gAnimControl[usAnimState].uiFlags & ANIM_MOVING) )
	{
		// ATE; Commented this out: NEVER, EVER, start a new anim from this function, as an eternal loop will result....
		//pSoldier->SoldierGotoStationaryStance( );
		// Reset flag!
		pSoldier->AdjustNoAPToFinishMove( FALSE );
	}

	if ( pSoldier->pathing.bDesiredDirection != pSoldier->ubDirection )
	{
		if ( (gAnimControl[usAnimState].uiFlags & (ANIM_BREATH | ANIM_OK_CHARGE_AP_FOR_TURN | ANIM_FIREREADY | ANIM_TURNING) || usForceAnimState != INVALID_ANIMATION) && !fInitalMove && !pSoldier->flags.fDontChargeTurningAPs )
		{
			// SANDRO: hey, we have a function for this around, why not to use it, hm?
			// silversurfer: we better don't do that. GetAPsToLook( ... ) will charge APs for getting to crouched/prone position
			// which is already done by SOLDIERTYPE::EVENT_InitNewSoldierAnim( ... ), we would charge twice...
			// SANDRO: I see. Thanks.
			// DeductPoints( pSoldier, GetAPsToLook( pSoldier ), 0 );
			// Deduct points for initial turn!
			switch ( gAnimControl[(usForceAnimState != INVALID_ANIMATION ? usForceAnimState : usAnimState)].ubEndHeight )//dnl ch70 160913
			{
				// Now change to appropriate animation
			case ANIM_STAND:
				sAPCost = APBPConstants[AP_LOOK_STANDING];
				break;

			case ANIM_CROUCH:
				sAPCost = APBPConstants[AP_LOOK_CROUCHED];
				break;

			case ANIM_PRONE:
				sAPCost = APBPConstants[AP_LOOK_PRONE];
				break;
			}
			// martial artists can turn faster
			if ( HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ) && gGameOptions.fNewTraitSystem )
				sAPCost = max( 1, (INT16)(sAPCost * (100 - gSkillTraitValues.ubMAApsTurnAroundReduction * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )) / 100.0f + 0.5f) );

			// SANDRO: get BP cost for weapon manipulating
			if ( gGameExternalOptions.ubEnergyCostForWeaponWeight )
				iBPCost = sAPCost * GetBPCostPer10APsForGunHolding( pSoldier ) / 10;
			else
				iBPCost = 0;

			DeductPoints( pSoldier, sAPCost, iBPCost );
			if ( usForceAnimState != INVALID_ANIMATION )//dnl ch70 170913
				pSoldier->flags.fDontUnsetLastTargetFromTurn = FALSE;
		}

		pSoldier->flags.fDontChargeTurningAPs = FALSE;

		if ( fInitalMove )
		{
			if ( gAnimControl[usAnimState].ubHeight == ANIM_PRONE )
			{
				if ( pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE )
				{
					pSoldier->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_START_UP_FROM_MOVE;
				}
			}
		}

		if ( gAnimControl[usAnimState].uiFlags & ANIM_STATIONARY || pSoldier->flags.fNoAPToFinishMove || fInitalMove )
		{
			if ( gAnimControl[usAnimState].ubHeight == ANIM_PRONE )
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
	pSoldier->ubHiResDesiredDirection = ubExtDirection[pSoldier->pathing.bDesiredDirection];

	if ( pSoldier->pathing.bDesiredDirection != pSoldier->ubDirection )
	{
		if ( pSoldier->flags.uiStatusFlags & (SOLDIER_VEHICLE) || CREATURE_OR_BLOODCAT( pSoldier ) )
		{
			pSoldier->flags.uiStatusFlags |= SOLDIER_PAUSEANIMOVE;
		}
	}


	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		pSoldier->bTurningIncrement = (INT8)ExtQuickestDirection( pSoldier->ubHiResDirection, pSoldier->ubHiResDesiredDirection );
	}
	else
	{
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE )
		{
			pSoldier->bTurningIncrement = (INT8)MultiTiledTurnDirection( pSoldier, pSoldier->ubDirection, pSoldier->pathing.bDesiredDirection );
		}
		else
		{
			pSoldier->bTurningIncrement = (INT8)QuickestDirection( pSoldier->ubDirection, pSoldier->pathing.bDesiredDirection );
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

	// Flugente
	BOOLEAN fNew = (this->ubDirection != (INT8)usNewDirection);

	this->ubDirection = (INT8)usNewDirection;

	// Updated extended direction.....
	this->ubHiResDirection = ubExtDirection[this->ubDirection];

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

	// Flugente: only update flashlights if we changed our direction
	if ( fNew )
		this->HandleFlashLights( );
}


void SOLDIERTYPE::EVENT_BeginMercTurn( BOOLEAN fFromRealTime, INT32 iRealTimeCounter )
{
	// NB realtimecounter is not used, always passed in as 0 now!

	INT32 iBlood;

	if ( this->aiData.bUnderFire )
	{
		// UnderFire now starts at 2 for "under fire this turn",
		// down to 1 for "under fire last turn", to 0.
		this->aiData.bUnderFire--;

		if ( !this->aiData.bUnderFire )
			this->usSoldierFlagMask2 &= ~SOLDIER_TAKEN_LARGE_HIT;
	}

	// sevenfm: reset AI flags
	this->usSoldierFlagMask2 &= ~SOLDIER_BACK_ATTACK;
	this->usSoldierFlagMask2 &= ~SOLDIER_SNEAK_ATTACK;

	// Flugente: reset extra stats. Currently they only depend on drug effects, and those are reset every turn
	this->ResetExtraStats( );

	// ATE: Add decay effect sfor drugs...
	//if ( fFromRealTime  ) //&& iRealTimeCounter % 300 )
	{
		HandleEndTurnDrugAdjustments_New( this );
	}

	// sevenfm: update morale
	RefreshSoldierMorale(this);

	// ATE: Don't bleed if in AUTO BANDAGE!
	if ( !gTacticalStatus.fAutoBandageMode )
	{
		// Blood is not for the weak of heart, or mechanical
		if ( !(this->flags.uiStatusFlags & (SOLDIER_VEHICLE | SOLDIER_ROBOT)) )
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

		// sevenfm: increase morale for AI soldiers
		if (this->ubProfile == NO_PROFILE &&
			!(this->flags.uiStatusFlags & SOLDIER_VEHICLE) &&
			!AM_A_ROBOT(this) &&
			!ARMED_VEHICLE(this) &&
			this->aiData.bShock == 0 &&
			!this->aiData.bUnderFire &&
			this->aiData.bMorale < 80 + 2 * this->stats.bExpLevel)
		{
			this->aiData.bMorale = __min(80 + 2 * this->stats.bExpLevel, this->aiData.bMorale + 2 + this->stats.bExpLevel / 5);
		}

		// if this person has heard a noise that hasn't been investigated
		if ( this->aiData.sNoiseGridno != NOWHERE )
		{
			if ( this->aiData.ubNoiseVolume )	// and the noise volume is still positive
			{
				this->aiData.ubNoiseVolume--;	// the volume of the noise "decays" by 1 point

				if ( !this->aiData.ubNoiseVolume )	// if the volume has reached zero
				{
					this->aiData.sNoiseGridno = NOWHERE;		// forget about the noise!
				}
			}
		}

		// save unused action points up to a maximum
		/*
		if ((savedPts = this->aiData.bActionPts) > APBPConstants[MAX_AP_CARRIED])
		savedPts = APBPConstants[MAX_AP_CARRIED];
		*/
		if ( this->flags.uiStatusFlags & SOLDIER_GASSED )
		{
			// then must get a gas mask or leave the gassed area to get over it
			if ( DoesSoldierWearGasMask( this ) && this->inv[FindGasMask( this )][0]->data.objectStatus >= GASMASK_MIN_STATUS || !(GetSmokeEffectOnTile( this->sGridNo, this->pathing.bLevel )) )//dnl ch40 200909
				this->flags.uiStatusFlags &= (~SOLDIER_GASSED);
		}

		if ( this->bBlindedCounter > 0 )
		{
			this->bBlindedCounter--;
			if ( this->bBlindedCounter == 0 )
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

		// Flugente: update for various personal properties
		// this has to happen before CalculateCarriedWeight(), otherwise strength modfiers will not be detected correctly
		this->SoldierPropertyUpkeep( );

		// Flugente: drug users might consume useful drugs on their own in combat
		this->DrugAutoUse();

		this->sWeightCarriedAtTurnStart = (INT16)CalculateCarriedWeight( this );

		UnusedAPsToBreath( this );

		// Set flag back to normal, after reaching a certain statge
		if ( this->bBreath > 80 )
		{
			this->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_LOW_BREATH);
		}
		if ( this->bBreath > 50 )
		{
			this->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_DROWNING);
		}


		if ( this->ubTurnsUntilCanSayHeardNoise > 0 )
		{
			this->ubTurnsUntilCanSayHeardNoise--;
		}

		if ( this->bInSector )
		{
			this->CheckForBreathCollapse( );
		}

		this->CalcNewActionPoints( );

		// SANDRO - Improved Interrupt System - reset interrupt counter
		memset( this->aiData.ubInterruptCounter, 0, sizeof(this->aiData.ubInterruptCounter) );

		// HEADROCK HAM 3.6: If this soldier is in a "moving" animation, but has not moved any tiles
		// in the previous turn, then the player has apparently forgotten that he was moving.
		// In this case, abort the character's action.

		// If hasn't moved since the start of last round
		// AND this function is being executed in Turn Based mode
		// AND character is a player-controlled merc
		if ( !fFromRealTime && !this->bTilesMoved && this->bTeam == OUR_TEAM )
		{
			// but are doing a movement animation
			if ( !(gAnimControl[this->usAnimState].uiFlags & ANIM_STATIONARY) )
			{
				// Stop the merc
				this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
				this->pathing.sFinalDestination = NOWHERE;
			}

			// Reset destination
			//this->pathing.sFinalDestination = this->sGridNo;
		}

		this->bTilesMoved = 0;

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
				if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) )
				{
					if ( MercIsHot( this ) )
					{
						HandleMoraleEvent( this, MORALE_HEAT_INTOLERANT_IN_DESERT, this->sSectorX, this->sSectorY, this->bSectorZ );
						if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) && this->ubWhatKindOfMercAmI != MERC_TYPE__PLAYER_CHARACTER )
						{
							TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
							this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;

							// Flugente: dynamic opinions
							if (gGameExternalOptions.fDynamicOpinions)
							{
								HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
							}
						}
					}
				}

				if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) )
				{
					if ( MercSeesCreature( this ) )
					{
						HandleMoraleEvent( this, MORALE_INSECT_PHOBIC_SEES_CREATURE, this->sSectorX, this->sSectorY, this->bSectorZ );
						if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
						{
							TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
							this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;

							// Flugente: dynamic opinions
							if (gGameExternalOptions.fDynamicOpinions)
							{
								HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
							}
						}
					}
				}

				if ( DoesMercHaveDisability( this, CLAUSTROPHOBIC ) )
				{
					if ( gbWorldSectorZ > 0 && Random( 6 - gbWorldSectorZ ) == 0 )
					{
						// underground!
						HandleMoraleEvent( this, MORALE_CLAUSTROPHOBE_UNDERGROUND, this->sSectorX, this->sSectorY, this->bSectorZ );
						if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
						{
							TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );
							this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;

							// Flugente: dynamic opinions
							if (gGameExternalOptions.fDynamicOpinions)
							{
								HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
							}
						}
					}
				}

				if ( DoesMercHaveDisability( this, NERVOUS ) )
				{
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

								// Flugente: dynamic opinions
								if (gGameExternalOptions.fDynamicOpinions)
								{
									HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
								}
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
				}
			}
		}

		// Reset quote flags for under heavy fire and close call!
		this->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_BEING_PUMMELED);
		this->usQuoteSaidExtFlags &= (~SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL);
		this->bNumHitsThisTurn = 0;
		this->ubSuppressionPoints = 0;

		// HEADROCK HAM 3.5: After considerable testing, suppression is now cleared after every attack. Total APs lost
		// is cleared every turn (here) and only acts as reference now (no effect on AP loss).
		this->ubAPsLostToSuppression = 0;
		this->ubLastShock = 0;
		this->ubLastSuppression = 0;
		this->ubLastAP = 0;
		this->ubLastMorale = 0;
		this->ubLastAPFromHit = 0;
		this->ubLastShockFromHit = 0;
		this->ubLastMoraleFromHit = 0;
		this->iLastBulletImpact = 0;
		this->iLastArmourProtection = 0;

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

	// HEADROCK HAM 4: Store this soldier's X/Y cell coordinates into his SOLDIERTYPE data.
	INT16 sStartPosX = 0;
	INT16 sStartPosY = 0;
	ConvertGridNoToCenterCellXY( this->sGridNo, &sStartPosX, &sStartPosY );
	this->sOldXPos = sStartPosX;
	this->sOldYPos = sStartPosY;

	// Flugente: Cool down all weapons and decay food in inventory
	this->SoldierInventoryCoolDown( );
}

// UTILITY FUNCTIONS CALLED BY OVERHEAD.H
UINT8		gDirectionFrom8to2[] = {0, 0, 1, 1, 0, 1, 1, 0};

// Flugente: frozen soldiers do not move. We simulate this by using fixed animation frames, which we determine here
UINT16 SOLDIERTYPE::CryoAniFrame()
{
	// get anim surface and determine # of frames
	UINT16 usAnimSurface = GetSoldierAnimationSurface( this, this->usAnimState );
	
	//If we are only one frame, ignore what the script is telling us!
	if ( usAnimSurface == INVALID_ANIMATION_SURFACE || gAnimSurfaceDatabase[usAnimSurface].hVideoObject == NULL || gAnimSurfaceDatabase[usAnimSurface].ubFlags & ANIM_DATA_FLAG_NOFRAMES )
	{
		return 0;
	}

	// COnvert world direction into sprite direction
	UINT8 ubTempDir = gOneCDirection[this->ubDirection];

	if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 32 )
	{
		ubTempDir = gExtOneCDirection[this->ubHiResDirection];
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 4 )
	{
		ubTempDir = ubTempDir / 2;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 1 )
	{
		ubTempDir = 0;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 3 )
	{
		if ( this->ubDirection == NORTHWEST )
		{
			ubTempDir = 1;
		}
		else if ( this->ubDirection == WEST )
		{
			ubTempDir = 0;
		}
		else if ( this->ubDirection == EAST )
		{
			ubTempDir = 2;
		}
	}
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 2 )
	{
		ubTempDir = gDirectionFrom8to2[this->ubDirection];
	}

	UINT16 cryoframe = 0;

	UINT16 newframe = cryoframe + (UINT16)((gAnimSurfaceDatabase[usAnimSurface].uiNumFramesPerDir * ubTempDir));
	
	if ( newframe >= gAnimSurfaceDatabase[usAnimSurface].hVideoObject->usNumberOfObjects )
	{
		return 0;
	}

	return newframe;
}

BOOLEAN SOLDIERTYPE::ConvertAniCodeToAniFrame( UINT16 usAniFrame )
{
	UINT16	usAnimSurface;
	UINT8		ubTempDir;
	// Given ani code, adjust for facing direction

	// get anim surface and determine # of frames
	usAnimSurface = GetSoldierAnimationSurface( this, this->usAnimState );

	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	// COnvert world direction into sprite direction
	ubTempDir = gOneCDirection[this->ubDirection];

	//If we are only one frame, ignore what the script is telling us!
	if ( gAnimSurfaceDatabase[usAnimSurface].ubFlags & ANIM_DATA_FLAG_NOFRAMES )
	{
		usAniFrame = 0;
	}

	if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 32 )
	{
		ubTempDir = gExtOneCDirection[this->ubHiResDirection];
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 4 )
	{
		ubTempDir = ubTempDir / 2;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 1 )
	{
		ubTempDir = 0;
	}
	// Check # of directions /surface, adjust if ness.
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 3 )
	{
		if ( this->ubDirection == NORTHWEST )
		{
			ubTempDir = 1;
		}
		else if ( this->ubDirection == WEST )
		{
			ubTempDir = 0;
		}
		else if ( this->ubDirection == EAST )
		{
			ubTempDir = 2;
		}
	}
	else if ( gAnimSurfaceDatabase[usAnimSurface].uiNumDirections == 2 )
	{
		ubTempDir = gDirectionFrom8to2[this->ubDirection];
	}

	this->usAniFrame = usAniFrame + (UINT16)((gAnimSurfaceDatabase[usAnimSurface].uiNumFramesPerDir * ubTempDir));

	if ( gAnimSurfaceDatabase[usAnimSurface].hVideoObject == NULL )
	{
		this->usAniFrame = 0;
		return(TRUE);
	}

	if ( this->usAniFrame >= gAnimSurfaceDatabase[usAnimSurface].hVideoObject->usNumberOfObjects )
	{
		// Debug msg here....
		//		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Soldier Animation: Wrong Number of frames per number of objects: %d vs %d, %S",  gAnimSurfaceDatabase[ usAnimSurface ].uiNumFramesPerDir, gAnimSurfaceDatabase[ usAnimSurface ].hVideoObject->usNumberOfObjects, gAnimControl[ this->usAnimState ].zAnimStr );

		this->usAniFrame = 0;
	}

	return(TRUE);
}


void SOLDIERTYPE::TurnSoldier( void )
{
	INT16		sDirection;
	BOOLEAN	fDoDirectionChange = TRUE;
	INT32		cnt;

	// If we are a vehicle... DON'T TURN!
	// anv: YES PLIZ DO
	//if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	//{
	//	if ( this->ubBodyType != TANK_NW && this->ubBodyType != TANK_NE )
	//	{
	//		return;
	//	}
	//}
	//else	// Lesh: patch for "Bug: Enemy turns around in turn based mode!"
	{
		// in case of errors in turning tasks
		if ( this->pathing.bDesiredDirection > 7 || this->pathing.bDesiredDirection < 0 )
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "TurnSoldier() Warinig: Invalid desired direction for non-vehicle unit" ) );
			this->pathing.bDesiredDirection = this->ubDirection;
		}
	}
	// Lesh: patch ended

	// We handle sight now....
	if ( this->flags.uiStatusFlags & SOLDIER_LOOK_NEXT_TURNSOLDIER )
	{
		if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_STATIONARY && this->usAnimState != CLIMBUPROOF && this->usAnimState != CLIMBDOWNROOF && this->usAnimState != JUMPUPWALL && this->usAnimState != JUMPDOWNWALL) )
		{
			// HANDLE SIGHT!
			HandleSight( this, SIGHT_LOOK | SIGHT_RADIO );
		}
		// Turn off!
		this->flags.uiStatusFlags &= (~SOLDIER_LOOK_NEXT_TURNSOLDIER);

		HandleSystemNewAISituation( this, FALSE );
	}

	if ( this->flags.fTurningToShoot )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			if ( ((gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY) &&
				this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_OFF) ||
				this->ubBodyType == ROBOTNOWEAPON ||
				ARMED_VEHICLE( this ) )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "TurnSoldier: EVENT_InitNewSoldierAnim" ) );
				this->EVENT_InitNewSoldierAnim( SelectFireAnimation( this, gAnimControl[this->usAnimState].ubEndHeight ), 0, FALSE );
				this->flags.fTurningToShoot = FALSE;

				// Save last target gridno!
				//this->sLastTarget = this->sTargetGridNo;

			}
			// Else check if we are trying to shoot and once was prone, but am now crouched because we needed to turn...
			else if ( this->flags.bTurningFromPronePosition )
			{
				//dnl ch71 170913
				if ( IsValidStance( this, ANIM_PRONE ) )
				{
					UINT16 usTrueAnimState = this->usAnimState;
					this->usAnimState = PRONE;
					this->usPendingAnimation = PickSoldierReadyAnimation( this, FALSE, FALSE );
					this->usAnimState = usTrueAnimState;
					SendChangeSoldierStanceEvent( this, ANIM_PRONE );
				}
				else
					this->EVENT_InitNewSoldierAnim( PickSoldierReadyAnimation( this, FALSE, FALSE ), 0, FALSE );
				this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
				return;
				/*
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
				//}
				*/
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

	if ( this->flags.fTurningUntilDone && (this->ubPendingStanceChange != NO_PENDING_STANCE) )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			SendChangeSoldierStanceEvent( this, this->ubPendingStanceChange );
			this->ubPendingStanceChange = NO_PENDING_STANCE;
			this->flags.fTurningUntilDone = FALSE;
		}
	}

	if ( this->flags.fTurningUntilDone && (this->usPendingAnimation != NO_PENDING_ANIMATION) )
	{
		if ( this->ubDirection == this->pathing.bDesiredDirection )
		{
			UINT16 usPendingAnimation;

			usPendingAnimation = this->usPendingAnimation;
			this->usPendingAnimation = NO_PENDING_ANIMATION;

			this->EVENT_InitNewSoldierAnim( usPendingAnimation, 0, FALSE );
			this->flags.fTurningUntilDone = FALSE;
		}
	}

	// Don't do anything if we are at dest direction!
	if ( this->ubDirection == this->pathing.bDesiredDirection )
	{
		if ( ARMED_VEHICLE( this ) )
		{
			if ( this->iTuringSoundID != NO_SAMPLE )
			{
				SoundStop( this->iTuringSoundID );
				this->iTuringSoundID = NO_SAMPLE;

				PlaySoldierJA2Sample( this->ubID, TURRET_STOP, RATE_11025, SoundVolume( HIGHVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ), TRUE );
			}
		}

		// Turn off!
		this->flags.uiStatusFlags &= (~SOLDIER_LOOK_NEXT_TURNSOLDIER);
		this->flags.fDontUnsetLastTargetFromTurn = FALSE;

		// Unset ui busy if from ui
		if ( this->bTurningFromUI &&
			 (this->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE) &&
			 (this->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ON) )
		{
			UnSetUIBusy( this->ubID );
			this->bTurningFromUI = FALSE;
		}

		if ( this->flags.uiStatusFlags & (SOLDIER_VEHICLE) || CREATURE_OR_BLOODCAT( this ) )
		{
			this->flags.uiStatusFlags &= (~SOLDIER_PAUSEANIMOVE);
		}

		FreeUpNPCFromTurning( this, LOOK );

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
				this->EVENT_InitNewSoldierAnim( this->usUIMovementMode, 0, FALSE );
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
					this->flags.uiStatusFlags &= (~SOLDIER_TURNINGFROMHIT);
					this->flags.fGettingHit = FALSE;
				}
			}
			else if ( this->flags.fGettingHit == 2 )
			{
				// Turn off
				this->flags.uiStatusFlags &= (~SOLDIER_TURNINGFROMHIT);


				// Release attacker
				// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, turning from hit animation ended") );
				// ReleaseSoldiersAttacker( this );
				// 0verhaul:  Since I disabled the turn from hit ABC increase, I need to disable the turn from hit decrease too.
				DebugAttackBusy( "Finished turning from hit.  Not Reducing attack busy.\n" );
				//ReduceAttackBusyCount( );

				//FREEUP GETTING HIT FLAG
				this->flags.fGettingHit = FALSE;
			}
		}

		if ( this->flags.bTurningFromPronePosition == TURNING_FROM_PRONE_FOR_PUNCH_OR_STAB )//dnl ch73 290913
		{
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
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
		if ( sDirection > 31 )
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
		for ( cnt = 0; cnt < NUM_WORLD_DIRECTIONS; ++cnt )
		{
			if ( sDirection == ubExtDirection[cnt] )
			{
				fDoDirectionChange = TRUE;

				sDirection = (INT16)cnt;

				break;
			}
		}

		if ( ARMED_VEHICLE( this ) )
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
		if ( sDirection > NORTHWEST )
		{
			sDirection = NORTH;
		}
		else
		{
			if ( sDirection < NORTH )
			{
				sDirection = NORTHWEST;
			}
		}
	}

	// CHECK FOR A VALID TURN DIRECTION
	// This is needed for prone animations as well as any multi-tiled structs
	if ( fDoDirectionChange )
	{
		if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			// need to turn around passengers inside
			INT32 iId = this->bVehicleID;

			// check which side vehicle turned
			INT16 bDirectionChange = QuickestDirection( this->ubDirection, this->pathing.bDesiredDirection );

			// Loop through passengers and update each guy's rotation
			for ( INT32 iCounter = 0; iCounter < gNewVehicle[pVehicleList[iId].ubVehicleType].iNewSeatingCapacities; iCounter++ )
			{
				if ( pVehicleList[iId].pPassengers[iCounter] != NULL )
				{
					pVehicleList[iId].pPassengers[iCounter]->flags.fDontChargeTurningAPs = TRUE;
					pVehicleList[iId].pPassengers[iCounter]->EVENT_SetSoldierDesiredDirection( (pVehicleList[iId].pPassengers[iCounter]->pathing.bDesiredDirection + bDirectionChange + NUM_WORLD_DIRECTIONS) % NUM_WORLD_DIRECTIONS );
				}
			}
			UpdateAllVehiclePassengersGridNo( this );
		}

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
				if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_STATIONARY) )
				{
					this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;
				}
				// otherwise, it's handled next tile...
			}

			this->EVENT_SetSoldierDirection( sDirection );

			if ( this->ubBodyType != LARVAE_MONSTER && !this->MercInWater( ) && this->bOverTerrainType != DIRT_ROAD && this->bOverTerrainType != PAVED_ROAD && !(this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
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


UINT8	gRedGlowR[] =
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



UINT8	gOrangeGlowR[] =
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



UINT8	gOrangeGlowG[] =
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
	CHAR8	zColFilename[100];
	INT32 iWhich;
	INT32 cnt;
	INT8	bBodyTypePalette;
	SGPPaletteEntry							Temp8BPPPalette[256];

	//NT32 uiCount;
	//PPaletteEntry Pal[256];

	if ( this->p8BPPPalette != NULL )
	{
		MemFree( this->p8BPPPalette );
		this->p8BPPPalette = NULL;
	}

	// Allocate mem for new palette
	this->p8BPPPalette = (SGPPaletteEntry *)MemAlloc( sizeof(SGPPaletteEntry)* 256 );
	memset( this->p8BPPPalette, 0, sizeof(SGPPaletteEntry)* 256 );

	CHECKF( this->p8BPPPalette != NULL );

	// --- TAKE FROM CURRENT ANIMATION HVOBJECT!
	usAnimSurface = GetSoldierAnimationSurface( this, this->usAnimState );

	//	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	if ( (bBodyTypePalette = GetBodyTypePaletteSubstitutionCode( this, this->ubBodyType, zColFilename )) == -1 )
	{
		// ATE: here we want to use the breath cycle for the palette.....
		usPaletteAnimSurface = LoadSoldierAnimationSurface( this, STANDING );

		if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
		{
			// Use palette from HVOBJECT, then use substitution for pants, etc
			memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof(this->p8BPPPalette) * 256 );

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
		memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[usAnimSurface].hVideoObject->pPaletteEntry, sizeof(this->p8BPPPalette) * 256 );
	}
	else
	{
		// Use col file
		if ( CreateSGPPaletteFromCOLFile( Temp8BPPPalette, zColFilename ) )
		{
			// Copy into palette
			memcpy( this->p8BPPPalette, Temp8BPPPalette, sizeof(this->p8BPPPalette) * 256 );
		}
		else
		{
			// Use palette from hvobject
			memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[usAnimSurface].hVideoObject->pPaletteEntry, sizeof(this->p8BPPPalette) * 256 );
		}
	}

	if ( this->p16BPPPalette != NULL )
	{
		MemFree( this->p16BPPPalette );
		this->p16BPPPalette = NULL;
	}

	// -- BUILD 16BPP Palette from this
	this->p16BPPPalette = Create16BPPPalette( this->p8BPPPalette );

	for ( iWhich = 0; iWhich < NUM_SOLDIER_SHADES; ++iWhich )
	{
		if ( this->pShades[iWhich] != NULL )
		{
			MemFree( this->pShades[iWhich] );
			this->pShades[iWhich] = NULL;
		}
	}

	for ( iWhich = 0; iWhich < NUM_SOLDIER_EFFECTSHADES; ++iWhich )
	{
		if ( this->pEffectShades[iWhich] != NULL )
		{
			MemFree( this->pEffectShades[iWhich] );
			this->pEffectShades[iWhich] = NULL;
		}
	}

	for ( iWhich = 0; iWhich < 20; ++iWhich )
	{
		if ( this->pGlowShades[iWhich] != NULL )
		{
			MemFree( this->pGlowShades[iWhich] );
			this->pGlowShades[iWhich] = NULL;
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
	this->pEffectShades[0] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	this->pEffectShades[1] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 150, 100, TRUE );

	// Build shades for glowing visible bad guy

	// First do visible guy
	this->pGlowShades[0] = Create16BPPPaletteShaded( this->p8BPPPalette, 255, 255, 255, FALSE );
	for ( cnt = 1; cnt < 10; ++cnt )
	{
		this->pGlowShades[cnt] = CreateEnemyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[cnt], 255, FALSE );
	}

	// Now for gray guy...
	this->pGlowShades[10] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	for ( cnt = 11; cnt < 19; ++cnt )
	{
		this->pGlowShades[cnt] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[cnt], 0, FALSE );
	}
	this->pGlowShades[19] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gRedGlowR[18], 0, FALSE );


	// ATE: OK, piggyback on the shades we are not using for 2 colored lighting....
	// ORANGE, VISIBLE GUY
	this->pShades[20] = Create16BPPPaletteShaded( this->p8BPPPalette, 255, 255, 255, FALSE );
	for ( cnt = 21; cnt < 30; ++cnt )
	{
		this->pShades[cnt] = CreateEnemyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[(cnt - 20)], gOrangeGlowG[(cnt - 20)], TRUE );
	}

	// ORANGE, GREY GUY
	this->pShades[30] = Create16BPPPaletteShaded( this->p8BPPPalette, 100, 100, 100, TRUE );
	for ( cnt = 31; cnt < 39; ++cnt )
	{
		this->pShades[cnt] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[(cnt - 20)], gOrangeGlowG[(cnt - 20)], TRUE );
	}
	this->pShades[39] = CreateEnemyGreyGlow16BPPPalette( this->p8BPPPalette, gOrangeGlowR[18], gOrangeGlowG[18], TRUE );

	return(TRUE);
}



void AdjustAniSpeed( SOLDIERTYPE *pSoldier )
{
	if ( (gTacticalStatus.uiFlags & SLOW_ANIMATION) )
	{
		if ( gTacticalStatus.bRealtimeSpeed == -1 )
		{
			pSoldier->sAniDelay = 10000;
		}
		else
		{
			pSoldier->sAniDelay = pSoldier->sAniDelay * (1 * gTacticalStatus.bRealtimeSpeed / 2);
		}
	}

	//pSoldier->sAniDelay =1;//for max speed uncomment //ddd
	RESETTIMECOUNTER( pSoldier->timeCounters.UpdateCounter, pSoldier->sAniDelay );
}


void CalculateSoldierAniSpeed( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pStatsSoldier )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "CalculateSoldierAniSpeed" );
	INT16 sTerrainDelay;

	INT8 bBreathDef = 0, bLifeDef = 0;
	INT16 bAgilDef = 0;
	INT16 bAdditional = 0;
	INT16 legbrokenpenalty = 60;

	// for those animations which have a speed of zero, we have to calculate it
	// here. Some animation, such as water-movement, have an ADDITIONAL speed
	switch ( pSoldier->usAnimState )
	{
		// Lesh: bursting animation delay control begins
		// Add your animation ID to control it
	case STANDING_BURST:
	case FIRE_STAND_BURST_SPREAD:
	case FIRE_BURST_LOW_STAND:
	case TANK_BURST:
	case CROUCHED_BURST:
	case PRONE_BURST:
	case BURST_ALTERNATIVE_STAND:
	case LOW_BURST_ALTERNATIVE_STAND:
		pSoldier->sAniDelay = Weapon[Item[pSoldier->inv[HANDPOS].usItem].ubClassIndex].sAniDelay;
		AdjustAniSpeed( pSoldier );
		return;
	case BURST_DUAL_STAND:
	case BURST_DUAL_CROUCH:
	case BURST_DUAL_PRONE:
		pSoldier->sAniDelay = (Weapon[Item[pSoldier->inv[HANDPOS].usItem].ubClassIndex].sAniDelay) / 2;
		AdjustAniSpeed( pSoldier );
		return;

	case PRONE:
	case STANDING:

		pSoldier->sAniDelay = (pStatsSoldier->bBreath * 2) + (100 - pStatsSoldier->stats.bLife);

		// Limit it!
		if ( pSoldier->sAniDelay < 40 )
		{
			pSoldier->sAniDelay = 40;
		}
		AdjustAniSpeed( pSoldier );
		return;

	case CROUCHING:

		pSoldier->sAniDelay = (pStatsSoldier->bBreath * 2) + ((100 - pStatsSoldier->stats.bLife));

		// Limit it!
		if ( pSoldier->sAniDelay < 40 )
		{
			pSoldier->sAniDelay = 40;
		}
		AdjustAniSpeed( pSoldier );
		return;

	case WALKING:
	case WALKING_WEAPON_RDY:
	case WALKING_DUAL_RDY:
	case WALKING_ALTERNATIVE_RDY:

		// Adjust based on body type
		bAdditional = gubAnimWalkSpeeds[pStatsSoldier->ubBodyType].sSpeed;

		// Flugente: disease can stop us from using our arms normally
		if ( gGameExternalOptions.fDisease
			&& gGameExternalOptions.fDiseaseSevereLimitations
			&& pSoldier->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
			bAdditional += legbrokenpenalty;

		if ( bAdditional < 0 )
			bAdditional = 0;
		break;

	case RUNNING:

		// Adjust based on body type
		bAdditional = gubAnimWalkSpeeds[pStatsSoldier->ubBodyType].sSpeed;

		// Flugente: disease can stop us from using our arms normally
		if ( gGameExternalOptions.fDisease
			&& gGameExternalOptions.fDiseaseSevereLimitations
			&& pSoldier->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
			bAdditional += legbrokenpenalty;

		if ( bAdditional < 0 )
			bAdditional = 0;
		break;

	case SWATTING:
		//***ddd
	case SWATTING_WK:
	case SWAT_BACKWARDS_WK:
	case CROUCHEDMOVE_RIFLE_READY:
	case CROUCHEDMOVE_PISTOL_READY:
	case CROUCHEDMOVE_DUAL_READY:
	case SIDE_STEP_CROUCH_RIFLE:
	case SIDE_STEP_CROUCH_PISTOL:
	case SIDE_STEP_CROUCH_DUAL:

		// Adjust based on body type
		if ( pStatsSoldier->ubBodyType <= REGFEMALE )
		{
			bAdditional = gubAnimWalkSpeeds[pStatsSoldier->ubBodyType].sSpeed;

			// Flugente: disease can stop us from using our arms normally
			if ( gGameExternalOptions.fDisease
				&& gGameExternalOptions.fDiseaseSevereLimitations
				&& pSoldier->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
				bAdditional += legbrokenpenalty;

			if ( bAdditional < 0 )
				bAdditional = 0;
		}
		break;

	case CRAWLING:

		// Adjust based on body type
		if ( pStatsSoldier->ubBodyType <= REGFEMALE )
		{
			bAdditional = gubAnimWalkSpeeds[pStatsSoldier->ubBodyType].sSpeed;

			// Flugente: disease can stop us from using our arms normally
			if ( gGameExternalOptions.fDisease
				&& gGameExternalOptions.fDiseaseSevereLimitations
				&& pSoldier->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
				bAdditional += legbrokenpenalty;

			if ( bAdditional < 0 )
				bAdditional = 0;
		}
		break;

	case READY_RIFLE_STAND:

		// Raise rifle based on aim vs non-aim.
		if ( pSoldier->aiData.bAimTime == 0 )
		{
			// Quick shot
			pSoldier->sAniDelay = 100;
		}
		else
		{
			pSoldier->sAniDelay = 200;
		}
		AdjustAniSpeed( pSoldier );
		return;
	}

	// figure out movement speed (terrspeed)
	if ( gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_MOVING )
	{
		sTerrainDelay = gsTerrainTypeSpeedModifiers[pStatsSoldier->bOverTerrainType];
	}
	else
	{
		sTerrainDelay = 40;			// standing still
	}

	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
	{
		bBreathDef = 50 - (pStatsSoldier->bBreath / 2);

		if ( bBreathDef > 30 )
			bBreathDef = 30;

		bAgilDef = 50 - (EffectiveAgility( pStatsSoldier, FALSE ) / 4);
		bLifeDef = 50 - (pStatsSoldier->stats.bLife / 2);
	}
	else
	{
		// anv: vehicles have no agility and making them slower with less fuel would make no sense
		// instead take gear into consideration here
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE && pSoldier->usAnimState == RUNNING )
		{
			bAgilDef = 10;
		}
		else
		{
			bAgilDef = 30;
		}
	}
	
	sTerrainDelay += (bLifeDef + bBreathDef + bAgilDef + bAdditional);

	// Flugente: backgrounds
	switch ( pSoldier->usAnimState )
	{
	case WALKING:
	case WALKING_WEAPON_RDY:
	case WALKING_DUAL_RDY:
	case CROUCHEDMOVE_RIFLE_READY:
	case CROUCHEDMOVE_PISTOL_READY:
	case CROUCHEDMOVE_DUAL_READY:
	case WALKING_ALTERNATIVE_RDY:
	case RUNNING:
	case SWATTING:
	case SWATTING_WK:
	case SIDE_STEP_CROUCH_RIFLE:
	case SIDE_STEP_CROUCH_PISTOL:
	case SIDE_STEP_CROUCH_DUAL:
	case SWAT_BACKWARDS_WK:
		// Flugente: background running speed reduces time needed: + is good, - is bad
		sTerrainDelay = ( sTerrainDelay * (100 - pSoldier->GetBackgroundValue( BG_PERC_SPEED_RUNNING ))) / 100;
		break;

	default:
		break;
	}

	pSoldier->sAniDelay = sTerrainDelay;

	// If a moving animation and we're on drugs, increase speed....
	if ( gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_MOVING )
	{
		if ( pSoldier->newdrugs.size[DRUG_EFFECT_AP] )
		{
			pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
		}
	}

	// MODIFTY NOW BASED ON REAL-TIME, ETC
	// Adjust speed, make twice as fast if in turn-based!
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
	}

	// MODIFY IF REALTIME COMBAT
	if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		// ATE: If realtime, and stealth mode...
		if ( pStatsSoldier->bStealthMode )
		{
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT ) )
			{
				// Stealthy skill decreases movement speed penalty while on stealthy mode - SANDRO
				pSoldier->sAniDelay = (INT16)((pSoldier->sAniDelay * (200 - gSkillTraitValues.ubSTStealthModeSpeedBonus)) / 100);
			}
			else // original
			{
				pSoldier->sAniDelay = (INT16)(pSoldier->sAniDelay * 2);
			}
		}

		// SANDRO - STOMP traits - bonus to movement speed for Athletics
		if ( gGameOptions.fNewTraitSystem && (gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_MOVING) )
		{
			if ( HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ) )
			{
				pSoldier->sAniDelay = (INT16)(pSoldier->sAniDelay * (100 - min( 75, gSkillTraitValues.ubATAPsMovementReduction )) / 100);
			}
		}

		//pSoldier->sAniDelay = pSoldier->sAniDelay * ( 1 * gTacticalStatus.bRealtimeSpeed / 2 );
	}

	// Flugente: riot shields lower movement speed
	if (pSoldier->IsRiotShieldEquipped())
	{
		pSoldier->sAniDelay = gItemSettings.fShieldMovementAPCostModifier * pSoldier->sAniDelay;
	}

	// Flugente: drag people
	if ( pSoldier->IsDragging( false ) )
	{
		pSoldier->sAniDelay = gItemSettings.fDragAPCostModifier * pSoldier->sAniDelay;
	}
}

FLOAT GetSpeedUpFactor( )
{
	switch ( gTacticalStatus.ubCurrentTeam )
	{
	case OUR_TEAM:
		return gGameExternalOptions.giPlayerTurnSpeedUpFactor;
	case ENEMY_TEAM:
		return gGameExternalOptions.giEnemyTurnSpeedUpFactor;
	case CREATURE_TEAM:
		return gGameExternalOptions.giCreatureTurnSpeedUpFactor;
	case MILITIA_TEAM:
		return gGameExternalOptions.giMilitiaTurnSpeedUpFactor;
	case CIV_TEAM:
		return gGameExternalOptions.giCivilianTurnSpeedUpFactor;
	}

	return 1.0;
}

void SetSoldierAniSpeed( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pStatsSoldier;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "SetSoldierAniSpeed" );

	// ATE: If we are an enemy and are not visible......
	// Set speed to 0
	if ( !is_client )
	{
		if ( (gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT)) || gTacticalStatus.fAutoBandageMode )
		{
			if ( ((pSoldier->bVisible == -1 && pSoldier->bVisible == pSoldier->bLastRenderVisibleValue) || gTacticalStatus.fAutoBandageMode) && pSoldier->usAnimState != MONSTER_UP )
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
		pStatsSoldier = MercPtrs[pSoldier->bOverrideMoveSpeed];
	}

	// Only calculate if set to zero
	if ( (pSoldier->sAniDelay = gAnimControl[pSoldier->usAnimState].sSpeed) == 0 )
	{
		CalculateSoldierAniSpeed( pSoldier, pStatsSoldier );
	}

	AdjustAniSpeed( pSoldier );

	// SANDRO - make the spin kick animation a bit faster 
	if (pSoldier->usAnimState == NINJA_SPINKICK || 
		pSoldier->usAnimState == FOCUSED_PUNCH || pSoldier->usAnimState == FOCUSED_STAB || pSoldier->usAnimState == FOCUSED_HTH_KICK)
	{
		pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
	}

	// sevenfm: faster radio animation
	if (pSoldier->usAnimState == AI_RADIO || pSoldier->usAnimState == AI_CR_RADIO)
	{
		pSoldier->sAniDelay = pSoldier->sAniDelay / 2;
	}

	// sevenfm: faster sidestepping
	if (pSoldier->usAnimState == SIDE_STEP || pSoldier->usAnimState == SIDE_STEP_ALTERNATIVE_RDY || pSoldier->usAnimState == SIDE_STEP_WEAPON_RDY || pSoldier->usAnimState == SIDE_STEP_DUAL_RDY)
	{
		pSoldier->sAniDelay = pSoldier->sAniDelay / 4;
	}

	if ( _KeyDown( SPACE ) )
	{
		//pSoldier->sAniDelay = 1000;
	}

	if ( gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT )
	if ( GetSpeedUpFactor( ) )
		pSoldier->sAniDelay = (INT16)((FLOAT)pSoldier->sAniDelay * GetSpeedUpFactor( ));
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
	if ( !FileRead( hFile, &guiNumPaletteSubRanges, sizeof(guiNumPaletteSubRanges), (UINT32 *)NULL ) )
	{
		return(FALSE);
	}

	// Malloc!
	gpPaletteSubRanges = (PaletteSubRangeType *)MemAlloc( sizeof(PaletteSubRangeType)* guiNumPaletteSubRanges );
	gubpNumReplacementsPerRange = (UINT8 *)MemAlloc( sizeof(UINT8)* guiNumPaletteSubRanges );

	// Read # of types for each!
	for ( cnt = 0; cnt < guiNumPaletteSubRanges; ++cnt )
	{
		if ( !FileRead( hFile, &gubpNumReplacementsPerRange[cnt], sizeof(UINT8), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}
	}

	// Loop for each one, read in data
	for ( cnt = 0; cnt < guiNumPaletteSubRanges; ++cnt )
	{
		if ( !FileRead( hFile, &gpPaletteSubRanges[cnt].ubStart, sizeof(UINT8), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}
		if ( !FileRead( hFile, &gpPaletteSubRanges[cnt].ubEnd, sizeof(UINT8), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}
	}

	// Read # of palettes
	if ( !FileRead( hFile, &guiNumReplacements, sizeof(guiNumReplacements), (UINT32 *)NULL ) )
	{
		return(FALSE);
	}

	// Malloc!
	gpPalRep = (PaletteReplacementType *)MemAlloc( sizeof(PaletteReplacementType)* guiNumReplacements );

	// Read!
	for ( cnt = 0; cnt < guiNumReplacements; ++cnt )
	{
		// type
		if ( !FileRead( hFile, &gpPalRep[cnt].ubType, sizeof(gpPalRep[cnt].ubType), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}

		if ( !FileRead( hFile, &gpPalRep[cnt].ID, sizeof(gpPalRep[cnt].ID), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}

		// # entries
		if ( !FileRead( hFile, &gpPalRep[cnt].ubPaletteSize, sizeof(gpPalRep[cnt].ubPaletteSize), (UINT32 *)NULL ) )
		{
			return(FALSE);
		}

		// Malloc
		gpPalRep[cnt].r = (UINT8 *)MemAlloc( gpPalRep[cnt].ubPaletteSize );
		CHECKF( gpPalRep[cnt].r != NULL );
		gpPalRep[cnt].g = (UINT8 *)MemAlloc( gpPalRep[cnt].ubPaletteSize );
		CHECKF( gpPalRep[cnt].g != NULL );
		gpPalRep[cnt].b = (UINT8 *)MemAlloc( gpPalRep[cnt].ubPaletteSize );
		CHECKF( gpPalRep[cnt].b != NULL );

		for ( cnt2 = 0; cnt2 < gpPalRep[cnt].ubPaletteSize; ++cnt2 )
		{
			if ( !FileRead( hFile, &gpPalRep[cnt].r[cnt2], sizeof(UINT8), (UINT32 *)NULL ) )
			{
				return(FALSE);
			}
			if ( !FileRead( hFile, &gpPalRep[cnt].g[cnt2], sizeof(UINT8), (UINT32 *)NULL ) )
			{
				return(FALSE);
			}
			if ( !FileRead( hFile, &gpPalRep[cnt].b[cnt2], sizeof(UINT8), (UINT32 *)NULL ) )
			{
				return(FALSE);
			}
		}
	}

	FileClose( hFile );

	return(TRUE);
}

BOOLEAN	SetPaletteReplacement( SGPPaletteEntry *p8BPPPalette, PaletteRepID aPalRep )
{
	UINT32 cnt2;
	UINT8	 ubType;
	UINT8  ubPalIndex;

	CHECKF( GetPaletteRepIndexFromID( aPalRep, &ubPalIndex ) );

	// Get range type
	ubType = gpPalRep[ubPalIndex].ubType;

	for ( cnt2 = gpPaletteSubRanges[ubType].ubStart; cnt2 <= gpPaletteSubRanges[ubType].ubEnd; ++cnt2 )
	{
		p8BPPPalette[cnt2].peRed = gpPalRep[ubPalIndex].r[cnt2 - gpPaletteSubRanges[ubType].ubStart];
		p8BPPPalette[cnt2].peGreen = gpPalRep[ubPalIndex].g[cnt2 - gpPaletteSubRanges[ubType].ubStart];
		p8BPPPalette[cnt2].peBlue = gpPalRep[ubPalIndex].b[cnt2 - gpPaletteSubRanges[ubType].ubStart];
	}

	return(TRUE);
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

	for ( cnt = 0; cnt < guiNumReplacements; ++cnt )
	{
		// Free
		if ( gpPalRep[cnt].r != NULL )
		{
			MemFree( gpPalRep[cnt].r );
			gpPalRep[cnt].r = NULL;
		}
		if ( gpPalRep[cnt].g != NULL )
		{
			MemFree( gpPalRep[cnt].g );
			gpPalRep[cnt].g = NULL;
		}
		if ( gpPalRep[cnt].b != NULL )
		{
			MemFree( gpPalRep[cnt].b );
			gpPalRep[cnt].b = NULL;
		}
	}

	// Free
	if ( gpPalRep != NULL )
	{
		MemFree( gpPalRep );
		gpPalRep = NULL;
	}

	return(TRUE);
}


BOOLEAN GetPaletteRepIndexFromID( PaletteRepID aPalRep, UINT8 *pubPalIndex )
{
	UINT32 cnt;

	// Check if type exists
	for ( cnt = 0; cnt < guiNumReplacements; cnt++ )
	{
		if ( COMPARE_PALETTEREP_ID( aPalRep, gpPalRep[cnt].ID ) )
		{
			*pubPalIndex = (UINT8)cnt;
			return(TRUE);
		}
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Invalid Palette Replacement ID given" );
	return(FALSE);
}

UINT16 SOLDIERTYPE::GetNewSoldierStateFromNewStance( UINT8 ubDesiredStance )
{
	UINT16 usNewState;
	INT8	bCurrentHeight;

	bCurrentHeight = (ubDesiredStance - gAnimControl[this->usAnimState].ubEndHeight);

	// Now change to appropriate animation

	switch ( bCurrentHeight )
	{
	case ANIM_STAND - ANIM_CROUCH:
		usNewState = KNEEL_UP;
		break;
	case ANIM_CROUCH - ANIM_STAND:
		usNewState = KNEEL_DOWN;
		break;

	case ANIM_STAND - ANIM_PRONE:
		usNewState = PRONE_UP;
		break;
	case ANIM_PRONE - ANIM_STAND:
		usNewState = KNEEL_DOWN;
		break;

	case ANIM_CROUCH - ANIM_PRONE:
		usNewState = PRONE_UP;
		break;
	case ANIM_PRONE - ANIM_CROUCH:
		usNewState = PRONE_DOWN;
		break;

	default:

		// Cannot get here unless ub desired stance is bogus
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "GetNewSoldierStateFromNewStance bogus ubDesiredStance value %d", ubDesiredStance ) );
		usNewState = this->usAnimState;

	}

	return(usNewState);
}


void MoveMercFacingDirection( SOLDIERTYPE *pSoldier, BOOLEAN fReverse, FLOAT dMovementDist )
{
	FLOAT					dAngle = (FLOAT)0;

	// Determine which direction we are in
	switch ( pSoldier->ubDirection )
	{
	case NORTH:
		dAngle = (FLOAT)(-1 * PI);
		break;

	case NORTHEAST:
		dAngle = (FLOAT)(PI * .75);
		break;

	case EAST:
		dAngle = (FLOAT)(PI / 2);
		break;

	case SOUTHEAST:
		dAngle = (FLOAT)(PI / 4);
		break;

	case SOUTH:
		dAngle = (FLOAT)0;
		break;

	case SOUTHWEST:
		//dAngle = (FLOAT)(  PI * -.25 );
		dAngle = (FLOAT)-0.786;
		break;

	case WEST:
		dAngle = (FLOAT)(PI *-.5);
		break;

	case NORTHWEST:
		dAngle = (FLOAT)(PI * -.75);
		break;

	}

	if ( fReverse )
	{
		dMovementDist = dMovementDist * -1;
	}

	pSoldier->MoveMerc( dMovementDist, dAngle, FALSE );

}

void SOLDIERTYPE::BeginSoldierClimbUpRoof(void)
{
	//CHRISL: Disable climbing up to a roof while wearing a backpack
	if (!this->CanClimbWithCurrentBackpack())
	{
		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, NewInvMessage[NIV_NO_CLIMB]);
		return;
	}

	if (is_client)
	{
		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[43]);
		return;//hayden disable climbing roof
	}

	INT8							bNewDirection;
	UINT16							ubWhoIsThere;

	if (FindHeigherLevel(this, this->sGridNo, this->ubDirection, &bNewDirection) && (this->pathing.bLevel == 0))
	{
		if (EnoughPoints(this, GetAPsToClimbRoof(this, FALSE), 0, TRUE))
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2(NewGridNo(this->sGridNo, (UINT16)DirectionInc(bNewDirection)), 1);
			if (ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID)
			{
				DebugAttackBusy(String("Soldier %d tried to climb up on someone.\n", this->ubID));
				this->aiData.bAction = AI_ACTION_NONE;
				return;
			}
			else
			{
				if (this->bTeam == gbPlayerNum)
				{
					// OK, SET INTERFACE FIRST
					SetUIBusy(this->ubID);
				}

				this->sTempNewGridNo = NewGridNo(this->sGridNo, (UINT16)DirectionInc(bNewDirection));

				this->ubPendingDirection = bNewDirection;
				//this->usPendingAnimation = CLIMBUPROOF;

				// Flugente: In case an animation is missing (zombies with bodytype of civilians), we TELEPORT instead
				if (IsAnimationValidForBodyType(this, CLIMBUPROOF) == FALSE)
				{
					SetSoldierHeight(50.0);
					TeleportSoldier(this, this->sTempNewGridNo, TRUE);
					EndAIGuysTurn(this);
				}
				else
					this->EVENT_InitNewSoldierAnim(CLIMBUPROOF, 0, FALSE);

				// Flugente: if we are afraid of heights, we complain
				if (DoesMercHaveDisability(this, AFRAID_OF_HEIGHTS))
				{
					if (!(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY))
					{
						HandleMoraleEvent(this, MORALE_FEAR_OF_HEIGHTS, this->sSectorX, this->sSectorY, this->bSectorZ);

						TacticalCharacterDialogue(this, QUOTE_PERSONALITY_TRAIT);
						this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;

						// Flugente: dynamic opinions
						if (gGameExternalOptions.fDynamicOpinions)
						{
							HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
						}
					}
					// otherwise remove flag, so we'll complain every second time we climb roof
					else
					{
						this->usQuoteSaidFlags &= ~SOLDIER_QUOTE_SAID_PERSONALITY;
					}
				}

				this->InternalReceivingSoldierCancelServices(FALSE);
				this->InternalGivingSoldierCancelServices(FALSE);
			}
		}
		else
		{
			DebugAttackBusy(String("Soldier %d tried to climb without AP.\n", this->ubID));
			this->aiData.bAction = AI_ACTION_NONE;
		}
	}
	else
	{
		DebugAttackBusy(String("Soldier %d tried to climb where no roof is.\n", this->ubID));
		this->aiData.bAction = AI_ACTION_NONE;
	}
}

void SOLDIERTYPE::BeginSoldierClimbFence( void )
{
	INT8							bDirection;

	// Make sure we hop the correct fence to follow our path!
	if ( this->pathing.usPathIndex < this->pathing.usPathDataSize )
	{
		bDirection = (INT8) this->pathing.usPathingData[this->pathing.usPathIndex];
	}
	else
	{
		bDirection = this->ubDirection;
	}

	if ( FindFenceJumpDirection( this, this->sGridNo, bDirection, &bDirection ) )
	{
		this->sTempNewGridNo = NewGridNo( this->sGridNo, (UINT16)DirectionInc( bDirection ) );

		// Flugente: we want to jump OVER the structure, not INTO it!
		this->sTempNewGridNo = NewGridNo( this->sTempNewGridNo, (UINT16)DirectionInc( bDirection ) );

		this->flags.fDontChargeTurningAPs = TRUE;
		EVENT_InternalSetSoldierDesiredDirection( this, bDirection, FALSE, this->usAnimState );
		this->flags.fTurningUntilDone = TRUE;
		// ATE: Reset flag to go back to prone...
		this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
		this->usPendingAnimation = HOPFENCE;
	}
}

//---legion by JAzz---------------------------------------

void SOLDIERTYPE::BeginSoldierClimbWindow( void )
{
	INT8	bDirection;

	// Make sure we hop the correct fence to follow our path!
	if ( this->pathing.usPathIndex < this->pathing.usPathDataSize )
	{
		bDirection = (INT8) this->pathing.usPathingData[this->pathing.usPathIndex];
	}
	else
	{
		bDirection = this->ubDirection;
	}

	if ( FindWindowJumpDirection( this, this->sGridNo, bDirection, &bDirection ) && this->pathing.bLevel == 0 && (this->ubDirection == NORTH || this->ubDirection == EAST || this->ubDirection == SOUTH || this->ubDirection == WEST) )
	{
		this->sTempNewGridNo = NewGridNo( this->sGridNo, (UINT16)DirectionInc( bDirection ) );
		this->flags.fDontChargeTurningAPs = TRUE;
		EVENT_InternalSetSoldierDesiredDirection( this, bDirection, FALSE, this->usAnimState );
		this->flags.fTurningUntilDone = TRUE;
		// ATE: Reset flag to go back to prone...

		// Flugente: In case an animation is missing (civilian bodytypes), we TELEPORT instead
		if ( IsAnimationValidForBodyType( this, JUMPWINDOWS ) == FALSE )
		{
			TeleportSoldier( this, this->sTempNewGridNo, TRUE );
		}
		else
		{
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_OFF;
			this->usPendingAnimation = JUMPWINDOWS;
		}

		// Flugente: should be fixed now, re-enable if not
		// Flugente: if an AI guy, end turn (weird endless clock syndrome)
		//if ( this->bTeam != OUR_TEAM )
		//EndAIGuysTurn( this);

		// Flugente: if we are jumping through an intact window, smash it during our animation
		if ( gGameExternalOptions.fCanJumpThroughClosedWindows )
		{
			INT32 sNewGridNo = sGridNo;
			if ( this->ubDirection == NORTH || this->ubDirection == WEST )
				sNewGridNo = NewGridNo( sGridNo, (UINT16)DirectionInc( (UINT8)this->ubDirection ) );

			// is there really an intact window that we jump through?
			if ( IsJumpableWindowPresentAtGridNo( sNewGridNo, this->ubDirection, TRUE ) && !IsJumpableWindowPresentAtGridNo( sNewGridNo, this->ubDirection, FALSE ) )
			{
				STRUCTURE * pStructure = FindStructure( sNewGridNo, STRUCTURE_WALLNWINDOW );
				if ( pStructure && !(pStructure->fFlags & STRUCTURE_OPEN) )
				{
					// intact window found. Smash it!
					WindowHit( sNewGridNo, pStructure->usStructureID, (this->ubDirection == SOUTH || this->ubDirection == EAST), TRUE );

					// we get a bit of damage for jumping through a window
					this->SoldierTakeDamage( 0, 2 + Random( 4 ), 1000, TAKE_DAMAGE_ELECTRICITY, NOBODY, sNewGridNo, 0, TRUE );
				}
			}
		}
	}
}

void SOLDIERTYPE::BeginSoldierClimbWall( void )
{
	INT8							bNewDirection;
	UINT16							ubWhoIsThere;

	if ( FindWallJumpDirection( this, this->sGridNo, this->ubDirection, &bNewDirection ) && (this->pathing.bLevel == 0) )
	{
		if ( EnoughPoints( this, GetAPsToClimbRoof( this, FALSE ), 0, TRUE ) )
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2( NewGridNo( this->sGridNo, (UINT16)DirectionInc( bNewDirection ) ), 1 );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID )
			{
				return;
			}
			else
			{
				if ( this->bTeam == gbPlayerNum )
				{
					SetUIBusy( this->ubID );
				}

				this->sTempNewGridNo = NewGridNo( this->sGridNo, (UINT16)DirectionInc( bNewDirection ) );

				this->ubPendingDirection = bNewDirection;
				this->EVENT_InitNewSoldierAnim( JUMPUPWALL, 0, FALSE );

				this->InternalReceivingSoldierCancelServices( FALSE );
				this->InternalGivingSoldierCancelServices( FALSE );

				//	this->BeginSoldierClimbWallUp(  );	
			}
		}
	}

}

void SOLDIERTYPE::BeginSoldierClimbWallUp( void )
{
	INT8							bNewDirection;
	UINT16	ubWhoIsThere;

	if ( FindLowerLevelWall( this, this->sGridNo, this->ubDirection, &bNewDirection ) && (this->pathing.bLevel > 0) )
	{
		if ( EnoughPoints( this, GetAPsToJumpWall( this, TRUE ), 0, TRUE ) )
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2( NewGridNo( this->sGridNo, (UINT16)DirectionInc( bNewDirection ) ), 0 );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID )
			{
				return;
			}
			else
			{

				if ( this->bTeam == gbPlayerNum )
				{
					SetUIBusy( this->ubID );
				}

				this->sTempNewGridNo = NewGridNo( this->sGridNo, (UINT16)DirectionInc( bNewDirection ) );

				bNewDirection = gTwoCDirection[bNewDirection];

				this->ubPendingDirection = bNewDirection;
				this->EVENT_InitNewSoldierAnim( JUMPDOWNWALL, 0, FALSE );

				this->InternalReceivingSoldierCancelServices( FALSE );
				this->InternalGivingSoldierCancelServices( FALSE );

				this->BeginSoldierClimbWall( );

			}
		}
	}

}
//------------------------------------------------------------------------------------------

UINT32 SleepDartSuccumbChance( SOLDIERTYPE * pSoldier )
{
	UINT32		uiChance;
	INT16		bEffectiveStrength;

	// figure out base chance of succumbing,
	bEffectiveStrength = EffectiveStrength( pSoldier, TRUE );

	if ( bEffectiveStrength > 90 )
	{
		uiChance = 110 - bEffectiveStrength;
	}
	else if ( bEffectiveStrength > 80 )
	{
		uiChance = 120 - bEffectiveStrength;
	}
	else if ( bEffectiveStrength > 70 )
	{
		uiChance = 130 - bEffectiveStrength;
	}
	else
	{
		uiChance = 140 - bEffectiveStrength;
	}

	// add in a bonus based on how long it's been since shot... highest chance at the beginning
	uiChance += (10 - pSoldier->bSleepDrugCounter);

	return(uiChance);
}

BOOLEAN SOLDIERTYPE::CanClimbWithCurrentBackpack()
{
	// only apply backpack climbing limitations to player mercs
	if (UsingNewInventorySystem() == true && this->inv[BPACKPOCKPOS].exists() == true && this->bTeam == OUR_TEAM
		&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)this->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[this->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
		&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || !ItemAllowsClimbing(this->inv[BPACKPOCKPOS].usItem)))
		return FALSE;

	return TRUE;
}

void SOLDIERTYPE::BeginSoldierGetup( void )
{
	// RETURN IF WE ARE BEING SERVICED
	if ( this->ubServiceCount > 0 )
	{
		return;
	}
#ifdef JA2UB
	//Ja25: No meanwhiles
#else
	// ATE: Don't getup if we are in a meanwhile
	if ( AreInMeanwhile( ) )
	{
		return;
	}
#endif
	if ( this->bCollapsed )
	{
		// anv: only get up if we're not blocked by anything (like vehicle)
		BOOLEAN fEnoughPlace = TRUE;
		STRUCTURE_FILE_REF		*pStructureFileRef;
		if ( IS_MERC_BODY_TYPE( this ) )
		{
			switch ( this->usAnimState )
			{
			case FALLOFF_FORWARD_STOP:
			case PRONE_LAYFROMHIT_STOP:
			case STAND_FALLFORWARD_STOP:
				pStructureFileRef = GetAnimationStructureRef( this->ubID, DetermineSoldierAnimationSurface( this, ANIM_CROUCH ), ANIM_CROUCH );
				break;

			case FALLBACKHIT_STOP:
			case FALLOFF_STOP:
			case FLYBACKHIT_STOP:
			case FALLBACK_HIT_STAND:
			case FALLOFF:
			case FLYBACK_HIT:
				pStructureFileRef = GetAnimationStructureRef( this->ubID, DetermineSoldierAnimationSurface( this, ROLLOVER ), ROLLOVER );
				break;

			default:
				pStructureFileRef = GetAnimationStructureRef( this->ubID, DetermineSoldierAnimationSurface( this, ANIM_CROUCH ), ANIM_CROUCH );
				break;
			}

			if ( pStructureFileRef )
				fEnoughPlace = OkayToAddStructureToWorld( this->sGridNo, this->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[this->ubDirection]]), this->ubID, FALSE, NOBODY );
		}
		// vehicles can't cower...
		else if ( !(this->flags.uiStatusFlags & SOLDIER_VEHICLE) )
		{
			pStructureFileRef = GetAnimationStructureRef( this->ubID, DetermineSoldierAnimationSurface( this, END_COWER ), END_COWER );

			if ( pStructureFileRef )
				fEnoughPlace = OkayToAddStructureToWorld( this->sGridNo, this->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[this->ubDirection]]), this->ubID, FALSE, NOBODY );
		}

		if ( this->stats.bLife >= OKLIFE && this->bBreath >= OKBREATH && (this->bSleepDrugCounter == 0) && fEnoughPlace )
		{
			// get up you hoser!

			// sevenfm: if someone is dragging this soldier, cancel drag
			SOLDIERTYPE *pSoldier;
			for (UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
			{
				pSoldier = MercPtrs[uiLoop];
				if (pSoldier && pSoldier->usDragPersonID == this->ubID)
				{
					pSoldier->CancelDrag();
				}
			}

			this->bCollapsed = FALSE;
			this->bTurnsCollapsed = 0;

			if ( IS_MERC_BODY_TYPE( this ) )
			{
				switch ( this->usAnimState )
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
					this->EVENT_InitNewSoldierAnim( ROLLOVER, 0, FALSE );
					break;

				default:

					this->ChangeSoldierStance( ANIM_CROUCH );
					break;
				}
			}
			// vehicles can't cower...
			else if ( !(this->flags.uiStatusFlags & SOLDIER_VEHICLE) )
			{
				this->EVENT_InitNewSoldierAnim( END_COWER, 0, FALSE );
			}
		}
		else
		{
			this->bTurnsCollapsed++;
			if ( (gTacticalStatus.bBoxingState == BOXING) && (this->flags.uiStatusFlags & SOLDIER_BOXER) )
			{
				if ( this->bTurnsCollapsed > 1 )
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
			DeductPoints( this, 0, (INT16)(this->bBreathMax * 100) );
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
	switch ( ubReason )
	{
	case TAKE_DAMAGE_BLOODLOSS:
	case TAKE_DAMAGE_ELECTRICITY:
	case TAKE_DAMAGE_GAS_FIRE:
	case TAKE_DAMAGE_GAS_NOTFIRE:

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

			if ( (ubReason == TAKE_DAMAGE_ELECTRICITY) && pSoldier->stats.bLife < OKLIFE )
			{
				pSoldier->flags.fInNonintAnim = FALSE;
			}

			// silversurfer: fix for the deadlock that could happen when the victim was running through a gas cloud that lead to his death.
			// If he is near death the next check will make him collapse. If he is really dead then he won't move anywhere anyway
			// so it should be safe to stop him here.
			if ( pSoldier->stats.bLife < OKLIFE && !pSoldier->bCollapsed )
			{
				pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
			}

			// Check for < OKLIFE
			if ( pSoldier->stats.bLife < OKLIFE && pSoldier->stats.bLife != 0 && !pSoldier->bCollapsed )
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

		if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN) || !pSoldier->bInSector )
		{
			if ( pSoldier->stats.bLife == 0 && !(pSoldier->flags.uiStatusFlags & SOLDIER_DEAD) )
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


UINT8 SOLDIERTYPE::SoldierTakeDamage( INT8 bHeight, INT16 sLifeDeduct, INT16 sBreathLoss, UINT8 ubReason, UINT16 ubAttacker, INT32 sSourceGrid, INT16 sSubsequent, BOOLEAN fShowDamage )
{
#ifdef JA2BETAVERSION
	if ( is_networked ) {
		CHAR tmpMPDbgString[512];
		sprintf( tmpMPDbgString, "SoldierTakeDamage ( bHeight : %i , sLifeDeduct : %i , sBreathLoss : %i , ubReason : %i , ubAttacker : %i , sSourceGrid : %i , sSubsequent : %i , fShowDamage : %i )\n", bHeight, sLifeDeduct, sBreathLoss, ubReason, ubAttacker, sSourceGrid, sSubsequent, fShowDamage );
		MPDebugMsg( tmpMPDbgString );
	}
#endif

	INT8		bOldLife;
	UINT8		ubCombinedLoss;
	INT8		bBandage;
	INT16		sAPCost;
	UINT8		ubBlood;
	UINT16		usItemFlags = 0; // Kaiden: Needed for the reveal all items after combat code from UB.

	this->ubLastDamageReason = ubReason;
		
	// Flugente: dynamic opinions
	if (ubAttacker != NOBODY && MercPtrs[ubAttacker] )
	{
		if (gGameExternalOptions.fDynamicOpinions)
		{
			AddOpinionEvent(this->ubProfile, MercPtrs[ubAttacker]->ubProfile, OPINIONEVENT_FRIENDLYFIRE);

			// if this is a civilian, other mercs can complain about mercs shooting innocents
			// Flugente: dynamic opinions: if this guy is not hostile towards us, then some mercs will complain about killing civilians
			if ((this->bTeam != OUR_TEAM) && (this->aiData.bNeutral || this->bSide == MercPtrs[ubAttacker]->bSide))
			{
				// not for killing animals though...
				if (this->ubBodyType != CROW && this->ubBodyType != COW)
					HandleDynamicOpinionChange(MercPtrs[ubAttacker], OPINIONEVENT_CIV_ATTACKER, TRUE, TRUE);
			}
		}

		// if we are a turncoat, lose the flag if we were attacked by player forces
		if ( (this->usSoldierFlagMask2 & SOLDIER_TURNCOAT) && MercPtrs[ubAttacker]->bSide == 0 )
		{
			this->usSoldierFlagMask2 &= ~SOLDIER_TURNCOAT;

			RemoveOneTurncoat( this->sSectorX, this->sSectorY, this->ubSoldierClass, FALSE );
		}
	}

	// CJC Jan 21 99: add check to see if we are hurting an enemy in an enemy-controlled
	// sector; if so, this is a sign of player activity
	switch ( this->bTeam )
	{
	case ENEMY_TEAM:
		// if we're in the wilderness this always counts
		if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( gWorldSectorX, gWorldSectorY )].fEnemyControlled || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] != TOWN )
		{
			// update current day of activity!
			UpdateLastDayOfPlayerActivity( (UINT16)GetWorldDay( ) );
		}
		break;
	case CREATURE_TEAM:
		// always a sign of activity?
		UpdateLastDayOfPlayerActivity( (UINT16)GetWorldDay( ) );
		break;
	case CIV_TEAM:
		if ( this->ubCivilianGroup == KINGPIN_CIV_GROUP && gubQuest[QUEST_RESCUE_MARIA] == QUESTINPROGRESS && gTacticalStatus.bBoxingState == NOT_BOXING )
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

	// Flugente: do we have a riot shield equipped?
	if ( this->IsRiotShieldEquipped( ) )
	{
		//  if we have equipped a riot shield and are being attacked in melee, ignore damage from some directions
		if ( ubReason == TAKE_DAMAGE_BLADE || ubReason == TAKE_DAMAGE_HANDTOHAND || ubReason == TAKE_DAMAGE_TENTACLES )
		{
			if ( ubAttacker != NOBODY && MercPtrs[ubAttacker] )
			{
				UINT8 attackdir_inverse = GetDirectionToGridNoFromGridNo( this->sGridNo, MercPtrs[ubAttacker]->sGridNo );

				// if the shield faces the direction of the attacker, we block the attack
				if ( attackdir_inverse == this->ubDirection || attackdir_inverse == gOneCCDirection[this->ubDirection] || attackdir_inverse == gOneCDirection[this->ubDirection] )
				{
					// damaging even a wooden shield is hard. For that reason we lower the initial damage.
					INT32 damage = sLifeDeduct / 3;
					INT32 breathdamage = sBreathLoss;
					DamageRiotShield( this, damage, breathdamage );

					sLifeDeduct = damage;
					sBreathLoss = breathdamage;

					PlayJA2Sample( (UINT32)(S_WOOD_IMPACT1 + Random(3)), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
				}
			}
		}
	}

	if (gTacticalStatus.uiFlags & GODMODE && this->bTeam == OUR_TEAM)
	{
		sLifeDeduct = 0;
		sBreathLoss = 0;
	}
	
	// Deduct life!, Show damage if we want!
	bOldLife = this->stats.bLife;
	
	// OK, If we are a vehicle.... damage vehicle...( people inside... )
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		if ( ubReason == TAKE_DAMAGE_GUNFIRE )
		{
			;
		}
		else if ( ubReason == TAKE_DAMAGE_EXPLOSION )
		{
			if ( ARMED_VEHICLE( this ) )
				;
			else
			{
				if ( sLifeDeduct > 50 )
				{
					// boom!
					sLifeDeduct *= 2;
				}
			}
		}

		if ( sLifeDeduct > 30 )
			this->usSoldierFlagMask2 |= SOLDIER_TAKEN_LARGE_HIT;
		
		VehicleTakeDamage( this->bVehicleID, ubReason, sLifeDeduct, this->sGridNo, ubAttacker );
		HandleTakeDamageDeath( this, bOldLife, ubReason );

		// add to our records.
		if ( ubAttacker != NOBODY && MercPtrs[ubAttacker] && MercPtrs[ubAttacker]->ubProfile != NO_PROFILE )
			gMercProfiles[MercPtrs[ubAttacker]->ubProfile].records.usDamageDealt += sLifeDeduct;

		if ( this->ubProfile != NO_PROFILE )
			gMercProfiles[this->ubProfile].records.usDamageTaken += sLifeDeduct;

		return(0);
	}

	// ATE: If we are elloit being attacked in a meanwhile...
	if ( this->flags.uiStatusFlags & SOLDIER_NPC_SHOOTING )
	{
		// Almost kill but not quite.....
		sLifeDeduct = (this->stats.bLife - 1);
		// Turn off
		this->flags.uiStatusFlags &= (~SOLDIER_NPC_SHOOTING);
	}
#ifdef JA2UB
	//Ja25: No meanwhiles
#else
	// CJC: make sure Elliot doesn't bleed to death!
	if ( ubReason == TAKE_DAMAGE_BLOODLOSS && AreInMeanwhile( ) )
	{
		return(0);
	}
#endif

	// Calculate bandage
	bBandage = this->stats.bLifeMax - this->stats.bLife - this->bBleeding;

	if ( guiCurrentScreen == MAP_SCREEN )
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
		else if ( this->flags.uiStatusFlags & SOLDIER_MONSTER )
		{
			switch ( this->ubBodyType )
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
					sReductionFactor = 4 + PythSpacesAway( MercPtrs[ubAttacker]->sGridNo, this->sGridNo ) / 2;
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
			sLifeDeduct = (sLifeDeduct + (sReductionFactor / 2)) / sReductionFactor;
		}
		else if ( ubReason == TAKE_DAMAGE_EXPLOSION )
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
			sBreathLoss = (sBreathLoss + (sReductionFactor / 2)) / sReductionFactor;
		}
	}

	if ( sLifeDeduct > this->stats.bLife )
	{
		this->stats.bLife = 0;
	}
	else
	{
		// Decrease Health
		this->stats.bLife -= sLifeDeduct;

		// make sure it doesn't rise too much
		this->stats.bLife = min( this->stats.bLife, this->stats.bLifeMax );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Doctor trait - need a variable holding the number of insta-healable hit points
	if ( (IS_MERC_BODY_TYPE( this ) || IS_CIV_BODY_TYPE( this )) && (gGameOptions.fNewTraitSystem) )
	{
		if ( this->stats.bLife <= 0 )
		{
			// noone can help him now, he's gone
			this->iHealableInjury = 0;
		}
		else
		{
			// Otherwise add healable injury value - it's in hundredths for better precision
			this->iHealableInjury += (sLifeDeduct * 100);
			// check if we are not mysteriously beyond a limit - we cannot have more than life we actually lost
			if ( this->iHealableInjury > ((this->stats.bLifeMax - this->stats.bLife) * 100) )
				this->iHealableInjury = ((this->stats.bLifeMax - this->stats.bLife) * 100);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// ATE: Put some logic in here to allow enemies to die quicker.....
	// Are we an enemy?
	if ( this->bSide != gbPlayerNum && !this->aiData.bNeutral && this->ubProfile == NO_PROFILE )
	{
		// ATE: Give them a chance to fall down...
		if ( this->stats.bLife > 0 && this->stats.bLife < (OKLIFE - 1) )
		{
			// Are we taking damage from bleeding?
			if ( ubReason == TAKE_DAMAGE_BLOODLOSS )
			{
				// Fifty-fifty chance to die now!				
				if ( Random( 3 ) == 0 || gTacticalStatus.Team[this->bTeam].bMenInSector == 1 )
				{
					// Kill!
					this->stats.bLife = 0;
				}
			}
			else
			{
				// OK, see how far we are..
				if ( this->stats.bLife < (OKLIFE - 3) )
				{
					// Kill!
					this->stats.bLife = 0;
				}
			}
		}
	}
	else if ( this->IsZombie( ) && this->stats.bLife > 0 && this->stats.bLife < OKLIFE )
	{
		// a zombie doesn't automatically die, so he would normally stand up again after being hit. 
		// We don't want that, because he is dying, so we manually skip that animation
		this->usPendingAnimation = NO_PENDING_ANIMATION;
	}

	// add to our records.
	if ( ubAttacker != NOBODY && MercPtrs[ubAttacker] && MercPtrs[ubAttacker]->ubProfile != NO_PROFILE )
		gMercProfiles[MercPtrs[ubAttacker]->ubProfile].records.usDamageDealt += sLifeDeduct;

	if ( this->ubProfile != NO_PROFILE )
		gMercProfiles[this->ubProfile].records.usDamageTaken += sLifeDeduct;

	if ( fShowDamage )
	{
		this->sDamage += sLifeDeduct;
	}

	// Truncate life
	if ( this->stats.bLife < 0 )
	{
		this->stats.bLife = 0;
	}

	// Flugente: note we received a fresh wound
	if ( sLifeDeduct > 0 )
		this->usSoldierFlagMask |= SOLDIER_FRESHWOUND;

	// Flugente we might get a disease from this...
	if ( gGameExternalOptions.fDisease && sLifeDeduct > 0 )
	{
		if ( ubAttacker != NOBODY && MercPtrs[ubAttacker] && CREATURE_OR_BLOODCAT( MercPtrs[ubAttacker] ) )
			HandlePossibleInfection( this, MercPtrs[ubAttacker], INFECTION_TYPE_WOUND_ANIMAL );

		if ( ubReason == TAKE_DAMAGE_TENTACLES )
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_WOUND_ANIMAL );

		if ( ubReason == TAKE_DAMAGE_GAS_FIRE )
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_WOUND_FIRE );
		else if ( ubReason == TAKE_DAMAGE_GAS_NOTFIRE )
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_WOUND_GAS );

		if ( ubReason == TAKE_DAMAGE_GUNFIRE && sLifeDeduct > 20 )
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_WOUND_GUNSHOT );
		else if ( ubReason == TAKE_DAMAGE_BLADE || ubReason == TAKE_DAMAGE_HANDTOHAND
			|| ubReason == TAKE_DAMAGE_EXPLOSION || ubReason == TAKE_DAMAGE_STRUCTURE_EXPLOSION || ubReason == TAKE_DAMAGE_TENTACLES )
		{
			FLOAT modifier = 0.5f + sLifeDeduct / 100;
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_WOUND_OPEN, modifier );
		}

		// possibly get traumatized if damage gets close to killing us (not if we're slowly bleeding)
		if ( this->stats.bLife < OKLIFE )//&& ubReason != TAKE_DAMAGE_BLOODLOSS )
			HandlePossibleInfection( this, NULL, INFECTION_TYPE_TRAUMATIC );
	}

	// Flugente: bandaging during retreat
	if ( gGameExternalOptions.fAllowBandagingDuringTravel && ubReason == TAKE_DAMAGE_BLOODLOSS && this->flags.fBetweenSectors && GetBestRetreatingMercDoctor( this ) != NOBODY )
	{
		SetRetreatBandaging( TRUE );
	}
	
	// Calculate damage to our items if from an explosion!
	if ( ubReason == TAKE_DAMAGE_EXPLOSION || ubReason == TAKE_DAMAGE_STRUCTURE_EXPLOSION )
	{
		CheckEquipmentForDamage( this, sLifeDeduct );
	}
	
	// Calculate bleeding
	if ( ubReason != TAKE_DAMAGE_GAS_FIRE && ubReason != TAKE_DAMAGE_GAS_NOTFIRE && !AM_A_ROBOT( this ) )
	{
		if ( ubReason == TAKE_DAMAGE_HANDTOHAND )
		{
			if ( sLifeDeduct > 0 )
			{
				// HTH does 1 pt bleeding per hit
				this->bBleeding = this->bBleeding + 1;
			}
		}
		else
		{
			// we reduce bleeding only if the new bBandage would be zero
			// by this, we can continue bleeding, and eventually bleeding
			if ( sLifeDeduct < 0 )
			{
				INT8 oldBleeding = this->bBleeding;
				this->bBleeding = min( this->bBleeding, this->stats.bLifeMax - this->stats.bLife );
			}
			else
			{
				this->bBleeding = this->stats.bLifeMax - (this->stats.bLife + bBandage);
			}
		}

	}

	//CHRISL: We need this to dynamically adjust based on maxAP.  Otherwise sLifeDeduct=16 results in a greater effective penalty
	//	the lower our AP_MAXIMUM value is set to.
	// Deduct breath AND APs!
	//sAPCost = (sLifeDeduct / APBPConstants[AP_GET_WOUNDED_DIVISOR]); // + fallCost;
	sAPCost = (sLifeDeduct / APBPConstants[AP_GET_WOUNDED_DIVISOR]) * APBPConstants[AP_MAXIMUM] / 100;

	// ATE: if the robot, do not deduct
	if ( !AM_A_ROBOT( this ) )
	{
		DeductPoints( this, sAPCost, sBreathLoss, DISABLED_INTERRUPT );
		this->ubLastAPFromHit += sAPCost;
	}

	ubCombinedLoss = (UINT8)sLifeDeduct / 10 + sBreathLoss / 2000;

	// Add shock
	if ( !AM_A_ROBOT( this ) )
	{
		this->aiData.bShock += ubCombinedLoss;
		this->ubLastShockFromHit += ubCombinedLoss;
	}

	// start the stopwatch - the blood is gushing!
	this->dNextBleed = CalcSoldierNextBleed( this );

	if ( this->bInSector && this->bVisible != -1 )
	{
		// If we are already dead, don't show damage!
		if ( bOldLife != 0 && fShowDamage && sLifeDeduct != 0 && sLifeDeduct < 1000 )
		{
			/*
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
			*/
			// sevenfm: moved code to function
			SetDamageDisplayCounter( this );
			// zero suppression values stored from last attack
			this->ubLastShock = 0;
			this->ubLastSuppression = 0;
			this->ubLastMorale = 0;
			this->ubLastAP = 0;
			//this->iLastBulletImpact = 0;
			//this->iLastArmourProtection = 0;
		}
	}
	
	// it is possible we have to drop the items in our hands
	bool dropiteminmainhand = false;

	// Flugente: disease can stop us from using our arms normally
	if ( gGameExternalOptions.fDisease
		&& gGameExternalOptions.fDiseaseSevereLimitations
		&& this->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_ARMS ) )
	{
		// drop item in main hand if twohanded
		if ( this->inv[HANDPOS].exists() == true && ItemIsTwoHanded( this->inv[HANDPOS].usItem ) )
			dropiteminmainhand = true;

		// we can only use one hand, so drop items in second hand
		if ( this->inv[SECONDHANDPOS].exists() == true )
		{
			// ATE: if our guy, make visible....
			if ( this->bTeam == gbPlayerNum )
			{
				bVisible = 1;
			}
			//if this soldier was an enemy
			// Kaiden Added for UB reveal All items after combat feature!
			else if ( this->bTeam == ENEMY_TEAM )
			{
				//add a flag to the item so when all enemies are killed, we can run through and reveal all the enemies items
				usItemFlags |= WORLD_ITEM_DROPPED_FROM_ENEMY;
			}

			if ( UsingNewAttachmentSystem() == true )
				ReduceAttachmentsOnGunForNonPlayerChars( this, &( this->inv[SECONDHANDPOS] ) );

			AddItemToPool( this->sGridNo, &( this->inv[SECONDHANDPOS] ), bVisible, this->pathing.bLevel, usItemFlags, -1 ); //Madd: added usItemFlags to function arguments
			DeleteObj( &( this->inv[SECONDHANDPOS] ) );
		}
	}

	// OK, if here, let's see if we should drop our weapon....
	if (!dropiteminmainhand && ubReason != TAKE_DAMAGE_BLOODLOSS && !(AM_A_ROBOT(this)) && !(this->bTeam == CIV_TEAM && this->ubProfile != NO_PROFILE))
	{
		INT16 sTestOne, sTestTwo, sChanceToDrop;
		INT8	bVisible = -1;

		sTestOne = EffectiveStrength( this, FALSE );
		sTestTwo = 2 * max(sLifeDeduct, (sBreathLoss / 100));
		
		if (this->ubAttackerID != NOBODY && MercPtrs[this->ubAttackerID]->ubBodyType == BLOODCAT)
		{
			// bloodcat boost, let them make people drop items more
			sTestTwo += 20;
		}

		// If damage > effective strength....
		sChanceToDrop = sTestTwo - sTestOne;

		// ATE: Increase odds of NOT dropping an UNDROPPABLE OBJECT
		if ( (this->inv[HANDPOS].fFlags & OBJECT_UNDROPPABLE) )
		{
			sChanceToDrop -= 30;
		}

#ifdef JA2TESTVERSION
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Chance To Drop Weapon: str: %d Dam: %d Chance: %d", sTestOne, sTestTwo, sChanceToDrop );
#endif

		if ((INT16)Random(100) < sChanceToDrop)
		{
			dropiteminmainhand = true;			
		}
	}

	if ( dropiteminmainhand )
	{
		// OK, drop item in main hand...
		if ( this->inv[HANDPOS].exists() == true )
		{
			// Flugente: If item has an attached rifle sling, place it the sling position instead
			int bSlot = GUNSLINGPOCKPOS;
			if ( HasAttachmentOfClass( &( this->inv[HANDPOS] ), AC_SLING ) && TryToPlaceInSlot( this, &( this->inv[HANDPOS] ), FALSE, bSlot, GUNSLINGPOCKPOS ) )
			{
				;
			}
			else if ( !( this->inv[HANDPOS].fFlags & OBJECT_UNDROPPABLE ) )
			{
				// ATE: if our guy, make visible....
				if ( this->bTeam == gbPlayerNum )
				{
					bVisible = 1;
				}
				//if this soldier was an enemy
				// Kaiden Added for UB reveal All items after combat feature!
				else if ( this->bTeam == ENEMY_TEAM )
				{
					//add a flag to the item so when all enemies are killed, we can run through and reveal all the enemies items
					usItemFlags |= WORLD_ITEM_DROPPED_FROM_ENEMY;
				}

				if ( UsingNewAttachmentSystem() == true )
					ReduceAttachmentsOnGunForNonPlayerChars( this, &( this->inv[HANDPOS] ) );

				AddItemToPool( this->sGridNo, &( this->inv[HANDPOS] ), bVisible, this->pathing.bLevel, usItemFlags, -1 ); //Madd: added usItemFlags to function arguments
				DeleteObj( &( this->inv[HANDPOS] ) );
			}
		}
	}

	// Drop some blood!
	// decide blood amt, if any
	ubBlood = (sLifeDeduct / BLOODDIVISOR);
	if ( ubBlood > MAXBLOODQUANTITY )
	{
		ubBlood = MAXBLOODQUANTITY;
	}

	if ( !(this->flags.uiStatusFlags & (SOLDIER_VEHICLE | SOLDIER_ROBOT)) )
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
	if ( this->bTeam == gbPlayerNum )
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
		if ( (MercPtrs[ubAttacker]->bTeam == gbPlayerNum) && (this->bTeam != gbPlayerNum) )
		{
			if ( ubReason == TAKE_DAMAGE_EXPLOSION )
			{
				// EXPLOSIVES GAIN (combLoss):  Causing wounds in battle
				StatChange( MercPtrs[ubAttacker], EXPLODEAMT, (UINT16)(10 * ubCombinedLoss), FROM_FAILURE );
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

	// Why this? No need for new declaration..
	//SOLDIERTYPE *pSoldier = this;
	//if (PTR_OURTEAM)
	if ( this->bTeam == gbPlayerNum )
	{
		// EXPERIENCE GAIN: Took some damage
		if ( ubReason != TAKE_DAMAGE_BLOODLOSS )
			StatChange( this, EXPERAMT, (UINT16)(5 * ubCombinedLoss), FROM_FAILURE );

		// SANDRO - gain some exp towards max health if bleeding
		if ( this->stats.bLifeMax < 100 && ubReason == TAKE_DAMAGE_BLOODLOSS && !(AM_A_ROBOT( this )) )
		{
			StatChange( this, HEALTHAMT, (UINT16)(3 * ubCombinedLoss), FROM_FAILURE );
		}

		// Check for quote
		if ( !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_BEING_PUMMELED) )
		{
			// Check attacker!
			if ( ubAttacker != NOBODY && ubAttacker != this->ubID )
			{
				this->bNumHitsThisTurn++;

				if ( (this->bNumHitsThisTurn >= 3) && (this->stats.bLife - this->bOldLife > 20) )
				{
					if ( Random( 100 ) < (UINT16)((40 * (this->bNumHitsThisTurn - 2))) )
					{
						DelayedTacticalCharacterDialogue( this, QUOTE_TAKEN_A_BREATING );
						this->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_BEING_PUMMELED;
						this->bNumHitsThisTurn = 0;
					}
				}
			}
		}
	}
	
	if ( (ubAttacker != NOBODY) && (Menptr[ubAttacker].bTeam == OUR_TEAM) && (this->ubProfile != NO_PROFILE) && gMercProfiles[this->ubProfile].Type == PROFILETYPE_RPC ||
		gMercProfiles[this->ubProfile].Type == PROFILETYPE_NPC )
	{
		gMercProfiles[this->ubProfile].ubMiscFlags |= PROFILE_MISC_FLAG_WOUNDEDBYPLAYER;
		if ( this->ubProfile == 114 )
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

#ifdef JA2UB	
	//if the attacker is MORRIS, AND he didnt kill the person
	if ( Menptr[ubAttacker].ubProfile == MORRIS_UB )	//MORRIS
	{
		//if the soldier is hurt, but not dead
		if ( this->stats.bLife < bOldLife && this->stats.bLife > 0 )
		{
			//if he hasnt said his quote #1 before
			if ( !(Menptr[ubAttacker].usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_THOUGHT_KILLED_YOU) )
			{
				//said a flag so morris can say this quote next turn
				gJa25SaveStruct.fMorrisToSayHurtPlayerQuoteNextTurn = TRUE;

				//Remeber who Morris is saying the quote too
				gJa25SaveStruct.ubPlayerMorrisHurt = this->ubProfile;
			}
		}

		// else if morris is to say the quote, he hasnt said it yet and he just killed the person he WAS going to say it to
		else if ( gJa25SaveStruct.fMorrisToSayHurtPlayerQuoteNextTurn &&
				  gJa25SaveStruct.ubPlayerMorrisHurt == this->ubProfile &&
				  this->stats.bLife <= 0 &&
				  !(Menptr[ubAttacker].usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_THOUGHT_KILLED_YOU) )
		{
			//said a flag so morris can say this quote next turn
			gJa25SaveStruct.fMorrisToSayHurtPlayerQuoteNextTurn = FALSE;

			//Remeber who Morris is saying the quote too
			gJa25SaveStruct.ubPlayerMorrisHurt = NO_PROFILE;
		}
	}
#endif
	switch ( ubReason )
	{
	case TAKE_DAMAGE_FALLROOF:
		PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_FALLROOF );
		break;
	case TAKE_DAMAGE_BLOODLOSS:
		PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_BLOODLOSS );
		break;
	case TAKE_DAMAGE_GAS_FIRE:
	case TAKE_DAMAGE_GAS_NOTFIRE:
		PossiblyStartEnemyTaunt( this, TAUNT_GOT_HIT_GAS );
		break;
	default:
		break;
	}


	return(ubCombinedLoss);
}

void SOLDIERTYPE::SoldierTakeDelayedDamage(INT8 bHeight, INT16 sLifeDeduct, INT16 sBreathLoss, UINT8 ubReason, UINT8 ubAttacker, INT32 sSourceGrid, INT16 sSubsequent, BOOLEAN fShowDamage)
{
	delayedDamageFunction = [this, bHeight, sLifeDeduct, sBreathLoss, ubReason, ubAttacker, sSourceGrid, sSubsequent, fShowDamage]()
	{
		this->SoldierTakeDamage(bHeight, sLifeDeduct, sBreathLoss, ubReason, ubAttacker, sSourceGrid, sSubsequent, fShowDamage);
	};
}

void SOLDIERTYPE::ResolveDelayedDamage()
{
	if (delayedDamageFunction)
	{
		delayedDamageFunction();
		delayedDamageFunction = nullptr;
	}
}

extern BOOLEAN IsMercSayingDialogue( UINT8 ubProfileID );

// Flugente: store how many sounds we've found for each npc type
#define BATTLESOUND_NPC_TYPES		3
#define BATTLESOUND_NPC_SOUNDSETS	8
UINT32 numBattleSounds_Npc[BATTLESOUND_NPC_TYPES][BATTLESOUND_NPC_SOUNDSETS][NUM_MERC_BATTLE_SOUNDS];
bool BattleSoundSearchDone_Npc[BATTLESOUND_NPC_TYPES][BATTLESOUND_NPC_SOUNDSETS][NUM_MERC_BATTLE_SOUNDS];

BOOLEAN SOLDIERTYPE::InternalDoMercBattleSound( UINT8 ubBattleSoundID, INT8 bSpecialCode )
{
	//in this function, pSoldier stands in for the this pointer, since
	//this soldier could be a vehicle, then the merc that makes the sound is inside
	SGPFILENAME		zFilename;
	SGPFILENAME		zFilename_Used;
	SOUNDPARMS		spParms;
	UINT8				ubSoundID = 0;
	UINT32				uiSoundID;
	UINT32				iFaceIndex;
	BOOLEAN				fDoSub = FALSE;
	INT32					uiSubSoundID = 0;
	SOLDIERTYPE*		pSoldier = this;

	// DOUBLECHECK RANGE
	CHECKF( ubBattleSoundID < NUM_MERC_BATTLE_SOUNDS );

	if ( (this->flags.uiStatusFlags & SOLDIER_VEHICLE) )
	{
		// Pick a passenger from vehicle....
		//pSoldier = PickRandomPassengerFromVehicle( this );
		// anv: as vehicles can be controlled, get a driver
		pSoldier = GetDriver( this->bVehicleID );

		if ( pSoldier == NULL )
		{
			return(FALSE);
		}
	}

	// If a death sound, and we have already done ours...
	if ( ubBattleSoundID == BATTLE_SOUND_DIE1 )
	{
		if ( pSoldier->flags.fDieSoundUsed )
		{
			return(TRUE);
		}
	}

	// Are we mute?
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_MUTE )
	{
		return(FALSE);
	}
	
	//	uiTimeSameBattleSndDone

	// If we are a creature, etc, pick a better sound...
	if ( ubBattleSoundID == BATTLE_SOUND_HIT1 )
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
			uiSubSoundID = (UINT32)(S_METAL_IMPACT1 + Random( 2 ));
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
			uiSubSoundID = (UINT32)(EXPLOSION_1);
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
		if ( guiCurrentScreen != GAME_SCREEN )
		{
			PlayJA2Sample( uiSubSoundID, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}
		else
		{
			PlayJA2Sample( uiSubSoundID, RATE_11025, SoundVolume( (UINT8)CalculateSpeechVolume( HIGHVOLUME ), pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		}

		return(TRUE);
	}

	// Check if this is the same one we just played...
	if ( pSoldier->bOldBattleSnd == ubBattleSoundID && gBattleSndsData[ubBattleSoundID].fDontAllowTwoInRow )
	{
		// Are we below the min delay?
		if ( (GetJA2Clock( ) - pSoldier->uiTimeSameBattleSndDone) < MIN_SUBSEQUENT_SNDS_DELAY )
		{
			return(TRUE);
		}
	}

	// If a battle snd is STILL playing....
	if ( SoundIsPlaying( pSoldier->uiBattleSoundID ) )
	{
		// We can do a few things here....
		// Is this a crutial one...?
		if ( gBattleSndsData[ubBattleSoundID].fStopDialogue == 1 )
		{
			// Stop playing origonal
			SoundStop( pSoldier->uiBattleSoundID );
		}
		else
		{
			// Skip this one...
			return(TRUE);
		}
	}

	// If we are talking now....
	if ( IsMercSayingDialogue( pSoldier->ubProfile ) )
	{
		// We can do a couple of things now...
		if ( gBattleSndsData[ubBattleSoundID].fStopDialogue == 1 )
		{
			// Stop dialigue...
			DialogueAdvanceSpeech( );
		}
		else if ( gBattleSndsData[ubBattleSoundID].fStopDialogue == 2 )
		{
			// Skip battle snd...
			return(TRUE);
		}
	}

	// Save this one we're doing...
	pSoldier->bOldBattleSnd = ubBattleSoundID;
	pSoldier->uiTimeSameBattleSndDone = GetJA2Clock( );
	
	//if the sound to be played is a confirmation, check to see if we are to play it
	if ( ubBattleSoundID == BATTLE_SOUND_OK1 )
	{
		if ( gGameSettings.fOptions[TOPTION_MUTE_CONFIRMATIONS] )
			return( TRUE );

		//ddd
		if ( !Chance( gGameExternalOptions.iChanceSayAnnoyingPhrase ) )
			return( TRUE );
	}

	// Adjust based on morale...
	if ( pSoldier->aiData.bMorale < LOW_MORALE_BATTLE_SND_THREASHOLD )
	{
		if ( ubBattleSoundID == BATTLE_SOUND_OK1 )
			ubBattleSoundID = BATTLE_SOUND_LOWMARALE_OK1;
		else if ( ubBattleSoundID == BATTLE_SOUND_ATTN1 )
			ubBattleSoundID = BATTLE_SOUND_LOWMARALE_ATTN1;
	}

	ubSoundID = ubBattleSoundID;
	
	// OK, build file and play!
	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		// Flugente: For the voice set itself, use this number
		UINT32 usVoiceSet = gMercProfiles[pSoldier->ubProfile].usVoiceIndex;

		// Flugente: check if perhaps a sound with a higher number is present, if so, increase number of found sounds
		// if not, mark that this search is finished (no need to constantly check for sounds)
		// This way we don't have to add new xml data, but can still use any soundfile we add (provided we numbered it correctly)
		// Soundfiles are named just like before, with increasing numbers
		// Due to legacy reasons, the first sound can either have a '1' at the end (212_OK1.xx) or no number at all (212_HUMM.xxx)
		// Otherwise we'd have to rename quite a lot of vanilla files
		while ( !QuoteExp[pSoldier->ubProfile].BattleSoundSearchDone[ubSoundID] )
		{
			// at least one sound exists (if not, we use a fallback solution anyway)
			QuoteExp[pSoldier->ubProfile].numBattleSounds[ubSoundID] = max( 1, QuoteExp[pSoldier->ubProfile].numBattleSounds[ubSoundID] );

			UINT16 numsounds = QuoteExp[pSoldier->ubProfile].numBattleSounds[ubSoundID];

			// check: is there a sound with a bigger number?
			sprintf( zFilename, "BATTLESNDS\\%03d_%s%d", usVoiceSet, gBattleSndsData[ubSoundID].zName, numsounds + 1 );

			if ( SoundFileExists( zFilename, zFilename_Used ) )
			{
				QuoteExp[pSoldier->ubProfile].numBattleSounds[ubSoundID]++;
			}
			else
			{
				QuoteExp[pSoldier->ubProfile].BattleSoundSearchDone[ubSoundID] = TRUE;
			}
		}

		UINT16 soundtoplay = 1 + Random( QuoteExp[pSoldier->ubProfile].numBattleSounds[ubSoundID] );

		if ( soundtoplay > 1 )
		{
			sprintf( zFilename, "BATTLESNDS\\%03d_%s%d", usVoiceSet, gBattleSndsData[ubSoundID].zName, soundtoplay );
		}
		else
		{
			sprintf( zFilename, "BATTLESNDS\\%03d_%s", usVoiceSet, gBattleSndsData[ubSoundID].zName );

			// due to legacy reasons, we both have to check for versions with '1' and without a number here
			if ( !SoundFileExists( zFilename, zFilename_Used ) )
				sprintf( zFilename, "BATTLESNDS\\%03d_%s%d", usVoiceSet, gBattleSndsData[ubSoundID].zName, 1 );
		}

		if ( !SoundFileExists( zFilename, zFilename_Used ) )
		{
			// OK, temp build file...
			if ( pSoldier->ubBodyType == REGFEMALE )
			{
				sprintf( zFilename, "BATTLESNDS\\f_%s", gBattleSndsData[ubSoundID].zName );
			}
			else
			{
				sprintf( zFilename, "BATTLESNDS\\m_%s", gBattleSndsData[ubSoundID].zName );
			}
		}
	}
	else
	{
		// Check if we can play this!
		if ( !gBattleSndsData[ubSoundID].fBadGuy )
		{
			return(FALSE);
		}
		
		int entrynum = 0;
		if ( pSoldier->IsZombie() ) // Madd: add zombie sounds
		{
			entrynum = 2;
			pSoldier->ubBattleSoundID = 0;		// atm only one soundset for zombies
		}
		else if ( pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV )
			entrynum = 1;

		// Flugente: check if perhaps a sound with a higher number is present, if so, increase number of found sounds
		// if not, mark that this search is finished (no need to constantly check for sounds)
		// This way we don't have to add new xml data, but can still use any soundfile we add (provided we numbered it correctly)
		// Soundfiles are named just like before, with increasing numbers
		// There are three categories for npc sound files here: bad (for ordinary humans), kid for kids and zombie
		// Due to legacy reasons, the first sound can either have a '1' at the end (212_OK1.xx) or no number at all (212_HUMM.xxx)
		// Otherwise we'd have to rename quite a lot of vanilla files
		while ( !BattleSoundSearchDone_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID] )
		{
			// at least one sound exists (if not, we use a fallback solution anyway)
			numBattleSounds_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID] = max( 1, numBattleSounds_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID] );

			UINT32 numsounds = numBattleSounds_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID];

			// check: is there a sound with a bigger number?			
			sprintf( zFilename, "BATTLESNDS\\%s%d_%s%d", gBattleSndsNpcHelperData[entrynum].zName, pSoldier->ubBattleSoundID, gBattleSndsData[ubSoundID].zName, numsounds + 1 );

			if ( SoundFileExists( zFilename, zFilename_Used ) )
			{
				numBattleSounds_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID]++;
			}
			else
			{
				BattleSoundSearchDone_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID] = true;
			}
		}

		UINT32 soundtoplay = 1 + Random( numBattleSounds_Npc[entrynum][pSoldier->ubBattleSoundID][ubSoundID] );

		sprintf( zFilename, "BATTLESNDS\\%s%d_%s%d", gBattleSndsNpcHelperData[entrynum].zName, pSoldier->ubBattleSoundID, gBattleSndsData[ubSoundID].zName, soundtoplay );

		// due to legacy reasons, we both have to check for versions with '1' and without a number here
		if ( !SoundFileExists( zFilename, zFilename_Used ) )
		{
			sprintf( zFilename, "BATTLESNDS\\%s%d_%s", gBattleSndsNpcHelperData[entrynum].zName, pSoldier->ubBattleSoundID, gBattleSndsData[ubSoundID].zName );
		}
	}

	if ( !SoundFileExists( zFilename, zFilename_Used ) )
		return FALSE;

	// Play sound!
	memset( &spParms, 0xff, sizeof(SOUNDPARMS) );

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
		if( ubBattleSoundID == BATTLE_SOUND_CURSE1 )
			spParms.uiVolume = (INT8)CalculateSpeechVolume( MIDVOLUME );
		else
			spParms.uiVolume = SoundVolume( (UINT8)spParms.uiVolume, pSoldier->sGridNo );
	}

	spParms.uiLoop = 1;
	spParms.uiPan = SoundDir( pSoldier->sGridNo );
	spParms.uiPriority = GROUP_PLAYER;

	if ( (uiSoundID = SoundPlay( zFilename_Used, &spParms )) == SOUND_ERROR )
	{
		return(FALSE);
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
	}
	
	return( TRUE );
}

BOOLEAN SOLDIERTYPE::DoMercBattleSound( UINT8 ubBattleSoundID )
{
	if ( this->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		return FALSE;

	// We WANT to play some RIGHT AWAY.....
	if ( gBattleSndsData[ubBattleSoundID].fStopDialogue == 1 || (this->ubProfile == NO_PROFILE) || InOverheadMap( ) )
	{
		return(this->InternalDoMercBattleSound( ubBattleSoundID, 0 ));
	}

	// So here, only if we were currently saying dialogue.....
	if ( !IsMercSayingDialogue( this->ubProfile ) )
	{
		return(this->InternalDoMercBattleSound( ubBattleSoundID, 0 ));
	}

	// OK, queue it up otherwise!
	TacticalCharacterDialogueWithSpecialEvent( this, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, ubBattleSoundID, 0 );

	return(TRUE);
}


BOOLEAN PreloadSoldierBattleSounds( SOLDIERTYPE *pSoldier, BOOLEAN fRemove )
{
	CHECKF( pSoldier->bActive != FALSE );

	for ( UINT32 cnt = 0; cnt < NUM_MERC_BATTLE_SOUNDS; ++cnt )
	{
		// OK, build file and play!
		if ( pSoldier->ubProfile != NO_PROFILE )
		{
			if ( gBattleSndsData[cnt].fPreload )
			{
				if ( fRemove )
				{
					SoundUnlockSample( gBattleSndsData[cnt].zName );
				}
				else
				{
					SoundLockSample( gBattleSndsData[cnt].zName );
				}
			}
		}
		else
		{
			if ( gBattleSndsData[cnt].fPreload && gBattleSndsData[cnt].fBadGuy )
			{
				if ( fRemove )
				{
					SoundUnlockSample( gBattleSndsData[cnt].zName );
				}
				else
				{
					SoundLockSample( gBattleSndsData[cnt].zName );
				}
			}
		}
	}

	return(TRUE);
}

BOOLEAN SOLDIERTYPE::CheckSoldierHitRoof( void )
{
	// Check if we are near a lower level
	INT8							bNewDirection;
	BOOLEAN						fReturnVal = FALSE;
	INT32							sNewGridNo;
	// Default to true
	BOOLEAN						fDoForwards = TRUE;

	if ( this->stats.bLife >= OKLIFE )
	{
		return(FALSE);
	}

	if ( FindLowerLevel( this, this->sGridNo, this->ubDirection, &bNewDirection ) && (this->pathing.bLevel > 0) )
	{
		// ONly if standing!
		if ( gAnimControl[this->usAnimState].ubHeight == ANIM_STAND )
		{
			// We are near a lower level.
			// Use opposite direction
			bNewDirection = gOppositeDirection[bNewDirection];

			// Alrighty, let's not blindly change here, look at whether the dest gridno is good!
			sNewGridNo = NewGridNo( this->sGridNo, DirectionInc( gOppositeDirection[bNewDirection] ) );
			if ( !NewOKDestination( this, sNewGridNo, TRUE, 0 ) )
			{
				return(FALSE);
			}
			sNewGridNo = NewGridNo( sNewGridNo, DirectionInc( gOppositeDirection[bNewDirection] ) );
			if ( !NewOKDestination( this, sNewGridNo, TRUE, 0 ) )
			{
				return(FALSE);
			}

			// Are wee near enough to fall forwards....
			if ( this->ubDirection == gOneCDirection[bNewDirection] ||
				 this->ubDirection == gTwoCDirection[bNewDirection] ||
				 this->ubDirection == bNewDirection ||
				 this->ubDirection == gOneCCDirection[bNewDirection] ||
				 this->ubDirection == gTwoCCDirection[bNewDirection] )
			{
				// Do backwards...
				fDoForwards = FALSE;
			}

			// If we are facing the opposite direction, fall backwards
			// ATE: Make this more usefull...
			if ( fDoForwards )
			{
				this->sTempNewGridNo = NewGridNo( this->sGridNo, (INT16)(-1 * DirectionInc( bNewDirection )) );
				this->sTempNewGridNo = NewGridNo( this->sTempNewGridNo, (INT16)(-1 * DirectionInc( bNewDirection )) );
				this->EVENT_SetSoldierDesiredDirection( gOppositeDirection[bNewDirection] );
				this->flags.fTurningUntilDone = TRUE;
				this->usPendingAnimation = FALLFORWARD_ROOF;
				//this->EVENT_InitNewSoldierAnim( FALLFORWARD_ROOF, 0 , FALSE );

				// Deduct hitpoints/breath for falling!
				this->SoldierTakeDamage( ANIM_CROUCH, 100, 5000, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );

				fReturnVal = TRUE;
			}
			else
			{
				this->sTempNewGridNo = NewGridNo( this->sGridNo, (INT16)(-1 * DirectionInc( bNewDirection )) );
				this->sTempNewGridNo = NewGridNo( this->sTempNewGridNo, (INT16)(-1 * DirectionInc( bNewDirection )) );
				this->EVENT_SetSoldierDesiredDirection( bNewDirection );
				this->flags.fTurningUntilDone = TRUE;
				this->usPendingAnimation = FALLOFF;

				// Deduct hitpoints/breath for falling!
				this->SoldierTakeDamage( ANIM_CROUCH, 100, 5000, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );

				fReturnVal = TRUE;
			}

			// Flugente: some bodytypes can't do these animations, so skip this
			if ( IsAnimationValidForBodyType( this, this->usPendingAnimation ) == FALSE )
				fReturnVal = FALSE;
		}
	}

	return(fReturnVal);
}

void SOLDIERTYPE::BeginSoldierClimbDownRoof(void)
{
	INT8							bNewDirection;
	UINT16	ubWhoIsThere;

	if (FindLowerLevel(this, this->sGridNo, this->ubDirection, &bNewDirection) && (this->pathing.bLevel > 0))
	{
		if (EnoughPoints(this, GetAPsToClimbRoof(this, TRUE), 0, TRUE))
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2(NewGridNo(this->sGridNo, (UINT16)DirectionInc(bNewDirection)), 0);
			if (ubWhoIsThere != NOBODY && ubWhoIsThere != this->ubID)
			{
				DebugAttackBusy(String("Soldier %d tried to climb down on someone.\n", this->ubID));
				this->aiData.bAction = AI_ACTION_NONE;
				return;
			}
			else
			{
				if (this->bTeam == gbPlayerNum)
				{
					// OK, SET INTERFACE FIRST
					SetUIBusy(this->ubID);
				}

				this->sTempNewGridNo = NewGridNo(this->sGridNo, (UINT16)DirectionInc(bNewDirection));
				bNewDirection = gTwoCDirection[bNewDirection];
				this->ubPendingDirection = bNewDirection;

				// Flugente: In case an animation is missing (zombies with bodytype of civilians), we TELEPORT instead
				if (IsAnimationValidForBodyType(this, JUMPDOWNWALL) == FALSE)
				{
					SetSoldierHeight(0.0);
					TeleportSoldier(this, this->sTempNewGridNo, TRUE);
					EndAIGuysTurn(this);
				}
				else
					this->EVENT_InitNewSoldierAnim(JUMPDOWNWALL, 0, FALSE);

				this->InternalReceivingSoldierCancelServices(FALSE);
				this->InternalGivingSoldierCancelServices(FALSE);
			}
		}
		else
		{
			DebugAttackBusy(String("Soldier %d tried to climb down without AP.\n", this->ubID));
			this->aiData.bAction = AI_ACTION_NONE;
		}
	}
	else
	{
		DebugAttackBusy(String("Soldier %d tried to climb down where no roof is.\n", this->ubID));
		this->aiData.bAction = AI_ACTION_NONE;
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

INT16 gsDragSoundNum = -1;

void SOLDIERTYPE::MoveMerc( FLOAT dMovementChange, FLOAT dAngle, BOOLEAN fCheckRange )
{
	FLOAT					dDeltaPos;
	FLOAT					dXPos, dYPos;
	BOOLEAN					fStop = FALSE;
	
	//dDegAngle = (INT16)( dAngle * 180 / PI );
	//sprintf( gDebugStr, "Move Angle: %d", (int)dDegAngle );

	// Find delta Movement for X pos
	dDeltaPos = (FLOAT)(dMovementChange * sin( dAngle ));

	// Find new position
	dXPos = this->dXPos + dDeltaPos;

	if ( fCheckRange )
	{
		fStop = FALSE;

		switch ( this->bMovementDirection )
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
	dDeltaPos = (FLOAT)(dMovementChange * cos( dAngle ));

	// Find new pos
	dYPos = this->dYPos + dDeltaPos;

	if ( fCheckRange )
	{
		fStop = FALSE;

		switch ( this->bMovementDirection )
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

	// Flugente: as we move a tile, we would now be too far away to drag someone.
	// So remember whether we were dragging (we have to set our position now, otherwise the person we drag woul soon occupy our gridno).
	BOOLEAN currentlydragging = this->IsDragging();
	INT32 sOldGridNo = this->sGridNo;

	// OK, set new position
	this->EVENT_InternalSetSoldierPosition( dXPos, dYPos, FALSE, FALSE, FALSE );
	
	this->ResolveDelayedDamage();

	// Flugente: drag people	
	if ( currentlydragging )
	{
		bool dragaborted = false;

		if ( this->usDragPersonID != NOBODY )
		{
			SOLDIERTYPE* pSoldier = MercPtrs[this->usDragPersonID];
			
			if ( pSoldier )
			{
				// while it would be neat to take the opposite direction (which would make it look like we drag the other person by the legs),
				// this causes problems, as a prone person needs additional space for the legs. So just take the same direction
				pSoldier->ubDirection = this->ubDirection;

				FLOAT dx = 0;
				FLOAT dy = 0;

				INT32 gridnotouse = pSoldier->sGridNo;
				if ( sOldGridNo != this->sGridNo )
				{
					gridnotouse = sOldGridNo;
				}
				else
				{
					INT16 this_base_x = 0;
					INT16 this_base_y = 0;
					ConvertGridNoToCenterCellXY( this->sGridNo, &this_base_x, &this_base_y );

					dx = this->dXPos - this_base_x;
					dy = this->dYPos - this_base_y;
				}

				INT16 base_x = 0;
				INT16 base_y = 0;
				ConvertGridNoToCenterCellXY( gridnotouse, &base_x, &base_y );

				pSoldier->EVENT_InternalSetSoldierPosition( base_x + dx, base_y + dy, FALSE, FALSE, FALSE );
			}
			else
			{
				dragaborted = true;
			}
		}
		else if ( this->sDragCorpseID >= 0 )
		{
			ROTTING_CORPSE* pCorpse = GetRottingCorpse( this->sDragCorpseID );

			if ( pCorpse )
			{
				// move all enemy-dropped items along with the corpse, to make it look as if the items are still 'on' the body
				if ( sOldGridNo != this->sGridNo )
					MoveItemPools_ForDragging( pCorpse->def.sGridNo, sOldGridNo, this->pathing.bLevel, this->pathing.bLevel );

				// move corpse to new location. We have to actually delete and recreate the corpse, otherwise direction changes will only be visible after saving the game
				ROTTING_CORPSE_DEFINITION CorpseDef;

				// Copy corpse definition...
				memcpy(&CorpseDef, &(pCorpse->def), sizeof(ROTTING_CORPSE_DEFINITION));

				// Remove old one...
				RemoveCorpse(pCorpse->iID);

				// drop blood at old location
				InternalDropBlood(pCorpse->def.sGridNo, this->pathing.bLevel, 0, 5, 1);

				// adjust both gridno and x,y coordinates
				if (sOldGridNo != this->sGridNo)
				{
					INT16 sX, sY;
					ConvertGridNoToCenterCellXY(sOldGridNo, &sX, &sY);

					CorpseDef.sGridNo = sOldGridNo;
					CorpseDef.dXPos = sX;
					CorpseDef.dYPos	= sY;
				}
				else
				{
					// move corpse a bit
					INT16 this_base_x = 0;
					INT16 this_base_y = 0;
					ConvertGridNoToCenterCellXY(this->sGridNo, &this_base_x, &this_base_y);

					FLOAT dx = this->dXPos - this_base_x;
					FLOAT dy = this->dYPos - this_base_y;
						
					INT16 base_x = 0;
					INT16 base_y = 0;
					ConvertGridNoToCenterCellXY(pCorpse->def.sGridNo, &base_x, &base_y);

					INT16 sX, sY;
					ConvertGridNoToCenterCellXY(pCorpse->def.sGridNo, &sX, &sY);

					CorpseDef.sGridNo	= pCorpse->def.sGridNo;
					CorpseDef.dXPos		= sX + dx;
					CorpseDef.dYPos		= sY + dy;
				}

				CorpseDef.usFlags		|= ROTTING_CORPSE_USE_XY_PROVIDED;

				CorpseDef.ubDirection	= this->ubDirection;

				this->sDragCorpseID = AddRottingCorpse(&CorpseDef);
			}
			else
			{
				dragaborted = true;
			}
		}
		else if ( sOldGridNo != this->sGridNo && this->sDragGridNo != NOWHERE )
		{
			bool success = false;
			UINT32 arusTileType;
			UINT16 arusStructureNumber;
			UINT8 hitpoints;
			UINT8 decalflag;
			
			if ( IsDragStructurePresent( this->sDragGridNo, this->pathing.bLevel, arusTileType, arusStructureNumber, hitpoints, decalflag ) )
			{
				// add
				if ( BuildStructDrag( sOldGridNo, gsInterfaceLevel, arusTileType, arusStructureNumber, this->ubID ) )
				{
					// as structures might be damaged/have decals, make sure to keep the old values
					CorrectDragStructData( sOldGridNo, (INT8)gsInterfaceLevel, hitpoints, decalflag );

					// remove
					RemoveStructDrag( this->sDragGridNo, (INT8)gsInterfaceLevel, arusTileType );

					// also move doors, this includes moving locks and traps
					DOOR* pDoor = FindDoorInfoAtGridNo( this->sDragGridNo );
					if ( pDoor )
						pDoor->sGridNo = sOldGridNo;

					success = true;
				}
			}

			if ( success )
			{
				// move all items in/on the structure along
				MoveItemPools_ForDragging( this->sDragGridNo, sOldGridNo, this->pathing.bLevel, this->pathing.bLevel );

				this->sDragGridNo = sOldGridNo;
			}
			else
			{
				this->CancelDrag();

				dragaborted = true;
			}
		}

		if ( !dragaborted )
		{
			// sevenfm: play sound while dragging
			if ( !( this->usSoldierFlagMask2 & SOLDIER_DRAG_SOUND ) )
			{
				SGPFILENAME		zFilename_Used;
				CHAR8	zFilename[512];
				// prepare drag sound
				if ( gsDragSoundNum < 0 )
				{
					gsDragSoundNum = 0;
					do
					{
						gsDragSoundNum++;
						sprintf( zFilename, "Sounds\\Misc\\DragBody%d", gsDragSoundNum );
					} while ( SoundFileExists( zFilename, zFilename_Used ) );
					gsDragSoundNum--;
				}

				if ( gsDragSoundNum > 0 )
				{
					sprintf( zFilename, "Sounds\\Misc\\DragBody%d", Random( gsDragSoundNum ) + 1 );
					if ( SoundFileExists( zFilename, zFilename_Used ) )
					{
						PlayJA2SampleFromFile( zFilename_Used, RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
					}

					this->usSoldierFlagMask2 |= SOLDIER_DRAG_SOUND;
				}
			}
		}
		else
		{
			this->CancelDrag();
		}
	}
	
	//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("X: %f Y: %f", dXPos, dYPos ) );
}

BOOLEAN GetDirectionChangeAmount( INT32 sGridNo, SOLDIERTYPE *pSoldier, UINT8 uiTurnAmount )
{
	//CHRISL: This function should return TRUE if the difference between our current facing and the facing needed to put
	//	the indicated sGrinNo into our facing is greater then uiTurnAmount
	UINT8	ubDirection = GetDirectionFromGridNo( sGridNo, pSoldier );
	UINT8	subDirection = pSoldier->ubDirection + 3;
	UINT8	uiDirArray[16] = {5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4};

	//Failsafe; just check to make sure we actually have to turn.
	if ( ubDirection == pSoldier->ubDirection )
		return FALSE;

	// We'll never turn more then 180 degrees (4) so reset uiTurnAmount if needed
	uiTurnAmount = min( uiTurnAmount, 4 );

	// Loop up the array
	for ( UINT8 i = 1; i <= 4; i++ )
	{
		if ( uiDirArray[subDirection + i] == ubDirection )
		{
			return !(i <= uiTurnAmount);
		}
	}
	// Loop down the array
	for ( UINT8 i = 1; i <= 4; i++ )
	{
		if ( uiDirArray[subDirection - i] == ubDirection )
		{
			return !(i <= uiTurnAmount);
		}
	}

	return TRUE;
}

UINT8 GetDirectionFromGridNo( INT32 sGridNo, SOLDIERTYPE *pSoldier )
{
	INT16 sXPos, sYPos;

	ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );

	return(GetDirectionFromXY( sXPos, sYPos, pSoldier ));
}

INT16 GetDirectionToGridNoFromGridNo( INT32 sGridNoDest, INT32 sGridNoSrc )
{
	INT16 sXPos2, sYPos2;
	INT16 sXPos, sYPos;

	ConvertGridNoToXY( sGridNoSrc, &sXPos, &sYPos );
	ConvertGridNoToXY( sGridNoDest, &sXPos2, &sYPos2 );

	return(atan8( sXPos2, sYPos2, sXPos, sYPos ));

}

UINT8 GetDirectionFromXY( INT16 sXPos, INT16 sYPos, SOLDIERTYPE *pSoldier )
{
	INT16 sXPos2, sYPos2;

	ConvertGridNoToXY( pSoldier->sGridNo, &sXPos2, &sYPos2 );

	return(atan8( sXPos2, sYPos2, sXPos, sYPos ));
}

INT16 GetDirectionFromCenterCellXYGridNo(INT32 EndGridNo, INT32 StartGridNo)
{
	INT16 sXPos2, sYPos2;
	INT16 sXPos, sYPos;

	ConvertGridNoToCenterCellXY(StartGridNo, &sXPos, &sYPos);
	ConvertGridNoToCenterCellXY(EndGridNo, &sXPos2, &sYPos2);

	return(atan8(sXPos2, sYPos2, sXPos, sYPos));
}


//#if 0
UINT8 atan8( INT16 sXPos, INT16 sYPos, INT16 sXPos2, INT16 sYPos2 )
{
	DOUBLE  test_x = sXPos2 - sXPos;
	DOUBLE  test_y = sYPos2 - sYPos;
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
		if ( angle >= -PI*.375 && angle <= -PI*.125 )
		{
			mFacing = SOUTHWEST;
			break;
		}

		if ( angle <= PI*.375 && angle >= PI*.125 )
		{
			mFacing = SOUTHEAST;
			break;
		}

		if ( angle >= PI*.623 && angle <= PI*.875 )
		{
			mFacing = NORTHEAST;
			break;
		}

		if ( angle <= -PI*.623 && angle >= -PI*.875 )
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
		if ( (angle > PI*0.875 && angle <= PI) || (angle > -PI && angle < -PI*0.875) )
		{
			mFacing = NORTH;
		}
		if ( angle > -PI*0.623 && angle < -PI*0.375 )
		{
			mFacing = WEST;
		}

	} while ( FALSE );

	return(mFacing);
}


UINT8 atan8FromAngle( DOUBLE angle )
{
	UINT8	  mFacing = WEST;

	if ( angle > PI )
	{
		angle = (angle - PI) - PI;
	}
	if ( angle < -PI )
	{
		angle = (PI - (fabs( angle ) - PI));
	}

	do
	{
		if ( angle >= -PI*.375 && angle <= -PI*.125 )
		{
			mFacing = SOUTHWEST;
			break;
		}

		if ( angle <= PI*.375 && angle >= PI*.125 )
		{
			mFacing = SOUTHEAST;
			break;
		}

		if ( angle >= PI*.623 && angle <= PI*.875 )
		{
			mFacing = NORTHEAST;
			break;
		}

		if ( angle <= -PI*.623 && angle >= -PI*.875 )
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
		if ( (angle > PI*0.875 && angle <= PI) || (angle > -PI && angle < -PI*0.875) )
		{
			mFacing = NORTH;
		}
		if ( angle > -PI*0.623 && angle < -PI*0.375 )
		{
			mFacing = WEST;
		}

	} while ( FALSE );

	return(mFacing);
}


void CheckForFullStructures( SOLDIERTYPE *pSoldier )
{
	// This function checks to see if we are near a specific structure type which requires us to blit a
	// small obscuring peice
	INT32 sGridNo;
	UINT16 usFullTileIndex;
	INT32		cnt;


	// Check in all 'Above' directions
	for ( cnt = 0; cnt < MAX_FULLTILE_DIRECTIONS; cnt++ )
	{
		sGridNo = pSoldier->sGridNo + gsFullTileDirections[cnt];

		if ( CheckForFullStruct( sGridNo, &usFullTileIndex ) )
		{
			// Add one for the item's obsuring part
			pSoldier->usFrontArcFullTileList[cnt] = usFullTileIndex + 1;
			pSoldier->usFrontArcFullTileGridNos[cnt] = sGridNo;
			AddTopmostToHead( sGridNo, pSoldier->usFrontArcFullTileList[cnt] );
		}
		else
		{
			if ( pSoldier->usFrontArcFullTileList[cnt] != 0 )
			{
				RemoveTopmost( pSoldier->usFrontArcFullTileGridNos[cnt], pSoldier->usFrontArcFullTileList[cnt] );
			}
			pSoldier->usFrontArcFullTileList[cnt] = 0;
			pSoldier->usFrontArcFullTileGridNos[cnt] = 0;
		}
	}

}


BOOLEAN CheckForFullStruct( INT32 sGridNo, UINT16 *pusIndex )
{
	LEVELNODE	*pStruct = NULL;
	LEVELNODE	*pOldStruct = NULL;
	UINT32				fTileFlags;

	pStruct = gpWorldLevelData[sGridNo].pStructHead;

	// Look through all structs and Search for type

	while ( pStruct != NULL )
	{

		if ( pStruct->usIndex != NO_TILE && pStruct->usIndex < giNumberOfTiles )
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
					return(TRUE);
				}
				else
				{
					return(FALSE);
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
	return(FALSE);

}


BOOLEAN FullStructAlone( INT32 sGridNo, UINT8 ubRadius )
{
	INT32  sTop, sBottom;
	INT32  sLeft, sRight;
	INT32  cnt1, cnt2;
	INT32	 iNewIndex;
	INT32	 leftmost;


	// Determine start end end indicies and num rows
	sTop = ubRadius;
	sBottom = -ubRadius;
	sLeft = -ubRadius;
	sRight = ubRadius;

	for ( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{

		leftmost = ((sGridNo + (WORLD_COLS * cnt1)) / WORLD_COLS) * WORLD_COLS;

		for ( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = sGridNo + (WORLD_COLS * cnt1) + cnt2;


			if ( iNewIndex >= 0 && iNewIndex < WORLD_MAX &&
				 iNewIndex >= leftmost && iNewIndex < (leftmost + WORLD_COLS) )
			{
				if ( iNewIndex != sGridNo )
				{
					if ( FindStructure( iNewIndex, STRUCTURE_TREE ) != NULL )
					{
						return(FALSE);
					}
				}
			}

		}
	}

	return(TRUE);
}


void AdjustForFastTurnAnimation( SOLDIERTYPE *pSoldier )
{

	// CHECK FOR FASTTURN ANIMATIONS
	// ATE: Mod: Only fastturn for OUR guys!
	if ( gAnimControl[pSoldier->usAnimState].uiFlags & ANIM_FASTTURN && pSoldier->bTeam == gbPlayerNum && !(pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT) )
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
	if ( gAnimControl[this->usAnimState].uiFlags & ANIM_NONINTERRUPT )
	{
		return(FALSE);
	}
	return(TRUE);
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

void SendSoldierDestinationEvent( SOLDIERTYPE *pSoldier, UINT32 usNewDestination )
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
	if ( is_server || (is_client && pSoldier->ubID <20) ) send_dir( pSoldier, usDesiredDirection );
}

void SendGetNewSoldierPathEvent( SOLDIERTYPE *pSoldier, INT32 sDestGridNo, UINT16 usMovementAnim )
{
	EV_S_GETNEWPATH	SGetNewPath;

	SGetNewPath.usSoldierID = pSoldier->ubID;
	SGetNewPath.sDestGridNo = sDestGridNo;
	SGetNewPath.usMovementAnim = usMovementAnim;
	SGetNewPath.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_GETNEWPATH, 0, &SGetNewPath );
}


void SendChangeSoldierStanceEvent( SOLDIERTYPE *pSoldier, UINT8 ubNewStance )
{

	if ( ((pSoldier->ubID > 19 && !is_server) || (pSoldier->ubID > 119 && is_server)) && is_networked )return;

	pSoldier->ChangeSoldierStance( ubNewStance );
	if ( is_server || (is_client && pSoldier->ubID <20) ) send_stance( pSoldier, ubNewStance );
}


void SendBeginFireWeaponEvent( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "SendBeginFireWeaponEvent" ) );
	EV_S_BEGINFIREWEAPON		SBeginFireWeapon;

	SBeginFireWeapon.usSoldierID = pSoldier->ubID;
	SBeginFireWeapon.sTargetGridNo = sTargetGridNo;
	SBeginFireWeapon.bTargetLevel = pSoldier->bTargetLevel;
	SBeginFireWeapon.bTargetCubeLevel = pSoldier->bTargetCubeLevel;
	SBeginFireWeapon.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;

	AddGameEvent( S_BEGINFIREWEAPON, 0, &SBeginFireWeapon );
}


BOOLEAN SOLDIERTYPE::MercInWater( void )
{
	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps

	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_WATER( this->bOverTerrainType ) && this->pathing.bLevel <= 0 )
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

BOOLEAN SOLDIERTYPE::MercInShallowWater( void )
{
	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps

	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_SHALLOW_WATER( this->bOverTerrainType ) && this->pathing.bLevel <= 0 )
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}


BOOLEAN SOLDIERTYPE::MercInDeepWater( void )
{
	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps

	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_DEEP_WATER( this->bOverTerrainType ) && this->pathing.bLevel <= 0 )
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

BOOLEAN SOLDIERTYPE::MercInHighWater( void )
{
	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps

	// Our water texture , for now is of a given type
	if ( TERRAIN_IS_HIGH_WATER( this->bOverTerrainType ) && this->pathing.bLevel <= 0 )
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

void RevivePlayerTeam( )
{
	INT32 cnt;
	SOLDIERTYPE		*pSoldier;

	// End the turn of player charactors
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pSoldier++ )
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
		this->flags.uiStatusFlags &= (~SOLDIER_DEAD);

		this->stats.bLife = this->stats.bLifeMax;
		this->bBleeding = 0;
		this->iHealableInjury = 0; // added by SANDRO
		this->ubDesiredHeight = ANIM_STAND;

		AddManToTeam( this->bTeam );

		// Set to standing
		this->flags.fInNonintAnim = FALSE;
		this->flags.fRTInNonintAnim = FALSE;

		// Change to standing,unless we can getup with an animation
		this->EVENT_InitNewSoldierAnim( STANDING, 0, TRUE );
		this->BeginSoldierGetup( );

		// Makesure center of tile
		ConvertGridNoToCenterCellXY(this->sGridNo, &sX, &sY);

		this->EVENT_SetSoldierPosition( (FLOAT)sX, (FLOAT)sY );

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
	INT32 sGridNo;
	UINT16						usAnimSurface;

	// ATE

	// Get Surface Index
	usAnimSurface = DetermineSoldierAnimationSurface( this, usAnimState );

	CHECKV( usAnimSurface != INVALID_ANIMATION_SURFACE );

	bProfileID = gAnimSurfaceDatabase[usAnimSurface].bProfile;

	// Determine if this animation has a profile
	if ( bProfileID != -1 )
	{
		// Getprofile
		pProfile = &(gpAnimProfiles[bProfileID]);

		// Get direction
		pProfileDir = &(pProfile->Dirs[this->ubDirection]);

		// Loop tiles and set accordingly into world
		for ( iTileCount = 0; iTileCount < pProfileDir->ubNumTiles; iTileCount++ )
		{
			pProfileTile = &(pProfileDir->pTiles[iTileCount]);

			sGridNo = this->sGridNo + ((WORLD_COLS * pProfileTile->bTileY) + pProfileTile->bTileX);

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


LEVELNODE *GetAnimProfileFlags( INT32 sGridNo, UINT16 *usFlags, SOLDIERTYPE **ppTargSoldier, LEVELNODE *pGivenNode )
{
	LEVELNODE				*pNode;

	(*ppTargSoldier) = NULL;
	(*usFlags) = 0;

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

	return(pNode);
}


BOOLEAN SOLDIERTYPE::GetProfileFlagsFromGridno( UINT16 usAnimState, INT32 sTestGridNo, UINT16 *usFlags )
{
	ANIM_PROF					*pProfile;
	ANIM_PROF_DIR			*pProfileDir;
	ANIM_PROF_TILE		*pProfileTile;
	INT8							bProfileID;
	UINT32						iTileCount;
	INT32 sGridNo;
	UINT16						usAnimSurface;

	// Get Surface Index
	usAnimSurface = DetermineSoldierAnimationSurface( this, usAnimState );

	CHECKF( usAnimSurface != INVALID_ANIMATION_SURFACE );

	bProfileID = gAnimSurfaceDatabase[usAnimSurface].bProfile;

	*usFlags = 0;

	// Determine if this animation has a profile
	if ( bProfileID != -1 )
	{
		// Getprofile
		pProfile = &(gpAnimProfiles[bProfileID]);

		// Get direction
		pProfileDir = &(pProfile->Dirs[this->ubDirection]);

		// Loop tiles and set accordingly into world
		for ( iTileCount = 0; iTileCount < pProfileDir->ubNumTiles; iTileCount++ )
		{
			pProfileTile = &(pProfileDir->pTiles[iTileCount]);

			sGridNo = this->sGridNo + ((WORLD_COLS * pProfileTile->bTileY) + pProfileTile->bTileX);

			// Check if in bounds
			if ( !OutOfBounds( this->sGridNo, sGridNo ) )
			{
				if ( sGridNo == sTestGridNo )
				{
					*usFlags = pProfileTile->usTileFlags;
					return(TRUE);
				}
			}

		}
	}

	return(FALSE);
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
		this->EVENT_InitNewSoldierAnim( GIVE_ITEM, 0, FALSE );
	}
	else
	{
		UnSetEngagedInConvFromPCAction( this );

		OBJECTTYPE::DeleteMe( &this->pTempObject );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginBladeAttack( INT32 sGridNo, UINT8 ubDirection )
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
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Begin blade attack: ATB  %d", gTacticalStatus.ubAttackBusyCount ) );
	DebugAttackBusy( String( "Begin blade attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount ) );
	//}

	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	//dnl ch73 290913
	if (this->ubDirection != ubDirection)
	{
		if (this->usAnimState != CRAWLING && gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE)
			usForceAnimState = CROUCHING;
		this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
		this->EVENT_SetSoldierDesiredDirection(ubDirection);
		this->EVENT_SetSoldierDirection(ubDirection);
		if (this->flags.bTurningFromPronePosition)
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_FOR_PUNCH_OR_STAB;
		usForceAnimState = INVALID_ANIMATION;
	}
	// CHANGE TO ANIMATION

	// sevenfm: initialize attack data
	this->sTargetGridNo = sGridNo;
	this->bTargetLevel = this->pathing.bLevel;	// melee attacks don't work between levels
	this->ubTargetID = WhoIsThere2(sGridNo, this->bTargetLevel);

	// DETERMINE ANIMATION TO PLAY
	// LATER BASED ON IF TAREGT KNOWS OF US, STANCE, ETC
	// GET POINTER TO TAREGT
	if ( this->flags.uiStatusFlags & SOLDIER_MONSTER )
	{
		// Is there an unconscious guy at gridno......
		UINT16 ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );

		if ( ubTargetID != NOBODY && ((MercPtrs[ubTargetID]->stats.bLife < OKLIFE && MercPtrs[ubTargetID]->stats.bLife > 0) || (MercPtrs[ubTargetID]->bBreath < OKBREATH && MercPtrs[ubTargetID]->bCollapsed)) )
		{
			this->aiData.uiPendingActionData4 = ubTargetID;

			// add regen bonus
			this->newdrugs.size[DRUG_EFFECT_HP] += 10;

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
	else if ( this->ubBodyType == BLOODCAT )
	{
		// Check if it's a claws or teeth...
		if ( this->inv[HANDPOS].usItem == BLOODCAT_CLAW_ATTACK )
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

			// sevenfm: set flag indicating back attack and sneak attack
			if (pTSoldier)
			{
				pTSoldier->usSoldierFlagMask2 &= ~SOLDIER_BACK_ATTACK;
				pTSoldier->usSoldierFlagMask2 &= ~SOLDIER_SNEAK_ATTACK;

				UINT8 ubAttackDirection = AIDirection(this->sGridNo, pTSoldier->sGridNo);
				if (ubAttackDirection == pTSoldier->ubDirection ||
					ubAttackDirection == gOneCDirection[pTSoldier->ubDirection] ||
					ubAttackDirection == gOneCCDirection[pTSoldier->ubDirection])
				{
					pTSoldier->usSoldierFlagMask2 |= SOLDIER_BACK_ATTACK;
				}

				if (pTSoldier->aiData.bOppList[this->ubID] != SEEN_CURRENTLY &&
					pTSoldier->aiData.bOppList[this->ubID] != SEEN_THIS_TURN &&
					pTSoldier->aiData.bOppList[this->ubID] != HEARD_THIS_TURN)
				{
					pTSoldier->usSoldierFlagMask2 |= SOLDIER_SNEAK_ATTACK;
				}
			}

			// Flugente: if we attack with a bayonet, we don't need to change stance if even if we are standing and the target is prone...
			// so we simulate here that the target is still standing
			UINT8 targetheight = gAnimControl[pTSoldier->usAnimState].ubEndHeight;

			// Look at stance of target
			switch ( targetheight )
			{
			case ANIM_STAND:
			case ANIM_CROUCH:
							
				// WE ARE SEEN
				if ( this->bWeaponMode == WM_ATTACHED_BAYONET && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND)
				{
					// if this attack happens directly after running, the attack is slightly more powerful due to extra force
					if (this->usAnimState == RUNNING || this->usAnimState == RUNNING_W_PISTOL)
						this->usSoldierFlagMask2 |= SOLDIER_BAYONET_RUNBONUS;

					this->EVENT_InitNewSoldierAnim(BAYONET_STAB_STANDING_VS_STANDING, 0, FALSE);
				}
				// SANDRO - use focused stab animation on aimed blade attacks
				else if (gGameExternalOptions.fEnhancedCloseCombatSystem && (this->aiData.bAimTime > 0))
				{
					this->EVENT_InitNewSoldierAnim(FOCUSED_STAB, 0, FALSE);
				}
				else if ( Chance( 50 ) )
				{
					this->EVENT_InitNewSoldierAnim( STAB, 0, FALSE );
				}
				else
				{
					this->EVENT_InitNewSoldierAnim( SLICE, 0, FALSE );
				}

				if (pTSoldier->aiData.bOppList[this->ubID] != 0 || pTSoldier->bTeam == this->bTeam)
				{
					// IF WE ARE SEEN, MAKE SURE GUY TURNS!
					// Get direction to target
					// IF WE ARE AN ANIMAL, CAR, MONSTER, DONT'T TURN
					if ( !(pTSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE)) )
					{
						// OK, stop merc....
						pTSoldier->EVENT_StopMerc( pTSoldier->sGridNo, pTSoldier->ubDirection );

						if ( pTSoldier->bTeam != gbPlayerNum )
						{
							DebugAI(AI_MSG_INFO, pTSoldier, String("CancelAIAction: begin blade attack"));
							CancelAIAction( pTSoldier, TRUE );
						}

						ubTDirection = (UINT8)GetDirectionFromGridNo( this->sGridNo, pTSoldier );
						SendSoldierSetDesiredDirectionEvent( pTSoldier, ubTDirection );
					}
				}

				break;

			case ANIM_PRONE:
				
				if (this->bWeaponMode == WM_ATTACHED_BAYONET && gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND)
				{
					// if this attack happens directly after running, the attack is slightly more powerful due to extra force
					if (this->usAnimState == RUNNING || this->usAnimState == RUNNING_W_PISTOL)
						this->usSoldierFlagMask2 |= SOLDIER_BAYONET_RUNBONUS;

					this->EVENT_InitNewSoldierAnim(BAYONET_STAB_STANDING_VS_PRONE, 0, FALSE);
				}
				// CHECK OUR STANCE
				else if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH )
				{
					// SET DESIRED STANCE AND SET PENDING ANIMATION
					SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
					this->usPendingAnimation = CROUCH_STAB;
				}
				else
				{
					// USE crouched one
					// NEED TO CHANGE STANCE IF NOT CROUCHD!
					this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0, FALSE );
				}
				break;
			}
		}
		else
		{
			// OK, SEE IF THERE IS AN OBSTACLE HERE...
			if ( !NewOKDestination( this, sGridNo, FALSE, this->pathing.bLevel ) )
			{
				this->EVENT_InitNewSoldierAnim( STAB, 0, FALSE );
			}
			else
			{
				// Check for corpse!
				pCorpse = GetCorpseAtGridNo( sGridNo, this->pathing.bLevel );

				if ( pCorpse == NULL )
				{
					this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0, FALSE );
				}
				else
				{
					// Flugente: decapitation action now also performs gutting, stripping and taking corpses
					if ( IsValidDecapitationCorpse( pCorpse ) || IsValidGutCorpse( pCorpse ) || IsValidStripCorpse( pCorpse ) || IsValidTakeCorpse( pCorpse ) )
					{
						this->EVENT_InitNewSoldierAnim( DECAPITATE, 0, FALSE );
					}
					else
					{
						this->EVENT_InitNewSoldierAnim( CROUCH_STAB, 0, FALSE );
					}
				}
			}
		}
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginPunchAttack( INT32 sGridNo, UINT8 ubDirection )
{
	BOOLEAN			fMartialArtist = FALSE;
	SOLDIERTYPE *pTSoldier;
	//UINT32 uiMercFlags;
	UINT16 usSoldierIndex;
	UINT8 ubTDirection;
	UINT16	usItem;

	// Get item in hand...
	usItem = this->inv[HANDPOS].usItem;


	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
	//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Begin HTH attack: ATB  %d", gTacticalStatus.ubAttackBusyCount ) );
	DebugAttackBusy( String( "Begin HTH attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount ) );

	//}

	// sevenfm: use supplied gridno as sTargetGridNo may not be set yet
	//usSoldierIndex = WhoIsThere2( this->sTargetGridNo, this->pathing.bLevel );
	usSoldierIndex = WhoIsThere2(sGridNo, this->pathing.bLevel);

	if (usSoldierIndex == NOBODY)
	{
		return;
	}
	
	GetSoldier( &pTSoldier, usSoldierIndex );

	// sevenfm: set flag indicating back attack and sneak attack
	if (pTSoldier)
	{
		pTSoldier->usSoldierFlagMask2 &= ~SOLDIER_BACK_ATTACK;
		pTSoldier->usSoldierFlagMask2 &= ~SOLDIER_SNEAK_ATTACK;

		UINT8 ubAttackDirection = AIDirection(this->sGridNo, pTSoldier->sGridNo);
		if (ubAttackDirection == pTSoldier->ubDirection ||
			ubAttackDirection == gOneCDirection[pTSoldier->ubDirection] ||
			ubAttackDirection == gOneCCDirection[pTSoldier->ubDirection])
		{
			pTSoldier->usSoldierFlagMask2 |= SOLDIER_BACK_ATTACK;
		}

		if (pTSoldier->aiData.bOppList[this->ubID] != SEEN_CURRENTLY &&
			pTSoldier->aiData.bOppList[this->ubID] != SEEN_THIS_TURN &&
			pTSoldier->aiData.bOppList[this->ubID] != HEARD_THIS_TURN)
		{
			pTSoldier->usSoldierFlagMask2 |= SOLDIER_SNEAK_ATTACK;
		}
	}

	if (this->ubDirection != ubDirection)
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		//dnl ch73 290913
		if ( this->usAnimState != CRAWLING && gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
			usForceAnimState = CROUCHING;
		this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
		this->EVENT_SetSoldierDesiredDirection(ubDirection);
		this->EVENT_SetSoldierDirection(ubDirection);
		if ( this->flags.bTurningFromPronePosition )
			this->flags.bTurningFromPronePosition = TURNING_FROM_PRONE_FOR_PUNCH_OR_STAB;
		usForceAnimState = INVALID_ANIMATION;
	}

	// Are we a martial artist? - SANDRO - added new/old traits check
	if ( ((NUM_SKILL_TRAITS( this, MARTIAL_ARTS_NT ) >= ((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA) ? 2 : 1)) && gGameOptions.fNewTraitSystem) ||
		 (HAS_SKILL_TRAIT( this, MARTIALARTS_OT ) && !gGameOptions.fNewTraitSystem) )
	{
		fMartialArtist = TRUE;
	}

	//Ja25 No meanwhiles
#ifdef JA2UB
	if ( fMartialArtist && !ItemIsCrowbar(usItem) && this->ubBodyType == REGMALE )
#else
	if ( fMartialArtist && !AreInMeanwhile( ) && !ItemIsCrowbar(usItem) && this->ubBodyType == REGMALE && !IsZombie( )
		&& !( gGameExternalOptions.fDiseaseSevereLimitations && this->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ))  ) // SANDRO - added check for body type
#endif
	{
		// Are we in attack mode yet?
		if ( this->usAnimState != NINJA_BREATH && gAnimControl[this->usAnimState].ubHeight == ANIM_STAND && gAnimControl[pTSoldier->usAnimState].ubHeight != ANIM_PRONE )
		{
			this->EVENT_InitNewSoldierAnim( NINJA_GOTOBREATH, 0, FALSE );
		}
		else
		{
			this->DoNinjaAttack( );
		}
	}
	else
	{
		// Flugente: civilians can be zombies too, but they do not have a 'punch' animation. Simple fix: They 'attack' without animation...
		// CHECK IF WE CAN DO THIS ANIMATION!
		if ( this->IsZombie( ) && IsAnimationValidForBodyType( this, PUNCH ) == FALSE )
		{
			if ( gGameExternalOptions.fZombieExplodingCivs )
			{
				// Solution 1: zombie explodes, killing himself and eventually also killing his target
				// Lower hitpoints, to ensure we will die in explosion (otherwise there might be multiple explosions or zombies not dying correctly...)
				if ( stats.bLife > 2 )
					stats.bLife = 2;
				// Explosion of a Jar of RDX Crystals
				IgniteExplosion( this->ubID, this->sX, this->sY, (INT16)(gpWorldLevelData[this->sGridNo].sHeight), this->sGridNo, 136, this->pathing.bLevel );
			}
			else
			{
				// Solution 2: Crude simulation of a 'melee' attack (this is a rig-up, a better solution needs to be found).
				// do we 'hit'?
				if ( Random( 100 ) > 30 )
				{
					// Play sound
					PlayJA2SampleFromFile( "Sounds\\zombie_swish1.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

					INT8 oldlife = pTSoldier->stats.bLife;

					INT16 damage = (INT16)(5 + Random( 20 ));
					if ( pTSoldier->stats.bLife - damage < 0 )
						damage = oldlife;

					// We've got a problem if we kill someone outright without him collapsing properly...
					// FIX: We'll adjust our damage, so if we'd kill someone without collapsing first, we lower our damage, to let him collapse
					// After all, this whole thing's a rig up, so there shouldn't be a problem with that
					if ( oldlife >= OKLIFE && oldlife <= damage )
						damage -= (INT16)((5 + Random( 5 )));

					INT16 breathdamage = (INT16)(500 + Random( 1500 ));
					if ( pTSoldier->bBreath - breathdamage < 0 )
						breathdamage = pTSoldier->bBreath;
					
					pTSoldier->SoldierTakeDamage( 0, damage, breathdamage, TAKE_DAMAGE_HANDTOHAND, this->ubID, pTSoldier->sGridNo, 0, TRUE );

					if ( pTSoldier->stats.bLife == 0 )
					{
						// FINISH HIM!
						HandleTakeDamageDeath( pTSoldier, oldlife, TAKE_DAMAGE_BLOODLOSS );
					}
					else if ( pTSoldier->stats.bLife < OKLIFE && !pTSoldier->bCollapsed )
					{
						// let the target collapse...
						SoldierCollapse( pTSoldier );
					}
				}
				else
				{
					// Play sound
					PlayJA2SampleFromFile( "Sounds\\zombie_swish2.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				}

				// zombie is done doing harm...
				EndAIGuysTurn( this );
			}

			// ensure that we are running on our way to the next enemy
			this->EVENT_InitNewSoldierAnim( RUNNING, 0, FALSE );
		}
		else
		{
			// Flugente: zombies do not kick
			BOOLEAN nokick = FALSE;
			if ( this->IsZombie( ) || !IS_MERC_BODY_TYPE(this))//shadooow: fix for endless clock issue with Elliot in interrogation cutscene 
				nokick = TRUE;

			// sevenfm: don't use kick when attacking with any weapon in hand
			if( this->inv[ HANDPOS ].exists() )
			{
				nokick = TRUE;
			}

			// Flugente: disease can stop us from using our arms normally
			if ( !nokick
				&& gGameExternalOptions.fDisease
				&& gGameExternalOptions.fDiseaseSevereLimitations
				&& this->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
				nokick = TRUE;

			// Look at stance of target
			switch ( gAnimControl[pTSoldier->usAnimState].ubEndHeight )
			{
			case ANIM_STAND:
			case ANIM_CROUCH:

				if ( !ItemIsCrowbar(usItem) )
				{
					BOOLEAN fCannotKick = (ubDirection & 1);
					// SANDRO - we will determine here what type of punch we are gonna use
					if ( gGameExternalOptions.fEnhancedCloseCombatSystem && (this->aiData.bAimTime > 0) )
					{
						if ( gAnimControl[pTSoldier->usAnimState].ubEndHeight == ANIM_STAND )
						{
							// if we aim for legs, always use kick
							if ( this->bAimMeleeLocation == AIM_SHOT_LEGS && !(ubDirection & 1) && !nokick )
							{
								this->EVENT_InitNewSoldierAnim( FOCUSED_HTH_KICK, 0, FALSE );
							}
							// if we aim for head, always use punch animation
							else if ( this->bAimMeleeLocation == AIM_SHOT_HEAD || (ubDirection & 1) )
							{
								this->EVENT_InitNewSoldierAnim( FOCUSED_PUNCH, 0, FALSE );
							}
							// otherwise make it random, but favor the punch a bit
							else
							{
								if ( nokick || Random( 20 ) > 8 )
									this->EVENT_InitNewSoldierAnim( FOCUSED_PUNCH, 0, FALSE );
								else
									this->EVENT_InitNewSoldierAnim( FOCUSED_HTH_KICK, 0, FALSE );
							}
						}
						else // if crouching enemy
						{
							// random if aiming on head, favor kick though
							if ( this->bAimShotLocation == AIM_SHOT_HEAD || (ubDirection & 1) || nokick )
							{
								if ( nokick || Random( 20 ) > 12 || (ubDirection & 1) )
									this->EVENT_InitNewSoldierAnim( FOCUSED_PUNCH, 0, FALSE );
								else
									this->EVENT_InitNewSoldierAnim( FOCUSED_HTH_KICK, 0, FALSE );
							}
							// otherwise always use kick
							else
							{
								this->EVENT_InitNewSoldierAnim( FOCUSED_HTH_KICK, 0, FALSE );
							}
						}
					}
					else
					{
						if ( gAnimControl[pTSoldier->usAnimState].ubEndHeight == ANIM_STAND )
						{
							// if we aim for legs, always use kick
							if ( this->bAimMeleeLocation == AIM_SHOT_LEGS && !(ubDirection & 1) && !nokick )
							{
								this->EVENT_InitNewSoldierAnim( HTH_KICK, 0, FALSE );
							}
							// if we aim for head, always use punch animation
							else if ( this->bAimMeleeLocation == AIM_SHOT_HEAD || (ubDirection & 1) )
							{
								this->EVENT_InitNewSoldierAnim( PUNCH, 0, FALSE );
							}
							// otherwise make it random, but favor the punch a bit
							else
							{
								if ( nokick || Random( 20 ) > 8 )
									this->EVENT_InitNewSoldierAnim( PUNCH, 0, FALSE );
								else
									this->EVENT_InitNewSoldierAnim( HTH_KICK, 0, FALSE );
							}
						}
						else // if crouching enemy
						{
							// random if aiming on head, favor kick though
							if ( this->bAimMeleeLocation == AIM_SHOT_HEAD || (ubDirection & 1) || nokick )
							{
								if ( nokick || Random( 20 ) > 12 || (ubDirection & 1) )
									this->EVENT_InitNewSoldierAnim( PUNCH, 0, FALSE );
								else
									this->EVENT_InitNewSoldierAnim( HTH_KICK, 0, FALSE );
							}
							// otherwise always use kick
							else
							{
								this->EVENT_InitNewSoldierAnim( HTH_KICK, 0, FALSE );
							}
						}
					}
				}
				else
				{
					this->EVENT_InitNewSoldierAnim( CROWBAR_ATTACK, 0, FALSE );
				}

				// CHECK IF HE CAN SEE US, IF SO CHANGE DIR
				if ( pTSoldier->aiData.bOppList[this->ubID] == 0 && pTSoldier->bTeam != this->bTeam )
				{
					// Get direction to target
					// IF WE ARE AN ANIMAL, CAR, MONSTER, DONT'T TURN
					if ( !(pTSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE)) )
					{
						// OK, stop merc....
						pTSoldier->EVENT_StopMerc( pTSoldier->sGridNo, pTSoldier->ubDirection );

						if ( pTSoldier->bTeam != gbPlayerNum )
						{
							DebugAI(AI_MSG_INFO, pTSoldier, String("CancelAIAction: begin blade attack"));
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
					this->EVENT_InitNewSoldierAnim( PUNCH, 0, FALSE );
				}
				else
				{
					if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH )
					{
						// SET DESIRED STANCE AND SET PENDING ANIMATION
						SendChangeSoldierStanceEvent( this, ANIM_CROUCH );
						this->usPendingAnimation = PUNCH_LOW;
					}
					else
					{
						// USE crouched one
						// NEED TO CHANGE STANCE IF NOT CROUCHD!
						this->EVENT_InitNewSoldierAnim( PUNCH_LOW, 0, FALSE );
					}
				}
				break;

			}
		}
	}

	// SET TARGET GRIDNO
	this->sTargetGridNo = sGridNo;
	this->bTargetLevel = this->pathing.bLevel;
	//this->sLastTarget		= sGridNo;//dnl ch73 021013
	this->ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );
}


void SOLDIERTYPE::EVENT_SoldierBeginKnifeThrowAttack( INT32 sGridNo, UINT8 ubDirection )
{
	// Increment the number of people busy doing stuff because of an attack
	//if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	//{
	//	gTacticalStatus.ubAttackBusyCount++;
	//}
	//	this->bBulletsLeft = 1;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "!!!!!!! Starting knifethrow attack, bullets left %d", this->bBulletsLeft ) );
	DebugAttackBusy( String( "Begin knife throwing attack: ATB  %d\n", gTacticalStatus.ubAttackBusyCount ) );

	// SANDRO - new animation for throwing for big mercs by PasHancock
	if ( this->ubBodyType == BIGMALE && (DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) || (HAS_SKILL_TRAIT( this, THROWING_NT ) && gGameOptions.fNewTraitSystem) || (HAS_SKILL_TRAIT( this, THROWING_OT ) && !gGameOptions.fNewTraitSystem)))
	{
		this->EVENT_InitNewSoldierAnim( THROW_KNIFE_SP_BM, 0, FALSE );
	}
	else
	{
		this->EVENT_InitNewSoldierAnim( THROW_KNIFE, 0, FALSE );
	}

	//dnl ch70 160913 ugly but fast fix for not charging turning APs as there is no fire ready animation for throwing knives and I don't want to break SOLDIERTYPE just for that
	if ( this->usAnimState == THROW_KNIFE || this->usAnimState == THROW_KNIFE_SP_BM )
		usForceAnimState = this->usAnimState;
	else if ( this->usPendingAnimation == THROW_KNIFE || this->usPendingAnimation == THROW_KNIFE_SP_BM )
		usForceAnimState = this->usPendingAnimation;
	else
		usForceAnimState = INVALID_ANIMATION;
	this->flags.fDontUnsetLastTargetFromTurn = TRUE;
	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	if (this->ubDirection != ubDirection)
	{
		this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
		this->EVENT_SetSoldierDesiredDirection(ubDirection);
		this->EVENT_SetSoldierDirection(ubDirection);
	}

	usForceAnimState = INVALID_ANIMATION;

	// SET TARGET GRIDNO
	this->sTargetGridNo = sGridNo;

	// WANNE: Fix a vanilla bug: Throwing a knife costed one point less than it should.
	// Fixed by Tron (Stracciatella): Revision: 5787
	//this->sLastTarget		= sGridNo;

	this->flags.bTurningFromPronePosition = 0;
	// NB target level must be set by functions outside of here... but I think it
	// is already set in HandleItem or in the AI code - CJC
	this->ubTargetID = WhoIsThere2( sGridNo, this->bTargetLevel );
}


void SOLDIERTYPE::EVENT_SoldierBeginDropBomb( )
{
	// Increment the number of people busy doing stuff because of an attack
	switch ( gAnimControl[this->usAnimState].ubHeight )
	{
	case ANIM_STAND:

		this->EVENT_InitNewSoldierAnim( PLANT_BOMB, 0, FALSE );
		break;

	default:

		// Call hander for planting bomb...
		HandleSoldierDropBomb( this, this->aiData.sPendingActionData2 );
		this->SoldierGotoStationaryStance( );
		break;
	}
}

void SOLDIERTYPE::EVENT_SoldierDefuseTripwire( INT32 sGridNo, INT32 sItem )
{
	// Flugente: if item is tripwireactivated and is a planted bomb, call the defuse dialogue. We obviously know about the item's existence already...
	if ( gWorldItems[sItem].object.exists( ) && gWorldItems[sItem].object.fFlags & OBJECT_ARMED_BOMB && ItemIsTripwire(gWorldItems[sItem].object.usItem) )
	{
		// Increment the number of people busy doing stuff because of an attack
		switch ( gAnimControl[this->usAnimState].ubHeight )
		{
		case ANIM_STAND:

			this->EVENT_InitNewSoldierAnim( CROUCHING, 0, FALSE );
			break;

		default:

			// Call handler for planting bomb...
			HandleSoldierDefuseTripwire( this, sGridNo, sItem );
			this->SoldierGotoStationaryStance( );
			break;
		}
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginUseDetonator( void )
{
	// Increment the number of people busy doing stuff because of an attack
	switch ( gAnimControl[this->usAnimState].ubHeight )
	{
	case ANIM_STAND:

		this->EVENT_InitNewSoldierAnim( USE_REMOTE, 0, FALSE );
		break;

	default:

		// Call hander for planting bomb...
		HandleSoldierUseRemote( this, this->aiData.sPendingActionData2 );
		break;
	}
}

void SOLDIERTYPE::EVENT_SoldierBeginFirstAid( INT32 sGridNo, UINT8 ubDirection )
{
	SOLDIERTYPE *pTSoldier;
	//UINT32 uiMercFlags;
	UINT16 usSoldierIndex;
	BOOLEAN fRefused = FALSE;

	usSoldierIndex = WhoIsThere2( sGridNo, this->pathing.bLevel );
	if ( usSoldierIndex != NOBODY )
	{
		pTSoldier = MercPtrs[usSoldierIndex];

		//SANDRO - hack! Find out if we are a doctor with medical bag trying to make a surgery
		this->fDoingSurgery = FALSE;
		if ( (NUM_SKILL_TRAITS( this, DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery) && ItemIsMedicalKit(this->inv[HANDPOS].usItem) && gGameOptions.fNewTraitSystem )
		{
			if ( ((pTSoldier->bTeam == OUR_TEAM) || (pTSoldier->bTeam == MILITIA_TEAM))
				 && (IS_MERC_BODY_TYPE( pTSoldier ) || IS_CIV_BODY_TYPE( pTSoldier ))
				 && (pTSoldier->iHealableInjury >= 100) && pTSoldier->ubID != this->ubID
				 && (gTacticalStatus.ubLastRequesterSurgeryTargetID == pTSoldier->ubID) )
			{
				this->fDoingSurgery = TRUE;
			}
		}

		// OK, check if we should play quote...
		if ( pTSoldier->bTeam != gbPlayerNum )
		{
			if ( pTSoldier->ubProfile != NO_PROFILE && !RPC_RECRUITED( pTSoldier ) && ( gMercProfiles[pTSoldier->ubProfile].Type == PROFILETYPE_RPC ||
				gMercProfiles[pTSoldier->ubProfile].Type == PROFILETYPE_NPC ) )
			{
				fRefused = PCDoesFirstAidOnNPC( pTSoldier->ubProfile );
			}

			if ( !fRefused )
			{
				/*
				if ( CREATURE_OR_BLOODCAT( pTSoldier ) )
				{
				// nope!!
				fRefused = TRUE;
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[ STR_REFUSE_FIRSTAID_FOR_CREATURE ] );
				}
				else if ( !pTSoldier->aiData.bNeutral && pTSoldier->stats.bLife >= OKLIFE && pTSoldier->bSide != this->bSide )
				*/
				// Flugente: people we captured don't refuse to be bandaged
				if ( !pTSoldier->aiData.bNeutral && pTSoldier->stats.bLife >= OKLIFE && pTSoldier->bSide != this->bSide && !(pTSoldier->usSoldierFlagMask & SOLDIER_POW) )
				{
					fRefused = TRUE;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, Message[STR_REFUSE_FIRSTAID] );
				}

			}
		}

		if ( fRefused )
		{
			this->fDoingSurgery = FALSE; // SANDRO

			UnSetUIBusy( this->ubID );
			return;
		}

		// ATE: We can only give firsty aid to one perosn at a time... cancel
		// any now...
		this->InternalGivingSoldierCancelServices( FALSE );

		BOOLEAN fInProne = FALSE;
		if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE && gAnimControl[pTSoldier->usAnimState].ubEndHeight == ANIM_PRONE && this->fDoingSurgery == FALSE )
		{
			fInProne = TRUE;
		}
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		// CHANGE TO ANIMATION
		if ( fInProne )
		{
			// HACK! If we are not prone after the above stance change and we should be, send us down before start
			if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_PRONE )
			{
				this->usPendingAnimation = START_AID_PRN;
				SendChangeSoldierStanceEvent( this, ANIM_PRONE );
			}
			else
			{
				if ( !is_networked )
					this->EVENT_InitNewSoldierAnim( START_AID_PRN, 0, FALSE );
				else
					this->ChangeSoldierState( START_AID_PRN, 0, 0 );
			}
		}
		else
		{
			if ( !is_networked )
				this->EVENT_InitNewSoldierAnim( START_AID, 0, FALSE );
			else
				this->ChangeSoldierState( START_AID, 0, 0 );
		}

		// SET TARGET GRIDNO
		this->sTargetGridNo = sGridNo;

		// SET PARTNER ID
		this->ubServicePartner = usSoldierIndex;

		// SET PARTNER'S COUNT REFERENCE
		pTSoldier->ubServiceCount++;

		// If target and doer are no the same guy...
		if ( pTSoldier->ubID != this->ubID && !pTSoldier->bCollapsed )
		{
			pTSoldier->SoldierGotoStationaryStance( );
		}

		// Flugente: additional dialogue
		AdditionalTacticalCharacterDialogue_CallsLua( this, ADE_BANDAGE_PERFORM_BEGIN, pTSoldier->ubProfile );
		AdditionalTacticalCharacterDialogue_CallsLua( pTSoldier, ADE_BANDAGE_RECEIVE_BEGIN, this->ubProfile );

		// Flugente: if the medkit we use has drug effects set, apply them
		// as a kit is used up over time, applying it several times would screw with chance effects, so just apply once
		ApplyDrugs_New( pTSoldier, this->inv[HANDPOS].usItem, ( ( this->inv[HANDPOS] ) )[0]->data.objectStatus );
	}
}


void SOLDIERTYPE::EVENT_SoldierEnterVehicle( INT32 sGridNo, UINT8 ubDirection, UINT8 ubSeatIndex )
{
	SOLDIERTYPE *pTSoldier;
	UINT32 uiMercFlags;
	UINT16 usSoldierIndex;

	if ( FindSoldier( sGridNo, &usSoldierIndex, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
	{
		pTSoldier = MercPtrs[usSoldierIndex];

		// Enter vehicle...
		EnterVehicle( pTSoldier, this, ubSeatIndex );
	}

	UnSetUIBusy( this->ubID );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - This whole procedure was merged with the surgery ability of the doctor trait
UINT32 SOLDIERTYPE::SoldierDressWound( SOLDIERTYPE *pVictim, INT16 sKitPts, INT16 sStatus )
{
	UINT32 uiDressSkill, uiPossible, uiActual, uiMedcost, uiDeficiency, uiAvailAPs, uiUsedAPs;
	UINT8 ubBelowOKlife = 0, ubPtsLeft = 0;
	BOOLEAN	fRanOut = FALSE;
	BOOLEAN	fOnSurgery = FALSE;
	INT8 bInitialBleeding;

	if ( (pVictim->bBleeding < 1 && pVictim->stats.bLife >= OKLIFE) && !(pVictim->iHealableInjury > 0 && this->fDoingSurgery) )
	{
		return(0);		// nothing to do, shouldn't have even been called!
	}

	if ( pVictim->stats.bLife <= 0 )
	{
		return(0);
	}

	// Flugente: dynamic opinions
	if (gGameExternalOptions.fDynamicOpinions)
	{
		AddOpinionEvent(pVictim->ubProfile, this->ubProfile, OPINIONEVENT_BANDAGED);
	}

	bInitialBleeding = pVictim->bBleeding;

	// in case he has multiple kits in hand, limit influence of kit status to 100%!
	if ( sStatus >= 100 )
	{
		sStatus = 100;
	}

	// if we are going to do the surgery
	// Flugente: AI medics are allowed to perform surgery without first aid kits, and can do this on themselves
	if ( pVictim->iHealableInjury > 0 && this->fDoingSurgery && (this->ubID != pVictim->ubID || (gGameExternalOptions.fEnemyMedicsHealSelf && this->bTeam == ENEMY_TEAM))
		 && gGameOptions.fNewTraitSystem && (NUM_SKILL_TRAITS( this, DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery)
		 && (ItemIsMedicalKit(this->inv[HANDPOS].usItem) || this->bTeam == ENEMY_TEAM) )
	{
		fOnSurgery = TRUE;
	}

	// calculate wound-dressing skill (3x medical, 2x equip, 1x level, 1x dex)
	if ( gGameOptions.fNewTraitSystem )
	{
		uiDressSkill = ((7 * EffectiveMedical( this )) +					// medical knowledge
						 (sStatus)+ 																// state of medical kit
						 (10 * EffectiveExpLevel( this )) +					// battle injury experience
						 EffectiveDexterity( this, FALSE )) / 10;		// general "handiness"
	}
	else
	{
		uiDressSkill = ((3 * EffectiveMedical( this )) +					// medical knowledge
						 (2 * sStatus) + 																// state of medical kit
						 (10 * EffectiveExpLevel( this )) +					// battle injury experience
						 EffectiveDexterity( this, FALSE )) / 7;		// general "handiness"
	}

	// try to use every AP that the merc has left
	uiAvailAPs = this->bActionPoints;

	// OK, If we are in real-time, use another value...
	if ( !(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		// Set to a value which looks good based on our tactical turns duration
		uiAvailAPs = RT_FIRST_AID_GAIN_MODIFIER;
	}

	// calculate how much bandaging CAN be done this turn
	uiPossible = (uiAvailAPs * uiDressSkill) / 50;	// max rate is 2 * fullAPs

	// if no healing is possible (insufficient APs or insufficient dressSkill)
	if ( !uiPossible )
		return(0);

	// using the GOOD medic stuff
	if (ItemIsMedicalKit(this->inv[HANDPOS].usItem) && !(fOnSurgery) ) // added check
	{
		uiPossible += (uiPossible / 2);			// add extra 50 %
	}

	// when prone, bandaging is harder
	if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
	{
		// if we bandage ourselves, make it rather had when prone
		if ( this->ubID == pVictim->ubID )
			uiPossible = uiPossible / 2; // -50% speed
		else
			uiPossible = uiPossible * 4 / 5; // -20% speed
	}

	// Doctor trait improves basic bandaging ability
	if ( !(fOnSurgery) && gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( this, DOCTOR_NT ) )
	{
		uiPossible = uiPossible * (100 - gSkillTraitValues.bSpeedModifierBandaging) / 100;
		uiPossible += (uiPossible * gSkillTraitValues.ubDOBandagingSpeedPercent * NUM_SKILL_TRAITS( this, DOCTOR_NT ) + this->GetBackgroundValue( BG_PERC_BANDAGING )) / 100;
	}

	uiActual = uiPossible;		// start by assuming maximum possible

	// figure out how far below OKLIFE the victim is
	// SANDRO - only if we are actually here to bandage the target
	if ( pVictim->bBleeding )
	{
		if ( pVictim->stats.bLife >= OKLIFE )
		{
			ubBelowOKlife = 0;
		}
		else
		{
			ubBelowOKlife = OKLIFE - pVictim->stats.bLife;
		}

		// figure out how many healing pts we need to stop dying (2x cost)
		uiDeficiency = (2 * ubBelowOKlife);

		// if, after that, the patient will still be bleeding
		if ( (pVictim->bBleeding - ubBelowOKlife) > 0 )
		{
			// then add how many healing pts we need to stop bleeding (1x cost)
			uiDeficiency += (pVictim->bBleeding - ubBelowOKlife);
		}
		// On surgery, alter this by amount of life we can heal
		if ( fOnSurgery )
		{
			uiDeficiency += (pVictim->iHealableInjury / 100);
		}

		// now, make sure we weren't going to give too much
		if ( uiActual > uiDeficiency )	// if we were about to apply too much
			uiActual = uiDeficiency;	// reduce actual not to waste anything
	}

	// now make sure we HAVE that much
	if (ItemIsMedicalKit(this->inv[HANDPOS].usItem))
	{
		if ( fOnSurgery )
			uiMedcost = (uiActual * gSkillTraitValues.usDOSurgeryMedBagConsumption) / 100;		// surgery drains the kit a lot
		else
			uiMedcost = (uiActual + 1) / 2;		// cost is only half, rounded up

		if ( uiMedcost > (UINT32)sKitPts )     		// if we can't afford this
		{
			fRanOut = TRUE;
			uiMedcost = sKitPts;		// what CAN we afford?
			if ( fOnSurgery ) // surgery check
				uiActual = (uiMedcost * 100) / gSkillTraitValues.usDOSurgeryMedBagConsumption;
			else
				uiActual = uiMedcost * 2;		// give double this as aid
		}
	}
	else
	{
		uiMedcost = uiActual;

		if ( uiMedcost > (UINT32)sKitPts )		// can't afford it
		{
			fRanOut = TRUE;
			uiMedcost = uiActual = sKitPts;   	// recalc cost AND aid
		}
	}

	ubPtsLeft = (UINT8)uiActual;

	// heal real life points first (if below OKLIFE) because we don't want the
	// patient still DYING if bandages run out, or medic is disabled/distracted!
	// NOTE: Dressing wounds for life below OKLIFE now costs 2 pts/life point!
	if ( ubPtsLeft && pVictim->stats.bLife < OKLIFE )
	{
		// if we have enough points to bring him all the way to OKLIFE this turn
		if ( ubPtsLeft >= (2 * ubBelowOKlife) )
		{
			// insta-healable injury check
			if ( pVictim->iHealableInjury > 0 )
			{
				pVictim->iHealableInjury -= ((OKLIFE - pVictim->stats.bLife) * 100);
				if ( pVictim->iHealableInjury < 0 )
					pVictim->iHealableInjury = 0;
			}

			// raise life to OKLIFE
			pVictim->stats.bLife = OKLIFE;

			// reduce bleeding by the same number of life points healed up
			pVictim->bBleeding -= ubBelowOKlife;

			// use up appropriate # of actual healing points
			ubPtsLeft -= (2 * ubBelowOKlife);
		}
		else
		{
			// SANDRO - insta-healable injury check
			if ( pVictim->iHealableInjury > 0 )
			{
				pVictim->iHealableInjury -= ((ubPtsLeft / 2) * 100);
				if ( pVictim->iHealableInjury < 0 )
					pVictim->iHealableInjury = 0;
			}

			pVictim->stats.bLife += (ubPtsLeft / 2);
			pVictim->bBleeding -= (ubPtsLeft / 2);

			ubPtsLeft = ubPtsLeft % 2;	// if ptsLeft was odd, ptsLeft = 1
		}

		// this should never happen any more, but make sure bleeding not negative
		if ( pVictim->bBleeding < 0 )
		{
			pVictim->bBleeding = 0;
		}

		// if this healing brought the patient out of the worst of it, cancel dying
		if ( pVictim->stats.bLife >= OKLIFE )
		{
			//pVictim->dying = pVictim->dyingComment = FALSE;
			//pVictim->shootOn = TRUE;

			// turn off merc QUOTE flags
			pVictim->flags.fDyingComment = FALSE;
		}

		// update patient's entire panel (could have regained consciousness, etc.)
	}

	// SURGERY 
	// first return the real life back, then bandage the rest if possible
	if ( fOnSurgery && gGameOptions.fNewTraitSystem ) // double check for new traits
	{
		UINT16 usLifeReturned = 0;
		UINT16 usReturnDamagedStatRate = 0;
		// find out if we will repair any stats...
		if ( NumberOfDamagedStats( pVictim ) > 0 )
		{
			usReturnDamagedStatRate = ((gSkillTraitValues.usDORepairStatsRateBasic + gSkillTraitValues.usDORepairStatsRateOnTop * NUM_SKILL_TRAITS( this, DOCTOR_NT )));
			usReturnDamagedStatRate -= max( 0, ((usReturnDamagedStatRate * gSkillTraitValues.ubDORepStPenaltyIfAlsoHealing) / 100) );

			// ... in which case, reduce the points
			ubPtsLeft = max( 0, ((ubPtsLeft * (100 - gSkillTraitValues.ubDOHealingPenaltyIfAlsoStatRepair)) / 100) );
		}

		// Important note! : HealableInjury is always stores the total HPs the victim is missing, not the amount which we will heal,
		// so we always take a portion of patient's damage here, reduce the HealableInjury by this portion, while only healing a portion of this portion in actual HPs;
		// this means the rest of HPs will remain as "unhealable", the patient will miss X HPs but has no HealableInjury on self..
		if ( ubPtsLeft >= (pVictim->iHealableInjury / 100) )
		{
			if ( this->usSoldierFlagMask2 & SOLDIER_SURGERY_BOOSTED )
				usLifeReturned = pVictim->iHealableInjury * ( gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentBloodbag + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( this, DOCTOR_NT ) ) / 100;
			else
				usLifeReturned = pVictim->iHealableInjury * (gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( this, DOCTOR_NT )) / 100;

			pVictim->iHealableInjury = 0;
			//CHRISL: Why would we arbitrarily use all ubPtsLeft when a victim isn't bleeding?  And why would the medical bag, which we have to use in order to 
			//	do surgery, have any extra benefit?  Plus, the medical back bonus can actually result in ubPtsLeft being HIGHER then it was before we healed the
			//	victim, which makes no sense.
			// keep the rest of the points to bandaging if neccessary
			//if (pVictim->bBleeding > 0)
			//{
			ubPtsLeft = max( 0, (ubPtsLeft - (usLifeReturned / 100)) );
			//	ubPtsLeft += (ubPtsLeft/2); // we use medical bag so add the bonus for that.
			//}
			//else
			//{
			//	ubPtsLeft = 0;
			//}

			// We are finished !!!
			this->fDoingSurgery = FALSE;
			gTacticalStatus.ubLastRequesterSurgeryTargetID = NOBODY; // reset last target

			if ( this->bTeam != ENEMY_TEAM )
			{
				// throw message
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SURGERY_FINISHED], pVictim->GetName( ) );
			}

			// add to record - another surgery undergoed
			if ( pVictim->ubProfile != NO_PROFILE && usLifeReturned >= 100 )
				gMercProfiles[pVictim->ubProfile].records.usTimesSurgeryUndergoed++;

			// add to record - another surgery made
			if ( this->ubProfile != NO_PROFILE && usLifeReturned >= 100 )
				gMercProfiles[this->ubProfile].records.usSurgeriesMade++;
		}
		else
		{
			if ( this->usSoldierFlagMask2 & SOLDIER_SURGERY_BOOSTED )
				usLifeReturned = ubPtsLeft * ( gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentBloodbag + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( this, DOCTOR_NT ) );
			else
				usLifeReturned = ubPtsLeft * (gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( this, DOCTOR_NT ));

			pVictim->iHealableInjury -= (ubPtsLeft * 100);
			ubPtsLeft = 0;
		}
		// repair the stats here!
		if ( usReturnDamagedStatRate > 0 )
		{
			RegainDamagedStats( pVictim, (usLifeReturned * usReturnDamagedStatRate / 100) );
		}

		// some paranoya checks for sure
		if ( (pVictim->stats.bLife + (usLifeReturned / 100)) <= pVictim->stats.bLifeMax )
		{
			pVictim->stats.bLife += (usLifeReturned / 100);
			if ( pVictim->bBleeding >= (usLifeReturned / 100) )
			{
				pVictim->bBleeding -= (usLifeReturned / 100);
				uiMedcost += (usLifeReturned / 200); // add medkit points cost for unbandaged part
			}
			else
			{
				pVictim->bBleeding = 0;
				uiMedcost += max( 0, (((usLifeReturned / 100) - pVictim->bBleeding) / 2) ); // add medkit points cost for unbandaged part 
			}

			// display healing done
			pVictim->flags.fDisplayDamage = TRUE;
			pVictim->sDamage -= (usLifeReturned / 100);
		}
		else // this shouldn't even happen, but we still want to have it here for sure
		{
			// display healing done
			pVictim->flags.fDisplayDamage = TRUE;
			pVictim->sDamage -= (pVictim->stats.bLifeMax - pVictim->stats.bLife);

			pVictim->stats.bLife = pVictim->stats.bLifeMax;
			pVictim->iHealableInjury = 0;
			pVictim->bBleeding = 0;
		}

		// Reduce max breath based on life returned
		if ( (pVictim->bBreathMax - (((usLifeReturned / 100) * gSkillTraitValues.usDOSurgeryMaxBreathLoss) / 100)) <= BREATHMAX_ABSOLUTE_MINIMUM )
		{
			pVictim->bBreathMax = BREATHMAX_ABSOLUTE_MINIMUM;
		}
		else
		{
			pVictim->bBreathMax -= (((usLifeReturned / 100) * gSkillTraitValues.usDOSurgeryMaxBreathLoss) / 100);
		}

		if ( pVictim->iHealableInjury > ((pVictim->stats.bLifeMax - pVictim->stats.bLife) * 100) )
			pVictim->iHealableInjury = ((pVictim->stats.bLifeMax - pVictim->stats.bLife) * 100);
		else if ( pVictim->iHealableInjury < 0 )
			pVictim->iHealableInjury = 0;

		// Flugente: campaign stats
		gCurrentIncident.usIncidentFlags |= INCIDENT_SURGERY;
	}

	// if any healing points remain, apply that to any remaining bleeding (1/1)
	// DON'T spend any APs/kit pts to cure bleeding until merc is no longer dying
	//if ( ubPtsLeft && pVictim->bBleeding && !pVictim->dying)
	if ( ubPtsLeft && pVictim->bBleeding )
	{
		// if we have enough points to bandage all remaining bleeding this turn
		if ( ubPtsLeft >= pVictim->bBleeding )
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

	//CHRISL: If by some chance ubPtsLeft ends up being higher then uiActual, we'll end up with a huge value since uiActual is an unsigned variable.
	// if there are any ptsLeft now, then we didn't actually get to use them
	uiActual = max( 0, (INT32)(uiActual - ubPtsLeft) );

	// usedAPs equals (actionPts) * (%of possible points actually used)
	uiUsedAPs = (uiActual * uiAvailAPs) / uiPossible;

	if (ItemIsMedicalKit(this->inv[HANDPOS].usItem) && !(fOnSurgery) )	// using the GOOD medic stuff
	{
		uiUsedAPs = (uiUsedAPs * 2) / 3;	// reverse 50% bonus by taking 2/3rds
	}

	// SANDRO - surgery is harder so cost more BPs
	if ( fOnSurgery )
	{
		DeductPoints( this, (INT16)uiUsedAPs, (INT16)(uiUsedAPs * 15) );
	}
	else
	{
		DeductPoints( this, (INT16)uiUsedAPs, (INT16)((uiUsedAPs * APBPConstants[BP_PER_AP_LT_EFFORT])) );
	}

	if ( this->bTeam == gbPlayerNum )
	{
		// surgery is harder so gives more exp
		if ( fOnSurgery )
		{
			// MEDICAL GAIN   (actual / 2):  Helped someone by giving first aid
			StatChange( this, MEDICALAMT, (UINT16)(uiActual + 2), FALSE );

			// DEXTERITY GAIN (actual / 6):  Helped someone by giving first aid
			StatChange( this, DEXTAMT, (UINT16)((uiActual / 3) + 2), FALSE );
		}
		else
		{
			// MEDICAL GAIN   (actual / 2):  Helped someone by giving first aid
			StatChange( this, MEDICALAMT, (UINT16)(uiActual / 2), FALSE );

			// DEXTERITY GAIN (actual / 6):  Helped someone by giving first aid
			StatChange( this, DEXTAMT, (UINT16)(uiActual / 6), FALSE );
		}
	}

	// merc records - bandaging
	if ( bInitialBleeding > 1 && pVictim->bBleeding == 0 && this->ubProfile != NO_PROFILE )
		gMercProfiles[this->ubProfile].records.usMercsBandaged++;

	if ( is_networked && pVictim->ubID > 19 )send_heal( pVictim );

	return(uiMedcost);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////


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

					// SANDRO - added end of surgery attempt
					if ( pTSoldier->fDoingSurgery )
						pTSoldier->fDoingSurgery = FALSE; // Surgery finished

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
							if ( gAnimControl[pTSoldier->usAnimState].ubEndHeight == ANIM_PRONE )
							{
								if ( !is_networked )
									pTSoldier->EVENT_InitNewSoldierAnim( END_AID_PRN, 0, FALSE );
								else
									pTSoldier->ChangeSoldierState( END_AID_PRN, 0, 0 );
							}
							else
							{
								if ( !is_networked )
									pTSoldier->EVENT_InitNewSoldierAnim( END_AID, 0, FALSE );
								else
									pTSoldier->ChangeSoldierState( END_AID, 0, 0 );
							}
						}
					}

					// Flugente: additional dialogue
					AdditionalTacticalCharacterDialogue_CallsLua( pTSoldier, ADE_BANDAGE_PERFORM_END, this->ubProfile );
					AdditionalTacticalCharacterDialogue_CallsLua( this, ADE_BANDAGE_RECEIVE_END, pTSoldier->ubProfile );

					pTSoldier->usSoldierFlagMask2 &= ~SOLDIER_SURGERY_BOOSTED;
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
		pTSoldier = MercPtrs[this->ubServicePartner];

		// END SERVICE!
		pTSoldier->ubServiceCount--;

		this->ubServicePartner = NOBODY;

		// SANDRO - added end of surgery attempt
		if ( this->fDoingSurgery )
			this->fDoingSurgery = FALSE; // Surgery finished

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
				if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
				{
					if ( !is_networked )
						this->EVENT_InitNewSoldierAnim( END_AID_PRN, 0, FALSE );
					else
						this->ChangeSoldierState( END_AID_PRN, 0, 0 );
				}
				else
				{
					if ( !is_networked )
						this->EVENT_InitNewSoldierAnim( END_AID, 0, FALSE );
					else
						this->ChangeSoldierState( END_AID, 0, 0 );
				}
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
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "HaultSoldierFromSighting" ) );
	// SEND HUALT EVENT!
	EV_S_STOP_MERC				SStopMerc;

	SStopMerc.sGridNo = this->sGridNo;
	SStopMerc.ubDirection = this->ubDirection;
	SStopMerc.usSoldierID = this->ubID;
	SStopMerc.fset = TRUE;
	SStopMerc.sXPos = this->sX;
	SStopMerc.sYPos = this->sY;
	//AddGameEvent( S_STOP_MERC, 0, &SStopMerc ); //hayden.
	if ( (is_networked) && (this->ubID >= 120 || (!is_server && this->ubID >= 20)) ) return;//hayden
	if ( is_client )send_stop( &SStopMerc );

	// If we are a 'specialmove... ignore...
	if ( (gAnimControl[this->usAnimState].uiFlags & ANIM_SPECIALMOVE) )
	{
		return;
	}

	// OK, check if we were going to throw something, and give it back if so!
	if ( this->pTempObject != NULL && fFromSightingEnemy )
	{
		if ( this->pThrowParams->ubActionCode == THROW_ARM_ITEM )
		{
			if (!this->inv[HANDPOS].exists())
			{
				// put the one-handed weapon in the guy's hand...
				if (!PlaceObject(this, HANDPOS, this->pTempObject))
				{
					AutoPlaceObject(this, this->pTempObject, FALSE);
				}
			}	
			//AXP 25.03.2007: Not needed anymore, grenade costs are only deducted on throwing the object
			//AXP 24.03.2007: Give APs back if we wanted to throw grenade, but interrupt/spotting occured
			//DeductPoints( this, -MinAPsToAttack( this, this->sTargetGridNo, FALSE ), 0 );
		}
		else
		{
			// Place it back into inv....
			AutoPlaceObject(this, this->pTempObject, FALSE);
		}


		OBJECTTYPE::DeleteMe( &this->pTempObject );
		this->usPendingAnimation = NO_PENDING_ANIMATION;
		this->usPendingAnimation2 = NO_PENDING_ANIMATION;

		// Decrement attack counter...
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending throw because saw something" ) );
		DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending throw because saw something\n" );
		FreeUpAttacker( );

		// ATE: Goto stationary stance......
		this->SoldierGotoStationaryStance( );

		DirtyMercPanelInterface( this, DIRTYLEVEL2 );
	}

	// Kaiden: Added fix from UB for seeing new enemies when throwing Knives.
	// ATE: Dave, don't kill me
	// Here, we need to handle the situation when we're throweing a knife and we see somebody
	// cause for some reason throwing a knie does not use the pTempObject stuff that all other stuff does...
	if ( this->usPendingAnimation == THROW_KNIFE || this->usPendingAnimation == THROW_KNIFE_SP_BM )
	{
		// Decrement attack counter...
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending throw knife because saw something" ) );
		DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending throw knife because saw something" );
		FreeUpAttacker( );

		// ATE: Goto stationary stance......
		this->SoldierGotoStationaryStance( );

		DirtyMercPanelInterface( this, DIRTYLEVEL2 );
	}
	
	if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
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

			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something" ) );
			DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something\n" );
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
	if ( fFromSightingEnemy || (this->pTempObject == NULL && !this->flags.fTurningToShoot) )
	{
		UnSetUIBusy( this->ubID );
	}

	this->bTurningFromUI = FALSE;

	UnSetEngagedInConvFromPCAction( this );
}


// HUALT EVENT IS USED TO STOP A MERC - NETWORKING SHOULD CHECK / ADJUST TO GRIDNO?
void SOLDIERTYPE::EVENT_StopMerc( INT32 sGridNo, INT8 bDirection )
{
	INT16 sX, sY;

	// MOVE GUY TO GRIDNO--- SHOULD BE THE SAME UNLESS IN MULTIPLAYER
	// Makesure center of tile
	ConvertGridNoToCenterCellXY(sGridNo, &sX, &sY);

	//Cancel pending events
	if ( !this->flags.fDelayedMovement )
	{
		this->usPendingAnimation = NO_PENDING_ANIMATION;
		this->usPendingAnimation2 = NO_PENDING_ANIMATION;
		this->ubPendingDirection = NO_PENDING_DIRECTION;
		this->aiData.ubPendingAction = NO_PENDING_ACTION;
	}

	this->bEndDoorOpenCode = 0;
	this->flags.bTurningFromPronePosition = 0;

	// Cancel path data!
	this->pathing.usPathIndex = this->pathing.usPathDataSize = 0;

	// Set ext tile waiting flag off!
	this->flags.fDelayedMovement = FALSE;

	// Turn off reverse...
	this->bReverse = FALSE;

	this->EVENT_SetSoldierPosition( (FLOAT)sX, (FLOAT)sY );
	this->pathing.sDestXPos = (INT16)this->dXPos;
	this->pathing.sDestYPos = (INT16)this->dYPos;
	this->EVENT_SetSoldierDirection( bDirection );

	if ( gAnimControl[this->usAnimState].uiFlags & ANIM_MOVING )
	{
		this->SoldierGotoStationaryStance( );
	}

	// ATE; IF turning to shoot, stop!
	if ( this->flags.fTurningToShoot )
	{
		this->flags.fTurningToShoot = FALSE;
		// Release attacker
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something" ) );
		DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something\n" );
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
	if ( Weapon[usNewItem].ubShotsPerBurst == 0 && !Weapon[this->inv[HANDPOS].usItem].NoSemiAuto )
	{
		this->bDoBurst = FALSE;
		this->bWeaponMode = WM_NORMAL;
		this->bDoAutofire = 0;
	}
	else if ( Weapon[usNewItem].NoSemiAuto )
	{
		this->bDoBurst = TRUE;
		this->bWeaponMode = WM_AUTOFIRE;
		this->bDoAutofire = 1;
	}

	// Flugente: if using a rifle grenade device, and a grenade i equipped, only grenade launching is allowed
	if ( HasAttachmentOfClass( &(this->inv[HANDPOS]), AC_RIFLEGRENADE ) )
	{
		OBJECTTYPE* pRifleGrenadeDeviceObj = FindAttachment_GrenadeLauncher( &(this->inv[HANDPOS]) );

		if ( pRifleGrenadeDeviceObj && FindLaunchableAttachment( &(this->inv[HANDPOS]), pRifleGrenadeDeviceObj->usItem ) )
			this->bWeaponMode = WM_ATTACHED_GL;
	}

	if (ItemIsTwoHanded(usNewItem) && Weapon[usNewItem].HeavyGun && gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
		this->bScopeMode = USE_ALT_WEAPON_HOLD;
	else
		this->bScopeMode = USE_BEST_SCOPE;

	// Flugente: use lowest valid barrel configuration
	this->usBarrelMode = 1;
	this->usBarrelMode = GetNextBarrelMode( usNewItem, this->usBarrelMode );

	if ( gAnimControl[this->usAnimState].uiFlags & ANIM_FIREREADY )
	{
		// Stop aiming!
		this->SoldierGotoStationaryStance( );
	}

	// Cancel services...
	this->GivingSoldierCancelServices( );

	// Did we have a rifle and do we now not have one?
	if ( usOldItem != NOTHING )
	{
		if ( Item[usOldItem].usItemClass == IC_GUN )
		{
			if (ItemIsTwoHanded(usOldItem) && !ItemIsRocketLauncher(usOldItem))
			{
				fOldRifle = TRUE;
			}
		}
	}

	if ( usNewItem != NOTHING )
	{
		if ( Item[usNewItem].usItemClass == IC_GUN )
		{
			if (ItemIsTwoHanded(usNewItem) && !ItemIsRocketLauncher(usNewItem))
			{
				fNewRifle = TRUE;
			}

			// Flugente: if we are using dual weapons, only allow iron sights (we can't look through two scopes simultaneously, can we?)
			if ( (Item[inv[HANDPOS].usItem].usItemClass & IC_WEAPON) && (Item[inv[SECONDHANDPOS].usItem].usItemClass & IC_WEAPON) )
			{
				std::map<INT8, OBJECTTYPE*> ObjList;
				GetScopeLists( this, &inv[HANDPOS], ObjList );

				std::map<INT8, OBJECTTYPE*>::iterator itend = ObjList.end( );
				for ( std::map<INT8, OBJECTTYPE*>::iterator it = ObjList.begin( ); it != itend; ++it )
				{
					if ( (*it).second != NULL )
					{
						bScopeMode = (*it).first;
					}
					else
						break;
				}
			}
		}
	}

	// Switch on stance!
	switch ( gAnimControl[this->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:

		if ( fOldRifle && !fNewRifle )
		{
			// Put it away!
			this->EVENT_InitNewSoldierAnim( LOWER_RIFLE, 0, FALSE );
		}
		else if ( !fOldRifle && fNewRifle )
		{
			// Bring it up!
			this->EVENT_InitNewSoldierAnim( RAISE_RIFLE, 0, FALSE );
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
	UINT8	 r, g, b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *)MemAlloc( sizeof(UINT16)* 256 );

	for ( cnt = 0; cnt < 256; ++cnt )
	{
		gmod = (pPalette[cnt].peGreen);
		bmod = (pPalette[cnt].peBlue);

		rmod = __max( rscale, (pPalette[cnt].peRed) );

		if ( fAdjustGreen )
		{
			gmod = __max( gscale, (pPalette[cnt].peGreen) );
		}

		r = (UINT8)__min( rmod, 255 );
		g = (UINT8)__min( gmod, 255 );
		b = (UINT8)__min( bmod, 255 );

		if ( gusRedShift < 0 )
			r16 = ((UINT16)r >> (-gusRedShift));
		else
			r16 = ((UINT16)r << gusRedShift);

		if ( gusGreenShift < 0 )
			g16 = ((UINT16)g >> (-gusGreenShift));
		else
			g16 = ((UINT16)g << gusGreenShift);


		if ( gusBlueShift < 0 )
			b16 = ((UINT16)b >> (-gusBlueShift));
		else
			b16 = ((UINT16)b << gusBlueShift);

		// Prevent creation of pure black color
		usColor = (r16&gusRedMask) | (g16&gusGreenMask) | (b16&gusBlueMask);

		if ( (usColor == 0) && ((r + g + b) != 0) )
			usColor = 0x0001;

		p16BPPPalette[cnt] = usColor;
	}
	return(p16BPPPalette);
}


UINT16 *CreateEnemyGreyGlow16BPPPalette( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, BOOLEAN fAdjustGreen )
{
	UINT16 *p16BPPPalette, r16, g16, b16, usColor;
	UINT32 cnt, lumin;
	UINT32 rmod, gmod, bmod;
	UINT8	 r, g, b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *)MemAlloc( sizeof(UINT16)* 256 );

	for ( cnt = 0; cnt < 256; cnt++ )
	{
		lumin = (pPalette[cnt].peRed * 299 / 1000) + (pPalette[cnt].peGreen * 587 / 1000) + (pPalette[cnt].peBlue * 114 / 1000);
		rmod = (100 * lumin) / 256;
		gmod = (100 * lumin) / 256;
		bmod = (100 * lumin) / 256;



		rmod = __max( rscale, rmod );

		if ( fAdjustGreen )
		{
			gmod = __max( gscale, gmod );
		}


		r = (UINT8)__min( rmod, 255 );
		g = (UINT8)__min( gmod, 255 );
		b = (UINT8)__min( bmod, 255 );

		if ( gusRedShift < 0 )
			r16 = ((UINT16)r >> (-gusRedShift));
		else
			r16 = ((UINT16)r << gusRedShift);

		if ( gusGreenShift < 0 )
			g16 = ((UINT16)g >> (-gusGreenShift));
		else
			g16 = ((UINT16)g << gusGreenShift);


		if ( gusBlueShift < 0 )
			b16 = ((UINT16)b >> (-gusBlueShift));
		else
			b16 = ((UINT16)b << gusBlueShift);

		// Prevent creation of pure black color
		usColor = (r16&gusRedMask) | (g16&gusGreenMask) | (b16&gusBlueMask);

		if ( (usColor == 0) && ((r + g + b) != 0) )
			usColor = 0x0001;

		p16BPPPalette[cnt] = usColor;
	}
	return(p16BPPPalette);
}


void ContinueMercMovement( SOLDIERTYPE *pSoldier )
{
	INT16		sAPCost;
	INT32 sGridNo;

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
		sAPCost = PtsToMoveDirection( pSoldier, (UINT8)guiPathingData[0] );

		if ( EnoughPoints( pSoldier, sAPCost, 0, (BOOLEAN)(pSoldier->bTeam == gbPlayerNum) ) )
		{
			// Acknowledge
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );

				// If we have a face, tell text in it to go away!
				if ( pSoldier->iFaceIndex != -1 )
				{
					gFacesData[pSoldier->iFaceIndex].fDisplayTextOver = FACE_ERASE_TEXT_OVER;
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
		if ( this->bBreath < 20 && !(this->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LOW_BREATH) &&
			 gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && !(this->ubServiceCount) ) // SANDRO - added check to not play this if on healing
		{
			// SANDRO - say our personality quote for being out of breath caused by heat 
			if ( MercIsHot( this ) && this->ubWhatKindOfMercAmI != MERC_TYPE__PLAYER_CHARACTER )
			{
				TacticalCharacterDialogue( this, QUOTE_PERSONALITY_TRAIT );

				// Flugente: dynamic opinions
				if (gGameExternalOptions.fDynamicOpinions)
				{
					HandleDynamicOpinionChange(this, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE);
				}
			}
			else
			{
				// WARN!
				TacticalCharacterDialogue( this, QUOTE_OUT_OF_BREATH );
			}
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

	if ( this->bBreath == 0 && !this->bCollapsed && !(this->flags.uiStatusFlags & (SOLDIER_VEHICLE | SOLDIER_ANIMAL | SOLDIER_MONSTER)) )
	{
		if ( !(this->ubServiceCount) ) // added by SANDRO (we don't want to collapse when on surgery)
		{
			// Collapse!
			// OK, Set a flag, because we may still be in the middle of an animation what is not interruptable...
			this->bBreathCollapsed = TRUE;

			return(TRUE);
		}
	}

	return(FALSE);
}


BOOLEAN SOLDIERTYPE::InternalIsValidStance( INT8 bDirection, INT8 bNewStance )
{
	UINT16								usOKToAddStructID = 0;
	STRUCTURE_FILE_REF		*pStructureFileRef;
	UINT16								usAnimSurface = 0;
	UINT16								usAnimState;

	// Check, if dest is prone, we can actually do this!

	// If we are a vehicle, we can only 'stand'
	if ( (this->flags.uiStatusFlags & SOLDIER_VEHICLE) && bNewStance != ANIM_STAND )
	{
		return(FALSE);
	}

	// Check if we are in water?
	if ( this->MercInWater( ) )
	{
		if ( bNewStance == ANIM_PRONE || bNewStance == ANIM_CROUCH )
		{
			return(FALSE);
		}
	}

	if ( this->ubBodyType == ROBOTNOWEAPON && bNewStance != ANIM_STAND )
	{
		return(FALSE);
	}

	// Check if we are in water?
	if ( AM_AN_EPC( this ) )
	{
		if ( bNewStance == ANIM_PRONE )
		{
			return(FALSE);
		}
		else
		{
			return(TRUE);
		}
	}


	if ( this->bCollapsed )
	{
		//CHRISL: Changes from ADB rev 1475.
		if ( bNewStance == ANIM_CROUCH )
		{
			return(FALSE);
		}
		//when civilians are collapsed and die they may change to stand in order to fall forward
		if ( bNewStance == ANIM_STAND && this->ubBodyType <= REGFEMALE )
		{
			//if we are trying to stand and we are a MERC
			return(FALSE);
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

	switch ( bNewStance )
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

	usAnimSurface = DetermineSoldierAnimationSurface( this, usAnimState );

	// Get structure ref........
	pStructureFileRef = GetAnimationStructureRef( this->ubID, usAnimSurface, usAnimState );

	if ( pStructureFileRef != NULL )
	{
		// Can we add structure data for this stance...?
		if ( !OkayToAddStructureToWorld( this->sGridNo, this->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[bDirection]]), usOKToAddStructID ) )
		{
			return(FALSE);
		}
	}

	return(TRUE);
}


BOOLEAN IsValidStance( SOLDIERTYPE *pSoldier, INT8 bNewStance )
{
	return(pSoldier->InternalIsValidStance( pSoldier->ubDirection, bNewStance ));
}


BOOLEAN IsValidMovementMode( SOLDIERTYPE *pSoldier, INT16 usMovementMode )
{
	// Check, if dest is prone, we can actually do this!

	// Check if we are in water?
	if ( pSoldier->MercInWater( ) )
	{
		if ( usMovementMode == RUNNING || usMovementMode == SWATTING || usMovementMode == CRAWLING )
		{
			return(FALSE);
		}
	}

	return(TRUE);
}


void SelectMoveAnimationFromStance( SOLDIERTYPE *pSoldier )
{
	// Determine which animation to do...depending on stance and gun in hand...
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
	{
	case ANIM_STAND:
		pSoldier->EVENT_InitNewSoldierAnim( WALKING, 0, FALSE );
		break;

	case ANIM_PRONE:
		pSoldier->EVENT_InitNewSoldierAnim( CRAWLING, 0, FALSE );
		break;

	case ANIM_CROUCH:
		pSoldier->EVENT_InitNewSoldierAnim( SWATTING, 0, FALSE );
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
		*psHeight = (INT16)5;
		*psWidth = (INT16)5;

		return;
	}

	if ( gAnimSurfaceDatabase[usAnimSurface].hVideoObject == NULL )
	{
		*psHeight = (INT16)5;
		*psWidth = (INT16)5;
		return;
	}

	// OK, noodle here on what we should do... If we take each frame, it will be different slightly
	// depending on the frame and the value returned here will vary thusly. However, for the
	// uses of this function, we should be able to use just the first frame...

	if ( pSoldier->usAniFrame >= gAnimSurfaceDatabase[usAnimSurface].hVideoObject->usNumberOfObjects )
	{
		//int i = 0;
		return;
	}

	pTrav = &(gAnimSurfaceDatabase[usAnimSurface].hVideoObject->pETRLEObject[pSoldier->usAniFrame]);

	*psHeight = (INT16)pTrav->usHeight;
	*psWidth = (INT16)pTrav->usWidth;
}

void GetActualSoldierAnimOffsets( SOLDIERTYPE *pSoldier, INT16 *sOffsetX, INT16 *sOffsetY )
{
	UINT16											 usAnimSurface;
	ETRLEObject *pTrav;

	usAnimSurface = GetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

	if ( usAnimSurface == INVALID_ANIMATION_SURFACE )
	{
		*sOffsetX = (INT16)0;
		*sOffsetY = (INT16)0;

		return;
	}

	if ( gAnimSurfaceDatabase[usAnimSurface].hVideoObject == NULL )
	{
		*sOffsetX = (INT16)0;
		*sOffsetY = (INT16)0;
		return;
	}

	pTrav = &(gAnimSurfaceDatabase[usAnimSurface].hVideoObject->pETRLEObject[pSoldier->usAniFrame]);

	*sOffsetX = (INT16)pTrav->sOffsetX;
	*sOffsetY = (INT16)pTrav->sOffsetY;
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

	pSoldier->sBoundingBoxWidth = sWidth;
	pSoldier->sBoundingBoxHeight = sHeight;
	pSoldier->sBoundingBoxOffsetX = sOffsetX;
	pSoldier->sBoundingBoxOffsetY = sOffsetY;

}

BOOLEAN SOLDIERTYPE::SoldierCarriesTwoHandedWeapon( void )
{
	UINT16 usItem;

	usItem = this->inv[HANDPOS].usItem;

	if ( this->inv[HANDPOS].exists( ) == true && ItemIsTwoHanded(usItem) )
	{
		return(TRUE);
	}

	return(FALSE);
}

extern void HandleItemCooldownFunctions( OBJECTTYPE* itemStack, INT32 deltaSeconds, BOOLEAN isUnderground = TRUE );
// Flugente: Cool down/decay all items in inventory
void SOLDIERTYPE::SoldierInventoryCoolDown( void )
{
	// if we have any active flashlights (in our hands for simplicity), drain their batteries
	// do this check for both hands
	// we do not lower a battery's status all the time - as an INT8, it would reach 0 way to fast. Instead we only have 5% chance of doing so, thereby increasing a battery's life
	if ( Chance( 5 ) )
	{
		UINT16 firstslot = HANDPOS;
		UINT16 lastslot = VESTPOCKPOS;
		for ( UINT16 invpos = firstslot; invpos < lastslot; ++invpos )
		{
			OBJECTTYPE* pObj = &(this->inv[invpos]);

			if ( !pObj || !(pObj->exists( )) )
				// can't use this, end
				continue;

			OBJECTTYPE* pBattery = FindAttachedBatteries( pObj );
			if ( !pBattery )
				continue;

			BOOLEAN flashlightfound = FALSE;
			if ( Item[pObj->usItem].usFlashLightRange )
				flashlightfound = TRUE;

			if ( !flashlightfound )
			{
				attachmentList::iterator iterend = (*pObj)[0]->attachments.end( );
				for ( attachmentList::iterator iter = (*pObj)[0]->attachments.begin( ); iter != iterend; ++iter )
				{
					if ( Item[iter->usItem].usFlashLightRange )
						flashlightfound = TRUE;
				}
			}

			if ( flashlightfound )
			{
				// lose 1 point
				(*pBattery)[0]->data.objectStatus -= 1;

				if ( (*pBattery)[0]->data.objectStatus <= 0 )
				{
					// destroy batteries
					pBattery->RemoveObjectsFromStack( 1 );
					if ( pBattery->exists( ) == false )
						this->inv[HANDPOS].RemoveAttachment( pBattery );
				}
			}
		}
	}

	// handle flashlight. This is necessary in this location, as we need to do this at least once per turn
	this->HandleFlashLights( );

	if ( !gGameExternalOptions.fWeaponOverheating && !UsingFoodSystem() )
		return;

	INT8 numStacks = (INT8)this->inv.size( );											// remember inventorysize, so we don't call size() repeatedly
	extern UINT32 guiLastTacticalRealTime, guiLastStrategicTime;
	UINT32 secondsPassed = 5;//GetJA2Clock() > guiLastTacticalRealTime? (GetJA2Clock() - guiLastTacticalRealTime)/1000 : 0;
	for ( INT8 bLoop = 0; bLoop < numStacks; ++bLoop )									// ... for all items in our inventory ...
	{
		HandleItemCooldownFunctions( &(this->inv[bLoop]), secondsPassed );
	}
}

// Flugente: determine if we can rest our weapon on something. This can only happen when STANDING/CROUCHED. As a result, we get superior handling modifiers (we apply the PRONE modfiers)
BOOLEAN	SOLDIERTYPE::IsWeaponMounted( void )
{
	BOOLEAN applybipod = FALSE;

	// we must be active
	if ( !bActive )
		return(FALSE);

	// we must be in a sector (not travelling)
	if ( !bInSector )
		return(FALSE);
		
	// this is odd - invalid GridNo... well, not mounted then
	if ( TileIsOutOfBounds( this->sGridNo ) )
		return(FALSE);

	// if we are a combat vehicle, our guns are always mounted
	if ( ARMED_VEHICLE( this ) )
		return TRUE;

	// anv: passengers who can shoot can rest their guns
	if ( this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER) )
	{
		if ( !gNewVehicle[pVehicleList[this->iVehicleId].ubVehicleType].VehicleSeats[GetSeatIndexFromSoldier( this )].fBlockedShots )
			return(TRUE);
		else
			return(FALSE);
	}
	// a gun can only be mounted if it's ready. The bonus is only relevant when this is true anyway (firing cth calculations require us to aim).
	// Drawback is that we do not know whether we will the bonus until we raise our gun - but then again the entire 'get behind a rock and then look over it' system isn't exactly complicated to understand.
	else if ( !WeaponReady( this ) )
		return FALSE;

	// if we are prone, then we are 'mounting' our gun on the very floor we are laying upon, which always exist
	if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
		return TRUE;

	// not possible to get this bonus on a roof, as there are no objects on the roof on which we could rest our gun
	if ( this->pathing.bLevel == 1 )
		return(FALSE);

	// we determine the height of the next tile in our direction. Because of the way structures are handled, we sometimes have to take the very tile we're occupying right now
	INT32 nextGridNoinSight = this->sGridNo;
	if ( this->ubDirection == NORTH || this->ubDirection == SOUTHWEST || this->ubDirection == WEST || this->ubDirection == NORTHWEST )
		nextGridNoinSight = NewGridNo( nextGridNoinSight, DirectionInc( this->ubDirection ) );

	INT8 adjacenttileheight = GetTallestStructureHeight( nextGridNoinSight, FALSE );

	// if the tile actually has a bit of height, we can rest our gun on it
	if ( (gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH && (adjacenttileheight == 1 || adjacenttileheight == 2)) ||
		 (adjacenttileheight == 2 && (gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && (gAnimControl[this->usAnimState].uiFlags &(ANIM_ALT_WEAPON_HOLDING)))) )
	{
		// now we really want to check the next tile
		nextGridNoinSight = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );

		// for some nefarious reason, trees also have height 2, so we have to check for that too...
		STRUCTURE * pStructure = FindStructure( nextGridNoinSight, STRUCTURE_TREE );

		if ( !pStructure )
		{
			// for some reason I find EXTREMELY FRUSTRATING, we might get a heigth of 2 on a totally empty tile... so we check if we could occupy the tile
			if ( !IsLocationSittable( nextGridNoinSight, 0 ) )
			{
				// resting our gun on people is allowed sometimes
				UINT16 usPersonID = WhoIsThere2( nextGridNoinSight, this->pathing.bLevel );
				if ( usPersonID == NOBODY )
					applybipod = TRUE;
				else
				{
					SOLDIERTYPE* pSoldier = MercPtrs[usPersonID];

					// anv: vehicles don't mind
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
					{
						applybipod = TRUE;
					}
					// if the other person is an ally and prone
					else if ( this->bSide == pSoldier->bSide && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE )
					{
						// if we are facing the other guy in a 90 degree angle, we can mount our gun on his back
						// Once merc's relationship allows angering mercs through actions of others, add a penalty here
						if ( this->ubDirection == gTwoCCDirection[pSoldier->ubDirection] || this->ubDirection == gTwoCDirection[pSoldier->ubDirection] )
						{
							applybipod = TRUE;

							// Flugente: dynamic opinions
							if (gGameExternalOptions.fDynamicOpinions)
							{
								AddOpinionEvent(MercPtrs[usPersonID]->ubProfile, this->ubProfile, OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS);
							}
						}
					}
				}
			}
		}
	}
	else if ( adjacenttileheight == 4 && (gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH || (gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && (gAnimControl[this->usAnimState].uiFlags &(ANIM_ALT_WEAPON_HOLDING)))) )
	{
		// tile is as high as a building, but there might be a window, we could look through that
		// note that we also check for STRUCTURE_OPEN - the window has to be open (smashed)
		STRUCTURE * pStructure = FindStructure( nextGridNoinSight, STRUCTURE_WALLNWINDOW );

		if ( pStructure )
		{
			if ( (this->ubDirection == SOUTH || this->ubDirection == NORTH)
				 && (pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT || pStructure->ubWallOrientation == INSIDE_TOP_LEFT)
				 && pStructure->fFlags & STRUCTURE_WALLNWINDOW && pStructure->fFlags & STRUCTURE_OPEN )
			{
				applybipod = TRUE;
			}
			else if ( (this->ubDirection == EAST || this->ubDirection == WEST)
					  && (pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT || pStructure->ubWallOrientation == INSIDE_TOP_RIGHT)
					  && pStructure->fFlags & STRUCTURE_WALLNWINDOW && pStructure->fFlags & STRUCTURE_OPEN )
			{
				applybipod = TRUE;
			}
			else if ( (this->ubDirection == SOUTHWEST || this->ubDirection == NORTHWEST || this->ubDirection == SOUTHEAST || this->ubDirection == NORTHEAST)
					  && (pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT || pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT || pStructure->ubWallOrientation == INSIDE_TOP_RIGHT)
					  && pStructure->fFlags & STRUCTURE_WALLNWINDOW && pStructure->fFlags & STRUCTURE_OPEN )
			{
				applybipod = TRUE;
			}
		}
	}
	else if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND && adjacenttileheight == 3 && !(gAnimControl[this->usAnimState].uiFlags &(ANIM_ALT_WEAPON_HOLDING)) )
	{
		// now we really want to check the next tile
		nextGridNoinSight = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );

		// for some nefarious reason, trees also have height 2, so we have to check for that too...
		STRUCTURE * pStructure = FindStructure( nextGridNoinSight, STRUCTURE_TREE );

		if ( !pStructure )
		{
			// for some reason I find EXTREMELY FRUSTRATING, we might get a heigth of 2 on a totally empty tile... so we check if we could occupy the tile
			if ( !IsLocationSittable( nextGridNoinSight, 0 ) )
			{
				// resting our gun on people is allowed sometimes
				UINT16 usPersonID = WhoIsThere2( nextGridNoinSight, this->pathing.bLevel );
				if ( usPersonID == NOBODY )
					applybipod = TRUE;
				else
				{
					SOLDIERTYPE* pSoldier = MercPtrs[usPersonID];

					// anv: vehicles don't mind
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
					{
						applybipod = TRUE;
					}
					// if the other person is an ally and prone
					else if ( this->bSide == pSoldier->bSide && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_CROUCH )
					{
						applybipod = TRUE;

						// Flugente: dynamic opinions
						if (gGameExternalOptions.fDynamicOpinions)
						{
							AddOpinionEvent(MercPtrs[usPersonID]->ubProfile, this->ubProfile, OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS);
						}
					}
				}
			}
		}
	}

	return(applybipod);
}

// Flugente: return weapon currently used
OBJECTTYPE* SOLDIERTYPE::GetUsedWeapon( OBJECTTYPE * pObj )
{
	if ( bWeaponMode == WM_ATTACHED_UB || bWeaponMode == WM_ATTACHED_UB_BURST || bWeaponMode == WM_ATTACHED_UB_AUTO )
	{
		OBJECTTYPE* pObjUnderBarrel = FindAttachedWeapon( pObj, IC_GUN );

		if ( pObjUnderBarrel )
			return(pObjUnderBarrel);
	}
	else  if ( bWeaponMode == WM_ATTACHED_BAYONET )
	{
		OBJECTTYPE* pObjUnderBarrel = FindAttachedWeapon( pObj, IC_BLADE );

		if ( pObjUnderBarrel )
			return(pObjUnderBarrel);
	}

	return(pObj);
}

UINT16 SOLDIERTYPE::GetUsedWeaponNumber( OBJECTTYPE * pObj )
{
	if ( bWeaponMode == WM_ATTACHED_UB || bWeaponMode == WM_ATTACHED_UB_BURST || bWeaponMode == WM_ATTACHED_UB_AUTO )
	{
		UINT16 weaponnr = GetAttachedWeapon( pObj, IC_GUN );

		if ( weaponnr != NONE )
			return(weaponnr);
	}
	else if ( bWeaponMode == WM_ATTACHED_BAYONET )
	{
		UINT16 weaponnr = GetAttachedWeapon( pObj, IC_BLADE );

		if ( weaponnr != NONE )
			return(weaponnr);
	}

	return(pObj->usItem);
}

INT32 SOLDIERTYPE::GetDamageResistance( BOOLEAN fAutoResolve, BOOLEAN fCalcBreathLoss )
{
	INT32 resistance = 0;
	FLOAT breathmodifiermilitia = 1.0;
	FLOAT breathmodifierspecialNPC = 2.0;

	if ( fCalcBreathLoss )
	{
		breathmodifiermilitia = 0.75;
		breathmodifierspecialNPC = 1.0;
	}

	// SANDRO - Damage resistance for Militia
	if ( !fAutoResolve )
	{
		if ( this->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.bGreenMilitiaDamageResistance != 0 )
			resistance += (INT32)(gGameExternalOptions.bGreenMilitiaDamageResistance / breathmodifiermilitia);
		else if ( this->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.bRegularMilitiaDamageResistance != 0 )
			resistance += (INT32)(gGameExternalOptions.bRegularMilitiaDamageResistance / breathmodifiermilitia);
		else if ( this->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.bVeteranMilitiaDamageResistance != 0 )
			resistance += (INT32)(gGameExternalOptions.bVeteranMilitiaDamageResistance / breathmodifiermilitia);
		// bonus for enemy too
		else if ( (this->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR || this->ubSoldierClass == SOLDIER_CLASS_BANDIT ) && gGameExternalOptions.sEnemyAdminDamageResistance != 0 )
			resistance += gGameExternalOptions.sEnemyAdminDamageResistance;
		else if ( this->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularDamageResistance != 0 )
			resistance += gGameExternalOptions.sEnemyRegularDamageResistance;
		else if ( this->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteDamageResistance != 0 )
			resistance += gGameExternalOptions.sEnemyEliteDamageResistance;
		else if ( IsZombie( ) )
		{
			if ( fCalcBreathLoss )
				resistance += gGameExternalOptions.sEnemyZombieBreathDamageResistance;
			else
				resistance += gGameExternalOptions.sEnemyZombieDamageResistance;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - option to make special NPCs stronger - damage resistance
	if ( gGameExternalOptions.usSpecialNPCStronger > 0 )
	{
		switch ( this->ubProfile )
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
			resistance += (INT32)(gGameExternalOptions.usSpecialNPCStronger / breathmodifierspecialNPC);
			break;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// STOMP traits - Bodybuilding damage resistance
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( this, BODYBUILDING_NT ) )
		resistance += gSkillTraitValues.ubBBDamageResistance;
	////////////////////////////////////////////////////////////////////////////////////

	// Flugente: drugs can now have an effect on damage resistance
	resistance += this->newdrugs.size[DRUG_EFFECT_PHYS_RES];

	resistance += this->GetBackgroundValue( BG_RESI_PHYSICAL );

	// frozen targets go down HARD
	if ( this->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		resistance -= 1000;

	// resistance is between -100% and 95%
	resistance = max( -1000, resistance );
	resistance = min( 95, resistance );

	return(resistance);
}

INT8	SOLDIERTYPE::GetHearingBonus( )
{
	INT8 bonus = 0;

	INT8 bSlot = FindHearingAid( this );
	if ( bSlot != -1 )
	{
		// at 81-100% adds +5, at 61-80% adds +4, at 41-60% adds +3, etc.
		bonus += GetHearingRangeBonus( this );	// pSoldier->inv[bSlot][0]->data.objectStatus / 20 + 1;
	}

	if ( DoesMercHaveDisability( this, DEAF ) )
		bonus -= 5;

	if ( NightTime( ) )
		bonus += this->GetBackgroundValue( BG_PERC_HEARING_NIGHT );
	else
		bonus += this->GetBackgroundValue( BG_PERC_HEARING_DAY );

	if ( this->IsRadioListening( ) )
		bonus += gSkillTraitValues.sVOListeningHearingBonus;

	return bonus;
}

INT16	SOLDIERTYPE::GetSightRangeBonus( )
{
	INT16 bonus = 0;

	if ( DoesMercHaveDisability( this, SHORTSIGHTED ) )
		bonus -= 10;

	if ( (gGameExternalOptions.usLowerVisionWhileRunning == 1) || ( gGameExternalOptions.usLowerVisionWhileRunning == 2 && bTeam == gbPlayerNum ) )
	{
		// Flugente: We have to decide depending on the animation we have, otherwise we can cause bugs if we do this after being hit by an explosion etc.
		switch ( this->usAnimState )
		{
		case RUNNING:
		case RUNNING_W_PISTOL:
			bonus -= 25;
			break;
		}
	}

	return bonus;
}

INT16 SOLDIERTYPE::GetSoldierCriticalDamageBonus( void )
{
	INT16 val = 0;

	return val;
}


BOOLEAN SOLDIERTYPE::IsZombie( void )
{
	return(ubSoldierClass == SOLDIER_CLASS_ZOMBIE);
}

// reset the extra stat variables
void	SOLDIERTYPE::ResetExtraStats( )
{
	bExtraStrength = 0;
	bExtraDexterity = 0;
	bExtraAgility = 0;
	bExtraWisdom = 0;
	bExtraExpLevel = 0;
}

// Flugente: inventory bombs can ignite while in mapscreen. Workaround: Damage items and health
void	SOLDIERTYPE::InventoryExplosion( void )
{
	INT8 invsize = (INT8)this->inv.size( );									// remember inventorysize, so we don't call size() repeatedly

	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )							// ... for all items in our inventory ...
	{
		if ( this->inv[bLoop].exists( ) == true )
		{
			OBJECTTYPE * pObj = &(this->inv[bLoop]);							// ... get pointer for this item ...

			if ( pObj != NULL )													// ... if pointer is not obviously useless ...
			{
				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )				// ... there might be multiple items here (item stack), so for each one ...
				{
					INT16 status = (*pObj)[0]->data.objectStatus;
					(*pObj)[0]->data.objectStatus = max( 1, (INT16)(status / 2) );

					// also damage every attachment
					attachmentList::iterator iterend = (*pObj)[i]->attachments.end( );
					for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin( ); iter != iterend; ++iter )
					{
						if ( iter->exists( ) )
						{
							INT16 status = (*iter)[0]->data.objectStatus;
							(*iter)[0]->data.objectStatus = max( 1, (INT16)(status / 2) );

							INT16 rtstatus = (*iter)[0]->data.sRepairThreshold;
							(*iter)[0]->data.sRepairThreshold = max( 1, (INT16)(rtstatus / 2) );
						}
					}
				}
			}
		}
	}

	// now damage our health
	INT8 oldlife = stats.bLife;

	INT16 damage = (INT16)(30 + Random( 20 ));
	if ( stats.bLife - damage < 0 )
		damage = oldlife;

	// We've got a problem if we kill someone outright without him collapsing properly...
	// FIX: We'll adjust our damage, so if we'd kill someone without collapsing first, we lower our damage, to let him collapse
	// After all, this whole thing's a rig up, so there shouldn't be a problem with that
	if ( oldlife >= OKLIFE && oldlife <= damage )
		damage -= (INT16)((5 + Random( 5 )));

	INT16 breathdamage = (INT16)(500 + Random( 1500 ));
	if ( bBreath - breathdamage < 0 )
		breathdamage = bBreath;

	// Play sound
	PlayJA2SampleFromFile( "Sounds\\Explode1.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

	SoldierTakeDamage( 0, damage, breathdamage, TAKE_DAMAGE_EXPLOSION, this->ubID, sGridNo, 0, TRUE );

	if ( stats.bLife <= 0 )
	{
		// FINISH HIM!
		HandleTakeDamageDeath( this, oldlife, TAKE_DAMAGE_BLOODLOSS );
	}
	else if ( stats.bLife < OKLIFE && !bCollapsed )
	{
		// let the target collapse...
		SoldierCollapse( this );
	}
}

// Flugente: do we currently provide ammo (pAmmoSlot) for someone else's (pubId) gun (pGunSlot)?
BOOLEAN		SOLDIERTYPE::IsFeedingExternal( UINT16 * pubId1, UINT16* pGunSlot1, UINT16* pAmmoSlot1, UINT16 * pubId2, UINT16* pGunSlot2, UINT16* pAmmoSlot2 )
{
	// make sure we have to check this...
	if ( gGameExternalOptions.ubExternalFeeding == 0 )
		return(FALSE);

	//  basic check if we are up to this task
	if ( !this->bActive || !this->bInSector || this->stats.bLife < OKLIFE )
		return(FALSE);

	// this is odd - invalid GridNo... well, no feeding then
	if ( TileIsOutOfBounds( this->sGridNo ) )
		return(FALSE);

	BOOLEAN	isFeeding = FALSE;

	UINT16 usGunItem = 0;
	UINT8  usGunCalibre = 0;
	UINT8  usGunAmmoType = 0;

	UINT16 usAmmoItem = 0;
	UINT8  usAmmoCalibre = 0;
	UINT8  usAmmoAmmoType = 0;

	UINT16 usMagIndex = 0;

	BOOLEAN firstgunfound = FALSE;

	// do this check for both hands
	UINT16 firstslot = HANDPOS;
	UINT16 lastslot = VESTPOCKPOS;
	for ( UINT16 invpos = firstslot; invpos < lastslot; ++invpos )
	{
		// do we have ammo in our hands?
		OBJECTTYPE* pAmmoObj = &(this->inv[invpos]);

		if ( !pAmmoObj || !(pAmmoObj->exists( )) || Item[pAmmoObj->usItem].usItemClass != IC_AMMO || (*pAmmoObj)[0]->data.ubShotsLeft <= 0 )
			// can't use this, end
			continue;

		usAmmoItem = pAmmoObj->usItem;

		if ( !HasItemFlag( usAmmoItem, AMMO_BELT ) )
			continue;

		usMagIndex = Item[usAmmoItem].ubClassIndex;

		usAmmoCalibre = Magazine[usMagIndex].ubCalibre;
		usAmmoAmmoType = Magazine[usMagIndex].ubAmmoType;

		// our current stance is important
		UINT8 usOurStance = gAnimControl[this->usAnimState].ubEndHeight;

		// we will check wether one of our teammates is on the gridno we face
		INT32 nextGridNoinSight = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );

		SOLDIERTYPE* pTeamSoldier = NULL;
		INT32 cnt = gTacticalStatus.Team[this->bTeam].bFirstID;
		INT32 lastid = gTacticalStatus.Team[this->bTeam].bLastID;
		for ( pTeamSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pTeamSoldier )
		{
			// check if teamsoldier exists in this sector
			if ( !pTeamSoldier || !pTeamSoldier->bActive || !pTeamSoldier->bInSector || pTeamSoldier->sSectorX != this->sSectorX || pTeamSoldier->sSectorY != this->sSectorY || pTeamSoldier->bSectorZ != this->bSectorZ )
				continue;

			// check if both soldiers are on the same level
			if ( this->pathing.bLevel != pTeamSoldier->pathing.bLevel )
				continue;

			// determine wether we can physically provide ammo to our teammate.
			// check the stance, prone on standing (both ways) doesn't work			
			if ( usOurStance == ANIM_STAND )
			{
				if ( gAnimControl[pTeamSoldier->usAnimState].ubEndHeight != ANIM_STAND && gAnimControl[pTeamSoldier->usAnimState].ubEndHeight != ANIM_CROUCH )
					continue;
			}
			else if ( usOurStance == ANIM_PRONE )
			{
				if ( gAnimControl[pTeamSoldier->usAnimState].ubEndHeight != ANIM_PRONE && gAnimControl[pTeamSoldier->usAnimState].ubEndHeight != ANIM_CROUCH )
					continue;
			}

			// check if we look at our teammate, or look the same way he does, or in the direction between
			BOOLEAN fPositioningOkay = FALSE;
			// the other person must be near
			if ( SpacesAway( this->sGridNo, pTeamSoldier->sGridNo ) == 0 )
			{
				// same tile -> its ourself -> ok
				fPositioningOkay = TRUE;
			}
			else if ( SpacesAway( this->sGridNo, pTeamSoldier->sGridNo ) == 1 )
			{
				// we look at him -> ok
				if ( nextGridNoinSight == pTeamSoldier->sGridNo )
					fPositioningOkay = TRUE;
				else
				{
					// if we look at the same tile, then that's okay too
					INT32 teamsoldiernextGridNoinSight = NewGridNo( pTeamSoldier->sGridNo, DirectionInc( pTeamSoldier->ubDirection ) );

					if ( nextGridNoinSight == teamsoldiernextGridNoinSight )
						fPositioningOkay = TRUE;
					else
					{
						// if we both look in the same direction...
						INT8 teammatedirection = pTeamSoldier->ubDirection;
						INT8 ourdirection = this->ubDirection;

						if ( teammatedirection == ourdirection )
						{
							// if the angle between our teammates sightline and the direct line from us to him is 90 degrees, then we are also able to supply 
							INT8 ourrightdirection = (ourdirection + 2) % NUM_WORLD_DIRECTIONS;
							INT8 ourleftdirection = (ourdirection - 2) % NUM_WORLD_DIRECTIONS;

							if ( NewGridNo( this->sGridNo, DirectionInc( ourrightdirection ) ) == pTeamSoldier->sGridNo || NewGridNo( this->sGridNo, DirectionInc( ourleftdirection ) ) == pTeamSoldier->sGridNo )
								fPositioningOkay = TRUE;
						}
					}
				}
			}

			if ( !fPositioningOkay )
				continue;

			// ok, we are facing a teammate. Check if he has a gun in any hand that still has ammo left
			UINT16 pTeamSoldierfirstslot = HANDPOS;
			UINT16 pTeamSoldierlastslot = VESTPOCKPOS;
			for ( UINT16 teamsoldierinvpos = pTeamSoldierfirstslot; teamsoldierinvpos < pTeamSoldierlastslot; ++teamsoldierinvpos )
			{
				OBJECTTYPE* pObjInHands = &(pTeamSoldier->inv[teamsoldierinvpos]);
				if ( pObjInHands && pObjInHands->exists( ) && Item[pObjInHands->usItem].usItemClass == IC_GUN && (HasItemFlag( pObjInHands->usItem, BELT_FED ) || HasAttachmentOfClass( pObjInHands, AC_FEEDER )) && (*pObjInHands)[0]->data.gun.ubGunShotsLeft > 0 )
				{
					// remember the caliber and type of ammo. They all have to fit
					usGunItem = pObjInHands->usItem;

					usGunCalibre = Weapon[usGunItem].ubCalibre;
					usGunAmmoType = (*pObjInHands)[0]->data.gun.ubGunAmmoType;

					if ( usGunCalibre == usAmmoCalibre && /*usGunMagSize == usAmmoMagSize &&*/ usGunAmmoType == usAmmoAmmoType )
					{
						// same calibre, same magsize, same ammotype. We can serve this guy
						if ( !firstgunfound )
						{
							firstgunfound = TRUE;
							(*pubId1) = cnt;
							(*pGunSlot1) = teamsoldierinvpos;
							(*pAmmoSlot1) = invpos;
							isFeeding = TRUE;
							break;
						}
						else
						{
							(*pubId2) = cnt;
							(*pGunSlot2) = teamsoldierinvpos;
							(*pAmmoSlot2) = invpos;
							isFeeding = TRUE;

							// we really found a second gun. we can only serve 2 guns maximum. lets end this
							return(isFeeding);
						}
					}
				}
			}
		}
	}

	// if set to 1, we do not wether we feed ourself from our inventory
	if ( gGameExternalOptions.ubExternalFeeding < 2 )
		return(isFeeding);

	// if we reach this point, we have checked all our teammates, and we do not provide external feeding for any of them
	// it is possible that we provide external feeding for OURSELF (think of ammo belts in a dedicated LBE slot, or of a gun that requires a separate energy source)
	// first, determine wether we need external feeding for our gun. We do this for both hands, as it is thinkable that someone has 2 one-handed guns with external feeding

	// this determines which slots we'll search for ammo
	UINT16 firstslotforammo = MEDPOCK1POS;
	UINT16 lastslotforammo = MEDPOCK3POS;

	// for robots and AI-controlled soldiers (who don't have any LBE gear), we put a change in here so that ALL their slots are checked for ammo
	if ( this->bTeam != gbPlayerNum || MercPtrs[this->ubID]->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		firstslotforammo = HANDPOS;
		lastslotforammo = NUM_INV_SLOTS;
	}
	else
	{
		// as a merc, the only slots that are valid for external feeding are the 2 medium-sized slots on a vest (because I say so). And that only if the vest is allowed to do that, which we will now check:
		if ( !(this->inv[VESTPOCKPOS].exists( )) || !HasItemFlag( this->inv[VESTPOCKPOS].usItem, AMMO_BELT_VEST ) )
			return(isFeeding);
	}

	UINT16 searchgunfirstslot = HANDPOS;
	UINT16 searchgunlastslot = VESTPOCKPOS;
	for ( UINT16 invpos = searchgunfirstslot; invpos < searchgunlastslot; ++invpos )
	{
		// check our hands for guns
		OBJECTTYPE* pObj = &(this->inv[invpos]);

		UINT16 usGunItem = pObj->usItem;

		if ( !pObj || !(pObj->exists( )) || Item[usGunItem].usItemClass != IC_GUN || !(HasItemFlag( usGunItem, BELT_FED ) || HasAttachmentOfClass( pObj, AC_FEEDER )) || (*pObj)[0]->data.gun.ubGunShotsLeft <= 0 )
			// can't use this, end
			continue;

		// remember the caliber and type of ammo. They all have to fit
		usGunCalibre = Weapon[usGunItem].ubCalibre;
		usGunAmmoType = (*pObj)[0]->data.gun.ubGunAmmoType;

		// now check the inventory for an ammo belt. If we are not from the player team or a robot, we will search the entire inventory
		for ( UINT16 bLoop = firstslotforammo; bLoop < lastslotforammo; ++bLoop )
		{
			if ( this->inv[bLoop].exists( ) == true )
			{
				OBJECTTYPE * pAmmoObj = &(this->inv[bLoop]);							// ... get pointer for this item ...

				if ( pAmmoObj != NULL )													// ... if pointer is not obviously useless ...
				{
					//if ( pAmmoObj->ubNumberOfObjects == 1 )
					{
						usAmmoItem = pAmmoObj->usItem;

						if ( Item[usAmmoItem].usItemClass == IC_AMMO && HasItemFlag( usAmmoItem, AMMO_BELT ) )
						{
							// remember the caliber and type of ammo. They all have to fit
							usMagIndex = Item[usAmmoItem].ubClassIndex;
							usAmmoCalibre = Magazine[usMagIndex].ubCalibre;
							usAmmoAmmoType = Magazine[usMagIndex].ubAmmoType;

							if ( usGunCalibre == usAmmoCalibre && usGunAmmoType == usAmmoAmmoType )
							{
								// same calibre, same ammotype. We can serve this guy
								if ( !firstgunfound )
								{
									firstgunfound = TRUE;
									(*pubId1) = this->ubID;
									(*pGunSlot1) = invpos;
									(*pAmmoSlot1) = bLoop;
									isFeeding = TRUE;
									break;
								}
								else
								{
									(*pubId2) = this->ubID;
									(*pGunSlot2) = invpos;
									(*pAmmoSlot2) = bLoop;
									isFeeding = TRUE;

									// we really found a second gun. we can only serve 2 guns maximum. lets end this
									return(isFeeding);
								}
							}
						}
					}
				}
			}
		}
	}

	return(isFeeding);
}

// Flugente: return first found object with a specific flag from our inventory
OBJECTTYPE* SOLDIERTYPE::GetObjectWithFlag( UINT64 aFlag )
{
	OBJECTTYPE* pObj = NULL;

	INT8 invsize = (INT8)inv.size( );									// remember inventorysize, so we don't call size() repeatedly

	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )						// ... for all items in our inventory ...
	{
		// ... if Item exists and is canteen (that can have drink points) ...
		if ( inv[bLoop].exists( ) == true && HasItemFlag( inv[bLoop].usItem, aFlag ) )
		{
			pObj = &(inv[bLoop]);							// ... get pointer for this item ...

			return(pObj);
		}
	}

	return(pObj);
}

extern INT16 uiNIVSlotType[NUM_INV_SLOTS];

// do we look like a civilian?
BOOLEAN		SOLDIERTYPE::LooksLikeACivilian( void )
{
	// if we have any camo: not covert
	if ( GetWornCamo( this ) > 0 || GetWornUrbanCamo( this ) > 0 || GetWornDesertCamo( this ) > 0 || GetWornSnowCamo( this ) > 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_CAMOFOUND], this->GetName( ) );
		return FALSE;
	}

	if ( UsingNewInventorySystem( ) )
	{
		INT8 invsize = (INT8)this->inv.size( );
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
		{
			if ( this->inv[bLoop].exists( ) )
			{
				/*// if we have a back pack: not covert
				if ( bLoop == BPACKPOCKPOS )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_BACKPACKFOUND], this->GetName( ) );
					return FALSE;
				}*/

				// do not check the LBE itself (we already checked for camo above)
				if ( bLoop >= VESTPOCKPOS && bLoop <= CPACKPOCKPOS )
					continue;

				// seriously? a corpse? of course this is suspicious!
				if ( HasItemFlag( this->inv[bLoop].usItem, CORPSE ) )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_CARRYCORPSEFOUND], this->GetName( ) );
					return FALSE;
				}

				BOOLEAN checkfurther = FALSE;

				// guns/launchers in our hands will always be noticed, even if covert
				if ( (Item[this->inv[bLoop].usItem].usItemClass & (IC_GUN | IC_LAUNCHER)) && (bLoop == HANDPOS || bLoop == SECONDHANDPOS) )
					checkfurther = TRUE;
				// further checks it item is not covert. This means that a gun that has that tag will not be detected if its inside a pocket!
				else if ( !HasItemFlag( this->inv[bLoop].usItem, COVERT ) )
				{
					checkfurther = TRUE;

					// visible slots are always checked if not covert
					if ( bLoop == HANDPOS || bLoop == SECONDHANDPOS || bLoop == GUNSLINGPOCKPOS || bLoop == KNIFEPOCKPOS || bLoop == HELMETPOS || bLoop == VESTPOS || bLoop == LEGPOS || bLoop == HEAD1POS || bLoop == HEAD2POS )
						;
					else
					{
						// check for the pocket the item is in
						// item will be detected if someone looks - check for the LBE item that gave us this slot. If that one is covert, this item is also covert
						UINT8 checkslot = 0;
						switch ( uiNIVSlotType[bLoop] )
						{
						case 2:
							// this is worn LBE gear itself
							break;
						case 3:
							checkslot = VESTPOCKPOS;
							break;
						case 4:
							if ( bLoop == MEDPOCK3POS || bLoop == SMALLPOCK11POS || bLoop == SMALLPOCK12POS || bLoop == SMALLPOCK13POS || bLoop == SMALLPOCK14POS )
								checkslot = LTHIGHPOCKPOS;
							else
								checkslot = RTHIGHPOCKPOS;
							break;
						case 5:
							checkslot = CPACKPOCKPOS;
							break;
						case 6:
							checkslot = BPACKPOCKPOS;
							break;
						default:
							{
								//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEM_SUSPICIOUS], this->GetName(), Item[this->inv[bLoop].usItem].szItemName );
								//return FALSE;
							}
							break;
						}

						// found a slot to check for LBE
						if ( checkslot > 0 )
						{
							// if LBE is covert
							if ( this->inv[checkslot].exists() && HasItemFlag( this->inv[checkslot].usItem, COVERT ) )
								// pass for this item
								checkfurther = FALSE;
						}
					}
				}

				if ( checkfurther )
				{
					// if that item is a gun, explosives, military armour or facewear, we're screwed
					if ( (Item[this->inv[bLoop].usItem].usItemClass & (IC_WEAPON | IC_GRENADE | IC_BOMB)) ||
						 ((Item[this->inv[bLoop].usItem].usItemClass & (IC_ARMOUR)) && !ItemIsLeatherJacket(this->inv[bLoop].usItem) && Armour[Item[this->inv[bLoop].usItem].ubClassIndex].ubProtection > 10) ||
						 (Item[this->inv[bLoop].usItem].nightvisionrangebonus > 0 || Item[this->inv[bLoop].usItem].hearingrangebonus > 0)
						 )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_MILITARYGEARFOUND], this->GetName( ), Item[this->inv[bLoop].usItem].szItemName );
						return FALSE;
					}
				}
			}
		}
	}
	else	// old inventory system. No LBE here, nothing fancy
	{
		INT8 invsize = (INT8)this->inv.size( );
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
		{
			if ( this->inv[bLoop].exists( ) )
			{
				if ( !HasItemFlag( this->inv[bLoop].usItem, COVERT ) )
				{
					// if that item is a gun, explosives, military armour or facewear, we're screwed
					if ( (Item[this->inv[bLoop].usItem].usItemClass & (IC_WEAPON | IC_GRENADE | IC_BOMB)) ||
						 ((Item[this->inv[bLoop].usItem].usItemClass & (IC_ARMOUR)) && !ItemIsLeatherJacket(this->inv[bLoop].usItem) && Armour[Item[this->inv[bLoop].usItem].ubClassIndex].ubProtection > 10) ||
						 (Item[this->inv[bLoop].usItem].nightvisionrangebonus > 0 || Item[this->inv[bLoop].usItem].hearingrangebonus > 0)
						 )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_MILITARYGEARFOUND], this->GetName( ), Item[this->inv[bLoop].usItem].szItemName );
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

// do we look like a soldier?
BOOLEAN		SOLDIERTYPE::LooksLikeASoldier( void )
{
	INT8 invsize = (INT8)this->inv.size( );
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
	{
		if ( this->inv[bLoop].exists( ) )
		{
			// seriously? a corpse? of course this is suspicious!
			if ( HasItemFlag( this->inv[bLoop].usItem, CORPSE ) )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_CARRYCORPSEFOUND], this->GetName( ) );
				return FALSE;
			}
		}
	}

	return TRUE;
}

INT8		SOLDIERTYPE::GetUniformType( )
{
	// we determine wether we are currently wearing civilian or military clothes
	for ( UINT8 i = UNIFORM_ENEMY_ADMIN; i < NUM_UNIFORMS; ++i )
	{
		// both parts have to fit. We cant mix different uniforms and get soldier disguise
		if ( COMPARE_PALETTEREP_ID( this->VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( this->PantsPal, gUniformColors[i].pants ) )
		{
			return i;
		}
	}

	return -1;
}

// is our equipment too good for a soldier?
BOOLEAN		SOLDIERTYPE::EquipmentTooGood( BOOLEAN fCloselook )
{
	// if militia is equipped from sector inventory(and thu by the player itself), then its item selection is no longer bound to any progress calculation
	// we thus canno check for equipment - the only way to find out is to look at this guy sharply, and to eventually realise that this gear did not come from the player
	if ( gGameExternalOptions.fMilitiaUseSectorInventory && this->IsAssassin( ) )
		return FALSE;

	// check the guns in our hands and rifle sling
	// alert if we have more than 2, any of them has too much attachments or they are way too cool
	UINT8 numberofguns = 0;
	UINT8 ubCurrentProgress = CurrentPlayerProgressPercentage( );
	UINT8 maxcoolnessallowed = 1 + ubCurrentProgress / 10;

	INT8 uniformtype = GetUniformType( );

	// adjust max coolness depending on uniform
	// enemy spies get a small bonus here
	switch ( uniformtype )
	{
	case UNIFORM_ENEMY_ADMIN:
		maxcoolnessallowed += 1;
		break;
	case UNIFORM_ENEMY_TROOP:
	case UNIFORM_MILITIA_ROOKIE:
		maxcoolnessallowed += 2;
		break;
	case UNIFORM_ENEMY_ELITE:
	case UNIFORM_MILITIA_REGULAR:
		maxcoolnessallowed += 3;
		break;
	case UNIFORM_MILITIA_ELITE:
		maxcoolnessallowed += 4;
		break;
	default:
		// we do not wear a proper army uniform, uncover us. Note: This should never happen - if this message shows, somewhere, something is wrong
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_UNIFORM_NOORDER], this->GetName( ) );
		return TRUE;
		break;
	}

	if ( UsingNewInventorySystem( ) )
	{
		INT8 invsize = (INT8)this->inv.size( );
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
		{
			if ( this->inv[bLoop].exists( ) )
			{
				// if we have a back pack: not covert
				if ( bLoop == BPACKPOCKPOS )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_BACKPACKFOUND], this->GetName( ) );
					return TRUE;
				}

				// guns/launchers in our hands will always be noticed, even if covert, so we need to check them later
				if ( bLoop == HANDPOS || bLoop == SECONDHANDPOS )
					;
				// other covert items are simply ignored
				else if ( HasItemFlag( this->inv[bLoop].usItem, COVERT ) )
					continue;
				// further checks it item is not covert. This means that an item that has that tag will not be detected if it is inside a pocket!
				else if ( (bLoop == GUNSLINGPOCKPOS || bLoop == HELMETPOS || bLoop == VESTPOS || bLoop == LEGPOS || bLoop == HEAD1POS || bLoop == HEAD2POS || bLoop == KNIFEPOCKPOS) )
					;
				else
				{
					// if we're not that close, we wont even see this, so don't check
					if ( !fCloselook )
						continue;

					// item will be detected if someone looks - check for the LBE item that gave us this slot. If that one is covert, this item is also covert
					UINT8 checkslot = 0;
					switch ( uiNIVSlotType[bLoop] )
					{
					case 2:
						// this is worn LBE gear itself
						break;
					case 3:
						checkslot = VESTPOCKPOS;
						break;
					case 4:
						if ( bLoop == MEDPOCK3POS || bLoop == SMALLPOCK11POS || bLoop == SMALLPOCK12POS || bLoop == SMALLPOCK13POS || bLoop == SMALLPOCK14POS )
							checkslot = LTHIGHPOCKPOS;
						else
							checkslot = RTHIGHPOCKPOS;
						break;
					case 5:
						checkslot = CPACKPOCKPOS;
						break;
					default:
					{
							   //ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEM_SUSPICIOUS], this->GetName(), Item[this->inv[bLoop].usItem].szItemName );
							   //return FALSE;
					}
						break;
					}

					// found a slot to check for LBE
					if ( checkslot > 0 )
					{
						// if LBE is covert
						if ( this->inv[checkslot].exists( ) && HasItemFlag( this->inv[checkslot].usItem, COVERT ) )
							// pass for this item
							continue;
					}
				}

				// if that item is a gun, explosives, military armour or facewear, investigate further
				if ( (Item[this->inv[bLoop].usItem].usItemClass & (IC_GUN | IC_LAUNCHER | IC_ARMOUR | IC_FACE)) )
				{
					if ( Item[this->inv[bLoop].usItem].usItemClass & (IC_GUN | IC_LAUNCHER) && !HasItemFlag( this->inv[bLoop].usItem, COVERT ) )
					{
						++numberofguns;

						if ( numberofguns > 2 )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOOMANYGUNS], this->GetName( ) );
							return TRUE;
						}
					}

					OBJECTTYPE * pObj = &(this->inv[bLoop]);								// ... get pointer for this item ...

					if ( pObj != NULL )
					{
						for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )				// ... there might be multiple items here (item stack), so for each one ...
						{
							// loop over every item and its attachments
							if ( Item[pObj->usItem].ubCoolness > maxcoolnessallowed )
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEMSTOOGOOD], this->GetName( ), Item[pObj->usItem].szItemName, pCountryNames[COUNTRY_NOUN] );
								return TRUE;
							}

							UINT8 numberofattachments = 0;
							// for every objects, we also have to check wether there are weapon attachments (eg. underbarrel grenade launchers), and cool them down too
							attachmentList::iterator iterend = (*pObj)[i]->attachments.end( );
							for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin( ); iter != iterend; ++iter )
							{
								if ( iter->exists( ) )
								{
									// loop over every item and its attachments
									if ( Item[iter->usItem].ubCoolness > maxcoolnessallowed )
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEMSTOOGOOD], this->GetName( ), Item[iter->usItem].szItemName, pCountryNames[COUNTRY_NOUN] );
										return TRUE;
									}

									++numberofattachments;
									
									// no ordinary soldier is allowed that many attachments -> not covert
									if ( fCloselook && numberofattachments > gGameExternalOptions.iMaxEnemyAttachments )
									{
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOOMANYATTACHMENTS], this->GetName( ), Item[pObj->usItem].szItemName );
										return TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else	// old inventory system. No LBE here, nothing fancy
	{
		INT8 invsize = (INT8)this->inv.size( );
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
		{
			if ( this->inv[bLoop].exists( ) )
			{
				// if that item is a gun, explosives, military armour or facewear, investigate further
				if ( !HasItemFlag( this->inv[bLoop].usItem, COVERT ) && (Item[this->inv[bLoop].usItem].usItemClass & (IC_GUN | IC_LAUNCHER | IC_ARMOUR | IC_FACE)) )
				{
					if ( (Item[this->inv[bLoop].usItem].usItemClass & (IC_GUN | IC_LAUNCHER)) )
					{
						++numberofguns;

						if ( numberofguns > 2 )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOOMANYGUNS], this->GetName( ) );
							return TRUE;
						}

						OBJECTTYPE * pObj = &(this->inv[bLoop]);								// ... get pointer for this item ...

						if ( pObj != NULL )
						{
							for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )				// ... there might be multiple items here (item stack), so for each one ...
							{
								// loop over every item and its attachments
								if ( Item[pObj->usItem].ubCoolness > maxcoolnessallowed )
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEMSTOOGOOD], this->GetName( ), Item[pObj->usItem].szItemName, pCountryNames[COUNTRY_NOUN] );
									return TRUE;
								}

								UINT8 numberofattachments = 0;
								// for every objects, we also have to check wether there are weapon attachments (eg. underbarrel grenade launchers), and cool them down too
								attachmentList::iterator iterend = (*pObj)[i]->attachments.end( );
								for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin( ); iter != iterend; ++iter )
								{
									if ( iter->exists( ) )
									{
										// loop over every item and its attachments
										if ( Item[iter->usItem].ubCoolness > maxcoolnessallowed )
										{
											ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ITEMSTOOGOOD], this->GetName( ), Item[iter->usItem].szItemName, pCountryNames[COUNTRY_NOUN] );
											return TRUE;
										}

										++numberofattachments;
									}
								}
								
								// no ordinary soldier is allowed that many attachments > not covert
								if ( fCloselook && numberofattachments > gGameExternalOptions.iMaxEnemyAttachments )
								{
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOOMANYATTACHMENTS], this->GetName( ), Item[pObj->usItem].szItemName );
									return TRUE;
								}
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}


// are we in covert mode? we need to have the correct flag set, and not wear anything suspicious, or behave in a suspicious way
BOOLEAN		SOLDIERTYPE::SeemsLegit( UINT16 ubObserverID )
{
	SOLDIERTYPE* pSoldier = MercPtrs[ubObserverID];

	if ( !pSoldier )
		return TRUE;

	// rftr: turncoats ignore suspicious people/behaviour
	if (gSkillTraitValues.fCOTurncoats && (pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT))
		return TRUE;

	// if we don't have the Flag: not covert
	// important: no messages up to this point. the function will get called a lot, up to this point there is nothing unusual
	if ( !(this->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER)) )
		return FALSE;

	// if we perform suspicious actions, we are easier to uncover for a short time (but not by ourselves if we test the disguise)
	if ( ubObserverID != this->ubID && this->usSoldierFlagMask & SOLDIER_COVERT_TEMPORARY_OVERT )
	{
		// if enough time has passed, or we have spend enough AP, lose the flag
		if ( this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_APS] == 0 || GetWorldTotalSeconds( ) >= this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_SECONDS] )
		{
			this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_SECONDS] = 0;
			this->usSkillCooldown[SOLDIER_COOLDOWN_COVERTOPS_TEMPORARYOVERT_APS] = 0;
			this->usSoldierFlagMask &= ~SOLDIER_COVERT_TEMPORARY_OVERT;
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ACTIVITIES], this->GetName( ) );
			return FALSE;
		}
	}

	// if we are trying to dress like a civilian, but aren't successful: not covert
	if ( this->usSoldierFlagMask & SOLDIER_COVERT_CIV && !(this->LooksLikeACivilian( )) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NO_CIV], this->GetName( ) );
		return FALSE;
	}

	// if we are trying to dress like a soldier, but aren't successful: not covert
	if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER && !(this->LooksLikeASoldier( )) )
	{
		return FALSE;
	}

	UINT8 covertlevel = NUM_SKILL_TRAITS( this, COVERT_NT );	// our level in covert operations
	INT32 distance = PythSpacesAway( this->sGridNo, pSoldier->sGridNo );

	// if we are closer than this, our cover will always break if we do not have the skill
	// if we have the skill, our cover will blow if we dress up as a soldier, but not if we are dressed like a civilian
	INT32 discoverrange = gSkillTraitValues.sCOCloseDetectionRange;

	if ( ubObserverID != this->ubID && distance < discoverrange )
	{
		switch ( covertlevel )
		{
		case 2:
			// a covert ops expert can get as close as he wants, even dressed up as a soldier, without arousing suspicion
			// exceptions: we are discovered if we are close and bleeding, or if we are drunk while dressed as a soldier
			{
				// if we are openly bleeding: not covert
				if ( gSkillTraitValues.fCODetectIfBleeding && this->bBleeding > 0 )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_BLEEDING], this->GetName( ) );
					return FALSE;
				}

				if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER && GetDrunkLevel( this ) != SOBER )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_DRUNKEN_SOLDIER], this->GetName( ) );
					return FALSE;
				}
			}
			break;
		case 1:
			// at lvl covert ops, we can be discovered if we are too close to the enemy and bleed or dressed up as a soldier
			// however, if we are dressed up as a civilian, we can get as close as we like, we won't be discovered
			{
				// if we are openly bleeding: not covert
				if ( gSkillTraitValues.fCODetectIfBleeding && this->bBleeding > 0 )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_BLEEDING], this->GetName( ) );
					return FALSE;
				}

				if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOO_CLOSE], this->GetName( ) );
					return FALSE;
				}
			}
			break;
		case 0:
		default:
			// without the covert ops skill, we can only dress up as civilians. We will be discovered if we get too close to the enemy
			// exception: special NPCs and EPCs can still get close (the Kulbas, for example, ARE civilians, so they apply)
			if ( (this->usSoldierFlagMask & SOLDIER_COVERT_NPC_SPECIAL) == 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOO_CLOSE], this->GetName( ) );
				return FALSE;
			}
			break;
		}

		// if we are disguised as a soldier, elites and officers can uncover us if they are close
		if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER && distance < gSkillTraitValues.usCOEliteUncoverRadius && EffectiveExpLevel( pSoldier ) >= EffectiveExpLevel( this ) + covertlevel )
		{
			// officers can uncover us even if we are disguised as an elite
			if ( pSoldier->usSoldierFlagMask & SOLDIER_ENEMY_OFFICER )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOO_CLOSE_TO_OFFICER], this->GetName( ) );
				return FALSE;
			}

			// elites uncover us if we a disguised as an admin or regular
			if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && GetUniformType( ) < UNIFORM_ENEMY_ELITE )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TOO_CLOSE_TO_ELITE], this->GetName( ) );
				return FALSE;
			}
		}
	}

	if ( this->usSoldierFlagMask & SOLDIER_COVERT_CIV )
	{
		// civilians are suspicious if they are found in certain sectors. Especially at night
		// sector specific value:
		// 0 - civilians are always ok
		// 1 - civilians are suspicious at night
		// 2 - civilians are always suspicious
		// if underground, we still use the surface value

		UINT8 ubSectorId = SECTOR( this->sSectorX, this->sSectorY );
		UINT8 sectordata = SectorExternalData[ubSectorId][0].usCurfewValue;
		
		if ( sectordata > 1 )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_CURFEW_BROKEN], this->GetName( ) );
			return FALSE;
		}
		// is it night?
		else if ( sectordata == 1 && GetTimeOfDayAmbientLightLevel( ) < NORMAL_LIGHTLEVEL_DAY + 2 )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_CURFEW_BROKEN_NIGHT], this->GetName( ) );
			return FALSE;
		}

		// do this check only if we are in the currently loaded sector
		if ( this->sSectorX == gWorldSectorX && this->sSectorY == gWorldSectorY && this->bSectorZ == gbWorldSectorZ )
		{
			// check wether we are around a fresh corpse - this will make us much more suspicious
			INT32				cnt;
			ROTTING_CORPSE *	pCorpse;
			for ( cnt = 0; cnt < giNumRottingCorpse; ++cnt )
			{
				pCorpse = &( gRottingCorpse[cnt] );

				if ( pCorpse && pCorpse->fActivated && pCorpse->def.ubAIWarningValue > 0 && PythSpacesAway( this->sGridNo, pCorpse->def.sGridNo ) <= 5 )
				{
					// check: is this corpse that of an ally of the observing soldier?
					BOOLEAN fCorpseOFAlly = FALSE;
					if ( pSoldier->bTeam == ENEMY_TEAM )
					{
						// check wether corpse was one of soldier's allies
						for ( UINT8 i = UNIFORM_ENEMY_ADMIN; i <= UNIFORM_ENEMY_ELITE; ++i )
						{
							if ( COMPARE_PALETTEREP_ID( pCorpse->def.VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( pCorpse->def.PantsPal, gUniformColors[i].pants ) )
							{
								fCorpseOFAlly = TRUE;
								break;
							}
						}
					}
					else if ( pSoldier->bTeam == OUR_TEAM || pSoldier->bTeam == MILITIA_TEAM )
					{
						// check wether corpse was one of soldier's allies					
						for ( UINT8 i = UNIFORM_MILITIA_ROOKIE; i <= UNIFORM_MILITIA_ELITE; ++i )
						{
							if ( COMPARE_PALETTEREP_ID( pCorpse->def.VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( pCorpse->def.PantsPal, gUniformColors[i].pants ) )
							{
								fCorpseOFAlly = TRUE;
								break;
							}
						}
					}

					// a corpse was found near our position. If the soldier observing us can see it, he will be alarmed 
					if ( fCorpseOFAlly && SoldierTo3DLocationLineOfSightTest( pSoldier, pCorpse->def.sGridNo, pCorpse->def.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
					{
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NEAR_CORPSE], this->GetName() );
						return FALSE;
					}
				}
			}
		}
	}

	if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER )
	{
		// if our equipment is too good, that is suspicious... not covert!
		if ( this->EquipmentTooGood( (distance < discoverrange) ) )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_SUSPICIOUS_EQUIPMENT], this->GetName( ) );
			return FALSE;
		}

		// do this check only if we are in the currently loaded sector
		if ( this->sSectorX == gWorldSectorX && this->sSectorY == gWorldSectorY && this->bSectorZ == gbWorldSectorZ )
		{
			// are we targeting a buddy of our observer?
			if ( this->ubTargetID != NOBODY && MercPtrs[this->ubTargetID] && MercPtrs[this->ubTargetID]->bTeam == pSoldier->bTeam )
			{
				// if we are aiming at a soldier, others will notice our intent... not covert!
				if ( WeaponReady( this ) )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TARGETTING_SOLDIER], this->GetName(), MercPtrs[this->ubTargetID]->GetName() );
					return FALSE;
				}
			}

			// even as a soldier, we will be caught around fresh corpses
			// assassins will not be uncovered around corpses, as the AI cannot willingly evade them... one could 'ward' against assassins by surrounding yourself with fresh corpses
			if ( distance < gSkillTraitValues.sCOCloseDetectionRangeSoldierCorpse && !this->IsAssassin() )
			{
				// check wether we are around a fresh corpse - this will make us much more suspicious
				// I deem this necessary, to avoid cheap exploits by nefarious players :-)
				INT32				cnt;
				ROTTING_CORPSE *	pCorpse;
				for ( cnt = 0; cnt < giNumRottingCorpse; ++cnt )
				{
					pCorpse = &( gRottingCorpse[cnt] );

					if ( pCorpse && pCorpse->fActivated && pCorpse->def.ubAIWarningValue > 0 && PythSpacesAway( this->sGridNo, pCorpse->def.sGridNo ) <= 5 )
					{
						// check: is this corpse that of an ally of the observing soldier?
						BOOLEAN fCorpseOFAlly = FALSE;
						if ( pSoldier->bTeam == ENEMY_TEAM )
						{
							// check wether corpse was one of soldier's allies
							for ( UINT8 i = UNIFORM_ENEMY_ADMIN; i <= UNIFORM_ENEMY_ELITE; ++i )
							{
								if ( COMPARE_PALETTEREP_ID( pCorpse->def.VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( pCorpse->def.PantsPal, gUniformColors[i].pants ) )
								{
									fCorpseOFAlly = TRUE;
									break;
								}
							}
						}
						else if ( pSoldier->bTeam == OUR_TEAM || pSoldier->bTeam == MILITIA_TEAM )
						{
							// check wether corpse was one of soldier's allies					
							for ( UINT8 i = UNIFORM_MILITIA_ROOKIE; i <= UNIFORM_MILITIA_ELITE; ++i )
							{
								if ( COMPARE_PALETTEREP_ID( pCorpse->def.VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( pCorpse->def.PantsPal, gUniformColors[i].pants ) )
								{
									fCorpseOFAlly = TRUE;
									break;
								}
							}
						}

						// a corpse was found near our position. If the soldier observing us can see it, he will be alarmed 
						if ( fCorpseOFAlly && SoldierTo3DLocationLineOfSightTest( pSoldier, pCorpse->def.sGridNo, pCorpse->def.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NEAR_CORPSE], this->GetName() );
							return FALSE;
						}
					}
				}
			}
		}
	}

	// uncover if merc is using flashlight and alert is raised
	if ( pSoldier->bTeam == ENEMY_TEAM &&
		 pSoldier->aiData.bAlertStatus >= STATUS_RED &&
		 (NightTime( ) || this->bSectorZ > 0) &&
		 this->GetBestEquippedFlashLightRange( ) > 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s has a flashlight!", this->GetName( ) );
		return FALSE;
	}

	return TRUE;
}

// do we recognize someone else as a combatant?
BOOLEAN		SOLDIERTYPE::RecognizeAsCombatant( UINT16 ubTargetID )
{
	// this will only work with the new trait system
	if ( !gGameOptions.fNewTraitSystem )
		return TRUE;

	if ( ubTargetID == NOBODY )
		return TRUE;

	SOLDIERTYPE* pSoldier = MercPtrs[ubTargetID];

	if ( !pSoldier )
		return TRUE;

	// zombies don't care about disguises
	if ( IsZombie( ) )
		return TRUE;

	// not in covert mode: we recognize him
	if ( (pSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER)) == 0 )
		return TRUE;

	// neutral characters just dont care
	if ( this->aiData.bNeutral )
		return TRUE;

	// check for for vehicles and creatures... weird things happen
	if ( IsVehicle( pSoldier ) || pSoldier->bTeam == CREATURE_TEAM || this->bTeam == CREATURE_TEAM )
		return TRUE;

	// if from same team, do not uncover
	if ( this->bTeam == pSoldier->bTeam || this->bSide == pSoldier->bSide )
		return TRUE;

	// hack: if this is attacking us at this very moment by punching, do not recognize him...
	// this resolves the problem that we attack someone from behind and kill him instantly, but the game mechanic forces him to turn before
	// only allow this if we are not yet alerted (we are suprised, so we don't recognize him in the moment of the attack)
	// also: only allow if he's next to us
	if ( this->aiData.bAlertStatus < STATUS_RED && pSoldier->ubTargetID == this->ubID )
	{
		INT32 nextGridNoinSight = NewGridNo( pSoldier->sGridNo, DirectionInc( pSoldier->ubDirection ) );
		if ( nextGridNoinSight == this->sGridNo && this->pathing.bLevel == pSoldier->pathing.bLevel )
		{
			if ( pSoldier->usAnimState == PUNCH )
				return FALSE;
			else if ( pSoldier->usAnimState == PUNCH_BREATH )
				return TRUE;
		}
	}

	// campaign stats
	if ( pSoldier->bTeam == ENEMY_TEAM )
		gCurrentIncident.usIncidentFlags |= INCIDENT_SPYACTION_ENEMY;
	else
		gCurrentIncident.usIncidentFlags |= INCIDENT_SPYACTION_PLAYERSIDE;

	// do we recognize this guy as an enemy?
	if ( !pSoldier->SeemsLegit( this->ubID ) )
	{
		// aha, he/she's a spy! Blow cover
		if ( pSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER) )
		{
			pSoldier->LooseDisguise();

			if ( gSkillTraitValues.fCOStripIfUncovered )
				pSoldier->Strip();

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_UNCOVERED], this->GetName(), pSoldier->GetName()  );

			// we have uncovered a spy! Get alerted, if we aren't already
			if ( this->aiData.bAlertStatus < STATUS_BLACK )
				this->aiData.bAlertStatus = STATUS_BLACK;

			// reset our sight of this guy
			this->aiData.bOppList[pSoldier->ubID] = NOT_HEARD_OR_SEEN;

			ManSeesMan( this, pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel, 0, 0 );

			// campaign stats
			gCurrentIncident.usIncidentFlags |= INCIDENT_SPYACTION_UNCOVERED;
		}

		return TRUE;
	}

	return FALSE;
}

// loose covert property
void	SOLDIERTYPE::LooseDisguise( void )
{
	// loose any covert flags
	this->usSoldierFlagMask &= ~(SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER | SOLDIER_COVERT_NPC_SPECIAL);

	// rehandle sight for everybody
	SOLDIERTYPE*		pSoldier;
	UINT16 iLoop = gTacticalStatus.Team[OUR_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[iLoop]; iLoop <= gTacticalStatus.Team[CIV_TEAM].bLastID; ++iLoop, ++pSoldier )
	{
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife > 0 )
		{
			RecalculateOppCntsDueToNoLongerNeutral( pSoldier );
		}
	}
}

void SOLDIERTYPE::Disguise()
{
	// this will only work with the new trait system
	if (!gGameOptions.fNewTraitSystem)
		return;

	// check if we already disguised
	if( this->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER | SOLDIER_COVERT_NPC_SPECIAL) )
		return;

	// check that soldier is active and in sector
	if ( !this->bActive || !this->bInSector )
		return;

	// if this flag is set, do not apply the disgusie properties
	if ( this->usSoldierFlagMask2 & SOLDIER_COVERT_NOREDISGUISE )
		return;

	ApplyCovert( FALSE );
}

void	SOLDIERTYPE::ApplyCovert( BOOLEAN aWithMessage )
{
	// check that we have correct clothes
	if ( this->usSoldierFlagMask & SOLDIER_NEW_VEST && this->usSoldierFlagMask & SOLDIER_NEW_PANTS )
	{
		// first, remove the covert flags, and then reapply the correct ones, in case we switch between civilian and military clothes
		this->usSoldierFlagMask &= ~(SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER);

		// if we apply the disguise property, remove the marker that we don't want this to happen
		// the idea is that if we explicitly remove a disguise, but not our new colours, we don't want to regain the disguise
		// we can then lose this marker again if we explicitly put on a disguise
		this->usSoldierFlagMask2 &= ~SOLDIER_COVERT_NOREDISGUISE;

		// we can only disguise successfully if we are not seen
		if ( !EnemySeenSoldierRecently( this ) )
		{
			// we now have to determine wether we are currently wearing civilian or military clothes
			for ( UINT8 i = UNIFORM_ENEMY_ADMIN; i <= UNIFORM_ENEMY_ELITE; ++i )
			{
				// both parts have to fit. We cant mix different uniforms and get soldier disguise
				if ( COMPARE_PALETTEREP_ID( this->VestPal, gUniformColors[i].vest ) && COMPARE_PALETTEREP_ID( this->PantsPal, gUniformColors[i].pants ) )
				{
					this->usSoldierFlagMask |= SOLDIER_COVERT_SOLDIER;

					if ( aWithMessage && this->bTeam == OUR_TEAM )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_DISGUISED_AS_SOLDIER], this->GetName( ) );

					break;
				}
			}

			// if not dressed as a soldier, we must be dressed as a civilian
			if ( !(this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER) )
			{
				this->usSoldierFlagMask |= SOLDIER_COVERT_CIV;

				if ( aWithMessage && this->bTeam == OUR_TEAM )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_DISGUISED_AS_CIVILIAN], this->GetName( ) );
			}
		}
		
		// reevaluate sight - otherwise we could hide by changing clothes in plain sight!
		OtherTeamsLookForMan( this );
	}
}

// undisguise or take off any clothes item and switch back to original clothes
// no - this function does not do what you think it does. Leave Fox alone, you perv.
void	SOLDIERTYPE::Strip()
{
	// if covert, loose that ability
	if ( this->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER) )
	{
		LooseDisguise( );

		// if we explicitly lose the disguise property, add a flag so that we aren't redisguised again immediately
		this->usSoldierFlagMask2 |= SOLDIER_COVERT_NOREDISGUISE;
	}
	// if already not covert, take off clothes
	else if ( this->usSoldierFlagMask & (SOLDIER_NEW_VEST|SOLDIER_NEW_PANTS) )
	{
		// if we have undamaged clothes, spawn them, the graphic will be removed anyway
		if ( (this->usSoldierFlagMask & SOLDIER_NEW_VEST) && !(this->usSoldierFlagMask & SOLDIER_DAMAGED_VEST) )
		{
			UINT16 vestitem = 0;
			if ( GetFirstClothesItemWithSpecificData( &vestitem, this->VestPal, "blank" ) )
			{
				CreateItem( vestitem, 100, &gTempObject );
				if ( !AutoPlaceObject( this, &gTempObject, FALSE ) )
					AddItemToPool( this->sGridNo, &gTempObject, 1, this->pathing.bLevel, 0, -1 );
			}
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NO_CLOTHES_ITEM] );
		}

		if ( (this->usSoldierFlagMask & SOLDIER_NEW_PANTS) && !(this->usSoldierFlagMask & SOLDIER_DAMAGED_PANTS) )
		{
			UINT16 pantsitem = 0;
			if ( GetFirstClothesItemWithSpecificData( &pantsitem, "blank", this->PantsPal ) )
			{
				CreateItem( pantsitem, 100, &gTempObject );
				if ( !AutoPlaceObject( this, &gTempObject, FALSE ) )
					AddItemToPool(this->sGridNo, &gTempObject, 1, this->pathing.bLevel, 0, -1);
			}
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NO_CLOTHES_ITEM] );
		}

		// loose any clothes flags
		this->usSoldierFlagMask &= ~(SOLDIER_NEW_VEST | SOLDIER_NEW_PANTS);

		// show our true colours
		UINT16 usPaletteAnimSurface = LoadSoldierAnimationSurface( this, this->usAnimState );

		if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
		{
			if ( this->bTeam == OUR_TEAM )
			{
				UINT8				ubProfileIndex;
				MERCPROFILESTRUCT * pProfile;

				ubProfileIndex = this->ubProfile;
				pProfile = &(gMercProfiles[ubProfileIndex]);

				SET_PALETTEREP_ID( this->VestPal, pProfile->VEST );
				SET_PALETTEREP_ID( this->PantsPal, pProfile->PANTS );
			}
			else if ( this->usSoldierFlagMask & SOLDIER_ASSASSIN )
			{
				SET_PALETTEREP_ID( this->VestPal, gUniformColors[UNIFORM_ENEMY_ELITE].vest );
				SET_PALETTEREP_ID( this->PantsPal, gUniformColors[UNIFORM_ENEMY_ELITE].pants );
			}

			// Use palette from HVOBJECT, then use substitution for pants, etc
			memcpy( this->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof(this->p8BPPPalette) * 256 );

			SetPaletteReplacement( this->p8BPPPalette, this->HeadPal );
			SetPaletteReplacement( this->p8BPPPalette, this->VestPal );
			SetPaletteReplacement( this->p8BPPPalette, this->PantsPal );
			SetPaletteReplacement( this->p8BPPPalette, this->SkinPal );

			this->CreateSoldierPalettes( );
		}
	}
	else
	{
		// if the player is an annoying little perv, tell them so, girls!
		// Flugente: additional dialogue
		AdditionalTacticalCharacterDialogue_CallsLua(this, ADE_SEXUALHARASSMENT );
		this->aiData.bMorale = max( 0, this->aiData.bMorale - 1 );
	}
}

// check wether our disguise is any good
void		SOLDIERTYPE::SpySelfTest( )
{
	if ( SeemsLegit( this->ubID ) )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TEST_OK], this->GetName( ) );
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_TEST_FAIL], this->GetName( ) );
}

// can we process prisoners in this sector?
BOOLEAN		SOLDIERTYPE::CanProcessPrisoners( )
{
	if ( stats.bLife < OKLIFE )
		return FALSE;

	// Is there a prison in this sector?
	BOOLEAN prisonhere = FALSE;
	for ( UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt )
	{
		// Is this facility here?
		if ( gFacilityLocations[SECTOR( this->sSectorX, this->sSectorY )][cnt].fFacilityHere )
		{
			// we determine wether this is a prison by checking for usPrisonBaseLimit
			if ( gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0 )
			{
				prisonhere = TRUE;
				break;
			}
		}
	}

	if ( !prisonhere )
		return FALSE;

	// Are there any prisoners in this prison? note that there are no underground prisons
	if ( !this->bSectorZ )
	{
		SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR( this->sSectorX, this->sSectorY )]);

		INT16 aPrisoners[PRISONER_MAX] = {0};
		if ( GetNumberOfPrisoners( pSectorInfo, aPrisoners ) > 0 )
			return TRUE;
	}

	return FALSE;
}

UINT32		SOLDIERTYPE::GetSurrenderStrength( )
{
	if ( this->stats.bLife < OKLIFE || this->flags.fMercAsleep || this->bCollapsed || (this->usSoldierFlagMask & SOLDIER_POW) )
		return 0;

	UINT32 value = 100 + 10 * EffectiveExpLevel( this ) + EffectiveStrength( this, FALSE ) + 3 * EffectiveMarksmanship( this ) + EffectiveLeadership( this ) / 4;

	ReducePointsForFatigue( this, &value );

	value = (value * this->stats.bLife / this->stats.bLifeMax);

	value = value * (5 + sqrt( (double)max( 1, this->aiData.bMorale ) )) / 15;

	// adjust for type of soldier
	if ( this->ubSoldierClass == SOLDIER_CLASS_ELITE || this->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA || this->ubSoldierClass == SOLDIER_CLASS_ROBOT )
		value *= 1.5f;
	else if ( this->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR || this->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA || this->ubSoldierClass == SOLDIER_CLASS_BANDIT )
		value *= 0.75f;

	// tanks won't surrender that easy
	if ( ARMED_VEHICLE( this ) )
		value *= 10;

	return value;
}

// used for an enemy liberating fellow prisoners 
BOOLEAN		SOLDIERTYPE::FreePrisoner( )
{
	// we can only free people we are facing
	INT32 nextGridNoinSight = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );

	UINT16 target = WhoIsThere2( nextGridNoinSight, this->pathing.bLevel );

	// is there somebody?
	if ( target != NOBODY )
	{
		SOLDIERTYPE* pSoldier = MercPtrs[target];

		// if he is captured, free him!
		// note that this would also work for prisoner civs that we spawn in our prisons. All needed would be commanding the AI to get there
		if ( pSoldier->usSoldierFlagMask & (SOLDIER_POW | SOLDIER_POW_PRISON) )
		{
			pSoldier->usSoldierFlagMask &= ~(SOLDIER_POW | SOLDIER_POW_PRISON);

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_X_FREES_Y], this->GetName( ), pSoldier->GetName( ) );

			// alert both soldiers
			this->aiData.bAlertStatus = max( this->aiData.bAlertStatus, STATUS_RED );
			pSoldier->aiData.bAlertStatus = max( pSoldier->aiData.bAlertStatus, STATUS_RED );

			return TRUE;
		}
	}

	return FALSE;
}

// can this guy be captured (by handcuffing or asking him to surrender)?
BOOLEAN		SOLDIERTYPE::CanBeCaptured( )
{
	// if this guy is not already handcuffed, and is not an NPC
	if ( !(this->usSoldierFlagMask & SOLDIER_POW) && this->ubProfile == NO_PROFILE )
	{
		// armed vehicles and robots cannot be captured
		if ( ARMED_VEHICLE(this) || ENEMYROBOT(this) )
			return FALSE;

		// enemies can be captured
		if ( this->bTeam == ENEMY_TEAM )
			return TRUE;

		// bandits can be captured
		if ( this->bTeam == CREATURE_TEAM && this->ubSoldierClass == SOLDIER_CLASS_BANDIT )
			return TRUE;

		// civilians can be captured if their faction can, and if they are hostile
		if ( this->bTeam == CIV_TEAM && zCivGroupName[this->ubCivilianGroup].fCanBeCaptured )
		{
			if ( !this->aiData.bNeutral && this->bSide == 1 )
				return TRUE;
		}
	}
	
	return FALSE;
}

// Flugente: scuba gear
BOOLEAN		SOLDIERTYPE::UsesScubaGear( )
{
	if ( !this->MercInHighWater( ) )
		return FALSE;

	// do we wear a scuba mask?
	if ( !(this->inv[HEAD1POS].exists( ) && HasItemFlag( this->inv[HEAD1POS].usItem, SCUBA_MASK )) && !(this->inv[HEAD2POS].exists( ) && HasItemFlag( this->inv[HEAD2POS].usItem, SCUBA_MASK )) )
		return FALSE;

	if (!(this->inv[CPACKPOCKPOS].exists() && HasItemFlag(this->inv[CPACKPOCKPOS].usItem, SCUBA_BOTTLE)) &&
		!(this->inv[BPACKPOCKPOS].exists() && HasItemFlag(this->inv[BPACKPOCKPOS].usItem, SCUBA_BOTTLE)))
		return FALSE;

	return TRUE;
}

// Flugente: are we an assassin?
BOOLEAN		SOLDIERTYPE::IsAssassin( )
{
	// kingpin's hitmen are assassins
	if ( this->ubProfile >= JIM && this->ubProfile <= TYRONE )
		return TRUE;

	// there can be non-NPC assassins too
	if ( this->usSoldierFlagMask & SOLDIER_ASSASSIN )
		return TRUE;

	return FALSE;
}

UINT8	SOLDIERTYPE::GetMultiTurnAction( )
{
	return usMultiTurnAction;
}

void	SOLDIERTYPE::StartMultiTurnAction( UINT8 usActionType, INT32 asGridNo )
{
	// check wether we can perform any action at all
	if ( !this->bActive || !this->bInSector || this->stats.bLife < OKLIFE || TileIsOutOfBounds( asGridNo ) || this->bCollapsed )
		return;

	// wether an action is possible or not depends on action itself (there are actions without a gridno)
	switch ( usMultiTurnAction )
	{
	case MTA_FORTIFY:
	case MTA_REMOVE_FORTIFY:
		if ( this->pathing.bLevel != 0 )
			return;
		break;
	}

	// if we already perform a multi-turn action, overwrite it
	if ( GetMultiTurnAction( ) > MTA_NONE )
		CancelMultiTurnAction( FALSE );

	// set up the action and costs
	usMultiTurnAction = usActionType;

	// the action shall be performed on the gridno directly in front of us
	sMTActionGridNo = asGridNo;

	// for now, adding and removing a structure has the same AP cost - that might change in the future
	bOverTurnAPS = GetAPsForMultiTurnAction( this, usMultiTurnAction );

	// immediately starting the action would leave us without APs, thus removing the benefit of multi-turn actions (ability to do something else while performing a longer action)
	// for this reason, we only do this when we are not in combat
	if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
		UpdateMultiTurnAction( );
}

void	SOLDIERTYPE::CancelMultiTurnAction( BOOLEAN fFinished )
{
	// stop action
	if ( !fFinished && this->usMultiTurnAction )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[STR_MTA_CANCEL], this->GetName( ), szMTATextStr[this->usMultiTurnAction] );

	bOverTurnAPS = 0;
	sMTActionGridNo = NOWHERE;
	usMultiTurnAction = MTA_NONE;
}

// if we are doing any multiturn-action, remove our current APs from it, and if possible, perform the action and finish the process
BOOLEAN	SOLDIERTYPE::UpdateMultiTurnAction( )
{
	// nothing to do here
	if ( this->usMultiTurnAction == MTA_NONE )
		return FALSE;

	// check wether we can perform any action at all
	if ( !this->bActive || !this->bInSector || this->stats.bLife < OKLIFE || TileIsOutOfBounds( this->sGridNo ) || this->bCollapsed )
	{
		CancelMultiTurnAction( FALSE );
		return FALSE;
	}

	// wether an action is possible or not depends on action itself (there are actions without a gridno)
	switch ( usMultiTurnAction )
	{
	case MTA_FORTIFY:
	case MTA_REMOVE_FORTIFY:
	
		/*// building on a roof is forbidden! stop this!
		if ( this->pathing.bLevel != 0 )
		{
			CancelMultiTurnAction( FALSE );
			return FALSE;
		}*/

		// we have to be crouched, get out of here, set us to be crouched first
		if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH )
		{
			return TRUE;
		}
		break;

	case MTA_HACK:
		break;

		default:	// default: exit
		{
			CancelMultiTurnAction( FALSE );
			return FALSE;
		}
		break;
	}

	// check wether our selected action can still be performed
	BOOLEAN fActionStillValid = TRUE;

	// determine the gridno before us and the item we have in our main hand, this is enough for the current actions
	OBJECTTYPE* pObj = &(this->inv[HANDPOS]);

	// error if object is missing
	if ( usMultiTurnAction == MTA_FORTIFY || usMultiTurnAction == MTA_REMOVE_FORTIFY )
	{
		if ( !pObj || !(pObj->exists( )) )
			fActionStillValid = FALSE;
	}

	// error if the gridno we started working on is not the gridno we are currently looking at
	if ( usMultiTurnAction == MTA_FORTIFY || usMultiTurnAction == MTA_REMOVE_FORTIFY )//|| usMultiTurnAction == MTA_HACK )
	{
		if ( this->sMTActionGridNo != NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) ) )
			fActionStillValid = FALSE;
	}

	if ( !fActionStillValid )
	{
		CancelMultiTurnAction( FALSE );
		return FALSE;
	}

	INT16 entireapcost = 0;
	INT16 entirebpcost = 0;
	switch ( usMultiTurnAction )
	{
		case MTA_FORTIFY:
		{
			entireapcost = GetAPsForMultiTurnAction( this, MTA_FORTIFY );
			entirebpcost = APBPConstants[BP_FORTIFICATION];

			if ( !IsFortificationPossibleAtGridNo( this->sMTActionGridNo ) )
				fActionStillValid = FALSE;
			else if ( !IsStructureConstructItem( this->inv[HANDPOS].usItem, this->sMTActionGridNo, this ) )
				fActionStillValid = FALSE;
		}
		break;

		case MTA_REMOVE_FORTIFY:
		{
			entireapcost = GetAPsForMultiTurnAction( this, MTA_REMOVE_FORTIFY );
			entirebpcost = APBPConstants[BP_REMOVE_FORTIFICATION];

			if ( !IsStructureDeconstructItem( this->inv[HANDPOS].usItem, this->sMTActionGridNo, this ) )
				fActionStillValid = FALSE;
		}
		break;

		case MTA_HACK:
		{
			entireapcost = GetAPsForMultiTurnAction( this, MTA_HACK );
			entirebpcost = 0;		// hacking isn't exactly hard to do physically :-)

			UINT16 structindex;
			if ( !this->GetInteractiveActionSkill( this->sMTActionGridNo, this->pathing.bLevel, INTERACTIVE_STRUCTURE_HACKABLE ) ||
				 !InteractiveActionPossibleAtGridNo( this->sMTActionGridNo, this->pathing.bLevel, structindex ) == INTERACTIVE_STRUCTURE_HACKABLE )
				fActionStillValid = FALSE;
		}
		break;
	}

	if ( !fActionStillValid )
	{
		CancelMultiTurnAction( FALSE );
		return FALSE;
	}

	// refresh animations
	switch ( usMultiTurnAction )
	{
		case MTA_FORTIFY:
		case MTA_REMOVE_FORTIFY:
		{
			// if we are not in turnbased and no enemies are around, we reduce the number of necessary action points to 0. No need to keep waiting if there's nobody around anyway
			if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
				bOverTurnAPS = 0;
			// otherwise this might take longer, so we refresh our animation
			else
			{
				if ( !is_networked )
					this->EVENT_InitNewSoldierAnim( CUTTING_FENCE, 0, FALSE );
				else
					this->ChangeSoldierState( CUTTING_FENCE, 0, 0 );

				// as setting the new animation costs APBPConstants[AP_USEWIRECUTTERS] APs every time, account for that
				bOverTurnAPS = max( 0, bOverTurnAPS - APBPConstants[AP_USEWIRECUTTERS] );
			}
		}
		break;

		case MTA_HACK:
		{
			// if we are not in turnbased and no enemies are around, we reduce the number of necessary action points to 0. No need to keep waiting if there's nobody around anyway
			if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
				bOverTurnAPS = 0;
		}
		break;
	}

	// if we can afford it, do it now
	if ( bOverTurnAPS <= this->bActionPoints )
	{
		switch ( usMultiTurnAction )
		{
			case MTA_FORTIFY:
			{
				// Build the thing
				if ( BuildFortification( this->sMTActionGridNo, this, pObj ) )
				{
					// we gain a bit of experience...
					StatChange( this, STRAMT, 4, TRUE );
					StatChange( this, HEALTHAMT, 2, TRUE );
				}
			}
			break;

			case MTA_REMOVE_FORTIFY:
			{
				if ( RemoveFortification( this->sMTActionGridNo, this, pObj ) )
				{
					// we gain a bit of experience...
					StatChange( this, STRAMT, 3, TRUE );
					StatChange( this, HEALTHAMT, 2, TRUE );
				}
			}
			break;

			case MTA_HACK:
			{
				UINT16 structindex;
				UINT16 possibleaction = InteractiveActionPossibleAtGridNo( this->sMTActionGridNo, this->pathing.bLevel, structindex );
				UINT16 skill = this->GetInteractiveActionSkill( sGridNo, this->pathing.bLevel, possibleaction );

				INT32 difficulty = gInteractiveStructure[structindex].difficulty;
				INT32 luaactionid = gInteractiveStructure[structindex].luaactionid;

				BOOLEAN success = (skill >= difficulty);
				if ( possibleaction != INTERACTIVE_STRUCTURE_HACKABLE )
					success = FALSE;

				// call lua with the action id - perhaps we might do something special here
				if ( luaactionid >= 0 )
				{
					LuaHandleInteractiveActionResult( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, sGridNo, this->pathing.bLevel, this->ubID, possibleaction, luaactionid, difficulty, skill );
				}
				else
				{
					DoInteractiveActionDefaultResult( sGridNo, this->ubID, (skill > difficulty) );
				}
			}
			break;
		}

		if ( entireapcost > 0 )
			DeductPoints( this, bOverTurnAPS, (INT32)(entirebpcost * this->bOverTurnAPS / entireapcost), 0 );

		// we're done here!
		CancelMultiTurnAction( TRUE );
	}
	// remove the costs as much as we can
	else if ( this->bActionPoints > 0 )
	{
		INT16 oldAPs = this->bActionPoints;
		if ( bOverTurnAPS > 0 )
			DeductPoints( this, this->bActionPoints, (INT32)(entirebpcost * this->bActionPoints / entireapcost), 0 );

		bOverTurnAPS -= oldAPs;
	}

	return TRUE;
}

void	SOLDIERTYPE::DropSectorEquipment( )
{
	// not if we already dropped the gear
	if ( this->usSoldierFlagMask & SOLDIER_EQUIPMENT_DROPPED )
		return;

	// set marker: we are about to drop our gear
	this->usSoldierFlagMask |= SOLDIER_EQUIPMENT_DROPPED;

	OBJECTTYPE* pObj = NULL;
	UINT8 size = this->inv.size( );

	INT32 sPutGridNo = this->sGridNo;
	if ( sPutGridNo == NOWHERE )
		sPutGridNo = RandomGridNo( );

	if ( Water( sPutGridNo, this->pathing.bLevel ) )
		sPutGridNo = gMapInformation.sCenterGridNo;

	if ( (this->sSectorX == gWorldSectorX) && (this->sSectorY == gWorldSectorY) && (this->bSectorZ == gbWorldSectorZ) )
	{
		for ( UINT8 cnt = 0; cnt < size; ++cnt )
		{
			pObj = &(this->inv[cnt]);

			if ( pObj->exists( ) )
			{
				// Check if it's supposed to be dropped
				if ( !((*pObj).fFlags & OBJECT_UNDROPPABLE) && !ItemIsUndroppableByDefault(pObj->usItem) && (*pObj)[0]->data.sObjectFlag & TAKEN_BY_MILITIA )
				{
					(*pObj)[0]->data.sObjectFlag &= ~TAKEN_BY_MILITIA;

					// if we are not replacing ammo, unload gun prior to dropping it
					if ( !gGameExternalOptions.fMilitiaUseSectorInventory_Ammo && Item[pObj->usItem].usItemClass & IC_GUN )
						(*pObj)[0]->data.gun.ubGunShotsLeft = 0;

					AddItemToPool( sPutGridNo, pObj, 1, this->pathing.bLevel, (WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE), -1 );
					DeleteObj( &(this->inv[cnt]) );
				}
			}
		}
	}
	else
	{
		OBJECTTYPE pObject[NUM_INV_SLOTS];
		UINT8 counter = 0;

		for ( UINT8 cnt = 0; cnt < size; ++cnt )
		{
			pObj = &(this->inv[cnt]);

			if ( pObj->exists( ) )
			{
				// Check if it's supposed to be dropped
				if ( !((*pObj).fFlags & OBJECT_UNDROPPABLE) && !ItemIsUndroppableByDefault(pObj->usItem) && (*pObj)[0]->data.sObjectFlag & TAKEN_BY_MILITIA )
				{
					(*pObj)[0]->data.sObjectFlag &= ~TAKEN_BY_MILITIA;

					// if we are not replacing ammo, unload gun prior to dropping it
					if ( !gGameExternalOptions.fMilitiaUseSectorInventory_Ammo && Item[pObj->usItem].usItemClass & IC_GUN )
						(*pObj)[0]->data.gun.ubGunShotsLeft = 0;

					pObject[counter++] = *pObj;

					DeleteObj( &(this->inv[cnt]) );
				}
			}
		}

		AddItemsToUnLoadedSector( this->sSectorX, this->sSectorY, this->bSectorZ, RandomGridNo( ), counter, pObject, 0, WORLD_ITEM_REACHABLE, 0, 1, FALSE );
	}
}

// sevenfm: take item from inventory to HANDPOS
void SOLDIERTYPE::TakeNewItemFromInventory( UINT16 usItem )
{
	if ( !UsingNewInventorySystem( ) )
		return;

	// this feature works now only in realtime
	if ( (gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
		return;

	if ( this->inv[HANDPOS].exists( ) )
		return;

	// search for item with same id
	INT8 invsize = (INT8)this->inv.size( );
	for ( INT8 i = 0; i < invsize; ++i )
	{
		if ( (this->inv[i].exists( ) == true) && (this->inv[i].usItem == usItem) )
		{
			this->inv[i].MoveThisObjectTo( this->inv[HANDPOS], 1, this );
			return;
		}
	}
}

// sevenfm: take item from inventory to HANDPOS
void SOLDIERTYPE::TakeNewBombFromInventory( UINT16 usItem )
{
	INT8 i;

	if ( !UsingNewInventorySystem( ) )
		return;

	if ( this->inv[HANDPOS].exists( ) )
		return;

	// search for item with same id
	INT8 invsize = (INT8)this->inv.size( );
	for ( i = 0; i < invsize; ++i )
	{
		if ( (this->inv[i].exists( ) == true) && (this->inv[i].usItem == usItem) )
		{
			this->inv[i].MoveThisObjectTo( this->inv[HANDPOS], 1, this );
			return;
		}
	}

	// search for any item with class IC_BOMB
	// take tripwire-activated item only if used item is tripwire activated
	for ( i = 0; i < invsize; i++ )
	{
		UINT16 usItem = this->inv[i].usItem;
		if ( this->inv[i].exists( ) == true &&
			 Item[usItem].usItemClass == IC_BOMB &&
			 Item[usItem].ubCursor == BOMBCURS &&
			 !ItemIsTripwire(usItem) &&
			 ((ItemHasTripwireActivation(usItem) && ItemHasTripwireActivation(usItem)) ||
			 (!ItemHasTripwireActivation(usItem) && !ItemHasTripwireActivation(usItem))) )
		{
			this->inv[i].MoveThisObjectTo( this->inv[HANDPOS], 1, this );
			return;
		}
	}
}

//  Flugente: switch hand item for gunsling weapon, or pistol, or knife
void	SOLDIERTYPE::SwitchWeapons( BOOLEAN fKnife, BOOLEAN fSideArm )
{
	UINT8 pocketsearch, retrieveslot, handobjstorageslot;
	BOOLEAN handCanMove, searchitemCanMove;

	// The slot we move our hand object from and the new object to is obviously always HANDPOS

	// first pocket to search in OIV & NIV
	if ( UsingNewInventorySystem( ) )
		pocketsearch = GUNSLINGPOCKPOS;
	else
		pocketsearch = BIGPOCK1POS;

	// The second slot is the one from where we retrieve the object we search
	if ( UsingNewInventorySystem( ) )
		retrieveslot = GUNSLINGPOCKPOS;
	else // search for gun in OIV
	{
		for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
		{
			// we use the first gun we can find
			if ( this->inv[i].exists( ) && Item[this->inv[i].usItem].usItemClass & IC_GUN && this->inv[i].ubNumberOfObjects == 1 )
			{
				retrieveslot = i;
				break;
			}
		}
	}

	// The third slot is where we put our hand item into
	handobjstorageslot = HANDPOS;

	// if we swap knifes or sidearms, we search for any such item if we do not already have it in our hands. If we do, we instead search for a gun to switch
	if ( fKnife )
	{
		// if we already have a knife in hand, search for a gun instead
		if ( this->inv[HANDPOS].exists( ) && Item[this->inv[HANDPOS].usItem].usItemClass & IC_BLADE )
		{
			for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
			{
				// we use the first gun we can find
				if ( this->inv[i].exists( ) && Item[this->inv[i].usItem].usItemClass & IC_GUN && this->inv[i].ubNumberOfObjects == 1 )
				{
					retrieveslot = i;
					break;
				}
			}
		}
		// search for a knife
		else
		{
			for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
			{
				// take first blade
				if ( this->inv[i].exists( ) && Item[this->inv[i].usItem].usItemClass & IC_BLADE && this->inv[i].ubNumberOfObjects == 1 )
				{
					retrieveslot = i;
					break;
				}
			}
		}
	}
	else if ( fSideArm )
	{
		// if we already have a sidearm in hand, search for a gun that isn't a sidearm instead
		if ( this->inv[HANDPOS].exists( ) && Item[this->inv[HANDPOS].usItem].usItemClass & IC_GUN
			 && Weapon[Item[this->inv[HANDPOS].usItem].ubClassIndex].ubWeaponClass == HANDGUNCLASS )
		{
			for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
			{
				// we use the first gun we can find
				if ( this->inv[i].exists( ) && Item[this->inv[i].usItem].usItemClass & IC_GUN && Weapon[Item[this->inv[i].usItem].ubClassIndex].ubWeaponClass != HANDGUNCLASS && this->inv[i].ubNumberOfObjects == 1 )
				{
					retrieveslot = i;
					break;
				}
			}
		}
		// search for a sidearm
		else
		{
			for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
			{
				// take first handgun
				if ( this->inv[i].exists( ) && Item[this->inv[i].usItem].usItemClass & IC_GUN && Weapon[Item[this->inv[i].usItem].ubClassIndex].ubWeaponClass == HANDGUNCLASS && this->inv[i].ubNumberOfObjects == 1 )
				{
					retrieveslot = i;
					break;
				}
			}
		}
	}

	// search a slot to put our hand object into
	for ( UINT8 i = pocketsearch; i < NUM_INV_SLOTS; ++i )
	{
		// take first slot that hand item would fit
		if ( CanItemFitInPosition( this, &this->inv[HANDPOS], i, FALSE ) )
		{
			if ( i == retrieveslot || !this->inv[i].exists( ) )
			{
				handobjstorageslot = i;
				break;
			}
		}
	}

	// check both items can be moved
	// check HANDPOS item that cannot be moved to inventory
	if ( handobjstorageslot == HANDPOS && this->inv[HANDPOS].exists( ) == true )
		handCanMove = FALSE;
	else
		handCanMove = (CanItemFitInPosition( this, &this->inv[HANDPOS], handobjstorageslot, FALSE ) || (this->inv[HANDPOS].exists( ) == false && this->inv[SECONDHANDPOS].exists( ) == false));

	if ( ItemIsTwoHanded(this->inv[retrieveslot].usItem) && this->inv[SECONDHANDPOS].exists( ) == true )
		searchitemCanMove = FALSE;
	else
		searchitemCanMove = (CanItemFitInPosition( this, &this->inv[retrieveslot], HANDPOS, FALSE ) || this->inv[retrieveslot].exists( ) == false);

	// execute swap 
	if ( handCanMove == TRUE && searchitemCanMove == TRUE )
	{
		if ( UsingInventoryCostsAPSystem() )
		{
			UINT16 APTotalCost = 0;

			if ( this->inv[retrieveslot].exists( ) )
				APTotalCost += GetInvMovementCost( &this->inv[retrieveslot], retrieveslot, HANDPOS );

			if ( this->inv[HANDPOS].exists( ) )
				APTotalCost += GetInvMovementCost( &this->inv[HANDPOS], HANDPOS, handobjstorageslot );

			// Flugente: backgrounds
			APTotalCost = (APTotalCost * (100 + this->GetBackgroundValue(BG_INVENTORY))) / 100;

			if ( this->bActionPoints >= APTotalCost )
			{
				// SANDRO - I dared to change this to use the appropriate function, as that function is actually important for IIS
				//pSoldier->bActionPoints -= APTotalCost;
				DeductPoints( this, APTotalCost, 0 );

				SwapObjs( &this->inv[HANDPOS], &this->inv[retrieveslot] );

				// if we store our hand item in a different position than the item we retrieve originally was, swap again
				if ( handobjstorageslot != retrieveslot && handobjstorageslot != HANDPOS )
					SwapObjs( &this->inv[retrieveslot], &this->inv[handobjstorageslot] );

				HandleTacticalEffectsOfEquipmentChange( this, HANDPOS, this->inv[retrieveslot].usItem, this->inv[HANDPOS].usItem );
			}
			else
			{
				CHAR16	zOutputString[512];
				swprintf( zOutputString, New113Message[MSG113_INVENTORY_APS_INSUFFICIENT], APTotalCost, this->bActionPoints );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, zOutputString );
			}
		}
		else
		{
			SwapObjs( &this->inv[HANDPOS], &this->inv[retrieveslot] );

			// if we store our hand item in a different position than the item we retrieve originally was, swap again
			if ( handobjstorageslot != retrieveslot && handobjstorageslot != HANDPOS )
				SwapObjs( &this->inv[retrieveslot], &this->inv[handobjstorageslot] );

			HandleTacticalEffectsOfEquipmentChange( this, HANDPOS, this->inv[retrieveslot].usItem, this->inv[HANDPOS].usItem );
		}
	}
	fCharacterInfoPanelDirty = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;

	// Flugente: we have to recheck our flashlights
	this->HandleFlashLights( );
}

UINT8 tmpuser = 0;
static CHAR16	tmpname[2][MAX_ENEMY_NAMES_CHARS];	// we need 2 arrays, in case we need 2 name pointers in one string
STR16 SOLDIERTYPE::GetName( )
{
	++tmpuser;
	if ( tmpuser > 1 )
		tmpuser = 0;

	tmpname[tmpuser][0] = '\0';
	wcscat( tmpname[tmpuser], this->name );

	MILITIA militia;
	if ( GetMilitia( this->usIndividualMilitiaID, &militia ) )
	{
		return militia.GetName( );
	}

	if ( this->usSoldierProfile )
	{
		INT8 type = this->GetSoldierProfileType( this->bTeam );
		if ( type > -1 )
		{
			wcscpy( tmpname[tmpuser], zSoldierProfile[type][this->usSoldierProfile].szName );
			tmpname[tmpuser][MAX_ENEMY_NAMES_CHARS - 1] = '\0';
		}
	}

	return tmpname[tmpuser];
}

INT8 SOLDIERTYPE::GetTraitCTHModifier( UINT16 usItem, INT16 ubAimTime, UINT8 ubTargetProfile )
{
	INT8 modifier = 0;

	// Modify for traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// Bonus for heavy weapons moved here from above to get instant CtH bonus and not marksmanship bonus, 
		// which is supressed by weapon condition
		if (ItemIsRocketLauncher(usItem) || ItemIsSingleShotRocketLauncher(usItem))
		{
			modifier += gSkillTraitValues.bCtHModifierRocketLaunchers; // -25% for untrained mercs !!!

			if ( HAS_SKILL_TRAIT( this, HEAVY_WEAPONS_NT ) )
				modifier += gSkillTraitValues.ubHWBonusCtHRocketLaunchers * NUM_SKILL_TRAITS( this, HEAVY_WEAPONS_NT ); // +25% per trait
		}
		// Added CtH bonus for Gunslinger trait on pistols and machine-pistols
		else if ( Weapon[usItem].ubWeaponType == GUN_PISTOL )
		{
			modifier += gSkillTraitValues.bCtHModifierPistols; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( this, GUNSLINGER_NT ) && this->bDoBurst == 0 && this->bDoAutofire == 0 )
				modifier += gSkillTraitValues.ubGSBonusCtHPistols * NUM_SKILL_TRAITS( this, GUNSLINGER_NT ); // +10% per trait
		}
		else if ( Weapon[usItem].ubWeaponType == GUN_M_PISTOL )
		{
			modifier += gSkillTraitValues.bCtHModifierMachinePistols; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( this, GUNSLINGER_NT ) && ((this->bDoBurst == 0 && this->bDoAutofire == 0) || !gSkillTraitValues.ubGSCtHMPExcludeAuto) )
				modifier += gSkillTraitValues.ubGSBonusCtHMachinePistols * NUM_SKILL_TRAITS( this, GUNSLINGER_NT ); // +5% per trait
		}
		// Added CtH bonus for Machinegunner skill on assault rifles, SMGs and LMGs
		else if ( Weapon[usItem].ubWeaponType == GUN_AS_RIFLE )
		{
			modifier += gSkillTraitValues.bCtHModifierAssaultRifles; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( this, AUTO_WEAPONS_NT ) )
				modifier += gSkillTraitValues.ubAWBonusCtHAssaultRifles * NUM_SKILL_TRAITS( this, AUTO_WEAPONS_NT ); // +5% per trait
		}
		else if ( Weapon[usItem].ubWeaponType == GUN_SMG )
		{
			modifier += gSkillTraitValues.bCtHModifierSMGs; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( this, AUTO_WEAPONS_NT ) )
				modifier += gSkillTraitValues.ubAWBonusCtHSMGs * NUM_SKILL_TRAITS( this, AUTO_WEAPONS_NT ); // +5% per trait
		}
		else if ( Weapon[usItem].ubWeaponType == GUN_LMG )
		{
			modifier += gSkillTraitValues.bCtHModifierLMGs; // -10% for untrained mercs.

			if ( HAS_SKILL_TRAIT( this, AUTO_WEAPONS_NT ) )
				modifier += gSkillTraitValues.ubAWBonusCtHLMGs * NUM_SKILL_TRAITS( this, AUTO_WEAPONS_NT ); // +5% per trait
		}
		// Added CtH bonus for Gunslinger trait on pistols and machine-pistols
		else if ( Weapon[usItem].ubWeaponType == GUN_SN_RIFLE )
		{
			modifier += gSkillTraitValues.bCtHModifierSniperRifles; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( this, SNIPER_NT ) && this->bDoBurst == 0 && this->bDoAutofire == 0 )
				modifier += gSkillTraitValues.ubSNBonusCtHSniperRifles * NUM_SKILL_TRAITS( this, SNIPER_NT ); // +5% per trait
		}
		// Added CtH bonus for Ranger skill on rifles and shotguns
		else if ( Weapon[usItem].ubWeaponType == GUN_RIFLE )
		{
			modifier += gSkillTraitValues.bCtHModifierRifles; // -5% for untrained mercs.

			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( this, RANGER_NT ) && this->bDoBurst == 0 && this->bDoAutofire == 0 )
				modifier += gSkillTraitValues.ubRABonusCtHRifles * NUM_SKILL_TRAITS( this, RANGER_NT ); // +5% per trait
			//CHRISL: Why wouldn't sniper training include standard rifles which are often used as "poor-man sniper rifles"
			// this bonus is applied only on single shots!
			if ( HAS_SKILL_TRAIT( this, SNIPER_NT ) && this->bDoBurst == 0 && this->bDoAutofire == 0 )
				modifier += gSkillTraitValues.ubSNBonusCtHRifles * NUM_SKILL_TRAITS( this, SNIPER_NT ); // +5% per trait
		}
		else if ( Weapon[usItem].ubWeaponType == GUN_SHOTGUN )
		{
			modifier += gSkillTraitValues.bCtHModifierShotguns; // -5% for untrained mercs.

			if ( HAS_SKILL_TRAIT( this, RANGER_NT ) )
				modifier += gSkillTraitValues.ubRABonusCtHShotguns * NUM_SKILL_TRAITS( this, RANGER_NT ); // +10% per trait
		}

		// Added small CtH penalty for robot if controller hasn't the Technician trait
		if ( AM_A_ROBOT( this ) )
		{
			modifier += gSkillTraitValues.bCtHModifierRobot; // -10% 

			if ( HAS_SKILL_TRAIT( this->GetRobotController( ), TECHNICIAN_NT ) )
				modifier += gSkillTraitValues.ubTECtHControlledRobotBonus * NUM_SKILL_TRAITS( this->GetRobotController( ), TECHNICIAN_NT ); // +10% per trait
		}

		// Added character traits influence
		if ( this->ubProfile != NO_PROFILE )
		{
			// Sociable - better performance in groups
			if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )
			{
				INT8 bNumMercs = CheckMercsNearForCharTraits( this->ubProfile, CHAR_TRAIT_SOCIABLE );
				if ( bNumMercs > 2 )
					modifier += 5;
				else if ( bNumMercs > 0 )
					modifier += 2;
			}
			// Loner - better performance when alone
			else if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )
			{
				INT8 bNumMercs = CheckMercsNearForCharTraits( this->ubProfile, CHAR_TRAIT_LONER );
				if ( bNumMercs == 0 )
					modifier += 5;
				else if ( bNumMercs <= 1 )
					modifier += 2;
			}
			// Aggressive - bonus on bursts/autofire
			else if ( DoesMercHavePersonality( this, CHAR_TRAIT_AGGRESSIVE ) )
			{
				if ( (this->bDoBurst || this->bDoAutofire) && !ubAimTime )
					modifier += 10;
			}
			// Show-off - better performance if some babes around to impress
			else if ( DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) )
			{
				INT8 bNumMercs = CheckMercsNearForCharTraits( this->ubProfile, CHAR_TRAIT_SHOWOFF );
				if ( bNumMercs > 1 )
					modifier += 5;
				else if ( bNumMercs > 0 )
					modifier += 2;
			}
			// Added disabilities
			if ( this->ubProfile != NO_PROFILE )
			{
				// Heat intolerant penalty
				if ( MercIsHot( this ) )
				{
					modifier -= 15;
				}
				// Small penalty for fear of insects in tropical sectors
				// Flugente: drugs can temporarily cause a merc get a new disability
				else if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) && MercIsInTropicalSector( this ) )
				{
					// fear of insects, and we are in tropical sector
					modifier -= 5;
				}
			}
		}

		// Dauntless - penalty for not taking proper cover
		if ( ubTargetProfile != NO_PROFILE )
		{
			if ( gMercProfiles[ubTargetProfile].bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
				modifier += 5;
		}
	}
	else
	{
		// this rather unlogical bonus for psychotic characters applies only with old traits
		if ( DoesMercHaveDisability( this, PSYCHO ) )
		{
			modifier += AIM_BONUS_PSYCHO;
		}
	}

	return modifier;
}

void SOLDIERTYPE::HandleFlashLights( )
{
	// no more need to redo this check
	this->usSoldierFlagMask &= ~SOLDIER_REDOFLASHLIGHT;

	// we must be active and in a sector (not travelling) in a valid position
	if ( !bActive || !bInSector || TileIsOutOfBounds( this->sGridNo ) )
		return;

	// no flashlight stuff if it isn't night, and we aren't underground
	if ( !NightTime( ) && !gbWorldSectorZ )
		return;

	// take note of wether we changed light
	BOOLEAN fLightChanged = FALSE;

	// remove existing lights we 'own'
	if ( this->usSoldierFlagMask & SOLDIER_LIGHT_OWNER )
	{
		RemovePersonalLights( this->ubID );

		this->usSoldierFlagMask &= ~SOLDIER_LIGHT_OWNER;

		fLightChanged = TRUE;
	}

    if ( AddBestFlashLight() )
    {
        // take note: we own a light source
        this->usSoldierFlagMask |= SOLDIER_LIGHT_OWNER;

        fLightChanged = TRUE;
    }

	if ( fLightChanged )
	{
		// refresh sight for everybody
		AllTeamsLookForAll( TRUE );

		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

UINT8 SOLDIERTYPE::GetBestEquippedFlashLightRange( )
{
	UINT8 bestrange = 0;

	// do this check for both hands
	UINT16 firstslot = HANDPOS;
	UINT16 lastslot = VESTPOCKPOS;
	for ( UINT16 invpos = firstslot; invpos < lastslot; ++invpos )
	{
		OBJECTTYPE* pObj = &(this->inv[invpos]);

		if ( !pObj || !(pObj->exists( )) )
			// can't use this, end
			continue;

		// due to our attachment system, flashlights on guns do not require the batteries to be attached to the flashlight itself - anywhere will do
		if ( !FindAttachedBatteries( pObj ) )
			continue;

		if ( Item[pObj->usItem].usFlashLightRange )
		{
			bestrange = max( bestrange, Item[pObj->usItem].usFlashLightRange );
		}

		attachmentList::iterator iterend = (*pObj)[0]->attachments.end( );
		for ( attachmentList::iterator iter = (*pObj)[0]->attachments.begin( ); iter != iterend; ++iter )
		{
			if ( iter->exists( ) && Item[iter->usItem].usFlashLightRange )
				bestrange = max( bestrange, Item[iter->usItem].usFlashLightRange );
		}
	}

	return(bestrange);
}

bool SOLDIERTYPE::AddBestFlashLight()
{
    // not possible to get this bonus on a roof, due to our lighting system
    if ( this->pathing.bLevel != 0 )
    {
        return false;
    }

    UINT8 maxRange = this->GetBestEquippedFlashLightRange();
    if ( maxRange < 1 )
    {
        return false;
    }

    // we don't use the flashlight to run better at night (light up our shoes), we use it to find enemies!
    UINT8 minRange = 4;
    if ( minRange > maxRange )
    {
        minRange = maxRange;
    }

    float maxAngle = 45;
    maxAngle *= PI / 180 / 2; // convert to rad and halven

    auto forward = DirectionInc(this->ubDirection);
    auto left = DirectionInc(DirectionIfTurnedClockwise(this->ubDirection, 6));
    auto leftLeft = DirectionInc(DirectionIfTurnedClockwise(this->ubDirection, 5));
    auto right = DirectionInc(DirectionIfTurnedClockwise(this->ubDirection, 2));
    auto rightRight = DirectionInc(DirectionIfTurnedClockwise(this->ubDirection, 3));

    bool isDiagonal = this->ubDirection == NORTHEAST || this->ubDirection == NORTHWEST || this->ubDirection == SOUTHEAST || this->ubDirection == SOUTHWEST;

	struct position_2d
	{
        INT16 x, y;

		position_2d(INT32 gridNo)
		{
			ConvertGridNoToXY(gridNo, &x, &y);
		}
		position_2d(INT16 _x, INT16 _y) : x{_x}, y{_y}
        {
        }
	};
	struct vector_2d
	{
        INT16 dx, dy;
        float length;

		vector_2d(INT8 direction)
		{
			ConvertDirectionToVectorInXY(direction, &dx, &dy);
            length = CalcLength(dx, dy);
		}
		vector_2d(position_2d from, position_2d to)
		{
			dx = to.x - from.x;
			dy = to.y - from.y;
            length = CalcLength(dx, dy);
		}
		vector_2d(INT16 _dx, INT16 _dy) : dx{_dx}, dy{_dy}
		{
			length = CalcLength(dx, dy);
		}

		float GetAngle( vector_2d other )
		{
			auto dot = dx * other.dx + dy * other.dy;
			return acos(dot / (length * other.length));
		}

        static float CalcLength(float dx, float dy)
        {
            return sqrt(powf(dx, 2) + powf(dy, 2));
        }
	};

	position_2d soldierPos(this->sGridNo);
    vector_2d soldierDir(this->ubDirection);

    auto is_in_area = [&](INT32 sGridNoToTest) -> bool
    {
        vector_2d v(soldierPos, position_2d(sGridNoToTest));

		if (v.length > maxRange)
		{
			return false;
		}

        if (v.length < minRange)
        {
            return false;
        }

        auto coneAngle = soldierDir.GetAngle( v );
        if (coneAngle > maxAngle)
        {
            return false;
        }

        return true;
    };

    auto add_light_if_in_line_of_sight = [&, this]( INT32 sGridNoToTest, bool allowSkip ) -> void
    {
        if (allowSkip) // improve performance by skipping 3/4 of the lights
        {
            INT16 sXPos, sYPos;
            ConvertGridNoToXY( sGridNoToTest, &sXPos, &sYPos );
            if (!(sXPos % 2 == 0 && sYPos % 2 == 0))
            {
                return;
            }
        }

        if ( SoldierToVirtualSoldierLineOfSightTest( this, sGridNoToTest, this->pathing.bLevel, gAnimControl[this->usAnimState].ubEndHeight, false, NO_DISTANCE_LIMIT ) )
        {
            CreatePersonalLight( sGridNoToTest, this->ubID );
        }
    };

    auto travel_direction_to_add_light = [&]( INT32 startingGridNo, INT16 directionIncrementer )
    {
        for ( auto currentGridNo = startingGridNo; !OutOfBounds( currentGridNo, -1 ) && is_in_area( currentGridNo ); currentGridNo += directionIncrementer )
        {
            add_light_if_in_line_of_sight( currentGridNo, true);
        }
    };

    for ( auto currentGridNo = this->sGridNo; !OutOfBounds( currentGridNo, -1 ); currentGridNo += forward )
    {
		vector_2d v(soldierPos, position_2d(currentGridNo));
        if ( v.length < minRange )
        {
            continue;
        }
		else if (v.length > maxRange)
		{
			break;
		}

        add_light_if_in_line_of_sight( currentGridNo, false );

        travel_direction_to_add_light( currentGridNo, left );
        travel_direction_to_add_light( currentGridNo, right );

        if ( isDiagonal )
        {
            travel_direction_to_add_light( NewGridNo( currentGridNo, leftLeft ), left );
            travel_direction_to_add_light( NewGridNo( currentGridNo, rightRight ), right );
        }
    }

    return true;
}

// Flugente: soldier profiles
// retrieves the correct sub-array
INT8 SOLDIERTYPE::GetSoldierProfileType( UINT8 usTeam )
{
	INT8 type = -1;

	if ( usTeam == ENEMY_TEAM && gGameExternalOptions.fSoldierProfiles_Enemy )
	{
		switch ( this->ubSoldierClass )
		{
		case SOLDIER_CLASS_ELITE:
			type = 2;
			break;
		case SOLDIER_CLASS_ARMY:
			type = 1;
			break;
		case SOLDIER_CLASS_ADMINISTRATOR:
			type = 0;
			break;
		}
	}
	else if ( usTeam == MILITIA_TEAM && gGameExternalOptions.fSoldierProfiles_Militia )
	{
		switch ( this->ubSoldierClass )
		{
		case SOLDIER_CLASS_ELITE_MILITIA:
			type = 5;
			break;
		case SOLDIER_CLASS_REG_MILITIA:
			type = 4;
			break;
		case SOLDIER_CLASS_GREEN_MILITIA:
			type = 3;
			break;
		}
	}

	return type;
}

// Flugente: do we have a specific background flag?
BOOLEAN SOLDIERTYPE::HasBackgroundFlag( UINT64 aFlag )
{
	if ( UsingBackGroundSystem() && this->ubProfile != NO_PROFILE )
	{
		if ( zBackground[gMercProfiles[this->ubProfile].usBackground].uiFlags & aFlag )
			return TRUE;
	}

	return FALSE;
}

INT16 SOLDIERTYPE::GetBackgroundValue( UINT16 aNr )
{
	if ( UsingBackGroundSystem() && this->ubProfile != NO_PROFILE )
	{
		return zBackground[gMercProfiles[this->ubProfile].usBackground].value[aNr];
	}

	return 0;
}

const std::vector<INT16>& SOLDIERTYPE::GetBackgroundValueVector(BackgroundVectorTypes backgroundVectorType) const
{
	static const std::vector<INT16> emptyVector;

	if (UsingBackGroundSystem() && this->ubProfile != NO_PROFILE)
	{
		const BACKGROUND_VALUES& background = zBackground[gMercProfiles[this->ubProfile].usBackground];
		auto iterator = background.valueVectors.find(backgroundVectorType);

		if (iterator != background.valueVectors.end())
		{
			return iterator->second;
		}
	}

	return emptyVector;
}

INT8 SOLDIERTYPE::GetSuppressionResistanceBonus( )
{
	INT8 bonus = 0;

	bonus += this->GetBackgroundValue( BG_RESI_SUPPRESSION );

	// Flugente: enemy roles
	if ( this->bTeam == ENEMY_TEAM )
	{
		// bonus if we have an officer around
		UINT8 officertype = OFFICER_NONE;
		if ( HighestEnemyOfficersInSector( officertype ) )
		{
			bonus += gGameExternalOptions.sEnemyOfficerSuppressionResistanceBonus * officertype;
		}
	}

	return min( 100, max( -100, bonus ) );
}

INT16 SOLDIERTYPE::GetMeleeDamageBonus( )
{
	INT8 bonus = 0;

	bonus += this->GetBackgroundValue( BG_PERC_DAMAGE_MELEE );

	return bonus;
}

INT16	SOLDIERTYPE::GetAPBonus( )
{
	INT16 bonus = 0;

	if ( this->usSoldierFlagMask & SOLDIER_AIRDROP_TURN )
		bonus += this->GetBackgroundValue( BG_AIRDROP );

	if ( this->usSoldierFlagMask & SOLDIER_ASSAULT_BONUS )
		bonus += this->GetBackgroundValue( BG_ASSAULT );

	UINT8 ubSector = (UINT8)SECTOR( this->sSectorX, this->sSectorY );
	UINT8 ubTraverseType = SectorInfo[ubSector].ubTraversability[THROUGH_STRATEGIC_MOVE];

	switch ( ubTraverseType )
	{
	case NS_RIVER:
	case EW_RIVER:
		bonus += this->GetBackgroundValue( BG_RIVER );
		break;
	case COASTAL:
	case COASTAL_ROAD:
		bonus += this->GetBackgroundValue( BG_COASTAL );
		break;
	case TROPICS_SAM_SITE:
		bonus += this->GetBackgroundValue( BG_COASTAL );
		bonus += this->GetBackgroundValue( BG_TROPICAL );
		break;
	case TROPICS:
	case TROPICS_ROAD:
		bonus += this->GetBackgroundValue( BG_TROPICAL );
		break;
	case PLAINS:
	case PLAINS_ROAD:
	case FARMLAND:
	case FARMLAND_ROAD:
		bonus += this->GetBackgroundValue( BG_PLAINS );
		break;
	case DENSE:
	case DENSE_ROAD:
		bonus += this->GetBackgroundValue( BG_FOREST );
		break;
	case HILLS:
	case HILLS_ROAD:
		bonus += this->GetBackgroundValue( BG_MOUNTAIN );
		break;
	case SWAMP:
	case SWAMP_ROAD:
		bonus += this->GetBackgroundValue( BG_SWAMP );
		break;
	case SAND:
	case SAND_ROAD:
	case SAND_SAM_SITE:
		bonus += this->GetBackgroundValue( BG_DESERT );
		break;
	case TOWN:
	case CAMBRIA_HOSPITAL_SITE:
	case DRASSEN_AIRPORT_SITE:
	case MEDUNA_AIRPORT_SITE:
		bonus += this->GetBackgroundValue( BG_URBAN );
		break;
	default:
		break;
	}

	if ( this->pathing.bLevel )
		bonus += this->GetBackgroundValue( BG_HEIGHT );

	// diseases can affect stat effectivity
	INT16 diseaseeffect = 0;
	for ( int i = 0; i < NUM_DISEASES; ++i )
		diseaseeffect += Disease[i].sEffAP * this->GetDiseaseMagnitude( i );

	bonus += diseaseeffect;

	return bonus;
}

INT8	SOLDIERTYPE::GetFearResistanceBonus( )
{
	INT8 bonus = 0;

	bonus += this->GetBackgroundValue( BG_RESI_FEAR );

	return min( 100, max( -100, bonus ) );
}

UINT8	SOLDIERTYPE::GetMoraleThreshold( )
{
	UINT8 threshold = 100;
	UINT8 moraledamage = 0;

	moraledamage = (moraledamage * (100 - GetFearResistanceBonus( ))) / 100;
	return min( threshold, max( 0, threshold - moraledamage ) );
}

FLOAT	SOLDIERTYPE::GetMoraleModifier( )
{
	FLOAT mod = 1.0f;

	UINT8 officertype = OFFICER_NONE;
	if ( bTeam == ENEMY_TEAM && HighestEnemyOfficersInSector( officertype ) )
	{
		mod += gGameExternalOptions.dEnemyOfficerMoraleModifier * officertype;
	}

	// Flugente: disease
	if ( gGameExternalOptions.fDisease )
	{
		FLOAT diseaseeffect = 1.0f;
		for ( int i = 0; i < NUM_DISEASES; ++i )
		{
			diseaseeffect *= 1.0f - (1.0f - Disease[i].moralemodifier) * this->GetDiseaseMagnitude( i );
		}

		mod *= diseaseeffect;
	}

	return mod;
}

INT16	SOLDIERTYPE::GetInterruptModifier( UINT8 usDistance )
{
	INT16 bonus = 0;

	/*// drugs can alter our perception
	if ( this->drugs.bDrugEffect[DRUG_TYPE_PERCEPTION] )
		bonus += 2;
	else if ( this->drugs.bDrugSideEffect[DRUG_TYPE_PERCEPTION] )
		bonus -= 2;*/

	// if we are listening on our radio, our mind will be somewhere else... we will be less focused
	if ( this->IsRadioListening( ) )
		bonus -= 3;

	// if we are airdropping and do not have the 'airdrop' background, we receive a substantial malus to our interrupt level. Roping down takes a lot of attention
	if ( this->usSoldierFlagMask & SOLDIER_AIRDROP_TURN && (this->GetBackgroundValue( BG_AIRDROP ) <= 0) )
		bonus -= 8;

	return bonus;
}

void SOLDIERTYPE::SoldierPropertyUpkeep( )
{
	// these effects last only one turn
	this->usSoldierFlagMask &= ~(SOLDIER_AIRDROP_TURN | SOLDIER_ASSAULT_BONUS | SOLDIER_RAISED_REDALERT);
	this->usSoldierFlagMask2 &= ~(SOLDIER_CONCEALINSERTION | SOLDIER_SPENT_AP);

	// this looks bizarre, but is required
	if ( this->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION_DISCOVERED )
	{
		this->usSoldierFlagMask2 &= ~SOLDIER_CONCEALINSERTION_DISCOVERED;

		// we loose our disguise
		this->LooseDisguise();

		if ( gSkillTraitValues.fCOStripIfUncovered )
			this->Strip();

		HandleInitialRedAlert( ENEMY_TEAM, FALSE );
	}

	// sevenfm: stop muzzle flash
	if (this->flags.fMuzzleFlash)
	{
		EndMuzzleFlash(this);
	}

	if ( HasBackgroundFlag( BACKGROUND_EXP_UNDERGROUND ) && this->bSectorZ )
		++bExtraExpLevel;
	
	// if we are dead or dying, we cannot continue radio work
	if ( this->stats.bLife < OKLIFE )
		this->SwitchOffRadio( );

	// if we are an enemy radio operator, and we are jamming frequencies, there is a slight chance that we set off remote-controlled bombs/defuses!
	if ( !gSkillTraitValues.fVOJammingBlocksRemoteBombs && gSkillTraitValues.fVOEnemyVOSetsOffRemoteBombs && this->bTeam == ENEMY_TEAM && IsJamming( ) && Chance( 5 ) )
		SetOffBombsByFrequency( this->ubID, 1 + Random( 8 ) );

	// effects eventually run out
	for ( UINT8 counter = 0; counter < SOLDIER_COUNTER_MAX; ++counter )
	{
		if ( counter == SOLDIER_COUNTER_ROLE_OBSERVED )
			continue;
		else if ( counter == SOLDIER_COUNTER_SPOTTER && usSkillCounter[counter] > 0 )
			usSkillCounter[counter] = min( 255, usSkillCounter[counter] + 1 );
		else
			usSkillCounter[counter] = max( 0, usSkillCounter[counter] - 1 );
	}

	if ( this->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		this->usSkillCooldown[SOLDIER_COOLDOWN_CRYO]--;

	if ( this->usSkillCooldown[SOLDIER_COOLDOWN_DRUGUSER_COMBAT] )
		this->usSkillCooldown[SOLDIER_COOLDOWN_DRUGUSER_COMBAT]--;

	if (AM_A_ROBOT(this) && ItemHasXRay(this->inv[ROBOT_UTILITY_SLOT].usItem))
	{
		if (this->usSkillCooldown[SOLDIER_COOLDOWN_ROBOT_XRAY])
			this->usSkillCooldown[SOLDIER_COOLDOWN_ROBOT_XRAY]--;
		else if(this->bInSector)
		{
			// this allows the robot to do an x-ray ping if there are enemies in sector
			SOLDIERTYPE* pTeamSoldier = Menptr;
			INT32 cnt = 0;
			for (; cnt < TOTAL_SOLDIERS; pTeamSoldier++, cnt++)
			{
				if (pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->stats.bLife > 0)
				{
					if (!pTeamSoldier->aiData.bNeutral && (pTeamSoldier->bSide != 0))
					{
						this->usSkillCooldown[SOLDIER_COOLDOWN_ROBOT_XRAY] += 2;
						ActivateXRayDevice(this);
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szRobotText[ROBOT_TEXT_XRAY_ACTIVATED] );
						break;
					}
				}
			}

		}
	}

	// if soldier was seen this turn, increase his observed counter
	if ( this->usSoldierFlagMask & SOLDIER_ENEMY_OBSERVEDTHISTURN )
	{
		this->usSoldierFlagMask &= ~SOLDIER_ENEMY_OBSERVEDTHISTURN;

		++usSkillCounter[SOLDIER_COUNTER_ROLE_OBSERVED];
	}

	// if there is a combat going and we are in sector, note that in the battle report
	if ( this->bInSector && (gTacticalStatus.uiFlags & INCOMBAT || gTacticalStatus.fEnemyInSector) )
	{
		if ( !(this->usSoldierFlagMask & SOLDIER_BATTLE_PARTICIPATION) )
		{
			this->usSoldierFlagMask |= SOLDIER_BATTLE_PARTICIPATION;

			// Flugente: campaign stats
			gCurrentIncident.AddStat( this, CAMPAIGNHISTORY_TYPE_PARTICIPANT );
		}
	}
	else
	{
		this->usSoldierFlagMask &= ~SOLDIER_BATTLE_PARTICIPATION;
	}

	// sevenfm: disguise automatically
	if ( !gSkillTraitValues.fCOStripIfUncovered )
		this->Disguise();
}

// check if Soldier can use the spell skillwise, with fAPCheck = TRUE also check current APs
BOOLEAN	SOLDIERTYPE::CanUseSkill( INT8 iSkill, BOOLEAN fAPCheck, INT32 sGridNo )
{
	if ( fAPCheck )
	{
		if ( this->bCollapsed )
			return FALSE;
	}

	BOOLEAN canuse = FALSE;

	switch ( iSkill )
	{	
	// radio operator
	case SKILLS_RADIO_ARTILLERY:
		if ( (!fAPCheck || EnoughPoints( this, APBPConstants[AP_RADIO], APBPConstants[BP_RADIO], FALSE )) && CanUseRadio( ) )
		{
			// we also have to check wether we can really order a strike from a sector
			UINT32 sector = 0;
			if ( this->CanAnyArtilleryStrikeBeOrdered( &sector ) )
				canuse = TRUE;
		}
		break;

	case SKILLS_RADIO_JAM:
	case SKILLS_RADIO_SCAN_FOR_JAM:
	case SKILLS_RADIO_LISTEN:
	case SKILLS_RADIO_CALLREINFORCEMENTS:
		if ( (!fAPCheck || EnoughPoints( this, APBPConstants[AP_RADIO], APBPConstants[BP_RADIO], FALSE )) && CanUseRadio( ) )
			canuse = TRUE;
		break;

	case SKILLS_RADIO_TURNOFF:
		if ( (!fAPCheck || EnoughPoints( this, APBPConstants[AP_RADIO], APBPConstants[BP_RADIO], FALSE )) && (IsJamming( ) || IsScanning( ) || IsRadioListening( )) )
			canuse = TRUE;
		break;

	case SKILLS_RADIO_ACTIVATE_TURNCOATS_ALL:
		if ( ( !fAPCheck || EnoughPoints( this, APBPConstants[AP_RADIO], APBPConstants[BP_RADIO], FALSE ) )
			&& CanUseRadio()
			&& gSkillTraitValues.fCOTurncoats
			&& !gbWorldSectorZ
			&& gTacticalStatus.ubInterruptPending == DISABLED_INTERRUPT
			&& IsFreeSlotAvailable( MILITIA_TEAM ) )
			canuse = TRUE;
		break;

	case SKILLS_INTEL_CONCEAL:
	case SKILLS_INTEL_GATHERINTEL:
		// in order to conceal, we need:
		// - enemy team not aware of us (otherwise we could use this skill to instantly escape from combat)
		// - an enemy presence (otherwise, why bother)
		// - we must be alone (otherwise player could start combat again, at which point we'd need to appear from thin air)
		// - no militia present (same reason)
		// - no hostile civilians or creatures
		// - valid disguise
		{
			canuse = TRUE;

			// we might already be on assignment, so be careful here
			INT8 sectorz = this->bSectorZ;
			if ( SPY_LOCATION( this->bAssignment ) )
				sectorz = max( 0, sectorz - 10 );

			// if we are disguised as a civilian, but there is a curfew here, don't allow that
			if ( ( this->usSoldierFlagMask & SOLDIER_COVERT_CIV ) )
			{
				// civilians are suspicious if they are found in certain sectors. Especially at night
				// sector specific value:
				// 0 - civilians are always ok
				// 1 - civilians are suspicious at night
				// 2 - civilians are always suspicious
				// if underground, we still use the surface value

				UINT8 ubSectorId = SECTOR( this->sSectorX, this->sSectorY );
				UINT8 sectordata = SectorExternalData[ubSectorId][sectorz].usCurfewValue;

				if ( sectordata > 1 )
					canuse = FALSE;
				// is it night?
				else if ( sectordata == 1 && GetTimeOfDayAmbientLightLevel() < NORMAL_LIGHTLEVEL_DAY + 2 )
					canuse = FALSE;
			}

			if ( canuse && NumEnemiesInAnySector( this->sSectorX, this->sSectorY, sectorz ) > 0 &&
				NumPlayerTeamMembersInSector( this->sSectorX, this->sSectorY, this->bSectorZ ) == 1 &&
				( sectorz || NumNonPlayerTeamMembersInSector( this->sSectorX, this->sSectorY, MILITIA_TEAM ) == 0 ) &&		
				SeemsLegit( this->ubID ) )
			{
				// additional checks if we are in the currently loaded sector
				if ( this->sSectorX == gWorldSectorX && this->sSectorY == gWorldSectorY && this->bSectorZ == gbWorldSectorZ )
				{
					if ( gTacticalStatus.Team[ENEMY_TEAM].bAwareOfOpposition ||
						( gTacticalStatus.uiFlags & INCOMBAT ) ||
						HostileCiviliansPresent() ||
						HostileCreaturesPresent() )
					{
						canuse = FALSE;
					}
				}
			}
			else
			{
				canuse = FALSE;
			}
		}
		break;

	case SKILLS_CREATE_TURNCOAT:
		// in order to try to create a turncoat, we need:
		// - a non-profile, not-already-turncoat enemy soldier
		// - enemy team not aware of us
		// - valid disguise
		// - enough AP to talk
		{
			SOLDIERTYPE* pSoldier = SimpleFindSoldier( sGridNo, gsInterfaceLevel );
			if ( pSoldier
				&& InPositionForTurncoatAttempt( pSoldier->ubID )
				&& (!fAPCheck || EnoughPoints( this, APBPConstants[AP_TALK], 0, FALSE ) ) )
			{
				canuse = TRUE;
			}
		}
		break;

	case SKILLS_ACTIVATE_TURNCOATS:
		// not during an interrupt
		if ( gSkillTraitValues.fCOTurncoats
			&& !gbWorldSectorZ
			&& gTacticalStatus.ubInterruptPending == DISABLED_INTERRUPT
			&& IsFreeSlotAvailable( MILITIA_TEAM ) )
		{
			SOLDIERTYPE* pSoldier = SimpleFindSoldier( sGridNo, gsInterfaceLevel );
			if ( pSoldier
				&& pSoldier->bTeam == ENEMY_TEAM
				&& pSoldier->ubProfile == NO_PROFILE
				&& ( pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT )
				&& SOLDIER_CLASS_ENEMY( pSoldier->ubSoldierClass ) )
			{
				canuse = TRUE;
			}
		}
		break;

	case SKILLS_ACTIVATE_TURNCOATS_ALL:
		// not during an interrupt
		if ( gSkillTraitValues.fCOTurncoats
			&& !gbWorldSectorZ
			&& gTacticalStatus.ubInterruptPending == DISABLED_INTERRUPT
			&& !gSkillTraitValues.fCOTurncoats_SectorActivationRequiresRadioOperator
			&& IsFreeSlotAvailable( MILITIA_TEAM ) )
		{
			canuse = TRUE;
		}
		break;

	case SKILLS_DISGUISE_APPLY_DISGUISE:
	case SKILLS_DISGUISE_REMOVE_CLOTHES:
		if (IS_MERC_BODY_TYPE(this) && !(usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER)))
			canuse = TRUE;
		break;

	case SKILLS_DISGUISE_REMOVE_DISGUISE:
	case SKILLS_DISGUISE_TEST_DISGUISE:
		if (IS_MERC_BODY_TYPE(this) && usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER))
			canuse = TRUE;
		break;

	case SKILLS_SPOTTER:
		if ( (!fAPCheck || EnoughPoints( this, APBPConstants[AP_SPOTTER], 0, FALSE )) && CanSpot( ) )
			canuse = TRUE;
		break;

	case SKILLS_FOCUS:
		// requires sniper trait, an aimed gun and only works on gridnos in our direction
		if ( gGameOptions.fNewTraitSystem && 
			 (HAS_SKILL_TRAIT( this, AUTO_WEAPONS_NT ) || HAS_SKILL_TRAIT( this, HEAVY_WEAPONS_NT ) || HAS_SKILL_TRAIT( this, SNIPER_NT ) || 
			 HAS_SKILL_TRAIT( this, RANGER_NT ) || HAS_SKILL_TRAIT( this, GUNSLINGER_NT ))
			 && this->inv[HANDPOS].exists( ) && Item[this->inv[HANDPOS].usItem].usItemClass & (IC_GUN | IC_LAUNCHER) && WeaponReady( this )
			 && sGridNo != NOWHERE && this->ubDirection == GetDirectionFromGridNo( sGridNo, this ) )
			canuse = TRUE;
		break;

	case SKILLS_DRAG:

		// TODO: a better check would be whether we can drag anything at the moment - CanDrag is more used for a specific person
		// sevenfm: added AP check to crouch before starting to drag
		if ((!fAPCheck || EnoughPoints(this, GetAPsToStartDrag(this, sGridNo), 0, FALSE)) && CanDragInPrinciple())
			canuse = TRUE;
		break;

	case SKILLS_FILL_CANTEENS:
		if ( !((guiCurrentScreen != GAME_SCREEN && guiCurrentScreen != MSG_BOX_SCREEN) || (gTacticalStatus.uiFlags & INCOMBAT) || gTacticalStatus.fEnemyInSector || gusSelectedSoldier == NOBODY) )
			canuse = TRUE;
		break;

	default:
		break;
	}

	return(canuse);
}

// use a skill. For safety reasons, this calls CanUseSkill again
BOOLEAN SOLDIERTYPE::UseSkill( UINT8 iSkill, INT32 usMapPos, UINT32 ID )
{
	if ( !CanUseSkill( iSkill, TRUE, usMapPos ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_CANNOT_USE_SKILL] );
		return FALSE;
	}

	switch ( iSkill )
	{		
	// radio operator
	// the call for SKILLS_RADIO_ARTILLERY is only used by the AI
	case SKILLS_RADIO_ARTILLERY:
		{
			UINT32 sector = 0;
			if ( this->CanAnyArtilleryStrikeBeOrdered( &sector ) )
			{
				return OrderArtilleryStrike( sector, usMapPos, this->bTeam );
			}
		}
		break;

	case SKILLS_RADIO_JAM:
		return JamCommunications( );
		break;
		
	case SKILLS_RADIO_SCAN_FOR_JAM:
		return ScanForJam( );
		break;

	case SKILLS_RADIO_LISTEN:
		return RadioListen( );
		break;

	case SKILLS_RADIO_CALLREINFORCEMENTS:
		// called separately
		//return RadioCallReinforcements();
		break;

	case SKILLS_RADIO_TURNOFF:
		return SwitchOffRadio( );
		break;

	case SKILLS_RADIO_ACTIVATE_TURNCOATS_ALL:
		return RadioOrderAllTurnCoatToSwitchSides();
		break;

	case SKILLS_INTEL_CONCEAL:
	case SKILLS_INTEL_GATHERINTEL:
		{
			// ATE: Patch fix If in a vehicle, remove from vehicle...
			TakeSoldierOutOfVehicle( this );

			// we store our location and later retrieve it, as the gridno will be set to NOWHERE
			this->sMTActionGridNo = this->sGridNo;

			// remove from squad
			RemoveCharacterFromSquads( this );

			ChangeSoldiersAssignment( this, CONCEALED + iSkill - SKILLS_INTEL_CONCEAL );

			// Remove soldier's graphic
			this->RemoveSoldierFromGridNo();
				
			UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			
			CheckForEndOfBattle( FALSE );

			CheckAndHandleUnloadingOfCurrentWorld();

			return TRUE;
		}
		break;

	case SKILLS_CREATE_TURNCOAT:
		AttemptToCreateTurncoat ( ID );
		return TRUE;
		break;

	case SKILLS_ACTIVATE_TURNCOATS:
		return OrderTurnCoatToSwitchSides( ID );
		break;

	case SKILLS_ACTIVATE_TURNCOATS_ALL:
		OrderAllTurnCoatToSwitchSides();
		return TRUE;
		break;

	case SKILLS_DISGUISE_APPLY_DISGUISE:
		this->Disguise();
		this->SpySelfTest();
		return TRUE;

	case SKILLS_DISGUISE_REMOVE_DISGUISE:
		this->LooseDisguise();
		return TRUE;

	case SKILLS_DISGUISE_TEST_DISGUISE:
		this->SpySelfTest();
		return TRUE;

	case SKILLS_DISGUISE_REMOVE_CLOTHES:
		this->Strip();
		return TRUE;

	case SKILLS_SPOTTER:
		return BecomeSpotter( usMapPos );
		break;

	case SKILLS_FOCUS:
		// activating skill on same gridno again deactivates it
		if ((usSoldierFlagMask2 & SOLDIER_TRAIT_FOCUS) && sFocusGridNo == usMapPos)
		{
			usSoldierFlagMask2 &= ~SOLDIER_TRAIT_FOCUS;
			sFocusGridNo = NOWHERE;

			return FALSE;
		}
		else
		{
			usSoldierFlagMask2 |= SOLDIER_TRAIT_FOCUS;
			sFocusGridNo = usMapPos;

			return TRUE;
		}
		break;

	case SKILLS_DRAG:
		// sevenfm: change to crouch before dragging
		if (gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH)
		{
			HandleStanceChangeFromUIKeys(ANIM_CROUCH);
		}
		if ( usMapPos != NOWHERE )
			SetDragOrderStructure( usMapPos );
		else if ( ID < NOBODY )
			SetDragOrderPerson( ID );
		else
			SetDragOrderCorpse( ID - NOBODY );

		return TRUE;
		break;

	case SKILLS_FILL_CANTEENS:
		SectorFillCanteens();
		break;

	default:
		break;
	}

	return FALSE;
}

// is the AI allowed to use a skill? we have to check how much breath and life using this skill would cost, as otherwise the AI might commit suicide by casting
BOOLEAN SOLDIERTYPE::IsAIAllowedtoUseSkill( INT8 iSkill )
{
	if ( CanUseSkill( iSkill, TRUE ) )
	{
		return TRUE;
	}

	return FALSE;
}

static CHAR16 skilldescarray[500];

// print a small description of the skill if we can use it, or its requirements if we cannot
STR16	SOLDIERTYPE::PrintSkillDesc( INT8 iSkill, INT32 sGridNo )
{
	if ( CanUseSkill( iSkill, TRUE, sGridNo ) )
	{
		return pTraitSkillsMenuDescStrings[iSkill];
	}
	else
	{
		CHAR16	atStr[200];
		swprintf( skilldescarray, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_REQ] );

		if ( iSkill >= SKILLS_RADIO_FIRST && iSkill <= SKILLS_RADIO_LAST )
		{
			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_AP], APBPConstants[AP_RADIO] );
			wcscat( skilldescarray, atStr );
		}

		switch ( iSkill )
		{
		// radio operator
		case SKILLS_RADIO_ARTILLERY:
		case SKILLS_RADIO_JAM:
		case SKILLS_RADIO_SCAN_FOR_JAM:
		case SKILLS_RADIO_LISTEN:
		case SKILLS_RADIO_CALLREINFORCEMENTS:
		case SKILLS_RADIO_TURNOFF:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], gzMercSkillTextNew[RADIO_OPERATOR_NT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], New113Message[MSG113_WORKING_RADIO_SET] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_RADIO_ACTIVATE_TURNCOATS_ALL:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], gzMercSkillTextNew[RADIO_OPERATOR_NT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], New113Message[MSG113_WORKING_RADIO_SET] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOT_DURING_INTERRUPT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_TURNED_ENEMY] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_SURFACELEVEL] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_INTEL_CONCEAL:
		case SKILLS_INTEL_GATHERINTEL:

			//swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_COVERTTRAIT] );
			//wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_ENEMYSECTOR] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_SINGLEMERC] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOALARM] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_DISGUISE_CIV_OR_MIL] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_CREATE_TURNCOAT:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_ENEMY] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOALARM] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_DISGUISE_CIV_OR_MIL] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_SURFACELEVEL] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_STRATEGIC_SUSPICION] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_AP], APBPConstants[AP_TALK] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_ACTIVATE_TURNCOATS:
		case SKILLS_ACTIVATE_TURNCOATS_ALL:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOT_DURING_INTERRUPT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_TURNED_ENEMY] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_SURFACELEVEL] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_DISGUISE_APPLY_DISGUISE:
		case SKILLS_DISGUISE_REMOVE_CLOTHES:
			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOT_DISGUISED] );
			wcscat( skilldescarray, atStr );
			break;

		case SKILLS_DISGUISE_REMOVE_DISGUISE:
		case SKILLS_DISGUISE_TEST_DISGUISE:
			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_DISGUISE_CIV_OR_MIL] );
			wcscat( skilldescarray, atStr );
			break;

		case SKILLS_SPOTTER:
			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_AP], APBPConstants[AP_SPOTTER] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], New113Message[MSG113_BINOCULAR] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_X_TXT], New113Message[MSG113_PATIENCE] );
			wcscat( skilldescarray, atStr );
			break;

		case SKILLS_FOCUS:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_GUNTRAIT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_AIMEDGUN] );
			wcscat(skilldescarray, atStr);

			break;

		case SKILLS_DRAG:

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_PRONEPERSONORCORPSE] );
			wcscat( skilldescarray, atStr );

			// sevenfm: allow to start dragging from any stance, will crouch automatically
			//swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_CROUCH] );
			//wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_FREEHANDS] );
			wcscat( skilldescarray, atStr );

			break;

		case SKILLS_FILL_CANTEENS:
			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_NOT_IN_COMBAT] );
			wcscat( skilldescarray, atStr );

			swprintf( atStr, pTraitSkillsDenialStrings[TEXT_SKILL_DENIAL_FRIENDLY_SECTOR] );
			wcscat( skilldescarray, atStr );

			break;

		default:
			break;
		}
	}

	return skilldescarray;
}

BOOLEAN SOLDIERTYPE::CanUseRadio( BOOLEAN fCheckForAP )
{
	// special case: robot upgraded with a radio set
	if (AM_A_ROBOT(this))
	{
		if (HasItemFlag(this->inv[ROBOT_UTILITY_SLOT].usItem, RADIO_SET))
			return !fCheckForAP || EnoughPoints(this, APBPConstants[AP_RADIO], 0, FALSE);

		return FALSE;
	}

	// only radio operators can use this equipment
	if ( !NUM_SKILL_TRAITS( this, RADIO_OPERATOR_NT ) )
		return FALSE;

	// if we check whether we have enough AP, exit if we don't
	if ( fCheckForAP && !EnoughPoints( this, APBPConstants[AP_RADIO], 0, FALSE ) )
		return FALSE;

	// if we use the new inventory system and we are a player character, the radio set has to be in a specific slot
	OBJECTTYPE* pObj = NULL;
	if ( this->bTeam == OUR_TEAM && UsingNewInventorySystem() )
		pObj = &(inv[CPACKPOCKPOS]);
	else
		pObj = GetObjectWithFlag( RADIO_SET );
	
	if ( pObj && HasItemFlag( pObj->usItem, RADIO_SET ) )
		return TRUE;

	return FALSE;
}

BOOLEAN SOLDIERTYPE::UseRadio( )
{
	BOOLEAN success = FALSE;

	OBJECTTYPE* pObj = NULL;
	if ( this->bTeam == OUR_TEAM && UsingNewInventorySystem( ) )
		pObj = &(inv[CPACKPOCKPOS]);
	else
		pObj = GetObjectWithFlag( RADIO_SET );

	if (AM_A_ROBOT(this))
		pObj = &(inv[ROBOT_UTILITY_SLOT]);

	if ( pObj && HasItemFlag( pObj->usItem, RADIO_SET ) )
	{
		// status % chance of success
		if ( Chance( (*pObj)[0]->data.objectStatus ) )
			success = TRUE;
	}
	
	if ( this->bInSector && (this->ubBodyType == REGMALE || this->ubBodyType == BIGMALE) )
	{
		switch ( gAnimControl[this->usAnimState].ubEndHeight )
		{
		case ANIM_STAND:
			this->EVENT_InitNewSoldierAnim( AI_RADIO, 0, FALSE );
			break;

		case ANIM_CROUCH:
			this->EVENT_InitNewSoldierAnim( AI_CR_RADIO, 0, FALSE );
			break;
		}
	}

	DeductPoints( this, APBPConstants[AP_RADIO], APBPConstants[BP_RADIO], 0 );

	// we gain a bit of experience... - even more if we are the one who began the communication
	StatChange( this, EXPERAMT, this->bInSector ? 8 : 4, TRUE );
	StatChange( this, MECHANAMT, 1, TRUE );

	if ( !success )
	{
		RadioFail( );

		return FALSE;
	}

	return TRUE;
}

BOOLEAN SOLDIERTYPE::HasMortar( )
{
	INT8 invsize = (INT8)inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) == true && ItemIsMortar(inv[bLoop].usItem) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::GetSlotOfSignalShellIfMortar( UINT8* pbLoop )
{
	UINT16 mortaritem = 0;

	INT8 invsize = (INT8)inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) == true && ItemIsMortar(inv[bLoop].usItem) )
		{
			mortaritem = inv[bLoop].usItem;
			break;
		}
	}

	if ( mortaritem )
	{
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
		{
			if ( inv[bLoop].exists( ) == true )
			{
				if ( Item[inv[bLoop].usItem].usItemClass == IC_BOMB && HasItemFlag( inv[bLoop].usItem, SIGNAL_SHELL ) && ValidLaunchable( inv[bLoop].usItem, mortaritem ) )
				{
					(*pbLoop) = bLoop;
					return TRUE;
				}

				if (ItemIsMortar(inv[bLoop].usItem))
				{
					OBJECTTYPE* pAttObj = FindAttachmentByClass( &(inv[bLoop]), IC_BOMB );

					if ( pAttObj )
					{
						if ( HasItemFlag( inv[bLoop].usItem, SIGNAL_SHELL ) )
						{
							(*pbLoop) = bLoop;
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::CanAnyArtilleryStrikeBeOrdered( UINT32* pSectorID )		// can any artillery strikes be ordered by this guy's team from the neighbouring sectors?
{
	if ( !gSkillTraitValues.fROAllowArtillery )
		return FALSE;

	if ( this->bSectorZ )
		return FALSE;

	// if we are AI-controlled, we have to wait for our timer to run out
	if ( this->bTeam != gbPlayerNum && this->usSkillCounter[SOLDIER_COUNTER_RADIO_ARTILLERY] )
		return FALSE;

	// check wether we can call artillery from the 4 adjacent sectors
	for ( UINT8 i = 0; i < 4; ++i )
	{
		INT16 loopX = this->sSectorX;
		INT16 loopY = this->sSectorY;

		if ( i == 0 )		++loopY;
		else if ( i == 1 )	++loopX;
		else if ( i == 2 )	--loopY;
		else if ( i == 3 )	--loopX;

		if ( loopX < 1 || loopX >= MAP_WORLD_X - 1 || loopY < 1 || loopY >= MAP_WORLD_Y - 1 )
			continue;

		// as the player team can order artillery from the militia, we have to check that too.
		if ( IsValidArtilleryOrderSector( loopX, loopY, this->bSectorZ, this->bTeam ) || (this->bTeam == gbPlayerNum && IsValidArtilleryOrderSector( loopX, loopY, this->bSectorZ, MILITIA_TEAM )) )
		{
			*pSectorID = (UINT32)SECTOR( loopX, loopY );
			return TRUE;
		}
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::OrderArtilleryStrike( UINT32 usSectorNr, INT32 sTargetGridNo, UINT8 bTeam )
{
	if ( !CanUseSkill( SKILLS_RADIO_ARTILLERY, TRUE ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_CANNOT_USE_SKILL] );
		return FALSE;
	}

	// check wether radio frequencies are jammed. Not possible to do this in CanUseSkill(), as CanUseRadio() only checks if we theoretically
	if ( SectorJammed( ) )
	{
		// only display message and play sound on our team - no need to signify to player that AI is trying to call in artillery
		if ( bTeam == OUR_TEAM || bTeam == MILITIA_TEAM )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RADIO_JAMMED_NO_COMMUNICATION] );

			PlayJA2SampleFromFile( "Sounds\\radioerror.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
		}

		return FALSE;
	}

	// sector number is in UINT32, even though INT16 would be normal
	INT16 sSectorX = SECTORX( (UINT8)usSectorNr );
	INT16 sSectorY = SECTORY( (UINT8)usSectorNr );

	// just to make sure...
	if ( !IsValidArtilleryOrderSector( sSectorX, sSectorY, this->bSectorZ, bTeam ) )
		return FALSE;

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio( ) )
		return FALSE;

	// determine from where the shells will come
	INT32 sStartingGridNo = gMapInformation.sNorthGridNo;
	if ( sSectorX < this->sSectorX )
		sStartingGridNo = gMapInformation.sWestGridNo;
	else if ( sSectorX > this->sSectorX )
		sStartingGridNo = gMapInformation.sEastGridNo;
	else if ( sSectorY > this->sSectorY )
		sStartingGridNo = gMapInformation.sSouthGridNo;

	if ( sStartingGridNo == -1 )
		sStartingGridNo = gMapInformation.sCenterGridNo;

	if ( TileIsOutOfBounds( sStartingGridNo ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_INCORRECT_GRIDNO_ARTILLERY] );
		return FALSE;
	}

	// Locate item indices for Signal and HE shells defined by the active MOD. Evade usage of hard-code values.
	static UINT16 usSignalShellIndex = NOTHING;
	static UINT16 usHeShellIndex = NOTHING;
	if (usSignalShellIndex == NOTHING || usHeShellIndex == NOTHING)
	{
		UINT16 findSignalShellIndex = 1700;  // try default Signal Shell item in 1.13
		UINT16 findHeShellIndex = 140;       // try default HE Shell item in 1.13
		if (HasItemFlag(findSignalShellIndex, SIGNAL_SHELL) == FALSE && GetFirstItemWithFlag(&findSignalShellIndex, SIGNAL_SHELL) == FALSE)
		{
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NO_SIGNAL_SHELL]);
			return FALSE;
		}
		UINT16 mortarIndex = GetLauncherFromLaunchable(findSignalShellIndex);
		if (mortarIndex != GetLauncherFromLaunchable(findHeShellIndex))
		{
			findHeShellIndex = GetLaunchableOfExplosionType(mortarIndex, EXPLOSV_NORMAL);
		}
		if (findHeShellIndex == NOTHING)
		{
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NO_DEFAULT_SHELL]);
			return FALSE;
		}
		// at this point both shells were found and are OK, so set it to static variables and never touch anymore:
		usSignalShellIndex = findSignalShellIndex;
		usHeShellIndex = findHeShellIndex;
	}

	// if a strike is ordered from the ENEMY_TEAM or MILITIA_TEAM, the number of mortars depends on the number of enemies/militia in that sector
	// number of waves depends on the number and quality of enemies/soldiers
	// only HE shells will be fired this way
	if ( bTeam == ENEMY_TEAM || bTeam == MILITIA_TEAM )
	{
		INT16 nummortars = 0;  // number of mortars determines size of wave (1 - 4)
		INT16 numwaves = 0;    // number of waves
		INT16 numshells = 0;   // number of shells
		INT16 numwavesMax = (INT16) Explosive[Item[usSignalShellIndex].ubClassIndex].ubDuration;

		SECTORINFO *pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

		if ( bTeam == ENEMY_TEAM )
		{
			// we also have to account for mobile groups
			GROUP *pGroup = gpGroupList;
			while ( pGroup )
			{
				if ( pGroup->usGroupTeam == bTeam && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
				{
					nummortars += pGroup->ubGroupSize;
					numshells += gSkillTraitValues.usVOMortarPointsTroop * pGroup->ubGroupSize;
				}
				pGroup = pGroup->next;
			}

			nummortars += pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites;
			nummortars /= gSkillTraitValues.usVOMortarCountDivisor;
			numshells += gSkillTraitValues.usVOMortarPointsAdmin * pSector->ubNumAdmins + gSkillTraitValues.usVOMortarPointsTroop * pSector->ubNumTroops + gSkillTraitValues.usVOMortarPointsElite * pSector->ubNumElites;
		}
		else if ( bTeam == MILITIA_TEAM )
		{
			UINT8 militia_green = MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA );
			UINT8 militia_troop = MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
			UINT8 militia_elite = MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );

			nummortars = (militia_green + militia_troop + militia_elite) / gSkillTraitValues.usVOMortarCountDivisor;
			numshells = gSkillTraitValues.usVOMortarPointsAdmin * militia_green + gSkillTraitValues.usVOMortarPointsTroop * militia_troop + gSkillTraitValues.usVOMortarPointsElite * militia_elite;
		}

		// turn number of mortar points into number of shells; in case of "militia use sector ammo" option, numshells
		// represents max potential shells militia can shot for this artillery strike.
		numshells = numshells / gSkillTraitValues.usVOMortarShellDivisor;

		if (numshells <= 0)
		{
			if (bTeam == MILITIA_TEAM)  // player does not care if enemy team has not enough points to strike
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NOT_ENOUGH_MORTAR_SHELLS] );
			return FALSE;
		}

		if (nummortars <= 0)
		{
			if (bTeam == MILITIA_TEAM)  // player does not care if enemy team has not enough men to strike
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NO_MORTARS]);
			return FALSE;
		}

		numwaves = max(1, numshells / nummortars);
		if (gSkillTraitValues.fROArtilleryDistributedOverTurns)  // if delay between waves is enabled, we shouldn't overextend, so trim to
			numwaves = min(numwaves, numwavesMax);               // signal duration; it doesn't matter if delay is disabled.

		// send a signal shell at first. This marks the area that the shells will come in
		ArtilleryStrike(usSignalShellIndex, this->ubID + 2, sStartingGridNo, sTargetGridNo);

		// we just 'plant' the mortar shells as bombs. We time them so that they will be fired at the beginning of the next turn
		// for every 'wave' of shells, we just plant one and then clone them when firing
		// create mortar shell item
		OBJECTTYPE shellobj;
		CreateItem(usHeShellIndex, 100, &shellobj );

		shellobj.fFlags |= OBJECT_ARMED_BOMB;
		shellobj[0]->data.misc.bDetonatorType = BOMB_TIMED;
		shellobj[0]->data.misc.usBombItem = shellobj.usItem;
		shellobj[0]->data.misc.ubBombOwner = this->ubID + 2;

		// delay in RT is one turn. In TB we have to make that 2 turns, as otherwise the attack can happen instantly.
		// Also use 2 if we are AI, otherwise the shells will fly immediately at the player's turn, giving him no chance to react (blame the way turns are handled)

		shellobj[0]->data.misc.bDelay = 1;
		if ( bTeam == ENEMY_TEAM || !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
			shellobj[0]->data.misc.bDelay += 1;

		// now set special flags - we simply abuse the ubWireNetworkFlag
		switch ( nummortars )
		{
		case 1:
			shellobj[0]->data.ubWireNetworkFlag = ARTILLERY_STRIKE_COUNT_1;
			break;

		case 2:
			shellobj[0]->data.ubWireNetworkFlag = ARTILLERY_STRIKE_COUNT_2;
			break;

		case 3:
			shellobj[0]->data.ubWireNetworkFlag = (ARTILLERY_STRIKE_COUNT_1 | ARTILLERY_STRIKE_COUNT_2);
			break;

		case 4:
		default:
			shellobj[0]->data.ubWireNetworkFlag = ARTILLERY_STRIKE_COUNT_4;
			break;
		}

		for ( INT16 i = 0; i < numwaves; ++i )
		{
			AddItemToPool( sStartingGridNo, &shellobj, HIDDEN_ITEM, 1, WORLD_ITEM_ARMED_BOMB, 0 );

			// if option is set, delay each wave by one turn
			if ( gSkillTraitValues.fROArtilleryDistributedOverTurns )
				shellobj[0]->data.misc.bDelay += 1;
		}

		// update the sector Artillery time
		pSector->uiTimeAIArtillerywasOrdered = GetWorldTotalMin( );

		// extra xp for succesfully ordering an artillery strike
		StatChange( this, EXPERAMT, 10, TRUE );

		// we add a bit to the counter, thus the AI has to wait a bit between ordering strikes (otherwise they'll instantly order all available strikes)
		this->usSkillCounter[SOLDIER_COUNTER_RADIO_ARTILLERY] = 2;
	}
	else if ( bTeam == OUR_TEAM )
	{
		// if we call a strike from our mercs, everything gets more complicated. We don't calculate the number of mortars or shells as an estimate, we have to search the inventory 
		// of every merc fit for shelling in that sector for mortars and shells. But thanks to this, we can also other shell-types, like mustard or phosphor
		// we already know from IsValidArtilleryOrderSector(..) that someone in there must have a radio set and a mortar, no need to check that again
		// sadly, we have to run over this 2 times. On the first run, we have to search for all mortar items and remember them (there can be different mortar systems, can't fire a 40mm shell with a 60mm mortar)

		// as of 2013-09-25, I say it is no longer necessary to fire a signal shell first. The player can fire a signal shell (by mortar or hand) manually to mark one or more targets if he wants
		// if he does not do so, active vox operators will be targetted. Who knows, the vox operator might be doing a heroic last stand for all we know...
		//BOOLEAN signalshellfired = FALSE;
		const UINT8 maxFiringMortarsAmount = 5;
		UINT16 radiooperatorID = 0;
		UINT8 mortaritemcnt = 0;
		UINT16 mortararray[maxFiringMortarsAmount] = { 0 };

		SOLDIERTYPE* pSoldier = NULL;
		INT32 cnt = gTacticalStatus.Team[bTeam].bFirstID;
		INT32 lastid = gTacticalStatus.Team[bTeam].bLastID;
		for ( pSoldier = MercPtrs[cnt]; (cnt < lastid) && (mortaritemcnt < maxFiringMortarsAmount); ++cnt, ++pSoldier )
		{
			// check if soldier exists in this sector
			if ( !pSoldier || !pSoldier->bActive || pSoldier->sSectorX != sSectorX || pSoldier->sSectorY != sSectorY || pSoldier->bSectorZ != bSectorZ || pSoldier->bAssignment > ON_DUTY )
				continue;

			if ( pSoldier->CanUseRadio( ) )
				radiooperatorID = cnt;

			INT8 invsize = (INT8)pSoldier->inv.size( );	 // remember inventorysize, so we don't call size() repeatedly

			for ( INT8 bLoop = 0; (bLoop < invsize) && (mortaritemcnt < maxFiringMortarsAmount); ++bLoop )
			{
				if ( pSoldier->inv[bLoop].exists( ) == true && ItemIsMortar(pSoldier->inv[bLoop].usItem))
				{
					// if not already in list, remember this mortar
					bool alreadyInList = false;
					for (INT8 i = 0; i < mortaritemcnt; i++)
						if (mortararray[i] == pSoldier->inv[bLoop].usItem)
						{
							alreadyInList = true;
							break;
						}

					if (alreadyInList == false)
						mortararray[mortaritemcnt++] = pSoldier->inv[bLoop].usItem;
				}
			}
		}

		// safety check, this shouldn't be happening
		if ( !mortaritemcnt )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NO_MORTARS] );
			return FALSE;
		}

		// depending on wether the mortars have ammunition, a radio operator will give a different dialogue
		BOOLEAN shellsfired = FALSE;

		// second loop: check for all mortar shells and 'fire' them		
		cnt = gTacticalStatus.Team[bTeam].bFirstID;
		for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
		{
			// check if soldier exists in this sector
			if ( !pSoldier || !pSoldier->bActive || pSoldier->sSectorX != sSectorX || pSoldier->sSectorY != sSectorY || pSoldier->bSectorZ != bSectorZ || pSoldier->bAssignment > ON_DUTY )
				continue;

			INT8 shelldelay = 1;
			// In realtime the player could choose to put down a bomb right before a turn expires, so add 1 to the setting in RT
			if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) )
				++shelldelay;

			INT8 invsize = (INT8)pSoldier->inv.size( );									// remember inventorysize, so we don't call size() repeatedly
			for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
			{
				if ( pSoldier->inv[bLoop].exists( ) == true )
				{
					if (ItemIsMortar(pSoldier->inv[bLoop].usItem))
					{
						OBJECTTYPE* pAttObj = FindAttachmentByClass( &(pSoldier->inv[bLoop]), IC_BOMB );

						// as of 2013-09-25, also fire these, as they are no longer necessary for a barrage
						// only fire if not signal shell, we already fired one, no need to do so again
						if ( pAttObj && HasItemFlag(pAttObj->usItem, SIGNAL_SHELL) == FALSE )
						{
							// if option is set, delay each wave by one turn
							if ( gSkillTraitValues.fROArtilleryDistributedOverTurns )
								++shelldelay;

							// create mortar shell item
							OBJECTTYPE shellobj;
							CreateItem( pAttObj->usItem, 100, &shellobj );

							// plant bomb data
							shellobj.fFlags |= OBJECT_ARMED_BOMB;
							shellobj[0]->data.misc.bDetonatorType = BOMB_TIMED;
							shellobj[0]->data.misc.usBombItem = shellobj.usItem;
							shellobj[0]->data.misc.ubBombOwner = this->ubID + 2;

							shellobj[0]->data.misc.bDelay = shelldelay;

							shellobj[0]->data.ubWireNetworkFlag = ARTILLERY_STRIKE_COUNT_1;

							AddItemToPool( sStartingGridNo, &shellobj, HIDDEN_ITEM, 1, WORLD_ITEM_ARMED_BOMB, 0 );

							shellsfired = TRUE;

							DeductAmmo( pSoldier, bLoop );
						}
					}

					if ( Item[pSoldier->inv[bLoop].usItem].usItemClass == IC_BOMB )
					{
						// found a bomb - if this fits any found mortar, fire it
						for ( UINT8 i = 0; i < mortaritemcnt; ++i )
						{
							if ( ValidLaunchable( pSoldier->inv[bLoop].usItem, mortararray[i] ) )
							{
								OBJECTTYPE* pShellObj = &(pSoldier->inv[bLoop]);							// ... get pointer for this item ...

								// only fire if not signal shell, we already fired one, no need to do so again
								if ( pShellObj && !HasItemFlag( pShellObj->usItem, SIGNAL_SHELL ) )
								{
									// if option is set, delay each wave by one turn
									if ( gSkillTraitValues.fROArtilleryDistributedOverTurns )
										++shelldelay;

									// create mortar shell item
									OBJECTTYPE shellobj;
									CreateItem( pShellObj->usItem, 100, &shellobj );

									// plant bomb data
									shellobj.fFlags |= OBJECT_ARMED_BOMB;
									shellobj[0]->data.misc.bDetonatorType = BOMB_TIMED;
									shellobj[0]->data.misc.usBombItem = shellobj.usItem;
									shellobj[0]->data.misc.ubBombOwner = this->ubID + 2;

									shellobj[0]->data.misc.bDelay = shelldelay;

									shellobj[0]->data.ubWireNetworkFlag = ARTILLERY_STRIKE_COUNT_1;

									for ( INT16 j = 0; j < pShellObj->ubNumberOfObjects; ++j )
									{
										AddItemToPool( sStartingGridNo, &shellobj, HIDDEN_ITEM, 1, WORLD_ITEM_ARMED_BOMB, 0 );

										shellsfired = TRUE;
									}

									// remove the shells: Delete object
									DeleteObj( pShellObj );
								}
							}
						}
					}
				}
			}
		}

		if ( radiooperatorID )
		{
			pSoldier = MercPtrs[radiooperatorID];

			// also drain the other guy's radio batteries
			pSoldier->UseRadio( );

			if ( shellsfired )
				TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, BATTLE_SOUND_OK1, 500 );
			else
				DelayedTacticalCharacterDialogue( pSoldier, QUOTE_OUT_OF_AMMO );
		}

		if ( shellsfired )
		{
			// extra xp for succesfully ordering an artillery strike
			StatChange( this, EXPERAMT, 10, TRUE );

			// we add a bit to the counter, thus the AI has to wait a bit between ordering strikes (otherwise they'll instantly order all available strikes)
			this->usSkillCounter[SOLDIER_COUNTER_RADIO_ARTILLERY] = 2;
		}
	}
	else
		// how did this even happen?
		return FALSE;

	if ( bTeam == ENEMY_TEAM )
		gCurrentIncident.usIncidentFlags |= INCIDENT_ARTILLERY_ENEMY;
	else
		gCurrentIncident.usIncidentFlags |= INCIDENT_ARTILLERY_PLAYERSIDE;

	return TRUE;
}

BOOLEAN SOLDIERTYPE::IsJamming( )
{
	if ( usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_JAMMING )
	{
		if ( CanUseRadio( FALSE ) )
			return TRUE;
		// if we cannot use the radio, remove that flag hile we're at it
		else
			usSoldierFlagMask &= ~SOLDIER_RADIO_OPERATOR_JAMMING;
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::JamCommunications( )
{
	// not possible if already jamming
	if ( usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_JAMMING )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ALREADY_JAMMING] );
		return FALSE;
	}

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio( ) )
		return FALSE;

	// stop other radio activities
	SwitchOffRadio( );

	// add flag
	usSoldierFlagMask |= SOLDIER_RADIO_OPERATOR_JAMMING;

	// play sound
	PlayJA2SampleFromFile( "Sounds\\radioerror2.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

	return TRUE;
}

BOOLEAN SOLDIERTYPE::IsScanning( )
{
	if ( usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_SCANNING )
	{
		if ( CanUseRadio( FALSE ) )
			return TRUE;
		// if we cannot use the radio, remove that flag hile we're at it
		else
			usSoldierFlagMask &= ~SOLDIER_RADIO_OPERATOR_SCANNING;
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::ScanForJam( )
{
	// not possible if already scanning
	if ( usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_SCANNING )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ALREADY_SCANNING] );
		return FALSE;
	}

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio( ) )
		return FALSE;

	// stop other radio activities
	SwitchOffRadio( );

	// add flag
	usSoldierFlagMask |= SOLDIER_RADIO_OPERATOR_SCANNING;

	// play sound
	PlayJA2SampleFromFile( "Sounds\\scan1.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

	return TRUE;
}

BOOLEAN SOLDIERTYPE::IsRadioListening( )
{
	return ((usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_LISTENING) && CanUseRadio( FALSE ));
}

BOOLEAN SOLDIERTYPE::RadioListen( )
{
	// not possible if already scanning
	if ( usSoldierFlagMask & SOLDIER_RADIO_OPERATOR_LISTENING )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ALREADY_LISTENING] );
		return FALSE;
	}

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio( ) )
		return FALSE;

	// stop other radio activities
	SwitchOffRadio( );

	// add flag
	usSoldierFlagMask |= SOLDIER_RADIO_OPERATOR_LISTENING;

	// play sound
	PlayJA2SampleFromFile( "Sounds\\scan1.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

	return TRUE;
}

// Flugente: order reinforcements from src sector to target sector
extern BOOLEAN CallMilitiaReinforcements( INT16 sTargetMapX, INT16 sTargetMapY, INT16 sSrcMapX, INT16 sSrcMapY, UINT16 sNumber );

BOOLEAN SOLDIERTYPE::RadioCallReinforcements( UINT32 usSector, UINT16 sNumber )
{
	if ( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio( ) )
		return FALSE;

	// check wether radio frequencies are jammed. Not possible to do this in CanUseSkill(), as CanUseRadio() only checks if we can theoretically order
	if ( SectorJammed( ) )
	{
		// error message and sound is handled in UseRadio()
		return FALSE;
	}

	// Flugente: order reinforcements from src sector to target sector
	if ( CallMilitiaReinforcements( this->sSectorX, this->sSectorY, SECTORX( usSector ), SECTORY( usSector ), sNumber ) )
	{
		CHAR16 pStr2[128];
		GetSectorIDString( SECTORX( usSector ), SECTORY( usSector ), 0, pStr2, FALSE );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ORDERS_REINFORCEMENTS], this->name, pStr2 );

		// play sound
		PlayJA2SampleFromFile( "Sounds\\scan1.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

		return TRUE;
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::SwitchOffRadio( )
{
	// erasing the flags is enough
	usSoldierFlagMask &= ~(SOLDIER_RADIO_OPERATOR_JAMMING | SOLDIER_RADIO_OPERATOR_SCANNING | SOLDIER_RADIO_OPERATOR_LISTENING);

	return TRUE;
}

BOOLEAN SOLDIERTYPE::RadioOrderAllTurnCoatToSwitchSides()
{
	// not possible if already scanning
	if ( !gSkillTraitValues.fCOTurncoats )
		return FALSE;

	// use the radio, this handles animation, batteries etc.
	if ( !UseRadio() )
		return FALSE;

	OrderAllTurnCoatToSwitchSides();

	return TRUE;
}

// display and error sound used either when the radio set fails or the sector is jammed - the player knows of the error, but cannot be sure of the cause
void
SOLDIERTYPE::RadioFail( )
{
	// only display message and play sound if on player team
	if ( this->bTeam == gbPlayerNum && this->bInSector )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_RADIO_ACTION_FAILED] );

		PlayJA2SampleFromFile( "Sounds\\radioerror.wav", RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );
	}
}

// Flugente: spotter
BOOLEAN SOLDIERTYPE::IsSpotting( )
{
	if ( this->usSkillCounter[SOLDIER_COUNTER_SPOTTER] > 0 )
	{
		// do we still fulfil the requirements?
		if ( CanSpot( ) )
		{
			// we are only a spotter if we did this long enough
			if ( this->usSkillCounter[SOLDIER_COUNTER_SPOTTER] >= gGameExternalOptions.usSpotterPreparationTurns )
				return TRUE;
			else
				return FALSE;
		}

		// no item -> lose status
		this->usSkillCounter[SOLDIER_COUNTER_SPOTTER] = 0;
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::CanSpot( INT32 sTargetGridNo )
{
	if ( this->stats.bLife < OKLIFE || this->flags.fMercAsleep || this->bCollapsed || (this->usSoldierFlagMask & SOLDIER_POW) )
		return FALSE;

	// additional checks if we want to know wether we can target a specific location
	if ( sTargetGridNo != NOWHERE && PythSpacesAway( this->sGridNo, sTargetGridNo ) >= 2 * gGameExternalOptions.usSpotterRange )
	{
		UINT16 usSightLimit = this->GetMaxDistanceVisible( sTargetGridNo, this->pathing.bLevel, CALC_FROM_WANTED_DIR );

		INT32 val = SoldierToVirtualSoldierLineOfSightTest( this, sTargetGridNo, this->pathing.bLevel, gAnimControl[this->usAnimState].ubEndHeight, FALSE, usSightLimit );

		// error if we cannot see the target
		if ( !val )
			return FALSE;
	}

	// no item -> no spotting
	if ( !(this->inv[HANDPOS].exists( ) && GetObjectModifier( this, &(this->inv[HANDPOS]), gAnimControl[this->usAnimState].ubEndHeight, ITEMMODIFIER_SPOTTER ))
		 && !(this->inv[SECONDHANDPOS].exists( ) && GetObjectModifier( this, &(this->inv[SECONDHANDPOS]), gAnimControl[this->usAnimState].ubEndHeight, ITEMMODIFIER_SPOTTER )) )
		 return FALSE;

	return TRUE;
}

BOOLEAN SOLDIERTYPE::BecomeSpotter( INT32 sTargetGridNo )
{
	// not possible if already scanning
	if ( this->usSkillCounter[SOLDIER_COUNTER_SPOTTER] )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_ALREADY_SPOTTING] );
		return FALSE;
	}

	if ( !CanSpot( sTargetGridNo ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_CANNOT_SPOT_LOCATION] );
		return FALSE;
	}

	// deduct APs
	DeductPoints( this, APBPConstants[AP_SPOTTER], 0, 0 );

	// add to counter
	this->usSkillCounter[SOLDIER_COUNTER_SPOTTER] = 1;

	// stop any multi-turn action
	CancelMultiTurnAction( FALSE );

	return TRUE;
}

// Flugente: enemy roles
BOOLEAN SOLDIERTYPE::HasSniper( )
{
	INT8 invsize = (INT8)inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) && Item[inv[bLoop].usItem].usItemClass == IC_GUN && Weapon[Item[inv[bLoop].usItem].ubClassIndex].ubWeaponType == GUN_SN_RIFLE )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// AI-only: can we heal a wounded ally? Do NOT, repeat, NOT use this with mercs!
BOOLEAN		SOLDIERTYPE::CanMedicAI( )
{
	if ( !gGameExternalOptions.fEnemyRoles || !gGameExternalOptions.fEnemyMedics || this->bTeam != ENEMY_TEAM )
		return FALSE;

	// this is not for tanks
	if ( ARMED_VEHICLE( this ) || ENEMYROBOT( this ) )
		return FALSE;

	if ( HAS_SKILL_TRAIT( this, DOCTOR_NT ) )
	{
		if ( FindFirstAidKit( this ) != NO_SLOT || FindMedKit( this ) != NO_SLOT )
			return TRUE;
	}

	return FALSE;
}

// AI-only: heal a nearby friend. Do NOT, repeat, NOT use this with mercs!
BOOLEAN		SOLDIERTYPE::AIDoctorFriend( )
{
	if ( this->bTeam != ENEMY_TEAM )
		return FALSE;

	// we can only free people we are facing
	INT32 nextGridNoinSight = NewGridNo( this->sGridNo, DirectionInc( this->ubDirection ) );

	UINT16 target = WhoIsThere2( nextGridNoinSight, this->pathing.bLevel );

	// is there somebody?
	if ( target != NOBODY )
	{
		SOLDIERTYPE* pSoldier = MercPtrs[target];

		if ( pSoldier->bTeam != ENEMY_TEAM )
			return FALSE;

		// this is not for tanks
		if ( ARMED_VEHICLE( pSoldier ) || ENEMYROBOT( pSoldier ) )
			return FALSE;

		// if this guy is wounded, heal him (should always be the case, otherwise this function was called needlessly)
		if ( pSoldier->iHealableInjury > 0 )
		{
			// move medkit into hand - if we don't have a medkit in our hands, abort
			if (!MakeSureMedKitIsInHand(this, true))
				return FALSE;

			if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH )
			{
				// sevenfm: first change to stationary
				this->SoldierGotoStationaryStance();

				this->EVENT_InitNewSoldierAnim( START_AID, 0, FALSE );
			}

			// sevenfm: change target to stationary
			if (pSoldier->stats.bLife >= OKLIFE && pSoldier->bBreath >= OKBREATH && !pSoldier->bCollapsed)
				pSoldier->SoldierGotoStationaryStance();

			// AI medics always perform surgery
			this->fDoingSurgery = TRUE;

			UINT16 usKitPts = TotalPoints( &(this->inv[HANDPOS]) );

			// note the current hp
			INT8 oldlife = pSoldier->stats.bLife;

			UINT16 uiPointsUsed = this->SoldierDressWound( pSoldier, usKitPts, usKitPts );

			UseKitPoints( &(this->inv[HANDPOS]), (UINT16)(uiPointsUsed * gGameExternalOptions.dEnemyMedicMedKitDrainFactor), this );

			// healing done will be displayed the next time the player sees this soldier
			pSoldier->flags.fDisplayDamage = TRUE;
			pSoldier->sDamage -= pSoldier->stats.bLife - oldlife;

			// alert both soldiers
			this->aiData.bAlertStatus = max( this->aiData.bAlertStatus, STATUS_RED );
			pSoldier->aiData.bAlertStatus = max( pSoldier->aiData.bAlertStatus, STATUS_RED );

			return TRUE;
		}
	}

	return FALSE;
}

// AI-only: heal self. Do NOT, repeat, NOT use this with mercs!
BOOLEAN		SOLDIERTYPE::AIDoctorSelf( )
{
	if ( this->bTeam != ENEMY_TEAM )
		return FALSE;

	// if this guy is wounded, heal him (should always be the case, otherwise this function was called needlessly)
	if ( this->iHealableInjury > 0 )
	{
		// move medkit into hand - if we don't have a medkit in our hands, abort
		if (!MakeSureMedKitIsInHand(this, true))
			return FALSE;

		if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH )
		{
			// sevenfm: first change to stationary
			this->SoldierGotoStationaryStance();

			this->EVENT_InitNewSoldierAnim( START_AID, 0, FALSE );
		}

		// AI medics always perform surgery
		this->fDoingSurgery = TRUE;

		UINT16 usKitPts = TotalPoints( &(this->inv[HANDPOS]) );

		// note the current hp
		INT8 oldlife = this->stats.bLife;

		UINT16 uiPointsUsed = this->SoldierDressWound( this, usKitPts, usKitPts );

		UseKitPoints( &(this->inv[HANDPOS]), (UINT16)(uiPointsUsed * gGameExternalOptions.dEnemyMedicMedKitDrainFactor), this );

		// healing done will be displayed the next time the player sees this soldier
		this->flags.fDisplayDamage = TRUE;
		this->sDamage -= this->stats.bLife - oldlife;

		// alert ourself
		this->aiData.bAlertStatus = max( this->aiData.bAlertStatus, STATUS_RED );

		return TRUE;
	}

	return FALSE;
}

// Flugente: boxing fix: this shall be the only location where the boxing flag gets removed (easier debugging)
void	SOLDIERTYPE::DeleteBoxingFlag( )
{
	if ( flags.uiStatusFlags & SOLDIER_BOXER )
		flags.uiStatusFlags &= (~SOLDIER_BOXER);
}

// Flugente: disease
void	SOLDIERTYPE::Infect( UINT8 aDisease )
{
	if ( !gGameExternalOptions.fDisease 
		|| aDisease >= NUM_DISEASES )
		return;

	// diseases should not affect machines
	if ( (this->flags.uiStatusFlags & SOLDIER_VEHICLE) || AM_A_ROBOT( this ) )
		return;

	// do not infect us if we are already infected
	if ( !( Disease[aDisease].usDiseaseProperties & DISEASE_PROPERTY_CANREINFECT ) && this->sDiseasePoints[aDisease] > 0 )
		return;

	// we are getting infected. Raise our disease points, but not over the level of an infection
	if ( this->sDiseasePoints[aDisease] <= Disease[aDisease].sInfectionPtsFull )
	{
		this->sDiseasePoints[aDisease] = min( this->sDiseasePoints[aDisease] + Disease[aDisease].sInfectionPtsInitial, Disease[aDisease].sInfectionPtsFull );

		// possibly add a new disability
		if ( Disease[aDisease].usDiseaseProperties & DISEASE_PROPERTY_ADD_DISABILITY )
		{
			// take a random disability we don't yet have and give it to us
			std::vector<UINT8> disabilitieswedonthaveset;
			for ( UINT8 i = NO_DISABILITY + 1; i < min( 31, NUM_DISABILITIES ); ++i )
			{
				if ( !DoesMercHaveDisability( this, i ) )
					disabilitieswedonthaveset.push_back(i);
			}

			if ( !disabilitieswedonthaveset.empty() )
			{
				UINT8 newdisability = disabilitieswedonthaveset[Random( disabilitieswedonthaveset.size() )];
				this->AddDisability( newdisability );
			}
		}

		if ( !( this->sDiseaseFlag[aDisease] & SOLDIERDISEASE_OUTBREAK ) && this->sDiseasePoints[aDisease] > Disease[aDisease].sInfectionPtsOutbreak )
		{
			this->sDiseaseFlag[aDisease] |= SOLDIERDISEASE_OUTBREAK;

			this->AnnounceDisease( aDisease );
		}

		// remove later on, for testing only
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s was infected with %s", gMercProfiles[this->ubProfile].zNickname, Disease[aDisease].szName );
	}
}

void	SOLDIERTYPE::AddDiseasePoints( UINT8 aDisease, INT32 aVal )
{
	if ( !gGameExternalOptions.fDisease )
		return;

	// diseases should not affect machines
	if ( (this->flags.uiStatusFlags & SOLDIER_VEHICLE) || AM_A_ROBOT( this ) )
		return;

	if ( aDisease < NUM_DISEASES )
	{
		this->sDiseasePoints[aDisease] = min( Disease[aDisease].sInfectionPtsFull, max( this->sDiseasePoints[aDisease] + aVal, -Disease[aDisease].sInfectionPtsOutbreak ) );

		// if the disease 'breaks out', make it known
		if ( this->sDiseasePoints[aDisease] > Disease[aDisease].sInfectionPtsOutbreak )
		{
			this->sDiseaseFlag[aDisease] |= SOLDIERDISEASE_OUTBREAK;

			if ( !(this->sDiseaseFlag[aDisease] & SOLDIERDISEASE_DIAGNOSED) )
				this->AnnounceDisease( aDisease );
		}

		// once disease is fullblown, some diseases reverse themself
		if ( (Disease[aDisease].usDiseaseProperties & DISEASE_PROPERTY_REVERSEONFULL) && this->sDiseasePoints[aDisease] >= Disease[aDisease].sInfectionPtsFull )
		{
			this->sDiseaseFlag[aDisease] |= SOLDIERDISEASE_REVERSEAL;
		}

		// if disease is cured, remove traces of it
		if ( this->sDiseasePoints[aDisease] <= 0 )
		{
			// if disease was known and this guy is under player control, let the player know the good news
			if ( this->sDiseaseFlag[aDisease] & SOLDIERDISEASE_DIAGNOSED && this->bTeam == gbPlayerNum )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szDiseaseText[TEXT_DISEASE_CURED], this->GetName( ), Disease[aDisease].szName );

			this->sDiseaseFlag[aDisease] &= ~(SOLDIERDISEASE_DIAGNOSED | SOLDIERDISEASE_OUTBREAK | SOLDIERDISEASE_SPLINTAPPLIED_LEG  | SOLDIERDISEASE_SPLINTAPPLIED_ARM );
		}
	}
}

void	SOLDIERTYPE::AnnounceDisease( UINT8 aDisease )
{
	this->sDiseaseFlag[aDisease] |= SOLDIERDISEASE_DIAGNOSED;

	if ( this->bTeam == gbPlayerNum )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szDiseaseText[TEXT_DISEASE_DIAGNOSE_GENERAL], this->GetName( ), Disease[aDisease].szName );

	// add to our records.
	if ( this->ubProfile != NO_PROFILE )
		gMercProfiles[this->ubProfile].records.usTimesInfected += 1;
}

void	SOLDIERTYPE::AddDisability( UINT8 aDisability )
{
	this->usDisabilityFlagMask |= ( 1 << (aDisability - 1 ) );
}

// Flugente: can we apply a medical splint to this guy?
bool	SOLDIERTYPE::CanReceiveSplint()
{
	// not during combat
	if ( gTacticalStatus.uiFlags & INCOMBAT )
		return FALSE;

	//  must be player team
	if ( this->bTeam != gbPlayerNum )
		return FALSE;

	if ( !gGameExternalOptions.fDisease
		|| !gGameExternalOptions.fDiseaseSevereLimitations )
		return FALSE;

	// check whether we have a disease that limits arm/leg use without having a splint 
	for ( int i = 0; i < NUM_DISEASES; ++i )
	{
		if ( this->sDiseasePoints[i] > 0 && this->sDiseaseFlag[i] & SOLDIERDISEASE_DIAGNOSED )
		{ 
			if ( (Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_ARMS && !( this->sDiseaseFlag[i] & SOLDIERDISEASE_SPLINTAPPLIED_ARM ) ) 
				|| ( Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_LEGS && !( this->sDiseaseFlag[i] & SOLDIERDISEASE_SPLINTAPPLIED_LEG ) ) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

// do we have any disease? fDiagnosedOnly: check for wether we know of this infection fHealableOnly: check wether it can be healed
BOOLEAN SOLDIERTYPE::HasDisease( BOOLEAN fDiagnosedOnly, BOOLEAN fHealableOnly, BOOLEAN fSymbolOnly )
{
	if ( !gGameExternalOptions.fDisease )
		return FALSE;

	for ( int i = 0; i < NUM_DISEASES; ++i )
	{
		// disease is relevant if we are infected and are not looking for symbols only while the disease has no symbol
		if ( this->sDiseasePoints[i] > 0 && !(fSymbolOnly && (Disease[i].usDiseaseProperties & DISEASE_PROPERTY_HIDESYMBOL)) )
		{
			// only if we don't check for diagnosis, or we already know of this
			if ( !fDiagnosedOnly || (this->sDiseaseFlag[i] & SOLDIERDISEASE_DIAGNOSED) )
			{
				// only if we don't check for cure, or this can be cured
				if ( !fHealableOnly || (Disease[i].usDiseaseProperties & DISEASE_PROPERTY_CANBECURED) )
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

// Do we have an outbroken disease with a special property?
BOOLEAN SOLDIERTYPE::HasDiseaseWithFlag( UINT32 aFlag )
{
	if ( !gGameExternalOptions.fDisease )
		return FALSE;

	for ( int i = 0; i < NUM_DISEASES; ++i )
	{
		// disease is relevant if we are infected and are not looking for symbols only while the disease has no symbol
		if ( ( Disease[i].usDiseaseProperties & aFlag ) && this->sDiseasePoints[i] > 0 && ( this->sDiseaseFlag[i] & SOLDIERDISEASE_OUTBREAK ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// get the magnitude of a disease we might have, used to determine wether there are any effects
FLOAT	SOLDIERTYPE::GetDiseaseMagnitude( UINT8 aDisease )
{
	if ( !gGameExternalOptions.fDisease )
		return 0.0f;

	// diseases only have effects once they have broken out (otherwise stuff happens without the player having any clue as to why)
	if ( aDisease < NUM_DISEASES && this->sDiseasePoints[aDisease] > 0 && (this->sDiseaseFlag[aDisease] & SOLDIERDISEASE_OUTBREAK) )
	{
		return ((FLOAT)this->sDiseasePoints[aDisease] / (FLOAT)Disease[aDisease].sInfectionPtsFull);
	}

	return 0.0f;
}

void SOLDIERTYPE::PrintDiseaseDesc( CHAR16* apStr, BOOLEAN fFullDesc )
{
	if ( !gGameExternalOptions.fDisease )
		return;

	// only for living mercs with a profile with a valid infection method
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE || this->ubProfile == NO_PROFILE )
		return;

	BOOLEAN fShowExactPoints = FALSE;
	if ( DEBUG_CHEAT_LEVEL( ) )
		fShowExactPoints = TRUE;

	CHAR16	atStr[500];
	swprintf( atStr, L"\n  \n" );
	wcscat( apStr, atStr );

	for ( int i = 0; i < NUM_DISEASES; ++i )
	{
		if ( this->sDiseaseFlag[i] & SOLDIERDISEASE_DIAGNOSED )
		{
			if ( fShowExactPoints )
			{
				swprintf( atStr, L"\n\n%s - %d / %d\n", Disease[i].szFatName, this->sDiseasePoints[i], Disease[i].sInfectionPtsFull );
			}
			else
			{
				swprintf( atStr, L"\n\n%s\n", Disease[i].szFatName );
			}

			wcscat( apStr, atStr );

			// if we give a full description, also print out the effects at the moment
			if ( fFullDesc )
			{
				swprintf( atStr, L"%s\n", Disease[i].szDescription );
				wcscat( apStr, atStr );

				FLOAT magnitude = GetDiseaseMagnitude( i );

				for ( int j = 0; j < INFST_MAX; ++j )
				{
					INT8 val = Disease[i].sEffStat[j] * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[j], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					INT8 val = Disease[i].sEffAP * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_AP], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					UINT8 val = Disease[i].usMaxBreath * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_MAXBREATH], L"-", val );
						wcscat( apStr, atStr );
					}
				}

				{
					INT8 val = Disease[i].sEffCarryStrength * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_CARRYSTRENGTH], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					FLOAT val = (FLOAT)(Disease[i].sLifeRegenHundreds) * magnitude / 100;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_LIFEREGENHUNDREDS], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					INT8 val = Disease[i].sNeedToSleep * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_NEEDTOSLEEP], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					INT16 val = Disease[i].sDrinkModifier * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_DRINK], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				{
					INT16 val = Disease[i].sFoodModifier * magnitude;

					if ( val )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_FOOD], val > 0 ? L"+" : L"", val );
						wcscat( apStr, atStr );
					}
				}

				if ( (this->ubProfile == BUNS || this->ubProfile == BUNS_CHAOTIC) && ( Disease[i].usDiseaseProperties & DISEASE_PROPERTY_PTSD_BUNS ) )
				{
					swprintf( atStr, szDiseaseText[TEXT_DISEASE_PTSD_BUNS_SPECIAL] );
					wcscat( apStr, atStr );
				}

				if ( gGameExternalOptions.fDiseaseSevereLimitations )
				{
					if ( Disease[i].usDiseaseProperties & DISEASE_PROPERTY_ADD_DISABILITY )
					{
						swprintf( atStr, szDiseaseText[TEXT_DISEASE_ADD_DISABILITY] );
						wcscat( apStr, atStr );
					}

					bool splintapplied = ( this->sDiseaseFlag[i] & ( SOLDIERDISEASE_SPLINTAPPLIED_ARM | SOLDIERDISEASE_SPLINTAPPLIED_LEG ) );

					if ( Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_ARMS )
					{
						if ( splintapplied )
							swprintf( atStr, szDiseaseText[TEXT_DISEASE_LIMITED_ARMS_SPLINT] );
						else
							swprintf( atStr, szDiseaseText[TEXT_DISEASE_LIMITED_ARMS] );

						wcscat( apStr, atStr );
					}

					if ( Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_LEGS )
					{
						if ( splintapplied )
							swprintf( atStr, szDiseaseText[TEXT_DISEASE_LIMITED_LEGS_SPLINT] );
						else
							swprintf( atStr, szDiseaseText[TEXT_DISEASE_LIMITED_LEGS] );

						wcscat( apStr, atStr );
					}
				}
			}
		}
		else if ( fShowExactPoints && this->sDiseasePoints[i] > 0 )
		{
			swprintf( atStr, szDiseaseText[TEXT_DISEASE_UNDIAGNOSED], Disease[i].szFatName, this->sDiseasePoints[i], Disease[i].sInfectionPtsFull );
			wcscat( apStr, atStr );
		}
	}
}

void SOLDIERTYPE::PrintFoodDesc( CHAR16* apStr, BOOLEAN fFullDesc )
{
	if ( !UsingFoodSystem() )
		return;

	// only for living mercs with a profile
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE || this->ubProfile == NO_PROFILE )
		return;
	
	CHAR16	atStr[500];
	swprintf( atStr, L"" );

	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( this, &foodsituation, &watersituation );

	INT8 bMoraleModifier_Food  = FoodMoraleMods[foodsituation].bMoraleModifier;
	INT8 bMoraleModifier_Water = FoodMoraleMods[watersituation].bMoraleModifier;

	INT8 bSleepModifier_Food  = FoodMoraleMods[foodsituation].bSleepModifier;
	INT8 bSleepModifier_Water = FoodMoraleMods[watersituation].bSleepModifier;

	INT8 bBreathRegenModifier_Food  = FoodMoraleMods[foodsituation].bBreathRegenModifier;
	INT8 bBreathRegenModifier_Water = FoodMoraleMods[watersituation].bBreathRegenModifier;

	INT8 bAssignmentEfficiencyModifier_Food  = FoodMoraleMods[foodsituation].bAssignmentEfficiencyModifier;
	INT8 bAssignmentEfficiencyModifier_Water = FoodMoraleMods[watersituation].bAssignmentEfficiencyModifier;

	UINT8 ubStatDamageChance_Food  = FoodMoraleMods[foodsituation].ubStatDamageChance;
	UINT8 ubStatDamageChance_Water = FoodMoraleMods[watersituation].ubStatDamageChance;
	
	swprintf( atStr, szFoodText[0], (INT32)(100 * (this->bDrinkLevel - FOOD_MIN) / FOOD_HALF_RANGE) );
	wcscat( apStr, atStr );

	if ( watersituation != FOOD_NORMAL )
	{
		if ( bMoraleModifier_Water )
		{
			swprintf( atStr, szFoodText[2], bMoraleModifier_Water > 0 ? L"+" : L"", bMoraleModifier_Water );
			wcscat( apStr, atStr );
		}

		if ( bSleepModifier_Water )
		{
			swprintf( atStr, szFoodText[3], bSleepModifier_Water > 0 ? L"+" : L"", bSleepModifier_Water );
			wcscat( apStr, atStr );
		}

		if ( bBreathRegenModifier_Water )
		{
			swprintf( atStr, szFoodText[4], bBreathRegenModifier_Water > 0 ? L"+" : L"", bBreathRegenModifier_Water );
			wcscat( apStr, atStr );
		}

		if ( bAssignmentEfficiencyModifier_Water )
		{
			swprintf( atStr, szFoodText[5], bAssignmentEfficiencyModifier_Water > 0 ? L"+" : L"", bAssignmentEfficiencyModifier_Water );
			wcscat( apStr, atStr );
		}

		if ( ubStatDamageChance_Water )
		{
			swprintf( atStr, szFoodText[6], ubStatDamageChance_Water > 0 ? L"+" : L"", ubStatDamageChance_Water );
			wcscat( apStr, atStr );
		}
	}

	swprintf( atStr, szFoodText[1], (INT32)(100 * (this->bFoodLevel - FOOD_MIN) / FOOD_HALF_RANGE) );
	wcscat( apStr, atStr );

	if ( foodsituation != FOOD_NORMAL )
	{
		if ( bMoraleModifier_Food )
		{
			swprintf( atStr, szFoodText[2], bMoraleModifier_Food > 0 ? L"+" : L"", bMoraleModifier_Food );
			wcscat( apStr, atStr );
		}

		if ( bSleepModifier_Food )
		{
			swprintf( atStr, szFoodText[3], bSleepModifier_Food > 0 ? L"+" : L"", bSleepModifier_Food );
			wcscat( apStr, atStr );
		}

		if ( bBreathRegenModifier_Food )
		{
			swprintf( atStr, szFoodText[4], bBreathRegenModifier_Food > 0 ? L"+" : L"", bBreathRegenModifier_Food );
			wcscat( apStr, atStr );
		}

		if ( bAssignmentEfficiencyModifier_Food )
		{
			swprintf( atStr, szFoodText[5], bAssignmentEfficiencyModifier_Food > 0 ? L"+" : L"", bAssignmentEfficiencyModifier_Food );
			wcscat( apStr, atStr );
		}

		if ( ubStatDamageChance_Food )
		{
			swprintf( atStr, szFoodText[6], ubStatDamageChance_Food > 0 ? L"+" : L"", ubStatDamageChance_Food );
			wcscat( apStr, atStr );
		}
	}
}

void SOLDIERTYPE::PrintSleepDesc( CHAR16* apStr )
{
	// only for people
	if ( this->flags.uiStatusFlags & SOLDIER_VEHICLE )
		return;

	CHAR16	atStr[100];
	swprintf( atStr, L"\n  \n" );
	wcscat( apStr, atStr );

	swprintf( atStr, gpStrategicString[STR_BREATH_REGEN_SLEEP], this->GetSleepBreathRegeneration( ) );
	wcscat( apStr, atStr );
}

// get percentage protection from infections via contact
FLOAT  SOLDIERTYPE::GetDiseaseContactProtection( )
{
	FLOAT val = 0.0f;

	// if we wear special equipment, lower our chances of being infected
	FLOAT bestfacegear = 0.0f;
	FLOAT bestprotectivegear = 0.0f;
	for ( const auto &item : inv.get() )
	{
		if ( item.exists( ) )
		{
			if ( item[0]->data.objectStatus >= USABLE )
			{	
				if ( HasItemFlag( item.usItem, DISEASEPROTECTION_1 ) )
				{
					bestfacegear = max( bestfacegear, (FLOAT)(item[0]->data.objectStatus / 100) );
				}
				if ( HasItemFlag( item.usItem, DISEASEPROTECTION_2 ) )
				{
					bestprotectivegear = max( bestprotectivegear, (FLOAT)(item[0]->data.objectStatus / 100) );
				}
			}
		}
	}

	// up to 100% protection if face and hand protection is worn
	val += (bestfacegear + bestprotectivegear) / 2;

	// not higher than 100%
	return min( val, 1.0f );
}

INT16	SOLDIERTYPE::GetDiseaseResistance( )
{
	// Flugente: resistance can per definition only be between -100 and 100 (at least that's my definition)
	INT16 val = 0;

	if ( HAS_SKILL_TRAIT( this, SURVIVAL_NT ) )
		val += gSkillTraitValues.usSVDiseaseResistance;

	val += this->GetBackgroundValue( BG_RESI_DISEASE );

	val = max( -100, val );
	val = min( 100, val );

	return(val);
}

UINT16		SOLDIERTYPE::GetDiseaseDiagnosePoints()
{
	// determine our skill at detecting disease
	UINT16 skill = this->stats.bMedical / 2 + NUM_SKILL_TRAITS( this, DOCTOR_NT ) * 15;

	skill = ( skill * ( 100 + this->GetBackgroundValue( BG_PERC_DISEASE_DIAGNOSE ) ) ) / 100;

	FLOAT administrationmodifier = GetAdministrationModifier();
	skill *= administrationmodifier;

	return skill;
}

FLOAT		SOLDIERTYPE::GetBurialPoints( UINT16* apCorpses )
{
	if ( this->stats.bLife < OKLIFE || this->bSectorZ || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0.0f;

	if ( apCorpses )
	{
		SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( this->sSectorX, this->sSectorY )] );

		if ( pSectorInfo )
			*apCorpses = pSectorInfo->usNumCorpses;
	}

	// if not on correct assignment, no gain
	if ( this->bAssignment != BURIAL )
		return 0.0f;
	
	UINT32 val = 4 * EffectiveStrength( this, FALSE );
	
	ReducePointsForFatigue( this, &val );

	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) )	persmodifier -= 0.01f;
	if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) )	persmodifier -= 0.03f;
	
	// background modifier
	persmodifier += ( this->GetBackgroundValue( BG_BURIAL_ASSIGNMENT ) ) / 100.0f;

	// equipment modifier
	FLOAT bestequipmentmodifier = 1.0f;
	
	OBJECTTYPE* pObj = NULL;

	INT8 invsize = (INT8)inv.size();									// remember inventorysize, so we don't call size() repeatedly

	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )						// ... for all items in our inventory ...
	{
		// ... if Item exists and is canteen (that can have drink points) ...
		if ( inv[bLoop].exists() == true && Item[inv[bLoop].usItem].usBurialModifier )
		{
			OBJECTTYPE * pObj = &( this->inv[bLoop] );							// ... get pointer for this item ...

			if ( pObj != NULL )													// ... if pointer is not obviously useless ...
			{
				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					FLOAT modifier = 1.0f + ( Item[inv[bLoop].usItem].usBurialModifier * ( *pObj )[i]->data.objectStatus ) / 10000.0f;

					if ( modifier > bestequipmentmodifier )
						bestequipmentmodifier = modifier;
				}
			}
		}
	}

	FLOAT administrationmodifier = GetAdministrationModifier();

	FLOAT totalvalue = val * persmodifier * bestequipmentmodifier * administrationmodifier * 0.01f;

	// A most awesome merc in Meduna palace, disguised as a soldier, would have a value of 1.15 * 4.63 * 2 = 10.649 at this point.
	// This would be the place where we modify our intel gain rate.

	return totalvalue;
}

// Flugente: hourly breath regen calculation
INT8	SOLDIERTYPE::GetSleepBreathRegeneration( )
{
	// handle the sleep of this character, update bBreathMax based on sleep they have
	INT8 bMaxBreathRegain = 0;
	INT16 sSectorModifier = 100;
	FLOAT bDivisor = 0;

	// Determine how many hours a day this merc must sleep. Normally this would range between 6 and 12 hours.
	// Injuries and/or martial arts trait can change the limits to between 3 and 18 hours a day.
	bDivisor = CalcSoldierNeedForSleep( this );

	// HEADROCK HAM 3.6:
	// Night ops specialists sleep better during the day. Others sleep better during the night.
	// silversurfer: The code below did the complete opposite. A higher bDivisor means LESS regeneration. Fixed.
	if ( DayTime( ) )	//if (NightTime())
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( !HAS_SKILL_TRAIT( this, NIGHT_OPS_NT ) )
				bDivisor += 3;
		}
		else
			bDivisor += 4 - (2 * NUM_SKILL_TRAITS( this, NIGHTOPS_OT ));
	}
	else
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( HAS_SKILL_TRAIT( this, NIGHT_OPS_NT ) )
				bDivisor += 3;
		}
		else
			bDivisor += (2 * NUM_SKILL_TRAITS( this, NIGHTOPS_OT ));
	}

	// HEADROCK HAM 3.5: Read adjustment from local sector facilities
	if ( this->bSectorZ == 0 )
	{
		if ( this->flags.fMercAsleep )
		{
			sSectorModifier = GetSectorModifier( this, FACILITY_SLEEP_MOD );
		}
		else
		{
			// Resting can be done at a facility now, and the program will automatically apply a performance bonus
			// to this if the facility has one. If the character is simply resting ("On Duty", assigned to a squad),
			// then only Ambient effects take place.
			sSectorModifier = GetSectorModifier( this, FACILITY_PERFORMANCE_MOD );
		}
		bDivisor = (bDivisor * 100) / sSectorModifier;
	}

	// silversurfer: Items can provide a bonus to regeneration, sleeping bags for example.
	// They will not provide such bonus if the merc is already using a bed in a facility.
	if ( GetSoldierFacilityAssignmentIndex( this ) != FAC_PATIENT && GetSoldierFacilityAssignmentIndex( this ) != FAC_REST )
	{
		bDivisor = (bDivisor * 100) / (100 + GetInventorySleepModifier( this ));
	}

	// silversurfer: I moved all modifiers above this point because we don't want anybody to rest faster or slower than the already extreme thresholds.
	// Re-enforce limits
	bDivisor = __min( 18, __max( 3, bDivisor ) );

	// round up so the bonuses above make more sense
	bMaxBreathRegain = (50 / bDivisor + 0.5);

	// Limit so that characters can't regain faster than 3 hours, ever
	if ( bMaxBreathRegain > 17 )
	{
		bMaxBreathRegain = 17;
	}

	// if breath max is below the "really tired" threshold
	if ( this->bBreathMax < BREATHMAX_PRETTY_TIRED )
	{
		// real tired, rest rate is 50% higher (this is to prevent absurdly long sleep times for totally exhausted mercs)
		bMaxBreathRegain = (UINT8)(bMaxBreathRegain * 3 / 2);
	}

	return bMaxBreathRegain;
}

// Flugente: assumed character weight (without any items)
FLOAT	SOLDIERTYPE::GetBodyWeight()
{
	switch ( this->ubBodyType )
	{
	case REGMALE:
	case MANCIV:
		return 85.0f;
		break;

	case BIGMALE:
	case STOCKYMALE:
		return 110.0f;
		break;

	case REGFEMALE:
		return 75.0f;
		break;
	
	case FATCIV:
		return 100.0f;
		break;

	case MINICIV:
	case DRESSCIV:
		return 60.0f;
		break;

	case HATKIDCIV:
	case KIDCIV:
		return 40.0f;
		break;

	case CRIPPLECIV:
		return 75.0f;
		break;
	}

	return 80.0f;
}

// Flugente: are we crouched against cover from a specific direction? WARNING: This does not suffice to determine our cover!
BOOLEAN	SOLDIERTYPE::IsCrouchedAgainstCoverFromDir( UINT8 aDirection )
{
	// we must be active
	if ( !bActive )
		return FALSE;

	// only valid directions, please
	if ( aDirection >= NUM_WORLD_DIRECTIONS )
		return FALSE;

	// only possible if crouched
	if ( gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH )
		return FALSE;

	// we must be in a sector
	if ( !bInSector )
		return FALSE;

	// this is odd - invalid GridNo...
	if ( TileIsOutOfBounds( this->sGridNo ) )
		return FALSE;

	// not in a car...
	if ( this->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER) )
		return FALSE;

	// we test whether we are crouched against cover in a specific direction, so determine the gridno to test
	INT32 covergridno = NewGridNo( this->sGridNo, DirectionInc( aDirection ) );

	// this is odd - invalid GridNo...
	if ( TileIsOutOfBounds( covergridno ) )
		return FALSE;

	// people don't count
	if ( WhoIsThere2( covergridno, this->pathing.bLevel ) != NOBODY )
		return FALSE;

	// if we can sit there, then it's obviously not dense enough to for us to cover againt it
	if ( IsLocationSittable( covergridno, this->pathing.bLevel ) )
		return FALSE;

	INT8 adjacenttileheight = GetTallestStructureHeight( covergridno, this->pathing.bLevel );

	if ( adjacenttileheight >= 2 )
		return TRUE;

	return FALSE;
}


// Flugente: fortification
FLOAT	SOLDIERTYPE::GetConstructionPoints( )
{
	if ( this->stats.bLife < OKLIFE || this->flags.fMercAsleep || this->bCollapsed || (this->usSoldierFlagMask & SOLDIER_POW) )
		return 0;

	UINT32 val = EffectiveStrength( this, FALSE );

	ReducePointsForFatigue( this, &val );
	
	FLOAT dval = val * (100 + this->GetBackgroundValue( BG_FORTIFY_ASSIGNMENT )) / 100.0f;

	dval = (dval * this->stats.bLife / this->stats.bLifeMax);

	dval *= GetAdministrationModifier();

	return dval;
}

BOOLEAN SOLDIERTYPE::HasItem(UINT16 usItem)
{
	for ( size_t bLoop = 0, invsize = inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) == true && inv[bLoop].usItem == usItem )
			return TRUE;
	}

	return FALSE;
}

// AI-only: heal self. Do NOT, repeat, NOT use this with mercs!
BOOLEAN		SOLDIERTYPE::SelfDetonate( )
{
	if ( !(this->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL) )
		return FALSE;

	for ( size_t bLoop = 0, invsize = inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) == true && inv[bLoop].usItem == this->aiData.usActionData )
		{
			IgniteExplosion( this->ubID, this->sX, this->sY, (INT16)(gpWorldLevelData[this->sGridNo].sHeight), this->sGridNo, inv[bLoop].usItem, this->pathing.bLevel, this->ubDirection );
		
			// Remove item!
			DeleteObj( &(this->inv[bLoop]) );

			return TRUE;
		}
	}

	return FALSE;
}

// Flugente: chance to defeat a water snake instead of being hit by it
UINT8	SOLDIERTYPE::GetWaterSnakeDefenseChance()
{
	// base evasion chance is 5%
	INT16 val = 5;

	if ( gGameOptions.fNewTraitSystem )
		val += gSkillTraitValues.usSVSnakeDefense * NUM_SKILL_TRAITS( this, SURVIVAL_NT );

	val += this->GetBackgroundValue( BG_SNAKEDEFENSE );

	// bonus if we have a knife, extra if it is in our hands
	for ( size_t bLoop = 0, invsize = inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists( ) )
		{
			OBJECTTYPE* pObj = &(inv[bLoop]);

			if ( pObj && (*pObj)[0]->data.objectStatus >= USABLE && Item[pObj->usItem].usItemClass == IC_BLADE )
			{
				if ( bLoop == HANDPOS || bLoop == SECONDHANDPOS )
					val += 25;
				else
					val += 15;

				break;
			}
		}
	}

	// chance is lowered if we are in deep water
	if ( TERRAIN_IS_DEEP_WATER( this->bOverTerrainType ) )
		val = max(0, val - 10);

	return (UINT8)(val);
}

// Flugente: interactive actions
UINT16	SOLDIERTYPE::GetInteractiveActionSkill( INT32 sGridNo, UINT8 usLevel, UINT16 usType )
{
	switch ( usType )
	{
		case INTERACTIVE_STRUCTURE_HACKABLE:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			UINT16 skill = this->GetBackgroundValue( BG_HACKERSKILL );

			// without the background property, we cannot hack at all
			if ( !skill )
				return 0;

			FLOAT bestmodifier = 1.0f;
			
			OBJECTTYPE* pObj = NULL;

			for ( size_t bLoop = 0, invsize = inv.size(); bLoop < invsize; ++bLoop )
			{
				if ( inv[bLoop].exists( ) == true && Item[inv[bLoop].usItem].usHackingModifier )
				{
					OBJECTTYPE * pObj = &(this->inv[bLoop]);							// ... get pointer for this item ...

					if ( pObj != NULL )													// ... if pointer is not obviously useless ...
					{
						for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
						{
							FLOAT modifier = 1.0f + (Item[inv[bLoop].usItem].usHackingModifier * (*pObj)[i]->data.objectStatus) / 10000.0f;

							if ( modifier > bestmodifier )
								bestmodifier = modifier;
						}
					}
				}
			}
			
			return (UINT16)(skill * bestmodifier);
		}
		break;

		case INTERACTIVE_STRUCTURE_READFILE:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			// reading is governed by wisdom
			return this->stats.bWisdom;
		}
		break;

		case INTERACTIVE_STRUCTURE_WATERTAP:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			// we are pros at drinking water
			return 100;
		}
		break;

		case INTERACTIVE_STRUCTURE_SODAMACHINE:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			// we are pros at buying from a vending machine
			return 100;
		}
		break;

		case INTERACTIVE_STRUCTURE_MINIGAME:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			// we are pros at playing games
			return 100;
		}
		break;

		case INTERACTIVE_STRUCTURE_VARIOUS:
		{
			if ( this->ubProfile == ROBOT || IsVehicle( this ) )
				return 0;

			// no idea what we're doing, but we're probably good at it
			return 100;
		}
		break;

		default:
			break;
	}

	return 0;
}

// Flugente: riot shields
OBJECTTYPE* SOLDIERTYPE::GetEquippedRiotShield()
{
	OBJECTTYPE* pObj = NULL;

	if ( this->inv[HANDPOS].exists( ) && Item[this->inv[HANDPOS].usItem].usRiotShieldStrength > 0 )
		pObj = &(this->inv[HANDPOS]);

	if ( this->inv[SECONDHANDPOS].exists( ) && Item[this->inv[SECONDHANDPOS].usItem].usRiotShieldStrength > 0 )
		pObj = &(this->inv[SECONDHANDPOS]);

	return pObj;
}


BOOLEAN	SOLDIERTYPE::IsRiotShieldEquipped()
{
	// shield is not erect if prone
	if ( gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE )
		return FALSE;

	// no shield while swimming
	if ( TERRAIN_IS_HIGH_WATER(this->sGridNo) )
		return FALSE;

	return (GetEquippedRiotShield() != NULL);
}

void	SOLDIERTYPE::DestroyEquippedRiotShield( )
{
	// create graphic (destroyed shield item?)
	OBJECTTYPE* pObj = GetEquippedRiotShield( );

	if ( pObj )
	{
		if ( Item[pObj->usItem].usBuddyItem )
		{
			CreateItem( Item[pObj->usItem].usBuddyItem, 100, pObj );

			// Flugente: why would we keep a piece of scrap in our hands in the first place? just drop it to the ground
			AddItemToPool( this->sGridNo, pObj, 1, this->pathing.bLevel, 0, -1 );

			NotifySoldiersToLookforItems( );
		}

		// Delete object
		DeleteObj( pObj );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SHIELD_DESTROYED], this->GetName( ) );

		// dirty interface panel
		DirtyMercPanelInterface( this, DIRTYLEVEL2 );

		this->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
	}
}

void	SOLDIERTYPE::RiotShieldTakeDamage( INT32 sDamage )
{
	OBJECTTYPE* pObj = GetEquippedRiotShield();

	if ( pObj  )
	{
		PlayJA2Sample( (UINT32)(S_METAL_IMPACT1 + +Random( 3 )), RATE_11025, SoundVolume( MIDVOLUME, this->sGridNo ), 1, SoundDir( this->sGridNo ) );

		(*pObj)[0]->data.objectStatus -= sDamage;

		// if shield should have been destroyed, do so
		if ( (*pObj)[0]->data.objectStatus < 1 )
		{
			DestroyEquippedRiotShield( );
		}
	}
}

// Flugente: drag people
BOOLEAN		SOLDIERTYPE::CanDragInPrinciple(BOOLEAN fCheckStance)
{
	// only allow while crouched
	if (fCheckStance && gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH)
		return FALSE;

	// not in water
	if ( TERRAIN_IS_HIGH_WATER( this->sGridNo ) )
		return FALSE;

	// main hand must be free
	if ( this->inv[HANDPOS].exists( ) )
		return FALSE;

	return TRUE;
}

BOOLEAN		SOLDIERTYPE::CanDragPerson(UINT16 usID, BOOLEAN fCheckStance)
{
	if (!CanDragInPrinciple(fCheckStance))
		return FALSE;
		
	// check whether this guy exists etc.
	SOLDIERTYPE* pSoldier = MercPtrs[usID];

	if ( pSoldier && pSoldier->bActive && pSoldier->bInSector )
	{
		// must be on same level
		if ( pSoldier->pathing.bLevel != this->pathing.bLevel )
			return FALSE;

		// only prone people can be dragged
		if ( gAnimControl[pSoldier->usAnimState].ubEndHeight != ANIM_PRONE )
			return FALSE;

		// not in water
		if ( TERRAIN_IS_HIGH_WATER( pSoldier->sGridNo ) )
			return FALSE;

		// don't drag nonsense around
		if ( pSoldier->ubBodyType >= COW || pSoldier->ubBodyType == QUEENMONSTER )
			return FALSE;

		// must be near us 
		if ( PythSpacesAway( pSoldier->sGridNo, this->sGridNo ) > 1 )
			return FALSE;

		// we must be able to see the other guy even if if both would be prone. This is to stop the player from dragging someone through solid structures
		//if ( !LocationToLocationLineOfSightTest(pSoldier->sGridNo, pSoldier->pathing.bLevel, this->sGridNo, this->pathing.bLevel, TRUE, CALC_FROM_ALL_DIRS, PRONE_LOS_POS, PRONE_LOS_POS))
		if (gubWorldMovementCosts[pSoldier->sGridNo][AIDirection(this->sGridNo, pSoldier->sGridNo)][this->pathing.bLevel] >= TRAVELCOST_BLOCKED)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOLEAN		SOLDIERTYPE::CanDragCorpse(UINT16 usCorpseNum, BOOLEAN fCheckStance)
{
	if (!CanDragInPrinciple(fCheckStance))
		return FALSE;

	ROTTING_CORPSE* pCorpse = GetRottingCorpse( usCorpseNum );

	if ( pCorpse )
	{
		// must be on same level
		if ( pCorpse->def.bLevel != this->pathing.bLevel )
			return FALSE;

		// don't drag nonsense around
		if ( pCorpse->def.ubBodyType >= COW || pCorpse->def.ubBodyType == QUEENMONSTER )
			return FALSE;
				
		// must be near us 
		if ( PythSpacesAway( pCorpse->def.sGridNo, this->sGridNo ) > 2 )
			return FALSE;

		// we must be able to see the other guy even if if both would be prone. This is to stop the player from dragging someone through solid structures
		//if (!LocationToLocationLineOfSightTest(pCorpse->def.sGridNo, this->pathing.bLevel, this->sGridNo, this->pathing.bLevel, TRUE, CALC_FROM_ALL_DIRS, PRONE_LOS_POS, PRONE_LOS_POS))
		if (this->sGridNo != pCorpse->def.sGridNo && gubWorldMovementCosts[pCorpse->def.sGridNo][AIDirection(this->sGridNo, pCorpse->def.sGridNo)][this->pathing.bLevel] >= TRAVELCOST_BLOCKED)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOLEAN		SOLDIERTYPE::CanDragStructure(INT32 sGridNo, BOOLEAN fCheckStance)
{
	if (!CanDragInPrinciple(fCheckStance))
		return FALSE;
	
	if ( sGridNo == NOWHERE )
		return FALSE;

	// not on the same tile
	if ( sGridNo == this->sGridNo )
		return FALSE;

	// not in water
	if ( TERRAIN_IS_HIGH_WATER( sGridNo ) )
		return FALSE;
		
	// must be near us 
	if ( PythSpacesAway( sGridNo, this->sGridNo ) > 1 )
		return FALSE;

	UINT32 tiletype;
	UINT16 structurenumber;
	UINT8 hitpoints;
	UINT8 decalflag;
	if ( !IsDragStructurePresent( sGridNo, this->pathing.bLevel, tiletype, structurenumber, hitpoints, decalflag ) )
		return FALSE;

	// Now we need to check if there is not a wall between the two middle tiles
	UINT8 ubDragDirection = GetDirectionToGridNoFromGridNo( this->sGridNo, sGridNo );
	
	{
		switch ( ubDragDirection )
		{
		case NORTH:
			if ( WallOrClosedDoorExistsOfTopLeftOrientation( sGridNo ) )
				return FALSE;
			break;
		case EAST:
			if ( WallOrClosedDoorExistsOfTopRightOrientation( this->sGridNo ) )
				return FALSE;
			break;
		case SOUTH:
			if ( WallOrClosedDoorExistsOfTopLeftOrientation( this->sGridNo ) )
				return FALSE;
			break;
		case WEST:
			if ( WallOrClosedDoorExistsOfTopRightOrientation( sGridNo ) )
				return FALSE;
			break;

		case NORTHEAST:
			{
				bool successA = true;
				bool successB = true;

				// two possibilities:
				// A) check whether there is no wall to our north, and no wall from there to the east	
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( NORTH ) );

					if ( WallOrClosedDoorExistsOfTopLeftOrientation( midpointgridno )
						|| WallOrClosedDoorExistsOfTopRightOrientation( midpointgridno ) )
					{
						successA = false;
					}
				}

				// B) check whether there is no wall to our east, and no wall from there to the north
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( EAST ) );

					if ( WallOrClosedDoorExistsOfTopRightOrientation( this->sGridNo )
						|| WallOrClosedDoorExistsOfTopLeftOrientation( sGridNo ) )
					{
						successB = false;
					}
				}

				if ( !successA && !successB )
					return FALSE;
			}
			break;
		case SOUTHEAST:
			{
				bool successA = true;
				bool successB = true;

				// two possibilities:
				// A) check whether there is no wall to our south, and no wall from there to the east	
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( SOUTH ) );

					if ( WallOrClosedDoorExistsOfTopLeftOrientation( this->sGridNo )
						|| WallOrClosedDoorExistsOfTopRightOrientation( midpointgridno ) )
					{
						successA = false;
					}
				}

				// B) check whether there is no wall to our east, and no wall from there to the south
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( EAST ) );

					if ( WallOrClosedDoorExistsOfTopRightOrientation( this->sGridNo )
						|| WallOrClosedDoorExistsOfTopLeftOrientation( midpointgridno ) )
					{
						successB = false;
					}
				}

				if ( !successA && !successB )
					return FALSE;
			}
			break;
		case SOUTHWEST:
			{
				bool successA = true;
				bool successB = true;
			
				// two possibilities:
				// A) check whether there is no wall to our south, and no wall from there to the west	
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( SOUTH ) );

					if ( WallOrClosedDoorExistsOfTopLeftOrientation( this->sGridNo )
						|| WallOrClosedDoorExistsOfTopRightOrientation( sGridNo ) )
					{
						successA = false;
					}
				}

				// B) check whether there is no wall to our west, and no wall from there to the south
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( WEST ) );

					if ( WallOrClosedDoorExistsOfTopRightOrientation( midpointgridno )
						|| WallOrClosedDoorExistsOfTopLeftOrientation( midpointgridno ) )
					{
						successB = false;
					}
				}

				if ( !successA && !successB )
					return FALSE;
			}
			break;
		case NORTHWEST:
			{
				bool successA = true;
				bool successB = true;

				// two possibilities:
				// A) check whether there is no wall to our north, and no wall from there to the west	
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( NORTH ) );

					if ( WallOrClosedDoorExistsOfTopLeftOrientation( midpointgridno )
						|| WallOrClosedDoorExistsOfTopRightOrientation( sGridNo ) )
					{
						successA = false;
					}
				}

				// B) check whether there is no wall to our west, and no wall from there to the north
				{
					INT32 midpointgridno = NewGridNo( this->sGridNo, DirectionInc( WEST ) );

					if ( WallOrClosedDoorExistsOfTopRightOrientation( midpointgridno )
						|| WallOrClosedDoorExistsOfTopLeftOrientation( sGridNo ) )
					{
						successB = false;
					}
				}

				if ( !successA && !successB )
					return FALSE;
			}
			break;
		default:
			return FALSE;
			break;
		}
	}
	
	return TRUE;
}

BOOLEAN		SOLDIERTYPE::IsDragging( bool aStopIfConditionNotSatisfied )
{
	if (this->sDragCorpseID >= 0)
	{
		if (this->CanDragCorpse(this->sDragCorpseID, TRUE))
			return TRUE;
		else if (aStopIfConditionNotSatisfied)
			CancelDrag();
	}
	else if (this->usDragPersonID != NOBODY)
	{
		if (this->CanDragPerson(this->usDragPersonID, TRUE))
			return TRUE;
		else if (aStopIfConditionNotSatisfied)
			CancelDrag();
	}
	else if (this->sDragGridNo != NOWHERE)
	{
		if (this->CanDragStructure(this->sDragGridNo, TRUE))
			return TRUE;
		else if (aStopIfConditionNotSatisfied)
			CancelDrag();
	}

	return FALSE;
}

void	SOLDIERTYPE::SetDragOrderPerson( UINT16 usID )
{
	if ( CanDragPerson( usID ) )
	{
		// sevenfm: if someone is dragging this soldier, cancel drag
		SOLDIERTYPE *pSoldier;
		for (UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop)
		{
			pSoldier = MercPtrs[uiLoop];
			if (pSoldier && pSoldier->usDragPersonID == usID)
			{
				pSoldier->CancelDrag();
			}
		}

		CancelDrag();

		this->usDragPersonID = usID;
	}
}

void	SOLDIERTYPE::SetDragOrderCorpse( UINT32 usID )
{
	if ( CanDragCorpse( usID ) )
	{
		// sevenfm: if someone is dragging this corpse, cancel drag
		SOLDIERTYPE *pSoldier;
		for (UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
		{
			pSoldier = MercPtrs[uiLoop];
			if (pSoldier && pSoldier->sDragCorpseID == usID)
			{
				pSoldier->CancelDrag();
			}
		}

		CancelDrag();

		this->sDragCorpseID = usID;
	}
}

void	SOLDIERTYPE::SetDragOrderStructure( INT32 sGridNo )
{
	if ( CanDragStructure( sGridNo ) )
	{
		// sevenfm: if someone is dragging this corpse, cancel drag
		SOLDIERTYPE *pSoldier;
		for ( UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; ++uiLoop )
		{
			pSoldier = MercPtrs[uiLoop];
			if ( pSoldier && pSoldier->sDragGridNo == sGridNo )
			{
				pSoldier->CancelDrag();
			}
		}

		CancelDrag();

		this->sDragGridNo = sGridNo;
	}
}

void	SOLDIERTYPE::CancelDrag()
{
	// sevenfm: update face icon
	if (this->usDragPersonID != NOBODY || this->sDragCorpseID != -1 || this->sDragGridNo != NOWHERE)
	{
		fInterfacePanelDirty = DIRTYLEVEL2;
	}

	// if we are dragging a person, set them to the center of their gridno, otherwise their position might be off
	if (this->usDragPersonID != NOBODY)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[this->usDragPersonID];

		if ( pSoldier && !TileIsOutOfBounds(pSoldier->sGridNo) )
		{
			INT16 base_x = 0;
			INT16 base_y = 0;
			ConvertGridNoToCenterCellXY(pSoldier->sGridNo, &base_x, &base_y);

			pSoldier->EVENT_InternalSetSoldierPosition(base_x, base_y, FALSE, FALSE, FALSE);
		}
	}

	this->usDragPersonID = NOBODY;
	this->sDragCorpseID = -1;
	this->sDragGridNo = NOWHERE;
}

// Flugente: spy assignments
extern UINT32 gCoolnessBySector[256];

UINT8		SOLDIERTYPE::GetUncoverRisk()
{
	if ( this->stats.bLife < OKLIFE || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0;

	if ( !SPY_LOCATION(this->bAssignment) )
		return 100;

	// base value:
	// 15% level
	// 15% stealth
	// 70% covert trait
	UINT32 val = 15 * EffectiveExpLevel ( this, FALSE )
		+ 1.5f * GetWornStealth( this )
		+ 350 * NUM_SKILL_TRAITS( this, COVERT_NT );

	ReducePointsForFatigue( this, &val );

	// personality/disability modifiers
	FLOAT modifier = 1.0f;
	if ( DoesMercHaveDisability( this, NERVOUS ) )					modifier -= 0.05f;

	if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		modifier += 0.05f;
	if ( DoesMercHavePersonality( this, CHAR_TRAIT_COWARD ) )		modifier -= 0.05f;

	// personal value in [0; 100]
	int personalvalue = (FLOAT)(val * modifier) / 10.0f;
	personalvalue = min( 100, max( 0, personalvalue ) );
	
	// if we do this disguised as a soldier, risk will be much higer, as we are under much more scrutiny. This makes up for the increased gain in soldier disguise
	// less risk if we are asleep, just hiding or forced to hide
	UINT8 typemultiplier = ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER ) ? 5 : 2;
	if ( ( this->bAssignment == CONCEALED ) || this->flags.fMercAsleep || this->usSkillCooldown[SOLDIER_COOLDOWN_INTEL_PENALTY] > 10 )
		typemultiplier = 1;
		
	// we now take the sector coolness as a measurement of how important the sector is, and thus how intel we gain
	// correct outliers - value in[0; 100]
	UINT32 sectorvalue = typemultiplier * min( 20, gCoolnessBySector[SECTOR( this->sSectorX, this->sSectorY )] );
	
	UINT8 totalvalue = sectorvalue * ( 110 - personalvalue ) / 100;
	totalvalue = min(100, max(0, totalvalue ) );

	// A most awesome merc in Meduna palace, disguised as a soldier, would have a value of 1.05 * 4. 63 * 4 = 10.649 at this point.
	// This would be the place where we modify our intel gain rate.

	return totalvalue;
}

FLOAT		SOLDIERTYPE::GetIntelGain()
{
	if ( this->stats.bLife < OKLIFE || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0.0f;

	// if not on correct assignments, no gain
	if ( this->bAssignment != GATHERINTEL )
		return 0.0f;

	// if we're asleep, or on a penalty, we accomplish nothing
	if ( this->flags.fMercAsleep || this->usSkillCooldown[SOLDIER_COOLDOWN_INTEL_PENALTY] > 10 )
		return 0.0f;

	// the covert trait isn't that important in determining the intel gain. It is much more important in mitigating the risk of exposure, however
	// base value:
	// 50% wisdom
	// 10% level
	// 5% scout trait
	// 15% covert trait
	// 20% snitch trait
	UINT32 val = 5 * EffectiveWisdom( this )
		+ 10 * EffectiveExpLevel ( this, FALSE ) 
		+ 50 * NUM_SKILL_TRAITS( this, SCOUTING_NT ) 
		+ 75 * NUM_SKILL_TRAITS( this, COVERT_NT ) 
		+ 200 * NUM_SKILL_TRAITS( this, SNITCH_NT );

	ReducePointsForFatigue( this, &val );

	// personality/disability modifiers
	FLOAT modifier = 1.0f;
	if ( DoesMercHaveDisability( this, FORGETFUL ) )	modifier -= 0.15f;
	if ( DoesMercHaveDisability( this, PSYCHO ) )		modifier -= 0.05f;
	
	if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		modifier += 0.10f;
	if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )		modifier -= 0.10f;
	if ( DoesMercHavePersonality( this, CHAR_TRAIT_ASSERTIVE ) )	modifier += 0.05f;
	if ( DoesMercHavePersonality( this, CHAR_TRAIT_PRIMITIVE ) )	modifier -= 0.10f;
		
	FLOAT personalvalue = (FLOAT)(val * modifier) / 1000.0f;

	// we now take the sector coolness as a measurement of how important the sector is, and thus how intel we gain
	// correct outliers
	UINT32 ubLocationModifier = 1 + max(2, min(20, gCoolnessBySector[SECTOR( this->sSectorX, this->sSectorY )] ) );

	// in order not to make the differences to great, alter these values - will now be in [0.6; 4.63]
	FLOAT sectorvalue = log( (FLOAT)ubLocationModifier );
	sectorvalue *= sectorvalue / 2.0f;

	FLOAT administrationmodifier = GetAdministrationModifier();

	FLOAT totalvalue = personalvalue * sectorvalue * administrationmodifier;

	// if we do this disguised as a soldier, we get more info
	if ( this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER )
		totalvalue *= 2;

	// A most awesome merc in Meduna palace, disguised as a soldier, would have a value of 1.15 * 4.63 * 2 = 10.649 at this point.
	// This would be the place where we modify our intel gain rate.

	return totalvalue;
}

void		SOLDIERTYPE::StopChatting()
{
	if ( this->usChatPartnerID != NOBODY )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[DISTRACT_STOP_STR], this->GetName(), MercPtrs[this->usChatPartnerID]->GetName() );

		MercPtrs[this->usChatPartnerID]->usChatPartnerID = NOBODY;
		this->usChatPartnerID = NOBODY;
	}
}

void		SOLDIERTYPE::DrugAutoUse()
{
	if ( !this->HasBackgroundFlag( BACKGROUND_DRUGUSE ) )
		return;
	
	if ( !( gTacticalStatus.uiFlags & (INCOMBAT| TURNBASED) ) )
		return;

	if ( this->usSkillCooldown[SOLDIER_COOLDOWN_DRUGUSER_COMBAT] )
		return;

	INT8 invsize = (INT8)inv.size();									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists() && Item[inv[bLoop].usItem].drugtype )
		{
			// use portionsize, if none was entered, use full item
			UINT8 portionsize = Item[inv[bLoop].usItem].usPortionSize;
			if ( !portionsize )
				portionsize = 100;

			// how much of this item do we use up
			UINT8 usable = min( portionsize, ( this->inv[bLoop] )[0]->data.objectStatus );
			if ( !usable )
				continue;

			std::vector<DRUG_EFFECT> vec_drug = NewDrug[Item[inv[bLoop].usItem].drugtype].drug_effects;

			std::vector<DRUG_EFFECT>::iterator drug_effects_itend = vec_drug.end();
			for ( std::vector<DRUG_EFFECT>::iterator drug_effects_it = vec_drug.begin(); drug_effects_it != drug_effects_itend; ++drug_effects_it )
			{
				if ( ( *drug_effects_it ).size > 0 )
				{
					if ( ( *drug_effects_it ).effect == DRUG_EFFECT_HP && this->bBleeding > 1 && ( *drug_effects_it ).size * ( *drug_effects_it ).duration * usable / 100 < this->bBleeding * 2 )
					{
						ApplyConsumable( this, &( this->inv[bLoop] ), TRUE, FALSE );

						this->usSkillCooldown[SOLDIER_COOLDOWN_DRUGUSER_COMBAT] += 6;

						return;
					}
					else if ( ( *drug_effects_it ).effect == DRUG_EFFECT_BP && this->bBreath < 50 )
					{
						ApplyConsumable( this, &( this->inv[bLoop] ), TRUE, FALSE );

						this->usSkillCooldown[SOLDIER_COOLDOWN_DRUGUSER_COMBAT] += 6;

						return;
					}
				}
			}
		}
	}
}

OBJECTTYPE*		SOLDIERTYPE::GetObjectWithItemFlag( UINT64 aFlag )
{
	for ( INT8 bLoop = 0, invsize = (INT8)inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists() )
		{
			OBJECTTYPE* pObj = &( inv[bLoop] );

			if ( pObj && HasItemFlag( pObj->usItem, aFlag ) )
			{
				return pObj;
			}
		}
	}

	return NULL;
}

bool		SOLDIERTYPE::DestroyOneObjectWithItemFlag( UINT64 aFlag )
{
	for ( INT8 bLoop = 0, invsize = (INT8)inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists() )
		{
			OBJECTTYPE* pObj = &( inv[bLoop] );

			if ( pObj && HasItemFlag( pObj->usItem, aFlag ) )
			{
				pObj->RemoveObjectsFromStack( 1 );

				if ( pObj->ubNumberOfObjects <= 0 )
				{
					DeleteObj( pObj );
				}

				return true;
			}
		}
	}

	return false;
}

bool		SOLDIERTYPE::DestroyOneItemInInventory( UINT16 ausItem )
{
	for ( INT8 bLoop = 0, invsize = (INT8)inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists() )
		{
			OBJECTTYPE* pObj = &( inv[bLoop] );

			if ( pObj && pObj->usItem == ausItem )
			{
				pObj->RemoveObjectsFromStack( 1 );

				if ( pObj->ubNumberOfObjects <= 0 )
				{
					DeleteObj( pObj );
				}

				return true;
			}
		}
	}

	return false;
}

bool		SOLDIERTYPE::HasItemInInventory( UINT16 ausItem )
{
	for ( INT8 bLoop = 0, invsize = (INT8)inv.size(); bLoop < invsize; ++bLoop )
	{
		if ( inv[bLoop].exists() )
		{
			OBJECTTYPE* pObj = &( inv[bLoop] );

			if ( pObj && pObj->usItem == ausItem )
			{
				return true;
			}
		}
	}

	return false;
}

#define BLOODDONATION_AMOUNT	10

// Flugente: can we fill a blood bag from this guy?
BOOLEAN		SOLDIERTYPE::IsValidBloodDonor()
{
	// not during combat
	if ( gTacticalStatus.uiFlags & INCOMBAT )
		return FALSE;

	//  must be player team
	if ( this->bTeam != gbPlayerNum )
		return FALSE;

	// mustn't be mechanical unit
	if (AM_A_ROBOT(this) || this->flags.uiStatusFlags & SOLDIER_VEHICLE)
		return FALSE;

	// not if wounded
	if ( this->stats.bLife < this->stats.bLifeMax )
		return FALSE;

	// not if doing so would drop us into coma
	if ( this->stats.bLife - BLOODDONATION_AMOUNT < OKLIFE )
		return FALSE;

	// not if we have any KNOWN disease
	if ( this->HasDisease( TRUE, FALSE ) )
		return FALSE;

	// not if we're drunk or drugged
	if ( MercDruggedOrDrunk( this ) )
		return FALSE;

	// need to be properly fed and watered
	if ( UsingFoodSystem() )
	{
		UINT8 foodsituation;
		UINT8 watersituation;
		GetFoodSituation( this, &foodsituation, &watersituation );
		if ( foodsituation > FOOD_NORMAL || watersituation > FOOD_NORMAL )
			return FALSE;
	}

	return TRUE;
}

UINT32		SOLDIERTYPE::GetAdministrationPoints()
{
	if ( this->stats.bLife < OKLIFE || this->bSectorZ || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0;
	
	// if not on correct assignment, no gain
	if ( this->bAssignment != ADMINISTRATION )
		return 0;

	UINT32 val = 250 + 4 * EffectiveWisdom( this ) + 3 * EffectiveLeadership( this ) + 5 * EffectiveExpLevel( this, FALSE );
	
	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	if ( DoesMercHaveDisability( this, NERVOUS ) )		persmodifier -= 0.01f;
	if ( DoesMercHaveDisability( this, FORGETFUL ) )	persmodifier -= 0.60f;
	if ( DoesMercHaveDisability( this, PSYCHO ) )		persmodifier -= 0.03f;
	if ( DoesMercHaveDisability( this, DEAF ) )			persmodifier -= 0.15f;
	if ( DoesMercHaveDisability( this, SHORTSIGHTED ) )	persmodifier -= 0.10f;
	
	if ( gGameOptions.fNewTraitSystem )
	{
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		persmodifier += 0.10f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )		persmodifier -= 0.10f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_OPTIMIST ) )		persmodifier += 0.02f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_ASSERTIVE ) )	persmodifier += 0.08f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_INTELLECTUAL ) )	persmodifier += 0.15f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_PRIMITIVE ) )	persmodifier -= 0.15f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_AGGRESSIVE ) )	persmodifier -= 0.04f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_PHLEGMATIC ) )	persmodifier -= 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) )		persmodifier -= 0.03f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_COWARD ) )		persmodifier -= 0.07f;
	}

	// background modifier
	persmodifier += ( this->GetBackgroundValue( BG_ADMINISTRATION_ASSIGNMENT ) ) / 100.0f;
	
	// equipment modifier
	FLOAT bestequipmentmodifier = 1.0f;

	OBJECTTYPE* pObj = NULL;

	INT8 invsize = (INT8)inv.size();									// remember inventorysize, so we don't call size() repeatedly

	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )						// ... for all items in our inventory ...
	{
		if ( inv[bLoop].exists() == true && Item[inv[bLoop].usItem].usAdministrationModifier )
		{
			OBJECTTYPE * pObj = &( this->inv[bLoop] );							// ... get pointer for this item ...

			if ( pObj != NULL )													// ... if pointer is not obviously useless ...
			{
				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					FLOAT modifier = 1.0f + ( Item[inv[bLoop].usItem].usAdministrationModifier * ( *pObj )[i]->data.objectStatus ) / 10000.0f;

					if ( modifier > bestequipmentmodifier )
						bestequipmentmodifier = modifier;
				}
			}
		}
	}

	// the best friendly/direct/recruit approach factor can alter the value up to 10%
	FLOAT approachmax = max( gMercProfiles[this->ubProfile].usApproachFactor[0], max( gMercProfiles[this->ubProfile].usApproachFactor[1], gMercProfiles[this->ubProfile].usApproachFactor[2] ) );
	FLOAT approachmodifier = 1.0f + max( -0.1f, min( 0.1f, (approachmax - 100.0f) / 100.0f ));

	UINT32 totalvalue = val * persmodifier * bestequipmentmodifier * approachmodifier / gGameExternalOptions.fAdministrationPointsPerPercent;
	
	ReducePointsForFatigue( this, &totalvalue );

	return totalvalue;
}

extern FLOAT GetAdministrationPercentage( INT16 sX, INT16 sY );

FLOAT		SOLDIERTYPE::GetAdministrationModifier()
{
	if ( ADMINISTRATION_BONUS( this->bAssignment ) )
		return 1.0f + GetAdministrationPercentage( this->sSectorX, this->sSectorY ) / 100.0f + RebelCommand::GetAssignmentBonus(this->sSectorX, this->sSectorY);

	return 1.0f;
}

// Flugente:  those with the <scrounging> background occasionally steal money from the locals
UINT8		SOLDIERTYPE::GetThiefStealMoneyChance()
{
	if ( this->stats.bLife < OKLIFE || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0;
	
	UINT32 val = 1 * EffectiveAgility( this, FALSE ) + 8 * EffectiveDexterity( this, FALSE ) + 10 * EffectiveExpLevel( this, FALSE );
	
	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	//if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) )		persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, NERVOUS ) )				persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, CLAUSTROPHOBIC ) )		persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, NONSWIMMER ) )			persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) )		persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, FORGETFUL ) )			persmodifier -= 0.12f;
	//if ( DoesMercHaveDisability( this, PSYCHO ) )				persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, DEAF ) )					persmodifier -= 0.15f;
	if ( DoesMercHaveDisability( this, SHORTSIGHTED ) )			persmodifier -= 0.30f;
	//if ( DoesMercHaveDisability( this, HEMOPHILIAC ) )			persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, AFRAID_OF_HEIGHTS ) )	persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, SELF_HARM ) )			persmodifier -= 0.20f;
		
	if ( gGameOptions.fNewTraitSystem )
	{
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		persmodifier += 0.25f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )		persmodifier -= 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_OPTIMIST ) )		persmodifier += 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_ASSERTIVE ) )	persmodifier += 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_INTELLECTUAL ) )	persmodifier += 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_PRIMITIVE ) )	persmodifier -= 0.15f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_AGGRESSIVE ) )	persmodifier -= 0.15f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_PHLEGMATIC ) )	persmodifier -= 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_DAUNTLESS ) )	persmodifier -= 0.13f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_PACIFIST ) )		persmodifier -= 0.03f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_MALICIOUS ) )	persmodifier -= 0.13f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) )		persmodifier -= 0.08f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_COWARD ) )		persmodifier -= 0.25f;
	}
	
	UINT32 totalvalue = val * persmodifier / 10.0f;

	ReducePointsForFatigue( this, &totalvalue );

	totalvalue = min( 100, max( 0, totalvalue ) );

	return totalvalue;
}

UINT8		SOLDIERTYPE::GetThiefEvadeDetectionChance()
{
	if ( this->stats.bLife < OKLIFE )
		return 0;
	
	// the theoretical unboosted maximum is 1100, yet we treat it like 1000 - effectively you can boost stealth gear to give you a serious edge
	UINT32 val = 250 + 5 * EffectiveExpLevel( this, FALSE ) + 5 * EffectiveAgility( this, FALSE ) + 3 * GetWornStealth( this );

	ReducePointsForFatigue( this, &val );

	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	//if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) )		persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, NERVOUS ) )				persmodifier -= 0.04f;
	//if ( DoesMercHaveDisability( this, CLAUSTROPHOBIC ) )		persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, NONSWIMMER ) )			persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) )		persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, FORGETFUL ) )			persmodifier -= 0.50f;
	//if ( DoesMercHaveDisability( this, PSYCHO ) )				persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, DEAF ) )					persmodifier -= 0.06f;
	//if ( DoesMercHaveDisability( this, SHORTSIGHTED ) )			persmodifier -= 0.40f;
	//if ( DoesMercHaveDisability( this, HEMOPHILIAC ) )			persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, AFRAID_OF_HEIGHTS ) )	persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, SELF_HARM ) )			persmodifier -= 0.20f;
	
	UINT32 totalvalue = val * persmodifier / 10.0f;

	ReducePointsForFatigue( this, &totalvalue );

	totalvalue = min( 100, max( 0, totalvalue ) );

	return totalvalue;
}

BOOLEAN	SOLDIERTYPE::InPositionForTurncoatAttempt( UINT16 usID )
{
	if ( !gSkillTraitValues.fCOTurncoats
		|| gbWorldSectorZ
		|| gTacticalStatus.Team[ENEMY_TEAM].bAwareOfOpposition )
		return FALSE;

	if ( this->stats.bLife < OKLIFE
		|| this->flags.fMercAsleep
		|| this->bCollapsed
		|| ( this->usSoldierFlagMask & SOLDIER_POW )
		|| this->usSkillCooldown[SOLDIER_COOLDOWN_INTEL_PENALTY] > 20
		|| usID == NOBODY )
		return FALSE;

	SOLDIERTYPE* pSoldier = MercPtrs[usID];

	if ( !pSoldier
		|| pSoldier->bTeam != ENEMY_TEAM
		|| pSoldier->ubProfile != NO_PROFILE
		|| pSoldier->stats.bLife != pSoldier->stats.bLifeMax
		|| pSoldier->bCollapsed
		|| ( pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT )
		|| !SOLDIER_CLASS_ENEMY( pSoldier->ubSoldierClass )
		|| !SeemsLegit( pSoldier->ubID ) )
		return FALSE;

	// additional checks if we want to know wether we can target a specific location
	if ( PythSpacesAway( this->sGridNo, pSoldier->sGridNo ) < 10 )
	{
		INT32 val = SoldierToVirtualSoldierLineOfSightTest( this, pSoldier->sGridNo, this->pathing.bLevel, gAnimControl[this->usAnimState].ubEndHeight, FALSE, 10 );

		// error if we cannot see the target
		if ( !val )
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

UINT8		SOLDIERTYPE::GetTurncoatConvinctionChance( UINT16 usID, INT16 sApproach )
{
	if ( usID >= NOBODY )
		return 0;

	SOLDIERTYPE* pSoldier = MercPtrs[usID];

	if ( !pSoldier
		|| pSoldier->bTeam != ENEMY_TEAM )
		return 0;

	// enemy robots can't be turncoats
	if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ROBOT)
		return 0;

	if ( this->stats.bLife < OKLIFE )
		return 0;

	// determine effectiveness of merc	
	// nominally in [0; 1000]
	INT32 basestatrating = 6 * EffectiveLeadership( this ) + 40 * EffectiveExpLevel( this, FALSE );

	FLOAT recruitmodifier = ( 100 + this->GetBackgroundValue( BG_PERC_APPROACH_RECRUIT ) ) / 100.0f;

	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	if ( DoesMercHaveDisability( this, NERVOUS ) )				persmodifier -= 0.10f;

	if ( gGameOptions.fNewTraitSystem )
	{
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		persmodifier += 0.08f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )		persmodifier -= 0.04f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_ASSERTIVE ) )	persmodifier += 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_AGGRESSIVE ) )	persmodifier -= 0.05f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_PHLEGMATIC ) )	persmodifier -= 0.02f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_DAUNTLESS ) )	persmodifier += 0.03f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) )		persmodifier += 0.04f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_COWARD ) )		persmodifier -= 0.07f;
	}

	// nominally in [0; 100]
	INT32 recruitrating = basestatrating * recruitmodifier * persmodifier * gMercProfiles[this->ubProfile].usApproachFactor[3] / 1000;

	// optional ini bonus
	recruitrating += gSkillTraitValues.sCOTurncoats_PlayerConvinctionBonus;

	ReducePointsForFatigue( this, &recruitrating );

	// determine resistance of soldier to our subversion
	INT32 ubLocationModifier = 2 * max( 1, min( 20, gCoolnessBySector[SECTOR( this->sSectorX, this->sSectorY )] ) );

	// enemy resistance is dependent on their level, class and the sector rating
	INT32 enemyresistancerating = ubLocationModifier + 8 * EffectiveExpLevel( pSoldier, FALSE );

	switch ( pSoldier->ubSoldierClass )
	{
	case SOLDIER_CLASS_ADMINISTRATOR:	enemyresistancerating -= 30;	break;
	case SOLDIER_CLASS_ELITE:			enemyresistancerating += 30;	break;
	default:	break;
	}

	if ( pSoldier->usSoldierFlagMask & SOLDIER_ENEMY_OFFICER )
		enemyresistancerating += 30;

	ReducePointsForFatigue( pSoldier, &enemyresistancerating );
	
	switch ( sApproach )
	{
		// base approach
	case 1:
		break;

		// we use our looks for seduction
		// thus looking attractive lowers enemy resistance, while being ugly can increase it
		// however this fails if the soldier is not attracted to us
	case 2:
	{
		// determine whether the soldier is attracted to us in the first place (don't display this, otherwise people will want to set sexual orientation and whatnot)
		INT32 stat_dependant_roll = ( 37 * EffectiveStrength( pSoldier, FALSE ) + 92 * EffectiveMedical( pSoldier ) + 51 * EffectiveDexterity( pSoldier, FALSE ) + 61 * pSoldier->stats.bLife ) % 100;
		bool samesexattraction = ( stat_dependant_roll < 8 );

		bool female_player = ( this->ubBodyType == REGFEMALE );
		bool female_soldier = ( pSoldier->ubBodyType == REGFEMALE );

		bool fittingattraction = false;
		if ( female_player != female_soldier && !samesexattraction )
			fittingattraction = true;
		else if ( female_player == female_soldier && samesexattraction )
			fittingattraction = true;

		if ( gMercProfiles[this->ubProfile].bAppearance == APPEARANCE_UGLY )			enemyresistancerating += 50 - ( fittingattraction ?  5 : 0 );
		else if ( gMercProfiles[this->ubProfile].bAppearance == APPEARANCE_HOMELY )		enemyresistancerating += 40 - ( fittingattraction ? 15 : 0 );
		else if ( gMercProfiles[this->ubProfile].bAppearance == APPEARANCE_AVERAGE )	enemyresistancerating += 30 - ( fittingattraction ? 30 : 0 );
		else if ( gMercProfiles[this->ubProfile].bAppearance == APPEARANCE_ATTRACTIVE )	enemyresistancerating += 20 - ( fittingattraction ? 45 : 0 );
		else if ( gMercProfiles[this->ubProfile].bAppearance == APPEARANCE_BABE )		enemyresistancerating += 10 - ( fittingattraction ? 60 : 0 );

		// seduction works better in civilian clothing
		if ( this->usSoldierFlagMask & SOLDIER_COVERT_CIV )
			enemyresistancerating -= 5;
	}
	break;

	// we try to bribe the soldier with money
	case 3:
	{
		// the amount of money depends on progress and unimportant in this case
		// the worse the location, the poorer the soldier, thus the more effective money is
		enemyresistancerating -= 30 + (40 - ubLocationModifier);
	}
	break;

	// we try to bribe the soldier with intel
	case 4:
	{
		// the amount of intel depends on progress and unimportant in this case
		enemyresistancerating -= 80;
	}
	break;

	default:
		break;
	}
	
	if ( enemyresistancerating > recruitrating )
		return 0;

	return max( 0, min( 100, recruitrating - enemyresistancerating ) );
}

void		SOLDIERTYPE::AttemptToCreateTurncoat( UINT16 usID )
{
	if ( usID >= NOBODY )
		return;

	SOLDIERTYPE* pSoldier = MercPtrs[usID];

	if ( !pSoldier
		|| pSoldier->bTeam != ENEMY_TEAM
		|| ( pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT ) )
		return;

	HandleTurncoatAttempt( pSoldier );
}

BOOLEAN		SOLDIERTYPE::OrderTurnCoatToSwitchSides( UINT16 usID )
{
	if ( usID >= NOBODY )
		return FALSE;

	SOLDIERTYPE* pSoldier = MercPtrs[usID];

	if (!pSoldier 
		|| pSoldier->bTeam != ENEMY_TEAM
		|| !( pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT ) )
		return FALSE;

	if ( IsFreeSlotAvailable( MILITIA_TEAM ) )
	{
		// remove turncoat property
		pSoldier->usSoldierFlagMask2 &= ~SOLDIER_TURNCOAT;
		RemoveOneTurncoat( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->ubSoldierClass, TRUE );

		MakeCivHostile( pSoldier );

		return TRUE;
	}

	return FALSE;
}

void		SOLDIERTYPE::OrderAllTurnCoatToSwitchSides()
{
	SOLDIERTYPE *pSoldier;
	INT32 cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;

	// rftr: force the player to enter turn-based combat. this function already includes a check to see if we're already in combat, so no harm calling this.
	// this also prevents a hang when activating a sector with 100% turncoats
	EnterCombatMode(OUR_TEAM);

	// run through list
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; ++cnt, ++pSoldier )
	{
		if ( pSoldier->bActive && pSoldier->bInSector )
		{
			if ( pSoldier->usSoldierFlagMask2 & SOLDIER_TURNCOAT )
			{
				if ( IsFreeSlotAvailable( MILITIA_TEAM ) )
				{
					// remove turncoat property
					pSoldier->usSoldierFlagMask2 &= ~SOLDIER_TURNCOAT;
					RemoveOneTurncoat( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->ubSoldierClass, TRUE );

					MakeCivHostile( pSoldier );
				}
				else
				{
					return;
				}
			}
		}
	}
}

UINT32		SOLDIERTYPE::GetExplorationPoints()
{
	if ( this->stats.bLife < OKLIFE || ( this->usSoldierFlagMask & SOLDIER_POW ) )
		return 0;

	// if not on correct assignment, no gain
	if ( this->bAssignment != EXPLORATION )
		return 0;

	UINT32 val = 400 + 1 * EffectiveWisdom( this ) + 1 * EffectiveAgility( this, FALSE ) + 5 * EffectiveExpLevel( this, FALSE )
		+ 150 * NUM_SKILL_TRAITS( this, SCOUTING_NT ) + 50 * NUM_SKILL_TRAITS( this, SURVIVAL_NT ) + (this->HasBackgroundFlag( BACKGROUND_SCROUNGING ) ? 150 : 0);

	// personality/disability modifiers
	FLOAT persmodifier = 1.0f;
	//if ( DoesMercHaveDisability( this, HEAT_INTOLERANT ) )		persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, NERVOUS ) )				persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, CLAUSTROPHOBIC ) )		persmodifier -= 0.03f;
	//if ( DoesMercHaveDisability( this, NONSWIMMER ) )			persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, FEAR_OF_INSECTS ) )		persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, FORGETFUL ) )			persmodifier -= 0.30f;
	//if ( DoesMercHaveDisability( this, PSYCHO ) )				persmodifier -= 0.20f;
	//if ( DoesMercHaveDisability( this, DEAF ) )					persmodifier -= 0.15f;
	if ( DoesMercHaveDisability( this, SHORTSIGHTED ) )			persmodifier -= 0.30f;
	//if ( DoesMercHaveDisability( this, HEMOPHILIAC ) )			persmodifier -= 0.20f;
	if ( DoesMercHaveDisability( this, AFRAID_OF_HEIGHTS ) )	persmodifier -= 0.02f;
	//if ( DoesMercHaveDisability( this, SELF_HARM ) )			persmodifier -= 0.20f;

	if ( gGameOptions.fNewTraitSystem )
	{
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_SOCIABLE ) )		persmodifier += 0.25f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_LONER ) )		persmodifier -= 0.05f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_OPTIMIST ) )		persmodifier += 0.05f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_ASSERTIVE ) )	persmodifier += 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_INTELLECTUAL ) )	persmodifier += 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_PRIMITIVE ) )	persmodifier -= 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_AGGRESSIVE ) )	persmodifier -= 0.15f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_PHLEGMATIC ) )	persmodifier -= 0.05f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_DAUNTLESS ) )	persmodifier -= 0.13f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_PACIFIST ) )		persmodifier -= 0.03f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_MALICIOUS ) )	persmodifier -= 0.13f;
		//if ( DoesMercHavePersonality( this, CHAR_TRAIT_SHOWOFF ) )		persmodifier -= 0.08f;
		if ( DoesMercHavePersonality( this, CHAR_TRAIT_COWARD ) )		persmodifier -= 0.02f;
	}

	// background modifier
	persmodifier += ( this->GetBackgroundValue( BG_EXPLORATION_ASSIGNMENT ) ) / 100.0f;
	
	UINT32 totalvalue = val * persmodifier * gGameExternalOptions.fExplorationPointsModifier / 10;

	ReducePointsForFatigue( this, &totalvalue );

	return totalvalue;
}

bool	SOLDIERTYPE::IsFastMovement()
{
	if ( this->flags.fUIMovementFast )
	{
		// Flugente: disease can stop us from using our legs normally
		if ( gGameExternalOptions.fDisease
			&& gGameExternalOptions.fDiseaseSevereLimitations
			&& this->HasDiseaseWithFlag( DISEASE_PROPERTY_LIMITED_USE_LEGS ) )
			this->flags.fUIMovementFast = false;
	}

	return this->flags.fUIMovementFast;
}

INT32 CheckBleeding( SOLDIERTYPE *pSoldier )
{
	INT8		bBandaged; //,savedOurTurn;
	INT32	iBlood = NOBLOOD;
	BOOLEAN bleeder = FALSE;

	if ( pSoldier->stats.bLife != 0 )
	{
		bleeder = DoesMercHaveDisability( pSoldier, HEMOPHILIAC );

		// if merc is hurt beyond the minimum required to bleed, or he's dying
		// Flugente: or if they are a hemophiliac
		if ( (pSoldier->bBleeding > MIN_BLEEDING_THRESHOLD) || pSoldier->stats.bLife < OKLIFE || bleeder )
		{
			// if he's NOT in the process of being bandaged or DOCTORed
			if ( (pSoldier->ubServiceCount == 0) && (AnyDoctorWhoCanHealThisPatient( pSoldier, HEALABLE_EVER ) == NULL) )
			{
				// may drop blood whether or not any bleeding takes place this turn
				if ( pSoldier->bTilesMoved < 1 )
				{
					iBlood = max(0, ((pSoldier->bBleeding - MIN_BLEEDING_THRESHOLD) / BLOODDIVISOR) ); // + pSoldier->dying;
					if ( iBlood > MAXBLOODQUANTITY )
					{
						iBlood = MAXBLOODQUANTITY;
					}
				}
				else
				{
					iBlood = NOBLOOD;
				}

				// Flugente: bleeders, well, bleed
				if ( bleeder )
					iBlood = min( 1, iBlood );

				// Are we in a different mode?
				if ( !(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT) )
				{
					pSoldier->dNextBleed -= (FLOAT)RT_NEXT_BLEED_MODIFIER;
				}
				else
				{
					// Do a single step descrease
					pSoldier->dNextBleed--;
				}

				// if it's time to lose some blood
				if ( pSoldier->dNextBleed <= 0 )
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
							if ( (pSoldier->stats.bLife >= CONSCIOUSNESS) && !pSoldier->flags.fDyingComment )
							{
								TacticalCharacterDialogue( pSoldier, QUOTE_SERIOUSLY_WOUNDED );

								pSoldier->flags.fDyingComment = TRUE;
							}

							// can't permit lifemax to ever bleed beneath OKLIFE, or that
							// soldier might as well be dead!
							if ( pSoldier->stats.bLifeMax >= OKLIFE )
							{
								// Flugente: reduce PERMANENT points of life only if through 'normal' bleeding, not by poisoning
								// problem is that this function applies every bleeding cycle, while loosing points through natural restoration (too much poison in body) only happens every hour.
								// so one might lose 1pt of life through poisoning at 8:00, and then lose 30 points of life PERMANTENLY in the following hour without dying
								// We bypass this by only allowing PERMANTENT lifeloss if really bleeding
								if ( pSoldier->bBleeding )
								{
									// bleeding while "dying" costs a PERMANENT point of life each time!
									pSoldier->stats.bLifeMax--;
									pSoldier->bBleeding = max( 0, pSoldier->bBleeding - 1 );
									
									if ( pSoldier->iHealableInjury >= 100 ) // added check for insta-healable injury - SANDRO
										pSoldier->iHealableInjury -= 100;
								}
							}
						}
					}

					// either way, a point of life (health) is lost because of bleeding
					if ( pSoldier->bBleeding )
					{
						// This will also update the life bar
						SoldierBleed( pSoldier, FALSE );
					}
					else
					{
						// just to update everything, like going unconscious or dying
						pSoldier->SoldierTakeDamage( ANIM_CROUCH, 0, 0, TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, TRUE );
					}


					// if he's not dying (which includes him saying the dying quote just
					// now), and he hasn't warned us that he's bleeding yet, he does so
					// Also, not if they are being bandaged....
					if ( (pSoldier->stats.bLife >= OKLIFE) && !pSoldier->flags.fDyingComment && !pSoldier->flags.fWarnedAboutBleeding && !gTacticalStatus.fAutoBandageMode && pSoldier->ubServiceCount == 0 )
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
	return(iBlood);
}


void SoldierBleed( SOLDIERTYPE *pSoldier, BOOLEAN fBandagedBleed )
{
	// OK, here make some stuff happen for bleeding
	// A banaged bleed does not show damage taken , just through existing bandages

	// ATE: Do this ONLY if buddy is in sector.....
	if ( (pSoldier->bInSector && guiCurrentScreen == GAME_SCREEN) || guiCurrentScreen != GAME_SCREEN )
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

	// If we are already dead, don't show damage!
	if ( !fBandagedBleed )
	{
		// SANDRO - if the soldier is bleeding out, consider this damage as done by the last attacker
		if ( pSoldier->ubAttackerID != NOBODY )
			pSoldier->SoldierTakeDamage( ANIM_CROUCH, 1, 100, TAKE_DAMAGE_BLOODLOSS, pSoldier->ubAttackerID, NOWHERE, 0, TRUE );
		else if ( pSoldier->ubPreviousAttackerID != NOBODY )
			pSoldier->SoldierTakeDamage( ANIM_CROUCH, 1, 100, TAKE_DAMAGE_BLOODLOSS, pSoldier->ubPreviousAttackerID, NOWHERE, 0, TRUE );
		else if ( pSoldier->ubNextToPreviousAttackerID != NOBODY )
			pSoldier->SoldierTakeDamage( ANIM_CROUCH, 1, 100, TAKE_DAMAGE_BLOODLOSS, pSoldier->ubNextToPreviousAttackerID, NOWHERE, 0, TRUE );
		else
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
	switch ( pSoldier->ubBodyType )
	{
	case ADULTFEMALEMONSTER:
	case AM_MONSTER:
	case YAF_MONSTER:
	case YAM_MONSTER:
	case LARVAE_MONSTER:
	case INFANT_MONSTER:
	case QUEENMONSTER:

		// Give breath back....
		DeductPoints( pSoldier, 0, (INT16)-5000 );
		return;
		break;
	}

	pSoldier->bCollapsed = TRUE;

	pSoldier->usUIMovementMode = CRAWLING;

	pSoldier->ReceivingSoldierCancelServices( );

	// CC has requested - handle sight here...
	HandleSight( pSoldier, SIGHT_LOOK );

	// Check height
	switch ( gAnimControl[pSoldier->usAnimState].ubEndHeight )
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
			if ( !is_networked )pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
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
		if ( !is_networked )pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_CROUCH, 0, FALSE );
		else pSoldier->ChangeSoldierState( FALLFORWARD_FROMHIT_CROUCH, 0, FALSE );
		//}
		//else
		//{
		//	// For civs... use fall from stand...
		//	pSoldier->EVENT_InitNewSoldierAnim( FALLFORWARD_FROMHIT_STAND, 0 , FALSE);
		//}
		break;

	case ANIM_PRONE:

		switch ( pSoldier->usAnimState )
		{
		case FALLFORWARD_FROMHIT_STAND:
		case ENDFALLFORWARD_FROMHIT_CROUCH:

			pSoldier->ChangeSoldierState( STAND_FALLFORWARD_STOP, 0, FALSE );
			break;

		case FALLBACK_HIT_STAND:
			pSoldier->ChangeSoldierState( FALLBACKHIT_STOP, 0, FALSE );
			break;

		default:
			if ( !is_networked )pSoldier->EVENT_InitNewSoldierAnim( PRONE_LAY_FROMHIT, 0, FALSE );
			else pSoldier->ChangeSoldierState( PRONE_LAY_FROMHIT, 0, FALSE );
			break;
		}
		break;
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY )
	{
		if ( gTacticalStatus.ubTheChosenOne == pSoldier->ubID )
		{
			auto bPanicTrigger = ClosestPanicTrigger(pSoldier);
			if (bPanicTrigger != -1 && !(gTacticalStatus.bPanicTriggerIsAlarm[bPanicTrigger]))
			{
				// replace this guy as the chosen one!
				gTacticalStatus.ubTheChosenOne = NOBODY;
				MakeClosestEnemyChosenOne( );
			}
		}

		if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) && (pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL) )
		{
#ifdef TESTAICONTROL
			DebugAI( String( "Ending turn for %d because of error from HandleItem", pSoldier->ubID ) );
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
	// calculate how many turns before he bleeds again
	// bleeding faster the lower life gets, and if merc is running around
	//pSoldier->nextbleed = 2 + (pSoldier->life / (10 + pSoldier->tilesMoved));  // min = 2

	// if bandaged, give 1/2 of the bandaged life points back into equation
	INT8 bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	FLOAT val = 1.0f;

	// Flugente: hemophiliacs bleed a lot faster
	if ( DoesMercHaveDisability( pSoldier, HEMOPHILIAC ) )
		val += ((FLOAT)(pSoldier->stats.bLife) / (FLOAT)(30 + 2 * pSoldier->bTilesMoved));
	else
		val += ((FLOAT)(pSoldier->stats.bLife + bBandaged / 2) / (FLOAT)(10 + pSoldier->bTilesMoved));

	return val;
}

FLOAT CalcSoldierNextUnmovingBleed( SOLDIERTYPE *pSoldier )
{
	INT8		bBandaged;

	// calculate bleeding rate without the penalty for tiles moved

	// if bandaged, give 1/2 of the bandaged life points back into equation
	bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	return((FLOAT)1 + (FLOAT)((pSoldier->stats.bLife + bBandaged / 2) / 10));  // min = 1
}

void HandlePlacingRoofMarker( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fSet, BOOLEAN fForce )
{
	LEVELNODE *pRoofNode;
	LEVELNODE *pNode;

	if ( pSoldier->bVisible == -1 && fSet )
	{
		return;
	}

	//CHRISL: If sGridNo is -1, which can be the case if there is a dead merc still listed as part of a unit, crashes will occur
	if ( sGridNo == -1 )
		return;

	if ( pSoldier->bTeam != gbPlayerNum )
	{
		//return;
	}

	// If we are on the roof, add roof UI peice!
	if ( pSoldier->pathing.bLevel == SECOND_LEVEL )
	{
		// Get roof node
		pRoofNode = gpWorldLevelData[sGridNo].pRoofHead;

		// Return if we are still climbing roof....
		if ( pSoldier->usAnimState == CLIMBUPROOF && !fForce )
		{
			return;
		}

		if ( pSoldier->usAnimState == JUMPUPWALL && !fForce )
		{
			return;
		}

		if ( pRoofNode != NULL )
		{
			if ( fSet )
			{
				if ( gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REVEALED )
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
				if ( gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REVEALED )
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
					pNode->ubShadeLevel = DEFAULT_SHADE_LEVEL;
					pNode->ubNaturalShadeLevel = DEFAULT_SHADE_LEVEL;
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
	if ( !gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] )
	{
		return;
	}

	if ( this->iLight == -1 )
	{
		this->CreateSoldierLight( );
	}

	//if ( this->ubID == gusSelectedSoldier )
	{
		LightSpritePower( this->iLight, TRUE );
		LightSpriteFake( this->iLight );

		LightSpritePosition( this->iLight, (INT16)(this->sX / CELL_X_SIZE), (INT16)(this->sY / CELL_Y_SIZE) );
	}
}

void SOLDIERTYPE::SetCheckSoldierLightFlag( )
{
	this->PositionSoldierLight( );
	//this->flags.uiStatusFlags |= SOLDIER_RECHECKLIGHT;
}


void PickPickupAnimation( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel )
{
	INT8				bDirection;
	STRUCTURE		*pStructure;
	BOOLEAN			fDoNormalPickup = TRUE;
	//SOLDIERTYPE *pTarget;
	//UINT16 usSoldierIndex;

	// OK, Given the gridno, determine if it's the same one or different....
	if ( sGridNo != pSoldier->sGridNo )
	{
		// Get direction to face....
		bDirection = (INT8)GetDirectionFromGridNo( sGridNo, pSoldier );
		pSoldier->ubPendingDirection = bDirection;

		// Change to pickup animation
		// SANDRO - determine which animation to choose, if we pickup item from struct, we can either stand or be crouched
		// when picking items from lying soldier (collapsed maybe), we need to be crouched always
		//usSoldierIndex = WhoIsThere2( sGridNo, this->pathing.bLevel );
		//if ( usSoldierIndex != NOBODY )
		/*{
		pTarget = MercPtrs[ usSoldierIndex ];

		if ( gAnimControl[ pTarget->usAnimState ].ubEndHeight == ANIM_PRONE || gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_CROUCH || gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE )
		pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM_CROUCHED, 0 , FALSE );
		else
		pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM, 0 , FALSE );
		}*/
		//else 
		{
			if ( gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_CROUCH || gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE )
				pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM_CROUCHED, 0, FALSE );
			else
				pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM, 0, FALSE );
		}

		if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC) )
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
			if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC) )
			{
				// reset action value for AI because we're done!
				ActionDone( pSoldier );
			}

		}
		else
		{
			// Don't show animation of getting item, if we are not standing
			switch ( gAnimControl[pSoldier->usAnimState].ubHeight )
			{
			case ANIM_STAND:

				// OK, if we are looking at z-level >0, AND
				// we have a strucxture with items in it
				// look for orientation and use angle accordingly....
				if ( bZLevel > 0 )
				{
					//#if 0
					// Get direction to face....
					if ( (pStructure = FindStructure( sGridNo, (STRUCTURE_HASITEMONTOP | STRUCTURE_OPENABLE) )) != NULL )
					{
						fDoNormalPickup = FALSE;

						// OK, look at orientation
						switch ( pStructure->ubWallOrientation )
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
						pSoldier->EVENT_InitNewSoldierAnim( ADJACENT_GET_ITEM, 0, FALSE );
					}
					//#endif
				}

				if ( fDoNormalPickup )
				{
					pSoldier->EVENT_InitNewSoldierAnim( PICKUP_ITEM, 0, FALSE );
				}

				if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC) )
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
				if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_PC) )
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
	switch ( gAnimControl[this->usAnimState].ubHeight )
	{
	case ANIM_STAND:

		this->aiData.bAction = AI_ACTION_PENDING_ACTION;
		this->EVENT_InitNewSoldierAnim( DROP_ITEM, 0, FALSE );
		break;

	case ANIM_CROUCH:
	case ANIM_PRONE:

		SoldierHandleDropItem( this );
		this->SoldierGotoStationaryStance( );
		ActionDone( this );
		break;
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginCutFence( INT32 sGridNo, UINT8 ubDirection )
{
	// Make sure we have a structure here....
	if ( IsCuttableWireFenceAtGridNo( sGridNo ) )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		//BOOLEAN CutWireFence( INT16 sGridNo )

		// SET TARGET GRIDNO
		this->sTargetGridNo = sGridNo;

		// CHANGE TO ANIMATION
		if ( !is_networked )
			this->EVENT_InitNewSoldierAnim( CUTTING_FENCE, 0, FALSE );
		else
			this->ChangeSoldierState( CUTTING_FENCE, 0, 0 );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginRepair( INT32 sGridNo, UINT8 ubDirection )
{
	INT8 bRepairItem;
	UINT16	ubID;

	// Make sure we have a structure here....
	bRepairItem = IsRepairableStructAtGridNo( sGridNo, &ubID );

	if ( bRepairItem )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( GOTO_REPAIRMAN, 0, FALSE );
		// SET BUDDY'S ASSIGNMENT TO REPAIR...
		if (gTacticalStatus.uiFlags & INCOMBAT)
		{
			//this doesn't work during combat, so return
			UnSetUIBusy(this->ubID);
			return;
		}
		
		// Are we a SAM site? ( 3 == SAM )
		if ( bRepairItem == 3 )
		{
			SetSoldierAssignment( this, REPAIR, TRUE, FALSE, -1 );
		}
		else if ( bRepairItem == 2 ) // ( 2 == VEHICLE )
		{
			SetSoldierAssignment( this, REPAIR, FALSE, FALSE, ubID );
		}
		else if (bRepairItem == 1) // ( 1 == ROBOT )
		{
			SetSoldierAssignment(this, REPAIR, FALSE, TRUE, -1);
		}

	}
}

void SOLDIERTYPE::EVENT_SoldierBeginRefuel( INT32 sGridNo, UINT8 ubDirection )
{
	INT8 bRefuelItem;
	UINT16	ubID;

	// Make sure we have a structure here....
	bRefuelItem = IsRefuelableStructAtGridNo( sGridNo, &ubID );

	if ( bRefuelItem )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( REFUEL_VEHICLE, 0, FALSE );
		// SET BUDDY'S ASSIGNMENT TO REPAIR...
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginTakeBlood( INT32 sGridNo, UINT8 ubDirection )
{
	ROTTING_CORPSE *pCorpse;


	// See if these is a corpse here....
	pCorpse = GetCorpseAtGridNo( sGridNo, this->pathing.bLevel );

	if ( pCorpse != NULL )
	{
		this->aiData.uiPendingActionData4 = pCorpse->iID;

		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		this->EVENT_InitNewSoldierAnim( TAKE_BLOOD_FROM_CORPSE, 0, FALSE );
	}
	else
	{
		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
}


void SOLDIERTYPE::EVENT_SoldierBeginAttachCan( INT32 sGridNo, UINT8 ubDirection )
{
	STRUCTURE *			pStructure;
	DOOR_STATUS *		pDoorStatus;

	// OK, find door, attach to door, do animation...., remove item....

	// First make sure we still have item in hand....
	if ( !ItemIsCanAndString(this->inv[HANDPOS].usItem))
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

	this->EVENT_InitNewSoldierAnim( ATTACH_CAN_TO_STRING, 0, FALSE );

	// Remove item...
	this->inv[HANDPOS].RemoveObjectsFromStack( 1 );
	fInterfacePanelDirty = DIRTYLEVEL2;

}

extern UINT8 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

void SOLDIERTYPE::EVENT_SoldierBuildStructure( INT32 sGridNo, UINT8 ubDirection )
{
	BOOLEAN fSuccess = FALSE;

	// if specified by the ini, building/disassembling stuff is disabled while enemies are around
	if ( !gGameExternalOptions.fFortificationAllowInHostileSector )
	{
		if ( gWorldSectorX != -1 && gWorldSectorY != -1 && gWorldSectorX != 0 && gWorldSectorY != 0 &&
			 NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) > 0 )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_CANNOT_BUILD] );
			return;
		}
	}

	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	if (this->ubDirection != ubDirection)
	{
		this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
		this->EVENT_SetSoldierDesiredDirection(ubDirection);
		this->EVENT_SetSoldierDirection(ubDirection);
	}

	// do checks here...
	OBJECTTYPE* pObj = &(this->inv[HANDPOS]);

	if ( pObj && pObj->exists( ) && ( IsStructureConstructItem( this->inv[HANDPOS].usItem, sGridNo, this ) || IsStructureDeconstructItem( this->inv[HANDPOS].usItem, sGridNo, this )) )
	{
		// it is possible that an item is used in both constructing and deconstructing. We thus check wether a structure already exists at sGridNo to dedcut the players intention
		// if there is a structure, we will remove this fortification. If there isn't, then we will dig and create a earth pile
		STRUCTURE* pStruct = FindStructure( sGridNo, STRUCTURE_GENERIC );

		if ( !pStruct && IsStructureConstructItem( this->inv[HANDPOS].usItem, sGridNo, this ) )
		{
			// Build the thing
			this->StartMultiTurnAction( MTA_FORTIFY, sGridNo );

			fSuccess = TRUE;
		}
		else if ( IsStructureDeconstructItem( this->inv[HANDPOS].usItem, sGridNo, this ) )
		{
			this->StartMultiTurnAction( MTA_REMOVE_FORTIFY, sGridNo );

			fSuccess = TRUE;
		}
	}

	if ( !fSuccess )
	{
		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
	else
	{
		if ( !is_networked )
			this->EVENT_InitNewSoldierAnim( CUTTING_FENCE, 0, FALSE );
		else
			this->ChangeSoldierState( CUTTING_FENCE, 0, 0 );
	}
}

void SOLDIERTYPE::EVENT_SoldierHandcuffPerson( INT32 sGridNo, UINT8 ubDirection )
{
	if ( !gGameExternalOptions.fAllowPrisonerSystem )
		return;

	UINT16 ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY && MercPtrs[ubPerson]->CanBeCaptured( ) )
	{
		// we found someone we can handcuff
		SOLDIERTYPE* pSoldier = MercPtrs[ubPerson];

		// check wether we will be successful
		BOOLEAN success = FALSE;
		if ( pSoldier->flags.fMercAsleep || pSoldier->bCollapsed )
			success = TRUE;
		else
		{
			// check wether we can forcefully handcuff the other soldier, but this will be hard
			UINT32 attackrating = 10 * EffectiveExpLevel( this ) + EffectiveStrength( this, FALSE ) + 2 * EffectiveDexterity( this, FALSE ) + EffectiveAgility( this, FALSE );
			UINT32 defenserating = 10 * EffectiveExpLevel( pSoldier ) + 2 * EffectiveStrength( pSoldier, FALSE ) + 2 * EffectiveDexterity( pSoldier, FALSE ) + 2 * EffectiveAgility( pSoldier, FALSE );

			if ( gGameOptions.fNewTraitSystem )
			{
				attackrating += 25 * NUM_SKILL_TRAITS( this, MARTIAL_ARTS_NT ) + 10 * HAS_SKILL_TRAIT( this, MELEE_NT );
				defenserating += 25 * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) + 10 * HAS_SKILL_TRAIT( pSoldier, MELEE_NT );
			}
			else
			{
				attackrating += 25 * NUM_SKILL_TRAITS( this, MARTIALARTS_OT ) + 25 * NUM_SKILL_TRAITS( this, HANDTOHAND_OT ) + 10 * HAS_SKILL_TRAIT( this, KNIFING_OT );
				defenserating += 25 * NUM_SKILL_TRAITS( pSoldier, MARTIALARTS_OT ) + 25 * NUM_SKILL_TRAITS( pSoldier, HANDTOHAND_OT ) + 10 * HAS_SKILL_TRAIT( pSoldier, KNIFING_OT );
			}

			ReducePointsForFatigue( this, &attackrating );
			ReducePointsForFatigue( pSoldier, &defenserating );

			if ( Random( attackrating ) > Random( defenserating ) + 100 )
				success = TRUE;
		}

		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim(RELOAD_ROBOT, 0, FALSE);

		if ( success )
		{
			// arrest this guy
			pSoldier->usSoldierFlagMask |= SOLDIER_POW;

			// Remove as target
			RemoveManAsTarget( pSoldier );

			// move the items in his hands to the floor and move our handcuff into his hands
			if ( pSoldier->inv[HANDPOS].exists( ) == true && !(pSoldier->inv[HANDPOS].fFlags & OBJECT_UNDROPPABLE) )
			{
				// ATE: if our guy, make visible....
				INT8 bVisible = (pSoldier->bTeam == gbPlayerNum) ? 1 : 0;
				UINT16 itemflags = (pSoldier->bTeam == ENEMY_TEAM) ? 0 : WORLD_ITEM_DROPPED_FROM_ENEMY;

				if ( UsingNewAttachmentSystem( ) == true )
					ReduceAttachmentsOnGunForNonPlayerChars( pSoldier, &(pSoldier->inv[HANDPOS]) );

				AddItemToPool( pSoldier->sGridNo, &(pSoldier->inv[HANDPOS]), bVisible, pSoldier->pathing.bLevel, itemflags, -1 ); //Madd: added usItemFlags to function arguments
				DeleteObj( &(pSoldier->inv[HANDPOS]) );
			}

			if ( pSoldier->inv[SECONDHANDPOS].exists( ) == true && !(pSoldier->inv[SECONDHANDPOS].fFlags & OBJECT_UNDROPPABLE) )
			{
				// ATE: if our guy, make visible....
				INT8 bVisible = (pSoldier->bTeam == gbPlayerNum) ? 1 : 0;
				UINT16 itemflags = (pSoldier->bTeam == ENEMY_TEAM) ? 0 : WORLD_ITEM_DROPPED_FROM_ENEMY;

				if ( UsingNewAttachmentSystem( ) == true )
					ReduceAttachmentsOnGunForNonPlayerChars( pSoldier, &(pSoldier->inv[SECONDHANDPOS]) );

				AddItemToPool( pSoldier->sGridNo, &(pSoldier->inv[SECONDHANDPOS]), bVisible, pSoldier->pathing.bLevel, itemflags, -1 ); //Madd: added usItemFlags to function arguments
				DeleteObj( &(pSoldier->inv[SECONDHANDPOS]) );
			}

			// move handcuffs to his hands
			if ( HasItemFlag( (&(this->inv[HANDPOS]))->usItem, HANDCUFFS ) )
			{
				// stack handcuffs (like a stack of binders) simply gets used up a bit
				if ( Item[(&(this->inv[HANDPOS]))->usItem].usItemClass == IC_KIT )
				{
					UseKitPoints( &(this->inv[HANDPOS]), 10, pSoldier );
				}
				else
				{
					AutoPlaceObject( pSoldier, &(this->inv[HANDPOS]), FALSE );
					DeleteObj( &(this->inv[HANDPOS]) );
				}
			}

			// we gain a bit of experience...
			StatChange( this, STRAMT, 2, TRUE );
			StatChange( this, DEXTAMT, 3, TRUE );
			StatChange( this, EXPERAMT, 2, TRUE );

			DeductPoints( this, GetAPsToHandcuff( this, sGridNo ), APBPConstants[BP_HANDCUFF], AFTERACTION_INTERRUPT );

			// this might have been the last guy...
			CheckForEndOfBattle( FALSE );
		}
		else
		{
			// we gain a bit of experience...
			StatChange( this, DEXTAMT, 2, TRUE );

			DeductPoints( this, GetAPsToHandcuff( this, sGridNo ), APBPConstants[BP_HANDCUFF], AFTERACTION_INTERRUPT );

			// curses!
			this->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

			// if we are disguised, there is a chance that he'll uncover us
			if ( this->usSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER) )
			{
				this->LooseDisguise();

				if ( gSkillTraitValues.fCOStripIfUncovered )
					this->Strip();
														
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_ACTIVITIES], this->GetName() );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_UNCOVERED], pSoldier->GetName(), this->GetName()  );

				// alert the soldier
				pSoldier->aiData.bAlertStatus = max( pSoldier->aiData.bAlertStatus, STATUS_RED );

				ProcessImplicationsOfPCAttack( this, &pSoldier, REASON_NORMAL_ATTACK );
			}
		}
	}
	else
	{
		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
}

void SOLDIERTYPE::EVENT_SoldierApplyItemToPerson( INT32 sGridNo, UINT8 ubDirection )
{
	UINT16 ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY )
	{
		// we found someone
		SOLDIERTYPE* pSoldier = MercPtrs[ubPerson];

		OBJECTTYPE* pObj = &(this->inv[HANDPOS]);

		if ( pSoldier && pObj->exists( ) )
		{
			UINT16 usItem = pObj->usItem;

			if ( ItemCanBeAppliedToOthers( usItem ) )
			{
				BOOLEAN success = TRUE;

				// if the other guy is not on our side, and he is concious, he resists
				if ( this->bSide != pSoldier->bSide && !pSoldier->bCollapsed )
				{
					// wether we are sucessful depends on dexterity, and his alert status (he gets a malus on green state)
					UINT32 attackervalue = 30 + 4 * EffectiveExpLevel( this ) + EffectiveDexterity( this, FALSE ) + 20 * HAS_SKILL_TRAIT( this, STEALTHY_NT );
					UINT32 defendervalue = 100 + 3 * EffectiveExpLevel( pSoldier ) + 100 * (pSoldier->aiData.bAlertStatus - 1);

					// we are penalized for the weight of the item
					UINT16 weight = pObj->GetWeightOfObjectInStack( 0 );
					attackervalue = attackervalue / weight;

					if ( Random( attackervalue ) > Random( defendervalue ) )
						success = TRUE;
					else
					{
						success = FALSE;

						// if we are disguised, there is a chance that he'll uncover us
						if ( this->usSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER) )
						{
							this->LooseDisguise();
							
							if ( gSkillTraitValues.fCOStripIfUncovered )
								this->Strip();
														
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_APPLYITEM_STEAL_FAIL], this->GetName(), pSoldier->GetName() );
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_UNCOVERED], pSoldier->GetName(), this->GetName()  );
						}

						// alert the soldier
						pSoldier->aiData.bAlertStatus = max( pSoldier->aiData.bAlertStatus, STATUS_RED );

						ProcessImplicationsOfPCAttack( this, &pSoldier, REASON_NORMAL_ATTACK );
					}
				}

				if ( success )
				{
					if (ItemIsGasmask(usItem))
					{
						// put this item into a (at best empty) faceslot if no gasmask is been worn
						INT8 bSlot = FindGasMask( pSoldier );
						if ( bSlot == NO_SLOT || (bSlot != HEAD1POS && bSlot != HEAD2POS) )
						{
							if ( !(pSoldier->inv[HEAD1POS]).exists( ) )
							{
								success = PlaceObject( pSoldier, HEAD1POS, pObj );
							}
							else if ( !(pSoldier->inv[HEAD2POS]).exists( ) )
							{
								success = PlaceObject( pSoldier, HEAD2POS, pObj );
							}
							else
							{
								// no gasmask is worn, and both face slots are occupied - remove the item in slot 2 and put the gasmask there
								AddItemToPool( pSoldier->sGridNo, &(pSoldier->inv[HEAD2POS]), 1, pSoldier->pathing.bLevel, 0, -1 );

								success = PlaceObject( pSoldier, HEAD2POS, pObj );
							}
						}
						else
						{
							success = FALSE;
						}
					}
					else if ( Item[usItem].usItemClass == IC_BOMB )
					{
						success = AutoPlaceObject( pSoldier, pObj, FALSE );
					}
					else if ( ApplyConsumable( pSoldier, pObj, TRUE, TRUE ) == TRUE)
					{
						success = TRUE;
					}
				}

				DeductPoints( this, GetAPsToApplyItem( this, sGridNo ), APBPConstants[BP_APPLYITEM], AFTERACTION_INTERRUPT );

				if ( !success )
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[ MSG113_COULD_NOT_APPLY ], this->GetName(), Item[usItem].szLongItemName, pSoldier->GetName() );
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_X_APPLY_Y_TO_Z], this->GetName(), Item[usItem].szLongItemName, pSoldier->GetName() );

					this->DoMercBattleSound( BATTLE_SOUND_COOL1 );
				}
			}
			else
			{
				DeductPoints( this, GetAPsToApplyItem( this, sGridNo ), APBPConstants[BP_APPLYITEM], AFTERACTION_INTERRUPT );

				// curses!
				this->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
			}

			return;
		}

		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
	else
	{
		// if this is  bomb, but nobody is there, plant the bomb instead
		OBJECTTYPE* pObj = &(this->inv[HANDPOS]);

		if ( pObj->exists( ) )
		{
			UINT16 usItem = pObj->usItem;

			if ( Item[usItem].usItemClass == IC_BOMB )
			{
				this->EVENT_SoldierBeginDropBomb( );
			}
		}
	}
}

void SOLDIERTYPE::EVENT_SoldierTakeBloodFromPerson( INT32 sGridNo, UINT8 ubDirection )
{
	UINT16 ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY && ubPerson != this->ubID )
	{
		// we found someone
		SOLDIERTYPE* pSoldier = MercPtrs[ubPerson];

		OBJECTTYPE* pObj = &( this->inv[HANDPOS] );

		if ( pSoldier && pObj->exists() && HasItemFlag( pObj->usItem, EMPTY_BLOOD_BAG ) && pSoldier->IsValidBloodDonor() )
		{
			// delete empty blood bag
			DeleteObj( pObj );
				
			// create full blood bag
			static UINT16 bloodbagitem = 1757;

			if ( HasItemFlag( bloodbagitem, BLOOD_BAG ) || GetFirstItemWithFlag( &bloodbagitem, BLOOD_BAG ) )
			{
				CreateItem( bloodbagitem, 100, &gTempObject );

				// Flugente: if this guy has the disease, infect object
				if ( pSoldier->sDiseasePoints[0] > 0 )
					gTempObject[0]->data.sObjectFlag |= INFECTED;

				if ( !AutoPlaceObject( this, &gTempObject, FALSE ) )
					AddItemToPool( pSoldier->sGridNo, &gTempObject, VISIBLE, this->pathing.bLevel, 0, -1 );

				// wound the donor
				// we don't want the health loss taken to be healable by surgery (how would surgery restore that), so reset it afterwards
				INT32 healableinjury_tmp = pSoldier->iHealableInjury;
				INT8 bleeding_tmp = pSoldier->bBleeding;

				// we need to set attacker as NOBODY, otherwise this calls dialogue events. This can be justified since they 'volunteered' for this
				pSoldier->SoldierTakeDamage( 0, BLOODDONATION_AMOUNT, 0, TAKE_DAMAGE_BLOODLOSS, NOBODY, sGridNo, 0, TRUE );

				pSoldier->iHealableInjury = healableinjury_tmp;
				pSoldier->bBleeding = bleeding_tmp;

				DeductPoints( this, GetAPsToFillBloodbag( this, sGridNo ), APBPConstants[BP_FILLBLOODBAG], AFTERACTION_INTERRUPT );

				return;
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Error: no blood bag item found in Items.xml!" );
			}
		}

		// Say NOTHING quote...
		this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}
	else
	{
		// if this is  bomb, but nobody is there, plant the bomb instead
		OBJECTTYPE* pObj = &( this->inv[HANDPOS] );

		if ( pObj->exists() )
		{
			UINT16 usItem = pObj->usItem;

			if ( Item[usItem].usItemClass == IC_BOMB )
			{
				this->EVENT_SoldierBeginDropBomb();
			}
		}
	}
}

void SOLDIERTYPE::EVENT_SoldierApplySplintToPerson( INT32 sGridNo, UINT8 ubDirection )
{
	UINT16 ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY && ubPerson != this->ubID )
	{
		// we found someone
		SOLDIERTYPE* pSoldier = MercPtrs[ubPerson];

		OBJECTTYPE* pObj = &( this->inv[HANDPOS] );
		
		if ( pSoldier
			&& pObj->exists()
			&& HasItemFlag( pObj->usItem, MEDICAL_SPLINT )
			&& pSoldier->CanReceiveSplint()
			&& ( (gGameOptions.fNewTraitSystem && NUM_SKILL_TRAITS( this, DOCTOR_NT ) > 0) || (!gGameOptions.fNewTraitSystem && EffectiveMedical( this ) >= 50) ) )
		{
			UINT16 usItem = pObj->usItem;

			// delete object
			DeleteObj( pObj );

			// add flag to arm or leg (we'll find out which while we're doing it)
			bool addtoarm = true;
			bool addtoleg = true;
			for ( int i = 0; i < NUM_DISEASES; ++i )
			{
				if ( pSoldier->sDiseasePoints[i] > 0 )
				{
					if ( addtoarm
						&& Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_ARMS
						&& !( pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_SPLINTAPPLIED_ARM ) )
					{
						pSoldier->sDiseaseFlag[i] |= SOLDIERDISEASE_SPLINTAPPLIED_ARM;
						addtoleg = false;
					}

					if ( addtoleg
						&& Disease[i].usDiseaseProperties & DISEASE_PROPERTY_LIMITED_USE_LEGS
						&& !( pSoldier->sDiseaseFlag[i] & SOLDIERDISEASE_SPLINTAPPLIED_LEG ) )
					{
						pSoldier->sDiseaseFlag[i] |= SOLDIERDISEASE_SPLINTAPPLIED_LEG;
						addtoarm = false;
					}
				}
			}

			DeductPoints( this, GetAPsToApplyItem( this, sGridNo ), APBPConstants[BP_APPLYITEM], AFTERACTION_INTERRUPT );

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_X_APPLY_Y_TO_Z], this->GetName(), Item[usItem].szLongItemName, pSoldier->GetName() );

			if ( !is_networked )
				this->EVENT_InitNewSoldierAnim( CUTTING_FENCE, 0, FALSE );
			else
				this->ChangeSoldierState( CUTTING_FENCE, 0, 0 );

			return;
		}
	}

	// If this didn't work, say NOTHING quote...
	this->DoMercBattleSound( BATTLE_SOUND_NOTHING );
}

void SOLDIERTYPE::EVENT_SoldierInteractiveAction( INT32 sGridNo, UINT16 usActionType )
{
	DoInteractiveAction( sGridNo, this );
}

void SOLDIERTYPE::EVENT_SoldierBeginReloadRobot( INT32 sGridNo, UINT8 ubDirection, UINT8 ubMercSlot )
{
	UINT16 ubPerson;

	// Make sure we have a robot here....
	ubPerson = WhoIsThere2( sGridNo, this->pathing.bLevel );

	if ( ubPerson != NOBODY && MercPtrs[ubPerson]->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		// CHANGE DIRECTION AND GOTO ANIMATION NOW
		if (this->ubDirection != ubDirection)
		{
			this->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
			this->EVENT_SetSoldierDesiredDirection(ubDirection);
			this->EVENT_SetSoldierDirection(ubDirection);
		}

		// CHANGE TO ANIMATION
		this->EVENT_InitNewSoldierAnim( RELOAD_ROBOT, 0, FALSE );

	}
}



void SOLDIERTYPE::ResetSoldierChangeStatTimer( void )
{
	this->timeChanges.uiChangeLevelTime = 0;
	this->timeChanges.uiChangeHealthTime = 0;
	this->timeChanges.uiChangeStrengthTime = 0;
	this->timeChanges.uiChangeDexterityTime = 0;
	this->timeChanges.uiChangeAgilityTime = 0;
	this->timeChanges.uiChangeWisdomTime = 0;
	this->timeChanges.uiChangeLeadershipTime = 0;
	this->timeChanges.uiChangeMarksmanshipTime = 0;
	this->timeChanges.uiChangeExplosivesTime = 0;
	this->timeChanges.uiChangeMedicalTime = 0;
	this->timeChanges.uiChangeMechanicalTime = 0;


	return;
}


void SOLDIERTYPE::ChangeToFlybackAnimation( UINT8 flyBackDirection )
{
	INT32 sNewGridNo;
	UINT8 ubOppositeDir;
	INT16 sDirectionInc;

	ubOppositeDir = gOppositeDirection[flyBackDirection];
	sDirectionInc = DirectionInc( ubOppositeDir );

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( this->sGridNo, sDirectionInc );
	if ( gubWorldMovementCosts[sNewGridNo][ubOppositeDir][this->pathing.bLevel] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Pretend we hit the wall and fall forward instead
		this->BeginTyingToFall( );
		this->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		return;
	}

	sNewGridNo = NewGridNo( sNewGridNo, sDirectionInc );
	if ( gubWorldMovementCosts[sNewGridNo][ubOppositeDir][this->pathing.bLevel] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Fall back instead
		this->BeginTyingToFall( );
		this->ChangeSoldierState( FALLBACK_HIT_STAND, 0, FALSE );
	}


	// Remove any previous actions
	this->aiData.ubPendingAction = NO_PENDING_ACTION;

	this->sPlotSrcGrid = this->sGridNo;

	// Since we're manually setting our path, we have to reset these @#$@# flags too.  Otherwise we don't reach the
	// destination a lot of the time
	this->flags.fPastXDest = 0;
	this->flags.fPastYDest = 0;

	// Set path....
	this->pathing.usPathDataSize = 0;
	this->pathing.usPathIndex = 0;
	this->pathing.usPathingData[this->pathing.usPathDataSize] = gOppositeDirection[this->ubDirection];
	this->pathing.usPathDataSize++;
	this->pathing.usPathingData[this->pathing.usPathDataSize] = gOppositeDirection[this->ubDirection];
	this->pathing.usPathDataSize++;
	this->pathing.sFinalDestination = sNewGridNo;
	this->EVENT_InternalSetSoldierDestination( (UINT8) this->pathing.usPathingData[this->pathing.usPathIndex], FALSE, FLYBACK_HIT );

	// Get a new direction based on direction
	this->EVENT_InitNewSoldierAnim( FLYBACK_HIT, 0, FALSE );
}

void SOLDIERTYPE::ChangeToFallbackAnimation( UINT8 fallBackDirection )
{
	INT32 sNewGridNo;
	UINT8 ubOppositeDir;
	INT16 sDirection;

	ubOppositeDir = gOppositeDirection[fallBackDirection];
	sDirection = DirectionInc( ubOppositeDir );

	// Get dest gridno, convert to center coords
	sNewGridNo = NewGridNo( this->sGridNo, sDirection );
	if ( gubWorldMovementCosts[sNewGridNo][ubOppositeDir][this->pathing.bLevel] >= TRAVELCOST_BLOCKED )
	{
		// No room to fly back.  Pretend we hit the wall and fall forward instead
		this->BeginTyingToFall( );
		this->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
		return;
	}

	// Get dest gridno, convert to center coords
	//sNewGridNo = NewGridNo( sNewGridNo, sDirection );
	//usNewGridNo = NewGridNo( (UINT16)usNewGridNo, (UINT16)(-1 * DirectionInc( bDirection ) ) );

	// Remove any previous actions
	this->aiData.ubPendingAction = NO_PENDING_ACTION;

	this->sPlotSrcGrid = this->sGridNo;

	// Since we're manually setting our path, we have to reset these @#$@# flags too.  Otherwise we don't reach the
	// destination a lot of the time
	this->flags.fPastXDest = 0;
	this->flags.fPastYDest = 0;

	// Set path....
	this->pathing.usPathDataSize = 0;
	this->pathing.usPathIndex = 0;
	this->pathing.usPathingData[this->pathing.usPathDataSize] = gOppositeDirection[this->ubDirection];
	this->pathing.usPathDataSize++;
	this->pathing.sFinalDestination = sNewGridNo;
	this->EVENT_InternalSetSoldierDestination( this->pathing.usPathingData[this->pathing.usPathIndex], FALSE, FALLBACK_HIT_STAND );

	// Get a new direction based on direction
	this->EVENT_InitNewSoldierAnim( FALLBACK_HIT_STAND, 0, FALSE );
}


void SOLDIERTYPE::SetSoldierCowerState( BOOLEAN fOn )
{
	// Robot's don't cower!
	if ( this->ubBodyType == ROBOTNOWEAPON )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "ERROR: Robot was told to cower!" ) );
		return;
	}

	// OK< set flag and do anim...
	if ( fOn )
	{
		if ( !(this->flags.uiStatusFlags & SOLDIER_COWERING) )
		{
			this->EVENT_InitNewSoldierAnim( START_COWER, 0, FALSE );

			this->flags.uiStatusFlags |= SOLDIER_COWERING;

			this->ubDesiredHeight = ANIM_CROUCH;
		}
	}
	else
	{
		if ( (this->flags.uiStatusFlags & SOLDIER_COWERING) || (gAnimControl[this->usAnimState].ubEndHeight != ANIM_STAND) )
		{
			this->EVENT_InitNewSoldierAnim( END_COWER, 0, FALSE );

			this->flags.uiStatusFlags &= (~SOLDIER_COWERING);

			this->ubDesiredHeight = ANIM_STAND;
		}
	}
}

void MercStealFromMerc( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTarget )
{
	INT32 sActionGridNo, sGridNo, sAdjustedGridNo;
	UINT8	ubDirection;


	// OK, find an adjacent gridno....
	sGridNo = pTarget->sGridNo;

	// See if we can get there to punch
	sActionGridNo = FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
	if ( sActionGridNo != -1 )
	{
		// SEND PENDING ACTION
		pSoldier->aiData.ubPendingAction = MERC_STEAL;
		pSoldier->bTargetLevel = pTarget->pathing.bLevel; // Overhaul:  Update the level too!
		pSoldier->aiData.sPendingActionData2 = pTarget->sGridNo;
		pSoldier->aiData.bPendingActionData3 = ubDirection;
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

			if ( gAnimControl[pTarget->usAnimState].ubEndHeight == ANIM_PRONE || gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_PRONE || gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_CROUCH )
				pSoldier->EVENT_InitNewSoldierAnim( STEAL_ITEM_CROUCHED, 0, FALSE );
			else
				pSoldier->EVENT_InitNewSoldierAnim( STEAL_ITEM, 0, FALSE );
		}

		// OK, set UI
		//		gTacticalStatus.ubAttackBusyCount++;
		// reset attacking item (hand)
		pSoldier->usAttackingWeapon = 0;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "!!!!!!! Starting STEAL attack, attack count now %d", gTacticalStatus.ubAttackBusyCount ) );
		DebugAttackBusy( String( "!!!!!!! Starting STEAL attack, attack count now %d\n", gTacticalStatus.ubAttackBusyCount ) );

		SetUIBusy( pSoldier->ubID );
	}
}

void AbandonBoxingDueToSurrenderCallback(UINT8 ubExitValue)
{
	if (ubExitValue == MSG_BOX_RETURN_YES)
	{
		SetBoxingState(DISQUALIFIED);
		TriggerNPCRecord(DARREN, 21);
	}
}

SOLDIERTYPE				*pTMilitiaSoldier;//global pointer
BOOLEAN SOLDIERTYPE::PlayerSoldierStartTalking( UINT16 ubTargetID, BOOLEAN fValidate )
{
	INT16							sFacingDir, sXPos, sYPos, sAPCost;
	SOLDIERTYPE				*pTSoldier;
	UINT32						uiRange;

	if ( ubTargetID == NOBODY )
	{
		return(FALSE);
	}

	pTSoldier = MercPtrs[ubTargetID];

	// Check distance again, to be sure
	if ( fValidate )
	{
		// OK, since we locked this guy from moving
		// we should be close enough, so talk ( unless he is now dead )
		if ( !IsValidTalkableNPC( ubTargetID, FALSE, FALSE, FALSE ) )
		{
			return(FALSE);
		}

		uiRange = PythSpacesAway( this->sGridNo, pTSoldier->sGridNo );

		if ( uiRange > (NPC_TALK_RADIUS * 2) )
		{
			// Todo here - should we follow dude?
			return(FALSE);
		}


	}

	// Get APs...
	sAPCost = APBPConstants[AP_TALK];

	if ( !(gTacticalStatus.uiFlags & INCOMBAT) || (gTacticalStatus.uiFlags & REALTIME) ) //lal
	{
		ConvertGridNoToXY( pTSoldier->sGridNo, &sXPos, &sYPos );

		// Get direction from mouse pos
		sFacingDir = GetDirectionFromXY( sXPos, sYPos, this );

		// Set our guy facing
		SendSoldierSetDesiredDirectionEvent( this, sFacingDir );

		// Set NPC facing
		SendSoldierSetDesiredDirectionEvent( pTSoldier, gOppositeDirection[sFacingDir] );

		// Stop our guys...
		this->EVENT_StopMerc( this->sGridNo, this->ubDirection );
	}

	pTMilitiaSoldier = pTSoldier; //lal

	//lal
	// ATE; Check for normal civs...

	if ( GetCivType( pTSoldier ) != CIV_TYPE_NA )
	{
		//lal
		if ( (pTSoldier->bTeam == MILITIA_TEAM) && (gGameExternalOptions.fAllowTacticalMilitiaCommand == TRUE) && (this->bSide == pTSoldier->bSide) )
		{
			PopupMilitiaControlMenu( pTSoldier );
			return(FALSE);
		}
		else
		{
			// Flugente: if we are talking to an enemy, we have the option to offer them surrender... we can also ask them to become a turncoat
			if ( (gSkillTraitValues.fCOTurncoats || gGameExternalOptions.fEnemyCanSurrender || gGameExternalOptions.fPlayerCanAsktoSurrender) && pTSoldier->CanBeCaptured( ) )
			{
				// AP costs are handled inside the callback (PrisonerSurrenderMessageBoxCallBack)
				if (gTacticalStatus.bBoxingState != NOT_BOXING)
				{
					// pop up dialogue asking whether the player wants to abandon the fight
					DoMessageBox(MSG_BOX_BASIC_STYLE, Message[STR_ABANDON_FIGHT], GAME_SCREEN, (UINT8)MSG_BOX_FLAG_YESNO, AbandonBoxingDueToSurrenderCallback, NULL);
					return FALSE;
				}
				HandleSurrenderOffer( pTSoldier );
				return(FALSE);
			}
			else if ( pTSoldier->sNonNPCTraderID > 0 )
			{
				DeductPoints( this, sAPCost, 0, UNTRIGGERED_INTERRUPT );

				UINT8 ubTownID = StrategicMap[CALCULATE_STRATEGIC_INDEX( pTSoldier->sSectorX, pTSoldier->sSectorY )].bNameId;

				// not possible if this guy is hostile towards us
				// however, if we are covert as a soldier, this check does not apply - merchants know better than do defy the army
				if ( !pTSoldier->aiData.bNeutral && !(this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER) )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, szNonProfileMerchantText[0] );
				}
				// not possible if this guy is incapitated
				else if ( pTSoldier->bCollapsed || pTSoldier->bBreathCollapsed || pTSoldier->stats.bLife < OKLIFE )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, szNonProfileMerchantText[1] );
				}
				// not possible in combat
				else if ( gTacticalStatus.uiFlags & INCOMBAT )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, szNonProfileMerchantText[2] );
				}		
				// not possible of loyalty too low
				// however, if we are covert as a soldier, this check does not apply - merchants know better than do defy the army
				else if ( ubTownID != BLANK_SECTOR && gTownLoyalty[ubTownID].ubRating < armsDealerInfo[pTSoldier->sNonNPCTraderID].nonprofile_loyaltyrequired && !(this->usSoldierFlagMask & SOLDIER_COVERT_SOLDIER) )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, szNonProfileMerchantText[3] );
				}
				// if we passed the above check, we can trade
				else
				{
					DeductPoints( this, sAPCost, 0, UNTRIGGERED_INTERRUPT );
					EnterShopKeeperInterfaceScreen_NonNPC( pTSoldier->sNonNPCTraderID, pTSoldier->ubID );
				}

				return FALSE;
			}
			else
			{
				DeductPoints( this, sAPCost, 0, UNTRIGGERED_INTERRUPT );

				// Flugente: if this guy is a potential volunteer, we might be able to sway him
				HandleVolunteerRecruitment( this, pTSoldier );

				StartCivQuote( pTSoldier );
				return(FALSE);
			}
		}
	}

	DeductPoints( this, sAPCost, 0, UNTRIGGERED_INTERRUPT );

	// Are we an EPC that is being escorted?
	if ( pTSoldier->ubProfile != NO_PROFILE && pTSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		return(InitiateConversation( pTSoldier, this, APPROACH_EPC_WHO_IS_RECRUITED, 0 ));
		//Converse( pTSoldier->ubProfile, this->ubProfile, APPROACH_EPC_WHO_IS_RECRUITED, 0 );
	}
	else if ( pTSoldier->aiData.bNeutral )
	{
		if ( pTSoldier->IsAssassin( ) )
		{
			// Start combat etc
			DeleteTalkingMenu( );
			DebugAI(AI_MSG_INFO, pTSoldier, String("CancelAIAction: assasin: start talking"));
			CancelAIAction( pTSoldier, TRUE );
			AddToShouldBecomeHostileOrSayQuoteList( pTSoldier->ubID );
		}
		else
		{
			// Start talking!
			return(InitiateConversation( pTSoldier, this, NPC_INITIAL_QUOTE, 0 ));
		}
	}
	else
	{
		// Start talking with hostile NPC
		return(InitiateConversation( pTSoldier, this, APPROACH_ENEMY_NPC_QUOTE, 0 ));
	}

	return(TRUE);
}


BOOLEAN SOLDIERTYPE::IsValidSecondHandShot( void )
{
	if ( Item[this->inv[SECONDHANDPOS].usItem].usItemClass == IC_GUN &&
		 !ItemIsTwoHanded(this->inv[SECONDHANDPOS].usItem) &&
		 (!this->bDoBurst || this->IsValidSecondHandBurst( )) &&
		 !ItemIsGrenadeLauncher(this->inv[HANDPOS].usItem) &&
		 Item[this->inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		 !ItemIsTwoHanded(this->inv[HANDPOS].usItem) &&
		 this->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE &&
		 this->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0 )
	{
		return(TRUE);
	}

	return(FALSE);
}

BOOLEAN SOLDIERTYPE::IsValidSecondHandBurst( void )
{
	// SANDRO - a function to determine if we can autofire with both weapons
	if ( Item[this->inv[SECONDHANDPOS].usItem].usItemClass == IC_GUN &&
		 !ItemIsTwoHanded(this->inv[SECONDHANDPOS].usItem) &&
		 !ItemIsGrenadeLauncher(this->inv[HANDPOS].usItem) &&
		 this->bDoBurst &&
		 Item[this->inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		 !ItemIsTwoHanded(this->inv[HANDPOS].usItem) &&
		 this->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE &&
		 this->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0 )
	{
		if ( this->bDoAutofire )
		{
			// if second gun cannot use atuofire mode
			if ( !IsGunAutofireCapable( &this->inv[SECONDHANDPOS] ) )
			{
				return(FALSE);
			}
		}
		else
		{
			// if second gun cannot use burst mode or the burst size is different
			if ( !(IsGunBurstCapable( &this->inv[SECONDHANDPOS], FALSE, NULL )) || (GetShotsPerBurst( &this->inv[HANDPOS] ) != GetShotsPerBurst( &this->inv[SECONDHANDPOS] )) )
			{
				return(FALSE);
			}
		}
		return(TRUE);
	}

	return(FALSE);
}

BOOLEAN SOLDIERTYPE::IsValidSecondHandShotForReloadingPurposes( void )
{
	// should be maintained as same as function above with line
	// about ammo taken out!
	if ( Item[this->inv[SECONDHANDPOS].usItem].usItemClass == IC_GUN &&
		 //!this->bDoBurst &&
		 !ItemIsGrenadeLauncher(this->inv[HANDPOS].usItem) &&
		 Item[this->inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		 this->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE //&&
		 //			 this->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0 &&
		 //			 gAnimControl[ this->usAnimState ].ubEndHeight != ANIM_PRONE )
		 )
	{
		return(TRUE);
	}

	return(FALSE);
}

BOOLEAN SOLDIERTYPE::IsValidAlternativeFireMode( INT16 bAimTime, INT32 iTrgGridNo )
{
	if ( this->IsValidShotFromHip( bAimTime, iTrgGridNo ) || this->IsValidPistolFastShot( bAimTime, iTrgGridNo ) )
	{
		return(TRUE);
	}

	return(FALSE);
}

BOOLEAN SOLDIERTYPE::IsValidShotFromHip( INT16 bAimTime, INT32 iTrgGridNo )
{
	// not allowed, or not gun in hand, or not standing
	if ( !gGameExternalOptions.ubAllowAlternativeWeaponHolding || Item[this->inv[HANDPOS].usItem].usItemClass != IC_GUN || gAnimControl[this->usAnimState].ubEndHeight != ANIM_STAND )
	{
		return(FALSE);
	}
	// robots and tanks cannot do this
	if ( AM_A_ROBOT( this ) || ARMED_VEHICLE( this ) || ENEMYROBOT( this ) )//dnl ch64 300813
	{
		return(FALSE);
	}
	// must be two handed for this
	if ( !ItemIsTwoHanded(this->inv[HANDPOS].usItem) )
	{
		return(FALSE);
	}
	// with hybrid aiming behaviour, our stance is important, we don't go from shoulder to hip
	if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 2 )
	{
		// shouldered already?
		if ( (gAnimControl[this->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) && !(gAnimControl[this->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING)) )
		{
			return(FALSE);
		}
		// aiming over hip (yellow) indicated levels (and not heavy gun - those are always fired from hip, if aiming from hip allowed)
		if ( bAimTime > GetNumberAltFireAimLevels( this, iTrgGridNo ) && !Weapon[this->inv[HANDPOS].usItem].HeavyGun )
		{
			return(FALSE);
		}
	}
	// "scope mode" behaviour lets us select firing from hip manually
	else if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
	{
		// we don't care about the stance here, as the player should know what he's doing, we just care about his fire mode selected
		if ( this->bScopeMode != USE_ALT_WEAPON_HOLD )
		{
			return(FALSE);
		}
		// we go through, only if scope mode is set on "alternative fire mode"
	}
	else if ( bAimTime > 0 )
	{
		// no aiming allowed with this
		return(FALSE);
	}

	// if we are here, assume we are going to fire from hip
	return(TRUE);

}

BOOLEAN SOLDIERTYPE::IsValidPistolFastShot( INT16 bAimTime, INT32 iTrgGridNo )
{
	// not allowed, or not gun in hand, or not standing
	if ( !gGameExternalOptions.ubAllowAlternativeWeaponHolding || Item[this->inv[HANDPOS].usItem].usItemClass != IC_GUN || gAnimControl[this->usAnimState].ubEndHeight != ANIM_STAND )
	{
		return(FALSE);
	}
	// robots and tanks cannot do this
	if ( AM_A_ROBOT( this ) || ARMED_VEHICLE( this ) || ENEMYROBOT( this ) )//dnl ch64 300813
	{
		return(FALSE);
	}
	// don't do this in water (yet), and not if firing from 2 guns
	if ( this->MercInWater( ) || this->IsValidSecondHandShot( ) )
	{
		return(FALSE);
	}
	// must be one handed for this
	if (ItemIsTwoHanded(this->inv[HANDPOS].usItem))
	{
		return(FALSE);
	}
	// with hybrid aiming behaviour, our stance is important, we don't go from two-handed to one-handed grip
	if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 2 )
	{
		// raised already?
		if ( (gAnimControl[this->usAnimState].uiFlags & (ANIM_FIREREADY | ANIM_FIRE)) && !(gAnimControl[this->usAnimState].uiFlags & (ANIM_ALT_WEAPON_HOLDING)) )
		{
			return(FALSE);
		}
		// aiming over alternative (yellow) indicated levels
		if ( bAimTime > GetNumberAltFireAimLevels( this, iTrgGridNo ) )
		{
			return(FALSE);
		}
	}
	// "scope mode" behaviour lets us select alternative firing manually
	else if ( gGameExternalOptions.ubAllowAlternativeWeaponHolding == 3 )
	{
		// we don't care about the stance here, as the player should know what he's doing, we just care about his fire mode selected
		if ( this->bScopeMode != USE_ALT_WEAPON_HOLD )
		{
			return(FALSE);
		}
		// we go through, only if scope mode is set on "alternative fire mode"
	}
	else if ( bAimTime > 0 )
	{
		// no aiming allowed with this
		return(FALSE);
	}

	// if we are here, assume we are going to fire from alternative stance
	return(TRUE);

}
BOOLEAN SOLDIERTYPE::CanRobotBeControlled( void )
{
	SOLDIERTYPE *pController;

	if ( !(this->flags.uiStatusFlags & SOLDIER_ROBOT) )
	{
		return(FALSE);
	}

	if ( this->ubRobotRemoteHolderID == NOBODY )
	{
		return(FALSE);
	}

	pController = MercPtrs[this->ubRobotRemoteHolderID];

	if ( pController->bActive )
	{
		if ( pController->ControllingRobot( ) )
		{
			// ALL'S OK!
			return(TRUE);
		}
	}

	return(FALSE);
}


BOOLEAN SOLDIERTYPE::ControllingRobot( void )
{
	SOLDIERTYPE *pRobot;
	INT8				bPos;

	if ( !this->bActive )
	{
		return(FALSE);
	}

	// EPCs can't control the robot (no inventory to hold remote, for one)
	if ( AM_AN_EPC( this ) )
	{
		return(FALSE);
	}

	// Don't require this->bInSector here, it must work from mapscreen!

	// are we in ok shape?
	if ( this->stats.bLife < OKLIFE || (this->bTeam != gbPlayerNum) )
	{
		return(FALSE);
	}

	// allow control from within vehicles - allows strategic travel in a vehicle with robot!
	if ( (this->bAssignment >= ON_DUTY) && (this->bAssignment != VEHICLE) )
	{
		return(FALSE);
	}

	// is the soldier wearing a robot remote control?
	bPos = FindRemoteControl( this );
	if ( bPos == NO_SLOT )
	{
		return(FALSE);
	}

	// Find the robot
	pRobot = FindSoldierByProfileID( ROBOT, TRUE );
	if ( !pRobot )
	{
		return(FALSE);
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
						return(FALSE);
					}

					// if in a vehicle, must be the same vehicle
					if ( pRobot->bAssignment == VEHICLE && (pRobot->iVehicleId != this->iVehicleId) )
					{
						return(FALSE);
					}
				}

				// all OK!
				return(TRUE);
			}
		}
	}

	return(FALSE);
}


SOLDIERTYPE *SOLDIERTYPE::GetRobotController( void )
{
	if ( this->ubRobotRemoteHolderID == NOBODY )
	{
		return(NULL);
	}
	else
	{
		return(MercPtrs[this->ubRobotRemoteHolderID]);
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
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive )
		{
			if ( pTeamSoldier->ControllingRobot( ) )
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
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// Loop through guys to find the robot....
	for ( pTeamSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && (pTeamSoldier->flags.uiStatusFlags & SOLDIER_ROBOT) )
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
		if ( (GetJA2Clock( ) - this->uiTimeSinceLastBleedGrunt) > 1000 )
		{
			this->uiTimeSinceLastBleedGrunt = GetJA2Clock( );

			this->DoMercBattleSound( BATTLE_SOUND_HIT1 );
		}
	}
	// shadooow: Do this ONLY if buddy is in sector.....
	if ((this->bInSector && guiCurrentScreen == GAME_SCREEN) || guiCurrentScreen != GAME_SCREEN)
	{
		// Flash portrait....
		this->flags.fFlashPortrait = TRUE;
		this->bFlashPortraitFrame = FLASH_PORTRAIT_STARTSHADE;
		RESETTIMECOUNTER(this->timeCounters.PortraitFlashCounter, FLASH_PORTRAIT_DELAY);
	}
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
			// silversurfer: bugfix for endless dying mercs on roof edges
			// if we delete their pending animation here they will just turn into the proper direction for the fall (in TurnSoldier( void ) )
			// and stand there forever afterwards in "dying" state, so let this guy fall off the roof first!
			if ( pSoldier->usPendingAnimation != FALLOFF && pSoldier->usPendingAnimation != FALLFORWARD_ROOF )
				pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
			pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;
			pSoldier->flags.bTurningFromPronePosition = FALSE;
			pSoldier->ubPendingDirection = NO_PENDING_DIRECTION;
			pSoldier->aiData.ubPendingAction = NO_PENDING_ACTION;
			pSoldier->bEndDoorOpenCode = 0;

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
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something: DONE IN SYSTEM NEW SITUATION" ) );
					DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending fire because saw something: DONE IN SYSTEM NEW SITUATION\n" );
					FreeUpAttacker( );
				}

				if ( pSoldier->pTempObject != NULL )
				{
					// Place it back into inv....
					AutoPlaceObject( pSoldier, pSoldier->pTempObject, FALSE );
					OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
					pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
					pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;

					// Decrement attack counter...
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "@@@@@@@ Reducing attacker busy count..., ending throw because saw something: DONE IN SYSTEM NEW SITUATION" ) );
					DebugAttackBusy( "@@@@@@@ Reducing attacker busy count..., ending throw because saw something: DONE IN SYSTEM NEW SITUATION\n" );
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
	if ( !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
	{
		if ( pSoldier->usAnimState == HOPFENCE || pSoldier->usAnimState == JUMPWINDOWS )
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
			if ( !(gTacticalStatus.uiFlags & INCOMBAT) && (pSoldier->ubID != gusSelectedSoldier) )
			{
				bVolume = LOWVOLUME;
			}

			PlaySoldierJA2Sample( pSoldier->ubID, ubSoundBase + pSoldier->ubLastFootPrintSound, RATE_11025, SoundVolume( bVolume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
		}
	}
	else
	{
		// anv: vehicle sounds
		//PlaySoldierJA2Sample( pSoldier->ubID, S_VECH1_MOVE, RATE_11025, SoundVolume( bVolume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
		if ( pSoldier->usAnimState == RUNNING )
		{
			bVolume = HIGHVOLUME;
		}

		if ( pVehicleList )
			PlaySoldierJA2Sample( pSoldier->ubID, pVehicleList[pSoldier->bVehicleID].iMoveSound, RATE_11025, SoundVolume( bVolume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
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

	for ( cnt = gTacticalStatus.Team[ubTeam].bFirstID, pTeamSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ubTeam].bLastID; cnt++, pTeamSoldier++ )
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
	CHAR16 sString[1024];
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
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt, pSoldier++ )
	{
		if ( pSoldier->bActive )
		{
			// OK, first check for alive people
			// Don't do this check if we are a vehicle...
			if ( pSoldier->stats.bLife > 0 && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
			{
				// Alive -- now check for proper group IDs
				if ( pSoldier->ubGroupID == 0 && 
					!SPY_LOCATION( pSoldier->bAssignment ) &&
					pSoldier->bAssignment != IN_TRANSIT && 
					pSoldier->bAssignment != ASSIGNMENT_POW && 
					!(pSoldier->flags.uiStatusFlags & (SOLDIER_DRIVER | SOLDIER_PASSENGER)) )
				{
					// This is bad!
					swprintf( sString, L"Soldier Data Error: Soldier %d is alive but has a zero group ID.", cnt );
					fProblemDetected = TRUE;
				}
				else if ( (pSoldier->ubGroupID != 0) && (GetGroup( pSoldier->ubGroupID ) == NULL) )
				{
					// This is bad!
					swprintf( sString, L"Soldier Data Error: Soldier %d has an invalid group ID of %d.", cnt, pSoldier->ubGroupID );
					fProblemDetected = TRUE;
				}
			}
			//else
			{
				//if ( pSoldier->ubGroupID != 0 && (pSoldier->flags.uiStatusFlags & SOLDIER_DEAD) )
				{
					// Dead guys should have 0 group IDs
					//swprintf( sString, L"GroupID Error: Soldier %d is dead but has a non-zero group ID.", cnt );
					//fProblemDetected = TRUE;
				}
			}

			// check for invalid sector data
			if ( (pSoldier->bAssignment != IN_TRANSIT) &&
				 ((pSoldier->sSectorX <= 0) || (pSoldier->sSectorX >= 17) ||
				 (pSoldier->sSectorY <= 0) || (pSoldier->sSectorY >= 17) ||
				 (pSoldier->bSectorZ  < 0) || (pSoldier->bSectorZ > (SPY_LOCATION( pSoldier->bAssignment ) ? 13 : 3) ) ) )
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
	ValidatePlayersAreInOneGroupOnly( );
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
	SOLDIERTYPE *pSoldier = NULL;
	INT32 cnt;

	//this is silly, but left over from when pSoldierToSet was passed in as a parameter
	if ( this == NULL )
	{
		return;
	}

	// Loop through ALL teams...
	cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[LAST_TEAM].bLastID; cnt++, pSoldier++ )
	{
		if ( pSoldier->bActive )
		{
			pSoldier->flags.uiStatusFlags &= ~SOLDIER_UNDERAICONTROL;
		}
	}

	this->flags.uiStatusFlags |= SOLDIER_UNDERAICONTROL;
}

void HandlePlayerTogglingLightEffects( BOOLEAN fToggleValue )
{
	if ( fToggleValue )
	{
		//Toggle light status
		if ( gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] )
		{
			gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] = FALSE;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_MERC_CASTS_LIGHT_OFF] );
		}
		else
		{
			gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] = TRUE;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_MERC_CASTS_LIGHT_ON] );
		}
	}

	//Update all the mercs in the sector
	EnableDisableSoldierLightEffects( gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT] );

	SetRenderFlags( RENDER_FLAG_FULL );
}


void EnableDisableSoldierLightEffects( BOOLEAN fEnableLights )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 cnt;

	// Loop through player teams...
	cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; cnt++, pSoldier++ )
	{
		//if the soldier is in the sector
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE )
		{
			//if we are to enable the lights
			if ( fEnableLights )
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
	if ( pSoldier == NULL )
	{
		return;
	}

	if ( TileIsOutOfBounds( pSoldier->sGridNo ) )
	{
		return;
	}

	if ( gpWorldLevelData[pSoldier->sGridNo].pMercHead == NULL )
	{
		return;
	}

	//THe light level for the soldier
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubShadeLevel = 3;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubSumLights = 5;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubMaxLights = 5;
	gpWorldLevelData[pSoldier->sGridNo].pMercHead->ubNaturalShadeLevel = 5;
}

void BeginSoldierClimbWallUp( SOLDIERTYPE *pSoldier )
{
	INT8							bNewDirection;
	UINT16	ubWhoIsThere;

	if ( FindLowerLevelWall( pSoldier, pSoldier->sGridNo, pSoldier->ubDirection, &bNewDirection ) && (pSoldier->pathing.bLevel > 0) )
	{
		if ( EnoughPoints( pSoldier, GetAPsToJumpWall( pSoldier, TRUE ), 0, TRUE ) )
		{
			//Kaiden: Helps if we look where we are going before we try to climb on top of someone
			ubWhoIsThere = WhoIsThere2( NewGridNo( pSoldier->sGridNo, (UINT16)DirectionInc( bNewDirection ) ), 0 );
			if ( ubWhoIsThere != NOBODY && ubWhoIsThere != pSoldier->ubID )
			{
				return;
			}
			else
			{

				if ( pSoldier->bTeam == gbPlayerNum )
				{
					SetUIBusy( pSoldier->ubID );
				}

				pSoldier->sTempNewGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)DirectionInc( bNewDirection ) );

				bNewDirection = gTwoCDirection[bNewDirection];

				pSoldier->ubPendingDirection = bNewDirection;
				pSoldier->EVENT_InitNewSoldierAnim( JUMPDOWNWALL, 0, FALSE );

				pSoldier->InternalReceivingSoldierCancelServices( FALSE );
				pSoldier->InternalGivingSoldierCancelServices( FALSE );

			}
		}
	}

}
//------------------------------------------------------------------------------------------

void SOLDIERTYPE::BreakWindow(void)
{
	if (this->inv[HANDPOS].exists() &&
		this->inv[HANDPOS][0]->data.objectStatus >= USABLE &&
		(ItemIsCrowbar(this->inv[HANDPOS].usItem) &&	Item[this->inv[HANDPOS].usItem].usItemClass & (IC_PUNCH) ||
		Item[this->inv[HANDPOS].usItem].usItemClass & IC_GUN && ItemIsTwoHanded(this->inv[HANDPOS].usItem) && ItemIsMetal(this->inv[HANDPOS].usItem) ))
	{
		this->usAttackingWeapon = this->inv[HANDPOS].usItem;
		this->aiData.bAction = AI_ACTION_KNIFE_STAB;
		this->aiData.usActionData = this->sGridNo;
		this->aiData.ubPendingAction = NO_PENDING_ACTION;
		this->sTargetGridNo = this->sGridNo;
		this->bTargetLevel = this->pathing.bLevel;
		//this->sLastTarget		= sGridNo;	//dnl ch73 021013
		this->ubTargetID = NOBODY; //WhoIsThere2(this->sTargetGridNo, this->bTargetLevel);
		this->EVENT_InitNewSoldierAnim(CROWBAR_ATTACK, 0, FALSE);
		SetUIBusy(this->ubID);

		DeductPoints(this, GetAPsToBreakWindow(this, FALSE), BP_USE_CROWBAR);
	}
}

BOOLEAN SOLDIERTYPE::CanBreakWindow(void)
{
	if (this->stats.bLife >= OKLIFE &&
		!this->IsUnconscious() &&
		IS_MERC_BODY_TYPE(this) &&
		this->inv[HANDPOS].exists() &&
		this->inv[HANDPOS][0]->data.objectStatus >= USABLE &&
		(ItemIsCrowbar(this->inv[HANDPOS].usItem) &&	Item[this->inv[HANDPOS].usItem].usItemClass & (IC_PUNCH) ||
		Item[this->inv[HANDPOS].usItem].usItemClass & IC_GUN && ItemIsTwoHanded(this->inv[HANDPOS].usItem) && ItemIsMetal(this->inv[HANDPOS].usItem) ))
	{
		//INT32 sWindowGridNo = this->sTargetGridNo;
		INT32 sWindowGridNo = this->sGridNo;
		if (this->ubDirection == NORTH || this->ubDirection == WEST)
			sWindowGridNo = NewGridNo(this->sGridNo, (UINT16)DirectionInc((UINT8)this->ubDirection));

		// is there really an intact window that we jump through?
		if (IsJumpableWindowPresentAtGridNo(sWindowGridNo, this->ubDirection, TRUE) && !IsJumpableWindowPresentAtGridNo(sWindowGridNo, this->ubDirection, FALSE))
		{
			STRUCTURE * pStructure = FindStructure(sWindowGridNo, STRUCTURE_WALLNWINDOW);
			if (pStructure && !(pStructure->fFlags & STRUCTURE_OPEN))
			{
				// intact window found
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOLEAN SOLDIERTYPE::CanStartDrag(void)
{
	if (!this->IsDragging(false) && this->CanDragInPrinciple())
	{
		INT32 sNewGridNo = NewGridNo(this->sGridNo, DirectionInc(this->ubDirection));

		if (!TileIsOutOfBounds(sNewGridNo) && sNewGridNo != this->sGridNo)
		{
			// soldiers
			for (UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[CIV_TEAM].bLastID; ++cnt)
			{
				if (cnt != this->ubID &&
					MercPtrs[cnt] &&
					MercPtrs[cnt]->sGridNo == sNewGridNo &&
					this->CanDragPerson(cnt))
				{
					return TRUE;
				}
			}

			// corpses
			ROTTING_CORPSE* pCorpse;
			for (INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt)
			{
				pCorpse = &(gRottingCorpse[cnt]);

				if (pCorpse &&
					pCorpse->fActivated &&
					pCorpse->def.bLevel == this->pathing.bLevel &&
					sNewGridNo == pCorpse->def.sGridNo &&
					this->CanDragCorpse(pCorpse->iID))
				{
					return TRUE;
				}
			}

			// gridno
			UINT32 tiletype;
			UINT16 structurenumber;
			UINT8 hitpoints;
			UINT8 decalflag;

			if (this->CanDragStructure(sNewGridNo) &&
				IsDragStructurePresent(sNewGridNo, this->pathing.bLevel, tiletype, structurenumber, hitpoints, decalflag))
			{
				int xmlentry;
				GetDragStructureXmlEntry(tiletype, structurenumber, xmlentry);

				if (xmlentry >= 0)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void SOLDIERTYPE::StartDrag(void)
{
	if (this->CanDragInPrinciple())
	{
		if (gAnimControl[this->usAnimState].ubEndHeight != ANIM_CROUCH)
		{
			HandleStanceChangeFromUIKeys(ANIM_CROUCH);
		}

		INT32 sNewGridNo = NewGridNo(this->sGridNo, DirectionInc(this->ubDirection));

		if (!TileIsOutOfBounds(sNewGridNo) && sNewGridNo != this->sGridNo)
		{
			// soldiers
			for (UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[CIV_TEAM].bLastID; ++cnt)
			{
				if (cnt != this->ubID &&
					MercPtrs[cnt] &&
					MercPtrs[cnt]->sGridNo == sNewGridNo &&
					this->CanDragPerson(cnt))
				{
					SetDragOrderPerson(cnt);
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
			}

			// corpses
			ROTTING_CORPSE* pCorpse;
			for (INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt)
			{
				pCorpse = &(gRottingCorpse[cnt]);

				if (pCorpse &&
					pCorpse->fActivated &&
					pCorpse->def.bLevel == this->pathing.bLevel &&
					sNewGridNo == pCorpse->def.sGridNo &&
					this->CanDragCorpse(pCorpse->iID))
				{
					SetDragOrderCorpse(pCorpse->iID);
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
			}

			// gridno
			UINT32 tiletype;
			UINT16 structurenumber;
			UINT8 hitpoints;
			UINT8 decalflag;

			if (this->CanDragStructure(sNewGridNo) &&
				IsDragStructurePresent(sNewGridNo, this->pathing.bLevel, tiletype, structurenumber, hitpoints, decalflag))
			{
				int xmlentry;
				GetDragStructureXmlEntry(tiletype, structurenumber, xmlentry);

				if (xmlentry >= 0)
				{
					SetDragOrderStructure(sNewGridNo);
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
			}
		}
	}
}

BOOLEAN DoesSoldierWearGasMask( SOLDIERTYPE *pSoldier )//dnl ch40 200909
{
	INT8 bPosOfMask = FindGasMask( pSoldier );

	if ( (bPosOfMask == HEAD1POS || bPosOfMask == HEAD2POS) && pSoldier->inv[bPosOfMask][0]->data.objectStatus >= USABLE )
		return(TRUE);
	return(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - added following functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN HAS_SKILL_TRAIT( SOLDIERTYPE * pSoldier, UINT8 uiSkillTraitNumber )
{
	if ( pSoldier == NULL )
		return FALSE;

	// Flugente: compatibility with skills
	if ( uiSkillTraitNumber == INTEL || uiSkillTraitNumber == DISGUISE || uiSkillTraitNumber == VARIOUSSKILLS )
		return TRUE;

	// sevenfm: add Autobandage option to skills menu
	if (uiSkillTraitNumber == AUTOBANDAGESKILLS)
	{
		return CheckAutoBandage();
	}

	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	// check old/new traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// exception for special merc
		//if ( gSkillTraitValues.fAllowSpecialMercTraitsException && pSoldier->ubProfile == gSkillTraitValues.ubSpecialMercID)
		//{
		//	bMaxTraits++;
		//	bMaxMajorTraits++;
		//}

		for ( INT8 bCnt = 0; bCnt < min( 30, bMaxTraits ); ++bCnt )
		{
			if ( pSoldier->stats.ubSkillTraits[bCnt] == uiSkillTraitNumber )
				return(TRUE);

			if ( MajorTrait( pSoldier->stats.ubSkillTraits[bCnt] ) )
				++bNumMajorTraitsCounted;

			// if we exceeded the allowed number of major traits, ignore the rest of them
			if ( bNumMajorTraitsCounted > min( 20, bMaxMajorTraits ) )
				break;
		}
	}
	else
	{
		if ( pSoldier->stats.ubSkillTraits[0] == uiSkillTraitNumber )
			return(TRUE);

		if ( pSoldier->stats.ubSkillTraits[1] == uiSkillTraitNumber )
			return(TRUE);
	}

	return(FALSE);
}

INT8 NUM_SKILL_TRAITS( SOLDIERTYPE * pSoldier, UINT8 uiSkillTraitNumber )
{
	if ( pSoldier == NULL )
		return(0);

	INT8 bNumberOfTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	// check old/new traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// exception for special merc
		//if ( gSkillTraitValues.fAllowSpecialMercTraitsException && pSoldier->ubProfile == gSkillTraitValues.ubSpecialMercID)
		//{
		//	bMaxTraits++;
		//	bMaxMajorTraits++;
		//}

		for ( INT8 bCnt = 0; bCnt < min( 30, bMaxTraits ); ++bCnt )
		{
			if ( pSoldier->stats.ubSkillTraits[bCnt] == uiSkillTraitNumber )
				++bNumberOfTraits;
				
			if ( MajorTrait( pSoldier->stats.ubSkillTraits[bCnt] ) )
				++bNumMajorTraitsCounted;

			// if we exceeded the allowed number of major traits, ignore the rest of them
			if ( bNumMajorTraitsCounted > min( 20, bMaxMajorTraits ) )
				break;
		}

		// cannot have more than one same minor trait
		if ( !TwoStagedTrait( uiSkillTraitNumber ) )
			return (min( 1, bNumberOfTraits ));
		
		return (min( 2, bNumberOfTraits ));
	}
	else
	{
		if ( pSoldier->stats.ubSkillTraits[0] == uiSkillTraitNumber )
			++bNumberOfTraits;

		if ( pSoldier->stats.ubSkillTraits[1] == uiSkillTraitNumber )
			++bNumberOfTraits;

		// Electronics, Ambidextrous and Camouflaged can only be of one grade
		if ( uiSkillTraitNumber == ELECTRONICS_OT ||
			 uiSkillTraitNumber == AMBIDEXT_OT ||
			 uiSkillTraitNumber == CAMOUFLAGED_OT )
			 return (min( 1, bNumberOfTraits ));

		return (bNumberOfTraits);
	}
}

UINT8 GetSquadleadersCountInVicinity( SOLDIERTYPE * pSoldier, BOOLEAN fWithHigherLevel, BOOLEAN fDontCheckDistance )
{
	UINT16 cnt = 0;
	UINT8 ubNumberSL = 0;

	// loop through all soldiers around
	for ( cnt = gTacticalStatus.Team[pSoldier->bTeam].bFirstID; cnt <= gTacticalStatus.Team[pSoldier->bTeam].bLastID; cnt++ )
	{
		// Get active conscious soldier
		if ( MercPtrs[cnt] != pSoldier && MercPtrs[cnt]->bActive && //MercPtrs[ cnt ]->aiData.bShock < 20 &&
			 MercPtrs[cnt]->stats.bLife >= OKLIFE && HAS_SKILL_TRAIT( MercPtrs[cnt], SQUADLEADER_NT ) )
		{
			// check if within distance
			// if both have extended ear, the distance is bigger and they don't need to sea each other 
			// note that enemy always get the bonus if within distance, regardless of extended ears
			// Flugente: moved around arguments for speed reason
			if ( fDontCheckDistance ||
				 (PythSpacesAway( pSoldier->sGridNo, MercPtrs[cnt]->sGridNo ) <= gSkillTraitValues.usSLRadiusNormal) ||
				 //((SoldierToVirtualSoldierLineOfSightTest( MercPtrs[ cnt ], pSoldier->sGridNo, pSoldier->pathing.bLevel, ANIM_STAND, TRUE, CALC_FROM_ALL_DIRS ) || 
				 //SoldierToVirtualSoldierLineOfSightTest( pSoldier, MercPtrs[ cnt ]->sGridNo, MercPtrs[ cnt ]->pathing.bLevel, ANIM_STAND, TRUE, CALC_FROM_ALL_DIRS ) ) && 
				 ((pSoldier->bTeam == ENEMY_TEAM || (HasExtendedEarOn( pSoldier ) && HasExtendedEarOn( MercPtrs[cnt] ))) && PythSpacesAway( pSoldier->sGridNo, MercPtrs[cnt]->sGridNo ) <= gSkillTraitValues.usSLRadiusExtendedEar)
				 )
			{
				// If checking for higher level SL
				// also count in already aquired level increses from other SLs
				if ( fWithHigherLevel )
				{
					if ( MercPtrs[cnt]->stats.bExpLevel > (pSoldier->stats.bExpLevel + (ubNumberSL*gSkillTraitValues.ubSLEffectiveLevelInRadius)) )
						ubNumberSL += min( (max( 0, (MercPtrs[cnt]->stats.bExpLevel - (pSoldier->stats.bExpLevel + (ubNumberSL*gSkillTraitValues.ubSLEffectiveLevelInRadius))) )), (NUM_SKILL_TRAITS( MercPtrs[cnt], SQUADLEADER_NT )) );
				}
				else
				{
					ubNumberSL += NUM_SKILL_TRAITS( MercPtrs[cnt], SQUADLEADER_NT );
				}

				if ( ubNumberSL >= gSkillTraitValues.ubSLMaxBonuses )
					break;
			}
		}
	}

	// special loop for militia - they can get a bonus from our mercs
	if ( pSoldier->bTeam == MILITIA_TEAM && ubNumberSL < gSkillTraitValues.ubSLMaxBonuses )
	{
		for ( cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt )
		{
			// Get active conscious soldier
			if ( MercPtrs[cnt] != pSoldier && MercPtrs[cnt]->bActive && //MercPtrs[ cnt ]->aiData.bShock < 20 &&
				 MercPtrs[cnt]->stats.bLife >= OKLIFE && HAS_SKILL_TRAIT( MercPtrs[cnt], SQUADLEADER_NT ) )
			{
				// check if within distance
				// Flugente: moved around arguments for speed reason
				if ( fDontCheckDistance ||
					 (PythSpacesAway( pSoldier->sGridNo, MercPtrs[cnt]->sGridNo ) <= gSkillTraitValues.usSLRadiusNormal) ||
					 //((SoldierToVirtualSoldierLineOfSightTest( MercPtrs[ cnt ], pSoldier->sGridNo, pSoldier->pathing.bLevel, ANIM_STAND, TRUE, CALC_FROM_ALL_DIRS ) || 
					 //SoldierToVirtualSoldierLineOfSightTest( pSoldier, MercPtrs[ cnt ]->sGridNo, MercPtrs[ cnt ]->pathing.bLevel, ANIM_STAND, TRUE, CALC_FROM_ALL_DIRS ) ) && 
					 ((HasExtendedEarOn( MercPtrs[cnt] ) && PythSpacesAway( pSoldier->sGridNo, MercPtrs[cnt]->sGridNo ) <= gSkillTraitValues.usSLRadiusExtendedEar))
					 )
				{
					// If checking for higher level SL
					// also count in already aquired level increses from other SLs
					if ( fWithHigherLevel )
					{
						if ( MercPtrs[cnt]->stats.bExpLevel > (pSoldier->stats.bExpLevel + (ubNumberSL*gSkillTraitValues.ubSLEffectiveLevelInRadius)) )
							ubNumberSL += min( (max( 0, (MercPtrs[cnt]->stats.bExpLevel - (pSoldier->stats.bExpLevel + (ubNumberSL*gSkillTraitValues.ubSLEffectiveLevelInRadius))) )), (NUM_SKILL_TRAITS( MercPtrs[cnt], SQUADLEADER_NT )) );
					}
					else
					{
						ubNumberSL += NUM_SKILL_TRAITS( MercPtrs[cnt], SQUADLEADER_NT );
					}

					if ( ubNumberSL >= gSkillTraitValues.ubSLMaxBonuses )
						break;
				}
			}
		}
	}

	// 3 bonuses are a max by default
	return(min( gSkillTraitValues.ubSLMaxBonuses, ubNumberSL ));
}

UINT16 NumberOfDamagedStats( SOLDIERTYPE * pSoldier )
{
	UINT16 ubTotalStatsDamaged = 0;
	for ( UINT8 cnt = 0; cnt < NUM_DAMAGABLE_STATS; ++cnt )
	{
		if ( pSoldier->ubCriticalStatDamage[cnt] > 0 )
			ubTotalStatsDamaged += pSoldier->ubCriticalStatDamage[cnt];
	}

	// Flugente: stats can also be damaged by lack of food
	ubTotalStatsDamaged += pSoldier->usStarveDamageHealth;
	ubTotalStatsDamaged += pSoldier->usStarveDamageStrength;

	return(ubTotalStatsDamaged);
}

UINT8 RegainDamagedStats( SOLDIERTYPE * pSoldier, UINT16 usAmountRegainedHundredths )
{
	UINT16 usStatIncreasement;
	//BOOLEAN fAnyStatToBeRepaired = FALSE;
	UINT8 cnt, ubAmountRegained;
	STR16 sStat = L"";

	UINT8 bStatsReturned = 0;

	// First determine how much we can return
	ubAmountRegained = (usAmountRegainedHundredths / 100); //transfer to whole numbers
	usAmountRegainedHundredths %= 100; // keep rest
	if ( usAmountRegainedHundredths > 0 ) // if some reamins, solve it as a chance
	{
		if ( Chance( usAmountRegainedHundredths ) ) // if rolled true, add one point
		{
			ubAmountRegained += 1;
			usAmountRegainedHundredths = 0;
		}
		else // otherwise ignore the rest
		{
			usAmountRegainedHundredths = 0;
		}
	}
	// return zero if we are not able to heal anything
	if ( ubAmountRegained <= 0 )
		return(0);

	// Second, run through all damagable stats
	for ( cnt = 0; cnt < NUM_DAMAGABLE_STATS; ++cnt )
	{
		// if we have a damaged stat here
		if ( pSoldier->ubCriticalStatDamage[cnt] > 0 )
		{
			if ( ubAmountRegained >= pSoldier->ubCriticalStatDamage[cnt] )
			{
				// if the amount we can return is bigger than what we need, keep the rest, for other stats
				usStatIncreasement = pSoldier->ubCriticalStatDamage[cnt];
				ubAmountRegained = max( 0, (ubAmountRegained - usStatIncreasement) );
				pSoldier->ubCriticalStatDamage[cnt] = 0;
			}
			else
			{
				// if not having full amount, heal what we can
				usStatIncreasement = ubAmountRegained;
				ubAmountRegained = 0;
				pSoldier->ubCriticalStatDamage[cnt] = max( 0, (pSoldier->ubCriticalStatDamage[cnt] - usStatIncreasement) );
			}
			// so we can start regaining the stats
			if ( usStatIncreasement > 0 )
			{
				bStatsReturned += usStatIncreasement; // keep value for feedback

				switch ( cnt ) // look on the stat
				{
					// actually we only test Health, Dexterity, Agility, Strength and Wisdom now, 
					// as there are no ways to lost other stats in the current code
				case DAMAGED_STAT_HEALTH:
					sStat = sStatGainStrings[0]; // set string
					pSoldier->stats.bLifeMax += usStatIncreasement;
					pSoldier->stats.bLife += usStatIncreasement;
					pSoldier->iHealableInjury -= (usStatIncreasement * 100); // don't forget the healable injury
					if ( pSoldier->iHealableInjury < 0 )
						pSoldier->iHealableInjury = 0;
					if ( pSoldier->stats.bLifeMax >= 100 || pSoldier->stats.bLife >= 100 ) // repair if going too far
					{
						pSoldier->stats.bLifeMax = 100;
						pSoldier->stats.bLife = 100;
						pSoldier->iHealableInjury = 0;
						pSoldier->ubCriticalStatDamage[cnt] = 0;
					}
					gMercProfiles[pSoldier->ubProfile].bLifeMax = pSoldier->stats.bLifeMax; // update profile
					break;
				case DAMAGED_STAT_DEXTERITY:
					sStat = sStatGainStrings[2]; // set string
					pSoldier->stats.bDexterity += usStatIncreasement;
					if ( pSoldier->stats.bDexterity >= 100 ) // repair if going too far
					{
						pSoldier->stats.bDexterity = 100;
						pSoldier->ubCriticalStatDamage[cnt] = 0;
					}
					gMercProfiles[pSoldier->ubProfile].bDexterity = pSoldier->stats.bDexterity; // update profile
					break;
				case DAMAGED_STAT_AGILITY:
					sStat = sStatGainStrings[1]; // set string
					pSoldier->stats.bAgility += usStatIncreasement;
					if ( pSoldier->stats.bAgility >= 100 ) // repair if going too far
					{
						pSoldier->stats.bAgility = 100;
						pSoldier->ubCriticalStatDamage[cnt] = 0;
					}
					gMercProfiles[pSoldier->ubProfile].bAgility = pSoldier->stats.bAgility; // update profile
					break;
				case DAMAGED_STAT_STRENGTH:
					sStat = sStatGainStrings[9]; // set string
					pSoldier->stats.bStrength += usStatIncreasement;
					if ( pSoldier->stats.bStrength >= 100 ) // repair if going too far
					{
						pSoldier->stats.bStrength = 100;
						pSoldier->ubCriticalStatDamage[cnt] = 0;
					}
					gMercProfiles[pSoldier->ubProfile].bStrength = pSoldier->stats.bStrength; // update profile
					break;
				case DAMAGED_STAT_WISDOM:
					sStat = sStatGainStrings[3]; // set string
					pSoldier->stats.bWisdom += usStatIncreasement;
					if ( pSoldier->stats.bWisdom >= 100 ) // repair if going too far
					{
						pSoldier->stats.bWisdom = 100;
						pSoldier->ubCriticalStatDamage[cnt] = 0;
					}
					gMercProfiles[pSoldier->ubProfile].bWisdom = pSoldier->stats.bWisdom; // update profile
					break;
				}
				// Throw a message if healed anything
				if ( gSkillTraitValues.fDORepStShouldThrowMessage && pSoldier->bTeam != ENEMY_TEAM )
				{
					if ( usStatIncreasement == 1 )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_ONE_POINTS_OF_STAT], pSoldier->GetName( ), sStat );
					else
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_X_POINTS_OF_STATS], pSoldier->GetName( ), usStatIncreasement, sStat );
				}
			}
			//if( pSoldier->ubCriticalStatDamage[cnt] > 0 )
			//	fAnyStatToBeRepaired = TRUE;

		}
	}

	// Flugente: Third, heal damage from starvation if possible
	if ( !UsingFoodSystem() || ( ubAmountRegained > 0 && pSoldier->bFoodLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold) )
	{
		// if we have a damaged stat here
		if ( pSoldier->usStarveDamageHealth > 0 )
		{
			if ( ubAmountRegained >= pSoldier->usStarveDamageHealth )
			{
				// if the amount we can return is bigger than what we need, keep the rest, for other stats
				usStatIncreasement = pSoldier->usStarveDamageHealth;
				ubAmountRegained = max( 0, (ubAmountRegained - usStatIncreasement) );
				pSoldier->usStarveDamageHealth = 0;
			}
			else
			{
				// if not having full amount, heal what we can
				usStatIncreasement = ubAmountRegained;
				ubAmountRegained = 0;
				pSoldier->usStarveDamageHealth = max( 0, (pSoldier->usStarveDamageHealth - usStatIncreasement) );
			}

			// so we can start regaining the stats
			if ( usStatIncreasement > 0 )
			{
				bStatsReturned += usStatIncreasement; // keep value for feedback

				sStat = sStatGainStrings[0]; // set string
				pSoldier->stats.bLifeMax += usStatIncreasement;
				pSoldier->stats.bLife += usStatIncreasement;
				pSoldier->iHealableInjury -= (usStatIncreasement * 100); // don't forget the healable injury
				if ( pSoldier->iHealableInjury < 0 )
					pSoldier->iHealableInjury = 0;
				if ( pSoldier->stats.bLifeMax >= 100 || pSoldier->stats.bLife >= 100 ) // repair if going too far
				{
					pSoldier->stats.bLifeMax = 100;
					pSoldier->stats.bLife = 100;
					pSoldier->iHealableInjury = 0;
					pSoldier->usStarveDamageHealth = 0;
				}
				gMercProfiles[pSoldier->ubProfile].bLifeMax = pSoldier->stats.bLifeMax; // update profile

				// Throw a message if healed anything
				if ( gSkillTraitValues.fDORepStShouldThrowMessage && pSoldier->bTeam != ENEMY_TEAM )
				{
					if ( usStatIncreasement == 1 )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_ONE_POINTS_OF_STAT], pSoldier->GetName( ), sStat );
					else
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_X_POINTS_OF_STATS], pSoldier->GetName( ), usStatIncreasement, sStat );
				}
			}
		}

		// if we have a damaged stat here
		if ( pSoldier->usStarveDamageStrength > 0 )
		{
			if ( ubAmountRegained >= pSoldier->usStarveDamageStrength )
			{
				// if the amount we can return is bigger than what we need, keep the rest, for other stats
				usStatIncreasement = pSoldier->usStarveDamageStrength;
				ubAmountRegained = max( 0, (ubAmountRegained - usStatIncreasement) );
				pSoldier->usStarveDamageStrength = 0;
			}
			else
			{
				// if not having full amount, heal what we can
				usStatIncreasement = ubAmountRegained;
				ubAmountRegained = 0;
				pSoldier->usStarveDamageStrength = max( 0, (pSoldier->usStarveDamageStrength - usStatIncreasement) );
			}

			// so we can start regaining the stats
			if ( usStatIncreasement > 0 )
			{
				bStatsReturned += usStatIncreasement; // keep value for feedback

				sStat = sStatGainStrings[9]; // set string
				pSoldier->stats.bStrength += usStatIncreasement;
				if ( pSoldier->stats.bStrength >= 100 ) // repair if going too far
				{
					pSoldier->stats.bStrength = 100;
					pSoldier->usStarveDamageStrength = 0;
				}
				gMercProfiles[pSoldier->ubProfile].bStrength = pSoldier->stats.bStrength; // update profile

				// Throw a message if healed anything
				if ( gSkillTraitValues.fDORepStShouldThrowMessage && pSoldier->bTeam != ENEMY_TEAM )
				{
					if ( usStatIncreasement == 1 )
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_ONE_POINTS_OF_STAT], pSoldier->GetName( ), sStat );
					else
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_REGAINED_X_POINTS_OF_STATS], pSoldier->GetName( ), usStatIncreasement, sStat );
				}
			}
		}
	}

	// Done, return what we healed
	return(bStatsReturned);
}

////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - Improved Interrupt System
/////////////////////////////////////////
BOOLEAN ResolvePendingInterrupt( SOLDIERTYPE * pSoldier, UINT8 ubInterruptType )
{
	// real time or not in combat? disable and clear
	if ( !(gTacticalStatus.uiFlags & TURNBASED) ||
		 !(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		gTacticalStatus.ubInterruptPending = DISABLED_INTERRUPT;
		ClearIntList( );
		return(FALSE);
	}

	// invalid guy
	if ( pSoldier == NULL )
	{
		//ClearIntList();
		return(FALSE);
	}

	// can't be interrupted if it's not our turn at all
	if ( gTacticalStatus.ubCurrentTeam != pSoldier->bTeam )
	{
		return(FALSE);
	}

	// no interrupt called or not gonna trigger it now
	if ( gTacticalStatus.ubInterruptPending == DISABLED_INTERRUPT ||
		 gTacticalStatus.ubInterruptPending == UNTRIGGERED_INTERRUPT )
	{
		return(FALSE);
	}

	// if the interrupt called match the type we are trying to resolve..
	if ( gTacticalStatus.ubInterruptPending == ubInterruptType || ubInterruptType == INSTANT_INTERRUPT )
	{
		/////////////////////////////
		// Gather all interrupters //
		/////////////////////////////
		SOLDIERTYPE *pInterrupter;
		UINT8 ubInterruptersFound = 0;
		UINT16 ubaInterruptersList[64];
		UINT16 uCnt = 0, uiReactionTime;
		INT16 iInjuryPenalty;

		for ( uCnt = 0; uCnt < MAX_NUM_SOLDIERS; uCnt++ )
		{
			// first find all guys who can see us
			pInterrupter = MercPtrs[uCnt];
			if ( pInterrupter == NULL )
				continue;			// not valid
			if (pInterrupter->stats.bLife < OKLIFE || pInterrupter->bCollapsed || !pInterrupter->bActive || !pInterrupter->bInSector || pInterrupter->bActionPoints < 4)
				continue;			// not active
			if (pInterrupter->bBreath < OKBREATH && pInterrupter->bTeam != OUR_TEAM)
				continue;			// BOB: prevent NPCs from getting interrupts when out of breath
			if ( pSoldier->bTeam == pInterrupter->bTeam )
				continue;			// same team
			if ( pSoldier->bSide == pInterrupter->bSide )
				continue;			// not enemy
			if ( CONSIDERED_NEUTRAL( pSoldier, pInterrupter ) )
				continue;			// neutral
			if ( CONSIDERED_NEUTRAL( pInterrupter, pSoldier ) )
				continue;			// neutral

			/////////////////////////////////////////////////////////////
			// Calculate Reaction Time (i.e. interrupt counter length) //
			/////////////////////////////////////////////////////////////

			// set base value ( interrupt per every X APs an enemy uses )
			// if not seen but just heard... we interrupt only if they attack us (or if they are very close) in that case
			if ( (pInterrupter->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY) || (pInterrupter->aiData.bOppList[pSoldier->ubID] == HEARD_THIS_TURN && (ubInterruptType == AFTERSHOT_INTERRUPT || ubInterruptType == AFTERACTION_INTERRUPT || PythSpacesAway( pInterrupter->sGridNo, pSoldier->sGridNo ) < 3)) )
			{
				uiReactionTime = gGameExternalOptions.ubBasicReactionTimeLengthIIS;
			}
			else
			{
				// not seen or not heard anything worth interrupting
				continue;
			}
			uiReactionTime = uiReactionTime * 10; // x10 ... we will divide by 10 after all adjustments done
			// adjust based on Agility
			if ( pInterrupter->stats.bAgility >= 80 )
			{
				uiReactionTime = (uiReactionTime * (100 - (2 * (pInterrupter->stats.bAgility - 80))) / 100);
			}
			else if ( pInterrupter->stats.bAgility < 80 && pInterrupter->stats.bAgility > 50 )
			{
				uiReactionTime = (uiReactionTime * (100 + (2 * (80 - pInterrupter->stats.bAgility))) / 100);
			}
			else
			{
				uiReactionTime = (uiReactionTime * 8 / 5);
			}
			// adjust based on APs left
			// at full possible APs no adjustement (100% applies), +1% length per every 2% of APs down from full
			uiReactionTime = (uiReactionTime * (100 + (50 - (50 * pInterrupter->bActionPoints / pInterrupter->CalcActionPoints( )))) / 100);
			// adjust based on injuries
			if ( pInterrupter->stats.bLife < pInterrupter->stats.bLifeMax )
			{
				// OK, this looks a bit complicated..
				// our HP lost minus half of the bandaged part gives us 2% longer reaction time per 1% of our health down from full health
				// this penalty is however slightly reduced by our experience level
				iInjuryPenalty = (200 * (pInterrupter->stats.bLifeMax - pInterrupter->stats.bLife + ((pInterrupter->stats.bLifeMax - pInterrupter->stats.bLife - pInterrupter->bBleeding) / 2))) / (pInterrupter->stats.bLifeMax);
				uiReactionTime = (uiReactionTime * (100 + iInjuryPenalty * (100 - (3 * EffectiveExpLevel( pInterrupter ))) / 100) / 100);
			}

			// adjust by breath down
			if ( pSoldier->bBreath < 100 )
			{
				// +1% per 2 points of breath down
				uiReactionTime = (uiReactionTime * (100 + ((100 - pSoldier->bBreath) / 2)) / 100);
			}

			// adjust for getting aid, being in gas or being in shock
			if ( pInterrupter->flags.uiStatusFlags & SOLDIER_GASSED )
				uiReactionTime = (uiReactionTime * (100 + AIM_PENALTY_GASSED) / 100);

			if ( pInterrupter->ubServiceCount > 0 )
				uiReactionTime = (uiReactionTime * (100 + AIM_PENALTY_GETTINGAID) / 100);

			if ( pInterrupter->aiData.bShock )
				uiReactionTime = (uiReactionTime * (100 + (pInterrupter->aiData.bShock * 20)) / 100); // this is severe, 20% per point

			// Phlegmatic characters has slightly longer reaction time			
			if ( DoesMercHavePersonality( pSoldier, CHAR_TRAIT_PHLEGMATIC ) )
			{
				uiReactionTime = ((uiReactionTime * 110) / 100);
			}

			// finally divide back by 10 to get the needed result (round properly)
			uiReactionTime = ((uiReactionTime + 5) / 10);

			/////////////////////////////////////////////
			// Check if we reached reaction time value //
			/////////////////////////////////////////////

			// if we have hit the needed amount, the actual interrupt occurs for the observer
			if ( pInterrupter->aiData.ubInterruptCounter[pSoldier->ubID] >= uiReactionTime )
			{
				///////////////////////////
				// Success! Add to list! //
				///////////////////////////

				// the soldier to be interrupted is added to the list (once only)
				if ( ubInterruptersFound == 0 )
				{
					AddToIntList( pSoldier->ubID, FALSE, TRUE );
				}
				ubaInterruptersList[ubInterruptersFound] = pInterrupter->ubID;
				ubInterruptersFound++;

				// add the observer who got the interrupt
				AddToIntList( pInterrupter->ubID, TRUE, TRUE );
				// reset the counter
				pInterrupter->aiData.ubInterruptCounter[pSoldier->ubID] = 0;
			}
		}
		if ( ubInterruptersFound > 0 )
		{
			////////////////////////////////////////////////
			// Check for possible "Collective Interrupts" //
			////////////////////////////////////////////////
			if ( gGameExternalOptions.fAllowCollectiveInterrupts )
			{
				SOLDIERTYPE *pTeammate;
				UINT16 uCnt2 = 0, usColIntChance = 0;
				UINT8 ubOriginalInterruptersCount = ubInterruptersFound, uCnt3 = 0;
				BOOLEAN fAlreadyIn;

				for ( uCnt = 0; uCnt < ubOriginalInterruptersCount; uCnt++ )
				{
					pInterrupter = MercPtrs[ubaInterruptersList[uCnt]];

					uCnt2 = gTacticalStatus.Team[pInterrupter->bTeam].bFirstID;
					for ( pTeammate = MercPtrs[uCnt2]; uCnt2 <= gTacticalStatus.Team[pInterrupter->bTeam].bLastID; uCnt2++, pTeammate++ )
					{
						if ( pTeammate == NULL )
							continue;			// not valid
						if ( pTeammate->bTeam != pInterrupter->bTeam )
							continue;			// little paranoya check here
						if ( pTeammate->stats.bLife < OKLIFE || pTeammate->bCollapsed || !pTeammate->bActive || !pTeammate->bInSector || pTeammate->bActionPoints < 4 )
							continue;			// not active

						// check if we haven't been added to the list already
						fAlreadyIn = FALSE;
						for ( uCnt3 = 0; uCnt3 < ubInterruptersFound; uCnt3++ )
						{
							if ( pTeammate->ubID == ubaInterruptersList[uCnt3] )
							{
								fAlreadyIn = TRUE;
								break;
							}
						}
						// if we are close enough
						if ( !fAlreadyIn && PythSpacesAway( pInterrupter->sGridNo, pTeammate->sGridNo ) <= 5 )
						{
							// calculate the chance
							// we would have base chance 100% (if both have maxed stats)
							// 0-30% is determined by Leadership of the original interrupted - i.e. how well and if he can "inform" us
							// 0-20% is determined by his Experience Level
							// 0-20% is determined by our Experience Level - i.e how well can we realize that we must act
							// 0-20% is determined by our Agility - can our body react so swiftly at all
							// 0-10% is determined by our Wisdom - do we have enough mental agility as well?
							usColIntChance = 10 * (((pInterrupter->stats.bLeadership * 3) +
								(EffectiveExpLevel( pInterrupter ) * 20) +
								(EffectiveExpLevel( pTeammate ) * 20) +
								(pTeammate->stats.bAgility * 2) +
								(pTeammate->stats.bWisdom)) / 100);
							// add bonus per Squadleader trait of the original interrupter
							if ( HAS_SKILL_TRAIT( pInterrupter, SQUADLEADER_NT ) && gGameOptions.fNewTraitSystem )
							{
								usColIntChance += gSkillTraitValues.ubSLCollectiveInterruptsBonus * NUM_SKILL_TRAITS( pInterrupter, SQUADLEADER_NT );
							}
							if ( PreChance( usColIntChance ) )
							{
								ubaInterruptersList[ubInterruptersFound] = pTeammate->ubID;
								ubInterruptersFound++;
								// if he can react on collective interrupt, give it to him
								AddToIntList( pTeammate->ubID, TRUE, TRUE );
								// reset the counter for him
								pTeammate->aiData.ubInterruptCounter[pSoldier->ubID] = 0;
							}
						}
					}
				}
			}

			/////////////////////////////////////////////
			// OK, done, all interrupters added, SEND! //
			/////////////////////////////////////////////

			// remove AI control from the interrupted guy just in case may not be neccessary, but it's harmless anyway
			if ( (gTacticalStatus.ubCurrentTeam != pSoldier->bTeam) && !(gTacticalStatus.Team[gTacticalStatus.ubCurrentTeam].bHuman) )
			{
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_UNDERAICONTROL )
				{
					pSoldier->flags.uiStatusFlags &= (~SOLDIER_UNDERAICONTROL);
				}
			}
			// reset 
			gTacticalStatus.ubInterruptPending = DISABLED_INTERRUPT;
			// start interrupt
			DoneAddingToIntList( pSoldier, TRUE, 1 );

			return(TRUE);
		}
		else // no interrupters found, reset until next occasion
		{
			// reset 
			gTacticalStatus.ubInterruptPending = DISABLED_INTERRUPT;
		}
	}
	return(FALSE);
}

BOOLEAN AIDecideHipOrShoulderStance( SOLDIERTYPE * pSoldier, INT32 iGridNo )
{
	// TO DO: this should be much more sophisticated

	UINT16 usInHand = pSoldier->usAttackingWeapon;

	// not 2-handed or not standing 
	if ( gAnimControl[pSoldier->usAnimState].ubEndHeight != ANIM_STAND || !ItemIsTwoHanded(pSoldier->inv[HANDPOS].usItem) )
	{
		return FALSE;
	}
	// heavy gun only from hip if standing
	if ( Weapon[usInHand].HeavyGun )
	{
		return TRUE;
	}
	// we want to make an aimed shot
	if ( pSoldier->aiData.bAimTime > GetNumberAltFireAimLevels( pSoldier, iGridNo ) )
	{
		return FALSE;
	}

	INT8 bChanceHip = 0;

	if ( pSoldier->bDoBurst > 0 )
		bChanceHip += 25;
	if ( Weapon[usInHand].ubWeaponType == GUN_LMG )
		bChanceHip += 30;
	if ( Weapon[usInHand].ubWeaponType == GUN_SHOTGUN )
		bChanceHip += 15;

	// chance to hit with no aiming, add it to the chance to fire from hip
	if ( !TileIsOutOfBounds( iGridNo ) )
	{
		bChanceHip += CalcChanceToHitGun( pSoldier, iGridNo, 0, AIM_SHOT_TORSO );
	}

	if ( PreChance( bChanceHip ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOLEAN DecideAltAnimForBigMerc( SOLDIERTYPE * pSoldier )
{
	if ( pSoldier->ubBodyType != BIGMALE )
	{
		// WTF!
		return FALSE;
	}

	//always use the other anim for badass mercs
	if ( pSoldier->uiAnimSubFlags & SUB_ANIM_BIGGUYSHOOT2 )
	{
		return TRUE;
	}

	// if it is player controlled merc
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
	{
		// are we in combat?
		if ( gTacticalStatus.uiFlags & INCOMBAT )
		{
			// then only use it if morale is very high (we are definately winning)
			if ( pSoldier->aiData.bMorale > 95 )
			{
				return TRUE;
			}
		}
		else
		{
			// if not we use this with slightly above avarage morale
			if ( pSoldier->aiData.bMorale > 65 )
			{
				return TRUE;
			}
		}
	}
	// enemy guy
	else
	{
		//never use this for regular enemies, only elites with high morale and level can sometimes show this animation
		if ( (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE || pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA) &&
			 (pSoldier->aiData.bAIMorale >= MORALE_FEARLESS) && (pSoldier->stats.bExpLevel > 8) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOLEAN TwoStagedTrait( UINT8 uiSkillTraitNumber )
{
	if ( gGameOptions.fNewTraitSystem )
	{
		if ( uiSkillTraitNumber > 0 )
		{
			// covert ops is a major trait that is in a different location
			if ( uiSkillTraitNumber == COVERT_NT )
				return TRUE;

			// other traits below NUM_ORIGINAL_MAJOR_TRAITS are all major
			if ( uiSkillTraitNumber <= NUM_ORIGINAL_MAJOR_TRAITS )
				return TRUE;
		}
	}
	else
	{
		if ( uiSkillTraitNumber == IMP_SKILL_TRAITS__ELECTRONICS ||
			 uiSkillTraitNumber == IMP_SKILL_TRAITS__AMBIDEXTROUS ||
			 uiSkillTraitNumber == IMP_SKILL_TRAITS__CAMO )
			return(FALSE);

		return TRUE;
	}

	return FALSE;
}

// determine if this is a major trait (no longer all two-staged)
BOOLEAN MajorTrait( UINT8 uiSkillTraitNumber )
{
	if ( uiSkillTraitNumber > 0 )
	{
		// covert ops is a major trait that is in a different location
		if ( uiSkillTraitNumber == COVERT_NT )
			return TRUE;

		// other traits below NUM_ORIGINAL_MAJOR_TRAITS are all major
		if ( uiSkillTraitNumber <= NUM_ORIGINAL_MAJOR_TRAITS )
			return TRUE;
	}

	return FALSE;
}

BOOLEAN GetRadioOperatorSignal( UINT8 usOwner, INT32* psTargetGridNo )
{
	// get the 'real owner'
	if ( usOwner > 1 )
	{
		// a merc planted this - if he's a radio operator, use his gridno
		SOLDIERTYPE* pSoldier = MercPtrs[usOwner - 2];

		if ( pSoldier && pSoldier->CanUseRadio( FALSE ) && pSoldier->bActive && pSoldier->bInSector && (pSoldier->sSectorX == gWorldSectorX) && (pSoldier->sSectorY == gWorldSectorY) && (pSoldier->bSectorZ == gbWorldSectorZ) )
		{
			*psTargetGridNo = pSoldier->sGridNo;
			pSoldier->bSide;
			return TRUE;
		}
	}
	// check for the side that ordered this
	else
	{
		UINT8 bTeam = MILITIA_TEAM;
		if ( usOwner )
			bTeam = ENEMY_TEAM;

		SOLDIERTYPE* pSoldier = NULL;
		INT32 cnt = gTacticalStatus.Team[bTeam].bFirstID;
		INT32 lastid = gTacticalStatus.Team[bTeam].bLastID;
		for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
		{
			if ( pSoldier && pSoldier->CanUseRadio( FALSE ) && pSoldier->bActive && pSoldier->bInSector && (pSoldier->sSectorX == gWorldSectorX) && (pSoldier->sSectorY == gWorldSectorY) && (pSoldier->bSectorZ == gbWorldSectorZ) )
			{
				*psTargetGridNo = pSoldier->sGridNo;
				pSoldier->bSide;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOLEAN IsValidArtilleryOrderSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 bTeam )
{
	// is the sector valid?
	if ( bSectorZ > 0 || sSectorX < 1 || sSectorX >= MAP_WORLD_X - 1 || sSectorY < 1 || sSectorY >= MAP_WORLD_Y - 1 )
		return FALSE;

	UINT16 usEnemies = (UINT16)NumNonPlayerTeamMembersInSector( sSectorX, sSectorY, ENEMY_TEAM );
	UINT16 usMilitia = (UINT16)NumNonPlayerTeamMembersInSector( sSectorX, sSectorY, MILITIA_TEAM );
	UINT16 usMercs = (UINT16)PlayerMercsInSector( (UINT8)sSectorX, (UINT8)sSectorY, (UINT8)bSectorZ );

	SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR( sSectorX, sSectorY )]);

	// sector must be free of members of an opposing team
	if ( bTeam == ENEMY_TEAM )
	{
		if ( !usEnemies || usMilitia || usMercs )
			return FALSE;

		// there have to be enough guys here to fire at least one shot
		if ( usEnemies < gSkillTraitValues.usVOMortarCountDivisor )
			return FALSE;

		if ( (usEnemies * gSkillTraitValues.usVOMortarPointsAdmin) < (gSkillTraitValues.usVOMortarShellDivisor * (usEnemies / gSkillTraitValues.usVOMortarCountDivisor)) )
			return FALSE;

		// cannot fire if artillery was used recently
		if ( GetWorldTotalMin( ) < pSectorInfo->uiTimeAIArtillerywasOrdered + gSkillTraitValues.bVOArtillerySectorFrequency )
			return FALSE;
	}
	else if ( bTeam == MILITIA_TEAM )
	{
		if ( usEnemies || !usMilitia )
			return FALSE;

		// there have to be enough guys here to fire at least one shot
		if ( usMilitia < gSkillTraitValues.usVOMortarCountDivisor )
			return FALSE;

		if ( (usMilitia * gSkillTraitValues.usVOMortarPointsAdmin) < (gSkillTraitValues.usVOMortarShellDivisor * (usMilitia / gSkillTraitValues.usVOMortarCountDivisor)) )
			return FALSE;

		// cannot fire if artillery was used recently
		if ( GetWorldTotalMin( ) < pSectorInfo->uiTimeAIArtillerywasOrdered + gSkillTraitValues.bVOArtillerySectorFrequency )
			return FALSE;
	}
	else if ( bTeam == OUR_TEAM )
	{
		if ( usEnemies || !usMercs )
			return FALSE;

		// we can relay orders only if someone in the sector has a working radio set and a mortar
		BOOLEAN activeradio = FALSE;
		BOOLEAN mortarfound = FALSE;
		SOLDIERTYPE* pSoldier = NULL;
		INT32 cnt = gTacticalStatus.Team[bTeam].bFirstID;
		INT32 lastid = gTacticalStatus.Team[bTeam].bLastID;
		for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
		{
			// check if soldier exists in this sector, and is on duty
			if ( !pSoldier || !pSoldier->bActive || pSoldier->sSectorX != sSectorX || pSoldier->sSectorY != sSectorY || pSoldier->bSectorZ != bSectorZ || pSoldier->bAssignment > ON_DUTY )
				continue;

			if ( pSoldier->CanUseRadio( FALSE ) )
				activeradio = TRUE;

			if ( pSoldier->HasMortar( ) )
				mortarfound = TRUE;
		}

		if ( !activeradio || !mortarfound )
			return FALSE;
	}

	return TRUE;
}

BOOLEAN SectorJammed( )
{
	// check every soldier: are we jamming frequencies?
	SOLDIERTYPE* pSoldier = NULL;
	INT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID;
	INT32 lastid = MAX_NUM_SOLDIERS;
	for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
	{
		if ( pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ && pSoldier->stats.bLife > 0 && pSoldier->IsJamming( ) )
			return TRUE;
	}

	return FALSE;
}

BOOLEAN PlayerTeamIsScanning( )
{
	// check every soldier: are we jamming frequencies?
	SOLDIERTYPE* pSoldier = NULL;
	INT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID;
	INT32 lastid = gTacticalStatus.Team[OUR_TEAM].bLastID;
	for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
	{
		if ( pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ && pSoldier->stats.bLife > 0 && pSoldier->IsScanning( ) )
			return TRUE;
	}

	return FALSE;
}

// bonus for snipers firing at this location (we get this if there are spotters)
UINT16	GridNoSpotterCTHBonus( SOLDIERTYPE* pSniper, INT32 sGridNo, INT8 bTeam )
{
	UINT16 bestvalue = 0;

	SOLDIERTYPE* pSoldier = NULL;
	INT32 cnt = gTacticalStatus.Team[bTeam].bFirstID;
	INT32 lastid = gTacticalStatus.Team[bTeam].bLastID;
	for ( pSoldier = MercPtrs[cnt]; cnt < lastid; ++cnt, ++pSoldier )
	{
		if ( pSoldier != pSniper && pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ
			 && pSoldier->IsSpotting( )
			 && PythSpacesAway( pSoldier->sGridNo, pSniper->sGridNo ) <= gGameExternalOptions.usSpotterRange
			 && PythSpacesAway( pSoldier->sGridNo, sGridNo ) >= 2 * gGameExternalOptions.usSpotterRange )
		{
			BOOLEAN targetseen = FALSE;

			UINT16 usID = WhoIsThere2( sGridNo, pSniper->bTargetLevel );

			// is someone is at the sGridNo, check wether the spotter can see any part of him check wether head can be seen)
			if ( usID != NOBODY && SoldierToSoldierLineOfSightTest( pSoldier, MercPtrs[usID], 0, NO_DISTANCE_LIMIT, AIM_SHOT_HEAD ) > 0 )
				targetseen = TRUE;
			// otherwise check wether we can see the ground floor
			else if ( SoldierToVirtualSoldierLineOfSightTest( pSoldier, sGridNo, pSniper->pathing.bLevel, ANIM_PRONE, FALSE, NO_DISTANCE_LIMIT ) > 0 )
				targetseen = TRUE;

			if ( targetseen )
			{
				// spotter items are used to determine effectiveness. cap each hand item to a maximum of 100 pts (to keep players from using guns with tons of attachments that have been declared 'spotter items')
				UINT16 itembonus = 0;
				if ( pSoldier->inv[HANDPOS].exists( ) )
					itembonus += min( 100, GetObjectModifier( pSoldier, &(pSoldier->inv[HANDPOS]), gAnimControl[pSoldier->usAnimState].ubEndHeight, ITEMMODIFIER_SPOTTER ) );

				if ( pSoldier->inv[SECONDHANDPOS].exists( ) )
					itembonus += min( 100, GetObjectModifier( pSoldier, &(pSoldier->inv[SECONDHANDPOS]), gAnimControl[pSoldier->usAnimState].ubEndHeight, ITEMMODIFIER_SPOTTER ) );

				// base spotter effectivity depends on 40% items, 30% experience, 20% marksmanship an 10% leadership 
				// the nominal value is between 0 and 1000 (though the actual value can be raised higher, due to effective stat and level boni)
				UINT32 value = 2 * itembonus + 30 * EffectiveExpLevel( pSoldier ) + 2 * EffectiveMarksmanship( pSoldier ) + EffectiveLeadership( pSoldier );

				// lowered effectivity if we're fatigued
				ReducePointsForFatigue( pSoldier, &value );

				// lowered effectivity if we're wounded
				value = (value * pSoldier->stats.bLife / pSoldier->stats.bLifeMax);

				// effectivity of spotter and sniper working together in percent
				UINT16 effectivity = 100;

				// sociable mercs get a bonus, loners get a malus
				if ( OKToCheckOpinion( pSoldier->ubProfile ) )
				{
					MERCPROFILESTRUCT*	pProfile = &(gMercProfiles[pSoldier->ubProfile]);

					switch ( pProfile->bCharacterTrait )
					{
					case CHAR_TRAIT_SOCIABLE:
						effectivity += 10;
						break;

					case CHAR_TRAIT_LONER:
						effectivity -= 10;
						break;
					}
				}

				if ( OKToCheckOpinion( pSniper->ubProfile ) )
				{
					MERCPROFILESTRUCT*	pProfile_Sniper = &(gMercProfiles[pSniper->ubProfile]);

					switch ( pProfile_Sniper->bCharacterTrait )
					{
					case CHAR_TRAIT_SOCIABLE:
						effectivity += 10;
						break;

					case CHAR_TRAIT_LONER:
						effectivity -= 10;
						break;
					}
				}

				// relation between sniper and spotter is important - they need to trust each other (-50 to 50)
				INT8 relation = min( 2 * BUDDY_OPINION, max( 2 * HATED_OPINION, SoldierRelation( pSoldier, pSniper ) + SoldierRelation( pSniper, pSoldier ) ) );

				// relation counts twice. Also account for special background. Effectivity cannot be lower than 0%!
				effectivity = max( 0, effectivity + 2 * relation + pSoldier->GetBackgroundValue( BG_PERC_SPOTTER ) );
				
				// a good relation boosts value tremendously - a bad relation makes spotting useless
				// the spotter background also alters effectiveness
				// -> value between 0 and 2000, nominal 1000
				value = (value * effectivity ) / 100;

				// longer spotting gives a linear bonus - up to 100% -> value between 0 and 4000, nominal 2000
				value = (value * min(pSoldier->usSkillCounter[SOLDIER_COUNTER_SPOTTER], 2 * gGameExternalOptions.usSpotterPreparationTurns)) / gGameExternalOptions.usSpotterPreparationTurns;
				
				// reasonable values: 0 to gGameExternalOptions.usSpotterMaxCTHBoost
				value = (value * gGameExternalOptions.usSpotterMaxCTHBoost) / 2000;
								
				if ( value > bestvalue )
					bestvalue = value;
			}
		}
	}

	// limit value
	bestvalue = min( gGameExternalOptions.usSpotterMaxCTHBoost, max( 0, bestvalue ) );

	return bestvalue;
}

// get overt penalty duration in AP for using an animation
UINT16	GetSuspiciousAnimationAPDuration( UINT16 usAnimation )
{
	switch ( usAnimation )
	{
	case NINJA_PUNCH:
	case NINJA_LOWKICK:
	case PUNCH_LOW:
	case CROWBAR_ATTACK:
	case DODGE_ONE:
	case SLICE:
	case STAB:
	case CROUCH_STAB:
	case BAYONET_STAB_STANDING_VS_STANDING:
	case BAYONET_STAB_STANDING_VS_PRONE:
	case PUNCH:
	case PUNCH_BREATH:
	case KICK_DOOR:
	case FOCUSED_PUNCH:
	case FOCUSED_STAB:
	case HTH_KICK:
	case FOCUSED_HTH_KICK:
		return 60; break;

	case THROW_GRENADE_STANCE:
	case LOB_GRENADE_STANCE:
	case THROW_KNIFE:
	case THROW_KNIFE_SP_BM:
	case THROW_ITEM:
	case LOB_ITEM:
	case THROW_ITEM_CROUCHED:
		return 50; break;

	case PICKUP_ITEM:
	case DROP_ITEM:
		return 30; break;

	case DECAPITATE:
	case TAKE_BLOOD_FROM_CORPSE:
		return 50; break;

	case PLANT_BOMB:
	case USE_REMOTE:
	case STEAL_ITEM:
	case PICK_LOCK:
	case LOCKPICK_CROUCHED:
	case STEAL_ITEM_CROUCHED:
		return 50; break;

	case SHOOT_ROCKET_CROUCHED:
	case SHOOT_ROCKET:
	case HELIDROP:
	case NINJA_SPINKICK:
		return 100; break;

	case CUTTING_FENCE:
	case JUMPWINDOWS:
	case LONG_JUMP:
		return 60; break;
	}

	return 0;
}

void SetDamageDisplayCounter( SOLDIERTYPE* pSoldier )
{
	INT16 sOffsetX, sOffsetY;

	if ( pSoldier->flags.fDisplayDamage )
	{
		pSoldier->bDisplayDamageCount = 0;
		return;
	}

	pSoldier->flags.fDisplayDamage = TRUE;
	pSoldier->bDisplayDamageCount = 0;

	if ( pSoldier->ubBodyType == QUEENMONSTER )
	{
		pSoldier->sDamageX = 0;
		pSoldier->sDamageY = 0;
	}
	else
	{
		GetSoldierAnimOffsets( pSoldier, &sOffsetX, &sOffsetY );
		pSoldier->sDamageX = sOffsetX;
		pSoldier->sDamageY = sOffsetY;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - This whole procedure was merged with the surgery ability of the doctor trait
UINT32 VirtualSoldierDressWound( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVictim, OBJECTTYPE *pKit, INT16 sKitPts, INT16 sStatus, BOOLEAN fOnSurgery )
{
	UINT32 uiDressSkill, uiPossible, uiActual, uiMedcost, uiDeficiency, uiAvailAPs, uiUsedAPs;
	UINT8 bBelowOKlife, bPtsLeft;
	INT8 bInitialBleeding;

	if ( pVictim->bBleeding < 1 && !fOnSurgery )
		return 0;		// nothing to do, shouldn't have even been called!
	if ( pVictim->stats.bLife == 0 )
		return 0;
	if ( fOnSurgery && pVictim->ubID == pSoldier->ubID ) // cannot make surgery on self
		return 0;

	bInitialBleeding = pVictim->bBleeding;

	if ( !gGameOptions.fNewTraitSystem && fOnSurgery ) // cannot make surgery if not new traits
		fOnSurgery = FALSE;

	// calculate wound-dressing skill (3x medical, 2x equip, 1x level, 1x dex)
	if ( gGameOptions.fNewTraitSystem )
	{
		uiDressSkill = ((7 * EffectiveMedical( pSoldier )) +					// medical knowledge
						 (sStatus)+ 																// state of medical kit
						 (10 * EffectiveExpLevel( pSoldier )) +					// battle injury experience
						 EffectiveDexterity( pSoldier, FALSE )) / 10;		// general "handiness"
	}
	else
	{
		uiDressSkill = ((3 * EffectiveMedical( pSoldier )) +					// medical knowledge
						 (2 * sStatus) + 																// state of medical kit
						 (10 * EffectiveExpLevel( pSoldier )) +					// battle injury experience
						 EffectiveDexterity( pSoldier, FALSE )) / 7;		// general "handiness"
	}

	// try to use every AP that the merc has left
	uiAvailAPs = pSoldier->bActionPoints;

	// OK, If we are in real-time, use another value...
	if ( !(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT) )
	{	// Set to a value which looks good based on out tactical turns duration
		uiAvailAPs = RT_FIRST_AID_GAIN_MODIFIER;
	}

	// calculate how much bandaging CAN be done this turn
	uiPossible = (uiAvailAPs * uiDressSkill) / 50;	// max rate is 2 * fullAPs

	// if no healing is possible (insufficient APs or insufficient dressSkill)
	if ( !uiPossible )
		return 0;

	if (ItemIsMedicalKit(pSoldier->inv[0].usItem) && !(fOnSurgery) )		// using the GOOD medic stuff
		uiPossible += (uiPossible / 2);			// add extra 50 %

	// Doctor trait improves basic bandaging ability
	if ( !(fOnSurgery) && gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, DOCTOR_NT ) )
	{
		uiPossible = uiPossible * (100 - gSkillTraitValues.bSpeedModifierBandaging) / 100;
		uiPossible += (uiPossible * gSkillTraitValues.ubDOBandagingSpeedPercent * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ) + pSoldier->GetBackgroundValue( BG_PERC_BANDAGING )) / 100;
	}

	uiActual = uiPossible;		// start by assuming maximum possible

	// figure out how far below OKLIFE the victim is
	if ( pVictim->stats.bLife >= OKLIFE )
		bBelowOKlife = 0;
	else
		bBelowOKlife = OKLIFE - pVictim->stats.bLife;

	// figure out how many healing pts we need to stop dying (2x cost)
	uiDeficiency = (2 * bBelowOKlife);

	// if, after that, the patient will still be bleeding
	if ( (pVictim->bBleeding - bBelowOKlife) > 0 )
	{ // then add how many healing pts we need to stop bleeding (1x cost)
		uiDeficiency += (pVictim->bBleeding - bBelowOKlife);
	}
	// On surgery, alter this by amount of life we can heal
	if ( fOnSurgery )
	{
		uiDeficiency += (pVictim->iHealableInjury / 100);
	}
	// now, make sure we weren't going to give too much
	if ( uiActual > uiDeficiency )	// if we were about to apply too much
		uiActual = uiDeficiency;	// reduce actual not to waste anything

	// now make sure we HAVE that much
	if (ItemIsMedicalKit(pKit->usItem))
	{
		if ( fOnSurgery )
			uiMedcost = (uiActual * gSkillTraitValues.usDOSurgeryMedBagConsumption) / 100;		// surgery drains the kit a lot
		else
			uiMedcost = (uiActual + 1) / 2;		// cost is only half, rounded up

		if ( uiMedcost == 0 && uiActual > 0 )
			uiMedcost = 1;
		if ( uiMedcost > (UINT32)sKitPts )     		// if we can't afford this
		{
			uiMedcost = sKitPts;		// what CAN we afford?
			if ( fOnSurgery ) // surgery check
				uiActual = (uiMedcost * 100) / gSkillTraitValues.usDOSurgeryMedBagConsumption;
			else
				uiActual = uiMedcost * 2;		// give double this as aid
		}
	}
	else
	{
		uiMedcost = uiActual;
		if ( uiMedcost == 0 && uiActual > 0 )
			uiMedcost = 1;
		if ( uiMedcost > (UINT32)sKitPts )		// can't afford it
			uiMedcost = uiActual = sKitPts;		// recalc cost AND aid
	}

	bPtsLeft = (INT8)uiActual;
	// heal real life points first (if below OKLIFE) because we don't want the
	// patient still DYING if bandages run out, or medic is disabled/distracted!
	// NOTE: Dressing wounds for life below OKLIFE now costs 2 pts/life point!
	if ( bPtsLeft && pVictim->stats.bLife < OKLIFE )
	{
		// if we have enough points to bring him all the way to OKLIFE this turn
		if ( bPtsLeft >= (2 * bBelowOKlife) )
		{
			// insta-healable injury check
			if ( pVictim->iHealableInjury > 0 )
			{
				pVictim->iHealableInjury -= ((OKLIFE - pVictim->stats.bLife) * 100);
				if ( pVictim->iHealableInjury < 0 )
					pVictim->iHealableInjury = 0;
			}
			// raise life to OKLIFE
			pVictim->stats.bLife = OKLIFE;
			// reduce bleeding by the same number of life points healed up
			pVictim->bBleeding -= bBelowOKlife;

			// use up appropriate # of actual healing points
			bPtsLeft -= (2 * bBelowOKlife);
		}
		else
		{
			// insta-healable injury check
			if ( pVictim->iHealableInjury > 0 )
			{
				pVictim->iHealableInjury -= ((bPtsLeft / 2) * 100);
				if ( pVictim->iHealableInjury < 0 )
					pVictim->iHealableInjury = 0;
			}
			pVictim->stats.bLife += (bPtsLeft / 2);
			pVictim->bBleeding -= (bPtsLeft / 2);
			
			bPtsLeft = bPtsLeft % 2;	// if ptsLeft was odd, ptsLeft = 1
		}

		// this should never happen any more, but make sure bleeding not negative
		if ( pVictim->bBleeding < 0 )
		{
			pVictim->bBleeding = 0;
		}

		// if this healing brought the patient out of the worst of it, cancel dying
		if ( pVictim->stats.bLife >= OKLIFE )
		{ // turn off merc QUOTE flags
			pVictim->flags.fDyingComment = FALSE;
		}

		if ( pVictim->bBleeding <= MIN_BLEEDING_THRESHOLD )
		{
			pVictim->flags.fWarnedAboutBleeding = FALSE;
		}
	}

	// SURGERY 
	// first return the real life back, then bandage the rest if possible
	if ( fOnSurgery && gGameOptions.fNewTraitSystem ) // double check for new traits
	{
		INT32 iLifeReturned = 0;
		UINT16 usReturnDamagedStatRate = 0;
		// find out if we will repair any stats...
		if ( NumberOfDamagedStats( pVictim ) > 0 )
		{
			usReturnDamagedStatRate = ((gSkillTraitValues.usDORepairStatsRateBasic + gSkillTraitValues.usDORepairStatsRateOnTop * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT )));
			usReturnDamagedStatRate -= max( 0, ((usReturnDamagedStatRate * gSkillTraitValues.ubDORepStPenaltyIfAlsoHealing) / 100) );

			// ... in which case, reduce the points
			bPtsLeft = max( 0, ((bPtsLeft * (100 - gSkillTraitValues.ubDOHealingPenaltyIfAlsoStatRepair)) / 100) );
		}

		// Important note! : HealableInjury is always stores the total HPs the victim is missing, not the amount which we will heal,
		// so we always take a portion of patient's damage here, reduce the HealableInjury by this portion, while only healing a portion of this portion in actual HPs;
		// this means the rest of HPs will remain as "unhealable", the patient will miss X HPs but has no HealableInjury on self..
		if ( bPtsLeft >= (pVictim->iHealableInjury / 100) )
		{
			if ( pSoldier->usSoldierFlagMask2 & SOLDIER_SURGERY_BOOSTED )
				iLifeReturned = pVictim->iHealableInjury * ( gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentBloodbag + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ) ) / 100;
			else
				iLifeReturned = pVictim->iHealableInjury * ( gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ) ) / 100;

			pVictim->iHealableInjury = 0;
			// keep the rest of the points to bandaging if neccessary
			if ( pVictim->bBleeding > 0 )
			{
				bPtsLeft = max( 0, (bPtsLeft - (iLifeReturned / 100)) );
				bPtsLeft += (bPtsLeft / 2); // we use medical bag so add the bonus for that.
			}
			else
			{
				bPtsLeft = 0;
			}

			// add to record - another surgery undergoed
			if ( pVictim->ubProfile != NO_PROFILE && iLifeReturned >= 100 )
				gMercProfiles[pVictim->ubProfile].records.usTimesSurgeryUndergoed++;

			// add to record - another surgery made
			if ( pSoldier->ubProfile != NO_PROFILE && iLifeReturned >= 100 )
				gMercProfiles[pSoldier->ubProfile].records.usSurgeriesMade++;
		}
		else
		{
			if ( pSoldier->usSoldierFlagMask2 & SOLDIER_SURGERY_BOOSTED )
				iLifeReturned = bPtsLeft * (gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentBloodbag + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ));
			else
				iLifeReturned = bPtsLeft * ( gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop * NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ) );

			pVictim->iHealableInjury -= (bPtsLeft * 100);
			bPtsLeft = 0;
		}
		// repair the stats here!
		if ( usReturnDamagedStatRate > 0 )
		{
			RegainDamagedStats( pVictim, (iLifeReturned * usReturnDamagedStatRate / 100) );
		}

		// some paranoya checks for sure
		if ( (pVictim->stats.bLife + (iLifeReturned / 100)) <= pVictim->stats.bLifeMax )
		{
			pVictim->stats.bLife += (iLifeReturned / 100);
			if ( pVictim->bBleeding >= (iLifeReturned / 100) )
			{
				pVictim->bBleeding -= (iLifeReturned / 100);
				uiMedcost += (iLifeReturned / 200); // add medkit points cost for unbandaged part
			}
			else
			{
				pVictim->bBleeding = 0;
				uiMedcost += max( 0, (((iLifeReturned / 100) - pVictim->bBleeding) / 2) ); // add medkit points cost for unbandaged part
			}

			// display healing done
			pVictim->flags.fDisplayDamage = TRUE;
			pVictim->sDamage -= (iLifeReturned / 100);
		}
		else // this shouldn't even happen, but we still want to have it here for sure
		{
			// display healing done
			pVictim->flags.fDisplayDamage = TRUE;
			pVictim->sDamage -= (pVictim->stats.bLifeMax - pVictim->stats.bLife);

			pVictim->stats.bLife = pVictim->stats.bLifeMax;
			pVictim->iHealableInjury = 0;
			pVictim->bBleeding = 0;
		}
		// Reduce max breath based on life returned
		if ( (pVictim->bBreathMax - (((iLifeReturned / 100) * gSkillTraitValues.usDOSurgeryMaxBreathLoss) / 100)) <= BREATHMAX_ABSOLUTE_MINIMUM )
		{
			pVictim->bBreathMax = BREATHMAX_ABSOLUTE_MINIMUM;
		}
		else
		{
			pVictim->bBreathMax -= (((iLifeReturned / 100) * gSkillTraitValues.usDOSurgeryMaxBreathLoss) / 100);
		}

		if ( pVictim->iHealableInjury > ((pVictim->stats.bLifeMax - pVictim->stats.bLife) * 100) )
			pVictim->iHealableInjury = ((pVictim->stats.bLifeMax - pVictim->stats.bLife) * 100);
		else if ( pVictim->iHealableInjury < 0 )
			pVictim->iHealableInjury = 0;

		// Flugente: campaign stats
		gCurrentIncident.usIncidentFlags |= INCIDENT_SURGERY;
	}

	// if any healing points remain, apply that to any remaining bleeding (1/1)
	// DON'T spend any APs/kit pts to cure bleeding until merc is no longer dying
	//if ( bPtsLeft && pVictim->bBleeding && !pVictim->dying)
	if ( bPtsLeft && pVictim->bBleeding )
	{
		// if we have enough points to bandage all remaining bleeding this turn
		if ( bPtsLeft >= pVictim->bBleeding )
		{
			bPtsLeft -= pVictim->bBleeding;
			pVictim->bBleeding = 0;
		}
		else		// bandage what we can
		{
			pVictim->bBleeding -= bPtsLeft;
			bPtsLeft = 0;
		}
	}

	//CHRISL: If by some chance ubPtsLeft ends up being higher then uiActual, we'll end up with a huge value since uiActual is an unsigned variable.
	// if there are any ptsLeft now, then we didn't actually get to use them
	uiActual = max( 0, (INT32)(uiActual - bPtsLeft) );

	// usedAPs equals (actionPts) * (%of possible points actually used)
	uiUsedAPs = (uiActual * uiAvailAPs) / uiPossible;

	if (ItemIsMedicalKit(pSoldier->inv[0].usItem) && !(fOnSurgery) )	// using the GOOD medic stuff
		uiUsedAPs = (uiUsedAPs * 2) / 3;	// reverse 50% bonus by taking 2/3rds

	// surgery is harder so cost more BPs
	if ( fOnSurgery )
		DeductPoints( pSoldier, (INT16)uiUsedAPs, (INT16)(uiUsedAPs * 15) );
	else
		DeductPoints( pSoldier, (INT16)uiUsedAPs, (INT16)((uiUsedAPs * APBPConstants[BP_PER_AP_LT_EFFORT])) );

	// surgery is harder so gives more exp
	if ( fOnSurgery )
	{
		// MEDICAL GAIN   (actual / 2):  Helped someone by giving first aid
		StatChange( pSoldier, MEDICALAMT, (UINT16)(uiActual + 2), FALSE );

		// DEXTERITY GAIN (actual / 6):  Helped someone by giving first aid
		StatChange( pSoldier, DEXTAMT, (UINT16)((uiActual / 3) + 2), FALSE );
	}
	else
	{
		if ( uiActual / 2 )
			// MEDICAL GAIN (actual / 2):	Helped someone by giving first aid
			StatChange( pSoldier, MEDICALAMT, ((UINT16)(uiActual / 2)), FALSE );

		if ( uiActual / 4 )
			// DEXTERITY GAIN (actual / 4):	Helped someone by giving first aid
			StatChange( pSoldier, DEXTAMT, (UINT16)((uiActual / 4)), FALSE );
	}

	// merc records - bandaging
	if ( bInitialBleeding > 1 && pVictim->bBleeding == 0 && pSoldier->ubProfile != NO_PROFILE )
		gMercProfiles[pSoldier->ubProfile].records.usMercsBandaged++;

	return uiMedcost;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Flugente: decide whether pRecruiter can successfully recruit pTarget to be a volunteer
void HandleVolunteerRecruitment( SOLDIERTYPE* pRecruiter, SOLDIERTYPE* pTarget )
{
	if ( !pRecruiter || pRecruiter->bTeam != OUR_TEAM )
		return;
	
	// potential recruit must be a civilain NPC of no other affilation, no kids
	if ( !pTarget || pTarget->bTeam != CIV_TEAM || pTarget->ubProfile != NO_PROFILE || pTarget->ubCivilianGroup != NON_CIV_GROUP || pTarget->ubBodyType > DRESSCIV )
		return;

	// target must unharmed
	if ( pTarget->bCollapsed || pTarget->bBreathCollapsed || pTarget->stats.bLife < pTarget->stats.bLifeMax )
		return;

	// target must be friendly
	if ( !pTarget->aiData.bNeutral )
		return;

	// Set a flag in this sector. This flag is removed every x hours. As long as it exists, newly created civilians won't be potential volunteers
	// this is simply there to prevent the exploit of reloading the sector repeatedly and 'harvesting' volunteers.
	// we do this even if we do not recruit this guy - otherwise the player could reload the sector over and over again until he find a volunteer
	UINT8 sector = SECTOR( pTarget->sSectorX, pTarget->sSectorY );

	SECTORINFO *pSectorInfo = &(SectorInfo[sector]);
	
	if ( pSectorInfo )
	{
		pSectorInfo->usSectorInfoFlag |= SECTORINFO_VOLUNTEERS_RECENTLY_RECRUITED;
	}

	// can we recruit him in the first place?
	if ( pTarget->usSoldierFlagMask2 & SOLDIER_POTENTIAL_VOLUNTEER )
	{
		// if sector not under our control, has enemies in it, or is currently in combat mode
		if ( !SectorOursAndPeaceful( pTarget->sSectorX, pTarget->sSectorY, pTarget->bSectorZ ) )
			return;
		
		// if this a town sector, min loyalty is required
		// other sectors do not have a loyalty rating. This is okay here, as then the player has an incentive to try his luck outside of towns
		UINT8 ubTownID = StrategicMap[CALCULATE_STRATEGIC_INDEX( pTarget->sSectorX, pTarget->sSectorY )].bNameId;
		if ( ubTownID != BLANK_SECTOR )
		{
			if ( gTownLoyalty[ubTownID].ubRating < gGameExternalOptions.iMinLoyaltyToTrain )
				return;
		}

		// several factors determine whether we can successfully recruit this guy
		FLOAT leadershipfactor = EffectiveLeadership( pRecruiter ) / 100.0f;

		// bonus for assertive characters
		if ( DoesMercHavePersonality( pRecruiter, CHAR_TRAIT_ASSERTIVE ) )
			leadershipfactor *= 1.05f;

		FLOAT recruitmodifier = (100 + pRecruiter->GetBackgroundValue( BG_PERC_APPROACH_RECRUIT )) / 100.0f;

		FLOAT rating = leadershipfactor * recruitmodifier * gMercProfiles[pRecruiter->ubProfile].usApproachFactor[3];
		
		// hard check. We do not use Chance()-base functions, as then the player would have to repeat this over and over again to be sure that someone is not a volunteer
		if ( rating > 70.0 )
		{
			// success! remove the flag, put this guy in a new group, and add one volunteer
			pTarget->usSoldierFlagMask2 &= ~SOLDIER_POTENTIAL_VOLUNTEER;

			pTarget->ubCivilianGroup = VOLUNTEER_CIV_GROUP;

			AddVolunteers( 1 );

			// the recruiter gets a bit of experience
			StatChange( pRecruiter, LDRAMT, 8, TRUE );
			StatChange( pRecruiter, EXPERAMT, 5, TRUE );
		}
	}
}

// Flugente: apply a consumable item on a soldier. Returns true if item was successfully interacted with
// Shadooow: Now returns 2 in case that the action failed due to the not enough action points!
BOOLEAN ApplyConsumable(SOLDIERTYPE* pSoldier, OBJECTTYPE *pObj, BOOLEAN fForce, BOOLEAN fUseAPs)
{
	if (!pSoldier || !pObj)
		return FALSE;

	// if it's not a kit or a misc item, we cannot consume it
	if (!(Item[pObj->usItem].usItemClass & (IC_KIT | IC_MISC)))
		return FALSE;

	BOOLEAN fSuccess = FALSE;
	BOOLEAN fDoSound = FALSE;

	// use portionsize, if none was entered, use full item
	UINT8 portionsize = Item[pObj->usItem].usPortionSize;
	if (!portionsize)
		portionsize = 100;

	// how much of this item do we use up
	UINT16 statusused = min(portionsize, (*pObj)[0]->data.objectStatus);
	if (!statusused || (statusused == 1 && ItemIsCanteen(pObj->usItem)))
		return FALSE;

	INT16 apcost = 0;
	
	// if we check for APs, do so - if we don't have enough, stop
	if ( fUseAPs )
	{
		// an object can be consumed in several ways (like food that is also a drug), but each consumption might have a different AP cost.
		// as it would be very odd if an effect does not happen because the corresponding AP cost could not be met, we analyze the item first and determine the AP cost.
		// We then either apply everything or nothing
		
		if ( HasItemFlag( pObj->usItem, CAMO_REMOVAL ) && gGameExternalOptions.fCamoRemoving )
		{
			apcost = max( apcost, (APBPConstants[AP_CAMOFLAGE] / 2) );
		}

		if (ItemIsCamoKit(pObj->usItem))
		{
			apcost = max( apcost, APBPConstants[AP_CAMOFLAGE] );
		}

		if (ItemIsCanteen(pObj->usItem))
		{
			apcost = max( apcost, APBPConstants[AP_DRINK] );
		}

		if ( pObj->usItem == JAR_ELIXIR )
		{
			apcost = max( apcost, APBPConstants[AP_CAMOFLAGE] );
		}

		if ( Item[pObj->usItem].clothestype )
		{
			INT16 disguise_apcost = (APBPConstants[AP_DISGUISE] * (100 - gSkillTraitValues.sCODisguiseAPReduction * NUM_SKILL_TRAITS( pSoldier, COVERT_NT ))) / 100;

			apcost = max( apcost, disguise_apcost );
		}

		if ( Item[pObj->usItem].drugtype )
		{
			apcost = max( apcost, APBPConstants[AP_DRINK] );
		}

		if ( Item[pObj->usItem].foodtype )
		{
			// do we eat or drink this stuff?
			UINT8 apcost_type = AP_EAT;
			if ( Food[Item[pObj->usItem].foodtype].bDrinkPoints > Food[Item[pObj->usItem].foodtype].bFoodPoints )
				apcost_type = AP_DRINK;

			apcost = max( apcost, APBPConstants[apcost_type] );
		}
	
		if ( !fForce && !EnoughPoints( pSoldier, apcost, 0, TRUE ) )
		{
			return 2;
		}
	}

	// under certain conditions, a merc can but simply does not want to consume an item, and can refuse if not forced to.
	if ( !fForce )
	{
		if ( DoesSoldierRefuseToEat( pSoldier, pObj ) )
		{
			return FALSE;
		}

		// some mercs will refuse to smoke
		if (ItemIsCigarette(pObj->usItem) && pSoldier->GetBackgroundValue( BG_SMOKERTYPE ) == 2 )
		{
			// merc gets slightly pissed by the player even suggesting this
			TacticalCharacterDialogue( pSoldier, QUOTE_REFUSE_TO_SMOKE );
			pSoldier->aiData.bMorale = max( 0, pSoldier->aiData.bMorale - 1 );

			return FALSE;
		}
	}
	
	// Try to apply camo....
	// this returns true if camo can be applied, but APs were only used, and the action happened, if *pfGoodAPs is TRUE
	if ( ApplyCamo( pSoldier, pObj->usItem, statusused ) )
	{
		fSuccess = TRUE;
		fDoSound = TRUE;

		// WANNE: We should only delete the face, if there was a camo we applied.
		// This should fix the bug and crashes with missing faces
		if ( gGameExternalOptions.fShowCamouflageFaces )
		{
			// Flugente: refresh face regardless of result of SetCamoFace(), otherwise applying a rag will not clean the picture
			SetCamoFace( pSoldier );
			DeleteSoldierFace( pSoldier );// remove face
			pSoldier->iFaceIndex = InitSoldierFace( pSoldier );// create new face
		}
	}
	
	if ( ApplyCanteen( pSoldier, pObj->usItem, statusused ) )
	{
		fSuccess = TRUE;
		fDoSound = FALSE;
	}
	
	if ( ApplyElixir( pSoldier, pObj->usItem, statusused ) )
	{
		fSuccess = TRUE;
		fDoSound = TRUE;
	}
	
	if ( ApplyClothes( pSoldier, pObj->usItem, statusused ) )
	{
		fSuccess = TRUE;
	}
	
	if ( ApplyFood( pSoldier, pObj, statusused ) )
	{
		fSuccess = TRUE;
		fDoSound = FALSE;
	}
	
	if ( ApplyDrugs_New( pSoldier, pObj->usItem, statusused ) )
	{
		fSuccess = TRUE;

		// no sound on consuming cigarettes, as that is very annoying
		if ( !ItemIsCigarette(pObj->usItem) )
		{
			fDoSound = TRUE;
		}
	}

	if ( !gGameExternalOptions.fFoodEatingSounds )
		fDoSound = FALSE;
	
	if ( fSuccess )
	{
		// Flugente: additional dialogue
		AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_CONSUMEITEM, pObj->usItem );

		// use up object
		UseKitPoints( pObj, statusused, pSoldier );

		if ( fUseAPs )
		{
			DeductPoints( pSoldier, (INT16)apcost, 0, false );

			// Dirty
			fInterfacePanelDirty = DIRTYLEVEL2;
		}

		if ( fDoSound )
		{
			// Say OK acknowledge....
			pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
		}

		return TRUE;
	}

	return FALSE;
}

// sevenfm: service functions
BOOLEAN SOLDIERTYPE::CheckInitialAP(void)
{
	if (this->bActionPoints < this->bInitialActionPoints || this->usSoldierFlagMask2 & SOLDIER_SPENT_AP)
	{
		return FALSE;
	}

	return TRUE;
}

BOOLEAN SOLDIERTYPE::IsFlanking(void)
{
	if (this->aiData.bAlertStatus < STATUS_YELLOW ||
		this->numFlanks == 0 ||
		this->numFlanks >= MAX_FLANKS_RED)
	{
		return FALSE;
	}

	return TRUE;
}

UINT8 SOLDIERTYPE::ShockLevelPercent(void)
{
	if (gGameExternalOptions.ubMaxSuppressionShock == 0)
		return 0;

	return min(100, 100 * this->aiData.bShock / gGameExternalOptions.ubMaxSuppressionShock);
}

BOOLEAN SOLDIERTYPE::TakenLargeHit(void)
{
	if (this->usSoldierFlagMask2 & SOLDIER_TAKEN_LARGE_HIT)
		return TRUE;

	return FALSE;
}

BOOLEAN SOLDIERTYPE::IsCowering(void)
{
	if (this->usAnimState == COWERING || this->usAnimState == COWERING_PRONE)
		return TRUE;

	return FALSE;
}

BOOLEAN SOLDIERTYPE::IsUnconscious(void)
{
	if (this->bCollapsed && this->bBreath < OKBREATH)
		return TRUE;

	return FALSE;
}

void SOLDIERTYPE::StopCoweringAnimation(void)
{
	if (this->usAnimState == COWERING)
	{
		if (gAnimControl[this->usAnimState].ubEndHeight == ANIM_STAND)
		{
			this->ubDesiredHeight = ANIM_STAND;
			this->EVENT_InitNewSoldierAnim(END_COWER, 0, FALSE);
		}
		else if (gAnimControl[this->usAnimState].ubEndHeight == ANIM_CROUCH)
		{
			this->ubDesiredHeight = ANIM_CROUCH;
			this->EVENT_InitNewSoldierAnim(END_COWER_CROUCHED, 0, FALSE);
		}
	}
	else if (this->usAnimState == COWERING_PRONE)
	{
		if (gAnimControl[this->usAnimState].ubEndHeight == ANIM_PRONE)
		{
			this->ubDesiredHeight = ANIM_PRONE;
			this->EVENT_InitNewSoldierAnim(END_COWER_PRONE, 0, FALSE);
		}
	}

	// remove AI cowering flag
	this->flags.uiStatusFlags &= ~SOLDIER_COWERING;
}

BOOLEAN	SOLDIERTYPE::IsGivingAid(void)
{
	if (this->usAnimState == GIVING_AID || this->usAnimState == GIVING_AID_PRN || this->usAnimState == START_AID || this->usAnimState == START_AID_PRN)
		return TRUE;

	return FALSE;
}

void	SOLDIERTYPE::RetreatCounterStart(UINT16 usValue)
{
	usSkillCounter[SOLDIER_COUNTER_RETREAT] = max(usValue, usSkillCounter[SOLDIER_COUNTER_RETREAT]);
}

void	SOLDIERTYPE::RetreatCounterStop(void)
{
	usSkillCounter[SOLDIER_COUNTER_RETREAT] = 0;
}

UINT16	SOLDIERTYPE::RetreatCounterValue(void)
{
	return usSkillCounter[SOLDIER_COUNTER_RETREAT];
}

void SOLDIERTYPE::StartRadioAnimation(void)
{
	if (this->ubBodyType != REGMALE && this->ubBodyType != BIGMALE ||
		Water(this->sGridNo, this->pathing.bLevel) ||
		this->bVisible != TRUE)
	{
		return;
	}

	switch (gAnimControl[this->usAnimState].ubEndHeight)
	{
	case ANIM_STAND:
		this->EVENT_InitNewSoldierAnim(AI_RADIO, 0, FALSE);
		break;
	case ANIM_CROUCH:
		this->EVENT_InitNewSoldierAnim(AI_CR_RADIO, 0, FALSE);
		break;
	case ANIM_PRONE:
		break;
	}
}

void SOLDIERTYPE::InitializeExtraData(void)
{
	this->ubLastShock = 0;
	this->ubLastSuppression = 0;
	this->ubLastAP = 0;
	this->ubLastMorale = 0;
	this->ubLastShockFromHit = 0;
	this->ubLastMoraleFromHit = 0;
	this->ubLastAPFromHit = 0;
	this->iLastBulletImpact = 0;
	this->iLastArmourProtection = 0;

	this->usQuickItemId = 0;
	this->ubQuickItemSlot = 0;

	this->usGrenadeItem = 0;

	this->delayedDamageFunction = nullptr;
}
