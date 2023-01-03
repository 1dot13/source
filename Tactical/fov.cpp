#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Isometric Utils.h"
	#include "worlddef.h"
	#include "render fun.h"
	#include "renderworld.h"
	#include "dialogue control.h"
	#include "structure wrap.h"
	#include "font control.h"
	#include "pathai.h"
	#include "overhead.h"
	#include "smell.h"
	#include "fov.h"
	#include "rotting corpses.h"
	#include "keys.h"
	#include "english.h"
	#include "Random.h"
	#include "input.h"
	#include "exit grids.h"
	#include "environment.h"
	#include "worldman.h"
	#include "Fog Of War.h"
	#include "Strategicmap.h"
	#include "boxing.h"
	#include "opplist.h"
	#include "lighting.h"
	#include "Soldier macros.h"
#endif

#include "connect.h"
#include "GameSettings.h"

/* view directions */
#define DLEFT			0
#define DRIGHT			1
#define UP				2
#define LEFT			3
#define RIGHT			4
#define NOVIEW			5
#define MAXVIEWPATHS		17
#define VIEWPATHLENGTH	13


extern INT16 DirIncrementer[8];

//UINT8		gubGridNoMarkers[ WORLD_MAX ];
UINT8	*	gubGridNoMarkers = NULL;
UINT8		gubGridNoValue			= 254;


#ifdef _DEBUG
//UINT8		gubFOVDebugInfoInfo[ WORLD_MAX ];
UINT8	*	gubFOVDebugInfoInfo = NULL;
#endif


UINT8	ViewPath[MAXVIEWPATHS][VIEWPATHLENGTH] ={
			{NOVIEW,UP,UP,UP,UP,UP,UP,UP,UP,													UP, UP, UP, UP},
			{UP,UP,UP,UP,DRIGHT,UP,UP,UP,UP,													UP, UP, UP, UP},
			{UP,UP,UP,UP,DLEFT,UP,UP,UP,UP,														UP, UP, UP, UP},

			{UP,UP,DLEFT,UP,DLEFT,UP,UP,UP,UP,												UP, UP, UP, UP},
			{UP,UP,DRIGHT,UP,DRIGHT,UP,UP,UP,UP,											UP, UP, UP, UP},

			{UP,UP,DRIGHT,DRIGHT,DRIGHT,UP,UP,UP,UP,									UP, UP, UP, UP},
			{UP,UP,DLEFT,DLEFT,DLEFT,UP,UP,UP,UP,											UP, UP, UP, UP},

			{UP,RIGHT,UP,DRIGHT,DRIGHT,DRIGHT,UP,UP,UP,								UP, UP, UP, UP},
			{UP,LEFT,UP,DLEFT,DLEFT,DLEFT,UP,UP,UP,										UP, UP, UP, UP},

			{DLEFT,DLEFT,DLEFT,DLEFT,DLEFT,UP,UP,UP,UP,								UP, UP, UP, UP},
			{DRIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,UP,UP,UP,UP,					UP, UP, UP, UP},

			{RIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,UP,UP,UP,				UP, UP, UP, UP},
			{LEFT,DLEFT,DLEFT,DLEFT,DLEFT,DLEFT,UP,UP,UP,							UP, UP, UP, UP},

			{DLEFT,LEFT,LEFT,UP,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,	NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{LEFT,LEFT,LEFT,UP,LEFT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,			NOVIEW, NOVIEW, NOVIEW, NOVIEW},

			{DRIGHT,RIGHT,RIGHT,UP,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{RIGHT,RIGHT,RIGHT,UP,RIGHT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,	NOVIEW, NOVIEW, NOVIEW, NOVIEW}
			};


UINT8	ViewPath2[MAXVIEWPATHS][VIEWPATHLENGTH]= {
			{NOVIEW,UP,UP,UP,UP,UP,UP,NOVIEW,NOVIEW,									NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{UP,UP,DLEFT,UP,UP,UP,DLEFT,DRIGHT,NOVIEW,								NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{UP,UP,DLEFT,UP,UP,UP,DRIGHT,DLEFT,NOVIEW,								NOVIEW, NOVIEW, NOVIEW, NOVIEW},

			{UP,UP,DLEFT,UP,UP,DLEFT,DLEFT,UP,NOVIEW,									NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{UP,UP,DRIGHT,UP,UP,DRIGHT,DRIGHT,UP,NOVIEW,							NOVIEW, NOVIEW, NOVIEW, NOVIEW},

			{UP,DLEFT,UP,UP,DLEFT,DLEFT,DLEFT,UP,UP,									UP, UP, UP, UP},
			{UP,DRIGHT,UP,UP,DRIGHT,DRIGHT,DRIGHT,UP,UP,							UP, UP, UP, UP},

			{DLEFT,DLEFT,UP,UP,DLEFT,DLEFT,DLEFT,UP,UP,								UP, UP, UP, UP},
			{DRIGHT,DRIGHT,UP,UP,DRIGHT,DRIGHT,DRIGHT,UP,UP,					UP, UP, UP, UP},

			{DLEFT,DLEFT,UP,DLEFT,DLEFT,DLEFT,DLEFT,UP,UP,						UP, UP, UP, UP},
			{DRIGHT,DRIGHT,UP,DRIGHT,DRIGHT,DRIGHT,DRIGHT,UP,UP,			UP, UP, UP, UP},

			{DLEFT,DLEFT,DLEFT,DLEFT,DLEFT,DLEFT,DLEFT,UP,UP,					UP, UP, UP, UP},
			{DRIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,DRIGHT,UP,UP,	UP, UP, UP, UP},

			{DLEFT,LEFT,DLEFT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,				NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{DRIGHT,RIGHT,DRIGHT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,			NOVIEW, NOVIEW, NOVIEW, NOVIEW},

			{LEFT,LEFT,DLEFT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,					NOVIEW, NOVIEW, NOVIEW, NOVIEW},
			{RIGHT,RIGHT,DRIGHT,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,NOVIEW,			NOVIEW, NOVIEW, NOVIEW, NOVIEW}

			};





void BuildSightDir(UINT32 dir, UINT32 *One, UINT32 *Two, UINT32 *Three, UINT32 *Four, UINT32 *Five)
{
 switch(dir)
 {
	case NORTH	: *One	= NORTHWEST;
		*Two	= NORTHEAST;
		*Three = NORTH;
		*Four	= WEST;
		*Five	= EAST;
		break;
	case NORTHEAST: *One = NORTH;
		*Two = EAST;
		*Three = NORTHEAST;
		*Four	= NORTHWEST;
		*Five	= SOUTHEAST;
		break;
	case EAST	: *One	= NORTHEAST;
		*Two = SOUTHEAST;
		*Three = EAST;
		*Four = NORTH;
		*Five = SOUTH;
		break;
	case SOUTHEAST: *One = EAST;
		*Two = SOUTH;
		*Three = SOUTHEAST;
		*Four = NORTHEAST;
		*Five = SOUTHWEST;
		break;
	case SOUTH	: *One = SOUTHEAST;
		*Two = SOUTHWEST;
		*Three = SOUTH;
		*Four = EAST;
		*Five = WEST;
		break;
	case SOUTHWEST: *One = SOUTH;
		*Two = WEST;
		*Three = SOUTHWEST;
		*Four = SOUTHEAST;
		*Five = NORTHWEST;
		break;
	case WEST	: *One = SOUTHWEST;
		*Two = NORTHWEST;
		*Three = WEST;
		*Four = SOUTH;
		*Five = NORTH;
		break;
	case NORTHWEST: *One = WEST;
		*Two = NORTH;
		*Three = NORTHWEST;
		*Four = SOUTHWEST;
		*Five = NORTHEAST;
		break;
#ifdef BETAVERSION
	default:
	NumMessage("BuildSightDir:	Invalid 'dir' value, = ",dir);
#endif
 }
}

//#if 0


#define		NUM_SLANT_ROOF_SLOTS					200

typedef struct
{
	INT32 sGridNo;
	BOOLEAN		fAllocated;

} SLANT_ROOF_FOV_TYPE;


SLANT_ROOF_FOV_TYPE		gSlantRoofData[ NUM_SLANT_ROOF_SLOTS ];
UINT32								guiNumSlantRoofs = 0;

INT32 GetFreeSlantRoof( void )
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumSlantRoofs; uiCount++)
	{
		if(( gSlantRoofData[uiCount].fAllocated==FALSE ) )
			return( (INT32)uiCount );
	}

	if( guiNumSlantRoofs < NUM_SLANT_ROOF_SLOTS )
		return( (INT32) guiNumSlantRoofs++ );

	return( -1 );
}

void RecountSlantRoofs( void )
{
	INT32 uiCount;

	for(uiCount=guiNumSlantRoofs-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gSlantRoofData[uiCount].fAllocated ) )
		{
			guiNumSlantRoofs=(UINT32)(uiCount+1);
			break;
		}
	}
}

void ClearSlantRoofs( void )
{
	UINT32 uiCount;

	for( uiCount = 0; uiCount < guiNumSlantRoofs; uiCount++ )
	{
		if( ( gSlantRoofData[uiCount].fAllocated ) )
		{
			gSlantRoofData[uiCount].fAllocated = FALSE;
		}
	}

	guiNumSlantRoofs = 0;
}

BOOLEAN FindSlantRoofSlot( INT32 sGridNo )
{
	UINT32 uiCount;

	for( uiCount = 0; uiCount < guiNumSlantRoofs; uiCount++ )
	{
		if( ( gSlantRoofData[uiCount].fAllocated ) )
		{
			if ( gSlantRoofData[uiCount].sGridNo == sGridNo )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

void AddSlantRoofFOVSlot( INT32 sGridNo )
{
	INT32									iSlantRoofSlot;
	SLANT_ROOF_FOV_TYPE		*pSlantRoof;

	// Check if this is a duplicate!
	if ( FindSlantRoofSlot( sGridNo ) )
	{
		return;
	}

	iSlantRoofSlot = GetFreeSlantRoof( );

	if ( iSlantRoofSlot != -1 )
	{
		pSlantRoof = &gSlantRoofData[ iSlantRoofSlot ];
		pSlantRoof->sGridNo = sGridNo;
		pSlantRoof->fAllocated = TRUE;
	}
}

void ExamineSlantRoofFOVSlots( )
{
	UINT32 uiCount;

	for( uiCount = 0; uiCount < guiNumSlantRoofs; uiCount++ )
	{
		if( ( gSlantRoofData[uiCount].fAllocated ) )
		{
			ExamineGridNoForSlantRoofExtraGraphic( gSlantRoofData[uiCount].sGridNo );
		}
	}

	ClearSlantRoofs( );

}

void RevealRoofsAndItems(SOLDIERTYPE *pSoldier, UINT32 itemsToo, BOOLEAN fShowLocators, UINT8 ubLevel, BOOLEAN fForce )
{
	INT32		maincnt,markercnt,marker,tilesLeftToSee,prevmarker;
	UINT8		cnt;
	INT32		Inc[6],Dir[6];
	INT8		itemVisible = FALSE;
	INT8		Blocking,twoMoreTiles,markerDir;
	INT8		nextDir=0;
	UINT8		who; //,itemIndex; // for each square checked
	UINT8		dir,range,Path2;
	//DBrot: More Rooms
	//UINT8		ubRoomNo;
	UINT16		usRoomNo;
	BOOLEAN		fCheckForRooms = FALSE;
	ITEM_POOL	*pItemPool;
	BOOLEAN		fHiddenStructVisible;
	UINT8		ubMovementCost;
	BOOLEAN		fTravelCostObs;
	BOOLEAN		fGoneThroughDoor = FALSE;
	BOOLEAN		fThroughWindow = FALSE;
	BOOLEAN		fItemsQuoteSaid = FALSE;
	UINT16		usIndex;
	BOOLEAN		fRevealItems = TRUE;
	BOOLEAN		fStopRevealingItemsAfterThisTile = FALSE;
	INT8		bTallestStructureHeight;
	INT32		iDoorGridNo;
	STRUCTURE   *pStructure, *pDummy;
	INT8		bStructHeight;
	INT8		bThroughWindowDirection;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ENEMY )
	{
		//pSoldier->needToLookForItems = FALSE;
		return;
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	// Return if this guy has no gridno, has bad life, etc	
	if(TileIsOutOfBounds(pSoldier->sGridNo) || !pSoldier->bInSector || pSoldier->stats.bLife < OKLIFE )
	{
		return;
	}

	if (pSoldier->bBlindedCounter > 0)
	{
		return;
	}


	gubGridNoValue++;

	if ( gubGridNoValue == 255 )
	{
		// Reset!
		Assert(gubGridNoMarkers);
        memset(gubGridNoMarkers, 0, sizeof(UINT8)*WORLD_MAX);
		gubGridNoValue = 1;
	}


	// OK, look for doors
	MercLooksForDoors( pSoldier, TRUE );


	who = pSoldier->ubID;
	dir = pSoldier->ubDirection;

	//NumMessage("good old reveal",dir);

	// a gassed merc can only see 1 tile away due to blurred vision
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
	{
		range = 1;
	}
	else
	{
		range = pSoldier->bViewRange;

		// Flugente: adjust sightrange
		range = (UINT8)( (range * (100 + pSoldier->GetSightRangeBonus()) ) / 100);

		// balance item viewing range between normal and the limit set by opplist-type functions -- CJC
		range = (AdjustMaxSightRangeForEnvEffects( pSoldier, LightTrueLevel( pSoldier->sGridNo, pSoldier->pathing.bLevel), range ) + range) / 2;
	}


	BuildSightDir(dir,(UINT32 *)&Dir[0],(UINT32 *)&Dir[1],(UINT32 *)&Dir[2],(UINT32 *)&Dir[3],(UINT32 *)&Dir[4]);

	for (cnt = 0; cnt < 5; cnt++)
		Inc[cnt] = DirectionInc( (UINT8) Dir[cnt]);

	// create gridno increment for NOVIEW - in other words, no increment!
	Inc[5] = 0;
	Dir[5] = pSoldier->ubDirection;

	if (dir % 2 == 1)	/* even numbers use ViewPath2 */
		Path2 = TRUE;
	else
		Path2 = FALSE;


	// ATE: if in this special cercumstance... our guys are moving on their own...
	// Stop sighting items
	// IN the future, we may want to do something else here...

	if ( gTacticalStatus.uiFlags & OUR_MERCS_AUTO_MOVE )
	{
		itemsToo = FALSE;
	}

	for (maincnt = 0; maincnt < MAXVIEWPATHS; maincnt++)
	{
		marker = pSoldier->sGridNo;
		Blocking = FALSE;
		twoMoreTiles = FALSE;
		tilesLeftToSee = 99;
		fRevealItems = TRUE;
		fStopRevealingItemsAfterThisTile = FALSE;

#ifdef _DEBUG
		if ( _KeyDown( NUM_LOCK ) )
		{
			memset( gubFOVDebugInfoInfo, 0, sizeof( gubFOVDebugInfoInfo ) );

			SetRenderFlags( RENDER_FLAG_FULL );
			RenderWorld( );
		}
#endif

		for (markercnt = 0; markercnt < range; markercnt++)
		{
			//fGoneThroughDoor = FALSE;
			//fThroughWindow		= FALSE;


			prevmarker = marker;

			nextDir = 99;
			fCheckForRooms = FALSE;
			fTravelCostObs = FALSE;
			if ( fStopRevealingItemsAfterThisTile )
			{
				fRevealItems = FALSE;
				fStopRevealingItemsAfterThisTile = FALSE;
			}


			if (Path2)
			{
				markerDir = ViewPath2[maincnt][markercnt];
				if (markercnt < 12)
					nextDir = ViewPath2[maincnt][markercnt+1];
			}
			else
			{
				markerDir = ViewPath[maincnt][markercnt];
				if (markercnt < 12)
					nextDir = ViewPath[maincnt][markercnt+1];
			}

			// OK, check flags for going through door/window last tile
			if ( fThroughWindow == 1 )
			{
				// ATE: Make sure we are going through the same direction!
				// THis is to solve the drassen SAM problem with seeing through walls
				if ( Dir[markerDir] == bThroughWindowDirection)
				{
					 fThroughWindow = 2;
				}
				else
				{
					 fThroughWindow = 0;
				}
			}
			else if ( fThroughWindow == 2 )
			{
					// We've overstayed our welcome - remove!
				fThroughWindow = 0;
			}


			if ( fGoneThroughDoor == 1 )
			{
				fGoneThroughDoor = 2;
			}
			else if ( fGoneThroughDoor == 2 )
			{
				// We've overstayed our welcome - remove!
				fGoneThroughDoor = 0;
			}

			//ATE CHECK FOR NOVIEW!
			if ( nextDir == NOVIEW )
			{
				nextDir = 99;
			}

			marker = NewGridNo(marker,(INT16)Inc[markerDir]);

			if ( marker == 12426 )
			{
				int i = 0;
			}

			// End if this is a no view...
			if ( markerDir == NOVIEW && markercnt != 0 )
			{
				break;
			}

#ifdef _DEBUG
			if ( _KeyDown( NUM_LOCK ) )
			{
				int cnt = GetJA2Clock( );

				gubFOVDebugInfoInfo[ marker ] = (UINT8)markercnt;

				StartFrameBufferRender();

				RenderFOVDebug( );

				SetFont( LARGEFONT1 );
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_MCOLOR_WHITE );
				mprintf( 10,	10 , L"%d", maincnt	);
				//mprintf( 10,	20 , L"%d", marker	);
				//mprintf( 50,	20 , L"%d", pSoldier->sGridNo	);

				InvalidateScreen( );
				EndFrameBufferRender();
				RefreshScreen( NULL );

				do
				{

				} while( ( GetJA2Clock( ) - cnt ) < 250 );

			}
#endif

			// Check if we can get to this gridno from our direction in
			ubMovementCost = gubWorldMovementCosts[ marker ][ Dir[ markerDir ] ][ ubLevel ];

			// ATE: Added: If our current sector is below ground, ignore any blocks!
			if ( gfCaves && ubMovementCost != TRAVELCOST_CAVEWALL )
			{
				ubMovementCost = TRAVELCOST_FLAT;
			}

			if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
			{
				ubMovementCost = DoorTravelCost( pSoldier, marker, ubMovementCost, (BOOLEAN) (pSoldier->bTeam == gbPlayerNum), &iDoorGridNo );
				pStructure = FindStructure( iDoorGridNo, STRUCTURE_ANYDOOR );
				if ( pStructure != NULL && pStructure->fFlags & STRUCTURE_TRANSPARENT)
				{
					// cell door or somehow otherwise transparent; allow merc to see through
					ubMovementCost = TRAVELCOST_FLAT;
				}
			}

			// If we have hit an obstacle, STOP HERE
			if ( ubMovementCost >= TRAVELCOST_BLOCKED )
			{
				// We have an obstacle here...

				// If it is bigger than a breadbox... err... taller than a man...
				// Then stop path altogether
				// otherwise just stop revealing items

				// CJC:	only do this when the direction is horizontal; easier and faster to check
				// and the effect should still be good enough

				if ( ubMovementCost == TRAVELCOST_WALL || ubMovementCost == TRAVELCOST_DOOR || ubMovementCost == TRAVELCOST_EXITGRID )
				{
					fTravelCostObs = TRUE;
					fRevealItems = FALSE;
				}
				else
				{
					// walls are handled above, so the blocking object is guaranteed not to be a wall
					bTallestStructureHeight = GetTallestStructureHeight( marker, FALSE );
					if (bTallestStructureHeight >= 3)
					{
						fTravelCostObs = TRUE;
						fStopRevealingItemsAfterThisTile = TRUE;
					}
					else if ( bTallestStructureHeight != 0 )
					{
						// stop revealing items after this tile but keep going
						fStopRevealingItemsAfterThisTile = TRUE;
					}

				}


				if ( (Dir[markerDir] % 2) == 1 )
				{
					// diagonal
					fTravelCostObs = TRUE;
					// cheap hack... don't reveal items
					fRevealItems = FALSE;
				}
				else
				{

					bTallestStructureHeight = GetTallestStructureHeight( marker, FALSE );
					if (bTallestStructureHeight >= 3)
					{
						fTravelCostObs = TRUE;
						fStopRevealingItemsAfterThisTile = TRUE;
					}
					else if ( bTallestStructureHeight != 0 )
					{
						// stop revealing items after this tile but keep going
						fStopRevealingItemsAfterThisTile = TRUE;
					}
				}

			}

			// Check if it's been done already!
			if ( gubGridNoMarkers[ marker ] != gubGridNoValue )
			{

				// Mark gridno
				gubGridNoMarkers[ marker ] = gubGridNoValue;

				// check and see if the gridno changed
				// if the gridno is the same, avoid redundancy and break
				if (marker==prevmarker && markercnt != 0 )
				{

				}
				else	// it changed
				{

					// Skip others if we have gone through a door but are too far away....
					if ( fGoneThroughDoor )
					{
						if (markercnt > 5 )	// Are we near the door?
						{
							break;
						}
					}
					// DO MINE FINDING STUFF
					// GET INDEX FOR ITEM HERE
					// if there IS a direction after this one, nextdir WILL NOT be 99
					if (nextDir != 99)
					{
						Blocking = GetBlockingStructureInfo( marker, (INT8)Dir[ markerDir ], (INT8)Dir[ nextDir ], ubLevel, &bStructHeight, &pDummy, FALSE );
					}
					else // no "next" direction, so pass in a NOWHERE so that
					// "SpecialViewObstruction" will know not to take it UINT32o consideration
					{
						Blocking = GetBlockingStructureInfo( marker, (INT8)Dir[markerDir], (INT8)30, ubLevel, &bStructHeight, &pDummy, FALSE  );
					}

					if ( gfCaves )
					{
						Blocking = NOTHING_BLOCKING;
					}

					// CHECK FOR ROOMS
					if ( Blocking == BLOCKING_TOPLEFT_WINDOW || Blocking == BLOCKING_TOPLEFT_OPEN_WINDOW )
					{
						// CHECK FACING DIRECTION!
						if ( Dir[markerDir] == NORTH || Dir[markerDir] == SOUTH )
						{
							if (markercnt <= 1 )	// Are we right beside it?
							{
								fThroughWindow = TRUE;
								bThroughWindowDirection = ( INT8 ) Dir[ markerDir ];
							}
						}
					}
					if ( Blocking == BLOCKING_TOPRIGHT_WINDOW || Blocking == BLOCKING_TOPRIGHT_OPEN_WINDOW )
					{
						// CHECK FACING DIRECTION!
						if ( Dir[markerDir] == EAST || Dir[markerDir] == WEST )
						{
							if (markercnt <= 1 )	// Are we right beside it?
							{
								fThroughWindow = TRUE;
								bThroughWindowDirection = ( INT8 ) Dir[ markerDir ];
							}
						}
					}

					if ( Blocking == BLOCKING_TOPLEFT_DOOR )
					{
						fGoneThroughDoor = TRUE;
					}
					if ( Blocking == BLOCKING_TOPRIGHT_DOOR )
					{
						fGoneThroughDoor = TRUE;
					}

					// ATE: If we hit this tile, find item always!
					//if (Blocking < FULL_BLOCKING )
					{

						// Handle special things for our mercs, like uncovering roofs
						// and revealing objects...
						//gpSoldier->shad |= SEENBIT;

						//itemVisible = ObjList[itemIndex].visible;

						// NOTE: don't allow object viewing if gassed XXX

						if (itemsToo && fRevealItems ) // && itemIndex < MAXOBJECTLIST)
						{
							// OK, look for corpses...
							LookForAndMayCommentOnSeeingCorpse( pSoldier, marker, ubLevel );

							if ( GetItemPool( marker, &pItemPool, ubLevel ) )
							{
								itemVisible = pItemPool->bVisible;

								if ( SetItemPoolVisibilityOn( pItemPool, INVISIBLE, fShowLocators ) )
								{
									SetRenderFlags(RENDER_FLAG_FULL);

									// WANNE: Should we pause when item was found in tactical?
									bool enableItemSpottingAction = true;

									if ( !is_networked && gGameExternalOptions.fItemSpottedNoTalk && gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT)
										enableItemSpottingAction = false;

									if (enableItemSpottingAction)
									{
										if ( fShowLocators )
										{
											// Set makred render flags
											//gpWorldLevelData[marker].uiFlags|=MAPELEMENT_REDRAW;
											//gpWorldLevelData[gusCurMousePos].pTopmostHead->uiFlags |= LEVELNODE_DYNAMIC;

											//SetRenderFlags(RENDER_FLAG_MARKED);
											SetRenderFlags(RENDER_FLAG_FULL);

											// Hault soldier
											// ATE: Only if in combat...
											if ( gTacticalStatus.uiFlags & INCOMBAT )
											{
												pSoldier->HaultSoldierFromSighting( FALSE );
											}
											else
											{
												// ATE: Make sure we show locators...
												gTacticalStatus.fLockItemLocators = FALSE;
											}

											if ( !fItemsQuoteSaid && gTacticalStatus.fLockItemLocators == FALSE )
											{
												gTacticalStatus.fLockItemLocators = TRUE;

												if ( gTacticalStatus.ubAttackBusyCount > 0 && ( gTacticalStatus.uiFlags & INCOMBAT ) )
												{
													gTacticalStatus.fItemsSeenOnAttack = TRUE;
													gTacticalStatus.ubItemsSeenOnAttackSoldier = pSoldier->ubID;
													gTacticalStatus.usItemsSeenOnAttackGridNo  = marker;
												}
												else
												{

													// Display quote!
													if ( !AM_AN_EPC( pSoldier ) )
													{

														TacticalCharacterDialogueWithSpecialEvent( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ), DIALOGUE_SPECIAL_EVENT_SIGNAL_ITEM_LOCATOR_START, marker, 0 );

													}
													else
													{
														 // Turn off item lock for locators...
														 gTacticalStatus.fLockItemLocators = FALSE;
														 // Slide to location!
														SlideToLocation( 0, marker );
													}
												}

												fItemsQuoteSaid = TRUE;
											}
										}

									}
								}
							}
						}

						// if blood here, let the user see it now...
						//if (ExtGrid[marker].patrolInfo < MAXBLOOD)
						//		gpSoldier->blood = ExtGrid[marker].patrolInfo;

						//DoRoofs(marker,gpSoldier);

						tilesLeftToSee--;
					}

					// CHECK FOR HIDDEN STRUCTS
					// IF we had a hidden struct here that is not visible ( which will still be true because
					// we set it revealed below...
					if ( DoesGridNoContainHiddenStruct( marker, &fHiddenStructVisible ) )
					{
						if ( !fHiddenStructVisible )
						{
							gpWorldLevelData[marker].uiFlags|=MAPELEMENT_REDRAW;
							SetRenderFlags(RENDER_FLAG_MARKED);
							RecompileLocalMovementCosts( marker );
						}
					}

					if (tilesLeftToSee <= 0)
						break;

					if ( Blocking == FULL_BLOCKING || ( fTravelCostObs && !fThroughWindow ) )
					{
						break;
					}

					//if ( Blocking == NOTHING_BLOCKING || Blocking == BLOCKING_NEXT_TILE )
					if ( Blocking == NOTHING_BLOCKING )
					{
						fCheckForRooms = TRUE;
					}

					if ( ubLevel != 0 )
					{
						fCheckForRooms = FALSE;
					}

					// CHECK FOR SLANT ROOF!
					{
						STRUCTURE	*pStructure, *pBase;

						pStructure = FindStructure( marker, STRUCTURE_SLANTED_ROOF );

						if ( pStructure != NULL )
						{
							pBase = FindBaseStructure( pStructure );

							// ADD TO SLANTED ROOF LIST!
							AddSlantRoofFOVSlot( marker );
						}
					}

					// Set gridno as revealed
					if ( ubLevel == FIRST_LEVEL )
					{
						if ( gfBasement || gfCaves )
						{
							// OK, if we are underground, we don't want to reveal stuff if
							// 1 ) there is a roof over us and
							// 2 ) we are not in a room
							if ( gusWorldRoomInfo[ marker ] == NO_ROOM && TypeRangeExistsInRoofLayer( marker, FIRSTROOF, FOURTHROOF, &usIndex ) )
							{
								int i = 0;
							}
							else
							{
								gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED;
								if( gfCaves )
								{
									RemoveFogFromGridNo( marker );
								}
							}
						}
						else
						{
							gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED;
						}

						// CHECK FOR ROOMS
						//if ( fCheckForRooms )
						{
							if ( InAHiddenRoom( marker, &usRoomNo ) )
							{
								RemoveRoomRoof( marker, usRoomNo, pSoldier );
								if ( usRoomNo == ROOM_SURROUNDING_BOXING_RING && gWorldSectorX == BOXING_SECTOR_X && gWorldSectorY == BOXING_SECTOR_Y && gbWorldSectorZ == BOXING_SECTOR_Z )
								{
									// reveal boxing ring at same time
									RemoveRoomRoof( marker, BOXING_RING, pSoldier );
								}
							}
						}
					}
					else
					{
						gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED_ROOF;
					}

					// Check for blood....
					UpdateBloodGraphics( marker, ubLevel );

					if ( Blocking != NOTHING_BLOCKING && Blocking != BLOCKING_TOPLEFT_DOOR && Blocking != BLOCKING_TOPRIGHT_DOOR && Blocking != BLOCKING_TOPLEFT_WINDOW && Blocking != BLOCKING_TOPRIGHT_WINDOW && Blocking != BLOCKING_TOPRIGHT_OPEN_WINDOW && Blocking != BLOCKING_TOPLEFT_OPEN_WINDOW)
					{
						break;
					}

					//gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_SHADELAND;
				}
			} // End of duplicate check
			else
			{
				if ( fTravelCostObs )
				{
					break;
				}
			}

		} // end of one path

	} // end of path loop

	// Loop through all availible slant roofs we collected and perform cool stuff on them
	ExamineSlantRoofFOVSlots( );

	//pSoldier->needToLookForItems = FALSE;

	//LookForDoors(pSoldier,UNAWARE);
}

//#endif
