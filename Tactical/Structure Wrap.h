#ifndef STRUCURE_WRAP_H
#define STRUCURE_WRAP_H


BOOLEAN	IsTreePresentAtGridNo( INT32 sGridNo );
BOOLEAN	IsFencePresentAtGridNo( INT32 sGridNo );
BOOLEAN	IsJumpableFencePresentAtGridNo( INT32 sGridNo );
BOOLEAN	IsDoorPresentAtGridNo( INT32 sGridNo );

// OK, THIS FUNCTION RETURNS A LEVELNODE POINTER TO A WALL OF THE SAME ORIENTATION
// AS WAS GIVEN. RETURNS NULL IF NONE FOUND.
LEVELNODE	*GetWallLevelNodeOfSameOrientationAtGridNo( INT32 sGridNo, INT8 ubOrientation );

LEVELNODE *IsWallPresentAtGridNo( INT32 sGridNo );

BOOLEAN IsDoorVisibleAtGridNo( INT32 sGridNo );

BOOLEAN IsHiddenStructureVisible( INT32 sGridNo, UINT16 usIndex );

BOOLEAN DoesGridNoContainHiddenStruct( INT32 sGridNo, BOOLEAN *pfVisible );

BOOLEAN	WallExistsOfTopLeftOrientation( INT32 sGridNo );

BOOLEAN	WallExistsOfTopRightOrientation( INT32 sGridNo );

BOOLEAN	WallOrClosedDoorExistsOfTopLeftOrientation( INT32 sGridNo );

BOOLEAN	WallOrClosedDoorExistsOfTopRightOrientation( INT32 sGridNo );

BOOLEAN OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( INT32 sGridNo );
BOOLEAN OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( INT32 sGridNo );

LEVELNODE	*GetWallLevelNodeAndStructOfSameOrientationAtGridNo( INT32 sGridNo, INT8 ubOrientation, STRUCTURE **ppStructure );

BOOLEAN CutWireFence( INT32 sGridNo );
BOOLEAN IsCuttableWireFenceAtGridNo( INT32 sGridNo );
BOOLEAN IsCutWireFenceAtGridNo( INT32 sGridNo );

BOOLEAN IsRepairableStructAtGridNo( INT32 sGridNo, UINT8 *pubID );
BOOLEAN IsRefuelableStructAtGridNo( INT32 sGridNo, UINT8 *pubID );
BOOLEAN IsFortificationPossibleAtGridNo( INT32 sGridNo );	// added by Flugente


BOOLEAN	IsRoofPresentAtGridNo( INT32 sGridNo );

INT32 FindDoorAtGridNoOrAdjacent( INT32 sGridNo );

BOOLEAN IsCorpseAtGridNo( INT32 sGridNo, UINT8 ubLevel );

BOOLEAN SetOpenableStructureToClosed( INT32 sGridNo, UINT8 ubLevel );

//Legion by Jazz
BOOLEAN	IsJumpableWindowPresentAtGridNo( INT32 sGridNo , INT8 direction2, BOOLEAN fIntactWindowsAlso ); //legion 2 Windows
BOOLEAN	IsLegionWallPresentAtGridno( INT32 sGridNo ); //legion 2 Fence

#endif