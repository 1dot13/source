#ifndef __INSURANCE_CONTRACT_H
#define __INSURANCE_CONTRACT_H

#include "Soldier Control.h"


void GameInitInsuranceContract();
BOOLEAN EnterInsuranceContract();
void ExitInsuranceContract();
void HandleInsuranceContract();
void RenderInsuranceContract();

extern	INT16		gsCurrentInsuranceMercIndex;

// determines if a merc will run out of there insurance contract
void DailyUpdateOfInsuredMercs();

//void InsuranceContractPayLifeInsuranceForDeadMerc( LIFE_INSURANCE_PAYOUT *pPayoutStruct );

BOOLEAN AddLifeInsurancePayout( SOLDIERTYPE *pSoldier );
void InsuranceContractPayLifeInsuranceForDeadMerc( UINT16	ubPayoutID );
void StartInsuranceInvestigation( UINT16	ubPayoutID );
void EndInsuranceInvestigation( UINT16	ubPayoutID );

INT32	CalculateInsuranceContractCost( INT32 iLength, UINT8 ubMercID );

void InsuranceContractEndGameShutDown();

void PurchaseOrExtendInsuranceForSoldier( SOLDIERTYPE *pSoldier, UINT32 uiInsuranceLength );

#endif
