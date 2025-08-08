#ifndef _ROTTING_CORPSES_H
#define _ROTTING_CORPSES_H

#include "tile animation.h"

#define	NUM_CORPSE_SHADES		17
#define CORPSE_WARNING_MAX 5
#define CORPSE_WARNING_DIST 5

enum RottingCorpseDefines
{
	NO_CORPSE,
	SMERC_JFK,
	SMERC_BCK,
	SMERC_FWD,
	SMERC_DHD,
	SMERC_PRN,
	SMERC_WTR,
	SMERC_FALL,
	SMERC_FALLF,

	MMERC_JFK,
	MMERC_BCK,
	MMERC_FWD,
	MMERC_DHD,
	MMERC_PRN,
	MMERC_WTR,
	MMERC_FALL,
	MMERC_FALLF,

	FMERC_JFK,		// head has exploded
	FMERC_BCK,		// body lying backward
	FMERC_FWD,		// body lying forward
	FMERC_DHD,
	FMERC_PRN,		// body lying on the belly while looking forward, one leg to the side 
	FMERC_WTR,		// body lying in water
	FMERC_FALL,		// body lying backward, fallen from roof
	FMERC_FALLF,	// body lying forward, fallen from roof

	// CIVS
	M_DEAD1,
	K_DEAD1,
	H_DEAD1,
	FT_DEAD1,
	S_DEAD1,
	W_DEAD1,
	C_DEAD1,
	M_DEAD2,
	K_DEAD2,
	H_DEAD2,

	FT_DEAD2,
	S_DEAD2,
	W_DEAD2,
	C_DEAD2,
	BLOODCAT_DEAD,
	COW_DEAD,
	ADULTMONSTER_DEAD,
	INFANTMONSTER_DEAD,
	LARVAEMONSTER_DEAD,
	ROTTING_STAGE2,

	TANK1_DEAD,
	TANK2_DEAD,
	HUMMER_DEAD,
	ICECREAM_DEAD,
	QUEEN_MONSTER_DEAD,
	ROBOT_DEAD,
	BURNT_DEAD,
	EXPLODE_DEAD,
	CRYO_CORPSE,

	NUM_CORPSES,

} ;



#define		ROTTING_CORPSE_FIND_SWEETSPOT_FROM_GRIDNO				0x00000001		//Find the closest spot to the given gridno
#define		ROTTING_CORPSE_USE_NORTH_ENTRY_POINT					0x00000002		//Find the spot closest to the north entry grid
#define		ROTTING_CORPSE_USE_SOUTH_ENTRY_POINT					0x00000004		//Find the spot closest to the south entry grid
#define		ROTTING_CORPSE_USE_EAST_ENTRY_POINT						0x00000008		//Find the spot closest to the east entry grid
#define		ROTTING_CORPSE_USE_WEST_ENTRY_POINT						0x00000010		//Find the spot closest to the west entry grid
#define		ROTTING_CORPSE_USE_CAMO_PALETTE							0x00000020		//We use cammo palette here....
#define		ROTTING_CORPSE_VEHICLE									0x00000040		//Vehicle Corpse
#define		ROTTING_CORPSE_USE_STEALTH_PALETTE						0x00000080		//We use stealth palette here....
#define		ROTTING_CORPSE_USE_URBAN_CAMO_PALETTE					0x00000100		//We use urban palette here....
#define		ROTTING_CORPSE_USE_DESERT_CAMO_PALETTE					0x00000200		//We use desert palette here....
#define		ROTTING_CORPSE_USE_SNOW_CAMO_PALETTE					0x00000400		//We use snow palette here....

#define		ROTTING_CORPSE_NEVER_RISE_AGAIN							0x00000800		//a zombie cannot be created from this corpse (if not set, it'll eventually rise again )

// Flugente: corpses can now be gutted after they have been decapitated. Atm there is no corpse that can be both gutted and decapitated (to be done later)
#define		ROTTING_CORPSE_HEAD_TAKEN								0x00001000		// head has been taken off
#define		ROTTING_CORPSE_GUTTED									0x00002000		// corpse has been gutted
#define		ROTTING_CORPSE_NO_VEST									0x00004000		// corpse has no vest (atm not visually)
#define		ROTTING_CORPSE_NO_PANTS									0x00008000		// corpse has no vest (atm not visually)

#define		ROTTING_CORPSE_USE_XY_PROVIDED							0x00010000		// use the x,y coordinates provided in the definition when adding a corpse

typedef struct
{
	UINT8												ubType;
	UINT8												ubBodyType;
	INT32												sGridNo;
	FLOAT												dXPos;
	FLOAT												dYPos;
	INT16												sHeightAdjustment;

	PaletteRepID										HeadPal;			// Palette reps
	PaletteRepID										PantsPal;	
	PaletteRepID										VestPal;	
	PaletteRepID										SkinPal;	

	UINT8												ubDirection;
	UINT32												uiTimeOfDeath;

	UINT32												usFlags;

	INT8												bLevel;

	INT8												bVisible;
	INT8												bNumServicingCrows;
	UINT8												ubProfile;
	BOOLEAN												fHeadTaken;
	UINT8												ubAIWarningValue;

	UINT8												ubFiller[ 10 ];		// Flugente: 12 -> 10, because usFlags was cahnged from UINT16 to UINT32

	// Flugente: added name so we can display individual name if corpse gets resurrected...
	CHAR16												name[ 10 ];

} ROTTING_CORPSE_DEFINITION;


typedef struct
{
	ROTTING_CORPSE_DEFINITION		def;
	BOOLEAN											fActivated;

	ANITILE											*pAniTile;

	SGPPaletteEntry							*p8BPPPalette; 
	UINT16											*p16BPPPalette;
	UINT16											*pShades[ NUM_CORPSE_SHADES ]; 
	INT16												sGraphicNum;
	INT32												iCachedTileID;
	FLOAT												dXPos;
	FLOAT												dYPos;

	BOOLEAN											fAttractCrowsOnlyWhenOnScreen;
	INT32												iID;

	// Flugente: added name so we can display individual name if corpse gets resurrected...
	CHAR16												name[ 10 ];
		
} ROTTING_CORPSE;



INT32	AddRottingCorpse( ROTTING_CORPSE_DEFINITION *pCorpseDef );

void RemoveCorpse( INT32 iCorpseID );
void RemoveCorpses( );


BOOLEAN TurnSoldierIntoCorpse( SOLDIERTYPE *pSoldier, BOOLEAN fRemoveMerc, BOOLEAN fCheckForLOS );

INT16 FindNearestRottingCorpse( SOLDIERTYPE *pSoldier );

void AllMercsOnTeamLookForCorpse( ROTTING_CORPSE *pCorpse, INT8 bTeam );
void MercLooksForCorpses( SOLDIERTYPE *pSoldier );
void RebuildAllCorpseShadeTables( );

UINT16				CreateCorpsePaletteTables( ROTTING_CORPSE *pCorpse );

INT32 FindNearestAvailableGridNoForCorpse( ROTTING_CORPSE_DEFINITION *pCorpseDef, INT8 ubRadius );


void HandleRottingCorpses( );
void AddCrowToCorpse( ROTTING_CORPSE *pCorpse );

void VaporizeCorpse( INT32 sGridNo, INT8 asLevel, UINT16 usStructureID );
void CorpseHit( INT32 sGridNo, INT8 asLevel, UINT16 usStructureID );

void HandleCrowLeave( SOLDIERTYPE *pSoldier );

void HandleCrowFlyAway( SOLDIERTYPE *pSoldier );

// WDS - increase number of corpses
// HEADROCK HAM 3.6: Increase again (250->500).
#define		MAX_ROTTING_CORPSES		500

//extern ROTTING_CORPSE	gRottingCorpse[ MAX_ROTTING_CORPSES ];
extern std::vector<ROTTING_CORPSE> gRottingCorpse;
extern INT32					giNumRottingCorpse;
extern UINT8					gb4DirectionsFrom8[8];

ROTTING_CORPSE *GetCorpseAtGridNo( INT32 sGridNo , INT8 bLevel );
BOOLEAN IsValidDecapitationCorpse( ROTTING_CORPSE *pCorpse );
BOOLEAN DecapitateCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel );

// Flugente: move all corpses on sGridNo level bLevel to level 1 - bLevel
void ShiftCorpse( INT32 sGridNo, INT8 bLevel );

// Flugente: actions for the covert opts trait
BOOLEAN IsValidGutCorpse( ROTTING_CORPSE *pCorpse );
BOOLEAN GutCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel );
BOOLEAN IsValidStripCorpse( ROTTING_CORPSE *pCorpse );
BOOLEAN StripCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel );
BOOLEAN IsValidTakeCorpse( ROTTING_CORPSE *pCorpse );
BOOLEAN TakeCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel );

BOOLEAN AddCorpseFromObject(OBJECTTYPE* pObj, INT32 sGridNo, INT8 bLevel );

void GetBloodFromCorpse( SOLDIERTYPE *pSoldier );

UINT16 GetCorpseStructIndex( ROTTING_CORPSE_DEFINITION *pCorpseDef, BOOLEAN fForImage );

void LookForAndMayCommentOnSeeingCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubLevel );

INT32 GetGridNoOfCorpseGivenProfileID( UINT8 ubProfileID );

void DecayRottingCorpseAIWarnings( void );
UINT8 GetNearestRottingCorpseAIWarning( INT32 sGridNo );

ROTTING_CORPSE* GetRottingCorpse( INT16 aNum );
std::vector<INT16> GetCorpseIDsNearGridNo( INT32 sGridNo, INT8 bLevel, INT8 sRadius );

// Flugente: Raise zombies
void RaiseZombies( void );

// Flugente: create a zombie from a corpse
void CreateZombiefromCorpse( ROTTING_CORPSE *	pCorpse, UINT16 usAnimState );

// Flugente: returns true if a zombie can be raised from this corpse, and returns the correct pAnimState for the new zombie
BOOLEAN CorpseOkToSpawnZombie( ROTTING_CORPSE *	pCorpse, UINT16* pAnimState  );

// Flugente: can we take the clothes of this corpse?
BOOLEAN CorpseOkToDress( ROTTING_CORPSE* pCorpse );

// Flugente: how rotten is this corpse? values from 0 to 1, 1 as soon as it is rotting
FLOAT GetCorpseRotFactor( ROTTING_CORPSE* pCorpse );

void CheckForZombieMusic();

#endif
