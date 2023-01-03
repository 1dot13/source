#ifndef __TACTICAL_PLACEMENT_GUI_H
#define __TACTICAL_PLACEMENT_GUI_H

//#include "Soldier Control.h"
//#include "mousesystem.h"
typedef struct _MOUSE_REGION MOUSE_REGION;
class SOLDIERTYPE;

void InitTacticalPlacementGUI();
void KillTacticalPlacementGUI();
void TacticalPlacementHandle();
void RenderTacticalPlacementGUI();

void HandleTacticalPlacementClicksInOverheadMap( MOUSE_REGION *reg, INT32 reason );

// WANNE - MP: This method checks, if the desired entry direction (N, E, S, W) on the map is valid. If not it chooses the next valid diretion
UINT8 GetValidInsertionDirectionForMP(UINT8	currentInsertionPoint);

extern BOOLEAN gfTacticalPlacementGUIActive;
extern BOOLEAN gfEnterTacticalPlacementGUI;
extern BOOLEAN gfTacticalPlacementGUIDirty;//dnl ch45 071009

extern SOLDIERTYPE *gpTacticalPlacementSelectedSoldier;
extern SOLDIERTYPE *gpTacticalPlacementHilightedSoldier;

//Saved value.	Contains the last choice for future battles.
extern UINT8	gubDefaultButton;

#endif