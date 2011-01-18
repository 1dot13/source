#ifndef __INTERFACE_ENHANCED_H
#define __INTERFACE_ENHANCED_H

extern INV_DESC_STATS gMiscItemStatsEDB[];

UINT8 UsingEDBSystem();
void InitItemDescriptionBoxOffsets(OBJECTTYPE * gpItemDescObject);
BOOLEAN InternalInitEnhancedDescBox();
void InternalInitEDBTooltipRegion( OBJECTTYPE * gpItemDescObject, UINT32 guiCurrentItemDescriptionScreen );
void DeleteEnhancedDescBox( UINT32 guiCurrentItemDescriptionScreen );
void InitDescStatCoords(OBJECTTYPE *pObject);
void InitLBECoords();
void InitEDBCoords(OBJECTTYPE * pObject);

// HEADROCK HAM 4: UDB Secondary Stats column icons (no values)
void DrawSecondaryStats( OBJECTTYPE * gpItemDescObject );
void DrawAdvancedStats( OBJECTTYPE * gpItemDescObject );
void DrawAdvancedValues( OBJECTTYPE *gpItemDescObject );
void DrawWeaponStats( OBJECTTYPE * gpItemDescObject );
void DrawAmmoStats( OBJECTTYPE * gpItemDescObject );
void DrawExplosiveStats( OBJECTTYPE * gpItemDescObject );
void DrawArmorStats( OBJECTTYPE * gpItemDescObject );
void DrawMiscStats( OBJECTTYPE * gpItemDescObject );

void DrawWeaponValues( OBJECTTYPE * gpItemDescObject );
void DrawAmmoValues( OBJECTTYPE * gpItemDescObject, int shotsLeft );
void DrawExplosiveValues( OBJECTTYPE * gpItemDescObject );
void DrawArmorValues( OBJECTTYPE * gpItemDescObject );
void DrawMiscValues( OBJECTTYPE * gpItemDescObject );

#endif
