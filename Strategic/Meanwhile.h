#ifndef MEANWHILE_H
#define MEANWHILE_H


enum
{
	END_OF_PLAYERS_FIRST_BATTLE,
	DRASSEN_LIBERATED,
	CAMBRIA_LIBERATED,
	ALMA_LIBERATED,
	GRUMM_LIBERATED,
	CHITZENA_LIBERATED,
	NW_SAM,
	NE_SAM,
	CENTRAL_SAM,
	FLOWERS,
	LOST_TOWN,
	INTERROGATION,
	CREATURES,
	KILL_CHOPPER,
	AWOL_SCIENTIST,
	OUTSKIRTS_MEDUNA,
	BALIME_LIBERATED,
	NUM_MEANWHILES
};


typedef struct
{
	INT16				sSectorX;
	INT16				sSectorY;
	UINT16			usTriggerEvent;

	UINT8				ubMeanwhileID;
	UINT8				ubNPCNumber;

} MEANWHILE_DEFINITION;


void ScheduleMeanwhileEvent( MEANWHILE_DEFINITION *pMeanwhileDef, UINT32 uiTime );
void HandleFirstBattleVictory( void );

BOOLEAN BeginMeanwhile( UINT8 ubMeanwhileID );

void CheckForMeanwhileOKStart( );
void EndMeanwhile( );



BOOLEAN AreInMeanwhile( );
UINT8		GetMeanwhileID( );
BOOLEAN AreReloadingFromMeanwhile( );

void LocateToMeanwhileCharacter( );

// post meanwhile event for town liberation
void HandleMeanWhileEventPostingForTownLiberation( UINT8 bTownId );

// post meanwhile event for SAM liberation
void HandleMeanWhileEventPostingForSAMLiberation( INT8 bSAMId );

// trash world has been called, should we do the first meanwhile?
void HandleFirstMeanWhileSetUpWithTrashWorld( void );

// battle ended, check if we should set up a meanwhile?
void HandleFirstBattleEndingWhileInTown( INT16 sSectorX, INT16 sSectorY, INT16 bSectorZ, BOOLEAN fFromAutoResolve );

// lost an entire town to the enemy!
void HandleMeanWhileEventPostingForTownLoss( UINT8 bTownId );

// handle short cutting past a meanwhilescene while it is being handled
void HandleShortCuttingOfMeanwhileSceneByPlayer( UINT8 ubMeanwhileID, INT32 iLastProfileId, INT32 iLastProfileAction );

// handle release of creatures meanwhile
void HandleCreatureRelease( void );

// handle sending flowers to the queen
void HandleFlowersMeanwhileScene( INT8 bTimeCode );

// player reaches the outskirts of Meduna
void HandleOutskirtsOfMedunaMeanwhileScene( void );

// let player know about Madlab after certain status % reached
void HandleScientistAWOLMeanwhileScene( void );

// handle chopper used meanwhile 
void HandleKillChopperMeanwhileScene( void );

// handle interrogation meanwhile 
void HandleInterrogationMeanwhileScene( void );

extern	MEANWHILE_DEFINITION	gCurrentMeanwhileDef;
extern	MEANWHILE_DEFINITION	gMeanwhileDef[NUM_MEANWHILES];
extern	BOOLEAN								gfMeanwhileScheduled;
extern	BOOLEAN								gfMeanwhileTryingToStart;
extern	BOOLEAN								gfInMeanwhile;
extern	UINT32								uiMeanWhileFlags;

#endif




 


