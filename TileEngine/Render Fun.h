#ifndef __RENDER_FUN_H
#define __RENDER_FUN_H

#include "worlddef.h"

#define		NO_ROOM			0
//DBrot: More Rooms
//#define	MAX_ROOMS		250
#define		MAX_ROOMS		65530

extern UINT8	gubWorldRoomHidden[ MAX_ROOMS ];
extern UINT16*	gusWorldRoomInfo;

BOOLEAN InitRoomDatabase( );
void ShutdownRoomDatabase( );

//void SetTileRoomNum( INT32 sGridNo, UINT8 ubRoomNum );
//void SetTileRangeRoomNum( SGPRect *pSelectRegion, UINT8 ubRoomNum );

//void RemoveRoomRoof( INT32 sGridNo, UINT8 bRoomNum, SOLDIERTYPE *pSoldier );
//BOOLEAN InARoom( INT32 sGridNo, UINT8 *pubRoomNo );
//BOOLEAN InAHiddenRoom( INT32 sGridNo, UINT8 *pubRoomNo );
void SetTileRoomNum( INT32 sGridNo, UINT16 usRoomNum );
void SetTileRangeRoomNum( SGPRect *pSelectRegion, UINT16 usRoomNum );

void RemoveRoomRoof( INT32 sGridNo, UINT16 usRoomNum, SOLDIERTYPE *pSoldier );
BOOLEAN InARoom( INT32 sGridNo, UINT16 *pusRoomNo );
BOOLEAN InAHiddenRoom( INT32 sGridNo, UINT16 *pusRoomNo );

void SetGridNoRevealedFlag( INT32 sGridNo );

void ExamineGridNoForSlantRoofExtraGraphic( INT32 sCheckGridNo );

void SetRecalculateWireFrameFlagRadius(INT16 sX, INT16 sY, INT16 sRadius);


#endif
