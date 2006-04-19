#ifndef ___NEW_GAME_SETTINGS___
#define ___NEW_GAME_SETTINGS___

extern BOOLEAN gfUseABResources;
extern BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents;
extern UINT32 guiMaxTossSearchDist;
extern BOOLEAN gfShowItemShadow;

void NSSaveSettings();
void NSLoadSettings();

#endif