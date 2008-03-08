
#include "stdafx.h"
#include "profiler.h"
#include "BinaryHeap.hpp"

using namespace std;

void AStarPathfinder::ResetAStarList()
{
	for each (CSmallPoint node in ClosedList) {
		SetAStarStatus(node, AStar_Init);
		SetAStarGCost(node, 0.0f);
		SetAStarGTime(node, 0.0f);
	}
	for (int size = OpenHeap.size(); size > 0; --size) {
		CSmallPoint node = OpenHeap.peekElement(size).data;
		SetAStarStatus(node, AStar_Init);
		SetAStarGCost(node, 0.0f);
		SetAStarGTime(node, 0.0f);
	}
	ClosedList.clear();
	OpenHeap.clear();
	return;
}//end ResetAStarList

AStarHeap AStarPathfinder::AStar(const CSmallPoint& StartCity,
								 const vector<AStarHeap>& acceptableDestCities,
								 const ePlayer player,
								 const unsigned int maxNodes)
{
	AStarHeap TopHeap(StartCity, 0);
	CSmallPoint BestNode = StartCity;
	for (;;) {
		//this peg is now closed
		//SetAStarStatus(BestNode, AStar_Closed);
		ClosedList.push_back(BestNode);
		if (isNodeDestCity(BestNode, acceptableDestCities) == true) {
			//the best path to date leads to a city, other cities are not as good, so return this city
			return TopHeap;
		}

		//if we have run out of time and the best node isn't a city
		if (OpenHeap.size() + ClosedList.size() > maxNodes) {
			float best = -99999999.0f;
			int bestIndex = 0;
			for (int index = 1; index <= OpenHeap.size(); ++index) {
				float key = OpenHeap.peekElement(index).key;
				if (key > best) {
					bestIndex = index;
					best = key;
				}
			}
			if (bestIndex) {
				//but at least one city was opened, it might not be A* efficient, but it's a path
				return OpenHeap.peekElement(bestIndex);
			}
			else {
				//no cities were opened
				return AStarHeap(CSmallPoint(0,0), 111);
			}
		}

		//this adds to the heap nodes that might be on a path to a city
		ExecuteAStarLogic(StartCity, BestNode, acceptableDestCities, player);

		if (OpenHeap.size() == 0) {
			return AStarHeap(CSmallPoint(0,0), -222);
		}

		//get the best point so far from the heap
		TopHeap = OpenHeap.popTopHeap();
		BestNode = TopHeap.data;
		//ASSERT(GetAStarStatus(BestNode) == AStar_Open);
	}
	return TopHeap;
}//end AStar

void AStarPathfinder::ExecuteAStarLogic(const CSmallPoint& StartCity,
										const CSmallPoint& BestNode,
										const vector<AStarHeap>& acceptableDestCities,
										const ePlayer player)
{
	float baseGCost = GetAStarGCost(BestNode);
	float baseGTime = GetAStarGTime(BestNode);

	for (int index = 0; index < 8; ++index) {
		CSmallPoint ThisNode = BestNode + offsets[index];
		if (CWorldManager::isNodeInWorld(ThisNode) == false ||
			GetAStarStatus(ThisNode) == AStar_Closed) {
			continue;
		}

		float AStarGCost = baseGCost + CalcGCost(BestNode, ThisNode, player);
		float AStarGTime = baseGTime + CalcGTime(BestNode, ThisNode);

		if (GetAStarStatus(ThisNode) == AStar_Open
			&& AStarGCost >= GetAStarGCost(ThisNode)
			&& AStarGTime >= GetAStarGTime(ThisNode)) {
			//no matter what Dest City, this path is less profitable
			continue;
		}

		CSmallPoint bestCity(0,0);
		float AStarF = 0.0f;
		for each (AStarHeap city in acceptableDestCities) {
			//is this path and Dest City combo profitable?
			if (city.key > AStarGCost) {
				//find the best H
				float AStarHTime = CalcHTime(ThisNode, city.data);
				float AStarHProfit = city.key;
				float estimatedF = (AStarHProfit - AStarGCost) / (AStarHTime + AStarGTime);
				if (AStarF < estimatedF) {
					bestCity = city.data;
					AStarF = estimatedF;
				}
			}
		}
		if (AStarF == 0.0f) {
			//no profitable paths remain from this node
			continue;
		}

		//insert this node onto the heap
		if (GetAStarStatus(ThisNode) == AStar_Init) {
			OpenHeap.insertElement(ThisNode, -AStarF);
		}
		else {
			if (GetAStarF(ThisNode) > AStarF) {
				//the node is already open and this path is not more cost efficient
				//we had to do the whole calculation and not just compare the GCost / GTime ratio
				//because changing them affect the Dest City and AStarF
				continue;
			}
			OpenHeap.editElement(ThisNode, -AStarF);
		}
		SetAStarStatus(ThisNode, AStar_Open);
		SetAStarF(ThisNode, AStarF);
		SetAStarGCost(ThisNode, AStarGCost);
		SetAStarGTime(ThisNode, AStarGTime);
		SetAStarParent(ThisNode, BestNode);
		SetAStarCity(ThisNode, bestCity);
	}

	return;
}//end ExecuteAStarLogic

bool AStarPathfinder::isNodeDestCity(const CSmallPoint& BestNode,
									 const vector<AStarHeap>& acceptableDestCities)
{
	for each (AStarHeap city in acceptableDestCities) {
		if (city.data == BestNode) return true;
	}
	return false;
}//end isNodeDestCity

float AStarPathfinder::CalcHTime(const CSmallPoint& start, const CSmallPoint& dest)
{
	int x = abs(start.x - dest.x);
	int y = abs(start.y - dest.y);
	int diagonal, straight;
	if (x > y) {
		diagonal = x;
		straight = x - y;
	}
	else {
		diagonal = y;
		straight = y - x;
	}
	//this can be made more exact by incorporating the traversal time
	//of the start half of the next square, * 1.42f to be sure
	return ((diagonal * 1.42f + straight) * MAX_TRAVERSAL_TIME);
}//end CalcHTime

float AStarPathfinder::CalcGCost(const CSmallPoint& start, const CSmallPoint& dest, const ePlayer player)
{
	return 0.0f;
}//end CalcGCost

float AStarPathfinder::CalcGTime(const CSmallPoint& start, const CSmallPoint& dest)
{
	//traversal costs are half the cost of traversing the whole node
	//we are going from midpoint to midpoint
	float traversalTime = CWorldManager::GetTerrainType(start) + CWorldManager::GetTerrainType(dest);
	if (abs(start.x - dest.x) + abs(start.y + dest.y) == 2) traversalTime *= 1.42f;
	return (traversalTime);
}//end CalcGTime
