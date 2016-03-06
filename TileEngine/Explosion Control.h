#ifndef _EXPLOSION_CONTROL_H
#define _EXPLOSION_CONTROL_H

#include "Weapons.h"

#define MAX_DISTANCE_EXPLOSIVE_CAN_DESTROY_STRUCTURES 2


#define EXPLOSION_FLAG_USEABSPOS		0x00000001
#define EXPLOSION_FLAG_DISPLAYONLY	0x00000002


// Explosion Data
typedef struct
{
	UINT32											uiFlags;

	UINT8												ubOwner;
	INT8												ubTypeID;

	UINT16											usItem;

	INT16												sX;										// World X ( optional )
	INT16												sY;										// World Y ( optional )
	INT16												sZ;										// World Z ( optional )
	INT32 sGridNo;							// World GridNo
	BOOLEAN											fLocate;
	INT8												bLevel;								// World level
	UINT8												ubUnsed[1];

} EXPLOSION_PARAMS;
 

typedef struct
{
	EXPLOSION_PARAMS		Params;
	BOOLEAN							fAllocated;
	INT16								sCurrentFrame;
	INT32								iID;
	INT32								iLightID;
	UINT8								ubUnsed[2];


} EXPLOSIONTYPE;


enum EXPLOSION_TYPES
{
	NO_BLAST,
	BLAST_1,
	BLAST_2,
	BLAST_3,
	STUN_BLAST,	
	WATER_BLAST,
	TARGAS_EXP,
	SMOKE_EXP,
	MUSTARD_EXP,
	BURN_EXP,
	THERMOBARIC_EXP,
	FLASHBANG_EXP,		// Lesh: enum added
	ROOF_COLLAPSE,		// Flugente: play an animation of roof tiles crashing down
	NUM_EXP_TYPES=50

} ;

// Lesh: introducing struct instead of linked arrays
#define MAX_BLAST_FILENAME_LEN 70

typedef struct
{
	UINT8	ubTransKeyFrame;
	UINT8	ubDamageKeyFrame;
	UINT32	uiExplosionSoundID;
	UINT32	uiAltExplosionSoundID;
	CHAR8	zBlastFilename[MAX_BLAST_FILENAME_LEN];
	CHAR8	sBlastSpeed;
} EXPLOSION_DATA;

extern EXPLOSION_DATA gExpAniData[NUM_EXP_TYPES];
// Lesh: done here


typedef struct
{
	UINT32	uiWorldBombIndex;
	UINT32	uiTimeStamp;
	UINT8		fExists;
} ExplosionQueueElement;


#define	ERASE_SPREAD_EFFECT					2
#define BLOOD_SPREAD_EFFECT					3
#define REDO_SPREAD_EFFECT					4


#define	NUM_EXPLOSION_SLOTS					100

extern	EXPLOSIONTYPE								gExplosionData[ NUM_EXPLOSION_SLOTS ];
extern UINT8 gubElementsOnExplosionQueue;
extern BOOLEAN gfExplosionQueueActive;

void IgniteExplosion( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT8 ubDirection = DIRECTION_IRRELEVANT, OBJECTTYPE * pObj =NULL );
void InternalIgniteExplosion( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection = DIRECTION_IRRELEVANT, OBJECTTYPE * pObj =NULL );


void GenerateExplosion( EXPLOSION_PARAMS *pExpParams );

void DecayBombTimers( void );
void SetOffBombsByFrequency( UINT8 ubID, INT8 bFrequency );
BOOLEAN SetOffBombsInGridNo( UINT8 ubID, INT32 sGridNo, BOOLEAN fAllBombs, INT8 bLevel );
void ActivateSwitchInGridNo( UINT8 ubID, INT32 sGridNo );
void SetOffPanicBombs( UINT8 ubID, INT8 bPanicTrigger );

void UpdateExplosionFrame( INT32 iIndex, INT16 sCurrentFrame );
void RemoveExplosionData( INT32 iIndex );

void UpdateAndDamageSAMIfFound( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo, UINT8 ubDamage );
void UpdateSAMDoneRepair( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ	);


BOOLEAN SaveExplosionTableToSaveGameFile( HWFILE hFile );

BOOLEAN LoadExplosionTableFromSavedGameFile( HWFILE hFile );

INT32 FindActiveTimedBomb( void );
BOOLEAN ActiveTimedBombExists( void );
void RemoveAllActiveTimedBombs( void );

// Flugente: remove any marker that an explosion is active but has not yet occured (to be used when leaving a sector)
void RemoveActiveExplosionMarkers();

#define GASMASK_MIN_STATUS 70

// OJW - 20091028 - Explosion damage sync
BOOLEAN DamageSoldierFromBlast( UINT8 ubPerson, UINT8 ubOwner, INT32 sBombGridNo, INT16 sWoundAmt, INT16 sBreathAmt, UINT32 uiDist, UINT16 usItem, INT16 sSubsequent , BOOL fFromRemoteClient = FALSE );
BOOLEAN DishOutGasDamage( SOLDIERTYPE * pSoldier, EXPLOSIVETYPE * pExplosive, INT16 sSubsequent, BOOLEAN fRecompileMovementCosts, INT16 sWoundAmt, INT16 sBreathAmt, UINT8 ubOwner , BOOL fFromRemoteClient = FALSE );
void SpreadEffect( INT32 sGridNo, UINT8 ubRadius, UINT16 usItem, UINT8 ubOwner, BOOLEAN fSubsequent, INT8 bLevel, INT32 iSmokeEffectNum , BOOL fFromRemoteClient = FALSE , BOOL fNewSmokeEffect = FALSE );
void AddBombToQueue( UINT32 uiWorldBombIndex, UINT32 uiTimeStamp, BOOL fFromRemoteClient = FALSE );

// Flugente: activate everything connected to a tripwire in the surrounding if sGridNo on level bLevel with regard to the tripwire netwrok and hierarchy determined by ubFlag
BOOLEAN ActivateSurroundingTripwire( UINT8 ubID, INT32 sGridNo, INT8 bLevel, UINT32 ubFlag );

// Flugente: A special function for tripwire gun traps. Search if pObj has a gun attached. If so, fire a shot from that gun in a specific direction. Afterwards place the gun on the ground
void CheckAndFireTripwireGun( OBJECTTYPE* pObj, INT32 sGridNo, INT8 bLevel, UINT8 ubId, UINT8 ubDirection );

extern void ToggleActionItemsByFrequency( INT8 bFrequency );
extern void PerformItemAction( INT32 sGridNo, OBJECTTYPE * pObj );
extern BOOLEAN		 gfExplosionQueueMayHaveChangedSight;
extern void TogglePressureActionItemsInGridNo( INT32 sGridNo );

extern BOOLEAN DoesSAMExistHere( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo );
//DBrot: More Rooms
//extern BOOLEAN HookerInRoom( UINT8 ubRoom );
extern BOOLEAN HookerInRoom( UINT16 usRoom );

#ifdef JA2UB
extern void HavePersonAtGridnoStop( UINT32 sGridNo );
extern BOOLEAN ShouldThePlayerStopWhenWalkingOnBiggensActionItem( UINT8 ubRecordNum );
extern void HandleDestructionOfPowerGenFan();
extern BOOLEAN IsFanGraphicInSectorAtThisGridNo( UINT32 sGridNo );
extern void HandleExplosionsInTunnelSector( UINT32 sGridNo );
extern void HandleSwitchToOpenFortifiedDoor( UINT32 sGridNo );
extern void HandleSeeingPowerGenFan( UINT32 sGridNo );
extern void HandleSeeingFortifiedDoor( UINT32 sGridNo );//Ja25 UB
#endif

void ExplosiveDamageGridNo( INT32 sGridNo, INT16 sWoundAmt, UINT32 uiDist, BOOLEAN *pfRecompileMovementCosts, BOOLEAN fOnlyWalls, INT8 bMultiStructSpecialFlag, BOOLEAN fSubSequentMultiTilesTransitionDamage, UINT8 ubOwner, INT8 bLevel );

// Flugente: handle secondary explosive effects
void HandleBuddyExplosions(UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection );

// sevenfm: handle explosive items from attachments
BOOLEAN HandleAttachedExplosions(UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection, OBJECTTYPE * pObj );
void CheckForBuriedBombsAndRemoveFlags( INT32 sGridNo, INT8 bLevel );
UINT16 CalcTotalVolatility(OBJECTTYPE * pObj);
BOOLEAN FindBinderAttachment (OBJECTTYPE * pObj);
BOOLEAN CheckExplosiveTypeAsDetonator(UINT16 ubType);

// Flugente: destroy rooftops
void RoofDestruction( INT32 sGridNo, BOOLEAN fWithExplosion = TRUE );
void HandleRoofDestruction( INT32 sGridNo, INT16 sDamage );

#endif
