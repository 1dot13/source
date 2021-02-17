#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
#else

	#include "Militia Control.h"
	#include "Town Militia.h"
	#include "Soldier Init List.h"
	#include "Campaign Types.h"
	#include "Overhead.h"
	#include "strategicmap.h"
	#include "PreBattle Interface.h"
	#include "Map Screen Interface.h"
	#include "Queen Command.h"
	#include "strategic.h"
	#include "popupbox.h"
	#include "interface control.h"
	#include "Map Screen Interface Map.h"
	#include "renderworld.h"
	#include "Text.h"
	#include "utilities.h"
	#include "Interface.h"
	#include "GameSettings.h"
	#include "Input.h"
	#include "opplist.h"
	#include "los.h"
	#include "Items.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "Campaign.h"
	#include "ai.h"
	#include "Isometric Utils.h"
	#include "MilitiaSquads.h"
	#include "MilitiaIndividual.h"	// added by Flugente
	#include "CampaignStats.h"		// added by Flugente
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


BOOLEAN gfStrategicMilitiaChangesMade = FALSE;

BOOLEAN fFirstClickInMilitiaControlScreenMask = FALSE;
BOOLEAN gfMSResetMilitia = FALSE;


// render pre battle interface?
extern BOOLEAN gfRenderPBInterface;
extern BOOLEAN fMapScreenBottomDirty;


// PopUp Box Handles
INT32 ghMilitiaControlBox = -1;
//INT32 ghTalkToAllBox = -1;


// the x,y position of assignment pop up in tactical
INT16 gsMilitiaControlBoxesX, gsMilitiaControlBoxesY;
//INT16 gsTalkToAllBoxesX, gsTalkToAllBoxesY;

// assignment menu mouse regions
MOUSE_REGION    gMilitiaControlMenuRegion[ MAX_MILCON_STRING_COUNT ]; 

MOUSE_REGION    gMilitiaControlScreenMaskRegion;


// pop up menu mouse regions
void CreateDestroyMouseRegionsForMilitiaControlMenu( void );
void MilitiaControlMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void MilitiaControlMenuBtnCallBack( MOUSE_REGION * pRegion, INT32 iReason );

//void CreateDestroyMouseRegionsForTalkToAllMenu( void );
//void TalkToAllMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
//void TalkToAllMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );


// assignment screen mask
void MilitiaControlScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );
void CreateDestroyScreenMaskForMilitiaControlMenus( void );


// misc assignment GUI functions
void HandleShadingOfLinesForMilitiaControlMenu( void );
//void HandleShadingOfLinesForTalkToAllMenu( void );

void PositionCursorForMilitiaControlBox( void );

void RepositionMouseRegionsForMilitiaControl( void );

void CheckAndUpdateTacticalMilitiaControlPopUpPositions( void );


// create assignments box
void CreateMilitiaControlBox( void );
//void CreateTalkToAllBox( void );


// rebuild assignments box
void RebuildMilitiaControlBox( void );


//funktions for soldier control
extern INT8 SearchForItems( SOLDIERTYPE * pSoldier, INT8 bReason, UINT16 usItem );
extern SOLDIERTYPE *GetSelectedAssignSoldier( BOOLEAN fNullOK );
extern SOLDIERTYPE *pTMilitiaSoldier;
extern BOOLEAN SoldierCanAffordNewStance( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );


void ResetMilitia()
{
	UINT8 ubNumGreen = 0;
	UINT8 ubNumReg = 0;
	UINT8 ubNumVet = 0;

//	if ( gWorldSectorX !=0 && gWorldSectorY != 0 && NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) )
//		fBattleInProgress = TRUE;

	// 0verhaul:  Instead of relying on the "changes made" flag, which isn't even saved in a saved game and therefore not
	// reliable, we'll just do this the hard way, by taking inventory.
//	gfStrategicMilitiaChangesMade = FALSE;
//	for (UINT32 cnt = gTacticalStatus.Team[MILITIA_TEAM].bFirstID; cnt <= gTacticalStatus.Team[MILITIA_TEAM].bLastID; cnt++)
//	{
//		if (!MercPtrs[cnt]->bActive)
//		{
//			continue;
//		}
//
//		switch (MercPtrs[cnt]->ubSoldierClass)
//		{
//		case SOLDIER_CLASS_GREEN_MILITIA: ubNumGreen++; break;
//		case SOLDIER_CLASS_REG_MILITIA: ubNumReg++; break;
//		case SOLDIER_CLASS_ELITE_MILITIA: ubNumVet++; break;
//		default: ;
//		}
//	}
//	if (MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, GREEN_MILITIA) != ubNumGreen ||
//		MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, REGULAR_MILITIA) != ubNumReg ||
//		MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, ELITE_MILITIA) != ubNumVet)
//	{
//		gfStrategicMilitiaChangesMade = TRUE;
//	}
//
	if (gfStrategicMilitiaChangesMade)
	{
		// I truly hope that we remove such inane control methods from the soldier create code when we break the merc slot barrier
		// Hacks like this really depress me.
		UINT32 cs = guiCurrentScreen;
		// Make sure we aren't on the AUTORESOLVE screen for this.  Even if we are.  We are removing and creating soldiers for 
		// tactical here, not autoresolve.  In my opinion the CreateSoldierXXX and TacticalRemoveSoldierXXX functions should take 
		// a flag for autoresolve if different initialization or destruction is desired.
		guiCurrentScreen = GAME_SCREEN;

		// Flugente: cause all militia whose equipment is from this sector to drop it
		TeamDropAll( MILITIA_TEAM );

		RemoveMilitiaFromTactical();
		ubNumGreen = MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, GREEN_MILITIA);
		ubNumReg = MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, REGULAR_MILITIA);
		ubNumVet = MilitiaInSectorOfRank(gWorldSectorX, gWorldSectorY, ELITE_MILITIA);

		AddSoldierInitListMilitia( ubNumGreen, ubNumReg, ubNumVet );

		// Now restore the original screen setting so the game doesn't go wacky.
		guiCurrentScreen = cs;

		gfStrategicMilitiaChangesMade = FALSE;
	}

//	if( ( gfStrategicMilitiaChangesMade && !fBattleInProgress ) || gTacticalStatus.uiFlags & LOADING_SAVED_GAME || gfMSResetMilitia )
//	if(gfMSResetMilitia )
//	{
//		if( !gfMSResetMilitia )
//			RemoveMilitiaFromTactical();
//		PrepareMilitiaForTactical();
//		gfMSResetMilitia = FALSE;
//	}
}

void RemoveMilitiaFromTactical()
{
	SOLDIERINITNODE *curr;
	INT32 i;
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive )
		{
			TacticalRemoveSoldier( MercPtrs[ i ]->ubID );
		}
	}
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->bTeam == MILITIA_TEAM )
		{ 
			curr->pSoldier = NULL;
		}
		curr = curr->next;
	}
}

void PrepareMilitiaForTactical( BOOLEAN fPrepareAll)
{
	SECTORINFO *pSector;
	INT32 x;
	UINT8 ubGreen, ubRegs, ubElites;

	if( gbWorldSectorZ > 0 )
		return;

	// Do we have a loaded sector?
	if ( gWorldSectorX == 0 && gWorldSectorY == 0 )
		return;
	
	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	// Prevent militia from just waiting on the border
	gTacticalStatus.Team[MILITIA_TEAM].bAwareOfOpposition = ( pSector->uiFlags & SF_PLAYER_KNOWS_ENEMIES_ARE_HERE ) != 0;

	ubGreen  = MilitiaInSectorOfRankStationary( gWorldSectorX, gWorldSectorY, GREEN_MILITIA );
	ubRegs	 = MilitiaInSectorOfRankStationary( gWorldSectorX, gWorldSectorY, REGULAR_MILITIA );
	ubElites = MilitiaInSectorOfRankStationary( gWorldSectorX, gWorldSectorY, ELITE_MILITIA );

	// Flugente: if we are entering combat, have militia groups that arrived from other sectors start at the edges.
	// As the arrival time is no longer set, we try to deduct that from the previous sectors of the groups.
	// If that isn't set, too bad, have them insert in the center then.
	if ( fPrepareAll && !guiDirNumber && NumHostilesInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) > 0 )//(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		guiDirNumber = 5;

		ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

		gpAttackDirs[0][0] += ubGreen;
		gpAttackDirs[0][1] += ubRegs;
		gpAttackDirs[0][2] += ubElites;

		GROUP *pGroup = gpGroupList;
		while ( pGroup )
		{
			if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY )
			{
				if ( pGroup->ubPrevX > 0 && pGroup->ubPrevY > 0 )
				{
					if ( pGroup->ubPrevX < gWorldSectorX )
					{
						gpAttackDirs[4][0] += pGroup->pEnemyGroup->ubNumAdmins;
						gpAttackDirs[4][1] += pGroup->pEnemyGroup->ubNumTroops;
						gpAttackDirs[4][2] += pGroup->pEnemyGroup->ubNumElites;
						gpAttackDirs[4][3] = INSERTION_CODE_WEST;
					}
					else if ( pGroup->ubPrevX > gWorldSectorX )
					{
						gpAttackDirs[2][0] += pGroup->pEnemyGroup->ubNumAdmins;
						gpAttackDirs[2][1] += pGroup->pEnemyGroup->ubNumTroops;
						gpAttackDirs[2][2] += pGroup->pEnemyGroup->ubNumElites;
						gpAttackDirs[2][3] = INSERTION_CODE_EAST;
					}
					else if ( pGroup->ubPrevY < gWorldSectorY )
					{
						gpAttackDirs[1][0] += pGroup->pEnemyGroup->ubNumAdmins;
						gpAttackDirs[1][1] += pGroup->pEnemyGroup->ubNumTroops;
						gpAttackDirs[1][2] += pGroup->pEnemyGroup->ubNumElites;
						gpAttackDirs[1][3] = INSERTION_CODE_NORTH;
					}
					else if ( pGroup->ubPrevY > gWorldSectorY )
					{
						gpAttackDirs[3][0] += pGroup->pEnemyGroup->ubNumAdmins;
						gpAttackDirs[3][1] += pGroup->pEnemyGroup->ubNumTroops;
						gpAttackDirs[3][2] += pGroup->pEnemyGroup->ubNumElites;
						gpAttackDirs[3][3] = INSERTION_CODE_SOUTH;
					}
					else
					{
						gpAttackDirs[0][0] += pGroup->pEnemyGroup->ubNumAdmins;
						gpAttackDirs[0][1] += pGroup->pEnemyGroup->ubNumTroops;
						gpAttackDirs[0][2] += pGroup->pEnemyGroup->ubNumElites;
					}
				}
				else
				{
					gpAttackDirs[0][0] += pGroup->pEnemyGroup->ubNumAdmins;
					gpAttackDirs[0][1] += pGroup->pEnemyGroup->ubNumTroops;
					gpAttackDirs[0][2] += pGroup->pEnemyGroup->ubNumElites;
				}
			}
			pGroup = pGroup->next;
		}
	}
	else
	{
		ubGreen  += MilitiaInSectorOfRankInGroups( gWorldSectorX, gWorldSectorY, GREEN_MILITIA );
		ubRegs	 += MilitiaInSectorOfRankInGroups( gWorldSectorX, gWorldSectorY, REGULAR_MILITIA );
		ubElites += MilitiaInSectorOfRankInGroups( gWorldSectorX, gWorldSectorY, ELITE_MILITIA );
	}
	
	if(guiDirNumber)
	{
		if (fPrepareAll)
		{
			AddSoldierInitListMilitia( gpAttackDirs[ 0 ][0], gpAttackDirs[ 0 ][1], gpAttackDirs[ 0 ][2] );
		}

		// If the sector is already loaded, don't add the existing militia
		for( x = 1; x < guiDirNumber ; ++x )
		{
#if 0
			//			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld,%ld,%ld", gpAttackDirs[ x ][ 0 ], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2], gpAttackDirs[ x ][3] );
			if( gfMSResetMilitia )
			{
				if( gpAttackDirs[ x ][ 3 ] != INSERTION_CODE_CENTER )
				{
					AddSoldierInitListMilitiaOnEdge( gpAttackDirs[ x ][ 3 ], gpAttackDirs[ x ][0], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2] );
					ubGreen -= gpAttackDirs[ x ][0];
					ubRegs -= gpAttackDirs[ x ][1];
					ubElites -= gpAttackDirs[ x ][2];
				}
			}
			//else
			//{
#endif
			AddSoldierInitListMilitiaOnEdge( gpAttackDirs[ x ][ 3 ], gpAttackDirs[ x ][0], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2] );
		}

		guiDirNumber = 0;
	}
	else if (fPrepareAll)
	{
		AddSoldierInitListMilitia( ubGreen, ubRegs, ubElites );
	}

//	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
//	{
//		if( MercPtrs[ i ]->bInSector )
//		{
//			MercPtrs[ i ]->bOrders = SEEKENEMY;
////			MercPtrs[ i ]->bAttitude = AGGRESSIVE;
//		}
//	}

	//for (int i=0; i<TOTAL_SOLDIERS; i++)
	{
		//CHRISL: What's this assert for?
		//Assert( !MercPtrs[i]->bActive || !MercPtrs[i]->bInSector || !TileIsOutOfBounds(MercPtrs[i]->sGridNo));
	}
}

void HandleMilitiaPromotions( void )
{
	UINT8				cnt;
	SOLDIERTYPE*		pTeamSoldier;

	gbGreenToElitePromotions = 0;
	gbGreenToRegPromotions = 0;
	gbRegToElitePromotions = 0;
	gbMilitiaPromotions = 0;

	cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; ++cnt, ++pTeamSoldier)
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->stats.bLife > 0 )
		{
			// Flugente: take care of promotions and individual militia update
			HandlePossibleMilitiaPromotion( pTeamSoldier, FALSE );	
		}
	}

	if( gbMilitiaPromotions )
	{
    // ATE: Problems here with bringing up message box...

		// CHAR16 str[ 512 ];
		// BuildMilitiaPromotionsString( str );
		// DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
	}

	if (gfStrategicMilitiaChangesMade)
	{
		ResetMilitia();
	}
}















//lal


void RebuildMilitiaControlBox( void )
{
	// destroy and recreate MilitiaControls box
	if ( ghMilitiaControlBox != -1 )
	{
		RemoveBox( ghMilitiaControlBox );
		ghMilitiaControlBox = -1;
	}

	CreateMilitiaControlBox( );
}


void MilitiaControlMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// mvt callback handler for MilitiaControl region
	INT32 iValue = -1;
	
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// highlight string
		if( GetBoxShadeFlag( ghMilitiaControlBox, iValue ) == FALSE )
		{
			// get the string line handle
			HighLightBoxLine( ghMilitiaControlBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghMilitiaControlBox );
	}
}





void DetermineWhichMilitiaControlMenusCanBeShown( void )
{
	BOOLEAN fCharacterNoLongerValid = FALSE;
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if( fShowMapScreenMovementList == TRUE )
		{
			if( GetSelectedDestChar() == -1 )
			{
				fCharacterNoLongerValid = TRUE;
				//HandleShowingOfMovementBox( ); //laltodo
			}
			else
			{
				fShowMapScreenMovementList = FALSE;
				fCharacterNoLongerValid = TRUE;
			}
		}
		else if( bSelectedAssignChar == -1 )
		{
			fCharacterNoLongerValid = TRUE;
		}

		// update the assignment positions
		UpdateMapScreenMilitiaControlPositions( );
	}
	//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"start DetermineWhichMilitiaControlMenusCanBeShown" );

	// determine which assign menu needs to be shown
	if( ( ( fShowMilitiaControlMenu == FALSE ) )||( fCharacterNoLongerValid == TRUE ) )
	{
		// reset show MilitiaControl menu
		fShowMilitiaControlMenu = FALSE;
		//fShowTalkToAllMenu = FALSE;

		//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"ShowMilitiaControlMenu = FALSE" );

		// destroy mask, if needed
		CreateDestroyScreenMaskForMilitiaControlMenus( );
		
		// destroy menu if needed
		CreateDestroyMouseRegionsForMilitiaControlMenu( );
		//CreateDestroyMouseRegionsForTalkToAllMenu( );

		if ( IsBoxShown( ghMilitiaControlBox ) )
		{
			HideBox( ghMilitiaControlBox );
			fTeamPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}

		//if ( IsBoxShown( ghTalkToAllBox ) )
		//{
		//	HideBox( ghTalkToAllBox );
		//	fTeamPanelDirty = TRUE;
		//	gfRenderPBInterface = TRUE;
		//}		

		// no menus, leave
		return;
	}

	// update the assignment positions
	UpdateMapScreenMilitiaControlPositions( );

	// create mask, if needed
	CreateDestroyScreenMaskForMilitiaControlMenus( );

	// created assignment menu if needed
	CreateDestroyMouseRegionsForMilitiaControlMenu( ); 
	//CreateDestroyMouseRegionsForTalkToAllMenu( ); 

	// Militia Command menu
	if( fShowMilitiaControlMenu == TRUE )
	{
		HandleShadingOfLinesForMilitiaControlMenu( );
		ShowBox( ghMilitiaControlBox );
	}
	else
	{
		if( IsBoxShown( ghMilitiaControlBox ) )
		{
			HideBox( ghMilitiaControlBox );
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;	
		}
	}

	// Talk to all menu
	//if( fShowTalkToAllMenu == TRUE )
	//{
	//	HandleShadingOfLinesForTalkToAllMenu( );
	//	ShowBox( ghTalkToAllBox );
	//}
	//else
	//{
	//	if( IsBoxShown( ghTalkToAllBox ) )
	//	{
	//		HideBox( ghTalkToAllBox );
	//		fTeamPanelDirty = TRUE;
	//		fMapPanelDirty = TRUE;
	//		gfRenderPBInterface = TRUE;	
	//	}
	//}

	return;
}

void CreateDestroyScreenMaskForMilitiaControlMenus( void )
{
	static BOOLEAN fCreated = FALSE;
	// will create a screen mask to catch mouse input to disable MilitiaControl menus

	// not created, create
	if( ( fCreated == FALSE ) && ( fShowMilitiaControlMenu == TRUE ) )
	{
		MSYS_DefineRegion( &gMilitiaControlScreenMaskRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST - 4 ,
			MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MilitiaControlScreenMaskBtnCallback );

		// created
		fCreated = TRUE;

		if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
		{
			MSYS_ChangeRegionCursor(  &gMilitiaControlScreenMaskRegion, 0 );
		}
	}
	else if( ( fCreated == TRUE ) && ( fShowMilitiaControlMenu == FALSE ) )
	{
		// created, get rid of it
		MSYS_RemoveRegion(  &gMilitiaControlScreenMaskRegion );

		// not created
		fCreated = FALSE;
	}

	return;
}


void MilitiaControlScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for MilitiaControl screen mask region
	
	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		if( fFirstClickInMilitiaControlScreenMask == TRUE )
		{
			fFirstClickInMilitiaControlScreenMask = FALSE;
			return;
		}

		// button event, stop showing menus
		fShowMilitiaControlMenu = FALSE;

		// update mapscreen
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;

		gfRenderPBInterface = TRUE;
		SetRenderFlags( RENDER_FLAG_FULL );
	}
	return;
}



void CreateDestroyMouseRegionsForMilitiaControlMenu( void )
{
	static BOOLEAN fCreated = FALSE;
	UINT32 iCounter = 0;
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;
	static BOOLEAN fShowRemoveMenu = FALSE;


	// will create/destroy mouse regions for the map screen assignment main menu
	// check if we can only remove character from team..not assign
	//if( ( bSelectedAssignChar != -1 ) || ( fShowRemoveMenu == TRUE ) )
	//{
	//	if( fShowRemoveMenu == TRUE )
	//	{
	//		// dead guy handle menu stuff
	//		fShowRemoveMenu = fShowMilitiaControlMenu | fShowContractMenu;
	//		
	//		CreateDestroyMouseRegionsForRemoveMenu( );

	//		return;
	//	}
	//	if( ( Menptr[gCharactersList[bSelectedAssignChar].usSolID].bLife == 0 ) || ( Menptr[gCharactersList[bSelectedAssignChar].usSolID].bMilitiaControl == MilitiaControl_POW ) )
	//	{
	//		// dead guy handle menu stuff
	//		fShowRemoveMenu = fShowMilitiaControlMenu | fShowContractMenu;
	//	
	//		CreateDestroyMouseRegionsForRemoveMenu( );

	//		return;
	//	}
	//}


	if( ( fShowMilitiaControlMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		gfIgnoreScrolling = FALSE;

		if( ( fShowMilitiaControlMenu ) && ( guiCurrentScreen == MAP_SCREEN ) ) 
		{
		  SetBoxPosition( ghMilitiaControlBox, MilitiaControlPosition );
		}

		//pSoldier = GetSelectedAssignSoldier( FALSE );

		//if( pSoldier -> ubWhatKindOfMercAmI != MERC_TYPE__EPC ) //laltodo
		{
			// grab height of font
			iFontHeight = GetLineSpace( ghMilitiaControlBox ) + GetFontHeight( GetBoxFont( ghMilitiaControlBox ) ); 

			// get x.y position of box
			GetBoxPosition( ghMilitiaControlBox, &pPosition);

			// grab box x and y position
			iBoxXPosition = pPosition.iX;
			iBoxYPosition = pPosition.iY;

			// get dimensions..mostly for width
			GetBoxSize( ghMilitiaControlBox, &pDimensions );

			// get width
			iBoxWidth = pDimensions.iRight;

			SetCurrentBox( ghMilitiaControlBox );
		}
		

		// define regions
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghMilitiaControlBox ); iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_DefineRegion( &gMilitiaControlMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMilitiaControlBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMilitiaControlBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							 MSYS_NO_CURSOR, MilitiaControlMenuMvtCallBack, MilitiaControlMenuBtnCallBack );

			MSYS_SetRegionUserData( &gMilitiaControlMenuRegion[ iCounter ], 0, iCounter );
		}

		// created
		fCreated = TRUE;

		// unhighlight all strings in box
		UnHighLightBox( ghMilitiaControlBox );
		CheckAndUpdateTacticalMilitiaControlPopUpPositions( );

		//PositionCursorForMilitiaControlBox( );
	}
	else if( ( fShowMilitiaControlMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		// destroy 
		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghMilitiaControlBox ); iCounter++ )
		{
			MSYS_RemoveRegion( &gMilitiaControlMenuRegion[ iCounter ] );
		}

		// not created
		fCreated = FALSE;
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}



void CreateMilitiaControlBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;
 CHAR16 sString[ 128 ];
 //SOLDIERTYPE *pSoldier = NULL;


 // will create attribute pop up menu for mapscreen MilitiaControls

	MilitiaControlPosition.iX = OrigMilitiaControlPosition.iX;

	if( giBoxY != 0 )
	{
		MilitiaControlPosition.iY = giBoxY;
	}

	//pSoldier = GetSelectedAssignSoldier( TRUE );
	// pSoldier NULL is legal here!  Gets called during every mapscreen initialization even when nobody is assign char

	// create basic box
	CreatePopUpBox(&ghMilitiaControlBox, MilitiaControlDimensions, MilitiaControlPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
 
	// which buffer will box render to
	SetBoxBuffer(ghMilitiaControlBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghMilitiaControlBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghMilitiaControlBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghMilitiaControlBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghMilitiaControlBox, 2);

	// set current box to this one
	SetCurrentBox( ghMilitiaControlBox );

	// add strings for box
	for(uiCounter=0; uiCounter < MAX_MILCON_STRING_COUNT; uiCounter++)
	{
		swprintf( sString, pMilitiaControlMenuStrings[uiCounter] );

		AddMonoString(&hStringHandle, sString );
  
		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghMilitiaControlBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghMilitiaControlBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghMilitiaControlBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghMilitiaControlBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghMilitiaControlBox, FONT_GRAY7 );
	SetBoxSecondaryShade( ghMilitiaControlBox, FONT_YELLOW );

	 // resize box to text
	ResizeBoxToText( ghMilitiaControlBox );

	//DetermineMilitiaControlBoxPositions( ); //laltodo
}


BOOLEAN CreateDestroyMilitiaControlPopUpBoxes( void )
{
	static BOOLEAN fCreated= FALSE;
	VSURFACE_DESC		vs_desc;
	VOBJECT_DESC    VObjectDesc; 


	if( ( fShowMilitiaControlMenu == TRUE ) && ( fCreated == FALSE ) )
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\popup.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiPOPUPBORDERS));

		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
		strcpy(vs_desc.ImageFile, "INTERFACE\\popupbackground.pcx");
		CHECKF(AddVideoSurface(&vs_desc, &guiPOPUPTEX));

		// these boxes are always created while in mapscreen...
		CreateMilitiaControlBox( );
		//CreateTalkToAllBox( );

		UpdateMapScreenMilitiaControlPositions( );

		fCreated = TRUE;
	}
	else if( ( fShowMilitiaControlMenu == FALSE ) && ( fCreated == TRUE ) )
	{
		DeleteVideoObjectFromIndex(guiPOPUPBORDERS);
		DeleteVideoSurfaceFromIndex(guiPOPUPTEX);

		RemoveBox(ghMilitiaControlBox);
		ghMilitiaControlBox = -1;

		//RemoveBox(ghTalkToAllBox);
		//ghTalkToAllBox = -1;

		fCreated = FALSE;
		gfIgnoreScrolling = FALSE;		
	}

	return( TRUE );
}


void DetermineMilitiaControlBoxPositions( void )
{
	// depending on how many boxes there are, reposition as needed
	SGPPoint pPoint;
	SGPPoint pNewPoint;
	SGPRect pDimensions;
	SOLDIERTYPE *pSoldier = NULL;


	if( ( fShowMilitiaControlMenu == FALSE ) || ( ghMilitiaControlBox == -1 ) )
	{
		return;
	}

	pSoldier = GetSelectedAssignSoldier( TRUE );
	// pSoldier NULL is legal here!  Gets called during every mapscreen initialization even when nobody is assign char
	if ( pSoldier == NULL )
	{
		return;
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		GetBoxPosition( ghMilitiaControlBox, &pPoint );
		gsMilitiaControlBoxesX = ( INT16 )pPoint.iX;
		gsMilitiaControlBoxesY = ( INT16 )pPoint.iY;
	}

	pPoint.iX = gsMilitiaControlBoxesX;
	pPoint.iY = gsMilitiaControlBoxesY;

	
	SetBoxPosition( ghMilitiaControlBox, pPoint );
	GetBoxSize( ghMilitiaControlBox, &pDimensions );
	

	// hang it right beside the MilitiaControl/EPC box menu
	pNewPoint.iX = pPoint.iX + pDimensions.iRight;
	pNewPoint.iY = pPoint.iY;



	//if( ( fShowTalkToAllMenu == TRUE ) && ( ghTalkToAllBox != -1 ) )
	//{
	//	pNewPoint.iY += ( ( GetFontHeight( MAP_SCREEN_FONT ) + 2 ) * MILCON_MENU_ALL );
	//	SetBoxPosition( ghTalkToAllBox, pNewPoint );
	//	TrainPosition.iX = pNewPoint.iX;
	//	TrainPosition.iY = pNewPoint.iY;
	//	OrigTrainPosition.iY = pNewPoint.iY;
	//	OrigTrainPosition.iX = pNewPoint.iX;

	//	GetBoxSize( ghTalkToAllBox, &pDimensions );
	//	GetBoxPosition( ghTalkToAllBox, &pPoint );
	//}

	return;
}



void SetTacticalPopUpMilitiaControlBoxXY( SOLDIERTYPE *pSoldier )
{
	INT16 sX, sY;
	//SOLDIERTYPE *pSoldier;

	//get the soldier
	//pSoldier = GetSelectedAssignSoldier( FALSE ); //gives wrong position (0,0) when cursor moved
	
	//if ( !GetSoldier( &pSoldier, gusSelectedSoldier )  )
	//{
	//	//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"No Soldier" );
	//	return;
	//}

	// grab soldier's x,y screen position
	GetSoldierScreenPos( pSoldier, &sX, &sY );
	
	//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"X-Position: %d", sX );
	//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Y-Position: %d", sY );
	
	if( sX < 0 )
	{
		sX = 0;
	}

	gsMilitiaControlBoxesX = sX + 30;
	
	if( sY < 0 )
	{
		sY = 0;
	}
	
	gsMilitiaControlBoxesY = sY;

	// ATE: Check if we are past tactical viewport....
	// Use estimate width's/heights
	if ( ( gsMilitiaControlBoxesX + 100 ) > SCREEN_WIDTH )
	{
		gsMilitiaControlBoxesX = SCREEN_WIDTH - 100;
	}

	if ( ( gsMilitiaControlBoxesY + 130 ) > SCREEN_HEIGHT )
	{
		gsMilitiaControlBoxesY = SCREEN_HEIGHT - 190;
	}

	return;
}


void CheckAndUpdateTacticalMilitiaControlPopUpPositions( void )
{
	SGPRect pDimensions2;
	SGPPoint pPoint;	
	//SOLDIERTYPE *pSoldier = NULL;


	if( fShowMilitiaControlMenu == FALSE )
	{
		return;
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	//get the soldier
	//pSoldier = GetSelectedAssignSoldier( FALSE );

	//if( pSoldier -> ubWhatKindOfMercAmI == MERC_TYPE__EPC ) //laltodo
	{
		GetBoxSize( ghMilitiaControlBox, &pDimensions2 );
	}

	// just the MilitiaControl box
	if( gsMilitiaControlBoxesX + pDimensions2.iRight  >= SCREEN_WIDTH ) 
	{
		gsMilitiaControlBoxesX = ( INT16 ) ( SCREEN_WIDTH - 1 - ( pDimensions2.iRight  ) );
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	if( gsMilitiaControlBoxesY + pDimensions2.iBottom  >= SCREEN_HEIGHT-120 )
	{
		gsMilitiaControlBoxesY = ( INT16 )( SCREEN_HEIGHT - 121 - ( pDimensions2.iBottom  ) );
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	pPoint.iX = gsMilitiaControlBoxesX;
	pPoint.iY = gsMilitiaControlBoxesY;

	//if( pSoldier -> ubWhatKindOfMercAmI != MERC_TYPE__EPC) //laltodo
	{
		SetBoxPosition( ghMilitiaControlBox, pPoint );
	}

	RepositionMouseRegionsForMilitiaControl( );
}


void RepositionMouseRegionsForMilitiaControl( void )
{
	INT16 sDeltaX, sDeltaY;
	INT32 iCounter = 0;

	if( fShowMilitiaControlMenu == TRUE )
	{
		sDeltaX = gsMilitiaControlBoxesX - gMilitiaControlMenuRegion[ 0 ].RegionTopLeftX;
		sDeltaY = ( INT16 ) ( gsMilitiaControlBoxesY - gMilitiaControlMenuRegion[ 0 ].RegionTopLeftY + GetTopMarginSize( ghMilitiaControlBox ) );

		// find the delta from the old to the new, and alter values accordingly
		for( iCounter = 0; iCounter < ( INT32 )GetNumberOfLinesOfTextInBox( ghMilitiaControlBox ); iCounter++ )
		{
			gMilitiaControlMenuRegion[ iCounter ].RegionTopLeftX += sDeltaX;
			gMilitiaControlMenuRegion[ iCounter ].RegionTopLeftY += sDeltaY;

			gMilitiaControlMenuRegion[ iCounter ].RegionBottomRightX += sDeltaX;
			gMilitiaControlMenuRegion[ iCounter ].RegionBottomRightY += sDeltaY;
		}

		gfPausedTacticalRenderFlags = RENDER_FLAG_FULL;
	}
}



void PositionCursorForMilitiaControlBox( void )
{
	// position cursor over y of on duty in tactical assignments
	SGPPoint pPosition;
	SGPRect pDimensions;
	INT32 iFontHeight;

	// get x.y position of box
	GetBoxPosition( ghMilitiaControlBox, &pPosition);

	// get dimensions..mostly for width
	GetBoxSize( ghMilitiaControlBox, &pDimensions );

	iFontHeight = GetLineSpace( ghMilitiaControlBox ) + GetFontHeight( GetBoxFont( ghMilitiaControlBox ) ); 
	
	if( gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ] == FALSE )
	{
		SimulateMouseMovement( pPosition.iX + pDimensions.iRight - 6, pPosition.iY + ( iFontHeight / 2 ) + 2 );
	}
}


void HandleShadingOfLinesForMilitiaControlMenu( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// check if valid
	if( ( fShowMilitiaControlMenu == FALSE ) || ( ghMilitiaControlBox == - 1 ) )
	{
		return;
	}

	if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
	{
		// Check LOS!
		// Flugente: active radio sets allows us to give individual orders even without a line of sight
		if (pSoldier->CanUseRadio() || SoldierToSoldierLineOfSightTest(pSoldier, pTMilitiaSoldier, TRUE, CALC_FROM_ALL_DIRS))
		{
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ATTACK );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_HOLD );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_RETREAT );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_COMETOME );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_GETDOWN );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_CROUCH );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_TAKE_COVER );		
		}
		else
		{
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ATTACK );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_HOLD );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_RETREAT );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_COMETOME );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_GETDOWN );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_CROUCH );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_TAKE_COVER );
		}
	}

	if ( CheckIfRadioIsEquipped() )
	{
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_ATTACK );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_HOLD );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_RETREAT );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_COMETOME );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_SPREAD );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_GETDOWN );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_CROUCH );
		UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_COVER );
		//UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_ITEMS );
	}
	else
	{
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_ATTACK );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_HOLD );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_RETREAT );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_COMETOME );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_SPREAD );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_GETDOWN );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_CROUCH );
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_COVER );
		//ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_ITEMS );
	}
	
	return;
}


BOOLEAN CheckIfRadioIsEquipped( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT8 bSlot = NO_SLOT;

	// do we have a radio ?
	//pSoldier = GetSelectedAssignSoldier( FALSE ); //do not use
	
	if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
	{
		// Flugente: active radio sets also count as radio
		if ( pSoldier->CanUseRadio() )
			return TRUE;

		//bSlot = FindObj( pSoldier, EXTENDEDEAR );
		bSlot = FindHearingAid(pSoldier);
		//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Position: %d", bSlot );
	}

	if (( bSlot == HEAD1POS ) || ( bSlot == HEAD2POS))
	{
		return TRUE;
	}
	
	return FALSE;
}


void MilitiaControlMenuBtnCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for MilitiaControl region
	INT32 iValue = -1;
	SOLDIERTYPE * pSoldier = NULL;
	UINT8 ubVolume = 10;
	BOOLEAN fAllowSectorOrder = FALSE;
	BOOLEAN fCanCommunicate = FALSE;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	GetSoldier(&pSoldier, gusSelectedSoldier);

	if (pSoldier && pTMilitiaSoldier)
	{
		if (pSoldier->CanUseRadio() || SoldierToSoldierLineOfSightTest(pSoldier, pTMilitiaSoldier, TRUE, CALC_FROM_ALL_DIRS))
			fCanCommunicate = TRUE;

		if (CheckIfRadioIsEquipped())
			fAllowSectorOrder = TRUE;			
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		//if( ( fShowTalkToAllMenu ) )
		//{
		//	return;
		//}

		UnHighLightBox( ghMilitiaControlBox );

		//if( pSoldier -> ubWhatKindOfMercAmI != MERC_TYPE__EPC ) //laltodo
		if (pSoldier)
		{
			switch( iValue )
			{
				case( MILCON_MENU_ATTACK ):
					if (fCanCommunicate)
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							// Attack !!!
							pTMilitiaSoldier->aiData.bOrders = SEEKENEMY;
							pTMilitiaSoldier->aiData.bAttitude = AGGRESSIVE;
							pTMilitiaSoldier->usUIMovementMode = RUNNING;

							MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_SCREAM);
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;
				
				case( MILCON_MENU_HOLD ):
					if (fCanCommunicate)
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							//Hold Position !!!
							//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Hold Position" );
							pTMilitiaSoldier->aiData.bOrders = STATIONARY;
							//pTMilitiaSoldier->bAttitude = DEFENSIVE;
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;						
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;
				
				case( MILCON_MENU_RETREAT ):
					if (fCanCommunicate)
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							INT16 sActionGridNo;

							pTMilitiaSoldier->aiData.bOrders = FARPATROL;
							pTMilitiaSoldier->aiData.bAttitude = DEFENSIVE;
							pTMilitiaSoldier->usUIMovementMode = RUNNING;

							pTMilitiaSoldier->RetreatCounterStart(2);

							// set up next action to run away
							sActionGridNo =  FindSpotMaxDistFromOpponents( pTMilitiaSoldier );

							pTMilitiaSoldier->aiData.usNextActionData = sActionGridNo;
							
							if (!TileIsOutOfBounds(pTMilitiaSoldier->aiData.usNextActionData))
							{
								// sevenfm: use TAKE_COVER order to stop soldier from retreating
								pTMilitiaSoldier->aiData.bNextAction = AI_ACTION_TAKE_COVER;
								//pTMilitiaSoldier->aiData.bNextAction = AI_ACTION_RUN_AWAY;

								pTMilitiaSoldier->aiData.usActionData = ANIM_STAND;

								// SEND PENDING ACTION
								pTMilitiaSoldier->aiData.sPendingActionData2  = sActionGridNo;
								pTMilitiaSoldier->aiData.ubPendingActionAnimCount = 0;
							}

							if ( pTMilitiaSoldier->sGridNo != sActionGridNo )
							{
								SendGetNewSoldierPathEvent( pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode );
							}
						}
						
						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_COMETOME ):
					if (fCanCommunicate)
					{	
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							INT32 sActionGridNo, sGridNo, sAdjustedGridNo;
							UINT8	ubDirection;

							// OK, find an adjacent gridno....
							sGridNo = pSoldier->sGridNo;

							// See if we can get there
							sActionGridNo = FindAdjacentGridEx(pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE);
							if (sActionGridNo != -1)
							{
								// SEND PENDING ACTION
								//pTMilitiaSoldier->ubPendingAction = MERC_STEAL;
								pTMilitiaSoldier->aiData.sPendingActionData2 = pSoldier->sGridNo;
								//pTMilitiaSoldier->bPendingActionData3  = ubDirection;
								pTMilitiaSoldier->aiData.ubPendingActionAnimCount = 0;
								pTMilitiaSoldier->usUIMovementMode = RUNNING;

								// CHECK IF WE ARE AT THIS GRIDNO NOW
								if (pTMilitiaSoldier->sGridNo != sActionGridNo)
								{
									// WALK UP TO DEST FIRST
									SendGetNewSoldierPathEvent(pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode);
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;						
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_GETDOWN ):
					if (fCanCommunicate)
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							//if ( SoldierCanAffordNewStance( pTeamSoldier, ANIM_PRONE ) )
							{
								SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_PRONE );
								//SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_CROUCH );
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_CROUCH ):
					if (fCanCommunicate)
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
							SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_CROUCH );

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_TAKE_COVER ):
					if (fCanCommunicate)
					{	
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->stats.bLife >= OKLIFE) )
						{
							INT16 sActionGridNo;
							INT32 iDummy;						

							sActionGridNo =  FindBestNearbyCover(pTMilitiaSoldier,pTMilitiaSoldier->aiData.bAIMorale,&iDummy);
							
							if (!TileIsOutOfBounds(sActionGridNo))
							{
								// SEND PENDING ACTION
								pTMilitiaSoldier->aiData.sPendingActionData2  = sActionGridNo;
								pTMilitiaSoldier->aiData.ubPendingActionAnimCount = 0;
								pTMilitiaSoldier->usUIMovementMode = RUNNING;

								// CHECK IF WE ARE AT THIS GRIDNO NOW
								if ( pTMilitiaSoldier->sGridNo != sActionGridNo )
								{
									SendGetNewSoldierPathEvent( pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode );
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;


				case( MILCON_MENU_ALL_ATTACK ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								pTeamSoldier->aiData.bOrders = SEEKENEMY;
								pTeamSoldier->aiData.bAttitude = AGGRESSIVE;
								pTeamSoldier->usUIMovementMode = RUNNING;
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_ALL_HOLD ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;						
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								pTeamSoldier->aiData.bOrders = STATIONARY;
								pTeamSoldier->aiData.bAttitude = DEFENSIVE;
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_ALL_RETREAT ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						INT16 sActionGridNo;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								pTeamSoldier->aiData.bOrders = FARPATROL;
								pTeamSoldier->aiData.bAttitude = DEFENSIVE;
								pTeamSoldier->usUIMovementMode = RUNNING;

								pTeamSoldier->RetreatCounterStart(2);

								// set up next action to run away
								sActionGridNo =  FindSpotMaxDistFromOpponents( pTeamSoldier );

								pTeamSoldier->aiData.usNextActionData = sActionGridNo;

								if ( !TileIsOutOfBounds(pTeamSoldier->aiData.usNextActionData) )
								{
									// sevenfm: use TAKE_COVER order to stop soldier from retreating
									//pTeamSoldier->aiData.bNextAction = AI_ACTION_RUN_AWAY;
									pTeamSoldier->aiData.bNextAction = AI_ACTION_TAKE_COVER;

									pTeamSoldier->aiData.usActionData = ANIM_STAND;									

									// SEND PENDING ACTION
									pTeamSoldier->aiData.sPendingActionData2  = sActionGridNo;
									pTeamSoldier->aiData.ubPendingActionAnimCount = 0;
								}

								if ( pTeamSoldier->sGridNo != sActionGridNo )
								{
									SendGetNewSoldierPathEvent( pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode );
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;
				
				case( MILCON_MENU_ALL_COMETOME ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt, ubDirection;
						INT32 sActionGridNo, sGridNo, sAdjustedGridNo;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								// OK, find an adjacent gridno....
								sGridNo = pSoldier->sGridNo;

								// See if we can get there
								sActionGridNo = FindAdjacentGridEx(pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE);
								if (sActionGridNo != -1)
								{
									// SEND PENDING ACTION
									pTeamSoldier->aiData.sPendingActionData2 = pSoldier->sGridNo;
									//pTeamSoldier->bPendingActionData3  = ubDirection;
									pTeamSoldier->aiData.ubPendingActionAnimCount = 0;
									pTeamSoldier->usUIMovementMode = RUNNING;

									// CHECK IF WE ARE AT THIS GRIDNO NOW
									if (pTeamSoldier->sGridNo != sActionGridNo)
									{
										// WALK UP TO DEST FIRST
										SendGetNewSoldierPathEvent(pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode);
									}
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;
				

				case( MILCON_MENU_ALL_SPREAD ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						INT32 sActionGridNo;
						SOLDIERTYPE *pTeamSoldier;

						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{

								// See if we can get there
								sActionGridNo =  RandDestWithinRange( pTeamSoldier );
								if ( sActionGridNo != -1 )
								{
									// SEND PENDING ACTION
									pTeamSoldier->aiData.sPendingActionData2  = sActionGridNo;
									//pTeamSoldier->bPendingActionData3  = ubDirection;
									pTeamSoldier->aiData.ubPendingActionAnimCount = 0;
									pTeamSoldier->usUIMovementMode = RUNNING;

									// CHECK IF WE ARE AT THIS GRIDNO NOW
									if ( pTeamSoldier->sGridNo != sActionGridNo )
									{
										// WALK UP TO DEST FIRST
										SendGetNewSoldierPathEvent( pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode );
									}
								}

							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_ALL_GETDOWN ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								//if ( SoldierCanAffordNewStance( pTeamSoldier, ANIM_PRONE ) )
								{
									SendChangeSoldierStanceEvent( pTeamSoldier, ANIM_PRONE );
									//SendChangeSoldierStanceEvent( pTeamSoldier, ANIM_CROUCH );
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_ALL_CROUCH ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; ++cnt, ++pTeamSoldier)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								SendChangeSoldierStanceEvent( pTeamSoldier, ANIM_CROUCH );
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

				case( MILCON_MENU_ALL_TAKE_COVER ):
					if (fAllowSectorOrder)
					{
						UINT8 cnt;
						INT16 sActionGridNo;
						INT32 iDummy;
						SOLDIERTYPE *pTeamSoldier;

						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->stats.bLife >= OKLIFE) )
							{
								// See if we can get there
								sActionGridNo =  FindBestNearbyCover(pTeamSoldier,pTeamSoldier->aiData.bAIMorale,&iDummy);
																
								if (!TileIsOutOfBounds(sActionGridNo))
								{
									// SEND PENDING ACTION
									pTeamSoldier->aiData.sPendingActionData2  = sActionGridNo;
									//pTeamSoldier->bPendingActionData3  = ubDirection;
									pTeamSoldier->aiData.ubPendingActionAnimCount = 0;
									pTeamSoldier->usUIMovementMode = RUNNING;

									// CHECK IF WE ARE AT THIS GRIDNO NOW
									if ( pTeamSoldier->sGridNo != sActionGridNo )
									{
										// WALK UP TO DEST FIRST
										SendGetNewSoldierPathEvent( pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode );
									}
								}
							}
						}

						DeductPoints( pSoldier, APBPConstants[AP_TALK], 0 );
						StatChange( pSoldier, LDRAMT, 1, FALSE );

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						pSoldier->StartRadioAnimation();
					}
					break;

					case( MILCON_MENU_CANCEL ):
					{
						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;

						// reset list of characters
						ResetSelectedListForMapScreen( );
					}
					break;
			}
		}
		
		gfRenderPBInterface = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		//if( ( fShowTalkToAllMenu ) )
		//{
		//	fShowTalkToAllMenu = FALSE;

		//	// rerender tactical stuff
		//	gfRenderPBInterface = TRUE;

		//	// set dirty flag
		//	fTeamPanelDirty = TRUE;
		//	fMapScreenBottomDirty = TRUE;

		//	return;
		//}
	}
}
