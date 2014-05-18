#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "Assignments.h"
	#include "Strategic.h"
	#include "Items.h"
	#include "Overhead.h"
	#include "Game Clock.h"
	#include "stdlib.h"
	#include "Message.h"
	#include "Font Control.h"
	#include "Map Screen Interface.h"
	#include "Soldier Profile Type.h"
	#include "Soldier Profile.h"
	#include "Campaign.h"
	#include "Queen Command.h"
	#include "strategicmap.h"
	#include "Text.h"
	#include "dialogue control.h"
	#include "NPC.h"
	#include "Strategic Town Loyalty.h"
	#include "animation control.h"
	#include "mapscreen.h"
	#include "Squads.h"
	#include "Map Screen Helicopter.h"
	#include "PopUpBox.h"
	#include "Vehicles.h"
	#include "Merc Contract.h"
	#include "Map Screen Interface Map.h"
	#include "Strategic Movement.h"
	#include "laptop.h"
	#include "Finances.h"
	#include "LaptopSave.h"
	#include "renderworld.h"
	#include "Interface Control.h"
	#include "Interface.h"
	#include "Soldier Find.h"
	#include "ai.h"
	#include "utilities.h"
	#include "random.h"
	#include "line.h"
	#include "Soldier Add.h"
	#include "GameSettings.h"
	#include "Isometric Utils.h"
	#include "Soldier Macros.h"
	#include "Explosion Control.h"
	#include "SkillCheck.h"
	#include "Quests.h"
	#include "Town Militia.h"
	#include "Map Screen Interface Border.h"
	#include "math.h"
	#include "SkillCheck.h"
	#include "Strategic Pathing.h"
	#include "Game Event Hook.h"
	#include "Strategic Event Handler.h"
	#include "Map Information.h"
	#include "Strategic Status.h"
	#include "history.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "interface dialogue.h"
	// added by SANDRO
	#include "AIInternals.h"
	#include "Morale.h"
	#include "Food.h"
	#include "Tactical Save.h"		// added by Flugente
	#include "Campaign Types.h"		// added by Flugente
	#include "Strategic Town Loyalty.h"
#endif
#include <vector>
#include <queue>

#include "Vehicles.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

#include "MilitiaSquads.h"
// HEADROCK HAM 3.5: Include Facility data
#include "Facilities.h"

#include "Vehicles.h"

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "mercs.h"
#include "ub_config.h"
#endif

// Flugente: external sector data
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// various reason an assignment can be aborted before completion
enum{
	NO_MORE_MED_KITS = 40,
	INSUF_DOCTOR_SKILL,
	NO_MORE_TOOL_KITS,
	INSUF_REPAIR_SKILL,

	NUM_ASSIGN_ABORT_REASONS
};

enum{
	REPAIR_MENU_VEHICLE1 = 0,
	REPAIR_MENU_VEHICLE2,
	REPAIR_MENU_VEHICLE3,
//	REPAIR_MENU_SAM_SITE,
	REPAIR_MENU_ROBOT,
	REPAIR_MENU_ITEMS,
	REPAIR_MENU_CANCEL,
};

enum{
	VEHICLE_MENU_VEHICLE1 = 0,
	VEHICLE_MENU_VEHICLE2,
	VEHICLE_MENU_VEHICLE3,
	VEHICLE_MENU_VEHICLE4,	// WANNE: Allow up to 6 vehicles
	VEHICLE_MENU_VEHICLE5,
	VEHICLE_MENU_VEHICLE6,
	VEHICLE_MENU_CANCEL,
};

enum{
	MOVEITEM_MENU_1 = 0,
	MOVEITEM_MENU_2,
	MOVEITEM_MENU_3,
	MOVEITEM_MENU_4,
	MOVEITEM_MENU_5,
	MOVEITEM_MENU_6,
	MOVEITEM_MENU_7,
	MOVEITEM_MENU_8,
	MOVEITEM_MENU_9,
	MOVEITEM_MENU_10,
	MOVEITEM_MENU_CANCEL,
};


/* CHRISL: Adjusted enumerations to allow for seperation of the three different pocket types in the new 
inventory system. */
enum {
	REPAIR_HANDS_AND_ARMOR = 0,
	REPAIR_HEADGEAR,
	REPAIR_BIG_POCKETS,
	REPAIR_MED_POCKETS,
	REPAIR_SML_POCKETS,
	REPAIR_LBE_GEAR, // HEADROCK HAM B2.8: New pass type for fixing LBEs only
	NUM_REPAIR_PASS_TYPES,
};

// HEADROCK HAM B2.8: Changed LBEs to be the final pass
#define FINAL_REPAIR_PASS			REPAIR_LBE_GEAR


/* CHRISL: bSlot[xx] array declaration needs to reflect largest number of inventory locations.  New inventory
system increses possible locations from 12 to 30. Also added a new field so we can set number of choices based
on game options. */
typedef struct REPAIR_PASS_SLOTS_TYPE
{
	UINT8		ubChoices[2];						// how many valid choices there are in this pass
	INT8		bSlot[ 30 ];					// list of slots to be repaired in this pass
} REPAIR_PASS_SLOTS_TYPE;

/* CHRISL:  Added new definitions introduced by the new inventory system.*/
REPAIR_PASS_SLOTS_TYPE gRepairPassSlotList[ NUM_REPAIR_PASS_TYPES ] =
{					// pass		# choices	# new choices		slots repaired in this pass
	{ /* hands and armor */		5,			12,					HANDPOS, SECONDHANDPOS, VESTPOS, HELMETPOS, LEGPOS, VESTPOCKPOS, LTHIGHPOCKPOS, RTHIGHPOCKPOS, CPACKPOCKPOS, BPACKPOCKPOS, GUNSLINGPOCKPOS, KNIFEPOCKPOS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ /* headgear */			2,			2,					HEAD1POS, HEAD2POS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ /* big pockets */			4,			7,					BIGPOCK1POS, BIGPOCK2POS, BIGPOCK3POS, BIGPOCK4POS, BIGPOCK5POS, BIGPOCK6POS, BIGPOCK7POS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ /* med pockets */			0,			4,					MEDPOCK1POS, MEDPOCK2POS, MEDPOCK3POS, MEDPOCK4POS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
	{ /* sml pockets */			8,			30,					SMALLPOCK1POS, SMALLPOCK2POS, SMALLPOCK3POS, SMALLPOCK4POS, SMALLPOCK5POS, SMALLPOCK6POS, SMALLPOCK7POS, SMALLPOCK8POS, SMALLPOCK9POS, SMALLPOCK10POS, SMALLPOCK11POS, SMALLPOCK12POS, SMALLPOCK13POS, SMALLPOCK14POS, SMALLPOCK15POS, SMALLPOCK16POS, SMALLPOCK17POS, SMALLPOCK18POS, SMALLPOCK19POS, SMALLPOCK20POS, SMALLPOCK21POS, SMALLPOCK22POS, SMALLPOCK23POS, SMALLPOCK24POS, SMALLPOCK25POS, SMALLPOCK26POS, SMALLPOCK27POS, SMALLPOCK28POS, SMALLPOCK29POS, SMALLPOCK30POS },
	{ /* HEADROCK HAM B2.8: LBE Slot pass */	0,		5,		VESTPOCKPOS, LTHIGHPOCKPOS, RTHIGHPOCKPOS, CPACKPOCKPOS, BPACKPOCKPOS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
};

extern STR16 sRepairsDoneString[];

// PopUp Box Handles
INT32 ghAssignmentBox = -1;
INT32 ghEpcBox = -1;
INT32 ghSquadBox = -1;
INT32 ghVehicleBox = -1;
INT32 ghRepairBox = -1;
INT32 ghMoveItemBox = -1;
INT32 ghTrainingBox = -1;
INT32 ghAttributeBox = -1;
INT32 ghRemoveMercAssignBox = -1;
INT32 ghContractBox = -1;
INT32 ghMoveBox = -1;
// HEADROCK HAM 3.6: Facility Menu, Submenu
INT32 ghFacilityBox = -1;
INT32 ghFacilityAssignmentBox = -1;
//INT32 ghUpdateBox = -1;
// anv: snitch menus
INT32 ghSnitchBox = -1;
INT32 ghSnitchToggleBox = -1;
INT32 ghSnitchSectorBox = -1;

// the x,y position of assignment pop up in tactical
INT16 gsAssignmentBoxesX, gsAssignmentBoxesY;


// assignment menu mouse regions
MOUSE_REGION	gAssignmentMenuRegion[ MAX_ASSIGN_STRING_COUNT ];
MOUSE_REGION	gTrainingMenuRegion[ MAX_TRAIN_STRING_COUNT ];
MOUSE_REGION	gAttributeMenuRegion[ MAX_ATTRIBUTE_STRING_COUNT ];
MOUSE_REGION	gSquadMenuRegion[ MAX_SQUAD_MENU_STRING_COUNT ];
MOUSE_REGION	gContractMenuRegion[ MAX_CONTRACT_MENU_STRING_COUNT ];
MOUSE_REGION	gRemoveMercAssignRegion[ MAX_REMOVE_MERC_COUNT ];
MOUSE_REGION	gEpcMenuRegion[ MAX_EPC_MENU_STRING_COUNT ];
MOUSE_REGION	gRepairMenuRegion[ 20 ];
MOUSE_REGION	gMoveItem[ 20 ];

#define			MOVEITEM_MAX_SECTORS	10
UINT8			usMoveItemSectors[MOVEITEM_MAX_SECTORS];

// mouse region for vehicle menu
MOUSE_REGION		gVehicleMenuRegion[ 20 ];
// HEADROCK HAM 3.6: Facility Menu
MOUSE_REGION	gFacilityMenuRegion[ MAX_NUM_FACILITY_TYPES ];
MOUSE_REGION	gFacilityAssignmentMenuRegion[ NUM_FACILITY_ASSIGNMENTS ];
// anv: snitch menus
MOUSE_REGION	gSnitchMenuRegion[ MAX_SNITCH_MENU_STRING_COUNT ];
MOUSE_REGION	gSnitchToggleMenuRegion[ MAX_SNITCH_TOGGLE_MENU_STRING_COUNT ];
MOUSE_REGION	gSnitchSectorMenuRegion[ MAX_SNITCH_SECTOR_MENU_STRING_COUNT ];

MOUSE_REGION	gAssignmentScreenMaskRegion;

BOOLEAN fShownAssignmentMenu = FALSE;
BOOLEAN fShowVehicleMenu = FALSE;
BOOLEAN fShowRepairMenu = FALSE;
BOOLEAN fShowMoveItemMenu = FALSE;
BOOLEAN fShownContractMenu = FALSE;
// anv: snitch menus
BOOLEAN fShowSnitchMenu = FALSE;
BOOLEAN fShowSnitchToggleMenu = FALSE;
BOOLEAN fShowSnitchSectorMenu = FALSE;

BOOLEAN fFirstClickInAssignmentScreenMask = FALSE;

// render pre battle interface?
extern BOOLEAN gfRenderPBInterface;
extern BOOLEAN fMapScreenBottomDirty;
extern SOLDIERTYPE *pMilitiaTrainerSoldier;

// in the mapscreen?
extern BOOLEAN fInMapMode;

// we are in fact training?..then who temmates, or self?
INT8 gbTrainingMode = -1;

// who is the highlighted guy
extern UINT16 gusUIFullTargetID;

// showing town info?
extern BOOLEAN fShowTownInfo;

extern INT32 giMapBorderButtons[];

extern BOOLEAN fProcessingAMerc;
extern SOLDIERTYPE *pProcessingSoldier ;

BOOLEAN gfAddDisplayBoxToWaitingQueue = FALSE;

// redraw character list
extern BOOLEAN fDrawCharacterList;

extern BOOLEAN fSelectedListOfMercsForMapScreen[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];

SOLDIERTYPE *gpDismissSoldier = NULL;

/////////////////////////////////////////////////////////////////////
// these added by SANDRO
SOLDIERTYPE *pAutomaticSurgeryDoctor;
SOLDIERTYPE *pAutomaticSurgeryPatient;
void SurgeryBeforeDoctoringRequesterCallback( UINT8 bExitValue );
void SurgeryBeforePatientingRequesterCallback( UINT8 bExitValue );
INT16 MakeAutomaticSurgeryOnAllPatients( SOLDIERTYPE * pDoctor );
BOOLEAN MakeAutomaticSurgery( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pDoctor );
/////////////////////////////////////////////////////////////////////

BOOLEAN gfReEvaluateEveryonesNothingToDo = FALSE;

// HEADROCK HAM 3.6: Stored Facility worker (for callback purposes)
SOLDIERTYPE *gpFacilityStaffer;
// HEADROCK HAM 3.6: Current FacilityType whose Assignments are shown in the Sub-Menu
INT8 gubFacilityInSubmenu;
UINT8 gubFacilityLineForSubmenu; // Which line to highlight in the facility menu...
/*
// the amount time must be on assignment before it can have any effect
#define MINUTES_FOR_ASSIGNMENT_TO_COUNT	45

// number we divide the total pts accumlated per day by for each assignment period
#define ASSIGNMENT_UNITS_PER_DAY 24

// base skill to deal with an emergency
#define BASE_MEDICAL_SKILL_TO_DEAL_WITH_EMERGENCY 20

// multiplier for skill needed for each point below OKLIFE
#define MULTIPLIER_FOR_DIFFERENCE_IN_LIFE_VALUE_FOR_EMERGENCY 4

// number of pts needed for each point below OKLIFE
#define POINT_COST_PER_HEALTH_BELOW_OKLIFE 2

// how many points of healing each hospital patients gains per hour in the hospital
#define HOSPITAL_HEALING_RATE		5				// a top merc doctor can heal about 4 pts/hour maximum, but that's spread among patients!

// increase to reduce repair pts, or vice versa
#define REPAIR_RATE_DIVISOR 2500
// increase to reduce doctoring pts, or vice versa
#define DOCTORING_RATE_DIVISOR 2400				// at 2400, the theoretical maximum is 150 full healing pts/day

// cost to unjam a weapon in repair pts
#define REPAIR_COST_PER_JAM 2

// divisor for rate of self-training
#define SELF_TRAINING_DIVISOR				1000
// the divisor for rate of training bonus due to instructors influence
#define INSTRUCTED_TRAINING_DIVISOR 3000

// this controls how fast town militia gets trained
#define TOWN_TRAINING_RATE	4

#define MAX_MILITIA_TRAINERS_PER_SECTOR 2

// militia training bonus for EACH level of teaching skill (percentage points)
#define TEACH_BONUS_TO_TRAIN 30
// militia training bonus for RPC (percentage points)
#define RPC_BONUS_TO_TRAIN	10

// the bonus to training in marksmanship in the Alma gun range sector
#define GUN_RANGE_TRAINING_BONUS	25

// breath bonus divider
#define BREATH_BONUS_DIVIDER 10

// the min rating that is need to teach a fellow teammate
#define MIN_RATING_TO_TEACH 25

// activity levels for natural healing ( the higher the number, the slower the natural recover rate
#define LOW_ACTIVITY_LEVEL		1
#define MEDIUM_ACTIVITY_LEVEL	4
#define HIGH_ACTIVITY_LEVEL			12
*/
/*
// the min breath to stay awake
#define MIN_BREATH_TO_STAY_AWAKE 15

// average number of hours a merc needs to sleep per day
#define AVG_NUMBER_OF_HOURS_OF_SLEEP_NEEDED 7
*/

/* Assignment distance limits removed.	Sep/11/98.	ARM
#define MAX_DISTANCE_FOR_DOCTORING	5
#define MAX_DISTANCE_FOR_REPAIR			5
#define MAX_DISTANCE_FOR_TRAINING		5
*/

/*
// controls how easily SAM sites are repaired
// NOTE: A repairman must generate a least this many points / hour to be ABLE to repair a SAM site at all!
#define SAM_SITE_REPAIR_DIVISOR		10

// minimum condition a SAM site must be in to be fixable
#define MIN_CONDITION_TO_FIX_SAM 20
*/

void MakeSoldierKnownAsMercInPrison(SOLDIERTYPE *pSoldier, INT16 sMapX, INT16 sMapY);

BOOLEAN IsSoldierKnownAsMercInSector(SOLDIERTYPE *pSoldier, INT16 sMapX, INT16 sMapY);

// how many points worth of tool kits does the character have?
UINT16 ToolKitPoints(SOLDIERTYPE *pSoldier);

// how many points worth of doctoring does the character have in his medical kits ?
UINT16 TotalMedicalKitPoints(SOLDIERTYPE *pSoldier);

// handle doctor in this sector
void HandleDoctorsInSector( INT16 sX, INT16 sY, INT8 bZ );

// handle any repair man in sector
void HandleRepairmenInSector( INT16 sX, INT16 sY, INT8 bZ );

// heal characters in this sector with this doctor
void HealCharacters( SOLDIERTYPE *pDoctor,	INT16 sX, INT16 sY, INT8 bZ );

// update characters who might done healing but are still patients
void UpdatePatientsWhoAreDoneHealing( void );

// returns minimum medical skill necessary to treat this patient
UINT8 GetMinHealingSkillNeeded( SOLDIERTYPE *pPatient );

// heal patient, given doctor and total healing pts available to doctor at this time
UINT16 HealPatient( SOLDIERTYPE *pPatient, SOLDIERTYPE * pDoctor, UINT16 usHealAmount );

// can item be repaired?
BOOLEAN IsItemRepairable( SOLDIERTYPE* pSoldier, UINT16 usItem, INT16 bStatus, INT16 bThreshold );

// does another merc have a repairable item on them?
OBJECTTYPE* FindRepairableItemOnOtherSoldier( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOtherSoldier, UINT8 ubPassType );

//CHRISL: This function will handle the actual searching for repairable items
OBJECTTYPE* FindRepairableItemInSpecificPocket( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, UINT8 subObject);

//CHRISL: This function will search through LBENODE items for repairable items
OBJECTTYPE* FindRepairableItemInLBENODE(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, UINT8 subObject);

// repair stuff
void HandleRepairBySoldier( SOLDIERTYPE *pSoldier );

// rest the character
void RestCharacter( SOLDIERTYPE *pSoldier );
// fatigue the character
void FatigueCharacter( SOLDIERTYPE *pSoldier );

// a list of which sectors have characters
BOOLEAN fSectorsWithSoldiers[ MAP_WORLD_X * MAP_WORLD_Y ][ 4 ];

// can soldier repair robot
BOOLEAN CanCharacterRepairRobot( SOLDIERTYPE *pSoldier );

// can the character repair this vehicle?
BOOLEAN CanCharacterRepairVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId );

// handle training of character in sector
void HandleTrainingInSector( INT16 sMapX, INT16 sMapY, INT8 bZ );

// QSort compare function for town training
int TownTrainerQsortCompare(const void *pArg1, const void *pArg2);

// this function will actually pass on the pts to the mercs stat
void TrainSoldierWithPts( SOLDIERTYPE *pSoldier, INT16 sTrainPts );

// train militia in this sector with this soldier
BOOLEAN TrainTownInSector( SOLDIERTYPE *pTrainer, INT16 sMapX, INT16 sMapY, INT16 sTrainingPts );

// Flugente:: handle processing of prisoners
void HandlePrisonerProcessingInSector( INT16 sMapX, INT16 sMapY, INT8 bZ );

// Flugente: prisons can riot if there aren't enough guards around
void HandlePrison( INT16 sMapX, INT16 sMapY, INT8 bZ );

// Flugente: assigned mercs can move equipemnt in city sectors
void HandleEquipmentMove( INT16 sMapX, INT16 sMapY, INT8 bZ );

// Flugente: handle radio scanning assignments
void HandleRadioScanInSector( INT16 sMapX, INT16 sMapY, INT8 bZ );

// reset scan flags in all sectors
void ClearSectorScanResults();

void HandleSpreadingPropagandaInSector( INT16 sMapX, INT16 sMapY, INT8 bZ );

// Flugente: handle militia command
void HandleMilitiaCommand();

// is the character between secotrs in mvt
BOOLEAN CharacterIsBetweenSectors( SOLDIERTYPE *pSoldier );

// update soldier life
void UpDateSoldierLife( SOLDIERTYPE *pSoldier );

// handle natural healing for all mercs on players team
void HandleNaturalHealing( void );

// handle natural healing for any individual grunt
void HandleHealingByNaturalCauses( SOLDIERTYPE *pSoldier );

/*
// auto sleep mercs
BOOLEAN AutoSleepMerc( SOLDIERTYPE *pSoldier );
*/

// assignment screen mask
void AssignmentScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );

// glow area for contract region?
BOOLEAN fGlowContractRegion = FALSE;


void HandleShadingOfLinesForSquadMenu( void );
void HandleShadingOfLinesForVehicleMenu( void );
void HandleShadingOfLinesForRepairMenu( void );
void HandleShadingOfLinesForMoveItemMenu( void );
void HandleShadingOfLinesForTrainingMenu( void );
void HandleShadingOfLinesForAttributeMenus( void );
// HEADROCK HAM 3.6: Shade Facility Box Lines
void HandleShadingOfLinesForFacilityMenu( void );
void HandleShadingOfLinesForFacilityAssignmentMenu( void );
// anv: snitch menus shading
void HandleShadingOfLinesForSnitchMenu( void );
void HandleShadingOfLinesForSnitchToggleMenu( void );
void HandleShadingOfLinesForSnitchSectorMenu( void );

// post message about contract
void PostContractMessage( SOLDIERTYPE *pCharacter, INT32 iContract );

// post a terminate message
void PostTerminateMessage( SOLDIERTYPE *pCharacter );

BOOLEAN DisplayVehicleMenu( SOLDIERTYPE *pSoldier );
BOOLEAN DisplayRepairMenu( SOLDIERTYPE *pSoldier );
// HEADROCK HAM 3.6: Display Facility Menu.
BOOLEAN DisplayFacilityMenu( SOLDIERTYPE *pSoldier );
BOOLEAN DisplayFacilityAssignmentMenu( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType );

// Flugente: move items menu
BOOLEAN DisplayMoveItemsMenu( SOLDIERTYPE *pSoldier );

// create menus
void CreateEPCBox( void );
void CreateSquadBox( void );
void CreateVehicleBox();
void CreateRepairBox( void );
void CreateMoveItemBox( void );
// HEADROCK HAM 3.6: Facility Box.
void CreateFacilityBox( void );
void CreateFacilityAssignmentBox( void );
// anv: snitch menus boxes
void CreateSnitchBox( void );
void CreateSnitchToggleBox( void );
void CreateSnitchSectorBox( void );
/*
// get how fast the person regains sleep
INT8 GetRegainDueToSleepNeeded( SOLDIERTYPE *pSoldier, INT32 iRateOfReGain );
*/

void PositionCursorForTacticalAssignmentBox( void );

// can this soldier be healed by this doctor?
// SANDRO - attention - a variable added to these 2 functions
BOOLEAN CanSoldierBeHealedByDoctor( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pDoctor, BOOLEAN fIgnoreAssignment, BOOLEAN fThisHour, BOOLEAN fSkipKitCheck, BOOLEAN fSkipSkillCheck, BOOLEAN fCheckForSurgery );
UINT8 GetNumberThatCanBeDoctored( SOLDIERTYPE *pDoctor, BOOLEAN fThisHour, BOOLEAN fSkipKitCheck, BOOLEAN fSkipSkillCheck, BOOLEAN fCheckForSurgery );

void CheckForAndHandleHospitalPatients( void );
void HealHospitalPatient( SOLDIERTYPE *pPatient, UINT16 usHealingPtsLeft );

BOOLEAN MakeSureToolKitIsInHand( SOLDIERTYPE *pSoldier );

void RepositionMouseRegions( void );
void CheckAndUpdateTacticalAssignmentPopUpPositions( void );
void HandleRestFatigueAndSleepStatus( void );
BOOLEAN CharacterIsTakingItEasy( SOLDIERTYPE *pSoldier );
void RepairMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
BOOLEAN CanCharacterDoctorButDoesntHaveMedKit( SOLDIERTYPE *pSoldier );
BOOLEAN CanCharacterRepairButDoesntHaveARepairkit( SOLDIERTYPE *pSoldier );

// robot replated stuff
BOOLEAN IsRobotInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
SOLDIERTYPE * GetRobotSoldier( void );
UINT8 RepairRobot( SOLDIERTYPE *pRobot, UINT8 ubRepairPts, BOOLEAN *pfNothingLeftToRepair );
UINT8 HandleRepairOfRobotBySoldier( SOLDIERTYPE *pSoldier, UINT8 ubRepairPts, BOOLEAN *pfNothingLeftToRepair );
BOOLEAN HandleAssignmentExpansionAndHighLightForAssignMenu( SOLDIERTYPE *pSoldier );
BOOLEAN HandleAssignmentExpansionAndHighLightForTrainingMenu( void );
BOOLEAN HandleAssignmentExpansionAndHighLightForFacilityMenu ( void ); // Facility menu and submenu expansion
BOOLEAN HandleAssignmentExpansionAndHighLightForSnitchMenu ( void );
BOOLEAN HandleShowingOfMovementBox( void );
//BOOLEAN HandleShowingOfUpBox( void );
void ReportTrainersTraineesWithoutPartners( void );
BOOLEAN ValidTrainingPartnerInSameSectorOnAssignmentFound( SOLDIERTYPE *pSoldier, INT8 bTargetAssignment, INT8 bTargetStat );

extern void AddSectorForSoldierToListOfSectorsThatCompletedMilitiaTraining( SOLDIERTYPE *pSoldier );

extern BOOLEAN SectorIsImpassable( INT16 sSector );

extern BOOLEAN CanChangeSleepStatusForCharSlot( INT8 bCharNumber );

extern UINT32 VirtualSoldierDressWound( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVictim, OBJECTTYPE *pKit, INT16 sKitPts, INT16 sStatus, BOOLEAN fOnSurgery ); // variable added by SANDRO

// only 2 trainers are allowed per sector, so this function counts the # in a guy's sector
// HEADROCK HAM 3.6: Now takes an extra argument for Militia Type
INT8 CountMilitiaTrainersInSoldiersSector( SOLDIERTYPE * pSoldier, UINT8 ubMilitiaType );
// HEADROCK HAM 3.6: Check number of mercs currently staffing a specific facility.
INT8 CountFreeFacilitySlots( UINT8 sMapX, UINT8 sMapY, UINT8 ubFacilityType );
// HEADROCK HAM 3.6: Check number of mercs currently staffing a specific facility AND performing the same assignment
INT8 CountFreeFacilityAssignmentSlots( UINT8 sMapX, UINT8 sMapY, UINT8 ubFacilityType, UINT8 ubAssignmentType );

// notify player of assignment attempt failure
void NotifyPlayerOfAssignmentAttemptFailure( INT8 bAssignment );

BOOLEAN PlayerSoldierTooTiredToTravel( SOLDIERTYPE *pSoldier );

void AssignmentAborted( SOLDIERTYPE *pSoldier, UINT8 ubReason );

UINT8 CalcSoldierNeedForSleep( SOLDIERTYPE *pSoldier );

UINT32 GetLastSquadListedInSquadMenu( void );

BOOLEAN IsAnythingAroundForSoldierToRepair( SOLDIERTYPE * pSoldier );
BOOLEAN HasCharacterFinishedRepairing( SOLDIERTYPE * pSoldier );
BOOLEAN DoesCharacterHaveAnyItemsToRepair( SOLDIERTYPE * pSoldier, INT8 bHighestPass );

BOOLEAN CanCharacterRepairAnotherSoldiersStuff( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOtherSoldier );

// can this character EVER train militia?
BOOLEAN BasicCanCharacterTrainMilitia( SOLDIERTYPE *pCharacter );
BOOLEAN BasicCanCharacterTrainMobileMilitia( SOLDIERTYPE *pSoldier );
// Can this character EVER work in any facility?
BOOLEAN BasicCanCharacterFacility( SOLDIERTYPE *pSoldier );

SOLDIERTYPE *GetSelectedAssignSoldier( BOOLEAN fNullOK );

BOOLEAN RepairObject( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOwner, OBJECTTYPE * pObj, UINT8 * pubRepairPtsLeft );
void RepairItemsOnOthers( SOLDIERTYPE *pSoldier, UINT8 *pubRepairPtsLeft );
BOOLEAN UnjamGunsOnSoldier( SOLDIERTYPE *pOwnerSoldier, SOLDIERTYPE *pRepairSoldier, UINT8 *pubRepairPtsLeft );

#ifdef JA2UB
void HaveMercSayWhyHeWontLeave( SOLDIERTYPE *pSoldier ); //Ja25 UB
BOOLEAN CanMercBeAllowedToLeaveTeam( SOLDIERTYPE *pSoldier ); //JA25 UB
#endif

/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
BOOLEAN IsTheSAMSiteInSectorRepairable( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
BOOLEAN SoldierInSameSectorAsSAM( SOLDIERTYPE *pSoldier );
BOOLEAN CanSoldierRepairSAM( SOLDIERTYPE *pSoldier, INT8 bRepairPoints );
BOOLEAN IsSoldierCloseEnoughToSAMControlPanel( SOLDIERTYPE *pSoldier );
*/

/* Assignment distance limits removed.	Sep/11/98.	ARM
BOOLEAN IsSoldierCloseEnoughToADoctor( SOLDIERTYPE *pPatient );
*/

#ifdef JA2BETAVERSION
void VerifyTownTrainingIsPaidFor( void );
#endif

/// Forward declarations for dynamic repair system.
/// They are only used in this file.

/// Comparator function for priority_queue to determine the repair priority of an item.
struct RepairPriority; 
/// Struct to store items to repair in
struct RepairItem;
// The data structure used for collecting repairable items
typedef std::priority_queue<RepairItem, std::vector<RepairItem>, RepairPriority> RepairQueue;
/// Gets the minimum durability of all items in an object stack
static INT16 GetMinimumStackDurability(const OBJECTTYPE* pObj);
/// Check if a gun is jammed
static BOOLEAN IsGunJammed(const OBJECTTYPE* pObj);
/// Collect items that need repairing and add them to the repair queue
static void CollectRepairableItems(SOLDIERTYPE* pRepairSoldier, SOLDIERTYPE* pSoldier, RepairQueue& itemsToFix);

extern BOOLEAN HandleSoldierDeath( SOLDIERTYPE *pSoldier , BOOLEAN *pfMadeCorpse );

extern BOOLEAN CheckConditionsForBattle( GROUP *pGroup );

void InitSectorsWithSoldiersList( void )
{
	// init list of sectors
	memset( &fSectorsWithSoldiers, 0, sizeof( fSectorsWithSoldiers ) );

	return;
}


void BuildSectorsWithSoldiersList( void )
{
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	pSoldier = MercPtrs[ 0 ];

	// fills array with pressence of player controlled characters
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if(pTeamSoldier->bActive)
		{
		fSectorsWithSoldiers[ pTeamSoldier->sSectorX + pTeamSoldier->sSectorY * MAP_WORLD_X ][ pTeamSoldier->bSectorZ ] = TRUE;
		}
	}
}

void ChangeSoldiersAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment )
{
	// This is the most basic assignment-setting function.	It must be called before setting any subsidiary
	// values like fFixingRobot.	It will clear all subsidiary values so we don't leave the merc in a messed
	// up state!

	AssertNotNIL(pSoldier);

	pSoldier->bAssignment = bAssignment;
/// don't kill iVehicleId, though, 'cause militia training tries to put guys back in their vehicles when it's done(!)

	pSoldier->flags.fFixingSAMSite = FALSE;
	pSoldier->flags.fFixingRobot = FALSE;
	pSoldier->bVehicleUnderRepairID = -1;

	// HEADROCK HAM 3.6: Clean out new Facility Operation variable.
	pSoldier->sFacilityTypeOperated = -1;

	if ( ( bAssignment == DOCTOR ) || ( bAssignment == PATIENT ) || ( bAssignment == ASSIGNMENT_HOSPITAL ) )
	{
		AddStrategicEvent( EVENT_BANDAGE_BLEEDING_MERCS, GetWorldTotalMin() + 1, 0 );
	}

	// update character info, and the team panel
	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;

	// merc may have come on/off duty, make sure map icons are updated
	fMapPanelDirty = TRUE;
}


BOOLEAN BasicCanCharacterAssignment( SOLDIERTYPE * pSoldier, BOOLEAN fNotInCombat )
{
	AssertNotNIL(pSoldier);
	// global conditions restricting all assignment changes
	if ( SectorIsImpassable( (INT16) SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ) )
	{
		return( FALSE );
	}

	if ( fNotInCombat && pSoldier->bActive && pSoldier->bInSector && gTacticalStatus.fEnemyInSector )
	{
		return( FALSE );
	}

	return( TRUE );
}



/*
BOOLEAN CanSoldierAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment )
{
	AssertNotNIL(pSoldier);
	switch( bAssignment )
	{
		case( DOCTOR ):
			return( CanCharacterDoctor( pSoldier ) );
			break;
		case( PATIENT ):
			return( CanCharacterPatient( pSoldier ) );
			break;
		case( REPAIR ):
			return( CanCharacterRepair( pSoldier ) );
			break;
		case( TRAIN_TOWN ):
			return( CanCharacterTrainMilitia( pSoldier ) );
			break;
		case( TRAIN_SELF ):
			return( CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, TRUE, FALSE ) );
			break;
		case( TRAIN_TEAMMATE ):
			return( CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, FALSE, TRUE ) );
			break;
		case TRAIN_BY_OTHER:
			return( CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, TRUE, FALSE ) );
			break;
		case( VEHICLE ):
			return( CanCharacterVehicle( pSoldier ) );
			break;
		default:
			return( (CanCharacterSquad( pSoldier, bAssignment ) == CHARACTER_CAN_JOIN_SQUAD ) );
			break;
	}
}
*/



BOOLEAN CanCharacterDoctorButDoesntHaveMedKit( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);
	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// make sure character is alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// has medical skill?
	if( pSoldier->stats.bMedical <= 0 )
	{
		// no skill whatsoever
		return ( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	return( TRUE );
}

// is character capable of 'playing' doctor?
// check that character is alive, conscious, has medical skill and equipment
BOOLEAN CanCharacterDoctor( SOLDIERTYPE *pSoldier )
{
	BOOLEAN fFoundMedKit = FALSE;
	INT8 bPocket = 0;

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	if( CanCharacterDoctorButDoesntHaveMedKit( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// find med kit
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = HANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		// doctoring is allowed using either type of med kit (but first aid kit halves doctoring effectiveness)
		if( IsMedicalKitItem( &( pSoldier -> inv[ bPocket ] ) ) )
		{
			fFoundMedKit = TRUE;
			break;
		}
	}

	if( fFoundMedKit == FALSE )
	{
		return( FALSE );
	}

	// all criteria fit, can doctor
	return ( TRUE );

}



BOOLEAN IsAnythingAroundForSoldierToRepair( SOLDIERTYPE * pSoldier )
{
	AssertNotNIL(pSoldier);

	// items?
	if ( DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS ) )
	{
		return( TRUE );
	}

	// robot?
	if ( CanCharacterRepairRobot( pSoldier ) )
	{
		return( TRUE );
	}

	// vehicles?
	if ( pSoldier->bSectorZ == 0 )
	{
		for (INT32 iCounter = 0; iCounter < ubNumberOfVehicles; iCounter++ )
		{
			if ( pVehicleList[ iCounter ].fValid == TRUE )
			{
				// the helicopter, is NEVER repairable...
				if ( iCounter != iHelicopterVehicleId )
				{
					if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounter ) )
					{
						if( CanCharacterRepairVehicle( pSoldier, iCounter ) == TRUE )
						{
							// there is a repairable vehicle here
							return( TRUE );
						}
					}
				}
			}
		}
	}

	return( FALSE );
}



BOOLEAN HasCharacterFinishedRepairing( SOLDIERTYPE * pSoldier )
{
	BOOLEAN fCanStillRepair;

	AssertNotNIL(pSoldier);

	// NOTE: This must detect situations where the vehicle/robot has left the sector, in which case we want the
	// guy to say "assignment done", so we return that he can no longer repair

	// check if we are repairing a vehicle
	if ( pSoldier->bVehicleUnderRepairID != -1 )
	{
		fCanStillRepair = CanCharacterRepairVehicle( pSoldier, pSoldier->bVehicleUnderRepairID );
	}
	// check if we are repairing a robot
	else if( pSoldier->flags.fFixingRobot )
	{
		fCanStillRepair = CanCharacterRepairRobot( pSoldier );
	}
	else	// repairing items
	{
		fCanStillRepair = DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS );
	}

	// if it's no longer damaged, we're finished!
	return( !fCanStillRepair );
}



BOOLEAN DoesCharacterHaveAnyItemsToRepair( SOLDIERTYPE *pSoldier, INT8 bHighestPass )
{
	INT8	bPocket;
	UINT8	ubItemsInPocket, ubObjectInPocketCounter;
	INT8 bLoop;
	OBJECTTYPE * pObj;
	UINT8 ubPassType;

	AssertNotNIL(pSoldier);

	// check for jams
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = HELMETPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		ubItemsInPocket = pSoldier->inv[ bPocket ].ubNumberOfObjects;
		// unjam any jammed weapons
		// run through pocket and repair
		for( ubObjectInPocketCounter = 0; ubObjectInPocketCounter < ubItemsInPocket; ubObjectInPocketCounter++ )
		{
			// jammed gun?
			if ( ( Item[ pSoldier->inv[ bPocket ].usItem ].usItemClass == IC_GUN ) && ( pSoldier->inv[ bPocket ][0]->data.gun.bGunAmmoStatus < 0 ) )
			{
				return( TRUE );
			}
		}
	}

	// now check for items to repair
	// CHRISL: Changed to dynamically determine max inventory locations.
	for( bPocket = HELMETPOS; bPocket < NUM_INV_SLOTS; bPocket++ )
	{
		// run through pocket
		for( ubObjectInPocketCounter = 0; ubObjectInPocketCounter < pSoldier->inv[ bPocket ].ubNumberOfObjects; ubObjectInPocketCounter++ )
		{
			pObj = FindRepairableItemInSpecificPocket(pSoldier, &(pSoldier->inv[ bPocket ]), ubObjectInPocketCounter);
			// if it's repairable and NEEDS repairing
			if(pObj != 0)
			{
				return( TRUE );
			}
			if(UsingNewInventorySystem() == true && Item[pSoldier->inv[ bPocket ].usItem].usItemClass == IC_LBEGEAR)
			{
				pObj = FindRepairableItemInLBENODE(pSoldier, &pSoldier->inv[ bPocket ], ubObjectInPocketCounter);
				if(pObj != 0)
				{
					return( TRUE );
				}
			}
		}
	}

	// if we wanna check for the items belonging to others in the sector
	if ( bHighestPass != - 1 )
	{
		// now look for items to repair on other mercs
		for( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop < gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
		{
			SOLDIERTYPE* pOtherSoldier = MercPtrs[ bLoop ];

			if ( CanCharacterRepairAnotherSoldiersStuff( pSoldier, pOtherSoldier ) )
			{
				// okay, seems like a candidate!	Check if he has anything that needs unjamming or repairs
				// CHRISL: Changed to dynamically determine max inventory locations.
				for ( bPocket = HANDPOS; bPocket < NUM_INV_SLOTS; bPocket++ )
				{
					// the object a weapon? and jammed?
					if ( ( Item[ pOtherSoldier->inv[ bPocket ].usItem ].usItemClass == IC_GUN ) && ( pOtherSoldier->inv[ bPocket ][0]->data.gun.bGunAmmoStatus < 0 ) )
					{
						return( TRUE );
					}
				}
				// repair everyone's hands and armor slots first, then headgear, and pockets last
				for ( ubPassType = REPAIR_HANDS_AND_ARMOR; ubPassType <= ( UINT8 ) bHighestPass; ubPassType++ )
				{
					if (FindRepairableItemOnOtherSoldier( pSoldier, pOtherSoldier, ubPassType )) {
						return( TRUE );
					}
				}
			}
		}
	}

	return( FALSE );
}



BOOLEAN BasicCanCharacterRepair( SOLDIERTYPE * pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// make sure character is alive and oklife
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// has repair skill?
	if( pSoldier->stats.bMechanical <= 0 )
	{
		// no skill whatsoever
		return ( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	return( TRUE );
}

BOOLEAN CanCharacterRepairButDoesntHaveARepairkit( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( BasicCanCharacterRepair( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// make sure he actually doesn't have a toolkit
	if ( FindToolkit( pSoldier ) != NO_SLOT )
	{
		return( FALSE );
	}

	return( TRUE );
}

// can character be assigned as repairman?
// check that character is alive, oklife, has repair skill, and equipment, etc.
BOOLEAN CanCharacterRepair( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	if ( BasicCanCharacterRepair( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// make sure he has a toolkit
	if ( FindToolkit( pSoldier ) == NO_SLOT )
	{
		return( FALSE );
	}

	// anything around to fix?
	if ( !IsAnythingAroundForSoldierToRepair( pSoldier ) )
	{
		return( FALSE );
	}

	// NOTE: This will not detect situations where character lacks the SKILL to repair the stuff that needs repairing...
	// So, in that situation, his assignment will NOT flash, but a message to that effect will be reported every hour.

	// all criteria fit, can repair
	return ( TRUE );
}


// can character be set to patient?
BOOLEAN CanCharacterPatient( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// Robot must be REPAIRED to be "healed", not doctored
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return ( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return ( FALSE );
	}

	// SANDRO - changed a bit
	// is character alive?
	if( pSoldier->stats.bLife <= 0 ) 
	{
		// dead
		return ( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	// SANDRO - added check if having damaged stat
	for ( UINT8 i = 0; i < NUM_DAMAGABLE_STATS; ++i)
	{
		if ( pSoldier->ubCriticalStatDamage[i] > 0 )
			return ( TRUE );

		// Flugente: stats can also be damaged
		if ( !gGameOptions.fFoodSystem || ( gGameOptions.fFoodSystem && pSoldier->bFoodLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold ) )
		{
			if ( pSoldier->usStarveDamageHealth > 0 || pSoldier->usStarveDamageStrength > 0 )
				return ( TRUE );
		}
	}

	// Flugente: check if poisoned
	if ( pSoldier->bPoisonSum > 0 )
		return( TRUE );

	// if we don't have damaged stat, look if we need healing
	if ( pSoldier->stats.bLife == pSoldier->stats.bLifeMax )
		return( FALSE );

	// alive and can be healed
	return ( TRUE );
}



BOOLEAN BasicCanCharacterTrainMilitia( SOLDIERTYPE *pSoldier )
{
	/////////////////////////////////////////////////////
	// Tests whether character can do assignments at all!

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// make sure character is alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// underground training is not allowed (code doesn't support and it's a reasonable enough limitation)
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	// Is character on the way into/out of Arulco?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// Is character travelling between sectors?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// Is character an Escortee?
	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// Is character a Vehicle or Robot?
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Tests to see whether this sector allows training militia for ANYBODY.

	// is there a SAM Site in the character's current sector?
	if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( pSoldier->sSectorX, pSoldier->sSectorY ) ].bNameId == BLANK_SECTOR )
	{
		BOOLEAN fSamSitePresent = IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

		// check if sam site
		if( fSamSitePresent == FALSE )
		{
			// nope
			return ( FALSE );
		}
	}
	else
	{
		// There's a city here. Does it allow training militia?
		INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
		if (!MilitiaTrainingAllowedInTown( bTownId ))
		{
			// City does not allow militia training at all.
			return ( FALSE );
		}
	}

	// HEADROCK HAM 3.5: Only facilities allow militia training, and determine how many trainers can work here.
	// Does sector have at least one facility that allows training?
	BOOLEAN fMilitiaTrainingAllowed = FALSE;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Does it allow training militia?
			if (gFacilityTypes[cnt].ubMilitiaTrainersAllowed)
			{
				// Cool.
				fMilitiaTrainingAllowed = TRUE;
			}
		}
	}
	if (!fMilitiaTrainingAllowed)
	{
		// Militia training NOT allowed here!
		return ( FALSE );
	}

	return ( TRUE );
}
// Determines whether the character has the required condition to train Militia at this time.
// The conditions tested in this function might change WHILE THE CHARACTER IS ALREADY TRAINING MILITIA, which is
// how this function is normally different from "BasicCan...".
BOOLEAN CanCharacterTrainMilitia( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// Make sure the basic sector/merc variables are still applicable. This is simply a fail-safe.
	if( !BasicCanCharacterTrainMilitia( pSoldier ) )
	{
		// Soldier/Sector have somehow failed the basic test. Character automatically fails this test as well.
		return( FALSE );
	}

	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}

	// Has leadership skill?
	if( pSoldier->stats.bLeadership <= 0 )
	{
		// no skill whatsoever
		return ( FALSE );
	}

	// Sector Loyalty above minimum?
	if( !DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) )
	{
		// Not enough Loyalty...
		return ( FALSE );
	}

	// HEADROCK HAM 3: When "Minimum Leadership for Militia Training" is enforced, this value holds the
	// merc's effective leadership, after the "TEACHER" trait is taken into account.
	UINT16 usEffectiveLeadership;

	// HEADROCK HAM 3: Determine whether the merc has enough leadership to train militia. The teacher trait may
	// increase or decrease the effective skill.
	if( gGameExternalOptions.ubMinimumLeadershipToTrainMilitia > 0 )
	{
		// Read BASE leadership
		usEffectiveLeadership = pSoldier->stats.bLeadership;
 
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits
		{
			if (HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ))
			{
				// bonus from Teaching trait
				usEffectiveLeadership = (usEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 );
			}
		}
		// Apply modifier for TEACHER trait, if that feature is activated
		else if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
		{
			// Modifier applied once for each TEACHING level.
			for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pSoldier, TEACHING_OT ); i++ )
			{
				// This is a percentage modifier.
				usEffectiveLeadership = (usEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100;
			}
		}
		
		usEffectiveLeadership = __min(100,usEffectiveLeadership);
		
		// Is leadership too low to proceed?
		if (usEffectiveLeadership < gGameExternalOptions.ubMinimumLeadershipToTrainMilitia)
		{
			return ( FALSE );
		}
	}

	//////////////////////////////////////////////
	// HEADROCK HAM 3.5: Militia Training Facility 
	//

	// Militia training is enabled in the sector only if there is a facility that allows this here. 
	// If one or more facilities are found, positive values are summed up and presented as the number 
	// of trainers allowed in the sector. Values are read from XML, and can be set to mimic JA2
	// defaults. This renders the INI setting "MAX_MILITIA_TRAINERS.." obsolete.
	// HEADROCK HAM 3.5: Only facilities allow militia training, and determine how many trainers can work here.
	// Does sector have at least one facility that allows training?
	UINT8 ubFacilityTrainersAllowed = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Increase tally
			ubFacilityTrainersAllowed += gFacilityTypes[cnt].ubMilitiaTrainersAllowed;
		}
	}

	// Count number of trainers already operating here
	if ( CountMilitiaTrainersInSoldiersSector( pSoldier, TOWN_MILITIA ) >= ubFacilityTrainersAllowed )
	{
		// Too many trainers in sector.
		return (FALSE);
	}

	// Is town full of Elites?
	if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, ELITE_MILITIA ))
	{
		// Town is full of Elites. No further training required.
		// Also note that this takes care of Regulars as well, if Elite training is disabled.
		return( FALSE );
	}

	// If we've reached this, then all is well.
	return( TRUE );
}

BOOLEAN DoesTownHaveRatingToTrainMilitia( INT8 bTownId )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Assignments1");
	
	// Ignores check if FALSE, thereby allowing militia training for town sectors with deactivated loyalty tag
	if (gfTownUsesLoyalty[ bTownId ])
	{
		// min loyalty rating?
		if( ( gTownLoyalty[ bTownId ].ubRating < gGameExternalOptions.iMinLoyaltyToTrain ) )
		{
			// nope
			return( FALSE );
		}
	}

	return( TRUE );
}



BOOLEAN DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( SOLDIERTYPE *pSoldier )
{
	INT8 bTownId = 0;
	BOOLEAN fSamSitePresent = FALSE;

	AssertNotNIL(pSoldier);

	// underground training is not allowed (code doesn't support and it's a reasonable enough limitation)
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

	// is there a town really here
	if( bTownId == BLANK_SECTOR )
	{
		fSamSitePresent = IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

		// if there is a sam site here
		if( fSamSitePresent )
		{
			return( TRUE );
		}

		return( FALSE );
	}

	// does this town have sufficient loyalty to train militia
	if( DoesTownHaveRatingToTrainMilitia( bTownId ) == FALSE )
	{
		return( FALSE );
	}

	return( TRUE );
}



INT8 CountMilitiaTrainersInSoldiersSector( SOLDIERTYPE * pSoldier, UINT8 ubMilitiaType )
{
	INT8	bLoop;
	INT8	bCount = 0;

	AssertNotNIL(pSoldier);

	for ( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
	{
		SOLDIERTYPE* pOtherSoldier = MercPtrs[ bLoop ];
		if ( pSoldier != pOtherSoldier && pOtherSoldier->bActive && pOtherSoldier->stats.bLife >= OKLIFE && pOtherSoldier->sSectorX == pSoldier->sSectorX && pOtherSoldier->sSectorY == pSoldier->sSectorY && pSoldier->bSectorZ == pOtherSoldier->bSectorZ )
		{
			// Count depends on Militia Type requested
			if (ubMilitiaType == TOWN_MILITIA && pOtherSoldier->bAssignment == TRAIN_TOWN )
			{
				bCount++;
			}
			else if (ubMilitiaType == MOBILE_MILITIA && pOtherSoldier->bAssignment == TRAIN_MOBILE )
			{
				bCount++;
			}
		}
	}
	return( bCount );
}



BOOLEAN IsMilitiaTrainableFromSoldiersSectorMaxed( SOLDIERTYPE *pSoldier, INT8 iMilitiaType )
{
	INT8 bTownId = 0;
	BOOLEAN fSamSitePresent = FALSE;

	AssertNotNIL(pSoldier);

	if( pSoldier->bSectorZ != 0 )
	{
		return( TRUE );
	}

	bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

	// is there a town really here
	if( bTownId == BLANK_SECTOR )
	{
		fSamSitePresent = IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

		// if there is a sam site here
		if( fSamSitePresent )
		{
			if( IsSAMSiteFullOfMilitia( pSoldier->sSectorX, pSoldier->sSectorY,iMilitiaType ) )
				{
					return( TRUE );
				}
				return( FALSE );
		}
		return( FALSE );
	}

	// this considers *ALL* safe sectors of the town, not just the one soldier is in
	if( IsTownFullMilitia( bTownId, iMilitiaType ) )
	{
		// town is full of militia
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN CanCharacterTrainStat( SOLDIERTYPE *pSoldier, INT8 bStat, BOOLEAN fTrainSelf, BOOLEAN fTrainTeammate )
{
	// is the character capable of training this stat? either self or as trainer

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// underground training is not allowed (code doesn't support and it's a reasonable enough limitation)
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// check stat values, 0 means no chance in hell
	switch( bStat )
	{
		case ( STRENGTH ):
			// strength
			if ( pSoldier->stats.bStrength < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bStrength < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bStrength >=	gGameExternalOptions.ubTrainingSkillMax ) && ( fTrainSelf ) )
			{
				return ( FALSE );
			}
		break;
		case( DEXTERITY ):
			// dexterity
			if ( pSoldier->stats.bDexterity < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bDexterity < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bDexterity >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}
		break;
		case( AGILITY ):
			// agility
			if ( pSoldier->stats.bAgility < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bAgility < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bAgility >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}

		break;
		case( HEALTH ):
			// health
			if ( pSoldier->stats.bLifeMax < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bLifeMax < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bLifeMax >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}

		break;
		case( MARKSMANSHIP ):
			// marksmanship
			if ( pSoldier->stats.bMarksmanship < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bMarksmanship < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bMarksmanship >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}

		break;
		case( MEDICAL ):
			// medical
			if ( pSoldier->stats.bMedical < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bMedical < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bMedical >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}

		break;
		case( MECHANICAL ):
			// mechanical
			if ( pSoldier->stats.bMechanical < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bMechanical < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bMechanical >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}
		break;
		case( LEADERSHIP ):
			// leadership
			if ( pSoldier->stats.bLeadership < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bLeadership < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bLeadership >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}
		break;
		case( EXPLOSIVE_ASSIGN ):
			// explosives
			if ( pSoldier->stats.bExplosive < gGameExternalOptions.ubTrainingSkillMin )
				return FALSE;
			else if( ( ( pSoldier->stats.bExplosive < gGameExternalOptions.ubMinSkillToTeach ) && ( fTrainTeammate ) ) )
			{
				return ( FALSE );
			}
			else if( ( pSoldier->stats.bExplosive >= gGameExternalOptions.ubTrainingSkillMax )&&( fTrainSelf ) )
			{
				return ( FALSE );
			}
		break;
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// stat is ok and character alive and conscious
	return( TRUE );
}


BOOLEAN CanCharacterOnDuty( SOLDIERTYPE *pSoldier )
{
	// can character commit themselves to on duty?

	AssertNotNIL(pSoldier);

	// only need to be alive and well to do so right now
	// alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	if ( !BasicCanCharacterAssignment( pSoldier, FALSE ) )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				if( gGameExternalOptions.ubSkyriderHotLZ == 0 )
					return( FALSE );
				else
					return( TRUE );
			}
		}
	}
		// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}


	// ARM: New rule: can't change squads or exit vehicles between sectors!
	if( pSoldier->flags.fBetweenSectors )
	{
		return ( FALSE );
	}

/*
	if( pSoldier->flags.fBetweenSectors )
	{
		if( pSoldier->bAssignment == VEHICLE )
		{
			if( GetNumberInVehicle( pSoldier->iVehicleId ) == 1 )
			{
				// can't change, go away
				return( FALSE );
			}
		}
	}
*/

	return( TRUE );
}



BOOLEAN CanCharacterPractise( SOLDIERTYPE *pSoldier )
{
	// can character practise right now?

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// only need to be alive and well to do so right now
	// alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}


	// can practise
	return( TRUE );
}


BOOLEAN CanCharacterTrainTeammates( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// can character train at all
	if( CanCharacterPractise( pSoldier ) == FALSE )
	{
		// nope
		return( FALSE );
	}

	// if alone in sector, can't enter the attributes submenu at all
	if ( PlayerMercsInSector( ( UINT8 ) pSoldier->sSectorX, ( UINT8 ) pSoldier->sSectorY, pSoldier->bSectorZ ) == 0 )
	{
		return( FALSE );
	}

	// ARM: we allow this even if there are no students assigned yet.	Flashing is warning enough.
	return( TRUE );
}


BOOLEAN CanCharacterBeTrainedByOther( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// can character train at all
	if( CanCharacterPractise( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// if alone in sector, can't enter the attributes submenu at all
	if ( PlayerMercsInSector( ( UINT8 ) pSoldier->sSectorX, ( UINT8 ) pSoldier->sSectorY, pSoldier->bSectorZ ) == 0 )
	{
		return( FALSE );
	}

	// ARM: we now allow this even if there are no trainers assigned yet.	Flashing is warning enough.
	return( TRUE );
}



// can character sleep right now?
BOOLEAN CanCharacterSleep( SOLDIERTYPE *pSoldier, BOOLEAN fExplainWhyNot )
{
	CHAR16 sString[ 128 ];

	AssertNotNIL(pSoldier);

	// dead or dying?
	if( pSoldier->stats.bLife < OKLIFE )
	{
		return( FALSE );
	}

	// vehicle or robot?
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return( FALSE );
	}

	// POW?
	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return( FALSE );
	}


	// traveling?
	if ( pSoldier->flags.fBetweenSectors )
	{
		// if walking
		if ( pSoldier->bAssignment != VEHICLE )
		{
			// can't sleep while walking or driving a vehicle
			if( fExplainWhyNot )
			{
				// on the move, can't sleep
				swprintf( sString, zMarksMapScreenText[ 5 ], pSoldier->GetName() );
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			}

			return( FALSE );
		}
		else	// in a vehicle
		{
			// if this guy has to drive ('cause nobody else can)
			if ( SoldierMustDriveVehicle( pSoldier, pSoldier->iVehicleId, FALSE ) )
			{
				// can't sleep while walking or driving a vehicle
				if( fExplainWhyNot )
				{
					// is driving, can't sleep
					swprintf( sString, zMarksMapScreenText[ 7 ], pSoldier->GetName() );
					DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
				}

				return( FALSE );
			}
		}
	}
	else	// in a sector
	{
		// if not above it all...
		if ( !SoldierAboardAirborneHeli( pSoldier ) )
		{
			// if he's in the loaded sector, and it's hostile or in combat
			if( pSoldier->bInSector && ( ( gTacticalStatus.uiFlags & INCOMBAT ) || gTacticalStatus.fEnemyInSector ) )
			{
				if( fExplainWhyNot )
				{
					DoScreenIndependantMessageBox( Message[ STR_SECTOR_NOT_CLEARED ], MSG_BOX_FLAG_OK, NULL );
				}

				return( FALSE );
			}

			// on surface, and enemies are in the sector
			if( ( pSoldier->bSectorZ == 0 ) && ( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) > 0 ) )
			{
				if( fExplainWhyNot )
				{
					DoScreenIndependantMessageBox( Message[ STR_SECTOR_NOT_CLEARED ], MSG_BOX_FLAG_OK, NULL );
				}

				return( FALSE );
			}
		}
	}


	// not tired?
	// HEADROCK HAM 3.5: Facilities can now reduce the maximum fatigue.

	if( pSoldier->bBreathMax >= __min(BREATHMAX_FULLY_RESTED, GetSectorModifier( pSoldier, FACILITY_MAX_BREATH ) ) )
	{
		if( fExplainWhyNot )
		{
			swprintf( sString, zMarksMapScreenText[ 4 ], pSoldier->GetName() );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		}

		return( FALSE );
	}


	// can sleep
	return( TRUE );
}


BOOLEAN CanCharacterBeAwakened( SOLDIERTYPE *pSoldier, BOOLEAN fExplainWhyNot )
{
	CHAR16 sString[ 128 ];

	AssertNotNIL(pSoldier);

	// if dead tired
	if( ( pSoldier->bBreathMax <= BREATHMAX_ABSOLUTE_MINIMUM ) && !pSoldier->flags.fMercCollapsedFlag )
	{
		// should be collapsed, then!
		pSoldier->flags.fMercCollapsedFlag = TRUE;
	}

	// merc collapsed due to being dead tired, you can't wake him up until he recovers substantially
	if ( pSoldier->flags.fMercCollapsedFlag == TRUE )
	{
		if ( fExplainWhyNot )
		{
			swprintf( sString, zMarksMapScreenText[ 6 ], pSoldier->GetName() );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		}

		return( FALSE );
	}

	// can be awakened
	return( TRUE );
}


BOOLEAN CanCharacterVehicle( SOLDIERTYPE *pSoldier )
{
	// can character enter/leave vehicle?

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// only need to be alive and well to do so right now
	// alive and conscious
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// character on the move?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// underground?
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	// check in helicopter in hostile sector
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	// any accessible vehicles in soldier's sector (excludes those between sectors, etc.)
	if( AnyAccessibleVehiclesInSoldiersSector( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// have to be in mapscreen (strictly for visual reasons - we don't want them just vanishing if in tactical)
	if( fInMapMode == FALSE )
	{
		return( FALSE );
	}

	// if we're in BATTLE in the current sector, disallow
	if ( gTacticalStatus.fEnemyInSector )
	{
		if ( ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->sSectorY == gWorldSectorY ) && ( pSoldier->bSectorZ == gbWorldSectorZ) )
		{
			return( FALSE );
		}
	}

	return( TRUE );
}


INT8 CanCharacterSquad( SOLDIERTYPE *pSoldier, INT8 bSquadValue )
{
	// can character join this squad?
	INT16 sX, sY, sZ;

	AssertLT( bSquadValue, ON_DUTY );
	AssertNotNIL(pSoldier);

	if ( pSoldier->bAssignment == bSquadValue )
	{
		return( CHARACTER_CANT_JOIN_SQUAD_ALREADY_IN_IT );
	}

	// is the character alive and well?
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( CHARACTER_CANT_JOIN_SQUAD );
	}

	// in transit?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( CHARACTER_CANT_JOIN_SQUAD );
	}

	if ( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		// not allowed to be put on a squad
		return( CHARACTER_CANT_JOIN_SQUAD );
	}

/* Driver can't abandon vehicle between sectors - OBSOLETE - nobody is allowed to change squads between sectors now!
	if( pSoldier->flags.fBetweenSectors )
	{
		if( pSoldier->bAssignment == VEHICLE )
		{
			if( GetNumberInVehicle( pSoldier->iVehicleId ) == 1 )
			{
				// can't change, go away
				return( CHARACTER_CANT_JOIN_SQUAD );
			}
		}
	}
*/

	// see if the squad us at the same x,y,z
	SectorSquadIsIn( bSquadValue, &sX, &sY, &sZ );

	// check sector x y and z, if not same, cannot join squad
	if( ( sX != pSoldier->sSectorX ) || ( sY != pSoldier->sSectorY ) || ( sZ != pSoldier->bSectorZ ) )
	{
		// is there anyone on this squad?
		if( NumberOfPeopleInSquad( bSquadValue ) > 0 )
		{
			return ( CHARACTER_CANT_JOIN_SQUAD_TOO_FAR );
		}
	}

	if( IsThisSquadOnTheMove( bSquadValue ) == TRUE )
	{
		// can't join while squad is moving
		return( CHARACTER_CANT_JOIN_SQUAD_SQUAD_MOVING );
	}

	if ( DoesVehicleExistInSquad( bSquadValue ) )
	{
		// sorry can't change to this squad that way!
		return( CHARACTER_CANT_JOIN_SQUAD_VEHICLE );
	}

	//SQUAD10 FIX
	if ( NumberOfPeopleInSquad( bSquadValue ) >= gGameOptions.ubSquadSize )
	{
		return( CHARACTER_CANT_JOIN_SQUAD_FULL );
	}

	return ( CHARACTER_CAN_JOIN_SQUAD );
}

BOOLEAN CanCharacterSnitch( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// has character a snitch trait
	if( ProfileHasSkillTrait( pSoldier->ubProfile, SNITCH_NT ) )
	{
		return( TRUE );
	}
	return( FALSE );
}

BOOLEAN CanCharacterSpreadPropaganda( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !CanCharacterSnitch( pSoldier ) )
	{
		return( FALSE );
	}
	// underground propaganda is not allowed (code doesn't support and it's a reasonable enough limitation)
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}
	// is there a town really here
	if( GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY ) == BLANK_SECTOR )
	{
		return( FALSE );
	}
	// it's not a major city (Tixa, Estoni, Orta)
	if( !gfTownUsesLoyalty[GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY )] )
	{
		return( FALSE );
	}
	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}
	return( TRUE );
}

BOOLEAN CanCharacterGatherInformation( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !CanCharacterSnitch( pSoldier ) )
	{
		return( FALSE );
	}
	// underground propaganda is not allowed (code doesn't support and it's a reasonable enough limitation)
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}
	// is there a town really here
	if( GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY ) == BLANK_SECTOR )
	{
		return( FALSE );
	}
	// it's not a major city (Tixa, Estoni, Orta)
	if( !gfTownUsesLoyalty[GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY )] )
	{
		return( FALSE );
	}
	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}
	return( TRUE );
}

BOOLEAN CanCharacterSnitchInPrison( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	if ( !CanCharacterSnitch( pSoldier ) )
	{
		return( FALSE );
	}
	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}
	if( IsSoldierKnownAsMercInSector(pSoldier, pSoldier->sSectorX, pSoldier->sSectorY ) ) 
	{
		return( FALSE );
	}
	// check if there's prison in sector
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )][cnt].fFacilityHere)
		{
			// we determine wether this is a prison by checking for usPrisonBaseLimit
			if (gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0)
			{
				// Are there any prisoners in this prison? note that there are no underground prisons
				if ( !pSoldier->bSectorZ )
				{
					SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ] );
		
					UINT8 tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = 0;
					if ( GetNumberOfPrisoners(pSectorInfo, &tmp1, &tmp2, &tmp3, &tmp4) > 0 )
						return TRUE;
				}
			}
		}
	}



	return( FALSE );
}

BOOLEAN IsCharacterInTransit( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// valid character?
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}


	// check if character is currently in transit
	if( pSoldier->bAssignment == IN_TRANSIT )
	{
		// yep
		return ( TRUE );
	}

	// no
	return ( FALSE );
}


void UpdateAssignments()
{
	INT8 sX,sY, bZ;

	// init sectors with soldiers list
	InitSectorsWithSoldiersList( );

	// build list
	BuildSectorsWithSoldiersList(	);

	// handle natural healing
	HandleNaturalHealing( );

	// handle any patients in the hospital
	CheckForAndHandleHospitalPatients( );

	// see if any grunt or trainer just lost student/teacher
	ReportTrainersTraineesWithoutPartners( );

	// clear out the update list
	ClearSectorListForCompletedTrainingOfMilitia( );


	// rest resting mercs, fatigue active mercs,
	// check for mercs tired enough go to sleep, and wake up well-rested mercs
	HandleRestFatigueAndSleepStatus( );


#ifdef JA2BETAVERSION
	// put this BEFORE training gets handled to avoid detecting an error everytime a sector completes training
	VerifyTownTrainingIsPaidFor();
#endif

	// HEADROCK HAM 3.6: See what effect, if any, our Facility Staffers have on global variables.
	UpdateGlobalVariablesFromFacilities();

	// reset scan flags in all sectors
	ClearSectorScanResults();

	// run through sectors and handle each type in sector
	for(sX = 0 ; sX < MAP_WORLD_X; sX++ )
	{
		for( sY =0; sY < MAP_WORLD_X; sY++ )
		{
			for( bZ = 0; bZ < 4; bZ++)
			{
				// handle militia squads movings and creating (not an assignment)
				if(!bZ && sX < 17 && sY < 17 && sX > 0 && sY > 0)
					UpdateMilitiaSquads( sX, sY );

				// is there anyone in this sector?
				if( fSectorsWithSoldiers[ sX + sY * MAP_WORLD_X ][ bZ ]	== TRUE )
				{
					// handle any doctors
					HandleDoctorsInSector( sX, sY, bZ );

					// handle any repairmen
					HandleRepairmenInSector( sX, sY, bZ );

					// handle any training
					HandleTrainingInSector( sX, sY, bZ );
					
					// handle training of character in sector
					HandleRadioScanInSector( sX, sY, bZ );

					HandleSpreadingPropagandaInSector( sX, sY, bZ );

					// handle processing of prisoners
					HandlePrisonerProcessingInSector( sX, sY, bZ );

					// handle moving of equipment
					HandleEquipmentMove( sX, sY, bZ );
				}

				// Flugente: prisons can riot if there aren't enough guards around
				if ( !bZ )
					HandlePrison( sX, sY, bZ );
			}
		}
	}

	// Flugente: handle militia command
	HandleMilitiaCommand();

	// check to see if anyone is done healing?
	UpdatePatientsWhoAreDoneHealing( );


	// check if we have anyone who just finished their assignment
	if( gfAddDisplayBoxToWaitingQueue )
	{
		AddDisplayBoxToWaitingQueue( );
		gfAddDisplayBoxToWaitingQueue = FALSE;
	}


	HandleContinueOfTownTraining( );

	// check if anyone is on an assignment where they have nothing to do
	ReEvaluateEveryonesNothingToDo();

	// update mapscreen
	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
}



#ifdef JA2BETAVERSION
void VerifyTownTrainingIsPaidFor( void )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


 	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		// valid character?
		if( gCharactersList[ iCounter ].fValid == FALSE )
		{
			// nope
			continue;
		}

		pSoldier = &Menptr[ gCharactersList[ iCounter ].usSolID ];

		if( pSoldier->bActive && ( pSoldier->bAssignment == TRAIN_TOWN ) )
		{
			// make sure that sector is paid up!
			if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMilitiaTrainingPaid == FALSE )
			{
				// NOPE!	We've got a bug somewhere
				StopTimeCompression();
					// report the error
				DoScreenIndependantMessageBox( L"ERROR: Unpaid militia training. Describe *how* you're re-assigning mercs, how many/where/when! Send *prior* save!", MSG_BOX_FLAG_OK, NULL );
					// avoid repeating this
				break;
			}
		}
		if( pSoldier->bActive && ( pSoldier->bAssignment == TRAIN_MOBILE ) )
		{
			// make sure that sector is paid up!
			if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE )
			{
				// NOPE!	We've got a bug somewhere
				StopTimeCompression();
					// report the error
				DoScreenIndependantMessageBox( L"ERROR: Unpaid militia training. Describe *how* you're re-assigning mercs, how many/where/when! Send *prior* save!", MSG_BOX_FLAG_OK, NULL );
					// avoid repeating this
				break;
			}
		}
	}
}
#endif



UINT8 FindNumberInSectorWithAssignment( INT16 sX, INT16 sY, INT8 bAssignment )
{
	// run thought list of characters find number with this assignment
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;
	INT8 bNumberOfPeople = 0;

	// set psoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// go through list of characters, find all who are on this assignment
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if( ( pTeamSoldier->sSectorX == sX ) && ( pTeamSoldier->sSectorY == sY ) &&( pTeamSoldier->bAssignment == bAssignment ) )
			{
				// increment number of people who are on this assignment
				if(pTeamSoldier->bActive)
					bNumberOfPeople++;
			}
		}
	}


	return( bNumberOfPeople );
}

UINT8 GetNumberThatCanBeDoctored( SOLDIERTYPE *pDoctor, BOOLEAN fThisHour, BOOLEAN fSkipKitCheck, BOOLEAN fSkipSkillCheck, BOOLEAN fCheckForSurgery )
{
	int cnt;
	SOLDIERTYPE *pSoldier = MercPtrs[0], *pTeamSoldier = NULL;
	UINT8 ubNumberOfPeople = 0;

	AssertNotNIL(pDoctor);

	// go through list of characters, find all who are patients/doctors healable by this doctor
	for ( cnt = 0, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if( CanSoldierBeHealedByDoctor( pTeamSoldier, pDoctor, FALSE, fThisHour, fSkipKitCheck, fSkipSkillCheck, fCheckForSurgery ) == TRUE )
			{
				// increment number of doctorable patients/doctors
				ubNumberOfPeople++;
			}
		}
	}

	return( ubNumberOfPeople );
}


SOLDIERTYPE *AnyDoctorWhoCanHealThisPatient( SOLDIERTYPE *pPatient, BOOLEAN fThisHour )
{
	int cnt;
	SOLDIERTYPE *pSoldier = MercPtrs[0], *pTeamSoldier = NULL;

	AssertNotNIL(pPatient);
	AssertNotNIL(pSoldier);

	// go through list of characters, find all who are patients/doctors healable by this doctor
	for ( cnt = 0, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		// doctor?
		if( ( pTeamSoldier->bActive ) && ( pTeamSoldier->bAssignment == DOCTOR ) )
		{
			if( CanSoldierBeHealedByDoctor( pPatient, pTeamSoldier, FALSE, fThisHour, FALSE, FALSE, FALSE ) == TRUE )
			{
				// found one
				return( pTeamSoldier );
			}
		}
	}

	// there aren't any doctors, or the ones there can't do the job
	return( NULL );
}


UINT16 CalculateHealingPointsForDoctor(SOLDIERTYPE *pDoctor, UINT16 *pusMaxPts, BOOLEAN fMakeSureKitIsInHand )
{
	UINT32 usHealPts = 0;
	UINT32 usKitPts = 0;
	INT8 bMedFactor;

	INT16 sSectorModifier = 100;

	AssertNotNIL(pDoctor);
	AssertNotNIL(pusMaxPts);

	// make sure he has a medkit in his hand, and preferably make it a medical bag, not a first aid kit
	if( fMakeSureKitIsInHand )
	{
		if ( !MakeSureMedKitIsInHand( pDoctor ) )
		{
			return(0);
		}
	}

	// HEADROCK HAM 3.5: Read bonus directly from Sector Facility info
	if (pDoctor->bSectorZ == 0 &&
		GetSoldierFacilityAssignmentIndex(pDoctor) == FAC_DOCTOR )
	{
		// Read percentage modifier from the facility in question, including ambient effects.
		sSectorModifier = GetSectorModifier( pDoctor, FACILITY_PERFORMANCE_MOD );
	}

	// calculate effective doctoring rate (adjusted for drugs, alcohol, etc.)
	usHealPts = (UINT32) (( EffectiveMedical( pDoctor ) * (( EffectiveDexterity( pDoctor, FALSE ) + EffectiveWisdom( pDoctor ) ) / 2) * (100 + ( 5 * EffectiveExpLevel( pDoctor) ) ) ) / gGameExternalOptions.ubDoctoringRateDivisor);
	usHealPts = __max(0, (usHealPts * sSectorModifier)/100);

	// calculate normal doctoring rate - what it would be if his stats were "normal" (ignoring drugs, fatigue, equipment condition)
	// and equipment was not a hindrance
	*pusMaxPts = ( pDoctor->stats.bMedical * (( pDoctor->stats.bDexterity + pDoctor->stats.bWisdom ) / 2 ) * (100 + ( 5 * pDoctor->stats.bExpLevel) ) ) / gGameExternalOptions.ubDoctoringRateDivisor;
	*pusMaxPts = __max(0,*pusMaxPts);

	// SANDRO - New Doctor Trait
	if ( gGameOptions.fNewTraitSystem )
	{
		// make normal doctoring rate slower
		usHealPts = usHealPts * (100 + gSkillTraitValues.bSpeedModifierDoctoring) / 100;
		*pusMaxPts = *pusMaxPts * (100 + gSkillTraitValues.bSpeedModifierDoctoring) / 100;

		// But with doctor make it faster. 
		if (HAS_SKILL_TRAIT( pDoctor, DOCTOR_NT ))
		{
			usHealPts += usHealPts * gSkillTraitValues.usDODoctorAssignmentBonus * NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT ) / 100;
			*pusMaxPts += *pusMaxPts * gSkillTraitValues.usDODoctorAssignmentBonus * NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT ) / 100;
		}
		// penalty for aggressive people
		if ( gMercProfiles[ pDoctor->ubProfile ].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
		{	
			usHealPts -= usHealPts / 10;
			*pusMaxPts -= *pusMaxPts / 10;
		}
		// bonus for phlegmatic people
		else if ( gMercProfiles[ pDoctor->ubProfile ].bCharacterTrait == CHAR_TRAIT_PHLEGMATIC )
		{	
			usHealPts += usHealPts / 20;
			*pusMaxPts += *pusMaxPts / 20;
		}
	}

	// adjust for fatigue
	ReducePointsForFatigue( pDoctor, &usHealPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pDoctor, &usHealPts );

	// count how much medical supplies we have
	usKitPts = 100 * TotalMedicalKitPoints( pDoctor );

	// if we don't have enough medical KIT points, reduce what we can heal
	if (usKitPts < usHealPts)
	{
		usHealPts = usKitPts;
	}

	// get the type of medkit being used
	bMedFactor = IsMedicalKitItem( &( pDoctor->inv[ HANDPOS ] ) );

	if( bMedFactor != 0 )
	{
		// no med kit left?
		// if he's working with only a first aid kit, the doctoring rate is halved!
		// for simplicity, we're ignoring the situation where a nearly empty medical bag in is hand and the rest are just first aid kits
		usHealPts /= bMedFactor;
	}
	else
	{
		usHealPts = 0;
	}


	// return healing pts value
	return( usHealPts );
}


UINT8 CalculateRepairPointsForRepairman(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts, BOOLEAN fMakeSureKitIsInHand )
{
	UINT32 usRepairPts;
	UINT16 usKitPts;
	UINT8	ubKitEffectiveness;
	// HEADROCK HAM 3.5: Modifier from local facilities
	INT16 sSectorModifier = 100;

	// make sure toolkit in hand?
	if( fMakeSureKitIsInHand )
	{
		MakeSureToolKitIsInHand( pSoldier );
	}

	// can't repair at all without a toolkit
	if (!Item[pSoldier->inv[HANDPOS].usItem].toolkit	)
	{
		*pusMaxPts = 0;
		return(0);
	}

	//JMich_SkillsModifiers: We should have the best repair kit in hands, so the effectiveness is 100 + kit's effectiveness.
	ubKitEffectiveness = 100 + Item[pSoldier->inv[HANDPOS].usItem].RepairModifier;

	// calculate effective repair rate (adjusted for drugs, alcohol, etc.)
	usRepairPts = (UINT16) ((EffectiveMechanical( pSoldier ) * EffectiveDexterity( pSoldier, FALSE ) * (100 + ( 5 * EffectiveExpLevel( pSoldier) ) ) ) / ( gGameExternalOptions.ubRepairRateDivisor * gGameExternalOptions.ubAssignmentUnitsPerDay ));

	// calculate normal repair rate - what it would be if his stats were "normal" (ignoring drugs, fatigue, equipment condition)
	// and equipment was not a hindrance
	*pusMaxPts = ( pSoldier->stats.bMechanical * pSoldier->stats.bDexterity * (100 + ( 5 * pSoldier->stats.bExpLevel) ) ) / ( gGameExternalOptions.ubRepairRateDivisor * gGameExternalOptions.ubAssignmentUnitsPerDay );

	// SANDRO - Technician trait gives a good bonus to repair items
	if ( gGameOptions.fNewTraitSystem )
	{
		usRepairPts = usRepairPts * (100 + gSkillTraitValues.bSpeedModifierRepairing) / 100;
		*pusMaxPts = *pusMaxPts * (100 + gSkillTraitValues.bSpeedModifierRepairing) / 100;

		if ( HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ) )
		{
			usRepairPts += usRepairPts * gSkillTraitValues.usTERepairSpeedBonus * (NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT )) / 100;
			*pusMaxPts += *pusMaxPts * gSkillTraitValues.usTERepairSpeedBonus * (NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT )) / 100;
		}
		// Penalty for aggressive people
		if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
		{	
			usRepairPts -= usRepairPts / 10;	// -10%
			*pusMaxPts -= *pusMaxPts / 10;
		}
		// Bonus for phlegmatic people
		else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_PHLEGMATIC )
		{	
			usRepairPts += usRepairPts / 20;	// +5%
			*pusMaxPts += *pusMaxPts / 20;
		}
	}

	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usRepairPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pSoldier, &usRepairPts );

	// figure out what shape his "equipment" is in ("coming" in JA3: Viagra - improves the "shape" your "equipment" is in)
	usKitPts = ToolKitPoints( pSoldier );

	// if kit(s) in extremely bad shape
	//JMich_SkillsModifiers: Changed this to a divisor
	if ( usKitPts < 25 )
	{
		//ubKitEffectiveness = 50;
		ubKitEffectiveness /= 2;
	}
	// if kit(s) in pretty bad shape
	else if ( usKitPts < 50 )
	{
		//ubKitEffectiveness = 75
		ubKitEffectiveness = (ubKitEffectiveness * 3) / 4;
	}
	else
	{
		//ubKitEffectiveness = 100;
	}

	if (pSoldier->bSectorZ == 0)
	{
		if (GetSoldierFacilityAssignmentIndex( pSoldier ) == FAC_REPAIR_ITEMS ||
			GetSoldierFacilityAssignmentIndex( pSoldier ) == FAC_REPAIR_VEHICLE ||
			GetSoldierFacilityAssignmentIndex( pSoldier ) == FAC_REPAIR_ROBOT)
		{
			// HEADROCK HAM 3.5: Read bonus directly from Sector Facility info
			sSectorModifier = GetSectorModifier(pSoldier, FACILITY_PERFORMANCE_MOD);
		}
	}
	usRepairPts = __max(0, (usRepairPts * sSectorModifier)/100);

	// adjust for equipment
	usRepairPts = (usRepairPts * ubKitEffectiveness) / 100;

	// return current repair pts
	return(( UINT8 )usRepairPts);
}

extern INT32 CalcThreateningEffectiveness( UINT8 ubMerc );

// Flugente: calculate interrogation value
UINT32 CalculateInterrogationValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts )
{
	UINT32 usInterrogationPoints = 0;

	// for max points we display the maximum amount of prisoners instead
	*pusMaxPts = 0;

	// no soldier (how does that happen?) or underground -> no interrogation points, as there are no underground prisons
	if ( !pSoldier || pSoldier->bSectorZ )
		return 0;

	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ] );
	
	UINT8 prisonersspecial = 0, prisonerselite = 0, prisonersregular = 0, prisonersadmin = 0;
	*pusMaxPts = GetNumberOfPrisoners( pSectorInfo, &prisonersspecial, &prisonerselite, &prisonersregular, &prisonersadmin );

	// no prisoners -> no interrogation (this should not happen)
	if ( !*pusMaxPts )
		return 0;

	usInterrogationPoints = 50 + 10 * EffectiveExpLevel( pSoldier ) + EffectiveLeadership( pSoldier );

	// adjust for threatening value
	INT32 threatenvalue = CalcThreateningEffectiveness( pSoldier->ubProfile ) * gMercProfiles[pSoldier->ubProfile].usApproachFactor[2] ;
	
	threatenvalue = (threatenvalue * (100 + pSoldier->GetBackgroundValue(BG_PERC_APPROACH_THREATEN))) / 100;
		
	usInterrogationPoints *= threatenvalue;
	
	usInterrogationPoints = (usInterrogationPoints * (100 + pSoldier->GetBackgroundValue(BG_PERC_INTERROGATION))) / 100;
	
	UINT16 performancemodifier = 100;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )][cnt].fFacilityHere)
		{
			// we determine wether this is a prison by checking for usPrisonBaseLimit
			if (gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0)
			{
				performancemodifier = gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPerformance;
				break;
			}
		}
	}

	performancemodifier = min(1000,  max(10, performancemodifier) );

	usInterrogationPoints = (usInterrogationPoints * performancemodifier) / (650000);
	
	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usInterrogationPoints );

	// return current repair pts
	return( usInterrogationPoints );
}

// Flugente: calculate prison guard value
UINT32 CalculatePrisonGuardValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts )
{
	// this is not an assignment. Simply being in the sector will allow us to be counted as guards
	UINT32 usValue = 0;	

	// for max points we display the maximum amount of prisoners instead
	*pusMaxPts = 0;

	if ( pSoldier->flags.fMercAsleep )
		return 0;

	// anv: undercover snitches don't count as guards as they don't guard in traditional sense
	if ( pSoldier->bAssignment == FACILITY_PRISON_SNITCH )
		return 0;

	usValue = 15 * EffectiveExpLevel( pSoldier ) + EffectiveLeadership( pSoldier ) / 2 + 2 * EffectiveStrength( pSoldier, FALSE);

	if (gGameOptions.fNewTraitSystem)
	{
		usValue += 25 * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) + 10 * HAS_SKILL_TRAIT( pSoldier, MELEE_NT );
	}
	else
	{
		usValue += 25 * NUM_SKILL_TRAITS( pSoldier, MARTIALARTS_OT ) + 25 * NUM_SKILL_TRAITS( pSoldier, HANDTOHAND_OT ) + 10 * HAS_SKILL_TRAIT( pSoldier, KNIFING_OT );
	}
	
	usValue = (usValue * (100 + max(0, pSoldier->GetBackgroundValue(BG_PERC_GUARD)))) / 100;
	
	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usValue );

	// return current repair pts
	return( usValue );
}

UINT32 CalculateSnitchGuardValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts )
{
	// this is an assignment
	UINT32 usValue = 0;	

	// for max points we display the maximum amount of prisoners instead
	*pusMaxPts = 0;

	if ( pSoldier->flags.fMercAsleep )
		return 0;

	// only undercover snitches count
	if ( !pSoldier->bAssignment == FACILITY_PRISON_SNITCH )
		return 0;

	usValue = 15 * EffectiveExpLevel( pSoldier ) + EffectiveLeadership( pSoldier ) / 2 + 2 * EffectiveWisdom( pSoldier );

	// snitch trait doesn't give bonus, as it's required to take assignment anyway
	if (gGameOptions.fNewTraitSystem)
	{
		usValue += 25 * NUM_SKILL_TRAITS( pSoldier, COVERT_NT ) + 10 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_NT );
	}
	else
	{
		usValue += 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
	}
		
	if( gMercProfiles[ pSoldier->ubProfile ].bDisability == DEAF )
	{
		usValue = usValue/2;
	}
	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usValue );

	// return current snitch pts
	return( usValue );
}

UINT32 CalculateAllGuardsValueInPrison( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT32 prisonguardvalue = 0;	

	// count any mercs found here, and sum up their guard values
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) && pSoldier->flags.fMercAsleep == FALSE )
		{
			UINT16 tmp;
			prisonguardvalue += CalculatePrisonGuardValue(pSoldier, &tmp );
		}
	}

	// add militia strength		
	prisonguardvalue += 100 * pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + 150 * pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + 200 * pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

	return( prisonguardvalue );
}

UINT32 CalculateAllSnitchesGuardValueInPrison( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT32 prisonguardvalue = 0;	

	// count any mercs found here, and sum up their guard values
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) && pSoldier->flags.fMercAsleep == FALSE )
		{
			UINT16 tmp;
			prisonguardvalue += CalculateSnitchGuardValue(pSoldier, &tmp );
		}
	}

	// add militia strength		
	prisonguardvalue += 100 * pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + 150 * pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + 200 * pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

	return( prisonguardvalue );
}

UINT32 CalculateAllGuardsNumberInPrison( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 numprisonguards = 0;

	// count any mercs found here
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) && pSoldier->flags.fMercAsleep == FALSE )
		{
			// anv: undercover snitches don't count as guards as they don't guard in traditional sense
			if ( !(pSoldier->bAssignment == FACILITY_PRISON_SNITCH) )
				++numprisonguards;
		}
	}

	// add militia strength		
	numprisonguards += pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

	return( numprisonguards );
}

// anv: totally not a copy of CalculateInterrogationValue
UINT32 CalculateSnitchInterrogationValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts )
{
	UINT32 usInterrogationPoints = 0;

	// for max points we display the maximum amount of prisoners instead
	*pusMaxPts = 0;

	// no soldier (how does that happen?) or underground -> no interrogation points, as there are no underground prisons
	if ( !pSoldier || pSoldier->bSectorZ )
		return 0;

	if ( !CanCharacterSnitchInPrison(pSoldier) )
		return 0;

	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ] );

	UINT8 prisonersspecial = 0, prisonerselite = 0, prisonersregular = 0, prisonersadmin = 0;
	*pusMaxPts = GetNumberOfPrisoners( pSectorInfo, &prisonersspecial, &prisonerselite, &prisonersregular, &prisonersadmin );

	// no prisoners -> no interrogation (this should not happen)
	if ( !*pusMaxPts )
		return 0;

	usInterrogationPoints = 50 + 10 * EffectiveExpLevel( pSoldier ) + EffectiveLeadership( pSoldier ) + EffectiveWisdom( pSoldier );

	// no bonuses for snitch trait, as merc has to have it to take this assignment anyway
	if (gGameOptions.fNewTraitSystem)
	{
		usInterrogationPoints += 10 * NUM_SKILL_TRAITS( pSoldier, COVERT_NT );
	}

	// adjust for friendly value
	INT32 friendlyvalue =  ( ( EffectiveLeadership( pSoldier ) + EffectiveWisdom( pSoldier ) ) /2 ) * gMercProfiles[pSoldier->ubProfile].usApproachFactor[0];

	if ( gGameOptions.fNewTraitSystem )
	{
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_ASSERTIVE )
			friendlyvalue += 30;
	}

	friendlyvalue = (friendlyvalue * (100 + pSoldier->GetBackgroundValue(BG_PERC_APPROACH_FRIENDLY))) / 100;

	usInterrogationPoints *= friendlyvalue;

	//usInterrogationPoints = (usInterrogationPoints * (100 + pSoldier->GetBackgroundValue(BG_PERC_INTERROGATION))) / 100;

	UINT16 performancemodifier = 100;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )][cnt].fFacilityHere)
		{
			// we determine wether this is a snitchable prison by checking for ubStaffLimit
			if (gFacilityTypes[cnt].AssignmentData[FAC_PRISON_SNITCH].ubStaffLimit > 0)
			{
				performancemodifier = gFacilityTypes[cnt].AssignmentData[FAC_PRISON_SNITCH].usPerformance;
				break;
			}
		}
	}

	performancemodifier = min(1000,  max(10, (UINT32)(gSkillTraitValues.fSNTPrisonSnitchInterrogationMultiplier * performancemodifier) ) );

	usInterrogationPoints = (usInterrogationPoints * performancemodifier) / (650000);

	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usInterrogationPoints );

	// return current repair pts
	return( usInterrogationPoints );
}

// anv: totally not a copy of CalculatePrisonGuardValue
UINT32 CalculateSnitchPrisonGuardValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts )
{
	UINT32 usValue = 0;	

	// for max points we display the maximum amount of prisoners instead
	*pusMaxPts = 0;

	if ( pSoldier->flags.fMercAsleep )
		return 0;

	if ( !CanCharacterSnitchInPrison(pSoldier) )
		return 0;

	usValue = ( 15 * EffectiveExpLevel( pSoldier ) + 2 * EffectiveLeadership( pSoldier ) + EffectiveWisdom( pSoldier ) );

	// no bonuses for snitch trait, as merc has to have it to take this assignment anyway
	if (gGameOptions.fNewTraitSystem)
	{
		usValue += 25 * NUM_SKILL_TRAITS( pSoldier, COVERT_NT ) + 10 * HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT );
	}
	else
	{
		usValue += 10 * HAS_SKILL_TRAIT( pSoldier, STEALTHY_OT );
	}

	usValue = (UINT32)(gSkillTraitValues.fSNTPrisonSnitchGuardStrengthMultiplier * usValue);

	// adjust for fatigue
	ReducePointsForFatigue( pSoldier, &usValue );

	// return current repair pts
	return( usValue );
}

// anv: handle prisoners exposing snitch as a snitch
BOOL HandleSnitchExposition(SOLDIERTYPE *pSoldier)
{
	UINT32 uiSuspicion = 0;	
	UINT32 uiCoverQuality = 0;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ] );
	UINT8 prisonersspecial = 0, prisonerselite = 0, prisonersregular = 0, prisonersadmin = 0;
	UINT16 numprisoners = GetNumberOfPrisoners( pSectorInfo, &prisonersspecial, &prisonerselite, &prisonersregular, &prisonersadmin );

	uiCoverQuality = ( 10 * EffectiveExpLevel( pSoldier ) + 2 * EffectiveLeadership( pSoldier ) + EffectiveWisdom( pSoldier ) ) / 4;

	uiSuspicion = 10 + prisonerselite + prisonersspecial;

	// no bonuses for snitch trait, as merc has to have it to take this assignment anyway
	if (gGameOptions.fNewTraitSystem)
	{
		uiCoverQuality += 25 * NUM_SKILL_TRAITS( pSoldier, COVERT_NT ) + 10 * HAS_SKILL_TRAIT( pSoldier, STEALTHY_NT );
	}
	else
	{
		uiCoverQuality += 10 * HAS_SKILL_TRAIT( pSoldier, STEALTHY_OT );
	}

	if( Random( (UINT32)uiSuspicion ) > Random( (UINT32)uiCoverQuality ) )
	{
		// yes, he was exposed!

		// remember that he was exposed in this prison/sector, and by how many people (so when they all are processed he can be a snitch again)
		MakeSoldierKnownAsMercInPrison( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY );

		// handle aftermath

		// check if he managed to get out
		if( EffectiveWisdom( pSoldier )/10 > PreRandom(100) ) // he noticed being exposed
		{
			ScreenMsg( FONT_GRAY2, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_FINE_WISDOM ], pSoldier->GetName() );
		}
		else if( EffectiveLeadership( pSoldier )/10 > PreRandom(100) ) // he talked his way out
		{
			ScreenMsg( FONT_GRAY2, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_FINE_LEADERSHIP ], pSoldier->GetName() );
		}
		else if( EffectiveExpLevel( pSoldier ) > PreRandom(100) ) // he avoided ambush
		{
			ScreenMsg( FONT_GRAY2, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_FINE_EXPLEVEL ], pSoldier->GetName() );
		}
		else if( CalculateAllGuardsValueInPrison( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) > PreRandom(100) ) // guards prevented assassination
		{
			ScreenMsg( FONT_GRAY2, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_FINE_GUARDS], pSoldier->GetName() );
		}
		else // no, he didn't
		{
			// calculate how long it will take guards to react
			UINT8 ubReactionTime = numprisoners / max(1, CalculateAllGuardsValueInPrison( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) );
			UINT16 usDamageTaken = 0;

			// decide prisoners' action
			switch( PreRandom(4) )
			{
				case 0:
					// drowning
					while( ubReactionTime > 0 && usDamageTaken < pSoldier->bBreathMax )
					{	
						ubReactionTime--;
						if ( gGameOptions.fNewTraitSystem )
						{
							if( ( pSoldier->stats.bStrength + NUM_SKILL_TRAITS( pSoldier, ATHLETICS_NT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
						else
						{
							if( ( pSoldier->stats.bStrength + NUM_SKILL_TRAITS( pSoldier, HANDTOHAND_OT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
					}
					// instead of normal damage take breath damage
					//pSoldier->SoldierTakeDamage( 0, 0, 0, usDamageTaken, TAKE_DAMAGE_HANDTOHAND, NOBODY, NOWHERE, 0, TRUE );
					pSoldier->bBreath = max( 0, pSoldier->bBreath - usDamageTaken );
					// he drowned?
					if( pSoldier->bBreath = 0 )
					{
						// dead
						pSoldier->SoldierTakeDamage( 0, 100, 0, 100, TAKE_DAMAGE_HANDTOHAND, NOBODY, NOWHERE, 0, TRUE );
						ScreenMsg( FONT_DKRED, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_DEAD_DROWN], pSoldier->GetName() );
					}
					else
					{
						ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_WOUNDED_DROWN], pSoldier->GetName() );
					}
					break;
				case 1:
					// beating
					while( ubReactionTime > 0 && usDamageTaken < pSoldier->bBreathMax )
					{			
						ubReactionTime--;
						if ( gGameOptions.fNewTraitSystem )
						{
							if( ( ( pSoldier->stats.bAgility + pSoldier->stats.bStrength )/2 + NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
						else
						{
							if( ( ( pSoldier->stats.bAgility + pSoldier->stats.bStrength )/2 + NUM_SKILL_TRAITS( pSoldier, MARTIALARTS_OT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
					}
					// he's dead?
					pSoldier->SoldierTakeDamage( 0, usDamageTaken, 0, usDamageTaken, TAKE_DAMAGE_HANDTOHAND, NOBODY, NOWHERE, 0, TRUE );
					if( pSoldier->stats.bLife == 0 )
					{
						ScreenMsg( FONT_DKRED, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_DEAD_BEATEN], pSoldier->GetName() );
					}
					else
					{
						ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_WOUNDED_BEATEN], pSoldier->GetName() );
					}
					break;
				case 2:
					// knifing
					while( ubReactionTime > 0 && usDamageTaken < pSoldier->stats.bLife )
					{			
						ubReactionTime--;
						if ( gGameOptions.fNewTraitSystem )
						{
							if( ( pSoldier->stats.bAgility + NUM_SKILL_TRAITS( pSoldier, MELEE_NT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
						else
						{
							if( ( pSoldier->stats.bAgility + NUM_SKILL_TRAITS( pSoldier, KNIFING_OT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
					}
					pSoldier->SoldierTakeDamage( 0, usDamageTaken, 0, usDamageTaken, TAKE_DAMAGE_BLADE, NOBODY, NOWHERE, 0, TRUE );
					// he's dead?
					if( pSoldier->stats.bLife == 0 )
					{		
						ScreenMsg( FONT_DKRED, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_DEAD_KNIFED], pSoldier->GetName() );
					}
					else
					{
						ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_WOUNDED_KNIFED], pSoldier->GetName() );
					}
					break;
				case 3:
					// strangulation
					while( ubReactionTime > 0 && usDamageTaken < pSoldier->bBreathMax )
					{			
						ubReactionTime--;
						if ( gGameOptions.fNewTraitSystem )
						{
							if( ( pSoldier->stats.bStrength + NUM_SKILL_TRAITS( pSoldier, BODYBUILDING_NT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
						else
						{
							if( ( pSoldier->stats.bStrength + NUM_SKILL_TRAITS( pSoldier, HANDTOHAND_OT )* 20 ) /2 < Random( 100 ) )
								usDamageTaken += Random(10);
						}
					}
					// instead of normal damage take breath damage
					//pSoldier->SoldierTakeDamage( 0, 0, 0, usDamageTaken, TAKE_DAMAGE_HANDTOHAND, NOBODY, NOWHERE, 0, TRUE );
					pSoldier->bBreath = max( 0, pSoldier->bBreath - usDamageTaken );
					// he's strangled?
					if( pSoldier->bBreath == 0 )
					{
						// dead
						pSoldier->SoldierTakeDamage( 0, 100, 0, 100, TAKE_DAMAGE_HANDTOHAND, NOBODY, NOWHERE, 0, TRUE );
						ScreenMsg( FONT_DKRED, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_DEAD_STRANGLED], pSoldier->GetName() );
					}
					else
					{
						ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pSnitchPrisonExposedStrings[ SNITCH_PRISON_EXPOSED_WOUNDED_STRANGLED], pSoldier->GetName() );
					}
					break;
			}
		}	
	}
	else
	{
		// he's fine, carry on
		return( FALSE );
	}
}

void MakeSoldierKnownAsMercInPrison(SOLDIERTYPE *pSoldier, INT16 sMapX, INT16 sMapY)
{
	//SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	//UINT8 prisonersspecial = 0, prisonerselite = 0, prisonersregular = 0, prisonersadmin = 0;
	//UINT16 numprisoners = GetNumberOfPrisoners( pSectorInfo, &prisonersspecial, &prisonerselite, &prisonersregular, &prisonersadmin );
	//gMercProfiles[pSoldier->ubProfile].ubExposedInSector[ SECTOR(sMapX, sMapY)] = numprisoners;

	gMercProfiles[pSoldier->ubProfile].ubSnitchExposedCooldown += 24;
}

BOOLEAN IsSoldierKnownAsMercInSector(SOLDIERTYPE *pSoldier, INT16 sMapX, INT16 sMapY)
{
	//if( gMercProfiles[pSoldier->ubProfile].ubExposedInSector[ SECTOR(sMapX, sMapY)] )
	//{
	//	return( TRUE );
	//}
	//else
	//{
	//	return( FALSE );
	//}
	if( gMercProfiles[pSoldier->ubProfile].ubSnitchExposedCooldown > 0 )
		return( TRUE );
	else
		return( FALSE );
}

UINT16 ToolKitPoints(SOLDIERTYPE *pSoldier)
{
	UINT16 usKitpts=0;
	UINT8 ubPocket;

	// add up kit points
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (ubPocket=HANDPOS; ubPocket < NUM_INV_SLOTS; ubPocket++)
	{
		if( Item[pSoldier->inv[ ubPocket ].usItem].toolkit )
		{
			usKitpts += TotalPoints( &( pSoldier->inv[ ubPocket ] ) );
		}
	}

	return( usKitpts );
}


UINT16 TotalMedicalKitPoints(SOLDIERTYPE *pSoldier)
{
	UINT8 ubPocket;
	UINT16 usKitpts=0;

	// add up kit points of all medkits
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (ubPocket = HANDPOS; ubPocket < NUM_INV_SLOTS; ubPocket++)
	{
		// NOTE: Here, we don't care whether these are MEDICAL BAGS or FIRST AID KITS!
		if ( IsMedicalKitItem( &( pSoldier->inv[ ubPocket ] ) ) )
		{
			usKitpts += TotalPoints( &( pSoldier->inv[ ubPocket ] ) );
		}
	}

	return( usKitpts );
}


void HandleDoctorsInSector( INT16 sX, INT16 sY, INT8 bZ )
{
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	// set psoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// will handle doctor/patient relationship in sector

	// go through list of characters, find all doctors in sector
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if(pTeamSoldier->bActive)
		{
			if( ( pTeamSoldier->sSectorX == sX ) && ( pTeamSoldier->sSectorY == sY ) && ( pTeamSoldier->bSectorZ == bZ ) )
			{
				if ( ( pTeamSoldier->bAssignment == DOCTOR ) && ( pTeamSoldier->flags.fMercAsleep == FALSE ) )
				{
					MakeSureMedKitIsInHand( pTeamSoldier );
					// character is in sector, check if can doctor, if so...heal people
					if ( CanCharacterDoctor( pTeamSoldier ) && EnoughTimeOnAssignment( pTeamSoldier ) )
					{
						HealCharacters( pTeamSoldier, sX, sY, bZ );
					}
				}
				/*
				if( ( pTeamSoldier->bAssignment == DOCTOR ) && ( pTeamSoldier->flags.fMercAsleep == FALSE ) )
				{
					MakeSureMedKitIsInHand( pTeamSoldier );
				}

				// character is in sector, check if can doctor, if so...heal people
				if( CanCharacterDoctor( pTeamSoldier ) && ( pTeamSoldier->bAssignment == DOCTOR ) && ( pTeamSoldier->flags.fMercAsleep == FALSE ) && EnoughTimeOnAssignment( pTeamSoldier ) )
				{
					HealCharacters( pTeamSoldier, sX, sY, bZ );
				}
				*/
			}
		}
	}

	// total healing pts for this sector, now heal people
	return;
}


void UpdatePatientsWhoAreDoneHealing( void )
{
	INT32 cnt = 0;
	SOLDIERTYPE *pSoldier = NULL, *pTeamSoldier = NULL;
	BOOLEAN fHasDamagedStat = FALSE; // added by SANDRO

	// set as first in list
	pSoldier = MercPtrs[0];

	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		// active soldier?
		if( pTeamSoldier->bActive )
		{
			// patient who doesn't need healing or curing
			if( ( pTeamSoldier->bAssignment == PATIENT ) &&( pTeamSoldier->stats.bLife == pTeamSoldier->stats.bLifeMax ) && ( pTeamSoldier->bPoisonSum == 0 ) )
			{
				// SANDRO - added check if we can help to heal lost stats to this one
				for (UINT8 cnt = 0; cnt < NUM_DAMAGABLE_STATS; ++cnt)
				{
					if (pTeamSoldier->ubCriticalStatDamage[cnt] > 0 )
						fHasDamagedStat = TRUE;

					// Flugente: stats can also be damaged
					if ( !gGameOptions.fFoodSystem || ( gGameOptions.fFoodSystem && pSoldier->bFoodLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_NORMAL].bThreshold ) )
					{
						if ( pSoldier->usStarveDamageHealth > 0 || pSoldier->usStarveDamageStrength > 0 )
							fHasDamagedStat = TRUE;
					}
				}
				if (!fHasDamagedStat )// || !DoctorIsPresent( pTeamSoldier, TRUE ))
					AssignmentDone( pTeamSoldier, TRUE, TRUE );
			}
		}
	}
	return;
}

void HealCharacters( SOLDIERTYPE *pDoctor, INT16 sX, INT16 sY, INT8 bZ )
{
	// heal all patients in this sector
	UINT16 usAvailableHealingPts = 0;
	UINT16 usRemainingHealingPts = 0;
	UINT16 usUsedHealingPts = 0;
	UINT16 usEvenHealingAmount = 0;
	UINT16 usMax =0;
	UINT8 ubTotalNumberOfPatients = 0;
	SOLDIERTYPE *pSoldier = MercPtrs[0], *pTeamSoldier = NULL, *pWorstHurtSoldier = NULL;
	INT32 cnt = 0;
	UINT16 usOldLeftOvers = 0;


	// now find number of healable mercs in sector that are wounded
	ubTotalNumberOfPatients = GetNumberThatCanBeDoctored( pDoctor, HEALABLE_THIS_HOUR, FALSE, FALSE, FALSE );

	// if there is anybody who can be healed right now
	if( ubTotalNumberOfPatients > 0 )
	{
		// get available healing pts
		usAvailableHealingPts = CalculateHealingPointsForDoctor( pDoctor, &usMax, TRUE );
		usRemainingHealingPts = usAvailableHealingPts;

		// find how many healing points can be evenly distributed to each wounded, healable merc
		usEvenHealingAmount = usRemainingHealingPts / ubTotalNumberOfPatients;


		// heal each of the healable mercs by this equal amount
		for ( cnt = 0, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
		{
			if( pTeamSoldier->bActive )
			{
				if( CanSoldierBeHealedByDoctor( pTeamSoldier, pDoctor, FALSE, HEALABLE_THIS_HOUR, FALSE, FALSE, FALSE ) == TRUE )
				{
					// can heal and is patient, heal them
					usRemainingHealingPts -= HealPatient( pTeamSoldier, pDoctor, usEvenHealingAmount );
				}
			}
		}


		// if we have any remaining pts
		if ( usRemainingHealingPts > 0)
		{
			// split those up based on need - lowest life patients get them first
			do
			{
				// find the worst hurt patient
				pWorstHurtSoldier = NULL;

				for ( cnt = 0, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
				{
					if( pTeamSoldier->bActive )
					{
						if( CanSoldierBeHealedByDoctor( pTeamSoldier, pDoctor, FALSE, HEALABLE_THIS_HOUR, FALSE, FALSE, FALSE ) == TRUE )
						{
							if( pWorstHurtSoldier == NULL )
							{
								pWorstHurtSoldier = pTeamSoldier;
							}
							else
							{
								// check to see if this guy is hurt worse than anyone previous?
								if( pTeamSoldier->stats.bLife < pWorstHurtSoldier->stats.bLife )
								{
									// he is now the worse hurt guy
									pWorstHurtSoldier = pTeamSoldier;
								}
							}
						}
					}
				}

				if( pWorstHurtSoldier != NULL )
				{
					// heal the worst hurt guy
					usOldLeftOvers = usRemainingHealingPts;
					usRemainingHealingPts -= HealPatient( pWorstHurtSoldier, pDoctor, usRemainingHealingPts );

					// couldn't expend any pts, leave
					if( usRemainingHealingPts == usOldLeftOvers )
					{
						usRemainingHealingPts = 0;
					}
				}
			} while( ( usRemainingHealingPts > 0 ) && ( pWorstHurtSoldier != NULL ) );
		}


		usUsedHealingPts = usAvailableHealingPts - usRemainingHealingPts;

		// increment skills based on healing pts used
		StatChange(pDoctor, MEDICALAMT,	(UINT16) (usUsedHealingPts / 100), FALSE);
		StatChange(pDoctor, DEXTAMT,		(UINT16) (usUsedHealingPts / 100), FALSE);
		StatChange(pDoctor, WISDOMAMT,	(UINT16) (usUsedHealingPts / 100), FALSE);
	}


	// if there's nobody else here who can EVER be helped by this doctor (regardless of whether they got healing this hour)
	if( GetNumberThatCanBeDoctored( pDoctor, HEALABLE_EVER, FALSE, FALSE, FALSE ) == 0 )
	{
		// then this doctor has done all that he can do, but let's find out why and tell player the reason

		// try again, but skip the med kit check!
		if( GetNumberThatCanBeDoctored( pDoctor, HEALABLE_EVER, TRUE, FALSE, FALSE ) > 0 )
		{
			// he could doctor somebody, but can't because he doesn't have a med kit!
			AssignmentAborted( pDoctor, NO_MORE_MED_KITS );
		}
		// try again, but skip the skill check!
		else if( GetNumberThatCanBeDoctored( pDoctor, HEALABLE_EVER, FALSE, TRUE, FALSE ) > 0 )
		{
			// he could doctor somebody, but can't because he doesn't have enough skill!
			AssignmentAborted( pDoctor, INSUF_DOCTOR_SKILL );
		}
		else
		{
			// all patients should now be healed - truly DONE!
			AssignmentDone( pDoctor, TRUE, TRUE );
		}
	}
}


/* Assignment distance limits removed.	Sep/11/98.	ARM
BOOLEAN IsSoldierCloseEnoughToADoctor( SOLDIERTYPE *pPatient )
{
	// run through all doctors in sector, if it is loaded
	// if no - one is close enough and there is a doctor assigned in sector, inform player
	BOOLEAN fDoctorInSector = FALSE;
	BOOLEAN fDoctorCloseEnough = FALSE;
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0;
	CHAR16 sString[ 128 ];

	if( ( pPatient->sSectorX != gWorldSectorX ) || ( pPatient->sSectorY != gWorldSectorY ) || ( pPatient->bSectorZ != gbWorldSectorZ ) )
	{
		// not currently loaded
		return( TRUE );
	}

	for( iCounter = 0; iCounter < CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		pSoldier = &Menptr[ iCounter ];

		if( pSoldier->bActive )
		{

			// are they two of these guys in the same sector?
			if( ( pSoldier->sSectorX == pPatient->sSectorX ) && ( pSoldier->sSectorY == pPatient->sSectorY ) && ( pSoldier->bSectorZ == pPatient->bSectorZ ) )
			{

				// is a doctor
				if( pSoldier->bAssignment == DOCTOR )
				{

					// the doctor is in the house
					fDoctorInSector = TRUE;

					// can this patient be healed by the doctor?
					if( CanSoldierBeHealedByDoctor( pPatient, pSoldier, TRUE, HEALABLE_EVER, FALSE, FALSE ) == TRUE )
					{
						// yep
						fDoctorCloseEnough = TRUE;
					}
				}
			}
		}
	}

	// there are doctors here but noone can heal this guy
	if( ( fDoctorInSector ) && ( fDoctorCloseEnough == FALSE ) )
	{
		swprintf( sString, pDoctorWarningString[ 0 ] , pPatient->GetName() );
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL);
		return( FALSE );
	}

	return( TRUE );
}
*/


BOOLEAN CanSoldierBeHealedByDoctor( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pDoctor, BOOLEAN fIgnoreAssignment, BOOLEAN fThisHour, BOOLEAN fSkipKitCheck, BOOLEAN fSkipSkillCheck, BOOLEAN fCheckForSurgery )
{
	// SANDRO - added check here, if we have damaged stat
	BOOLEAN fHealDamagedStat = FALSE;

	// must be an active guy
	if (pSoldier->bActive == FALSE)
	{
		return(FALSE);
	}

	// must be a patient or a doctor
	if( ( pSoldier->bAssignment != PATIENT ) && ( pSoldier->bAssignment != DOCTOR ) && ( fIgnoreAssignment == FALSE ) )
	{
		return(FALSE);
	}

	// if dead
	if ( pSoldier->stats.bLife == 0) 
	{
		return(FALSE);
	}

	// if we care about how long it's been, and he hasn't been on a healable assignment long enough
	if( fThisHour && ( EnoughTimeOnAssignment( pSoldier ) == FALSE ) && ( fIgnoreAssignment == FALSE ) )
	{
		return( FALSE );
	}

	// must be in the same sector
	if( ( pSoldier->sSectorX != pDoctor->sSectorX ) || ( pSoldier->sSectorY != pDoctor->sSectorY ) || ( pSoldier->bSectorZ != pDoctor->bSectorZ ) )
	{
		return(FALSE);
	}

	// can't be between sectors (possible to get here if ignoring assignment)
	if ( pSoldier->flags.fBetweenSectors )
	{
		return(FALSE);
	}

	// if doctor's skill is unsufficient to save this guy
	if ( !fSkipSkillCheck && ( pDoctor->stats.bMedical < GetMinHealingSkillNeeded( pSoldier ) ) )
	{
		return(FALSE);
	}

	if ( !fSkipKitCheck && ( FindMedKit( pDoctor ) == NO_SLOT ) )
	{
		// no medical kit to use!
		return( FALSE );
	}

	// check if having damaged stat
	if ( gGameOptions.fNewTraitSystem && NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT) > 0 && (NumberOfDamagedStats( pSoldier ) > 0))
	{
		fHealDamagedStat = TRUE;
	}
	// added check for surgery
	if ( !fHealDamagedStat && fCheckForSurgery && pSoldier->iHealableInjury < 100 ) // at least one life can be healed
	{
		// cannot be healed
		return( FALSE );
	}
	// if we have no damaged stat and don't need healing
	if (!fHealDamagedStat && (pSoldier->stats.bLife == pSoldier->stats.bLifeMax) )
	{
		// if we are poisoned
		if ( pSoldier->bPoisonSum )
		{
			// if using the new trait system, Doctor skill is needed
			if ( gGameOptions.fNewTraitSystem && ( NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT ) == 0 ) )
			{
				// we may have the medical stats, but we lack the skills!
				return( FALSE );
			}
		}
		else	// if we are not poisoned
		{
			// cannot be healed
			return( FALSE );
		}
	}

	return( TRUE );
}


UINT8 GetMinHealingSkillNeeded( SOLDIERTYPE *pPatient )
{
	// get the minimum skill to handle a character under OKLIFE

	if( pPatient->stats.bLife < OKLIFE )
	{
		// less than ok life, return skill needed
		return( gGameExternalOptions.ubBaseMedicalSkillToDealWithEmergency + ( gGameExternalOptions.ubMultiplierForDifferenceInLifeValueForEmergency * ( OKLIFE - pPatient->stats.bLife ) ) );
	}
	// if at full life, only poison needs to be removed
	else if ( pPatient->stats.bLife == pPatient->stats.bLifeMax && pPatient->bPoisonSum > 0 )
	{
		//	skill to remove poison needed
		return( gGameExternalOptions.ubPoisonBaseMedicalSkillToCure );
	}
	else
	{
		// only need some skill
		return ( 1 );
	}
}


UINT16 HealPatient( SOLDIERTYPE *pPatient, SOLDIERTYPE * pDoctor, UINT16 usHealAmount )
{
	//////////////////////////////////////////////////////////////////////////////
	// SANDRO - this whole procedure was heavily changed
	////////////////////////////////////////////////////
	UINT16 usHealingPtsLeft = 0;
	UINT16 usTotalHundredthsUsed = 0;
	UINT16 usHundredthsHealed = 0;
	INT16 sPointsToUse = 0;
	INT8 bPointsHealed = 0;
	INT8 bMedFactor = 1;	// basic medical factor
	BOOLEAN fWillRepiarStats = FALSE;
	BOOLEAN fWillHealLife = TRUE;	
	UINT16 usTotalMedPoints = 0;
	UINT16 ubReturnDamagedStatRate = 0;

	// usPointsLeftToCurePoison will measure how much Hundreths will be left to cure poison
	INT16 usPointsLeftToCurePoison = usHealAmount;

	// Look how much life do we need to heal
	sPointsToUse = ( pPatient->stats.bLifeMax - pPatient->stats.bLife );
	if ( sPointsToUse <= 0 )
		fWillHealLife = FALSE;
	else if ( pPatient->stats.bLife < OKLIFE )
		sPointsToUse += ((OKLIFE - pPatient->stats.bLife )* gGameExternalOptions.ubPointCostPerHealthBelowOkLife);

	// Look how much stats do we need to repair
	if ( gGameOptions.fNewTraitSystem && ( NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT ) > 0 ) && (NumberOfDamagedStats( pPatient ) > 0) )
	{
		fWillRepiarStats = TRUE;
		sPointsToUse += NumberOfDamagedStats( pPatient );

		ubReturnDamagedStatRate = ((gSkillTraitValues.usDORepairStatsRateBasic + gSkillTraitValues.usDORepairStatsRateOnTop * NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT )));
		// reduce rate if we are going to heal at the same time
		if ( fWillHealLife )
			ubReturnDamagedStatRate -= ((ubReturnDamagedStatRate * gSkillTraitValues.ubDORepStPenaltyIfAlsoHealing ) / 100);
	}

	// Start
	if ( sPointsToUse > 0 && ( fWillHealLife || fWillRepiarStats ))
	{
		// here is our maximum, we can heal this time
		usHealingPtsLeft = max( 1, ((pPatient->sFractLife + usHealAmount) / 100));

		// if guy is hurt more than points we have...heal only what we have
		if( sPointsToUse > usHealingPtsLeft )
		{
			sPointsToUse = ( INT8 )usHealingPtsLeft;
		}

		// if we will heal life and stats at the same time, increases the medical cost
		if (fWillRepiarStats && fWillHealLife)
		{
			bMedFactor += 1;
		}
		// Added a penalty for not experienced mercs, they consume the bag faster
		if ( gGameOptions.fNewTraitSystem && !HAS_SKILL_TRAIT( pDoctor, DOCTOR_NT ) && (pDoctor->stats.bMedical < 50) )
		{
			bMedFactor += 1;
		}
		/*if ( pPatient->stats.bLife < OKLIFE )
		{
			bMedFactor += 1;			
		}*/

		// calculate how much total points we have in all medical bags
		usTotalMedPoints = TotalMedicalKitPoints(pDoctor);

		// if having enough, no problem
		if (usTotalMedPoints >= (sPointsToUse * bMedFactor))
		{
			usTotalHundredthsUsed = sPointsToUse * 100;
			usTotalMedPoints = (sPointsToUse * bMedFactor);
		}
		else
		{
			// only heal what we have
			usTotalHundredthsUsed = (usTotalMedPoints * 100 / bMedFactor) ;
		}

		// calculate points for healing life (it has priority)
		if ( fWillHealLife )
		{
			// if we haven't accumulated any full points yet
			if ((pPatient->sFractLife + usTotalHundredthsUsed) < 100)
			{
				pPatient->sFractLife += usTotalHundredthsUsed;
				fWillHealLife = FALSE;
				usHundredthsHealed = usTotalHundredthsUsed;
			}
			// if we would heal more than we need
			else if ( !fWillRepiarStats && (pPatient->stats.bLifeMax < pPatient->stats.bLife + bPointsHealed) )
			{
				usHundredthsHealed = max( 1, (usHealAmount - usTotalHundredthsUsed));
			}
			else
			{
				usHundredthsHealed = usHealAmount;
			}

			usHealAmount = max(0, usHealAmount - usHundredthsHealed);
		}

		// repair our stats here!!
		if ( fWillRepiarStats && (ubReturnDamagedStatRate > 0) ) 
		{
			// always at least one point if repairing stats only
			if ( !fWillHealLife )
				bPointsHealed = max( 1, (usTotalHundredthsUsed / 100));
			else
				bPointsHealed = (usTotalHundredthsUsed / 100);

			// reduce remaining points for upcoming healing
			if ( RegainDamagedStats( pPatient, (bPointsHealed * ubReturnDamagedStatRate) ) != 0 )
			{
				usTotalHundredthsUsed -= ((usTotalHundredthsUsed * gSkillTraitValues.ubDOHealingPenaltyIfAlsoStatRepair ) / 100);
			}
		}
		
		// if we are actually here to heal life
		if ( fWillHealLife )
		{
			pPatient->sFractLife += usTotalHundredthsUsed;

			bPointsHealed = 0;
			if (pPatient->sFractLife >= 100 && pPatient->stats.bLife >= OKLIFE)  
			{
				// convert fractions into full points
				bPointsHealed = (pPatient->sFractLife / 100);
				pPatient->sFractLife %= 100;

				pPatient->stats.bLife = min( pPatient->stats.bLifeMax, (pPatient->stats.bLife + bPointsHealed));

				// potentially increase bPoisonLife: as life points are healed, unhealed poinson points become healed poison points
				if ( pPatient->bPoisonSum > 0 )
				{
					// check if there are unhealed poison points
					if ( pPatient->bPoisonLife < pPatient->bPoisonSum ) 
					{
						pPatient->bPoisonLife = min( pPatient->bPoisonSum, (pPatient->bPoisonLife + bPointsHealed));
					}
				}
			}
			else if (((pPatient->sFractLife / gGameExternalOptions.ubPointCostPerHealthBelowOkLife) >= 100) && (pPatient->stats.bLife < OKLIFE))
			{
				bPointsHealed = ((pPatient->sFractLife / gGameExternalOptions.ubPointCostPerHealthBelowOkLife) / 100);
				pPatient->sFractLife %= 100;

				pPatient->stats.bLife = min( pPatient->stats.bLifeMax, (pPatient->stats.bLife + bPointsHealed));

				// potentially increase bPoisonLife: as life points are healed, unhealed poinson points become healed poison points
				if ( pPatient->bPoisonSum > 0 )
				{
					// check if there are unhealed poison points
					if ( pPatient->bPoisonLife < pPatient->bPoisonSum ) 
					{
						pPatient->bPoisonLife = min( pPatient->bPoisonSum, (pPatient->bPoisonLife + bPointsHealed));
					}
				}
			}
			
			// when being healed normally, reduce insta-healable HPs value 
			if ( pPatient->iHealableInjury > 0 && bPointsHealed > 0 ) 
			{
				pPatient->iHealableInjury -= (bPointsHealed * 100);
				if (pPatient->iHealableInjury < 0)
					pPatient->iHealableInjury = 0;
			}

		}

		// Finaly use all kit points (we are sure, we have that much)
		if (UseTotalMedicalKitPoints( pDoctor, usTotalMedPoints ) == FALSE )
		{
			// throw message if this went wrong for feedback on debugging
#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"Warning! UseTotalKitPOints returned false, not all points were probably used." );
#endif
		}
	}

	// As curing poison happens only when there is no more life to heal ortats to repair, we don't have to edit the above function - we simply cure afterwards
	// Look how much poison needs to be cured. Conversion of fresh life points to poison points has already taken place
	if ( pPatient->bPoisonSum > 0 || pPatient->sFractLife < 0 )
	{
		if ( fWillHealLife || fWillRepiarStats )
			usPointsLeftToCurePoison = max(0, usPointsLeftToCurePoison - usHealAmount);

		if ( usPointsLeftToCurePoison > 0 )
		{
			INT8 usPoisontoCure = pPatient->bPoisonSum;

			if ( (INT8) (usPointsLeftToCurePoison/100) < usPoisontoCure )
				usPoisontoCure = (INT8) (usPointsLeftToCurePoison/100);

			// calculate how much total points we have in all medical bags
			UINT16 usTotalMedPointsLeft = TotalMedicalKitPoints(pDoctor);

			if ( usTotalMedPointsLeft > 0 )
			{
				UINT16 usTotalHundredthsUsedToCurePoison = 0;
				// if having enough, no problem
				// gGameExternalOptions.sPoisonMedicalPtsToCureMultiplicator alters the amount needed for curing poison
				if (usTotalMedPointsLeft >= ( (UINT16)(gGameExternalOptions.sPoisonMedicalPtsToCureMultiplicator * usPoisontoCure * bMedFactor)))
				{
					usTotalHundredthsUsedToCurePoison = usPoisontoCure * 100;
					usTotalMedPointsLeft = ( (UINT16)(gGameExternalOptions.sPoisonMedicalPtsToCureMultiplicator * usPoisontoCure * bMedFactor));
				}
				else
				{
					// only heal what we have
					usTotalHundredthsUsedToCurePoison = (usTotalMedPointsLeft * 100 / bMedFactor) ;
				}

				pPatient->sFractLife += usTotalHundredthsUsedToCurePoison;
				usHundredthsHealed += (UINT16)(usTotalHundredthsUsedToCurePoison * gGameExternalOptions.sPoisonMedicalPtsToCureMultiplicator);

				if (pPatient->sFractLife >= 100)  
				{
					// convert fractions into full points
					INT8 bPoisonPointsHealed = (pPatient->sFractLife / 100);
					pPatient->sFractLife %= 100;

					pPatient->bPoisonSum = max( 0, (pPatient->bPoisonSum - bPoisonPointsHealed));
					pPatient->bPoisonLife = max( 0, (pPatient->bPoisonLife - bPoisonPointsHealed));
				}

				// Finally use all kit points (we are sure, we have that much)
				if (UseTotalMedicalKitPoints( pDoctor, usTotalMedPointsLeft ) == FALSE )
				{
					// throw message if this went wrong for feedback on debugging
#ifdef JA2TESTVERSION
					ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"Warning! UseTotalKitPOints returned false, not all points were probably used." );
#endif
				}
			}
		}
	}

	return ( usHundredthsHealed );
}


void CheckForAndHandleHospitalPatients( void )
{
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	if( fSectorsWithSoldiers[ HOSPITAL_SECTOR_X + HOSPITAL_SECTOR_Y * MAP_WORLD_X ][ 0 ] == FALSE )
	{
		// nobody in the hospital sector... leave
		return;
	}

	// set pSoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// go through list of characters, find all who are on this assignment
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if ( pTeamSoldier->bAssignment == ASSIGNMENT_HOSPITAL )
			{
				if ( ( pTeamSoldier->sSectorX == HOSPITAL_SECTOR_X ) && ( pTeamSoldier->sSectorY == HOSPITAL_SECTOR_Y ) && ( pTeamSoldier->bSectorZ == 0 ) )
				{
					// heal this character
					HealHospitalPatient( pTeamSoldier, gGameExternalOptions.ubHospitalHealingRate );
				}
			}
		}
	}
}


void HealHospitalPatient( SOLDIERTYPE *pPatient, UINT16 usHealingPtsLeft )
{
	INT8 bPointsToUse;

	if (usHealingPtsLeft <= 0)
	{
		return;
	}

/*	Stopping hospital patients' bleeding must be handled immediately, not during a regular hourly check
	// stop all bleeding of patient..for 1 pt.
	if (pPatient->bBleeding > 0)
	{
		usHealingPtsLeft--;
		pPatient->bBleeding = 0;
	}
*/

	// if below ok life, heal these first at double cost
	if( pPatient->stats.bLife < OKLIFE )
	{
		// get points needed to heal him to OKLIFE
		bPointsToUse = gGameExternalOptions.ubPointCostPerHealthBelowOkLife * ( OKLIFE - pPatient->stats.bLife );

		// if he needs more than we have, reduce to that
		if( bPointsToUse > usHealingPtsLeft )
		{
			bPointsToUse = ( INT8 )usHealingPtsLeft;
		}

		usHealingPtsLeft -= bPointsToUse;

		// heal person the amount / POINT_COST_PER_HEALTH_BELOW_OKLIFE
		pPatient->stats.bLife += ( bPointsToUse / gGameExternalOptions.ubPointCostPerHealthBelowOkLife );

		// potentially increase bPoisonLife: as life points are healed, unhealed poinson points become healed poison points
		if ( pPatient->bPoisonSum > 0 )
		{
			// check if there are unhealed poison points
			if ( pPatient->bPoisonLife < pPatient->bPoisonSum ) 
			{
				pPatient->bPoisonLife = min( pPatient->bPoisonSum, (pPatient->bPoisonLife + ( bPointsToUse / gGameExternalOptions.ubPointCostPerHealthBelowOkLife ) ) );
			}
		}
		
		// SANDRO - doctor trait - when being healed normally, reduce insta-healable HPs value 
		if ( gGameOptions.fNewTraitSystem && pPatient->iHealableInjury > 0 ) 
		{
			pPatient->iHealableInjury -= (bPointsToUse / gGameExternalOptions.ubPointCostPerHealthBelowOkLife * 100);
			if (pPatient->iHealableInjury < 0)
				pPatient->iHealableInjury = 0;
		}
	}

	// critical condition handled, now solve normal healing

	if ( pPatient->stats.bLife < pPatient->stats.bLifeMax )
	{
		bPointsToUse = ( pPatient->stats.bLifeMax - pPatient->stats.bLife );

		// if guy is hurt more than points we have...heal only what we have
		if( bPointsToUse > usHealingPtsLeft )
		{
			bPointsToUse = ( INT8 )usHealingPtsLeft;
		}

		usHealingPtsLeft -= bPointsToUse;

		// heal person the amount
		pPatient->stats.bLife += bPointsToUse;

		// potentially increase bPoisonLife: as life points are healed, unhealed poinson points become healed poison points
		if ( pPatient->bPoisonSum > 0 )
		{
			// check if there are unhealed poison points
			if ( pPatient->bPoisonLife < pPatient->bPoisonSum ) 
			{
				pPatient->bPoisonLife = min( pPatient->bPoisonSum, (pPatient->bPoisonLife + bPointsToUse) );
			}
		}
		
		// SANDRO - doctor trait - when being healed normally, reduce insta-healable HPs value 
		if ( gGameOptions.fNewTraitSystem && pPatient->iHealableInjury > 0 ) 
		{
			pPatient->iHealableInjury -= (bPointsToUse * 100);
			if (pPatient->iHealableInjury < 0)
				pPatient->iHealableInjury = 0;
		}
	}

	// are here still points left to cure poison?
	if ( usHealingPtsLeft > 0 )
	{
		UINT16 usPoisonToCure = pPatient->bPoisonSum;

		if ( usPoisonToCure > usHealingPtsLeft )
			usPoisonToCure = usHealingPtsLeft;

		usHealingPtsLeft -= usPoisonToCure;

		pPatient->bPoisonSum = max( 0, (pPatient->bPoisonSum - usPoisonToCure));
	}

	// if this patient is fully healed and cured
	if ( pPatient->stats.bLife == pPatient->stats.bLifeMax && pPatient->bPoisonSum == 0 )
	{
		AssignmentDone( pPatient, TRUE, TRUE );
	}
}


void HandleRepairmenInSector( INT16 sX, INT16 sY, INT8 bZ )
{
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	// set psoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// will handle doctor/patient relationship in sector

	// go through list of characters, find all doctors in sector
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if( ( pTeamSoldier->sSectorX == sX ) && ( pTeamSoldier->sSectorY == sY ) && ( pTeamSoldier->bSectorZ == bZ) )
			{
				if ( ( pTeamSoldier->bAssignment == REPAIR ) && ( pTeamSoldier->flags.fMercAsleep == FALSE ) )
				{
					if ( MakeSureToolKitIsInHand( pTeamSoldier ) )
					{
						// character is in sector, check if can repair
						if ( CanCharacterRepair( pTeamSoldier ) && ( EnoughTimeOnAssignment( pTeamSoldier ) ) )
						{
							HandleRepairBySoldier( pTeamSoldier );
						}
					}
					else
						// if we have no toolkit, then we cannot repair anything
						AssignmentDone( pTeamSoldier, TRUE, TRUE );
				}
			}
		}
	}

	return;
}


/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
INT8 HandleRepairOfSAMSite( SOLDIERTYPE *pSoldier, INT8 bPointsAvailable, BOOLEAN * pfNothingLeftToRepair )
{
	INT8 bPtsUsed = 0;
	INT16 sStrategicSector = 0;

	if( IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == FALSE )
	{
		return( bPtsUsed );
	}
	else if( ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->bSectorZ == gbWorldSectorZ )&&( pSoldier->sSectorY == gWorldSectorY ) )
	{
		if( CanSoldierRepairSAM( pSoldier, bPointsAvailable ) == FALSE )
		{
			return( bPtsUsed );
		}
	}

	// repair the SAM

	sStrategicSector = CALCULATE_STRATEGIC_INDEX( pSoldier->sSectorX, pSoldier->sSectorY );

	// do we have more than enough?
	if( 100 - StrategicMap[ sStrategicSector ].bSAMCondition >= bPointsAvailable / SAM_SITE_REPAIR_DIVISOR )
	{
		// no, use up all we have
		StrategicMap[ sStrategicSector ].bSAMCondition += bPointsAvailable / SAM_SITE_REPAIR_DIVISOR;
		bPtsUsed = bPointsAvailable - ( bPointsAvailable % SAM_SITE_REPAIR_DIVISOR );

		// SAM site may have been put back into working order...
		UpdateAirspaceControl( );
	}
	else
	{
		// yep
		bPtsUsed = SAM_SITE_REPAIR_DIVISOR * ( 100 - StrategicMap[ sStrategicSector ].bSAMCondition );
		StrategicMap[ sStrategicSector ].bSAMCondition = 100;

//ARM: NOTE THAT IF THIS CODE IS EVER RE-ACTIVATED, THE SAM GRAPHICS SHOULD CHANGE NOT WHEN THE SAM SITE RETURNS TO
// FULL STRENGTH (condition 100), but as soon as it reaches MIN_CONDITION_TO_FIX_SAM!!!

		// Bring Hit points back up to full, adjust graphic to full graphic.....
		UpdateSAMDoneRepair( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
	}

	if ( StrategicMap[ sStrategicSector ].bSAMCondition == 100 )
	{
		*pfNothingLeftToRepair = TRUE;
	}
	else
	{
		*pfNothingLeftToRepair = FALSE;
	}
	return( bPtsUsed );
}
*/

struct RepairItem {
	const OBJECTTYPE* item;
	const SOLDIERTYPE* owner;
	INVENTORY_SLOT inventorySlot;

	RepairItem (const OBJECTTYPE* object, const SOLDIERTYPE* soldier, INVENTORY_SLOT slot) :
		item(object), owner(soldier), inventorySlot(slot) {}
};

struct RepairPriority : std::binary_function<RepairItem, RepairItem, bool> {
	/// Comperator function
	bool operator() (const RepairItem& firstItem, const RepairItem& secondItem) const {
		UINT8 priFirst = CalculateItemPriority(firstItem),
			  priSecond = CalculateItemPriority(secondItem);

		// Both items have the same priority, prefer the item with less durability
		if (priFirst == priSecond) {
			INT16 durabilityFirst = GetMinimumStackDurability(firstItem.item),
				  durabilitySecond = GetMinimumStackDurability(secondItem.item);
		
			// Sort by durability in ascending order
			return durabilityFirst > durabilitySecond;
		}
		else
			// Sort by priority in descending order
			return priFirst < priSecond;
	}

	/// Calculate an item's priority
	///
	/// LBE: 0
	/// Items: 1
	/// Armor: 2
	/// Face items: 3
	/// Weapons: 4
	/// Equipped armor: 5
	/// Equipped weapons: 6
	private: UINT8 CalculateItemPriority(const RepairItem& object) const {
		
		UINT32 itemClass = Item[object.item->usItem].usItemClass;
		// Default priority
		UINT8 priority = 1;

		// Set base priority
		if ( itemClass == IC_LBEGEAR )
			priority = 0;
		else if ( itemClass == IC_ARMOUR )
			priority = 2;
		else if ( itemClass == IC_FACE )
			priority = 3;
		else if ( IsWeapon(object.item->usItem) )
			priority = 4;
		
		// Set priority based on equip slot
		if ((itemClass == IC_ARMOUR) &&  (object.inventorySlot == HELMETPOS || object.inventorySlot == VESTPOS || object.inventorySlot == LEGPOS))
			priority = 5;
		if ((IsWeapon(object.item->usItem)) && (object.inventorySlot == HANDPOS || object.inventorySlot == SECONDHANDPOS)) 
			priority = 6;

		// Set priority for jammed weapons; those weapons should always be highest priority
		if (IsGunJammed(object.item))
			priority = 100;

		return priority;
	}
};

static INT16 GetMinimumStackDurability(const OBJECTTYPE* pObj) {
	INT16 minDur = 100;
	for (UINT8 stackIndex = 0; stackIndex < pObj->ubNumberOfObjects; ++stackIndex) {
		INT16 durability = (*pObj)[stackIndex]->data.objectStatus;

		if (durability < minDur) 
			minDur = durability;
	}

	return minDur;
}

//CHRISL: During the repair process, we already attempt to repair the attachments on an item.  So rather then adding the attachment to the stack, we want to
//	add the main item, even if it's just the attachment that actually needs to be repaired.  Also, if multiple items in a stack are damaged, we only want to
//	include the stack once since the repair system already looks through the entire stack.
static void CollectRepairableItems(SOLDIERTYPE* pRepairSoldier, SOLDIERTYPE* pSoldier, RepairQueue& itemsToFix) {
	bool foundItem = false;
	// Iterate over all pocket slots and add items in need of repair
	for (UINT8 pocketIndex = HELMETPOS; pocketIndex < NUM_INV_SLOTS; ++pocketIndex) {
		const OBJECTTYPE* pObj = &(const_cast<SOLDIERTYPE *>(pSoldier)->inv[pocketIndex]);
		if(pObj == NULL || pObj->ubNumberOfObjects == NOTHING || pObj->usItem == NOTHING)
			continue;

		// Check if item needs repairing
		foundItem = false;
		for (UINT8 stackIndex = 0; stackIndex < pObj->ubNumberOfObjects; ++stackIndex) {
			// Check the stack item itself
			if (IsItemRepairable(pRepairSoldier, pObj->usItem, (*pObj)[stackIndex]->data.objectStatus, (*pObj)[stackIndex]->data.sRepairThreshold)) {
				RepairItem item(pObj, pSoldier, (INVENTORY_SLOT) pocketIndex);
				itemsToFix.push(item);
				break;
			}

			// Check for attachments (are there stackable items that can take attachments though?)
			UINT8 attachmentIndex = 0;
			for (attachmentList::const_iterator iter = (*pObj)[stackIndex]->attachments.begin(); iter != (*pObj)[stackIndex]->attachments.end(); ++iter, ++attachmentIndex) {
				if (IsItemRepairable(pRepairSoldier, iter->usItem, (*iter)[attachmentIndex]->data.objectStatus, (*iter)[attachmentIndex]->data.sRepairThreshold )) {
					// Send the main item, not the attachment
					RepairItem item(pObj, pSoldier, (INVENTORY_SLOT) pocketIndex);
					itemsToFix.push(item);
					foundItem = true;
					break;
				}
			}
			if(foundItem)
				break;
		}
	}
}

static BOOLEAN IsGunJammed(const OBJECTTYPE* pObj) {
	return (Item[pObj->usItem].usItemClass == IC_GUN) && ((*pObj)[0]->data.gun.bGunAmmoStatus < 0);
}

OBJECTTYPE* FindRepairableItemOnOtherSoldier( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOtherSoldier, UINT8 ubPassType )
{
	INT8 bLoop, bLoop2;
	REPAIR_PASS_SLOTS_TYPE *pPassList;
	INT8 bSlotToCheck;
	OBJECTTYPE * pObj;

	AssertLT( ubPassType, NUM_REPAIR_PASS_TYPES );
	AssertNotNIL(pOtherSoldier);

	pPassList = &( gRepairPassSlotList[ ubPassType ] );

	// CHRISL:
	for ( bLoop = 0; bLoop < pPassList->ubChoices[UsingNewInventorySystem()]; bLoop++ )
	{
		bSlotToCheck = pPassList->bSlot[ bLoop ];
		AssertNE( bSlotToCheck, -1 );

		for ( bLoop2 = 0; bLoop2 < pOtherSoldier->inv[ bSlotToCheck ].ubNumberOfObjects; bLoop2++ )
		{
			pObj = FindRepairableItemInSpecificPocket(pSoldier, &( pOtherSoldier->inv[ bSlotToCheck ] ), bLoop2);
			if(pObj != 0)
			{
				return( pObj );
			}
			//CHRISL: In NewInv, we should also repair items stored in LBENODE items
			if(UsingNewInventorySystem() == true && Item[pOtherSoldier->inv[ bSlotToCheck ].usItem].usItemClass == IC_LBEGEAR)
			{
				pObj = FindRepairableItemInLBENODE(pSoldier, &pOtherSoldier->inv[ bSlotToCheck ], bLoop2);
				if(pObj != 0)
				{
					return( pObj );
				}
			}
		}
	}

	return( 0 );
}

OBJECTTYPE* FindRepairableItemInLBENODE(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, UINT8 subObject)
{
	OBJECTTYPE * pObject;

	AssertNotNIL(pObj);

	if(UsingNewInventorySystem() == false)
		return( 0 );

	if(pObj->IsActiveLBE(subObject) == true)
	{
		LBENODE* pLBE = pObj->GetLBEPointer(subObject);
		UINT8 invsize = pLBE->inv.size();
		for(UINT8 lbePocket = 0; lbePocket < invsize; ++lbePocket)
		{
			for(UINT8 ubItemsInPocket = 0; ubItemsInPocket < pLBE->inv[lbePocket].ubNumberOfObjects; ubItemsInPocket++)
			{
				pObject = FindRepairableItemInSpecificPocket(pSoldier, &pLBE->inv[lbePocket], ubItemsInPocket);
				if(pObject != 0)
				{
					return( pObject );
				}
				if(Item[pLBE->inv[lbePocket].usItem].usItemClass == IC_LBEGEAR)
				{
					pObject = FindRepairableItemInLBENODE(pSoldier, &pLBE->inv[lbePocket], ubItemsInPocket);
					if(pObject != 0)
					{
						return( pObject );
					}
				}
			}
		}
	}
	return( 0 );
}

OBJECTTYPE* FindRepairableItemInSpecificPocket(SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, UINT8 subObject)
{
	AssertNotNIL(pObj);
	if ( IsItemRepairable( pSoldier, pObj->usItem, (*pObj)[subObject]->data.objectStatus, (*pObj)[subObject]->data.sRepairThreshold ) )
	{
		return( pObj );
	}

	// have to check for attachments after...
	for (attachmentList::iterator iter = (*pObj)[subObject]->attachments.begin(); iter != (*pObj)[subObject]->attachments.end(); ++iter) {
		// if it's repairable and NEEDS repairing
		if ( IsItemRepairable( pSoldier, iter->usItem, (*iter)[subObject]->data.objectStatus, (*iter)[subObject]->data.sRepairThreshold ) && iter->exists() ) {
			return( &(*iter) );
		}
	}

	return( 0 );
}

// Flugente: changed this function so that it repairs items up to a variable threshold instead of always 100%. This will only happen if the option gGameExternalOptions.fAdvRepairSystem is used
void DoActualRepair( SOLDIERTYPE * pSoldier, UINT16 usItem, INT16 * pbStatus, INT16 sThreshold, UINT8 * pubRepairPtsLeft )
{
	INT16		sRepairCostAdj;
	UINT16	usDamagePts, usPtsFixed;
	
	AssertNotNIL (pSoldier);
	AssertNotNIL (pbStatus);
	AssertNotNIL (pubRepairPtsLeft);

	// get item's repair ease, for each + point is 10% easier, each - point is 10% harder to repair
	sRepairCostAdj = sThreshold - ( 10 * Item[ usItem ].bRepairEase );

	// make sure it ain't somehow gone too low!
	if (sRepairCostAdj < 10)
	{
		sRepairCostAdj = 10;
	}

	// repairs on electronic items take twice as long if the guy doesn't have the skill
	// Technician/Electronic traits - repairing electronic items - SANDRO
	if ( Item[ usItem ].electronic )
	{
		if (gGameOptions.fNewTraitSystem)
		{
			if (HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
				sRepairCostAdj += (150 * max( 0, ((100 - gSkillTraitValues.ubTERepairElectronicsPenaltyReduction * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT ))/100)));
			else 
				sRepairCostAdj += 150; 
		}
		else if ( !HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT ) )
			sRepairCostAdj *= 2; // +100% cost
	}

	// how many points of damage is the item down by?
	usDamagePts = sThreshold - *pbStatus;

	// adjust that by the repair cost adjustment percentage
	usDamagePts = (usDamagePts * sRepairCostAdj) / 100;

	// do we have enough pts to fully repair the item?
	if ( *pubRepairPtsLeft >= usDamagePts )
	{
		// fix it up to the threshold (max 100%)
		*pbStatus = sThreshold;
		*pubRepairPtsLeft -= usDamagePts;
	}
	else	// not enough, partial fix only, if any at all
	{
		// fix what we can - add pts left adjusted by the repair cost
		usPtsFixed = ( *pubRepairPtsLeft * 100 ) / sRepairCostAdj;

		// if we have enough to actually fix anything
		// NOTE: a really crappy repairman with only 1 pt/hr CAN'T repair electronics or difficult items!
		if (usPtsFixed > 0)
		{
			*pbStatus += usPtsFixed;

			// make sure we don't somehow end up over the threshold
			if ( *pbStatus > sThreshold )
			{
				*pbStatus = sThreshold;
			}
		}

		*pubRepairPtsLeft = 0;
	}
}



BOOLEAN RepairObject( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pOwner, OBJECTTYPE * pObj, UINT8 * pubRepairPtsLeft )
{
	UINT8	ubLoop, ubItemsInPocket, lbeLoop, ubBeforeRepair; // added by SANDRO
	BOOLEAN fSomethingWasRepaired = FALSE;

	ubItemsInPocket = pObj->ubNumberOfObjects;

	for ( ubLoop = 0; ubLoop < ubItemsInPocket; ubLoop++ )
	{
		// Flugente: if using the new advanced repair system, we can only repair up to the repair threshold
		INT16 threshold = 100;
		if ( gGameExternalOptions.fAdvRepairSystem && (Item[pObj->usItem].usItemClass & (IC_WEAPON|IC_ARMOUR)) )
		{
			if ( !gGameExternalOptions.fMercsCanDoAdvancedRepairs || !HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ) )
			{
				threshold = (*pObj)[ubLoop]->data.sRepairThreshold;
			}
		}

		// if it's repairable and NEEDS repairing
		if ( IsItemRepairable( pSoldier, pObj->usItem, (*pObj)[ubLoop]->data.objectStatus, threshold ) )
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records, num items repaired
			// Actually we check if we repaired at least 5% of status, otherwise the item is not considered broken
			ubBeforeRepair = (UINT8)((*pObj)[ubLoop]->data.objectStatus);
						
			// repairable, try to repair it
			DoActualRepair( pSoldier, pObj->usItem, &((*pObj)[ubLoop]->data.objectStatus), threshold, pubRepairPtsLeft );

			if ( gGameExternalOptions.fAdvRepairSystem && gGameExternalOptions.fMercsCanDoAdvancedRepairs && ( HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ) ) && ( (Item[pObj->usItem].usItemClass & (IC_WEAPON|IC_ARMOUR)) ) )
				(*pObj)[ubLoop]->data.sRepairThreshold = max((*pObj)[ubLoop]->data.sRepairThreshold, (*pObj)[ubLoop]->data.objectStatus);
									
			// if the item was repaired to full status and the repair wa at least 5%, add a point
			if ( (*pObj)[ubLoop]->data.objectStatus == threshold && (((*pObj)[ubLoop]->data.objectStatus - ubBeforeRepair) > 4 ))
			{
				gMercProfiles[ pSoldier->ubProfile ].records.usItemsRepaired++;
			}
			// if the item was now repaired to a status of 96-99 and the repair was at least 2%, add a point, consider the item repaired (no points will be awarded for it anyway)
			else if ( (*pObj)[ubLoop]->data.objectStatus > threshold - 5 && (*pObj)[ubLoop]->data.objectStatus < threshold && ((*pObj)[ubLoop]->data.objectStatus - ubBeforeRepair) > 1) 
			{
				gMercProfiles[ pSoldier->ubProfile ].records.usItemsRepaired++;
			}

			if ( (*pObj)[ubLoop]->data.objectStatus == threshold )
			{
				// if item was fully repaired, consider it cleaned
				if ( gGameExternalOptions.fDirtSystem && gGameExternalOptions.fFullRepairCleansGun )
					(*pObj)[ubLoop]->data.bDirtLevel = 0.0f;
			}
			// note: this system is bad if we can repair only 1% per hour (which is rather we are total losers)
			///////////////////////////////////////////////////////////////////////////////////////////////////////

			fSomethingWasRepaired = true;

			if ( (*pObj)[ubLoop]->data.objectStatus == 100 )
			{
				// report it as fixed
				if ( pSoldier == pOwner )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_REPAIRED ], pSoldier->GetName(), ItemNames[ pObj->usItem ] );
				}
				else
				{
					// NOTE: may need to be changed for localized versions
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 35 ], pSoldier->GetName(), pOwner->GetName(), ItemNames[ pObj->usItem ] );
				}
			}
			// Flugente: if we repaired as much as possible, but the threshold is below 100, display a slightly different message
			else if ( (*pObj)[ubLoop]->data.objectStatus == threshold )
			{
				// report it as fixed
				if ( pSoldier == pOwner )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_REPAIRED_PARTIAL ], pSoldier->GetName(), ItemNames[ pObj->usItem ] );
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[ STR_REPAIRED_PARTIAL_FOR_OWNER ], pSoldier->GetName(), pOwner->GetName(), ItemNames[ pObj->usItem ] );
				}
			}

			if ( *pubRepairPtsLeft == 0 )
			{
				// we're out of points!
				return true;
			}
		}

		// now check for attachments after
		for (attachmentList::iterator iter = (*pObj)[ubLoop]->attachments.begin(); iter != (*pObj)[ubLoop]->attachments.end(); ++iter) 
		{
			if (iter->exists() && RepairObject(pSoldier, pOwner, &(*iter), pubRepairPtsLeft)) 
			{
				fSomethingWasRepaired = true;
				if ( *pubRepairPtsLeft == 0 )
				{
					// we're out of points!
					return true;
				}
			}
		}

		//CHRISL: Now check and see if this is an LBENODE with items that need repairing
		if(UsingNewInventorySystem() == true && Item[pObj->usItem].usItemClass == IC_LBEGEAR && pObj->IsActiveLBE(ubLoop) == true)
		{
			LBENODE* pLBE = pObj->GetLBEPointer(ubLoop);
			UINT8 invsize = pLBE->inv.size();
			for(lbeLoop = 0; lbeLoop < invsize; ++lbeLoop)
			{
				if(RepairObject(pSoldier, pOwner, &pLBE->inv[lbeLoop], pubRepairPtsLeft))
				{
					fSomethingWasRepaired = true;
					if ( *pubRepairPtsLeft == 0 )
					{
						// we're out of points!
						return true;
					}
				}
			}
		}
	}

	return( fSomethingWasRepaired );
}


void HandleRepairBySoldier( SOLDIERTYPE *pSoldier )
{
	UINT16 usMax=0;
	UINT8 ubRepairPtsLeft =0;
	UINT8 ubInitialRepairPts = 0;
	UINT8 ubRepairPtsUsed = 0;
	BOOLEAN fNothingLeftToRepair = FALSE;
	UINT16 usKitDegrade = 100;

	// grab max number of repair pts open to this soldier
	ubRepairPtsLeft = CalculateRepairPointsForRepairman( pSoldier, &usMax, TRUE );

	// no points
	if ( ubRepairPtsLeft == 0 )
	{
		AssignmentDone( pSoldier, TRUE, TRUE );
	return;
	}

	// remember what we've started off with
	ubInitialRepairPts = ubRepairPtsLeft;

	// check if we are repairing a vehicle
	if ( pSoldier->bVehicleUnderRepairID != -1 )
	{
		if ( CanCharacterRepairVehicle( pSoldier, pSoldier->bVehicleUnderRepairID ) )
		{
			// attempt to fix vehicle
			ubRepairPtsLeft -= RepairVehicle( pSoldier->bVehicleUnderRepairID, ubRepairPtsLeft, &fNothingLeftToRepair );
		}
	}
	// check if we are repairing a robot
	else if( pSoldier->flags.fFixingRobot )
	{
		if ( CanCharacterRepairRobot( pSoldier ) )
		{
			// repairing the robot is very slow & difficult

			// Check for new robot-repair system for Technicians - SANDRO
			if (gGameOptions.fNewTraitSystem)
			{
				if (HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
				{
					ubRepairPtsLeft = (ubRepairPtsLeft * (100 - (80 * ( 100 - gSkillTraitValues.ubTERepairRobotPenaltyReduction * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT ))/100))/100);
					ubInitialRepairPts = (ubInitialRepairPts * (100 - (80 * ( 100 - gSkillTraitValues.ubTERepairRobotPenaltyReduction * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT ))/100))/100);
				}
				else 
				{ 
					ubRepairPtsLeft = ((ubRepairPtsLeft * 20 )/100);
					ubInitialRepairPts = ((ubInitialRepairPts * 20 )/100);
				}
			}
			else if ( HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT ) )
			{
				ubRepairPtsLeft /= 2;
				ubInitialRepairPts /= 2;
			}
			else
			{
				// original value (just moved here)
				ubRepairPtsLeft /= 4;
				ubInitialRepairPts /= 4;
			}

			// robot
			ubRepairPtsLeft -= HandleRepairOfRobotBySoldier( pSoldier, ubRepairPtsLeft, &fNothingLeftToRepair );
		}
	}
	else
	{
		if (gGameExternalOptions.fAdditionalRepairMode) 
		{
			// 2Points: Use new repair algorithm
			// Collect all items in need of repair and assign them priorities
			RepairQueue itemsToFix;

			// silversurfer: Looks strange? It's not. This function now needs the guy that does the repairs and the one that owns the stuff. 
			CollectRepairableItems(pSoldier, pSoldier, itemsToFix);
			for(UINT8 teamIndex = gTacticalStatus.Team[gbPlayerNum].bFirstID; teamIndex < gTacticalStatus.Team[gbPlayerNum].bLastID; ++teamIndex) 
			{
				// Ignore self, mercs in other sectors, etc.
				if (CanCharacterRepairAnotherSoldiersStuff(pSoldier, MercPtrs[teamIndex]))
					// silversurfer: This function now needs the guy that does the repairs and the one that owns the stuff.
					CollectRepairableItems(pSoldier, MercPtrs[teamIndex], itemsToFix);
			}

			// Step through items, starting with the highest priority item
			while (!itemsToFix.empty() && ubRepairPtsLeft > 0) 
			{
				const RepairItem object = itemsToFix.top();
				itemsToFix.pop();

				// Jammed gun; call unjam function first
				if ( IsGunJammed(object.item) )
					UnjamGunsOnSoldier(const_cast<SOLDIERTYPE*> (object.owner), pSoldier, &ubRepairPtsLeft);

				// Regular repair function
				BOOLEAN itemRepaired = RepairObject( pSoldier, const_cast<SOLDIERTYPE*> (object.owner), const_cast<OBJECTTYPE*> (object.item), &ubRepairPtsLeft );

#ifdef _DEBUG
				if (itemRepaired)
					ScreenMsg(FONT_ORANGE, MSG_BETAVERSION, L"Repaired: %s's %s in item slot %d [Dur: %d]. %d points left.", 
						object.owner->name, Item[object.item->usItem].szItemName, object.inventorySlot, GetMinimumStackDurability(object.item), ubRepairPtsLeft);
#endif

				// The following assumes that weapon/armor has higher priority than regular items! If the priorities are changed, this notification
				// probably won't work reliably anymore.

				// The item has been repaired completely
				if (GetMinimumStackDurability(object.item) == 100) 
				{
					// No items left in queue: All items have been repaired
					if (itemsToFix.empty())
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sRepairsDoneString[ 3 ], pSoldier->GetName() );
					else {
						// The current item was a weapon/armor
						if ( (IsWeapon(object.item->usItem) || Item[object.item->usItem].usItemClass == IC_ARMOUR) &&
						// ...and the next item isn't:
							 (!IsWeapon(itemsToFix.top().item->usItem) && Item[itemsToFix.top().item->usItem].usItemClass != IC_ARMOUR) ) 
						{

							// All weapons & armor have been repaired
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sRepairsDoneString[ 1 ], pSoldier->GetName() );
							StopTimeCompression();
						}
					}
				}
			}
		}
		else 
		{
			// Old repair algorithm

			INT8 bPocket =0;
 			BOOLEAN fNothingLeftToRepair = FALSE;
			INT8	bLoop, bLoopStart, bLoopEnd;
			OBJECTTYPE * pObj;

			BOOLEAN fAnyOfSoldiersOwnItemsWereFixed = UnjamGunsOnSoldier( pSoldier, pSoldier, &ubRepairPtsLeft );

			// repair items on self
			// HEADROCK HAM B2.8: Experimental feature: Fixes LBEs last, as they don't actually require repairs.
			for( bLoop = 0; bLoop < 4; bLoop++ )
			{
				if ( bLoop == 0 )
				{
					bLoopStart = SECONDHANDPOS;
					// HEADROCK: New loop stage only checks second hand, to avoid LBEs.
					bLoopEnd = SECONDHANDPOS;
				}
				else if ( bLoop == 1 )
				{
					// HEADROCK: Second check is for armor and headgear only.
					bLoopStart = HELMETPOS;
					bLoopEnd = HEAD2POS;
				}
				else if ( bLoop == 2 )
				{
					// HEADROCK: Loop stage altered to run through inventory only
					bLoopStart = UsingNewInventorySystem() == false ? BIGPOCKSTART : GUNSLINGPOCKPOS;
					// CHRISL: Changed to dynamically determine max inventory locations.
					bLoopEnd = (NUM_INV_SLOTS - 1);
				}
				else if ( bLoop == 3 )
				{
					if (UsingNewInventorySystem() == true)
					{
						// HEADROCK: Last loop fixes LBEs
						bLoopStart = VESTPOCKPOS;
						bLoopEnd = BPACKPOCKPOS;
					}
					else
					{
						// HEADROCK: In OIV, simply check everything again.
						bLoopStart = SECONDHANDPOS;
						bLoopEnd = (NUM_INV_SLOTS - 1);
					}
				}

				// now repair objects running from left hand to small pocket
				for( bPocket = bLoopStart; bPocket <= bLoopEnd; bPocket++ )
				{
					//CHRISL: These two conditions allow us to repair LBE pocket items at the same time as worn armor, while
					//	still letting us repair the item in our offhand first.
					// HEADROCK HAM B2.8: No longer necessary, as I've artificially added new stages for this. LBE
					// pockets are now repaired LAST.
					//if(UsingNewInventorySystem() == true && bLoop == 0 && bPocket>SECONDHANDPOS && bPocket<GUNSLINGPOCKPOS)
					//	continue;
					//if(UsingNewInventorySystem() == true && bLoop == 1 && bPocket==SECONDHANDPOS)
					//	continue;
					pObj = &(pSoldier->inv[ bPocket ]);

					if ( RepairObject( pSoldier, pSoldier, pObj, &ubRepairPtsLeft ) )
					{
						fAnyOfSoldiersOwnItemsWereFixed = TRUE;

						// quit looking if we're already out
						if ( ubRepairPtsLeft == 0 )
							break;
					}
				}
			}

			// if he fixed something of his, and now has no more of his own items to fix
			if ( fAnyOfSoldiersOwnItemsWereFixed && !DoesCharacterHaveAnyItemsToRepair( pSoldier, -1 ) )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sRepairsDoneString[ 0 ], pSoldier->GetName() );

				// let player react
				StopTimeCompression();
			}


			// repair items on others
			RepairItemsOnOthers( pSoldier, &ubRepairPtsLeft );
		}
	}

	// what are the total amount of pts used by character?
	ubRepairPtsUsed = ubInitialRepairPts - ubRepairPtsLeft;
	if( ubRepairPtsUsed > 0 )
	{
		// improve stats
		StatChange( pSoldier, MECHANAMT, ( UINT16 ) (ubRepairPtsUsed / 2), FALSE );
		StatChange( pSoldier, DEXTAMT,	( UINT16 ) (ubRepairPtsUsed / 2), FALSE );

		// HEADROCK HAM 3.6: Facilities can change the speed of kit degrade.
		if (pSoldier->bSectorZ == 0)
		{
			usKitDegrade = GetSectorModifier( pSoldier, FACILITY_KIT_DEGRADE_MOD );
		}

		// check if kit damaged/depleted
		if( ( Random( 100 ) ) < (UINT32) __max(1,((ubRepairPtsUsed * 5 * usKitDegrade) / 100)) ) // CJC: added a x5 as this wasn't going down anywhere fast enough
		{
			// kit item damaged/depleted, burn up points of toolkit..which is in right hand
			UseKitPoints( &( pSoldier->inv[ HANDPOS ] ), 1, pSoldier );
		}
	}


	// if he really done
	if ( HasCharacterFinishedRepairing( pSoldier ) )
	{
		// yup, that's all folks
		AssignmentDone( pSoldier, TRUE, TRUE );
	}
	else	// still has stuff to repair
	{
		// Flugente: observed an instance where toolkit ran out, but assignment was not quitted, resulting in a crash on the next hour - better check here
		if ( FindToolkit( pSoldier ) == NO_SLOT )
		{
			// he could (maybe) repair something, but can't because he doesn't have a tool kit!
			AssignmentAborted( pSoldier, NO_MORE_TOOL_KITS );
		}

		// if nothing got repaired, there's a problem
		if ( ubRepairPtsUsed == 0 )
		{
			// he can't repair anything because he doesn't have enough skill!
			AssignmentAborted( pSoldier, INSUF_REPAIR_SKILL );
		}
	}

	return;
}



BOOLEAN IsItemRepairable(SOLDIERTYPE* pSoldier, UINT16 usItem, INT16 bStatus, INT16 bThreshold )
{
	// check to see if item can/needs to be repaired
//	if ( ( bStatus < 100) && ( Item[ usItem ].fFlags & ITEM_REPAIRABLE ) )
	if ( ( bStatus < 100) && ( Item[ usItem ].repairable ) )
	{
		if ( gGameExternalOptions.fAdvRepairSystem )
		{
			if ( gGameExternalOptions.fOnlyRepairGunsArmour )
			{
				if ( ((Item[usItem].usItemClass & IC_WEAPON|IC_ARMOUR) != 0) && ( bStatus < bThreshold
					|| ( gGameExternalOptions.fMercsCanDoAdvancedRepairs && HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ) )) )
					return ( TRUE );
				else
					return ( FALSE );
			}

			if ( ((Item[usItem].usItemClass & IC_WEAPON|IC_ARMOUR) != 0) && bStatus >= bThreshold 
				&& ( !gGameExternalOptions.fMercsCanDoAdvancedRepairs || !HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ) ) )
				// nay
				return ( FALSE );
		}

		// yep
		return ( TRUE );
	}

	// nope
	return ( FALSE );
}

// Not used anywhere?!
/*void HandleRestAndFatigueInSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	// this will handle all sleeping characters in this sector
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	pSoldier = MercPtrs[0];

	// go through list of characters, find all sleepers in sector
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( ( pTeamSoldier->bActive ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) )
		{
			if( ( pTeamSoldier->sSectorX == sMapX ) && ( pTeamSoldier->sSectorY == sMapY ) && ( pTeamSoldier->bSectorZ == bMapZ ) )
			{
			}
		}
	}
}*/


/*
INT8 GetRegainDueToSleepNeeded( SOLDIERTYPE *pSoldier, INT32 iRateOfReGain )
{
	// look at persons regain rate,
	// if they infact loses sleep, make sure it doesn't go below the current rate
	INT8 bRate = 0;
	UINT8 ubNeedForSleep = 0;

	// get profile id and then grab sleep need value
	ubNeedForSleep = gMercProfiles[ pSoldier->ubProfile ].ubNeedForSleep;

	bRate = ( AVG_NUMBER_OF_HOURS_OF_SLEEP_NEEDED - ( INT8 )ubNeedForSleep );

	if( bRate >= iRateOfReGain )
	{
		bRate = ( - iRateOfReGain ) + 1;
	}
	return( bRate );
}
*/



void RestCharacter( SOLDIERTYPE *pSoldier )
{
	// handle the sleep of this character, update bBreathMax based on sleep they have
	INT8 bMaxBreathRegain = 0;
	INT16 sSectorModifier = 100;
	FLOAT bDivisor = 0;

	// Determine how many hours a day this merc must sleep. Normally this would range between 6 and 12 hours.
	// Injuries and/or martial arts trait can change the limits to between 3 and 18 hours a day.
	bDivisor = CalcSoldierNeedForSleep( pSoldier );
	
	// HEADROCK HAM 3.6:
	// Night ops specialists sleep better during the day. Others sleep better during the night.
	// silversurfer: The code below did the complete opposite. A higher bDivisor means LESS regeneration. Fixed.
	if (DayTime())	//if (NightTime())
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( !HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
				bDivisor += 3;
		}
		else
			bDivisor += 4-(2*NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT ));
	}
	else
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
				bDivisor += 3;
		}
		else
			bDivisor += (2*NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT ));
	}

	// HEADROCK HAM 3.5: Read adjustment from local sector facilities
	if (pSoldier->bSectorZ == 0)
	{
		if (pSoldier->flags.fMercAsleep)
		{
			sSectorModifier = GetSectorModifier( pSoldier, FACILITY_SLEEP_MOD );
		}
		else
		{
			// Resting can be done at a facility now, and the program will automatically apply a performance bonus
			// to this if the facility has one. If the character is simply resting ("On Duty", assigned to a squad),
			// then only Ambient effects take place.
			sSectorModifier = GetSectorModifier( pSoldier, FACILITY_PERFORMANCE_MOD );
		}
		bDivisor = (bDivisor * 100) / sSectorModifier;
	}

	// silversurfer: Items can provide a bonus to regeneration, sleeping bags for example.
	// They will not provide such bonus if the merc is already using a bed in a facility.
	if ( GetSoldierFacilityAssignmentIndex( pSoldier ) != FAC_PATIENT && GetSoldierFacilityAssignmentIndex( pSoldier ) != FAC_REST )
	{
		bDivisor = ( bDivisor * 100 ) / ( 100 + GetInventorySleepModifier( pSoldier ) );
	}

	// silversurfer: I moved all modifiers above this point because we don't want anybody to rest faster or slower than the already extreme thresholds.
	// Re-enforce limits
	bDivisor = __min(18, __max(3, bDivisor));

	// round up so the bonuses above make more sense
	bMaxBreathRegain = ( 50 / bDivisor + 0.5 );
	
	// Limit so that characters can't regain faster than 3 hours, ever
	if (bMaxBreathRegain > 17)
	{
		bMaxBreathRegain = 17;
	}

	// if breath max is below the "really tired" threshold
	if( pSoldier->bBreathMax < BREATHMAX_PRETTY_TIRED )
	{
		// real tired, rest rate is 50% higher (this is to prevent absurdly long sleep times for totally exhausted mercs)
		bMaxBreathRegain = (UINT8)( bMaxBreathRegain * 3 / 2 );
	}

	pSoldier->bBreathMax += bMaxBreathRegain;


	if( pSoldier->bBreathMax > 100 )
	{
		pSoldier->bBreathMax = 100;
	}
	else if( pSoldier->bBreathMax < BREATHMAX_ABSOLUTE_MINIMUM )
	{
		pSoldier->bBreathMax = BREATHMAX_ABSOLUTE_MINIMUM;
	}

	pSoldier->bBreath = pSoldier->bBreathMax;


	if ( pSoldier->bBreathMax >= BREATHMAX_CANCEL_TIRED )
	{
		pSoldier->flags.fComplainedThatTired = FALSE;
	}

	return;
}


void FatigueCharacter( SOLDIERTYPE *pSoldier )
{
	// fatigue character
	INT32 iPercentEncumbrance;
	INT32 iBreathLoss;
	INT8 bDivisor = 1;
	INT16 sSectorModifier = 100;
	float bMaxBreathLoss = 0; // SANDRO - changed to float
	INT8 bMaxBreathTaken = 0;


	// vehicle or robot?
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return;
	}

	// check if in transit, do not wear out
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return;
	}

	// POW?
	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return;
	}

	// Determine how many hours a day this merc can operate. Normally this would range between 12 and 18 hours.
	// Injuries and/or martial arts trait can change the limits to between 6 and 20 hours a day.
	bDivisor = 24 - CalcSoldierNeedForSleep( pSoldier );
	
	// HEADROCK HAM 3.6:
	// Night ops specialists tire faster during the day. Others tire faster during the night.
	if (NightTime())
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( !HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
				bDivisor -= 3;
		}
		else
			bDivisor -= 4-(2*NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT ));
	}
	else
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - Old/New traits
		{
			if ( HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ) )
				bDivisor -= 3;
		}
		else
			bDivisor -= (2*NUM_SKILL_TRAITS( pSoldier, NIGHTOPS_OT ));
	}


	// Re-enforce limits
	bDivisor = __min(20, __max(6, bDivisor));

	bMaxBreathLoss = (float)(50 / bDivisor);

	// HEADROCK HAM 3.6: And make sure we allow no more than 18 hours of straight
	// work. (Actually, 16, but who's counting)
	if( bMaxBreathLoss < 3 )
	{
		bMaxBreathLoss = 3;
	}

	//KM: Added encumbrance calculation to soldiers moving on foot.	Anything above 100% will increase
	//	rate of fatigue.	200% encumbrance will cause soldiers to tire twice as quickly.
	if( pSoldier->flags.fBetweenSectors && pSoldier->bAssignment != VEHICLE )
	{ //Soldier is on foot and travelling.	Factor encumbrance into fatigue rate.
		iPercentEncumbrance = CalculateCarriedWeight( pSoldier );
		if( iPercentEncumbrance > 100 )
		{
			iBreathLoss = (INT32)(bMaxBreathLoss * iPercentEncumbrance / 100);
			bMaxBreathLoss = (float)min( 127, iBreathLoss );
		}
	}

	// if breath max is below the "really tired" threshold
	if( pSoldier->bBreathMax < BREATHMAX_PRETTY_TIRED )
	{
		// real tired, fatigue rate is 50% higher
		bMaxBreathLoss = ( bMaxBreathLoss * 3 / 2 );
	}

	// SANDRO - STOMP traits
	if( gGameOptions.fNewTraitSystem )
	{
		// bonus for ranger travelling between sectors
		if ( pSoldier->flags.fBetweenSectors && HAS_SKILL_TRAIT( pSoldier, RANGER_NT ) )
		{
			bMaxBreathLoss = (bMaxBreathLoss * (100 - gSkillTraitValues.ubRABreathForTravellingReduction * NUM_SKILL_TRAITS( pSoldier, RANGER_NT ) ) / 100); 
		}
		// primitive people get exhausted slower
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_PRIMITIVE )
		{	
			switch ( pSoldier->bAssignment )
			{
				case DOCTOR:
				case REPAIR:
				case TRAIN_TEAMMATE:
				case TRAIN_TOWN:
					break;
				case TRAIN_BY_OTHER:
				case TRAIN_SELF:
					switch (pSoldier->bTrainStat)
					{
						case LEADERSHIP:
						case MECHANICAL:
						case MEDICAL:
						case EXPLOSIVE_ASSIGN:
							break;
						default:
							bMaxBreathLoss = (bMaxBreathLoss * 9 / 10);
							break;
					}
					break;
				default:
					bMaxBreathLoss = (bMaxBreathLoss * 9 / 10);
					break;
			}
		}
		// Picifist actually gain morale, when on peaceful assignments
		else if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_PACIFIST )
		{
			switch ( pSoldier->bAssignment )
			{
				case DOCTOR:
				case REPAIR:
				case TRAIN_TEAMMATE:
					if ( Chance( 60 ) )
						HandleMoraleEvent( pSoldier, MORALE_PACIFIST_GAIN_NONCOMBAT, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
					break;
				case TRAIN_BY_OTHER:
				case TRAIN_SELF:
				case SNITCH_SPREAD_PROPAGANDA:
				case SNITCH_GATHER_RUMOURS:
					if ( Chance( 20 ) )
						HandleMoraleEvent( pSoldier, MORALE_PACIFIST_GAIN_NONCOMBAT, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
					break;
			}
		}
	}

	// HEADROCK HAM 3.5: Read adjustment from local sector facilities
	if (pSoldier->bSectorZ == 0)
	{
		sSectorModifier = GetSectorModifier( pSoldier, FACILITY_FATIGUE_MOD );
		bMaxBreathLoss = (bMaxBreathLoss * sSectorModifier) / 100;
	}

	/////////////////////////////////////
	// SANDRO
	if (bMaxBreathLoss <= 0 )
	{
		bMaxBreathLoss = 0;		
	}
	// if breath loss is lower than one, handle it as a chance
	else //if( bMaxBreathLoss < 1 )
	{
		bMaxBreathTaken = (INT8)(bMaxBreathLoss);
		bMaxBreathLoss = bMaxBreathLoss * 100;
		bMaxBreathLoss = (float)(((INT32)(bMaxBreathLoss)) % 100);
		if (Chance( (UINT32)(bMaxBreathLoss) ) )
			bMaxBreathTaken += 1;
	}
	pSoldier->bBreathMax -= bMaxBreathTaken;
	/////////////////////////////////////

	if( pSoldier->bBreathMax > 100 )
	{
		pSoldier->bBreathMax = 100;
	}
	else if( pSoldier->bBreathMax < BREATHMAX_ABSOLUTE_MINIMUM )
	{
		pSoldier->bBreathMax = BREATHMAX_ABSOLUTE_MINIMUM;
	}

	// current breath can't exceed maximum
	if( pSoldier->bBreath > pSoldier->bBreathMax )
	{
		pSoldier->bBreath = pSoldier->bBreathMax;
	}

	return;
}


// ONCE PER HOUR, will handle ALL kinds of training (self, teaching, and town) in this sector
void HandleTrainingInSector( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SOLDIERTYPE *pTrainer;
	SOLDIERTYPE *pStudent;
	UINT8 ubStat;
	UINT32 uiCnt=0;
	INT16 sTotalTrainingPts = 0;
	INT16 sTrainingPtsDueToInstructor = 0;
	SOLDIERTYPE *pStatTrainerList[ NUM_TRAINABLE_STATS ];		// can't have more "best" trainers than trainable stats
	INT16 sBestTrainingPts;
	INT16 sTownTrainingPts;
    // WDS - make number of mercenaries, etc. be configurable
	TOWN_TRAINER_TYPE TownTrainer[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
//	std::vector<TOWN_TRAINER_TYPE>	TownTrainer (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS);
	UINT8 ubTownTrainers;
	UINT16 usMaxPts;
	BOOLEAN fTrainingCompleted = FALSE;


	// find out if a sam site here
	BOOLEAN fSamSiteInSector = IsThisSectorASAMSector( sMapX, sMapY, 0 );

	// Training in underground sectors is disallowed by the interface code, so there should never be any
	if (bZ != 0)
	{
		return;
	}

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
	{
		// then training is canceled for this hour.
		// This is partly logical, but largely to prevent newly trained militia from appearing in mid-battle
		return;
	}

	// init trainer list
	memset( pStatTrainerList, 0, sizeof( pStatTrainerList ) );

	// build list of teammate trainers in this sector.

	// Only the trainer with the HIGHEST training ability in each stat is effective.	This is mainly to avoid having to
	// sort them from highest to lowest if some form of trainer degradation formula was to be used for multiple trainers.

	// for each trainable stat
	for (ubStat = 0; ubStat < NUM_TRAINABLE_STATS; ubStat++)
	{
		sBestTrainingPts = -1;

		// search team for active instructors in this sector
		for ( uiCnt = 0, pTrainer = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pTrainer++)
		{
			if( pTrainer->bActive && ( pTrainer->sSectorX == sMapX ) && ( pTrainer->sSectorY == sMapY ) && ( pTrainer->bSectorZ == bZ) )
			{
				// if he's training teammates in this stat
				if( ( pTrainer->bAssignment == TRAIN_TEAMMATE ) && ( pTrainer->bTrainStat == ubStat) && ( EnoughTimeOnAssignment( pTrainer ) ) && ( pTrainer->flags.fMercAsleep == FALSE ) )
				{
					sTrainingPtsDueToInstructor = GetBonusTrainingPtsDueToInstructor( pTrainer, NULL, ubStat, &usMaxPts );

					// if he's the best trainer so far for this stat
					if (sTrainingPtsDueToInstructor > sBestTrainingPts)
					{
						// then remember him as that, and the points he scored
						pStatTrainerList[ ubStat ] = pTrainer;
						sBestTrainingPts = sTrainingPtsDueToInstructor;
					}
				}
			}
		}
	}


	// now search team for active self-trainers in this sector
	for ( uiCnt = 0, pStudent = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pStudent++)
	{
		// see if this merc is active and in the same sector
		if( ( pStudent->bActive) && ( pStudent->sSectorX == sMapX ) && ( pStudent->sSectorY == sMapY ) && ( pStudent->bSectorZ == bZ ) )
		{
			// if he's training himself (alone, or by others), then he's a student
			if ( ( pStudent->bAssignment == TRAIN_SELF ) || ( pStudent->bAssignment == TRAIN_BY_OTHER ) )
			{
				if ( EnoughTimeOnAssignment( pStudent ) && ( pStudent->flags.fMercAsleep == FALSE ) )
				{
					// figure out how much the grunt can learn in one training period
					sTotalTrainingPts = GetSoldierTrainingPts( pStudent, pStudent->bTrainStat, &usMaxPts );

					// if he's getting help
					if ( pStudent->bAssignment == TRAIN_BY_OTHER )
					{
						// grab the pointer to the (potential) trainer for this stat
						pTrainer = pStatTrainerList[ pStudent->bTrainStat ];

						// if this stat HAS a trainer in sector at all
						if (pTrainer != NULL)
						{
/* Assignment distance limits removed.	Sep/11/98.	ARM
							// if this sector either ISN'T currently loaded, or it is but the trainer is close enough to the student
							if ( ( sMapX != gWorldSectorX ) || ( sMapY != gWorldSectorY ) || ( pStudent->bSectorZ != gbWorldSectorZ ) ||
									( PythSpacesAway( pStudent->sGridNo, pTrainer->sGridNo ) < MAX_DISTANCE_FOR_TRAINING ) && ( EnoughTimeOnAssignment( pTrainer ) ) )
*/
							// NB this EnoughTimeOnAssignment() call is redundent since it is called up above
							//if ( EnoughTimeOnAssignment( pTrainer ) )
							{
								// valid trainer is available, this gives the student a large training bonus!
								sTrainingPtsDueToInstructor = GetBonusTrainingPtsDueToInstructor( pTrainer, pStudent, pStudent->bTrainStat, &usMaxPts );

								StatChange(pTrainer,LDRAMT,sTrainingPtsDueToInstructor,FALSE);
								StatChange(pTrainer,WISDOMAMT,sTrainingPtsDueToInstructor,FALSE);

								// add the bonus to what merc can learn on his own
								sTotalTrainingPts += sTrainingPtsDueToInstructor;
							}
						}
					}

					// now finally train the grunt
					// HEADROCK HAM B2.8: A new trainer/student synch system allows students to rest while
					// their trainer is asleep. If this happens, the student should not train on their own!
					if (gGameExternalOptions.ubSmartTrainingRest == 0 || gGameExternalOptions.ubSmartTrainingRest == 2)
					{
						TrainSoldierWithPts( pStudent, sTotalTrainingPts );
					}
					else if ( pTrainer != NULL )
					{
						if ( pTrainer->flags.fMercAsleep == FALSE )
						{
							// This only occurs if at least one trainer is awake.
							TrainSoldierWithPts( pStudent, sTotalTrainingPts );
						}
					}
				}
			}
		}
	}


	// check if we're doing a sector where militia can be trained
	if( ( (StrategicMap[ sMapX + ( sMapY * MAP_WORLD_X ) ].bNameId != BLANK_SECTOR ) || ( fSamSiteInSector == TRUE ) ) && (bZ == 0) )
	{
		// init town trainer list
	    memset( TownTrainer, 0, sizeof( TownTrainer ) );
		ubTownTrainers = 0;

		// build list of all the town trainers in this sector and their training pts
		for ( uiCnt = 0, pTrainer = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++,pTrainer++)
		{
			if( pTrainer->bActive && ( pTrainer->sSectorX == sMapX ) && ( pTrainer->sSectorY == sMapY ) && ( pTrainer->bSectorZ == bZ ) )
			{
				// HEADROCK HAM 3.6: TRAIN_MOBILE also possible now. Handled the same way.
				if( ( pTrainer->bAssignment == TRAIN_TOWN || pTrainer->bAssignment == TRAIN_MOBILE ) && ( EnoughTimeOnAssignment( pTrainer ) )	&& ( pTrainer->flags.fMercAsleep == FALSE ) )
				{
					sTownTrainingPts = GetTownTrainPtsForCharacter( pTrainer, &usMaxPts );

					// if he's actually worth anything
					if( sTownTrainingPts > 0 )
					{
						// remember this guy as a town trainer
						TownTrainer[ubTownTrainers].sTrainingPts = sTownTrainingPts;
						TownTrainer[ubTownTrainers].pSoldier = pTrainer;
						ubTownTrainers++;
					}
				}
			}
		}


		// if we have more than one
		if (ubTownTrainers > 1)
		{
			// sort the town trainer list from best trainer to worst
			qsort( TownTrainer, ubTownTrainers, sizeof(TOWN_TRAINER_TYPE), TownTrainerQsortCompare);
		}

		// for each trainer, in sorted order from the best to the worst
		for (uiCnt = 0; uiCnt < ubTownTrainers; uiCnt++)
		{
			// top trainer has full effect (divide by 1), then divide by 2, 4, 8, etc.
			//sTownTrainingPts = TownTrainer[ uiCnt ].sTrainingPts / (UINT16) pow(2, uiCnt);
			// CJC: took this out and replaced with limit of 2 guys per sector
			sTownTrainingPts = TownTrainer[ uiCnt ].sTrainingPts;

			if (sTownTrainingPts > 0)
			{
				fTrainingCompleted = TrainTownInSector( TownTrainer[ uiCnt ].pSoldier, sMapX, sMapY, sTownTrainingPts );
				
				if ( fTrainingCompleted && !gGameExternalOptions.gfMilitiaTrainingCarryOver )
				{
					// there's no carryover into next session for extra training (cause player might cancel), so break out of loop
					break;
				}
			}
		}
	}
}


// handle radio scanning assignments
void HandleRadioScanInSector( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt=0;
	UINT8 numberofradiooperators = 0;

	// no underground scanning
	if (bZ != 0)
		return;

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
		return;

	// we will count the number of radio operators in this sector that have scanned successfully this hour. The higher this number, the higher the chance to detect enemy patrols!
	// search team for radio operators in this sector that performed this assignemnt successfully
	for ( uiCnt = 0, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pSoldier++)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) )
		{
			if( ( pSoldier->bAssignment == RADIO_SCAN ) && ( EnoughTimeOnAssignment( pSoldier ) ) && ( pSoldier->flags.fMercAsleep == FALSE ) )
			{
				++numberofradiooperators;

				// use up radio energy
				pSoldier->DepleteActiveRadioSetEnergy(FALSE, TRUE);
			}
		}
	}

	if ( !numberofradiooperators )
		return;
	
	INT8 range = gSkillTraitValues.sVOScanAssignmentBaseRange;

	UINT8 ubSectorId = SECTOR(sMapX, sMapY);
	if ( ubSectorId >= 0 && ubSectorId < 256  )
		range += SectorExternalData[ubSectorId][0].sRadioScanModifier;

	if ( range < 1 )
		return;

	UINT8 normalgroupsize = 0;
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			normalgroupsize = gGameExternalOptions.ubMinEnemyGroupSizeNovice;
			break;
		case DIF_LEVEL_MEDIUM:
			normalgroupsize = gGameExternalOptions.ubMinEnemyGroupSizeExperienced;
			break;
		case DIF_LEVEL_HARD:
			normalgroupsize = gGameExternalOptions.ubMinEnemyGroupSizeExpert;
			break;
		case DIF_LEVEL_INSANE:
			default:
			normalgroupsize = gGameExternalOptions.ubMinEnemyGroupSizeInsane;
			break;
	}

	normalgroupsize *= 2;

	FLOAT detect_basechance = 1.0f - pow(0.5f, numberofradiooperators);
	FLOAT detect_rangefactor = .0f;
	FLOAT detect_sizefactor = .0f;
	FLOAT detect_chance = .0f;
	FLOAT scandirectionmalus = .2f;
	FLOAT scanexactnumbermalus = .3f;
	UINT8 patrolsize = 0;
		
	// run through sectors and handle each type in sector
	for(INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for(INT16 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
		{
			// is this sector within range?
			FLOAT euklid_dist = (sX - sMapX)*(sX - sMapX) + (sY - sMapY)*(sY - sMapY);
			detect_rangefactor = euklid_dist / (range*range);
			//detect_rangefactor *= detect_rangefactor;

			if ( detect_rangefactor > 1.0f )
				continue;

			detect_rangefactor = 1.01f - detect_rangefactor;

			patrolsize = NumEnemiesInSector( sX, sY );

			// no or single enemies -> skip this
			if ( patrolsize < 2 )
				continue;

			detect_sizefactor = sqrt((FLOAT)(patrolsize) / (FLOAT)(normalgroupsize));

			detect_chance = detect_basechance * detect_rangefactor * detect_sizefactor;

			UINT32 scanresult = Random(100);
			if ( scanresult < 100 * detect_chance )
			{
				// enemy patrol detected
				SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_HERE;

				if ( scanresult < 100 * (detect_chance - scanexactnumbermalus) )
				{
					// our scan was very good, we even got the exact enemy numbers
					SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER;
				}

				// we roll again to decide wether we know the direction this patrol is moving in
				scanresult = Random(100);

				if ( scanresult < 100 * (detect_chance - scandirectionmalus) )
				{
					// we were succesful in deducting the direction they are moving in
					SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_KNOW_DIRECTION;
				}
			}
		}
	}

	// award experience to all radio operators
	for ( uiCnt = 0, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pSoldier++)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) )
		{
			if( ( pSoldier->bAssignment == RADIO_SCAN ) && ( EnoughTimeOnAssignment( pSoldier ) ) && ( pSoldier->flags.fMercAsleep == FALSE ) )
			{
				StatChange( pSoldier, WISDOMAMT, 5, TRUE );
				StatChange( pSoldier, EXPERAMT, 3, TRUE );
			}
		}
	}
}

// reset scan flags in all sectors
void ClearSectorScanResults()
{
	for(INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for(INT16 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
		{
			SectorInfo[ SECTOR( sX, sY ) ].uiFlags &= ~(SF_ASSIGN_NOTICED_ENEMIES_HERE|SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER);
		}
	}
}

// Flugente: handle militia command
void HandleMilitiaCommand()
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier && pSoldier->bAssignment == FACILITY_STRATEGIC_MILITIA_MOVEMENT && pSoldier->flags.fMercAsleep == FALSE )
		{
			// every commander gets a bit of leadership and wisdom
			StatChange( pSoldier, LDRAMT,		2, TRUE );
			StatChange( pSoldier, WISDOMAMT,	1, TRUE );
		}
	}
}

// handle snitch spreading propaganda assignment
// totally not a copy of HandleRadioScanInSector
void HandleSpreadingPropagandaInSector( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	SOLDIERTYPE *pSnitch = NULL;
	UINT32 uiCnt=0;
	UINT8 ubTownSnitches = 0;
	UINT32 uiPropagandaEffect = 0;

	UINT8 ubTownId = GetTownIdForSector( sMapX, sMapY );

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
		return;

	// if not a town, stop
	if ( !ubTownId )
		return;

	// we will count the number of snitches in this sector that have spread propaganda successfully this hour. The higher this number, the higher loyalty increase!
	// search team for snitches in this sector that performed this assignemnt successfully
	for ( uiCnt = 0, pSnitch = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pSnitch++)
	{
		if( ( pSnitch->bActive && pSnitch->flags.fMercAsleep == FALSE && EnoughTimeOnAssignment( pSnitch ) ) &&
			( pSnitch->bAssignment == SNITCH_SPREAD_PROPAGANDA || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA_GLOBAL ) &&
			( ( pSnitch->sSectorX == sMapX && pSnitch->sSectorY == sMapY && pSnitch->bSectorZ == bZ ) || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA_GLOBAL ) )
		{
			uiPropagandaEffect += GAIN_PTS_PER_LOYALTY_PT * 
				(  ( 50 + EffectiveLeadership(pSnitch) / 2 ) / 100.0 ) *
				(  ( 75 + EffectiveWisdom(pSnitch) / 4 ) / 100.0 ) *
				(  ( 75 + ( gMercProfiles[ pSnitch->ubProfile ].usApproachFactor[3] + pSnitch->GetBackgroundValue(BG_PERC_APPROACH_RECRUIT) ) / 4 ) / 100.0 ) ;
			if( pSnitch->sFacilityTypeOperated && // Soldier is operating facility
				GetSoldierFacilityAssignmentIndex( pSnitch ) != -1) 
			{
				UINT8 ubFacilityType = (UINT8)pSnitch->sFacilityTypeOperated;
				UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSnitch );
				uiPropagandaEffect *= ( GetFacilityModifier(FACILITY_PERFORMANCE_MOD, ubFacilityType, ubAssignmentType ) / 100.0 );
			}
		}
	}

	if ( !uiPropagandaEffect )
		return;

	IncrementTownLoyalty( ubTownId, uiPropagandaEffect );

	// award experience to all snitches
	for ( uiCnt = 0, pSnitch = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pSnitch++)
	{
		if( pSnitch->bActive && ( pSnitch->sSectorX == sMapX ) && ( pSnitch->sSectorY == sMapY ) && ( pSnitch->bSectorZ == bZ) )
		{
			if( ( pSnitch->bAssignment == SNITCH_SPREAD_PROPAGANDA ) && ( EnoughTimeOnAssignment( pSnitch ) ) && ( pSnitch->flags.fMercAsleep == FALSE ) )
			{
				StatChange( pSnitch, WISDOMAMT, 1, TRUE );
				StatChange( pSnitch, LDRAMT, 2, TRUE );
				StatChange( pSnitch, EXPERAMT, 1, TRUE );
			}
		}
	}
}

UINT32 HandlePropagandaBlockingBadNewsInTown( INT8 bTownId, UINT32 uiLoyaltyDecrease )
{
	SOLDIERTYPE *pSnitch = NULL;
	UINT32 uiCnt=0;	
	FLOAT fPropagandaEffect;
	UINT32 uiNewLoyaltyDecrease = uiLoyaltyDecrease;

	// search team for snitches in this sector that performed this assignment successfully
	for ( uiCnt = 0, pSnitch = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pSnitch++)
	{
		if( ( pSnitch->bActive && pSnitch->flags.fMercAsleep == FALSE && EnoughTimeOnAssignment( pSnitch ) ) &&
			( pSnitch->bAssignment == SNITCH_SPREAD_PROPAGANDA || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA_GLOBAL ) &&
			( GetTownIdForSector( pSnitch->sSectorX, pSnitch->sSectorY ) == bTownId || pSnitch->bAssignment == FACILITY_SPREAD_PROPAGANDA_GLOBAL ) )
		{
			fPropagandaEffect = 0.5 * 
				(  ( 50 + EffectiveLeadership(pSnitch) / 2 ) / 100.0 ) *
				(  ( 75 + EffectiveWisdom(pSnitch) / 4 ) / 100.0 ) *
				(  ( 75 + ( gMercProfiles[ pSnitch->ubProfile ].usApproachFactor[3] + pSnitch->GetBackgroundValue(BG_PERC_APPROACH_RECRUIT) ) / 4 ) / 100.0 );

			if( pSnitch->sFacilityTypeOperated && // Soldier is operating facility
				GetSoldierFacilityAssignmentIndex( pSnitch ) != -1) 
			{
				UINT8 ubFacilityType = (UINT8)pSnitch->sFacilityTypeOperated;
				UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSnitch );
				fPropagandaEffect *= ( GetFacilityModifier(FACILITY_PERFORMANCE_MOD, ubFacilityType, ubAssignmentType ) / 100.0 );
			}

			uiNewLoyaltyDecrease *= ( 1.0 - fPropagandaEffect );
		}
	}
	return uiNewLoyaltyDecrease;
}

// anv: handle snitch gathering information assignment
// totally not a copy of HandleRadioScanInSector
void HandleGatheringInformationBySoldier( SOLDIERTYPE* pSoldier )
{
	UINT32 uiCnt=0;

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ))
		return;

	if( !(pSoldier->bActive) || !EnoughTimeOnAssignment( pSoldier ) || pSoldier->flags.fMercAsleep == TRUE || pSoldier->flags.fBetweenSectors == TRUE )
	{
		if( pSoldier->bAssignment != SNITCH_GATHER_RUMOURS && pSoldier->bAssignment != FACILITY_GATHER_RUMOURS )
		{
			return;
		}
	}

	UINT16 usNormalGroupSize = 0;
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			usNormalGroupSize = gGameExternalOptions.ubMinEnemyGroupSizeNovice;
			break;
		case DIF_LEVEL_MEDIUM:
			usNormalGroupSize = gGameExternalOptions.ubMinEnemyGroupSizeExperienced;
			break;
		case DIF_LEVEL_HARD:
			usNormalGroupSize = gGameExternalOptions.ubMinEnemyGroupSizeExpert;
			break;
		case DIF_LEVEL_INSANE:
		default:
			usNormalGroupSize = gGameExternalOptions.ubMinEnemyGroupSizeInsane;
			break;
	}

	usNormalGroupSize *= 2;

	FLOAT fBaseChance = ( EffectiveLeadership(pSoldier) + EffectiveWisdom(pSoldier) + EffectiveExpLevel(pSoldier) * 10 ) / 3000.0;

	if( gMercProfiles[ pSoldier->ubProfile ].bDisability == DEAF )
	{
		fBaseChance /= 2.0;
	}

	if( pSoldier->sFacilityTypeOperated && // Soldier is operating facility
		GetSoldierFacilityAssignmentIndex( pSoldier ) != -1) 
	{
		UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
		UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );
		fBaseChance *= ( GetFacilityModifier(FACILITY_PERFORMANCE_MOD, ubFacilityType, ubAssignmentType ) / 100.0 );
	}

	FLOAT fDetectSizeFactor = .0;
	FLOAT fChance = .0;
	FLOAT fBlooperChance =  ( ( 100 - EffectiveWisdom( pSoldier ) ) / 100.0 ) * .01 + .01;

	UINT16 usPatrolSize = 0;
	UINT16 usDetectedLocations = 0;

	// run through sectors and handle each type in sector
	for(INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for(INT16 sY = 1; sY < MAP_WORLD_X - 1; ++sY )
		{
			if( SectorInfo[ SECTOR( sX, sY ) ].uiFlags & ( SF_ASSIGN_NOTICED_ENEMIES_HERE | SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER | SF_ASSIGN_NOTICED_ENEMIES_KNOW_DIRECTION ) )
			{
				// no point if we already know about enemies there
				continue;
			}
			if( SectorIsImpassable( SECTOR( sX, sY ) ) )
			{
				// only passable sectors matter
				continue;
			}
			usPatrolSize = NumEnemiesInSector( sX, sY );

			// no enemies
			if ( usPatrolSize < 1 )
			{
				if( fBlooperChance * 100 > Random(100) )
				{
					// enemy patrol detected - except it's not really there!
					SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_HERE;
					usDetectedLocations++;
				}
			}
			else
			{
				fDetectSizeFactor = sqrt((FLOAT)(usPatrolSize) / (FLOAT)(usNormalGroupSize));

				fChance = fBaseChance * fDetectSizeFactor;

				if ( fChance * 100 > Random(100) )
				{				
					// enemy patrol detected
					SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_HERE;
					usDetectedLocations++;
				}
			}
		}
	}

	if ( usDetectedLocations )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pSnitchGatheringRumoursResultStrings[SNITCH_GATHERING_RUMOURS_RESULT], pSoldier->GetName(), usDetectedLocations );
		// award experience
		StatChange( pSoldier, WISDOMAMT, 1, TRUE );
		StatChange( pSoldier, LDRAMT, 2, TRUE );
		StatChange( pSoldier, EXPERAMT, 3, TRUE );
	}

}
int TownTrainerQsortCompare(const void *pArg1, const void *pArg2)
{
	if (((TOWN_TRAINER_TYPE *)pArg1)->sTrainingPts > ((TOWN_TRAINER_TYPE *)pArg2)->sTrainingPts)
	{
	return(-1);
	}
	else
	if (((TOWN_TRAINER_TYPE *)pArg1)->sTrainingPts < ((TOWN_TRAINER_TYPE *)pArg2)->sTrainingPts)
	{
	return(1);
	}
	else
	{
	return(0);
	}
}


INT16 GetBonusTrainingPtsDueToInstructor( SOLDIERTYPE *pInstructor, SOLDIERTYPE *pStudent, INT8 bTrainStat, UINT16 *pusMaxPts )
{
	// return the bonus training pts of this instructor with this student,...if student null, simply assignment student skill of 0 and student wisdom of 100
	INT16 sTrainingPts = 0;
	INT16 bTraineeEffWisdom = 0;
	INT16 bTraineeNatWisdom = 0;
	INT16 bTraineeSkill = 0;
	INT16 bTrainerEffSkill = 0;
	INT16 bTrainerNatSkill = 0;
	INT16 bTrainingBonus = 0;
	INT8 bOpinionFactor = 0;

	// assume training impossible for max pts
	*pusMaxPts = 0;

	if( pInstructor == NULL )
	{
		// no instructor, leave
		return ( 0 );
	}


	switch( bTrainStat )
	{
		case( STRENGTH ):
			bTrainerEffSkill = EffectiveStrength ( pInstructor, TRUE );
			bTrainerNatSkill = pInstructor->stats.bStrength;
		break;
		case( DEXTERITY ):
			bTrainerEffSkill = EffectiveDexterity ( pInstructor, TRUE );
			bTrainerNatSkill = pInstructor->stats.bDexterity;
		break;
		case( AGILITY ):
			bTrainerEffSkill = EffectiveAgility( pInstructor, TRUE );
			bTrainerNatSkill = pInstructor->stats.bAgility;
		break;
		case( HEALTH ):
			bTrainerEffSkill = pInstructor->stats.bLifeMax;
			bTrainerNatSkill = pInstructor->stats.bLifeMax;
		break;
		case( LEADERSHIP ):
			bTrainerEffSkill = EffectiveLeadership( pInstructor );
			bTrainerNatSkill = pInstructor->stats.bLeadership;
		break;
		case( MARKSMANSHIP ):
			bTrainerEffSkill = EffectiveMarksmanship( pInstructor );
			bTrainerNatSkill = pInstructor->stats.bMarksmanship;
		break;
		case( EXPLOSIVE_ASSIGN ):
			bTrainerEffSkill = EffectiveExplosive( pInstructor );
			bTrainerNatSkill = pInstructor->stats.bExplosive;
		break;
		case( MEDICAL ):
			bTrainerEffSkill = EffectiveMedical( pInstructor );
			bTrainerNatSkill = pInstructor->stats.bMedical;
		break;
		case( MECHANICAL ):
			bTrainerEffSkill = EffectiveMechanical( pInstructor );
			bTrainerNatSkill = pInstructor->stats.bMechanical;
		break;
		// NOTE: Wisdom can't be trained!
		default:
			// BETA message
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"GetBonusTrainingPtsDueToInstructor: ERROR - Unknown bTrainStat %d", bTrainStat);
			#endif
			return(0);
	}


	// if there's no student
	if( pStudent == NULL )
	{
		// assume these default values
		bTraineeEffWisdom = 100;
		bTraineeNatWisdom = 100;
		bTraineeSkill = 0;
		bOpinionFactor = 0;
	}
	else
	{
		// set student's variables
		bTraineeEffWisdom = EffectiveWisdom ( pStudent );
		bTraineeNatWisdom = pStudent->stats.bWisdom;

		// for trainee's stat skill, must use the natural value, not the effective one, to avoid drunks training beyond cap
		switch( bTrainStat )
		{
			case( STRENGTH ):
				bTraineeSkill = pStudent->stats.bStrength;
			break;
			case( DEXTERITY ):
				bTraineeSkill = pStudent->stats.bDexterity;
			break;
			case( AGILITY ):
				bTraineeSkill = pStudent->stats.bAgility;
			break;
			case( HEALTH ):
				bTraineeSkill = pStudent->stats.bLifeMax;
			break;
			case( LEADERSHIP ):
				bTraineeSkill = pStudent->stats.bLeadership;
			break;
			case( MARKSMANSHIP ):
				bTraineeSkill = pStudent->stats.bMarksmanship;
			break;
			case( EXPLOSIVE_ASSIGN ):
				bTraineeSkill = pStudent->stats.bExplosive;
			break;
			case( MEDICAL ):
				bTraineeSkill = pStudent->stats.bMedical;
			break;
			case( MECHANICAL ):
				bTraineeSkill = pStudent->stats.bMechanical;
			break;
			// NOTE: Wisdom can't be trained!
			default:
				// BETA message
				#ifdef JA2BETAVERSION
					ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"GetBonusTrainingPtsDueToInstructor: ERROR - Unknown bTrainStat %d", bTrainStat);
				#endif
				return(0);
		}

		// if trainee skill 0 or at/beyond the training cap, can't train
//Orig:		if ( ( bTraineeSkill == 0 ) || ( bTraineeSkill >= TRAINING_RATING_CAP ) )
		// Madd
		if ( bTraineeSkill < gGameExternalOptions.ubTrainingSkillMin || bTraineeSkill >= gGameExternalOptions.ubTrainingSkillMax )
		{
			return 0;
		}

		// factor in their mutual relationship
		if (OKToCheckOpinion(pInstructor->ubProfile))
		bOpinionFactor	= gMercProfiles[	pStudent->ubProfile ].bMercOpinion[ pInstructor->ubProfile ];
		if (OKToCheckOpinion(pStudent->ubProfile))
		bOpinionFactor += gMercProfiles[ pInstructor->ubProfile ].bMercOpinion[	pStudent->ubProfile ] / 2;
	}


	// check to see if student better than/equal to instructor's effective skill, if so, return 0
	// don't use natural skill - if the guy's too doped up to tell what he know, student learns nothing until sobriety returns!
	/////////////////////////////////////////////////////////////////////////
	// SANDRO - Teaching Skill now increases the effective skill to determine if we can instruct other mercs
	if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pInstructor, TEACHING_NT ))
	{
		if( bTraineeSkill >= (bTrainerEffSkill + (INT16)(gSkillTraitValues.ubTGEffectiveSkillValueForTeaching)) )
			return ( 0 );
	}
	else if( bTraineeSkill >= bTrainerEffSkill )
	{
		return ( 0 );
	}

	// old/new teaching trait behaviour - SANDRO
	if( gGameOptions.fNewTraitSystem )
	{
		// SANDRO - make difference between stats min 10, so if teaching trait is in place and instructor has lesser stat than trainee, the value doesn't go negative
		// calculate effective training pts
		sTrainingPts = max( 10, ( bTrainerEffSkill - bTraineeSkill )) * ( bTraineeEffWisdom + ( EffectiveWisdom( pInstructor ) + EffectiveLeadership( pInstructor ) ) / 2 ) / gGameExternalOptions.ubInstructedTrainingDivisor;
		// calculate normal training pts - what it would be if his stats were "normal" (ignoring drugs, fatigue)
		*pusMaxPts	= max( 10, ( bTrainerNatSkill - bTraineeSkill )) * ( bTraineeNatWisdom + ( pInstructor->stats.bWisdom + pInstructor->stats.bLeadership ) / 2 ) / gGameExternalOptions.ubInstructedTrainingDivisor;

		// penalty for non-specialized mercs
		bTrainingBonus = bTrainingBonus * (100 + gSkillTraitValues.bSpeedModifierTeachingOthers) / 100;

		// check for teaching skill bonuses
		if ( HAS_SKILL_TRAIT( pInstructor, TEACHING_NT) )
		{
			bTrainingBonus += gSkillTraitValues.ubTGBonusToTeachOtherMercs;
		}
	}
	else
	{
		// calculate effective training pts
		sTrainingPts = ( bTrainerEffSkill - bTraineeSkill ) * ( bTraineeEffWisdom + ( EffectiveWisdom( pInstructor ) + EffectiveLeadership( pInstructor ) ) / 2 ) / gGameExternalOptions.ubInstructedTrainingDivisor;
		// calculate normal training pts - what it would be if his stats were "normal" (ignoring drugs, fatigue)
		*pusMaxPts	= ( bTrainerNatSkill - bTraineeSkill ) * ( bTraineeNatWisdom + ( pInstructor->stats.bWisdom + pInstructor->stats.bLeadership ) / 2 ) / gGameExternalOptions.ubInstructedTrainingDivisor;

		// put in a minimum (that can be reduced due to instructor being tired?)
		if (*pusMaxPts <= 0) // stay safe
		{
			// we know trainer is better than trainee, make sure they are at least 10 pts better
			if ( bTrainerEffSkill > bTraineeSkill + 10 )
			{
				sTrainingPts = 1;
				*pusMaxPts = 1;
			}
		}

		// check for teaching skill bonuses
		if ( HAS_SKILL_TRAIT( pInstructor, TEACHING_OT) )
		{
			bTrainingBonus += (gGameExternalOptions.ubTeachBonusToTrain * NUM_SKILL_TRAITS( pInstructor, TEACHING_OT));
		}
	}
	/////////////////////////////////////////////////////////////////////////

	// teaching bonus is counted as normal, but gun range bonus is not
	*pusMaxPts += ( ( ( bTrainingBonus + bOpinionFactor ) * *pusMaxPts ) / 100 );

	// get special bonus if we're training marksmanship and we're in the gun range sector in Alma
	// HEADROCK HAM 3.5: Now reads from XML facilities, and works for all stats.
	UINT8 bFacilityModifier = 100;
	if ( pInstructor->bSectorZ == 0 )
	{
		bFacilityModifier = (UINT8)GetSectorModifier( pInstructor, FACILITY_PERFORMANCE_MOD );
	}

	// adjust for any training bonuses and for the relationship
	sTrainingPts += ( ( ( bTrainingBonus + (bFacilityModifier-100) + bOpinionFactor ) * sTrainingPts ) / 100 );

	// adjust for instructor fatigue
	UINT32 uiTrainingPts = (UINT32) sTrainingPts;
	ReducePointsForFatigue( pInstructor, &uiTrainingPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pInstructor, &uiTrainingPts );

	sTrainingPts = (INT16)uiTrainingPts;

	return( sTrainingPts );
}


INT16 GetSoldierTrainingPts( SOLDIERTYPE *pSoldier, INT8 bTrainStat, UINT16 *pusMaxPts )
{
	INT16 sTrainingPts = 0;
	INT16	bTrainingBonus = 0;
	INT8	bSkill = 0;

	// assume training impossible for max pts
	*pusMaxPts = 0;

	// use NATURAL not EFFECTIVE values here
	switch( bTrainStat )
	{
		case( STRENGTH ):
			bSkill = pSoldier->stats.bStrength;
		break;
		case( DEXTERITY ):
			bSkill = pSoldier->stats.bDexterity;
		break;
		case( AGILITY ):
			bSkill = pSoldier->stats.bAgility;
		break;
		case( HEALTH ):
			bSkill = pSoldier->stats.bLifeMax;
		break;
		case( LEADERSHIP ):
			bSkill = pSoldier->stats.bLeadership;
		break;
		case( MARKSMANSHIP ):
			bSkill = pSoldier->stats.bMarksmanship;
		break;
		case( EXPLOSIVE_ASSIGN ):
			bSkill = pSoldier->stats.bExplosive;
		break;
		case( MEDICAL ):
			bSkill = pSoldier->stats.bMedical;
		break;
		case( MECHANICAL ):
			bSkill = pSoldier->stats.bMechanical;
		break;
		// NOTE: Wisdom can't be trained!
		default:
			// BETA message
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"GetSoldierTrainingPts: ERROR - Unknown bTrainStat %d", bTrainStat);
			#endif
				return(0);
	}

	// if skill 0 or at/beyond the training cap, can't train
	if ( ( bSkill < gGameExternalOptions.ubTrainingSkillMin) || ( bSkill >= gGameExternalOptions.ubTrainingSkillMax ) )
	{
		return 0;
	}


	// calculate normal training pts - what it would be if his stats were "normal" (ignoring drugs, fatigue)
	*pusMaxPts = __max( ( ( pSoldier->stats.bWisdom * ( gGameExternalOptions.ubTrainingSkillMax - bSkill ) ) / gGameExternalOptions.ubSelfTrainingDivisor ), 1 );

	// calculate effective training pts
	sTrainingPts = __max( ( ( EffectiveWisdom( pSoldier ) * ( gGameExternalOptions.ubTrainingSkillMax - bSkill ) ) / gGameExternalOptions.ubSelfTrainingDivisor ), 1 );

	// SANDRO - STOMP traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// Teaching trait helps to practise self a little
		if ( HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ) && pSoldier->bAssignment == TRAIN_SELF )
		{
			// +25%
			*pusMaxPts += (*pusMaxPts * gSkillTraitValues.ubTGBonusOnPractising / 100);
			bTrainingBonus += gSkillTraitValues.ubTGBonusOnPractising;
			//sTrainingPts += (sTrainingPts * gSkillTraitValues.ubTGBonusOnPractising / 100);
		}
		// bonus for practising for intellectuals
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_INTELLECTUAL )
		{
			// +10%
			*pusMaxPts += (*pusMaxPts / 10);
			bTrainingBonus += 10;
			//sTrainingPts += (sTrainingPts / 10);
		}
		// bonus for practising for intellectuals
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
		{
			switch (bTrainStat)
			{
				case MECHANICAL:
				case MEDICAL:
				case EXPLOSIVE_ASSIGN:
					*pusMaxPts -= (*pusMaxPts / 20);
					bTrainingBonus -= 5;
					break;
			}
		}
	}

	// get special bonus if we're training marksmanship and we're in the gun range sector in Alma
	// HEADROCK HAM 3.5: Now reads from XML facilities, and works for all stats.
	if ( pSoldier->bSectorZ == 0 )
	{
		bTrainingBonus += (GetSectorModifier( pSoldier, FACILITY_PERFORMANCE_MOD ) - 100 );
	}

	// adjust for any training bonuses
	sTrainingPts += ( ( bTrainingBonus * sTrainingPts ) / 100 );

	// adjust for fatigue
	UINT32 uiTrainingPts = (UINT32) sTrainingPts;
	ReducePointsForFatigue( pSoldier, &uiTrainingPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pSoldier, &uiTrainingPts );

	sTrainingPts = (INT16)uiTrainingPts;

	return( sTrainingPts );
}

INT16 GetSoldierStudentPts( SOLDIERTYPE *pSoldier, INT8 bTrainStat, UINT16 *pusMaxPts )
{
	INT16 sTrainingPts = 0;
	INT16	bTrainingBonus = 0;
	INT8	bSkill = 0;

	INT16 sBestTrainingPts, sTrainingPtsDueToInstructor;
	UINT16	usMaxTrainerPts, usBestMaxTrainerPts = 0;
	UINT32	uiCnt;
	SOLDIERTYPE * pTrainer;

	// assume training impossible for max pts
	*pusMaxPts = 0;

	// use NATURAL not EFFECTIVE values here
	switch( bTrainStat )
	{
		case( STRENGTH ):
			bSkill = pSoldier->stats.bStrength;
		break;
		case( DEXTERITY ):
			bSkill = pSoldier->stats.bDexterity;
		break;
		case( AGILITY ):
			bSkill = pSoldier->stats.bAgility;
		break;
		case( HEALTH ):
			bSkill = pSoldier->stats.bLifeMax;
		break;
		case( LEADERSHIP ):
			bSkill = pSoldier->stats.bLeadership;
		break;
		case( MARKSMANSHIP ):
			bSkill = pSoldier->stats.bMarksmanship;
		break;
		case( EXPLOSIVE_ASSIGN ):
			bSkill = pSoldier->stats.bExplosive;
		break;
		case( MEDICAL ):
			bSkill = pSoldier->stats.bMedical;
		break;
		case( MECHANICAL ):
			bSkill = pSoldier->stats.bMechanical;
		break;
		// NOTE: Wisdom can't be trained!
		default:
			// BETA message
			#ifdef JA2BETAVERSION
		ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"GetSoldierTrainingPts: ERROR - Unknown bTrainStat %d", bTrainStat);
			#endif
		return(0);
	}

	// if skill 0 or at/beyond the training cap, can't train
	if ( bSkill < gGameExternalOptions.ubTrainingSkillMin || bSkill >= gGameExternalOptions.ubTrainingSkillMax )
	{
		return 0;
	}


	// calculate normal training pts - what it would be if his stats were "normal" (ignoring drugs, fatigue)
	*pusMaxPts = __max( ( ( pSoldier->stats.bWisdom * ( gGameExternalOptions.ubTrainingSkillMax - bSkill ) ) / gGameExternalOptions.ubSelfTrainingDivisor ), 1 );

	// calculate effective training pts
	sTrainingPts = __max( ( ( EffectiveWisdom( pSoldier ) * ( gGameExternalOptions.ubTrainingSkillMax - bSkill ) ) / gGameExternalOptions.ubSelfTrainingDivisor ), 1 );

	// SANDRO - STOMP traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// Teaching trait helps to practise self a little
		if ( HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ) && pSoldier->bAssignment == TRAIN_SELF )
		{
			// +25%
			*pusMaxPts += (*pusMaxPts * gSkillTraitValues.ubTGBonusOnPractising / 100);
			bTrainingBonus += gSkillTraitValues.ubTGBonusOnPractising;
			//sTrainingPts += (sTrainingPts * gSkillTraitValues.ubTGBonusOnPractising / 100);
		}
		// bonus for practising for intellectuals
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_INTELLECTUAL )
		{
			// +10%
			*pusMaxPts += (*pusMaxPts / 10);
			bTrainingBonus += 10;
			//sTrainingPts += (sTrainingPts / 10);
		}
		// bonus for practising for intellectuals
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
		{
			switch (bTrainStat)
			{
				case MECHANICAL:
				case MEDICAL:
				case EXPLOSIVE_ASSIGN:
					*pusMaxPts -= (*pusMaxPts / 20);
					bTrainingBonus -= 5;
					break;
			}
		}
	}

	// get special bonus if we're training marksmanship and we're in the gun range sector in Alma
	// HEADROCK HAM 3.5: Now reads from XML facilities, and works for all stats.
	if ( pSoldier->bSectorZ == 0 )
	{
		bTrainingBonus += ( GetSectorModifier( pSoldier, FACILITY_PERFORMANCE_MOD ) - 100 );
	}

	// adjust for any training bonuses
	sTrainingPts += ( ( bTrainingBonus * sTrainingPts ) / 100 );

	// adjust for fatigue
	UINT32 uiTrainingPts = (UINT32) sTrainingPts;
	ReducePointsForFatigue( pSoldier, &uiTrainingPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pSoldier, &uiTrainingPts );

	sTrainingPts = (INT16)uiTrainingPts;


	// now add in stuff for trainer

	// for each trainable stat
	sBestTrainingPts = -1;

	// search team for active instructors in this sector
	for ( uiCnt = 0, pTrainer = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ MercPtrs[0]->bTeam ].bLastID; uiCnt++, pTrainer++)
	{
		if( pTrainer->bActive && ( pTrainer->sSectorX == pSoldier->sSectorX ) && ( pTrainer->sSectorY == pSoldier->sSectorY ) && ( pTrainer->bSectorZ == pSoldier->bSectorZ) )
		{
			// if he's training teammates in this stat
			// NB skip the EnoughTime requirement to display what the value should be even if haven't been training long yet...
			if ( ( pTrainer->bAssignment == TRAIN_TEAMMATE ) && ( pTrainer->bTrainStat == bTrainStat) && ( pTrainer->flags.fMercAsleep == FALSE ) )
			{
				sTrainingPtsDueToInstructor = GetBonusTrainingPtsDueToInstructor( pTrainer, pSoldier, bTrainStat, &usMaxTrainerPts );

				// if he's the best trainer so far for this stat
				if (sTrainingPtsDueToInstructor > sBestTrainingPts)
				{
					// then remember him as that, and the points he scored
					sBestTrainingPts = sTrainingPtsDueToInstructor;
					usBestMaxTrainerPts = usMaxTrainerPts;
				}
			}
		}
	}

	if ( sBestTrainingPts != -1 )
	{
		// add the bonus to what merc can learn on his own
		sTrainingPts += sBestTrainingPts;
		*pusMaxPts += usBestMaxTrainerPts;
	}

	return( sTrainingPts );
}



void TrainSoldierWithPts( SOLDIERTYPE *pSoldier, INT16 sTrainPts )
{
	UINT8 ubChangeStat = 0;

	if( sTrainPts <= 0 )
	{
		return;
	}

	BOOLEAN addWis = FALSE;
	// which stat to modify?
	switch( pSoldier->bTrainStat )
	{
		case( STRENGTH ):
			ubChangeStat = STRAMT;
			break;
		case( DEXTERITY ):
			ubChangeStat = DEXTAMT;
			break;
		case( AGILITY ):
			ubChangeStat = AGILAMT;
			break;
		case( HEALTH ):
			ubChangeStat = HEALTHAMT;
			break;
		case( LEADERSHIP ):
			ubChangeStat = LDRAMT;
			addWis = TRUE;
			break;
		case( MARKSMANSHIP ):
			ubChangeStat = MARKAMT;
			addWis = TRUE;
			break;
		case( EXPLOSIVE_ASSIGN ):
			ubChangeStat = EXPLODEAMT;
			addWis = TRUE;
			break;
		case( MEDICAL ):
			ubChangeStat = MEDICALAMT;
			addWis = TRUE;
			break;
		case( MECHANICAL ):
			ubChangeStat = MECHANAMT;
			addWis = TRUE;
			break;
		// NOTE: Wisdom can't be trained!
		default:
			// BETA message
			#ifdef JA2BETAVERSION
		ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"TrainSoldierWithPts: ERROR - Unknown bTrainStat %d", pSoldier->bTrainStat);
			#endif
		return;
	}

	// give this merc a few chances to increase a stat (TRUE means it's training, reverse evolution doesn't apply)
	StatChange( pSoldier, ubChangeStat, sTrainPts, FROM_TRAINING );
	if (addWis)
		StatChange( pSoldier, WISDOMAMT, sTrainPts/2, FROM_TRAINING );

}


// will train a town in sector by character
BOOLEAN TrainTownInSector( SOLDIERTYPE *pTrainer, INT16 sMapX, INT16 sMapY, INT16 sTrainingPts )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 ubTownId = 0;
	
	// find out if a sam site here
	BOOLEAN fSamSiteInSector = IsThisSectorASAMSector( sMapX, sMapY, 0 );

	// get town index
	ubTownId = StrategicMap[ pTrainer->sSectorX + pTrainer->sSectorY * MAP_WORLD_X ].bNameId;
	if( fSamSiteInSector == FALSE )
	{
		AssertNE(ubTownId, BLANK_SECTOR);
	}

	// trainer gains leadership - training argument is FALSE, because the trainer is not the one training!
	StatChange( pTrainer, LDRAMT,		(UINT16) ( 1 + ( sTrainingPts / 200 ) ), FALSE );
	StatChange( pTrainer, WISDOMAMT, (UINT16) ( 1 + ( sTrainingPts / 400 ) ), FALSE );

	// increase town's training completed percentage
	if (pTrainer->bAssignment == TRAIN_TOWN)
	{	
		pSectorInfo->ubMilitiaTrainingPercentDone	+= (sTrainingPts / 100);
		pSectorInfo->ubMilitiaTrainingHundredths	+= (sTrainingPts % 100);

		if (pSectorInfo->ubMilitiaTrainingHundredths >= 100)
		{
			pSectorInfo->ubMilitiaTrainingPercentDone++;
			pSectorInfo->ubMilitiaTrainingHundredths -= 100;
		}

		// NOTE: Leave this at 100, change TOWN_TRAINING_RATE if necessary.	This value gets reported to player as a %age!
		if( pSectorInfo->ubMilitiaTrainingPercentDone >= 100 )
		{
			// Flugente: carry over training progress instead of losing it
			if ( gGameExternalOptions.gfMilitiaTrainingCarryOver )
			{
				pSectorInfo->ubMilitiaTrainingPercentDone	-= 100;
			}
			else
			{
				// zero out training completion - there's no carryover to the next training session
				pSectorInfo->ubMilitiaTrainingPercentDone	= 0;
				pSectorInfo->ubMilitiaTrainingHundredths	= 0;
			}

			// Flugente: this check is now necessary, as we might complete multiple militia session in one hour (theoretically)
			if ( pSectorInfo->fMilitiaTrainingPaid )
			{
				// make the player pay again next time he wants to train here
				pSectorInfo->fMilitiaTrainingPaid = FALSE;

				TownMilitiaTrainingCompleted( pTrainer, sMapX, sMapY );
			}

			// training done
			return( TRUE );
		}
	}
	else if (pTrainer->bAssignment == TRAIN_MOBILE)
	{
		pSectorInfo->ubMobileMilitiaTrainingPercentDone += (sTrainingPts / 100);
		pSectorInfo->ubMobileMilitiaTrainingHundredths	+= (sTrainingPts % 100);

		if (pSectorInfo->ubMobileMilitiaTrainingHundredths >= 100)
		{
			pSectorInfo->ubMobileMilitiaTrainingPercentDone++;
			pSectorInfo->ubMobileMilitiaTrainingHundredths -= 100;
		}

		// NOTE: Leave this at 100, change TOWN_TRAINING_RATE if necessary.	This value gets reported to player as a %age!
		if( pSectorInfo->ubMobileMilitiaTrainingPercentDone >= 100 )
		{
			// Flugente: carry over training progress instead of losing it
			if ( gGameExternalOptions.gfMilitiaTrainingCarryOver )
			{
				pSectorInfo->ubMobileMilitiaTrainingPercentDone	-= 100;
			}
			else
			{
				// zero out training completion - there's no carryover to the next training session
				pSectorInfo->ubMobileMilitiaTrainingPercentDone	= 0;
				pSectorInfo->ubMobileMilitiaTrainingHundredths	= 0;
			}

			// Flugente: this check is now necessary, as we might complete multiple militia session in one hour (theoretically)
			if ( pSectorInfo->fMobileMilitiaTrainingPaid )
			{
				// make the player pay again next time he wants to train here
				pSectorInfo->fMobileMilitiaTrainingPaid = FALSE;

				TownMilitiaTrainingCompleted( pTrainer, sMapX, sMapY );
			}

			// training done
			return( TRUE );
		}
	}

	return ( FALSE );
}

extern INT32 giReinforcementPool;

// handle processing of prisoners
void HandlePrisonerProcessingInSector( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	if ( !gGameExternalOptions.fAllowPrisonerSystem )
		return;

	// no underground prisons!
	if ( bZ )
		return;

	// Is there a prison in this sector?
	UINT16 prisonerbaselimit = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(sMapX, sMapY)][cnt].fFacilityHere)
		{
			// we determine wether this is a prison by checking for usPrisonBaseLimit
			if (gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0)
			{
				prisonerbaselimit = gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit;
				break;
			}
		}
	}

	if ( !prisonerbaselimit )
		return;

	// Are there any prisoners in this prison?
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
		
	UINT8 prisoners[PRISONER_MAX] = {0};
	UINT16 numprisoners = GetNumberOfPrisoners( pSectorInfo, &prisoners[PRISONER_OFFICER], &prisoners[PRISONER_ELITE], &prisoners[PRISONER_REGULAR], &prisoners[PRISONER_ADMIN] );

	// add interrogation progress from last hour and erase it in data
	UINT32	interrogationpoints = pSectorInfo->uiInterrogationHundredsLeft;
	pSectorInfo->uiInterrogationHundredsLeft = 0;

	// remember old interrogation points for exp distribution later
	UINT32 oldinterrogationpoints = interrogationpoints;

	if ( !numprisoners )
		return;

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
		return;

	// loop over all mercs in this sector that are on the FACILITY_INTERROGATE_PRISONERS assignment and determine their interrogation progress	
	UINT8 numinterrogators = 0;

	// count any interrogators found here, and sum up their interrogation values
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt=0;
	for ( uiCnt = 0, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) )
		{
			if( ( pSoldier->bAssignment == FACILITY_INTERROGATE_PRISONERS ) && ( EnoughTimeOnAssignment( pSoldier ) ) && ( pSoldier->flags.fMercAsleep == FALSE ) )
			{
				++numinterrogators;
				
				UINT16 tmp;
				interrogationpoints += CalculateInterrogationValue(pSoldier, &tmp );
			}
			else if( ( pSoldier->bAssignment == FACILITY_PRISON_SNITCH ) && CanCharacterSnitchInPrison(pSoldier) && EnoughTimeOnAssignment( pSoldier ) && ( pSoldier->flags.fMercAsleep == FALSE ) )
			{
				// first check if he wasn't exposed
					//exposition fallout handled in HandleSnitchExposition
				if( !HandleSnitchExposition(pSoldier) )
				{
					++numinterrogators;

					UINT16 tmp;
					interrogationpoints += CalculateSnitchInterrogationValue(pSoldier, &tmp );
				}
			}
		}
	}

	if ( !numinterrogators )
		return;

	// we first interrogate admins, then troops, then elites, then specials (once we figure out who those are :-) )
	// higher quality prisoners require more effort, but yield better rewards
	UINT8 interrogatedprisoners[PRISONER_MAX] = {0};

	for (int i = PRISONER_ADMIN; i < PRISONER_MAX; ++i)
	{
		while ( prisoners[i] && interrogationpoints >= gGameExternalOptions.ubPrisonerInterrogationPoints[i] )
		{
			interrogationpoints -= gGameExternalOptions.ubPrisonerInterrogationPoints[i];
			--prisoners[i];
			++interrogatedprisoners[i];
		}
	}
	
	UINT16 prisonersinterrogated = interrogatedprisoners[PRISONER_ADMIN] + interrogatedprisoners[PRISONER_REGULAR] + interrogatedprisoners[PRISONER_ELITE] + interrogatedprisoners[PRISONER_OFFICER];
	
	// the part that gets left behind is saved to the map (but not the part that gets lost due to there not being enough prisoners)
	UINT32  losthundreds = interrogationpoints / 100;
	pSectorInfo->uiInterrogationHundredsLeft = (UINT8)(interrogationpoints - losthundreds * 100);
			
	if ( !prisonersinterrogated )
		return;

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_PROCESSED], interrogatedprisoners[PRISONER_OFFICER], interrogatedprisoners[PRISONER_ELITE], interrogatedprisoners[PRISONER_REGULAR], interrogatedprisoners[PRISONER_ADMIN] );
		
	UINT16 turnedmilitia[PRISONER_MAX] = { 0 };
	UINT32 revealedpositions = 0;
	UINT32 ransomscollected = 0;
	UINT32 ransommoney = 0;
	for ( UINT32 i = 0; i < prisonersinterrogated; ++i )
	{
		// we determine what happens to the prisoners
		UINT8 result = Random( 100 );

		// chance that prisoner will work on our side as militia
		if ( result < gGameExternalOptions.ubPrisonerProcessDefectChance )
		{
			// troops are converted to militia, but there is a chance that they will be demoted in the process
			if ( i >= interrogatedprisoners[PRISONER_ADMIN] + interrogatedprisoners[PRISONER_REGULAR] + interrogatedprisoners[PRISONER_ELITE] && Chance( 80 ) )
				++turnedmilitia[PRISONER_OFFICER];
			if ( i >= interrogatedprisoners[PRISONER_ADMIN] + interrogatedprisoners[PRISONER_REGULAR] && Chance( 80 ) )
				++turnedmilitia[PRISONER_ELITE];
			else if ( i >= interrogatedprisoners[PRISONER_ADMIN] && Chance( 80 ) )
				++turnedmilitia[PRISONER_REGULAR];
			else
				++turnedmilitia[PRISONER_ADMIN];

			// we continue so that this guy cannot also run back to the queen
			continue;
		}
		// chance that prisoner will give us random info about enemy positions
		else if ( result < gGameExternalOptions.ubPrisonerProcessDefectChance + gGameExternalOptions.ubPrisonerProcessInfoBaseChance )
		{
			// if this guy is an elite or a special prisoner, there is a chance he might tell us about high-value targets!
			BOOLEAN fGetInfoOnHiddenVIPs = FALSE;
			if ( i < interrogatedprisoners[PRISONER_ADMIN] )
			{
				if ( Chance( gGameExternalOptions.ubPrisonerInterrogationEnemyGeneralInfoChance[PRISONER_ADMIN] ) )
					fGetInfoOnHiddenVIPs = TRUE;
			}
			else if ( i < interrogatedprisoners[PRISONER_REGULAR] )
			{
				if ( Chance( gGameExternalOptions.ubPrisonerInterrogationEnemyGeneralInfoChance[PRISONER_REGULAR] ) )
					fGetInfoOnHiddenVIPs = TRUE;
			}
			else if ( i < interrogatedprisoners[PRISONER_ELITE] )
			{
				if ( Chance( gGameExternalOptions.ubPrisonerInterrogationEnemyGeneralInfoChance[PRISONER_ELITE] ) )
					fGetInfoOnHiddenVIPs = TRUE;
			}
			else if ( i < interrogatedprisoners[PRISONER_OFFICER] )
			{
				if ( Chance( gGameExternalOptions.ubPrisonerInterrogationEnemyGeneralInfoChance[PRISONER_OFFICER] ) )
					fGetInfoOnHiddenVIPs = TRUE;
			}

			if ( fGetInfoOnHiddenVIPs )
			{
				UINT16 unknownvipector = 0;
				if ( GetRandomUnknownVIPSector( unknownvipector ) )
				{
					// make this guy known to the player
					StrategicMap[unknownvipector].usFlags |= ENEMY_VIP_PRESENT_KNOWN;

					CHAR16 str[128];
					GetSectorIDString( SECTORX( unknownvipector ), SECTORY( unknownvipector ), 0, str, TRUE );

					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_DETECTION_VIP], str );

					// enough info from this guy
					continue;
				}
			}

			BOOLEAN found = FALSE;
			UINT8 maxtries = 20;
			for(UINT8 infotry = 0; infotry < maxtries; ++infotry)
			{
				UINT8 usX = 1 + Random( MAP_WORLD_X - 2 );
				UINT8 usY = 1 + Random( MAP_WORLD_Y - 2 );

				// there need to be mobile enemies here - that the queen has troops in towns we do not own is hardly worthy information, and empty sectors aren't interesting
				if ( NumMobileEnemiesInSector( usX, usY ) == 0 )
					continue;

				// not if we already know about this sector
				if ( SectorInfo[ SECTOR( usX, usY ) ].uiFlags & SF_ASSIGN_NOTICED_ENEMIES_HERE )
					continue;

				// enemy patrol detected
				SectorInfo[ SECTOR( usX, usY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_HERE;

				if ( Chance(gGameExternalOptions.ubPrisonerProcessInfoNumberChance) )
				{
					// we also learned the number of enemies
					SectorInfo[ SECTOR( usX, usY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER;
				}

				if ( Chance(gGameExternalOptions.ubPrisonerProcessInfoDirectionChance) )
				{
					// we also learned the direction of the patrol
					SectorInfo[ SECTOR( usX, usY ) ].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_KNOW_DIRECTION;
				}

				++revealedpositions;
				found = TRUE;
				break;
			}
		}
		// chance prisoner will grant us ransom money
		else if ( result < gGameExternalOptions.ubPrisonerProcessDefectChance + gGameExternalOptions.ubPrisonerProcessInfoBaseChance + gGameExternalOptions.ubPrisonerProcessRansomBaseChance )
		{
			ransommoney += (Random(5) + 1) * 100;

			++ransomscollected;
		}
		// we have to let him go without any benefits
		else
		{

		}

		// there is a chance that escaped prisoners may return to the queen...
		if ( Random( 100 ) < gGameExternalOptions.ubPrisonerReturntoQueenChance )
			++giReinforcementPool;
	}
		
	if ( turnedmilitia[PRISONER_ADMIN] + turnedmilitia[PRISONER_REGULAR] + turnedmilitia[PRISONER_ELITE] + turnedmilitia[PRISONER_OFFICER] )
	{
		// add these guys to the local garrison as green militias
		StrategicAddMilitiaToSector( sMapX, sMapY, GREEN_MILITIA,   turnedmilitia[PRISONER_ADMIN] );
		StrategicAddMilitiaToSector( sMapX, sMapY, REGULAR_MILITIA, turnedmilitia[PRISONER_REGULAR] );
		StrategicAddMilitiaToSector( sMapX, sMapY, ELITE_MILITIA,   turnedmilitia[PRISONER_ELITE] + turnedmilitia[PRISONER_OFFICER] );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_TURN_MILITIA], turnedmilitia[PRISONER_OFFICER], turnedmilitia[PRISONER_ELITE], turnedmilitia[PRISONER_REGULAR], turnedmilitia[PRISONER_ADMIN] );
	}

	if ( revealedpositions )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_DETECTION], revealedpositions  );

	if ( ransomscollected )
	{
		AddTransactionToPlayersBook( PRISONER_RANSOM, 0, GetWorldTotalMin(), ransommoney );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_RANSOM], ransomscollected  );
	}

	// remove interrogated prisoners...
	ChangeNumberOfPrisoners( pSectorInfo, -interrogatedprisoners[PRISONER_OFFICER], -interrogatedprisoners[PRISONER_ELITE], -interrogatedprisoners[PRISONER_REGULAR], -interrogatedprisoners[PRISONER_ADMIN], sMapX, sMapY );

	// give experience rewards to the interrogators
	// total experience to share
	if ( !oldinterrogationpoints )
		return;

	FLOAT totalexp = (FLOAT) (1 * prisonersinterrogated);
	FLOAT expratio = totalexp / (oldinterrogationpoints * 33);	// TODO

	// award experience
	for ( uiCnt = 0, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) )
		{
			// if he's training teammates in this stat
			if( ( pSoldier->bAssignment == FACILITY_INTERROGATE_PRISONERS ) && ( EnoughTimeOnAssignment( pSoldier ) ) && ( pSoldier->flags.fMercAsleep == FALSE ) )
			{
				UINT16 tmp;
				UINT16 exppoints = (UINT16)(expratio * CalculateInterrogationValue(pSoldier, &tmp ) );
								
				StatChange( pSoldier, LDRAMT,		exppoints, TRUE );
				StatChange( pSoldier, WISDOMAMT,	max(0, exppoints - 1), TRUE );
				StatChange( pSoldier, EXPERAMT,		max(0, exppoints - 2), TRUE );
			}
		}
	}
}

// Flugente: prisons can riot if there aren't enough guards around
void HandlePrison( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	if ( !gGameExternalOptions.fAllowPrisonerSystem )
		return;

	// no underground prisons!
	if ( bZ )
		return;

	BOOLEAN fBeginRiot = FALSE;

	// Is there a prison in this sector?
	UINT16 prisonerbaselimit = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(sMapX, sMapY)][cnt].fFacilityHere)
		{
			// we determine wether this is a prison by checking for usPrisonBaseLimit
			if (gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0)
			{
				prisonerbaselimit = gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit;
				break;
			}
		}
	}

	if ( !prisonerbaselimit )
		return;

	// Are there any prisoners in this prison?
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	UINT8 prisonersofficer = 0, prisonerselite = 0, prisonersregular = 0, prisonersadmin = 0;
	UINT16 numprisoners = GetNumberOfPrisoners( pSectorInfo, &prisonersofficer, &prisonerselite, &prisonersregular, &prisonersadmin );

	if ( !numprisoners )
		return;

	CHAR16 wSectorName[ 64 ];
	GetShortSectorString( sMapX, sMapY, wSectorName );

	// if sector is not under our control, the prisoners are added to the local garrison
	if( StrategicMap[ sMapX + sMapY * MAP_WORLD_X ].fEnemyControlled == TRUE )
	{
		// add enemies
		pSectorInfo->ubNumTroops = min(255, pSectorInfo->ubNumTroops + prisonersregular);
		pSectorInfo->ubNumElites = min(255, pSectorInfo->ubNumElites + prisonerselite + prisonersofficer);
		pSectorInfo->ubNumAdmins = min(255, pSectorInfo->ubNumAdmins + prisonersadmin);

		// all prisoners are free, reduce count!
		DeleteAllPrisoners(pSectorInfo);
				
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_ARMY_FREED_PRISON], wSectorName );

		return;
	}

	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
		return;

	// loop over all mercs in this sector that are on the FACILITY_INTERROGATE_PRISONERS assignment and add up their interrogation values
	UINT8 numprisonguards = 0;
	UINT32 prisonguardvalue = 0;
		
	// count any interrogators found here, and sum up their interrogation values
	numprisonguards = CalculateAllGuardsNumberInPrison( sMapX, sMapY, bZ );
	prisonguardvalue = CalculateAllGuardsValueInPrison( sMapX, sMapY, bZ );

	// anv: snitches can only prevent mutiny if there are normal guards to cooperate with
	if( numprisonguards )
		prisonguardvalue += CalculateAllSnitchesGuardValueInPrison( sMapX, sMapY, bZ );

	if ( !numprisonguards )
		fBeginRiot = TRUE;

	// we now have to determine the combined strength of the prisoners
	UINT32 prisonerriotvalue = 200 * prisonersofficer + 125 * prisonerselite + 100 * prisonersregular + 75 * prisonersadmin;

	if ( prisonerriotvalue > prisonguardvalue )
	{
		if ( numprisoners > prisonerbaselimit && Random( prisonerriotvalue ) > Random( prisonguardvalue ) )
			fBeginRiot = TRUE;
	}

	if ( fBeginRiot )
	{
		FLOAT prisonertoguardratio = 1.0f;
		if ( prisonguardvalue )
			prisonertoguardratio = (FLOAT)(prisonerriotvalue / prisonguardvalue);

		// in a riot, prisoners escape and are added to the sector as enemies. Not all might escape - the worse the prisoner/guard ratio, the more escape
		INT16 escapedadmins		= min( Random( prisonersadmin	* prisonertoguardratio ), prisonersadmin );
		INT16 escapedregulars	= min( Random( prisonersregular	* prisonertoguardratio ), prisonersregular );
		INT16 escapedelites		= min( Random( prisonerselite	* prisonertoguardratio ), prisonerselite );
		INT16 escapedofficers   = min( Random( prisonersofficer	* prisonertoguardratio ), prisonersofficer );

		// add enemies
		pSectorInfo->ubNumTroops = min( 512, pSectorInfo->ubNumTroops + escapedregulars );
		pSectorInfo->ubNumElites = min( 512, pSectorInfo->ubNumElites + escapedelites + escapedofficers );
		pSectorInfo->ubNumAdmins = min( 512, pSectorInfo->ubNumAdmins + escapedadmins );

		// reduce prisoner count!
		ChangeNumberOfPrisoners( pSectorInfo, -escapedofficers, -escapedelites, -escapedregulars, -escapedadmins, sMapX, sMapY );

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szPrisonerTextStr[STR_PRISONER_RIOT], wSectorName  );
	}
}

// Flugente: assigned mercs can move equipemnt in city sectors
void HandleEquipmentMove( INT16 sMapX, INT16 sMapY, INT8 bZ )
{
	// no underground
	if ( bZ )
		return;
		
	// if sector not under our control, has enemies in it, or is currently in combat mode
	if (!SectorOursAndPeaceful( sMapX, sMapY, bZ ))
		return;

	// we loop over all mercs with this assignment in this sector, and then do a separate loop over each target sector
	std::map<UINT8, UINT8> sectormercmap;		// this map uses the sectors we take stuff from as keys and the number of mercs as elements

	// we need a gridno to which we drop stuff
	INT32 sDropOffGridNo = gMapInformation.sCenterGridNo;
	if ( !GridNoOnVisibleWorldTile( sDropOffGridNo ) )
		sDropOffGridNo = RandomGridNo();

	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) && pSoldier->flags.fMercAsleep == FALSE )
		{
			if( ( pSoldier->bAssignment == MOVE_EQUIPMENT ) && ( EnoughTimeOnAssignment( pSoldier ) ) )
			{
				// which sector do we want to move stuff to?
				UINT8 targetsector = pSoldier->usItemMoveSectorID;

				if ( sectormercmap[targetsector] )
					sectormercmap[targetsector]++;
				else
					sectormercmap[targetsector] = 1;

				if ( pSoldier->sGridNo != NOWHERE )
					sDropOffGridNo = pSoldier->sGridNo;
			}
		}
	}

	// no mercs that move stuff here, exit
	if ( sectormercmap.empty() )
		return;

	CHAR16 wSectorName[ 64 ];
	GetShortSectorString( sMapX, sMapY, wSectorName );

	std::vector<WORLDITEM> pWorldItem_Target;//dnl ch75 271013

	// now loop over all sectors from which we take stuff, and move the equipment
	std::map<UINT8, UINT8>::iterator itend = sectormercmap.end();
	for (std::map<UINT8, UINT8>::iterator it = sectormercmap.begin(); it != itend; ++it)
	{
		UINT8 sector = (*it).first;

		INT16 targetX = SECTORX(sector);
		INT16 targetY = SECTORY(sector);

		// if sector not under our control, has enemies in it, or is currently in combat mode
		if (!SectorOursAndPeaceful( targetX, targetY, bZ ))
			continue;

		// the longer the distance, the less we can move
		UINT8 distance = abs(sMapX - targetX) + abs(sMapY - targetY);

		// if distance is 0, somethings awry
		if ( distance == 0 )
			continue;

		// each soldier can carry 40 items or 40 kg, and needs 10 minutes (two way walk) per sector distance, thereby 6 / distance runs possible per hour
		UINT16 maxitems  =  40 * (*it).second * 6 / distance;
		UINT16 maxweight = 400 * (*it).second * 6 / distance;
		
		// open the inventory of the sector we are taking stuff from
		SECTORINFO *pSectorInfo_Target = &( SectorInfo[ SECTOR(targetX, targetY) ] );
		UINT32 uiTotalNumberOfRealItems_Target = 0;

		// use the new map
		pWorldItem_Target.clear();//dnl ch75 021113

		if( ( gWorldSectorX == targetX )&&( gWorldSectorY == targetY ) && (gbWorldSectorZ == bZ ) )
		{
			uiTotalNumberOfRealItems_Target = guiNumWorldItems;
			pWorldItem_Target = gWorldItems;
		}
		else
		{
			// not loaded, load
			// get total number, visable and invisible
			BOOLEAN fReturn = GetNumberOfWorldItemsFromTempItemFile( targetX, targetY, bZ, &( uiTotalNumberOfRealItems_Target ), FALSE );
			Assert( fReturn );

			if( uiTotalNumberOfRealItems_Target > 0 )
			{
				// allocate space for the list
				pWorldItem_Target.resize(uiTotalNumberOfRealItems_Target);//dnl ch75 271013

				// now load into mem
				LoadWorldItemsFromTempItemFile(  targetX,  targetY, bZ, pWorldItem_Target );
			}
		}
		
		// move items from Target to Here
		UINT16 moveditems = 0;
		UINT32 movedweight = 0;
		OBJECTTYPE* pObjectToMove = new OBJECTTYPE[uiTotalNumberOfRealItems_Target];
		UINT8 moveobjectcounter = 0;

		for( UINT32 uiCount = 0; uiCount < uiTotalNumberOfRealItems_Target; ++uiCount )				// ... for all items in the world ...
		{
			if( pWorldItem_Target[ uiCount ].fExists )										// ... if item exists ...
			{
				// test wether item is reachable and allowed to be moved by this assignment
				if ( (pWorldItem_Target[ uiCount ].usFlags & WORLD_ITEM_REACHABLE) && !(pWorldItem_Target[ uiCount ].usFlags & WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE) && pWorldItem_Target[ uiCount ].bVisible > 0)
				{
					OBJECTTYPE* pObj = &(pWorldItem_Target[ uiCount ].object);			// ... get pointer for this item ...

					if ( pObj != NULL && pObj->exists() )												// ... if pointer is not obviously useless ...
					{
						moveditems  += pObj->ubNumberOfObjects;
						movedweight += CalculateObjectWeight(pObj);
												
						pObjectToMove[moveobjectcounter++] = *pObj;

						pWorldItem_Target[ uiCount ].fExists = FALSE;

						// if the sector is currently loaded, we need to also remove items from item pools
						if( ( gWorldSectorX == targetX )&&( gWorldSectorY == targetY ) && (gbWorldSectorZ == bZ ) )
						{
							RemoveItemFromPool(pWorldItem_Target[ uiCount ].sGridNo, uiCount, pWorldItem_Target[ uiCount ].ubLevel);
						}

						if ( moveditems > maxitems )
							break;

						if ( movedweight > maxweight )
							break;
					}
				}
			}
		}

		// move
		if( ( gWorldSectorX == sMapX )&&( gWorldSectorY == sMapY ) && (gbWorldSectorZ == bZ ) )
		{
			for (UINT16 i = 0; i < moveobjectcounter; ++i )
			{
				AddItemToPool( sDropOffGridNo, &(pObjectToMove[i]), 1 , 0, (WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO|WORLD_ITEM_REACHABLE), -1 );
			}
		}
		else
		{
			AddItemsToUnLoadedSector( sMapX, sMapY, bZ, sDropOffGridNo, moveobjectcounter, pObjectToMove, 0, WORLD_ITEM_REACHABLE, 0, 1, FALSE );
		}

		if ( pObjectToMove )
		{
			delete[] pObjectToMove;
			pObjectToMove = NULL;
		}
				
		CHAR16 wSectorName_Target[ 64 ];
		GetShortSectorString( targetX, targetY, wSectorName_Target );
		
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d items moved from %s to %s", moveditems, wSectorName_Target, wSectorName );

		// if we didn't move any item, no need to save a changed inventory etc.
		if ( !moveditems )
			continue;

		// if the sector we took stuff from isn't loaded, do a resort, otherwise the item list will have holes (linked lists have to be rebuilt)
		WORLDITEM* pWorldItem_tmp = NULL;
		if( ( gWorldSectorX != targetX ) || ( gWorldSectorY != targetY ) || (gbWorldSectorZ != bZ ) )
		{
			std::vector<WORLDITEM> pWorldItem_tmp(uiTotalNumberOfRealItems_Target);//dnl ch75 271013

			// copy over old inventory
			UINT32 newcount = 0;
			for( UINT32 uiCount = 0; uiCount < uiTotalNumberOfRealItems_Target; ++uiCount )
			{
				if ( pWorldItem_Target[ uiCount ].fExists )
				{
					pWorldItem_tmp[newcount] = pWorldItem_Target[uiCount];
					++newcount;
				}
			}

			// use the new map
			 pWorldItem_Target.clear();//dnl ch75 021113

			pWorldItem_Target = pWorldItem_tmp;
		}
		
		// save the changed inventory
		if( ( targetX == gWorldSectorX )&&( gWorldSectorY == targetY ) && (gbWorldSectorZ == bZ ) )
		{
			guiNumWorldItems = uiTotalNumberOfRealItems_Target;
			gWorldItems = pWorldItem_Target;
		}
		else
		{
			//Save the Items to the the file
			SaveWorldItemsToTempItemFile( targetX, targetY, bZ, uiTotalNumberOfRealItems_Target, pWorldItem_Target );
		}

		// award a bit of experience to the movers
		UINT16  itemsperperson = moveditems  / (*it).second;
		UINT16 weightperperson = movedweight / (*it).second;
		for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
		{
			if( pSoldier->bActive && ( pSoldier->sSectorX == sMapX ) && ( pSoldier->sSectorY == sMapY ) && ( pSoldier->bSectorZ == bZ) && pSoldier->flags.fMercAsleep == FALSE )
			{
				if( ( pSoldier->bAssignment == MOVE_EQUIPMENT ) && ( EnoughTimeOnAssignment( pSoldier ) ) )
				{
					// which sector do we want to move stuff to?
					UINT8 targetsector = pSoldier->usItemMoveSectorID;

					if ( sector == targetsector )
					{
						UINT16 exppoints = weightperperson / 400;
								
						StatChange( pSoldier, HEALTHAMT,	exppoints, TRUE );
						StatChange( pSoldier, STRAMT,		exppoints, TRUE );
					}
				}
			}
		}
	}
}

INT16 GetTownTrainPtsForCharacter( SOLDIERTYPE *pTrainer, UINT16 *pusMaxPts )
{
	INT16 sTotalTrainingPts = 0;
	INT16 sTrainingBonus = 0;
//	UINT8 ubTownId = 0;

	// calculate normal training pts - what it would be if his stats were "normal" (ignoring drugs, fatigue)
	*pusMaxPts = ( pTrainer->stats.bWisdom + pTrainer->stats.bLeadership + ( 10 * pTrainer->stats.bExpLevel ) ) * gGameExternalOptions.ubTownMilitiaTrainingRate;

	// calculate effective training points (this is hundredths of pts / hour)
	// typical: 300/hr, maximum: 600/hr
	sTotalTrainingPts = ( EffectiveWisdom( pTrainer ) + EffectiveLeadership ( pTrainer ) + ( 10 * EffectiveExpLevel ( pTrainer ) ) ) * gGameExternalOptions.ubTownMilitiaTrainingRate;

	// check for teaching bonuses
	if( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO 
	{
		sTrainingBonus -= gSkillTraitValues.bSpeedModifierTrainingMilitia; // penalty for untrained mercs

		// bonus for teching trait
		if ( HAS_SKILL_TRAIT( pTrainer, TEACHING_NT) )
		{
			sTrainingBonus += gSkillTraitValues.ubTGBonusToTrainMilitia;
		}
		// +10% for Assertive people
		if ( gMercProfiles[pTrainer->ubProfile].bCharacterTrait == CHAR_TRAIT_ASSERTIVE )
		{
			sTrainingBonus += 10;
		}
		// -5% for Aggressive people
		else if ( gMercProfiles[pTrainer->ubProfile].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
		{
			sTrainingBonus -= 5;
		}
		// +5% for Phlegmatic people
		else if ( gMercProfiles[pTrainer->ubProfile].bCharacterTrait == CHAR_TRAIT_PHLEGMATIC )
		{
			sTrainingBonus += 5;
		}

	}
	else
	{
		if ( HAS_SKILL_TRAIT( pTrainer, TEACHING_OT) )
		{
			sTrainingBonus += (gGameExternalOptions.ubTeachBonusToTrain * NUM_SKILL_TRAITS( pTrainer, TEACHING_OT));
		}
	}

	// RPCs get a small training bonus for being more familiar with the locals and their customs/needs than outsiders
	//new profiles by Jazz
	//if( pTrainer->ubProfile >= FIRST_RPC )
	if ( gProfilesRPC[pTrainer->ubProfile].ProfilId == pTrainer->ubProfile || gProfilesNPC[pTrainer->ubProfile].ProfilId == pTrainer->ubProfile )
	{
		sTrainingBonus += gGameExternalOptions.ubRpcBonusToTrainMilitia;
	}

	// HEADROCK HAM 3.5: Training bonus given by local facilities
	if (pTrainer->bSectorZ == 0)
	{
		for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
		{
			if (gFacilityLocations[SECTOR(pTrainer->sSectorX, pTrainer->sSectorY)][cnt].fFacilityHere)
			{
				if (pTrainer->bAssignment == TRAIN_TOWN)
				{
					sTrainingBonus += (100 - gFacilityTypes[cnt].usMilitiaTraining);
				}
				else if (pTrainer->bAssignment == TRAIN_MOBILE)
				{
					sTrainingBonus += (100 - gFacilityTypes[cnt].usMobileMilitiaTraining);
				}
			}
		}
	}
	// adjust for teaching bonus (a percentage)
	sTotalTrainingPts += ( ( sTrainingBonus * sTotalTrainingPts ) / 100 );
	// teach bonus is considered "normal" - it's always there
	*pusMaxPts				+= ( ( sTrainingBonus * *pusMaxPts		) / 100 );


	// adjust for fatigue of trainer
	UINT32 uiTrainingPts = (UINT32) sTotalTrainingPts;
	ReducePointsForFatigue( pTrainer, &uiTrainingPts );

	// Flugente: our food situation influences our effectiveness
	if ( gGameOptions.fFoodSystem )
		ReducePointsForHunger( pTrainer, &uiTrainingPts );

	sTotalTrainingPts = (INT16)uiTrainingPts;


/* ARM: Decided this didn't make much sense - the guys I'm training damn well BETTER be loyal - and screw the rest!
	// get town index
	ubTownId = StrategicMap[ pTrainer->sSectorX + pTrainer->sSectorY * MAP_WORLD_X ].bNameId;
	AssertNE(ubTownId, BLANK_SECTOR);

	// adjust for town loyalty
	sTotalTrainingPts = (sTotalTrainingPts * gTownLoyalty[ ubTownId ].ubRating) / 100;
*/

	return( sTotalTrainingPts );
}


void MakeSoldiersTacticalAnimationReflectAssignment( SOLDIERTYPE *pSoldier )
{
	// soldier is in tactical, world loaded, he's OKLIFE
	if( ( pSoldier->bInSector ) && gfWorldLoaded && ( pSoldier->stats.bLife >= OKLIFE ) )
	{
		// Set animation based on his assignment
		if ( pSoldier->bAssignment == DOCTOR )
		{
			SoldierInSectorDoctor( pSoldier, pSoldier->usStrategicInsertionData );
		}
		else if ( pSoldier->bAssignment == PATIENT )
		{
			SoldierInSectorPatient( pSoldier, pSoldier->usStrategicInsertionData );
		}
		else if ( pSoldier->bAssignment == REPAIR )
		{
			SoldierInSectorRepair( pSoldier, pSoldier->usStrategicInsertionData );
		}
		else
		{
		if ( pSoldier->usAnimState != WKAEUP_FROM_SLEEP && !(pSoldier->bOldAssignment < ON_DUTY ) )
		{
				// default: standing
				pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
		}
		}
	}
}



void AssignmentAborted( SOLDIERTYPE *pSoldier, UINT8 ubReason )
{
	AssertLT( ubReason, NUM_ASSIGN_ABORT_REASONS );

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ ubReason ], pSoldier->GetName() );

	StopTimeCompression();

	// update mapscreen
	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
}



void AssignmentDone( SOLDIERTYPE *pSoldier, BOOLEAN fSayQuote, BOOLEAN fMeToo )
{
	if ( ( pSoldier->bInSector ) && ( gfWorldLoaded ) )
	{
		if ( pSoldier->bAssignment == DOCTOR )
		{
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				pSoldier->ChangeSoldierState( END_DOCTOR, 1, TRUE );
			}
			else
			{
				pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
			}

		}
		else if ( pSoldier->bAssignment == REPAIR )
		{
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				pSoldier->ChangeSoldierState( END_REPAIRMAN, 1, TRUE );
			}
			else
			{
				pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
			}
		}
		else if ( pSoldier->bAssignment == PATIENT )
		{
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				pSoldier->ChangeSoldierStance( ANIM_CROUCH );
			}
			else
			{
				pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
			}
		}
	}

	if ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL )
	{
		// hack - reset AbsoluteFinalDestination in case it was left non-nowhere
		pSoldier->sAbsoluteFinalDestination = NOWHERE;
	}

	if ( fSayQuote )
	{
		// HEADROCK HAM 3.6: Separated Militia Training
		if ( ( fMeToo == FALSE ) && (pSoldier->bAssignment == TRAIN_TOWN || pSoldier->bAssignment == TRAIN_MOBILE ) )
		{
			TacticalCharacterDialogue( pSoldier, QUOTE_ASSIGNMENT_COMPLETE );

			if( pSoldier->bAssignment == TRAIN_TOWN || pSoldier->bAssignment == TRAIN_MOBILE )
			{
				AddSectorForSoldierToListOfSectorsThatCompletedMilitiaTraining( pSoldier );
			}
		}
	}

	// don't bother telling us again about guys we already know about
	if ( !( pSoldier->usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT ) )
	{
		pSoldier->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT;

		if ( fSayQuote )
		{
			if ( pSoldier->bAssignment == DOCTOR || pSoldier->bAssignment == REPAIR ||
					pSoldier->bAssignment == PATIENT || pSoldier->bAssignment == ASSIGNMENT_HOSPITAL )
			{
				TacticalCharacterDialogue( pSoldier, QUOTE_ASSIGNMENT_COMPLETE );
			}
		}


		AddReasonToWaitingListQueue( ASSIGNMENT_FINISHED_FOR_UPDATE );
		AddSoldierToWaitingListQueue( pSoldier );

		// trigger a single call AddDisplayBoxToWaitingQueue for assignments done
		gfAddDisplayBoxToWaitingQueue = TRUE;
	}

	// update mapscreen
	fCharacterInfoPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
}

BOOLEAN CharacterIsBetweenSectors( SOLDIERTYPE *pSoldier )
{
	// is the character on the move
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}
	else
	{
		return( pSoldier->flags.fBetweenSectors );
	}
}


void HandleNaturalHealing( void )
{
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	// set psoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// go through list of characters, find all who are on this assignment
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			// mechanical members don't regenerate!
			if( !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !( AM_A_ROBOT( pTeamSoldier ) ) )
			{
				HandleHealingByNaturalCauses( pTeamSoldier );
			}
		}
	}

	return;
}


	// handle healing of this soldier by natural causes.
void HandleHealingByNaturalCauses( SOLDIERTYPE *pSoldier )
{
	UINT32 uiPercentHealth = 0;
	UINT32 uiPercentPoison = 0;
	INT8 bActivityLevelDivisor = 0;
	UINT16 usFacilityModifier = 100;
	UINT8 ubAssignmentType = 0;


	// check if soldier valid
	if( pSoldier == NULL )
	{
		return;
	}

	// dead
	if( pSoldier->stats.bLife == 0 )
	{
		return;
	}

	// lost any pts?
	if( pSoldier->stats.bLife == pSoldier->stats.bLifeMax && pSoldier->bPoisonLife == 0 )
	{
		return;
	}

	// any bleeding pts - can' recover if still bleeding!
	if( pSoldier->bBleeding != 0 )
	{
		return;
	}


	// not bleeding and injured...

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		// use high activity level to simulate stress, torture, poor conditions for healing
		bActivityLevelDivisor = gGameExternalOptions.ubHighActivityLevel;
	}
	if( ( pSoldier->flags.fMercAsleep == TRUE ) || ( pSoldier->bAssignment == PATIENT ) || ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL ) )
	{
		bActivityLevelDivisor = gGameExternalOptions.ubLowActivityLevel;
	}
	else if ( pSoldier->bAssignment < ON_DUTY )
	{
		// if time is being compressed, and the soldier is not moving strategically
		if ( IsTimeBeingCompressed() && !PlayerIDGroupInMotion( pSoldier->ubGroupID ) )
		{
			// basically resting
			bActivityLevelDivisor = gGameExternalOptions.ubLowActivityLevel;
		}
		else
		{
			// either they're on the move, or they're being tactically active
			bActivityLevelDivisor = gGameExternalOptions.ubHighActivityLevel;
		}
	}
	else	// this includes being in a vehicle - that's neither very strenous, nor very restful
	{
		bActivityLevelDivisor = gGameExternalOptions.ubMediumActivityLevel;
	}

	// HEADROCK HAM 3.6: Add better healing rate from facility
	ubAssignmentType = GetSoldierFacilityAssignmentIndex( pSoldier );
	if (ubAssignmentType == FAC_PATIENT )
	{
		usFacilityModifier = GetSectorModifier( pSoldier, FACILITY_PERFORMANCE_MOD );
	}

	// what percentage of health is he down to
	uiPercentHealth = ( pSoldier->stats.bLife * 100 ) / pSoldier->stats.bLifeMax;

	// how much is he poisoned?
	uiPercentPoison = ( pSoldier->bPoisonSum * 100 ) / pSoldier->stats.bLifeMax;

	// SANDRO - experimental - increase health regenariton of soldiers when doctors are around
	if ( gGameOptions.fNewTraitSystem )
	{
		SOLDIERTYPE *	pMedic = NULL;
		UINT8			cnt;
		UINT16 bRegenerationBonus = 0;

		cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
		for ( pMedic = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pMedic++)
		{
			if ( !(pMedic->bActive) || !(pMedic->bInSector) || ( pMedic->flags.uiStatusFlags & SOLDIER_VEHICLE ) || (pMedic->bAssignment == VEHICLE ) )
			{
				continue; // NEXT!!!
			}
			if (pMedic->stats.bLife > OKLIFE && !(pMedic->bCollapsed) && pMedic->stats.bMedical > 0 
				&& pMedic->ubID != pSoldier->ubID && HAS_SKILL_TRAIT( pMedic, DOCTOR_NT ))
			{
				bRegenerationBonus += NUM_SKILL_TRAITS( pMedic, DOCTOR_NT );
				if (bRegenerationBonus >= gSkillTraitValues.ubDOMaxRegenBonuses) // how many doctor traits can help
				{
					bRegenerationBonus = gSkillTraitValues.ubDOMaxRegenBonuses;
					break;
				}
			}
		}
		if (bRegenerationBonus > 0)
		{
			pSoldier->sFractLife += ( INT16 ) (((( uiPercentHealth / bActivityLevelDivisor ) * (100 + gSkillTraitValues.ubDONaturalRegenBonus * bRegenerationBonus) / 100 ) * usFacilityModifier ) / 100 );
		}
		else
		{
			// gain that many hundredths of life points back, divided by the activity level modifier
			pSoldier->sFractLife += ( INT16 ) ((( uiPercentHealth / bActivityLevelDivisor ) * usFacilityModifier) / 100 );
		}
	}
	else // original
	{
		// gain that many hundredths of life points back, divided by the activity level modifier
		pSoldier->sFractLife += ( INT16 ) ((( uiPercentHealth / bActivityLevelDivisor ) * usFacilityModifier) / 100 );
	}

	// Flugente: reduce gained life because of poison
	// poison resistance reduces poison damage
	// a high activity level (stress) increases poisoning
	pSoldier->sFractLife -= ( INT16 ) ( (uiPercentPoison * (100 - pSoldier->GetPoisonResistance())/100) * gGameExternalOptions.sPoisonInfectionDamageMultiplier * ((99 +  bActivityLevelDivisor)/100) );

	// now update the real life values
	UpDateSoldierLife( pSoldier );

	return;
}


void UpDateSoldierLife( SOLDIERTYPE *pSoldier )
{
	// update soldier life, make sure we don't go out of bounds
	INT8 sAddedLife		 = pSoldier->sFractLife/100;

	// if sAddedLife, it must be poison damage
	if ( sAddedLife < 0 )
	{
		// it is hereby possible to heal yourself through poison, if you have high absorption ( > 100%)
		INT8 sPoisonAbsorped = - (INT8) ( sAddedLife * (pSoldier->GetPoisonAbsorption()/100) );

		sAddedLife += sPoisonAbsorped;

		// if we're not fully poisoned, we are now poisoned a bit more, even if we won life thanks to absorption
		if ( pSoldier->bPoisonSum < pSoldier->stats.bLifeMax ) 
		{
			pSoldier->bPoisonSum += abs(sAddedLife);
		}
	}

	INT8 oldlife = pSoldier->stats.bLife;
	pSoldier->stats.bLife += sAddedLife;

	// if we fall below OKLIFE (can only be because of poison), we start bleeding again...
	if ( pSoldier->stats.bLife < OKLIFE && oldlife >= OKLIFE && sAddedLife < 0 )
	{
		/*pSoldier->stats.bLife = 0;
		BOOLEAN fMadeCorpse;
		HandleSoldierDeath( pSoldier, &fMadeCorpse );*/

		pSoldier->bBleeding = pSoldier->stats.bLifeMax - pSoldier->stats.bLife;
		pSoldier->bPoisonBleeding = pSoldier->bPoisonSum - pSoldier->bPoisonLife;

		/*pSoldier->bPoisonSum = 0;
		pSoldier->bPoisonLife = 0;
		pSoldier->bPoisonBleeding = 0;*/
	}

	// SANDRO - when being healed normally, reduce insta-healable HPs value 
	if ( gGameOptions.fNewTraitSystem && pSoldier->iHealableInjury > 0 ) 
	{
		pSoldier->iHealableInjury -= sAddedLife * 100;

		if (pSoldier->iHealableInjury < 0)
			pSoldier->iHealableInjury = 0;
	}

	// keep remaining fract of life
	pSoldier->sFractLife %= 100;

	// check if we have gone too far
	if( pSoldier->stats.bLife >= pSoldier->stats.bLifeMax )
	{
		// reduce
		pSoldier->stats.bLife = pSoldier->stats.bLifeMax;

		// only set sFractLife to be 0 if > 0, saving possible poison damage
		if ( pSoldier->sFractLife > 0 )
			pSoldier->sFractLife = 0;

		pSoldier->iHealableInjury = 0; // check added by SANDRO
	}
	return;
}



/*
// merc is tired, put to sleep
BOOLEAN AutoSleepMerc( SOLDIERTYPE *pSoldier )
{
	if( pSoldier == NULL )
	{
		return ( FALSE );
	}

	// already asleep
	if( pSoldier->flags.fMercAsleep == TRUE )
	{
		return ( FALSE );
	}

	if( pSoldier->bBreathMax > MIN_BREATH_TO_STAY_AWAKE )
	{
		if( ( pSoldier->bAssignment < ON_DUTY ) )
		{
			return ( FALSE );
		}

		if( pSoldier->stats.bLife < OKLIFE )
		{
			// can't sleep
			return ( FALSE );
		}


		// if	was forced to stay awake, leave
		if( pSoldier->fForcedToStayAwake == TRUE )
		{
			return( FALSE );
		}
	}
	else
	{
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s",	pMercFellAsleepString[ 0 ], pSoldier->GetName() );
	}


	// store old assignment
	pSoldier->bOldAssignment = pSoldier->bAssignment;


	if( pSoldier->bAssignment < ON_DUTY )
	{
		RemoveCharacterFromASquad( pSoldier, pSoldier->bAssignment );
	}

	if( SetMercAsleep( pSoldier, FALSE ) )
	{
		// change soldier state
		SoldierInSectorSleep( pSoldier, pSoldier->usStrategicInsertionData );

		// update mapscreen
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}
*/



void CheckIfSoldierUnassigned( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->bAssignment == NO_ASSIGNMENT )
	{
		// unassigned
		AddCharacterToAnySquad( pSoldier );

		if( ( gfWorldLoaded ) && ( pSoldier->bInSector ) )
		{
			pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
		}
	}

	return;
}



void CreateDestroyMouseRegionsForAssignmentMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SOLDIERTYPE *pSoldier = NULL;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	static BOOLEAN fShowRemoveMenu = FALSE;


	// will create/destroy mouse regions for the map screen assignment main menu
	// check if we can only remove character from team..not assign
	if( ( bSelectedAssignChar != -1 )|| ( fShowRemoveMenu == TRUE ) )
	{
		if( fShowRemoveMenu == TRUE )
		{
			// dead guy handle menu stuff
			fShowRemoveMenu = fShowAssignmentMenu | fShowContractMenu;

			CreateDestroyMouseRegionsForRemoveMenu( );

			return;
		}
		if( ( Menptr[gCharactersList[bSelectedAssignChar].usSolID].stats.bLife == 0 ) || ( Menptr[gCharactersList[bSelectedAssignChar].usSolID].bAssignment == ASSIGNMENT_POW ) )
		{
			// dead guy handle menu stuff
			fShowRemoveMenu = fShowAssignmentMenu | fShowContractMenu;

			CreateDestroyMouseRegionsForRemoveMenu( );

			return;
		}
	}


	if( ( fShowAssignmentMenu == TRUE ) && ( fCreated == FALSE ) )
	{

		gfIgnoreScrolling = FALSE;

		if( ( fShowAssignmentMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		{
		SetBoxPosition( ghAssignmentBox, AssignmentPosition );
		}

		pSoldier = GetSelectedAssignSoldier( FALSE );

		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			// grab height of font
			iFontHeight = GetLineSpace( ghEpcBox ) + GetFontHeight( GetBoxFont( ghEpcBox ) );

			// get x.y position of box
			GetBoxPosition( ghEpcBox, &pPosition);

			// grab box x and y position
			iBoxXPosition = pPosition.iX;
			iBoxYPosition = pPosition.iY;

			// get dimensions..mostly for width
			GetBoxSize( ghEpcBox, &pDimensions );

			// get width
			iBoxWidth = pDimensions.iRight;

			SetCurrentBox( ghEpcBox );
		}
		else
		{
			// grab height of font
			iFontHeight = GetLineSpace( ghAssignmentBox ) + GetFontHeight( GetBoxFont( ghAssignmentBox ) );

			// get x.y position of box
			GetBoxPosition( ghAssignmentBox, &pPosition);

			// grab box x and y position
			iBoxXPosition = pPosition.iX;
			iBoxYPosition = pPosition.iY;

			// get dimensions..mostly for width
			GetBoxSize( ghAssignmentBox, &pDimensions );

			// get width
			iBoxWidth = pDimensions.iRight;

			SetCurrentBox( ghAssignmentBox );
		}


		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAssignmentBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_DefineRegion( &gAssignmentMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, AssignmentMenuMvtCallBack, AssignmentMenuBtnCallback );

			MSYS_SetRegionUserData( &gAssignmentMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghAssignmentBox );
		CheckAndUpdateTacticalAssignmentPopUpPositions( );

		PositionCursorForTacticalAssignmentBox( );
	}
	else if( ( fShowAssignmentMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAssignmentBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gAssignmentMenuRegion[ iCounter ] );
		}

		fShownAssignmentMenu = FALSE;

		// not created
		fCreated = FALSE;
		SetRenderFlags( RENDER_FLAG_FULL );

	}
}



void CreateDestroyMouseRegionForVehicleMenu( void )
{
	static BOOLEAN fCreated = FALSE;

	UINT32 uiMenuLine = 0;
	INT32 iVehicleId = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition, pPoint;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;


	if( fShowVehicleMenu )
	{
		GetBoxPosition( ghAssignmentBox, &pPoint);

		// get dimensions..mostly for width
		GetBoxSize( ghAssignmentBox, &pDimensions );

		// vehicle position
		VehiclePosition.iX = pPoint.iX + pDimensions.iRight;

		SetBoxPosition( ghVehicleBox , VehiclePosition );
	}


	if( ( fShowVehicleMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// grab height of font
		iFontHeight = GetLineSpace( ghVehicleBox ) + GetFontHeight( GetBoxFont( ghVehicleBox ) );

		// get x.y position of box
		GetBoxPosition( ghVehicleBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghVehicleBox, &pDimensions );
		SetBoxSecondaryShade( ghVehicleBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghVehicleBox );

		pSoldier = GetSelectedAssignSoldier( FALSE );

		// run through list of vehicles in sector
		for ( iVehicleId = 0; iVehicleId < ubNumberOfVehicles; iVehicleId++ )
		{
			if ( pVehicleList[ iVehicleId ].fValid == TRUE )
			{
				if ( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleId ) )
				{
					// add mouse region for each accessible vehicle
					MSYS_DefineRegion( &gVehicleMenuRegion[ uiMenuLine ],	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
									MSYS_NO_CURSOR, VehicleMenuMvtCallback, VehicleMenuBtnCallback );

					MSYS_SetRegionUserData( &gVehicleMenuRegion[ uiMenuLine ], 0, uiMenuLine );
					// store vehicle ID in the SECOND user data
					MSYS_SetRegionUserData( &gVehicleMenuRegion[ uiMenuLine ], 1, iVehicleId );

					uiMenuLine++;
				}
			}
		}


		// cancel line
		MSYS_DefineRegion( &gVehicleMenuRegion[ uiMenuLine ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, VehicleMenuMvtCallback, VehicleMenuBtnCallback );
		MSYS_SetRegionUserData( &gVehicleMenuRegion[ uiMenuLine ], 0, VEHICLE_MENU_CANCEL );

		// created
		fCreated = TRUE;

		// pause game
		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghVehicleBox );

		fCreated = TRUE;

		HandleShadingOfLinesForVehicleMenu( );
	}
	else if( ( ( fShowVehicleMenu == FALSE ) || ( fShowAssignmentMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;

		// remove these regions
		for( uiMenuLine = 0; uiMenuLine < GetNumberOfLinesOfTextInBox( ghVehicleBox ); uiMenuLine++ )
		{
			MSYS_RemoveRegion( &gVehicleMenuRegion[ uiMenuLine ] );
		}

		fShowVehicleMenu = FALSE;

		SetRenderFlags( RENDER_FLAG_FULL );

		HideBox( ghVehicleBox );

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}

	return;
}



void HandleShadingOfLinesForVehicleMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleId;
	UINT32 uiMenuLine = 0;


	if ( ( fShowVehicleMenu == FALSE ) || ( ghVehicleBox == - 1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	// run through list of vehicles
	for ( iVehicleId = 0; iVehicleId < ubNumberOfVehicles; iVehicleId++ )
	{
		if ( pVehicleList[ iVehicleId ].fValid == TRUE )
		{
			// inaccessible vehicles aren't listed at all!
			if ( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleId ) )
			{
				if ( IsEnoughSpaceInVehicle( iVehicleId ) )
				{
					// legal vehicle, leave it green
					UnShadeStringInBox( ghVehicleBox, uiMenuLine );
					UnSecondaryShadeStringInBox( ghVehicleBox, uiMenuLine );
				}
				else
				{
					// unjoinable vehicle - yellow
					SecondaryShadeStringInBox( ghVehicleBox, uiMenuLine );
				}

				uiMenuLine++;
			}
		}
	}
}



void VehicleMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	INT32 iVehicleID;
	SOLDIERTYPE * pSoldier;


	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == VEHICLE_MENU_CANCEL )
		{
			fShowVehicleMenu = FALSE;
			UnHighLightBox( ghAssignmentBox );
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
			return;
		}

		pSoldier = GetSelectedAssignSoldier( FALSE );
		iVehicleID = MSYS_GetRegionUserData( pRegion, 1 );

		// inaccessible vehicles shouldn't be listed in the menu!
//		AssertT( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleID ) );

		if ( IsEnoughSpaceInVehicle( iVehicleID ) )
		{
			PutSoldierInVehicle( pSoldier, ( INT8 ) iVehicleID );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, gzLateLocalizedString[ 18 ], zVehicleName[ pVehicleList[ iVehicleID ].ubVehicleType ] );
		}

		fShowAssignmentMenu = FALSE;

		// update mapscreen
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		giAssignHighLine = -1;

		SetAssignmentForList( VEHICLE, ( INT8 ) iVehicleID );
	}
}


void VehicleMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( iValue != VEHICLE_MENU_CANCEL )
		{
			// no shaded(disabled) lines actually appear in vehicle menus
			if( GetBoxShadeFlag( ghVehicleBox, iValue ) == FALSE )
			{
				// highlight vehicle line
				HighLightBoxLine( ghVehicleBox, iValue );
			}
		}
		else
		{
			// highlight cancel line
		HighLightBoxLine( ghVehicleBox, GetNumberOfLinesOfTextInBox( ghVehicleBox ) - 1 );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghVehicleBox );

		HandleShadingOfLinesForVehicleMenu( );
	}
}



BOOLEAN DisplayRepairMenu( SOLDIERTYPE *pSoldier )
{
	INT32 iVehicleIndex=0;
	INT32 hStringHandle=0;


	// run through list of vehicles in sector and add them to pop up box
	// first, clear pop up box
	RemoveBox(ghRepairBox);
	ghRepairBox = -1;

	CreateRepairBox();
	SetCurrentBox(ghRepairBox);

	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForRepairMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForRepairMenu().

	if( pSoldier->bSectorZ == 0 )
	{
		// run through list of vehicles and see if any in sector
		for ( iVehicleIndex = 0; iVehicleIndex < ubNumberOfVehicles; iVehicleIndex++ )
		{
			if ( pVehicleList[ iVehicleIndex ].fValid == TRUE )
			{
				// don't even list the helicopter, because it's NEVER repairable...
				if ( iVehicleIndex != iHelicopterVehicleId )
				{
					if ( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleIndex ) )
					{
						//AddMonoString( (UINT32 *)&hStringHandle,pVehicleStrings[ pVehicleList[ iVehicleIndex ].ubVehicleType ] );
						AddMonoString( (UINT32 *)&hStringHandle,gNewVehicle[ pVehicleList[ iVehicleIndex ].ubVehicleType ].NewVehicleStrings );
					}
				}
			}
		}
	}


/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
	// is there a SAM SITE Here?
	if( ( IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == TRUE ) && ( IsTheSAMSiteInSectorRepairable( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) ) )
	{
		// SAM site
		AddMonoString(&hStringHandle, pRepairStrings[ 1 ] );
	}
*/


	// is the ROBOT here?
	if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		// robot
		AddMonoString((UINT32 *)&hStringHandle, pRepairStrings[ 3 ] );
	}


	// items
	AddMonoString((UINT32 *)&hStringHandle, pRepairStrings[ 0 ] );


	// cancel
	AddMonoString((UINT32 *)&hStringHandle, pRepairStrings[ 2 ] );

	SetBoxFont(ghRepairBox, MAP_SCREEN_FONT);
	SetBoxHighLight(ghRepairBox, FONT_WHITE);
	SetBoxShade(ghRepairBox, FONT_GRAY7);
	SetBoxForeground(ghRepairBox, FONT_LTGREEN);
	SetBoxBackground(ghRepairBox, FONT_BLACK);

	// resize box to text
	ResizeBoxToText( ghRepairBox );

	CheckAndUpdateTacticalAssignmentPopUpPositions( );

	return TRUE;
}


void HandleShadingOfLinesForRepairMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleIndex = 0;
	INT32 iCount = 0;


	if( ( fShowRepairMenu == FALSE ) || ( ghRepairBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );


	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForRepairMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForRepairMenu().

	if ( pSoldier->bSectorZ == 0 )
	{
		for ( iVehicleIndex = 0; iVehicleIndex < ubNumberOfVehicles; iVehicleIndex++ )
		{
			if ( pVehicleList[ iVehicleIndex ].fValid == TRUE )
			{
				// don't even list the helicopter, because it's NEVER repairable...
				if ( iVehicleIndex != iHelicopterVehicleId )
				{
					if ( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleIndex ) )
					{
						if( CanCharacterRepairVehicle( pSoldier, iVehicleIndex ) == TRUE )
						{
							// unshade vehicle line
							UnShadeStringInBox( ghRepairBox, iCount );
						}
						else
						{
							// shade vehicle line
							ShadeStringInBox( ghRepairBox, iCount );
						}

						iCount++;
					}
				}
			}
		}
	}


/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
	if( ( IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == TRUE ) && ( IsTheSAMSiteInSectorRepairable( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) ) )
	{
		// handle enable disable of repair sam option
		if( CanSoldierRepairSAM( pSoldier, SAM_SITE_REPAIR_DIVISOR ) )
		{
			// unshade SAM line
			UnShadeStringInBox( ghRepairBox, iCount );
		}
		else
		{
			// shade SAM line
			ShadeStringInBox( ghRepairBox, iCount );
		}

		iCount++;
	}
*/


	if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		// handle shading of repair robot option
		if( CanCharacterRepairRobot( pSoldier ) )
		{
			// unshade robot line
			UnShadeStringInBox( ghRepairBox, iCount );
		}
		else
		{
			// shade robot line
			ShadeStringInBox( ghRepairBox, iCount );
		}

		iCount++;
	}


	if ( DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS ) )
	{
		// unshade items line
		UnShadeStringInBox( ghRepairBox, iCount );
	}
	else
	{
		// shade items line
		ShadeStringInBox( ghRepairBox, iCount );
	}

	iCount++;


	return;
}


void CreateDestroyMouseRegionForRepairMenu( void )
{
	static BOOLEAN fCreated = FALSE;

	UINT32 uiCounter = 0;
	INT32 iCount = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleIndex = 0;


	if( ( fShowRepairMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed below: repositioning now the same way as for training in AssignmentMenuBtnCB as it caused missplaced box for higher resolutions then 3.
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );
		//if( ( fShowRepairMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		// //SetBoxPosition( ghRepairBox ,RepairPosition);
		//}

		// grab height of font
		iFontHeight = GetLineSpace( ghRepairBox ) + GetFontHeight( GetBoxFont( ghRepairBox ) );

		// get x.y position of box
		GetBoxPosition( ghRepairBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghRepairBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghRepairBox );


		pSoldier = GetSelectedAssignSoldier( FALSE );

		// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
		// CreateDestroyMouseRegionForRepairMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForRepairMenu().

		if ( pSoldier->bSectorZ == 0 )
		{
			// vehicles
			for ( iVehicleIndex = 0; iVehicleIndex < ubNumberOfVehicles; iVehicleIndex++ )
			{
				if ( pVehicleList[ iVehicleIndex ].fValid == TRUE )
				{
					// don't even list the helicopter, because it's NEVER repairable...
					if ( iVehicleIndex != iHelicopterVehicleId )
					{
						// other vehicles *in the sector* are listed, but later shaded dark if they're not repairable
						if ( IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleIndex ) )
						{
							// add mouse region for each line of text..and set user data
							MSYS_DefineRegion( &gRepairMenuRegion[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
										MSYS_NO_CURSOR, RepairMenuMvtCallback, RepairMenuBtnCallback );

							MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 0, iCount );
							// 2nd user data is the vehicle index, which can easily be different from the region index!
							MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 1, iVehicleIndex );
							iCount++;
						}
					}
				}
			}
		}


/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
		// SAM site
		if( ( IsThisSectorASAMSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == TRUE ) && ( IsTheSAMSiteInSectorRepairable( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) ) )
		{
			MSYS_DefineRegion( &gRepairMenuRegion[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
								MSYS_NO_CURSOR, RepairMenuMvtCallback, RepairMenuBtnCallback );

			MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 0, REPAIR_MENU_SAM_SITE );
			iCount++;
		}
*/


		// robot
		if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
		{
			MSYS_DefineRegion( &gRepairMenuRegion[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
								MSYS_NO_CURSOR, RepairMenuMvtCallback, RepairMenuBtnCallback );

			MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 0, iCount );
			MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 1, REPAIR_MENU_ROBOT );
			iCount++;
		}


		// items
		MSYS_DefineRegion( &gRepairMenuRegion[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
								MSYS_NO_CURSOR, RepairMenuMvtCallback, RepairMenuBtnCallback );

		MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 0, iCount );
		MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 1, REPAIR_MENU_ITEMS );
		iCount++;


		// cancel
		MSYS_DefineRegion( &gRepairMenuRegion[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, RepairMenuMvtCallback, RepairMenuBtnCallback );

		MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 0, iCount );
		MSYS_SetRegionUserData( &gRepairMenuRegion[ iCount ], 1, REPAIR_MENU_CANCEL );


		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghRepairBox );

		fCreated = TRUE;
	}
	else if( ( ( fShowRepairMenu == FALSE ) || ( fShowAssignmentMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;

		// remove these regions
		for( uiCounter = 0; uiCounter < GetNumberOfLinesOfTextInBox( ghRepairBox ); uiCounter++ )
		{
			MSYS_RemoveRegion( &gRepairMenuRegion[ uiCounter ] );
		}

		fShowRepairMenu = FALSE;

		SetRenderFlags( RENDER_FLAG_FULL );

		HideBox( ghRepairBox );

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}

	return;
}



void RepairMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iRepairWhat;


	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	// ignore clicks on disabled lines
	if( GetBoxShadeFlag( ghRepairBox, iValue ) == TRUE )
	{
		return;
	}

	// WHAT is being repaired is stored in the second user data argument
	iRepairWhat = MSYS_GetRegionUserData( pRegion, 1 );


	pSoldier = GetSelectedAssignSoldier( FALSE );


	if ( pSoldier && pSoldier->bActive && ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		if( ( iRepairWhat >= REPAIR_MENU_VEHICLE1 ) && ( iRepairWhat <= REPAIR_MENU_VEHICLE3 ) )
		{
			// repair VEHICLE

			pSoldier->bOldAssignment = pSoldier->bAssignment;

			if( ( pSoldier->bAssignment != REPAIR )|| ( pSoldier->flags.fFixingRobot ) || ( pSoldier->flags.fFixingSAMSite ) )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}

			// remove from squad
			RemoveCharacterFromSquads( pSoldier );
			MakeSureToolKitIsInHand( pSoldier );

			ChangeSoldiersAssignment( pSoldier, REPAIR );

			pSoldier->bVehicleUnderRepairID = ( INT8 ) iRepairWhat;

			MakeSureToolKitIsInHand( pSoldier );

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );

			// set assignment for group
			SetAssignmentForList( ( INT8 ) REPAIR, 0 );
			fShowAssignmentMenu = FALSE;

		}
/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
		else if( iRepairWhat == REPAIR_MENU_SAM_SITE )
		{
			// repair SAM site

			// remove from squad
			RemoveCharacterFromSquads( pSoldier );
			MakeSureToolKitIsInHand( pSoldier );

			if( ( pSoldier->bAssignment != REPAIR )|| ( pSoldier->flags.fFixingSAMSite == FALSE ) )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			ChangeSoldiersAssignment( pSoldier, REPAIR );
			pSoldier->flags.fFixingSAMSite = TRUE;

			// the second argument is irrelevant here, function looks at pSoldier itself to know what's being repaired
			SetAssignmentForList( ( INT8 ) REPAIR, 0 );
			fShowAssignmentMenu = FALSE;

			MakeSureToolKitIsInHand( pSoldier );

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );
		}
*/
		else if( iRepairWhat == REPAIR_MENU_ROBOT )
		{
			// repair ROBOT
			pSoldier->bOldAssignment = pSoldier->bAssignment;

			// remove from squad
			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}

			RemoveCharacterFromSquads( pSoldier );
			MakeSureToolKitIsInHand( pSoldier );

			if( ( pSoldier->bAssignment != REPAIR )|| ( pSoldier->flags.fFixingRobot == FALSE ) )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			ChangeSoldiersAssignment( pSoldier, REPAIR );
			pSoldier->flags.fFixingRobot = TRUE;

			// the second argument is irrelevant here, function looks at pSoldier itself to know what's being repaired
			SetAssignmentForList( ( INT8 ) REPAIR, 0 );
			fShowAssignmentMenu = FALSE;

			MakeSureToolKitIsInHand( pSoldier );

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );
		}
		else if( iRepairWhat == REPAIR_MENU_ITEMS )
		{
			// items
			SetSoldierAssignment( pSoldier, REPAIR, FALSE, FALSE, -1 );

			// the second argument is irrelevant here, function looks at pSoldier itself to know what's being repaired
			SetAssignmentForList( ( INT8 ) REPAIR, 0 );
			fShowAssignmentMenu = FALSE;
		}
		else
		{
			// CANCEL
			fShowRepairMenu = FALSE;
		}

		// update mapscreen
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		giAssignHighLine = -1;
	}
}


void RepairMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( iValue < REPAIR_MENU_CANCEL )
		{
			if( GetBoxShadeFlag( ghRepairBox, iValue ) == FALSE )
			{
				// highlight choice
				HighLightBoxLine( ghRepairBox, iValue );
			}
		}
		else
		{
			// highlight cancel line
			HighLightBoxLine( ghRepairBox, GetNumberOfLinesOfTextInBox( ghRepairBox ) - 1 );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghRepairBox );
	}
}

BOOLEAN MakeSureToolKitIsInHand( SOLDIERTYPE *pSoldier )
{
	//JMich_SkillModifiers: added bonus to see which is the maximum, and an extra pocket to store the highest bonus found so far.
	INT8 bPocket = 0, bonus = -101, bToolkitPocket = NO_SLOT;

	// if there isn't a toolkit in his hand
	if( Item[pSoldier->inv[ HANDPOS].usItem].toolkit )
	{
		bonus = Item[pSoldier->inv[ HANDPOS].usItem].RepairModifier;
		bToolkitPocket = HANDPOS;
	}
		
	// run through rest of inventory looking for toolkits, swap the first one into hand if found
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = SECONDHANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		if( Item[pSoldier->inv[ bPocket ].usItem].toolkit && Item[pSoldier->inv[ bPocket ].usItem].RepairModifier > bonus)
		{
			bonus = Item[pSoldier->inv[ bPocket ].usItem].RepairModifier;
			bToolkitPocket = bPocket;
		}
	}

	// Flugente: ehem... shouldn't we actually CHECK wether there IS a toolkit? We should check that beforehand...
	if ( bToolkitPocket == NO_SLOT )
		return FALSE;
	
	// HEADROCK HAM B2.8: These new conditions will create a bias for swapping an item out of our hand. 
				
	//If the second hand is free, the item will go to the SECONDHANDPOS while the toolkit goes into the HANDPOS
	if( Item[pSoldier->inv[HANDPOS].usItem].usItemClass & (IC_WEAPON | IC_PUNCH) && !pSoldier->inv[SECONDHANDPOS].exists())
		SwapObjs(pSoldier, HANDPOS, SECONDHANDPOS, TRUE);
	// Else, if the gun sling slot is free, and the item can go there, it will.
	else if( UsingNewInventorySystem() && !pSoldier->inv[GUNSLINGPOCKPOS].exists() && CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], GUNSLINGPOCKPOS, FALSE) )
		SwapObjs(pSoldier, HANDPOS, GUNSLINGPOCKPOS, TRUE);
	else if(!CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], bToolkitPocket, FALSE))
		SwapObjs(pSoldier, HANDPOS, SECONDHANDPOS, TRUE);
	
	SwapObjs( pSoldier, HANDPOS, bToolkitPocket, TRUE );

	return TRUE;
}


BOOLEAN MakeSureMedKitIsInHand( SOLDIERTYPE *pSoldier )
{
	INT8 bPocket = 0;

	fTeamPanelDirty = TRUE;

	// if there is a MEDICAL BAG in his hand, we're set
	if ( Item[pSoldier->inv[ HANDPOS ].usItem].medicalkit )
	{
		return(TRUE);
	}

	// run through rest of inventory looking 1st for MEDICAL BAGS, swap the first one into hand if found
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = SECONDHANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		if ( Item[pSoldier->inv[ bPocket ].usItem].medicalkit )
		{
			fCharacterInfoPanelDirty = TRUE;
			// HEADROCK HAM B2.8: These new conditions will create a bias for swapping an item out of
			// our hand. 
			
			//If the second hand is free, the item will go to the SECONDHANDPOS while the medikit
			// goes into the HANDPOS
			if( Item[pSoldier->inv[HANDPOS].usItem].usItemClass & (IC_WEAPON | IC_PUNCH) && !pSoldier->inv[SECONDHANDPOS].exists())
				SwapObjs(pSoldier, HANDPOS, SECONDHANDPOS, TRUE);
			// Else, if the gun sling slot is free, and the item can go there, it will.
			else if(UsingNewInventorySystem() && !pSoldier->inv[GUNSLINGPOCKPOS].exists() && CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], GUNSLINGPOCKPOS, FALSE))
				SwapObjs(pSoldier, HANDPOS, GUNSLINGPOCKPOS, TRUE);
			else if(!CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], bPocket, FALSE))
				SwapObjs(pSoldier, HANDPOS, SECONDHANDPOS, TRUE);
			SwapObjs( pSoldier, HANDPOS, bPocket, TRUE );
			return(TRUE);
		}
	}

	// we didn't find a medical bag, so settle for a FIRST AID KIT
	if ( Item[pSoldier->inv[ HANDPOS ].usItem].firstaidkit )
	{
		return(TRUE);
	}

	// run through rest of inventory looking 1st for MEDICAL BAGS, swap the first one into hand if found
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = SECONDHANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		if ( Item[pSoldier->inv[ bPocket ].usItem].firstaidkit )
		{
			// CHRISL: This needs to start with the first "non-big" pocket.
			if( ( Item[ pSoldier -> inv[ HANDPOS ].usItem ].twohanded  ) && ( bPocket >= SMALLPOCKSTART ) )
			{
				// first move from hand to second hand
				SwapObjs( pSoldier, HANDPOS, SECONDHANDPOS, TRUE );

				// dirty mapscreen and squad panels
				fCharacterInfoPanelDirty = TRUE;
				fInterfacePanelDirty = DIRTYLEVEL2;
			}

			SwapObjs( pSoldier, HANDPOS, bPocket, TRUE );
			return(TRUE);
		}
	}

	// no medkit items in possession!
	return(FALSE);
}



void HandleShadingOfLinesForAssignmentMenus( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// updates which menus are selectable based on character status


	if( ( fShowAssignmentMenu == FALSE ) || ( ghAssignmentBox == - 1 ) )
	{
		return;
	}


	pSoldier = GetSelectedAssignSoldier( FALSE );

	if ( pSoldier && pSoldier->bActive )
	{
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			// patient
			if( CanCharacterPatient( pSoldier ) )
			{
				// unshade patient line
				UnShadeStringInBox( ghEpcBox, EPC_MENU_PATIENT );
			}
			else
			{
				// shade patient line
				ShadeStringInBox( ghEpcBox, EPC_MENU_PATIENT );
			}


			if( CanCharacterOnDuty( pSoldier ) )
			{
				// unshade on duty line
				UnShadeStringInBox( ghEpcBox, EPC_MENU_ON_DUTY );
			}
			else
			{
				// shade on duty line
				ShadeStringInBox( ghEpcBox, EPC_MENU_ON_DUTY );
			}

			if( CanCharacterVehicle( pSoldier ) )
			{
				// unshade vehicle line
				UnShadeStringInBox( ghEpcBox, EPC_MENU_VEHICLE );
			}
			else
			{
				// shade vehicle line
				ShadeStringInBox( ghEpcBox, EPC_MENU_VEHICLE );
			}
		}
		else
		{
			// doctor
			if( CanCharacterDoctor( pSoldier ) )
			{
				// unshade doctor line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_DOCTOR );
				UnSecondaryShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_DOCTOR );
			}
			else
			{
				// only missing a med kit
				if( CanCharacterDoctorButDoesntHaveMedKit( pSoldier ) )
				{
					SecondaryShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_DOCTOR );
				}
				else
				{
					// shade doctor line
					ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_DOCTOR );
				}

			}

			// repair
			if( CanCharacterRepair( pSoldier ) )
			{
				// unshade repair line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_REPAIR );
				UnSecondaryShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_REPAIR );
			}
			else
			{
				// only missing a tool kit
				if( CanCharacterRepairButDoesntHaveARepairkit( pSoldier ) )
				{
					SecondaryShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_REPAIR );
				}
				else
				{
					// shade repair line
					ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_REPAIR );
				}
			}

			// radio scan
			if( pSoldier->CanUseRadio() )
			{
				// unshade line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_RADIO_SCAN );
			}
			else
			{
				// shade line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_RADIO_SCAN );
			}

			// patient
			if( CanCharacterPatient( pSoldier ) )
			{
				// unshade patient line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_PATIENT );
			}
			else
			{
				// shade patient line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_PATIENT );
			}


			if( CanCharacterOnDuty( pSoldier ) )
			{
				// unshade on duty line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_ON_DUTY );
			}
			else
			{
				// shade on duty line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_ON_DUTY );
			}


			if( CanCharacterPractise( pSoldier ) )
			{
				// unshade train line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_TRAIN );
			}
			else
			{
				// shade train line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_TRAIN );
			}

			if( CanCharacterPractise( pSoldier ) )
			{
				// unshade train line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_MOVE_ITEMS );
			}
			else
			{
				// shade train line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_MOVE_ITEMS );
			}

			if( CanCharacterVehicle( pSoldier ) )
			{
				// unshade vehicle line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_VEHICLE );
			}
			else
			{
				// shade vehicle line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_VEHICLE );
			}
			if (BasicCanCharacterFacility( pSoldier ))
			{
				// unshade facility line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_FACILITY );
			}
			else
			{
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_FACILITY );
			}
			// snitch
			if( CanCharacterSnitch( pSoldier ) )
			{
				// unshade line
				UnShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_SNITCH );
			}
			else
			{
				// shade line
				ShadeStringInBox( ghAssignmentBox, ASSIGN_MENU_SNITCH );
			}
		}
	}

	// squad submenu
	HandleShadingOfLinesForSquadMenu( );

	// vehicle submenu
	HandleShadingOfLinesForVehicleMenu( );

	// repair submenu
	HandleShadingOfLinesForRepairMenu( );

	// move item submenu
	HandleShadingOfLinesForMoveItemMenu();

	// training submenu
	HandleShadingOfLinesForTrainingMenu( );

	// training attributes submenu
	HandleShadingOfLinesForAttributeMenus( );
	// HEADROCK HAM 3.6: Facility Menu
	HandleShadingOfLinesForFacilityMenu( );

	// HEADROCK HAM 3.6: Facility Submenu
	HandleShadingOfLinesForFacilityAssignmentMenu( );

	// anv: snitch menus shading
	HandleShadingOfLinesForSnitchMenu( );
	HandleShadingOfLinesForSnitchToggleMenu( );
	HandleShadingOfLinesForSnitchSectorMenu( );

	return;
}



void DetermineWhichAssignmentMenusCanBeShown( void )
{
	BOOLEAN fCharacterNoLongerValid = FALSE;
	SOLDIERTYPE *pSoldier = NULL;

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if( fShowMapScreenMovementList == TRUE )
		{
			if( bSelectedDestChar == -1 )
			{
				fCharacterNoLongerValid = TRUE;
				HandleShowingOfMovementBox( );
			}
			else
			{
				fShowMapScreenMovementList = FALSE;
				fCharacterNoLongerValid = TRUE;
			}
		}
/*
		else if( fShowUpdateBox )
		{
			//handle showing of the merc update box
			HandleShowingOfUpBox( );
		}
*/
		else if( bSelectedAssignChar == -1 )
		{
			fCharacterNoLongerValid = TRUE;
		}

		// update the assignment positions
		UpdateMapScreenAssignmentPositions( );
	}

	// determine which assign menu needs to be shown
	if( ( ( fShowAssignmentMenu == FALSE ) )||( fCharacterNoLongerValid == TRUE ) )
	{
		// reset show assignment menus
		fShowAssignmentMenu = FALSE;
		fShowVehicleMenu = FALSE;
		fShowRepairMenu = FALSE;
		fShowMoveItemMenu = FALSE;
		// HEADROCK HAM 3.6: Reset Facility menu
		fShowFacilityMenu = FALSE;
		// anv: reset show snitch menu
		fShowSnitchMenu = FALSE;

		// destroy mask, if needed
		CreateDestroyScreenMaskForAssignmentAndContractMenus( );


		// destroy menu if needed
		CreateDestroyMouseRegionForVehicleMenu( );
		CreateDestroyMouseRegionsForAssignmentMenu( );
		CreateDestroyMouseRegionsForTrainingMenu( );
		CreateDestroyMouseRegionsForAttributeMenu( );
		CreateDestroyMouseRegionsForSquadMenu( TRUE );
		CreateDestroyMouseRegionForRepairMenu( );
		CreateDestroyMouseRegionForMoveItemMenu();
		// HEADROCK HAM 3.6: Facility Menu, Submenu
		CreateDestroyMouseRegionForFacilityMenu( );
		CreateDestroyMouseRegionsForFacilityAssignmentMenu( );
		// anv: snitch menus
		CreateDestroyMouseRegionsForSnitchMenu( );
		CreateDestroyMouseRegionsForSnitchToggleMenu( );
		CreateDestroyMouseRegionsForSnitchSectorMenu( );

		// hide all boxes being shown
		if ( IsBoxShown( ghEpcBox ) )
		{
			HideBox( ghEpcBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghAssignmentBox ) )
		{
			HideBox( ghAssignmentBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghTrainingBox ) )
		{
			HideBox( ghTrainingBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghRepairBox ) )
		{
			HideBox( ghRepairBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghMoveItemBox ) )
		{
			HideBox( ghMoveItemBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghAttributeBox ) )
		{
			HideBox( ghAttributeBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghVehicleBox ) )
		{
			HideBox( ghVehicleBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		// HEADROCK HAM 3.6: Facility Menu
		if ( IsBoxShown( ghFacilityBox ) )
		{
			HideBox( ghFacilityBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		// HEADROCK HAM 3.6: Facility Submenu
		if ( IsBoxShown( ghFacilityAssignmentBox ) )
		{
			HideBox( ghFacilityAssignmentBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		// anv: snitch menus
		if ( IsBoxShown( ghSnitchBox ) )
		{
			HideBox( ghSnitchBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghSnitchToggleBox ) )
		{
			HideBox( ghSnitchToggleBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		if ( IsBoxShown( ghSnitchSectorBox ) )
		{
			HideBox( ghSnitchSectorBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		// do we really want ot hide this box?
		if( fShowContractMenu == FALSE )
		{
			if ( IsBoxShown( ghRemoveMercAssignBox ) )
		{
			HideBox( ghRemoveMercAssignBox );
				fTeamPanelDirty = TRUE;
				gfRenderPBInterface = TRUE;
		}
		}
		//HideBox( ghSquadBox );


		//SetRenderFlags(RENDER_FLAG_FULL);

		// no menus, leave
		return;
	}

	// update the assignment positions
	UpdateMapScreenAssignmentPositions( );

	// create mask, if needed
	CreateDestroyScreenMaskForAssignmentAndContractMenus( );


	// created assignment menu if needed
	CreateDestroyMouseRegionsForAssignmentMenu( );
	CreateDestroyMouseRegionsForTrainingMenu( );
	CreateDestroyMouseRegionsForAttributeMenu( );
	CreateDestroyMouseRegionsForSquadMenu( TRUE );
	CreateDestroyMouseRegionForRepairMenu(	);
	CreateDestroyMouseRegionForMoveItemMenu();
	CreateDestroyMouseRegionsForSnitchMenu( );
	CreateDestroyMouseRegionsForSnitchToggleMenu( );
	CreateDestroyMouseRegionsForSnitchSectorMenu( );

	if( ( ( Menptr[gCharactersList[ bSelectedInfoChar ].usSolID].stats.bLife == 0 )||( Menptr[gCharactersList[bSelectedInfoChar].usSolID].bAssignment == ASSIGNMENT_POW ) ) && ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) ) )
	{
		// show basic assignment menu
		ShowBox( ghRemoveMercAssignBox );
	}
	else
	{
		pSoldier = GetSelectedAssignSoldier( FALSE );

		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			ShowBox( ghEpcBox );
		}
		else
		{
			// show basic assignment menu

			ShowBox( ghAssignmentBox );
		}
	}

	// TRAINING menu
	if( fShowTrainingMenu == TRUE )
	{
		HandleShadingOfLinesForTrainingMenu( );
		ShowBox( ghTrainingBox );
	}
	else
	{
		if( IsBoxShown( ghTrainingBox ) )
		{
			HideBox( ghTrainingBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}

	// REPAIR menu
	if( fShowRepairMenu == TRUE )
	{
		HandleShadingOfLinesForRepairMenu( );
		ShowBox( ghRepairBox );
	}
	else
	{
		// hide box
		if( IsBoxShown( ghRepairBox ) )
		{
			HideBox( ghRepairBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}

	// Move Item menu
	if( fShowMoveItemMenu == TRUE )
	{
		HandleShadingOfLinesForMoveItemMenu( );
		ShowBox( ghMoveItemBox );
	}
	else
	{
		// hide box
		if( IsBoxShown( ghMoveItemBox ) )
		{
			HideBox( ghMoveItemBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}
		
	// ATTRIBUTE menu
	if( fShowAttributeMenu == TRUE )
	{
		HandleShadingOfLinesForAttributeMenus( );
		ShowBox( ghAttributeBox );
	}
	else
	{
		if( IsBoxShown( ghAttributeBox ) )
		{
			HideBox( ghAttributeBox );
			fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}

	}

	// VEHICLE menu
	if( fShowVehicleMenu == TRUE )
	{
		ShowBox( ghVehicleBox );
	}
	else
	{
		if( IsBoxShown( ghVehicleBox ) )
		{
			HideBox( ghVehicleBox );
			fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}

	CreateDestroyMouseRegionForVehicleMenu( );
	// HEADROCK HAM 3.6: FACILITY menu
	if( fShowFacilityMenu == TRUE )
	{
		HandleShadingOfLinesForFacilityMenu( );
		ShowBox( ghFacilityBox );
	}
	else
	{
		if( IsBoxShown( ghFacilityBox ) )
		{
			HideBox( ghFacilityBox );
			fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		gfRenderPBInterface = TRUE;
		//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}
	CreateDestroyMouseRegionForFacilityMenu( );

	// Facility Sub-menu
	if( fShowFacilityAssignmentMenu == TRUE )
	{
		HandleShadingOfLinesForFacilityAssignmentMenu( );
		ShowBox( ghFacilityAssignmentBox );
	}
	else
	{
		if( IsBoxShown( ghFacilityAssignmentBox ) )
		{
			HideBox( ghFacilityAssignmentBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			//	SetRenderFlags(RENDER_FLAG_FULL);
		}

	}
	CreateDestroyMouseRegionsForFacilityAssignmentMenu( );

	// SNITCH menu
	if( fShowSnitchMenu == TRUE )
	{
		HandleShadingOfLinesForSnitchMenu( );
		ShowBox( ghSnitchBox );
	}
	else
	{
		if( IsBoxShown( ghSnitchBox ) )
		{
			HideBox( ghSnitchBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}

	// SNITCH sub-menu 1
	if( fShowSnitchToggleMenu == TRUE )
	{
		HandleShadingOfLinesForSnitchToggleMenu( );
		ShowBox( ghSnitchToggleBox );
	}
	else
	{
		if( IsBoxShown( ghSnitchToggleBox ) )
		{
			HideBox( ghSnitchToggleBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}
	// SNITCH sub-menu 2
	if( fShowSnitchSectorMenu == TRUE )
	{
		HandleShadingOfLinesForSnitchSectorMenu( );
		ShowBox( ghSnitchSectorBox );
	}
	else
	{
		if( IsBoxShown( ghSnitchSectorBox ) )
		{
			HideBox( ghSnitchSectorBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			//	SetRenderFlags(RENDER_FLAG_FULL);
		}
	}
	return;
}



void CreateDestroyScreenMaskForAssignmentAndContractMenus( void )
{

	static BOOLEAN fCreated = FALSE;
	// will create a screen mask to catch mouse input to disable assignment menus

	// not created, create
	if( ( fCreated == FALSE ) && ( ( fShowAssignmentMenu == TRUE )||( fShowContractMenu == TRUE ) || ( fShowTownInfo == TRUE) ) )
	{
		MSYS_DefineRegion( &gAssignmentScreenMaskRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, AssignmentScreenMaskBtnCallback );

		// created
		fCreated = TRUE;

		if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
		{
			MSYS_ChangeRegionCursor(	&gAssignmentScreenMaskRegion, 0 );
		}

	}
	else if( ( fCreated == TRUE ) && ( fShowAssignmentMenu == FALSE )&&( fShowContractMenu == FALSE ) && ( fShowTownInfo == FALSE )	)
	{
		// created, get rid of it
		MSYS_RemoveRegion(	&gAssignmentScreenMaskRegion );

		// not created
		fCreated = FALSE;
	}

	return;
}


void AssignmentScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment screen mask region

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		if( fFirstClickInAssignmentScreenMask == TRUE )
		{
			fFirstClickInAssignmentScreenMask = FALSE;
			return;
		}

		// button event, stop showing menus
		fShowAssignmentMenu = FALSE;

		fShowVehicleMenu = FALSE;

		fShowContractMenu = FALSE;

		// stop showing town mine info
		fShowTownInfo = FALSE;

		// reset contract character and contract highlight line
		giContractHighLine =-1;
		bSelectedContractChar = -1;
		fGlowContractRegion = FALSE;


		// update mapscreen
	fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

	gfRenderPBInterface = TRUE;
		SetRenderFlags( RENDER_FLAG_FULL );

	}
	return;
}

void ClearScreenMaskForMapScreenExit( void )
{

	// reset show assignment menu
	fShowAssignmentMenu = FALSE;

	// update the assignment positions
	UpdateMapScreenAssignmentPositions( );

	// stop showing town mine info too
	fShowTownInfo = FALSE;

	// destroy mask, if needed
	CreateDestroyScreenMaskForAssignmentAndContractMenus( );

	// destroy assignment menu if needed
	CreateDestroyMouseRegionsForAssignmentMenu( );
	CreateDestroyMouseRegionsForTrainingMenu( );
	CreateDestroyMouseRegionsForAttributeMenu( );
	CreateDestroyMouseRegionsForSquadMenu( TRUE );
	CreateDestroyMouseRegionForRepairMenu(	);
	CreateDestroyMouseRegionForMoveItemMenu();
	// HEADROCK HAM 3.6: Facility Menu
	CreateDestroyMouseRegionForFacilityMenu( );
	CreateDestroyMouseRegionsForSnitchMenu( );
	CreateDestroyMouseRegionsForSnitchToggleMenu( );
	CreateDestroyMouseRegionsForSnitchSectorMenu( );

	return;
}

void CreateDestroyMouseRegions( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen assignment main menu


	// do we show the remove menu
	if( fShowRemoveMenu )
	{
		CreateDestroyMouseRegionsForRemoveMenu( );
		return;
	}

	if( ( fShowAssignmentMenu == TRUE ) && ( fCreated == FALSE ) )
	{

		// grab height of font
		iFontHeight = GetLineSpace( ghAssignmentBox ) + GetFontHeight( GetBoxFont( ghAssignmentBox ) );

		// get x.y position of box
		GetBoxPosition( ghAssignmentBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghAssignmentBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghAssignmentBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAssignmentBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gAssignmentMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, AssignmentMenuMvtCallBack, AssignmentMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gAssignmentMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// pause game
		PauseGame( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghAssignmentBox );

	}
	else if( ( fShowAssignmentMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAssignmentBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gAssignmentMenuRegion[ iCounter ] );
		}

		RestorePopUpBoxes();

		// not created
		fCreated = FALSE;
	}


}


void CreateDestroyMouseRegionsForContractMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	static BOOLEAN fShowRemoveMenu = FALSE;

	// will create/destroy mouse regions for the map screen Contract main menu
	// will create/destroy mouse regions for the map screen assignment main menu
	// check if we can only remove character from team..not assign
	if( ( bSelectedContractChar != -1 )|| ( fShowRemoveMenu == TRUE ) )
	{
		if( fShowRemoveMenu == TRUE )
		{
			// dead guy handle menu stuff
			fShowRemoveMenu =	fShowContractMenu;

		// ATE: Added this setting of global variable 'cause
		// it will cause an assert failure in GetSelectedAssignSoldier()
		bSelectedAssignChar = bSelectedContractChar;

			CreateDestroyMouseRegionsForRemoveMenu( );

			return;
		}
		if( Menptr[gCharactersList[bSelectedContractChar].usSolID].stats.bLife == 0 )
		{

			// dead guy handle menu stuff
			fShowRemoveMenu =	fShowContractMenu;

		// ATE: Added this setting of global variable 'cause
		// it will cause an assert failure in GetSelectedAssignSoldier()
		bSelectedAssignChar = bSelectedContractChar;

			CreateDestroyMouseRegionsForRemoveMenu( );

			return;
		}
	}


	if( ( fShowContractMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		if( bSelectedContractChar == -1 )
		{
			return;
		}

		if( fShowContractMenu )
		{
		SetBoxPosition( ghContractBox , ContractPosition );
		}
		// grab height of font
		iFontHeight = GetLineSpace( ghContractBox ) + GetFontHeight( GetBoxFont( ghContractBox ) );

		// get x.y position of box
		GetBoxPosition( ghContractBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghContractBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghContractBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghContractBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gContractMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghContractBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghContractBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, ContractMenuMvtCallback, ContractMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gContractMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// pause game
		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghContractBox );

	}
	else if( ( fShowContractMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghContractBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gContractMenuRegion[ iCounter ] );
		}

		fShownContractMenu = FALSE;
		//if( ( fProcessingAMerc ) && ( pProcessingSoldier ) )
		//{
		//	if( (UINT32)(pProcessingSoldier->iEndofContractTime) == GetWorldTotalMin() )
		//	{
		//		StrategicRemoveMerc( pProcessingSoldier, MERC_FIRED );
		//		pProcessingSoldier = NULL;
		//		fProcessingAMerc = FALSE;
		//	}
		//}

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		RestorePopUpBoxes( );

		// not created
		fCreated = FALSE;
	}
}



void CreateDestroyMouseRegionsForTrainingMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen assignment main menu



	if( ( fShowTrainingMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowTrainingMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//SetBoxPosition( ghTrainingBox, TrainPosition );
		//}
		//
		//HandleShadingOfLinesForTrainingMenu( );
		//
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghTrainingBox ) + GetFontHeight( GetBoxFont( ghTrainingBox ) );

		// get x.y position of box
		GetBoxPosition( ghTrainingBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghTrainingBox, &pDimensions );
		SetBoxSecondaryShade( ghTrainingBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghTrainingBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghTrainingBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gTrainingMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghTrainingBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghTrainingBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 3 ,
							MSYS_NO_CURSOR, TrainingMenuMvtCallBack, TrainingMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gTrainingMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghTrainingBox );

	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowTrainingMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghTrainingBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gTrainingMenuRegion[ iCounter ] );
		}

		// stop showing training menu
		if( fShowAssignmentMenu == FALSE )
		{
			fShowTrainingMenu = FALSE;
		}

		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghTrainingBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}
}



void CreateDestroyMouseRegionsForAttributeMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen attribute	menu



	if( ( fShowAttributeMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowAssignmentMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//SetBoxPosition( ghAssignmentBox, AssignmentPosition );
		//}
		
		//HandleShadingOfLinesForAttributeMenus( );
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghAttributeBox ) + GetFontHeight( GetBoxFont( ghAttributeBox ) );

		// get x.y position of box
		GetBoxPosition( ghAttributeBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghAttributeBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghAttributeBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAttributeBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gAttributeMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAttributeBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAttributeBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
							MSYS_NO_CURSOR, AttributeMenuMvtCallBack, AttributesMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gAttributeMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghAttributeBox );

	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowTrainingMenu == FALSE ) ||( fShowAttributeMenu == FALSE) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghAttributeBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gAttributeMenuRegion[ iCounter ] );
		}

		// stop showing training menu
		if( ( fShowAssignmentMenu == FALSE )||(	fShowTrainingMenu == FALSE ) || ( fShowAttributeMenu == FALSE))
		{
			fShowAttributeMenu = FALSE;
			gfRenderPBInterface = TRUE;
		}


		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghAttributeBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;

		if ( fShowTrainingMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghTrainingBox );
		}
	}
}



void CreateDestroyMouseRegionsForRemoveMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen attribute	menu
	if( ( ( fShowAssignmentMenu == TRUE ) || ( fShowContractMenu == TRUE ) ) && ( fCreated == FALSE ) )
	{

		if( fShowContractMenu )
		{
		SetBoxPosition( ghContractBox , ContractPosition );
		}
		else
		{
			SetBoxPosition( ghAssignmentBox , AssignmentPosition );
		}

		if( fShowContractMenu )
		{
			// set box position to contract box position
			SetBoxPosition( ghRemoveMercAssignBox , ContractPosition );
		}
		else
		{
			// set box position to contract box position
			SetBoxPosition( ghRemoveMercAssignBox , AssignmentPosition );
		}

		CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghRemoveMercAssignBox ) + GetFontHeight( GetBoxFont( ghRemoveMercAssignBox ) );

		// get x.y position of box
		GetBoxPosition( ghRemoveMercAssignBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghRemoveMercAssignBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghRemoveMercAssignBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghRemoveMercAssignBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gRemoveMercAssignRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAttributeBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAttributeBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
							MSYS_NO_CURSOR, RemoveMercMenuMvtCallBack,	RemoveMercMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gRemoveMercAssignRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghRemoveMercAssignBox );

	}
	else if( ( fShowAssignmentMenu == FALSE ) && ( fCreated == TRUE ) && ( fShowContractMenu == FALSE )	)
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghRemoveMercAssignBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gRemoveMercAssignRegion[ iCounter ] );
		}

		fShownContractMenu = FALSE;

		// stop showing	menu
		if( fShowRemoveMenu == FALSE )
		{
			fShowAttributeMenu = FALSE;
			// HEADROCK HAM 3.6: Stop showing Facility submenu
			fShowFacilityAssignmentMenu = FALSE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}





		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		// turn off the GLOBAL fShowRemoveMenu flag!!!
		fShowRemoveMenu = FALSE;
		// and the assignment menu itself!!!
		fShowAssignmentMenu = FALSE;

		// not created
		fCreated = FALSE;
	}


}


void CreateDestroyMouseRegionsForSquadMenu( BOOLEAN fPositionBox )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;


	// will create/destroy mouse regions for the map screen attribute	menu

	if( ( fShowSquadMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// create squad box
		CreateSquadBox( );
		GetBoxSize( ghAssignmentBox, &pDimensions );

		CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghSquadBox ) + GetFontHeight( GetBoxFont( ghSquadBox ) );

		// get x.y position of box
		GetBoxPosition( ghSquadBox, &pPosition);


		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghSquadBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghSquadBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSquadBox ) - 1; iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_DefineRegion( &gSquadMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSquadBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSquadBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
							MSYS_NO_CURSOR, SquadMenuMvtCallBack, SquadMenuBtnCallback );

			MSYS_SetRegionUserData( &gSquadMenuRegion[ iCounter ], 0, iCounter );
		}

		// now create cancel region
		MSYS_DefineRegion( &gSquadMenuRegion[ iCounter ], ( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSquadBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSquadBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
							MSYS_NO_CURSOR, SquadMenuMvtCallBack, SquadMenuBtnCallback );

		MSYS_SetRegionUserData( &gSquadMenuRegion[ iCounter ], 0, SQUAD_MENU_CANCEL );


		// created
		fCreated = TRUE;

		// show the box
		ShowBox( ghSquadBox );

		// unhighlight all strings in box
		UnHighLightBox( ghSquadBox );

		// update based on current squad
		HandleShadingOfLinesForSquadMenu( );
	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowSquadMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSquadBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gSquadMenuRegion[ iCounter ] );
		}

		fShowSquadMenu = FALSE;

		// remove squad box
		RemoveBox(ghSquadBox);
		ghSquadBox = -1;

		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;
		fMapPanelDirty = TRUE;

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}
}

void CreateDestroyMouseRegionsForSnitchMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen assignment main menu

	if( ( fShowSnitchMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowTrainingMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//SetBoxPosition( ghTrainingBox, TrainPosition );
		//}
		//
		//HandleShadingOfLinesForTrainingMenu( );
		//
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghSnitchBox ) + GetFontHeight( GetBoxFont( ghSnitchBox ) );

		// get x.y position of box
		GetBoxPosition( ghSnitchBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghSnitchBox, &pDimensions );
		SetBoxSecondaryShade( ghSnitchBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghSnitchBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gSnitchMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 3 ,
				MSYS_NO_CURSOR, SnitchMenuMvtCallBack, SnitchMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gSnitchMenuRegion[ iCounter ], 0, iCounter );

			// Add tooltip for region
			if (wcscmp(pSnitchToggleMenuDescStrings[ iCounter ], L"") != 0)
			{
				SetRegionFastHelpText( &gSnitchMenuRegion[ iCounter ], pSnitchMenuDescStrings[ iCounter ] );
			}
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghSnitchBox );

	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowSnitchMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gSnitchMenuRegion[ iCounter ] );
		}

		// stop showing training menu
		if( fShowAssignmentMenu == FALSE )
		{
			fShowSnitchMenu = FALSE;
		}

		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghSnitchBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}
}

void CreateDestroyMouseRegionsForSnitchToggleMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen assignment main menu

	if( ( fShowSnitchToggleMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowTrainingMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//SetBoxPosition( ghTrainingBox, TrainPosition );
		//}
		//
		//HandleShadingOfLinesForTrainingMenu( );
		//
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghSnitchToggleBox ) + GetFontHeight( GetBoxFont( ghSnitchToggleBox ) );

		// get x.y position of box
		GetBoxPosition( ghSnitchToggleBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghSnitchToggleBox, &pDimensions );
		SetBoxSecondaryShade( ghSnitchToggleBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghSnitchToggleBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchToggleBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gSnitchToggleMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchToggleBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchToggleBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 3 ,
				MSYS_NO_CURSOR, SnitchToggleMenuMvtCallBack, SnitchToggleMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gSnitchToggleMenuRegion[ iCounter ], 0, iCounter );

			// Add tooltip for region
			if (wcscmp(pSnitchToggleMenuDescStrings[ iCounter ], L"") != 0)
			{
				SetRegionFastHelpText( &gSnitchToggleMenuRegion[ iCounter ], pSnitchToggleMenuDescStrings[ iCounter ] );
			}
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghSnitchToggleBox );

	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowSnitchMenu == FALSE ) || ( fShowSnitchToggleMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchToggleBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gSnitchToggleMenuRegion[ iCounter ] );
		}

		// stop showing training menu
		if( fShowAssignmentMenu == FALSE )
		{
			fShowSnitchMenu = FALSE;
		}
		if( fShowSnitchMenu == FALSE )
		{
			fShowSnitchToggleMenu = FALSE;
		}
		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghSnitchToggleBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;

		if ( fShowSnitchMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghSnitchBox );
		}
	}
}

void CreateDestroyMouseRegionsForSnitchSectorMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	// will create/destroy mouse regions for the map screen assignment main menu

	if( ( fShowSnitchSectorMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowTrainingMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//SetBoxPosition( ghTrainingBox, TrainPosition );
		//}
		//
		//HandleShadingOfLinesForTrainingMenu( );
		//
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghSnitchSectorBox ) + GetFontHeight( GetBoxFont( ghSnitchSectorBox ) );

		// get x.y position of box
		GetBoxPosition( ghSnitchSectorBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghSnitchSectorBox, &pDimensions );
		SetBoxSecondaryShade( ghSnitchSectorBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghSnitchSectorBox );

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchSectorBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data


			MSYS_DefineRegion( &gSnitchSectorMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchSectorBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghSnitchSectorBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 3 ,
				MSYS_NO_CURSOR, SnitchSectorMenuMvtCallBack, SnitchSectorMenuBtnCallback );

			// set user defines
			MSYS_SetRegionUserData( &gSnitchSectorMenuRegion[ iCounter ], 0, iCounter );

			// Add tooltip for region
			if (wcscmp(pSnitchSectorMenuDescStrings[ iCounter ], L"") != 0)
			{
				SetRegionFastHelpText( &gSnitchSectorMenuRegion[ iCounter ], pSnitchSectorMenuDescStrings[ iCounter ] );
			}
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghSnitchSectorBox );

	}
	else if( ( ( fShowAssignmentMenu == FALSE ) || ( fShowSnitchMenu == FALSE ) || ( fShowSnitchSectorMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		// destroy
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghSnitchSectorBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gSnitchSectorMenuRegion[ iCounter ] );
		}

		// stop showing training menu
		if( fShowAssignmentMenu == FALSE )
		{
			fShowSnitchMenu = FALSE;
		}
		if( fShowSnitchMenu == FALSE )
		{
			fShowSnitchSectorMenu = FALSE;
		}
		RestorePopUpBoxes( );

		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghSnitchSectorBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		// not created
		fCreated = FALSE;

		if ( fShowSnitchMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghSnitchBox );
		}
	}
}

void AssignmentMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	SOLDIERTYPE *pSoldier;

	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );


	pSoldier = GetSelectedAssignSoldier( FALSE );

	if( HandleAssignmentExpansionAndHighLightForAssignMenu( pSoldier ) == TRUE )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// is the line shaded?..if so, don't highlight
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			if( GetBoxShadeFlag( ghEpcBox, iValue ) == FALSE )
			{
				HighLightBoxLine( ghEpcBox, iValue );
			}
		}
		else
		{
			if( GetBoxShadeFlag( ghAssignmentBox, iValue ) == FALSE )
			{
				HighLightBoxLine( ghAssignmentBox, iValue );
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			// unhighlight all strings in box
			UnHighLightBox( ghEpcBox );
		}
		else
		{
			// unhighlight all strings in box
			UnHighLightBox( ghAssignmentBox );
		}
	}
}


void RemoveMercMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		// get the string line handle
		// is the line shaded?..if so, don't highlight
		if( GetBoxShadeFlag( ghRemoveMercAssignBox, iValue ) == FALSE )
		{
			HighLightBoxLine( ghRemoveMercAssignBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghRemoveMercAssignBox );
	}
}


void ContractMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for Contract region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		if( iValue != CONTRACT_MENU_CURRENT_FUNDS )
		{
			if( GetBoxShadeFlag( ghContractBox, iValue ) == FALSE )
			{
				// get the string line handle
				HighLightBoxLine( ghContractBox, iValue );
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghContractBox );
	}
}



void SquadMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		if( iValue != SQUAD_MENU_CANCEL )
		{
			if( GetBoxShadeFlag( ghSquadBox, iValue ) == FALSE )
			{
				// get the string line handle
				HighLightBoxLine( ghSquadBox, iValue );
			}
		}
		else
		{
			// highlight cancel line
		HighLightBoxLine( ghSquadBox, GetNumberOfLinesOfTextInBox( ghSquadBox ) - 1 );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghSquadBox );

		// update based on current squad
		HandleShadingOfLinesForSquadMenu( );
	}
}


void RemoveMercMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for contract region
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		switch( iValue )
		{
		case( REMOVE_MERC_CANCEL ):

				// stop showing menus
				fShowAssignmentMenu = FALSE;
				fShowContractMenu = FALSE;

				// reset characters
				bSelectedAssignChar = -1;
				bSelectedContractChar = -1;
				giAssignHighLine = -1;

				// dirty regions
				fCharacterInfoPanelDirty = TRUE;
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;


				// stop contratc glow if we are
				fGlowContractRegion = FALSE;
				giContractHighLine = -1;

				break;
			case( REMOVE_MERC ):
				StrategicRemoveMerc( pSoldier );

				// dirty region
				fCharacterInfoPanelDirty = TRUE;
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;

				// stop contratc glow if we are
				fGlowContractRegion = FALSE;
				giContractHighLine = -1;

				// reset selected characters
				bSelectedAssignChar = -1;
				bSelectedContractChar = -1;
				giAssignHighLine = -1;

				// stop showing menus
				fShowAssignmentMenu = FALSE;
				fShowContractMenu = FALSE;

				//Def: 10/13/99:	When a merc is either dead or a POW, the Remove Merc popup comes up instead of the
				// Assign menu popup.	When the the user removes the merc, we need to make sure the assignment menu
				//doesnt come up ( because the both assign menu and remove menu flags are needed for the remove pop up to appear
				//dont ask why?!! )
				fShownContractMenu = FALSE;
				fShownAssignmentMenu = FALSE;
				fShowRemoveMenu = FALSE;

				break;
		}
	}
}

void BeginRemoveMercFromContract( SOLDIERTYPE *pSoldier )
{
	// This function will setup the quote, then start dialogue beginning the actual leave sequence
	if( ( pSoldier->stats.bLife > 0 ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) )
	{

#ifdef JA2UB	
		//Ja25 UB
		//if the merc cant leave
		if( !CanMercBeAllowedToLeaveTeam( pSoldier ) )
		{
			HaveMercSayWhyHeWontLeave( pSoldier );
			return;
		}
#endif

		// WANNE: Nothing to do here, when we want to dismiss the robot
		BOOLEAN	fAmIaRobot = AM_A_ROBOT( pSoldier );

		// Flugente: If merc is unconscious, just fire him anyway (if talking stuff is called, this leads to a geme lock)
		if (!fAmIaRobot && pSoldier->stats.bLife > CONSCIOUSNESS )		
		{
			if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC ) || ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__NPC ) )
			{
				HandleImportantMercQuote( pSoldier,	QUOTE_RESPONSE_TO_MIGUEL_SLASH_QUOTE_MERC_OR_RPC_LETGO );

				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE,0 ,MAP_SCREEN ,0 ,0 ,0 );
				TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING, 1,0 );

			}
			else
			{
				// quote is different if he's fired in less than 48 hours
				if( ( GetWorldTotalMin() - pSoldier->uiTimeOfLastContractUpdate ) < 60 * 48 )
				{
					SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE,1 ,MAP_SCREEN ,0 ,0 ,0 );
					if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) )
					{
						// Only do this if they want to renew.....
						if ( WillMercRenew( pSoldier, FALSE ) )
						{
							HandleImportantMercQuote( pSoldier, QUOTE_DEPART_COMMET_CONTRACT_NOT_RENEWED_OR_TERMINATED_UNDER_48 );
						}
					}

					SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE,0 ,MAP_SCREEN ,0 ,0 ,0 );
					TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING, 1,0 );

				}
				else
				{
					SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE,1 ,MAP_SCREEN ,0 ,0 ,0 );
					if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) )
					{
						// Only do this if they want to renew.....
						if ( WillMercRenew( pSoldier, FALSE ) )
						{
							HandleImportantMercQuote( pSoldier,	QUOTE_DEPARTING_COMMENT_CONTRACT_NOT_RENEWED_OR_48_OR_MORE );
						}
					}
					else if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC ) || ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__NPC ) )
					{
						HandleImportantMercQuote( pSoldier,	QUOTE_RESPONSE_TO_MIGUEL_SLASH_QUOTE_MERC_OR_RPC_LETGO );
					}

					SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE,0 ,MAP_SCREEN ,0 ,0 ,0 );
					TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING, 1,0 );
				}
			}

			if( ( GetWorldTotalMin() - pSoldier->uiTimeOfLastContractUpdate ) < 60 * 3 )
			{
				// this will cause him give us lame excuses for a while until he gets over it
				// 3-6 days (but the first 1-2 days of that are spent "returning" home)
				gMercProfiles[ pSoldier->ubProfile ].ubDaysOfMoraleHangover = (UINT8) (3 + Random(4));

				// if it's an AIM merc, word of this gets back to AIM...	Bad rep.
				if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
				{
					ModifyPlayerReputation(REPUTATION_EARLY_FIRING);

					// piss off his buddies too
					HandleBuddiesReactionToFiringMerc(pSoldier, MORALE_BUDDY_FIRED_EARLY);
					
				}
			}
		}
		// WANNE: When we want to dismiss the robot, simply dismiss, without any special stuff
		else
		{
			StrategicRemoveMerc(pSoldier);
		}		
	}
}


void MercDismissConfirmCallBack( UINT8 bExitValue )
{
	if ( bExitValue == MSG_BOX_RETURN_YES )
	{
		// Setup history code
		gpDismissSoldier->ubLeaveHistoryCode = HISTORY_MERC_FIRED;

		BeginRemoveMercFromContract( gpDismissSoldier );
	}
}


void ContractMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for contract region
	INT32 iValue = -1;
	BOOLEAN fOkToClose = FALSE;
	SOLDIERTYPE * pSoldier = NULL;


	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		pSoldier = &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ];
	}
	else
	{
		// can't renew contracts from tactical!
	}

	AssertNotNIL( pSoldier );
	AssertT( pSoldier->bActive );


	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		fOkToClose = TRUE;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( GetBoxShadeFlag( ghContractBox, iValue ) == TRUE )
		{
			// not valid
			return;
		}


		if( iValue == CONTRACT_MENU_CANCEL )
		{
			// reset contract character and contract highlight line
			giContractHighLine =-1;
			bSelectedContractChar = -1;
			fGlowContractRegion = FALSE;

			fShowContractMenu = FALSE;
			// dirty region
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;

			if ( gfInContractMenuFromRenewSequence )
			{
				BeginRemoveMercFromContract( pSoldier );
			}
			return;
		}

		// else handle based on contract

		switch( iValue )
		{
		case( CONTRACT_MENU_DAY ):
				MercContractHandling( pSoldier, CONTRACT_EXTEND_1_DAY );
				PostContractMessage( pSoldier, CONTRACT_EXTEND_1_DAY );
				fOkToClose = TRUE;
			break;
			case( CONTRACT_MENU_WEEK ):
				MercContractHandling( pSoldier, CONTRACT_EXTEND_1_WEEK );
				PostContractMessage( pSoldier, CONTRACT_EXTEND_1_WEEK );
				fOkToClose = TRUE;
			break;
			case( CONTRACT_MENU_TWO_WEEKS ):
				MercContractHandling( pSoldier, CONTRACT_EXTEND_2_WEEK );
				PostContractMessage( pSoldier, CONTRACT_EXTEND_2_WEEK );
				fOkToClose = TRUE;
			break;

			case( CONTRACT_MENU_TERMINATE ):
		gpDismissSoldier = pSoldier;

		// If in the renewal sequence.. do right away...
		// else put up requester.
			if ( gfInContractMenuFromRenewSequence )
		{
			MercDismissConfirmCallBack( MSG_BOX_RETURN_YES );
		}
		else
		{
			// The game should be unpaused when this message box disappears
			UnPauseGame();
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[ 48 ], MAP_SCREEN, MSG_BOX_FLAG_YESNO, MercDismissConfirmCallBack );
		}

		fOkToClose = TRUE;

			break;
		}

		pProcessingSoldier = NULL;
		fProcessingAMerc = FALSE;
	}


	if( fOkToClose == TRUE )
	{
		// reset contract character and contract highlight line
		giContractHighLine =-1;
		bSelectedContractChar = -1;
		fGlowContractRegion = FALSE;
		fShowContractMenu = FALSE;

	// dirty region
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	gfRenderPBInterface = TRUE;
	}


	return;
}



void TrainingMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( HandleAssignmentExpansionAndHighLightForTrainingMenu( ) == TRUE )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		// do not highlight current balance
		if( GetBoxShadeFlag( ghTrainingBox, iValue ) == FALSE )
		{
		// get the string line handle
		HighLightBoxLine( ghTrainingBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghTrainingBox );
	}
}



void AttributeMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string
		if( GetBoxShadeFlag( ghAttributeBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghAttributeBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghAttributeBox );
	}
}


void SquadMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	CHAR16 sString[ 128 ];
	INT8	bCanJoinSquad;

	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == SQUAD_MENU_CANCEL )
		{
			// stop displaying, leave
			fShowSquadMenu = FALSE;

			// unhighlight the assignment box
			UnHighLightBox( ghAssignmentBox );

			// dirty region
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;

			return;
		}

		bCanJoinSquad =	CanCharacterSquad( pSoldier, ( INT8 )iValue );
		// can the character join this squad?	(If already in it, accept that as a legal choice and exit menu)
		if ( ( bCanJoinSquad == CHARACTER_CAN_JOIN_SQUAD ) ||
				( bCanJoinSquad == CHARACTER_CANT_JOIN_SQUAD_ALREADY_IN_IT ) )
		{
			if ( bCanJoinSquad == CHARACTER_CAN_JOIN_SQUAD )
			{
				// able to add, do it

/* ARM: Squad menu is now disabled for anyone between sectors
				// old squad character was in
				iOldSquadValue = SquadCharacterIsIn( pSoldier );

				// grab if char was between sectors
				fCharacterWasBetweenSectors = pSoldier->flags.fBetweenSectors;

				if( fCharacterWasBetweenSectors )
				{
					if( pSoldier->bAssignment == VEHICLE )
					{
						if( GetNumberInVehicle( pSoldier->iVehicleId ) == 1 )
						{
							// can't change, go away
							return;
						}
					}
				}

				if( pSoldier->ubGroupID )
				{
					GetGroupPosition(&ubNextX, &ubNextY, &ubPrevX, &ubPrevY, &uiTraverseTime, &uiArriveTime, pSoldier->ubGroupID );
				}
*/
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// silversurfer: This guy was in the heli and gets out in a hostile sector. Everyone else get out of the heli and start combat!
				if ( pSoldier->bOldAssignment == VEHICLE && pSoldier->iVehicleId == iHelicopterVehicleId && NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
				{
					UINT8 ubGroupID = MoveAllInHelicopterToFootMovementGroup( iValue );
					CheckConditionsForBattle( GetGroup( ubGroupID ) );
				}
				// old normal handling
				else
				{

					if( pSoldier->bOldAssignment == VEHICLE )
					{
						TakeSoldierOutOfVehicle( pSoldier );
					}

					AddCharacterToSquad( pSoldier, ( INT8 )iValue );

					if( pSoldier->bOldAssignment == VEHICLE )
					{
						SetSoldierExitVehicleInsertionData( pSoldier, pSoldier->iVehicleId, pSoldier->ubGroupID );
					}

					//Clear any desired squad assignments -- seeing the player has physically changed it!
					pSoldier->ubNumTraversalsAllowedToMerge = 0;
					pSoldier->ubDesiredSquadAssignment = NO_ASSIGNMENT;


/* ARM: Squad menu is now disabled for anyone between sectors
				if( fCharacterWasBetweenSectors )
				{
					// grab location of old squad and set this value for new squad
					if( iOldSquadValue != -1 )
					{
						GetSquadPosition( &ubNextX, &ubNextY, &ubPrevX, &ubPrevY, &uiTraverseTime, &uiArriveTime,	( UINT8 )iOldSquadValue );
					}

					SetGroupPosition( ubNextX, ubNextY, ubPrevX, ubPrevY, uiTraverseTime, uiArriveTime, pSoldier->ubGroupID );
				}
*/

					MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );
				}
			}

			// stop displaying, leave
			fShowAssignmentMenu = FALSE;
			giAssignHighLine = -1;

			// dirty region
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		else
		{
			BOOLEAN fDisplayError = TRUE;

			switch( bCanJoinSquad )
			{
				case CHARACTER_CANT_JOIN_SQUAD_SQUAD_MOVING:
					if ( gGameExternalOptions.fUseXMLSquadNames )
						swprintf( sString, pMapErrorString[ 36 ], pSoldier->GetName(), SquadNames[ iValue ].squadname );
					else
						swprintf( sString, pMapErrorString[ 36 ], pSoldier->GetName(), pLongAssignmentStrings[ iValue ] );
					break;
				case CHARACTER_CANT_JOIN_SQUAD_VEHICLE:
					swprintf( sString, pMapErrorString[ 37 ], pSoldier->GetName() );
					break;
				case CHARACTER_CANT_JOIN_SQUAD_TOO_FAR:
					if ( gGameExternalOptions.fUseXMLSquadNames )
						swprintf( sString, pMapErrorString[ 20 ], pSoldier->GetName(), SquadNames[ iValue ].squadname );
					else
						swprintf( sString, pMapErrorString[ 20 ], pSoldier->GetName(), pLongAssignmentStrings[ iValue ] );
					break;
				case CHARACTER_CANT_JOIN_SQUAD_FULL:
					if ( gGameExternalOptions.fUseXMLSquadNames )
						swprintf( sString, pMapErrorString[ 19 ], pSoldier->GetName(), SquadNames[ iValue ].squadname );
					else
						swprintf( sString, pMapErrorString[ 19 ], pSoldier->GetName(), pLongAssignmentStrings[ iValue ] );
					break;
				default:
					// generic "you can't join this squad" msg
					swprintf( sString, pMapErrorString[ 38 ], pSoldier->GetName(), pLongAssignmentStrings[ iValue ] );
					break;
			}

			if ( fDisplayError )
			{
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL);
			}

		}

		// set this assignment for the list too
		SetAssignmentForList( ( INT8 ) iValue, 0 );
	}

	return;
}

void SnitchMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( HandleAssignmentExpansionAndHighLightForSnitchMenu( ) == TRUE )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		// do not highlight current balance
		if( GetBoxShadeFlag( ghSnitchBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghSnitchBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghSnitchBox );
	}
}


void SnitchToggleMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		// do not highlight current balance
		if( GetBoxShadeFlag( ghSnitchToggleBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghSnitchToggleBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghSnitchToggleBox );
	}
}

void SnitchSectorMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string

		// do not highlight current balance
		if( GetBoxShadeFlag( ghSnitchSectorBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghSnitchSectorBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghSnitchSectorBox );
	}
}

void TrainingMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	BOOLEAN fCanTrainMilitia = TRUE;

	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN ) )
	{
		if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && !fShowMapInventoryPool )
		{
			UnMarkButtonDirty( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( fShowAttributeMenu )
		{
			// cancel attribute submenu
			fShowAttributeMenu = FALSE;
			// rerender tactical stuff
			gfRenderPBInterface = TRUE;

			return;
		}

		switch( iValue )
		{
			case( TRAIN_MENU_SELF):

				// practise in stat
				gbTrainingMode = TRAIN_SELF;

				// show menu
				fShowAttributeMenu = TRUE;
				DetermineBoxPositions( );

			break;
			case( TRAIN_MENU_TOWN):

				// Full test of Character and Sector to see if this training is possible at the moment.
				if( !BasicCanCharacterTrainMilitia(pSoldier) )
				{
					// No feedback. The menu options should be greyed out, anyway.
					break;
				}
				
				// Check for specific errors why this merc should not be able to train, 
				// and display a specific error message if one is encountered.
				if( !CanCharacterTrainMilitiaWithErrorReport(pSoldier) )
				{
					// Error found. Breaking. Note that the above function DOES display feedback if an error is
					// encountered at all.
					break;
				}

					// PASSED ALL THE TESTS - ALLOW SOLDIER TO TRAIN MILITIA HERE

					pSoldier->bOldAssignment = pSoldier->bAssignment;

					if( ( pSoldier->bAssignment != TRAIN_TOWN ) )
					{
						SetTimeOfAssignmentChangeForMerc( pSoldier );
					}

					MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

					// stop showing menu
				fShowAssignmentMenu = FALSE;
					giAssignHighLine = -1;

					// remove from squad

					if( pSoldier->bOldAssignment == VEHICLE )
					{
						TakeSoldierOutOfVehicle( pSoldier );
					}
					RemoveCharacterFromSquads(	pSoldier );

					ChangeSoldiersAssignment( pSoldier, TRAIN_TOWN );

					// assign to a movement group
					AssignMercToAMovementGroup( pSoldier );
					if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMilitiaTrainingPaid == FALSE )
					{
						// show a message to confirm player wants to charge cost
						HandleInterfaceMessageForCostOfTrainingMilitia( pSoldier );
					}
					else
					{
						SetAssignmentForList( TRAIN_TOWN, 0 );
					}
				gfRenderPBInterface = TRUE;
				break;

			// HEADROCK HAM 3.6: New separate Mobile Militia training.
			case( TRAIN_MENU_MOBILE ):

				// Full test of Character and Sector to see if this training is possible at the moment.
				if( !BasicCanCharacterTrainMobileMilitia(pSoldier) )
				{
					// No feedback. The menu options should be greyed out, anyway.
					break;
				}
				
				// Check for specific errors why this merc should not be able to train, 
				// and display a specific error message if one is encountered.
				if( !CanCharacterTrainMobileMilitiaWithErrorReport(pSoldier) )
				{
					// Error found. Breaking. Note that the above function DOES display feedback if an error is
					// encountered at all.
					break;
				}

				// PASSED BOTH TESTS - ALLOW SOLDIER TO TRAIN MILITIA HERE

				pSoldier->bOldAssignment = pSoldier->bAssignment;

				if( ( pSoldier->bAssignment != TRAIN_MOBILE ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

				// stop showing menu
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;

				// remove from squad

				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}
				RemoveCharacterFromSquads(	pSoldier );

				ChangeSoldiersAssignment( pSoldier, TRAIN_MOBILE );

				// assign to a movement group
				AssignMercToAMovementGroup( pSoldier );
				if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE )
				{
					// show a message to confirm player wants to charge cost
					HandleInterfaceMessageForCostOfTrainingMilitia( pSoldier );
				}
				else
				{
					SetAssignmentForList( TRAIN_MOBILE, 0 );
				}
				gfRenderPBInterface = TRUE;
				break;
			case( TRAIN_MENU_TEAMMATES):

				if( CanCharacterTrainTeammates( pSoldier ) == TRUE )
				{
					// train teammates
					gbTrainingMode = TRAIN_TEAMMATE;

					// show menu
					fShowAttributeMenu = TRUE;
					DetermineBoxPositions( );
				}
			break;
			case( TRAIN_MENU_TRAIN_BY_OTHER ):

				if( CanCharacterBeTrainedByOther( pSoldier ) == TRUE )
				{
					// train teammates
					gbTrainingMode = TRAIN_BY_OTHER;

					// show menu
					fShowAttributeMenu = TRUE;
					DetermineBoxPositions( );
				}
			break;
			case( TRAIN_MENU_CANCEL ):
				// stop showing menu
			fShowTrainingMenu = FALSE;

			// unhighlight the assignment box
			UnHighLightBox( ghAssignmentBox );

			// reset list
			ResetSelectedListForMapScreen( );
			gfRenderPBInterface = TRUE;

			break;
		}

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if( fShowAttributeMenu )
		{
			// cancel attribute submenu
			fShowAttributeMenu = FALSE;
			// rerender tactical stuff
			gfRenderPBInterface = TRUE;

			return;
		}
	}
}

void AttributesMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == ATTRIB_MENU_CANCEL )
		{
			// cancel, leave

			// stop showing menu
			fShowAttributeMenu = FALSE;

			// unhighlight the training box
			UnHighLightBox( ghTrainingBox );
		}
		else if( CanCharacterTrainStat( pSoldier, ( INT8 )( iValue ), ( BOOLEAN )( ( gbTrainingMode == TRAIN_SELF ) || ( gbTrainingMode == TRAIN_BY_OTHER ) ), ( BOOLEAN )( gbTrainingMode == TRAIN_TEAMMATE ) ) )
		{
			pSoldier->bOldAssignment = pSoldier->bAssignment;

			if( ( pSoldier->bAssignment != gbTrainingMode ) )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			// set stat to train
			pSoldier->bTrainStat = ( INT8 )iValue;

			MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

			// stop showing ALL menus
			fShowAssignmentMenu = FALSE;
			giAssignHighLine = -1;

			// remove from squad/vehicle
			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}
			RemoveCharacterFromSquads( pSoldier );

			// train stat
			ChangeSoldiersAssignment( pSoldier, gbTrainingMode );

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );

			// set assignment for group
			SetAssignmentForList( gbTrainingMode, ( INT8 )iValue );
		}

		// rerender tactical stuff
		gfRenderPBInterface = TRUE;

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}

};

void SnitchMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );

	if( ( fShowSnitchToggleMenu )||( fShowSnitchSectorMenu ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == SNITCH_MENU_CANCEL )
		{
			// cancel, leave

			// stop showing menu
			fShowSnitchMenu = FALSE;

			// unhighlight the training box
			UnHighLightBox( ghSnitchBox );

		}
		else if( iValue == SNITCH_MENU_TOGGLE )
		{
			if ( CanCharacterSnitch( pSoldier ) )
			{
				fShowSnitchToggleMenu = TRUE;
				DetermineBoxPositions( );
			}
		}
		else if( iValue == SNITCH_MENU_SECTOR)
		{
			if ( CanCharacterSnitch( pSoldier ) )
			{
				fShowSnitchSectorMenu = TRUE;
				DetermineBoxPositions( );
			}
		}
		// rerender tactical stuff
		gfRenderPBInterface = TRUE;

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
}

void SnitchToggleMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == SNITCH_MENU_TOGGLE_CANCEL )
		{
			// cancel, leave

			// stop showing menu
			fShowSnitchToggleMenu = FALSE;

			// unhighlight the training box
			UnHighLightBox( ghSnitchToggleBox );

		}
		else if( iValue == SNITCH_MENU_TOGGLE_ON )
		{
			if ( pSoldier->usSoldierFlagMask2 & SOLDIER_SNITCHING_OFF )
			{
				pSoldier->usSoldierFlagMask2 &= ~SOLDIER_SNITCHING_OFF;
				fShowSnitchToggleMenu = FALSE;
				fShowSnitchMenu = FALSE;
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;
			}
		}
		else if( iValue == SNITCH_MENU_TOGGLE_OFF )
		{
			if ( !(pSoldier->usSoldierFlagMask2 & SOLDIER_SNITCHING_OFF) )
			{
				pSoldier->usSoldierFlagMask2 |= SOLDIER_SNITCHING_OFF;
				fShowSnitchToggleMenu = FALSE;
				fShowSnitchMenu = FALSE;
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;
				
			}
		}
		else if( iValue == SNITCH_MENU_MISBEHAVIOUR_ON )
		{
			if ( pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF )
			{
				pSoldier->usSoldierFlagMask2 &= ~SOLDIER_PREVENT_MISBEHAVIOUR_OFF;
				fShowSnitchToggleMenu = FALSE;
				fShowSnitchMenu = FALSE;
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;
			}
		}
		else if( iValue == SNITCH_MENU_MISBEHAVIOUR_OFF)
		{
			if ( !(pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF) )
			{
				pSoldier->usSoldierFlagMask2 |= SOLDIER_PREVENT_MISBEHAVIOUR_OFF;
				fShowSnitchToggleMenu = FALSE;
				fShowSnitchMenu = FALSE;
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;
				
			}
		}
		// rerender tactical stuff
		gfRenderPBInterface = TRUE;

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
}

void SnitchSectorMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( iValue == SNITCH_MENU_SECTOR_CANCEL )
		{
			// cancel, leave

			// stop showing menu
			fShowSnitchSectorMenu = FALSE;

			// unhighlight the training box
			UnHighLightBox( ghSnitchSectorBox );

		}
		else if( iValue == SNITCH_MENU_SECTOR_PROPAGANDA )
		{
			if ( CanCharacterSpreadPropaganda( pSoldier ) )
			{
				fShowSnitchSectorMenu = FALSE;

				// stop showing menu
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;

				MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad

				if ( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}
				RemoveCharacterFromSquads(	pSoldier );
				ChangeSoldiersAssignment( pSoldier, SNITCH_SPREAD_PROPAGANDA );
				AssignMercToAMovementGroup( pSoldier );

				// set assignment for group
				SetAssignmentForList( ( INT8 ) SNITCH_SPREAD_PROPAGANDA, 0 );
			}
		}
		else if( iValue == SNITCH_MENU_SECTOR_GATHER_RUMOURS )
		{
			if ( CanCharacterGatherInformation( pSoldier ) )
			{
				fShowSnitchSectorMenu = FALSE;

				// stop showing menu
				fShowAssignmentMenu = FALSE;
				giAssignHighLine = -1;

				MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad

				if ( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}
				RemoveCharacterFromSquads(	pSoldier );
				ChangeSoldiersAssignment( pSoldier, SNITCH_GATHER_RUMOURS );
				AssignMercToAMovementGroup( pSoldier );

				// set assignment for group
				SetAssignmentForList( ( INT8 ) SNITCH_GATHER_RUMOURS, 0 );
			}
		}
		// rerender tactical stuff
		gfRenderPBInterface = TRUE;

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
}

void SetShowAllMenus( BOOLEAN fShowMenu )
{
	//fShowAssignmentMenu = fShowMenu;
	fShowSquadMenu = fShowMenu;
	fShowTrainingMenu = fShowMenu;
	fShowVehicleMenu = fShowMenu;
	fShowFacilityMenu = fShowMenu;
	fShowRepairMenu = fShowMenu;
	fShowMoveItemMenu = fShowMenu;
	fShowSnitchMenu = fShowMenu;
}

void AssignmentMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	CHAR16 sString[ 128 ];

	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// HEADROCK HAM 3.6: Added facility menu.
		if( ( fShowAttributeMenu )||( fShowTrainingMenu ) || ( fShowRepairMenu ) || fShowMoveItemMenu || ( fShowVehicleMenu ) ||( fShowSquadMenu ) || ( fShowFacilityMenu ) || ( fShowFacilityAssignmentMenu ) || ( fShowSnitchMenu ) )
		{
			return;
		}

		UnHighLightBox( ghAssignmentBox );

		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			switch( iValue )
			{
				case( EPC_MENU_ON_DUTY ):
					if( CanCharacterOnDuty( pSoldier ) )
						{
							// put character on a team
							fShowSquadMenu = TRUE;
							fShowTrainingMenu = FALSE;
							fShowVehicleMenu = FALSE;
							fShowFacilityMenu = FALSE; // HEADROCK HAM 3.6: Facility Menu
							fTeamPanelDirty = TRUE;
							fMapScreenBottomDirty = TRUE;

						}
				break;
				case( EPC_MENU_PATIENT ):
						// can character doctor?
					if( CanCharacterPatient( pSoldier ) )
					{

/* Assignment distance limits removed.	Sep/11/98.	ARM
						if( IsSoldierCloseEnoughToADoctor( pSoldier ) == FALSE )
						{
							return;
						}
*/

						pSoldier->bOldAssignment = pSoldier->bAssignment;

						if( ( pSoldier->bAssignment != PATIENT ) )
						{
							SetTimeOfAssignmentChangeForMerc( pSoldier );
						}

						// stop showing menu
						fShowAssignmentMenu = FALSE;
						giAssignHighLine = -1;

						MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						// remove from squad

						if ( pSoldier->bOldAssignment == VEHICLE )
						{
							TakeSoldierOutOfVehicle( pSoldier );
						}
						RemoveCharacterFromSquads(	pSoldier );
						ChangeSoldiersAssignment( pSoldier, PATIENT );
						AssignMercToAMovementGroup( pSoldier );

						// set assignment for group
						SetAssignmentForList( ( INT8 ) PATIENT, 0 );
						
						/////////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - added check for surgery
						if( pSoldier->iHealableInjury >= 100 && gGameOptions.fNewTraitSystem ) // if we can heal at least one life point
						{
							SOLDIERTYPE * pMedic = NULL;
							SOLDIERTYPE * pBestMedic = NULL;
							UINT8 cnt;
							INT8 bSlot;

							// Find the best doctor
							cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
							for ( pMedic = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pMedic++)
							{
								if ( !(pMedic->bActive) || !(pMedic->bInSector) || ( pMedic->flags.uiStatusFlags & SOLDIER_VEHICLE ) || (pMedic->bAssignment == VEHICLE ) )
									continue; // is nowhere around!

								if (( pSoldier->ubID == pMedic->ubID ) || ( pMedic->bAssignment != DOCTOR ))
									continue; // cannot make surgery on self or not on the right assignment!	

								bSlot = FindMedKit( pMedic );
								if (bSlot == NO_SLOT)
									continue;// no medical kit!

								if (pMedic->stats.bLife > OKLIFE && !(pMedic->bCollapsed) && pMedic->stats.bMedical > 0 && ( NUM_SKILL_TRAITS( pMedic, DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery ))
								{
									if (pBestMedic != NULL)
									{
										if (NUM_SKILL_TRAITS( pMedic, DOCTOR_NT ) > NUM_SKILL_TRAITS( pBestMedic, DOCTOR_NT ))
											pBestMedic = pMedic;
									}
									else
									{
										pBestMedic = pMedic;
									}
								}
							}
							if (pBestMedic != NULL)
							{
								CHAR16	zStr[200];
								pAutomaticSurgeryDoctor = pBestMedic;
								pAutomaticSurgeryPatient = pSoldier;
								swprintf( zStr, New113Message[ MSG113_SURGERY_BEFORE_PATIENT_ASSIGNMENT ] );

								DoMapMessageBox( MSG_BOX_BASIC_STYLE, zStr, MAP_SCREEN, MSG_BOX_FLAG_YESNO, SurgeryBeforePatientingRequesterCallback );
							}
						}
						/////////////////////////////////////////////////////////////////////////////////////////
					}
				break;

				case( EPC_MENU_VEHICLE ):
					if ( CanCharacterVehicle( pSoldier ) )
					{
						if( DisplayVehicleMenu( pSoldier ) )
						{
							fShowVehicleMenu = TRUE;
							ShowBox( ghVehicleBox );
						}
						else
						{
							fShowVehicleMenu = FALSE;
						}
					}
				break;

				case( EPC_MENU_REMOVE ):

					fShowAssignmentMenu = FALSE;
			UnEscortEPC( pSoldier );
				break;

				case( EPC_MENU_CANCEL ):
					fShowAssignmentMenu = FALSE;
					giAssignHighLine = -1;

					// set dirty flag
					fTeamPanelDirty = TRUE;
					fMapScreenBottomDirty = TRUE;

					// reset list of characters
					ResetSelectedListForMapScreen( );
				break;
			}
		}
		else
		{
			switch( iValue )
			{
				case( ASSIGN_MENU_ON_DUTY ):
						if( CanCharacterOnDuty( pSoldier ) )
						{
							// put character on a team
							fShowSquadMenu = TRUE;
							fShowTrainingMenu = FALSE;
							fShowVehicleMenu = FALSE;
							fShowFacilityMenu = FALSE; // HEADROCK HAM 3.6: Facility Menu
							fTeamPanelDirty = TRUE;
							fMapScreenBottomDirty = TRUE;
							fShowRepairMenu = FALSE;
							fShowMoveItemMenu = FALSE;
						}
				break;
				case( ASSIGN_MENU_DOCTOR ):

					// can character doctor?
					if( CanCharacterDoctor( pSoldier ) )
					{
						// stop showing menu
						fShowAssignmentMenu = FALSE;
						giAssignHighLine = -1;


						pSoldier->bOldAssignment = pSoldier->bAssignment;

						if( ( pSoldier->bAssignment != DOCTOR ) )
						{
							SetTimeOfAssignmentChangeForMerc( pSoldier );
						}

						// remove from squad

						if( pSoldier->bOldAssignment == VEHICLE )
						{
							TakeSoldierOutOfVehicle( pSoldier );
						}
						RemoveCharacterFromSquads(	pSoldier );

						ChangeSoldiersAssignment( pSoldier, DOCTOR );

						MakeSureMedKitIsInHand( pSoldier );
						AssignMercToAMovementGroup( pSoldier );

						MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						// set assignment for group
						SetAssignmentForList( ( INT8 ) DOCTOR, 0 );

						///////////////////////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - added check for surgery
						if( gGameOptions.fNewTraitSystem && ( GetNumberThatCanBeDoctored( pSoldier, HEALABLE_EVER, FALSE, FALSE, TRUE ) > 0 ) && 
							( NUM_SKILL_TRAITS( pSoldier, DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery ) )
						{
							CHAR16	zStr[200];
							pAutomaticSurgeryDoctor = pSoldier;
							swprintf( zStr, New113Message[ MSG113_SURGERY_BEFORE_DOCTOR_ASSIGNMENT ], GetNumberThatCanBeDoctored( pSoldier, HEALABLE_EVER, TRUE, TRUE, TRUE ) );

							DoMapMessageBox( MSG_BOX_BASIC_STYLE, zStr, MAP_SCREEN, MSG_BOX_FLAG_YESNO, SurgeryBeforeDoctoringRequesterCallback );
						}
						///////////////////////////////////////////////////////////////////////////////////////////////////////
					}
					else if( CanCharacterDoctorButDoesntHaveMedKit( pSoldier ) )
					{
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
						swprintf( sString, zMarksMapScreenText[ 19 ], pSoldier->GetName() );

						DoScreenIndependantMessageBox( sString , MSG_BOX_FLAG_OK, NULL );
					}

				break;
				case( ASSIGN_MENU_PATIENT ):

					// can character patient?
					if( CanCharacterPatient( pSoldier ) )
					{

/* Assignment distance limits removed.	Sep/11/98.	ARM
						if( IsSoldierCloseEnoughToADoctor( pSoldier ) == FALSE )
						{
							return;
						}
*/

						pSoldier->bOldAssignment = pSoldier->bAssignment;

						if( ( pSoldier->bAssignment != PATIENT ) )
						{
							SetTimeOfAssignmentChangeForMerc( pSoldier );
						}

						MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

						// stop showing menu
						fShowAssignmentMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						// remove from squad

						if( pSoldier->bOldAssignment == VEHICLE )
						{
							TakeSoldierOutOfVehicle( pSoldier );
						}
						RemoveCharacterFromSquads(	pSoldier );
						ChangeSoldiersAssignment( pSoldier, PATIENT );

						AssignMercToAMovementGroup( pSoldier );

						// set assignment for group
						SetAssignmentForList( ( INT8 ) PATIENT, 0 );

						/////////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - added check for surgery
						if( pSoldier->iHealableInjury >= 100 && gGameOptions.fNewTraitSystem ) // if we can heal at least one life point
						{
							SOLDIERTYPE * pMedic = NULL;
							SOLDIERTYPE * pBestMedic = NULL;
							UINT8 cnt;
							INT8 bSlot;

							// Find the best doctor
							cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
							for ( pMedic = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pMedic++)
							{
								if ( !(pMedic->bActive) || !(pMedic->bInSector) || ( pMedic->flags.uiStatusFlags & SOLDIER_VEHICLE ) || (pMedic->bAssignment == VEHICLE ) )
									continue; // is nowhere around!

								if (( pSoldier->ubID == pMedic->ubID ) || ( pMedic->bAssignment != DOCTOR ))
									continue; // cannot make surgery on self or not on the right assignment!

								bSlot = FindMedKit( pMedic );
								if (bSlot == NO_SLOT)
									continue;// no medical kit!

								if (pMedic->stats.bLife > OKLIFE && !(pMedic->bCollapsed) && pMedic->stats.bMedical > 0 && ( NUM_SKILL_TRAITS( pMedic, DOCTOR_NT ) >= gSkillTraitValues.ubDONumberTraitsNeededForSurgery ))
								{
									if (pBestMedic != NULL)
									{
										if (NUM_SKILL_TRAITS( pMedic, DOCTOR_NT ) > NUM_SKILL_TRAITS( pBestMedic, DOCTOR_NT ))
											pBestMedic = pMedic;
									}
									else
									{
										pBestMedic = pMedic;
									}
								}
							}
							if (pBestMedic != NULL)
							{
								CHAR16	zStr[200];
								pAutomaticSurgeryDoctor = pBestMedic;
								pAutomaticSurgeryPatient = pSoldier;
								swprintf( zStr, New113Message[ MSG113_SURGERY_BEFORE_PATIENT_ASSIGNMENT ] );

								DoMapMessageBox( MSG_BOX_BASIC_STYLE, zStr, MAP_SCREEN, MSG_BOX_FLAG_YESNO, SurgeryBeforePatientingRequesterCallback );
							}
						}
						/////////////////////////////////////////////////////////////////////////////////////////
					}
				break;

				case( ASSIGN_MENU_VEHICLE ):
					if ( CanCharacterVehicle( pSoldier ) )
					{
						if( DisplayVehicleMenu( pSoldier ) )
						{
							fShowVehicleMenu = TRUE;
							ShowBox( ghVehicleBox );
						}
						else
						{
							fShowVehicleMenu = FALSE;
						}
					}
				break;
				case( ASSIGN_MENU_REPAIR ):
					if( CanCharacterRepair( pSoldier ) )
					{

						fShowSquadMenu = FALSE;
						fShowTrainingMenu = FALSE;
						fShowVehicleMenu = FALSE;
						fShowFacilityMenu = FALSE; // HEADROCK HAM 3.6: Facility Menu
						fShowMoveItemMenu = FALSE;
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->bOldAssignment = pSoldier->bAssignment;


						if( pSoldier->bSectorZ ==0 )
						{
							fShowRepairMenu = FALSE;

							if( DisplayRepairMenu( pSoldier ) )
							{
								fShowRepairMenu = TRUE;
								DetermineBoxPositions( );
							}
						}

					}
					else if( CanCharacterRepairButDoesntHaveARepairkit( pSoldier ) )
					{
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
						swprintf( sString, zMarksMapScreenText[ 18 ], pSoldier->GetName() );

						DoScreenIndependantMessageBox( sString , MSG_BOX_FLAG_OK, NULL );
					}
					break;
				case( ASSIGN_MENU_RADIO_SCAN ):
					if( pSoldier->CanUseRadio() )
					{
						// stop showing menu
						fShowAssignmentMenu = FALSE;
						giAssignHighLine = -1;

						pSoldier->bOldAssignment = pSoldier->bAssignment;

						if( ( pSoldier->bAssignment != RADIO_SCAN ) )
						{
							SetTimeOfAssignmentChangeForMerc( pSoldier );
						}

						// remove from squad
						if( pSoldier->bOldAssignment == VEHICLE )
						{
							TakeSoldierOutOfVehicle( pSoldier );
						}
						RemoveCharacterFromSquads(	pSoldier );

						ChangeSoldiersAssignment( pSoldier, RADIO_SCAN );

						AssignMercToAMovementGroup( pSoldier );

						MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						// set assignment for group
						SetAssignmentForList( ( INT8 ) RADIO_SCAN, 0 );
					}
					break;
				case( ASSIGN_MENU_SNITCH ):
					if ( CanCharacterSnitch( pSoldier ) )
					{
						//SetShowAllMenus(FALSE);

						//DisplaySnitchMenu( pSoldier );
						fShowSnitchMenu = TRUE;
						//ShowBox( ghSnitchBox );
						DetermineBoxPositions( );
						fShowSquadMenu = FALSE;
						fShowVehicleMenu = FALSE;
						fShowRepairMenu = FALSE;
						fShowTrainingMenu = FALSE;
						fShowMoveItemMenu = FALSE;
						fShowFacilityMenu = FALSE;

						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;
				case( ASSIGN_MENU_TRAIN ):
					if( CanCharacterPractise( pSoldier ) )
					{
						fShowTrainingMenu = TRUE;
						DetermineBoxPositions( );
						fShowSquadMenu = FALSE;
						fShowVehicleMenu = FALSE;
						fShowRepairMenu = FALSE;
						fShowMoveItemMenu = FALSE;
						fShowFacilityMenu = FALSE; // HEADROCK HAM 3.6: Facility Menu
						fShowSnitchMenu = FALSE;

						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
				break;

				case( ASSIGN_MENU_MOVE_ITEMS ):
					if( 1 )
					{

						fShowSquadMenu = FALSE;
						fShowTrainingMenu = FALSE;
						fShowVehicleMenu = FALSE;
						fShowFacilityMenu = FALSE; // HEADROCK HAM 3.6: Facility Menu
						//fShownAssignmentMenu = FALSE;
						fShowRepairMenu = FALSE;
						fShownContractMenu = FALSE;
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->bOldAssignment = pSoldier->bAssignment;

						if( pSoldier->bSectorZ == 0 )
						{
							fShowMoveItemMenu = FALSE;

							if( DisplayMoveItemsMenu( pSoldier ) )
							{
								fShowMoveItemMenu = TRUE;
								DetermineBoxPositions( );
							}
						}
					}
					else if( 0 )
					{
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
						swprintf( sString, zMarksMapScreenText[ 18 ], pSoldier->GetName() );

						DoScreenIndependantMessageBox( sString , MSG_BOX_FLAG_OK, NULL );
					}
					break;

				// HEADROCK HAM 3.6: New assignments for Facility operation.
				case( ASSIGN_MENU_FACILITY ):
					if ( BasicCanCharacterFacility( pSoldier ) )
					{
						// put character on a team
						fShowSquadMenu = FALSE;
						fShowTrainingMenu = FALSE;
						fShowVehicleMenu = FALSE;
						fShowRepairMenu = FALSE;
						fShowMoveItemMenu = FALSE;
						fShowFacilityMenu = TRUE; // HEADROCK HAM 3.6: Facility Menu
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						if( DisplayFacilityMenu( pSoldier ) )
						{
							fShowFacilityMenu = TRUE;
							ShowBox( ghFacilityBox );
						}
						else
						{
							fShowFacilityMenu = FALSE;
						}
					}
				break;
				case( ASSIGN_MENU_CANCEL ):
					fShowAssignmentMenu = FALSE;
					giAssignHighLine = -1;

					// set dirty flag
					fTeamPanelDirty = TRUE;
					fMapScreenBottomDirty = TRUE;

					// reset list of characters
					ResetSelectedListForMapScreen( );
				break;
			}
		}
		gfRenderPBInterface = TRUE;

	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		// HEADROCK HAM 3.6: Added facility menu
		if( ( fShowAttributeMenu )||( fShowTrainingMenu ) || ( fShowRepairMenu ) || fShowMoveItemMenu || ( fShowVehicleMenu ) ||( fShowSquadMenu ) || ( fShowFacilityMenu ) || ( fShowFacilityAssignmentMenu ) || ( fShowSnitchMenu ) )
		{
			fShowAttributeMenu = FALSE;
			fShowTrainingMenu = FALSE;
			fShowRepairMenu = FALSE;
			fShowMoveItemMenu = FALSE;
			fShowVehicleMenu = FALSE;
			fShowSquadMenu = FALSE;
			fShowFacilityMenu = FALSE; // Added facilities
			fShowFacilityAssignmentMenu = FALSE; // Sub-menu for facilities

			// rerender tactical stuff
			gfRenderPBInterface = TRUE;

			// set dirty flag
	 fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;

			return;
		}
	}
}



void RestorePopUpBoxes( void )
{
	ContractPosition.iX = OrigContractPosition.iX;
	AttributePosition.iX = OrigAttributePosition.iX;
	SquadPosition.iX = OrigSquadPosition.iX ;
	AssignmentPosition.iX = OrigAssignmentPosition.iX ;
	TrainPosition.iX = OrigTrainPosition.iX;
	VehiclePosition.iX = OrigVehiclePosition.iX;
	FacilityPosition.iX = OrigFacilityPosition.iX;
	FacilityAssignmentPosition.iX = OrigFacilityAssignmentPosition.iX;

	return;
}


void CreateSquadBox( void )
{
	// will create a pop up box for squad selection
	SGPPoint pPoint;
	SGPRect pDimensions;
	UINT32 hStringHandle;
	UINT32 uiCounter;
	CHAR16 sString[ 64 ];
	UINT32 uiMaxSquad;


 // create basic box
 CreatePopUpBox(&ghSquadBox, SquadDimensions, SquadPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_RESIZE ));

 // which buffer will box render to
 SetBoxBuffer(ghSquadBox, FRAME_BUFFER);

 // border type?
 SetBorderType(ghSquadBox,guiPOPUPBORDERS);

 // background texture
 SetBackGroundSurface(ghSquadBox, guiPOPUPTEX);

 // margin sizes
 SetMargins( ghSquadBox, 6, 6, 4, 4 );

 // space between lines
 SetLineSpace(ghSquadBox, 2);

 // set current box to this one
 SetCurrentBox( ghSquadBox );


 uiMaxSquad = GetLastSquadListedInSquadMenu();

 // add strings for box
 for(uiCounter=0; uiCounter <= uiMaxSquad; uiCounter++)
 {
	 // get info about current squad and put in	string
	 //SQUAD10 FIX
	 if ( gGameExternalOptions.fUseXMLSquadNames )
		swprintf( sString, L"%s ( %d/%d )", SquadNames[ uiCounter ].squadname, NumberOfPeopleInSquad( ( INT8 )uiCounter ), gGameOptions.ubSquadSize );
	 else
		swprintf( sString, L"%s ( %d/%d )", pSquadMenuStrings[uiCounter], NumberOfPeopleInSquad( ( INT8 )uiCounter ), gGameOptions.ubSquadSize );

	 AddMonoString(&hStringHandle, sString );

	 // make sure it is unhighlighted
	 UnHighLightLine(hStringHandle);
 }

 // add cancel line
 AddMonoString(&hStringHandle, pSquadMenuStrings[ NUMBER_OF_SQUADS ]);

 // set font type
 SetBoxFont(ghSquadBox, MAP_SCREEN_FONT);

 // set highlight color
 SetBoxHighLight(ghSquadBox, FONT_WHITE);

 // unhighlighted color
 SetBoxForeground(ghSquadBox, FONT_LTGREEN);

 // the secondary shade color
 SetBoxSecondaryShade( ghSquadBox, FONT_YELLOW );

 // background color
 SetBoxBackground(ghSquadBox, FONT_BLACK);

 // shaded color..for darkened text
 SetBoxShade( ghSquadBox, FONT_GRAY7 );

 // resize box to text
 ResizeBoxToText( ghSquadBox );

 DetermineBoxPositions( );

 GetBoxPosition( ghSquadBox, &pPoint);
 GetBoxSize( ghSquadBox, &pDimensions );

 if( giBoxY + pDimensions.iBottom > 479 )
 {
		pPoint.iY = SquadPosition.iY = 479 - pDimensions.iBottom;
	}

	SetBoxPosition( ghSquadBox, pPoint );
}

void CreateEPCBox( void )
{
	// will create a pop up box for squad selection
	SGPPoint pPoint;
	SGPRect pDimensions;
	UINT32 hStringHandle;
	INT32 iCount;

	// create basic box
	CreatePopUpBox(&ghEpcBox, SquadDimensions, AssignmentPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_RESIZE|POPUP_BOX_FLAG_CENTER_TEXT ));

	// which buffer will box render to
	SetBoxBuffer(ghEpcBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghEpcBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghEpcBox, guiPOPUPTEX);

	// margin sizes
	SetMargins( ghEpcBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghEpcBox, 2);

	// set current box to this one
	SetCurrentBox( ghEpcBox );

	for(iCount=0; iCount < MAX_EPC_MENU_STRING_COUNT; iCount++)
	{
		AddMonoString(&hStringHandle, pEpcMenuStrings[ iCount]);
	}

	// set font type
	SetBoxFont(ghEpcBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghEpcBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghEpcBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghEpcBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghEpcBox, FONT_GRAY7 );

	// resize box to text
	ResizeBoxToText( ghEpcBox );

	GetBoxPosition( ghEpcBox, &pPoint);

	GetBoxSize( ghEpcBox, &pDimensions );

	if( giBoxY + pDimensions.iBottom > 479 )
	{
			pPoint.iY = AssignmentPosition.iY = 479 - pDimensions.iBottom;
	}

		SetBoxPosition( ghEpcBox, pPoint );
}



void HandleShadingOfLinesForSquadMenu( void )
{
	// find current squad and set that line the squad box a lighter green
	UINT32 uiCounter;
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiMaxSquad;
	INT8 bResult = 0;


	if ( ( fShowSquadMenu == FALSE ) || ( ghSquadBox == -1 ) )
	{
		return;
	}


	pSoldier = GetSelectedAssignSoldier( FALSE );

	uiMaxSquad = GetLastSquadListedInSquadMenu();

	for( uiCounter = 0; uiCounter <= uiMaxSquad; uiCounter++ )
	{
		if ( pSoldier != NULL )
		{
			bResult = CanCharacterSquad( pSoldier, (INT8) uiCounter );
		}

		// if no soldier, or a reason which doesn't have a good explanatory message
		if ( ( pSoldier == NULL ) || ( bResult == CHARACTER_CANT_JOIN_SQUAD ) )
		{
			// darken /disable line
			ShadeStringInBox( ghSquadBox, uiCounter );
		}
		else
		{
			if ( bResult == CHARACTER_CAN_JOIN_SQUAD )
			{
				// legal squad, leave it green
				UnShadeStringInBox( ghSquadBox, uiCounter );
				UnSecondaryShadeStringInBox( ghSquadBox, uiCounter );
			}
			else
			{
				// unjoinable squad - yellow
				SecondaryShadeStringInBox( ghSquadBox, uiCounter );
			}
		}
	}
}



void PostContractMessage( SOLDIERTYPE *pCharacter, INT32 iContract )
{

	// do nothing
	return;

	// send a message stating that offer of contract extension made
	//MapScreenMessage(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Offered to extend %s's contract by another %s.", pCharacter->GetName(), pContractExtendStrings[ iContract ] );

	return;
}

void PostTerminateMessage( SOLDIERTYPE *pCharacter )
{

	// do nothing
	return;

	// send a message stating that termination of contract done
	//MapScreenMessage(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s's contract has been terminated.", pCharacter->GetName() );

}

BOOLEAN DisplayVehicleMenu( SOLDIERTYPE *pSoldier )
{
	BOOLEAN fVehiclePresent=FALSE;
	INT32 iCounter=0;
	INT32 hStringHandle=0;

	// first, clear pop up box
	RemoveBox(ghVehicleBox);
	ghVehicleBox = -1;

	CreateVehicleBox();
	SetCurrentBox(ghVehicleBox);

	// run through list of vehicles in sector and add them to pop up box
	for ( iCounter = 0; iCounter < ubNumberOfVehicles; iCounter++ )
	{
		if ( pVehicleList[iCounter].fValid == TRUE )
		{
			if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounter ) )
			{
				//AddMonoString((UINT32 *)&hStringHandle, pVehicleStrings[ pVehicleList[ iCounter ].ubVehicleType ]);
					AddMonoString((UINT32 *)&hStringHandle, gNewVehicle[ pVehicleList[ iCounter ].ubVehicleType ].NewVehicleStrings);
				fVehiclePresent = TRUE;
			}
		}
	}

	// cancel string (borrow the one in the squad menu)
	AddMonoString((UINT32 *)&hStringHandle, pSquadMenuStrings[ SQUAD_MENU_CANCEL ]);

	SetBoxFont(ghVehicleBox, MAP_SCREEN_FONT);
	SetBoxHighLight(ghVehicleBox, FONT_WHITE);
	SetBoxForeground(ghVehicleBox, FONT_LTGREEN);
	SetBoxBackground(ghVehicleBox, FONT_BLACK);

	return fVehiclePresent;
}

void CreateVehicleBox()
{
	 CreatePopUpBox(&ghVehicleBox, VehicleDimensions, VehiclePosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
	 SetBoxBuffer(ghVehicleBox, FRAME_BUFFER);
	 SetBorderType(ghVehicleBox,guiPOPUPBORDERS);
	 SetBackGroundSurface(ghVehicleBox, guiPOPUPTEX);
	 SetMargins( ghVehicleBox, 6, 6, 4, 4 );
	 SetLineSpace(ghVehicleBox, 2);
}


void CreateRepairBox()
{
	CreatePopUpBox(&ghRepairBox, RepairDimensions, RepairPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
	SetBoxBuffer(ghRepairBox, FRAME_BUFFER);
	SetBorderType(ghRepairBox,guiPOPUPBORDERS);
	SetBackGroundSurface(ghRepairBox, guiPOPUPTEX);
	SetMargins( ghRepairBox, 6, 6, 4, 4 );
	SetLineSpace(ghRepairBox, 2);
}

void CreateMoveItemBox()
{
	// will create a pop up box for squad selection
	SGPPoint pPoint;
	SGPRect pDimensions;

	CreatePopUpBox(&ghMoveItemBox, FacilityAssignmentDimensions, FacilityAssignmentPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
	SetBoxBuffer(ghMoveItemBox, FRAME_BUFFER);
	SetBorderType(ghMoveItemBox,guiPOPUPBORDERS);
	SetBackGroundSurface(ghMoveItemBox, guiPOPUPTEX);
	SetMargins( ghMoveItemBox, 6, 6, 4, 4 );
	SetLineSpace(ghMoveItemBox, 2);

	// set current box to this one
	SetCurrentBox( ghMoveItemBox );

	// resize box to text
	ResizeBoxToText( ghMoveItemBox );

	DetermineBoxPositions( );

	GetBoxPosition( ghMoveItemBox, &pPoint);
	GetBoxSize( ghMoveItemBox, &pDimensions );

	if( giBoxY + pDimensions.iBottom > 479 )
	{
		pPoint.iY = FacilityAssignmentPosition.iY = 479 - pDimensions.iBottom;
	}
	
	SetBoxPosition( ghMoveItemBox, pPoint );
}

void CreateSnitchBox()
{
	UINT32 hStringHandle;
	UINT32 uiCounter;

	// will create attribute pop up menu for mapscreen assignments
	SnitchPosition.iX = OrigSnitchPosition.iX;

	if( giBoxY != 0 )
	{
		SnitchPosition.iY = giBoxY + ( ASSIGN_MENU_SNITCH * GetFontHeight( MAP_SCREEN_FONT ) );
	}

	// create basic box
	CreatePopUpBox(&ghSnitchBox, SnitchDimensions, SnitchPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghSnitchBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghSnitchBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghSnitchBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghSnitchBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghSnitchBox, 2);

	// set current box to this one
	SetCurrentBox( ghSnitchBox );


	// add strings for box
	for(uiCounter=0; uiCounter < MAX_SNITCH_MENU_STRING_COUNT; uiCounter++)
	{
		AddMonoString(&hStringHandle, pSnitchMenuStrings[uiCounter]);

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghSnitchBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghSnitchBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghSnitchBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghSnitchBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghSnitchBox, FONT_GRAY7 );

	// resize box to text
	ResizeBoxToText( ghSnitchBox );

	DetermineBoxPositions( );
}

void CreateSnitchToggleBox()
{
	UINT32 hStringHandle;
	UINT32 uiCounter;

	// will create attribute pop up menu for mapscreen assignments
	SnitchTogglePosition.iX = OrigSnitchTogglePosition.iX;

	if( giBoxY != 0 )
	{
		SnitchTogglePosition.iY = giBoxY + ( SNITCH_MENU_TOGGLE * GetFontHeight( MAP_SCREEN_FONT ) );
	}

	// create basic box
	CreatePopUpBox(&ghSnitchToggleBox, SnitchToggleDimensions, SnitchTogglePosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghSnitchToggleBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghSnitchToggleBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghSnitchToggleBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghSnitchToggleBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghSnitchToggleBox, 2);

	// set current box to this one
	SetCurrentBox( ghSnitchToggleBox );


	// add strings for box
	for(uiCounter=0; uiCounter < MAX_SNITCH_TOGGLE_MENU_STRING_COUNT; uiCounter++)
	{
		AddMonoString(&hStringHandle, pSnitchToggleMenuStrings[uiCounter]);

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghSnitchToggleBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghSnitchToggleBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghSnitchToggleBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghSnitchToggleBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghSnitchToggleBox, FONT_GRAY7 );

	// resize box to text
	ResizeBoxToText( ghSnitchToggleBox );

	DetermineBoxPositions( );
}

void CreateSnitchSectorBox()
{
	UINT32 hStringHandle;
	UINT32 uiCounter;

	// will create attribute pop up menu for mapscreen assignments

	SnitchSectorPosition.iX = OrigSnitchSectorPosition.iX;

	if( giBoxY != 0 )
	{
		SnitchSectorPosition.iY = giBoxY + ( SNITCH_MENU_SECTOR * GetFontHeight( MAP_SCREEN_FONT ) );
	}
	// create basic box
	CreatePopUpBox(&ghSnitchSectorBox, SnitchSectorDimensions, SnitchSectorPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghSnitchSectorBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghSnitchSectorBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghSnitchSectorBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghSnitchSectorBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghSnitchSectorBox, 2);

	// set current box to this one
	SetCurrentBox( ghSnitchSectorBox );


	// add strings for box
	for(uiCounter=0; uiCounter < MAX_SNITCH_SECTOR_MENU_STRING_COUNT; uiCounter++)
	{
		AddMonoString(&hStringHandle, pSnitchSectorMenuStrings[uiCounter]);

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghSnitchSectorBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghSnitchSectorBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghSnitchSectorBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghSnitchSectorBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghSnitchSectorBox, FONT_GRAY7 );

	// resize box to text
	ResizeBoxToText( ghSnitchSectorBox );

	DetermineBoxPositions( );
}

void CreateContractBox( SOLDIERTYPE *pCharacter )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;
 CHAR16 sString[ 50 ];
 CHAR16 sDollarString[ 50 ];

 ContractPosition.iX = (SCREEN_WIDTH - INTERFACE_WIDTH)/2 + OrigContractPosition.iX;

 if( giBoxY != 0 )
 {
	ContractPosition.iX = giBoxY;
 }

 CreatePopUpBox(&ghContractBox, ContractDimensions, ContractPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_RESIZE ));
 SetBoxBuffer(ghContractBox, FRAME_BUFFER);
 SetBorderType(ghContractBox,guiPOPUPBORDERS);
 SetBackGroundSurface(ghContractBox, guiPOPUPTEX);
 SetMargins( ghContractBox, 6, 6, 4, 4 );
 SetLineSpace(ghContractBox, 2);

 // set current box to this one
 SetCurrentBox( ghContractBox );

 // not null character?
 if( pCharacter != NULL )
 {
	for(uiCounter=0; uiCounter < MAX_CONTRACT_MENU_STRING_COUNT; uiCounter++)
	{
		switch( uiCounter )
		{
			case( CONTRACT_MENU_CURRENT_FUNDS ):
/*
				// add current balance after title string
				swprintf( sDollarString, L"%d", LaptopSaveInfo.iCurrentBalance);
				InsertCommasForDollarFigure( sDollarString );
				InsertDollarSignInToString( sDollarString );
				swprintf( sString, L"%s %s",	pContractStrings[uiCounter], sDollarString );
				AddMonoString(&hStringHandle, sString);
*/
				AddMonoString(&hStringHandle, pContractStrings[uiCounter]);
			break;
			case( CONTRACT_MENU_DAY ):

				if( pCharacter->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
				{
					swprintf( sDollarString, L"%d", 0 );
				}
				else
				{
					swprintf( sDollarString, L"%d", gMercProfiles[ pCharacter->ubProfile ].sSalary );
				}
				InsertCommasForDollarFigure( sDollarString );
				InsertDollarSignInToString( sDollarString );
				swprintf( sString, L"%s ( %s )",	pContractStrings[uiCounter], sDollarString);
				AddMonoString(&hStringHandle, sString);
			break;
			case( CONTRACT_MENU_WEEK ):

				if( pCharacter->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
				{
					swprintf( sDollarString, L"%d", 0 );
				}
				else
				{
					swprintf( sDollarString, L"%d", gMercProfiles[ pCharacter->ubProfile ].uiWeeklySalary );
				}

				InsertCommasForDollarFigure( sDollarString );
				InsertDollarSignInToString( sDollarString );
				swprintf( sString, L"%s ( %s )",	pContractStrings[uiCounter], sDollarString );
				AddMonoString(&hStringHandle, sString);
			break;
			case( CONTRACT_MENU_TWO_WEEKS ):

				if( pCharacter->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
				{
					swprintf( sDollarString, L"%d", 0 );
				}
				else
				{
					swprintf( sDollarString, L"%d", gMercProfiles[ pCharacter->ubProfile ].uiBiWeeklySalary );
				}


				InsertCommasForDollarFigure( sDollarString );
				InsertDollarSignInToString( sDollarString );
				swprintf( sString, L"%s ( %s )",	pContractStrings[uiCounter], sDollarString);
				AddMonoString(&hStringHandle, sString);
			break;
			default:
				AddMonoString(&hStringHandle, pContractStrings[uiCounter] );
				break;
		}
		UnHighLightLine(hStringHandle);
	}
 }


 SetBoxFont(ghContractBox, MAP_SCREEN_FONT);
 SetBoxHighLight(ghContractBox, FONT_WHITE);
 SetBoxForeground(ghContractBox, FONT_LTGREEN);
 SetBoxBackground(ghContractBox, FONT_BLACK);

 // shaded color..for darkened text
 SetBoxShade( ghContractBox, FONT_GRAY7 );

 if( pCharacter != NULL )
 {
	// now set the color for the current balance value
	SetBoxLineForeground( ghContractBox, 0, FONT_YELLOW );
 }

 // resize box to text
 ResizeBoxToText( ghContractBox );

}

void CreateAttributeBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;

 // will create attribute pop up menu for mapscreen assignments


 AttributePosition.iX = OrigAttributePosition.iX;

 if( giBoxY != 0 )
 {
	AttributePosition.iY = giBoxY;
 }

 // update screen assignment positions
 UpdateMapScreenAssignmentPositions( );

 // create basic box
 CreatePopUpBox(&ghAttributeBox, AttributeDimensions, AttributePosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

 // which buffer will box render to
 SetBoxBuffer(ghAttributeBox, FRAME_BUFFER);

 // border type?
 SetBorderType(ghAttributeBox,guiPOPUPBORDERS);

 // background texture
 SetBackGroundSurface(ghAttributeBox, guiPOPUPTEX);

 // margin sizes
 SetMargins( ghAttributeBox, 6, 6, 4, 4 );

 // space between lines
 SetLineSpace(ghAttributeBox, 2);

 // set current box to this one
 SetCurrentBox( ghAttributeBox );



 // add strings for box
 for(uiCounter=0; uiCounter < MAX_ATTRIBUTE_STRING_COUNT; uiCounter++)
 {
	AddMonoString(&hStringHandle, pAttributeMenuStrings[uiCounter]);

	// make sure it is unhighlighted
	UnHighLightLine(hStringHandle);
 }

 // set font type
 SetBoxFont(ghAttributeBox, MAP_SCREEN_FONT);

 // set highlight color
 SetBoxHighLight(ghAttributeBox, FONT_WHITE);

 // unhighlighted color
 SetBoxForeground(ghAttributeBox, FONT_LTGREEN);

 // background color
 SetBoxBackground(ghAttributeBox, FONT_BLACK);

 // shaded color..for darkened text
 SetBoxShade( ghAttributeBox, FONT_GRAY7 );

 // resize box to text
 ResizeBoxToText( ghAttributeBox );


}

void CreateTrainingBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;

 // will create attribute pop up menu for mapscreen assignments

 TrainPosition.iX = OrigTrainPosition.iX;

 if( giBoxY != 0 )
 {
	TrainPosition.iY = giBoxY + ( ASSIGN_MENU_TRAIN * GetFontHeight( MAP_SCREEN_FONT ) );
 }

 // create basic box
 CreatePopUpBox(&ghTrainingBox, TrainDimensions, TrainPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

 // which buffer will box render to
 SetBoxBuffer(ghTrainingBox, FRAME_BUFFER);

 // border type?
 SetBorderType(ghTrainingBox,guiPOPUPBORDERS);

 // background texture
 SetBackGroundSurface(ghTrainingBox, guiPOPUPTEX);

 // margin sizes
 SetMargins(ghTrainingBox, 6, 6, 4, 4 );

 // space between lines
 SetLineSpace(ghTrainingBox, 2);

 // set current box to this one
 SetCurrentBox( ghTrainingBox );


 // add strings for box
 for(uiCounter=0; uiCounter < MAX_TRAIN_STRING_COUNT; uiCounter++)
 {
	AddMonoString(&hStringHandle, pTrainingMenuStrings[uiCounter]);

	// make sure it is unhighlighted
	UnHighLightLine(hStringHandle);
 }

 // set font type
 SetBoxFont(ghTrainingBox, MAP_SCREEN_FONT);

 // set highlight color
 SetBoxHighLight(ghTrainingBox, FONT_WHITE);

 // unhighlighted color
 SetBoxForeground(ghTrainingBox, FONT_LTGREEN);

 // background color
 SetBoxBackground(ghTrainingBox, FONT_BLACK);

 // shaded color..for darkened text
 SetBoxShade( ghTrainingBox, FONT_GRAY7 );

 // resize box to text
 ResizeBoxToText( ghTrainingBox );

 DetermineBoxPositions( );

}


void CreateAssignmentsBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;
 CHAR16 sString[ 128 ];
 SOLDIERTYPE *pSoldier = NULL;


 // will create attribute pop up menu for mapscreen assignments

	AssignmentPosition.iX = (SCREEN_WIDTH - INTERFACE_WIDTH)/2 + OrigAssignmentPosition.iX;

	if( giBoxY != 0 )
	{
		AssignmentPosition.iY = giBoxY;
	}


	pSoldier = GetSelectedAssignSoldier( TRUE );
	// pSoldier NULL is legal here!	Gets called during every mapscreen initialization even when nobody is assign char

	// create basic box
	CreatePopUpBox(&ghAssignmentBox, AssignmentDimensions, AssignmentPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghAssignmentBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghAssignmentBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghAssignmentBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghAssignmentBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghAssignmentBox, 2);

	// set current box to this one
	SetCurrentBox( ghAssignmentBox );

	// add strings for box
	for(uiCounter=0; uiCounter < MAX_ASSIGN_STRING_COUNT; uiCounter++)
	{
		// if we have a soldier, and this is the squad line
		if( ( uiCounter == ASSIGN_MENU_ON_DUTY ) && ( pSoldier != NULL ) && ( pSoldier->bAssignment < ON_DUTY ) )
		{
			// show his squad # in brackets
			if ( gGameExternalOptions.fUseXMLSquadNames )
				swprintf( sString, L"%s(%s)", pAssignMenuStrings[uiCounter], SquadNames[ pSoldier->bAssignment ].squadname );
			else
				swprintf( sString, L"%s(%d)", pAssignMenuStrings[uiCounter], pSoldier->bAssignment + 1 );
		}
		else
		{
			swprintf( sString, pAssignMenuStrings[uiCounter] );
		}

		AddMonoString(&hStringHandle, sString );

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghAssignmentBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghAssignmentBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghAssignmentBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghAssignmentBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghAssignmentBox, FONT_GRAY7 );
	SetBoxSecondaryShade( ghAssignmentBox, FONT_YELLOW );

	// resize box to text
	ResizeBoxToText( ghAssignmentBox );

	DetermineBoxPositions( );
}


void CreateMercRemoveAssignBox( void )
{
		// will create remove mercbox to be placed in assignment area

 UINT32 hStringHandle;
 UINT32 uiCounter;
 // create basic box
 CreatePopUpBox(&ghRemoveMercAssignBox, AssignmentDimensions, AssignmentPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

 // which buffer will box render to
 SetBoxBuffer(ghRemoveMercAssignBox, FRAME_BUFFER);

 // border type?
 SetBorderType(ghRemoveMercAssignBox,guiPOPUPBORDERS);

 // background texture
 SetBackGroundSurface(ghRemoveMercAssignBox, guiPOPUPTEX);

 // margin sizes
 SetMargins( ghRemoveMercAssignBox, 6, 6, 4, 4 );

 // space between lines
 SetLineSpace(ghRemoveMercAssignBox, 2);

 // set current box to this one
 SetCurrentBox( ghRemoveMercAssignBox );

 // add strings for box
 for(uiCounter=0; uiCounter < MAX_REMOVE_MERC_COUNT; uiCounter++)
 {
	AddMonoString(&hStringHandle, pRemoveMercStrings[uiCounter]);

	// make sure it is unhighlighted
	UnHighLightLine(hStringHandle);
 }

 // set font type
 SetBoxFont(ghRemoveMercAssignBox, MAP_SCREEN_FONT);

 // set highlight color
 SetBoxHighLight(ghRemoveMercAssignBox, FONT_WHITE);

 // unhighlighted color
 SetBoxForeground(ghRemoveMercAssignBox, FONT_LTGREEN);

 // background color
 SetBoxBackground(ghRemoveMercAssignBox, FONT_BLACK);

 // shaded color..for darkened text
 SetBoxShade( ghRemoveMercAssignBox, FONT_GRAY7 );

 // resize box to text
 ResizeBoxToText( ghRemoveMercAssignBox );
}


BOOLEAN CreateDestroyAssignmentPopUpBoxes( void )
{
	static BOOLEAN fCreated= FALSE;
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC	VObjectDesc;


	if( ( fShowAssignmentMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\popup.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiPOPUPBORDERS));

		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
		strcpy(vs_desc.ImageFile, "INTERFACE\\popupbackground.pcx");
		CHECKF(AddVideoSurface(&vs_desc, &guiPOPUPTEX));

		// these boxes are always created while in mapscreen...
		CreateEPCBox( );

		CreateAssignmentsBox( );
		CreateTrainingBox( );
		CreateAttributeBox();
		CreateVehicleBox();
		CreateRepairBox();
		CreateMoveItemBox();
		// HEADROCK HAM 3.6: Facility Menu
		CreateFacilityBox( );
		CreateFacilityAssignmentBox( );
		// anv: snitch menus
		CreateSnitchBox( );
		CreateSnitchToggleBox( );
		CreateSnitchSectorBox( );
		UpdateMapScreenAssignmentPositions( );

		fCreated = TRUE;
	}
	else if( ( fShowAssignmentMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		DeleteVideoObjectFromIndex(guiPOPUPBORDERS);
		DeleteVideoSurfaceFromIndex(guiPOPUPTEX);

		RemoveBox(ghAttributeBox);
		ghAttributeBox = -1;

		// HEADROCK HAM 3.6: Remove Facility Assignment Menu
		RemoveBox(ghFacilityAssignmentBox);
		ghFacilityAssignmentBox = -1;

		RemoveBox(ghVehicleBox);
		ghVehicleBox = -1;

		RemoveBox(ghAssignmentBox);
		ghAssignmentBox = -1;

		RemoveBox(ghEpcBox);
		ghEpcBox = -1;

		RemoveBox(ghRepairBox);
		ghRepairBox = -1;

		RemoveBox(ghMoveItemBox);
		ghMoveItemBox = -1;

		RemoveBox(ghTrainingBox);
		ghTrainingBox = -1;

		// HEADROCK HAM 3.6: Remove Facility Menu
		RemoveBox(ghFacilityBox);
		ghFacilityBox = -1;

		RemoveBox(ghFacilityAssignmentBox);
		ghFacilityAssignmentBox = -1;
		
		// anv: remove snitch menus
		RemoveBox(ghSnitchBox);
		ghSnitchBox = -1;

		RemoveBox(ghSnitchToggleBox);
		ghSnitchToggleBox = -1;

		RemoveBox(ghSnitchSectorBox);
		ghSnitchSectorBox = -1;

		fCreated = FALSE;
		gfIgnoreScrolling = FALSE;
		RebuildCurrentSquad( );
	}

	return( TRUE );
}



void DetermineBoxPositions( void )
{
	// depending on how many boxes there are, reposition as needed
	SGPPoint pPoint;
	SGPPoint pNewPoint;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;


	if( ( fShowAssignmentMenu == FALSE ) || ( ghAssignmentBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( TRUE );
	// pSoldier NULL is legal here!	Gets called during every mapscreen initialization even when nobody is assign char
	if ( pSoldier == NULL )
	{
		return;
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		GetBoxPosition( ghAssignmentBox, &pPoint );
		gsAssignmentBoxesX = ( INT16 )pPoint.iX;
		gsAssignmentBoxesY = ( INT16 )pPoint.iY;
	}

	pPoint.iX = gsAssignmentBoxesX;
	pPoint.iY = gsAssignmentBoxesY;

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		SetBoxPosition( ghEpcBox, pPoint );
		GetBoxSize( ghEpcBox, &pDimensions );
	}
	else
	{
		SetBoxPosition( ghAssignmentBox, pPoint );
		GetBoxSize( ghAssignmentBox, &pDimensions );
	}


	// hang it right beside the assignment/EPC box menu
	pNewPoint.iX = pPoint.iX + pDimensions.iRight;
	pNewPoint.iY = pPoint.iY;

	if( ( fShowSquadMenu == TRUE ) && ( ghSquadBox != -1 ) )
	{
		SetBoxPosition( ghSquadBox, pNewPoint );
	}

	if( ( fShowRepairMenu == TRUE ) && ( ghRepairBox != -1 ) )
	{
		//CreateDestroyMouseRegionForRepairMenu( );
		pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_REPAIR );

		SetBoxPosition( ghRepairBox, pNewPoint );
		CreateDestroyMouseRegionForRepairMenu( );
	}

	if( ( fShowMoveItemMenu == TRUE ) && ( ghMoveItemBox != -1 ) )
	{
		//CreateDestroyMouseRegionForMoveItemMenu( );
		pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS );

		SetBoxPosition( ghMoveItemBox, pNewPoint );
		CreateDestroyMouseRegionForMoveItemMenu( );
	}
		
	if( ( fShowTrainingMenu == TRUE ) && ( ghTrainingBox != -1 ) )
	{
		pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN );
		SetBoxPosition( ghTrainingBox, pNewPoint );
		TrainPosition.iX = pNewPoint.iX;
		TrainPosition.iY = pNewPoint.iY;
		OrigTrainPosition.iY = pNewPoint.iY;
		OrigTrainPosition.iX = pNewPoint.iX;

		GetBoxSize( ghTrainingBox, &pDimensions );
		GetBoxPosition( ghTrainingBox, &pPoint );

		if( ( fShowAttributeMenu == TRUE ) && ( ghAttributeBox != -1 ) )
		{
			// hang it right beside the training box menu
			pNewPoint.iX = pPoint.iX + pDimensions.iRight;
			pNewPoint.iY = pPoint.iY;
			SetBoxPosition( ghAttributeBox, pNewPoint );
		}
	}

	// HEADROCK HAM 3.6: Facility Sub-menu
	if( ( fShowFacilityMenu == TRUE ) && ( ghFacilityBox != -1 ) )
	{
		//CreateDestroyMouseRegionForFacilityMenu( );
		pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );
		SetBoxPosition( ghFacilityBox, pNewPoint );
		FacilityPosition.iX = pNewPoint.iX;
		FacilityPosition.iY = pNewPoint.iY;
		OrigFacilityPosition.iY = pNewPoint.iY;
		OrigFacilityPosition.iX = pNewPoint.iX;

		ResizeBoxToText( ghFacilityBox );

		GetBoxSize( ghFacilityBox, &pDimensions );
		GetBoxPosition( ghFacilityBox, &pPoint );

		if( ( fShowFacilityAssignmentMenu == TRUE ) && ( ghFacilityAssignmentBox != -1 ) )
		{
			// hang it right beside the training box menu
			pNewPoint.iX = pPoint.iX + pDimensions.iRight;
			pNewPoint.iY = pPoint.iY;
			SetBoxPosition( ghFacilityAssignmentBox, pNewPoint );
		}
	}

	if( ( fShowSnitchMenu == TRUE ) && ( ghSnitchBox != -1 ) )
	{
		//CreateDestroyMouseRegionForSnitchMenu( );
		pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_SNITCH );

		SetBoxPosition( ghSnitchBox, pNewPoint );
		SnitchPosition.iX = pNewPoint.iX;
		SnitchPosition.iY = pNewPoint.iY;
		OrigSnitchPosition.iY = pNewPoint.iY;
		OrigSnitchPosition.iX = pNewPoint.iX;

		ResizeBoxToText( ghSnitchBox );

		GetBoxSize( ghSnitchBox, &pDimensions );
		GetBoxPosition( ghSnitchBox, &pPoint );

		if( ( fShowSnitchToggleMenu == TRUE ) && ( ghSnitchToggleBox != -1 ) )
		{
			// hang it right beside the training box menu
			pNewPoint.iX = pPoint.iX + pDimensions.iRight;
			pNewPoint.iY = pPoint.iY;
			SetBoxPosition( ghSnitchToggleBox, pNewPoint );
		}
		else if( ( fShowSnitchSectorMenu == TRUE ) && ( ghSnitchSectorBox != -1 ) )
		{
			// hang it right beside the training box menu
			pNewPoint.iX = pPoint.iX + pDimensions.iRight;
			pNewPoint.iY = pPoint.iY;
			SetBoxPosition( ghSnitchSectorBox, pNewPoint );
		}
	}

	return;
}



void SetTacticalPopUpAssignmentBoxXY( void )
{
	INT16 sX, sY;
	SOLDIERTYPE *pSoldier;


	//get the soldier
	pSoldier = GetSelectedAssignSoldier( FALSE );

	// grab soldier's x,y screen position
	GetSoldierScreenPos( pSoldier, &sX, &sY );

	if( sX < 0 )
	{
		sX = 0;
	}

	gsAssignmentBoxesX = sX + 30;

	if( sY < 0 )
	{
		sY = 0;
	}

	gsAssignmentBoxesY = sY;

	// ATE: Check if we are past tactical viewport....
	// Use estimate width's/heights
	if ( ( gsAssignmentBoxesX + 100 ) > SCREEN_WIDTH )
	{
		//gsAssignmentBoxesX = 540;
		gsAssignmentBoxesX = SCREEN_WIDTH - 100;
	}

	if ( ( gsAssignmentBoxesY + 130 ) > (SCREEN_HEIGHT - 160) )
	{
		gsAssignmentBoxesY = SCREEN_HEIGHT - 290;
	}

	return;
}



void RepositionMouseRegions( void )
{
	INT16 sDeltaX, sDeltaY;
	INT32 iCounter = 0;

	if( fShowAssignmentMenu == TRUE )
	{
		sDeltaX = gsAssignmentBoxesX - gAssignmentMenuRegion[ 0 ].RegionTopLeftX;
		sDeltaY = ( INT16 ) ( gsAssignmentBoxesY - gAssignmentMenuRegion[ 0 ].RegionTopLeftY + GetTopMarginSize( ghAssignmentBox ) );

		// find the delta from the old to the new, and alter values accordingly
		for( iCounter = 0; iCounter < ( INT32 )GetNumberOfLinesOfTextInBox( ghAssignmentBox ); iCounter++ )
		{
			gAssignmentMenuRegion[ iCounter ].RegionTopLeftX += sDeltaX;
			gAssignmentMenuRegion[ iCounter ].RegionTopLeftY += sDeltaY;

			gAssignmentMenuRegion[ iCounter ].RegionBottomRightX += sDeltaX;
			gAssignmentMenuRegion[ iCounter ].RegionBottomRightY += sDeltaY;
		}

		gfPausedTacticalRenderFlags = RENDER_FLAG_FULL;
	}
}



void CheckAndUpdateTacticalAssignmentPopUpPositions( void )
{
	SGPRect pDimensions, pDimensions2, pDimensions3;
	SGPPoint pPoint;
	INT16 sLongest;
	SOLDIERTYPE *pSoldier = NULL;


	if( fShowAssignmentMenu == FALSE )
	{
		return;
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}


	//get the soldier
	pSoldier = GetSelectedAssignSoldier( FALSE );

	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		GetBoxSize( ghEpcBox, &pDimensions2 );
	}
	else
	{
		GetBoxSize( ghAssignmentBox, &pDimensions2 );
	}

	if( fShowRepairMenu == TRUE )
	{
		GetBoxSize( ghRepairBox, &pDimensions );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( pDimensions2.iBottom >	pDimensions.iBottom )
		{
			sLongest = ( INT16 )pDimensions2.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_REPAIR );
		}
		else
		{
			sLongest	= ( INT16 )pDimensions.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_REPAIR );
		}

		if( gsAssignmentBoxesY + sLongest >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( sLongest ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_REPAIR );

		SetBoxPosition( ghRepairBox, pPoint );
	}
	else if( fShowMoveItemMenu == TRUE )
	{
		GetBoxSize( ghMoveItemBox, &pDimensions );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( pDimensions2.iBottom >	pDimensions.iBottom )
		{
			sLongest = ( INT16 )pDimensions2.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS );
		}
		else
		{
			sLongest	= ( INT16 )pDimensions.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS );
		}

		if( gsAssignmentBoxesY + sLongest >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( sLongest ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS );

		SetBoxPosition( ghMoveItemBox, pPoint );
	}
	else if( fShowSquadMenu == TRUE )
	{
		GetBoxSize( ghSquadBox, &pDimensions );


		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( pDimensions2.iBottom >	pDimensions.iBottom )
		{
			sLongest = ( INT16 )pDimensions2.iBottom;
		}
		else
		{
			sLongest	= ( INT16 )pDimensions.iBottom;
		}

		if( gsAssignmentBoxesY + sLongest >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( sLongest ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY;

		SetBoxPosition( ghSquadBox, pPoint );
	}
	else if( fShowAttributeMenu == TRUE )
	{
		GetBoxSize( ghTrainingBox, &pDimensions );
		GetBoxSize( ghAttributeBox, &pDimensions3 );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight + pDimensions3.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight + pDimensions3.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( gsAssignmentBoxesY + pDimensions3.iBottom + ( GetFontHeight( MAP_SCREEN_FONT ) * ASSIGN_MENU_TRAIN ) >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( pDimensions3.iBottom ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight;
		pPoint.iY = gsAssignmentBoxesY;

		pPoint.iY += (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN );
		SetBoxPosition( ghAttributeBox, pPoint );

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY;

		pPoint.iY += (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN );

		SetBoxPosition( ghTrainingBox, pPoint );


	}
	else if( fShowTrainingMenu == TRUE )
	{
		GetBoxSize( ghTrainingBox, &pDimensions );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight	>= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight	) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( gsAssignmentBoxesY + pDimensions2.iBottom +	( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN ) >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( pDimensions2.iBottom ) - (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}



		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY;
		pPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_TRAIN );

		SetBoxPosition( ghTrainingBox, pPoint );
	}
	else if ( fShowMoveItemMenu )
	{
		GetBoxSize( ghMoveItemBox, &pDimensions );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight	>= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight	) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( gsAssignmentBoxesY + pDimensions2.iBottom +	( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS ) >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( pDimensions2.iBottom ) - (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}
		
		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY;
		pPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_MOVE_ITEMS );

		SetBoxPosition( ghMoveItemBox, pPoint );
	}
	// HEADROCK HAM 3.6: Facility Sub-menu
	else if( fShowFacilityAssignmentMenu == TRUE )
	{
		GetBoxSize( ghFacilityBox, &pDimensions );
		GetBoxSize( ghFacilityAssignmentBox, &pDimensions3 );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight + pDimensions3.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight + pDimensions3.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( gsAssignmentBoxesY + pDimensions3.iBottom + ( GetFontHeight( MAP_SCREEN_FONT ) * ASSIGN_MENU_FACILITY ) >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( pDimensions3.iBottom ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight;
		pPoint.iY = gsAssignmentBoxesY;

		pPoint.iY += (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );
		SetBoxPosition( ghFacilityAssignmentBox, pPoint );

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY;

		pPoint.iY += (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );

		SetBoxPosition( ghFacilityBox, pPoint );


	}

	// HEADROCK HAM 3.6: Facility Menu
	if( fShowFacilityMenu == TRUE )
	{
		GetBoxSize( ghFacilityBox, &pDimensions );

		if( gsAssignmentBoxesX + pDimensions2.iRight + pDimensions.iRight >= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight + pDimensions.iRight ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( pDimensions2.iBottom >	pDimensions.iBottom )
		{
			sLongest = ( INT16 )pDimensions2.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );
		}
		else
		{
			sLongest	= ( INT16 )pDimensions.iBottom + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );
		}

		if( gsAssignmentBoxesY + sLongest >= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( sLongest ) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX + pDimensions2.iRight;
		pPoint.iY = gsAssignmentBoxesY + (	( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * ASSIGN_MENU_FACILITY );

		SetBoxPosition( ghFacilityBox, pPoint );
	}
	else
	{
		// just the assignment box
		if( gsAssignmentBoxesX + pDimensions2.iRight	>= SCREEN_WIDTH )
		{
			gsAssignmentBoxesX = ( INT16 ) ( (SCREEN_WIDTH - 1) - ( pDimensions2.iRight	) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		if( gsAssignmentBoxesY + pDimensions2.iBottom	>= (SCREEN_HEIGHT - 120) )
		{
			gsAssignmentBoxesY = ( INT16 )( (SCREEN_HEIGHT - 121) - ( pDimensions2.iBottom	) );
			SetRenderFlags( RENDER_FLAG_FULL );
		}

		pPoint.iX = gsAssignmentBoxesX;
		pPoint.iY = gsAssignmentBoxesY;

		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC)
		{
			SetBoxPosition( ghEpcBox, pPoint );
		}
		else
		{
			SetBoxPosition( ghAssignmentBox, pPoint );
		}
	}

	RepositionMouseRegions( );
}



void PositionCursorForTacticalAssignmentBox( void )
{
	// position cursor over y of on duty in tactical assignments
	SGPPoint pPosition;
	SGPRect pDimensions;
	INT32 iFontHeight;

	// get x.y position of box
	GetBoxPosition( ghAssignmentBox, &pPosition);

	// get dimensions..mostly for width
	GetBoxSize( ghAssignmentBox, &pDimensions );

	iFontHeight = GetLineSpace( ghAssignmentBox ) + GetFontHeight( GetBoxFont( ghAssignmentBox ) );

	if( gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ] == FALSE )
	{
		//SimulateMouseMovement( pPosition.iX + pDimensions.iRight - 6, pPosition.iY + ( iFontHeight / 2 ) + 2 );
	}
}



void HandleRestFatigueAndSleepStatus( void )
{
	INT32 iCounter = 0, iNumberOnTeam = 0;
	SOLDIERTYPE * pSoldier;
	BOOLEAN fReasonAdded = FALSE;
	BOOLEAN fBoxSetUp = FALSE;
	BOOLEAN fMeToo = FALSE;

	iNumberOnTeam =gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// run through all player characters and handle their rest, fatigue, and going to sleep
	for( iCounter = 0; iCounter < iNumberOnTeam; ++iCounter )
	{
		pSoldier = &Menptr[ iCounter ];

		if( pSoldier->bActive )
		{
			if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
			{
				continue;
			}

			if( ( pSoldier->bAssignment == ASSIGNMENT_POW ) || ( pSoldier->bAssignment == IN_TRANSIT ) )
			{
				continue;
			}

			// if character CAN sleep, he doesn't actually have to be put asleep to get some rest,
			// many other assignments and conditions allow for automatic recovering from fatigue.
			if( CharacterIsTakingItEasy( pSoldier ) )
			{
				// let them rest some
				RestCharacter( pSoldier );
			}
			else
			{
				// wear them down
				FatigueCharacter( pSoldier );
			}

			// HEADROCK HAM 3.5: Enforce breath limits from sector facilities
			if (pSoldier->bSectorZ == 0)
			{
				// Find maximum breath allowed by facilities (lowest limit found will be used)
				UINT8 ubMaxFatigue = (UINT8)GetSectorModifier( pSoldier, FACILITY_MAX_BREATH );
				if ( ubMaxFatigue > 0 && pSoldier->bBreathMax > ubMaxFatigue )
				{
					// Normalize to the maximum allowed here.
					pSoldier->bBreathMax = (pSoldier->bBreathMax + ubMaxFatigue) / 2;

					if (pSoldier->bBreath > pSoldier->bBreathMax)
					{
						// Adjust breath to max.
						pSoldier->bBreath = pSoldier->bBreathMax;
					}
				}
			}

			// CHECK FOR MERCS GOING TO SLEEP

			// if awake
			if ( !pSoldier->flags.fMercAsleep )
			{
				// if dead tired
				if( pSoldier->bBreathMax <= BREATHMAX_ABSOLUTE_MINIMUM )
				{
					// if between sectors, don't put tired mercs to sleep...	will be handled when they arrive at the next sector
					if ( pSoldier->flags.fBetweenSectors )
					{
						continue;
					}

					// he goes to sleep, provided it's at all possible (it still won't happen in a hostile sector, etc.)
					if( SetMercAsleep( pSoldier, FALSE ) )
					{
						if( ( pSoldier->bAssignment < ON_DUTY ) || ( pSoldier->bAssignment == VEHICLE ) )
						{
							// on a squad/vehicle, complain, then drop
							TacticalCharacterDialogue( pSoldier, QUOTE_NEED_SLEEP );
							TacticalCharacterDialogueWithSpecialEvent( pSoldier, QUOTE_NEED_SLEEP, DIALOGUE_SPECIAL_EVENT_SLEEP, 1,0 );
							fMeToo = TRUE;
						}
						// HEADROCK HAM 2.8/HAM 3.6: Run sleep synchronization between Trainers and Trainees
						else
						{
							HandleTrainingSleepSynchronize( pSoldier );
						}

						// guy collapses
						pSoldier->flags.fMercCollapsedFlag = TRUE;
					}
				}
				// if pretty tired, and not forced to stay awake
				else if( ( pSoldier->bBreathMax < BREATHMAX_PRETTY_TIRED ) && ( pSoldier->flags.fForcedToStayAwake == FALSE ) )
				{
					// if not on squad/ in vehicle
					if( ( pSoldier->bAssignment >= ON_DUTY ) && ( pSoldier->bAssignment != VEHICLE ) )
					{
						// try to go to sleep on your own
						if( SetMercAsleep( pSoldier, FALSE ) )
						{
							if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
							{
								// if the first one
								if( fReasonAdded == FALSE )
								{
									// tell player about it
									AddReasonToWaitingListQueue( ASLEEP_GOING_AUTO_FOR_UPDATE );
									TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_SHOW_UPDATE_MENU, 0,0 );

									fReasonAdded = TRUE;
								}

								AddSoldierToWaitingListQueue( pSoldier );
								fBoxSetUp = TRUE;
							}

							// seems unnecessary now?	ARM
							pSoldier->bOldAssignment = pSoldier->bAssignment;

							// HEADROCK HAM 2.8/HAM 3.6: If a trainer goes to sleep, we may need to put all his trainees to sleep as well.
							// If all trainees are asleep, put the trainer to sleep as well.
							HandleTrainingSleepSynchronize( pSoldier );
						}
					}
					else	// tired, in a squad / vehicle
					{
						// if he hasn't complained yet
						if ( !pSoldier->flags.fComplainedThatTired )
						{
							// say quote
							if( fMeToo == FALSE )
							{
								TacticalCharacterDialogue( pSoldier, QUOTE_NEED_SLEEP );
								fMeToo = TRUE;
							}
							else
							{
								TacticalCharacterDialogue( pSoldier, QUOTE_ME_TOO );
							}

							pSoldier->flags.fComplainedThatTired = TRUE;
						}
					}
				}
			}
		}
	}

	if( fBoxSetUp )
	{
		UnPauseGameDuringNextQuote( );
		AddDisplayBoxToWaitingQueue( );
		fBoxSetUp = FALSE;
	}

	fReasonAdded = FALSE;


	// now handle waking (needs seperate list queue, that's why it has its own loop)
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pSoldier = &Menptr[ iCounter ];

		if( pSoldier->bActive )
		{
			if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
			{
				continue;
			}

			if( ( pSoldier->bAssignment == ASSIGNMENT_POW ) || ( pSoldier->bAssignment == IN_TRANSIT ) )
			{
				continue;
			}

			// guys between sectors CAN wake up while between sectors (sleeping in vehicle)...


			// CHECK FOR MERCS WAKING UP

			if ( pSoldier->bBreathMax >= BREATHMAX_CANCEL_COLLAPSE )
			{
				// reset the collapsed flag well before reaching the wakeup state
				pSoldier->flags.fMercCollapsedFlag = FALSE;
			}


			// if asleep
			if ( pSoldier->flags.fMercAsleep )
			{
				// but has had enough rest?
				// HEADROCK HAM 3.5: Facilities can reduce maximum fatigue below 95...
				if( pSoldier->bBreathMax >= __min(BREATHMAX_FULLY_RESTED, GetSectorModifier( pSoldier, FACILITY_MAX_BREATH ) ) )
				{
					// try to wake merc up
					if( SetMercAwake( pSoldier, FALSE, FALSE ) )
					{
						// if not on squad/ in vehicle, tell player about it
						if( ( pSoldier->bAssignment >= ON_DUTY ) && ( pSoldier->bAssignment != VEHICLE ) )
						{
							if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
							{
								if( fReasonAdded == FALSE )
								{
									AddReasonToWaitingListQueue( ASSIGNMENT_RETURNING_FOR_UPDATE );
									fReasonAdded = TRUE;
								}

								AddSoldierToWaitingListQueue( pSoldier );
								fBoxSetUp = TRUE;
							}
							// HEADROCK HAM 2.8/HAM 3.6: If a trainer wakes up, we may need to wake up all his trainees as well.
							// If all trainees are awake, wake up the trainer as well.
							HandleTrainingWakeSynchronize( pSoldier );
						}
					}
				}
			}
		}
	}

	if( fBoxSetUp )
	{
		UnPauseGameDuringNextQuote( );
		AddDisplayBoxToWaitingQueue( );
		fBoxSetUp = FALSE;
	}

	return;
}



BOOLEAN CanCharacterRepairVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId )
{
	// is the vehicle valid?
	if( VehicleIdIsValid( iVehicleId ) == FALSE )
	{
		return( FALSE );
	}

	// is vehicle destroyed?
	if( pVehicleList[ iVehicleId ].fDestroyed )
	{
		return(FALSE);
	}

	// is it damaged at all?
	if ( !DoesVehicleNeedAnyRepairs( iVehicleId ) )
	{
		return(FALSE);
	}

	// it's not Skyrider's helicopter (which isn't damagable/repairable)
	if ( iVehicleId == iHelicopterVehicleId )
	{
		return( FALSE );
	}

	// same sector, neither is between sectors, and OK To Use (player owns it) ?
	if ( !IsThisVehicleAccessibleToSoldier( pSoldier, iVehicleId ) )
	{
		return( FALSE );
	}

/* Assignment distance limits removed.	Sep/11/98.	ARM
	// if currently loaded sector, are we close enough?
	if( ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->sSectorY == gWorldSectorY ) && ( pSoldier->bSectorZ == gbWorldSectorZ ) )
	{
		if( PythSpacesAway( pSoldier->sGridNo, pVehicleList[ iVehicleId ].sGridNo ) > MAX_DISTANCE_FOR_REPAIR )
		{
		return( FALSE );
		}
	}
*/

	return( TRUE );
}



BOOLEAN IsRobotInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	SOLDIERTYPE *pSoldier;

	pSoldier = GetRobotSoldier( );

	if ( pSoldier != NULL )
	{
		if( ( pSoldier->sSectorX == sSectorX ) && ( pSoldier->sSectorY == sSectorY ) && ( pSoldier->bSectorZ == bSectorZ ) && ( pSoldier->flags.fBetweenSectors == FALSE ) )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}



SOLDIERTYPE * GetRobotSoldier( void )
{
	SOLDIERTYPE *pSoldier = NULL, *pTeamSoldier = NULL;
	INT32 cnt=0;

	// set pSoldier as first in merc ptrs
	pSoldier = MercPtrs[0];

	// go through list of characters, find all who are on this assignment
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if( AM_A_ROBOT( pTeamSoldier ) )
			{
				return (pTeamSoldier);
			}
		}
	}

	return( NULL );
}


BOOLEAN CanCharacterRepairRobot( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pRobot = NULL;

	// do we in fact have the robot on the team?
	pRobot = GetRobotSoldier( );
	if( pRobot == NULL )
	{
		return( FALSE );
	}

	// if robot isn't damaged at all
	if( pRobot->stats.bLife == pRobot->stats.bLifeMax )
	{
		return( FALSE );
	}

	// is the robot in the same sector
	if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == FALSE )
	{
		return( FALSE );
	}

	// Only Technicians can repair the robot! Hehehe.. - SANDRO (well, externalized now)
	if( gSkillTraitValues.ubTETraitsNumToRepairRobot > NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT ) )
	{
		return( FALSE );
	}

/* Assignment distance limits removed.	Sep/11/98.	ARM
	// if that sector is currently loaded, check distance to robot
	if( ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->sSectorY == gWorldSectorY ) && ( pSoldier->bSectorZ == gbWorldSectorZ ) )
	{
		if( PythSpacesAway( pSoldier->sGridNo, pRobot->sGridNo ) > MAX_DISTANCE_FOR_REPAIR )
		{
		return( FALSE );
		}
	}
*/

	return( TRUE );
}



UINT8 HandleRepairOfRobotBySoldier( SOLDIERTYPE *pSoldier, UINT8 ubRepairPts, BOOLEAN *pfNothingLeftToRepair )
{
	SOLDIERTYPE *pRobot = NULL;

	pRobot = GetRobotSoldier( );

	// do the actual repairs
	return( RepairRobot( pRobot, ubRepairPts, pfNothingLeftToRepair ) );
}


UINT8 RepairRobot( SOLDIERTYPE *pRobot, UINT8 ubRepairPts, BOOLEAN *pfNothingLeftToRepair	)
{
	UINT8 ubPointsUsed = 0;


	// is it "dead" ?
	if( pRobot->stats.bLife == 0 )
	{
		*pfNothingLeftToRepair = TRUE;
		return( ubPointsUsed );
	}

	// is it "unhurt" ?
	if( pRobot->stats.bLife == pRobot->stats.bLifeMax )
	{
		*pfNothingLeftToRepair = TRUE;
		return( ubPointsUsed );
	}

	// if we have enough or more than we need
	if( pRobot->stats.bLife + ubRepairPts >= pRobot->stats.bLifeMax )
	{
		ubPointsUsed = ( pRobot->stats.bLifeMax - pRobot->stats.bLife );
		pRobot->stats.bLife = pRobot->stats.bLifeMax;
	}
	else
	{
		// not enough, do what we can
		ubPointsUsed = ubRepairPts;
		pRobot->stats.bLife += ubRepairPts;
	}

	if ( pRobot->stats.bLife == pRobot->stats.bLifeMax )
	{
		*pfNothingLeftToRepair = TRUE;
	}
	else
	{
		*pfNothingLeftToRepair = FALSE;
	}

	return( ubPointsUsed );
}


void SetSoldierAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment, INT32 iParam1, INT32 iParam2, INT32 iParam3 )
{
	switch( bAssignment )
	{
		case( ASSIGNMENT_HOSPITAL ):
			if( CanCharacterPatient( pSoldier ) )
			{

				pSoldier->bOldAssignment = pSoldier->bAssignment;
				pSoldier->bBleeding = 0;
				pSoldier->bPoisonBleeding = 0;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad

				RemoveCharacterFromSquads(	pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != ASSIGNMENT_HOSPITAL ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}


				RebuildCurrentSquad( );

				ChangeSoldiersAssignment( pSoldier, ASSIGNMENT_HOSPITAL );

				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case( PATIENT ):
			if( CanCharacterPatient( pSoldier ) )
			{
				// set as doctor

/* Assignment distance limits removed.	Sep/11/98.	ARM
				if( IsSoldierCloseEnoughToADoctor( pSoldier ) == FALSE )
				{
					return;
				}
*/

				pSoldier->bOldAssignment = pSoldier->bAssignment;


				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad
				RemoveCharacterFromSquads(	pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != PATIENT ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, PATIENT );

				AssignMercToAMovementGroup( pSoldier );
			}

		break;
		case( DOCTOR ):
			if( CanCharacterDoctor( pSoldier ) )
			{

				pSoldier->bOldAssignment = pSoldier->bAssignment;


					// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;

				// remove from squad
				RemoveCharacterFromSquads(	pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != DOCTOR ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, DOCTOR );

				MakeSureMedKitIsInHand( pSoldier );
				AssignMercToAMovementGroup( pSoldier );
			}
		break;
		case( TRAIN_TOWN ):
			if( CanCharacterTrainMilitia( pSoldier ) )
			{
				// train militia
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad
				RemoveCharacterFromSquads(	pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != TRAIN_TOWN ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, TRAIN_TOWN );

				if( pMilitiaTrainerSoldier == NULL )
				{
					if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMilitiaTrainingPaid == FALSE )
					{
						// show a message to confirm player wants to charge cost
						HandleInterfaceMessageForCostOfTrainingMilitia( pSoldier );
					}
				}

				AssignMercToAMovementGroup( pSoldier );
				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
		break;
		// HEADROCK HAM 3.6: Training mobiles is now a separate assignment.
		case( TRAIN_MOBILE ):
			if( CanCharacterTrainMobileMilitia( pSoldier ) )
			{
				// train mobile militia
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;

				// remove from squad
				RemoveCharacterFromSquads(	pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != TRAIN_MOBILE ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, TRAIN_MOBILE );

				if( pMilitiaTrainerSoldier == NULL )
				{
					if( SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE )
					{
						// show a message to confirm player wants to charge cost
						HandleInterfaceMessageForCostOfTrainingMilitia( pSoldier );
					}
				}

				AssignMercToAMovementGroup( pSoldier );
				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
		break;
		case( TRAIN_SELF ):
			if( CanCharacterTrainStat( pSoldier, ( INT8 )iParam1, TRUE, FALSE ) )
			{
				// train stat
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != TRAIN_SELF ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );

				AssignMercToAMovementGroup( pSoldier );

				// set stat to train
				pSoldier->bTrainStat = ( INT8 )iParam1;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
			break;
		case( TRAIN_TEAMMATE ):
			if( CanCharacterTrainStat( pSoldier, ( INT8 )iParam1, FALSE, TRUE ) )
			{

				pSoldier->bOldAssignment = pSoldier->bAssignment;
				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != TRAIN_TEAMMATE ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
				AssignMercToAMovementGroup( pSoldier );

				// set stat to train
				pSoldier->bTrainStat = ( INT8 )iParam1;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
		break;
		case( TRAIN_BY_OTHER ):
			if( CanCharacterTrainStat( pSoldier, ( INT8 )iParam1, TRUE, FALSE ) )
			{
				// train stat
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != TRAIN_BY_OTHER ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );

				AssignMercToAMovementGroup( pSoldier );

				// set stat to train
				pSoldier->bTrainStat = ( INT8 )iParam1;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
			break;

		case MOVE_EQUIPMENT:
			{
				// train stat
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != MOVE_EQUIPMENT ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, MOVE_EQUIPMENT );
								
				AssignMercToAMovementGroup( pSoldier );

				// set sector to take stuff from
				pSoldier->usItemMoveSectorID = iParam1;

				// set dirty flag
				fTeamPanelDirty = TRUE;
				fMapScreenBottomDirty = TRUE;
				gfRenderPBInterface = TRUE;
			}
			break;

		case( REPAIR ):
			if( CanCharacterRepair( pSoldier ) )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if( ( pSoldier->bAssignment != REPAIR ) ||( pSoldier->flags.fFixingSAMSite != ( UINT8 )iParam1 ) || ( pSoldier->flags.fFixingRobot != ( UINT8 )iParam2 ) || ( pSoldier->bVehicleUnderRepairID != ( UINT8 )iParam3 ) )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, REPAIR );
				MakeSureToolKitIsInHand( pSoldier );
				AssignMercToAMovementGroup( pSoldier );
				pSoldier->flags.fFixingSAMSite = ( UINT8 )iParam1;
				pSoldier->flags.fFixingRobot = ( UINT8 )iParam2;
				pSoldier->bVehicleUnderRepairID = ( INT8 )iParam3;
			}
			break;

		case( RADIO_SCAN ):
			if( pSoldier->CanUseRadio() )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != RADIO_SCAN )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, RADIO_SCAN );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;

		case( FACILITY_INTERROGATE_PRISONERS ):
			if( pSoldier->CanProcessPrisoners() )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != FACILITY_INTERROGATE_PRISONERS )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				MakeSoldierKnownAsMercInPrison( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY );
				ChangeSoldiersAssignment( pSoldier, FACILITY_INTERROGATE_PRISONERS );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case( FACILITY_PRISON_SNITCH ):
			if( CanCharacterSnitchInPrison(pSoldier) )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != bAssignment )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, bAssignment );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case( SNITCH_SPREAD_PROPAGANDA ):
		case( FACILITY_SPREAD_PROPAGANDA ):
		case( FACILITY_SPREAD_PROPAGANDA_GLOBAL ):
			if( CanCharacterSpreadPropaganda(pSoldier) )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != bAssignment )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, bAssignment );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case( SNITCH_GATHER_RUMOURS ):
		case( FACILITY_GATHER_RUMOURS ):
			if( CanCharacterGatherInformation(pSoldier) )
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != bAssignment )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, bAssignment );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case FACILITY_STRATEGIC_MILITIA_MOVEMENT:
			{
				pSoldier->bOldAssignment = pSoldier->bAssignment;

				// remove from squad
				RemoveCharacterFromSquads( pSoldier );

				// remove from any vehicle
				if( pSoldier->bOldAssignment == VEHICLE )
				{
					TakeSoldierOutOfVehicle( pSoldier );
				}

				if ( pSoldier->bAssignment != bAssignment )
				{
					SetTimeOfAssignmentChangeForMerc( pSoldier );
				}

				ChangeSoldiersAssignment( pSoldier, bAssignment );
				AssignMercToAMovementGroup( pSoldier );
			}
			break;
		case( VEHICLE ):
			if( CanCharacterVehicle( pSoldier ) && IsThisVehicleAccessibleToSoldier( pSoldier, iParam1 ) )
			{
				if ( IsEnoughSpaceInVehicle( (INT8) iParam1 ) )
				{
					pSoldier->bOldAssignment = pSoldier->bAssignment;

					// set dirty flag
					fTeamPanelDirty = TRUE;
					fMapScreenBottomDirty = TRUE;
					gfRenderPBInterface = TRUE;


					if( pSoldier->bOldAssignment == VEHICLE )
					{
						TakeSoldierOutOfVehicle( pSoldier );
					}

					// remove from squad
					RemoveCharacterFromSquads( pSoldier );

					if( PutSoldierInVehicle( pSoldier, ( INT8 )( iParam1 ) ) == FALSE )
					{
						AddCharacterToAnySquad( pSoldier );
					}
					else
					{
						if( ( pSoldier->bAssignment != VEHICLE ) || ( pSoldier->iVehicleId != ( UINT8 )iParam1 ) )
						{
							SetTimeOfAssignmentChangeForMerc( pSoldier );
						}

						pSoldier->iVehicleId = iParam1;
						ChangeSoldiersAssignment( pSoldier, VEHICLE );
						AssignMercToAMovementGroup( pSoldier );
					}
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 18 ], zVehicleName[ pVehicleList[ iParam1 ].ubVehicleType ] );
				}
			}
		break;
	}

	return;
}



/* No point in allowing SAM site repair any more.	Jan/13/99.	ARM
BOOLEAN CanSoldierRepairSAM( SOLDIERTYPE *pSoldier, INT8 bRepairPoints)
{
	INT16 sGridNoA = 0, sGridNoB = 0;

	// is the soldier in the sector as the SAM
	if( SoldierInSameSectorAsSAM( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	// is the soldier close enough to the control panel?
	if( IsSoldierCloseEnoughToSAMControlPanel( pSoldier ) == FALSE )
	{
		return( FALSE );
	}

	//can it be fixed?
	if( IsTheSAMSiteInSectorRepairable( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == FALSE )
	{
		return( FALSE );
	}

	// is he good enough?	(Because of the division of repair pts in SAM repair formula, a guy with any less that this
	// can't make any headway
	if (bRepairPoints < SAM_SITE_REPAIR_DIVISOR )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN IsTheSAMSiteInSectorRepairable( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	INT32 iCounter = 0;
	INT8 bSAMCondition;


	// is the guy above ground, if not, it can't be fixed, now can it?
	if( sSectorZ != 0 )
	{
		return( FALSE );
	}

	for( iCounter = 0; iCounter < NUMBER_OF_SAMS; iCounter++ )
	{
		if( pSamList[ iCounter ] == SECTOR( sSectorX, sSectorY ) )
		{
			bSAMCondition = StrategicMap[ CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY ) ].bSAMCondition;

			if( ( bSAMCondition < 100 ) && ( bSAMCondition >= MIN_CONDITION_TO_FIX_SAM ) )
			{
				return( TRUE );
			}
			else
			{
				// it's not broken at all, or it's beyond repair
				return( FALSE );
			}
		}
	}

	// none found
	return( FALSE );
}

BOOLEAN SoldierInSameSectorAsSAM( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter = 0;

	// is the soldier on the surface?
	if( pSoldier->bSectorZ != 0 )
	{
		return( FALSE );
	}

	// now check each sam site in the list
	for( iCounter = 0; iCounter < NUMBER_OF_SAMS; iCounter++ )
	{
		if( pSamList[ iCounter] == SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

BOOLEAN IsSoldierCloseEnoughToSAMControlPanel( SOLDIERTYPE *pSoldier )
{

	INT32 iCounter = 0;

		// now check each sam site in the list
	for( iCounter = 0; iCounter < NUMBER_OF_SAMS; iCounter++ )
	{
		if( pSamList[ iCounter ] == SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) )
		{
// Assignment distance limits removed.	Sep/11/98.	ARM
//			if( ( PythSpacesAway( pSamGridNoAList[ iCounter ], pSoldier->sGridNo ) < MAX_DISTANCE_FOR_REPAIR )||( PythSpacesAway( pSamGridNoBList[ iCounter ], pSoldier->sGridNo ) < MAX_DISTANCE_FOR_REPAIR ) )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}
*/



BOOLEAN HandleAssignmentExpansionAndHighLightForAssignMenu( SOLDIERTYPE *pSoldier )
{
	if( fShowSquadMenu )
	{
		// squad menu up?..if so, highlight squad line the previous menu
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
		{
			HighLightBoxLine( ghEpcBox, EPC_MENU_ON_DUTY );
		}
		else
		{
			HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_ON_DUTY );
		}

		return( TRUE );
	}
	else if( fShowTrainingMenu )
	{
		// highlight train line the previous menu
		HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_TRAIN );
		return( TRUE );
	}
	else if( fShowRepairMenu )
	{
		// highlight repair line the previous menu
		HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_REPAIR );
		return( TRUE );
	}
	else if( fShowMoveItemMenu )
	{
		// highlight repair line the previous menu
		HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_MOVE_ITEMS );
		return( TRUE );
	}
	else if( fShowVehicleMenu )
	{
		// highlight vehicle line the previous menu
		HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_VEHICLE );
		return( TRUE );
	}
	// HEADROCK HAM 3.6: Facility Menu.
	else if( fShowFacilityMenu )
	{
		// highlight the facility line in the previous menu
		HighLightBoxLine( ghAssignmentBox, ASSIGN_MENU_FACILITY );
		return( TRUE );
	}
	else if( fShowSnitchMenu )
	{
		// highlight the snitch line in the previous menu
		HighLightBoxLine( ghSnitchBox, ASSIGN_MENU_SNITCH );
		return( TRUE );
	}
	return( FALSE );
}


BOOLEAN HandleAssignmentExpansionAndHighLightForTrainingMenu( void )
{
	if( fShowAttributeMenu )
	{
		switch ( gbTrainingMode )
		{
			case TRAIN_SELF:
				HighLightBoxLine( ghTrainingBox, TRAIN_MENU_SELF );
				return( TRUE );
			case TRAIN_TEAMMATE:
				HighLightBoxLine( ghTrainingBox, TRAIN_MENU_TEAMMATES );
				return( TRUE );
			case TRAIN_BY_OTHER:
				HighLightBoxLine( ghTrainingBox, TRAIN_MENU_TRAIN_BY_OTHER );
				return( TRUE );

			default:
				Assert( FALSE );
				break;
		}
	}

	return( FALSE );
}

BOOLEAN HandleAssignmentExpansionAndHighLightForSnitchMenu( void )
{
	if( fShowSnitchToggleMenu )
	{
		// highlight the snitch line in the previous menu
		HighLightBoxLine( ghSnitchBox, SNITCH_MENU_TOGGLE );
		return( TRUE );
	}
	else if( fShowSnitchSectorMenu )
	{
		// highlight the snitch line in the previous menu
		HighLightBoxLine( ghSnitchBox, SNITCH_MENU_SECTOR );
		return( TRUE );
	}

	return( FALSE );
}

/*
BOOLEAN HandleShowingOfUpBox( void )
{

	// if the list is being shown, then show it
	if( fShowUpdateBox == TRUE )
	{
		MarkAllBoxesAsAltered( );
		ShowBox( ghUpdateBox );
		return( TRUE );
	}
	else
	{
		if( IsBoxShown( ghUpdateBox ) )
		{
			HideBox( ghUpdateBox );
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
		}
	}

	return( FALSE );
}
*/



BOOLEAN HandleShowingOfMovementBox( void )
{

	// if the list is being shown, then show it
	if( fShowMapScreenMovementList == TRUE )
	{
		MarkAllBoxesAsAltered( );
		ShowBox( ghMoveBox );
		return( TRUE );
	}
	else
	{
		if( IsBoxShown( ghMoveBox ) )
		{
			HideBox( ghMoveBox );
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
			fTeamPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;
		}
	}

	return( FALSE );
}


void HandleShadingOfLinesForTrainingMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// check if valid
	if( ( fShowTrainingMenu == FALSE ) || ( ghTrainingBox == - 1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	// can character practise?
	if( CanCharacterPractise( pSoldier ) == FALSE )
	{
		ShadeStringInBox( ghTrainingBox, TRAIN_MENU_SELF );
	}
	else
	{
		UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_SELF );
	}


	// can character EVER train militia?
	if( BasicCanCharacterTrainMilitia( pSoldier ) )
	{
		// can he train here, now?
		if( CanCharacterTrainMilitia( pSoldier ) )
		{
			// unshade train militia line
			UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
			UnSecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
		}
		else
		{
			UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
			SecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
		}
	}
	else
	{
		UnSecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
		// shade train militia line
		ShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
	}

	// HEADROCK HAM 3.6: Training Mobile Militia.
	// can character EVER train Mobile Militia?
	if( BasicCanCharacterTrainMobileMilitia( pSoldier ) )
	{
		// can he train here, now?
		if( CanCharacterTrainMobileMilitia( pSoldier ) )
		{
			// unshade train militia line
			UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
			UnSecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
		}
		else
		{
			UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
			SecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
		}
	}
	else
	{
		UnSecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
		// shade train militia line
		ShadeStringInBox( ghTrainingBox, TRAIN_MENU_MOBILE );
	}


	// can character train teammates?
	if( CanCharacterTrainTeammates( pSoldier ) == FALSE )
	{
		ShadeStringInBox( ghTrainingBox, TRAIN_MENU_TEAMMATES );
	}
	else
	{
		UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_TEAMMATES );
	}


	// can character be trained by others?
	if( CanCharacterBeTrainedByOther( pSoldier ) == FALSE )
	{
		ShadeStringInBox( ghTrainingBox, TRAIN_MENU_TRAIN_BY_OTHER );
	}
	else
	{
		UnShadeStringInBox( ghTrainingBox, TRAIN_MENU_TRAIN_BY_OTHER );
	}

	return;
}


void HandleShadingOfLinesForAttributeMenus( void )
{
	// will do the same as updateassignments...but with training pop up box strings
	SOLDIERTYPE *pSoldier;
	INT8 bAttrib =0;
	BOOLEAN fStatTrainable;


	if( ( fShowTrainingMenu == FALSE ) || ( ghTrainingBox == - 1 ) )
	{
		return;
	}

	if( ( fShowAttributeMenu == FALSE ) || ( ghAttributeBox == - 1 ) )
	{
		return;
	}


	pSoldier = GetSelectedAssignSoldier( FALSE );

	for( bAttrib = 0; bAttrib < ATTRIB_MENU_CANCEL; bAttrib++ )
	{
		switch ( gbTrainingMode )
		{
			case TRAIN_SELF:
				fStatTrainable = CanCharacterTrainStat( pSoldier, bAttrib, TRUE, FALSE );
				break;
			case TRAIN_TEAMMATE:
				// DO allow trainers to be assigned without any partners (students)
				fStatTrainable = CanCharacterTrainStat( pSoldier, bAttrib, FALSE, TRUE );
				break;
			case TRAIN_BY_OTHER:
				// DO allow students to be assigned without any partners (trainers)
				fStatTrainable = CanCharacterTrainStat( pSoldier, bAttrib, TRUE, FALSE );
				break;
			default:
				Assert( FALSE );
				fStatTrainable = FALSE;
				break;
		}

		if ( fStatTrainable )
		{
			// also unshade stat
			UnShadeStringInBox( ghAttributeBox, bAttrib );
		}
		else
		{
			// shade stat
			ShadeStringInBox( ghAttributeBox, bAttrib );
		}
	}

	return;
}

void HandleShadingOfLinesForSnitchMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// check if valid
	if( ( fShowSnitchMenu == FALSE ) || ( ghSnitchBox == - 1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	// can character snitch?
	if( CanCharacterSnitch( pSoldier ) == TRUE )
	{
		UnShadeStringInBox( ghSnitchBox, SNITCH_MENU_TOGGLE);

		if( CanCharacterSpreadPropaganda( pSoldier ) || CanCharacterGatherInformation( pSoldier ) )
		{
			UnShadeStringInBox( ghSnitchBox, SNITCH_MENU_SECTOR );
			UnSecondaryShadeStringInBox( ghSnitchBox, SNITCH_MENU_SECTOR );
		}
		else
		{
			ShadeStringInBox( ghSnitchBox, SNITCH_MENU_SECTOR );
			UnSecondaryShadeStringInBox( ghSnitchBox, SNITCH_MENU_SECTOR );
		}
	}
	else
	{
		// shouldn't even get that far
		ShadeStringInBox( ghSnitchBox, SNITCH_MENU_TOGGLE );
		ShadeStringInBox( ghSnitchBox, SNITCH_MENU_SECTOR );
	}
	//UnSecondaryShadeStringInBox( ghTrainingBox, TRAIN_MENU_TOWN );
	return;
}

void HandleShadingOfLinesForSnitchToggleMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// check if valid
	if( ( fShowSnitchToggleMenu == FALSE ) || ( ghSnitchToggleBox == - 1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	if( pSoldier->usSoldierFlagMask2 & SOLDIER_SNITCHING_OFF )
	{
		UnShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_TOGGLE_ON );
		ShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_TOGGLE_OFF );
	}
	else
	{
		ShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_TOGGLE_ON );
		UnShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_TOGGLE_OFF );
	}

	if( pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF )
	{
		UnShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_MISBEHAVIOUR_ON );
		ShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_MISBEHAVIOUR_OFF );
	}
	else
	{
		ShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_MISBEHAVIOUR_ON );
		UnShadeStringInBox( ghSnitchToggleBox, SNITCH_MENU_MISBEHAVIOUR_OFF );
	}
	return;
}

void HandleShadingOfLinesForSnitchSectorMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// check if valid
	if( ( fShowSnitchSectorMenu == FALSE ) || ( ghSnitchSectorBox == - 1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	if( !CanCharacterSpreadPropaganda(pSoldier) )
	{
		ShadeStringInBox( ghSnitchSectorBox, SNITCH_MENU_SECTOR_PROPAGANDA );
	}

	if( !CanCharacterGatherInformation(pSoldier) )
	{
		ShadeStringInBox( ghSnitchSectorBox, SNITCH_MENU_SECTOR_GATHER_RUMOURS );
	}

	return;
}

/*void ResetAssignmentsForAllSoldiersInSectorWhoAreTrainingTown( SOLDIERTYPE *pSoldier )
{
	INT32 iNumberOnTeam = 0, iCounter = 0;
	SOLDIERTYPE *pCurSoldier = NULL;

	iNumberOnTeam = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pCurSoldier = &Menptr[ iCounter ];

		if( ( pCurSoldier->bActive ) && ( pCurSoldier->stats.bLife >= OKLIFE ) )
		{
			if( pCurSoldier->bAssignment == TRAIN_TOWN )
			{
				if( ( pCurSoldier->sSectorX == pSoldier->sSectorX ) && ( pCurSoldier->sSectorY == pSoldier->sSectorY ) && ( pSoldier->bSectorZ == 0 ) )
				{
					AddCharacterToAnySquad( pCurSoldier );
				}
			}
		}
	}

	return;
}*/


void ReportTrainersTraineesWithoutPartners( void )
{
	SOLDIERTYPE *pTeamSoldier = NULL;
	INT32 iCounter = 0, iNumberOnTeam = 0;


	iNumberOnTeam = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// check for each instructor
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pTeamSoldier = &Menptr[ iCounter ];

		if( ( pTeamSoldier->bAssignment == TRAIN_TEAMMATE ) && ( pTeamSoldier->stats.bLife > 0 ) )
		{
			if ( !ValidTrainingPartnerInSameSectorOnAssignmentFound( pTeamSoldier, TRAIN_BY_OTHER, pTeamSoldier->bTrainStat ) )
			{
				AssignmentDone( pTeamSoldier, TRUE, TRUE );
			}
		}
	}

	// check each trainee
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pTeamSoldier = &Menptr[ iCounter ];

		if( ( pTeamSoldier->bAssignment == TRAIN_BY_OTHER ) && ( pTeamSoldier->stats.bLife > 0 ) )
		{
			if ( !ValidTrainingPartnerInSameSectorOnAssignmentFound( pTeamSoldier, TRAIN_TEAMMATE, pTeamSoldier->bTrainStat ) )
			{
				AssignmentDone( pTeamSoldier, TRUE, TRUE );
			}
		}
	}

	return;
}


BOOLEAN SetMercAsleep( SOLDIERTYPE *pSoldier, BOOLEAN fGiveWarning )
{
	if( CanCharacterSleep( pSoldier, fGiveWarning ) )
	{
		// put him to sleep
		PutMercInAsleepState( pSoldier );

		// successful
		return( TRUE );
	}
	else
	{
		// can't sleep for some other reason
		return( FALSE );
	}
}



BOOLEAN PutMercInAsleepState( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->flags.fMercAsleep == FALSE )
	{
		if( ( gfWorldLoaded ) && ( pSoldier->bInSector ) )
		{
			if( guiCurrentScreen == GAME_SCREEN )
			{
				pSoldier->ChangeSoldierState( GOTO_SLEEP, 1, TRUE );
			}
			else
			{
				pSoldier->ChangeSoldierState( SLEEPING, 1, TRUE );
			}
		}

		// set merc asleep
		pSoldier->flags.fMercAsleep = TRUE;

		// refresh panels
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
	}

	return( TRUE );
}


BOOLEAN SetMercAwake( SOLDIERTYPE *pSoldier, BOOLEAN fGiveWarning, BOOLEAN fForceHim )
{
	// forcing him skips all normal checks!
	if ( !fForceHim )
	{
		if ( !CanCharacterBeAwakened( pSoldier, fGiveWarning ) )
		{
			return( FALSE );
		}
	}

	PutMercInAwakeState( pSoldier );
	return( TRUE );
}


BOOLEAN PutMercInAwakeState( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->flags.fMercAsleep )
	{
		if ( ( gfWorldLoaded ) && ( pSoldier->bInSector ) )
		{
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				pSoldier->ChangeSoldierState( WKAEUP_FROM_SLEEP, 1, TRUE );
			}
			else
			{
				pSoldier->ChangeSoldierState( STANDING, 1, TRUE );
			}
		}

		// set merc awake
		pSoldier->flags.fMercAsleep = FALSE;

		// HEADROCK HAM 3.6: Merc hasn't been working all this time, so let's reset his assignment start time. This
		// will squash an exploit, and is a bit more realistic anyway.
		pSoldier->uiLastAssignmentChangeMin = GetWorldTotalMin();

		// refresh panels
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;

		// determine if merc is being forced to stay awake
		if ( pSoldier->bBreathMax < BREATHMAX_PRETTY_TIRED )
		{
			pSoldier->flags.fForcedToStayAwake = TRUE;
		}
		else
		{
			pSoldier->flags.fForcedToStayAwake = FALSE;
		}
	}

	return( TRUE );
}

BOOLEAN IsThereASoldierInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	if( fSectorsWithSoldiers[	sSectorX + sSectorY * MAP_WORLD_X ][ bSectorZ ] == TRUE )
	{
		return( TRUE );
	}

	return( FALSE );
}


// set the time this soldier's assignment changed
void SetTimeOfAssignmentChangeForMerc( SOLDIERTYPE *pSoldier )
{
	// if someone is being taken off of HOSPITAL then track how much
	// of payment wasn't used up
	if ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL )
	{
		giHospitalRefund += CalcPatientMedicalCost( pSoldier );
		pSoldier->bHospitalPriceModifier = 0;
	}

	// set time of last assignment change
	pSoldier->uiLastAssignmentChangeMin = GetWorldTotalMin( );

	// assigning new PATIENTs gives a DOCTOR something to do, etc., so set flag to recheck them all.
	// CAN'T DO IT RIGHT AWAY IN HERE 'CAUSE WE TYPICALLY GET CALLED *BEFORE* bAssignment GETS SET TO NEW VALUE!!
	gfReEvaluateEveryonesNothingToDo = TRUE;

	return;
}

// have we spent enough time on assignment for it to count?
BOOLEAN EnoughTimeOnAssignment( SOLDIERTYPE *pSoldier )
{
	if( GetWorldTotalMin() - pSoldier->uiLastAssignmentChangeMin >= (UINT32)gGameExternalOptions.ubMinutesForAssignmentToCount )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN AnyMercInGroupCantContinueMoving( GROUP *pGroup )
{
	PLAYERGROUP *pPlayer;
	SOLDIERTYPE *pSoldier;
	BOOLEAN fMeToo = FALSE;
	BOOLEAN fGroupMustStop = FALSE;


	AssertNotNIL( pGroup );
	AssertT( pGroup->fPlayer );

	pPlayer = pGroup->pPlayerList;

	while( pPlayer )
	{
		// if group has player list...	and a valid first soldier
		if( pPlayer->pSoldier )
		{
			pSoldier = pPlayer->pSoldier;

			if ( PlayerSoldierTooTiredToTravel( pSoldier ) )
			{
				// NOTE: we only complain about it if it's gonna force the group to stop moving!
				fGroupMustStop = TRUE;

				// say quote
				if( fMeToo == FALSE )
				{
					HandleImportantMercQuote( pSoldier, QUOTE_NEED_SLEEP );
					fMeToo = TRUE;
				}
				else
				{
					HandleImportantMercQuote( pSoldier, QUOTE_ME_TOO );
				}

				// put him to bed
				PutMercInAsleepState( pSoldier );

				// player can't wake him up right away
				pSoldier->flags.fMercCollapsedFlag = TRUE;
			}
		}

		pPlayer = pPlayer->next;
	}

	return( fGroupMustStop );
}



BOOLEAN PlayerSoldierTooTiredToTravel( SOLDIERTYPE *pSoldier )
{
	Assert( pSoldier );

	// if this guy ever needs sleep at all
	if ( CanChangeSleepStatusForSoldier( pSoldier ) )
	{
		// if walking, or only remaining possible driver for a vehicle group
		if ( ( pSoldier->bAssignment != VEHICLE ) || SoldierMustDriveVehicle( pSoldier, pSoldier->iVehicleId, TRUE ) )
		{
			// if awake, but so tired they can't move/drive anymore
			if ( ( !pSoldier->flags.fMercAsleep ) && ( pSoldier->bBreathMax < BREATHMAX_GOTTA_STOP_MOVING ) )
			{
				return( TRUE );
			}

			// asleep, and can't be awakened?
			if ( ( pSoldier->flags.fMercAsleep ) && !CanCharacterBeAwakened( pSoldier, FALSE ) )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}



BOOLEAN AssignMercToAMovementGroup( SOLDIERTYPE *pSoldier )
{
	// if merc doesn't have a group or is in a vehicle or on a squad assign to group
	INT8 bGroupId = 0;

	// on a squad?
	if( pSoldier->bAssignment < ON_DUTY )
	{
		return( FALSE );
	}

	// in a vehicle?
	if( pSoldier->bAssignment == VEHICLE )
	{
		return( FALSE );
	}

	// in transit
	if( pSoldier->bAssignment == IN_TRANSIT )
	{
		return( FALSE );
	}

	// in a movement group?
	if( pSoldier->ubGroupID != 0 )
	{
		return( FALSE );
	}

	// create group
	bGroupId = CreateNewPlayerGroupDepartingFromSector( ( UINT8 )( pSoldier->sSectorX ), ( UINT8 )( pSoldier->sSectorY ) );

	if( bGroupId )
	{
		// add merc
		AddPlayerToGroup( bGroupId, pSoldier );

		// success
		return( TRUE );
	}

	return( TRUE );
}


void NotifyPlayerOfAssignmentAttemptFailure( INT8 bAssignment )
{
	// notify player
	if ( guiCurrentScreen != MSG_BOX_SCREEN )
	{
		DoScreenIndependantMessageBox( pMapErrorString[ 18 ], MSG_BOX_FLAG_OK, NULL);
	}
	else
	{
		// use screen msg instead!
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMapErrorString[ 18 ] );
	}

	if ( bAssignment == TRAIN_TOWN )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMapErrorString[ 48 ] );
	}
}


BOOLEAN HandleSelectedMercsBeingPutAsleep( BOOLEAN fWakeUp, BOOLEAN fDisplayWarning )
{
	BOOLEAN fSuccess = TRUE;
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	UINT8 ubNumberOfSelectedSoldiers = 0;
	CHAR16 sString[ 128 ];


	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		pSoldier = NULL;

		// if the current character in the list is valid...then grab soldier pointer for the character
		if( gCharactersList[ iCounter ].fValid )
		{
			// get the soldier pointer
			pSoldier = &Menptr[ gCharactersList[ iCounter ].usSolID ];

			if( pSoldier->bActive == FALSE )
			{
				continue;
			}

			if( iCounter == bSelectedInfoChar )
			{
				continue;
			}

			if( IsEntryInSelectedListSet( ( INT8 )iCounter ) == FALSE )
			{
				continue;
			}

			// don't try to put vehicles, robots, to sleep if they're also selected
			if ( CanChangeSleepStatusForCharSlot( (INT8) iCounter ) == FALSE )
			{
				continue;
			}

			// up the total number of soldiers
			ubNumberOfSelectedSoldiers++;

			if( fWakeUp )
			{
				// try to wake merc up
				if( SetMercAwake( pSoldier, FALSE, FALSE ) == FALSE )
				{
					fSuccess = FALSE;
				}
			}
			else
			{
				// set this soldier asleep
				if( SetMercAsleep( pSoldier, FALSE ) == FALSE )
				{
					fSuccess = FALSE;
				}
			}
		}
	}

	// if there was anyone processed, check for success and inform player of failure
	if( ubNumberOfSelectedSoldiers )
	{
		if( fSuccess == FALSE )
		{
			if( fWakeUp )
			{
				// inform player not everyone could be woke up
				swprintf( sString, pMapErrorString[ 27 ] );
			}
			else
			{
				// inform player not everyone could be put to sleep
				swprintf( sString, pMapErrorString[ 26 ]);
			}

			if( fDisplayWarning )
			{
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL);
			}
		}
	}

	return( fSuccess );
}

BOOLEAN IsAnyOneOnPlayersTeamOnThisAssignment( INT8 bAssignment )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	for( iCounter = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
	{
		// get the current soldier
		pSoldier = &Menptr[ iCounter ];

		// active?
		if( pSoldier->bActive == FALSE )
		{
			continue;
		}

		if( pSoldier->bAssignment == bAssignment )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void RebuildAssignmentsBox( void )
{
	// destroy and recreate assignments box
	if ( ghAssignmentBox != -1 )
	{
		RemoveBox( ghAssignmentBox );
		ghAssignmentBox = -1;
	}

	CreateAssignmentsBox( );
}



void BandageBleedingDyingPatientsBeingTreated( )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	SOLDIERTYPE *pDoctor = NULL;
	INT32 iKitSlot;
	OBJECTTYPE *pKit = NULL;
	UINT16 usKitPts;
	UINT32 uiKitPtsUsed;
	BOOLEAN fSomeoneStillBleedingDying = FALSE;


	for( iCounter = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
	{
		// get the soldier
		pSoldier = &Menptr[ iCounter ];

		// check if the soldier is currently active?
		if( pSoldier->bActive == FALSE )
		{
			continue;
		}

		// and he is bleeding or dying
		if( ( pSoldier->bBleeding ) || ( pSoldier->stats.bLife < OKLIFE ) )
		{
			// if soldier is receiving care
			if( ( pSoldier->bAssignment == PATIENT ) || ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL ) || ( pSoldier->bAssignment == DOCTOR ) )
			{
				// if in the hospital
				if ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL )
				{
					// this is instantaneous, and doesn't use up any bandages!

					// stop bleeding automatically
					pSoldier->bBleeding = 0;
					pSoldier->bPoisonBleeding = 0;

					if ( pSoldier->stats.bLife < OKLIFE )
					{
						// SANDRO - added to alter the value of insta-healable injuries for doctors
						if (pSoldier->iHealableInjury > 0)
						{
							pSoldier->iHealableInjury -= ((OKLIFE - pSoldier->stats.bLife) * 100);
						}

						// convert poison points to poison life points
						INT8 oldlife = pSoldier->stats.bLife;

						pSoldier->stats.bLife = OKLIFE;

						INT8 lifegained = OKLIFE - oldlife;
						pSoldier->bPoisonLife = min(pSoldier->bPoisonSum, pSoldier->bPoisonLife + lifegained); 
					}
				}
				else	// assigned to DOCTOR/PATIENT
				{
					// see if there's a doctor around who can help him
					pDoctor = AnyDoctorWhoCanHealThisPatient( pSoldier, HEALABLE_EVER );
					if ( pDoctor != NULL )
					{
						iKitSlot = FindObjClass( pDoctor, IC_MEDKIT );
						if( iKitSlot != NO_SLOT )
						{
							pKit = &( pDoctor->inv[ iKitSlot ] );

							usKitPts = TotalPoints( pKit );
							if( usKitPts )
							{
								uiKitPtsUsed = VirtualSoldierDressWound( pDoctor, pSoldier, pKit, usKitPts, usKitPts, FALSE ); // SANDRO - added variable
								UseKitPoints( pKit, (UINT16)uiKitPtsUsed, pDoctor );

								// if he is STILL bleeding or dying
								if( ( pSoldier->bBleeding ) || ( pSoldier->stats.bLife < OKLIFE ) )
								{
									fSomeoneStillBleedingDying = TRUE;
								}
							}
						}
					}
				}
			}
		}
	}


	// this event may be posted many times because of multiple assignment changes.	Handle it only once per minute!
	DeleteAllStrategicEventsOfType( EVENT_BANDAGE_BLEEDING_MERCS );

	if ( fSomeoneStillBleedingDying )
	{
		AddStrategicEvent( EVENT_BANDAGE_BLEEDING_MERCS, GetWorldTotalMin() + 1, 0 );
	}
}



void ReEvaluateEveryonesNothingToDo()
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	BOOLEAN fNothingToDo;


	for( iCounter = 0; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
	{
		pSoldier = &Menptr[ iCounter ];

		if( pSoldier->bActive )
		{
			switch( pSoldier->bAssignment )
			{
				case DOCTOR:
					fNothingToDo = !CanCharacterDoctor( pSoldier ) || ( GetNumberThatCanBeDoctored( pSoldier, HEALABLE_EVER, FALSE, FALSE, FALSE ) == 0 ); // SANDRO - added variable
					break;

				case PATIENT:
					// HEADROCK HAM 3.6: Characters can be facility patients, in which case they are resting with a
					// bonus or penalty to their healing rate. No doctor is actually required, even though the
					// character is still eligible for a doctor's treatment.
					fNothingToDo = !CanCharacterPatient( pSoldier ) || 
						( AnyDoctorWhoCanHealThisPatient( pSoldier, HEALABLE_EVER ) == NULL &&
						GetSoldierFacilityAssignmentIndex( pSoldier ) != FAC_PATIENT );
					break;

				case ASSIGNMENT_HOSPITAL:
					fNothingToDo = !CanCharacterPatient( pSoldier );
					break;

				case REPAIR:
					fNothingToDo = !CanCharacterRepair( pSoldier ) || HasCharacterFinishedRepairing( pSoldier );
					break;

				case RADIO_SCAN:
					fNothingToDo = !(pSoldier->CanUseRadio());
					break;

				case FACILITY_INTERROGATE_PRISONERS:
					fNothingToDo = !(pSoldier->CanProcessPrisoners());
					break;

				case  FACILITY_PRISON_SNITCH:
					fNothingToDo = !CanCharacterSnitchInPrison(pSoldier);
					break;

				case  FACILITY_SPREAD_PROPAGANDA:
				case  FACILITY_SPREAD_PROPAGANDA_GLOBAL:
				case  SNITCH_SPREAD_PROPAGANDA:
					fNothingToDo = !CanCharacterSpreadPropaganda(pSoldier);
					break;

				case  FACILITY_GATHER_RUMOURS:
				case  SNITCH_GATHER_RUMOURS:
					fNothingToDo = !CanCharacterGatherInformation(pSoldier);
					break;

				case TRAIN_TOWN:
					fNothingToDo = !CanCharacterTrainMilitia( pSoldier );
					break;

				// HEADROCK HAM 3.6: Mobile Militia Training
				case TRAIN_MOBILE:
					fNothingToDo = !CanCharacterTrainMobileMilitia( pSoldier );
					break;

				case TRAIN_SELF:
					fNothingToDo = !CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, TRUE, FALSE );
					break;

				case TRAIN_TEAMMATE:
					fNothingToDo = !CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, FALSE, TRUE ) ||
												!ValidTrainingPartnerInSameSectorOnAssignmentFound( pSoldier, TRAIN_BY_OTHER, pSoldier->bTrainStat );
					break;

				case TRAIN_BY_OTHER:
					fNothingToDo = !CanCharacterTrainStat( pSoldier, pSoldier->bTrainStat, TRUE, FALSE ) ||
												!ValidTrainingPartnerInSameSectorOnAssignmentFound( pSoldier, TRAIN_TEAMMATE, pSoldier->bTrainStat );
					break;

				case MOVE_EQUIPMENT:
					fNothingToDo = FALSE;
					break;

				case FACILITY_STRATEGIC_MILITIA_MOVEMENT:
					fNothingToDo = FALSE;
					break;

				case VEHICLE:
				default:	// squads
					fNothingToDo = FALSE;
					break;
			}
			// HEADROCK HAM 3.6: Separate "can do" checks when the character is using a facility.
			if (fNothingToDo == FALSE && GetSoldierFacilityAssignmentIndex(pSoldier) >= 0)
			{
				// Can this soldier continue working at the same facility doing the same job?
				fNothingToDo = !CanCharacterFacility( pSoldier, (UINT8)pSoldier->sFacilityTypeOperated, (UINT8)GetSoldierFacilityAssignmentIndex(pSoldier) );
			}

			// if his flag is wrong
			if ( fNothingToDo != pSoldier->flags.fDoneAssignmentAndNothingToDoFlag )
			{
				// update it!
				pSoldier->flags.fDoneAssignmentAndNothingToDoFlag = fNothingToDo;

				// update mapscreen's character list display
				fDrawCharacterList = TRUE;
			}

			// if he now has something to do, reset the quote flag
			if ( !fNothingToDo )
			{
				pSoldier->usQuoteSaidExtFlags &= ~SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT;
			}


		}
	}

	// re-evaluation completed
	gfReEvaluateEveryonesNothingToDo = FALSE;


	// redraw the map, in case we're showing teams, and someone just came on duty or off duty, their icon needs updating
	fMapPanelDirty = TRUE;
}



void SetAssignmentForList( INT8 bAssignment, INT8 bParam )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSelectedSoldier = NULL;
	SOLDIERTYPE *pSoldier = NULL;
	BOOLEAN fItWorked;
	BOOLEAN fRemoveFromSquad = TRUE;
	BOOLEAN fNotifiedOfFailure = FALSE;
	INT8 bCanJoinSquad;

	// if not in mapscreen, there is no functionality available to change multiple assignments simultaneously!
	if ( !( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	// pSelectedSoldier is currently used only for REPAIR, and this block of code is copied from RepairMenuBtnCallback()
	if( bSelectedAssignChar != -1 )
	{
		if( gCharactersList[ bSelectedAssignChar ].fValid == TRUE )
		{
			pSelectedSoldier = &Menptr[ gCharactersList[ bSelectedAssignChar ].usSolID ];
		}
	}

	Assert( pSelectedSoldier && pSelectedSoldier->bActive );


	// sets assignment for the list
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( ( gCharactersList[ iCounter ].fValid ) &&
				( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE ) &&
				( iCounter != bSelectedAssignChar ) &&
				!(Menptr[ gCharactersList[ iCounter ].usSolID].flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

			// assume it's NOT gonna work
			fItWorked = FALSE;

			switch( bAssignment )
			{
				case( DOCTOR ):
					// can character doctor?
					if( CanCharacterDoctor( pSoldier ) )
					{
						// set as doctor
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, DOCTOR, 0,0,0 );
						fItWorked = TRUE;
					}
					break;
				case( PATIENT ):
					// can character patient?
					if( CanCharacterPatient( pSoldier ) )
					{
						// set as patient
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, PATIENT, 0,0,0 );
						fItWorked = TRUE;
					}
					break;
				case( VEHICLE ):
					if( CanCharacterVehicle( pSoldier ) && IsThisVehicleAccessibleToSoldier( pSoldier, bParam ) )
					{
//						if ( IsEnoughSpaceInVehicle( bParam ) )
						{
							// if the vehicle is FULL, then this will return FALSE!
							fItWorked = PutSoldierInVehicle( pSoldier, bParam );
							// failure produces its own error popup
							fNotifiedOfFailure = TRUE;
						}
					}
					break;
				case( REPAIR ):
					if( CanCharacterRepair( pSoldier ) )
					{
						BOOLEAN fCanFixSpecificTarget = TRUE;

						// make sure he can repair the SPECIFIC thing being repaired too (must be in its sector, for example)

/*
						if ( pSelectedSoldier->flags.fFixingSAMSite )
						{
							fCanFixSpecificTarget = CanSoldierRepairSAM( pSoldier, SAM_SITE_REPAIR_DIVISOR );
						}
						else
*/
						if ( pSelectedSoldier->bVehicleUnderRepairID != -1 )
						{
							fCanFixSpecificTarget = CanCharacterRepairVehicle( pSoldier, pSelectedSoldier->bVehicleUnderRepairID );
						}
						else if( pSoldier->flags.fFixingRobot )
						{
							fCanFixSpecificTarget = CanCharacterRepairRobot( pSoldier );
						}

						if ( fCanFixSpecificTarget )
						{
							// set as repair
							pSoldier->bOldAssignment = pSoldier->bAssignment;
							SetSoldierAssignment( pSoldier, REPAIR, pSelectedSoldier->flags.fFixingSAMSite, pSelectedSoldier->flags.fFixingRobot, pSelectedSoldier->bVehicleUnderRepairID );
							fItWorked = TRUE;
						}
					}
					break;
				case ( RADIO_SCAN ):
					if ( pSoldier->CanUseRadio() )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, RADIO_SCAN, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case( TRAIN_SELF ):
					if( CanCharacterTrainStat( pSoldier, bParam , TRUE, FALSE ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, TRAIN_SELF, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case( TRAIN_TOWN ):
					if( CanCharacterTrainMilitia( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, TRAIN_TOWN, 0, 0, 0 );
						fItWorked = TRUE;
					}
					break;
				case( TRAIN_MOBILE ):
					if( CanCharacterTrainMobileMilitia( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, TRAIN_MOBILE, 0, 0, 0 );
						fItWorked = TRUE;
					}
					break;
				case( TRAIN_TEAMMATE ):
					if( CanCharacterTrainStat( pSoldier, bParam, FALSE, TRUE ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, TRAIN_TEAMMATE, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case TRAIN_BY_OTHER:
					if( CanCharacterTrainStat( pSoldier, bParam, TRUE, FALSE ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, TRAIN_BY_OTHER, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;

				case MOVE_EQUIPMENT:
					//if( CanCharacterTrainStat( pSoldier, bParam, TRUE, FALSE ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, MOVE_EQUIPMENT, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;

				case SNITCH_SPREAD_PROPAGANDA:
					if( CanCharacterSpreadPropaganda( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, SNITCH_SPREAD_PROPAGANDA, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;

				case SNITCH_GATHER_RUMOURS:
					if( CanCharacterGatherInformation( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, SNITCH_GATHER_RUMOURS, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;


				// HEADROCK HAM 3.6: Facility Staffing
				case FACILITY_STAFF:
					if ( CanCharacterFacility( pSoldier, bParam, FAC_STAFF ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						ChangeSoldiersAssignment( pSoldier, FACILITY_STAFF );
						pSoldier->sFacilityTypeOperated = bParam;
						fItWorked = TRUE;
					}
					break;

				case FACILITY_INTERROGATE_PRISONERS:
					if ( CanCharacterFacility( pSoldier, bParam, FAC_INTERROGATE_PRISONERS ) && pSoldier->CanProcessPrisoners() )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						MakeSoldierKnownAsMercInPrison( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY );
						ChangeSoldiersAssignment( pSoldier, FACILITY_INTERROGATE_PRISONERS );
						pSoldier->sFacilityTypeOperated = bParam;
						fItWorked = TRUE;
					}
					break;

				case FACILITY_PRISON_SNITCH:
					if( CanCharacterFacility( pSoldier, bParam, FAC_PRISON_SNITCH ) && CanCharacterSnitchInPrison( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, FACILITY_PRISON_SNITCH, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;

				case FACILITY_SPREAD_PROPAGANDA:
					if( CanCharacterFacility( pSoldier, bParam, FAC_SPREAD_PROPAGANDA ) && CanCharacterSpreadPropaganda( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, FACILITY_SPREAD_PROPAGANDA, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case FACILITY_SPREAD_PROPAGANDA_GLOBAL:
					if( CanCharacterFacility( pSoldier, bParam, FAC_SPREAD_PROPAGANDA_GLOBAL ) && CanCharacterSpreadPropaganda( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, FACILITY_SPREAD_PROPAGANDA_GLOBAL, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case FACILITY_GATHER_RUMOURS:
					if( CanCharacterFacility( pSoldier, bParam, FAC_GATHER_RUMOURS ) && CanCharacterGatherInformation( pSoldier ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, FACILITY_GATHER_RUMOURS, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case FACILITY_STRATEGIC_MILITIA_MOVEMENT:
					if( CanCharacterFacility( pSoldier, bParam, FAC_STRATEGIC_MILITIA_MOVEMENT ) )
					{
						pSoldier->bOldAssignment = pSoldier->bAssignment;
						SetSoldierAssignment( pSoldier, FACILITY_STRATEGIC_MILITIA_MOVEMENT, bParam, 0,0 );
						fItWorked = TRUE;
					}
					break;
				case( SQUAD_1 ):
				case( SQUAD_2 ):
				case( SQUAD_3 ):
				case( SQUAD_4 ):
				case( SQUAD_5 ):
				case( SQUAD_6 ):
				case( SQUAD_7 ):
				case( SQUAD_8 ):
				case( SQUAD_9 ):
				case( SQUAD_10 ):
				case( SQUAD_11 ):
				case( SQUAD_12 ):
				case( SQUAD_13 ):
				case( SQUAD_14 ):
				case( SQUAD_15 ):
				case( SQUAD_16 ):
				case( SQUAD_17 ):
				case( SQUAD_18 ):
				case( SQUAD_19 ):
				case( SQUAD_20 ):
					bCanJoinSquad = CanCharacterSquad( pSoldier, ( INT8 )bAssignment );

					// if already in it, don't repor that as an error...
					if ( ( bCanJoinSquad == CHARACTER_CAN_JOIN_SQUAD ) ||
							( bCanJoinSquad == CHARACTER_CANT_JOIN_SQUAD_ALREADY_IN_IT ) )
					{
						if ( bCanJoinSquad == CHARACTER_CAN_JOIN_SQUAD )
						{
							pSoldier->bOldAssignment = pSoldier->bAssignment;

							// is the squad between sectors
							if( Squad[ bAssignment ][ 0 ] )
							{
								if( Squad[ bAssignment ][ 0 ]->flags.fBetweenSectors )
								{
									// between sectors, remove from old mvt group
									if ( pSoldier->bOldAssignment >= ON_DUTY )
									{
										// remove from group
										// the guy wasn't in a sqaud, but moving through a sector?
										if ( pSoldier->ubGroupID != 0 )
										{
											// now remove from mvt group
											RemovePlayerFromGroup( pSoldier->ubGroupID, pSoldier );
										}
									}
								}
							}

							if( pSoldier->bOldAssignment == VEHICLE )
							{
								TakeSoldierOutOfVehicle( pSoldier );
							}
							// remove from current squad, if any
							RemoveCharacterFromSquads( pSoldier );

							// able to add, do it
							AddCharacterToSquad( pSoldier, bAssignment );
						}

						fItWorked = TRUE;
						fRemoveFromSquad = FALSE;	// already done, would screw it up!
					}
					break;

				default:
					// remove from current vehicle/squad, if any
					if( pSoldier->bAssignment == VEHICLE )
					{
						TakeSoldierOutOfVehicle( pSoldier );
					}
					RemoveCharacterFromSquads( pSoldier );

					AddCharacterToAnySquad( pSoldier );

					fItWorked = TRUE;
					fRemoveFromSquad = FALSE;	// already done, would screw it up!
					break;
			}

			if ( fItWorked )
			{
				if ( fRemoveFromSquad )
				{
					// remove him from his old squad if he was on one
					RemoveCharacterFromSquads( pSoldier );
				}

				MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );
			}
			else
			{
				// didn't work - report it once
				if( !fNotifiedOfFailure )
				{
					fNotifiedOfFailure = TRUE;
					NotifyPlayerOfAssignmentAttemptFailure( bAssignment );
				}
			}
		}
	}
	// reset list
//	ResetSelectedListForMapScreen( );

	//CHRISL: When setting mercs to a squad, resort the Squad list so we're in ubID order so team panel appears the same
	//	when switching between squads
	switch( bAssignment )
	{
		case( SQUAD_1 ):
		case( SQUAD_2 ):
		case( SQUAD_3 ):
		case( SQUAD_4 ):
		case( SQUAD_5 ):
		case( SQUAD_6 ):
		case( SQUAD_7 ):
		case( SQUAD_8 ):
		case( SQUAD_9 ):
		case( SQUAD_10 ):
		case( SQUAD_11 ):
		case( SQUAD_12 ):
		case( SQUAD_13 ):
		case( SQUAD_14 ):
		case( SQUAD_15 ):
		case( SQUAD_16 ):
		case( SQUAD_17 ):
		case( SQUAD_18 ):
		case( SQUAD_19 ):
		case( SQUAD_20 ):
			SortSquadByID(bAssignment);
			break;
	}


	// check if we should start/stop flashing any mercs' assignment strings after these changes
	gfReEvaluateEveryonesNothingToDo = TRUE;

	return;
}


BOOLEAN IsCharacterAliveAndConscious( SOLDIERTYPE *pCharacter )
{
	// is the character alive and conscious?
	if( pCharacter->stats.bLife < CONSCIOUSNESS )
	{
		return( FALSE );
	}

	return ( TRUE );
}



BOOLEAN ValidTrainingPartnerInSameSectorOnAssignmentFound( SOLDIERTYPE *pTargetSoldier, INT8 bTargetAssignment, INT8 bTargetStat )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sTrainingPts = 0;
	UINT16 usMaxPts;


	// this function only makes sense for training teammates or by others, not for self training which doesn't require partners
	Assert( ( bTargetAssignment == TRAIN_TEAMMATE ) || ( bTargetAssignment == TRAIN_BY_OTHER ) );

	for( iCounter = 0; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
	{
		pSoldier = &Menptr[ iCounter ];

		if ( pSoldier->bActive )
		{
			// if the guy is not the target, has the assignment we want, is training the same stat, and is in our sector, alive
			// and is training the stat we want
			if( ( pSoldier != pTargetSoldier ) &&
					( pSoldier->bAssignment == bTargetAssignment ) &&
					// CJC: this seems incorrect in light of the check for bTargetStat and in any case would
					// cause a problem if the trainer was assigned and we weren't!
					//( pSoldier->bTrainStat == pTargetSoldier->bTrainStat ) &&
					( pSoldier->sSectorX == pTargetSoldier->sSectorX ) &&
					( pSoldier->sSectorY == pTargetSoldier->sSectorY ) &&
					( pSoldier->bSectorZ == pTargetSoldier->bSectorZ ) &&
					( pSoldier->bTrainStat == bTargetStat ) &&
					( pSoldier->stats.bLife > 0 ) )
			{
				// so far so good, now let's see if the trainer can really teach the student anything new

				if ( pSoldier->bAssignment == TRAIN_TEAMMATE )
				{
					// pSoldier is the instructor, target is the student
					sTrainingPts = GetBonusTrainingPtsDueToInstructor( pSoldier, pTargetSoldier, bTargetStat, &usMaxPts );
				}
				else
				{
					// target is the instructor, pSoldier is the student
					sTrainingPts = GetBonusTrainingPtsDueToInstructor( pTargetSoldier, pSoldier, bTargetStat, &usMaxPts );
				}

				if ( sTrainingPts > 0 )
				{
					// yes, then he makes a valid training partner for us!
					return( TRUE );
				}
			}
		}
	}

	// no one found
	return( FALSE );
}

void UnEscortEPC( SOLDIERTYPE *pSoldier )
{
	if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
	{
	BOOLEAN fGotInfo;
	UINT16 usQuoteNum;
	UINT16 usFactToSetToTrue;

	SetupProfileInsertionDataForSoldier( pSoldier );

	fGotInfo = GetInfoForAbandoningEPC( pSoldier->ubProfile, &usQuoteNum, &usFactToSetToTrue );
	if ( fGotInfo )
	{
		// say quote usQuoteNum
		gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags |= PROFILE_MISC_FLAG_FORCENPCQUOTE;
		TacticalCharacterDialogue( pSoldier, usQuoteNum );
			// the flag will be turned off in the remove-epc event
		//gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags &= ~PROFILE_MISC_FLAG_FORCENPCQUOTE;
		SetFactTrue( usFactToSetToTrue );
	}
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_REMOVE_EPC, pSoldier->ubProfile, 0, 0, 0, 0 );

	HandleFactForNPCUnescorted( pSoldier->ubProfile );

	if ( pSoldier->ubProfile == JOHN )
	{
		SOLDIERTYPE * pSoldier2;

		// unrecruit Mary as well
		pSoldier2 = FindSoldierByProfileID( MARY, TRUE );
		if ( pSoldier2 )
		{
			SetupProfileInsertionDataForSoldier( pSoldier2 );
				fGotInfo = GetInfoForAbandoningEPC( MARY, &usQuoteNum, &usFactToSetToTrue );
				if ( fGotInfo )
				{
					// say quote usQuoteNum
					gMercProfiles[ MARY ].ubMiscFlags |= PROFILE_MISC_FLAG_FORCENPCQUOTE;
					TacticalCharacterDialogue( pSoldier2, usQuoteNum );
					// the flag will be turned off in the remove-epc event
					//gMercProfiles[ MARY ].ubMiscFlags &= ~PROFILE_MISC_FLAG_FORCENPCQUOTE;
					SetFactTrue( usFactToSetToTrue );
				}

			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_REMOVE_EPC, MARY, 0, 0, 0, 0 );
		}
	}
	else if ( pSoldier->ubProfile == MARY )
	{
		SOLDIERTYPE * pSoldier2;

		// unrecruit John as well
		pSoldier2 = FindSoldierByProfileID( JOHN, TRUE );
		if ( pSoldier2 )
		{
			SetupProfileInsertionDataForSoldier( pSoldier2 );
				fGotInfo = GetInfoForAbandoningEPC( JOHN, &usQuoteNum, &usFactToSetToTrue );
				if ( fGotInfo )
				{
					// say quote usQuoteNum
					gMercProfiles[ JOHN ].ubMiscFlags |= PROFILE_MISC_FLAG_FORCENPCQUOTE;
					TacticalCharacterDialogue( pSoldier2, usQuoteNum );
					// the flag will be turned off in the remove-epc event
					//gMercProfiles[ JOHN ].ubMiscFlags &= ~PROFILE_MISC_FLAG_FORCENPCQUOTE;
					SetFactTrue( usFactToSetToTrue );
				}
			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_REMOVE_EPC, JOHN, 0, 0, 0, 0 );
		}
	}
	// stop showing menu
	giAssignHighLine = -1;

	// set dirty flag
	fTeamPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
	// how do we handle this if it's the right sector?
	TriggerNPCWithGivenApproach( pSoldier->ubProfile, APPROACH_EPC_IN_WRONG_SECTOR, TRUE );
	}
}


BOOLEAN CharacterIsTakingItEasy( SOLDIERTYPE *pSoldier )
{
	// actually asleep?
	if ( pSoldier->flags.fMercAsleep == TRUE )
	{
		return( TRUE );
	}

	// if able to sleep
	if ( CanCharacterSleep( pSoldier, FALSE ) )
	{
		// on duty, but able to catch naps (either not traveling, or not the driver of the vehicle)
		// The actual checks for this are in the "can he sleep" check above
		if ( ( pSoldier->bAssignment < ON_DUTY ) || ( pSoldier->bAssignment == VEHICLE ) )
		{
			return( TRUE );
		}

		// and healing up?
		if ( ( pSoldier->bAssignment == PATIENT ) || ( pSoldier->bAssignment == ASSIGNMENT_HOSPITAL ) )
		{
			return( TRUE );
		}

		// HEADROCK HAM 2.8/HAM 3.6: Trainers whose trainees are all asleep will not become fatigued.
		if ( pSoldier->bAssignment == TRAIN_TEAMMATE && (gGameExternalOptions.ubSmartTrainingRest == 1 || gGameExternalOptions.ubSmartTrainingRest == 2))
		{
			if (FindAnyAwakeTrainees( pSoldier ) == FALSE)
			{
				return( TRUE );
			}
		}

		// HEADROCK HAM 2.8/HAM 3.6: Characters in training whose trainer is asleep will not be fatigued.
		if ( pSoldier->bAssignment == TRAIN_BY_OTHER && (gGameExternalOptions.ubSmartTrainingRest == 1 || gGameExternalOptions.ubSmartTrainingRest == 3))
		{
			if (FindAnyAwakeTrainers( pSoldier ) == FALSE)
			{
				return( TRUE );
			}
			// silversurfer: Fix for JaggZilla bug #591. This guy is set as student and there is a trainer available that is awake.
			// Even if the trainer is not good enough this guy will still practice and therefore letting him fall through to 
			// pSoldier->flags.fDoneAssignmentAndNothingToDoFlag will allow him to practice without breath loss. Lame exploit...
			else
				return( FALSE );
		}
		
		// HEADROCK HAM 3.6: Added new resting assignment for facilities only.
		if ( pSoldier->bAssignment == FACILITY_REST )
		{
			return( TRUE );
		}

		// on a real assignment, but done with it?
		if ( pSoldier->flags.fDoneAssignmentAndNothingToDoFlag )
		{
			return( TRUE );
		}
	}


	// on assignment, or walking/driving & unable to sleep
	return( FALSE );
}



UINT8 CalcSoldierNeedForSleep( SOLDIERTYPE *pSoldier )
{
	UINT8 ubNeedForSleep;
	UINT8 ubPercentHealth;


	// base comes from profile
	ubNeedForSleep = gMercProfiles[ pSoldier->ubProfile ].ubNeedForSleep;

	// Enforce a maximum of 12 hours before injury penalties.
	if ( ubNeedForSleep > 12 )
	{
		ubNeedForSleep = 12;
	}

	// Enforce a minimum of 3 hours, before night/day considerations.
	if ( ubNeedForSleep < 3 )
	{
		ubNeedForSleep = 3;
	}
	// HEADROCK HAM 3.5: WTF! This calculation is NOT correct!
	//ubPercentHealth = pSoldier->stats.bLife / pSoldier->stats.bLifeMax;
	ubPercentHealth = (pSoldier->stats.bLife*100) / pSoldier->stats.bLifeMax;

	// Increase need for sleep based on injuries.
	if ( ubPercentHealth < 75 )
	{
		ubNeedForSleep++;

		if ( ubPercentHealth < 50 )
		{
			ubNeedForSleep += 2; // 3 extra hours a day

			if ( ubPercentHealth < 25 )
			{
				ubNeedForSleep += 4; // 7 extra hours a day
			}
		}
	}
	
	ubNeedForSleep += pSoldier->GetBackgroundValue(BG_PERC_SLEEP);

	// Re-Enforce a maximum of 18 hours after injury penalties.
	if ( ubNeedForSleep > 18 )
	{
		ubNeedForSleep = 18;
	}

	// reduce for Night Ops trait	
	// SANDRO - new traits
	if (gGameOptions.fNewTraitSystem)
	{
		if (HAS_SKILL_TRAIT( pSoldier, NIGHT_OPS_NT ))
			ubNeedForSleep -= gSkillTraitValues.ubNONeedForSleepReduction;

		if ( ubNeedForSleep < 3 )
			ubNeedForSleep = 3;
	}
	else
	{
		// HEADROCK HAM 3.6: This is now split and applied depending on whether the merc is resting or working.
		//ubNeedForSleep -= NUM_SKILL_TRAITS( pSoldier, NIGHTOPS );
		ubNeedForSleep -= NUM_SKILL_TRAITS( pSoldier, MARTIALARTS_OT );

		if ( ubNeedForSleep < 3 )
			ubNeedForSleep = 3;
	}

	// Flugente: ubNeedForSleep can now be influenced by our food situation
	if ( gGameOptions.fFoodSystem )
		FoodNeedForSleepModifiy(pSoldier, &ubNeedForSleep);

	return( ubNeedForSleep );
}



UINT32 GetLastSquadListedInSquadMenu( void )
{
	UINT32 uiMaxSquad;

	uiMaxSquad = GetLastSquadActive( ) + 1;

	if ( uiMaxSquad >= NUMBER_OF_SQUADS )
	{
		uiMaxSquad = NUMBER_OF_SQUADS - 1;
	}

	// Flugente: if using xml squad names, always show all squads - people will propably want to use them
	if ( gGameExternalOptions.fUseXMLSquadNames )
		uiMaxSquad = NUMBER_OF_SQUADS - 1;

	return( uiMaxSquad );
}



BOOLEAN CanCharacterRepairAnotherSoldiersStuff( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOtherSoldier )
{
	if ( pOtherSoldier == pSoldier )
	{
		return( FALSE );
	}
	if ( !pOtherSoldier->bActive )
	{
		return( FALSE );
	}
	if ( pOtherSoldier->stats.bLife == 0 )
	{
		return( FALSE );
	}
	if ( pOtherSoldier->sSectorX != pSoldier->sSectorX ||
			pOtherSoldier->sSectorY != pSoldier->sSectorY ||
			pOtherSoldier->bSectorZ != pSoldier->bSectorZ )
	{
		return( FALSE );
	}

	if ( pOtherSoldier->flags.fBetweenSectors )
	{
		return( FALSE );
	}

	if ( ( pOtherSoldier->bAssignment == IN_TRANSIT ) ||
			( pOtherSoldier->bAssignment == ASSIGNMENT_POW ) ||
			( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ||
			( AM_A_ROBOT( pSoldier ) ) ||
			( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC ) ||
			( pOtherSoldier->bAssignment == ASSIGNMENT_DEAD ) )
	{
		return( FALSE );
	}

	return( TRUE );
}



SOLDIERTYPE *GetSelectedAssignSoldier( BOOLEAN fNullOK )
{
	SOLDIERTYPE *pSoldier = NULL;

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		// mapscreen version
		if( ( bSelectedAssignChar >= 0 ) && ( bSelectedAssignChar < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS ) &&
				( gCharactersList[ bSelectedAssignChar ].fValid ) )
		{
			pSoldier = &Menptr[ gCharactersList[ bSelectedAssignChar ].usSolID ];
		}
	}
	else
	{
		// tactical version
		pSoldier = &Menptr[ gusUIFullTargetID ];
	}

	if ( !fNullOK )
	{
		Assert( pSoldier );
	}

	if ( pSoldier != NULL )
	{
		// better be an active person, not a vehicle
		Assert( pSoldier->bActive );
		Assert( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) );
	}

	return( pSoldier );
}



void ResumeOldAssignment( SOLDIERTYPE *pSoldier )
{
	AddCharacterToAnySquad( pSoldier );

	// make sure the player has time to OK this before proceeding
	StopTimeCompression();

	// assignment has changed, redraw left side as well as the map (to update on/off duty icons)
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	fMapPanelDirty = TRUE;
}



void RepairItemsOnOthers( SOLDIERTYPE *pSoldier, UINT8 *pubRepairPtsLeft )
{
	UINT8 ubPassType;
	INT8 bLoop;
	SOLDIERTYPE * pOtherSoldier;
	SOLDIERTYPE * pBestOtherSoldier;
	INT8 bPriority, bBestPriority = -1;
	BOOLEAN fSomethingWasRepairedThisPass;


	// repair everyone's hands and armor slots first, then headgear, and pockets last
	for ( ubPassType = REPAIR_HANDS_AND_ARMOR; ubPassType <= FINAL_REPAIR_PASS; ubPassType++ )
	{
		fSomethingWasRepairedThisPass = FALSE;


		// look for jammed guns on other soldiers in sector and unjam them
		for( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop < gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
		{
			pOtherSoldier = MercPtrs[ bLoop ];

			// check character is valid, alive, same sector, not between, has inventory, etc.
			if ( CanCharacterRepairAnotherSoldiersStuff( pSoldier, pOtherSoldier ) )
			{
				if ( UnjamGunsOnSoldier( pOtherSoldier, pSoldier, pubRepairPtsLeft ) )
				{
					fSomethingWasRepairedThisPass = TRUE;
				}
			}
		}


		while ( *pubRepairPtsLeft > 0 )
		{
			bBestPriority = -1;
			pBestOtherSoldier = NULL;

			// now look for items to repair on other mercs
			for( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop < gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
			{
				pOtherSoldier = MercPtrs[ bLoop ];

				// check character is valid, alive, same sector, not between, has inventory, etc.
				if ( CanCharacterRepairAnotherSoldiersStuff( pSoldier, pOtherSoldier ) )
				{
					// okay, seems like a candidate!
					if ( FindRepairableItemOnOtherSoldier( pSoldier, pOtherSoldier, ubPassType ) != 0 )
					{
						bPriority = pOtherSoldier->stats.bExpLevel;
						if ( bPriority > bBestPriority )
						{
							bBestPriority = bPriority;
							pBestOtherSoldier = pOtherSoldier;
						}
					}
				}
			}

			// did we find anyone to repair on this pass?
			if ( pBestOtherSoldier != NULL )
			{
				// yes, repair all items (for this pass type!) on this soldier that need repair
				OBJECTTYPE * pObjectToRepair;
				do
				{
					pObjectToRepair = FindRepairableItemOnOtherSoldier( pSoldier, pBestOtherSoldier, ubPassType );
					if ( pObjectToRepair )
					{
						if ( RepairObject( pSoldier, pBestOtherSoldier, pObjectToRepair, pubRepairPtsLeft ) )
						{
							fSomethingWasRepairedThisPass = TRUE;
						}
					}
				}
				while ( pObjectToRepair && *pubRepairPtsLeft > 0 );
			}
			else
			{
				break;
			}
		}

		if ( fSomethingWasRepairedThisPass && !DoesCharacterHaveAnyItemsToRepair( pSoldier, ubPassType ) )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, sRepairsDoneString[ 1 + ubPassType ], pSoldier->GetName() );

			// let player react
			StopTimeCompression();
		}
	}
}



BOOLEAN UnjamGunsOnSoldier( SOLDIERTYPE *pOwnerSoldier, SOLDIERTYPE *pRepairSoldier, UINT8 *pubRepairPtsLeft )
{
	BOOLEAN fAnyGunsWereUnjammed = FALSE;
	INT8	bPocket;


	// try to unjam everything before beginning any actual repairs.. successful unjamming costs 2 points per weapon
	// CHRISL: Changed to dynamically determine max inventory locations.
	for (bPocket = HANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
	{
		// the object a weapon? and jammed?
		if ( ( Item[ pOwnerSoldier->inv[ bPocket ].usItem ].usItemClass == IC_GUN ) && ( pOwnerSoldier->inv[ bPocket ][0]->data.gun.bGunAmmoStatus < 0 ) )
		{
			if ( *pubRepairPtsLeft >= gGameExternalOptions.ubRepairCostPerJam )
			{
				*pubRepairPtsLeft -= gGameExternalOptions.ubRepairCostPerJam;

				pOwnerSoldier->inv [ bPocket ][0]->data.gun.bGunAmmoStatus *= -1;

				// MECHANICAL/DEXTERITY GAIN: Unjammed a gun
				StatChange( pRepairSoldier, MECHANAMT, 5, FALSE );
				StatChange( pRepairSoldier, DEXTAMT, 5, FALSE );

				// report it as unjammed
				if ( pRepairSoldier == pOwnerSoldier )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 53 ], pRepairSoldier->GetName(), ItemNames[ pOwnerSoldier->inv[ bPocket ].usItem ] );
				}
				else
				{
					// NOTE: may need to be changed for localized versions
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 54 ], pRepairSoldier->GetName(), pOwnerSoldier->GetName(), ItemNames[ pOwnerSoldier->inv[ bPocket ].usItem ] );
				}

				fAnyGunsWereUnjammed = TRUE;
			}
			else
			{
				// out of points, we're done for now
				break;
			}
		}
	}

	return ( fAnyGunsWereUnjammed );
}

// HEADROCK HAM B2.8: A set of functions to synchronize sleeping periods of trainers and trainees
BOOLEAN SetTrainerSleepWhenTraineesSleep( SOLDIERTYPE *pThisTrainee)
{
	UINT16 sMapX = pThisTrainee->sSectorX;
	UINT16 sMapY = pThisTrainee->sSectorY;
	UINT16 sMapZ = pThisTrainee->bSectorZ;
	UINT8 bStat = pThisTrainee->bTrainStat;
	INT32 iCounter, iNumberOnTeam;
	
	SOLDIERTYPE * pOtherTrainee;
	SOLDIERTYPE * pTrainer;
	BOOLEAN fAllTraineesAsleep = TRUE;
	BOOLEAN fTrainersSentToSleep = FALSE;

	if (pThisTrainee->bAssignment != TRAIN_BY_OTHER)
	{
		// Shouldn't happen...
		return (FALSE);
	}

	iNumberOnTeam =gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// Check to see if all other trainees of the same stat are also asleep
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pOtherTrainee = &Menptr[ iCounter ];
		if (pOtherTrainee->bAssignment == TRAIN_BY_OTHER && pOtherTrainee->bTrainStat == pThisTrainee->bTrainStat && 
			pOtherTrainee->sSectorX == sMapX && pOtherTrainee->sSectorY == sMapY && pOtherTrainee->bSectorZ == sMapZ &&			
			pOtherTrainee->bActive && !pOtherTrainee->flags.fMercAsleep )
		{
			// Trainee is present and awake. Flag is reset to false.
			fAllTraineesAsleep = FALSE;
		}
	}
	// If they are all asleep
	if (fAllTraineesAsleep)
	{
		// Look for trainers of that stat, in the same sector
		for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
		{
			pTrainer = &Menptr[ iCounter ];
			if (pTrainer->bAssignment == TRAIN_TEAMMATE && pTrainer->bTrainStat == pThisTrainee->bTrainStat && 
				pTrainer->sSectorX == sMapX && pTrainer->sSectorY == sMapY && pTrainer->bSectorZ == sMapZ &&	
				pTrainer->bActive && !pTrainer->flags.fMercAsleep )
			{
				// Trainer will go to sleep
				if( SetMercAsleep( pTrainer, FALSE ) )
				{
					if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
					{
						// tell player about it
						AddSoldierToWaitingListQueue( pTrainer );
					}
					
					// seems unnecessary now?	ARM
					pTrainer->bOldAssignment = pTrainer->bAssignment;
					
					fTrainersSentToSleep = TRUE;
				}
			}
		}
		if (fTrainersSentToSleep)
		{
			return(TRUE);
		}
		else
		{
			return(FALSE);
		}
	}
	else
	{
		return(FALSE);
	}
}


BOOLEAN SetTraineesSleepWhenTrainerSleeps( SOLDIERTYPE *pTrainer)
{
	UINT16 sMapX = pTrainer->sSectorX;
	UINT16 sMapY = pTrainer->sSectorY;
	UINT16 sMapZ = pTrainer->bSectorZ;
	UINT8 bStat = pTrainer->bTrainStat;
	INT32 iCounter, iNumberOnTeam;
	BOOLEAN fTraineesSentToSleep = FALSE;

	SOLDIERTYPE * pTrainee;

	if (pTrainer->bAssignment != TRAIN_TEAMMATE)
	{
		// Shouldn't happen...
		return(FALSE);
	}

	iNumberOnTeam =gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pTrainee = &Menptr[ iCounter ];
		if (pTrainee->bAssignment == TRAIN_BY_OTHER && pTrainee->bTrainStat == pTrainer->bTrainStat && 
			pTrainee->sSectorX == sMapX && pTrainee->sSectorY == sMapY && pTrainee->bSectorZ == sMapZ &&
			pTrainee->bActive && !pTrainee->flags.fMercAsleep )
		{
			// Trainee will go to sleep
			if( SetMercAsleep( pTrainee, FALSE ) )
			{
				if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
				{
					// tell player about it
					AddSoldierToWaitingListQueue( pTrainee );
				}
				
				// seems unnecessary now?	ARM
				pTrainee->bOldAssignment = pTrainee->bAssignment;
				
				fTraineesSentToSleep = TRUE;
			}
		}
	}

	if (fTraineesSentToSleep)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}


BOOLEAN SetTrainerWakeWhenTraineesWake( SOLDIERTYPE *pThisTrainee)
{
	UINT16 sMapX = pThisTrainee->sSectorX;
	UINT16 sMapY = pThisTrainee->sSectorY;
	UINT16 sMapZ = pThisTrainee->bSectorZ;
	UINT8 bStat = pThisTrainee->bTrainStat;
	INT32 iCounter, iNumberOnTeam;
	
	SOLDIERTYPE * pOtherTrainee;
	SOLDIERTYPE * pTrainer;
	BOOLEAN fAllTraineesAwake = TRUE;
	BOOLEAN fTrainersWokenUp = FALSE;

	if (pThisTrainee->bAssignment != TRAIN_BY_OTHER)
	{
		// Shouldn't happen...
		return (FALSE);
	}

	iNumberOnTeam =gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// Check to see if all other trainees of the same stat are also asleep
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pOtherTrainee = &Menptr[ iCounter ];
		if (pOtherTrainee->bAssignment == TRAIN_BY_OTHER && pOtherTrainee->bTrainStat == pThisTrainee->bTrainStat && 
			pOtherTrainee->sSectorX == sMapX && pOtherTrainee->sSectorY == sMapY && pOtherTrainee->bSectorZ == sMapZ &&			
			pOtherTrainee->bActive && pOtherTrainee->flags.fMercAsleep )
		{
			// Trainee is present and asleep. Flag is reset to FALSE.
			fAllTraineesAwake = FALSE;
		}
	}
	// If they are all awake
	if (fAllTraineesAwake)
	{
		// Look for trainers of that stat, in the same sector
		for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
		{
			pTrainer = &Menptr[ iCounter ];
			if (pTrainer->bAssignment == TRAIN_TEAMMATE && pTrainer->bTrainStat == pThisTrainee->bTrainStat && 
				pTrainer->sSectorX == sMapX && pTrainer->sSectorY == sMapY && pTrainer->bSectorZ == sMapZ &&	
				pTrainer->bActive && pTrainer->flags.fMercAsleep )
			{
				// Trainer will wake up
				if( SetMercAwake( pTrainer, FALSE, FALSE ) )
				{
					if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
					{
						// tell player about it
						AddSoldierToWaitingListQueue( pTrainer );
					}
					
					// seems unnecessary now?	ARM
					pTrainer->bOldAssignment = pTrainer->bAssignment;
					
					fTrainersWokenUp = TRUE;
				}
			}
		}
		if (fTrainersWokenUp)
		{
			return(TRUE);
		}
		else
		{
			return(FALSE);
		}
	}
	else
	{
		return(FALSE);
	}
}

BOOLEAN SetTraineesWakeWhenTrainerWakes( SOLDIERTYPE *pTrainer)
{
	UINT16 sMapX = pTrainer->sSectorX;
	UINT16 sMapY = pTrainer->sSectorY;
	UINT16 sMapZ = pTrainer->bSectorZ;
	UINT8 bStat = pTrainer->bTrainStat;
	INT32 iCounter, iNumberOnTeam;
	BOOLEAN fTraineesWokenUp = FALSE;

	SOLDIERTYPE * pTrainee;

	if (pTrainer->bAssignment != TRAIN_TEAMMATE)
	{
		// Shouldn't happen...
		return(FALSE);
	}

	iNumberOnTeam =gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pTrainee = &Menptr[ iCounter ];
		if (pTrainee->bAssignment == TRAIN_BY_OTHER && pTrainee->bTrainStat == pTrainer->bTrainStat && 
			pTrainee->sSectorX == sMapX && pTrainee->sSectorY == sMapY && pTrainee->bSectorZ == sMapZ &&
			pTrainee->bActive && pTrainee->flags.fMercAsleep )
		{
			// Trainee will wake up
			if( SetMercAwake( pTrainee, FALSE, FALSE ) )
			{
				if( gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ] )
				{
					// tell player about it
					AddSoldierToWaitingListQueue( pTrainee );
				}
				
				// seems unnecessary now?	ARM
				pTrainee->bOldAssignment = pTrainee->bAssignment;
				
				fTraineesWokenUp = TRUE;
			}
		}
	}

	if (fTraineesWokenUp)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

void HandleTrainingSleepSynchronize( SOLDIERTYPE *pSoldier )
{
	// HEADROCK HAM B2.8: Trainees will now go to sleep if the trainer goes to sleep.
	if ((gGameExternalOptions.ubSmartTrainingSleep == 1 || gGameExternalOptions.ubSmartTrainingSleep == 2) && pSoldier->bAssignment == TRAIN_TEAMMATE)
	{
		SetTraineesSleepWhenTrainerSleeps( pSoldier );
	}
		
	// HEADROCK HAM B2.8: If this is a trainee, and all other trainees are already asleep, put all trainers to sleep as well.
	if ( (gGameExternalOptions.ubSmartTrainingSleep == 1 || gGameExternalOptions.ubSmartTrainingSleep == 3 ) && pSoldier->bAssignment == TRAIN_BY_OTHER)
	{
		SetTrainerSleepWhenTraineesSleep( pSoldier );
	}
}

void HandleTrainingWakeSynchronize( SOLDIERTYPE *pSoldier )
{
	// HEADROCK HAM B2.8: Trainees will now go to sleep if the trainer goes to sleep.
	if ((gGameExternalOptions.ubSmartTrainingWake == 1 || gGameExternalOptions.ubSmartTrainingWake == 2) && pSoldier->bAssignment == TRAIN_TEAMMATE)
	{
		SetTraineesWakeWhenTrainerWakes( pSoldier );
	}
		
	// HEADROCK HAM B2.8: If this is a trainee, and all other trainees are already asleep, put all trainers to sleep as well.
	if ( (gGameExternalOptions.ubSmartTrainingWake == 1 || gGameExternalOptions.ubSmartTrainingWake == 3 ) && pSoldier->bAssignment == TRAIN_BY_OTHER)
	{
		SetTrainerWakeWhenTraineesWake( pSoldier );
	}
}

BOOLEAN FindAnyAwakeTrainers( SOLDIERTYPE *pTrainee )
{
	UINT16 sMapX = pTrainee->sSectorX;
	UINT16 sMapY = pTrainee->sSectorY;
	UINT16 sMapZ = pTrainee->bSectorZ;
	UINT8 bStat = pTrainee->bTrainStat;
	INT32 ubCounter = 0;
	BOOLEAN fAllTrainersAsleep = TRUE;

	SOLDIERTYPE * pTrainer;

	if (pTrainee->bAssignment != TRAIN_BY_OTHER)
	{
		// Shouldn't happen...
		return(FALSE);
	}

	while(gCharactersList[ubCounter].fValid)
	{
		pTrainer = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];
			
		// Is trainer awake?
		if (pTrainer->bAssignment == TRAIN_TEAMMATE && pTrainer->bTrainStat == pTrainee->bTrainStat && 
			pTrainer->sSectorX == sMapX && pTrainer->sSectorY == sMapY && pTrainer->bSectorZ == sMapZ &&
			pTrainer->bActive && !pTrainer->flags.fMercAsleep )
		{
			// Reset flag
			fAllTrainersAsleep = FALSE;
		}
		ubCounter++;
	}

	return(!fAllTrainersAsleep);
}

BOOLEAN FindAnyAwakeTrainees( SOLDIERTYPE *pTrainer )
{
	UINT16 sMapX = pTrainer->sSectorX;
	UINT16 sMapY = pTrainer->sSectorY;
	UINT16 sMapZ = pTrainer->bSectorZ;
	UINT8 bStat = pTrainer->bTrainStat;
	INT32 ubCounter = 0;
	BOOLEAN fAllTraineesAsleep = TRUE;

	SOLDIERTYPE * pTrainee;

	if (pTrainer->bAssignment != TRAIN_TEAMMATE)
	{
		// Shouldn't happen...
		return(FALSE);
	}

	while(gCharactersList[ubCounter].fValid)
	{
		pTrainee = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];
			
		// Is trainee awake?
		if (pTrainee->bAssignment == TRAIN_BY_OTHER && pTrainee->bTrainStat == pTrainer->bTrainStat && 
			pTrainee->sSectorX == sMapX && pTrainee->sSectorY == sMapY && pTrainee->bSectorZ == sMapZ &&
			pTrainee->bActive && !pTrainee->flags.fMercAsleep )
		{
			// Reset flag.
			fAllTraineesAsleep = FALSE;
		}
		ubCounter++;
	}

	return(!fAllTraineesAsleep);
}
// HEADROCK HAM 3.6: A new set of functions (this & next) to determine whether a character can train Mobile Militia.
// This function tests character statistics.

BOOLEAN BasicCanCharacterTrainMobileMilitia( SOLDIERTYPE *pSoldier )
{

	/////////////////////////////////////////////////////
	// Tests whether character can do assignments at all!

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// Mobile Training allowed by INI settings?
	if (!gGameExternalOptions.gfmusttrainroaming || // Mobiles turned off?
		GetWorldDay( ) < gGameExternalOptions.guiAllowMilitiaGroupsDelay || !gGameExternalOptions.gfAllowMilitiaGroups) // Mobiles not yet available?
	{
		// No Mobile Militia training allowed!
		return ( FALSE );
	}

	// Is character dead or unconscious?
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// Is character underground?
	if( pSoldier->bSectorZ != 0 )
	{
		// underground training is not allowed (code doesn't support and it's a reasonable enough limitation)
		return( FALSE );
	}

	// Is character on the way into/out of Arulco?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// Is character travelling between sectors?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// Is character an Escortee?
	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// Is character a Vehicle or Robot?
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}

	// IS character inside a helicopter over a hostile sector?
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Tests to see whether this sector allows training militia for ANYBODY.

	// is there a city in the character's current sector?
	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	if (!bTownId)
	{
		// No city? No Mobiles.
		return ( FALSE );
	}
	else
	{
		// There's a city here. Does it allow training militia?
		if (!MilitiaTrainingAllowedInTown( bTownId ))
		{
			// City does not allow militia training at all.
			return ( FALSE );
		}
	}

	// HEADROCK HAM 3.5: Only facilities allow militia training, and determine how many trainers can work here.
	// Does sector have at least one facility that allows training?
	// HEADROCK HAM 3.5: Only facilities allow militia training, and determine how many trainers can work here.
	// Does sector have at least one facility that allows training?
	BOOLEAN fMobileMilitiaTrainingAllowed = FALSE;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Does it allow training militia?
			if (gFacilityTypes[cnt].ubMobileMilitiaTrainersAllowed)
			{
				// Cool.
				fMobileMilitiaTrainingAllowed = TRUE;
			}
		}
	}
	if (!fMobileMilitiaTrainingAllowed)
	{
		// Mobile Militia training NOT allowed here!
		return ( FALSE );
	}

	return ( TRUE );
}

// HEADROCK HAM 3.6: A new set of functions (this & previous) to determine whether a character can train Mobile Militia.
// This function tests location.

BOOLEAN CanCharacterTrainMobileMilitia( SOLDIERTYPE *pSoldier )
{
	AssertNotNIL(pSoldier);

	// Make sure the basic sector/merc variables are still applicable. This is simply a fail-safe.
	if( !BasicCanCharacterTrainMobileMilitia( pSoldier ) )
	{
		// Soldier/Sector have somehow failed the basic test. Character automatically fails this test as well.
		return( FALSE );
	}

	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}

	// Has leadership skill?
	if( pSoldier->stats.bLeadership <= 0 )
	{
		// no skill whatsoever
		return ( FALSE );
	}

	// Sector Loyalty above minimum?
	if( !DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) )
	{
		// Not enough Loyalty...
		return ( FALSE );
	}

	// HEADROCK HAM 3: When "Minimum Leadership for Militia Training" is enforced, this value holds the
	// merc's effective leadership, after the "TEACHER" trait is taken into account.
	UINT16 usEffectiveLeadership;

	// HEADROCK HAM 3: Determine whether the merc has enough leadership to train militia. The teacher trait may
	// increase or decrease the effective skill.
	if( gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia > 0 )
	{
		// Read BASE leadership
		usEffectiveLeadership = pSoldier->stats.bLeadership;
 
		// Apply modifier for TEACHER trait, if that feature is activated
		if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
		{
			// Read BASE leadership
			usEffectiveLeadership = pSoldier->stats.bLeadership;
	 
			if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits
			{
				if (HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ))
				{
					// bonus from Teaching trait
					usEffectiveLeadership = (usEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 );
				}
			}
			else
			{
				// Modifier applied once for each TEACHING level.
				for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pSoldier, TEACHING_OT ); i++ )
				{
					// This is a percentage modifier.
					usEffectiveLeadership = (usEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100;
				}
			}
		}
		
		usEffectiveLeadership = __min(100,usEffectiveLeadership);
		
		// Is leadership too low to proceed?
		if (usEffectiveLeadership < gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia)
		{
			return ( FALSE );
		}
	}

	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) ] );

	////////////////////////////////////////////////
	// Check whether controlled town sectors already have full militia

	// HEADROCK HAM 4: This check is no longer required. We can manually restrict mobiles from entering a city
	// after being created, so we should be able to train them if we need them straight away.

	/*

	INT32 iCounter = 0;
	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) ] );
	BOOLEAN fUnfullSectorFound = FALSE;

	if ( CountMilitia( pSectorInfo ) < gGameExternalOptions.iMaxMilitiaPerSector )
	{
		fUnfullSectorFound = TRUE;
	}
	else
	{
		iCounter = 0;
		while( pTownNamesList[ iCounter ] != 0 )
		{
			// Are we in this city?
			if( pTownNamesList[ iCounter] == bTownId )
			{
				INT16 sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
				INT16 sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

				pSectorInfo = &( SectorInfo[ SECTOR(sCurrentX, sCurrentY) ] );
				// if sector has enemies or hasn't already been taken at least once, then
				if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
					NumEnemiesInSector( sCurrentX, sCurrentY ) > 0 )
				{
					// skip the rest. This sector cannot generate militia anyway. 
					iCounter++;
					continue;
				}
				else
				{
					if (CountMilitia(pSectorInfo) < gGameExternalOptions.iMaxMilitiaPerSector )
					{
						// Found a controlled city sector that does not yet have a full garrison
						fUnfullSectorFound = TRUE;
					}
				}
			}
			iCounter++;
		}
	}
	if (fUnfullSectorFound)
	{
		// At least one city sector is controlled but not full of garrison militia. Can't train mobiles!
		return (FALSE);
	}
	*/

	//////////////////////////////////////////////
	// HEADROCK HAM 3.5: Militia Training Facility 
	//

	// Militia training is enabled in the sector only if there is a facility that allows this here. 
	// If one or more facilities are found, positive values are summed up and presented as the number 
	// of trainers allowed in the sector. Values are read from XML, and can be set to mimic JA2
	// defaults. This renders the INI setting "MAX_MILITIA_TRAINERS.." obsolete.

	// HEADROCK HAM 3.5: Only facilities allow militia training, and determine how many trainers can work here.
	// Does sector have at least one facility that allows training?
	UINT8 ubFacilityTrainersAllowed = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Increase tally
			ubFacilityTrainersAllowed += gFacilityTypes[cnt].ubMobileMilitiaTrainersAllowed;
		}
	}

	// Count number of trainers already operating here
	if ( CountMilitiaTrainersInSoldiersSector( pSoldier, MOBILE_MILITIA ) >= ubFacilityTrainersAllowed )
	{
		// Too many trainers in sector.
		return (FALSE);
	}

	// This will be replaced with an appropriate check to see if we have militia presence in this city at all.
	// For now, Mobile Militia training is allowed regardless of how many troopers you have here.
	//
	// Is town full of Elites?
	//if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, ELITE_MILITIA ))
	//{
	//	// Town is full of Elites. No further training required.
	//	// Also note that this takes care of Regulars as well, if Elite training is disabled.
	//	return( FALSE );
	//}


	if ( 100 <= GetMobileMilitiaQuota( FALSE ) )
		return ( FALSE );

	// If we've reached this, then all is well.
	return( TRUE );

}

BOOLEAN CanCharacterTrainMilitiaWithErrorReport( SOLDIERTYPE *pSoldier )
{
	// Temp string.
	CHAR16 sString[ 128 ];
	CHAR16 sStringA[ 128 ];

	// Enemies present?
	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		// Report "Enemies present!"
		DoScreenIndependantMessageBox( New113HAMMessage[5], MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}

	if ( 100 <= GetMobileMilitiaQuota( TRUE ) )
		return ( FALSE );


	///////////////////////////////
	// Test for required Leadership

	UINT16 usEffectiveLeadership = pSoldier->stats.bLeadership; // Basic leadership score
	BOOLEAN fSufficientLeadership = TRUE; // Result of check

	// Apply modifier for TEACHER trait, if that feature is activated
	if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits
		{
			if (HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ))
			{
				// bonus from Teaching trait
				usEffectiveLeadership = (usEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 );
			}
		}
		else
		{
			// Modifier applied once for each TEACHING level.
			for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pSoldier, TEACHING_OT ); i++ )
			{
				// This is a percentage modifier.
				usEffectiveLeadership = (usEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100;
			}
		}
		usEffectiveLeadership = __min(100,usEffectiveLeadership);
	}
	
	// Is there an INI-set requirement?
	if (gGameExternalOptions.ubMinimumLeadershipToTrainMilitia)
	{
		// Does he fail the requirement?
		if (usEffectiveLeadership < gGameExternalOptions.ubMinimumLeadershipToTrainMilitia)
		{
			fSufficientLeadership = FALSE;
		}
	}
	// If there is no requirement, does the soldier have ANY leadership skill?
	else if (usEffectiveLeadership <= 0)
	{
		fSufficientLeadership = FALSE;
	}

	// Failed above leadership tests?
	if( !fSufficientLeadership )
	{
		// Report "Insufficient Leadership Skill"
		swprintf(sString, New113HAMMessage[6], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return ( FALSE );
	}

	////////////////////////
	// Test for town loyalty

	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

	// Is this a town sector?
	if( bTownId != BLANK_SECTOR )
	{
		// is the current loyalty high enough to train militia at all?
		if( DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) == FALSE )
		{
			// Report "Not enough loyalty!"
			DoScreenIndependantMessageBox( zMarksMapScreenText[ 20 ], MSG_BOX_FLAG_OK, NULL );
			return (FALSE);
		}
	}

	// HEADROCK HAM 3.6: To be moved into the Mobile Militia training routine....
	// Kaiden: Roaming Militia Training:
	//if(IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, ELITE_MILITIA ))
	//	if (!gGameExternalOptions.gfmusttrainroaming)
	//		fCanTrainMilitia = FALSE;
	//	else if (GetWorldDay( ) < gGameExternalOptions.guiAllowMilitiaGroupsDelay)
	//		fCanTrainMilitia = FALSE;
	//	else if (IsThisSectorASAMSector(pSoldier->sSectorX,pSoldier->sSectorY,0 ))
	//		fCanTrainMilitia = FALSE;
	//	else
	//		fCanTrainMilitia = TRUE;

	////////////////////////////////
	// Test for Militia Capacity

	if(IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, ELITE_MILITIA ))
	{
		if( bTownId == BLANK_SECTOR )
		{
			// SAM site
			GetShortSectorString(	pSoldier->sSectorX, pSoldier->sSectorY, sStringA );
			swprintf( sString, zMarksMapScreenText[ 21 ], sStringA );
		}
		else
		{
			// town
			swprintf( sString, zMarksMapScreenText[ 21 ], pTownNames[ bTownId ] );
		}
		
		// Report "Not enough room for Militia!"
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}

	//////////////////////////////////////////////
	// HEADROCK HAM 3.5: Militia Training Facility 
	//
	// Militia training is enabled in the sector only if there is a facility that allows this here. 
	// If one or more facilities are found, positive values are summed up and presented as the number 
	// of trainers allowed in the sector. Values are read from XML, and can be set to mimic JA2
	// defaults. This renders the INI setting "MAX_MILITIA_TRAINERS.." obsolete.

	UINT8 ubFacilityTrainersAllowed = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Increase tally
			ubFacilityTrainersAllowed += gFacilityTypes[cnt].ubMilitiaTrainersAllowed;
		}
	}

	// If we are here, then TrainersAllowed > 0. 
	// Otherwise we'd have failed the BasicCanTrain check
	if ( CountMilitiaTrainersInSoldiersSector( pSoldier, TOWN_MILITIA ) >= ubFacilityTrainersAllowed )
	{
		swprintf( sString, gzLateLocalizedString[ 47 ], ubFacilityTrainersAllowed );
		
		// Report "Too many Militia Trainers!"
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}

	// No errors to report. Character can perform this assignment.
	return (TRUE);

}


BOOLEAN CanCharacterTrainMobileMilitiaWithErrorReport( SOLDIERTYPE *pSoldier )
{
	// Temp string.
	CHAR16 sString[ 256 ];
	INT32 iCounter = 0;

	// Enemies present?
	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		// Report "Enemies present!"
		DoScreenIndependantMessageBox( New113HAMMessage[5], MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}

	if ( 100 <= GetMobileMilitiaQuota( TRUE ) )
		return ( FALSE );


	///////////////////////////////
	// Test for required Leadership

	UINT16 usEffectiveLeadership = pSoldier->stats.bLeadership; // Basic leadership score
	BOOLEAN fSufficientLeadership = TRUE; // Result of check

	// Apply modifier for TEACHER trait, if that feature is activated
	if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
	{
		if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits
		{
			if (HAS_SKILL_TRAIT( pSoldier, TEACHING_NT ))
			{
				// bonus from Teaching trait
				usEffectiveLeadership = (usEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 );
			}
		}
		else
		{
			// Modifier applied once for each TEACHING level.
			for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pSoldier, TEACHING_OT ); i++ )
			{
				// This is a percentage modifier.
				usEffectiveLeadership = (usEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100;
			}
		}
		usEffectiveLeadership = __min(100,usEffectiveLeadership);
	}
	
	// Is there an INI-set requirement?
	if (gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia)
	{
		// Does he fail the requirement?
		if (usEffectiveLeadership < gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia)
		{
			fSufficientLeadership = FALSE;
		}
	}
	// If there is no requirement, does the soldier have ANY leadership skill?
	else if (usEffectiveLeadership <= 0)
	{
		fSufficientLeadership = FALSE;
	}

	// Failed above leadership tests?
	if( !fSufficientLeadership )
	{
		// Report "Insufficient Leadership Skill"
		swprintf(sString, New113HAMMessage[6], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return ( FALSE );
	}

	/////////////////////////////////////////
	// Town loyalty test
	// is the current loyalty high enough to train militia at all?
	if( DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) == FALSE )
	{
		// Report "Not enough loyalty!"
		DoScreenIndependantMessageBox( zMarksMapScreenText[ 20 ], MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}

	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) ] );

	////////////////////////////////////////////////
	// Check whether controlled town sectors already have full militia

	// HEADROCK HAM 4: This check is no longer necessary. We can manually restrict mobiles from moving into a city
	// in-game, so you can train mobiles straight away if you need them.
	/*

	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) ] );
	UINT8 ubUnfullSectorsFound = 0;

	if ( CountMilitia( pSectorInfo ) < gGameExternalOptions.iMaxMilitiaPerSector )
	{
		ubUnfullSectorsFound++;
	}
	else
	{
		iCounter = 0;
		while( pTownNamesList[ iCounter ] != 0 )
		{
			// Are we in this city?
			if( pTownNamesList[ iCounter] == bTownId )
			{
				INT16 sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
				INT16 sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

				pSectorInfo = &( SectorInfo[ SECTOR(sCurrentX, sCurrentY) ] );
				// if sector has enemies or hasn't already been taken at least once, then
				if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
					NumEnemiesInSector( sCurrentX, sCurrentY ) > 0 )
				{
					// skip the rest. This sector cannot generate militia anyway. 
					iCounter++;
					continue;
				}
				else
				{
					if (CountMilitia(pSectorInfo) < gGameExternalOptions.iMaxMilitiaPerSector )
					{
						// Found a controlled city sector that does not yet have a full garrison
						ubUnfullSectorsFound++;
					}
				}
			}
			iCounter++;
		}
	}
	if (ubUnfullSectorsFound)
	{
		// At least one city sector is controlled but not full of garrison militia. Can't train mobiles!
		swprintf(sString, New113HAMMessage[9], gGameExternalOptions.iMaxMilitiaPerSector, pTownNames[bTownId]);
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}

	*/

	//////////////////////////////////////////
	// Capacity and Garrison checks in nearby sectors
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	BOOLEAN fFoundValidSector = FALSE;

	GenerateDirectionInfosForTraining( pSoldier->sSectorX, pSoldier->sSectorY, &uiDirNumber, pMoveDir );
	// Found at least one suitable place to put Mobiles?
	if (uiDirNumber)
	{
		fFoundValidSector = TRUE;
	}
	// Try entire city. Only the HAM Smart Militia Generator can handle this.
	else
	{
		iCounter = 0;
		// Go through each city in the game
		while( pTownNamesList[ iCounter ] != 0 )
		{
			// Are we in this city?
			if( pTownNamesList[ iCounter] == bTownId )
			{
				INT16 sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
				INT16 sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

				// if sector has enemies or hasn't already been taken at least once, then
				if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
					NumEnemiesInSector( sCurrentX, sCurrentY ) > 0 )
				{
					// skip the rest. This sector cannot generate militia anyway. 
					iCounter++;
					continue;
				}

				// Find out if any adjacent sectors have room in them.
				GenerateDirectionInfosForTraining( sCurrentX, sCurrentY, &uiDirNumber, pMoveDir );

				if(uiDirNumber)
				{
					fFoundValidSector = TRUE;
					break;
				}
			}
			iCounter++;
		}
	}

	// Couldn't find at least one sector to place Mobiles. Report "No room!"
	if (!fFoundValidSector) 
	{
		swprintf(sString, New113HAMMessage[8], pTownNames[ bTownId ]);
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}



	//////////////////////////////////////////////
	// HEADROCK HAM 3.5: Militia Training Facility 
	//
	// Militia training is enabled in the sector only if there is a facility that allows this here. 
	// If one or more facilities are found, positive values are summed up and presented as the number 
	// of trainers allowed in the sector. Values are read from XML, and can be set to mimic JA2
	// defaults. This renders the INI setting "MAX_MILITIA_TRAINERS.." obsolete.

	UINT8 ubFacilityTrainersAllowed = 0;
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		// Is this facility here?
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			// Increase tally
			ubFacilityTrainersAllowed += gFacilityTypes[cnt].ubMobileMilitiaTrainersAllowed;
		}
	}
	// If we are here, then TrainersAllowed > 0. 
	// Otherwise we'd have failed the BasicCanTrain check
	if ( CountMilitiaTrainersInSoldiersSector( pSoldier, MOBILE_MILITIA ) >= ubFacilityTrainersAllowed )
	{
		swprintf( sString, New113HAMMessage[ 7 ], ubFacilityTrainersAllowed );
		// Report "Too many Militia Trainers!"
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}

	// No errors to report. Character can perform this assignment.
	return (TRUE);

}
// HEADROCK HAM 3.6: This function determines whether a character can use facilities at all, or whether the sector has
// any "useable" facilities at all.
BOOLEAN BasicCanCharacterFacility( SOLDIERTYPE *pSoldier )
{
	/////////////////////////////////////////////////////
	// Tests whether character can do assignments at all!

	AssertNotNIL(pSoldier);

	if ( !BasicCanCharacterAssignment( pSoldier, TRUE ) )
	{
		return( FALSE );
	}

	// Is character dead or unconscious?
	if( pSoldier->stats.bLife < OKLIFE )
	{
		// dead or unconscious...
		return ( FALSE );
	}

	// Is character underground?
	if( pSoldier->bSectorZ != 0 )
	{
		// underground training is not allowed (code doesn't support and it's a reasonable enough limitation)
		return( FALSE );
	}

	// Is character on the way into/out of Arulco?
	if( IsCharacterInTransit( pSoldier ) == TRUE )
	{
		return ( FALSE );
	}

	// Is character travelling between sectors?
	if( CharacterIsBetweenSectors( pSoldier ) )
	{
		return( FALSE );
	}

	// Is character an Escortee?
	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )
	{
		// epcs can't do this
		return( FALSE );
	}

	// Is character a Vehicle or Robot?
	if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}

	// IS character inside a helicopter over a hostile sector?
	if( pSoldier->bAssignment == VEHICLE )
	{
		if( ( iHelicopterVehicleId != -1 ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			// enemies in sector
			if( NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY ) > 0 )
			{
				return( FALSE );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Tests to see whether this sector contains any facilities that could be used at all.

	UINT8 ubSector = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
	BOOLEAN fFoundUseableFacility = FALSE;
	
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		if (gFacilityLocations[ubSector][cnt].fFacilityHere)
		{
			if (gFacilityTypes[cnt].ubTotalStaffLimit)
			{
				fFoundUseableFacility = TRUE;
			}
		}
	}
	if (!fFoundUseableFacility)
	{
		// No useable facilities in this sector.
		return FALSE;
	}

	return ( TRUE );
}

BOOLEAN DisplayFacilityMenu( SOLDIERTYPE *pSoldier )
{
	BOOLEAN fFacilityPresent=FALSE;
	INT32 iCounter=0;
	INT32 hStringHandle=0;

	// first, clear pop up box
	RemoveBox(ghFacilityBox);
	ghFacilityBox = -1;

	CreateFacilityBox();
	SetCurrentBox(ghFacilityBox);

	// run through list of staff/use facilities in sector and add them to pop up box
	for ( iCounter = 0; iCounter < MAX_NUM_FACILITY_TYPES; iCounter++ )
	{
		if ( gFacilityLocations[ SECTOR(pSoldier->sSectorX,pSoldier->sSectorY) ][iCounter].fFacilityHere )
		{
			if ( gFacilityTypes[ iCounter ].ubTotalStaffLimit )
			{
				AddMonoString((UINT32 *)&hStringHandle, gFacilityTypes[ iCounter ].szFacilityName);
				fFacilityPresent = TRUE;
			}
		}
	}

	if (!fFacilityPresent)
	{
		return FALSE;
	}

	// cancel string (borrow the one in the squad menu)
	AddMonoString((UINT32 *)&hStringHandle, pSquadMenuStrings[ SQUAD_MENU_CANCEL ]);

	SetBoxFont(ghFacilityBox, MAP_SCREEN_FONT);
	SetBoxHighLight(ghFacilityBox, FONT_WHITE);
	SetBoxShade(ghFacilityBox, FONT_GRAY7);
	SetBoxForeground(ghFacilityBox, FONT_LTGREEN);
	SetBoxBackground(ghFacilityBox, FONT_BLACK);

	ResizeBoxToText( ghFacilityBox );

	CheckAndUpdateTacticalAssignmentPopUpPositions( );

	return TRUE;
}

BOOLEAN DisplayFacilityAssignmentMenu( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType )
{

	if (!gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][ubFacilityType].fFacilityHere)
	{	
		// Facility isn't here? Odd.
		return (FALSE);
	}

	INT32 iCounter=0;
	INT32 iCounterB = 0;
	INT32 hStringHandle=0;
	CHAR16 sTempString[128];
	BOOLEAN fFoundVehicle;

	// first, clear pop up box
	RemoveBox(ghFacilityAssignmentBox);
	ghFacilityAssignmentBox = -1;

	CreateFacilityAssignmentBox();
	SetCurrentBox(ghFacilityAssignmentBox);

	BOOLEAN fAssignmentsFound = FALSE;
	// Create a list of possible assignments at this facility
	for ( iCounter = 0; iCounter < NUM_FACILITY_ASSIGNMENTS; iCounter++ )
	{
		if ( gFacilityTypes[ ubFacilityType ].AssignmentData[iCounter].ubStaffLimit )
		{
			if ( iCounter == FAC_REPAIR_VEHICLE )
			{
				// Test to see whether there are any.
				for ( iCounterB = 0; iCounterB < ubNumberOfVehicles; iCounterB++ )
				{
					if ( pVehicleList[iCounterB].fValid == TRUE )
					{
						if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounterB ) )
						{
							// Create line that says "Repair X" where X is the vehicle.
						//	swprintf( sTempString, gzFacilityAssignmentStrings[ FAC_REPAIR_VEHICLE ], pVehicleStrings[ pVehicleList[ iCounterB ].ubVehicleType ]);
							swprintf( sTempString, gzFacilityAssignmentStrings[ FAC_REPAIR_VEHICLE ], gNewVehicle[ pVehicleList[ iCounterB ].ubVehicleType ].NewVehicleStrings);
							AddMonoString((UINT32 *)&hStringHandle, sTempString );
							fFoundVehicle = TRUE;
						}
					}
				}
				if (fFoundVehicle == FALSE)
				{
					// Create line that says "Repair Vehicle", and will be shaded.
					swprintf( sTempString, gzFacilityAssignmentStrings[ FAC_REPAIR_VEHICLE ], L"Vehicle" );
					AddMonoString((UINT32 *)&hStringHandle, sTempString );
				}
			}
			else if ( iCounter == FAC_REPAIR_ROBOT )
			{
				// is the ROBOT here?
				if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
				{
					// robot
					AddMonoString((UINT32 *)&hStringHandle, gzFacilityAssignmentStrings[ FAC_REPAIR_ROBOT ] );
				}
			}
			else if ( iCounter == FAC_PRISON_SNITCH || iCounter == FAC_SPREAD_PROPAGANDA || iCounter == FAC_SPREAD_PROPAGANDA_GLOBAL || iCounter == FAC_GATHER_RUMOURS )
			{
				// anv: is character a snitch?
				if( CanCharacterSnitch( pSoldier ) )
				{
					// yeah, show snitch line then
					AddMonoString((UINT32 *)&hStringHandle, gzFacilityAssignmentStrings[ iCounter ] );
				}
			}
			else
			{
				AddMonoString((UINT32 *)&hStringHandle, gzFacilityAssignmentStrings[ iCounter ]);
			}
			fAssignmentsFound = TRUE;
		}
	}

	if (!fAssignmentsFound)
	{
		return FALSE;
	}

	// cancel string (borrow the one in the squad menu)
	AddMonoString((UINT32 *)&hStringHandle, pSquadMenuStrings[ SQUAD_MENU_CANCEL ]);

	SetBoxFont(ghFacilityAssignmentBox, MAP_SCREEN_FONT);
	SetBoxHighLight(ghFacilityAssignmentBox, FONT_WHITE);
	SetBoxShade(ghFacilityAssignmentBox, FONT_GRAY7);
	SetBoxForeground(ghFacilityAssignmentBox, FONT_LTGREEN);
	SetBoxBackground(ghFacilityAssignmentBox, FONT_BLACK);

	ResizeBoxToText( ghFacilityAssignmentBox );

	CheckAndUpdateTacticalAssignmentPopUpPositions( );

	// Set the current facility whose assignments are being shown
	gubFacilityInSubmenu = ubFacilityType;

	return TRUE;
}

// HEADROCK HAM 3.6: Create the facility menu box.
void CreateFacilityBox()
{
	FacilityPosition.iX = OrigFacilityPosition.iX;

	if( giBoxY != 0 )
	{
		FacilityPosition.iY = giBoxY + ( ASSIGN_MENU_FACILITY * GetFontHeight( MAP_SCREEN_FONT ) );
	}

	CreatePopUpBox(&ghFacilityBox, FacilityDimensions, FacilityPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
	SetBoxBuffer(ghFacilityBox, FRAME_BUFFER);
	SetBorderType(ghFacilityBox,guiPOPUPBORDERS);
	SetBackGroundSurface(ghFacilityBox, guiPOPUPTEX);
	SetMargins( ghFacilityBox, 6, 6, 4, 4 );
	SetLineSpace(ghFacilityBox, 2);

	// resize box to text
	ResizeBoxToText( ghFacilityBox );

	DetermineBoxPositions( );
}

void CreateDestroyMouseRegionForFacilityMenu( void )
{
	static BOOLEAN fCreated = FALSE;

	UINT32 uiMenuLine = 0;
	INT32 iFacilityType = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition, pPoint;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;


	if( fShowFacilityMenu )
	{
		GetBoxPosition( ghAssignmentBox, &pPoint);

		// get dimensions..mostly for width
		GetBoxSize( ghAssignmentBox, &pDimensions );

		// vehicle position
		FacilityPosition.iX = pPoint.iX + pDimensions.iRight;

		SetBoxPosition( ghFacilityBox , FacilityPosition );
	}


	if( ( fShowFacilityMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghFacilityBox ) + GetFontHeight( GetBoxFont( ghFacilityBox ) );

		// get x.y position of box
		GetBoxPosition( ghFacilityBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghFacilityBox, &pDimensions );
		SetBoxSecondaryShade( ghFacilityBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghFacilityBox );

		pSoldier = GetSelectedAssignSoldier( FALSE );

		// define regions
		//for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghFacilityBox ); iCounter++ )
		// run through list of staff/use facilities in sector and add them to pop up box
		for ( UINT32 iCounter = 0; iCounter < MAX_NUM_FACILITY_TYPES; iCounter++ )
		{
			if ( gFacilityLocations[ SECTOR(pSoldier->sSectorX,pSoldier->sSectorY) ][iCounter].fFacilityHere )
			{
				if ( gFacilityTypes[ iCounter ].ubTotalStaffLimit )
				{
					// add mouse region for each facility
					MSYS_DefineRegion( &gFacilityMenuRegion[ uiMenuLine ],	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
								MSYS_NO_CURSOR, FacilityMenuMvtCallback, FacilityMenuBtnCallback );
		
					MSYS_SetRegionUserData( &gFacilityMenuRegion[ uiMenuLine ], 0, uiMenuLine );
					// store facility ID in the SECOND user data
		
					MSYS_SetRegionUserData( &gFacilityMenuRegion[ uiMenuLine ], 1, iCounter );
		
					uiMenuLine++;
				}
			}
		}

		// cancel line
		MSYS_DefineRegion( &gFacilityMenuRegion[ uiMenuLine ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, FacilityMenuMvtCallback, FacilityMenuBtnCallback );
		MSYS_SetRegionUserData( &gFacilityMenuRegion[ uiMenuLine ], 0, uiMenuLine );
		MSYS_SetRegionUserData( &gFacilityMenuRegion[ uiMenuLine ], 1, MAX_NUM_FACILITY_TYPES );

		// created
		fCreated = TRUE;

		// pause game
		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghFacilityBox );

		fCreated = TRUE;

		//HandleShadingOfLinesForFacilityMenu( );
	}
	else if( ( ( fShowFacilityMenu == FALSE ) || ( fShowAssignmentMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;

		// remove these regions
		for( uiMenuLine = 0; uiMenuLine < GetNumberOfLinesOfTextInBox( ghFacilityBox ); uiMenuLine++ )
		{
			MSYS_RemoveRegion( &gFacilityMenuRegion[ uiMenuLine ] );
		}

		fShowFacilityMenu = FALSE;

		SetRenderFlags( RENDER_FLAG_FULL );

		HideBox( ghFacilityBox );

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}

	return;
}

void FacilityMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( HandleAssignmentExpansionAndHighLightForFacilityMenu( ) == TRUE )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( iValue < MAX_NUM_FACILITY_TYPES )
		{
			// no shaded(disabled) lines actually appear in vehicle menus
			if( GetBoxShadeFlag( ghFacilityBox, iValue ) == FALSE )
			{
				// highlight vehicle line
				HighLightBoxLine( ghFacilityBox, iValue );
			}
		}
		else
		{
			// highlight cancel line
			HighLightBoxLine( ghFacilityBox, GetNumberOfLinesOfTextInBox( ghFacilityBox ) - 1 );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghFacilityBox );

		//HandleShadingOfLinesForVehicleMenu( );
	}
}

void FacilityMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	BOOLEAN fCanOperateFacility = TRUE;

	INT32 iValue = MSYS_GetRegionUserData( pRegion, 1 );
	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN ) )
	{
		if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && !fShowMapInventoryPool )
		{
			UnMarkButtonDirty( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{

		if( fShowFacilityAssignmentMenu )
		{
			// cancel Facility submenu
			fShowFacilityAssignmentMenu = FALSE;
			// Reset facility number
			gubFacilityInSubmenu = 0;
			gubFacilityLineForSubmenu = 0;
			// rerender tactical stuff
			gfRenderPBInterface = TRUE;

			return;
		}

		if (iValue > 0 && iValue < MAX_NUM_FACILITY_TYPES)
		{
			// The line we clicked will open a new menu adjacent to this one. This menu lists all possible assignments
			// that can be performed at this facility.
			gubFacilityInSubmenu = (INT8)iValue;
			gubFacilityLineForSubmenu = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

			fShowFacilityAssignmentMenu = TRUE;
			DetermineBoxPositions();

			DisplayFacilityAssignmentMenu( pSoldier, iValue );


			// For now, only tests whether the character can use facilities at all, or whether there ARE facilities
			// to be used at the moment.
			if( !BasicCanCharacterFacility(pSoldier) )
			{
				// No feedback. The menu options should be greyed out, anyway.
				return;
			}
		}

					





			/*// Check for standing debt for Facility Operation. May trigger an on-screen prompt or error message.
			if ( gFacilityTypes[iValue].AssignmentData[FAC_STAFF].sCostPerHour && // Facility costs money to operate
				!IsOutstandingFacilityDebtWithErrorReport() ) // There's an outstanding debt for facilities
			{
				// Facility debt needs to be paid first.
				return;
			}

			// PASSED BOTH TESTS - ALLOW SOLDIER TO TRAIN MILITIA HERE

			pSoldier->bOldAssignment = pSoldier->bAssignment;

			if( ( pSoldier->bAssignment != FACILITY_STAFF ) )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

			// stop showing menu
			fShowAssignmentMenu = FALSE;
			giAssignHighLine = -1;

			// remove from squad

			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}
			RemoveCharacterFromSquads(	pSoldier );

			ChangeSoldiersAssignment( pSoldier, FACILITY_STAFF );

			pSoldier->sFacilityTypeOperated = iValue;

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );
			if( gFacilityTypes[ iValue ].AssignmentData[FAC_STAFF].sCostPerHour )
			{
				// Ask player if he wishes to expend this money.
				HandleInterfaceMessageForCostOfOperatingFacility( pSoldier, FAC_STAFF );
			}
			else
			{
				SetAssignmentForList( FACILITY_STAFF, iValue );
			}
			gfRenderPBInterface = TRUE;*/

		else
		{
			// stop showing menu
			fShowFacilityMenu = FALSE;

			// unhighlight the assignment box
			UnHighLightBox( ghAssignmentBox );

			// reset list
			ResetSelectedListForMapScreen( );
			gfRenderPBInterface = TRUE;
		}

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if( fShowFacilityAssignmentMenu )
		{
			// cancel attribute submenu
			fShowFacilityAssignmentMenu = FALSE;
			// rerender tactical stuff
			gfRenderPBInterface = TRUE;

			return;
		}
	}
}

// Check whether a character can staff a specific type of facility in this sector.
BOOLEAN CanCharacterFacility( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType )
{
	AssertNotNIL(pSoldier);
	UINT8 ubSectorID = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
	BOOLEAN fFoundVehicleToRepair = FALSE;

	// Make sure the basic sector/merc variables are still applicable. This is simply a fail-safe.
	if( !BasicCanCharacterFacility( pSoldier ) )
	{
		// Soldier/Sector have somehow failed the basic test. Character automatically fails this test as well.
		return( FALSE );
	}

	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		return( FALSE );
	}

	// Facility exists in this sector at all? (Failsafe)
	if (!gFacilityLocations[ubSectorID][ubFacilityType].fFacilityHere)
	{
		// No such facility here! Odd.
		return( FALSE );
	}

	//////////////////////////////////////////
	// Does character have sufficient skill?
	if (pSoldier->stats.bStrength < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumStrength ||
		pSoldier->stats.bLife < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumHealth ||
		pSoldier->stats.bWisdom < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumWisdom ||
		pSoldier->stats.bAgility < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumAgility ||
		pSoldier->stats.bDexterity < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumDexterity ||
		pSoldier->stats.bMarksmanship < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMarksmanship ||
		pSoldier->stats.bMechanical < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMechanical ||
		pSoldier->stats.bMedical < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMedical ||
		pSoldier->stats.bLeadership < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLeadership ||
		pSoldier->stats.bExplosive < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumExplosives ||
		pSoldier->stats.bExpLevel < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLevel ||

		pSoldier->aiData.bMorale < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMorale ||
		pSoldier->bBreathMax < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumBreath
		)
	{
		// Character is lacking a stat required for this specific assignment.
		return ( FALSE );
	}

	////////////////////////////////////////
	// Check town loyalty

	UINT8 ubTownID = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	if (ubTownID != BLANK_SECTOR)
	{
		if (gTownLoyalty[ ubTownID ].ubRating < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLoyaltyHere )
		{
			// Insufficient loyalty.
			return (FALSE);
		}
	}	

	//////////////////////////////////////////////
	// Check capacity for mercs working at this particular facility.

	// Count the number of open slots left for people trying to perform this assignment in the same facility, in the
	// soldier's sector.
	// Note that we can reach this when the soldier is ALREADY one of the people working at this facility, so in that
	// case count him out.
	INT8 bX = 0;
	INT8 bY = 0;
	if ( ubFacilityType == (UINT8)pSoldier->sFacilityTypeOperated )
		bX = -1;
	if ( ubAssignmentType == GetSoldierFacilityAssignmentIndex( pSoldier ) )
		bY = -1;

	if ( CountFreeFacilitySlots( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, ubFacilityType) <= bX || // Too many people in the facility, or
		CountFreeFacilityAssignmentSlots( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, ubFacilityType, ubAssignmentType ) <= bY ) // Too many people doing this assignment
	{
		// No free slots.
		return (FALSE);
	}

	////////////////////////////////////////////////////
	// Check for extra requirements for specific assignments.

	// DOCTOR
	if (ubAssignmentType == FAC_DOCTOR)
	{
		BOOLEAN fFoundMedKit = FALSE;
		INT8 bPocket = 0;

		// find med kit
		// CHRISL: Changed to dynamically determine max inventory locations.
		for (bPocket = HANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
		{
			// doctoring is allowed using either type of med kit (but first aid kit halves doctoring effectiveness)
			if( IsMedicalKitItem( &( pSoldier -> inv[ bPocket ] ) ) )
			{
				fFoundMedKit = TRUE;
				break;
			}
		}

		if( fFoundMedKit == FALSE )
		{
			return( FALSE );
		}
	}

	// REPAIR ITEMS
	if ( ubAssignmentType == FAC_REPAIR_ITEMS ||
		ubAssignmentType == FAC_REPAIR_VEHICLE ||
		ubAssignmentType == FAC_REPAIR_ROBOT )
	{
		// make sure he has a toolkit
		if ( FindToolkit( pSoldier ) == NO_SLOT )
		{
			return( FALSE );
		}

		switch (ubAssignmentType)
		{
			case FAC_REPAIR_ITEMS:
				// items?
				if ( !DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS ) )
				{
					return( FALSE );
				}
				break;
			case FAC_REPAIR_VEHICLE:
				for ( INT32 iCounter = 0; iCounter < ubNumberOfVehicles; iCounter++ )
				{
					if ( pVehicleList[ iCounter ].fValid == TRUE )
					{
						// the helicopter, is NEVER repairable...
						if ( iCounter != iHelicopterVehicleId )
						{
							if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounter ) )
							{
								if( CanCharacterRepairVehicle( pSoldier, iCounter ) == TRUE )
								{
									// there is a repairable vehicle here
									fFoundVehicleToRepair = TRUE;
								}
							}
						}
					}
				}
				if (!fFoundVehicleToRepair)
				{
					return (FALSE);
				}
				break;
			case FAC_REPAIR_ROBOT:
				SOLDIERTYPE *pRobot = NULL;

				// do we in fact have the robot on the team?
				pRobot = GetRobotSoldier( );
				if( pRobot == NULL )
				{
					return( FALSE );
				}

				// if robot isn't damaged at all
				if( pRobot->stats.bLife == pRobot->stats.bLifeMax )
				{
					return( FALSE );
				}

				// is the robot in the same sector
				if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == FALSE )
				{
					return( FALSE );
				}
				break;
		}
	}
	if ( ubAssignmentType == FAC_PRISON_SNITCH )
	{
		if( !CanCharacterSnitchInPrison(pSoldier) )
		{
			return( FALSE );
		}
	}
	if ( ubAssignmentType == FAC_SPREAD_PROPAGANDA || ubAssignmentType == FAC_SPREAD_PROPAGANDA_GLOBAL )
	{
		if( !CanCharacterSpreadPropaganda(pSoldier) )
		{
			return( FALSE );
		}
	}
	if ( ubAssignmentType == FAC_GATHER_RUMOURS )
	{
		if( !CanCharacterGatherInformation(pSoldier) )
		{
			return( FALSE );
		}
	}

	/*if (  ubAssignmentType == FAC_STRATEGIC_MILITIA_MOVEMENT )
	{
		if( !CanCharacterGatherInformation(pSoldier) )
		{
			return( FALSE );
		}
	}*/

	// If we've reached this, then all is well.
	return( TRUE );
}

INT8 CountFreeFacilitySlots( UINT8 sMapX, UINT8 sMapY, UINT8 ubFacilityType )
{
	INT16 sFreeSlotsFound = 0;

	UINT8 ubStaffLimit = gFacilityTypes[ubFacilityType].ubTotalStaffLimit;
	UINT8 ubStaffFoundHere = 0;

	if (!gFacilityLocations[SECTOR(sMapX,sMapY)][ubFacilityType].fFacilityHere)
	{
		// The facility is not present!
		return (0);
	}
	
	if (!ubStaffLimit)
	{
		// No people are allowed to work at this facility at all!
		return (0);
	}
	else
	{
		UINT8 ubCounter = 0;
		SOLDIERTYPE *pSoldier;
		// Count number of people doing anything at this facility.
		while(gCharactersList[ubCounter].fValid)
		{
			pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

			// Is character operating this facility?
			if( (UINT8)pSoldier->sFacilityTypeOperated == ubFacilityType &&
				pSoldier->sSectorX == sMapX &&  // Is he in the same sector?
				pSoldier->sSectorY == sMapY )
			{
				// Increase tally.
				ubStaffFoundHere++;
			}
			ubCounter++;
		}
		sFreeSlotsFound = ubStaffLimit - ubStaffFoundHere;
		return ((INT8)sFreeSlotsFound);
	}
}

INT8 CountFreeFacilityAssignmentSlots( UINT8 sMapX, UINT8 sMapY, UINT8 ubFacilityType, UINT8 ubAssignmentIndex )
{
	INT16 sFreeSlotsFound = 0;

	UINT8 ubStaffLimit = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentIndex].ubStaffLimit;
	UINT8 ubStaffFoundHere = 0;

	if (!gFacilityLocations[SECTOR(sMapX,sMapY)][ubFacilityType].fFacilityHere)
	{
		// The facility is not present!
		return (0);
	}

	if (!ubStaffLimit)
	{
		// No people are allowed to perform this assignment at this facility at all!
		return (0);
	}
	else
	{
		UINT8 ubCounter = 0;
		SOLDIERTYPE *pSoldier;

		// Count number of people doing this assignment at this facility.
		while(gCharactersList[ubCounter].fValid)
		{
			pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

			// Is character operating this facility?
			if( (UINT8)pSoldier->sFacilityTypeOperated == ubFacilityType &&
				pSoldier->sSectorX == sMapX &&  // Is he in the same sector?
				pSoldier->sSectorY == sMapY )
			{
				// Is he performing the same exact assignment we're looking to do?
				if (GetSoldierFacilityAssignmentIndex( pSoldier ) == ubAssignmentIndex)
				{
					// Increase tally.
					ubStaffFoundHere++;
				}
			}
			ubCounter++;
		}
		sFreeSlotsFound = ubStaffLimit - ubStaffFoundHere;
		return ((INT8)sFreeSlotsFound);
	}
}


// Check whether a character can staff a specific type of facility in this sector. Display a message informing player
// of specific obstacles.
BOOLEAN CanCharacterFacilityWithErrorReport( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType )
{
	CHAR16 sString[ 256 ];
	AssertNotNIL(pSoldier);
	UINT8 ubSectorID = SECTOR(pSoldier->sSectorX, pSoldier->sSectorY);
	BOOLEAN fFoundVehicleToRepair = FALSE;

	// Make sure the basic sector/merc variables are still applicable. This is simply a fail-safe.
	if( !BasicCanCharacterFacility( pSoldier ) )
	{
		// Soldier/Sector have somehow failed the basic test. Character automatically fails this test as well.
		return( FALSE );
	}

	if( NumEnemiesInAnySector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
	{
		DoScreenIndependantMessageBox( New113HAMMessage[10], MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}

	// Facility exists in this sector at all? (Failsafe)
	if (!gFacilityLocations[ubSectorID][ubFacilityType].fFacilityHere)
	{
		// No such facility here! Odd.
		return( FALSE );
	}

	//////////////////////////////////////////
	// Skill/Condition check

	if (pSoldier->stats.bStrength < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumStrength)
	{
		swprintf(sString, gzFacilityErrorMessage[0], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bDexterity < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumDexterity)
	{
		swprintf(sString, gzFacilityErrorMessage[1], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bAgility < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumAgility)
	{
		swprintf(sString, gzFacilityErrorMessage[2], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bLife < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumHealth)
	{
		swprintf(sString, gzFacilityErrorMessage[3], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bWisdom < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumWisdom)
	{
		swprintf(sString, gzFacilityErrorMessage[4], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bMarksmanship < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMarksmanship)
	{
		swprintf(sString, gzFacilityErrorMessage[5], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bMedical < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMedical)
	{
		swprintf(sString, gzFacilityErrorMessage[6], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bMechanical < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMechanical)
	{
		swprintf(sString, gzFacilityErrorMessage[7], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bLeadership < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLeadership)
	{
		swprintf(sString, gzFacilityErrorMessage[8], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bExplosive < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumExplosives)
	{
		swprintf(sString, gzFacilityErrorMessage[9], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->stats.bExpLevel < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLevel)
	{
		swprintf(sString, gzFacilityErrorMessage[10], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->aiData.bMorale < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumMorale)
	{
		swprintf(sString, gzFacilityErrorMessage[11], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	if (pSoldier->bBreathMax < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumBreath)
	{
		swprintf(sString, gzFacilityErrorMessage[12], pSoldier->GetName());
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return( FALSE );
	}
	
	////////////////////////////////////////
	// Check town loyalty

	UINT8 ubTownID = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
	if (ubTownID != BLANK_SECTOR)
	{
		if (gTownLoyalty[ ubTownID ].ubRating < gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].ubMinimumLoyaltyHere )
		{
			swprintf(sString, gzFacilityErrorMessage[13], pTownNames[ ubTownID ]);
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			// Insufficient loyalty.
			return (FALSE);
		}
	}	

	//////////////////////////////////////////////
	// Check capacity for mercs working at this particular facility.

	// Count the number of open slots left for people trying to perform this assignment in the same facility, in the
	// soldier's sector.
	// Note that we can reach this when the soldier is ALREADY one of the people working at this facility, so in that
	// case count him out.
	INT8 bX = 0;
	INT8 bY = 0;
	if ( ubFacilityType == (UINT8)pSoldier->sFacilityTypeOperated )
		bX = -1;
	if ( ubAssignmentType == GetSoldierFacilityAssignmentIndex( pSoldier ) )
		bY = -1;

	if ( CountFreeFacilitySlots( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, ubFacilityType) <= bX )
	{
		// Too many people working at this facility (overall)
		swprintf(sString, gzFacilityErrorMessage[14], gFacilityTypes[ubFacilityType].szFacilityName);
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
		return (FALSE);
	}
	else if (CountFreeFacilityAssignmentSlots( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, ubFacilityType, ubAssignmentType ) <= bY )
	{
		// Too many people performing this specific assignment at this facility.
		swprintf(sString, gzFacilityErrorMessage[15], gFacilityTypes[ubFacilityType].szFacilityName);
		DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );	
		return (FALSE);
	}

	////////////////////////////////////////////////////
	// Check for extra requirements for specific assignments.

	// DOCTOR
	if (ubAssignmentType == FAC_DOCTOR)
	{
		BOOLEAN fFoundMedKit = FALSE;
		INT8 bPocket = 0;

		// find med kit
		// CHRISL: Changed to dynamically determine max inventory locations.
		for (bPocket = HANDPOS; bPocket < NUM_INV_SLOTS; bPocket++)
		{
			// doctoring is allowed using either type of med kit (but first aid kit halves doctoring effectiveness)
			if( IsMedicalKitItem( &( pSoldier -> inv[ bPocket ] ) ) )
			{
				fFoundMedKit = TRUE;
				break;
			}
		}

		if( fFoundMedKit == FALSE )
		{
			swprintf( sString, zMarksMapScreenText[19], pSoldier->GetName() );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			return( FALSE );
		}
	}

	if ( ubAssignmentType == FAC_REPAIR_ITEMS ||
		ubAssignmentType == FAC_REPAIR_VEHICLE ||
		ubAssignmentType == FAC_REPAIR_ROBOT )
	{
		// make sure he has a toolkit
		if ( FindToolkit( pSoldier ) == NO_SLOT )
		{
			swprintf( sString, zMarksMapScreenText[18], pSoldier->GetName() );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			return( FALSE );
		}

		switch (ubAssignmentType)
		{
			case FAC_REPAIR_ITEMS:
				// items?
				if ( !DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS ) )
				{
					swprintf( sString, gzFacilityErrorMessage[16], pSoldier->GetName() );
					DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
					return( FALSE );
				}
				break;
			case FAC_REPAIR_VEHICLE:
				for ( INT32 iCounter = 0; iCounter < ubNumberOfVehicles; iCounter++ )
				{
					if ( pVehicleList[ iCounter ].fValid == TRUE )
					{
						// the helicopter, is NEVER repairable...
						if ( iCounter != iHelicopterVehicleId )
						{
							if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounter ) )
							{
								if( CanCharacterRepairVehicle( pSoldier, iCounter ) == TRUE )
								{
									// there is a repairable vehicle here
									fFoundVehicleToRepair = TRUE;
								}
							}
						}
					}
				}
				if (!fFoundVehicleToRepair)
				{
					// No message. Will be greyed out anyway.
					return (FALSE);
				}
				break;
			case FAC_REPAIR_ROBOT:
				SOLDIERTYPE *pRobot = NULL;

				// do we in fact have the robot on the team?
				pRobot = GetRobotSoldier( );
				if( pRobot == NULL )
				{
					return( FALSE );
				}

				// if robot isn't damaged at all
				if( pRobot->stats.bLife == pRobot->stats.bLifeMax )
				{
					return( FALSE );
				}

				// is the robot in the same sector
				if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) == FALSE )
				{
					return( FALSE );
				}
				break;
		}
	}

	if( ubAssignmentType == FAC_PRISON_SNITCH )
	{
		if( IsSoldierKnownAsMercInSector( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY ) )
		{
			//swprintf( sString, gzFacilityErrorMessage[32], pSoldier->GetName() );
			swprintf( sString, gzFacilityErrorMessage[33], pSoldier->GetName(), gMercProfiles[pSoldier->ubProfile].ubSnitchExposedCooldown );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			return( FALSE );
		} 
		else if( !CanCharacterSnitchInPrison(pSoldier) )
		{
			return( FALSE );
		} 

	}

	// If we've reached this, then all is well.
	return( TRUE );

}

void HandleShadingOfLinesForFacilityMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0;
	INT32 iNumLine = 0;

	if( ( fShowFacilityMenu == FALSE ) || ( ghFacilityBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );


	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForMoveItemMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForMoveItemMenu().

	// run through list of staff/use facilities in sector and add them to pop up box
	for ( iCounter = 0; iCounter < MAX_NUM_FACILITY_TYPES; iCounter++ )
	{
		if ( gFacilityLocations[ SECTOR(pSoldier->sSectorX,pSoldier->sSectorY) ][iCounter].fFacilityHere )
		{
			if ( gFacilityTypes[ iCounter ].ubTotalStaffLimit )
			{
				if (BasicCanCharacterFacility( pSoldier ))
				{
					UnShadeStringInBox( ghFacilityBox, iNumLine );
					UnSecondaryShadeStringInBox( ghFacilityBox, iNumLine );
					// No secondary shade. Facilities are either shaded or unshaded. Specific
					// assignments for this facility might be secondary-shaded though.
				}
				else
				{
					UnSecondaryShadeStringInBox( ghFacilityBox, iNumLine );
					ShadeStringInBox( ghFacilityBox, iNumLine );
				}
				iNumLine++;
			}
		}
	}
	return;
}

void HandleInterfaceMessageForCostOfOperatingFacility( SOLDIERTYPE *pSoldier, UINT8 ubAssignmentType )
{
	// If you hit this assertion, then the soldier was not told to operate any facility before this function
	// was called. Generally, it should happen RIGHT BEFORE calling this function!
	Assert (pSoldier->sFacilityTypeOperated != -1);

	UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
	INT32 iFacilityOperatingCost = gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentType].sCostPerHour;
	gpFacilityStaffer = pSoldier;

	CHAR16 sString[ 128 ];
	SGPRect pCenteringRect= {0, 0, 640, INV_INTERFACE_START_Y };

	swprintf( sString, New113HAMMessage[13], iFacilityOperatingCost );

	// if we are in mapscreen, make a pop up
	if( guiCurrentScreen == MAP_SCREEN )
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayFacilityCostsYesNoBoxCallback );
	}
	else
	{
		DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO, PayFacilityCostsYesNoBoxCallback, &pCenteringRect );
	}

	return;
}

// HEADROCK HAM 3.6: Callback on agreeing to continue facility staffing.
void PayFacilityCostsYesNoBoxCallback( UINT8 bExitValue )
{
	// yes
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		// reset staffer variable. We've already set his assignment so everything's good.
		gpFacilityStaffer = NULL;
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{
		StopTimeCompression();

		FacilityStaffingRejected();
	}

	return;
}

// HEADROCK HAM 3.6: Callback on agreeing to pay off facility debts before you can assign another character to facility work.
void PayFacilityDebtManuallyYesNoBoxCallback( UINT8 bExitValue )
{
	// This callback should only be called if the player can pay off the ENTIRE debt!!
	Assert( LaptopSaveInfo.iCurrentBalance >= giTotalOwedForFacilityOperationsToday );

	// yes
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		// Pay total debt.
		INT32 iToPay = giTotalOwedForFacilityOperationsToday;
		AddTransactionToPlayersBook( FACILITY_OPERATIONS, 0, GetWorldTotalMin(), -( iToPay ) );
		giTotalOwedForFacilityOperationsToday = 0;
		gfOutstandingFacilityDebt = FALSE;

		// reset staffer variable. We've already set his assignment so everything's good.
		gpFacilityStaffer = NULL;
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{
		StopTimeCompression();
	}

	return;
}

// IMPORTANT: Rejected player prompt to pay hourly for using a facility.
void FacilityStaffingRejected( )
{
	// take the selected merc off Facility work.
	gpFacilityStaffer->sFacilityTypeOperated = -1;
	AddCharacterToAnySquad( gpFacilityStaffer );

	// this completes the facility costs prompt sequence
	gpFacilityStaffer = NULL;
}

// Resets all assignments for characters working at facilities that cost money to operate. This is run whenever the
// player incurs an unpayable debt for facility operation.
void ResetAllExpensiveFacilityAssignments()
{
	SOLDIERTYPE *pSoldier;
	UINT8 ubCounter = 0;

	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];

		// Is character doing facility work?
		INT8 ubAssignmentIndex = GetSoldierFacilityAssignmentIndex( pSoldier );
		if( ubAssignmentIndex != -1 )
		{
			UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
			// Does facility cost money to operate?
			if (gFacilityTypes[ubFacilityType].AssignmentData[ubAssignmentIndex].sCostPerHour)
			{
				// Reset assignment.
				ResumeOldAssignment( pSoldier );
			}
		}
		ubCounter++;
	}
}

BOOLEAN IsOutstandingFacilityDebtWithErrorReport()
{
	CHAR16 sString[256];
	SGPRect pCenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, INV_INTERFACE_START_Y };

	if (giTotalOwedForFacilityOperationsToday && // Owe money
		gfOutstandingFacilityDebt ) // Owed money tonight as well
	{
		if (LaptopSaveInfo.iCurrentBalance)
		{
			if (LaptopSaveInfo.iCurrentBalance >= giTotalOwedForFacilityOperationsToday)
			{
				// Do message "want to pay entire sum?"
				swprintf( sString, New113HAMMessage[18], giTotalOwedForFacilityOperationsToday );
				if( guiCurrentScreen == MAP_SCREEN )
				{
					DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayFacilityDebtManuallyYesNoBoxCallback );
				}
				else
				{
					DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO, PayFacilityDebtManuallyYesNoBoxCallback, &pCenteringRect );
				}
				return FALSE;
			}
			else
			{
				// Do message "not enough money to pay entire debt"
				swprintf( sString, New113HAMMessage[17], giTotalOwedForFacilityOperationsToday );
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
				return FALSE;
			}
		}
		else
		{
			// No money!
			swprintf( sString, New113HAMMessage[16], giTotalOwedForFacilityOperationsToday );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			return FALSE;
		}
	}
	
	return TRUE;
}

// HEADROCK HAM 3.6: Create the facility menu box.
void CreateFacilityAssignmentBox()
{
	FacilityAssignmentPosition.iX = OrigFacilityAssignmentPosition.iX;

	if( giBoxY != 0 )
	{
		FacilityAssignmentPosition.iY = giBoxY + ( ASSIGN_MENU_FACILITY * GetFontHeight( MAP_SCREEN_FONT ) );
	}

	CreatePopUpBox(&ghFacilityAssignmentBox, FacilityAssignmentDimensions, FacilityAssignmentPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
	SetBoxBuffer(ghFacilityAssignmentBox, FRAME_BUFFER);
	SetBorderType(ghFacilityAssignmentBox,guiPOPUPBORDERS);
	SetBackGroundSurface(ghFacilityAssignmentBox, guiPOPUPTEX);
	SetMargins( ghFacilityAssignmentBox, 6, 6, 4, 4 );
	SetLineSpace(ghFacilityAssignmentBox, 2);

	// resize box to text
	ResizeBoxToText( ghFacilityAssignmentBox );

	DetermineBoxPositions( );
}

void CreateDestroyMouseRegionsForFacilityAssignmentMenu( void )
{
	static BOOLEAN fCreated = FALSE;

	UINT32 uiMenuLine = 0;
	UINT8 ubFacilityType = gubFacilityInSubmenu;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;
	INT8 bCurrentVehicleID = -1;

	INT32 iCounter = 0;
	INT32 iCounterB = 0;
	BOOLEAN fFoundVehicle = FALSE;


	if( ( fShowFacilityAssignmentMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed, this missplaces popups when screensize>3.
		//if( ( fShowAssignmentMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		//	SetBoxPosition( ghAssignmentBox, AssignmentPosition );
		//}

		//HandleShadingOfLinesForFacilityAssignmentMenus( );
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );

		// grab height of font
		iFontHeight = GetLineSpace( ghFacilityAssignmentBox ) + GetFontHeight( GetBoxFont( ghFacilityAssignmentBox ) );

		// get x.y position of box
		GetBoxPosition( ghFacilityAssignmentBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghFacilityAssignmentBox, &pDimensions );
		SetBoxSecondaryShade( ghFacilityAssignmentBox, FONT_YELLOW );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghFacilityAssignmentBox );

		pSoldier = GetSelectedAssignSoldier( FALSE );

		// define regions
		//for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghFacilityBox ); iCounter++ )
		// run through list of staff/use facilities in sector and add them to pop up box
		for ( iCounter = 0; iCounter < NUM_FACILITY_ASSIGNMENTS; iCounter++ )
		{
			if ( gFacilityTypes[ ubFacilityType ].AssignmentData[iCounter].ubStaffLimit )
			{
				if ( iCounter == FAC_REPAIR_VEHICLE )
				{
					for ( iCounterB = 0; iCounterB < ubNumberOfVehicles; iCounterB++ )
					{
						if ( pVehicleList[iCounterB].fValid == TRUE )
						{
							if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounterB ) )
							{
								MSYS_DefineRegion( &gFacilityAssignmentMenuRegion[ uiMenuLine ],	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
									MSYS_NO_CURSOR, FacilityAssignmentMenuMvtCallBack, FacilityAssignmentMenuBtnCallback );
								MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 0, uiMenuLine );
								MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 1, iCounter );
								// Store Vehicle ID in THIRD location
								MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 2, iCounterB );
								uiMenuLine++;
								fFoundVehicle = TRUE;
							}
						}
					}
					if (!fFoundVehicle)
					{
						// WANNE: This is a fix by Headrock
						//uiMenuLine++; // Skip this line, it'll always be shaded anyway.
					}
				}
				else if ( iCounter == FAC_REPAIR_ROBOT )
				{
					// is the ROBOT here?
					if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
					{
						// robot line only appears when it is around.
						MSYS_DefineRegion( &gFacilityAssignmentMenuRegion[ uiMenuLine ],	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
									MSYS_NO_CURSOR, FacilityAssignmentMenuMvtCallBack, FacilityAssignmentMenuBtnCallback );
						MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 0, uiMenuLine );
						MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 1, iCounter );
						MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 2, -1 );
						uiMenuLine++;
					}
				}
				else if( ( iCounter == FAC_PRISON_SNITCH || iCounter == FAC_SPREAD_PROPAGANDA || iCounter == FAC_SPREAD_PROPAGANDA_GLOBAL || iCounter == FAC_GATHER_RUMOURS ) &&
					!CanCharacterSnitch( pSoldier ))
				{
					// anv: don't show line then
				}
				else
				{
					// add mouse region for each remaining assignment type
					MSYS_DefineRegion( &gFacilityAssignmentMenuRegion[ uiMenuLine ],	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
								MSYS_NO_CURSOR, FacilityAssignmentMenuMvtCallBack, FacilityAssignmentMenuBtnCallback );

					// Add tooltip for region
					if (wcscmp(gFacilityTypes[ ubFacilityType ].AssignmentData[iCounter].szTooltipText, L"") != 0)
					{
						CHAR16 szTextLeft[300];
						CHAR16 szNewTextLeft[300];
						CHAR16 szTooltipText[500];
						wcscpy( szTextLeft, gFacilityTypes[ ubFacilityType ].AssignmentData[iCounter].szTooltipText );
						swprintf( szTooltipText, L"" );
						// Del First Part
						BOOLEAN fLineSplit = TRUE;
						
						while (fLineSplit)
						{
							fLineSplit = WrapString( szTextLeft, szNewTextLeft, 250, FONT10ARIAL );
							wcscat( szTooltipText, szTextLeft );
							wcscat( szTooltipText, L"\n"); // Add line break.
							wcscpy( szTextLeft, szNewTextLeft );
						}

						SetRegionFastHelpText( &gFacilityAssignmentMenuRegion[ uiMenuLine ], szTooltipText );
					}
					//SetRegionHelpEndCallback( &gFacilityAssignmentMenuRegion[ uiMenuLine ], HelpTextDoneCallback );

					MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 0, uiMenuLine );
					// store assignment type ID in the SECOND user data
					MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 1, iCounter );
					MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 2, -1 );
					uiMenuLine++;
				}
			}
		}

		// cancel line
		MSYS_DefineRegion( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * uiMenuLine ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( uiMenuLine + 1 ) ), MSYS_PRIORITY_HIGHEST - 2 ,
							MSYS_NO_CURSOR, FacilityAssignmentMenuMvtCallBack, FacilityAssignmentMenuBtnCallback );
		MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 0, uiMenuLine );
		MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 1, NUM_FACILITY_ASSIGNMENTS );
		MSYS_SetRegionUserData( &gFacilityAssignmentMenuRegion[ uiMenuLine ], 2, -1 );

		// created
		fCreated = TRUE;

		// pause game
		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghFacilityAssignmentBox );

		fCreated = TRUE;

		//HandleShadingOfLinesForFacilityMenu( );
	}
	else if( ( ( fShowFacilityAssignmentMenu == FALSE ) || ( fShowFacilityMenu == FALSE ) || ( fShowAssignmentMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;

		// remove these regions
		for( uiMenuLine = 0; uiMenuLine < GetNumberOfLinesOfTextInBox( ghFacilityAssignmentBox ); uiMenuLine++ )
		{
			MSYS_RemoveRegion( &gFacilityAssignmentMenuRegion[ uiMenuLine ] );
		}

		fShowFacilityAssignmentMenu = FALSE;
		gfRenderPBInterface = TRUE;

		RestorePopUpBoxes( );
		fMapPanelDirty = TRUE;
		fCharacterInfoPanelDirty= TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		HideBox( ghFacilityAssignmentBox );
		SetRenderFlags( RENDER_FLAG_FULL );

		if ( fShowFacilityMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghFacilityAssignmentBox );
		}
	}

	return;
}

void FacilityAssignmentMenuMvtCallBack ( MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string
		if( GetBoxShadeFlag( ghFacilityAssignmentBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghFacilityAssignmentBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghFacilityAssignmentBox );
	}
	return;
}

void FacilityAssignmentMenuBtnCallback ( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	BOOLEAN fCanOperateFacility = TRUE;

	SOLDIERTYPE* pSoldier = GetSelectedAssignSoldier( FALSE );
	INT16 ubFacilityType = gubFacilityInSubmenu;
	INT16 ubAssignmentType = (INT16)MSYS_GetRegionUserData( pRegion, 1 );
	INT16 ubVehicleID = (INT16)MSYS_GetRegionUserData( pRegion, 2 );

	if (ubFacilityType <= 0 || ubFacilityType >= NUM_FACILITY_TYPES || ubAssignmentType <= 0)
	{
		return;
	}

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN ) )
	{
		if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && !fShowMapInventoryPool )
		{
			UnMarkButtonDirty( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (ubAssignmentType > 0 && ubAssignmentType < NUM_FACILITY_ASSIGNMENTS )
		{
			if (!BasicCanCharacterFacility( pSoldier ))
			{
				return;
			}

			if (!CanCharacterFacilityWithErrorReport( pSoldier, (UINT8)ubFacilityType, (UINT8)ubAssignmentType ) )
			{
				return;
			}

			// Check for standing debt for Facility Operation. May trigger an on-screen prompt or error message.
			if ( gFacilityTypes[ ubFacilityType ].AssignmentData[ ubAssignmentType ].sCostPerHour > 0 && // Facility costs money to operate
				!IsOutstandingFacilityDebtWithErrorReport() ) // There's an outstanding debt for facilities
			{
				// Facility debt needs to be paid first.
				return;
			}

			// PASSED ALL TESTS - ALLOW SOLDIER TO WORK AT THIS FACILITY.

			fShowAssignmentMenu = FALSE;
			giAssignHighLine = -1;

			pSoldier->bOldAssignment = pSoldier->bAssignment; // Set Old Assignment

			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}
			RemoveCharacterFromSquads(	pSoldier );

			// Change assignment to new type
			switch (ubAssignmentType)
			{
				case FAC_STAFF:
					ChangeSoldiersAssignment( pSoldier, FACILITY_STAFF );
					break;
				case FAC_FOOD:
					ChangeSoldiersAssignment( pSoldier, FACILITY_EAT );
					break;
				case FAC_REST:
					ChangeSoldiersAssignment( pSoldier, FACILITY_REST );
					break;
				case FAC_REPAIR_ITEMS:
					MakeSureToolKitIsInHand( pSoldier );
					ChangeSoldiersAssignment( pSoldier, REPAIR );
					pSoldier->flags.fFixingRobot = FALSE;
					pSoldier->bVehicleUnderRepairID = -1;
					break;
				case FAC_REPAIR_VEHICLE:
					MakeSureToolKitIsInHand( pSoldier );
					ChangeSoldiersAssignment( pSoldier, REPAIR );
					pSoldier->flags.fFixingRobot = FALSE;
					pSoldier->bVehicleUnderRepairID = (INT8)ubVehicleID;
					break;
				case FAC_REPAIR_ROBOT:
					MakeSureToolKitIsInHand( pSoldier );
					ChangeSoldiersAssignment( pSoldier, REPAIR );
					pSoldier->flags.fFixingRobot = TRUE;
					pSoldier->bVehicleUnderRepairID = -1;
					break;
				case FAC_DOCTOR:
					MakeSureMedKitIsInHand( pSoldier );
					ChangeSoldiersAssignment( pSoldier, DOCTOR );
					break;
				case FAC_PATIENT:
					ChangeSoldiersAssignment( pSoldier, PATIENT );
					break;
				case FAC_PRACTICE_STRENGTH:
					pSoldier->bTrainStat = STRENGTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_DEXTERITY:
					pSoldier->bTrainStat = DEXTERITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_AGILITY:
					pSoldier->bTrainStat = AGILITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_HEALTH:
					pSoldier->bTrainStat = HEALTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_MARKSMANSHIP:
					pSoldier->bTrainStat = MARKSMANSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_LEADERSHIP:
					pSoldier->bTrainStat = LEADERSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_MEDICAL:
					pSoldier->bTrainStat = MEDICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_MECHANICAL:
					pSoldier->bTrainStat = MECHANICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_PRACTICE_EXPLOSIVES:
					pSoldier->bTrainStat = EXPLOSIVE_ASSIGN;
					ChangeSoldiersAssignment( pSoldier, TRAIN_SELF );
					break;
				case FAC_STUDENT_STRENGTH:
					pSoldier->bTrainStat = STRENGTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_DEXTERITY:
					pSoldier->bTrainStat = DEXTERITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_AGILITY:
					pSoldier->bTrainStat = AGILITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_HEALTH:
					pSoldier->bTrainStat = HEALTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_MARKSMANSHIP:
					pSoldier->bTrainStat = MARKSMANSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_LEADERSHIP:
					pSoldier->bTrainStat = LEADERSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_MEDICAL:
					pSoldier->bTrainStat = MEDICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_MECHANICAL:
					pSoldier->bTrainStat = MECHANICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_STUDENT_EXPLOSIVES:
					pSoldier->bTrainStat = EXPLOSIVE_ASSIGN;
					ChangeSoldiersAssignment( pSoldier, TRAIN_BY_OTHER );
					break;
				case FAC_TRAINER_STRENGTH:
					pSoldier->bTrainStat = STRENGTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_DEXTERITY:
					pSoldier->bTrainStat = DEXTERITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_AGILITY:
					pSoldier->bTrainStat = AGILITY;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_HEALTH:
					pSoldier->bTrainStat = HEALTH;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_MARKSMANSHIP:
					pSoldier->bTrainStat = MARKSMANSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_LEADERSHIP:
					pSoldier->bTrainStat = LEADERSHIP;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_MEDICAL:
					pSoldier->bTrainStat = MEDICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_MECHANICAL:
					pSoldier->bTrainStat = MECHANICAL;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_TRAINER_EXPLOSIVES:
					pSoldier->bTrainStat = EXPLOSIVE_ASSIGN;
					ChangeSoldiersAssignment( pSoldier, TRAIN_TEAMMATE );
					break;
				case FAC_INTERROGATE_PRISONERS:
					MakeSoldierKnownAsMercInPrison( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY );
					ChangeSoldiersAssignment( pSoldier, FACILITY_INTERROGATE_PRISONERS );
					break;
				case FAC_PRISON_SNITCH:
					ChangeSoldiersAssignment( pSoldier, FACILITY_PRISON_SNITCH );
					break;
				case FAC_SPREAD_PROPAGANDA:
					ChangeSoldiersAssignment( pSoldier, FACILITY_SPREAD_PROPAGANDA );
					break;
				case FAC_SPREAD_PROPAGANDA_GLOBAL:
					ChangeSoldiersAssignment( pSoldier, FACILITY_SPREAD_PROPAGANDA_GLOBAL );
					break;
				case FAC_GATHER_RUMOURS:
					ChangeSoldiersAssignment( pSoldier, FACILITY_GATHER_RUMOURS );
					break;
				case FAC_STRATEGIC_MILITIA_MOVEMENT:
					ChangeSoldiersAssignment( pSoldier, FACILITY_STRATEGIC_MILITIA_MOVEMENT );
					break;
			}
			
			// Flugente: I guess this piece of code is here to get a group Id for the soldier, which must not be there for movement specifically. Just my understanding, in case anybody else coming here wonders
			// why we get movement related stuff when we were jsut ordered to stay in a facility
			AssignMercToAMovementGroup( pSoldier );			
			MakeSoldiersTacticalAnimationReflectAssignment( pSoldier );

			pSoldier->sFacilityTypeOperated = ubFacilityType; // Set soldier as working at this facility.

			if( gFacilityTypes[ ubFacilityType ].AssignmentData[ ubAssignmentType ].sCostPerHour > 0 )
			{
				// Ask player if he wishes to expend money on operating the facility.
				HandleInterfaceMessageForCostOfOperatingFacility( pSoldier, (UINT8)ubAssignmentType );
			}
			gfRenderPBInterface = TRUE;
		}
		else
		{
			// stop showing menu
			fShowFacilityAssignmentMenu = FALSE;

			// unhighlight the assignment box
			UnHighLightBox( ghFacilityBox );

			// reset list
			ResetSelectedListForMapScreen( );
			gfRenderPBInterface = TRUE;
		}
		// rerender tactical stuff
		gfRenderPBInterface = TRUE;

		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}

}

BOOLEAN HandleAssignmentExpansionAndHighLightForFacilityMenu( void )
{
	if( fShowFacilityAssignmentMenu )
	{
		HighLightBoxLine( ghFacilityBox, gubFacilityLineForSubmenu );
		return( TRUE );
	}

	return( FALSE );
}

void HandleShadingOfLinesForFacilityAssignmentMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0;
	INT32 iCounterB = 0;
	INT32 iNumLine = 0;
	UINT8 ubFacilityType = gubFacilityInSubmenu;
	BOOLEAN fFoundVehicle = FALSE;

	if( ( fShowFacilityAssignmentMenu == FALSE ) || ( ghFacilityAssignmentBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );

	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForMoveItemMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForMoveItemMenu().

	// run through all possible assignments. Shade as necessary
	for ( iCounter = 0; iCounter < NUM_FACILITY_ASSIGNMENTS; iCounter++ )
	{
		if ( gFacilityTypes[ubFacilityType].AssignmentData[iCounter].ubStaffLimit )
		{
			if ( iCounter == FAC_REPAIR_VEHICLE )
			{
				// Test to see whether there are any.
				for ( iCounterB = 0; iCounterB < ubNumberOfVehicles; iCounterB++ )
				{
					if ( pVehicleList[iCounterB].fValid == TRUE )
					{
						if ( IsThisVehicleAccessibleToSoldier( pSoldier, iCounterB ) &&
							CanCharacterFacility( pSoldier, ubFacilityType, iCounter ) )
						{
							UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
							UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						}
						else
						{
							UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
							SecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						}
						iNumLine++;
						fFoundVehicle = TRUE;
					}
				}
				if (fFoundVehicle == FALSE)
				{
					// The line here says "Repair Vehicle", and is shaded because there's no vehicle present.
					UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					ShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					iNumLine++;
				}
			}
			else if ( iCounter == FAC_REPAIR_ROBOT )
			{
				// is the ROBOT here?
				if( IsRobotInThisSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) )
				{
					if ( CanCharacterFacility( pSoldier, ubFacilityType, iCounter ) )
					{
						UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					}
					else
					{
						UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						SecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					}
					iNumLine++;
				}
				else
				{
					// Line won't appear in the menu, so don't bother shading it.
				}
			}
			else if ( iCounter == FAC_PATIENT )
			{
				// is injured?
				if ( CanCharacterPatient ( pSoldier ) )
				{
					UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				}
				else
				{
					// A fully shaded line appears.
					UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					ShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				}
				iNumLine++;
			}
			else if ( iCounter == FAC_PRISON_SNITCH || iCounter == FAC_SPREAD_PROPAGANDA || iCounter == FAC_SPREAD_PROPAGANDA_GLOBAL || iCounter == FAC_GATHER_RUMOURS )
			{
				// is character a snitch
				if ( CanCharacterSnitch( pSoldier ) )
				{
					if ( CanCharacterFacility( pSoldier, ubFacilityType, iCounter ) )
					{
						UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					}
					else
					{
						UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
						SecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
					}
					iNumLine++;
				}
				else
				{
					// Line won't appear in the menu, so don't bother shading it.
				}
			}
			else if ( !BasicCanCharacterFacility( pSoldier ) )
			{
				// Character cannot perform facility work at all. This shouldn't happen, actually.
				// Every line in this menu is, in theory, either valid or "not valid right now"...
				UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				ShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				iNumLine++;
			}
			else if ( !CanCharacterFacility( pSoldier, ubFacilityType, iCounter ) )
			{
				UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				SecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				iNumLine++;
			}
			else
			{
				UnShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				UnSecondaryShadeStringInBox( ghFacilityAssignmentBox, iNumLine );
				iNumLine++;
			}
		}
	}
	return;
}

// SANDRO - function for automatic surgery button callback
void SurgeryBeforeDoctoringRequesterCallback( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		if (MakeAutomaticSurgeryOnAllPatients( pAutomaticSurgeryDoctor ) > 0)
		{
			DoScreenIndependantMessageBox( L"Healed!" , MSG_BOX_FLAG_OK, NULL );
		}
		else
		{
			DoScreenIndependantMessageBox( L"NOT Healed!" , MSG_BOX_FLAG_OK, NULL );
		}
		pAutomaticSurgeryDoctor = NULL;

	}
}

// SANDRO - function for automatic surgery button callback
void SurgeryBeforePatientingRequesterCallback( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		if( (CanSoldierBeHealedByDoctor( pAutomaticSurgeryPatient, pAutomaticSurgeryDoctor, FALSE, HEALABLE_EVER, FALSE, FALSE, TRUE ) == TRUE ) &&
				(MakeAutomaticSurgery( pAutomaticSurgeryPatient, pAutomaticSurgeryDoctor ) == TRUE) )
		{
			DoScreenIndependantMessageBox( L"Healed!" , MSG_BOX_FLAG_OK, NULL );
		}
		else
		{
			DoScreenIndependantMessageBox( L"NOT Healed!" , MSG_BOX_FLAG_OK, NULL );
		}
		pAutomaticSurgeryDoctor = NULL;

	}
}
// SANDRO - function for automatic surgery on all patients
INT16 MakeAutomaticSurgeryOnAllPatients( SOLDIERTYPE * pDoctor )
{
	int cnt;
	SOLDIERTYPE *pTeamSoldier = NULL;
	UINT8 ubNumberOfPeopleHealed = 0;

	AssertNotNIL(pDoctor);

	// go through list of characters, find all who are patients/doctors healable by this doctor
	for ( cnt = 0, pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pDoctor->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( CanSoldierBeHealedByDoctor( pTeamSoldier, pDoctor, FALSE, HEALABLE_EVER, FALSE, FALSE, TRUE ) == TRUE )
		{
			if( MakeAutomaticSurgery( pTeamSoldier, pDoctor ) == TRUE )
			{
				// increment number of doctorable patients/doctors
				ubNumberOfPeopleHealed++;
			}
		}
	}

	return( ubNumberOfPeopleHealed );
}
// SANDRO - automatic surgery
BOOLEAN MakeAutomaticSurgery( SOLDIERTYPE * pSoldier, SOLDIERTYPE * pDoctor )
{
	UINT16 usKitPts;
	UINT32 uiPointsUsed;
	OBJECTTYPE *pKit = NULL;
	INT8 bSlot, cnt;
	INT32 bLifeToReturn = 0;

	if ( gSkillTraitValues.ubDONumberTraitsNeededForSurgery > NUM_SKILL_TRAITS( pDoctor, DOCTOR_NT ) )
	{
		return( FALSE );
	}

	cnt = 0;
	while( pSoldier->iHealableInjury >= 100 )
	{
		bSlot = FindMedKit( pDoctor );
		if ( bSlot != NO_SLOT )
		{
			pKit = &pDoctor->inv[ bSlot ];
		}
		else
		{
			break;
		}
		usKitPts = TotalPoints( pKit );

		uiPointsUsed = VirtualSoldierDressWound( pDoctor, pSoldier, pKit, usKitPts, usKitPts, TRUE );
		UseKitPoints( pKit, (UINT16)uiPointsUsed, pDoctor );
		
		cnt++;
		if( cnt > 30 )
			break;
	}

	if ( pSoldier->iHealableInjury < 100 )
	{
		pSoldier->iHealableInjury = 0;
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}

}

// SANDRO - added a function to write down to our records, how many militia we trained
void RecordNumMilitiaTrainedForMercs( INT16 sX, INT16 sY, INT8 bZ, UINT8 ubMilitiaTrained, BOOLEAN fMobile )
{

	UINT16 cnt = 0;
	SOLDIERTYPE * pTrainer;
	UINT16 usTotalLeadershipValue = 0;
	UINT8 usTrainerEffectiveLeadership = 0;

	// First, get total leadership value of all trainers
	for ( pTrainer = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pTrainer++)
	{
		if (pTrainer->bActive && pTrainer->stats.bLife >= OKLIFE && pTrainer->sSectorX == sX && pTrainer->sSectorY == sY && pTrainer->bSectorZ == bZ &&
			( (!fMobile && pTrainer->bAssignment == TRAIN_TOWN) || (fMobile && pTrainer->bAssignment == TRAIN_MOBILE) ) )
		{
			usTrainerEffectiveLeadership = EffectiveLeadership( pTrainer );

			if ( gGameOptions.fNewTraitSystem ) //old/new traits
			{
				// -10% penalty for untrained mercs
				usTrainerEffectiveLeadership = (usTrainerEffectiveLeadership * (100 + gSkillTraitValues.bSpeedModifierTrainingMilitia) / 100);

				if (HAS_SKILL_TRAIT( pTrainer, TEACHING_NT ))
				{
					// bonus from Teaching trait
					usTrainerEffectiveLeadership = __min(100,(usTrainerEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 ));
				}
			}
			// Effective leadership is modified by an INI-based percentage, once for every TEACHING trait level.
			else if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
			{
				for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pTrainer, TEACHING_OT ); i++ )
				{
					// percentage-based.
					usTrainerEffectiveLeadership = __min(100,((usTrainerEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100));
				}
			}
			
			if (gGameExternalOptions.fLeadershipAffectsMilitiaQuantity)
			{
				usTrainerEffectiveLeadership = __max(usTrainerEffectiveLeadership, gGameExternalOptions.ubMinimumLeadershipToTrainMilitia);
				if (usTrainerEffectiveLeadership > gGameExternalOptions.ubReqLeadershipForFullTraining) 
					usTrainerEffectiveLeadership = __min( 100, (gGameExternalOptions.ubReqLeadershipForFullTraining + ((usTrainerEffectiveLeadership - gGameExternalOptions.ubReqLeadershipForFullTraining)/2)));
				
				usTrainerEffectiveLeadership = __max( 0, (usTrainerEffectiveLeadership - gGameExternalOptions.ubMinimumLeadershipToTrainMilitia));
			}

			// Add to the total amount
			usTotalLeadershipValue += __min(100,usTrainerEffectiveLeadership);
		}
	}
	// Now we have to run again and percentually award points towards militia trained
	cnt = 0;
	for ( pTrainer = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pTrainer++)
	{
		if (pTrainer->bActive && pTrainer->stats.bLife >= OKLIFE && pTrainer->sSectorX == sX && pTrainer->sSectorY == sY && pTrainer->bSectorZ == bZ &&
			( (!fMobile && pTrainer->bAssignment == TRAIN_TOWN) || (fMobile && pTrainer->bAssignment == TRAIN_MOBILE) ) )
		{
			usTrainerEffectiveLeadership = EffectiveLeadership( pTrainer );

			if ( gGameOptions.fNewTraitSystem ) //old/new traits
			{
				// -10% penalty for untrained mercs
				usTrainerEffectiveLeadership = (usTrainerEffectiveLeadership * (100 + gSkillTraitValues.bSpeedModifierTrainingMilitia) / 100);

				if (HAS_SKILL_TRAIT( pTrainer, TEACHING_NT ))
				{
					// bonus from Teaching trait
					usTrainerEffectiveLeadership = __min(100,(usTrainerEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 ));
				}
			}
			// Effective leadership is modified by an INI-based percentage, once for every TEACHING trait level.
			else if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
			{
				for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pTrainer, TEACHING_OT ); i++ )
				{
					// percentage-based.
					usTrainerEffectiveLeadership = __min(100,((usTrainerEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100));
				}
			}
			
			if (gGameExternalOptions.fLeadershipAffectsMilitiaQuantity)
			{
				usTrainerEffectiveLeadership = __max(usTrainerEffectiveLeadership, gGameExternalOptions.ubMinimumLeadershipToTrainMilitia);
				if (usTrainerEffectiveLeadership > gGameExternalOptions.ubReqLeadershipForFullTraining) 
					usTrainerEffectiveLeadership = __min( 100, (gGameExternalOptions.ubReqLeadershipForFullTraining + ((usTrainerEffectiveLeadership - gGameExternalOptions.ubReqLeadershipForFullTraining)/2)));
				
				usTrainerEffectiveLeadership = __max( 0, (usTrainerEffectiveLeadership - gGameExternalOptions.ubMinimumLeadershipToTrainMilitia));
			}

			if( usTrainerEffectiveLeadership > 0 )
			{
				gMercProfiles[ pTrainer->ubProfile ].records.usMilitiaTrained += (UINT16)((double)((double)(ubMilitiaTrained * usTrainerEffectiveLeadership) / usTotalLeadershipValue) + 0.5);
			}
		}
	}
}
#ifdef JA2UB
//Ja25 UB

BOOLEAN CanMercBeAllowedToLeaveTeam( SOLDIERTYPE *pSoldier )
{
/*
	//if we are in sector... J14_1 && K14_1
	if( gWorldSectorX == 14 && gWorldSectorY == MAP_ROW_J && gbWorldSectorZ == 1 ||	
			gWorldSectorX == 14 && gWorldSectorY == MAP_ROW_K && gbWorldSectorZ == 1 )
*/
	//if we are in, or passed the tunnels
	if( pSoldier->sSectorX >= 14 )
	{
		//dont allow anyone to leave
		return( FALSE );
	}

	return( TRUE );
}

void HaveMercSayWhyHeWontLeave( SOLDIERTYPE *pSoldier )
{
	//if the merc is qualified
	if( IsSoldierQualifiedMerc( pSoldier ) )
	{
		TacticalCharacterDialogue( pSoldier, QUOTE_ANSWERING_MACHINE_MSG );
	}
	else
	{
		TacticalCharacterDialogue( pSoldier, QUOTE_REFUSING_ORDER );
	}
}
#endif

// Flugente: move items menu
BOOLEAN DisplayMoveItemsMenu( SOLDIERTYPE *pSoldier )
{
	INT32 iVehicleIndex=0;
	INT32 hStringHandle=0;
	INT32 iCount = 0;

	// run through list of vehicles in sector and add them to pop up box
	// first, clear pop up box
	RemoveBox(ghMoveItemBox);
	ghMoveItemBox = -1;

	CreateMoveItemBox();
	SetCurrentBox(ghMoveItemBox);

	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForMoveItemMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForMoveItemMenu().

	// delete old sectors
	for (UINT8 i = 0; i < MOVEITEM_MAX_SECTORS; ++i)
	{
		usMoveItemSectors[i] = 0;
	}

	// we now have to show every sector of the town we are in
	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

	if ( bTownId != BLANK_SECTOR )
	{
		for (UINT16 X = 0; X < 256; ++X)
		{
			INT16 sectorX = SECTORX(X);
			INT16 sectorY = SECTORY(X);

			// if sector not under our control, has enemies in it, or is currently in combat mode
			if (!SectorOursAndPeaceful( sectorX, sectorY, 0 ) )
				continue;

			if ( sectorX == pSoldier->sSectorX && sectorY == pSoldier->sSectorY )
				continue;

			if ( GetTownIdForSector( sectorX, sectorY ) == bTownId )
			{
				usMoveItemSectors[iCount] = (UINT8)X;

				CHAR16 wSectorName[ 64 ];
				GetShortSectorString( sectorX, sectorY, wSectorName );

				AddMonoString( (UINT32 *)&hStringHandle, wSectorName );

				iCount++;
				if ( iCount >= MOVEITEM_MAX_SECTORS )
					break;
			}
		}
	}
	
	// cancel
	AddMonoString((UINT32 *)&hStringHandle, L"Cancel" );

	SetBoxFont(ghMoveItemBox, MAP_SCREEN_FONT);
	SetBoxHighLight(ghMoveItemBox, FONT_WHITE);
	SetBoxShade(ghMoveItemBox, FONT_GRAY7);
	SetBoxForeground(ghMoveItemBox, FONT_LTGREEN);
	SetBoxBackground(ghMoveItemBox, FONT_BLACK);

	// resize box to text
	ResizeBoxToText( ghMoveItemBox );

	CheckAndUpdateTacticalAssignmentPopUpPositions( );

	return TRUE;
}


void HandleShadingOfLinesForMoveItemMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleIndex = 0;
	INT32 iCount = 0;
	
	if( ( fShowMoveItemMenu == FALSE ) || ( ghMoveItemBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( FALSE );


	// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
	// CreateDestroyMouseRegionForMoveItemMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForMoveItemMenu().

	// we now have to show every sector of the town we are in
	INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

	// only in towns
	if ( bTownId != BLANK_SECTOR && pSoldier->bSectorZ == 0 )
	{
		for(UINT i = 0; i < MOVEITEM_MAX_SECTORS; ++i)
		{
			UINT8 sector = usMoveItemSectors[i];

			if ( sector > 0 )
			{
				INT16 sectorX = SECTORX(sector);
				INT16 sectorY = SECTORY(sector);

				UINT32 uiTotalNumberOfRealItems = 0;
				GetNumberOfWorldItemsFromTempItemFile( sectorX, sectorY, 0, &( uiTotalNumberOfRealItems ), FALSE );

				if ( uiTotalNumberOfRealItems > 0 )
				{
					// unshade vehicle line
					UnShadeStringInBox( ghMoveItemBox, iCount );
				}
				else
				{
					// shade vehicle line
					ShadeStringInBox( ghMoveItemBox, iCount );
				}
			}
		}
	}

	//if ( DoesCharacterHaveAnyItemsToRepair( pSoldier, FINAL_REPAIR_PASS ) )
	if ( 1 )
	{
		// unshade items line
		UnShadeStringInBox( ghMoveItemBox, iCount );
	}
	else
	{
		// shade items line
		ShadeStringInBox( ghMoveItemBox, iCount );
	}

	iCount++;


	return;
}


void CreateDestroyMouseRegionForMoveItemMenu( void )
{
	static BOOLEAN fCreated = FALSE;

	UINT32 uiCounter = 0;
	INT32 iCount = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleIndex = 0;
	
	if( ( fShowMoveItemMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		// Moa: removed below: repositioning now the same way as for training in AssignmentMenuBtnCB as it caused missplaced box for higher resolutions then 3.
		//CheckAndUpdateTacticalAssignmentPopUpPositions( );
		//if( ( fShowMoveItemMenu ) && ( guiCurrentScreen == MAP_SCREEN ) )
		//{
		// //SetBoxPosition( ghMoveItemBox ,RepairPosition);
		//}

		// grab height of font
		iFontHeight = GetLineSpace( ghMoveItemBox ) + GetFontHeight( GetBoxFont( ghMoveItemBox ) );

		// get x.y position of box
		GetBoxPosition( ghMoveItemBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghMoveItemBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghMoveItemBox );

		pSoldier = GetSelectedAssignSoldier( FALSE );

		// PLEASE NOTE: make sure any changes you do here are reflected in all 3 routines which must remain in synch:
		// CreateDestroyMouseRegionForMoveItemMenu(), DisplayRepairMenu(), and HandleShadingOfLinesForMoveItemMenu().

		// we now have to show every sector of the town we are in
		INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );

		// only in towns
		if ( bTownId != BLANK_SECTOR && pSoldier->bSectorZ == 0 )
		{
			for(UINT i = 0; i < MOVEITEM_MAX_SECTORS; ++i)
			{
				UINT8 sector = usMoveItemSectors[i];

				if ( sector > 0 )
				{
					// add mouse region for each line of text..and set user data
					MSYS_DefineRegion( &gMoveItem[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
								MSYS_NO_CURSOR, MoveItemMenuMvtCallback, MoveItemMenuBtnCallback );

					MSYS_SetRegionUserData( &gMoveItem[ iCount ], 0, iCount );
					// 2nd user data is the vehicle index, which can easily be different from the region index!
					MSYS_SetRegionUserData( &gMoveItem[ iCount ], 1, iCount );
					iCount++;

					if ( iCount >= MOVEITEM_MAX_SECTORS )
						break;
				}
			}
		}
		
		// cancel
		MSYS_DefineRegion( &gMoveItem[ iCount ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * iCount ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghAssignmentBox ) + ( iFontHeight ) * ( iCount + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, MoveItemMenuMvtCallback, MoveItemMenuBtnCallback );

		MSYS_SetRegionUserData( &gMoveItem[ iCount ], 0, iCount );
		MSYS_SetRegionUserData( &gMoveItem[ iCount ], 1, MOVEITEM_MENU_CANCEL );


		PauseGame( );

		// unhighlight all strings in box
		UnHighLightBox( ghMoveItemBox );

		fCreated = TRUE;
	}
	else if( ( ( fShowMoveItemMenu == FALSE ) || ( fShowAssignmentMenu == FALSE ) ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;

		// remove these regions
		for( uiCounter = 0; uiCounter < GetNumberOfLinesOfTextInBox( ghMoveItemBox ); uiCounter++ )
		{
			MSYS_RemoveRegion( &gMoveItem[ uiCounter ] );
		}

		fShowMoveItemMenu = FALSE;

		SetRenderFlags( RENDER_FLAG_FULL );

		HideBox( ghRepairBox );

		if ( fShowAssignmentMenu )
		{
			// remove highlight on the parent menu
			UnHighLightBox( ghAssignmentBox );
		}
	}

	return;
}

void MoveItemMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment region
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iWhat;

	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	// ignore clicks on disabled lines
	if( GetBoxShadeFlag( ghMoveItemBox, iValue ) == TRUE )
	{
		return;
	}

	// WHAT is being repaired is stored in the second user data argument
	iWhat = MSYS_GetRegionUserData( pRegion, 1 );

	pSoldier = GetSelectedAssignSoldier( FALSE );

	if ( pSoldier && pSoldier->bActive && ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		if( iWhat < MOVEITEM_MENU_CANCEL )
		{
			pSoldier->bOldAssignment = pSoldier->bAssignment;

			if( pSoldier->bAssignment != MOVE_EQUIPMENT )
			{
				SetTimeOfAssignmentChangeForMerc( pSoldier );
			}

			if( pSoldier->bOldAssignment == VEHICLE )
			{
				TakeSoldierOutOfVehicle( pSoldier );
			}

			// remove from squad
			RemoveCharacterFromSquads( pSoldier );

			ChangeSoldiersAssignment( pSoldier, MOVE_EQUIPMENT );

			if ( iWhat < MOVEITEM_MAX_SECTORS )
				pSoldier->usItemMoveSectorID = usMoveItemSectors[iWhat];

			// assign to a movement group
			AssignMercToAMovementGroup( pSoldier );

			// set assignment for group
			SetAssignmentForList( ( INT8 ) MOVE_EQUIPMENT, pSoldier->usItemMoveSectorID );
			fShowAssignmentMenu = FALSE;
		}
		else
		{
			// CANCEL
			fShowMoveItemMenu = FALSE;
		}

		// update mapscreen
		fCharacterInfoPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		giAssignHighLine = -1;
	}
}


void MoveItemMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for assignment region
	INT32 iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( iValue < MOVEITEM_MENU_CANCEL )
		{
			if( GetBoxShadeFlag( ghMoveItemBox, iValue ) == FALSE )
			{
				// highlight choice
				HighLightBoxLine( ghMoveItemBox, iValue );
			}
		}
		else
		{
			// highlight cancel line
			HighLightBoxLine( ghMoveItemBox, GetNumberOfLinesOfTextInBox( ghMoveItemBox ) - 1 );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghMoveItemBox );
	}
}

BOOLEAN MercStaffsMilitaryHQ()
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiCnt = 0;
	UINT32 firstid = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	UINT32 lastid  = gTacticalStatus.Team[ OUR_TEAM ].bLastID;
	for ( uiCnt = firstid, pSoldier = MercPtrs[ uiCnt ]; uiCnt <= lastid; ++uiCnt, ++pSoldier)
	{
		if( pSoldier && pSoldier->bAssignment == FACILITY_STRATEGIC_MILITIA_MOVEMENT && pSoldier->flags.fMercAsleep == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}
