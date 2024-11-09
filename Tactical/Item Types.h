#ifndef ITEM_TYPES_H
#define ITEM_TYPES_H

#include "types.h"
#include <vector>
#include <list>
// THE_BOB : added for pocket popup definitions
#include <map>
#include "popup_definition.h"

//if the number of slots are ever changed, the loading / saving checksum should use this value to make conversion easier
#define NUM_ORIGINAL_INV_SLOTS 19


// I don't care if this isn't intuitive!	The hand positions go right
// before the big pockets so we can loop through them that way. --CJC
#define NO_SLOT -1

// NOTE NOTE NOTE!	Leave this alone until it is no longer needed.	It must match the
// original definition so old files can be read.
namespace OldInventory {
	enum {
		HELMETPOS = 0,
		VESTPOS,
		LEGPOS,
		HEAD1POS,
		HEAD2POS,
		HANDPOS,
		SECONDHANDPOS,
		BIGPOCK1POS,
		BIGPOCK2POS,
		BIGPOCK3POS,
		BIGPOCK4POS,
		SMALLPOCK1POS,
		SMALLPOCK2POS,
		SMALLPOCK3POS,
		SMALLPOCK4POS,
		SMALLPOCK5POS,
		SMALLPOCK6POS,
		SMALLPOCK7POS,
		SMALLPOCK8POS, // = 18, so 19 pockets needed

		NUM_INV_SLOTS = NUM_ORIGINAL_INV_SLOTS
	};
};

/* CHRISL: Added listings for each of the new inventory pockets.  Also split the enum so we could include
endpoint markers for each type (big, med, sml) of pocket. */
typedef enum INVENTORY_SLOT{
	HELMETPOS = 0,
	VESTPOS,
	LEGPOS,
	HEAD1POS,
	HEAD2POS,
	HANDPOS,
	SECONDHANDPOS,
	VESTPOCKPOS,
	LTHIGHPOCKPOS,
	RTHIGHPOCKPOS,
	CPACKPOCKPOS,
	BPACKPOCKPOS,
	GUNSLINGPOCKPOS,
	KNIFEPOCKPOS,
	BIGPOCK1POS,
	BIGPOCK2POS,
	BIGPOCK3POS,
	BIGPOCK4POS,
	BIGPOCK5POS,
	BIGPOCK6POS,
	BIGPOCK7POS,
	MEDPOCK1POS,
	MEDPOCK2POS,
	MEDPOCK3POS,
	MEDPOCK4POS,
	SMALLPOCK1POS,
	SMALLPOCK2POS,
	SMALLPOCK3POS,
	SMALLPOCK4POS,
	SMALLPOCK5POS,
	SMALLPOCK6POS,
	SMALLPOCK7POS,
	SMALLPOCK8POS,
	SMALLPOCK9POS,
	SMALLPOCK10POS,
	SMALLPOCK11POS,
	SMALLPOCK12POS,
	SMALLPOCK13POS,
	SMALLPOCK14POS,
	SMALLPOCK15POS,
	SMALLPOCK16POS,
	SMALLPOCK17POS,
	SMALLPOCK18POS,
	SMALLPOCK19POS,
	SMALLPOCK20POS,
	SMALLPOCK21POS,
	SMALLPOCK22POS,
	SMALLPOCK23POS,
	SMALLPOCK24POS,
	SMALLPOCK25POS,
	SMALLPOCK26POS,
	SMALLPOCK27POS,
	SMALLPOCK28POS,
	SMALLPOCK29POS,
	SMALLPOCK30POS,
	NUM_INV_SLOTS,
} INVENTORY_SLOT;

#define		INV_START_POS		0
#define		BODYPOSSTART		HELMETPOS
extern	int	BODYPOSFINAL		;//= GUNSLINGPOCKPOS;//RESET in initInventory
#define		BIGPOCKSTART		BIGPOCK1POS
extern	int	BIGPOCKFINAL		;//= MEDPOCK1POS;//RESET in initInventory
extern	int	MEDPOCKSTART		;//= MEDPOCK1POS;//RESET in initInventory
extern	int	MEDPOCKFINAL		;//= SMALLPOCK1POS;//RESET in initInventory
#define		SMALLPOCKSTART		SMALLPOCK1POS
extern	int	SMALLPOCKFINAL		;//= NUM_INV_SLOTS;//RESET in initInventory
#define		STACK_SIZE_LIMIT	NUM_INV_SLOTS

typedef enum ATTACHMENT_SLOT{
	ATTACHMENTPOINT1 = 0,
	ATTACHMENTPOINT2,
	ATTACHMENTPOINT3,
	ATTACHMENTPOINT4,
	ATTACHMENTPOINT5,
	ATTACHMENTPOINT6,
	ATTACHMENTPOINT7,
	ATTACHMENTPOINT8,
	ATTACHMENTPOINT9,
	ATTACHMENTPOINT10,
	ATTACHMENTPOINT11,
	ATTACHMENTPOINT12,
	ATTACHMENTPOINT13,
	ATTACHMENTPOINT14,
	ATTACHMENTPOINT15,
	ATTACHMENTPOINT16,
	ATTACHMENTPOINT17,
	ATTACHMENTPOINT18,
	ATTACHMENTPOINT19,
	ATTACHMENTPOINT20,
	ATTACHMENTPOINT21,
	ATTACHMENTPOINT22,
	ATTACHMENTPOINT23,
	ATTACHMENTPOINT24,
	ATTACHMENTPOINT25,
	ATTACHMENTPOINT26,
	ATTACHMENTPOINT27,
	ATTACHMENTPOINT28,
	ATTACHMENTPOINT29,
	ATTACHMENTPOINT30,
	ATTACHMENTPOINT31,
	NUM_ATTACH_SLOTS
} ATTACHMENT_SLOT;

#define INVALIDCURS 0
#define QUESTCURS 1
#define PUNCHCURS 2
#define TARGETCURS 3
#define KNIFECURS 4
#define AIDCURS 5
#define TOSSCURS 6
#define MINECURS 8
#define LPICKCURS 9
#define MDETECTCURS 10
#define CROWBARCURS 11
#define SURVCAMCURS 12
#define CAMERACURS 13
#define KEYCURS 14
#define SAWCURS 15
#define WIRECUTCURS	16
#define	REMOTECURS	17
#define BOMBCURS		18 // ( only calculated, not set item table )
#define	REPAIRCURS	19
#define TRAJECTORYCURS	20
#define	JARCURS			21
#define	TINCANCURS	22
#define REFUELCURS  23
#define FORTICURS 24
#define HANDCUFFCURS 25
#define APPLYITEMCURS 26
#define INTERACTIVEACTIONCURS 27
#define BLOODBAGCURS 28
#define SPLINTCURS 29

#define CAMERARANGE 10

#define CONDBUL 0
#define COND 0
#define SINGLE 0
#define LIQ 0
#define USAGE 0
#define BUCKS 0

#define ITEM_NOT_FOUND -1


#define USABLE          10      // minimum work% of items to still be usable

#define MAX_OBJECTS_PER_SLOT 255
#define MAX_ATTACHMENTS 32
#define MAX_DEFAULT_ATTACHMENTS 20
extern UINT16 LAST_SLOT_INDEX;
#define MAX_MONEY_PER_SLOT 20000

typedef enum
{
	BOMB_TIMED = 1,
	BOMB_REMOTE,
	BOMB_PRESSURE,
	BOMB_SWITCH
} DetonatorType;

#define FIRST_MAP_PLACED_FREQUENCY 50
#define PANIC_FREQUENCY 127
#define PANIC_FREQUENCY_2 126
#define PANIC_FREQUENCY_3 125

#define OBJECT_UNDROPPABLE					0x01
#define OBJECT_MODIFIED						0x02
#define OBJECT_AI_UNUSABLE					0x04
#define OBJECT_ARMED_BOMB					0x08
#define OBJECT_KNOWN_TO_BE_TRAPPED			0x10
#define OBJECT_DISABLED_BOMB				0x20
#define OBJECT_ALARM_TRIGGER				0x40
#define OBJECT_NO_OVERWRITE					0x80

#define GS_CARTRIDGE_IN_CHAMBER				0x01
#define GS_WEAPON_BEING_RELOADED			0x02

// Flugente: define for maximum temperature
#define OVERHEATING_MAX_TEMPERATURE			60000.0f

//forward declaration
class OBJECTTYPE;
class SOLDIERTYPE;

#define MAX_ITEMS_IN_LBE 12

//CHRISL:
class LBENODE
{
public:
	LBENODE() { initialize();};
	void	initialize() {inv.clear();};
	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	UINT32				lbeClass;
	UINT16				lbeIndex;
	UINT16				ubID;
	BOOLEAN				ZipperFlag;
	int					uniqueID;
	UINT32				uiNodeChecksum;
	char				endOfPOD;
	//compiler complains about too big an array since OBJECTTYPE's size is unknown at this time, because of forward declaration
	//OBJECTTYPE			inv[ITEMS_IN_LBE];
	std::vector<OBJECTTYPE> inv;
};
#define SIZEOF_LBENODE_POD (offsetof(LBENODE, endOfPOD))
bool	checkLBEArrayIntegrity(bool verbose=false); // BOB: checks all worldItems for missing LBE info

void	CreateLBE(OBJECTTYPE* pObj, UINT16 ubID, int numSubPockets);
bool	DestroyLBEIfEmpty(OBJECTTYPE* pObj, int stackIndex = 0);
void	DestroyLBE(OBJECTTYPE* pObj, int stackIndex = 0);
void	GetLBESlots(UINT32 LBEType, std::vector<INT8>& LBESlots);
void	MoveItemsInSlotsToLBE( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, LBENODE* pLBE, OBJECTTYPE* pObj);

// CHRISL:
BOOLEAN MoveItemsToActivePockets( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, UINT32 uiHandPos, OBJECTTYPE *pObj );
BOOLEAN	MoveItemToLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos );
BOOLEAN MoveItemFromLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj );
bool	IsSlotAnLBESlot(int slot);
bool	IsSlotASmallPocket(int slot);

extern	std::list<LBENODE>	LBEArray;


//do not alter or saves will break, create new defines if the size changes
#define OLD_MAX_ATTACHMENTS_101 4
#define OLD_MAX_OBJECTS_PER_SLOT_101 8

// HEADROCK HAM 5: Define Self-Transformations
// Have to define this early on, because this struct is used in OBJECTTYPE
#define MAX_NUM_TRANSFORMATION_RESULTS 10

typedef struct
{
	UINT16 usItem;
	UINT16 usResult[MAX_NUM_TRANSFORMATION_RESULTS];
	UINT16 usAPCost;
	INT32 iBPCost;
	CHAR16 szMenuRowText[50];
	CHAR16 szTooltipText[300];
} TransformInfoStruct;

namespace Version101
{
	//union was originally unnamed
	union OLD_OBJECTTYPE_101_UNION
	{
		struct
		{
			INT8		bGunStatus;			// status % of gun
			UINT8		ubGunAmmoType;	// ammo type, as per weapons.h
			UINT8		ubGunShotsLeft;	// duh, amount of ammo left
			UINT16		usGunAmmoItem;	// the item # for the item table
			INT8		bGunAmmoStatus; // only for "attached ammo" - grenades, mortar shells
			UINT8		ubGunState; // SB manual recharge
	//warning, this unused space is the wrong size, 7 bytes above, 2 in the array, but it's been saved like that
			UINT8		ubGunUnused[OLD_MAX_OBJECTS_PER_SLOT_101 - 6];
		};
		struct
		{
			UINT8		ubShotsLeft[OLD_MAX_OBJECTS_PER_SLOT_101];
		};
		struct
		{
			INT8		bStatus[OLD_MAX_OBJECTS_PER_SLOT_101];
		};		
		struct
		{
			INT8		bMoneyStatus;
			UINT32		uiMoneyAmount;
			UINT8		ubMoneyUnused[OLD_MAX_OBJECTS_PER_SLOT_101 - 5];
		};
		struct
		{ // this is used by placed bombs, switches, and the action item
			INT8		bBombStatus;			// % status
			INT8		bDetonatorType;		// timed, remote, or pressure-activated
			UINT16		usBombItem;				// the usItem of the bomb.
			union
			{
				struct
				{
					INT8		bDelay;				// >=0 values used only
				};
				struct
				{
					INT8		bFrequency;		// >=0 values used only
				};
			};
			UINT8	ubBombOwner; // side which placed the bomb
			UINT8	bActionValue;// this is used by the ACTION_ITEM fake item
			union
			{
				struct
				{
					UINT8 ubTolerance; // tolerance value for panic triggers
				};
				struct 
				{
					UINT8 ubLocationID; // location value for remote non-bomb (special!) triggers
				};
			};		
		};
		struct
		{
			INT8 bKeyStatus[ 6 ];
			UINT8 ubKeyID;
			UINT8 ubKeyUnused[1];
		};
		struct
		{
			UINT8 ubOwnerProfile;
			UINT8 ubOwnerCivGroup;
			UINT8 ubOwnershipUnused[6];
		};
	};
};
#define SIZEOF_OLD_OBJECTTYPE_101_UNION (sizeof(Version101::OLD_OBJECTTYPE_101_UNION))

class OLD_OBJECTTYPE_101
{
public:
	OLD_OBJECTTYPE_101& operator=(OBJECTTYPE& src);//dnl ch33 120909
	UINT16		usItem;
	UINT8		ubNumberOfObjects;

	Version101::OLD_OBJECTTYPE_101_UNION	ugYucky;

  // attached objects
	UINT16		usAttachItem[OLD_MAX_ATTACHMENTS_101];
	INT8		bAttachStatus[OLD_MAX_ATTACHMENTS_101];

	INT8		fFlags;
	UINT8		ubMission;
	INT8		bTrap;        // 1-10 exp_lvl to detect
	UINT8		ubImprintID;	// ID of merc that item is imprinted on
	UINT8		ubWeight;
	UINT8		fUsed;				// flags for whether the item is used or not
};

namespace ObjectDataStructs {
	struct OBJECT_GUN
	{
		INT16		bGunStatus;		// status % of gun
		UINT8		ubGunAmmoType;	// ammo type, as per weapons.h
		UINT16		ubGunShotsLeft;	// duh, amount of ammo left
		UINT16		usGunAmmoItem;	// the item # for the item table
		INT16		bGunAmmoStatus; // only for "attached ammo" - grenades, mortar shells
		UINT8		ubGunState;		// SB manual recharge
	};
	struct OBJECT_MONEY
	{
		INT16		bMoneyStatus;
		UINT32		uiMoneyAmount;
	};
	struct OBJECT_BOMBS_AND_OTHER
	{ // this is used by placed bombs, switches, and the action item
		INT16		bBombStatus;		// % status
		INT8		bDetonatorType;		// timed, remote, or pressure-activated
		UINT16		usBombItem;			// the usItem of the bomb.
		union
		{
			INT8		bDelay;			// >=0 values used only
			INT8		bFrequency;		// >=0 values used only
		};
		UINT16	ubBombOwner;			// side which placed the bomb
		UINT8	bActionValue;			// this is used by the ACTION_ITEM fake item
		union
		{
			UINT8 ubTolerance;			// tolerance value for panic triggers
			UINT8 ubLocationID;			// location value for remote non-bomb (special!) triggers
		};
	};
	struct OBJECT_KEY
	{
		INT16 bKeyStatus;
		UINT8 ubKeyID;
	};
	struct OBJECT_OWNER
	{
		UINT16 ubOwnerProfile;
		UINT8 ubOwnerCivGroup;
	};
	struct OBJECT_LBE
	{
		INT16	bLBEStatus;
		INT8	bLBE;				// Marks item as LBENODE
		int		uniqueID;			// how the LBENODE is accessed
	};
};

class ObjectData
{
public:
	//needs a default ctor that inits stuff so that an objectStack can be init with 1 empty ObjectData
	ObjectData() {initialize();};
	~ObjectData();
	// Copy Constructor
	ObjectData(const ObjectData&);
	// Assignment operator
    ObjectData& operator=(const ObjectData&);


	void	initialize() {memset(this, 0, sizeof(ObjectData));};
	void	DeleteLBE();
	void	DuplicateLBE();
	bool operator==(ObjectData& compare);
	bool operator==(const ObjectData& compare)const;


	union {
		INT16										objectStatus;//holds the same value as bStatus[0]
		UINT16										ubShotsLeft;//holds the same value as ubShotsLeft[0]
		ObjectDataStructs::OBJECT_GUN				gun;
		ObjectDataStructs::OBJECT_MONEY				money;
		ObjectDataStructs::OBJECT_BOMBS_AND_OTHER	misc;
		ObjectDataStructs::OBJECT_KEY				key;
		ObjectDataStructs::OBJECT_OWNER				owner;
		ObjectDataStructs::OBJECT_LBE				lbe;
	};
	INT8		bTrap;			// 1-10 exp_lvl to detect
	UINT8		fUsed;			// flags for whether the item is used or not
	UINT8		ubImprintID;	// ID of merc that item is imprinted on

	// Flugente: due do inconsistencies with WF maps, where data from a map is laoded differently, I had to add this marker. 
	// New values, like bTemperature, have to come after this. And please, don't destroy ObjectData's POD-ness.
	char		endOfPOD;

	// these variables should belong to a different position. However, I am forced to put them here, otherwise loading of WF maps and other old data would not work properly
	FLOAT		bTemperature;	// Flugente FTW 1.2: temperature of gun

	// should belong to misc, but was moved here because of the old maps issue
	// added by Flugente 12-04-15
	UINT8		ubDirection;		// direction the bomb faces (for directional explosives)
	UINT32		ubWireNetworkFlag;	// flags for the tripwire network
	INT8		bDefuseFrequency;	// frequency for defusing, >=0 values used only

	// Flugente: advanced repair system
	INT16		sRepairThreshold;	// repair only possible up to this value

	FLOAT		bFiller;			// unused for now

	UINT64		sObjectFlag;		// used to notify of various states that apply to this object, but not the item in general
};
// Flugente: needed for reading WF maps
#define SIZEOF_OBJECTDATA_POD	(offsetof(ObjectData, endOfPOD))

typedef	std::list<OBJECTTYPE>	attachmentList;
class StackedObjectData  {
public:
	StackedObjectData();
	~StackedObjectData();
	void	initialize() {attachments.clear(); data.initialize();};
	OBJECTTYPE* GetAttachmentAtIndex(UINT8 index);
	BOOLEAN	RemoveAttachmentAtIndex(UINT8 index, attachmentList::iterator * returnIter = NULL);
	attachmentList::iterator RemoveAttachmentAtIter(attachmentList::iterator iter);
	BOOLEAN	AddAttachmentAtIndex(UINT8 index, OBJECTTYPE pObj);
	UINT16 AttachmentListSize();
	bool operator==(StackedObjectData& compare);
	bool operator==(const StackedObjectData& compare)const;

	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	attachmentList	attachments;
	ObjectData		data;
};
typedef std::list<StackedObjectData>	StackedObjects;


#define ALL_OBJECTS -1
class OBJECTTYPE
{
public:
	// Constructor
	OBJECTTYPE();
	// Conversion operator
    OBJECTTYPE& operator=(const OLD_OBJECTTYPE_101&);
	// Copy Constructor
	OBJECTTYPE(const OBJECTTYPE&);
	// Assignment operator
    OBJECTTYPE& operator=(const OBJECTTYPE&);
	// Destructor
	~OBJECTTYPE();

	const StackedObjectData* operator[](const unsigned int index) const;
	StackedObjectData* operator[](const unsigned int index);

	// Initialize the soldier.  
	//  Use this instead of the old method of calling memset.
	//  Note that the constructor does this automatically.
	void	initialize();

	bool	operator==(OBJECTTYPE& compare);
	bool	operator==(const OBJECTTYPE& compare)const;
	bool	exists() const;
	bool	IsActiveLBE(unsigned int index);
	bool	HasAnyActiveLBEs(SOLDIERTYPE * pSoldier = NULL, UINT8 iter = 0);
	LBENODE*	GetLBEPointer(unsigned int index);


	UINT16	GetWeightOfObjectInStack(unsigned int index = 0);
	int		AddObjectsToStack(int howMany, int objectStatus = 100);
	int		AddObjectsToStack(OBJECTTYPE& sourceObject, int howManyWanted = ALL_OBJECTS, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0, bool allowLBETransfer = true);
	int		ForceAddObjectsToStack(OBJECTTYPE& sourceObject, int howManyWanted = ALL_OBJECTS);
	int		MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove = ALL_OBJECTS, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0);
	int		RemoveObjectsFromStack(int howMany);
	bool	RemoveObjectAtIndex(unsigned int index, OBJECTTYPE* destObject = NULL);
private://these are only helpers for the above functions
	int		PrivateRemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject = NULL, SOLDIERTYPE* pSoldier = NULL, int slot = STACK_SIZE_LIMIT, int cap = 0);
	void	SpliceData(OBJECTTYPE& sourceObject, unsigned int numToSplice, StackedObjects::iterator beginIter);
	bool	CanStack(OBJECTTYPE& sourceObject, int& numToStack);
public:

	BOOLEAN AttachObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound = TRUE, UINT8 subObject = 0, INT32 iItemPos = -1, BOOLEAN fRemoveProhibited = TRUE, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>());
	BOOLEAN AttachObjectOAS( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound = TRUE, UINT8 subObject = 0);
	BOOLEAN AttachObjectNAS( SOLDIERTYPE * pSoldier, OBJECTTYPE * pAttachment, BOOLEAN playSound = TRUE, UINT8 subObject = 0, INT32 iItemPos = -1, BOOLEAN fRemoveProhibited = TRUE, std::vector<UINT16> usAttachmentSlotIndexVector = std::vector<UINT16>());
	BOOLEAN RemoveAttachment( OBJECTTYPE* pAttachment, OBJECTTYPE * pNewObj = NULL, UINT8 subObject = 0, SOLDIERTYPE * pSoldier = NULL, BOOLEAN fForceInseperable = FALSE, BOOLEAN fRemoveProhibited = TRUE);
	// HEADROCK HAM 5: Object Transformation
	BOOLEAN TransformObject( SOLDIERTYPE * pSoldier, UINT8 ubStatusIndex, TransformInfoStruct * Transform, OBJECTTYPE *pParent );

	//see comments in .cpp
	static	void DeleteMe(OBJECTTYPE** ppObject);
	static	void CopyToOrCreateAt(OBJECTTYPE** ppTarget, OBJECTTYPE* pSource);

	BOOLEAN	Load( HWFILE hFile );
	BOOLEAN	Load( INT8** hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion );
	BOOLEAN	Save( HWFILE hFile, bool fSavingMap );

	//POD
	UINT16		usItem;
	UINT8		ubNumberOfObjects;
	UINT8		ubMission;		//EDIT THIS OUT WHEN THERE ARE NO ASSERTS!

	//ADB ubWeight has been removed because it is always recalculated at every object change but only used in 1 place!!!
	//much better to recalculate it only where it is used once!!!
	//UINT16		ubWeight;		//used to be UINT8
	UINT8		fFlags;//used to be INT8, but that makes anything with OBJECT_NO_OVERWRITE negative

	char		endOfPOD;
#define SIZEOF_OBJECTTYPE_POD	(offsetof(OBJECTTYPE, endOfPOD))

	StackedObjects		objectStack;
//	std::vector<UINT16>	usAttachmentSlotIndexVector;	//WarmSteel - This holds the slots indexes this weapon has.  -- CHRISL: Can't have this here because of item stacks
};

extern OBJECTTYPE gTempObject;

// SUBTYPES
#define IC_NONE						0x00000001
#define IC_GUN						0x00000002
#define IC_BLADE					0x00000004
#define IC_THROWING_KNIFE	0x00000008

#define IC_LAUNCHER				0x00000010
#define IC_TENTACLES			0x00000020

#define IC_THROWN					0x00000040
#define IC_PUNCH					0x00000080

#define IC_GRENADE				0x00000100
#define IC_BOMB						0x00000200
#define IC_AMMO						0x00000400
#define IC_ARMOUR					0x00000800

#define IC_MEDKIT					0x00001000
#define IC_KIT						0x00002000
#define IC_APPLIABLE			0x00004000
#define IC_FACE           0x00008000

#define IC_KEY						0x00010000
#define IC_LBEGEAR					0x00020000	// Added for LBE items as part of the new inventory system
#define IC_BELTCLIP					0x00040000	// Added for Belt Clip items

#define IC_MISC						0x10000000
#define IC_MONEY					0x20000000
#define IC_RANDOMITEM				0x40000000	// added by Flugente for random items

// PARENT TYPES
#define IC_WEAPON					( IC_GUN | IC_BLADE | IC_THROWING_KNIFE | IC_LAUNCHER | IC_TENTACLES )
#define IC_EXPLOSV				( IC_GRENADE | IC_BOMB )

#define IC_BOBBY_GUN			( IC_GUN | IC_LAUNCHER )
#define IC_BOBBY_MISC			( IC_GRENADE | IC_BOMB | IC_MISC | IC_MEDKIT | IC_KIT | IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH | IC_FACE | IC_LBEGEAR )

// HEADROCK HAM 5: Inventory Filter Types
#define IC_MAPFILTER_GUN		( IC_GUN | IC_LAUNCHER )
#define IC_MAPFILTER_AMMO		( IC_AMMO )
#define IC_MAPFILTER_EXPLOSV	( IC_GRENADE | IC_BOMB )
#define IC_MAPFILTER_MELEE		( IC_BLADE | IC_PUNCH | IC_THROWN | IC_THROWING_KNIFE )
#define IC_MAPFILTER_KIT		( IC_KIT | IC_MEDKIT | IC_APPLIABLE )
#define IC_MAPFILTER_LBE		( IC_LBEGEAR | IC_BELTCLIP )
#define IC_MAPFILTER_ARMOR		( IC_ARMOUR | IC_FACE )
#define IC_MAPFILTER_MISC		( IC_TENTACLES | IC_KEY | IC_MISC | IC_MONEY | IC_NONE )
#define IC_MAPFILTER_ALL		( IC_MAPFILTER_GUN | IC_MAPFILTER_AMMO | IC_MAPFILTER_EXPLOSV | IC_MAPFILTER_MELEE | IC_MAPFILTER_KIT | IC_MAPFILTER_LBE | IC_MAPFILTER_ARMOR | IC_MAPFILTER_MISC )

// redesigned by Flugente on 12-04-10
// Chrisl: Define attachment classes
#define AC_BIPOD		0x00000001	//1
#define AC_MUZZLE		0x00000002	//2
#define AC_LASER		0x00000004	//4
#define AC_SIGHT		0x00000008	//8
#define AC_SCOPE		0x00000010	//16
#define AC_STOCK		0x00000020	//32
#define AC_MAGWELL   	0x00000040	//64
#define AC_INTERNAL		0x00000080	//128
#define AC_EXTERNAL		0x00000100	//256
#define AC_UNDERBARREL	0x00000200	//512
#define AC_GRENADE		0x00000400	//1024
#define AC_ROCKET		0x00000800	//2048
#define AC_FOREGRIP		0x00001000	//4096
#define AC_HELMET       0x00002000	//8192
#define AC_VEST 		0x00004000	//16384
#define AC_PANTS		0x00008000	//32768
#define AC_DETONATOR    0x00010000	//65536
#define AC_BATTERY      0x00020000	//131072
#define AC_EXTENDER		0x00040000	//262144
#define AC_SLING		0x00080000	//524288			// rifle sling
#define AC_REMOTEDET    0x00100000	//1048576			// remote detonator for bombs
#define AC_DEFUSE		0x00200000	//2097152			// defuse item for bombs
#define AC_IRONSIGHT	0x00400000	//4194304			// for attachable Iron Sights
#define AC_FEEDER		0x00800000	//8388608			// allow external feeding
#define AC_MODPOUCH		0x01000000	//16777216			// for new modular pouches
#define AC_RIFLEGRENADE	0x02000000	//33554432			// GL, needs a bullet and blocks other firing modes
#define AC_BAYONET		0x04000000	//67108864
#define AC_MISC16		0x08000000	//134217728
#define AC_MISC17		0x10000000	//268435456
#define AC_MISC18		0x20000000	//536870912
#define AC_MISC19		0x40000000	//1073741824
#define AC_MISC20		0x80000000	//2147483648

// -------- added by Flugente: flags for tripwire networks --------
// the numbering of these flags is important. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// these flags were revised on 2012-9-2
// it is theoretically possible to have much more than the currently existing 4 networks, and to have many more hierarchy levels
// however, at the moment, there is no need for that
#define TRIPWIRE_NETWORK_OWNER_ENEMY		0x00000001	//1			// this wire belongs to an enemy network
#define TRIPWIRE_NETWORK_OWNER_PLAYER		0x00000002	//2			// this wire was set by the player
//#define TRIPWIRE_NETWORK_OWNER_PLAYER		0x00000004	//4			// unused, might one day be used in multiplayer or with several factions
//#define TRIPWIRE_NETWORK_OWNER_PLAYER		0x00000008	//8			// unused, might one day be used in multiplayer or with several factions

#define TRIPWIRE_NETWORK_NET_1				0x00000010	//16		// network number  of the wire
#define TRIPWIRE_NETWORK_NET_2				0x00000020	//32
#define TRIPWIRE_NETWORK_NET_3   			0x00000040	//64
#define TRIPWIRE_NETWORK_NET_4				0x00000080	//128

/*#define ENEMY_NET_1_LVL_3		0x00000100	//256
#define ENEMY_NET_2_LVL_3		0x00000200	//512
#define ENEMY_NET_3_LVL_3		0x00000400	//1024
#define ENEMY_NET_4_LVL_3		0x00000800	//2048

#define TRIPWIRE_NETWORK_LVL_1				0x00001000	//4096		// hierarchy level of the wire
#define TRIPWIRE_NETWORK_LVL_2				0x00002000	//8192
#define TRIPWIRE_NETWORK_LVL_3 				0x00004000	//16384
#define TRIPWIRE_NETWORK_LVL_4				0x00008000	//32768

#define PLAYER_NET_1_LVL_1		0x00010000	//65536
#define PLAYER_NET_2_LVL_1      0x00020000	//131072
#define PLAYER_NET_3_LVL_1		0x00040000	//262144
#define PLAYER_NET_4_LVL_1		0x00080000	//524288*/

// WARNING! It is important that the flags with the highest numbers are the hierarchy flags!
// This allows for a very simple check on wether a wire gets called
// keep this in mind when adding additional flags!
#define TRIPWIRE_NETWORK_LVL_1				0x00100000	//1048576	// hierarchy level of the wire
#define TRIPWIRE_NETWORK_LVL_2				0x00200000	//2097152
#define TRIPWIRE_NETWORK_LVL_3				0x00400000	//4194304
#define TRIPWIRE_NETWORK_LVL_4				0x00800000	//8388608

#define ARTILLERY_STRIKE_COUNT_1			0x01000000	//16777216
#define ARTILLERY_STRIKE_COUNT_2			0x02000000	//33554432
#define ARTILLERY_STRIKE_COUNT_4			0x04000000	//67108864
/*#define TRIPWIRE_NETWORK_LVL_4		0x08000000	//134217728

#define PLAYER_NET_1_LVL_4		0x10000000	//268435456
#define PLAYER_NET_2_LVL_4		0x20000000	//536870912
#define PLAYER_NET_3_LVL_4		0x40000000	//1073741824
#define PLAYER_NET_4_LVL_4		0x80000000	//2147483648*/
// ----------------------------------------------------------------

#define ANY_ARTILLERY_FLAG	(ARTILLERY_STRIKE_COUNT_1|ARTILLERY_STRIKE_COUNT_2|ARTILLERY_STRIKE_COUNT_4)

// -------- added by Flugente: various item flags --------
// flags used for various item properties (easier than adding 32 differently named variables). DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// note that these should not be used to determine what kind of an attachment an item is, that is determined by attachmentclass and the AC_xxx flags above
#define BLOOD_BAG				0x00000001	//1			// this item is a blood bag can can be used to boost surgery
#define MANPAD					0x00000002	//2			// this item is a MAn-Portable Air-Defense System
#define BEARTRAP				0x00000004	//4			// a mechanical trap that does no explosion, but causes leg damage to whoever activates it
#define CAMERA					0x00000008	//8

#define WATER_DRUM				0x00000010	//16		// water drums allow to refill canteens in the sector they are in
#define MEAT_BLOODCAT			0x00000020	//32		// retrieve this by gutting a bloodcat
#define MEAT_COW   				0x00000040	//64		// retrieve this by gutting a cow
#define BELT_FED				0x00000080	//128		// item can be fed externally

#define AMMO_BELT				0x00000100	//256		// this item can be used to feed externally
#define AMMO_BELT_VEST			0x00000200	//512		// this is a vest that can contain AMMO_BELT items in its medium slots
#define CAMO_REMOVAL			0x00000400	//1024		// item can be used to remove camo
#define CLEANING_KIT			0x00000800	//2048		// weapon cleaning kit

#define ATTENTION_ITEM			0x00001000	//4096		// this item is 'interesting' to the AI. Dumb soldiers may try to pick it up
#define GAROTTE					0x00002000	//8192		// this item is a garotte
#define COVERT 					0x00004000	//16384		// if LBE, any gun inside will be covert. On a gun, it will be covert in any LBE, even if the LBE does not have that tag itself
#define CORPSE					0x00008000	//32768		// a dead body

#define SKIN_BLOODCAT			0x00010000	//65536		// retrieve this by skinning (=decapitating) a bloodcat
#define NO_METAL_DETECTION      0x00020000	//131072	// a planted bomb with this flag can NOT be detected via metal detector. Use sparingly!
#define JUMP_GRENADE			0x00040000	//262144	// add +25 heigth to explosion, used for bouncing grenades and jumping mines
#define HANDCUFFS				0x00080000	//524288	// item can be used to capture soldiers

#define TASER					0x00100000	//1048576	// item is a taser, melee hits with this will drain breath (if batteries are supplied)
#define SCUBA_BOTTLE			0x00200000	//2097152	// item is a scuba gear air bottle
#define SCUBA_MASK				0x00400000	//4194304	// item is a scuba gear breathing mask
#define SCUBA_FINS				0x00800000	//8388608	// this item speed up swimming, but slows walking and running

#define TRIPWIREROLL			0x01000000	//16777216	// this item is a tripwire roll
#define RADIO_SET				0x02000000	//33554432	// item can be used to radio militia/squads in other sectors
#define SIGNAL_SHELL			0x04000000	//67108864	// this is a signal shell that precedes artillery barrages
#define SODA					0x08000000	//134217728	// item is a can of soda, sold in vending machines

#define ROOF_COLLAPSE_ITEM		0x10000000	//268435456	// this item is required in the collapsing of roof tiles. It is used internally and should never be seen by the player
#define DISEASEPROTECTION_1		0x20000000	//536870912		// this item protects us from getting diseases by human contact if kept in inventory
#define DISEASEPROTECTION_2		0x40000000	//1073741824	// this item protects us from getting diseases by human contact if kept in inventory
#define LBE_EXPLOSIONPROOF		0x80000000	//2147483648	// any gear in LBE with this flag is protected from explosions

// extended flagmask to UINT64
#define EMPTY_BLOOD_BAG			0x0000000100000000		// this item is a empty blood bag
#define MEDICAL_SPLINT			0x0000000200000000		// this item is a medical splint that can be applied to some diseases
#define ITEM_damageable			0x0000000400000000
#define ITEM_repairable			0x0000000800000000

#define ITEM_waterdamages		0x0000001000000000
#define ITEM_metal				0x0000002000000000
#define ITEM_sinks				0x0000004000000000
#define ITEM_showstatus			0x0000008000000000

#define ITEM_hiddenaddon		0x0000010000000000
#define ITEM_twohanded			0x0000020000000000
#define ITEM_notbuyable			0x0000040000000000
#define ITEM_attachment			0x0000080000000000

#define ITEM_hiddenattachment	0x0000100000000000
#define ITEM_biggunlist			0x0000200000000000
#define ITEM_notineditor		0x0000400000000000
#define ITEM_defaultundroppable	0x0000800000000000

#define ITEM_unaerodynamic		0x0001000000000000
#define ITEM_electronic			0x0002000000000000
#define ITEM_cannon				0x0004000000000000
#define ITEM_rocketrifle		0x0008000000000000

#define ITEM_fingerprintid		0x0010000000000000
#define ITEM_metaldetector		0x0020000000000000
#define ITEM_gasmask			0x0040000000000000
#define ITEM_lockbomb			0x0080000000000000

#define ITEM_flare				0x0100000000000000
#define ITEM_grenadelauncher	0x0200000000000000
#define ITEM_mortar				0x0400000000000000
#define ITEM_duckbill			0x0800000000000000

#define ITEM_detonator			0x1000000000000000
#define ITEM_remotedetonator	0x2000000000000000
#define ITEM_hidemuzzleflash	0x4000000000000000
#define ITEM_rocketlauncher		0x8000000000000000

// New UINT64 Item Flag => usItemFlag2
#define ITEM_singleshotrocketlauncher	0x00000001
#define ITEM_brassknuckles				0x00000002
#define ITEM_crowbar					0x00000004
#define ITEM_glgrenade					0x00000008

#define ITEM_flakjacket					0x00000010
#define ITEM_leatherjacket				0x00000020
#define ITEM_batteries					0x00000040
#define ITEM_needsbatteries				0x00000080

#define ITEM_xray						0x00000100
#define ITEM_wirecutters				0x00000200
#define ITEM_toolkit					0x00000400
#define ITEM_firstaidkit				0x00000800

#define ITEM_medicalkit					0x00001000
#define ITEM_canteen					0x00002000
#define ITEM_jar						0x00004000
#define ITEM_canandstring				0x00008000

#define ITEM_marbles					0x00010000
#define ITEM_walkman					0x00020000
#define ITEM_remotetrigger				0x00040000
#define ITEM_robotremotecontrol			0x00080000

#define ITEM_camouflagekit				0x00100000
#define ITEM_locksmithkit				0x00200000
#define ITEM_mine						0x00400000
#define ITEM_antitankmine				0x00800000

#define ITEM_hardware					0x01000000
#define ITEM_medical					0x02000000
#define ITEM_gascan						0x04000000
#define ITEM_containsliquid				0x08000000

#define ITEM_rock						0x10000000
#define ITEM_thermaloptics				0x20000000
#define ITEM_scifi						0x40000000
#define ITEM_newinv						0x80000000

#define ITEM_DiseaseSystemExclusive		0x0000000100000000		// kitty: item exclusively available with disease feature 
#define ITEM_barrel						0x0000000200000000		// item can be used on some guns as an exchange barrel
#define ITEM_tripwireactivation			0x0000000400000000		// item (mine) can be activated by nearby tripwire
#define ITEM_tripwire					0x0000000800000000		// item is tripwire

#define ITEM_directional				0x0000001000000000		// item is a directional mine/bomb (actual direction is set upon planting)
#define ITEM_blockironsight				0x0000002000000000		// if a gun or any attachment have this property, the iron sight won't be usable (if there is at least one other usable sight)
#define ITEM_fAllowClimbing				0x0000004000000000		// JMich: BackpackClimb does item allow climbing while wearing it
#define ITEM_cigarette					0x0000008000000000		// Flugente: this item can be smoked

#define ITEM_fProvidesRobotCamo			0x0000010000000000		// rftr: robot attachments
#define ITEM_fProvidesRobotNightVision	0x0000020000000000		// rftr: robot attachments
#define ITEM_fProvidesRobotLaserBonus	0x0000040000000000		// rftr: robot attachments

// ----------------------------------------------------------------

// -------- added by Flugente: flags for objects --------
// the numbering of these flags is important. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define CORPSE_M_SMALL			0x00000001	//1
#define CORPSE_M_BIG			0x00000002	//2
#define CORPSE_F				0x00000004	//4
#define CORPSE_NO_HEAD			0x00000008	//8

#define CORPSE_HAIR_BROWN		0x00000010	//16
#define CORPSE_HAIR_BLACK		0x00000020	//32
#define CORPSE_HAIR_WHITE   	0x00000040	//64
#define CORPSE_HAIR_BLOND		0x00000080	//128

#define CORPSE_NO_ZOMBIE_RISE	0x00000100	//256		// no zombie can rise from this corpse
#define CORPSE_SKIN_PINK		0x00000200	//512
#define CORPSE_SKIN_TAN			0x00000400	//1024
#define CORPSE_SKIN_DARK		0x00000800	//2048

#define CORPSE_SKIN_BLACK		0x00001000	//4096
#define CORPSE_VEST_BROWN       0x00002000	//8192
#define CORPSE_VEST_grey 		0x00004000	//16384
#define CORPSE_VEST_GREEN		0x00008000	//32768

#define CORPSE_VEST_JEAN		0x00010000	//65536
#define CORPSE_VEST_RED			0x00020000	//131072
#define CORPSE_VEST_BLUE		0x00040000	//262144
#define CORPSE_VEST_YELLOW		0x00080000	//524288

#define CORPSE_VEST_WHITE		0x00100000	//1048576
#define CORPSE_VEST_BLACK		0x00200000	//2097152
#define CORPSE_VEST_GYELLOW		0x00400000	//4194304
#define CORPSE_VEST_PURPLE		0x00800000	//8388608

#define CORPSE_PANTS_GREEN		0x01000000	//16777216
#define CORPSE_PANTS_JEAN		0x02000000	//33554432
#define CORPSE_PANTS_TAN		0x04000000	//67108864
#define CORPSE_PANTS_BLACK		0x08000000	//134217728

#define CORPSE_PANTS_BLUE		0x10000000	//268435456
#define CORPSE_PANTS_BEIGE		0x20000000	//536870912
#define CORPSE_NO_VEST			0x40000000	//1073741824		// corpse has no vest item (it has been either taken or been destroyed)
#define CORPSE_NO_PANTS			0x80000000	//2147483648		// corpse has no pants item/for tripwire activation (gets set and unset when activating tripwire)

#define TRIPWIRE_ACTIVATED					0x0000000100000000	//		 4294967296
#define TAKEN_BY_MILITIA					0x0000000200000000	//		 8589934592		// this item was picked up by militia
#define TAKEN_BY_MILITIA_TABOO_GREEN		0x0000000400000000	//		17179869184		// this item is taboo for green militia (have to reset flag for world item upon dropping it)
#define TAKEN_BY_MILITIA_TABOO_BLUE			0x0000000800000000	//		34359738368		// this item is taboo for blue militia (have to reset flag for world item upon dropping it)

#define INFECTED							0x0000001000000000	//						// this item is infected with disease 0, getting damaged by this will infect you
#define DELAYED_GRENADE_EXPLOSION			0x0000002000000000	//						// grenade will blow up on next turn
#define INFECTION_DIAGNOSED					0x0000004000000000	//						// the player knows that this item is infected

// Flugente TODO 2012-09-17: next time we break savegame compatibility, extend the flagmasks from UINT32 to UINT64. I didn't do it this time (see double-used flag above), as we try to minimise those breaks. But it is needed.
// ----------------------------------------------------------------

// -------- added by Flugente: flags for action items --------
// the numbering of these flags is important. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// these flags are supposed to be given to items that are used with action items. When an object is created from these action items, various object properties are set depending on this
// for example, directional explosive mines have to get their direction from this, as there is no way to specify this in the editor. Even if there was, there are no varaibles to store this information
// adding variables to the existing structure is unadvisable, as it would break too many existing structures.
// note: it will be assumed that tripwire placed via the editor is of enemy networks, so there is no flag for that
#define ITEM_TRIPWIRE_NETWORK_NET_1		0x00000001	//1			// network number  of the wire
#define ITEM_TRIPWIRE_NETWORK_NET_2		0x00000002	//2
#define ITEM_TRIPWIRE_NETWORK_NET_3		0x00000004	//4
#define ITEM_TRIPWIRE_NETWORK_NET_4		0x00000008	//8

#define ITEM_TRIPWIRE_NETWORK_LVL_1		0x00000010	//16		// hierarchy level of the wire
#define ITEM_TRIPWIRE_NETWORK_LVL_2		0x00000020	//32
#define ITEM_TRIPWIRE_NETWORK_LVL_3   	0x00000040	//64
#define ITEM_TRIPWIRE_NETWORK_LVL_4		0x00000080	//128

#define ITEM_DIRECTION_NORTH			0x00000100	//256		// direction for directional exxplosives (claymore etc.)
#define ITEM_DIRECTION_NORTHEAST		0x00000200	//512
#define ITEM_DIRECTION_EAST				0x00000400	//1024
#define ITEM_DIRECTION_SOUTHEAST		0x00000800	//2048

#define ITEM_DIRECTION_SOUTH			0x00001000	//4096
#define ITEM_DIRECTION_SOUTHWEST		0x00002000	//8192
#define ITEM_DIRECTION_WEST 			0x00004000	//16384
#define ITEM_DIRECTION_NORTHWEST		0x00008000	//32768

/*#define PLAYER_NET_1_LVL_1		0x00010000	//65536
#define PLAYER_NET_2_LVL_1      0x00020000	//131072
#define PLAYER_NET_3_LVL_1		0x00040000	//262144
#define PLAYER_NET_4_LVL_1		0x00080000	//524288

#define TRIPWIRE_NETWORK_LVL_1				0x00100000	//1048576	// hierarchy level of the wire
#define TRIPWIRE_NETWORK_LVL_2				0x00200000	//2097152
#define TRIPWIRE_NETWORK_LVL_3				0x00400000	//4194304
#define TRIPWIRE_NETWORK_LVL_4				0x00800000	//8388608

/*#define TRIPWIRE_NETWORK_LVL_1		0x01000000	//16777216
#define TRIPWIRE_NETWORK_LVL_2		0x02000000	//33554432
#define TRIPWIRE_NETWORK_LVL_3		0x04000000	//67108864
#define TRIPWIRE_NETWORK_LVL_4		0x08000000	//134217728

#define PLAYER_NET_1_LVL_4		0x10000000	//268435456
#define PLAYER_NET_2_LVL_4		0x20000000	//536870912
#define PLAYER_NET_3_LVL_4		0x40000000	//1073741824
#define PLAYER_NET_4_LVL_4		0x80000000	//2147483648*/

#define ITEM_TRIPWIRE_ANY				0x000000FF
#define ITEM_DIRECTION_ANY				0x0000FF00
// ----------------------------------------------------------------



// replaces candamage
//#define ITEM_DAMAGEABLE			0x0001
//// replaces canrepair
//#define ITEM_REPAIRABLE			0x0002
//// replaces waterdamage
//#define ITEM_WATER_DAMAGES	0x0004
//// replaces metal
//#define ITEM_METAL					0x0008
//// replaces sinkable
//#define ITEM_SINKS					0x0010
//// replaces seemeter
//#define ITEM_SHOW_STATUS		0x0020
//// for attachers/merges, hidden
//#define ITEM_HIDDEN_ADDON		0x0040
//// needs two hands
//#define ITEM_TWO_HANDED			0x0080
//// can't be found for sale
//#define ITEM_NOT_BUYABLE		0x0100
//// item is an attachment for something
//#define ITEM_ATTACHMENT			0x0200
//// item only belongs in the "big gun list"
//#define ITEM_BIGGUNLIST			0x0400
//// item should not be placed via the editor
//#define ITEM_NOT_EDITOR			0x0800
//// item defaults to undroppable
//#define ITEM_DEFAULT_UNDROPPABLE	0x1000
//// item is terrible for throwing
//#define ITEM_UNAERODYNAMIC	0x2000
//// item is electronic for repair (etc) purposes
//#define ITEM_ELECTRONIC			0x4000
//// item is a PERMANENT attachment
//#define ITEM_INSEPARABLE		0x8000
//
//// item flag combinations
//
//#define IF_STANDARD_GUN ITEM_DAMAGEABLE | ITEM_WATER_DAMAGES | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_METAL | ITEM_SINKS
//#define IF_TWOHANDED_GUN IF_STANDARD_GUN | ITEM_TWO_HANDED
//#define IF_STANDARD_BLADE ITEM_DAMAGEABLE | ITEM_WATER_DAMAGES | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_METAL | ITEM_SINKS
//#define IF_STANDARD_ARMOUR ITEM_DAMAGEABLE | ITEM_REPAIRABLE | ITEM_SHOW_STATUS | ITEM_SINKS
//#define IF_STANDARD_KIT ITEM_DAMAGEABLE | ITEM_SHOW_STATUS | ITEM_SINKS
//#define IF_STANDARD_CLIP ITEM_SINKS | ITEM_METAL

//#define EXPLOSIVE_GUN( x ) ( x == ROCKET_LAUNCHER || x == TANK_CANNON || x == RPG7 )

// HEADROCK HAM 4: As part of NCTH, over a dozen new variables have been added to the item struct. About the same number
// have also been removed. The new variables relate to the way the new shooting mechanism works, and obviously the ones
// taken out are no longer used anywhere as a result. Note that most of these new variables, the ones having to do with
// the CTH part of the shooting mechanism, come in size-3 arrays. The first item ([0]) represents modifiers given when the
// character is standing. [1] represents crouched, and [2] represents prone. Read more about how this is read from XML
// in the XML-reader formula.
// Removed vars:
// bipod, 
// tohitbonus,
// bestlaserrange,
// aimbonus,
// minrangeforaimbonus,
// autofiretohitbonus,
// bursttohitbonus

typedef struct
{
	CHAR16 szItemDesc[400];
	CHAR16 szBRDesc[400];
	CHAR16	szItemName[80]; //+1 for the null terminator //added
	CHAR16 szLongItemName[80];
	CHAR16 szBRName[80];
	UINT16 defaultattachments[MAX_DEFAULT_ATTACHMENTS]; //Need more default attachments, chose an array to do so. (no vector / list just to keep this all plain data)

	UINT64 nasAttachmentClass;	//CHRISL: Identify the class of attachment 
	UINT64 nasLayoutClass;
	UINT64 ulAvailableAttachmentPoint;
	UINT64 ulAttachmentPoint;
	UINT64 usItemFlag; // bitflags to store various item properties (better than introducing 64 BOOLEAN values). If I only had thought of this earlier....
	UINT64 usItemFlag2; // bitflags to store various item properties

	UINT32 uiIndex;
	UINT32 usItemClass;
	UINT32 attachmentclass; // attachmentclass used
	UINT32 drugtype; // this flagmask determines what different components are used in a drug, which results in different effects
	UINT32 foodtype;
	UINT32 usActionItemFlag; // Flugente: a flag that is necessary for transforming action items to objects with new abilities (for now, tripwire networks and directional explosives)
	UINT32 clothestype; // Flugente: clothes type that 'links' to an entry in Clothes.xml

	//zilpin: pellet spread patterns externalized in XML
	INT32 spreadPattern;

	FLOAT alcohol;
	// HEADROCK HAM 4: New modifiers that do not require a stance array, since they affect the gun objectively, not
	// subjectively.
	FLOAT RecoilModifierX;
	FLOAT RecoilModifierY;
	FLOAT scopemagfactor;
	FLOAT projectionfactor;
	FLOAT usOverheatingCooldownFactor;			// every turn/5 seconds, a gun's temperature is lowered by this amount
	FLOAT overheatTemperatureModificator;			// percentage modifier of heat a shot generates (read from attachments)
	FLOAT overheatCooldownModificator;			// percentage modifier of cooldown amount (read from attachments, applies to guns & barrels)
	FLOAT overheatJamThresholdModificator;		// percentage modifier of a gun's jam threshold (read from attachments)
	FLOAT overheatDamageThresholdModificator;		// percentage modifier of a gun's damage threshold (read from attachments)
	FLOAT dirtIncreaseFactor; // Flugente: advanced repair/dirt system. One shot causes this much dirt on a gun
	FLOAT fRobotDamageReductionModifier; // rftr: robot attachments

	// HEADROCK HAM 4: New variable arrays for the New CTH system.
	INT16 flatbasemodifier[3];
	INT16 percentbasemodifier[3];
	INT16 flataimmodifier[3];
	INT16 percentaimmodifier[3];
	INT16 percentcapmodifier[3];
	INT16 percenthandlingmodifier[3];
	INT16 percentdropcompensationmodifier[3];
	INT16 maxcounterforcemodifier[3];
	INT16 counterforceaccuracymodifier[3];
	INT16 targettrackingmodifier[3];
	INT16 aimlevelsmodifier[3];

	//Madd: Common Attachment Framework:  attach items based on matching connection points rather than using the old long attachmentinfo method
	UINT16 ubClassIndex;
	UINT16 ubGraphicNum;
	UINT16 ubWeight; //2 units per kilogram; roughly 1 unit per pound
	UINT16 ItemSize;
	UINT16 usPrice;
	UINT16 discardedlauncheritem;
	UINT16 randomitem; // Flugente: a link to RandomItemsClass.xml. Out of such an item, a random object is created, depending on the entries in the xml
	UINT16 usBuddyItem; // Flugente: item is connected to another item. Type of connection depends on item specifics
	UINT16 usRiotShieldStrength;	// Flugente: riot shields. strength of shield
	UINT16 usRiotShieldGraphic;	// Flugente: riot shields. graphic of shield (when deployed in tactical, taken from Tilecache/riotshield.sti)

	INT16 percentnoisereduction;
	INT16 bipod;
	INT16 tohitbonus;
	INT16 bestlaserrange;
	INT16 rangebonus;
	INT16 percentrangebonus;
	INT16 aimbonus;
	INT16 minrangeforaimbonus;
	INT16 percentapreduction;
	INT16 percentstatusdrainreduction;
	INT16 bloodieditem;
	INT16 hearingrangebonus;
	INT16 visionrangebonus;
	INT16 nightvisionrangebonus;
	INT16 dayvisionrangebonus;
	INT16 cavevisionrangebonus;
	INT16 brightlightvisionrangebonus;
	INT16 itemsizebonus;
	INT16 damagebonus;
	INT16 meleedamagebonus;
	INT16 magsizebonus;
	INT16 percentautofireapreduction;
	INT16 autofiretohitbonus;
	INT16 APBonus;
	INT16 rateoffirebonus;
	INT16 burstsizebonus;
	INT16 bursttohitbonus;
	INT16 percentreadytimeapreduction;
	INT16 bulletspeedbonus;
	INT16 percentreloadtimeapreduction;
	INT16 percentburstfireapreduction;
	INT16 camobonus;
	INT16 stealthbonus;
	INT16 urbanCamobonus;
	INT16 desertCamobonus;
	INT16 snowCamobonus;
	INT16 PercentRecoilModifier;
	INT16 percentaccuracymodifier;
	INT16 usSpotting; // Flugente: spotting effectiveness
	INT16 sBackpackWeightModifier; // JMich: BackpackClimb modifier to weight calculation to climb.
	INT16 sFireResistance;

	UINT8 ubAttachToPointAPCost; // cost to attach to any matching point
	UINT8 ubCursor;
	UINT8 ubGraphicType;
	UINT8 ubPerPocket;
	UINT8 ubCoolness;
	UINT8 percenttunnelvision;
	UINT8 ubAttachmentSystem; //Item availability per attachment system: 0 = both, 1 = OAS, 2 = NAS
	UINT8 CrowbarModifier;
	UINT8 DisarmModifier;
	UINT8 usHackingModifier;
	UINT8 usBurialModifier; // Flugente: a modifier for burial effectiveness
	UINT8 usDamageChance; //  Flugente: advanced repair/dirt system. Chance that damage to the status will also damage the repair threshold
	UINT8 usFlashLightRange; // Flugente: range of a flashlight (an item with usFlashLightRange > 0 is deemed a flashlight)
	UINT8 usItemChoiceTimeSetting; // Flugente: determine wether the AI should pick this item for its choices only at certain times
	UINT8 ubSleepModifier; // silversurfer: item provides breath regeneration bonus while resting
	UINT8 usPortionSize;			// Flugente: for consumables: how much of this item is consumed at once
	UINT8 usAdministrationModifier; // Flugente: a modifier for administration effectiveness
	UINT8 inseparable; //Madd:Normally, an inseparable attachment can never be removed.  
	//But now we will make it so that these items can be replaced, but still not removed directly.
	//0 = removeable (as before)
	//1 = inseparable (as before)
	//2 = inseparable, but replaceable

	INT8 bSoundType;
	INT8 bReliability;
	INT8 bRepairEase;
	INT8 LockPickModifier;
	INT8 RepairModifier;
	INT8 randomitemcoolnessmodificator; // Flugente: a link to RandomItemsClass.xml. alters the allowed maximum coolness a random item can have
	INT8 bRobotStrBonus; // rftr: robot attachments
	INT8 bRobotAgiBonus; // rftr: robot attachments
	INT8 bRobotDexBonus; // rftr: robot attachments
	INT8 bRobotTargetingSkillGrant; // rftr: robot attachments
	INT8 bRobotChassisSkillGrant; // rftr: robot attachments
	INT8 bRobotUtilitySkillGrant; // rftr: robot attachments
	INT8 iTransportGroupMinProgress; // rftr: the progress bounds that allow a transport group to drop an item
	INT8 iTransportGroupMaxProgress; // rftr: the progress bounds that allow a transport group to drop an item
} INVTYPE;


// Flugente: move this to a better position
typedef struct RANDOM_ITEM_CHOICE_TYPE
{
	UINT16		uiIndex;
	//CHAR16		szName[80];					// name of this choice (not used ingame, only for readability)
	UINT16		randomitem[10];				// room to link to other random item choices
	UINT16		item[10];					// room for up to 10 items

} RANDOM_ITEM_CHOICE_TYPE;

#define RANDOM_ITEM_MAX_CATEGORIES 1000

extern RANDOM_ITEM_CHOICE_TYPE gRandomItemClass[RANDOM_ITEM_MAX_CATEGORIES];

// CHRISL: Added new structures to handle LBE gear and the two new XML files that will be needed to deal
// with the IC pockets and the new inventory system.
//DBrot: Increased the LBE combo to 32bit to allow for multiple combos (using a bitwise system)
class LBETYPE{
public:
	LBETYPE();
	LBETYPE(const LBETYPE&);
	LBETYPE& operator=(const LBETYPE&);
	~LBETYPE();
	UINT16			lbeIndex;
	UINT32			lbeClass;
	UINT32			lbeCombo;
	UINT8			lbeFilledSize;
	//DBrot: MOLLE
	UINT8			lbeAvailableVolume;
	UINT16			lbePocketsAvailable;
	char			POD;
	std::vector<UINT8>	lbePocketIndex;

};
#define SIZEOF_LBETYPE offsetof( LBETYPE, POD )
extern std::vector<LBETYPE> LoadBearingEquipment;
typedef enum eLBE_CLASS	// Designation of lbeClass
{
	THIGH_PACK=1,
	VEST_PACK,
	COMBAT_PACK,
	BACKPACK,
	LBE_POCKET,
	OTHER_POCKET,
} eLBE_CLASS;


class POCKETTYPE{
public:
	POCKETTYPE();
	POCKETTYPE(const POCKETTYPE&);
	POCKETTYPE& operator=(const POCKETTYPE&);
	~POCKETTYPE();
	UINT16			pIndex;
	CHAR16			pName[80];
	UINT8			pSilhouette;
	UINT16			pType;
	UINT32			pRestriction;
	//DBrot: MOLLE
	UINT8			pVolume;
	char			POD;
	std::vector<UINT8>	ItemCapacityPerSize;
};
#define SIZEOF_POCKETTYPE offsetof( POCKETTYPE, POD )
extern std::vector<POCKETTYPE> LBEPocketType;

// THE_BOB : added for pocket popup definitions
extern std::map<UINT8,popupDef> LBEPocketPopup;

typedef enum ePOCKET_TYPE
{
	NO_POCKET_TYPE = 0,
	GUNSLING_POCKET_TYPE = 1,
	KNIFE_POCKET_TYPE = 2,
	VEHICLE_POCKET_TYPE = 3,
} ePOCKET_TYPE;

#define FIRST_WEAPON 1
#define FIRST_AMMO 71
#define FIRST_EXPLOSIVE 131
#define FIRST_ARMOUR 161
#define FIRST_MISC 201
#define FIRST_KEY 271
#ifdef JA2EDITOR
#define OLD_MAXITEMS 351//dnl ch74 191013
#else
#define OLD_MAXITEMS MAXITEMS
#endif
#define NOTHING NONE
typedef enum
{
	NONE									= 0,

	// weapons
	GLOCK_17							= FIRST_WEAPON,
	GLOCK_18,
	BERETTA_92F,
	BERETTA_93R,
	SW38,
	BARRACUDA,
	DESERTEAGLE,
	M1911,
	MP5K,
	MAC10,	//10

	THOMPSON,
	COMMANDO,
	MP53,
	AKSU74,
	P90,
	TYPE85,
	SKS,
	DRAGUNOV,
	M24,
	AUG,	//20

	G41,
	MINI14,
	C7,
	FAMAS,
	AK74,
	AKM,
	M14,
	FNFAL,
	G3A3,
	G11,	//30

	M870,
	SPAS15,
	CAWS,
	MINIMI,
	RPK74,
	HK21E,
	COMBAT_KNIFE,
	THROWING_KNIFE,
	ROCK,
	GLAUNCHER,	//40

	MORTAR,					
	ROCK2,
	CREATURE_YOUNG_MALE_CLAWS,
	CREATURE_OLD_MALE_CLAWS,
	CREATURE_YOUNG_FEMALE_CLAWS,
	CREATURE_OLD_FEMALE_CLAWS,
	CREATURE_QUEEN_TENTACLES,
	CREATURE_QUEEN_SPIT,
	BRASS_KNUCKLES,
	UNDER_GLAUNCHER,	//50

	ROCKET_LAUNCHER,
	BLOODCAT_CLAW_ATTACK,
	BLOODCAT_BITE,
	MACHETE,
	ROCKET_RIFLE,
	AUTOMAG_III,
	CREATURE_INFANT_SPIT,
	CREATURE_YOUNG_MALE_SPIT,
	CREATURE_OLD_MALE_SPIT,
	TANK_CANNON,	//60

	DART_GUN,
	BLOODY_THROWING_KNIFE,
	FLAMETHROWER,
	CROWBAR,
	AUTO_ROCKET_RIFLE,	//65
// MADD MARKER

	BARRETT, //previously unused
	VAL_SILENT, //previously unused
	PSG, //previously unused
	TAR21, //previously unused
	
	MAX_WEAPONS							= ( FIRST_AMMO - 1 ), 

	CLIP9_15							= FIRST_AMMO, 
	CLIP9_30,
	CLIP9_15_AP,
	CLIP9_30_AP,
	CLIP9_15_HP,
	CLIP9_30_HP,
	CLIP38_6,
	CLIP38_6_AP,
	CLIP38_6_HP,
	CLIP45_7,  

	CLIP45_30,
	CLIP45_7_AP,
	CLIP45_30_AP,
	CLIP45_7_HP,
	CLIP45_30_HP,
	CLIP357_6,
	CLIP357_9,
	CLIP357_6_AP,
	CLIP357_9_AP,
	CLIP357_6_HP,

	CLIP357_9_HP,
	CLIP545_30_AP,
	CLIP545_30_HP,
	CLIP556_30_AP,
	CLIP556_30_HP,
	CLIP762W_10_AP,
	CLIP762W_30_AP,
	CLIP762W_10_HP,
	CLIP762W_30_HP,
	CLIP762N_5_AP,

	CLIP762N_20_AP,
	CLIP762N_5_HP,
	CLIP762N_20_HP,
	CLIP47_50_SAP,
	CLIP57_50_AP,
	CLIP57_50_HP,
	CLIP12G_7,
	CLIP12G_7_BUCKSHOT,
	CLIPCAWS_10_SAP,
	CLIPCAWS_10_FLECH,

	CLIPROCKET_AP,
	CLIPROCKET_HE,
	CLIPROCKET_HEAT,
	CLIPDART_SLEEP,

	CLIPFLAME,
// MADD MARKER
	CLIP50_11, // unused 
	CLIP9H_20, // unused 
	CLIP9_50, 
	CLIP9_50_AP, 
	CLIP9_50_HP, 
	DRUM545_75_AP,
	DRUM545_75_HP,
	BELT556_200_AP,
	BELT556_200_HP,
	BELT762N_100_AP,
	BELT762N_100_HP,
	CLIP57_20_AP,
	CLIP57_20_HP,
	// unused 
	// unused 

	// explosives
	STUN_GRENADE					= FIRST_EXPLOSIVE,
	TEARGAS_GRENADE,
	MUSTARD_GRENADE,
	MINI_GRENADE,
	HAND_GRENADE,
	RDX,
	TNT,
	HMX,
	C1,
	MORTAR_SHELL,

	MINE,
	C4,
	TRIP_FLARE,
	TRIP_KLAXON,
	SHAPED_CHARGE,
	BREAK_LIGHT,
	GL_HE_GRENADE,
	GL_TEARGAS_GRENADE,
	GL_STUN_GRENADE,
	GL_SMOKE_GRENADE,

	SMOKE_GRENADE,
	TANK_SHELL,
	STRUCTURE_IGNITE,
	CREATURE_COCKTAIL,
	STRUCTURE_EXPLOSION,
	GREAT_BIG_EXPLOSION,
	BIG_TEAR_GAS,
	SMALL_CREATURE_GAS,
	LARGE_CREATURE_GAS,
	VERY_SMALL_CREATURE_GAS=270, // Renumbered because the original 160 was lost
	SMALL_SMOKE=1702,			//	sevenfm - item for small smoke effect after explosion
	MOLOTOV_EXPLOSION=979,				//	molotov - small fire explosion
	GAS_EXPLOSION=1703,				//	molotov - small fire explosion
	FRAG_EXPLOSION=1704,				//	molotov - small fire explosion

	// armor
	FLAK_JACKET=161,					//= FIRST_ARMOUR, ( We're out of space! )
	FLAK_JACKET_18,
	FLAK_JACKET_Y,
	KEVLAR_VEST,
	KEVLAR_VEST_18,
	KEVLAR_VEST_Y,
	SPECTRA_VEST,
	SPECTRA_VEST_18,
	SPECTRA_VEST_Y,
	KEVLAR_LEGGINGS,

	KEVLAR_LEGGINGS_18,
	KEVLAR_LEGGINGS_Y,
	SPECTRA_LEGGINGS,
	SPECTRA_LEGGINGS_18,
	SPECTRA_LEGGINGS_Y,
	STEEL_HELMET,
	KEVLAR_HELMET,
	KEVLAR_HELMET_18,
	KEVLAR_HELMET_Y,
	SPECTRA_HELMET,

	SPECTRA_HELMET_18,
	SPECTRA_HELMET_Y,
	CERAMIC_PLATES,
	CREATURE_INFANT_HIDE,
	CREATURE_YOUNG_MALE_HIDE,
	CREATURE_OLD_MALE_HIDE,
	CREATURE_QUEEN_HIDE,
	LEATHER_JACKET,
	LEATHER_JACKET_W_KEVLAR,
	LEATHER_JACKET_W_KEVLAR_18,
	
	LEATHER_JACKET_W_KEVLAR_Y,
	CREATURE_YOUNG_FEMALE_HIDE,
	CREATURE_OLD_FEMALE_HIDE,
	TSHIRT,
	TSHIRT_DEIDRANNA,
	KEVLAR2_VEST,
	KEVLAR2_VEST_18,
	KEVLAR2_VEST_Y,

	// kits
	FIRSTAIDKIT								= FIRST_MISC,
	MEDICKIT,
	TOOLKIT,
	LOCKSMITHKIT,
	CAMOUFLAGEKIT,
	BOOBYTRAPKIT,
	// miscellaneous
	SILENCER,
	SNIPERSCOPE,
	BIPOD,
	EXTENDEDEAR,

	NIGHTGOGGLES,
	SUNGOGGLES,
	GASMASK,
	CANTEEN,
	METALDETECTOR,
	COMPOUND18,
	JAR_QUEEN_CREATURE_BLOOD,
	JAR_ELIXIR,
	MONEY,
	JAR,

	JAR_CREATURE_BLOOD,
	ADRENALINE_BOOSTER,
	DETONATOR,
	REMDETONATOR,	
	VIDEOTAPE,
	DEED,
	LETTER,
	TERRORIST_INFO,
	CHALICE,
	BLOODCAT_CLAWS,

	BLOODCAT_TEETH,
	BLOODCAT_PELT,
	SWITCH,
	// fake items
	ACTION_ITEM,
	REGEN_BOOSTER,
	SYRINGE_3,
	SYRINGE_4,
	SYRINGE_5,
	JAR_HUMAN_BLOOD,
	OWNERSHIP,

	// additional items
	LASERSCOPE,
	REMOTEBOMBTRIGGER,
	WIRECUTTERS,
	DUCKBILL,
	ALCOHOL,
	UVGOGGLES,
	DISCARDED_LAW,
	HEAD_1,
	HEAD_2,
	HEAD_3,
	HEAD_4,
	HEAD_5,
	HEAD_6,
	HEAD_7,
	WINE,
	BEER,
	PORNOS,
	VIDEO_CAMERA,
	ROBOT_REMOTE_CONTROL,
	CREATURE_PART_CLAWS,
	CREATURE_PART_FLESH,
	CREATURE_PART_ORGAN,
	REMOTETRIGGER,
	GOLDWATCH,
	GOLFCLUBS,
	WALKMAN,
	PORTABLETV,
	MONEY_FOR_PLAYERS_ACCOUNT,
	CIGARS,

	KEY_1 = FIRST_KEY,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_10,

	FLASH_SUPPRESSOR,
	RPG7,
	RPG_HE_ROCKET,
	RPG_AP_ROCKET,
	RPG_FRAG_ROCKET,
	REFLEX_SCOPED,
	REFLEX_UNSCOPED,
	KEY_18,
	KEY_19,
	KEY_20,

	KEY_21,
	KEY_22,
	KEY_23,
	KEY_24,
	KEY_25,
	KEY_26,
	KEY_27,
	KEY_28,
	KEY_29,
	KEY_30,

	KEY_31,
	KEY_32,		// 302
	SILVER_PLATTER,
	DUCT_TAPE,
	ALUMINUM_ROD,
	SPRING,
	SPRING_AND_BOLT_UPGRADE,
	STEEL_ROD,
	QUICK_GLUE,
	GUN_BARREL_EXTENDER,

	STRING,
	TIN_CAN,
	STRING_TIED_TO_TIN_CAN,
	MARBLES,
	LAME_BOY,
	COPPER_WIRE,
	DISPLAY_UNIT,
	FUMBLE_PAK,
	XRAY_BULB,
	CHEWING_GUM, // 320

	FLASH_DEVICE,
	BATTERIES,
	ELASTIC,
	XRAY_DEVICE,
	SILVER,
	GOLD,
	GAS_CAN,
	M900, //UNUSED_26,
	M950, //UNUSED_27,
	M960, //UNUSED_28,
	MICRO_UZI, // UNUSED_29,
	
	ENFIELD, // UNUSED_30,
	MP5,
	MP5SD,
	MP5N,
	UMP45,
	FIVE7,
	P7M8,
	G36K,
	G36C, // 340

	MSG90A1,
	BENNELLI,
	AK103,
	UNUSED_42,
	UNUSED_43,
	UNUSED_44,
	UNUSED_45,
	UNUSED_46,
	UNUSED_47,
	UNUSED_48, // 350

	//CHRISL: Default NIV items
	DEFAULT_THIGH = 428,
	DEFAULT_VEST = 434,
	DEFAULT_CPACK = 442,
	DEFAULT_BPACK = 448,

	MAXITEMS = 16001
} ITEMDEFINE;

struct AttachmentStruct
{
	UINT16 attachmentIndex;
	UINT16 itemIndex;
	UINT16 APCost;
	UINT16 NASOnly;

	bool operator<(const AttachmentStruct& a) const
	{
		bool result = false;
		if (attachmentIndex < a.attachmentIndex)
			result = true;
		else if (attachmentIndex == a.attachmentIndex)
			result = itemIndex < a.itemIndex;

		return result;
	}
};

// Flugente: in order not to loop over MAXITEMS items if we only have a few thousand, remember the actual number of items in the xml
extern UINT32 gMAXITEMS_READ;
extern UINT32 gMAXAMMOTYPES_READ;
extern UINT32 gMAXATTACHMENTS_READ;
extern UINT32 gMAXLAUNCHABLES_READ;

/* CHRISL: Arrays to track ic group information.  These allow us to determine which LBE slots control which pockets and
what LBE class the pockets are.*/
//										{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54}
// Determines which LBE Slot controls each pocket
const INT8	icLBE[NUM_INV_SLOTS] =		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,10,10,11,11,11,11, 7, 7, 8, 9, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,11,11,11,11,11,11,11,11};

// Determines the class (and default definition) of the controlling pocket
const INT8	icClass[NUM_INV_SLOTS] =	{-1,-1,-1,-1,-1,-1,-1, 5, 5, 5, 5, 5, 6, 6, 3, 3, 3, 4, 4, 4, 4, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

// Determines the pocket number to look at in LBETYPE
const INT8	icPocket[NUM_INV_SLOTS] =	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4, 5, 6, 8, 9,10,11,10,11, 4, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7};

// Determines which pockets are used in the old inventory system.
const INT8	oldInv[NUM_INV_SLOTS] =		{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Determines which pockets to use for vehicles in the new inventory system.
const INT8	vehicleInv[NUM_INV_SLOTS]=	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};

// Determines which pockets to use for Madlab's Robot in the new inventory system.
const INT8	robotInv[NUM_INV_SLOTS]=	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define		ROBOT_AMMO_SLOT			14
#define		ROBOT_TARGETING_SLOT	15
#define		ROBOT_CHASSIS_SLOT		16
#define		ROBOT_UTILITY_SLOT		17
#define		ROBOT_INVENTORY_SLOT	18

// Determines the default pocket
const INT16	icDefault[NUM_INV_SLOTS] =	{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,//0-13
	DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK,//14,15,16
	DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK,//17,18,19,20
	DEFAULT_VEST, DEFAULT_VEST,//21,22
	DEFAULT_THIGH, DEFAULT_THIGH,//23,24
	DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST, DEFAULT_VEST,//24-34
	DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH, DEFAULT_THIGH,//35-42
	DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK, DEFAULT_CPACK,//43,44,45,46
	DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK, DEFAULT_BPACK};//47+

#define FIRST_HELMET STEEL_HELMET
#define LAST_HELMET SPECTRA_HELMET_Y

#define FIRST_VEST FLAK_JACKET
#define LAST_VEST KEVLAR2_VEST_Y

#define FIRST_LEGGINGS KEVLAR_LEGGINGS
#define LAST_LEGGINGS SPECTRA_LEGGINGS_Y

#define FIRST_HEAD_ITEM EXTENDEDEAR
#define LAST_HEAD_ITEM SUNGOGGLES

#define MAXATTACHMENTS 60000

extern INVTYPE Item[MAXITEMS];
extern AttachmentStruct Attachment[MAXATTACHMENTS];
extern std::multimap<UINT16, AttachmentStruct> AttachmentBackmap;

//WarmSteel - Here we have some definitions for NAS
typedef struct
{
	UINT16	uiSlotIndex;
	CHAR16	szSlotName[200];
	UINT64	nasAttachmentClass;
	UINT64	nasLayoutClass;
	UINT16	usDescPanelPosX;
	UINT16	usDescPanelPosY;
	BOOLEAN	fMultiShot;
	BOOLEAN	fBigSlot;
	//DBrot: MOLLE
	UINT8	ubPocketMapping;
} AttachmentSlotStruct;

extern AttachmentSlotStruct AttachmentSlots[MAXITEMS+1]; 

typedef struct
{
	UINT32	uiIndex;
	UINT32	OIV;
	UINT32	NIV;
	UINT32	OAS;
	UINT32	NAS;
} ItemReplacementStruct;

extern ItemReplacementStruct ItemReplacement[MAXATTACHMENTS];

typedef struct
{
	std::vector<UINT16> usItemExclude;
	std::vector<UINT16> usItemInclude;
	std::vector<UINT16> ubWeaponClass;
	std::vector<UINT16> usAddsSlots;
	std::vector<UINT16> usRemovesSlots;

} AlterationStruct;

typedef struct
{
	UINT16	usItem;
	UINT32	uiItemClass;
	INT8		bAttachmentSkillCheck;
	INT8		bAttachmentSkillCheckMod;
	UINT32	uiIndex;
} AttachmentInfoStruct;

extern AttachmentInfoStruct AttachmentInfo[MAXITEMS+1];

extern UINT16 Launchable[MAXITEMS+1][2];
extern UINT16 CompatibleFaceItems[MAXITEMS+1][2];
extern UINT16 IncompatibleAttachments[MAXATTACHMENTS][2];

typedef enum
{
	DESTRUCTION,
	COMBINE_POINTS,
	TREAT_ARMOUR,
	EXPLOSIVE_MERGE_HARD, //Madd: renamed to hard
	EASY_MERGE,
	ELECTRONIC_MERGE,
	USE_ITEM,
	USE_ITEM_HARD,
	TEMPERATURE,
	EXPLOSIVE_MERGE_EASY, //Madd: new merge types
	MECHANICAL_MERGE_EASY,
	MECHANICAL_MERGE_HARD,
	TRIPWIRE_ROLL,
	USE_ITEM_NEW,
	CANNIBALIZE,
	LOSE_10_PERCENT,
} MergeType;

extern UINT16 Merge[MAXITEMS+1][6];

typedef struct
{
	UINT16	usItem;
	UINT16	usAttachment[MAX_DEFAULT_ATTACHMENTS]; //Madd: extended from 2 to 20, being lazy with constant reuse :p
	UINT16	usResult;
	UINT32  uiIndex;
} ComboMergeInfoStruct;

// HEADROCK HAM 5: Defining this here because we need MAXITEMS. The struct is defined earlier.
extern TransformInfoStruct Transform[MAXITEMS+1];

extern ComboMergeInfoStruct AttachmentComboMerge[MAXITEMS+1];
BOOLEAN EXPLOSIVE_GUN(UINT16 x );

typedef struct IMP_ITEM_CHOICE_TYPE
{
	UINT8		ubChoices;						// how many valid choices there are in this category
	INT16		bItemNo[50];						// room for up to 50 choices in each category
	UINT8		ubNumItems;						// # of items to select

	UINT32		uiIndex;						
} IMP_ITEM_CHOICE_TYPE;

enum
{
	IMP_DEFAULT = 0,
	IMP_RANDOMDEFAULT,
//ATTRIBUTES
	IMP_WISDOM,
	IMP_DEXTERITY,
	IMP_AGILITY,
	IMP_STRENGTH,
	IMP_HEALTH,
	IMP_LEADERSHIP,
	IMP_MARKSMANSHIP,
	IMP_EXPLOSIVE,
	IMP_MECHANICAL,
	IMP_MEDICAL,
//SKILLS - SANDRO was here
	IMP_AUTO_WEAPONS,
	IMP_HEAVY_WEAPONS,
	IMP_SNIPER,
	IMP_RANGER,
	IMP_GUNSLINGER,
	IMP_MARTIAL_ARTS,
	IMP_SQUADLEADER,
	IMP_TECHNICIAN,
	IMP_DOCTOR,
	IMP_AMBIDEXTROUS,
	IMP_MELEE,
	IMP_THROWING,
	IMP_NIGHT_OPS,
	IMP_STEALTHY,
	IMP_ATHLETICS,
	IMP_BODYBUILDING,
	IMP_DEMOLITIONS,
	IMP_TEACHING,
	IMP_SCOUTING,
	IMP_LOCKPICKING,
	IMP_ELECTRONICS,
	IMP_CAMOUFLAGED,
// DBrot: added seperate choices for experts	
	IMP_AUTO_WEAPONS_EXP,
	IMP_HEAVY_WEAPONS_EXP,
	IMP_SNIPER_EXP,
	IMP_RANGER_EXP,
	IMP_GUNSLINGER_EXP,
	IMP_MARTIAL_ARTS_EXP,
	IMP_SQUADLEADER_EXP,
	IMP_TECHNICIAN_EXP,
	IMP_DOCTOR_EXP,
	IMP_MELEE_EXP,
	IMP_THROWING_EXP,
	IMP_NIGHT_OPS_EXP,
	IMP_STEALTHY_EXP,
	IMP_TEACHING_EXP,
	IMP_LOCKPICKING_EXP,

	// Flugente: added covert ops
	IMP_COVERT,
	IMP_COVERT_EXP,

	// Flugente: added radio operator
	IMP_RADIO_OPERATOR,
	IMP_SURVIVAL,

	// silversurfer: added sidearm for standard weapons. Special weapons should come from traits.
	IMP_SIDEARM,

	MAX_IMP_ITEM_TYPES
};

// sevenfm: types for quick items
enum
{
	INV_ITEM_TYPE_UNKNOWN = 0,
	INV_ITEM_TYPE_HTH = -1,
	INV_ITEM_TYPE_MELEE = -2,
	INV_ITEM_TYPE_TASER = -3,
	INV_ITEM_TYPE_HANDCUFFS = -4,
	INV_ITEM_TYPE_SIDEARM = -5,
	INV_ITEM_TYPE_FIRSTAID = -6,
	INV_ITEM_TYPE_BINOCULARS = -7,	
	INV_ITEM_TYPE_XRAY = -8,	
	INV_ITEM_TYPE_METALDETECTOR = -9,
};

extern IMP_ITEM_CHOICE_TYPE gIMPItemChoices[MAX_IMP_ITEM_TYPES];

// rftr: better LBE tooltips. see NasAttachmentClass.xml
#define MOLLE_SMALL 4096
#define MOLLE_MEDIUM 8192

#endif
