#ifndef __LIGHT_EFFECTS
#define __LIGHT_EFFECTS


/*
// Light effect types
enum
{
	NO_LIGHT_EFFECT,
	LIGHT_FLARE_MARK_1,
};
*/

typedef struct
{
	INT32 sGridNo;          // gridno at which the tear gas cloud is centered

	UINT8		ubDuration;			// the number of turns will remain effective
	UINT8		bRadius;			// the current radius
	UINT8		bAge;					 // the number of turns light has been around
	BOOLEAN	fAllocated;
	INT8		bType;
	INT32		iLight;
	UINT32	uiTimeOfLastUpdate;

	// Flugente: added variables necessary for dynamic (flash-)lighting
	INT32		flags;
	UINT8		ubOwner;
	
} LIGHTEFFECT;

#define LIGHTEFFECT_OLDSIZE				20			// Flugente: the old size of LIGHTEFFECT (before Gameversion DYNAMIC_FLASHLIGHTS)
#define	LIGHTEFFECT_FLASHLIGHT			0x00000001


// Decays all light effects...
void DecayLightEffects( UINT32 uiTime );

// Add light to gridno
// ( Replacement algorithm uses distance away )
void AddLightEffectToTile( INT8 bType, INT32 sGridNo );

void RemoveLightEffectFromTile( INT32 sGridNo );

INT32 NewLightEffect( INT32 sGridNo, UINT8 ubDuration, UINT8 ubStartRadius );


BOOLEAN SaveLightEffectsToSaveGameFile( HWFILE hFile );
BOOLEAN LoadLightEffectsFromLoadGameFile( HWFILE hFile );

BOOLEAN SaveLightEffectsToMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );
BOOLEAN LoadLightEffectsFromMapTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );
void ResetLightEffects();

BOOLEAN IsLightEffectAtTile( INT32 sGridNo );

// Flugente: create and destroy light sources tied to a person
void CreatePersonalLight( INT32 sGridNo, UINT8 ubID );
void RemovePersonalLights( UINT8 ubID );

#endif