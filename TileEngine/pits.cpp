#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "types.h"
	#include "pits.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "Simple Render Utils.h"
	#include "SaveLoadMap.h"
	#include "Sys Globals.h"
	#include "strategicmap.h"
	#include "Isometric Utils.h"
	#include "Sound Control.h"
	#include "animation control.h"
	#include "strategic.h"
	#include "Action Items.h"
#endif

//used by editor
BOOLEAN gfShowPits = FALSE;

BOOLEAN gfLoadPitsWithoutArming = FALSE;

// Flugente: while I don't really see a use to the pit mechanism at the moment, I tested it a bit. I highly recommend to use
// AddStructToHead(...) and AddObjectToHead(...) along the existing AddObjectToTail(...)-calls. Otherwise the water tiles disappear when reloading the map.
// It is also highly advisable to perform CompileWorldTerrainIDs() (but only for each relevant gridno, this needs a separate function), otherwise the water tiles
// do not behave like water tiles at all (they are still treated like normal floor tiles).
// An issue remaining for me is that after calling this function, mercs standing on new water tiles do not use the water aniamtions - one has ti move them to anothe tile, or save and reload.
// It might be useful to do SearchForOtherMembersWithinPitRadiusAndMakeThemFall() but instead reset their animations in there.
// The pit mechnism is notable for its exitgrid mechanism however, that allows painless creation of new exitgrids, and sends mercs somewhere else.
void Add3X3Pit( INT32 iMapIndex )
{
	EXITGRID ExitGrid;
	if( !gfEditMode )
		ApplyMapChangesToMapTempFile( TRUE );
	AddObjectToTail( iMapIndex + WORLD_COLS-1,	REGWATERTEXTURE1 );
	AddObjectToTail( iMapIndex - 1,				REGWATERTEXTURE2 );
	AddObjectToTail( iMapIndex - WORLD_COLS+1,	REGWATERTEXTURE3 );
	AddObjectToTail( iMapIndex + WORLD_COLS,	REGWATERTEXTURE4 );
	AddObjectToTail( iMapIndex,					REGWATERTEXTURE5 );
	AddObjectToTail( iMapIndex - WORLD_COLS,	REGWATERTEXTURE6 );
	AddObjectToTail( iMapIndex + WORLD_COLS+1,	REGWATERTEXTURE7 );
	AddObjectToTail( iMapIndex + 1,				REGWATERTEXTURE8 );
	AddObjectToTail( iMapIndex - WORLD_COLS-1,	REGWATERTEXTURE9 );
	if( !gfEditMode )
	{
		//Add the exitgrids associated with the pit.
		ExitGrid.ubGotoSectorX = (UINT8)gWorldSectorX;
		ExitGrid.ubGotoSectorY = (UINT8)gWorldSectorY;
		ExitGrid.ubGotoSectorZ = (UINT8)(gbWorldSectorZ+1);
		ExitGrid.usGridNo = iMapIndex;
		AddExitGridToWorld( iMapIndex + WORLD_COLS-1,	&ExitGrid );
		AddExitGridToWorld( iMapIndex - 1,				&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS+1,	&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS,		&ExitGrid );
		AddExitGridToWorld( iMapIndex,					&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS+1,	&ExitGrid );
		AddExitGridToWorld( iMapIndex + 1,				&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS-1,	&ExitGrid );
		RecompileLocalMovementCostsFromRadius( iMapIndex, 2 );
	}

	MarkWorldDirty();
	if( !gfEditMode )
		ApplyMapChangesToMapTempFile( FALSE );
}

void Add5X5Pit( INT32 iMapIndex )
{
	EXITGRID ExitGrid;
	if( !gfEditMode )
		ApplyMapChangesToMapTempFile( TRUE );
	AddObjectToTail( iMapIndex + WORLD_COLS*2-2, 	REGWATERTEXTURE10 );
	AddObjectToTail( iMapIndex + WORLD_COLS-2, 		REGWATERTEXTURE11 );
	AddObjectToTail( iMapIndex - 2,					REGWATERTEXTURE12 );
	AddObjectToTail( iMapIndex - WORLD_COLS+2, 		REGWATERTEXTURE13 );
	AddObjectToTail( iMapIndex - WORLD_COLS*2+2,	REGWATERTEXTURE14 );
	AddObjectToTail( iMapIndex + WORLD_COLS*2-1,	REGWATERTEXTURE15 );
	AddObjectToTail( iMapIndex + WORLD_COLS-1,		REGWATERTEXTURE16 );
	AddObjectToTail( iMapIndex - 1,					REGWATERTEXTURE17 );
	AddObjectToTail( iMapIndex - WORLD_COLS+1,		REGWATERTEXTURE18 );
	AddObjectToTail( iMapIndex - WORLD_COLS*2+1,	REGWATERTEXTURE19 );
	AddObjectToTail( iMapIndex + WORLD_COLS*2,		REGWATERTEXTURE20 );
	AddObjectToTail( iMapIndex + WORLD_COLS,		REGWATERTEXTURE21 );
	AddObjectToTail( iMapIndex,						REGWATERTEXTURE22 );
	AddObjectToTail( iMapIndex - WORLD_COLS,		REGWATERTEXTURE23 );
	AddObjectToTail( iMapIndex - WORLD_COLS*2,		REGWATERTEXTURE24 );
	AddObjectToTail( iMapIndex + WORLD_COLS*2+1,	REGWATERTEXTURE25 );
	AddObjectToTail( iMapIndex + WORLD_COLS+1,		REGWATERTEXTURE26 );
	AddObjectToTail( iMapIndex + 1,					REGWATERTEXTURE27 );
	AddObjectToTail( iMapIndex - WORLD_COLS-1,		REGWATERTEXTURE28 );
	AddObjectToTail( iMapIndex - WORLD_COLS*2-1,	REGWATERTEXTURE29 );
	AddObjectToTail( iMapIndex + WORLD_COLS*2+2,	REGWATERTEXTURE30 );
	AddObjectToTail( iMapIndex + WORLD_COLS+2,		REGWATERTEXTURE31 );
	AddObjectToTail( iMapIndex + 2,					REGWATERTEXTURE32 );
	AddObjectToTail( iMapIndex - WORLD_COLS-2,		REGWATERTEXTURE33 );
	AddObjectToTail( iMapIndex - WORLD_COLS*2-2,	REGWATERTEXTURE34 );
	if( !gfEditMode )
	{ //Add the exitgrids associated with the pit.
		ExitGrid.ubGotoSectorX = (UINT8)gWorldSectorX;
		ExitGrid.ubGotoSectorY = (UINT8)gWorldSectorY;
		ExitGrid.ubGotoSectorZ = (UINT8)(gbWorldSectorZ+1);
		ExitGrid.usGridNo = iMapIndex;
		AddExitGridToWorld( iMapIndex + WORLD_COLS*2-2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS-2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - 2,					&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS+2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS*2+2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS*2-1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS-1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - 1,					&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS+1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS*2+1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS*2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS,			&ExitGrid );
		AddExitGridToWorld( iMapIndex,						&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS,			&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS*2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS*2+1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS+1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + 1,					&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS-1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS*2-1,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS*2+2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + WORLD_COLS+2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex + 2,					&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS-2,		&ExitGrid );
		AddExitGridToWorld( iMapIndex - WORLD_COLS*2-2,		&ExitGrid );
		RecompileLocalMovementCostsFromRadius( iMapIndex, 3 );
	}
	MarkWorldDirty();
	if( !gfEditMode )
		ApplyMapChangesToMapTempFile( FALSE );
}

void Remove3X3Pit( INT32 iMapIndex )
{
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS-1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - 1,				REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS+1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex,					REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS+1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + 1,				REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS-1,	REGWATERTEXTURE, REGWATERTEXTURE );
	MarkWorldDirty();
}

void Remove5X5Pit( INT32 iMapIndex )
{
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS*2-2,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS-2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - 2,					REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS+2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS*2+2,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS*2-1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS-1,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - 1,					REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS+1,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS*2+1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS*2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex,						REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS*2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS*2+1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS+1,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + 1,					REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS-1,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS*2-1,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS*2+2,	REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + WORLD_COLS+2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex + 2,					REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS-2,		REGWATERTEXTURE, REGWATERTEXTURE );
	RemoveAllObjectsOfTypeRange( iMapIndex - WORLD_COLS*2-2,	REGWATERTEXTURE, REGWATERTEXTURE );
	MarkWorldDirty();
}

void AddAllPits()
{
	UINT32 i;
	for( i = 0; i < guiNumWorldItems; i++ )
	{
		if( gWorldItems[ i ].object.usItem == ACTION_ITEM )
		{
			if( gWorldItems[ i ].object[0]->data.misc.bActionValue == ACTION_ITEM_SMALL_PIT )
				Add3X3Pit( gWorldItems[ i ].sGridNo );
			else if( gWorldItems[ i ].object[0]->data.misc.bActionValue == ACTION_ITEM_LARGE_PIT )
				Add5X5Pit( gWorldItems[ i ].sGridNo );
		}
	}
}

void RemoveAllPits()
{
	UINT32 i;
	for( i = 0; i < guiNumWorldItems; i++ )
	{
		if( gWorldItems[ i ].object.usItem == ACTION_ITEM )
		{
			if( gWorldItems[ i ].object[0]->data.misc.bActionValue == ACTION_ITEM_SMALL_PIT )
				Remove3X3Pit( gWorldItems[ i ].sGridNo );
			else if( gWorldItems[ i ].object[0]->data.misc.bActionValue == ACTION_ITEM_LARGE_PIT )
				Remove5X5Pit( gWorldItems[ i ].sGridNo );
		}
	}
}

void SearchForOtherMembersWithinPitRadiusAndMakeThemFall( INT32 sGridNo, INT16 sRadius )
{
	INT32 x, y, sNewGridNo;
	UINT8	ubID;
	SOLDIERTYPE *pSoldier;


	PlayJA2Sample( CAVE_COLLAPSE, RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
	for( y = -sRadius; y <= sRadius; y++ ) for( x = -sRadius; x <= sRadius; x++ )
	{
		sNewGridNo = sGridNo + y * WORLD_COLS + x;
		//Validate gridno location, and check if there are any mercs here.	If there are
		//any mercs, we want them to fall below.	The exitgrid already exists at this location
		if( GridNoOnVisibleWorldTile( sNewGridNo ) )
		{

			// Check if buddy exists here.....
			ubID = WhoIsThere2( sNewGridNo, 0 );

			if ( ubID != NOBODY )
			{
				// OK, make guy fall...
				// Set data to look for exit grid....
				pSoldier = MercPtrs[ ubID ];

				pSoldier->aiData.uiPendingActionData4 = sNewGridNo;

				pSoldier->EVENT_InitNewSoldierAnim( FALL_INTO_PIT, 0 , FALSE );
			}
		}
	}
}

void HandleFallIntoPitFromAnimation( UINT8 ubID )
{
	SOLDIERTYPE *pSoldier = MercPtrs[ ubID ];
	EXITGRID ExitGrid;
	INT32 sPitGridNo;
	// OK, get exit grid...

	sPitGridNo = pSoldier->aiData.uiPendingActionData4;

	GetExitGrid( sPitGridNo, &ExitGrid );

	// Given exit grid, make buddy move to next sector....
	pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	pSoldier->usStrategicInsertionData = ExitGrid.usGridNo;

	pSoldier->sSectorX = ExitGrid.ubGotoSectorX;
	pSoldier->sSectorY = ExitGrid.ubGotoSectorY;
	pSoldier->bSectorZ = ExitGrid.ubGotoSectorZ;

	// Remove from world......
	RemoveSoldierFromTacticalSector( pSoldier, TRUE );

	HandleSoldierLeavingSectorByThemSelf( pSoldier );

	pSoldier->SetSoldierHeight( 0 );

}
