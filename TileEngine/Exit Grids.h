#ifndef __EXIT_GRIDS_H
#define __EXIT_GRIDS_H

#include "Fileman.h"
#include "Worlddef.h"

//dnl ch42 250909
typedef struct
{
	INT16 sMapIndex;//dnl ch86 170214 not exist in v1.12, add to simplify load
	INT16 usGridNo;
	UINT8 ubGotoSectorX;
	UINT8 ubGotoSectorY;
	UINT8 ubGotoSectorZ;
}_OLD_EXITGRID;

class EXITGRID// For exit grids (object level)
{
// If an item pool is also in same gridno, then this would be a separate levelnode in the object level list
public:
	INT32 iMapIndex;//dnl ch86 170214
	INT32 usGridNo;// Sweet spot for placing mercs in new sector.
	UINT8 ubGotoSectorX;
	UINT8 ubGotoSectorY;
	UINT8 ubGotoSectorZ;
public:
	EXITGRID() : iMapIndex(-1), usGridNo(0), ubGotoSectorX(1), ubGotoSectorY(1), ubGotoSectorZ(0) {}//dnl ch86 180214
	EXITGRID& operator=(const _OLD_EXITGRID& src);
	BOOLEAN Load(INT8** hBuffer, FLOAT dMajorMapVersion);
	BOOLEAN Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
	BOOLEAN Equal(EXITGRID &ExitGrid) { return(!memcmp(&this->usGridNo, &ExitGrid.usGridNo, sizeof(usGridNo)+sizeof(ubGotoSectorX)+sizeof(ubGotoSectorY)+sizeof(ubGotoSectorZ))); }
};

BOOLEAN	ExitGridAtGridNo( UINT32 usMapIndex );
BOOLEAN	GetExitGridLevelNode( UINT32 usMapIndex, LEVELNODE **ppLevelNode );
BOOLEAN	GetExitGrid( UINT32 usMapIndex, EXITGRID *pExitGrid );

void AddExitGridToWorld( INT32 iMapIndex, EXITGRID *pExitGrid );
void RemoveExitGridFromWorld( INT32 iMapIndex );
void TrashExitGridTable(void);//dnl ch86 170214
//dnl ch42 250909
void SaveExitGrids(HWFILE hFile, UINT16 usNumExitGrids, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
void LoadExitGrids(INT8** hBuffer, FLOAT dMajorMapVersion);

void AttemptToChangeFloorLevel( INT8 bRelativeZLevel );

extern EXITGRID gExitGrid;
extern BOOLEAN gfOverrideInsertionWithExitGrid;
extern BOOLEAN gfShowExitGrids;//dnl ch86 190214

// Finds closest ExitGrid of same type as is at gridno, within a radius.	Checks
// valid paths, destinations, etc.
INT32 FindGridNoFromSweetSpotCloseToExitGrid( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );
INT32 FindClosestExitGrid( SOLDIERTYPE *pSoldier, INT32 sSrcGridNo, INT16 ubRadius );

#endif
