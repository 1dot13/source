#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "HelpScreen.h"
#else
	#include "Map Screen Interface TownMine Info.h"
	#include "strategicmap.h"
	#include "popupbox.h"
	#include "Map Screen Interface.h"
	#include "Queen Command.h"
	#include "Player Command.h"
	#include "Font Control.h"
	#include "Font.h"
	#include "Text.h"
	#include "Map Screen Interface Map.h"
	#include "Map Screen Interface Border.h"
	#include "Handle UI.h"
	#include "NPC.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "finances.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Strategic.h"
	#include "Utilities.h"
	#include "video.h"
	#include "Town Militia.h"
	#include "HelpScreen.h"
	#include "Map Screen Helicopter.h"
	#include "Tactical Save.h"
	#include "GameSettings.h"
	#include "debug.h"
	#include "Overhead.h"	// added by Flugente
	#include "Game Clock.h"			// added by Flugente
	#include "Game Event Hook.h"	// added by Flugente
#endif

#include "Strategic Mines.h"

#define BOX_BUTTON_WIDTH 100
#define BOX_BUTTON_HEIGHT 20


// flag to say if we are showing town/mine box at all
BOOLEAN fShowTownInfo = FALSE;

INT32 ghTownMineBox = -1;
SGPPoint TownMinePosition ={ 300, 150 };
SGPRect TownMineDimensions ={ 0, 0, 240, 60 };

INT8 bCurrentTownMineSectorX = 0;
INT8 bCurrentTownMineSectorY = 0;
INT8 bCurrentTownMineSectorZ = 0;

// inventory button
UINT32 guiMapButtonInventoryImage[2];
UINT32 guiMapButtonInventory[2];

UINT16 sTotalButtonWidth = 0;

//extern MINE_LOCATION_TYPE gMineLocation[];
//extern MINE_STATUS_TYPE gMineStatus[];

extern BOOLEAN fMapScreenBottomDirty;
//extern UINT8 gubMonsterMineInfestation[];

// HEADROCK HAM 5: Must be externed here for correct sector name display
extern CHAR16 gzSectorNames[256][4][MAX_SECTOR_NAME_LENGTH];

// create the town/mine info box
void CreateTownInfoBox( void );

// add town text to town info box
void AddTextToTownBox( void );

// add text to mine info box
void AddTextToMineBox( void );

// add text to non-town/non-mine the other boxes
void AddTextToBlankSectorBox( void );

// add "sector" line text to any popup box
void AddSectorToBox(void);

void AddCommonInfoToBox(void);

void AddItemsInSectorToBox(void);

// position town/mine info box on the screen
void PositionTownMineInfoBox( void );

// add the pop up button for the map inventory pop up activation
void AddInventoryButtonForMapPopUpBox( void );

// now remove the above button
void RemoveInventoryButtonForMapPopUpBox( void );

// callback to turn on sector inventory list
void MapTownMineInventoryButtonCallBack( GUI_BUTTON *btn, INT32 reason );
void MapTownMineExitButtonCallBack( GUI_BUTTON *btn, INT32 reason );
void MinWidthOfTownMineInfoBox( void );

extern INT32 GetCurrentBalance( void );

void DisplayTownInfo( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	// will display town info for a particular town
	AssertGE( sMapX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sMapX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sMapY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sMapY, MAXIMUM_VALID_Y_COORDINATE );
	AssertGE( bMapZ, bMapZ );
	AssertLE( bMapZ, bMapZ );

	// set current sector
	if( ( bCurrentTownMineSectorX != sMapX ) || ( bCurrentTownMineSectorY != sMapY ) || ( bCurrentTownMineSectorZ != bMapZ ) )
	{
	bCurrentTownMineSectorX = ( INT8 )sMapX;
	bCurrentTownMineSectorY = ( INT8 )sMapY;
	bCurrentTownMineSectorZ = bMapZ;
	}

	//create destroy the box
	CreateDestroyTownInfoBox( );

}

void CreateDestroyTownInfoBox( void )
{
	// create destroy pop up box for town/mine info
	static BOOLEAN fCreated = FALSE;
	SGPRect pDimensions;
	SGPPoint pPosition;
	INT8 bTownId = 0;

	if( ( fCreated == FALSE ) && ( fShowTownInfo == TRUE ) )
	{
		// create pop up box
		CreateTownInfoBox( );

		// decide what kind of text to add to display

		if ( bCurrentTownMineSectorZ == 0 )
		{
			// only show the mine info when mines button is selected, otherwise we need to see the sector's regular town info
			if ( ( IsThereAMineInThisSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) == TRUE) && fShowMineFlag )
			{
				AddTextToMineBox( );
			}
			else
			{
				bTownId = GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

				// do we add text for the town box?
				if( bTownId != BLANK_SECTOR )
				{
					// add text for town box
					AddTextToTownBox( );
				}
				else
				{
					// just a blank sector (handles SAM sites if visible)
					AddTextToBlankSectorBox( );
				}
			}

			// add "militia", "militia training", "control" "enemy forces", etc. lines text to any popup box
			AddCommonInfoToBox();
		}
		else	// underground
		{
			// sector
			AddSectorToBox();
		}

		AddItemsInSectorToBox();

		// set font type
		SetBoxFont(ghTownMineBox, BLOCKFONT2);

		// set highlight color
		SetBoxHighLight(ghTownMineBox, FONT_WHITE);

		SetBoxSecondColumnForeground( ghTownMineBox, FONT_WHITE );
		SetBoxSecondColumnBackground( ghTownMineBox, FONT_BLACK );
		SetBoxSecondColumnHighLight( ghTownMineBox, FONT_WHITE );
		SetBoxSecondColumnShade( ghTownMineBox, FONT_BLACK );
		SetBoxSecondColumnFont( ghTownMineBox, BLOCKFONT2 );
		SetBoxSecondColumnMinimumOffset( ghTownMineBox, 20 );

		// unhighlighted color
		SetBoxForeground(ghTownMineBox, FONT_YELLOW);

		// background color
		SetBoxBackground(ghTownMineBox, FONT_BLACK);

		// shaded color..for darkened text
		SetBoxShade( ghTownMineBox, FONT_BLACK );

		// give title line (0) different color from the rest
		SetBoxLineForeground( ghTownMineBox, 0, FONT_LTGREEN );

		// ressize box to text
		ResizeBoxToText( ghTownMineBox );

		// make box bigger to this size
		GetBoxSize( ghTownMineBox , &pDimensions );

		if( pDimensions.iRight < BOX_BUTTON_WIDTH )
		{
			// resize box to fit button
			pDimensions.iRight += BOX_BUTTON_WIDTH;
		}
		
		pDimensions.iBottom += BOX_BUTTON_HEIGHT;

		for ( UINT16 x = 0; x < MAX_NUMBER_OF_SAMS; ++x )
		{
			if ( pSamList[x] == SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) )
			{
				pDimensions.iBottom += BOX_BUTTON_HEIGHT;

				break;
			}
		}

		SetBoxSize( ghTownMineBox, pDimensions );

		ShowBox( ghTownMineBox );

		// now position box
		MinWidthOfTownMineInfoBox( );
		PositionTownMineInfoBox( );

		// now add the button
		AddInventoryButtonForMapPopUpBox( );

		// now position box
		PositionTownMineInfoBox( );

		fCreated = TRUE;
	}
	else if( ( fCreated == TRUE ) && ( fShowTownInfo == FALSE ) )
	{
		// get box size
		GetBoxSize( ghTownMineBox, &pDimensions );

		// get position
		GetBoxPosition( ghTownMineBox, &pPosition);

		// destroy pop up box
		RemoveBox( ghTownMineBox );
		ghTownMineBox = -1;

		// remove inventory button
		RemoveInventoryButtonForMapPopUpBox( );

		// restore background
		RestoreExternBackgroundRect( ( INT16 )pPosition.iX, ( INT16 )pPosition.iY, ( INT16 ) ( pDimensions.iRight - pDimensions.iLeft ), ( INT16 ) ( pDimensions.iBottom - pDimensions.iTop + 3 ) );

		fCreated = FALSE;
	}
}


void CreateTownInfoBox( void )
{
	// create basic box
 CreatePopUpBox(&ghTownMineBox, TownMineDimensions, TownMinePosition, (POPUP_BOX_FLAG_CLIP_TEXT ));

 // which buffer will box render to
 SetBoxBuffer(ghTownMineBox, FRAME_BUFFER);

 // border type?
 SetBorderType(ghTownMineBox, guiPOPUPBORDERS);

 // background texture
 SetBackGroundSurface(ghTownMineBox, guiPOPUPTEX);

 // margin sizes
 SetMargins(ghTownMineBox, 6,6,8,6);

 // space between lines
 SetLineSpace(ghTownMineBox, 2);

 // set current box to this one
 SetCurrentBox( ghTownMineBox );

 return;
}


// adds text to town info box
void AddTextToTownBox( void )
{
	UINT32 hStringHandle = 0;
	CHAR16 wString[ 64 ];
	UINT8 ubTownId = 0;
	UINT16 usTownSectorIndex;
	INT16 sMineSector = 0;

	// remember town id
	ubTownId = GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY );
	Assert((ubTownId >= FIRST_TOWN) && (ubTownId < NUM_TOWNS));

	usTownSectorIndex = SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

	AssertGE(usTownSectorIndex, 0);
	AssertLT(usTownSectorIndex,256);

	////////////////////////////////////
	// HEADROCK HAM 5:
	// Read and verify XML sector names
	BOOLEAN fSectorHasXMLNames = TRUE;
	CHAR16 zUnexplored[MAX_SECTOR_NAME_LENGTH];
	CHAR16 zExplored[MAX_SECTOR_NAME_LENGTH];
	
	wcscpy( zUnexplored, gzSectorNames[ usTownSectorIndex ][0] );
	wcscpy( zExplored, gzSectorNames[ usTownSectorIndex ][2] );

	if (zUnexplored[0] == 0 || zExplored[0] == 0)
	{
		fSectorHasXMLNames = FALSE;
	}

	if (fSectorHasXMLNames) // ABOVE GROUND XML
	{
		// HEADROCK HAM 3.6: The program can now read custom names from XML for all above-ground sectors.
		// In the event that a specific name or set of names is missing, the program generates a default
		// name as it always has.
		// I've also updated the SAM Site sectors to rely on SamSite.XML data.
		
		BOOLEAN fVisited = (SectorInfo[ usTownSectorIndex ].uiFlags & SF_ALREADY_VISITED);
		BOOLEAN fSAMSiteKnown = FALSE;

		// Test for known SAM Site at this location
		for (UINT16 x=0; x < MAX_NUMBER_OF_SAMS; x++)
		{
			if ( pSamList[x] == usTownSectorIndex )
			{
				if ( fSamSiteFound[ x ] )
				{
					fSAMSiteKnown = TRUE;
					break;
				}
			}
		}

		if (fVisited || fSAMSiteKnown)
		{
			AddMonoString( &hStringHandle, zExplored );
		}
		else
		{
			AddMonoString( &hStringHandle, zUnexplored );
		}
	}
	else // ABOVE GROUND HARDCODED
	{
		switch( usTownSectorIndex )
		{
			case SEC_B13:
				AddMonoString( &hStringHandle, pLandTypeStrings[ DRASSEN_AIRPORT_SITE ] );
				break;
			case SEC_F8:
				AddMonoString( &hStringHandle, pLandTypeStrings[ CAMBRIA_HOSPITAL_SITE ] );
				break;
			case SEC_J9: //Tixa
				//if( !fFoundTixa )
				if( gfHiddenTown[ TIXA ] == FALSE )
					AddMonoString( &hStringHandle, pLandTypeStrings[ SAND ] );
				else
					AddMonoString( &hStringHandle, pTownNames[ TIXA ] );
				break;
			case SEC_K4: //Orta
				//if( !fFoundOrta )
				if( gfHiddenTown[ ORTA ] == FALSE )
					AddMonoString( &hStringHandle, pLandTypeStrings[ SWAMP ] );
				else
					AddMonoString( &hStringHandle, pTownNames[ ORTA ] );
				break;
			case SEC_N3:
				AddMonoString( &hStringHandle, pLandTypeStrings[ MEDUNA_AIRPORT_SITE ] );
				break;
			default:
				if( usTownSectorIndex == SEC_N4 && fSamSiteFound[ SAM_SITE_FOUR ] )
				{	//Meduna's SAM site
					AddMonoString( &hStringHandle, pLandTypeStrings[ MEDUNA_SAM_SITE ] );
				}
				else
				{ // town name
					swprintf( wString, L"%s", pTownNames[ ubTownId ] );
					AddMonoString( &hStringHandle, wString );
				}
				break;
		}
	}

	// blank line
	AddMonoString( &hStringHandle, L"" );

	// sector
	AddSectorToBox();

	// town size
	if( gfHiddenTown[ GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) ] )
	{
		swprintf( wString, L"%s:", pwTownInfoStrings[ 0 ] );
		AddMonoString( &hStringHandle, wString );
		swprintf( wString, L"%d",	GetTownSectorSize( ubTownId ) );
		AddSecondColumnMonoString( &hStringHandle, wString );
	}

	// main facilities
	// HEADROCK HAM 3.6: This function now does all the work of assembling a facility entry.
	AddFacilitiesToBox( bCurrentTownMineSectorX, bCurrentTownMineSectorY, &hStringHandle, TRUE );

	// the concept of control is only meaningful in sectors where militia can be trained
	if ( MilitiaTrainingAllowedInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, 0 ) &&
		gfHiddenTown[ GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) ] )
	{
		// town control
		swprintf( wString, L"%s:", pwTownInfoStrings[ 2 ] );
		AddMonoString( &hStringHandle, wString );
		swprintf( wString, L"%d%%%%",	(GetTownSectorsUnderControl( ubTownId ) * 100) / GetTownSectorSize( ubTownId ));
		AddSecondColumnMonoString( &hStringHandle, wString );
	}

	// the concept of town loyalty is only meaningful in towns where loyalty is tracked
	if( gTownLoyalty[ ubTownId ].fStarted && gfTownUsesLoyalty[ ubTownId ] &&
		gfHiddenTown[ GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) ] )
	{
		// town loyalty
		swprintf( wString, L"%s:", pwTownInfoStrings[ 5 ] );
		AddMonoString( &hStringHandle, wString );
		swprintf( wString, L"%d%%%%", gTownLoyalty[ ubTownId ].ubRating );
		AddSecondColumnMonoString( &hStringHandle, wString );
	}

	// if the town has a mine
	sMineSector = GetMineSectorForTown( ubTownId );
	if( sMineSector != -1 && gfHiddenTown[ GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) ] )
	{
		// Associated Mine: Sector
		swprintf( wString, L"%s:",	pwTownInfoStrings[ 4 ] );
		AddMonoString( &hStringHandle, wString );
		GetShortSectorString( ( INT16 )( sMineSector % MAP_WORLD_X ), ( INT16 )( sMineSector / MAP_WORLD_X ), wString );
		AddSecondColumnMonoString( &hStringHandle, wString );
	}

	// Flugente: if this is a prison we control, display number of prisoners here
	INT16 aPrisoners[PRISONER_MAX] = {0};
	UINT16 numprisoners = GetNumberOfPrisoners( &(SectorInfo[SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY )]), aPrisoners );
	if ( numprisoners )
	{
		// determine prison capacity
		UINT16 capacity = 0;
		for ( UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt )
		{
			// Is this facility here?
			if ( gFacilityLocations[SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY )][cnt].fFacilityHere )
			{
				// we determine wether this is a prison by checking for usPrisonBaseLimit
				if ( gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit > 0 )
				{
					capacity = gFacilityTypes[cnt].AssignmentData[FAC_INTERROGATE_PRISONERS].usPrisonBaseLimit;
					break;
				}
			}
		}

		// prisoners
		swprintf( wString, L"%s:", pwTownInfoStrings[ 13 ] );
		AddMonoString( &hStringHandle, wString );
		swprintf( wString, pwTownInfoStrings[14], numprisoners, capacity );
		AddSecondColumnMonoString( &hStringHandle, wString );

		for ( int i = 0; i < PRISONER_MAX; ++i )
		{
			if ( aPrisoners[i] )
			{
				swprintf( wString, L"" );
				AddMonoString( &hStringHandle, wString );
				swprintf( wString, pwTownInfoStrings[15 + i], aPrisoners[i] );
				AddSecondColumnMonoString( &hStringHandle, wString );
			}
		}
	}
}


// adds text to mine info box
void AddTextToMineBox( void )
{
	UINT8 ubMineIndex;
	UINT8 ubTown;
	UINT32 hStringHandle;
	CHAR16 wString[ 64 ];


	ubMineIndex = GetMineIndexForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

	// name of town followed by "mine"
	//swprintf( wString, L"%s %s", pTownNames[ GetTownAssociatedWithMine( ubMineIndex ) ], pwMineStrings[ 0 ] );
	swprintf( wString, L"%s %s", pTownNames[ GetTownAssociatedWithMine( ubMineIndex ) ], MineralsName[gMineStatus[ubMineIndex].ubMineType].sType );
	AddMonoString( &hStringHandle, wString );

	// blank line
	AddMonoString( &hStringHandle, L"" );


	// sector
	AddSectorToBox();

	// mine status
	swprintf( wString, L"%s:", pwMineStrings[ 9 ]);
	AddMonoString( &hStringHandle, wString );

	// check if mine is empty (abandoned) or running out
	if (gMineStatus[ ubMineIndex ].fEmpty)
	{
		// abandonded
		wcscpy( wString, pwMineStrings[ 5 ] );
	}
	else
	if (gMineStatus[ ubMineIndex ].fShutDown)
	{
		// shut down
		wcscpy( wString, pwMineStrings[ 6 ] );
	}
	else
	if (gMineStatus[ ubMineIndex ].fRunningOut)
	{
		// running out
		wcscpy( wString, pwMineStrings[ 7 ] );
	}
	else
	{
		// producing
		wcscpy( wString, pwMineStrings[ 8 ] );
	}
	AddSecondColumnMonoString( &hStringHandle, wString );


	// if still producing
	if (!gMineStatus[ ubMineIndex ].fEmpty)
	{
		// current production
		swprintf( wString, L"%s:", pwMineStrings[ 3 ]);
		AddMonoString( &hStringHandle, wString );

		swprintf( wString, L"%d", PredictDailyIncomeFromAMine( ubMineIndex, TRUE ) );
		InsertCommasForDollarFigure( wString );
		InsertDollarSignInToString( wString );
		AddSecondColumnMonoString( &hStringHandle, wString );


		// potential production
		swprintf( wString, L"%s:", pwMineStrings[ 4 ]);
		AddMonoString( &hStringHandle, wString );

		swprintf( wString, L"%d", GetMaxDailyRemovalFromMine( ubMineIndex ) );
		InsertCommasForDollarFigure( wString );
		InsertDollarSignInToString( wString );
		AddSecondColumnMonoString( &hStringHandle, wString );


		// if potential is not nil
		if (GetMaxPeriodicRemovalFromMine(ubMineIndex) > 0)
		{
			// production rate (current production as a percentage of potential production)
			swprintf( wString, L"%s:", pwMineStrings[ 10 ]);
			AddMonoString( &hStringHandle, wString );
			swprintf( wString, L"%d%%%%", (PredictDailyIncomeFromAMine(ubMineIndex, TRUE) * 100 ) / GetMaxDailyRemovalFromMine(ubMineIndex) );
			AddSecondColumnMonoString( &hStringHandle, wString );
		}


		// town control percentage
		swprintf( wString, L"%s:", pwMineStrings[ 12 ]);
		AddMonoString( &hStringHandle, wString );
		//swprintf( wString, L"%d%%%%", (GetTownSectorsUnderControl( gMineLocation[ ubMineIndex ].bAssociatedTown ) *	100) / GetTownSectorSize( gMineLocation[ ubMineIndex ].bAssociatedTown ));
		swprintf( wString, L"%d%%%%", (GetTownSectorsUnderControl( gMineStatus[ ubMineIndex ].bAssociatedTown ) *	100) / GetTownSectorSize( gMineStatus[ ubMineIndex ].bAssociatedTown ));
		AddSecondColumnMonoString( &hStringHandle, wString );

		//ubTown = gMineLocation[ ubMineIndex ].bAssociatedTown;
		ubTown = gMineStatus[ ubMineIndex ].bAssociatedTown;
		if( gTownLoyalty[ ubTown ].fStarted && gfTownUsesLoyalty[ ubTown ])
		{
			// town loyalty percentage
			swprintf( wString, L"%s:", pwMineStrings[ 13 ]);
			AddMonoString( &hStringHandle, wString );
			//swprintf( wString, L"%d%%%%", gTownLoyalty[ gMineLocation[ ubMineIndex ].bAssociatedTown ].ubRating);
			swprintf( wString, L"%d%%%%", gTownLoyalty[ gMineStatus[ ubMineIndex ].bAssociatedTown ].ubRating);
			AddSecondColumnMonoString( &hStringHandle, wString );
		}

/* gradual monster infestation concept was ditched, now simply IN PRODUCTION or SHUT DOWN
		// percentage of miners working
		swprintf( wString, L"%s:", pwMineStrings[ 14 ]);
		AddMonoString( &hStringHandle, wString );
		swprintf( wString, L"%d%%%%", gubMonsterMineInfestation[ gMineStatus[ ubMineIndex ].bMonsters ]);
		AddSecondColumnMonoString( &hStringHandle, wString );
*/

		// ore type (silver/gold
		swprintf( wString, L"%s:", pwMineStrings[ 11 ]);
		AddMonoString( &hStringHandle, wString );
		AddSecondColumnMonoString( &hStringHandle, MineralsName[gMineStatus[ubMineIndex].ubMineType].sMinerals );
		//AddSecondColumnMonoString( &hStringHandle, (gMineStatus[ubMineIndex].ubMineType == SILVER_MINE) ? pwMineStrings[ 1 ] : pwMineStrings[ 2 ] );
	}


#ifdef _DEBUG
	// dollar amount remaining in mine
	wcscpy( wString, L"Remaining (DEBUG):");
	AddMonoString( &hStringHandle, wString );

	swprintf( wString, L"%d", GetTotalLeftInMine( ubMineIndex ) );
	InsertCommasForDollarFigure( wString );
	InsertDollarSignInToString( wString );
	AddSecondColumnMonoString( &hStringHandle, wString );
#endif

}


void AddTextToBlankSectorBox( void )
{
	UINT32 hStringHandle;
	UINT16 usSectorValue = 0;


	// get the sector value
	usSectorValue = SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

	AssertGE(usSectorValue, 0);
	AssertLT(usSectorValue,256);

	////////////////////////////////////
	// HEADROCK HAM 5:
	// Read and verify XML sector names
	BOOLEAN fSectorHasXMLNames = TRUE;
	CHAR16 zUnexplored[MAX_SECTOR_NAME_LENGTH];
	CHAR16 zExplored[MAX_SECTOR_NAME_LENGTH];
	
	wcscpy( zUnexplored, gzSectorNames[ usSectorValue ][0] );
	wcscpy( zExplored, gzSectorNames[ usSectorValue ][2] );

	if (zUnexplored[0] == 0 || zExplored[0] == 0)
	{
		fSectorHasXMLNames = FALSE;
	}

	if (fSectorHasXMLNames) // ABOVE GROUND XML
	{
		// HEADROCK HAM 3.6: The program can now read custom names from XML for all above-ground sectors.
		// In the event that a specific name or set of names is missing, the program generates a default
		// name as it always has.
		// I've also updated the SAM Site sectors to rely on SamSite.XML data.
		
		BOOLEAN fVisited = (SectorInfo[ usSectorValue ].uiFlags & SF_ALREADY_VISITED);
		BOOLEAN fSAMSiteKnown = FALSE;

		// Test for known SAM Site at this location
		for (UINT16 x=0; x < MAX_NUMBER_OF_SAMS; x++)
		{
			if ( pSamList[x] == usSectorValue )
			{
				if ( fSamSiteFound[ x ] )
				{
					fSAMSiteKnown = TRUE;
					break;
				}
			}
		}

		if (fVisited || fSAMSiteKnown)
		{
			AddMonoString( &hStringHandle, zExplored );
		}
		else
		{
			AddMonoString( &hStringHandle, zUnexplored );
		}
	}
	else // ABOVE GROUND HARDCODED
	{
		switch( usSectorValue )
		{
			case SEC_D2: //Chitzena SAM
				if( !fSamSiteFound[ SAM_SITE_ONE ] )
					AddMonoString( &hStringHandle, pLandTypeStrings[ TROPICS ] );
				else
					AddMonoString( &hStringHandle, pLandTypeStrings[ TROPICS_SAM_SITE ] );
				break;
			case SEC_D15: //Drassen SAM
				if( !fSamSiteFound[ SAM_SITE_TWO ] )
					AddMonoString( &hStringHandle, pLandTypeStrings[ SPARSE ] );
				else
					AddMonoString( &hStringHandle, pLandTypeStrings[ SPARSE_SAM_SITE ] );
				break;
			case SEC_I8: //Cambria SAM
				if( !fSamSiteFound[ SAM_SITE_THREE ] )
					AddMonoString( &hStringHandle, pLandTypeStrings[ SAND ] );
				else
					AddMonoString( &hStringHandle, pLandTypeStrings[ SAND_SAM_SITE ] );
				break;
			// SAM Site 4 in Meduna is within town limits, so it's handled in AddTextToTownBox()

			default:
				AddMonoString( &hStringHandle, pLandTypeStrings[ ( SectorInfo[ usSectorValue ].ubTraversability[ 4 ] ) ] );
				break;
		}
	}

	// blank line
	AddMonoString( &hStringHandle, L"" );

	// sector
	AddSectorToBox();

	// HEADROCK HAM 3.6: Facilities
	AddFacilitiesToBox( bCurrentTownMineSectorX, bCurrentTownMineSectorY, &hStringHandle, FALSE );
}


void AddSectorToBox(void)
{
	CHAR16 wString[ 64 ];
	CHAR16 wString2[ 10 ];
	UINT32 hStringHandle = 0;

	// sector
	swprintf( wString, L"%s:", pwMiscSectorStrings[ 1 ]);
	AddMonoString( &hStringHandle, wString );

	GetShortSectorString( bCurrentTownMineSectorX, bCurrentTownMineSectorY, wString );
	if (bCurrentTownMineSectorZ != 0 )
	{
		swprintf( wString2, L"-%d", bCurrentTownMineSectorZ );
		wcscat( wString, wString2 );
	}

	AddSecondColumnMonoString( &hStringHandle, wString );
}



void AddCommonInfoToBox(void)
{
	CHAR16 wString[ 64 ];
	UINT32 hStringHandle = 0;
	BOOLEAN fKnownSite = FALSE;
	UINT8 ubMilitiaTotal = 0;
	UINT8 ubNumEnemies;
	UINT16 usSectorValue = 0;

	// get the sector value
	usSectorValue = SECTOR( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

	BOOLEAN fVisited = (SectorInfo[ usSectorValue ].uiFlags & SF_ALREADY_VISITED);
	UINT8 ubTownId = GetTownIdForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY );

	// visited sector check
	if ( fVisited )
		fKnownSite = TRUE;

	// known town check, to exclude non-town sectors
	else if( gfHiddenTown[ ubTownId ] && ubTownId != BLANK_SECTOR )
		fKnownSite = TRUE;
	
	// known SAM Site check
	else
	{
		for (UINT16 x=0; x < MAX_NUMBER_OF_SAMS; x++)
		{
			if ( pSamList[x] == usSectorValue )
			{
				if ( fSamSiteFound[ x ] )
				{
					fKnownSite = TRUE;
					break;
				}
			}
		}
	}
	
	/*// ABOVE GROUND HARDCODED
	fKnownSite = TRUE;
	switch( usSectorValue )
	{
		case SEC_D2: //Chitzena SAM
			if( !fSamSiteFound[ SAM_SITE_ONE ] )
				fKnownSite = FALSE;
			break;
		case SEC_D15: //Drassen SAM
			if( !fSamSiteFound[ SAM_SITE_TWO ] )
				fKnownSite = FALSE;
			break;
		case SEC_I8: //Cambria SAM
			if( !fSamSiteFound[ SAM_SITE_THREE ] )
				fKnownSite = FALSE;
			break;
		// SAM Site 4 in Meduna is within town limits, so it's always controllable
		default:
			break;
	}
	*/

	// in sector where militia can be trained,
	// control of the sector matters, display who controls this sector.	Map brightness no longer gives this!
	if ( MilitiaTrainingAllowedInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, 0 ) && fKnownSite )
	{
		// controlled:
		swprintf( wString, L"%s:", pwMiscSectorStrings[ 4 ] );
		AddMonoString( &hStringHandle, wString );

		// No/Yes
		swprintf( wString, L"%s", pwMiscSectorStrings[ ( StrategicMap[ CALCULATE_STRATEGIC_INDEX( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) ].fEnemyControlled ) ? 6 : 5 ] );
		AddSecondColumnMonoString( &hStringHandle, wString );


		// militia - is there any?
		swprintf( wString, L"%s:", pwTownInfoStrings[ 11 ] );
		AddMonoString( &hStringHandle, wString );

		ubMilitiaTotal = NumNonPlayerTeamMembersInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, MILITIA_TEAM );
		if (ubMilitiaTotal > 0)
		{
			// some militia, show total & their breakdown by level
			swprintf( wString, L"%d (%d/%d/%d)", ubMilitiaTotal,
												MilitiaInSectorOfRank(bCurrentTownMineSectorX, bCurrentTownMineSectorY, GREEN_MILITIA),
												MilitiaInSectorOfRank(bCurrentTownMineSectorX, bCurrentTownMineSectorY, REGULAR_MILITIA),
												MilitiaInSectorOfRank(bCurrentTownMineSectorX, bCurrentTownMineSectorY, ELITE_MILITIA));
			AddSecondColumnMonoString( &hStringHandle, wString );
		}
		else
		{
			// no militia: don't bother displaying level breakdown
			wcscpy( wString, L"0");
			AddSecondColumnMonoString( &hStringHandle, wString );
		}

		// HEADROCK HAM 3.6: Only show these for sectors that have a training facility
		BOOLEAN fMilitiaTrainingAllowed = FALSE;
		BOOLEAN fMobileTrainingAllowed = FALSE;

		// percentage of current militia squad training completed
		swprintf( wString, L"%s:", pwTownInfoStrings[ 10 ] );
		AddMonoString( &hStringHandle, wString );

		// Sector contains Militia training facility?
		for (UINT8 ubCounter = 0; ubCounter < MAX_NUM_FACILITY_TYPES; ubCounter++)
		{
			if (gFacilityLocations[usSectorValue][ubCounter].fFacilityHere)
			{
				if (gFacilityTypes[ubCounter].ubMilitiaTrainersAllowed)
				{
					fMilitiaTrainingAllowed = TRUE;
				}
			}
		}
		if (fMilitiaTrainingAllowed)
		{
			// Show percent completed
			swprintf( wString, L"%d%%%%", SectorInfo[ usSectorValue ].ubMilitiaTrainingPercentDone );
			AddSecondColumnMonoString( &hStringHandle, wString );
		}
		else
		{
			// Show N/A
			AddSecondColumnMonoString( &hStringHandle, New113HAMMessage[19] );
		}

		// HEADROCK HAM 3.6: percentage of current Mobile Militia squad training completed
		swprintf( wString, L"%s:", pwTownInfoStrings[ 12 ] );
		AddMonoString( &hStringHandle, wString );

		// Sector contains Mobile training facility?
		for (UINT8 ubCounter = 0; ubCounter < MAX_NUM_FACILITY_TYPES; ubCounter++)
		{
			if (gFacilityLocations[usSectorValue][ubCounter].fFacilityHere)
			{
				if (gFacilityTypes[ubCounter].ubMobileMilitiaTrainersAllowed)
				{
					fMobileTrainingAllowed = TRUE;
				}
			}
		}

		if (fMobileTrainingAllowed)
		{
			// Show percentage completed
			swprintf( wString, L"%d%%%%", SectorInfo[ usSectorValue ].ubMobileMilitiaTrainingPercentDone );
			AddSecondColumnMonoString( &hStringHandle, wString );
		}
		else
		{
			// Show N/A
			AddSecondColumnMonoString( &hStringHandle, New113HAMMessage[19] );
		}
	}


	// enemy forces
	swprintf( wString, L"%s:", pwMiscSectorStrings[ 0 ] );
	AddMonoString( &hStringHandle, wString );

	// how many are there, really?
	ubNumEnemies = NumNonPlayerTeamMembersInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, ENEMY_TEAM );
	UINT8 enemyfreeslots = NumFreeSlots( ENEMY_TEAM );
	AssertGE( gGameExternalOptions.ubGameMaximumNumberOfEnemies, enemyfreeslots );
	unsigned numEnemiesOnMap = gGameExternalOptions.ubGameMaximumNumberOfEnemies - enemyfreeslots;

	switch ( WhatPlayerKnowsAboutEnemiesInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY ) )
	{
		case KNOWS_NOTHING:
			// show "Unknown"
			wcscpy(wString, pwMiscSectorStrings[ 3 ] );
			break;

		// HEADROCK HAM 5: New Case
		case KNOWS_THEYRE_THERE:
		case KNOWS_THEYRE_THERE_AND_WHERE_GOING:
			// if there are any there
			if ( ubNumEnemies > 0 )
			{
				// show "?", but not exactly how many
				wcscpy(wString, L"?" );
			}
			else
			{
				// we know there aren't any (or we'd be seing them on map, too)
				wcscpy(wString, L"0" );
			}
			break;

		// HEADROCK HAM 5: New case
		case KNOWS_HOW_MANY:
		case KNOWS_HOW_MANY_AND_WHERE_GOING:
			// show exactly how many
			if (numEnemiesOnMap != ubNumEnemies)
				swprintf( wString, L"%d (%d)", numEnemiesOnMap, ubNumEnemies );
			else
				swprintf( wString, L"%d", ubNumEnemies );
			break;
	}

	AddSecondColumnMonoString( &hStringHandle, wString );

	// Flugente: if this is a SAM site we know about, display (hack-)status
	for ( UINT16 x = 0; x < MAX_NUMBER_OF_SAMS; ++x )
	{
		if ( pSamList[x] == usSectorValue && fSamSiteFound[x] )
		{
			swprintf( wString, pwMiscSectorStrings[7] );
			AddMonoString( &hStringHandle, wString );

			swprintf( wString, L"%d / %d", StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( usSectorValue )].bSAMCondition, StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( usSectorValue )].sSamHackStatus );
			AddSecondColumnMonoString( &hStringHandle, wString );

			break;
		}
	}
}


void AddItemsInSectorToBox(void)
{
	CHAR16 wString[ 64 ];
	UINT32 hStringHandle = 0;

	// items in sector (this works even for underground)

	swprintf( wString, L"%s:", pwMiscSectorStrings[ 2 ] );
	AddMonoString( &hStringHandle, wString );

//	swprintf( wString, L"%d", GetSizeOfStashInSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, bCurrentTownMineSectorZ, FALSE ));
	swprintf( wString, L"%d", GetNumberOfVisibleWorldItemsFromSectorStructureForSector( bCurrentTownMineSectorX, bCurrentTownMineSectorY, bCurrentTownMineSectorZ ));
	AddSecondColumnMonoString( &hStringHandle, wString );
}


void PositionTownMineInfoBox( void )
{
	// position town mine info box
	SGPRect pDimensions;
	SGPPoint pPosition;
	INT16 sX =0, sY = 0;

	// position the box based on x and y of the selected sector
	GetScreenXYFromMapXY( bCurrentTownMineSectorX, bCurrentTownMineSectorY, &sX, &sY);

	// set box position
	pPosition.iX = sX;
	pPosition.iY = sY;

	// set new position
	SetBoxPosition( ghTownMineBox, pPosition );

	// get box size
	GetBoxSize( ghTownMineBox, &pDimensions );

	// get position
	GetBoxPosition( ghTownMineBox, &pPosition);

	if( pDimensions.iRight < ( sTotalButtonWidth + 30 ) )
	{
		SpecifyBoxMinWidth( ghTownMineBox, ( sTotalButtonWidth + 30 )	);
		pDimensions.iRight = sTotalButtonWidth + 30;
	}

	// now position box - the x axis
	if( pPosition.iX < MapScreenRect.iLeft )
	{
		pPosition.iX = MapScreenRect.iLeft + 5;
	}

	if( pPosition.iX + pDimensions.iRight > MapScreenRect.iRight )
	{
		pPosition.iX = MapScreenRect.iRight - pDimensions.iRight - 5;
	}


	// position - the y axis
	if( pPosition.iY < MapScreenRect.iTop )
	{
		pPosition.iY = MapScreenRect.iTop + 5;
	}

	if(	pPosition.iY + pDimensions.iBottom > MapScreenRect.iBottom )
	{
		pPosition.iY = MapScreenRect.iBottom - pDimensions.iBottom - 8;
	}
	
	// reset position
	SetBoxPosition( ghTownMineBox, pPosition );
}


void AddInventoryButtonForMapPopUpBox( void )
{
	INT16 sX, sY;
	SGPRect pDimensions;
	SGPPoint pPosition;
	VOBJECT_DESC VObjectDesc;
	UINT32 uiObject;
	ETRLEObject	*pTrav;
	INT16 sWidthA = 0, sWidthB = 0, sTotalBoxWidth = 0;
	HVOBJECT hHandle;
	
	// load the button
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\mapinvbtns.sti", VObjectDesc.ImageFile);
	AddVideoObject( &VObjectDesc, &uiObject );

	// Calculate smily face positions...
	GetVideoObject( &hHandle, uiObject );
	pTrav = &(hHandle->pETRLEObject[ 0 ] );

	sWidthA = pTrav->usWidth;

	pTrav = &(hHandle->pETRLEObject[ 1 ] );
	sWidthB = pTrav->usWidth;

	sTotalBoxWidth = sTotalButtonWidth;

	GetBoxSize( ghTownMineBox , &pDimensions );
	GetBoxPosition( ghTownMineBox, &pPosition );

	sX = pPosition.iX + ( pDimensions.iRight - sTotalBoxWidth ) / 3;
	sY = pPosition.iY + pDimensions.iBottom - ( (	BOX_BUTTON_HEIGHT + 5 ) );

	guiMapButtonInventoryImage[0] =	LoadButtonImage( "INTERFACE\\mapinvbtns.sti" ,-1,0,-1,2,-1 );

	guiMapButtonInventory[0] = CreateIconAndTextButton( guiMapButtonInventoryImage[0], pMapPopUpInventoryText[ 0 ], BLOCKFONT2,
														FONT_WHITE, FONT_BLACK,
														FONT_WHITE, FONT_BLACK,
														TEXT_CJUSTIFIED,
														(INT16)(sX ), (INT16)( sY ), BUTTON_TOGGLE , MSYS_PRIORITY_HIGHEST - 1,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MapTownMineInventoryButtonCallBack );


	sX = sX + sWidthA + ( pDimensions.iRight - sTotalBoxWidth ) / 3;
	sY = pPosition.iY + pDimensions.iBottom - ( ( BOX_BUTTON_HEIGHT + 5) );

	guiMapButtonInventoryImage[1] =	LoadButtonImage( "INTERFACE\\mapinvbtns.sti" ,-1,1,-1,3,-1 );

	guiMapButtonInventory[1] = CreateIconAndTextButton( guiMapButtonInventoryImage[1], pMapPopUpInventoryText[ 1 ], BLOCKFONT2,
														FONT_WHITE, FONT_BLACK,
														FONT_WHITE, FONT_BLACK,
														TEXT_CJUSTIFIED,
														(INT16)(sX ), (INT16)( sY ), BUTTON_TOGGLE , MSYS_PRIORITY_HIGHEST - 1,
														DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)MapTownMineExitButtonCallBack );
	
	// delete video object
	DeleteVideoObjectFromIndex( uiObject );
}

void RemoveInventoryButtonForMapPopUpBox( void )
{
	// get rid of button
	RemoveButton( guiMapButtonInventory[0] );
	UnloadButtonImage( guiMapButtonInventoryImage[0] );

	RemoveButton( guiMapButtonInventory[1] );
	UnloadButtonImage( guiMapButtonInventoryImage[1] );
}


void MapTownMineInventoryButtonCallBack( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// done
			fShowMapInventoryPool = TRUE;
			fMapPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fShowTownInfo = FALSE;

			//since we are bring up the sector inventory, check to see if the help screen should come up
			if( ShouldTheHelpScreenComeUp( HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY, FALSE ) )
			{
				//normally this is handled in the screen handler, we have to set up a little different this time around
				ShouldTheHelpScreenComeUp( HELP_SCREEN_MAPSCREEN_SECTOR_INVENTORY, TRUE );
			}
		}
	}
}

void MapTownMineExitButtonCallBack( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// done
			fMapPanelDirty = TRUE;
			fMapScreenBottomDirty = TRUE;
			fShowTownInfo = FALSE;

		}
	}
}

// get the min width of the town mine info pop up box
void MinWidthOfTownMineInfoBox( void )
{
	HVOBJECT hHandle;
	VOBJECT_DESC VObjectDesc;
	INT16 sWidthA = 0, sWidthB = 0, sTotalBoxWidth = 0;
	UINT32 uiObject;
	ETRLEObject	*pTrav;

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\mapinvbtns.sti", VObjectDesc.ImageFile);
	AddVideoObject( &VObjectDesc, &uiObject );

	// Calculate smily face positions...
	GetVideoObject( &hHandle, uiObject );
	pTrav = &(hHandle->pETRLEObject[ 0 ] );

	sWidthA = pTrav->usWidth;

	pTrav = &(hHandle->pETRLEObject[ 1 ] );
	sWidthB = pTrav->usWidth;

	sTotalBoxWidth = sWidthA + sWidthB;
	sTotalButtonWidth = sTotalBoxWidth;

	// delete video object
	DeleteVideoObjectFromIndex( uiObject );

	return;
}
