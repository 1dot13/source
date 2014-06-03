#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "sgp.h"
	#include "message.h"
	#include "quests.h"
	#include "game clock.h"
	#include "StrategicMap.h"
	#include "soldier profile.h"
	#include "LaptopSave.h"
	#include "Handle Items.h"
	#include "overhead.h"
	#include "Interface Dialogue.h"
	#include "Soldier Profile.h"
	#include "Isometric Utils.h"
	#include "Render Fun.h"
	#include "History.h"
	#include "Map Screen Helicopter.h"
	#include "Overhead.h"
	#include "Strategic Mines.h"
	#include "Boxing.h"
	#include "Campaign Types.h"
	#include "Strategic Town Loyalty.h"
	#include "Queen Command.h"
	#include "Campaign.h"
	#include "GameSettings.h"
	#include "Arms Dealer Init.h"
	#include "GameSettings.h"
	#include "Random.h"
	#include "Assignments.h"
	#include "strategic.h"
	#include "Strategic Event Handler.h"
#endif

#include "BobbyRMailOrder.h"
#include "connect.h"

#ifdef JA2UB
#include "email.h"
#include "Strategic Merc Handler.h"
#include "laptop.h"
#include "Ja25 Strategic Ai.h"
#include "ub_config.h"
#include "Ja25_Tactical.h"
#endif

#include "LuaInitNPCs.h"

#include "Luaglobal.h"

// anv: Waldo The Mechanic - for fact checking with helicopter
#include "Vehicles.h"

#define TESTQUESTS

extern SOLDIERTYPE * gpSrcSoldier;
extern SOLDIERTYPE * gpDestSoldier;

UINT8 gubQuest[MAX_QUESTS];
UINT8 gubFact[ NUM_FACTS ]; // this has to be updated when we figure out how many facts we have

INT16	gsFoodQuestSectorX;
INT16	gsFoodQuestSectorY;

extern void	GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );

extern void GiveQuestRewardPoint( INT16 sQuestSectorX, INT16 sQuestsSectorY, INT8 bExpReward, UINT8 bException );


void SetFactTrue( UINT16 usFact )
{
	// This function is here just for control flow purposes (debug breakpoints)
	// and code is more readable that way

	// must intercept when Jake is first trigered to start selling fuel
	if ( ( usFact == FACT_ESTONI_REFUELLING_POSSIBLE ) && ( CheckFact( usFact, 0 ) == FALSE ) )
	{
		// give him some gas...
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_JAKE, GAS_CAN, ( UINT8 ) ( 4 + Random( 3 ) ) );
	}

	gubFact[usFact] = TRUE;
}

void SetFactFalse( UINT16 usFact )
{
	gubFact[usFact] = FALSE;
}

BOOLEAN CheckForNewShipment( void )
{
	ITEM_POOL *			pItemPool;

	if ( ( gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X ) && ( gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y ) && ( gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z ) )
	{
		if ( GetItemPoolFromGround( BOBBYR_SHIPPING_DEST_GRIDNO, &pItemPool ) )
		{
			return( !( ITEMPOOL_VISIBLE( pItemPool ) ) );
		}
	}
	return( FALSE );
}

BOOLEAN CheckNPCWounded( UINT8 ubProfileID, BOOLEAN fByPlayerOnly )
{
	SOLDIERTYPE * pSoldier;

	// is the NPC is wounded at all?
	pSoldier = FindSoldierByProfileID( ubProfileID, FALSE );
	if (pSoldier && pSoldier->stats.bLife < pSoldier->stats.bLifeMax)
	{
		if (fByPlayerOnly)
		{
			if (gMercProfiles[ubProfileID].ubMiscFlags & PROFILE_MISC_FLAG_WOUNDEDBYPLAYER)
			{
				return( TRUE );
			}
			else
			{
				return( FALSE );
			}
		}
		else
		{
			return( TRUE );
		}
	}
	else
	{
		return( FALSE );

	}
}

BOOLEAN CheckNPCInOkayHealth( UINT8 ubProfileID )
{
	SOLDIERTYPE * pSoldier;

	// is the NPC at better than half health?
	pSoldier = FindSoldierByProfileID( ubProfileID, FALSE );
	if (pSoldier && pSoldier->stats.bLife > (pSoldier->stats.bLifeMax / 2) && pSoldier->stats.bLife > 30)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN CheckNPCBleeding( UINT8 ubProfileID )
{
	SOLDIERTYPE * pSoldier;

	// the NPC is wounded...
	pSoldier = FindSoldierByProfileID( ubProfileID, FALSE );
	if (pSoldier && pSoldier->stats.bLife > 0 && pSoldier->bBleeding > 0)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}

}

BOOLEAN CheckNPCWithin( UINT8 ubFirstNPC, UINT8 ubSecondNPC, UINT8 ubMaxDistance )
{
	SOLDIERTYPE * pFirstNPC, * pSecondNPC;

	pFirstNPC = FindSoldierByProfileID( ubFirstNPC, FALSE );
	pSecondNPC = FindSoldierByProfileID( ubSecondNPC, FALSE );
	if (!pFirstNPC || !pSecondNPC)
	{
		return( FALSE );
	}
	return( PythSpacesAway( pFirstNPC->sGridNo, pSecondNPC->sGridNo ) <= ubMaxDistance );
}

BOOLEAN CheckGuyVisible( UINT8 ubNPC, UINT8 ubGuy )
{
	// NB ONLY WORKS IF ON DIFFERENT TEAMS
	SOLDIERTYPE * pNPC, * pGuy;

	pNPC = FindSoldierByProfileID( ubNPC, FALSE );
	pGuy = FindSoldierByProfileID( ubGuy, FALSE );
	if (!pNPC || !pGuy)
	{
		return( FALSE );
	}
	if (pNPC->aiData.bOppList[ pGuy->ubID ] == SEEN_CURRENTLY )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN CheckNPCAt( UINT8 ubNPC, INT32 sGridNo )
{
	SOLDIERTYPE * pNPC;

	pNPC = FindSoldierByProfileID( ubNPC, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	return( pNPC->sGridNo == sGridNo );
}

BOOLEAN CheckNPCIsEnemy( UINT8 ubProfileID )
{
	SOLDIERTYPE * pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	if (pNPC->bSide == gbPlayerNum || pNPC->aiData.bNeutral)
	{
		if (pNPC->ubCivilianGroup != NON_CIV_GROUP)
		{
			// although the soldier is NOW the same side, this civ group could be set to "will become hostile"
			return( gTacticalStatus.fCivGroupHostile[ pNPC->ubCivilianGroup ] >= CIV_GROUP_WILL_BECOME_HOSTILE );
		}
		else
		{
			return( FALSE );
		}
	}
	else
	{
		return( TRUE );
	}
}

BOOLEAN CheckIfMercIsNearNPC( SOLDIERTYPE *pMerc, UINT8 ubProfileId )
{
	SOLDIERTYPE *		pNPC;
	INT32 sGridNo;

	// no merc nearby?
	if ( pMerc == NULL )
	{
		return( FALSE );
	}

	pNPC = FindSoldierByProfileID( ubProfileId, FALSE );
	if ( pNPC == NULL )
	{
		return( FALSE );
	}
	sGridNo = pNPC->sGridNo;

	// is the merc and NPC close enough?
	if (PythSpacesAway( sGridNo, pMerc->sGridNo ) <= 9)
	{
		return( TRUE );
	}

	return( FALSE );
}


INT8 NumWoundedMercsNearby( UINT8 ubProfileID )
{
	INT8						bNumber = 0;
	UINT32					uiLoop;
	SOLDIERTYPE *		pNPC;
	SOLDIERTYPE *		pSoldier;
	INT32 sGridNo;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	sGridNo = pNPC->sGridNo;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife > 0 && pSoldier->stats.bLife < pSoldier->stats.bLifeMax && pSoldier->bAssignment != ASSIGNMENT_HOSPITAL )
		{
			if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= HOSPITAL_PATIENT_DISTANCE)
			{
				bNumber++;
			}
		}
	}

	return( bNumber );
}

INT8 NumMercsNear( UINT8 ubProfileID, UINT8 ubMaxDist )
{
	INT8						bNumber = 0;
	UINT32					uiLoop;
	SOLDIERTYPE *		pNPC;
	SOLDIERTYPE *		pSoldier;
	INT32 sGridNo;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	sGridNo = pNPC->sGridNo;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE )
		{
			if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= ubMaxDist)
			{
				bNumber++;
			}
		}
	}

	return( bNumber );
}

BOOLEAN CheckNPCIsEPC( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;

	if ( gMercProfiles[ ubProfileID ].bMercStatus == MERC_IS_DEAD )
	{
		return( FALSE );
	}

	pNPC = FindSoldierByProfileID( ubProfileID, TRUE );
	if (!pNPC)
	{
		return( FALSE );
	}
	return( (pNPC->ubWhatKindOfMercAmI == MERC_TYPE__EPC ) );
}

BOOLEAN CheckNPCIsRPC( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;

	if ( gMercProfiles[ ubProfileID ].bMercStatus == MERC_IS_DEAD )
	{
		return( FALSE );
	}

	pNPC = FindSoldierByProfileID( ubProfileID, TRUE );
	if (!pNPC)
	{
		return( FALSE );
	}
	return( (pNPC->ubWhatKindOfMercAmI == MERC_TYPE__NPC ) );
}
//DBrot: More Rooms
BOOLEAN NPCInRoom( UINT8 ubProfileID, UINT16 usRoomID )
{
	SOLDIERTYPE *		pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC || (gusWorldRoomInfo[ pNPC->sGridNo ] != usRoomID) )
	{
		return( FALSE );
	}
	return( TRUE );
}
//DBrot: More Rooms
BOOLEAN NPCInRoomRange( UINT8 ubProfileID, UINT16 usRoomID1, UINT16 usRoomID2 )
{
	SOLDIERTYPE *		pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC || (gusWorldRoomInfo[ pNPC->sGridNo ] < usRoomID1) || (gusWorldRoomInfo[ pNPC->sGridNo ] > usRoomID2) )
	{
		return( FALSE );
	}
	return( TRUE );
}

BOOLEAN PCInSameRoom( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;
	//DBrot: More Rooms
	//UINT8						ubRoom;
	UINT16 usRoom;
	INT8		bLoop;
	SOLDIERTYPE * pSoldier;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC )
	{
		return( FALSE );
	}
	usRoom = gusWorldRoomInfo[ pNPC->sGridNo ];

	for ( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
	{
		pSoldier = MercPtrs[ bLoop ];
		if ( pSoldier && pSoldier->bActive && pSoldier->bInSector )
		{
			if ( gusWorldRoomInfo[ pSoldier->sGridNo ] == usRoom )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


BOOLEAN CheckTalkerStrong( void )
{
	if (gpSrcSoldier && gpSrcSoldier->bTeam == gbPlayerNum)
	{
		return( gpSrcSoldier->stats.bStrength >= 84 );
	}
	else if (gpDestSoldier && gpDestSoldier->bTeam == gbPlayerNum)
	{
		return( gpDestSoldier->stats.bStrength >= 84 );
	}
	return( FALSE );
}

BOOLEAN CheckTalkerFemale( void )
{
	if (gpSrcSoldier && gpSrcSoldier->bTeam == gbPlayerNum && gpSrcSoldier->ubProfile != NO_PROFILE)
	{
		return( gMercProfiles[ gpSrcSoldier->ubProfile ].bSex == FEMALE );
	}
	else if (gpDestSoldier && gpDestSoldier->bTeam == gbPlayerNum && gpDestSoldier->ubProfile != NO_PROFILE)
	{
		return( gMercProfiles[ gpDestSoldier->ubProfile ].bSex == FEMALE );
	}
	return( FALSE );
}

BOOLEAN CheckTalkerUnpropositionedFemale( void )
{
	if (gpSrcSoldier && gpSrcSoldier->bTeam == gbPlayerNum && gpSrcSoldier->ubProfile != NO_PROFILE)
	{
		if ( !(gMercProfiles[ gpSrcSoldier->ubProfile ].ubMiscFlags2 & PROFILE_MISC_FLAG2_ASKED_BY_HICKS) )
		{
			return( gMercProfiles[ gpSrcSoldier->ubProfile ].bSex == FEMALE );
		}
	}
	else if (gpDestSoldier && gpDestSoldier->bTeam == gbPlayerNum && gpDestSoldier->ubProfile != NO_PROFILE)
	{
		if ( !(gMercProfiles[ gpDestSoldier->ubProfile ].ubMiscFlags2 & PROFILE_MISC_FLAG2_ASKED_BY_HICKS) )
		{
			return( gMercProfiles[ gpDestSoldier->ubProfile ].bSex == FEMALE );
		}
	}
	return( FALSE );
}

INT8 NumMalesPresent( UINT8 ubProfileID )
{
	INT8						bNumber = 0;
	UINT32					uiLoop;
	SOLDIERTYPE *		pNPC;
	SOLDIERTYPE *		pSoldier;
	INT32 sGridNo;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	sGridNo = pNPC->sGridNo;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE)
		{
			if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[ pSoldier->ubProfile].bSex == MALE )
			{
				if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= 8)
				{
					bNumber++;
				}
			}
		}
	}

	return( bNumber );
}


BOOLEAN FemalePresent( UINT8 ubProfileID )
{
	UINT32					uiLoop;
	SOLDIERTYPE *		pNPC;
	SOLDIERTYPE *		pSoldier;
	INT32 sGridNo;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pNPC)
	{
		return( FALSE );
	}
	sGridNo = pNPC->sGridNo;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && pSoldier->bTeam == gbPlayerNum && pSoldier->stats.bLife >= OKLIFE)
		{
			if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[ pSoldier->ubProfile].bSex == FEMALE )
			{
				if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= 10)
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}


BOOLEAN CheckPlayerHasHead( void )
{
	INT8						bLoop;
	SOLDIERTYPE *		pSoldier;

	for ( bLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; bLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; bLoop++ )
	{
		pSoldier = MercPtrs[ bLoop ];

		if ( pSoldier->bActive && pSoldier->stats.bLife > 0 )
		{
			if ( FindObjInObjRange( pSoldier, HEAD_2, HEAD_7 ) != NO_SLOT )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );

}

BOOLEAN CheckNPCSector( UINT8 ubProfileID, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	SOLDIERTYPE * pSoldier;

	pSoldier = FindSoldierByProfileID( ubProfileID, TRUE );

	if( pSoldier )
	{
		if (pSoldier->sSectorX == sSectorX &&
			pSoldier->sSectorY == sSectorY &&
			pSoldier->bSectorZ == bSectorZ )
		{
			return( TRUE );
		}
	}
	else if (gMercProfiles[ubProfileID].sSectorX == sSectorX &&
		gMercProfiles[ubProfileID].sSectorY == sSectorY &&
		gMercProfiles[ubProfileID].bSectorZ == bSectorZ )
	{
		return( TRUE );
	}

	return( FALSE );

}

BOOLEAN AIMMercWithin( INT32 sGridNo, INT16 sDistance )
{
	UINT32					uiLoop;
	SOLDIERTYPE *		pSoldier;

	for ( uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pSoldier = MercSlots[ uiLoop ];

		if ( pSoldier && (pSoldier->bTeam == gbPlayerNum) && (pSoldier->stats.bLife >= OKLIFE) && ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) )
		{
			if (PythSpacesAway( sGridNo, pSoldier->sGridNo ) <= sDistance)
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

BOOLEAN CheckNPCCowering( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC )
	{
		return( FALSE );
	}
	return( ( (pNPC->flags.uiStatusFlags & SOLDIER_COWERING) != 0) );
}

UINT8 CountBartenders( void )
{
	UINT8		ubLoop;
	UINT8		ubBartenders = 0;

	for( ubLoop = HERVE; ubLoop <= CARLO; ubLoop++ )
	{
		if (gMercProfiles[ ubLoop ].bNPCData != 0 )
		{
			ubBartenders++;
		}
	}
	return( ubBartenders );
}

BOOLEAN CheckNPCIsUnderFire( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC )
	{
		return( FALSE );
	}
	return( pNPC->aiData.bUnderFire != 0 );
}

BOOLEAN NPCHeardShot( UINT8 ubProfileID )
{
	SOLDIERTYPE *		pNPC;

	pNPC = FindSoldierByProfileID( ubProfileID, FALSE );
	if ( !pNPC )
	{
		return( FALSE );
	}
	return( pNPC->ubMiscSoldierFlags & SOLDIER_MISC_HEARD_GUNSHOT );
}

BOOLEAN InTownSectorWithTrainingLoyalty( INT16 sSectorX, INT16 sSectorY )
{
	UINT8	ubTown;
	INT32 iMinLoyaltyToTrain = gGameExternalOptions.iMinLoyaltyToTrain;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Quests");

	ubTown = GetTownIdForSector( sSectorX, sSectorY );
	if ( ( ubTown != BLANK_SECTOR ) && gTownLoyalty[ ubTown ].fStarted && gfTownUsesLoyalty[ ubTown ])
	{
		return( gTownLoyalty[ ubTown ].ubRating >= iMinLoyaltyToTrain );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN CheckFact( UINT16 usFact, UINT8 ubProfileID )
{


//	LuaCheckFact ( usFact, ubProfileID, 0 );

//#if 0
	INT8 bTown = -1;


	switch( usFact )
	{
#ifdef JA2UB
//Ja25 No dimitri
#else
		case FACT_DIMITRI_DEAD:
			gubFact[ usFact ] = (gMercProfiles[ DIMITRI ].bMercStatus == MERC_IS_DEAD );
			break;
#endif
		case FACT_CURRENT_SECTOR_IS_SAFE:
			gubFact[FACT_CURRENT_SECTOR_IS_SAFE] = !( ( (gTacticalStatus.fEnemyInSector && NPCHeardShot( ubProfileID ) ) || gTacticalStatus.uiFlags & INCOMBAT ) );
			break;
		case FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT:
		case FACT_NEW_BOBBYRAY_SHIPMENT_WAITING:
			if ( gubFact[FACT_PABLO_PUNISHED_BY_PLAYER] == TRUE && gubFact[FACT_PABLO_RETURNED_GOODS] == FALSE && gMercProfiles[ PABLO ].bMercStatus != MERC_IS_DEAD )
			{
				gubFact[FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT] = FALSE;
				gubFact[FACT_NEW_BOBBYRAY_SHIPMENT_WAITING] = FALSE;
			}
			else
			{
				if (CheckForNewShipment()) // if new stuff waiting unseen in Drassen
				{
					gubFact[FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT] = FALSE;
					gubFact[FACT_NEW_BOBBYRAY_SHIPMENT_WAITING] = TRUE;
				}
				else if ( CountNumberOfBobbyPurchasesThatAreInTransit() > 0) // if stuff in transit
				{
					if ( gubFact[ FACT_PACKAGE_DAMAGED ] == TRUE )
					{
						gubFact[FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT] = FALSE;
					}
					else
					{
						gubFact[FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT] = TRUE;
					}
					gubFact[FACT_NEW_BOBBYRAY_SHIPMENT_WAITING] = FALSE;
				}
				else
				{
					gubFact[FACT_BOBBYRAY_SHIPMENT_IN_TRANSIT] = FALSE;
					gubFact[FACT_NEW_BOBBYRAY_SHIPMENT_WAITING] = FALSE;
				}
			}
			break;
		case FACT_NPC_WOUNDED:
			gubFact[FACT_NPC_WOUNDED] = CheckNPCWounded( ubProfileID, FALSE );
			break;
		case FACT_NPC_WOUNDED_BY_PLAYER:
			gubFact[FACT_NPC_WOUNDED_BY_PLAYER] = CheckNPCWounded( ubProfileID, TRUE );
			break;
#ifdef JA2UB
// no ja25 UB
#else
		case FACT_IRA_NOT_PRESENT:
			gubFact[FACT_IRA_NOT_PRESENT] = !CheckNPCWithin( ubProfileID, IRA, 10 );
			break;
		case FACT_IRA_TALKING:
			gubFact[FACT_IRA_TALKING] = ( gubSrcSoldierProfile == IRA );
			break;
		case FACT_IRA_UNHIRED_AND_ALIVE:
			if ( gMercProfiles[ IRA ].bMercStatus != MERC_IS_DEAD && CheckNPCSector( IRA, 10, 1, 1) && !(gMercProfiles[IRA].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED) )
			{
				gubFact[FACT_IRA_UNHIRED_AND_ALIVE] = TRUE;
			}
			else
			{
				gubFact[FACT_IRA_UNHIRED_AND_ALIVE] = FALSE;
			}
			break;
#endif
		case FACT_NPC_BLEEDING:
			gubFact[FACT_NPC_BLEEDING] = CheckNPCBleeding( ubProfileID );
			break;
		case FACT_NPC_BLEEDING_BUT_OKAY:
			if ( CheckNPCBleeding( ubProfileID ) && CheckNPCInOkayHealth( ubProfileID ) )
			{
				gubFact[FACT_NPC_BLEEDING_BUT_OKAY] = TRUE;
			}
			else
			{
				gubFact[FACT_NPC_BLEEDING_BUT_OKAY] = FALSE;
			}
			break;
#ifdef JA2UB
//Ja25: NO Carmen
#else
		case FACT_PLAYER_HAS_HEAD_AND_CARMEN_IN_SAN_MONA:
			gubFact[usFact] = (CheckNPCSector( CARMEN, 5, MAP_ROW_C, 0 ) && CheckPlayerHasHead() );
			break;

		case FACT_PLAYER_HAS_HEAD_AND_CARMEN_IN_CAMBRIA:
			gubFact[usFact] = (CheckNPCSector( CARMEN, 9, MAP_ROW_G, 0 ) && CheckPlayerHasHead() );
			break;

		case FACT_PLAYER_HAS_HEAD_AND_CARMEN_IN_DRASSEN:
			gubFact[usFact] = (CheckNPCSector( CARMEN, 13, MAP_ROW_C, 0 ) && CheckPlayerHasHead() );
			break;
#endif
		case FACT_NPC_OWED_MONEY:
			gubFact[FACT_NPC_OWED_MONEY] = (gMercProfiles[ubProfileID].iBalance < 0);
			break;

		case FACT_FATHER_DRUNK:
			gubFact[FACT_FATHER_DRUNK] = ( gMercProfiles[ FATHER ].bNPCData >= 5 );
			break;

		case FACT_MICKY_DRUNK:
			gubFact[FACT_MICKY_DRUNK] = ( gMercProfiles[ MICKY ].bNPCData >= 5 );
			break;

		case FACT_BRENDA_IN_STORE_AND_ALIVE:
			// ensure alive
			if ( gMercProfiles[ BRENDA ].bMercStatus == MERC_IS_DEAD)
			{
				gubFact[FACT_BRENDA_IN_STORE_AND_ALIVE] = FALSE;
			}
			// ensure in a building and nearby
			else if ( !(NPCInRoom( BRENDA, gModSettings.usPornShopRoomBrenda ) )	)
			{
				gubFact[FACT_BRENDA_IN_STORE_AND_ALIVE] = FALSE;
			}
			else
			{
				gubFact[FACT_BRENDA_IN_STORE_AND_ALIVE] = CheckNPCWithin( ubProfileID, BRENDA, 12 );
			}
			break;
		case FACT_BRENDA_DEAD:
			gubFact[FACT_BRENDA_DEAD] = (gMercProfiles[ BRENDA ].bMercStatus == MERC_IS_DEAD);
			break;
		case FACT_NPC_IS_ENEMY:
			gubFact[FACT_NPC_IS_ENEMY] = CheckNPCIsEnemy( ubProfileID ) || gMercProfiles[ ubProfileID ].ubMiscFlags2 & PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE;
			break;
			/*
		case FACT_SKYRIDER_CLOSE_TO_CHOPPER:
			SetUpHelicopterForPlayer( 13, MAP_ROW_B, gNewVehicle[ HELICOPTER ].NewPilot, HELICOPTER );
			break;
			*/
		case FACT_SPIKE_AT_DOOR:
			gubFact[FACT_SPIKE_AT_DOOR] = CheckNPCAt( SPIKE, 9817 );
			break;
		case FACT_WOUNDED_MERCS_NEARBY:
			gubFact[usFact] = (NumWoundedMercsNearby( ubProfileID ) > 0);
			break;
		case FACT_ONE_WOUNDED_MERC_NEARBY:
			gubFact[usFact] = (NumWoundedMercsNearby( ubProfileID ) == 1);
			break;
		case FACT_MULTIPLE_WOUNDED_MERCS_NEARBY:
			gubFact[usFact] = (NumWoundedMercsNearby( ubProfileID ) > 1);
			break;
		case FACT_HANS_AT_SPOT:
			gubFact[usFact] = CheckNPCAt( HANS, gModSettings.iHansGridNo );
			break;
		case FACT_MULTIPLE_MERCS_CLOSE:
			gubFact[usFact] = ( NumMercsNear( ubProfileID, 3 ) > 1 );
			break;
		case FACT_SOME_MERCS_CLOSE:
			gubFact[usFact] = ( NumMercsNear( ubProfileID, 3 ) > 0 );
			break;
		case FACT_MARIA_ESCORTED:
			gubFact[usFact] = CheckNPCIsEPC( MARIA );
			break;
		case FACT_JOEY_ESCORTED:
			gubFact[usFact] = CheckNPCIsEPC( JOEY );
			break;
		case FACT_ESCORTING_SKYRIDER:
			gubFact[usFact] = CheckNPCIsEPC( SKYRIDER );
			break;
		case FACT_MARIA_ESCORTED_AT_LEATHER_SHOP:
			gubFact[usFact] = ( CheckNPCIsEPC( MARIA ) && (NPCInRoom( MARIA, gModSettings.usLeatherShop )) );
			break;
		case FACT_PC_STRONG_AND_LESS_THAN_3_MALES_PRESENT:
			gubFact[usFact] = ( CheckTalkerStrong() && (NumMalesPresent( ubProfileID ) < 3) );
			break;
		case FACT_PC_STRONG_AND_3_PLUS_MALES_PRESENT:
			gubFact[usFact] = ( CheckTalkerStrong() && (NumMalesPresent( ubProfileID ) >= 3) );
			break;
		case FACT_FEMALE_SPEAKING_TO_NPC:
			gubFact[usFact] = CheckTalkerFemale();
			break;
		case FACT_CARMEN_IN_C5:
			gubFact[usFact] = CheckNPCSector( CARMEN, 5, MAP_ROW_C, 0 );
			break;
		case FACT_JOEY_IN_C5:
			gubFact[usFact] = CheckNPCSector( JOEY, 5, MAP_ROW_C, 0 );
			break;
		case FACT_JOEY_NEAR_MARTHA:
			gubFact[usFact] = CheckNPCWithin( JOEY, MARTHA, 5 ) && ( CheckGuyVisible( MARTHA, JOEY ) || CheckGuyVisible( JOEY, MARTHA ) );
			break;
		case FACT_JOEY_DEAD:
			gubFact[usFact] = gMercProfiles[ JOEY ].bMercStatus == MERC_IS_DEAD;
			break;
		case FACT_MERC_NEAR_MARTHA:
			gubFact[usFact] = ( NumMercsNear( ubProfileID, 5 ) > 0 );
			break;
		case FACT_REBELS_HATE_PLAYER:
			gubFact[usFact] = (gTacticalStatus.fCivGroupHostile[ REBEL_CIV_GROUP ] == CIV_GROUP_HOSTILE);
			break;
		case FACT_CURRENT_SECTOR_G9:
			gubFact[usFact] = ( gWorldSectorX == 9 && gWorldSectorY == MAP_ROW_G && gbWorldSectorZ == 0 );
			break;
		case FACT_CURRENT_SECTOR_C5:
			gubFact[usFact] = ( gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_C && gbWorldSectorZ == 0 );
			break;
		case FACT_CURRENT_SECTOR_C13:
			gubFact[usFact] = ( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_C && gbWorldSectorZ == 0 );
			break;
		case FACT_CARMEN_HAS_TEN_THOUSAND:
			gubFact[usFact] = ( gMercProfiles[ CARMEN ].uiMoney >= 10000 );
			break;
		case FACT_SLAY_IN_SECTOR:
			gubFact[usFact] = (gMercProfiles[ SLAY ].sSectorX == gWorldSectorX && gMercProfiles[ SLAY ].sSectorY == gWorldSectorY && gMercProfiles[ SLAY ].bSectorZ == gbWorldSectorZ );
			break;
		case FACT_SLAY_HIRED_AND_WORKED_FOR_48_HOURS:
			gubFact[usFact] = ( ( gMercProfiles[ SLAY ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) && ( gMercProfiles[ SLAY ].usTotalDaysServed > 1 ) );
			break;
		case FACT_SHANK_IN_SQUAD_BUT_NOT_SPEAKING:
			gubFact[usFact] = ( ( FindSoldierByProfileID( SHANK, TRUE ) != NULL) && ( gMercProfiles[ SHANK ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) && ( gpSrcSoldier == NULL || gpSrcSoldier->ubProfile != SHANK ) );
			break;
		case FACT_SHANK_NOT_IN_SECTOR:
			gubFact[usFact] = ( FindSoldierByProfileID( SHANK, FALSE ) == NULL );
			break;
#ifdef JA2UB
//Ja25 No queen
#else
		case FACT_QUEEN_DEAD:
			gubFact[usFact] = (gMercProfiles[ QUEEN ].bMercStatus == MERC_IS_DEAD);
			break;
#endif
		case FACT_MINE_EMPTY:
			gubFact[usFact] = IsHisMineEmpty( ubProfileID );
			break;
		case FACT_MINE_RUNNING_OUT:
			gubFact[usFact] = IsHisMineRunningOut( ubProfileID );
			break;
		case FACT_MINE_PRODUCING_BUT_LOYALTY_LOW:
			gubFact[usFact] = HasHisMineBeenProducingForPlayerForSomeTime( ubProfileID ) && IsHisMineDisloyal( ubProfileID );
			break;
		case FACT_CREATURES_IN_MINE:
			gubFact[usFact] = IsHisMineInfested( ubProfileID );
			break;
		case FACT_PLAYER_LOST_MINE:
			gubFact[usFact] = IsHisMineLostAndRegained( ubProfileID );
			break;
		case FACT_MINE_AT_FULL_PRODUCTION:
			gubFact[usFact] = IsHisMineAtMaxProduction( ubProfileID );
			break;
		case FACT_DYNAMO_IN_J9:
			gubFact[usFact] = CheckNPCSector( DYNAMO, gModSettings.ubDyanmoCaptiveSectorX, gModSettings.ubDyanmoCaptiveSectorY, gModSettings.ubDyanmoCaptiveSectorZ ) &&
				NumEnemiesInAnySector( gModSettings.ubDyanmoCaptiveSectorX, gModSettings.ubDyanmoCaptiveSectorY, gModSettings.ubDyanmoCaptiveSectorZ ); //( 9, 10, 0 )
			break;
		case FACT_DYNAMO_ALIVE:
			gubFact[usFact] = ( gMercProfiles[ DYNAMO ].bMercStatus != MERC_IS_DEAD );
			break;
		case FACT_DYNAMO_SPEAKING_OR_NEARBY:
			gubFact[usFact] = ( gpSrcSoldier != NULL && (gpSrcSoldier->ubProfile == DYNAMO || ( CheckNPCWithin( gpSrcSoldier->ubProfile, DYNAMO, 10 ) && CheckGuyVisible( gpSrcSoldier->ubProfile, DYNAMO ) ) ) );
			break;
		case FACT_JOHN_EPC:
			gubFact[usFact] = CheckNPCIsEPC( JOHN );
			break;
		case FACT_MARY_EPC:
			gubFact[usFact] = CheckNPCIsEPC( MARY );
			break;
		case FACT_JOHN_AND_MARY_EPCS:
			gubFact[usFact] = CheckNPCIsEPC( JOHN ) && CheckNPCIsEPC( MARY );
			break;
		case FACT_MARY_ALIVE:
			gubFact[usFact] = ( gMercProfiles[ MARY ].bMercStatus != MERC_IS_DEAD );
			break;
		case FACT_MARY_BLEEDING:
			gubFact[usFact] = CheckNPCBleeding( MARY );
			break;
		case FACT_JOHN_ALIVE:
			gubFact[usFact] = ( gMercProfiles[ JOHN ].bMercStatus != MERC_IS_DEAD );
			break;
		case FACT_JOHN_BLEEDING:
			gubFact[usFact] = CheckNPCBleeding( JOHN );
			break;
		case FACT_MARY_DEAD:
			gubFact[usFact] = ( gMercProfiles[ MARY ].bMercStatus == MERC_IS_DEAD );
			break;

		case FACT_ANOTHER_FIGHT_POSSIBLE:
			gubFact[usFact] = AnotherFightPossible();
			break;

		case FACT_RECEIVING_INCOME_FROM_DCAC:
			gubFact[usFact] = (
				( PredictDailyIncomeFromAMine( MINE_DRASSEN, TRUE ) > 0 ) &&
				( PredictDailyIncomeFromAMine( MINE_ALMA, TRUE ) > 0 ) &&
				( PredictDailyIncomeFromAMine( MINE_CAMBRIA, TRUE ) > 0 ) &&
				( PredictDailyIncomeFromAMine( MINE_CHITZENA, TRUE ) > 0 ) );
			break;

		case FACT_PLAYER_BEEN_TO_K4:
			{
				UNDERGROUND_SECTORINFO * pUnderGroundSector;

				pUnderGroundSector = FindUnderGroundSector( 4, MAP_ROW_K, 1 );
				gubFact[usFact] = pUnderGroundSector->fVisited;
			}
			break;
		case FACT_WARDEN_DEAD:
			gubFact[usFact] = ( gMercProfiles[ WARDEN ].bMercStatus == MERC_IS_DEAD );
			break;

		case FACT_PLAYER_PAID_FOR_TWO_IN_BROTHEL:
			gubFact[usFact] = (gMercProfiles[ MADAME ].bNPCData > 1);
			break;

		case FACT_LOYALTY_OKAY:
			bTown = gMercProfiles[ ubProfileID ].bTown;
			if( ( bTown != BLANK_SECTOR ) && gTownLoyalty[ bTown ].fStarted && gfTownUsesLoyalty[ bTown ])
			{
				gubFact[usFact] = ( (gTownLoyalty[ bTown ].ubRating >= LOYALTY_LOW_THRESHOLD ) && (gTownLoyalty[ bTown ].ubRating < LOYALTY_OK_THRESHOLD ) );
			}
			else
			{
				gubFact[usFact] = FALSE;
			}
			break;

		case FACT_LOYALTY_LOW:
			bTown = gMercProfiles[ ubProfileID ].bTown;
			if( ( bTown != BLANK_SECTOR ) && gTownLoyalty[ bTown ].fStarted && gfTownUsesLoyalty[ bTown ])
			{
				// if Skyrider, ignore low loyalty until he has monologues, and wait at least a day since the latest monologue to avoid a hot/cold attitude
				if ( ( ubProfileID == SKYRIDER ) &&
						( ( guiHelicopterSkyriderTalkState == 0 ) || ( ( GetWorldTotalMin() - guiTimeOfLastSkyriderMonologue ) < ( 24 * 60 ) ) || 
						( gHelicopterSettings.fHelicopterTownLoyaltyCheck == FALSE ) ) )
				{
					gubFact[usFact] = FALSE;
				}
				else
				{
					gubFact[usFact] = (gTownLoyalty[ bTown ].ubRating < LOYALTY_LOW_THRESHOLD );
				}
			}
			else
			{
				gubFact[usFact] = FALSE;
			}
			break;

		case FACT_LOYALTY_HIGH:
			bTown = gMercProfiles[ ubProfileID ].bTown;
			if( ( bTown != BLANK_SECTOR ) && gTownLoyalty[ bTown ].fStarted && gfTownUsesLoyalty[ bTown ])
			{
				gubFact[usFact] = (gTownLoyalty[ gMercProfiles[ ubProfileID ].bTown ].ubRating >= LOYALTY_HIGH_THRESHOLD );
			}
			else
			{
				gubFact[usFact] = FALSE;
			}
			break;

		case FACT_ELGIN_ALIVE:
			gubFact[usFact] = ( gMercProfiles[ DRUGGIST ].bMercStatus != MERC_IS_DEAD );
			break;

		case FACT_SPEAKER_AIM_OR_AIM_NEARBY:
			gubFact[usFact] = gpDestSoldier && AIMMercWithin( gpDestSoldier->sGridNo, 10 );
			break;

		case FACT_MALE_SPEAKING_FEMALE_PRESENT:
			gubFact[usFact] = ( !CheckTalkerFemale() && FemalePresent( ubProfileID ) );
			break;

		case FACT_PLAYER_OWNS_2_TOWNS_INCLUDING_OMERTA:
#ifdef JA2UB
//UB
#else
			gubFact[usFact] = ( ( GetNumberOfWholeTownsUnderControl() == gGameExternalOptions.ubEarlyRebelsRecruitment[1] || gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 1 
								|| ( gubQuest[QUEST_FOOD_ROUTE] == QUESTDONE && gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 4 ) ) && IsTownUnderCompleteControlByPlayer( OMERTA ) );
#endif
			break;

		case FACT_PLAYER_OWNS_3_TOWNS_INCLUDING_OMERTA:
#ifdef JA2UB
//UB
#else
			gubFact[usFact] = ( ( GetNumberOfWholeTownsUnderControl() == gGameExternalOptions.ubEarlyRebelsRecruitment[2] || gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 1 
								|| ( gubQuest[QUEST_FOOD_ROUTE] == QUESTDONE && gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 4 ) ) && IsTownUnderCompleteControlByPlayer( OMERTA ) );
#endif
			break;

		case FACT_PLAYER_OWNS_4_TOWNS_INCLUDING_OMERTA:
#ifdef JA2UB
//UB
#else
			gubFact[usFact] = ( ( GetNumberOfWholeTownsUnderControl() >= gGameExternalOptions.ubEarlyRebelsRecruitment[3] || gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 1 
								|| ( gubQuest[QUEST_FOOD_ROUTE] == QUESTDONE && gGameExternalOptions.ubEarlyRebelsRecruitment[0] == 4 ) ) && IsTownUnderCompleteControlByPlayer( OMERTA ) );
			// silversurfer: this is the highest requirement and therefore we will automatically enable recruitment of Miguel
			if ( gubFact[usFact] )
				SetFactTrue(FACT_MIGUEL_AND_ALL_REBELS_CAN_BE_RECRUITED); // Miguel will now be willing to join and so will the other RPC
#endif
			break;

		case FACT_PLAYER_FOUGHT_THREE_TIMES_TODAY:
			gubFact[usFact] = !BoxerAvailable();
			break;

		case FACT_PLAYER_DOING_POORLY:
			gubFact[usFact] = ( CurrentPlayerProgressPercentage() < 20 );
			break;

		case FACT_PLAYER_DOING_WELL:
			gubFact[usFact] = ( CurrentPlayerProgressPercentage() > 50 );
			break;

		case FACT_PLAYER_DOING_VERY_WELL:
			gubFact[usFact] = ( CurrentPlayerProgressPercentage() > 80 );
			break;

		case FACT_FATHER_DRUNK_AND_SCIFI_OPTION_ON:
			gubFact[usFact] = ( ( gMercProfiles[ FATHER ].bNPCData >= 5 ) && gGameOptions.ubGameStyle == STYLE_SCIFI && gGameExternalOptions.fEnableCrepitus);
			break;

		case FACT_BLOODCAT_QUEST_STARTED_TWO_DAYS_AGO:
			gubFact[usFact] = ( (gubQuest[ QUEST_BLOODCATS ] != QUESTNOTSTARTED) && (GetWorldTotalMin() - GetTimeQuestWasStarted( QUEST_BLOODCATS ) > 2 * NUM_SEC_IN_DAY / NUM_SEC_IN_MIN) );
			break;

		case FACT_NOTHING_REPAIRED_YET:
			gubFact[usFact] = RepairmanIsFixingItemsButNoneAreDoneYet( ubProfileID );
			break;

		case FACT_NPC_COWERING:
			gubFact[usFact] = CheckNPCCowering( ubProfileID );
			break;

		case FACT_TOP_AND_BOTTOM_LEVELS_CLEARED:
			gubFact[usFact] = ( gubFact[ FACT_TOP_LEVEL_CLEARED ] && gubFact[ FACT_BOTTOM_LEVEL_CLEARED ] );
			break;

		case FACT_FIRST_BARTENDER:
			gubFact[ usFact ] = ( gMercProfiles[ubProfileID].bNPCData == 1 || (gMercProfiles[ubProfileID].bNPCData == 0 && CountBartenders() == 0) );
			break;

		case FACT_SECOND_BARTENDER:
			gubFact[ usFact ] = ( gMercProfiles[ubProfileID].bNPCData == 2 || (gMercProfiles[ubProfileID].bNPCData == 0 && CountBartenders() == 1) );
			break;

		case FACT_THIRD_BARTENDER:
			gubFact[ usFact ] = ( gMercProfiles[ubProfileID].bNPCData == 3 || (gMercProfiles[ubProfileID].bNPCData == 0 && CountBartenders() == 2) );
			break;

		case FACT_FOURTH_BARTENDER:
			gubFact[ usFact ] = ( gMercProfiles[ubProfileID].bNPCData == 4 || (gMercProfiles[ubProfileID].bNPCData == 0 && CountBartenders() == 3) );
			break;

		case FACT_NPC_NOT_UNDER_FIRE:
			gubFact[usFact] = !CheckNPCIsUnderFire( ubProfileID );
			break;

		case FACT_KINGPIN_NOT_IN_OFFICE:
			gubFact[usFact] = !( gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_D && NPCInRoomRange( KINGPIN, 30, 39 ) );
			// 30 to 39
			break;

		case FACT_DONT_OWE_KINGPIN_MONEY:
			gubFact[usFact] = (gubQuest[ QUEST_KINGPIN_MONEY ] != QUESTINPROGRESS);
			break;

		case FACT_NO_CLUB_FIGHTING_ALLOWED:
			// anv: added !BoxersAvailable, otherwise van Haussen would offer fight even when all boxers are dead
			gubFact[usFact] = ( gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTINPROGRESS || gfBoxersResting || !BoxersAvailable() );// plus other conditions
			break;

		case FACT_MADDOG_IS_SPEAKER:
			gubFact[usFact] = ( gubSrcSoldierProfile == MADDOG );
			break;

		case FACT_PC_HAS_CONRADS_RECRUIT_OPINION:
			gubFact[usFact] = ( gpDestSoldier && (CalcDesireToTalk( gpDestSoldier->ubProfile, gubSrcSoldierProfile, APPROACH_RECRUIT ) >= 50) );
			break;

		case FACT_NPC_HOSTILE_OR_PISSED_OFF:
			gubFact[usFact] = CheckNPCIsEnemy( ubProfileID ) || (gMercProfiles[ ubProfileID ].ubMiscFlags3 & PROFILE_MISC_FLAG3_NPC_PISSED_OFF);
			break;

		case FACT_TONY_IN_BUILDING:
			gubFact[usFact] = CheckNPCSector( TONY, gModSettings.ubPornShopTonySectorX, gModSettings.ubPornShopTonySectorY, gModSettings.ubPornShopTonySectorZ ) && 
				NPCInRoom( TONY, gModSettings.usPornShopRoomTony );
			break;

		case FACT_SHANK_SPEAKING:
			gubFact[usFact] = ( gpSrcSoldier && gpSrcSoldier->ubProfile == SHANK );
			break;

		case FACT_ROCKET_RIFLE_EXISTS:
			gubFact[usFact] = ItemTypeExistsAtLocation( gModSettings.sRocketRifleGridNo, ROCKET_RIFLE, 0, NULL );
			break;

		case FACT_PABLO_ALIVE:
			gubFact[usFact] = gMercProfiles[ PABLO ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_DOREEN_ALIVE:
			gubFact[usFact] = gMercProfiles[ DOREEN ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_WALDO_ALIVE:
			gubFact[usFact] = gMercProfiles[ WALDO ].bMercStatus != MERC_IS_DEAD;
			break;
#ifdef JA2UB
//UB
#else        
		case FACT_PERKO_ALIVE:
			gubFact[usFact] = gMercProfiles[ PERKO ].bMercStatus != MERC_IS_DEAD;
			break;
#endif
		case FACT_TONY_ALIVE:
			gubFact[usFact] = gMercProfiles[ TONY ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_VINCE_ALIVE:
			gubFact[usFact] = gMercProfiles[ VINCE ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_JENNY_ALIVE:
			gubFact[usFact] = gMercProfiles[ JENNY ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_ARNOLD_ALIVE:
			gubFact[usFact] = gMercProfiles[ ARNIE ].bMercStatus != MERC_IS_DEAD;
			break;

		// HEADROCK HAM 3.6: No longer necessarily I16.
		case FACT_BLOODCAT_LAIR_CLEANED:
			gubFact[usFact] = (SectorInfo[ gubBloodcatLairSectorId ].bBloodCats == 0);
			break;

		case FACT_NPC_BANDAGED_TODAY:
			gubFact[usFact] = (gMercProfiles[ ubProfileID ].ubMiscFlags2 & PROFILE_MISC_FLAG2_BANDAGED_TODAY) != 0;
			break;

		case FACT_PLAYER_IN_SAME_ROOM:
			gubFact[usFact] = PCInSameRoom( ubProfileID );
			break;

		case FACT_PLAYER_SPOKE_TO_DRASSEN_MINER:
			gubFact[usFact] = SpokenToHeadMiner( MINE_DRASSEN );
			break;
		case FACT_PLAYER_IN_CONTROLLED_DRASSEN_MINE:
			gubFact[usFact] = ( GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == MINE_DRASSEN && !(StrategicMap[ gWorldSectorX + MAP_WORLD_X * gWorldSectorY ].fEnemyControlled) );
			break;
		case FACT_PLAYER_SPOKE_TO_CAMBRIA_MINER:
			gubFact[usFact] = SpokenToHeadMiner( MINE_CAMBRIA );
			break;
		case FACT_PLAYER_IN_CONTROLLED_CAMBRIA_MINE:
			gubFact[usFact] = ( GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == MINE_CAMBRIA && !(StrategicMap[ gWorldSectorX + MAP_WORLD_X * gWorldSectorY ].fEnemyControlled) );
			break;
		case FACT_PLAYER_SPOKE_TO_CHITZENA_MINER:
			gubFact[usFact] = SpokenToHeadMiner( MINE_CHITZENA );
			break;
		case FACT_PLAYER_IN_CONTROLLED_CHITZENA_MINE:
			gubFact[usFact] = ( GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == MINE_CHITZENA && !(StrategicMap[ gWorldSectorX + MAP_WORLD_X * gWorldSectorY ].fEnemyControlled) );
			break;
		case FACT_PLAYER_SPOKE_TO_ALMA_MINER:
			gubFact[usFact] = SpokenToHeadMiner( MINE_ALMA );
			break;
		case FACT_PLAYER_IN_CONTROLLED_ALMA_MINE:
			gubFact[usFact] = ( GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == MINE_ALMA && !(StrategicMap[ gWorldSectorX + MAP_WORLD_X * gWorldSectorY ].fEnemyControlled) );
			break;
		case FACT_PLAYER_SPOKE_TO_GRUMM_MINER:
			gubFact[usFact] = SpokenToHeadMiner( MINE_GRUMM );
			break;
		case FACT_PLAYER_IN_CONTROLLED_GRUMM_MINE:
			gubFact[usFact] = ( GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) == MINE_GRUMM && !(StrategicMap[ gWorldSectorX + MAP_WORLD_X * gWorldSectorY ].fEnemyControlled) );
			break;

		case FACT_ENOUGH_LOYALTY_TO_TRAIN_MILITIA:
			gubFact[usFact] = InTownSectorWithTrainingLoyalty( gWorldSectorX, gWorldSectorY );
			break;

		case FACT_WALKER_AT_BAR:
			gubFact[usFact] = (gMercProfiles[ FATHER ].sSectorX == 13 && gMercProfiles[ FATHER ].sSectorY == MAP_ROW_C);
			break;

		case FACT_JOEY_ALIVE:
			gubFact[usFact] = gMercProfiles[ JOEY ].bMercStatus != MERC_IS_DEAD;
			break;

		case FACT_UNPROPOSITIONED_FEMALE_SPEAKING_TO_NPC:
			gubFact[usFact] = CheckTalkerUnpropositionedFemale();
			break;

		case FACT_84_AND_85_TRUE:
			gubFact[usFact] = CheckFact( 84, ubProfileID ) && CheckFact( FACT_HANS_AT_SPOT, ubProfileID );
			break;

		case FACT_SKYRIDER_IN_B15:
			gubFact[ usFact ] = CheckNPCSector( SKYRIDER, 15, MAP_ROW_B, 0 );
			break;

		case FACT_SKYRIDER_IN_C16:
			gubFact[ usFact ] = CheckNPCSector( SKYRIDER, 16, MAP_ROW_C, 0 );
			break;
		case FACT_SKYRIDER_IN_E14:
			gubFact[ usFact ] = CheckNPCSector( SKYRIDER, 14, MAP_ROW_E, 0 );
			break;
		case FACT_SKYRIDER_IN_D12:
			gubFact[ usFact ] = CheckNPCSector( SKYRIDER, 12, MAP_ROW_D, 0 );
			break;

		case FACT_KINGPIN_IS_ENEMY:
			gubFact[usFact] = (gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] >= CIV_GROUP_WILL_BECOME_HOSTILE);
			break;

		case FACT_DYNAMO_NOT_SPEAKER:
			gubFact[usFact] = !( gpSrcSoldier != NULL && (gpSrcSoldier->ubProfile == DYNAMO ) );
			break;

		case FACT_PABLO_BRIBED:
			gubFact[usFact] = !CheckFact( FACT_PABLOS_BRIBED, ubProfileID );
			break;

		case FACT_VEHICLE_PRESENT:
			gubFact[usFact] = CheckFact( FACT_OK_USE_HUMMER, ubProfileID ) && ( (FindSoldierByProfileID( PROF_HUMMER, TRUE ) != NULL) || (FindSoldierByProfileID( PROF_ICECREAM, TRUE ) != NULL) );
			break;

		case FACT_PLAYER_KILLED_BOXERS:
			gubFact[usFact] = !BoxerExists();
			break;

		case FACT_DIMITRI_CAN_BE_RECRUITED: // Can dimitri be recruited? should be true if already true, OR if Miguel has been recruited already OR is available for recruitment
			gubFact[usFact] = ( gubFact[usFact] || FindSoldierByProfileID( MIGUEL, TRUE ) || gubFact[FACT_MIGUEL_AND_ALL_REBELS_CAN_BE_RECRUITED] );
			break;
/*			
		case FACT_:
			gubFact[usFact] = ;
			break;
*/
		// anv: facts for checking if Waldo can repair helicopter

		case FACT_HELI_DAMAGED_CAN_START_REPAIR:
			gubFact[usFact] = ( gGameExternalOptions.fWaldoCanRepairHelicopter && 
				!fHelicopterDestroyed && !fHelicopterIsAirBorne && gubHelicopterHitsTaken == 1 &&
				pVehicleList[ iHelicopterVehicleId ].sSectorX == gMercProfiles[ WALDO ].sSectorX && pVehicleList[ iHelicopterVehicleId ].sSectorY == gMercProfiles[ WALDO ].sSectorY &&
				LaptopSaveInfo.iCurrentBalance >= CalculateHelicopterRepairCost( FALSE ) &&
				gubHelicopterHoursToRepair == 0 && CheckFact( FACT_WALDO_MET, 0 ) );
			break;

		case FACT_HELI_SERIOUSLY_DAMAGED_CAN_START_REPAIR:
			gubFact[usFact] = ( gGameExternalOptions.fWaldoCanRepairHelicopter && 
				!fHelicopterDestroyed && !fHelicopterIsAirBorne && gubHelicopterHitsTaken == 2 &&
				pVehicleList[ iHelicopterVehicleId ].sSectorX == gMercProfiles[ WALDO ].sSectorX && pVehicleList[ iHelicopterVehicleId ].sSectorY == gMercProfiles[ WALDO ].sSectorY && 
				LaptopSaveInfo.iCurrentBalance >= CalculateHelicopterRepairCost( TRUE ) &&
				gubHelicopterHoursToRepair == 0 && CheckFact( FACT_WALDO_MET, 0 ) );
			break;

		case FACT_HELI_GIVEN_MONEY_CAN_START_REPAIR:
			gubFact[usFact] = ( ( gGameExternalOptions.fWaldoCanRepairHelicopter && 
				!fHelicopterDestroyed && !fHelicopterIsAirBorne && gubHelicopterHitsTaken > 0 &&
				pVehicleList[ iHelicopterVehicleId ].sSectorX == gMercProfiles[ WALDO ].sSectorX && pVehicleList[ iHelicopterVehicleId ].sSectorY == gMercProfiles[ WALDO ].sSectorY && 
				gubHelicopterHoursToRepair == 0 ) &&
				( CheckFact(FACT_GIVEN_ENOUGH_TO_REPAIR_HELI, 0) || CheckFact(FACT_GIVEN_ENOUGH_TO_SERIOUSLY_REPAIR_HELI, 0) ) && CheckFact( FACT_WALDO_MET, 0 ) );
			break;

		case FACT_HELI_CANT_START_REPAIR:
			gubFact[usFact] = ( !CheckFact(FACT_HELI_GIVEN_MONEY_CAN_START_REPAIR, 0) && !CheckFact(FACT_HELICOPTER_IN_PERFECT_CONDITION, 0));
			break;			

		case FACT_HELICOPTER_IN_PERFECT_CONDITION:
			gubFact[usFact] = ( gubHelicopterHitsTaken == 0 );
			break;

		case FACT_HELICOPTER_LOST:
			gubFact[usFact] = fHelicopterDestroyed;
			break;

		default:
			break;
	}
	
//#endif
	return( gubFact[usFact] );
}

void StartQuest( UINT8 ubQuest, INT16 sSectorX, INT16 sSectorY )
{
	InternalStartQuest( ubQuest, sSectorX, sSectorY, TRUE );
}


void InternalStartQuest( UINT8 ubQuest, INT16 sSectorX, INT16 sSectorY, BOOLEAN fUpdateHistory )
{
#ifdef LUA_QUESTS
		LuaInternalQuest( ubQuest, sSectorX, sSectorY, fUpdateHistory, 1);
#else
	if ( gubQuest[ubQuest ] == QUESTNOTSTARTED )
	{
		gubQuest[ubQuest] = QUESTINPROGRESS;

	if ( fUpdateHistory )
	{
			if (!is_networked)
		SetHistoryFact( HISTORY_QUEST_STARTED, ubQuest, GetWorldTotalMin(), sSectorX, sSectorY );
	}
	}
	else
	{
		gubQuest[ubQuest] = QUESTINPROGRESS;
	}
#endif
}

void EndQuest( UINT8 ubQuest, INT16 sSectorX, INT16 sSectorY )
{
	InternalEndQuest( ubQuest, sSectorX, sSectorY, TRUE );
}

void InternalEndQuest( UINT8 ubQuest, INT16 sSectorX, INT16 sSectorY, BOOLEAN fUpdateHistory )
{

#ifdef LUA_QUESTS
		LuaInternalQuest( ubQuest, sSectorX, sSectorY, fUpdateHistory, 0);
#else

	// SANDRO merc records - quests handled counter
	if ( gubQuest[ubQuest] != QUESTDONE && fUpdateHistory ) // only public quests
	{
		switch (ubQuest)
		{
			case QUEST_FOOD_ROUTE :
				// food quest is handled differently (completing when spoken to father Walker)
				break;
			case QUEST_DELIVER_LETTER :
			case QUEST_LEATHER_SHOP_DREAM :
				GiveQuestRewardPoint( sSectorX, sSectorY, 3, NO_PROFILE );
				break;
			case QUEST_ESCORT_SKYRIDER :
			case QUEST_CHOPPER_PILOT :
			case QUEST_FREE_CHILDREN :
			case QUEST_RESCUE_MARIA :
			case QUEST_FIND_SCIENTIST :
			case QUEST_DELIVER_VIDEO_CAMERA :
			case QUEST_FIND_HERMIT :
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE );
				break;
			case QUEST_FREE_DYNAMO :
				// don't give Dynamo reward for himself
				if( gMercProfiles[ DYNAMO ].bMercStatus != MERC_IS_DEAD )
					GiveQuestRewardPoint( sSectorX, sSectorY, 4, DYNAMO );
				break;
			case QUEST_KINGPIN_IDOL :
			case QUEST_CHITZENA_IDOL :
			case QUEST_HELD_IN_ALMA :
			case QUEST_RUNAWAY_JOEY :
			case QUEST_ESCORT_TOURISTS :
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE );
				break;
			case QUEST_ARMY_FARM :
			case QUEST_KINGPIN_MONEY :
				GiveQuestRewardPoint( sSectorX, sSectorY, 6, NO_PROFILE );
				break;
			case QUEST_BLOODCATS :
				GiveQuestRewardPoint( sSectorX, sSectorY, 7, NO_PROFILE );
				break;
			case QUEST_INTERROGATION : // we get here only if wiped out enemies after interrogation
			case QUEST_CREATURES :
				GiveQuestRewardPoint( sSectorX, sSectorY, 8, NO_PROFILE );
				break;
			case QUEST_KILL_TERRORISTS :  // only reduced experiences if we chosen Slay over Carmen
				if( gMercProfiles[ CARMEN ].bMercStatus == MERC_IS_DEAD )
					GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE );
				else
					GiveQuestRewardPoint( sSectorX, sSectorY, 9, NO_PROFILE );
				break;
#ifdef JA2UB
//off
#else
			case QUEST_KILL_DEIDRANNA :
				GiveQuestRewardPoint( sSectorX, sSectorY, 25, NO_PROFILE );
				break;
#endif
			default :
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE );
				break;
		}
	}

	if ( gubQuest[ubQuest ] == QUESTINPROGRESS )
	{	
		gubQuest[ubQuest] = QUESTDONE;

	if ( fUpdateHistory )
	{
		ResetHistoryFact( ubQuest, sSectorX, sSectorY );
	}
	}
	else
	{
		gubQuest[ubQuest] = QUESTDONE;
	}

	if ( ubQuest == QUEST_RESCUE_MARIA )
	{
		// cheap hack to try to prevent Madame Layla from thinking that you are
		// still in the brothel with Maria...
		gMercProfiles[ MADAME ].bNPCData = 0;
		gMercProfiles[ MADAME ].bNPCData2 = 0;
	}
	
#endif
	
#ifdef JA2UB	
	//if the quest is the FIX LAPTOP quest
	if( ubQuest == QUEST_FIX_LAPTOP && gGameUBOptions.LaptopQuestEnabled == TRUE )
	{
		//Set the fact that AIM and MERC should start selling
		gJa25SaveStruct.fHaveAimandMercOffferItems = TRUE;

		//Remeber that we should send email in the next sector
		gJa25SaveStruct.fSendEmail_10_NextSector = TRUE;

		AddEmail( EMAIL_PILOTMISSING, EMAIL_PILOTMISSING_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() ,-1,-1, TYPE_EMAIL_EMAIL_EDT);
		AddEmail( EMAIL_MAKECONTACT, EMAIL_MAKECONTACT_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() ,-1,-1, TYPE_EMAIL_EMAIL_EDT);

		//Merc and Aim emails
		AddEmail( EMAIL_AIM_PROMOTION_1, EMAIL_AIM_PROMOTION_1_LENGTH, AIM_SITE,  GetWorldTotalMin(),-1 ,-1, TYPE_EMAIL_EMAIL_EDT);
		AddEmail( EMAIL_MERC_PROMOTION_1, EMAIL_MERC_PROMOTION_1_LENGTH, SPECK_FROM_MERC,  GetWorldTotalMin(),-1,-1, TYPE_EMAIL_EMAIL_EDT);
		AddEmail( EMAIL_AIM_PROMOTION_2, EMAIL_AIM_PROMOTION_2_LENGTH, AIM_SITE,  GetWorldTotalMin(),-1 ,-1, TYPE_EMAIL_EMAIL_EDT);

		//Manuel
		{
			SOLDIERTYPE *pSoldier=NULL;

			pSoldier = FindSoldierByProfileID( MANUEL_UB , TRUE ); //MANUEL

			if( pSoldier != NULL )
			{
				//Add the Manuel email
				AddEmail( EMAIL_MANUEL, EMAIL_MANUEL_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() ,-1, -1, TYPE_EMAIL_EMAIL_EDT);
			}
		}

		//Miguel
		{
			//if miguel was dead, when importing the save
			if( gubFact[ FACT_PLAYER_IMPORTED_SAVE_MIGUEL_DEAD ] == FALSE )
			{
				//Add the miguel email
				AddEmail( EMAIL_MIGUELHELLO, EMAIL_MIGUELHELLO_LENGTH, MAIL_MIGUEL,  GetWorldTotalMin(),-1, -1, TYPE_EMAIL_EMAIL_EDT);
			}
		}

		//If any aim mercs were asked to send emails when the get back from duty elsewhere
		HandleAddingAnyAimAwayEmailsWhenLaptopGoesOnline();

		//Should we send the IMP reminder email when we go back online
		ShouldImpReminderEmailBeSentWhenLaptopBackOnline();

		//Force which ever of these emails that needed to be sent, to be sent
		HandleEmailBeingSentWhenEnteringSector( 0, 0, 0, TRUE );
	}
#endif
};

void InitQuestEngine()
{
	memset(gubQuest, 0, sizeof(gubQuest));
	memset(gubFact,	0, sizeof(gubFact));

	// semi-hack to make the letter quest start right away
	CheckForQuests( 1 );

	if ( gGameOptions.ubGameStyle == STYLE_SCIFI )
	{
		// 3 medical boosters
		gubCambriaMedicalObjects = 21;
	}
	else
	{
		gubCambriaMedicalObjects = 18;
	}

	gubBoxingMatchesWon = 0;
	gubBoxersRests = 0;
	gfBoxersResting = FALSE;
}



void CheckForQuests( UINT32 uiDay )
{
	// This function gets called at 8:00 AM time of the day

#ifdef TESTQUESTS
	ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Checking For Quests, Day %d", uiDay );
#endif

#ifdef JA2UB
 // -------------------------------------------------------------------------------
	// QUEST 23 : Detroy missles
	// -------------------------------------------------------------------------------
	// The game always starts with destrouy missles quest, so turn it on if it hasn't
	// already started
	if( gubQuest[ QUEST_DESTROY_MISSLES ] == QUESTNOTSTARTED )
	{
		StartQuest( QUEST_DESTROY_MISSLES, -1, -1 );
#ifdef TESTQUESTS
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Started DESTORY MISSLES quest");
#endif
	}
//Ja25: No deliver letter quest, dont start it
#else
	// -------------------------------------------------------------------------------
	// QUEST 0 : DELIVER LETTER
	// -------------------------------------------------------------------------------
	// The game always starts with DELIVER LETTER quest, so turn it on if it hasn't
	// already started
	if (gubQuest[QUEST_DELIVER_LETTER] == QUESTNOTSTARTED)
	{
		StartQuest( QUEST_DELIVER_LETTER, -1, -1 );
#ifdef TESTQUESTS

		if (!is_networked)
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Started DELIVER LETTER quest");
#endif
	}
#endif
	// This quest gets turned OFF through conversation with Miguel - when user hands
	// Miguel the letter
}


BOOLEAN SaveQuestInfoToSavedGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	//Save all the states if the Quests
	FileWrite( hFile, gubQuest, MAX_QUESTS, &uiNumBytesWritten );
	if( uiNumBytesWritten != MAX_QUESTS )
	{
		return(FALSE);
	}

	//Save all the states for the facts
	FileWrite( hFile, gubFact, NUM_FACTS, &uiNumBytesWritten );
	if( uiNumBytesWritten != NUM_FACTS )
	{
		return(FALSE);
	}


	return( TRUE );
}


BOOLEAN LoadQuestInfoFromSavedGameFile( HWFILE hFile, UINT8 MaxQuest )
{
	UINT32	uiNumBytesRead;

	//Save all the states if the Quests
	//FileRead( hFile, gubQuest, MAX_QUESTS, &uiNumBytesRead );
	//if( uiNumBytesRead != MAX_QUESTS )
	FileRead( hFile, gubQuest, MaxQuest, &uiNumBytesRead );
	if( uiNumBytesRead != MaxQuest )
	{
		return(FALSE);
	}

	//Save all the states for the facts
	FileRead( hFile, gubFact, NUM_FACTS, &uiNumBytesRead );
	if( uiNumBytesRead != NUM_FACTS )
	{
		return(FALSE);
	}

	return( TRUE );
}

// SANDRO - a function to add quest-done point to merc records, and award some exp possibly
void GiveQuestRewardPoint( INT16 sQuestSectorX, INT16 sQuestsSectorY, INT8 bExpReward, UINT8 bException )
{
	ScreenMsg( FONT_MCOLOR_LTBLUE, MSG_TESTVERSION, L"QUEST COMPLETED - Adding to merc records and awarding experiences (%d).", (bExpReward * gGameExternalOptions.usAwardSpecialExpForQuests) );

	for ( UINT8 i = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; i <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife >= CONSCIOUSNESS && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) && MercPtrs[ i ]->ubProfile != NO_PROFILE &&
			MercPtrs[ i ]->sSectorX == sQuestSectorX && MercPtrs[ i ]->sSectorY == sQuestsSectorY && !MercPtrs[ i ]->flags.fBetweenSectors && MercPtrs[ i ]->bTeam == gbPlayerNum &&
			MercPtrs[ i ]->bAssignment != IN_TRANSIT && MercPtrs[ i ]->bAssignment != ASSIGNMENT_DEAD && gMercProfiles[ MercPtrs[ i ]->ubProfile ].ubBodyType != 21 ) // != ROBOTNOWEAPON )
		{
			if ( MercPtrs[ i ]->ubProfile != bException )
			{
				gMercProfiles[ MercPtrs[ i ]->ubProfile ].records.ubQuestsHandled++;

				if ( bExpReward > 0 && gGameExternalOptions.usAwardSpecialExpForQuests > 0 )
					StatChange( MercPtrs[ i ], EXPERAMT, (bExpReward * gGameExternalOptions.usAwardSpecialExpForQuests), FALSE );
			}
		}
	}	
}







