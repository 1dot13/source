#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "Action Items.h"
	#include "weapons.h"
	#include "Interface Cursors.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "Sound Control.h"
	#include "Sys globals.h"
	#include "Isometric Utils.h"
	#include "Animation Data.h"
	#include "worldman.h"
	#include "Random.h"
	#include "Campaign.h"
	#include "interface.h"
	#include "interface panels.h"
	#include "explosion control.h"
	#include "Keys.h"

	#include "wcheck.h"
	#include "soldier profile.h"
	#include "SkillCheck.h"
	#include "los.h"
	#include "message.h"
	#include "text.h"

	#include "fov.h"
	#include "MessageBoxScreen.h"

	#include "PathAIDebug.h"
	#include "Interface Control.h"
	#include "ShopKeeper Interface.h"
	#include "Cursors.h"

	#include "GameSettings.h"
	#include "environment.h"
	#include "Auto Resolve.h"
	#include "Interface Items.h"
	#include "Campaign Types.h"
	#include "history.h"
	#include "Game Clock.h"
	#include "strategicmap.h"
	#include "Inventory Choosing.h"
	#include "Soldier macros.h"
	#include "Smell.h"
	#include "lighting.h"
	#include "utilities.h"
	#include "english.h"
	#include "debug control.h"
#endif

#ifdef JA2UB

#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "Ja25_Tactical.h"
#include "Dialogue Control.h"
#include "interface Dialogue.h"
#include "Quests.h"
#include "SaveLoadMap.h"
#include "renderworld.h"
#include "qarray.h"
#include "Soldier Init List.h"
#include "Soldier Control.h"
#include "End Game.h"
#include "structure.h"
#include "Explosion Control.h"
#include "ub_config.h"

//*******************************************************************
//
// Local Defines
//
//*******************************************************************


#define		TCTL__DELAY_BETWEEN_ALARM_SOUNDS													4000

#define		NUM_NEW_GUN_QUOTES																				15


#define		PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_EASY								2
#define		PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_NORMAL							2
#define		PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_HARD								1



typedef struct
{
	INT16		sItem;
	BOOLEAN	fHasBeenSaid;

} NEW_GUN_QUOTES_STRUCT;


//Morris items
//UINT32 MORRIS_INSTRUCTION_NOTE = 1362;

UINT8 Get3RandomQualifiedMercs( UINT8 *pSoldierId1, UINT8 *pSoldierId2, UINT8 *pSoldierId3 );

//*******************************************************************
//
// Global Variables
//
//*******************************************************************

NEW_GUN_QUOTES_STRUCT gNewGunQuotes[ NUM_NEW_GUN_QUOTES ];


//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		StopPowerGenFan();
void		SetNewGunQuoteToBePlayedForThisGun( INT32 iItemIndex );
BOOLEAN HasNewGunQuoteBeenPlayedForThisGun( INT32 iItemIndex );
BOOLEAN IsThisGunANewJa25Gun( INT32 iItemIndex );
BOOLEAN SaveNewGunQuotesArrayToSaveGameFile( HWFILE hFile );
BOOLEAN LoadNewGunQuotesArrayToSaveGameFile( HWFILE hFile );
void		HandlePickingUpMorrisInstructionNote( SOLDIERTYPE *pSoldier, INT32 iIndex );
BOOLEAN IsSoldierAliveWithInitListGridNo( INT16 sInitListID );

void		Ja25ScaleAllEnemiesByValue( INT8 bExpScaleValue );
INT8		JA25SecondHighestExpLevelOnPlayersTeam( );
INT8		JA25HighestExpLevelOnTeam( INT8 bTeam );
INT8		JA25SecondHighestExpLevelOnEnemiesTeam( );
INT8		CountNumberOfMercsOnSameTeamOfSameExpLevel( INT8 bTeam, INT8 bExpLevel );

BOOLEAN	IsSoldierQualifiedGunCommenterMerc( SOLDIERTYPE *pSoldier );
UINT32	GetNumberOfTurnsPowerGenFanWillBeStoppedFor();
void HandleInitialEventsInHeliCrash();

BOOLEAN		gfFirstTimeInGameHeliCrash = FALSE;

void HandleHowPlayerGotThroughFan();

//ppp

//*******************************************************************
//
// Functions
//
//*******************************************************************

BOOLEAN useOldJA2Inventory = TRUE;

//inshy: chenge index of items to 1.13 version. Now we dont need Items.xml in UB-113 folder
/*
UINT32	BARRETT_UB = 335;
UINT32	CALICO_960_UB = 69;
UINT32	PSG1_UB =334;
UINT32	L85_UB =331;
UINT32	TAR21_UB =332;
UINT32	VAL_SILENT_UB =333;
UINT32	MICRO_UZI_UB = 330;
UINT32	CALICO_950_UB = 67;
UINT32	CALICO_900_UB = 68;
	
UINT32	CLIP_CANNON_BALL = 1354;
UINT32	MERC_UMBRELLA = 1361;
UINT32	HAND_CANNON = 1352;
UINT32	HARTFORD_6_SHOOTER = 66;
UINT32	TEX_MOVIE_ATTACK_CLYDESDALES = 1356;
UINT32	TEX_MOVIE_WILD_EAST = 1357;
UINT32	TEX_MOVIE_HAVE_HONDA = 1358;
UINT32	LAPTOP_TRANSMITTER = 1355;
UINT32	CHE_GUEVARA_CANTEEN = 1359;
UINT32	MERC_WRISTWATCH = 1360;
UINT32	SAM_GARVER_COMBAT_KNIFE = 1353;
UINT32	MERC_UMBRELLA_OLD = 1361;	//not used???
UINT32	MORRIS_INSTRUCTION_NOTE = 1362;

//inshy: chenge index of items to 1.13 version. Now we dont need Items.xml in UB-113 folder

UINT32	BARRETT_UB = 43;
UINT32	CALICO_960_UB = 44;
UINT32	PSG1_UB =45;
UINT32	L85_UB =46;
UINT32	TAR21_UB =47;
UINT32	VAL_SILENT_UB =48;
UINT32	MICRO_UZI_UB = 57;
UINT32	CALICO_950_UB = 66;
UINT32	CALICO_900_UB = 67;
	
UINT32	CLIP_CANNON_BALL = 115;
UINT32	MERC_UMBRELLA = 70;
UINT32	HAND_CANNON = 63;
UINT32	HARTFORD_6_SHOOTER = 68;
UINT32	TEX_MOVIE_ATTACK_CLYDESDALES = 328;
UINT32	TEX_MOVIE_WILD_EAST = 329;
UINT32	TEX_MOVIE_HAVE_HONDA = 330;
UINT32	LAPTOP_TRANSMITTER = 331;
UINT32	CHE_GUEVARA_CANTEEN = 332;
UINT32	MERC_WRISTWATCH = 333;
UINT32	SAM_GARVER_COMBAT_KNIFE = 69;
UINT32	MERC_UMBRELLA_OLD = 335;
UINT32	MORRIS_INSTRUCTION_NOTE = 336;	
*/	

UINT32	TEX_MOVIE_ATTACK_CLYDESDALES;
UINT32	TEX_MOVIE_WILD_EAST;
UINT32	TEX_MOVIE_HAVE_HONDA;
UINT32	LAPTOP_TRANSMITTER;
UINT32	CHE_GUEVARA_CANTEEN;
UINT32	MERC_WRISTWATCH;
UINT32	SAM_GARVER_COMBAT_KNIFE;
UINT32	MERC_UMBRELLA_OLD;
UINT32	MORRIS_INSTRUCTION_NOTE;
UINT32	HAND_CANNON;
UINT32	HARTFORD_6_SHOOTER;
UINT32	MERC_UMBRELLA;

UINT32	BARRETT_UB;
UINT32	CALICO_960_UB;
UINT32	PSG1_UB;
UINT32	L85_UB;
UINT32	TAR21_UB;
UINT32	VAL_SILENT_UB;
UINT32	MICRO_UZI_UB;
UINT32	CALICO_950_UB;
UINT32	CALICO_900_UB;

UINT32	CLIP_CANNON_BALL;

void Old_UB_Inventory ();
void New_UB_Inventory ();

void Old_UB_Inventory ()
{
	BARRETT_UB = 43;
	CALICO_960_UB = 44;
	PSG1_UB =45;
	L85_UB =46;
	TAR21_UB =47;
	VAL_SILENT_UB =48;
	MICRO_UZI_UB = 57;
	CALICO_950_UB = 66;
	CALICO_900_UB = 67;
	
	CLIP_CANNON_BALL = 115;
	MERC_UMBRELLA = 70;
	HAND_CANNON = 63;
	HARTFORD_6_SHOOTER = 68;
	TEX_MOVIE_ATTACK_CLYDESDALES = 328;
	TEX_MOVIE_WILD_EAST = 329;
	TEX_MOVIE_HAVE_HONDA = 330;
	LAPTOP_TRANSMITTER = 331;
	CHE_GUEVARA_CANTEEN = 332;
	MERC_WRISTWATCH = 333;
	SAM_GARVER_COMBAT_KNIFE = 69;
	MERC_UMBRELLA_OLD = 335;
	MORRIS_INSTRUCTION_NOTE = 336;
}


void New_UB_Inventory ()
{
	//inshy: chenge index of items to 1.13 version. Now we dont need Items.xml in UB-113 folder
	BARRETT_UB = 335;
	CALICO_960_UB = 69;
	PSG1_UB =334;
	L85_UB =331;
	TAR21_UB =332;
	VAL_SILENT_UB =333;
	MICRO_UZI_UB = 330;
	CALICO_950_UB = 67;
	CALICO_900_UB = 68;
	
	CLIP_CANNON_BALL = 1354;
	MERC_UMBRELLA = 1361;
	HAND_CANNON = 1352;
	HARTFORD_6_SHOOTER = 66;
	TEX_MOVIE_ATTACK_CLYDESDALES = 1356;
	TEX_MOVIE_WILD_EAST = 1357;
	TEX_MOVIE_HAVE_HONDA = 1358;
	LAPTOP_TRANSMITTER = 1355;
	CHE_GUEVARA_CANTEEN = 1359;
	MERC_WRISTWATCH = 1360;
	SAM_GARVER_COMBAT_KNIFE = 1353;
	MERC_UMBRELLA_OLD = 1361;	//not used???
	MORRIS_INSTRUCTION_NOTE = 1362;
}

//GridNo
UINT32  SWITCHINMORRISAREA_GRIDNO = 15231;
UINT32  SWITCHTOLAUNCHMISSLES_GRIDNO1 = 14268;
UINT32  SWITCHTOLAUNCHMISSLES_GRIDNO2 = 15708;
UINT32  RADIOLOCATORS_GRIDNO1 = 15070;
UINT32  RADIOLOCATORS_GRIDNO2 = 14744;
UINT32  POWERGENSECTOR_GRIDNO1 = 15100;
UINT32  POWERGENSECTOR_GRIDNO2 = 12220;
UINT32  POWERGENSECTOR_GRIDNO3 = 14155;
UINT32  POWERGENSECTOR_GRIDNO4 = 13980;
UINT32  POWERGENSECTOREXITGRID_GRIDNO1 = 19749;
UINT32  POWERGENFANSOUND_GRIDNO1 = 10979;
UINT32  POWERGENFANSOUND_GRIDNO2 = 19749;
UINT32  STARTFANBACKUPAGAIN_GRIDNO = 10980;
UINT32  STOPPOWERGENFAN_GRIDNO = 10980;

void InitGridNoUB()
{
	SWITCHINMORRISAREA_GRIDNO = gGameUBOptions.SwitchInMorrisAreaGridNo; //= 15231;
	SWITCHTOLAUNCHMISSLES_GRIDNO1 = gGameUBOptions.SwitchToLaunchMisslesGridNo1; //= 14268;
	SWITCHTOLAUNCHMISSLES_GRIDNO2 = gGameUBOptions.SwitchToLaunchMisslesGridNo2; //= 15708;
	RADIOLOCATORS_GRIDNO1 = gGameUBOptions.RadioLocatorsGridNo1; //= 15070;
	RADIOLOCATORS_GRIDNO2 = gGameUBOptions.RadioLocatorsGridNo2; //= 14744;
	POWERGENSECTOR_GRIDNO1 = gGameUBOptions.PowergenSectorGridNo1; //= 15100;
	POWERGENSECTOR_GRIDNO2 = gGameUBOptions.PowergenSectorGridNo2; //= 12220;
	POWERGENSECTOR_GRIDNO3 = gGameUBOptions.PowergenSectorGridNo3; //= 14155;
	POWERGENSECTOR_GRIDNO4 = gGameUBOptions.PowergenSectorGridNo4; //= 13980;
	POWERGENSECTOREXITGRID_GRIDNO1 = gGameUBOptions.PowergenSectorExitgridGridNo; // = 19749;
	POWERGENFANSOUND_GRIDNO1 = gGameUBOptions.PowergenFanSoundGridNo1; //= 10979;
	POWERGENFANSOUND_GRIDNO2 = gGameUBOptions.PowergenFanSoundGridNo2; //= 19749;
	STARTFANBACKUPAGAIN_GRIDNO = gGameUBOptions.StartFanbackupAgainGridNo; //= 10980;
	STOPPOWERGENFAN_GRIDNO = gGameUBOptions.StopPowergenFanGridNo; //= 10980;
}

	
BOOLEAN	IsSoldierQualifiedMerc( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->ubProfile == 	58	||  ///  GASTON
			pSoldier->ubProfile == 59 ||  // STOGIE
			pSoldier->ubProfile == 64 ||// TEX 			||
			pSoldier->ubProfile == 62 || //JOHN_K		||
			pSoldier->ubProfile == 61 || //BIGGENS	||
			pSoldier->ubProfile == 60 || //MANUEL		||
			pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER 
		)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN	IsSoldierQualifiedMercForSeeingPowerGenFan( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->ubProfile == 60 ||//MANUEL		||
			pSoldier->ubProfile ==  53 || //PGCMale3
			pSoldier->ubProfile ==  55 || //PGCFem2
			pSoldier->ubProfile ==  57		//PGCMale4
		)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN	IsSoldierQualifiedGunCommenterMerc( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->ubProfile == 	58	||  //  GASTON
			pSoldier->ubProfile == 	59	|| //   STOGIE
			pSoldier->ubProfile == 	64		||   // TEX
			pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER 
		)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


BOOLEAN	IsSoldierQualifiedInitialHireMerc( SOLDIERTYPE *pSoldier )
{
	if( pSoldier->ubProfile == 	58	|| //  GASTON
			pSoldier->ubProfile == 	59	||  // STOGIE
			pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER 
		)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}



INT8 GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( UINT8 *pSoldierIdArray, UINT8 *pProfileIdArray )
{
	SOLDIERTYPE *pSoldier=NULL;
	INT32       cnt;
	INT8				bNumMercsPresent=0;

	if( pSoldierIdArray )
		memset( pSoldierIdArray, NOBODY, NUM_MERCS_WITH_NEW_QUOTES );

	if( pProfileIdArray )
		memset( pProfileIdArray, NO_PROFILE, NUM_MERCS_WITH_NEW_QUOTES );

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	//Check to see if Gaston, Stogie or the PGC is on the team
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{    
		//if the merc is alive, in sector, etc...
		//Note: cant do the OK_CONTROLLABLE_MERC() cause it does bInSector which is not set when EnterSector is finshed ( we need it then )
		if( pSoldier->bActive	&&
				pSoldier->sSectorX == gWorldSectorX &&
				pSoldier->sSectorY == gWorldSectorY &&
				pSoldier->bSectorZ == gbWorldSectorZ &&
				pSoldier->stats.bLife >= OKLIFE &&
				!pSoldier->flags.fBetweenSectors )
		{
			//if the merc is one of the mercs we are looking for
			if( IsSoldierQualifiedMerc( pSoldier ) )
			{
				//if we are to return an array of the mercs
				if( pProfileIdArray != NULL )
				{
					//record the profile num of the merc
					pProfileIdArray[ bNumMercsPresent ] = pSoldier->ubProfile;
				}

				//if we are to return an array of the mercs
				if( pSoldierIdArray != NULL )
				{
					//record the soldier num of the merc
					pSoldierIdArray[ bNumMercsPresent ] = pSoldier->ubID;
				}

				bNumMercsPresent++;
			}
		}
	}

	return( bNumMercsPresent );
}



INT8	RandomProfileIdFromNewMercsOnPlayerTeam()
{
	INT8	bNumMercsPresent=-1;
	UINT8 ProfileIdArray[NUM_MERCS_WITH_NEW_QUOTES];

	//Get the number and array of the new soldiers
	bNumMercsPresent = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( NULL, ProfileIdArray );

	Assert( bNumMercsPresent < NUM_MERCS_WITH_NEW_QUOTES );

	if( bNumMercsPresent != 0 )
	{
		//return a random merc from the array
		return( ProfileIdArray[ Random( bNumMercsPresent ) ] );
	}
	else
	{
		return( -1 );
	}
}


INT8	RandomSoldierIdFromNewMercsOnPlayerTeam()
{
	INT8	bNumMercsPresent=-1;
	UINT8 SoldierIdArray[NUM_MERCS_WITH_NEW_QUOTES];

	//Get the number and array of the new soldiers
	bNumMercsPresent = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( SoldierIdArray, NULL );

	Assert( bNumMercsPresent < NUM_MERCS_WITH_NEW_QUOTES );

	if( bNumMercsPresent != 0 )
	{
		//return a random merc from the array
		return( SoldierIdArray[ Random( bNumMercsPresent ) ] );
	}
	else
	{
		return( -1 );
	}
}


INT8 RandomArrayOfQualifiedMercs( UINT8 *pRandomSoldierIdArray )
{
	INT8	bNumMercsPresent=-1;
	UINT8 SoldierIdArray[NUM_MERCS_WITH_NEW_QUOTES];
	BOOLEAN UsedArray[NUM_MERCS_WITH_NEW_QUOTES];
	UINT8	ubCnt;
	BOOLEAN	fFound=FALSE;
	UINT8		ubRand;

	memset( UsedArray, 0, NUM_MERCS_WITH_NEW_QUOTES );
	memset( pRandomSoldierIdArray, NOBODY, NUM_MERCS_WITH_NEW_QUOTES );

	//Get the number and array of the new soldiers
	bNumMercsPresent = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( SoldierIdArray, NULL );

	Assert( bNumMercsPresent < NUM_MERCS_WITH_NEW_QUOTES );

	if( bNumMercsPresent != 0 )
	{
		//loop through all the available mercs
		for( ubCnt=0; ubCnt<bNumMercsPresent; ubCnt++ )
		{
			fFound = FALSE;

			//loop
			while( !fFound )
			{
				ubRand = Random( bNumMercsPresent );
				if( !UsedArray[ ubRand ] )
				{
					UsedArray[ ubRand ] = TRUE;
					pRandomSoldierIdArray[ubCnt] = SoldierIdArray[ubRand];
					fFound = TRUE;
				}
			}
		}
	}
	return( bNumMercsPresent );
}


UINT8 Get3RandomQualifiedMercs( UINT8 *pSoldierId1, UINT8 *pSoldierId2, UINT8 *pSoldierId3 )
{
	UINT8	bNumMercs;
	UINT8 RandomSoldierIdArray[ NUM_MERCS_WITH_NEW_QUOTES ];
	UINT8 ubNumberDifMercsAssigned=0;

	bNumMercs = RandomArrayOfQualifiedMercs( RandomSoldierIdArray );

	if( bNumMercs == 0 )
	{
		return( 0 );
	}

	if( pSoldierId1 != NULL )
		*pSoldierId1 = NOBODY;

	if( pSoldierId2 != NULL )
		*pSoldierId2 = NOBODY;
	
	if( pSoldierId3 != NULL )
		*pSoldierId3 = NOBODY;


	if( bNumMercs >= 1 && pSoldierId1 != NULL )
	{
		*pSoldierId1 = RandomSoldierIdArray[ 0 ];
		ubNumberDifMercsAssigned++;
	}

	if( bNumMercs >= 2 && pSoldierId2 != NULL )
	{
		*pSoldierId2 = RandomSoldierIdArray[ 1 ];
		ubNumberDifMercsAssigned++;
	}
	else
	{
		*pSoldierId2 = RandomSoldierIdArray[ 0 ];
	}

	if( pSoldierId3 != NULL )
	{
		if( bNumMercs >= 3 )
		{
			*pSoldierId3 = RandomSoldierIdArray[ 2 ];
			ubNumberDifMercsAssigned++;
		}
		else if( bNumMercs >= 2 )
		{
			if( Chance( 50 ) )
				*pSoldierId3 = RandomSoldierIdArray[ 1 ];
			else
				*pSoldierId3 = RandomSoldierIdArray[ 0 ];
		}
	}

	return( ubNumberDifMercsAssigned );
}


void HandleWhenCertainPercentageOfEnemiesDie()
{
	UINT32				uiPercentEnemiesKilled;
	UINT8					ubSectorID;

	//if there isnt enemies in the sector
	if( ( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector + gTacticalStatus.ubArmyGuysKilled ) == 0 )
	{
		//get out
		return;
	}
	
	uiPercentEnemiesKilled = (UINT32)( 100 * (UINT32)(gTacticalStatus.ubArmyGuysKilled + 1) / (UINT32)( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector + gTacticalStatus.ubArmyGuysKilled ) );	

	ubSectorID = SECTOR( gWorldSectorX, gWorldSectorY );

	switch( gbWorldSectorZ )
	{
		case 0:
			switch( ubSectorID )
			{
				case SEC_K15:
					//all enemies are dead and if the quote hasnt been said yet
					if( uiPercentEnemiesKilled >= 100 && !( gJa25SaveStruct.uiJa25GeneralFlags & JA_GF__ALL_DEAD_TOP_LEVEL_OF_COMPLEX ) )
					{
						INT8 bProfile = RandomProfileIdFromNewMercsOnPlayerTeam();

						if( bProfile != -1 )
							DelayedMercQuote( bProfile, QUOTE_WONT_RENEW_CONTRACT_LAME_REFUSAL, GetWorldTotalSeconds() + 5 );

						gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__ALL_DEAD_TOP_LEVEL_OF_COMPLEX;
					}
					break;
			}
	}
}

void StopPowerGenFan()
{
	UINT16 usTileIndex;
	SOLDIERTYPE *pSoldier=NULL;

	// ATE: If destroyed, don't go into here
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__BLOWN_UP )
	{
		return;
	}

	//If the fan is already stopped, or has been stopped in the past
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__STOPPED ||
			IsJa25GeneralFlagSet( JA_GF__POWER_GEN_FAN_HAS_BEEN_STOPPED ) )
	{
		//leave
		return;
	}

	//if we are in turn based combat
	if( !((gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT)) )
	{
		return;
	}

	//Remeber how the player got through
	SetJa25GeneralFlag( JA_GF__POWER_GEN_FAN_HAS_BEEN_STOPPED );

	gJa25SaveStruct.ubStateOfFanInPowerGenSector = PGF__STOPPED;

	//Set the fact that player stopped the fan
	SetFactTrue( FACT_FAN_STOPPPED );

	//Is biggens on the team
	pSoldier = FindSoldierByProfileID( 61, TRUE );       //    BIGGENS
	if( pSoldier != NULL )
	{
		SetFactTrue( FACT_BIGGENS_ON_TEAM_AND_FAN_STOPPED );
	}

	//Turn off the power gen fan sound
	HandleRemovingPowerGenFanSound();

	//remeber which turn the fan stopped on
	gJa25SaveStruct.uiTurnPowerGenFanWentDown = gJa25SaveStruct.uiTacticalTurnCounter;


	//
	// Replace the Fan graphic
	//

	// Turn on permenant changes....
	ApplyMapChangesToMapTempFile( TRUE );

	//Add the exit grid to the power gen fan
	AddExitGridForFanToPowerGenSector();

	// Remove it!
	// Get index for it...
	GetTileIndexFromTypeSubIndex( FIFTHOSTRUCT, (INT8)( 1 ), &usTileIndex );
	RemoveStruct( STOPPOWERGENFAN_GRIDNO, usTileIndex );
	

	// Add the new one
	// Get index for it...
	GetTileIndexFromTypeSubIndex( FIFTHOSTRUCT, (INT8)( 7 ), &usTileIndex );
	AddStructToHead( STOPPOWERGENFAN_GRIDNO, usTileIndex );

	ApplyMapChangesToMapTempFile( FALSE );

	//Recompile the movement costs since we have added a exit grid
	RecompileLocalMovementCosts( STOPPOWERGENFAN_GRIDNO );

	gpWorldLevelData[ STOPPOWERGENFAN_GRIDNO ].uiFlags |= MAPELEMENT_REVEALED;

	// Re-render the world!
	gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;

	// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
	SetRenderFlags(RENDER_FLAG_FULL);
}

void HandleStartingFanBackUp()
{
	//if the fan has been stopped for the right amount of time
	if( gJa25SaveStruct.uiTacticalTurnCounter == ( gJa25SaveStruct.uiTurnPowerGenFanWentDown + GetNumberOfTurnsPowerGenFanWillBeStoppedFor() ) )
	{
		//if it is stopped
		if( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__STOPPED )
		{
			StartFanBackUpAgain();
		}
	}
}


void StartFanBackUpAgain()
{
	UINT16 usTileIndex;

	// ATE: If destroyed, don't go into here
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__BLOWN_UP )
	{
		return;
	}

	//Remeber how the player got through
	gJa25SaveStruct.ubStateOfFanInPowerGenSector = PGF__RUNNING_NORMALLY;


	//Start the fan sound
	HandleAddingPowerGenFanSound();


	//
	// Replace the Fan graphic
	//

	// Turn on permenant changes....
	ApplyMapChangesToMapTempFile( TRUE );

	// Remove it!
	// Get index for it...
	GetTileIndexFromTypeSubIndex( FIFTHOSTRUCT, (INT8)( 7 ), &usTileIndex );
	RemoveStruct( STARTFANBACKUPAGAIN_GRIDNO, usTileIndex );
	

	// Add the new one
	// Get index for it...
	GetTileIndexFromTypeSubIndex( FIFTHOSTRUCT, (INT8)( 1 ), &usTileIndex );
	AddStructToHead( STARTFANBACKUPAGAIN_GRIDNO, usTileIndex );

	ApplyMapChangesToMapTempFile( FALSE );

	//Recompile the movement costs since we have added a exit grid
	RecompileLocalMovementCosts( STARTFANBACKUPAGAIN_GRIDNO );

	gpWorldLevelData[ STARTFANBACKUPAGAIN_GRIDNO ].uiFlags |= MAPELEMENT_REVEALED;

	// Re-render the world!
	gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;

	//Remove the exit grid
	RemoveExitGridFromWorld( PGF__FAN_EXIT_GRID_GRIDNO );

	// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
	SetRenderFlags(RENDER_FLAG_FULL);
}

void HandlePowerGenAlarm()
{
	static UINT32 uiLastTime=0;
	static UINT32 uiAlarmCounter=0;
	UINT32 uiCurTime=0;

	//if its not the right sector
	if( !( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_J && gbWorldSectorZ == 0 ) )
	{
		//leave
		return;
	}

	//if the fan is not stopped
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector != PGF__STOPPED )
	{
		uiAlarmCounter = 0;
		//leave
		return;
	}

	uiCurTime = GetJA2Clock();

	if( ( uiCurTime - uiLastTime ) >= TCTL__DELAY_BETWEEN_ALARM_SOUNDS )
	{
		uiAlarmCounter++;

		PlayJA2SampleFromFile( "SOUNDS\\AlarmFan.wav", RATE_11025, MIDVOLUME, 1, MIDDLE );

		uiLastTime = uiCurTime;

	
		if( uiAlarmCounter == 2 && !IsJa25GeneralFlagSet( JA_GF__BIGGENS_SAID_QUOTE_117 ) )
		{
			//
			// Have merc say the quote, if the conditions are right
			//
			if( gubFact[ FACT_PLAYER_KNOWS_ABOUT_FAN_STOPPING ] )
			{
				INT8 bID = RandomSoldierIdFromNewMercsOnPlayerTeam();

				if( bID != -1 )
				{
					TacticalCharacterDialogue( &Menptr[ bID ], QUOTE_PERSONALITY_BIAS_WITH_MERC_2 );
				}
			}
			else
			{
				UINT8 bSoldierId1, bSoldierId2, bSoldierId3;
				Get3RandomQualifiedMercs( &bSoldierId1, &bSoldierId2, &bSoldierId3 );

				if( bSoldierId1 != -1 && Menptr[ bSoldierId1 ].ubProfile != 61 ) //BIGGENS
				{
					TacticalCharacterDialogue( &Menptr[ bSoldierId1 ], QUOTE_PERSONALITY_BIAS_WITH_MERC_1 );
				}
				else if( bSoldierId2 != -1 && Menptr[ bSoldierId2 ].ubProfile != 61 ) //BIGGENS
				{
					TacticalCharacterDialogue( &Menptr[ bSoldierId2 ], QUOTE_PERSONALITY_BIAS_WITH_MERC_1 );
				}
				else if( bSoldierId3 != -1 && Menptr[ bSoldierId3 ].ubProfile != 61 ) // BIGGENS
				{
					TacticalCharacterDialogue( &Menptr[ bSoldierId3 ], QUOTE_PERSONALITY_BIAS_WITH_MERC_1 );
				}
			}
		}
	}
}




void HandleAddingPowerGenFanSound()
{
	UINT32 sGridNo;

	//if its not already playing
	if( gJa25SaveStruct.iPowerGenFanPositionSndID != -1 )
	{
		return;
	}

	if( gbWorldSectorZ == 0 )
		sGridNo = POWERGENFANSOUND_GRIDNO1;
	else
		sGridNo = POWERGENFANSOUND_GRIDNO2;

	//Create the new ambient fan sound
	//gJa25SaveStruct.iPowerGenFanPositionSndID = NewPositionSnd( sGridNo, POSITION_SOUND_STATIONATY_OBJECT, 0, POWER_GEN_FAN_SOUND );

	SetPositionSndsInActive( );
	SetPositionSndsActive( );
}

void HandleRemovingPowerGenFanSound()
{
	//if there is an ambience sound playing
	if( gJa25SaveStruct.iPowerGenFanPositionSndID != -1 )
	{
		//delete it
		DeletePositionSnd( gJa25SaveStruct.iPowerGenFanPositionSndID );

		gJa25SaveStruct.iPowerGenFanPositionSndID = -1;
	}
}


void AddExitGridForFanToPowerGenSector()
{
	EXITGRID ExitGrid;

	memset( &ExitGrid, 0, sizeof( EXITGRID ) );

	ExitGrid.ubGotoSectorX = 14;
	ExitGrid.ubGotoSectorY = MAP_ROW_J;
	ExitGrid.ubGotoSectorZ = 1;
	ExitGrid.usGridNo = POWERGENSECTOREXITGRID_GRIDNO1;

	//Add the exit grid when the fan is either stopped or blown up
	AddExitGridToWorld( PGF__FAN_EXIT_GRID_GRIDNO, &ExitGrid );
}

BOOLEAN HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( SOLDIERTYPE *pSoldierAtDoor, BOOLEAN fSayQuoteOnlyOnce )
{
	INT8					bSlot;
	UINT32				cnt;
	UINT8					ubID;
	SOLDIERTYPE		*pSoldier;


	//is this the right sector
	if( !( gWorldSectorX == 14 && gWorldSectorY == MAP_ROW_K && gbWorldSectorZ == 1 ) )
	{
		//wrong door
		return( FALSE );
	}

	//if the player has already said the quote
	if( IsJa25GeneralFlagSet( JA_GF__PLAYER_SAID_GATE_LOCKED_QUOTE ) )
	{
		return( FALSE );
	}

	//
	// look to see if anyone on the team has wire cutters
	//
	// loop throught the team and see if anyone in this sector has the wire cutter
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{
    //if the soldier is in the sector
		if( pSoldier->bActive && pSoldier->bInSector && ( pSoldier->stats.bLife >= CONSCIOUSNESS ) &&
				 !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
		{
			bSlot = FindObj( pSoldier, WIRECUTTERS );
			if( bSlot != NO_SLOT )
			{
				//the merc has wire cutters
				return( FALSE );
			}
		}
	}

	ubID = RandomSoldierIdFromNewMercsOnPlayerTeam();

	if( ubID != -1 )
	{
		//have the merc say the quote about the tough gate
		TacticalCharacterDialogue( &Menptr[ ubID ], QUOTE_IMPATIENT_QUOTE );
	}

	//remeber we said the quote
	if( fSayQuoteOnlyOnce )
	{
		SetJa25GeneralFlag( JA_GF__PLAYER_SAID_GATE_LOCKED_QUOTE );
	}

	return( TRUE );
}

void HandleHowPlayerGotThroughFan()
{
		switch( gJa25SaveStruct.ubStateOfFanInPowerGenSector )
		{
			case PGF__STOPPED:
				gJa25SaveStruct.ubHowPlayerGotThroughFan = PG__PLAYER_STOPPED_FAN_TO_GET_THROUGH;
				break;
			
			case PGF__BLOWN_UP:
				gJa25SaveStruct.ubHowPlayerGotThroughFan = PG__PLAYER_BLEW_UP_FAN_TO_GET_THROUGH;
			
				//If the player blew up the fan, then the enemies can hear it in the tunnel and prepare for it.
				gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__DID_PLAYER_MAKE_SOUND_GOING_THROUGH_TUNNEL_GATE;
				break;
		}

}
/*
void RevealAllDroppedEnemyItems()
{
	UINT32 uiCnt=0;

	//loop through all the items
	for(uiCnt=0; uiCnt<guiNumWorldItems; uiCnt++ )
	{
		//if the item exists AND the item was droppped from an enemy
		if( gWorldItems[ uiCnt ].fExists && gWorldItems[ uiCnt ].usFlags & WORLD_ITEM_DROPPED_FROM_ENEMY )
		{
			gWorldItems[ uiCnt ].bVisible = VISIBLE;
		}
	}

	// Make team look for items
	AllSoldiersLookforItems( TRUE );

}
*/
void HandlePlayingQuoteWhenHiringNpc( UINT8 ubProfile )
{
	switch( ubProfile )
	{
	case 60://MANUEL:
			SayQuoteFromAllNewHiredMercButDoGastonLast( ubProfile, QUOTE_PRECEDENT_TO_REPEATING_ONESELF );
			break;
	case 61 : //BIGGENS:
			SayQuoteFromAllNewHiredMercButDoGastonLast( ubProfile, QUOTE_REFUSAL_TO_JOIN_LACK_OF_FUNDS );
			break;
	case 62 : //JOHN_K:
			SayQuoteFromAllNewHiredMercButDoGastonLast( ubProfile, QUOTE_DEPART_COMMET_CONTRACT_NOT_RENEWED_OR_TERMINATED_UNDER_48 );

			//Delay John saying quote about town
			DelayedMercQuote( 62, QUOTE_HATE_MERC_2_ON_TEAM, GetWorldTotalSeconds() + 5 + Random( 10 ) );
			break;
	case 64 : //TEX:
			SayQuoteFromAllNewHiredMercButDoGastonLast( ubProfile, QUOTE_DEATH_RATE_RENEWAL );
			break;
	}
}

BOOLEAN SayQuoteFromAllNewHiredMercButDoGastonLast( UINT8 ubProfile, UINT32 uiQuoteNum )
{
	INT8				bNumMercsPresent=-1;
	UINT8				SoldierIdArray[NUM_MERCS_WITH_NEW_QUOTES];
	INT32				iCnt;
	SOLDIERTYPE *pSoldier=NULL;

	//Get an array of the mercs on the team
	bNumMercsPresent = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( SoldierIdArray, NULL );

	if( bNumMercsPresent == -1 )
	{
		return( FALSE );
	}

	for( iCnt=0; iCnt<bNumMercsPresent; iCnt++ )
	{
		//Do Gaston and the newly hired RPC last
		if( Menptr[ SoldierIdArray[ iCnt ] ].ubProfile == 58 || Menptr[ SoldierIdArray[ iCnt ] ].ubProfile == ubProfile) //  GASTON
		{
			continue;
		}

		TacticalCharacterDialogue( &Menptr[ SoldierIdArray[ iCnt ] ], (UINT16)uiQuoteNum );
	}

	//if Gaston is on the team, say his quote
	pSoldier = FindSoldierByProfileID( 58, TRUE ); // GASTON
	if( pSoldier &&
			pSoldier->sSectorX == gWorldSectorX &&
			pSoldier->sSectorY == gWorldSectorY &&
			pSoldier->bSectorZ == gbWorldSectorZ )
	{
		TacticalCharacterDialogue( pSoldier, (UINT16)uiQuoteNum );
	}

	//Say the quote of the newly hired person
	pSoldier = FindSoldierByProfileID( ubProfile, TRUE );
	if( pSoldier )
	{
		TacticalCharacterDialogue( pSoldier, (UINT16)uiQuoteNum );
	}

	return( TRUE );
}

//returns false if a new merc is not going to handle saying the new quote
BOOLEAN HandleNewGunComment( SOLDIERTYPE *pSoldier, INT32 iItemIndex, BOOLEAN fFromGround )
{
	BOOLEAN fNewMerc = IsSoldierQualifiedGunCommenterMerc( pSoldier );

	if( !fFromGround )
	{
		//if this is morris's note, handle it
		HandlePickingUpMorrisInstructionNote( pSoldier, iItemIndex );
	}
	
	//if the quote has already been said
	if( HasNewGunQuoteBeenPlayedForThisGun( iItemIndex ) )
	{
		//get out
		return( TRUE );
	}
	
	//if we are not in tactical or map screen, 
	if( guiCurrentScreen != GAME_SCREEN && guiCurrentScreen != MAP_SCREEN )
	{
		//only remeber that we have picked up the gun at some point
		SetNewGunQuoteToBePlayedForThisGun( iItemIndex );

		return( FALSE );
	}

	//if the soldier is a NOT new merc AND this item is from the ground AND the merc didnt say this quote recently
	if( !fNewMerc && fFromGround && ( pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE ) == 0 )
	{
		//if the merc can say it
		if( QuoteExp_GotGunOrUsedGun[ pSoldier->ubProfile ] == QUOTE_FOUND_SOMETHING_SPECIAL )
		{
			//Have the merc say his cool item quote
			TacticalCharacterDialogue( pSoldier, QUOTE_FOUND_SOMETHING_SPECIAL );

			pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE;
		}
	}

	//if it is a new merc
	if( fNewMerc )
	{
		//if the item is the hand cannon
		if( iItemIndex == HAND_CANNON )   //4498  
		{
			//say the new gun quote
			TacticalCharacterDialogue( pSoldier, QUOTE_PRECEDENT_TO_REPEATING_ONESELF_RENEW );
		}
		else
		{
			//say the new gun quote
			TacticalCharacterDialogue( pSoldier, QUOTE_HATE_MERC_1_ON_TEAM_WONT_RENEW );
		}

		//rememeber we have played the quote
		SetNewGunQuoteToBePlayedForThisGun( iItemIndex );
	}
	
	return( TRUE );
}




BOOLEAN SaveJa25TacticalInfoToSaveGame( HWFILE hFile )
{
	if( !SaveNewGunQuotesArrayToSaveGameFile( hFile ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadJa25TacticalInfoFromSavedGame( HWFILE hFile )
{
	if( !LoadNewGunQuotesArrayToSaveGameFile( hFile ) )
	{
		return( FALSE );
	}

	return( TRUE );
}



BOOLEAN SaveNewGunQuotesArrayToSaveGameFile( HWFILE hFile )
{
	UINT32 uiNumBytesWritten;
	UINT32	uiSize = sizeof( NEW_GUN_QUOTES_STRUCT ) * NUM_NEW_GUN_QUOTES;

	FileWrite( hFile, &gNewGunQuotes, uiSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSize )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadNewGunQuotesArrayToSaveGameFile( HWFILE hFile )
{
	UINT32 uiNumBytesRead;
	UINT32	uiSize = sizeof( NEW_GUN_QUOTES_STRUCT ) * NUM_NEW_GUN_QUOTES;

	FileRead( hFile, &gNewGunQuotes, uiSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiSize )
	{
		return( FALSE );
	}
	
	InitNewGunArray();
	
	return( TRUE );
}

void InitNewGunArray()
{
	INT32 iCnt=0;

	gNewGunQuotes[iCnt++].sItem = BARRETT_UB;
	gNewGunQuotes[iCnt++].sItem = CALICO_960_UB;
	gNewGunQuotes[iCnt++].sItem = PSG1_UB;
	gNewGunQuotes[iCnt++].sItem = L85_UB;
	gNewGunQuotes[iCnt++].sItem = TAR21_UB;
	gNewGunQuotes[iCnt++].sItem = VAL_SILENT_UB;
	gNewGunQuotes[iCnt++].sItem = MICRO_UZI_UB;
	gNewGunQuotes[iCnt++].sItem = HAND_CANNON;
	gNewGunQuotes[iCnt++].sItem = CALICO_950_UB;
	gNewGunQuotes[iCnt++].sItem = CALICO_900_UB;
}


BOOLEAN IsThisGunANewJa25Gun( INT32 iItemIndex )
{
	//if this gun ISNT a new gun
	if( !( iItemIndex == BARRETT_UB	||
			iItemIndex == CALICO_960_UB	|| 
			iItemIndex == PSG1_UB				|| 
			iItemIndex == L85_UB					|| 
			iItemIndex == TAR21_UB				|| 
			iItemIndex == VAL_SILENT_UB	|| 
			iItemIndex == MICRO_UZI_UB		|| 
			iItemIndex == HAND_CANNON ||
			iItemIndex == CALICO_950_UB	|| 
			iItemIndex == CALICO_900_UB ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN HasNewGunQuoteBeenPlayedForThisGun( INT32 iItemIndex )
{
	INT32 iCnt;

	for( iCnt=0; iCnt<NUM_NEW_GUN_QUOTES; iCnt++)
	{
		//if this is the item
		if( gNewGunQuotes[ iCnt ].sItem == iItemIndex )
		{
			//return wether it has been said
			return( gNewGunQuotes[ iCnt ].fHasBeenSaid );
		}
	}

	return( TRUE );
}


void SetNewGunQuoteToBePlayedForThisGun( INT32 iItemIndex )
{
	INT32 iCnt;

	for( iCnt=0; iCnt<NUM_NEW_GUN_QUOTES; iCnt++)
	{
		//if this is the item
		if( gNewGunQuotes[ iCnt ].sItem == iItemIndex )
		{
			gNewGunQuotes[ iCnt ].fHasBeenSaid = TRUE;
			return;
		}
	}
}

void HandlePickingUpMorrisInstructionNote( SOLDIERTYPE *pSoldier, INT32 iIndex )
{
	INT8	bID=-1;

	if( iIndex != MORRIS_INSTRUCTION_NOTE )
	{
		return;
	}

	//if the note has already been picked up by a new merc, or the not ehas been displayed
	if( gJa25SaveStruct.ubMorrisNoteState == MN__PICKED_UP_BY_NEW_MERC ||
			gJa25SaveStruct.ubMorrisNoteState == MN__FINISHED )
	{
		return;
	}

	bID = RandomSoldierIdFromNewMercsOnPlayerTeam();

	//if this is a qualified merc on the team
	if( IsSoldierQualifiedMerc( pSoldier ) )
	{
		//Remember we said the quote
		gJa25SaveStruct.ubMorrisNoteState = MN__PICKED_UP_BY_NEW_MERC;

		//Delaying the merc to say the note
		DelayedMercQuote( pSoldier->ubProfile, DQ__NEW_MERC_SAY_NOTE_QUOTES, GetWorldTotalSeconds()+1 );
	}

	//else if there is a new merc on the team
	else if( bID != -1 )
	{
		UINT32 uiTime=0;

		if( gJa25SaveStruct.ubMorrisNoteState == MN__PICKED_UP_BY_OLD_MERC_SAID_QUOTE_ALREADY )
		{
			return;
		}

		//the merc is not a new merc
		gJa25SaveStruct.ubMorrisNoteState = MN__PICKED_UP_BY_OLD_MERC_SAID_QUOTE_ALREADY;

		TacticalCharacterDialogue( &Menptr[ bID ], QUOTE_RENEWING_CAUSE_BUDDY_1_ON_TEAM );

		gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote = bID;

		//if we are in turnbased combat
		if( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			uiTime = GetWorldTotalSeconds() + 3;
		}
		else
		{
			uiTime = GetWorldTotalSeconds() + 10;
		}

		DelayedMercQuote( NOBODY, DQ__MORRIS_NOTE_NEW_MERC_DELAY, uiTime );
	}

	else
	{
		//
		// Create a merc popup box that will display a message telling player what to do
		//

		gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote = pSoldier->ubProfile;

		DelayedMercQuote( pSoldier->ubProfile, DQ__MORRIS_NOTE_DISPLAY_NOTE_1, GetWorldTotalSeconds() + 1 );

		gJa25SaveStruct.ubMorrisNoteState = MN__FINISHED;
	}
}

void HandleDeathInPowerGenSector( SOLDIERTYPE *pSoldier )
{
	//if this is NOT the power gen sector
	if( gWorldSectorX != 13 || gWorldSectorY != 10 || gbWorldSectorZ != 0 )
	{
		return;
	}
	else
	{
		BOOLEAN fFoundValidEnemy=FALSE;
		UINT8		ubNumFlagedEnemiesInSector=0;
		UINT32		sRandomSlotGridNo;
		UINT8		uiCnt;
		SOLDIERINITNODE	*pInitListSoldier;

		#define NUM_ENEMIES_SLOTS			4

		UINT32	sEnemyPlacementGridNo[ NUM_ENEMIES_SLOTS ]=
						{
							POWERGENSECTOR_GRIDNO1,
							POWERGENSECTOR_GRIDNO2,
							POWERGENSECTOR_GRIDNO3,
							POWERGENSECTOR_GRIDNO4,
						};

		//
		// Count number of Flaged guys still left
		//
		for( uiCnt=0; uiCnt<NUM_ENEMIES_SLOTS; uiCnt++ )
		{
			//Choose a random ID
			sRandomSlotGridNo = sEnemyPlacementGridNo[ uiCnt ];

			//is this soldier still alive
			if( IsSoldierAliveWithInitListGridNo( sRandomSlotGridNo ) )
			{
				ubNumFlagedEnemiesInSector++;
			}
		}

/*
		//
		// Chooose a random slot ID for the enemy
		//


		for( uiCnt=0; uiCnt<50; uiCnt++ )
		{
			//Choose a random ID
			sRandomSlotGridNo = sEnemyPlacementGridNo[ Random( NUM_ENEMIES_SLOTS ) ];

			//is this soldier still alive
			if( IsSoldierAliveWithInitListGridNo( sRandomSlotGridNo ) )
			{
				pInitListSoldier = FindSoldierInitNodeWithID( pSoldier->ubID );

				//is this the same soldier
				if( pInitListSoldier && pInitListSoldier->pBasicPlacement->usStartingGridNo == sRandomSlotGridNo )
				{
					fFoundValidEnemy = TRUE;
				}

				//we are done, 
				break;
			}
		}
*/	
		// if we found an enemy, or this is the 2nd last enemy in the sector
		if( fFoundValidEnemy || ubNumFlagedEnemiesInSector <= 2 )
		{
			StopPowerGenFan();
		}

	}	

}


BOOLEAN IsSoldierAliveWithInitListGridNo( INT16 sInitListGridNo )
{
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->usStartingGridNo == sInitListGridNo && curr->pSoldier != NULL )
		{
			if ( curr->pSoldier->stats.bLife >= OKLIFE )
			{
				return( TRUE );
			}
		}
		curr = curr->next;
	}
	return( FALSE );
}

#define	JA25__MAX_EXP_DECREASE							(-2)
#define	JA25__MIN_EXP_LEVEL_AFTER_CHANGE		(2)

void HandleJa25EnemyExpLevelModifier( )
{
	INT8 bPlayerExpLevel=0;
	INT8 bEnemyExpLevel=0;
	INT8 bDifference=0;

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		return;
	}

	//if there are no enemies
	if( NumEnemyInSector() == 0 )
	{
		//get out
		return;
	}

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:

			//Get the 2nd highest player exp level
			bPlayerExpLevel = JA25SecondHighestExpLevelOnPlayersTeam( );

			//get the enemies exp level
			bEnemyExpLevel = JA25SecondHighestExpLevelOnEnemiesTeam( );

			//get the difference b/n the 2
			bDifference = bPlayerExpLevel - bEnemyExpLevel;
			
			//if the players exp level is less then enemies
			if( bDifference < 0 )
			{
				//if the difference 
				if( bDifference < JA25__MAX_EXP_DECREASE )
				{
					bDifference = JA25__MAX_EXP_DECREASE;
				}

				//Degrade all the enemies exp levels by difference
				Ja25ScaleAllEnemiesByValue( bDifference );
			}
			break;
		case DIF_LEVEL_MEDIUM:

			//if the player imported the save
			if( gJa25SaveStruct.fImportCharactersFromOldJa2Save )
			{
				//Get the 2nd highest player exp level
				bPlayerExpLevel = JA25SecondHighestExpLevelOnPlayersTeam( );

				//get the best enemies exp level
				bEnemyExpLevel = JA25HighestExpLevelOnTeam( ENEMY_TEAM );

				//get the difference b/n the 2
				bDifference = bPlayerExpLevel - bEnemyExpLevel;
				
				//if the players exp level is greater then enemies
				if( bDifference > 0 )
				{
					//Upgrade all the enemies exp levels by difference
					Ja25ScaleAllEnemiesByValue( bDifference );
				}
			}

			break;
		case DIF_LEVEL_HARD:
			//Get the highest player exp level
			bPlayerExpLevel = JA25HighestExpLevelOnTeam( OUR_TEAM );

			//get the best enemies exp level
			bEnemyExpLevel = JA25HighestExpLevelOnTeam( ENEMY_TEAM );

			//get the difference b/n the 2
			bDifference = bPlayerExpLevel - bEnemyExpLevel;
			
			//if the players exp level is greater then enemies
			if( bDifference > 0 )
			{
				//if the player imported the save
				if( gJa25SaveStruct.fImportCharactersFromOldJa2Save )
				{
					//Upgrade all the enemies exp levels by difference
					Ja25ScaleAllEnemiesByValue( bDifference );
				}
				else
				{
					//Upgrade all the enemies exp levels by 1
					Ja25ScaleAllEnemiesByValue( 1 );
				}
			}
			break;
	}
}

INT8 JA25HighestExpLevelOnTeam( INT8 bTeam )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;
	INT8	bHighestExpLevel=0;

	cnt = gTacticalStatus.Team[ bTeam ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ bTeam ].bLastID; cnt++, pSoldier++)
	{
		if( pSoldier->bActive )
		{
			if( bHighestExpLevel < pSoldier->stats.bExpLevel )
			{
				bHighestExpLevel = pSoldier->stats.bExpLevel;
			}
		}
	}

	return( bHighestExpLevel );
}


INT8 JA25SecondHighestExpLevelOnPlayersTeam( )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;
	INT8	bHighestExpLevel=0;
	INT8	b2ndHighestExpLevel=0;
	INT8  bNumber=0;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++, pSoldier++)
	{
		if( pSoldier->bActive )
		{
			if( bHighestExpLevel < pSoldier->stats.bExpLevel )
			{
				bHighestExpLevel = pSoldier->stats.bExpLevel;
			}

			else if( b2ndHighestExpLevel < pSoldier->stats.bExpLevel )
			{
				b2ndHighestExpLevel = pSoldier->stats.bExpLevel;
			}
		}
	}

	//Count number of people on team with same EXP level
	bNumber = CountNumberOfMercsOnSameTeamOfSameExpLevel( OUR_TEAM, bHighestExpLevel );

	//if there are more then 2 mercs on the team with the same high exp level
	if( bNumber >= 2 )
	{
		//use the highest exp level
		b2ndHighestExpLevel = bHighestExpLevel;
	}
	else
	{
		//else we are to only decuct 1 exp level from the main guy so there isnt a HUGE descrepincy b/n 1 super merc
		//and a bunch of low level losers on a team
		b2ndHighestExpLevel = bHighestExpLevel - 1;
	}

	return( b2ndHighestExpLevel );
}

INT8 JA25SecondHighestExpLevelOnEnemiesTeam( )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;
	INT8	bHighestExpLevel=0;
	INT8	b2ndHighestExpLevel=0;
	INT8	bNumber=0;

	//Get the highest exp level on the enemy team
	bHighestExpLevel = JA25HighestExpLevelOnTeam( ENEMY_TEAM );

	//Count number of people on team with same EXP level
	bNumber = CountNumberOfMercsOnSameTeamOfSameExpLevel( ENEMY_TEAM, bHighestExpLevel );

	//if there are more then 2 enemies at the high exp level
	if( bNumber > 2 )
	{
		//use the highest value
		b2ndHighestExpLevel = bHighestExpLevel;
	}
	else
	{
		//otherwise loop through and determine the second highest exp level
		cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++, pSoldier++)
		{
			if( pSoldier->bActive )
			{
				//if the exp level is less then the highest
				if( pSoldier->stats.bExpLevel < bHighestExpLevel )
				{
					//is this a new second highest level
					if( b2ndHighestExpLevel < pSoldier->stats.bExpLevel )
					{
						b2ndHighestExpLevel = pSoldier->stats.bExpLevel;
					}
				}
			}
		}
	}

	return( b2ndHighestExpLevel );
}


void Ja25ScaleAllEnemiesByValue( INT8 bExpScaleValue )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;
	INT8		bNewExpLevel=0;

	cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++, pSoldier++)
	{
		if( pSoldier->bActive )
		{
			bNewExpLevel = pSoldier->stats.bExpLevel + bExpScaleValue;

			if( bNewExpLevel > 10 )
			{
				bNewExpLevel = 10;
			}
			//else if the new value will bring it below 1
			else if( bNewExpLevel < 1 )
			{
				bNewExpLevel = 1;
			}

			//if the enemy originally had a higher exp level AND will now go below set level
			else if( pSoldier->stats.bExpLevel > JA25__MIN_EXP_LEVEL_AFTER_CHANGE && bNewExpLevel < JA25__MIN_EXP_LEVEL_AFTER_CHANGE )
			{
				bNewExpLevel = JA25__MIN_EXP_LEVEL_AFTER_CHANGE;
			}


			pSoldier->stats.bExpLevel = bNewExpLevel;
		}
	}
}

INT8 CountNumberOfMercsOnSameTeamOfSameExpLevel( INT8 bTeam, INT8 bExpLevel )
{
	INT8	bNumber=0;
	SOLDIERTYPE *pSoldier=NULL;
	INT32 cnt;

	cnt = gTacticalStatus.Team[ bTeam ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ bTeam ].bLastID; cnt++, pSoldier++)
	{
		if( pSoldier->bActive )
		{
			if( bExpLevel == pSoldier->stats.bExpLevel )
			{
				bNumber++;
			}
		}
	}

	return( bNumber );
}

INT8 RandomSoldierIdForAnyMercInSector()
{
	UINT8 SoldierIdArray[18];
	UINT8	ubNumMercsInSector=0;
	INT32	cnt;
	SOLDIERTYPE *pSoldier=NULL;
	UINT8 ubCount=0;

	ubNumMercsInSector = PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, gbWorldSectorZ );

	if( ubNumMercsInSector == 0 )
	{
		return( -1 );
	}

	memset( SoldierIdArray, 0, 18 );

	//loop through the merc array and fill in the soldier id
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{
    //if the soldier is in the sector
		if( pSoldier->bActive && pSoldier->bInSector && ( pSoldier->stats.bLife >= CONSCIOUSNESS ) &&
				 !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
		{
			SoldierIdArray[ ubCount++ ] = cnt;			
		}
	}

	return( SoldierIdArray[ Random( ubCount ) ] );
}

void HandleFanStartingAtEndOfCombat()
{
	//if its not the right sector
	if( !( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_J && gbWorldSectorZ == 0 ) )
	{
		//leave
		return;
	}

	//if the fan is not stopped
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector != PGF__STOPPED )
	{
		//leave
		return;
	}

	StartFanBackUpAgain();
}


void HandleInitialEventsInHeliCrash()
{
	SOLDIERTYPE *pSoldier=NULL;
	INT32 cnt;

	//first, loop through all the mercs and injure them
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{    
		//if the merc is alive
		if( pSoldier->bActive )
		{
			pSoldier->stats.bLife -= ( 3 + Random( 5 ) );
			pSoldier->sBreathRed = ( 15 + Random( 15 ) ) * 100;
			pSoldier->bActionPoints = 0;
		}
	}
/*
#ifdef JA2TESTVERSION
	//Add a smoke effect behind the heli
	NewSmokeEffect( 15102, SMOKE_GRENADE, 0, NOBODY );
#endif
*/
}


UINT32 GetNumberOfTurnsPowerGenFanWillBeStoppedFor()
{
	UINT32 uiNumTurns = PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_EASY;

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			uiNumTurns = PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_EASY;
			break;
		case DIF_LEVEL_MEDIUM:
			uiNumTurns = PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_NORMAL;
			break;
		case DIF_LEVEL_HARD:
			uiNumTurns = PGF__NUM_TURNS_TILL_START_FAN_BACK_UP_HARD;
			break;
	}

	return( uiNumTurns );
}


void DisplayCommanderMorrisNote( SOLDIERTYPE *pSoldier )
{
	CHAR16	zString[1024];

	if( gJa25SaveStruct.ubDisplayCommanderMorrisNote == DMN__FINISHED )
	{
		return;
	}

	if( pSoldier == NULL )
	{
		return;
	}

	//if it si the first part of the note to be displayed
	if( gJa25SaveStruct.ubDisplayCommanderMorrisNote == DMN__NOT_TO_DISPLAY_IT )
	{
		gJa25SaveStruct.ubDisplayCommanderMorrisNote = DMN__DISPLAY_PART_1; 

		swprintf( zString, zNewTacticalMessages[ TCTL_MSG__END_GAME_POPUP_TXT_1 ], pSoldier->name );
	}

	//if it si the second part of the note to be displayed
	else if( gJa25SaveStruct.ubDisplayCommanderMorrisNote == DMN__DISPLAY_PART_1 )
	{
		gJa25SaveStruct.ubDisplayCommanderMorrisNote = DMN__DISPLAY_PART_2; 

		swprintf( zString, zNewTacticalMessages[ TCTL_MSG__END_GAME_POPUP_TXT_2 ], pSoldier->name, pSoldier->name );
	}

	//Display it
	ExecuteTacticalTextBox( 110, zString );
}



void HandleCommanderMorrisNewMercWantsNoteDelayedSpeech()
{
	INT8 bID=-1;

	//if the note has already been picked up by a new merc, or the note has been displayed
	if( gJa25SaveStruct.ubMorrisNoteState == MN__PICKED_UP_BY_NEW_MERC ||
			gJa25SaveStruct.ubMorrisNoteState == MN__FINISHED )
	{
		return;
	}

	//if the original merc who said the quote is valid
	bID = gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote;
	if( bID != -1 && IsSoldierQualifiedMerc( &Menptr[ bID ] ) )
	{
	}
	else
	{
		bID = RandomSoldierIdFromNewMercsOnPlayerTeam();

		if( bID != -1 )
		{
			gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote = bID;
		}
		else
		{
			return;
		}
	}

	TacticalCharacterDialogue( &Menptr[ bID ], QUOTE_RENEWING_CAUSE_BUDDY_1_ON_TEAM );
}


void HandleShowingRadioLocatorsInMorrisArea()
{
	//Reveal the switch in Morris area
	HandleOpenControlPanelToRevealSwitchInMorrisArea();

	//control panel locater
	BeginMultiPurposeLocator( RADIOLOCATORS_GRIDNO1, 0, TRUE );

	//if we are not in combat
	if( !((gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT)) )
	{
		DelayedMercQuote( RADIOLOCATORS_GRIDNO2, DQ__SHOW_RADIO_LOCATOR, GetWorldTotalSeconds() + 3 );
	}
}

void HandleNewMercSayingContentsOfMorrisNote( SOLDIERTYPE *pSoldier )
{
	TacticalCharacterDialogue( pSoldier, QUOTE_MERC_LEAVING_ALSUCO_SOON );

	TacticalCharacterDialogue( pSoldier, QUOTE_MERC_GONE_UP_IN_PRICE );
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_JERRY_MILO, JM_SE__SHOW_RADIO_LOCATOR, 0, 0, 0, DIALOGUE_TACTICAL_UI );
}

void HandlePlayerHittingSwitchToLaunchMissles()
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;

	//
	// The player Just won the game, remeber this
	//
	gGameSettings.fPlayerFinishedTheGame = TRUE;
	SaveGameSettings();

	//Update the quest log
	InternalEndQuest( QUEST_DESTROY_MISSLES, -1, -1, TRUE );

	//lock the interface
	guiPendingOverrideEvent = LU_BEGINUILOCK;
	HandleTacticalUI( );

	//
	// Loop through all the mercs in the sector and make them run to the elevator
	//
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{       
		// if the soldier was in the complex
		if( pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && pSoldier->bInSector &&
				pSoldier->sSectorX == 15 && pSoldier->sSectorY == 12 && pSoldier->bSectorZ == 3 )
		{
			if( PythSpacesAway( pSoldier->sGridNo, SWITCHTOLAUNCHMISSLES_GRIDNO1 ) < PythSpacesAway( pSoldier->sGridNo, SWITCHTOLAUNCHMISSLES_GRIDNO2 ) )
				pSoldier->EVENT_InternalGetNewSoldierPath( SWITCHTOLAUNCHMISSLES_GRIDNO1, RUNNING, TRUE, TRUE );
			else
				pSoldier->EVENT_InternalGetNewSoldierPath( SWITCHTOLAUNCHMISSLES_GRIDNO2, RUNNING, TRUE, TRUE );
		}
	}

	//Start a timer so we can fade out
	DelayedMercQuote( NOBODY, DQ__FADE_OUT_TO_END_GAME_CINEMATIC, GetWorldTotalSeconds()+2 );
}

void HandleFadingOutToEndGameCinematics()
{
	gFadeOutDoneCallback = DoneFadeOutEndCinematic;

	FadeOutGameScreen( );	

	// UnLock UI!
	guiPendingOverrideEvent = LU_ENDUILOCK;
	HandleTacticalUI( );
}

void HandleOpenControlPanelToRevealSwitchInMorrisArea()
{
	//if the note has already been picked up by a new merc, or the note has been displayed
	if( gJa25SaveStruct.ubMorrisNoteState == MN__PICKED_UP_BY_NEW_MERC ||
			gJa25SaveStruct.ubMorrisNoteState == MN__FINISHED )
	{
		STRUCTURE *		pStructure;
		INT32			iItemIndex;
		ITEM_POOL *		pItemPool;
		

		// first, find the switch item and turn off its trap level

		if ( ItemTypeExistsAtLocation( SWITCHINMORRISAREA_GRIDNO, SWITCH, 0, &iItemIndex ) )
		{
			//gWorldItems[ iItemIndex ]->data.bTrap = 0;
			gWorldItems[ iItemIndex ].object[0]->data.bTrap = 0;
			GetItemPool( SWITCHINMORRISAREA_GRIDNO, &pItemPool, 0 );
			if ( pItemPool )
			{
				SetItemPoolVisibilityOn( pItemPool, ANY_VISIBILITY_VALUE, FALSE );
			}
		}

		//Open up the Control Panel so the switch is visibile
		pStructure = FindStructure( SWITCHINMORRISAREA_GRIDNO, STRUCTURE_GENERIC );
		if( pStructure == NULL )
		{
			return;
		}

		if( SwapStructureForPartnerForcingGraphicalChange( SWITCHINMORRISAREA_GRIDNO, pStructure ) == NULL )
		{
			//an error occured
			return;
		}
	}
}
#endif