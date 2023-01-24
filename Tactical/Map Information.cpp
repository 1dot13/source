	#include <stdio.h>
	#include "fileMan.h"
	#include "types.h"
	#include "Map Information.h"
	#include "Soldier Init List.h"
	#include "Random.h"
	#include "worlddef.h"
	#include "renderworld.h"
	#include "EditorMercs.h"
	#include "Exit Grids.h"
	#include "Isometric Utils.h"
	#include "Animation Data.h"
	#include "Road Smoothing.h"
	#include "message.h"
	#include "weapons.h"
	#include "environment.h"
	#include "lighting.h"
	#include "Animated ProgressBar.h"

//CHRISL: MAJOR_MAP_VERSION information moved to worlddef.h by ADB.  We're using these values elsewhere and need them
//	in the header file
// SB: new map version, with map dimensions added
//#define MAJOR_MAP_VERSION		7.0

FLOAT gdMajorMapVersion = MAJOR_MAP_VERSION;

BOOLEAN gfWorldLoaded;

MAPCREATE_STRUCT gMapInformation;

//CHRISL: MINOR_MAP_VERSION information moved to worlddef.h by ADB.  We're using these values elsewhere and need them
//	in the header file

UINT8 gubMinorMapVersion = MINOR_MAP_VERSION;

/*
MINOR_MAP_VERSION Log -- Created by Kris Morness, November 14, 1997
Version 0 -- Kris -- obsolete November 14, 1997
	The newly created soldier information had a couple bugs regarding initialization
	Bug 1)	Soldier placements without detailed slots weren't initialized.
	Bug 2)	The attitude variable was accidentally being generated like attributes
			which usually put it completely out of range.
Version 1 -- Kris -- obsolete January 7, 1998
	Bug 3)	New changes to the wall decals, made the currently existing ones in the world
			unpredictable, and are automatically removed, so new ones can be used.
Version 2 -- Kris -- obsolete February 3, 1998
	Bug 4)	Enemy mercs now have a color code assignment which defaults to army.
Version 3 -- Kris -- obsolete February 9, 1998
	Bug 5)	Move entry points down if they are too high.
Version 4 -- Kris -- obsolete February 25, 1998
	Bug 6)	Change all doors to FIRSTDOOR
Version 5 -- Kris -- obsolete March 4, 1998
	Bug 7)	Remove all exit grids (the format has changed)
Version 6 -- Kris -- obsolete March 9, 1998
	Bug 8)	Change droppable status of merc items so that they are all undroppable.
Version 7 -- Kris -- obsolete April 14, 1998
	Bug 9)	Priority placements have been dropped in favor of splitting it into two categories.
					The first is Detailed placements, and the second is priority existance.	So, all
					current detailed placements will also have priority existance.
Version 8 -- Kris -- obsolete April 16, 1998
	MAJOR CONFLICT RESULTING IN A MAJOR VERSION UPDATE 2.00!
	Bug 10) Padding on detailed placements is uninitialized.	Clear all data starting at
					fKillSlotIfOwnerDies.
Version 9 -- Kris -- obsolete April 26, 1998
	This version requires no auto updating, but external code has adjusted the size of the mapedgepoint
	arraysize from UINT8 to UINT16.	See Map Edgepoints.c.
	Bug 11) Convert all wheelchaired placement bodytypes to cows.	Result of change in the animation database.
Version 11 -- Kris -- obsolete May 2, 1998
	Added new center entry point.	Need to initialize the original padding to -1.

	Version 6.27 ADB July 22 07
	Changing the format of OBJECTTYPE means the maps should be loaded, converted, and saved
	if one wants to take advantage of the new format
	Converting to the new format is not necessary to load a map and run the game.
	**Note** this conversion hasn't happened yet
*/

//EntryPoints can't be placed on the top two gridnos in a map.	So all we do in this case
//is return the closest gridno.	Returns TRUE if the mapindex changes.
BOOLEAN ValidateEntryPointGridNo( INT32 *sGridNo )
{
	INT16 sXMapPos, sYMapPos;
	INT16 sWorldX, sWorldY;
	INT32 iNewMapX, iNewMapY;
	INT32 sTopLimit, sBottomLimit;
	
	if ( TileIsOutOfBounds( *sGridNo ) )
		return FALSE; //entry point is non-existant

	ConvertGridNoToXY( *sGridNo, &sXMapPos, &sYMapPos );

	sTopLimit = 80;
	sBottomLimit = gsBRY - gsTLY - 40;

	//Get screen coordinates for current gridno
	GetWorldXYAbsoluteScreenXY( sXMapPos, sYMapPos, &sWorldX, &sWorldY);

	if( sWorldY < sTopLimit )
	{
		GetFromAbsoluteScreenXYWorldXY( &iNewMapX, &iNewMapY, sWorldX, sTopLimit );
	}
	else if( sWorldY > sBottomLimit )
	{
		GetFromAbsoluteScreenXYWorldXY( &iNewMapX, &iNewMapY, sWorldX, sBottomLimit );
	}
	else
	{
		return FALSE; //already valid
	}

	*sGridNo = MAPROWCOLTOPOS( iNewMapY/10, iNewMapX/10 );

	return TRUE; //modified
}

//dnl ch42 250909
MAPCREATE_STRUCT& MAPCREATE_STRUCT::operator=(const _OLD_MAPCREATE_STRUCT& src)
{
	if((void*)this != (void*)&src)
	{
		sNorthGridNo = src.sNorthGridNo;
		sEastGridNo = src.sEastGridNo;
		sSouthGridNo = src.sSouthGridNo;
		sWestGridNo = src.sWestGridNo;
		ubNumIndividuals = src.ubNumIndividuals;
		ubMapVersion = src.ubMapVersion;
		ubRestrictedScrollID = src.ubRestrictedScrollID;
		ubEditorSmoothingType = src.ubEditorSmoothingType;
		sCenterGridNo = src.sCenterGridNo;
		sIsolatedGridNo = src.sIsolatedGridNo;
	}
	return(*this);
}

BOOLEAN MAPCREATE_STRUCT::Load(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	if(dMajorMapVersion < 7.0)
	{
		_OLD_MAPCREATE_STRUCT OldMapInformation;
		LOADDATA(&OldMapInformation, *hBuffer, sizeof(_OLD_MAPCREATE_STRUCT));
		*this = OldMapInformation;
	}
	else
		LOADDATA(this, *hBuffer, sizeof(MAPCREATE_STRUCT));
	return(TRUE);
}

BOOLEAN MAPCREATE_STRUCT::Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	PTR pData = this;
	UINT32 uiBytesToWrite = sizeof(MAPCREATE_STRUCT);
	_OLD_MAPCREATE_STRUCT OldMapCreateStruct;
	if(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION)
	{
		memset(&OldMapCreateStruct, 0, sizeof(_OLD_MAPCREATE_STRUCT));
		OldMapCreateStruct.sNorthGridNo = sNorthGridNo;
		OldMapCreateStruct.sEastGridNo = sEastGridNo;
		OldMapCreateStruct.sSouthGridNo = sSouthGridNo;
		OldMapCreateStruct.sWestGridNo = sWestGridNo;
		OldMapCreateStruct.ubNumIndividuals = (UINT8)ubNumIndividuals;
		OldMapCreateStruct.ubMapVersion = ubMinorMapVersion;
		OldMapCreateStruct.ubRestrictedScrollID = ubRestrictedScrollID;
		OldMapCreateStruct.ubEditorSmoothingType = ubEditorSmoothingType;
		OldMapCreateStruct.sCenterGridNo = sCenterGridNo;
		OldMapCreateStruct.sIsolatedGridNo = sIsolatedGridNo;
		pData = &OldMapCreateStruct;
		uiBytesToWrite = sizeof(_OLD_MAPCREATE_STRUCT);
	}

	// a map without entry points would be fatal in a campaign, so print out a warning to the maper
	if ( sNorthGridNo == NOWHERE && sEastGridNo == NOWHERE && sSouthGridNo == NOWHERE && sWestGridNo == NOWHERE && sCenterGridNo == NOWHERE )
	{
		ScreenMsg( MSG_FONT_YELLOW, MSG_ERROR, L"Map has no entry points - please fix this!" );
	}

	UINT32 uiBytesWritten = 0;
	FileWrite(hFile, pData, uiBytesToWrite, &uiBytesWritten);
	if(uiBytesToWrite == uiBytesWritten)
		return(TRUE);
	return(FALSE);
}

void LoadMapInformation(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	gMapInformation.Load(hBuffer, dMajorMapVersion);
	// ATE: OK, do some handling here for basement level scroll restrictions
	// Calcuate world scrolling restrictions
	InitRenderParams(gMapInformation.ubRestrictedScrollID);
}

void SaveMapInformation(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	gMapInformation.Save(hFile, dMajorMapVersion, ubMinorMapVersion);
}

//This will automatically update obsolete map versions to the new ones.	This will even
//work in the game itself, but would require conversion to happen every time.	This is completely
//transparent to the rest of the game, but in the editor, obsolete versions will be updated upon
//loading and won't be permanently updated until the map is saved, regardless of changes.
void UpdateOldVersionMap()
{
	if( gMapInformation.ubMapVersion < 15 )
	{
		AssertMsg( 0, "Map is less than minimum supported version." );
	}
	if( gMapInformation.ubMapVersion < 16 )
	{
		gMapInformation.ubMapVersion = 16;
		gMapInformation.sIsolatedGridNo = -1;
	}
	if( gMapInformation.ubMapVersion < 17 )
	{
		gMapInformation.ubMapVersion = 17;
		//EliminateObjectLayerRedundancy();
	}
	if( gMapInformation.ubMapVersion < 18 )
	{
		// replace useless crowbars with proper ones
		UINT32 i;
		gMapInformation.ubMapVersion = 18;
		for ( i = 0; i < guiNumWorldItems; i++ )
		{
			if ( gWorldItems[ i ].object.usItem == JAR_ELIXIR )
			{
				gWorldItems[ i ].object.usItem = CROWBAR;
			}
		}
	}
	if( gMapInformation.ubMapVersion < 19 )
	{
		//Do nothing, this is used to force regenerate the map edgepoints in map edgepoints.c
		gMapInformation.ubMapVersion = 19;
	}
	if( gMapInformation.ubMapVersion < 20 )
	{
		//validate the map entry points as the world boundaries have changed.
		gMapInformation.ubMapVersion = 20;
		ValidateEntryPointGridNo( &gMapInformation.sNorthGridNo );
		ValidateEntryPointGridNo( &gMapInformation.sEastGridNo );
		ValidateEntryPointGridNo( &gMapInformation.sSouthGridNo );
		ValidateEntryPointGridNo( &gMapInformation.sWestGridNo );
		ValidateEntryPointGridNo( &gMapInformation.sCenterGridNo );
		ValidateEntryPointGridNo( &gMapInformation.sIsolatedGridNo );
	}
	if( gMapInformation.ubMapVersion < 21 )
	{
		SOLDIERINITNODE *curr;
		//override any item slots being locked if there is no item in that slot.
		//Laymen terms:	If any items slots are locked to be empty, make them empty but available
		//for random item generation.
		gMapInformation.ubMapVersion = 21;
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pDetailedPlacement )
			{
				UINT32 invsize = curr->pDetailedPlacement->Inv.size();
				for( UINT32 i = 0; i < invsize; ++i )
				{
					if( !curr->pDetailedPlacement->Inv[ i ].usItem )
					{
						if( curr->pDetailedPlacement->Inv[ i ].fFlags & OBJECT_UNDROPPABLE )
						{
							if( curr->pDetailedPlacement->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							{
								curr->pDetailedPlacement->Inv[ i ].fFlags &= ~OBJECT_NO_OVERWRITE;
							}
						}
					}
				}
			}
			curr = curr->next;
		}
	}
	if( gMapInformation.ubMapVersion < 22 )
	{ //Allow map edgepoints to be regenerated as new system has been reenabled.
		gMapInformation.ubMapVersion = 22;
	}
	if( gMapInformation.ubMapVersion < 23 )
	{ //Allow map edgepoints to be regenerated as new system has been reenabled.
		SOLDIERINITNODE *curr;
		gMapInformation.ubMapVersion = 23;
		if( giCurrentTilesetID == 1 ) //cave/mine tileset only
		{ //convert all civilians to miners which use uniforms and more masculine body types.
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->pBasicPlacement->bTeam == CIV_TEAM && !curr->pDetailedPlacement )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_MINER;
					curr->pBasicPlacement->ubBodyType = -1;
				}
				curr = curr->next;
			}
		}
	}
	if( gMapInformation.ubMapVersion < 25 )
	{
		gMapInformation.ubMapVersion = 25;
		if( gfCaves )
		{
			LightSetBaseLevel( 13 );
		}
	}
	if( gMapInformation.ubMapVersion < 26 )
	{ //Allow map edgepoints to be regenerated as new system has been reenabled.
		gMapInformation.ubMapVersion = 26;
	}
	if( gMapInformation.ubMapVersion < 27 )
	{ //Allow map edgepoints to be regenerated as new system has been reenabled.
		gMapInformation.ubMapVersion = 27;
	}
	if( gMapInformation.ubMapVersion < 29 )
	{ 
		gMapInformation.ubMapVersion = 29;
	}
}

void AutoCalculateItemNoOverwriteStatus()
{
	SOLDIERINITNODE *curr;
	OBJECTTYPE *pItem;

	//Recalculate the "no overwrite" status flag on all items.	There are two different cases:
	//1)	If detailed placement has item, the item "no overwrite" flag is set
	//2)	If detailed placement doesn't have item, but item is set to drop (forced empty slot), the "no overwrite" flag is set.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pDetailedPlacement )
		{
			UINT32 invsize = curr->pDetailedPlacement->Inv.size();
			for( UINT32 i = 0; i < invsize; ++i )
			{
				pItem = &curr->pDetailedPlacement->Inv[ i ];
				if( pItem->exists() == true )
				{	//case 1 (see above)
					(*pItem).fFlags |= OBJECT_NO_OVERWRITE;
				}
				else if( !((*pItem).fFlags & OBJECT_UNDROPPABLE) )
				{ //case 2 (see above)
					(*pItem).fFlags |= OBJECT_NO_OVERWRITE;
				}
			}
		}
		curr = curr->next;
	}
}

void ValidateAndUpdateMapVersionIfNecessary()
{
	//Older versions of mercs may require updating due to past bug fixes, new changes, etc.
	if( gMapInformation.ubMapVersion < MINOR_MAP_VERSION )
	{
		SetRelativeStartAndEndPercentage( 0, 92, 93, L"Updating older map version..." );
		RenderProgressBar( 0, 0 );
		UpdateOldVersionMap();
	}
	else if( gMapInformation.ubMapVersion > MINOR_MAP_VERSION )
	{
		//we may have a problem...
		//AssertMsg( 0, "Map version is greater than the current version (old ja2.exe?)" );
	}
	AutoCalculateItemNoOverwriteStatus() ;
}

#ifdef JA2EDITOR
#include "Exit Grids.h"
#include "Summary Info.h"
//This function is used to avoid conflicts between minor version updates and sector summary info.
//By updating the summary info in conjunction with minor version updates, we can avoid these conflicts
//and really prevent major map updates.
void UpdateSummaryInfo( SUMMARYFILE *pSummary )
{
	if( pSummary->MapInfo.ubMapVersion == MINOR_MAP_VERSION )
		return;
	if( pSummary->MapInfo.ubMapVersion < 9 )
	{ //See bug 10
		pSummary->ubCivSchedules = 0;
	}
	if( pSummary->MapInfo.ubMapVersion < 12 )
	{
		pSummary->MapInfo.sCenterGridNo = -1;
	}
	if( pSummary->MapInfo.ubMapVersion < 16 )
	{
		pSummary->MapInfo.sIsolatedGridNo = -1;
	}
}
#endif
