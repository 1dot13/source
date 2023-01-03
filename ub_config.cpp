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
#include "Ja25_Tactical.h"
#include "Ja25Update.h"
#endif

#include "GameInitOptionsScreen.h"

#include "ub_config.h" //legion2
#include "Campaign Types.h"
#include "Interface.h"
#include "Map Screen Interface Map.h"
#include "renderworld.h"
#include "connect.h"
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
	UINT8 ubNumTanks = 0;
	UINT8 ubNumJeeps = 0;
	UINT8 ubNumRobots = 0;

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
					SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
					break;
				case DIF_LEVEL_MEDIUM:
					ubNumAdmins = Random( 0 );
					ubNumTroops = 15 + Random( 8 );
					ubNumElites = 1 + Random( 2 );		
					SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
					break;
				case DIF_LEVEL_HARD:
					ubNumAdmins = Random( 0 );
					ubNumTroops = 20 + Random( 7 );
					ubNumElites = 2 + Random( 2 );	
					SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
					break;
				case DIF_LEVEL_INSANE:
					ubNumAdmins = Random( 0 );
					ubNumTroops = 20 + Random( 3 );
					ubNumElites = 6 + Random( 3 );		
					SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
					break;
				default:
					ubNumAdmins = Random( 0 );
					ubNumTroops = 10 + Random( 5 );
					ubNumElites = Random( 4 );			
					SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
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
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 15 + Random( 8 );
				ubNumElites = 1 + Random( 2 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 23 + Random( 7);
				ubNumElites = 2 + Random( 2 );	
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 20 + Random( 3 );
				ubNumElites = 6 + Random( 3 );		
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
				break;
			default:
				ubNumAdmins = Random( 0 );
				ubNumTroops = 10 + Random( 5 );
				ubNumElites = Random( 4 );			
				SetNumberJa25EnemiesInSector( SectorX, SectorY, Level, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots );
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
	
	//L15-3
	gGameUBOptions.SectorLaunchMisslesX = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_LAUNCH_MISSLES_X", 15);
	gGameUBOptions.SectorLaunchMisslesY = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_LAUNCH_MISSLES_Y", 12); 
	gGameUBOptions.SectorLaunchMisslesZ = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_LAUNCH_MISSLES_Z", 3); 
	
	//J13-0
	gGameUBOptions.SectorFanX = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_X", 13);
	gGameUBOptions.SectorFanY = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_Y", 10); 
	gGameUBOptions.SectorFanZ = iniReader.ReadInteger("Unfinished Business Settings","POWERGEN_SECTOR_Z", 0); 
	
	//K14-1
	gGameUBOptions.SectorOpenGateInTunnelX = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_OPEN_GATE_IN_TUNNEL_X", 14);
	gGameUBOptions.SectorOpenGateInTunnelY = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_OPEN_GATE_IN_TUNNEL_Y", 11); 
	gGameUBOptions.SectorOpenGateInTunnelZ = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_OPEN_GATE_IN_TUNNEL_Z", 1); 
	
	//J14-1
	gGameUBOptions.ExitForFanToPowerGenSectorX = iniReader.ReadInteger("Unfinished Business Settings","EXIT_FOR_FAN_TO_POWER_GEN_SECTOR_X", 14);
	gGameUBOptions.ExitForFanToPowerGenSectorY = iniReader.ReadInteger("Unfinished Business Settings","EXIT_FOR_FAN_TO_POWER_GEN_SECTOR_Y", 10); 
	gGameUBOptions.ExitForFanToPowerGenSectorZ = iniReader.ReadInteger("Unfinished Business Settings","EXIT_FOR_FAN_TO_POWER_GEN_SECTOR_Z", 1); 
	
	
	//I13
	gGameUBOptions.MineSectorX = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_X", 13);
	gGameUBOptions.MineSectorY = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_Y", 9); 
	gGameUBOptions.MineSectorZ = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_Z",0); 
	
	gGameUBOptions.MineEntranceGridno = iniReader.ReadInteger("Unfinished Business Settings","MINE_ENTRANCE_GRIDNO",12421); 
	gGameUBOptions.MineGridnoAddStructToHead = iniReader.ReadInteger("Unfinished Business Settings","MINE_GRIDNO_ADD_STRUCT_TO_HEAD",12745); 
	
	gGameUBOptions.MineRemoveExitGridFromWorld1 = iniReader.ReadInteger("Unfinished Business Settings","MINE_REMOVE_EXIT_GRID_FROM_WORLD_1",12422); 
	gGameUBOptions.MineRemoveExitGridFromWorld2 = iniReader.ReadInteger("Unfinished Business Settings","MINE_REMOVE_EXIT_GRID_FROM_WORLD_2",12423); 

	gGameUBOptions.MineSectorUndergroundX = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_UNDERGROUND_X", 13);
	gGameUBOptions.MineSectorUndergroundY = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_UNDERGROUND_Y", 9); 
	gGameUBOptions.MineSectorUndergroundZ = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_UNDERGROUND_Z", 1); 

	gGameUBOptions.MineSectorUndergroundGridno1 = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_UNDERGROUND_GRIDNO_1", 13057);
	gGameUBOptions.MineSectorUndergroundGridno2 = iniReader.ReadInteger("Unfinished Business Settings","MINE_SECTOR_UNDERGROUND_GRIDNO_2", 12897); 
	
	//K15-1
	gGameUBOptions.SectorDoorInTunnelX = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_DOOR_IN_TUNNEL_X", 15); 	
	gGameUBOptions.SectorDoorInTunnelY = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_DOOR_IN_TUNNEL_Y", 11); 
	gGameUBOptions.SectorDoorInTunnelZ = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_DOOR_IN_TUNNEL_Z", 1); 
	
	gGameUBOptions.SectorDoorInTunnelGridNo = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_DOOR_IN_TUNNEL_GRIDNO", 11419); 
	
	//H9
	gGameUBOptions.SectorGuardPostX = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_GUARDPOST_X", 9); 	
	gGameUBOptions.SectorGuardPostY = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_GUARDPOST_Y", 8); 
	gGameUBOptions.SectorGuardPostZ = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_GUARDPOST_Z", 0); 
	
	gGameUBOptions.H9MoneyGridNo = iniReader.ReadInteger("Unfinished Business Settings","H9_MONEY_GRIDNO", 9026); 	
	gGameUBOptions.H9MoneyEasy = iniReader.ReadInteger("Unfinished Business Settings","H9_MONEY_EASY", 15000); 
	gGameUBOptions.H9MoneyMedium = iniReader.ReadInteger("Unfinished Business Settings","H9_MONEY_MEDIUM", 10000); 
	gGameUBOptions.H9MoneyHard = iniReader.ReadInteger("Unfinished Business Settings","H9_MONEY_HARD", 7000); 
	
	//I10
	gGameUBOptions.FristSectorTownX = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN1_X", 10); 	
	gGameUBOptions.FristSectorTownY = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN1_Y", 9); 
	gGameUBOptions.FristSectorTownZ = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN1_Z", 0); 
	
	gGameUBOptions.I10MoneyGridNo1 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_GRIDNO_1", 11894); 	
	gGameUBOptions.I10MoneyEasy1 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_EASY_1", 8000); 
	gGameUBOptions.I10MoneyMedium1 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_MEDIUM_1", 4000); 
	gGameUBOptions.I10MoneyHard1 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_HARD_1", 3000); 
	
	gGameUBOptions.I10MoneyGridNo2 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_GRIDNO_2", 7906); 	
	gGameUBOptions.I10MoneyEasy2 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_EASY_2", 12000); 
	gGameUBOptions.I10MoneyMedium2 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_MEDIUM_2", 6000); 
	gGameUBOptions.I10MoneyHard2 = iniReader.ReadInteger("Unfinished Business Settings","I10_MONEY_HARD_2", 5000); 
	
	//J11
	gGameUBOptions.SectorTown2X = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN2_X", 10); 	
	gGameUBOptions.SectorTown2Y = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN2_Y", 9); 
	gGameUBOptions.SectorTown2Z = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN2_Z", 0); 
	
	//H11
	gGameUBOptions.SectorTown3X = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN3_X", 11); 	
	gGameUBOptions.SectorTown3Y = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN3_Y", 8); 
	gGameUBOptions.SectorTown3Z = iniReader.ReadInteger("Unfinished Business Settings","SECTOR_TOWN3_Z", 0); 
	
	gGameUBOptions.SectorTownGridNo1a = iniReader.ReadInteger("Unfinished Business Settings","TOWN2_FROM_GRIDNO_1", 15446); 
	gGameUBOptions.SectorTownGridNo1b = iniReader.ReadInteger("Unfinished Business Settings","TOWN2_TO_ROOF_GRIDNO_1", 13993); 

	gGameUBOptions.SectorTownGridNo2a = iniReader.ReadInteger("Unfinished Business Settings","TOWN2_FROM_GRIDNO_2", 15436); 
	gGameUBOptions.SectorTownGridNo2b = iniReader.ReadInteger("Unfinished Business Settings","TOWN2_TO_ROOF_GRIDNO_2", 14006); 
	
	gGameUBOptions.SectorTownGridNo3a = iniReader.ReadInteger("Unfinished Business Settings","TOWN3_FROM_GRIDNO_1", 8711); 
	gGameUBOptions.SectorTownGridNo3b = iniReader.ReadInteger("Unfinished Business Settings","TOWN3_TO_ROOF_GRIDNO_1", 5521); 
	
	//I9
	gGameUBOptions.I9SectorPlayerQuoteX = iniReader.ReadInteger("Unfinished Business Settings","I9_SECTOR_PLAYER_QUOTE_X", 9); 	
	gGameUBOptions.I9SectorPlayerQuoteY = iniReader.ReadInteger("Unfinished Business Settings","I9_SECTOR_PLAYER_QUOTE_Y", 9); 
	gGameUBOptions.I9SectorPlayerQuoteZ = iniReader.ReadInteger("Unfinished Business Settings","I9_SECTOR_PLAYER_QUOTE_Z", 0); 
	
	//H10
	gGameUBOptions.H10SectorPlayerQuoteX = iniReader.ReadInteger("Unfinished Business Settings","H10_SECTOR_PLAYER_QUOTE_X", 10); 	
	gGameUBOptions.H10SectorPlayerQuoteY = iniReader.ReadInteger("Unfinished Business Settings","H10_SECTOR_PLAYER_QUOTE_Y", 8); 
	gGameUBOptions.H10SectorPlayerQuoteZ = iniReader.ReadInteger("Unfinished Business Settings","H10_SECTOR_PLAYER_QUOTE_Z", 0); 


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

	// -----------------------
	// Weapon
	// -----------------------
	#ifdef JA2UB
	gGameUBOptions.ubTEX_MOVIE_ATTACK_CLYDESDALES = iniReader.ReadInteger("Unfinished Business Items","TEX_MOVIE_ATTACK_CLYDESDALES", 1356);
	gGameUBOptions.ubTEX_MOVIE_WILD_EAST = iniReader.ReadInteger("Unfinished Business Items","TEX_MOVIE_WILD_EAST", 1357);
	gGameUBOptions.ubTEX_MOVIE_HAVE_HONDA = iniReader.ReadInteger("Unfinished Business Items","TEX_MOVIE_HAVE_HONDA", 1358);
	gGameUBOptions.ubLAPTOP_TRANSMITTER = iniReader.ReadInteger("Unfinished Business Items","LAPTOP_TRANSMITTER", 1355);
	gGameUBOptions.ubCHE_GUEVARA_CANTEEN = iniReader.ReadInteger("Unfinished Business Items","CHE_GUEVARA_CANTEEN", 1359);
	gGameUBOptions.ubMERC_WRISTWATCH = iniReader.ReadInteger("Unfinished Business Items","MERC_WRISTWATCH", 1360);
	gGameUBOptions.ubSAM_GARVER_COMBAT_KNIFE = iniReader.ReadInteger("Unfinished Business Items","SAM_GARVER_COMBAT_KNIFE", 1353);
	gGameUBOptions.ubMERC_UMBRELLA_OLD = iniReader.ReadInteger("Unfinished Business Items","MERC_UMBRELLA_OLD", 1361);
	gGameUBOptions.ubMORRIS_INSTRUCTION_NOTE = iniReader.ReadInteger("Unfinished Business Items","MORRIS_INSTRUCTION_NOTE", 1362);
	gGameUBOptions.ubHAND_CANNON = iniReader.ReadInteger("Unfinished Business Items","HAND_CANNON", 1352);
	gGameUBOptions.ubHARTFORD_6_SHOOTER = iniReader.ReadInteger("Unfinished Business Items","HARTFORD_6_SHOOTER", 66);
	gGameUBOptions.ubMERC_UMBRELLA = iniReader.ReadInteger("Unfinished Business Items","MERC_UMBRELLA", 1361);
	gGameUBOptions.ubCLIP_CANNON_BALL = iniReader.ReadInteger("Unfinished Business Items","CLIP_CANNON_BALL", 1354);
	gGameUBOptions.ubBARRETT_UB = iniReader.ReadInteger("Unfinished Business Items","BARRETT_UB", 335);
	gGameUBOptions.ubCALICO_960_UB = iniReader.ReadInteger("Unfinished Business Items","CALICO_960_UB", 69);
	gGameUBOptions.ubPSG1_UB = iniReader.ReadInteger("Unfinished Business Items","PSG1_UB", 334);
	gGameUBOptions.ubL85_UB = iniReader.ReadInteger("Unfinished Business Items","L85_UB", 331);
	gGameUBOptions.ubTAR21_UB = iniReader.ReadInteger("Unfinished Business Items","TAR21_UB", 332);
	gGameUBOptions.ubVAL_SILENT_UB  = iniReader.ReadInteger("Unfinished Business Items","VAL_SILENT_UB", 333);
	gGameUBOptions.ubMICRO_UZI_UB  = iniReader.ReadInteger("Unfinished Business Items","MICRO_UZI_UB", 330);
	gGameUBOptions.ubCALICO_950_UB  = iniReader.ReadInteger("Unfinished Business Items","CALICO_950_UB", 67);
	gGameUBOptions.ubCALICO_900_UB  = iniReader.ReadInteger("Unfinished Business Items","CALICO_900_UB", 68);

	// -----------------------
	// Profiles
	// -----------------------
	
	gGameUBOptions.ubMANUEL_UB = iniReader.ReadInteger("Unfinished Business Profiles","MANUEL", 60);
	gGameUBOptions.ubBIGGENS_UB = iniReader.ReadInteger("Unfinished Business Profiles","BIGENS", 61);
	gGameUBOptions.ubJOHN_K_UB = iniReader.ReadInteger("Unfinished Business Profiles","JOHN_K", 62);
	gGameUBOptions.ubTEX_UB = iniReader.ReadInteger("Unfinished Business Profiles","TEX", 64);
	gGameUBOptions.ubGASTON_UB = iniReader.ReadInteger("Unfinished Business Profiles","GASTON", 58);
	gGameUBOptions.ubSTOGIE_UB = iniReader.ReadInteger("Unfinished Business Profiles","STOGIE", 59);
	gGameUBOptions.ubJERRY_MILO_UB = iniReader.ReadInteger("Unfinished Business Profiles","JERRY", 76);
	gGameUBOptions.ubPGMALE4_UB = iniReader.ReadInteger("Unfinished Business Profiles","PGMALE4", 57);
	gGameUBOptions.ubBETTY_UB = iniReader.ReadInteger("Unfinished Business Profiles","BETTY", 73);
	gGameUBOptions.ubRAUL_UB = iniReader.ReadInteger("Unfinished Business Profiles","RAUL", 74);
	gGameUBOptions.ubMORRIS_UB = iniReader.ReadInteger("Unfinished Business Profiles","MORRIS", 75);
	gGameUBOptions.ubRUDY_UB = iniReader.ReadInteger("Unfinished Business Profiles","RUDY", 77);
	

	#endif
}
