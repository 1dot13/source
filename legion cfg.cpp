//legion 2
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
	#include "legion cfg.h" //legion2
	//#include "XMLWriter.h"
#else
	#include "Types.h"
	#include "types.h"
	#include "Random.h"
	#include "Campaign Types.h"
	#include "Queen Command.h"
	#include "overhead.h"
	#include "Strategic Movement.h"
	#include "Strategic Status.h"
	#include "GameSettings.h"
	#include "Strategic AI.h"
	#include "XML.h"
	#include "INIReader.h"
	#include "Game Event Hook.h"
	#include "sgp.h"
	#include "jascreens.h"
	#include "laptop.h"
	#include "worlddef.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "email.h"
	#include "soldier profile.h"
	#include "strategicmap.h"
	#include "game init.h"
	#include "animation data.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "strategic.h"
	#include "Squads.h"
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
	#include "Quests.h"
	#include "Strategic AI.h"
	#include "dialogue control.h"
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

#include "legion cfg.h" //legion2
#include "Campaign Types.h"
#include "Interface.h"
#include "Map Screen Interface Map.h"
#include "renderworld.h"
#include "connect.h"
#include "XMLWriter.h"
#include "Debug Control.h"
#include <vfs/Core/vfs.h>

GAME_LEGION_OPTIONS gGameLegionOptions; 

#define				GAME_LEGION_OPTIONS_FILE	"UB_Options.ini"

void LoadGameLegionOptions();

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

void LoadGameLegionOptions()
{
#ifdef JA2UB
	JA2_5_START_SECTOR_X = gGameExternalOptions.ubDefaultArrivalSectorX;
	JA2_5_START_SECTOR_Y = gGameExternalOptions.ubDefaultArrivalSectorY;
#endif

	gGameLegionOptions.MercStartingGear25XML = TRUE;
	gGameLegionOptions.LOCATEGRIDNO  = gGameExternalOptions.iInitialMercArrivalLocation;			
	gGameLegionOptions.LOCATEGRIDNO2 = 0;
	gGameLegionOptions.OLDUBInventory = FALSE;

	CIniReader iniReader(GAME_LEGION_OPTIONS_FILE);

	// -----------------------
	// Unfinished Business
	// -----------------------
	
	gGameLegionOptions.BorderTown = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_TOWN", TRUE);
	gGameLegionOptions.BorderMine = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_MINE", TRUE);
	gGameLegionOptions.BorderTeams = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_TEAMS", TRUE);
	gGameLegionOptions.BorderMilitia = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_MILITIA", FALSE);
	gGameLegionOptions.BorderAirspace = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_AIRSPACE", TRUE);
	gGameLegionOptions.BorderItem = iniReader.ReadBoolean("Unfinished Business Settings","BORDER_ITEM", TRUE);
		
	gGameLegionOptions.EventAttackInitialSectorIfPlayerStillThere = iniReader.ReadBoolean("Unfinished Business Settings","EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE", TRUE);	
	gGameLegionOptions.HandleAddingEnemiesToTunnelMaps = iniReader.ReadBoolean("Unfinished Business Settings","HANDLE_ADDING_ENEMIES_TO_TUNNEL_MAPS", TRUE);		
	gGameLegionOptions.ubEndDefaultSectorX	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_X", 16, 1, 16);
	gGameLegionOptions.ubEndDefaultSectorY	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_Y", 11, 1, 16);
	gGameLegionOptions.ubEndDefaultSectorZ	= iniReader.ReadInteger("Unfinished Business Settings","DEFAULT_END_SECTOR_Z", 0, 0, 16);	
	gGameLegionOptions.AutoResolove = iniReader.ReadBoolean("Unfinished Business Settings","AUTO_RESOLVE", FALSE);
	
	gGameLegionOptions.InitialHeliGridNo[ 0 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_1", 14947);
	gGameLegionOptions.InitialHeliGridNo[ 1 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_2", 15584);
	gGameLegionOptions.InitialHeliGridNo[ 2 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_3", 15754);
	gGameLegionOptions.InitialHeliGridNo[ 3 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_4", 16232);
	gGameLegionOptions.InitialHeliGridNo[ 4 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_5", 16067);
	gGameLegionOptions.InitialHeliGridNo[ 5 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_6", 16230);
	gGameLegionOptions.InitialHeliGridNo[ 6 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_GRIDNO_7", 15272);
	
	gGameLegionOptions.InitalHeliRandomTimes[ 0 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_1",  1300);
	gGameLegionOptions.InitalHeliRandomTimes[ 1 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_2",  2000);
	gGameLegionOptions.InitalHeliRandomTimes[ 2 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_3",  2750);
	gGameLegionOptions.InitalHeliRandomTimes[ 3 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_4",  3400);
	gGameLegionOptions.InitalHeliRandomTimes[ 4 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_5",  4160);
	gGameLegionOptions.InitalHeliRandomTimes[ 5 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_6",  4700);
	gGameLegionOptions.InitalHeliRandomTimes[ 6 ]  = iniReader.ReadInteger("Unfinished Business Settings","INITIAL_HELICOPTER_RANDOM_TIME_7",  5630);
		
	gGameLegionOptions.InGameHeliCrash = iniReader.ReadBoolean("Unfinished Business Settings","ENABLE_UB_HELICOPTER_CRASH", TRUE);
	gGameLegionOptions.JerryQuotes = iniReader.ReadBoolean("Unfinished Business Settings","JERRY_QUOTES", TRUE);
	gGameLegionOptions.InJerry = iniReader.ReadBoolean("Unfinished Business Settings","JERRY", TRUE);	
	gGameLegionOptions.JerryGridNo = iniReader.ReadInteger("Unfinished Business Settings","JERRY_GRIDNO", 15109);	
	gGameLegionOptions.InGameHeli = iniReader.ReadBoolean("Unfinished Business Settings","ENABLE_JA2_HELICOPTER_LANDING", FALSE);	

	gGameLegionOptions.SwitchInMorrisAreaGridNo = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_IN_MORRIS_AREA_GRIDNO", 15231);	
	gGameLegionOptions.SwitchToLaunchMisslesGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_TO_LAUNCH_MISSLES_GRIDNO_1", 14268);	
	gGameLegionOptions.SwitchToLaunchMisslesGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","SWITCH_TO_LAUNCH_MISSLES_GRIDNO_2", 15708);	
	gGameLegionOptions.RadioLocatorsGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","RADIO_LOCATORS_GRIDNO_1", 15070);	
	gGameLegionOptions.RadioLocatorsGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","RADIO_LOCATORS_GRIDNO_2", 14744);	
	gGameLegionOptions.PowergenSectorGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_1", 15100);	
	gGameLegionOptions.PowergenSectorGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_2", 12220);	
	gGameLegionOptions.PowergenSectorGridNo3 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_3", 14155);	
	gGameLegionOptions.PowergenSectorGridNo4 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_GRIDNO_4", 13980);	
	
	gGameLegionOptions.PowergenSectorExitgridGridNo = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_EXITGRID_GRIDNO", 19749);	
	gGameLegionOptions.PowergenFanSoundGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_FAN_SOUND_GRIDNO_1", 10979);	
	gGameLegionOptions.PowergenFanSoundGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_FAN_SOUND_GRIDNO_2", 19749);	
	gGameLegionOptions.StartFanbackupAgainGridNo = iniReader.ReadInteger("Unfinished Business Settings","START_FANBACKUP_AGAIN_GRIDNO", 10980);	
	gGameLegionOptions.StopPowergenFanGridNo = iniReader.ReadInteger("Unfinished Business Settings","STOP_POWERGEN_FAN_GRIDNO", 10980);

	//sec J13
	gGameLegionOptions.FanGridNo[0] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_1", 10978);
	gGameLegionOptions.FanGridNo[1] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_2", 10979);
	gGameLegionOptions.FanGridNo[2] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_3", 10980);
	gGameLegionOptions.FanGridNo[3] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_4", 10818);
	gGameLegionOptions.FanGridNo[4] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_5", 10819);
	gGameLegionOptions.FanGridNo[5] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_6", 10820);
	gGameLegionOptions.FanGridNo[6] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_7", 10658);
	gGameLegionOptions.FanGridNo[7] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_8", 10659);
	gGameLegionOptions.FanGridNo[8] = iniReader.ReadInteger("Unfinished Business Settings","FAN_GRIDNO_9", 10660);
	
	if ( gGameLegionOptions.InGameHeli == TRUE ) 
		gGameLegionOptions.InGameHeliCrash = FALSE;
	
	if ( gGameLegionOptions.InGameHeliCrash == TRUE )
		gGameLegionOptions.InGameHeli = FALSE;
	
	if ( gGameLegionOptions.InGameHeli == TRUE && gGameLegionOptions.InGameHeliCrash == TRUE ) 
	{
		gGameLegionOptions.InGameHeliCrash = TRUE;
		gGameLegionOptions.InGameHeli = FALSE;
	}

	// -----------------------
	// Campaign
	// -----------------------
	
	gGameLegionOptions.EnemyXML = iniReader.ReadBoolean("Campaign Settings","USE_ENEMY_ARMY_XMLS", TRUE);
	gGameLegionOptions.AddRandomEnemyToSector = iniReader.ReadBoolean("Campaign Settings","ADD_RANDOM_ENEMY_TO_SECTOR", FALSE);
	gGameLegionOptions.StrategicMovementCostsXML = iniReader.ReadBoolean("Campaign Settings","STRATEGIC_MOVEMENT_COSTS_XML", FALSE);
	gGameLegionOptions.MakeStrategicMovementCosts = iniReader.ReadBoolean("Campaign Settings","MAKE_STRATEGIC_MOVEMENT_COSTS", FALSE);	
	gGameLegionOptions.LaptopQuestEnabled = iniReader.ReadBoolean("Campaign Settings","LAPTOP_QUEST", TRUE);	
	gGameLegionOptions.pJA2UB = iniReader.ReadBoolean("Campaign Settings","JA2UB", TRUE);	
	gGameLegionOptions.fDeadMerc = iniReader.ReadBoolean("Campaign Settings","NOTIFY_ON_DEAD_AIM_MERCS", FALSE);		
			
	// -----------------------
	// Laptop
	// -----------------------

	gGameLegionOptions.LaptopIMPPassJA2  = iniReader.ReadBoolean("Laptop Settings","IMP_PASS_JA2",  FALSE);	
	gGameLegionOptions.LaptopIMPPassUB  = iniReader.ReadBoolean("Laptop Settings","IMP_PASS_UB",  TRUE);	
	gGameLegionOptions.LaptopLinkInsurance  = iniReader.ReadBoolean("Laptop Settings","LINK_INSURANCE",  FALSE);
	gGameLegionOptions.LaptopLinkFuneral  = iniReader.ReadBoolean("Laptop Settings","LINK_FUNERAL",  TRUE);
	gGameLegionOptions.LaptopLinkBobby  = iniReader.ReadBoolean("Laptop Settings","LINK_BOBBY",  FALSE);
	gGameLegionOptions.fBobbyRSite  = iniReader.ReadBoolean("Laptop Settings","BOBBY_SITE_ACCESSED",  FALSE);			
}
