#ifndef _TOWN_MILITIA_H
#define _TOWN_MILITIA_H

// header for town militia strategic control module

#include "Types.h"
#include "Soldier Control.h"


// how many militia of all ranks can be in any one sector at once
//#define MAX_ALLOWABLE_MILITIA_PER_SECTOR 20

// how many new green militia civilians are trained at a time
//#define MILITIA_TRAINING_SQUAD_SIZE		10		// was 6

// cost of starting a new militia training assignment
//#define MILITIA_TRAINING_COST		750

// minimum loyalty rating before training is allowed in a town
//#define MIN_RATING_TO_TRAIN_TOWN 20


// handle promoting a militia during militia training. return TRUE if militia could be promoted
BOOLEAN TownMilitiaTrainingPromotion( INT16 sMapX, INT16 sMapY );

// this handles what happens when a new militia unit is finishes getting trained
void TownMilitiaTrainingCompleted( SOLDIERTYPE *pTrainer, INT16 sMapX, INT16 sMapY );

// feed this a SOLDIER_CLASS_, it will return you a _MITILIA rank, or -1 if the guy's not militia
INT8 SoldierClassToMilitiaRank(UINT8 ubSoldierClass);
// feed this a _MITILIA rank, it will return you a SOLDIER_CLASS_, or -1 if the guy's not militia
INT8 MilitiaRankToSoldierClass(UINT8 ubRank);

// these add, promote, and remove militias of a certain rank
void StrategicAddMilitiaToSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany);
void StrategicPromoteMilitiaInSector(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank, UINT8 ubHowMany);
void StrategicRemoveMilitiaFromSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany);

void StrategicRemoveAllStaticMilitiaFromSector( INT16 sMapX, INT16 sMapY, UINT8 ubRank );

// this will check for promotions and handle them for you
UINT8 CheckOneMilitiaForPromotion(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank, UINT8 ubRecentKillPts);

void BuildMilitiaPromotionsString( STR16 str );

// HEADROCK HAM 3.3: Function to determine best leadership for militia training in a given sector.
// Is used for both town and mobile militia.
// HEADROCK HAM 3.6: Argument for Mobile/Town Militia training
UINT8 FindBestMilitiaTrainingLeadershipInSector ( INT16 sMapX, INT16 sMapY, INT8 usMapZ, UINT8 ubMilitiaType );

// call this if the player attacks his own militia
void HandleMilitiaDefections(INT16 sMapX, INT16 sMapY);

UINT8 MilitiaInSectorOfRank(INT16 sMapX, INT16 sMapY, UINT8 ubRank);
UINT8 MilitiaInSectorOfRankStationary( INT16 sMapX, INT16 sMapY, UINT8 ubRank );
UINT8 MilitiaInSectorOfRankInGroups( INT16 sMapX, INT16 sMapY, UINT8 ubRank );

// Returns TRUE if sector is under player control, has no enemies in it, and isn't currently in combat mode
BOOLEAN SectorOursAndPeaceful( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

// tell player how much it will cost 
void HandleInterfaceMessageForCostOfTrainingMilitia( SOLDIERTYPE *pSoldier );

// continue training?
void HandleInterfaceMessageForContinuingTrainingMilitia( SOLDIERTYPE *pSoldier );

// call this when the sector changes...
void HandleMilitiaStatusInCurrentMapBeforeLoadingNewMap( void );

// is there a town with militia here or nearby?
BOOLEAN CanSomeoneNearbyScoutThisSector( INT16 sSectorX, INT16 sSectorY, BOOLEAN fScoutTraitCheck ); // changed - SANDRO

// is the town militia full?
BOOLEAN IsTownFullMilitia( INT8 bTownId, INT8 iMilitiaType );
// is the SAM site full of militia?
BOOLEAN IsSAMSiteFullOfMilitia( INT16 sSectorX, INT16 sSectorY, INT8 iMilitiaType );


// now that town training is complete, handle the continue boxes
void HandleContinueOfTownTraining( void );

// handle completion of assignment byt his soldier too and inform the player
// HEADROCK HAM 3.6: Added argument for Mobile Militia training.
void HandleCompletionOfTownTrainingByGroupWithTrainer( SOLDIERTYPE *pTrainer, UINT8 ubMilitiaType );

// clear the list of training completed sectors
void ClearSectorListForCompletedTrainingOfMilitia( void );

BOOLEAN MilitiaTrainingAllowedInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
BOOLEAN MilitiaTrainingAllowedInTown( INT8 bTownId );

// HEADROCK HAM 3.6: Function to calculate Militia and Mobile Militia training squad size.
UINT8 CalcNumMilitiaTrained(UINT8 ubBestLeadership, BOOLEAN fMobile);

// HEADROCK HAM 3.6: Total upkeep costs for YESTERDAY at midnight. Saved to savegames.
extern UINT32 guiTotalUpkeepForMilitia;

// HEADROCK HAM 3.6: Daily check for upkeep of all militia
void HandleMilitiaUpkeepPayment( void );

// Type to hold number of militia in each sector.
typedef struct MILITIA_LIST_TYPE
{
	UINT8 ubSectorId;
	UINT8 ubNumTownGreens;
	UINT8 ubNumTownRegulars;
	UINT8 ubNumTownElites;
	UINT8 ubNumMobileGreens;
	UINT8 ubNumMobileRegulars;
	UINT8 ubNumMobileElites;

} MILITIA_LIST_TYPE;

// HEADROCK HAM 3.6: Calculate upkeep costs for militia
UINT32 CalcMilitiaUpkeep( void );

// Flugente: militia volunteer pool
// how many volunteers do we currently have?
INT32 GetVolunteerPool();

// add/remove volunteers from pool
void AddVolunteers( FLOAT asNum );

FLOAT CalcHourlyVolunteerGain();

// every hour, controlled sectors add to our volunteer pool
void UpdateVolunteers();

#endif