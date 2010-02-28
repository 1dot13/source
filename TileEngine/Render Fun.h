#ifndef __RENDER_FUN_H
#define __RENDER_FUN_H

#include <stdio.h>
#include "worlddef.h"

#define		NO_ROOM			0
#define	MAX_ROOMS		250


extern UINT8	gubWorldRoomHidden[ MAX_ROOMS ];
extern UINT8*						gubWorldRoomInfo;


BOOLEAN InitRoomDatabase( );
void ShutdownRoomDatabase( );

void SetTileRoomNum( INT32 sGridNo, UINT8 ubRoomNum );
void SetTileRangeRoomNum( SGPRect *pSelectRegion, UINT8 ubRoomNum );

void RemoveRoomRoof( INT32 sGridNo, UINT8 bRoomNum, SOLDIERTYPE *pSoldier );
BOOLEAN InARoom( INT32 sGridNo, UINT8 *pubRoomNo );
BOOLEAN InAHiddenRoom( INT32 sGridNo, UINT8 *pubRoomNo );

void SetGridNoRevealedFlag( INT32 sGridNo );

void ExamineGridNoForSlantRoofExtraGraphic( INT32 sCheckGridNo );

void SetRecalculateWireFrameFlagRadius(INT16 sX, INT16 sY, INT16 sRadius);


#endif