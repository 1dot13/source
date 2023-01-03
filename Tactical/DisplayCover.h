#ifndef _DISPLAY_COVER__H_
#define _DISPLAY_COVER__H_

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo );
void DisplayCover( BOOLEAN forceUpdate = FALSE);

void SwitchToMercView();
void SwitchToEnemyView();
void SwitchViewOff();

void ToggleMercView();
void ToggleEnemyView();

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

	FORTIFICATIONNODE_REMOVE,
	FORTIFICATIONNODE_ADJACENTADJUSTMENT,
	FORTIFICATIONNODE_NORMAL,
	MAX_FORTIFICATION,
};

enum COVER_DRAW_MODE {
	DRAW_MODE_OFF,

	COVER_DRAW_MERC_VIEW,
	COVER_DRAW_ENEMY_VIEW,

	MINES_DRAW_DETECT_ENEMY,
	MINES_DRAW_PLAYERTEAM_NETWORKS,
	MINES_DRAW_NETWORKCOLOURING,
	MINES_DRAW_NET_A,
	MINES_DRAW_NET_B,
	MINES_DRAW_NET_C,
	MINES_DRAW_NET_D,

	DRAW_MODE_TRAIT_RANGE,

	DRAW_MODE_TRACKER_SMELL,

	DRAW_MODE_FORTIFY,

	DRAW_MODE_WEAPONDATA,
};


// ----------------------------- Mines display after this ----------------------------------------
// added by Flugente: display of mine, bombs and tripwire

void SwitchToTrapNetworkView();
void SwitchToHostileTrapsView();
void SwitchMineViewOff();

void ToggleTrapNetworkView();
void ToggleHostileTrapsView();

// ----------------------------- trait range display after this ----------------------------------------
// added by Flugente: display of ranges and areas of effects for traits

void ToggleTraitRangeView(BOOLEAN fOn = TRUE);
void SetTraitToDisplay( UINT32 aTrait );
void SetGridNoForTraitDisplay( INT32 sGridNo );

#endif
