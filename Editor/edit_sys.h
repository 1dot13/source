#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __EDIT_SYS_H
#define __EDIT_SYS_H

#define	SMALLBRUSH		0
#define	MEDIUMBRUSH		1
#define	LARGEBRUSH		2

#define	NO_BANKS		0 
#define	DRAW_BANKS		1
#define	DRAW_BANK_WATER 2
#define	DRAW_ERASE		3

#define	NO_CLIFFS		0 
#define	DRAW_CLIFFS		1
#define	DRAW_CLIFF_LAND 2

extern BOOLEAN	gfWarning;	

extern BOOLEAN	gfDoFill;	
extern UINT16	CurrentPaste;
extern UINT16	gDebrisPaste;
extern UINT16	gChangeElevation;
extern UINT16	CurrentStruct;
extern UINT32	gDoBanks;
extern UINT32	gDoCliffs;

void EraseMapTile( INT32 iMapIndex );
void QuickEraseMapTile( INT32 iMapIndex );
void DeleteStuffFromMapTile(INT32 iMapIndex);//dnl ch86 140214

void PasteDebris( INT32 iMapIndex );

void PasteStructure( INT32 iMapIndex );
void PasteStructure1( INT32 iMapIndex );
void PasteStructure2( INT32 iMapIndex );
void PasteStructureCommon( INT32 iMapIndex );

void PasteSingleWall( INT32 iMapIndex );
void PasteSingleDoor( INT32 iMapIndex );
void PasteSingleWindow( INT32 iMapIndex );
void PasteSingleRoof( INT32 iMapIndex );
void PasteSingleBrokenWall( INT32 iMapIndex );
void PasteSingleDecoration( INT32 iMapIndex );
void PasteSingleDecal( INT32 iMapIndex );
void PasteSingleFloor( INT32 iMapIndex );
void PasteSingleToilet( INT32 iMapIndex );
void PasteRoomNumber( INT32 iMapIndex, UINT16 usRoomNumber );

void PasteSingleWallCommon( INT32 iMapIndex );

UINT16 GetRandomIndexByRange( UINT16 usRangeStart, UINT16 usRangeEnd );
UINT16 GetRandomTypeByRange( UINT16 usRangeStart, UINT16 usRangeEnd );

void PasteFloor( INT32 iMapIndex, UINT16 usFloorIndex , BOOLEAN fReplace);

void PasteBanks( INT32 iMapIndex, UINT16 usStructIndex, BOOLEAN fReplace );
void PasteRoads( INT32 iMapIndex );
void PasteCliffs( INT32 iMapIndex, UINT16 usStructIndex , BOOLEAN fReplace);

void PasteTexture( INT32 iMapIndex );
void PasteTextureCommon( INT32 iMapIndex );

void PasteHigherTexture( INT32 iMapIndex, UINT32 fNewType );

//dnl ch3 230909
void RaiseWorldLand(void);
void RaiseWorldLandOld(void);

void EliminateObjectLayerRedundancy();


#endif
#endif



 


