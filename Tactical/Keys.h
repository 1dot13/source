#ifndef		_KEYS_H_
#define		_KEYS_H_

#include "types.h"
#include "FileMan.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


typedef struct
{
	UINT16		usItem;						// index in item table for key
	UINT8			fFlags;						// flags...
	UINT16		usSectorFound;		// where and
	UINT16		usDateFound;			// when the key was found
} KEY;


#define KEY_USED				0x01

#define LOCK_UNOPENABLE	255
#define NO_KEY					255

#define MAX_KEYS_PER_LOCK 4

#define LOCK_REGULAR			1
#define LOCK_PADLOCK			2
#define LOCK_CARD					3
#define LOCK_ELECTRONIC		4
#define LOCK_SPECIAL			5

/*
typedef struct
{
	UINT8		ubKeyID[MAX_KEYS_PER_LOCK];
	UINT8		ubLockType;							// numeric lock type value... easier to use than flags!
	INT8		bPickSkillAdjustment;		// difficulty to pick a lock which takes this key
	INT8		bSmashSkillAdjustment;	// the strength of the lock (resistance to smashing)
} LOCK;
*/

#define MAXLOCKDESCLENGTH 40 
typedef struct
{
	UINT8		ubEditorName[ MAXLOCKDESCLENGTH ];	// name to display in editor
	UINT16	usKeyItem;													// key for this door uses which graphic (item #)?
	UINT8		ubLockType;													// regular, padlock, electronic, etc
	UINT8		ubPickDifficulty;										// difficulty to pick such a lock
	UINT8		ubSmashDifficulty;										// difficulty to smash such a lock
	UINT8		ubFiller;
} LOCK;

// Defines below for the perceived value of the door
#define DOOR_PERCEIVED_UNKNOWN		0
#define DOOR_PERCEIVED_LOCKED			1
#define DOOR_PERCEIVED_UNLOCKED		2
#define DOOR_PERCEIVED_BROKEN			3

#define DOOR_PERCEIVED_TRAPPED		1
#define DOOR_PERCEIVED_UNTRAPPED	2

//dnl ch42 250909
// WANNE - BMP: DONE!
typedef struct
{
	INT16			sGridNo;
	BOOLEAN		fLocked;							// is the door locked
	UINT8			ubTrapLevel;					// difficulty of finding the trap, 0-10
	UINT8			ubTrapID;							// the trap type (0 is no trap)
	UINT8			ubLockID;							// the lock (0 is no lock)
	INT8			bPerceivedLocked;			// The perceived lock value can be different than the fLocked.
																	// Values for this include the fact that we don't know the status of
																	// the door, etc
	INT8			bPerceivedTrapped;		// See above, but with respect to traps rather than locked status
	INT8			bLockDamage;					// Damage to the lock
	INT8			bPadding[4];					// extra bytes
} _OLD_DOOR;

class DOOR
{
public:
	INT32			sGridNo;
	BOOLEAN			fLocked;				// is the door locked
	UINT8			ubTrapLevel;			// difficulty of finding the trap, 0-10
	UINT8			ubTrapID;				// the trap type (0 is no trap)
	UINT8			ubLockID;				// the lock (0 is no lock)
	INT8			bPerceivedLocked;		// The perceived lock value can be different than the fLocked.
	// Values for this include the fact that we don't know the status of the door, etc
	INT8			bPerceivedTrapped;		// See above, but with respect to traps rather than locked status
	INT8			bLockDamage;			// Damage to the lock
public:
//	DOOR();
//	~DOOR();
	DOOR& operator=(const _OLD_DOOR& src);
	BOOLEAN Load(INT8** hBuffer, FLOAT dMajorMapVersion);
	BOOLEAN Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
};

typedef enum
{
	NO_TRAP = 0,
	EXPLOSION,
	ELECTRIC,
	SIREN,
	SILENT_ALARM,
	BROTHEL_SIREN,
	SUPER_ELECTRIC,
	NUM_DOOR_TRAPS
} DoorTrapTypes;

#define DOOR_TRAP_STOPS_ACTION		0x01
#define DOOR_TRAP_RECURRING				0x02
#define DOOR_TRAP_SILENT					0x04


typedef struct
{
	UINT8		fFlags;									// stops action?	recurring trap?
} DOORTRAP;


//The status of the door, either open or closed
#define		DOOR_OPEN								0x01
#define		DOOR_PERCEIVED_OPEN			0x02
#define		DOOR_PERCEIVED_NOTSET		0x04
#define		DOOR_BUSY								0x08
#define		DOOR_HAS_TIN_CAN				0x10


#define	DONTSETDOORSTATUS			2

// WANNE - BMP: Maybe we also need here an "_OLD_DOOR_STATUS" struct
// but this does not exist in BMP code!
typedef struct
{
	INT32 sGridNo;
	UINT8		ubFlags;

} DOOR_STATUS;



// This is the number of different types of doors we can have 
// in one map at a time...

#define NUM_KEYS 255
#define NUM_KEYS_OLD 64		// anv: increased max number, kept old so correct number can be read from save files

#define NUM_LOCKS 255
#define NUM_LOCKS_OLD 64	// anv: increased max number, kept old so correct number can be read from LOCKS.BIN
#define INVALID_KEY_NUMBER 255

#define ANYKEY									252
#define AUTOUNLOCK							253
#define OPENING_NOT_POSSIBLE		254

extern KEY KeyTable[NUM_KEYS];
extern LOCK LockTable[NUM_LOCKS];
extern DOORTRAP DoorTrapTable[NUM_DOOR_TRAPS];

extern BOOLEAN AddKeysToKeyRing( SOLDIERTYPE *pSoldier, UINT8 ubKeyID, UINT8 ubNumber );
extern BOOLEAN RemoveKeyFromKeyRing( SOLDIERTYPE *pSoldier, UINT8 ubPos, OBJECTTYPE * pObj );
extern BOOLEAN RemoveAllOfKeyFromKeyRing( SOLDIERTYPE *pSoldier, UINT8 ubPos, OBJECTTYPE * pObj );
extern BOOLEAN KeyExistsInInventory( SOLDIERTYPE *pSoldier, UINT8 ubKeyID );
extern BOOLEAN KeyExistsInKeyRing( SOLDIERTYPE *pSoldier, UINT8 ubKeyID, UINT8 * pubPos );
extern BOOLEAN SoldierHasKey( SOLDIERTYPE *pSoldier, UINT8 ubKeyID );

extern STR16 sKeyDescriptionStrings[];
/**********************************
 * Door utils add by Kris Morness *
 **********************************/

//Dynamic array of Doors.	For general game purposes, the doors that are locked and/or trapped
//are permanently saved within the map, and are loaded and allocated when the map is loaded.	Because
//the editor allows more doors to be added, or removed, the actual size of the DoorTable may change.
extern DOOR *DoorTable;

//Current number of doors in world.
extern UINT8 gubNumDoors;
//Current max number of doors.	This is only used by the editor.	When adding doors to the 
//world, we may run out of space in the DoorTable, so we will allocate a new array with extra slots,
//then copy everything over again.	gubMaxDoors holds the arrays actual number of slots, even though
//the current number (gubNumDoors) will be <= to it.
extern UINT8 gubMaxDoors;
//dnl ch42 250909
// File I/O for loading the door information from the map. This automatically allocates the exact number of slots when loading.
void LoadDoorTableFromMap(INT8** hBuffer, FLOAT dMajorMapVersion);
// Saves the existing door information to the map. Before it actually saves, it'll verify that the door still exists. Otherwise, it'll ignore it. It is possible in the editor to delete doors in many different ways, so I opted to put it in the saving routine.
void SaveDoorTableToMap(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
//The editor adds locks to the world.	If the gridno already exists, then the currently existing door
//information is overwritten.
extern void AddDoorInfoToTable( DOOR *pDoor );
//When the editor removes a door from the world, this function looks for and removes accompanying door
//information.	If the entry is not the last entry, the last entry is move to it's current slot, to keep
//everything contiguous.
extern void RemoveDoorInfoFromTable( INT32 iMapIndex );
//This is the link to see if a door exists at a gridno.	
DOOR * FindDoorInfoAtGridNo( INT32 iMapIndex );
//Upon world deallocation, the door table needs to be deallocated.
extern void TrashDoorTable();

BOOLEAN AttemptToUnlockDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN AttemptToLockDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN AttemptToSmashDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN AttemptToPickLock( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN AttemptToBlowUpLock( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN AttemptToUntrapDoor( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN ExamineDoorForTraps( SOLDIERTYPE * pSoldier, DOOR * pDoor );
BOOLEAN HasDoorTrapGoneOff( SOLDIERTYPE * pSoldier, DOOR * pDoor );
void HandleDoorTrap( SOLDIERTYPE * pSoldier, DOOR * pDoor );


// Updates the perceived value to the user of the state of the door
void UpdateDoorPerceivedValue( DOOR *pDoor );


//Saves the Door Table array to the temp file
BOOLEAN	SaveDoorTableToDoorTableTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

//Load the door table from the temp file
BOOLEAN		LoadDoorTableFromDoorTableTempFile( );



//	Adds a door to the Door status array.	As the user comes across the door, they are added.
//	if the door already exists, nothing happens
// fOpen is True if the door is to be initially open, false if it is closed
// fInitiallyPercieveOpen is true if the door is to be initially open, else false
BOOLEAN ModifyDoorStatus( INT32 sGridNo, BOOLEAN fOpen, BOOLEAN fInitiallyPercieveOpen );

//Deletes the door status array
void TrashDoorStatusArray( );

// Returns true if the door is open, otherwise false
BOOLEAN	IsDoorOpen( INT32 sGridNo );

//Returns true if the door is perceioved as open
BOOLEAN	IsDoorPerceivedOpen( INT32 sGridNo );

// Saves the Door Status array to the MapTempfile
BOOLEAN SaveDoorStatusArrayToDoorStatusTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

//Load the door status from the door status temp file
BOOLEAN LoadDoorStatusArrayFromDoorStatusTempFile();

//Modify the Doors open status
BOOLEAN	SetDoorOpenStatus( INT32 sGridNo, BOOLEAN fOpen );

//Modify the doors perceived open status
BOOLEAN	SetDoorPerceivedOpenStatus( INT32 sGridNo, BOOLEAN fPerceivedOpen );


//Save the key table to the saved game file
BOOLEAN SaveKeyTableToSaveGameFile( HWFILE hFile );

//Load the key table from the saved game file
BOOLEAN LoadKeyTableFromSaveedGameFile( HWFILE hFile, UINT32 uiSaveGameVersion );

// Returns a doors status value, NULL if not found
DOOR_STATUS	*GetDoorStatus( INT32 sGridNo );

BOOLEAN UpdateDoorStatusPerceivedValue( INT32 sGridNo );

BOOLEAN AllMercsLookForDoor( INT32 sGridNo, BOOLEAN fUpdateValue );

BOOLEAN MercLooksForDoors( SOLDIERTYPE *pSoldier, BOOLEAN fUpdateValue );

void UpdateDoorGraphicsFromStatus( BOOLEAN fUsePerceivedStatus, BOOLEAN fDirty );

BOOLEAN AttemptToCrowbarLock( SOLDIERTYPE * pSoldier, DOOR * pDoor );

BOOLEAN LoadLockTable( void );

void ExamineDoorsOnEnteringSector( );

void HandleDoorsChangeWhenEnteringSectorCurrentlyLoaded( );

void AttachStringToDoor( INT32 sGridNo );

void DropKeysInKeyRing( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel, INT8 bVisible, BOOLEAN fAddToDropList, INT32 iDropListSlot, BOOLEAN fUseUnLoaded );

#endif