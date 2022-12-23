#ifndef _STRATEGIC_TRANSPORT_GROUPS_H
#define _STRATEGIC_TRANSPORT_GROUPS_H

#include <map>
#include "Types.h"

struct GROUP;

BOOLEAN DeployTransportGroup(INT16);
BOOLEAN ReturnTransportGroup(INT32);
void ProcessTransportGroupReachedDestination(GROUP* pGroup);
void UpdateTransportGroupInventory(std::map<UINT8, std::map<int, UINT8>> &groupIdToSoldierMap);

extern std::map<UINT8, std::map<int, UINT8>> gTransportGroupIdToSoldierMap; // groupId -> soldierclass, count

#endif
