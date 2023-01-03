#ifndef _MERC_CONTRACT_H_
#define _MERC_CONTRACT_H_


#include "Soldier Control.h"


//enums used for extending contract, etc.
enum
{
	CONTRACT_EXTEND_1_DAY,
	CONTRACT_EXTEND_1_WEEK,
	CONTRACT_EXTEND_2_WEEK,
	NUM_CONTRACT_EXTEND,
};


typedef struct 
{
	UINT8 ubProfileID;
	UINT8	ubFiller[ 3 ];

} CONTRACT_NEWAL_LIST_NODE;


// WDS - make number of mercenaries, etc. be configurable
//extern CONTRACT_NEWAL_LIST_NODE	ContractRenewalList[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
//extern UINT8										ubNumContractRenewals;
extern BOOLEAN									gfContractRenewalSquenceOn;		
//extern UINT8										ubCurrentContractRenewal;
extern BOOLEAN									gfInContractMenuFromRenewSequence;


/*

//list of quotes used in renewing a mercs contract
enum
{
	LAME_REFUSAL_DOING_SOMETHING_ELSE = 73,
	DEPARTING_COMMENT_AFTER_48_HOURS	= 75,
	CONTRACTS_OVER_U_EXTENDING = 79,
	ACCEPT_CONTRACT_RENEWAL = 80,
	REFUSAL_TO_RENEW_POOP_MORALE = 85,
	DEPARTING_COMMENT_BEFORE_48_HOURS=88,
	DEATH_RATE_REFUSAL=89,
	HATE_MERC_1_ON_TEAM,
	HATE_MERC_2_ON_TEAM,
	LEARNED_TO_HATE_MERC_ON_TEAM,
	JOING_CAUSE_BUDDY_1_ON_TEAM,
	JOING_CAUSE_BUDDY_2_ON_TEAM,
	JOING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM,
	PRECEDENT_TO_REPEATING_ONESELF,
	REFUSAL_DUE_TO_LACK_OF_FUNDS,
};
*/

BOOLEAN	MercContractHandling( SOLDIERTYPE	*pSoldier, UINT8 ubDesiredAction );

BOOLEAN StrategicRemoveMerc( SOLDIERTYPE *pSoldier );
BOOLEAN BeginStrategicRemoveMerc( SOLDIERTYPE *pSoldier, BOOLEAN fAddRehireButton );


BOOLEAN WillMercRenew( SOLDIERTYPE	*pSoldier, BOOLEAN fSayQuote );
void HandleBuddiesReactionToFiringMerc(SOLDIERTYPE *pFiredSoldier, INT8 bMoraleEvent );
void CheckIfMercGetsAnotherContract( SOLDIERTYPE *pSoldier );
void FindOutIfAnyMercAboutToLeaveIsGonnaRenew( void );

void BeginContractRenewalSequence( );
void HandleContractRenewalSequence( );
void EndCurrentContractRenewal( );
void HandleMercIsWillingToRenew( UINT8 ubID );
void HandleMercIsNotWillingToRenew( UINT8 ubID );

BOOLEAN ContractIsExpiring( SOLDIERTYPE *pSoldier );
UINT32 GetHourWhenContractDone( SOLDIERTYPE *pSoldier );
BOOLEAN ContractIsGoingToExpireSoon( SOLDIERTYPE *pSoldier );

BOOLEAN LoadContractRenewalDataFromSaveGameFile( HWFILE hFile );
BOOLEAN SaveContractRenewalDataToSaveGameFile( HWFILE hFile );



// rehiring of mercs from leave equipment pop up 
extern BOOLEAN	fEnterMapDueToContract;
extern SOLDIERTYPE *pContractReHireSoldier;
extern SOLDIERTYPE *pLeaveSoldier;
extern UINT8 ubQuitType;
extern BOOLEAN gfFirstMercSayQuote;

#endif

