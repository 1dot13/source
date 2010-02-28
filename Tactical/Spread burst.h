#ifndef _SPREAD_BURST_H
#define _SPREAD_BURST_H

#define		MAX_BURST_LOCATIONS		50

typedef struct
{
	INT16 sX;
	INT16 sY;
	INT32 sGridNo;

} BURST_LOCATIONS;


extern BURST_LOCATIONS			gsBurstLocations[ MAX_BURST_LOCATIONS ];
extern INT8					gbNumBurstLocations;


void ResetBurstLocations( );
void AccumulateBurstLocation( INT32 sGridNo );
void PickBurstLocations( SOLDIERTYPE *pSoldier );
void AIPickBurstLocations( SOLDIERTYPE *pSoldier, INT8 bTargets, SOLDIERTYPE *pTargets[5] );

void RenderAccumulatedBurstLocations( );

#endif