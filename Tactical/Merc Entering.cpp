#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	//#include "soldier control.h"
	// HEADROCK HAM 3.5: Strange that this wasn't included.
	#include "GameSettings.h"
	#include "weapons.h"
	#include "handle items.h"
	#include "worlddef.h"
	#include "rotting corpses.h"
	#include "tile cache.h"
	#include "isometric utils.h"
	#include "animation control.h"
	#include "utilities.h"
	#include "game clock.h"
	#include "soldier create.h"
	#include "renderworld.h"
	#include "tile animation.h"
	#include "merc entering.h"
	#include "Sound Control.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "Handle UI.h"
	#include "squads.h"
	#include "english.h"
	#include "cursor control.h"
	#include "Merc Hiring.h"
	#include "Font Control.h"
	#include "message.h"
	#include "Text.h"
	#include "strategic turns.h"
	#include "ai.h"
	#include "Dialogue Control.h"
	#include "Music Control.h"
	#include "Tactical Save.h"
	// HEADROCK HAM 3.5: Need this to see if enemies present at starting sector
	#include "Overhead.h"
	#include "Map Information.h"	// added by Flugente
#endif

#ifdef JA2UB
#include "ub_config.h"
#endif

#include "Music Control.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// WDS - make number of mercenaries, etc. be configurable
#define		MAX_MERC_IN_HELI		CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS
#define		MAX_HELI_SCRIPT			30
#define		ME_SCRIPT_DELAY				100
#define		NUM_PER_HELI_RUN		6

enum
{
	HELI_APPROACH,
	HELI_MOVETO,
	HELI_BEGINDROP,
	HELI_DROP,
	HELI_ENDDROP,
	HELI_MOVEAWAY,
	HELI_EXIT,
	NUM_HELI_STATES

} HeliStateEnums;

enum
{
	HELI_REST,
	HELI_MOVE_DOWN,
	HELI_MOVE_UP,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_UP,
	HELI_MOVEY,
	HELI_MOVELARGERY,
	HELI_HANDLE_DROP,
	HELI_SHOW_HELI,

	HELI_GOTO_BEGINDROP,
	HELI_GOTO_DROP,
	HELI_GOTO_EXIT,
	HELI_GOTO_MOVETO,
	HELI_GOTO_MOVEAWAY,
	HELI_DONE,


} HeliCodes;


UINT8			ubHeliScripts[ NUM_HELI_STATES ][ MAX_HELI_SCRIPT ] =
{
	// HELI_APPROACH
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_GOTO_MOVETO,

	// MOVE TO
	HELI_SHOW_HELI,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,

	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,

	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,

	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,

	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,

	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_MOVEY,
	HELI_GOTO_BEGINDROP,

	// HELI_BEGIN_DROP
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,

	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,

	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,

	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,

	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,

	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_MOVE_DOWN,
	HELI_GOTO_DROP,

	// Heli Begin Drop
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,

	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,

	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,

	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,

	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,
	HELI_MOVESMALL_UP,

	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_MOVESMALL_DOWN,
	HELI_GOTO_DROP,

	// HELI END DROP
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,

	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,

	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,

	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,

	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,

	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_MOVE_UP,
	HELI_GOTO_MOVEAWAY,

	// MOVE AWAY
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,

	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,

	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,

	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,

	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,

	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_MOVELARGERY,
	HELI_GOTO_EXIT,

	// HELI EXIT
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,

	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_REST,
	HELI_DONE,
};


BOOLEAN		gfHandleHeli = FALSE;
UINT8			gusHeliSeats[ MAX_MERC_IN_HELI ];
INT8			gbNumHeliSeatsOccupied = 0;

BOOLEAN		gfFirstGuyDown = FALSE;

UINT32		uiSoundSample;
INT32			gsGridNoSweetSpot;
INT16			gsHeliXPos;
INT16			gsHeliYPos;
FLOAT			gdHeliZPos;
INT16			gsHeliScript;
UINT8			gubHeliState;
UINT32		guiHeliLastUpdate;
INT8			gbCurDrop;
INT8			gbExitCount;
INT8			gbHeliRound;

// Flugente: direction of helicopter approach
UINT8 gHeliEnterDirection = NORTH;

BOOLEAN		fFadingHeliIn = FALSE;
BOOLEAN		fFadingHeliOut = FALSE;

BOOLEAN		gfIngagedInDrop = FALSE;

ANITILE		*gpHeli;
BOOLEAN		gfFirstHeliRun;

extern BOOLEAN		gfTacticalDoHeliRun;

void HandleFirstHeliDropOfGame( );

void ResetHeliSeats( )
{
	gbNumHeliSeatsOccupied = 0;
}

void AddMercToHeli( UINT8 ubID )
{
	if ( gbNumHeliSeatsOccupied < MAX_MERC_IN_HELI )
	{
		// Check if it already exists!
		for ( INT32 cnt = 0; cnt < gbNumHeliSeatsOccupied; ++cnt )
		{
			if ( gusHeliSeats[ cnt ] == ubID )
			{
				return;
			}
		}

		gusHeliSeats[ gbNumHeliSeatsOccupied ] = ubID;
		++gbNumHeliSeatsOccupied;
	}
}

// Flugente: we might want to set the helicopter dropoff point without starting it at the same time
void SetHelicopterDroppoint( INT32 sGridNo )
{
	gsGridNoSweetSpot = sGridNo;
}

void SetHelicopterDropDirection( UINT8 usDirection )
{
	gHeliEnterDirection = usDirection;
}

void StartHelicopterRun()
{
	INT16 sX, sY;
	
	if ( gbNumHeliSeatsOccupied == 0 )
	{
		return;
	}

	InterruptTime();
	PauseGame();
	LockPauseState( 20 );

	ConvertGridNoToCenterCellXY( gsGridNoSweetSpot, &sX, &sY );

	if ( gHeliEnterDirection == SOUTH )
	{
		gsHeliXPos = sX - (2 * CELL_X_SIZE);
		gsHeliYPos = sY + (10 * CELL_Y_SIZE);
	}
	else if ( gHeliEnterDirection == EAST )
	{
		gsHeliXPos = sX + (10 * CELL_X_SIZE);
		gsHeliYPos = sY + (2 * CELL_Y_SIZE);
	}
	else if ( gHeliEnterDirection == WEST )
	{
		gsHeliXPos = sX - (10 * CELL_X_SIZE);
		gsHeliYPos = sY - (2 * CELL_Y_SIZE);
	}
	else
	{
		gsHeliXPos = sX - (2 * CELL_X_SIZE);
		gsHeliYPos = sY - (10 * CELL_Y_SIZE);
	}

	gdHeliZPos					= 0;
	gsHeliScript				= 0;
	gbCurDrop						= 0;
	gbExitCount					= 0;
	gbHeliRound					= 1;

	gubHeliState				= HELI_APPROACH;
	guiHeliLastUpdate			= GetJA2Clock( );

	// Start sound
	uiSoundSample = PlayJA2Sample( HELI_1, RATE_11025, 0, 10000, MIDDLEPAN );
	fFadingHeliIn				= TRUE;

	gfHandleHeli				= TRUE;
	gfFirstGuyDown				= TRUE;

	guiPendingOverrideEvent = LU_BEGINUILOCK;
}


void HandleHeliDrop( BOOLEAN fPlayer )
{
	UINT8 ubScriptCode;
	UINT32	uiClock;
	INT32 iVol;
	INT32 cnt;
	ANITILE_PARAMS	AniParams;

	if ( !fPlayer )
		return HandleEnemyAirdrop();
	
	if ( gfHandleHeli )
	{
		if ( gCurrentUIMode != LOCKUI_MODE )
		{
			guiPendingOverrideEvent = LU_BEGINUILOCK;
		}
#ifdef JA113DEMO
		//if ( _KeyDown( ESC ) )
		//{
			// Loop through all mercs not yet placed
			for ( cnt = gbCurDrop; cnt < gbNumHeliSeatsOccupied; cnt++ )
			{
				// Add merc to sector
				#ifdef JA2UB
				//MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
				MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				MercPtrs[ gusHeliSeats[ cnt ] ]->usStrategicInsertionData = gGameUBOptions.LOCATEGRIDNO;
				#else
				//MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
				MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				MercPtrs[ gusHeliSeats[ cnt ] ]->usStrategicInsertionData = gGameExternalOptions.iInitialMercArrivalLocation;
				#endif
				// HEADROCK HAM 3.5: Externalized!
				UpdateMercInSector( MercPtrs[ gusHeliSeats[ cnt ] ], gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );

				// Check for merc arrives quotes...
				HandleMercArrivesQuotes( MercPtrs[ gusHeliSeats[ cnt ] ] );

				ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[ MERC_HAS_ARRIVED_STR ], MercPtrs[ gusHeliSeats[ cnt ] ]->GetName() );

			}

			// Remove heli
			DeleteAniTile( gpHeli );

			RebuildCurrentSquad( );

			// Remove sound
			if( uiSoundSample!=NO_SAMPLE )
			{
				SoundStop( uiSoundSample );
			}

			gfHandleHeli = FALSE;
			gfIgnoreScrolling = FALSE;
			gbNumHeliSeatsOccupied = 0;
			UnLockPauseState();
			UnPauseGame();


			// Select our first guy
			SelectSoldier( gusHeliSeats[ 0 ], FALSE, TRUE );

			//guiCurrentEvent = LU_ENDUILOCK;
			//gCurrentUIMode	= LOCKUI_MODE;
			guiPendingOverrideEvent = LU_ENDUILOCK;
			//UIHandleLUIEndLock( NULL );

			HandleFirstHeliDropOfGame( );
			return;

		//}
#else
		if ( _KeyDown( ESC ) )
		{
			// Loop through all mercs not yet placed
			for ( cnt = gbCurDrop; cnt < gbNumHeliSeatsOccupied; cnt++ )
			{
				// Add merc to sector
#ifdef JA2UB
				//MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
				MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				MercPtrs[ gusHeliSeats[ cnt ] ]->usStrategicInsertionData = gGameUBOptions.LOCATEGRIDNO;
#else
				//MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
				MercPtrs[ gusHeliSeats[ cnt ] ]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				MercPtrs[ gusHeliSeats[ cnt ] ]->usStrategicInsertionData = gGameExternalOptions.iInitialMercArrivalLocation;
				#endif
				// HEADROCK HAM 3.5: Externalized!
				UpdateMercInSector( MercPtrs[ gusHeliSeats[ cnt ] ], gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );

				// Check for merc arrives quotes...
				HandleMercArrivesQuotes( MercPtrs[ gusHeliSeats[ cnt ] ] );

				ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[ MERC_HAS_ARRIVED_STR ], MercPtrs[ gusHeliSeats[ cnt ] ]->GetName() );
			}

			// Remove heli
			DeleteAniTile( gpHeli );

			RebuildCurrentSquad( );

			// Remove sound
			if( uiSoundSample!=NO_SAMPLE )
			{
				SoundStop( uiSoundSample );
			}

			gfHandleHeli = FALSE;
			gfIgnoreScrolling = FALSE;
			gbNumHeliSeatsOccupied = 0;
			UnLockPauseState();
			UnPauseGame();

			// Select our first guy
			SelectSoldier( gusHeliSeats[ 0 ], FALSE, TRUE );

			//guiCurrentEvent = LU_ENDUILOCK;
			//gCurrentUIMode	= LOCKUI_MODE;
			guiPendingOverrideEvent = LU_ENDUILOCK;
			//UIHandleLUIEndLock( NULL );

			HandleFirstHeliDropOfGame( );
			return;
		}
#endif
		gfIgnoreScrolling = TRUE;

		uiClock = GetJA2Clock( );

		if ( ( uiClock - guiHeliLastUpdate ) >	ME_SCRIPT_DELAY )
		{
			guiHeliLastUpdate = uiClock;

			if(fFadingHeliIn)
			{
				if( uiSoundSample!=NO_SAMPLE )
				{
					iVol=SoundGetVolume( uiSoundSample );
					iVol=__min( HIGHVOLUME, iVol+5);
					SoundSetVolume(uiSoundSample, iVol);
					if(iVol==HIGHVOLUME)
						fFadingHeliIn=FALSE;
				}
				else
				{
					fFadingHeliIn=FALSE;
				}
			}
			else if(fFadingHeliOut)
			{
				if( uiSoundSample!=NO_SAMPLE )
				{
					iVol=SoundGetVolume(uiSoundSample);

					iVol=__max( 0, iVol-5);

					SoundSetVolume(uiSoundSample, iVol);
					if(iVol==0)
					{
						// Stop sound
						SoundStop( uiSoundSample );
						fFadingHeliOut=FALSE;
						gfHandleHeli = FALSE;
						gfIgnoreScrolling = FALSE;
						gbNumHeliSeatsOccupied = 0;
						guiPendingOverrideEvent = LU_ENDUILOCK;
						UnLockPauseState();
						UnPauseGame();

						RebuildCurrentSquad( );

						HandleFirstHeliDropOfGame( );
					}
				}
				else
				{
					fFadingHeliOut=FALSE;
					gfHandleHeli = FALSE;
					gfIgnoreScrolling = FALSE;
					gbNumHeliSeatsOccupied = 0;
					guiPendingOverrideEvent = LU_ENDUILOCK;
					UnLockPauseState();
					UnPauseGame();

					RebuildCurrentSquad( );

					HandleFirstHeliDropOfGame( );
				}
			}

			if ( gsHeliScript == MAX_HELI_SCRIPT )
			{
				return;
			}

			ubScriptCode = ubHeliScripts[ gubHeliState ][ gsHeliScript ];

			// Switch on mode...
			if ( gubHeliState == HELI_DROP )
			{
				if ( !gfIngagedInDrop )
				{
					INT8 bEndVal = ( gbHeliRound * NUM_PER_HELI_RUN );

					if ( bEndVal > gbNumHeliSeatsOccupied )
					{
						bEndVal = gbNumHeliSeatsOccupied;
					}

					// Flugente: watching mercs rope down one by one gets boring fast. So we allow up to 3 mercs to rope down  at the same time
					for ( int i = 0; i < 3; ++i )
					{
						// OK, Check if we have anybody left to send!
						if ( gbCurDrop < bEndVal )
						{
							// Flugente: it is now possible to use airdrops with soldiers after they have arrived in Arulco. In that case, they might have an animation that breaks EVENT_InitNewSoldierAnim prematurely.
							// In the worst case, this can cause the game to be unable to finish the airdrop. For that reason, we set all those soldiers to the STANDING aniamtion. 
							MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->usAnimState = STANDING;
							MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->EVENT_InitNewSoldierAnim( HELIDROP, 0 , FALSE );

							// Change insertion code
							MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
							MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->usStrategicInsertionData = gsGridNoSweetSpot;

							// HEADROCK HAM 3.5: Externalized!
							UpdateMercInSector( MercPtrs[ gusHeliSeats[ gbCurDrop ] ], gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );
							//EVENT_SetSoldierPosition( MercPtrs[ gusHeliSeats[ gbCurDrop ] ], sWorldX, sWorldY );

							// IF the first guy down, set squad!
							if ( gfFirstGuyDown )
							{
								gfFirstGuyDown = FALSE;
								SetCurrentSquad( MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->bAssignment, TRUE );
							}
							ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[ MERC_HAS_ARRIVED_STR ], MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->GetName() );

							++gbCurDrop;
						
							gfIngagedInDrop = TRUE;
						}
						else
						{
							if( gbExitCount == 0 )
							{
								gbExitCount = 2;
							}
							else
							{
								--gbExitCount;

								if ( gbExitCount == 1 )
								{
									// Goto leave
									gsHeliScript				= -1;
									gubHeliState				= HELI_ENDDROP;
								}
							}

							break;
						}
					}
				}
			}

			switch( ubScriptCode )
			{
				case HELI_REST:

					break;

				case HELI_MOVE_DOWN:

					gdHeliZPos -= 1;
					gpHeli->pLevelNode->sRelativeZ = ( INT16 )gdHeliZPos;
					break;

				case HELI_MOVE_UP:

					gdHeliZPos += 1;
					gpHeli->pLevelNode->sRelativeZ = ( INT16 )gdHeliZPos;
					break;

				case HELI_MOVESMALL_DOWN:

					gdHeliZPos -= 0.25;
					gpHeli->pLevelNode->sRelativeZ = ( INT16 )gdHeliZPos;
					break;

				case HELI_MOVESMALL_UP:

					gdHeliZPos += 0.25;
					gpHeli->pLevelNode->sRelativeZ = ( INT16 )gdHeliZPos;
					break;


				case HELI_MOVEY:

					if ( gHeliEnterDirection == SOUTH )
					{
						gpHeli->sRelativeY -= 4;
					}
					else if ( gHeliEnterDirection == EAST )
					{
						gpHeli->sRelativeX -= 4;
						gpHeli->sRelativeY -= 1;
					}
					else if ( gHeliEnterDirection == WEST )
					{
						gpHeli->sRelativeX += 4;
						gpHeli->sRelativeY += 1;
					}
					else
					{
						gpHeli->sRelativeY += 4;
					}
					break;

				case HELI_MOVELARGERY:

					if ( gHeliEnterDirection == SOUTH )
					{
						gpHeli->sRelativeY -= 6;
					}
					else if ( gHeliEnterDirection == EAST )
					{
						gpHeli->sRelativeX -= 6;
						gpHeli->sRelativeY -= 1;
					}
					else if ( gHeliEnterDirection == WEST )
					{
						gpHeli->sRelativeX += 6;
						gpHeli->sRelativeY += 1;
					}
					else
					{
						gpHeli->sRelativeY += 6;
					}
					break;

				case HELI_GOTO_BEGINDROP:

					gsHeliScript				= -1;
					gubHeliState				= HELI_BEGINDROP;
					break;

				case HELI_SHOW_HELI:

					// Start animation
					memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
					AniParams.sGridNo							= gsGridNoSweetSpot;
					AniParams.ubLevelID						= ANI_SHADOW_LEVEL;
					AniParams.sDelay							= 90;
					AniParams.sStartFrame					= 0;
					AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_LOOPING;
					AniParams.sX									= gsHeliXPos;
					AniParams.sY									= gsHeliYPos;
					AniParams.sZ									= (INT16)gdHeliZPos;

					if ( gHeliEnterDirection == SOUTH )
					{
						strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_SOUTH.STI" );
					}
					else if ( gHeliEnterDirection == EAST )
					{
						strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_EAST.STI" );
					}
					else if ( gHeliEnterDirection == WEST )
					{
						strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_WEST.STI" );
					}
					else
					{
						strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH.STI" );
					}

					gpHeli = CreateAnimationTile( &AniParams );
					break;

				case HELI_GOTO_DROP:

					// Goto drop animation
					gdHeliZPos -= 0.25;
					gpHeli->pLevelNode->sRelativeZ = ( INT16 )gdHeliZPos;
					gsHeliScript				= -1;
					gubHeliState				= HELI_DROP;
					break;

				case HELI_GOTO_MOVETO:

					// Goto drop animation
					gsHeliScript				= -1;
					gubHeliState				= HELI_MOVETO;
					break;

				case HELI_GOTO_MOVEAWAY:

					// Goto drop animation
					gsHeliScript				= -1;
					gubHeliState				= HELI_MOVEAWAY;
					break;

				case HELI_GOTO_EXIT:

					if ( gbCurDrop < gbNumHeliSeatsOccupied )
					{
						// Start another run......
						INT16 sX, sY;

						ConvertGridNoToCenterCellXY( gsGridNoSweetSpot, &sX, &sY );

						if ( gHeliEnterDirection == SOUTH )
						{
							gsHeliXPos = sX - (2 * CELL_X_SIZE);
							gsHeliYPos = sY + (10 * CELL_Y_SIZE);
						}
						else if ( gHeliEnterDirection == EAST )
						{
							gsHeliXPos = sX + (10 * CELL_X_SIZE);
							gsHeliYPos = sY + (2 * CELL_Y_SIZE);
						}
						else if ( gHeliEnterDirection == WEST )
						{
							gsHeliXPos = sX - (10 * CELL_X_SIZE);
							gsHeliYPos = sY - (2 * CELL_Y_SIZE);
						}
						else
						{
							gsHeliXPos = sX - (2 * CELL_X_SIZE);
							gsHeliYPos = sY - (10 * CELL_Y_SIZE);
						}

						gdHeliZPos					= 0;
						gsHeliScript				= 0;
						gbExitCount					= 0;
						gubHeliState				= HELI_APPROACH;
						++gbHeliRound;

						// Ahh, but still delete the heli!
						DeleteAniTile( gpHeli );
						gpHeli = NULL;
					}
					else
					{
						// Goto drop animation
						gsHeliScript				= -1;
						gubHeliState				= HELI_EXIT;

						// Delete helicopter image!
						DeleteAniTile( gpHeli );
						gpHeli = NULL;
						gfIgnoreScrolling = FALSE;

						// Select our first guy
						SelectSoldier( gusHeliSeats[ 0 ], FALSE, TRUE );
					}
					break;

				case HELI_DONE:

					// End
					fFadingHeliOut			= TRUE;

					// HEADROCK HAM 3.5: Update now, in case the LZ is still in a "RED" airspace sector. This is only
					// required if the sector is free of enemies... but still required. Will run immediately after the
					// helicopter is gone.
					UpdateAirspaceControl( );
					break;
			}

			++gsHeliScript;
		}
	}
}


void BeginMercEntering( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	ResetHeliSeats( );

	AddMercToHeli( pSoldier->ubID );
	
	StartHelicopterRun();

	// Make sure AI does nothing.....
	PauseAIUntilManuallyUnpaused();
}


void HandleFirstHeliDropOfGame( )
{
	// Flugente: always call people to area
	CallAvailableEnemiesTo( gsGridNoSweetSpot );

	// Are we in the first heli drop?
	if ( gfFirstHeliRun )
	{
		SyncStrategicTurnTimes();

		// Move to header file...
		// HEADROCK HAM 3.5: Externalized!
		AddExtraItems( (UINT8)gGameExternalOptions.ubDefaultArrivalSectorX, (UINT8)gGameExternalOptions.ubDefaultArrivalSectorY, startingZ, true );

		// HEADROCK HAM 3.5: Starting sector externalized - might not contain enemies at all!
		if (NumEnemyInSector( ) > 0)
		//if ( NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, 0 ) > 0 )
		{
			// Say quote.....
			SayQuoteFromAnyBodyInSector( QUOTE_ENEMY_PRESENCE );
			
			// Start music
#ifdef ENABLE_ZOMBIES
			UseCreatureMusic(HostileZombiesPresent());
#endif
			#ifdef NEWMUSIC
			GlobalSoundID  = MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ];
			if ( MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ] != -1 )
				SetMusicModeID( MUSIC_TACTICAL_ENEMYPRESENT, MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ] );
			else
			#endif
				SetMusicMode( MUSIC_TACTICAL_ENEMYPRESENT );

		}
		else
		{
			// Say quote.....
			SayQuoteFromAnyBodyInSector( QUOTE_MERC_REACHED_DESTINATION );
			// Start music
			#ifdef NEWMUSIC
			GlobalSoundID  = MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ];
			if ( MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] != -1 )
				//SetMusicMode( MUSIC_TACTICAL_VICTORY );
				SetMusicModeID( MUSIC_TACTICAL_NOTHING, MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalNothing[gbWorldSectorZ] );
			else
			#endif
			SetMusicMode( MUSIC_TACTICAL_NOTHING );
		}

		gfFirstHeliRun = FALSE;
	}

	// Send message to turn on ai again....
	CharacterDialogueWithSpecialEvent( 0, 0, 0, DIALOGUE_TACTICAL_UI , FALSE , FALSE , DIALOGUE_SPECIAL_EVENT_ENABLE_AI ,0, 0 );
}

UINT16 SpawnAirDropElite( INT32 sGridNo )
{
	SOLDIERTYPE *pSoldier;

	// not underground!
	if ( gbWorldSectorZ )
		return NOBODY;

	// hmm...
	if ( !IsLocationSittable( sGridNo, 0 ) )
		return NOBODY;

	// Flugente hack		
	pSoldier = TacticalCreateEliteEnemy( );

	//Add soldier strategic info, so it doesn't break the counters!
	if ( pSoldier )
	{
		if ( !gbWorldSectorZ )
		{
			SECTORINFO *pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];
			switch ( pSoldier->ubSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:	pSector->ubNumAdmins++; pSector->ubAdminsInBattle++; break;
			case SOLDIER_CLASS_ARMY:			pSector->ubNumTroops++; pSector->ubTroopsInBattle++; break;
			case SOLDIER_CLASS_ELITE:			pSector->ubNumElites++; pSector->ubElitesInBattle++; break;
			}
		}

		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CHOPPER;
		pSoldier->usStrategicInsertionData = sGridNo; // required, otherwise soldiers will spawn in map before jumping out of the heli
		UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		//AllTeamsLookForAll( NO_INTERRUPTS );
	}

	return pSoldier->ubID;
}

void InitiateEnemyAirDropSoldiers( INT32 sGridNo )
{
	gfTacticalDoHeliRun = TRUE;

	ResetHeliSeats( );

	//SetHelicopterDroppoint( gMapInformation.sCenterGridNo );
	SetHelicopterDroppoint( sGridNo );

	SetHelicopterDropDirection( WEST );

	for ( int i = 0; i < 6; ++i )
	{
		UINT8 id = SpawnAirDropElite( gMapInformation.sSouthGridNo + i );

		if ( id == NOBODY )
			return;

		AddMercToHeli( id );
	}
}


void HandleEnemyAirdrop( )
{
	UINT8 ubScriptCode;
	UINT32	uiClock;
	INT32 iVol;
	ANITILE_PARAMS	AniParams;

	if ( gfHandleHeli )
	{
		if ( gCurrentUIMode != LOCKUI_MODE )
		{
			guiPendingOverrideEvent = LU_BEGINUILOCK;
		}

		gfIgnoreScrolling = TRUE;

		uiClock = GetJA2Clock( );

		if ( (uiClock - guiHeliLastUpdate) >	ME_SCRIPT_DELAY )
		{
			guiHeliLastUpdate = uiClock;

			if ( fFadingHeliIn )
			{
				if ( uiSoundSample != NO_SAMPLE )
				{
					iVol = SoundGetVolume( uiSoundSample );
					iVol = __min( HIGHVOLUME, iVol + 5 );
					SoundSetVolume( uiSoundSample, iVol );
					if ( iVol == HIGHVOLUME )
						fFadingHeliIn = FALSE;
				}
				else
				{
					fFadingHeliIn = FALSE;
				}
			}
			else if ( fFadingHeliOut )
			{
				if ( uiSoundSample != NO_SAMPLE )
				{
					iVol = SoundGetVolume( uiSoundSample );

					iVol = __max( 0, iVol - 5 );

					SoundSetVolume( uiSoundSample, iVol );
					if ( iVol == 0 )
					{
						// Stop sound
						SoundStop( uiSoundSample );
						fFadingHeliOut = FALSE;
						gfHandleHeli = FALSE;
						gfIgnoreScrolling = FALSE;
						gbNumHeliSeatsOccupied = 0;
						guiPendingOverrideEvent = LU_ENDUILOCK;
						UnLockPauseState( );
						UnPauseGame( );

						RebuildCurrentSquad( );

						HandleFirstHeliDropOfGame( );
					}
				}
				else
				{
					fFadingHeliOut = FALSE;
					gfHandleHeli = FALSE;
					gfIgnoreScrolling = FALSE;
					gbNumHeliSeatsOccupied = 0;
					guiPendingOverrideEvent = LU_ENDUILOCK;
					UnLockPauseState( );
					UnPauseGame( );

					RebuildCurrentSquad( );

					HandleFirstHeliDropOfGame( );
				}
			}

			if ( gsHeliScript == MAX_HELI_SCRIPT )
			{
				return;
			}

			ubScriptCode = ubHeliScripts[gubHeliState][gsHeliScript];

			// Switch on mode...
			if ( gubHeliState == HELI_DROP )
			{
				if ( !gfIngagedInDrop )
				{
					INT8 bEndVal = (gbHeliRound * NUM_PER_HELI_RUN);

					if ( bEndVal > gbNumHeliSeatsOccupied )
					{
						bEndVal = gbNumHeliSeatsOccupied;
					}

					// Flugente: watching mercs rope down one by one gets boring fast. So we allow up to 3 mercs to rope down  at the same time
					for ( int i = 0; i < 3; ++i )
					{
						// OK, Check if we have anybody left to send!
						if ( gbCurDrop < bEndVal )
						{
							// Flugente: it is now possible to use airdrops with soldiers after they have arrived in Arulco. In that case, they might have an animation that breaks EVENT_InitNewSoldierAnim prematurely.
							// In the worst case, this can cause the game to be unable to finish the airdrop. For that reason, we set all those soldiers to the STANDING aniamtion. 
							//MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->usAnimState = STANDING;
							MercPtrs[gusHeliSeats[gbCurDrop]]->EVENT_InitNewSoldierAnim( HELIDROP, 0, FALSE );

							// Change insertion code
							MercPtrs[gusHeliSeats[gbCurDrop]]->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
							MercPtrs[gusHeliSeats[gbCurDrop]]->usStrategicInsertionData = gsGridNoSweetSpot;

							// HEADROCK HAM 3.5: Externalized!
							UpdateMercInSector( MercPtrs[gusHeliSeats[gbCurDrop]], gWorldSectorX, gWorldSectorY, startingZ );

							// IF the first guy down, set squad!
							if ( gfFirstGuyDown )
							{
								gfFirstGuyDown = FALSE;
								//SetCurrentSquad( MercPtrs[ gusHeliSeats[ gbCurDrop ] ]->bAssignment, TRUE );
							}
							ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[MERC_HAS_ARRIVED_STR], MercPtrs[gusHeliSeats[gbCurDrop]]->GetName( ) );

							++gbCurDrop;

							gfIngagedInDrop = TRUE;
						}
						else
						{
							if ( gbExitCount == 0 )
							{
								gbExitCount = 2;
							}
							else
							{
								--gbExitCount;

								if ( gbExitCount == 1 )
								{
									// Goto leave
									gsHeliScript = -1;
									gubHeliState = HELI_ENDDROP;
								}
							}

							break;
						}
					}
				}
			}

			switch ( ubScriptCode )
			{
			case HELI_REST:

				break;

			case HELI_MOVE_DOWN:

				gdHeliZPos -= 1;
				gpHeli->pLevelNode->sRelativeZ = (INT16)gdHeliZPos;
				break;

			case HELI_MOVE_UP:

				gdHeliZPos += 1;
				gpHeli->pLevelNode->sRelativeZ = (INT16)gdHeliZPos;
				break;

			case HELI_MOVESMALL_DOWN:

				gdHeliZPos -= 0.25;
				gpHeli->pLevelNode->sRelativeZ = (INT16)gdHeliZPos;
				break;

			case HELI_MOVESMALL_UP:

				gdHeliZPos += 0.25;
				gpHeli->pLevelNode->sRelativeZ = (INT16)gdHeliZPos;
				break;


			case HELI_MOVEY:

				if ( gHeliEnterDirection == SOUTH )
				{
					gpHeli->sRelativeY -= 4;
				}
				else if ( gHeliEnterDirection == EAST )
				{
					gpHeli->sRelativeX -= 4;
					gpHeli->sRelativeY -= 1;
				}
				else if ( gHeliEnterDirection == WEST )
				{
					gpHeli->sRelativeX += 4;
					gpHeli->sRelativeY += 1;
				}
				else
				{
					gpHeli->sRelativeY += 4;
				}
				break;

			case HELI_MOVELARGERY:

				if ( gHeliEnterDirection == SOUTH )
				{
					gpHeli->sRelativeY -= 6;
				}
				else if ( gHeliEnterDirection == EAST )
				{
					gpHeli->sRelativeX -= 6;
					gpHeli->sRelativeY -= 1;
				}
				else if ( gHeliEnterDirection == WEST )
				{
					gpHeli->sRelativeX += 6;
					gpHeli->sRelativeY += 1;
				}
				else
				{
					gpHeli->sRelativeY += 6;
				}
				break;

			case HELI_GOTO_BEGINDROP:

				gsHeliScript = -1;
				gubHeliState = HELI_BEGINDROP;
				break;

			case HELI_SHOW_HELI:

				// Start animation
				memset( &AniParams, 0, sizeof(ANITILE_PARAMS) );
				AniParams.sGridNo = gsGridNoSweetSpot;
				AniParams.ubLevelID = ANI_SHADOW_LEVEL;
				AniParams.sDelay = 90;
				AniParams.sStartFrame = 0;
				AniParams.uiFlags = ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_LOOPING;
				AniParams.sX = gsHeliXPos;
				AniParams.sY = gsHeliYPos;
				AniParams.sZ = (INT16)gdHeliZPos;

				if ( gHeliEnterDirection == SOUTH )
				{
					strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_SOUTH.STI" );
				}
				else if ( gHeliEnterDirection == EAST )
				{
					strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_EAST.STI" );
				}
				else if ( gHeliEnterDirection == WEST )
				{
					strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH_WEST.STI" );
				}
				else
				{
					strcpy( AniParams.zCachedFile, "TILECACHE\\HELI_SH.STI" );
				}

				gpHeli = CreateAnimationTile( &AniParams );
				break;

			case HELI_GOTO_DROP:

				// Goto drop animation
				gdHeliZPos -= 0.25;
				gpHeli->pLevelNode->sRelativeZ = (INT16)gdHeliZPos;
				gsHeliScript = -1;
				gubHeliState = HELI_DROP;
				break;

			case HELI_GOTO_MOVETO:

				// Goto drop animation
				gsHeliScript = -1;
				gubHeliState = HELI_MOVETO;
				break;

			case HELI_GOTO_MOVEAWAY:

				// Goto drop animation
				gsHeliScript = -1;
				gubHeliState = HELI_MOVEAWAY;
				break;

			case HELI_GOTO_EXIT:

				if ( gbCurDrop < gbNumHeliSeatsOccupied )
				{
					// Start another run......
					INT16 sX, sY;

					ConvertGridNoToCenterCellXY( gsGridNoSweetSpot, &sX, &sY );

					if ( gHeliEnterDirection == SOUTH )
					{
						gsHeliXPos = sX - (2 * CELL_X_SIZE);
						gsHeliYPos = sY + (10 * CELL_Y_SIZE);
					}
					else if ( gHeliEnterDirection == EAST )
					{
						gsHeliXPos = sX + (10 * CELL_X_SIZE);
						gsHeliYPos = sY + (2 * CELL_Y_SIZE);
					}
					else if ( gHeliEnterDirection == WEST )
					{
						gsHeliXPos = sX - (10 * CELL_X_SIZE);
						gsHeliYPos = sY - (2 * CELL_Y_SIZE);
					}
					else
					{
						gsHeliXPos = sX - (2 * CELL_X_SIZE);
						gsHeliYPos = sY - (10 * CELL_Y_SIZE);
					}

					gdHeliZPos = 0;
					gsHeliScript = 0;
					gbExitCount = 0;
					gubHeliState = HELI_APPROACH;
					++gbHeliRound;

					// Ahh, but still delete the heli!
					DeleteAniTile( gpHeli );
					gpHeli = NULL;
				}
				else
				{
					// Goto drop animation
					gsHeliScript = -1;
					gubHeliState = HELI_EXIT;

					// Delete helicopter image!
					DeleteAniTile( gpHeli );
					gpHeli = NULL;
					gfIgnoreScrolling = FALSE;

					// Select our first guy
					SelectSoldier( gusHeliSeats[0], FALSE, TRUE );
				}
				break;

			case HELI_DONE:

				// End
				fFadingHeliOut = TRUE;

				// HEADROCK HAM 3.5: Update now, in case the LZ is still in a "RED" airspace sector. This is only
				// required if the sector is free of enemies... but still required. Will run immediately after the
				// helicopter is gone.
				UpdateAirspaceControl( );
				break;
			}

			++gsHeliScript;
		}
	}
}
