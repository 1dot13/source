#ifndef _STRATEGIC_TRANSPORT_GROUPS_H
#define _STRATEGIC_TRANSPORT_GROUPS_H

#include "Campaign Types.h"
#include "Types.h"
#include <map>

enum TransportGroupSectorInfo
{
	TransportGroupSectorInfo_LocatedGroup = 0,
	TransportGroupSectorInfo_LocatedDestination,
};

struct GROUP;

BOOLEAN DeployTransportGroup();
BOOLEAN ForceDeployTransportGroup(UINT8 sectorId);
BOOLEAN ReturnTransportGroup(INT32 groupId);
void FillMapColoursForTransportGroups(INT32(&colorMap)[MAXIMUM_VALID_Y_COORDINATE][MAXIMUM_VALID_X_COORDINATE]);
void ProcessTransportGroupReachedDestination(GROUP* pGroup);
void UpdateTransportGroupInventory();

const std::map<UINT8, TransportGroupSectorInfo> GetTransportGroupSectorInfo();

void AddToTransportGroupMap(UINT8 groupId, int soldierClass, UINT16 amount);
void ClearTransportGroupMap();

void NotifyTransportGroupDefeated();

#endif

