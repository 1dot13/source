/*
	Filename		:		pathai.c
	Author			:		Ray E. Bornert II
	Date			:		1992-MAR-15

	Skip list additions
	Author			:		Chris Camfield
	Date			:		1997-NOV
*/

#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "MemMan.h"
	#include "Overhead Types.h"

	#include "Animation Cache.h"
	#include "Animation Data.h"
	#include "Animation Control.h"
	#include "container.h"
	#include "interface.h"
	#include <math.h>

	#include "input.h"
	#include "english.h"
	#include "worlddef.h"
	#include "worldman.h"
//	#include "renderworld.h"
	#include "pathai.h"
	#include "Points.h"
	#include "ai.h"
	#include "Random.h"
	#include "message.h"
	#include "structure wrap.h"
	#include "keys.h"
	#include "gamesettings.h"
	#include "Buildings.h"
	#include "soldier profile.h" // added by SANDRO
	#include "Soldier macros.h"
	#include "AIinternals.h"
	#include "Rotting Corpses.h"
	#include "Meanwhile.h"
#endif
#include "connect.h"

#include "LOS.h"  //ddd

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

#ifdef USE_ASTAR_PATHS
#include "BinaryHeap.hpp"
#include "AIInternals.h"
#include "opplist.h"
#include "weapons.h"
#endif
//#include "dnlprocesstalk.h"//dnl???

extern UINT16 gubAnimSurfaceIndex[ TOTALBODYTYPES ][ NUMANIMATIONSTATES ];

// sevenfm:
extern BOOLEAN InGas( SOLDIERTYPE *pSoldier, INT32 sGridNo );
extern BOOLEAN InGasSpot(SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel);

//extern UINT8 gubDiagCost[20];
// skiplist has extra level of pointers every 4 elements, so a level 5is optimized for
// 4 to the power of 5 elements, or 2 to the power of 10, 1024

//#define PATHAI_VISIBLE_DEBUG

//#define PATHAI_SKIPLIST_DEBUG

#ifdef PATHAI_VISIBLE_DEBUG
	#include "video.h"

//extern INT16 gsCoverValue[WORLD_MAX];
extern INT16 * gsCoverValue;
	BOOLEAN gfDisplayCoverValues = TRUE;
	BOOLEAN gfDrawPathPoints = TRUE;
#endif

BOOLEAN gfPlotPathToExitGrid = FALSE;
BOOLEAN gfRecalculatingExistingPathCost = FALSE;
UINT8 gubGlobalPathFlags = 0;
UINT8 gubBuildingInfoToSet;
INT32 iMaxSkipListLevel = MAX_SKIPLIST_LEVEL;
INT32 iMaxTrailTree = MAX_TRAIL_TREE;
INT32 iMaxPathQ = MAX_PATHQ;
UINT8 gfPlotPathEndDirection;

extern BOOLEAN gfGeneratingMapEdgepoints;

#define VEHICLE

//#define TRAILCELLTYPE UINT16 //Lalien: moved to pathai.h

// OLD PATHAI STUFF
/////////////////////////////////////////////////
//dnl ch50 071009
struct path_s
{
	INT32					iLocation;
	INT32					sPathNdx;
	struct path_s*			pNext[ABSMAX_SKIPLIST_LEVEL];
	TRAILCELLTYPE			usCostSoFar;
	TRAILCELLTYPE			usCostToGo;
	TRAILCELLTYPE			usTotalCost;
	INT16					ubTotalAPCost;
	INT8					bLevel;
	UINT8					ubLegDistance;
};

typedef struct path_s path_t;

struct trail_s
{
	INT32 nextLink;
	INT8	stepDir;
	INT8	fFlags;
	INT32 sGridNo;
};
typedef struct trail_s trail_t;

enum
{
	STEP_BACKWARDS = 0x01,
} TrailFlags;

#define EASYWATERCOST	TRAVELCOST_FLAT / 2
#define ISWATER(t)	(((t)==TRAVELCOST_KNEEDEEP) || ((t)==TRAVELCOST_DEEPWATER))
#define NOPASS (TRAVELCOST_BLOCKED)
//#define VEINCOST TRAVELCOST_FLAT	 //actual cost for bridges and doors and such
//#define ISVEIN(v) ((v==TRAVELCOST_VEINMID) || (v==TRAVELCOST_VEINEND))

static path_t * pathQ;
static UINT16	gusPathShown,gusAPtsToMove;
static INT32	queRequests;
static INT32	iSkipListSize;
static INT32	iClosedListSize;
static INT8		bSkipListLevel;
static INT32	iSkipListLevelLimit[ABSMAX_SKIPLIST_LEVEL] = { 0, 4, 16, 64, 256, 1024, 4192, 16384, 65536, 262144 };//dnl ch50 071009

#define ESTIMATE0	((dx>dy) ?		(dx)		:		(dy))
#define ESTIMATE1	((dx<dy) ? ((dx*14)/10+dy) : ((dy*14)/10+dx) )
#define ESTIMATE2	FLATCOST*( (dx<dy) ? ((dx*14)/10+dy) : ((dy*14)/10+dx) )
#define ESTIMATEn	((int)(FLATCOST*sqrt(dx*dx+dy*dy)))
#define ESTIMATEC ( ( (dx<dy) ? (TRAVELCOST_BUMPY * (dx * 14 + dy * 10) / 10) : ( TRAVELCOST_BUMPY * (dy * 14 + dx * 10) / 10 ) ) )
//#define ESTIMATEC (((dx<dy) ? ( (TRAVELCOST_FLAT * dx * 14) / 10 + dy) : (TRAVELCOST_FLAT * dy * 14 ) / 10 + dx) ) )
#define ESTIMATE ESTIMATEC

#define MAXCOST (16384)//(9990)
//#define MAXCOST (255)
//#define TOTALCOST( pCurrPtr ) (pCurrPtr->usCostSoFar + pCurrPtr->usCostToGo)
#define TOTALCOST( ptr ) (ptr->usTotalCost)
#define XLOC(a) (a%WORLD_COLS)
#define YLOC(a) (a/WORLD_COLS)
//#define LEGDISTANCE(a,b) ( abs( XLOC(b)-XLOC(a) ) + abs( YLOC(b)-YLOC(a) ) )
#define LEGDISTANCE( x1, y1, x2, y2 ) ( abs( x2 - x1 ) + abs( y2 - y1 ) )
//#define FARTHER(ndx,NDX) ( LEGDISTANCE( ndx->sLocation,sDestination) > LEGDISTANCE(NDX->sLocation,sDestination) )
#define FARTHER(ndx,NDX) ( ndx->ubLegDistance > NDX->ubLegDistance )

#define SETLOC( str, loc ) \
{\
	(str).iLocation = loc;\
}

static TRAILCELLTYPE * trailCost;
static UINT8 * trailCostUsed;
static UINT8 gubGlobalPathCount = 0;
static trail_t * trailTree;

static INT32 trailTreeNdx = 0;//dnl ch50 071009

#define QHEADNDX (0)
#define QPOOLNDX (iMaxPathQ-1)

static path_t * pQueueHead;
static path_t * pClosedHead;


#define pathQNotEmpty (pQueueHead->pNext[0] != NULL)
#define pathFound (pQueueHead->pNext[0]->iLocation == iDestination)
#define pathNotYetFound (!pathFound)

// Note, the closed list is maintained as a doubly-linked list;
// it's a regular queue, essentially, as we always add to the end
// and remove from the front

// pNext[0] is used for the next pointers
// and pNext[1] is used for prev pointers

/*
#define ClosedListAdd( pNew ) \
{\
	pNew->pNext[0] = pClosedHead;\
	pNew->pNext[1] = pClosedHead->pNext[1];\
	pClosedHead->pNext[1]->pNext[0] = pNew;\
	pClosedHead->pNext[1] = pNew;\
	pNew->iLocation = -1;\
	iClosedListSize++;\
}

#define ClosedListGet( pNew )\
{\
	if (queRequests<QPOOLNDX)\
	{\
		pNew = pathQ + (queRequests);\
		queRequests++;\
		memset( pNew->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else if (iClosedListSize > 0)\
	{\
		pNew = pClosedHead->pNext[0];\
		pNew->pNext[1]->pNext[0] = pNew->pNext[0];\
		pNew->pNext[0]->pNext[1] = pNew->pNext[1];\
		iClosedListSize--;\
		queRequests++;\
		memset( pNew->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else\
	{\
		pNew = NULL;\
	}\
}
*/

// experiment 1, seemed to fail
#define ClosedListAdd( pNew ) \
{\
	pNew->pNext[0] = pClosedHead->pNext[0];\
	pClosedHead->pNext[0] = pNew;\
	pNew->iLocation = -1;\
	iClosedListSize++;\
}

#define ClosedListGet( pNew )\
{\
	if (queRequests<QPOOLNDX)\
	{\
		pNew = pathQ + (queRequests);\
		queRequests++;\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else if (iClosedListSize > 0)\
	{\
		pNew = pClosedHead->pNext[0];\
		pClosedHead->pNext[0] = pNew->pNext[0];\
		iClosedListSize--;\
		queRequests++;\
		memset( pNew->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else\
	{\
		pNew = NULL;\
	}\
}

/*
#define ClosedListAdd( pNew ) \
{\
	pNew->pNext[0] = pClosedHead;\
	pNew->pNext[1] = pClosedHead->pNext[1];\
	pClosedHead->pNext[1]->pNext[0] = pNew;\
	pClosedHead->pNext[1] = pNew;\
	pNew->iLocation = -1;\
	iClosedListSize++;\
}

#define ClosedListGet( pNew )\
{\
	if (queRequests<QPOOLNDX)\
	{\
		pNew = pathQ + (queRequests);\
		queRequests++;\
		memset( pNew->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else if (iClosedListSize > 0)\
	{\
		pNew = pClosedHead->pNext[0];\
		pNew->pNext[1]->pNext[0] = pNew->pNext[0];\
		pNew->pNext[0]->pNext[1] = pNew->pNext[1];\
		iClosedListSize--;\
		queRequests++;\
		memset( pNew->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );\
		pNew->pathing.bLevel = RandomSkipListLevel();\
	}\
	else\
	{\
		pNew = NULL;\
	}\
}
*/

#define SkipListRemoveHead()\
{\
	pDel = pQueueHead->pNext[0];\
	for (iLoop = 0; iLoop < __min( bSkipListLevel, pDel->bLevel ); iLoop++)\
	{\
		pQueueHead->pNext[iLoop] = pDel->pNext[iLoop];\
	}\
	iSkipListSize--;\
	ClosedListAdd( pDel );\
}

#define SkipListInsert( pNew )\
{\
	pCurr = pQueueHead;\
	uiCost = TOTALCOST( pNew );\
	memset( pUpdate, 0, MAX_SKIPLIST_LEVEL * sizeof( path_t *) );\
	for (iCurrLevel = bSkipListLevel - 1; iCurrLevel >= 0; iCurrLevel--)\
	{\
		pNext = pCurr->pNext[iCurrLevel];\
		while (pNext)\
		{\
			if ( uiCost > TOTALCOST( pNext ) || (uiCost == TOTALCOST( pNext ) && FARTHER( pNew, pNext ) ) )\
			{\
				pCurr = pNext;\
				pNext = pCurr->pNext[iCurrLevel];\
			}\
			else\
			{\
				break;\
			}\
		}\
		pUpdate[iCurrLevel] = pCurr;\
	}\
	pCurr = pCurr->pNext[0];\
	for (iCurrLevel = 0; iCurrLevel < pNew->pathing.bLevel; iCurrLevel++)\
	{\
		if (!(pUpdate[iCurrLevel]))\
		{\
			break;\
		}\
		pNew->pNext[iCurrLevel] = pUpdate[iCurrLevel]->pNext[iCurrLevel];\
		pUpdate[iCurrLevel]->pNext[iCurrLevel] = pNew;\
	}\
	iSkipListSize++;\
	if (iSkipListSize > iSkipListLevelLimit[bSkipListLevel])\
	{\
		pCurr = pQueueHead;\
		pNext = pQueueHead->pNext[bSkipListLevel - 1];\
		while( pNext )\
		{\
			if (pNext->pathing.bLevel > bSkipListLevel)\
			{\
				pCurr->pNext[bSkipListLevel] = pNext;\
				pCurr = pNext;\
			}\
			pNext = pNext->pNext[bSkipListLevel - 1];\
		}\
		pCurr->pNext[bSkipListLevel] = pNext;\
		bSkipListLevel++;\
	}\
}

#define REMQUEHEADNODE()							SkipListRemoveHead();

#define DELQUENODE(ndx) SkipListRemoveHead()

#define REMAININGCOST(ptr)					\
(								\
	(dy = abs(iDestY-iLocY)),					\
	(dx = abs(iDestX-iLocX)),					\
	ESTIMATE						\
)
/*
#define REMAININGCOST(ptr)					\
(								\
	(locY = (ptr)->iLocation/MAPWIDTH),			\
	(locX = (ptr)->iLocation%MAPWIDTH),			\
	(dy = abs(iDestY-locY)),					\
	(dx = abs(iDestX-locX)),					\
	ESTIMATE						\
)
*/

#define NEWQUENODE ClosedListGet( pNewPtr )

#define QUEINSERT(ndx) SkipListInsert( ndx )

#define GREENSTEPSTART		0
#define REDSTEPSTART			16
#define PURPLESTEPSTART		32
#define BLUESTEPSTART			48
#define ORANGESTEPSTART		64

INT16 gubNPCAPBudget = 0;
UINT16 gusNPCMovementMode;
UINT8 gubNPCDistLimit = 0;
BOOLEAN gfNPCCircularDistLimit = FALSE;
UINT8	gubNPCPathCount;

BOOLEAN gfPlotDirectPath = FALSE;
BOOLEAN gfEstimatePath = FALSE;
BOOLEAN	gfPathAroundObstacles = TRUE;

// Lesh: fix CTD in Tixa
// The game was trying to store very long path (270 cells) in 256-cell array and corrupted
// some data, not belonged to array. It was ordinary array boundary break.
UINT32* guiPlottedPath = NULL;
UINT32* guiPathingData = NULL;
static INT32 giPathDataSize;
static INT32 giPlotCnt;
static INT32 guiEndPlotGridNo;

//static INT32 dirDelta[8]=
//{
//	-MAPWIDTH,        //N
//	1-MAPWIDTH,       //NE
//	1,                //E
//	1+MAPWIDTH,       //SE
//	MAPWIDTH,         //S
//	MAPWIDTH-1,       //SW
//	-1,               //W
//	-MAPWIDTH-1       //NW
//};

///*static */INT32 dirDelta[8]=
//{
//		-OLD_WORLD_COLS,        //N
//		1-OLD_WORLD_COLS,       //NE
//		1,                //E
//		1+OLD_WORLD_COLS,       //SE
//		OLD_WORLD_COLS,         //S
//		OLD_WORLD_COLS-1,       //SW
//		-1,               //W
//		-OLD_WORLD_COLS-1       //NW
//};
INT32 dirDelta[8]=
{
		-WORLD_COLS,        //N
		1-WORLD_COLS,       //NE
		1,					//E
		1+WORLD_COLS,       //SE
		WORLD_COLS,         //S
		WORLD_COLS-1,       //SW
		-1,					//W
		-WORLD_COLS-1       //NW
};

#define LOOPING_CLOCKWISE 0
#define LOOPING_COUNTERCLOCKWISE 1
#define LOOPING_REVERSE 2

#ifdef COUNT_PATHS
UINT32 guiSuccessfulPathChecks = 0;
UINT32 guiTotalPathChecks = 0;
UINT32 guiFailedPathChecks = 0;
UINT32 guiUnsuccessfulPathChecks = 0;
#endif

#ifdef USE_ASTAR_PATHS


//ADB the extra cover feature is supposed to pick a path of the same distance as one calculated with the feature off,
//but a safer path, usually farther away from an enemy or following behind some cover.
//however it has not been tested and it may need some work, I haven't touched it in a while
//#define ASTAR_USING_EXTRACOVER

using namespace std;
using namespace ASTAR;

AStarPathfinder::AStarPathfinder()
{
	return;
}

//init the pointer to the AStarPathfinder singleton instance
// And init a static class so that it will destroy itself on game exit
static AStarPathfinder AStarThis;
AStarPathfinder* AStarPathfinder::pThis = &AStarThis;

AStarPathfinder& AStarPathfinder::GetInstance()
{
	//if (pThis)
	//{
	//	return *pThis;
	//}
	//pThis = new AStarPathfinder;
	return *pThis;
}

void AStarPathfinder::ResetAStarList()
{
	//only the starting node's data needs to be init,
	//and all other data doesn't need to be at all
	//for ( GridNode_t::iterator node = ClosedList.begin();
	//	node != ClosedList.end(); 
	//	node++) 
	//{
	//	SetAStarStatus(*node, AStar_Init);
	//	SetAStarG(*node, 0);
	//}
	//for (int size = OpenHeap.size(); size > 0; --size) 
	//{
	//	GridNode node = OpenHeap.peekElement(size).data;
	//	SetAStarStatus(node, AStar_Init);
	//	SetAStarG(node, 0);
	//}
	//comm by ddd{
	//for (INT16 node = 0; node < WORLD_MAX; node++)
	//{
	//	SetAStarStatus(node, AStar_Init); //comm by ddd
	//	SetAStarG(node, 0); //comm by ddd
	//}
	//comm by ddd}

	//ddd{

	INT32 node;
	for (node = 0; node < (WORLD_MAX & ~3); node+=4) //ddd loop unwinding
	{
		AStarData[node].status = AStarData[node+1].status
			= AStarData[node+2].status = AStarData[node+3].status = AStar_Init; 

		AStarData[node+3].cost = AStarData[node+2].cost =
		 AStarData[node+1].cost = AStarData[node].cost = 0;
	}
	for(node=(WORLD_MAX & ~3); node < WORLD_MAX; node++)
	{AStarData[node].status = AStar_Init;  AStarData[node].cost = 0;}
		////ddd}


	//ClosedList.clear();
	OpenHeap.clear();
	return;
}//end ResetAStarList

void AStarPathfinder::SetLoopState(const INT32 node,
								int const loopState)
{
	if ( loopState == LOOPING_REVERSE ) 
	{
		AStarData[node].wasBackwards = STEP_BACKWARDS;
	}
	else 
	{
		AStarData[node].wasBackwards = false;
	}
}

int AStarPathfinder::PythSpacesAway(const INT32 node1,
								    const INT32 node2)
{
	int sRows,sCols,sResult;
	GridNode *n1, *n2;

	n1 = GridNode::MapXY + node1;
	n2 = GridNode::MapXY + node2;

	sRows = n1->x - n2->x;
	sCols = n1->y - n2->y;
 
	// apply Pythagoras's theorem for right-handed triangle:
	// Except skip the square root.  No reason to care about actual distance since we only need it for comparison.
	// And conversion to double, then sqrt, then conversion back to int, is expensive.
	sResult = (sRows * sRows) + (sCols * sCols);

	return(sResult);
}

INT16 AStarPathfinder::SpacesAway(const INT32 node1,
								  const INT32 node2)
{
	//should be faster than its counterpart
	int sRows,sCols;
	GridNode *n1, *n2;

	n1 = GridNode::MapXY + node1;
	n2 = GridNode::MapXY + node2;

	sRows = abs(n1->x - n2->x);
	sCols = abs(n1->y - n2->y);
 
	return (INT16)(__max(sRows, sCols));
}

int AStarPathfinder::GetPath(SOLDIERTYPE *s ,
							INT32 dest,
							INT8 ubLevel,
							INT16 usMovementMode,
							INT8 bCopy,
							UINT8 fFlags )
{
	//copy over private data that makes code more readable
	this->pSoldier = s;
	this->mercsMaxAPs = this->pSoldier->bActionPoints;
	this->onRooftop = ubLevel;
	this->movementMode = usMovementMode;
	this->maxAPBudget = gubNPCAPBudget;
	// Set the distance for this calc
	this->gubNPCDistLimitSq = (int) gubNPCDistLimit * (int) gubNPCDistLimit;

	StartNode = pSoldier->sGridNo;
	DestNode = dest;
	fFlags |= gubGlobalPathFlags;	

	if (StartNode < 0 || StartNode > WORLD_MAX) 
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!  Trying to calculate path from off-world gridno %d to %d", StartNode, DestNode );
		#endif
		return( 0 );
	}
	else if (!GridNoOnVisibleWorldTile( StartNode ) ) 
	{
		#ifdef JA2BETAVERSION
			if(!is_networked)ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!  Trying to calculate path from non-visible gridno %d to %d", StartNode, DestNode );
		#endif
		return( 0 );
	}
	else if (pSoldier->pathing.bLevel != onRooftop) 
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, different level" ) );
		// pathing to a different level... bzzzt!
		return( 0 );
	}



#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		memset( gsCoverValue, 0x7F, sizeof( INT16 ) * WORLD_MAX );
	}
	gsCoverValue[ StartNode ] = 0;
	PATHAI_VISIBLE_DEBUG_Counter = 1;
#endif

	//init other private data, mostly flags
	endDir = lastDir = direction = startDir = 0;
	pStructureFileRef = NULL;
	bLoopState = LOOPING_CLOCKWISE;
	fCheckedBehind = FALSE;
	fGoingThroughDoor = FALSE;

	fTurnBased = ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) );
	fPathingForPlayer = ( (pSoldier->bTeam == gbPlayerNum) && (!gTacticalStatus.fAutoBandageMode) && !(pSoldier->flags.uiStatusFlags & SOLDIER_PCUNDERAICONTROL) );
	fNonFenceJumper = !( IS_MERC_BODY_TYPE( pSoldier ) );
	fNonSwimmer = !( IS_MERC_BODY_TYPE( pSoldier ) );
	fPathAroundPeople = ( (fFlags & PATH_THROUGH_PEOPLE) == 0 );
	fCloseGoodEnough = ( (fFlags & PATH_CLOSE_GOOD_ENOUGH) != 0);
	fConsiderPersonAtDestAsObstacle = (BOOLEAN)( fPathingForPlayer && fPathAroundPeople && !(fFlags & PATH_IGNORE_PERSON_AT_DEST) );

	if ( fNonSwimmer && Water( dest ) ) 
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, water" ) );
		return( 0 );
	}
	if ( fCloseGoodEnough ) 
	{
		sClosePathLimitSq = __min( PythSpacesAway( StartNode, DestNode ) - 1,  PATH_CLOSE_RADIUS_SQ );
		if ( sClosePathLimitSq <= 0 ) 
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, path limit" ) );
			return( 0 );
		}
	}

	if (bCopy >= COPYREACHABLE) 
	{
		fCopyReachable = TRUE;
		fCopyPathCosts = (bCopy == COPYREACHABLE_AND_APS);
		fVisitSpotsOnlyOnce = (bCopy == COPYREACHABLE || bCopy == FINDCLIMBPOINTS);
		fFindClimbPoints = (bCopy == FINDCLIMBPOINTS);
		// make sure we aren't trying to copy path costs for an area greater than the AI array...
		if (fCopyPathCosts && gubNPCDistLimitSq > AI_PATHCOST_RADIUS*AI_PATHCOST_RADIUS) 
		{
			// oy!!!! dis no supposed to happen!
			gubNPCDistLimitSq = AI_PATHCOST_RADIUS*AI_PATHCOST_RADIUS;
			gubNPCDistLimit = AI_PATHCOST_RADIUS;
		}
	}
	else 
	{
		fCopyReachable = FALSE;
		fCopyPathCosts = FALSE;
		fVisitSpotsOnlyOnce = FALSE;
	}

	gubNPCPathCount++;
	
	if (!TileIsOutOfBounds(DestNode))
	{
		if (DestNode == pSoldier->sGridNo) 
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, dest is start" ) );
			return( 0 );
		}

		// the very first thing to do is make sure the destination tile is reachable
		if (!NewOKDestination( pSoldier, dest, fConsiderPersonAtDestAsObstacle, onRooftop )) 
		{
			maxAPBudget = 0;
			gubNPCDistLimitSq = 0;
			DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, not ok dest" ) );
			return( 0 );
		}
	}


#ifdef COUNT_PATHS
	guiTotalPathChecks++;
#endif

#ifdef VEHICLE

	fMultiTile = ((pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE) != 0);
	if ( fMultiTile == false)
	{
		fContinuousTurnNeeded = FALSE;
	}
	else
	{
		// Get animation surface...
		// Chris_C... change this to use parameter.....
		UINT16 usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, movementMode );
		// Get structure ref...
		pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, movementMode );

		if ( pStructureFileRef )
		{
			fContinuousTurnNeeded = ( ( pSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE) ) != 0 );

			/*
			//bool fVehicle = ( (pSoldier->uiStatusFlags & SOLDIER_VEHICLE) != 0 );
			if (fVehicle && pSoldier->bReverse)
			{
				fReverse = TRUE;
			}
			if (fVehicle || pSoldier->ubBodyType == COW || pSoldier->ubBodyType == BLOODCAT) // or a vehicle
			{
				fTurnSlow = TRUE;
			}
			*/
			if ( gfEstimatePath )
			{
				bOKToAddStructID = IGNORE_PEOPLE_STRUCTURE_ID;
			}
			else if ( pSoldier->pLevelNode != NULL && pSoldier->pLevelNode->pStructureData != NULL )
			{
				bOKToAddStructID = pSoldier->pLevelNode->pStructureData->usStructureID;
			}
			else
			{
				bOKToAddStructID = INVALID_STRUCTURE_ID;
			}
		}
		else
		{
			// turn off multitile pathing
			fMultiTile = FALSE;
			fContinuousTurnNeeded = FALSE;
		}
	}
#endif

	if (fContinuousTurnNeeded == false)
	{
		bLoopState = LOOPING_CLOCKWISE;
	}

	// if origin and dest is water, then user wants to stay in water!
	// so, check and set waterToWater flag accordingly	
	if (TileIsOutOfBounds(dest))
	{
		bWaterToWater = false;
	}
	else
	{
		if (ISWATER(gubWorldMovementCosts[StartNode][0][onRooftop]) && ISWATER(gubWorldMovementCosts[DestNode][0][onRooftop])) 
		{
			bWaterToWater = true;
		}
		else
		{
			bWaterToWater = false;
		}
	}

	//make sure all data is init ok
	ResetAStarList();

	//Start the main loop and get a path
	int bestPath = AStar();

	gubNPCAPBudget = 0;
	gubNPCDistLimitSq = 0;
	if (bestPath == -222) 
	{
		//error, path not found
		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path failed, path not found" ) );
		return 0;
	}

#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		SetRenderFlags( RENDER_FLAG_FULL );
// The RenderCoverDebugInfo call is now made by RenderWorld.  So don't try to call it here
#if 0
		if ( guiCurrentScreen == GAME_SCREEN ) 
		{
			RenderWorld();
			RenderCoverDebug( );
			InvalidateScreen( );
			EndFrameBufferRender();
			RefreshScreen( NULL );
		}
#endif
	}
#endif

	// Count the number of steps, but keep it less than the max path length.
	// Adjust the parent until it begins at the tail end of the max path length (or the dest if reachable)
	INT32 parent = DestNode;
	INT32 current = parent;
	unsigned int sizePath = 0;
	//CHRISL: If, for whatever reason, the pathfinding system gets the wrong Grid values, it's possible that an infinite
	//	loop can result.  This can often require a hard reboot to get around.  To try and force a CTD in this event, lets
	//	track all the grids and force a crash if we ever pull the same GridNo.
	std::vector<INT32>	trackNode;
	trackNode.resize(1,current);
	while (current != -1)
	{
		//sizePath += GetNumSteps( current); //comm by ddd
		sizePath +=AStarData[current].numSteps;
		while (sizePath > MAX_PATH_LIST_SIZE)
		{
			//sizePath -= GetNumSteps( parent);//comm by ddd
			sizePath -=AStarData[parent].numSteps;
			//parent = GetAStarParent( parent);//comm by ddd
			parent = AStarData[parent].parent;
		}
		//current = GetAStarParent( current);
		current = AStarData[current].parent ;
		for(unsigned int i = 0; i<trackNode.size(); i++)
		{
			if(trackNode.at(i) == current)
			{
				//CHAR8		str[128];
				//sprintf( str, "Duplicate GridNo found during pathfinding" );
				//AssertMsg( 0, str );
				//AssertMsg( FALSE, String( "Duplicate GridNo found during pathfinding at %d.  Related to soldier %d at GridNo %d.", current, s->ubID, s->sGridNo ) );
				
				// lalien: hack to avoid freezing, return 0 if the path is bad.
				// Probably it will make trouble, need to check this
				return 0;
			}
		}
		trackNode.push_back(current);
	}

	// if this function was called because a solider is about to embark on an actual route
	// (as opposed to "test" path finding (used by cursor, etc), then grab all pertinent
	// data and copy into soldier's database
	if (bCopy == COPYROUTE) 
	{
		pSoldier->pathing.usPathDataSize  = (UINT16) sizePath;
		pSoldier->pathing.usPathIndex = 0;
		while (sizePath > 0)
		{
			int numSteps = GetNumSteps(parent);
			while (numSteps > 0)
			{
				sizePath--;
				numSteps--;
				pSoldier->pathing.usPathingData[sizePath] = GetDirection(parent);
			}
			parent = GetAStarParent( parent);
		}
		// Since the sizePath began as the total path size, it should be 0 when we finish copying it
		Assert( sizePath == 0);
		sizePath = pSoldier->pathing.usPathDataSize;
	}
	else if (bCopy == NO_COPYROUTE) 
	{
		giPathDataSize = (UINT16) sizePath;
		while (sizePath > 0)
		{
			int numSteps = GetNumSteps(parent);
			while (numSteps > 0)
			{
				sizePath--;
				numSteps--;
				guiPathingData[sizePath] = GetDirection(parent);
			}
			parent = GetAStarParent( parent);
		}
		// Since the sizePath began as the total path size, it should be 0 when we finish copying it
		Assert( sizePath == 0);
		sizePath = giPathDataSize;
	}

#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		SetRenderFlags( RENDER_FLAG_FULL );
#if 0
		RenderWorld();
		RenderCoverDebug( );
		InvalidateScreen( );
		EndFrameBufferRender();
		RefreshScreen( NULL );
#endif
	}
#endif

	#ifdef COUNT_PATHS
		guiSuccessfulPathChecks++;
	#endif

	//TEMP:	This is returning zero when I am generating edgepoints, so I am force returning 1 until
	//		this is fixed?
	if( gfGeneratingMapEdgepoints )
	{
		return 1;
	}


	DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR: path ok, path size %d", sizePath ) );
	// return path length : serves as a "successful" flag and a path length counter
	return sizePath;
}//end GetPath
#pragma optimize("gpt",on)
//__forceinline
INT16 AStarPathfinder::AStar()
{
	HEAP<int, INT32> TopHeap(StartNode, 0);
	ParentNode = StartNode;

	this->travelcostOrth = CalcAP( TRAVELCOST_FLAT, 0);
	this->travelcostDiag = CalcAP( TRAVELCOST_FLAT, 1);

	// Since the startup cost is always charged for the first node, why not make it the source node's cost?
	//init the starting node's data that hasn't been reset
	SetAStarParent(StartNode, -1);
	SetNumSteps(StartNode, 0);
	SetAStarG( StartNode, CalcStartingAP());

	for (;;) 
	{
		if ( fCloseGoodEnough ) 
		{
			if ( PythSpacesAway( CurrentNode, DestNode ) <= sClosePathLimitSq ) 
			{
				// stop the path here!
				DestNode = CurrentNode;
				fCloseGoodEnough = FALSE;
			}
		}

		if (ParentNode == DestNode) 
		{
			//the best path to date leads to the dest
			//this node is neither in the open heap or the closed list, make sure it gets reset ok.
			//ClosedList.push_back(ParentNode);
			//We just clear the entire array before starting, so no prob.
			break;
		}

		//this adds to the heap nodes that might be on a path
		ExecuteAStarLogic();

		if (OpenHeap.size() == 0)
		{
			return -222;//error code meaning no path at all!
		}

		//get the best point so far from the heap
		TopHeap = OpenHeap.popTopHeap();
		ParentNode = TopHeap.data;
		//ASSERT(GetAStarStatus(ParentNode) == AStar_Open);
	}
	return GetAStarG( TopHeap.data);
}//end AStar

void AStarPathfinder::IncrementLoop()
{
	startingLoop = false;
	if ( fContinuousTurnNeeded && direction == gOppositeDirection[ startDir ] )
	{
		fCheckedBehind = TRUE;
	}
	if (bLoopState == LOOPING_CLOCKWISE)
	{// backwards
		direction = gOneCCDirection[ direction ];
	}
	else
	{
		direction = gOneCDirection[ direction ];
	}
	return;
}

void AStarPathfinder::InitLoop()
{
	startingLoop = true;
	direction = startDir;
	return;
}

bool AStarPathfinder::ContinueLoop()
{
	if (direction == endDir && startingLoop == false)
	{
		return false;
	}
	return true;
}

void AStarPathfinder::ExecuteAStarLogic()
{
	//parent node set in AStar
	//this node is now closed
	SetAStarStatus(ParentNode, AStar_Closed);
	//ClosedList.push_back(ParentNode);

#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		if (gsCoverValue[ ParentNode ] > 0)
		{
			gsCoverValue[ ParentNode ] *= -1;
		}
	}
#endif

	// Shouldn't G and AP be the same thing?
	INT16 baseGCost = GetAStarG(ParentNode);
	//int baseAP = GetActionPoints(ParentNode);

	if (fCopyReachable) 
	{
		//if (GetPrevCost(ParentNode) != TRAVELCOST_FENCE) 
		{
			gpWorldLevelData[ParentNode].uiFlags |= MAPELEMENT_REACHABLE;
			if (gubBuildingInfoToSet > 0) 
			{
				gubBuildingInfo[ ParentNode ] = gubBuildingInfoToSet;
			}
		}
	}

	if (fContinuousTurnNeeded)
	{
		INT32 parent = GetAStarParent(ParentNode);
		if (parent == -1) 
		{
			lastDir = pSoldier->ubDirection;
		}
		else if ( GetLoopState(parent) == false )
		{
			lastDir = GetDirection(parent);
		}
		else
		{
			lastDir = gOppositeDirection[ GetDirection(parent) ];
		}
		startDir = lastDir;
		endDir = lastDir;
		bLoopState = LOOPING_CLOCKWISE;
		fCheckedBehind = FALSE;
	}
	else
	{
		//startDir and endDir are init to 0, and not reinit after a loop in the original code
	}

	//because startDir and endDir are set to the same thing, and to easily use
	//continue along with the loop increment at the end of the loop, use functions
	for (InitLoop(); ContinueLoop(); IncrementLoop()) 
	{
		int prevCost = 1;

		CurrentNode = ParentNode + dirDelta[direction];

		//if the node is not in the world, or it has already been searched, continue
		if (CurrentNode < 0 ||
			CurrentNode >= WORLD_MAX ||
			GetAStarStatus(CurrentNode) == AStar_Closed) 
		{
			continue;
		}

#ifdef VEHICLE
		//has side effects, including setting loop counters
		int retVal = VehicleObstacleCheck();
		if (retVal == 1)
		{
			continue;
		}
		else if (retVal == 2)
		{
			return;
		}
#endif

		//calc the cost to move from the current node to here
		INT16 terrainCost = EstimateActionPointCost( pSoldier, CurrentNode, direction, movementMode, 0, 3 );

		if (terrainCost == 100) {
			if (fFindClimbPoints)
			{
				if ((direction & 1) == 0 &&
					NewOKDestination( pSoldier, CurrentNode, FALSE, 0) &&
					FindStructure( CurrentNode, STRUCTURE_ROOF ) == NULL)
				{
					gpWorldLevelData[ CurrentNode ].ubExtFlags[0] |= MAPELEMENT_EXT_CLIMBPOINT;
					gpWorldLevelData[ ParentNode ].ubExtFlags[1] |= MAPELEMENT_EXT_CLIMBPOINT;
#ifdef ROOF_DEBUG
					gsCoverValue[CurrentNode] = 1;
#endif

				}
			}

			//an error code like diagonal door or obstruction was returned
			continue;
		}

		//ddd: window. we can forbid standing on tiles with windows
		//if (gubWorldMovementCosts[CurrentNode][direction][pSoldier->pathing.bLevel] == TRAVELCOST_JUMPABLEWINDOW)
		//	continue;


		if (gubWorldMovementCosts[CurrentNode][direction][pSoldier->pathing.bLevel] == TRAVELCOST_FENCE)
		{
			SetAStarStatus(CurrentNode, AStar_Closed);
			CurrentNode = CurrentNode + dirDelta[direction];
			if (!NewOKDestination( pSoldier, CurrentNode, fPathAroundPeople, pSoldier->pathing.bLevel))
			{
				continue;
			}
			prevCost = 2;
		}

		//if the node is not traversible, continue
		if (WantToTraverse() == false) 
		{
			continue;
		}

		// if contemplated tile is NOT final dest and someone there, disqualify route
		// when doing a reachable test, ignore all locations with people in them
		if (IsSomeoneInTheWay() == true) {
			continue;
		}

		//int movementG = TerrainCostToAStarG(terrainCost);
		//INT16 movementG = CalcAP( terrainCost, direction);
		INT16 movementG;
		//movementG = (INT16) CalcG( &prevCost);
		//movementG = CalcAP( movementG, direction);
		
		//movementG = terrainCost * 100;
		movementG = terrainCost;
		// Favor continuing in the same direction by increasing the cost for changing direction
		INT32 ParentParentNode = GetAStarParent( ParentNode);
		if ((ParentParentNode != -1 &&
			 ParentNode - ParentParentNode != CurrentNode - ParentNode) ||
			 (ParentParentNode == -1 && (direction & 1) ) )
		{
			movementG++;
		}

		INT16 AStarG = baseGCost + movementG;
		//if the node is more costly in this path than in another open path, continue
		if (GetAStarStatus(CurrentNode) == AStar_Open
			&& AStarG >= GetAStarG(CurrentNode))
		{
			continue;
		}

		if (maxAPBudget && AStarG > maxAPBudget * 100) 
		{
			continue;
		}

		int extraGCoverCost = 0;
#ifdef ASTAR_USING_EXTRACOVER
		//check if we will run out of AP while entering this node or before
		//if we run out, the merc will stop at the parent node for a turn and be vulnerable
		if (mercsMaxAPs && APCost > mercsMaxAPs)
		{

			extraGCoverCost = GetExtraGCover(ParentNode);
			//if the parent did not have the cost cached, then calc it
			if (extraGCoverCost == -1)
			{
				//use the stance and cover to see how much we really want to stop at the parent node
				//as opposed to an equal path with different cover
				//because other nodes further on the path will stop here too, add this value to the F cost
				extraGCoverCost = CalcGCover(ParentNodeIndex, APCost);

				//remember, we have run out of points to *enter* this node, so we are stuck at the *parent* node
				//cache the cost to stay at the parent node
				SetExtraGCover(ParentNode, extraGCoverCost);
			}
			else
			{
				//the parent node is too long, all other nodes from start node
				//will be too long too, and have the extra cost already added
			}
		}
#endif
		int AStarH = CalcH();
		int AStarF = (AStarG + extraGCoverCost) + AStarH;

#if defined( PATHAI_VISIBLE_DEBUG )
		if (gfDisplayCoverValues && gfDrawPathPoints) 
		{
			if (gsCoverValue[CurrentNode] == 0x7F7F) 
			{
				//gsCoverValue[CurrentNode] = PATHAI_VISIBLE_DEBUG_Counter++;
				gsCoverValue[CurrentNode] = (INT16) AStarF;
			}
			/*
			else if (gsCoverValue[CurrentNodeIndex] >= 0) 
			{
				gsCoverValue[CurrentNodeIndex]++;
			}
			else 
			{
				gsCoverValue[CurrentNodeIndex]--;
			}
			*/
		}
#endif

		//insert this node onto the heap
		if (GetAStarStatus(CurrentNode) == AStar_Init)
		{
			OpenHeap.insertElement(CurrentNode, AStarF);
		}
		else
		{
			OpenHeap.editElement(CurrentNode, AStarF);
		}

		SetAStarStatus(CurrentNode, AStar_Open);
		//SetAStarF(CurrentNode, AStarF);
		SetAStarG(CurrentNode, AStarG);
		//SetActionPoints(CurrentNode, APCost);
		SetAStarParent(CurrentNode, ParentNode);
		SetLoopState(CurrentNode, bLoopState);
		SetDirection(CurrentNode, direction);
		//SetPrevCost(CurrentNode, prevCost);
		SetExtraGCover(CurrentNode, extraGCoverCost);
		// Set the number of steps for this tile.  It's 2 if we are hopping a fence
		SetNumSteps( CurrentNode, prevCost);

		if (maxAPBudget) 
		{
			if (fCopyPathCosts) 
			{
				//This is used by FindBestNearbyCover to look for something that can be reached this turn
				GridNode *n1, *n2;
				n1 = GridNode::MapXY + CurrentNode;
				n2 = GridNode::MapXY + StartNode;
				int iX = AI_PATHCOST_RADIUS + n1->x - n2->x;
				int iY = AI_PATHCOST_RADIUS + n1->y - n2->y;
				Assert( iX >= 0 && iX <= AI_PATHCOST_RADIUS*2 && iY >= 0 && iY <= AI_PATHCOST_RADIUS*2);
				gubAIPathCosts[iX][iY] = (INT8) AStarG;
			}
		}
	}

	return;
}//end ExecuteAStarLogic

INT16 AStarPathfinder::CalcStartingAP()
{
	// Add to points, those needed to start from different stance!
	INT16 startingAPCost = MinAPsToStartMovement( pSoldier, movementMode );

	// WANNE - BMP: We have to reset the CurrentNode.
	// This can happen, if the previous savegame was on a big map
	// and now we are on a "normal" map.
	if (CurrentNode > WORLD_MAX)
		CurrentNode = 0;

	// We should reduce points for starting to run if first tile is a fence...
	if ( gubWorldMovementCosts[ CurrentNode ][ direction ][ onRooftop ] == TRAVELCOST_FENCE )
	{
		if ( movementMode == RUNNING && pSoldier->usAnimState != RUNNING )
		{
			startingAPCost = startingAPCost - GetAPsStartRun( pSoldier ); // changed by SANDRO
		}
	}
	return startingAPCost;
}

int AStarPathfinder::TerrainCostToAStarG(int const terrainCost)
{
	int movementG;
	// NOTE: on September 24, 1997, Chris went back to a diagonal bias system
	if (IsDiagonal(direction) == true)
	{
		movementG = terrainCost * 14;
	}
	else
	{
		movementG = terrainCost * 10;
	}

	if ( bLoopState == LOOPING_REVERSE)
	{
		// penalize moving backwards to encourage turning sooner
		movementG += 500;
	}
	return movementG;
}

INT16 AStarPathfinder::CalcAP(int const terrainCost, UINT8 const direction)
{
	// NEW Apr 21 by Ian: abort if cost exceeds budget
	INT16 movementAPCost;
	switch(terrainCost)
	{
		case TRAVELCOST_NONE:		movementAPCost = 0; break;

		case TRAVELCOST_DIRTROAD:
		case TRAVELCOST_FLAT:		movementAPCost = APBPConstants[AP_MOVEMENT_FLAT]; break;

		//case TRAVELCOST_BUMPY:
		case TRAVELCOST_GRASS:		movementAPCost = APBPConstants[AP_MOVEMENT_GRASS]; break;

		case TRAVELCOST_THICK:		movementAPCost = APBPConstants[AP_MOVEMENT_BUSH]; break;

		case TRAVELCOST_DEBRIS:		movementAPCost = APBPConstants[AP_MOVEMENT_RUBBLE]; break;

		case TRAVELCOST_SHORE:		movementAPCost = APBPConstants[AP_MOVEMENT_SHORE]; break; // wading shallow water

		case TRAVELCOST_KNEEDEEP:	movementAPCost = APBPConstants[AP_MOVEMENT_LAKE]; break; // wading waist/chest deep - very slow


		case TRAVELCOST_DEEPWATER:	movementAPCost = APBPConstants[AP_MOVEMENT_OCEAN]; break; // can swim, so it's faster than wading

		//case TRAVELCOST_VEINEND:
		//case TRAVELCOST_VEINMID:	movementAPCost = APBPConstants[AP_MOVEMENT_FLAT]; break;

		//case TRAVELCOST_DOOR:		movementAPCost = APBPConstants[AP_MOVEMENT_FLAT]; break;

		case TRAVELCOST_FENCE:		movementAPCost = APBPConstants[AP_JUMPFENCE]; break;
			///dddokno{
		case TRAVELCOST_JUMPABLEWINDOW:
		case TRAVELCOST_JUMPABLEWINDOW_N:
		case TRAVELCOST_JUMPABLEWINDOW_W:
									movementAPCost = APBPConstants[AP_JUMPFENCE]; break;
			///dddokno}

		case TRAVELCOST_OBSTACLE:
		default:					return -1;	// Cost too much to be considered!
	}


	// don't make the mistake of adding directly to
	// ubCurAPCost, that must be preserved for remaining dirs!
	if (IsDiagonal(direction))
	{
		movementAPCost = (movementAPCost * 14) / 10;
	}

	int movementModeToUseForAPs = movementMode;

	// ATE: if water, force to be walking always!
	if ( terrainCost == TRAVELCOST_SHORE || terrainCost == TRAVELCOST_KNEEDEEP || terrainCost == TRAVELCOST_DEEPWATER )
	{
		movementModeToUseForAPs = WALKING;
	}

	// adjust AP cost for movement mode
	switch( movementModeToUseForAPs )
	{
		case RUNNING:
		case ADULTMONSTER_WALKING:
			// save on casting
			//movementAPCost = (INT16) (movementAPCost * 100 / ( (UINT8) (RUNDIVISOR * 100)));
			movementAPCost = (INT16) movementAPCost + APBPConstants[AP_MODIFIER_RUN];
			//movementAPCost = (INT16)(DOUBLE)( (sTileCost / RUNDIVISOR) );	break;
			break;
		case WALKING:
		case ROBOT_WALK:
			movementAPCost = (movementAPCost + APBPConstants[AP_MODIFIER_WALK]);	//WALKCOST);
			break;
		case SWATTING:
			movementAPCost = (movementAPCost + APBPConstants[AP_MODIFIER_SWAT]);	//SWATCOST);
			break;
		case CRAWLING:
			movementAPCost = (movementAPCost + APBPConstants[AP_MODIFIER_CRAWL]);	//CRAWLCOST);
			break;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - STOMP traits - Athletics trait decreases movement cost
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, ATHLETICS_NT ))
	{
		movementAPCost = max(1, (INT16)((movementAPCost * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100) + 0.5));
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	// Flugente: riot shields lower movement speed
	if ( pSoldier->IsRiotShieldEquipped( ) )
	{
		movementAPCost *= gItemSettings.fShieldMovementAPCostModifier;
	}

	// Flugente: dragging someone
	if ( pSoldier->IsDraggingSomeone( ) )
	{
		movementAPCost *= gItemSettings.fDragAPCostModifier;
	}

	if (terrainCost == TRAVELCOST_FENCE)
	{
		switch( movementModeToUseForAPs )
		{
			case RUNNING:
			case WALKING :
				// Here pessimistically assume the path will continue after hopping the fence
				movementAPCost += GetAPsCrouch( pSoldier, TRUE ); // SANDRO - changed
				break;

			case SWATTING:
				// Add cost to stand once there BEFORE jumping....
				movementAPCost += GetAPsCrouch( pSoldier, TRUE ); // SANDRO - changed
				break;

			case CRAWLING:
				// Can't do it here.....
				return -1;
		}
	}
	//dddokno 
	else if ( terrainCost == TRAVELCOST_JUMPABLEWINDOW
		|| terrainCost == TRAVELCOST_JUMPABLEWINDOW_N
		|| terrainCost == TRAVELCOST_JUMPABLEWINDOW_W )
	{
		switch(movementModeToUseForAPs)
		{
			case RUNNING:
			case WALKING :
			case SWATTING:
				movementAPCost += APBPConstants[AP_CROUCH];
				break;

			default:
				break;
		}
	}
//dddokno 
	else if (terrainCost == TRAVELCOST_NOT_STANDING)
	{
		switch(movementModeToUseForAPs)
		{
			case RUNNING:
			case WALKING :
				// charge crouch APs for ducking head!
				movementAPCost += GetAPsCrouch( pSoldier, TRUE ); // SANDRO - changed
				break;

			default:
				break;
		}
	}
	else if (fGoingThroughDoor)
	{
		// Uh, there IS a cost to close the door too!
		// Then if the door has to be unlocked to open there's the cost of locking and unlocking.
		movementAPCost += GetAPsToOpenDoor( pSoldier ) + GetAPsToOpenDoor( pSoldier ); // changed by SANDRO
		fGoingThroughDoor = FALSE;
	}
	return movementAPCost;
}

int AStarPathfinder::CalcG(int* pPrevCost)
{
	//how much is admission to the next tile
	if ( gfPathAroundObstacles == false)
	{
		return TRAVELCOST_FLAT;
	}


	int nextCost = gubWorldMovementCosts[ CurrentNode ][ direction ][ onRooftop ];
	*pPrevCost = nextCost;

	//if we are holding down shift and finding a direct path, count non obstacles as flat terrain
	if (gfPlotDirectPath && nextCost < NOPASS && nextCost != 0)
	{
		return TRAVELCOST_FLAT;
	}

	//performance: if nextCost is low then do not do many many if == checks
	if ( nextCost >= TRAVELCOST_FENCE )
	{
		//ATE:	Check for differences from reality
		// Is next cost an obstcale
		if ( nextCost == TRAVELCOST_HIDDENOBSTACLE ) 
		{
			if ( fPathingForPlayer ) 
			{
				// Is this obstacle a hidden tile that has not been revealed yet?
				BOOLEAN fHiddenStructVisible;
				if( DoesGridNoContainHiddenStruct( CurrentNode, &fHiddenStructVisible ) ) 
				{
					// Are we not visible, if so use terrain costs!
					if ( !fHiddenStructVisible )
					{
						// Set cost of terrain!
						nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
					}
				}
			}
		}
		else if ( nextCost == TRAVELCOST_NOT_STANDING )
		{
			// for path plotting purposes, use the terrain value
			nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
		}
		else if ( nextCost == TRAVELCOST_EXITGRID )
		{
			if (gfPlotPathToExitGrid)
			{
				// replace with terrain cost so that we can plot path, otherwise is obstacle
				nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
			}
		}

		//ddd: window. { check 2 conditions: 1. if next tile is a window and we will have to hump through it
		//2. if current tile is a window and we should jump through the window to reach another tile
		else if ( nextCost == TRAVELCOST_JUMPABLEWINDOW
			|| nextCost == TRAVELCOST_JUMPABLEWINDOW_N
			|| nextCost == TRAVELCOST_JUMPABLEWINDOW_W)
		{

			// don't let anyone path diagonally through doors!
			if (IsDiagonal(direction) == true)
			{
				return -1;
			}
			nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];//?

		}
		//ddd: window }

		else if ( nextCost == TRAVELCOST_FENCE && fNonFenceJumper )
		{
			return -1;
		}
		else if ( IS_TRAVELCOST_DOOR( nextCost ) )
		{
			// don't let anyone path diagonally through doors!
			if (IsDiagonal(direction) == true)
			{
				return -1;
			}

			INT32 iDoorGridNo = CurrentNode;
			bool fDoorIsObstacleIfClosed = FALSE;
			bool fDoorIsOpen = false;
			switch( nextCost )
			{
				case TRAVELCOST_DOOR_CLOSED_HERE:
					fDoorIsObstacleIfClosed = TRUE;
					iDoorGridNo = CurrentNode;
					break;
				case TRAVELCOST_DOOR_CLOSED_N:
					fDoorIsObstacleIfClosed = TRUE;
					iDoorGridNo = CurrentNode + dirDelta[ NORTH ];
					break;
				case TRAVELCOST_DOOR_CLOSED_W:
					fDoorIsObstacleIfClosed = TRUE;
					iDoorGridNo = CurrentNode + dirDelta[ WEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_HERE:
					iDoorGridNo = CurrentNode;
					break;
				case TRAVELCOST_DOOR_OPEN_N:
					iDoorGridNo = CurrentNode + dirDelta[ NORTH ];
					break;
				case TRAVELCOST_DOOR_OPEN_NE:
					iDoorGridNo = CurrentNode + dirDelta[ NORTHEAST ];
					break;
				case TRAVELCOST_DOOR_OPEN_E:
					iDoorGridNo = CurrentNode + dirDelta[ EAST ];
					break;
				case TRAVELCOST_DOOR_OPEN_SE:
					iDoorGridNo = CurrentNode + dirDelta[ SOUTHEAST ];
					break;
				case TRAVELCOST_DOOR_OPEN_S:
					iDoorGridNo = CurrentNode + dirDelta[ SOUTH ];
					break;
				case TRAVELCOST_DOOR_OPEN_SW:
					iDoorGridNo = CurrentNode + dirDelta[ SOUTHWEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_W:
					iDoorGridNo = CurrentNode + dirDelta[ WEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_NW:
					iDoorGridNo = CurrentNode + dirDelta[ NORTHWEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_N_N:
					iDoorGridNo = CurrentNode + dirDelta[ NORTH ] + dirDelta[ NORTH ];
					break;
				case TRAVELCOST_DOOR_OPEN_NW_N:
					iDoorGridNo = CurrentNode + dirDelta[ NORTHWEST ] + dirDelta[ NORTH ];
					break;
				case TRAVELCOST_DOOR_OPEN_NE_N:
					iDoorGridNo = CurrentNode + dirDelta[ NORTHEAST ] + dirDelta[ NORTH ];
					break;
				case TRAVELCOST_DOOR_OPEN_W_W:
					iDoorGridNo = CurrentNode + dirDelta[ WEST ] + dirDelta[ WEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_SW_W:
					iDoorGridNo = CurrentNode + dirDelta[ SOUTHWEST ] + dirDelta[ WEST ];
					break;
				case TRAVELCOST_DOOR_OPEN_NW_W:
					iDoorGridNo = CurrentNode + dirDelta[ NORTHWEST ] + dirDelta[ WEST ];
					break;
				default:
					break;
			}

			if ( fPathingForPlayer && gpWorldLevelData[ iDoorGridNo ].ubExtFlags[0] & MAPELEMENT_EXT_DOOR_STATUS_PRESENT )
			{
				// check door status
				DOOR_STATUS* pDoorStatus = GetDoorStatus( iDoorGridNo );
				if (pDoorStatus)
				{
					fDoorIsOpen = (pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN) != 0;
				}
				else
				{
					// door destroyed?
					nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
				}
			}
			else
			{
				// check door structure
				STRUCTURE* pDoorStructure = FindStructure( iDoorGridNo, STRUCTURE_ANYDOOR );
				if (pDoorStructure)
				{
					fDoorIsOpen = (pDoorStructure->fFlags & STRUCTURE_OPEN) != 0;
				}
				else
				{
					// door destroyed?
					nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
				}
			}

			// now determine movement cost... if it hasn't been changed already
			if ( IS_TRAVELCOST_DOOR( nextCost ) )
			{
				if (fDoorIsOpen)
				{
					if (fDoorIsObstacleIfClosed)
					{
						nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
					}
					else
					{
						nextCost = TRAVELCOST_OBSTACLE;
					}
				}
				else
				{
					if (fDoorIsObstacleIfClosed)
					{
						// door is closed and this should be an obstacle, EXCEPT if we are calculating
						// a path for an enemy or NPC with keys
						if ( fPathingForPlayer || ( pSoldier && (pSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL) ) ) ) 
						{
							nextCost = TRAVELCOST_OBSTACLE;
						}
						else
						{
							// have to check if door is locked and NPC does not have keys!
							// This function has an inaccurate name.  It is actually checking if the door has LOCK info.
							DOOR* pDoor = FindDoorInfoAtGridNo( iDoorGridNo );
							if (pDoor)
							{
								if (!pDoor->fLocked || pSoldier->flags.bHasKeys)
								{
									// add to AP cost
									//if (maxAPBudget)
									{
										fGoingThroughDoor = TRUE;
									}
									nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
								}
								else
								{
									nextCost = TRAVELCOST_OBSTACLE;
								}
							}
							else
							{
								// The door is closed, so we still have to open it
								fGoingThroughDoor = TRUE;
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
							}
						}
					}
					else
					{
						nextCost = gTileTypeMovementCost[ gpWorldLevelData[ CurrentNode ].ubTerrainID ];
					}
				}
			}
		}
		else if ( fNonSwimmer && ISWATER( gpWorldLevelData[ CurrentNode ].ubTerrainID))
		{
			// creatures and animals can't go in water
			nextCost = TRAVELCOST_OBSTACLE;
		}
	}

	// Apr. '96 - moved up be ahead of AP_Budget stuff
	if ( nextCost >= NOPASS ) // || ( nextCost == TRAVELCOST_DOOR ) )
	{
		return -1;
	}

	// make water cost attractive for water to water paths
	// Why?  If a path through water gets you there sooner, you shouldn't need
	// artificial inflation to figure that out.  And if not, then get out of the water!
	//if (bWaterToWater && ISWATER(nextCost) ) 
	//{
	//	nextCost = EASYWATERCOST;
	//}

	return nextCost;
}//end CalcG

int AStarPathfinder::CalcH()
{
	if ( fCopyReachable )
	{
		//if this is a reachability test, turn AStar into breadth first Dijkstra
		return 0; // Technically speaking, 0 makes it Dijkstra best-first search
	}

//	#define ESTIMATEC ( ( (dx<dy) ? (TRAVELCOST_BUMPY * (dx * 14 + dy * 10) / 10) : ( TRAVELCOST_BUMPY * (dy * 14 + dx * 10) / 10 ) ) )
	//the original calcH grossly overestimates,
	//one reason is it uses BUMPY not FLAT!
	//assuming x dist and y dist are the same, that it is totally diagonal
	//ESTIMATEC returns the diagonal dist plus an extra straight distance!!!

	//furthermore let's not lose precision by dividing by 10, as long as 10 is taken into account elsewhere
	//this also helps with cover cost precision where it is needed more

	//I changed the cost scale to APs, so TRAVELCOST * anything is a gross overestimate.  Let's do something
	//more on an AP scale.  The minimum between X and Y is calculated as a diagonal traversal and the remainder
	//is calculated as straight traversal.

	GridNode *n1, *n2;

	n1 = GridNode::MapXY + CurrentNode;
	n2 = GridNode::MapXY + DestNode;

	int x = abs(n1->x - n2->x);
	int y = abs(n1->y - n2->y);
#if 1
	if (x >= y) 
	{
		return this->travelcostDiag * y + this->travelcostOrth * (x-y);
	}
	else
	{
		return this->travelcostDiag * x + this->travelcostOrth * (y-x);
	}
#else
	// Try a real distance method.  This should underestimate in some cases
	// However, the distances need to be increased for the moment because running orthogonal is 1AP while running diagonal is 2AP
	// so the total to reach a diagonal tile is identical for 2 moves.  So we have to trick the pathing calc into thinking it's
	// a longer distance and also calculate the other costs accordingly.

	x *= 100;
	y *= 100;

	int d = x*x + y*y;
	int r = 1200; // Just a guess

	if (d == 0)
	{
		return d;
	}

	while (1)
	{
		int gr = (r + (d/r)) / 2;
		if (gr == r || gr == r+1)
		{
			break;
		}
		r = gr;
	}

	return r * travelcostOrth;
#endif
}

#ifdef ASTAR_USING_EXTRACOVER
int AStarPathfinder::CalcGCover(int const NodeIndex,
								int const APCost)
{
	// There's no cover when boxing!
	if (gTacticalStatus.bBoxingState == BOXING) {
		return 0;
	}

	std::vector<THREATTYPE> Threats;
	// all 32-bit integers for max. speed
	INT32	iCurrentCoverValue;
	INT32	iThreatCertainty;
	INT32	iThreatRange, iClosestThreatRange = 1500;
	INT32	iMyThreatValue;
	INT16	sThreatLoc;
	UINT32	uiThreatCnt = 0;
	INT16	*		pusLastLoc;
	INT8 *		pbPersOL;
	INT8 *		pbPublOL;

	//although we have run out of APs to get here, it could just mean we have some APs but not enough to enter
	int APLeft = this->mercsMaxAPs - APCost;

	// BUILD A LIST OF THREATENING GRID #s FROM PERSONAL & PUBLIC opplists
	pusLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

	// hang a pointer into personal opplist
	pbPersOL = &(pSoldier->bOppList[0]);
	// hang a pointer into public opplist
	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);

	// calculate OUR OWN general threat value (not from any specific location)
	iMyThreatValue = CalcManThreatValue(pSoldier, NOWHERE, FALSE, pSoldier);

	// look through all opponents for those we know of
	for (UINT32 uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
		SOLDIERTYPE* pOpponent = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, dead, unconscious
		if (!pOpponent || pOpponent->bLife < OKLIFE) {
			continue;			// next merc
		}

		// if this man is neutral / on the same side, he's not an opponent
 		if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide)) {
			continue;			// next merc
		}

		pbPersOL = pSoldier->bOppList + pOpponent->ubID; 
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpponent->ubID;
		pusLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpponent->ubID;

		// if this opponent is unknown personally and publicly
		if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN)) {
			continue;			// next merc
		}

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != 64) {
			continue;	// next opponent
		}

		// if personal knowledge is more up to date or at least equal
		if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
			(*pbPersOL == *pbPublOL)) {

			// using personal knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = *pusLastLoc;
			iThreatCertainty = ThreatPercent[*pbPersOL - OLDEST_HEARD_VALUE];
		}
		else {
			// using public knowledge, obtain opponent's "best guess" gridno
			sThreatLoc = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpponent->ubID];
			iThreatCertainty = ThreatPercent[*pbPublOL - OLDEST_HEARD_VALUE];
		}

		// calculate how far away this threat is (in adjusted pixels)
		iThreatRange = GetRangeInCellCoordsFromGridNoDiff( NodeIndex, sThreatLoc );

#ifdef DEBUGCOVER
//		DebugAI( String( "FBNC: Opponent %d believed to be at gridno %d (mine %d, public %d)\n",iLoop,sThreatLoc,*pusLastLoc,PublicLastKnownOppLoc[pSoldier->bTeam][iLoop] ) );
#endif

		// if this opponent is believed to be too far away to really be a threat
		if (iThreatRange > MAX_THREAT_RANGE) {
			continue;			// check next opponent
		}

		Threats.push_back(THREATTYPE());
		// remember this opponent as a current threat, but DON'T REDUCE FOR COVER!
		Threats[uiThreatCnt].iValue = CalcManThreatValue(pOpponent, NodeIndex, FALSE, pSoldier);

		// if the opponent is no threat at all for some reason
		if (Threats[uiThreatCnt].iValue == -999) {
			continue;			// check next opponent
		}

		Threats[uiThreatCnt].pOpponent		= pOpponent;
		Threats[uiThreatCnt].sGridNo		= sThreatLoc;
		Threats[uiThreatCnt].iCertainty		= iThreatCertainty;
		Threats[uiThreatCnt].iOrigRange		= iThreatRange;

		// calculate how many APs he will have at the start of the next turn
		Threats[uiThreatCnt].iAPs = CalcActionPoints(pOpponent);

		if (iThreatRange < iClosestThreatRange) {
			iClosestThreatRange = iThreatRange;
		}

		uiThreatCnt++;
	}

	// if no known opponents were found to threaten us, can't worry about cover
	if (uiThreatCnt == 0) {
		return 0;
	}

	// calculate our current cover value in the place we are now
	iCurrentCoverValue = 0;

	// for every opponent that threatens, consider this spot's cover vs. him
	for (UINT32 uiLoop = 0; uiLoop < uiThreatCnt; uiLoop++) {

		// if this threat is CURRENTLY within 20 tiles
		if (Threats[uiLoop].iOrigRange <= MAX_THREAT_RANGE) {

			// add this opponent's cover value to our current total cover value
			iCurrentCoverValue += CalcCoverValue(NodeIndex, iMyThreatValue, APLeft,
				Threats[uiLoop].iOrigRange, Threats[uiLoop].sGridNo, Threats[uiLoop].pOpponent,
				Threats[uiLoop].iValue, Threats[uiLoop].iAPs, Threats[uiLoop].iCertainty);
		}
	}

	//body shield!
	iCurrentCoverValue -= ( ( iCurrentCoverValue * NumberOfTeamMatesAdjacent( pSoldier, NodeIndex ) ) / 10 );

	//make the cost less than one TRAVELCOST_FLAT's worth so that this cost doesn't alter the path's APs
	return (min (iCurrentCoverValue, (TRAVELCOST_FLAT * 10 - 1)));
}

int AStarPathfinder::CalcCoverValue(INT32 sMyGridNo, INT32 iMyThreat, INT32 iMyAPsLeft,
									INT32 myThreatsiOrigRange, INT32 myThreatssGridNo, SOLDIERTYPE* myThreatspOpponent,
									INT32 myThreatsiValue, INT32 myThreatsiAPs, INT32 myThreatsiCertainty)
{
	SOLDIERTYPE* pMe = this->pSoldier;
	INT32	morale = pSoldier->bAIMorale;

	INT32	iRange = myThreatsiOrigRange;
	// all 32-bit integers for max. speed
	INT32	sHisGridNo, sMyRealGridNo, sHisRealGridNo = NOWHERE;
	INT16 sTempX, sTempY;
	FLOAT dMyX, dMyY, dHisX, dHisY;
	INT8	bHisBestCTGT, bHisActualCTGT, bHisCTGT, bMyCTGT;
	SOLDIERTYPE *pHim;

	dMyX = dMyY = dHisX = dHisY = -1.0;

	pHim = myThreatspOpponent;
	sHisGridNo = myThreatssGridNo;

	// THE FOLLOWING STUFF IS *VEERRRY SCAARRRY*, BUT SHOULD WORK.	IF YOU REALLY
	// HATE IT, THEN CHANGE ChanceToGetThrough() TO WORK FROM A GRIDNO TO GRIDNO

	// this is theoretical, and I'm not actually at sMyGridNo right now
	sMyRealGridNo = pMe->sGridNo;		// remember where I REALLY am
	dMyX = pMe->dXPos;
	dMyY = pMe->dYPos;

	pMe->sGridNo = sMyGridNo;				// but pretend I'm standing at sMyGridNo
	ConvertGridNoToCenterCellXY( sMyGridNo, &sTempX, &sTempY );
	pMe->dXPos = (FLOAT) sTempX;
	pMe->dYPos = (FLOAT) sTempY;

	// if this is theoretical, and he's not actually at hisGrid right now
	if (pHim->sGridNo != sHisGridNo)
	{
		sHisRealGridNo = pHim->sGridNo;		// remember where he REALLY is
		dHisX = pHim->dXPos;
		dHisY = pHim->dYPos;

		pHim->sGridNo = sHisGridNo;			// but pretend he's standing at sHisGridNo
		ConvertGridNoToCenterCellXY( sHisGridNo, &sTempX, &sTempY );
		pHim->dXPos = (FLOAT) sTempX;
		pHim->dYPos = (FLOAT) sTempY;
	}


	if (InWaterOrGas(pHim,sHisGridNo))
	{
		bHisActualCTGT = 0;
	}
	else
	{
		// optimistically assume we'll be behind the best cover available at this spot
		bHisActualCTGT = CalcWorstCTGTForPosition( pHim, pMe->ubID, sMyGridNo, pMe->bLevel, iMyAPsLeft );
	}

	// normally, that will be the cover I'll use, unless worst case over-rides it
	bHisCTGT = bHisActualCTGT;

	// only calculate his best case CTGT if there is room for improvement!
	if (bHisActualCTGT < 100)
	{
		// if we didn't remember his real gridno earlier up above, we got to now,
		// because calculating worst case is about to play with it in a big way!
		if (TileIsOutOfBounds(sHisRealGridNo))
		{
			sHisRealGridNo = pHim->sGridNo;		// remember where he REALLY is
			dHisX = pHim->dXPos;
			dHisY = pHim->dYPos;
		}

		// calculate where my cover is worst if opponent moves just 1 tile over
		bHisBestCTGT = CalcBestCTGT(pHim, pMe->ubID, sMyGridNo, pMe->bLevel, iMyAPsLeft);

		// if he can actually improve his CTGT by moving to a nearby gridno
		if (bHisBestCTGT > bHisActualCTGT)
		{
			// he may not take advantage of his best case, so take only 2/3 of best
			bHisCTGT = ((2 * bHisBestCTGT) + bHisActualCTGT) / 3;
		}
	}

	// if my intended gridno is in water or gas, I can't attack at all from there
	// here, for smoke, consider bad
	if (InWaterGasOrSmoke(pMe,sMyGridNo))
	{
		bMyCTGT = 0;
	}
	else
	{
		// put him at sHisGridNo if necessary!
		if (pHim->sGridNo != sHisGridNo )
		{
			pHim->sGridNo = sHisGridNo;
			ConvertGridNoToCenterCellXY( sHisGridNo, &sTempX, &sTempY );
			pHim->dXPos = (FLOAT) sTempX;
			pHim->dYPos = (FLOAT) sTempY;
		}

		// let's not assume anything about the stance the enemy might take, so take an average
		// value... no cover give a higher value than partial cover
		bMyCTGT = CalcAverageCTGTForPosition( pMe, pHim->ubID, sHisGridNo, pHim->bLevel, iMyAPsLeft );

		// since NPCs are too dumb to shoot "blind", ie. at opponents that they
		// themselves can't see (mercs can, using another as a spotter!), if the
		// cover is below the "see_thru" threshold, it's equivalent to perfect cover!
		if (bMyCTGT < SEE_THRU_COVER_THRESHOLD)
		{
			bMyCTGT = 0;
		}
	}

	// UNDO ANY TEMPORARY "DAMAGE" DONE ABOVE
	pMe->sGridNo = sMyRealGridNo;		// put me back where I belong!
	pMe->dXPos = dMyX;						// also change the 'x'
	pMe->dYPos = dMyY;						// and the 'y'

	if (!TileIsOutOfBounds(sHisRealGridNo))
	{
		pHim->sGridNo = sHisRealGridNo;		// put HIM back where HE belongs!
		pHim->dXPos = dHisX;					// also change the 'x'
		pHim->dYPos = dHisY;					// and the 'y'
	}


	// these value should be < 1 million each
	float HisPosValue = (float) (bHisCTGT * myThreatsiValue * myThreatsiAPs);
	float MyPosValue =	(float) (bMyCTGT *	iMyThreat * iMyAPsLeft);

	// try to account for who outnumbers who: the side with the advantage thus
	// (hopefully) values offense more, while those in trouble will play defense
	if (pHim->bOppCnt > 1)
	{
		HisPosValue /= pHim->bOppCnt;
	}

	if (pMe->bOppCnt > 1)
	{
		MyPosValue /= pMe->bOppCnt;
	}


 // if my positional value is worth something at all here
 if (MyPosValue > 0.0)
	{
	// if I CAN'T crouch when I get there, that makes it significantly less
	// appealing a spot (how much depends on range), so that's a penalty to me
	//ADB and if I'm not already crouching
	if (iMyAPsLeft < AP_CROUCH && movementMode != SWATTING)
	 // subtract another 1 % penalty for NOT being able to crouch per tile
	 // the farther away we are, the bigger a difference crouching will make!
	 MyPosValue -= ((MyPosValue * (AIM_PENALTY_TARGET_CROUCHED + (iRange / CELL_X_SIZE))) / 100);
	}

	// the farther apart we are, the less important the cover differences are
	// the less certain his position, the less important cover differences are
	float ReductionFactor = float (((MAX_THREAT_RANGE - iRange) * myThreatsiCertainty) /
		 (MAX_THREAT_RANGE * myThreatsiCertainty));

	int iCoverValue = (int) (((HisPosValue - MyPosValue) / HisPosValue) * ReductionFactor);

	return( max(iCoverValue, 0) );
}
#endif //#ifdef ASTAR_USING_EXTRACOVER

#ifdef VEHICLE
void AStarPathfinder::InitVehicle()
{
	fMultiTile = ((pSoldier->flags.uiStatusFlags & SOLDIER_MULTITILE) != 0);
	if (fMultiTile)
	{
		// Get animation surface...
		// Chris_C... change this to use parameter.....
		UINT16 usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, movementMode );
		// Get structure ref...
		pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, movementMode );

		if ( pStructureFileRef )
		{
			fContinuousTurnNeeded = ( ( pSoldier->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE) ) != 0 );

			/*
			//bool fVehicle = ( (pSoldier->uiStatusFlags & SOLDIER_VEHICLE) != 0 );
			if (fVehicle && pSoldier->bReverse)
			{
				fReverse = TRUE;
			}
			if (fVehicle || pSoldier->ubBodyType == COW || pSoldier->ubBodyType == BLOODCAT) // or a vehicle
			{
				fTurnSlow = TRUE;
			}
			*/
			if ( gfEstimatePath )
			{
				bOKToAddStructID = IGNORE_PEOPLE_STRUCTURE_ID;
			}
			else if ( pSoldier->pLevelNode != NULL && pSoldier->pLevelNode->pStructureData != NULL )
			{
				bOKToAddStructID = pSoldier->pLevelNode->pStructureData->usStructureID;
			}
			else
			{
				bOKToAddStructID = INVALID_STRUCTURE_ID;
			}

		}
		else
		{
			// turn off multitile pathing
			fMultiTile = FALSE;
			fContinuousTurnNeeded = FALSE;
		}
	}
	else
	{
		fContinuousTurnNeeded = FALSE;
	}
	return;
}

int AStarPathfinder::VehicleObstacleCheck()
{
	/*
	if (fTurnSlow) {
		if (iLastDir == iPrevToLastDir) {
			if ( iCnt != iLastDir && iCnt != gOneCDirection[ iLastDir ] && iCnt != gOneCCDirection[ iLastDir ]) {
				goto NEXTDIR;
			}
		}
		else {
			if ( iCnt != iLastDir ) {
				goto NEXTDIR;
			}
		}
	}
	*/

	int iStructIndex;
	if ( bLoopState == LOOPING_REVERSE )
	{
		iStructIndex = gOppositeDirection[ gOneCDirection[ direction ] ];
	}
	else {
		iStructIndex = gOneCDirection[ direction ];
	}

	if (fMultiTile)
	{
		if ( fContinuousTurnNeeded )
		{
			if ( direction != lastDir )
			{
				if ( !OkayToAddStructureToWorld( CurrentNode, onRooftop, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), bOKToAddStructID ) ) 
				{
					// we have to abort this loop and possibly reset the loop conditions to
					// search in the other direction (if we haven't already done the other dir)
					if (bLoopState == LOOPING_CLOCKWISE)
					{
						startDir = lastDir;
						endDir = direction;
						bLoopState = LOOPING_COUNTERCLOCKWISE; // backwards
						// when we go to the bottom of the loop, iLoopIncrement will be added to iCnt
						// which is good since it avoids duplication of effort
						direction = startDir;
						return 1;
						//goto NEXTDIR;
					}
					else if ( bLoopState == LOOPING_COUNTERCLOCKWISE && !fCheckedBehind )
					{
						// check rear dir
						bLoopState = LOOPING_REVERSE;

						// NB we're stuck with adding 1 to the loop counter down below so configure to accomodate...
						//iLoopStart = (iLastDir + (MAXDIR / 2) - 1) % MAXDIR;
						startDir = gOppositeDirection[ gOneCCDirection[ lastDir ] ];
						endDir = (startDir + 2) % MAXDIR;
						direction = startDir;
						fCheckedBehind = TRUE;
						return 1;
						//goto NEXTDIR;
					}
					else
					{
						// done
						//goto ENDOFLOOP;
						return 2;
					}

				}
			}
		}//end fContinuousTurnNeeded

		// check to make sure it's okay for us to turn to the new direction in our current tile

		// vehicle test for obstacles: prevent movement to next tile if
		// a tile covered by the vehicle in that position & direction
		// has an obstacle in it

		// because of the order in which animations are stored (dir 7 first,
		// then 0 1 2 3 4 5 6), we must subtract 1 from the direction
		// ATE: Send in our existing structure ID so it's ignored!
		// 0verhaul:  The turn happens before the move, and the check for change dir happens on the turn.  So
		// we have to check whether the struct can be added at the PARENT node, as well as the intended new node.
		if (!OkayToAddStructureToWorld( ParentNode, onRooftop, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), bOKToAddStructID ) ||
			!OkayToAddStructureToWorld( CurrentNode, onRooftop, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), bOKToAddStructID )) 
		{
			return 1;
			//goto NEXTDIR;
		}
	}
	return 0;
}
#endif

bool AStarPathfinder::WantToTraverse()
{

	// 0verhaul:	Cannot change direction over a fence!
	// Decided on a different way to handle this
	//if (GetPrevCost(ParentNode) == TRAVELCOST_FENCE && GetDirection(ParentNode) != direction)
	//{
	//	return false;
	//}

	if ( fVisitSpotsOnlyOnce && GetAStarStatus(CurrentNode) != AStar_Init ) 
	{
		// on a "reachable" test, never revisit locations, even if open!
		return false;
	}

	//checks distance, mines, cliffs
	if (gubNPCDistLimitSq) 
	{
		if ( gfNPCCircularDistLimit )
		{
			if (PythSpacesAway( StartNode, CurrentNode) > gubNPCDistLimitSq) 
			{
				return false;
			}
		}
		else
		{
			if (SpacesAway( StartNode, CurrentNode) > gubNPCDistLimit)
			{
				return false;
			}
		}
	}

	// AI check for mines
	if ( gpWorldLevelData[CurrentNode].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT && pSoldier->bSide != 0) 
	{
		return false;
	}

	// WANNE: Know mines (for enemy or player) do not explode - BEGIN
	if ( gpWorldLevelData[CurrentNode].uiFlags & (MAPELEMENT_ENEMY_MINE_PRESENT | MAPELEMENT_PLAYER_MINE_PRESENT) ) 
	{
		if (pSoldier->bSide == 0)
		{
			// For our team, skip a location with a known mines unless it is the end of our
			// path; for others on our side, skip such locations completely;
			if (pSoldier->bTeam != gbPlayerNum || CurrentNode != DestNode)
			{
				if (gpWorldLevelData[CurrentNode].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT) 
				{
					return false;
				}
			}
		}
		else
		{
			// For the enemy, always skip known mines
			if (gpWorldLevelData[CurrentNode].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT) 
			{
				return false;
			}
		}
	}
	// WANNE: - END

	//ATE: Movement onto cliffs? Check vs the soldier's gridno height
	// CJC: PREVIOUS LOCATION's height
	if ( gpWorldLevelData[ CurrentNode ].sHeight != gpWorldLevelData[ ParentNode ].sHeight ) 
	{
		return false;
	}
	
	///ddd{ 
	if(gGameExternalOptions.bNewTacticalAIBehavior)
	{
		// don't walk over corpses. TODO: only avoid in 80% ?
		if ( ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
				&& pSoldier->bTeam == ENEMY_TEAM 
				//&& IsCorpseAtGridNo( CurrentNode, pSoldier->pathing.bLevel ) 
				&& gubIsCorpseThere[CurrentNode]
				)
			return false;

		//from NightOps // elite AI will not walk on illuminated tiles, which are seen by the enemy
		if( ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
			&& pSoldier->bTeam == ENEMY_TEAM && pSoldier->ubProfile == NO_PROFILE 
			&& pSoldier->aiData.bAction != AI_ACTION_LEAVE_WATER_GAS
			//&& (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE 
			//|| ( (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY 
			//		|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR) 
			//		&& (BOOLEAN)(Random( 100 ) < 50)  ))
			//&& InLightAtNight((INT16)CurrentNode, gpWorldLevelData[ CurrentNode ].sHeight)
			&& gubWorldTileInLight[CurrentNode]
			&& gubMerkCanSeeThisTile[CurrentNode]
			)
		{
			return false;
			//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, L"svet!" );
				/*INT32 tcnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
				SOLDIERTYPE *tS;
				// look for all mercs on the same team,
				for ( tS = MercPtrs[ tcnt ]; tcnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; tcnt++,tS++ )
				{		
					if ( tS->stats.bLife >= OKLIFE && tS->sGridNo != NOWHERE && tS->bInSector )
						if ( SoldierTo3DLocationLineOfSightTest(
							tS, tS->sGridNo, pSoldier->pathing.bLevel, 3, 
							TRUE, CALC_FROM_WANTED_DIR ))
						{
							//if( pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE  )
							 return false;
							//else if ( Chance(35) ) return false;
						}
				}*/
		}
	}
	///ddd}

	return true;
}//end CanTraverse

bool AStarPathfinder::IsSomeoneInTheWay()
{
	// if contemplated tile is NOT final dest and someone there, disqualify route
	// when doing a reachable test, ignore all locations with people in them
	if (fPathAroundPeople && ( (CurrentNode != DestNode) || fCopyReachable) )
	{
		// ATE: ONLY cancel if they are moving.....
		UINT8 ubMerc = WhoIsThere2( CurrentNode, pSoldier->pathing.bLevel);
		if ( ubMerc < NOBODY && ubMerc != pSoldier->ubID )
		{
			// Check for movement....
			//if ( fTurnBased || ( (Menptr[ ubMerc ].sFinalDestination == Menptr[ ubMerc ].sGridNo) || (Menptr[ ubMerc ].fDelayedMovement) ) )
			//{
				return true;
			//}
			//	else
			//{
			//	nextCost += 50;
			//}
		}
	}
	return false;
}

#endif//end ifdef USE_ASTAR_PATHS

INT8 RandomSkipListLevel( void )
{
	INT8 bLevel = 1;

	while( Random( 4 ) == 0 && bLevel < iMaxSkipListLevel - 1)
	{
		bLevel++;
	}
	return( bLevel );
}

void ReconfigurePathAI( INT32 iNewMaxSkipListLevel, INT32 iNewMaxTrailTree, INT32 iNewMaxPathQ )
{
	// make sure the specified parameters are reasonable
	iNewMaxSkipListLevel = __max( iNewMaxSkipListLevel, ABSMAX_SKIPLIST_LEVEL );
	iNewMaxTrailTree = __max( iNewMaxTrailTree, ABSMAX_TRAIL_TREE );
	iNewMaxPathQ = __max( iNewMaxPathQ, ABSMAX_PATHQ );
	// assign them
	iMaxSkipListLevel = iNewMaxSkipListLevel;
	iMaxTrailTree = iNewMaxTrailTree;
	iMaxPathQ = iNewMaxPathQ;
	// relocate the head of the closed list to the end of the array portion being used
	pClosedHead = &(pathQ[QPOOLNDX]);
	memset( pClosedHead, 0, sizeof( path_t ) );
}

void RestorePathAIToDefaults( void )
{
	iMaxSkipListLevel = MAX_SKIPLIST_LEVEL;
	iMaxTrailTree = MAX_TRAIL_TREE;
	iMaxPathQ = MAX_PATHQ;
	// relocate the head of the closed list to the end of the array portion being used
	pClosedHead = &(pathQ[QPOOLNDX]);
	memset( pClosedHead, 0, sizeof( path_t ) );
}

//dnl ch50 071009
BOOLEAN InitPathAI(void)
{
	guiPathingData = (UINT32*)MemAlloc(MAX_PATH_DATA_LENGTH * sizeof(UINT32));
	guiPlottedPath = (UINT32*)MemAlloc(MAX_PATH_DATA_LENGTH * sizeof(UINT32));
	pathQ = (path_t*)MemAlloc(ABSMAX_PATHQ * sizeof(path_t));
	trailCost = (UINT16*)MemAlloc(WORLD_MAX * sizeof(TRAILCELLTYPE));
	trailCostUsed = (UINT8*)MemAlloc(WORLD_MAX);
	trailTree = (trail_t*)MemAlloc(ABSMAX_TRAIL_TREE * sizeof(trail_t));
	if(!guiPlottedPath || !guiPathingData || !pathQ || !trailCost || !trailCostUsed || !trailTree)
		return(FALSE);
	pQueueHead = &pathQ[QHEADNDX];
	pClosedHead = &pathQ[QPOOLNDX];
	memset(trailCostUsed, 0, WORLD_MAX);
	RestorePathAIToDefaults();
	return(TRUE);
}

void ShutDownPathAI(void)
{
	MemFree(guiPathingData);
	MemFree(guiPlottedPath);
	MemFree(pathQ);
	MemFree(trailCostUsed);
	MemFree(trailCost);
	MemFree(trailTree);
}

///////////////////////////////////////////////////////////////////////
//	FINDBESTPATH													/
////////////////////////////////////////////////////////////////////////
INT32 FindBestPath(SOLDIERTYPE *s , INT32 sDestination, INT8 bLevel, INT16 usMovementMode, INT8 bCopy, UINT8 fFlags )
{
	s->sPlotSrcGrid = s->sGridNo;

#ifdef USE_ASTAR_PATHS
//ddd
CHAR8 errorBuf[511]; UINT32 b,e;
b=GetJA2Clock();//return s->sGridNo+6;
	
	int retVal = ASTAR::AStarPathfinder::GetInstance().GetPath(s, sDestination, bLevel, usMovementMode, bCopy, fFlags);

	e=GetJA2Clock();sprintf(errorBuf, "timefind bestpath= %d",e-b );LiveMessage(errorBuf);

	if (retVal || TileIsOutOfBounds(sDestination)) {
		return retVal;
	}
	else {
		DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "ASTAR path failed!" ) );
	}

	//	if (TileIsOutOfBounds(sDestination))
	{
		return 0;
	}
#else
	//__try
	//{
	INT32 iDestination = sDestination, iOrigination;
	INT32 iCnt=-1, iStructIndex;
	INT32 iLoopStart = 0, iLoopEnd = 0;
	INT8	bLoopState = LOOPING_CLOCKWISE;
	//BOOLEAN fLoopForwards = FALSE;
	BOOLEAN	fCheckedBehind = FALSE;
	UINT8	ubMerc;
	INT32 iDestX,iDestY, iLocX, iLocY, dx, dy;
	INT32	newLoc,curLoc;
	//INT32 curY;
	INT32 curCost,newTotCost,nextCost;
	INT32	sCurPathNdx;
	INT32 prevCost;
	INT32 iWaterToWater;
	INT16 ubCurAPCost,ubAPCost;
	INT16 ubNewAPCost=0;
	#ifdef VEHICLE
		//BOOLEAN fTurnSlow = FALSE;
		//BOOLEAN fReverse = FALSE; // stuff for vehicles turning
		BOOLEAN fMultiTile, fVehicle;
		//INT32 iLastDir, iPrevToLastDir;
		//INT8 bVehicleCheckDir;
		//UINT16 adjLoc;
		STRUCTURE_FILE_REF * pStructureFileRef=NULL;
		UINT16							usAnimSurface;
		//INT32 iCnt2, iCnt3;
		BOOLEAN fVehicleIgnoreObstacles = FALSE;
	#endif

	INT32			iLastDir = 0;

	path_t *	pNewPtr;
	path_t *	pCurrPtr;

	path_t *	pUpdate[ABSMAX_SKIPLIST_LEVEL];
	path_t *	pCurr;
	path_t *	pNext;
	path_t *	pDel;
	UINT32		uiCost;
	INT32			iCurrLevel, iLoop;

	BOOLEAN		fHiddenStructVisible;				// Used for hidden struct visiblity
	UINT16		usOKToAddStructID=0;

	BOOLEAN		fCopyReachable;
	BOOLEAN		fCopyPathCosts;
	BOOLEAN		fVisitSpotsOnlyOnce;
	INT32			iOriginationX, iOriginationY, iX, iY;

	BOOLEAN		fTurnBased;
	BOOLEAN		fPathingForPlayer;
	INT32			iDoorGridNo=-1;
	BOOLEAN		fDoorIsObstacleIfClosed= 0; // if false, door is obstacle if it is open
	DOOR_STATUS *	pDoorStatus;
	DOOR *				pDoor;
	STRUCTURE *		pDoorStructure;
	BOOLEAN		fDoorIsOpen = FALSE;
	BOOLEAN		fNonFenceJumper;
	BOOLEAN		fNonSwimmer;
	BOOLEAN		fPathAroundPeople;
	BOOLEAN		fConsiderPersonAtDestAsObstacle;
	BOOLEAN		fGoingThroughDoor = FALSE; // for one tile
	BOOLEAN		fContinuousTurnNeeded;
	BOOLEAN		fCloseGoodEnough;
	UINT16	usMovementModeToUseForAPs;
	INT16			sClosePathLimit = 0;

#ifdef PATHAI_SKIPLIST_DEBUG
	CHAR8				zTempString[1000], zTS[50];
#endif

#ifdef PATHAI_VISIBLE_DEBUG
	UINT16		usCounter = 0;
#endif

	fVehicle = FALSE;
	iOriginationX = iOriginationY = 0;
	iOrigination = s->sGridNo;
/*
//dnl??? ch53 111009
if(!GridNoOnVisibleWorldTile(iOrigination))
{
	SendFmtMsg("shuuu=%d,%d", s->ubID, iOrigination);//dnl???
//bad!!!	return(0);
}
if(!GridNoOnVisibleWorldTile(iDestination))
{
//iDestination != (guiWorldCols*guiWorldRows)
	SendFmtMsg("blaaa=%d,%d", s->ubID, iDestination);//dnl???
//bad!!!	return(0);
}
*/
	if(TileIsOutOfBounds(sDestination) && sDestination != GRIDSIZE)//dnl ch50 071009 GRIDSIZE is use when want to travel all over the map, one of useage is when seeking suitable place for edge points
	{
		return(0); // There is no path to nowhere, because everywhere you can go is somewhere!
	}
	else if(sDestination < GRIDSIZE && !GridNoOnVisibleWorldTile(iDestination))//dnl ch53 111009
	{
		return(0);
	}

	if ( TileIsOutOfBounds( iOrigination ) )
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!	Trying to calculate path from off-world gridno %d to %d", iOrigination, sDestination );
		#endif
		return( 0 );
	}
	else if (!GridNoOnVisibleWorldTile( iOrigination ))
	{
		#ifdef JA2BETAVERSION
			if(!is_networked)ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!	Trying to calculate path from non-visible gridno %d to %d", iOrigination, sDestination );
		#endif
		return( 0 );
	}
	else if (s->pathing.bLevel != bLevel)
	{
		// pathing to a different level... bzzzt!
		return( 0 );
	}

	if ( gubGlobalPathFlags )
	{
		fFlags |= gubGlobalPathFlags;
	}

	fTurnBased = ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) );

	fPathingForPlayer = ( (s->bTeam == gbPlayerNum) && (!gTacticalStatus.fAutoBandageMode) && !(s->flags.uiStatusFlags & SOLDIER_PCUNDERAICONTROL) );
	fNonFenceJumper = !( IS_MERC_BODY_TYPE( s ) ) || (UsingNewInventorySystem() == true && s->inv[BPACKPOCKPOS].exists() == true
		&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)s->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[s->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb)
		&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[s->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)));//Moa: added backpack check

	// Flugente: nonswimmers are those who are not mercs and not boats
	fNonSwimmer = !(IS_MERC_BODY_TYPE( s ) );

	if ( fNonSwimmer )
	{
		if ( Water( sDestination, bLevel ) )
		{
			return( 0 );
		}
	}
	fPathAroundPeople = ( (fFlags & PATH_THROUGH_PEOPLE) == 0 );

	// anv: vehicles can drive through people and structures if shift is pressed 
	// (or rather route through can be planned, actual driving through is dependent on SOLDIER_RAM_THROUGH_OBSTACLES flag set in Handle UI)
	if ( _KeyDown( SHIFT ) && fPathingForPlayer && ( s->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	{
		fVehicleIgnoreObstacles = TRUE;
	}
	// AI can't press shift, let's assume only tanks ram through (we wouldn't want AI cars to get destroyed kamikazing through walls, no matter how funny that would be)
	else if ( !fPathingForPlayer && ARMED_VEHICLE( s ) )
	{
		fVehicleIgnoreObstacles = TRUE;
	}
	if( fVehicleIgnoreObstacles && 
		((gGameExternalOptions.fAllowCarsDrivingOverPeople && !ARMED_VEHICLE( s )) ||
		(gGameExternalOptions.fAllowTanksDrivingOverPeople && ARMED_VEHICLE( s ))) )
	{
		fPathAroundPeople = FALSE;
	}

	fCloseGoodEnough = ( (fFlags & PATH_CLOSE_GOOD_ENOUGH) != 0);
	if ( fCloseGoodEnough )
	{
		sClosePathLimit = __min( PythSpacesAway( s->sGridNo, sDestination ) - 1,  PATH_CLOSE_RADIUS );
		if ( sClosePathLimit <= 0 )
		{
			return( 0 );
		}
	}

	fConsiderPersonAtDestAsObstacle = (BOOLEAN)( fPathingForPlayer && fPathAroundPeople && !(fFlags & PATH_IGNORE_PERSON_AT_DEST) );

	if (bCopy >= COPYREACHABLE)
	{
		fCopyReachable = TRUE;
		fCopyPathCosts = (bCopy == COPYREACHABLE_AND_APS);
		fVisitSpotsOnlyOnce = (bCopy == COPYREACHABLE);
		// make sure we aren't trying to copy path costs for an area greater than the AI array...
		if (fCopyPathCosts && gubNPCDistLimit > AI_PATHCOST_RADIUS)
		{
			// oy!!!! dis no supposed to happen!
			gubNPCDistLimit = AI_PATHCOST_RADIUS;
		}
	}
	else
	{
		fCopyReachable = FALSE;
		fCopyPathCosts = FALSE;
		fVisitSpotsOnlyOnce = FALSE;
	}

	gubNPCPathCount++;

	if (gubGlobalPathCount == 255)
	{
		// reset arrays!
		memset( trailCostUsed, 0, WORLD_MAX );
		gubGlobalPathCount = 1;
	}
	else
	{
		gubGlobalPathCount++;
	}

	// only allow nowhere destination if distance limit set	
	if (TileIsOutOfBounds(sDestination))
	{
		/*
		if (gubNPCDistLimit == 0)
		{
			return( FALSE );
		}
		*/
	}
	else
	{
		// the very first thing to do is make sure the destination tile is reachable
		if (!NewOKDestination( s, sDestination, fConsiderPersonAtDestAsObstacle, bLevel ))
		{
			gubNPCAPBudget = 0;
			gubNPCDistLimit = 0;
			return( FALSE );
		}

		if (sDestination == s->sGridNo)
		{
			return( FALSE );
		}
	}

	if (gubNPCAPBudget)
	{
		ubAPCost = MinAPsToStartMovement( s, usMovementMode );
		if (ubAPCost > gubNPCAPBudget)
		{
			gubNPCAPBudget = 0;
			gubNPCDistLimit = 0;
			return( 0 );
		}
		else
		{
			gubNPCAPBudget = gubNPCAPBudget - ubAPCost;
		}
	}

#ifdef COUNT_PATHS
	guiTotalPathChecks++;
#endif

#ifdef VEHICLE

	fMultiTile = ((s->flags.uiStatusFlags & SOLDIER_MULTITILE) != 0);
	if (fMultiTile)
	{
		// Get animation surface...
		// Chris_C... change this to use parameter.....
		usAnimSurface = DetermineSoldierAnimationSurface( s, usMovementMode );
		// Get structure ref...
		pStructureFileRef = GetAnimationStructureRef( s->ubID, usAnimSurface, usMovementMode );

		if ( pStructureFileRef )
		{
			fVehicle = ( (s->flags.uiStatusFlags & SOLDIER_VEHICLE) != 0 );

			fContinuousTurnNeeded = ( ( s->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE) ) != 0 );

			/*
			if (fVehicle && s->bReverse)
			{
				fReverse = TRUE;
			}
			*/
			/*
			if (fVehicle || s->ubBodyType == COW || s->ubBodyType == BLOODCAT) // or a vehicle
			{
				fTurnSlow = TRUE;
			}
			*/
			if ( gfEstimatePath )
			{
				usOKToAddStructID = IGNORE_PEOPLE_STRUCTURE_ID;
			}
			else if ( s->pLevelNode != NULL && s->pLevelNode->pStructureData != NULL )
			{
				usOKToAddStructID = s->pLevelNode->pStructureData->usStructureID;
			}
			else
			{
				usOKToAddStructID = INVALID_STRUCTURE_ID;
			}
			if( fVehicleIgnoreObstacles )
			{
				usOKToAddStructID = VEHICLE_IGNORE_OBSTACLES_STRUCTURE_ID;
			}

		}
		else
		{
			// turn off multitile pathing
			fMultiTile = FALSE;
			fContinuousTurnNeeded = FALSE;
		}

	}
	else
	{
		fContinuousTurnNeeded = FALSE;
	}

#endif

	if (!fContinuousTurnNeeded)
	{
		iLoopStart = 0;
		iLoopEnd = 0;
		bLoopState = LOOPING_CLOCKWISE;
	}

	ubCurAPCost = 0;
	queRequests = 2;

	//initialize the path data structures
	memset( pathQ, 0, iMaxPathQ * sizeof( path_t ) );
	memset( trailTree, 0, iMaxTrailTree * sizeof( trail_t ) );

#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		memset( gsCoverValue, 0x7F, sizeof( INT16 ) * WORLD_MAX );
	}
#endif

	bSkipListLevel = 1;
	iSkipListSize = 0;
	iClosedListSize = 0;

	trailTreeNdx=0;

	//set up common info
	if (fCopyPathCosts)
	{
		iOriginationY = (iOrigination / WORLD_COLS);
		iOriginationX = (iOrigination % WORLD_COLS);
	}

	iDestY = (iDestination / WORLD_COLS);
	iDestX = (iDestination % WORLD_COLS);

	// if origin and dest is water, then user wants to stay in water!
	// so, check and set waterToWater flag accordingly	
	if (TileIsOutOfBounds(iDestination))
	{
		iWaterToWater = 0;
	}
	else
	{
		if (ISWATER(gubWorldMovementCosts[iOrigination][0][bLevel]) && ISWATER(gubWorldMovementCosts[iDestination][0][bLevel]))
			iWaterToWater = 1;
		else
			iWaterToWater = 0;
	}

	//setup Q and first path record

	SETLOC( *pQueueHead, iOrigination );
	pQueueHead->usCostSoFar	= MAXCOST;
	pQueueHead->bLevel			= (INT8) (iMaxSkipListLevel - 1);

	pClosedHead->pNext[0] = pClosedHead;
	pClosedHead->pNext[1] = pClosedHead;

	//setup first path record
	iLocY = iOrigination / WORLD_COLS;
	iLocX = iOrigination % WORLD_COLS;

	SETLOC( pathQ[1], iOrigination );
	pathQ[1].sPathNdx						= 0;
	pathQ[1].usCostSoFar				= 0;
	if ( fCopyReachable )
	{
		pathQ[1].usCostToGo					= 100;
	}
	else
	{
		pathQ[1].usCostToGo					= REMAININGCOST( &(pathQ[1]) );
	}
	pathQ[1].usTotalCost				= pathQ[1].usCostSoFar + pathQ[1].usCostToGo;
	pathQ[1].ubLegDistance			= LEGDISTANCE( iLocX, iLocY, iDestX, iDestY );
	pathQ[1].bLevel							= 1;
	pQueueHead->pNext[0] = &( pathQ[1] );
	iSkipListSize++;

	trailTreeNdx						=	0;
	trailCost[iOrigination]	=	0;
	pCurrPtr											= pQueueHead->pNext[0];
	pCurrPtr->sPathNdx						= trailTreeNdx;
	trailTreeNdx++;

	// sevenfm: moved calculations out of loop
	UINT16 usBackpackPenalty = 0;

	if (UsingNewInventorySystem() && gGameExternalOptions.fBackPackWeightLowersAP)
	{
		INT8 bSlot = FindBackpackOnSoldier(s);
		if (bSlot != ITEM_NOT_FOUND)
		{
			UINT16 usBPPenalty = APBPConstants[AP_MODIFIER_PACK];
			if (bSlot == BPACKPOCKPOS) //Backpack carried on back
			{
				OBJECTTYPE * pObj = &(s->inv[BPACKPOCKPOS]);
				UINT16 usBackPackWeight = CalculateObjectWeight(pObj);
				// CalculateObjectWeight checks for active LBE gear. Unfortunately our backpack is not active since we are carrying it.
				// Sounds not intuitive at all, active means the LBE caries items (marked with blue *), but when put on the LBE adittional slots of our soldier
				// are activated where something can be carried. So we have to add the weights of those slots as well.
				std::vector<INT8> vbLBESlots;
				GetLBESlots(BPACKPOCKPOS, vbLBESlots);
				for (UINT8 i = 0; i < vbLBESlots.size(); i++)
				{
					pObj = &(s->inv[vbLBESlots[i]]);
					usBackPackWeight += CalculateObjectWeight(pObj);
				}
				usBPPenalty = min((usBackPackWeight / 50), usBPPenalty); //1 AP penalty for each 5kg of weight up to the penalty defined by AP_MODIFIER_PACK (default = 4)
			}
			else //Backpack carried not on back (maybe somewhere inside another LBE or in Hand?)
			{
				//apply full penalty
			}
			usBackpackPenalty = usBPPenalty;
		}
	}

	do
	{
		//remove the first and best path so far from the que
		pCurrPtr				= pQueueHead->pNext[0];
		curLoc		= pCurrPtr->iLocation;
		curCost		= pCurrPtr->usCostSoFar;
		sCurPathNdx = pCurrPtr->sPathNdx;

		// remember the cost used to get here...
		prevCost = gubWorldMovementCosts[ trailTree[ sCurPathNdx ].sGridNo ][ trailTree[ sCurPathNdx ].stepDir ][ bLevel ];

#if defined( PATHAI_VISIBLE_DEBUG )
		if (gfDisplayCoverValues && gfDrawPathPoints)
		{
			if (gsCoverValue[ curLoc ] > 0)
			{
				gsCoverValue[ curLoc ] *= -1;
			}
		}
#endif

#ifdef VEHICLE
		/*
		if (fTurnSlow)
		{
			if (pCurrPtr->pNext[0] == 0)
			{
				if (fReverse)
				{
					iLastDir = gOppositeDirection[s->ubDirection];
				}
				else
				{
					iLastDir = s->ubDirection;
				}
				// start prev-to-last dir at same as current (could cause a problem)
				iPrevToLastDir = iLastDir;
			}
			else
			{
				iPrevToLastDir = trailTree[trailTree[pCurrPtr->sPathNdx].nextLink].dirDelta;
				iLastDir = trailTree[pCurrPtr->sPathNdx].dirDelta;
			}

		}
		*/
#endif

		if (gubNPCAPBudget)
		{
			ubCurAPCost = pCurrPtr->ubTotalAPCost;
		}
		if (fCopyReachable && prevCost != TRAVELCOST_FENCE)
		{
			gpWorldLevelData[curLoc].uiFlags |= MAPELEMENT_REACHABLE;
			if (gubBuildingInfoToSet > 0)
			{
				gubBuildingInfo[ curLoc ] = gubBuildingInfoToSet;
			}
		}

		DELQUENODE( pCurrPtr );

		if ( trailCostUsed[curLoc] == gubGlobalPathCount && trailCost[curLoc] < curCost)
			goto NEXTDIR;

		//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "PATHAI %d", curLoc ) );

		// 0verhaul:	Cannot change direction over a fence!
		if (prevCost == TRAVELCOST_FENCE)
		{
			iLoopStart = trailTree[pCurrPtr->sPathNdx].stepDir;
			if (bLoopState == LOOPING_CLOCKWISE) {
				iLoopEnd = gOneCCDirection[ iLoopStart];
			} else {
				iLoopEnd = gOneCDirection[ iLoopStart];
			}
		}
		else
		{
			iLoopStart = iLoopEnd = 0;
		}

		if (fContinuousTurnNeeded)
		{
			if (trailTreeNdx < 2)
			{
				iLastDir = s->ubDirection;
			}
			else if ( trailTree[pCurrPtr->sPathNdx].fFlags & STEP_BACKWARDS )
			{
				iLastDir = gOppositeDirection[ trailTree[pCurrPtr->sPathNdx].stepDir ];
			}
			else
			{
				iLastDir = trailTree[pCurrPtr->sPathNdx].stepDir;
			}
			iLoopStart = iLastDir;
			iLoopEnd = iLastDir;
			bLoopState = LOOPING_CLOCKWISE;
			fCheckedBehind = FALSE;
		}

		//contemplate a new path in each direction
		//for ( iCnt = iLoopStart; iCnt != iLoopEnd; iCnt = (iCnt + iLoopIncrement) % MAXDIR )
		for ( iCnt = iLoopStart; ; )
		{
#ifdef VEHICLE
			/*
			if (fTurnSlow)
			{
				if (iLastDir == iPrevToLastDir)
				{
					if ( iCnt != iLastDir && iCnt != gOneCDirection[ iLastDir ] && iCnt != gOneCCDirection[ iLastDir ])
					{
						goto NEXTDIR;
					}
				}
				else
				{
					if ( iCnt != iLastDir )
					{
						goto NEXTDIR;
					}
				}
			}
			*/

			if ( bLoopState == LOOPING_REVERSE )
			{
				iStructIndex = gOppositeDirection[ gOneCDirection[ iCnt ] ];
			}
			else
			{
				iStructIndex = gOneCDirection[ iCnt ];
			}

			if (fMultiTile)
			{
				if ( fContinuousTurnNeeded )
				{
					if ( iCnt != iLastDir )
					{
						if ( !OkayToAddStructureToWorld( curLoc, bLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID, FALSE, s->ubID ) )
						{
							// we have to abort this loop and possibly reset the loop conditions to
							// search in the other direction (if we haven't already done the other dir)
							if (bLoopState == LOOPING_CLOCKWISE)
							{
								iLoopStart = iLastDir;
								iLoopEnd = iCnt;
								bLoopState = LOOPING_COUNTERCLOCKWISE; // backwards
								// when we go to the bottom of the loop, iLoopIncrement will be added to iCnt
								// which is good since it avoids duplication of effort
								iCnt = iLoopStart;
								goto NEXTDIR;
							}
							else if ( bLoopState == LOOPING_COUNTERCLOCKWISE && !fCheckedBehind )
							{
								// check rear dir
								bLoopState = LOOPING_REVERSE;

								// NB we're stuck with adding 1 to the loop counter down below so configure to accomodate...
								//iLoopStart = (iLastDir + (MAXDIR / 2) - 1) % MAXDIR;
								iLoopStart = gOppositeDirection[ gOneCCDirection[ iLastDir ] ];
								iLoopEnd = (iLoopStart + 2) % MAXDIR;
								iCnt = iLoopStart;
								fCheckedBehind = TRUE;
								goto NEXTDIR;
							}
							else
							{
								// done
								goto ENDOFLOOP;
							}

						}
					}
				}
				else if ( pStructureFileRef )
				{
					// check to make sure it's okay for us to turn to the new direction in our current tile
					if (!OkayToAddStructureToWorld( curLoc, bLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID ) )
					{
						goto NEXTDIR;
					}
				}
			}

#endif

			newLoc = curLoc + dirDelta[iCnt];

			if (newLoc < 0 || newLoc >= WORLD_MAX)
			{
				goto NEXTDIR;
			}

			if ( fVisitSpotsOnlyOnce && trailCostUsed[newLoc] == gubGlobalPathCount )
			{
				// on a "reachable" test, never revisit locations!
				goto NEXTDIR;
			}


			//if (gpWorldLevelData[newLoc].sHeight != ubLevel)
			//ATE: Movement onto cliffs? Check vs the soldier's gridno height
			// CJC: PREVIOUS LOCATION's height
			if ( gpWorldLevelData[ newLoc ].sHeight != gpWorldLevelData[ curLoc ].sHeight )
			{
				goto NEXTDIR;
			}

			if (gubNPCDistLimit)
			{
				if ( gfNPCCircularDistLimit )
				{
					if (PythSpacesAway( iOrigination, newLoc) > gubNPCDistLimit)
					{
						goto NEXTDIR;
					}
				}
				else
				{
					if (SpacesAway( iOrigination, newLoc) > gubNPCDistLimit)
					{
						goto NEXTDIR;
					}
				}
			}

			// AI check for mines
			if ( gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT && s->bSide != 0)
			{
				goto NEXTDIR;
			}

			// sevenfm: skip deep water if not in deep water already
			if (!(s->flags.uiStatusFlags & SOLDIER_PC) &&
				s->ubProfile == NO_PROFILE &&
				s->aiData.bOrders != SEEKENEMY &&
				DeepWater(newLoc, bLevel) &&
				!DeepWater(s->sGridNo, bLevel) &&
				!FindNotDeepWaterNearby(newLoc, bLevel))
			{
				goto NEXTDIR;
			}

			// sevenfm: skip gas if not in gas already
			if ((!(s->flags.uiStatusFlags & SOLDIER_PC) || gTacticalStatus.fAutoBandageMode || s->flags.uiStatusFlags & SOLDIER_PCUNDERAICONTROL) &&
				InGasSpot(s, newLoc, bLevel) &&
				!InGasSpot(s, s->sGridNo, bLevel))
			{
				goto NEXTDIR;
			}

			// WANNE: Known mines (for enemy or player) do not explode - BEGIN
			if ( gpWorldLevelData[newLoc].uiFlags & (MAPELEMENT_ENEMY_MINE_PRESENT | MAPELEMENT_PLAYER_MINE_PRESENT) )
			{
				if (s->bSide == 0)
				{
					// For our team, skip a location with a known mines unless it is the end of our
					// path; for others on our side, skip such locations completely;
					if (s->bTeam != gbPlayerNum || newLoc != iDestination)
					{
						if (gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT)
						{
							goto NEXTDIR;
						}
					}
				}
				else
				{
					// For the enemy, always skip known mines
					if (gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT)
					{
						goto NEXTDIR;
					}
				}
			}

			//how much is admission to the next tile
			if ( gfPathAroundObstacles && !fVehicleIgnoreObstacles )
			{
				nextCost = gubWorldMovementCosts[ newLoc ][ iCnt ][ bLevel ];

				//ATE:	Check for differences from reality
				// Is next cost an obstcale
				if ( nextCost == TRAVELCOST_HIDDENOBSTACLE )
				{
					if ( fPathingForPlayer )
					{
						// Is this obstcale a hidden tile that has not been revealed yet?
						if( DoesGridNoContainHiddenStruct( newLoc, &fHiddenStructVisible ) )
						{
							// Are we not visible, if so use terrain costs!
							if ( !fHiddenStructVisible )
							{
								// Set cost of terrain!
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
						}
					}
				}
				else if ( nextCost == TRAVELCOST_NOT_STANDING )
				{
					// for path plotting purposes, use the terrain value
					nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
				}
				else if ( nextCost == TRAVELCOST_EXITGRID )
				{
					if (gfPlotPathToExitGrid)
					{
						// replace with terrain cost so that we can plot path, otherwise is obstacle
						nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
					}
				}
				else if ( nextCost == TRAVELCOST_FENCE && fNonFenceJumper )
				{
					goto NEXTDIR;
				}
				else if ( IS_TRAVELCOST_DOOR( nextCost ) )
				{

					// don't let anyone path diagonally through doors!
					if (iCnt & 1)
					{
						goto NEXTDIR;
					}

					switch( nextCost )
					{
						case TRAVELCOST_DOOR_CLOSED_HERE:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc;
							break;
						case TRAVELCOST_DOOR_CLOSED_N:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_CLOSED_W:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_HERE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc;
							break;
						case TRAVELCOST_DOOR_OPEN_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHEAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_E:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ EAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_SE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHEAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_S:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_SW:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHWEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_N_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NE_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHEAST ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_W_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ WEST ] + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_SW_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHWEST ] + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ] + dirDelta[ WEST ];
							break;
						default:
							break;
					}

					if ( fPathingForPlayer && gpWorldLevelData[ iDoorGridNo ].ubExtFlags[0] & MAPELEMENT_EXT_DOOR_STATUS_PRESENT )
					{
						// check door status
						pDoorStatus = GetDoorStatus( iDoorGridNo );
						if (pDoorStatus)
						{
							fDoorIsOpen = (pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN) != 0;
						}
						else
						{
							// door destroyed?
							nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
						}
					}
					else
					{
						// check door structure
						pDoorStructure = FindStructure( iDoorGridNo, STRUCTURE_ANYDOOR );
						if (pDoorStructure)
						{
							fDoorIsOpen = (pDoorStructure->fFlags & STRUCTURE_OPEN) != 0;
						}
						else
						{
							// door destroyed?
							nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
						}
					}

					// now determine movement cost... if it hasn't been changed already
					if ( IS_TRAVELCOST_DOOR( nextCost ) )
					{

						if (fDoorIsOpen)
						{
							if (fDoorIsObstacleIfClosed)
							{
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
							else
							{
								nextCost = TRAVELCOST_OBSTACLE;
							}
						}
						else
						{
							if (fDoorIsObstacleIfClosed)
							{
								// door is closed and this should be an obstacle, EXCEPT if we are calculating
								// a path for an enemy or NPC with keys
								if ( fPathingForPlayer || ( s && (s->flags.uiStatusFlags & SOLDIER_MONSTER || s->flags.uiStatusFlags & SOLDIER_ANIMAL) ) )
								{
									nextCost = TRAVELCOST_OBSTACLE;
								}
								else
								{
									// have to check if door is locked and NPC does not have keys!
									pDoor = FindDoorInfoAtGridNo( iDoorGridNo );
									if (pDoor)
									{
										if (!pDoor->fLocked || s->flags.bHasKeys)
										{
											// add to AP cost
											if (gubNPCAPBudget)
											{
												fGoingThroughDoor = TRUE;
											}
											nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
										}
										else
										{
											nextCost = TRAVELCOST_OBSTACLE;
										}
									}
									else
									{
										nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
									}
								}
							}
							else
							{
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
						}
					}
				}
				else if ( (nextCost == TRAVELCOST_SHORE || nextCost == TRAVELCOST_KNEEDEEP || nextCost == TRAVELCOST_DEEPWATER ) && fNonSwimmer )
				{
					// creatures and animals can't go in water
					nextCost = TRAVELCOST_OBSTACLE;
				}


				// Apr. '96 - moved up be ahead of AP_Budget stuff
				if ( ( nextCost >= NOPASS) ) // || ( nextCost == TRAVELCOST_DOOR ) )
						goto NEXTDIR;

			}
			else if( fVehicleIgnoreObstacles )
			{
				nextCost = TRAVELCOST_FLAT;
			}
			else
			{
				nextCost = TRAVELCOST_FLAT;
			}

			if ( newLoc > GRIDSIZE )
			{
				// WHAT THE??? hack.
				goto NEXTDIR;
			}

			// if contemplated tile is NOT final dest and someone there, disqualify route
			// when doing a reachable test, ignore all locations with people in them
			if (fPathAroundPeople && ( (newLoc != iDestination) || fCopyReachable) )
			{
				// ATE: ONLY cancel if they are moving.....
				ubMerc = WhoIsThere2( newLoc, s->pathing.bLevel);

				// sevenfm: for player mercs, ignore invisible opponents
				if (ubMerc < TOTAL_SOLDIERS && ubMerc != s->ubID && 
					(!(s->flags.uiStatusFlags & SOLDIER_PC) || MercPtrs[ubMerc]->bSide == s->bSide || MercPtrs[ubMerc]->aiData.bNeutral || MercPtrs[ubMerc]->bVisible >= 0 || SoldierToSoldierLineOfSightTest(s, MercPtrs[ubMerc], TRUE, CALC_FROM_ALL_DIRS)))
				//if ( ubMerc < TOTAL_SOLDIERS && ubMerc != s->ubID )
				{
					goto NEXTDIR;
				}
			}

#ifdef VEHICLE
			if (fMultiTile)
			{
				// vehicle test for obstacles: prevent movement to next tile if
				// a tile covered by the vehicle in that position & direction
				// has an obstacle in it

				// because of the order in which animations are stored (dir 7 first,
				// then 0 1 2 3 4 5 6), we must subtract 1 from the direction
				// ATE: Send in our existing structure ID so it's ignored!

				if (!OkayToAddStructureToWorld( newLoc, bLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID, FALSE, s->ubID ) )
				{
					goto NEXTDIR;
				}

				/*
				// vehicles aren't moving any more....
				if (fVehicle)
				{
					// transmogrify pathing costs for vehicles!
					switch(nextCost)
					{
						case TRAVELCOST_THICK		:	nextCost = TRAVELCOST_GRASS;
																			break;
						case TRAVELCOST_SHORE		:
						case TRAVELCOST_KNEEDEEP:
						case TRAVELCOST_DEEPWATER:
//						case TRAVELCOST_VEINEND	:
//						case TRAVELCOST_VEINMID	:
						//case TRAVELCOST_DOOR		:
						case TRAVELCOST_FENCE		:	nextCost = TRAVELCOST_OBSTACLE;
																			break;

						default									:	break;
					}
				}
				*/
			}
#endif

			// NEW Apr 21 by Ian: abort if cost exceeds budget
			if (gubNPCAPBudget)
			{
				switch(nextCost)
				{
					case TRAVELCOST_NONE		: ubAPCost = 0;
																		break;

					case TRAVELCOST_DIRTROAD:
					case TRAVELCOST_FLAT		: ubAPCost = APBPConstants[AP_MOVEMENT_FLAT];
																		break;
					//case TRAVELCOST_BUMPY	:
					case TRAVELCOST_GRASS		: ubAPCost = APBPConstants[AP_MOVEMENT_GRASS];
																		break;
					case TRAVELCOST_THICK		:	ubAPCost = APBPConstants[AP_MOVEMENT_BUSH];
																		break;
					case TRAVELCOST_DEBRIS	: ubAPCost = APBPConstants[AP_MOVEMENT_RUBBLE];
																		break;
					case TRAVELCOST_SHORE		: ubAPCost = APBPConstants[AP_MOVEMENT_SHORE]; // wading shallow water
																		break;
					case TRAVELCOST_KNEEDEEP:	ubAPCost = APBPConstants[AP_MOVEMENT_LAKE]; // wading waist/chest deep - very slow
																		break;

					case TRAVELCOST_DEEPWATER:ubAPCost = APBPConstants[AP_MOVEMENT_OCEAN]; // can swim, so it's faster than wading
																		break;
					//case TRAVELCOST_VEINEND	:
					//case TRAVELCOST_VEINMID	: ubAPCost = APBPConstants[AP_MOVEMENT_FLAT];
					//													break;

					//case TRAVELCOST_DOOR		:	ubAPCost = APBPConstants[AP_MOVEMENT_FLAT];
					//													break;

					case TRAVELCOST_FENCE		: 
						// SANDRO - slightly changed
						if((UsingNewInventorySystem() == true) && FindBackpackOnSoldier( s ) != ITEM_NOT_FOUND )
							ubAPCost = GetAPsToJumpFence( s, TRUE ); 
						else
							ubAPCost = GetAPsToJumpFence( s, FALSE );
			
/*
			if ( sSwitchValue == TRAVELCOST_FENCE )
			{
				sPoints += sTileCost;

				// If we are changeing stance ( either before or after getting there....
				// We need to reflect that...
				switch(usMovementMode)
				{
					case RUNNING:
					case WALKING :

						// Add here cost to go from crouch to stand AFTER fence hop....
						// Since it's AFTER.. make sure we will be moving after jump...
						if ( ( iCnt + 2 ) < iLastGrid )
						{
							sPoints += APBPConstants[AP_CROUCH];
						}
						break;

					case SWATTING:

						// Add cost to stand once there BEFORE....
						sPoints += AP_CROUCH;
						break;

					case CRAWLING:

						// Can't do it here.....
						break;

				}
			}

*/

																		break;

					case TRAVELCOST_OBSTACLE:
					default									:	goto NEXTDIR;	// Cost too much to be considered!
																		break;
				}


			// don't make the mistake of adding directly to
			// ubCurAPCost, that must be preserved for remaining dirs!
				if (iCnt & 1)
				{
					//ubAPCost++;
					//ubAPCost = gubDiagCost[ubAPCost];
					ubAPCost = (ubAPCost * 14) / 10;
				}

		usMovementModeToUseForAPs = usMovementMode;

		// ATE: if water, force to be walking always!
		if ( nextCost == TRAVELCOST_SHORE || nextCost == TRAVELCOST_KNEEDEEP || nextCost == TRAVELCOST_DEEPWATER )
		{
			usMovementModeToUseForAPs = WALKING;
		}

				// adjust AP cost for movement mode
		// CHRISL: Adjusted system to use different move costs while wearing a backpack
			// SANDRO - slightly changed this to add backpack modifier after Athletics trait check
				switch( usMovementModeToUseForAPs )
				{
					case RUNNING:
					case ADULTMONSTER_WALKING:
						// save on casting
						//ubAPCost = (UINT8)(ubAPCost * 10 / ( (UINT8) (RUNDIVISOR * 10)));
						ubAPCost += APBPConstants[AP_MODIFIER_RUN];
						//ubAPCost = (INT16)(DOUBLE)( (sTileCost / RUNDIVISOR) );	break;
						break;
					case WALKING_WEAPON_RDY:
					case WALKING_DUAL_RDY:
						ubAPCost += APBPConstants[AP_MODIFIER_WALK] + APBPConstants[AP_MODIFIER_READY];	//WALKCOST);
						break;
					case WALKING_ALTERNATIVE_RDY:
						ubAPCost += APBPConstants[AP_MODIFIER_WALK];	//WALKCOST);
						break;
					case WALKING:
						ubAPCost += APBPConstants[AP_MODIFIER_WALK];	//WALKCOST);
						if (!(s->MercInWater()) && ( (gAnimControl[ s->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ s->usAnimState ].uiFlags & ANIM_FIRE ) ))
						{
							ubAPCost += APBPConstants[AP_MODIFIER_READY];	//WALKCOST);
						}
						break;
					case ROBOT_WALK:
						ubAPCost += APBPConstants[AP_MODIFIER_WALK];	//WALKCOST);
						break;
					case SWATTING:
					case SIDE_STEP_CROUCH_RIFLE:
					case SIDE_STEP_CROUCH_PISTOL:
					case SIDE_STEP_CROUCH_DUAL:
					case CROUCHEDMOVE_RIFLE_READY:
					case CROUCHEDMOVE_PISTOL_READY:
					case CROUCHEDMOVE_DUAL_READY:
						ubAPCost += APBPConstants[AP_MODIFIER_SWAT];	//SWATCOST);
						break;
					case CRAWLING:
						ubAPCost += APBPConstants[AP_MODIFIER_CRAWL];	//CRAWLCOST);
						break;
					default:
						ubAPCost += APBPConstants[AP_MODIFIER_WALK];
						break;
				}

				///////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - STOMP traits - Athletics trait decreases movement cost
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( s, ATHLETICS_NT ))
				{
					ubAPCost = max(1, (INT16)(ubAPCost * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f + 0.5f));
				}

				// Moa: scuba fins and swimming background
				if ( s->inv[LEGPOS].exists() && HasItemFlag( s->inv[LEGPOS].usItem, SCUBA_FINS ) )
				{
					if ( TERRAIN_IS_HIGH_WATER( gpWorldLevelData[ newLoc ].ubTerrainID) )
					{
						ubAPCost /= 2;
					}
					else
					{
						ubAPCost *= 2;
					}
				}
				if ( TERRAIN_IS_HIGH_WATER( gpWorldLevelData[ newLoc ].ubTerrainID ) )
					ubAPCost = (ubAPCost * (100 + s->GetBackgroundValue(BG_SWIMMING))) / 100;

				// Flugente: riot shields lower movement speed
				if ( s->IsRiotShieldEquipped( ) )
				{
					ubAPCost *= gItemSettings.fShieldMovementAPCostModifier;
				}

				// Flugente: dragging someone
				if ( s->IsDragging( ) )
				{
					ubAPCost *= gItemSettings.fDragAPCostModifier;
				}

				// SANDRO - moved backpack check to here
				// Moa: backpack penalty
				ubAPCost += usBackpackPenalty;

				if (nextCost == TRAVELCOST_FENCE)
				{
					switch( usMovementModeToUseForAPs )
					{
						case RUNNING:
						case WALKING :
						case WALKING_WEAPON_RDY:
						case WALKING_DUAL_RDY:
						case WALKING_ALTERNATIVE_RDY :
							// Here pessimistically assume the path will continue after hopping the fence
							ubAPCost += GetAPsCrouch( s, TRUE );
							break;

						case SWATTING:
						case CROUCHEDMOVE_RIFLE_READY:
						case CROUCHEDMOVE_PISTOL_READY:
						case CROUCHEDMOVE_DUAL_READY:

							// Add cost to stand once there BEFORE jumping....
							ubAPCost += GetAPsCrouch( s, TRUE );
							break;

						case CRAWLING:

							// Add cost to stand up before jump and go prone after jump
							ubAPCost += GetAPsCrouch(s, TRUE) + ( 2 * GetAPsProne(s, TRUE) );
							// Can't do it here.....
							//goto NEXTDIR;
							break;
					}
				}
				else if (nextCost == TRAVELCOST_NOT_STANDING)
				{
					switch(usMovementModeToUseForAPs)
					{
						case RUNNING:
						case WALKING :
						case WALKING_WEAPON_RDY:
						case WALKING_DUAL_RDY:
						case WALKING_ALTERNATIVE_RDY :
							// charge crouch APs for ducking head!
							ubAPCost += GetAPsCrouch( s, TRUE );
							break;

						default:
							break;
					}
				}
				else if (fGoingThroughDoor)
				{
					ubAPCost += GetAPsToOpenDoor( s );
					fGoingThroughDoor = FALSE;
				}


			ubNewAPCost = ubCurAPCost + ubAPCost;


			if (ubNewAPCost > gubNPCAPBudget)
			 goto NEXTDIR;

			}

			//ATE: Uncommented out for doors, if we are at a door but not dest, continue!
		//	if ( nextCost == TRAVELCOST_DOOR	) //&& newLoc != iDestination)
		//		goto NEXTDIR;
/*
			// FOLLOWING SECTION COMMENTED OUT ON MARCH 7/97 BY IC

			if (nextCost == SECRETCOST && !s->human)
				goto NEXTDIR;

			if (prevCost==VEINMIDCOST)
				if (!ISVEIN(nextCost))
					goto NEXTDIR;
			//veining check
			if (nextCost==VEINMIDCOST)
				if (!ISVEIN(prevCost))
					goto NEXTDIR;

			if (nextCost==VEINMIDCOST)
			//if (ISVEIN(nextCost))
				nextCost=VEINCOST;
			else
			if (nextCost==VEINENDCOST)
				if (Grid[newLoc].land < LAKE1)
				 nextCost = VEINCOST;
				else
				 nextCost = OCEANCOST+(10*PATHFACTOR);

	*/

			if ( fCloseGoodEnough )
			{
				if ( PythSpacesAway( newLoc, sDestination ) <= sClosePathLimit )
				{
					// stop the path here!
					iDestination = newLoc;
					sDestination = newLoc;
					fCloseGoodEnough = FALSE;
				}
			}
			//make the destination look very attractive
			if (newLoc == iDestination)
					nextCost = 0;
			else
				//if (_KeyDown(CTRL_DOWN) && nextCost < TRAVELCOST_VEINEND)
				if (gfPlotDirectPath && nextCost < NOPASS)
						nextCost = TRAVELCOST_FLAT;



			//if (ISVEIN(prevCost))
			//		prevCost=VEINCOST;


			// make water cost attractive for water to water paths
			if (iWaterToWater)
			{
				if (ISWATER(prevCost))
					prevCost = EASYWATERCOST;
				if (ISWATER(nextCost))
					nextCost = EASYWATERCOST;
			}

			// NOTE: on September 24, 1997, Chris went back to a diagonal bias system
			// SANDRO - you know this is here twice if gubNPCAPBudget is set
			if (iCnt & 1 && !gubNPCAPBudget)
			{
				// moving on a diagonal
				//nextCost = gubDiagCost[nextCost];
				nextCost = nextCost * 14 / 10;
				//nextCost++;
			}			

			if ( bLoopState == LOOPING_REVERSE)
			{
				// penalize moving backwards to encourage turning sooner
				nextCost += 50;
			}			

			if (gGameExternalOptions.fAIPathTweaks && s->bTeam != gbPlayerNum)
			{
				// tanks prefer moving in straight directions to avoid sliding effect
				if (TANK(s) && (iCnt & 1))
				{
					nextCost += 50;
				}

				// combat jeeps prefer moving on roads
				UINT8 ubTerrainType = GetTerrainTypeForGrid(newLoc, bLevel);				
				if (COMBAT_JEEP(s) && ubTerrainType != PAVED_ROAD && ubTerrainType != DIRT_ROAD)
				{
					nextCost += 20;
				}

				// armed vehicles avoid moving into buildings
				if (ARMED_VEHICLE(s) && ubTerrainType == FLAT_FLOOR)
				{
					nextCost += 20;
				}

				// experimental path tweaks for AI
				if (IS_MERC_BODY_TYPE(s))
				{
					if (InGasSpot(s, newLoc, bLevel))
					{
						nextCost += 40;
					}
					else if (DeepWater(newLoc, bLevel) && (s->numFlanks == 0 || s->numFlanks >= MAX_FLANKS_RED))
					{
						nextCost += 20;
					}
					else if (s->bTeam == ENEMY_TEAM &&
						s->aiData.bAlertStatus >= STATUS_RED &&
						(InLightAtNight(newLoc, bLevel) || GetNearestRottingCorpseAIWarning(newLoc) > 0))
					{
						nextCost += 20;
					}
				}
			}

			newTotCost = curCost + nextCost;

			// have we found a path to the current location that
			// costs less than the best so far to the same location?
			if (trailCostUsed[newLoc] != gubGlobalPathCount || newTotCost < trailCost[newLoc])
			{

				#if defined( PATHAI_VISIBLE_DEBUG )

					if (gfDisplayCoverValues && gfDrawPathPoints)
					{
						if (gsCoverValue[newLoc] == 0x7F7F)
						{
							gsCoverValue[newLoc] = usCounter++;
						}
						/*
						else if (gsCoverValue[newLoc] >= 0)
						{
							gsCoverValue[newLoc]++;
						}
						else
						{
							gsCoverValue[newLoc]--;
						}
						*/
					}
				#endif

				//NEWQUENODE;
				{
					if (queRequests<QPOOLNDX)
					{
						pNewPtr = pathQ + (queRequests);
						queRequests++;
						memset( pNewPtr->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );
						pNewPtr->bLevel = RandomSkipListLevel();
					}
					else if (iClosedListSize > 0)
					{
						pNewPtr = pClosedHead->pNext[0];
						pClosedHead->pNext[0] = pNewPtr->pNext[0];
						iClosedListSize--;
						queRequests++;
						memset( pNewPtr->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );
						pNewPtr->bLevel = RandomSkipListLevel();
					}
					else
					{
						pNewPtr = NULL;
					}
				}

				if (pNewPtr == NULL)
				{
					#ifdef COUNT_PATHS
						guiFailedPathChecks++;
					#endif
					gubNPCAPBudget = 0;
					gubNPCDistLimit = 0;
					return(0);
				}

				//make new path to current location
				trailTree[trailTreeNdx].nextLink	= sCurPathNdx;
				trailTree[trailTreeNdx].stepDir	= (INT8) iCnt;
				if ( bLoopState == LOOPING_REVERSE )
				{
					trailTree[trailTreeNdx].fFlags = STEP_BACKWARDS;
				}
				else
				{
					trailTree[trailTreeNdx].fFlags = 0;
				}
				trailTree[trailTreeNdx].sGridNo		= newLoc;
				pNewPtr->sPathNdx						= trailTreeNdx;
				trailTreeNdx++;

				if (trailTreeNdx >= iMaxTrailTree)
				{
					#ifdef COUNT_PATHS
						guiFailedPathChecks++;
					#endif
					gubNPCAPBudget = 0;
					gubNPCDistLimit = 0;
					return(0);
				}

				iLocY = newLoc / WORLD_COLS;
				iLocX = newLoc % WORLD_COLS;
				SETLOC( *pNewPtr, newLoc );
				pNewPtr->usCostSoFar		= (UINT16) newTotCost;
				pNewPtr->usCostToGo			= (UINT16) REMAININGCOST(pNewPtr);
				if ( fCopyReachable )
				{
					pNewPtr->usCostToGo			= 100;
				}
				else
				{
					pNewPtr->usCostToGo			= (UINT16) REMAININGCOST(pNewPtr);
				}

				pNewPtr->usTotalCost		= newTotCost + pNewPtr->usCostToGo;
				pNewPtr->ubLegDistance	= (UINT8) LEGDISTANCE( iLocX, iLocY, iDestX, iDestY );

				if (gubNPCAPBudget)
				{
				//save the AP cost so far along this path
				pNewPtr->ubTotalAPCost = ubNewAPCost;
					// update the AP costs in the AI array of path costs if necessary...
					if (fCopyPathCosts)
					{
						iX = AI_PATHCOST_RADIUS + iLocX - iOriginationX;
						iY = AI_PATHCOST_RADIUS + iLocY - iOriginationY;
						gubAIPathCosts[iX][iY] = ubNewAPCost;
					}
				}

				//update the trail map to reflect the newer shorter path
				trailCost[newLoc] = (UINT16) newTotCost;
				trailCostUsed[newLoc] = gubGlobalPathCount;

				//do a sorted que insert of the new path
				// COMMENTED OUT TO DO BOUNDS CHECKER CC JAN 18 99
				//QUEINSERT(pNewPtr);
				//#define SkipListInsert( pNewPtr )
				{
					pCurr = pQueueHead;
					uiCost = TOTALCOST( pNewPtr );
					memset( pUpdate, 0, MAX_SKIPLIST_LEVEL * sizeof( path_t *) );
					AssertMsg((bSkipListLevel <= MAX_SKIPLIST_LEVEL), String("bSkipListLevel=%d but must be less then %d", bSkipListLevel, MAX_SKIPLIST_LEVEL));//dnl ch50 071009
					for (iCurrLevel = bSkipListLevel - 1; iCurrLevel >= 0; iCurrLevel--)
					{
						pNext = pCurr->pNext[iCurrLevel];
						while (pNext)
						{
							if ( uiCost > TOTALCOST( pNext ) || (uiCost == TOTALCOST( pNext ) && FARTHER( pNewPtr, pNext ) ) )
							{
								pCurr = pNext;
								pNext = pCurr->pNext[iCurrLevel];
							}
							else
							{
								break;
							}
						}
						pUpdate[iCurrLevel] = pCurr;
					}
					pCurr = pCurr->pNext[0];
					for (iCurrLevel = 0; iCurrLevel < pNewPtr->bLevel; iCurrLevel++)
					{
						if (!(pUpdate[iCurrLevel]))
						{
							break;
						}
						pNewPtr->pNext[iCurrLevel] = pUpdate[iCurrLevel]->pNext[iCurrLevel];
						pUpdate[iCurrLevel]->pNext[iCurrLevel] = pNewPtr;
					}
					iSkipListSize++;
					if (iSkipListSize > iSkipListLevelLimit[bSkipListLevel])
					{
						pCurr = pQueueHead;
						pNext = pQueueHead->pNext[bSkipListLevel - 1];
						while( pNext )
						{
							if (pNext->bLevel > bSkipListLevel)
							{
								pCurr->pNext[bSkipListLevel] = pNext;
								pCurr = pNext;
							}
							pNext = pNext->pNext[bSkipListLevel - 1];
						}
						pCurr->pNext[bSkipListLevel] = pNext;
						bSkipListLevel++;
					}
				}

#ifdef PATHAI_SKIPLIST_DEBUG
					// print out contents of queue
					{
						INT32				iLoop;
						INT8				bTemp;

						zTempString[0] = '\0';
						pCurr = pQueueHead;
						iLoop = 0;
						while( pCurr )
						{
							sprintf( zTS, "\t%ld", pCurr->sPathNdx );
							if (pCurr == pNewPtr)
							{
								strcat( zTS, "*" );
							}
							strcat( zTempString, zTS );
							pCurr = pCurr->pNext[0];
							iLoop++;
							if (iLoop > 50)
							{
								break;
							}
						}
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, zTempString );


						zTempString[0] = '\0';
						pCurr = pQueueHead;
						iLoop = 0;
						while( pCurr )
						{
							sprintf( zTS, "\t%d", pCurr->bLevel );
							strcat( zTempString, zTS );
							pCurr = pCurr->pNext[0];
							iLoop++;
							if (iLoop > 50)
							{
								break;
							}
						}
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, zTempString );

						zTempString[0] = '\0';
						bTemp = pQueueHead->bLevel;
						pCurr = pQueueHead;
						iLoop = 0;
						while( pCurr )
						{
							bTemp = pQueueHead->bLevel;
							while ( pCurr->pNext[ bTemp - 1 ] == NULL )
							{
								bTemp--;
							}
							sprintf( zTS, "\t%d", bTemp );
							strcat( zTempString, zTS );
							pCurr = pCurr->pNext[0];
							iLoop++;
							if (iLoop > 50)
							{
								break;
							}
						}
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, zTempString );
						DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "------" );
					}
#endif

			}

NEXTDIR:
			if (bLoopState == LOOPING_CLOCKWISE) // backwards
			{
				iCnt = gOneCCDirection[ iCnt ];
			}
			else
			{
				iCnt = gOneCDirection[ iCnt ];
			}
			if ( iCnt == iLoopEnd )
			{
ENDOFLOOP:
				break;
			}
			else if ( fContinuousTurnNeeded && iCnt == gOppositeDirection[ iLoopStart ] )
			{
				fCheckedBehind = TRUE;
			}
		}
	}
	while (pathQNotEmpty && pathNotYetFound);


	#if defined( PATHAI_VISIBLE_DEBUG )
		if (gfDisplayCoverValues && gfDrawPathPoints)
		{
			SetRenderFlags( RENDER_FLAG_FULL );
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				RenderWorld();
				RenderCoverDebug( );
				InvalidateScreen( );
				EndFrameBufferRender();
				RefreshScreen( NULL );
			}
		}
	#endif


	// work finished. Did we find a path?
	if (pathQNotEmpty && pathFound)
	{
		//dnl ch53 121009
		INT32 _nextLink;
		INT32 z, _z;

		_z = 0;
		z = pQueueHead->pNext[0]->sPathNdx;

		while(z)
		{
			_nextLink = trailTree[z].nextLink;
			trailTree[z].nextLink = _z;
			_z = z;
			z = _nextLink;
		}

		// if this function was called because a solider is about to embark on an actual route
		// (as opposed to "test" path finding (used by cursor, etc), then grab all pertinent
		// data and copy into soldier's database
		if (bCopy == COPYROUTE)
		{
		z=_z;

			for (iCnt=0; z && (iCnt < MAX_PATH_LIST_SIZE); iCnt++)
		{
			s->pathing.usPathingData[iCnt] = (INT16) trailTree[z].stepDir;

			z = trailTree[z].nextLink;
		}

			s->pathing.usPathIndex = 0;
		s->pathing.usPathDataSize	= (UINT16) iCnt;

		}
		else if (bCopy == NO_COPYROUTE)
		{

		z=_z;

			for (iCnt=0; z != 0; iCnt++)
		{
			guiPathingData[ iCnt ] = trailTree[z].stepDir;

			z = trailTree[z].nextLink;
		}

		  giPathDataSize = iCnt;

		}

		#if defined( PATHAI_VISIBLE_DEBUG )
			if (gfDisplayCoverValues && gfDrawPathPoints)
			{
				SetRenderFlags( RENDER_FLAG_FULL );
				RenderWorld();
				RenderCoverDebug( );
				InvalidateScreen( );
				EndFrameBufferRender();
				RefreshScreen( NULL );
			}
		#endif


		// return path length : serves as a "successful" flag and a path length counter
		#ifdef COUNT_PATHS
			guiSuccessfulPathChecks++;
		#endif
		gubNPCAPBudget = 0;
		gubNPCDistLimit = 0;

		//TEMP:	This is returning zero when I am generating edgepoints, so I am force returning 1 until
		//		this is fixed?
		if( gfGeneratingMapEdgepoints )
		{
			return TRUE;
		}


		//Assert(!iCnt);
		return(iCnt);
	}

	#ifdef COUNT_PATHS
		if (fCopyReachable)
		{
			// actually this is a success
			guiSuccessfulPathChecks++;
		}
		else
		{
			guiUnsuccessfulPathChecks++;
		}
	#endif

	// failed miserably, report...
	gubNPCAPBudget = 0;
	gubNPCDistLimit = 0;
	return(0);
	//}
	//__except(filter(GetExceptionCode(), GetExceptionInformation()))
	//{
	//	return (0);
	//}
#endif
}

void GlobalReachableTest( INT32 sStartGridNo )
{
	SOLDIERTYPE s;
	INT32 iCurrentGridNo =0;

	// WDS - Clean up inventory handling
	//memset( &s, 0, SIZEOF_SOLDIERTYPE );
	s.initialize();
	s.sGridNo = sStartGridNo;
	s.pathing.bLevel = 0;
	s.bTeam = 1;

	//reset the flag for gridno's
	for( iCurrentGridNo =0; iCurrentGridNo < WORLD_MAX; iCurrentGridNo++ )
	{
		gpWorldLevelData[ iCurrentGridNo ].uiFlags &= ~( MAPELEMENT_REACHABLE );
	}

	ReconfigurePathAI( ABSMAX_SKIPLIST_LEVEL, ABSMAX_TRAIL_TREE, ABSMAX_PATHQ );
	gubNPCDistLimit = 0;//dnl without this each save will not calculate properly map edgepoints
	FindBestPath( &s, GRIDSIZE, 0, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009
	RestorePathAIToDefaults();
}

void LocalReachableTest( INT32 sStartGridNo, INT8 bRadius )
{
	SOLDIERTYPE s;
	INT32 iCurrentGridNo = 0;
	INT32	iX, iY;

	// WDS - Clean up inventory handling
	//memset( &s, 0, SIZEOF_SOLDIERTYPE );
	s.initialize();
	s.sGridNo = sStartGridNo;

	//if we are moving on the gorund level
	if( gsInterfaceLevel == I_ROOF_LEVEL )
	{
		s.pathing.bLevel = 1;
	}
	else
	{
		s.pathing.bLevel = 0;
	}

	s.bTeam = OUR_TEAM;

	//reset the flag for gridno's
	for ( iY = -bRadius; iY <= bRadius; iY++ )
	{
		for ( iX = -bRadius; iX <= bRadius; iX++ )
		{
			iCurrentGridNo = sStartGridNo + iX + iY * MAXCOL;
			if ( iCurrentGridNo >= 0 && iCurrentGridNo <= WORLD_MAX )
			{
				gpWorldLevelData[ iCurrentGridNo ].uiFlags &= ~( MAPELEMENT_REACHABLE );
			}
		}
	}

	// set the dist limit
	gubNPCDistLimit = bRadius;
	// make the function call
	FindBestPath( &s, GRIDSIZE, s.pathing.bLevel, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009
	// reset dist limit
	gubNPCDistLimit = 0;
}

void GlobalItemsReachableTest( INT32 sStartGridNo1, INT32 sStartGridNo2 )
{
	SOLDIERTYPE s;
	INT32 iCurrentGridNo =0;

	// WDS - Clean up inventory handling
	//memset( &s, 0, SIZEOF_SOLDIERTYPE );
	s.initialize();
	s.sGridNo = sStartGridNo1;
	s.pathing.bLevel = 0;
	s.bTeam = 1;

	//reset the flag for gridno's
	for( iCurrentGridNo =0; iCurrentGridNo < WORLD_MAX; iCurrentGridNo++ )
	{
		gpWorldLevelData[ iCurrentGridNo ].uiFlags &= ~( MAPELEMENT_REACHABLE );
	}

	ReconfigurePathAI( ABSMAX_SKIPLIST_LEVEL, ABSMAX_TRAIL_TREE, ABSMAX_PATHQ );
	FindBestPath( &s, GRIDSIZE, 0, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009
	
	if (!TileIsOutOfBounds(sStartGridNo2))
	{
		s.sGridNo = sStartGridNo2;
		FindBestPath( &s, GRIDSIZE, 0, WALKING, COPYREACHABLE, PATH_THROUGH_PEOPLE );//dnl ch50 071009
	}
	RestorePathAIToDefaults();
}

void RoofReachableTest( INT32 sStartGridNo, UINT8 ubBuildingID )
{
	SOLDIERTYPE s;
	INT32 sGridNo;

	// WDS - Clean up inventory handling
	//memset( &s, 0, SIZEOF_SOLDIERTYPE );
	s.initialize();
	s.sGridNo = sStartGridNo;
	s.pathing.bLevel = 1;
	s.bTeam = 1;

	// clearing flags

	//for( sGridNo = 0 ; sGridNo < NOWHERE ; ++sGridNo )
	//for( sGridNo = 0 ; sGridNo < MAX_MAP_POS; ++sGridNo )
	for( sGridNo = 0 ; sGridNo < WORLD_MAX ; ++sGridNo )
	{
		gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
	}

	gubBuildingInfoToSet = ubBuildingID;

	ReconfigurePathAI( ABSMAX_SKIPLIST_LEVEL, ABSMAX_TRAIL_TREE, ABSMAX_PATHQ );
	FindBestPath( &s, GRIDSIZE, 1, WALKING, FINDCLIMBPOINTS, 0 );//dnl ch50 071009
	RestorePathAIToDefaults();

	// set start position to reachable since path code sets it unreachable
	gpWorldLevelData[ sStartGridNo ].uiFlags |= MAPELEMENT_REACHABLE;

	// reset building variable
	gubBuildingInfoToSet = 0;
}

void ErasePath(char bEraseOldOne)
{
	INT32 iCnt;

 // NOTE: This routine must be called BEFORE anything happens that changes
 //		a merc's gridno, else the....

 //EraseAPCursor();

	if ( gfUIHandleShowMoveGrid )
	{
			gfUIHandleShowMoveGrid					= FALSE;

		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS4	);
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS9	);
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS2 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS13 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS15 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS19 );
		RemoveTopmost( gsUIHandleShowMoveGridLocation, FIRSTPOINTERS20 );
	}

	if (!gusPathShown)
	{
	//OldPath = FALSE;
	return;
	}


 //if (OldPath > 0 && !eraseOldOne)
 //	return;

 //OldPath = FALSE;



	gusPathShown = FALSE;


	for (iCnt=0; iCnt < giPlotCnt; iCnt++)
	{
		//Grid[PlottedPath[cnt]].fstep = 0;

		RemoveAllObjectsOfTypeRange( guiPlottedPath[iCnt], FOOTPRINTS, FOOTPRINTS );

		RemoveAllOnRoofsOfTypeRange( guiPlottedPath[iCnt], FOOTPRINTS, FOOTPRINTS );

		//RemoveAllObjectsOfTypeRange( guiPlottedPath[iCnt], FIRSTPOINTERS, FIRSTPOINTERS );
	}

 //for (cnt=0; cnt < GRIDSIZE; cnt++)
 //	Grid[cnt].fstep = 0;
 //RemoveAllStructsOfTypeRange( gusEndPlotGridNo, GOODRING, GOODRING );

 giPlotCnt = 0;
 memset(guiPlottedPath,0,256*sizeof(UINT32));


}




INT32 PlotPath( SOLDIERTYPE *pSold, INT32 sDestGridNo, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget)
{
	INT16 sTileCost,sPoints=0,sAnimCost=0;
	INT16 sPointsWalk=0,sPointsCrawl=0,sPointsRun=0,sPointsSwat=0;
	INT16 sExtraCostStand,sExtraCostSwat,sExtraCostCrawl;
	FLOAT sMovementAPsCost = 0; // added by SANDRO
	INT32 iLastGrid, sTempGrid;
	INT32 iCnt;
	INT32 sOldGrid=0;
	INT16 sFootOrderIndex;
	INT16 sSwitchValue;
	INT16 sFootOrder[5] = {	GREENSTEPSTART, PURPLESTEPSTART, BLUESTEPSTART, ORANGESTEPSTART, REDSTEPSTART };
	UINT16 usTileIndex;
	UINT16 usTileNum;
	LEVELNODE *pNode;
	UINT16 usMovementModeToUseForAPs;
	BOOLEAN	bIgnoreNextCost = FALSE;
	INT32 sTestGridNo;

	if ( bPlot && gusPathShown )
	{
		ErasePath(FALSE);
	}

	gusAPtsToMove = 0;
	sTempGrid = pSold->sGridNo;

	sFootOrderIndex = 0;

	// check for active reverse mode ( ALT key pressed )
	if ( gUIUseReverse )
		bReverse = gUIUseReverse;

	//gubNPCMovementMode = (UINT8) usMovementMode;
	// distance limit to reduce the cost of plotting a path to a location we can't reach

	// For now, use known hight adjustment
	// sevenfm: ignore person at destination if we are estimating path cost
	if ( gfRecalculatingExistingPathCost || FindBestPath( pSold, sDestGridNo, pSold->pathing.bLevel, usMovementMode, bCopyRoute, gfEstimatePath ? PATH_IGNORE_PERSON_AT_DEST : 0 ) )
	{
		// if soldier would be STARTING to run then he pays a penalty since it takes time to
		// run full speed
		if (pSold->usAnimState != RUNNING)
		{
			// for estimation purposes, always pay penalty
			sPointsRun = GetAPsStartRun( pSold ); // changed by SANDRO
		}

		//shadooow: a little hack to calculate correct AP cost because game doesn't allow to run in reverse and forces walk mode
		if (usMovementMode && (pSold->bReverse || bReverse))
		{
			usMovementMode = WALKING;
		}

		// Add to points, those needed to start from different stance!
		sPoints = sPoints + MinAPsToStartMovement( pSold, usMovementMode );

		// We should reduce points for starting to run if first tile is a fence...
		sTestGridNo  = NewGridNo(pSold->sGridNo, DirectionInc( (UINT8)guiPathingData[0]));

		// WANNE: Quickfix for wrong pathing data (direction). This fixes crash that could rarly occur
		if ((UINT8)guiPathingData[0] > 7)
		{
			guiPathingData[0] = 0;
		}

		if ( gubWorldMovementCosts[ sTestGridNo ][ guiPathingData[0] ][ pSold->pathing.bLevel] == TRAVELCOST_FENCE )
		{
			if ( usMovementMode == RUNNING && pSold->usAnimState != RUNNING )
			{
				sPoints -= GetAPsStartRun( pSold ); // changed by SANDRO
			}
		}

		if (bStayOn)
		{
			iLastGrid = giPathDataSize+1;
		}
		else
		{
			iLastGrid = giPathDataSize;
		}

		INT16 usMovementModeBefore = pSold->usAnimState;

		// sevenfm: moved calculations out of loop
		UINT16 usBackpackPenalty = 0;

		// Check for backpack
		if (UsingNewInventorySystem() && gGameExternalOptions.fBackPackWeightLowersAP)
		{
			INT8 bSlot = FindBackpackOnSoldier(pSold);
			if (bSlot != ITEM_NOT_FOUND)
			{
				UINT16 usBPPenalty = APBPConstants[AP_MODIFIER_PACK];
				if (bSlot == BPACKPOCKPOS) //Backpack carried on back
				{
					OBJECTTYPE * pObj = &(pSold->inv[BPACKPOCKPOS]);
					UINT16 usBackPackWeight = CalculateObjectWeight(pObj);
					// CalculateObjectWeight checks for active LBE gear. Unfortunately our backpack is not active since we are carrying it.
					// Sounds not intuitive at all, active means the LBE caries items (marked with blue *), but when put on the LBE adittional slots of our soldier
					// are activated where something can be carried. So we have to add the weights of those slots as well.
					std::vector<INT8> vbLBESlots;
					GetLBESlots(BPACKPOCKPOS, vbLBESlots);
					for (UINT8 i = 0; i < vbLBESlots.size(); i++)
					{
						pObj = &(pSold->inv[vbLBESlots[i]]);
						usBackPackWeight += CalculateObjectWeight(pObj);
					}
					usBPPenalty = min((usBackPackWeight / 50), usBPPenalty); //1 AP penalty for each 5kg of weight up to the penalty defined by AP_MODIFIER_PACK (default = 4)
				}
				else //Backpack carried not on back (maybe somewhere inside another LBE or in Hand?)
				{
					//apply full penalty
				}
				usBackpackPenalty = usBPPenalty;
			}
		}

		for ( iCnt=0; iCnt < iLastGrid; iCnt++ )
		{
			sExtraCostStand = 0;
			sExtraCostSwat = 0;
			sExtraCostCrawl = 0;
			// what is the next gridno in the path?
			sOldGrid	= sTempGrid;

			sTempGrid  = NewGridNo(sTempGrid, DirectionInc( (UINT8)guiPathingData[iCnt]));
			gfPlotPathEndDirection = GetDirectionToGridNoFromGridNo(sOldGrid, sTempGrid);
			// Get switch value...
			sSwitchValue = gubWorldMovementCosts[ sTempGrid ][ (INT8)guiPathingData[iCnt] ][ pSold->pathing.bLevel];

			usMovementModeToUseForAPs = usMovementMode;

			// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
			// This should fix "problems" for special modified maps
			UINT8 ubTerrainID = gpWorldLevelData[ sTempGrid ].ubTerrainID;

			if ( TERRAIN_IS_WATER( ubTerrainID) && pSold->pathing.bLevel > 0 )
				ubTerrainID = FLAT_GROUND;

			// ATE - MAKE MOVEMENT ALWAYS WALK IF IN WATER
			if ( TERRAIN_IS_WATER( ubTerrainID) )
			{
				usMovementModeToUseForAPs = WALKING;
			}

			//shadooow: moved inside loop because it can happen we (re)start running after we walked in water
			if (sSwitchValue != TRAVELCOST_FENCE && usMovementModeToUseForAPs == RUNNING && usMovementModeBefore != RUNNING)
			{
				sPoints += GetAPsStartRun(pSold);
			}
			usMovementModeBefore = usMovementModeToUseForAPs;
			
			// get the tile cost for that tile based on WALKING
			sTileCost = TerrainActionPoints( pSold, sTempGrid, (INT8)guiPathingData[iCnt], pSold->pathing.bLevel );

			if ( bIgnoreNextCost )
			{
				bIgnoreNextCost = FALSE;
			}
			else
			{
				// ATE: If we have a 'special cost, like jump fence...
				if ( sSwitchValue == TRAVELCOST_FENCE )
				{
					sPoints = sPoints + sTileCost;

					bIgnoreNextCost = TRUE;

					// If we are changing stance ( either before or after getting there....
					// We need to reflect that...
					switch( usMovementModeToUseForAPs )
					{
						case RUNNING:
						case WALKING :
						case WALKING_WEAPON_RDY:
						case WALKING_DUAL_RDY:
						case WALKING_ALTERNATIVE_RDY :

							// silversurfer: It doesn't matter if we continue moving after the jump. If we were standing before we will stand up again.
/*							// Add here cost to go from crouch to stand AFTER fence hop....
							// Since it's AFTER.. make sure we will be moving after jump...
							if ( ( iCnt + 2 ) < iLastGrid )
							{
								sExtraCostStand += GetAPsCrouch(pSold, TRUE);

								// ATE: if running, charge extra point to start again
								if ( usMovementModeToUseForAPs == RUNNING )
								{
									sExtraCostStand += GetAPsStartRun(pSold);
								}

								sPoints = sPoints + sExtraCostStand;              
							}*/
							// Add cost to stand up after jump
							sExtraCostStand += GetAPsCrouch(pSold, TRUE);
							if ( ( iCnt + 2 ) < iLastGrid && usMovementModeToUseForAPs == RUNNING )
							{
								sExtraCostStand += GetAPsStartRun(pSold);
							}
							sPoints = sPoints + sExtraCostStand;              
							break;

						case SWATTING:
						case CROUCHEDMOVE_RIFLE_READY:
						case CROUCHEDMOVE_PISTOL_READY:
						case CROUCHEDMOVE_DUAL_READY:

							// Add cost to stand up once BEFORE....
							sExtraCostSwat += GetAPsCrouch(pSold, TRUE);
							sPoints = sPoints + sExtraCostSwat;              
							break;

						case CRAWLING:

							// Add cost to stand up before and go prone again after jumping
							sExtraCostCrawl += GetAPsCrouch(pSold, TRUE) + ( 2 * GetAPsProne(pSold, TRUE) );
							sPoints = sPoints + sExtraCostCrawl;              

							// Can't do it here.....
							break;

					}
				}
				else if (sTileCost > 0)
				{
					// else, movement is adjusted based on mode...

					if (sSwitchValue == TRAVELCOST_NOT_STANDING)
					{
						switch( usMovementModeToUseForAPs )
						{
							case RUNNING:
							case WALKING :
							case WALKING_WEAPON_RDY:
							case WALKING_DUAL_RDY:
							case WALKING_ALTERNATIVE_RDY :
								// charge crouch APs for ducking head!
								sExtraCostStand += GetAPsCrouch(pSold, TRUE);
								break;

							default:
								break;
						}
					}

					// so, then we must modify it for other movement styles and accumulate
					// CHRISL: Force display path to calculate AP cost differently if we're wearing a backpack
					///////////////////////////////////////////////////////////////////////////////////////////////////////////
					// SANDRO - This part have been modified "a bit" (see also "TerrainActionPoints" in "points.cpp")
					// Check movement modifiers
					switch( usMovementModeToUseForAPs )
					{
						case RUNNING:
							sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_RUN];
							break;
						case WALKING:
							if ( 0 && !(pSold->MercInWater()) && ( (gAnimControl[ pSold->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSold->usAnimState ].uiFlags & ANIM_FIRE ) ))
							{
								sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_WALK] + APBPConstants[AP_MODIFIER_READY];	
							}
							else
							{
								sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_WALK];						
							}
							break;
						case WALKING_ALTERNATIVE_RDY :
							sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_WALK];
							break;
						case WALKING_WEAPON_RDY:
						case WALKING_DUAL_RDY:
							sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_WALK] + APBPConstants[AP_MODIFIER_READY];
							break;
						case SWATTING:
						case SIDE_STEP_CROUCH_RIFLE:
						case SIDE_STEP_CROUCH_PISTOL:
						case SIDE_STEP_CROUCH_DUAL:
						case CROUCHEDMOVE_RIFLE_READY:
						case CROUCHEDMOVE_PISTOL_READY:
						case CROUCHEDMOVE_DUAL_READY:
							sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_SWAT];
							break;
						case CRAWLING:
							sMovementAPsCost = sTileCost + APBPConstants[AP_MODIFIER_CRAWL];
							break;
						default:
							sMovementAPsCost = sTileCost;
							break;
					}
				
					// Check for reverse mode
					if ( pSold->bReverse || bReverse )
						sMovementAPsCost += APBPConstants[AP_REVERSE_MODIFIER];

					// STOMP traits - Athletics trait decreases movement cost
					if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSold, ATHLETICS_NT ))
					{
						sMovementAPsCost = max(1, (sMovementAPsCost * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f) );
					}

					// Moa: scuba fins and swimming background
					if ( pSold->inv[LEGPOS].exists() && HasItemFlag( pSold->inv[LEGPOS].usItem, SCUBA_FINS ) )
					{
						if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
							sMovementAPsCost /= 2;
						else
							sMovementAPsCost *= 2;
					}
					if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
						sMovementAPsCost = sMovementAPsCost * (100 + pSold->GetBackgroundValue(BG_SWIMMING)) / 100.0f;

					// Check if doors if not player's merc (they have to open them manually)
					if ( sSwitchValue == TRAVELCOST_DOOR && pSold->bTeam != gbPlayerNum )
					{
						sMovementAPsCost += GetAPsToOpenDoor( pSold ) + GetAPsToOpenDoor( pSold ); // Include open and close costs!
					}
					// Check for stealth mode
					if (pSold->bStealthMode || bStealth)
					{
						// STOMP traits - Stealthy trait decreases stealth AP modifier
						if (gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT(pSold, STEALTHY_NT))
						{
							sMovementAPsCost += max(0.0f, (APBPConstants[AP_STEALTH_MODIFIER] * (100.0f - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100.0f));
						}
						else
						{
							sMovementAPsCost += APBPConstants[AP_STEALTH_MODIFIER];
						}
					}

					// Flugente: riot shields lower movement speed
					if ( pSold->IsRiotShieldEquipped( ) )
					{
						sMovementAPsCost *= gItemSettings.fShieldMovementAPCostModifier;
					}
				
					// Flugente: dragging someone
					if ( pSold->IsDragging( ) )
					{
						sMovementAPsCost *= gItemSettings.fDragAPCostModifier;
					}

					// Check for backpack
					sMovementAPsCost += usBackpackPenalty;

					// moving diagonally
					if (guiPathingData[iCnt] & 1)
						sMovementAPsCost *= 1.4f;

					sPoints += (INT16)(sMovementAPsCost + 0.5f) + sExtraCostStand;
					///////////////////////////////////////////////////////////////////////////////////////////////////////////
				}		
			}

			// THIS NEXT SECTION ONLY NEEDS TO HAPPEN FOR CURSOR UI FEEDBACK, NOT ACTUAL COSTING

			if (bPlot && ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) ) ) // OR USER OPTION ON... ***)
			{
				// ATE; TODO: Put stuff in here to allow for fact of costs other than movement ( jump fence, open door )

				// CHRISL: Adjusted system to use different move costs while wearing a backpack
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - This part was modified "a bit"
				sMovementAPsCost = sTileCost;

				// Check for reverse mode
				if ( pSold->bReverse || bReverse )
				{
					sMovementAPsCost += APBPConstants[AP_REVERSE_MODIFIER];
				}

				// STOMP traits - Athletics trait decreases movement cost
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSold, ATHLETICS_NT ))
				{
					sPointsWalk += max(1, (INT16)((sMovementAPsCost + APBPConstants[AP_MODIFIER_WALK]) * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f + 0.5f));
					sPointsCrawl += max(1, (INT16)((sMovementAPsCost + APBPConstants[AP_MODIFIER_CRAWL]) * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f + 0.5f));
					sPointsSwat += max(1, (INT16)((sMovementAPsCost + APBPConstants[AP_MODIFIER_SWAT]) * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f + 0.5f));
					sPointsRun += max(1, (INT16)((sMovementAPsCost + APBPConstants[AP_MODIFIER_RUN]) * (100 - gSkillTraitValues.ubATAPsMovementReduction) / 100.0f + 0.5f));
				}
				// Specify movement modes
				else
				{
					sPointsWalk += sMovementAPsCost + APBPConstants[AP_MODIFIER_WALK];
					sPointsCrawl += sMovementAPsCost + APBPConstants[AP_MODIFIER_CRAWL];
					sPointsSwat += sMovementAPsCost + APBPConstants[AP_MODIFIER_SWAT];
					sPointsRun += sMovementAPsCost + APBPConstants[AP_MODIFIER_RUN];
				}

				// Moa: scuba fins and swimming background
				if ( pSold->inv[LEGPOS].exists() && HasItemFlag( pSold->inv[LEGPOS].usItem, SCUBA_FINS ) )
				{
					if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
					{
						sPointsWalk /= 2;
						sPointsCrawl /= 2;
						sPointsSwat /= 2;
						sPointsRun /= 2;
					}
					else
					{
						sPointsWalk *= 2;
						sPointsCrawl *= 2;
						sPointsSwat *= 2;
						sPointsRun *= 2;
					}
				}
				if ( TERRAIN_IS_HIGH_WATER( ubTerrainID) )
				{
					sPointsWalk = (sPointsWalk * (100 + pSold->GetBackgroundValue(BG_SWIMMING))) / 100;
					sPointsCrawl = (sPointsCrawl * (100 + pSold->GetBackgroundValue(BG_SWIMMING))) / 100;
					sPointsSwat = (sPointsSwat * (100 + pSold->GetBackgroundValue(BG_SWIMMING))) / 100;
					sPointsRun = (sPointsRun * (100 + pSold->GetBackgroundValue(BG_SWIMMING))) / 100;
				}
				// walking with weapon raised?
				if (!(pSold->MercInWater()) && ( (gAnimControl[ pSold->usAnimState ].uiFlags & ANIM_FIREREADY ) || (gAnimControl[ pSold->usAnimState ].uiFlags & ANIM_FIRE ) ))
				{
					sPointsWalk += APBPConstants[AP_MODIFIER_READY];
				}
				// Check for stealth mode
				if ( pSold->bStealthMode || bStealth )
				{
					 // STOMP traits - Stealthy trait decreases stealth AP modifier
					if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSold, STEALTHY_NT ))
					{
						 sPointsWalk += (max(0, (INT16)(APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100.0f + 0.5f))); 
						 sPointsCrawl += (max(0, (INT16)(APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100.0f + 0.5f)));
						 sPointsSwat += (max(0, (INT16)(APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100.0f + 0.5f)));
						 sPointsRun += (max(0, (INT16)(APBPConstants[AP_STEALTH_MODIFIER] * (100 - gSkillTraitValues.ubSTStealthModeSpeedBonus) / 100.0f + 0.5f)));
					}
					else
					{
						sPointsWalk += APBPConstants[AP_STEALTH_MODIFIER];
						sPointsCrawl += APBPConstants[AP_STEALTH_MODIFIER];
						sPointsSwat += APBPConstants[AP_STEALTH_MODIFIER];
						sPointsRun += APBPConstants[AP_STEALTH_MODIFIER];
					}
				}

				// Flugente: riot shields lower movement speed
				if ( pSold->IsRiotShieldEquipped( ) )
				{
					sPointsWalk *= gItemSettings.fShieldMovementAPCostModifier;
					sPointsCrawl *= gItemSettings.fShieldMovementAPCostModifier;
					sPointsSwat *= gItemSettings.fShieldMovementAPCostModifier;
					sPointsRun *= gItemSettings.fShieldMovementAPCostModifier;
				}

				// Flugente: dragging someone
				if ( pSold->IsDragging() )
				{
					sPointsWalk *= gItemSettings.fDragAPCostModifier;
					sPointsCrawl *= gItemSettings.fDragAPCostModifier;
					sPointsSwat *= gItemSettings.fDragAPCostModifier;
					sPointsRun *= gItemSettings.fDragAPCostModifier;
				}

				// Check for backpack
				// Moa: apply penalty for heavily packed backpack (wobble penalty)				
				sPointsWalk += usBackpackPenalty;
				sPointsCrawl += usBackpackPenalty;
				sPointsSwat += usBackpackPenalty;
				sPointsRun += usBackpackPenalty;

				if ( sExtraCostStand )
				{
					sPointsWalk += sExtraCostStand;
					sPointsCrawl += sExtraCostStand;
					sPointsSwat += sExtraCostStand;
					sPointsRun += sExtraCostStand;
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

			if ( iCnt == 0 && bPlot )
			{
				gusAPtsToMove = sPoints;

				giPlotCnt = 0;

			}

			//if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) ) // OR USER OPTION "show paths" ON... ***
			{
				if (bPlot && ((iCnt < (iLastGrid-1)) || (iCnt < iLastGrid && bStayOn)))
				{
					guiPlottedPath[giPlotCnt++] = sTempGrid;

					// we need a footstep graphic ENTERING the next tile

					// get the direction
					usTileNum = (UINT16) guiPathingData[iCnt] + 2;
					if (usTileNum > 8)
					{
						usTileNum = 1;
					}

					// Are we a vehicle?
					//if ( pSold->flags.uiStatusFlags & SOLDIER_VEHICLE )
					//{
					//	// did we exceed WALK cost?
					//	if ( sPointsSwat > sAPBudget)
					//	{
					//		sFootOrderIndex = 4;
					//	}
					//	else
					//	{
					//		sFootOrderIndex = 3;
					//	}
					//}
					//else
					{
						// did we exceed CRAWL cost?
						if (sFootOrderIndex == 0 && sPointsCrawl > sAPBudget)
						{
							sFootOrderIndex++;
						}

						// did we exceed WALK cost?
						if (sFootOrderIndex == 1 && sPointsSwat > sAPBudget)
						{
							sFootOrderIndex++;
						}

						// did we exceed SWAT cost?
						if (sFootOrderIndex == 2 && sPointsWalk > sAPBudget)
						{
							sFootOrderIndex++;
						}

						// did we exceed RUN cost?
						if (sFootOrderIndex == 3 && sPointsRun > sAPBudget)
						{
							sFootOrderIndex++;
						}
					}

					GetTileIndexFromTypeSubIndex( FOOTPRINTS, (UINT16)usTileNum, &usTileIndex );

					// Adjust based on what mode we are in...
					if ( (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// find out which color we're using
						usTileIndex = usTileIndex + sFootOrder[ 4 ];
					}
					else // turn based
					{
						// find out which color we're using
						usTileIndex = usTileIndex + sFootOrder[sFootOrderIndex];
					}


					/*
					if ( sPoints <= sAPBudget)
					{
						// find out which color we're using
					usTileIndex += sFootOrder[sFootOrderIndex];
					}
					else
					{
						// use red footprints ( offset by 16 )
					usTileIndex += REDSTEPSTART;
					}
					*/

					if ( pSold->pathing.bLevel == 0 )
					{
						pNode = AddObjectToTail(sTempGrid, usTileIndex );
						pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}
					else
					{
						pNode = AddOnRoofToTail(sTempGrid, usTileIndex );
						pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}



					// we need a footstep graphic LEAVING this tile

					// get the direction using the NEXT tile (thus iCnt+1 as index)
					usTileNum = (UINT16) guiPathingData[iCnt + 1] + 2;
					if (usTileNum > 8)
					{
						usTileNum = 1;
					}


					// this is a LEAVING footstep which is always the second set of 8
					usTileNum += 8;

					GetTileIndexFromTypeSubIndex( FOOTPRINTS, (UINT16)usTileNum, &usTileIndex );

					// Adjust based on what mode we are in...
					if ( (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
					{
						// find out which color we're using
						usTileIndex = usTileIndex + sFootOrder[ 4 ];
					}
					else // turnbased
					{
						// find out which color we're using
						usTileIndex = usTileIndex + sFootOrder[sFootOrderIndex];
					}



					if ( pSold->pathing.bLevel == 0 )
					{
						pNode = AddObjectToTail(sTempGrid, usTileIndex );
						pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}
					else
					{
						pNode = AddOnRoofToTail(sTempGrid, usTileIndex );
						pNode->ubShadeLevel=DEFAULT_SHADE_LEVEL;
						pNode->ubNaturalShadeLevel=DEFAULT_SHADE_LEVEL;
					}
				}

			}		// end of if turn based or real-time user option "show paths" on...
		}

		if ( bPlot )
		{
			gusPathShown = TRUE;
		}

 } 	// end of found a path

 // reset distance limit
 gubNPCDistLimit = 0;

 return(sPoints);

}

INT32 UIPlotPath( SOLDIERTYPE *pSold, INT32 sDestGridNo, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget)
{
	// This function is specifically for UI calls to the pathing routine, to
	// check whether the shift key is pressed, etc.
	INT32	 sRet;

	if ( _KeyDown( SHIFT ) )
	{
		gfPlotDirectPath = TRUE;
	}

	// If we are on the same level as the interface level, continue, else return
	if ( pSold->pathing.bLevel != gsInterfaceLevel )
	{
		return( 0 );
	}

	if ( gGameSettings.fOptions[ TOPTION_ALWAYS_SHOW_MOVEMENT_PATH ] )
	{
		bPlot = TRUE;
	}

	sRet = PlotPath( pSold, sDestGridNo, bCopyRoute, bPlot, bStayOn, usMovementMode, bStealth, bReverse, sAPBudget);
	gfPlotDirectPath = FALSE;
	return( sRet );
}

INT16 RecalculatePathCost( SOLDIERTYPE *pSoldier, UINT16 usMovementMode )
{
	// AI function for a soldier already with a path; this will return the cost of that path using the given movement mode
	INT16	sRet;

	if ( !pSoldier->pathing.bPathStored || pSoldier->pathing.usPathDataSize == 0 )
	{
		return( 0 );
	}

	gfRecalculatingExistingPathCost = TRUE;
	sRet = PlotPath( pSoldier, pSoldier->pathing.sFinalDestination, NO_COPYROUTE, FALSE, FALSE, usMovementMode, FALSE, FALSE, 0 );
	gfRecalculatingExistingPathCost = FALSE;
	return( sRet );
}

INT32 EstimatePlotPath( SOLDIERTYPE *pSold, INT32 sDestGridNo, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget)
{
	// This function is specifically for AI calls to estimate path cost to a location
	// It sets stuff up to ignore all people
	INT32			sRet;

	gfEstimatePath = TRUE;

	sRet = PlotPath( pSold, sDestGridNo, bCopyRoute, bPlot, bStayOn, usMovementMode, bStealth, bReverse, sAPBudget);

	gfEstimatePath = FALSE;

	return( sRet );
}


UINT8 InternalDoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo, BOOLEAN fReturnDoorCost )
{
	// This function will return either TRAVELCOST_DOOR (in place of closed door cost),
	// TRAVELCOST_OBSTACLE, or the base ground terrain
	// travel cost, depending on whether or not the door is open or closed etc.
	BOOLEAN				fDoorIsObstacleIfClosed=FALSE;
	INT32					iDoorGridNo=-1;
	DOOR_STATUS *	pDoorStatus;
	DOOR *				pDoor;
	STRUCTURE *		pDoorStructure;
	BOOLEAN				fDoorIsOpen;
	UINT8					ubReplacementCost;

	if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
	{
		ubReplacementCost = TRAVELCOST_OBSTACLE;

		switch( ubMovementCost )
		{
			case TRAVELCOST_DOOR_CLOSED_HERE:
				fDoorIsObstacleIfClosed = TRUE;
				iDoorGridNo = iGridNo;
				ubReplacementCost = TRAVELCOST_DOOR;
				break;
			case TRAVELCOST_DOOR_CLOSED_N:
				fDoorIsObstacleIfClosed = TRUE;
				iDoorGridNo = iGridNo + dirDelta[ NORTH ];
				ubReplacementCost = TRAVELCOST_DOOR;
				break;
			case TRAVELCOST_DOOR_CLOSED_W:
				fDoorIsObstacleIfClosed = TRUE;
				iDoorGridNo = iGridNo + dirDelta[ WEST ];
				ubReplacementCost = TRAVELCOST_DOOR;
				break;
			case TRAVELCOST_DOOR_OPEN_HERE:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo;
				break;
			case TRAVELCOST_DOOR_OPEN_N:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTH ];
				break;
			case TRAVELCOST_DOOR_OPEN_NE:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTHEAST ];
				break;
			case TRAVELCOST_DOOR_OPEN_E:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ EAST ];
				break;
			case TRAVELCOST_DOOR_OPEN_SE:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ SOUTHEAST ];
				break;
			case TRAVELCOST_DOOR_OPEN_S:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ SOUTH ];
				break;
			case TRAVELCOST_DOOR_OPEN_SW:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ SOUTHWEST ];
				break;
			case TRAVELCOST_DOOR_OPEN_W:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ WEST ];
				break;
			case TRAVELCOST_DOOR_OPEN_NW:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTHWEST ];
				break;
			case TRAVELCOST_DOOR_OPEN_N_N:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTH ] + dirDelta[ NORTH ];
				break;
			case TRAVELCOST_DOOR_OPEN_NW_N:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTHWEST ] + dirDelta[ NORTH ];
				break;
			case TRAVELCOST_DOOR_OPEN_NE_N:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTHEAST ] + dirDelta[ NORTH ];
				break;
			case TRAVELCOST_DOOR_OPEN_W_W:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ WEST ] + dirDelta[ WEST ];
				break;
			case TRAVELCOST_DOOR_OPEN_SW_W:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ SOUTHWEST ] + dirDelta[ WEST ];
				break;
			case TRAVELCOST_DOOR_OPEN_NW_W:
				fDoorIsObstacleIfClosed = FALSE;
				iDoorGridNo = iGridNo + dirDelta[ NORTHWEST ] + dirDelta[ WEST ];
				break;
			default:
				break;
		}

		if ( pSoldier && (pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER || pSoldier->flags.uiStatusFlags & SOLDIER_ANIMAL) )
		{
			// can't open doors!
			ubReplacementCost = TRAVELCOST_OBSTACLE;
		}

		if (piDoorGridNo)
		{
			// return gridno of door through pointer
			*piDoorGridNo = iDoorGridNo;
		}

		if ( fReturnPerceivedValue && gpWorldLevelData[ iDoorGridNo ].ubExtFlags[0] & MAPELEMENT_EXT_DOOR_STATUS_PRESENT )
		{
			// check door status
			pDoorStatus = GetDoorStatus( iDoorGridNo );
			if (pDoorStatus)
			{
				fDoorIsOpen = (pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN) != 0;
			}
			else
			{
				// abort!
				return( ubMovementCost );
			}
		}
		else
		{
			// check door structure
			pDoorStructure = FindStructure( iDoorGridNo, STRUCTURE_ANYDOOR );
			if (pDoorStructure)
			{
				fDoorIsOpen = (pDoorStructure->fFlags & STRUCTURE_OPEN) != 0;
			}
			else
			{
				// abort!
				return( ubMovementCost );
			}
		}
		// now determine movement cost
		if (fDoorIsOpen)
		{
			if (fDoorIsObstacleIfClosed)
			{
				ubMovementCost = gTileTypeMovementCost[ gpWorldLevelData[ iGridNo ].ubTerrainID ];
			}
			else
			{
				ubMovementCost = ubReplacementCost;
			}
		}
		else
		{
			if (fDoorIsObstacleIfClosed)
			{
				// door is closed and this should be an obstacle, EXCEPT if we are calculating
				// a path for an enemy or NPC with keys

				// creatures and animals can't open doors!
				if ( fReturnPerceivedValue || ( pSoldier && (pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER || pSoldier->flags.uiStatusFlags & SOLDIER_ANIMAL ) ) )
				{
					ubMovementCost = ubReplacementCost;
				}
				else
				{
					// have to check if door is locked and NPC does not have keys!
					pDoor = FindDoorInfoAtGridNo( iDoorGridNo );
					if ( pDoor )
					{
						if ( ( !pDoor->fLocked || (pSoldier && pSoldier->flags.bHasKeys) ) && !fReturnDoorCost )
						{
							ubMovementCost = gTileTypeMovementCost[ gpWorldLevelData[ iGridNo ].ubTerrainID ];
						}
						else
						{
							ubMovementCost = ubReplacementCost;
						}
					}
					else
					{
						ubMovementCost = ubReplacementCost;
					}
				}
			}
			else
			{
				ubMovementCost = gTileTypeMovementCost[ gpWorldLevelData[ iGridNo ].ubTerrainID ];
			}
		}

	}
	return( ubMovementCost );

}

UINT8 DoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo )
{
	return( InternalDoorTravelCost( pSoldier, iGridNo, ubMovementCost, fReturnPerceivedValue, piDoorGridNo, FALSE ) );
}


