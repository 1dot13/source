#ifndef __INTERFACE_CONTROL_H
#define __INTERFACE_CONTROL_H


#define		INTERFACE_MAPSCREEN							0x00000001
#define		INTERFACE_NORENDERBUTTONS				0x00000002
#define		INTERFACE_LOCKEDLEVEL1					0x00000004
#define		INTERFACE_SHOPKEEP_INTERFACE		0x00000008

extern UINT32		guiTacticalInterfaceFlags;


void SetTacticalInterfaceFlags( UINT32 uiFlags );

void SetUpInterface( );
void ResetInterface( );
void RenderTopmostTacticalInterface( );
void RenderTacticalInterface( );

// Flugente: handle flashlights
void HandleFlashLights();

void StartViewportOverlays( );
void EndViewportOverlays( );

void LockTacticalInterface( );
void UnLockTacticalInterface( );

void RenderTacticalInterfaceWhileScrolling( );

void EraseInterfaceMenus( BOOLEAN fIgnoreUIUnLock );

// handle paused render of tactical panel, if flag set, OR it in with tactical render flags
// then reset
void HandlePausedTacticalRender( void );

void ResetInterfaceAndUI( );

BOOLEAN AreWeInAUIMenu( );

void HandleTacticalPanelSwitch( );

BOOLEAN InterfaceOKForMeanwhilePopup();

// sevenfm: these values are used to calculate scale for displaying values as asterisks (no more than 5 asterisks for any value)
enum{ 
	PRINT_SCALE_PLAIN_NUMBER, 
	PRINT_SCALE_ASTERISK, 
	PRINT_SCALE_ASTERISK_SUPPRESSION, 
	PRINT_SCALE_ASTERISK_DAMAGE, 
	PRINT_SCALE_ASTERISK_MORALE,
	PRINT_SCALE_ASTERISK_AP,
	PRINT_SCALE_ASTERISK_SHOCK,
};

UINT8 CalcScaleSuppression( INT16 data );
UINT8 CalcScaleDamage( INT16 data );
UINT8 CalcScaleMorale( INT16 data );
UINT8 CalcScaleAP( INT16 data );
UINT8 CalcScaleShock( INT16 data );

void DrawCounters( SOLDIERTYPE *pSoldier );

// print counter as number or asterisks ( applying provided scale when displaying asterisks )
// width is updated as X offset after printing
void PrintCounter( INT16 x, INT16 y, INT16 data, UINT16 &width, UINT8 ubForegound, UINT8 scale = PRINT_SCALE_PLAIN_NUMBER );
void PrintSuppressionCounter( INT16 x, INT16 y, INT16 sX, INT16 sY, UINT8 data, UINT16 &widthDamage, UINT16 &widthSuppression, UINT8 ubForeground, UINT8 scale, UINT8 option);

// Flugente: draw moving circles around a gridno. This is used to warn the player of impending explosions
void DrawExplosionWarning( INT32 sGridno, INT8 sLevel, INT8 sDelay );

// Flugente: draw a circle around a gridno
// For now, we aren't using usColour, but that will likely change in the future
void DrawTraitRadius( INT32 sGridno, INT8 sLevel, INT32 sRadius, INT16 sThickness, UINT16 usColour );

#endif
