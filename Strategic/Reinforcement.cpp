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
	#include "Queen Command.h"
	#include "GameSettings.h"
	#include "Soldier Init List.h"
	#include "Inventory Choosing.h"
#endif

#include "connect.h"
#include "Reinforcement.h"
#include "MilitiaSquads.h"

UINT8 gubReinforcementMinEnemyStaticGroupSize = 12;

void GetNumberOfEnemiesInFiveSectors( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites )
{
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites;
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber, ubIndex;
	
	GetNumberOfStationaryEnemiesInSector( sSectorX, sSectorY, pubNumAdmins, pubNumTroops, pubNumElites );

	GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( sSectorX, sSectorY, &ubNumAdmins, &ubNumTroops, &ubNumElites );

	*pubNumAdmins += ubNumAdmins;
	*pubNumTroops += ubNumTroops;
	*pubNumElites += ubNumElites;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return;

	if ( ( GetTownIdForSector( sSectorX, sSectorY ) == OMERTA )&&( gGameOptions.ubDifficultyLevel != DIF_LEVEL_INSANE ) ) //Madd: skip Omerta //Lal: but not on insane ;-)
		return;

	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
	{
		GetNumberOfStationaryEnemiesInSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),  &ubNumAdmins, &ubNumTroops, &ubNumElites );

		while( ubNumElites + ubNumTroops + ubNumAdmins > gubReinforcementMinEnemyStaticGroupSize)
		{
			if( ubNumElites )
			{
				*pubNumElites += 1;
				ubNumElites--;
			}else if( ubNumTroops )
			{
				*pubNumTroops += 1;
				ubNumTroops--;
			}else if( ubNumAdmins )
			{
				*pubNumAdmins += 1;
				ubNumAdmins--;
			}
		}

		GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),  &ubNumAdmins, &ubNumTroops, &ubNumElites );

		*pubNumAdmins += ubNumAdmins;
		*pubNumTroops += ubNumTroops;
		*pubNumElites += ubNumElites;
	}
}

UINT8 NumEnemiesInFiveSectors( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites;

	GetNumberOfEnemiesInFiveSectors( sMapX, sMapY, &ubNumAdmins, &ubNumTroops, &ubNumElites );
	
	return ubNumAdmins + ubNumTroops + ubNumElites;
}

BOOLEAN IsGroupInARightSectorToReinforce( GROUP *pGroup, INT16 sSectorX, INT16 sSectorY )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;

	if( pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		return TRUE;  //Well, it's in the same sector, so allow to reinforce

	if( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
		if( pGroup->ubSectorX == SECTORX( pusMoveDir[ ubIndex ][ 0 ] ) && pGroup->ubSectorY == SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) )
			return TRUE;

	return FALSE;
}

UINT8 GetAdjacentSectors( UINT8 pSectors[4], INT16 sSectorX, INT16 sSectorY )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;
	UINT8 ubCounter = 0;

	GenerateDirectionInfos( sSectorX, sSectorY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
		pSectors[ ubCounter++ ] = (UINT8)pusMoveDir[ ubIndex ][ 0 ];

	return ubCounter;
}

UINT8 CountAllMilitiaInFiveSectors(INT16 sMapX, INT16 sMapY)
{
	UINT8 ubResult = 0;
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber, ubIndex;
	
	ubResult = CountAllMilitiaInSector( sMapX, sMapY );

	if( !gGameExternalOptions.gfAllowReinforcements )
		return ubResult;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
		ubResult += CountAllMilitiaInSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) );


	return ubResult;
}

UINT8 MilitiaInFiveSectorsOfRank( INT16 sMapX, INT16 sMapY, UINT8 ubRank )
{
	UINT8 ubResult = 0;
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber, ubIndex;
	
	ubResult = MilitiaInSectorOfRank( sMapX, sMapY, ubRank );

	if( !gGameExternalOptions.gfAllowReinforcements )
		return ubResult;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
		ubResult += MilitiaInSectorOfRank( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ), ubRank );

	return ubResult;
}

BOOLEAN ARMoveBestMilitiaManFromAdjacentSector(INT16 sMapX, INT16 sMapY)
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber;
	UINT8 ubRandom;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	if( CountAllMilitiaInSector( sMapX, sMapY ) >= gGameExternalOptions.iMaxMilitiaPerSector ||
		CountAllMilitiaInFiveSectors( sMapX, sMapY ) - CountAllMilitiaInSector( sMapX, sMapY ) == 0 )
		return FALSE;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	ubRandom = Random( ubDirNumber );

	while( !MoveOneBestMilitiaMan( SECTORX( pusMoveDir[ ubRandom ][ 0 ] ), SECTORY( pusMoveDir[ ubRandom ][ 0 ] ), sMapX, sMapY )  )
		ubRandom = Random( ubDirNumber );

	return TRUE;
}

BOOLEAN ARRemoveMilitiaMan( INT16 sMapX, INT16 sMapY, UINT8 ubRank )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber, ubRandom;

	if( MilitiaInSectorOfRank( sMapX, sMapY, ubRank ) )
	{
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, ubRank, 1 );
		ARMoveBestMilitiaManFromAdjacentSector( sMapX, sMapY );
		return TRUE;
	}

	if( !gGameExternalOptions.gfAllowReinforcements )
		return FALSE;

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ; ;  )
	{
		ubRandom = Random( ubDirNumber );
		if( MilitiaInSectorOfRank( SECTORX( pusMoveDir[ ubRandom ][ 0 ] ), SECTORY( pusMoveDir[ ubRandom ][ 0 ] ), ubRank ) )
		{
			StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubRandom ][ 0 ] ), SECTORY( pusMoveDir[ ubRandom ][ 0 ] ), ubRank, 1 );
			return TRUE;
		}
	}


	return FALSE;
}

GROUP* GetEnemyGroupInSector( INT16 sMapX, INT16 sMapY )
{
	GROUP *curr;
	curr = gpGroupList;
	while( curr )
	{
		if( curr->ubSectorX == sMapX && curr->ubSectorY == sMapY && !curr->fPlayer && curr->ubGroupID )
			return curr;
		curr = curr->next;
	}
	return NULL;
}


UINT8 DoReinforcementAsPendingEnemy( INT16 sMapX, INT16 sMapY )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;
	GROUP *pGroup;
	//ENEMYGROUP *pEnemyGroup;
	SECTORINFO *pThisSector, *pSector;
	
	if( !gGameExternalOptions.gfAllowReinforcements )
		return 255;

	if ( ( GetTownIdForSector( sMapX, sMapY ) == OMERTA )&&( gGameOptions.ubDifficultyLevel != DIF_LEVEL_INSANE ) ) //Madd: skip Omerta //Lal: but not on insane ;-)
		return 255;

	pThisSector = &SectorInfo[ SECTOR( sMapX, sMapY ) ];

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	for( ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
	{
		while ((pGroup = GetEnemyGroupInSector( SECTORX( pusMoveDir[ ubIndex][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) ) ) != NULL)
		{
			pGroup->ubPrevX = pGroup->ubSectorX;
			pGroup->ubPrevY = pGroup->ubSectorY;

			pGroup->ubSectorX = pGroup->ubNextX = (UINT8)sMapX;
			pGroup->ubSectorY = pGroup->ubNextY = (UINT8)sMapY;

			gfPendingEnemies = TRUE;
			ResetMortarsOnTeamCount();
			ResetNumSquadleadersInArmyGroup(); // added by SANDRO
		}
	}

	while( ubDirNumber > 0 )
	{
		ubIndex = Random(ubDirNumber);

		if( NumEnemiesInSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) ) > gubReinforcementMinEnemyStaticGroupSize )
		{
			pSector = &SectorInfo[ pusMoveDir[ ubIndex ][ 0 ] ];

			if( pSector->ubNumElites )
			{
				(pThisSector->ubNumElites)++;
				(pSector->ubNumElites)--;
				(pThisSector->ubElitesInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 0, 1, FALSE );
			}else if( pSector->ubNumTroops )
			{
				(pThisSector->ubNumTroops)++;
				(pSector->ubNumTroops)--;
				(pThisSector->ubTroopsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 0, 1, 0, FALSE );
			}else if( pSector->ubNumAdmins )
			{
				(pThisSector->ubNumAdmins)++;
				(pSector->ubNumAdmins)--;
				(pThisSector->ubAdminsInBattle)++;
				AddEnemiesToBattle( NULL, (UINT8)pusMoveDir[ ubIndex ][ 2 ], 1, 0, 0, FALSE );
			}

			return (UINT8)pusMoveDir[ ubIndex ][ 2 ];
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


UINT8 NumFreeMilitiaSlots()
{
	UINT8 ubNumFreeSlots = 0;
	INT32 i;
	SOLDIERTYPE *pSoldier;
	//Count the number of free militia slots.  It is possible to have multiple groups exceed the maximum.
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM].bLastID; i++ )
	{
		pSoldier = &Menptr[ i ];
		if( !pSoldier->bActive )
			ubNumFreeSlots++;
	}
	return max( 0 , ubNumFreeSlots - ( gGameExternalOptions.ubGameMaximumNumberOfRebels - gGameExternalOptions.iMaxMilitiaPerSector ) );
}

UINT8 DoReinforcementAsPendingMilitia( INT16 sMapX, INT16 sMapY, UINT8 *pubRank )
{
	UINT16 pusMoveDir[4][3];
	UINT8 ubDirNumber = 0, ubIndex;
	SECTORINFO *pSector;//*pThisSector,

	if( !gGameExternalOptions.gfAllowReinforcements )
		return 255;

	//	pThisSector = &SectorInfo[ SECTOR( sMapX, sMapY ) ];

	GenerateDirectionInfos( sMapX, sMapY, &ubDirNumber, pusMoveDir, FALSE, TRUE );

	if( CountAllMilitiaInFiveSectors( sMapX, sMapY ) - CountAllMilitiaInSector( sMapX, sMapY ) == 0 )
	{
		*pubRank = 255;
		return 255;
	}else
		for(;;)
		{
			ubIndex = Random(ubDirNumber);
			if( CountAllMilitiaInSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) ) )
			{
				pSector = &SectorInfo[ pusMoveDir[ ubIndex ][ 0 ] ];

				if( pSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,ELITE_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),ELITE_MILITIA, 1 );
					*pubRank = ELITE_MILITIA;
				}else if( pSector->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,REGULAR_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),REGULAR_MILITIA, 1 );
					*pubRank = REGULAR_MILITIA;
				}else if( pSector->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] )
				{
					StrategicAddMilitiaToSector( sMapX, sMapY,GREEN_MILITIA, 1 );
					StrategicRemoveMilitiaFromSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ),GREEN_MILITIA, 1 );
					*pubRank = GREEN_MILITIA;
				}

				return (UINT8)pusMoveDir[ ubIndex ][ 2 ];
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
	//		|| !NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) ) return;
	if( (PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, 0 ) == 0) 
		|| !(gTacticalStatus.uiFlags & WANT_MILITIA_REINFORCEMENTS)
		|| (NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) == 0)
		) 
		return;
	//gGameExternalOptions.guiMaxMilitiaSquadSize - CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY );
	ubSlots = NumFreeMilitiaSlots();
	if( !ubSlots )
	{ //no available slots to add militia  to.  Try again later...
		return;
	}

	if( ubPredefinedInsertionCode != 255 && ubPredefinedRank != 255 &&
		CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY ) )
	{
		ubNumElites = ubNumRegulars = ubNumGreens = 0;

		//	while( ubSlots )
		switch( ubPredefinedRank )
		{
		case ELITE_MILITIA:
			ubSlots--;
			ubNumElites++;
			break;
		case REGULAR_MILITIA:
			ubSlots--;
			ubNumRegulars++;
			break;
		case GREEN_MILITIA:
			ubSlots--;
			ubNumGreens++;
			break;
		default:
			ubSlots--;
		}

		if( ubNumGreens || ubNumRegulars || ubNumElites )
			AddSoldierInitListMilitiaOnEdge( ubPredefinedInsertionCode, ubNumGreens, ubNumRegulars, ubNumElites );
	}

	ubPredefinedInsertionCode = 255;
	ubPredefinedRank = 255;

	if( ubSlots )
	{ //After going through the process, we have finished with some free slots and no more enemies to add.
		//So, we can turn off the flag, as this check is no longer needed.
		ubPredefinedInsertionCode = DoReinforcementAsPendingMilitia( gWorldSectorX, gWorldSectorY, &ubPredefinedRank );

		if( ubPredefinedInsertionCode != 255 )
		{
			AddPossiblePendingMilitiaToBattle();
			gfStrategicMilitiaChangesMade = FALSE; // Handled them here
		}
	}
}

