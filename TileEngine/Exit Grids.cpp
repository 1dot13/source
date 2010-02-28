#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include <stdio.h>
	#include "FileMan.h"
	#include "debug.h"
	#include "types.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "smooth.h"
	#include "Exit Grids.h"
	#include "Editor Undo.h"
	#include "StrategicMap.h"
	#include "Strategic Movement.h"
	#include "message.h"
	#include "Font Control.h"
	#include "pathai.h"
	#include "overhead.h"
	#include "Animation Control.h"
	#include "Sys Globals.h"
	#include "quests.h"
	#include "SaveLoadMap.h"
	#include "Text.h"
#endif

BOOLEAN gfLoadingExitGrids = FALSE;

//used by editor.
EXITGRID		gExitGrid	= {0,1,1,0};

BOOLEAN gfOverrideInsertionWithExitGrid = FALSE;

//<SB>

#define MAX_EXITGRIDS	4096

EXITGRID gpExitGrids[MAX_EXITGRIDS];
UINT guiExitGridsCount = 0;


//INT32 ConvertExitGridToINT32( EXITGRID *pExitGrid )
//{
//	INT32 iExitGridInfo;
//	iExitGridInfo  = (pExitGrid->ubGotoSectorX-1)<< 28;
//	iExitGridInfo += (pExitGrid->ubGotoSectorY-1)<< 24;
//	iExitGridInfo += pExitGrid->ubGotoSectorZ    << 20;
//	iExitGridInfo += pExitGrid->usGridNo & 0x0000ffff;
//	return iExitGridInfo;
//}
//
//void ConvertINT32ToExitGrid( INT32 iExitGridInfo, EXITGRID *pExitGrid )
//{
//	//convert the int into 4 unsigned bytes.
//	pExitGrid->ubGotoSectorX		= (UINT8)(((iExitGridInfo & 0xf0000000)>>28)+1);
//	pExitGrid->ubGotoSectorY		= (UINT8)(((iExitGridInfo & 0x0f000000)>>24)+1);
//	pExitGrid->ubGotoSectorZ		= (UINT8)((iExitGridInfo & 0x00f00000)>>20);
//	pExitGrid->usGridNo					= (UINT16)(iExitGridInfo & 0x0000ffff);
//}

BOOLEAN	GetExitGrid( UINT32 usMapIndex, EXITGRID *pExitGrid )
{
	LEVELNODE *pShadow;
	pShadow = gpWorldLevelData[ usMapIndex ].pShadowHead;
	//Search through object layer for an exitgrid
	while( pShadow )
	{
		if ( pShadow->uiFlags & LEVELNODE_EXITGRID )
		{
//<SB>
//			ConvertINT32ToExitGrid( pShadow->iExitGridInfo, pExitGrid );
			memcpy(pExitGrid, pShadow->pExitGridInfo, sizeof(EXITGRID));
//</SB>
			return TRUE;
		}
		pShadow = pShadow->pNext;
	}
	pExitGrid->ubGotoSectorX = 0;
	pExitGrid->ubGotoSectorY = 0;
	pExitGrid->ubGotoSectorZ = 0;
	pExitGrid->usGridNo = 0;
	return FALSE;
}

BOOLEAN	ExitGridAtGridNo( UINT32 usMapIndex )
{
	LEVELNODE *pShadow;
	pShadow = gpWorldLevelData[ usMapIndex ].pShadowHead;
	//Search through object layer for an exitgrid
	while( pShadow )
	{
		if ( pShadow->uiFlags & LEVELNODE_EXITGRID )
		{
			return TRUE;
		}
		pShadow = pShadow->pNext;
	}
	return FALSE;
}

BOOLEAN	GetExitGridLevelNode( UINT32 usMapIndex, LEVELNODE **ppLevelNode )
{
	LEVELNODE *pShadow;
	pShadow = gpWorldLevelData[ usMapIndex ].pShadowHead;
	//Search through object layer for an exitgrid
	while( pShadow )
	{
		if ( pShadow->uiFlags & LEVELNODE_EXITGRID )
		{
			*ppLevelNode = pShadow;
			return TRUE;
		}
		pShadow = pShadow->pNext;
	}
	return FALSE;
}


void AddExitGridToWorld( INT32 iMapIndex, EXITGRID *pExitGrid )
{
	LEVELNODE *pShadow, *tail;
	pShadow = gpWorldLevelData[ iMapIndex ].pShadowHead;

	//Search through object layer for an exitgrid
	while( pShadow )
	{
		tail = pShadow;
		if( pShadow->uiFlags & LEVELNODE_EXITGRID )
		{ //we have found an existing exitgrid in this node, so replace it with the new information.
//<SB>
//			pShadow->iExitGridInfo = ConvertExitGridToINT32( pExitGrid );
			memcpy(gpExitGrids + guiExitGridsCount, pExitGrid, sizeof(EXITGRID));
			pShadow->pExitGridInfo = gpExitGrids + guiExitGridsCount;
			guiExitGridsCount++;
//</SB>
			//SmoothExitGridRadius( (INT16)iMapIndex, 0 );
			return;
		}
		pShadow = pShadow->pNext;
	}

	// Add levelnode
	AddShadowToHead( iMapIndex, MOCKFLOOR1 );
	// Get new node
	pShadow = gpWorldLevelData[ iMapIndex ].pShadowHead;

	//fill in the information for the new exitgrid levelnode.
//<SB>
//	pShadow->iExitGridInfo = ConvertExitGridToINT32( pExitGrid );
	memcpy(gpExitGrids + guiExitGridsCount, pExitGrid, sizeof(EXITGRID));
	pShadow->pExitGridInfo = gpExitGrids + guiExitGridsCount;
	guiExitGridsCount++;
//</SB>
	pShadow->uiFlags |= ( LEVELNODE_EXITGRID | LEVELNODE_HIDDEN );

	//Add the exit grid to the sector, only if we call ApplyMapChangesToMapTempFile() first.
	if( !gfEditMode && !gfLoadingExitGrids )
	{
		AddExitGridToMapTempFile( iMapIndex, pExitGrid, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	}
}

void RemoveExitGridFromWorld( INT32 iMapIndex )
{
	UINT16 usDummy;
	if( TypeExistsInShadowLayer( iMapIndex, MOCKFLOOR, &usDummy ) )
	{
		RemoveAllShadowsOfTypeRange( iMapIndex, MOCKFLOOR, MOCKFLOOR );
	}
}

//dnl ch42 250909
EXITGRID& EXITGRID::operator=(const _OLD_EXITGRID& src)
{
	if((void*)this != (void*)&src)
	{
		usGridNo = src.usGridNo;
		ubGotoSectorX = src.ubGotoSectorX;
		ubGotoSectorY = src.ubGotoSectorY;
		ubGotoSectorZ = src.ubGotoSectorZ;
	}
	return(*this);

}

BOOLEAN EXITGRID::Load(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	if(dMajorMapVersion < 7.0)
	{
		_OLD_EXITGRID OldExitGrid;
		LOADDATA(&OldExitGrid, *hBuffer, 5);// Never use sizeof(_OLD_EXITGRID) because return 6 and all maps was saved with 5 bytes
		*this = OldExitGrid;
	}
	else
		LOADDATA(this, *hBuffer, sizeof(EXITGRID));
	return(TRUE);
}

BOOLEAN EXITGRID::Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	PTR pData = this;
	UINT32 uiBytesToWrite = sizeof(EXITGRID);
	_OLD_EXITGRID OldExitGrid;
	if(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION)
	{
		OldExitGrid.usGridNo = usGridNo;
		OldExitGrid.ubGotoSectorX = ubGotoSectorX;
		OldExitGrid.ubGotoSectorY = ubGotoSectorY;
		OldExitGrid.ubGotoSectorZ = ubGotoSectorZ;
		pData = &OldExitGrid;
		uiBytesToWrite = 5;// Never use sizeof(_OLD_EXITGRID) because return 6 and all maps was saved with 5 bytes
	}
	UINT32 uiBytesWritten = 0;
	FileWrite(hFile, pData, uiBytesToWrite, &uiBytesWritten);
	if(uiBytesToWrite == uiBytesWritten)
		return(TRUE);
	return(FALSE);
}

void LoadExitGrids(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	UINT16 usNumExitGrids;
	INT32 usMapIndex;
	EXITGRID ExitGrid;

	// New world is loading so trash all old EXITGRID's
	memset(gpExitGrids, 0, sizeof(gpExitGrids));
	guiExitGridsCount = 0;

	gfLoadingExitGrids = TRUE;
	LOADDATA(&usNumExitGrids, *hBuffer, sizeof(usNumExitGrids));
	for(int i=0; i<usNumExitGrids; i++)
	{
		if(dMajorMapVersion < 7.0)
		{
			UINT16 usOldMapIndex;
			LOADDATA(&usOldMapIndex, *hBuffer, sizeof(usOldMapIndex));
			usMapIndex = usOldMapIndex;
		}
		else
			LOADDATA(&usMapIndex, *hBuffer, sizeof(usMapIndex));
		ExitGrid.Load(hBuffer, dMajorMapVersion);
		//dnl ch44 280909 EXITGRID translation
		gMapTrn.GetTrnCnt(usMapIndex);
		//gMapTrn.GetTrnCnt(ExitGrid.usGridNo);//dnl ch56 151009 This is gridno in sector which size you don't know, so no translation here
		AddExitGridToWorld(usMapIndex, &ExitGrid);
	}
	gfLoadingExitGrids = FALSE;
}

void SaveExitGrids(HWFILE hFile, UINT16 usNumExitGrids, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	UINT32 uiBytesWritten;
	FileWrite(hFile, &usNumExitGrids, sizeof(usNumExitGrids), &uiBytesWritten);
	EXITGRID ExitGrid;
	UINT32 usNumSaved = 0;
	for(INT32 i=0; i<WORLD_MAX; i++)
	{
		if(GetExitGrid(i, &ExitGrid))
		{
			if(dMajorMapVersion < 7.0)
			{
				UINT16 usOldMapIndex = i;
				FileWrite(hFile, &usOldMapIndex, sizeof(usOldMapIndex), &uiBytesWritten);
			}
			else
				FileWrite(hFile, &i, sizeof(i), &uiBytesWritten);
			ExitGrid.Save(hFile, dMajorMapVersion, ubMinorMapVersion);
			usNumSaved++;
		}
	}
	//If these numbers aren't equal, something is wrong!
	Assert(usNumExitGrids == usNumSaved);
}

void AttemptToChangeFloorLevel( INT8 bRelativeZLevel )
{
	UINT8 ubLookForLevel=0;
	INT32 i;
	if( bRelativeZLevel != 1 && bRelativeZLevel != -1 )
		return;
	//Check if on ground level -- if so, can't go up!
	if( bRelativeZLevel == -1 && !gbWorldSectorZ )
	{
		ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_CANT_GO_UP ], ubLookForLevel );
		return;
	}
	//Check if on bottom level -- if so, can't go down!
	if( bRelativeZLevel == 1 && gbWorldSectorZ == 3 )
	{

		ScreenMsg( FONT_DKYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_CANT_GO_DOWN ], ubLookForLevel );
		return;
	}
	ubLookForLevel = (UINT8)(gbWorldSectorZ + bRelativeZLevel);
	for( i = 0; i < WORLD_MAX; i++ )
	{
		if( GetExitGrid( i, &gExitGrid ) )
		{
			if( gExitGrid.ubGotoSectorZ == ubLookForLevel )
			{ //found an exit grid leading to the goal sector!
				gfOverrideInsertionWithExitGrid = TRUE;
				//change all current mercs in the loaded sector, and move them
				//to the new sector.
				MoveAllGroupsInCurrentSectorToSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, ubLookForLevel );
				if( ubLookForLevel )
					ScreenMsg( FONT_YELLOW, MSG_INTERFACE, pMessageStrings[ MSG_ENTERING_LEVEL ], ubLookForLevel );
				else
					ScreenMsg( FONT_YELLOW, MSG_INTERFACE, pMessageStrings[ MSG_LEAVING_BASEMENT ] );

				SetCurrentWorldSector( gWorldSectorX, gWorldSectorY, ubLookForLevel );
				gfOverrideInsertionWithExitGrid = FALSE;
			}
		}
	}
}


INT32 FindGridNoFromSweetSpotCloseToExitGrid( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2;
	INT32 sGridNo;
	INT32		uiRange, uiLowestRange = 999999;
	INT32 sLowestGridNo=0;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;
	EXITGRID	ExitGrid;
	UINT8	ubGotoSectorX, ubGotoSectorY, ubGotoSectorZ;

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
	soldier.sGridNo = pSoldier->sGridNo;

	// OK, Get an exit grid ( if possible )
	if ( !GetExitGrid( sSweetGridNo, &ExitGrid ) )
	{
		return( NOWHERE );
	}

	// Copy our dest values.....
	ubGotoSectorX = ExitGrid.ubGotoSectorX;
	ubGotoSectorY = ExitGrid.ubGotoSectorY;
	ubGotoSectorZ = ExitGrid.ubGotoSectorZ;


	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = pSoldier->sGridNo + (WORLD_COLS * cnt1) + cnt2;
			if ( sGridNo >= 0 && sGridNo < WORLD_MAX )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	gfPlotPathToExitGrid = TRUE;

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( pSoldier->sGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = pSoldier->sGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if ( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) &&
				gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				// ATE: Added this check because for all intensive purposes, cavewalls will be not an OKDEST
				// but we want thenm too...
				if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) )
				{
					if ( GetExitGrid( sGridNo, &ExitGrid ) )
					{
						// Is it the same exitgrid?
						if ( ExitGrid.ubGotoSectorX == ubGotoSectorX && ExitGrid.ubGotoSectorY == ubGotoSectorY && ExitGrid.ubGotoSectorZ == ubGotoSectorZ )
						{
							uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

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
	}

	gfPlotPathToExitGrid = FALSE;
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


INT32 FindClosestExitGrid( SOLDIERTYPE *pSoldier, INT32 sSrcGridNo, INT8 ubRadius )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2;
	INT32 sGridNo;
	INT32		uiRange, uiLowestRange = 999999;
	INT32		sLowestGridNo=0;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	EXITGRID	ExitGrid;


	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSrcGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSrcGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) )
			{
				if ( GetExitGrid( sGridNo, &ExitGrid ) )
				{
					uiRange = GetRangeInCellCoordsFromGridNoDiff( sSrcGridNo, sGridNo );

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
		return( sLowestGridNo );
	}
	else
	{
		return( NOWHERE );
	}
}




