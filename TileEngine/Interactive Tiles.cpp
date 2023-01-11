#include "builddefines.h"

	#include "sysutil.h"
	#include "renderworld.h"
	#include "Sound Control.h"
	#include "Interactive Tiles.h"
	#include "interface cursors.h"
	#include "worldman.h"
	#include "points.h"
	#include "structure wrap.h"
	#include "tile animation.h"
	#include "tile cache.h"
	#include "handle doors.h"
	#include "Dialogue Control.h"
	#include "english.h"
	#include "NPC.h"
	#include "Explosion Control.h"
	#include "Text.h"

#ifdef JA2TESTVERSION
	#include "message.h"
#endif


#define		MAX_INTTILE_STACK							10

typedef struct
{
	INT32 sGridNo;
	UINT8					ubFlags;
	INT16					sTileIndex;
	INT16					sMaxScreenY;
	INT16					sHeighestScreenY;
	BOOLEAN				fFound;
	LEVELNODE			*pFoundNode;
	INT32					sFoundGridNo;
	UINT16				usStructureID;
	BOOLEAN				fStructure;

} CUR_INTERACTIVE_TILE;


typedef struct
{
	INT8										bNum;
	CUR_INTERACTIVE_TILE		bTiles[ MAX_INTTILE_STACK ];
	INT8										bCur;

} INTERACTIVE_TILE_STACK_TYPE;


INTERACTIVE_TILE_STACK_TYPE		gCurIntTileStack;
BOOLEAN												gfCycleIntTile = FALSE;


CUR_INTERACTIVE_TILE	gCurIntTile;
BOOLEAN								gfOverIntTile				= FALSE;

// Values to determine if we should check or not
INT16		gsINTOldRenderCenterX = 0;
INT16		gsINTOldRenderCenterY = 0;
UINT16	gusINTOldMousePosX = 0;
UINT16	gusINTOldMousePosY = 0;


BOOLEAN RefinePointCollisionOnStruct( INT32 sGridNo, INT16 sTestX, INT16 sTestY, INT16 sSrcX, INT16 sSrcY, LEVELNODE *pNode );
BOOLEAN CheckVideoObjectScreenCoordinateInData( HVOBJECT hSrcVObject, UINT16 usIndex, INT32 iTextX, INT32 iTestY );
BOOLEAN RefineLogicOnStruct( INT32 sGridNo, LEVELNODE *pNode );


BOOLEAN InitInteractiveTileManagement( )
{
	return( TRUE );
}

void ShutdownInteractiveTileManagement( )
{
}

BOOLEAN AddInteractiveTile( INT32 sGridNo, LEVELNODE *pLevelNode, UINT32 uiFlags, UINT16 usType )
{
	return( TRUE );
}

BOOLEAN StartInteractiveObject( INT32 sGridNo, UINT16 usStructureID, SOLDIERTYPE *pSoldier, UINT8 ubDirection )
{
	STRUCTURE * pStructure;

	// ATE: Patch fix: Don't allow if alreay in animation
	if ( pSoldier->usAnimState == OPEN_STRUCT || pSoldier->usAnimState == OPEN_STRUCT_CROUCHED ||
		pSoldier->usAnimState == BEGIN_OPENSTRUCT || pSoldier->usAnimState == BEGIN_OPENSTRUCT_CROUCHED )
	{
	return( FALSE );
	}

	pStructure = FindStructureByID( sGridNo, usStructureID );
	if (pStructure == NULL)
	{
		return( FALSE );
	}
	if (pStructure->fFlags & STRUCTURE_ANYDOOR)
	{
		// Add soldier event for opening door....
		pSoldier->aiData.ubPendingAction = MERC_OPENDOOR;
		pSoldier->aiData.uiPendingActionData1 = usStructureID;
		pSoldier->aiData.sPendingActionData2	= sGridNo;
		pSoldier->aiData.bPendingActionData3	= ubDirection;
		pSoldier->aiData.ubPendingActionAnimCount = 0;


	}
	else
	{
		// Add soldier event for opening door....
		pSoldier->aiData.ubPendingAction = MERC_OPENSTRUCT;
		pSoldier->aiData.uiPendingActionData1 = usStructureID;
		pSoldier->aiData.sPendingActionData2	= sGridNo;
		pSoldier->aiData.bPendingActionData3	= ubDirection;
		pSoldier->aiData.ubPendingActionAnimCount = 0;

	}

	return( TRUE );
}


BOOLEAN CalcInteractiveObjectAPs( SOLDIERTYPE * pSoldier, INT32 sGridNo, STRUCTURE * pStructure, INT16 *psAPCost, INT16 *psBPCost ) // SANDRO - added argument
{
	if (pStructure == NULL)
	{
		return( FALSE );
	}
	if (pStructure->fFlags & STRUCTURE_ANYDOOR)
	{
		// For doors, if open, we can safely add APs for closing
		// If closed, we do not know what to do yet...
		//if ( pStructure->fFlags & STRUCTURE_OPEN )
		//{
			// SANDRO - changed this
			*psAPCost = GetAPsToOpenDoor( pSoldier );
			*psBPCost = APBPConstants[AP_OPEN_DOOR];
		//}
		//else
		//{
		//	*psAPCost = 0;
		//	*psBPCost = 0;
		//}
	}
	else
	{
		// SANDRO - changed this
		*psAPCost = GetAPsToOpenDoor( pSoldier );
		*psBPCost = APBPConstants[AP_OPEN_DOOR];
	}

	return( TRUE );
}


BOOLEAN InteractWithInteractiveObject( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection )
{
	BOOLEAN	fDoor = FALSE;

	if (pStructure == NULL)
	{
		return( FALSE );
	}

	if (pStructure->fFlags & STRUCTURE_ANYDOOR)
	{
		fDoor = TRUE;
	}

	InteractWithOpenableStruct( pSoldier, pStructure, ubDirection, fDoor );

	return( TRUE );
}


BOOLEAN SoldierHandleInteractiveObject( SOLDIERTYPE *pSoldier )
{
	STRUCTURE			*pStructure;
	UINT16				usStructureID;
	INT32 sGridNo;

	sGridNo					= pSoldier->aiData.sPendingActionData2;
	usStructureID		= (UINT16)pSoldier->aiData.uiPendingActionData1;

	// HANDLE SOLDIER ACTIONS
	pStructure = FindStructureByID( sGridNo, usStructureID );
	if (pStructure == NULL)
	{
		//DEBUG MSG!
		return( FALSE );
	}

	return( HandleOpenableStruct( pSoldier, sGridNo, pStructure ) );
}

void HandleStructChangeFromGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	STRUCTURE			*pStructure, *pNewStructure;
	ITEM_POOL			*pItemPool;
	BOOLEAN		fDidMissingQuote = FALSE;

	pStructure = FindStructure( sGridNo, STRUCTURE_OPENABLE );

	if ( pStructure == NULL )
	{
#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"ERROR: Told to handle struct that does not exist at %d.", sGridNo );
#endif
		return;
	}

	// Do sound...
	if ( !( pStructure->fFlags & STRUCTURE_OPEN ) )
	{
		// Play Opening sound...
		PlayJA2Sample( GetStructureOpenSound( pStructure, FALSE ), RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
	}
	else
	{
		// Play Opening sound...
		PlayJA2Sample( ( GetStructureOpenSound( pStructure, TRUE ) ), RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
	}

	// ATE: Don't handle switches!
	if ( !( pStructure->fFlags & STRUCTURE_SWITCH ) )
	{
		if ( pSoldier->bTeam == gbPlayerNum )
		{
			if ( sGridNo == BOBBYR_SHIPPING_DEST_GRIDNO && gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X && gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y && gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z && CheckFact( FACT_PABLOS_STOLE_FROM_LATEST_SHIPMENT, 0 ) && !(CheckFact( FACT_PLAYER_FOUND_ITEMS_MISSING, 0) ) )
			{
			SetFactFalse( 38); // Reset whether we punched Pablo lately
			SetFactFalse( FACT_PABLO_RETURNED_GOODS);
			SayQuoteFromNearbyMercInSector( BOBBYR_SHIPPING_DEST_GRIDNO, 3, QUOTE_STUFF_MISSING_DRASSEN );
		 fDidMissingQuote = TRUE;
			}
		}
		else if ( pSoldier->bTeam == CIV_TEAM )
		{
			if ( pSoldier->ubProfile != NO_PROFILE )
			{
				TriggerNPCWithGivenApproach( pSoldier->ubProfile, APPROACH_DONE_OPEN_STRUCTURE, FALSE );
			}
		}


		// LOOK for item pool here...
		if ( GetItemPool( sGridNo, &pItemPool, pSoldier->pathing.bLevel ) )
		{
			// Update visiblity....
			if ( !( pStructure->fFlags & STRUCTURE_OPEN ) )
			{
				BOOLEAN fDoHumm			= TRUE;
				BOOLEAN	fDoLocators = TRUE;

				if ( pSoldier->bTeam != gbPlayerNum )
				{
					fDoHumm			= FALSE;
					fDoLocators = FALSE;
				}

				// Look for ownership here....
				if ( 	gWorldItems[ pItemPool->iItemIndex ].object.usItem == OWNERSHIP )
				{
					fDoHumm			= FALSE;
					TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, BATTLE_SOUND_NOTHING , 500 );
				}

				// If now open, set visible...
				SetItemPoolVisibilityOn( pItemPool, ANY_VISIBILITY_VALUE, fDoLocators );

				// Display quote!
				//TacticalCharacterDialogue( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ) );

				// ATE: Check now many things in pool.....
		if ( !fDidMissingQuote )
		{
				if ( pItemPool->pNext != NULL )
				{
					if ( pItemPool->pNext->pNext != NULL )
					{
						fDoHumm = FALSE;

						TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, BATTLE_SOUND_COOL1 , 500 );

					}
				}

				if ( fDoHumm )
				{
					TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, BATTLE_SOUND_HUMM , 500 );
				}
		}
			}
			else
			{
				SetItemPoolVisibilityHidden( pItemPool );
			}
		}
		else
		{
			if ( !( pStructure->fFlags & STRUCTURE_OPEN ) )
			{
				TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND, BATTLE_SOUND_NOTHING , 500 );
			}
		}
	}

	// Deduct points!
	// CalcInteractiveObjectAPs( sGridNo, pStructure, &sAPCost, &sBPCost );
	// DeductPoints( pSoldier, sAPCost, sBPCost );



	pNewStructure = SwapStructureForPartnerAndStoreChangeInMap( sGridNo, pStructure );
	if ( pNewStructure != NULL)
	{
		RecompileLocalMovementCosts( sGridNo );
		SetRenderFlags( RENDER_FLAG_FULL );
		if ( pNewStructure->fFlags & STRUCTURE_SWITCH )
		{
			// just turned a switch on!
			ActivateSwitchInGridNo( pSoldier->ubID, sGridNo );
		}
	}

}



UINT32 GetInteractiveTileCursor( UINT32 uiOldCursor, BOOLEAN fConfirm )
{
	LEVELNODE	*pIntNode;
	STRUCTURE	*pStructure;
	INT32 sGridNo;

	// OK, first see if we have an in tile...
	pIntNode = GetCurInteractiveTileGridNoAndStructure( &sGridNo, &pStructure );

	if ( pIntNode != NULL && pStructure != NULL )
	{
		if( pStructure->fFlags & STRUCTURE_ANYDOOR )
		{
			SetDoorString( sGridNo );

			if ( fConfirm )
			{
				return( OKHANDCURSOR_UICURSOR );
			}
			else
			{
				return( NORMALHANDCURSOR_UICURSOR );
			}

		}
		else
		{
		if( pStructure->fFlags & STRUCTURE_SWITCH )
		{
			wcscpy( gzIntTileLocation, gzLateLocalizedString[ 25 ] );
			gfUIIntTileLocation = TRUE;
		}


			if ( fConfirm )
			{
				return( OKHANDCURSOR_UICURSOR );
			}
			else
			{
				return( NORMALHANDCURSOR_UICURSOR );
			}
		}

	}

	return( uiOldCursor );
}

void SetActionModeDoorCursorText( )
{
	LEVELNODE	*pIntNode;
	STRUCTURE	*pStructure;
	INT32 sGridNo;

	// If we are over a merc, don't
	if ( gfUIFullTargetFound )
	{
	return;
	}

	// OK, first see if we have an in tile...
	pIntNode = GetCurInteractiveTileGridNoAndStructure( &sGridNo, &pStructure );

	if ( pIntNode != NULL && pStructure != NULL )
	{
		if( pStructure->fFlags & STRUCTURE_ANYDOOR )
		{
			SetDoorString( sGridNo );
		}
	}
}


void GetLevelNodeScreenRect( LEVELNODE *pNode, SGPRect *pRect, INT16 sXPos, INT16 sYPos, INT32 sGridNo )
{
		INT16 sScreenX, sScreenY;
		INT16 sOffsetX, sOffsetY;
		INT16 sTempX_S, sTempY_S;
		ETRLEObject *pTrav;
		UINT32 usHeight, usWidth;
		TILE_ELEMENT *TileElem;



		// Get 'TRUE' merc position
		sOffsetX = sXPos - gsRenderCenterX;
		sOffsetY = sYPos - gsRenderCenterY;

		FromCellToScreenCoordinates( sOffsetX, sOffsetY, &sTempX_S, &sTempY_S );

		if ( pNode->uiFlags & LEVELNODE_CACHEDANITILE )
		{
			pTrav = &(gpTileCache[ pNode->pAniTile->sCachedTileID ].pImagery->vo->pETRLEObject[ pNode->pAniTile->sCurrentFrame ] );
		}
		else
		{
			TileElem = &(gTileDatabase[pNode->usIndex]);

			//Adjust for current frames and animations....
			if ( TileElem->uiFlags & ANIMATED_TILE)
			{
				Assert( TileElem->pAnimData != NULL );
				TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[TileElem->pAnimData->bCurrentFrame]];
			}
			else if( ( pNode->uiFlags & LEVELNODE_ANIMATION ) )
			{
				if ( pNode->sCurrentFrame != -1	)
				{
					Assert( TileElem->pAnimData != NULL );
					TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[pNode->sCurrentFrame]];
				}
			}

			pTrav = &(TileElem->hTileSurface->pETRLEObject[ TileElem->usRegionIndex ] );
		}

		sScreenX = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) + (INT16)sTempX_S;
		sScreenY = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) + (INT16)sTempY_S;

		// Adjust for offset position on screen
		sScreenX = sScreenX - gsRenderWorldOffsetX;
		sScreenY = sScreenY - gsRenderWorldOffsetY;
		sScreenY = sScreenY - gpWorldLevelData[ sGridNo ].sHeight;

		// Adjust based on interface level
		if ( gsInterfaceLevel > 0 )
		{
			sScreenY += ROOF_LEVEL_HEIGHT;
		}

		// Adjust for render height
		sScreenY = sScreenY + gsRenderHeight;



		usHeight				= (UINT32)pTrav->usHeight;
		usWidth					= (UINT32)pTrav->usWidth;

		// Add to start position of dest buffer
		sScreenX += ( pTrav->sOffsetX - ( WORLD_TILE_X/2 ) );
		sScreenY += ( pTrav->sOffsetY - ( WORLD_TILE_Y/2 ) );

		// Adjust y offset!
		sScreenY += ( WORLD_TILE_Y/2);

		pRect->iLeft		= sScreenX;
		pRect->iTop			= sScreenY;
		pRect->iBottom	= sScreenY + usHeight;
		pRect->iRight		= sScreenX + usWidth;
}




void CompileInteractiveTiles( )
{

}


void LogMouseOverInteractiveTile( INT32 sGridNo )
{
	SGPRect				aRect;
	INT16					sXMapPos, sYMapPos, sScreenX, sScreenY;
	LEVELNODE			*pNode;

	// OK, for now, don't allow any interactive tiles on higher interface level!
	if ( gsInterfaceLevel > 0 )
	{
		return;
	}

	// Also, don't allow for mercs who are on upper level...
	if ( gusSelectedSoldier != NOBODY && MercPtrs[ gusSelectedSoldier ]->pathing.bLevel == 1 )
	{
		return;
	}

	// Get World XY From gridno
	ConvertGridNoToCellXY( sGridNo, &sXMapPos, &sYMapPos );

	// Set mouse stuff
	sScreenX = gusMouseXPos;
	sScreenY = gusMouseYPos;

	pNode = gpWorldLevelData[ sGridNo ].pStructHead;

	while( pNode != NULL )
	{
		{
			GetLevelNodeScreenRect( pNode, &aRect, sXMapPos, sYMapPos , sGridNo );

			// Make sure we are always on guy if we are on same gridno
			if ( IsPointInScreenRect( sScreenX, sScreenY, &aRect ) )
			{
				// OK refine it!
				if ( RefinePointCollisionOnStruct( sGridNo, sScreenX, sScreenY, (INT16)aRect.iLeft, (INT16)aRect.iBottom, pNode ) )
				{
					// Do some additional checks here!
					if ( RefineLogicOnStruct( sGridNo, pNode ) )
					{

						gCurIntTile.fFound = TRUE;

						// Only if we are not currently cycling....
						if ( !gfCycleIntTile )
						{
							// Accumulate them!
							gCurIntTileStack.bTiles[ gCurIntTileStack.bNum ].pFoundNode		= pNode;
							gCurIntTileStack.bTiles[ gCurIntTileStack.bNum ].sFoundGridNo = sGridNo;
							gCurIntTileStack.bNum++;


							// Determine if it's the best one
							if ( aRect.iBottom > gCurIntTile.sHeighestScreenY )
							{
								gCurIntTile.sMaxScreenY = (UINT16)aRect.iBottom;
								gCurIntTile.sHeighestScreenY = gCurIntTile.sMaxScreenY;

								// Set it!
								gCurIntTile.pFoundNode = pNode;
								gCurIntTile.sFoundGridNo = sGridNo;

								// Set stack current one...
								gCurIntTileStack.bCur = gCurIntTileStack.bNum-1;
							}
						}
					}
				}
			}

			pNode = pNode->pNext;
		}
	}

}


LEVELNODE *InternalGetCurInteractiveTile( BOOLEAN fRejectItemsOnTop )
{
	LEVELNODE *pNode = NULL;
	STRUCTURE	*pStructure = NULL;

	// OK, Look for our tile!

	// Check for shift down!
	if ( _KeyDown( SHIFT ) )
	{
		return( NULL );
	}


	if ( gfOverIntTile )
	{
		pNode = gpWorldLevelData[ gCurIntTile.sGridNo	].pStructHead;

		while( pNode != NULL )
		{
			if ( pNode->usIndex == gCurIntTile.sTileIndex )
			{
				if ( fRejectItemsOnTop )
				{
					// get strucuture here...
					if ( gCurIntTile.fStructure )
					{
						pStructure = FindStructureByID( gCurIntTile.sGridNo, gCurIntTile.usStructureID );
						if (pStructure != NULL)
						{
							if ( pStructure->fFlags & STRUCTURE_HASITEMONTOP )
							{
								return( NULL );
							}
						}
						else
						{
							return( NULL );
						}
					}
				}

				return( pNode );
			}

			pNode = pNode->pNext;
		}
	}

	return( NULL );
}



LEVELNODE *GetCurInteractiveTile( )
{
	return( InternalGetCurInteractiveTile( TRUE ) );
}


LEVELNODE *GetCurInteractiveTileGridNo( INT32 *psGridNo )
{
	LEVELNODE *pNode;

	pNode = GetCurInteractiveTile( );

	if ( pNode != NULL )
	{
		*psGridNo = gCurIntTile.sGridNo;
	}
	else
	{
		*psGridNo = NOWHERE;
	}

	return( pNode );
}



LEVELNODE *ConditionalGetCurInteractiveTileGridNoAndStructure( INT32 *psGridNo, STRUCTURE **ppStructure, BOOLEAN fRejectOnTopItems )
{
	LEVELNODE *pNode;
	STRUCTURE	*pStructure;

	*ppStructure = NULL;

	pNode = InternalGetCurInteractiveTile( fRejectOnTopItems );

	if ( pNode != NULL )
	{
		*psGridNo = gCurIntTile.sGridNo;
	}
	else
	{
		*psGridNo = NOWHERE;
	}

	if ( pNode != NULL )
	{
		if ( gCurIntTile.fStructure )
		{
			pStructure = FindStructureByID( gCurIntTile.sGridNo, gCurIntTile.usStructureID );
			if (pStructure == NULL)
			{
				*ppStructure = NULL;
				return( NULL );
			}
			else
			{
				*ppStructure = pStructure;
			}
		}
	}

	return( pNode );
}


LEVELNODE *GetCurInteractiveTileGridNoAndStructure( INT32 *psGridNo, STRUCTURE **ppStructure )
{
	return( ConditionalGetCurInteractiveTileGridNoAndStructure( psGridNo, ppStructure, TRUE ) );
}


void BeginCurInteractiveTileCheck( UINT8 bCheckFlags )
{
	gfOverIntTile = FALSE;

	// OK, release our stack, stuff could be different!
	gfCycleIntTile = FALSE;

	// Reset some highest values
	gCurIntTile.sHeighestScreenY	= 0;
	gCurIntTile.fFound						= FALSE;
	gCurIntTile.ubFlags						= bCheckFlags;

	// Reset stack values
	gCurIntTileStack.bNum = 0;

}

void EndCurInteractiveTileCheck( )
{
	CUR_INTERACTIVE_TILE		*pCurIntTile;

	if ( gCurIntTile.fFound )
	{
		// Set our currently cycled guy.....
		if ( gfCycleIntTile )
		{
			// OK, we're over this cycled node
			pCurIntTile = &( gCurIntTileStack.bTiles[ gCurIntTileStack.bCur ] );
		}
		else
		{
			// OK, we're over this levelnode,
			pCurIntTile = &gCurIntTile;
		}

		gCurIntTile.sGridNo				= pCurIntTile->sFoundGridNo;
		gCurIntTile.sTileIndex	= pCurIntTile->pFoundNode->usIndex;

		if ( pCurIntTile->pFoundNode->pStructureData != NULL )
		{
			gCurIntTile.usStructureID			= pCurIntTile->pFoundNode->pStructureData->usStructureID;
			gCurIntTile.fStructure				= TRUE;
		}
		else
		{
			gCurIntTile.fStructure				= FALSE;
		}


		gfOverIntTile							= TRUE;

	}
	else
	{
		// If we are in cycle mode, end it
		if ( gfCycleIntTile )
		{
			gfCycleIntTile = FALSE;
		}
	}
}


BOOLEAN RefineLogicOnStruct( INT32 sGridNo, LEVELNODE *pNode )
{
	TILE_ELEMENT *TileElem;
	STRUCTURE		*pStructure;


	if ( pNode->uiFlags & LEVELNODE_CACHEDANITILE )
	{
		return ( FALSE );
	}


	TileElem = &(gTileDatabase[pNode->usIndex]);

	if ( gCurIntTile.ubFlags == INTILE_CHECK_SELECTIVE )
	{
		// See if we are on an interactable tile!
		// Try and get struct data from levelnode pointer
		pStructure = pNode->pStructureData;

		// If no data, quit
		if ( pStructure == NULL )
		{
			return( FALSE );
		}

		if ( !( pStructure->fFlags & ( STRUCTURE_OPENABLE | STRUCTURE_HASITEMONTOP ) ) )
		{
			return( FALSE );
		}

		if ( gusSelectedSoldier != NOBODY && MercPtrs[ gusSelectedSoldier ]->ubBodyType == ROBOTNOWEAPON )
		{
			return( FALSE );
		}

		// If we are a door, we need a different definition of being visible than other structs
		if ( pStructure->fFlags & STRUCTURE_ANYDOOR )
		{
			if ( !IsDoorVisibleAtGridNo( sGridNo ) )
			{
				return( FALSE );
			}

			// OK, For a OPENED door, addition requirements are: need to be in 'HAND CURSOR' mode...
			if ( pStructure->fFlags & STRUCTURE_OPEN )
			{
				//Are we in hand cursor mode?
				if ( gCurrentUIMode != HANDCURSOR_MODE && gCurrentUIMode != ACTION_MODE )
				{
					return( FALSE );
				}
			}

			// If this option is on...
			if ( !gGameSettings.fOptions[ TOPTION_SNAP_CURSOR_TO_DOOR ] )
			{
				if ( gCurrentUIMode != HANDCURSOR_MODE )
				{
					return( FALSE );
				}
			}
		}
		else
		{
			// IF we are a switch, reject in another direction...
			if ( pStructure->fFlags & STRUCTURE_SWITCH )
			{
				// Find a new gridno based on switch's orientation...				
				INT32 sNewGridNo = NOWHERE;

				switch( pStructure->pDBStructureRef->pDBStructure->ubWallOrientation )
				{
					case OUTSIDE_TOP_LEFT:
					case INSIDE_TOP_LEFT:

						// Move south...
						sNewGridNo = NewGridNo( sGridNo, DirectionInc( SOUTH ) );
						break;

					case OUTSIDE_TOP_RIGHT:
					case INSIDE_TOP_RIGHT:

						// Move east...
						sNewGridNo = NewGridNo( sGridNo, DirectionInc( EAST ) );
						break;

				}
				
				if (!TileIsOutOfBounds(sNewGridNo))
				{
					// If we are hidden by a roof, reject it!
					if ( !gfBasement && IsRoofVisible2( sNewGridNo ) && !( gTacticalStatus.uiFlags&SHOW_ALL_ITEMS ) )
					{
						return( FALSE );
					}
				}
			}
			else
			{
				// If we are hidden by a roof, reject it!
				if ( !gfBasement && IsRoofVisible( sGridNo ) && !( gTacticalStatus.uiFlags&SHOW_ALL_ITEMS ) )
				{
					return( FALSE );
				}
			}
		}

		// Check if it's a hidden struct and we have not revealed anything!
		if ( TileElem->uiFlags & HIDDEN_TILE )
		{
			if ( !IsHiddenStructureVisible( sGridNo, pNode->usIndex ) )
			{
				// Return false
				return( FALSE );
			}
		}
	}

	return( TRUE );
}


BOOLEAN RefinePointCollisionOnStruct( INT32 sGridNo, INT16 sTestX, INT16 sTestY, INT16 sSrcX, INT16 sSrcY, LEVELNODE *pNode )
{
	TILE_ELEMENT *TileElem;

	if ( pNode->uiFlags & LEVELNODE_CACHEDANITILE )
	{
		//Check it!
		return ( CheckVideoObjectScreenCoordinateInData( gpTileCache[ pNode->pAniTile->sCachedTileID ].pImagery->vo, pNode->pAniTile->sCurrentFrame, (INT32)( sTestX - sSrcX	), (INT32)( -1 * ( sTestY - sSrcY	) ) ) );

	}
	else
	{
		TileElem = &( gTileDatabase[pNode->usIndex] );

		//Adjust for current frames and animations....
		if ( TileElem->uiFlags & ANIMATED_TILE)
		{
				Assert( TileElem->pAnimData != NULL );
				TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[TileElem->pAnimData->bCurrentFrame]];
		}
		else if( ( pNode->uiFlags & LEVELNODE_ANIMATION ) )
		{
			if ( pNode->sCurrentFrame != -1	)
			{
				Assert( TileElem->pAnimData != NULL );
				TileElem = &gTileDatabase[TileElem->pAnimData->pusFrames[pNode->sCurrentFrame]];
			}
		}

		//Check it!
		return ( CheckVideoObjectScreenCoordinateInData( TileElem->hTileSurface, TileElem->usRegionIndex, (INT32)( sTestX - sSrcX	), (INT32)( -1 * ( sTestY - sSrcY	) ) ) );
	}
}


// This function will check the video object at SrcX and SrcY for the lack of transparency
// will return true if data found, else false
BOOLEAN CheckVideoObjectScreenCoordinateInData( HVOBJECT hSrcVObject, UINT16 usIndex, INT32 iTestX, INT32 iTestY )
{
	UINT32 uiOffset;
	UINT32 usHeight, usWidth;
	UINT8	*SrcPtr;
	UINT32 LineSkip;
	ETRLEObject *pTrav;
	BOOLEAN	fDataFound = FALSE;
	INT32	iTestPos, iStartPos;

	// Assertions
	Assert( hSrcVObject != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;


	// Calculate test position we are looking for!
	// Calculate from 0, 0 at top left!
	iTestPos	= ( ( usHeight - iTestY ) * usWidth ) + iTestX;
	iStartPos	= 0;
	LineSkip	= usWidth;

	SrcPtr= (UINT8 *)hSrcVObject->pPixData + uiOffset;

	__asm {

		mov		esi, SrcPtr
		mov		edi, iStartPos
		xor		eax, eax
		xor		ebx, ebx
		xor		ecx, ecx

BlitDispatch:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		BlitDoneLine

//BlitNonTransLoop:

		clc
		rcr		cl, 1
		jnc		BlitNTL2

		inc		esi

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1


BlitNTL2:
		clc
		rcr		cl, 1
		jnc		BlitNTL3

		add		esi, 2

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1


BlitNTL3:

		or		cl, cl
		jz		BlitDispatch

		xor		ebx, ebx

BlitNTL4:

		add		esi, 4

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1

		// Check
		cmp		edi, iTestPos
		je		BlitFound
		add		edi, 1

		dec		cl
		jnz		BlitNTL4

		jmp		BlitDispatch

BlitTransparent:

		and		ecx, 07fH
//		shl		ecx, 1
		add		edi, ecx
		jmp		BlitDispatch


BlitDoneLine:

// Here check if we have passed!
		cmp		edi, iTestPos
		jge		BlitDone

		dec		usHeight
		jz		BlitDone
//		add		edi, LineSkip
		jmp		BlitDispatch


BlitFound:

		mov		fDataFound, 1

BlitDone:
	}

	return(fDataFound);

}


BOOLEAN ShouldCheckForMouseDetections( )
{
	BOOLEAN fOK = FALSE;

	if ( gsINTOldRenderCenterX != gsRenderCenterX || gsINTOldRenderCenterY != gsRenderCenterY ||
			gusINTOldMousePosX	!= gusMouseXPos	|| gusINTOldMousePosY	!= gusMouseYPos	)
	{
		fOK = TRUE;
	}

	// Set old values
	gsINTOldRenderCenterX = gsRenderCenterX;
	gsINTOldRenderCenterY = gsRenderCenterY;

	gusINTOldMousePosX		= gusMouseXPos;
	gusINTOldMousePosY		= gusMouseYPos;

	return( fOK );
}


void CycleIntTileFindStack( INT32 usMapPos )
{
	gfCycleIntTile = TRUE;

	// Cycle around!
	gCurIntTileStack.bCur++;

	//PLot new movement
	gfPlotNewMovement = TRUE;

	if ( gCurIntTileStack.bCur == gCurIntTileStack.bNum )
	{
		gCurIntTileStack.bCur = 0;
	}
}
