#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "types.h"
	#include "fileman.h"
	#include "himage.h"
	#include "Creature Spreading.h"
	#include "Campaign Types.h"
	#include "Strategic Movement.h"
	#include "Game Event Hook.h"
	#include "GameSettings.h"
	#include "Random.h"
	#include "message.h"
	#include "Font Control.h"
	#include "Soldier Init List.h"
	#include "lighting.h"
	#include "strategicmap.h"
	#include "Game Clock.h"
	#include "Strategic Mines.h"
	#include "Music Control.h"
	#include "strategic.h"
	#include "jascreens.h"
	#include "Town Militia.h"
	#include "Strategic Town Loyalty.h"
	#include "PreBattle Interface.h"
	#include "Map Edgepoints.h"
	#include "Animation Data.h"
	#include "opplist.h"
	#include "meanwhile.h"
	#include "Strategic AI.h"
	#include "MessageBoxScreen.h"
	#include "Map Information.h"
	#include "Queen Command.h"
	#include "ai.h"					// added by Flugente
	#include "Isometric Utils.h"	// added by Flugente
	#include "Soldier Create.h"		// added by Flugente
	#include "Player Command.h"		// added by Flugente
#endif

#include "Strategic Mines.h"
#include "connect.h"

#include "GameInitOptionsScreen.h"

#ifdef JA2BETAVERSION
	BOOLEAN gfClearCreatureQuest = FALSE;
	extern UINT32 uiMeanWhileFlags;
#endif

//GAME BALANCING DEFINITIONS FOR CREATURE SPREADING
//Hopefully, adjusting these following definitions will ease the balancing of the
//creature spreading.
//The one note here is that for any definitions that have a XXX_BONUS at the end of a definition,
//it gets added on to it's counterpart via:
//								XXX_VALUE + Random( 1 + XXX_BONUS )

//The start day random bonus that the queen begins
//#define EASY_QUEEN_START_DAY								10	//easy start day 10-13
//#define EASY_QUEEN_START_BONUS							3
//#define NORMAL_QUEEN_START_DAY							8		//normal start day 8-10
//#define NORMAL_QUEEN_START_BONUS						2
//#define HARD_QUEEN_START_DAY								7		//hard start day 7-8
//#define HARD_QUEEN_START_BONUS							1

//This is how often the creatures spread, once the quest begins.	The smaller the gap,
//the faster the creatures will advance.	This is also directly related to the reproduction
//rates which are applied each time the creatures spread.
//#define EASY_SPREAD_TIME_IN_MINUTES					gCreaturesSettings.usCreatureSpreadTimeNovice	//easy spreads every 8.5 hours
//#define NORMAL_SPREAD_TIME_IN_MINUTES				gCreaturesSettings.usCreatureSpreadTimeExperienced	//normal spreads every 7.5 hours
//#define HARD_SPREAD_TIME_IN_MINUTES					gCreaturesSettings.usCreatureSpreadTimeExpert	//hard spreads every 6.5 hours
//#define INSANE_SPREAD_TIME_IN_MINUTES				gCreaturesSettings.usCreatureSpreadTimeInsane	//insane spreads every 2.5 hours

//Once the queen is added to the game, we can instantly let her spread x number of times
//to give her a head start.	This can also be a useful tool for having slow reproduction rates
//but quicker head start to compensate to make the creatures less aggressive overall.
//#define EASY_QUEEN_INIT_BONUS_SPREADS				gCreaturesSettings.ubQueenInitBonusSpreadsNovice //1
//#define NORMAL_QUEEN_INIT_BONUS_SPREADS				gCreaturesSettings.ubQueenInitBonusSpreadsExperienced //2
//#define HARD_QUEEN_INIT_BONUS_SPREADS				gCreaturesSettings.ubQueenInitBonusSpreadsExpert //3
//#define INSANE_QUEEN_INIT_BONUS_SPREADS				gCreaturesSettings.ubQueenInitBonusSpreadsInsane //5

//This value modifies the chance to populate a given sector.	This is different from the previous definition.
//This value gets applied to a potentially complicated formula, using the creature habitat to modify
//chance to populate, along with factoring in the relative distance to the hive range (to promote deeper lair
//population increases), etc.	I would recommend not tweaking the value too much in either direction from
//zero due to the fact that this can greatly effect spread times and maximum populations.	Basically, if the
//creatures are spreading too quickly, increase the value, otherwise decrease it to a negative value
//#define EASY_POPULATION_MODIFIER					gCreaturesSettings.bCreaturePopulationModifierNovice //0
//#define NORMAL_POPULATION_MODIFIER					gCreaturesSettings.bCreaturePopulationModifierExperienced //0
//#define HARD_POPULATION_MODIFIER					gCreaturesSettings.bCreaturePopulationModifierExpert //0
//#define INSANE_POPULATION_MODIFIER					gCreaturesSettings.bCreaturePopulationModifierInsane //0

//Augments the chance that the creatures will attack a town.	The conditions for attacking a town
//are based strictly on the occupation of the creatures in each of the four mine exits.	For each creature
//there is a base chance of 10% that the creatures will feed sometime during the night.
//#define EASY_CREATURE_TOWN_AGGRESSIVENESS		gCreaturesSettings.bCreatureTownAggressivenessNovice //-10
//#define NORMAL_CREATURE_TOWN_AGGRESSIVENESS		gCreaturesSettings.bCreatureTownAggressivenessExperienced //0
//#define HARD_CREATURE_TOWN_AGGRESSIVENESS		gCreaturesSettings.bCreatureTownAggressivenessExpert //10
//#define INSANE_CREATURE_TOWN_AGGRESSIVENESS		gCreaturesSettings.bCreatureTownAggressivenessInsane //50


//This is how many creatures the queen produces for each cycle of spreading.	The higher
//the numbers the faster the creatures will advance.
//#define EASY_QUEEN_REPRODUCTION_BASE				gCreaturesSettings.ubQueenReproductionBaseNovice //6 //6-7
//#define EASY_QUEEN_REPRODUCTION_BONUS				gCreaturesSettings.ubQueenReproductionBonusNovice //1
//#define NORMAL_QUEEN_REPRODUCTION_BASE				gCreaturesSettings.ubQueenReproductionBaseExperienced //7 //7-9
//#define NORMAL_QUEEN_REPRODUCTION_BONUS				gCreaturesSettings.ubQueenReproductionBonusExperienced //2
//#define HARD_QUEEN_REPRODUCTION_BASE				gCreaturesSettings.ubQueenReproductionBaseExpert //9 //9-12
//#define HARD_QUEEN_REPRODUCTION_BONUS				gCreaturesSettings.ubQueenReproductionBonusExpert //3
//#define INSANE_QUEEN_REPRODUCTION_BASE				gCreaturesSettings.ubQueenReproductionBaseInsane //15 //15-20
//#define INSANE_QUEEN_REPRODUCTION_BONUS				gCreaturesSettings.ubQueenReproductionBonusInsane //5

//When either in a cave level with blue lights or there is a creature presence, then
//we override the normal music with the creature music.	The conditions are maintained
//inside the function PrepareCreaturesForBattle() in this module.
//BOOLEAN gfUseCreatureMusic = FALSE; // moved to music control.cpp
BOOLEAN gfCreatureMeanwhileScenePlayed = FALSE;

// externalize to xml data
enum
{
	QUEEN_LAIR,				//where the queen lives.	Highly protected
	LAIR,							//part of the queen's lair -- lots of babies and defending mothers
	LAIR_ENTRANCE,		//where the creatures access the mine.
	INNER_MINE,				//parts of the mines that aren't close to the outside world
	OUTER_MINE,				//area's where miners work, close to towns, creatures love to eat :)
	FEEDING_GROUNDS,	//creatures love to populate these sectors :)
	MINE_EXIT,				//the area that creatures can initiate town attacks if lots of monsters.
};

typedef struct CREATURE_DIRECTIVE
{
	struct CREATURE_DIRECTIVE *next;
	UNDERGROUND_SECTORINFO *pLevel;
}CREATURE_DIRECTIVE;

CREATURE_DIRECTIVE *lair;
INT32	giHabitatedDistance = 0;
INT32 giPopulationModifier = 0;
INT32 giLairID = 0;
INT32 giDestroyedLairID = 0;

//various information required for keeping track of the battle sector involved for
//prebattle interface, autoresolve, etc.
INT16 gsCreatureInsertionCode = 0;
INT32 gsCreatureInsertionGridNo = 0;
UINT8 gubNumCreaturesAttackingTown = 0;
UINT8 gubYoungMalesAttackingTown = 0;
UINT8 gubYoungFemalesAttackingTown = 0;
UINT8 gubAdultMalesAttackingTown = 0;
UINT8 gubAdultFemalesAttackingTown = 0;
UINT8 gubCreatureBattleCode = CREATURE_BATTLE_CODE_NONE;
UINT8 gubSectorIDOfCreatureAttack = 0;

// Flugente: bloodcat raids are mostly similar to creature attacks, so use the same code and differ when necessary with this
UINT8 guCreatureAttackType = CREATURE_ATTACK_TYPE_CREATURE;

extern UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );
extern UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );
extern void BuildUndergroundSectorInfoList();
void DeleteCreatureDirectives();

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

//extern MINE_STATUS_TYPE gMineStatus[ MAX_NUMBER_OF_MINES ];


CREATURE_DIRECTIVE* NewDirective( UINT8 ubSectorID, UINT8 ubSectorZ, UINT8 ubCreatureHabitat )
{
	CREATURE_DIRECTIVE *curr;
	UINT8 ubSectorX, ubSectorY;
	curr = (CREATURE_DIRECTIVE*)MemAlloc( sizeof( CREATURE_DIRECTIVE ) );
	Assert( curr );
	ubSectorX = SECTORX( ubSectorID);
	ubSectorY = SECTORY( ubSectorID );
	curr->pLevel = FindUnderGroundSector( ubSectorX, ubSectorY, ubSectorZ );
	if( !curr->pLevel )
	{
		AssertMsg( 0, String( "Could not find underground sector node (%c%db_%d) that should exist.",
			ubSectorY + 'A' - 1, ubSectorX, ubSectorZ ) );
		return 0;
	}

	curr->pLevel->ubCreatureHabitat = ubCreatureHabitat;
	Assert( curr->pLevel );
	curr->next = NULL;
	return curr;
}

/* // externalize to xml data
void InitLairDrassen()
{
	CREATURE_DIRECTIVE *curr;
	giLairID = 1;
	//initialize the linked list of lairs
	lair = NewDirective( SEC_F13, 3, QUEEN_LAIR );
	curr = lair;
	if( !curr->pLevel->ubNumCreatures )
	{
		curr->pLevel->ubNumCreatures = 1;	//for the queen.
	}
	curr->next = NewDirective( SEC_G13, 3, LAIR );
	curr = curr->next;
	curr->next = NewDirective( SEC_G13, 2, LAIR_ENTRANCE );
	curr = curr->next;
	curr->next = NewDirective( SEC_F13, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_E13, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_E13, 1, OUTER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_D13, 1, MINE_EXIT );
}

void InitLairAlma()
{
	CREATURE_DIRECTIVE *curr;
	giLairID = 2;
	//initialize the linked list of lairs
	lair = NewDirective( SEC_K13, 3, QUEEN_LAIR );
	curr = lair;
	if( !curr->pLevel->ubNumCreatures )
	{
		curr->pLevel->ubNumCreatures = 1;	//for the queen.
	}
	curr->next = NewDirective( SEC_J13, 3, LAIR );
	curr = curr->next;
	curr->next = NewDirective( SEC_J13, 2, LAIR_ENTRANCE );
	curr = curr->next;
	curr->next = NewDirective( SEC_J14, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_J14, 1, OUTER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_I14, 1, MINE_EXIT );
}

void InitLairCambria()
{
	CREATURE_DIRECTIVE *curr;
	giLairID = 3;
	//initialize the linked list of lairs
	lair = NewDirective( SEC_J8, 3, QUEEN_LAIR );
	curr = lair;
	if( !curr->pLevel->ubNumCreatures )
	{
		curr->pLevel->ubNumCreatures = 1;	//for the queen.
	}
	curr->next = NewDirective( SEC_I8, 3, LAIR );
	curr = curr->next;
	curr->next = NewDirective( SEC_H8, 3, LAIR );
	curr = curr->next;
	curr->next = NewDirective( SEC_H8, 2, LAIR_ENTRANCE );
	curr = curr->next;
	curr->next = NewDirective( SEC_H9, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_H9, 1, OUTER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_H8, 1, MINE_EXIT );
}

void InitLairGrumm()
{
	CREATURE_DIRECTIVE *curr;
	giLairID = 4;
	//initialize the linked list of lairs
	lair = NewDirective( SEC_G4, 3, QUEEN_LAIR );
	curr = lair;
	if( !curr->pLevel->ubNumCreatures )
	{
		curr->pLevel->ubNumCreatures = 1;	//for the queen.
	}
	curr->next = NewDirective( SEC_H4, 3, LAIR );
	curr = curr->next;
	curr->next = NewDirective( SEC_H4, 2, LAIR_ENTRANCE );
	curr = curr->next;
	curr->next = NewDirective( SEC_H3, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_I3, 2, INNER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_I3, 1, OUTER_MINE );
	curr = curr->next;
	curr->next = NewDirective( SEC_H3, 1, MINE_EXIT );
}
*/

void InitLair(INT32 iChosenMine)
{
	CREATURE_DIRECTIVE *curr;
	giLairID = iChosenMine;

	// initialize the linked list of lairs

	// initialize the queen sector
	lair = NewDirective( SECTOR( gCreaturePlacements[ giLairID ].sQueenX, gCreaturePlacements[ giLairID ].sQueenY ),
		gCreaturePlacements[ giLairID ].ubQueenZ, QUEEN_LAIR );
	curr = lair;	
	
	if( !curr->pLevel->ubNumCreatures )
	{
		curr->pLevel->ubNumCreatures = 1;	//for the queen.
	}
	
	// initialize valid linked creature sectors
	for (UINT8 i = 0; i < MAX_NUMBER_OF_CREATURE_SECTORS; i++)
	{
		if( gCreaturePlacements[ giLairID ].Habitat[ i ].fValid )
		{
			curr->next = NewDirective( SECTOR( gCreaturePlacements[ giLairID ].Habitat[ i ].sX , gCreaturePlacements[ giLairID ].Habitat[ i ].sY ),
				gCreaturePlacements[ giLairID ].Habitat[ i ].ubZ, gCreaturePlacements[ giLairID ].Habitat[ i ].ubComposition );
			curr = curr->next;
		}
	}
}

#ifdef JA2BETAVERSION
extern BOOLEAN gfExitViewer;
#endif

void InitCreatureQuest()
{
	UNDERGROUND_SECTORINFO *curr;
	BOOLEAN fPlayMeanwhile = FALSE;
	INT32 i=-1;
	INT32 x=0;
	INT32 iChosenMine;
	INT32 iRandom;
	INT32 iNumMinesInfectible=0;
	INT32 iNumMinesInfectibleLUA=0;
	#ifdef JA2BETAVERSION
		INT32 iOrigRandom;
	#endif
	BOOLEAN fMineInfectible[MAX_NUMBER_OF_INFECTIBLE_SITES];

	if( giLairID )
	{
		return; //already active!
	}

	#ifdef JA2BETAVERSION
	if( guiCurrentScreen != AIVIEWER_SCREEN )
	{
		fPlayMeanwhile = TRUE;
	}
	#else
		fPlayMeanwhile = TRUE;
	#endif

#ifdef JA2UB 
//Ja25 No meanwhiles && no creatures
#else
	if( fPlayMeanwhile && !gfCreatureMeanwhileScenePlayed && gModSettings.CreatureMeanwhileCutscene == TRUE )
	{
		//Start the meanwhile scene for the queen ordering the release of the creatures.
		HandleCreatureRelease();
		gfCreatureMeanwhileScenePlayed = TRUE;
	}
#endif
	giHabitatedDistance = 0;
	
	giPopulationModifier = zDiffSetting[gGameOptions.ubDifficultyLevel].iCreaturePopulationModifier;
		
	//Determine which of the maps are infectible by creatures.	Infectible mines
	//are those that are player controlled and unlimited.	We don't want the creatures to
	//infect the mine that runs out.

	//Default them all to infectible
	memset( fMineInfectible, 1, sizeof( BOOLEAN ) * MAX_NUMBER_OF_INFECTIBLE_SITES );
	
	/* // externalize to xml data
	if( gMineStatus[ MINE_DRASSEN ].fAttackedHeadMiner ||
			//gMineStatus[ MINE_DRASSEN ].uiOreRunningOutPoint ||
			!gMineStatus[ MINE_DRASSEN ].fInfectible ||
			//StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_D13 ) ].fEnemyControlled
			StrategicMap[ gMineStatus[MINE_DRASSEN].StrategicIndex() ].fEnemyControlled
			)
	{ //If head miner was attacked, ore will/has run out, or enemy controlled
		fMineInfectible[ 0 ] = FALSE;
	}
	if( gMineStatus[ MINE_CAMBRIA ].fAttackedHeadMiner ||
			//gMineStatus[ MINE_CAMBRIA ].uiOreRunningOutPoint ||
			!gMineStatus[ MINE_CAMBRIA ].fInfectible ||
			//StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_H8 ) ].fEnemyControlled )
			StrategicMap[ gMineStatus[MINE_CAMBRIA].StrategicIndex() ].fEnemyControlled )
	{ //If head miner was attacked, ore will/has run out, or enemy controlled
		fMineInfectible[ 1 ] = FALSE;
	}
	if( gMineStatus[ MINE_ALMA ].fAttackedHeadMiner ||
			//gMineStatus[ MINE_ALMA ].uiOreRunningOutPoint ||
			!gMineStatus[ MINE_ALMA ].fInfectible ||
			//StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_I14 ) ].fEnemyControlled )
			StrategicMap[ gMineStatus[MINE_ALMA].StrategicIndex() ].fEnemyControlled )
	{ //If head miner was attacked, ore will/has run out, or enemy controlled
		fMineInfectible[ 2 ] = FALSE;
	}
	if( gMineStatus[ MINE_GRUMM ].fAttackedHeadMiner ||
			//gMineStatus[ MINE_GRUMM ].uiOreRunningOutPoint ||
			!gMineStatus[ MINE_GRUMM ].fInfectible ||
			//StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_H3 ) ].fEnemyControlled )
			StrategicMap[ gMineStatus[MINE_GRUMM].StrategicIndex() ].fEnemyControlled )
	{ //If head miner was attacked, ore will/has run out, or enemy controlled
		fMineInfectible[ 3 ] = FALSE;
	}
	*/
	
	// determine mine infectible status in initmines.lua script
	for (x = 0; x < MAX_NUMBER_OF_MINES; ++x)
	{
		if( gMineStatus[ x ].fInfectible )
		{
			if( gMineStatus[ x ].fAttackedHeadMiner ||
				//gMineStatus[ x ].uiOreRunningOutPoint ||
				StrategicMap[ gMineStatus[ x ].StrategicIndex() ].fEnemyControlled )
			{
				//If head miner was attacked, ore will/has run out, or enemy controlled
				fMineInfectible[ iNumMinesInfectibleLUA ] = FALSE;
			}
			iNumMinesInfectibleLUA++;
		}
	}

	#ifdef JA2BETAVERSION
	if( guiCurrentScreen == AIVIEWER_SCREEN )
	{ //If in the AIViewer, allow any mine to get infected
		memset( fMineInfectible, 1, sizeof( BOOLEAN ) * NUMBER_OF_INFECTIBLE_SITES );
	}
	#endif
	
	//// externalize to xml data
	//iNumMinesInfectible = fMineInfectible[0] + fMineInfectible[1] + fMineInfectible[2] + fMineInfectible[3];
	
	//count actual infectible sites, use min of infectible sites defined in xml and initmines.lua script in case they do not tally
	for (x = 0; x < min( NUMBER_OF_INFECTIBLE_SITES, iNumMinesInfectibleLUA ); ++x)
	{
		iNumMinesInfectible += fMineInfectible[x];
	}

	if( !iNumMinesInfectible )
	{
		return;
	}

	//Choose one of the infectible mines randomly
	iRandom = Random( iNumMinesInfectible ) + 1;

	#ifdef JA2BETAVERSION
		iOrigRandom = iRandom;
	#endif

	iChosenMine = 0;

	for( x = 0; x < min( NUMBER_OF_INFECTIBLE_SITES, iNumMinesInfectibleLUA ); ++x )
	{
		if( iRandom )
		{
			iChosenMine++;
			if( fMineInfectible[x] )
			{
				--iRandom;
			}
		}
	}

	/* // externalize to xml data
	//Now, choose a start location for the queen.
	switch( iChosenMine )
	{
		case 1: //Drassen
			InitLairDrassen();
			curr = FindUnderGroundSector( 13, 5, 1 );
			curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;
			break;
		case 2: //Alma's mine
			InitLairAlma();
			curr = FindUnderGroundSector( 14, 10, 1 );
			curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;
			break;
		case 3: //Cambria
			InitLairCambria();
			curr = FindUnderGroundSector( 9, 8, 1 );
			curr->uiFlags |= SF_PENDING_ALTERNATE_MAP; //entrance
			break;
		case 4: //Grumm's mine
			InitLairGrumm();
			curr = FindUnderGroundSector( 4, 8, 2 );
			curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;
			break;
		default:
			#ifdef JA2BETAVERSION
			{
				CHAR16 str[512];
				swprintf( str, L"Creature quest never chose a lair and won't infect any mines.	Infectible mines = %d, iRandom = %d.	"
											L"This isn't a bug if you are not receiving income from any mines.", iNumMinesInfectible, iOrigRandom );
				DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
			}
			#endif
			return;
	}
	*/
	
	//Now, choose a start location for the queen.
	InitLair( iChosenMine );

	//load alternate map for creature changes
	curr = FindUnderGroundSector( gCreaturePlacements[ giLairID ].sAltMapX, gCreaturePlacements[ giLairID ].sAltMapY,
		gCreaturePlacements[ giLairID ].ubAltMapZ );
	curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;

	//Now determine how often we will spread the creatures.
	
	i = zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenInitBonusSpread;
	AddPeriodStrategicEvent( EVENT_CREATURE_SPREAD, zDiffSetting[gGameOptions.ubDifficultyLevel].iCreatureSpreadTime, 0 );
	
	//Set things up so that the creatures can plan attacks on helpless miners and civilians while
	//they are sleeping.	They do their planning at 10PM every day, and decide to attack sometime
	//during the night.
	AddEveryDayStrategicEvent( EVENT_CREATURE_NIGHT_PLANNING, 1320, 0 );

	//Got to give the queen some early protection, so do some creature spreading.
	while( i-- )
	{ //# times spread is based on difficulty, and the values in the defines.
		SpreadCreatures();
	}
}

void AddCreatureToNode( CREATURE_DIRECTIVE *node )
{
	node->pLevel->ubNumCreatures++;

	if( node->pLevel->uiFlags & SF_PENDING_ALTERNATE_MAP )
	{ //there is an alternate map meaning that there is a dynamic opening.	From now on
		//we substitute this map.
		node->pLevel->uiFlags &= ~SF_PENDING_ALTERNATE_MAP;
		node->pLevel->uiFlags |= SF_USE_ALTERNATE_MAP;
	}
}

BOOLEAN PlaceNewCreature( CREATURE_DIRECTIVE *node, INT32 iDistance )
{
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CreatureSpreading1");
	if( !node )
		return FALSE;
	//check to see if the creatures are permitted to spread into certain areas.	There are 4 mines (human perspective), and
	//creatures won't spread to them until the player controls them.	Additionally, if the player has recently cleared the
	//mine, then temporarily prevent the spreading of creatures.

	if( giHabitatedDistance == iDistance )
	{	//FRONT-LINE CONDITIONS -- consider expansion or frontline fortification.	The formulae used
		//in this sector are geared towards outer expansion.
		//we have reached the distance limitation for the spreading.	We will determine if
		//the area is populated enough to spread further.	The minimum population must be 4 before
		//spreading is even considered.
		if( node->pLevel->ubNumCreatures*10 - 10 <= (INT32)Random( 60 ) )
		{ // x<=1	100%
			// x==2		83%
			// x==3		67%
			// x==4		50%
			// x==5		33%
			// x==6		17%
			// x>=7		0%
			AddCreatureToNode( node );
			return TRUE;
		}
	}
	else if( giHabitatedDistance > iDistance )
	{
        // WDS - make number of mercenaries, etc. be configurable
		//we are within the "safe" habitated area of the creature's area of influence.	The chance of
		//increasing the population inside this sector depends on how deep we are within the sector.
		if( node->pLevel->ubNumCreatures < gGameExternalOptions.iMaxEnemyGroupSize ||
			node->pLevel->ubNumCreatures < gGameExternalOptions.ubGameMaximumNumberOfCreatures && node->pLevel->ubCreatureHabitat == QUEEN_LAIR )
		{ //there is ALWAYS a chance to habitate an interior sector, though the chances are slim for
			//highly occupied sectors.	This chance is modified by the type of area we are in.
			INT32 iAbsoluteMaxPopulation;
			INT32 iMaxPopulation=-1;
			INT32 iChanceToPopulate;
			
			/* // externalize to xml data
			switch( node->pLevel->ubCreatureHabitat )
			{
				case QUEEN_LAIR: //Defend the queen bonus
					iAbsoluteMaxPopulation = gGameExternalOptions.ubGameMaximumNumberOfCreatures;
					break;
				case LAIR: //Smaller defend the queen bonus
					iAbsoluteMaxPopulation = 18;
					break;
				case LAIR_ENTRANCE: //Smallest defend the queen bonus
					iAbsoluteMaxPopulation = 15;
					break;
				case INNER_MINE: //neg bonus -- actually promotes expansion over population, and decrease max pop here.
					iAbsoluteMaxPopulation = 12;
					break;
				case OUTER_MINE: //neg bonus -- actually promotes expansion over population, and decrease max pop here.
					iAbsoluteMaxPopulation = 10;
					break;
				case FEEDING_GROUNDS: //get free food bonus!	yummy humans :)
					iAbsoluteMaxPopulation = 15;
					break;
				case MINE_EXIT:	//close access to humans (don't want to overwhelm them)
					iAbsoluteMaxPopulation = 10;
					break;
				default:
					Assert( 0 );
					return FALSE;
			}
			*/

			iAbsoluteMaxPopulation = gCreatureComposition[ node->pLevel->ubCreatureHabitat ].iMaxPopulation;

			switch( gGameOptions.ubDifficultyLevel )
			{
				case DIF_LEVEL_EASY: //50%
					iAbsoluteMaxPopulation /= 2; //Half
					break;
				case DIF_LEVEL_MEDIUM: //80%
					iAbsoluteMaxPopulation = iAbsoluteMaxPopulation * 4 / 5;
					break;
				case DIF_LEVEL_HARD: //100%
					break;
				case DIF_LEVEL_INSANE: //200%
					iAbsoluteMaxPopulation *= 2;
					break;
				default:
					iAbsoluteMaxPopulation /= 2; //Half
					break;
			}

			//Calculate the desired max population percentage based purely on current distant to creature range.
			//The closer we are to the lair, the closer this value will be to 100.
			iMaxPopulation = 100 - iDistance * 100 / giHabitatedDistance;
			iMaxPopulation = max( iMaxPopulation, 25 );
			//Now, convert the previous value into a numeric population.
			iMaxPopulation = iAbsoluteMaxPopulation * iMaxPopulation / 100;
			iMaxPopulation = max( iMaxPopulation, 4 );


			//The chance to populate a sector is higher for lower populations.	This is calculated on
			//the ratio of current population to the max population.
			iChanceToPopulate = 100 - node->pLevel->ubNumCreatures * 100 / iMaxPopulation;

			if( !node->pLevel->ubNumCreatures || iChanceToPopulate > (INT32)Random( 100 )
					&& iMaxPopulation > node->pLevel->ubNumCreatures )
			{
				AddCreatureToNode( node );
				return TRUE;
			}
		}
	}
	else
	{ //we are in a new area, so we will populate it
		AddCreatureToNode( node );
		giHabitatedDistance++;
		return TRUE;
	}
	if( PlaceNewCreature( node->next, iDistance + 1 ) )
		return TRUE;
	return FALSE;
}

void SpreadCreatures()
{
	if( giLairID == -1 )
	{
		DecayCreatures();
		return;
	}

	//queen just produced a litter of creature larvae.	Let's do some spreading now.
	UINT16 usNewCreatures = (UINT16)(zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenReproductionBase + Random( 1 + zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenReproductionBonus ));
	
	while( usNewCreatures-- )
	{
		//Note, this function can and will fail if the population gets dense.	This is a necessary
		//feature.	Otherwise, the queen would fill all the cave levels with MAX_STRATEGIC_TEAM_SIZE monsters, and that would
		//be bad.
		PlaceNewCreature( lair, 0 );
	}
}

void DecayCreatures()
{
	//when the queen dies, we need to kill off the creatures over a period of time.

}

void AddCreaturesToBattle( UINT8 ubNumYoungMales, UINT8 ubNumYoungFemales, UINT8 ubNumAdultMales, UINT8 ubNumAdultFemales )
{
	INT32 iRandom;
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo = { };
	UINT8 bDesiredDirection=0;
	UINT8 ubCurrSlot = 0;

	switch( gsCreatureInsertionCode )
	{
		case INSERTION_CODE_NORTH:	bDesiredDirection = SOUTHEAST;										break;
		case INSERTION_CODE_EAST:	bDesiredDirection = SOUTHWEST;										break;
		case INSERTION_CODE_SOUTH:	bDesiredDirection = NORTHWEST;										break;
		case INSERTION_CODE_WEST:	bDesiredDirection = NORTHEAST;										break;
		case INSERTION_CODE_GRIDNO:																		break;
		default:	AssertMsg( 0, "Illegal direction passed to AddCreaturesToBattle()" );	break;
	}

	#ifdef JA2TESTVERSION
		ScreenMsg( FONT_RED, MSG_INTERFACE, L"Creature attackers have arrived!" );
	#endif

	if( gsCreatureInsertionCode != INSERTION_CODE_GRIDNO )
	{
		ChooseMapEdgepoints( &MapEdgepointInfo, (UINT8)gsCreatureInsertionCode, (UINT8)(ubNumYoungMales+ubNumYoungFemales+ubNumAdultMales+ubNumAdultFemales) );
		ubCurrSlot = 0;
	}
	while( ubNumYoungMales || ubNumYoungFemales || ubNumAdultMales || ubNumAdultFemales )
	{
		iRandom = (INT32)Random( ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales );
		if( ubNumYoungMales &&	iRandom < (INT32)ubNumYoungMales )
		{
			ubNumYoungMales--;
			pSoldier = TacticalCreateCreature( YAM_MONSTER );
		}
		else if( ubNumYoungFemales && iRandom < (INT32)(ubNumYoungMales + ubNumYoungFemales) )
		{
			ubNumYoungFemales--;
			pSoldier = TacticalCreateCreature( YAF_MONSTER );
		}
		else if( ubNumAdultMales && iRandom < (INT32)(ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales) )
		{
			ubNumAdultMales--;
			pSoldier = TacticalCreateCreature( AM_MONSTER );
		}
		else if( ubNumAdultFemales && iRandom < (INT32)(ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales ) )
		{
			ubNumAdultFemales--;
			pSoldier = TacticalCreateCreature( ADULTFEMALEMONSTER );
		}
		else
		{
			ResetCreatureAttackVariables();

			AllTeamsLookForAll( FALSE );

			Assert(0);
			return;
		}

		pSoldier->ubInsertionDirection = bDesiredDirection;
		//Setup the position
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->aiData.bHunting = TRUE;
		if( gsCreatureInsertionCode != INSERTION_CODE_GRIDNO )
		{
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = (UINT8)gsCreatureInsertionCode;
			}
		}
		else
		{
			pSoldier->usStrategicInsertionData = gsCreatureInsertionGridNo;
		}
		UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
	}
	
	ResetCreatureAttackVariables();

	AllTeamsLookForAll( FALSE );
}

void AddCreaturesToBattle_Other( UINT8 ubNum )
{
	INT32 iRandom;
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo = {};
	UINT8 bDesiredDirection = 0;
	UINT8 ubCurrSlot = 0;

	switch ( gsCreatureInsertionCode )
	{
	case INSERTION_CODE_NORTH:	bDesiredDirection = SOUTHEAST;										break;
	case INSERTION_CODE_EAST:	bDesiredDirection = SOUTHWEST;										break;
	case INSERTION_CODE_SOUTH:	bDesiredDirection = NORTHWEST;										break;
	case INSERTION_CODE_WEST:	bDesiredDirection = NORTHEAST;										break;
	case INSERTION_CODE_GRIDNO:																		break;
	default:	AssertMsg( 0, "Illegal direction passed to AddCreaturesToBattle_Bloodcats()" );	break;
	}

#ifdef JA2TESTVERSION
	ScreenMsg( FONT_RED, MSG_INTERFACE, L"Creature attackers have arrived!" );
#endif

	if ( gsCreatureInsertionCode != INSERTION_CODE_GRIDNO )
	{
		ChooseMapEdgepoints( &MapEdgepointInfo, (UINT8)gsCreatureInsertionCode, ubNum );
		ubCurrSlot = 0;
	}

	while ( ubNum )
	{
		--ubNum;

		switch ( guCreatureAttackType )
		{
			// this should not happen!
		case CREATURE_ATTACK_TYPE_CREATURE:
			pSoldier = TacticalCreateCreature( ADULTFEMALEMONSTER );
			break;
		case CREATURE_ATTACK_TYPE_ZOMBIE:
			pSoldier = TacticalCreateZombie();
			break;
		case CREATURE_ATTACK_TYPE_BANDIT:
			pSoldier = TacticalCreateBandit();
			break;
		case CREATURE_ATTACK_TYPE_BLOODCAT:
			pSoldier = TacticalCreateCreature( BLOODCAT );
			break;
		}

		pSoldier->ubInsertionDirection = bDesiredDirection;

		//Setup the position
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->aiData.bHunting = TRUE;

		if ( gsCreatureInsertionCode != INSERTION_CODE_GRIDNO )
		{
			if ( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{
				//using an edgepoint
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ubCurrSlot++];
			}
			else
			{
				//no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = (UINT8)gsCreatureInsertionCode;
			}
		}
		else
		{
			pSoldier->usStrategicInsertionData = gsCreatureInsertionGridNo;
		}

		UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );

		if ( pSoldier )
		{
			// send soldier to centre of map, roughly
			pSoldier->aiData.sNoiseGridno = ( CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS );
			pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
		}
	}
	
	ResetCreatureAttackVariables();

	AllTeamsLookForAll( FALSE );
}

void ChooseTownSectorToAttack( UINT8 ubSectorID, BOOLEAN fOverrideTest )
{
	INT32 iRandom, i;
	UINT8 ubSectorX, ubSectorY;
	UINT8 ubAttackSectorX, ubAttackSectorY;
	INT8 bTownId, bTownSize;
	UINT8 ubAttackSectorID;

	ubSectorX = SECTORX( ubSectorID );
	ubSectorY = SECTORY( ubSectorID );

	/* // externalize to xml data
	if( !fOverrideTest )
	{
		iRandom = PreRandom( 100 );
		switch( ubSectorID )
		{
			case SEC_D13: //DRASSEN
				if( iRandom < 45 )
					ubAttackSectorID = SEC_D13;
				else if( iRandom < 70 )
					ubAttackSectorID = SEC_C13;
				else
					ubAttackSectorID = SEC_B13;
				break;
			case SEC_H3: //GRUMM
				if( iRandom < 35 )
					ubAttackSectorID = SEC_H3;
				else if( iRandom < 55 )
					ubAttackSectorID = SEC_H2;
				else if( iRandom < 70 )
					ubAttackSectorID = SEC_G2;
				else if( iRandom < 85 )
					ubAttackSectorID = SEC_H1;
				else
					ubAttackSectorID = SEC_G1;
				break;
			case SEC_H8: //CAMBRIA
				if( iRandom < 35 )
					ubAttackSectorID = SEC_H8;
				else if( iRandom < 55 )
					ubAttackSectorID = SEC_G8;
				else if( iRandom < 70 )
					ubAttackSectorID = SEC_F8;
				else if( iRandom < 85 )
					ubAttackSectorID = SEC_G9;
				else
					ubAttackSectorID = SEC_F9;
				break;
			case SEC_I14: //ALMA
				if( iRandom < 45 )
					ubAttackSectorID = SEC_I14;
				else if( iRandom < 65 )
					ubAttackSectorID = SEC_I13;
				else if( iRandom < 85 )
					ubAttackSectorID = SEC_H14;
				else
					ubAttackSectorID = SEC_H13;
				break;
			default:
				Assert( 0 );
				return;
		}
	}
	else
		ubAttackSectorID = ubSectorID;

	switch( ubAttackSectorID )
	{
		case SEC_D13: //DRASSEN
			gsCreatureInsertionCode = INSERTION_CODE_GRIDNO;
			gsCreatureInsertionGridNo = 20703;//dnl!!!
			break;
		case SEC_C13:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_B13:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_H3: //GRUMM
			gsCreatureInsertionCode = INSERTION_CODE_GRIDNO;
			gsCreatureInsertionGridNo = 10303; //dnl!!!
			break;
		case SEC_H2:
			gsCreatureInsertionCode = INSERTION_CODE_EAST;
			break;
		case SEC_G2:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_H1:
			gsCreatureInsertionCode = INSERTION_CODE_EAST;
			break;
		case SEC_G1:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_H8: //CAMBRIA
			gsCreatureInsertionCode = INSERTION_CODE_GRIDNO;
			gsCreatureInsertionGridNo = 13005; //dnl!!!
			break;
		case SEC_G8:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_F8:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_G9:
			gsCreatureInsertionCode = INSERTION_CODE_WEST;
			break;
		case SEC_F9:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_I14: //ALMA
			gsCreatureInsertionCode = INSERTION_CODE_GRIDNO;
			gsCreatureInsertionGridNo = 9726; //dnl!!!
			break;
		case SEC_I13:
			gsCreatureInsertionCode = INSERTION_CODE_EAST;
			break;
		case SEC_H14:
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
			break;
		case SEC_H13:
			gsCreatureInsertionCode = INSERTION_CODE_EAST;
			break;
		default:
			return;
	}
	*/
	
	// determine the sector to attack
	if( !fOverrideTest )
	{
		// attack a random player-controlled town sector
		if( gGameExternalOptions.fCrepitusAttackAllTowns )
		{
			do
			{
				i = Random( SEC_P16 + 1 );
			} while ( !StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX ( i ) ].bNameId ||
				StrategicMap[  SECTOR_INFO_TO_STRATEGIC_INDEX ( i ) ].fEnemyControlled );

			ubAttackSectorID = i;
		}
		// attack only the creature originating town, nothing will happen if sector is enemy-controlled
		else
		{
			bTownId = StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX ( ubSectorID )].bNameId;
			bTownSize = GetTownSectorSize( bTownId );
			iRandom = PreRandom( bTownSize + 1 ) + 1;

			// originating sector has double the chance
			if( iRandom == bTownSize + 1 )
				ubAttackSectorID = ubSectorID;
			else
			{
				INT32 j=0;
				for( i = 0; i < MAP_WORLD_X * MAP_WORLD_Y + 1; i++ )
				{
					if( StrategicMap[ i ].bNameId == bTownId )
					{
						j++;
						if( iRandom == j )
							ubAttackSectorID = STRATEGIC_INDEX_TO_SECTOR_INFO( i );
					}
				}
			}
		}
	}
	else
		ubAttackSectorID = ubSectorID;
	
	ubAttackSectorX = SECTORX( ubAttackSectorID );
	ubAttackSectorY = SECTORY( ubAttackSectorID );

	// determine the attack sector insertion code
	if( ubAttackSectorID == ubSectorID )		
	{
		gsCreatureInsertionCode = INSERTION_CODE_GRIDNO;
		gsCreatureInsertionGridNo = gCreaturePlacements[ giLairID ].iAttackSourceGridNo;
	}
	// attack sector north of source sector
	else if( ubAttackSectorX < ubSectorX )
	{
		// valid entry
		if( SectorInfo[ ubAttackSectorID ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] != GROUNDBARRIER &&
			SectorInfo[ ubAttackSectorID ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
		{
			gsCreatureInsertionCode = INSERTION_CODE_SOUTH;
		}
		else
		{
			for( i = 0; i < THROUGH_STRATEGIC_MOVE; i++ )
			{
				if( SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != GROUNDBARRIER &&
				SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != EDGEOFWORLD )
				{
					gsCreatureInsertionCode = i;
					break;
				}	
			}
		}
	}
	// attack sector south of source sector
	else if ( ubAttackSectorX > ubSectorX )
	{
		// valid entry
		if( SectorInfo[ ubAttackSectorID ].ubTraversability[ NORTH_STRATEGIC_MOVE ] != GROUNDBARRIER &&
			SectorInfo[ ubAttackSectorID ].ubTraversability[ NORTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
		{
			gsCreatureInsertionCode = INSERTION_CODE_NORTH;
		}
		else
		{
			for( i = 0; i < THROUGH_STRATEGIC_MOVE; i++ )
			{
				if( SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != GROUNDBARRIER &&
				SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != EDGEOFWORLD )
				{
					gsCreatureInsertionCode = i;
					break;
				}	
			}
		}
	}
	// attack sector west of source sector
	else if( ubAttackSectorY < ubSectorY )
	{
		// valid entry
		if( SectorInfo[ ubAttackSectorID ].ubTraversability[ EAST_STRATEGIC_MOVE ] != GROUNDBARRIER &&
			SectorInfo[ ubAttackSectorID ].ubTraversability[ EAST_STRATEGIC_MOVE ] != EDGEOFWORLD )
		{
			gsCreatureInsertionCode = INSERTION_CODE_EAST;
		}
		else
		{
			for( i = 0; i < THROUGH_STRATEGIC_MOVE; i++ )
			{
				if( SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != GROUNDBARRIER &&
				SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != EDGEOFWORLD )
				{
					gsCreatureInsertionCode = i;
					break;
				}	
			}
		}
	}
	// attack sector east of source sector
	else if( ubAttackSectorY > ubSectorY )
	{
		// valid entry
		if( SectorInfo[ ubAttackSectorID ].ubTraversability[ WEST_STRATEGIC_MOVE ] != GROUNDBARRIER &&
			SectorInfo[ ubAttackSectorID ].ubTraversability[ WEST_STRATEGIC_MOVE ] != EDGEOFWORLD )
		{
			gsCreatureInsertionCode = INSERTION_CODE_WEST;
		}
		else
		{
			for( i = 0; i < THROUGH_STRATEGIC_MOVE; i++ )
			{
				if( SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != GROUNDBARRIER &&
				SectorInfo[ ubAttackSectorID ].ubTraversability[ i ] != EDGEOFWORLD )
				{
					gsCreatureInsertionCode = i;
					break;
				}	
			}
		}
	}
	else
	{
			Assert(0);
			return;
	}

	gubSectorIDOfCreatureAttack = ubAttackSectorID;
}

void CreatureAttackTown( UINT8 ubSectorID, BOOLEAN fOverrideTest )
{
	//This is the launching point of the creature attack.
	UNDERGROUND_SECTORINFO *pSector;
	UINT8 ubSectorX, ubSectorY;

	if( gfWorldLoaded && gTacticalStatus.fEnemyInSector )
	{ //Battle currently in progress, repost the event
		AddStrategicEvent( EVENT_CREATURE_ATTACK, GetWorldTotalMin() + Random( 10 ), ubSectorID );
		return;
	}

	guCreatureAttackType = CREATURE_ATTACK_TYPE_CREATURE;
	gubCreatureBattleCode = CREATURE_BATTLE_CODE_NONE;

	ubSectorX = SECTORX( ubSectorID );
	ubSectorY = SECTORY( ubSectorID );

	if( !fOverrideTest )
	{
		//Record the number of creatures in the sector.
		pSector = FindUnderGroundSector( ubSectorX, ubSectorY, 1 );
		if( !pSector )
		{
			CreatureAttackTown( ubSectorID, TRUE );
			return;
		}
		gubNumCreaturesAttackingTown = pSector->ubNumCreatures;
		if( !gubNumCreaturesAttackingTown )
		{
			CreatureAttackTown( ubSectorID, TRUE );
			return;
		}

		pSector->ubNumCreatures = 0;

		//Choose one of the town sectors to attack.	Sectors closer to
		//the mine entrance have a greater chance of being chosen.
		ChooseTownSectorToAttack( ubSectorID, FALSE );
		ubSectorX = (UINT8)SECTORX( gubSectorIDOfCreatureAttack );
		ubSectorY = (UINT8)SECTORY( gubSectorIDOfCreatureAttack );
	}
	else
	{
		ChooseTownSectorToAttack( ubSectorID, TRUE );
		gubNumCreaturesAttackingTown = 5;
	}

	//Now that the sector has been chosen, attack it!
	if( PlayerGroupsInSector( ubSectorX, ubSectorY, 0 ) )
	{
		gubCreatureBattleCode = CREATURE_BATTLE_CODE_PREBATTLEINTERFACE;

		//we have players in the sector
		if( ubSectorX == gWorldSectorX && ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{
			//This is the currently loaded sector.	All we have to do is change the music and insert
			//the creatures tactically.
			if( guiCurrentScreen == GAME_SCREEN )
			{
				gubCreatureBattleCode = CREATURE_BATTLE_CODE_TACTICALLYADD;
			}
		}
	}
	else if ( NumNonPlayerTeamMembersInSector( ubSectorX, ubSectorY, MILITIA_TEAM ) )
	{
		//we have militia in the sector
		gubCreatureBattleCode = CREATURE_BATTLE_CODE_AUTORESOLVE;
	}
	else if( !StrategicMap[ ubSectorX + MAP_WORLD_X * ubSectorY ].fEnemyControlled )
	{ //player controlled sector -- eat some civilians
		AdjustLoyaltyForCivsEatenByMonsters( ubSectorX, ubSectorY, gubNumCreaturesAttackingTown, ( guCreatureAttackType == CREATURE_ATTACK_TYPE_BANDIT ) );
		SectorInfo[ ubSectorID ].ubDayOfLastCreatureAttack = (UINT8)GetWorldDay();
		return;
	}
	else
	{ //enemy controlled sectors don't get attacked.
		return;
	}

	SectorInfo[ ubSectorID ].ubDayOfLastCreatureAttack = (UINT8)GetWorldDay();
	switch( gubCreatureBattleCode )
	{
		case CREATURE_BATTLE_CODE_PREBATTLEINTERFACE:
			InitPreBattleInterface( NULL, TRUE );
			break;
		case CREATURE_BATTLE_CODE_AUTORESOLVE:
			gfAutomaticallyStartAutoResolve = TRUE;
			InitPreBattleInterface( NULL, TRUE );
			break;
		case CREATURE_BATTLE_CODE_TACTICALLYADD:
			if (is_networked)
			{
				if(is_server && gCreatureEnabled == 1)
					PrepareCreaturesForBattle();
			}
			else
				PrepareCreaturesForBattle();
			break;
	}
	InterruptTime();
	PauseGame();
	LockPauseState( 2 );
}

void CreatureAttackTown_OtherCreatures( UINT8 ubSectorID, UINT8 ubType )
{
	if ( gfWorldLoaded && gTacticalStatus.fEnemyInSector )
	{
		//Battle currently in progress, repost the event
		AddStrategicEvent( ubType, GetWorldTotalMin() + Random( 10 ), ubSectorID );
		return;
	}

	// if this sector is not under player control, nothing to do
	if ( StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX ( ubSectorID )].fEnemyControlled )
		return;
	
	guCreatureAttackType = ubType;	
	
	UINT8 ubSectorX = SECTORX( ubSectorID );
	UINT8 ubSectorY = SECTORY( ubSectorID );

	// test whether the attack is possible in this sector according to xml settings
	switch ( guCreatureAttackType )
	{
	case CREATURE_ATTACK_TYPE_BLOODCAT:
		if ( !gGameExternalOptions.gRaid_Bloodcats || !( SectorExternalData[ubSectorID][0].usSectorFlagMask & SECTORFLAG_RAIDPOSSIBLE_BLOODCAT ) )
			return;
		break;

	case CREATURE_ATTACK_TYPE_ZOMBIE:
		if ( !gGameExternalOptions.gRaid_Zombies || !( SectorExternalData[ubSectorID][0].usSectorFlagMask & SECTORFLAG_RAIDPOSSIBLE_ZOMBIE ) )
			return;
		break;

	case CREATURE_ATTACK_TYPE_BANDIT:
		if ( !gGameExternalOptions.gRaid_Bandits || !( SectorExternalData[ubSectorID][0].usSectorFlagMask & SECTORFLAG_RAIDPOSSIBLE_BANDIT ) )
			return;
		break;

	default:
		return;
		break;
	}
	
	// choose an insertion direction. The intention is to make it seem as if the raids occur from wilderness sectors into the sector
	// note that attacks against non-town sectors work, but if no player forces are present, nothing happens (as loyalty won't be affected)
	std::vector<INT16> possibleinsertioncodes;
	for ( INT16 i = 0; i < THROUGH_STRATEGIC_MOVE; ++i )
	{
		INT16 loopX = ubSectorX;
		INT16 loopY = ubSectorY;

		if ( i == NORTH_STRATEGIC_MOVE )		--loopY;
		else if ( i == EAST_STRATEGIC_MOVE )	++loopX;
		else if ( i == SOUTH_STRATEGIC_MOVE )	++loopY;
		else if ( i == WEST_STRATEGIC_MOVE )	--loopX;

		if ( loopX < 1 || loopX >= MAP_WORLD_X - 1 || loopY < 1 || loopY >= MAP_WORLD_Y - 1 )
			continue;

		UINT8 townid_B = GetTownIdForSector( loopX, loopY );

		if ( townid_B != BLANK_SECTOR )
			continue;

		UINT8 originsector = SECTOR(loopX, loopY);

		// valid entry
		if ( SectorInfo[originsector].ubTraversability[i] != GROUNDBARRIER &&
			SectorInfo[originsector].ubTraversability[i] != TOWN &&
			SectorInfo[originsector].ubTraversability[i] != WATER &&
			SectorInfo[originsector].ubTraversability[i] != EDGEOFWORLD )
		{
			possibleinsertioncodes.push_back( i );

			gsCreatureInsertionCode = i;
		}
	}

	if ( possibleinsertioncodes.empty() )
		return;

	gsCreatureInsertionCode = possibleinsertioncodes[Random( possibleinsertioncodes.size())];
	
	// determine how strong the player presence is, and then determine whether to attack, with how many dudes, or not to attack because the sector is defended to strongly
	UINT16 militiapresent	= NumNonPlayerTeamMembersInSector( ubSectorX, ubSectorY, MILITIA_TEAM );
	UINT16 mercspresent		= NumPlayerTeamMembersInSector( ubSectorX, ubSectorY, 0 );

	// determine how many troops are available and stop the attack if we don't have enough
	INT32 available_bloodcats;
	INT32 available_zombies;
	INT32 available_bandits;
	GetRaidPersonnel( available_bloodcats, available_zombies, available_bandits );
	
	switch ( guCreatureAttackType )
	{
	case CREATURE_ATTACK_TYPE_BLOODCAT:		
		gubNumCreaturesAttackingTown = min( gGameExternalOptions.gRaidMaxSize_Bloodcats, available_bloodcats );

		// bloodcats won't attack if player forces are too numerous or not enough forces are available
		if ( gubNumCreaturesAttackingTown < mercspresent + militiapresent )
			return;
		break;

	case CREATURE_ATTACK_TYPE_ZOMBIE:		
		gubNumCreaturesAttackingTown = min( gGameExternalOptions.gRaidMaxSize_Zombies, available_zombies );
		break;

	case CREATURE_ATTACK_TYPE_BANDIT:		
		gubNumCreaturesAttackingTown = min( gGameExternalOptions.gRaidMaxSize_Bandits, available_bandits );

		// bandits won't attack if player forces are too numerous or not enough forces are available
		if ( gubNumCreaturesAttackingTown < mercspresent + militiapresent )
			return;
		break;

	default:
		return;
		break;
	}

	// safety check - not too small
	if ( gubNumCreaturesAttackingTown < 1 )
		return;
	
	gubCreatureBattleCode = CREATURE_BATTLE_CODE_NONE;
	gubSectorIDOfCreatureAttack = ubSectorID;
	
	//Now that the sector has been chosen, attack it!
	if ( mercspresent )
	{
		gubCreatureBattleCode = CREATURE_BATTLE_CODE_PREBATTLEINTERFACE;

		//we have players in the sector
		if ( ubSectorX == gWorldSectorX && ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{
			//This is the currently loaded sector.	All we have to do is change the music and insert
			//the creatures tactically.
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				gubCreatureBattleCode = CREATURE_BATTLE_CODE_TACTICALLYADD;
			}
		}
	}
	else if ( militiapresent )
	{
		//we have militia in the sector
		gubCreatureBattleCode = CREATURE_BATTLE_CODE_AUTORESOLVE;
	}
	else
	{
		// player controlled sector -- eat some civilians
		AdjustLoyaltyForCivsEatenByMonsters( ubSectorX, ubSectorY, min(1, gubNumCreaturesAttackingTown / 2), ( guCreatureAttackType == CREATURE_ATTACK_TYPE_BANDIT ) );

		// do NOT change sector ownership. This would cause the AI to stop attacking, and we can't have that, no?
		//SetThisSectorAsEnemyControlled( ubSectorX, ubSectorY, 0, TRUE );
		SectorInfo[ubSectorID].ubDayOfLastCreatureAttack = GetWorldDay();

		return;
	}

	switch ( guCreatureAttackType )
	{
	case CREATURE_ATTACK_TYPE_BLOODCAT:
		SetEnemyEncounterCode( BLOODCAT_ATTACK_CODE );
		break;

	case CREATURE_ATTACK_TYPE_ZOMBIE:
		SetEnemyEncounterCode( ZOMBIE_ATTACK_CODE );
		break;

	case CREATURE_ATTACK_TYPE_BANDIT:
		SetEnemyEncounterCode( BANDIT_ATTACK_CODE );
		break;

	default:
		SetEnemyEncounterCode( CREATURE_ATTACK_CODE );
		break;
	}

	SectorInfo[ubSectorID].ubDayOfLastCreatureAttack = (UINT8)GetWorldDay();

	switch ( gubCreatureBattleCode )
	{
	case CREATURE_BATTLE_CODE_PREBATTLEINTERFACE:
		InitPreBattleInterface( NULL, TRUE );
		break;
	case CREATURE_BATTLE_CODE_AUTORESOLVE:
		gfAutomaticallyStartAutoResolve = TRUE;
		InitPreBattleInterface( NULL, TRUE );
		break;
	case CREATURE_BATTLE_CODE_TACTICALLYADD:
		if ( is_networked )
		{
			if ( is_server && gCreatureEnabled == 1 )
				PrepareCreaturesForBattle();
		}
		else
			PrepareCreaturesForBattle();

		// alert the creatures
		HandleInitialRedAlert( CREATURE_TEAM, FALSE );

		// we need this so that the battle is properly recognized in strategic
		{
			//Set up the location as well as turning on the blit flag.
			gubPBSectorX = (UINT8)ubSectorX;
			gubPBSectorY = (UINT8)ubSectorY;
			gubPBSectorZ = (UINT8)0;
			gfBlitBattleSectorLocator = TRUE;
		}

		break;
	}

	// Flugente: the pause state isn't always removed properly. It seems we can also do without it... let's see whether this has unintentional consequences
	InterruptTime();
	PauseGame();
	//LockPauseState( 2 );
}

//Called by campaign init.
void ChooseCreatureQuestStartDay()
{
	if (gGameOptions.ubGameStyle == STYLE_REALISTIC || !gGameExternalOptions.fEnableCrepitus)
		return;

	/*
//	INT32 iRandom, iDay;
	if( !(gGameOptions.ubGameStyle == STYLE_SCIFI) || !gGameExternalOptions.fEnableCrepitus)
		return; //only available in science fiction mode.
	*/
	
	//Post the event.	Once it becomes due, it will setup the queen monster's location, and
	//begin spreading and attacking towns from there.
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			//AddPeriodStrategicEvent( EVENT_BEGIN_CREATURE_QUEST, (EASY_QUEEN_START_DAY + Random( 1 + EASY_QUEEN_START_BONUS )) * 1440 , 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			//AddPeriodStrategicEvent( EVENT_BEGIN_CREATURE_QUEST, (NORMAL_QUEEN_START_DAY + Random( 1 + NORMAL_QUEEN_START_BONUS )) * 1440, 0 );
			break;
		case DIF_LEVEL_HARD:
			//AddPeriodStrategicEvent( EVENT_BEGIN_CREATURE_QUEST, (HARD_QUEEN_START_DAY + Random( 1 + HARD_QUEEN_START_BONUS )) * 1440, 0 );
			break;
		case DIF_LEVEL_INSANE:
			//AddPeriodStrategicEvent( EVENT_BEGIN_CREATURE_QUEST, (INSANE_QUEEN_START_DAY + Random( 1 + INSANE_QUEEN_START_BONUS )) * 1440, 0 );
			break;
	}
}

void DeleteDirectiveNode( CREATURE_DIRECTIVE **node )
{
	if( (*node)->next )
		DeleteDirectiveNode( &((*node)->next) );
	MemFree( *node );
	*node = NULL;
}

//Recursively delete all nodes (from the top down).
void DeleteCreatureDirectives()
{
	if( lair )
		DeleteDirectiveNode( &lair );
	giLairID = 0;
}

void ClearCreatureQuest()
{
	//This will remove all of the underground sector information and reinitialize it.
	//The only part that doesn't get added are the queen's lair.
	BuildUndergroundSectorInfoList();
	DeleteAllStrategicEventsOfType( EVENT_BEGIN_CREATURE_QUEST );
	DeleteCreatureDirectives();
}

void EndCreatureQuest()
{
	CREATURE_DIRECTIVE *curr;
	UNDERGROUND_SECTORINFO *pSector;
	INT32 i;

	//By setting the lairID to -1, when it comes time to spread creatures,
	//They will get subtracted instead.
	giDestroyedLairID = giLairID;
	giLairID = -1;

	//Also nuke all of the creatures in all of the other mine sectors.	This
	//is keyed on the fact that the queen monster is killed.
	curr = lair;
	if( curr )
	{ //skip first node (there could be other creatures around.
		curr = curr->next;
	}
	while( curr )
	{
		curr->pLevel->ubNumCreatures = 0;
		curr = curr->next;
	}

	//Remove the creatures that are trapped underneath Tixa
	pSector = FindUnderGroundSector( gCreaturesSettings.ubCrepitusFeedingSectorX, gCreaturesSettings.ubCrepitusFeedingSectorY,
		gCreaturesSettings.ubCrepitusFeedingSectorZ ); // (9, 10, 2)
	if( pSector )
	{
		pSector->ubNumCreatures = 0;
	}

	//Also find and nuke all creatures on any surface levels!!!
	//KM: Sept 3, 1999 patch
	for( i = 0; i < SEC_P16 + 1; i++ )
	{
		SectorInfo[ i ].ubNumCreatures = 0;
		SectorInfo[ i ].ubCreaturesInBattle = 0;
	}
}

UINT8 CreaturesInUndergroundSector( UINT8 ubSectorID, UINT8 ubSectorZ )
{
	UNDERGROUND_SECTORINFO *pSector;
	UINT8 ubSectorX, ubSectorY;
	ubSectorX = (UINT8)SECTORX( ubSectorID );
	ubSectorY = (UINT8)SECTORY( ubSectorID );
	pSector = FindUnderGroundSector( ubSectorX, ubSectorY, ubSectorZ );
	if( pSector )
		return pSector->ubNumCreatures;
	return 0;
}

BOOLEAN MineClearOfMonsters( UINT8 ubMineIndex )
{
	Assert( ( ubMineIndex >= 0 ) && ( ubMineIndex < MAX_NUMBER_OF_MINES ) );

	if( !gMineStatus[ ubMineIndex ].fPrevInvadedByMonsters )
	{
		/* // externalize to xml data
		switch( ubMineIndex )
		{
			case MINE_GRUMM:
				if( CreaturesInUndergroundSector( SEC_H3, 1 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_I3, 1 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_I3, 2 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_H3, 2 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_H4, 2 ) )
					return FALSE;
				break;
			case MINE_CAMBRIA:
				if( CreaturesInUndergroundSector( SEC_H8, 1 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_H9, 1 ) )
					return FALSE;
				break;
			case MINE_ALMA:
				if( CreaturesInUndergroundSector( SEC_I14, 1 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_J14, 1 ) )
					return FALSE;
				break;
			case MINE_DRASSEN:
				if( CreaturesInUndergroundSector( SEC_D13, 1 ) )
					return FALSE;
				if( CreaturesInUndergroundSector( SEC_E13, 1 ) )
					return FALSE;
				break;
			case MINE_CHITZENA:
			case MINE_SAN_MONA:
				// these are never attacked
				break;

			default:
				#ifdef JA2BETAVERSION
					ScreenMsg( FONT_RED, MSG_ERROR, L"Attempting to check if mine is clear but mine index is invalid (%d).", ubMineIndex );
				#endif
				break;
		}
		*/
		
		// Buggler: use the defined underground mine sectors in lua script instead of the hardcoded ones
		UINT32 i;
		
		for( i = 0; i < associatedMineSectors.size(); i++ )
		{
			if( associatedMineSectors[ i ].mineID == ubMineIndex )
			{
				if( CreaturesInUndergroundSector( SECTOR( associatedMineSectors[ i ].x, associatedMineSectors[ i ].y ), associatedMineSectors[ i ].z ) )
					return FALSE;
			}
		}
	}
	else
	{ //mine was previously invaded by creatures.	Don't allow mine production until queen is dead.
		if( giLairID != -1 )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void DetermineCreatureTownComposition( UINT8 ubNumCreatures, UINT8 *pubNumYoungMales, UINT8 *pubNumYoungFemales,
									  UINT8 *pubNumAdultMales, UINT8 *pubNumAdultFemales )
{
	INT32 i, iRandom;
	UINT8 ubYoungMalePercentage = 10;
	UINT8 ubYoungFemalePercentage = 65;
	UINT8 ubAdultMalePercentage = 5;
	UINT8 ubAdultFemalePercentage = 20;

	//First step is to convert the percentages into the numbers we will use.
	ubYoungFemalePercentage += ubYoungMalePercentage;
	ubAdultMalePercentage += ubYoungFemalePercentage;
	ubAdultFemalePercentage += ubAdultMalePercentage;
	if( ubAdultFemalePercentage != 100 )
	{
		AssertMsg( 0, "Percentage for adding creatures don't add up to 100." );
	}
	//Second step is to determine the breakdown of the creatures randomly.
	i = ubNumCreatures;
	while( i-- )
	{
		iRandom = Random( 100 );
		if( iRandom < ubYoungMalePercentage )
			(*pubNumYoungMales)++;
		else if( iRandom < ubYoungFemalePercentage )
			(*pubNumYoungFemales)++;
		else if( iRandom < ubAdultMalePercentage )
			(*pubNumAdultMales)++;
		else
			(*pubNumAdultFemales)++;
	}
}

void DetermineCreatureTownCompositionBasedOnTacticalInformation( UINT8 *pubNumCreatures, UINT8 *pubNumYoungMales, UINT8 *pubNumYoungFemales,
																UINT8 *pubNumAdultMales, UINT8 *pubNumAdultFemales )
{
	SOLDIERTYPE *pSoldier;

	SECTORINFO* pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
	*pubNumCreatures = 0;
	pSector->ubNumCreatures = 0;
	pSector->ubCreaturesInBattle = 0;
	for( INT32 i = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; ++i )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			switch( pSoldier->ubBodyType )
			{
				case ADULTFEMALEMONSTER:
					(*pubNumCreatures)++;
					(*pubNumAdultFemales)++;
					break;
				case AM_MONSTER:
					(*pubNumCreatures)++;
					(*pubNumAdultMales)++;
					break;
				case YAF_MONSTER:
					(*pubNumCreatures)++;
					(*pubNumYoungFemales)++;
					break;
				case YAM_MONSTER:
					(*pubNumCreatures)++;
					(*pubNumYoungMales)++;
					break;
			}
		}
	}
}

void DetermineOtherCreatureTownCompositionBasedOnTacticalInformation( UINT8* pubNumCreatures, UINT8* pubNumBloodcats, UINT8* pubNumZombies, UINT8* pubNumBandits )
{
	SOLDIERTYPE *pSoldier;

	SECTORINFO* pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];
	*pubNumCreatures = 0;
	pSector->ubNumCreatures = 0;
	pSector->ubCreaturesInBattle = 0;
	for ( INT32 i = gTacticalStatus.Team[CREATURE_TEAM].bFirstID; i <= gTacticalStatus.Team[CREATURE_TEAM].bLastID; ++i )
	{
		pSoldier = MercPtrs[i];
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			if ( pSoldier->IsZombie() )
			{
				( *pubNumCreatures )++;
				( *pubNumZombies )++;
			}
			else if ( pSoldier->ubBodyType == BLOODCAT )
			{
				( *pubNumCreatures )++;
				( *pubNumBloodcats )++;
			}
			else
			{
				( *pubNumCreatures )++;
				( *pubNumBandits )++;
			}
		}
	}
}

BOOLEAN PrepareCreaturesForBattle()
{
	UNDERGROUND_SECTORINFO *pSector;
	INT32 i, iRandom;
	SGPPaletteEntry	LColors[3];
	UINT8 ubNumColors;
	BOOLEAN fQueen;
	UINT8 ubLarvaePercentage;
	UINT8 ubInfantPercentage;
	UINT8 ubYoungMalePercentage;
	UINT8 ubYoungFemalePercentage;
	UINT8 ubAdultMalePercentage;
	UINT8 ubAdultFemalePercentage;
	UINT8 ubCreatureHabitat;
	UINT8 ubNumLarvae = 0;
	UINT8 ubNumInfants = 0;
	UINT8 ubNumYoungMales = 0;
	UINT8 ubNumYoungFemales = 0;
	UINT8 ubNumAdultMales = 0;
	UINT8 ubNumAdultFemales = 0;
	UINT8 ubNumCreatures;

	if( !gubCreatureBattleCode )
	{
		ubNumColors = LightGetColors( LColors );

		if ( !gbWorldSectorZ )
		{
			UseCreatureMusic(LColors->peBlue);
			return FALSE;	//Creatures don't attack overworld with this battle code.
		}

		pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		if( !pSector )
		{
			return FALSE;
		}

		//if( ubNumColors != 1 )
		//	ScreenMsg( FONT_RED, MSG_ERROR, L"This map has more than one light color -- KM, LC : 1" );

		//By default, we only play creature music in the cave levels (the creature levels all consistently
		//have blue lights while human occupied mines have red lights.	We always play creature music
		//when creatures are in the level.
		BOOLEAN fCreatures = pSector->ubNumCreatures > 0;

		switch (pSector->ubMusicMode)
		{
			case CM_AUTO:
				UseCreatureMusic(fCreatures);
				break;
			case CM_NEVER:
				UseCreatureMusic(FALSE);
				break;
			case CM_ALWAYS:
				UseCreatureMusic(TRUE);
				break;
			case CM_COMPAT:
			default:
				UseCreatureMusic(LColors->peBlue || fCreatures);
				break;
		}

		if( !fCreatures )
		{
			return FALSE;
		}

		//gfUseCreatureMusic = TRUE; //creatures are here, so play creature music
		ubCreatureHabitat = pSector->ubCreatureHabitat;
		ubNumCreatures = pSector->ubNumCreatures;
	}
	else
	{
		//creatures are attacking a town sector
		//gfUseCreatureMusic = TRUE;
		UseCreatureMusic(TRUE);
#ifdef NEWMUSIC
		GlobalSoundID  = MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ];
		if ( MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] != -1 )
			SetMusicModeID( MUSIC_TACTICAL_NOTHING, MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] );
		else
#endif
		SetMusicMode( MUSIC_TACTICAL_NOTHING );
					
		// externalize to xml data
		//ubCreatureHabitat = MINE_EXIT;
		
		//assign the B1 underground habitat composition to the attacking creatures
		for (UINT8 i = 0; i < MAX_NUMBER_OF_CREATURE_SECTORS; ++i)
		{
			INT16 sX = gCreaturePlacements[ giLairID ].sAttackSourceX;
			INT16 sY = gCreaturePlacements[ giLairID ].sAttackSourceY;
			UINT8 ubZ = 1;
			
			if( sX == gCreaturePlacements[ giLairID ].Habitat[ i ].sX &&
				sY == gCreaturePlacements[ giLairID ].Habitat[ i ].sY &&
				ubZ == gCreaturePlacements[ giLairID ].Habitat[ i ].ubZ )
			{
				ubCreatureHabitat = gCreaturePlacements[ giLairID ].Habitat[ i ].ubComposition;
				break;
			}
		}

		ubNumCreatures = gubNumCreaturesAttackingTown;
	}

	/* // externalize to xml data
	switch( ubCreatureHabitat )
	{
		case QUEEN_LAIR:
			fQueen = TRUE;
			ubLarvaePercentage = 20;
			ubInfantPercentage = 40;
			ubYoungMalePercentage = 0;
			ubYoungFemalePercentage = 0;
			ubAdultMalePercentage = 30;
			ubAdultFemalePercentage = 10;
			break;
		case LAIR:
			fQueen = FALSE;
			ubLarvaePercentage = 15;
			ubInfantPercentage = 35;
			ubYoungMalePercentage = 10;
			ubYoungFemalePercentage = 5;
			ubAdultMalePercentage = 25;
			ubAdultFemalePercentage = 10;
			break;
		case LAIR_ENTRANCE:
			fQueen = FALSE;
			ubLarvaePercentage = 0;
			ubInfantPercentage = 15;
			ubYoungMalePercentage = 30;
			ubYoungFemalePercentage = 10;
			ubAdultMalePercentage = 35;
			ubAdultFemalePercentage = 10;
			break;
		case INNER_MINE:
			fQueen = FALSE;
			ubLarvaePercentage = 0;
			ubInfantPercentage = 0;
			ubYoungMalePercentage = 20;
			ubYoungFemalePercentage = 40;
			ubAdultMalePercentage = 10;
			ubAdultFemalePercentage = 30;
			break;
		case OUTER_MINE:
		case FEEDING_GROUNDS:
		case MINE_EXIT:
			fQueen = FALSE;
			ubLarvaePercentage = 0;
			ubInfantPercentage = 0;
			ubYoungMalePercentage = 10;
			ubYoungFemalePercentage = 65;
			ubAdultMalePercentage = 5;
			ubAdultFemalePercentage = 20;
			break;
		default:
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_RED, MSG_ERROR, L"Invalid creature habitat ID of %d for PrepareCreaturesForBattle.	Ignoring...", ubCreatureHabitat );
			#endif
			return FALSE;
	}
	*/

	if ( guCreatureAttackType == CREATURE_ATTACK_TYPE_CREATURE )
	{
		//out-of-range habitat
		if ( ubCreatureHabitat >= NUMBER_OF_CREATURE_COMPOSITIONS )
		{
			AssertMsg( 0, String( "Invalid creature habitat ID of %d for PrepareCreaturesForBattle.", ubCreatureHabitat ) );
		}

		//queen sector
		if ( !ubCreatureHabitat )
			fQueen = TRUE;
		else
			fQueen = FALSE;

		ubLarvaePercentage = gCreatureComposition[ubCreatureHabitat].ubLarvaePercent;
		ubInfantPercentage = gCreatureComposition[ubCreatureHabitat].ubInfantPercent;
		ubYoungMalePercentage = gCreatureComposition[ubCreatureHabitat].ubYoungMalePercent;
		ubYoungFemalePercentage = gCreatureComposition[ubCreatureHabitat].ubYoungFemalePercent;
		ubAdultMalePercentage = gCreatureComposition[ubCreatureHabitat].ubAdultMalePercent;
		ubAdultFemalePercentage = gCreatureComposition[ubCreatureHabitat].ubAdultFemalePercent;

		//First step is to convert the percentages into the numbers we will use.
		if ( fQueen )
		{
			ubNumCreatures--;
		}
		ubInfantPercentage += ubLarvaePercentage;
		ubYoungMalePercentage += ubInfantPercentage;
		ubYoungFemalePercentage += ubYoungMalePercentage;
		ubAdultMalePercentage += ubYoungFemalePercentage;
		ubAdultFemalePercentage += ubAdultMalePercentage;
		if ( ubAdultFemalePercentage != 100 )
		{
			AssertMsg( 0, "Percentage for adding creatures don't add up to 100." );
		}
		//Second step is to determine the breakdown of the creatures randomly.
		i = ubNumCreatures;
		while ( i-- )
		{
			iRandom = Random( 100 );
			if ( iRandom < ubLarvaePercentage )
				ubNumLarvae++;
			else if ( iRandom < ubInfantPercentage )
				ubNumInfants++;
			else if ( iRandom < ubYoungMalePercentage )
				ubNumYoungMales++;
			else if ( iRandom < ubYoungFemalePercentage )
				ubNumYoungFemales++;
			else if ( iRandom < ubAdultMalePercentage )
				ubNumAdultMales++;
			else
				ubNumAdultFemales++;
		}
	}

	if( gbWorldSectorZ )
	{
		UNDERGROUND_SECTORINFO *pUndergroundSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

		if( !pUndergroundSector )
		{
			//No info?!!!!!
			AssertMsg( 0, "Please report underground sector you are in or going to and send save if possible.	KM : 0" );
			return FALSE;
		}

		pUndergroundSector->ubCreaturesInBattle = pUndergroundSector->ubNumCreatures;
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

		switch ( guCreatureAttackType )
		{
		case CREATURE_ATTACK_TYPE_CREATURE:
		case CREATURE_ATTACK_TYPE_ZOMBIE:
		case CREATURE_ATTACK_TYPE_BANDIT:
			pSector->ubNumCreatures = ubNumCreatures;
			pSector->ubCreaturesInBattle = ubNumCreatures;
			break;

		case CREATURE_ATTACK_TYPE_BLOODCAT:
			pSector->bBloodCats = ubNumCreatures;
			pSector->ubCreaturesInBattle = ubNumCreatures;
			break;
		}
	}
	
	switch( gubCreatureBattleCode )
	{
		case CREATURE_BATTLE_CODE_NONE: //in the mines
			switch ( guCreatureAttackType )
			{
			case CREATURE_ATTACK_TYPE_CREATURE:
				AddSoldierInitListCreatures( fQueen, ubNumLarvae, ubNumInfants, ubNumYoungMales, ubNumYoungFemales, ubNumAdultMales, ubNumAdultFemales );
				break;

			case CREATURE_ATTACK_TYPE_ZOMBIE:
			case CREATURE_ATTACK_TYPE_BANDIT:
			case CREATURE_ATTACK_TYPE_BLOODCAT:
				AddSoldierInitListOtherCreatures( ubNumCreatures );
				break;
			}
			break;
			
		case CREATURE_BATTLE_CODE_TACTICALLYADD: //creature attacking a town sector
		case CREATURE_BATTLE_CODE_PREBATTLEINTERFACE:
			switch ( guCreatureAttackType )
			{
			case CREATURE_ATTACK_TYPE_CREATURE:
				AddCreaturesToBattle( ubNumYoungMales, ubNumYoungFemales, ubNumAdultMales, ubNumAdultFemales );
				break;

			case CREATURE_ATTACK_TYPE_ZOMBIE:
			case CREATURE_ATTACK_TYPE_BANDIT:
			case CREATURE_ATTACK_TYPE_BLOODCAT:
				AddCreaturesToBattle_Other( ubNumCreatures );
				break;
			}
			break;

		case CREATURE_BATTLE_CODE_AUTORESOLVE:
			return FALSE;
	}

	return TRUE;
}

void CreatureNightPlanning()
{
	//Check the populations of the mine exits, and factor a chance for them to attack at night.
			
	UINT8 ubSectorID = SECTOR( gCreaturePlacements[ giLairID ].sAttackSourceX, gCreaturePlacements[ giLairID ].sAttackSourceY );

	// Attacksource B1 underground sector must be a valid creature habitat!
	UINT8 ubNumCreatures = CreaturesInUndergroundSector( ubSectorID, 1 );

	//10% chance for each creature with difficulty modifier to decide it's time to attack.	
	if ( ubNumCreatures > 1 && ubNumCreatures * 10 + zDiffSetting[gGameOptions.ubDifficultyLevel].iCreatureTownAggressiveness > (INT32)PreRandom( 100 ) )
	{
		AddStrategicEvent( EVENT_CREATURE_ATTACK, GetWorldTotalMin() + 1 + PreRandom( 429 ), ubSectorID );
	}
}


void CheckConditionsForTriggeringCreatureQuest( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{

// move to Scripts/StrategicTownLoyalty.lua
	UINT8 ubValidMines = 0;

	if (gGameOptions.ubGameStyle == STYLE_REALISTIC || !gGameExternalOptions.fEnableCrepitus)
		return;
	
	/*
	if( !(gGameOptions.ubGameStyle == STYLE_SCIFI) || !gGameExternalOptions.fEnableCrepitus)
		return; //No scifi, no creatures...
	*/

	if( giLairID )
		return;	//Creature quest already begun

	//Count the number of "infectible mines" the player occupies
	if( !StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_D13 ) ].fEnemyControlled )
	{
		ubValidMines++;
	}
	if( !StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_H8 ) ].fEnemyControlled )
	{
		ubValidMines++;
	}
	if( !StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_I14 ) ].fEnemyControlled )
	{
		ubValidMines++;
	}
	if( !StrategicMap[ SECTOR_INFO_TO_STRATEGIC_INDEX( SEC_H3 ) ].fEnemyControlled )
	{
		ubValidMines++;
	}

	if( ubValidMines >= 3 )
	{
		InitCreatureQuest();
	}
}


BOOLEAN SaveCreatureDirectives( HWFILE hFile )
{
	UINT32 uiNumBytesWritten;

	FileWrite( hFile, &giHabitatedDistance, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( INT32 ) )
	{
		return( FALSE );
	}

	FileWrite( hFile, &giPopulationModifier, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( INT32 ) )
	{
		return( FALSE );
	}
	FileWrite( hFile, &giLairID, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( INT32 ) )
	{
		return( FALSE );
	}
	BOOLEAN fUseCreatureMusic = UsingCreatureMusic();
	FileWrite( hFile, &fUseCreatureMusic, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( BOOLEAN ) )
	{
		return( FALSE );
	}
	FileWrite( hFile, &giDestroyedLairID, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( INT32 ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadCreatureDirectives( HWFILE hFile, UINT32 uiSavedGameVersion )
{
	UINT32 uiNumBytesRead;
	FileRead( hFile, &giHabitatedDistance, 4, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( INT32 ) )
	{
		return( FALSE );
	}

	FileRead( hFile, &giPopulationModifier, 4, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( INT32 ) )
	{
		return( FALSE );
	}
	FileRead( hFile, &giLairID, 4, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( INT32 ) )
	{
		return( FALSE );
	}

	BOOLEAN fUseCreatureMusic;
	FileRead( hFile, &fUseCreatureMusic, 1, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( BOOLEAN ) )
	{
		return( FALSE );
	}
	UseCreatureMusic(fUseCreatureMusic);

	if( uiSavedGameVersion >= 82 )
	{
		FileRead( hFile, &giDestroyedLairID, 4, &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( INT32 ) )
		{
			return( FALSE );
		}
	}
	else
	{
		giDestroyedLairID = 0;
	}

	#ifdef JA2BETAVERSION
		if( gfClearCreatureQuest && giLairID != -1 )
		{
			giLairID = 0;
#ifdef JA2UB 
// no UB
#else
			gfCreatureMeanwhileScenePlayed = FALSE;
			uiMeanWhileFlags &= ~(0x00000800);
#endif
		}
		gfClearCreatureQuest = FALSE;
	#endif

	/* // externalize to xml data
	switch( giLairID )
	{
		case -1:
			break; //creature quest finished -- it's okay
		case 0:
			break; //lair doesn't exist yet -- it's okay
		case 1:
			InitLairDrassen();
			break;
		case 2:
			InitLairAlma();
			break;
		case 3:
			InitLairCambria();
			break;
		case 4:
			InitLairGrumm();
			break;
		default:
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_RED, MSG_ERROR, L"Invalid restoration of creature lair ID of %d.	Save game potentially hosed.", giLairID );
			#endif
			break;
	}
	*/
	
	//count infectible sites defined initmines.lua script
	INT32 iNumMinesInfectibleLUA=0;

	for (INT32 x = 0; x < MAX_NUMBER_OF_MINES; x++)
	{
		if( gMineStatus[ x ].fInfectible )
			iNumMinesInfectibleLUA++;
	}

	if( giLairID <= 0 )
	{
		//quest finished/lair doesn't exist yet -- it's okay
	}
	else if( giLairID <= min( NUMBER_OF_INFECTIBLE_SITES, iNumMinesInfectibleLUA ) )
	{	
		InitLair( giLairID );
	}
	else
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_RED, MSG_ERROR, L"Invalid restoration of creature lair ID of %d.	Save game potentially hosed.", giLairID );
		#endif
	}


	return( TRUE );
}

void ForceCreaturesToAvoidMineTemporarily( UINT8 ubMineIndex )
{
	gMineStatus[ ubMineIndex ].usValidDayCreaturesCanInfest = (UINT16)(GetWorldDay() + 2);
}

BOOLEAN PlayerGroupIsInACreatureInfestedMine()
{
	CREATURE_DIRECTIVE *curr;
	SOLDIERTYPE *pSoldier;
	INT32 i;
	INT16 sSectorX, sSectorY;
	INT8 bSectorZ;

	if( giLairID <= 0 )
	{ //Creature quest inactive
		return FALSE;
	}

	//Lair is active, so look for live soldier in any creature level
	curr = lair;
	while( curr )
	{
		sSectorX = curr->pLevel->ubSectorX;
		sSectorY = curr->pLevel->ubSectorY;
		bSectorZ = (INT8)curr->pLevel->ubSectorZ;
		//Loop through all the creature directives (mine sectors that are infectible) and
		//see if players are there.
		for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
		{
			pSoldier = MercPtrs[ i ];
			if( pSoldier->bActive && pSoldier->stats.bLife &&
					pSoldier->sSectorX == sSectorX &&
					pSoldier->sSectorY == sSectorY &&
					pSoldier->bSectorZ == bSectorZ &&
					!pSoldier->flags.fBetweenSectors )
			{
				return TRUE;
			}
		}
		curr = curr->next;
	}

	//Lair is active, but no mercs are in these sectors
	return FALSE;
}

//Returns TRUE if valid and creature quest over, FALSE if creature quest active or not yet started
BOOLEAN GetWarpOutOfMineCodes( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ, INT32 *psInsertionGridNo )
{
	INT32 iSwitchValue;

	if( !gfWorldLoaded )
	{
		return FALSE;
	}

	if ( gbWorldSectorZ == 0 )
	{
	return( FALSE );
	}

	iSwitchValue = giLairID;

	if( iSwitchValue == -1 )
	{
		iSwitchValue = giDestroyedLairID;
	}

	if( !iSwitchValue )
	{
		return( FALSE );
	}
	
	/* // externalize to xml data
	//Now make sure the mercs are in the previously infested mine
	switch( iSwitchValue )
	{
		case 1: //Drassen
			if( gWorldSectorX == 13 && gWorldSectorY == 6 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 13 && gWorldSectorY == 7 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 13 && gWorldSectorY == 7 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 13 && gWorldSectorY == 6 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 13 && gWorldSectorY == 5 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 13 && gWorldSectorY == 5 && gbWorldSectorZ == 1 ||
					gWorldSectorX == 13 && gWorldSectorY == 4 && gbWorldSectorZ == 1 )
			{
				*psSectorX = 13;
				*psSectorY = 4;
				*pbSectorZ = 0;
				*psInsertionGridNo = 20700;//dnl!!!
				return TRUE;
			}
			break;
		case 2: //Alma
			if( gWorldSectorX == 13 && gWorldSectorY == 11 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 13 && gWorldSectorY == 10 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 13 && gWorldSectorY == 10 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 14 && gWorldSectorY == 10 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 14 && gWorldSectorY == 10 && gbWorldSectorZ == 1 ||
					gWorldSectorX == 14 && gWorldSectorY == 9 && gbWorldSectorZ == 1 )
			{
				*psSectorX = 14;
				*psSectorY = 9;
				*pbSectorZ = 0;
				*psInsertionGridNo = 9085;//dnl!!!
				return TRUE;
			}
			break;
		case 3: //Cambria
			if( gWorldSectorX == 8 && gWorldSectorY == 9 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 8 && gWorldSectorY == 8 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 8 && gWorldSectorY == 8 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 9 && gWorldSectorY == 8 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 9 && gWorldSectorY == 8 && gbWorldSectorZ == 1 ||
					gWorldSectorX == 8 && gWorldSectorY == 8 && gbWorldSectorZ == 1 )
			{
				*psSectorX = 8;
				*psSectorY = 8;
				*pbSectorZ = 0;
				*psInsertionGridNo = 13002;//dnl!!!
				return TRUE;
			}
			break;
		case 4: //Grumm
			if( gWorldSectorX == 4 && gWorldSectorY == 7 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 4 && gWorldSectorY == 8 && gbWorldSectorZ == 3 ||
					gWorldSectorX == 3 && gWorldSectorY == 8 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 3 && gWorldSectorY == 8 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 3 && gWorldSectorY == 9 && gbWorldSectorZ == 2 ||
					gWorldSectorX == 3 && gWorldSectorY == 9 && gbWorldSectorZ == 1 ||
					gWorldSectorX == 3 && gWorldSectorY == 8 && gbWorldSectorZ == 1 )
			{
				*psSectorX = 3;
				*psSectorY = 8;
				*pbSectorZ = 0;
				*psInsertionGridNo = 9822;//dnl!!!
				return TRUE;
			}
			break;
	}
	*/

	//Now make sure the mercs are in the previously infested mine
	else
	{
		// queen sector
		if( gWorldSectorX == gCreaturePlacements[ iSwitchValue ].sQueenX
			&& gWorldSectorY == gCreaturePlacements[ iSwitchValue ].sQueenY
			&& gbWorldSectorZ == gCreaturePlacements[ iSwitchValue ].ubQueenZ )
		{
			*psSectorX = gCreaturePlacements[ iSwitchValue ].sWarpToX;
			*psSectorY = gCreaturePlacements[ iSwitchValue ].sWarpToY;
			*pbSectorZ = gCreaturePlacements[ iSwitchValue ].ubWarpToZ;
			*psInsertionGridNo = gCreaturePlacements[ iSwitchValue ].iWarpToGridNo;
			return( TRUE );
		}
		
		// other creature habitat sector
		for (UINT8 i = 0; i < MAX_NUMBER_OF_CREATURE_SECTORS; i++)
		{
			if( gWorldSectorX == gCreaturePlacements[ iSwitchValue ].Habitat[ i ].sX
				&& gWorldSectorY == gCreaturePlacements[ iSwitchValue ].Habitat[ i ].sY
				&& gbWorldSectorZ == gCreaturePlacements[ iSwitchValue ].Habitat[ i ].ubZ )
			{
				*psSectorX = gCreaturePlacements[ iSwitchValue ].sWarpToX;
				*psSectorY = gCreaturePlacements[ iSwitchValue ].sWarpToY;
				*pbSectorZ = gCreaturePlacements[ iSwitchValue ].ubWarpToZ;
				*psInsertionGridNo = gCreaturePlacements[ iSwitchValue ].iWarpToGridNo;
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

// Flugente: reset code for creature attacks
void ResetCreatureAttackVariables()
{
	gsCreatureInsertionCode = 0;
	gsCreatureInsertionGridNo = 0;
	gubNumCreaturesAttackingTown = 0;
	gubYoungMalesAttackingTown = 0;
	gubYoungFemalesAttackingTown = 0;
	gubAdultMalesAttackingTown = 0;
	gubAdultFemalesAttackingTown = 0;
	gubCreatureBattleCode = CREATURE_BATTLE_CODE_NONE;
	gubSectorIDOfCreatureAttack = 0;
}