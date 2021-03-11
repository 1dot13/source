#include "builddefines.h"

// WANNE 2 <changed some lines>

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
	#include "PreBattle Interface.h"
	#include "vehicles.h"
#else
	//sgp
	#include "english.h"
	#include "debug.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "line.h"

	//ja2
	#include "Tactical Placement GUI.h"
	#include "Overhead Map.h"
	#include "strategicmap.h"
	#include "Interface.h"
	#include "overhead.h"
	#include "Render Dirty.h"
	#include "sysutil.h"
	#include "PreBattle Interface.h"
	#include "Soldier Profile.h"
	#include "Map Edgepoints.h"
	#include "gameloop.h"
	#include "message.h"
	#include "Map Information.h"
	#include "Soldier Add.h"
	#include "cursors.h"
	#include "Cursor Control.h"
	#include "MessageBoxScreen.h"
	#include "text.h"
	#include "WordWrap.h"
	#include "Game Clock.h"
	#include "Isometric Utils.h"
#endif
#include "connect.h"
#include "renderworld.h"//dnl ch45 051009
#include "merc entering.h"
#include "CampaignStats.h"		// added by Flugente

typedef struct MERCPLACEMENT
{
	SOLDIERTYPE		*pSoldier;
	UINT32				uiVObjectID;
	MOUSE_REGION	region;
	UINT8					ubStrategicInsertionCode;
	BOOLEAN				fPlaced;
}MERCPLACEMENT;

MERCPLACEMENT *gMercPlacement = NULL;

enum
{
	DONE_BUTTON,
	SPREAD_BUTTON,
	GROUP_BUTTON,
	CLEAR_BUTTON,
	NUM_TP_BUTTONS
};
UINT32 iTPButtons[ NUM_TP_BUTTONS ];

//dnl ch45 051009
#define PLACEMENT_OFFSET 150
extern INT32 giXA, giYA;

extern BOOLEAN gfOverheadMapDirty;
extern BOOLEAN GetOverheadMouseGridNo( INT32 *psGridNo );

extern UINT16 iOffsetHorizontal;
extern UINT16 iOffsetVertical;
int islocked;

UINT8	gubDefaultButton = CLEAR_BUTTON;
BOOLEAN gfTacticalPlacementGUIActive = FALSE;
BOOLEAN gfTacticalPlacementFirstTime = FALSE;
BOOLEAN gfEnterTacticalPlacementGUI = FALSE;
BOOLEAN gfKillTacticalGUI = FALSE;
INT32 giOverheadPanelImage = 0;
INT32 giOverheadButtonImages[ NUM_TP_BUTTONS ];
INT32 giMercPanelImage = 0;
INT32 giPlacements = 0;
BOOLEAN gfTacticalPlacementGUIDirty = FALSE;
BOOLEAN gfValidLocationsChanged = FALSE;
SGPRect gTPClipRect = {0,0,0,0};

// WANNE - MP: Center
SGPRect gTPClipRectCenterLeft = {0,0,0,0};
SGPRect gTPClipRectCenterTop = {0,0,0,0};
SGPRect gTPClipRectCenterRight = {0,0,0,0};
SGPRect gTPClipRectCenterBottom = {0,0,0,0};

BOOLEAN gfValidCursor = FALSE;
BOOLEAN gfEveryonePlaced = FALSE;

UINT8	gubSelectedGroupID = 0;
UINT8	gubHilightedGroupID = 0;
UINT8 gubCursorGroupID = 0;
INT8	gbSelectedMercID = -1;
INT8	gbHilightedMercID = -1;
INT8	gbCursorMercID = -1;
SOLDIERTYPE *gpTacticalPlacementSelectedSoldier = NULL;
SOLDIERTYPE *gpTacticalPlacementHilightedSoldier = NULL;

// WANNE - MP: Center
BOOLEAN gfCenter;

extern BOOLEAN GetOverheadScreenXYFromGridNo( INT32 sGridNo, INT16* psScreenX, INT16* psScreenY );

void DoneOverheadPlacementClickCallback( GUI_BUTTON *btn, INT32 reason );
void SpreadPlacementsCallback ( GUI_BUTTON *btn, INT32 reason );
void GroupPlacementsCallback( GUI_BUTTON *btn, INT32 reason );
void ClearPlacementsCallback( GUI_BUTTON *btn, INT32 reason );
void MercMoveCallback( MOUSE_REGION *reg, INT32 reason );
void MercClickCallback( MOUSE_REGION *reg, INT32 reason );
void PlaceMercs();
void FastHelpRemovedCallback();
void FastHelpRemoved2Callback();
void DialogRemoved( UINT8 ubResult );

void PutDownMercPiece( INT32 iPlacement );
void PickUpMercPiece( INT32 iPlacement );
void SetCursorMerc( INT8 bPlacementID );
void SelectNextUnplacedUnit();

// WANNE: Made methods available for Release build, because we need them in multiplayer
//#ifdef JA2BETAVERSION

BOOLEAN gfNorthValid, gfEastValid, gfSouthValid, gfWestValid;
BOOLEAN gfChangedEntrySide = FALSE;

void FindValidInsertionCode( UINT8 *pubStrategicInsertionCode )
{
	if( gMapInformation.sNorthGridNo == -1 &&
			gMapInformation.sEastGridNo == -1 &&
			gMapInformation.sSouthGridNo == -1 &&
			gMapInformation.sWestGridNo == -1 )
	{
		AssertMsg( 0, "Map has no entry points at all.	Can't generate edge points.	LC:1" );
	}
	if( gMapInformation.sNorthGridNo	!= -1 && !gps1stNorthEdgepointArray	||
			gMapInformation.sEastGridNo		!= -1 && !gps1stEastEdgepointArray		||
			gMapInformation.sSouthGridNo	!= -1 && !gps1stSouthEdgepointArray	||
			gMapInformation.sWestGridNo		!= -1 && !gps1stWestEdgepointArray		)
	{
		InvalidateScreen();
		DrawTextToScreen( L"Map doesn't has entrypoints without corresponding edgepoints. LC:1",
			iOffsetHorizontal + 30, iOffsetVertical + 150, 600, FONT10ARIALBOLD, FONT_RED, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED	);
		DrawTextToScreen( L"GENERATING MAP EDGEPOINTS!	Please wait...",
			iOffsetHorizontal + 30, iOffsetVertical + 160, 600, FONT10ARIALBOLD, FONT_YELLOW, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED	);

		RefreshScreen( NULL );
		GenerateMapEdgepoints(TRUE);//dnl ch43 290909
		switch( *pubStrategicInsertionCode )
		{
			case INSERTION_CODE_NORTH:
				if( !gps1stNorthEdgepointArray )
					AssertMsg( 0, "Map Edgepoint generation failed.	KM : 0 -- send map" );
				break;
			case INSERTION_CODE_EAST:
				if( !gps1stEastEdgepointArray )
					AssertMsg( 0, "Map Edgepoint generation failed.	KM : 0 -- send map" );
				break;
			case INSERTION_CODE_SOUTH:
				if( !gps1stSouthEdgepointArray )
					AssertMsg( 0, "Map Edgepoint generation failed.	KM : 0 -- send map" );
				break;
			case INSERTION_CODE_WEST:
				if( !gps1stWestEdgepointArray )
					AssertMsg( 0, "Map Edgepoint generation failed.	KM : 0 -- send map" );
				break;
		}
		return;
	}
	if( gMapInformation.sNorthGridNo != -1 )
	{
		*pubStrategicInsertionCode = INSERTION_CODE_NORTH;
		gfChangedEntrySide = TRUE;
	}
	else if( gMapInformation.sEastGridNo != -1 )
	{
		*pubStrategicInsertionCode = INSERTION_CODE_EAST;
		gfChangedEntrySide = TRUE;
	}
	else if( gMapInformation.sSouthGridNo != -1 )
	{
		*pubStrategicInsertionCode = INSERTION_CODE_SOUTH;
		gfChangedEntrySide = TRUE;
	}
	else if( gMapInformation.sWestGridNo != -1 )
	{
		*pubStrategicInsertionCode = INSERTION_CODE_WEST;
		gfChangedEntrySide = TRUE;
	}
	else
	{
		AssertMsg( 0, "No maps edgepoints at all! KM, LC : 1" );
	}
}

void CheckForValidMapEdge( UINT8 *pubStrategicInsertionCode )
{
	switch( *pubStrategicInsertionCode )
	{
		case INSERTION_CODE_NORTH:
			if( !gps1stNorthEdgepointArray )
				FindValidInsertionCode( pubStrategicInsertionCode );
			break;
		case INSERTION_CODE_EAST:
			if( !gps1stEastEdgepointArray )
				FindValidInsertionCode( pubStrategicInsertionCode );
			break;
		case INSERTION_CODE_SOUTH:
			if( !gps1stSouthEdgepointArray )
				FindValidInsertionCode( pubStrategicInsertionCode );
			break;
		case INSERTION_CODE_WEST:
			if( !gps1stWestEdgepointArray )
				FindValidInsertionCode( pubStrategicInsertionCode );
			break;
	}
}

//#endif

extern BOOLEAN		gfTacticalDoHeliRun;

void InitTacticalPlacementGUI()
{
	islocked=0;//hayden
	VOBJECT_DESC VObjectDesc;
	INT32 i, xp, yp;
	UINT8 ubFaceIndex;
	gfTacticalPlacementGUIActive = TRUE;
	gfTacticalPlacementGUIDirty = TRUE;
	gfValidLocationsChanged = TRUE;
	gfTacticalPlacementFirstTime = TRUE;

	// WANNE - MP: Center
	gfCenter = FALSE;
	#ifdef JA2BETAVERSION
		gfNorthValid = gfEastValid = gfSouthValid = gfWestValid = FALSE;
		gfChangedEntrySide = FALSE;
	#endif

	// WANNE: Make a black background color for the whole screen
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

	//Enter overhead map
	GoIntoOverheadMap();

	//Load the images
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		sprintf( VObjectDesc.ImageFile, "Interface\\OverheadInterface.sti" );
		if( !AddVideoObject( &VObjectDesc, (UINT32 *)&giOverheadPanelImage ) )
		{
			AssertMsg( 0, "Failed to load Interface\\OverheadInterface.sti" );
		}
	}
	else if (iResolution < _1024x768)
	{
		sprintf( VObjectDesc.ImageFile, "Interface\\OverheadInterface_800x600.sti" );
		if( !AddVideoObject( &VObjectDesc, (UINT32 *)&giOverheadPanelImage ) )
		{
			AssertMsg( 0, "Failed to load Interface\\OverheadInterface_800x600.sti" );
		}
	}
	else
	{
		sprintf( VObjectDesc.ImageFile, "Interface\\OverheadInterface_1024x768.sti" );
		if( !AddVideoObject( &VObjectDesc, (UINT32 *)&giOverheadPanelImage ) )
		{
			AssertMsg( 0, "Failed to load Interface\\OverheadInterface_1024x768.sti" );
		}
	}

	sprintf( VObjectDesc.ImageFile, "Interface\\panels.sti" );
	if( !AddVideoObject( &VObjectDesc, (UINT32 *)&giMercPanelImage ) )
	{
		AssertMsg( 0, "Failed to load Interface\\panels.sti" );
	}

	giOverheadButtonImages[ DONE_BUTTON ] = LoadButtonImage( "Interface\\OverheadUIButtons.sti", -1, 0, -1, 1, -1 );
	if( giOverheadButtonImages[ DONE_BUTTON ] == -1 )
	{
		AssertMsg( 0, "Failed to load Interface\\OverheadUIButtons.sti" );
	}
	giOverheadButtonImages[ SPREAD_BUTTON ]		= UseLoadedButtonImage( giOverheadButtonImages[ DONE_BUTTON ], -1, 0, -1, 1, -1 );
	giOverheadButtonImages[ GROUP_BUTTON ]		= UseLoadedButtonImage( giOverheadButtonImages[ DONE_BUTTON ], -1, 0, -1, 1, -1 );
	giOverheadButtonImages[ CLEAR_BUTTON ]		= UseLoadedButtonImage( giOverheadButtonImages[ DONE_BUTTON ], -1, 0, -1, 1, -1 );

	//Create the buttons which provide automatic placements.
	iTPButtons[ CLEAR_BUTTON ] =
		QuickCreateButton( giOverheadButtonImages[ CLEAR_BUTTON ], 11 + xResOffset, SCREEN_HEIGHT - 148, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, ClearPlacementsCallback );
	SpecifyGeneralButtonTextAttributes( iTPButtons[ CLEAR_BUTTON ], gpStrategicString[ STR_TP_CLEAR ], BLOCKFONT, FONT_BEIGE, 141 );
	SetButtonFastHelpText( iTPButtons[ CLEAR_BUTTON ], gpStrategicString[ STR_TP_CLEARHELP ] );
	SetBtnHelpEndCallback( iTPButtons[ CLEAR_BUTTON ], FastHelpRemoved2Callback );
	iTPButtons[ SPREAD_BUTTON ] =
		QuickCreateButton( giOverheadButtonImages[ SPREAD_BUTTON ], 11 + xResOffset, SCREEN_HEIGHT - 113, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, SpreadPlacementsCallback );
	SpecifyGeneralButtonTextAttributes( iTPButtons[ SPREAD_BUTTON ], gpStrategicString[ STR_TP_SPREAD ], BLOCKFONT, FONT_BEIGE, 141 );
	SetButtonFastHelpText( iTPButtons[ SPREAD_BUTTON ], gpStrategicString[ STR_TP_SPREADHELP ] );
	SetBtnHelpEndCallback( iTPButtons[ SPREAD_BUTTON ], FastHelpRemovedCallback );
	iTPButtons[ GROUP_BUTTON ] =
		QuickCreateButton( giOverheadButtonImages[ GROUP_BUTTON ], 11 + xResOffset, SCREEN_HEIGHT - 78, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, GroupPlacementsCallback );
	SpecifyGeneralButtonTextAttributes( iTPButtons[ GROUP_BUTTON ], gpStrategicString[ STR_TP_GROUP ], BLOCKFONT, FONT_BEIGE, 141 );
	SetButtonFastHelpText( iTPButtons[ GROUP_BUTTON ], gpStrategicString[ STR_TP_GROUPHELP ] );
	SetBtnHelpEndCallback( iTPButtons[ GROUP_BUTTON ], FastHelpRemovedCallback );
	iTPButtons[ DONE_BUTTON ] =
		QuickCreateButton( giOverheadButtonImages[ DONE_BUTTON ], 11 + xResOffset, SCREEN_HEIGHT - 43, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, DoneOverheadPlacementClickCallback );
	SpecifyGeneralButtonTextAttributes( iTPButtons[ DONE_BUTTON ], gpStrategicString[ STR_TP_DONE ], BLOCKFONT, FONT_BEIGE, 141 );
	SetButtonFastHelpText( iTPButtons[ DONE_BUTTON ], gpStrategicString[ STR_TP_DONEHELP ] );
	SetBtnHelpEndCallback( iTPButtons[ DONE_BUTTON ], FastHelpRemovedCallback );
	AllowDisabledButtonFastHelp( iTPButtons[ DONE_BUTTON ], TRUE );

	SpecifyButtonHilitedTextColors( iTPButtons[ CLEAR_BUTTON ], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonHilitedTextColors( iTPButtons[ SPREAD_BUTTON ], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonHilitedTextColors( iTPButtons[ GROUP_BUTTON ], FONT_WHITE, FONT_NEARBLACK );
	SpecifyButtonHilitedTextColors( iTPButtons[ DONE_BUTTON ], FONT_WHITE, FONT_NEARBLACK );

	//First pass:	Count the number of mercs that are going to be placed by the player.
	//			 This determines the size of the array we will allocate.
	giPlacements = 0;
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++i )
	{

		if( MercPtrs[ i ]->bActive && !MercPtrs[ i ]->flags.fBetweenSectors &&
			CurrentBattleSectorIs( MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY, MercPtrs[i]->bSectorZ ) &&
				!( MercPtrs[ i ]->flags.uiStatusFlags & ( SOLDIER_VEHICLE ) ) && // ATE Ignore vehicles
				MercPtrs[ i ]->bAssignment != ASSIGNMENT_POW &&
				!( MercPtrs[i]->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION ) &&
				MercPtrs[ i ]->bAssignment != IN_TRANSIT )
		{
			++giPlacements;
		}
	}
	//Allocate the array based on how many mercs there are.
	gMercPlacement = (MERCPLACEMENT*)MemAlloc( sizeof( MERCPLACEMENT ) * giPlacements );
	Assert( gMercPlacement );
	//Second pass:	Assign the mercs to their respective slots.
	giPlacements = 0;
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++i )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !MercPtrs[ i ]->flags.fBetweenSectors &&
			CurrentBattleSectorIs( MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY, MercPtrs[i]->bSectorZ ) &&
				MercPtrs[ i ]->bAssignment != ASSIGNMENT_POW &&
				!( MercPtrs[i]->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION ) &&
				MercPtrs[ i ]->bAssignment != IN_TRANSIT &&
				!( MercPtrs[ i ]->flags.uiStatusFlags & ( SOLDIER_VEHICLE ) ) ) // ATE Ignore vehicles
		{
			// Flugente: if options allow it and we entered this sector - in combat - via helicopter, then allow us free selection of our entry point, and drop us from the helicopter
			if ( MercPtrs[ i ]->bTeam == gbPlayerNum && gGameExternalOptions.ubSkyriderHotLZ == 3 && MercPtrs[ i ]->usSoldierFlagMask & SOLDIER_AIRDROP )
			{
				AddMercToHeli( MercPtrs[ i ]->ubID );

				gMercPlacement[ giPlacements ].ubStrategicInsertionCode = INSERTION_CODE_CHOPPER;
				MercPtrs[ i ]->ubStrategicInsertionCode					= INSERTION_CODE_CHOPPER;
				gfCenter = TRUE;
			}

			if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
			{
				gMercPlacement[giPlacements].ubStrategicInsertionCode	= INSERTION_CODE_CENTER;
				MercPtrs[i]->ubStrategicInsertionCode					= INSERTION_CODE_CENTER;
				gfCenter = TRUE;
			}

			// WANNE - MP: Check if the desired insertion direction is valid on the map. If not, choose another entry direction!
			if (is_networked)
			{
				MercPtrs[ i ]->ubStrategicInsertionCode = GetValidInsertionDirectionForMP(MercPtrs[ i ]->ubStrategicInsertionCode);
			}
			// ATE: If we are in a vehicle - remove ourselves from it!
			//if ( MercPtrs[ i ]->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			//{
			//	RemoveSoldierFromVehicle( MercPtrs[ i ], MercPtrs[ i ]->bVehicleID );
			//}

			if( MercPtrs[ i ]->ubStrategicInsertionCode == INSERTION_CODE_PRIMARY_EDGEINDEX ||
					MercPtrs[ i ]->ubStrategicInsertionCode == INSERTION_CODE_SECONDARY_EDGEINDEX )
			{
				MercPtrs[ i ]->ubStrategicInsertionCode = (UINT8)MercPtrs[ i ]->usStrategicInsertionData;
			}
			gMercPlacement[ giPlacements ].pSoldier = MercPtrs[ i ];
			gMercPlacement[ giPlacements ].ubStrategicInsertionCode = MercPtrs[ i ]->ubStrategicInsertionCode;
			gMercPlacement[ giPlacements ].fPlaced = FALSE;
			
			// WANNE: We always want to have edgepoints
			CheckForValidMapEdge( &MercPtrs[ i ]->ubStrategicInsertionCode );
			
			// Flugente: campaign stats
			switch( MercPtrs[ i ]->ubStrategicInsertionCode )
			{
				case INSERTION_CODE_NORTH:					
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_NORTH;
					break;
				case INSERTION_CODE_EAST:
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_EAST;
					break;
				case INSERTION_CODE_SOUTH:
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_SOUTH;
					break;
				case INSERTION_CODE_WEST:
					gCurrentIncident.usIncidentFlags |= INCIDENT_ATTACKDIR_WEST;
					break;
			}

			// WANNE - MP: Center
			if (is_networked && MercPtrs[ i ]->ubStrategicInsertionCode == INSERTION_CODE_CENTER)
			{
				gfCenter = TRUE;
			}	

			++giPlacements;
		}
	}

	//add all the faces now
	for( i = 0; i < giPlacements; ++i )
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

		//Load the faces
		{
			ubFaceIndex = gMercProfiles[ gMercPlacement[ i ].pSoldier->ubProfile ].ubFaceIndex;
			
		if ( ( ubFaceIndex < 100 ) && gMercProfiles[gMercPlacement[i].pSoldier->ubProfile].Type == PROFILETYPE_IMP )
		{
			sprintf( VObjectDesc.ImageFile, "IMPFaces\\65Face\\%02d.sti", ubFaceIndex );
		} 
		else if ( ( ubFaceIndex > 99 ) && gMercProfiles[gMercPlacement[i].pSoldier->ubProfile].Type == PROFILETYPE_IMP )
		{
			sprintf( VObjectDesc.ImageFile, "IMPFaces\\65Face\\%03d.sti", ubFaceIndex );
		}
		else if( ubFaceIndex < 100 )
		{	
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\%02d.sti", ubFaceIndex );
		}
		else if( ubFaceIndex > 99 )
		{
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\%03d.sti", ubFaceIndex );
		}
		}

		if( !AddVideoObject( &VObjectDesc, &gMercPlacement[ i ].uiVObjectID ) )
		{
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\speck.sti" );
			if( !AddVideoObject( &VObjectDesc, &gMercPlacement[ i ].uiVObjectID ) )
			{
				AssertMsg( 0, String("Failed to load %Faces\\65Face\\%03d.sti or it's placeholder, speck.sti", gMercProfiles[ gMercPlacement[ i ].pSoldier->ubProfile ].ubFaceIndex) );
			}
		}

		xp = (xResOffset) + 91 + (i / 2) * 54;

		if (i % 2)
		{
			yp = SCREEN_HEIGHT - 68;
		}
		else
		{
			yp = SCREEN_HEIGHT - 119;
		}

		//yp = (i % 2) ? 412 : 361;
		MSYS_DefineRegion( &gMercPlacement[ i ].region, (UINT16)xp, (UINT16)yp, (UINT16)(xp + 54), (UINT16)(yp + 62), MSYS_PRIORITY_HIGH, 0, MercMoveCallback, MercClickCallback );
	}

	if(!is_client)
		PlaceMercs();//hayden

	if( gubDefaultButton == GROUP_BUTTON )
	{
		ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;

		for( i = 0; i < giPlacements; ++i )
		{ //go from the currently selected soldier to the end
			if( !gMercPlacement[ i ].fPlaced )
			{ //Found an unplaced merc.	Select him.
				gbSelectedMercID = (INT8)i;
				if( gubDefaultButton == GROUP_BUTTON )
					gubSelectedGroupID = gMercPlacement[ i ].pSoldier->ubGroupID;
				gfTacticalPlacementGUIDirty = TRUE;
				SetCursorMerc( (INT8)i );
				gpTacticalPlacementSelectedSoldier = gMercPlacement[ i ].pSoldier;
				break;
			}
		}
	}
}

// WANNE - MP: This method checks, if the desired entry direction (N, E, S, W) on the map is valid. If not it chooses the next valid diretion
UINT8 GetValidInsertionDirectionForMP(UINT8	currentInsertionPoint)
{
	bool foundValidDirection = false;
	UINT8 validInsertionDirection = currentInsertionPoint;

	// Check if current insertion direction is valid
	switch (currentInsertionPoint)
	{
		case INSERTION_CODE_NORTH:
			if (gus1stNorthEdgepointArraySize > 0 || gus2ndNorthEdgepointArraySize > 0)
			{
				foundValidDirection = true;
				validInsertionDirection = INSERTION_CODE_NORTH;
			}
			break;
		case INSERTION_CODE_SOUTH:
			if (gus1stSouthEdgepointArraySize > 0 || gus2ndSouthEdgepointArraySize > 0)
			{
				foundValidDirection = true;
				validInsertionDirection = INSERTION_CODE_SOUTH;
			}
			break;
		case INSERTION_CODE_EAST:
			if (gus1stEastEdgepointArraySize > 0 || gus2ndEastEdgepointArraySize > 0)
			{
				foundValidDirection = true;
				validInsertionDirection = INSERTION_CODE_EAST;
			}
			break;
		case INSERTION_CODE_WEST:
			if (gus1stWestEdgepointArraySize > 0 || gus2ndWestEdgepointArraySize > 0)
			{
				foundValidDirection = true;
				validInsertionDirection = INSERTION_CODE_WEST;
			}
			break;
		// WANNE - MP: Center
		case INSERTION_CODE_CENTER:
			foundValidDirection = true;
			validInsertionDirection = INSERTION_CODE_CENTER;
			break;

		case INSERTION_CODE_CHOPPER:
			foundValidDirection = true;
			validInsertionDirection = INSERTION_CODE_CHOPPER;
			break;
	}		

	// Find alternate insertion direction by looping through all directions (N, S, E, W)
	if (!foundValidDirection)
	{
		UINT8 direction = 0;

		// Find NEXT valid direction
		for (int i = currentInsertionPoint; (i < currentInsertionPoint + 4); i++)
		{
			// First iteration, start with current insertion direction
			if (i == currentInsertionPoint)
				direction = i;
			else
			{
				if (i <= 3)
					direction = i;
				else
				{
					direction = 4 - i;
				}
			}

			switch (direction)
			{
				case INSERTION_CODE_NORTH:
					if (gus1stNorthEdgepointArraySize > 0 || gus2ndNorthEdgepointArraySize > 0)
					{
						foundValidDirection = true;
						validInsertionDirection = INSERTION_CODE_NORTH;
					}
					break;
				case INSERTION_CODE_SOUTH:
					if (gus1stSouthEdgepointArraySize > 0 || gus2ndSouthEdgepointArraySize > 0)
					{
						foundValidDirection = true;
						validInsertionDirection = INSERTION_CODE_SOUTH;
					}
					break;
				case INSERTION_CODE_EAST:
					if (gus1stEastEdgepointArraySize > 0 || gus2ndEastEdgepointArraySize > 0)
					{
						foundValidDirection = true;
						validInsertionDirection = INSERTION_CODE_EAST;
					}
					break;
				case INSERTION_CODE_WEST:
					if (gus1stWestEdgepointArraySize > 0 || gus2ndWestEdgepointArraySize > 0)
					{
						foundValidDirection = true;
						validInsertionDirection = INSERTION_CODE_WEST;
					}
					break;
			}

			// Exit loop condition
			if (foundValidDirection)
				break;
		}
	}

	return validInsertionDirection;
}

void RenderTacticalPlacementGUI()
{
	INT32 i, xp, yp, width, height;
	INT32 iStartY;
	SOLDIERTYPE *pSoldier;
	UINT32 uiDestPitchBYTES;
	UINT16 usHatchColor;
	CHAR16 str[ 128 ];
	UINT8 *pDestBuf;
	UINT8 ubColor;	

	if( gfTacticalPlacementFirstTime )
	{
		gfTacticalPlacementFirstTime = FALSE;
		DisableScrollMessages();
	}

	//Check to make sure that if we have a hilighted merc (not selected) and the mouse has moved out
	//of it's region, then we will clear the hilighted ID, and refresh the display.
	if( !gfTacticalPlacementGUIDirty && gbHilightedMercID != -1 )
	{
		xp = xResOffset + 91 + (gbHilightedMercID / 2) * 54;

		if (gbHilightedMercID % 2)
		{
			yp = SCREEN_HEIGHT - 68;
		}
		else
		{
			yp = SCREEN_HEIGHT - 119;
		}

		//yp = (gbHilightedMercID % 2) ? 412 : 361;

		if( gusMouseXPos < xp || gusMouseXPos > xp + 54 || gusMouseYPos < yp || gusMouseYPos > yp + 62 )
		{
			gbHilightedMercID = -1;
			gubHilightedGroupID = 0;
			SetCursorMerc( gbSelectedMercID );
			gpTacticalPlacementHilightedSoldier = NULL;
		}
	}

	//If the display is dirty render the entire panel.
	if( gfTacticalPlacementGUIDirty )
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, giOverheadPanelImage, 0, xResOffset, SCREEN_HEIGHT - 160, VO_BLT_SRCTRANSPARENCY, 0 );
		InvalidateRegion( 0 + xResOffset, SCREEN_HEIGHT - 160, 320 + xResOffset, SCREEN_HEIGHT );
		gfTacticalPlacementGUIDirty = FALSE;
		MarkButtonsDirty();
		//DisableHilightsAndHelpText();
		//RenderButtons();
		//EnableHilightsAndHelpText();
		for( i = 0; i < giPlacements; ++i )
		{
			//Render the mercs
			pSoldier = gMercPlacement[ i ].pSoldier;
			
			xp = xResOffset + 95 + (i / 2) * 54;

			if (i % 2)
			{
				yp = SCREEN_HEIGHT - 58;
			}
			else
			{
				yp = SCREEN_HEIGHT - 109;
			}

			//yp = (i % 2) ? 422 : 371;

			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+36, yp+2, xp+44,	yp+30, 0 );
			BltVideoObjectFromIndex( FRAME_BUFFER, giMercPanelImage, 0, xp, yp, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( FRAME_BUFFER, gMercPlacement[ i ].uiVObjectID, 0, xp+2, yp+2, VO_BLT_SRCTRANSPARENCY, NULL );
			//HEALTH BAR

			if( !pSoldier->stats.bLife )
				continue;

			//yellow one for bleeding
			iStartY = yp + 29 - 27*pSoldier->stats.bLifeMax/100;
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+36, iStartY, xp+37, yp+29, Get16BPPColor( FROMRGB( 107, 107, 57 ) ) );
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+37, iStartY, xp+38, yp+29, Get16BPPColor( FROMRGB( 222, 181, 115 ) ) );
						
			//pink one for bandaged.
			iStartY = yp + 29 - 27*(pSoldier->stats.bLifeMax - pSoldier->bBleeding)/100;
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+36, iStartY, xp+37, yp+29, Get16BPPColor( FROMRGB( 156, 57, 57 ) ) );
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+37, iStartY, xp+38, yp+29, Get16BPPColor( FROMRGB( 222, 132, 132 ) ) );
						
			//red one for actual health
			iStartY = yp + 29 - 27*pSoldier->stats.bLife/100;
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+36, iStartY, xp+37, yp+29, Get16BPPColor( FROMRGB( 107, 8, 8 ) ) );
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+37, iStartY, xp+38, yp+29, Get16BPPColor( FROMRGB( 206, 0, 0 ) ) );
						
			//BREATH BAR
			iStartY = yp + 29 - 27*pSoldier->bBreathMax/100;
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+39, iStartY, xp+40, yp+29, Get16BPPColor( FROMRGB( 8, 8, 132 ) ) );
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+40, iStartY, xp+41, yp+29, Get16BPPColor( FROMRGB( 8, 8, 107 ) ) );

			//MORALE BAR
			iStartY = yp + 29 - 27*pSoldier->aiData.bMorale/100;
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+42, iStartY, xp+43, yp+29, Get16BPPColor( FROMRGB( 8, 156, 8 ) ) );
			ColorFillVideoSurfaceArea( FRAME_BUFFER, xp+43, iStartY, xp+44, yp+29, Get16BPPColor( FROMRGB( 8, 107, 8 ) ) );
		}

		SetFont( BLOCKFONT );
		SetFontForeground( FONT_BEIGE );
		SetFontShadow( 141 );

		GetSectorIDString( gubPBSectorX, gubPBSectorY, gubPBSectorZ, str, TRUE );

		mprintf(120 + xResOffset, SCREEN_HEIGHT - 145, L"%s %s -- %s...", gpStrategicString[ STR_TP_SECTOR ], str, gpStrategicString[ STR_TP_CHOOSEENTRYPOSITIONS ] );

		//Shade out the part of the tactical map that isn't considered placable.
		BlitBufferToBuffer(FRAME_BUFFER, guiSAVEBUFFER, 0, SCREEN_HEIGHT - 160, SCREEN_WIDTH, 160);
	}

	if( gfValidLocationsChanged )
	{
		if( DayTime() )
		{ //6AM to 9PM is black
			usHatchColor = 0; //Black
		}
		else
		{ //9PM to 6AM is gray (black is too dark to distinguish)
			usHatchColor = Get16BPPColor( FROMRGB( 63, 31, 31 ) );
		}

		--gfValidLocationsChanged;

		//DBrot bigger map code
		if(gfUseBiggerOverview)
		{
			BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, iOffsetHorizontal, iOffsetVertical, 1434, 716 );
			InvalidateRegion( iOffsetHorizontal, iOffsetVertical, iOffsetHorizontal + 1434, iOffsetVertical + 716 );
			gTPClipRect.iLeft = iOffsetHorizontal + 1;
			gTPClipRect.iTop = iOffsetVertical + 1;
			gTPClipRect.iBottom = iOffsetVertical + 716;// 318;
			gTPClipRect.iRight = iOffsetHorizontal + 1434; //634;
		}
		else
		{
			BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, iOffsetHorizontal, iOffsetVertical, 640, 320 );
			InvalidateRegion( iOffsetHorizontal, iOffsetVertical, iOffsetHorizontal + 640, iOffsetVertical + 320 );
			//dnl ch45 051009
			gTPClipRect.iLeft = iOffsetHorizontal + 1;
			gTPClipRect.iTop = iOffsetVertical + 1;
			gTPClipRect.iBottom = iOffsetVertical + 318;
			gTPClipRect.iRight = iOffsetHorizontal + 634;
		}
	
		if( gbCursorMercID == -1 )
		{
			if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
			{
				INT16 sCellX = 0;
				INT16 sCellY = 0;

				GetOverheadScreenXYFromGridNo( gMapInformation.sCenterGridNo, &sCellX, &sCellY );

				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + sCellX - 5 * gGameExternalOptions.usAmbushSpreadRadiusMercs;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + sCellY - 3 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + sCellX + 5 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + sCellY + 3 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
			// WANNE - MP: Center
			else if (is_networked && gfCenter)
			{
				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + CENTERENTRYPTS_LEFT_X;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + CENTERENTRYPTS_TOP_Y;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + CENTERENTRYPTS_RIGHT_X;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + CENTERENTRYPTS_BOTTOM_Y;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
			// Flugente airdrop
			else if ( gfCenter && gGameExternalOptions.ubSkyriderHotLZ == 3 )
			{
				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + AIRDROPENTRYPTS_LEFT_X;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + AIRDROPENTRYPTS_TOP_Y;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + AIRDROPENTRYPTS_RIGHT_X;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + AIRDROPENTRYPTS_BOTTOM_Y;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
		}
		else
		{
			if (is_networked)
			{
				gMercPlacement[ gbCursorMercID ].ubStrategicInsertionCode = GetValidInsertionDirectionForMP(gMercPlacement[ gbCursorMercID ].ubStrategicInsertionCode);
			}

			//dnl ch45 051009
			INT16 sWorldScreenX, sX;
			INT16 sWorldScreenY, sY;
			sX = giXA;
			sY = giYA;
			GetWorldXYAbsoluteScreenXY(sX, sY, &sWorldScreenX, &sWorldScreenY);
			sWorldScreenX += 20;// Correction from invisible area X
			sWorldScreenY += 35;// Correction from invisible area Y

			//DBrot: adjust offsets for big maps
			if(gfUseBiggerOverview)
			{
				switch(gMercPlacement[gbCursorMercID].ubStrategicInsertionCode)
				{
				case INSERTION_CODE_NORTH:
					//if(sWorldScreenY <= PLACEMENT_OFFSET){
						//sY = (PLACEMENT_OFFSET - sWorldScreenY) / 5;
						//gTPClipRect.iTop += sY;
						gTPClipRect.iTop += PLACEMENT_OFFSET/5;
					//}
					break;
				case INSERTION_CODE_EAST:
					//if((sWorldScreenX + NORMAL_MAP_SCREEN_WIDTH) >= (MAPWIDTH - PLACEMENT_OFFSET)){
						//sX = ((sWorldScreenX + NORMAL_MAP_SCREEN_WIDTH) - (MAPWIDTH - PLACEMENT_OFFSET)) / 5;
						//gTPClipRect.iRight -= sX;
						gTPClipRect.iRight -= PLACEMENT_OFFSET/5;
					//}
					break;
				case INSERTION_CODE_SOUTH:
					//if((sWorldScreenY + NORMAL_MAP_SCREEN_HEIGHT) >= (MAPHEIGHT - PLACEMENT_OFFSET)){
						//sY = ((sWorldScreenY + NORMAL_MAP_SCREEN_HEIGHT) - (MAPHEIGHT - PLACEMENT_OFFSET)) / 5;
						//gTPClipRect.iBottom -= sY;
						gTPClipRect.iBottom -= PLACEMENT_OFFSET/5;
					//}
					break;
				case INSERTION_CODE_WEST:
					//if(sWorldScreenX <= PLACEMENT_OFFSET){
						//sX = (PLACEMENT_OFFSET - sWorldScreenX) / 5;
						//gTPClipRect.iLeft += sX;
						gTPClipRect.iLeft += PLACEMENT_OFFSET/5;
					//}
					break;
				}
			}
			else
			{
				switch(gMercPlacement[gbCursorMercID].ubStrategicInsertionCode)
				{
				case INSERTION_CODE_NORTH:
					if(sWorldScreenY <= PLACEMENT_OFFSET)
					{
						sY = (PLACEMENT_OFFSET - sWorldScreenY) / 5;
						gTPClipRect.iTop += sY;
					}
					break;
				case INSERTION_CODE_EAST:
					if((sWorldScreenX + NORMAL_MAP_SCREEN_WIDTH) >= (MAPWIDTH - PLACEMENT_OFFSET))
					{
						sX = ((sWorldScreenX + NORMAL_MAP_SCREEN_WIDTH) - (MAPWIDTH - PLACEMENT_OFFSET)) / 5;
						gTPClipRect.iRight -= sX;
					}
					break;
				case INSERTION_CODE_SOUTH:
					if((sWorldScreenY + NORMAL_MAP_SCREEN_HEIGHT) >= (MAPHEIGHT - PLACEMENT_OFFSET))
					{
						sY = ((sWorldScreenY + NORMAL_MAP_SCREEN_HEIGHT) - (MAPHEIGHT - PLACEMENT_OFFSET)) / 5;
						gTPClipRect.iBottom -= sY;
					}
					break;
				case INSERTION_CODE_WEST:
					if(sWorldScreenX <= PLACEMENT_OFFSET)
					{
						sX = (PLACEMENT_OFFSET - sWorldScreenX) / 5;
						gTPClipRect.iLeft += sX;
					}
					break;
				}
			}

			if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
			{
				INT16 sCellX = 0;
				INT16 sCellY = 0;
				
				GetOverheadScreenXYFromGridNo( gMapInformation.sCenterGridNo, &sCellX, &sCellY );
				
				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + sCellX - 5 * gGameExternalOptions.usAmbushSpreadRadiusMercs;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + sCellY - 3 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + sCellX + 5 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + sCellY + 3 * gGameExternalOptions.usAmbushSpreadRadiusMercs;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
			// WANNE - MP: Center
			else if (is_networked && gfCenter)
			{
				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + CENTERENTRYPTS_LEFT_X;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + CENTERENTRYPTS_TOP_Y;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + CENTERENTRYPTS_RIGHT_X;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + CENTERENTRYPTS_BOTTOM_Y;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
			// Flugente airdrop
			else if ( gfCenter && gGameExternalOptions.ubSkyriderHotLZ == 3 && gbSelectedMercID >= 0 && gMercPlacement[ gbSelectedMercID ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
			{
				// Left black border
				gTPClipRectCenterLeft.iLeft = iOffsetHorizontal;
				gTPClipRectCenterLeft.iTop = iOffsetVertical + 3;
				gTPClipRectCenterLeft.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterLeft.iRight = iOffsetHorizontal + AIRDROPENTRYPTS_LEFT_X;

				// Top black border
				gTPClipRectCenterTop.iLeft = iOffsetHorizontal;
				gTPClipRectCenterTop.iTop = iOffsetVertical + 3;
				gTPClipRectCenterTop.iBottom = iOffsetVertical + AIRDROPENTRYPTS_TOP_Y;
				gTPClipRectCenterTop.iRight = iOffsetHorizontal + 634;

				// Right black border
				gTPClipRectCenterRight.iLeft = iOffsetHorizontal + AIRDROPENTRYPTS_RIGHT_X;
				gTPClipRectCenterRight.iTop = iOffsetVertical + 3;
				gTPClipRectCenterRight.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterRight.iRight = iOffsetHorizontal + 634;

				// Bottom black border
				gTPClipRectCenterBottom.iLeft = iOffsetHorizontal;
				gTPClipRectCenterBottom.iTop = iOffsetVertical + AIRDROPENTRYPTS_BOTTOM_Y;
				gTPClipRectCenterBottom.iBottom = iOffsetVertical + 320;
				gTPClipRectCenterBottom.iRight = iOffsetHorizontal + 634;
			}
		}

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		
		if (!gfCenter || (gGameExternalOptions.ubSkyriderHotLZ == 3 && gbSelectedMercID >= 0 && !(gMercPlacement[ gbSelectedMercID ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP)) )
			Blt16BPPBufferLooseHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &gTPClipRect, usHatchColor );
		// WANNE - MP: Center
		else
		{
			Blt16BPPBufferLooseHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &gTPClipRectCenterLeft, usHatchColor );
			Blt16BPPBufferLooseHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &gTPClipRectCenterTop, usHatchColor );
			Blt16BPPBufferLooseHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &gTPClipRectCenterRight, usHatchColor );
			Blt16BPPBufferLooseHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &gTPClipRectCenterBottom, usHatchColor );
		}

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

		if (!gfCenter || (gGameExternalOptions.ubSkyriderHotLZ == 3 && gbSelectedMercID >= 0 && !(gMercPlacement[ gbSelectedMercID ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP)) )
			RectangleDraw( TRUE, gTPClipRect.iLeft, gTPClipRect.iTop, gTPClipRect.iRight, gTPClipRect.iBottom, usHatchColor, pDestBuf );
		else
		{
			RectangleDraw( TRUE, gTPClipRectCenterLeft.iLeft, gTPClipRectCenterLeft.iTop, gTPClipRectCenterLeft.iRight, gTPClipRectCenterLeft.iBottom, usHatchColor, pDestBuf );
			RectangleDraw( TRUE, gTPClipRectCenterTop.iLeft, gTPClipRectCenterTop.iTop, gTPClipRectCenterTop.iRight, gTPClipRectCenterTop.iBottom, usHatchColor, pDestBuf );
			RectangleDraw( TRUE, gTPClipRectCenterRight.iLeft, gTPClipRectCenterRight.iTop, gTPClipRectCenterRight.iRight, gTPClipRectCenterRight.iBottom, usHatchColor, pDestBuf );
			RectangleDraw( TRUE, gTPClipRectCenterBottom.iLeft, gTPClipRectCenterBottom.iTop, gTPClipRectCenterBottom.iRight, gTPClipRectCenterBottom.iBottom, usHatchColor, pDestBuf );
		}
		
		UnLockVideoSurface( FRAME_BUFFER );
	}

	for( i = 0; i < giPlacements; ++i )
	{
		//Render the merc's names
		pSoldier = gMercPlacement[ i ].pSoldier;

		xp = xResOffset + 95 + (i / 2) * 54;

		if (i % 2)
		{
			yp = SCREEN_HEIGHT - 58;
		}
		else
		{
			yp = SCREEN_HEIGHT - 109;
		}

		//yp = (i % 2) ? 422 : 371;
		//NAME
		if( gubDefaultButton == GROUP_BUTTON && gMercPlacement[ i ].pSoldier->ubGroupID == gubSelectedGroupID ||
			gubDefaultButton != GROUP_BUTTON && i == gbSelectedMercID )
		{
			ubColor = FONT_YELLOW;
		}
		else if( gubDefaultButton == GROUP_BUTTON && gMercPlacement[ i ].pSoldier->ubGroupID == gubHilightedGroupID ||
						gubDefaultButton != GROUP_BUTTON && i == gbHilightedMercID )
		{
			ubColor = FONT_WHITE;
		}
		else
		{
			ubColor = FONT_GRAY3;
		}
		SetFont( FONT10ARIALBOLD );
		SetFontForeground( ubColor );
		SetFontShadow( 141 );

		//Render the question mark over the face if the merc hasn't yet been placed.
		if( gMercPlacement[ i ].fPlaced )
		{
			RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, (INT16)(xp + 16), (INT16)(yp + 14), (INT16)(xp + 24), (INT16)(yp + 22) );
		}
		else
		{
			mprintf( xp + 16, yp + 14, L"?" );
			InvalidateRegion( xp + 16, yp + 14, xp + 24, yp + 22 );
		}
		SetFont( BLOCKFONT );
		width = StringPixLength( pSoldier->name, BLOCKFONT );
		height = GetFontHeight( BLOCKFONT );
		xp = xp + ( 48 - width ) / 2;
		yp = yp + 33;
		mprintf( xp, yp, pSoldier->name );
		InvalidateRegion( xp, yp, xp + width, yp + width );
	}
}

void EnsureDoneButtonStatus()
{
	INT32 i;
	//static BOOLEAN fInside = FALSE;
	//BOOLEAN fChanged = FALSE;
	for( i = 0; i < giPlacements; i++ )
	{
		if( !gMercPlacement[ i ].fPlaced )
		{
			if( ButtonList[ iTPButtons[ DONE_BUTTON ] ]->uiFlags & BUTTON_ENABLED )
			{
				DisableButton( iTPButtons[ DONE_BUTTON ] );
				SetButtonFastHelpText( iTPButtons[ DONE_BUTTON ], gpStrategicString[ STR_TP_DISABLED_DONEHELP ] );
			}
			return;
		}
	}
	if( !(ButtonList[ iTPButtons[ DONE_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) )
	{ //only enable it when it is disabled, otherwise the button will stay down!
		EnableButton( iTPButtons[ DONE_BUTTON ] );
		SetButtonFastHelpText( iTPButtons[ DONE_BUTTON ], gpStrategicString[ STR_TP_DONEHELP ] );
	}
}

void lockui (bool unlock) //lock onluck ui for lan //hayden
{
		if(unlock==0 && islocked==0 && is_client)
		{
			islocked=1;
			DisableButton( iTPButtons[ DONE_BUTTON ] );
					DisableButton( iTPButtons[ SPREAD_BUTTON ] );
						DisableButton( iTPButtons[ GROUP_BUTTON ] );
							DisableButton( iTPButtons[ CLEAR_BUTTON ] );

			SGPRect CenterRect = { 100 + xResOffset, 100, SCREEN_WIDTH - 100 - xResOffset, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, MPServerMessage[8],  guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, DialogRemoved,  &CenterRect );

			//send loaded
			send_loaded();
		}

		if(unlock) //oh yeah ! :)
		{
					islocked=3;
					EnableButton( iTPButtons[ SPREAD_BUTTON ] );
					EnableButton( iTPButtons[ GROUP_BUTTON ] );
					EnableButton( iTPButtons[ CLEAR_BUTTON ] );

					ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
					ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags |= BUTTON_DIRTY;
					gubDefaultButton = CLEAR_BUTTON;
					PlaceMercs();
					gMsgBox.bHandled = MSG_BOX_RETURN_OK; //close if still open
		}
}

void TacticalPlacementHandle()
{
	InputAtom InputEvent;

	EnsureDoneButtonStatus();

	RenderTacticalPlacementGUI();

	if (is_networked)
		lockui(0);//lockui before placement while clients loading //hayden

	if( gfRightButtonState )
	{
		gbSelectedMercID = -1;
		gubSelectedGroupID = 0;
		gpTacticalPlacementSelectedSoldier = NULL;
	}

	while (DequeueEvent(&InputEvent) == TRUE)
	{
		if( InputEvent.usEvent == KEY_DOWN )
		{
			switch( InputEvent.usParam )
			{
				#ifdef JA2TESTVERSION
				case ESC:
					//if (!is_networked)
						KillTacticalPlacementGUI();
					break;
				#endif
				case ENTER:
					if( ButtonList[ iTPButtons[ DONE_BUTTON ] ]->uiFlags & BUTTON_ENABLED )
					{
									if(!is_client)KillTacticalPlacementGUI();
									if(is_client)send_donegui(0);
					}
					break;
				case 'c':
								if(islocked!=1)ClearPlacementsCallback( ButtonList[ iTPButtons[ CLEAR_BUTTON ] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
								break;
				case 'g':
								if(islocked!=1)GroupPlacementsCallback( ButtonList[ iTPButtons[ GROUP_BUTTON ] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
								break;
				case 's':
								if(islocked!=1)SpreadPlacementsCallback( ButtonList[ iTPButtons[ SPREAD_BUTTON ] ], MSYS_CALLBACK_REASON_LBUTTON_UP );
								break;
				case 'x':
					if( InputEvent.usKeyState & ALT_DOWN )
					{
						HandleShortCutExitState();
					}
					break;
			}
		}
	}
	gfValidCursor = FALSE;

	if(gbSelectedMercID != -1 && ((gusMouseYPos < (iOffsetVertical + 320) && gusMouseYPos > iOffsetVertical && gusMouseXPos > iOffsetHorizontal && gusMouseXPos < (iOffsetHorizontal + 640))||(gfUseBiggerOverview && (gusMouseYPos < (iOffsetVertical + 716) && gusMouseYPos > iOffsetVertical && gusMouseXPos > iOffsetHorizontal && gusMouseXPos < (iOffsetHorizontal + 1432) ))))
	{
		//dnl ch45 051009
		INT16 sWorldScreenX = (gusMouseXPos - iOffsetHorizontal) * 5;
		INT16 sWorldScreenY = (gusMouseYPos - iOffsetVertical) * 5;
		INT32 iCellX, iCellY;
		GetFromAbsoluteScreenXYWorldXY(&iCellX, &iCellY, sWorldScreenX, sWorldScreenY);
		iCellX = (iCellX / CELL_X_SIZE) + (giXA - 0);
		iCellY = (iCellY / CELL_Y_SIZE) + (giYA - WORLD_ROWS/2);
		GetWorldXYAbsoluteScreenXY(iCellX, iCellY, &sWorldScreenX, &sWorldScreenY);
		switch(gMercPlacement[gbCursorMercID].ubStrategicInsertionCode)
		{
		case INSERTION_CODE_NORTH:
			if(sWorldScreenY <= PLACEMENT_OFFSET)
				gfValidCursor = TRUE;
			break;
		case INSERTION_CODE_EAST:
			if(sWorldScreenX >= (MAPWIDTH - PLACEMENT_OFFSET))
				gfValidCursor = TRUE;
			break;
		case INSERTION_CODE_SOUTH:
			if(sWorldScreenY >= (MAPHEIGHT - PLACEMENT_OFFSET))
				gfValidCursor = TRUE;
			break;
		case INSERTION_CODE_WEST:
			if(sWorldScreenX <= PLACEMENT_OFFSET)
				gfValidCursor = TRUE;
			break;
		}

		// WANNE - MP: Center
		if ( gfCenter && (is_networked || 
			GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE ||
			( gGameExternalOptions.ubSkyriderHotLZ == 3 && gMercPlacement[gbSelectedMercID].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )) )
		{
			if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
			{
				INT32 testgridno = NOWHERE;

				if ( GetOverheadMouseGridNo( &testgridno ) && PythSpacesAway( testgridno, gMapInformation.sCenterGridNo ) < gGameExternalOptions.usAmbushSpreadRadiusMercs )
				{
					gfValidCursor = TRUE;
				}
			}
			else if( gMercPlacement[gbCursorMercID].ubStrategicInsertionCode == INSERTION_CODE_CENTER )
			{
				if (gusMouseYPos >= (iOffsetVertical + CENTERENTRYPTS_TOP_Y) &&		// N
					gusMouseYPos <= (iOffsetVertical + CENTERENTRYPTS_BOTTOM_Y) &&	// S
					gusMouseXPos >= (iOffsetHorizontal + CENTERENTRYPTS_LEFT_X) &&	// W
					gusMouseXPos <= (iOffsetHorizontal + CENTERENTRYPTS_RIGHT_X))	// E
				{
					gfValidCursor = TRUE;
				}
			}
			else if ( gMercPlacement[ gbCursorMercID ].ubStrategicInsertionCode == INSERTION_CODE_CHOPPER )
			{
				if (gusMouseYPos >= (iOffsetVertical + AIRDROPENTRYPTS_TOP_Y) &&		// N
					gusMouseYPos <= (iOffsetVertical + AIRDROPENTRYPTS_BOTTOM_Y) &&		// S
					gusMouseXPos >= (iOffsetHorizontal + AIRDROPENTRYPTS_LEFT_X) &&		// W
					gusMouseXPos <= (iOffsetHorizontal + AIRDROPENTRYPTS_RIGHT_X))		// E
				{
					gfValidCursor = TRUE;
				}
			}
		}

		if( gubDefaultButton == GROUP_BUTTON )
		{
			if( gfValidCursor )
			{
				SetCurrentCursorFromDatabase( CURSOR_PLACEGROUP );
			}
			else
			{
				SetCurrentCursorFromDatabase( CURSOR_DPLACEGROUP );
			}
		}
		else
		{
			if( gfValidCursor )
			{
				SetCurrentCursorFromDatabase( CURSOR_PLACEMERC );
			}
			else
			{
				SetCurrentCursorFromDatabase( CURSOR_DPLACEMERC );
			}
		}
	}
	else
	{
		SetCurrentCursorFromDatabase( CURSOR_NORMAL );
	}
	if( gfKillTacticalGUI == 1 )
	{
		if(is_client)
		{
			gfKillTacticalGUI = FALSE;
			send_donegui(0);
		}
		if(!is_client)KillTacticalPlacementGUI();
	}
	else if( gfKillTacticalGUI == 2 )
	{
		gfKillTacticalGUI = 1;
	}

	ScrollOverheadMap();//dnl ch45 021009
}

void KillTacticalPlacementGUI()
{
	INT32 i;

	gbHilightedMercID = -1;
	gbSelectedMercID = -1;
	gubSelectedGroupID = 0;
	gubHilightedGroupID = 0;
	gbCursorMercID = -1;
	gpTacticalPlacementHilightedSoldier = NULL;
	gpTacticalPlacementSelectedSoldier = NULL;

	//Destroy the tactical placement gui.
	gfEnterTacticalPlacementGUI = FALSE;
	gfTacticalPlacementGUIActive = FALSE;
	gfKillTacticalGUI = FALSE;
	//Delete video objects
	DeleteVideoObjectFromIndex( giOverheadPanelImage );
	DeleteVideoObjectFromIndex( giMercPanelImage );
	//Delete buttons
	for( i = 0; i < NUM_TP_BUTTONS; i++ )
	{
		UnloadButtonImage( giOverheadButtonImages[ i ] );
		RemoveButton( iTPButtons[ i ] );
	}
	//Delete faces and regions
	for( i = 0; i < giPlacements; i++ )
	{
		DeleteVideoObjectFromIndex( gMercPlacement[ i ].uiVObjectID );
		MSYS_RemoveRegion( &gMercPlacement[ i ].region );
	}

	if( gsCurInterfacePanel < 0 || gsCurInterfacePanel >= NUM_UI_PANELS )
		gsCurInterfacePanel = TEAM_PANEL;

	SetCurrentInterfacePanel( (UINT8)gsCurInterfacePanel );

	//Leave the overhead map.
	KillOverheadMap();
	//Recreate the tactical panel.
	MSYS_EnableRegion(&gRadarRegion);
	SetCurrentInterfacePanel( TEAM_PANEL );
	//Initialize the rest of the map (AI, enemies, civs, etc.)

	for( i = 0; i < giPlacements; i++ )
	{
		PickUpMercPiece( i );
	}

	PrepareLoadedSector();
	EnableScrollMessages();

	#ifdef JA2BETAVERSION
	if( gfChangedEntrySide )
	{
		ScreenMsg( FONT_RED, MSG_ERROR, L"Substituted different entry side due to invalid entry points or map edgepoints.	KM, LC : 1" );
	}
	#endif

	MemFree( gMercPlacement);
}

void ChooseRandomEdgepoints()
{
	INT32 i;
	UINT8	lastValidICode = INSERTION_CODE_GRIDNO;
	for( i = 0; i < giPlacements; i++ )
	{
		if ( !( gMercPlacement[ i ].pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
			{
				UINT8 ubDirection;

				gMercPlacement[i].pSoldier->usStrategicInsertionData = FindRandomGridNoFromSweetSpotExcludingSweetSpot( gMercPlacement[i].pSoldier, gMapInformation.sCenterGridNo, gGameExternalOptions.usAmbushSpreadRadiusMercs, &ubDirection );

				// have the merc look outward. We add + 100 because later on we use this to signify that we want really enforce this direction
				gMercPlacement[i].pSoldier->ubInsertionDirection = (UINT8)GetDirectionToGridNoFromGridNo( gMapInformation.sCenterGridNo, gMercPlacement[i].pSoldier->usStrategicInsertionData ) + 100;
			}
			else
			{
				gMercPlacement[ i ].pSoldier->usStrategicInsertionData = ChooseMapEdgepoint( &gMercPlacement[ i ].ubStrategicInsertionCode, lastValidICode );
			}
			
			if( !TileIsOutOfBounds(gMercPlacement[ i ].pSoldier->usStrategicInsertionData))
			{
				gMercPlacement[ i ].pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				lastValidICode = gMercPlacement[ i ].ubStrategicInsertionCode;
			}
			else
			{
				gMercPlacement[ i ].pSoldier->ubStrategicInsertionCode = gMercPlacement[ i ].ubStrategicInsertionCode;
			}
		}

		PutDownMercPiece( i );
	}
	gfEveryonePlaced = TRUE;
}

void PlaceMercs()
{
	INT32 i;
	switch( gubDefaultButton )
	{
		case SPREAD_BUTTON: //Place mercs randomly along their side using map edgepoints.
			ChooseRandomEdgepoints();
			break;
		case CLEAR_BUTTON:
			for( i = 0; i < giPlacements; i++ )
			{
				PickUpMercPiece( i );
			}
			gubSelectedGroupID = 0;
			gbSelectedMercID = 0;
			SetCursorMerc( 0 );
			gfEveryonePlaced = FALSE;
			break;
		default:
			return;
	}
	gfTacticalPlacementGUIDirty = TRUE;
}

void DoneOverheadPlacementClickCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfKillTacticalGUI = 2;
	}
}

void SpreadPlacementsCallback ( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gubDefaultButton = SPREAD_BUTTON;
		ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags |= BUTTON_DIRTY;
		PlaceMercs();
		gubSelectedGroupID = 0;
		gbSelectedMercID = -1;
		SetCursorMerc( -1 );
	}
}

void GroupPlacementsCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( gubDefaultButton == GROUP_BUTTON )
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			btn->uiFlags |= BUTTON_DIRTY;
			gubDefaultButton = CLEAR_BUTTON;
			gubSelectedGroupID = 0;
		}
		else
		{
			btn->uiFlags |= BUTTON_CLICKED_ON | BUTTON_DIRTY;
			gubDefaultButton = GROUP_BUTTON;
			gbSelectedMercID = 0;
			SetCursorMerc( gbSelectedMercID );
			gubSelectedGroupID = gMercPlacement[ gbSelectedMercID ].pSoldier->ubGroupID;
		}
	}
}

void ClearPlacementsCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ iTPButtons[ GROUP_BUTTON ] ]->uiFlags |= BUTTON_DIRTY;
		gubDefaultButton = CLEAR_BUTTON;
		PlaceMercs();
	}
}

void MercMoveCallback( MOUSE_REGION *reg, INT32 reason )
{
	if( reg->uiFlags & MSYS_MOUSE_IN_AREA )
	{
		INT8 i;
		for( i = 0; i < giPlacements; i++ )
		{
			if( &gMercPlacement[ i ].region == reg )
			{
				if( gbHilightedMercID != i )
				{
					gbHilightedMercID = i;
					if( gubDefaultButton == GROUP_BUTTON )
						gubHilightedGroupID = gMercPlacement[ i ].pSoldier->ubGroupID;
					SetCursorMerc( i );
					gpTacticalPlacementHilightedSoldier = gMercPlacement[ i ].pSoldier;
				}
				return;
			}
		}
	}
}

void MercClickCallback( MOUSE_REGION *reg, INT32 reason )
{
	if(islocked != 1)//hayden
	{
		if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
		{
			INT8 i;
			for( i = 0; i < giPlacements; i++ )
			{
				if( &gMercPlacement[ i ].region == reg )
				{
					if( gbSelectedMercID != i )
					{
						if ( gGameExternalOptions.ubSkyriderHotLZ == 3 && gMercPlacement[ i ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
							gubDefaultButton = GROUP_BUTTON;

						gbSelectedMercID = i;
						gpTacticalPlacementSelectedSoldier = gMercPlacement[ i ].pSoldier;
						if( gubDefaultButton == GROUP_BUTTON )
						{
							gubSelectedGroupID = gpTacticalPlacementSelectedSoldier->ubGroupID;
						}
					}
					return;
				}
			}
		}
	}
}

void SelectNextUnplacedUnit()
{
	INT32 i;
	if( gbSelectedMercID == -1 )
		return;

	if ( gGameExternalOptions.ubSkyriderHotLZ == 3 && gMercPlacement[ gbSelectedMercID ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
		gubDefaultButton = GROUP_BUTTON;

	for( i = gbSelectedMercID; i < giPlacements; ++i )
	{ //go from the currently selected soldier to the end
		if( !gMercPlacement[ i ].fPlaced )
		{ //Found an unplaced merc.	Select him.
			gbSelectedMercID = (INT8)i;
			if( gubDefaultButton == GROUP_BUTTON )
				gubSelectedGroupID = gMercPlacement[ i ].pSoldier->ubGroupID;
			gfTacticalPlacementGUIDirty = TRUE;
			SetCursorMerc( (INT8)i );
			gpTacticalPlacementSelectedSoldier = gMercPlacement[ i ].pSoldier;
			return;
		}
	}

	for( i = 0; i < gbSelectedMercID; ++i )
	{ //go from the beginning to the currently selected soldier
		if( !gMercPlacement[ i ].fPlaced )
		{ //Found an unplaced merc.	Select him.
			gbSelectedMercID = (INT8)i;
			if( gubDefaultButton == GROUP_BUTTON )
				gubSelectedGroupID = gMercPlacement[ i ].pSoldier->ubGroupID;
			gfTacticalPlacementGUIDirty = TRUE;
			SetCursorMerc( (INT8)i );
			gpTacticalPlacementSelectedSoldier = gMercPlacement[ i ].pSoldier;
			return;
		}
	}

	//checked the whole array, and everybody has been placed.	Select nobody.
	if( !gfEveryonePlaced )
	{
		gfEveryonePlaced = TRUE;
		SetCursorMerc( -1 );
		gbSelectedMercID = -1;
		gubSelectedGroupID = 0;
		gfTacticalPlacementGUIDirty = TRUE;
		gfValidLocationsChanged = TRUE;
		gpTacticalPlacementSelectedSoldier = gMercPlacement[ i ].pSoldier;
	}
}

void HandleTacticalPlacementClicksInOverheadMap( MOUSE_REGION *reg, INT32 reason )
{
	INT32 i;
	INT32 sGridNo;
	BOOLEAN fInvalidArea = FALSE;
	UINT8	lastValidICode = INSERTION_CODE_GRIDNO;
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{ //if we have a selected merc, move him to the new closest map edgepoint of his side.
		if( gfValidCursor )
		{
			if( gbSelectedMercID != -1 )
			{
				if ( gGameExternalOptions.ubSkyriderHotLZ == 3 && gMercPlacement[ gbSelectedMercID ].pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
					gubDefaultButton = GROUP_BUTTON;

				if( GetOverheadMouseGridNo( &sGridNo ) )
				{ //we have clicked within a valid part of the map.
					BeginMapEdgepointSearch();

					if( gubDefaultButton == GROUP_BUTTON )
					{ //We are placing a whole group.
						for( i = 0; i < giPlacements; i++ )
						{ //Find locations of each member of the group, but don't place them yet.	If
							//one of the mercs can't be placed, then we won't place any, and tell the user
							//the problem.	If everything's okay, we will place them all.
							if( gMercPlacement[ i ].pSoldier->ubGroupID == gubSelectedGroupID )
							{
								gMercPlacement[ i ].pSoldier->usStrategicInsertionData = SearchForClosestPrimaryMapEdgepoint( sGridNo, gMercPlacement[ i ].ubStrategicInsertionCode, lastValidICode, &gMercPlacement[ i ].ubStrategicInsertionCode );
								
								if(TileIsOutOfBounds(gMercPlacement[ i ].pSoldier->usStrategicInsertionData))
								{
									fInvalidArea = TRUE;
									break;
								}
								else
									lastValidICode = gMercPlacement[ i ].ubStrategicInsertionCode;
							}
						}
						if( !fInvalidArea )
						{ //One or more of the mercs in the group didn't get gridno assignments, so we
							//report an error.
							for( i = 0; i < giPlacements; i++ )
							{
								gMercPlacement[ i ].pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
								if( gMercPlacement[ i ].pSoldier->ubGroupID == gubSelectedGroupID )
								{
									PutDownMercPiece( i );
								}
							}
						}
					}
					else
					{ //This is a single merc placement.	If valid, then place him, else report error.
						gMercPlacement[ gbSelectedMercID ].pSoldier->usStrategicInsertionData = SearchForClosestPrimaryMapEdgepoint( sGridNo, gMercPlacement[ gbSelectedMercID ].ubStrategicInsertionCode );
						
						if( !TileIsOutOfBounds(gMercPlacement[ gbSelectedMercID ].pSoldier->usStrategicInsertionData))
						{
							gMercPlacement[ gbSelectedMercID ].pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
							PutDownMercPiece( gbSelectedMercID );
						}
						else
						{
							fInvalidArea = TRUE;
						}

						//gbSelectedMercID++;
						//if( gbSelectedMercID == giPlacements )
						//	gbSelectedMercID = 0;
						//gpTacticalPlacementSelectedSoldier = gMercPlacement[ gbSelectedMercID ].pSoldier;
						gfTacticalPlacementGUIDirty = TRUE;
						//SetCursorMerc( gbSelectedMercID );
					}
					EndMapEdgepointSearch();

					if( fInvalidArea )
					{
						// Only display the error message, when the cursor is on the overview map
						if (gusMouseYPos < (iOffsetVertical + 320) && gusMouseYPos > iOffsetVertical
							&& gusMouseXPos > iOffsetHorizontal && gusMouseXPos < (iOffsetHorizontal + 640))
						{
							//Report error due to invalid placement.
							SGPRect CenterRect = { 100 + xResOffset, 100 + 0, SCREEN_WIDTH - 100 - xResOffset, 300 };
						DoMessageBox( MSG_BOX_BASIC_STYLE, gpStrategicString[ STR_TP_INACCESSIBLE_MESSAGE ],	guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, DialogRemoved,	&CenterRect );
					}
					}
					else
					{ //Placement successful, so select the next unplaced unit (single or group).
						SelectNextUnplacedUnit();
					}
				}
			}
		}
		else
		{ //not a valid cursor location...
			if( gbCursorMercID != - 1 )
			{
				// Only display the error message, when the cursor is on the overview map
				if (gusMouseYPos < (iOffsetVertical + 320) && gusMouseYPos > iOffsetVertical
					&& gusMouseXPos > iOffsetHorizontal && gusMouseXPos < (iOffsetHorizontal + 640))
				{
					SGPRect CenterRect = { 100 + xResOffset, 100, SCREEN_WIDTH - 100 - xResOffset, 300 };
					DoMessageBox( MSG_BOX_BASIC_STYLE, gpStrategicString[ STR_TP_INVALID_MESSAGE ],	guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, DialogRemoved,	&CenterRect );
				}
			}
		}
	}
}

void SetCursorMerc( INT8 bPlacementID )
{
	if( gbCursorMercID != bPlacementID )
	{
		if( gbCursorMercID == -1 || bPlacementID == -1 ||
			gMercPlacement[ gbCursorMercID ].ubStrategicInsertionCode != gMercPlacement[ bPlacementID ].ubStrategicInsertionCode )
			gfValidLocationsChanged = TRUE;
		gbCursorMercID = bPlacementID;
	}
}


void PutDownMercPiece( INT32 iPlacement )
{
	INT32 sGridNo;
	INT16 sCellX, sCellY;
	UINT8 ubDirection;

	SOLDIERTYPE *pSoldier;
	pSoldier = gMercPlacement[ iPlacement ].pSoldier;
	switch( pSoldier->ubStrategicInsertionCode )
	{
		case INSERTION_CODE_NORTH:
			pSoldier->sInsertionGridNo = gMapInformation.sNorthGridNo;
			break;
		case INSERTION_CODE_SOUTH:
			pSoldier->sInsertionGridNo = gMapInformation.sSouthGridNo;
			break;
		case INSERTION_CODE_EAST:
			pSoldier->sInsertionGridNo = gMapInformation.sEastGridNo;
			break;
		case INSERTION_CODE_WEST:
			pSoldier->sInsertionGridNo = gMapInformation.sWestGridNo;
			break;
		case INSERTION_CODE_GRIDNO:
			pSoldier->sInsertionGridNo = pSoldier->usStrategicInsertionData;
			break;
		case INSERTION_CODE_CHOPPER:
			pSoldier->sInsertionGridNo = pSoldier->usStrategicInsertionData;
			break;
		default:
			Assert( 0 );
			break;
	}

	if( gMercPlacement[ iPlacement ].fPlaced )
		PickUpMercPiece( iPlacement );

	sGridNo = FindGridNoFromSweetSpot( pSoldier, pSoldier->sInsertionGridNo, 4, &ubDirection );
	
	if(!TileIsOutOfBounds(sGridNo))
	{
		// Flugente: initiate airdrop, place merc on roof if necessary
		if ( gGameExternalOptions.ubSkyriderHotLZ == 3 && pSoldier->usSoldierFlagMask & SOLDIER_AIRDROP )
		{
			// hideous hack
			gfTacticalDoHeliRun = TRUE;
			SetHelicopterDroppoint(sGridNo);

			pSoldier->usStrategicInsertionData = sGridNo;

			if ( FindStructure( sGridNo, STRUCTURE_NORMAL_ROOF ) != NULL && NewOKDestination( pSoldier, sGridNo, false, 1 ) )
			{
				pSoldier->SetSoldierHeight( (FLOAT)58 );
			}
		}

		ConvertGridNoToCellXY( sGridNo, &sCellX, &sCellY );
		
		FLOAT scX = (FLOAT)sCellX;
		FLOAT scY = (FLOAT)sCellY;//hayden
		if (is_networked)
		{
			pSoldier->EVENT_SetSoldierPosition( scX, scY );
		}
		else
		{
			pSoldier->EVENT_SetSoldierPosition( (FLOAT)sCellX, (FLOAT)sCellY );
		}

		if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
		{
			ubDirection = (UINT8)GetDirectionToGridNoFromGridNo( gMapInformation.sCenterGridNo, sGridNo ) + 100;
		}
		
		pSoldier->EVENT_SetSoldierDirection( ubDirection );
		pSoldier->ubInsertionDirection = pSoldier->ubDirection;

		gMercPlacement[ iPlacement ].fPlaced = TRUE;
		gMercPlacement[ iPlacement ].pSoldier->bInSector = TRUE;
//hayden
		if(is_client)send_gui_pos(pSoldier, scX, scY);
		if(is_client)send_gui_dir(pSoldier, ubDirection);
	}
}

void PickUpMercPiece( INT32 iPlacement )
{
	gMercPlacement[ iPlacement ].pSoldier->RemoveSoldierFromGridNo( );
	gMercPlacement[ iPlacement ].fPlaced = FALSE;
	gMercPlacement[ iPlacement ].pSoldier->bInSector = FALSE;
}

void FastHelpRemovedCallback()
{
	gfTacticalPlacementGUIDirty = TRUE;
}

void FastHelpRemoved2Callback()
{
	gfTacticalPlacementGUIDirty = TRUE;
	gfValidLocationsChanged = 2; //because fast help text covers it.
}

void DialogRemoved( UINT8 ubResult )
{
	gfTacticalPlacementGUIDirty = TRUE;
	gfValidLocationsChanged = TRUE;
}
