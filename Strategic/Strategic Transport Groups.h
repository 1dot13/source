#ifndef _STRATEGIC_TRANSPORT_GROUPS_H
#define _STRATEGIC_TRANSPORT_GROUPS_H

#include <map>
#include "Types.h"

struct GROUP;

BOOLEAN DeployTransportGroup(INT16);
BOOLEAN ReturnTransportGroup(INT32);
void ProcessTransportGroupReachedDestination(GROUP* pGroup);
void UpdateTransportGroupInventory();

void AddToTransportGroupMap(UINT8 groupId, int soldierClass, UINT8 amount);
void ClearTransportGroupMap();

#endif
