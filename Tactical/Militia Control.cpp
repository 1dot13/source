#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
#else
	#include "Soldier Control.h"
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
#endif

#include "MilitiaSquads.h"

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
//MOUSE_REGION    gTalkToAllMenuRegion[ MAX_TRAIN_STRING_COUNT ];

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
	BOOLEAN fBattleInProgress = FALSE;

	if ( gWorldSectorX !=0 && gWorldSectorY != 0 && NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) )
		fBattleInProgress = TRUE;

	if( ( gfStrategicMilitiaChangesMade && !fBattleInProgress ) || gTacticalStatus.uiFlags & LOADING_SAVED_GAME || gfMSResetMilitia )
	{
		gfStrategicMilitiaChangesMade = FALSE;
		
		if( !gfMSResetMilitia )
			RemoveMilitiaFromTactical();
		PrepareMilitiaForTactical();
		gfMSResetMilitia = FALSE;
	}
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

void PrepareMilitiaForTactical()
{
	SECTORINFO *pSector;
	INT32 x;
	UINT8 ubGreen, ubRegs, ubElites;
	if( gbWorldSectorZ > 0 )
		return;

	// Do we have a loaded sector?
	if ( gWorldSectorX ==0 && gWorldSectorY == 0 )
		return;

	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
	ubGreen = pSector->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
	ubRegs = pSector->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
	ubElites = pSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
	
	if(guiDirNumber)
		for( x = 0 ; x < guiDirNumber ; ++x )
		{
//			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld,%ld,%ld", gpAttackDirs[ x ][ 0 ], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2], gpAttackDirs[ x ][3] );
			if( gfMSResetMilitia )
			{
				if( gpAttackDirs[ x ][ 3 ] != INSERTION_CODE_CENTER )
					AddSoldierInitListMilitiaOnEdge( gpAttackDirs[ x ][ 3 ], gpAttackDirs[ x ][0], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2] );
			}
			else
				if( gpAttackDirs[ x ][ 3 ] == INSERTION_CODE_CENTER )
					AddSoldierInitListMilitia( gpAttackDirs[ x ][0], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2] );
				else
					AddSoldierInitListMilitiaOnEdge( gpAttackDirs[ x ][ 3 ], gpAttackDirs[ x ][0], gpAttackDirs[ x ][1], gpAttackDirs[ x ][2] );

		}
		else AddSoldierInitListMilitia( ubGreen, ubRegs, ubElites );
	
//	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
//	{
//		if( MercPtrs[ i ]->bInSector )
//		{
//			MercPtrs[ i ]->bOrders = SEEKENEMY;
////			MercPtrs[ i ]->bAttitude = AGGRESSIVE;
//		}
//	}
	guiDirNumber = 0;
}

void HandleMilitiaPromotions( void )
{
	UINT8						cnt;
	UINT8						ubMilitiaRank;
	SOLDIERTYPE *		pTeamSoldier;
	UINT8						ubPromotions;

	gbGreenToElitePromotions = 0;
	gbGreenToRegPromotions = 0;
	gbRegToElitePromotions = 0;
	gbMilitiaPromotions = 0;

	cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bLife > 0 )
		{
			if ( pTeamSoldier->ubMilitiaKills > 0 )
			{	
				ubMilitiaRank = SoldierClassToMilitiaRank( pTeamSoldier->ubSoldierClass );
				ubPromotions = CheckOneMilitiaForPromotion( gWorldSectorX, gWorldSectorY, ubMilitiaRank, pTeamSoldier->ubMilitiaKills );
				if( ubPromotions )
				{
					if( ubPromotions == 2 )
					{
						gbGreenToElitePromotions++;
						gbMilitiaPromotions++;
					}
					else if( pTeamSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA )
					{
						gbGreenToRegPromotions++;
						gbMilitiaPromotions++;
					}
					else if( pTeamSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
					{
						gbRegToElitePromotions++;
						gbMilitiaPromotions++;
					}
				}

				pTeamSoldier->ubMilitiaKills = 0;
			}
		}
	}
	if( gbMilitiaPromotions )
	{
    // ATE: Problems here with bringing up message box...

		// CHAR16 str[ 512 ];
		// BuildMilitiaPromotionsString( str );
		// DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
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
	SOLDIERTYPE *pSoldier = NULL;

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if( fShowMapScreenMovementList == TRUE )
		{
			if( bSelectedDestChar == -1 )
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
	SOLDIERTYPE *pSoldier = NULL;
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
	INT16 sDistVisible;

	// check if valid
	if( ( fShowMilitiaControlMenu == FALSE ) || ( ghMilitiaControlBox == - 1 ) )
	{
		return;
	}

	if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
	{
		sDistVisible = DistanceVisible( pSoldier, DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, pTMilitiaSoldier->sGridNo, pTMilitiaSoldier->bLevel, pSoldier );

		// Check LOS!
		if ( SoldierTo3DLocationLineOfSightTest( pSoldier, pTMilitiaSoldier->sGridNo,  pTMilitiaSoldier->bLevel, 3, (UINT8) sDistVisible, TRUE ) )
		{
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ATTACK );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_HOLD );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_RETREAT );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_COMETOME );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_GETDOWN );
			UnShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_TAKE_COVER );		
		}
		else
		{
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ATTACK );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_HOLD );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_RETREAT );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_COMETOME );
			ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_GETDOWN );
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
		ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_COVER );
		//ShadeStringInBox( ghMilitiaControlBox, MILCON_MENU_ALL_TAKE_ITEMS );
	}
	
	return;
}


BOOLEAN CheckIfRadioIsEquipped( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT8 bSlot;

	// do we have a radio ?
	//pSoldier = GetSelectedAssignSoldier( FALSE ); //do not use
	
	if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
	{	
		bSlot = FindObj( pSoldier, EXTENDEDEAR );
		//bSlot = FindHearingAid(pSoldier);
		//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Position: %d", bSlot );
	}

	if (( bSlot == HEAD1POS ) || ( bSlot == HEAD2POS))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//INT16	sFacingDir, sXPos, sYPos, sAPCost;

//ConvertGridNoToXY( pTMilitiaSoldier->sGridNo, &sXPos, &sYPos );				
//sFacingDir = GetDirectionFromXY( sXPos, sYPos, pTMilitiaSoldier );
//SendSoldierSetDesiredDirectionEvent( pTMilitiaSoldier, sFacingDir );
//SendSoldierSetDesiredDirectionEvent( pTMilitiaSoldier, gOppositeDirection[ sFacingDir ] );
//EVENT_StopMerc( pTMilitiaSoldier, pTMilitiaSoldier->sGridNo, pTMilitiaSoldier->bDirection );

//SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_PRONE );
//SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_CROUCH );

//pTMilitiaSoldier->bNextAction = AI_ACTION_CHANGE_STANCE;
//pTMilitiaSoldier->usActionData = ANIM_PRONE;



//// some orders are more offensive, others more defensive
//switch( pSoldier->bOrders )
//{
//	case STATIONARY:	swprintf( szOrders, L"STATIONARY" );			break;
//	case ONGUARD:			swprintf( szOrders, L"ON GUARD" );				break;
//	case ONCALL:			swprintf( szOrders, L"ON CALL" );					break;
//	case SEEKENEMY:		swprintf( szOrders, L"SEEK ENEMY" );			break;
//	case CLOSEPATROL:	swprintf( szOrders, L"CLOSE PATROL" );		break;
//	case FARPATROL:		swprintf( szOrders, L"FAR PATROL" );			break;
//	case POINTPATROL:	swprintf( szOrders, L"POINT PATROL" );		break;
//	case RNDPTPATROL:	swprintf( szOrders, L"RND PT PATROL" );		break;
//	default:					swprintf( szOrders, L"UNKNOWN" );					break;
//}
//switch( pSoldier->bAttitude )
//{
//	case DEFENSIVE:		swprintf( szAttitude, L"DEFENSIVE" );			break;
//	case BRAVESOLO:		swprintf( szAttitude, L"BRAVE SOLO" );		break;
//	case BRAVEAID:		swprintf( szAttitude, L"BRAVE AID" );			break;
//	case AGGRESSIVE:	swprintf( szAttitude, L"AGGRESSIVE" );		break;
//	case CUNNINGSOLO:	swprintf( szAttitude, L"CUNNING SOLO" );	break;
//	case CUNNINGAID:	swprintf( szAttitude, L"CUNNING AID"	);	break;
//	default:					swprintf( szAttitude, L"UNKNOWN" );				break;
//}

//MILCON_MENU_ATTACK,
//MILCON_MENU_HOLD,
//MILCON_MENU_RETREAT,
//MILCON_MENU_COMETOME,
//MILCON_MENU_GETDOWN,
//MILCON_MENU_ALL_ATTACK,
//MILCON_MENU_ALL_HOLD,
//MILCON_MENU_ALL_RETREAT,
//MILCON_MENU_ALL_COMETOME,
//MILCON_MENU_ALL_GETDOWN,
//MILCON_MENU_CANCEL,
//MAX_MILCON_STRING_COUNT,


//AI_ACTION_NONE = 0,						// maintain current position & facing

//// actions that involve a move to another tile
//AI_ACTION_RANDOM_PATROL,			// move towards a random destination
//AI_ACTION_SEEK_FRIEND,				// move towards friend in trouble
//AI_ACTION_SEEK_OPPONENT,			// move towards a reported opponent
//AI_ACTION_TAKE_COVER,					// run for nearest cover from threat
//AI_ACTION_GET_CLOSER,					// move closer to a strategic location

//AI_ACTION_POINT_PATROL,				// move towards next patrol point
//AI_ACTION_LEAVE_WATER_GAS,		// seek nearest spot of ungassed land
//AI_ACTION_SEEK_NOISE,					// seek most important noise heard
//AI_ACTION_ESCORTED_MOVE,			// go where told to by escortPlayer
//AI_ACTION_RUN_AWAY,						// run away from nearby opponent(s)

//AI_ACTION_KNIFE_MOVE,					// preparing to stab an opponent
//AI_ACTION_APPROACH_MERC,			// move up to a merc in order to talk with them; RT
//AI_ACTION_TRACK,							// track a scent
//AI_ACTION_EAT,								// monster eats corpse
//AI_ACTION_PICKUP_ITEM,				// grab things lying on the ground

//AI_ACTION_SCHEDULE_MOVE,			// move according to schedule
//AI_ACTION_WALK,								// walk somewhere (NPC stuff etc)
//AI_ACTION_RUN,								// run somewhere (NPC stuff etc)
//AI_ACTION_WITHDRAW,	// back off
//AI_ACTION_FLANK_LEFT,	// move to the left
//AI_ACTION_FLANK_RIGHT,	// move to the right
//AI_ACTION_MOVE_TO_CLIMB,			// move to edge of roof/building
//// miscellaneous movement actions
//AI_ACTION_CHANGE_FACING,			// turn to face a different direction

//AI_ACTION_CHANGE_STANCE,			// stand, crouch, or go prone
//// actions related to items and attacks
//AI_ACTION_YELLOW_ALERT,				// tell friends opponent(s) heard
//AI_ACTION_RED_ALERT,					// tell friends opponent(s) seen
//AI_ACTION_CREATURE_CALL,			// creature communication
//AI_ACTION_PULL_TRIGGER,				// go off to activate a panic trigger

//AI_ACTION_USE_DETONATOR,			// grab detonator and set off bomb(s)
//AI_ACTION_FIRE_GUN,						// shoot at nearby opponent
//AI_ACTION_TOSS_PROJECTILE,		// throw grenade at/near opponent(s)
//AI_ACTION_KNIFE_STAB,					// during the actual knifing attack
//AI_ACTION_THROW_KNIFE,				// throw a knife

//AI_ACTION_GIVE_AID,						// help injured/dying friend
//AI_ACTION_WAIT,								// RT: don't do anything for a certain length of time
//AI_ACTION_PENDING_ACTION,			// RT: wait for pending action (pickup, door open, etc) to finish
//AI_ACTION_DROP_ITEM,					// duh
//AI_ACTION_COWER,							// for civilians:  cower in fear and stay there!

//AI_ACTION_STOP_COWERING,			// stop cowering
//AI_ACTION_OPEN_OR_CLOSE_DOOR,	// schedule-provoked; open or close door
//AI_ACTION_UNLOCK_DOOR,				// schedule-provoked; unlock door (don't open)
//AI_ACTION_LOCK_DOOR,					// schedule-provoked; lock door (close if necessary)
//AI_ACTION_LOWER_GUN,					// lower gun prior to throwing knife

//AI_ACTION_ABSOLUTELY_NONE,		// like "none" but can't be converted to a wait by realtime
//AI_ACTION_CLIMB_ROOF,					// climb up or down roof
//AI_ACTION_END_TURN,						// end turn (after final stance change)
//AI_ACTION_END_COWER_AND_MOVE,	// sort of dummy value, special for civilians who are to go somewhere at end of battle
//AI_ACTION_TRAVERSE_DOWN,			// move down a level
//AI_ACTION_OFFER_SURRENDER,		// offer surrender to the player


//// stand up!
//pSoldier->bAction = AI_ACTION_CHANGE_STANCE;
//pSoldier->usActionData = ANIM_STAND;
//
//

//pSoldier->bAction = pSoldier->bNextAction;
//pSoldier->usActionData = pSoldier->usNextActionData;
//pSoldier->bTargetLevel = pSoldier->bNextTargetLevel;
//pSoldier->bNextAction = AI_ACTION_NONE;
//pSoldier->usNextActionData = 0;
//pSoldier->bNextTargetLevel = 0;
//


//EV_S_CHANGESTATE			SChangeState;
//EV_S_CHANGEDEST				SChangeDest;
//EV_S_SETPOSITION			SSetPosition;
//EV_S_GETNEWPATH				SGetNewPath;
//EV_S_BEGINTURN				SBeginTurn;
//EV_S_CHANGESTANCE			SChangeStance;
//EV_S_SETDIRECTION			SSetDirection;
//EV_S_SETDESIREDDIRECTION			SSetDesiredDirection;
//EV_S_BEGINFIREWEAPON	SBeginFireWeapon;
//EV_S_FIREWEAPON				SFireWeapon;
//EV_S_WEAPONHIT				SWeaponHit;
//EV_S_STRUCTUREHIT			SStructureHit;
//EV_S_WINDOWHIT				SWindowHit;
//EV_S_MISS							SMiss;
//EV_S_NOISE						SNoise;
//EV_S_STOP_MERC				SStopMerc;
//EV_S_SENDPATHTONETWORK SUpdateNetworkSoldier;

void MilitiaControlMenuBtnCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for MilitiaControl region
	INT32 iValue = -1;
	SOLDIERTYPE * pSoldier = NULL;
	UINT8 ubVolume = 10;

	//pSoldier = GetSelectedAssignSoldier( FALSE );

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		//if( ( fShowTalkToAllMenu ) )
		//{
		//	return;
		//}

		UnHighLightBox( ghMilitiaControlBox );

		//if( pSoldier -> ubWhatKindOfMercAmI != MERC_TYPE__EPC ) //laltodo
		{
			switch( iValue )
			{
				case( MILCON_MENU_ATTACK ):					
					{					
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							// Attack !!!

							pTMilitiaSoldier->bOrders = SEEKENEMY;
							pTMilitiaSoldier->bAttitude = AGGRESSIVE;
							pTMilitiaSoldier->usUIMovementMode = RUNNING;

							if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
							{
								//ubVolume = CalcScreamVolume( pSoldier, 10 );
								MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->bLevel, pSoldier->bOverTerrainType, ubVolume, NOISE_SCREAM);
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;
				
				case( MILCON_MENU_HOLD ):						
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							//Hold Position !!!
							//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Hold Position" );
							pTMilitiaSoldier->bOrders = STATIONARY;
							//pTMilitiaSoldier->bAttitude = DEFENSIVE;
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;						
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;
				
				case( MILCON_MENU_RETREAT ):
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							INT16 sActionGridNo;

							pTMilitiaSoldier->bOrders = FARPATROL;
							pTMilitiaSoldier->bAttitude = DEFENSIVE;
							pTMilitiaSoldier->usUIMovementMode = RUNNING;

							// set up next action to run away
							sActionGridNo =  FindSpotMaxDistFromOpponents( pTMilitiaSoldier );

							pTMilitiaSoldier->usNextActionData = sActionGridNo;

							if ( pTMilitiaSoldier->usNextActionData != NOWHERE )
							{
								pTMilitiaSoldier->bNextAction = AI_ACTION_RUN_AWAY;
								pTMilitiaSoldier->usActionData = ANIM_STAND;

								// SEND PENDING ACTION
								pTMilitiaSoldier->sPendingActionData2  = sActionGridNo;
								pTMilitiaSoldier->ubPendingActionAnimCount = 0;
							}

							if ( pTMilitiaSoldier->sGridNo != sActionGridNo )
							{
								SendGetNewSoldierPathEvent( pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode );
							}
						}
						
						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;

				case( MILCON_MENU_COMETOME ):
					{	
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							INT16 sActionGridNo, sGridNo, sAdjustedGridNo;
							UINT8	ubDirection;

							if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
							{
								// OK, find an adjacent gridno....
								sGridNo = pSoldier->sGridNo;

								// See if we can get there
								sActionGridNo =  FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
								if ( sActionGridNo != -1 )
								{
									// SEND PENDING ACTION
									//pTMilitiaSoldier->ubPendingAction = MERC_STEAL;
									pTMilitiaSoldier->sPendingActionData2  = pSoldier->sGridNo;
									//pTMilitiaSoldier->bPendingActionData3  = ubDirection;
									pTMilitiaSoldier->ubPendingActionAnimCount = 0;
									pTMilitiaSoldier->usUIMovementMode = RUNNING;

									// CHECK IF WE ARE AT THIS GRIDNO NOW
									if ( pTMilitiaSoldier->sGridNo != sActionGridNo )
									{
										// WALK UP TO DEST FIRST
										SendGetNewSoldierPathEvent( pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode );
									}
								}
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;						
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;

				case( MILCON_MENU_GETDOWN ):
					{
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							//if ( SoldierCanAffordNewStance( pTeamSoldier, ANIM_PRONE ) )
							{
								SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_PRONE );
								//SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_CROUCH );
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;


					case( MILCON_MENU_TAKE_COVER ):
					{	
						if ( (pTMilitiaSoldier->bActive) && (pTMilitiaSoldier->bInSector) && (pTMilitiaSoldier->bLife >= OKLIFE) )
						{
							INT16 sActionGridNo;
							INT32 iDummy;						

							sActionGridNo =  FindBestNearbyCover(pTMilitiaSoldier,pTMilitiaSoldier->bAIMorale,&iDummy);

							if ( sActionGridNo != NOWHERE )
							{
								// SEND PENDING ACTION
								pTMilitiaSoldier->sPendingActionData2  = sActionGridNo;
								pTMilitiaSoldier->ubPendingActionAnimCount = 0;
								pTMilitiaSoldier->usUIMovementMode = RUNNING;

								// CHECK IF WE ARE AT THIS GRIDNO NOW
								if ( pTMilitiaSoldier->sGridNo != sActionGridNo )
								{
									SendGetNewSoldierPathEvent( pTMilitiaSoldier, sActionGridNo, pTMilitiaSoldier->usUIMovementMode );
								}
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;


				case( MILCON_MENU_ALL_ATTACK ):
					{
						UINT8 cnt;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								pTeamSoldier->bOrders = SEEKENEMY;
								pTeamSoldier->bAttitude = AGGRESSIVE;
								pTeamSoldier->usUIMovementMode = RUNNING;
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;

				case( MILCON_MENU_ALL_HOLD ):
					{
						UINT8 cnt;						
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								pTeamSoldier->bOrders = STATIONARY;
								pTeamSoldier->bAttitude = DEFENSIVE;
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;

				case( MILCON_MENU_ALL_RETREAT ):
					{
						UINT8 cnt;
						INT16 sActionGridNo;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								pTeamSoldier->bOrders = FARPATROL;
								pTeamSoldier->bAttitude = DEFENSIVE;
								pTeamSoldier->usUIMovementMode = RUNNING;

								//// set up next action to run away
								//pTeamSoldier->usNextActionData = FindSpotMaxDistFromOpponents( pTeamSoldier );

								//if ( pTeamSoldier->usNextActionData != NOWHERE )
								//{
								//	pTeamSoldier->bNextAction = AI_ACTION_RUN_AWAY;
								//	pTeamSoldier->usActionData = ANIM_STAND;									
								//}

								// set up next action to run away
								sActionGridNo =  FindSpotMaxDistFromOpponents( pTeamSoldier );

								pTeamSoldier->usNextActionData = sActionGridNo;

								if ( pTeamSoldier->usNextActionData != NOWHERE )
								{
									pTeamSoldier->bNextAction = AI_ACTION_RUN_AWAY;
									pTeamSoldier->usActionData = ANIM_STAND;									

									// SEND PENDING ACTION
									pTeamSoldier->sPendingActionData2  = sActionGridNo;
									pTeamSoldier->ubPendingActionAnimCount = 0;
								}

								if ( pTeamSoldier->sGridNo != sActionGridNo )
								{
									SendGetNewSoldierPathEvent( pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode );
								}
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;
				
				case( MILCON_MENU_ALL_COMETOME ):
					{
						UINT8 cnt, ubDirection;
						INT16 sActionGridNo, sGridNo, sAdjustedGridNo;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
								{
									// OK, find an adjacent gridno....
									sGridNo = pSoldier->sGridNo;

									// See if we can get there
									sActionGridNo =  FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
									if ( sActionGridNo != -1 )
									{
										// SEND PENDING ACTION
										pTeamSoldier->sPendingActionData2  = pSoldier->sGridNo;
										//pTeamSoldier->bPendingActionData3  = ubDirection;
										pTeamSoldier->ubPendingActionAnimCount = 0;
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
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;
				

				case( MILCON_MENU_ALL_SPREAD ):
					{
						UINT8 cnt;
						INT16 sActionGridNo;
						SOLDIERTYPE *pTeamSoldier;

						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{

								// See if we can get there
								sActionGridNo =  RandDestWithinRange( pTeamSoldier );
								if ( sActionGridNo != -1 )
								{
									// SEND PENDING ACTION
									pTeamSoldier->sPendingActionData2  = sActionGridNo;
									//pTeamSoldier->bPendingActionData3  = ubDirection;
									pTeamSoldier->ubPendingActionAnimCount = 0;
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

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;



				case( MILCON_MENU_ALL_GETDOWN ):
					{
						UINT8 cnt;
						SOLDIERTYPE *pTeamSoldier;
						
						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								//if ( SoldierCanAffordNewStance( pTeamSoldier, ANIM_PRONE ) )
								{
									SendChangeSoldierStanceEvent( pTeamSoldier, ANIM_PRONE );
									//SendChangeSoldierStanceEvent( pTeamSoldier, ANIM_CROUCH );
								}
							}
						}

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;



					case( MILCON_MENU_ALL_TAKE_COVER ):
					{
						//UINT8 cnt;
						//SOLDIERTYPE *pTeamSoldier;
						//INT32 iDummy;
						
						//cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						//for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						//{
						//	if ( pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bLife > 0 )
						//	{
						//		pTeamSoldier->usActionData = FindBestNearbyCover(pTeamSoldier,pTeamSoldier->bAIMorale,&iDummy);
						//								
						//		pTeamSoldier->usNextActionData = FindBestNearbyCover(pTeamSoldier,pTeamSoldier->bAIMorale,&iDummy);
						//		
						//		//if ( pTeamSoldier->usNextActionData != NOWHERE )
						//		{
						//			pTeamSoldier->bNextAction = AI_ACTION_TAKE_COVER;
						//			pTeamSoldier->usActionData = ANIM_STAND;									
						//		}
						//	}
						//}


						UINT8 cnt;
						INT16 sActionGridNo;
						INT32 iDummy;
						SOLDIERTYPE *pTeamSoldier;

						cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

						for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
						{
							if ( (pTeamSoldier->bActive) && (pTeamSoldier->bInSector) && (pTeamSoldier->bLife >= OKLIFE) )
							{
								// See if we can get there
								sActionGridNo =  FindBestNearbyCover(pTeamSoldier,pTeamSoldier->bAIMorale,&iDummy);
								
								if ( sActionGridNo != NOWHERE )
								{
									// SEND PENDING ACTION
									pTeamSoldier->sPendingActionData2  = sActionGridNo;
									//pTeamSoldier->bPendingActionData3  = ubDirection;
									pTeamSoldier->ubPendingActionAnimCount = 0;
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

						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
						{
							DeductPoints( pSoldier, AP_TALK, 0 );
							StatChange( pSoldier, LDRAMT, 1, FALSE );
						}

						// stop showing menu
						fShowMilitiaControlMenu = FALSE;
						giAssignHighLine = -1;

						// set dirty flag
						fTeamPanelDirty = TRUE;
						fMapScreenBottomDirty = TRUE;
					}
					break;



					//case( MILCON_MENU_ALL_TAKE_ITEMS ):
					//{
					//	UINT8 cnt;
					//	SOLDIERTYPE *pTeamSoldier;
					//	
					//	cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;

					//	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
					//	{
					//		if ( pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bLife > 0 )
					//		{

					//			//CREATURE_IMMOBILE = 2
					//			if ( (pTeamSoldier->bMobility != 2) && (pTeamSoldier->bActionPoints >= MinPtsToMove(pTeamSoldier)) )
					//			{
					//				//SEARCH_GENERAL_ITEMS=0
					//				pTeamSoldier->bAction = SearchForItems( pTeamSoldier, 0, pTeamSoldier->inv[HANDPOS].usItem );

					//				pTeamSoldier->usNextActionData = SearchForItems( pTeamSoldier, 0, pTeamSoldier->inv[HANDPOS].usItem );


					//				//if ( pTeamSoldier->usNextActionData != NOWHERE )
					//				{
					//					pTeamSoldier->bNextAction = AI_ACTION_PICKUP_ITEM;
					//					//pTeamSoldier->usActionData = ANIM_STAND;									
					//				}

					//				if (pTeamSoldier->bAction == AI_ACTION_PICKUP_ITEM)
					//				{
					//					// the item pool index was stored in the special data field
					//					pTeamSoldier->uiPendingActionData1 = pTeamSoldier->iNextActionSpecialData;
					//				}

					//				//if (pTeamSoldier->bAction != AI_ACTION_NONE)
					//				//{
					//				//	return( pTeamSoldier->bAction );
					//				//}
					//			}
					//		}
					//	}
					
					//if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
					//{
					//	DeductPoints( pSoldier, AP_TALK, 0 );
					//}

					//	// stop showing menu
					//	fShowMilitiaControlMenu = FALSE;
					//	giAssignHighLine = -1;

					//	// set dirty flag
					//	fTeamPanelDirty = TRUE;
					//	fMapScreenBottomDirty = TRUE;
					//}
					//break;

					case( MILCON_MENU_CANCEL ):
					{
						//Cancel
						//ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Cancel" );

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






























//
//
//void CreateTalkToAllBox( void )
//{
// UINT32 hStringHandle;
// UINT32 uiCounter;
//
// // will create attribute pop up menu for mapscreen assignments
//
// TalkToAllPosition.iX = OrigTalkToAllPosition.iX;
//
// if( giBoxY != 0 )
// {
//	 TalkToAllPosition.iY = giBoxY + ( MILCON_MENU_ALL * GetFontHeight( MAP_SCREEN_FONT ) );
// }
//
// // create basic box
// CreatePopUpBox(&ghTalkToAllBox, TalkToAllDimensions, TalkToAllPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));
//
// // which buffer will box render to
// SetBoxBuffer(ghTalkToAllBox, FRAME_BUFFER);
//
// // border type?
// SetBorderType(ghTalkToAllBox,guiPOPUPBORDERS);
//
// // background texture
// SetBackGroundSurface(ghTalkToAllBox, guiPOPUPTEX);
//
// // margin sizes
// SetMargins(ghTalkToAllBox, 6, 6, 4, 4 );
//
// // space between lines
// SetLineSpace(ghTalkToAllBox, 2);
//
// // set current box to this one
// SetCurrentBox( ghTalkToAllBox );
//
//
// // add strings for box
// for(uiCounter=0; uiCounter < MAX_TTALL_STRING_COUNT; uiCounter++)
// {
//  AddMonoString(&hStringHandle, pTalkToAllMenuStrings[uiCounter]);
//  
//	// make sure it is unhighlighted
//	UnHighLightLine(hStringHandle);
// }
//
// // set font type
// SetBoxFont(ghTalkToAllBox, MAP_SCREEN_FONT);
//
// // set highlight color
// SetBoxHighLight(ghTalkToAllBox, FONT_WHITE);
//
// // unhighlighted color
// SetBoxForeground(ghTalkToAllBox, FONT_LTGREEN);
//
// // background color
// SetBoxBackground(ghTalkToAllBox, FONT_BLACK);
//
// // shaded color..for darkened text
// SetBoxShade( ghTalkToAllBox, FONT_GRAY7 );
//
// // resize box to text
// ResizeBoxToText( ghTalkToAllBox );
//
// DetermineMilitiaControlBoxPositions( );
//
//}
//
//
//
//
//void CreateDestroyMouseRegionsForTalkToAllMenu( void )
//{
//	static BOOLEAN fCreated = FALSE;
//	UINT32 iCounter = 0;
//	INT32 iFontHeight = 0;
//	INT32 iBoxXPosition = 0;
//	INT32 iBoxYPosition = 0;
//	SGPPoint pPosition;
//	INT32 iBoxWidth = 0;
//	SGPRect pDimensions;
//
//	// will create/destroy mouse regions for the map screen assignment main menu
//
//	if( ( fShowTalkToAllMenu == TRUE ) && ( fCreated == FALSE ) )
//	{
//
//		if( ( fShowTalkToAllMenu ) && ( guiCurrentScreen == MAP_SCREEN ) ) //laltodo
//		{
//		  SetBoxPosition( ghTalkToAllBox, TalkToAllPosition );
//		}
//	
//		HandleShadingOfLinesForTalkToAllMenu( );
//
//		CheckAndUpdateTacticalMilitiaControlPopUpPositions( );
//
//		// grab height of font
//		iFontHeight = GetLineSpace( ghTalkToAllBox ) + GetFontHeight( GetBoxFont( ghTalkToAllBox ) ); 
//
//		// get x.y position of box
//		GetBoxPosition( ghTalkToAllBox, &pPosition);
//
//		// grab box x and y position
//		iBoxXPosition = pPosition.iX;
//		iBoxYPosition = pPosition.iY;
//
//		// get dimensions..mostly for width
//		GetBoxSize( ghTalkToAllBox, &pDimensions );
//		SetBoxSecondaryShade( ghTalkToAllBox, FONT_YELLOW );
//
//		// get width
//		iBoxWidth = pDimensions.iRight;
//
//		SetCurrentBox( ghTalkToAllBox );
//
//		// define regions
//		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghTalkToAllBox ); iCounter++ )
//		{
//			// add mouse region for each line of text..and set user data
//		
//
//			MSYS_DefineRegion( &gTalkToAllMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghTalkToAllBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghTalkToAllBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 3 ,
//							 MSYS_NO_CURSOR, TalkToAllMenuMvtCallBack, TalkToAllMenuBtnCallback );
//
//			// set user defines
//			MSYS_SetRegionUserData( &gTalkToAllMenuRegion[ iCounter ], 0, iCounter );
//		}
//
//		// created
//		fCreated = TRUE;
//
//		// unhighlight all strings in box
//		UnHighLightBox( ghTalkToAllBox );
//
//	}
//	else if( ( ( fShowMilitiaControlMenu == FALSE ) || ( fShowTalkToAllMenu == FALSE ) ) && ( fCreated == TRUE ) )
//	{
//		// destroy 
//		for( iCounter = 0; iCounter < GetNumberOfLinesOfTextInBox( ghTalkToAllBox ); iCounter++ )
//		{
//			MSYS_RemoveRegion( &gTalkToAllMenuRegion[ iCounter ] );
//		}
//
//		// stop showing TalkToAll menu
//		if( fShowMilitiaControlMenu == FALSE )
//		{
//			fShowTalkToAllMenu = FALSE;
//		}
//
//		RestorePopUpBoxes( );
//
//		fMapPanelDirty = TRUE;
//		fCharacterInfoPanelDirty= TRUE;
//		fTeamPanelDirty = TRUE;
//		fMapScreenBottomDirty = TRUE;
//		HideBox( ghTalkToAllBox );
//		SetRenderFlags( RENDER_FLAG_FULL );
//		
//		// not created
//		fCreated = FALSE;
//
//		if ( fShowMilitiaControlMenu )
//		{
//			// remove highlight on the parent menu
//			UnHighLightBox( ghMilitiaControlBox );
//		}
//	}
//}
//
//void TalkToAllMenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
//{
//	// mvt callback handler for assignment region
//	INT32 iValue = -1;
//	
//	iValue = MSYS_GetRegionUserData( pRegion, 0 );
//
//	//if( HandleAssignmentExpansionAndHighLightForTalkToAllMenu( ) == TRUE )
//	//{
//	//	return;
//	//}
//
//	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
//	{
//		// highlight string
//
//		// do not highlight current balance
//		if( GetBoxShadeFlag( ghTalkToAllBox, iValue ) == FALSE )
//		{
//		  // get the string line handle
//		  HighLightBoxLine( ghTalkToAllBox, iValue );
//		}
//	}
//	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
//	{
//		// unhighlight all strings in box
//		UnHighLightBox( ghTalkToAllBox );
//	}
//}
//
//
//
//void TalkToAllMenuBtnCallback( MOUSE_REGION * pRegion, INT32 iReason )
//{
//	// btn callback handler for assignment region
//	INT32 iValue = -1;
//	SOLDIERTYPE * pSoldier = NULL;
//
//	iValue = MSYS_GetRegionUserData( pRegion, 0 );
//
//	//if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ) || ( iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN ) )
//	//{
//	//	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && !fShowMapInventoryPool )
//	//	{
//	//		UnMarkButtonDirty( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] );
//	//	}
//	//}
//
//	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
//	{
//		switch( iValue )
//		{
//			case( TTALL_MENU_ATTACK ):
//			{					
//				// Attack !!!
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Attack" );
//
//				pTMilitiaSoldier->bOrders = SEEKENEMY;
//				pTMilitiaSoldier->bAttitude = AGGRESSIVE;
//
//				// stop showing menu
//				fShowMilitiaControlMenu = FALSE;
//				//fShowTalkToAllMenu = FALSE;
//				giAssignHighLine = -1;
//			}
//			break;
//
//			case( TTALL_MENU_HOLD ):						
//			{
//				//Hold Position !!!
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Hold Position" );
//				
//				pTMilitiaSoldier->bOrders = STATIONARY;
//				//pTMilitiaSoldier->bAttitude = DEFENSIVE;
//
//				// stop showing menu
//				fShowMilitiaControlMenu = FALSE;
//				//fShowTalkToAllMenu = FALSE;
//				giAssignHighLine = -1;
//			}
//			break;
//
//			case( TTALL_MENU_RETREAT ):
//			{
//				// Retreat !!!
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Retreat" );
//				
//				pTMilitiaSoldier->bOrders = FARPATROL;
//				//pTMilitiaSoldier->bAttitude = DEFENSIVE;
//
//				// set up next action to run away
//				pTMilitiaSoldier->usNextActionData = FindSpotMaxDistFromOpponents( pTMilitiaSoldier );
//				if ( pTMilitiaSoldier->usNextActionData != NOWHERE )
//				{
//					pTMilitiaSoldier->bNextAction = AI_ACTION_RUN_AWAY;
//					pTMilitiaSoldier->usActionData = ANIM_STAND;
//					//return( AI_ACTION_STOP_COWERING );
//				}
//
//				// stop showing menu
//				fShowMilitiaControlMenu = FALSE;
//				//fShowTalkToAllMenu = FALSE;
//				giAssignHighLine = -1;
//			}
//			break;
//
//
//			case( TTALL_MENU_COMETOME ):
//			{
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Come to me" );
//
//				UINT8						cnt;
//				SOLDIERTYPE *		pTeamSoldier;
//				INT16 sActionGridNo, sGridNo, sAdjustedGridNo;
//				UINT8	ubDirection;
//
//				cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID;
//
//				for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++, pTeamSoldier++)
//				{
//					if ( pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bLife > 0 )
//					{
//
//						if ( GetSoldier( &pSoldier, gusSelectedSoldier )  )
//						{
//							// OK, find an adjacent gridno....
//							sGridNo = pSoldier->sGridNo;
//
//							// See if we can get there to punch	
//							sActionGridNo =  FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
//							if ( sActionGridNo != -1 )
//							{
//								// SEND PENDING ACTION
//								//pTMilitiaSoldier->ubPendingAction = MERC_STEAL;
//								pTeamSoldier->sPendingActionData2  = pSoldier->sGridNo;
//								pTeamSoldier->bPendingActionData3  = ubDirection;
//								pTeamSoldier->ubPendingActionAnimCount = 0;
//
//								// CHECK IF WE ARE AT THIS GRIDNO NOW
//								if ( pTeamSoldier->sGridNo != sActionGridNo )
//								{
//									// WALK UP TO DEST FIRST
//									SendGetNewSoldierPathEvent( pTeamSoldier, sActionGridNo, pTeamSoldier->usUIMovementMode );
//								}
//							}
//						}
//					}
//				}
//
//				// stop showing menu
//				fShowMilitiaControlMenu = FALSE;
//				//fShowTalkToAllMenu = FALSE;
//				giAssignHighLine = -1;
//			}
//			break;
//
//			case( TTALL_MENU_GETDOWN ):
//			{
//				// Get Down !!!
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Get Down" );
//
//				SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_PRONE );
//				//SendChangeSoldierStanceEvent( pTMilitiaSoldier, ANIM_CROUCH );
//
//				// stop showing menu
//				fShowMilitiaControlMenu = FALSE;
//				//fShowTalkToAllMenu = FALSE;
//				giAssignHighLine = -1;
//			}
//			break;
//
//			case( TTALL_MENU_CANCEL ):
//			{
//				//Cancel
//				ScreenMsg( FONT_WHITE, MSG_INTERFACE, L"Cancel" );
//
//				// stop showing menu
//				fShowTalkToAllMenu = FALSE;
//
//				// unhighlight the assignment box
//				UnHighLightBox( ghMilitiaControlBox );
//
//				// reset list of characters
//				ResetSelectedListForMapScreen( );
//				gfRenderPBInterface = TRUE;
//			}
//			break;
//		}
//		// set dirty flag
//		fTeamPanelDirty = TRUE;
//		fMapScreenBottomDirty = TRUE;
//	}
//}
//
//












//void HandleShadingOfLinesForTalkToAllMenu( void )
//{
//	SOLDIERTYPE *pSoldier = NULL;
//	INT32 iCounter = 0;
//
//
//	// check if valid
//	if( ( fShowTalkToAllMenu == FALSE ) || ( ghTalkToAllBox == - 1 ) )
//	{
//		return;
//	}
//
//	//pSoldier = GetSelectedAssignSoldier( FALSE );
//
//	//// can character practise?
//	//if( CanCharacterPractise( pSoldier ) == FALSE )
//	//{
//	//	ShadeStringInBox( ghTalkToAllBox, TalkToAll_MENU_SELF );
//	//}
//	//else
//	//{
//	//	UnShadeStringInBox( ghTalkToAllBox, TalkToAll_MENU_SELF );
//	//}
//
//
//	return;
//}


