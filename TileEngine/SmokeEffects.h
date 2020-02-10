#ifndef __SMOKE_EFFECTS
#define __SMOKE_EFFECTS


// Smoke effect types
enum
{
	NO_SMOKE_EFFECT,
	NORMAL_SMOKE_EFFECT,
	TEARGAS_SMOKE_EFFECT,
	MUSTARDGAS_SMOKE_EFFECT,
	CREATURE_SMOKE_EFFECT,
	BURNABLEGAS_SMOKE_EFFECT,
	SIGNAL_SMOKE_EFFECT,		// added by Flugente for artillery
	DEBRIS_SMOKE_EFFECT,		// added by Flugente for collapsing roofs
};

#define SMOKE_EFFECT_INDOORS			0x01
#define SMOKE_EFFECT_ON_ROOF			0x02
#define SMOKE_EFFECT_MARK_FOR_UPDATE	0x04

#define		NUM_SMOKE_EFFECT_SLOTS					25 // OJW - 20091027 - moved here to allow global access

typedef struct TAG_SMOKE_EFFECT
{
	INT32 sGridNo;          // gridno at which the tear gas cloud is centered

	UINT8		ubDuration;		// the number of turns gas will remain effective
	UINT8		ubRadius;			// the current radius of the cloud in map tiles
	UINT8	bFlags;			// 0 - outdoors (fast spread), 1 - indoors (slow)
	UINT8		bAge;			 // the number of turns gas has been around
	BOOLEAN	fAllocated;
	INT8		bType;
	UINT16	usItem;
	UINT8	ubOwner;
	UINT8	ubGeneration;
	UINT32	uiTimeOfLastUpdate;
	INT8	iMPTeamIndex;
	INT32	iMPSmokeEffectID;
} SMOKEEFFECT;

extern SMOKEEFFECT				gSmokeEffectData[ NUM_SMOKE_EFFECT_SLOTS ];
extern UINT32					guiNumSmokeEffects;

// Returns NO_SMOKE_EFFECT if none there...
INT8 GetSmokeEffectOnTile( INT32 sGridNo, INT8 bLevel );

// Decays all smoke effects...
// HEADROCK HAM 5: New argument here tells the decay function to only process clouds belonging to one team.
// -1 = all teams.
void DecaySmokeEffects( UINT32 uiTime );

// Add smoke to gridno
// ( Replacement algorithm uses distance away )
void AddSmokeEffectToTile( INT32 iSmokeEffectID, INT8 bType, INT32 sGridNo, INT8 bLevel );

void RemoveSmokeEffectFromTile( INT32 sGridNo, INT8 bLevel );

INT32 NewSmokeEffect(INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT8 ubOwner, BOOLEAN fFromRemoteClient = 0, UINT8 ubDuration = 0, UINT8 ubRadius = 0, UINT8 ubGeneration = 0);

BOOLEAN SaveSmokeEffectsToSaveGameFile( HWFILE hFile );
BOOLEAN LoadSmokeEffectsFromLoadGameFile( HWFILE hFile );

BOOLEAN SaveSmokeEffectsToMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );
BOOLEAN LoadSmokeEffectsFromMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

void ResetSmokeEffects();

void UpdateSmokeEffectGraphics( );

// Flugente: get the gridno and blevel of a random smoke signal, if one exists
BOOL GetRandomSignalSmokeGridNo(INT32* psGridNo);

// find smoke effect on visible screen
BOOLEAN FindVisibleSmokeEffect(INT8 bType);

#endif