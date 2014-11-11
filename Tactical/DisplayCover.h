#ifndef _DISPLAY_COVER__H_
#define _DISPLAY_COVER__H_

// resets the overlay modes so that everything will be reset at the NEXT display cycle
void ResetOverlayModes();

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo );
void DisplayCover( BOOLEAN forceUpdate = FALSE);

void SwitchToMercView();
void SwitchToEnemyView();
void SwitchViewOff();

void ToggleMercView();
void ToggleEnemyView();

void	CalculateCoverForSoldier( SOLDIERTYPE* pForSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover );
void	CalculateCoverFromSoldier( SOLDIERTYPE* pFromSoldier, const INT32& sTargetGridNo, const BOOLEAN& fRoof, INT8& bCover, SOLDIERTYPE* pToSoldier = NULL );
BOOLEAN CanSoldierSeeFloor( SOLDIERTYPE* pSoldier, INT32 sGridNo, INT8 bLevel );

// Flugente: all different overlays in one enum
enum OVERLAY_VALUES
{
	INVALID_COVER = -1,
	NO_COVER,
	MIN_COVER,
	MED_COVER,
	MAX_COVER,

	NO_SEE,
	MIN_SEE,
	MED_SEE,
	MAX_SEE,

	MINES_ALL,
	MINE_BOMB,
	MINE_WIRE,
	MINE_BOMB_AND_WIRE,
	MINES_NET_1,
	MINES_NET_2,
	MINES_NET_3,
	MINES_NET_4,
	MINES_LVL_1,
	MINES_LVL_2,
	MINES_LVL_3,
	MINES_LVL_4,
	MAX_MINES,

	TRAIT_ALL,
	TRAIT_1,
	TRAIT_2,
	MAX_TRAIT,

	TRACKS_VERYOLD,
	TRACKS_OLD,
	TRACKS_RECENT,
	TRACKS_BLOOD,
	MAX_TRACKS,
};

enum COVER_DRAW_MODE {
	COVER_DRAW_OFF,
	COVER_DRAW_MERC_VIEW,
	COVER_DRAW_ENEMY_VIEW
};

void ChangeSizeOfDisplayCover( INT32 iNewSize );

void ChangeSizeOfLOS( INT32 iNewSize );
//ddd for optimize oppponent turn
INT8	CalcCoverForGridNoBasedOnTeamKnownEnemies( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo, INT8 bStance );
//ddd for optimize oppponent turn


// ----------------------------- Mines display after this ----------------------------------------
// added by Flugente: display of mine, bombs and tripwire
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

void SwitchToTrapNetworkView();
void SwitchToHostileTrapsView();
void SwitchMineViewOff();

void ToggleTrapNetworkView();
void ToggleHostileTrapsView();

void SwitchMinesDrawModeForNetworks();

// ----------------------------- trait range display after this ----------------------------------------
// added by Flugente: display of ranges and areas of effects for traits
// Flugente: trait draw mode
enum TRAIT_DRAW_MODE {
	TRAIT_DRAW_OFF,
	TRAIT_DRAW_RANGE,
	TRAIT_DRAW_MAX
};

void ToggleTraitRangeView(BOOLEAN fOn = TRUE);
void SetTraitToDisplay( UINT32 aTrait );
void SetGridNoForTraitDisplay( INT32 sGridNo );

// ----------------------------- tracker display after this ----------------------------------------
// added by Flugente: display of ranges and areas of effects for traits

// Flugente: trait draw mode
enum TRACKER_DRAW_MODE {
	TRACKER_DRAW_OFF,
	TRACKER_DRAW_SMELL,
	TRACKER_DRAW_MAX
};

extern TRACKER_DRAW_MODE gubDrawModeTracker;

#endif
