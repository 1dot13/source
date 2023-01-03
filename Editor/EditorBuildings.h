#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __EDITORBUILDINGS_H
#define __EDITORBUILDINGS_H

#include "video.h"
#include "vobject_blitters.h"

extern BOOLEAN fBuildingShowRoofs, fBuildingShowWalls, fBuildingShowRoomInfo;
//DBrot: More Rooms
//extern UINT8 gubCurrRoomNumber;
//extern UINT8 gubMaxRoomNumber;
extern UINT16 gusCurrRoomNumber;
extern UINT16 gusMaxRoomNumber;

void SetupTextInputForBuildings();
void ExtractAndUpdateBuildingInfo();

//Initialization routines
void GameInitEditorBuildingInfo();
void InitEditorBuildingsToolbar();

//Selection method callbacks
//Building utility functions
void UpdateBuildingsInfo();
void KillBuilding( INT32 iMapIndex );


typedef struct BUILDINGLAYOUTNODE
{
	struct BUILDINGLAYOUTNODE *next;
	INT32 sGridNo;
}BUILDINGLAYOUTNODE;

extern BUILDINGLAYOUTNODE *gpBuildingLayoutList;
extern INT32 gsBuildingLayoutAnchorGridNo;

//The first step is copying a building.	After that, it either must be pasted or moved.
void CopyBuilding( INT32 iMapIndex );
void MoveBuilding( INT32 iMapIndex );
void PasteBuilding( INT32 iMapIndex );
void DeleteBuildingLayout();

void ReplaceBuildingWithNewRoof( INT32 iMapIndex );
void UpdateWallsView();
void UpdateRoofsView();

void InitDoorEditing( INT32 iMapIndex );
void ExtractAndUpdateDoorInfo();
void KillDoorEditing();
void RenderDoorEditingWindow();

void AddLockedDoorCursors();
void RemoveLockedDoorCursors();
void FindNextLockedDoor();

extern BOOLEAN gfEditingDoor;

extern UINT16 usCurrentMode;

#endif

#endif



 


