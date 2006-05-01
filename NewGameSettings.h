#ifndef ___NEW_GAME_SETTINGS___
#define ___NEW_GAME_SETTINGS___

//extern BOOLEAN gfUseABResources;
//extern BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents;
//extern UINT32 guiMaxTossSearchDist;
//extern BOOLEAN gfShowItemShadow;

extern BOOLEAN gfAllowMilitiaGroups;
extern BOOLEAN gfAllowReinforcments;
extern BOOLEAN gfAllowReinforcmentsOnlyInCity;
extern UINT32 guiBaseQueenPoolIncrement;
extern UINT8 ubStraightSightRange;

void NSSaveSettings();
void NSLoadSettings();

#endif