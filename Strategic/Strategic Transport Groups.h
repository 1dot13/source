#ifndef _STRATEGIC_TRANSPORT_GROUPS_H
#define _STRATEGIC_TRANSPORT_GROUPS_H

#include "Campaign Types.h"
#include "Types.h"

struct GROUP;

BOOLEAN DeployTransportGroup();
BOOLEAN ReturnTransportGroup(INT32);
void FillMapColoursForTransportGroups(INT32(&colorMap)[MAXIMUM_VALID_Y_COORDINATE][MAXIMUM_VALID_X_COORDINATE]);
void ProcessTransportGroupReachedDestination(GROUP* pGroup);
void UpdateTransportGroupInventory();

void AddToTransportGroupMap(UINT8 groupId, int soldierClass, UINT8 amount);
void ClearTransportGroupMap();

void NotifyTransportGroupDefeated();

#endif
