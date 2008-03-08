#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Town Militia.h"
	#include "Militia Control.h"
	#include "Campaign Types.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "Overhead.h"
	#include "Strategic Town Loyalty.h"
	#include "Utilities.h"
	#include "random.h"
	#include "text.h"
	#include "Map Screen Interface.h"
	#include "Interface.h"
	#include "Laptopsave.h"
	#include "finances.h"
	#include "Game Clock.h"
	#include "Assignments.h"
	#include "squads.h"
	#include "Soldier Create.h"
	#include "Dialogue Control.h"
	#include "GameSettings.h"
	#include "Queen Command.h"
	#include "math.h"
	#include "Auto Resolve.h"
	#include "Vehicles.h"
	#include "Tactical Save.h"
#endif

#include "MilitiaSquads.h"
#include "Reinforcement.h"

// Debug defines

//#define DEBUG_SHOW_RATINGS
#define DEBUG_RATINGS_CONDITION !fForBattle


// will create a squad each n hours (12 for example). Should divide 24 without remainder
//#define CREATE_EACH_N_HOURS 12

// more these values - less the chances
#define CHANCE_TO_MOVE_A_SQUAD 5
#define CHANCE_TO_MOVE_AN_UNFULL_SQUAD 40

//#define DIV_OF_ORIGINAL_MILITIA 2
#define BASE_DIR_PRIORITY 5
// little chance to move if below minimum

//#define MINIMUM_MILITIA_SQUAD_SIZE 10
// should be 2* Minimum
//#define MAXIMUM_MILITIA_SQUAD_SIZE 30
 //#define MAXIMUM_MILITIA_SQUAD_SIZE_BATTLE MAXIMUM_MILITIA_SQUAD_SIZE

#define DIR_WITH_UNFULL_SQUAD_RATING_BONUS 100
// rating of battle, when there's equal number of soldiers
//#define DIR_RATING_OF_BATTLE 10
#define DIR_MIN_DIF 0.75

#define DIR_NORTH 0
#define DIR_EAST 1
#define DIR_SOUTH 2
#define DIR_WEST 3

INT32	iRestrictedSectorArraySize;
UINT32 gRestrictMilitia[256];

UINT8 gpAttackDirs[5][4]; // 0. Green Militia 1. Regular Militia 2. Elite Militia 3. Insertion code
UINT8 guiDirNumber = 0;
BOOLEAN gfMSBattle = FALSE;

INT16 sMSMapX, sMSMapY;

extern UINT8 GetTraversability( INT16 sStartSector, INT16 sEndSector );
extern void TriggerPrebattleInterface( UINT8 ubResult );
extern BOOLEAN gfEnterAutoResolveMode;


//Counts the number of live mercs in any given sector.
BOOLEAN PlayerMercsInSector_MSE( UINT8 ubSectorX, UINT8 ubSectorY, BOOLEAN fDontConsiderNextSector )
{
	GROUP *pGroup;
	PLAYERGROUP *pPlayer;
	UINT8 ubNumMercs = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( pGroup->fPlayer )// && !pGroup->fBetweenSectors)
		{
			if ( ( pGroup->ubSectorX == ubSectorX && pGroup->ubSectorY == ubSectorY && pGroup->ubSectorZ == 0 ) ||
				( !fDontConsiderNextSector && ( pGroup->ubNextX == ubSectorX && pGroup->ubNextY == ubSectorY && pGroup->ubSectorZ == 0 ) ) )
			{
				//we have a group, make sure that it isn't a group containing only dead members.
				pPlayer = pGroup->pPlayerList;
				while( pPlayer )
				{
					// robots count as mercs here, because they can fight, but vehicles don't
					if( ( pPlayer->pSoldier->stats.bLife ) && !( pPlayer->pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
					{
						ubNumMercs++;
					}
					pPlayer = pPlayer->next;
				}
			}
		}
		pGroup = pGroup->next;
	}
	return ubNumMercs;
}


// get any enemy group id in the sector (if no group, will return 0 )
UINT8 GetEnemyGroupIdInSector( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubRes = 0;
	GROUP *curr;
	curr = gpGroupList;
	while( curr )
	{
		if( curr->ubSectorX == sMapX && curr->ubSectorY == sMapY && !curr->fPlayer )
			if( !curr->ubGroupID )
				return curr->ubGroupID;
			else
				ubRes = curr->ubGroupID;
		curr = curr->next;
	}
	return ubRes;
}


// Count all militia in the sector
UINT8 CountMilitia(SECTORINFO *pSectorInfo)
{
	return pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA]+
	pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]+
	pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
}

// Creates militia at destination sector. The type and amount of militia depends on current sector's miltia type and amount
void GenerateMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);
	StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);
	StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);

	pSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	while( CountMilitia(pSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSize )
	{
		if(pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
		}
		else if(pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
		}
		else if(pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}
	}

	// Update the militia if the current sector is affected
	if (gfStrategicMilitiaChangesMade)
	{
		ResetMilitia();
	}
}

// Creates militia at destination sector and removes it from starting sector
void MoveMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY, BOOLEAN fAlternativeMax )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	SECTORINFO *pTSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
	StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
	StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

	StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
	StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
	StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

	while( !fAlternativeMax && CountMilitia(pTSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSize ||
		fAlternativeMax && CountMilitia(pTSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSizeBattle )
	{
		if(pTSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
		}
		else if(pTSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
		}
		else if(pTSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}
	}

	if (gfStrategicMilitiaChangesMade)
	{
		ResetMilitia();
	}
}

BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
//	SECTORINFO *pTSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	if( pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, 1 );
		return TRUE;
	}
	if( pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, 1 );
		return TRUE;
	}
	if( pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, 1 );
		return TRUE;
	}
	return FALSE;
}

// Generate rating for a direction
UINT16 CountDirectionEnemyRating( INT16 sMapX, INT16 sMapY, UINT8 uiDir )
{
	DOUBLE ddMinAngle, ddMaxAngle;
	DOUBLE ddRes = 0;
	DOUBLE pix2 = 3.1415926 * 2; // pi * 2
	//UINT16 uiSector = 0;
	INT16 sLMX, sLMY;

	switch(uiDir)
	{
	case DIR_NORTH:
		ddMinAngle = -2.3561944; // -135 degrees
		ddMaxAngle = -0.7853981; // -45 degrees
		break;
	case DIR_EAST:
		ddMinAngle = -0.7853981; // -45 degrees
		ddMaxAngle = 0.7853981; // 45 degrees
		break;
	case DIR_SOUTH:
		ddMinAngle = 0.7853981; // 45 degrees
		ddMaxAngle = 2.3561944; // 135 degrees
		break;
	case DIR_WEST:
		ddMinAngle = 2.3561944; // 135 degrees
		ddMaxAngle = -2.3561944 + pix2; // 225 degrees(-135)
		break;
	default:
		return 0;
	}

	//for( uiSector = 0 ; uiSector < 256 ; ++uiSector )
	for( sLMX = 0; sLMX < 16 ; ++sLMX )
		for( sLMY = 0; sLMY < 16 ; ++sLMY )
	{
//		SECTORINFO *pSectorInfo = &( SectorInfo[ uiSector ] );
		UINT8 uiSumOfEnemyTroops = NumEnemiesInSector( sLMX, sLMY );
			//pSectorInfo->ubNumAdmins + pSectorInfo->ubNumTroops + pSectorInfo->ubNumElites;

		// there's an enemy
		if( uiSumOfEnemyTroops )
		{
			DOUBLE ddAngle;

			if( sLMX - sMapX == 0 )ddAngle = ( sLMY - sMapY > 0 ) ? 1.57079632 : -1.57079632;
				else ddAngle = atan2( (double)(sLMY - sMapY), (double)(sLMX - sMapX) );

			if( ddAngle < 0 && uiDir == DIR_WEST )ddAngle += pix2;

			// is it in a right direction?
			if( ddAngle >= ddMinAngle && ddAngle <= ddMaxAngle )
			{
				DOUBLE ddDistance = sqrt( pow( (double)(sLMY - sMapY), 2 ) + pow( (double)(sLMX - sMapX), 2 ) );

				ddRes += (DOUBLE)uiSumOfEnemyTroops / pow( ddDistance, 2 );

			}
		}
	}

	return (UINT16) ddRes;
}

UINT16 CountDirectionRating( INT16 sMapX, INT16 sMapY, UINT8 uiDir )
{
	INT32 iRes = BASE_DIR_PRIORITY + CountDirectionEnemyRating( sMapX, sMapY, uiDir );

	INT16 sDMapX, sDMapY;
	INT32 iDiff;

	switch(uiDir)
	{
	case DIR_NORTH:
		sDMapX = sMapX;
		sDMapY = sMapY - 1;
		break;
	case DIR_EAST:
		sDMapX = sMapX + 1;
		sDMapY = sMapY;
		break;
	case DIR_SOUTH:
		sDMapX = sMapX;
		sDMapY = sMapY + 1;
		break;
	case DIR_WEST:
		sDMapX = sMapX - 1;
		sDMapY = sMapY;
		break;
	default:
		return iRes;
	}

	if( CountAllMilitiaInSector( sDMapX, sDMapY ) &&
		(UINT32)( CountAllMilitiaInSector( sDMapX, sDMapY ) + CountAllMilitiaInSector( sMapX, sMapY ) ) <= gGameExternalOptions.guiMaxMilitiaSquadSize )
		iRes += DIR_WITH_UNFULL_SQUAD_RATING_BONUS;

	if( NumEnemiesInSector( sDMapX, sDMapY	) )
	{
	//	if( GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR )
			iDiff = (INT32)( (FLOAT)iRes * ( (FLOAT)CountAllMilitiaInFiveSectors( sDMapX, sDMapY ) / (FLOAT)NumEnemiesInFiveSectors( sDMapX, sDMapY ) ) );
	//	else
	//		iDiff = iRes * ( (FLOAT)CountAllMilitiaInFiveSectors( sMapX, sMapY ) / DIV_OF_ORIGINAL_MILITIA / (FLOAT)NumEnemiesInFiveSectors( sDMapX, sDMapY ) );

		if( iDiff > (INT32)( (FLOAT)DIR_MIN_DIF * (FLOAT)iRes ) )
			iRes = iDiff;
		else
			iRes = 0;
	}


// There's player's mercs! Go there
	if( PlayerMercsInSector_MSE( (UINT8)sDMapX, (UINT8)sDMapY, FALSE ) )
		iRes += 15000;// should be enough

//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"M %ld, E%ld,%ld %ld, Diff %ld", CountAllMilitiaInSector( sMapX, sMapY ), sDMapX, sDMapY, NumEnemiesInSector( sDMapX, sDMapY ), ((INT32)CountAllMilitiaInSector( sMapX, sMapY )) - ((INT32)NumEnemiesInSector( sDMapX, sDMapY )) );

	return iRes;
}


// Standard conditions the direction should fit
BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors )
{
	UINT8 uiTravab = GetTraversability( SECTOR( sSMapX , sSMapY ) , SECTOR( sMapX, sMapY ) );

 	if( uiTravab == GROUNDBARRIER || uiTravab == EDGEOFWORLD ) return FALSE;

	if( !fForBattle && gfMSBattle && NumEnemiesInSector( sMapX, sMapY ) ) return FALSE;

	return (GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR || fWithCities) && !IsThisSectorASAMSector(	sMapX, sMapY , 0 ) &&
		(GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR || !fOnlyCitySectors);// &&
//		( !NumEnemiesInSector( sMapX, sMapY ) || GetEnemyGroupIdInSector(sMapX, sMapY ) || fForBattle ) &&
//		( fForBattle || CountAllMilitiaInSector( sMapX, sMapY ) < gGameExternalOptions.guiMaxMilitiaSquadSize || PlayerMercsInSector( sMapX, sMapY, 0 ) )
}

void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors )
{
	*uiDirNumber = 0;

#ifdef DEBUG_SHOW_RATINGS
	if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld-------------------", sMapX, sMapY );
#endif

	if(CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, fWithCities, fForBattle, fOnlyCitySectors ) )
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY - 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_NORTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_NORTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX - 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX - 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_WEST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_WEST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of west direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX, sMapY + 1, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY + 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_SOUTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_SOUTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of south direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX + 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX + 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_EAST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_EAST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of east direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}

}

UINT8 pBlockMoveList[256];
UINT8 uiBlockMoveListPointer;

void AddToBlockMoveList(INT16 sMapX, INT16 sMapY)
{
	pBlockMoveList[ uiBlockMoveListPointer++ ] = SECTOR( sMapX, sMapY );
}

void ClearBlockMoveList()
{
	uiBlockMoveListPointer = 0;
}

BOOLEAN CheckInBlockMoveList(INT16 sMapX, INT16 sMapY)
{
	UINT16 i;
	for( i = 0; i < uiBlockMoveListPointer ; ++i)
		if( sMapX == SECTORX(pBlockMoveList[i]) && sMapY == SECTORY(pBlockMoveList[i]) )return TRUE;

	return FALSE;
}

void UpdateMilitiaSquads(INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT32 iRandomRes = 0, iRandom = 0;
	UINT8 x;//,y;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 uiMilitiaCount;


	// If we don't want roaming militia
	// We shouldn't be here.
	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	if( sMapX == 1 && sMapY == 1 )
	{
		ClearBlockMoveList();
		gfMSBattle = FALSE;
	}

	if( CheckInBlockMoveList( sMapX, sMapY ) )return;

	uiMilitiaCount = CountMilitia(pSectorInfo);

	if( !uiMilitiaCount )return;

	// Kaiden: Moved Create Militia code into CreateMilitiaSquads
	// Removed the code from here and added the procedure call
	// If it's time, and this is a city sector create a new group.
	// But only if we're not training our own.

	if( (!gGameExternalOptions.gfmusttrainroaming) && ( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ) )
	{
		if (GetWorldDay( ) < gGameExternalOptions.guiAllowMilitiaGroupsDelay)
			return;

		if( GetWorldHour() % gGameExternalOptions.guiCreateEachNHours )
			return;

		CreateMilitiaSquads( sMapX, sMapY );
		if (gfStrategicMilitiaChangesMade)
		{
			ResetMilitia();
		}
	}


	// moving squad, if it is not a SAM site
	if( ( GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR ) && (!IsThisSectorASAMSector(	sMapX, sMapY, 0 )) )
	{
		if( !PlayerMercsInSector_MSE( (UINT8)sMapX, (UINT8)sMapY, FALSE ) ) // and there's no player's mercs in the sector
		{
			if( GetWorldHour() % 2 )return;

			memset(pMoveDir, 0, sizeof(pMoveDir));
			GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, FALSE, FALSE );

			if( uiDirNumber )
			{
				for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];
			//			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld", x, pMoveDir[x][1]);

				iRandom = Random( pMoveDir[ uiDirNumber - 1 ][1] +
					( uiMilitiaCount >= gGameExternalOptions.guiMinMilitiaSquadSize ? CHANCE_TO_MOVE_A_SQUAD : CHANCE_TO_MOVE_AN_UNFULL_SQUAD ) );

				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Roll %ld", iRandomRes);

				iRandomRes = 256;

				for( x = 0; x < uiDirNumber; ++x)
					if( iRandom < pMoveDir[x][1] )
					{
						iRandomRes = x;
						break;
					}

					// "Decided" to stay here
					if(iRandomRes >= uiDirNumber)
						return;

					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld:Dir count %ld, Rand %ld. Go to %ld,%ld. Have %ld militia men", sMapX, sMapY, uiDirNumber, iRandomRes, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ), uiMilitiaCount);
					//Kaiden: if Restricted Sectors List option is turned on,
					// militia can't move to any sectors in the list.
					// Unless they are following a group of mercs.
					if (gGameExternalOptions.gflimitedRoaming)
						for (INT32 i=0;i<iRestrictedSectorArraySize; ++i)
						{
							DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RestrictedSectors, %d", gRestrictMilitia[i]));
							if(pMoveDir[iRandomRes][0] == gRestrictMilitia[i])
								return;
						}

					// WDS bug fix for moving militia
					int targetX = SECTORX( pMoveDir[ iRandomRes ][0] );
					int targetY = SECTORY( pMoveDir[ iRandomRes ][0] );
					Assert(targetX >= 0 && targetX < MAP_WORLD_X);
					Assert(targetY >= 0 && targetY < MAP_WORLD_Y);
					MoveMilitiaSquad( sMapX, sMapY,  targetX, targetY, FALSE );
					AddToBlockMoveList( targetX, targetY );

					if ( gfStrategicMilitiaChangesMade)
					{
						ResetMilitia();
					}

					if( NumEnemiesInSector( targetX, targetY ) )
					{
						extern GROUP *gpBattleGroup;
						gpBattleGroup = GetGroup( GetEnemyGroupIdInSector( targetX, targetY ) );
		/*				GROUP* pEnemyGroup = GetGroup( GetEnemyGroupIdInSector( targetX, targetY ) );

						if(pEnemyGroup && pEnemyGroup->ubGroupID)
						{
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Attacking from %ld,%ld to %ld,%ld - enemy's group id %ld", sMapX, sMapY, targetX, SECTORY( pMoveDir[ iRandomRes ][0], pEnemyGroup->ubGroupID ));
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Arrival 1, Arrival 2");

							pEnemyGroup->ubPrevX = sMapX;
							pEnemyGroup->ubPrevY = sMapY;

							pEnemyGroup->ubNextX = targetX;
							pEnemyGroup->ubNextY = targetY;
		*/
							gfMSBattle = TRUE;

			//				GroupArrivedAtSector( pEnemyGroup->ubGroupID , TRUE, FALSE );
							EnterAutoResolveMode( targetX,  targetY );
		//				}
					}
			}
					else return;
		}
	}
}

	// Kaiden: Roaming Militia Training:
	// If we're training roaming militia,
	// we'll get our squad from here:
	// Don't need to check for delay, as this function won't be
	// called if there is a delay set.
void CreateMilitiaSquads(INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT32 iRandomRes = 0;
	UINT8 x;//,y;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 uiMilitiaCount;


	// If we're not allowing roaming groups,
	// then we're not creating them either.
	// Remove this if you want to create them
	// for some other reason. But you also have to
	// remove the same line in UpdateMilitiaSquads
	// Or the ones you create won't move.
	// This function is only called from UpdateMilitiaSquads
	// and TownMilitiaTrainingCompleted
	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	if( sMapX == 1 && sMapY == 1 )
	{
		ClearBlockMoveList();
		gfMSBattle = FALSE;
	}

	if( CheckInBlockMoveList( sMapX, sMapY ) )return;

	uiMilitiaCount = CountMilitia(pSectorInfo);

	if( !uiMilitiaCount )return;

	// Create new Militia Squad

		if( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR )
		{
			GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, FALSE, FALSE );

			if(uiDirNumber)// && Random(100) < CHANCE_TO_GENERATE_A_SQUAD)
			{
				for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];

				iRandomRes = Random( pMoveDir[ uiDirNumber - 1 ][1] );

				for( x = 0; x < uiDirNumber; ++x)
					if( iRandomRes < pMoveDir[x][1] )
					{
						iRandomRes = x;
						break;
					}

				// shouldn't be!
				if(iRandomRes >= uiDirNumber)iRandomRes = 0;

				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld:Dir count %ld, Rand %ld. Go to %ld,%ld", sMapX, sMapY, uiDirNumber, iRandomRes, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ));

				GenerateMilitiaSquad( sMapX, sMapY,	SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );
				AddToBlockMoveList( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );

				if( NumEnemiesInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) && CountAllMilitiaInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) )
				{
	/*				GROUP* pEnemyGroup = GetGroup( GetEnemyGroupIdInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) );

					if(pEnemyGroup && pEnemyGroup->ubGroupID)
					{
						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Attacking from %ld,%ld to %ld,%ld - enemy's group id %ld", sMapX, sMapY, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0], pEnemyGroup->ubGroupID ));
						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Arrival 1, Arrival 2");

						pEnemyGroup->ubPrevX = sMapX;
						pEnemyGroup->ubPrevY = sMapY;

						pEnemyGroup->ubNextX = SECTORX( pMoveDir[ iRandomRes ][0] );
						pEnemyGroup->ubNextY = SECTORY( pMoveDir[ iRandomRes ][0] );
	*/
						gfMSBattle = TRUE;
	//					GroupArrivedAtSector( pEnemyGroup->ubGroupID , TRUE, FALSE );

						EnterAutoResolveMode( SECTORX( pMoveDir[ iRandomRes ][0] ),	SECTORY( pMoveDir[ iRandomRes ][0] ) );
	//				}
				}

			}
				else return;
		}
	return;
}










extern BOOLEAN gfMSResetMilitia;

void DoMilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT8 x;
	UINT8 uiNumGreen = 0, uiNumReg = 0, uiNumElite = 0;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	BOOLEAN fMoreTroopsLeft[4] = {FALSE,FALSE,FALSE,FALSE};
	BOOLEAN fFirstLoop = TRUE;
	BOOLEAN fMilitiaMoved = FALSE;

	guiDirNumber = 0;

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir,
		( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ? TRUE : FALSE ), TRUE, FALSE );

	ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

	gpAttackDirs[ guiDirNumber ][0] = uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
	gpAttackDirs[ guiDirNumber ][1] = uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
	gpAttackDirs[ guiDirNumber ][2] = uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
	gpAttackDirs[ guiDirNumber ][3] = INSERTION_CODE_CENTER;

	guiDirNumber = uiDirNumber	+ 1;

	x = 0;
	while( CountMilitia(pSectorInfo ) < gGameExternalOptions.guiMaxMilitiaSquadSizeBattle &&
		( fMoreTroopsLeft[0] || fMoreTroopsLeft[1] || fMoreTroopsLeft[2] || fMoreTroopsLeft[3] || fFirstLoop ) )
	{
		fMoreTroopsLeft[ x ] = MoveOneBestMilitiaMan( SECTORX( pMoveDir[ x ][0] ), SECTORY( pMoveDir[ x ][0] ), sMapX, sMapY );

		if( fMoreTroopsLeft[ x ] )
		{
			fMilitiaMoved = TRUE;

			gpAttackDirs[ x + 1 ][0] += pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] - uiNumGreen;
			gpAttackDirs[ x + 1 ][1] += pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA] - uiNumReg;
			gpAttackDirs[ x + 1 ][2] += pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] - uiNumElite;
			gpAttackDirs[ x + 1 ][3] = (UINT8)pMoveDir[ x ][2];

			uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
			uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
			uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}

		++x;
		if( x >= uiDirNumber )
		{
			x = 0;
			fFirstLoop = FALSE;
		}
	}

	// If militia have been moved here, no reason to reset--just add them.	If militia have not moved, then no strategic
	// changes were made.	Either case, this flag should be false.
	gfStrategicMilitiaChangesMade = FALSE;
	}

void MSCallBack( UINT8 ubResult )
{
	if( ubResult == MSG_BOX_RETURN_YES )
	{
		gTacticalStatus.uiFlags |= WANT_MILITIA_REINFORCEMENTS;
		DoMilitiaHelpFromAdjacentSectors( sMSMapX, 	sMSMapY );
	}
}

BOOLEAN IsThereMilitiaInAdjacentSector( INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT8 x;
	BOOLEAN fResult = FALSE;

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir,
		( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ? TRUE : FALSE ), TRUE, FALSE );

	for( x = 0; x < uiDirNumber ; ++x )
		if( CountAllMilitiaInSector( SECTORX( pMoveDir[ x ][ 0 ] ), SECTORY( pMoveDir[ x ][ 0 ] ) ) )
			fResult = TRUE;

	return fResult;
}

void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY )
{
	sMSMapX = sMapX;
	sMSMapY = sMapY;

	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	gTacticalStatus.uiFlags &= (~WANT_MILITIA_REINFORCEMENTS);

	guiDirNumber = 0;

//	if( CountAllMilitiaInSector( sMapX, sMapY ) ) MSCallBack( MSG_BOX_RETURN_YES );

	// This is no longer a question of simply whether to have a full militia count, but also whether we want
	// reinforcements.	So if there are any available, always ask.
	if( IsThereMilitiaInAdjacentSector( sMapX, sMapY ) ) // && CountAllMilitiaInSector( sMapX, sMapY ) < gGameExternalOptions.guiMaxMilitiaSquadSizeBattle )
		DoScreenIndependantMessageBox( gzCWStrings[0], MSG_BOX_FLAG_YESNO, MSCallBack );
}

void MilitiaFollowPlayer( INT16 sMapX, INT16 sMapY, INT16 sDMapX, INT16 sDMapY )
{
	if( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ||
		IsThisSectorASAMSector( sMapX, sMapY, 0 ) ||
		PlayerMercsInSector_MSE( (UINT8)sMapX, (UINT8)sMapY, TRUE ) )return;

	if( GetTownIdForSector( sDMapX, sDMapY ) != BLANK_SECTOR ||
		IsThisSectorASAMSector( sDMapX, sDMapY, 0 ) )return;


	MoveMilitiaSquad( sMapX, sMapY, sDMapX, sDMapY, FALSE );
}
