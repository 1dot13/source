#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif


#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include "worlddef.h"
	#include "worldman.h"
	#include "smooth.h"
	#include "newsmooth.h"
	#include "edit_sys.h"
	#include "editscreen.h"
	#include "selectwin.h"
	#include "EditorTerrain.h"
	#include "EditorBuildings.h"
	#include "EditorMercs.h"
	#include "EditorDefines.h"
	#include "Smoothing Utils.h"
	#include "Cursor Modes.h"
	#include "Render Fun.h"
	#include "Isometric Utils.h"
	#include "Editor Undo.h"
	#include "Exit Grids.h"
	#include "environment.h"
	#include "Simple Render Utils.h"
	#include "Road Smoothing.h"
#endif

BOOLEAN PasteHigherTextureFromRadius( INT32 iMapIndex, UINT32 uiNewType, UINT8 ubRadius );
BOOLEAN PasteExistingTexture( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN PasteExistingTextureFromRadius( INT32 iMapIndex, UINT16 usIndex, UINT8 ubRadius );
BOOLEAN SetLowerLandIndexWithRadius( INT32 iMapIndex, UINT32 uiNewType, UINT8 ubRadius, BOOLEAN fReplace );

void PasteTextureEx( INT32 sGridNo, UINT16 usType );
void PasteTextureFromRadiusEx( INT32 sGridNo, UINT16 usType, UINT8 ubRadius );


BOOLEAN			gfWarning = FALSE;

BOOLEAN			gfDoFill = FALSE;
UINT16			CurrentPaste = NO_TILE;
UINT16			gDebrisPaste = NO_TILE;
UINT16			gChangeElevation = FALSE;
UINT16			CurrentStruct = NO_TILE;
UINT32			gDoBanks = NO_BANKS;
UINT32			gDoCliffs = NO_CLIFFS;



//---------------------------------------------------------------------------------------------------------------
//	QuickEraseMapTile
//
//	Performs ersing operation when the DEL key is hit in the editor
//
void QuickEraseMapTile( INT32 iMapIndex )
{
	if ( iMapIndex >= 0x80000000 )
		return;
	AddToUndoList( iMapIndex );
	DeleteStuffFromMapTile( iMapIndex );
	MarkWorldDirty();
}


//---------------------------------------------------------------------------------------------------------------
//	DeleteStuffFromMapTile
//
//	Common delete function for both QuickEraseMapTile and EraseMapTile
//
void DeleteStuffFromMapTile( INT32 iMapIndex )
{
	//UINT16		usUseIndex;
	//UINT16		usType;
	//UINT32		uiCheckType;
	//UINT16		usDummy;

	//GetTileType( gpWorldLevelData[ iMapIndex ].pLandHead->usIndex, &uiCheckType );
	//RemoveLand( iMapIndex, gpWorldLevelData[ iMapIndex ].pLandHead->usIndex );
	//SmoothTerrainRadius( iMapIndex, uiCheckType, 1, TRUE );

	RemoveExitGridFromWorld( iMapIndex );
	RemoveAllStructsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllObjectsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllLandsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllRoofsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllOnRoofsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	RemoveAllTopmostsOfTypeRange( iMapIndex, FIRSTTEXTURE, WIREFRAMES );
	PasteRoomNumber( iMapIndex, 0 );
}


//---------------------------------------------------------------------------------------------------------------
//	EraseMapTile
//
//	Generic tile erasing function. Erases things from the world depending on the current drawing mode
//
void EraseMapTile( INT32 iMapIndex )
{
	INT32			iEraseMode;
	UINT32		uiCheckType;
	if ( iMapIndex >= 0x80000000 )
		return;

	// Figure out what it is we are trying to erase
	iEraseMode = iDrawMode - DRAW_MODE_ERASE;

	switch ( iEraseMode )
	{
		case DRAW_MODE_NORTHPOINT:
		case DRAW_MODE_WESTPOINT:
		case DRAW_MODE_EASTPOINT:
		case DRAW_MODE_SOUTHPOINT:
		case DRAW_MODE_CENTERPOINT:
		case DRAW_MODE_ISOLATEDPOINT:
			SpecifyEntryPoint( iMapIndex );
			break;
		case DRAW_MODE_EXITGRID:
			AddToUndoList( iMapIndex );
			RemoveExitGridFromWorld( iMapIndex );
			RemoveTopmost( iMapIndex, FIRSTPOINTERS8 );
			break;
		case DRAW_MODE_GROUND:
			// Is there ground on this tile? if not, get out o here
			if ( gpWorldLevelData[ iMapIndex ].pLandHead == NULL )
				break;

			// is there only 1 ground tile here? if so, get out o here
			if ( gpWorldLevelData[ iMapIndex ].pLandHead->pNext == NULL )
				break;
			AddToUndoList( iMapIndex );
			GetTileType( gpWorldLevelData[ iMapIndex ].pLandHead->usIndex, &uiCheckType );
			RemoveLand( iMapIndex, gpWorldLevelData[ iMapIndex ].pLandHead->usIndex );
			SmoothTerrainRadius( iMapIndex, uiCheckType, 1, TRUE );
			break;
		case DRAW_MODE_HIGH_GROUND://dnl ch1 210909
			gpWorldLevelData[iMapIndex].sHeight = 0;
			RemoveTopmost(iMapIndex, FIRSTPOINTERS6);
			break;
		case DRAW_MODE_OSTRUCTS:
		case DRAW_MODE_OSTRUCTS1:
		case DRAW_MODE_OSTRUCTS2:
			AddToUndoList( iMapIndex );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTOSTRUCT, LASTOSTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTVEHICLE, SECONDVEHICLE );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTDEBRISSTRUCT, SECONDDEBRISSTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, NINTHOSTRUCT, TENTHOSTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTLARGEEXPDEBRIS, SECONDLARGEEXPDEBRIS );
			RemoveAllObjectsOfTypeRange( iMapIndex, DEBRIS2MISC, DEBRIS2MISC );
			RemoveAllObjectsOfTypeRange( iMapIndex, ANOTHERDEBRIS, ANOTHERDEBRIS );
			break;
		case DRAW_MODE_DEBRIS:
			AddToUndoList( iMapIndex );
			RemoveAllObjectsOfTypeRange( iMapIndex, DEBRISROCKS, LASTDEBRIS );
			RemoveAllObjectsOfTypeRange( iMapIndex, DEBRIS2MISC, DEBRIS2MISC );
			RemoveAllObjectsOfTypeRange( iMapIndex, ANOTHERDEBRIS, ANOTHERDEBRIS );
			break;
		case DRAW_MODE_BANKS:
			AddToUndoList( iMapIndex );
			RemoveAllObjectsOfTypeRange( iMapIndex, FIRSTROAD, LASTROAD );
			// Note, for this routine, cliffs are considered a subset of banks
			RemoveAllStructsOfTypeRange( iMapIndex, ANIOSTRUCT, ANIOSTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTCLIFF, LASTBANKS );
			RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTCLIFFSHADOW, LASTCLIFFSHADOW );
			RemoveAllObjectsOfTypeRange( iMapIndex, FIRSTCLIFFHANG, LASTCLIFFHANG );
			RemoveAllStructsOfTypeRange( iMapIndex, FENCESTRUCT, FENCESTRUCT );
			RemoveAllShadowsOfTypeRange( iMapIndex, FENCESHADOW, FENCESHADOW );
			break;
		case DRAW_MODE_FLOORS:
			AddToUndoList( iMapIndex );
			RemoveAllLandsOfTypeRange( iMapIndex, FIRSTFLOOR, LASTFLOOR );
			break;
		case DRAW_MODE_ROOFS:
		case DRAW_MODE_NEWROOF:
			AddToUndoList( iMapIndex );
			RemoveAllRoofsOfTypeRange( iMapIndex, FIRSTTEXTURE, LASTITEM );
			RemoveAllOnRoofsOfTypeRange( iMapIndex, FIRSTTEXTURE, LASTITEM );
			break;
		case DRAW_MODE_WALLS:
		case DRAW_MODE_DOORS:
		case DRAW_MODE_WINDOWS:
		case DRAW_MODE_BROKEN_WALLS:
			AddToUndoList( iMapIndex );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
			RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTDOOR, LASTDOOR );
			RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTDOORSHADOW, LASTDOORSHADOW );
			break;
		case DRAW_MODE_DECOR:
		case DRAW_MODE_DECALS:
		case DRAW_MODE_ROOM:
		case DRAW_MODE_TOILET:
			AddToUndoList( iMapIndex );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTWALLDECAL, LASTWALLDECAL );
			RemoveAllStructsOfTypeRange( iMapIndex, FIFTHWALLDECAL, EIGTHWALLDECAL );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTDECORATIONS, LASTDECORATIONS );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTISTRUCT, LASTISTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, FIFTHISTRUCT, EIGHTISTRUCT );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTSWITCHES, FIRSTSWITCHES );
			break;
		case DRAW_MODE_CAVES:
			AddToUndoList( iMapIndex );
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTWALL, LASTWALL );
			break;
		case DRAW_MODE_ROOMNUM:
			PasteRoomNumber( iMapIndex, 0 );
			break;
		case DRAW_MODE_ROADS:
			RemoveAllObjectsOfTypeRange( iMapIndex, ROADPIECES, ROADPIECES );
			break;
		default:
			//DeleteStuffFromMapTile( iMapIndex );
			break;
	}
}


//---------------------------------------------------------------------------------------------------------------
//	PasteDebris
//
//	Place some "debris" on the map at the current mouse coordinates. This function is called repeatedly if
//	the current brush size is larger than 1 tile.
//
void PasteDebris( INT32 iMapIndex )
{
	UINT16				usUseIndex;
	UINT16				usUseObjIndex;
	INT32					iRandSelIndex;

	// Get selection list for debris
	pSelList = SelDebris;
	pNumSelList = &iNumDebrisSelected;

	if ( iMapIndex < 0x80000000 )
	{
		AddToUndoList( iMapIndex );

		// Remove any debris that is currently at this map location
		if ( gpWorldLevelData[ iMapIndex ].pObjectHead != NULL )
		{
			RemoveAllObjectsOfTypeRange( iMapIndex, ANOTHERDEBRIS, FIRSTPOINTERS - 1 );
		}

		// Get a random debris from current selection
		iRandSelIndex = GetRandomSelection( );
		if ( iRandSelIndex != -1 )
		{
			// Add debris to the world
			usUseIndex = pSelList[ iRandSelIndex ].usIndex;
			usUseObjIndex = (UINT16)pSelList[ iRandSelIndex ].uiObject;

			AddObjectToTail( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
		}
	}
}


void PasteSingleWall( INT32 iMapIndex )
{
	pSelList = SelSingleWall;
	pNumSelList = &iNumWallsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleDoor( INT32 iMapIndex )
{
	pSelList = SelSingleDoor;
	pNumSelList = &iNumDoorsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleWindow( INT32 iMapIndex )
{
	pSelList = SelSingleWindow;
	pNumSelList = &iNumWindowsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleRoof( INT32 iMapIndex )
{
	pSelList = SelSingleRoof;
	pNumSelList = &iNumRoofsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteRoomNumber( INT32 iMapIndex, UINT8 ubRoomNumber )
{
	if( gubWorldRoomInfo[ iMapIndex ] != ubRoomNumber )
	{
		AddToUndoList( iMapIndex );
		gubWorldRoomInfo[ iMapIndex ] = ubRoomNumber;
	}
}

void PasteSingleBrokenWall( INT32 iMapIndex )
{
	UINT16 usIndex, usObjIndex, usTileIndex, usWallOrientation;

	pSelList = SelSingleBrokenWall;
	pNumSelList = &iNumBrokenWallsSelected;

	usIndex = pSelList[ iCurBank ].usIndex;
	usObjIndex = (UINT16)pSelList[ iCurBank ].uiObject;
	usTileIndex = GetTileIndexFromTypeSubIndex( usObjIndex, usIndex, &usTileIndex );
	GetWallOrientation( usTileIndex, &usWallOrientation );
	if( usWallOrientation == INSIDE_TOP_LEFT || usWallOrientation == INSIDE_TOP_RIGHT )
		EraseHorizontalWall( iMapIndex );
	else
		EraseVerticalWall( iMapIndex );

	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleDecoration( INT32 iMapIndex )
{
	pSelList = SelSingleDecor;
	pNumSelList = &iNumDecorSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleDecal( INT32 iMapIndex )
{
	pSelList = SelSingleDecal;
	pNumSelList = &iNumDecalsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleFloor( INT32 iMapIndex )
{
	pSelList = SelSingleFloor;
	pNumSelList = &iNumFloorsSelected;
	PasteSingleWallCommon( iMapIndex );
}

void PasteSingleToilet( INT32 iMapIndex )
{
	pSelList = SelSingleToilet;
	pNumSelList = &iNumToiletsSelected;
	PasteSingleWallCommon( iMapIndex );
}

//---------------------------------------------------------------------------------------------------------------
//	PasteSingleWallCommon
//
//	Common paste routine for PasteSingleWall, PasteSingleDoor, PasteSingleDecoration, and
//	PasteSingleDecor (above).
//
void PasteSingleWallCommon( INT32 iMapIndex )
{
	UINT16				usUseIndex;
	UINT16				usUseObjIndex;
	UINT16				usTempIndex;

	if ( iMapIndex < 0x80000000 )
	{
		AddToUndoList( iMapIndex );

		usUseIndex = pSelList[ iCurBank ].usIndex;
		usUseObjIndex = (UINT16)pSelList[ iCurBank ].uiObject;

		// TEMP STUFF FOR ONROOF THINGS!
		if ( (usUseObjIndex >= FIRSTONROOF) && (usUseObjIndex <= SECONDONROOF ) )
		{
			// WANNE: Disabled the next line, because if makes placing sandbags on rooftops impossible!
			//dnl Remove all onroof things before placing new one to avoid stacking problems of same element
			//RemoveAllOnRoofsOfTypeRange( iMapIndex, FIRSTONROOF, SECONDONROOF );

			// Add to onroof section!
			AddOnRoofToTail( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );

			if ( gTileDatabase[ (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) ].sBuddyNum != -1 )
			{
				AddOnRoofToTail( iMapIndex, gTileDatabase[ (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) ].sBuddyNum );
			}
			return;
		}

		// Make sure A-frames are on roof level!
		if ( ( usUseIndex >= WALL_AFRAME_START && usUseIndex <= WALL_AFRAME_END ) )
		{
			AddRoofToTail( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
			return;
		}

		if ( (usUseObjIndex >= FIRSTDOOR) && (usUseObjIndex <= LASTDOOR) )
		{
			// PLace shadow for doors
			if( !gfBasement )
				AddExclusiveShadow( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex - FIRSTDOOR + FIRSTDOORSHADOW ] + usUseIndex	) );
		}

		// Is it a wall?
		if ( ((usUseObjIndex >= FIRSTWALL) && (usUseObjIndex <= LASTWALL)) )
		{
			// ATE		If it is a wall shadow, place differenty!
			if ( usUseIndex == 29 || usUseIndex == 30 )
			{
				if( !gfBasement )
					AddExclusiveShadow( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
			}
			else
			{
				// Slap down wall/window/door/decoration (no smoothing)
				AddWallToStructLayer( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex), TRUE );
			}
		}
		// Is it a door/window/decoration?
		else if ( ( (usUseObjIndex >= FIRSTDOOR) && (usUseObjIndex <= LASTDOOR)) ||
				((usUseObjIndex >= FIRSTDECORATIONS) && (usUseObjIndex <= LASTDECORATIONS)) )
		{
			// Slap down wall/window/door/decoration (no smoothing)
			AddWallToStructLayer( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex), TRUE );
		}
		else if ( ((usUseObjIndex >= FIRSTROOF) && (usUseObjIndex <= LASTROOF))	||
							((usUseObjIndex >= FIRSTSLANTROOF) && (usUseObjIndex <= LASTSLANTROOF)) )
		{
			// Put a roof on this tile (even if nothing else is here)
			RemoveAllRoofsOfTypeRange( iMapIndex, FIRSTROOF, LASTROOF );
			AddRoofToTail( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
		}
		else if ( (usUseObjIndex >= FIRSTFLOOR) && (usUseObjIndex <= LASTFLOOR) )
		{
			// Drop a floor on this tile
			if ( TypeExistsInLandLayer( iMapIndex, usUseObjIndex, &usTempIndex ) )
				RemoveLand( iMapIndex, usTempIndex );

			AddLandToHead( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
		}
		else if( usUseObjIndex >= FIRSTWALLDECAL && usUseObjIndex <= LASTWALLDECAL ||
						usUseObjIndex >= FIFTHWALLDECAL && usUseObjIndex <= EIGTHWALLDECAL )
		{
			// Plop a decal here
			RemoveAllStructsOfTypeRange( iMapIndex, FIRSTWALLDECAL, LASTWALLDECAL );
			RemoveAllStructsOfTypeRange( iMapIndex, FIFTHWALLDECAL, EIGTHWALLDECAL );

			AddStructToTail( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );
		}
		else if ( usUseObjIndex >= FIRSTISTRUCT && usUseObjIndex <= LASTISTRUCT ||
							usUseObjIndex >= FIFTHISTRUCT && usUseObjIndex <= EIGHTISTRUCT )
		{
			AddStructToHead( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );
		}
		else if( usUseObjIndex == FIRSTSWITCHES )
		{
			AddStructToTail( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );
		}

 	}
}


//---------------------------------------------------------------------------------------------------------------
//	GetRandomIndexByRange
//
//	Returns a randomly picked object index given the current selection list, and the type or types of objects we want
//	from that list. If no such objects are in the list, we return 0xffff (-1).
UINT16 GetRandomIndexByRange( UINT16 usRangeStart, UINT16 usRangeEnd )
{
	UINT16	usPickList[50];
	UINT16	usNumInPickList;
	UINT16	usWhich;
	UINT16	usObject;
	// Get a list of valid object to select from
	usNumInPickList = 0;
	for ( usWhich = 0; usWhich < *pNumSelList; usWhich++ )
	{
		usObject = (UINT16)pSelList[ usWhich ].uiObject;
		if ( (usObject >= usRangeStart) && (usObject <= usRangeEnd) )
		{
			usPickList[ usNumInPickList ] = usObject;
			usNumInPickList++;
		}
	}
	return ( usNumInPickList ) ? usPickList[ rand() % usNumInPickList ] : 0xffff;
}

UINT16 GetRandomTypeByRange( UINT16 usRangeStart, UINT16 usRangeEnd )
{
	UINT16	usPickList[50];
	UINT16	usNumInPickList;
	UINT16	i;
	UINT16	usObject;
	UINT32	uiType;
	// Get a list of valid object to select from
	usNumInPickList = 0;
	for ( i = 0; i < *pNumSelList; i++ )
	{
		usObject = (UINT16)pSelList[ i ].uiObject;
		if ( (usObject >= usRangeStart) && (usObject <= usRangeEnd) )
		{
			GetTileType( usObject, &uiType );
			usPickList[ usNumInPickList ] = (UINT16)uiType;
			usNumInPickList++;
		}
	}
	return ( usNumInPickList ) ? usPickList[ rand() % usNumInPickList ] : 0xffff;
}


//---------------------------------------------------------------------------------------------------------------
//	PasteStructure			(See also PasteStructure1, PasteStructure2, and PasteStructureCommon)
//
//	Puts a structure (trees, trucks, etc.) into the world
//
void PasteStructure( INT32 iMapIndex )
{
	pSelList = SelOStructs;
	pNumSelList = &iNumOStructsSelected;

	PasteStructureCommon( iMapIndex );
}

//---------------------------------------------------------------------------------------------------------------
//	PasteStructure1			(See also PasteStructure, PasteStructure2, and PasteStructureCommon)
//
//	Puts a structure (trees, trucks, etc.) into the world
//
void PasteStructure1( INT32 iMapIndex )
{
	pSelList = SelOStructs1;
	pNumSelList = &iNumOStructs1Selected;

	PasteStructureCommon( iMapIndex );
}

//---------------------------------------------------------------------------------------------------------------
//	PasteStructure2			(See also PasteStructure, PasteStructure1, and PasteStructureCommon)
//
//	Puts a structure (trees, trucks, etc.) into the world
//
void PasteStructure2( INT32 iMapIndex )
{
	pSelList = SelOStructs2;
	pNumSelList = &iNumOStructs2Selected;

	PasteStructureCommon( iMapIndex );
}


//---------------------------------------------------------------------------------------------------------------
//	PasteStructureCommon
//
//	This is the main (common) structure pasting function. The above three wrappers are only required because they
//	each use different selection lists. Other than that, they are COMPLETELY identical.
//
void PasteStructureCommon( INT32 iMapIndex )
{
	UINT32				fHeadType;
	UINT16				usUseIndex;
	UINT16				usUseObjIndex;
	INT32					iRandSelIndex;
	BOOLEAN				fOkayToAdd;

	if ( iMapIndex < 0x80000000 )
	{
	/*
		if ( gpWorldLevelData[ iMapIndex ].pStructHead != NULL )
		{
			fDoPaste = FALSE;
		}
		else
		{
			// Nothing is here, paste
			fDoPaste = TRUE;
		}
*/
		if ( /*fDoPaste &&*/ iMapIndex < 0x80000000 )
		{
			iRandSelIndex = GetRandomSelection( );
			if ( iRandSelIndex == -1 )
			{
				return;
			}

			AddToUndoList( iMapIndex );

			usUseIndex = pSelList[ iRandSelIndex ].usIndex;
			usUseObjIndex = (UINT16)pSelList[ iRandSelIndex ].uiObject;

			// Check with Structure Database (aka ODB) if we can put the object here!
			fOkayToAdd = OkayToAddStructureToWorld( iMapIndex, 0, gTileDatabase[ (gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) ].pDBStructureRef, INVALID_STRUCTURE_ID );
			if ( fOkayToAdd || (gTileDatabase[ (gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) ].pDBStructureRef == NULL) )
			{
				//dnl Remove existing structure before adding the same, seems to solve problem with stacking but still need test to be sure that is not removed something what should stay
				RemoveStruct( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );//dnl
				// Actual structure info is added by the functions below
				AddStructToHead( iMapIndex, (UINT16)(gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex) );
				// For now, adjust to shadows by a hard-coded amount,

				// Add mask if in long grass
				GetLandHeadType( iMapIndex, &fHeadType	);
			}
		}
	}
	else if ( CurrentStruct == ERASE_TILE && iMapIndex < 0x80000000 )
	{
		RemoveAllStructsOfTypeRange( iMapIndex, FIRSTOSTRUCT, LASTOSTRUCT );
		RemoveAllShadowsOfTypeRange( iMapIndex, FIRSTSHADOW, LASTSHADOW );
	}
}


//---------------------------------------------------------------------------------------------------------------
//	PasteBanks
//
//	Places a river bank or cliff into the world
//
void PasteBanks( INT32 iMapIndex, UINT16 usStructIndex , BOOLEAN fReplace)
{
	BOOLEAN				fDoPaste = FALSE;
	UINT16				usUseIndex;
	UINT16				usUseObjIndex;
	UINT16 usIndex;

	pSelList = SelBanks;
	pNumSelList = &iNumBanksSelected;

	usUseIndex = pSelList[ iCurBank ].usIndex;
	usUseObjIndex = (UINT16)pSelList[ iCurBank ].uiObject;

	if ( iMapIndex < 0x80000000 )
	{
		fDoPaste = TRUE;

		if (	gpWorldLevelData[ iMapIndex ].pStructHead != NULL )
		{
				// CHECK IF THE SAME TILE IS HERE
				if ( gpWorldLevelData[ iMapIndex ].pStructHead->usIndex == (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) )
				{
					fDoPaste = FALSE;
				}
		}
		else
		{
			// Nothing is here, paste
			fDoPaste = TRUE;
		}

		if ( fDoPaste )
		{

			usIndex = gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex;

			AddToUndoList( iMapIndex );

			{
					if ( usUseObjIndex == FIRSTROAD )
					{
						RemoveObject( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );//dnl without this road stacking occur when you holding left mouse key and pasting roads in bank selection
						AddObjectToHead( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );
					}
					else
					{
						AddStructToHead( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex ] + usUseIndex ) );
						// Add shadows
						if ( !gfBasement && usUseObjIndex == FIRSTCLIFF )
						{
							//AddShadowToHead( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex - FIRSTCLIFF + FIRSTCLIFFSHADOW ] + usUseIndex ) );
							AddObjectToHead( iMapIndex, (UINT16)( gTileTypeStartIndex[ usUseObjIndex - FIRSTCLIFF + FIRSTCLIFFHANG ] + usUseIndex ) );
						}
					}
			}
		}
	}
}

void PasteRoads( INT32 iMapIndex )
{
	UINT16				usUseIndex;

	pSelList = SelRoads;
	pNumSelList = &iNumRoadsSelected;

	usUseIndex = pSelList[ iCurBank ].usIndex;

	PlaceRoadMacroAtGridNo( iMapIndex, usUseIndex );
}

//---------------------------------------------------------------------------------------------------------------
//	PasteTexture
//
//	Puts a ground texture in the world. Ground textures are then "smoothed" in order to blend the edges with one
//	another. The current drawing brush also affects this function.
//
void PasteTexture( INT32 iMapIndex )
{
	ChooseWeightedTerrainTile(); //Kris
	PasteTextureCommon( iMapIndex );
}

//---------------------------------------------------------------------------------------------------------------
//	PasteTextureCommon
//
//	The PasteTexture function calls this one to actually put a ground tile down. If the brush size is larger than
//	one tile, then the above function will call this one and indicate that they should all be placed into the undo
//	stack as the same undo command.
//
void PasteTextureCommon( INT32 iMapIndex )
{
	UINT8					ubLastHighLevel;
	UINT16					usTileIndex;
	//UINT16					Dummy;

	 if ( CurrentPaste != NO_TILE && iMapIndex < 0x80000000 )
	{

		// Set undo, then set new
		AddToUndoList( iMapIndex );

		if ( CurrentPaste == DEEPWATERTEXTURE )
		{
			// IF WE ARE PASTING DEEP WATER AND WE ARE NOT OVER WATER, IGNORE!
			if ( TypeExistsInLandLayer( iMapIndex, REGWATERTEXTURE, &usTileIndex ) )
			{
					if ( !gTileDatabase[ usTileIndex ].ubFullTile )
					{
						return;
					}
			}
			else
			{
					return;
			}
		}

		// Don't draw over floors
		if ( TypeRangeExistsInLandLayer( iMapIndex, FIRSTFLOOR, FOURTHFLOOR, &usTileIndex ) )
		{
			return;
		}



		// Compare heights and do appropriate action
		if ( AnyHeigherLand( iMapIndex, CurrentPaste, &ubLastHighLevel ) )
		{
			// Here we do the following:
			// - Remove old type from layer
			// - Smooth World with old type
			// - Add a 3 by 3 square of new type at head
			// - Smooth World with new type
			PasteHigherTexture( iMapIndex, CurrentPaste );

		}
		else
		{
				PasteTextureEx( iMapIndex, CurrentPaste );
				SmoothTerrainRadius( iMapIndex, CurrentPaste, 1, TRUE );
		}
	}
}


//---------------------------------------------------------------------------------------------------------------
//	PasteHigherTexture
//
//	Some ground textures should be placed "above" others. That is, grass needs to be placed "above" sand etc.
//	This function performs the appropriate actions.
//
void PasteHigherTexture( INT32 iMapIndex, UINT32 fNewType )
{
	UINT16					NewTile;
	UINT8					ubLastHighLevel;
	UINT32					*puiDeletedTypes = NULL;
	UINT8					ubNumTypes;
	UINT8					cnt;

	// Here we do the following:
	// - Remove old type from layer
	// - Smooth World with old type
	// - Add a 3 by 3 square of new type at head
	// - Smooth World with new type

		//if ( iMapIndex < 0x8000 && TypeRangeExistsInLandLayer( iMapIndex, FIRSTFLOOR, LASTFLOOR, &NewTile ) )
		//ATE: DONOT DO THIS!!!!!!! - I know what was intended - not to draw over floors - this
		// I don't know is the right way to do it!
			//return;


	 if ( iMapIndex < 0x80000000 && AnyHeigherLand( iMapIndex, fNewType, &ubLastHighLevel ))
	{
		AddToUndoList( iMapIndex );

		// - For all heigher level, remove
		RemoveHigherLandLevels( iMapIndex, fNewType, &puiDeletedTypes, &ubNumTypes );

		// Set with a radius of 1 and smooth according to height difference
		SetLowerLandIndexWithRadius( iMapIndex, fNewType, 1 , TRUE );

		// Smooth all deleted levels
		for ( cnt = 0; cnt < ubNumTypes; cnt++ )
		{
				SmoothTerrainRadius( iMapIndex, puiDeletedTypes[ cnt ], 1, TRUE );
		}

		MemFree( puiDeletedTypes );

	}
	 else if ( iMapIndex < 0x80000000 )
	{
			AddToUndoList( iMapIndex );

	 GetTileIndexFromTypeSubIndex( fNewType, REQUIRES_SMOOTHING_TILE, &NewTile );
			SetLandIndex( iMapIndex, NewTile, fNewType, FALSE );

			// Smooth item then adding here
			SmoothTerrain( iMapIndex, fNewType, &NewTile, FALSE );

		if ( NewTile != NO_TILE )
		{
				// Change tile
				SetLandIndex( iMapIndex, NewTile, fNewType, FALSE );
		}
	}
}


//---------------------------------------------------------------------------------------------------------------
//	PasteHigherTextureFromRadius
//
//	Like above function except it performs it's operation on a redial area.
//
BOOLEAN PasteHigherTextureFromRadius( INT32 iMapIndex, UINT32 uiNewType, UINT8 ubRadius )
{
	INT32  sTop, sBottom;
	INT32  sLeft, sRight;
	INT32  cnt1, cnt2;
	INT32				iNewIndex;
	INT32				iXPos,iYPos;

	// Determine start and end indicies and num rows
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= -ubRadius;
	sRight	= ubRadius;

	iXPos = (iMapIndex % WORLD_COLS);
	iYPos = (iMapIndex - iXPos) / WORLD_COLS;

	if ( (iXPos + sLeft) < 0 )
		sLeft = (-iXPos);

	if ( (iXPos + sRight) >= WORLD_COLS )
		sRight = (WORLD_COLS - iXPos - 1);

	if ( (iYPos + sTop) >= WORLD_ROWS )
		sTop = (WORLD_ROWS - iYPos - 1);

	if ( (iYPos + sBottom) < 0 )
		sBottom = (-iYPos);

	if ( iMapIndex >= 0x80000000 )
		return (FALSE);

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = iMapIndex + ( WORLD_COLS * cnt1 ) + cnt2;

			PasteHigherTexture( iNewIndex, uiNewType );
		}
	}

	return( TRUE );
}


//---------------------------------------------------------------------------------------------------------------
//	PasteExistingTexture
//
BOOLEAN PasteExistingTexture( INT32 iMapIndex, UINT16 usIndex )
{
	UINT32					uiNewType;
	UINT16					usNewIndex;
	//UINT16					Dummy;

	// If here, we want to make, esentially, what is a type in
	// a level other than TOP-MOST the TOP-MOST level.
	// We should:
	// - remove what was top-most
	// - re-adjust the world to reflect missing top-most peice

	if ( iMapIndex >= 0x80000000 )
		return ( FALSE );

	//if ( TypeRangeExistsInLandLayer( iMapIndex, FIRSTFLOOR, LASTFLOOR, &Dummy ) )
	//	return( FALSE );

	// Get top tile index
	// Remove all land peices except
	GetTileType( usIndex, &uiNewType );

	DeleteAllLandLayers( iMapIndex );

	// ADD BASE LAND AT LEAST!
	usNewIndex = (UINT16)(rand( ) % 10 );

	// Adjust for type
	usNewIndex += gTileTypeStartIndex[ gCurrentBackground ];

	// Set land index
	AddLandToHead( iMapIndex, usNewIndex );

	SetLandIndex( iMapIndex, usIndex, uiNewType, FALSE );

	// ATE: Set this land peice to require smoothing again!
	SmoothAllTerrainTypeRadius( iMapIndex, 2, TRUE );

	return( TRUE );
}


//---------------------------------------------------------------------------------------------------------------
//	PasteExistingTextureFromRadius
//
//	As above, but on a radial area
//
BOOLEAN PasteExistingTextureFromRadius( INT32 iMapIndex, UINT16 usIndex, UINT8 ubRadius )
{
	INT32 sTop, sBottom;
	INT32 sLeft, sRight;
	INT32 cnt1, cnt2;
	INT32				iNewIndex;
	INT32				leftmost;

	// Determine start end end indicies and num rows
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	if ( iMapIndex >= 0x80000000 )
		return ( FALSE );

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{

		leftmost = ( ( iMapIndex + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = iMapIndex + ( WORLD_COLS * cnt1 ) + cnt2;

			if ( iNewIndex >=0 && iNewIndex < WORLD_MAX &&
				iNewIndex >= leftmost && iNewIndex < ( leftmost + WORLD_COLS ) )
			{
				AddToUndoList( iMapIndex );
				PasteExistingTexture( iNewIndex, usIndex );
			}

		}
	}

	return( TRUE );
}


//---------------------------------------------------------------------------------------------------------------
//	SetLowerLandIndexWithRadius
//
//	Puts a land index "under" an existing ground texture. Affects a radial area.
//
BOOLEAN SetLowerLandIndexWithRadius( INT32 iMapIndex, UINT32 uiNewType, UINT8 ubRadius, BOOLEAN fReplace )
{
	UINT16				usTempIndex;
	INT32					sTop, sBottom;
	INT32					sLeft, sRight;
	INT32					cnt1, cnt2;
	INT32				iNewIndex;
	BOOLEAN				fDoPaste = FALSE;
	INT32					leftmost;
	UINT8					ubLastHighLevel;
	UINT32				*puiSmoothTiles = NULL;
	INT16 			sNumSmoothTiles = 0;
	UINT16				usTemp;
	UINT16				NewTile; //,Dummy;

	// Determine start end end indicies and num rows
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	if ( iMapIndex >= 0x80000000 )
		return ( FALSE );

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{

		leftmost = ( ( iMapIndex + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = iMapIndex + ( WORLD_COLS * cnt1 ) + cnt2;

			if ( iNewIndex >=0 && iNewIndex < WORLD_MAX &&
				iNewIndex >= leftmost && iNewIndex < ( leftmost + WORLD_COLS ) )
			{

				if ( fReplace )
				{
					fDoPaste = TRUE;
				}
				else
				{
					if ( TypeExistsInLandLayer( iNewIndex, uiNewType, &usTempIndex ) )
					{
						fDoPaste = TRUE;
					}
				}

				//if ( fDoPaste && !TypeRangeExistsInLandLayer( iMapIndex, FIRSTFLOOR, LASTFLOOR, &Dummy ) )
				if ( fDoPaste	)
				{
					if ( iMapIndex == iNewIndex )
					{
						AddToUndoList( iMapIndex );

						// Force middle one to NOT smooth, and set to random 'full' tile
						usTemp = ( rand( ) % 10 ) + 1;
						GetTileIndexFromTypeSubIndex( uiNewType, usTemp, &NewTile );
						SetLandIndex( iNewIndex, NewTile, uiNewType, FALSE );
					}
					else if ( AnyHeigherLand( iNewIndex, uiNewType, &ubLastHighLevel ) )
					{
						AddToUndoList( iMapIndex );

						// Force middle one to NOT smooth, and set to random 'full' tile
						usTemp = ( rand( ) % 10 ) + 1;
						GetTileIndexFromTypeSubIndex( uiNewType, usTemp, &NewTile );
						SetLandIndex( iNewIndex, NewTile, uiNewType, FALSE );
					}
					else
					{
						AddToUndoList( iMapIndex );

						// Set tile to 'smooth target' tile
						GetTileIndexFromTypeSubIndex( uiNewType, REQUIRES_SMOOTHING_TILE, &NewTile );
						SetLandIndex( iNewIndex, NewTile, uiNewType, FALSE );

						// If we are top-most, add to smooth list
						sNumSmoothTiles++;
						puiSmoothTiles = (UINT32 *) MemRealloc( puiSmoothTiles, sNumSmoothTiles * sizeof( UINT32 ) );
						puiSmoothTiles[ sNumSmoothTiles-1 ] = iNewIndex;
					}
				}
			}
		}
	}

	// Once here, smooth any tiles that need it
	if ( sNumSmoothTiles > 0 )
	{
		for (	cnt1 = 0; cnt1 < sNumSmoothTiles; cnt1++ )
		{

			SmoothTerrainRadius( puiSmoothTiles[ cnt1 ], uiNewType, 10, FALSE );

		}
		MemFree( puiSmoothTiles );
	}

	return( TRUE );
}

// ATE FIXES
void PasteTextureEx( INT32 sGridNo, UINT16 usType )
{
	UINT16 usIndex;
	UINT8	ubTypeLevel;
	UINT16 NewTile;

	// CHECK IF THIS TEXTURE EXISTS!
	if ( TypeExistsInLandLayer( sGridNo, usType, &usIndex ) )
	{
			if ( GetTypeLandLevel( sGridNo, usType, &ubTypeLevel ) )
			{
				// If top-land , do not change
				if ( ubTypeLevel != LANDHEAD )
				{
					PasteExistingTexture( sGridNo, usIndex );
				}
			}
	}
	else
	{

		// Fill with just first tile, smoothworld() will pick proper piece later
		GetTileIndexFromTypeSubIndex( usType, REQUIRES_SMOOTHING_TILE, &NewTile );

		SetLandIndex( sGridNo, NewTile, usType, FALSE );
	}

}


void PasteTextureFromRadiusEx( INT32 sGridNo, UINT16 usType, UINT8 ubRadius )
{
	INT32 sTop, sBottom;
	INT32 sLeft, sRight;
	INT32 cnt1, cnt2;
	INT32				iNewIndex;
	INT32				leftmost;

	// Determine start end end indicies and num rows
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	if ( sGridNo >= 0x80000000 )
		return;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{

		leftmost = ( ( sGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			iNewIndex = sGridNo + ( WORLD_COLS * cnt1 ) + cnt2;

			if ( iNewIndex >=0 && iNewIndex < WORLD_MAX &&
				iNewIndex >= leftmost && iNewIndex < ( leftmost + WORLD_COLS ) )
			{
				PasteTextureEx( sGridNo, usType );
			}

		}
	}

	return;
}

/**************   Start of New Definition for Cliff tiles   **************/
//dnl ch3 210909
#define FIRSTCLIFFSNUMBER (FIRSTCLIFF17-FIRSTCLIFF1+1)
#define FIRSTCLIFFSHANGNUMBER (FIRSTCLIFFHANG17-FIRSTCLIFFHANG1+1)
typedef struct
{
	UINT8 ubNumberOfTiles;
	RelTileLoc TileLocData[256];
}CLIFF_OFFSET_DATA;

CLIFF_OFFSET_DATA CliffOffsetData[FIRSTCLIFFSNUMBER]=
{
	{11,  -6,-7,  -5,-7,  -6,-6,  -4,-6,  -3,-5,  -2,-4,  -1,-3,  -1,-2,  -1,-1,  -1, 0,   0, 0 },//FIRSTCLIFF1
	{10,  -7,-6,  -7,-5,  -6,-4,  -5,-3,  -4,-2,  -3,-1,  -2,-1,  -1,-1,   0,-1,   0, 0 },//FIRSTCLIFF2
	{ 6,   3,-3,   2,-2,   0,-1,   1,-1,   2,-1,   0, 0 },//FIRSTCLIFF3
	{ 6,   2,-3,   3,-3,   0,-2,   1,-2,   0,-1,   0, 0 },//FIRSTCLIFF4
	{ 5,   0,-4,   0,-3,   0,-2,   0,-1,   0, 0 },//FIRSTCLIFF5
	{ 7,   0,-4,   1,-3,   1,-2,   0,-1,   1,-1,   0, 0,  1, 0 },//FIRSTCLIFF6
	{ 8,  -4,-1,  -3,-1,  -2,-1,  -1,-1,  -4,0,  -3, 0,  -2, 0,  -1, 0 },//FIRSTCLIFF7
	{ 6,  -4,-1,  -3,-1,  -2,-1,  -1,-1,  -4, 0,   0, 0 },//FIRSTCLIFF8
	{ 7,   2,-3,   3,-3,   1,-2,   2,-2,   0,-1,   1,-1,   0, 0 },//FIRSTCLIFF9
	{ 5,  -4, 0,  -3, 0,  -2, 0,  -1, 0,   0, 0 },//FIRSTCLIFF10
	{ 7,  -2,-5,  -2,-4,  -1,-4,   0,-3,   0,-2,   0,-1,   0, 0 },//FIRSTCLIFF11
	{ 4,  -2,-2,  -2,-1,  -1, 0,   0, 0 },//FIRSTCLIFF12
	{ 6,  -5,-2,  -4,-2,  -3,-1,  -2, 0,  -1, 0,   0, 0 },//FIRSTCLIFF13
	{ 4,  -2,-2,  -1,-2,   0,-1,   0, 0 },//FIRSTCLIFF14
	{10,  -6,-7,  -5,-7,  -6,-6,  -4,-6,  -3,-5,  -2,-4,  -1,-3,  -1,-2,  -1,-1,  -1, 0 },//FIRSTCLIFF15
	{10,  -7,-6,  -7,-5,  -6,-4,  -5,-3,  -4,-2,  -3,-1,  -2,-1,  -1,-1,   0,-1,   0, 0 },//FIRSTCLIFF16
	{ 5,   0,-4,   0,-3,   0,-2,   0,-1,  0, 0 },//FIRSTCLIFF17
};

UINT8 GetNumberOfCliffTiles(UINT16 fType, UINT16 usIndex)
{
	if(fType == FIRSTCLIFF)
		return(CliffOffsetData[usIndex-FIRSTCLIFF1].ubNumberOfTiles);
	else if(fType == FIRSTCLIFFHANG)
		return(CliffOffsetData[usIndex-FIRSTCLIFFHANG1].ubNumberOfTiles);
	else
		return(0);
}

INT16 CountCliffOffset(UINT16 fType, UINT16 usIndex, UINT8 ubLoop)
{
	if(fType == FIRSTCLIFF)
		return(CliffOffsetData[usIndex-FIRSTCLIFF1].TileLocData[ubLoop].bTileOffsetX + CliffOffsetData[usIndex-FIRSTCLIFF1].TileLocData[ubLoop].bTileOffsetY * WORLD_COLS);
	else if(fType == FIRSTCLIFFHANG)
		return(CliffOffsetData[usIndex-FIRSTCLIFFHANG1].TileLocData[ubLoop].bTileOffsetX + CliffOffsetData[usIndex-FIRSTCLIFFHANG1].TileLocData[ubLoop].bTileOffsetY * WORLD_COLS);
	else
		return(0);
}

enum CliffRaise
{
	RAISE_NONE,	//MAPELEMENT_RAISE_LAND_NONE
	RAISE_START,//MAPELEMENT_RAISE_LAND_START
	RAISE_END,	//MAPELEMENT_RAISE_LAND_END
	RAISE_BOTH,	//(MAPELEMENT_RAISE_LAND_START | MAPELEMENT_RAISE_LAND_END)
};

typedef struct
{
	UINT8 ubNumberOfTiles;
	UINT8 ubTileRaise[256];
}CLIFF_RAISE_DATA;

CLIFF_RAISE_DATA CliffRaiseData[FIRSTCLIFFSNUMBER+FIRSTCLIFFSHANGNUMBER]=
{
	{11,  1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2 },//FIRSTCLIFF1
	{10,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },//FIRSTCLIFF2
	{ 6,  1, 1, 1, 1, 1, 1 },//FIRSTCLIFF3
	{ 6,  1, 1, 1, 1, 1, 1 },//FIRSTCLIFF4
	{ 5,  1, 1, 1, 1, 1 },//FIRSTCLIFF5
	{ 7,  1, 1, 1, 1, 1, 1, 1 },//FIRSTCLIFF6
	{ 7,  1, 1, 1, 1, 1, 1, 1, 1 },//FIRSTCLIFF7
	{ 6,  1, 1, 1, 2/*1*/, 1, 2 },//FIRSTCLIFF8
	{ 7,  1, 1, 1, 1, 1, 1, 1 },//FIRSTCLIFF9
	{ 5,  2/*1*/, 1, 1, 1, 1 },//FIRSTCLIFF10
	{ 7,  2, 2, 2, 2, 2, 2, 2 },//FIRSTCLIFF11
	{ 4,  2, 2, 2, 2 },//FIRSTCLIFF12
	{ 6,  1, 1, 1, 1, 1, 1 },//FIRSTCLIFF13
	{ 4,  1, 1, 1, 1 },//FIRSTCLIFF14
	{10,  1, 2, 1, 2, 2, 2, 2, 2, 2, 2 },//FIRSTCLIFF15
	{10,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },//FIRSTCLIFF16
	{ 5,  1, 1, 1, 1, 1 },//FIRSTCLIFF17

	{11,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG1
	{10,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG2
	{ 6,  2, 2, 2, 2, 2, 2 },//FIRSTCLIFFHANG3
	{ 6,  2, 2, 2, 2, 2, 2 },//FIRSTCLIFFHANG4
	{ 5,  2, 2, 2, 2, 2 },//FIRSTCLIFFHANG5
	{ 7,  2, 2, 2, 2, 2, 2, 2 },//FIRSTCLIFFHANG6
	{ 7,  0, 0, 0, 0, 2, 2, 2 },//FIRSTCLIFFHANG7
	{ 6,  2, 2, 2, 2, 2, 0 },//FIRSTCLIFFHANG8
	{ 7,  2, 2, 2, 2, 2, 2, 0 },//FIRSTCLIFFHANG9
	{ 5,  2, 2, 2, 2, 2 },//FIRSTCLIFFHANG10
	{ 7,  0, 0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG11
	{ 4,  0, 0, 0, 0 },//FIRSTCLIFFHANG12
	{ 6,  0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG13
	{ 4,  0, 0, 0, 0 },//FIRSTCLIFFHANG14
	{10,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG15
	{10,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//FIRSTCLIFFHANG16
	{ 5,  2, 2, 2, 2, 2 },//FIRSTCLIFFHANG17
};

UINT16 GetCliffRaiseData(UINT16 fType, UINT16 usIndex, UINT8 ubLoop)
{
	UINT8 ubTileRaise;
	UINT16 uiFlags;

	if(fType == FIRSTCLIFF)
		ubTileRaise = CliffRaiseData[usIndex-FIRSTCLIFF1].ubTileRaise[ubLoop];//return(CliffRaiseData[usIndex].ubTileRaise[ubLoop]);
	else if(fType == FIRSTCLIFFHANG)
		ubTileRaise = CliffRaiseData[FIRSTCLIFFSNUMBER+usIndex-FIRSTCLIFFHANG1].ubTileRaise[ubLoop];//return(CliffRaiseData[FIRSTCLIFFSNUMBER+usIndex].ubTileRaise[ubLoop]);
	else
		return(0);
	switch(ubTileRaise)
	{
	case RAISE_START:
		uiFlags = MAPELEMENT_RAISE_LAND_START;
		break;
	case RAISE_END:
		uiFlags = MAPELEMENT_RAISE_LAND_END;
		break;
	case RAISE_BOTH:
		uiFlags = MAPELEMENT_RAISE_LAND_START | MAPELEMENT_RAISE_LAND_END;
		break;
	default:
		uiFlags = 0;
	}
	return(uiFlags);
}

void CreateCliffsDefinition(void)
{
	INT32 i;
	UINT32 uiBytesRead, uiBytesWrite;
	HWFILE hFile;
	STR8 szFileName = "TILESETS\\0\\l_cliff.JSD";
	STRUCTURE_FILE_HEADER CliffHeader;
	AuxObjectData CliffData[FIRSTCLIFFSNUMBER];
	hFile = FileOpen(szFileName, FILE_ACCESS_READ, FALSE);
	Assert(hFile);
	FileRead(hFile, &CliffHeader, sizeof(STRUCTURE_FILE_HEADER), &uiBytesRead);
	FileRead(hFile, &CliffData, FIRSTCLIFFSNUMBER*sizeof(AuxObjectData), &uiBytesRead);
	FileClose(hFile);
	hFile = FileOpen(szFileName, FILE_ACCESS_WRITE, FALSE);
	Assert(hFile);
	for(i=0; i<FIRSTCLIFFSNUMBER; i++)
	{
		CliffData[i].ubNumberOfTiles = CliffOffsetData[i].ubNumberOfTiles;
		if(i == 0)
			CliffData[i].usTileLocIndex = 0;
		else
			CliffData[i].usTileLocIndex = CliffData[i-1].ubNumberOfTiles + CliffData[i-1].usTileLocIndex;
	}
	CliffHeader.usNumberOfImageTileLocsStored = CliffData[i-1].ubNumberOfTiles + CliffData[i-1].usTileLocIndex;
	FileWrite(hFile, &CliffHeader, sizeof(STRUCTURE_FILE_HEADER), &uiBytesWrite);
	FileWrite(hFile, &CliffData, FIRSTCLIFFSNUMBER*sizeof(AuxObjectData), &uiBytesWrite);
	for(int i=0; i<FIRSTCLIFFSNUMBER; i++)
		FileWrite(hFile, CliffOffsetData[i].TileLocData, CliffOffsetData[i].ubNumberOfTiles*sizeof(RelTileLoc), &uiBytesWrite);
	FileClose(hFile);
}

void RaiseWorldLand(void)
{
	//dnl ch56 141009
	INT32 cnt, sTempGridNo, iNumberOfRaises;
	LEVELNODE *pStruct;
	TILE_ELEMENT *pTileElement;
	UINT16 usIndex;
	UINT16 fType;
	UINT8 ubLoop;

	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
L02:	//remove any FIRSTCLIFFHANG if on same tile already exist FIRSTCLIFF
		pStruct = gpWorldLevelData[cnt].pStructHead;
		fType = FIRSTTEXTURE;
		usIndex = FIRSTTEXTURE1;
		while(pStruct)
		{
			pTileElement = &gTileDatabase[pStruct->usIndex];
			if(pTileElement->fType == FIRSTCLIFF)
				fType = pTileElement->fType;
			else
			if(pTileElement->fType == FIRSTCLIFFHANG)
				usIndex = pStruct->usIndex;
			pStruct = pStruct->pNext;
		}
		if(fType != FIRSTTEXTURE && usIndex != FIRSTTEXTURE1)
		{
			RemoveStruct(cnt, usIndex);
			goto L02;
		}
		gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_START);
		gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_END);
		// Get Structure levelnode
		pStruct = gpWorldLevelData[cnt].pStructHead;
		gpWorldLevelData[cnt].sHeight=0;
		while(pStruct)
		{
			// Skip cached tiles
			if(pStruct->usIndex >= giNumberOfTiles)
			{
				pStruct = pStruct->pNext;
				continue;
			}
/*
// Old way which requires new "TILESETS\\0\\l_cliff.JSD" created with CreateCliffsDefinition()
			pTileElement = &gTileDatabase[pStruct->usIndex];
			fType = pTileElement->fType;
			if(pTileElement->fType == FIRSTCLIFFHANG)// for use in middle cliff construction
			{
				pTileElement = &gTileDatabase[pStruct->usIndex+(FIRSTCLIFF17-FIRSTCLIFF1+1)];// to get FIRSTCLIFF data, because there are no definition for FIRSTCLIFFHANG data (each FIRSTCLIFF has proper FIRSTCLIFFHANG and FIRSTCLIFFSHADOW)
				goto L01;
			}
			if(pTileElement->fType == FIRSTCLIFF)
			{
L01:
				usIndex = pStruct->usIndex;
				// DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Cliff found at count=%d",cnt));
				if(pTileElement->ubNumberOfTiles > 1)
				{
					// DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Cliff has %d children", pTileElement->ubNumberOfTiles));
					for(ubLoop=0; ubLoop<pTileElement->ubNumberOfTiles; ubLoop++)
					{
						sTempGridNo = cnt + pTileElement->pTileLocData[ubLoop].bTileOffsetX + pTileElement->pTileLocData[ubLoop].bTileOffsetY * WORLD_COLS;
						// Check for valid gridno
						if(OutOfBounds(cnt, sTempGridNo))
						{
							continue;
						}
						gpWorldLevelData[sTempGridNo].uiFlags |= GetCliffRaiseData(fType, usIndex, ubLoop);
					}
				}
			}
*/
			if(pStruct->usIndex >= FIRSTCLIFFHANG1 && pStruct->usIndex <= FIRSTCLIFF17)
			{
				usIndex = pStruct->usIndex;
				fType = FIRSTCLIFFHANG;
				if(pStruct->usIndex >= FIRSTCLIFF1)
					fType = FIRSTCLIFF;
				for(ubLoop=0; ubLoop<GetNumberOfCliffTiles(fType, usIndex); ubLoop++)
				{
					sTempGridNo = cnt + CountCliffOffset(fType, usIndex, ubLoop);
					if(OutOfBounds(cnt, sTempGridNo))// Check for valid gridno
						continue;
					gpWorldLevelData[sTempGridNo].uiFlags |= GetCliffRaiseData(fType, usIndex, ubLoop);
				}
			}
			pStruct = pStruct->pNext;
		}
	}
//HighGroundDebug();

	// Edge conditions for eastern region of map
	// Visible y have values from 4 to 79, so correction +0 for SE corner, and +4 for NE corner, visible x goes from 82 to 157
	for(int y=(WORLD_ROWS/2-1 +0); y>=0 +4; y--)
	{
		// -2 correction because there are maps where cliffs are not place exactly on maps edges, but maybe this condition is not quite precise
		for(int x=y+ WORLD_COLS/2 -2; x<WORLD_COLS; x++)
		{
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START) || (gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
			{
				// If on (x-1, y-1) exist rised element then skip this one becasue he will rise map
				if((gpWorldLevelData[x-1+(y-1)*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START) || (gpWorldLevelData[x-1+(y-1)*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
					break;
				for(int yy=0; yy<y; yy++)
					for(int xx=x; xx<WORLD_COLS; xx++)
						gpWorldLevelData[xx+yy*WORLD_COLS].uiFlags = gpWorldLevelData[xx+y*WORLD_COLS].uiFlags;
				break;
			}
		}
	}
//HighGroundDebug();

	// Edge conditions for northern region of map
	// Visible x,y have values from 4 to 81, so correction +2 for NW corner, and +4 for NE corner
	for(int y=(WORLD_ROWS/2-1 +2); y>=0 +4; y--)
	{
		// x depends on current y, and extra +4 because visible area begins from 4, x>=0 because copy goes from visible map edge to the end of northern part of map
		for(int x=(WORLD_COLS/2-1 +2 -y) +4; x>=0; x--)
		{
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START) || (gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
			{
				// If on (x-1, y-1) exist rised element then skip this one becasue he will rise map
				if((gpWorldLevelData[x+1+(y-1)*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START) || (gpWorldLevelData[x+1+(y-1)*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
					break;
				// Find first appearance of MAPELEMENT_RAISE_LAND_END and from them goes copy entire unvisible row
				for(x; x>=0; x--)
					if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
						break;
				for(int yy=0; yy<y; yy++)
					for(int xx=x; xx>=0; xx--)
						gpWorldLevelData[xx+yy*WORLD_COLS].uiFlags = gpWorldLevelData[xx+y*WORLD_COLS].uiFlags;
				break;
			}
		}
	}
//HighGroundDebug();

//RaiseLevelDebug(0);
	for(int y=0; y<WORLD_ROWS; y++)
	{
		iNumberOfRaises=0;
		for(int x=0; x<WORLD_COLS; x++)// Scaning from left to right
		{
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
			{
				if(x>0 && !(gpWorldLevelData[(x-1)+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
				{
					if(iNumberOfRaises < 3)
						iNumberOfRaises++;
				}
			}
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START))
			{
//RaiseLevelDebug(1, (x+y*WORLD_COLS), iNumberOfRaises);
				gpWorldLevelData[x+y*WORLD_COLS].sHeight = iNumberOfRaises * WORLD_CLIFF_HEIGHT;
				if(x>0 && !(gpWorldLevelData[(x-1)+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START))
				{
					if(iNumberOfRaises)
					{
						iNumberOfRaises--;
						continue;
					}
				}
			}
			if(iNumberOfRaises)
			{
//RaiseLevelDebug(1, (x+y*WORLD_COLS), iNumberOfRaises);
				gpWorldLevelData[x+y*WORLD_COLS].sHeight = iNumberOfRaises * WORLD_CLIFF_HEIGHT;
			}
		}
		for(int x=WORLD_COLS-1; x>=0; x--)
		{
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START))
			{
				if(x<WORLD_ROWS-1 && !(gpWorldLevelData[(x+1)+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_START))
				{
					if(iNumberOfRaises < 3)
						iNumberOfRaises++;
				}
			}
			if((gpWorldLevelData[x+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
			{
//RaiseLevelDebug(1, (x+y*WORLD_COLS), iNumberOfRaises);
				gpWorldLevelData[x+y*WORLD_COLS].sHeight = iNumberOfRaises * WORLD_CLIFF_HEIGHT;
				if(x<WORLD_ROWS-1 && !(gpWorldLevelData[(x-1)+y*WORLD_COLS].uiFlags & MAPELEMENT_RAISE_LAND_END))
				{
					if(iNumberOfRaises)
					{
						iNumberOfRaises--;
						continue;
					}
				}
			}
			if(iNumberOfRaises)
			{
//RaiseLevelDebug(1, (x+y*WORLD_COLS), iNumberOfRaises);
				gpWorldLevelData[x+y*WORLD_COLS].sHeight = iNumberOfRaises * WORLD_CLIFF_HEIGHT;
			}
		}
	}
//HighGroundDebug();
//RaiseLevelDebug(2);

	fRaiseWorld = TRUE;
}
/***************   End of New Definition for Cliff tiles   ***************/

// FUNCTION TO GIVE NEAREST GRIDNO OF A CLIFF
#define LAND_DROP_1 FIRSTCLIFF1
#define LAND_DROP_2 FIRSTCLIFF11
#define LAND_DROP_3 FIRSTCLIFF12
#define LAND_DROP_4 FIRSTCLIFF15
#define LAND_DROP_5 FIRSTCLIFF8
void RaiseWorldLandOld(void)//dnl ch3 230909
{
	INT32					cnt;
	INT32				sTempGridNo;
	LEVELNODE			*pStruct;
	TILE_ELEMENT	*pTileElement;
	BOOLEAN fRaise;
	BOOLEAN fRaiseSet;
	BOOLEAN fSomethingRaised = FALSE;
	UINT8 ubLoop;
	UINT16 usIndex;
	BOOLEAN fStopRaise = FALSE;
	INT32 iCounterA = 0, iCounterB = 0;
	INT32 iStartNumberOfRaises = 0;
	INT32 iNumberOfRaises = 0;
	BOOLEAN fAboutToRaise = FALSE;

	fRaise=FALSE;
	fRaiseSet=FALSE;

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
			gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
			gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_END );

	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{

		// Get Structure levelnode
		pStruct = gpWorldLevelData[ cnt ].pStructHead;
		gpWorldLevelData[cnt].sHeight=0;

		while( pStruct )
		{
			// Skip cached tiles
			if (pStruct->usIndex >= giNumberOfTiles)
			{
				pStruct = pStruct->pNext;
				continue;
			}
			pTileElement = &(gTileDatabase[ pStruct->usIndex ]);
			if (pTileElement->fType==FIRSTCLIFF)
			{
				fSomethingRaised = TRUE;
				usIndex=pStruct->usIndex;
				// DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Cliff found at count=%d",cnt));
				if( pTileElement->ubNumberOfTiles > 1 )
				{
					// DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Cliff has %d children", pTileElement->ubNumberOfTiles));
					for (ubLoop = 0; ubLoop < pTileElement->ubNumberOfTiles; ubLoop++)
					{
						// need means to turn land raising on and off based on the tile ID and the offset in the
						// tile database when reading into the mapsystem
						// turning off of land raising can only be done
						// presently by CLIFF object/tileset 1
						// so simply detect this tile set and turn off instead of on
						// element 1 is 12 tiles and is unique

						sTempGridNo = cnt + pTileElement->pTileLocData[ubLoop].bTileOffsetX + pTileElement->pTileLocData[ubLoop].bTileOffsetY * WORLD_COLS;
						// Check for valid gridno
						if ( OutOfBounds( cnt, sTempGridNo ) )
						{
							continue;
						}
						//if (pTileElement->ubNumberOfTiles==10)
						if((usIndex==LAND_DROP_1)||(usIndex==LAND_DROP_2) ||(usIndex==LAND_DROP_4))
						{
							gpWorldLevelData[sTempGridNo].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
							gpWorldLevelData[sTempGridNo].uiFlags |= MAPELEMENT_RAISE_LAND_END;
						}
						else if( ( usIndex==LAND_DROP_5 ) && ( ubLoop == 4 ) )
						{
							gpWorldLevelData[sTempGridNo].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
							gpWorldLevelData[sTempGridNo].uiFlags |= MAPELEMENT_RAISE_LAND_END;
							if( !( gpWorldLevelData[sTempGridNo + 1 ].uiFlags & MAPELEMENT_RAISE_LAND_START ) )
							{
								gpWorldLevelData[sTempGridNo + 1].uiFlags |= MAPELEMENT_RAISE_LAND_START;
							}
						}
						else if( ( usIndex== LAND_DROP_3 ) && ( ( ubLoop == 0 ) || ( ubLoop == 1 ) || ( ubLoop == 2 )) )
						{
							gpWorldLevelData[sTempGridNo].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
							gpWorldLevelData[sTempGridNo].uiFlags |= MAPELEMENT_RAISE_LAND_END;
						}
						else
						{
							gpWorldLevelData[sTempGridNo].uiFlags |= MAPELEMENT_RAISE_LAND_START;
						}
					}
				}
				else
				{
					if( usIndex==LAND_DROP_3 )
					{
						gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
						gpWorldLevelData[cnt].uiFlags |= MAPELEMENT_RAISE_LAND_END;
					}
					else
					{
						//if (pTileElement->ubNumberOfTiles==10)
						if(usIndex==LAND_DROP_1)
						{
							gpWorldLevelData[cnt].uiFlags &= (~MAPELEMENT_RAISE_LAND_START );
							gpWorldLevelData[cnt].uiFlags |= MAPELEMENT_RAISE_LAND_END;
						}
						else
							gpWorldLevelData[cnt].uiFlags |= MAPELEMENT_RAISE_LAND_START;
					}
				}
			}
			pStruct=pStruct->pNext;
		}
	}

	if (fSomethingRaised == FALSE)
	{
		// no cliffs
		return;
	}

	// run through again, this pass is for placing raiselandstart in rows that have raiseland end but no raiselandstart
	for (cnt=WORLD_MAX-1; cnt >=0 ; cnt--)
	{
		if(cnt%WORLD_ROWS==WORLD_ROWS-1)
		{
		// start of new row
		fRaiseSet=FALSE;
		}
		if (gpWorldLevelData[cnt].uiFlags&MAPELEMENT_RAISE_LAND_START)
		{
			fRaiseSet=TRUE;
		}
		else if((gpWorldLevelData[cnt].uiFlags&MAPELEMENT_RAISE_LAND_END)&&(!fRaiseSet))
		{
			// there is a dropoff without a rise.
			// back up and set beginning to raiseland start
			gpWorldLevelData[cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))].uiFlags &= (~MAPELEMENT_RAISE_LAND_END );
			gpWorldLevelData[cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))].uiFlags|=MAPELEMENT_RAISE_LAND_START;
			if(cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))-WORLD_ROWS >0)
			{
				gpWorldLevelData[cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))-WORLD_ROWS].uiFlags &= (~MAPELEMENT_RAISE_LAND_END );
				gpWorldLevelData[cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))-WORLD_ROWS].uiFlags|=MAPELEMENT_RAISE_LAND_START;
			}
			fRaiseSet=TRUE;
		}
	}
	fRaiseSet=FALSE;
	// Look for a cliff face that is along either the lower edge or the right edge of the map, this is used for a special case fill
	// start at y=159, x= 80 and go to x=159, y=80

	// now check along x=159, y=80 to x=80, y=0
	for (cnt=( ( WORLD_COLS * WORLD_ROWS ) - ( WORLD_ROWS / 2 ) * ( WORLD_ROWS - 2 ) - 1 ); cnt >WORLD_ROWS-1 ; cnt-=(WORLD_ROWS+1))
	{
		if( fAboutToRaise == TRUE )
		{
			fRaiseSet=TRUE;
			fAboutToRaise = FALSE;
		}

		if ((gpWorldLevelData[cnt].uiFlags&MAPELEMENT_RAISE_LAND_START)||(gpWorldLevelData[cnt-1].uiFlags&MAPELEMENT_RAISE_LAND_START)||(gpWorldLevelData[ cnt + 1 ].uiFlags&MAPELEMENT_RAISE_LAND_START))
		{
			fAboutToRaise = TRUE;
			fRaiseSet = FALSE;
		}
		else if((gpWorldLevelData[cnt].uiFlags&MAPELEMENT_RAISE_LAND_END)||(gpWorldLevelData[cnt-1].uiFlags&MAPELEMENT_RAISE_LAND_END) || ( gpWorldLevelData[cnt+1].uiFlags&MAPELEMENT_RAISE_LAND_END) )
		{
			fRaiseSet=FALSE;
		}
		if (fRaiseSet)
		{
			gpWorldLevelData[cnt+((WORLD_ROWS-1)-(cnt%WORLD_ROWS))].uiFlags|=MAPELEMENT_RAISE_LAND_START;
			//gpWorldLevelData[cnt].uiFlags|=MAPELEMENT_RAISE_LAND_START;
			//gpWorldLevelData[cnt-1].uiFlags|=MAPELEMENT_RAISE_LAND_START;
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("Land Raise start at count: %d is raised",cnt ));
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("Land Raise start at count: %d is raised",cnt - 1 ));
		}
	}

	//fRaiseSet = FALSE;

	// Now go through the world, starting at x=max(x) and y=max(y) and work backwards
	// if a cliff is found turn raise flag on, if the end of a screen is found, turn off
	// the system uses world_cord=x+y*(row_size)


	for (cnt=WORLD_MAX-1; cnt >= 0;	cnt--)
	{
		//	reset the RAISE to FALSE
		// End of the row
		if ( !(cnt % WORLD_ROWS) )
		{
			iNumberOfRaises = 0;
			iStartNumberOfRaises = 0;
		}

		if( ( gpWorldLevelData[cnt].uiFlags & MAPELEMENT_RAISE_LAND_END ) )
		{
			if( cnt > 1 )
			{
				if( ( !( gpWorldLevelData[ cnt - 1 ].uiFlags & MAPELEMENT_RAISE_LAND_END) && !( gpWorldLevelData[ cnt - 2 ].uiFlags & MAPELEMENT_RAISE_LAND_END ) ) )
				{
					iNumberOfRaises--;
				}
			}
		}
		else if( gpWorldLevelData[cnt].uiFlags & MAPELEMENT_RAISE_LAND_START )
		{
			// check tile before and after, if either are raise land flagged, then don't increment number of
			// raises
			if( cnt < WORLD_MAX - 2 )
			{
				if( ( !( gpWorldLevelData[ cnt + 1 ].uiFlags & MAPELEMENT_RAISE_LAND_START) && !( gpWorldLevelData[ cnt + 2 ].uiFlags & MAPELEMENT_RAISE_LAND_START ) )	)
				{
					iNumberOfRaises++;
				}
			}
		}

		// look at number of raises.. if negative, then we have more downs than ups, restart row with raises + 1;
		// now raise land of any tile while the flag is on
		if( iNumberOfRaises < 0 )
		{
			// something wrong, reset cnt
			iStartNumberOfRaises++;
			cnt += WORLD_ROWS - cnt % WORLD_ROWS;
			iNumberOfRaises = iStartNumberOfRaises;
			continue;
		}

		if( iNumberOfRaises >= 0 )
		{
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("Land Raise start at count: %d is raised",cnt ));
			gpWorldLevelData[cnt].sHeight=iNumberOfRaises * WORLD_CLIFF_HEIGHT;
		}
	}

	for (cnt=WORLD_MAX-1; cnt >= 0;	cnt--)
	{
		if( ( cnt < WORLD_MAX - WORLD_ROWS ) && ( cnt > WORLD_ROWS ) )
		{
			if( ( gpWorldLevelData[cnt + WORLD_ROWS ].sHeight == gpWorldLevelData[cnt - WORLD_ROWS ].sHeight ) && ( gpWorldLevelData[ cnt	].sHeight!= gpWorldLevelData[cnt - WORLD_ROWS ].sHeight ) )
			{
				gpWorldLevelData[cnt].sHeight = gpWorldLevelData[cnt + WORLD_ROWS ].sHeight;
			}
			else if( ( gpWorldLevelData[ cnt ].sHeight > gpWorldLevelData[cnt - WORLD_ROWS ].sHeight )&& ( gpWorldLevelData[cnt + WORLD_ROWS ].sHeight != gpWorldLevelData[cnt - WORLD_ROWS ].sHeight ) && ( gpWorldLevelData[cnt ].sHeight > gpWorldLevelData[cnt + WORLD_ROWS ].sHeight ) )
			{
				if( gpWorldLevelData[cnt - WORLD_ROWS ].sHeight > gpWorldLevelData[cnt + WORLD_ROWS ].sHeight )
				{
					gpWorldLevelData[ cnt ].sHeight = gpWorldLevelData[cnt - WORLD_ROWS ].sHeight;
				}
				else
				{
					gpWorldLevelData[ cnt ].sHeight = gpWorldLevelData[cnt + WORLD_ROWS ].sHeight;
				}
			}
		}
	}

//*/
	fRaiseWorld = TRUE;//dnl ch3 210909
}

void EliminateObjectLayerRedundancy()
{
	INT32 i, numRoads, numAnothers;
	UINT32 uiType;
	LEVELNODE *pObject, *pValidRoad, *pValidAnother;
	UINT16 usIndex;

	for( i = 0; i < WORLD_MAX; i++ )
	{ //Eliminate all but the last ROADPIECE and ANOTHERDEBRIS
		pObject = gpWorldLevelData[ i ].pObjectHead;
		pValidRoad = pValidAnother = NULL;
		numRoads = numAnothers = 0;
		while( pObject )
		{
			GetTileType( pObject->usIndex, &uiType );
			if( uiType == ROADPIECES )
			{ //keep track of the last valid road piece, and count the total
				pValidRoad = pObject;
				numRoads++;
			}
			else if( uiType == ANOTHERDEBRIS )
			{ //keep track of the last valid another debris, and count the total
				pValidAnother = pObject;
				numAnothers++;
			}
			pObject = pObject->pNext;
		}
		if( pValidRoad && numRoads > 1 )
		{ //we have more than two roadpieces on the same gridno, so get rid of them, replacing it
			//with the visible one.
			usIndex = pValidRoad->usIndex;
			RemoveAllObjectsOfTypeRange( i, ROADPIECES, ROADPIECES );
			AddObjectToHead( i, usIndex );
		}
		if( pValidAnother && numAnothers > 1 )
		{ //we have more than two anotherdebris on the same gridno, so get rid of them, replacing it
			//with the visible one.
			usIndex = pValidAnother->usIndex;
			RemoveAllObjectsOfTypeRange( i, ANOTHERDEBRIS, ANOTHERDEBRIS );
			AddObjectToHead( i, usIndex );
		}
	}
}


#endif //JA2EDITOR


