#ifndef __INTERACTIVE_TILES_H
#define __INTERACTIVE_TILES_H


#define INTTILE_DOOR_TILE_ONE			1

#define INTTILE_DOOR_OPENSPEED	70

#define	INTILE_CHECK_FULL						1
#define	INTILE_CHECK_SELECTIVE			2


extern BOOLEAN		gfOverIntTile;


void GetLevelNodeScreenRect( LEVELNODE *pNode, SGPRect *pRect, INT16 sXPos, INT16 sYPos, INT32 sGridNo );
BOOLEAN InitInteractiveTileManagement( );
void ShutdownInteractiveTileManagement( );
BOOLEAN AddInteractiveTile( INT32 sGridNo, LEVELNODE *pLevelNode, UINT32 uiFlags, UINT16 usType );
BOOLEAN StartInteractiveObject( INT32 sGridNo, UINT16 usStructureID, SOLDIERTYPE *pSoldier, UINT8 ubDirection );
BOOLEAN StartInteractiveObjectFromMouse( SOLDIERTYPE *pSoldier, UINT8 ubDirection );
void CompileInteractiveTiles( );
UINT32	GetInteractiveTileCursor( UINT32 uiOldCursor, BOOLEAN fConfirm );
BOOLEAN InteractWithInteractiveObject( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection );
BOOLEAN SoldierHandleInteractiveObject( SOLDIERTYPE *pSoldier );

BOOLEAN CalcInteractiveObjectAPs( SOLDIERTYPE * pSoldier, INT32 sGridNo, STRUCTURE * pStructure, INT16 *psAPCost, INT16 *psBPCost ); // SANDRO - added argument

void HandleStructChangeFromGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo );


void BeginCurInteractiveTileCheck( UINT8 bCheckFlags );
void EndCurInteractiveTileCheck( );
void LogMouseOverInteractiveTile( INT32 sGridNo );
BOOLEAN ShouldCheckForMouseDetections( );
 
void CycleIntTileFindStack( INT32 usMapPos );
void SetActionModeDoorCursorText( );

LEVELNODE *GetCurInteractiveTile( );
LEVELNODE *GetCurInteractiveTileGridNo( INT32 *psGridNo );
LEVELNODE *GetCurInteractiveTileGridNoAndStructure( INT32 *psGridNo, STRUCTURE **ppStructure );
LEVELNODE *ConditionalGetCurInteractiveTileGridNoAndStructure( INT32 *psGridNo, STRUCTURE **ppStructure, BOOLEAN fRejectOnTopItems );





#endif
