#ifndef _ASSIGNMENTS_H
#define _ASSIGNMENTS_H

// header for assignment manipulation/updating for characters

#include "Types.h"
#include "Soldier Control.h"
#include "mousesystem.h"
#include "Strategic Movement.h"


// this distinguishes whether we're only looking for patients healable THIS HOUR (those that have been on their assignment
// long enough), or those that will be healable EVER (regardless of whether they're getting healed during this hour)
#define HEALABLE_EVER				0
#define HEALABLE_THIS_HOUR	1


// merc collapses from fatigue if max breath drops to this.	Can't go any lower!
#define BREATHMAX_ABSOLUTE_MINIMUM	10
#define BREATHMAX_GOTTA_STOP_MOVING	30
#define BREATHMAX_PRETTY_TIRED			50
#define BREATHMAX_CANCEL_COLLAPSE		60
#define BREATHMAX_CANCEL_TIRED			75
#define BREATHMAX_FULLY_RESTED			95


#define	VEHICLE_REPAIR_POINTS_DIVISOR		10

// controls how easily SAM sites are repaired
// NOTE: A repairman must generate a least this many points / hour to be ABLE to repair a SAM site at all!
#define SAM_SITE_REPAIR_DIVISOR		4			// Flugente: changed from 10 to 4

// minimum condition at which a destroyed SAM site controller is visible in tactical again
#define MIN_CONDITION_SHOW_SAM_CONTROLLER 20

// Assignments Defines
enum
{
	SQUAD_1 = 0,
	SQUAD_2,
	SQUAD_3,
	SQUAD_4,
	SQUAD_5,
	SQUAD_6,
	SQUAD_7,
	SQUAD_8,
	SQUAD_9,
	SQUAD_10,
	SQUAD_11,
	SQUAD_12,
	SQUAD_13,
	SQUAD_14,
	SQUAD_15,
	SQUAD_16,
	SQUAD_17,
	SQUAD_18,
	SQUAD_19,
	SQUAD_20,
	ON_DUTY,
	DOCTOR,
	PATIENT,
	VEHICLE,
	IN_TRANSIT,
	REPAIR,
	RADIO_SCAN,						// added by Flugente: a radio operators scans for enemy patrols in nearby sectors
	TRAIN_SELF,
	TRAIN_TOWN,
	TRAIN_MOBILE,					// HEADROCK HAM 3.6: Training mobile militia.
	TRAIN_TEAMMATE,
	TRAIN_BY_OTHER,
	MOVE_EQUIPMENT,					// added by Flugente: move items from one city sector to another
	FACILITY_STAFF,					// HEADROCK HAM 3.6: Operating a facility for strategic gain.
	FACILITY_EAT,					// added by Flugente
	FACILITY_REST,					// HEADROCK HAM 3.6: Facility equivalent of resting (no assignment)
	FACILITY_INTERROGATE_PRISONERS,	// added by Flugente
	ASSIGNMENT_DEAD,
	ASSIGNMENT_UNCONCIOUS,			// unused
	ASSIGNMENT_POW,
	ASSIGNMENT_HOSPITAL,
	ASSIGNMENT_EMPTY,
	FACILITY_PRISON_SNITCH,
	FACILITY_SPREAD_PROPAGANDA,
	FACILITY_SPREAD_PROPAGANDA_GLOBAL,
	FACILITY_GATHER_RUMOURS,
	SNITCH_SPREAD_PROPAGANDA,
	SNITCH_GATHER_RUMOURS,
	FACILITY_STRATEGIC_MILITIA_MOVEMENT,	// Flugente: merc staffs a military HQ, we can now control militia movement on the map
	DISEASE_DIAGNOSE,				// Flugente: check for diseases among mercs and local population
	DISEASE_DOCTOR_SECTOR,			// Flugente: treatpopulation (all non-mercs in the sector) against disease
	FACILITY_DOCTOR,
	FACILITY_PATIENT,
	FACILITY_REPAIR,
	FORTIFICATION,					// Flugente: sectors can be fortified according to external layout plans
	TRAIN_WORKERS,
	CONCEALED,						// Flugente: spy hides among the population
	GATHERINTEL,					// gathers information while disguised in enemy territory
	NUM_ASSIGNMENTS,
};

#define IS_DOCTOR(assignment) ((assignment == DOCTOR) || (assignment == FACILITY_DOCTOR))
// left the last : off so syntax is visually consistent with non-macro case statements
#define CASE_DOCTOR case DOCTOR: case FACILITY_DOCTOR

// Does not include HOSPITAL since mercs are not responsible for their care
#define IS_PATIENT(assignment) ((assignment == DOCTOR) || (assignment == PATIENT) || (assignment == FACILITY_DOCTOR) || (assignment == FACILITY_PATIENT))
// Does not include DOCTOR types because switch cases must be unique
#define CASE_PATIENT case PATIENT: case FACILITY_PATIENT

#define IS_REPAIR(assignment) ((assignment == REPAIR) || (assignment == FACILITY_REPAIR))
#define CASE_REPAIR case REPAIR: case FACILITY_REPAIR

#define SPY_LOCATION(assignment) ((assignment == CONCEALED) || (assignment == GATHERINTEL) )

// strings for snitch exposition
enum
{
	SNITCH_PRISON_EXPOSED_FINE_WISDOM,
	SNITCH_PRISON_EXPOSED_FINE_LEADERSHIP,
	SNITCH_PRISON_EXPOSED_FINE_EXPLEVEL,
	SNITCH_PRISON_EXPOSED_FINE_GUARDS,

	SNITCH_PRISON_EXPOSED_WOUNDED_DROWN,	
	SNITCH_PRISON_EXPOSED_WOUNDED_BEATEN,
	SNITCH_PRISON_EXPOSED_WOUNDED_KNIFED,
	SNITCH_PRISON_EXPOSED_WOUNDED_STRANGLED,

	SNITCH_PRISON_EXPOSED_DEAD_DROWN,	
	SNITCH_PRISON_EXPOSED_DEAD_BEATEN,
	SNITCH_PRISON_EXPOSED_DEAD_KNIFED,
	SNITCH_PRISON_EXPOSED_DEAD_STRANGLED,

	NUM_SNITCH_PRISON_EXPOSED,
};

// strings for snitch gathering rumours
enum
{
	SNITCH_GATHERING_RUMOURS_RESULT,
	NUM_SNITCH_GATHERING_RUMOURS_RESULT,
};

#define NO_ASSIGNMENT		127 //used when no pSoldier->ubDesiredSquad

// Train stats defines (must match ATTRIB_MENU_ defines, and pAttributeMenuStrings )
enum
{
	HEALTH = 0,
	AGILITY,
	DEXTERITY,
	STRENGTH,
	LEADERSHIP,
	MARKSMANSHIP,
	MECHANICAL,
	EXPLOSIVE_ASSIGN,
	MEDICAL,
	NUM_TRAINABLE_STATS
	// NOTE: Wisdom isn't trainable!
};


typedef struct TOWN_TRAINER_TYPE
{
	SOLDIERTYPE *pSoldier;
	INT16	sTrainingPts;

} TOWN_TRAINER_TYPE;


// can character do this assignment?
//BOOLEAN CanSoldierAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment );

// can this character be assigned as a doctor?
BOOLEAN CanCharacterDoctor( SOLDIERTYPE *pCharacter );

// can this character diagnose diseases?
BOOLEAN CanCharacterDiagnoseDisease( SOLDIERTYPE *pSoldier );

// can this character treat diseases of the population (NOT mercs)?
BOOLEAN CanCharacterTreatSectorDisease( SOLDIERTYPE *pSoldier );

BOOLEAN CanCharacterFortify( SOLDIERTYPE *pSoldier );

BOOLEAN CanCharacterSpyAssignment( SOLDIERTYPE *pSoldier );

// can this character be assigned as a repairman?
BOOLEAN CanCharacterRepair( SOLDIERTYPE *pCharacter );

// can character be patient?
BOOLEAN CanCharacterPatient( SOLDIERTYPE *pCharacter );

// can character train militia?
BOOLEAN CanCharacterTrainMilitia( SOLDIERTYPE *pCharacter );
BOOLEAN CanCharacterTrainMobileMilitia( SOLDIERTYPE *pSoldier );

BOOLEAN CanCharacterTrainWorkers( SOLDIERTYPE *pSoldier );

// can character train stat?..as train self or as trainer?
BOOLEAN CanCharacterTrainStat( SOLDIERTYPE *pSoldier, INT8 bStat, BOOLEAN fTrainSelf, BOOLEAN fTrainTeammate );

// is character capable of practising at all?
BOOLEAN CanCharacterPractise( SOLDIERTYPE *pCharacter );

// can this character train others?
BOOLEAN CanCharacterTrainTeammates( SOLDIERTYPE *pSoldier );
BOOLEAN CanCharacterFacility( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType );

// put character on duty?
BOOLEAN CanCharacterOnDuty( SOLDIERTYPE *pCharacter );

// put character to sleep?
BOOLEAN CanCharacterSleep( SOLDIERTYPE *pCharacter, BOOLEAN fExplainWhyNot );

BOOLEAN CanCharacterBeAwakened( SOLDIERTYPE *pSoldier, BOOLEAN fExplainWhyNot );

// put character in vehicle?
BOOLEAN CanCharacterVehicle( SOLDIERTYPE *pCharacter );

#define CHARACTER_CANT_JOIN_SQUAD_ALREADY_IN_IT -6
#define CHARACTER_CANT_JOIN_SQUAD_SQUAD_MOVING -5
#define CHARACTER_CANT_JOIN_SQUAD_MOVING -4
#define CHARACTER_CANT_JOIN_SQUAD_VEHICLE -3
#define CHARACTER_CANT_JOIN_SQUAD_TOO_FAR -2
#define CHARACTER_CANT_JOIN_SQUAD_FULL -1
#define CHARACTER_CANT_JOIN_SQUAD 0
#define CHARACTER_CAN_JOIN_SQUAD 1

// can character be added to squad
INT8 CanCharacterSquad( SOLDIERTYPE *pCharacter, INT8 bSquadValue );

// can character snitch
BOOLEAN CanCharacterSnitch( SOLDIERTYPE *pCharacter );

// if merc could train militia here, do they have sufficient loyalty?
BOOLEAN DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( SOLDIERTYPE *pSoldier );
BOOLEAN DoesTownHaveRatingToTrainMilitia( INT8 bTownId );

// is the character in transit?
BOOLEAN IsCharacterInTransit( SOLDIERTYPE *pCharacter );

// handler for assignments -- called once per hour via event
void UpdateAssignments();

// how many people in this secotr have this assignment?
UINT8 FindNumberInSectorWithAssignment( INT16 sX, INT16 sY, INT8 bAssignment );

void MakeSoldiersTacticalAnimationReflectAssignment( SOLDIERTYPE *pSoldier );

// build list of sectors with mercs
void BuildSectorsWithSoldiersList( void );

// init sectors with soldiers list
void InitSectorsWithSoldiersList( void );

// is there a soldier in this sector?..only use after BuildSectorsWithSoldiersList is called
BOOLEAN IsThereASoldierInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void CheckIfSoldierUnassigned( SOLDIERTYPE *pSoldier );

// figure out the assignment menu pop up box positions
void DetermineBoxPositions( void );

// set x,y position in tactical 
void SetTacticalPopUpAssignmentBoxXY( void );

// get number of pts that are being used this strategic turn
INT16 GetTownTrainPtsForCharacter( SOLDIERTYPE *pTrainer, UINT16 *pusMaxPts );


// find number of healing pts 
UINT16 CalculateHealingPointsForDoctor(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts, BOOLEAN fMakeSureKitIsInHand );

// find number of repair pts repairman has available
UINT8 CalculateRepairPointsForRepairman(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts, BOOLEAN fMakeSureKitIsInHand );

// find number of cleaning pts repairman has available
UINT8 CalculateCleaningPointsForRepairman(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts );

// Flugente: calculate interrogation and prison guard values
UINT32 CalculateInterrogationValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts );
UINT32 CalculatePrisonGuardValue(SOLDIERTYPE *pSoldier );

UINT32 CalculateSnitchInterrogationValue(SOLDIERTYPE *pSoldier, UINT16 *pusMaxPts );

// Flugente: Determine the best cth with SAMs in a sector, and which merc has that cth if present
FLOAT GetBestSAMOperatorCTH_Player( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, UINT16 *pubID );

INT16 GetTrainWorkerPts(SOLDIERTYPE *pSoldier);

// Flugente: intel shenanigans
void BuildIntelInfoArray();
void CalcIntelInfoOfferings();
void GetIntelInfoOfferings( int aInfo[] );
void GetIntelInfoTextAndPrice( int aInfoNumber, STR16 aString, int& arIntelCost );
void BuyIntelInfo( int aInfoNumber );

// get bonus tarining pts due to an instructor for this student
// HEADROCK HAM 3.5: Three functions below have lost an argument which is no longer required ("uiAtGunRange", which was "uiAtFacility" in HAM 3.4)
INT16 GetBonusTrainingPtsDueToInstructor( SOLDIERTYPE *pInstructor, SOLDIERTYPE *pStudent, INT8 bTrainStat, UINT16 *pusMaxPts );

// get training pts for this soldier
INT16 GetSoldierTrainingPts( SOLDIERTYPE *pSoldier, INT8 bTrainStat, UINT16 *pusMaxPts );

// pts for being a student for this soldier
INT16 GetSoldierStudentPts( SOLDIERTYPE *pSoldier, INT8 bTrainStat, UINT16 *pusMaxPts );

// HEADROCK HAM 3.6: Completely unused.
// reset these soldiers
//void ResetAssignmentsForAllSoldiersInSectorWhoAreTrainingTown( SOLDIERTYPE *pSoldier );

// Handle assignment done
void AssignmentDone( SOLDIERTYPE *pSoldier, BOOLEAN fSayQuote, BOOLEAN fMeToo );

extern INT32 ghAssignmentBox;
extern INT32 ghEpcBox;
extern INT32 ghSquadBox;
extern INT32 ghVehicleBox;
extern INT32 ghRepairBox;
extern INT32 ghTrainingBox;
extern INT32 ghMoveItemBox;
extern INT32 ghDiseaseBox;
extern INT32 ghSpyBox;
extern INT32 ghAttributeBox;
extern INT32 ghRemoveMercAssignBox;
extern INT32 ghContractBox;
extern INT32 ghMoveBox;
// HEADROCK HAM 3.6: Facility Menu, Submenu
extern INT32 ghFacilityBox;
extern INT32 ghFacilityAssignmentBox;
//extern INT32 ghUpdateBox;
// anv: snitch menus
extern INT32 ghSnitchBox;
extern INT32 ghSnitchToggleBox;
extern INT32 ghSnitchSectorBox;

// Flugente: prisoner menu
extern INT32 ghPrisonerBox;


extern MOUSE_REGION	gAssignmentScreenMaskRegion;

extern MOUSE_REGION	gAssignmentMenuRegion[	];
extern MOUSE_REGION	gTrainingMenuRegion[	];
extern MOUSE_REGION	gAttributeMenuRegion[	];
extern MOUSE_REGION	gSquadMenuRegion[	];
extern MOUSE_REGION	gContractMenuRegion[	];
extern MOUSE_REGION	gRemoveMercAssignRegion[	];
extern MOUSE_REGION		gVehicleMenuRegion[];
// HEADROCK HAM 3.6: Facility Menu.
extern MOUSE_REGION gFacilityMenuRegion[];
extern MOUSE_REGION gFacilityMenuRegion[];

extern BOOLEAN fShownContractMenu;
extern BOOLEAN fShownAssignmentMenu;
extern BOOLEAN fShowRepairMenu;
extern BOOLEAN fShowMoveItemMenu;
extern BOOLEAN fShowDiseaseMenu;
extern BOOLEAN fShowSpyMenu;

extern BOOLEAN fFirstClickInAssignmentScreenMask;

extern void RestorePopUpBoxes( void );

extern BOOLEAN fGlowContractRegion;

extern BOOLEAN gfReEvaluateEveryonesNothingToDo;


// pop up menu mouse regions
void CreateDestroyMouseRegionsForAssignmentMenu( void );
void AssignmentMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void AssignmentMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

void CreateDestroyMouseRegionsForTrainingMenu( void );
void TrainingMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void TrainingMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

void CreateDestroyMouseRegionsForAttributeMenu( void );
void AttributeMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void AttributesMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

void CreateDestroyMouseRegionsForSquadMenu( BOOLEAN fPositionBox );
void SquadMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SquadMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// vehicle menu 
void CreateDestroyMouseRegionForVehicleMenu( void );
void VehicleMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void VehicleMenuBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );

// repair menu
void CreateDestroyMouseRegionForRepairMenu( void );
void RepairMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void RepairMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// Flugente: move item menu
void CreateDestroyMouseRegionForMoveItemMenu( void );
void MoveItemMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void MoveItemMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// Flugente: disease menu
void CreateDestroyMouseRegionForDiseaseMenu( void );
void DiseaseMenuMvtCallback( MOUSE_REGION * pRegion, INT32 iReason );
void DiseaseMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// Flugente: spy menu
void CreateDestroyMouseRegionForSpyMenu( void );
void SpyMenuMvtCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SpyMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// HEADROCK HAM 3.6: Facility Menu
void CreateDestroyMouseRegionForFacilityMenu( void );
void FacilityMenuMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void FacilityMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// HEADROCK HAM 3.6: Facility sub-menu
void CreateDestroyMouseRegionsForFacilityAssignmentMenu( void );
void FacilityAssignmentMenuMvtCallBack( MOUSE_REGION * pRegion, INT32 iReason );
void FacilityAssignmentMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// anv: snitch menus
void CreateDestroyMouseRegionsForSnitchMenu( void );
void SnitchMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SnitchMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CreateDestroyMouseRegionsForSnitchToggleMenu( void );
void SnitchToggleMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SnitchToggleMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CreateDestroyMouseRegionsForSnitchSectorMenu( void );
void SnitchSectorMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SnitchSectorMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// Flugente: prisoner menu
void CreateDestroyMouseRegionsForPrisonerMenu();
void PrisonerMenuMvtCallBack( MOUSE_REGION * pRegion, INT32 iReason );
void PrisonerMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );

// contract menu
void CreateDestroyMouseRegionsForContractMenu( void );
void ContractMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
void ContractMenuMvtCallback( MOUSE_REGION * pRegion, INT32 iReason );
void RebuildContractBoxForMerc( SOLDIERTYPE *pCharacter );

// remove merc from team menu callback
void RemoveMercMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
void RemoveMercMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void CreateDestroyMouseRegionsForRemoveMenu( void );

// misc assignment GUI functions
void HandleShadingOfLinesForAssignmentMenus( void );
BOOLEAN IsCharacterAliveAndConscious( SOLDIERTYPE *pCharacter );
void CreateDestroyScreenMaskForAssignmentAndContractMenus( void );


BOOLEAN CreateDestroyAssignmentPopUpBoxes( void );
void SetSoldierAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment, INT32 iParam1, INT32 iParam2, INT32 iParam3 );

// set merc asleep and awake under the new sleep system implemented June 29, 1998
// if give warning is false, the function can be used as an internal function
BOOLEAN SetMercAwake( SOLDIERTYPE *pSoldier, BOOLEAN fGiveWarning, BOOLEAN fForceHim );
BOOLEAN SetMercAsleep( SOLDIERTYPE *pSoldier, BOOLEAN fGiveWarning );
BOOLEAN PutMercInAsleepState( SOLDIERTYPE *pSoldier );
BOOLEAN PutMercInAwakeState( SOLDIERTYPE *pSoldier );

BOOLEAN AssignMercToAMovementGroup( SOLDIERTYPE *pSoldier );

// set what time this merc undertook this assignment 
void SetTimeOfAssignmentChangeForMerc( SOLDIERTYPE *pSoldier );

// enough time on assignment for it to count?
BOOLEAN EnoughTimeOnAssignment( SOLDIERTYPE *pSoldier );

// check if any merc in group is too tired to keep moving
BOOLEAN AnyMercInGroupCantContinueMoving( GROUP *pGroup );

// handle selected group of mercs being put to sleep
BOOLEAN HandleSelectedMercsBeingPutAsleep( BOOLEAN fWakeUp, BOOLEAN fDisplayWarning );

// is any one on the team on this assignment?
BOOLEAN IsAnyOneOnPlayersTeamOnThisAssignment( INT8 bAssignment );

// rebuild assignments box
void RebuildAssignmentsBox( void );

void BandageBleedingDyingPatientsBeingTreated( );

void ReEvaluateEveryonesNothingToDo(BOOLEAN aDoExtensiveCheck = FALSE);

// set assignment for list of characters
void SetAssignmentForList( INT8 bAssignment, INT8 bParam );

// is this area maxed out on militia?
BOOLEAN IsMilitiaTrainableFromSoldiersSectorMaxed( SOLDIERTYPE *pSoldier, INT8 iMilitiaType );

// function where we actually set someone's assignment so we can trap certain situations
void ChangeSoldiersAssignment( SOLDIERTYPE *pSoldier, INT8 bAssignment );

void UnEscortEPC( SOLDIERTYPE *pSoldier );

SOLDIERTYPE *AnyDoctorWhoCanHealThisPatient( SOLDIERTYPE *pPatient, BOOLEAN fThisHour );

// HEADROCK HAM B2.8: A set of functions to synchronize sleeping periods of trainers and trainees
BOOLEAN SetTrainerSleepWhenTraineesSleep( SOLDIERTYPE *pTrainee);
BOOLEAN SetTraineesSleepWhenTrainerSleeps( SOLDIERTYPE *pTrainer );
BOOLEAN SetTrainerWakeWhenTraineesWake( SOLDIERTYPE *pTrainee );
BOOLEAN SetTraineesWakeWhenTrainerWakes( SOLDIERTYPE *pTrainer );
void HandleTrainingSleepSynchronize( SOLDIERTYPE *pSoldier );
void HandleTrainingWakeSynchronize( SOLDIERTYPE *pSoldier );
BOOLEAN FindAnyAwakeTrainers( SOLDIERTYPE *pTrainee );
BOOLEAN FindAnyAwakeTrainees( SOLDIERTYPE *pTrainer );

// HEADROCK HAM 3.6: Functions for testing and reporting reasons why character can't train here.
BOOLEAN CanCharacterTrainMilitiaWithErrorReport( SOLDIERTYPE *pSoldier );
BOOLEAN CanCharacterTrainMobileMilitiaWithErrorReport( SOLDIERTYPE *pSoldier );
// HEADROCK HAM 3.6: Function for testing and reporting reasons why character can't staff a facility.
BOOLEAN CanCharacterFacilityWithErrorReport( SOLDIERTYPE *pSoldier, UINT8 ubFacilityType, UINT8 ubAssignmentType );
void HandleInterfaceMessageForCostOfOperatingFacility( SOLDIERTYPE *pSoldier, UINT8 ubAssignmentType );
void PayFacilityCostsYesNoBoxCallback( UINT8 bExitValue );
void FacilityStaffingRejected( void );
void ResetAllExpensiveFacilityAssignments();
BOOLEAN IsOutstandingFacilityDebtWithErrorReport();
extern INT8 gubFacilityInSubmenu;
extern UINT8 gubFacilityLineForSubmenu;
extern SOLDIERTYPE *gpFacilityStaffer;

// SANDRO - function to award record points for militia training
void RecordNumMilitiaTrainedForMercs( INT16 sX, INT16 sY, INT8 sZ, UINT8 ubMilitiaTrained, BOOLEAN fMobile );

// anv: decrease town loyalty hits
UINT32 HandlePropagandaBlockingBadNewsInTown( INT8 bTownId, UINT32 uiLoyaltyDecrease );

void HandleGatheringInformationBySoldier( SOLDIERTYPE* pSoldier );

BOOLEAN MercStaffsMilitaryHQ();

BOOLEAN MakeSureMedKitIsInHand( SOLDIERTYPE *pSoldier );

UINT8 CalcSoldierNeedForSleep( SOLDIERTYPE *pSoldier );

#endif