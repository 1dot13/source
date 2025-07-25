#include "builddefines.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "Render Fun.h"
#include "stdlib.h"
#include "DEBUG.H"
#include "MemMan.h"
#include "Overhead Types.h"
//#include "Soldier Control.h"
#include "Animation Cache.h"
#include "Animation Data.h"
#include "Animation Control.h"
#include <math.h>
#include "PATHAI.H"
#include "random.h"
#include "Isometric Utils.h"
#include "renderworld.h"
#include "video.h"
#include "Weapons.h"
#include "vobject_blitters.h"
#include "Handle UI.h"
#include "Event Pump.h"
#include "ai.h"
#include "Interface.h"
#include "lighting.h"
#include "faces.h"
#include "Soldier Profile.h"
#include "Soldier macros.h"
#include "english.h"
#include "Squads.h"

#ifdef NETWORKED
#include "Networking.h"
#include "NetworkEvent.h"
#endif
#include "Items.h"
#include "soundman.h"
#include "Utilities.h"
#include "strategic.h"
#include "soldier tile.h"
#include "Smell.h"
#include "Keys.h"
#include "Dialogue Control.h"
#include "Soldier Functions.h"
#include "Exit Grids.h"
#include "Quests.h"
#include "message.h"
#include "SkillCheck.h"
#include "interface Dialogue.h"
#include	"GameSettings.h"
#include "ShopKeeper Interface.h"
#include "Vehicles.h"
#include "strategicmap.h"
#include "Morale.h"
#include "Drugs And Alcohol.h"
#include "SkillCheck.h"
#include "Map Information.h"
#include "Buildings.h"
#include "Text.h"
#include "Campaign Types.h"
#include "Strategic Status.h"
#include "Civ Quotes.h"
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
