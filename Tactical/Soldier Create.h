#ifndef _SOLDIER_CREATE_H
#define _SOLDIER_CREATE_H

#include "Animation Cache.h"
#include "Timer Control.h"
#include "vobject.h"
#include "Overhead Types.h"
//#include "Item Types.h"
#include "soldier control.h"

#include <vector>
#include <iterator>

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
class SOLDIERCREATE_STRUCT;//dnl ch33 130909
class _OLD_SOLDIERCREATE_STRUCT;//dnl ch42 250909

#define		SOLDIER_CREATE_AUTO_TEAM			-1

// WDS - make number of mercenaries, etc. be configurable
//Kris: 
//This value is the total maximum number of slots in a map.	
//Players		20
//Enemies		32
//Creatures 32
//Rebels		32
//Civilians 32
//Total			148
#define MAX_INDIVIDUALS											TOTAL_SOLDIERS

enum 
{ 
	PINKSKIN, 
	TANSKIN, 
	DARKSKIN, 
	BLACKSKIN, 
	NUMSKINS
};

enum
{
	BROWNHEAD,						//dark skin (this line plus all above)
	BLACKHEAD, //black skin (only this line )
	WHITEHEAD,
	BLONDEHEAD,
	REDHEAD,	//pink/tan skin (this line plus all above )
	NUMHEADS
};

enum
{
	WHITEVEST,
	GYELLOWSHIRT,
	YELLOWVEST,
	GREYVEST,
	BROWNVEST,
	PURPLESHIRT,
	BLUEVEST,
	JEANVEST,
	GREENVEST,
	REDVEST,
	BLACKSHIRT,
	NUMSHIRTS
};

enum
{
	BLUEPANTS,
	BLACKPANTS,
	JEANPANTS,
	TANPANTS,
	BEIGEPANTS,
	GREENPANTS,
	NUMPANTS
};

//Kris:	SERIALIZING INFORMATION
//All maps must have:
//	-MAPCREATE_STRUCT
//		MAPCREATE_STRUCT.ubNumIndividuals determines how many BASIC_SOLDIERCREATE_STRUCTs there are
//	-The BASIC_SOLDIERCREATE_STRUCTS are saved contiguously, but if any of them
//		fDetailedPlacement set, then there is a SOLDIERCREATE_STRUCT saved immediately after.

//These are the placement slots used by the editor to define where characters are in a map, what 
//they are, what team they are on, personality traits, etc.	The Merc section of the editor is 
//what is used to define these values.
//dnl ch42 250909
// WANNE - BMP: DONE!
typedef struct
{
	BOOLEAN fDetailedPlacement;			//Specialized information.	Has a counterpart containing all info.
	INT16 sStartingGridNo;				//Where the placement position is.
	INT8 bTeam;											//The team this individual is part of.
	INT8 bRelativeAttributeLevel;		
	INT8 bRelativeEquipmentLevel;		
	UINT8 ubDirection;								//1 of 8 values (always mandatory)
	INT8 bOrders;										
	INT8 bAttitude;									
	INT8 ubBodyType;									//up to 128 body types, -1 means random
	INT16 sPatrolGrid[OLD_MAXPATROLGRIDS];//dnl ch27 230909 Possible locations to visit, patrol, etc.
	INT8 bPatrolCnt;
	BOOLEAN fOnRoof;
	UINT8	ubSoldierClass;							//army, administrator, elite
	UINT8 ubCivilianGroup;
	BOOLEAN fPriorityExistance;			//These slots are used first
	BOOLEAN fHasKeys;
	INT8 PADDINGSLOTS[ 14 ];
} _OLD_BASIC_SOLDIERCREATE_STRUCT; //50 bytes

class BASIC_SOLDIERCREATE_STRUCT
{
public:
	BOOLEAN fDetailedPlacement;			// Specialized information, has a counterpart containing all info
	INT32 usStartingGridNo;				// Where the placement position is
	INT8 bTeam;							// The team this individual is part of
	INT8 bRelativeAttributeLevel;
	INT8 bRelativeEquipmentLevel;
	UINT8 ubDirection;					// 1 of 8 values (always mandatory)
	INT8 bOrders;
	INT8 bAttitude;
	INT8 ubBodyType;						// Up to 128 body types, -1 means random
	INT32 sPatrolGrid[MAXPATROLGRIDS];	// Possible locations to visit, patrol, etc.
	INT8 bPatrolCnt;
	BOOLEAN fOnRoof;
	UINT8 ubSoldierClass;				// Army, administrator, elite
	UINT8 ubCivilianGroup;
	BOOLEAN fPriorityExistance;			// These slots are used first
	BOOLEAN fHasKeys;
public:
	BASIC_SOLDIERCREATE_STRUCT& operator=(const _OLD_BASIC_SOLDIERCREATE_STRUCT& src);
	BOOLEAN Load(INT8** hBuffer, FLOAT dMajorMapVersion);
	BOOLEAN Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
};

class OLD_SOLDIERCREATE_STRUCT_101
{
public:
	// Constructor
	OLD_SOLDIERCREATE_STRUCT_101();
	// Copy Constructor
	OLD_SOLDIERCREATE_STRUCT_101(const OLD_SOLDIERCREATE_STRUCT_101&);
	// Assignment operator
	OLD_SOLDIERCREATE_STRUCT_101& operator=(const OLD_SOLDIERCREATE_STRUCT_101&);
	OLD_SOLDIERCREATE_STRUCT_101& operator=(SOLDIERCREATE_STRUCT&);//dnl ch33 130909
	// Destructor
	~OLD_SOLDIERCREATE_STRUCT_101();

	// Initialize the soldier.	
	//	Use this instead of the old method of calling memset!
	//	Note that the constructor does this automatically.
	void initialize();

	// Note!	These two functions should ONLY be used either just before saving to a
	// file (NewToOld) or after loading a file (OldToNew).
	void CopyOldInventoryToNew();

public:
	//Bulletproofing so static detailed placements aren't used to tactically create soldiers.
	//Used by editor for validation purposes.
	BOOLEAN						fStatic;	
	
	//Profile information used for special NPCs and player mercs.
	UINT8							ubProfile;
	BOOLEAN						fPlayerMerc;
	BOOLEAN						fPlayerPlan;
	BOOLEAN						fCopyProfileItemsOver;

	//Location information
	INT16							sSectorX;
	INT16							sSectorY;
	UINT8							ubDirection;
	INT16							sInsertionGridNo;

	// Can force a team, but needs flag set
	INT8							bTeam;
	INT8							ubBodyType;

	//Orders and attitude settings
	INT8							bAttitude;
	INT8							bOrders;

	//Attributes
	INT8							bLifeMax;	
	INT8							bLife;
	INT8							bAgility;
	INT8							bDexterity;
	INT8							bExpLevel;
	INT8							bMarksmanship;
	INT8							bMedical;
	INT8							bMechanical;
	INT8							bExplosive;
	INT8							bLeadership;
	INT8							bStrength;
	INT8							bWisdom;
	INT8							bMorale;
	INT8							bAIMorale;

private:
	//Inventory
	OLD_OBJECTTYPE_101				DO_NOT_USE_Inv[ OldInventory::NUM_INV_SLOTS ];	
public:
	
	//Palette information for soldiers.
	PaletteRepID			HeadPal;	
	PaletteRepID			PantsPal;	
	PaletteRepID			VestPal;	
	PaletteRepID			SkinPal;	
	PaletteRepID			MiscPal;
	
	//Waypoint information for patrolling
	INT16 sPatrolGrid[OLD_MAXPATROLGRIDS];//dnl ch33 200909
	INT8 bPatrolCnt;
	
	//Kris:	Additions November 16, 1997 (padding down to 129 from 150)
	BOOLEAN						fVisible;
	CHAR16						name[ 10 ];

	UINT8							ubSoldierClass;	//army, administrator, elite

	BOOLEAN						fOnRoof;

	INT8							bSectorZ;

	SOLDIERTYPE				*pExistingSoldier;
	BOOLEAN						fUseExistingSoldier;
	UINT8							ubCivilianGroup;

	BOOLEAN						fKillSlotIfOwnerDies;
	UINT8							ubScheduleID;

	BOOLEAN						fUseGivenVehicle;				
	INT8							bUseGivenVehicleID;				
	BOOLEAN						fHasKeys;

	INT8 bPadding[115];

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//
	char ef1,ef2;	// Extra filler to get "offsetof(endOfPOD)" to match SIZEOF(oldstruct)

	char endOfPOD;	// marker for end of POD (plain old data)

	Inventory				Inv;
}; // OLD_SOLDIERCREATE_STRUCT_101;

// WANNE - BMP: DONE!
class SOLDIERCREATE_STRUCT
{
public:
	// Constructor
	SOLDIERCREATE_STRUCT();
	// Copy Constructor
	SOLDIERCREATE_STRUCT(const SOLDIERCREATE_STRUCT&);
	// Assignment operator
	SOLDIERCREATE_STRUCT& operator=(const SOLDIERCREATE_STRUCT&);
	// Conversion operator from old to new
	SOLDIERCREATE_STRUCT& operator=(const OLD_SOLDIERCREATE_STRUCT_101&);
	// Conversion operator from SOLDIERTYPE to SOLDIERCREATE_STRUCT
	SOLDIERCREATE_STRUCT& operator=(const SOLDIERTYPE&);
	// Destructor
	~SOLDIERCREATE_STRUCT();

	UINT16 GetChecksum();

	SOLDIERCREATE_STRUCT& operator=(const _OLD_SOLDIERCREATE_STRUCT& src);//dnl ch42 250909

	// Initialize the soldier.	
	//	Use this instead of the old method of calling memset!
	//	Note that the constructor does this automatically.
	void initialize();

	BOOLEAN Load(HWFILE hFile, int versionToLoad, bool loadChecksum);
	BOOLEAN Load(INT8 **hBuffer, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
	BOOLEAN Save(HWFILE hFile, bool fSavingMap, FLOAT dMajorMapVersion=MAJOR_MAP_VERSION, UINT8 ubMinorMapVersion=MINOR_MAP_VERSION);//dnl ch42 250909

public:
	//Bulletproofing so static detailed placements aren't used to tactically create soldiers.
	//Used by editor for validation purposes.
	BOOLEAN						fStatic;	
	
	//Profile information used for special NPCs and player mercs.
	UINT8							ubProfile;
	BOOLEAN						fPlayerMerc;
	BOOLEAN						fPlayerPlan;
	BOOLEAN						fCopyProfileItemsOver;

	//Location information
	INT16							sSectorX;
	INT16							sSectorY;
	UINT8							ubDirection;
	INT32							sInsertionGridNo;//dnl ch42 290909

	// Can force a team, but needs flag set
	INT8							bTeam;
	INT8							ubBodyType;

	//Orders and attitude settings
	INT8							bAttitude;
	INT8							bOrders;

	//Attributes
	INT8							bLifeMax;	
	INT8							bLife;
	INT8							bAgility;
	INT8							bDexterity;
	INT8							bExpLevel;
	INT8							bMarksmanship;
	INT8							bMedical;
	INT8							bMechanical;
	INT8							bExplosive;
	INT8							bLeadership;
	INT8							bStrength;
	INT8							bWisdom;
	INT8							bMorale;
	INT8							bAIMorale;

public:
	
	//Palette information for soldiers.
	PaletteRepID			HeadPal;	
	PaletteRepID			PantsPal;	
	PaletteRepID			VestPal;	
	PaletteRepID			SkinPal;	
	PaletteRepID			MiscPal;
	
	//Waypoint information for patrolling
	INT32 sPatrolGrid[MAXPATROLGRIDS];//dnl ch42 290909
	INT8					bPatrolCnt;
	
	//Kris:	Additions November 16, 1997 (padding down to 129 from 150)
	BOOLEAN						fVisible;
	CHAR16						name[ 10 ];

	UINT8						ubSoldierClass;	//army, administrator, elite

	BOOLEAN						fOnRoof;

	INT8						bSectorZ;

	SOLDIERTYPE					*pExistingSoldier;
	BOOLEAN						fUseExistingSoldier;
	UINT8						ubCivilianGroup;

	BOOLEAN						fKillSlotIfOwnerDies;
	UINT8						ubScheduleID;

	BOOLEAN						fUseGivenVehicle;				
	INT8						bUseGivenVehicleID;				
	BOOLEAN						fHasKeys;

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//
	char endOfPOD;	// marker for end of POD (plain old data)

	Inventory				Inv;
}; // SOLDIERCREATE_STRUCT;

// WANNE - BMP: DONE!
class _OLD_SOLDIERCREATE_STRUCT
{
public:
	//Bulletproofing so static detailed placements aren't used to tactically create soldiers.
	//Used by editor for validation purposes.
	BOOLEAN						fStatic;	
	
	//Profile information used for special NPCs and player mercs.
	UINT8							ubProfile;
	BOOLEAN						fPlayerMerc;
	BOOLEAN						fPlayerPlan;
	BOOLEAN						fCopyProfileItemsOver;

	//Location information
	INT16							sSectorX;
	INT16							sSectorY;
	UINT8							ubDirection;
	INT16							sInsertionGridNo;

	// Can force a team, but needs flag set
	INT8							bTeam;
	INT8							ubBodyType;

	//Orders and attitude settings
	INT8							bAttitude;
	INT8							bOrders;

	//Attributes
	INT8							bLifeMax;	
	INT8							bLife;
	INT8							bAgility;
	INT8							bDexterity;
	INT8							bExpLevel;
	INT8							bMarksmanship;
	INT8							bMedical;
	INT8							bMechanical;
	INT8							bExplosive;
	INT8							bLeadership;
	INT8							bStrength;
	INT8							bWisdom;
	INT8							bMorale;
	INT8							bAIMorale;

public:
	
	//Palette information for soldiers.
	PaletteRepID			HeadPal;	
	PaletteRepID			PantsPal;	
	PaletteRepID			VestPal;	
	PaletteRepID			SkinPal;	
	PaletteRepID			MiscPal;
	
	//Waypoint information for patrolling
	INT16					sPatrolGrid[OLD_MAXPATROLGRIDS];//dnl ch27 230909
	INT8					bPatrolCnt;
	
	//Kris:	Additions November 16, 1997 (padding down to 129 from 150)
	BOOLEAN						fVisible;
	CHAR16						name[ 10 ];

	UINT8						ubSoldierClass;	//army, administrator, elite

	BOOLEAN						fOnRoof;

	INT8						bSectorZ;

	SOLDIERTYPE					*pExistingSoldier;
	BOOLEAN						fUseExistingSoldier;
	UINT8						ubCivilianGroup;

	BOOLEAN						fKillSlotIfOwnerDies;
	UINT8						ubScheduleID;

	BOOLEAN						fUseGivenVehicle;				
	INT8						bUseGivenVehicleID;				
	BOOLEAN						fHasKeys;

	//
	// New and OO stuff goes after here.	Above this point any changes will goof up reading from files.
	//
	char endOfPOD;	// marker for end of POD (plain old data)

	Inventory				Inv;
}; // _OLD_SOLDIERCREATE_STRUCT;

#define SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD offsetof( OLD_SOLDIERCREATE_STRUCT_101, endOfPOD )
#define SIZEOF_SOLDIERCREATE_STRUCT_POD offsetof( SOLDIERCREATE_STRUCT, endOfPOD )
#define _OLD_SIZEOF_SOLDIERCREATE_STRUCT_POD offsetof( _OLD_SOLDIERCREATE_STRUCT, endOfPOD )


//Original functions currently used throughout the game.
BOOLEAN TacticalRemoveSoldier( SoldierID usSoldierIndex );
BOOLEAN TacticalRemoveSoldierPointer( SOLDIERTYPE *pSoldier, BOOLEAN fRemoveVehicle );

INT8 CalcDifficultyModifier( UINT8 ubSoldierClass );

void RandomizeNewSoldierStats( SOLDIERCREATE_STRUCT *pCreateStruct );

//Kris:
//Modified return type from BOOLEAN to SOLDIERTYPE*
SOLDIERTYPE* TacticalCreateSoldier( SOLDIERCREATE_STRUCT *pCreateStruct, UINT16 * pubID );

//Randomly generated enemies used by strategic AI.
SOLDIERTYPE* TacticalCreateAdministrator();
SOLDIERTYPE* TacticalCreateEliteEnemy();
SOLDIERTYPE* TacticalCreateArmyTroop();
SOLDIERTYPE* TacticalCreateZombie();		// Flugente Zombies
SOLDIERTYPE* TacticalCreateMilitia( UINT8 ubMilitiaClass, INT16 sX, INT16 sY );	// Flugente: added sector coordinates
SOLDIERTYPE* TacticalCreateCreature( INT8 bCreatureBodyType );
SOLDIERTYPE* TacticalCreateEnemyTank();
SOLDIERTYPE* TacticalCreateEnemyJeep( );
SOLDIERTYPE* TacticalCreateEnemyRobot();

// Flugente: create an armed civilian
SOLDIERTYPE* TacticalCreateArmedCivilian( UINT8 usSoldierClass = SOLDIER_CLASS_ARMY );

SOLDIERTYPE* TacticalCreateCivilian( INT32 sGridNo, UINT8 usCivilianGroup, INT8 usTraderID,
									 INT8 sBodyType, INT8 aVest, INT8 aPants, INT8 aHair, INT8 aSkin, 
									 INT16 sItem1, INT16 sItem2, INT16 sItem3, INT16 sItem4 );

// Flugente: assassins are elite soldiers of the civ team that go hostile on a certain event, otherwise they just blend in
SOLDIERTYPE* TacticalCreateEnemyAssassin(UINT8 disguisetype);

SOLDIERTYPE* TacticalCreateBandit();

void CreateAssassin(UINT8 disguisetype);

// create a prisoner (in a prison cell) in the current sector
void CreatePrisonerOfWar();

// create a downed pilot in a random location in the current sector
void CreateDownedPilot();

// randomly generates a relative level rating (attributes or equipment)
void RandomizeRelativeLevel( INT8 *pbRelLevel, UINT8 ubSoldierClass );

// get the pythag. distance from the passed sector to the palace..
UINT8 GetPythDistanceFromPalace( INT16 sSectorX, INT16 sSectorY );


//These following functions are currently used exclusively by the editor.
//Now, this will be useful for the strategic AI.
//Definitions:
//Soldier (s):	Currently in the game, but subject to modifications.	The editor has the capability to 
//	modify soldier attributes on the fly for testing purposes.
//BasicPlacement (bp):	A BASIC_SOLDIERCREATE_STRUCT that contains compact, very general, information about 
//	a soldier.	The BasicPlacement is then used to generate a DetailedPlacement before creating a soldier.	
//	Most of the soldiers saved in the maps will be saved in this manner.
//DetailedPlacement (pp):	A SOLDIERCREATE_STRUCT ready to be passed to TacticalCreateSoldier to generate 
//	and add a new soldier to the world.	The DetailedPlacement contains all of the necessary information
//	to do this.	This information won't be saved in maps. In most cases, only very few attributes are static, 
//	and the rest are generated at runtime.	Because of this situation, saved detailed placements must be in a 
//	different format.
//StaticDetailedPlacement (spp):	A hybrid version of the DetailedPlacement.	This is the information saved in 
//	the map via the editor.	When loaded, this information is converted to a normal detailed placement, but 
//	must also use the BasicPlacement information to complete this properly.	Once the conversion is complete, 
//	the static information is lost.	This gives us complete flexibility.	The basic placements contain relative
//	values that work in conjunction with the strategic AI's relative values to generate soldiers.	In no
//	circumstances will static detailed placements be used outside of the editor.	Note, that this hybrid version
//	uses the identical structure as detailed placements.	All non-static values are set to -1.

//Used to generate a detailed placement from a basic placement.	This assumes that the detailed placement
//doesn't exist, meaning there are no static attributes.	This is called when you wish to convert a basic 
//placement into a detailed placement just before creating a soldier.
// Flugente: added sector coordinates, used for militia using sector equipment
void CreateDetailedPlacementGivenBasicPlacementInfo( SOLDIERCREATE_STRUCT *pp, BASIC_SOLDIERCREATE_STRUCT *bp, INT16 sX = -1, INT16 sY = -1 );

//Used exclusively by the editor when the user wishes to change a basic placement into a detailed placement.
//Because the intention is to make some of the attributes static, all of the information that can be generated
//are defaulted to -1.	When an attribute is made to be static, that value in replaced by the new static value.
//This information is NOT compatible with TacticalCreateSoldier.	Before doing so, you must first convert the
//static detailed placement to a regular detailed placement.	
void CreateStaticDetailedPlacementGivenBasicPlacementInfo( SOLDIERCREATE_STRUCT *spp, BASIC_SOLDIERCREATE_STRUCT *bp );

//When you are ready to generate a soldier with a static detailed placement slot, this function will generate
//the proper priority placement slot given the static detailed placement and it's accompanying basic placment.
//For the purposes of merc editing, the static detailed placement is preserved.
// Flugente: added sector coordinates, used for militia using sector equipment
void CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo( 
		SOLDIERCREATE_STRUCT *pp, SOLDIERCREATE_STRUCT *spp, BASIC_SOLDIERCREATE_STRUCT *bp, INT16 sX = -1, INT16 sY = -1);

//Used to update a existing soldier's attributes with the new static detailed placement info.	This is used
//by the editor upon exiting the editor into the game, to update the existing soldiers with new information.
//This gives flexibility of testing mercs.	Upon entering the editor again, this call will reset all the 
//mercs to their original states.
void UpdateSoldierWithStaticDetailedInformation( SOLDIERTYPE *s, SOLDIERCREATE_STRUCT *spp );

//In the case of setting a profile ID in order to extract a soldier from the profile array, we
//also want to copy that information to the static detailed placement, for editor viewing purposes.
void UpdateStaticDetailedPlacementWithProfileInformation( SOLDIERCREATE_STRUCT *spp, UINT8 ubProfile );

//When the editor modifies the soldier's relative attribute level,
//this function is called to update that information.
void ModifySoldierAttributesWithNewRelativeLevel( SOLDIERTYPE *s, INT8 bLevel );

// Force the soldier to be a different ID
void ForceSoldierProfileID( SOLDIERTYPE *pSoldier, UINT8 ubProfileID );

void GeneratePaletteForSoldier( SOLDIERTYPE *pSoldier, UINT8 ubSoldierClass, UINT8 ubTeam );

void QuickCreateProfileMerc( INT8 bTeam, UINT8 ubProfileID );

BOOLEAN InternalTacticalRemoveSoldier( SoldierID usSoldierIndex, BOOLEAN fRemoveVehicle );

//SPECIAL!	Certain events in the game can cause profiled NPCs to become enemies.	The two cases are 
//adding Mike and Iggy.	We will only add one NPC in any given combat and the conditions for setting
//the associated facts are done elsewhere.	The function will set the profile for the SOLDIERCREATE_STRUCT
//and the rest will be handled automatically so long the ubProfile field doesn't get changed.
//NOTE:	We don't want to add Mike or Iggy if this is being called from autoresolve!
void OkayToUpgradeEliteToSpecialProfiledEnemy( SOLDIERCREATE_STRUCT *pp );
extern BOOLEAN gfProfiledEnemyAdded; //needs to be saved (used by the above function)

//Legion by Jazz
//void BeginSoldierClimbUpOkno( SOLDIERTYPE *pSoldier );
void BeginSoldierFenceUp( SOLDIERTYPE *pSoldier );
//dnl ch27 240909
typedef enum
{
	UINT8_UINT8,
	UINT16_UINT16,
	UINT32_UINT32,
	UINT8_UINT16,
	UINT16_UINT8,
	INT16_INT32,
	INT32_INT16,
}TranslateArrayFieldsCommands;
BOOLEAN TranslateArrayFields(void* out, const void* inp, int len, int cmd);

//dnl ch56 141009
#define CENTRAL_GRIDNO (MAPROWCOLTOPOS(WORLD_ROWS/2,WORLD_COLS/2))
#define CENTRAL_RADIUS (min(WORLD_ROWS,WORLD_COLS)/5)

void ResetNumSquadleadersInArmyGroup( void ); // added by SANDRO
BOOLEAN AssignTraitsToSoldier( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct ); // Added a function to add traits to soldiers - SANDRO

INT32 ChooseHairColor( UINT8 usBodyType, INT32 skin );

// Flugente: set palettes for vest/shirt
void SetClothes( SOLDIERTYPE* pSoldier, INT8 aVest, INT8 aPants, INT8 aHair = -1, INT8 aSkin = -1 );

#endif
