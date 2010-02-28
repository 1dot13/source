#ifndef __WORLDMAN_H_
#define __WORLDMAN_H_

// memory-accounting function
void CountLevelNodes( void );


// Object manipulation functions
BOOLEAN RemoveObject( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE *AddObjectToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddObjectToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInObjectLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusObjectIndex );
BOOLEAN RemoveAllObjectsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
void SetAllObjectShadeLevels( INT32 iMapIndex, UINT8 ubShadeLevel );
void AdjustAllObjectShadeLevels( INT32 iMapIndex, INT8 bShadeDiff );
BOOLEAN TypeRangeExistsInObjectLayer( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT16 *pusObjectIndex );


// Roof manipulation functions
BOOLEAN RemoveRoof( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE  *AddRoofToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddRoofToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInRoofLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusRoofIndex );
BOOLEAN RemoveAllRoofsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
void SetAllRoofShadeLevels( INT32 iMapIndex, UINT8 ubShadeLevel );
void AdjustAllRoofShadeLevels( INT32 iMapIndex, INT8 bShadeDiff );
void RemoveRoofIndexFlagsFromTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT32 uiFlags  );
void SetRoofIndexFlagsFromTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT32 uiFlags  );
BOOLEAN TypeRangeExistsInRoofLayer( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT16 *pusRoofIndex );
void SetWallLevelnodeFlags( INT32 sGridNo, UINT32 uiFlags );
void RemoveWallLevelnodeFlags( INT32 sGridNo, UINT32 uiFlags );
BOOLEAN IndexExistsInRoofLayer( INT32 sGridNo, UINT16 usIndex );


// OnRoof manipulation functions
BOOLEAN RemoveOnRoof( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE *AddOnRoofToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddOnRoofToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInOnRoofLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusOnRoofIndex );
BOOLEAN RemoveAllOnRoofsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
void SetAllOnRoofShadeLevels( INT32 iMapIndex, UINT8 ubShadeLevel );
void AdjustAllOnRoofShadeLevels( INT32 iMapIndex, INT8 bShadeDiff );
BOOLEAN RemoveOnRoofFromLevelNode( INT32 iMapIndex, LEVELNODE *pNode );


// Land manipulation functions
BOOLEAN RemoveLand( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE *AddLandToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddLandToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInLandLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusLandIndex );
BOOLEAN RemoveAllLandsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
BOOLEAN TypeRangeExistsInLandLayer( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT16 *pusLandIndex );
BOOLEAN TypeRangeExistsInLandHead( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT16 *pusLandIndex );
BOOLEAN ReplaceLandIndex( INT32 iMapIndex, UINT16 usOldIndex, UINT16 usNewIndex );
BOOLEAN DeleteAllLandLayers( INT32 iMapIndex );
BOOLEAN InsertLandIndexAtLevel( INT32 iMapIndex, UINT16 usIndex, UINT8 ubLevel );
BOOLEAN RemoveHigherLandLevels( INT32 iMapIndex, UINT32 fSrcType, UINT32 **puiHigherTypes, UINT8 *pubNumHigherTypes );
BOOLEAN SetLowerLandLevels( INT32 iMapIndex, UINT32 fSrcType, UINT16 usIndex  );
BOOLEAN AdjustForFullTile( INT32 iMapIndex );
void SetAllLandShadeLevels( INT32 iMapIndex, UINT8 ubShadeLevel );
void AdjustAllLandShadeLevels( INT32 iMapIndex, INT8 bShadeDiff );
void AdjustAllLandDirtyCount( INT32 iMapIndex, INT8 bDirtyDiff );
UINT8	GetTerrainType( INT32 sGridNo );
BOOLEAN Water( INT32 sGridNo );
BOOLEAN DeepWater( INT32 sGridNo );
BOOLEAN WaterTooDeepForAttacks( INT32 sGridNo );

// Structure manipulation routines
BOOLEAN RemoveStruct( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE *AddStructToTail( INT32 iMapIndex, UINT16 usIndex );
LEVELNODE *AddStructToTailCommon( INT32 iMapIndex, UINT16 usIndex, BOOLEAN fAddStructDBInfo );
LEVELNODE *ForceStructToTail( INT32 iMapIndex, UINT16 usIndex );

BOOLEAN AddStructToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInStructLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusStructIndex );
BOOLEAN RemoveAllStructsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
BOOLEAN AddWallToStructLayer( INT32 iMapIndex, UINT16 usIndex, BOOLEAN fReplace );
BOOLEAN ReplaceStructIndex( INT32 iMapIndex, UINT16 usOldIndex, UINT16 usNewIndex );
BOOLEAN HideStructOfGivenType( INT32 iMapIndex, UINT32 fType, BOOLEAN fHide );
BOOLEAN InsertStructIndex( INT32 iMapIndex, UINT16 usIndex, UINT8 ubLevel );
void SetAllStructShadeLevels( INT32 iMapIndex, UINT8 ubShadeLevel );
void AdjustAllStructShadeLevels( INT32 iMapIndex, INT8 bShadeDiff );
void SetStructIndexFlagsFromTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT32 uiFlags  );
void RemoveStructIndexFlagsFromTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT32 uiFlags  );
void SetStructAframeFlags( INT32 iMapIndex, UINT32 uiFlags  );
void RemoveStructAframeFlags( INT32 iMapIndex, UINT32 uiFlags  );
BOOLEAN RemoveStructFromLevelNode( INT32 iMapIndex, LEVELNODE *pNode );


BOOLEAN RemoveStructFromTail( INT32 iMapIndex );
BOOLEAN RemoveStructFromTailCommon( INT32 iMapIndex, BOOLEAN fRemoveStructDBInfo );
BOOLEAN ForceRemoveStructFromTail( INT32 iMapIndex );

BOOLEAN TypeRangeExistsInStructLayer( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType, UINT16 *pusStructIndex );


// Shadow manipulation routines
BOOLEAN RemoveShadow( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddShadowToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddShadowToHead( INT32 iMapIndex, UINT16 usIndex );
void AddExclusiveShadow( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInShadowLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusShadowIndex );
BOOLEAN RemoveAllShadowsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
BOOLEAN	RemoveAllShadows( INT32 iMapIndex );
BOOLEAN RemoveShadowFromLevelNode( INT32 iMapIndex, LEVELNODE *pNode );


// Merc manipulation routines
// #################################################################

BOOLEAN AddMercToHead( INT32 iMapIndex, SOLDIERTYPE *pSoldier, BOOLEAN fAddStructInfo );
BOOLEAN RemoveMerc( INT32 iMapIndex, SOLDIERTYPE *pSoldier, BOOLEAN fPlaceHolder  );
UINT8 WhoIsThere2( INT32 sGridNo, INT8 bLevel );
BOOLEAN AddMercStructureInfo( INT32 sGridNo, SOLDIERTYPE *pSoldier );
BOOLEAN AddMercStructureInfoFromAnimSurface( INT32 sGridNo, SOLDIERTYPE *pSoldier, UINT16 usAnimSurface, UINT16 usAnimState );
BOOLEAN UpdateMercStructureInfo( SOLDIERTYPE *pSoldier );
BOOLEAN OKToAddMercToWorld( SOLDIERTYPE *pSoldier, INT8 bDirection );
 

// TOPMOST manipulation functions
LEVELNODE  *AddTopmostToTail( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN AddTopmostToHead( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN RemoveTopmost( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN TypeExistsInTopmostLayer( INT32 iMapIndex, UINT32 fType, UINT16 *pusTopmostIndex );
BOOLEAN RemoveAllTopmostsOfTypeRange( INT32 iMapIndex, UINT32 fStartType, UINT32 fEndType );
BOOLEAN SetMapElementShadeLevel( INT32 uiMapIndex, UINT8 ubShadeLevel );
void SetTopmostFlags( INT32 iMapIndex, UINT32 uiFlags, UINT16 usIndex );
void RemoveTopmostFlags( INT32 iMapIndex, UINT32 uiFlags, UINT16 usIndex );
BOOLEAN AddUIElem( INT32 iMapIndex, UINT16 usIndex, INT8	sRelativeX, INT8 sRelativeY, LEVELNODE **ppNewNode );
void RemoveUIElem( INT32 iMapIndex, UINT16 usIndex );
BOOLEAN RemoveTopmostFromLevelNode( INT32 iMapIndex, LEVELNODE *pNode );


BOOLEAN IsLowerLevel( INT32 sGridNo );
BOOLEAN IsHeigherLevel( INT32 sGridNo );
BOOLEAN IsRoofVisible( INT32 sMapPos );
BOOLEAN IsRoofVisible2( INT32 sMapPos );


LEVELNODE * FindLevelNodeBasedOnStructure( INT32 sGridNo, STRUCTURE * pStructure );
LEVELNODE * FindShadow( INT32 sGridNo, UINT16 usStructIndex );

void WorldHideTrees( );
void WorldShowTrees( );


BOOLEAN IsTileRedundent( UINT32 uiDestPitchBYTES, UINT16 *pZBuffer, UINT16 usZValue, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );

//this is found in editscreen.c
//Andrew, you had worldman.c checked out at the time, so I stuck it here.
//The best thing to do is toast it here, and include editscreen.h in worldman.c.
extern UINT32	gCurrentBackground;

void		SetTreeTopStateForMap();

#endif