
#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H
#include "stdafx.h"
#include "BinaryHeap.hpp"

using namespace std;

typedef HEAP<float, CSmallPoint> AStarHeap;

class AStarPathfinder
{
public:
	AStarPathfinder();
	vector<CSmallPoint>		ClosedList;
	CBinaryHeap<float, CSmallPoint>	OpenHeap;

	AStarHeap		AStar	(const CSmallPoint& StartCity,
							const vector<AStarHeap>& acceptableDestCities,
							const ePlayer player,
							const unsigned int maxNodes);

	void	ExecuteAStarLogic(const CSmallPoint& StartCity,
							const CSmallPoint& BestNode,
				
							
							const vector<AStarHeap>& acceptableDestCities,
							const ePlayer player);

	bool	isNodeDestCity	(const CSmallPoint& BestNode,
							const vector<AStarHeap>& acceptableDestCities);

	void	ResetAStarList	();

private:
	AStar_Data	AStarData[MAX_SIZE_X][MAX_SIZE_Y];

	float	CalcGCost		(const CSmallPoint& start, const CSmallPoint& dest, const ePlayer player);
	float	CalcGTime		(const CSmallPoint& start, const CSmallPoint& dest);
	float	CalcHTime		(const CSmallPoint& start, const CSmallPoint& dest);

	eAStar	GetAStarStatus	(const CSmallPoint& node) const {return AStarData[node.x][node.y].status;};
	CSmallPoint	GetAStarParent	(const CSmallPoint& node) const {return AStarData[node.x][node.y].parent;};
	CSmallPoint	GetAStarCity	(const CSmallPoint& node) const {return AStarData[node.x][node.y].city;};
	float	GetAStarGCost	(const CSmallPoint& node) const {return AStarData[node.x][node.y].cost;};
	float	GetAStarGTime	(const CSmallPoint& node) const {return AStarData[node.x][node.y].time;};
	float	GetAStarF		(const CSmallPoint& node) const {return AStarData[node.x][node.y].profit;};

	void	SetAStarStatus	(const CSmallPoint& node, const eAStar status) {AStarData[node.x][node.y].status = status;};
	void	SetAStarParent	(const CSmallPoint& node, const CSmallPoint& parent) {AStarData[node.x][node.y].parent = parent;};
	void	SetAStarCity	(const CSmallPoint& node, const CSmallPoint& city) {AStarData[node.x][node.y].city = city;};
	void	SetAStarGCost	(const CSmallPoint& node, const float cost) {AStarData[node.x][node.y].cost = cost;};
	void	SetAStarGTime	(const CSmallPoint& node, const float time) {AStarData[node.x][node.y].time = time;};
	void	SetAStarF		(const CSmallPoint& node, const float profit) {AStarData[node.x][node.y].profit = profit;};
};

#endif
