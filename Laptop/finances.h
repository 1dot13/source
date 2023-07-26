#ifndef __FINANCES_H
#define __FINANCES_H

void GameInitFinances();
void EnterFinances();
void ExitFinances();
void HandleFinances();
void RenderFinances();

#define	FINANCES_DATA_FILE					"TEMP\\finances.dat"

// the financial structure
struct finance{
 UINT8 ubCode; // the code index in the finance code table
 UINT32 uiIdNumber; // unique id number
 UINT8 ubSecondCode; // secondary code 
 UINT32 uiDate; // time in the world in global time
 INT32 iAmount; // the amount of the transaction
 INT32 iBalanceToDate;
 struct finance *Next; // next unit in the list
};

enum
{
	ACCRUED_INTEREST,
	ANONYMOUS_DEPOSIT,
	TRANSACTION_FEE,
	HIRED_MERC,
	BOBBYR_PURCHASE,
	PAY_SPECK_FOR_MERC,
	MEDICAL_DEPOSIT,
	IMP_PROFILE,
	PURCHASED_INSURANCE,
	REDUCED_INSURANCE,
	EXTENDED_INSURANCE,
	CANCELLED_INSURANCE,
	INSURANCE_PAYOUT,
	EXTENDED_CONTRACT_BY_1_DAY,
	EXTENDED_CONTRACT_BY_1_WEEK,
	EXTENDED_CONTRACT_BY_2_WEEKS,
	DEPOSIT_FROM_GOLD_MINE,
	DEPOSIT_FROM_SILVER_MINE,
	PURCHASED_FLOWERS,
	FULL_MEDICAL_REFUND,
	PARTIAL_MEDICAL_REFUND,
	NO_MEDICAL_REFUND,
	PAYMENT_TO_NPC,
	TRANSFER_FUNDS_TO_MERC,
	TRANSFER_FUNDS_FROM_MERC,
	TRAIN_TOWN_MILITIA,
	PURCHASED_ITEM_FROM_DEALER,
	MERC_DEPOSITED_MONEY_TO_PLAYER_ACCOUNT,
	SOLD_ITEMS,
	FACILITY_OPERATIONS, // HEADROCK HAM 3.6: Facility costs (daily payment)
	MILITIA_UPKEEP, // HEADROCK HAM 3.6: Militia Upkeep Costs (daily payment)
	PRISONER_RANSOM,		// Flugente: ransom for released prisoners
	WHO_SUBSCRIPTION,		// Flugente: subscription do WHO data
	PMC_CONTRACT,			// Flugente: hired militia from a PMC
	SAM_REPAIR,				// Flugente: repair SAM site
	WORKERS_TRAINED,		// Flugente: train workers
	PROMOTE_MILITIA,		// Flugente: drill militia
	MINI_EVENT,				// rftr: mini events
	REBEL_COMMAND,			// rftr: rebel command
	REBEL_COMMAND_SPENDING,	// rftr: rebel command
	REBEL_COMMAND_BOUNTY_PAYOUT, // rftr: rebel command soldier bounties
	TEXT_NUM_FINCANCES
};


typedef struct finance FinanceUnit;
typedef struct finance *FinanceUnitPtr;

extern FinanceUnitPtr pFinanceListHead;
//extern INT32 iCurrentBalance;
UINT32 AddTransactionToPlayersBook(UINT8 ubCode, UINT8 ubSecondCode, UINT32 uiDate, INT32 iAmount);
void InsertDollarSignInToString( STR16 pString );
void InsertCommasForDollarFigure( STR16 pString );
INT32 GetTodaysDaysIncome( void );
INT32 GetProjectedTotalDailyIncome( void );
INT32 GetProjectedExpenses( void ); // HEADROCK HAM 3.6: Projected daily expenses.

#endif



 


