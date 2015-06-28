#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"

	#include "overhead.h"
	#include "overhead types.h"
	#include "isometric utils.h"
	#include "interface panels.h"
	#include "soldier macros.h"
	#include "strategicmap.h"
	#include "strategic.h"
	#include "animation control.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "soldier add.h"
	#include "Map Information.h"
	#include "fov.h"
	#include "pathai.h"
	#include "Random.h"
	#include "Render Fun.h"
	#include "meanwhile.h"
	#include "Exit Grids.h"
	#include "Interface.h"			// added by Flugente for zBackground
	#include "renderworld.h"		// added by Flugente
	#include "Vehicles.h"			// added by Flugente
	#include "CampaignStats.h"		// added by Flugente
	#include "worldman.h"			// added by Flugente for Water(...)
#endif

#ifdef JA2UB
#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#include "ub_config.h"
#endif

#include "GameSettings.h"	// ary-05/05/2009 : add forced turn mode
#include "text.h"			//	: add forced turn mode
#include "font control.h"	//	: add forced turn mode
#include "message.h"		//  : add forced turn mode
#include "connect.h"
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// Adds a soldier to a world gridno and set's direction
void AddSoldierToSectorGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubDirection, BOOLEAN fUseAnimation, UINT16 usAnimState, UINT16 usAnimCode );

INT32 FindGridNoFromSweetSpotWithStructData( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc );


// SO, STEPS IN CREATING A MERC!

// 1 ) Setup the SOLDIERCREATE_STRUCT
//			Among other things, this struct needs a sSectorX, sSectorY, and a valid InsertionDirection
//			and InsertionGridNo.
//			This GridNo will be determined by a prevouis function that will examine the sector
//			Infomration regarding placement positions and pick one
// 2 ) Call TacticalCreateSoldier() which will create our soldier
//			What it does is:	Creates a soldier in the MercPtrs[] array.
//												Allocates the Animation cache for this merc
//												Loads up the intial aniamtion file
//												Creates initial palettes, etc
//												And other cool things.
//			Now we have an allocated soldier, we just need to set him in the world!
// 3) When we want them in the world, call AddSoldierToSector().
//			This function sets the graphic in the world, lighting effects, etc
//			It also formally adds it to the TacticalSoldier slot and interface panel slot.



//Kris:	modified to actually path from sweetspot to gridno.	Previously, it only checked if the
//destination was sittable (though it was possible that that location would be trapped.
INT32 FindGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT32		sTop, sBottom;
	INT32		sLeft, sRight;
	INT32		cnt1, cnt2;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
				{
					// ATE: INstead of using absolute range, use the path cost!
				//uiRange = PlotPath( &soldier, sGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, WALKING, NOT_STEALTH, FORWARD, 50 );
					uiRange = CardinalSpacesAway( sSweetGridNo, sGridNo );

				//	if ( uiRange == 0 )
				//	{
				//		uiRange = 999999;
				//	}

					if ( uiRange < uiLowestRange )
					{
						sLowestGridNo = sGridNo;
						uiLowestRange = uiRange;
						fFound = TRUE;
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		// Set direction to center of map!
		*pubDirection =	(UINT8)GetDirectionToGridNoFromGridNo( sLowestGridNo, ( ( ( WORLD_ROWS / 2 ) * WORLD_COLS ) + ( WORLD_COLS / 2 ) ) );
		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}

INT32 FindGridNoFromSweetSpotThroughPeople( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT32		sTop, sBottom;
	INT32		sLeft, sRight;
	INT32		cnt1, cnt2;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = pSoldier->bTeam;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
				{
					uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );

					{
						if ( uiRange < uiLowestRange )
						{
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;
							fFound = TRUE;
						}
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		// Set direction to center of map!
		*pubDirection =	(UINT8)GetDirectionToGridNoFromGridNo( sLowestGridNo, ( ( ( WORLD_ROWS / 2 ) * WORLD_COLS ) + ( WORLD_COLS / 2 ) ) );
		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


//Kris:	modified to actually path from sweetspot to gridno.	Previously, it only checked if the
//destination was sittable (though it was possible that that location would be trapped.
INT32 FindGridNoFromSweetSpotWithStructData( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc )
{
	INT32		sTop, sBottom;
	INT32		sLeft, sRight;
	INT32		cnt1, cnt2, cnt3;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;
	UINT8	ubBestDirection=0;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	// If we are already at this gridno....
	if ( pSoldier->sGridNo == sSweetGridNo && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
	*pubDirection = pSoldier->ubDirection;
	return( sSweetGridNo );
	}

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
				{
					BOOLEAN fDirectionFound = FALSE;
					UINT16	usOKToAddStructID;
					STRUCTURE_FILE_REF * pStructureFileRef;
					UINT16							usAnimSurface;

					if ( pSoldier->pLevelNode != NULL )
					{
						if ( pSoldier->pLevelNode->pStructureData != NULL )
						{
							usOKToAddStructID = pSoldier->pLevelNode->pStructureData->usStructureID;
						}
						else
						{
							usOKToAddStructID = INVALID_STRUCTURE_ID;
						}
					}
					else
					{
						usOKToAddStructID = INVALID_STRUCTURE_ID;
					}

					// Get animation surface...
					usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, usAnimState );
					// Get structure ref...
					pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, usAnimState );

					if( !pStructureFileRef )
					{
						Assert( 0 );
					}

					// Check each struct in each direction
					for( cnt3 = 0; cnt3 < 8; cnt3++ )
					{
						if (OkayToAddStructureToWorld( sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ cnt3 ]]), usOKToAddStructID ) )
						{
							fDirectionFound = TRUE;
							break;
						}

					}

					if ( fDirectionFound )
					{
						if ( fClosestToMerc )
						{
						uiRange = FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, pSoldier->usUIMovementMode, NO_COPYROUTE, 0 );

				if (uiRange == 0 )
				{
				uiRange = 999;
				}
						}
						else
						{
							uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );
						}

						if ( uiRange < uiLowestRange )
						{
							ubBestDirection = (UINT8)cnt3;
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;
							fFound = TRUE;
						}
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		// Set direction we chose...
		*pubDirection = ubBestDirection;

		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


INT32 FindGridNoFromSweetSpotWithStructDataUsingGivenDirectionFirst( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc, INT8 bGivenDirection )
{
	INT32		sTop, sBottom;
	INT32		sLeft, sRight;
	INT32		cnt1, cnt2, cnt3;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;
	UINT8	ubBestDirection=0;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	// If we are already at this gridno....
	if ( pSoldier->sGridNo == sSweetGridNo && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
	*pubDirection = pSoldier->ubDirection;
	return( sSweetGridNo );
	}


	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
				{
					BOOLEAN fDirectionFound = FALSE;
					UINT16	usOKToAddStructID;
					STRUCTURE_FILE_REF * pStructureFileRef;
					UINT16							usAnimSurface;

					if ( pSoldier->pLevelNode != NULL )
					{
						if ( pSoldier->pLevelNode->pStructureData != NULL )
						{
							usOKToAddStructID = pSoldier->pLevelNode->pStructureData->usStructureID;
						}
						else
						{
							usOKToAddStructID = INVALID_STRUCTURE_ID;
						}
					}
					else
					{
						usOKToAddStructID = INVALID_STRUCTURE_ID;
					}

					// Get animation surface...
					usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, usAnimState );
					// Get structure ref...
					pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, usAnimState );

					if( !pStructureFileRef )
					{
						Assert( 0 );
					}

			// OK, check the perfered given direction first
					if (OkayToAddStructureToWorld( sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ bGivenDirection ]]), usOKToAddStructID ) )
					{
						fDirectionFound = TRUE;
			cnt3 = bGivenDirection;
					}
			else
			{
					// Check each struct in each direction
					for( cnt3 = 0; cnt3 < 8; cnt3++ )
					{
				if ( cnt3 != bGivenDirection )
				{
						 if (OkayToAddStructureToWorld( sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ cnt3 ]]), usOKToAddStructID ) )
						 {
							 fDirectionFound = TRUE;
							 break;
						 }
				}
					}
			}

					if ( fDirectionFound )
					{
						if ( fClosestToMerc )
						{
						uiRange = FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, pSoldier->usUIMovementMode, NO_COPYROUTE, 0 );

				if (uiRange == 0 )
				{
				uiRange = 999;
				}
						}
						else
						{
							uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );
						}

						if ( uiRange < uiLowestRange )
						{
							ubBestDirection = (UINT8)cnt3;
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;
							fFound = TRUE;
						}
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		// Set direction we chose...
		*pubDirection = ubBestDirection;

		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


INT32 FindGridNoFromSweetSpotWithStructDataFromSoldier( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT8 ubRadius, UINT8 *pubDirection, BOOLEAN fClosestToMerc, SOLDIERTYPE *pSrcSoldier, BOOLEAN fAllowSoldierCurrentGrid )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2, cnt3;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;
	UINT8	ubBestDirection=0;
	INT32		sSweetGridNo = NOWHERE;
	SOLDIERTYPE soldier;

	sSweetGridNo = pSrcSoldier->sGridNo;


	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				// anv: sometimes it's possible soldier already is on the best grid, NewOKDestination alone would skip it
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) || ( fAllowSoldierCurrentGrid && pSoldier->sGridNo == sGridNo ) )
				{
					BOOLEAN fDirectionFound = FALSE;
					UINT16	usOKToAddStructID;
					STRUCTURE_FILE_REF * pStructureFileRef;
					UINT16							usAnimSurface;

					if ( fClosestToMerc != 3 )
					{
						if ( pSoldier->pLevelNode != NULL && pSoldier->pLevelNode->pStructureData != NULL )
						{
							usOKToAddStructID = pSoldier->pLevelNode->pStructureData->usStructureID;
						}
						else
						{
							usOKToAddStructID = INVALID_STRUCTURE_ID;
						}

						// Get animation surface...
						usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, usAnimState );
						// Get structure ref...
						pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, usAnimState );

						// Check each struct in each direction
						for( cnt3 = 0; cnt3 < 8; cnt3++ )
						{
							if (OkayToAddStructureToWorld( sGridNo, pSoldier->pathing.bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ cnt3 ]]), usOKToAddStructID ) )
							{
								fDirectionFound = TRUE;
								break;
							}

						}
					}
					else
					{
						fDirectionFound = TRUE;
						cnt3 = (UINT8)Random( 8 );
					}

					if ( fDirectionFound )
					{
						if ( fClosestToMerc == 1 )
						{
							uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );
						}
						else if ( fClosestToMerc == 2 )
						{
							uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo ) + GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );
						}
						else
						{
							//uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );
							uiRange = abs((sSweetGridNo / MAXCOL) - (sGridNo / MAXCOL)) +
								abs((sSweetGridNo % MAXROW) - (sGridNo % MAXROW));
						}

						if ( uiRange < uiLowestRange || (uiRange == uiLowestRange && PythSpacesAway( pSoldier->sGridNo, sGridNo ) < PythSpacesAway( pSoldier->sGridNo, sLowestGridNo ) ) )
						{
							ubBestDirection = (UINT8)cnt3;
							sLowestGridNo		= sGridNo;
							uiLowestRange		= uiRange;
							fFound = TRUE;
						}
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		// Set direction we chose...
		*pubDirection = ubBestDirection;

		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


INT32 FindGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;


	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;

			if ( sSweetGridNo == sGridNo )
			{
				continue;
			}

			if ( sGridNo >=0 && sGridNo < WORLD_MAX &&
					sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{

					// Go on sweet stop
					if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
					{
						uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );

						if ( uiRange < uiLowestRange )
						{
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;

							fFound = TRUE;
						}
					}
			}
		}

	}

	if ( fFound )
	{
		// Set direction to center of map!
		*pubDirection =	(UINT8)GetDirectionToGridNoFromGridNo( sLowestGridNo, ( ( ( WORLD_ROWS / 2 ) * WORLD_COLS ) + ( WORLD_COLS / 2 ) ) );

		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


INT32 FindGridNoFromSweetSpotExcludingSweetSpotInQuardent( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection, INT8 ubQuardentDir )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2;
	INT32		sGridNo = NOWHERE;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;


	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	// Switch on quadrent
	if ( ubQuardentDir == SOUTHEAST )
	{
		sBottom = 0;
		sLeft = 0;
	}

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;

			if ( sSweetGridNo == sGridNo )
			{
				continue;
			}

			if ( sGridNo >=0 && sGridNo < WORLD_MAX &&
					sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{

					// Go on sweet stop
					if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
					{
						uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );

						if ( uiRange < uiLowestRange )
						{
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;
							fFound = TRUE;
						}
					}
			}
		}

	}

	if ( fFound )
	{
		// Set direction to center of map!
		*pubDirection =	(UINT8)GetDirectionToGridNoFromGridNo( sLowestGridNo, ( ( ( WORLD_ROWS / 2 ) * WORLD_COLS ) + ( WORLD_COLS / 2 ) ) );

		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}


BOOLEAN CanSoldierReachGridNoInGivenTileLimit( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 sMaxTiles, INT8 bLevel )
{
	INT32 iNumTiles;
	INT32 sActionGridNo = NOWHERE;
	UINT8	ubDirection;

	if ( pSoldier->pathing.bLevel != bLevel )
	{
		return( FALSE );
	}

	sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, NULL, FALSE, FALSE );

	if ( sActionGridNo == -1 )
	{
		sActionGridNo = sGridNo;
	}

	if ( sActionGridNo == pSoldier->sGridNo )
	{
		return( TRUE );
	}

	iNumTiles = FindBestPath( pSoldier, sActionGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_IGNORE_PERSON_AT_DEST );

	if ( iNumTiles <= sMaxTiles && iNumTiles != 0 )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


INT32 FindRandomGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT16		sX, sY;
	INT32		sGridNo = NOWHERE;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	UINT32		cnt = 0;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;
	INT32		sTop, sBottom;
	INT32		sLeft, sRight;
	INT32		cnt1, cnt2;
	//DBrot: More Rooms
	//UINT8	ubRoomNum;
	UINT16 usRoomNum;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.pathing.bLevel = 0;
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	// ATE: CHECK FOR BOUNDARIES!!!!!!
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, ( PATH_IGNORE_PERSON_AT_DEST | PATH_THROUGH_PEOPLE ) );//dnl ch50 071009

	do
	{
		sX = (UINT16)Random( ubRadius );
		sY = (UINT16)Random( ubRadius );

		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * sY ) )/ WORLD_COLS ) * WORLD_COLS;

		sGridNo = sSweetGridNo + ( WORLD_COLS * sY ) + sX;

		if ( sGridNo >=0 && sGridNo < WORLD_MAX &&
				sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS )
				&& gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
		{
			// Go on sweet stop
			if ( NewOKDestination( pSoldier, sGridNo, TRUE , pSoldier->pathing.bLevel) )
			{
				// If we are a crow, we need this additional check
				if ( pSoldier->ubBodyType == CROW )
				{
					if ( !InARoom( sGridNo, &usRoomNum ) )
					{
						fFound = TRUE;
					}
				}
				else
				{
					fFound = TRUE;
				}
			}
		}

		cnt++;

		if ( cnt > 2000 )
		{
			return( NOWHERE );
		}

	} while( !fFound );

	// Set direction to center of map!
	*pubDirection =	(UINT8)GetDirectionToGridNoFromGridNo( sGridNo, ( ( ( WORLD_ROWS / 2 ) * WORLD_COLS ) + ( WORLD_COLS / 2 ) ) );

	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;

	return( sGridNo );

}

// Flugente: I've altered this function in two ways:
// 1. The gridno is now drawn from the entirety of the circle - not just for sX and sY being positive
// 2. The direction now points to sSweetGridNo, center of the circle, instead of the map center
INT32 FindRandomGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT16	sX, sY;
	INT32	sGridNo = NOWHERE;
	BOOLEAN	fFound = FALSE;
	UINT32	cnt = 0;

	do
	{
		sX = (UINT16)Random( 2 * ubRadius ) - ubRadius;
		sY = (UINT16)Random( 2 * ubRadius ) - ubRadius;
		
		sGridNo = sSweetGridNo + (WORLD_COLS * sY ) + sX;
		
		if ( sGridNo == sSweetGridNo || TileIsOutOfBounds( sGridNo ) || PythSpacesAway( sGridNo, sSweetGridNo ) >= ubRadius || !IsLocationSittable( sGridNo, 0 ) )
			sGridNo = NOWHERE;
		else
		{
			// Go on sweet stop
			if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
			{
				fFound = TRUE;
			}
		}

		++cnt;

		if ( cnt > 2000 )
		{
			return( NOWHERE );
		}
	} while( !fFound );

	// Set direction to center of map!
	*pubDirection = (UINT8)GetDirectionToGridNoFromGridNo( sGridNo, sSweetGridNo );

	return( sGridNo );
}

// Flugente: returns random gridno in a circle around sCenterGridNo with radius uOuterRadius that is not inside the circle with radius uInnerRadius
INT32 FindRandomGridNoBetweenCircles( INT32 sCenterGridNo, UINT8 uInnerRadius, UINT8 uOuterRadius, UINT8& urDirection )
{
	INT16	sX, sY;
	INT32	sGridNo = NOWHERE;
	BOOLEAN	fFound = FALSE;
	UINT32	cnt = 0;

	if ( uInnerRadius >= uOuterRadius )
		return NOWHERE;

	do
	{
		/*sX = (UINT16)Random( 2 * (uOuterRadius - uInnerRadius) ) - (uOuterRadius - uInnerRadius);
		sY = (UINT16)Random( 2 * (uOuterRadius - uInnerRadius) ) - (uOuterRadius - uInnerRadius);

		if ( sX > 0 )
			sX += uInnerRadius;
		else
			sX -= uInnerRadius;

		if ( sY > 0 )
			sY += uInnerRadius;
		else
			sY -= uInnerRadius;*/

		sX = (UINT16)Random( 2 * uOuterRadius ) - uOuterRadius;
		sY = (UINT16)Random( 2 * uOuterRadius ) - uOuterRadius;
		
		sGridNo = sCenterGridNo + (WORLD_COLS * sY) + sX;

		if ( TileIsOutOfBounds( sGridNo ) || Water( sGridNo ) || !IsLocationSittable( sGridNo, 0 ) || PythSpacesAway( sGridNo, sCenterGridNo ) <= uInnerRadius || PythSpacesAway( sGridNo, sCenterGridNo ) > uOuterRadius )
			sGridNo = NOWHERE;
		else
			fFound = TRUE;

		++cnt;

		if ( cnt > 2000 )
		{
			return(NOWHERE);
		}
	} while ( !fFound );

	// Set direction to center of map!
	urDirection = (UINT8)GetDirectionToGridNoFromGridNo( sGridNo, sCenterGridNo );

	return(sGridNo);
}


BOOLEAN InternalAddSoldierToSector( UINT8 ubID, BOOLEAN fCalculateDirection, BOOLEAN fUseAnimation, UINT16 usAnimState, UINT16 usAnimCode )
{
	UINT8					ubDirection = 0;
	UINT8					ubCalculatedDirection = 0;
	SOLDIERTYPE				*pSoldier = 0;
	INT32			sGridNo = NOWHERE;
	INT32			sExitGridNo = NOWHERE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("InternalAddSoldierToSector"));

	pSoldier = MercPtrs[ ubID ];

	if ( pSoldier->bActive	)
	{
#ifdef JA2UB
//Ja25 No meanwhiles in exp
#else
		// ATE: Make sure life of elliot is OK if from a meanwhile
		if ( AreInMeanwhile() && pSoldier->ubProfile == ELLIOT )
		{
			if ( pSoldier->stats.bLife < OKLIFE )
			{
				pSoldier->stats.bLife = 25;
			}
		}
#endif
		// ADD SOLDIER TO SLOT!
		if (pSoldier->flags.uiStatusFlags & SOLDIER_OFF_MAP)
		{
			AddAwaySlot( pSoldier );

			// Guy is NOT "in sector"
			pSoldier->bInSector = FALSE;

		}
		else
		{
			AddMercSlot( pSoldier );

			// Add guy to sector flag
			pSoldier->bInSector = TRUE;

		}

		// If a driver or passenger - stop here!
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_DRIVER || pSoldier->flags.uiStatusFlags & SOLDIER_PASSENGER )
		{
			return( FALSE );
		}

		// Add to panel
		CheckForAndAddMercToTeamPanel( pSoldier );

		pSoldier->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_SPOTTING_CREATURE_ATTACK);
		pSoldier->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_SMELLED_CREATURE);
		pSoldier->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_WORRIED_ABOUT_CREATURES);

		// Add to interface if the are ours
		if ( pSoldier->bTeam == CREATURE_TEAM )
		{
			sGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, STANDING, pSoldier->sInsertionGridNo, 7, &ubCalculatedDirection, FALSE );
			if( fCalculateDirection )
				ubDirection = ubCalculatedDirection;
			else
				ubDirection = pSoldier->ubInsertionDirection;
		}
		else
		{			
			if(TileIsOutOfBounds(pSoldier->sInsertionGridNo))
			{ //Add the soldier to the respective entrypoint.	This is an error condition.
				// So treat it like an error already then
				
				// WANNE: ASSERTION: Removed the assertion until we find the bug!
				//Assert(0);
			}
			if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
			{
				sGridNo = FindGridNoFromSweetSpotWithStructDataUsingGivenDirectionFirst( pSoldier, STANDING, pSoldier->sInsertionGridNo, 12, &ubCalculatedDirection, FALSE, pSoldier->ubInsertionDirection );
				// ATE: Override insertion direction				
				if (TileIsOutOfBounds(sGridNo))
				{
					// Well, we gotta place this soldier/vehicle somewhere.	Just use the first position for now
					sGridNo = pSoldier->sGridNo = pSoldier->sInsertionGridNo;
				}
				else
				{
					pSoldier->ubInsertionDirection = ubCalculatedDirection;
				}
			}
			else
			{
				if(is_client && (pSoldier->ubStrategicInsertionCode == INSERTION_CODE_GRIDNO)) 
				{
					sGridNo = pSoldier->sInsertionGridNo;
					ubCalculatedDirection = pSoldier->ubDirection;
				}
				else 
					sGridNo = FindGridNoFromSweetSpot( pSoldier, pSoldier->sInsertionGridNo, 7, &ubCalculatedDirection );
				//hayden
				// ATE: Error condition - if nowhere use insertion gridno!				
				if (TileIsOutOfBounds(sGridNo))
				{
					sGridNo = pSoldier->sInsertionGridNo;
				}
			}

			// Flugente: campaign stats
			if ( pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
				gCurrentIncident.usIncidentFlags |= INCIDENT_AIRDROP;

			// problem: soldiers already present in a sector have ubStrategicInsertionCode = 0, which is INSERTION_CODE_NORTH - but they don't actually come from north, as they are already present
			// we thus count them as coming from north if they have valid usStrategicInsertionData
			if ( pSoldier->ubStrategicInsertionCode == INSERTION_CODE_NORTH && pSoldier->usStrategicInsertionData )//|| pSoldier->sGridNo != NOWHERE )
			{
				if ( pSoldier->bSide == gbPlayerNum )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_NORTH;
				else if ( !pSoldier->aiData.bNeutral )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_NORTH_ENEMY;
			}
			else if ( pSoldier->ubStrategicInsertionCode == INSERTION_CODE_SOUTH )
			{
				if ( pSoldier->bSide == gbPlayerNum )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_SOUTH;
				else if ( !pSoldier->aiData.bNeutral )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_SOUTH_ENEMY;
			}
			else if ( pSoldier->ubStrategicInsertionCode == INSERTION_CODE_EAST )
			{
				if ( pSoldier->bSide == gbPlayerNum )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_EAST;
				else if ( !pSoldier->aiData.bNeutral )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_EAST_ENEMY;
			}
			else if ( pSoldier->ubStrategicInsertionCode == INSERTION_CODE_WEST )
			{
				if ( pSoldier->bSide == gbPlayerNum )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_WEST;
				else if ( !pSoldier->aiData.bNeutral )
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_WEST_ENEMY;
			}

			// add this flag whenever we enter strategically enter a sector (= we attack a sector)
			pSoldier->usSoldierFlagMask |= SOLDIER_ASSAULT_BONUS;
			
			// Override calculated direction if we were told to....
			if ( pSoldier->ubInsertionDirection >= 100 )
			{
				pSoldier->ubInsertionDirection = pSoldier->ubInsertionDirection - 100;
				fCalculateDirection = FALSE;
			}

			if ( fCalculateDirection )
			{
				ubDirection = ubCalculatedDirection;

				// Check if we need to get direction from exit grid...
				if ( pSoldier->bUseExitGridForReentryDirection )
				{
					pSoldier->bUseExitGridForReentryDirection = FALSE;

					// OK, we know there must be an exit gridno SOMEWHERE close...
					sExitGridNo = FindClosestExitGrid( pSoldier, sGridNo, 10 );
					
					if (!TileIsOutOfBounds(sExitGridNo))
					{
						// We found one
						// Calculate direction...
						ubDirection = (UINT8)GetDirectionToGridNoFromGridNo( sExitGridNo, sGridNo );
					}
				}
			}
			else
			{
				ubDirection = pSoldier->ubInsertionDirection;
			}
		}

		//Add
		if(gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
			AddSoldierToSectorGridNo( pSoldier, sGridNo, pSoldier->ubDirection, fUseAnimation, usAnimState, usAnimCode );
		else
			AddSoldierToSectorGridNo( pSoldier, sGridNo, ubDirection, fUseAnimation, usAnimState, usAnimCode );

		// SANDRO - only do this if we are not loading the game (we keep this value saved)
		if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			CheckForPotentialAddToBattleIncrement( pSoldier );
		}

		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN AddSoldierToSector( UINT8 ubID )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierToSector"));
	return( InternalAddSoldierToSector( ubID, TRUE, FALSE, 0 , 0) );
}

BOOLEAN AddSoldierToSectorNoCalculateDirection( UINT8 ubID )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierToSectorNoCalculateDirection"));
	return( InternalAddSoldierToSector( ubID, FALSE, FALSE, 0, 0 ) );
}

BOOLEAN AddSoldierToSectorNoCalculateDirectionUseAnimation( UINT8 ubID, UINT16 usAnimState, UINT16 usAnimCode )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierToSectorNoCalculateDirectionUseAnimation"));
	return( InternalAddSoldierToSector( ubID, FALSE, TRUE, usAnimState, usAnimCode ) );
}


void InternalSoldierInSectorSleep( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fDoTransition )
{
	INT16 sWorldX, sWorldY;
	UINT8	ubNewDirection;
	INT32	sGoodGridNo = NOWHERE;
	UINT16	usAnim = SLEEPING;

	if ( !pSoldier->bInSector )
	{
		return;
	}

	if ( AM_AN_EPC( pSoldier ) )
	{
	usAnim = STANDING;
	}

	// OK, look for sutable placement....
	sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, usAnim, sGridNo, 5, &ubNewDirection, FALSE );

	sWorldX = CenterX( sGoodGridNo );
	sWorldY = CenterY( sGoodGridNo );

	pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );

	pSoldier->EVENT_SetSoldierDirection( ubNewDirection );
	pSoldier->EVENT_SetSoldierDesiredDirection( ubNewDirection );

	//pSoldier->pathing.bDesiredDirection = pSoldier->ubDirection;

	if ( AM_AN_EPC( pSoldier ) )
	{
		pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
	}
	else
	{
		if ( fDoTransition )
		{
			pSoldier->EVENT_InitNewSoldierAnim( GOTO_SLEEP, 1, TRUE );
		}
		else
		{
			pSoldier->EVENT_InitNewSoldierAnim( SLEEPING, 1, TRUE );
		}
	}
}

void SoldierInSectorIncompaciated( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	INT16 sWorldX, sWorldY;
	UINT8	ubNewDirection;
	INT32 sGoodGridNo = NOWHERE;

	if ( !pSoldier->bInSector )
	{
		return;
	}

	// OK, look for sutable placement....
	sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, STAND_FALLFORWARD_STOP, sGridNo, 5, &ubNewDirection, FALSE );

	sWorldX = CenterX( sGoodGridNo );
	sWorldY = CenterY( sGoodGridNo );

	pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );

	pSoldier->EVENT_SetSoldierDirection( ubNewDirection );
	pSoldier->EVENT_SetSoldierDesiredDirection( ubNewDirection );

	//pSoldier->pathing.bDesiredDirection = pSoldier->ubDirection;

	pSoldier->EVENT_InitNewSoldierAnim( STAND_FALLFORWARD_STOP, 1, TRUE );
}


/*
void SoldierInSectorSleep( SOLDIERTYPE *pSoldier, INT16 sGridNo )
{
	InternalSoldierInSectorSleep( pSoldier, sGridNo, TRUE );
}
*/


void SoldierInSectorPatient( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	INT16 sWorldX, sWorldY;
	UINT8	ubNewDirection;
	INT32 sGoodGridNo = NOWHERE;

	if ( !pSoldier->bInSector )
	{
		return;
	}

	// OK, look for sutable placement....
	sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, BEING_PATIENT, sGridNo, 5, &ubNewDirection, FALSE );

	sWorldX = CenterX( sGoodGridNo );
	sWorldY = CenterY( sGoodGridNo );

	pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );

	pSoldier->EVENT_SetSoldierDirection( ubNewDirection );
	pSoldier->EVENT_SetSoldierDesiredDirection( ubNewDirection );

	//pSoldier->pathing.bDesiredDirection = pSoldier->ubDirection;

	if ( !IS_MERC_BODY_TYPE( pSoldier ) )
	{
		pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
	}
	else
	{
		pSoldier->EVENT_InitNewSoldierAnim( BEING_PATIENT, 1, TRUE );
	}
}


void SoldierInSectorDoctor( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	INT16 sWorldX, sWorldY;
	UINT8	ubNewDirection;
	INT32 sGoodGridNo = NOWHERE;

	if ( !pSoldier->bInSector )
	{
		return;
	}

	// OK, look for sutable placement....
	sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, BEING_DOCTOR, sGridNo, 5, &ubNewDirection, FALSE );

	sWorldX = CenterX( sGoodGridNo );
	sWorldY = CenterY( sGoodGridNo );

	pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );

	pSoldier->EVENT_SetSoldierDirection( ubNewDirection );
	pSoldier->EVENT_SetSoldierDesiredDirection( ubNewDirection );

	//pSoldier->pathing.bDesiredDirection = pSoldier->ubDirection;

	if ( !IS_MERC_BODY_TYPE( pSoldier ) )
	{
		pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
	}
	else
	{
		pSoldier->EVENT_InitNewSoldierAnim( BEING_DOCTOR, 1, TRUE );
	}
}


void SoldierInSectorRepair( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	INT16 sWorldX, sWorldY;
	UINT8	ubNewDirection;
	INT32 sGoodGridNo = NOWHERE;

	if ( !pSoldier->bInSector )
	{
		return;
	}

	// OK, look for sutable placement....
	sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, BEING_REPAIRMAN, sGridNo, 5, &ubNewDirection, FALSE );

	sWorldX = CenterX( sGoodGridNo );
	sWorldY = CenterY( sGoodGridNo );

	pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );

	pSoldier->EVENT_SetSoldierDirection( ubNewDirection );
	pSoldier->EVENT_SetSoldierDesiredDirection( ubNewDirection );

	//pSoldier->pathing.bDesiredDirection = pSoldier->ubDirection;

	if ( !IS_MERC_BODY_TYPE( pSoldier ) )
	{
		pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
	}
	else
	{
		pSoldier->EVENT_InitNewSoldierAnim( BEING_REPAIRMAN, 1, TRUE );
	}
}

extern void EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateDest,	BOOLEAN fUpdateFinalDest );

void AddSoldierToSectorGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubDirection, BOOLEAN fUseAnimation, UINT16 usAnimState, UINT16 usAnimCode )
{
	INT16 sWorldX, sWorldY;
	INT32	sNewGridNo = NOWHERE;
	UINT8	ubNewDirection;
	UINT8	ubInsertionCode;
	BOOLEAN fUpdateFinalPosition = TRUE;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierToSectorGridNo"));

	// Add merc to gridno
	sWorldX = CenterX( sGridNo );
	sWorldY = CenterY( sGridNo );

	// Set reserved location!
	pSoldier->sReservedMovementGridNo = NOWHERE;

	// Save OLD insertion code.. as this can change...
	ubInsertionCode = pSoldier->ubStrategicInsertionCode;

	// Remove any pending animations
	pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
	pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;
	pSoldier->ubPendingDirection = NO_PENDING_DIRECTION;
	pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

	//If we are not loading a saved game
	if( (gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		// Set final dest to be the same...
		fUpdateFinalPosition = FALSE;
	}

	// Flugente: if we are airdropping, center screen on the action and remove soldier initially. He will be dropped into the sector by the helicopter
	if ( pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
	{
		pSoldier->usSoldierFlagMask &= ~SOLDIER_AIRDROP;

		if ( gGameExternalOptions.ubSkyriderHotLZ == 3 )
		{
			gfIgnoreScrolling = FALSE;
			INT16 sNewCenterWorldX, sNewCenterWorldY;
			ConvertGridNoToCenterCellXY( sGridNo, &sNewCenterWorldX, &sNewCenterWorldY );
		
			SetRenderCenter( sNewCenterWorldX, sNewCenterWorldY );
			gfIgnoreScrolling = TRUE;
			
			pSoldier->RemoveSoldierFromGridNo( );
			pSoldier->bInSector = FALSE;

			return;
		}
	}

	// If this is a special insertion location, get path!
	if ( ubInsertionCode == INSERTION_CODE_ARRIVING_GAME )
	{
		EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( pSoldier, sWorldX, sWorldY, fUpdateFinalPosition, fUpdateFinalPosition );
		pSoldier->EVENT_SetSoldierDirection( ubDirection );
		pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
	}
	else if ( ubInsertionCode == INSERTION_CODE_CHOPPER )
	{

	}
	else
	{
		EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( pSoldier, sWorldX, sWorldY, fUpdateFinalPosition, fUpdateFinalPosition );

		//if we are loading, dont set the direction ( they are already set )
		if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			pSoldier->EVENT_SetSoldierDirection( ubDirection );

			pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
		}
	}

	if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
		{
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				RevealRoofsAndItems( pSoldier, TRUE, FALSE, pSoldier->pathing.bLevel, TRUE );

		// ATE: Patch fix: If we are in an non-interruptable animation, stop!
		if ( pSoldier->usAnimState == HOPFENCE )
		{
			pSoldier->flags.fInNonintAnim = FALSE;
			pSoldier->SoldierGotoStationaryStance( );
		}

				pSoldier->EVENT_StopMerc( sGridNo, ubDirection );
			}
		}

		// If just arriving, set a destination to walk into from!
		if ( ubInsertionCode == INSERTION_CODE_ARRIVING_GAME )
		{
			// Find a sweetspot near...
			sNewGridNo = FindGridNoFromSweetSpot( pSoldier, gMapInformation.sNorthGridNo, 4, &ubNewDirection );
			pSoldier->EVENT_GetNewSoldierPath( sNewGridNo, WALKING );
		}

		// If he's an enemy... set presence
		if ( !pSoldier->aiData.bNeutral && (pSoldier->bSide != gbPlayerNum ) )
		{
			// ATE: Added if not bloodcats
			// only do this once they are seen.....
			if ( pSoldier->ubBodyType != BLOODCAT )
			{
				SetEnemyPresence( );
				// ary-05/05/2009 : add forced turn mode : note : not for bloodcats..
				//  : note : no forced turn mode option for Multi Player
				if ( is_networked ) gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ] = FALSE;
				if ( gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ])
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_FTM_ENTER_COMBAT ] );
					if( Random( 100 ) >= Random( 100 ) ) // give a chance for either to go first
						EnterCombatMode( OUR_TEAM );
					else
						EnterCombatMode( ENEMY_TEAM );
				}
			}
		}
	}

	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
	{
		//if we are loading a 'pristine' map ( ie, not loading a saved game )
		if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			// ATE: Double check if we are on the roof that there is a roof there!
			if ( pSoldier->pathing.bLevel == 1 )
			{
				if ( !FindStructure( pSoldier->sGridNo, STRUCTURE_ROOF ) )
				{
					pSoldier->SetSoldierHeight( (FLOAT)( 0 )	);
				}
			}

			if ( ubInsertionCode != INSERTION_CODE_ARRIVING_GAME )
			{
				// default to standing on arrival
				if ( pSoldier->usAnimState != HELIDROP )
				{
#ifdef JA2UB				
					// DAVE!!!!
					if ( gfFirstTimeInGameHeliCrash && gGameUBOptions.InGameHeli == FALSE )
					{
						//should we be on our back or tummy
						if( Random( 100 ) < 50 )
							pSoldier->EVENT_InitNewSoldierAnim( STAND_FALLFORWARD_STOP, 1, TRUE );
						else
							 pSoldier->EVENT_InitNewSoldierAnim( FALLBACKHIT_STOP, 1, TRUE );

						pSoldier->bCollapsed = TRUE;

					}					
					else if ( fUseAnimation )
					{
						pSoldier->EVENT_InitNewSoldierAnim( usAnimState, usAnimCode, TRUE );
					}
					else if ( pSoldier->ubBodyType != CROW )
					{
						pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
					}
#else
					if ( fUseAnimation )
					{
						pSoldier->EVENT_InitNewSoldierAnim( usAnimState, usAnimCode, TRUE );
					}
					else if ( pSoldier->ubBodyType != CROW )
					{
						pSoldier->EVENT_InitNewSoldierAnim( STANDING, 1, TRUE );
					}
#endif
				}

				// ATE: if we are below OK life, make them lie down!
				if ( pSoldier->stats.bLife < OKLIFE )
				{
					SoldierInSectorIncompaciated( pSoldier, pSoldier->sInsertionGridNo );
				}
				else if ( pSoldier->flags.fMercAsleep == TRUE )
				{
					InternalSoldierInSectorSleep( pSoldier, pSoldier->sInsertionGridNo, FALSE );
				}
				else if ( pSoldier->bAssignment == PATIENT )
				{
					SoldierInSectorPatient( pSoldier, pSoldier->sInsertionGridNo );
				}
				else if ( pSoldier->bAssignment == DOCTOR )
				{
					SoldierInSectorDoctor( pSoldier, pSoldier->sInsertionGridNo );
				}
				else if ( pSoldier->bAssignment == REPAIR )
				{
					SoldierInSectorRepair( pSoldier, pSoldier->sInsertionGridNo );
				}

		// ATE: Make sure movement mode is up to date!
				pSoldier->usUIMovementMode =	pSoldier->GetMoveStateBasedOnStance( gAnimControl[ pSoldier->usAnimState ].ubEndHeight );

			}
		}
		else
		{
			// THIS ALL SHOULD HAVE BEEN HANDLED BY THE FACT THAT A GAME WAS LOADED

			//pSoldier->EVENT_InitNewSoldierAnim( pSoldier->usAnimState, pSoldier->usAniCode, TRUE );

			// if the merc had a final destination, get the merc walking there
			//if( pSoldier->pathing.sFinalDestination != pSoldier->sGridNo )
			//{
			//	pSoldier->EVENT_GetNewSoldierPath( pSoldier->pathing.sFinalDestination, pSoldier->usUIMovementMode );
			//}
		}
	}
}




// IsMercOnTeam() checks to see if the passed in Merc Profile ID is currently on the player's team
BOOLEAN IsMercOnTeam(UINT8 ubMercID)
{
	UINT16 cnt;
	UINT8		ubLastTeamID;
	SOLDIERTYPE		*pTeamSoldier;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	ubLastTeamID = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= ubLastTeamID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->ubProfile == ubMercID )
		{
			if( pTeamSoldier->bActive )
				return(TRUE);
		}
	}

	return(FALSE);
}

// ATE: Added this new function for contract renewals
BOOLEAN IsMercOnTeamAndAlive(UINT8 ubMercID)
{
	UINT16 cnt;
	UINT8		ubLastTeamID;
	SOLDIERTYPE		*pTeamSoldier;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	ubLastTeamID = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= ubLastTeamID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->ubProfile == ubMercID )
		{
			if( pTeamSoldier->bActive )
		{
		if ( pTeamSoldier->stats.bLife > 0 )
		{
				return(TRUE);
		}
		}
		}
	}

	return(FALSE);
}

BOOLEAN IsMercOnTeamAndInOmertaAlready(UINT8 ubMercID)
{
	UINT16 cnt;
	UINT8		ubLastTeamID;
	SOLDIERTYPE		*pTeamSoldier;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	ubLastTeamID = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= ubLastTeamID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->ubProfile == ubMercID )
		{
			if ( pTeamSoldier->bActive && pTeamSoldier->bAssignment != IN_TRANSIT )
				return(TRUE);
		}
	}

	return(FALSE);
}

BOOLEAN IsMercOnTeamAndInOmertaAlreadyAndAlive(UINT8 ubMercID)
{
	UINT16 cnt;
	UINT8		ubLastTeamID;
	SOLDIERTYPE		*pTeamSoldier;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	ubLastTeamID = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= ubLastTeamID; cnt++,pTeamSoldier++)
	{
		if ( pTeamSoldier->ubProfile == ubMercID )
		{
			if ( pTeamSoldier->bActive && pTeamSoldier->bAssignment != IN_TRANSIT )
		{
		if ( pTeamSoldier->stats.bLife > 0 )
		{
				return(TRUE);
		}
		}
		}
	}

	return(FALSE);
}


// GetSoldierIDFromMercID() Gets the Soldier ID from the Merc Profile ID, else returns -1
INT16 GetSoldierIDFromMercID(UINT8 ubMercID)
{
	SOLDIERTYPE		*pTeamSoldier = NULL;

	UINT16 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID;

	// look for all mercs on the same team,
	for ( pTeamSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt, ++pTeamSoldier )
	{
		if ( pTeamSoldier->ubProfile == ubMercID )
		{
			if( pTeamSoldier->bActive )
				return( cnt );
		}
	}

	return -1 ;
}



