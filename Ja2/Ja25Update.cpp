#include <types.h>
#ifdef NETWORKED
#include "Networking.h"
#include "NetworkEvent.h"
#endif

#ifdef JA2UB


INT16 JA2_5_START_SECTOR_X;
INT16 JA2_5_START_SECTOR_Y;

//** Defines *******************************************************************

//Starting money
#define JA2_5_STARTING_MONEY_EASY			40000
#define JA2_5_STARTING_MONEY_MEDIUM		35000
#define JA2_5_STARTING_MONEY_HARD			30000


#define	JA2SAVE_CONV_FILENAME		"..\\SavedGames\\Ja2.dts"

typedef struct
{
	INT8		bMercID;
	INT8		bPercentStatIncrease;
} STAT_CHANGE;

#define	NUM_MERCS_TO_USE_IN_PERCENT_STAT_INCREASE			6

#define	MAX_STAT_INCREASE	30

#endif														

//******************************************************************************
