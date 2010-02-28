#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include <memory.h>
	#include "Map Edgepoints.h"

	#include "pathai.h"
	#include "ai.h"
	#include "Map Information.h"
	#include "renderworld.h"
	#include "Isometric Utils.h"
	#include "debug.h"
	#include "Random.h"
	#include "strategic.h"
	#include "Animation Control.h"
	#include "Render Fun.h"
	#include "strategicmap.h"
	#include "environment.h"
	#include "worldman.h"
#endif

#include "connect.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


//dynamic arrays that contain the valid gridno's for each edge
INT32 *gps1stNorthEdgepointArray					= NULL;
INT32 *gps1stEastEdgepointArray						= NULL;
INT32 *gps1stSouthEdgepointArray					= NULL;
INT32 *gps1stWestEdgepointArray						= NULL;

// WANNE - MP: Center
INT32 *gps1stCenterEdgepointArray					= NULL;
UINT16 gus1stCenterEdgepointArraySize				= 0;

//contains the size for each array
UINT16 gus1stNorthEdgepointArraySize			= 0;
UINT16 gus1stEastEdgepointArraySize				= 0;
UINT16 gus1stSouthEdgepointArraySize			= 0;
UINT16 gus1stWestEdgepointArraySize				= 0;
//contains the index value for the first array index of the second row of each edgepoint array.
//Because each edgepoint side has two rows, the outside most row is calculated first, then the inside row.
//For purposes of AI, it may become necessary to avoid this.
UINT16 gus1stNorthEdgepointMiddleIndex		= 0;
UINT16 gus1stEastEdgepointMiddleIndex			= 0;
UINT16 gus1stSouthEdgepointMiddleIndex		= 0;
UINT16 gus1stWestEdgepointMiddleIndex			= 0;

//dynamic arrays that contain the valid gridno's for each edge
INT32 *gps2ndNorthEdgepointArray					= NULL;
INT32 *gps2ndEastEdgepointArray						= NULL;
INT32 *gps2ndSouthEdgepointArray					= NULL;
INT32 *gps2ndWestEdgepointArray						= NULL;
//contains the size for each array
UINT16 gus2ndNorthEdgepointArraySize			= 0;
UINT16 gus2ndEastEdgepointArraySize				= 0;
UINT16 gus2ndSouthEdgepointArraySize			= 0;
UINT16 gus2ndWestEdgepointArraySize				= 0;
//contains the index value for the first array index of the second row of each edgepoint array.
//Because each edgepoint side has two rows, the outside most row is calculated first, then the inside row.
//For purposes of AI, it may become necessary to avoid this.
UINT16 gus2ndNorthEdgepointMiddleIndex		= 0;
UINT16 gus2ndEastEdgepointMiddleIndex			= 0;
UINT16 gus2ndSouthEdgepointMiddleIndex		= 0;
UINT16 gus2ndWestEdgepointMiddleIndex			= 0;

BOOLEAN gfEdgepointsExist = FALSE;
BOOLEAN gfGeneratingMapEdgepoints = FALSE;

BOOLEAN VerifyEdgepoint( SOLDIERTYPE *pSoldier, INT32 sEdgepoint );
BOOLEAN EdgepointsClose( SOLDIERTYPE *pSoldier, INT32 sEdgepoint1, INT32 sEdgepoint2 );

extern UINT8 gubTacticalDirection;

// WANNE - MP: Center
void InitCenterEdgepoint()
{
	if (gps1stCenterEdgepointArray )
		MemFree( gps1stCenterEdgepointArray );

	gps1stCenterEdgepointArray = NULL;

	// Set some center points
	gus1stCenterEdgepointArraySize = 30;

	gps1stCenterEdgepointArray = (INT32*)MemAlloc( gus1stCenterEdgepointArraySize * sizeof( INT32 ) );

	gps1stCenterEdgepointArray[ 0 ] = 13024;
	gps1stCenterEdgepointArray[ 1 ] = 13507;
	gps1stCenterEdgepointArray[ 2 ] = 13990;
	gps1stCenterEdgepointArray[ 3 ] = 14473;
	gps1stCenterEdgepointArray[ 4 ] = 15117;
	gps1stCenterEdgepointArray[ 5 ] = 12547;
	gps1stCenterEdgepointArray[ 6 ] = 13030;
	gps1stCenterEdgepointArray[ 7 ] = 13513;
	gps1stCenterEdgepointArray[ 8 ] = 13996;
	gps1stCenterEdgepointArray[ 9 ] = 14479;
	gps1stCenterEdgepointArray[ 10 ] = 12070;
	gps1stCenterEdgepointArray[ 11 ] = 12553;
	gps1stCenterEdgepointArray[ 12 ] = 13036;
	gps1stCenterEdgepointArray[ 13 ] = 13519;
	gps1stCenterEdgepointArray[ 14 ] = 14002;
	gps1stCenterEdgepointArray[ 15 ] = 11593;
	gps1stCenterEdgepointArray[ 16 ] = 12076;
	gps1stCenterEdgepointArray[ 17 ] = 12559;
	gps1stCenterEdgepointArray[ 18 ] = 13042;
	gps1stCenterEdgepointArray[ 19 ] = 13525;
	gps1stCenterEdgepointArray[ 20 ] = 11117;
	gps1stCenterEdgepointArray[ 21 ] = 11600;
	gps1stCenterEdgepointArray[ 22 ] = 12083;
	gps1stCenterEdgepointArray[ 23 ] = 12566;
	gps1stCenterEdgepointArray[ 24 ] = 13049;
	gps1stCenterEdgepointArray[ 25 ] = 10640;
	gps1stCenterEdgepointArray[ 26 ] = 11123;
	gps1stCenterEdgepointArray[ 27 ] = 11606;
	gps1stCenterEdgepointArray[ 28 ] = 12089;
	gps1stCenterEdgepointArray[ 29 ] = 12572;
}

void TrashMapEdgepoints()
{
	//Primary edgepoints
	if( gps1stNorthEdgepointArray )
		MemFree( gps1stNorthEdgepointArray );
	if( gps1stEastEdgepointArray )
		MemFree( gps1stEastEdgepointArray );
	if( gps1stSouthEdgepointArray )
		MemFree( gps1stSouthEdgepointArray );
	if( gps1stWestEdgepointArray )
		MemFree( gps1stWestEdgepointArray );
	
	// WANNE - MP: Center
	if (gps1stCenterEdgepointArray )
		MemFree( gps1stCenterEdgepointArray );
	gps1stCenterEdgepointArray = NULL;
	gus1stCenterEdgepointArraySize = 0;
	
	gps1stNorthEdgepointArray					= NULL;
	gps1stEastEdgepointArray					= NULL;
	gps1stSouthEdgepointArray					= NULL;
	gps1stWestEdgepointArray					=	NULL;
	gus1stNorthEdgepointArraySize			= 0;
	gus1stEastEdgepointArraySize			= 0;
	gus1stSouthEdgepointArraySize			= 0;
	gus1stWestEdgepointArraySize			= 0;
	gus1stNorthEdgepointMiddleIndex		= 0;
	gus1stEastEdgepointMiddleIndex		= 0;
	gus1stSouthEdgepointMiddleIndex		= 0;
	gus1stWestEdgepointMiddleIndex		= 0;

	
	//Secondary edgepoints
	if( gps2ndNorthEdgepointArray )
		MemFree( gps2ndNorthEdgepointArray );
	if( gps2ndEastEdgepointArray )
		MemFree( gps2ndEastEdgepointArray );
	if( gps2ndSouthEdgepointArray )
		MemFree( gps2ndSouthEdgepointArray );
	if( gps2ndWestEdgepointArray )
		MemFree( gps2ndWestEdgepointArray );
	gps2ndNorthEdgepointArray					= NULL;
	gps2ndEastEdgepointArray					= NULL;
	gps2ndSouthEdgepointArray					= NULL;
	gps2ndWestEdgepointArray					=	NULL;
	gus2ndNorthEdgepointArraySize			= 0;
	gus2ndEastEdgepointArraySize			= 0;
	gus2ndSouthEdgepointArraySize			= 0;
	gus2ndWestEdgepointArraySize			= 0;
	gus2ndNorthEdgepointMiddleIndex		= 0;
	gus2ndEastEdgepointMiddleIndex		= 0;
	gus2ndSouthEdgepointMiddleIndex		= 0;
	gus2ndWestEdgepointMiddleIndex		= 0;
}

//This final step eliminates some edgepoints which actually don't path directly to the edge of the map.
//Cases would include an area that is close to the edge, but a fence blocks it from direct access to the edge
//of the map.
void ValidateEdgepoints()
{
	INT32 i;
	UINT16 usValidEdgepoints;
	SOLDIERTYPE Soldier;
	Soldier.bTeam = 1;

	//north
	usValidEdgepoints = 0;
	for( i = 0; i < gus1stNorthEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps1stNorthEdgepointArray[ i ] ) )
		{
			gps1stNorthEdgepointArray[ usValidEdgepoints ] = gps1stNorthEdgepointArray[ i ];
			if( i == gus1stNorthEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus1stNorthEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus1stNorthEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus1stNorthEdgepointMiddleIndex++;
		}
	}
	gus1stNorthEdgepointArraySize = usValidEdgepoints;
	//East
	usValidEdgepoints = 0;
	for( i = 0; i < gus1stEastEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps1stEastEdgepointArray[ i ] ) )
		{
			gps1stEastEdgepointArray[ usValidEdgepoints ] = gps1stEastEdgepointArray[ i ];
			if( i == gus1stEastEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus1stEastEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus1stEastEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus1stEastEdgepointMiddleIndex++;
		}
	}
	gus1stEastEdgepointArraySize = usValidEdgepoints;
	//South
	usValidEdgepoints = 0;
	for( i = 0; i < gus1stSouthEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps1stSouthEdgepointArray[ i ] ) )
		{
			gps1stSouthEdgepointArray[ usValidEdgepoints ] = gps1stSouthEdgepointArray[ i ];
			if( i == gus1stSouthEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus1stSouthEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus1stSouthEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus1stSouthEdgepointMiddleIndex++;
		}
	}
	gus1stSouthEdgepointArraySize = usValidEdgepoints;
	//West
	usValidEdgepoints = 0;
	for( i = 0; i < gus1stWestEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps1stWestEdgepointArray[ i ] ) )
		{
			gps1stWestEdgepointArray[ usValidEdgepoints ] = gps1stWestEdgepointArray[ i ];
			if( i == gus1stWestEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus1stWestEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus1stWestEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus1stWestEdgepointMiddleIndex++;
		}
	}
	gus1stWestEdgepointArraySize = usValidEdgepoints;



	//north
	usValidEdgepoints = 0;
	for( i = 0; i < gus2ndNorthEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps2ndNorthEdgepointArray[ i ] ) )
		{
			gps2ndNorthEdgepointArray[ usValidEdgepoints ] = gps2ndNorthEdgepointArray[ i ];
			if( i == gus2ndNorthEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus2ndNorthEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus2ndNorthEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus2ndNorthEdgepointMiddleIndex++;
		}
	}
	gus2ndNorthEdgepointArraySize = usValidEdgepoints;
	//East
	usValidEdgepoints = 0;
	for( i = 0; i < gus2ndEastEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps2ndEastEdgepointArray[ i ] ) )
		{
			gps2ndEastEdgepointArray[ usValidEdgepoints ] = gps2ndEastEdgepointArray[ i ];
			if( i == gus2ndEastEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus2ndEastEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus2ndEastEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus2ndEastEdgepointMiddleIndex++;
		}
	}
	gus2ndEastEdgepointArraySize = usValidEdgepoints;
	//South
	usValidEdgepoints = 0;
	for( i = 0; i < gus2ndSouthEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps2ndSouthEdgepointArray[ i ] ) )
		{
			gps2ndSouthEdgepointArray[ usValidEdgepoints ] = gps2ndSouthEdgepointArray[ i ];
			if( i == gus2ndSouthEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus2ndSouthEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus2ndSouthEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus2ndSouthEdgepointMiddleIndex++;
		}
	}
	gus2ndSouthEdgepointArraySize = usValidEdgepoints;
	//West
	usValidEdgepoints = 0;
	for( i = 0; i < gus2ndWestEdgepointArraySize; i++ )
	{
		if( VerifyEdgepoint( &Soldier, gps2ndWestEdgepointArray[ i ] ) )
		{
			gps2ndWestEdgepointArray[ usValidEdgepoints ] = gps2ndWestEdgepointArray[ i ];
			if( i == gus2ndWestEdgepointMiddleIndex )
			{ //adjust the middle index to the new one.
				gus2ndWestEdgepointMiddleIndex = usValidEdgepoints;
			}
			usValidEdgepoints++;
		}
		else if( i == gus2ndWestEdgepointMiddleIndex )
		{ //increment the middle index because it's edgepoint is no longer valid.
			gus2ndWestEdgepointMiddleIndex++;
		}
	}
	gus2ndWestEdgepointArraySize = usValidEdgepoints;

}

void CompactEdgepointArray( INT32 **psArray, UINT16 *pusMiddleIndex, UINT16 *pusArraySize )
{
	INT32 i;
	UINT16 usArraySize, usValidIndex = 0;

	usArraySize = *pusArraySize;

	for( i = 0; i < usArraySize; i++ )
	{
		if( (*psArray)[ i ] == -1 )
		{
			(*pusArraySize)--;
			if( i < *pusMiddleIndex )
			{
				(*pusMiddleIndex)--;
			}
		}
		else
		{
			if( usValidIndex != i )
			{
				(*psArray)[ usValidIndex ] = (*psArray)[ i ];
			}
			usValidIndex++;
		}
	}
	*psArray = (INT32*)MemRealloc( *psArray, *pusArraySize * sizeof( INT32 ) );
	Assert( *psArray );
}

void InternallyClassifyEdgepoints( SOLDIERTYPE *pSoldier, INT32 sGridNo, 
																	 INT32 **psArray1, UINT16 *pusMiddleIndex1, UINT16 *pusArraySize1,
																	 INT32 **psArray2, UINT16 *pusMiddleIndex2, UINT16 *pusArraySize2 )
{
	INT32 i;
	UINT16 us1stBenchmarkID, us2ndBenchmarkID;
	us1stBenchmarkID = us2ndBenchmarkID = 0xffff;
	if( !(*psArray2) )
	{
		*psArray2 = (INT32*)MemAlloc( sizeof( INT32 ) * 400 );
	}
	for( i = 0; i < *pusArraySize1; i++ )
	{
		if( sGridNo == (*psArray1)[ i ] )
		{
			if( i < *pusMiddleIndex1 )
			{ //in the first half of the array
				us1stBenchmarkID = (UINT16)i;
				//find the second benchmark
				for( i = *pusMiddleIndex1; i < *pusArraySize1; i++ )
				{
					if( EdgepointsClose( pSoldier, (*psArray1)[ us1stBenchmarkID ], (*psArray1)[ i ] ) )
					{
						us2ndBenchmarkID = (UINT16)i;
						break;
					}
				}
			}
			else
			{ //in the second half of the array
				us2ndBenchmarkID = (UINT16)i;
				//find the first benchmark
				for( i = 0; i < *pusMiddleIndex1; i++ )
				{
					if( EdgepointsClose( pSoldier, (*psArray1)[ us2ndBenchmarkID ], (*psArray1)[ i ] ) )
					{
						us1stBenchmarkID = (UINT16)i;
						break;
					}
				}
			}
			break;
		}
	}
	//Now we have found the two benchmarks, so go in both directions for each one to determine which entrypoints
	//are going to be used in the primary array.	All rejections will be positioned in the secondary array for
	//use for isolated entry when tactically traversing.
	if( us1stBenchmarkID != 0xffff )
	{
		for( i = us1stBenchmarkID; i > 0; i-- )
		{
			if( !EdgepointsClose( pSoldier, (*psArray1)[ i ], (*psArray1)[ i-1 ] ) )
			{ //All edgepoints from index 0 to i-1 are rejected.
				while( i )
				{
					i--;
					(*psArray2)[ *pusArraySize2 ] = (*psArray1)[ i ];
					(*pusMiddleIndex2)++;
					(*pusArraySize2)++;
					(*psArray1)[ i ] = -1;
				}
				break;
			}
		}
		for( i = us1stBenchmarkID; i < *pusMiddleIndex1 - 1; i++ )
		{
			if( !EdgepointsClose( pSoldier, (*psArray1)[ i ], (*psArray1)[ i+1 ] ) )
			{ //All edgepoints from index i+1 to 1st middle index are rejected.
				while( i < *pusMiddleIndex1 - 1 )
				{
					i++;
					(*psArray2)[ *pusArraySize2 ] = (*psArray1)[ i ];
					(*pusMiddleIndex2)++;
					(*pusArraySize2)++;
					(*psArray1)[ i ] = -1;
				}
				break;
			}
		}
	}
	if( us2ndBenchmarkID != 0xffff )
	{
		for( i = us2ndBenchmarkID; i > *pusMiddleIndex1; i-- )
		{
			if( !EdgepointsClose( pSoldier, (*psArray1)[ i ], (*psArray1)[ i-1 ] ) )
			{ //All edgepoints from 1st middle index	to i-1 are rejected.
				while( i > *pusMiddleIndex1 )
				{
					i--;
					(*psArray2)[ *pusArraySize2 ] = (*psArray1)[ i ];
					(*pusArraySize2)++;
					(*psArray1)[ i ] = -1;
				}
				break;
			}
		}
		for( i = us2ndBenchmarkID; i < *pusArraySize1 - 1; i++ )
		{
			if( !EdgepointsClose( pSoldier, (*psArray1)[ i ], (*psArray1)[ i+1 ] ) )
			{ //All edgepoints from index 0 to i-1 are rejected.
				while( i < *pusArraySize1 - 1 )
				{
					i++;
					(*psArray2)[ (*pusArraySize2) ] = (*psArray1)[ i ];
					(*pusArraySize2)++;
					(*psArray1)[ i ] = -1;
				}
				break;
			}
		}
	}
	//Now compact the primary array, because some edgepoints have been removed.
	CompactEdgepointArray( psArray1, pusMiddleIndex1, pusArraySize1 );
	(*psArray2) = (INT32*)MemRealloc( (*psArray2), *pusArraySize2 * sizeof( INT32 ) );
}

void ClassifyEdgepoints()
{
	SOLDIERTYPE Soldier;
	INT32 sGridNo = -1;

	Soldier.bTeam = 1;

	//north
	if( gMapInformation.sNorthGridNo != -1 )
	{
		sGridNo = FindNearestEdgepointOnSpecifiedEdge( gMapInformation.sNorthGridNo, NORTH_EDGEPOINT_SEARCH );
		
		if(!TileIsOutOfBounds(sGridNo))
		{
			InternallyClassifyEdgepoints( &Soldier, sGridNo,
				&gps1stNorthEdgepointArray, &gus1stNorthEdgepointMiddleIndex, &gus1stNorthEdgepointArraySize,
				&gps2ndNorthEdgepointArray, &gus2ndNorthEdgepointMiddleIndex, &gus2ndNorthEdgepointArraySize );
		}
	}
	//east
	if( gMapInformation.sEastGridNo != -1 )
	{
		sGridNo = FindNearestEdgepointOnSpecifiedEdge( gMapInformation.sEastGridNo, EAST_EDGEPOINT_SEARCH );
		
		if(!TileIsOutOfBounds(sGridNo))
		{
			InternallyClassifyEdgepoints( &Soldier, sGridNo,
				&gps1stEastEdgepointArray, &gus1stEastEdgepointMiddleIndex, &gus1stEastEdgepointArraySize,
				&gps2ndEastEdgepointArray, &gus2ndEastEdgepointMiddleIndex, &gus2ndEastEdgepointArraySize );
		}
	}
	//south
	if( gMapInformation.sSouthGridNo != -1 )
	{
		sGridNo = FindNearestEdgepointOnSpecifiedEdge( gMapInformation.sSouthGridNo, SOUTH_EDGEPOINT_SEARCH );
		
		if(!TileIsOutOfBounds(sGridNo))
		{
			InternallyClassifyEdgepoints( &Soldier, sGridNo,
				&gps1stSouthEdgepointArray, &gus1stSouthEdgepointMiddleIndex, &gus1stSouthEdgepointArraySize,
				&gps2ndSouthEdgepointArray, &gus2ndSouthEdgepointMiddleIndex, &gus2ndSouthEdgepointArraySize );
		}
	}
	//west
	if( gMapInformation.sWestGridNo != -1 )
	{
		sGridNo = FindNearestEdgepointOnSpecifiedEdge( gMapInformation.sWestGridNo, WEST_EDGEPOINT_SEARCH );
		
		if(!TileIsOutOfBounds(sGridNo))
		{
			InternallyClassifyEdgepoints( &Soldier, sGridNo,
				&gps1stWestEdgepointArray, &gus1stWestEdgepointMiddleIndex, &gus1stWestEdgepointArraySize,
				&gps2ndWestEdgepointArray, &gus2ndWestEdgepointMiddleIndex, &gus2ndWestEdgepointArraySize );
		}
	}
}

//dnl ch53 101009
void GenerateMapEdgepoints(BOOLEAN fValidate)
{
	INT32 i, x, y;
	INT32 sGridNo;
	INT32* sVGridNo;//dnl ch43 290909

	// Get rid of the current edgepoint lists.
	TrashMapEdgepoints();
	gfGeneratingMapEdgepoints = TRUE;

	if(gMapInformation.sNorthGridNo != -1)
		sGridNo = gMapInformation.sNorthGridNo;
	else if(gMapInformation.sEastGridNo != -1)
		sGridNo = gMapInformation.sEastGridNo;
	else if(gMapInformation.sSouthGridNo != -1)
		sGridNo = gMapInformation.sSouthGridNo;
	else if(gMapInformation.sWestGridNo != -1)
		sGridNo = gMapInformation.sWestGridNo;
	else if(gMapInformation.sCenterGridNo != -1)
		sGridNo = gMapInformation.sCenterGridNo;
	else
		return;
	sVGridNo = (INT32*)MemAlloc(sizeof(UINT32)*__max(WORLD_ROWS, WORLD_COLS)*10);
	Assert(sVGridNo);
	GlobalReachableTest(sGridNo);

	// Calculate the north edges
	if(gMapInformation.sNorthGridNo != NOWHERE)
	{
		for(i=0; i<5; i++)
		{
			x = 0 + i;
			y = WORLD_ROWS/2-1 + i;
			for(x, y; x<WORLD_COLS/2+i; x++, y--)
			{
				sGridNo = x + y * WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stNorthEdgepointArraySize++] = sGridNo;
				++sGridNo;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stNorthEdgepointArraySize++] = sGridNo;
			}
		}
		if(gus1stNorthEdgepointArraySize)
		{
			gps1stNorthEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus1stNorthEdgepointArraySize);
			Assert(gps1stNorthEdgepointArray);
			memcpy(gps1stNorthEdgepointArray, sVGridNo, sizeof(INT32)*gus1stNorthEdgepointArraySize);
		}
	}
	// Calculate the east edges
	if(gMapInformation.sEastGridNo != NOWHERE)
	{
		for(i=0; i<3; i++)
		{
			x = WORLD_COLS/2 - i;
			y = 0 + i;
			for(x, y; x<WORLD_COLS-i; x++, y++)
			{
				sGridNo = x + y * WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stEastEdgepointArraySize++] = sGridNo;
				sGridNo += WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stEastEdgepointArraySize++] = sGridNo;
			}
		}
		if(gus1stEastEdgepointArraySize)
		{
			gps1stEastEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus1stEastEdgepointArraySize);
			Assert(gps1stEastEdgepointArray);
			memcpy(gps1stEastEdgepointArray, sVGridNo, sizeof(INT32)*gus1stEastEdgepointArraySize);
		}
	}
	// Calculate the south edges
	if(gMapInformation.sSouthGridNo != NOWHERE)
	{
		for(i=0; i<3; i++)
		{
			x = WORLD_COLS-1 - i;
			y = WORLD_ROWS/2 - i;
			for(x, y; x>=WORLD_COLS/2-i; x--, y++)
			{
				sGridNo = x + y * WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stSouthEdgepointArraySize++] = sGridNo;
				--sGridNo;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stSouthEdgepointArraySize++] = sGridNo;
			}
		}
		if(gus1stSouthEdgepointArraySize)
		{
			gps1stSouthEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus1stSouthEdgepointArraySize);
			Assert(gps1stSouthEdgepointArray);
			memcpy(gps1stSouthEdgepointArray, sVGridNo, sizeof(INT32)*gus1stSouthEdgepointArraySize);
		}
	}
	// Calculate the west edges
	if(gMapInformation.sWestGridNo != NOWHERE)
	{
		for(i=0; i<3; i++)
		{
			x = WORLD_COLS/2-1 + i;
			y = WORLD_ROWS-1 - i;
			for(x, y; x>=0+i; x--, y--)
			{
				sGridNo = x + y * WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stWestEdgepointArraySize++] = sGridNo;
				sGridNo -= WORLD_COLS;
				if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
					sVGridNo[gus1stWestEdgepointArraySize++] = sGridNo;
			}
		}
		if(gus1stWestEdgepointArraySize)
		{
			gps1stWestEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus1stWestEdgepointArraySize);
			Assert(gps1stWestEdgepointArray);
			memcpy(gps1stWestEdgepointArray, sVGridNo, sizeof(INT32)*gus1stWestEdgepointArraySize);
		}
	}

	// CHECK FOR ISOLATED EDGEPOINTS (but only if the entrypoint is ISOLATED!!!)
	if(gMapInformation.sIsolatedGridNo != NOWHERE && !(gpWorldLevelData[gMapInformation.sIsolatedGridNo].uiFlags & MAPELEMENT_REACHABLE))
	{
		GlobalReachableTest(gMapInformation.sIsolatedGridNo);
		// Calculate the north isolated edges
		if(gMapInformation.sNorthGridNo != NOWHERE)
		{
			for(i=0; i<5; i++)
			{
				x = 0 + i;
				y = WORLD_ROWS/2-1 + i;
				for(x, y; x<WORLD_COLS/2+i; x++, y--)
				{
					sGridNo = x + y * WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndNorthEdgepointArraySize++] = sGridNo;
					++sGridNo;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndNorthEdgepointArraySize++] = sGridNo;
				}
			}
			if(gus2ndNorthEdgepointArraySize)
			{
				gps2ndNorthEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus2ndNorthEdgepointArraySize);
				Assert(gps2ndNorthEdgepointArray);
				memcpy(gps2ndNorthEdgepointArray, sVGridNo, sizeof(INT32)*gus2ndNorthEdgepointArraySize);
			}
		}
		// Calculate the east isolated edges
		if(gMapInformation.sEastGridNo != NOWHERE)
		{
			for(i=0; i<3; i++)
			{
				x = WORLD_COLS/2 - i;
				y = 0 + i;
				for(x, y; x<WORLD_COLS-i; x++, y++)
				{
					sGridNo = x + y * WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndEastEdgepointArraySize++] = sGridNo;
					sGridNo += WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndEastEdgepointArraySize++] = sGridNo;
				}
			}
			if(gus2ndEastEdgepointArraySize)
			{
				gps2ndEastEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus2ndEastEdgepointArraySize);
				Assert(gps2ndEastEdgepointArray);
				memcpy(gps2ndEastEdgepointArray, sVGridNo, sizeof(INT32)*gus2ndEastEdgepointArraySize);
			}
		}
		// Calculate the south isolated edges
		if(gMapInformation.sSouthGridNo != NOWHERE)
		{
			for(i=0; i<3; i++)
			{
				x = WORLD_COLS-1 - i;
				y = WORLD_ROWS/2 - i;
				for(x, y; x>=WORLD_COLS/2-i; x--, y++)
				{
					sGridNo = x + y * WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndSouthEdgepointArraySize++] = sGridNo;
					--sGridNo;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndSouthEdgepointArraySize++] = sGridNo;
				}
			}
			if(gus2ndSouthEdgepointArraySize)
			{
				gps2ndSouthEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus2ndSouthEdgepointArraySize);
				Assert(gps2ndSouthEdgepointArray);
				memcpy(gps2ndSouthEdgepointArray, sVGridNo, sizeof(INT32)*gus2ndSouthEdgepointArraySize);
			}
		}
		// Calculate the west isolated edges
		if(gMapInformation.sWestGridNo != NOWHERE)
		{
			for(i=0; i<3; i++)
			{
				x = WORLD_COLS/2-1 + i;
				y = WORLD_ROWS-1 - i;
				for(x, y; x>=0+i; x--, y--)
				{
					sGridNo = x + y * WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndWestEdgepointArraySize++] = sGridNo;
					sGridNo -= WORLD_COLS;
					if(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE && (!gubWorldRoomInfo[sGridNo] || gfBasement))
						sVGridNo[gus2ndWestEdgepointArraySize++] = sGridNo;
				}
			}
			if(gus2ndWestEdgepointArraySize)
			{
				gps2ndWestEdgepointArray = (INT32*)MemAlloc(sizeof(INT32)*gus2ndWestEdgepointArraySize);
				Assert(gps2ndWestEdgepointArray);
				memcpy(gps2ndWestEdgepointArray, sVGridNo, sizeof(INT32)*gus2ndWestEdgepointArraySize);
			}
		}
	}

	// Eliminates any edgepoints not accessible to the edge of the world. This is done to the primary edgepoints
	if(fValidate)//dnl ch43 290909 Vaildate only before saving map
		ValidateEdgepoints();
	// Second step is to process the primary edgepoints and determine if any of the edgepoints aren't accessible from the associated entrypoint. These edgepoints that are rejected are placed in the secondary list.
	if(gMapInformation.sIsolatedGridNo != NOWHERE)
	{
		// Only if there is an isolated gridno in the map. There is a flaw in the design of this system. The classification
		// process will automatically assign areas to be isolated if there is an obstacle between one normal edgepoint and another
		// causing a 5 tile connection check to fail. So, all maps with isolated edgepoints will need to be checked manually to
		// make sure there are no obstacles causing this to happen (except for obstacles between normal areas and the isolated area)

		// Good thing most maps don't have isolated sections. This is one expensive function to call! Maybe 200MI!
		ClassifyEdgepoints();
	}
	gfGeneratingMapEdgepoints = FALSE;
	MemFree(sVGridNo);//dnl ch43 290909
}

//dnl ch33 240909
void WriteMapEdgepoints(HWFILE fp, INT16 EdgepointArraySize, INT16 EdgepointMiddleIndex, INT32* EdgepointArray, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	UINT32 uiBytesWritten;
	FileWrite(fp, &EdgepointArraySize, sizeof(INT16), &uiBytesWritten);
	FileWrite(fp, &EdgepointMiddleIndex, sizeof(INT16), &uiBytesWritten);
	if(!EdgepointArraySize)
		return;
	if(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION)
	{
		INT16* OldEdgepointArray = new INT16[EdgepointArraySize];
		Assert(OldEdgepointArray);
		TranslateArrayFields(OldEdgepointArray, EdgepointArray, EdgepointArraySize, INT32_INT16);
		FileWrite(fp, OldEdgepointArray, EdgepointArraySize*sizeof(INT16), NULL);
		delete [] OldEdgepointArray;
	}
	else
		FileWrite(fp, EdgepointArray, EdgepointArraySize*sizeof(INT32), NULL);
}

void SaveMapEdgepoints(HWFILE fp, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	//1st priority edgepoints -- for common entry -- tactical placement gui uses only these points.
	WriteMapEdgepoints(fp, gus1stNorthEdgepointArraySize, gus1stNorthEdgepointMiddleIndex, gps1stNorthEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus1stEastEdgepointArraySize, gus1stEastEdgepointMiddleIndex, gps1stEastEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus1stSouthEdgepointArraySize, gus1stSouthEdgepointMiddleIndex, gps1stSouthEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus1stWestEdgepointArraySize, gus1stWestEdgepointMiddleIndex, gps1stWestEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	//2nd priority edgepoints -- for isolated areas.	Okay to be zero
	WriteMapEdgepoints(fp, gus2ndNorthEdgepointArraySize, gus2ndNorthEdgepointMiddleIndex, gps2ndNorthEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus2ndEastEdgepointArraySize, gus2ndEastEdgepointMiddleIndex, gps2ndEastEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus2ndSouthEdgepointArraySize, gus2ndSouthEdgepointMiddleIndex, gps2ndSouthEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
	WriteMapEdgepoints(fp, gus2ndWestEdgepointArraySize, gus2ndWestEdgepointMiddleIndex, gps2ndWestEdgepointArray, dMajorMapVersion, ubMinorMapVersion);
}

// WANNE - BMP: DONE!
void OldLoadMapEdgepoints( INT8 **hBuffer )
{
	LOADDATA( &gus1stNorthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stNorthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stNorthEdgepointArraySize )
	{
		gps1stNorthEdgepointArray = (INT32*)MemAlloc( gus1stNorthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stNorthEdgepointArray );
		LOADDATA( gps1stNorthEdgepointArray, *hBuffer, gus1stNorthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stEastEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stEastEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stEastEdgepointArraySize )
	{
		gps1stEastEdgepointArray = (INT32*)MemAlloc( gus1stEastEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stEastEdgepointArray );
		LOADDATA( gps1stEastEdgepointArray, *hBuffer, gus1stEastEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stSouthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stSouthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stSouthEdgepointArraySize )
	{
		gps1stSouthEdgepointArray = (INT32*)MemAlloc( gus1stSouthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stSouthEdgepointArray );
		LOADDATA( gps1stSouthEdgepointArray, *hBuffer, gus1stSouthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stWestEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stWestEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stWestEdgepointArraySize )
	{
		gps1stWestEdgepointArray = (INT32*)MemAlloc( gus1stWestEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stWestEdgepointArray );
		LOADDATA( gps1stWestEdgepointArray, *hBuffer, gus1stWestEdgepointArraySize * sizeof( INT32 ) );
	}
}

// WANNE - BMP: DONE!
BOOLEAN LoadMapEdgepoints( INT8 **hBuffer, FLOAT dMajorMapVersion )
{
	TrashMapEdgepoints();
	if( gMapInformation.ubMapVersion < 17 )
	{	
		//To prevent invalidation of older maps, which only used one layer of edgepoints, and a UINT8 for
		//containing the size, we will preserve that paradigm, then kill the loaded edgepoints and
		//regenerate them.
		OldLoadMapEdgepoints( hBuffer );
		TrashMapEdgepoints();
		return FALSE;
	}
	LOADDATA( &gus1stNorthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stNorthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stNorthEdgepointArraySize )
	{
		gps1stNorthEdgepointArray = (INT32*)MemAlloc( gus1stNorthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stNorthEdgepointArray );
		memset( gps1stNorthEdgepointArray, 0, gus1stNorthEdgepointArraySize * sizeof( INT32 ) );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus1stNorthEdgepointArraySize * sizeof( INT16 ) );
			memset( gpsOldEdgepointArray, 0, gus1stNorthEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus1stNorthEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus1stNorthEdgepointArraySize; i++)
				gps1stNorthEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps1stNorthEdgepointArray, *hBuffer, gus1stNorthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stEastEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stEastEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stEastEdgepointArraySize )
	{
		gps1stEastEdgepointArray = (INT32*)MemAlloc( gus1stEastEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stEastEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus1stEastEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus1stEastEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus1stEastEdgepointArraySize; i++)
				gps1stEastEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps1stEastEdgepointArray, *hBuffer, gus1stEastEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stSouthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stSouthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stSouthEdgepointArraySize )
	{
		gps1stSouthEdgepointArray = (INT32*)MemAlloc( gus1stSouthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stSouthEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus1stSouthEdgepointArraySize * sizeof( INT16 ) );
			memset( gpsOldEdgepointArray, 0, gus1stSouthEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus1stSouthEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus1stSouthEdgepointArraySize; i++)
				gps1stSouthEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps1stSouthEdgepointArray, *hBuffer, gus1stSouthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus1stWestEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus1stWestEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus1stWestEdgepointArraySize )
	{
		gps1stWestEdgepointArray = (INT32*)MemAlloc( gus1stWestEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps1stWestEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus1stWestEdgepointArraySize * sizeof( INT16 ) );
			memset( gpsOldEdgepointArray, 0, gus1stWestEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus1stWestEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus1stWestEdgepointArraySize; i++)
				gps1stWestEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps1stWestEdgepointArray, *hBuffer, gus1stWestEdgepointArraySize * sizeof( INT32 ) );
	}

	LOADDATA( &gus2ndNorthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus2ndNorthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus2ndNorthEdgepointArraySize )
	{
		gps2ndNorthEdgepointArray = (INT32*)MemAlloc( gus2ndNorthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps2ndNorthEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus2ndNorthEdgepointArraySize * sizeof( INT16 ) );
			memset( gpsOldEdgepointArray, 0, gus2ndNorthEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus2ndNorthEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus2ndNorthEdgepointArraySize; i++)
				gps2ndNorthEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps2ndNorthEdgepointArray, *hBuffer, gus2ndNorthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus2ndEastEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus2ndEastEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus2ndEastEdgepointArraySize )
	{
		gps2ndEastEdgepointArray = (INT32*)MemAlloc( gus2ndEastEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps2ndEastEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus2ndEastEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus2ndEastEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus2ndEastEdgepointArraySize; i++)
				gps2ndEastEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps2ndEastEdgepointArray, *hBuffer, gus2ndEastEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus2ndSouthEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus2ndSouthEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus2ndSouthEdgepointArraySize )
	{
		gps2ndSouthEdgepointArray = (INT32*)MemAlloc( gus2ndSouthEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps2ndSouthEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus2ndSouthEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus2ndSouthEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus2ndSouthEdgepointArraySize; i++)
				gps2ndSouthEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps2ndSouthEdgepointArray, *hBuffer, gus2ndSouthEdgepointArraySize * sizeof( INT32 ) );
	}
	LOADDATA( &gus2ndWestEdgepointArraySize, *hBuffer, 2 );
	LOADDATA( &gus2ndWestEdgepointMiddleIndex, *hBuffer, 2 );
	if( gus2ndWestEdgepointArraySize )
	{
		gps2ndWestEdgepointArray = (INT32*)MemAlloc( gus2ndWestEdgepointArraySize * sizeof( INT32 ) );
		Assert( gps2ndWestEdgepointArray );
		if(dMajorMapVersion < 7.0)
		{
			int i;
			INT16 * gpsOldEdgepointArray = (INT16*)MemAlloc( gus2ndWestEdgepointArraySize * sizeof( INT16 ) );
			memset( gpsOldEdgepointArray, 0, gus2ndWestEdgepointArraySize * sizeof( INT16 ) );
			LOADDATA( gpsOldEdgepointArray, *hBuffer, gus2ndWestEdgepointArraySize * sizeof( INT16 ) );
			for(i=0; i<gus2ndWestEdgepointArraySize; i++)
				gps2ndWestEdgepointArray[i] = gpsOldEdgepointArray[i];
			MemFree(gpsOldEdgepointArray);
		}
		else
			LOADDATA( gps2ndWestEdgepointArray, *hBuffer, gus2ndWestEdgepointArraySize * sizeof( INT32 ) );
	}
	if( gMapInformation.ubMapVersion < 22 )
	{	//regenerate them.
		TrashMapEdgepoints();
		return FALSE;
	}

	return TRUE;
}

INT32 ChooseMapEdgepoint( UINT8 *ubStrategicInsertionCode, UINT8 lastValidICode )
{
	INT32 *psArray=NULL;
	UINT16 usArraySize=0;
	static INT32 randomVal=0;

	//First validate and get access to the correct array based on strategic direction.
	//We will use the selected array to choose insertion gridno's.
	if( *ubStrategicInsertionCode == INSERTION_CODE_GRIDNO)
		*ubStrategicInsertionCode = lastValidICode;

	if (is_networked && *ubStrategicInsertionCode == INSERTION_CODE_CENTER)
	{
		InitCenterEdgepoint();

		psArray = gps1stCenterEdgepointArray;
		usArraySize = gus1stCenterEdgepointArraySize;
	}
	else
	{
		switch( *ubStrategicInsertionCode )
		{
			case INSERTION_CODE_NORTH:
				psArray = gps1stNorthEdgepointArray;
				usArraySize = gus1stNorthEdgepointArraySize;
				break;
			case INSERTION_CODE_EAST:
				psArray = gps1stEastEdgepointArray;
				usArraySize = gus1stEastEdgepointArraySize;
				break;
			case INSERTION_CODE_SOUTH:
				psArray = gps1stSouthEdgepointArray;
				usArraySize = gus1stSouthEdgepointArraySize;
				break;
			case INSERTION_CODE_WEST:
				psArray = gps1stWestEdgepointArray;
				usArraySize = gus1stWestEdgepointArraySize;
				break;
			default:
				AssertMsg( 0, "ChooseMapEdgepoints:	Failed to pass a valid strategic insertion code." );
				break;
		}
	}
	if( !usArraySize )
	{
		return NOWHERE;
	}
	return psArray[ Random( usArraySize ) ];
}

void ChooseMapEdgepoints( MAPEDGEPOINTINFO *pMapEdgepointInfo, UINT8 ubStrategicInsertionCode, UINT8 ubNumDesiredPoints )
{
	INT32 *psArray=NULL;
	UINT16 usArraySize=0;
	INT32 i=-1;
	UINT16 usSlots, usCurrSlot;
	INT32 *psTempArray = NULL;

// WDS 07/20/2008 - beta 4 problem
//	AssertMsg( ubNumDesiredPoints > 0	&& ubNumDesiredPoints <= 32,
//		String( "ChooseMapEdgepoints:	Desired points = %d, valid range is 1-32", ubNumDesiredPoints ) );
	//First validate and get access to the correct array based on strategic direction.
	//We will use the selected array to choose insertion gridno's.
	switch( ubStrategicInsertionCode )
	{
		case INSERTION_CODE_NORTH:
			psArray = gps1stNorthEdgepointArray;
			usArraySize = gus1stNorthEdgepointArraySize;
			break;
		case INSERTION_CODE_EAST:
			psArray = gps1stEastEdgepointArray;
			usArraySize = gus1stEastEdgepointArraySize;
			break;
		case INSERTION_CODE_SOUTH:
			psArray = gps1stSouthEdgepointArray;
			usArraySize = gus1stSouthEdgepointArraySize;
			break;
		case INSERTION_CODE_WEST:
			psArray = gps1stWestEdgepointArray;
			usArraySize = gus1stWestEdgepointArraySize;
			break;
		default:
			AssertMsg( 0, "ChooseMapEdgepoints:	Failed to pass a valid strategic insertion code." );
			break;
	}
	pMapEdgepointInfo->ubStrategicInsertionCode = ubStrategicInsertionCode;
	#ifdef JA2BETAVERSION
		if( !psArray || !usArraySize )
		{
			if( gMapInformation.sNorthGridNo == -1 && gMapInformation.sEastGridNo == -1 &&
				gMapInformation.sSouthGridNo == -1 && gMapInformation.sWestGridNo == -1 &&
					gMapInformation.sCenterGridNo == -1 )
			{
				if( gbWorldSectorZ )
				{
					AssertMsg( 0,
						String( "Map %c%d_b%d(_a) doesn't have ANY entry points which means that it is impossible to generate map edgepoints. (LC : 1)",
						gWorldSectorY + 'A' - 1, gWorldSectorX, gbWorldSectorZ ) );
				}
				else
				{
					AssertMsg( 0,
						String( "Map %c%d(_a) doesn't have ANY entry points which means that it is impossible to generate map edgepoints. (LC : 1)",
						gWorldSectorY + 'A' - 1, gWorldSectorX ) );
				}
			}
			else switch( ubStrategicInsertionCode )
			{
				case INSERTION_CODE_NORTH:
					AssertMsg( 0, "This map doesn't have any north mapedgepoints.	Possibly because there is no north entry point. (LC : 1)" );
					break;
				case INSERTION_CODE_EAST:
					AssertMsg( 0, "This map doesn't have any east mapedgepoints.	Possibly because there is no east entry point. (LC : 1)" );
					break;
				case INSERTION_CODE_SOUTH:
					AssertMsg( 0, "This map doesn't have any south mapedgepoints.	Possibly because there is no south entry point. (LC : 1)" );
					break;
				case INSERTION_CODE_WEST:
					AssertMsg( 0, "This map doesn't have any west mapedgepoints.	Possibly because there is no west entry point.	NOTE:	Teleportation always uses the west entrypoint.	Some maps shouldn't have west entrypoints. (LC : 1)" );
					break;
			}
		}
	#endif
	if( !usArraySize )
	{
		pMapEdgepointInfo->ubNumPoints = 0;
		return;
	}

	// JA2 Gold: don't place people in the water.
	// If any of the waypoints is on a water spot, we're going to have to remove it
	psTempArray = (INT32 *) MemAlloc( sizeof(INT32) * usArraySize );
	memcpy(psTempArray, psArray, sizeof(INT32) * usArraySize );
	psArray = psTempArray;
	for (i = 0; i < usArraySize; i++)
	{
		if (TERRAIN_IS_HIGH_WATER( GetTerrainType(psArray[ i ]) ) )
		{
			if (i == usArraySize - 1)
			{
				// just axe it and we're done.
				psArray[ i ] = 0;
				usArraySize--;
				break;
			}
			else
			{
				// replace this element in the array with the LAST element in the array, then decrement
				// the array size
				psArray[ i ] = psArray[usArraySize-1];
				usArraySize--;
				// we're going to have to check the array element we just copied into this spot, too
				i--;
			}
		}
	}

	if( ubNumDesiredPoints >= usArraySize )
	{ //We don't have enough points for everyone, return them all.
		pMapEdgepointInfo->ubNumPoints = (UINT8)usArraySize;
		for( i = 0; i < usArraySize; i++ )
			pMapEdgepointInfo->sGridNo[i] = psArray[i];

		// JA2Gold: free the temp array
		MemFree(psTempArray);
		return;
	}
	//We have more points, so choose them randomly.
	usSlots = usArraySize;
	usCurrSlot = 0;
	pMapEdgepointInfo->ubNumPoints = ubNumDesiredPoints;
	for( i = 0; i < usArraySize; i++ )
	{
		if( Random( usSlots ) < ubNumDesiredPoints )
		{
			pMapEdgepointInfo->sGridNo[ usCurrSlot++ ] = psArray[ i ];
			ubNumDesiredPoints--;
		}
		usSlots--;
	}

	// JA2Gold: free the temp array
	MemFree(psTempArray);

}


//dnl ch48 051009 Start
INT32* gpReservedGridNos = NULL;
INT16 gsReservedLen = 0;
INT16 gsReservedIndex = 0;

void BeginMapEdgepointSearch()
{
	INT32 sGridNo;
	INT32 iNum1 = gus1stNorthEdgepointArraySize + gus1stEastEdgepointArraySize + gus1stWestEdgepointArraySize + gus1stSouthEdgepointArraySize;
	INT32 iNum2 = gus2ndNorthEdgepointArraySize + gus2ndEastEdgepointArraySize + gus2ndWestEdgepointArraySize + gus2ndSouthEdgepointArraySize;
	gsReservedLen = __max(iNum1, iNum2);
	gsReservedLen = __max(gsReservedLen, TOTAL_SOLDIERS);
	// Create the reserved list
	AssertMsg(!gpReservedGridNos, "Attempting to BeginMapEdgepointSearch that has already been created.");
	gpReservedGridNos = (INT32*)MemAlloc(sizeof(INT32) * gsReservedLen);
	Assert(gpReservedGridNos);
	gsReservedIndex = 0;
	if(gMapInformation.sNorthGridNo != -1)
		sGridNo = gMapInformation.sNorthGridNo;
	else if(gMapInformation.sEastGridNo != -1)
		sGridNo = gMapInformation.sEastGridNo;
	else if(gMapInformation.sSouthGridNo != -1)
		sGridNo = gMapInformation.sSouthGridNo;
	else if(gMapInformation.sWestGridNo != -1)
		sGridNo = gMapInformation.sWestGridNo;
	else
		return;
	GlobalReachableTest(sGridNo);
	// Now, we have the path values calculated. Now, we can check for closest edgepoints.
}

void EndMapEdgepointSearch()
{
	AssertMsg(gpReservedGridNos, "Attempting to EndMapEdgepointSearch that has already been removed.");
	MemFree(gpReservedGridNos);
	gpReservedGridNos = NULL;
	gsReservedLen = 0;
	gsReservedIndex = 0;
}

INT32 FindClosestMapEdgepoint(INT32 sGridNo, INT32* psArray, UINT16 usArraySize)
{
	INT16 sOrigX, sOrigY, sEdgeX, sEdgeY;
	INT32 iDiff, iCalc;
	INT32 sOriginalGridNo = sGridNo;
	if(gsReservedIndex >= gsReservedLen)// Check if everything is reserved.
		AssertMsg(0, "All closest map edgepoints have been reserved!!!");
	sGridNo = NOWHERE;
	ConvertGridNoToXY(sOriginalGridNo, &sOrigX, &sOrigY);
	iDiff = 0x0FFFFFFF;
	for(UINT16 i=0; i<usArraySize; i++)
	{
		BOOLEAN fReserved = FALSE;
		for(INT16 j=0; j<gsReservedIndex; j++)
		{
			if(gpReservedGridNos[j] == psArray[i])
			{
				fReserved = TRUE;
				break;
			}
		}
		if(fReserved)
			continue;
		ConvertGridNoToXY(psArray[i], &sEdgeX, &sEdgeY);
		iCalc = (sOrigX-sEdgeX)*(sOrigX-sEdgeX) + (sOrigY-sEdgeY)*(sOrigY-sEdgeY);
		if(iCalc <= iDiff)
		{
			iDiff = iCalc;
			sGridNo = psArray[i];
		}
	}
	// Is sGridNo in the edgepoint array	
	if(!TileIsOutOfBounds(sGridNo))
		gpReservedGridNos[gsReservedIndex++] = sGridNo;
	return(sGridNo);
}

INT32 SearchForClosestPrimaryMapEdgepoint(INT32 sGridNo, UINT8 ubInsertionCode, UINT8 defaultICode, UINT8* storedICode)
{
	INT32* psArray = NULL;
	UINT16 usArraySize = 0;
	if(ubInsertionCode == INSERTION_CODE_GRIDNO)
		*storedICode = ubInsertionCode = defaultICode;
	switch(ubInsertionCode)
	{
	case INSERTION_CODE_NORTH:
		psArray = gps1stNorthEdgepointArray;
		usArraySize = gus1stNorthEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any north mapedgepoints. LC:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_EAST:
		psArray = gps1stEastEdgepointArray;
		usArraySize = gus1stEastEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any east mapedgepoints. LC:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_SOUTH:
		psArray = gps1stSouthEdgepointArray;
		usArraySize = gus1stSouthEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any south mapedgepoints. LC:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_WEST:
		psArray = gps1stWestEdgepointArray;
		usArraySize = gus1stWestEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any west mapedgepoints. LC:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	}
	// WANNE - MP: Center
	if(is_networked && ubInsertionCode == INSERTION_CODE_CENTER)
	{
		InitCenterEdgepoint();
		psArray = gps1stCenterEdgepointArray;
		usArraySize = gus1stCenterEdgepointArraySize;
	}
	if(!usArraySize)
		return(NOWHERE);
	return(FindClosestMapEdgepoint(sGridNo, psArray, usArraySize));
}

INT32 SearchForClosestSecondaryMapEdgepoint(INT32 sGridNo, UINT8 ubInsertionCode)
{
	INT32* psArray = NULL;
	UINT16 usArraySize = 0;
	switch(ubInsertionCode)
	{
	case INSERTION_CODE_NORTH:
		psArray = gps2ndNorthEdgepointArray;
		usArraySize = gus2ndNorthEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any isolated north mapedgepoints. KM:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_EAST:
		psArray = gps2ndEastEdgepointArray;
		usArraySize = gus2ndEastEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any isolated east mapedgepoints. KM:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_SOUTH:
		psArray = gps2ndSouthEdgepointArray;
		usArraySize = gus2ndSouthEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any isolated south mapedgepoints. KM:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	case INSERTION_CODE_WEST:
		psArray = gps2ndWestEdgepointArray;
		usArraySize = gus2ndWestEdgepointArraySize;
		if(!usArraySize)
			AssertMsg(0, String("Sector %c%d level %d doesn't have any isolated west mapedgepoints. KM:1", gWorldSectorY+'A'-1, gWorldSectorX, gbWorldSectorZ));
		break;
	}
	if(!usArraySize)
		return(NOWHERE);
	return(FindClosestMapEdgepoint(sGridNo, psArray, usArraySize));
}
//dnl ch48 051009 Finish

#define EDGE_OF_MAP_SEARCH 5
BOOLEAN VerifyEdgepoint( SOLDIERTYPE * pSoldier, INT32 sEdgepoint )
{
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32 sGridNo;
	INT8		bDirection;

	pSoldier->sGridNo = sEdgepoint;

	iSearchRange = EDGE_OF_MAP_SEARCH;

	// determine maximum horizontal limits
	sMaxLeft	= min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp	= min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = EDGE_OF_MAP_SEARCH;

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = sEdgepoint + sXOffset + (MAXCOL * sYOffset);
			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	FindBestPath( pSoldier, GRIDSIZE, pSoldier->pathing.bLevel, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009

	// Turn off the "reachable" flag for the current location
	// so we don't consider it
	//gpWorldLevelData[sEdgepoint].uiFlags &= ~(MAPELEMENT_REACHABLE);

	// SET UP DOUBLE-LOOP TO STEP THROUGH POTENTIAL GRID #s
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			// calculate the next potential gridno
			sGridNo = sEdgepoint + sXOffset + (MAXCOL * sYOffset);

			if (!(gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_REACHABLE))
			{
				continue;
			}

			if (GridNoOnEdgeOfMap( sGridNo, &bDirection ) )
			{
				// ok!
				return TRUE;
			}
		}
	}

	// no spots right on edge of map within 5 tiles
	return FALSE;
}

BOOLEAN EdgepointsClose( SOLDIERTYPE *pSoldier, INT32 sEdgepoint1, INT32 sEdgepoint2 )
{
	INT32		iSearchRange;
	INT16		sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXOffset, sYOffset;
	INT32 sGridNo;

	pSoldier->sGridNo = sEdgepoint1;

	if( gWorldSectorX == 14 && gWorldSectorY == 9 && !gbWorldSectorZ )
	{ //BRUTAL CODE	-- special case map.
		iSearchRange = 250;
	}
	else
	{
		iSearchRange = EDGE_OF_MAP_SEARCH;
	}

	// determine maximum horizontal limits
	sMaxLeft	= min( iSearchRange, (pSoldier->sGridNo % MAXCOL));
	//NumMessage("sMaxLeft = ",sMaxLeft);
	sMaxRight = min( iSearchRange, MAXCOL - ((pSoldier->sGridNo % MAXCOL) + 1));
	//NumMessage("sMaxRight = ",sMaxRight);

	// determine maximum vertical limits
	sMaxUp	= min( iSearchRange, (pSoldier->sGridNo / MAXROW));
	//NumMessage("sMaxUp = ",sMaxUp);
	sMaxDown = min( iSearchRange, MAXROW - ((pSoldier->sGridNo / MAXROW) + 1));

	// Call FindBestPath to set flags in all locations that we can
	// walk into within range.	We have to set some things up first...

	// set the distance limit of the square region
	gubNPCDistLimit = (UINT8)iSearchRange;

	// reset the "reachable" flags in the region we're looking at
	for (sYOffset = -sMaxUp; sYOffset <= sMaxDown; sYOffset++)
	{
		for (sXOffset = -sMaxLeft; sXOffset <= sMaxRight; sXOffset++)
		{
			sGridNo = sEdgepoint1 + sXOffset + (MAXCOL * sYOffset);
			gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_REACHABLE);
		}
	}

	if( FindBestPath( pSoldier, sEdgepoint2, pSoldier->pathing.bLevel, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE ) )
	{
		return TRUE;
	}
	return FALSE;
}

UINT8 CalcMapEdgepointClassInsertionCode( INT32 sGridNo )
{
	SOLDIERTYPE Soldier;
	INT32			iLoop;
	INT32			*psEdgepointArray1, *psEdgepointArray2;
	INT32			iEdgepointArraySize1, iEdgepointArraySize2;
	INT32			sClosestSpot1 = NOWHERE, sClosestDist1 = 0x7FFFFFFF, sTempDist = NOWHERE;
	INT32			sClosestSpot2 = NOWHERE, sClosestDist2 = 0x7FFFFFFF;
	BOOLEAN		fPrimaryValid = FALSE, fSecondaryValid = FALSE;

	Soldier.bTeam = 1;
	Soldier.sGridNo = sGridNo;

	if( gMapInformation.sIsolatedGridNo == -1 )
	{ //If the map has no isolated area, then all edgepoints are primary.
		return INSERTION_CODE_PRIMARY_EDGEINDEX;
	}

	switch( gubTacticalDirection )
	{
		case NORTH:
			psEdgepointArray1 = gps1stNorthEdgepointArray;
			iEdgepointArraySize1 = gus1stNorthEdgepointArraySize;
			psEdgepointArray2 = gps2ndNorthEdgepointArray;
			iEdgepointArraySize2 = gus2ndNorthEdgepointArraySize;
			break;
		case EAST:
			psEdgepointArray1 = gps1stEastEdgepointArray;
			iEdgepointArraySize1 = gus1stEastEdgepointArraySize;
			psEdgepointArray2 = gps2ndEastEdgepointArray;
			iEdgepointArraySize2 = gus2ndEastEdgepointArraySize;
			break;
		case SOUTH:
			psEdgepointArray1 = gps1stSouthEdgepointArray;
			iEdgepointArraySize1 = gus1stSouthEdgepointArraySize;
			psEdgepointArray2 = gps2ndSouthEdgepointArray;
			iEdgepointArraySize2 = gus2ndSouthEdgepointArraySize;
			break;
		case WEST:
			psEdgepointArray1 = gps1stWestEdgepointArray;
			iEdgepointArraySize1 = gus1stWestEdgepointArraySize;
			psEdgepointArray2 = gps2ndWestEdgepointArray;
			iEdgepointArraySize2 = gus2ndWestEdgepointArraySize;
			break;
		default:
			// WTF???
			return INSERTION_CODE_PRIMARY_EDGEINDEX;
	}

	// Do a 2D search to find the closest map edgepoint and
	// try to create a path there
	for ( iLoop = 0; iLoop < iEdgepointArraySize1; iLoop++ )
	{
		sTempDist = PythSpacesAway( sGridNo, psEdgepointArray1[ iLoop ] );
		if ( sTempDist < sClosestDist1 )
		{
			sClosestDist1 = sTempDist;
			sClosestSpot1 = psEdgepointArray1[ iLoop ];
		}
	}
	for ( iLoop = 0; iLoop < iEdgepointArraySize2; iLoop++ )
	{
		sTempDist = PythSpacesAway( sGridNo, psEdgepointArray2[ iLoop ] );
		if ( sTempDist < sClosestDist2 )
		{
			sClosestDist2 = sTempDist;
			sClosestSpot2 = psEdgepointArray2[ iLoop ];
		}
	}

	// set the distance limit of the square region
	gubNPCDistLimit = 15;

	if( !sClosestDist1 || FindBestPath( &Soldier, sClosestSpot1, 0, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
	{
		fPrimaryValid = TRUE;
	}
	if( !sClosestDist2 || FindBestPath( &Soldier, sClosestSpot2, 0, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
	{
		fSecondaryValid = TRUE;
	}

	if( fPrimaryValid == fSecondaryValid )
	{
		if( sClosestDist2 < sClosestDist1 )
		{
			return INSERTION_CODE_SECONDARY_EDGEINDEX;
		}
		return INSERTION_CODE_PRIMARY_EDGEINDEX;
	}
	if( fPrimaryValid )
	{
		return INSERTION_CODE_PRIMARY_EDGEINDEX;
	}
	return INSERTION_CODE_SECONDARY_EDGEINDEX;
}

#ifdef JA2BETAVERSION
#include "worldman.h"
#include "message.h"
void ShowMapEdgepoints()
{
	INT32 i, usIllegal1 = 0, usIllegal2 = 0;
	for( i = 0 ; i < gus1stNorthEdgepointArraySize; i++ )
	{
		if( gps1stNorthEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps1stNorthEdgepointArray[ i ], FIRSTPOINTERS5 );
		}
		else
		{
			usIllegal1++;
		}
	}
	for( i = 0 ; i < gus1stEastEdgepointArraySize; i++ )
	{
		if( gps1stEastEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps1stEastEdgepointArray[ i ], FIRSTPOINTERS5 );
		}
		else
		{
			usIllegal1++;
		}
	}
	for( i = 0 ; i < gus1stSouthEdgepointArraySize; i++ )
	{
		if( gps1stSouthEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps1stSouthEdgepointArray[ i ], FIRSTPOINTERS5 );
		}
		else
		{
			usIllegal1++;
		}
	}
	for( i = 0 ; i < gus1stWestEdgepointArraySize; i++ )
	{
		if( gps1stWestEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps1stWestEdgepointArray[ i ], FIRSTPOINTERS5 );
		}
		else
		{
			usIllegal1++;
		}
	}

	for( i = 0 ; i < gus2ndNorthEdgepointArraySize; i++ )
	{
		if( gps2ndNorthEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps2ndNorthEdgepointArray[ i ], FIRSTPOINTERS6 );
		}
		else
		{
			usIllegal2++;
		}
	}
	for( i = 0 ; i < gus2ndEastEdgepointArraySize; i++ )
	{
		if( gps2ndEastEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps2ndEastEdgepointArray[ i ], FIRSTPOINTERS6 );
		}
		else
		{
			usIllegal2++;
		}
	}
	for( i = 0 ; i < gus2ndSouthEdgepointArraySize; i++ )
	{
		if( gps2ndSouthEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps2ndSouthEdgepointArray[ i ], FIRSTPOINTERS6 );
		}
		else
		{
			usIllegal2++;
		}
	}
	for( i = 0 ; i < gus2ndWestEdgepointArraySize; i++ )
	{
		if( gps2ndWestEdgepointArray[ i ] != -1 )
		{
			AddTopmostToTail( gps2ndWestEdgepointArray[ i ], FIRSTPOINTERS6 );
		}
		else
		{
			usIllegal2++;
		}
	}
	if( !usIllegal1 && !usIllegal2 )
	{
		ScreenMsg( 0, MSG_TESTVERSION, L"Showing display of map edgepoints" );
	}
	else
	{
		ScreenMsg( 0, MSG_TESTVERSION, L"Showing display of map edgepoints (%d illegal primary, %d illegal secondary)" );
	}
	ScreenMsg( 0, MSG_TESTVERSION, L"N:%d:%d E:%d:%d S:%d:%d W:%d:%d",
		gus1stNorthEdgepointArraySize, gus2ndNorthEdgepointArraySize,
		gus1stEastEdgepointArraySize,	gus2ndEastEdgepointArraySize,
		gus1stSouthEdgepointArraySize, gus2ndSouthEdgepointArraySize,
		gus1stWestEdgepointArraySize,	gus2ndWestEdgepointArraySize );
}

void HideMapEdgepoints()
{
	INT32 i;
	ScreenMsg( 0, MSG_TESTVERSION, L"Removing display of map edgepoints" );
	for( i = 0 ; i < gus1stNorthEdgepointArraySize; i++ )
	{
		if( gps1stNorthEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps1stNorthEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus1stEastEdgepointArraySize; i++ )
	{
		if( gps1stEastEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps1stEastEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus1stSouthEdgepointArraySize; i++ )
	{
		if( gps1stSouthEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps1stSouthEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus1stWestEdgepointArraySize; i++ )
	{
		if( gps1stWestEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps1stWestEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}

	for( i = 0 ; i < gus2ndNorthEdgepointArraySize; i++ )
	{
		if( gps2ndNorthEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps2ndNorthEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus2ndEastEdgepointArraySize; i++ )
	{
		if( gps2ndEastEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps2ndEastEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus2ndSouthEdgepointArraySize; i++ )
	{
		if( gps2ndSouthEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps2ndSouthEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
	for( i = 0 ; i < gus2ndWestEdgepointArraySize; i++ )
	{
		if( gps2ndWestEdgepointArray[ i ] != -1 )
		{
			RemoveAllTopmostsOfTypeRange( gps2ndWestEdgepointArray[ i ], FIRSTPOINTERS, FIRSTPOINTERS );
		}
	}
}
#endif
