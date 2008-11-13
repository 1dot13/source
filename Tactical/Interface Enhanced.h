#ifndef __INTERFACE_ENHANCED_H
#define __INTERFACE_ENHANCED_H

extern INV_DESC_STATS gMiscItemStatsEDB[];

UINT8 UsingEDBSystem();
void InitItemDescriptionBoxOffsets();
BOOLEAN InternalInitEnhancedDescBox();
void InternalInitEDBTooltipRegion( OBJECTTYPE * gpItemDescObject, UINT32 guiCurrentItemDescriptionScreen );
void DeleteEnhancedDescBox( UINT32 guiCurrentItemDescriptionScreen );
void InitDescStatCoords();
void InitLBECoords();
void InitEDBCoords();

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
