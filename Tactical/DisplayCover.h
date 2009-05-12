#ifndef _DISPLAY_COVER__H_
#define _DISPLAY_COVER__H_

void	DisplayCoverOfSelectedGridNo( );
void	RemoveCoverOfSelectedGridNo();

void DisplayRangeToTarget( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo );


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

#endif

