#ifdef PRECOMPILEDHEADERS
	#include "Types.h"
	#include "types.h"
	#include "Strategic All.h"
	#include "XML.h"
	#include "INIReader.h"
	#include "GameSettings.h"
	#include "Soldier Profile.h"
	#include "XML.h"
	#include "Item Types.h"
	#include "Items.h"
	#include "Game Event Hook.h"
	#include "faces.h"
	#include "Language Defines.h"
	#include "Types.h"
	#include "Map Screen Interface Map.h"
	#include "ub_config.h" //legion2	
#else
	#include "Types.h"
	#include "types.h"
	#include "Campaign Init.h"
	#include "Random.h"
	#include "Campaign Types.h"
	#include "Queen Command.h"
	#include "overhead.h"
	#include "Strategic Movement.h"
	#include "Strategic Status.h"
	#include "GameSettings.h"
	#include "Creature Spreading.h"
	#include "Strategic AI.h"
	#include "XML.h"
	#include "INIReader.h"
	#include "Game init.h"
	#include "Game Event Hook.h"
	#include "sgp.h"
	#include "jascreens.h"
	#include "laptop.h"
	#include "worlddef.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "fade screen.h"
	#include "gamescreen.h" 
	#include "email.h"
	#include "Game Clock.h"
	#include "soldier profile.h"
	#include "strategicmap.h"
	#include "game init.h"
	#include "animation data.h"
	#include "finances.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "music control.h"
	#include "strategic.h"
	#include "history.h"
	#include "merc entering.h"
	#include "Squads.h"
	#include "Campaign Init.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "gameloop.h"
	#include "Random.h"
	#include "Map Screen Interface.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "Message.h"
	#include "Game Event Hook.h"
	#include "Strategic Movement.h"
	#include "Creature Spreading.h"
	#include "Quests.h"
	#include "Strategic AI.h"
	#include "sound control.h"
	#include "Laptopsave.h"
	#include "AimMembers.h"
	#include "dialogue control.h"
	#include "npc.h"
	#include "Init.h"
	#include "GameSettings.h"
	#include "INIReader.h"
	#include "Soldier Profile.h"
	#include "XML.h"
	#include "Item Types.h"
	#include "Items.h"
	#include "text.h"
	#include "GameSettings.h"
#endif

#ifdef JA2UB
#include "Ja25 Strategic Ai.h"
#include "Ja25Update.h"
#endif

#include "ub_config.h" //legion2
#include "Campaign Types.h"
#include "Interface.h"
#include "Map Screen Interface Map.h"
#include "renderworld.h"
#include "aim.h"
#include "connect.h"
#include "expat.h"
#include "XMLWriter.h"
#include "Debug Control.h"
#include <vfs/Core/vfs.h>

GAME_UB_OPTIONS gGameUBOptions; 

#define				GAME_UB_OPTIONS_FILE	"UB_Options.ini"

void LoadGameUBOptions();

#ifdef JA2UB
void RandomAddEnemy( UINT8 SectorX, UINT8 SectorY, UINT8 Level );
#endif

void RandomStats ();

#ifdef JA2UB

void RandomAddEnemy( UINT8 SectorX, UINT8 SectorY, UINT8 Level )
{
UNDERGROUND_SECTORINFO *pSector;
UINT8 ubNumAdmins = 0;
UINT8 ubNumTroops = 0;
UINT8 ubNumElites = 0;

	if ( Level != 0 )
	{
	pSector = FindUnderGroundSector( SectorX, SectorY, Level );
	if ( pSector )
	{
		if ( pSector->fVisited != TRUE )
		{
			switch( gGameOptions.ubDifficultyLevel )
			{
			case DIF_LEVEL_EASY:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 10 + Random( 5 );
				ubNumElites = Random( 4 );			
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 15 + Random( 8 );
				ubNumElites = 1 + Random( 2 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 20 + Random( 7 );
				ubNumElites = 2 + Random( 2 );	
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 20 + Random( 3 );
				ubNumElites = 6 + Random( 3 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			}
		}
	}
	}
	else
	{
		if ( !GetSectorFlagStatus( SectorX, SectorY, Level, SF_ALREADY_VISITED ) )
		{
			switch( gGameOptions.ubDifficultyLevel )
			{
			case DIF_LEVEL_EASY:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 10 + Random( 5 );
				ubNumElites = Random( 4 );			
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 15 + Random( 8 );
				ubNumElites = 1 + Random( 2 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 23 + Random( 7);
				ubNumElites = 2 + Random( 2 );	
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 20 + Random( 3 );
				ubNumElites = 6 + Random( 3 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites );
				break;
			}
		}
	}
}

#endif

void LoadGameUBOptions()
{
#ifdef JA2UB
	JA2_5_START_SECTOR_X = gGameExternalOptions.ubDefaultArrivalSectorX;
	JA2_5_START_SECTOR_Y = gGameExternalOptions.ubDefaultArrivalSectorY;
#endif

	gGameUBOptions.LOCATEGRIDNO  = gGameExternalOptions.iInitialMercArrivalLocation;			
	gGameUBOptions.LOCATEGRIDNO2 = 0;

	CIniReader iniReader(GAME_UB_OPTIONS_FILE);

	// -----------------------
	// Unfinished Business
	// -----------------------
	
	gGameUBOptions.BorderTown = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_TOWN", TRUE);
	gGameUBOptions.BorderMine = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_MINE", TRUE);
	gGameUBOptions.BorderTeams = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_TEAMS", TRUE);
	gGameUBOptions.BorderMilitia = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_MILITIA", FALSE);
	gGameUBOptions.BorderAirspace = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_AIRSPACE", TRUE);
	gGameUBOptions.BorderItem = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_ITEM", TRUE);
		
	gGameUBOptions.EventAttackInitialSectorIfPlayerStillThere = iniReader.ReadBoolean("Unfinished Business Settings","EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE", TRUE);	
	gGameUBOptions.HandleAddingEnemiesToTunnelMaps = iniReader.ReadBoolean("Unfinished Business Settings","HANDLE_ADDING_ENEMIES_TO_TUNNEL_MAPS", TRUE);		
	gGameUBOptions.ubEndDefaultSectorX	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_X", 16, 1, 16);
	gGameUBOptions.ubEndDefaultSectorY	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_Y", 11, 1, 16);
	gGameUBOptions.ubEndDefaultSectorZ	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_Z", 0, 0, 16);	
	gGameUBOptions.AutoResolve = iniReader.ReadBoolean("Unfinished Business Settings","AUTO_RESOLVE", FALSE);
	
	gGameUBOptions.InitialHeliGridNo[ 0 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_1", 14947);
	gGameUBOptions.InitialHeliGridNo[ 1 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_2", 15584);
	gGameUBOptions.InitialHeliGridNo[ 2 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_3", 15754);
	gGameUBOptions.InitialHeliGridNo[ 3 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_4", 16232);
	gGameUBOptions.InitialHeliGridNo[ 4 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_5", 16067);
	gGameUBOptions.InitialHeliGridNo[ 5 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_6", 16230);
	gGameUBOptions.InitialHeliGridNo[ 6 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_7", 15272);
	
	gGameUBOptions.InitalHeliRandomTimes[ 0 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_1",  1300);
	gGameUBOptions.InitalHeliRandomTimes[ 1 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_2",  2000);
	gGameUBOptions.InitalHeliRandomTimes[ 2 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_3",  2750);
	gGameUBOptions.InitalHeliRandomTimes[ 3 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_4",  3400);
	gGameUBOptions.InitalHeliRandomTimes[ 4 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_5",  4160);
	gGameUBOptions.InitalHeliRandomTimes[ 5 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_6",  4700);
	gGameUBOptions.InitalHeliRandomTimes[ 6 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_7",  5630);
		
	gGameUBOptions.InGameHeliCrash = iniReader.ReadBoolean("Unfinished Business Settings","ENABLE_UB_HELICOPTER_CRASH", TRUE);
	gGameUBOptions.JerryQuotes = iniReader.ReadBoolean("Unfinished Business Settings","JERRY_QUOTES", TRUE);
	gGameUBOptions.InJerry = iniReader.ReadBoolean("Unfinished Business Settings","JERRY", TRUE);	
	gGameUBOptions.JerryGridNo = iniReader.ReadInteger("Unfinished Business Settings","JERRY_GRIDNO", 15109);	
	gGameUBOptions.InGameHeli = iniReader.ReadBoolean("Unfinished Business Settings","ENABLE_JA2_HELICOPTER_LANDING", FALSE);	

	gGameUBOptions.SwitchInMorrisAreaGridNo = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_IN_MORRIS_AREA_GRIDNO", 15231);	
	gGameUBOptions.SwitchToLaunchMisslesGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_TO_LAUNCH_MISSLES_GRIDNO_1", 14268);	
	gGameUBOptions.SwitchToLaunchMisslesGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_TO_LAUNCH_MISSLES_GRIDNO_2", 15708);	
	gGameUBOptions.RadioLocatorsGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","RADIO_LOCATORS_GRIDNO_1", 15070);	
	gGameUBOptions.RadioLocatorsGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","RADIO_LOCATORS_GRIDNO_2", 14744);	
	gGameUBOptions.PowergenSectorGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_1", 15100);	
	gGameUBOptions.PowergenSectorGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_2", 12220);	
	gGameUBOptions.PowergenSectorGridNo3 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_3", 14155);	
	gGameUBOptions.PowergenSectorGridNo4 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_4", 13980);	
	
	gGameUBOptions.PowergenSectorExitgridGridNo = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_EXITGRID_GRIDNO", 19749);	
	gGameUBOptions.PowergenFanSoundGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_FAN_SOUND_GRIDNO_1", 10979);	
	gGameUBOptions.PowergenFanSoundGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_FAN_SOUND_GRIDNO_2", 19749);	
	gGameUBOptions.StartFanbackupAgainGridNo = iniReader.ReadInteger("Unfinished Business Settings","START_FANBACKUP_AGAIN_GRIDNO", 10980);	
	gGameUBOptions.StopPowergenFanGridNo = iniReader.ReadInteger("Unfinished Business Settings","STOP_POWERGEN_FAN_GRIDNO", 10980);

	//sec J13
	gGameUBOptions.FanGridNo[0] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_1", 10978);
	gGameUBOptions.FanGridNo[1] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_2", 10979);
	gGameUBOptions.FanGridNo[2] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_3", 10980);
	gGameUBOptions.FanGridNo[3] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_4", 10818);
	gGameUBOptions.FanGridNo[4] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_5", 10819);
	gGameUBOptions.FanGridNo[5] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_6", 10820);
	gGameUBOptions.FanGridNo[6] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_7", 10658);
	gGameUBOptions.FanGridNo[7] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_8", 10659);
	gGameUBOptions.FanGridNo[8] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_9", 10660);
	
	if ( gGameUBOptions.InGameHeli == TRUE ) 
		gGameUBOptions.InGameHeliCrash = FALSE;
	
	if ( gGameUBOptions.InGameHeliCrash == TRUE )
		gGameUBOptions.InGameHeli = FALSE;
	
	if ( gGameUBOptions.InGameHeli == TRUE && gGameUBOptions.InGameHeliCrash == TRUE ) 
	{
		gGameUBOptions.InGameHeliCrash = TRUE;
		gGameUBOptions.InGameHeli = FALSE;
	}

	// -----------------------
	// Campaign
	// -----------------------
	
	gGameUBOptions.EnemyXML = iniReader.ReadBoolean("Campaign Settings","USE_ENEMY_ARMY_XMLS", TRUE);
	gGameUBOptions.AddRandomEnemyToSector = iniReader.ReadBoolean("Campaign Settings","ADD_RANDOM_ENEMY_TO_SECTOR", FALSE);
	gGameUBOptions.StrategicMovementCostsXML = iniReader.ReadBoolean("Campaign Settings","STRATEGIC_MOVEMENT_COSTS_XML", FALSE);
	gGameUBOptions.MakeStrategicMovementCosts = iniReader.ReadBoolean("Campaign Settings","MAKE_STRATEGIC_MOVEMENT_COSTS", FALSE);	
	gGameUBOptions.LaptopQuestEnabled = iniReader.ReadBoolean("Campaign Settings","LAPTOP_QUEST", TRUE);	
	gGameUBOptions.pJA2UB = iniReader.ReadBoolean("Campaign Settings","JA2UB", TRUE);	
	gGameUBOptions.fDeadMerc = iniReader.ReadBoolean("Campaign Settings","NOTIFY_ON_DEAD_AIM_MERCS", FALSE);			
			
	// -----------------------
	// Laptop
	// -----------------------

	gGameUBOptions.LaptopIMPPassJA2  = iniReader.ReadBoolean("Laptop Settings","IMP_PASS_JA2",  FALSE);	
	gGameUBOptions.LaptopIMPPassUB  = iniReader.ReadBoolean("Laptop Settings","IMP_PASS_UB",  TRUE);	
	gGameUBOptions.LaptopLinkInsurance  = iniReader.ReadBoolean("Laptop Settings","LINK_INSURANCE",  FALSE);
	gGameUBOptions.LaptopLinkFuneral  = iniReader.ReadBoolean("Laptop Settings","LINK_FUNERAL",  TRUE);
	gGameUBOptions.LaptopLinkBobby  = iniReader.ReadBoolean("Laptop Settings","LINK_BOBBY",  FALSE);
	gGameUBOptions.fBobbyRSite  = iniReader.ReadBoolean("Laptop Settings","BOBBY_SITE_ACCESSED",  FALSE);			
}