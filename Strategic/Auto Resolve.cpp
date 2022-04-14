#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "types.h"
	#include "Auto Resolve.h"
	#include "Strategic Movement.h"
	#include "Queen Command.h"
	#include "Music Control.h"
	#include "PreBattle Interface.h"
	#include "Player Command.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "gameloop.h"
	#include "screenids.h"
	#include "mapscreen.h"
	#include "vobject.h"
	#include "video.h"
	#include "input.h"
	#include "gamescreen.h"
	#include "render dirty.h"
	#include "vObject_Blitters.h"
	#include "sysutil.h"
	#include "Font Control.h"
	#include "Soldier Create.h"
	#include "Overhead.h"
	#include "Interface.h"
	#include "Items.h"
	#include "Weapons.h"
	#include "Sound Control.h"
	#include "Game Clock.h"
	#include "Soldier Profile.h"
	#include "Campaign.h"
	#include "Tactical Save.h"
	#include "Strategic Status.h"
	#include "Map Screen Interface.h"
	#include "text.h"
	#include "WordWrap.h"
	#include "Squads.h"
	#include "Random.h"
	#include "line.h"
	#include "english.h"
	#include "Strategic Pathing.h"
	#include "Strategic Merc Handler.h"
	#include "strategic.h"
	#include "message.h"
	#include "Town Militia.h"
	#include "Animation Data.h"
	#include "Creature Spreading.h"
	#include "Strategic AI.h"
	#include "SkillCheck.h"
	#include "rt time defines.h"
	#include "morale.h"
	#include "Strategic Town Loyalty.h"
	#include "GameSettings.h"
	#include "Soldier macros.h"
	#include "strategicmap.h"
	#include "Quests.h"
	#include "meanwhile.h"
	#include "Inventory Choosing.h"
	#include "Game Event Hook.h"
	#include "Assignments.h"
	#include "cheats.h"
	#include "Map Information.h"
	#include "MilitiaSquads.h"
//	#include "Strategic AI.h"
	#include "interface Dialogue.h"
	#include "AIInternals.h" // added by SANDRO
	#include "Bullets.h" // HEADROCK HAM 5, for use with Bullet Impact.
	#include "CampaignStats.h"				// added by Flugente
	#include "DynamicDialogue.h"			// added by Flugente
	#include "MilitiaIndividual.h"			// added by Flugente
	#include "Rebel Command.h"
#endif

#include "Reinforcement.h"

//#define INVULNERABILITY

// default = 7 (%)
#define EXP_BONUS	7

#define MAX_AR_TEAM_SIZE 255 // Must fit in 8 bits

#define REINFORCMENT_ATTACK_DELAY_PER_SOLDIER_IN_SECTOR 1000

INT32 giMaxEnemiesToRender = 40;
INT32 giMaxMilitiaToRender = 20;//Changes depending on merc amount

extern UINT8 gubReinforcementMinEnemyStaticGroupSize;
extern BOOLEAN gfStrategicMilitiaChangesMade;

extern void ResetMilitia();
extern BOOLEAN AutoReload( SOLDIERTYPE *pSoldier, bool aReloadEvenIfNotEmpty );
extern HVSURFACE ghFrameBuffer;
BOOLEAN gfTransferTacticalOppositionToAutoResolve = FALSE;

//button images
enum
{
	PAUSE_BUTTON,
	PLAY_BUTTON,
	FAST_BUTTON,
	FINISH_BUTTON,
	YES_BUTTON,
	NO_BUTTON,
	BANDAGE_BUTTON,
	RETREAT_BUTTON,
	DONEWIN_BUTTON,
	DONELOSE_BUTTON,
	NUM_AR_BUTTONS
};


typedef struct SOLDIERCELL
{
	SOLDIERTYPE *pSoldier;
	MOUSE_REGION *pRegion; //only used for player mercs.
	UINT32 uiVObjectID;
	UINT16 usIndex;
	UINT32 uiFlags;
	UINT16 usFrame;
	INT16 xp, yp;
	UINT16 usAttack, usDefence;
	UINT32 usNextAttack;
	UINT16 usNextHit[3];
	UINT16 usHitDamage[3];
	struct SOLDIERCELL *pAttacker[3];
	UINT32 uiFlashTime;
	INT8 bWeaponSlot;
}SOLDIERCELL;

typedef struct AUTORESOLVE_STRUCT
{
	SOLDIERCELL *pRobotCell;

	//IDs into the graphic images
	INT32 iPanelImages;
	INT32 iButton[ NUM_AR_BUTTONS ];
	INT32 iButtonImage[ NUM_AR_BUTTONS ];
	INT32 iFaces; //for generic civs and enemies
    // WDS - make number of mercenaries, etc. be configurable
	//INT32 iMercFaces[CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS]; //for each merc face
	INT32 iIndent;
	INT32 iInterfaceBuffer;
	INT32 iNumMercFaces;
	INT32 iActualMercFaces; //this represents the real number of merc faces.	Because
													//my debug mode allows to freely add and subtract mercs, we
													//can add/remove temp mercs, but we don't want to remove the
													//actual mercs.
	UINT32 uiTimeSlice;
	UINT32 uiTotalElapsedBattleTimeInMilliseconds;
	UINT32 uiPrevTime, uiCurrTime;
	UINT32 uiStartExpanding;
	UINT32 uiEndExpanding;
	UINT32 uiPreRandomIndex;

	SGPRect Rect, ExRect;

	UINT32 usPlayerAttack;
	UINT32 usPlayerDefence;
	UINT32 usEnemyAttack;
	UINT32 usEnemyDefence;
	INT16 sWidth, sHeight;
	INT16 sCenterStartX;

	UINT8 ubEnemyLeadership;
	UINT8 ubPlayerLeadership;
	UINT8 ubMercs, ubCivs, ubEnemies;
	UINT8 ubAdmins, ubTroops, ubElites, ubTanks, ubJeeps, ubRobots;
	UINT8 ubYMCreatures, ubYFCreatures, ubAMCreatures, ubAFCreatures;
	UINT8 ubBloodcats;
	UINT8 ubZombies;
	UINT8 ubBandits;
	UINT8 ubAliveMercs, ubAliveCivs, ubAliveEnemies;
	UINT8 ubMercCols, ubMercRows;
	UINT8 ubEnemyCols, ubEnemyRows;
	UINT8 ubCivCols, ubCivRows;
	UINT8 ubTimeModifierPercentage;
	UINT8 ubSectorX, ubSectorY;
	INT8 bVerticalOffset;

	BOOLEAN fRenderAutoResolve;
	BOOLEAN fExitAutoResolve;
	BOOLEAN fPaused;
	BOOLEAN fDebugInfo;
	BOOLEAN ubBattleStatus;
	BOOLEAN fUnlimitedAmmo;
	BOOLEAN fSound;
	BOOLEAN ubPlayerDefenceAdvantage;
	BOOLEAN ubEnemyDefenceAdvantage;
	BOOLEAN fInstantFinish;
	BOOLEAN fAllowCapture;
	BOOLEAN fPlayerRejectedSurrenderOffer;
	BOOLEAN fPendingSurrender;
	BOOLEAN fExpanding;
	BOOLEAN fShowInterface;
	BOOLEAN fEnteringAutoResolve;
	BOOLEAN fMoraleEventsHandled;
	BOOLEAN fCaptureNotPermittedDueToEPCs;

	MOUSE_REGION AutoResolveRegion;

}AUTORESOLVE_STRUCT;

//Classifies the type of soldier the soldier cell is
#define CELL_MERC							0x00000001
#define CELL_MILITIA					0x00000002
#define CELL_ELITE						0x00000004
#define CELL_TROOP						0x00000008
#define	CELL_ADMIN						0x00000010
#define CELL_AF_CREATURE			0x00000020
#define CELL_AM_CREATURE			0x00000040
#define CELL_YF_CREATURE			0x00000080
#define CELL_YM_CREATURE			0x00000100
//The team leader is the one with the highest leadership.
//There can only be one teamleader per side (mercs/civs and enemies)
#define CELL_TEAMLEADER				0x00000200
//Combat flags
#define CELL_FIREDATTARGET		0x00000400
#define CELL_DODGEDATTACK			0x00000800
#define CELL_HITBYATTACKER		0x00001000
#define CELL_HITLASTFRAME			0x00002000
//Cell statii
#define CELL_SHOWRETREATTEXT	0x00004000
#define CELL_RETREATING				0x00008000
#define CELL_RETREATED				0x00010000
#define CELL_DIRTY						0x00020000
#define CELL_PROCESSED				0x00040000
#define CELL_ASSIGNED					0x00080000
#define CELL_EPC							0x00100000
#define CELL_ROBOT						0x00200000
#define CELL_TANK						0x00400000
#define CELL_JEEP						0x00800000
#define CELL_BLOODCAT					0x01000000
#define CELL_ZOMBIE						0x02000000
#define CELL_BANDIT						0x04000000
#define CELL_ENEMYROBOT					0x08000000

//Combined flags
#define CELL_PLAYER						( CELL_MERC | CELL_MILITIA )
#define CELL_ENEMY						( CELL_ELITE | CELL_TROOP | CELL_ADMIN | CELL_TANK | CELL_JEEP | CELL_BANDIT | CELL_ENEMYROBOT )
#define CELL_FEMALECREATURE				( CELL_AF_CREATURE | CELL_YF_CREATURE )
#define CELL_MALECREATURE				( CELL_AM_CREATURE | CELL_YM_CREATURE )
#define CELL_BUG						( CELL_FEMALECREATURE | CELL_MALECREATURE )
#define CELL_CREATURE					( CELL_BUG | CELL_BLOODCAT | CELL_ZOMBIE )

#define CELL_INVOLVEDINCOMBAT ( CELL_FIREDATTARGET | CELL_DODGEDATTACK | CELL_HITBYATTACKER )

enum
{
	BATTLE_IN_PROGRESS,
	BATTLE_VICTORY,
	BATTLE_DEFEAT,
	BATTLE_RETREAT,
	BATTLE_SURRENDERED,
	BATTLE_CAPTURED
};

//panel pieces
enum
{
	TL_BORDER,
	T_BORDER,
	TR_BORDER,
	L_BORDER,
	C_TEXTURE,
	R_BORDER,
	BL_BORDER,
	B_BORDER,
	BR_BORDER,
	TOP_MIDDLE,
	AUTO_MIDDLE,
	BOT_MIDDLE,
	MERC_PANEL,
	OTHER_PANEL,
};

extern void CreateDestroyMapInvButton();
extern void DestroyMapCharInvIOregions();

//Autoresolve sets this variable which defaults to -1 when not needed.
INT16 gsEnemyGainedControlOfSectorID = -1;
INT16 gsCiviliansEatenByMonsters = -1;

BOOLEAN ProcessLoyalty();
//Autoresolve handling -- keyboard input, callbacks
void HandleAutoResolveInput();
void PauseButtonCallback( GUI_BUTTON *btn, INT32 reason );
void PlayButtonCallback( GUI_BUTTON *btn, INT32 reason );
void FastButtonCallback( GUI_BUTTON *btn, INT32 reason );
void FinishButtonCallback( GUI_BUTTON *btn, INT32 reason );
void RetreatButtonCallback( GUI_BUTTON *btn, INT32 reason );
void BandageButtonCallback( GUI_BUTTON *btn, INT32 reason );
void DoneButtonCallback( GUI_BUTTON *btn, INT32 reason );
void MercCellMouseMoveCallback( MOUSE_REGION *reg, INT32 reason );
void MercCellMouseClickCallback( MOUSE_REGION *reg, INT32 reason );

void DetermineBandageButtonState();

//Surrender interface
void SetupDoneInterface();
void SetupSurrenderInterface();
void HideSurrenderInterface();
void AcceptSurrenderCallback( GUI_BUTTON *btn, INT32 reason );
void RejectSurrenderCallback( GUI_BUTTON *btn, INT32 reason );

//Precalculations for interface positioning and the calculation routines to do so.
void CalculateAutoResolveInfo();
void CalculateSoldierCells( BOOLEAN fReset );
void CalculateRowsAndColumns();
void CreateAutoResolveInterface();
void RemoveAutoResolveInterface( BOOLEAN fDeleteForGood );

//Battle system routines
void DetermineTeamLeader( BOOLEAN fFriendlyTeam );
void CalculateAttackValues();
void ProcessBattleFrame();
BOOLEAN IsBattleOver();
BOOLEAN AttemptPlayerCapture();

void AutoBandageFinishedCallback( UINT8 ubResult );

//Debug utilities
void ResetAutoResolveInterface();
void CreateTempPlayerMerc();
void DrawDebugText( SOLDIERCELL *pCell );

//Rendering routines
void RenderAutoResolve();
void RenderSoldierCellHealth( SOLDIERCELL *pCell );
void RenderSoldierCell( SOLDIERCELL *pCell );
void RenderSoldierCellBars( SOLDIERCELL *pCell );

//Dynamic globals -- to conserve memory, all global variables are allocated upon entry
//and deleted before we leave.
AUTORESOLVE_STRUCT *gpAR = NULL;
SOLDIERCELL *gpMercs = NULL;
SOLDIERCELL *gpCivs = NULL;
SOLDIERCELL *gpEnemies = NULL;

//Simple wrappers for autoresolve sounds that are played.
void PlayAutoResolveSample( UINT32 usNum, UINT32 usRate, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan )
{
	if( gpAR->fSound )
	{
		PlayJA2Sample( usNum, usRate, ubVolume, ubLoops, uiPan );
	}
}

void	PlayAutoResolveSampleFromFile( STR8 szFileName, UINT32 usRate, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan )
{
	if( gpAR->fSound )
	{
		PlayJA2SampleFromFile( szFileName, usRate, ubVolume, ubLoops, uiPan );
	}
}


extern void ClearPreviousAIGroupAssignment( GROUP *pGroup );

void EliminateAllMercs()
{
	SOLDIERCELL *pAttacker = NULL;
	INT32 i, iNum = 0;
	if( gpAR )
	{
		for( i = 0; i < gpAR->ubEnemies; i++ )
		{
			if( gpEnemies[ i ].pSoldier->stats.bLife )
			{
				pAttacker = &gpEnemies[ i ];
				break;
			}
		}
		if( pAttacker )
		{
			for( i = 0; i < gpAR->ubMercs; i++ )
			{
				if( gpMercs[ i ].pSoldier->stats.bLife )
				{
					iNum++;
					gpMercs[ i ].pSoldier->stats.bLife = 1;
					gpMercs[ i ].usNextHit[0] = (UINT16)(250 * iNum);
					gpMercs[ i ].usHitDamage[0] = 100;
					gpMercs[ i ].pAttacker[0] = pAttacker;
				}
			}
		}
	}
}

void EliminateAllFriendlies()
{
	INT32 i;
	if( gpAR )
	{
		for( i = 0; i < gpAR->ubMercs; i++ )
		{
			gpMercs[ i ].pSoldier->stats.bLife = 0;
		}
		gpAR->ubAliveMercs = 0;
		for( i = 0; i < gpAR->ubCivs; i++ )
		{
			gpCivs[ i ].pSoldier->stats.bLife = 0;
		}
		gpAR->ubAliveCivs = 0;
	}
}

void EliminateAllEnemies( UINT8 ubSectorX, UINT8 ubSectorY )
{
	GROUP *pGroup, *pDeleteGroup;
	SECTORINFO *pSector;
	INT32 i;
	UINT8 ubNumEnemies[ NUM_ENEMY_RANKS ];
	UINT8 ubNumTanks = 0;
	UINT8 ubNumJeeps = 0;
	UINT8 ubNumRobots = 0;
	UINT8 ubRankIndex;

	//Clear any possible battle locator
	gfBlitBattleSectorLocator = FALSE;

	pGroup = gpGroupList;
	pSector = &SectorInfo[ SECTOR( ubSectorX, ubSectorY ) ];

	// if we're doing this from the Pre-Battle interface, gpAR is NULL, and RemoveAutoResolveInterface(0 won't happen, so
	// we must process the enemies killed right here & give out loyalty bonuses as if the battle had been fought & won
	if( !gpAR )
	{
		GetNumberOfEnemiesInSector( ubSectorX, ubSectorY, &ubNumEnemies[0], &ubNumEnemies[1], &ubNumEnemies[2], &ubNumRobots, &ubNumTanks, &ubNumJeeps );

		for ( ubRankIndex = 0; ubRankIndex < NUM_ENEMY_RANKS; ++ubRankIndex )
		{
			for ( i = 0; i < ubNumEnemies[ ubRankIndex ]; ++i )
			{
				if( ProcessLoyalty() )
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_ENEMY_KILLED, ubSectorX, ubSectorY, 0 );

				TrackEnemiesKilled( ENEMY_KILLED_IN_AUTO_RESOLVE, RankIndexToSoldierClass( ubRankIndex ) );
			}
		}

		if ( ProcessLoyalty( ) )
		{
			for ( i = 0; i < ubNumTanks; ++i )
			{
				HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_ENEMY_KILLED, ubSectorX, ubSectorY, 0 );
			}

			HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_BATTLE_WON, ubSectorX, ubSectorY, 0 );
		}
	}

	if( !gpAR || gpAR->ubBattleStatus != BATTLE_IN_PROGRESS )
	{
		//Remove the defend force here.
		pSector->ubNumTroops = 0;
		pSector->ubNumElites = 0;
		pSector->ubNumAdmins = 0;
		pSector->ubNumTanks = 0;
		pSector->ubNumJeeps = 0;
		pSector->ubNumRobots = 0;
		pSector->ubNumCreatures = 0;
		pSector->bLastKnownEnemies = 0;
		//Remove the mobile forces here, but only if battle is over.
		while( pGroup )
		{
			if ( pGroup->usGroupTeam != OUR_TEAM && pGroup->ubSectorX == ubSectorX && pGroup->ubSectorY == ubSectorY )
			{
				ClearPreviousAIGroupAssignment( pGroup );
				pDeleteGroup = pGroup;
				pGroup = pGroup->next;
				if( gpBattleGroup == pDeleteGroup )
					gpBattleGroup = NULL;
				RemovePGroup( pDeleteGroup );
			}
			else
				pGroup = pGroup->next;
		}
		if( gpBattleGroup )
		{
			CalculateNextMoveIntention( gpBattleGroup );
		}
		// set this sector as taken over
		SetThisSectorAsPlayerControlled( ubSectorX, ubSectorY, 0, TRUE );
		RecalculateSectorWeight( (UINT8)SECTOR( ubSectorX, ubSectorY ) );

		// dirty map panel
		fMapPanelDirty = TRUE;
	}

	if( gpAR )
	{
		for( i = 0; i < gpAR->ubEnemies; ++i )
		{
			gpEnemies[ i ].pSoldier->stats.bLife = 0;
		}
		gpAR->ubAliveEnemies = 0;
	}
	gpBattleGroup = NULL;
}

#define ORIG_LEFT			26
#define ORIG_TOP			53
#define ORIG_RIGHT		92
#define ORIG_BOTTOM		84

void DoTransitionFromPreBattleInterfaceToAutoResolve()
{
	SGPRect SrcRect, DstRect;
	UINT32 uiStartTime, uiCurrTime;
	INT32 iPercentage, iFactor;
	UINT32 uiTimeRange;
	INT16 sStartLeft, sEndLeft, sStartTop, sEndTop;
	INT32 iLeft, iTop, iWidth, iHeight;

	PauseTime( FALSE );

	gpAR->fShowInterface = TRUE;

	SrcRect.iLeft = gpAR->Rect.iLeft;
	SrcRect.iTop = gpAR->Rect.iTop;
	SrcRect.iRight = gpAR->Rect.iRight;
	SrcRect.iBottom = gpAR->Rect.iBottom;

	iWidth = SrcRect.iRight - SrcRect.iLeft + 1;
	iHeight = SrcRect.iBottom - SrcRect.iTop + 1;

	uiTimeRange = 1000;
	iPercentage = 0;
	uiStartTime = GetJA2Clock();

	sStartLeft = 59 + xResOffset;
	sStartTop = 69 + yResOffset;
	sEndLeft = SrcRect.iLeft + gpAR->sWidth / 2;
	sEndTop = SrcRect.iTop + gpAR->sHeight / 2;

	//save the prebattle/mapscreen interface background
	BlitBufferToBuffer( FRAME_BUFFER, guiEXTRABUFFER, 0 + xResOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	//render the autoresolve panel
	RenderAutoResolve();
	RenderButtons();
	RenderButtonsFastHelp();
	//save it
	BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, (UINT16)SrcRect.iLeft, (UINT16)SrcRect.iTop, (UINT16)SrcRect.iRight, (UINT16)SrcRect.iBottom );

	//hide the autoresolve
	BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, (UINT16)SrcRect.iLeft, (UINT16)SrcRect.iTop, (UINT16)SrcRect.iRight, (UINT16)SrcRect.iBottom );

	PlayJA2SampleFromFile( "SOUNDS\\Laptop power up (8-11).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
	while( iPercentage < 100	)
	{
		uiCurrTime = GetJA2Clock();
		iPercentage = (uiCurrTime-uiStartTime) * 100 / uiTimeRange;
		iPercentage = min( iPercentage, 100 );

		//Factor the percentage so that it is modified by a gravity falling acceleration effect.
		iFactor = (iPercentage - 50) * 2;
		if( iPercentage < 50 )
			iPercentage = (UINT32)(iPercentage + iPercentage * iFactor * 0.01 + 0.5);
		else
			iPercentage = (UINT32)(iPercentage + (100-iPercentage) * iFactor * 0.01 + 0.05);

		//Calculate the center point.
		iLeft = sStartLeft + (sEndLeft-sStartLeft+1) * iPercentage / 100;
		iTop = sStartTop + (sEndTop-sStartTop+1) * iPercentage / 100;

		DstRect.iLeft = iLeft - iWidth * iPercentage / 200;
		DstRect.iRight = DstRect.iLeft + max( iWidth * iPercentage / 100, 1 );
		DstRect.iTop = iTop - iHeight * iPercentage / 200;
		DstRect.iBottom = DstRect.iTop + max( iHeight * iPercentage / 100, 1 );

		BltStretchVideoSurface( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, 0, &SrcRect, &DstRect );
		InvalidateScreen();
		RefreshScreen( NULL );

		//Restore the previous rect.
		BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, (UINT16)DstRect.iLeft, (UINT16)DstRect.iTop,
			(UINT16)(DstRect.iRight-DstRect.iLeft+1), (UINT16)(DstRect.iBottom-DstRect.iTop+1) );
	}
	//BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, 640, 480 );
}

extern UINT8 guiDirNumber;

void EnterAutoResolveMode( UINT8 ubSectorX, UINT8 ubSectorY )
{
	guiDirNumber = 0;

	//Set up mapscreen for removal
	SetPendingNewScreen( AUTORESOLVE_SCREEN );
	if (isWidescreenUI())
	{
		DestroyMapCharInvIOregions();
	}
	else
	{
		CreateDestroyMapInvButton();
	}
	RenderButtons();

DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Autoresolve1");
    // WDS - make number of mercenaries, etc. be configurable
	//Allocate memory for all the globals while we are in this mode.
	gpAR = (AUTORESOLVE_STRUCT*)MemAlloc( sizeof( AUTORESOLVE_STRUCT ) );
	Assert( gpAR );
	memset( gpAR, 0, sizeof( AUTORESOLVE_STRUCT ) );
	//Mercs 
	gpMercs = (SOLDIERCELL*)MemAlloc( sizeof( SOLDIERCELL) * MAX_AR_TEAM_SIZE );
	Assert( gpMercs );
	memset( gpMercs, 0, sizeof( SOLDIERCELL ) * MAX_AR_TEAM_SIZE );
	//Militia
	gpCivs = (SOLDIERCELL*)MemAlloc( sizeof( SOLDIERCELL) * MAX_AR_TEAM_SIZE	);
	Assert( gpCivs );
	memset( gpCivs, 0, sizeof( SOLDIERCELL ) * MAX_AR_TEAM_SIZE	);
	//Enemies
	gpEnemies = (SOLDIERCELL*)MemAlloc( sizeof( SOLDIERCELL) * MAX_AR_TEAM_SIZE	);
	Assert( gpEnemies );
	memset( gpEnemies, 0, sizeof( SOLDIERCELL ) * MAX_AR_TEAM_SIZE	);

	//Set up autoresolve
	gpAR->fEnteringAutoResolve = TRUE;
	gpAR->ubSectorX = ubSectorX;
	gpAR->ubSectorY = ubSectorY;
	gpAR->ubBattleStatus = BATTLE_IN_PROGRESS;
	gpAR->uiTimeSlice = 1000;
	gpAR->uiTotalElapsedBattleTimeInMilliseconds = 0;
	gpAR->fSound = TRUE;
	gpAR->fMoraleEventsHandled = FALSE;
	gpAR->uiPreRandomIndex = guiPreRandomIndex;

	//Determine who gets the defensive advantage
	switch( GetEnemyEncounterCode() )
	{
		case ENEMY_ENCOUNTER_CODE:
			gpAR->ubPlayerDefenceAdvantage = 21; //Skewed to the player's advantage for convenience purposes.
			break;
		case ENEMY_INVASION_CODE:
			gpAR->ubPlayerDefenceAdvantage = 0;
			break;
		case CREATURE_ATTACK_CODE:
		case BLOODCAT_ATTACK_CODE:
		case ZOMBIE_ATTACK_CODE:
		case BANDIT_ATTACK_CODE:
			gpAR->ubPlayerDefenceAdvantage = 0;
			break;
		default:
			//shouldn't happen
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_RED, MSG_ERROR, L"Autoresolving with entering enemy sector code %d -- illegal KM:1", GetEnemyEncounterCode() );
			#endif
			break;
	}
}

UINT32 AutoResolveScreenInit()
{
	return TRUE;
}

UINT32 AutoResolveScreenShutdown()
{
	gpBattleGroup = NULL;
	return TRUE;
}

UINT32 AutoResolveScreenHandle()
{
	RestoreBackgroundRects();

	if( !gpAR )
	{
		gfEnteringMapScreen = TRUE;
		return MAP_SCREEN;
	}
	if( gpAR->fEnteringAutoResolve )
	{
		UINT8 *pDestBuf;
		UINT32 uiDestPitchBYTES;
		SGPRect ClipRect;
		gpAR->fEnteringAutoResolve = FALSE;
		//Take the framebuffer, shade it, and save it to the SAVEBUFFER.
		ClipRect.iLeft = 0 + xResOffset;
		ClipRect.iTop = 0;
		/*ClipRect.iRight = 640;
		ClipRect.iBottom = 480;*/
		ClipRect.iRight = SCREEN_WIDTH;
		ClipRect.iBottom = SCREEN_HEIGHT;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		Blt16BPPBufferShadowRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
		UnLockVideoSurface( FRAME_BUFFER );
		//BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, 640, 480 );
		BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0 + xResOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		KillPreBattleInterface();
		CalculateAutoResolveInfo();
		CalculateSoldierCells( FALSE );
		CreateAutoResolveInterface();
		DetermineTeamLeader( TRUE ); //friendly team
		DetermineTeamLeader( FALSE ); //enemy team
		CalculateAttackValues();
		if( gfExtraBuffer )
		{
			DoTransitionFromPreBattleInterfaceToAutoResolve();
		}
		else
			gpAR->fExpanding = TRUE;
		gpAR->fRenderAutoResolve = TRUE;
	}
	if( gpAR->fExitAutoResolve )
	{
		gfEnteringMapScreen = TRUE;
		RemoveAutoResolveInterface( TRUE );
		return MAP_SCREEN;
	}
	if( gpAR->fPendingSurrender )
	{
		gpAR->uiPrevTime = gpAR->uiCurrTime = GetJA2Clock();

	}
	else if( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS && !gpAR->fExpanding )
	{
		ProcessBattleFrame();
	}
	HandleAutoResolveInput();
	RenderAutoResolve();


	SaveBackgroundRects();
	RenderButtons();
	RenderButtonsFastHelp();
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();
	return AUTORESOLVE_SCREEN;
}

void RefreshMerc( SOLDIERTYPE *pSoldier )
{
	pSoldier->stats.bLife = pSoldier->stats.bLifeMax;
	pSoldier->bBleeding = 0;
	pSoldier->iHealableInjury = 0; // added by SANDRO
	pSoldier->bBreath = pSoldier->bBreathMax = 100;
	pSoldier->sBreathRed = 0;
	if( gpAR->pRobotCell)
	{
		gpAR->pRobotCell->pSoldier->UpdateRobotControllerGivenRobot(	);
	}
	//gpAR->fUnlimitedAmmo = TRUE;
}

//Now assign the pSoldier->ubGroupIDs for the enemies, so we know where to remove them.	Start with
//stationary groups first.
void AssociateEnemiesWithStrategicGroups()
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps, ubNumRobots;	//how many soldiers of the type do we still have to assign to a group? 
	UINT8 ubISNumAdmins, ubISNumTroops, ubISNumElites, ubISNumTanks, ubISNumJeeps, ubISNumRobots;
	UINT8 ubNumElitesInGroup, ubNumTroopsInGroup, ubNumAdminsInGroup, ubNumTanksInGroup, ubNumJeepsInGroup, ubNumRobotsInGroup;
	INT32 i;
	UINT8 pSectors[4];
	UINT8 ubDirAmount;
	UINT8 ubCurrSI;

	if ( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE || 
		GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
		GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE  ||
		GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
		return;

	pSector = &SectorInfo[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ];

	//grab the number of each type in the stationary sector
	ubNumAdmins = pSector->ubNumAdmins;
	ubNumTroops = pSector->ubNumTroops;
	ubNumElites = pSector->ubNumElites;
	ubNumTanks = pSector->ubNumTanks;
	ubNumJeeps = pSector->ubNumJeeps;
	ubNumRobots = pSector->ubNumRobots;

	//Now go through our enemies in the autoresolve array, and assign the ubGroupID to the soldier
	//Stationary groups have a group ID of 0 - first assign enemies from those stationary groups
	for( i = 0; i < gpAR->ubEnemies; ++i )
	{
		if ( gpEnemies[i].uiFlags & CELL_TANK && ubNumTanks )	//is this soldier a tank? and we still have some tanks to add? (since there might not be a static tank in sector) 
		{
			gpEnemies[ i ].pSoldier->ubGroupID = 0;
			gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
			ubNumTanks--;
		}
		else if ( gpEnemies[i].uiFlags & CELL_JEEP && ubNumJeeps )
		{
			gpEnemies[i].pSoldier->ubGroupID = 0;
			gpEnemies[i].uiFlags |= CELL_ASSIGNED;
			ubNumJeeps--;
		}
		else if ( gpEnemies[i].uiFlags & CELL_ENEMYROBOT && ubNumRobots )
		{
			gpEnemies[i].pSoldier->ubGroupID = 0;
			gpEnemies[i].uiFlags |= CELL_ASSIGNED;
			ubNumRobots--;
		}
		else if ( gpEnemies[i].uiFlags & CELL_ELITE && ubNumElites )
		{
			gpEnemies[ i ].pSoldier->ubGroupID = 0;
			gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
			ubNumElites--;
		}
		else if( gpEnemies[ i ].uiFlags & CELL_TROOP && ubNumTroops )
		{
			gpEnemies[ i ].pSoldier->ubGroupID = 0;
			gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
			ubNumTroops--;
		}
		else if( gpEnemies[ i ].uiFlags & CELL_ADMIN && ubNumAdmins )
		{
			gpEnemies[ i ].pSoldier->ubGroupID = 0;
			gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
			ubNumAdmins--;
		}
	}

	ubNumAdmins = gpAR->ubAdmins - pSector->ubNumAdmins;
	ubNumTroops = gpAR->ubTroops - pSector->ubNumTroops;
	ubNumElites = gpAR->ubElites - pSector->ubNumElites;
	ubNumTanks = gpAR->ubTanks - pSector->ubNumTanks;
	ubNumJeeps = gpAR->ubJeeps - pSector->ubNumJeeps;
	ubNumRobots = gpAR->ubRobots - pSector->ubNumRobots;

	if ( !ubNumElites && !ubNumTroops && !ubNumAdmins && !ubNumTanks && !ubNumJeeps && !ubNumRobots )
	{ //All troops accounted for.
		return;
	}

	//Now assign the rest of the soldiers to groups
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && pGroup->ubSectorX == gpAR->ubSectorX && pGroup->ubSectorY == gpAR->ubSectorY )
		{
			ubNumElitesInGroup = pGroup->pEnemyGroup->ubNumElites;
			ubNumTroopsInGroup = pGroup->pEnemyGroup->ubNumTroops;
			ubNumAdminsInGroup = pGroup->pEnemyGroup->ubNumAdmins;
			ubNumTanksInGroup = pGroup->pEnemyGroup->ubNumTanks;
			ubNumJeepsInGroup = pGroup->pEnemyGroup->ubNumJeeps;
			ubNumRobotsInGroup = pGroup->pEnemyGroup->ubNumRobots;
			for( i = 0; i < gpAR->ubEnemies; i++ )
			{
				if( !(gpEnemies[ i ].uiFlags & CELL_ASSIGNED) )	//has this soldier already been assigned to a cell and therefore a group (while processing the static enemies above) ?
				{
					if (ubNumTanks && ubNumTanksInGroup && gpEnemies[i].uiFlags & CELL_TANK ) //is there still a tank to assign and is this a cell for tank?
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumTanks--;
						ubNumTanksInGroup--;
					}
					else if ( ubNumJeeps && ubNumJeepsInGroup && gpEnemies[i].uiFlags & CELL_JEEP )
					{
						gpEnemies[i].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[i].uiFlags |= CELL_ASSIGNED;
						ubNumJeeps--;
						ubNumJeepsInGroup--;
					}
					else if ( ubNumRobots && ubNumRobotsInGroup && gpEnemies[i].uiFlags & CELL_ENEMYROBOT )
					{
						gpEnemies[i].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[i].uiFlags |= CELL_ASSIGNED;
						ubNumRobots--;
						ubNumRobotsInGroup--;
					}
					else if (ubNumElites && ubNumElitesInGroup && gpEnemies[i].uiFlags & CELL_ELITE)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumElites--;
						ubNumElitesInGroup--;
					}
					else if (ubNumTroops && ubNumTroopsInGroup && gpEnemies[i].uiFlags & CELL_TROOP)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumTroops--;
						ubNumTroopsInGroup--;
					}
					else if (ubNumAdmins && ubNumAdminsInGroup && gpEnemies[i].uiFlags & CELL_ADMIN)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumAdmins--;
						ubNumAdminsInGroup--;
					} 
					}
				}
			}
		pGroup = pGroup->next;
	}

/////////////////////////////////////////////////////////////////////////// Reinforcement

	pGroup = gpGroupList;
	while( pGroup )
	{
		// Don't process road block. It'll be processed as static	(because their ubGroupID is 0 so the first term eval to false)
		if ( pGroup->ubGroupID && pGroup->usGroupTeam == ENEMY_TEAM && IsGroupInARightSectorToReinforce( pGroup, gpAR->ubSectorX, gpAR->ubSectorY ) )
		{
			ubNumElitesInGroup = pGroup->pEnemyGroup->ubNumElites;
			ubNumTroopsInGroup = pGroup->pEnemyGroup->ubNumTroops;
			ubNumAdminsInGroup = pGroup->pEnemyGroup->ubNumAdmins;
			ubNumTanksInGroup = pGroup->pEnemyGroup->ubNumTanks;
			ubNumJeepsInGroup = pGroup->pEnemyGroup->ubNumJeeps;
			ubNumRobotsInGroup = pGroup->pEnemyGroup->ubNumRobots;
			for( i = 0; i < gpAR->ubEnemies; i++ )
			{
				if( !(gpEnemies[ i ].uiFlags & CELL_ASSIGNED) )
				{
					if (ubNumTanks && ubNumTanksInGroup &&  gpEnemies[i].uiFlags & CELL_TANK)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumTanks--;
						ubNumTanksInGroup--;
					}
					else if ( ubNumJeeps && ubNumJeepsInGroup &&  gpEnemies[i].uiFlags & CELL_JEEP )
					{
						gpEnemies[i].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[i].uiFlags |= CELL_ASSIGNED;
						ubNumJeeps--;
						ubNumJeepsInGroup--;
					}
					else if ( ubNumRobots && ubNumRobotsInGroup &&  gpEnemies[i].uiFlags & CELL_ENEMYROBOT )
					{
						gpEnemies[i].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[i].uiFlags |= CELL_ASSIGNED;
						ubNumRobots--;
						ubNumRobotsInGroup--;
					}
					else if (ubNumElites && ubNumElitesInGroup &&  gpEnemies[i].uiFlags & CELL_ELITE)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumElites--;
						ubNumElitesInGroup--;
					}
					else if (ubNumTroops && ubNumTroopsInGroup &&  gpEnemies[i].uiFlags & CELL_TROOP)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumTroops--;
						ubNumTroopsInGroup--;
					}
					else if (ubNumAdmins && ubNumAdminsInGroup &&  gpEnemies[i].uiFlags & CELL_ADMIN)
					{
						gpEnemies[ i ].pSoldier->ubGroupID = pGroup->ubGroupID;
						gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
						ubNumAdmins--;
						ubNumAdminsInGroup--;
					}
					}
				}
			}
		pGroup = pGroup->next;
	}

	// Set GroupID = 0 for the rest	(that includes reinforcements)
	ubDirAmount = GetAdjacentSectors( pSectors, gpAR->ubSectorX, gpAR->ubSectorY );

	for( ubCurrSI = 0; ubCurrSI < ubDirAmount; ++ubCurrSI )
	{
		pSector = &SectorInfo[ pSectors[ ubCurrSI ] ];

		ubISNumAdmins = pSector->ubNumAdmins;
		ubISNumTroops = pSector->ubNumTroops;
		ubISNumElites = pSector->ubNumElites;
		ubISNumTanks = pSector->ubNumTanks;
		ubISNumJeeps = pSector->ubNumJeeps;
		ubISNumRobots = pSector->ubNumRobots;

		for( i = 0; i < gpAR->ubEnemies; ++i )
		{
			if ( ubISNumAdmins + ubISNumTroops + ubISNumElites + ubISNumTanks + ubISNumJeeps + ubISNumRobots <= gubReinforcementMinEnemyStaticGroupSize ) break;	//if group would be left understaffed, it wont reinforce - go chceck another sector (what if are there more groups here?)

			if( !(gpEnemies[ i ].uiFlags & CELL_ASSIGNED) )
			{
				if ( gpEnemies[i].uiFlags & CELL_TANK && ubISNumTanks && ubNumTanks )
				{
					gpEnemies[ i ].pSoldier->ubGroupID = 0;
					gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
					gpEnemies[ i ].pSoldier->sSectorX = SECTORX( pSectors[ ubCurrSI ] );
					gpEnemies[ i ].pSoldier->sSectorY = SECTORY( pSectors[ ubCurrSI ] );
					ubISNumTanks--;
					ubNumTanks--;
				}
				else if ( gpEnemies[i].uiFlags & CELL_JEEP && ubISNumJeeps && ubNumJeeps )
				{
					gpEnemies[i].pSoldier->ubGroupID = 0;
					gpEnemies[i].uiFlags |= CELL_ASSIGNED;
					gpEnemies[i].pSoldier->sSectorX = SECTORX( pSectors[ubCurrSI] );
					gpEnemies[i].pSoldier->sSectorY = SECTORY( pSectors[ubCurrSI] );
					ubISNumJeeps--;
					ubNumJeeps--;
				}
				else if ( gpEnemies[i].uiFlags & CELL_ENEMYROBOT && ubISNumRobots && ubNumRobots )
				{
					gpEnemies[i].pSoldier->ubGroupID = 0;
					gpEnemies[i].uiFlags |= CELL_ASSIGNED;
					gpEnemies[i].pSoldier->sSectorX = SECTORX( pSectors[ubCurrSI] );
					gpEnemies[i].pSoldier->sSectorY = SECTORY( pSectors[ubCurrSI] );
					ubISNumRobots--;
					ubNumRobots--;
				}
				else if ( gpEnemies[i].uiFlags & CELL_ELITE && ubISNumElites && ubNumElites )
				{
					gpEnemies[ i ].pSoldier->ubGroupID = 0;
					gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
					gpEnemies[ i ].pSoldier->sSectorX = SECTORX( pSectors[ ubCurrSI ] );
					gpEnemies[ i ].pSoldier->sSectorY = SECTORY( pSectors[ ubCurrSI ] );
					ubISNumElites--;
					ubNumElites--;
				}
				else if( gpEnemies[ i ].uiFlags & CELL_TROOP && ubISNumTroops && ubNumTroops )
				{
					gpEnemies[ i ].pSoldier->ubGroupID = 0;
					gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
					gpEnemies[ i ].pSoldier->sSectorX = SECTORX( pSectors[ ubCurrSI ] );
					gpEnemies[ i ].pSoldier->sSectorY = SECTORY( pSectors[ ubCurrSI ] );
					ubISNumTroops--;
					ubNumTroops--;
				}
				else if( gpEnemies[ i ].uiFlags & CELL_ADMIN && ubISNumAdmins && ubNumAdmins )
				{
					gpEnemies[ i ].pSoldier->ubGroupID = 0;
					gpEnemies[ i ].uiFlags |= CELL_ASSIGNED;
					gpEnemies[ i ].pSoldier->sSectorX = SECTORX( pSectors[ ubCurrSI ] );
					gpEnemies[ i ].pSoldier->sSectorY = SECTORY( pSectors[ ubCurrSI ] );
					ubISNumAdmins--;
					ubNumAdmins--;
				}
				}
			}
		}
	/*at this point, all enemies should have been assigned to their cell and group. If not, there is a bug around
	Because number and type of cells should be computed for the same composition of enemies as the one we see in this function, it should not happen though*/
	AssertMsg( !(ubISNumAdmins & ubISNumTroops & ubISNumElites & ubISNumTanks & ubISNumJeeps & ubISNumRobots), "Mapping between actual enemies and autoresolve cells is wrong." );

	}


void CalculateSoldierCells( BOOLEAN fReset )
{
	INT32 i, x, y;
	INT32 index, iStartY, iTop, gapStartRow;
	INT32 iMaxTeamSize;

	gpAR->ubAliveMercs = gpAR->ubMercs;
	gpAR->ubAliveCivs = gpAR->ubCivs;
	gpAR->ubAliveEnemies = gpAR->ubEnemies;

    // WDS - make number of mercenaries, etc. be configurable
	// WDS TO BE FIXED -- Why is this 40 here? (18 April 2008)
	//iMaxTeamSize = max( gpAR->ubMercs + gpAR->ubCivs, gpAR->ubEnemies );
	iMaxTeamSize = max( min( 40,	gpAR->ubMercs + gpAR->ubCivs ), min( 40,	gpAR->ubEnemies ) );
	
	if( iMaxTeamSize > 12 )
	{
		gpAR->ubTimeModifierPercentage = (UINT8)(118 - iMaxTeamSize*1.5);
	}
	else
	{
		gpAR->ubTimeModifierPercentage = 100;
	}

	gpAR->uiTimeSlice = gpAR->uiTimeSlice * gpAR->ubTimeModifierPercentage / 100;

	iTop = iScreenHeightOffset + (240 - gpAR->sHeight/2);
	if( iTop > (iScreenHeightOffset + 120) )
		iTop -= 40;

	if( gpAR->ubMercs )
	{
		iStartY = iTop + (gpAR->sHeight - (min(10,gpAR->ubMercRows+gpAR->ubCivRows)*47+7))/2 + 6;
		//iStartY = iTop + (gpAR->sHeight - ((gpAR->ubMercRows+gpAR->ubCivRows)*47+7))/2 + 6;
		y = gpAR->ubMercRows;
		x = gpAR->ubMercCols;
		i = gpAR->ubMercs;
		gapStartRow = gpAR->ubMercRows - gpAR->ubMercRows * gpAR->ubMercCols + gpAR->ubMercs;
		for( x = 0; x < gpAR->ubMercCols; x++ ) for( y = 0; i && y < gpAR->ubMercRows; y++, i-- )
		{
			index = y * gpAR->ubMercCols + gpAR->ubMercCols - x - 1;
			if( y >= gapStartRow )
				index -= y - gapStartRow + 1;
			Assert( index >= 0 && index < gpAR->ubMercs );

			gpMercs[ index ].xp = gpAR->sCenterStartX + 3 - 55*(x+1);
			gpMercs[ index ].yp = iStartY + y*47;
			gpMercs[ index ].uiFlags = CELL_MERC;
			if( AM_AN_EPC( gpMercs[ index ].pSoldier ) )
			{
				if( AM_A_ROBOT( gpMercs[ index ].pSoldier ) )
				{ //treat robot as a merc for the purpose of combat.
					gpMercs[ index ].uiFlags |= CELL_ROBOT;
				}
				else
				{
					gpMercs[ index ].uiFlags |= CELL_EPC;
				}
			}
			gpMercs[ index ].pRegion = (MOUSE_REGION*)MemAlloc( sizeof( MOUSE_REGION ) );
			Assert( gpMercs[ index ].pRegion );
			memset( gpMercs[ index ].pRegion, 0, sizeof( MOUSE_REGION ) );
			MSYS_DefineRegion( gpMercs[ index ].pRegion, gpMercs[ index ].xp, gpMercs[ index ].yp,
				(UINT16)(gpMercs[ index ].xp + 50), (UINT16)(gpMercs[ index ].yp + 44), MSYS_PRIORITY_HIGH, 0,
				MercCellMouseMoveCallback, MercCellMouseClickCallback );
			if( fReset )
				RefreshMerc( gpMercs[ index ].pSoldier );
			if( !gpMercs[ index ].pSoldier->stats.bLife )
				gpAR->ubAliveMercs--;
		}
	}

	if( gpAR->ubCivs )
	{
		iStartY = iTop + (gpAR->sHeight - (min(10,gpAR->ubMercRows+gpAR->ubCivRows)*47+7))/2 + gpAR->ubMercRows*47 + 5;
		y = gpAR->ubCivRows;
		x = gpAR->ubCivCols;
		i = gpAR->ubCivs;

		for( index = 0; index < gpAR->ubCivs ; ++index )
		{
			x = gpAR->ubCivCols - 1	- index % gpAR->ubCivCols;
			y = index / gpAR->ubCivCols;

			gpCivs[ index ].xp = gpAR->sCenterStartX + 3 - 55*(x+1);
			gpCivs[ index ].yp = iStartY + y*47;
			gpCivs[ index ].uiFlags |= CELL_MILITIA;
		}
	}

	if( gpAR->ubEnemies )
	{
		iStartY = iTop + (gpAR->sHeight - (min(10,gpAR->ubEnemyRows)*47+7))/2 + 5;
		y = gpAR->ubEnemyRows;
		x = gpAR->ubEnemyCols;
		i = gpAR->ubEnemies;

		for( index = 0; index < gpAR->ubEnemies ; ++index )
		{
			x = index % gpAR->ubEnemyCols;
			y = index / gpAR->ubEnemyCols;

			gpEnemies[ index ].xp = (UINT16)(gpAR->sCenterStartX + 141 + 55*x);
			gpEnemies[ index ].yp = iStartY + y*47;
			
			if ( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE )
			{
				if( index < gpAR->ubAFCreatures )
					gpEnemies[ index ].uiFlags = CELL_AF_CREATURE;
				else if( index < gpAR->ubAMCreatures + gpAR->ubAFCreatures )
					gpEnemies[ index ].uiFlags = CELL_AM_CREATURE;
				else if( index < gpAR->ubYFCreatures + gpAR->ubAMCreatures + gpAR->ubAFCreatures )
					gpEnemies[ index ].uiFlags = CELL_YF_CREATURE;
				else
					gpEnemies[ index ].uiFlags = CELL_YM_CREATURE;
			}
			else if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
				GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
				GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
			{
				if ( index < gpAR->ubBloodcats )
					gpEnemies[index].uiFlags = CELL_BLOODCAT;
				else if ( index < gpAR->ubBloodcats + gpAR->ubZombies )
					gpEnemies[index].uiFlags = CELL_ZOMBIE;
				else
					gpEnemies[index].uiFlags = CELL_BANDIT;
			}
			else
			{
				if ( index < gpAR->ubElites )
					gpEnemies[index].uiFlags = CELL_ELITE;
				else if ( index < gpAR->ubElites + gpAR->ubTroops )
					gpEnemies[index].uiFlags = CELL_TROOP;
				else if ( index < gpAR->ubElites + gpAR->ubTroops + gpAR->ubAdmins )
					gpEnemies[index].uiFlags = CELL_ADMIN;
				else if ( index < gpAR->ubElites + gpAR->ubTroops + gpAR->ubAdmins + gpAR->ubTanks )
					gpEnemies[index].uiFlags = CELL_TANK;
				else if ( index < gpAR->ubElites + gpAR->ubTroops + gpAR->ubAdmins + gpAR->ubTanks + gpAR->ubJeeps )
					gpEnemies[index].uiFlags = CELL_JEEP;
				else
					gpEnemies[index].uiFlags = CELL_ENEMYROBOT;
			}
		}
	}
}


INT32 DetermineCellID( SOLDIERCELL *pCell )
{
	INT32 iIndex;

	if( pCell->pSoldier->bTeam == ENEMY_TEAM )
	{
		for( iIndex = 0 ; iIndex < gpAR->ubEnemies ; iIndex++ )
			if(	&gpEnemies[ iIndex ] == pCell )
				return iIndex;
	}else if( pCell->pSoldier->bTeam == MILITIA_TEAM )
	{
		for( iIndex = 0 ; iIndex < gpAR->ubCivs ; iIndex++ )
			if(	&gpCivs[ iIndex ] == pCell )
				return iIndex;
	}

	return 0;
}

BOOLEAN IsItAllowedToRender( SOLDIERCELL *pCell )
{
	INT32 iID = DetermineCellID( pCell );

	switch( pCell->pSoldier->bTeam )
	{
	case ENEMY_TEAM:
		if( iID >= giMaxEnemiesToRender ) return FALSE;
		break;
	case MILITIA_TEAM:
		if( iID >= giMaxMilitiaToRender ) return FALSE;
		break;
	}
	return TRUE;
}


void RenderSoldierCell( SOLDIERCELL *pCell )
{
	UINT8 x;

	if( !IsItAllowedToRender( pCell ) ) return;

	if( pCell->uiFlags & CELL_MERC )
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+36, pCell->yp+2, pCell->xp+44,	pCell->yp+30, 0 );
		BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iPanelImages, MERC_PANEL, pCell->xp, pCell->yp, VO_BLT_SRCTRANSPARENCY, NULL );
		RenderSoldierCellBars( pCell );
		x = 0;
	}
	else
	{
		BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iPanelImages, OTHER_PANEL, pCell->xp, pCell->yp, VO_BLT_SRCTRANSPARENCY, NULL );
		x = 6;
	}
	if( !pCell->pSoldier->stats.bLife )
	{
		SetObjectHandleShade( pCell->uiVObjectID, 0 );
		
		if ( ( pCell->uiFlags & CELL_BLOODCAT ) )
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iFaces, BLOODCAT_SKULL, pCell->xp + 3 + x, pCell->yp + 3, VO_BLT_SRCTRANSPARENCY, NULL );
		else if ( (pCell->uiFlags & CELL_BUG ) )
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iFaces, CREATURE_SKULL, pCell->xp + 3 + x, pCell->yp + 3, VO_BLT_SRCTRANSPARENCY, NULL );
		else if ( (pCell->uiFlags & CELL_TANK) )
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iFaces, TANK_WRECK, pCell->xp + 3 + x, pCell->yp + 3, VO_BLT_SRCTRANSPARENCY, NULL );
		else if ( (pCell->uiFlags & CELL_JEEP) )
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iFaces, JEEP_WRECK, pCell->xp + 3 + x, pCell->yp + 3, VO_BLT_SRCTRANSPARENCY, NULL );
		else
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iFaces, HUMAN_SKULL, pCell->xp+3+x, pCell->yp+3, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	else
	{
		if( pCell->uiFlags & CELL_HITBYATTACKER )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+3+x, pCell->yp+3, pCell->xp+33+x,	pCell->yp+29, 65535 );
		}
		else if( pCell->uiFlags & CELL_HITLASTFRAME )
		{
			ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+3+x, pCell->yp+3, pCell->xp+33+x,	pCell->yp+29, 0 );
			SetObjectHandleShade( pCell->uiVObjectID, 1 );
			BltVideoObjectFromIndex( FRAME_BUFFER, pCell->uiVObjectID, pCell->usIndex, pCell->xp+3+x, pCell->yp+3, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			SetObjectHandleShade( pCell->uiVObjectID, 0 );
			BltVideoObjectFromIndex( FRAME_BUFFER, pCell->uiVObjectID, pCell->usIndex, pCell->xp+3+x, pCell->yp+3, VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}

	if( pCell->pSoldier->stats.bLife > 0 && pCell->pSoldier->stats.bLife < OKLIFE && !(pCell->uiFlags & (CELL_HITBYATTACKER|CELL_HITLASTFRAME|CELL_CREATURE)) )
	{ //Merc is unconcious (and not taking damage), so darken his portrait.
		UINT8 *pDestBuf;
		UINT32 uiDestPitchBYTES;
		SGPRect ClipRect;
		ClipRect.iLeft = pCell->xp+3+x;
		ClipRect.iTop = pCell->yp+3;
		ClipRect.iRight = pCell->xp+33+x;
		ClipRect.iBottom = pCell->yp+29;
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		Blt16BPPBufferShadowRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
		UnLockVideoSurface( FRAME_BUFFER );
	}

	//Draw the health text
	RenderSoldierCellHealth( pCell );

	DrawDebugText( pCell );

	if( !(pCell->uiFlags & CELL_RETREATING) )
		pCell->uiFlags &= ~CELL_DIRTY;

	InvalidateRegion( pCell->xp, pCell->yp, pCell->xp+50, pCell->yp+44 );

	//Adjust flags accordingly
	if( pCell->uiFlags & CELL_HITBYATTACKER )
	{
		pCell->uiFlags &= ~CELL_HITBYATTACKER;
		pCell->uiFlags |= CELL_HITLASTFRAME | CELL_DIRTY;
		pCell->uiFlashTime = GetJA2Clock() + 150;
	}
	else if( pCell->uiFlags & CELL_HITLASTFRAME )
	{
		if( pCell->uiFlashTime < GetJA2Clock() )
		{
			pCell->uiFlags &= ~CELL_HITLASTFRAME;
		}
		pCell->uiFlags |= CELL_DIRTY;
	}
}

void RenderSoldierCellBars( SOLDIERCELL *pCell )
{
	INT32 iStartY;
	//HEALTH BAR
	if( !pCell->pSoldier->stats.bLife )
		return;

	//yellow one for bleeding
	iStartY = pCell->yp + 29 - 25*pCell->pSoldier->stats.bLifeMax/100;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+37, iStartY, pCell->xp+38, pCell->yp+29, Get16BPPColor( FROMRGB( 107, 107, 57 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+38, iStartY, pCell->xp+39, pCell->yp+29, Get16BPPColor( FROMRGB( 222, 181, 115 ) ) );
	
	//pink one for bandaged.
	iStartY = pCell->yp + 29 - 25*(pCell->pSoldier->stats.bLifeMax - pCell->pSoldier->bBleeding)/100;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+37, iStartY, pCell->xp+38, pCell->yp+29, Get16BPPColor( FROMRGB( 156, 57, 57 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+38, iStartY, pCell->xp+39, pCell->yp+29, Get16BPPColor( FROMRGB( 222, 132, 132 ) ) );
		
	//red one for actual health
	iStartY = pCell->yp + 29 - 25*pCell->pSoldier->stats.bLife/100;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+37, iStartY, pCell->xp+38, pCell->yp+29, Get16BPPColor( FROMRGB( 107, 8, 8 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+38, iStartY, pCell->xp+39, pCell->yp+29, Get16BPPColor( FROMRGB( 206, 0, 0 ) ) );

	//BREATH BAR
	iStartY = pCell->yp + 29 - 25*pCell->pSoldier->bBreathMax/100;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+41, iStartY, pCell->xp+42, pCell->yp+29, Get16BPPColor( FROMRGB( 8, 8, 132 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+42, iStartY, pCell->xp+43, pCell->yp+29, Get16BPPColor( FROMRGB( 8, 8, 107 ) ) );

	//MORALE BAR
	iStartY = pCell->yp + 29 - 25*pCell->pSoldier->aiData.bMorale/100;
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+45, iStartY, pCell->xp+46, pCell->yp+29, Get16BPPColor( FROMRGB( 8, 156, 8 ) ) );
	ColorFillVideoSurfaceArea( FRAME_BUFFER, pCell->xp+46, iStartY, pCell->xp+47, pCell->yp+29, Get16BPPColor( FROMRGB( 8, 107, 8 ) ) );
}

void BuildInterfaceBuffer()
{
	VSURFACE_DESC		vs_desc;
	UINT16					usUselessWidth, usUselessHeight;
	UINT8						ubBitDepth;
	SGPRect					ClipRect;
	SGPRect					DestRect;
	INT32						x,y;

	//Setup the blitting clip regions, so we don't draw outside of the region (for excess panelling)
	gpAR->Rect.iLeft		= iScreenWidthOffset + (320 - gpAR->sWidth/2);
	gpAR->Rect.iRight		= gpAR->Rect.iLeft + gpAR->sWidth;
	gpAR->Rect.iTop			= iScreenHeightOffset + (240 - gpAR->sHeight/2);
	if( gpAR->Rect.iTop > (iScreenHeightOffset + 120) )
		gpAR->Rect.iTop -= 40;
	gpAR->Rect.iBottom	= gpAR->Rect.iTop + gpAR->sHeight;

	DestRect.iLeft			= 0;
	DestRect.iTop			= 0;
	DestRect.iRight			= gpAR->sWidth;
	DestRect.iBottom		= gpAR->sHeight;

	//create buffer for the transition slot for merc items.	This slot contains the newly
	//selected item graphic in it's inventory size version.	This buffer is then scaled down
	//into the associated merc inventory panel slot buffer which is approximately 20% smaller.
	GetCurrentVideoSettings( &usUselessWidth, &usUselessHeight, &ubBitDepth );
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = gpAR->sWidth;
	vs_desc.usHeight = gpAR->sHeight;
	vs_desc.ubBitDepth = ubBitDepth;
	if( !AddVideoSurface( &vs_desc, (UINT32 *)&gpAR->iInterfaceBuffer ) )
		AssertMsg( 0, "Failed to allocate memory for autoresolve interface buffer." );

	GetClippingRect( &ClipRect );
	SetClippingRect( &DestRect );

	//Blit the back panels...
	for( y = DestRect.iTop; y < DestRect.iBottom; y += 40 )
	{
		for( x = DestRect.iLeft; x < DestRect.iRight; x += 50 )
		{
			BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, C_TEXTURE, x, y, VO_BLT_SRCTRANSPARENCY, 0 );
		}
	}
	//Blit the left and right edges
	for( y = DestRect.iTop; y < DestRect.iBottom; y += 40 )
	{
		x = DestRect.iLeft;
		BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, L_BORDER, x, y, VO_BLT_SRCTRANSPARENCY, 0 );
		x = DestRect.iRight - 3;
		BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, R_BORDER, x, y, VO_BLT_SRCTRANSPARENCY, 0 );
	}
	//Blit the top and bottom edges
	for( x = DestRect.iLeft; x < DestRect.iRight; x += 50 )
	{
		y = DestRect.iTop;
		BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, T_BORDER, x, y, VO_BLT_SRCTRANSPARENCY, 0 );
		y = DestRect.iBottom - 3;
		BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, B_BORDER, x, y, VO_BLT_SRCTRANSPARENCY, 0 );
	}
	//Blit the 4 corners
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, TL_BORDER, DestRect.iLeft, DestRect.iTop, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, TR_BORDER, DestRect.iRight-10, DestRect.iTop, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, BL_BORDER, DestRect.iLeft, DestRect.iBottom-9, VO_BLT_SRCTRANSPARENCY, NULL );
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, BR_BORDER, DestRect.iRight-10, DestRect.iBottom-9, VO_BLT_SRCTRANSPARENCY, NULL );

	//Blit the center pieces
	x = gpAR->sCenterStartX - gpAR->Rect.iLeft;
	y = 0;
	//Top
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, TOP_MIDDLE, x, y, VO_BLT_SRCTRANSPARENCY, NULL );
	//Middle
	for( y = 40; y < gpAR->sHeight - 40; y += 40 )
	{
		BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, AUTO_MIDDLE, x, y, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	y = gpAR->sHeight - 40;
	BltVideoObjectFromIndex( gpAR->iInterfaceBuffer, gpAR->iPanelImages, BOT_MIDDLE, x, y, VO_BLT_SRCTRANSPARENCY, NULL );

	SetClippingRect( &ClipRect );
}

void ExpandWindow()
{
	SGPRect OldRect;
	UINT32	uiDestPitchBYTES;
	UINT32 uiCurrentTime, uiTimeRange, uiPercent;
	UINT8		*pDestBuf;
	INT32 i;

	if( !gpAR->ExRect.iLeft && !gpAR->ExRect.iRight )
	{ //First time
		gpAR->ExRect.iLeft = ORIG_LEFT;
		gpAR->ExRect.iTop = ORIG_TOP;
		gpAR->ExRect.iRight = ORIG_RIGHT;
		gpAR->ExRect.iBottom = ORIG_BOTTOM;
		gpAR->uiStartExpanding = GetJA2Clock();
		gpAR->uiEndExpanding = gpAR->uiStartExpanding + 333;
		for( i = 0; i < DONEWIN_BUTTON; i++ )
			HideButton( gpAR->iButton[ i ] );
	}
	else
	{
		//Restore the previous area
		//left
		BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, (UINT16)gpAR->ExRect.iLeft, (UINT16)gpAR->ExRect.iTop, 1, (UINT16)(gpAR->ExRect.iBottom-gpAR->ExRect.iTop+1) );
		InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iTop, gpAR->ExRect.iLeft+1, gpAR->ExRect.iBottom+1 );
		//right
		BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, (UINT16)gpAR->ExRect.iRight, (UINT16)gpAR->ExRect.iTop, 1, (UINT16)(gpAR->ExRect.iBottom-gpAR->ExRect.iTop+1) );
		InvalidateRegion( gpAR->ExRect.iRight, gpAR->ExRect.iTop, gpAR->ExRect.iRight+1, gpAR->ExRect.iBottom+1 );
		//top
		BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, (UINT16)gpAR->ExRect.iLeft, (UINT16)gpAR->ExRect.iTop, (UINT16)(gpAR->ExRect.iRight-gpAR->ExRect.iLeft+1), 1 );
		InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iTop, gpAR->ExRect.iRight+1, gpAR->ExRect.iTop+1 );
		//bottom
		BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, (UINT16)gpAR->ExRect.iLeft, (UINT16)gpAR->ExRect.iBottom, (UINT16)(gpAR->ExRect.iRight-gpAR->ExRect.iLeft+1), 1 );
		InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iBottom, gpAR->ExRect.iRight+1, gpAR->ExRect.iBottom+1 );

		uiCurrentTime = GetJA2Clock();
		if( uiCurrentTime >= gpAR->uiStartExpanding && uiCurrentTime <= gpAR->uiEndExpanding )
		{
			//Debug purposes
			OldRect.iLeft = ORIG_LEFT;
			OldRect.iTop = ORIG_TOP;
			OldRect.iRight = ORIG_RIGHT;
			OldRect.iBottom = ORIG_BOTTOM;

			uiTimeRange = gpAR->uiEndExpanding - gpAR->uiStartExpanding;
			uiPercent = (uiCurrentTime - gpAR->uiStartExpanding ) * 100 / uiTimeRange;

			//Left
			if( OldRect.iLeft <= gpAR->Rect.iLeft )
				gpAR->ExRect.iLeft = OldRect.iLeft + ( gpAR->Rect.iLeft - OldRect.iLeft ) * uiPercent / 100;
			else
				gpAR->ExRect.iLeft = gpAR->Rect.iLeft + ( OldRect.iLeft - gpAR->Rect.iLeft ) * uiPercent / 100;
			//Top
			if( OldRect.iTop <= gpAR->Rect.iTop )
				gpAR->ExRect.iTop = OldRect.iTop + ( gpAR->Rect.iTop - OldRect.iTop ) * uiPercent / 100;
			else
				gpAR->ExRect.iTop = gpAR->Rect.iTop + ( OldRect.iTop - gpAR->Rect.iTop ) * uiPercent / 100;
			//Right
			if( OldRect.iRight <= gpAR->Rect.iRight )
				gpAR->ExRect.iRight = OldRect.iRight + ( gpAR->Rect.iRight - OldRect.iRight ) * uiPercent / 100;
			else
				gpAR->ExRect.iRight = gpAR->Rect.iRight + ( OldRect.iRight - gpAR->Rect.iRight ) * uiPercent / 100;
			//Bottom
			if( OldRect.iBottom <= gpAR->Rect.iBottom )
				gpAR->ExRect.iBottom = OldRect.iBottom + ( gpAR->Rect.iBottom - OldRect.iBottom ) * uiPercent / 100;
			else
				gpAR->ExRect.iBottom = gpAR->Rect.iBottom + ( OldRect.iBottom - gpAR->Rect.iBottom ) * uiPercent / 100;
		}
		else
		{ //expansion done -- final frame
			gpAR->ExRect.iLeft		= gpAR->Rect.iLeft;
			gpAR->ExRect.iTop			= gpAR->Rect.iTop;
			gpAR->ExRect.iRight		= gpAR->Rect.iRight;
			gpAR->ExRect.iBottom	= gpAR->Rect.iBottom;
			gpAR->fExpanding = FALSE;
			gpAR->fShowInterface = TRUE;
		}
	}

	//The new rect now determines the state of the current rectangle.
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0 + xResOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	RectangleDraw( TRUE, gpAR->ExRect.iLeft, gpAR->ExRect.iTop, gpAR->ExRect.iRight, gpAR->ExRect.iBottom, Get16BPPColor( FROMRGB( 200, 200, 100 ) ), pDestBuf );
	UnLockVideoSurface( FRAME_BUFFER );
	//left
	InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iTop, gpAR->ExRect.iLeft+1, gpAR->ExRect.iBottom+1 );
	//right
	InvalidateRegion( gpAR->ExRect.iRight, gpAR->ExRect.iTop, gpAR->ExRect.iRight+1, gpAR->ExRect.iBottom+1 );
	//top
	InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iTop, gpAR->ExRect.iRight+1, gpAR->ExRect.iTop+1 );
	//bottom
	InvalidateRegion( gpAR->ExRect.iLeft, gpAR->ExRect.iBottom, gpAR->ExRect.iRight+1, gpAR->ExRect.iBottom+1 );

}

OBJECTTYPE* FindMedicalKit()
{
	INT32 i;
	INT32 iSlot;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		iSlot = FindObjClass( gpMercs[ i ].pSoldier, IC_MEDKIT );
		if( iSlot != NO_SLOT )
		{
			return( &gpMercs[ i ].pSoldier->inv[ iSlot ] );
		}
	}
	return NULL;
}

UINT32 AutoBandageMercs()
{
	INT32 i, iBest;
	UINT32 uiPointsUsed, uiCurrPointsUsed, uiMaxPointsUsed, uiParallelPointsUsed;
	UINT16 usKitPts;
	OBJECTTYPE *pKit = NULL;
	BOOLEAN fFound = FALSE;
	BOOLEAN fComplete = TRUE;
	INT8 bSlot, cnt;

	//Do we have any doctors?	If so, bandage selves first.
	fFound = FALSE;
	uiMaxPointsUsed = uiParallelPointsUsed = 0;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		if( gpMercs[ i ].pSoldier->stats.bLife >= OKLIFE &&
			!gpMercs[ i ].pSoldier->bCollapsed &&
				gpMercs[ i ].pSoldier->stats.bMedical > 0 &&
				( bSlot = FindObjClass( gpMercs[ i ].pSoldier, IC_MEDKIT ) ) != NO_SLOT )
		{
			fFound = TRUE;
			//bandage self first!
			uiCurrPointsUsed = 0;
			cnt = 0;
			while( gpMercs[ i ].pSoldier->bBleeding )
			{
				pKit = &gpMercs[ i ].pSoldier->inv[ bSlot ];
				usKitPts = TotalPoints( pKit );
				if( !usKitPts )
				{ //attempt to find another kit before stopping
					if( ( bSlot = FindObjClass( gpMercs[ i ].pSoldier, IC_MEDKIT ) ) != NO_SLOT )
					continue;
					break;
				}
				uiPointsUsed = VirtualSoldierDressWound( gpMercs[ i ].pSoldier, gpMercs[ i ].pSoldier, pKit, usKitPts, usKitPts, FALSE ); // changed by SANDRO
				UseKitPoints( pKit, (UINT16)uiPointsUsed, gpMercs[ i ].pSoldier );
				uiCurrPointsUsed += uiPointsUsed;
				cnt++;
				if( cnt > 50 )
					break;
			}
			if( uiCurrPointsUsed > uiMaxPointsUsed )
				uiMaxPointsUsed = uiCurrPointsUsed;
			if( !pKit )
				break;
		}
	}


	//Find the best rated doctor to do all of the bandaging.
	iBest = 0;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		if( gpMercs[ i ].pSoldier->stats.bLife >= OKLIFE && !gpMercs[ i ].pSoldier->bCollapsed && gpMercs[ i ].pSoldier->stats.bMedical > 0 )
		{
			if( gpMercs[ i ].pSoldier->stats.bMedical > gpMercs[ iBest ].pSoldier->stats.bMedical )
			{
				iBest = i;
			}			
		}
	}

	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		cnt = 0; // SANDRO - added safety check
		while( gpMercs[ i ].pSoldier->bBleeding && gpMercs[ i ].pSoldier->stats.bLife )
		{ //This merc needs medical attention
			if( !pKit )
			{
				pKit = FindMedicalKit();
				if( !pKit )
				{
					fComplete = FALSE;
					break;
				}
			}
			usKitPts = TotalPoints( pKit );
			if( !usKitPts )
			{
				pKit = NULL;
				fComplete = FALSE;
				continue;
			}
			uiPointsUsed = VirtualSoldierDressWound( gpMercs[ iBest ].pSoldier, gpMercs[ i ].pSoldier, pKit, usKitPts, usKitPts, FALSE );
			UseKitPoints( pKit, (UINT16)uiPointsUsed, gpMercs[ i ].pSoldier );
			uiParallelPointsUsed += uiPointsUsed;
			fComplete = TRUE;

			// SANDRO - added safety check
			cnt++;
			// sevenfm: INT8 cannot be larger than 127, should be 50 probably as in other check here
			//if( cnt > 500 )
			if (cnt > 50)
				break;
		}
	}
	if( fComplete )
	{
		DoScreenIndependantMessageBox( gzLateLocalizedString[ 13 ], MSG_BOX_FLAG_OK, AutoBandageFinishedCallback );
	}
	else
	{
		DoScreenIndependantMessageBox( gzLateLocalizedString[ 10 ], MSG_BOX_FLAG_OK, AutoBandageFinishedCallback );
	}

	gpAR->uiTotalElapsedBattleTimeInMilliseconds += uiParallelPointsUsed * 200;
	return 1;
}

void RenderAutoResolve()
{
	INT32 i;
	HVSURFACE hVSurface;
	INT32 xp, yp;
	CHAR16 str[100];
	UINT8 ubGood, ubBad;

	if( gpAR->fExpanding )
	{ //animate the expansion of the window.
		ExpandWindow();
		return;
	}
	else if( gpAR->fShowInterface )
	{ //After expanding the window, we now show the interface
		if( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS && !gpAR->fPendingSurrender )
		{
			for( i = 0 ; i < DONEWIN_BUTTON; i++ )
				ShowButton( gpAR->iButton[ i ] );
			HideButton( gpAR->iButton[ BANDAGE_BUTTON ] );
			HideButton( gpAR->iButton[ YES_BUTTON ] );
			HideButton( gpAR->iButton[ NO_BUTTON ] );
			gpAR->fShowInterface = FALSE;
		}
		else if( gpAR->ubBattleStatus == BATTLE_VICTORY )
		{
			ShowButton( gpAR->iButton[ DONEWIN_BUTTON ] );
			ShowButton( gpAR->iButton[ BANDAGE_BUTTON ] );
		}
		else
		{
			ShowButton( gpAR->iButton[ DONELOSE_BUTTON ] );
		}
	}

	if( !gpAR->fRenderAutoResolve && !gpAR->fDebugInfo )
	{ //update the dirty cells only
		for( i = 0; i < gpAR->ubMercs; i++ )
		{
			if( gpMercs[ i ].uiFlags & CELL_DIRTY )
			{
				RenderSoldierCell( &gpMercs[ i ] );
			}
		}
		for( i = 0; i < gpAR->ubCivs; i++ )
		{
			if( gpCivs[ i ].uiFlags & CELL_DIRTY )
			{
				RenderSoldierCell( &gpCivs[ i ] );
			}
		}
		for( i = 0; i < gpAR->ubEnemies; i++ )
		{
			if( gpEnemies[ i ].uiFlags & CELL_DIRTY )
			{
				RenderSoldierCell( &gpEnemies[ i ] );
			}
		}
		return;
	}
	gpAR->fRenderAutoResolve = FALSE;

	GetVideoSurface( &hVSurface, gpAR->iInterfaceBuffer );
	BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, gpAR->Rect.iLeft, gpAR->Rect.iTop, VO_BLT_SRCTRANSPARENCY, 0 );

	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		RenderSoldierCell( &gpMercs[ i ] );
	}
	for( i = 0; i < gpAR->ubCivs; i++ )
	{
		RenderSoldierCell( &gpCivs[ i ] );
	}
	for( i = 0; i < gpAR->ubEnemies; i++ )
	{
		RenderSoldierCell( &gpEnemies[ i ] );
	}

	//Render the titles
	SetFont( FONT10ARIALBOLD );
	SetFontForeground( FONT_WHITE );
	SetFontShadow( FONT_NEARBLACK );

	switch( GetEnemyEncounterCode() )
	{
		case NO_ENCOUNTER_CODE:
			swprintf( str, gpStrategicString[STR_AR_ATTACK_HEADER] );
			break;
		case ENEMY_ENCOUNTER_CODE:
			swprintf( str, gpStrategicString[STR_AR_ENCOUNTER_HEADER] );
			break;
		case ENEMY_INVASION_CODE:
		case CREATURE_ATTACK_CODE:
		case BLOODCAT_ATTACK_CODE:
		case ZOMBIE_ATTACK_CODE:
		case BANDIT_ATTACK_CODE:
			swprintf( str, gpStrategicString[STR_AR_DEFEND_HEADER] );
			break;
		default:
			swprintf( str, gpStrategicString[STR_AR_ATTACK_HEADER] );
			break;
	}

	xp = gpAR->sCenterStartX + 70 - StringPixLength( str, FONT10ARIALBOLD )/2;
	yp = gpAR->Rect.iTop + 15;
	mprintf( xp, yp, str );

	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_GRAY2 );
	SetFontShadow( FONT_NEARBLACK );

	GetSectorIDString( gpAR->ubSectorX, gpAR->ubSectorY, 0, str, TRUE );
	xp = gpAR->sCenterStartX + 70 - StringPixLength( str, FONT10ARIAL )/2;
	yp += 11;
	mprintf( xp, yp, str );

	//Display the remaining forces
	ubGood = (UINT8)(gpAR->ubAliveMercs + gpAR->ubAliveCivs);
	ubBad = gpAR->ubAliveEnemies;
	swprintf( str, gzLateLocalizedString[ 17 ], ubGood, ubBad );

	SetFont( FONT14ARIAL );
	if( ubGood * 3 <= ubBad * 2 )
	{
		SetFontForeground( FONT_LTRED );
	}
	else if( ubGood * 2 >= ubBad * 3 )
	{
		SetFontForeground( FONT_LTGREEN );
	}
	else
	{
		SetFontForeground( FONT_YELLOW );
	}

	xp = gpAR->sCenterStartX + 70 - StringPixLength( str, FONT14ARIAL )/2;
	yp += 11;
	mprintf( xp, yp, str );

	#ifdef JA2BETAVERSION
		if( gpAR->fAllowCapture )
		{
			mprintf( 2, 2, L"Enemy capture enabled." );
		}
	#endif

	if( gpAR->fPendingSurrender )
	{
		DisplayWrappedString( (UINT16)(gpAR->sCenterStartX+16), (UINT16)(iScreenHeightOffset + 230+gpAR->bVerticalOffset), 108, 2,
			(UINT8)FONT10ARIAL, FONT_YELLOW, gpStrategicString[ STR_ENEMY_SURRENDER_OFFER ], FONT_BLACK, FALSE, LEFT_JUSTIFIED );
	}

	if( gpAR->ubBattleStatus != BATTLE_IN_PROGRESS )
	{
		// Handle merc morale, Global loyalty, and change of sector control
		if( !gpAR->fMoraleEventsHandled )
		{
			gpAR->uiTotalElapsedBattleTimeInMilliseconds *= 3;
			gpAR->fMoraleEventsHandled = TRUE;
			if ( CheckFact( FACT_FIRST_BATTLE_FOUGHT, 0 ) == FALSE )
			{
				// this was the first battle against the army
				SetFactTrue( FACT_FIRST_BATTLE_FOUGHT );
				if ( gpAR->ubBattleStatus == BATTLE_VICTORY)
				{
					SetFactTrue( FACT_FIRST_BATTLE_WON );
				}
				SetTheFirstBattleSector( ( INT16 ) (gpAR->ubSectorX + gpAR->ubSectorY * MAP_WORLD_X ) );
#ifdef JA2UB
//Ja25:	no loyalty
#else
				HandleFirstBattleEndingWhileInTown( gpAR->ubSectorX, gpAR->ubSectorY, 0, TRUE );
#endif
			}

			switch( gpAR->ubBattleStatus )
			{
				case BATTLE_VICTORY:
					HandleMoraleEvent( NULL, MORALE_BATTLE_WON, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
					if( ProcessLoyalty() )HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_BATTLE_WON, gpAR->ubSectorX, gpAR->ubSectorY, 0 );

					SectorInfo[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].bLastKnownEnemies = 0;
					SetThisSectorAsPlayerControlled( gpAR->ubSectorX, gpAR->ubSectorY, 0, TRUE );
					#ifdef NEWMUSIC
					GlobalSoundID  = MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalVictory[0];
					if ( MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalVictory[0] != -1 )
						SetMusicModeID( MUSIC_TACTICAL_VICTORY, MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalVictory[0] );
					else
					#endif
					SetMusicMode( MUSIC_TACTICAL_VICTORY );
					
					LogBattleResults( LOG_VICTORY );
					break;

				case BATTLE_SURRENDERED:
				case BATTLE_CAPTURED:
					for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
					{
						if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( MercPtrs[ i ] ) )
						{ //Merc is active and alive, and not a vehicle or robot
							if ( PlayerMercInvolvedInThisCombat( MercPtrs[ i ] ) )
							{
								// This morale event is PER INDIVIDUAL SOLDIER
								HandleMoraleEvent( MercPtrs[ i ], MORALE_MERC_CAPTURED, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
							}
						}
					}
					HandleMoraleEvent( NULL, MORALE_HEARD_BATTLE_LOST, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
					if( ProcessLoyalty() )HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_BATTLE_LOST, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
					#ifdef NEWMUSIC
					GlobalSoundID  = MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0];
					if ( MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0] != -1 )
						SetMusicModeID( MUSIC_TACTICAL_DEATH, MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0] );
					else
					#endif
					SetMusicMode( MUSIC_TACTICAL_DEATH );

					gsEnemyGainedControlOfSectorID = (INT16)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
					break;
				case BATTLE_DEFEAT:
					HandleMoraleEvent( NULL, MORALE_HEARD_BATTLE_LOST, gpAR->ubSectorX, gpAR->ubSectorY, 0 );

					if( ProcessLoyalty() )
						HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_BATTLE_LOST, gpAR->ubSectorX, gpAR->ubSectorY, 0 );

					if ( GetEnemyEncounterCode() != CREATURE_ATTACK_CODE &&
						GetEnemyEncounterCode () != BLOODCAT_ATTACK_CODE &&
						GetEnemyEncounterCode () != ZOMBIE_ATTACK_CODE &&
						GetEnemyEncounterCode () != BANDIT_ATTACK_CODE )
					{
						gsEnemyGainedControlOfSectorID = (INT16)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
					}
					else
					{
						gsEnemyGainedControlOfSectorID = (INT16)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
						gsCiviliansEatenByMonsters = gpAR->ubAliveEnemies;
					}
					#ifdef NEWMUSIC
					GlobalSoundID  = MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0];
					if ( MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0] != -1 )
						SetMusicModeID( MUSIC_TACTICAL_DEATH, MusicSoundValues[ SECTOR( gpAR->ubSectorX, gpAR->ubSectorY ) ].SoundTacticalDeath[0] );
					else
					#endif
					SetMusicMode( MUSIC_TACTICAL_DEATH );
						
					LogBattleResults( LOG_DEFEAT );
					break;

				case BATTLE_RETREAT:

					//Tack on 5 minutes for retreat.
					gpAR->uiTotalElapsedBattleTimeInMilliseconds += 300000;

					if( ProcessLoyalty() )
						HandleLoyaltyImplicationsOfMercRetreat( RETREAT_AUTORESOLVE, gpAR->ubSectorX, gpAR->ubSectorY, 0 );

					if ( GetEnemyEncounterCode() != CREATURE_ATTACK_CODE && 
						GetEnemyEncounterCode () != BLOODCAT_ATTACK_CODE &&
						GetEnemyEncounterCode () != ZOMBIE_ATTACK_CODE &&
						GetEnemyEncounterCode () != BANDIT_ATTACK_CODE )
					{
						gsEnemyGainedControlOfSectorID = (INT16)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
					}
					else if( gpAR->ubAliveEnemies )
					{
						gsEnemyGainedControlOfSectorID = (INT16)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
						gsCiviliansEatenByMonsters = gpAR->ubAliveEnemies;
					}
					LogBattleResults( LOG_DEFEAT );
					break;
			}
		}
		//Render the end battle condition.
			switch( gpAR->ubBattleStatus )
			{
				case BATTLE_VICTORY:
					SetFontForeground( FONT_LTGREEN );
					swprintf( str, gpStrategicString[ STR_AR_OVER_VICTORY ] );
					break;
				case BATTLE_SURRENDERED:
				case BATTLE_CAPTURED:
					if( gpAR->ubBattleStatus == BATTLE_SURRENDERED )
					{
						swprintf( str, gpStrategicString[ STR_AR_OVER_SURRENDERED ] );
					}
					else
					{
						DisplayWrappedString( (UINT16)(gpAR->sCenterStartX+16), iScreenHeightOffset + 310, 108, 2,
							FONT10ARIAL, FONT_YELLOW, gpStrategicString[ STR_ENEMY_CAPTURED ], FONT_BLACK, FALSE, LEFT_JUSTIFIED );
						swprintf( str, gpStrategicString[ STR_AR_OVER_CAPTURED ] );
					}
					SetFontForeground( FONT_RED );
					break;
				case BATTLE_DEFEAT:
					SetFontForeground( FONT_RED );
					swprintf( str, gpStrategicString[ STR_AR_OVER_DEFEAT ] );
					break;
				case BATTLE_RETREAT:
					SetFontForeground( FONT_YELLOW );
					swprintf( str, gpStrategicString[ STR_AR_OVER_RETREATED ] );
					break;
			}
			//Render the results of the battle.
			SetFont( BLOCKFONT2 );
			xp = gpAR->sCenterStartX + 12;
			yp = iScreenHeightOffset + 218 + gpAR->bVerticalOffset;
			BltVideoObjectFromIndex( FRAME_BUFFER, gpAR->iIndent, 0, xp, yp, VO_BLT_SRCTRANSPARENCY, NULL );
			xp = gpAR->sCenterStartX + 70 - StringPixLength( str, BLOCKFONT2 )/2;
			yp = iScreenHeightOffset + 227 + gpAR->bVerticalOffset;
			mprintf( xp, yp, str );

			//Render the total battle time elapsed.
			SetFont( FONT10ARIAL );
			swprintf( str, L"%s: %dm %02ds",
				gpStrategicString[ STR_AR_TIME_ELAPSED ],
				gpAR->uiTotalElapsedBattleTimeInMilliseconds/60000,
				(gpAR->uiTotalElapsedBattleTimeInMilliseconds%60000)/1000 );
			xp = gpAR->sCenterStartX + 70 - StringPixLength( str, FONT10ARIAL )/2;
			yp = iScreenHeightOffset + 290 + gpAR->bVerticalOffset;
			SetFontForeground( FONT_YELLOW );
			mprintf( xp, yp, str );

		// Flugente: in any case, reset creature attack variables
		ResetCreatureAttackVariables();
	}

	MarkButtonsDirty();
	InvalidateScreen();
}

static void ARCreateMilitia( UINT8 mclass, INT32 i, INT16 sX, INT16 sY)
{
	// reset counter of how many mortars this team has rolled
	ResetMortarsOnTeamCount();
	ResetNumSquadleadersInArmyGroup(); // added by SANDRO

	if( !gpBattleGroup ) {
		//AssertMsg(0, "No battle group set while creating militia");
	}

	if( mclass == SOLDIER_CLASS_ELITE_MILITIA )
	{
		gpCivs[i].pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_ELITE_MILITIA, sX, sY );
		if( gpCivs[i].pSoldier->ubBodyType == REGFEMALE )
		{
			gpCivs[i].usIndex = MILITIA3F_FACE;
		}
		else
		{
			gpCivs[i].usIndex = MILITIA3_FACE;
		}
	}
	else if( mclass == SOLDIER_CLASS_REG_MILITIA )
	{
		gpCivs[i].pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_REG_MILITIA, sX, sY );
		if( gpCivs[i].pSoldier->ubBodyType == REGFEMALE )
		{
			gpCivs[i].usIndex = MILITIA2F_FACE;
		}
		else
		{
			gpCivs[i].usIndex = MILITIA2_FACE;
		}
	}
	else if( mclass == SOLDIER_CLASS_GREEN_MILITIA )
	{
		gpCivs[i].pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_GREEN_MILITIA, sX, sY );
		if( gpCivs[i].pSoldier->ubBodyType == REGFEMALE )
		{
			gpCivs[i].usIndex = MILITIA1F_FACE;
		}
		else
		{
			gpCivs[i].usIndex = MILITIA1_FACE;
		}
	}
	else
	{
		AssertMsg( 0, "Attempting to illegally create a militia soldier." );
	}
	if( !gpCivs[ i ].pSoldier )
	{
		AssertMsg( 0, "Failed to create militia soldier for autoresolve." );
	}
	gpCivs[i].uiVObjectID = gpAR->iFaces;
	gpCivs[i].pSoldier->sSectorX = sX;
	gpCivs[i].pSoldier->sSectorY = sY;
	swprintf( gpCivs[i].pSoldier->name, gpStrategicString[ STR_AR_MILITIA_NAME ] );
}

static void ARCreateMilitiaSquad( UINT8 *cnt, UINT8 ubEliteMilitia, UINT8 ubRegMilitia, UINT8 ubGreenMilitia, INT16 sX, INT16 sY)
{
	while( *cnt < gpAR->ubCivs && (ubEliteMilitia || ubRegMilitia || ubGreenMilitia) )
	{
		if (ubEliteMilitia)
		{
			ARCreateMilitia( SOLDIER_CLASS_ELITE_MILITIA, *cnt, sX, sY);
			ubEliteMilitia--;
		}
		else if (ubRegMilitia)
		{
			ARCreateMilitia( SOLDIER_CLASS_REG_MILITIA, *cnt, sX, sY);
			ubRegMilitia--;
		}
		else if (ubGreenMilitia)
		{
			ARCreateMilitia( SOLDIER_CLASS_GREEN_MILITIA, *cnt, sX, sY);
			ubGreenMilitia--;
		}

		(*cnt)++;
	}
}

void CreateAutoResolveInterface()
{
	VOBJECT_DESC	VObjectDesc;
	INT32 i, index;
	HVOBJECT hVObject;
	UINT8 ubGreenMilitia, ubRegMilitia, ubEliteMilitia;
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT8 cnt;

	//Setup new autoresolve blanket interface.
	MSYS_DefineRegion( &gpAR->AutoResolveRegion, 0 + xResOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGH-1, 0,
		MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	gpAR->fRenderAutoResolve = TRUE;
	gpAR->fExitAutoResolve = FALSE;

	//Load the general panel image pieces, to be combined to make the dynamically sized window.
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	sprintf( VObjectDesc.ImageFile, "Interface\\AutoResolve.sti" );
	if( !AddVideoObject( &VObjectDesc, (UINT32 *)&gpAR->iPanelImages ) )
	{
		AssertMsg( 0, "Failed to load Interface\\AutoResolve.sti" );
	}

	//Load the button images file, and assign it to the first button.
	/* OLD BEFORE THE MEDICAL BUTTON WAS ADDED
	gpAR->iButtonImage[ PAUSE_BUTTON ] = LoadButtonImage( "Interface\\AutoBtns.sti", -1, 0, -1, 6, -1 );
	if( gpAR->iButtonImage[ PAUSE_BUTTON ] == -1 )
	{
		AssertMsg( 0, "Failed to load Interface\\AutoBtns.sti" );
	}

	//Have the other buttons hook into the first button containing the images.
	gpAR->iButtonImage[ PLAY_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 1, -1, 7, -1 );
	gpAR->iButtonImage[ FAST_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 2, -1, 8, -1 );
	gpAR->iButtonImage[ FINISH_BUTTON ]		= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 3, -1, 9, -1 );
	gpAR->iButtonImage[ YES_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 4, -1, 10, -1 );
	gpAR->iButtonImage[ NO_BUTTON ]				= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 5, -1, 11, -1 );
	gpAR->iButtonImage[ RETREAT_BUTTON ]	= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 12, -1, 13, -1 );
	gpAR->iButtonImage[ DONE_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 14, -1, 15, -1 );
	*/
	gpAR->iButtonImage[ PAUSE_BUTTON ] = LoadButtonImage( "Interface\\AutoBtns.sti", -1, 0, -1, 7, -1 );
	if( gpAR->iButtonImage[ PAUSE_BUTTON ] == -1 )
	{
		AssertMsg( 0, "Failed to load Interface\\AutoBtns.sti" );
	}

	//Have the other buttons hook into the first button containing the images.
	gpAR->iButtonImage[ PLAY_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 1, -1, 8, -1 );
	gpAR->iButtonImage[ FAST_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 2, -1, 9, -1 );
	gpAR->iButtonImage[ FINISH_BUTTON ]		= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 3, -1, 10, -1 );
	gpAR->iButtonImage[ YES_BUTTON ]			= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 4, -1, 11, -1 );
	gpAR->iButtonImage[ NO_BUTTON ]				= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 5, -1, 12, -1 );
	gpAR->iButtonImage[ BANDAGE_BUTTON ]	= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 6, -1, 13, -1 );
	gpAR->iButtonImage[ RETREAT_BUTTON ]	= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 14, -1, 15, -1 );
	gpAR->iButtonImage[ DONEWIN_BUTTON ]	= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 14, -1, 15, -1 );
	gpAR->iButtonImage[ DONELOSE_BUTTON ]	= UseLoadedButtonImage( gpAR->iButtonImage[ PAUSE_BUTTON ], -1, 16, -1, 17, -1 );

	//Load the generic faces for civs and enemies
	sprintf( VObjectDesc.ImageFile, "Interface\\SmFaces.sti" );
	if( !AddVideoObject( &VObjectDesc, (UINT32 *)&gpAR->iFaces ) )
	{
		AssertMsg( 0, "Failed to load Interface\\SmFaces.sti" );
	}
	if( GetVideoObject( &hVObject, gpAR->iFaces ) )
	{
		hVObject->pShades[ 0 ] = Create16BPPPaletteShaded( hVObject->pPaletteEntry, 255, 255, 255, FALSE );
		hVObject->pShades[ 1 ] = Create16BPPPaletteShaded( hVObject->pPaletteEntry, 250, 25, 25, TRUE );
	}

	//Add the battle over panels
	sprintf( VObjectDesc.ImageFile, "Interface\\indent.sti" );
	if( !AddVideoObject( &VObjectDesc, (UINT32 *)&gpAR->iIndent	) )
	{
		AssertMsg( 0, "Failed to load Interface\\indent.sti" );
	}

	//add all the faces now
	for( i = 0; i < gpAR->ubMercs; ++i )
	{
		VOBJECT_DESC VObjectDesc;
		//Load the face
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
				
		if ( ( gpMercs[ i ].pSoldier->ubProfile >= 0 ) && ( gpMercs[ i ].pSoldier->ubProfile < 100 ) && gMercProfiles[gpMercs[i].pSoldier->ubProfile].Type == PROFILETYPE_IMP )
		{
			sprintf( VObjectDesc.ImageFile, "IMPFaces\\65Face\\%02d.sti", gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].ubFaceIndex );
		} 
		else if ( ( gpMercs[ i ].pSoldier->ubProfile > 99 ) && gMercProfiles[gpMercs[i].pSoldier->ubProfile].Type == PROFILETYPE_IMP )
		{			
			sprintf( VObjectDesc.ImageFile, "IMPFaces\\65Face\\%02d.sti", gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].ubFaceIndex );			
		}
		else if ( ( gpMercs[ i ].pSoldier->ubProfile >= 0 ) && ( gpMercs[ i ].pSoldier->ubProfile < 100 ) )
		{			
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\%02d.sti", gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].ubFaceIndex );			
		}
		else 
		{			
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\%02d.sti", gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].ubFaceIndex );		
		}
		
		if( !AddVideoObject( &VObjectDesc, &gpMercs[ i ].uiVObjectID ) )
		{
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\speck.sti" );
			if( !AddVideoObject( &VObjectDesc, &gpMercs[ i ].uiVObjectID ) )
			{
				AssertMsg( 0, String("Failed to load %Faces\\65Face\\%02d.sti or it's placeholder, speck.sti", gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].ubFaceIndex) );
			}
		}
		
		if( GetVideoObject( &hVObject, gpMercs[ i ].uiVObjectID ) )
		{
			hVObject->pShades[ 0 ] = Create16BPPPaletteShaded( hVObject->pPaletteEntry, 255, 255, 255, FALSE );
			hVObject->pShades[ 1 ] = Create16BPPPaletteShaded( hVObject->pPaletteEntry, 250, 25, 25, TRUE );
		}
	}

	// 0verhaul:	The following code was modified so that militia bookkeeping could be handled.
	// In the previous auto-resolve method, promotions were often lost because the sector did not
	// actually have the militia to be promoted--they came from surrounding sectors.	The
	// sector X and Y are not actually used for auto-resolve itself, so I can use it to adjust
	// both the actual sector counts and the promotions, by using the origin sectors of each
	// soldier.
	ubEliteMilitia = MilitiaInSectorOfRank( gpAR->ubSectorX, gpAR->ubSectorY, ELITE_MILITIA );
	ubRegMilitia = MilitiaInSectorOfRank( gpAR->ubSectorX, gpAR->ubSectorY, REGULAR_MILITIA );
	ubGreenMilitia = MilitiaInSectorOfRank( gpAR->ubSectorX, gpAR->ubSectorY, GREEN_MILITIA );

	// see if we get any bonus militia from nearby towns
	UINT8 bonusGreenMilitia = 0, bonusRegularMilitia = 0, bonusEliteMilitia = 0;
	RebelCommand::GetBonusMilitia(gpAR->ubSectorX, gpAR->ubSectorY, bonusGreenMilitia, bonusRegularMilitia, bonusEliteMilitia, FALSE); // no need to create a group for autoresolve as we're just increasing local militia pop
	ubEliteMilitia += bonusEliteMilitia;
	ubRegMilitia += bonusRegularMilitia;
	ubGreenMilitia += bonusGreenMilitia;

	// This block should be unnecessary.	If the counts do not line up, there is a bug.
#if 0
	while( ubEliteMilitia + ubRegMilitia + ubGreenMilitia < gpAR->ubCivs )
	{
		switch( PreRandom( 3 ) )
		{
			case 0: ubEliteMilitia++;	break;
			case 1:	ubRegMilitia++;		break;
			case 2:	ubGreenMilitia++;	break;
		}
	}
#endif

	cnt = 0;
	// Add the militia in this sector
	ARCreateMilitiaSquad( &cnt, ubEliteMilitia, ubRegMilitia, ubGreenMilitia, gpAR->ubSectorX, gpAR->ubSectorY);

	// Add the militia in the surrounding sectors
	// HEADROCK HAM 3.4: Changed argument configuration.
	GenerateDirectionInfos( gpAR->ubSectorX, gpAR->ubSectorY, &uiDirNumber, pMoveDir, FALSE, TRUE );
	for( i=0; i<uiDirNumber; ++i)
	{
		INT16 sX = SECTORX( pMoveDir[ i ][0] );
		INT16 sY = SECTORY( pMoveDir[ i ][0] );

		ubEliteMilitia = MilitiaInSectorOfRank( sX, sY, ELITE_MILITIA );
		ubRegMilitia = MilitiaInSectorOfRank( sX, sY, REGULAR_MILITIA );
		ubGreenMilitia = MilitiaInSectorOfRank( sX, sY, GREEN_MILITIA );

		ARCreateMilitiaSquad( &cnt, ubEliteMilitia, ubRegMilitia, ubGreenMilitia, sX, sY );
	}
	
	index = 0;

	if ( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE )
	{
		for ( i = 0; i < gpAR->ubAFCreatures; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateCreature( ADULTFEMALEMONSTER );
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = AF_CREATURE_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[ STR_AR_CREATURE_NAME ] );
		}
		for( i = 0; i < gpAR->ubAMCreatures; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateCreature( AM_MONSTER );
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = AM_CREATURE_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[ STR_AR_CREATURE_NAME ] );
		}
		for( i = 0; i < gpAR->ubYFCreatures; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateCreature( YAF_MONSTER );
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = YF_CREATURE_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[ STR_AR_CREATURE_NAME ] );
		}
		for( i = 0; i < gpAR->ubYMCreatures; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateCreature( YAM_MONSTER );
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = YM_CREATURE_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[ STR_AR_CREATURE_NAME ] );
		}
	}
	else if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
		GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
		GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
	{
		for ( i = 0; i < gpAR->ubBloodcats; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateCreature( BLOODCAT );
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = BLOODCAT_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, pDisplayBodyTypeInfoText[27] );
		}

		for ( i = 0; i < gpAR->ubZombies; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateZombie();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = ZOMBIE_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_PB_ZOMBIE] );
		}

		for ( i = 0; i < gpAR->ubBandits; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateBandit();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;

			if ( gpEnemies[index].pSoldier->ubBodyType == REGFEMALE )
			{
				gpEnemies[index].usIndex = BANDIT1_F_FACE + Random( BLOODCAT_FACE - BANDIT1_F_FACE );
			}
			else
			{
				gpEnemies[index].usIndex = BANDIT1_FACE + Random( BANDIT1_F_FACE - BANDIT1_FACE );
			}

			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_PB_BANDIT] );
		}
	}
	else
	{
		for (i = 0; i < gpAR->ubRobots; ++i, ++index)
		{
			gpEnemies[index].pSoldier = TacticalCreateEnemyRobot();
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_ROBOT_NAME] );

			// reuse madlab's robot's face
			VOBJECT_DESC VObjectDesc;
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			sprintf( VObjectDesc.ImageFile, "Faces\\65Face\\%02d.sti", gMercProfiles[ ROBOT ].ubFaceIndex );
			AddVideoObject(&VObjectDesc, &gpEnemies[index].uiVObjectID);
		}
		for ( i = 0; i < gpAR->ubElites; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateEliteEnemy();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			if ( gpEnemies[index].pSoldier->ubBodyType == REGFEMALE )
			{
				gpEnemies[index].usIndex = ELITEF_FACE;
			}
			else
			{
				gpEnemies[index].usIndex = ELITE_FACE;
			}
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_ELITE_NAME] );
		}
		for ( i = 0; i < gpAR->ubTroops; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateArmyTroop();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			if ( gpEnemies[index].pSoldier->ubBodyType == REGFEMALE )
			{
				gpEnemies[index].usIndex = TROOPF_FACE;
			}
			else
			{
				gpEnemies[index].usIndex = TROOP_FACE;
			}
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_TROOP_NAME] );
		}
		for ( i = 0; i < gpAR->ubAdmins; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateAdministrator();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			if ( gpEnemies[index].pSoldier->ubBodyType == REGFEMALE )
			{
				gpEnemies[index].usIndex = ADMINF_FACE;
			}
			else
			{
				gpEnemies[index].usIndex = ADMIN_FACE;
			}
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_ADMINISTRATOR_NAME] );
		}

		for ( i = 0; i < gpAR->ubTanks; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateEnemyTank();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = TANK_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_TANK_NAME] );
		}

		for ( i = 0; i < gpAR->ubJeeps; ++i, ++index )
		{
			gpEnemies[index].pSoldier = TacticalCreateEnemyJeep();
			gpEnemies[index].uiVObjectID = gpAR->iFaces;
			gpEnemies[index].usIndex = JEEP_FACE;
			gpEnemies[index].pSoldier->sSectorX = gpAR->ubSectorX;
			gpEnemies[index].pSoldier->sSectorY = gpAR->ubSectorY;
			swprintf( gpEnemies[index].pSoldier->name, gpStrategicString[STR_AR_JEEP_NAME] );
		}

		AssociateEnemiesWithStrategicGroups();
	}

	if( gpAR->ubSectorX == gWorldSectorX && gpAR->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
	{
		CheckAndHandleUnloadingOfCurrentWorld();
	}
	else
	{
		gfBlitBattleSectorLocator = FALSE;
	}

	//Build the interface buffer, and blit the "shaded" background.	This info won't
	//change from now on, but will be used to restore text.
	BuildInterfaceBuffer();
	BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, 0 + xResOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	//If we are bumping up the interface, then also use that piece of info to
	//move the buttons up by the same amount.

	gpAR->bVerticalOffset = (240 - gpAR->sHeight/2) > 120 ? -40 : 0;

	//Create the buttons -- subject to relocation
	gpAR->iButton[ PLAY_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ PLAY_BUTTON ] , (INT16)(gpAR->sCenterStartX+11), (INT16)(iScreenHeightOffset + 240+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, PlayButtonCallback );
	gpAR->iButton[ FAST_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ FAST_BUTTON ] , (INT16)(gpAR->sCenterStartX+51), (INT16)(iScreenHeightOffset + 240+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, FastButtonCallback );
	gpAR->iButton[ FINISH_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ FINISH_BUTTON ] , (INT16)(gpAR->sCenterStartX+91), (INT16)(iScreenHeightOffset + 240+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, FinishButtonCallback );
	gpAR->iButton[ PAUSE_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ PAUSE_BUTTON ] , (INT16)(gpAR->sCenterStartX+11), (INT16)(iScreenHeightOffset + 274+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, PauseButtonCallback );

	gpAR->iButton[ RETREAT_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ RETREAT_BUTTON ], (INT16)(gpAR->sCenterStartX+51), (INT16)(iScreenHeightOffset + 274+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, RetreatButtonCallback );

	// no retreat possible if no mercs are present and militia isn't present or cannot be commanded
	if ( !gpAR->ubMercs && (!gpAR->ubCivs || !CanGiveStrategicMilitiaMoveOrder( gpAR->ubSectorX, gpAR->ubSectorY )) )
	{
		DisableButton( gpAR->iButton[ RETREAT_BUTTON ] );
	}

	SpecifyGeneralButtonTextAttributes( gpAR->iButton[ RETREAT_BUTTON ], gpStrategicString[STR_AR_RETREAT_BUTTON], BLOCKFONT2, 169, FONT_NEARBLACK );

	gpAR->iButton[ BANDAGE_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ BANDAGE_BUTTON ] , (INT16)(gpAR->sCenterStartX+11), (INT16)(iScreenHeightOffset + 245+gpAR->bVerticalOffset), BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BandageButtonCallback );

	gpAR->iButton[ DONEWIN_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ DONEWIN_BUTTON ], (INT16)(gpAR->sCenterStartX+51), (INT16)(iScreenHeightOffset + 245+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, DoneButtonCallback );
	SpecifyGeneralButtonTextAttributes( gpAR->iButton[ DONEWIN_BUTTON ], gpStrategicString[STR_AR_DONE_BUTTON], BLOCKFONT2, 169, FONT_NEARBLACK );

	gpAR->iButton[ DONELOSE_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ DONELOSE_BUTTON ], (INT16)(gpAR->sCenterStartX+25), (INT16)(iScreenHeightOffset + 245+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, DoneButtonCallback );
	SpecifyGeneralButtonTextAttributes( gpAR->iButton[ DONELOSE_BUTTON ], gpStrategicString[STR_AR_DONE_BUTTON], BLOCKFONT2, 169, FONT_NEARBLACK );
	gpAR->iButton[ YES_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ YES_BUTTON ], (INT16)(gpAR->sCenterStartX+21), (INT16)(iScreenHeightOffset + 257+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, AcceptSurrenderCallback );
	gpAR->iButton[ NO_BUTTON ] =
		QuickCreateButton( gpAR->iButtonImage[ NO_BUTTON ], (INT16)(gpAR->sCenterStartX+81), (INT16)(iScreenHeightOffset + 257+gpAR->bVerticalOffset), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, RejectSurrenderCallback );
	HideButton( gpAR->iButton[ YES_BUTTON ] );
	HideButton( gpAR->iButton[ NO_BUTTON ] );
	HideButton( gpAR->iButton[ DONEWIN_BUTTON ] );
	HideButton( gpAR->iButton[ DONELOSE_BUTTON ] );
	HideButton( gpAR->iButton[ BANDAGE_BUTTON ] );
	ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
}

void RemoveAutoResolveInterface( BOOLEAN fDeleteForGood )
{
	INT32 i;
	UINT8 ubCurrentRank;
	UINT8 ubCurrentGroupID = 0;
	BOOLEAN fFirstGroup = TRUE;

DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Autoresolve2");
	//VtResumeSampling();

	MSYS_RemoveRegion( &gpAR->AutoResolveRegion );
	DeleteVideoObjectFromIndex( gpAR->iPanelImages );
	DeleteVideoObjectFromIndex( gpAR->iFaces );
	DeleteVideoObjectFromIndex( gpAR->iIndent );
	DeleteVideoSurfaceFromIndex( gpAR->iInterfaceBuffer );

	if( fDeleteForGood )
	{ //Delete the soldier instances -- done when we are completely finished.

		//KM: By request of AM, I have added this bleeding event in cases where autoresolve is
		//	complete and there are bleeding mercs remaining.	AM coded the internals
		//	of the strategic event.
		for( i = 0; i < gpAR->ubMercs; i++ )
		{
			if( gpMercs[ i ].pSoldier->bBleeding && gpMercs[ i ].pSoldier->stats.bLife )
			{
				// ARM: only one event is needed regardless of how many are bleeding
				AddStrategicEvent( EVENT_BANDAGE_BLEEDING_MERCS, GetWorldTotalMin() + 1, 0 );
				break;
			}
		}

		// ARM: Update assignment flashing: Doctors may now have new patients or lost them all, etc.
		gfReEvaluateEveryonesNothingToDo = TRUE;


		if( gpAR->pRobotCell)
		{
			gpAR->pRobotCell->pSoldier->UpdateRobotControllerGivenRobot( );
		}
		for( i = 0; i < gpAR->iNumMercFaces; i++ )
		{
			if( i >= gpAR->iActualMercFaces )
				TacticalRemoveSoldierPointer( gpMercs[ i ].pSoldier, FALSE );
			else
			{ //Record finishing information for our mercs
				if( !gpMercs[ i ].pSoldier->stats.bLife )
				{
					StrategicHandlePlayerTeamMercDeath( gpMercs[ i ].pSoldier );

					// now remove character from a squad
					RemoveCharacterFromSquads( gpMercs[ i ].pSoldier );
					ChangeSoldiersAssignment( gpMercs[ i ].pSoldier, ASSIGNMENT_DEAD );

					AddDeadSoldierToUnLoadedSector( gpAR->ubSectorX, gpAR->ubSectorY, 0, gpMercs[ i ].pSoldier, RandomGridNo(), ADD_DEAD_SOLDIER_TO_SWEETSPOT );
				}
				else if( gpAR->ubBattleStatus == BATTLE_SURRENDERED || gpAR->ubBattleStatus == BATTLE_CAPTURED )
				{
					EnemyCapturesPlayerSoldier( gpMercs[ i ].pSoldier );
				}
				else if( gpAR->ubBattleStatus == BATTLE_VICTORY )
				{ //merc is alive, so group them at the center gridno.
					gpMercs[ i ].pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CENTER;
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// SANDRO - records - autoresolve battles
					if( gpMercs[ i ].pSoldier->ubProfile != NO_PROFILE )
					{
						// if we ran before the battle was over but the others won in the end, we still get retreated counter, no victory
						if ( gpMercs[ i ].uiFlags & CELL_RETREATED || gpMercs[ i ].uiFlags & CELL_RETREATING )
						{
							gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].records.usBattlesRetreated++;
						}
						else
						{
							gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].records.usBattlesAutoresolve++; 
							if ( gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].records.usLargestBattleFought < gpAR->ubEnemies )
							{
								gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].records.usLargestBattleFought = gpAR->ubEnemies;
							}
						}
					}
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - merc records - times retreated counter
				else if( gpAR->ubBattleStatus == BATTLE_RETREAT )
				{
					if( gpMercs[ i ].uiFlags & CELL_RETREATED || gpMercs[ i ].uiFlags & CELL_RETREATING )
					{
						if( gpMercs[ i ].pSoldier->ubProfile != NO_PROFILE && !AM_A_ROBOT( gpMercs[ i ].pSoldier ) )
						{
							gMercProfiles[ gpMercs[ i ].pSoldier->ubProfile ].records.usBattlesRetreated++;
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
			}
		}
		for( i = 0; i < gpAR->iNumMercFaces; i++ )
		{
			if( gpAR->ubBattleStatus == BATTLE_VICTORY && gpMercs[ i ].pSoldier->stats.bLife >= OKLIFE )
			{
				if( gpMercs[ i ].pSoldier->ubGroupID != ubCurrentGroupID )
				{
					ubCurrentGroupID = gpMercs[ i ].pSoldier->ubGroupID;

					// look for NPCs to stop for, anyone is too tired to keep going, if all OK rebuild waypoints & continue movement
					// NOTE: Only the first group found will stop for NPCs, it's just too much hassle to stop them all
					PlayerGroupArrivedSafelyInSector( GetGroup( gpMercs[ i ].pSoldier->ubGroupID ), fFirstGroup );
					fFirstGroup = FALSE;
				}
			}
			gpMercs[ i ].pSoldier = NULL;
		}

		// End capture squence....
		if( gpAR->ubBattleStatus == BATTLE_SURRENDERED || gpAR->ubBattleStatus == BATTLE_CAPTURED )
		{
			EndCaptureSequence( );
		}

	}
	//Delete all of the faces.
	for( i = 0; i < gpAR->iNumMercFaces; i++ )
	{
		if( gpMercs[ i ].uiVObjectID != -1 )
			DeleteVideoObjectFromIndex( gpMercs[ i ].uiVObjectID );
		gpMercs[ i ].uiVObjectID = -1;
		if( gpMercs[ i ].pRegion )
		{
			MSYS_RemoveRegion( gpMercs[ i ].pRegion );
			MemFree( gpMercs[ i ].pRegion );
			gpMercs[ i ].pRegion = NULL;
		}
	}
	//Delete all militia
	gbGreenToElitePromotions = 0;
	gbGreenToRegPromotions = 0;
	gbRegToElitePromotions = 0;
	gbMilitiaPromotions = 0;
	for( i = 0; i < MAX_AR_TEAM_SIZE; ++i )
	{
		if( gpCivs[ i ].pSoldier )
		{
			ubCurrentRank = GREEN_MILITIA;
			switch( gpCivs[ i ].pSoldier->ubSoldierClass )
			{
				case SOLDIER_CLASS_GREEN_MILITIA:		ubCurrentRank = GREEN_MILITIA;		break;
				case SOLDIER_CLASS_REG_MILITIA:			ubCurrentRank = REGULAR_MILITIA;	break;
				case SOLDIER_CLASS_ELITE_MILITIA:		ubCurrentRank = ELITE_MILITIA;		break;
				default:
					#ifdef JA2BETAVERSION
						ScreenMsg( FONT_RED, MSG_ERROR, L"Removing autoresolve militia with invalid ubSoldierClass %d.",gpCivs[ i ].pSoldier->ubSoldierClass );
					#endif
					break;
			}

			// Flugente: drop sector equipment
			gpCivs[ i ].pSoldier->DropSectorEquipment();

			if( fDeleteForGood && gpCivs[ i ].pSoldier->stats.bLife < OKLIFE/2 )
			{
				// Flugente: individual militia
				// we not only handle promotions here, but basically update this guy (if not already counted as dead)
				MILITIA militia;
				if ( GetMilitia( gpCivs[i].pSoldier->usIndividualMilitiaID, &militia ) && !(militia.flagmask & MILITIAFLAG_DEAD) )
				{
					MILITIA_BATTLEREPORT report;
					report.id = GetIdOfCurrentlyOngoingIncident( );

					report.flagmask |= MILITIA_BATTLEREPORT_FLAG_DIED;

					if ( gpCivs[i].pSoldier->ubMilitiaKills )
						report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY;

					militia.history.push_back( report );

					militia.healthratio = 0.0f;
					militia.flagmask |= MILITIAFLAG_DEAD;

					UpdateMilitia( militia );
				}

				AddDeadSoldierToUnLoadedSector( gpAR->ubSectorX, gpAR->ubSectorY, 0, gpCivs[ i ].pSoldier, RandomGridNo(), ADD_DEAD_SOLDIER_TO_SWEETSPOT );
				StrategicRemoveMilitiaFromSector( gpCivs[ i ].pSoldier->sSectorX, gpCivs[ i ].pSoldier->sSectorY, ubCurrentRank, 1 );

				if( ProcessLoyalty() )
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_NATIVE_KILLED, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
			}
			else
			{
				// Flugente: take care of promotions and individual militia update
				HandlePossibleMilitiaPromotion( gpCivs[i].pSoldier, TRUE );
			}

			TacticalRemoveSoldierPointer( gpCivs[ i ].pSoldier, FALSE );
			memset( &gpCivs[ i ], 0, sizeof( SOLDIERCELL ) );
		}
	}

	// Flugente: after we fought, move any militia in groups into the sector garrison - the player will likely want to rethink orders for the survivors
	// Flugente: in case militia died we have to lower the group sizes
	// loop over all militia groups that are in this sector and alter their size. If a group has size 0 delete it. If any militia are left place them into this sector
	DissolveAllMilitiaGroupsInSector(gpAR->ubSectorX, gpAR->ubSectorY);

	//Record and process all enemy deaths
	for( i = 0; i < MAX_AR_TEAM_SIZE; ++i )
	{
		if( gpEnemies[ i ].pSoldier )
		{
			if( fDeleteForGood && gpEnemies[ i ].pSoldier->stats.bLife < OKLIFE ) //if we are finished with battle and soldier is either dead or dying
			{
				TrackEnemiesKilled( ENEMY_KILLED_IN_AUTO_RESOLVE, gpEnemies[ i ].pSoldier->ubSoldierClass );	//add casualty to some statistic
				if( ProcessLoyalty() )HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_ENEMY_KILLED, gpAR->ubSectorX, gpAR->ubSectorY, 0 );
				ProcessQueenCmdImplicationsOfDeath( gpEnemies[ i ].pSoldier );
				AddDeadSoldierToUnLoadedSector( gpAR->ubSectorX, gpAR->ubSectorY, 0, gpEnemies[ i ].pSoldier, RandomGridNo(), ADD_DEAD_SOLDIER_TO_SWEETSPOT );
			}
		}
	}
	//Eliminate all excess soldiers
	if( fDeleteForGood )
	{ //Warp the game time accordingly
		if( gpAR->ubBattleStatus == BATTLE_VICTORY )
		{	//Get rid of any extra enemies that could be here.
			EliminateAllEnemies( gpAR->ubSectorX, gpAR->ubSectorY );
		}
		else
		{ //The enemy won, so repoll movement.
			ResetMovementForNonPlayerGroupsInLocation( gpAR->ubSectorX, gpAR->ubSectorY );
		}
	}
	//Physically delete the soldiers now.
	for( i = 0; i < MAX_AR_TEAM_SIZE; ++i )
	{
		if( gpEnemies[ i ].pSoldier )
		{
			TacticalRemoveSoldierPointer( gpEnemies[ i ].pSoldier, TRUE );
			memset( &gpEnemies[ i ], 0, sizeof( SOLDIERCELL ) );
		}
	}

	for( i = 0; i < NUM_AR_BUTTONS; ++i )
	{
		UnloadButtonImage( gpAR->iButtonImage[ i ] );
		RemoveButton( gpAR->iButton[ i ] );
	}

	if( fDeleteForGood )
	{
		//Warp the game time accordingly
		WarpGameTime( gpAR->uiTotalElapsedBattleTimeInMilliseconds/1000, WARPTIME_NO_PROCESSING_OF_EVENTS );

		//Deallocate all of the global memory.
		//Everything internal to them, should have already been deleted.
		MemFree( gpAR );
		gpAR = NULL;

		MemFree( gpMercs );
		gpMercs = NULL;

		MemFree( gpCivs );
		gpCivs = NULL;

		MemFree( gpEnemies );
		gpEnemies = NULL;
	}

	//KM : Aug 09, 1999 Patch fix -- Would break future dialog while time compressing
	gTacticalStatus.ubCurrentTeam = gbPlayerNum;

	gpBattleGroup = NULL;

	if ( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE || 
		GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
		GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
		GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
	{
		gubNumCreaturesAttackingTown = 0;
		gubSectorIDOfCreatureAttack = 0;
	}
	//VtPauseSampling();
}

void PauseButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FAST_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FINISH_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		gpAR->fPaused = TRUE;
	}
}

void PlayButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ gpAR->iButton[ PAUSE_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FAST_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FINISH_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		gpAR->uiTimeSlice = 1000 * gpAR->ubTimeModifierPercentage / 100;
		gpAR->fPaused = FALSE;
	}
}

void FastButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ gpAR->iButton[ PAUSE_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FINISH_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		gpAR->uiTimeSlice = 4000;
		gpAR->fPaused = FALSE;
	}
}

void FinishButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ gpAR->iButton[ PAUSE_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ gpAR->iButton[ FAST_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		gpAR->uiTimeSlice = 0xffffffff;
		gpAR->fSound = FALSE;
		gpAR->fPaused = FALSE;
		PlayJA2StreamingSample( AUTORESOLVE_FINISHFX, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
	}
}

void RetreatButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		INT32 i;
		for ( i = 0; i < gpAR->ubMercs; ++i )
		{
			if( !(gpMercs[ i ].uiFlags & (CELL_RETREATING|CELL_RETREATED)) )
			{
				gpMercs[ i ].uiFlags |= CELL_RETREATING | CELL_DIRTY;
				//Gets to retreat after a total of 2 attacks.
				gpMercs[ i ].usNextAttack = (UINT16)((1000 + gpMercs[ i ].usNextAttack * 2 + PreRandom( 2000 - gpMercs[ i ].usAttack ))*2);
				gpAR->usPlayerAttack -= gpMercs[ i ].usAttack;
				gpMercs[ i ].usAttack = 0;
			}
		}

		// Flugente: militia can be ordered to retreat
		if ( gGameExternalOptions.fMilitiaStrategicCommand )
		{
			for ( INT32 i = 0; i < gpAR->ubCivs; ++i )
			{
				if ( !(gpCivs[i].uiFlags & (CELL_RETREATING | CELL_RETREATED)) )
				{
					gpCivs[i].uiFlags |= CELL_RETREATING | CELL_DIRTY;
					//Gets to retreat after a total of 2 attacks.
					gpCivs[i].usNextAttack = (UINT16)((1000 + gpCivs[i].usNextAttack * 2 + PreRandom( 2000 - gpCivs[i].usAttack )) * 2);
					gpAR->usPlayerAttack -= gpCivs[i].usAttack;
					gpCivs[i].usAttack = 0;
				}
			}
		}

		if( gpAR->pRobotCell )
		{
			//if robot is retreating, set the retreat time to be the same as the robot's controller.
			UINT8 ubRobotControllerID;

			ubRobotControllerID = gpAR->pRobotCell->pSoldier->ubRobotRemoteHolderID;

			if( ubRobotControllerID == NOBODY )
			{
				gpAR->pRobotCell->uiFlags &= ~CELL_RETREATING;
				gpAR->pRobotCell->uiFlags |= CELL_DIRTY;
				gpAR->pRobotCell->usNextAttack = 0xffff;
				return;
			}

			for( i = 0; i < gpAR->ubMercs; ++i )
			{
				if( ubRobotControllerID == gpMercs[ i ].pSoldier->ubID )
				{
					//Found the controller, make the robot's retreat time match the contollers.
					gpAR->pRobotCell->usNextAttack = gpMercs[ i ].usNextAttack;
					return;
				}
			}
		}
	}
}

void DetermineBandageButtonState()
{
	INT32 i;
	OBJECTTYPE *pKit = NULL;
	BOOLEAN fFound = FALSE;

	//Does anyone need bandaging?
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		if( gpMercs[ i ].pSoldier->bBleeding && gpMercs[ i ].pSoldier->stats.bLife )
		{
			fFound = TRUE;
			break;
		}
	}
	if( !fFound )
	{
		DisableButton( gpAR->iButton[ BANDAGE_BUTTON ] );
		SetButtonFastHelpText( gpAR->iButton[ BANDAGE_BUTTON ], gzLateLocalizedString[ 11 ] );
		return;
	}

	//Do we have any doctors?
	fFound = FALSE;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		if( gpMercs[ i ].pSoldier->stats.bLife >= OKLIFE &&
			!gpMercs[ i ].pSoldier->bCollapsed &&
				gpMercs[ i ].pSoldier->stats.bMedical > 0 )
		{
			fFound = TRUE;
		}
	}
	if( !fFound )
	{	//No doctors
		DisableButton( gpAR->iButton[ BANDAGE_BUTTON ] );
		SetButtonFastHelpText( gpAR->iButton[ BANDAGE_BUTTON ], gzLateLocalizedString[ 8 ] );
		return;
	}

	//Do have a kit?
	pKit = FindMedicalKit();
	if( !pKit )
	{ //No kits
		DisableButton( gpAR->iButton[ BANDAGE_BUTTON ] );
		SetButtonFastHelpText( gpAR->iButton[ BANDAGE_BUTTON ], gzLateLocalizedString[ 9 ] );
		return;
	}

	//Allow bandaging.
	EnableButton( gpAR->iButton[ BANDAGE_BUTTON ] );
	SetButtonFastHelpText( gpAR->iButton[ BANDAGE_BUTTON ], gzLateLocalizedString[ 12 ] );
}

void BandageButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		AutoBandageMercs();
		SetupDoneInterface();
	}
}

void DoneButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gpAR->fExitAutoResolve = TRUE;
	}
}

void MercCellMouseMoveCallback( MOUSE_REGION *reg, INT32 reason )
{
	//Find the merc with the same region.
	INT32 i;
	SOLDIERCELL *pCell = NULL;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		if( gpMercs[ i ].pRegion == reg )
		{
			pCell = &gpMercs[ i ];
			break;
		}
	}
	Assert( pCell );
	if( gpAR->fPendingSurrender )
	{ //Can't setup retreats when pending surrender.
		pCell->uiFlags &= ~CELL_SHOWRETREATTEXT;
		pCell->uiFlags |= CELL_DIRTY;
		return;
	}
	if( reg->uiFlags & MSYS_MOUSE_IN_AREA )
	{
		if( !(pCell->uiFlags & CELL_SHOWRETREATTEXT) )
			pCell->uiFlags |= CELL_SHOWRETREATTEXT | CELL_DIRTY;
	}
	else
	{
		if( pCell->uiFlags & CELL_SHOWRETREATTEXT )
		{
			pCell->uiFlags &= ~CELL_SHOWRETREATTEXT;
			pCell->uiFlags |= CELL_DIRTY;
		}
	}
}

void MercCellMouseClickCallback( MOUSE_REGION *reg, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		//Find the merc with the same region.
		INT32 i;
		SOLDIERCELL *pCell = NULL;

		if( gpAR->fPendingSurrender )
		{ //Can't setup retreats when pending surrender.
			return;
		}

		for( i = 0; i < gpAR->ubMercs; i++ )
		{
			if( gpMercs[ i ].pRegion == reg )
			{
				pCell = &gpMercs[ i ];
				break;
			}
		}

        Assert(pCell);
		if( pCell->uiFlags & ( CELL_RETREATING | CELL_RETREATED ) )
		{ //already retreated/retreating.
			return;
		}

		Assert( pCell );

		if( pCell == gpAR->pRobotCell )
		{ //robot retreats only when controller retreats
			return;
		}

		pCell->uiFlags |= CELL_RETREATING | CELL_DIRTY;
		//Gets to retreat after a total of 2 attacks.
		pCell->usNextAttack = (UINT16)((1000 + pCell->usNextAttack * 5 + PreRandom( 2000 - pCell->usAttack ))*2);
		gpAR->usPlayerAttack -= pCell->usAttack;
		pCell->usAttack = 0;

		if( gpAR->pRobotCell )
		{ //if controller is retreating, make the robot retreat too.
			UINT8 ubRobotControllerID;

			ubRobotControllerID = gpAR->pRobotCell->pSoldier->ubRobotRemoteHolderID;

			if( ubRobotControllerID == NOBODY )
			{
				gpAR->pRobotCell->uiFlags &= ~CELL_RETREATING;
				gpAR->pRobotCell->uiFlags |= CELL_DIRTY;
				gpAR->pRobotCell->usNextAttack = 0xffff;
			}
			else if( ubRobotControllerID == pCell->pSoldier->ubID )
			{ //Found the controller, make the robot's retreat time match the contollers.
				gpAR->pRobotCell->uiFlags |= CELL_RETREATING | CELL_DIRTY;
				gpAR->pRobotCell->usNextAttack = pCell->usNextAttack;
				gpAR->usPlayerAttack -= gpAR->pRobotCell->usAttack;
				gpAR->pRobotCell->usAttack = 0;
				return;
			}
		}
	}
}

//Determine how many players, militia, and enemies that are going at it, and use these values
//to figure out how many rows and columns we can use.	The will effect the size of the panel.
void CalculateAutoResolveInfo()
{
	VOBJECT_DESC VObjectDesc;
	GROUP *pGroup;
	PLAYERGROUP *pPlayer;
	Assert( gpAR->ubSectorX >= 1 && gpAR->ubSectorX <= 16 );
	Assert( gpAR->ubSectorY >= 1 && gpAR->ubSectorY <= 16 );

	if ( GetEnemyEncounterCode() == CREATURE_ATTACK_CODE )
	{
		if( gfTransferTacticalOppositionToAutoResolve )
		{
			DetermineCreatureTownCompositionBasedOnTacticalInformation( &gubNumCreaturesAttackingTown,
																				&gpAR->ubYMCreatures, &gpAR->ubYFCreatures,
																				&gpAR->ubAMCreatures, &gpAR->ubAFCreatures );
		}
		else
		{
			DetermineCreatureTownComposition( gubNumCreaturesAttackingTown,
																				&gpAR->ubYMCreatures, &gpAR->ubYFCreatures,
																				&gpAR->ubAMCreatures, &gpAR->ubAFCreatures );
		}

		// WANNE: Quickfix: If we have 0 creatues call the DetermineCreatureTownCompositionBasedOnTacticalInformation(),
		// because we called the DetermineCreatureTownComposition() method before which was wrong.
		if (gubNumCreaturesAttackingTown == 0)
		{
			DetermineCreatureTownCompositionBasedOnTacticalInformation( &gubNumCreaturesAttackingTown, 
																				&gpAR->ubYMCreatures, &gpAR->ubYFCreatures,
																				&gpAR->ubAMCreatures, &gpAR->ubAFCreatures );
		}

		gpAR->ubEnemies = (UINT8)min( gpAR->ubYMCreatures + gpAR->ubYFCreatures + gpAR->ubAMCreatures + gpAR->ubAFCreatures, MAX_AR_TEAM_SIZE );
	}
	else if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
		GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
		GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
	{
		if ( gfTransferTacticalOppositionToAutoResolve || gubNumCreaturesAttackingTown == 0 )
		{
			DetermineOtherCreatureTownCompositionBasedOnTacticalInformation( &gubNumCreaturesAttackingTown, &gpAR->ubBloodcats, &gpAR->ubZombies, &gpAR->ubBandits );
		}
		else
		{
			if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE )
				gpAR->ubBloodcats = gubNumCreaturesAttackingTown;
			else if ( GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE )
				gpAR->ubZombies = gubNumCreaturesAttackingTown;
			else if ( GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
				gpAR->ubBandits = gubNumCreaturesAttackingTown;
		}

		gpAR->ubEnemies = (UINT8)min( gubNumCreaturesAttackingTown, MAX_AR_TEAM_SIZE );
	}
	else
	{
		ActivateTurncoatsForAutoResolve( gpAR->ubSectorX, gpAR->ubSectorY );

		GetNumberOfEnemiesInFiveSectors( gpAR->ubSectorX, gpAR->ubSectorY,
			&gpAR->ubAdmins, &gpAR->ubTroops, &gpAR->ubElites, &gpAR->ubRobots, &gpAR->ubTanks, &gpAR->ubJeeps );

		gpAR->ubEnemies = (UINT8)min( gpAR->ubAdmins + gpAR->ubTroops + gpAR->ubElites + gpAR->ubTanks + gpAR->ubJeeps + gpAR->ubRobots, MAX_AR_TEAM_SIZE );
	}

	gfTransferTacticalOppositionToAutoResolve = FALSE;
	gpAR->ubCivs = CountAllMilitiaInFiveSectors( gpAR->ubSectorX, gpAR->ubSectorY );
	gpAR->ubMercs = 0;
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( PlayerGroupInvolvedInThisCombat( pGroup ) )
		{
			pPlayer = pGroup->pPlayerList;
			while( pPlayer )
			{
				// NOTE: Must check each merc individually, e.g. Robot without controller is an uninvolved merc on an involved group!
				if ( PlayerMercInvolvedInThisCombat( pPlayer->pSoldier ) )
				{
					gpMercs[ gpAR->ubMercs ].pSoldier = pPlayer->pSoldier;

					//!!! CLEAR OPPCOUNT HERE.	All of these soldiers are guaranteed to not be in tactical anymore.
					//ClearOppCount( pPlayer->pSoldier );

					gpAR->ubMercs++;
					if( AM_AN_EPC( pPlayer->pSoldier ) )
					{
						gpAR->fCaptureNotPermittedDueToEPCs = TRUE;
					}
					if( AM_A_ROBOT( pPlayer->pSoldier ) )
					{
						gpAR->pRobotCell = &gpMercs[ gpAR->ubMercs - 1 ];
						gpAR->pRobotCell->pSoldier->UpdateRobotControllerGivenRobot( );
					}
				}
				pPlayer = pPlayer->next;
			}
		}
		pGroup = pGroup->next;
	}
	gpAR->iNumMercFaces = gpAR->ubMercs;
	gpAR->iActualMercFaces = gpAR->ubMercs;

	giMaxMilitiaToRender = 50 - ( (gpAR->ubMercs + 4) / 5 ) * 5;

	CalculateRowsAndColumns();
}

void ResetAutoResolveInterface()
{
	guiPreRandomIndex = gpAR->uiPreRandomIndex;

	RemoveAutoResolveInterface( FALSE );

	gpAR->ubBattleStatus = BATTLE_IN_PROGRESS;

	if( !gpAR->ubCivs && !gpAR->ubMercs )
		gpAR->ubCivs = 1;

	//Make sure the number of enemy portraits is the same as needed.
	//The debug keypresses may add or remove more than one at a time.
	while ( gpAR->ubElites + gpAR->ubAdmins + gpAR->ubTroops + gpAR->ubTanks + gpAR->ubJeeps + gpAR->ubRobots > gpAR->ubEnemies )
	{
		switch( PreRandom( 6 ) )
		{
			case 0: if ( gpAR->ubElites ) { gpAR->ubElites--; break; }
			case 1: if ( gpAR->ubAdmins ) { gpAR->ubAdmins--; break; }
			case 2: if ( gpAR->ubTroops ) { gpAR->ubTroops--; break; }
			case 3: if ( gpAR->ubTanks ) { gpAR->ubTanks--; break; }
			case 4: if ( gpAR->ubJeeps ) { gpAR->ubJeeps--; break; }
			case 5: if ( gpAR->ubRobots ) { gpAR->ubRobots--; break; }
		}
	}
	while ( gpAR->ubElites + gpAR->ubAdmins + gpAR->ubTroops + gpAR->ubTanks + gpAR->ubJeeps + gpAR->ubRobots < gpAR->ubEnemies )
	{
		switch( PreRandom( 5 ) )
		{
			case 0: gpAR->ubElites++; break;
			case 1: case 2: gpAR->ubAdmins++; break;
			case 3: case 4: gpAR->ubTroops++; break;
		}
	}


	//Do the same for the player mercs.
	while( gpAR->iNumMercFaces > gpAR->ubMercs && gpAR->iNumMercFaces > gpAR->iActualMercFaces )
	{ //Removing temp mercs
		gpAR->iNumMercFaces--;
		TacticalRemoveSoldierPointer( gpMercs[gpAR->iNumMercFaces].pSoldier, FALSE );
		gpMercs[gpAR->iNumMercFaces].pSoldier = NULL;
	}
	while( gpAR->iNumMercFaces < gpAR->ubMercs && gpAR->iNumMercFaces >= gpAR->iActualMercFaces )
	{
		CreateTempPlayerMerc();
	}

	if( gpAR->uiTimeSlice == 0xffffffff )
	{
		gpAR->fSound = TRUE;
	}
	gpAR->uiTimeSlice = 1000;
	gpAR->uiTotalElapsedBattleTimeInMilliseconds	= 0;
	gpAR->uiCurrTime = 0;
	gpAR->fPlayerRejectedSurrenderOffer = FALSE;
	gpAR->fPendingSurrender = FALSE;
	CalculateRowsAndColumns();
	CalculateSoldierCells( TRUE);
	CreateAutoResolveInterface();
	DetermineTeamLeader( TRUE ); //friendly team
	DetermineTeamLeader( FALSE ); //enemy team
	CalculateAttackValues();
}

void CalculateRowsAndColumns()
{
	//now that we have the number on each team, calculate the number of rows and columns to be used on
	//the player's sides.	NOTE:	Militia won't appear on the same row as mercs.
	if( !gpAR->ubMercs )
	{ //0
		gpAR->ubMercCols = gpAR->ubMercRows = 0;
	}
	else if( gpAR->ubMercs < 5 )
	{ //1-4
		gpAR->ubMercCols = 1;
		gpAR->ubMercRows = gpAR->ubMercs;
	}
	else if( gpAR->ubMercs < 9 || gpAR->ubMercs == 10 )
	{ //5-8, 10
		gpAR->ubMercCols = 2;
		gpAR->ubMercRows = (gpAR->ubMercs+1)/2;
	}
	else if( gpAR->ubMercs < 16 )
	{ //9, 11-15
		gpAR->ubMercCols = 3;
		gpAR->ubMercRows = (gpAR->ubMercs+2)/3;
	}
	else
	{ //16-MAX_STRATEGIC_TEAM_SIZE
		gpAR->ubMercCols = 4;
		gpAR->ubMercRows = (gpAR->ubMercs+3)/4;
	}

	if( !gpAR->ubCivs )
	{
		gpAR->ubCivCols = gpAR->ubCivRows = 0;
	}
	else if( gpAR->ubCivs < 5 )
	{ //1-4
		gpAR->ubCivCols = 1;
		gpAR->ubCivRows = gpAR->ubCivs;
	}
	else if( gpAR->ubCivs < 9 || gpAR->ubCivs == 10 )
	{ //5-8, 10
		gpAR->ubCivCols = 2;
		gpAR->ubCivRows = (gpAR->ubCivs+1)/2;
	}
	else if( gpAR->ubCivs < 16 )
	{ //9, 11-15
		gpAR->ubCivCols = 3;
		gpAR->ubCivRows = (gpAR->ubCivs+2)/3;
	}
	else
	{ //16-MAX_ALLOWABLE_MILITIA_PER_SECTOR
		gpAR->ubCivCols = 4;
		gpAR->ubCivRows = (gpAR->ubCivs+3)/4;
	}

	if( !gpAR->ubEnemies )
	{
		gpAR->ubEnemyCols = gpAR->ubEnemyRows = 0;
	}
	else if( gpAR->ubEnemies < 5 )
	{ //1-4
		gpAR->ubEnemyCols = 1;
		gpAR->ubEnemyRows = gpAR->ubEnemies;
	}
	else if( gpAR->ubEnemies < 9 || gpAR->ubEnemies == 10 )
	{ //5-8, 10
		gpAR->ubEnemyCols = 2;
		gpAR->ubEnemyRows = (gpAR->ubEnemies+1)/2;
	}
	else if( gpAR->ubEnemies < 16 )
	{ //9, 11-15
		gpAR->ubEnemyCols = 3;
		gpAR->ubEnemyRows = (gpAR->ubEnemies+2)/3;
	}
	else
	{ //16+
		gpAR->ubEnemyCols = 4;
		gpAR->ubEnemyRows = (gpAR->ubEnemies+3)/4;
	}


	//Now, that we have the number of mercs, militia, and enemies, it is possible that there
	//may be some conflicts.	Our goal is to make the window as small as possible.	Bumping up
	//the number of columns to 5 or rows to 10 will force one or both axes to go full screen.	If we
	//have high numbers of both, then we will have to.

	//Step one:	equalize the number of columns for both the mercs and civs.
	if( gpAR->ubMercs && gpAR->ubCivs && gpAR->ubMercCols != gpAR->ubCivCols )
	{
		if( gpAR->ubMercCols < gpAR->ubCivCols )
		{
			gpAR->ubMercCols = gpAR->ubCivCols;
			gpAR->ubMercRows = (gpAR->ubMercs+gpAR->ubMercCols-1)/gpAR->ubMercCols;
		}
		else
		{
			gpAR->ubCivCols = gpAR->ubMercCols;
			gpAR->ubCivRows = (gpAR->ubCivs+gpAR->ubCivCols-1)/gpAR->ubCivCols;
		}
	}
	//If we have both mercs and militia, we must make sure that the height to width ratio is never higher than
	//a factor of two.
	if( gpAR->ubMercs && gpAR->ubCivs && gpAR->ubMercRows + gpAR->ubCivRows > 4 )
	{
		if( gpAR->ubMercCols * 2 < gpAR->ubMercRows + gpAR->ubCivRows )
		{
			gpAR->ubMercCols++;
			gpAR->ubMercRows = (gpAR->ubMercs+gpAR->ubMercCols-1)/gpAR->ubMercCols;
			gpAR->ubCivCols++;
			gpAR->ubCivRows = (gpAR->ubCivs+gpAR->ubCivCols-1)/gpAR->ubCivCols;
		}
	}


	if( gpAR->ubMercRows + gpAR->ubCivRows > 9 )
	{
		if( gpAR->ubMercCols < 5 )
		{ //bump it up
			gpAR->ubMercCols++;
			gpAR->ubMercRows = (gpAR->ubMercs+gpAR->ubMercCols-1)/gpAR->ubMercCols;
		}
		if( gpAR->ubCivCols < 5 )
		{ //match it up with the mercs
			gpAR->ubCivCols++; // = gpAR->ubMercCols;
			gpAR->ubCivRows = (gpAR->ubCivs+gpAR->ubCivCols-1)/gpAR->ubCivCols;
		}
	}

	if( gpAR->ubMercCols + gpAR->ubEnemyCols == 9 )
		gpAR->sWidth = SCREEN_WIDTH;
	else
		gpAR->sWidth = 146 + 55 * (max( max( gpAR->ubMercCols, gpAR->ubCivCols ), 2 ) + max( gpAR->ubEnemyCols, 2 ));

	//gpAR->sCenterStartX = 323 - gpAR->sWidth/2 + max( max( gpAR->ubMercCols, 2), max( gpAR->ubCivCols, 2 ) ) *55;
	gpAR->sCenterStartX = iScreenWidthOffset + (323 - gpAR->sWidth/2 + max( max( gpAR->ubMercCols, 2), max( gpAR->ubCivCols, 2 ) ) *55);


	//Anywhere from 48*3 to 48*10
	//gpAR->sHeight = 48 * max( 3, max( gpAR->ubMercRows + gpAR->ubCivRows, gpAR->ubEnemyRows ) );
	gpAR->sHeight = 48 * max( 3, max( min( 10, gpAR->ubMercRows + gpAR->ubCivRows ), min( 10, gpAR->ubEnemyRows ) ) );
	//Make it an even multiple of 40 (rounding up).
	gpAR->sHeight += 39;
	gpAR->sHeight /= 40;
	gpAR->sHeight *= 40;

	//Here is a extremely bitchy case.	The formulae throughout this module work for most cases.
	//However, when combining mercs and civs, the column must be the same.	However, we retract that
	//in cases where there are less mercs than available to fill up *most* of the designated space.
	if( gpAR->ubMercs && gpAR->ubCivs )
	{
		if( gpAR->ubMercRows * gpAR->ubMercCols > gpAR->ubMercs + gpAR->ubMercRows )
			gpAR->ubMercCols--;
		if( gpAR->ubCivRows * gpAR->ubCivCols > gpAR->ubCivs + gpAR->ubCivRows )
			gpAR->ubCivCols--;
	}
}

void HandleAutoResolveInput()
{

DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Autoresolve3");

	InputAtom InputEvent;
	BOOLEAN fResetAutoResolve = FALSE;

	while( DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT) )
	{
		if( InputEvent.usEvent == KEY_DOWN || InputEvent.usEvent == KEY_REPEAT )
		{
			switch( InputEvent.usParam )
			{
				case SPACE:
					gpAR->fPaused ^= TRUE;
					if( gpAR->fPaused )
					{
						ButtonList[ gpAR->iButton[ PAUSE_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
						ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
						ButtonList[ gpAR->iButton[ FAST_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
						ButtonList[ gpAR->iButton[ FINISH_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
					}
					else
					{
						ButtonList[ gpAR->iButton[ PAUSE_BUTTON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
						ButtonList[ gpAR->iButton[ PLAY_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
					}
					break;
				case 'x':
					if( _KeyDown( ALT ) )
					{
						HandleShortCutExitState( );
					}
					break;
				#ifdef JA2BETAVERSION
				case 'c':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->fAllowCapture ^= TRUE;
						gpAR->fPlayerRejectedSurrenderOffer = FALSE;
						gStrategicStatus.uiFlags &= ~STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
						gpAR->fRenderAutoResolve = TRUE;
					}
					break;
				case F5:
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->fDebugInfo ^= TRUE;
					}
					break;
				case F6:
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->fSound ^= TRUE;
					}
					break;
				case F7:
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->fInstantFinish ^= TRUE;
					}
					break;
				// WDS - Debug "Drassen" battles
				case F12:
					if( CHEATER_CHEAT_LEVEL() )
					{
						UINT8 ubMapX, ubMapY;
						for ( ubMapX = 1; ubMapX < MAP_WORLD_X - 1; ubMapX++ ) {
							for ( ubMapY = 1; ubMapY < MAP_WORLD_Y - 1; ubMapY++ ) {
							SetSectorFlag( ubMapX, ubMapY, 0, SF_ALREADY_VISITED ); //hayden
							}
						}
						ExecuteStrategicAIAction( NPC_ACTION_SEND_SOLDIERS_TO_DRASSEN, 13, 4 );
					}
					break;
				case BACKSPACE:
					if( CHEATER_CHEAT_LEVEL() )
					{
						fResetAutoResolve = TRUE;
					}
					break;
				case 'd':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( _KeyDown( ALT ) )
						{
							if ( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
							{
								PlayAutoResolveSample( EXPLOSION_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
								EliminateAllFriendlies();
							}
						}
					}
					break;
				case 'z':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( _KeyDown( ALT ) )
						{
							if ( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
							{
								PlayAutoResolveSample( EXPLOSION_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
								EliminateAllMercs();
							}
						}
					}
					break;
				case 'o':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( _KeyDown( ALT ) )
						{
							if ( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
							{
								PlayAutoResolveSample( EXPLOSION_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
								// this is not very accurate, any enemies already dead will be counted as killed twice
								gStrategicStatus.usPlayerKills += NumNonPlayerTeamMembersInSector( gpAR->ubSectorX, gpAR->ubSectorY, ENEMY_TEAM );
								EliminateAllEnemies( gpAR->ubSectorX, gpAR->ubSectorY );
							}
						}
					}
					break;
				case '{':
					if (_KeyDown( ALT) )
					{
					}
					else if (_KeyDown( CTRL) )
					{
					}
					else if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubMercs = 0;
						fResetAutoResolve = TRUE;
					}
					break;
				case '}':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubMercs = MAX_AR_TEAM_SIZE;
						fResetAutoResolve = TRUE;
					}
					break;
				case '[':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubMercs )
						{
							gpAR->ubMercs--;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case ']':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubMercs < MAX_AR_TEAM_SIZE )
						{
							gpAR->ubMercs++;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case ':':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubCivs = 0;
						fResetAutoResolve = TRUE;
					}
					break;
				case '"':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubCivs = MAX_AR_TEAM_SIZE;
						fResetAutoResolve = TRUE;
					}
					break;
				case ';':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubCivs )
						{
							gpAR->ubCivs--;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case 39: // ' quote
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubCivs < MAX_AR_TEAM_SIZE )
						{
							gpAR->ubCivs++;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case '<':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubEnemies = 1;
						fResetAutoResolve = TRUE;
					}
					break;
				case '>':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubEnemies = gGameExternalOptions.ubGameMaximumNumberOfEnemies;
						fResetAutoResolve = TRUE;
					}
					break;
				case ',':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubEnemies > 1 )
						{
							gpAR->ubEnemies--;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case '.':
					if( CHEATER_CHEAT_LEVEL() )
					{
						if( gpAR->ubEnemies < gGameExternalOptions.ubGameMaximumNumberOfEnemies )
						{
							gpAR->ubEnemies++;
							fResetAutoResolve = TRUE;
						}
					}
					break;
				case '/':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubMercs = 1;
						gpAR->ubCivs = 0;
						gpAR->ubEnemies = 1;
						fResetAutoResolve = TRUE;
					}
					break;
				case '?':
					if( CHEATER_CHEAT_LEVEL() )
					{
						gpAR->ubMercs = OUR_TEAM_SIZE_NO_VEHICLE;
						gpAR->ubCivs = MAX_AR_TEAM_SIZE;
						gpAR->ubEnemies = gGameExternalOptions.ubGameMaximumNumberOfEnemies;
						fResetAutoResolve = TRUE;
					}
					break;
				#endif
			}
		}
	}
	if( fResetAutoResolve )
	{
		ResetAutoResolveInterface();
	}
}

void RenderSoldierCellHealth( SOLDIERCELL *pCell )
{
	INT32 cnt, cntStart;
	INT32 xp, yp;
	STR16 pStr;
	CHAR16 str[20];
	UINT8	*pDestBuf, *pSrcBuf;
	UINT32 uiSrcPitchBYTES, uiDestPitchBYTES;
	UINT16 usColor;

	SetFont( SMALLCOMPFONT );
	//Restore the background before drawing text.
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	pSrcBuf = LockVideoSurface( gpAR->iInterfaceBuffer, &uiSrcPitchBYTES );
	xp = pCell->xp + 2;
	yp = pCell->yp + 32;
	Blt16BPPTo16BPP( (UINT16*)pDestBuf, uiDestPitchBYTES,
			(UINT16*)pSrcBuf, uiSrcPitchBYTES, xp, yp, xp-gpAR->Rect.iLeft, yp-gpAR->Rect.iTop, 46, 10 );
	UnLockVideoSurface( gpAR->iInterfaceBuffer );
	UnLockVideoSurface( FRAME_BUFFER );

	if( pCell->pSoldier->stats.bLife )
	{
		if( pCell->pSoldier->stats.bLife == pCell->pSoldier->stats.bLifeMax )
		{
			cntStart = 4;
		}
		else
		{
			cntStart = 0;
		}
		for( cnt = cntStart; cnt < 6; cnt ++ )
		{
			if( pCell->pSoldier->stats.bLife < bHealthStrRanges[ cnt ] )
			{
				break;
			}
		}
		switch( cnt )
		{
			case 0: //DYING
			case 1: //CRITICAL
				usColor = FONT_RED;
				break;
			case 2: //WOUNDED
			case 3: //POOR
				usColor = FONT_YELLOW;
				break;
			default: //REST
				usColor = FONT_GRAY1;
				break;
		}
		if( cnt > 3 && pCell->pSoldier->stats.bLife != pCell->pSoldier->stats.bLifeMax )
		{ //Merc has taken damage, even though his life if good.
			usColor = FONT_YELLOW;
		}
		if( pCell->pSoldier->stats.bLife == pCell->pSoldier->stats.bLifeMax )
			usColor = FONT_GRAY1;
		pStr = zHealthStr[ cnt ];
	}
	else
	{
		wcscpy( str, pCell->pSoldier->name );
		pStr = _wcsupr( str );
		usColor = FONT_BLACK;
	}

	//Draw the retreating text, if applicable
	if( pCell->uiFlags & CELL_RETREATED && gpAR->ubBattleStatus != BATTLE_VICTORY )
	{
		usColor = FONT_LTGREEN;
		swprintf( str, gpStrategicString[ STR_AR_MERC_RETREATED ] );
		pStr = str;
	}
	else if( pCell->uiFlags & CELL_RETREATING && gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
	{
		if( pCell->pSoldier->stats.bLife >= OKLIFE )
		{ //Retreating is shared with the status string.	Alternate between the
			//two every 450 milliseconds
			if( GetJA2Clock() % 900 < 450 )
			{ //override the health string with the retreating string.
				usColor = FONT_LTRED;
				swprintf( str, gpStrategicString[ STR_AR_MERC_RETREATING ] );
				pStr = str;
			}
		}
	}
	else if( pCell->uiFlags & CELL_SHOWRETREATTEXT && gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
	{
		if( pCell->pSoldier->stats.bLife >= OKLIFE )
		{
			SetFontForeground( FONT_YELLOW );
			swprintf( str, gpStrategicString[ STR_AR_MERC_RETREAT ] );
			xp = pCell->xp + 25 - StringPixLength( pStr, SMALLCOMPFONT ) / 2;
			yp = pCell->yp + 12;
			mprintf( xp, yp, str );
		}
	}
	SetFontForeground( (UINT8)usColor );
	xp = pCell->xp + 25 - StringPixLength( pStr, SMALLCOMPFONT ) / 2;
	yp = pCell->yp + 33;
	mprintf( xp, yp, pStr );
}

UINT8 GetUnusedMercProfileID()
{
	UINT8 ubRandom=0;
	INT32 i;
	BOOLEAN fUnique = FALSE;
	while( !fUnique )
	{
		ubRandom = (UINT8)PreRandom( 40 );
		for( i = 0; i < 19; i++ )
		{
			fUnique = TRUE;
			if( Menptr[ i ].ubProfile == ubRandom )
			{
				fUnique = FALSE;
				break;
			}
		}
	}
	return ubRandom;
}

void CreateTempPlayerMerc()
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	static INT32		iSoldierCount=0;
	UINT8							ubID;

	//Init the merc create structure with basic information
	MercCreateStruct.bTeam									= SOLDIER_CREATE_AUTO_TEAM;
	MercCreateStruct.ubProfile							= GetUnusedMercProfileID();
	MercCreateStruct.sSectorX								= gpAR->ubSectorX;
	MercCreateStruct.sSectorY								= gpAR->ubSectorY;
	MercCreateStruct.bSectorZ								= 0;
	MercCreateStruct.fPlayerMerc						= TRUE;
	MercCreateStruct.fCopyProfileItemsOver	= TRUE;

	//Create the player soldier

	gpMercs[ gpAR->iNumMercFaces ].pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );
	if( gpMercs[ gpAR->iNumMercFaces ].pSoldier )
	{
		gpAR->iNumMercFaces++;
	}
}

void DetermineTeamLeader( BOOLEAN fFriendlyTeam )
{
	INT32 i;
	SOLDIERCELL *pBestLeaderCell = NULL;
	//For each team (civs and players count as same team), find the merc with the best
	//leadership ability.
	if( fFriendlyTeam )
	{
		gpAR->ubPlayerLeadership = 0;
		for( i = 0; i < gpAR->ubMercs; i++ )
		{
			if( gpMercs[ i ].pSoldier->stats.bLeadership > gpAR->ubPlayerLeadership )
			{
				gpAR->ubPlayerLeadership = gpMercs[ i ].pSoldier->stats.bLeadership;
				pBestLeaderCell = &gpMercs[ i ];
			}
		}
		for( i = 0; i < gpAR->ubCivs; i++ )
		{
			if( gpCivs[ i ].pSoldier->stats.bLeadership > gpAR->ubPlayerLeadership )
			{
				gpAR->ubPlayerLeadership = gpCivs[ i ].pSoldier->stats.bLeadership;
				pBestLeaderCell = &gpCivs[ i ];
			}
		}

		if( pBestLeaderCell )
		{
			//Assign the best leader the honour of team leader.
			pBestLeaderCell->uiFlags |= CELL_TEAMLEADER;
		}
		return;
	}
	//ENEMY TEAM
	gpAR->ubEnemyLeadership = 0;
	for( i = 0; i < gpAR->ubEnemies; i++ )
	{
		if( gpEnemies[ i ].pSoldier->stats.bLeadership > gpAR->ubEnemyLeadership )
		{
			gpAR->ubEnemyLeadership = gpEnemies[ i ].pSoldier->stats.bLeadership;
			pBestLeaderCell = &gpEnemies[ i ];
		}
	}
	if( pBestLeaderCell )
	{
		//Assign the best enemy leader the honour of team leader
		pBestLeaderCell->uiFlags |= CELL_TEAMLEADER;
	}
}

void ResetNextAttackCounter( SOLDIERCELL *pCell )
{
	pCell->usNextAttack = min( 1000 - pCell->usAttack, 800 );
	pCell->usNextAttack = (UINT16)(1000 + pCell->usNextAttack * 5 + PreRandom( 2000 - pCell->usAttack ) );
	if( pCell->uiFlags & CELL_CREATURE )
	{
		pCell->usNextAttack = pCell->usNextAttack * 8 / 10;
	}
}

FLOAT CalcClassBonusOrPenalty( SOLDIERTYPE *pSoldier )
{
	switch( pSoldier->ubSoldierClass )
	{
	case SOLDIER_CLASS_ELITE:
	case SOLDIER_CLASS_ELITE_MILITIA:
		return 1.3f;
	case SOLDIER_CLASS_ARMY:
	case SOLDIER_CLASS_REG_MILITIA:
		return 1.0f;
	case SOLDIER_CLASS_ADMINISTRATOR:
	case SOLDIER_CLASS_GREEN_MILITIA:
	case SOLDIER_CLASS_BANDIT:
		return 0.7f;
	}

	return 1.0f;
}

void CalculateAttackValues()
{
	INT32 i;
	SOLDIERCELL *pCell;
	SOLDIERTYPE *pSoldier;
	UINT16 usBonus;
	UINT16 usBestAttack = 0xffff;
	UINT16 usBreathStrengthPercentage;
	//INT16 sOutnumberBonus = 0;
	//PLAYER TEAM
	gpAR->usPlayerAttack = 0;
	gpAR->usPlayerDefence = 0;

	UINT8 uiEffectiveLevelExp = 0; // added by SANDRO

	//if( gpAR->ubEnemies )
	//{
	//	//bonus equals 20 if good guys outnumber bad guys 2 to 1.
	//	sMaxBonus = 20;
	//	sOutnumberBonus = (INT16)(gpAR->ubMercs + gpAR->ubCivs) * sMaxBonus / gpAR->ubEnemies - sMaxBonus;
	//	sOutnumberBonus = (INT16)min( sOutnumberBonus, max( sMaxBonus, 0 ) );
	//}

	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		pCell = &gpMercs[ i ];
		pSoldier = pCell->pSoldier;
		if( !pSoldier->stats.bLife )
			continue;
		pCell->usAttack =		pSoldier->stats.bStrength +
												pSoldier->stats.bDexterity +
												pSoldier->stats.bWisdom +
												pSoldier->stats.bMarksmanship +
												pSoldier->aiData.bMorale;
		//Give player controlled mercs a significant bonus to compensate for lack of control
		//as the player would typically do much better in tactical.
		if( pCell->usAttack < 1000 )
		{ //A player with 500 attack will be augmented to 625
			//A player with 600 attack will be augmented to 700
			pCell->usAttack = (UINT16)(pCell->usAttack + (1000 - pCell->usAttack) / 4);
		}
		usBreathStrengthPercentage = 100 - ( 100 - pCell->pSoldier->bBreathMax ) / 3;
		pCell->usAttack =		pCell->usAttack * usBreathStrengthPercentage / 100;
		pCell->usDefence =	pSoldier->stats.bAgility +
												pSoldier->stats.bWisdom +
												pSoldier->bBreathMax +
												pSoldier->stats.bMedical +
												pSoldier->aiData.bMorale;
		//100 team leadership adds a bonus of 10%,
		usBonus = 100 + gpAR->ubPlayerLeadership/10;// + sOutnumberBonus;

		//bExpLevel adds a bonus of 7% per level after 2, level 1 soldiers get a 7% decrease
		//usBonus += 7 * (pSoldier->stats.bExpLevel-2);
		// SANDRO - STOMP traits - Squadleaders bonus to effective level
		uiEffectiveLevelExp = pSoldier->stats.bExpLevel;
		if ( gGameOptions.fNewTraitSystem )
			uiEffectiveLevelExp = min(10,(uiEffectiveLevelExp + (gSkillTraitValues.ubSLEffectiveLevelInRadius * GetSquadleadersCountInVicinity( pSoldier, TRUE, TRUE ))));
		usBonus += EXP_BONUS * (uiEffectiveLevelExp-5);

		usBonus += gpAR->ubPlayerDefenceAdvantage;
		pCell->usAttack = pCell->usAttack * usBonus / 100;
		pCell->usDefence = pCell->usDefence * usBonus / 100;

		if( pCell->uiFlags & CELL_EPC )
		{ //strengthen the defense (seeing the mercs will keep them safe).
			pCell->usAttack = 0;
			pCell->usDefence = 1000;
		}

		pCell->usAttack = min( pCell->usAttack, 1000 );
		pCell->usDefence = min( pCell->usDefence, 1000 );

		gpAR->usPlayerAttack += pCell->usAttack;
		gpAR->usPlayerDefence += pCell->usDefence;
		ResetNextAttackCounter( pCell );
		if( i > 8 )
		{ //Too many mercs, delay attack entry of extra mercs.
			pCell->usNextAttack += (UINT16)( ( i - 8 ) * 2000 );
		}
		if( pCell->usNextAttack < usBestAttack )
			usBestAttack = pCell->usNextAttack;
	}
	//CIVS
	for( i = 0; i < gpAR->ubCivs; i++ )
	{
		pCell = &gpCivs[ i ];
		pSoldier = pCell->pSoldier;
		pCell->usAttack =		pSoldier->stats.bStrength +
												pSoldier->stats.bDexterity +
												pSoldier->stats.bWisdom +
												pSoldier->stats.bMarksmanship +
												pSoldier->aiData.bMorale;
		pCell->usAttack =		pCell->usAttack * pSoldier->bBreath / 100;
		pCell->usDefence =	pSoldier->stats.bAgility +
												pSoldier->stats.bWisdom +
												pSoldier->bBreathMax +
												pSoldier->stats.bMedical +
												pSoldier->aiData.bMorale;
		//100 team leadership adds a bonus of 10%
		usBonus = 100 + gpAR->ubPlayerLeadership/10;// + sOutnumberBonus;
		//bExpLevel adds a bonus of 7% per level after 2, level 1 soldiers get a 7% decrease
		//usBonus += 7 * (pSoldier->stats.bExpLevel-2);
		// SANDRO - STOMP traits - Squadleaders bonus to effective level
		uiEffectiveLevelExp = pSoldier->stats.bExpLevel;
		if ( gGameOptions.fNewTraitSystem )
			uiEffectiveLevelExp = min(10,(uiEffectiveLevelExp + (gSkillTraitValues.ubSLEffectiveLevelInRadius * GetSquadleadersCountInVicinity( pSoldier, TRUE, TRUE ))));
		usBonus += EXP_BONUS * (uiEffectiveLevelExp-5);

		usBonus += gpAR->ubPlayerDefenceAdvantage;
		pCell->usAttack = pCell->usAttack * usBonus / 100;
		pCell->usDefence = pCell->usDefence * usBonus / 100;

		pCell->usAttack = (UINT16)( pCell->usAttack * CalcClassBonusOrPenalty( pCell->pSoldier ) );
		pCell->usDefence =(UINT16)( pCell->usDefence * CalcClassBonusOrPenalty( pCell->pSoldier ) );

		pCell->usAttack = min( pCell->usAttack, 1000 );
		pCell->usDefence = min( pCell->usDefence, 1000 );

		gpAR->usPlayerAttack += pCell->usAttack;
		gpAR->usPlayerDefence += pCell->usDefence;
		ResetNextAttackCounter( pCell );
		if( i > 6 )
		{
			//Too many militia, delay attack entry of extra mercs.
			pCell->usNextAttack += (UINT32)(( i - 4 ) * 2000 );
		}

		UINT8 militiainsector = NumNonPlayerTeamMembersInSector( gpAR->ubSectorX, gpAR->ubSectorY, MILITIA_TEAM );
		if ( i >= militiainsector )
		{
			//Extra delay if soldier's from reinforcement
			pCell->usNextAttack += REINFORCMENT_ATTACK_DELAY_PER_SOLDIER_IN_SECTOR * militiainsector;
		}

		if( pCell->usNextAttack < usBestAttack )
			usBestAttack = pCell->usNextAttack;
	}

	//ENEMIES
	gpAR->usEnemyAttack = 0;
	gpAR->usEnemyDefence = 0;
	//if( gpAR->ubMercs + gpAR->ubCivs )
	//{
	//	//bonus equals 20 if good guys outnumber bad guys 2 to 1.
	//	sMaxBonus = 20;
	//	sOutnumberBonus = (INT16)gpAR->ubEnemies * sMaxBonus / (gpAR->ubMercs + gpAR->ubCivs) - sMaxBonus;
	//	sOutnumberBonus = (INT16)min( sOutnumberBonus, max( sMaxBonus, 0 ) );
	//}

	for( i = 0; i < gpAR->ubEnemies; ++i )
	{
		pCell = &gpEnemies[ i ];
		pSoldier = pCell->pSoldier;
		pCell->usAttack =		pSoldier->stats.bStrength +
												pSoldier->stats.bDexterity +
												pSoldier->stats.bWisdom +
												pSoldier->stats.bMarksmanship +
												pSoldier->aiData.bMorale;
		pCell->usAttack =		pCell->usAttack * pSoldier->bBreath / 100;
		pCell->usDefence =	pSoldier->stats.bAgility +
												pSoldier->stats.bWisdom +
												pSoldier->bBreathMax +
												pSoldier->stats.bMedical +
												pSoldier->aiData.bMorale;
		//100 team leadership adds a bonus of 10%
		usBonus = 100 + gpAR->ubPlayerLeadership/10;// + sOutnumberBonus;
		//bExpLevel adds a bonus of 7% per level after 2, level 1 soldiers get a 7% decrease
		//usBonus += 7 * (pSoldier->stats.bExpLevel-2);
		// SANDRO - STOMP traits - Squadleaders bonus to effective level
		uiEffectiveLevelExp = pSoldier->stats.bExpLevel;
		if ( gGameOptions.fNewTraitSystem )
			uiEffectiveLevelExp = min(10,(uiEffectiveLevelExp + (gSkillTraitValues.ubSLEffectiveLevelInRadius * GetSquadleadersCountInVicinity( pSoldier, TRUE, TRUE ))));
		usBonus += EXP_BONUS * (uiEffectiveLevelExp-5);

		usBonus += gpAR->ubEnemyDefenceAdvantage;
		pCell->usAttack	= pCell->usAttack * usBonus / 100;
		pCell->usDefence = pCell->usDefence * usBonus / 100;
		pCell->usAttack	= (UINT16)( pCell->usAttack * CalcClassBonusOrPenalty( pCell->pSoldier ) );
		pCell->usDefence = (UINT16)( pCell->usDefence * CalcClassBonusOrPenalty( pCell->pSoldier ) );

		pCell->usAttack = min( pCell->usAttack, 1000 );
		pCell->usDefence = min( pCell->usDefence, 1000 );

		gpAR->usEnemyAttack += pCell->usAttack;
		gpAR->usEnemyDefence += pCell->usDefence;
		ResetNextAttackCounter( pCell );

		if( i > 4 && !(pCell->uiFlags & CELL_CREATURE) )
		{
			//Too many enemies, delay attack entry of extra mercs.
			pCell->usNextAttack += (UINT32)( ( i - 4 ) * 1000 );
		}

		if ( i >= NumNonPlayerTeamMembersInSector( gpAR->ubSectorX, gpAR->ubSectorY, ENEMY_TEAM ) && !(pCell->uiFlags & CELL_CREATURE) )
		{
			//Extra delay if it's a reinforcement
			pCell->usNextAttack += REINFORCMENT_ATTACK_DELAY_PER_SOLDIER_IN_SECTOR * NumNonPlayerTeamMembersInSector( gpAR->ubSectorX, gpAR->ubSectorY, ENEMY_TEAM );
		}
		
		if( pCell->usNextAttack < usBestAttack )
			usBestAttack = pCell->usNextAttack;
	}

	//Now, because we are starting a new battle, we want to get the ball rolling a bit earlier.	So,
	//we will take the usBestAttack value and subtract 60% of it from everybody's next attack.
	usBestAttack = usBestAttack * 60 / 100;
	for( i = 0; i < gpAR->ubMercs; i++ )
		gpMercs[ i ].usNextAttack -= usBestAttack;
	for( i = 0; i < gpAR->ubCivs; i++ )
		gpCivs[ i ].usNextAttack -= usBestAttack;
	for( i = 0; i < gpAR->ubEnemies; i++ )
		gpEnemies[ i ].usNextAttack -= usBestAttack;
}

void DrawDebugText( SOLDIERCELL *pCell )
{
	INT32 xp, yp;
	if( !gpAR->fDebugInfo )
		return;
	SetFont( SMALLCOMPFONT );
	SetFontForeground( FONT_WHITE );
	xp = pCell->xp + 4;
	yp = pCell->yp + 4;
	if( pCell->uiFlags & CELL_TEAMLEADER )
	{
		//debug str
		mprintf( xp, yp, L"LEADER" );
		yp += 9;
	}
	mprintf( xp, yp, L"AT: %d", pCell->usAttack );
	yp += 9;
	mprintf( xp, yp, L"DF: %d", pCell->usDefence );
	yp += 9;

	xp = pCell->xp;
	yp = pCell->yp - 4;
	SetFont( LARGEFONT1 );
	SetFontShadow( FONT_NEARBLACK );
	if( pCell->uiFlags & CELL_FIREDATTARGET )
	{
		SetFontForeground( FONT_YELLOW );
		mprintf( xp, yp, L"FIRE" );
		pCell->uiFlags &= ~CELL_FIREDATTARGET;
		yp += 13;
	}
	if( pCell->uiFlags & CELL_DODGEDATTACK )
	{
		SetFontForeground( FONT_BLUE );
		mprintf( xp, yp, L"MISS" );
		pCell->uiFlags &= ~CELL_DODGEDATTACK;
		yp += 13;
	}
	if( pCell->uiFlags & CELL_HITBYATTACKER )
	{
		SetFontForeground( FONT_RED );
		mprintf( xp, yp, L"HIT" );
		pCell->uiFlags &= ~CELL_HITBYATTACKER;
		yp += 13;
	}
}

SOLDIERCELL* ChooseTarget( SOLDIERCELL *pAttacker )
{
	INT32 iAvailableTargets;
	INT32 index;
	INT32 iRandom = -1;
	SOLDIERCELL *pTarget = NULL;
	UINT32 usSavedDefence;

	//Determine what team we are attacking
	if( pAttacker->uiFlags & (CELL_ENEMY | CELL_CREATURE) )
	{
		//enemy team attacking a player
		iAvailableTargets = gpAR->ubMercs + gpAR->ubCivs;
		index = 0;
		usSavedDefence = gpAR->usPlayerDefence;
		while( iAvailableTargets )
		{
			pTarget = ( index < gpAR->ubMercs ) ? &gpMercs[ index ] : &gpCivs[ index - gpAR->ubMercs ];
			if( !pTarget->pSoldier->stats.bLife || pTarget->uiFlags & CELL_RETREATED )
			{
				index++;
				iAvailableTargets--;
				continue;
			}
			iRandom = PreRandom( gpAR->usPlayerDefence );
			gpAR->usPlayerDefence -= pTarget->usDefence;
			if( iRandom < pTarget->usDefence )
			{
				gpAR->usPlayerDefence = usSavedDefence;
				return pTarget;
			}
			index++;
			iAvailableTargets--;
		}
		if( !IsBattleOver() )
		{
			AssertMsg( 0, String("***Please send PRIOR save and screenshot of this message***	iAvailableTargets %d, index %d, iRandom %d, defence %d. ",
				iAvailableTargets, index, iRandom, gpAR->usPlayerDefence) );
		}
	}
	else
	{
		//player team attacking an enemy
		iAvailableTargets = gpAR->ubEnemies;
		index = 0;
		usSavedDefence = gpAR->usEnemyDefence;
		while( iAvailableTargets )
		{
			pTarget = &gpEnemies[ index ];
			if( !pTarget->pSoldier->stats.bLife )
			{
				index++;
				iAvailableTargets--;
				continue;
			}
			iRandom = PreRandom( gpAR->usEnemyDefence );
			gpAR->usEnemyDefence -= pTarget->usDefence;
			if( iRandom < pTarget->usDefence )
			{
				gpAR->usEnemyDefence = usSavedDefence;
				return pTarget;
			}
			index++;
			iAvailableTargets--;
		}
	}
	AssertMsg( 0, "Error in ChooseTarget logic for choosing enemy target." );
	return NULL;
}

BOOLEAN FireAShot( SOLDIERCELL *pAttacker )
{
	OBJECTTYPE *pItem;
	SOLDIERTYPE *pSoldier;

	pSoldier = pAttacker->pSoldier;

	if( pAttacker->uiFlags & CELL_MALECREATURE )
	{
		PlayAutoResolveSample( ACR_SPIT, RATE_11025, 50, 1, MIDDLEPAN );
		pAttacker->bWeaponSlot = SECONDHANDPOS;
		return TRUE;
	}

	UINT8 invsize = pSoldier->inv.size();
	for( UINT8 i = 0; i < invsize; ++i )
	{
		pItem = &pSoldier->inv[ i ];

		if( Item[ pItem->usItem ].usItemClass == IC_GUN )
		{
			pAttacker->bWeaponSlot = (INT8)i;
			if( gpAR->fUnlimitedAmmo )
			{
				PlayAutoResolveSample( Weapon[ pItem->usItem ].sSound, RATE_11025, 50, 1, MIDDLEPAN );
				return TRUE;
			}
			if( !(*pItem)[0]->data.gun.ubGunShotsLeft )
			{
				AutoReload( pSoldier );
				if ( (*pItem)[0]->data.gun.ubGunShotsLeft && Weapon[ pItem->usItem ].sLocknLoadSound )
				{
					PlayAutoResolveSample( Weapon[ pItem->usItem ].sLocknLoadSound, RATE_11025, 50, 1, MIDDLEPAN );
				}
			}
			if( (*pItem)[0]->data.gun.ubGunShotsLeft )
			{
				PlayAutoResolveSample( Weapon[ pItem->usItem ].sSound, RATE_11025, 50, 1, MIDDLEPAN );
				if( pAttacker->uiFlags & CELL_MERC )
				{
					gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usShotsFired++;
					// MARKSMANSHIP GAIN: Attacker fires a shot

					StatChange( pAttacker->pSoldier, MARKAMT, 3, FALSE );
				}
				(*pItem)[0]->data.gun.ubGunShotsLeft--;
				return TRUE;
			}
		}
	}
	pAttacker->bWeaponSlot = -1;
	return FALSE;
}

BOOLEAN FireTankCannon( SOLDIERCELL *pAttacker )
{
	OBJECTTYPE *pItem;
	SOLDIERTYPE *pSoldier;

	pSoldier = pAttacker->pSoldier;

	// we need to be a tank to do this...
	if ( !pSoldier || (pAttacker->uiFlags & CELL_TANK) == 0 || pSoldier->ubSoldierClass != SOLDIER_CLASS_TANK || COMBAT_JEEP( pSoldier ) )
		return FALSE;

	// a tank has several guns, don't fire the main gun all the time
	//if ( Chance(33) )
		//return FALSE;
	
	UINT8 invsize = pSoldier->inv.size();
	for ( UINT8 i = 0; i < invsize; ++i )
	{
		pItem = &pSoldier->inv[i];

		if ( Item[pItem->usItem].cannon )
		{
			PlayAutoResolveSample( Weapon[pItem->usItem].sSound, RATE_11025, 50, 1, MIDDLEPAN );

			if ( pAttacker->uiFlags & CELL_MERC )
			{
				gMercProfiles[pAttacker->pSoldier->ubProfile].records.usShotsFired++;
				// MARKSMANSHIP GAIN: Attacker fires a shot

				StatChange( pAttacker->pSoldier, MARKAMT, 3, FALSE );
			}

			pAttacker->bWeaponSlot = (INT8)i;

			return TRUE;			
		}
	}

	pAttacker->bWeaponSlot = -1;
	return FALSE;
}

BOOLEAN FireAntiTankWeapon( SOLDIERCELL *pAttacker )
{
	OBJECTTYPE *pItem;
	SOLDIERTYPE *pSoldier;

	pSoldier = pAttacker->pSoldier;
	
	UINT8 invsize = pSoldier->inv.size( );
	for ( UINT8 i = 0; i < invsize; ++i )
	{
		pItem = &pSoldier->inv[i];

		if ( Item[pItem->usItem].usItemClass == IC_LAUNCHER || Item[pItem->usItem].cannon )
		{
			pAttacker->bWeaponSlot = (INT8)i;
			if ( gpAR->fUnlimitedAmmo )
			{
				PlayAutoResolveSample( Weapon[pItem->usItem].sSound, RATE_11025, 50, 1, MIDDLEPAN );
				return TRUE;
			}

			if ( FindLaunchable( pSoldier, pSoldier->inv[pAttacker->bWeaponSlot].usItem ) == NO_SLOT )
				continue;

			DeductAmmo( pSoldier, pAttacker->bWeaponSlot );

			if ( !(*pItem)[0]->data.gun.ubGunShotsLeft )
			{
				INT8 ammoslot = FindAmmoToReload( pSoldier, pAttacker->bWeaponSlot, NO_SLOT );

				BOOLEAN fRet = ReloadGun( pSoldier, pItem, &(pSoldier->inv[ammoslot]) );

				if ( fRet && ( *pItem )[0]->data.gun.ubGunShotsLeft && Weapon[pItem->usItem].sLocknLoadSound )
				{
					PlayAutoResolveSample( Weapon[pItem->usItem].sLocknLoadSound, RATE_11025, 50, 1, MIDDLEPAN );
				}
			}
			
			PlayAutoResolveSample( Weapon[pItem->usItem].sSound, RATE_11025, 50, 1, MIDDLEPAN );
			if ( pAttacker->uiFlags & CELL_MERC )
			{
				gMercProfiles[pAttacker->pSoldier->ubProfile].records.usShotsFired++;
				// MARKSMANSHIP GAIN: Attacker fires a shot

				StatChange( pAttacker->pSoldier, MARKAMT, 3, FALSE );
			}
				
			return TRUE;
		}
	}

	pAttacker->bWeaponSlot = -1;
	return FALSE;
}

BOOLEAN AttackerHasKnife( SOLDIERCELL *pAttacker )
{
	UINT8 invsize = pAttacker->pSoldier->inv.size();
	for( UINT8 i = 0; i < invsize; ++i )
	{
		if( Item[ pAttacker->pSoldier->inv[ i ].usItem ].usItemClass == IC_BLADE )
		{
			pAttacker->bWeaponSlot = (INT8)i;
			return TRUE;
		}
	}
	pAttacker->bWeaponSlot = -1;
	return FALSE;
}

BOOLEAN TargetHasLoadedGun( SOLDIERTYPE *pSoldier )
{
	OBJECTTYPE *pItem;
	UINT8 invsize = pSoldier->inv.size();
	for( UINT8 i = 0; i < invsize; ++i )
	{
		pItem = &pSoldier->inv[ i ];
		if( Item[ pItem->usItem ].usItemClass == IC_GUN )
		{
			if( gpAR->fUnlimitedAmmo )
			{
				return TRUE;
			}
			if( (*pItem)[0]->data.gun.ubGunShotsLeft )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void AttackTarget( SOLDIERCELL *pAttacker, SOLDIERCELL *pTarget )
{
	INT16 sAttack;
	INT16 sDefence;
	UINT8 ubImpact;
	UINT8 ubLocation;
	INT16 sAccuracy;
	INT32 iRandom;
	INT32 iImpact;
	INT32 iNewLife;
	BOOLEAN fMelee = FALSE;
	BOOLEAN fKnife = FALSE;
	BOOLEAN fClaw = FALSE;
	BOOLEAN fCannon = FALSE;
	BOOLEAN fAntiTank = FALSE;
	INT8	bAttackIndex = -1;

	pAttacker->uiFlags |= CELL_FIREDATTARGET | CELL_DIRTY;
	if( pAttacker->usAttack < 950 )
		sAttack = (INT16)(pAttacker->usAttack + PreRandom(max(0, (INT16)(gGameExternalOptions.iAutoResolveLuckFactor*1000.0) - (INT16)pAttacker->usAttack)));
	else
		sAttack = 950 + PreRandom(50);

	if( pTarget->uiFlags & CELL_RETREATING && !(pAttacker->uiFlags & CELL_FEMALECREATURE) )
	{
		//Attacking a retreating merc is harder.	Modify the attack value to 70% of it's value.
		//This allows retreaters to have a better chance of escaping.
		sAttack = sAttack * 7 / 10;
	}

	if( pTarget->usDefence < 950 )
		sDefence = (INT16)(pTarget->usDefence + PreRandom(max(0, (INT16)(gGameExternalOptions.iAutoResolveLuckFactor*1000.0) - (INT16)pTarget->usDefence)));
	else
		sDefence = 950 + PreRandom(50);

	///////////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - Increase Militia Strength in autoresolve battles 
	// because the attack and defense is limited to max 1000, rather than only increasing the attack of militia,
	// decrease the defense of target - so +100% bonus means +50% attack of attacker and -50% defense of target

	//if militia is attacking increase attack by half and decrease the defense of enemy
	if (pAttacker->pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaAutoresolveStrength != 0)
	{
		sAttack += (sAttack * gGameExternalOptions.sGreenMilitiaAutoresolveStrength /200);
		sDefence -= (sDefence * gGameExternalOptions.sGreenMilitiaAutoresolveStrength /200);
	}
	else if (pAttacker->pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaAutoresolveStrength != 0)
	{
		sAttack += (sAttack * gGameExternalOptions.sRegularMilitiaAutoresolveStrength /200);
		sDefence -= (sDefence * gGameExternalOptions.sRegularMilitiaAutoresolveStrength /200);
	}
	else if (pAttacker->pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaAutoresolveStrength != 0)
	{
		sAttack += (sAttack * gGameExternalOptions.sVeteranMilitiaAutoresolveStrength /200);
		sDefence -= (sDefence * gGameExternalOptions.sVeteranMilitiaAutoresolveStrength /200);
	}
	else if (pAttacker->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveOffenseBonus != 0)
	{
		sAttack += (sAttack * gGameExternalOptions.sMercsAutoresolveOffenseBonus /200);
		sDefence -= (sDefence * gGameExternalOptions.sMercsAutoresolveOffenseBonus /200);
	}

	//if enemy is attacking decrease his attack by half and increase the defense of militia by half
	if (pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.sGreenMilitiaAutoresolveStrength != 0)
	{
		sDefence += (sDefence * gGameExternalOptions.sGreenMilitiaAutoresolveStrength /200);
		sAttack -= (sAttack * gGameExternalOptions.sGreenMilitiaAutoresolveStrength /200);
	}
	else if (pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.sRegularMilitiaAutoresolveStrength != 0)
	{
		sDefence += (sDefence * gGameExternalOptions.sRegularMilitiaAutoresolveStrength /200);
		sAttack -= (sAttack * gGameExternalOptions.sRegularMilitiaAutoresolveStrength /200);
	}
	else if (pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.sVeteranMilitiaAutoresolveStrength != 0)
	{
		sDefence += (sDefence * gGameExternalOptions.sVeteranMilitiaAutoresolveStrength /200);
		sAttack -= (sAttack * gGameExternalOptions.sVeteranMilitiaAutoresolveStrength /200);
	}
	else if (pTarget->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveDeffenseBonus != 0)
	{
		sDefence += (sDefence * gGameExternalOptions.sMercsAutoresolveDeffenseBonus /200);
		sAttack -= (sAttack * gGameExternalOptions.sMercsAutoresolveDeffenseBonus /200);
	}

	// check fortifications bonus
	if (pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA
		|| pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA
		|| pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA
		|| (pTarget->uiFlags & CELL_MERC))
	{
		sDefence += (sDefence * RebelCommand::GetFortificationsBonus(GetAutoResolveSectorID()) / 200);
		sAttack -= (sAttack * RebelCommand::GetFortificationsBonus(GetAutoResolveSectorID()) / 200);
	}

	// repair values
	sAttack = max(0, min(1000, sAttack ));
	sDefence = max(0, min(1000, sDefence ));
	///////////////////////////////////////////////////////////////////////////////////////////

	if( pAttacker->uiFlags & (CELL_FEMALECREATURE | CELL_BLOODCAT | CELL_ZOMBIE) )
	{
		pAttacker->bWeaponSlot = HANDPOS;
		fMelee = TRUE;

		if ( pAttacker->uiFlags & ( CELL_FEMALECREATURE | CELL_BLOODCAT ) )
			fClaw = TRUE;
	}
	// Flugente: if we are a tank, we might fire the main cannon...
	else if ( FireTankCannon( pAttacker ) )
	{
		fCannon = TRUE;

		// cannons have a huge splash zone, so they are much more likely to hit
		sAttack = (INT16)(sAttack * 1.5);
	}
	// if our target is a tank, we use heavy weapons if we have any
	else if ( (ARMED_VEHICLE( pTarget->pSoldier ) || ENEMYROBOT( pTarget->pSoldier )) && FireAntiTankWeapon( pAttacker ) )
	{
		fAntiTank = TRUE;

		// hitting a tank with an rpg isn't easy
		sAttack = (INT16)(sAttack * 0.8);
	}
	else if( !FireAShot( pAttacker ) )
	{
		//Maybe look for a weapon, such as a knife or grenade?
		fMelee = TRUE;
		fKnife = AttackerHasKnife( pAttacker );
		if( TargetHasLoadedGun( pTarget->pSoldier ) )
		{
			//Penalty to attack with melee weapons against target with loaded gun.
			if( !(pAttacker->uiFlags & CELL_CREATURE ) )
			{
				//except for creatures
				if( fKnife )
					sAttack = sAttack * 6 / 10;
				else
					sAttack = sAttack * 4 / 10;
			}
		}
	}

	//Set up a random delay for the hit or miss.
	if( !fMelee )
	{
		if( !pTarget->usNextHit[0] )
		{
			bAttackIndex = 0;
		}
		else if( !pTarget->usNextHit[1] )
		{
			bAttackIndex = 1;
		}
		else if( !pTarget->usNextHit[2] )
		{
			bAttackIndex = 2;
		}

		if ( bAttackIndex != -1 )
		{
			if ( fAntiTank )
				pTarget->usNextHit[ bAttackIndex ] = (UINT16)( 200 + PreRandom( 400 ) );
			else
			pTarget->usNextHit[ bAttackIndex ] = (UINT16)( 50 + PreRandom( 400 ) );

			pTarget->pAttacker[ bAttackIndex ] = pAttacker;
		}
	}

	if( sAttack < sDefence )
	{
		if( pTarget->pSoldier->stats.bLife >= OKLIFE || !PreRandom( 5 ) )
		{
			//Attacker misses -- use up a round of ammo.	If target is unconscious, then 80% chance of hitting.
			pTarget->uiFlags |= CELL_DODGEDATTACK | CELL_DIRTY;

			if( fMelee )
			{
				if( fKnife )
					PlayAutoResolveSample( MISS_KNIFE, RATE_11025, 50, 1, MIDDLEPAN );
				else if( fClaw )
				{
					if ( pAttacker->uiFlags & ( CELL_BLOODCAT ) )
					{
						if ( Chance( 50 ) )
						{
							PlayAutoResolveSample( BLOODCAT_ATTACK, RATE_11025, 50, 1, MIDDLEPAN );
						}
						else
						{
							PlayAutoResolveSample( BLOODCAT_ROAR, RATE_11025, 50, 1, MIDDLEPAN );
						}
					}
					else
					{
						if ( Chance( 50 ) )
						{
							PlayAutoResolveSample( ACR_SWIPE, RATE_11025, 50, 1, MIDDLEPAN );
						}
						else
						{
							PlayAutoResolveSample( ACR_LUNGE, RATE_11025, 50, 1, MIDDLEPAN );
						}
					}
				}
				else
					PlayAutoResolveSample( SWOOSH_1 + PreRandom( 6 ), RATE_11025, 50, 1, MIDDLEPAN );

				if( pTarget->uiFlags & CELL_MERC )
					// AGILITY GAIN: Target "dodged" an attack
					StatChange( pTarget->pSoldier, AGILAMT, 5, FALSE );
			}

			return;
		}
	}

	// hit with a tank
	if ( fCannon )
	{
		ubImpact = (UINT8)GetDamage( &pAttacker->pSoldier->inv[pAttacker->bWeaponSlot] ); 
		
		ubLocation = AIM_SHOT_TORSO;

		//ubAccuracy = (UINT8)((usAttack - usDefence + PreRandom( usDefence - pTarget->usDefence )) / 10);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - increased mercs' offense/defense rating
		if ( pAttacker->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveOffenseBonus != 0 )
		{
			ubImpact += (ubImpact * gGameExternalOptions.sMercsAutoresolveOffenseBonus / 150);
		}
		else if ( pTarget->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveDeffenseBonus != 0 && (ubImpact > 3) )
		{
			ubImpact = max( 3, ((ubImpact * (100 - (gGameExternalOptions.sMercsAutoresolveDeffenseBonus / 2))) / 100) );
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if ( bAttackIndex == -1 )
		{
			// tack damage on to end of last hit
			pTarget->usHitDamage[2] += (UINT16)ubImpact;
		}
		else
		{
			pTarget->usHitDamage[bAttackIndex] = (UINT16)ubImpact;
		}

		// tanks might do splash damage to other troops as well...
		UINT8 numtries = min(4, Random( gpAR->ubMercs + gpAR->ubCivs ) );
		for ( int tries = 0; tries < numtries; ++tries )
		{
			SOLDIERCELL* pAnotherTarget = ChooseTarget( pAttacker );

			if ( pAnotherTarget != pTarget )
			{
				INT8 bNewAttackIndex = -1;

				if ( !pTarget->usNextHit[0] )
				{
					bNewAttackIndex = 0;
				}
				else if ( !pTarget->usNextHit[1] )
				{
					bNewAttackIndex = 1;
				}
				else if ( !pTarget->usNextHit[2] )
				{
					bNewAttackIndex = 2;
				}

				if ( bAttackIndex != -1 )
				{
					pAnotherTarget->usNextHit[bNewAttackIndex] = pTarget->usNextHit[bAttackIndex];
					pAnotherTarget->pAttacker[bNewAttackIndex] = pAttacker;

					pAnotherTarget->usHitDamage[bNewAttackIndex] = ubImpact / (numtries + 1) + Random( 20 );
				}
			}
		}
	}
	// attack a tank with heavy weapons
	else if ( fAntiTank )
	{
		ubImpact = (UINT8)GetDamage( &pAttacker->pSoldier->inv[pAttacker->bWeaponSlot] ) / 3;

		UINT8 ubAmmoType = Magazine[Item[(&pAttacker->pSoldier->inv[pAttacker->bWeaponSlot])->usItem].ubClassIndex].ubAmmoType;
				
		ubImpact = (UINT8)(ubImpact * AmmoTypes[ubAmmoType].dDamageModifierTank);

		iRandom = Random( 100 );
		if ( iRandom < 15 )
			ubLocation = AIM_SHOT_HEAD;
		else if ( iRandom < 50 )
			ubLocation = AIM_SHOT_LEGS;
		else
			ubLocation = AIM_SHOT_TORSO;

		//ubAccuracy = (UINT8)((usAttack - usDefence + PreRandom( usDefence - pTarget->usDefence )) / 10);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - increased mercs' offense/defense rating
		if ( pAttacker->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveOffenseBonus != 0 )
		{
			ubImpact += (ubImpact * gGameExternalOptions.sMercsAutoresolveOffenseBonus / 150);
		}
		else if ( pTarget->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveDeffenseBonus != 0 && (ubImpact > 3) )
		{
			ubImpact = max( 3, ((ubImpact * (100 - (gGameExternalOptions.sMercsAutoresolveDeffenseBonus / 2))) / 100) );
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if ( bAttackIndex == -1 )
		{
			// tack damage on to end of last hit
			pTarget->usHitDamage[2] += (UINT16)ubImpact;
		}
		else
		{
			pTarget->usHitDamage[bAttackIndex] = (UINT16)ubImpact;
		}
	}
	//Attacker hits
	else if( !fMelee )
	{
		// silversurfer: We want to use the function instead of the raw value because it applies the modifiers that we configured in the ini.
		ubImpact = (UINT8) GetDamage(&pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ]);
		//ubImpact = Weapon[ pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ].usItem ].ubImpact;
		iRandom = PreRandom( 100 );
		if( iRandom < 15 )
			ubLocation = AIM_SHOT_HEAD;
		else if( iRandom < 30 )
			ubLocation = AIM_SHOT_LEGS;
		else
			ubLocation = AIM_SHOT_TORSO;
		
		if (sDefence >= pTarget->usDefence)
			iRandom = PreRandom(sDefence - pTarget->usDefence);
		else
			iRandom = -(INT16)PreRandom(pTarget->usDefence - sDefence);

		sAccuracy = (sAttack - sDefence + iRandom) / 10;

		// HEADROCK HAM 5: Added argument
		iImpact = BulletImpact( pAttacker->pSoldier, NULL, pTarget->pSoldier, ubLocation, ubImpact, sAccuracy, NULL );
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - increased mercs' offense/defense rating
		if (pAttacker->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveOffenseBonus != 0)
		{
			iImpact += (iImpact * gGameExternalOptions.sMercsAutoresolveOffenseBonus /150);
		}
		else if (pTarget->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveDeffenseBonus != 0 && (iImpact > 3))
		{
			iImpact = max( 3, ((iImpact * (100 - (gGameExternalOptions.sMercsAutoresolveDeffenseBonus / 2))) /100));
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if ( bAttackIndex == -1 )
		{
			// tack damage on to end of last hit
			pTarget->usHitDamage[2] += (UINT16) iImpact;
		}
		else
		{
			pTarget->usHitDamage[ bAttackIndex ] = (UINT16) iImpact;
		}
	}
	else
	{
		OBJECTTYPE *pItem;
		OBJECTTYPE tempItem;

		PlayAutoResolveSample( (UINT8)(BULLET_IMPACT_1+PreRandom(3)), RATE_11025, 50, 1, MIDDLEPAN );
		if( !pTarget->pSoldier->stats.bLife )
		{ //Soldier already dead (can't kill him again!)
			return;
		}

		if (sDefence >= pTarget->usDefence)
			iRandom = PreRandom(sDefence - pTarget->usDefence);
		else
			iRandom = -(INT16)PreRandom(pTarget->usDefence - sDefence);

		sAccuracy = (sAttack - sDefence + iRandom) / 10;

		//Determine attacking weapon.
		pAttacker->pSoldier->usAttackingWeapon = 0;
		if( pAttacker->bWeaponSlot != -1 )
		{
			pItem = &pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ];
			if( Item[ pItem->usItem ].usItemClass & IC_WEAPON )
				pAttacker->pSoldier->usAttackingWeapon = pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ].usItem;
		}

        // WDS - make number of mercenaries, etc. be configurable
		// WANNE: Does this lead to a CTD -> no I did not get any CTD, so I reenabled it
		if( pAttacker->bWeaponSlot != HANDPOS && pAttacker->bWeaponSlot != -1)
		{
			//switch items
			tempItem = pAttacker->pSoldier->inv[ HANDPOS ];
			pAttacker->pSoldier->inv[ HANDPOS ] = pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ]; //CTD
			iImpact = HTHImpact( pAttacker->pSoldier, pTarget->pSoldier, sAccuracy, (BOOLEAN)(fKnife || fClaw) );
			pAttacker->pSoldier->inv[ pAttacker->bWeaponSlot ] = pAttacker->pSoldier->inv[ HANDPOS ];
			pAttacker->pSoldier->inv[ HANDPOS ] = tempItem;
		}
		else
		{
			iImpact = HTHImpact( pAttacker->pSoldier, pTarget->pSoldier, sAccuracy, (BOOLEAN)(fKnife || fClaw) );
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - increased mercs' offense/deffense rating
		if (pAttacker->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveOffenseBonus != 0)
		{
			iImpact += (iImpact * gGameExternalOptions.sMercsAutoresolveOffenseBonus /150);
		}
		else if (pTarget->uiFlags & CELL_MERC && gGameExternalOptions.sMercsAutoresolveDeffenseBonus != 0 && (iImpact > 3))
		{
			iImpact = max( 3, ((iImpact * (100 - (gGameExternalOptions.sMercsAutoresolveDeffenseBonus / 2))) /100));
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// WANNE: Why is impact here always set to 0? The impact was calculated a few lines before!
		//iImpact = 0;
		if (gTacticalStatus.uiFlags & GODMODE && pTarget->pSoldier->bTeam == OUR_TEAM)
		{
			iImpact = 0;
		}

		// WANNE: Just for safety.
		if (iImpact < 0)
			iImpact = 0;
		
		iNewLife = pTarget->pSoldier->stats.bLife - iImpact;

		if( pAttacker->uiFlags & CELL_MERC )
		{ 
			//Attacker is a player, so increment the number of shots that hit.
			gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usShotsHit++;
			// MARKSMANSHIP GAIN: Attacker's shot hits
			StatChange( pAttacker->pSoldier, MARKAMT, 6, FALSE );		// in addition to 3 for taking a shot
		}
		if( pTarget->uiFlags & CELL_MERC )
		{ 
			//Target is a player, so increment the times he has been wounded.
			if( iImpact > 1 )
				gMercProfiles[ pTarget->pSoldier->ubProfile ].records.usTimesWoundedStabbed++;
			// EXPERIENCE GAIN: Took some damage
			StatChange( pTarget->pSoldier, EXPERAMT, ( UINT16 )( 5 * ( iImpact / 10 ) ), FALSE );
		}
		if( pTarget->pSoldier->stats.bLife >= CONSCIOUSNESS || pTarget->uiFlags & CELL_CREATURE )
		{
			if( gpAR->fSound )
				pTarget->pSoldier->DoMercBattleSound( BATTLE_SOUND_HIT1 );
		}
		if( !(pTarget->uiFlags & CELL_CREATURE) && iNewLife < OKLIFE && pTarget->pSoldier->stats.bLife >= OKLIFE )
		{
			//the hit caused the merc to fall.	Play the falling sound
			PlayAutoResolveSample( (UINT8)FALL_1, RATE_11025, 50, 1, MIDDLEPAN );
			pTarget->uiFlags &= ~CELL_RETREATING;
		}
		if( iNewLife <= 0 )
		{
			//soldier has been killed

			// Flugente: campaign stats
			gCurrentIncident.AddStat( pTarget->pSoldier, CAMPAIGNHISTORY_TYPE_KILL );

			// Flugente: individual militia
			MILITIA militia;
			if ( GetMilitia( pTarget->pSoldier->usIndividualMilitiaID, &militia ) && !(militia.flagmask & MILITIAFLAG_DEAD) )
			{
				militia.healthratio = 0.0f;
				militia.flagmask |= MILITIAFLAG_DEAD;

				// note the current incident (when closing the incident, we only do this for those still alive)
				MILITIA_BATTLEREPORT report;
				report.id = GetIdOfCurrentlyOngoingIncident( );
				report.flagmask = MILITIA_BATTLEREPORT_FLAG_DIED;

				if ( pTarget->pSoldier->ubMilitiaKills )
					report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY;

				militia.history.push_back( report );

				UpdateMilitia( militia );
			}

			// Flugente: for raids, we need to keep track of killed forces
			if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
				GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
				GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
			{
				AddRaidPersonnel( -( pTarget->pSoldier->ubBodyType == BLOODCAT ), -( pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_ZOMBIE ), -( pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_BANDIT ) );
			}
			
			if( pAttacker->uiFlags & CELL_MERC )
			{ 
				//Player killed the enemy soldier -- update his stats as well as any assisters.
				/////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - experimental - more specific statistics of mercs
				switch(pTarget->pSoldier->ubSoldierClass)
				{
					case SOLDIER_CLASS_ROBOT :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsOthers++;
						break;
					case SOLDIER_CLASS_ELITE :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsElites++;
						break;
					case SOLDIER_CLASS_ARMY :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsRegulars++;
						break;
					case SOLDIER_CLASS_ADMINISTRATOR :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsAdmins++;
						break;
					case SOLDIER_CLASS_CREATURE :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsCreatures++;
						break;
					case SOLDIER_CLASS_ZOMBIE :
						gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsZombies++;
						break;
					case SOLDIER_CLASS_BANDIT:
						gMercProfiles[pAttacker->pSoldier->ubProfile].records.usKillsOthers++;
						break;
					default :
						if ( CREATURE_OR_BLOODCAT( pTarget->pSoldier ) )
							gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsCreatures++;
						else if ( ARMED_VEHICLE( pTarget->pSoldier ) ) // we hardly can kill a tank in autoresolve, but well.. who knows..
							gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsTanks++;
						else if ( pTarget->pSoldier->bTeam == CIV_TEAM && !pTarget->pSoldier->aiData.bNeutral && pTarget->pSoldier->bSide != gbPlayerNum )
							gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsHostiles++;
						else
						{
							gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usKillsOthers++;

							// Flugente: dynamic opinions: if this guy is not hostile towards us, then some mercs will complain about killing civilians
							if ((gGameExternalOptions.fDynamicOpinions) && (pTarget->pSoldier->bTeam != OUR_TEAM) && (pTarget->pSoldier->aiData.bNeutral || pTarget->pSoldier->bSide == pAttacker->pSoldier->bSide) )
							{
								// not for killing animals though...
								if ( pTarget->pSoldier->ubBodyType != CROW && pTarget->pSoldier->ubBodyType != COW )
									HandleDynamicOpinionChange( pAttacker->pSoldier, OPINIONEVENT_CIVKILLER, TRUE, TRUE );
							}
						}
						break;
				}
				/////////////////////////////////////////////////////////////////////////////////////
				gStrategicStatus.usPlayerKills++;
			}
			else if( pAttacker->uiFlags & CELL_MILITIA )
			{
				pAttacker->pSoldier->ubMilitiaKills += 1;
			}
			if( pTarget->uiFlags & CELL_MERC && gpAR->fSound )
			{
				PlayAutoResolveSample( (UINT8)DOORCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				PlayAutoResolveSample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			}
		}
		///////////////////////////////////////////////////////////////
		// SANDRO - added the insta-healable value for doctor trait
		else if ((IS_MERC_BODY_TYPE( pTarget->pSoldier ) || IS_CIV_BODY_TYPE( pTarget->pSoldier )) && ( gGameOptions.fNewTraitSystem ))
		{
			pTarget->pSoldier->iHealableInjury += ((pTarget->pSoldier->stats.bLife - iNewLife) * 100);
			if (pTarget->pSoldier->iHealableInjury > ((pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife) * 100))
				pTarget->pSoldier->iHealableInjury = ((pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife) * 100);
		}
		///////////////////////////////////////////////////////////////
		//Adjust the soldiers stats based on the damage.
		pTarget->pSoldier->stats.bLife = (INT8)max( iNewLife, 0 );
		if( pTarget->uiFlags & CELL_MERC && gpAR->pRobotCell)
		{
			gpAR->pRobotCell->pSoldier->UpdateRobotControllerGivenRobot( );
		}
		if( fKnife || fClaw )
		{
			if( pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->bBleeding - iImpact >= pTarget->pSoldier->stats.bLife )
				pTarget->pSoldier->bBleeding += (INT8)iImpact;
			else
				pTarget->pSoldier->bBleeding = (INT8)(pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife);
		}
		if( !pTarget->pSoldier->stats.bLife )
		{
			// SANDRO - added check to erase insta-healable amount of HPs if dead
			if (pTarget->pSoldier->iHealableInjury > 0)
			{ pTarget->pSoldier->iHealableInjury = 0; } 

			gpAR->fRenderAutoResolve = TRUE;
			#ifdef INVULNERABILITY
			if( 1 )
				RefreshMerc( pTarget->pSoldier );
			else
			#endif
			if( pTarget->uiFlags & CELL_MERC )
			{
				gpAR->usPlayerAttack -= pTarget->usAttack;
				gpAR->usPlayerDefence -= pTarget->usDefence;
				gpAR->ubAliveMercs--;
				pTarget->usAttack = 0;
				pTarget->usDefence = 0;
			}
			else if( pTarget->uiFlags & CELL_MILITIA )
			{
				gpAR->usPlayerAttack -= pTarget->usAttack;
				gpAR->usPlayerDefence -= pTarget->usDefence;
				gpAR->ubAliveCivs--;
				pTarget->usAttack = 0;
				pTarget->usDefence = 0;
			}
			else if( pTarget->uiFlags & (CELL_ENEMY|CELL_CREATURE) )
			{
				gpAR->usEnemyAttack -= pTarget->usAttack;
				gpAR->usEnemyDefence -= pTarget->usDefence;
				gpAR->ubAliveEnemies--;
				pTarget->usAttack = 0;
				pTarget->usDefence = 0;
			}
		}
		pTarget->uiFlags |= CELL_HITBYATTACKER | CELL_DIRTY;
	}
}

void TargetHitCallback( SOLDIERCELL *pTarget, INT32 index )
{
	INT32 iNewLife;
	SOLDIERCELL *pAttacker;
	if( !pTarget->pSoldier->stats.bLife )
	{ //Soldier already dead (can't kill him again!)
		return;
	}
	pAttacker = pTarget->pAttacker[ index ];

	//creatures get damage reduction bonuses
	switch( pTarget->pSoldier->ubBodyType )
	{
		case LARVAE_MONSTER:
		case INFANT_MONSTER:
			break;
		case YAF_MONSTER:
		case YAM_MONSTER:
			pTarget->usHitDamage[index] = (pTarget->usHitDamage[index] + 2) / 4;
			break;
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:
			pTarget->usHitDamage[index] = (pTarget->usHitDamage[index] + 3) / 6;
			break;
		case QUEENMONSTER:
			pTarget->usHitDamage[index] = (pTarget->usHitDamage[index] + 4) / 8;
			break;
	}

	if (gTacticalStatus.uiFlags & GODMODE && pTarget->pSoldier->bTeam == OUR_TEAM)
	{
		iNewLife = pTarget->pSoldier->stats.bLife;
	}
	else
	{
		iNewLife = pTarget->pSoldier->stats.bLife - pTarget->usHitDamage[index];
	}	
	if( !pTarget->usHitDamage[index] )
	{ //bullet missed -- play a ricochet sound.
		if( pTarget->uiFlags & CELL_MERC )
			// AGILITY GAIN: Target "dodged" an attack
			StatChange( pTarget->pSoldier, AGILAMT, 5, FALSE );
		PlayAutoResolveSample( MISS_1 + PreRandom(8), RATE_11025, 50, 1, MIDDLEPAN );
		return;
	}

	if( pAttacker->uiFlags & CELL_MERC )
	{ //Attacker is a player, so increment the number of shots that hit.
		gMercProfiles[ pAttacker->pSoldier->ubProfile ].records.usShotsHit++;
		// MARKSMANSHIP GAIN: Attacker's shot hits
		StatChange( pAttacker->pSoldier, MARKAMT, 6, FALSE );		// in addition to 3 for taking a shot
	}
	if( pTarget->uiFlags & CELL_MERC && pTarget->usHitDamage[ index ] )
	{ //Target is a player, so increment the times he has been wounded.
		if( pTarget->usHitDamage[index] > 1 )
			gMercProfiles[ pTarget->pSoldier->ubProfile ].records.usTimesWoundedShot++;
		// EXPERIENCE GAIN: Took some damage
		StatChange( pTarget->pSoldier, EXPERAMT, ( UINT16 )( 5 * ( pTarget->usHitDamage[ index ] / 10 ) ), FALSE );
	}

	//bullet hit -- play an impact sound and a merc hit sound
	if ( ARMED_VEHICLE( pTarget->pSoldier ) || ENEMYROBOT( pTarget->pSoldier ) )
		PlayAutoResolveSample( (UINT8)(S_METAL_IMPACT1 + PreRandom( 3 )), RATE_11025, 50, 1, MIDDLEPAN );
	else	
		PlayAutoResolveSample( (UINT8)(BULLET_IMPACT_1+PreRandom(3)), RATE_11025, 50, 1, MIDDLEPAN );

	if( pTarget->pSoldier->stats.bLife >= CONSCIOUSNESS )
	{
		if( gpAR->fSound )
			pTarget->pSoldier->DoMercBattleSound( BATTLE_SOUND_HIT1 );
	}

	if( iNewLife < OKLIFE && pTarget->pSoldier->stats.bLife >= OKLIFE )
	{
		//the hit caused the merc to fall.	Play the falling sound
		PlayAutoResolveSample( (UINT8)FALL_1, RATE_11025, 50, 1, MIDDLEPAN );
		pTarget->uiFlags &= ~CELL_RETREATING;
	}

	if( iNewLife <= 0 )
	{
		// Flugente: campaign stats
		gCurrentIncident.AddStat( pTarget->pSoldier, CAMPAIGNHISTORY_TYPE_KILL );

		// Flugente: individual militia
		MILITIA militia;
		if ( GetMilitia( pTarget->pSoldier->usIndividualMilitiaID, &militia ) && !(militia.flagmask & MILITIAFLAG_DEAD) )
		{
			militia.healthratio = 0.0f;
			militia.flagmask |= MILITIAFLAG_DEAD;

			// note the current incident (when closing the incident, we only do this for those still alive)
			MILITIA_BATTLEREPORT report;
			report.id = GetIdOfCurrentlyOngoingIncident( );
			report.flagmask = MILITIA_BATTLEREPORT_FLAG_DIED;

			if ( pTarget->pSoldier->ubMilitiaKills )
				report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY;

			militia.history.push_back( report );

			UpdateMilitia( militia );
		}

		// Flugente: for raids, we need to keep track of killed forces
		if ( GetEnemyEncounterCode() == BLOODCAT_ATTACK_CODE ||
			GetEnemyEncounterCode() == ZOMBIE_ATTACK_CODE ||
			GetEnemyEncounterCode() == BANDIT_ATTACK_CODE )
		{
			AddRaidPersonnel( -( pTarget->pSoldier->ubBodyType == BLOODCAT ), -( pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_ZOMBIE ), -( pTarget->pSoldier->ubSoldierClass == SOLDIER_CLASS_BANDIT ) );
		}
		
		//soldier has been killed
		if( pTarget->pAttacker[ index ]->uiFlags & CELL_PLAYER )
		{ //Player killed the enemy soldier -- update his stats as well as any assisters.
			SOLDIERCELL *pKiller;
			SOLDIERCELL *pAssister1, *pAssister2;
			pKiller = pTarget->pAttacker[ index ];
			pAssister1 = pTarget->pAttacker[ index < 2 ? index + 1 : 0 ];
			pAssister2 = pTarget->pAttacker[ index > 0 ? index - 1 : 2 ];
			if( pKiller == pAssister1 )
				pAssister1 = NULL;
			if( pKiller == pAssister2 )
				pAssister2 = NULL;
			if( pAssister1 == pAssister2 )
				pAssister2 = NULL;
			if( pKiller )
			{
				if( pKiller->uiFlags & CELL_MERC )
				{
					/////////////////////////////////////////////////////////////////////////////////////
					// SANDRO - new mercs' records
					switch(pTarget->pSoldier->ubSoldierClass)
					{
						case SOLDIER_CLASS_ROBOT :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsOthers++;
							break;
						case SOLDIER_CLASS_ELITE :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsElites++;
							break;
						case SOLDIER_CLASS_ARMY :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsRegulars++;
							break;
						case SOLDIER_CLASS_ADMINISTRATOR :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsAdmins++;
							break;
						case SOLDIER_CLASS_CREATURE :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsCreatures++;
							break;
						case SOLDIER_CLASS_ZOMBIE :
							gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsZombies++;
							break;
						case SOLDIER_CLASS_BANDIT:
							gMercProfiles[pKiller->pSoldier->ubProfile].records.usKillsOthers++;
							break;
						default :
							if ( CREATURE_OR_BLOODCAT( pTarget->pSoldier ) )
								gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsCreatures++;
							else if ( ARMED_VEHICLE( pTarget->pSoldier ) ) // we hardly can kill a tank in autoresolve, but well.. who knows..
								gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsTanks++;
							else if ( pTarget->pSoldier->bTeam == CIV_TEAM && !pTarget->pSoldier->aiData.bNeutral && pTarget->pSoldier->bSide != gbPlayerNum )
								gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsHostiles++;
							else
								gMercProfiles[ pKiller->pSoldier->ubProfile ].records.usKillsOthers++;
							break;
					}
					/////////////////////////////////////////////////////////////////////////////////////
					gStrategicStatus.usPlayerKills++;
					// EXPERIENCE CLASS GAIN:	Earned a kill
					StatChange( pKiller->pSoldier, EXPERAMT, ( UINT16 )( 10 * pTarget->pSoldier->pathing.bLevel ), FALSE );
					HandleMoraleEvent( pKiller->pSoldier, MORALE_KILLED_ENEMY, gpAR->ubSectorX, gpAR->ubSectorY, 0	);
				}
				else if( pKiller->uiFlags & CELL_MILITIA )
					pKiller->pSoldier->ubMilitiaKills += 1;
			}
			if( pAssister1 )
			{
				if( pAssister1->uiFlags & CELL_MERC )
				{
					/////////////////////////////////////////////////////////////////////////////////////
					// SANDRO - new mercs' records
					if( pKiller )
					{
						if( pKiller->uiFlags & CELL_MERC && pKiller->pSoldier->bTeam == gbPlayerNum )
							gMercProfiles[ pAssister1->pSoldier->ubProfile ].records.usAssistsMercs++;
						else if ( pKiller->pSoldier->bTeam == MILITIA_TEAM )
							gMercProfiles[ pAssister1->pSoldier->ubProfile ].records.usAssistsMilitia++;
						else
							gMercProfiles[ pAssister1->pSoldier->ubProfile ].records.usAssistsOthers++;
					}
					else
					{
						gMercProfiles[ pAssister1->pSoldier->ubProfile ].records.usAssistsOthers++;
					}
					/////////////////////////////////////////////////////////////////////////////////////
					// EXPERIENCE CLASS GAIN:	Earned an assist
					StatChange( pAssister1->pSoldier, EXPERAMT, ( UINT16 )( 5 * pTarget->pSoldier->pathing.bLevel ), FALSE );
				}
				else if( pAssister1->uiFlags & CELL_MILITIA )
					pAssister1->pSoldier->ubMilitiaAssists++;
			}
			else if( pAssister2 )
			{
				if( pAssister2->uiFlags & CELL_MERC )
				{
					/////////////////////////////////////////////////////////////////////////////////////
					// SANDRO - new mercs' records
					if( pKiller )
					{
						if( pKiller->uiFlags & CELL_MERC && pKiller->pSoldier->bTeam == gbPlayerNum )
							gMercProfiles[ pAssister2->pSoldier->ubProfile ].records.usAssistsMercs++;
						else if ( pKiller->pSoldier->bTeam == MILITIA_TEAM )
							gMercProfiles[ pAssister2->pSoldier->ubProfile ].records.usAssistsMilitia++;
						else
							gMercProfiles[ pAssister2->pSoldier->ubProfile ].records.usAssistsOthers++;
					}
					else
					{
						gMercProfiles[ pAssister2->pSoldier->ubProfile ].records.usAssistsOthers++;
					}
					/////////////////////////////////////////////////////////////////////////////////////
					// EXPERIENCE CLASS GAIN:	Earned an assist
					StatChange( pAssister2->pSoldier, EXPERAMT, ( UINT16 )( 5 * pTarget->pSoldier->pathing.bLevel ), FALSE );
				}
				else if( pAssister2->uiFlags & CELL_MILITIA )
					pAssister2->pSoldier->ubMilitiaAssists++;
			}
		}
		if( pTarget->uiFlags & CELL_MERC && gpAR->fSound )
		{
			PlayAutoResolveSample( (UINT8)DOORCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			PlayAutoResolveSample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}

		if( iNewLife < -60 && !(pTarget->uiFlags & CELL_BUG ) )
		{ //High damage death
			if( gpAR->fSound )
			{
				if( PreRandom( 3 ) )
					PlayAutoResolveSample( (UINT8)BODY_SPLAT_1, RATE_11025, 50, 1, MIDDLEPAN );
				else
					PlayAutoResolveSample( (UINT8)HEADSPLAT_1, RATE_11025, 50, 1, MIDDLEPAN );
			}
		}
		else
		{ //Normal death
			if( gpAR->fSound )
			{
				if ( ARMED_VEHICLE( pTarget->pSoldier ) || ENEMYROBOT( pTarget->pSoldier ) )
					PlayAutoResolveSample( (UINT8)(S_RAID_TB_BOMB), RATE_11025, 50, 1, MIDDLEPAN );
				else
					pTarget->pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
			}
		}
		#ifdef INVULNERABILITY
			RefreshMerc( pTarget->pSoldier );
			return;
		#endif
	}
	///////////////////////////////////////////////////////////////
	// SANDRO - added the insta-healable value for doctor trait
	else if ((IS_MERC_BODY_TYPE( pTarget->pSoldier ) || IS_CIV_BODY_TYPE( pTarget->pSoldier )) && ( gGameOptions.fNewTraitSystem ))
	{
		pTarget->pSoldier->iHealableInjury += ((pTarget->pSoldier->stats.bLife - iNewLife) * 100);
		if (pTarget->pSoldier->iHealableInjury > ((pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife) * 100))
			pTarget->pSoldier->iHealableInjury = ((pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife) * 100);
	}
	///////////////////////////////////////////////////////////////
	//Adjust the soldiers stats based on the damage.
	pTarget->pSoldier->stats.bLife = (INT8)max( iNewLife, 0 );
	if( pTarget->uiFlags & CELL_MERC && gpAR->pRobotCell)
	{
		gpAR->pRobotCell->pSoldier->UpdateRobotControllerGivenRobot( );
	}

	if( pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->bBleeding - pTarget->usHitDamage[index] >= pTarget->pSoldier->stats.bLife )
		pTarget->pSoldier->bBleeding += (INT8)pTarget->usHitDamage[index];
	else
		pTarget->pSoldier->bBleeding = (INT8)(pTarget->pSoldier->stats.bLifeMax - pTarget->pSoldier->stats.bLife);
	if( !pTarget->pSoldier->stats.bLife )
	{
		// SANDRO - added check to erase insta-healable amount of HPs if dead
		if (pTarget->pSoldier->iHealableInjury > 0)
		{ pTarget->pSoldier->iHealableInjury = 0; } 

		gpAR->fRenderAutoResolve = TRUE;
		if( pTarget->uiFlags & CELL_MERC )
		{
			gpAR->usPlayerAttack -= pTarget->usAttack;
			gpAR->usPlayerDefence -= pTarget->usDefence;
			gpAR->ubAliveMercs--;
			pTarget->usAttack = 0;
			pTarget->usDefence = 0;
		}
		else if( pTarget->uiFlags & CELL_MILITIA )
		{
			gpAR->usPlayerAttack -= pTarget->usAttack;
			gpAR->usPlayerDefence -= pTarget->usDefence;
			gpAR->ubAliveCivs--;
			pTarget->usAttack = 0;
			pTarget->usDefence = 0;
		}
		else if( pTarget->uiFlags & (CELL_ENEMY|CELL_CREATURE) )
		{
			gpAR->usEnemyAttack -= pTarget->usAttack;
			gpAR->usEnemyDefence -= pTarget->usDefence;
			gpAR->ubAliveEnemies--;
			pTarget->usAttack = 0;
			pTarget->usDefence = 0;
		}
	}
	pTarget->uiFlags |= CELL_HITBYATTACKER | CELL_DIRTY;
}

void Delay( UINT32 uiMilliseconds )
{
	INT32 iTime;
	iTime = GetJA2Clock();
	while( GetJA2Clock() < iTime + uiMilliseconds );
}

BOOLEAN IsBattleOver()
{
	INT32 i;
	INT32 iNumInvolvedMercs = 0;
	INT32 iNumMercsRetreated = 0;
	INT32 iNumInvolvedMilitia = 0;
	INT32 iNumMilitiaRetreated = 0;
	BOOLEAN fOnlyEPCsLeft = TRUE;

	if( gpAR->ubBattleStatus != BATTLE_IN_PROGRESS )
		return TRUE;

	for( i = 0; i < gpAR->ubMercs; ++i )
	{
		if( !(gpMercs[ i ].uiFlags & CELL_RETREATED) && gpMercs[ i ].pSoldier->stats.bLife )
		{
			if( !(gpMercs[ i ].uiFlags & CELL_EPC) )
			{
				fOnlyEPCsLeft = FALSE;
				++iNumInvolvedMercs;
			}
		}

		if( gpMercs[ i ].uiFlags & CELL_RETREATED )
		{
			++iNumMercsRetreated;
		}
	}

	for ( i = 0; i < gpAR->ubCivs; ++i )
	{
		if ( !(gpCivs[i].uiFlags & CELL_RETREATED) && gpCivs[i].pSoldier->stats.bLife )
		{
			++iNumInvolvedMilitia;
		}
		else if ( gpCivs[i].uiFlags & CELL_RETREATED )
		{
			++iNumMilitiaRetreated;
		}
	}

	if( gpAR->pRobotCell )
	{
		//Do special robot checks
		SOLDIERTYPE *pRobot;
		pRobot = gpAR->pRobotCell->pSoldier;
		if( pRobot->ubRobotRemoteHolderID == NOBODY )
		{
			//Robot can't fight anymore.
			gpAR->usPlayerAttack -= gpAR->pRobotCell->usAttack;
			gpAR->pRobotCell->usAttack = 0;

			if( iNumInvolvedMercs == 1 && !gpAR->ubAliveCivs )
			{
				//Robot is the only one left in battle, so instantly kill him.
				pRobot->DoMercBattleSound( BATTLE_SOUND_DIE1 );
				pRobot->stats.bLife = 0;
				gpAR->ubAliveMercs--;
				iNumInvolvedMercs = 0;
			}
		}
	}

	if( !gpAR->ubAliveCivs && !iNumInvolvedMercs && iNumMercsRetreated )
	{
		//RETREATED
		gpAR->ubBattleStatus = BATTLE_RETREAT;

		// wake everyone up
		WakeUpAllMercsInSectorUnderAttack( );

		RetreatAllInvolvedPlayerGroups( );
	}
	else if ( !iNumInvolvedMercs && !iNumInvolvedMilitia && iNumMilitiaRetreated && gGameExternalOptions.fMilitiaStrategicCommand )
	{
		//RETREATED
		gpAR->ubBattleStatus = BATTLE_RETREAT;

		// wake everyone up
		WakeUpAllMercsInSectorUnderAttack( );

		RetreatAllInvolvedMilitiaGroups( );
	}
	else if( !gpAR->ubAliveCivs && !iNumInvolvedMercs )
	{
		//DEFEAT

		if( fOnlyEPCsLeft )
			{
			//Kill the EPCs.
			for( i = 0; i < gpAR->ubMercs; ++i )
			{
				if( gpMercs[ i ].uiFlags & CELL_EPC )
				{
					gpMercs[ i ].pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
					gpMercs[ i ].pSoldier->stats.bLife = 0;
					gpMercs[ i ].pSoldier->iHealableInjury = 0; // added by SANDRO
					--gpAR->ubAliveMercs;
				}
			}
		}

		for( i = 0; i < gpAR->ubEnemies; ++i )
		{
			if( gpEnemies[ i ].pSoldier->stats.bLife )
			{
				if ( GetEnemyEncounterCode() != CREATURE_ATTACK_CODE && 
					GetEnemyEncounterCode() != BLOODCAT_ATTACK_CODE &&
					GetEnemyEncounterCode() != ZOMBIE_ATTACK_CODE )
				{
					gpEnemies[ i ].pSoldier->DoMercBattleSound( BATTLE_SOUND_LAUGH1 );
				}
				else
				{
					PlayJA2Sample( ACR_EATFLESH, RATE_11025, 50, 1, MIDDLEPAN );
				}
				break;
			}
		}
		gpAR->ubBattleStatus = BATTLE_DEFEAT;
	}
	else if( !gpAR->ubAliveEnemies )
	{
		//VICTORY
		gpAR->ubBattleStatus = BATTLE_VICTORY;
	}
	else
	{
		return FALSE;
	}

	SetupDoneInterface();

	return TRUE;
}


//#define TESTSURRENDER

BOOLEAN AttemptPlayerCapture()
{
	INT32 i;
	BOOLEAN fConcious;
	INT32 iConciousEnemies;

#ifndef TESTSURRENDER

	//Only attempt capture if day is less than four.
	if( GetWorldDay() < STARTDAY_ALLOW_PLAYER_CAPTURE_FOR_RESCUE && !gpAR->fAllowCapture )
	{
		return FALSE;
	}
	if( gpAR->fPlayerRejectedSurrenderOffer )
	{
		return FALSE;
	}
	if( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE )
	{
		return FALSE;
	}
	if( gpAR->fCaptureNotPermittedDueToEPCs )
	{ //EPCs make things much more difficult when considering capture.	Simply don't allow it.
		return FALSE;
	}
	//Only attempt capture of mercs if there are 2 or 3 of them alive
	if( gpAR->ubAliveCivs || gpAR->ubAliveMercs < 2 || gpAR->ubAliveMercs > 3 )
	{
		return FALSE;
	}
	//if the number of alive enemies doesn't double the number of alive mercs, don't offer surrender.
	if( gpAR->ubAliveEnemies < gpAR->ubAliveMercs*2 )
	{
		return FALSE;
	}
	//make sure that these enemies are actually concious!
	iConciousEnemies = 0;
	for( i = 0; i < gpAR->ubEnemies; i++ )
	{
		if( gpEnemies[ i ].pSoldier->stats.bLife >= OKLIFE )
		{
			iConciousEnemies++;
		}
	}
	if( iConciousEnemies < gpAR->ubAliveMercs * 2 )
	{
		return FALSE;
	}

	//So far, the conditions are right.	Now, we will determine if the the remaining players are
	//wounded and/or unconcious.	If any are concious, we will prompt for a surrender, otherwise,
	//it is automatic.
	fConcious = FALSE;
	for( i = 0; i < gpAR->ubMercs; i++ )
	{
		//if any of the 2 or 3 mercs has more than 60% life, then return.
		if( gpMercs[ i ].uiFlags & CELL_ROBOT )
		{
			return FALSE;
		}
		if( gpMercs[ i ].pSoldier->stats.bLife*100 > gpMercs[ i ].pSoldier->stats.bLifeMax*60 )
		{
			return FALSE;
		}
		if( gpMercs[ i ].pSoldier->stats.bLife >= OKLIFE )
		{
			fConcious = TRUE;
		}
	}
	if( fConcious )
	{
		if( PreRandom( 100 ) < 2 )
		{
			SetupSurrenderInterface();
		}
	}
	else if( PreRandom( 100 ) < 25 )
	{
#else
	{
#endif

		BeginCaptureSquence( );

		gpAR->ubBattleStatus = BATTLE_CAPTURED;
		gpAR->fRenderAutoResolve = TRUE;
		SetupDoneInterface();
	}
	return TRUE;
}

void SetupDoneInterface()
{
	INT32 i;
	gpAR->fRenderAutoResolve = TRUE;

	HideButton( gpAR->iButton[ PAUSE_BUTTON ] );
	HideButton( gpAR->iButton[ PLAY_BUTTON ] );
	HideButton( gpAR->iButton[ FAST_BUTTON ] );
	HideButton( gpAR->iButton[ FINISH_BUTTON ] );
	HideButton( gpAR->iButton[ RETREAT_BUTTON ] );
	HideButton( gpAR->iButton[ YES_BUTTON ] );
	HideButton( gpAR->iButton[ NO_BUTTON ] );
	if( gpAR->ubBattleStatus == BATTLE_VICTORY && gpAR->ubAliveMercs )
	{
		ShowButton( gpAR->iButton[ DONEWIN_BUTTON ] );
		ShowButton( gpAR->iButton[ BANDAGE_BUTTON ] );
	}
	else
	{
		ShowButton( gpAR->iButton[ DONELOSE_BUTTON ] );
	}
	DetermineBandageButtonState();
	for( i = 0; i < gpAR->ubMercs; i++ )
	{ //So they can't retreat!
		MSYS_DisableRegion( gpMercs[ i ].pRegion );
	}
}

void SetupSurrenderInterface()
{
	HideButton( gpAR->iButton[ PAUSE_BUTTON ] );
	HideButton( gpAR->iButton[ PLAY_BUTTON ] );
	HideButton( gpAR->iButton[ FAST_BUTTON ] );
	HideButton( gpAR->iButton[ FINISH_BUTTON ] );
	HideButton( gpAR->iButton[ RETREAT_BUTTON ] );
	HideButton( gpAR->iButton[ BANDAGE_BUTTON ] );
	HideButton( gpAR->iButton[ DONEWIN_BUTTON ] );
	HideButton( gpAR->iButton[ DONELOSE_BUTTON ] );
	ShowButton( gpAR->iButton[ YES_BUTTON ] );
	ShowButton( gpAR->iButton[ NO_BUTTON ] );
	gpAR->fRenderAutoResolve = TRUE;
	gpAR->fPendingSurrender = TRUE;
}

void HideSurrenderInterface()
{
	HideButton( gpAR->iButton[ PAUSE_BUTTON ] );
	HideButton( gpAR->iButton[ PLAY_BUTTON ] );
	HideButton( gpAR->iButton[ FAST_BUTTON ] );
	HideButton( gpAR->iButton[ FINISH_BUTTON ] );
	HideButton( gpAR->iButton[ RETREAT_BUTTON ] );
	HideButton( gpAR->iButton[ BANDAGE_BUTTON ] );
	HideButton( gpAR->iButton[ DONEWIN_BUTTON ] );
	HideButton( gpAR->iButton[ DONELOSE_BUTTON ] );
	HideButton( gpAR->iButton[ YES_BUTTON ] );
	HideButton( gpAR->iButton[ NO_BUTTON ] );
	gpAR->fPendingSurrender = FALSE;
	gpAR->fRenderAutoResolve = TRUE;
}

void AcceptSurrenderCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		BeginCaptureSquence( );

		gpAR->ubBattleStatus = BATTLE_SURRENDERED;
		gpAR->fPendingSurrender = FALSE;
		SetupDoneInterface();
	}
}

void RejectSurrenderCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gpAR->fPlayerRejectedSurrenderOffer = TRUE;
		HideSurrenderInterface();
	}
}


void ProcessBattleFrame()
{
	INT32 iRandom;
	INT32 i;
	SOLDIERCELL *pAttacker, *pTarget;
	UINT32 uiDiff;
	static INT32 iTimeSlice = 0;
	static BOOLEAN fContinue = FALSE;
	static UINT32 uiSlice = 0;
	static INT32 iTotal = 0;
	static INT32 iMercs = 0;
	static INT32 iCivs = 0;
	static INT32 iEnemies = 0;
	static INT32 iMercsLeft = 0;
	static INT32 iCivsLeft = 0;
	static INT32 iEnemiesLeft = 0;
	BOOLEAN found = FALSE;
	INT32 iTime, iAttacksThisFrame;

	pAttacker = NULL;
	iAttacksThisFrame = 0;

	if( fContinue )
	{
		gpAR->uiCurrTime = GetJA2Clock();
		fContinue = FALSE;
		goto CONTINUE_BATTLE;
	}
	//determine how much real-time has passed since the last frame
	if( gpAR->uiCurrTime )
	{
		gpAR->uiPrevTime = gpAR->uiCurrTime;
		gpAR->uiCurrTime = GetJA2Clock();
	}
	else
	{
		gpAR->uiCurrTime = GetJA2Clock();
		return;
	}
	if( gpAR->fPaused )
		return;

	uiDiff = gpAR->uiCurrTime - gpAR->uiPrevTime;
	if( gpAR->uiTimeSlice < 0xffffffff )
	{
		iTimeSlice = uiDiff*gpAR->uiTimeSlice/1000;
	}
	else
	{ //largest positive signed value
		iTimeSlice = 0x7fffffff;
	}

	while( iTimeSlice > 0 )
	{
		uiSlice = min( iTimeSlice, 1000 );
		if( gpAR->ubBattleStatus == BATTLE_IN_PROGRESS )
			gpAR->uiTotalElapsedBattleTimeInMilliseconds += uiSlice;

		//Now process each of the players
		iTotal = gpAR->ubMercs + gpAR->ubCivs + gpAR->ubEnemies + 1;
		iMercs	= iMercsLeft		= gpAR->ubMercs;
		iCivs		= iCivsLeft		= gpAR->ubCivs;
		iEnemies = iEnemiesLeft = gpAR->ubEnemies;
		for( i = 0; i < gpAR->ubMercs; i++ )
			gpMercs[ i ].uiFlags &= ~CELL_PROCESSED;
		for( i = 0; i < gpAR->ubCivs; i++ )
			gpCivs[ i ].uiFlags &= ~CELL_PROCESSED;
		for( i = 0; i < gpAR->ubEnemies; i++ )
			gpEnemies[ i ].uiFlags &= ~CELL_PROCESSED;
		while( --iTotal )
		{
			INT32 cnt;
			if( iTimeSlice != 0x7fffffff && GetJA2Clock() > gpAR->uiCurrTime+17 ||
				!gpAR->fInstantFinish && iAttacksThisFrame > (gpAR->ubMercs+gpAR->ubCivs+gpAR->ubEnemies)/4 )
			{ //We have spent too much time in here.	In order to maintain 60FPS, we will
				//leave now, which will allow for updating of the graphics (and mouse cursor),
				//and all of the necessary locals are saved via static variables.	It'll check
				//the fContinue flag, and goto the CONTINUE_BATTLE label the next time this function
				//is called.
				fContinue = TRUE;
				return;
			}
			CONTINUE_BATTLE:
			if ( IsBattleOver() || 
				(GetEnemyEncounterCode() != CREATURE_ATTACK_CODE && GetEnemyEncounterCode() != BLOODCAT_ATTACK_CODE && GetEnemyEncounterCode() != ZOMBIE_ATTACK_CODE && GetEnemyEncounterCode() != BANDIT_ATTACK_CODE )
				&& AttemptPlayerCapture() )
				return;

			iRandom = PreRandom( iTotal );
			found = FALSE;
			if( iMercs && iRandom < iMercsLeft )
			{
				iMercsLeft--;
				while( !found )
				{
					iRandom = Random( iMercs );
					pAttacker = &gpMercs[ iRandom ];
					if( !(pAttacker->uiFlags & CELL_PROCESSED ) )
					{
						pAttacker->uiFlags |= CELL_PROCESSED;
						found = TRUE;
					}
				}
			}
			else if( iCivs && iRandom < iMercsLeft + iCivsLeft )
			{
				iCivsLeft--;
				while( !found )
				{
					iRandom = Random( iCivs );
					pAttacker = &gpCivs[ iRandom ];
					if( !(pAttacker->uiFlags & CELL_PROCESSED ) )
					{
						pAttacker->uiFlags |= CELL_PROCESSED;
						found = TRUE;
					}
				}
			}
			else if( iEnemies && iEnemiesLeft )
			{
				iEnemiesLeft--;
				while( !found )
				{
					iRandom = Random( iEnemies );
					pAttacker = &gpEnemies[ iRandom ];
					if( !(pAttacker->uiFlags & CELL_PROCESSED ) )
					{
						pAttacker->uiFlags |= CELL_PROCESSED;
						found = TRUE;
					}
				}
			}
			else
				AssertMsg( 0, "Logic error in ProcessBattleFrame()" );
			//Apply damage and play miss/hit sounds if delay between firing and hit has expired.
			if( !(pAttacker->uiFlags & CELL_RETREATED ) )
			{
				for( cnt = 0; cnt < 3; ++cnt )
				{ //Check if any incoming bullets have hit the target.
					if( pAttacker->usNextHit[ cnt ] )
					{
						iTime = pAttacker->usNextHit[ cnt ];
						iTime -= uiSlice;
						if( iTime >= 0 )
						{ //Bullet still on route.
							pAttacker->usNextHit[ cnt ] = (UINT16)iTime;
						}
						else
						{ //Bullet is going to hit/miss.
							TargetHitCallback( pAttacker, cnt );
							pAttacker->usNextHit[ cnt ] = 0;
						}
					}
				}
			}
			if( pAttacker->pSoldier->stats.bLife < OKLIFE || pAttacker->uiFlags & CELL_RETREATED )
			{
				if( !(pAttacker->uiFlags & CELL_CREATURE) || !pAttacker->pSoldier->stats.bLife )
					continue; //can't attack if you are unconcious or not around (Or a live creature)
			}
			iTime = pAttacker->usNextAttack;
			iTime -= uiSlice;
			if( iTime > 0 )
			{
				pAttacker->usNextAttack = (UINT32)iTime;
				continue;
			}
			else
			{
				if( pAttacker->uiFlags & CELL_RETREATING )
				{
					//The merc has successfully retreated.	Remove the stats, and continue on.
					if( pAttacker == gpAR->pRobotCell )
					{
						if( gpAR->pRobotCell->pSoldier->ubRobotRemoteHolderID == NOBODY )
						{
							gpAR->pRobotCell->uiFlags &= ~CELL_RETREATING;
							gpAR->pRobotCell->uiFlags |= CELL_DIRTY;
							gpAR->pRobotCell->usNextAttack = 0xffff;
							continue;
						}
					}
					gpAR->usPlayerDefence -= pAttacker->usDefence;
					pAttacker->usDefence = 0;
					pAttacker->uiFlags |= CELL_RETREATED;
					continue;
				}

				if( pAttacker->usAttack )
				{
					pTarget = ChooseTarget( pAttacker );

					if( pAttacker->uiFlags & CELL_BUG && PreRandom( 100 ) < 7 )
						PlayAutoResolveSample( ACR_SMELL_THREAT + PreRandom( 2 ), RATE_11025, 50, 1, MIDDLEPAN );
					else
						AttackTarget( pAttacker, pTarget );

					ResetNextAttackCounter( pAttacker );
					pAttacker->usNextAttack += (UINT32)iTime; //tack on the remainder
					++iAttacksThisFrame;
				}
			}
		}
		if( iTimeSlice != 0x7fffffff )//|| !gpAR->fInstantFinish )
		{
			iTimeSlice -= 1000;
		}
	}
}


BOOLEAN IsAutoResolveActive()
{
	//is the autoresolve up or not?
	if( gpAR )
	{
		return TRUE;
	}
	return FALSE;
}

UINT8 GetAutoResolveSectorID()
{
	if( gpAR )
	{
		return (UINT8)SECTOR( gpAR->ubSectorX, gpAR->ubSectorY );
	}
	return 0xff;
}

//Returns TRUE if a battle is happening or sector is loaded
BOOLEAN GetCurrentBattleSectorXYZ( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ )
{
	if( gpAR )
	{
		*psSectorX = gpAR->ubSectorX;
		*psSectorY = gpAR->ubSectorY;
		*psSectorZ = 0;
		return TRUE;
	}
	else if( gfPreBattleInterfaceActive )
	{
		*psSectorX = gubPBSectorX;
		*psSectorY = gubPBSectorY;
		*psSectorZ = gubPBSectorZ;
		return TRUE;
	}
	else if( gfWorldLoaded )
	{
		*psSectorX = gWorldSectorX;
		*psSectorY = gWorldSectorY;
		*psSectorZ = gbWorldSectorZ;
		return TRUE;
	}
	else
	{
		*psSectorX = 0;
		*psSectorY = 0;
		*psSectorZ = -1;
		return FALSE;
	}
}

//Returns TRUE if a battle is happening ONLY
BOOLEAN GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ )
{
	if ( ( psSectorX == NULL )||(psSectorY == NULL )||(psSectorZ == NULL ) )
	{
		return FALSE;
	}

	if( gpAR )
	{
		*psSectorX = gpAR->ubSectorX;
		*psSectorY = gpAR->ubSectorY;
		*psSectorZ = 0;
		return TRUE;
	}
	else if( gfPreBattleInterfaceActive )
	{
		*psSectorX = gubPBSectorX;
		*psSectorY = gubPBSectorY;
		*psSectorZ = gubPBSectorZ;
		return TRUE;
	}
	else if( gfWorldLoaded )
	{
		*psSectorX = gWorldSectorX;
		*psSectorY = gWorldSectorY;
		*psSectorZ = gbWorldSectorZ;
		if( gTacticalStatus.fEnemyInSector )
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		*psSectorX = 0;
		*psSectorY = 0;
		*psSectorZ = -1;
		return FALSE;
	}
}

void AutoBandageFinishedCallback( UINT8 ubResult )
{
	SetupDoneInterface();
}

BOOLEAN ProcessLoyalty()
{
	if( !gpAR ) return FALSE;

	if( PlayerMercsInSector( gpAR->ubSectorX, gpAR->ubSectorY, 0 ) )
		return TRUE;

	if( GetTownIdForSector(	gpAR->ubSectorX,	gpAR->ubSectorY ) != BLANK_SECTOR ||
		IsThisSectorASAMSector(	gpAR->ubSectorX,	gpAR->ubSectorY, 0 ) )
		return TRUE;

	return FALSE;
}


void CheckForSoldiersWhoRetreatedIntoMilitiaHeldSectors()
{
	for( int sX = 1; sX < ( MAP_WORLD_X - 1 ); ++sX )
	{
		for( int sY = 1; sY < ( MAP_WORLD_Y - 1); ++sY )
		{
			// Check if there is a sector where enemies retreated to and there are also militia present
			if ( (NumNonPlayerTeamMembersInSector( sX, sY, ENEMY_TEAM ) > 0) &&
				 (NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ) > 0) &&
				(!gTacticalStatus.fEnemyInSector))
			{
				unsigned mercCnt = 0;
				for( int i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++i )
				{
					if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( MercPtrs[ i ] ) )
					{
						//Merc is active and alive, and not a vehicle or robot
						if ((MercPtrs[ i ]->sSectorX == sX) &&(MercPtrs[ i ]->sSectorY == sY) && (MercPtrs[ i ]->bSectorZ == 0))
						{
							++mercCnt;
						}
					}
				}

				// If there are PC mercs here the player will have to handle the battle
				if (mercCnt == 0)
				{
//					EnterAutoResolveMode ((UINT8)sX, (UINT8)sY);
				}
			}
		}
	}
}

// Flugente: have all militia in autoresolve drop their gear and be promoted in necessary
void AutoResolveMilitiaDropAndPromote()
{
	gbGreenToElitePromotions = 0;
	gbGreenToRegPromotions = 0;
	gbRegToElitePromotions = 0;
	gbMilitiaPromotions = 0;
	for ( INT32 i = 0; i < MAX_AR_TEAM_SIZE; ++i )
	{
		if ( gpCivs[i].pSoldier )
		{
			UINT8 ubCurrentRank = GREEN_MILITIA;
			switch ( gpCivs[i].pSoldier->ubSoldierClass )
			{
			case SOLDIER_CLASS_GREEN_MILITIA:		ubCurrentRank = GREEN_MILITIA;		break;
			case SOLDIER_CLASS_REG_MILITIA:			ubCurrentRank = REGULAR_MILITIA;	break;
			case SOLDIER_CLASS_ELITE_MILITIA:		ubCurrentRank = ELITE_MILITIA;		break;
			default:
#ifdef JA2BETAVERSION
				ScreenMsg( FONT_RED, MSG_ERROR, L"Removing autoresolve militia with invalid ubSoldierClass %d.", gpCivs[i].pSoldier->ubSoldierClass );
#endif
				break;
			}

			// Flugente: drop sector equipment
			gpCivs[i].pSoldier->DropSectorEquipment( );
						
			if ( gpCivs[i].pSoldier->stats.bLife < OKLIFE / 2 )
			{
				// Flugente: individual militia
				// we not only handle promotions here, but basically update this guy
				MILITIA militia;
				if ( GetMilitia( gpCivs[i].pSoldier->usIndividualMilitiaID, &militia ) && !(militia.flagmask & MILITIAFLAG_DEAD) )
				{
					MILITIA_BATTLEREPORT report;
					report.id = GetIdOfCurrentlyOngoingIncident( );
					
					report.flagmask |= MILITIA_BATTLEREPORT_FLAG_DIED;
					
					if ( gpCivs[i].pSoldier->ubMilitiaKills )
						report.flagmask |= MILITIA_BATTLEREPORT_FLAG_KILLEDENEMY;
					
					militia.history.push_back( report );

					militia.healthratio = 0.0f;
					militia.flagmask |= MILITIAFLAG_DEAD;

					UpdateMilitia( militia );
				}

				StrategicRemoveMilitiaFromSector( gpCivs[i].pSoldier->sSectorX, gpCivs[i].pSoldier->sSectorY, ubCurrentRank, 1 );
			}
			else if ( gpCivs[i].pSoldier->stats.bLife >= OKLIFE / 2 )
			{
				// Flugente: take care of promotions and individual militia update
				HandlePossibleMilitiaPromotion( gpCivs[i].pSoldier, TRUE );
			}

			// DO NOT DELETE HERE!!!!
			//TacticalRemoveSoldierPointer( gpCivs[i].pSoldier, FALSE );
			//memset( &gpCivs[i], 0, sizeof(SOLDIERCELL) );
		}
	}
}

BOOLEAN IndividualMilitiaInUse_AutoResolve( UINT32 aMilitiaId )
{
	if ( gpAR )
	{
		for ( INT32 i = 0; i < gpAR->ubCivs; ++i )
		{
			if ( gpCivs[i].pSoldier && aMilitiaId == gpCivs[i].pSoldier->usIndividualMilitiaID && IsLegalMilitiaId( gpCivs[i].pSoldier->usIndividualMilitiaID ) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}