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
	#include "Assignments.h"
	#include "squads.h"
	#include "Soldier Create.h"
	#include "Dialogue Control.h"
	#include "Queen Command.h"
	#include "GameSettings.h"
	#include "Soldier Init List.h"
	#include "Inventory Choosing.h"
#endif

#include "GameInitOptionsScreen.h"

#include "connect.h"
#include "Reinforcement.h"
#include "MilitiaSquads.h"

UINT8 gubReinforcementMinEnemyStaticGroupSize = 12;
UINT32 guiMilitiaReinforceTurn = 0, guiMilitiaArrived = 0;//dnl ch68 090913

void GetNumberOfEnemiesInFiveSectors( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites, UINT8 *pubNumRobots, UINT8 *pubNumTanks, UINT8 *pubNumJeeps )
{
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps;
	UINT16 pusMoveDir[4][3];	//first column in this matrix is number of sector, except for 4th row
	UINT8 ubDirNumber, ubIndex;
	
	GetNumberOfStationaryEnemiesInSector( sSectorX, sSectorY, pubNumAdmins, pubNumTroops, pubNumElites, pubNumRobots, pubNumTanks, pubNumJeeps );

	GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( sSectorX, sSectorY, ENEMY_TEAM, &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumRobots, &ubNumTanks, &ubNumJeeps );

	*pubNumAdmins += ubNumAdmins;
	*pubNumTroops += ubNumTroops;
	*pubNumElites += ubNumElites;
	*pubNumRobots += ubNumRobots;
	*pubNumTanks += ubNumTanks;
	*pubNumJeeps += ubNumJeeps;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return;

	if ( ( GetTownIdForSector( sSectorX, sSectorY ) == OMERTA )&& ( !zDiffSetting[gGameOptions.ubDifficultyLevel].bAllowReinforcementsOmerta ) ) //Madd: skip Omerta //Lal: but not on insane ;-)
		return;
	
	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ++ubIndex )
	{	//take number of the involved sector, find its X and Y coordintes and then ask for number of troops there
		GetNumberOfStationaryEnemiesInSector( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumRobots, &ubNumTanks, &ubNumJeeps );

		while ( ubNumRobots + ubNumElites + ubNumTroops + ubNumAdmins + ubNumTanks + ubNumJeeps > gubReinforcementMinEnemyStaticGroupSize )  //count how many of static group will reinforce the battle, but leave minimal group size to guard
		{
			if ( ubNumRobots )
			{
				*pubNumRobots += 1;
				ubNumRobots--;
			}
			else if( ubNumElites )
			{
				*pubNumElites += 1;
				ubNumElites--;
			}
			else if( ubNumTroops )
			{
				*pubNumTroops += 1;
				ubNumTroops--;
			}
			else if( ubNumAdmins )
			{
				*pubNumAdmins += 1;
				ubNumAdmins--;
			}
			else if( ubNumTanks )
			{
				*pubNumTanks += 1;
				ubNumTanks--;
			}
			else if ( ubNumJeeps )
			{
				*pubNumJeeps += 1;
				ubNumJeeps--;
			}
		}

		GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), ENEMY_TEAM, &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumRobots, &ubNumTanks, &ubNumJeeps );

		*pubNumAdmins += ubNumAdmins;
		*pubNumTroops += ubNumTroops;
		*pubNumElites += ubNumElites;
		*pubNumRobots += ubNumRobots;
		*pubNumTanks += ubNumTanks;
		*pubNumJeeps += ubNumJeeps;
	}
}

void ActivateTurncoatsForAutoResolve( INT16 sSectorX, INT16 sSectorY )
{
	if (!gSkillTraitValues.fCOTurncoatsActivateInAutoResolve)
		return;

	if (!gGameExternalOptions.gfAllowReinforcements)
		return;

	if ( ( GetTownIdForSector( sSectorX, sSectorY ) == OMERTA )&& ( !zDiffSetting[gGameOptions.ubDifficultyLevel].bAllowReinforcementsOmerta ) ) //Madd: skip Omerta //Lal: but not on insane ;-)
		return;
	
	UINT16 ubAdminTurncoats = NumTurncoatsOfClassInSector(sSectorX, sSectorY, SOLDIER_CLASS_ADMINISTRATOR);
	UINT16 ubTroopTurncoats = NumTurncoatsOfClassInSector(sSectorX, sSectorY, SOLDIER_CLASS_ARMY);
	UINT16 ubEliteTurncoats = NumTurncoatsOfClassInSector(sSectorX, sSectorY, SOLDIER_CLASS_ELITE);

	for (int i = 0; i < ubAdminTurncoats; ++i) RemoveOneTurncoat(sSectorX, sSectorY, SOLDIER_CLASS_ADMINISTRATOR, TRUE);
	for (int i = 0; i < ubTroopTurncoats; ++i) RemoveOneTurncoat(sSectorX, sSectorY, SOLDIER_CLASS_ARMY, TRUE);
	for (int i = 0; i < ubEliteTurncoats; ++i) RemoveOneTurncoat(sSectorX, sSectorY, SOLDIER_CLASS_ELITE, TRUE);

	StrategicAddMilitiaToSector(sSectorX, sSectorY, GREEN_MILITIA, ubAdminTurncoats);
	StrategicAddMilitiaToSector(sSectorX, sSectorY, REGULAR_MILITIA, ubTroopTurncoats);
	StrategicAddMilitiaToSector(sSectorX, sSectorY, ELITE_MILITIA, ubEliteTurncoats);

	UINT16 pusMoveDir[4][3];	//first column in this matrix is number of sector, except for 4th row
	UINT8 ubDirNumber;
	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for (UINT8 i = 0; i < ubDirNumber; ++i)
	{
		GROUP* pGroup = gpGroupList;
		while ( pGroup )
		{
			if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == SECTORX(pusMoveDir[i][0]) && pGroup->ubSectorY == SECTORY(pusMoveDir[i][0]) )
			{
				ubAdminTurncoats = pGroup->pEnemyGroup->ubNumAdmins_Turncoat;
				ubTroopTurncoats = pGroup->pEnemyGroup->ubNumTroops_Turncoat;
				ubEliteTurncoats = pGroup->pEnemyGroup->ubNumElites_Turncoat;

				for (int j = 0; j < ubAdminTurncoats; ++j) RemoveOneTurncoat(SECTORX(pusMoveDir[i][0]), SECTORY(pusMoveDir[i][0]), SOLDIER_CLASS_ADMINISTRATOR, TRUE);
				for (int j = 0; j < ubTroopTurncoats; ++j) RemoveOneTurncoat(SECTORX(pusMoveDir[i][0]), SECTORY(pusMoveDir[i][0]), SOLDIER_CLASS_ARMY, TRUE);
				for (int j = 0; j < ubEliteTurncoats; ++j) RemoveOneTurncoat(SECTORX(pusMoveDir[i][0]), SECTORY(pusMoveDir[i][0]), SOLDIER_CLASS_ELITE, TRUE);

				// since we're going directly to autoresolve, add miliita directly to sector after removing from a reinforcement group
				StrategicAddMilitiaToSector(sSectorX, sSectorY, GREEN_MILITIA, ubAdminTurncoats);
				StrategicAddMilitiaToSector(sSectorX, sSectorY, REGULAR_MILITIA, ubTroopTurncoats);
				StrategicAddMilitiaToSector(sSectorX, sSectorY, ELITE_MILITIA, ubEliteTurncoats);
			}

			pGroup = pGroup->next;
		}
	}
}

UINT8 NumEnemiesInFiveSectors( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps;

	GetNumberOfEnemiesInFiveSectors( sMapX, sMapY, &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumRobots, &ubNumTanks, &ubNumJeeps );
	
	return ubNumAdmins + ubNumTroops + ubNumElites + ubNumRobots + ubNumTanks + ubNumJeeps;
}

BOOLEAN IsGroupInARightSectorToReinforce( GROUP *pGroup, INT16 sSectorX, INT16 sSectorY )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;

	// silversurfer: A group that is already in the right sector is not reinforcement. It's either stationary or mobile. Reinforcements have to come from different sectors.
	// If we allow groups here that are already in the sector we will assign them twice (stationary/mobile + reinforcement)! Not good.
/*	if( pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		return TRUE;  //Well, it's in the same sector, so allow to reinforce*/

	if( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for ( ubIndex = 0; ubIndex < ubDirNumber; ++ubIndex )
	{
		if( pGroup->ubSectorX == SECTORX( pusMoveDir[ ubIndex ][ 0 ] ) && pGroup->ubSectorY == SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) )
			return TRUE;
	}

	return FALSE;
}

UINT8 GetAdjacentSectors( UINT8 pSectors[4], INT16 sSectorX, INT16 sSectorY )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;
	UINT8 ubCounter = 0;

	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ++ubIndex )
		pSectors[ ubCounter++ ] = (UINT8)pusMoveDir[ ubIndex ][ 0 ];

	return ubCounter;
}

UINT16 CountAllMilitiaInFiveSectors(INT16 sMapX, INT16 sMapY)
{
	UINT16 ubResult = 0;
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber, ubIndex;
	
	ubResult = NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM );

	if( !gGameExternalOptions.gfAllowReinforcements )
		return ubResult;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
		ubResult += NumNonPlayerTeamMembersInSector( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), MILITIA_TEAM );
	
	return ubResult;
}

BOOLEAN ARMoveBestMilitiaManFromAdjacentSector(INT16 sMapX, INT16 sMapY)
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber;
	UINT8 ubRandom;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	UINT8 militiainsector = NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM );
	if ( militiainsector >= gGameExternalOptions.iMaxMilitiaPerSector ||
		 CountAllMilitiaInFiveSectors( sMapX, sMapY ) - militiainsector == 0 )
		return FALSE;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	ubRandom = Random( ubDirNumber );

	while( !MoveOneBestMilitiaMan( SECTORX( pusMoveDir[ ubRandom ][ 0 ] ), SECTORY( pusMoveDir[ ubRandom ][ 0 ] ), sMapX, sMapY )  )
		ubRandom = Random( ubDirNumber );

	return TRUE;
}

GROUP* GetNonPlayerGroupInSector( INT16 sMapX, INT16 sMapY, UINT8 usTeam )
{
	GROUP *curr;
	curr = gpGroupList;
	while( curr )
	{
		if ( curr->ubSectorX == sMapX && curr->ubSectorY == sMapY && curr->usGroupTeam == usTeam && curr->ubGroupID )
			return curr;
		curr = curr->next;
	}
	return NULL;
}


UINT8 DoReinforcementAsPendingNonPlayer( INT16 sMapX, INT16 sMapY, UINT8 usTeam )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;
	GROUP *pGroup;
	//ENEMYGROUP *pEnemyGroup;
	SECTORINFO *pThisSector, *pSector;
	
	if( !gGameExternalOptions.gfAllowReinforcements )
		return 255;

	if ( usTeam == ENEMY_TEAM && ( GetTownIdForSector( sMapX, sMapY ) == OMERTA ) && (gGameOptions.ubDifficultyLevel != DIF_LEVEL_INSANE) ) //Madd: skip Omerta //Lal: but not on insane ;-)
		return 255;

	pThisSector = &SectorInfo[ SECTOR( sMapX, sMapY ) ];

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ++ubIndex )
	{
		while ( (pGroup = GetNonPlayerGroupInSector( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), usTeam )) != NULL )
		{
			pGroup->ubPrevX = pGroup->ubSectorX;
			pGroup->ubPrevY = pGroup->ubSectorY;

			pGroup->ubSectorX = pGroup->ubNextX = (UINT8)sMapX;
			pGroup->ubSectorY = pGroup->ubNextY = (UINT8)sMapY;

			if ( usTeam == ENEMY_TEAM )
			{
				gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				ResetMortarsOnTeamCount();
				ResetNumSquadleadersInArmyGroup(); // added by SANDRO	
			}
			else if ( usTeam == MILITIA_TEAM )
			{
				gfPendingNonPlayerTeam[MILITIA_TEAM] = TRUE;
			}
		}
	}

	while( ubDirNumber > 0 )
	{
		ubIndex = Random(ubDirNumber);

		INT16 sOtherX = SECTORX( pusMoveDir[ubIndex][0] );
		INT16 sOtherY = SECTORY( pusMoveDir[ubIndex][0] );

		if ( usTeam == ENEMY_TEAM && NumNonPlayerTeamMembersInSector( sOtherX, sOtherY, usTeam ) > gubReinforcementMinEnemyStaticGroupSize )
		{
			pSector = &SectorInfo[ pusMoveDir[ ubIndex ][ 0 ] ];

			if( pSector->ubNumRobots )
			{
				(pThisSector->ubNumRobots)++;
				(pSector->ubNumRobots)--;
				(pThisSector->ubRobotsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 0, 0, 1, 0, 0, FALSE );
			}
			else if( pSector->ubNumElites )
			{
				(pThisSector->ubNumElites)++;
				(pSector->ubNumElites)--;
				(pThisSector->ubElitesInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 0, 1, 0, 0, 0, FALSE );
			}
			else if( pSector->ubNumTroops )
			{
				(pThisSector->ubNumTroops)++;
				(pSector->ubNumTroops)--;
				(pThisSector->ubTroopsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 1, 0, 0, 0, 0, FALSE );
			}
			else if( pSector->ubNumAdmins )
			{
				(pThisSector->ubNumAdmins)++;
				(pSector->ubNumAdmins)--;
				(pThisSector->ubAdminsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 1, 0, 0, 0, 0, 0, FALSE );
			}
			else if( pSector->ubNumTanks )
			{
				(pThisSector->ubNumTanks)++;
				(pSector->ubNumTanks)--;
				(pThisSector->ubTanksInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 0, 0, 0, 1, 0, FALSE );
			}
			else if ( pSector->ubNumJeeps )
			{
				(pThisSector->ubNumJeeps)++;
				(pSector->ubNumJeeps)--;
				(pThisSector->ubJeepsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 0, 0, 0, 0, 1, FALSE );
			}

			return (UINT8)pusMoveDir[ ubIndex ][ 2 ];
		}
		// no required min size on militia groups
		else if ( usTeam == MILITIA_TEAM && NumNonPlayerTeamMembersInSector( sOtherX, sOtherY, usTeam ) )
		{
			if ( MilitiaInSectorOfRank( sOtherX, sOtherY, ELITE_MILITIA ) )
			{
				StrategicAddMilitiaToSector( sMapX, sMapY, ELITE_MILITIA, 1 );
				StrategicRemoveMilitiaFromSector( sOtherX, sOtherY, ELITE_MILITIA, 1 );
				AddMilitiaToBattle( NULL, (UINT8)pusMoveDir[ubIndex][2], 0, 0, 1, FALSE );
			}
			else if ( MilitiaInSectorOfRank( sOtherX, sOtherY, REGULAR_MILITIA ) )
			{
				StrategicAddMilitiaToSector( sMapX, sMapY, REGULAR_MILITIA, 1 );
				StrategicRemoveMilitiaFromSector( sOtherX, sOtherY, REGULAR_MILITIA, 1 );
				AddMilitiaToBattle( NULL, (UINT8)pusMoveDir[ubIndex][2], 0, 1, 0, FALSE );
			}
			else if ( MilitiaInSectorOfRank( sOtherX, sOtherY, GREEN_MILITIA ) )
			{
				StrategicAddMilitiaToSector( sMapX, sMapY, GREEN_MILITIA, 1 );
				StrategicRemoveMilitiaFromSector( sOtherX, sOtherY, GREEN_MILITIA, 1 );
				AddMilitiaToBattle( NULL, (UINT8)pusMoveDir[ubIndex][2], 1, 0, 0, FALSE );
			}

			return (UINT8)pusMoveDir[ubIndex][2];
		}
		else
		{
			// If there are no reinforcements from this direction, remove this as a direction
			memcpy( &(pusMoveDir[ubIndex][0]), &(pusMoveDir[ --ubDirNumber][0]), sizeof(pusMoveDir[0]));
		}
	}

	// No reinforcements from other sectors are available
	return 255;
}

UINT8 DoReinforcementAsPendingMilitia( INT16 sMapX, INT16 sMapY, UINT8 *pubRank )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return 255;

	//	pThisSector = &SectorInfo[ SECTOR( sMapX, sMapY ) ];

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	if ( CountAllMilitiaInFiveSectors( sMapX, sMapY ) - NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) == 0 )
	{
		*pubRank = 255;
		return 255;
	}
	else
	{
		for(;;)
		{
			ubIndex = Random(ubDirNumber);
			if ( NumNonPlayerTeamMembersInSector( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), MILITIA_TEAM ) )
			{
				if ( MilitiaInSectorOfRank( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), ELITE_MILITIA ) )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,ELITE_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),ELITE_MILITIA, 1 );
					*pubRank = ELITE_MILITIA;
				}
				else if ( MilitiaInSectorOfRank( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), REGULAR_MILITIA ) )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,REGULAR_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),REGULAR_MILITIA, 1 );
					*pubRank = REGULAR_MILITIA;
				}
				else if ( MilitiaInSectorOfRank( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), GREEN_MILITIA ) )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,GREEN_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),GREEN_MILITIA, 1 );
					*pubRank = GREEN_MILITIA;
				}

				return (UINT8)pusMoveDir[ ubIndex ][ 2 ];
			}
		}
	}
}


void AddPossiblePendingMilitiaToBattle()
{
	UINT8 ubSlots;
	UINT8 ubNumElites, ubNumRegulars, ubNumGreens;
	static UINT8 ubPredefinedInsertionCode = 255;
	static UINT8 ubPredefinedRank = 255;

	// Check if no world is loaded, and is not underground level
	if(!(gWorldSectorX > 0 && gWorldSectorY > 0 && gbWorldSectorZ == 0))//dnl ch57 161009
		return;

	// Haydent
	if (is_networked)
	{
		if ((is_client && !is_server) || gMilitiaEnabled == 0)
		{
			return;
		}
	}

	//	if( !PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, 0 ) || !CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY ) 
	//		|| !NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, ENEMY_TEAM ) ) return;
	if( (PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, 0 ) == 0) 
		|| !(gTacticalStatus.uiFlags & WANT_MILITIA_REINFORCEMENTS)
		|| (NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, ENEMY_TEAM ) == 0)
		) 
		return;

	//gGameExternalOptions.guiMaxMilitiaSquadSize - CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY );
	ubSlots = NumFreeSlots( MILITIA_TEAM );
	if(gGameExternalOptions.sMinDelayMilitiaReinforcements)//dnl ch68 090913
	{
		if(gTacticalStatus.Team[MILITIA_TEAM].bAwareOfOpposition == TRUE)
		{
			if(guiMilitiaReinforceTurn == 0)
			{
				guiMilitiaReinforceTurn = guiTurnCnt + gGameExternalOptions.sMinDelayMilitiaReinforcements + Random(gGameExternalOptions.sRndDelayMilitiaReinforcements+1);// first possible reinforcement
				ubSlots = 0;
			}
			else if(guiTurnCnt >= guiMilitiaReinforceTurn)
			{
				guiMilitiaReinforceTurn = guiTurnCnt + gGameExternalOptions.sMinDelayMilitiaReinforcements/3 + Random(gGameExternalOptions.sRndDelayMilitiaReinforcements+1);// any other reinforcement should appear more frequently
				UINT8 ubNumAvailable = gGameExternalOptions.sMinEnterMilitiaReinforcements + Random(gGameExternalOptions.sRndEnterMilitiaReinforcements+1);// total allowed units to enter per reinforce turn
//SendFmtMsg("Militia reinforcements: ubSlots=%d ubNumAvailable=%d", ubSlots, ubNumAvailable);
				if(ubSlots > ubNumAvailable)
					ubSlots = ubNumAvailable;
				guiMilitiaArrived += (ubNumAvailable - ubSlots);
			}
			else
			{
				if(guiMilitiaArrived > 0)
				{
					if(ubSlots > guiMilitiaArrived)
						ubSlots = guiMilitiaArrived;
					guiMilitiaArrived -= ubSlots;
				}
				else
					ubSlots = 0;
			}
		}
		else
			guiMilitiaReinforceTurn = guiMilitiaArrived = 0;
//SendFmtMsg("Militia reinforcements: guiTurnCnt=%d guiReinforceTurn=%d guiArrived=%d", guiTurnCnt, guiMilitiaReinforceTurn, guiMilitiaArrived);
	}

	if( !ubSlots )
	{
		//no available slots to add militia  to.  Try again later...
		return;
	}

	if ( !gfPendingNonPlayerTeam[MILITIA_TEAM] )
	{
		//Optimization.  No point in checking for group reinforcements if we know that there aren't any more enemies that can
		//be added to this battle.  This changes whenever a new enemy group arrives at the scene.
		while ( ubSlots )
		{
			UINT8 ubInsertionCode = DoReinforcementAsPendingNonPlayer( gWorldSectorX, gWorldSectorY, MILITIA_TEAM );

			if ( ubInsertionCode == 255 )
				break;

			// Assume we added one since there are supposedly more available and room for them
			--ubSlots;
		}
	}

	if( ubPredefinedInsertionCode != 255 && ubPredefinedRank != 255 &&
		NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, MILITIA_TEAM ) )
	{
		ubNumElites = ubNumRegulars = ubNumGreens = 0;

		switch( ubPredefinedRank )
		{
		case ELITE_MILITIA:
			ubNumElites++;
			break;
		case REGULAR_MILITIA:
			ubNumRegulars++;
			break;
		case GREEN_MILITIA:
			ubNumGreens++;
			break;
		default:
			break;
		}

		--ubSlots;

		if( ubNumGreens || ubNumRegulars || ubNumElites )
			AddSoldierInitListMilitiaOnEdge( ubPredefinedInsertionCode, ubNumGreens, ubNumRegulars, ubNumElites );
	}

	ubPredefinedInsertionCode = 255;
	ubPredefinedRank = 255;

	if( ubSlots )
	{
		//After going through the process, we have finished with some free slots and no more enemies to add.
		//So, we can turn off the flag, as this check is no longer needed.
		ubPredefinedInsertionCode = DoReinforcementAsPendingMilitia( gWorldSectorX, gWorldSectorY, &ubPredefinedRank );

		if( ubPredefinedInsertionCode != 255 )
		{
			AddPossiblePendingMilitiaToBattle();
			gfStrategicMilitiaChangesMade = FALSE; // Handled them here
		}
	}
	
	if ( ubSlots )
	{
		//After going through the process, we have finished with some free slots and no more enemies to add.
		//So, we can turn off the flag, as this check is no longer needed.
		gfPendingNonPlayerTeam[MILITIA_TEAM] = FALSE;
	}
}
