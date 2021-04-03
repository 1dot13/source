#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
#include "saveloadgame.h"
#else
	#include "sgp.h"
	#include "Soldier Create.h"
	#include "overhead.h"
	#include "wcheck.h"
	#include "soldier profile.h"
	#include "animation control.h"
	#include "animation data.h"
	#include "Sound Control.h"
	#include "overhead types.h"
	#include "faces.h"
	#include "isometric utils.h"
	#include "items.h"
	#include "weapons.h"
	#include "opplist.h"
	#include "random.h"
	#include "Assignments.h"
	#include "Soldier Init List.h"
	#include "Smell.h"
	#include "Squads.h"
	#include "Interface Panels.h"
	#include "Strategicmap.h"
	#include "Inventory Choosing.h"
	#include "Queen Command.h"
	#include "Soldier Add.h"
	#include "SaveLoadGame.h"
	#include "Quests.h"
	#include "Sys Globals.h"
	#include "Scheduling.h"
	#include "Rotting Corpses.h"
	#include "vehicles.h"
	#include "handle ui.h"
	#include "text.h"
	#include "Campaign.h"
	#include "GameSettings.h"
	#include "PreBattle Interface.h"
	#include "Auto Resolve.h"
	#include "Morale.h"
	#include "ai.h"
	#include "Strategic Mines.h"
	#include "math.h"
	#include "Game Clock.h" // added this one - SANDRO
	#include "Interface.h"	// added by Flugente
	#include "Soldier macros.h"		// added by Flugente
	#include "MilitiaIndividual.h"	// added by Flugente
#endif

#include "connect.h"
#include "message.h"
#include "fresh_header.h"

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

#include "GameInitOptionsScreen.h"

// THESE 3 DIFFICULTY FACTORS MUST ALWAYS ADD UP TO 100% EXACTLY!!!
#define DIFF_FACTOR_PLAYER_PROGRESS			50
#define DIFF_FACTOR_PALACE_DISTANCE			30
#define DIFF_FACTOR_GAME_DIFFICULTY			20

// additional difficulty modifiers
#define DIFF_MODIFIER_SOME_PROGRESS			+5
#define DIFF_MODIFIER_NO_INCOME					-5
#define DIFF_MODIFIER_DRASSEN_MILITIA		+10


#define PALACE_SECTOR_X 3
#define PALACE_SECTOR_Y 16

#define MAX_PALACE_DISTANCE		20

INT8 bNumSquadleadersInArmy = 0; // added by SANDRO
// HEADROCK HAM 5: Read target coolness by sector
UINT32 gCoolnessBySector[256];

OLD_SOLDIERCREATE_STRUCT_101::OLD_SOLDIERCREATE_STRUCT_101() {
	initialize();
}

// Copy Constructor
OLD_SOLDIERCREATE_STRUCT_101::OLD_SOLDIERCREATE_STRUCT_101(const OLD_SOLDIERCREATE_STRUCT_101& src) {
	memcpy(this, &src, SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD);
	this->Inv = src.Inv;
}

// Assignment operator
OLD_SOLDIERCREATE_STRUCT_101& OLD_SOLDIERCREATE_STRUCT_101::operator=(const OLD_SOLDIERCREATE_STRUCT_101& src)
{
	if (this != &src) {
		memcpy(this, &src, SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD);
		this->Inv = src.Inv;
	}
	return *this;
}

//dnl ch33 130909
OLD_SOLDIERCREATE_STRUCT_101& OLD_SOLDIERCREATE_STRUCT_101::operator=(SOLDIERCREATE_STRUCT& src)
{
	if((void*)this != (void*)&src)
	{
		this->Inv = src.Inv;//had no meaning for 1.12 but let it be

		this->DO_NOT_USE_Inv[OldInventory::HELMETPOS] = src.Inv[HELMETPOS];
		this->DO_NOT_USE_Inv[OldInventory::VESTPOS] = src.Inv[VESTPOS];
		this->DO_NOT_USE_Inv[OldInventory::LEGPOS] = src.Inv[LEGPOS];
		this->DO_NOT_USE_Inv[OldInventory::HEAD1POS] = src.Inv[HEAD1POS];
		this->DO_NOT_USE_Inv[OldInventory::HEAD2POS] = src.Inv[HEAD2POS];
		this->DO_NOT_USE_Inv[OldInventory::HANDPOS] = src.Inv[HANDPOS];
		this->DO_NOT_USE_Inv[OldInventory::SECONDHANDPOS] = src.Inv[SECONDHANDPOS];
		this->DO_NOT_USE_Inv[OldInventory::BIGPOCK1POS] = src.Inv[BIGPOCK1POS];
		this->DO_NOT_USE_Inv[OldInventory::BIGPOCK2POS] = src.Inv[BIGPOCK2POS];
		this->DO_NOT_USE_Inv[OldInventory::BIGPOCK3POS] = src.Inv[BIGPOCK3POS];
		this->DO_NOT_USE_Inv[OldInventory::BIGPOCK4POS] = src.Inv[BIGPOCK4POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK1POS] = src.Inv[SMALLPOCK1POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK2POS] = src.Inv[SMALLPOCK2POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK3POS] = src.Inv[SMALLPOCK3POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK4POS] = src.Inv[SMALLPOCK4POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK5POS] = src.Inv[SMALLPOCK5POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK6POS] = src.Inv[SMALLPOCK6POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK7POS] = src.Inv[SMALLPOCK7POS];
		this->DO_NOT_USE_Inv[OldInventory::SMALLPOCK8POS] = src.Inv[SMALLPOCK8POS];

		memcpy(&(this->name), &(src.name), sizeof(CHAR16)*10);
		memcpy(&(this->HeadPal), &(src.HeadPal), sizeof(PaletteRepID));
		memcpy(&(this->PantsPal), &(src.PantsPal), sizeof(PaletteRepID));
		memcpy(&(this->VestPal), &(src.VestPal), sizeof(PaletteRepID));
		memcpy(&(this->SkinPal), &(src.SkinPal), sizeof(PaletteRepID));
		memcpy(&(this->MiscPal), &(src.MiscPal), sizeof(PaletteRepID));
		TranslateArrayFields(this->sPatrolGrid, src.sPatrolGrid, OLD_MAXPATROLGRIDS, INT32_INT16);

		this->bAgility = src.bAgility;
		this->bAIMorale = src.bAIMorale;
		this->bAttitude = src.bAttitude;
		this->ubBodyType = src.ubBodyType;
		this->bDexterity = src.bDexterity;
		this->ubDirection = src.ubDirection;
		this->bExpLevel = src.bExpLevel;
		this->bExplosive = src.bExplosive;
		this->bLeadership = src.bLeadership;
		this->bLife = src.bLife;
		this->bLifeMax = src.bLifeMax;
		this->bMarksmanship = src.bMarksmanship;
		this->bMechanical = src.bMechanical;
		this->bMedical = src.bMedical;
		this->bMorale = src.bMorale;
		this->bOrders = src.bOrders;
		this->bPatrolCnt = src.bPatrolCnt;
		this->bSectorZ = src.bSectorZ;
		this->bStrength = src.bStrength;
		this->bTeam = src.bTeam;
		this->bUseGivenVehicleID = src.bUseGivenVehicleID;
		this->bWisdom = src.bWisdom;
		this->fCopyProfileItemsOver = src.fCopyProfileItemsOver;
		this->fHasKeys = src.fHasKeys;
		this->fKillSlotIfOwnerDies = src.fKillSlotIfOwnerDies;
		this->fOnRoof = src.fOnRoof;
		this->fPlayerMerc = src.fPlayerMerc;
		this->fPlayerPlan = src.fPlayerPlan;
		this->fStatic = src.fStatic;
		this->fUseExistingSoldier = src.fUseExistingSoldier;
		this->fUseGivenVehicle = src.fUseGivenVehicle;
		this->fVisible = src.fVisible;
		this->pExistingSoldier = src.pExistingSoldier;
		this->sInsertionGridNo = src.sInsertionGridNo;
		this->sSectorX = src.sSectorX;
		this->sSectorY = src.sSectorY;
		this->ubCivilianGroup = src.ubCivilianGroup;
		this->ubProfile = src.ubProfile;
		this->ubScheduleID = src.ubScheduleID;
		this->ubSoldierClass = src.ubSoldierClass;
	}
	return(*this);
}

// Destructor
OLD_SOLDIERCREATE_STRUCT_101::~OLD_SOLDIERCREATE_STRUCT_101() {
}

// Initialize the soldier.
//	Use this instead of the old method of calling memset!
//	Note that the constructor does this automatically.
void OLD_SOLDIERCREATE_STRUCT_101::initialize() {
	memset( this, 0, SIZEOF_OLD_SOLDIERCREATE_STRUCT_101_POD);
	Inv.clear();
}





// Conversion operator from old to new
SOLDIERCREATE_STRUCT& SOLDIERCREATE_STRUCT::operator=(const OLD_SOLDIERCREATE_STRUCT_101& src)
{
	if ((void*)this != (void*)&src) {
		this->Inv = src.Inv;

		memcpy( &(this->name), &(src.name), sizeof(CHAR16) * 10 );
		memcpy( &(this->HeadPal), &(src.HeadPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->PantsPal), &(src.PantsPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->VestPal), &(src.VestPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->SkinPal), &(src.SkinPal), sizeof(PaletteRepID) );	// 30
		memcpy( &(this->MiscPal), &(src.MiscPal), sizeof(PaletteRepID) );	// 30
		TranslateArrayFields(this->sPatrolGrid, src.sPatrolGrid, OLD_MAXPATROLGRIDS, INT16_INT32);//dnl ch27 230909
		this->bAgility = src.bAgility;
		this->bAIMorale = src.bAIMorale;
		this->bAttitude = src.bAttitude;
		this->ubBodyType = src.ubBodyType;
		this->bDexterity = src.bDexterity;
		this->ubDirection = src.ubDirection;
		this->bExpLevel = src.bExpLevel;
		this->bExplosive = src.bExplosive;
		this->bLeadership = src.bLeadership;
		this->bLife = src.bLife;
		this->bLifeMax = src.bLifeMax;
		this->bMarksmanship = src.bMarksmanship;
		this->bMechanical = src.bMechanical;
		this->bMedical = src.bMedical;
		this->bMorale = src.bMorale;
		this->bOrders = src.bOrders;
		this->bPatrolCnt = src.bPatrolCnt;
		this->bSectorZ = src.bSectorZ;
		this->bStrength = src.bStrength;
		this->bTeam = src.bTeam;
		this->bUseGivenVehicleID = src.bUseGivenVehicleID;
		this->bWisdom = src.bWisdom;
		this->fCopyProfileItemsOver = src.fCopyProfileItemsOver;
		this->fHasKeys = src.fHasKeys;
		this->fKillSlotIfOwnerDies = src.fKillSlotIfOwnerDies;
		this->fOnRoof = src.fOnRoof;
		this->fPlayerMerc = src.fPlayerMerc;
		this->fPlayerPlan = src.fPlayerPlan;
		this->fStatic = src.fStatic;
		this->fUseExistingSoldier = src.fUseExistingSoldier;
		this->fUseGivenVehicle = src.fUseGivenVehicle;
		this->fVisible = src.fVisible;
		this->pExistingSoldier = src.pExistingSoldier;
		this->sInsertionGridNo = src.sInsertionGridNo;
		this->sSectorX = src.sSectorX;
		this->sSectorY = src.sSectorY;
		this->ubCivilianGroup = src.ubCivilianGroup;
		this->ubProfile = src.ubProfile;
		this->ubScheduleID = src.ubScheduleID;
		this->ubSoldierClass = src.ubSoldierClass;
	}
	return *this;
}

// Conversion operator from SOLDIERTYPE to SOLDIERCREATE_STRUCT
SOLDIERCREATE_STRUCT& SOLDIERCREATE_STRUCT::operator=(const SOLDIERTYPE& Soldier)
{
	//WARNING, this may not copy all data you expect over, I'm not sure
	//but it does copy all the data from the previous function
	//Copy over the data of the soldier.
	this->ubProfile							= NO_PROFILE;
	this->bLife									= Soldier.stats.bLife;
	this->bLifeMax							= Soldier.stats.bLifeMax;
	this->bAgility							= Soldier.stats.bAgility;
	this->bDexterity						= Soldier.stats.bDexterity;
	this->bExpLevel							= Soldier.stats.bExpLevel;
	this->bMarksmanship					= Soldier.stats.bMarksmanship;
	this->bMedical							= Soldier.stats.bMedical;
	this->bMechanical						= Soldier.stats.bMechanical;
	this->bExplosive						= Soldier.stats.bExplosive;
	this->bLeadership						= Soldier.stats.bLeadership;
	this->bStrength							= Soldier.stats.bStrength;
	this->bWisdom								= Soldier.stats.bWisdom;
	this->bAttitude							= Soldier.aiData.bAttitude;
	this->bOrders								= Soldier.aiData.bOrders;
	this->bMorale								= Soldier.aiData.bMorale;
	this->bAIMorale							= Soldier.aiData.bAIMorale;
	this->ubBodyType							= Soldier.ubBodyType;
	this->ubCivilianGroup				= Soldier.ubCivilianGroup;
	this->ubScheduleID					= Soldier.ubScheduleID;
	this->fHasKeys							= Soldier.flags.bHasKeys;
	this->sSectorX							= Soldier.sSectorX;
	this->sSectorY							= Soldier.sSectorY;
	this->bSectorZ							= Soldier.bSectorZ;
	this->ubSoldierClass				= Soldier.ubSoldierClass;
	this->bTeam									= Soldier.bTeam;
	this->ubDirection						= Soldier.ubDirection;

	this->fOnRoof								= Soldier.pathing.bLevel;
	this->sInsertionGridNo			= Soldier.sGridNo;

	swprintf( this->name, Soldier.name );

	//Copy patrol points
	this->bPatrolCnt						= Soldier.aiData.bPatrolCnt;
	memcpy( this->sPatrolGrid, Soldier.aiData.sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );

	//copy colors for soldier based on the body type.
	sprintf( this->HeadPal,		Soldier.HeadPal );
	sprintf( this->VestPal,		Soldier.VestPal );
	sprintf( this->SkinPal,		Soldier.SkinPal );
	sprintf( this->PantsPal,	Soldier.PantsPal );
	sprintf( this->MiscPal,		Soldier.MiscPal );

	//copy soldier's inventory
	this->Inv = Soldier.inv;
	return *this;
}

UINT16 SOLDIERCREATE_STRUCT::GetChecksum()
{
	return (
	this->bLife								* 7		+
	this->bLifeMax							* 8		-
	this->bAgility							* 2		+
	this->bDexterity						* 1		+
	this->bExpLevel							* 5		-
	this->bMarksmanship						* 9		+
	this->bMedical							* 10	+
	this->bMechanical						* 3		+
	this->bExplosive						* 4		+
	this->bLeadership						* 5		+
	this->bStrength							* 7		+
	this->bWisdom							* 11	+
	this->bMorale							* 7		+
	this->bAIMorale							* 3		-
	this->ubBodyType							* 7		+
	4										* 6		+
	this->sSectorX							* 7		-
	this->ubSoldierClass					* 4		+
	this->bTeam								* 7		+
	this->ubDirection						* 5		+
	this->fOnRoof							* 17	+
	this->sInsertionGridNo					* 1		+
	3);
}


SOLDIERCREATE_STRUCT::SOLDIERCREATE_STRUCT() {
	initialize();
}

// Copy Constructor
SOLDIERCREATE_STRUCT::SOLDIERCREATE_STRUCT(const SOLDIERCREATE_STRUCT& src) {
	memcpy(this, &src, SIZEOF_SOLDIERCREATE_STRUCT_POD);
	this->Inv = src.Inv;
}

// Assignment operator
SOLDIERCREATE_STRUCT& SOLDIERCREATE_STRUCT::operator=(const SOLDIERCREATE_STRUCT& src)
{
	if (this != &src) {
		memcpy(this, &src, SIZEOF_SOLDIERCREATE_STRUCT_POD);
		this->Inv = src.Inv;
	}
	return *this;
}

//dnl ch42 290909
SOLDIERCREATE_STRUCT& SOLDIERCREATE_STRUCT::operator=(const _OLD_SOLDIERCREATE_STRUCT& src)
{
	if((void*)this != (void*)&src)
	{
		Inv = src.Inv;
		memcpy(name, src.name, sizeof(CHAR16)*10);
		memcpy(&HeadPal, &src.HeadPal, sizeof(PaletteRepID));
		memcpy(&PantsPal, &src.PantsPal, sizeof(PaletteRepID));
		memcpy(&VestPal, &src.VestPal, sizeof(PaletteRepID));
		memcpy(&SkinPal, &src.SkinPal, sizeof(PaletteRepID));
		memcpy(&MiscPal, &src.MiscPal, sizeof(PaletteRepID));
		TranslateArrayFields(sPatrolGrid, src.sPatrolGrid, OLD_MAXPATROLGRIDS, INT16_INT32);
		bAgility = src.bAgility;
		bAIMorale = src.bAIMorale;
		bAttitude = src.bAttitude;
		ubBodyType = src.ubBodyType;
		bDexterity = src.bDexterity;
		ubDirection = src.ubDirection;
		bExpLevel = src.bExpLevel;
		bExplosive = src.bExplosive;
		bLeadership = src.bLeadership;
		bLife = src.bLife;
		bLifeMax = src.bLifeMax;
		bMarksmanship = src.bMarksmanship;
		bMechanical = src.bMechanical;
		bMedical = src.bMedical;
		bMorale = src.bMorale;
		bOrders = src.bOrders;
		bPatrolCnt = src.bPatrolCnt;
		bSectorZ = src.bSectorZ;
		bStrength = src.bStrength;
		bTeam = src.bTeam;
		bUseGivenVehicleID = src.bUseGivenVehicleID;
		bWisdom = src.bWisdom;
		fCopyProfileItemsOver = src.fCopyProfileItemsOver;
		fHasKeys = src.fHasKeys;
		fKillSlotIfOwnerDies = src.fKillSlotIfOwnerDies;
		fOnRoof = src.fOnRoof;
		fPlayerMerc = src.fPlayerMerc;
		fPlayerPlan = src.fPlayerPlan;
		fStatic = src.fStatic;
		fUseExistingSoldier = src.fUseExistingSoldier;
		fUseGivenVehicle = src.fUseGivenVehicle;
		fVisible = src.fVisible;
		pExistingSoldier = src.pExistingSoldier;
		sInsertionGridNo = src.sInsertionGridNo;
		sSectorX = src.sSectorX;
		sSectorY = src.sSectorY;
		ubCivilianGroup = src.ubCivilianGroup;
		ubProfile = src.ubProfile;
		ubScheduleID = src.ubScheduleID;
		ubSoldierClass = src.ubSoldierClass;
	}
	return(*this);
}

// Destructor
SOLDIERCREATE_STRUCT::~SOLDIERCREATE_STRUCT() {
}

// Initialize the soldier.
//	Use this instead of the old method of calling memset!
//	Note that the constructor does this automatically.
void SOLDIERCREATE_STRUCT::initialize() {
	memset( this, 0, SIZEOF_SOLDIERCREATE_STRUCT_POD);
	Inv.clear();
}


// Ugly temporary solution
//
// These two functions map the "old" style inventory (fixed array of ) to the new (a flexibly sized vector).
// If you change names or eliminate some positions or such you need to change these.
// Eventually the need for these functions will disappear.

void OLD_SOLDIERCREATE_STRUCT_101::CopyOldInventoryToNew() {
	// Do not use a loop in case the new inventory slots are arranged differently than the old
	Inv[HELMETPOS] = DO_NOT_USE_Inv[OldInventory::HELMETPOS];
	Inv[VESTPOS] = DO_NOT_USE_Inv[OldInventory::VESTPOS];
	Inv[LEGPOS] = DO_NOT_USE_Inv[OldInventory::LEGPOS];
	Inv[HEAD1POS] = DO_NOT_USE_Inv[OldInventory::HEAD1POS];
	Inv[HEAD2POS] = DO_NOT_USE_Inv[OldInventory::HEAD2POS];
	Inv[HANDPOS] = DO_NOT_USE_Inv[OldInventory::HANDPOS];
	Inv[SECONDHANDPOS] = DO_NOT_USE_Inv[OldInventory::SECONDHANDPOS];
	Inv[BIGPOCK1POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK1POS];
	Inv[BIGPOCK2POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK2POS];
	Inv[BIGPOCK3POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK3POS];
	Inv[BIGPOCK4POS] = DO_NOT_USE_Inv[OldInventory::BIGPOCK4POS];
	Inv[SMALLPOCK1POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK1POS];
	Inv[SMALLPOCK2POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK2POS];
	Inv[SMALLPOCK3POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK3POS];
	Inv[SMALLPOCK4POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK4POS];
	Inv[SMALLPOCK5POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK5POS];
	Inv[SMALLPOCK6POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK6POS];
	Inv[SMALLPOCK7POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK7POS];
	Inv[SMALLPOCK8POS] = DO_NOT_USE_Inv[OldInventory::SMALLPOCK8POS];
}
/*
void OLD_SOLDIERCREATE_STRUCT_101::CopyNewInventoryToOld() {
	// Do not use a loop in case the new inventory slots are arranged differently than the old
	DO_NOT_USE_Inv[OldInventory::HELMETPOS] = Inv[HELMETPOS];
	DO_NOT_USE_Inv[OldInventory::VESTPOS] = Inv[VESTPOS];
	DO_NOT_USE_Inv[OldInventory::LEGPOS] = Inv[LEGPOS];
	DO_NOT_USE_Inv[OldInventory::HEAD1POS] = Inv[HEAD1POS];
	DO_NOT_USE_Inv[OldInventory::HEAD2POS] = Inv[HEAD2POS];
	DO_NOT_USE_Inv[OldInventory::HANDPOS] = Inv[HANDPOS];
	DO_NOT_USE_Inv[OldInventory::SECONDHANDPOS] = Inv[SECONDHANDPOS];
	DO_NOT_USE_Inv[OldInventory::BIGPOCK1POS] = Inv[BIGPOCK1POS];
	DO_NOT_USE_Inv[OldInventory::BIGPOCK2POS] = Inv[BIGPOCK2POS];
	DO_NOT_USE_Inv[OldInventory::BIGPOCK3POS] = Inv[BIGPOCK3POS];
	DO_NOT_USE_Inv[OldInventory::BIGPOCK4POS] = Inv[BIGPOCK4POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK1POS] = Inv[SMALLPOCK1POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK2POS] = Inv[SMALLPOCK2POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK3POS] = Inv[SMALLPOCK3POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK4POS] = Inv[SMALLPOCK4POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK5POS] = Inv[SMALLPOCK5POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK6POS] = Inv[SMALLPOCK6POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK7POS] = Inv[SMALLPOCK7POS];
	DO_NOT_USE_Inv[OldInventory::SMALLPOCK8POS] = Inv[SMALLPOCK8POS];
}
*/


//Private functions used within TacticalCreateStruct()
void InitSoldierStruct( SOLDIERTYPE *pSoldier );
BOOLEAN TacticalCopySoldierFromProfile( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct );
BOOLEAN TacticalCopySoldierFromCreateStruct( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct );
void CopyProfileItems( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct );
UINT8 GetLocationModifier( UINT8 ubSoldierClass );
void ReduceHighExpLevels( INT8 *pbExpLevel );


BOOLEAN gfProfiledEnemyAdded = FALSE;

UINT32 guiCurrentUniqueSoldierId = 1;

// CJC note: trust me, it's easiest just to put this here; this is the only
// place it should need to be used
/* CHRISL: There's nothing to fix here.  This array is only used on non-hireable NPCs to designate whether an object can
be dropped or not.  And we only have an 8bit field to store these flags.  So there's no need to change anything.  New or
Old inventory system, NPC profiles use the old style pocket system.*/
//ADB actually that's not correct, all NPC profiles use the new pocket system.
//To verify this, give Fatima the letter then check what slot it is in when she is in A10.
//AFAIK there is no way to seperate non hireable NPCs from hireable NPCs, so to fix Ira all NPC profiles are adjusted.
/* CHRISL: You can't make change to this.  You can add values to the end if you want, but prof.dat requires
this setup.  That's one of the many reasons I setup the new inventory system to NOT be used by computer
controlled characters. */
UINT8 gubItemDroppableFlag[NUM_INV_SLOTS] =
{
	0x01,
	0x02,
	0x04,
	0,
	0,
	0x08,
	0,
	0,	//0x10,
	0,	//0x20,
	0,	//0x40,
	0,	//0x80,
	0,
	0,
	0,
	0x10,	//0,
	0x20,	//0,
	0x40,	//0,
	0x80,	//0,
	0
};


void RandomizeNewSoldierStats( SOLDIERCREATE_STRUCT *pCreateStruct )
{
	pCreateStruct->bLifeMax								= (UINT8)Random(50)+50;
	pCreateStruct->bLife								= pCreateStruct->bLifeMax;
	pCreateStruct->bAgility								= (UINT8)Random(50)+50;
	pCreateStruct->bDexterity							= (UINT8)Random(50)+50;
	pCreateStruct->bExpLevel							= 1 + (UINT8)Random(4);

	// Randomize skills (for now)
	pCreateStruct->bMarksmanship					= (UINT8)Random(50)+50;
	pCreateStruct->bMedical								= (UINT8)Random(50)+50;
	pCreateStruct->bMechanical						= (UINT8)Random(50)+50;
	pCreateStruct->bExplosive							= (UINT8)Random(50)+50;
	pCreateStruct->bLeadership						= (UINT8)Random(50)+50;
	pCreateStruct->bStrength							= (UINT8)Random(50)+50;
	pCreateStruct->bWisdom								= (UINT8)Random(50)+50;
	pCreateStruct->bAttitude							= (INT8) Random( MAXATTITUDES );
	pCreateStruct->bOrders								= FARPATROL;
	pCreateStruct->bMorale								= 50;
	pCreateStruct->bAIMorale							= MORALE_FEARLESS;
}

void DecideToAssignSniperOrders( SOLDIERCREATE_STRUCT * pp )
{
	//Madd: decide if soldier should be a sniper

	if ( pp->bOrders == STATIONARY || pp->bOrders == ONGUARD )
	{
		//CHRISL: Externalized values and added ability to restrict non-SR Snipers to Elite only
		if(Weapon[pp->Inv[HANDPOS].usItem].ubWeaponType != GUN_SN_RIFLE && GunRange(&pp->Inv[HANDPOS], NULL) >= gGameExternalOptions.fAISniperRange && IsScoped(&pp->Inv[HANDPOS]) && Random(100) < gGameExternalOptions.fAISniperChance){
			if((gGameExternalOptions.fAISniperElite == TRUE && pp->ubSoldierClass == SOLDIER_CLASS_ELITE) || gGameExternalOptions.fAISniperElite == FALSE)
				pp->bOrders = SNIPER;
		}
		else if(Weapon[pp->Inv[HANDPOS].usItem].ubWeaponType == GUN_SN_RIFLE && IsScoped(&pp->Inv[HANDPOS]) && Random(100) < gGameExternalOptions.fAISniperChanceWithSR)
			pp->bOrders = SNIPER;

		DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Sniper Created") );
	}
}


// Flugente: disease in a sector affects the health of any NPC spawning there.
// This causes an issue if someone enters a sector from an adjacent sector - they should be affected by their point of origin, not the current sector
// For this reason, we add a little helper variable that stores such a sector.
INT16 gsStrategicDiseaseOriginSector = -1;

SOLDIERTYPE* TacticalCreateSoldier( SOLDIERCREATE_STRUCT *pCreateStruct, UINT8 *pubID )
{
	SOLDIERTYPE			Soldier;
	INT32						cnt;
	SOLDIERTYPE			*pTeamSoldier;
	BOOLEAN					fGuyAvail = FALSE;
	UINT8						bLastTeamID;
	UINT8						ubVehicleID = 0;
	
	*pubID = NOBODY;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("TacticalCreateSoldier"));

	INT8 tbTeam;
	BOOLEAN tfPP;
	tbTeam=pCreateStruct->bTeam;
	tfPP=pCreateStruct->fPlayerPlan; //used as temp indicator of struct sent from the server //hayden.

	if(is_networked)
	{
		if(is_networked && (pCreateStruct->fOnRoof==1))
		{
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"skipping roof merc");
			return NULL;
		}

		if ( is_client && !is_server && (tbTeam > OUR_TEAM && tbTeam < PLAYER_PLAN) && tfPP == 0 )
		{
			return NULL; // pure client to not spawn AI unless from server, Hayden.
			//gTacticalStatus.Team[ tbTeam ].bTeamActive=0;
		}
		if ( is_server && tbTeam > OUR_TEAM && tbTeam < PLAYER_PLAN )
		{
			send_AI(pCreateStruct,pubID);
		}
		if(is_client && !is_server && tfPP==1)
		{
			pCreateStruct->fPlayerPlan = 0;
		}

		if ( is_networked && (ARMED_VEHICLE( pCreateStruct ) || ENEMYROBOT( pCreateStruct )) )
		{
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"skipping tank/jeep/robot");
			return NULL;
		}
	}

	//hayden
		//Kris:
	//Huge no no!	See the header file for description of static detailed placements.
	//If this expression ever evaluates to true, then it will expose serious problems.
	//Simply returning won't help.
	if( pCreateStruct->fStatic )
	{
		Assert( 0 );
	}

	// Some values initialized here but could be changed before going to the common one
	InitSoldierStruct( &Soldier );

	Soldier.uiUniqueSoldierIdValue = guiCurrentUniqueSoldierId;

	guiCurrentUniqueSoldierId++;

	// Flugente: if this is a miltia, set individual ID
	// do not do so during loading of a savegame
	if ( tbTeam == MILITIA_TEAM && !pCreateStruct->fUseExistingSoldier )
	{
		Soldier.usIndividualMilitiaID = GetIdOfUnusedIndividualMilitia( pCreateStruct->ubSoldierClass, SECTOR( pCreateStruct->sSectorX, pCreateStruct->sSectorY ) );
	}

	// OK, CHECK IF WE HAVE A VALID PROFILE ID!
	if ( pCreateStruct->ubProfile != NO_PROFILE )
	{
		// We have a merc created by profile, do this!
		TacticalCopySoldierFromProfile( &Soldier, pCreateStruct );
	}
	else
	{
		TacticalCopySoldierFromCreateStruct( &Soldier, pCreateStruct );
	}

	// If we are NOT creating an existing soldier ( ie, this is not from a save game ), create soldier normally
	if( !pCreateStruct->fUseExistingSoldier )
	{
		// We want to determine what team to place these guys in...

		// First off, force player team if they are a player guy! ( do some other stuff for only our guys!
		if ( pCreateStruct->fPlayerMerc )
		{
			Soldier.flags.uiStatusFlags |= SOLDIER_PC;
			Soldier.bTeam = gbPlayerNum;
			Soldier.bVisible = 1;
		}
		else if ( pCreateStruct->fPlayerPlan )
		{
			Soldier.flags.uiStatusFlags |= SOLDIER_PC;
			Soldier.bVisible = 1;
		}
		else
		{
			Soldier.flags.uiStatusFlags |= SOLDIER_ENEMY;
		}


		// Check for auto team
		if ( pCreateStruct->bTeam == SOLDIER_CREATE_AUTO_TEAM )
		{
			// Auto determine!
			// OK, if this is our guy, set team as ours!
			if ( pCreateStruct->fPlayerMerc )
			{
				Soldier.bTeam = OUR_TEAM;
				Soldier.aiData.bNormalSmell = NORMAL_HUMAN_SMELL_STRENGTH;
			}
			else if ( pCreateStruct->fPlayerPlan )
			{
				Soldier.bTeam = PLAYER_PLAN;
			}
			else
			{
				// LOOK AT BODY TYPE!
				switch ( pCreateStruct->ubBodyType )
				{
					case REGMALE:
					case BIGMALE:
					case STOCKYMALE:
					case REGFEMALE:

						Soldier.bTeam = ENEMY_TEAM;
						break;

					case ADULTFEMALEMONSTER:
					case AM_MONSTER:
					case YAF_MONSTER:
					case YAM_MONSTER:
					case LARVAE_MONSTER:
					case INFANT_MONSTER:
					case QUEENMONSTER:

						Soldier.bTeam = CREATURE_TEAM;
						break;

					case FATCIV:
					case MANCIV:
					case MINICIV:
					case DRESSCIV:
					case HATKIDCIV:
					case KIDCIV:
					case COW:
					case CROW:
					case ROBOTNOWEAPON:

						Soldier.bTeam = CIV_TEAM;
						break;

				}
				
				// enemy robot special case
				if (pCreateStruct->ubBodyType == ROBOTNOWEAPON && pCreateStruct->ubSoldierClass == SOLDIER_CLASS_ROBOT)
				{
					Soldier.bTeam = ENEMY_TEAM;
				}

			}
		}
		else
		{
			Soldier.bTeam = pCreateStruct->bTeam;
			// if WE_SEE_WHAT_MILITIA_SEES
			if ( Soldier.bTeam == MILITIA_TEAM )
			{
				Soldier.bVisible = 1;
			}
		}

		// Copy the items over for the soldier, only if we have a valid profile id!
		if ( pCreateStruct->ubProfile != NO_PROFILE )
		{
			CopyProfileItems( &Soldier, pCreateStruct );
			//CHRISL: make sure nails gets his jacket no matter what
			if(pCreateStruct->ubProfile == 34 && pCreateStruct->fCopyProfileItemsOver == FALSE)
			{
				CreateItem(LEATHER_JACKET, 100, &Soldier.inv[VESTPOS]);
			}
		}

		// Given team, get an ID for this guy!

		if( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			cnt = gTacticalStatus.Team[ Soldier.bTeam ].bFirstID;

			// ATE: If we are a vehicle, and a player, start at a different slot ( 2 - max )
			if( Soldier.ubBodyType == HUMVEE ||
					Soldier.ubBodyType == ELDORADO ||
					Soldier.ubBodyType == ICECREAMTRUCK ||
					Soldier.ubBodyType == JEEP )
			{
				if( Soldier.bTeam == gbPlayerNum )
				{
					cnt = gTacticalStatus.Team[ Soldier.bTeam ].bLastID - gGameExternalOptions.ubGameMaximumNumberOfPlayerVehicles+1;
				}
			}

			bLastTeamID = gTacticalStatus.Team[ Soldier.bTeam ].bLastID;

			// look for all mercs on the same team,
			for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pTeamSoldier++)
			{
				if ( !pTeamSoldier->bActive )
				{
					fGuyAvail = TRUE;
					break;
				}
			}

			// Check if there was space!
			if ( !fGuyAvail )
			{ //No space, so can't create the soldier.
				return NULL;
			}

			// OK, set ID
			Soldier.ubID = (UINT8)cnt;
			*pubID = Soldier.ubID;
		}

		// LOAD MERC's FACE!
		if ( pCreateStruct->ubProfile != NO_PROFILE && Soldier.bTeam == OUR_TEAM )
		{
			Soldier.iFaceIndex = InitSoldierFace( &Soldier );
		}

		Soldier.bActionPoints					= Soldier.CalcActionPoints(	);
		Soldier.bInitialActionPoints			= Soldier.bActionPoints;
		Soldier.bSide							= gTacticalStatus.Team[ Soldier.bTeam ].bSide;
		Soldier.bActive							= TRUE;
		Soldier.sSectorX						= pCreateStruct->sSectorX;
		Soldier.sSectorY						= pCreateStruct->sSectorY;
		Soldier.bSectorZ						= pCreateStruct->bSectorZ;
		Soldier.ubInsertionDirection			= pCreateStruct->ubDirection;
		Soldier.pathing.bDesiredDirection		= pCreateStruct->ubDirection;
		Soldier.aiData.bDominantDir				= pCreateStruct->ubDirection;
		Soldier.ubDirection						= pCreateStruct->ubDirection;

		Soldier.sInsertionGridNo				= pCreateStruct->sInsertionGridNo;
		Soldier.bOldLife						= Soldier.stats.bLifeMax;

		// set custom side for civilian group
		if (Soldier.bTeam == CIV_TEAM &&
			Soldier.ubCivilianGroup != NON_CIV_GROUP &&
			zCivGroupName[Soldier.ubCivilianGroup].fCustomSide &&
			zCivGroupName[Soldier.ubCivilianGroup].bSide >= 0)
		{
			Soldier.bSide = zCivGroupName[Soldier.ubCivilianGroup].bSide;
		}

		// Flugente: disease can affect a soldier's health
		// not for us, and not for individual militia (their health is affected by their hourly healing instead)
		if ( gGameExternalOptions.fDisease && gGameExternalOptions.fDiseaseStrategic && Soldier.bTeam != OUR_TEAM && Soldier.bTeam != CREATURE_TEAM && !ARMED_VEHICLE((&Soldier)) && !ENEMYROBOT((&Soldier)) &&
			!( Soldier.bTeam == MILITIA_TEAM && gGameExternalOptions.fIndividualMilitia && gGameExternalOptions.fIndividualMilitia_ManageHealth )  )
		{
			UINT8 sector = SECTOR( Soldier.sSectorX, Soldier.sSectorY );
			
			// if this is autoresolve, we have to get the sector in a different way..
			if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
				sector = GetAutoResolveSectorID( );

			if ( gsStrategicDiseaseOriginSector > 0 )
				sector = (UINT8)gsStrategicDiseaseOriginSector;
			
			SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

			if ( pSectorInfo )
			{
				INT32 diseaseamount = (Disease[0].sInfectionPtsFull * pSectorInfo->fDiseasePoints) / DISEASE_MAX_SECTOR;
				Soldier.AddDiseasePoints( 0, diseaseamount );

				// if disease has broken out, lower life points
				if ( Soldier.sDiseaseFlag[0] & SOLDIERDISEASE_OUTBREAK )
				{
					// we only alter breath and life points here, stats effectivity will be handled automatically
					FLOAT magnitude = Soldier.GetDiseaseMagnitude( 0 );

					UINT16 diseasemaxbreathreduction = Disease[0].usMaxBreath * magnitude;

					Soldier.bBreathMax = min( Soldier.bBreathMax, 100 - diseasemaxbreathreduction );
					Soldier.bBreath = min( Soldier.bBreath, Soldier.bBreathMax );

					INT8 lifereduction = (6 * Disease[0].sLifeRegenHundreds) * (magnitude / 100.0f);

					Soldier.stats.bLife = max( OKLIFE, min(100, Soldier.stats.bLife + lifereduction) );
				}
			}
		}

		// If a civvy, set neutral
		if ( Soldier.bTeam == CIV_TEAM )
		{
			if (Soldier.ubProfile == WARDEN )
			{
				Soldier.aiData.bNeutral = FALSE;
			}
			else if (Soldier.ubCivilianGroup != NON_CIV_GROUP)
			{
				if ( gTacticalStatus.fCivGroupHostile[ Soldier.ubCivilianGroup ] == CIV_GROUP_HOSTILE )
				{
					// HEADROCK HAM 3.6: Flag to prevent non-combat civilians from becoming hostile and forcing
					// you to kill them...
					if (!gGameExternalOptions.fCanTrueCiviliansBecomeHostile && 
						(Soldier.ubBodyType >= FATCIV && Soldier.ubBodyType <= CRIPPLECIV ))
					{
						Soldier.aiData.bNeutral = TRUE;
					}
					else
					{
						Soldier.aiData.bNeutral = FALSE;
					}
				}
				else
				{
					Soldier.aiData.bNeutral = TRUE;
				}
			}
			else
			{
				Soldier.aiData.bNeutral = TRUE;
			}

			//Weaken stats based on the bodytype of the civilian.
			if( Soldier.ubProfile == NO_PROFILE )
			{
				switch( Soldier.ubBodyType )
				{
					case REGMALE:
					case BIGMALE:
					case STOCKYMALE:
					case REGFEMALE:
						//no adjustments necessary for these "healthy" bodytypes.
						break;
					case FATCIV:
						//fat, so slower
						Soldier.stats.bAgility = (INT8)( 30 + Random( 26 ) ); //30 - 55
						break;
					case MANCIV:
						Soldier.stats.bLife = Soldier.stats.bLifeMax = (INT8)( 35 + Random( 26 ) ); //35 - 60
						break;
					case MINICIV:
					case DRESSCIV:
						Soldier.stats.bLife = Soldier.stats.bLifeMax = (INT8)( 30 + Random( 16 ) ); //30 - 45
						break;
					case HATKIDCIV:
					case KIDCIV:
						Soldier.stats.bLife = Soldier.stats.bLifeMax = (INT8)( 20 + Random( 16 ) ); //20 - 35
						break;
					case CRIPPLECIV:
						Soldier.stats.bLife = Soldier.stats.bLifeMax = (INT8)( 20 + Random( 26 ) ); //20 - 45
						Soldier.stats.bAgility = (INT8)( 30 + Random( 16 ) ); // 30 - 45
						break;
				}
			}
		}
		else if ( Soldier.bTeam == CREATURE_TEAM )
		{
			// bloodcats are neutral to start out
			if ( Soldier.ubBodyType == BLOODCAT )
			{
				Soldier.aiData.bNeutral = TRUE;
			} // otherwise (creatures) false
		}

		// WANNE: Extnernalized!
		// ATE: TEMP : No enemy women mercs (unless elite)!
		if (gGameExternalOptions.fRestrictFemaleEnemiesExceptElite == TRUE)
		{
			if( Soldier.ubProfile == NO_PROFILE && Soldier.bTeam == ENEMY_TEAM &&
					Soldier.ubBodyType == REGFEMALE && Soldier.ubSoldierClass != SOLDIER_CLASS_ELITE )
			{
				Soldier.ubBodyType = (UINT8)( REGMALE + Random( 3 ) );
			}
		}

		// ATE
		// Set some values for variation in anims...
		if ( Soldier.ubBodyType == BIGMALE )
		{
			Soldier.uiAnimSubFlags |= SUB_ANIM_BIGGUYTHREATENSTANCE;
		}

		//For inventory, look for any face class items that may be located in the big pockets and if found, move
		//that item to a face slot and clear the pocket!
		if( Soldier.bTeam != OUR_TEAM )
		{
			INT32 i;
			BOOLEAN fSecondFaceItem = FALSE;
			// CHRISL:
			for( i = BIGPOCKSTART; i < BIGPOCKFINAL; i++ )
			{
				if( Item[ Soldier.inv[ i ].usItem ].usItemClass & IC_FACE )
				{
					if( !fSecondFaceItem )
					{ //Don't check for compatibility...	automatically assume there are no head positions filled.
						fSecondFaceItem = TRUE;
						Soldier.inv[ i ].MoveThisObjectTo(Soldier.inv[ HEAD1POS ], ALL_OBJECTS, &Soldier, HEAD1POS);
					}
					else
					{ //if there is a second item, compare it to the first one we already added.
						if( CompatibleFaceItem( Soldier.inv[ HEAD1POS ].usItem, Soldier.inv[ i ].usItem ) )
						{
							Soldier.inv[ i ].MoveThisObjectTo(Soldier.inv[ HEAD2POS ], ALL_OBJECTS, &Soldier, HEAD2POS);
							break;
						}
					}
				}
			}

			if( guiCurrentScreen != AUTORESOLVE_SCREEN )
			{
				// also, if an army guy has camouflage, roll to determine whether they start camouflaged
				if ( ( Soldier.bTeam == ENEMY_TEAM ) || ( Soldier.bTeam == MILITIA_TEAM ) )
				{
					i = FindCamoKit ( &Soldier );

					if ( i != NO_SLOT && Random( 5 ) < SoldierDifficultyLevel( &Soldier ))
					{
						// start camouflaged
						Soldier.bCamo = (INT8)__min( gGameExternalOptions.bCamoKitArea, Item[Soldier.inv[ i ].usItem].camobonus );
						Soldier.urbanCamo = (INT8)__min( gGameExternalOptions.bCamoKitArea, Item[Soldier.inv[ i ].usItem].urbanCamobonus );
						Soldier.desertCamo = (INT8)__min( gGameExternalOptions.bCamoKitArea, Item[Soldier.inv[ i ].usItem].desertCamobonus );
						Soldier.snowCamo = (INT8)__min( gGameExternalOptions.bCamoKitArea, Item[Soldier.inv[ i ].usItem].snowCamobonus );

						// silversurfer: With the new balance setting for kit camo we would probably not see camouflaged soldiers anymore
						// (the default for gGameExternalOptions.bCamoKitArea is 5 which is less than the required 50 to be shown as camouflaged).
						// So let's give him some bonus to worn camo if kit camo is too low.
						if ( Soldier.bCamo > 0 )
							Soldier.wornCamo = __max(0, ( 50 - Soldier.bCamo ) );
						if ( Soldier.urbanCamo > 0 )
							Soldier.wornUrbanCamo = __max(0, ( 50 - Soldier.urbanCamo ) );
						if ( Soldier.desertCamo > 0 )
							Soldier.wornDesertCamo = __max(0, ( 50 - Soldier.desertCamo ) );
						if ( Soldier.snowCamo > 0 )
							Soldier.wornSnowCamo = __max(0, ( 50 - Soldier.snowCamo ) );
					}
				}
			}
		}


		// OK, If not given a profile num, set a randomized default battle sound set
		// and then adjust it according to body type!
		if ( Soldier.ubProfile == NO_PROFILE )
		{
			// default (men) badguy battlesound sets 0-5
			Soldier.ubBattleSoundID = (UINT8)Random( 6 );
		}

		//Set some flags, actions based on what body type we are
		//NOTE:	BE VERY CAREFUL WHAT YOU DO IN THIS SECTION!
		//	It is very possible to override editor settings, especially orders and attitude.
		//	In those cases, you can check for !gfEditMode (not in editor).
		switch ( Soldier.ubBodyType )
		{
			case HATKIDCIV:
			case KIDCIV:
				// kids have 2 battlesound sets
				Soldier.ubBattleSoundID = (UINT8)Random( 2 );
				break;

			case REGFEMALE:
			case MINICIV:
			case DRESSCIV:

				// women get badguy battlesound sets 6-11
				Soldier.ubBattleSoundID = 6 + (UINT8) Random( 6 );
				Soldier.aiData.bNormalSmell = NORMAL_HUMAN_SMELL_STRENGTH;
				break;

			case BLOODCAT:
				AssignCreatureInventory( &Soldier );
				Soldier.aiData.bNormalSmell = NORMAL_HUMAN_SMELL_STRENGTH;
				Soldier.flags.uiStatusFlags |= SOLDIER_ANIMAL;
				break;

			case ADULTFEMALEMONSTER:
			case AM_MONSTER:
			case YAF_MONSTER:
			case YAM_MONSTER:
			case LARVAE_MONSTER:
			case INFANT_MONSTER:
			case QUEENMONSTER:

				AssignCreatureInventory( &Soldier );
				Soldier.aiData.ubCaller = NOBODY;
				if( !gfEditMode )
				{
					Soldier.aiData.bOrders = FARPATROL;
					Soldier.aiData.bAttitude = AGGRESSIVE;
				}
				Soldier.flags.uiStatusFlags |= SOLDIER_MONSTER;
				Soldier.aiData.bMonsterSmell = NORMAL_CREATURE_SMELL_STRENGTH;
				break;

			case COW:
				Soldier.flags.uiStatusFlags |= SOLDIER_ANIMAL;
				Soldier.aiData.bNormalSmell = COW_SMELL_STRENGTH;
				break;
			case CROW:

				Soldier.flags.uiStatusFlags |= SOLDIER_ANIMAL;
				break;

			case ROBOTNOWEAPON:

				Soldier.flags.uiStatusFlags |= SOLDIER_ROBOT;
				break;

			case HUMVEE:
			case ELDORADO:
			case ICECREAMTRUCK:
			case JEEP:
			case TANK_NW:
			case TANK_NE:
			case COMBAT_JEEP:

				Soldier.flags.uiStatusFlags |= SOLDIER_VEHICLE;

				switch( Soldier.ubBodyType )
				{
					case HUMVEE:
					case ELDORADO:
					case ICECREAMTRUCK:
					case JEEP:
						if ( Soldier.ubProfile != HELICOPTER && Soldier.ubProfile != 0 && Soldier.ubProfile != NO_PROFILE && Soldier.ubProfile != TANK_CAR ) 
						{
							ubVehicleID = Soldier.ubProfile;
							Soldier.aiData.bNeutral = gNewVehicle[Soldier.ubProfile].bNewNeutral;
						}
					break;
				
				/*	case HUMVEE:

						ubVehicleID = HUMMER;
						Soldier.aiData.bNeutral = TRUE;
						break;

					case ELDORADO:

						ubVehicleID = ELDORADO_CAR;
						Soldier.aiData.bNeutral = TRUE;
						break;

					case ICECREAMTRUCK:

						ubVehicleID = ICE_CREAM_TRUCK;
						Soldier.aiData.bNeutral = TRUE;
						break;
					
					case JEEP:

						ubVehicleID = JEEP_CAR;
						break;
		
					*/	
					case TANK_NW:
					case TANK_NE:					
						ubVehicleID = TANK_CAR;
						break;

					case COMBAT_JEEP:
						ubVehicleID = COMBAT_JEEP_CAR;
						break;

				}

				if ( pCreateStruct->fUseGivenVehicle )
				{
					Soldier.bVehicleID = pCreateStruct->bUseGivenVehicleID;
				}
				else
				{
					// Add vehicle to list....
					Soldier.bVehicleID = (INT8)AddVehicleToList( Soldier.sSectorX, Soldier.sSectorY, Soldier.bSectorZ, ubVehicleID );
				}
				SetVehicleValuesIntoSoldierType( &Soldier );
				break;

			default:
				Soldier.aiData.bNormalSmell = NORMAL_HUMAN_SMELL_STRENGTH;
				break;
		}

		// Flugente: under certain conditions, we can recruit civilians
		if ( Soldier.bTeam == CIV_TEAM && Soldier.ubProfile == NO_PROFILE && Soldier.ubCivilianGroup == NON_CIV_GROUP && Soldier.ubBodyType <= DRESSCIV )
		{
			// there has to officially be an outbreak in this sector - if we don't know of a disease, we cannot treat it!
			UINT8 sector = SECTOR( Soldier.sSectorX, Soldier.sSectorY );

			SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

			// only allow us to be a volunteer if this isn't blocked
			if ( pSectorInfo && !(pSectorInfo->usSectorInfoFlag & SECTORINFO_VOLUNTEERS_RECENTLY_RECRUITED) )
			{
				if ( Chance(10) )
					Soldier.usSoldierFlagMask2 |= SOLDIER_POTENTIAL_VOLUNTEER;
			}
		}

		if( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			// Copy into merc struct
			*MercPtrs[ Soldier.ubID ] = Soldier;
			// Alrighty then, we are set to create the merc, stuff after here can fail!
			CHECKF( MercPtrs[ Soldier.ubID ]->CreateSoldierCommon( Soldier.ubBodyType, Soldier.ubID, STANDING ) != FALSE );
		}
	}
	else
	{
		//Copy the data from the existing soldier struct to the new soldier struct
		if( !CopySavedSoldierInfoToNewSoldier( &Soldier, pCreateStruct->pExistingSoldier ) )
			return( FALSE );

		//Reset the face index
		Soldier.iFaceIndex = -1;
		Soldier.iFaceIndex = InitSoldierFace( &Soldier );

		// ATE: Reset soldier's light value to -1....
		Soldier.iLight = -1;

			if ( Soldier.ubBodyType == HUMVEE || Soldier.ubBodyType == ICECREAMTRUCK )
		{
			Soldier.aiData.bNeutral = TRUE;
		}

		// Copy into merc struct
		*MercPtrs[ Soldier.ubID ] = Soldier;

		// Alrighty then, we are set to create the merc, stuff after here can fail!
		CHECKF( MercPtrs[ Soldier.ubID ]->CreateSoldierCommon( Soldier.ubBodyType, Soldier.ubID, Menptr[ Soldier.ubID ].usAnimState ) != FALSE );

		*pubID = Soldier.ubID;

		// The soldiers animation frame gets reset, set
//		Menptr[ Soldier.ubID ].usAniCode = pCreateStruct->pExistingSoldier->usAniCode;
//		Menptr[ Soldier.ubID ].usAnimState = Soldier.usAnimState;
//		Menptr[ Soldier.ubID ].usAniFrame = Soldier.usAniFrame;
	}
	
	if( guiCurrentScreen != AUTORESOLVE_SCREEN )
	{
		if( pCreateStruct->fOnRoof && FlatRoofAboveGridNo( pCreateStruct->sInsertionGridNo ) )
		{
			MercPtrs[ Soldier.ubID ]->SetSoldierHeight( 58.0 );
		}

		//if we are loading DONT add men to team, because the number is restored in gTacticalStatus
		if( !( gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			// Increment men in sector number!
			AddManToTeam( Soldier.bTeam );

			//Madd: pre-load launchers
			//DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"TacticalCreateSoldier: Load launcher");
			//INT8 bSlot = FindObjClass(MercPtrs[ Soldier.ubID ],IC_LAUNCHER);
			//if ( bSlot != NO_SLOT )
			//{
			//	INT8 lSlot = FindNonSmokeLaunchable(MercPtrs[ Soldier.ubID ], Item[MercPtrs[ Soldier.ubID ]->inv[bSlot].usItem].ubClassIndex);
			//	AttachObject(MercPtrs[ Soldier.ubID ],&MercPtrs[ Soldier.ubID ]->inv[bSlot],&MercPtrs[ Soldier.ubID ]->inv[lSlot],FALSE);
			//}
		}

		return MercPtrs[ Soldier.ubID ];
	}
	else
	{
		//We are creating a dynamically allocated soldier for autoresolve.
		SOLDIERTYPE* pSoldier = new SOLDIERTYPE( Soldier ); //(SOLDIERTYPE*)MemAlloc( SIZEOF_SOLDIERTYPE );

		if( !pSoldier )
			return NULL;

		UINT8 ubSectorID = GetAutoResolveSectorID( );
		pSoldier->ubID = NUM_PROFILES;
		pSoldier->sSectorX = (INT16)SECTORX( ubSectorID );
		pSoldier->sSectorY = (INT16)SECTORY( ubSectorID );
		pSoldier->bSectorZ = 0;
		*pubID = NUM_PROFILES;

		return pSoldier;
	}
}

BOOLEAN TacticalCopySoldierFromProfile( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct )
{
	UINT8						ubProfileIndex;
	MERCPROFILESTRUCT * pProfile;

	ubProfileIndex = pCreateStruct->ubProfile;
	pProfile = &(gMercProfiles[ubProfileIndex]);

	SET_PALETTEREP_ID ( pSoldier->HeadPal,		pProfile->HAIR );
	SET_PALETTEREP_ID ( pSoldier->VestPal,		pProfile->VEST );
	SET_PALETTEREP_ID ( pSoldier->SkinPal,		pProfile->SKIN );
	SET_PALETTEREP_ID ( pSoldier->PantsPal,	pProfile->PANTS );

	// Set profile index!
	pSoldier->ubProfile									= ubProfileIndex;
	pSoldier->ubScheduleID							= pCreateStruct->ubScheduleID;
	pSoldier->flags.bHasKeys									= pCreateStruct->fHasKeys;

	wcscpy( pSoldier->name, pProfile->zNickname );

	pSoldier->stats.bLife 										= pProfile->bLife;
	pSoldier->stats.bLifeMax									= pProfile->bLifeMax;
	pSoldier->stats.bAgility									= pProfile->bAgility;
	pSoldier->stats.bLeadership								= pProfile->bLeadership;
	pSoldier->stats.bDexterity								= pProfile->bDexterity;
	pSoldier->stats.bStrength									= pProfile->bStrength;
	pSoldier->stats.bWisdom										= pProfile->bWisdom;
	pSoldier->stats.bExpLevel									= pProfile->bExpLevel;
	pSoldier->stats.bMarksmanship							= pProfile->bMarksmanship;
	pSoldier->stats.bMedical									= pProfile->bMedical;
	pSoldier->stats.bMechanical								= pProfile->bMechanical;
	pSoldier->stats.bExplosive								= pProfile->bExplosive;
	pSoldier->stats.bScientific								= pProfile->bScientific;

	// added by SANDRO - insta-healable injury zero on soldier creation
	pSoldier->iHealableInjury = 0; 

	pSoldier->bVocalVolume							= MIDVOLUME;
	pSoldier->uiAnimSubFlags						= pProfile->uiBodyTypeSubFlags;
	pSoldier->ubBodyType								= pProfile->ubBodyType;
	pSoldier->ubCivilianGroup						= pProfile->ubCivilianGroup;
	//OK set initial duty
//	pSoldier->bAssignment=ON_DUTY;

	pSoldier->bOldAssignment = NO_ASSIGNMENT;
	for ( INT8 bCnt = 0; bCnt < 30; ++bCnt )
	{
		pSoldier->stats.ubSkillTraits[ bCnt ] = pProfile->bSkillTraits[ bCnt ];	
	}
	//pSoldier->stats.ubSkillTrait2 = pProfile->bSkillTrait2;
	//if ( gGameOptions.fNewTraitSystem )
	//	pSoldier->stats.ubSkillTrait3 = pProfile->bSkillTrait3; // added by SANDRO - 3rd skill trait

	pSoldier->aiData.bOrders								= pCreateStruct->bOrders;
	pSoldier->aiData.bAttitude							= pCreateStruct->bAttitude;
	pSoldier->ubDirection						= pCreateStruct->ubDirection;
	pSoldier->aiData.bPatrolCnt						= pCreateStruct->bPatrolCnt;
	memcpy( pSoldier->aiData.sPatrolGrid, pCreateStruct->sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );

	// SANDRO - Well, it is nonsense to have 4 traits for this, one is enough, as we can now repaint the camo type by another one
	if ( !gGameOptions.fNewTraitSystem )
	{
		if ( HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_OT ) )
		{
			// set camouflaged to 100 automatically
			// silversurfer: No, set it to the new limit for camo kits.
			pSoldier->bCamo = gGameExternalOptions.bCamoKitArea;	// 100
		}
		//if ( HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED ) )
		//{
		//	// set camouflaged to 100 automatically
		//	pSoldier->bCamo = 100;
		//}
		//if ( HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_URBAN ) )
		//{
		//	// set camouflaged to 100 automatically
		//	pSoldier->urbanCamo = 100;
		//}
		//if ( HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_DESERT ) )
		//{
		//	// set camouflaged to 100 automatically
		//	pSoldier->desertCamo = 100;
		//}
		//if ( HAS_SKILL_TRAIT( pSoldier, CAMOUFLAGED_SNOW ) )
		//{
		//	// set camouflaged to 100 automatically
		//	pSoldier->snowCamo = 100;
		//}
	}

	// Flugente: VIPs: The general is, well, a general
	if ( pSoldier->ubProfile == GENERAL )
	{
		pSoldier->usSoldierFlagMask |= (SOLDIER_ENEMY_OFFICER | SOLDIER_VIP);
	}

	// Flugente: if playing with the covert trait, the assassins come covert, so they are tougher to find	
	if ( gGameExternalOptions.fAssassinsAreDisguised && gGameOptions.fNewTraitSystem && pSoldier->IsAssassin() )
	{
		pSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_SOLDIER|SOLDIER_COVERT_NPC_SPECIAL|SOLDIER_NEW_VEST|SOLDIER_NEW_PANTS);

		SetClothes( pSoldier, Random( NUMSHIRTS ), Random( NUMPANTS ) );
	}

	return( TRUE );
}

INT32 ChooseHairColor( UINT8 usBodyType, INT32 skin )
{
	INT32 iRandom;
	INT32 hair = 0;
	iRandom = Random( 100 );
	switch( skin )
	{
		case PINKSKIN:
		case TANSKIN:
			if( iRandom < 12 )
			{
				hair = REDHEAD;
			}
			else if( iRandom < 34 )
			{
				hair = BLONDEHEAD;
			}
			else if( iRandom < 60 )
			{
				hair = BROWNHEAD;
			}
			else if( iRandom < 92 )
			{
				hair = BLACKHEAD;
			}
			else
			{
				hair = WHITEHEAD;
				if ( usBodyType == REGFEMALE ||
					 usBodyType == MINICIV ||
					 usBodyType == DRESSCIV ||
					 usBodyType == HATKIDCIV ||
					 usBodyType == KIDCIV )
				{
					hair = Random( NUMHEADS - 1 ) + 1;
				}
			}
			hair = Random( NUMHEADS );
			break;
		case DARKSKIN:
			if( iRandom < 35 )
			{
				hair = BROWNHEAD;
			}
			else if( iRandom < 84 )
			{
				hair = BLACKHEAD;
			}
			else
			{
				hair = WHITEHEAD;
				if ( usBodyType == REGFEMALE ||
					 usBodyType == MINICIV ||
					 usBodyType == DRESSCIV ||
					 usBodyType == HATKIDCIV ||
					 usBodyType == KIDCIV )
				{
					hair = Random( 2 ) + 1;
				}
			}
			break;
		case BLACKSKIN:
			if( iRandom < 84 )
			{
				hair = BLACKHEAD;
			}
			else
			{
				hair = WHITEHEAD;
				if ( usBodyType == REGFEMALE ||
					 usBodyType == MINICIV ||
					 usBodyType == DRESSCIV ||
					 usBodyType == HATKIDCIV ||
					 usBodyType == KIDCIV )
				{
					hair = BLACKHEAD;
				}
			}
			break;
		default:
			AssertMsg( 0, "Skin type not accounted for." );
			break;
	}

	if ( usBodyType == CRIPPLECIV )
	{
		if( Chance( 50 ) )
		{
			hair = WHITEHEAD;
		}
	}

	return hair;
}

// Flugente: set palettes for vest/shirt
void SetClothes( SOLDIERTYPE* pSoldier, INT8 aVest, INT8 aPants, INT8 aHair, INT8 aSkin )
{
	if ( !pSoldier )
		return;
	
	// Vest
	if ( aVest >= 0 && aVest < NUMSHIRTS )
	{
		switch ( aVest )
		{
		case WHITEVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "WHITEVEST" );	break;
		case GYELLOWSHIRT:	SET_PALETTEREP_ID( pSoldier->VestPal, "GYELLOWSHIRT" );	break;
		case YELLOWVEST:	SET_PALETTEREP_ID( pSoldier->VestPal, "YELLOWVEST" );	break;
		case GREYVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "greyVEST" );		break;
		case BROWNVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "BROWNVEST" );	break;
		case PURPLESHIRT:	SET_PALETTEREP_ID( pSoldier->VestPal, "PURPLESHIRT" );	break;
		case BLUEVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "BLUEVEST" );		break;
		case JEANVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "JEANVEST" );		break;
		case GREENVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "GREENVEST" );	break;		
		case REDVEST:		SET_PALETTEREP_ID( pSoldier->VestPal, "REDVEST" );		break;		
		case BLACKSHIRT:	SET_PALETTEREP_ID( pSoldier->VestPal, "BLACKSHIRT" );	break;
		
		}
	}

	// Pants
	if ( aPants >= 0 && aPants < NUMPANTS )
	{
		switch ( aPants )
		{
		case BLUEPANTS:		SET_PALETTEREP_ID( pSoldier->PantsPal, "BLUEPANTS" );	break;
		case BLACKPANTS:	SET_PALETTEREP_ID( pSoldier->PantsPal, "BLACKPANTS" );	break;
		case JEANPANTS:		SET_PALETTEREP_ID( pSoldier->PantsPal, "JEANPANTS" );	break;
		case TANPANTS:		SET_PALETTEREP_ID( pSoldier->PantsPal, "TANPANTS" );	break;
		case BEIGEPANTS:	SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS" );	break;
		case GREENPANTS:	SET_PALETTEREP_ID( pSoldier->PantsPal, "GREENPANTS" );	break;
		}
	}

	// Hair
	if ( aHair >= 0 && aHair < NUMHEADS )
	{
		switch ( aHair )
		{
		case BROWNHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "BROWNHEAD" );	break;
		case BLACKHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "BLACKHEAD" );	break;
		case WHITEHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "WHITEHEAD" );	break;
		case BLONDEHEAD:SET_PALETTEREP_ID( pSoldier->HeadPal, "BLONDHEAD" );	break;
		case REDHEAD:	SET_PALETTEREP_ID( pSoldier->HeadPal, "REDHEAD" );		break;
		}
	}

	// Skin
	if ( aSkin >= 0 && aSkin < NUMSKINS )
	{
		switch ( aSkin )
		{
		case PINKSKIN:	SET_PALETTEREP_ID( pSoldier->SkinPal, "PINKSKIN" );		break;
		case TANSKIN:	SET_PALETTEREP_ID( pSoldier->SkinPal, "TANSKIN" );		break;
		case DARKSKIN:	SET_PALETTEREP_ID( pSoldier->SkinPal, "DARKSKIN" );		break;
		case BLACKSKIN:	SET_PALETTEREP_ID( pSoldier->SkinPal, "BLACKSKIN" );	break;
		}
	}
}

void GeneratePaletteForSoldier( SOLDIERTYPE *pSoldier, UINT8 ubSoldierClass, UINT8 ubTeam )
{
	INT32 skin, hair;
	BOOLEAN fMercClothingScheme;
	hair = -1;

	// Flugente: if all palettes are already set, no need to rebuild them. This allows zombies to use a corpses palettes
	if( pSoldier->PantsPal[0] && pSoldier->VestPal[0] && pSoldier->SkinPal[0] && pSoldier->HeadPal[0] )
		return;

	//choose random skin tone which will limit the choice of hair colors.
	skin = (INT8)Random( NUMSKINS );

	//Choose hair color which uses the skin color to limit choices
	hair = ChooseHairColor( pSoldier->ubBodyType, skin );

	MILITIA militia;
	if ( GetMilitia( pSoldier->usIndividualMilitiaID, &militia ) )
	{
		skin = militia.skin;
		hair = militia.hair;
	}
	else if ( pSoldier->usSoldierProfile )
	{
		INT8 type = pSoldier->GetSoldierProfileType(ubTeam);

		if ( type > -1 && zSoldierProfile[type][pSoldier->usSoldierProfile].uiSkin )
			skin = zSoldierProfile[type][pSoldier->usSoldierProfile].uiSkin - 1;

		if ( type > -1 && zSoldierProfile[type][pSoldier->usSoldierProfile].uiHair )
			hair = zSoldierProfile[type][pSoldier->usSoldierProfile].uiHair - 1;
	}

	switch( skin )
	{
		case PINKSKIN:
			SET_PALETTEREP_ID( pSoldier->SkinPal,	"PINKSKIN" );
			break;
		case TANSKIN:
			SET_PALETTEREP_ID( pSoldier->SkinPal,	"TANSKIN" );
			break;
		case DARKSKIN:
			SET_PALETTEREP_ID( pSoldier->SkinPal,	"DARKSKIN" );
			break;
		case BLACKSKIN:
			SET_PALETTEREP_ID( pSoldier->SkinPal,	"BLACKSKIN" );
			break;
		default:
			AssertMsg( 0, "Skin type not accounted for." );
			break;
	}

	switch( hair )
	{
		case BROWNHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "BROWNHEAD" ); break;
		case BLACKHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "BLACKHEAD" ); break;
		case WHITEHEAD: SET_PALETTEREP_ID( pSoldier->HeadPal, "WHITEHEAD" ); break;
		case BLONDEHEAD:SET_PALETTEREP_ID( pSoldier->HeadPal, "BLONDHEAD" ); break;
		case REDHEAD:	SET_PALETTEREP_ID( pSoldier->HeadPal, "REDHEAD"	); break;
		default:	AssertMsg( 0, "Hair type not accounted for.");						break;
	}

	// OK, After skin, hair we could have a forced color scheme.. use here if so
	switch( ubSoldierClass )
	{
		// HEADROCK HAM 3.6: Now reads default colors from XML.
		case SOLDIER_CLASS_ADMINISTRATOR:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "YELLOWVEST"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "GREENPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_ENEMY_ADMIN ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_ENEMY_ADMIN ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_ELITE:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "BLACKSHIRT"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "BLACKPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_ENEMY_ELITE ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_ENEMY_ELITE ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_ARMY:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "REDVEST"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "GREENPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_ENEMY_TROOP ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_ENEMY_TROOP ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_GREEN_MILITIA:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "GREENVEST"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_MILITIA_ROOKIE ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_MILITIA_ROOKIE ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_REG_MILITIA:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "JEANVEST"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_MILITIA_REGULAR ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_MILITIA_REGULAR ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_ELITE_MILITIA:
			//SET_PALETTEREP_ID( pSoldier->VestPal, "BLUEVEST"	);
			//SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS"	);
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[ UNIFORM_MILITIA_ELITE ].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[ UNIFORM_MILITIA_ELITE ].pants );
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
		case SOLDIER_CLASS_MINER:
			SET_PALETTEREP_ID( pSoldier->VestPal, "greyVEST"	);
			SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS"	);
			pSoldier->ubSoldierClass = ubSoldierClass;
			return;
	}

	//there are 2 clothing schemes, 1 for mercs and 1 for civilians.	The
	//merc clothing scheme is much larger and general and is an exclusive superset
	//of the civilian clothing scheme which means the civilians will choose the
	//merc clothing scheme often ( actually 60% of the time ).
	if( !pSoldier->PantsPal[0] || !pSoldier->VestPal[0] )
	{
		fMercClothingScheme = TRUE;
		if( pSoldier->bTeam == CIV_TEAM && Random( 100 ) < 40 )
		{ //40% chance of using cheezy civilian colors
			fMercClothingScheme = FALSE;
		}
		if( !fMercClothingScheme ) //CHEEZY CIVILIAN COLORS
		{
			if( Random( 100 ) < 30 )
			{ //30% chance that the civilian will choose a gaudy yellow shirt with pants.
				SET_PALETTEREP_ID( pSoldier->VestPal, "GYELLOWSHIRT" );
				switch( Random( 3 ) )
				{
					case 0:	SET_PALETTEREP_ID( pSoldier->PantsPal, "TANPANTS"	); break;
					case 1: SET_PALETTEREP_ID( pSoldier->PantsPal, "BEIGEPANTS" ); break;
					case 2: SET_PALETTEREP_ID( pSoldier->PantsPal, "GREENPANTS" ); break;
				}
			}
			else
			{ //70% chance that the civilian will choose jeans with a shirt.
				SET_PALETTEREP_ID( pSoldier->PantsPal, "JEANPANTS" );
				switch( Random( 7 ) )
				{
					case 0:	SET_PALETTEREP_ID( pSoldier->VestPal, "WHITEVEST"	); break;
					case 1: SET_PALETTEREP_ID( pSoldier->VestPal, "BLACKSHIRT"	); break;
					case 2: SET_PALETTEREP_ID( pSoldier->VestPal, "PURPLESHIRT" ); break;
					case 3: SET_PALETTEREP_ID( pSoldier->VestPal, "BLUEVEST"	); break;
					case 4: SET_PALETTEREP_ID( pSoldier->VestPal, "BROWNVEST"	); break;
					case 5: SET_PALETTEREP_ID( pSoldier->VestPal, "JEANVEST"	); break;
					case 6: SET_PALETTEREP_ID( pSoldier->VestPal, "REDVEST"	 ); break;
				}
			}
			return;
		}
		//MERC COLORS
		switch( Random( 3 ) )
		{
			case 0:
				SET_PALETTEREP_ID( pSoldier->PantsPal, "GREENPANTS" );
				switch( Random( 4 ) )
				{
					case 0: SET_PALETTEREP_ID( pSoldier->VestPal, "YELLOWVEST" ); break;
					case 1: SET_PALETTEREP_ID( pSoldier->VestPal, "WHITEVEST"	); break;
					case 2: SET_PALETTEREP_ID( pSoldier->VestPal, "BROWNVEST"	); break;
					case 3: SET_PALETTEREP_ID( pSoldier->VestPal, "GREENVEST"	); break;
				}
				break;
			case 1:
				SET_PALETTEREP_ID( pSoldier->PantsPal, "TANPANTS" );
				switch( Random( 8 ) )
				{
					case 0: SET_PALETTEREP_ID( pSoldier->VestPal, "YELLOWVEST" ); break;
					case 1: SET_PALETTEREP_ID( pSoldier->VestPal, "WHITEVEST"	); break;
					case 2: SET_PALETTEREP_ID( pSoldier->VestPal, "BLACKSHIRT" ); break;
					case 3: SET_PALETTEREP_ID( pSoldier->VestPal, "BLUEVEST"	); break;
					case 4: SET_PALETTEREP_ID( pSoldier->VestPal, "BROWNVEST"	); break;
					case 5: SET_PALETTEREP_ID( pSoldier->VestPal, "GREENVEST"	); break;
					case 6: SET_PALETTEREP_ID( pSoldier->VestPal, "JEANVEST"	); break;
					case 7: SET_PALETTEREP_ID( pSoldier->VestPal, "REDVEST"	); break;
				}
				break;
			case 2:
				SET_PALETTEREP_ID( pSoldier->PantsPal, "BLUEPANTS" );
				switch( Random( 4 ) )
				{
					case 0: SET_PALETTEREP_ID( pSoldier->VestPal, "YELLOWVEST" ); break;
					case 1: SET_PALETTEREP_ID( pSoldier->VestPal, "WHITEVEST"	); break;
					case 2: SET_PALETTEREP_ID( pSoldier->VestPal, "REDVEST"	); break;
					case 3: SET_PALETTEREP_ID( pSoldier->VestPal, "BLACKSHIRT" ); break;
				}
				break;
		}
	}
}

BOOLEAN TacticalCopySoldierFromCreateStruct( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct )
{
	pSoldier->ubProfile							= NO_PROFILE;

	// Randomize attributes
	pSoldier->stats.bLife								= pCreateStruct->bLife;
	pSoldier->stats.bLifeMax							= pCreateStruct->bLifeMax;
	// added by SANDRO - insta-healable injury zero on soldier creation
	pSoldier->iHealableInjury = 0; 
	pSoldier->stats.bAgility							= pCreateStruct->bAgility;
	pSoldier->stats.bDexterity						= pCreateStruct->bDexterity;
	pSoldier->stats.bExpLevel							= pCreateStruct->bExpLevel;

	pSoldier->stats.bMarksmanship					= pCreateStruct->bMarksmanship;
	pSoldier->stats.bMedical							= pCreateStruct->bMedical;
	pSoldier->stats.bMechanical						= pCreateStruct->bMechanical;
	pSoldier->stats.bExplosive						= pCreateStruct->bExplosive;
	pSoldier->stats.bLeadership						= pCreateStruct->bLeadership;
	pSoldier->stats.bStrength							= pCreateStruct->bStrength;
	pSoldier->stats.bWisdom								= pCreateStruct->bWisdom;

	pSoldier->aiData.bAttitude							= pCreateStruct->bAttitude;
	pSoldier->aiData.bOrders								= pCreateStruct->bOrders;
	pSoldier->aiData.bMorale								= pCreateStruct->bMorale;
	pSoldier->aiData.bAIMorale							= pCreateStruct->bAIMorale;
	pSoldier->bVocalVolume					= MIDVOLUME;
	pSoldier->ubBodyType					= pCreateStruct->ubBodyType;
	pSoldier->ubCivilianGroup				= pCreateStruct->ubCivilianGroup;

	pSoldier->ubScheduleID					= pCreateStruct->ubScheduleID;
	pSoldier->flags.bHasKeys							= pCreateStruct->fHasKeys;
	pSoldier->ubSoldierClass				= pCreateStruct->ubSoldierClass;

	// Flugente: soldier profiles
	// silversurfer: Don't replace tanks!
	if ( !ARMED_VEHICLE( pCreateStruct ) && !ENEMYROBOT( pCreateStruct ) )
	{
		MILITIA militia;
		if ( GetMilitia( pSoldier->usIndividualMilitiaID, &militia ) )
		{
			pSoldier->ubBodyType = militia.bodytype;

			if ( gGameExternalOptions.fIndividualMilitia_ManageHealth )
			{
				// make sure militia has at least OKLIFE
				pSoldier->stats.bLife = max( OKLIFE, (militia.healthratio / 100.0f) * pSoldier->stats.bLifeMax );
			}
		}
		else
		{
			// We have a function for this
			INT8 type = pSoldier->GetSoldierProfileType( pCreateStruct->bTeam );

			if ( type > -1 )
			{
				INT16 availablenames[NUM_SOLDIER_PROFILES];
				UINT16 cnt = 0;

				for (UINT16 i = 1; i < num_found_soldier_profiles[type]; ++i)
				{
					// make sure the name isn't already currently in use!
					if ( !IsProfileInUse(pCreateStruct->bTeam, type, i) )
						availablenames[cnt++] = i;
				}

				if ( cnt > 0 )
				{
					pSoldier->usSoldierProfile = availablenames[Random(cnt)];
								
					if ( zSoldierProfile[type][pSoldier->usSoldierProfile].uiBodyType > 0 && zSoldierProfile[type][pSoldier->usSoldierProfile].uiBodyType < 5 )
						pSoldier->ubBodyType = zSoldierProfile[type][pSoldier->usSoldierProfile].uiBodyType - 1;
				}
			}
		}
	}

	if( pCreateStruct->fVisible )
	{
		sprintf( pSoldier->HeadPal, pCreateStruct->HeadPal );
		sprintf( pSoldier->PantsPal, pCreateStruct->PantsPal );
		sprintf( pSoldier->VestPal, pCreateStruct->VestPal );
		sprintf( pSoldier->SkinPal, pCreateStruct->SkinPal );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Added by SANDRO - the enemy/militia has a decent chance to have some traits
	if ( gGameExternalOptions.fAssignTraitsToEnemy && (SOLDIER_CLASS_ENEMY( pSoldier->ubSoldierClass ) || pSoldier->ubSoldierClass == SOLDIER_CLASS_BANDIT) )
		AssignTraitsToSoldier( pSoldier, pCreateStruct );
	if ( gGameExternalOptions.fAssignTraitsToMilitia && SOLDIER_CLASS_MILITIA( pSoldier->ubSoldierClass ) )
		AssignTraitsToSoldier( pSoldier, pCreateStruct );

	// Flugente: enemy roles
	if ( gGameExternalOptions.fEnemyRoles && gGameExternalOptions.fEnemyOfficers && SOLDIER_CLASS_ENEMY( pSoldier->ubSoldierClass ) )
	{
		if ( HAS_SKILL_TRAIT( pSoldier, SQUADLEADER_NT) )
		{
			UINT8 numenemies  = NumEnemyInSector();
			UINT8 officertype = OFFICER_NONE;
			UINT8 numofficers = HighestEnemyOfficersInSector( officertype );

			// this guy becomes an officer if there are enough soldiers around, and we aren't already at max of officers
			if ( numenemies > gGameExternalOptions.usEnemyOfficersPerTeamSize * numofficers && numofficers < gGameExternalOptions.usEnemyOfficersMax )
				pSoldier->usSoldierFlagMask |= SOLDIER_ENEMY_OFFICER;
		}
	}

	// Flugente: VIPs
	if ( gGameExternalOptions.fEnemyRoles && gGameExternalOptions.fEnemyGenerals && !gbWorldSectorZ && SectorHasVIP( gWorldSectorX, gWorldSectorY ) )
	{
		if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE )
		{
			// has the VIP already been placed?
			// if we are in the Alam barracks,don't spawn a VIP here - the General is the VIP
			if ( !NumSoldiersWithFlagInSector( ENEMY_TEAM, SOLDIER_VIP ) && !NumSoldiersWithFlagInSector( CIV_TEAM, SOLDIER_VIP ) && (gWorldSectorX != gMercProfiles[GENERAL].sSectorX || gWorldSectorY != gMercProfiles[GENERAL].sSectorY) )
			{
				// nope, so this guy will become the VIP
				pSoldier->usSoldierFlagMask |= SOLDIER_VIP;
			}
			// a VIP has bodyguards
			else if ( NumSoldiersWithFlagInSector( ENEMY_TEAM, SOLDIER_BODYGUARD ) < gGameExternalOptions.usEnemyGeneralsBodyGuardsNumber )
			{
				pSoldier->usSoldierFlagMask |= SOLDIER_BODYGUARD;
				pSoldier->aiData.bOrders = SEEKENEMY;		// required, otherwise stationary orders forbid them from moving to the VIP
			}
		}
		else if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY )
		{
			// check wether there are any elites at all
			SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )]);

			// there is a chance that all elites have been removed from this sector, for example, they could have been sent as reinforcements to an adjacent sector
			// in this case, we have to make a normal troop the general - otherwise no general will be created, thus there is no chance to remove him
			// no need to to also replace the bodyguards - if the player was successful in luring them away, give him that advantage
			if ( !pSectorInfo->ubNumElites )
			{
				// has the VIP already been placed?
				// if we are in the Alam barracks,don't spawn a VIP here - the General is the VIP
				if ( !NumSoldiersWithFlagInSector( ENEMY_TEAM, SOLDIER_VIP ) && !NumSoldiersWithFlagInSector( CIV_TEAM, SOLDIER_VIP ) && (gWorldSectorX != gMercProfiles[GENERAL].sSectorX || gWorldSectorY != gMercProfiles[GENERAL].sSectorY) )
				{
					// nope, so this guy will become the VIP
					// generals are also officers...
					pSoldier->usSoldierFlagMask |= (SOLDIER_ENEMY_OFFICER | SOLDIER_VIP);
				}
			}
		}
	}

	// Flugente: turncoats
	if ( gSkillTraitValues.fCOTurncoats && pCreateStruct->bTeam == ENEMY_TEAM && SOLDIER_CLASS_ENEMY( pSoldier->ubSoldierClass ) )
	{
		if ( NumSoldiersofClassWithFlag2InSector( pCreateStruct->bTeam, pSoldier->ubSoldierClass, SOLDIER_TURNCOAT ) < NumTurncoatsOfClassInSector( gWorldSectorX, gWorldSectorY, pSoldier->ubSoldierClass ) )
		{
			pSoldier->usSoldierFlagMask2 |= SOLDIER_TURNCOAT;
		}
	}
	
	// SANDRO - If neither of these two options are activated, use the original code
	// however, "no traits" means no traits at all, so commented out
	/*if ( !(gGameExternalOptions.fAssignTraitsToEnemy) && !(gGameExternalOptions.fAssignTraitsToMilitia) )
	{
		//KM:	March 25, 1999
		//Assign nightops traits to enemies/militia
		if( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE || pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			INT32 iChance;
			UINT8	ubProgress;

			ubProgress = HighestPlayerProgressPercentage();

			if ( ubProgress < 60 )
			{
				// ramp chance from 40 to 80% over the course of 60% progress
				// 60 * 2/3 = 40, and 40+40 = 80
				iChance = 40 + (ubProgress * 2) / 3;
			}
			else
			{
				iChance = 80;
			}

			if ( Chance( iChance ) )
			{
				if (gGameOptions.fNewTraitSystem) // SANDRO - new/old traits
				{
					pSoldier->stats.ubSkillTrait1 = NIGHT_OPS_NT;
				}
				else
				{
					pSoldier->stats.ubSkillTrait1 = NIGHTOPS_OT;
					if ( ubProgress >= 40 && Chance( 30 ) )
					{
						pSoldier->stats.ubSkillTrait2 = NIGHTOPS_OT;
					}
				}
			}
		}
		else if( pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY || pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
		{
			INT32 iChance;
			UINT8	ubProgress;

			ubProgress = HighestPlayerProgressPercentage();

			if ( ubProgress < 60 )
			{
				// ramp chance from 0 to 40% over the course of 60% progress
				// 60 * 2/3 = 40
				iChance = (ubProgress * 2) / 3;
			}
			else
			{
				iChance = 40;
			}

			if ( Chance( iChance ) )
			{
				if (gGameOptions.fNewTraitSystem) // SANDRO - new/old traits
				{
					pSoldier->stats.ubSkillTrait1 = NIGHT_OPS_NT;
				}
				else
				{
					pSoldier->stats.ubSkillTrait1 = NIGHTOPS_OT;
					if ( ubProgress >= 50 && Chance( 20 ) )
					{
						pSoldier->stats.ubSkillTrait2 = NIGHTOPS_OT;
					}
				}
			}
		}



		// Lesh: give to enemies AMBIDEXTEROUS skill and a second pistol
		if( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE || pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			if ( Chance( 20 ) )
			{
				if (gGameOptions.fNewTraitSystem) // new/old traits - SANDRO
					pSoldier->stats.ubSkillTrait1 = AMBIDEXT_OT;
				else
					pSoldier->stats.ubSkillTrait1 = AMBIDEXTROUS_NT;

				if ( Chance( 10 ) )
				{
					if (gGameOptions.fNewTraitSystem) // new/old traits - SANDRO
						pSoldier->stats.ubSkillTrait2 = AMBIDEXT_OT;
					else
						pSoldier->stats.ubSkillTrait2 = AMBIDEXTROUS_NT;
				}
				if ( Item[ pCreateStruct->Inv[ HANDPOS ].usItem ].usItemClass == IC_GUN &&
					!Item[ pCreateStruct->Inv[ HANDPOS ].usItem ].twohanded )
				{
					(pCreateStruct->Inv[SECONDHANDPOS]) = (pCreateStruct->Inv[HANDPOS]);
				}
			}
		}
		else if( pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY || pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
		{
			if ( Chance( 10 ) )
			{
				if (gGameOptions.fNewTraitSystem) // new/old traits - SANDRO
					pSoldier->stats.ubSkillTrait1 = AMBIDEXT_OT;
				else
					pSoldier->stats.ubSkillTrait1 = AMBIDEXTROUS_NT;

				if ( Chance( 5 ) )
				{
					if (gGameOptions.fNewTraitSystem) // new/old traits - SANDRO
						pSoldier->stats.ubSkillTrait2 = AMBIDEXT_OT;
					else
						pSoldier->stats.ubSkillTrait2 = AMBIDEXTROUS_NT;
				}
				if ( Item[ pCreateStruct->Inv[ HANDPOS ].usItem ].usItemClass == IC_GUN &&
					!Item[ pCreateStruct->Inv[ HANDPOS ].usItem ].twohanded )
				{
					pCreateStruct->Inv[SECONDHANDPOS] = pCreateStruct->Inv[HANDPOS];
				}
			}
		}
	}*/
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//KM:	November 10, 1997
	//Adding patrol points
	//CAUTION:	CONVERTING SIGNED TO UNSIGNED though the values should never be negative.
	pSoldier->aiData.bPatrolCnt						= pCreateStruct->bPatrolCnt;
	memcpy( pSoldier->aiData.sPatrolGrid, pCreateStruct->sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );

	//Kris:	November 10, 1997
	//Expanded the default names based on team.
	switch( pCreateStruct->bTeam )
	{
		case ENEMY_TEAM:		swprintf( pSoldier->name, TacticalStr[ ENEMY_TEAM_MERC_NAME ] );		break;
		case MILITIA_TEAM:	swprintf( pSoldier->name, TacticalStr[ MILITIA_TEAM_MERC_NAME ] );	break;
		case CIV_TEAM:
			if( pSoldier->ubSoldierClass == SOLDIER_CLASS_MINER )
			{
				swprintf( pSoldier->name, TacticalStr[ CIV_TEAM_MINER_NAME ] );
			}
			else
			{
				swprintf( pSoldier->name, TacticalStr[ CIV_TEAM_MERC_NAME ] );
			}
			break;
		case CREATURE_TEAM:
			if ( pSoldier->ubBodyType == BLOODCAT )
			{
				swprintf( pSoldier->name, gzLateLocalizedString[ 36 ] );
			}
			else if ( pSoldier->IsZombie() )
			{
				swprintf( pSoldier->name, TacticalStr[ ZOMBIE_TEAM_MERC_NAME ] );
			}
			else if ( pSoldier->ubBodyType >= ADULTFEMALEMONSTER && pSoldier->ubBodyType <= QUEENMONSTER )
			{
				swprintf( pSoldier->name, TacticalStr[CREATURE_TEAM_MERC_NAME] );	break;
			}
			else
			{
				swprintf( pSoldier->name, gpStrategicString[STR_PB_BANDIT] );	break;
			}
			break;
	}

	//Generate colors for soldier based on the body type.
	GeneratePaletteForSoldier( pSoldier, pCreateStruct->ubSoldierClass, pCreateStruct->bTeam );

	// Copy item info over
	pSoldier->inv = pCreateStruct->Inv;

	return( TRUE );
}


void InitSoldierStruct( SOLDIERTYPE *pSoldier )
{
	// Memset values
	pSoldier->initialize();

	// Set default values
	pSoldier->bVisible							= -1;
	pSoldier->iFaceIndex						= -1;

	// Set morale default
	//pSoldier->aiData.bMorale								= DEFAULT_MORALE;
	pSoldier->aiData.bMorale								= gMoraleSettings.ubDefaultMorale;

	pSoldier->ubAttackerID					= NOBODY;
	pSoldier->ubPreviousAttackerID	= NOBODY;
	pSoldier->ubNextToPreviousAttackerID	= NOBODY;

	//Set AI Delay!
	pSoldier->uiAIDelay							= 100;

	pSoldier->iLight								= -1;
	pSoldier->iFaceIndex						= -1;

	// Set update time to new speed
	pSoldier->ubDesiredHeight				= NO_DESIRED_HEIGHT;
	pSoldier->bViewRange						= NORMAL_VIEW_RANGE;
	pSoldier->bInSector							= FALSE;
	pSoldier->sGridNo								= NOWHERE;
	pSoldier->iMuzFlash							= -1;
	pSoldier->usPendingAnimation		= NO_PENDING_ANIMATION;
	pSoldier->usPendingAnimation2		= NO_PENDING_ANIMATION;
	pSoldier->ubPendingStanceChange	= NO_PENDING_STANCE;
	pSoldier->ubPendingDirection		= NO_PENDING_DIRECTION;
	pSoldier->aiData.ubPendingAction				= NO_PENDING_ACTION;
	pSoldier->bLastRenderVisibleValue	= -1;
	pSoldier->bBreath								= 99;
	pSoldier->bBreathMax						= 100;
	pSoldier->bActive									= TRUE;
	pSoldier->flags.fShowLocator						= FALSE;
	pSoldier->sLastTarget							= NOWHERE;
	pSoldier->sAbsoluteFinalDestination = NOWHERE;
	pSoldier->sZLevelOverride					= -1;
	pSoldier->ubServicePartner				= NOBODY;
	pSoldier->ubAttackingHand					= HANDPOS;
	pSoldier->usAnimState							= STANDING;
	pSoldier->aiData.bInterruptDuelPts				= NO_INTERRUPT;
	pSoldier->aiData.bMoved									= FALSE;
	pSoldier->ubRobotRemoteHolderID		= NOBODY;
	pSoldier->aiData.sNoiseGridno						= NOWHERE;
	pSoldier->ubPrevSectorID					= 255;
	pSoldier->aiData.bNextPatrolPnt					= 1;
	pSoldier->bCurrentCivQuote				= -1;
	pSoldier->bCurrentCivQuoteDelta		= 0;
	pSoldier->uiBattleSoundID					= NO_SAMPLE;
	pSoldier->aiData.ubXRayedBy							= NOBODY;
	pSoldier->uiXRayActivatedTime			= 0;
	pSoldier->bBulletsLeft						= 0;
	pSoldier->bVehicleUnderRepairID		= -1;
	pSoldier->sFacilityTypeOperated		= -1; // HEADROCK HAM 3.6: Facility Operated
	pSoldier->usChatPartnerID			= NOBODY;

	// sevenfm: initialize additional data
	pSoldier->InitializeExtraData();

	pSoldier->sDragGridNo = NOWHERE;
	pSoldier->sFocusGridNo = NOWHERE;
}


BOOLEAN InternalTacticalRemoveSoldier( UINT16 usSoldierIndex, BOOLEAN fRemoveVehicle )
{
	SOLDIERTYPE *		pSoldier;

	// Check range of index given
	if ( usSoldierIndex < 0 || usSoldierIndex > TOTAL_SOLDIERS-1 )
	{
		// Set debug message

		return( FALSE );
	}
	
	// ATE: If this guy is our global selected dude, take selection off...
	if ( gfUIFullTargetFound && gusUIFullTargetID == usSoldierIndex )
	{
		gfUIFullTargetFound = FALSE;
	}
	//This one is for a single-gridno guy.....
	if ( gfUISelectiveTargetFound && gusUISelectiveTargetID == usSoldierIndex )
	{
		gfUISelectiveTargetFound = FALSE;
	}

	pSoldier = MercPtrs[ usSoldierIndex ];

	return TacticalRemoveSoldierPointer( pSoldier, fRemoveVehicle );
}

BOOLEAN TacticalRemoveSoldierPointer( SOLDIERTYPE *pSoldier, BOOLEAN fRemoveVehicle )
{
	if( !pSoldier->bActive )
		return FALSE;

	if( pSoldier->ubScheduleID )
	{
		DeleteSchedule( pSoldier->ubScheduleID );
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE && fRemoveVehicle )
	{
		// remove this vehicle from the list
		RemoveVehicleFromList( pSoldier->bVehicleID );
	}

	// Handle crow leave....
	if ( pSoldier->ubBodyType == CROW )
	{
		HandleCrowLeave( pSoldier );
	}

	if( guiCurrentScreen != AUTORESOLVE_SCREEN )
	{
		// remove character from squad list.. if they are on one
		RemoveCharacterFromSquads( pSoldier );

		//remove the soldier from the interface panel
		RemovePlayerFromTeamSlotGivenMercID( pSoldier->ubID );

		// Check if a guy exists here
		// Does another soldier exist here?
		if ( pSoldier->bActive	)
		{
			pSoldier->RemoveSoldierFromGridNo( );

		// Delete shadow of crow....
			if ( pSoldier->pAniTile != NULL )
			{
				DeleteAniTile( pSoldier->pAniTile );
				pSoldier->pAniTile = NULL;
			}

			if ( ! (pSoldier->flags.uiStatusFlags & SOLDIER_OFF_MAP) )
			{
				// Decrement men in sector number!
				RemoveManFromTeam( pSoldier->bTeam );
			} // people specified off-map have already been removed from their team count

			pSoldier->bActive = FALSE;

			// Delete!
			pSoldier->DeleteSoldier( );
		}
	}
	else
	{
		if( gfPersistantPBI )
		{
			pSoldier->DeleteSoldier( );
		}

		MemFree( pSoldier );
	}

	return( TRUE );
}

BOOLEAN TacticalRemoveSoldier( UINT16 usSoldierIndex )
{
	return( InternalTacticalRemoveSoldier( usSoldierIndex, TRUE ) );
}


// returns a soldier difficulty modifier from 0 to 100 based on player's progress, distance from the Palace, mining income, and
// playing difficulty level.	Used for generating soldier stats, equipment, and AI skill level.
INT8 CalcDifficultyModifier( UINT8 ubSoldierClass )
{
	INT8 bDiffModifier = 0;
	UINT8 ubProgress;
	UINT8 ubProgressModifier;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcDifficultyModifier"));

	if( gfEditMode )
	{
		// return an average rating for editor purposes
		return( 50 );
	}

	// THESE 3 DIFFICULTY FACTORS MUST ALWAYS ADD UP TO 100% EXACTLY!!!
	Assert( ( DIFF_FACTOR_PLAYER_PROGRESS	+ DIFF_FACTOR_PALACE_DISTANCE	+ DIFF_FACTOR_GAME_DIFFICULTY ) == 100 );
	
	// adjust for game difficulty level
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			// very strong militia, very weak enemies/cratures/bloodcats
			if ( SOLDIER_CLASS_MILITIA( ubSoldierClass ) )
			{
				// +20
				bDiffModifier += DIFF_FACTOR_GAME_DIFFICULTY;
			}
			break;

		case DIF_LEVEL_MEDIUM:
			// equally strong militia, enemies, creatures, bloodcats (+10)
			bDiffModifier += ( DIFF_FACTOR_GAME_DIFFICULTY / 2 );
			break;

		case DIF_LEVEL_HARD:
			// equally stronger militia/enemies/creatures/bloodcats (+20)
			bDiffModifier += DIFF_FACTOR_GAME_DIFFICULTY;
			break;
		case DIF_LEVEL_INSANE:
			// equally stronger enemies/creatures/bloodcats (+40)
			if ( SOLDIER_CLASS_MILITIA( ubSoldierClass ) )
			{
				// militia only +20
				bDiffModifier += DIFF_FACTOR_GAME_DIFFICULTY;
			}
			else
			{
				bDiffModifier += (INT8)(DIFF_FACTOR_GAME_DIFFICULTY * 1.5);
			}
			break;
		default:
			// equally strong militia, enemies, creatures, bloodcats (+10)
			bDiffModifier += ( DIFF_FACTOR_GAME_DIFFICULTY / 2 );		
		break;
	}
	
	// the progress returned ranges from 0 to 100
	ubProgress = HighestPlayerProgressPercentage();

	// bump it a bit once we've accomplished something (killed some enemies or taken an important sector)
	if ( ubProgress > 0 )
	{
		// +5
		bDiffModifier += DIFF_MODIFIER_SOME_PROGRESS;
	}

	// drop it down a bit if we still don't have any mine income
	if ( PredictIncomeFromPlayerMines(TRUE) == 0 )
	{
		// -5
		bDiffModifier += DIFF_MODIFIER_NO_INCOME;
	}
	
	// adjust for progress level (0 to +50)
	ubProgressModifier = ( ubProgress * DIFF_FACTOR_PLAYER_PROGRESS ) / 100;
	bDiffModifier += ubProgressModifier;

	// adjust for map location
	bDiffModifier += GetLocationModifier( ubSoldierClass );
	
	// should be no way to go over 100, although it's possible to go below 0 when just starting on easy
	// Assert( bDiffModifier <= 100 );

	// limit the range of the combined factors to between 0 and 100
	bDiffModifier = __max(	0, bDiffModifier );
	bDiffModifier = __min( 100, bDiffModifier );

	// DON'T change this function without carefully considering the impact on GenerateRandomEquipment(),
	// CreateDetailedPlacementGivenBasicPlacementInfo(), and SoldierDifficultyLevel().
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CalcDifficultyModifier done"));

	return(bDiffModifier);
}

//When the editor modifies the soldier's relative attribute level,
//this function is called to update that information.
//Used to generate a detailed placement from a basic placement.	This assumes that the detailed placement
//doesn't exist, meaning there are no static attributes.	This is called when you wish to convert a basic
//placement into a detailed placement just before creating a soldier.
void CreateDetailedPlacementGivenBasicPlacementInfo( SOLDIERCREATE_STRUCT *pp, BASIC_SOLDIERCREATE_STRUCT *bp, INT16 sX, INT16 sY )
{
	INT8 bBaseAttribute;
	UINT8 ubSoldierClass;
	UINT8 ubDiffFactor;
	INT8 bExpLevelModifier;
	INT8 bStatsModifier;
	UINT8 ubStatsLevel;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo"));
	
	if( !pp || !bp )
		return;
	pp->fStatic								= FALSE;
	pp->ubProfile							= NO_PROFILE;
	pp->sInsertionGridNo			= bp->usStartingGridNo;
	pp->fPlayerMerc						= FALSE;
	pp->fPlayerPlan						= FALSE;
	pp->fCopyProfileItemsOver = FALSE;
	pp->bTeam									= bp->bTeam;
	pp->ubSoldierClass				= bp->ubSoldierClass;
	pp->ubCivilianGroup				= bp->ubCivilianGroup;
	pp->ubScheduleID					= 0;
	pp->sSectorX							= gWorldSectorX;
	pp->sSectorY							= gWorldSectorY;
	pp->bSectorZ							= gbWorldSectorZ;
	pp->fHasKeys							= bp->fHasKeys;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: choose body type"));
	//Choose a body type randomly if none specified.
	if( bp->ubBodyType < 0 )
	{
		switch ( bp->bTeam )
		{
			case OUR_TEAM:
			case ENEMY_TEAM:
			case MILITIA_TEAM:
				switch( Random( 4 ) )
				{
					case 0:	pp->ubBodyType = REGMALE;		break;
					case 1:	pp->ubBodyType = BIGMALE;		break;
					case 2:	pp->ubBodyType = STOCKYMALE;	break;
					case 3:	pp->ubBodyType = REGFEMALE;	break;
				}
				break;
			case CIV_TEAM:
				if( pp->ubSoldierClass == SOLDIER_CLASS_MINER )
				{
					switch( Random( 3 ) )
					{ //only strong and fit men can be miners.
						case 0:	pp->ubBodyType = REGMALE;	break;
						case 1:	pp->ubBodyType = BIGMALE;	break;
						case 2: pp->ubBodyType = MANCIV;		break;
					}
				}
				else
				{
					INT32 iRandom;
					iRandom = Random( 100 );
					if( iRandom < 8 )
					{ //8% chance FATCIV
						pp->ubBodyType = FATCIV;
					}
					else if( iRandom < 38 )
					{ //30% chance MANCIV
						pp->ubBodyType = MANCIV;
					}
					else if( iRandom < 57 )
					{ //19% chance MINICIV
						pp->ubBodyType = MINICIV;
					}
					else if( iRandom < 76 )
					{ //19% chance DRESSCIV
						pp->ubBodyType = DRESSCIV;
					}
					else if( iRandom < 88 )
					{ //12% chance HATKIDCIV
						pp->ubBodyType = HATKIDCIV;
					}
					else
					{ //12% chance KIDCIV
						pp->ubBodyType = KIDCIV;
					}
				}
				break;
		}
	}
	else
	{
		pp->ubBodyType = bp->ubBodyType;
	}

	//Pass over mandatory information specified from the basic placement
	pp->bOrders = bp->bOrders;
	pp->bAttitude = bp->bAttitude;
	pp->ubDirection = bp->ubDirection;
	
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: determine soldier's class"));
	// determine this soldier's soldier class
	if( bp->bTeam == CREATURE_TEAM )
	{
		ubSoldierClass = SOLDIER_CLASS_CREATURE;
	}
	else
	{
		ubSoldierClass = bp->ubSoldierClass;
	}

	ubDiffFactor = CalcDifficultyModifier( ubSoldierClass );
	
	// experience level is modified by game difficulty, player's progress, & proximity to Queen's Palace, etc.
	// This formula gives the following results:
	//	DIFFICULTY FACTOR			EXP. LEVEL	MODIFIER		LEVEL OF AVG REGULAR TROOP
	//			0 to 19									-2													2
	//			20 to 39									-1													3
	//			41 to 59									-0													4
	//				60 to 79									+1													5
	//				80 to 99									+2													6
	//				100											+3													7		(can happen in P3 Meduna itself on HARD only!)
	bExpLevelModifier = ( ubDiffFactor / 20 ) - 2;

	// if in the upper half of this difficulty rating (10-19, 30-39, 50-59, 70-79, and 90-99)
	if ( ( ubDiffFactor % 20 ) >= 10 )
	{
		// increase stats only by one level's worth
		bStatsModifier = +1;
	}
	else
	{
		// normal stats for this level
		bStatsModifier = 0;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: adjust stats and level"));
	// Adjust level and statistics for Linda's prespecified relative attribute level

	// silversurfer: new variable level range which modifies the narrow old ranges below
	INT8 sRandomizedLevelModifier = Random( zDiffSetting[gGameOptions.ubDifficultyLevel].usLevelModifierLowLimit + zDiffSetting[gGameOptions.ubDifficultyLevel].usLevelModifierHighLimit + 1 );
	sRandomizedLevelModifier = sRandomizedLevelModifier - zDiffSetting[gGameOptions.ubDifficultyLevel].usLevelModifierLowLimit;

	switch ( bp->bRelativeAttributeLevel )
	{
		// NOTE: bStatsModifier already includes the bExpLevelModifier since it's based on the level itself!
		case 0:	bExpLevelModifier += -1 + sRandomizedLevelModifier; bStatsModifier += -1; break;
		case 1:	bExpLevelModifier += -1 + sRandomizedLevelModifier; bStatsModifier +=  0; break;
		case 2:	bExpLevelModifier +=  0 + sRandomizedLevelModifier; bStatsModifier +=  0; break;
		case 3:	bExpLevelModifier += +1 + sRandomizedLevelModifier; bStatsModifier +=  0; break;
		case 4:	bExpLevelModifier += +1 + sRandomizedLevelModifier; bStatsModifier += +1; break;

		default:
			AssertMsg( FALSE, String( "Invalid bRelativeAttributeLevel = %d", bp->bRelativeAttributeLevel ) );
			break;
	}

	UINT8 DiffLevel;
	if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		DiffLevel = 1;
	else if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
		DiffLevel = 2;
	else if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
		DiffLevel = 3;
	else if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
		DiffLevel = 4;
	else
	{
		DiffLevel = 1 + Random ( 4 );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: set level based on soldier class"));
	//Set the experience level based on the soldier class and exp level modifier or relative attribute level
	switch( ubSoldierClass )
	{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_ZOMBIE:
		case SOLDIER_CLASS_BANDIT:
			pp->bExpLevel = (INT8) 2 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_ARMY:
			pp->bExpLevel = (INT8) 4 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_ELITE:
			pp->bExpLevel = (INT8) 6 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_GREEN_MILITIA:
			pp->bExpLevel = (INT8) 2 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_REG_MILITIA:
			pp->bExpLevel = (INT8) 4 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_ELITE_MILITIA:
			pp->bExpLevel = (INT8) 6 + bExpLevelModifier;
			break;
		case SOLDIER_CLASS_MINER:
			pp->bExpLevel = bp->bRelativeAttributeLevel;					//avg 2 (1-4)
			break;

		case SOLDIER_CLASS_CREATURE:
			switch( bp->ubBodyType )
			{
				case LARVAE_MONSTER:
					pp->bExpLevel = 1 + bExpLevelModifier;
					break;
				case INFANT_MONSTER:
					pp->bExpLevel = 2 + bExpLevelModifier;
					break;
				case YAF_MONSTER:
				case YAM_MONSTER:
					pp->bExpLevel = (INT8)(3 + Random( 2 ) + bExpLevelModifier ); //3-4
					break;
				case ADULTFEMALEMONSTER:
				case AM_MONSTER:
					pp->bExpLevel = (INT8)(5 + Random( 2 ) + bExpLevelModifier ); //5-6
					break;
				case QUEENMONSTER:
					pp->bExpLevel = 7 + bExpLevelModifier;
					break;

				case BLOODCAT:
					pp->bExpLevel = 5 + bExpLevelModifier;
					// HEADROCK HAM 3.6: There can be several lairs now. Find out if this one is.
					UINT8 PlacementType = gBloodcatPlacements[ SECTOR(gWorldSectorX, gWorldSectorY) ][0].PlacementType;
					if( PlacementType == BLOODCAT_PLACEMENT_LAIR )
					{
						pp->bExpLevel += DiffLevel;
					}
					break;
			}
			break;

		case SOLDIER_CLASS_ROBOT:
		case SOLDIER_CLASS_TANK:
		case SOLDIER_CLASS_JEEP:
			pp->bExpLevel = bp->bRelativeAttributeLevel;
			break;

		default:
			pp->bExpLevel = bp->bRelativeAttributeLevel;					//avg 2 (1-4)
			ubSoldierClass = SOLDIER_CLASS_NONE;
			break;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: set exp. level"));

	pp->bExpLevel = max( DiffLevel, pp->bExpLevel ); //minimum exp. level of 1 -- madd->= dif level
	if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
		pp->bExpLevel = max( 6, pp->bExpLevel ); //minimum exp. level of 6 in insane - madd

	pp->bExpLevel = min( 10, pp->bExpLevel ); //maximum exp. level of 9 // 10 - Madd

	ubStatsLevel = pp->bExpLevel + bStatsModifier;
	
	ubStatsLevel = max( DiffLevel, ubStatsLevel );	//minimum stats level of 0 -- madd->= dif level	
	if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
		ubStatsLevel = max( 6, ubStatsLevel );	//minimum stats level of 6 in insane

	ubStatsLevel = min( 10, ubStatsLevel );	//maximum stats level of 9 // 10 - Madd - this will probably apply to all dif levels though

	//Set the minimum base attribute
	bBaseAttribute = 45 + ( 4 * ubStatsLevel );

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: roll stats and skills"));
	//Roll soldier's statistics and skills
	//Stat range is currently 49-100, bell-curved around a range of 16 values dependent on the stats level
	pp->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bLife				= pp->bLifeMax;
	pp->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));

	pp->bMarksmanship	= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bWisdom				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	pp->bMorale				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));

	// CJC: now calculate the REAL experience level if in the really upper end
	// Madd
	// silversurfer: changed that to be optional. Some players might want to have the full level range for the opposition without playing "Insane".
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: reduce stats and level"));
	if ( gGameOptions.ubDifficultyLevel <= DIF_LEVEL_HARD && !zDiffSetting[gGameOptions.ubDifficultyLevel].bAllowUnrestrictedXPLevels )
		ReduceHighExpLevels( &( pp->bExpLevel ) );

	pp->fVisible			= 0;

	pp->fOnRoof				= bp->fOnRoof;

	pp->ubSoldierClass = ubSoldierClass;

	//Transfer over the patrol points.
	pp->bPatrolCnt		= bp->bPatrolCnt;
	memcpy( pp->sPatrolGrid, bp->sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );

	//If it is a detailed placement, don't do this yet, as detailed placements may have their
	//own equipment.
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo: generate random equipment"));

	if( !bp->fDetailedPlacement && 
		ubSoldierClass != SOLDIER_CLASS_NONE && 
		ubSoldierClass != SOLDIER_CLASS_CREATURE &&
		ubSoldierClass != SOLDIER_CLASS_MINER &&
		ubSoldierClass != SOLDIER_CLASS_ZOMBIE )
	{
		GenerateRandomEquipment( pp, ubSoldierClass, bp->bRelativeEquipmentLevel);

		// Flugente testing: militia get equipment in a different way
		if ( pp->bTeam == MILITIA_TEAM && sX > 0 && sX < 17 && sY > 0 && sY < 17 )
		{
			INT8 sZ = gbWorldSectorZ > -1 ? gbWorldSectorZ : 0;

			TakeMilitiaEquipmentfromSector(sX, sY, sZ, pp, ubSoldierClass);
		}
	}

	DecideToAssignSniperOrders(pp);

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenBasicPlacementInfo done"));
}


//Used exclusively by the editor when the user wishes to change a basic placement into a detailed placement.
//Because the intention is to make some of the attributes static, all of the information that can be generated
//are defaulted to -1.	When an attribute is made to be static, that value in replaced by the new static value.
//This information is NOT compatible with TacticalCreateSoldier.	Before doing so, you must first convert the
//static detailed placement to a regular detailed placement.
void CreateStaticDetailedPlacementGivenBasicPlacementInfo( SOLDIERCREATE_STRUCT *spp, BASIC_SOLDIERCREATE_STRUCT *bp )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateStaticDetailedPlacementGivenBasicPlacementInfo"));

	UINT32 i;
	if( !spp || !bp )
		return;
	spp->initialize();
	spp->fStatic								= TRUE;
	spp->ubProfile							= NO_PROFILE;
	spp->sInsertionGridNo				= bp->usStartingGridNo;
	spp->fPlayerMerc						= FALSE;
	spp->fPlayerPlan						= FALSE;
	spp->fCopyProfileItemsOver	= FALSE;
	spp->bTeam									= bp->bTeam;
	spp->ubSoldierClass					= bp->ubSoldierClass;
	spp->ubCivilianGroup				= bp->ubCivilianGroup;
	spp->ubScheduleID						= 0;
	spp->sSectorX							= gWorldSectorX;
	spp->sSectorY							= gWorldSectorY;
	spp->bSectorZ							= gbWorldSectorZ;
	spp->fHasKeys							= bp->fHasKeys;

	//Pass over mandatory information specified from the basic placement
	spp->bOrders = bp->bOrders;
	spp->bAttitude = bp->bAttitude;
	spp->ubDirection = bp->ubDirection;

	//Only creatures have mandatory body types specified.
	if( spp->bTeam == CREATURE_TEAM )
		spp->ubBodyType = bp->ubBodyType;
	else
		spp->ubBodyType = -1;

	//Set up all possible static values.
	//By setting them all to -1, that signifies that the attribute isn't static.
	//The static placement needs to be later *regenerated* in order to create a valid soldier.
	spp->bExpLevel			= -1;
	spp->bLifeMax				= -1;
	spp->bLife				= -1;
	spp->bAgility				= -1;
	spp->bDexterity			= -1;
	spp->bMarksmanship	= -1;
	spp->bMedical				= -1;
	spp->bMechanical		= -1;
	spp->bExplosive			= -1;
	spp->bLeadership		= -1;
	spp->bStrength			= -1;
	spp->bWisdom				= -1;
	spp->bMorale				= -1;

	spp->fVisible				=	0;

	spp->fOnRoof				= bp->fOnRoof;

	//Transfer over the patrol points.
	spp->bPatrolCnt			= bp->bPatrolCnt;
	memcpy( spp->sPatrolGrid, bp->sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );

	//Starts with nothing
	UINT32 invsize = spp->Inv.size();
	for( i = 0; i < invsize; ++i )
	{
		DeleteObj(&spp->Inv[ i ]);
		spp->Inv[ i ].fFlags |= OBJECT_UNDROPPABLE;
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateStaticDetailedPlacementGivenBasicPlacementInfo done"));
}

#include "Font Control.h"
//When you are ready to generate a soldier with a static detailed placement slot, this function will generate
//the proper detailed placement slot given the static detailed placement and it's accompanying basic placement.
//For the purposes of merc editing, the static detailed placement is preserved.
void CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo(
		SOLDIERCREATE_STRUCT *pp, SOLDIERCREATE_STRUCT *spp, BASIC_SOLDIERCREATE_STRUCT *bp, INT16 sX, INT16 sY )
{
	UINT32 i;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo"));

	pp->initialize();
	pp->fOnRoof = spp->fOnRoof = bp->fOnRoof;
	pp->fStatic = FALSE;
	pp->ubSoldierClass = bp->ubSoldierClass;
	//Generate the new placement
	pp->ubProfile = spp->ubProfile;
	if( pp->ubProfile != NO_PROFILE )
	{
		// Copy over team
		pp->bTeam = bp->bTeam;

		pp->ubDirection						= bp->ubDirection;
		pp->sInsertionGridNo			= bp->usStartingGridNo;

		//ATE: Copy over sector coordinates from profile to create struct
		pp->sSectorX							= gMercProfiles[ pp->ubProfile ].sSectorX;
		pp->sSectorY							= gMercProfiles[ pp->ubProfile ].sSectorY;
		pp->bSectorZ							= gMercProfiles[ pp->ubProfile ].bSectorZ;

		pp->ubScheduleID					= spp->ubScheduleID;

		pp->bOrders								= bp->bOrders;
		pp->bAttitude							= bp->bAttitude;
		pp->ubDirection						= bp->ubDirection;
		pp->ubBodyType						= bp->ubBodyType;
		pp->bPatrolCnt						= bp->bPatrolCnt;
		memcpy( pp->sPatrolGrid, bp->sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );
		pp->fHasKeys							= bp->fHasKeys;
		pp->ubCivilianGroup				= bp->ubCivilianGroup;

		return; //done
	}
	CreateDetailedPlacementGivenBasicPlacementInfo( pp, bp, sX, sY );
	pp->ubScheduleID			= spp->ubScheduleID;
	//Replace any of the new placement's attributes with static attributes.
	if( spp->bExpLevel		!= -1 )			pp->bExpLevel			=	spp->bExpLevel;
	if( spp->bLife				!= -1 )			pp->bLife					= spp->bLife;
	if( spp->bLifeMax			!= -1 )			pp->bLifeMax			= spp->bLifeMax;
	if( spp->bMarksmanship!= -1 )			pp->bMarksmanship	= spp->bMarksmanship;
	if( spp->bStrength		!= -1 )			pp->bStrength			= spp->bStrength;
	if( spp->bAgility			!= -1 )			pp->bAgility			= spp->bAgility;
	if( spp->bDexterity		!= -1 )			pp->bDexterity		= spp->bDexterity;
	if( spp->bWisdom			!= -1 )			pp->bWisdom				= spp->bWisdom;
	if( spp->bLeadership	!= -1 )			pp->bLeadership		= spp->bLeadership;
	if( spp->bExplosive		!= -1 )			pp->bExplosive		= spp->bExplosive;
	if( spp->bMedical			!= -1 )			pp->bMedical			= spp->bMedical;
	if( spp->bMechanical	!= -1 )			pp->bMechanical		= spp->bMechanical;
	if( spp->bMorale			!= -1 )			pp->bMorale				= spp->bMorale;

	pp->fVisible = spp->fVisible;
	if( spp->fVisible )
	{
		sprintf( pp->HeadPal, spp->HeadPal );
		sprintf( pp->PantsPal, spp->PantsPal );
		sprintf( pp->VestPal, spp->VestPal );
		sprintf( pp->SkinPal, spp->SkinPal );
	}

	//This isn't perfect, however, it blindly brings over the items from the static
	//detailed placement.	Due to the order of things, other items would
	UINT32 invsize = spp->Inv.size();
	for( i = 0; i < invsize; ++i )
	{
		//copy over static items and empty slots that are droppable (signifies a forced empty slot)
		if( spp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
		{
			pp->Inv[ i ] = spp->Inv[ i ];
			//WarmSteel - if someone in the map editor placed a wrong attachment on this gun, fix it.
			AttachDefaultAttachments(&pp->Inv[i]);//RemoveProhibitedAttachments(NULL, &pp->Inv[i], pp->Inv[i].usItem);//dnl ch75 261013
			//return;
		}
	}
	//if ( !gGameOptions.fGunNut )
	//{
	//	ReplaceExtendedGuns( pp, bp->ubSoldierClass );
	//}

	if( bp->ubSoldierClass != SOLDIER_CLASS_NONE && 
		bp->ubSoldierClass != SOLDIER_CLASS_CREATURE && 
		bp->ubSoldierClass != SOLDIER_CLASS_MINER && 
		bp->ubSoldierClass != SOLDIER_CLASS_ZOMBIE )
	{
		GenerateRandomEquipment( pp, bp->ubSoldierClass, bp->bRelativeEquipmentLevel);

		// Flugente testing: militia get equipment in a different way
		if ( pp->bTeam == MILITIA_TEAM && sX > 0 && sX < 17 && sY > 0 && sY < 17 )
		{
			INT8 sZ = gbWorldSectorZ > -1 ? gbWorldSectorZ : 0;

			TakeMilitiaEquipmentfromSector(sX, sY, sZ, pp, bp->ubSoldierClass);
		}

		DecideToAssignSniperOrders(pp);
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo done"));
}

//Used to update a existing soldier's attributes with the new static detailed placement info.	This is used
//by the editor upon exiting the editor into the game, to update the existing soldiers with new information.
//This gives flexibility of testing mercs.	Upon entering the editor again, this call will reset all the
//mercs to their original states.
void UpdateSoldierWithStaticDetailedInformation( SOLDIERTYPE *s, SOLDIERCREATE_STRUCT *spp )
{
	//First, check to see if the soldier has a profile.	If so, then it'll extract the information
	//and update the soldier with the profile information instead.	This has complete override
	//authority.
	if( spp->ubProfile != NO_PROFILE )
	{
		TacticalCopySoldierFromProfile( s, spp );
		UpdateStaticDetailedPlacementWithProfileInformation( spp, spp->ubProfile );
		SetSoldierAnimationSurface( s, s->usAnimState );
		s->CreateSoldierPalettes( );
		return;
	}

	switch( spp->ubSoldierClass )
	{ //If the soldier is an administrator, then
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_ARMY:
		case SOLDIER_CLASS_ELITE:
		case SOLDIER_CLASS_BANDIT:
			GeneratePaletteForSoldier( s, spp->ubSoldierClass, spp->bTeam );
			break;
	}

	if( spp->bExpLevel != -1 )
	{ //We have a static experience level, so generate all of the soldier's attributes.
		INT8 bBaseAttribute;
		s->stats.bExpLevel = spp->bExpLevel;
		//Set the minimum base attribute
		bBaseAttribute = 45 + ( 4 * s->stats.bExpLevel );

		//Roll enemy's combat statistics, taking bExpLevel into account.
		//Stat range is currently 49-100, slightly bell-curved around the bExpLevel
		s->stats.bLifeMax				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bLife				= s->stats.bLifeMax;
		s->stats.bAgility				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bDexterity			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bMarksmanship	= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bMedical				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bExplosive			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->stats.bWisdom				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		s->aiData.bMorale				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	}
	//Replace any soldier attributes with any static values in the detailed placement.
	if( spp->bLife				!= -1 )			s->stats.bLife					= spp->bLife;
	if( spp->bLifeMax			!= -1 )			s->stats.bLifeMax				= spp->bLifeMax;
	if( spp->bMarksmanship!= -1 )			s->stats.bMarksmanship	= spp->bMarksmanship;
	if( spp->bStrength		!= -1 )			s->stats.bStrength			= spp->bStrength;
	if( spp->bAgility			!= -1 )			s->stats.bAgility				= spp->bAgility;
	if( spp->bDexterity		!= -1 )			s->stats.bDexterity			= spp->bDexterity;
	if( spp->bWisdom			!= -1 )			s->stats.bWisdom				= spp->bWisdom;
	if( spp->bLeadership	!= -1 )			s->stats.bLeadership		= spp->bLeadership;
	if( spp->bExplosive		!= -1 )			s->stats.bExplosive			= spp->bExplosive;
	if( spp->bMedical			!= -1 )			s->stats.bMedical				= spp->bMedical;
	if( spp->bMechanical	!= -1 )			s->stats.bMechanical		= spp->bMechanical;
	if( spp->bMorale			!= -1 )			s->aiData.bMorale				= spp->bMorale;

	//life can't exceed the life max.
	if( s->stats.bLife > s->stats.bLifeMax )
		s->stats.bLife = s->stats.bLifeMax;

	// added by SANDRO - insta-healable injury zero on soldier creation
	s->iHealableInjury = 0; 

	s->ubScheduleID		=	spp->ubScheduleID;

	//Copy over the current inventory list.
	s->inv = spp->Inv;
}

//In the case of setting a profile ID in order to extract a soldier from the profile array, we
//also want to copy that information to the static detailed placement, for editor viewing purposes.
void UpdateStaticDetailedPlacementWithProfileInformation( SOLDIERCREATE_STRUCT *spp, UINT8 ubProfile )
{
	UINT32					cnt;
	MERCPROFILESTRUCT * pProfile;

	spp->ubProfile = ubProfile;

	pProfile = &(gMercProfiles[ ubProfile ]);

	SET_PALETTEREP_ID ( spp->HeadPal,		pProfile->HAIR );
	SET_PALETTEREP_ID ( spp->VestPal,		pProfile->VEST );
	SET_PALETTEREP_ID ( spp->SkinPal,		pProfile->SKIN );
	SET_PALETTEREP_ID ( spp->PantsPal,	pProfile->PANTS );

	wcscpy( spp->name, pProfile->zNickname );

	spp->bLife										= pProfile->bLife;
	spp->bLifeMax									= pProfile->bLifeMax;
	spp->bAgility									= pProfile->bAgility;
	spp->bLeadership							= pProfile->bLeadership;
	spp->bDexterity								= pProfile->bDexterity;
	spp->bStrength								= pProfile->bStrength;
	spp->bWisdom									= pProfile->bWisdom;
	spp->bExpLevel								= pProfile->bExpLevel;
	spp->bMarksmanship						= pProfile->bMarksmanship;
	spp->bMedical									= pProfile->bMedical;
	spp->bMechanical							= pProfile->bMechanical;
	spp->bExplosive								= pProfile->bExplosive;

	spp->ubBodyType								= pProfile->ubBodyType;

	// Copy over inv if we want to
	UINT32 invsize = pProfile->inv.size();
	for ( cnt = 0; cnt < invsize; ++cnt )
	{
		CreateItems( pProfile->inv[ cnt ], pProfile->bInvStatus[ cnt ], pProfile->bInvNumber[ cnt ], &(spp->Inv[ cnt ]) );
	}
}

//When the editor modifies the soldier's relative attribute level,
//this function is called to update that information.
void ModifySoldierAttributesWithNewRelativeLevel( SOLDIERTYPE *s, INT8 bRelativeAttributeLevel )
{
	INT8 bBaseAttribute;
	//Set the experience level based on the relative attribute level
	// NOTE OF WARNING: THIS CURRENTLY IGNORES THE ENEMY CLASS (ADMIN/REG/ELITE) FOR CALCULATING LEVEL & ATTRIBUTES

	// Rel level 0: Lvl 1-2, 1: Lvl 2-4, 2: Lvl 4-6, 3: Lvl 6-8, 4: Lvl 8-10
	s->stats.bExpLevel = (INT8)(2 * bRelativeAttributeLevel + Random(3));

	s->stats.bExpLevel = max( 1, s->stats.bExpLevel ); //minimum level of 1
	s->stats.bExpLevel = min( 10, s->stats.bExpLevel ); //maximum level of 9 // 10

	//Set the minimum base attribute
	bBaseAttribute = 45 + ( 4 * s->stats.bExpLevel );

	//Roll enemy's combat statistics, taking bExpLevel into account.
	//Stat range is currently 49-100, slightly bell-curved around the bExpLevel
	s->stats.bLifeMax				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bLife				= s->stats.bLifeMax;
	// added by SANDRO - insta-healable injury zero on soldier creation
	s->iHealableInjury = 0;
	s->stats.bAgility				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bDexterity			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bMarksmanship	= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bMedical				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bExplosive			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->stats.bWisdom				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
	s->aiData.bMorale				= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
}


void ForceSoldierProfileID( SOLDIERTYPE *pSoldier, UINT8 ubProfileID )
{
	SOLDIERCREATE_STRUCT CreateStruct;
	CreateStruct.ubProfile = ubProfileID;

	TacticalCopySoldierFromProfile( pSoldier, &CreateStruct );

	// Delete face and re-create
	DeleteSoldierFace( pSoldier );

	// Init face
	pSoldier->iFaceIndex = InitSoldierFace( pSoldier );

	// Update animation, palettes
	SetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

	// Re-Create palettes
	pSoldier->CreateSoldierPalettes( );
}

//dnl ch56 141009
//#define CENTRAL_GRIDNO 13202
//#define CENTRAL_RADIUS 30

SOLDIERTYPE* ReserveTacticalSoldierForAutoresolve( UINT8 ubSoldierClass )
{
	INT32 i, iStart, iEnd;
	SOLDIERTYPE *pSoldier;
	//This code looks for a soldier of specified type that currently exists in tactical and
	//returns the pointer to that soldier.	This is used when copying the exact status of
	//all remaining enemy troops (or creatures) to finish the battle in autoresolve.	To
	//signify that the troop has already been reserved, we simply set their gridno to NOWHERE.
	if( ubSoldierClass != SOLDIER_CLASS_CREATURE && ubSoldierClass != SOLDIER_CLASS_ZOMBIE && ubSoldierClass != SOLDIER_CLASS_BANDIT )
	{
		//use the enemy team
		iStart = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
		iEnd = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID;
	}
	else
	{ //use the creature team
		iStart = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID;
		iEnd = gTacticalStatus.Team[ CREATURE_TEAM ].bLastID;
	}
	for( i = iStart; i <= iEnd; ++i )
	{		
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->bInSector && MercPtrs[ i ]->stats.bLife && !TileIsOutOfBounds(MercPtrs[ i ]->sGridNo))
		{
			if( MercPtrs[ i ]->ubSoldierClass == ubSoldierClass )
			{
				//reserve this soldier
				MercPtrs[ i ]->sGridNo = NOWHERE;

				//Allocate and copy the soldier
				pSoldier = new SOLDIERTYPE(*MercPtrs[i]); //(SOLDIERTYPE*)MemAlloc( SIZEOF_SOLDIERTYPE );
				if( !pSoldier )
					return NULL;

				//Assign a bogus ID, then return it
				pSoldier->ubID = NUM_PROFILES;
				return pSoldier;
			}
		}
	}
	return NULL;
}

//USED BY STRATEGIC AI and AUTORESOLVE
SOLDIERTYPE* TacticalCreateAdministrator()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_ADMINISTRATOR );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ADMINISTRATOR );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ADMINISTRATOR );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders = SEEKENEMY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	bp.ubBodyType = -1;
	bp.ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );
	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
	}
	return( pSoldier );
}

//USED BY STRATEGIC AI and AUTORESOLVE
SOLDIERTYPE* TacticalCreateArmyTroop()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_ARMY );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ARMY );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ARMY );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	bp.ubBodyType = -1;
	bp.ubSoldierClass = SOLDIER_CLASS_ARMY;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );
	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
	}

	return( pSoldier );
}

//USED BY STRATEGIC AI and AUTORESOLVE
SOLDIERTYPE* TacticalCreateEliteEnemy()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_ELITE );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ELITE );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ELITE );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	bp.ubBodyType = -1;
	bp.ubSoldierClass = SOLDIER_CLASS_ELITE;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	//SPECIAL!	Certain events in the game can cause profiled NPCs to become enemies.	The two cases are
	//adding Mike and Iggy.	We will only add one NPC in any given combat and the conditions for setting
	//the associated facts are done elsewhere.	There is also another place where NPCs can get added, which
	//is in AddPlacementToWorld() used for inserting defensive enemies.
	//NOTE:	We don't want to add Mike or Iggy if this is being called from autoresolve!
	OkayToUpgradeEliteToSpecialProfiledEnemy( &pp );

	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
	}

	return( pSoldier );
}

//USED BY STRATEGIC AI and AUTORESOLVE
SOLDIERTYPE* TacticalCreateEnemyTank()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_TANK );

		if( pSoldier )
			return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ELITE );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ELITE );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	bp.ubBodyType = TANK_NW;
	bp.ubSoldierClass = SOLDIER_CLASS_TANK;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );
	
	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;

		// Flugente: why would a vehicle's armour depend on game progress? Always give them 100 HP
		pSoldier->stats.bLifeMax = 100;
		pSoldier->stats.bLife = pSoldier->stats.bLifeMax;
	}

	return( pSoldier );
}

SOLDIERTYPE* TacticalCreateEnemyJeep( )
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if ( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_JEEP );

		if ( pSoldier )
			return pSoldier;
	}

	memset( &bp, 0, sizeof(BASIC_SOLDIERCREATE_STRUCT) );
	RandomizeRelativeLevel( &(bp.bRelativeAttributeLevel), SOLDIER_CLASS_ELITE );
	RandomizeRelativeLevel( &(bp.bRelativeEquipmentLevel), SOLDIER_CLASS_ELITE );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders = SEEKENEMY;
	bp.bAttitude = (INT8)Random( MAXATTITUDES );
	bp.ubBodyType = COMBAT_JEEP;
	bp.ubSoldierClass = SOLDIER_CLASS_JEEP;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;

		// Flugente: why would a vehicle's armour depend on game progress? Always give them 100 HP
		pSoldier->stats.bLifeMax = 100;
		pSoldier->stats.bLife = pSoldier->stats.bLifeMax;
	}

	return(pSoldier);
}

// rftr: enemy robots
SOLDIERTYPE* TacticalCreateEnemyRobot()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if ( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_ROBOT );

		if ( pSoldier )
			return pSoldier;
	}

	memset( &bp, 0, sizeof(BASIC_SOLDIERCREATE_STRUCT) );
	RandomizeRelativeLevel( &(bp.bRelativeAttributeLevel), SOLDIER_CLASS_ELITE );
	RandomizeRelativeLevel( &(bp.bRelativeEquipmentLevel), SOLDIER_CLASS_ELITE );
	bp.bTeam = ENEMY_TEAM;
	bp.bOrders = SEEKENEMY;
	bp.bAttitude = AGGRESSIVE;
	bp.ubBodyType = ROBOTNOWEAPON;
	bp.ubSoldierClass = SOLDIER_CLASS_ROBOT;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;

		pSoldier->stats.bLifeMax = 80;
		pSoldier->stats.bLife = pSoldier->stats.bLifeMax;
	}

	return(pSoldier);
}


//USED BY STRATEGIC AI and AUTORESOLVE
SOLDIERTYPE* TacticalCreateZombie()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_ZOMBIE );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ZOMBIE );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ZOMBIE );
	bp.bTeam = CREATURE_TEAM;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = AGGRESSIVE;
	bp.ubBodyType = Random( ADULTFEMALEMONSTER );
	bp.ubSoldierClass = SOLDIER_CLASS_ZOMBIE;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	// need to properly set this here
	pp.ubSoldierClass = SOLDIER_CLASS_ZOMBIE;

	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
	}

	return( pSoldier );
}

SOLDIERTYPE* ReserveTacticalMilitiaSoldierForAutoresolve( UINT8 ubSoldierClass )
{
	INT32 i, iStart, iEnd;
	SOLDIERTYPE *pSoldier;

	// For description look original ReserveTacticalSoldierForAutoresolve()

	iStart = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;
	iEnd = gTacticalStatus.Team[ MILITIA_TEAM ].bLastID;

	for( i = iStart; i <= iEnd; i++ )
	{		
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->bInSector && MercPtrs[ i ]->stats.bLife && !TileIsOutOfBounds(MercPtrs[ i ]->sGridNo))
		{
			if( MercPtrs[ i ]->ubSoldierClass == ubSoldierClass )
			{
				//reserve this soldier
				MercPtrs[ i ]->sGridNo = NOWHERE;

				//Allocate and copy the soldier
				pSoldier = new SOLDIERTYPE(*MercPtrs[i]); //(SOLDIERTYPE*)MemAlloc( SIZEOF_SOLDIERTYPE );
				if( !pSoldier )
					return NULL;

				// the militia in autoresolve will drop their gear after combat. For this reason, there is no need for MercPtrs[i] to also drop it
				MercPtrs[i]->usSoldierFlagMask |= SOLDIER_EQUIPMENT_DROPPED;

				//Assign a bogus ID, then return it
				pSoldier->ubID = NUM_PROFILES;
				return pSoldier;
			}
		}
	}
	return NULL;
}


SOLDIERTYPE* TacticalCreateMilitia( UINT8 ubMilitiaClass, INT16 sX, INT16 sY )
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if (gpBattleGroup &&
		gpBattleGroup->ubSectorZ == gbWorldSectorZ &&
		gpBattleGroup->ubSectorX == gWorldSectorX &&
		gpBattleGroup->ubSectorY == gWorldSectorY &&
		guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalMilitiaSoldierForAutoresolve( ubMilitiaClass );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), ubMilitiaClass );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), ubMilitiaClass );
	bp.bTeam = MILITIA_TEAM;
	bp.ubSoldierClass = ubMilitiaClass;
	bp.bOrders = STATIONARY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	//bp.bAttitude = AGGRESSIVE;
	bp.ubBodyType = -1;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp, sX, sY );

	// Flugente: this might not be properly filled in autoresolve
	if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
	{
		pp.sSectorX = sX;
		pp.sSectorY = sY;
		pp.bSectorZ = 0;
	}

	pSoldier = TacticalCreateSoldier( &pp, &ubID );

	return pSoldier;
}

SOLDIERTYPE* TacticalCreateCreature( INT8 bCreatureBodyType )
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_CREATURE );
		if( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_CREATURE );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_CREATURE );
	bp.bTeam = CREATURE_TEAM;
	bp.ubSoldierClass = SOLDIER_CLASS_CREATURE;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = AGGRESSIVE;
	bp.ubBodyType = bCreatureBodyType;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );
	return TacticalCreateSoldier( &pp, &ubID );
}

// Flugente: create an armed civilian
SOLDIERTYPE* TacticalCreateArmedCivilian( UINT8 usSoldierClass )
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier = NULL;

	// this needs the covert ops trait, and thus the new trait system
	if ( !gGameOptions.fNewTraitSystem )
		return NULL;

	// not in autoresolve!
	if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
		return NULL;
	
	memset( &bp, 0, sizeof(BASIC_SOLDIERCREATE_STRUCT) );
	RandomizeRelativeLevel( &(bp.bRelativeAttributeLevel), usSoldierClass );
	RandomizeRelativeLevel( &(bp.bRelativeEquipmentLevel), usSoldierClass );
	bp.bTeam = CIV_TEAM;
	bp.bOrders = FARPATROL;
	bp.bAttitude = (INT8)Random( MAXATTITUDES );
	bp.ubBodyType = Random( 4 );
	bp.ubSoldierClass = usSoldierClass;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	pSoldier = TacticalCreateSoldier( &pp, &ubID );

	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;

		if ( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			// random clothes
			SetClothes( pSoldier, Random( NUMSHIRTS ), Random( NUMPANTS ) );

			pSoldier->CreateSoldierPalettes();
		}
	}

	return(pSoldier);
}

SOLDIERTYPE* TacticalCreateCivilian( INT32 sGridNo, UINT8 usCivilianGroup, INT8 usTraderID,
									 INT8 sBodyType, INT8 aVest, INT8 aPants, INT8 aHair, INT8 aSkin, 
									 INT16 sItem1, INT16 sItem2, INT16 sItem3, INT16 sItem4 )
{
	// not in autoresolve!
	if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
		return NULL;

	UINT8					ubID = NOBODY;
	SOLDIERTYPE*			pSoldier = NULL;
	SOLDIERCREATE_STRUCT	MercCreateStruct;

	MercCreateStruct.initialize( );
	MercCreateStruct.bTeam = CIV_TEAM;
	MercCreateStruct.ubProfile = NO_PROFILE;
	MercCreateStruct.sSectorX = gWorldSectorX;
	MercCreateStruct.sSectorY = gWorldSectorY;
	MercCreateStruct.bSectorZ = gbWorldSectorZ;
	MercCreateStruct.sInsertionGridNo = sGridNo;
	MercCreateStruct.ubDirection = Random( NUM_WORLD_DIRECTIONS );

	while ( sBodyType < 0 || sBodyType > CRIPPLECIV || (sBodyType >= ADULTFEMALEMONSTER && sBodyType <= QUEENMONSTER) )
	{
		sBodyType = Random( CRIPPLECIV + 1 );
	}

	MercCreateStruct.ubBodyType = sBodyType;

	RandomizeNewSoldierStats( &MercCreateStruct );

	if ( sItem1 >= NOTHING )
	{
		CreateItems( (UINT16)(sItem1), (INT8)(80 + Random( 21 )), 1, &gTempObject );
		PlaceObjectInSoldierCreateStruct( &MercCreateStruct, &gTempObject );
	}

	if ( sItem2 >= NOTHING )
	{
		CreateItems( (UINT16)(sItem2), (INT8)(80 + Random( 21 )), 1, &gTempObject );
		PlaceObjectInSoldierCreateStruct( &MercCreateStruct, &gTempObject );
	}

	if ( sItem3 >= NOTHING )
	{
		CreateItems( (UINT16)(sItem3), (INT8)(80 + Random( 21 )), 1, &gTempObject );
		PlaceObjectInSoldierCreateStruct( &MercCreateStruct, &gTempObject );
	}

	if ( sItem4 >= NOTHING )
	{
		CreateItems( (UINT16)(sItem4), (INT8)(80 + Random( 21 )), 1, &gTempObject );
		PlaceObjectInSoldierCreateStruct( &MercCreateStruct, &gTempObject );
	}

	pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );	

	if ( pSoldier )
	{
		AddSoldierToSector( pSoldier->ubID );

		// set correct civ group
		pSoldier->ubCivilianGroup = usCivilianGroup;

		pSoldier->sNonNPCTraderID = -1;

		if ( gGameExternalOptions.bExtraMerchants )
		{
			pSoldier->sNonNPCTraderID = usTraderID;

			// if we're a dealer, don't walk around that much
			if ( pSoldier->sNonNPCTraderID > 0 )
				pSoldier->aiData.bOrders = ONGUARD;
		}

		if ( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			// make him wear administrator uniform
			UINT16 usPaletteAnimSurface = LoadSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

			if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
			{
				SetClothes( pSoldier, aVest, aPants, aHair, aSkin );

				// Use palette from HVOBJECT, then use substitution for pants, etc
				memcpy( pSoldier->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof( pSoldier->p8BPPPalette ) * 256 );

				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->HeadPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->VestPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->PantsPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->SkinPal );

				pSoldier->CreateSoldierPalettes();

				// Dirty
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}
	}

	return(pSoldier);
}

// Flugente: assassins are elite soldiers of the civ team that go hostile on a certain event, otherwise they just blend in
SOLDIERTYPE* TacticalCreateEnemyAssassin(UINT8 disguisetype)
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	// this needs the covert ops trait, and thus the new trait system
	if ( !gGameOptions.fNewTraitSystem )
		return NULL;

	// not in autoresolve!
	if( guiCurrentScreen == AUTORESOLVE_SCREEN )
	{
		return NULL;
	}

	UINT8 assassinclass = SOLDIER_CLASS_GREEN_MILITIA;
	if ( disguisetype == REGULAR_MILITIA )
		assassinclass = SOLDIER_CLASS_REG_MILITIA;
	else if ( disguisetype == ELITE_MILITIA )
		assassinclass = SOLDIER_CLASS_ELITE_MILITIA;
	else if ( disguisetype > ELITE_MILITIA )
		// invalid class, abort
		return NULL;

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), assassinclass );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), assassinclass );
	bp.bTeam = CIV_TEAM;
	bp.bOrders	= SEEKENEMY;
	bp.bAttitude = (INT8) Random( MAXATTITUDES );
	bp.ubBodyType = Random(4);
	bp.ubSoldierClass = SOLDIER_CLASS_ELITE;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	pSoldier = TacticalCreateSoldier( &pp, &ubID );

	if ( pSoldier )
	{
		// set correct stats
		pSoldier->stats.bLife = pSoldier->stats.bLifeMax = (INT8)( 70 + Random( 26 ) );
		pSoldier->stats.bAgility = (INT8)( 70 + Random( 16 ) );
				
		// add assassin flag
		pSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_SOLDIER|SOLDIER_ASSASSIN);

		// add spy trait lvl2
		pSoldier->stats.ubSkillTraits[0] = COVERT_NT;
		pSoldier->stats.ubSkillTraits[1] = COVERT_NT;

		// set correct civ group
		pSoldier->ubCivilianGroup = ASSASSIN_CIV_GROUP;

		// set militia name to further irritate the player
		swprintf( pSoldier->name, TacticalStr[ MILITIA_TEAM_MERC_NAME ] );

		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
	}

	return( pSoldier );
}

SOLDIERTYPE* TacticalCreateBandit()
{
	BASIC_SOLDIERCREATE_STRUCT bp;
	SOLDIERCREATE_STRUCT pp;
	UINT8 ubID;
	SOLDIERTYPE * pSoldier;

	if ( guiCurrentScreen == AUTORESOLVE_SCREEN && !gfPersistantPBI )
	{
		pSoldier = ReserveTacticalSoldierForAutoresolve( SOLDIER_CLASS_BANDIT );
		if ( pSoldier ) return pSoldier;
	}

	memset( &bp, 0, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	RandomizeRelativeLevel( &( bp.bRelativeAttributeLevel ), SOLDIER_CLASS_ADMINISTRATOR );
	RandomizeRelativeLevel( &( bp.bRelativeEquipmentLevel ), SOLDIER_CLASS_ADMINISTRATOR );
	bp.bTeam = ENEMY_TEAM;		// set wrong team here, otherwise no bodytype/equipment will be set
	bp.bOrders = SEEKENEMY;
	bp.bAttitude = (INT8)Random( MAXATTITUDES );
	bp.ubBodyType = -1;
	bp.ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
	CreateDetailedPlacementGivenBasicPlacementInfo( &pp, &bp );

	// need to properly set this here
	pp.bTeam = CREATURE_TEAM;
	pp.ubSoldierClass = SOLDIER_CLASS_BANDIT;

	pSoldier = TacticalCreateSoldier( &pp, &ubID );
	if ( pSoldier )
	{
		// send soldier to centre of map, roughly
		pSoldier->aiData.sNoiseGridno = ( CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS );
		pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;

		if ( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			UINT16 usPaletteAnimSurface = LoadSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

			if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
			{
				// random clothes
				SetClothes( pSoldier, Random( NUMSHIRTS ), Random( NUMPANTS ) );

				// Use palette from HVOBJECT, then use substitution for pants, etc
				memcpy( pSoldier->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof( pSoldier->p8BPPPalette ) * 256 );

				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->HeadPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->VestPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->PantsPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->SkinPal );

				pSoldier->CreateSoldierPalettes();
			}
		}
	}

	return( pSoldier );
}

void CreateAssassin(UINT8 disguisetype)
{
	SOLDIERTYPE* pSoldier = TacticalCreateEnemyAssassin( disguisetype );

	if ( pSoldier )
	{
		// find a valid starting gridno
		if ( pSoldier->sInsertionGridNo <= 0 )
		{
			UINT8 tries = 0;		// counter for gridno function... if we fail to get a valid starting gridno multiple times, do not place assassin
			INT32 sGridNo = NOWHERE;
			do
			{
				if ( ++tries > 20 )
					return;

				sGridNo = RandomGridNo();				
			}
			// a valid starting gridno must be valid, not in a structure, not in water, and not too near to our mercs
			while( TileIsOutOfBounds(sGridNo) || FindStructure( sGridNo, STRUCTURE_BLOCKSMOVES ) || TERRAIN_IS_WATER( gpWorldLevelData[ sGridNo ].ubTerrainID) || GridNoNearPlayerMercs(sGridNo,  12) );

			pSoldier->sInsertionGridNo = sGridNo;
		}

		AddSoldierToSector( pSoldier->ubID );

		// assassins are elite soldiers disguised as militia. Create militia clothes and apply them
		BOOLEAN vestfound  = FALSE;
		BOOLEAN pantsfound = FALSE;
		UINT16 vestitem = 0;
		UINT16 pantsitem = 0;
		switch( disguisetype )
		{
		case ELITE_MILITIA:
			vestfound  =  GetFirstClothesItemWithSpecificData(&vestitem, gUniformColors[ UNIFORM_MILITIA_ELITE ].vest, "blank");
			pantsfound =  GetFirstClothesItemWithSpecificData(&pantsitem, "blank", gUniformColors[ UNIFORM_MILITIA_ELITE ].pants);
			break;
		case REGULAR_MILITIA:
			vestfound  =  GetFirstClothesItemWithSpecificData(&vestitem, gUniformColors[ UNIFORM_MILITIA_REGULAR ].vest, "blank");
			pantsfound =  GetFirstClothesItemWithSpecificData(&pantsitem, "blank", gUniformColors[ UNIFORM_MILITIA_REGULAR ].pants);
			break;
		default:
			vestfound  =  GetFirstClothesItemWithSpecificData(&vestitem, gUniformColors[ UNIFORM_MILITIA_ROOKIE ].vest, "blank");
			pantsfound =  GetFirstClothesItemWithSpecificData(&pantsitem, "blank", gUniformColors[ UNIFORM_MILITIA_ROOKIE ].pants);
			break;
		}

		if ( vestfound )
		{
			ApplyClothes( pSoldier, vestitem, 100 );
		}

		if ( pantsfound )
		{
			ApplyClothes( pSoldier, vestitem, 100 );
		}

		// Dirty
		fInterfacePanelDirty = DIRTYLEVEL2;

		// add correct flags. Undo SOLDIER_COVERT_CIV flag, we disguise as militia, not as a civilian (player cannot disguise as militia, thus he becomes a civilian)
		pSoldier->usSoldierFlagMask &= ~SOLDIER_COVERT_CIV;
		pSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_SOLDIER|SOLDIER_ASSASSIN);

		// So we can see them!
		AllTeamsLookForAll(NO_INTERRUPTS);

		gTacticalStatus.fCivGroupHostile[ ASSASSIN_CIV_GROUP ] = CIV_GROUP_WILL_BECOME_HOSTILE;
	}
}

extern UINT16*	gusWorldRoomInfo;
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// For now, this is only used for prison cells
INT32 GetSittableGridNoInRoom(UINT16 usRoom, BOOLEAN fEnoughSpace)
{
	INT32 insertiongridno = NOWHERE;

	// get sector data and look fo a fitting room
	UINT8 ubSectorId = SECTOR(gWorldSectorX, gWorldSectorY);
	if ( ubSectorId >= 0 && ubSectorId < 256  )
	{
		for ( INT32 uiLoop = 0; uiLoop < WORLD_MAX; ++uiLoop )
		{
			if ( (gusWorldRoomInfo[ uiLoop ] == usRoom) )
			{
				if ( fEnoughSpace )
				{
					// we have to make sure that the gridno is deep in the room, as otherwise the gridno might be corrected to be on the other side of a wall
					if ( ( gusWorldRoomInfo[ NewGridNo( uiLoop, DirectionInc(NORTH) ) ] != usRoom ) 
						|| ( gusWorldRoomInfo[ NewGridNo( uiLoop, DirectionInc(EAST) ) ] != usRoom )
						|| ( gusWorldRoomInfo[ NewGridNo( uiLoop, DirectionInc(SOUTH) ) ] != usRoom )
						|| ( gusWorldRoomInfo[ NewGridNo( uiLoop, DirectionInc(WEST) ) ] != usRoom ) )
									continue;
				}
								
				// check wether this location is sittable
				if ( IsLocationSittable( uiLoop, 0 ) )
				{
					return uiLoop;
				}
			}
		}
	}

	return NOWHERE;
}

static UINT16 roomcnt = 0;

void CreatePrisonerOfWar()
{
	INT32 insertiongridno = NOWHERE;

	// get sector data and look fo a fitting room
	UINT8 ubSectorId = SECTOR(gWorldSectorX, gWorldSectorY);
	if ( ubSectorId >= 0 && ubSectorId < 256  )
	{
		if ( SectorExternalData[ubSectorId][0].prisonroomvector.empty( ) )
			return;

		++roomcnt;
		if ( roomcnt >= SectorExternalData[ubSectorId][0].prisonroomvector.size() )
			roomcnt = 0;

		UINT16 room = SectorExternalData[ubSectorId][0].prisonroomvector[roomcnt];

		insertiongridno = GetSittableGridNoInRoom( room, TRUE );

		// invalid gridno? Get out of here
		if ( TileIsOutOfBounds(insertiongridno) )
			return;
	}

	SOLDIERCREATE_STRUCT		MercCreateStruct;
	UINT8						ubID;

	static INT8 bPowBodyType = REGMALE;

	MercCreateStruct.initialize();
	MercCreateStruct.bTeam				= CIV_TEAM;
	MercCreateStruct.ubProfile			= NO_PROFILE;
	MercCreateStruct.sSectorX			= gWorldSectorX;
	MercCreateStruct.sSectorY			= gWorldSectorY;
	MercCreateStruct.bSectorZ			= gbWorldSectorZ;
	MercCreateStruct.sInsertionGridNo	= insertiongridno;
	MercCreateStruct.ubDirection		= Random(NUM_WORLD_DIRECTIONS);
	MercCreateStruct.ubBodyType			= bPowBodyType;

	++bPowBodyType;
	if ( bPowBodyType > REGFEMALE )
		bPowBodyType = REGMALE;

	RandomizeNewSoldierStats( &MercCreateStruct );
	
	SOLDIERTYPE* pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );

	if ( pSoldier )
	{
		AddSoldierToSector( pSoldier->ubID );

		// mark this guy
		pSoldier->usSoldierFlagMask |= SOLDIER_POW_PRISON;

		// set correct civ group
		pSoldier->ubCivilianGroup = POW_PRISON_CIV_GROUP;

		swprintf( pSoldier->name, TacticalStr[ POW_TEAM_MERC_NAME ] );

		if ( guiCurrentScreen != AUTORESOLVE_SCREEN )
		{
			// make him wear 'prisoner garb' (just what Shank and Dynamo are wearing)
			UINT16 usPaletteAnimSurface = LoadSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

			if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
			{
				SET_PALETTEREP_ID( pSoldier->VestPal, "greyVEST" );
				SET_PALETTEREP_ID( pSoldier->PantsPal, "BLACKPANTS" );

				// Use palette from HVOBJECT, then use substitution for pants, etc
				memcpy( pSoldier->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof( pSoldier->p8BPPPalette ) * 256 );

				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->HeadPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->VestPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->PantsPal );
				SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->SkinPal );

				pSoldier->CreateSoldierPalettes();

				// Dirty
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}

		// So we can see them!
		AllTeamsLookForAll(NO_INTERRUPTS);

		gTacticalStatus.fCivGroupHostile[ POW_PRISON_CIV_GROUP ] = CIV_GROUP_NEUTRAL;
	}
}

// create a downed pilot in a random location in the current sector
void CreateDownedPilot( )
{
	UINT8 tries = 0;
	INT32 sGridNo = NOWHERE;
	do
	{
		if ( ++tries > 20 )
			return;

		sGridNo = RandomGridNo( );
	}
	// a valid starting gridno must be valid, not in a structure, not in water, and not too near to our mercs
	while ( TileIsOutOfBounds( sGridNo ) || FindStructure( sGridNo, STRUCTURE_BLOCKSMOVES ) || TERRAIN_IS_WATER( gpWorldLevelData[sGridNo].ubTerrainID ) || GridNoNearPlayerMercs( sGridNo, 25 ) );

	SOLDIERCREATE_STRUCT		MercCreateStruct;
	UINT8						ubID;
	
	MercCreateStruct.initialize( );
	MercCreateStruct.bTeam = CIV_TEAM;
	MercCreateStruct.ubProfile = NO_PROFILE;
	MercCreateStruct.sSectorX = gWorldSectorX;
	MercCreateStruct.sSectorY = gWorldSectorY;
	MercCreateStruct.bSectorZ = gbWorldSectorZ;
	MercCreateStruct.sInsertionGridNo = sGridNo;
	MercCreateStruct.ubDirection = Random( NUM_WORLD_DIRECTIONS );
	MercCreateStruct.ubBodyType = Random( ADULTFEMALEMONSTER );
	
	RandomizeNewSoldierStats( &MercCreateStruct );

	SOLDIERTYPE* pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );

	if ( pSoldier )
	{
		// a downed pilot might be wounded
		pSoldier->stats.bLife = min( pSoldier->stats.bLifeMax, max( OKLIFE + 20, pSoldier->stats.bLife - 20 ) );
		pSoldier->bBleeding = pSoldier->stats.bLifeMax - pSoldier->stats.bLife;

		AddSoldierToSector( pSoldier->ubID );
		
		// set correct civ group
		pSoldier->ubCivilianGroup = DOWNEDPILOT_CIV_GROUP;
		
		// make him wear administrator uniform
		UINT16 usPaletteAnimSurface = LoadSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

		if ( usPaletteAnimSurface != INVALID_ANIMATION_SURFACE )
		{
			SET_PALETTEREP_ID( pSoldier->VestPal, gUniformColors[UNIFORM_ENEMY_ADMIN].vest );
			SET_PALETTEREP_ID( pSoldier->PantsPal, gUniformColors[UNIFORM_ENEMY_ADMIN].pants );

			// Use palette from HVOBJECT, then use substitution for pants, etc
			memcpy( pSoldier->p8BPPPalette, gAnimSurfaceDatabase[usPaletteAnimSurface].hVideoObject->pPaletteEntry, sizeof(pSoldier->p8BPPPalette) * 256 );

			SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->HeadPal );
			SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->VestPal );
			SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->PantsPal );
			SetPaletteReplacement( pSoldier->p8BPPPalette, pSoldier->SkinPal );

			pSoldier->CreateSoldierPalettes( );

			// Dirty
			fInterfacePanelDirty = DIRTYLEVEL2;
		}

		// So we can see them!
		AllTeamsLookForAll( NO_INTERRUPTS );

		// downed pilots are hostile, even though they stand no chance against us
		gTacticalStatus.fCivGroupHostile[POW_PRISON_CIV_GROUP] = CIV_GROUP_HOSTILE;

		pSoldier->aiData.bNeutral = FALSE;
	}
}

void RandomizeRelativeLevel( INT8 *pbRelLevel, UINT8 ubSoldierClass )
{
	UINT8 ubLocationModifier = 0;
	INT8 bRollModifier;
	INT8 bRoll, bAdjustedRoll;


	// We now adjust the relative level by location on the map, so enemies in NE corner will be generally very crappy (lots
	// of bad and poor, with avg about best), while enemies in the SW will have lots of great and good, with avg about as
	// lousy as it gets.	Militia are generally unmodified by distance, and never get bad or great at all.

	// this returns 0 to DIFF_FACTOR_PALACE_DISTANCE (0 to +30)
	ubLocationModifier = GetLocationModifier( ubSoldierClass );

	// convert to 0 to 10 (divide by 3), the subtract 5 to get a range of -5 to +5
	bRollModifier = ( INT8 ) ( ubLocationModifier / ( DIFF_FACTOR_PALACE_DISTANCE / 10 ) ) - 5;

	// roll a number from 0 to 9
	bRoll = ( INT8 ) Random( 10 );

	// adjust by the modifier (giving -5 to +14)
	bAdjustedRoll = bRoll + bRollModifier;

	if ( SOLDIER_CLASS_MILITIA( ubSoldierClass ) )
	{
		// Militia never get to roll bad/great results at all (to avoid great equipment drops from them if killed)
		bAdjustedRoll = __max( 1, bAdjustedRoll );
		bAdjustedRoll = __min( 8, bAdjustedRoll );
		if( IsAutoResolveActive() )
		{ //Artificially strengthen militia strength for sake of gameplay
			bAdjustedRoll++;
		}
	}
	else
	{
		// max-min this to a range of 0-9
		bAdjustedRoll = __max( 0, bAdjustedRoll );
		bAdjustedRoll = __min( 9, bAdjustedRoll );
		if( IsAutoResolveActive() )
		{ //Artificially weaken enemy/creature strength for sake of gameplay
			if( bAdjustedRoll > 0 )
			{
				bAdjustedRoll--;
			}
		}
	}

	switch ( bAdjustedRoll )
	{
		case 0:
			// bad
			*pbRelLevel = 0;
			break;
		case 1:
		case 2:
			// poor
			*pbRelLevel = 1;
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			// average
			*pbRelLevel = 2;
			break;
		case 7:
		case 8:
			// good
			*pbRelLevel = 3;
			break;
		case 9:
			// great
			*pbRelLevel = 4;
			break;

		default:
			Assert( FALSE );
			*pbRelLevel = 2;
			break;
	}

/*
	if( IsAutoResolveActive() )
	{ //Artificially strengthen militia strength for sake of gameplay
		if ( SOLDIER_CLASS_MILITIA( ubSoldierClass ) )
		{
			*pbRelLevel = 4;
		}
		else
		{
			*pbRelLevel = 0;
		}
	}
*/
}


//This function shouldn't be called outside of tactical
void QuickCreateProfileMerc( INT8 bTeam, UINT8 ubProfileID )
{
	// Create guy # X
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT16										sWorldX, sWorldY, sSectorX, sSectorY, sGridX, sGridY;
	UINT8									ubID;
	INT32 usMapPos;

	if ( GetMouseXY( &sGridX, &sGridY ) )
	{
		usMapPos = MAPROWCOLTOPOS( sGridY, sGridX );
		// Get Grid Coordinates of mouse
		if ( GetMouseWorldCoordsInCenter( &sWorldX, &sWorldY ) )
		{
			GetCurrentWorldSector( &sSectorX, &sSectorY );


			MercCreateStruct.initialize();
			MercCreateStruct.bTeam				= bTeam;
			MercCreateStruct.ubProfile		= ubProfileID;
			MercCreateStruct.sSectorX			= sSectorX;
			MercCreateStruct.sSectorY			= sSectorY;
			MercCreateStruct.bSectorZ			= gbWorldSectorZ;
			MercCreateStruct.sInsertionGridNo		= usMapPos;

			RandomizeNewSoldierStats( &MercCreateStruct );

			if ( TacticalCreateSoldier( &MercCreateStruct, &ubID ) )
			{
				AddSoldierToSector( ubID );

				// So we can see them!
				AllTeamsLookForAll(NO_INTERRUPTS);
			}
		}
	}
}

// CHRISL: External function call to resort profile inventory
extern void DistributeInitialGear(MERCPROFILESTRUCT *pProfile);

void CopyProfileItems( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct )
{
	UINT32			cnt, cnt2;
	MERCPROFILESTRUCT*		pProfile;
	BOOLEAN success;
    BOOLEAN fRet;

	pProfile = &(gMercProfiles[pCreateStruct->ubProfile]);

	// Copy over inv if we want to
	if ( pCreateStruct->fCopyProfileItemsOver || pSoldier->bTeam != OUR_TEAM )
	{
		if (pCreateStruct->fPlayerMerc)
		{
			// do some special coding to put stuff in the profile in better-looking
			// spots
			// CHRISL: Resort profile items to use LBE pockets properly
			DistributeInitialGear(pProfile);
			//place all items that are NOT attachments
			UINT32 invsize = pProfile->inv.size();
			for ( cnt = 0; cnt < pProfile->inv.size(); ++cnt )
			{
				if ( pProfile->inv[ cnt ] == NOTHING || Item[pProfile->inv[cnt]].attachment) {
					continue;
				}
				fRet = CreateItems( pProfile->inv[ cnt ], pProfile->bInvStatus[ cnt ], pProfile->bInvNumber[ cnt ], &gTempObject );
				//CHRISL: Place items by slots chosen in profile if using new inventory system
				if(fRet)
				{
					if((UsingNewInventorySystem() == false)) {
						success = AutoPlaceObject( pSoldier, &gTempObject, FALSE );
					}
					else {
						success = PlaceObject( pSoldier, cnt, &gTempObject );
					}
					if (success == FALSE && pSoldier->inv[cnt].exists() == false) {
						pSoldier->inv[cnt] = gTempObject;
					}
				}
			}

			//done placing all non attachments, now place all attachments on objects!
			for ( cnt = 0; cnt < invsize; ++cnt )
			{
				if ( pProfile->inv[ cnt ] == NOTHING) {
					continue;
				}
				if (!Item[pProfile->inv[cnt]].attachment) {
					continue;
				}
				fRet = CreateItems( pProfile->inv[ cnt ], pProfile->bInvStatus[ cnt ], pProfile->bInvNumber[ cnt ], &gTempObject );
				// try to find the appropriate item to attach to!
				if(fRet)
				{
					for ( cnt2 = INV_START_POS; cnt2 < NUM_INV_SLOTS; cnt2++ )
					{
						if ( pSoldier->inv[ cnt2 ].exists() == true && ValidAttachment( gTempObject.usItem, &(pSoldier->inv[ cnt2 ]) ) )
						{
							if(pSoldier->inv[ cnt2 ].AttachObject( NULL, &gTempObject, FALSE ))
							{
								break;
							}
						}
					}
					if (cnt2 == NUM_INV_SLOTS)
					{
						// oh well, couldn't find anything to attach to!
						//CHRISL: Place items by slots chosen in profile if using new inventory system
						if((UsingNewInventorySystem() == false)) {
							success = AutoPlaceObject( pSoldier, &gTempObject, FALSE );
						}
						else {
							// CHRISL: If we're placing a "body" item, then just place it.  Otherwise, first
							// try to autoplace the item.  Only use "PlaceObject" as a last resort.
							if(cnt < BIGPOCKSTART) {
								success = PlaceObject( pSoldier, cnt, &gTempObject );
							}
							else{
								success = AutoPlaceObject( pSoldier, &gTempObject, FALSE );
								if(success == FALSE) {
									success = PlaceObject( pSoldier, cnt, &gTempObject );
								}
							}
						}
						if (success == FALSE && pSoldier->inv[cnt].exists() == false) {
							pSoldier->inv[cnt] = gTempObject;
						}
					}
				}
			}
		}
		else
		{
            fRet=FALSE;

			for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
			{

				if ( pProfile->inv[ cnt ] != NOTHING )
				{

					if ( Item[ pProfile->inv[ cnt ] ].usItemClass == IC_KEY )
					{
						// since keys depend on 2 values, they pretty much have to be hardcoded.
						// and if a case isn't handled here it's better to not give any key than
						// to provide one which doesn't work and would confuse everything.
						
						#ifdef JA2UB						
							if ( pCreateStruct->ubProfile == MORRIS_UB )
						{
								if ( pProfile->inv[ cnt ] >= KEY_1 && pProfile->inv[ cnt ] <= KEY_32)
								{
									fRet = CreateKeyObject( &gTempObject , pProfile->bInvNumber[ cnt ], 32 );
									//CreateKeyObject( &(pSoldier->inv[ cnt ] ), pProfile->bInvNumber[ cnt ], 32 );
								}
								//else
								//{
								//	memset( &(pSoldier->inv[cnt]), 0, sizeof( OBJECTTYPE ) );
								//}
							}
						#endif
						
						switch( pCreateStruct->ubProfile )
						{
#ifdef JA2UB						

#endif

							// WANNE: Changed KEY_32 to KEY_8 because we only have 8 keys defined in Items.xml
							case BREWSTER:
								if ( pProfile->inv[ cnt ] >= KEY_1 && pProfile->inv[ cnt ] <= KEY_8){
									fRet = CreateKeyObject( &gTempObject , pProfile->bInvNumber[ cnt ], 19 );
								}
								break;
							case SKIPPER:
								if ( pProfile->inv[ cnt ] >= KEY_1 && pProfile->inv[ cnt ] <= KEY_8){
									fRet = CreateKeyObject( &gTempObject, pProfile->bInvNumber[ cnt ], 11 );
								}
								break;
							case DOREEN:
								if ( pProfile->inv[ cnt ] >= KEY_1 && pProfile->inv[ cnt ] <= KEY_8){
									fRet = CreateKeyObject( &gTempObject, pProfile->bInvNumber[ cnt ], 32 );
								}
						}
					}
					else
					{
						fRet = CreateItems( pProfile->inv[ cnt ], pProfile->bInvStatus[ cnt ], pProfile->bInvNumber[ cnt ], &gTempObject );
					}
					if(fRet)
					{
						pSoldier->inv[cnt] = gTempObject;
						if ( Item[gTempObject.usItem].fingerprintid )
						{
							for (int x = 0; x < pProfile->bInvNumber[ cnt ]; ++x) {
								gTempObject[x]->data.ubImprintID = pSoldier->ubProfile;
							}
						}
						if (gubItemDroppableFlag[cnt])
						{
							if (pProfile->ubInvUndroppable & gubItemDroppableFlag[cnt])
							{
								gTempObject.fFlags |= OBJECT_UNDROPPABLE;
							}
						}
					}
				}
			}
			if (pProfile->uiMoney > 0)
			{
				//only npcs can have money
				fRet = CreateMoney(pProfile->uiMoney, &gTempObject );
				if(fRet)
					AutoPlaceObject(pSoldier, &gTempObject, true);
			}
		}
	}
}

//SPECIAL!	Certain events in the game can cause profiled NPCs to become enemies.	The two cases are
//adding Mike and Iggy.	We will only add one NPC in any given combat and the conditions for setting
//the associated facts are done elsewhere.	The function will set the profile for the SOLDIERCREATE_STRUCT
//and the rest will be handled automatically so long the ubProfile field doesn't get changed.
//NOTE:	We don't want to add Mike or Iggy if this is being called from autoresolve!
void OkayToUpgradeEliteToSpecialProfiledEnemy( SOLDIERCREATE_STRUCT *pp )
{
	if( !gfProfiledEnemyAdded && GetEnemyEncounterCode() != ENEMY_ENCOUNTER_CODE && GetEnemyEncounterCode() != ENEMY_INVASION_CODE )
	{
		if( gubFact[ FACT_MIKE_AVAILABLE_TO_ARMY ] == 1 && !pp->fOnRoof )
		{
			gubFact[ FACT_MIKE_AVAILABLE_TO_ARMY ] = 2; //so it fails all subsequent checks
			pp->ubProfile = MIKE;
			gfProfiledEnemyAdded = TRUE;
		}
		else if( gubFact[ FACT_IGGY_AVAILABLE_TO_ARMY ] == 1 && !pp->fOnRoof )
		{
			gubFact[ FACT_IGGY_AVAILABLE_TO_ARMY ] = 2; //so it fails all subsequent checks
			pp->ubProfile = IGGY;
			gfProfiledEnemyAdded = TRUE;
		}
	}
}


void TrashAllSoldiers( )
{
	INT32		cnt;
	SOLDIERTYPE *pSoldier;

	cnt = 0;

	for ( pSoldier = MercPtrs[ cnt ]; cnt < MAX_NUM_SOLDIERS; pSoldier++, cnt++ )
	{
		if ( pSoldier->bActive )
		{
			// Delete from world
			TacticalRemoveSoldier( (UINT16)cnt );
		}
	}
}



UINT8 GetLocationModifier( UINT8 ubSoldierClass )
{
	UINT8 ubLocationModifier = 0;	
	INT16 sSectorX, sSectorY, sSectorZ;

	// Flugente: why do we always crash the game if something does not work?
	// where is all this taking place?
	if ( !GetCurrentBattleSectorXYZ( &sSectorX, &sSectorY, &sSectorZ ) )
		return 0;

#ifdef JA2UB
	//Ja25 UB
	//switch on the sector, to determine modifer
	//the modifier is based between 0 and 40.  40 being the "hardest"
	switch( SECTOR( sSectorX, sSectorY ) )
	{
		//Starting sector
		case SEC_H7:
			ubLocationModifier = 4;
			break;

		//First sector that has enemies in it
		case SEC_H8:
			ubLocationModifier = 8;
			break;

		//Guard Post
		case SEC_H9:
			ubLocationModifier = 14;
			break;

		// The 2 "empty" sectors before the town ( north and west of town )
		case SEC_H10:
		case SEC_I9:
			ubLocationModifier = 12;
			break;

		//the town of varrez
		case SEC_I10:
			ubLocationModifier = 16;
			break;
		case SEC_I11:
			ubLocationModifier = 19;
			break;


		// The 2 "empty" sectors after the town ( east and south of town )
		case SEC_I12:
		case SEC_J11:
			ubLocationModifier = 22;
			break;

		//The abandoned mine
		case SEC_I13:
			ubLocationModifier = 22;
			break;

		//"empty field" that player can take to avoid going through the mine
		case SEC_J12:
			ubLocationModifier = 22;
			break;

		//The power Generator facility
		case SEC_J13:
			{
				//the top floor
				switch( sSectorZ )
				{
					//Main floor
					case 0:
						ubLocationModifier = 26;
						break;

					//Basement level
					case 1:
						ubLocationModifier = 15;
						break;
					default:
						Assert( 0 );
						break;
				}
			}
			break;

		//tunnel levels, no enemies
		case SEC_J14:
		case SEC_K14:
			ubLocationModifier = 35;
			break;

		case SEC_K15:
		{
				//the top floor
				switch( sSectorZ )
				{
					//Main floor
					case 0:
						ubLocationModifier = 30;
						break;

					//Basement level
					case 1:
						ubLocationModifier = 28;
						break;
					case 2:
						ubLocationModifier = 35;
						break;

					default:
					        ubLocationModifier = 0;
					//	Assert( 0 );
						break;
				}
		}
		break;
		case SEC_L15:
		{
				//the top floor
				switch( sSectorZ )
				{
					//Basement level
					case 2:
						ubLocationModifier = 40;
						break;
					case 3:
						ubLocationModifier = 40;
						break;

					default:
						Assert( 0 );
						break;
				}
		}
		break;
	}
#else	
	// HEADROCK HAM 5:
	// The calculation has been replaced with an XML table.

	ubLocationModifier = gCoolnessBySector[SECTOR(sSectorX, sSectorY)];

	/*

	// ignore sSectorZ - treat any underground enemies as if they were on the surface!
	bTownId = GetTownIdForSector( sSectorX, sSectorY );


	switch ( bTownId )
	{
		case ORTA:
		case TIXA:
			// enemy troops in these special places are stronger than geography would indicate
			ubPalaceDistance = 4;
			break;

		case ALMA:
			// enemy troops in these special places are stronger than geography would indicate
			ubPalaceDistance = 10;
			break;

		default:
			// how far is this sector from the palace ?
			// the distance returned is in sectors, and the possible range is about 0-20
			ubPalaceDistance = GetPythDistanceFromPalace( sSectorX, sSectorY );
			if ( ubPalaceDistance > MAX_PALACE_DISTANCE )
			{
				ubPalaceDistance = MAX_PALACE_DISTANCE;
			}
	}

	// adjust for distance from Queen's palace (P3) (0 to +30)
	ubLocationModifier = ( ( MAX_PALACE_DISTANCE - ubPalaceDistance ) * DIFF_FACTOR_PALACE_DISTANCE ) / MAX_PALACE_DISTANCE;
	*/

#endif
	return( ubLocationModifier );
}



// grab the distance from the palace
UINT8 GetPythDistanceFromPalace( INT16 sSectorX, INT16 sSectorY )
{
	UINT8 ubDistance = 0;
	INT16 sRows = 0, sCols = 0;
	float fValue = 0.0;

	// grab number of rows and cols
	sRows = (INT16)(abs((sSectorX) - ( PALACE_SECTOR_X )));
	sCols = (INT16)(abs((sSectorY) - ( PALACE_SECTOR_Y )));


	// apply Pythagoras's theorem for right-handed triangle:
	// dist^2 = rows^2 + cols^2, so use the square root to get the distance
	fValue = ( float )sqrt(( float )(sRows * sRows) + ( float )(sCols * sCols));

	if(	fmod( fValue, 1.0f ) >= 0.50 )
	{
		ubDistance = (UINT8)( 1 + fValue );
	}
	else
	{
		ubDistance = ( UINT8 )fValue;
	}

	return( ubDistance );
}



void ReduceHighExpLevels( INT8 *pbExpLevel )
{
	UINT8 ubRoll;
	// important: must reset these to 0 by default for logic to work!
	UINT8 ubChanceLvl8 = 0;
	UINT8 ubChanceLvl7 = 0;
	UINT8 ubChanceLvl6 = 0;
	UINT8 ubChanceLvl5 = 0;


	// this function reduces the experience levels of very high level enemies to something that player can compete with
	// for interrupts.	It doesn't affect attributes and skills, those are rolled prior to this reduction!


	// adjust for game difficulty level
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			// max level: 6
			switch ( *pbExpLevel )
			{
				case 6:
					ubChanceLvl6 = 25;
					ubChanceLvl5 = 100;
					break;
				case 7:
					ubChanceLvl6 = 50;
					ubChanceLvl5 = 100;
					break;
				case 8:
					ubChanceLvl6 = 75;
					ubChanceLvl5 = 100;
					break;
				case 9:
				case 10: // Madd
					ubChanceLvl6 = 100;
					break;
			}
			break;

		case DIF_LEVEL_MEDIUM:
			// max level: 7
			switch ( *pbExpLevel )
			{
				case 7:
					ubChanceLvl7 = 25;
					ubChanceLvl6 = 100;
					break;
				case 8:
					ubChanceLvl7 = 50;
					ubChanceLvl6 = 100;
					break;
				case 9:
				case 10: // Madd
					ubChanceLvl7 = 75;
					ubChanceLvl6 = 100;
					break;
			}
			break;

		case DIF_LEVEL_HARD:
			// max level: 8
			switch ( *pbExpLevel )
			{
				case 8:
					ubChanceLvl8 = 25;
					ubChanceLvl7 = 100;
					break;
				case 9:
				case 10: // Madd
					ubChanceLvl8 = 50;
					ubChanceLvl7 = 100;
					break;
			}
			break;
	}


	ubRoll = ( UINT8 ) Random( 100 );

	if			( ubRoll < ubChanceLvl8 )
		*pbExpLevel	= 8;
	else if ( ubRoll < ubChanceLvl7 )
		*pbExpLevel	= 7;
	else if ( ubRoll < ubChanceLvl6 )
		*pbExpLevel	= 6;
	else if ( ubRoll < ubChanceLvl5 )
		*pbExpLevel	= 5;
	// else leave it alone

}

BOOLEAN TranslateArrayFields(void* out, const void* inp, int len, int cmd)//dnl ch27 240909
{
	switch(cmd)
	{
	case UINT8_UINT8:
		memcpy(out, inp, len);
		//for(int i=0; i<len; i++)
		//	((UINT8*)out)[i] = ((UINT8*)inp)[i];
		break;
	case UINT16_UINT16:
		memcpy(out, inp, sizeof(UINT16)*len);
		//for(int i=0; i<len; i++)
		//	((UINT16*)out)[i] = ((UINT16*)inp)[i];
		break;
	case UINT32_UINT32:
		memcpy(out, inp, sizeof(UINT32)*len);
		//for(int i=0; i<len; i++)
		//	((UINT32*)out)[i] = ((UINT32*)inp)[i];
		break;
	case UINT8_UINT16:
		for(int i=0; i<len; i++)
			((UINT16*)out)[i] = ((UINT8*)inp)[i];
		break;
	case UINT16_UINT8:
		for(int i=0; i<len; i++)
			((UINT8*)out)[i] = (UINT8)((UINT16*)inp)[i];
		break;
	case INT16_INT32:
		for(int i=0; i<len; i++)
			((INT32*)out)[i] = ((INT16*)inp)[i];
		break;
	case INT32_INT16:
		for(int i=0; i<len; i++)
			((INT16*)out)[i] = ((INT32*)inp)[i];
		break;
	default:
		return(FALSE);
	}
	return(TRUE);
}

// SANDRO - function for resetting Squadleader traits number
void ResetNumSquadleadersInArmyGroup( void )
{
	bNumSquadleadersInArmy = 0;
}

// SANDRO - Added a function to add traits to soldiers
BOOLEAN AssignTraitsToSoldier( SOLDIERTYPE *pSoldier, SOLDIERCREATE_STRUCT *pCreateStruct )
{
	INT32 iChance;
	UINT8	ubProgress, ubSolClass;
	BOOLEAN ATraitAssigned = FALSE;
	BOOLEAN BTraitAssigned = FALSE;
	BOOLEAN CTraitAssigned = FALSE;

	ubProgress = HighestPlayerProgressPercentage( );
	ubSolClass = pSoldier->ubSoldierClass;

	// Flugente: soldier profiles - if any traits are in the xml, use them, but fill up empty slots afterwards
	if ( gGameOptions.fNewTraitSystem && pSoldier->usSoldierProfile )
	{
		INT8 type = pSoldier->GetSoldierProfileType( pCreateStruct->bTeam );

		if ( type > -1 )
		{
			if ( zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[0] > 0 )
			{
				pSoldier->stats.ubSkillTraits[0] = zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[0];
				ATraitAssigned = TRUE;
			}

			if ( zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[1] > 0 )
			{
				pSoldier->stats.ubSkillTraits[1] = zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[1];
				BTraitAssigned = TRUE;
			}

			if ( zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[2] > 0 )
			{
				// we have to make sure that not all 3 traits are major traits - if that happens, we ignore the third one
				if ( MajorTrait( pSoldier->stats.ubSkillTraits[0] ) && MajorTrait( pSoldier->stats.ubSkillTraits[1] ) && MajorTrait( zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[2] ) )
					;
				else
				{
					pSoldier->stats.ubSkillTraits[2] = zSoldierProfile[type][pSoldier->usSoldierProfile].uiTrait[2];
					CTraitAssigned = TRUE;
				}
			}
		}
	}

	// First determine who will be the squadleader for this group (administrators do not get a squadleader
	if ( gGameOptions.fNewTraitSystem && bNumSquadleadersInArmy < ((ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ARMY) ? gGameOptions.ubDifficultyLevel : 2) &&
		(ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA ||
		ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA) )
	{
		// set the first in command 
		if ( bNumSquadleadersInArmy == 0 )
		{
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
			{
				iChance = 100;
			}
			else
			{
				iChance = 50;
			}
			if ( gGameExternalOptions.bAssignedTraitsRarity < 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}
			if ( Chance( iChance ) )
			{
				pSoldier->stats.ubSkillTraits[0] = SQUADLEADER_NT;
				// raise level and leadership for this guy
				pSoldier->stats.bExpLevel = min( 10, (pSoldier->stats.bExpLevel + 1) );
				pSoldier->stats.bLeadership = min( 100, (pSoldier->stats.bLeadership + 20) );
				ATraitAssigned = TRUE;

				bNumSquadleadersInArmy++;
			}
		}

		// second level can be given by a chance depending on soldier type
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 60-100%
			iChance = 60 + ubProgress * 2 / 5;
		}
		else
		{
			// 30-70%
			iChance = 30 + ubProgress * 2 / 5;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		if ( Chance( iChance ) )
		{
			if ( !ATraitAssigned )
			{
				pSoldier->stats.ubSkillTraits[0] = SQUADLEADER_NT;
				// raise level and leadership for this guy
				pSoldier->stats.bExpLevel = min( 10, (pSoldier->stats.bExpLevel + 1) );
				pSoldier->stats.bLeadership = min( 100, (pSoldier->stats.bLeadership + 20) );
				ATraitAssigned = TRUE;

				bNumSquadleadersInArmy++;
			}
			else
			{
				pSoldier->stats.ubSkillTraits[1] = SQUADLEADER_NT;
				// raise level and leadership for this guy
				pSoldier->stats.bExpLevel = min( 10, (pSoldier->stats.bExpLevel + 1) );
				pSoldier->stats.bLeadership = min( 100, (pSoldier->stats.bLeadership + 20) );
				BTraitAssigned = TRUE;

				bNumSquadleadersInArmy++;
			}
		}

	}

	// Now look what we have in hands and assign traits according to it

	// ASSAULT RIFLE OR LMG - CHANCE FOR AUTO WEAPONS TRAIT
	if ( Item[pCreateStruct->Inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		(Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_AS_RIFLE ||
		Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_LMG) )
	{
		// setup the chances
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 50-100%
			iChance = 50 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
		{
			// 30-80%
			iChance = 30 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
		{
			// 15-65%
			iChance = 15 + ubProgress / 2;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		// Now assign the trait
		if ( Chance( iChance ) )
		{
			if ( !ATraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[0] = AUTO_WEAPONS_NT;
				else
					pSoldier->stats.ubSkillTraits[0] = AUTO_WEAPS_OT;

				ATraitAssigned = TRUE;

				// Half chance to assign expert level of the trait
				if ( Chance( iChance / 2 ) && !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPONS_NT;
					else
						pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPS_OT;

					BTraitAssigned = TRUE;
				}
			}
			else if ( !BTraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPONS_NT;
				else
					pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPS_OT;

				BTraitAssigned = TRUE;
			}
		}
	}
	// SNIPER RIFLE - CHANCE FOR SNIPER TRAIT
	else if ( Item[pCreateStruct->Inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_SN_RIFLE )
	{
		// setup the chances
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 45-95%
			iChance = 45 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
		{
			// 30-80%
			iChance = 30 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
		{
			// 15-65%
			iChance = 15 + ubProgress / 2;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		// Now assign the trait
		if ( Chance( iChance ) )
		{
			if ( !ATraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[0] = SNIPER_NT;
				else
					pSoldier->stats.ubSkillTraits[0] = PROF_SNIPER_OT;

				ATraitAssigned = TRUE;

				// Lower chance to assign expert level of the trait
				if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = SNIPER_NT;
					else
						pSoldier->stats.ubSkillTraits[1] = PROF_SNIPER_OT;

					BTraitAssigned = TRUE;
				}
			}
			else if ( !BTraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[1] = SNIPER_NT;
				else
					pSoldier->stats.ubSkillTraits[1] = PROF_SNIPER_OT;

				BTraitAssigned = TRUE;
			}
		}
	}
	// RIFLE OR SHOTGUN - CHANCE FOR RANGER TRAIT (new traits only)
	else if ( Item[pCreateStruct->Inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		(Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_RIFLE ||
		Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_SHOTGUN) )
	{
		// setup the chances
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 40-90%
			iChance = 40 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
		{
			// 25-75%
			iChance = 25 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
		{
			// 10-60%
			iChance = 10 + ubProgress / 2;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		// Now assign the trait
		if ( gGameOptions.fNewTraitSystem ) // for STOMP traits only
		{
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = RANGER_NT;
					ATraitAssigned = TRUE;

					// Lower chance to assign expert level of the trait
					if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = RANGER_NT;
						BTraitAssigned = TRUE;
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = RANGER_NT;
					BTraitAssigned = TRUE;
				}
			}
		}
		else
		{
			// with old traits, we have some chance to gain sniper trait with rifle in hands
			if ( Chance( iChance * 2 / 3 ) && (Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_RIFLE) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = PROF_SNIPER_OT;
					ATraitAssigned = TRUE;
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = PROF_SNIPER_OT;
					BTraitAssigned = TRUE;
				}
			}
		}
	}
	// SMG - CHANCE FOR AUTO WEAPONS TRAIT AND CHANCE FOR A SECOND WEAPON (+GUNSLINGER/AMBIDEXTROUS TRAIT)
	else if ( Item[pCreateStruct->Inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_SMG )
	{
		// setup the chances
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 40-90%
			iChance = 40 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
		{
			// 25-75%
			iChance = 25 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
		{
			// 10-60%
			iChance = 10 + ubProgress / 2;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		// Now assign the trait - Auto weapons are superior here
		if ( Chance( iChance ) )
		{
			if ( !ATraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[0] = AUTO_WEAPONS_NT;
				else
					pSoldier->stats.ubSkillTraits[0] = AUTO_WEAPS_OT;

				ATraitAssigned = TRUE;

				// Half chance to assign expert level of the trait
				if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPONS_NT;
					else
						pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPS_OT;

					BTraitAssigned = TRUE;
				}
			}
			else if ( !BTraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
					pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPONS_NT;
				else
					pSoldier->stats.ubSkillTraits[1] = AUTO_WEAPS_OT;

				BTraitAssigned = TRUE;
			}
		}
		// Chance to gain second weapon
		if ( Chance( iChance / 4 ) && !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded ) // 1/4 of chance
		{
			(pCreateStruct->Inv[SECONDHANDPOS]) = (pCreateStruct->Inv[HANDPOS]);

			// if traits not assigned, small chance to assign Gunslinger/Ambidextrous trait
			if ( !ATraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
				{
					if ( pSoldier->stats.ubSkillTraits[1] != AMBIDEXTROUS_NT && pSoldier->stats.ubSkillTraits[2] != AMBIDEXTROUS_NT ) // paranoya check
						pSoldier->stats.ubSkillTraits[0] = AMBIDEXTROUS_NT;
				}
				else
				{
					if ( pSoldier->stats.ubSkillTraits[1] != AMBIDEXT_OT ) // paranoya check
						pSoldier->stats.ubSkillTraits[0] = AMBIDEXT_OT;
				}
				ATraitAssigned = TRUE;
			}
			else if ( !BTraitAssigned )
			{
				if ( gGameOptions.fNewTraitSystem )
				{
					if ( pSoldier->stats.ubSkillTraits[1] != AMBIDEXTROUS_NT && pSoldier->stats.ubSkillTraits[2] != AMBIDEXTROUS_NT ) // paranoya check
						pSoldier->stats.ubSkillTraits[0] = AMBIDEXTROUS_NT;
				}
				else
				{
					if ( pSoldier->stats.ubSkillTraits[0] != AMBIDEXT_OT ) // paranoya check
						pSoldier->stats.ubSkillTraits[1] = AMBIDEXT_OT;
				}
				BTraitAssigned = TRUE;
			}
			else if ( gGameOptions.fNewTraitSystem && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
			{
				if ( pSoldier->stats.ubSkillTraits[0] != AMBIDEXTROUS_NT && pSoldier->stats.ubSkillTraits[1] != AMBIDEXTROUS_NT ) // paranoya check
					pSoldier->stats.ubSkillTraits[2] = AMBIDEXTROUS_NT;

				CTraitAssigned = TRUE;
				return(TRUE); // We no longer need to continue from here
			}
		}
	}
	// PISTOL OR MACHINE PISTOL - CHANCE FOR GUNSLINGER/AMBIDEXTROUS TRAIT AND SECOND WEAPON
	else if ( Item[pCreateStruct->Inv[HANDPOS].usItem].usItemClass == IC_GUN &&
		(Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_PISTOL ||
		Weapon[pCreateStruct->Inv[HANDPOS].usItem].ubWeaponType == GUN_M_PISTOL) )
	{
		// setup the chances
		if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			// 40-90%
			iChance = 40 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
		{
			// 25-75%
			iChance = 25 + ubProgress / 2;
		}
		else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
		{
			// 10-60%
			iChance = 10 + ubProgress / 2;
		}

		// modify the chance by preset ini setting
		if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
		{
			iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
		}

		// Now assign the trait
		if ( Chance( iChance ) )
		{
			if ( gGameOptions.fNewTraitSystem )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = GUNSLINGER_NT;
					ATraitAssigned = TRUE;

					// Lower chance to assign expert level of the trait
					if ( Chance( iChance / 2 ) && !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = GUNSLINGER_NT;
						BTraitAssigned = TRUE;

						// elites can have third skill trait
						if ( Chance( iChance / 3 ) && !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded &&
							(ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
						{
							pSoldier->stats.ubSkillTraits[2] = AMBIDEXTROUS_NT;
							CTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}

					}
					// Chance to gain second weapon and ambidextrous trait
					else if ( Chance( iChance / 2 ) && !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded && !BTraitAssigned ) // 1/2 of chance
					{
						if ( pCreateStruct->bTeam == MILITIA_TEAM && gGameExternalOptions.fMilitiaUseSectorInventory && gGameExternalOptions.fMilitiaUseSectorInventory_Gun )
							;
						else
							(pCreateStruct->Inv[SECONDHANDPOS]) = (pCreateStruct->Inv[HANDPOS]);

						pSoldier->stats.ubSkillTraits[1] = AMBIDEXTROUS_NT;

						BTraitAssigned = TRUE;
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = GUNSLINGER_NT;
					BTraitAssigned = TRUE;

					// elites can have third skill trait
					if ( Chance( iChance / 3 ) && !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded &&
						(ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
					{
						pSoldier->stats.ubSkillTraits[2] = AMBIDEXTROUS_NT;
						CTraitAssigned = TRUE;
						return(TRUE); // We no longer need to continue from here
					}
				}
			}
			else // with original traits
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = AMBIDEXT_OT;
					ATraitAssigned = TRUE;
					// Ambidextrous trait gives us second weapon automatically
					if ( !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded )
					{
						(pCreateStruct->Inv[SECONDHANDPOS]) = (pCreateStruct->Inv[HANDPOS]);
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = AMBIDEXT_OT;
					BTraitAssigned = TRUE;
					// Ambidextrous trait gives us second weapon automatically
					if ( !Item[pCreateStruct->Inv[HANDPOS].usItem].twohanded )
					{
						(pCreateStruct->Inv[SECONDHANDPOS]) = (pCreateStruct->Inv[HANDPOS]);
					}
				}
			}
		}
	}
	// NO GUN IN HANDS, CONTINUE
	else
	{
	}

	//NOW, IF SOME TRAITS HAVEN'T BEEN ASSIGNED, TRY TO ASSIGN DIFFERENT TRAITS
	if ( !ATraitAssigned || !BTraitAssigned || (!CTraitAssigned && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA)) )
	{
		INT8 bLoop;
		BOOLEAN foundMortar = FALSE;
		BOOLEAN foundRocketlauncher = FALSE;
		BOOLEAN foundGrenadelauncher = FALSE;
		BOOLEAN foundKnife = FALSE;
		BOOLEAN foundGrenades = FALSE;
		BOOLEAN foundThrowing = FALSE;
		BOOLEAN foundHtH = FALSE;
		BOOLEAN foundMelee = FALSE;
		BOOLEAN fRadioSetFound = FALSE;
		BOOLEAN fFirstAidKitFound = FALSE;
		BOOLEAN fMedKitFound = FALSE;

		// FIRST FIND OUT THE COMPOSITION OF OUR GEAR
		INT8 invsize = (INT8)pSoldier->inv.size( );
		for ( bLoop = 0; bLoop < invsize; ++bLoop )
		{
			if ( pCreateStruct->Inv[bLoop].exists( ) == true )
			{
				if ( Item[pCreateStruct->Inv[bLoop].usItem].mortar )
					foundMortar = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].rocketlauncher )
					foundRocketlauncher = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].grenadelauncher )
					foundGrenadelauncher = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].usItemClass == IC_BLADE )
					foundKnife = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].usItemClass == IC_THROWING_KNIFE )
					foundThrowing = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].usItemClass == IC_GRENADE )
					foundGrenades = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].brassknuckles )
					foundHtH = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].usItemClass == 128 && // 128 is an identifier of blunt melee weapons
					Item[pCreateStruct->Inv[bLoop].usItem].uiIndex != 0 )
					foundMelee = TRUE;
				else if ( HasItemFlag( pCreateStruct->Inv[bLoop].usItem, RADIO_SET ) )
					fRadioSetFound = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].firstaidkit )
					fFirstAidKitFound = TRUE;
				else if ( Item[pCreateStruct->Inv[bLoop].usItem].medicalkit )
				{
					// Flugente: for enemy medic purposes, med kits also count as first aid kits
					fFirstAidKitFound = TRUE;
					fMedKitFound = TRUE;
				}
			}
		}

		// NOW BASED ON WHAT WE HAVE FOUND, ASSIGN UNASSIGNED TRAITS
		// Flugente: radio operator
		if ( gGameOptions.fNewTraitSystem && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// if we have a radio set, give us the corresponding trait so we can use it...
			if ( fRadioSetFound )
			{
				// this is a minor trait, so try to first fill the minor slot if possible
				// reasoning: this trait has to be one of the first evaluated - getting a radio set is rare, so e want to make sure we become a radio guy if we're lucky
				if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				{
					pSoldier->stats.ubSkillTraits[2] = RADIO_OPERATOR_NT;
					CTraitAssigned = TRUE;
					return(TRUE); // We no longer need to continue from here
				}
				else if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = RADIO_OPERATOR_NT;
					ATraitAssigned = TRUE;
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = RADIO_OPERATOR_NT;
					BTraitAssigned = TRUE;
				}
			}
		}

		// HEAVY WEAPONS TRAIT 
		if ( foundMortar || foundRocketlauncher || foundGrenadelauncher )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 45 + ubProgress / 2; // 45-95% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 30 + ubProgress / 2; // 30-80% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = 15 + ubProgress / 2; // 15-65% chance

			// adust the chance by the type of heavy weapon
			if ( foundMortar )
				iChance = 200; // instant chance if we are a mortar guy
			else if ( foundGrenadelauncher )
				iChance -= 15; // -15% chance if we only have a grenade launcher

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[0] = HEAVY_WEAPONS_NT;
					else
						pSoldier->stats.ubSkillTraits[0] = HEAVY_WEAPS_OT;

					ATraitAssigned = TRUE;

					// half the chance to be an expert
					if ( Chance( iChance / 2 ) && !BTraitAssigned )
					{
						if ( gGameOptions.fNewTraitSystem )
							pSoldier->stats.ubSkillTraits[1] = HEAVY_WEAPONS_NT;
						else
						{
							pSoldier->stats.ubSkillTraits[1] = HEAVY_WEAPS_OT;
							return(TRUE); // We no longer need to continue from here
						}
						BTraitAssigned = TRUE;
					}
				}
				else if ( !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = HEAVY_WEAPONS_NT;
					else
					{
						pSoldier->stats.ubSkillTraits[1] = HEAVY_WEAPS_OT;
						return(TRUE); // We no longer need to continue from here
					}
					BTraitAssigned = TRUE;
				}
			}
		}

		// MARTIAL ARTS - new traits
		if ( (!ATraitAssigned || !BTraitAssigned) && gGameOptions.fNewTraitSystem )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 20 + ubProgress / 4; // 20-45% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 10 + ubProgress / 4; // 10-35% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = ubProgress / 4; // 0-25% chance
			else if ( ubSolClass == SOLDIER_CLASS_ZOMBIE )
				iChance = 100; // 100% chance

			if ( foundHtH ) // if found brass knuckless, increase the chance (doesn't happen often)
				iChance += 35;

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
					ATraitAssigned = TRUE;

					// reduced chance to be an expert
					if ( Chance( iChance / 4 ) && !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = MARTIAL_ARTS_NT;
						BTraitAssigned = TRUE;
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = MARTIAL_ARTS_NT;
					BTraitAssigned = TRUE;
				}
			}
		}

		// NIGHT OPS TRAIT
		if ( (DayTime( ) != TRUE) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 40 + ubProgress * 2 / 5; // 40-80% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 25 + ubProgress * 2 / 5; // 25-65% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = 10 + ubProgress * 2 / 5; // 10-50% chance
			else if ( ubSolClass == SOLDIER_CLASS_ZOMBIE )
				iChance = 100; // 100% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
					{
						pSoldier->stats.ubSkillTraits[0] = NIGHT_OPS_NT;
						ATraitAssigned = TRUE;
					}
					else
					{
						pSoldier->stats.ubSkillTraits[0] = NIGHTOPS_OT;
						ATraitAssigned = TRUE;

						// reduced chance to be an expert
						if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[1] = NIGHTOPS_OT;
							BTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}
					}
				}
				else if ( !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = NIGHT_OPS_NT;
					else
					{
						pSoldier->stats.ubSkillTraits[1] = NIGHTOPS_OT;
						return(TRUE); // We no longer need to continue from here
					}
					BTraitAssigned = TRUE;
				}
				// allow third only to elites
				else if ( !CTraitAssigned && gGameOptions.fNewTraitSystem && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
				{
					pSoldier->stats.ubSkillTraits[2] = NIGHT_OPS_NT;
					CTraitAssigned = TRUE;
					return(TRUE);
				}
			}
		}

		// THROWING TRAIT
		if ( (foundThrowing) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 30 + ubProgress / 3 + 30; // 60-92% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 15 + ubProgress / 3 + 25; // 40-73% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = ubProgress / 3 + 20; // 20-53% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
					{
						pSoldier->stats.ubSkillTraits[0] = THROWING_NT;
						ATraitAssigned = TRUE;
					}
					else
					{
						pSoldier->stats.ubSkillTraits[0] = THROWING_OT;
						ATraitAssigned = TRUE;

						// reduced the chance to be an expert
						if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[1] = THROWING_OT;
							BTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}
					}
				}
				else if ( !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = THROWING_NT;
					else
					{
						pSoldier->stats.ubSkillTraits[1] = THROWING_OT;
						return(TRUE); // We no longer need to continue from here
					}

					BTraitAssigned = TRUE;
				}
				// allow third only to elites
				else if ( !CTraitAssigned && gGameOptions.fNewTraitSystem && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
				{
					pSoldier->stats.ubSkillTraits[2] = THROWING_NT;
					CTraitAssigned = TRUE;
					return(TRUE);
				}
			}
		}

		// DEMOLITIONS TRAIT
		if ( (foundGrenades) && (gGameOptions.fNewTraitSystem) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 20 + ubProgress / 3; // 20-53% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 10 + ubProgress / 3; // 10-43% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = ubProgress / 3; // 0-33% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = DEMOLITIONS_NT;
					ATraitAssigned = TRUE;
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = DEMOLITIONS_NT;
					BTraitAssigned = TRUE;
				}
				// allow third only to elites
				else if ( !CTraitAssigned && gGameOptions.fNewTraitSystem && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
				{
					pSoldier->stats.ubSkillTraits[2] = DEMOLITIONS_NT;
					CTraitAssigned = TRUE;
					return(TRUE);
				}
			}
		}

		// MELEE / KNIFING TRAIT
		if ( (foundKnife || (foundMelee && gGameOptions.fNewTraitSystem)) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 50 + ubProgress * 2 / 5; // 50-90% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 35 + ubProgress * 2 / 5; // 35-75% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = 15 + ubProgress * 2 / 5; // 15-55% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{

					if ( gGameOptions.fNewTraitSystem )
					{
						pSoldier->stats.ubSkillTraits[0] = MELEE_NT;
						ATraitAssigned = TRUE;
					}
					else
					{
						pSoldier->stats.ubSkillTraits[0] = KNIFING_OT;
						ATraitAssigned = TRUE;

						// reduced the chance to be an expert
						if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[1] = KNIFING_OT;
							BTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}
					}
				}
				else if ( !BTraitAssigned )
				{
					if ( gGameOptions.fNewTraitSystem )
						pSoldier->stats.ubSkillTraits[1] = MELEE_NT;
					else
					{
						pSoldier->stats.ubSkillTraits[1] = KNIFING_OT;
						return(TRUE); // We no longer need to continue from here
					}
					BTraitAssigned = TRUE;
				}
				// allow third only to elites
				else if ( !CTraitAssigned && gGameOptions.fNewTraitSystem && (ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA) )
				{
					pSoldier->stats.ubSkillTraits[2] = MELEE_NT;
					CTraitAssigned = TRUE;
					return(TRUE); // We no longer need to continue from here
				}
			}
		}

		// HAND TO HAND TRAIT - old traits nly
		if ( foundHtH && !gGameOptions.fNewTraitSystem && (!ATraitAssigned || !BTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 40 + ubProgress * 2 / 5; // 40-80% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 25 + ubProgress * 2 / 5; // 25-65% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = 10 + ubProgress * 2 / 5; // 10-50% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = HANDTOHAND_OT;
					ATraitAssigned = TRUE;

					// half the chance to be an expert
					if ( Chance( iChance * 2 / 5 ) && !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = HANDTOHAND_OT;
						BTraitAssigned = TRUE;
						return(TRUE); // We no longer need to continue from here
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = HANDTOHAND_OT;
					BTraitAssigned = TRUE;
					return(TRUE); // We no longer need to continue from here
				}
			}
		}

		// MARTIAL ARTS - old traits
		if ( (!ATraitAssigned || !BTraitAssigned) && !gGameOptions.fNewTraitSystem && (pCreateStruct->ubBodyType == REGMALE) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 10 + ubProgress / 4; // 10-35% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = ubProgress / 5; // 0-20% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = 0; // 0% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = MARTIALARTS_OT;
					ATraitAssigned = TRUE;

					// reduced chance to be an expert
					if ( Chance( iChance / 2 ) && !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = MARTIALARTS_OT;
						BTraitAssigned = TRUE;
						return(TRUE); // We no longer need to continue from here
					}
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = MARTIALARTS_OT;
					BTraitAssigned = TRUE;
					return(TRUE); // We no longer need to continue from here
				}
			}
		}

		// BODYBUILDING/ATHLETICS TRAITS (only for new traits)
		if ( gGameOptions.fNewTraitSystem && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
		{
			// setup basic chances based on soldier type
			if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
				iChance = 30 + ubProgress / 2; // 30-90% chance
			else if ( ubSolClass == SOLDIER_CLASS_ARMY || ubSolClass == SOLDIER_CLASS_REG_MILITIA )
				iChance = 10 + ubProgress / 2; // 10-60% chance
			else if ( ubSolClass == SOLDIER_CLASS_ADMINISTRATOR || ubSolClass == SOLDIER_CLASS_GREEN_MILITIA || ubSolClass == SOLDIER_CLASS_BANDIT )
				iChance = ubProgress / 3; // 0-33% chance

			// modify the chance by preset ini setting
			if ( gGameExternalOptions.bAssignedTraitsRarity != 0 )
			{
				iChance += ((iChance * gGameExternalOptions.bAssignedTraitsRarity) / 100);
			}

			// assign the trait based on the chance
			if ( Chance( iChance ) )
			{
				UINT32 uiBodyBuildChance; // chance to geet bodybuilding rather than athletics
				if ( pSoldier->ubBodyType == BIGMALE ) // big guys more likely take bb
					uiBodyBuildChance = 90;
				else if ( pSoldier->ubBodyType == REGFEMALE )  // while women athletics
					uiBodyBuildChance = 10;
				else
					uiBodyBuildChance = 60; // normally prefer bb a bit

				if ( Chance( uiBodyBuildChance ) )
				{
					if ( !ATraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[0] = BODYBUILDING_NT;
						ATraitAssigned = TRUE;
					}
					else if ( !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = BODYBUILDING_NT;
						BTraitAssigned = TRUE;
					}
					else if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
					{
						pSoldier->stats.ubSkillTraits[2] = BODYBUILDING_NT;
						CTraitAssigned = TRUE;
						return(TRUE); // We no longer need to continue from here
					}
					// give us also a small chance to get athletics on top
					if ( Chance( iChance / 3 ) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
					{
						if ( !ATraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[0] = ATHLETICS_NT;
							ATraitAssigned = TRUE;
						}
						else if ( !BTraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[1] = ATHLETICS_NT;
							BTraitAssigned = TRUE;
						}
						else if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
						{
							pSoldier->stats.ubSkillTraits[2] = ATHLETICS_NT;
							CTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}
					}
				}
				else
				{
					if ( !ATraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[0] = ATHLETICS_NT;
						ATraitAssigned = TRUE;
					}
					else if ( !BTraitAssigned )
					{
						pSoldier->stats.ubSkillTraits[1] = ATHLETICS_NT;
						BTraitAssigned = TRUE;
					}
					else if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
					{
						pSoldier->stats.ubSkillTraits[2] = ATHLETICS_NT;
						CTraitAssigned = TRUE;
						return(TRUE); // We no longer need to continue from here
					}
					// give us also a small chance to get bodybuilding on top
					if ( Chance( iChance / 3 ) && (!ATraitAssigned || !BTraitAssigned || !CTraitAssigned) )
					{
						if ( !ATraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[0] = BODYBUILDING_NT;
							ATraitAssigned = TRUE;
						}
						else if ( !BTraitAssigned )
						{
							pSoldier->stats.ubSkillTraits[1] = BODYBUILDING_NT;
							BTraitAssigned = TRUE;
						}
						else if ( ubSolClass == SOLDIER_CLASS_ELITE || ubSolClass == SOLDIER_CLASS_ELITE_MILITIA )
						{
							pSoldier->stats.ubSkillTraits[2] = BODYBUILDING_NT;
							CTraitAssigned = TRUE;
							return(TRUE); // We no longer need to continue from here
						}
					}
				}
			}
		}

		// Flugente: enemy roles: allow medics
		if ( gGameExternalOptions.fEnemyRoles && gGameExternalOptions.fEnemyMedics && gGameOptions.fNewTraitSystem && (!ATraitAssigned || !BTraitAssigned) )
		{
			// if we have a radio set, give us the corresponding trait so we can use it...
			if ( fFirstAidKitFound )
			{
				if ( !ATraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[0] = DOCTOR_NT;
					ATraitAssigned = TRUE;

					// if we found a medkit, chance to become a full doctor
					if ( !BTraitAssigned && fMedKitFound && Chance( 50 ) )
					{
						pSoldier->stats.ubSkillTraits[1] = DOCTOR_NT;
						BTraitAssigned = TRUE;
					}

					// make sure we ar reasonably skilled
					pSoldier->stats.bMedical = max( pSoldier->stats.bMedical, 50 );
				}
				else if ( !BTraitAssigned )
				{
					pSoldier->stats.ubSkillTraits[1] = DOCTOR_NT;
					BTraitAssigned = TRUE;

					// make sure we ar reasonably skilled
					pSoldier->stats.bMedical = max( pSoldier->stats.bMedical, 50 );
				}
			}
		}
	}
	// RETURN TRUE IF ALL TRAITS ASSIGNED OTHERWISE FALSE
	if ( (!gGameOptions.fNewTraitSystem && ATraitAssigned && BTraitAssigned) || (gGameOptions.fNewTraitSystem && ATraitAssigned && BTraitAssigned && CTraitAssigned) )
		return(TRUE);
	else
		return(FALSE);
}
