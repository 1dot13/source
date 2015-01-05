#ifndef __PERSONNEL_H
#define __PERSONNEL_H


#include "Types.h"
#include "Soldier Control.h"


// delay for change in ATM mode
#define DELAY_PER_MODE_CHANGE_IN_ATM 2000

//enums for the pPersonnelScreenStrings[]
enum
{
	PRSNL_TXT_HEALTH,			// 0	HEALTH OF MERC
	PRSNL_TXT_AGILITY,
	PRSNL_TXT_DEXTERITY,
	PRSNL_TXT_STRENGTH,
	PRSNL_TXT_LEADERSHIP,
	PRSNL_TXT_WISDOM,			// 5
	PRSNL_TXT_EXP_LVL,			//		EXPERIENCE LEVEL
	PRSNL_TXT_MARKSMANSHIP,
	PRSNL_TXT_MECHANICAL,
	PRSNL_TXT_EXPLOSIVES,
	PRSNL_TXT_MEDICAL,			// 10
	PRSNL_TXT_MED_DEPOSIT,		//		AMOUNT OF MEDICAL DEPOSIT PUT DOWN ON THE MERC
	PRSNL_TXT_CURRENT_CONTRACT,	//		COST OF CURRENT CONTRACT
	PRSNL_TXT_KILLS,			//		NUMBER OF KILLS BY MERC
	PRSNL_TXT_ASSISTS,			//		NUMBER OF ASSISTS ON KILLS BY MERC
	PRSNL_TXT_DAILY_COST,		// 15	DAILY COST OF MERC
	PRSNL_TXT_TOTAL_COST,		//		TOTAL COST OF MERC
	PRSNL_TXT_CONTRACT,			//		COST OF CURRENT CONTRACT
	PRSNL_TXT_TOTAL_SERVICE,	//		TOTAL SERVICE RENDERED BY MERC
	PRSNL_TXT_UNPAID_AMOUNT,	//		AMOUNT LEFT ON MERC MERC TO BE PAID
	PRSNL_TXT_HIT_PERCENTAGE,	// 20	PERCENTAGE OF SHOTS THAT HIT TARGET
	PRSNL_TXT_BATTLES,			//		NUMBER OF BATTLES FOUGHT
	PRSNL_TXT_TIMES_WOUNDED,	//		NUMBER OF TIMES MERC HAS BEEN WOUNDED
	PRSNL_TXT_SKILLS,
	PRSNL_TXT_NOSKILLS,
	PRSNL_TXT_ACHIEVEMNTS,
	TEXT_NUM_PRSNL,
};

// departed states
enum
{
	DEPARTED_DEAD = 0,
	DEPARTED_FIRED,
	DEPARTED_OTHER,
	DEPARTED_MARRIED,
	DEPARTED_CONTRACT_EXPIRED,
	DEPARTED_QUIT,
	TEXT_NUM_DEPARTED
};

void GameInitPersonnel();
void EnterPersonnel();
void ExitPersonnel();
void HandlePersonnel();
void RenderPersonnel();


// add character to:

// leaving for odd reasons
void AddCharacterToOtherList( SOLDIERTYPE *pSoldier );

// killed and removed
void AddCharacterToDeadList( SOLDIERTYPE *pSoldier );

// simply fired...but alive
void AddCharacterToFiredList( SOLDIERTYPE *pSoldier );

// get the total amt of money on this guy
INT32 GetFundsOnMerc( SOLDIERTYPE *pSoldier );

BOOLEAN TransferFundsFromMercToBank( SOLDIERTYPE *pSoldier, INT32 iCurrentBalance );
BOOLEAN TransferFundsFromBankToMerc( SOLDIERTYPE *pSoldier, INT32 iCurrentBalance );

BOOLEAN RemoveNewlyHiredMercFromPersonnelDepartedList( UINT8 ubProfile );

void AssignPersonnelSkillTraitHelpText( UINT8 ubTraitNumber, BOOLEAN fExpertLevel, BOOLEAN fRegMale, STR16 apStr );

#endif
