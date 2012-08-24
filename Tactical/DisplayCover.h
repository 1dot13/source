#ifndef _DISPLAY_COVER__H_
#define _DISPLAY_COVER__H_

void	DisplayCoverOfSelectedGridNo( );
void	RemoveCoverOfSelectedGridNo();

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo );
void DisplayCover(const BOOLEAN& forceUpdate = FALSE);

void SwitchToMercView();
void SwitchToEnemyView();
void SwitchViewOff();

void ToggleMercView();
void ToggleEnemyView();

void SwitchCoverDrawMode();

void RemoveVisibleGridNoAtSelectedGridNo();
void DisplayGridNoVisibleToSoldierGrid( );


#define		DC_MAX_COVER_RANGE					43 //31
#define		DC__SOLDIER_VISIBLE_RANGE			43 //31
#define		DC__MIN_SIZE						4
#define		DC__MAX_SIZE						21 //11

#ifdef JA2TESTVERSION
	void DisplayLosAndDisplayCoverUsageScreenMsg();
#endif

void ChangeSizeOfDisplayCover( INT32 iNewSize );

void ChangeSizeOfLOS( INT32 iNewSize );
//ddd for optimize oppponent turn
INT8	CalcCoverForGridNoBasedOnTeamKnownEnemies( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, INT8 bStance );
//ddd for optimize oppponent turn


// ----------------------------- Mines display after this ----------------------------------------
// added by Flugente: display of mine, bombs and tripwire
void DisplayMines(const BOOLEAN& forceUpdate = FALSE);

void SwitchToTrapNetworkView();
void SwitchToHostileTrapsView();
void SwitchMineViewOff();

void ToggleTrapNetworkView();
void ToggleHostileTrapsView();

void SwitchMinesDrawModeForNetworks();


#endif
