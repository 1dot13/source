#ifndef _VEHICLES_H
#define _VEHICLES_H

#include "Types.h"

#include "Soldier Control.h"
#include "Strategic Movement.h"
#include "soldier profile type.h"

#define	MAX_VEHICLES	10
extern std::vector<INT8>	gubVehicleMovementGroups;

#define MAX_MAIN_VEHICLE_CHARS 500
typedef struct
{
	UINT8 uiIndex;
	INT32 iNewMvtTypes;
	INT32 iNewSeatingCapacities;
	INT32 iNewEnterVehicleSndID;
	INT32 iNewMoveVehicleSndID;
	UINT8 ubNewVehicleTypeProfileID;
	INT16 sNewVehicleArmourType;
	INT8 NewVehicleBodyType;
	BOOLEAN bNewNeutral;
	BOOLEAN bNewVehicle;
	INT8 NewPilot;
	CHAR8	szIconFace[MAX_MAIN_VEHICLE_CHARS];
	CHAR16	NewVehicleStrings[128];
	CHAR16	NewVehicleName[128];
	CHAR16	NewShortVehicleStrings[128];
	INT32 NewCarPortraits;
	BOOLEAN NewUsed;
	
} NEW_CAR;

extern NEW_CAR gNewVehicle[NUM_PROFILES];

extern BOOLEAN SaveNewVehiclesToSaveGameFile( HWFILE hFile );
extern BOOLEAN LoadNewVehiclesToSaveGameFile( HWFILE hFile );

// type of vehicles
enum{
/*	NEW_VEHICLE1 = 0,
	NEW_VEHICLE2,
	NEW_VEHICLE3,
	NEW_VEHICLE4,
	NEW_VEHICLE5,
	NEW_VEHICLE6,
	NEW_VEHICLE7,
	NEW_VEHICLE8,
	NEW_VEHICLE9,
	NEW_VEHICLE10,
	NEW_VEHICLE11,
	NEW_VEHICLE12,
	NEW_VEHICLE13,
	NEW_VEHICLE14,
	NEW_VEHICLE15,
	NEW_VEHICLE16,
	NEW_VEHICLE17,
	NEW_VEHICLE18,
	NEW_VEHICLE19,
	NEW_VEHICLE20,	
	NEW_VEHICLE21,
	NEW_VEHICLE22,
	NEW_VEHICLE23,
	NEW_VEHICLE24,
	NEW_VEHICLE25,
	NEW_VEHICLE26,
	NEW_VEHICLE27,
	NEW_VEHICLE28,
	NEW_VEHICLE29,
	NEW_VEHICLE30,
	NEW_VEHICLE31,
	NEW_VEHICLE32,
	NEW_VEHICLE33,
	NEW_VEHICLE34,
	NEW_VEHICLE35,
	NEW_VEHICLE36,
	NEW_VEHICLE37,
	NEW_VEHICLE38,
	NEW_VEHICLE39,
	NEW_VEHICLE40,
	NEW_VEHICLE41,
	NEW_VEHICLE42,
	NEW_VEHICLE43,
	NEW_VEHICLE44,
	NEW_VEHICLE45,
	NEW_VEHICLE46,
	NEW_VEHICLE47,
	NEW_VEHICLE48,
	NEW_VEHICLE49,
	NEW_VEHICLE50,	
	NEW_VEHICLE51,
	NEW_VEHICLE52,
	NEW_VEHICLE53,
	NEW_VEHICLE54,
	NEW_VEHICLE55,
	NEW_VEHICLE56,
	NEW_VEHICLE57,
	NEW_VEHICLE58,
	NEW_VEHICLE59,
	NEW_VEHICLE60,
	NEW_VEHICLE61,
	NEW_VEHICLE62,
	NEW_VEHICLE63,
	NEW_VEHICLE64,
	NEW_VEHICLE65,
	NEW_VEHICLE66,
	NEW_VEHICLE67,
	NEW_VEHICLE68,
	NEW_VEHICLE69,
	NEW_VEHICLE70,	
	NEW_VEHICLE71,
	NEW_VEHICLE72,
	NEW_VEHICLE73,
	NEW_VEHICLE74,
	NEW_VEHICLE75,
	NEW_VEHICLE76,
	NEW_VEHICLE77,
	NEW_VEHICLE78,
	NEW_VEHICLE79,
	NEW_VEHICLE80,
	NEW_VEHICLE81,
	NEW_VEHICLE82,
	NEW_VEHICLE83,
	NEW_VEHICLE84,
	NEW_VEHICLE85,
	NEW_VEHICLE86,
	NEW_VEHICLE87,
	NEW_VEHICLE88,
	NEW_VEHICLE89,
	NEW_VEHICLE90,
	NEW_VEHICLE91,
	NEW_VEHICLE92,
	NEW_VEHICLE93,
	NEW_VEHICLE94,
	NEW_VEHICLE95,
	NEW_VEHICLE96,
	NEW_VEHICLE97,
	NEW_VEHICLE98,
	NEW_VEHICLE99,
	NEW_VEHICLE100,
	NEW_VEHICLE101,
	NEW_VEHICLE102,
	NEW_VEHICLE103,
	NEW_VEHICLE104,
	NEW_VEHICLE105,
	NEW_VEHICLE106,
	NEW_VEHICLE107,
	NEW_VEHICLE108,
	NEW_VEHICLE109,
	NEW_VEHICLE110,
	NEW_VEHICLE111,
	NEW_VEHICLE112,
	NEW_VEHICLE113,
	NEW_VEHICLE114,
	NEW_VEHICLE115,
	NEW_VEHICLE116,
	NEW_VEHICLE117,
	NEW_VEHICLE118,
	NEW_VEHICLE119,
	NEW_VEHICLE120,	
	NEW_VEHICLE121,
	NEW_VEHICLE122,
	NEW_VEHICLE123,
	NEW_VEHICLE124,
	NEW_VEHICLE125,
	NEW_VEHICLE126,
	NEW_VEHICLE127,
	NEW_VEHICLE128,
	NEW_VEHICLE129,
	NEW_VEHICLE130,		
	NEW_VEHICLE131,
	NEW_VEHICLE132,
	NEW_VEHICLE133,
	NEW_VEHICLE134,
	NEW_VEHICLE135,
	NEW_VEHICLE136,
	NEW_VEHICLE137,
	NEW_VEHICLE138,
	NEW_VEHICLE139,
	NEW_VEHICLE140,	
	NEW_VEHICLE141,
	NEW_VEHICLE142,
	NEW_VEHICLE143,
	NEW_VEHICLE144,
	NEW_VEHICLE145,
	NEW_VEHICLE146,
	NEW_VEHICLE147,
	NEW_VEHICLE148,
	NEW_VEHICLE149,
	NEW_VEHICLE150,	
	NEW_VEHICLE151,
	NEW_VEHICLE152,
	NEW_VEHICLE153,
	NEW_VEHICLE154,
	NEW_VEHICLE155,
	NEW_VEHICLE156,
	NEW_VEHICLE157,
	NEW_VEHICLE158,
	NEW_VEHICLE159,
	*/
	NEW_VEHICLE1 = 0,
	NEW_VEHICLE2 = NEW_VEHICLE1 + 159,
	HUMMER = 160,	
	ELDORADO_CAR = 161,
	ICE_CREAM_TRUCK = 162,
	HELICOPTER = 163,
	//JEEP_CAR = 164,
	TANK_CAR = 164,
	NEW_VEHICLE165 = 165,
	NEW_VEHICLE166 = 166,
	NEW_VEHICLE167 = 167,
	NEW_VEHICLE168 = 168,
	NEW_VEHICLE169 = 169,	
	NUMBER_OF_TYPES_OF_VEHICLES = NEW_VEHICLE169 + 84, 
	
	/*
	ELDORADO_CAR = 0,
	HUMMER,
	ICE_CREAM_TRUCK,
	JEEP_CAR,
	TANK_CAR,
	HELICOPTER,
	
	NUMBER_OF_TYPES_OF_VEHICLES, 
	*/
};

// external armor hit locations
enum{
	FRONT_EXTERNAL_HIT_LOCATION,
	LEFT_EXTERNAL_HIT_LOCATION,
	RIGHT_EXTERNAL_HIT_LOCATION,
	REAR_EXTERNAL_HIT_LOCATION,
	BOTTOM_EXTERNAL_HIT_LOCATION,
	TOP_EXTERNAL_HIT_LOCATION,
	NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE,
};

// internal critical hit locations
enum{
	ENGINE_HIT_LOCATION,
	CREW_COMPARTMENT_HIT_LOCATION,
	RF_TIRE_HIT_LOCATION,
	LF_TIRE_HIT_LOCATION,
	RR_TIRE_HIT_LOCATION,
	LR_TIRE_HIT_LOCATION,
	GAS_TANK_HIT_LOCATION,
	NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE,
};

//extern STR16 sCritLocationStrings[];

//extern INT8 bInternalCritHitsByLocation[NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE][ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE ];

extern INT16 sVehicleOrigArmorValues[ NUMBER_OF_TYPES_OF_VEHICLES][ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE];

#define MAXPASSENGERS 10
// struct for vehicles
typedef struct
{
 PathStPtr pMercPath;	// vehicle's stategic path list
 UINT8	 ubMovementGroup; // the movement group this vehicle belongs to
 UINT8	 ubVehicleType; // type of vehicle 
 INT16	 sSectorX;	// X position on the Stategic Map
 INT16	 sSectorY;	// Y position on the Stategic Map
 INT16	 sSectorZ;
 BOOLEAN fBetweenSectors;	// between sectors?
 INT32 sGridNo;   // location in tactical
 SOLDIERTYPE *pPassengers[ MAXPASSENGERS ];
 UINT8		ubDriver;
 INT16		sInternalHitLocations[ NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE ];
 INT16		sArmourType;
 INT16		sExternalArmorLocationsStatus[ NUMBER_OF_EXTERNAL_HIT_LOCATIONS_ON_VEHICLE ];
 INT16		sCriticalHits[ NUMBER_OF_INTERNAL_HIT_LOCATIONS_IN_VEHICLE ];
 INT32		iOnSound;
 INT32		iOffSound;
 INT32		iMoveSound;
 INT32		iOutOfSound;
 BOOLEAN	fFunctional;
 BOOLEAN	fDestroyed;
 INT32		iMovementSoundID;
 UINT8		ubProfileID;

 BOOLEAN fValid;

} VEHICLETYPE;



// the list of vehicles
extern VEHICLETYPE *pVehicleList;

// number of vehicles on the list
extern UINT8 ubNumberOfVehicles;

extern INT32 iMvtTypes[];

void SetVehicleValuesIntoSoldierType( SOLDIERTYPE *pVehicle );

// add vehicle to list and return id value
INT32 AddVehicleToList( INT16 sMapX, INT16 sMapY, INT32 sGridNo, UINT8 ubType );

// remove this vehicle from the list
BOOLEAN RemoveVehicleFromList( INT32 iId );

// clear out the vehicle list
void ClearOutVehicleList( void );

BOOLEAN AnyAccessibleVehiclesInSoldiersSector( SOLDIERTYPE *pSoldier );

// is this vehicle in the same sector (not between sectors), and accesible
BOOLEAN IsThisVehicleAccessibleToSoldier( SOLDIERTYPE *pSoldier, INT32 iId );

// add soldier to Vehicle
BOOLEAN AddSoldierToVehicle( SOLDIERTYPE *pSoldier, INT32 iId );

// remove soldier from vehicle
BOOLEAN RemoveSoldierFromVehicle( SOLDIERTYPE *pSoldier, INT32 iId );

// strategic mvt stuff
// move character path to the vehicle
BOOLEAN MoveCharactersPathToVehicle( SOLDIERTYPE *pSoldier );

// travel time at the startegic level
INT32 GetTravelTimeOfVehicle( INT32 iId );

// is this vehicle a valid one?
BOOLEAN VehicleIdIsValid( INT32 iId );

// Test whether a soldier is actually a vehicle
BOOLEAN IsVehicle(SOLDIERTYPE *pSoldier);

// set up vehicle mvt for this grunt involved
BOOLEAN SetUpMvtGroupForVehicle( SOLDIERTYPE *pSoldier );

// set up soldier mvt for vehicle
BOOLEAN CopyVehiclePathToSoldier( SOLDIERTYPE *pSoldier );

// update mercs position when vehicle arrives
void UpdatePositionOfMercsInVehicle( INT32 iId );

// find vehicle id of group with this vehicle
INT32 GivenMvtGroupIdFindVehicleId( UINT8 ubGroupId );

// given vehicle id, add all peopel in vehicle to mvt group, after clearing mvt group out
BOOLEAN AddVehicleMembersToMvtGroup( INT32 iId );

// injure this person in the vehicle
BOOLEAN InjurePersonInVehicle( INT32 iId, SOLDIERTYPE *pSoldier, UINT8 ubPointsOfDmg );

// kill this person in the vehicle
BOOLEAN KillPersonInVehicle( INT32 iId, SOLDIERTYPE *pSoldier );

// kill everyone in vehicle
BOOLEAN KillAllInVehicle( INT32 iId );

#ifdef JA2TESTVERSION
// test vehicle stuff
void VehicleTest( void );
#endif

// grab number of occupants in vehicles
INT32 GetNumberInVehicle( INT32 iId );

// grab # in vehicle skipping EPCs (who aren't allowed to drive :-)
INT32 GetNumberOfNonEPCsInVehicle( INT32 iId );

BOOLEAN EnterVehicle( SOLDIERTYPE *pVehicle, SOLDIERTYPE *pSoldier );

SOLDIERTYPE *GetDriver( INT32 iID );

void SetVehicleName( SOLDIERTYPE *pVehicle );
	
BOOLEAN ExitVehicle( SOLDIERTYPE *pSoldier );

void AddPassangersToTeamPanel( INT32 iId );


void VehicleTakeDamage( UINT8 ubID, UINT8 ubReason, INT16 sDamage, INT32 sGridNo , UINT8 ubAttackerID );

// the soldiertype containing this tactical incarnation of this vehicle
SOLDIERTYPE * GetSoldierStructureForVehicle( INT32 iId );

void AdjustVehicleAPs( SOLDIERTYPE *pSoldier, INT16 *pubPoints );

// get orig armor values for vehicle in this location
//INT16 GetOrigInternalArmorValueForVehicleInLocation( UINT8 ubID, UINT8 ubLocation );

// handle crit hit to vehicle in this location
void HandleCriticalHitForVehicleInLocation( UINT8 ubID, INT16 sDmg, INT32 sGridNo, UINT8 ubAttackerID );

// ste up armor values for this vehicle
void SetUpArmorForVehicle( UINT8 ubID );

// does it need fixing?
BOOLEAN DoesVehicleNeedAnyRepairs( INT32 iVehicleId );

// repair the vehicle
INT8 RepairVehicle( INT32 iVehicleId, INT8 bTotalPts, BOOLEAN *pfNothingToRepair );


//Save all the vehicle information to the saved game file
BOOLEAN SaveVehicleInformationToSaveGameFile( HWFILE hFile );

//Load all the vehicle information From the saved game file
BOOLEAN LoadVehicleInformationFromSavedGameFile( HWFILE hFile, UINT32 uiSavedGameVersion );

// take soldier out of vehicle
BOOLEAN TakeSoldierOutOfVehicle( SOLDIERTYPE *pSoldier );

// put soldier in vehicle
BOOLEAN PutSoldierInVehicle( SOLDIERTYPE *pSoldier, INT8 bVehicleId );

void SetVehicleSectorValues( INT32 iVehId, UINT8 ubSectorX, UINT8 ubSectorY );

void UpdateAllVehiclePassengersGridNo( SOLDIERTYPE *pSoldier );

BOOLEAN SaveVehicleMovementInfoToSavedGameFile( HWFILE hFile );
BOOLEAN LoadVehicleMovementInfoFromSavedGameFile( HWFILE hFile );
BOOLEAN NewSaveVehicleMovementInfoToSavedGameFile( HWFILE hFile );
BOOLEAN NewLoadVehicleMovementInfoFromSavedGameFile( HWFILE hFile );

BOOLEAN OKUseVehicle( UINT8 ubProfile );

BOOLEAN IsRobotControllerInVehicle( INT32 iId );

void AddVehicleFuelToSave( );

BOOLEAN CanSoldierDriveVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId, BOOLEAN fIgnoreAsleep ); 
BOOLEAN SoldierMustDriveVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId, BOOLEAN fTryingToTravel );
BOOLEAN OnlythisCanDriveVehicle( SOLDIERTYPE *pSoldier, INT32 iVehicleId );

BOOLEAN IsEnoughSpaceInVehicle( INT32 iID );

BOOLEAN IsSoldierInThisVehicleSquad( SOLDIERTYPE *pSoldier, INT8 bSquadNumber );

SOLDIERTYPE*	PickRandomPassengerFromVehicle( SOLDIERTYPE *pSoldier );

BOOLEAN DoesVehicleHaveAnyPassengers( INT32 iVehicleID );
BOOLEAN DoesVehicleGroupHaveAnyPassengers( GROUP *pGroup );

void SetSoldierExitVehicleInsertionData( SOLDIERTYPE *pSoldier, INT32 iId, UINT8 iOldGroupID );

void InitAVehicle(int index, int x, int y);

void InitAllVehicles( );


#endif
