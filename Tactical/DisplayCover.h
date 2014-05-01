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

void	CalculateCoverForSoldier( SOLDIERTYPE* pForSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover );
void	CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover, SOLDIERTYPE* pToSoldier = NULL );


#define		DC_MAX_COVER_RANGE					43 //31
#define		DC__SOLDIER_VISIBLE_RANGE			43 //31
#define		DC__MIN_SIZE						4
#define		DC__MAX_SIZE						21 //11

enum COVER_VALUES
{
	INVALID_COVER = -1,
	NO_COVER = 0,
	MIN_COVER = 1,
	MED_COVER = 2,
	MAX_COVER = 3
};

// Flugente: mines display - stuff needs to be here
// Sevenfm: moved here from DisplayCover.cpp
enum MINES_DRAW_MODE {
	MINES_DRAW_OFF,
	MINES_DRAW_DETECT_ENEMY,
	MINES_DRAW_PLAYERTEAM_NETWORKS,
	MINES_DRAW_NETWORKCOLOURING,
	MINES_DRAW_NET_A,
	MINES_DRAW_NET_B,
	MINES_DRAW_NET_C,
	MINES_DRAW_NET_D,
	MINES_DRAW_MAX
};

// Flugente: trait draw mode
enum TRAIT_DRAW_MODE {
	TRAIT_DRAW_OFF,
	TRAIT_DRAW_RANGE,
	TRAIT_DRAW_MAX
};

enum COVER_DRAW_MODE {
	COVER_DRAW_OFF,
	COVER_DRAW_MERC_VIEW,
	COVER_DRAW_ENEMY_VIEW
};

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

// ----------------------------- trait range display after this ----------------------------------------
// added by Flugente: display of ranges and areas of effects for traits
void DisplayTraitRange(const BOOLEAN& forceUpdate = FALSE);

void ToggleTraitRangeView(BOOLEAN fOn = TRUE);
void SetTraitToDisplay( UINT32 aTrait );
void SetGridNoForTraitDisplay( INT32 sGridNo );

#endif
