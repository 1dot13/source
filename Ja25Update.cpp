#include "builddefines.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "Render Fun.h"
#include "stdlib.h"
#include "debug.h"
#include "MemMan.h"
#include "Overhead Types.h"
//#include "Soldier Control.h"
#include "Animation Cache.h"
#include "Animation Data.h"
#include "Animation Control.h"
#include "container.h"
#include <math.h>
#include "pathai.h"
#include "Random.h"
#include "Isometric Utils.h"
#include "renderworld.h"
#include "video.h"
#include "weapons.h"
#include "vobject_blitters.h"
#include "Handle UI.h"
#include "Event pump.h"
#include "ai.h"
#include "interface.h"
#include "lighting.h"
#include "faces.h"
#include "Soldier Profile.h"
#include "soldier macros.h"
#include "english.h"
#include "Squads.h"

#ifdef NETWORKED
#include "Networking.h"
#include "NetworkEvent.h"
#endif
#include "items.h"
#include "Soundman.h"
#include "utilities.h"
#include "Strategic.h"
#include "soldier tile.h"
#include "Smell.h"
#include "Keys.h"
#include "dialogue control.h"
#include "soldier functions.h"
#include "Exit Grids.h"
#include "Quests.h"
#include "message.h"
#include "SkillCheck.h"
#include "interface dialogue.h"
#include	"GameSettings.h"
#include "ShopKeeper Interface.h"
#include "vehicles.h"
#include "strategicmap.h"
#include "morale.h"
#include "drugs and alcohol.h"
#include "SkillCheck.h"
#include "Map Information.h"
#include "Buildings.h"
#include "Text.h"
#include "Campaign Types.h"
#include "Strategic Status.h"
#include "civ quotes.h"
#include "Debug Control.h"

#ifdef JA2UB

#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"

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
