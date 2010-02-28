#ifndef __EXIT_GRIDS_H
#define __EXIT_GRIDS_H

#include "Fileman.h"
#include "Worlddef.h"

//dnl ch42 250909
typedef struct
{
	INT16 usGridNo;
	UINT8 ubGotoSectorX;
	UINT8 ubGotoSectorY;
	UINT8 ubGotoSectorZ;
}_OLD_EXITGRID;

class EXITGRID// For exit grids (object level)
{
// If an item pool is also in same gridno, then this would be a separate levelnode in the object level list
public:
	INT32 usGridNo;// Sweet spot for placing mercs in new sector.
	UINT8 ubGotoSectorX;
	UINT8 ubGotoSectorY;
	UINT8 ubGotoSectorZ;
public:
	EXITGRID& operator=(const _OLD_EXITGRID& src);
	BOOLEAN Load(INT8** hBuffer, FLOAT dMajorMapVersion);
	BOOLEAN Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
};

BOOLEAN	ExitGridAtGridNo( UINT32 usMapIndex );
BOOLEAN	GetExitGridLevelNode( UINT32 usMapIndex, LEVELNODE **ppLevelNode );
BOOLEAN	GetExitGrid( UINT32 usMapIndex, EXITGRID *pExitGrid );

void AddExitGridToWorld( INT32 iMapIndex, EXITGRID *pExitGrid );
void RemoveExitGridFromWorld( INT32 iMapIndex );
//dnl ch42 250909
void SaveExitGrids(HWFILE hFile, UINT16 usNumExitGrids, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion);
void LoadExitGrids(INT8** hBuffer, FLOAT dMajorMapVersion);

void AttemptToChangeFloorLevel( INT8 bRelativeZLevel );

extern EXITGRID gExitGrid;
extern BOOLEAN gfOverrideInsertionWithExitGrid;

// Finds closest ExitGrid of same type as is at gridno, within a radius.	Checks
// valid paths, destinations, etc.
INT32 FindGridNoFromSweetSpotCloseToExitGrid( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

INT32 FindClosestExitGrid( SOLDIERTYPE *pSoldier, INT32 sSrcGridNo, INT8 ubRadius );

#endif
