/*
	Filename        :       pathai.h
	Author          :       Ray E. Bornert II
	Date            :       1992-MAR-15

	Copyright (C) 1993 HixoxiH Software
*/

#ifndef _PATHAI_H
#define _PATHAI_H
#include "isometric utils.h"

#define USE_ASTAR_PATHS

#ifdef USE_ASTAR_PATHS

namespace ASTAR {
#include "BinaryHeap.hpp"
#include <vector>

enum eAStar
{
	AStar_Init,
	AStar_Open,
	AStar_Closed
};

class AStar_Data
{
public:
	AStar_Data()
	{
		cost = f = APCost = direction = prevCost = 0;
#ifdef ASTAR_USING_EXTRACOVER
		extraGCoverCost = -1;//-1 means we have not stopped at this node
#endif
		wasBackwards = false;
		status = AStar_Init;
		parent = GridNode(-1,-1);
	};
	//no H
	int			cost;//G
	int			f;//F
	int			APCost;//the APs spent to get here
#ifdef ASTAR_USING_EXTRACOVER
	int			extraGCoverCost;//an extra cost that makes stopping in midpath at a node with little cover worse
#endif
	GridNode	parent;
	eAStar		status;
	int			prevCost;
	bool		wasBackwards;
	int			direction;
};

typedef HEAP<int, GridNode>		AStarHeap;

class AStarPathfinder
{
public:
	AStarPathfinder			();
	static AStarPathfinder&	GetInstance();
	int				GetPath	(SOLDIERTYPE *s ,
							INT16 dest,
							INT8 ubLevel,
							INT16 usMovementMode,
							INT8 bCopy,
							UINT8 fFlags );

private:
	static	AStarPathfinder* pThis;
	std::vector<GridNode>		ClosedList;
	CBinaryHeap<int, GridNode>	OpenHeap;

	int		direction;//current direction
	int		startDir;
	int		endDir;
	int		lastDir;
	bool	startingLoop;

	SOLDIERTYPE*	pSoldier;
	INT8	onRooftop;//aka ubLevel, not sure if this bool is logically reversed yet
	bool	fNonFenceJumper;
	bool	fNonSwimmer;
	bool	fPathingForPlayer;
	bool	fPathAroundPeople;
	bool	fGoingThroughDoor;
	int		bOKToAddStructID;
	int		bLoopState;
	bool	bWaterToWater;
	bool	fVisitSpotsOnlyOnce;
	bool	fCheckedBehind;
	bool	fTurnBased;
	bool	fCloseGoodEnough;
	bool	fContinuousTurnNeeded;
	bool	fConsiderPersonAtDestAsObstacle;
	bool	fCopyReachable;
	bool	fCopyPathCosts;

	int		maxAPBudget;//the gubNPCAPBudget
	int		mercsMaxAPs;//the merc only has so many points to move with
	int		movementMode;
	int		sClosePathLimit;

	int		PATHAI_VISIBLE_DEBUG_Counter;

	//vehicle defined in cpp
	//#ifdef VEHICLE
		BOOLEAN fMultiTile;
		STRUCTURE_FILE_REF * pStructureFileRef;
	//#endif

	// member variables to prevent passing them around
	GridNode	StartNode;
	GridNode	DestNode;
	GridNode	CurrentNode;
	GridNode	ParentNode;
	INT16	ParentNodeIndex;
	INT16	CurrentNodeIndex;

	AStar_Data	AStarData[WORLD_COLS][WORLD_ROWS];





	AStarHeap		AStar	();

	void	ExecuteAStarLogic();

	void	ResetAStarList	();

	int		TerrainCostToAStarG(int const terrainCost);
	int		CalcG			(int* pPrevCost);
	int		CalcAP			(int const terrainCost);
	int		CalcH			();
	int		CalcGCover		(int const NodeIndex,
							int const APCost);
	int		CalcStartingAP	();

			//including THREATTYPE was a pain, so pass by value
	int		CalcCoverValue	(INT16 sMyGridNo, INT32 iMyThreat, INT32 iMyAPsLeft,
							INT32 myThreatsiOrigRange, INT16 myThreatssGridNo, SOLDIERTYPE* myThreatspOpponent,
							INT32 myThreatsiValue, INT32 myThreatsiAPs, INT32 myThreatsiCertainty);

	eAStar	GetAStarStatus	(const GridNode node) const {return AStarData[node.x][node.y].status;};
	GridNode	GetAStarParent	(const GridNode node) const {return AStarData[node.x][node.y].parent;};
	int		GetAStarG		(const GridNode node) const {return AStarData[node.x][node.y].cost;};
	int		GetAStarF		(const GridNode node) const {return AStarData[node.x][node.y].f;};
	int		GetActionPoints	(const GridNode node) const {return AStarData[node.x][node.y].APCost;};
	bool	GetLoopState	(const GridNode node) const {return AStarData[node.x][node.y].wasBackwards;};
	int		GetPrevCost		(const GridNode node) const {return AStarData[node.x][node.y].prevCost;};
	int		GetDirection	(const GridNode node) const {return AStarData[node.x][node.y].direction;};
#ifdef ASTAR_USING_EXTRACOVER
	int		GetExtraGCover	(const GridNode node) const {return AStarData[node.x][node.y].extraGCoverCost;};

	void	SetExtraGCover	(const GridNode node, const int extraGCoverCost) {AStarData[node.x][node.y].extraGCoverCost = extraGCoverCost;};
#endif
	void	SetAStarStatus	(const GridNode node, const eAStar status) {AStarData[node.x][node.y].status = status;};
	void	SetAStarParent	(const GridNode node, const GridNode parent) {AStarData[node.x][node.y].parent = parent;};
	void	SetAStarG		(const GridNode node, const int cost) {AStarData[node.x][node.y].cost = cost;};
	void	SetAStarF		(const GridNode node, const int f) {AStarData[node.x][node.y].f = f;};
	void	SetActionPoints	(const GridNode node, const int APCost) {AStarData[node.x][node.y].APCost = APCost;};
	void	SetLoopState	(const GridNode node, const int loopState);
	void	SetPrevCost		(const GridNode node, const int prevCost) {AStarData[node.x][node.y].prevCost = prevCost;};
	void	SetDirection	(const GridNode node, const int direction) {AStarData[node.x][node.y].direction = direction;};

	INT16	PythSpacesAway	(GridNode const node1,
							GridNode const node2);
	INT16	SpacesAway		(GridNode const node1,
							GridNode const node2);
	//bool	IsDiagonal		(GridNode const node1,
	//						GridNode const node2) {return (abs(node1.x - node2.x) && abs(node1.y - node2.y));};
	bool	IsDiagonal		(int const direction) {return (direction & 1);};
	void	InitVehicle		();
	int		VehicleObstacleCheck();
	bool	CanTraverse		();
	bool	IsSomeoneInTheWay();
	void	IncrementLoop	();
	void	InitLoop		();
	bool	ContinueLoop	();
};

};//end namespace ASTAR

#endif//end #ifdef USE_ASTAR_PATHS


BOOLEAN InitPathAI( void );
void ShutDownPathAI( void );
INT16 PlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);
INT16 UIPlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);
INT16 EstimatePlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);

void ErasePath(char bEraseOldOne);
INT32 FindBestPath(SOLDIERTYPE *s , INT16 sDestination, INT8 ubLevel, INT16 usMovementMode, INT8 bCopy, UINT8 fFlags );
void GlobalReachableTest( INT16 sStartGridNo );
void GlobalItemsReachableTest( INT16 sStartGridNo1, INT16 sStartGridNo2 );
void RoofReachableTest( INT16 sStartGridNo, UINT8 ubBuildingID );
void LocalReachableTest( INT16 sStartGridNo, INT8 bRadius );

UINT8 DoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo );
UINT8 InternalDoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo, BOOLEAN fReturnDoorCost );

INT16 RecalculatePathCost( SOLDIERTYPE *pSoldier, UINT16 usMovementMode );

// Lesh: setup max length of path
#define MAX_PATH_DATA_LENGTH 512

// Exporting these global variables
extern UINT32 guiPathingData[MAX_PATH_DATA_LENGTH];
extern UINT8 gubNPCAPBudget;
extern UINT16 gusNPCMovementMode;
extern UINT8 gubNPCDistLimit;
extern UINT8 gubNPCPathCount;
extern BOOLEAN gfPlotPathToExitGrid;
extern BOOLEAN gfNPCCircularDistLimit;
extern BOOLEAN gfEstimatePath;
extern BOOLEAN	gfPathAroundObstacles;
extern UINT8 gubGlobalPathFlags;

// Ian's terrain values for travelling speed/pathing purposes
// Fixed by CJC March 4, 1998.  Please do not change these unless familiar
// with how this will affect the path code!

#define TRAVELCOST_NONE						0
#define TRAVELCOST_FLAT						10
#define TRAVELCOST_BUMPY					12
#define TRAVELCOST_GRASS					12
#define TRAVELCOST_THICK					16
#define TRAVELCOST_DEBRIS					20
#define TRAVELCOST_SHORE					30
#define TRAVELCOST_KNEEDEEP				36
#define TRAVELCOST_DEEPWATER			50
#define TRAVELCOST_FENCE					40

// these values are used to indicate "this is an obstacle
// if there is a door (perceived) open/closed in this tile
#define TRAVELCOST_DOOR_CLOSED_HERE	220
#define TRAVELCOST_DOOR_CLOSED_N		221
#define TRAVELCOST_DOOR_CLOSED_W		222
#define TRAVELCOST_DOOR_OPEN_HERE		223
#define TRAVELCOST_DOOR_OPEN_N			224
#define TRAVELCOST_DOOR_OPEN_NE			225
#define TRAVELCOST_DOOR_OPEN_E			226
#define TRAVELCOST_DOOR_OPEN_SE			227
#define TRAVELCOST_DOOR_OPEN_S			228
#define TRAVELCOST_DOOR_OPEN_SW			229
#define TRAVELCOST_DOOR_OPEN_W			230
#define TRAVELCOST_DOOR_OPEN_NW			231
#define TRAVELCOST_DOOR_OPEN_N_N		232
#define TRAVELCOST_DOOR_OPEN_NW_N		233
#define TRAVELCOST_DOOR_OPEN_NE_N		234
#define TRAVELCOST_DOOR_OPEN_W_W		235
#define TRAVELCOST_DOOR_OPEN_SW_W		236
#define TRAVELCOST_DOOR_OPEN_NW_W		237
#define TRAVELCOST_NOT_STANDING		248
#define TRAVELCOST_OFF_MAP				249
#define TRAVELCOST_CAVEWALL				250
#define TRAVELCOST_HIDDENOBSTACLE 251
#define TRAVELCOST_DOOR						252
#define TRAVELCOST_OBSTACLE				253
#define TRAVELCOST_WALL						254
#define TRAVELCOST_EXITGRID				255

#define TRAVELCOST_TRAINTRACKS		30
#define TRAVELCOST_DIRTROAD				9
#define TRAVELCOST_PAVEDROAD			9
#define TRAVELCOST_FLATFLOOR			10

#define TRAVELCOST_BLOCKED (TRAVELCOST_OFF_MAP)
#define IS_TRAVELCOST_DOOR( x ) (x >= TRAVELCOST_DOOR_CLOSED_HERE && x <= TRAVELCOST_DOOR_OPEN_NW_W)
#define IS_TRAVELCOST_CLOSED_DOOR( x ) (x >= TRAVELCOST_DOOR_CLOSED_HERE && x << TRAVELCOST_DOOR_CLOSED_W)

// ------------------------------------------
// PLOT PATH defines
#define NOT_STEALTH							0
#define	STEALTH									1

#define NO_PLOT									0
#define PLOT										1

#define TEMPORARY								0
#define PERMANENT								1

#define FORWARD									0
#define REVERSE									1

#define NO_COPYROUTE						0
#define COPYROUTE								1
#define COPYREACHABLE           2
#define COPYREACHABLE_AND_APS		3

#define PATH_THROUGH_PEOPLE					0x01
#define PATH_IGNORE_PERSON_AT_DEST	0x02
#define PATH_CLOSE_GOOD_ENOUGH			0x04

#define PATH_CLOSE_RADIUS 5

// ------------------------------------------

#endif
