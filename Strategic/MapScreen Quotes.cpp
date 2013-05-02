#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Map Screen Interface.h"
	#include "string.h"
	#include "Map Screen Interface Map.h"
	#include "Font Control.h"
	#include "Assignments.h"
	#include "Soldier Control.h"
	#include "Overhead.h"
	#include "Squads.h"
	#include "Soundman.h"
	#include "Message.h"
	#include "PopUpBox.h"
	#include "Handle Items.h"
	#include "Interface Items.h"
	#include "Isometric Utils.h"
	#include "Interface.h"
	#include "Game Event Hook.h"
	#include "Tactical Save.h"
	#include "Quests.h"
	#include "Strategicmap.h"
	#include "Soldier Profile.h"
	#include "Strategic Movement.h"
	#include "Dialogue Control.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Interface Bottom.h"
	#include "Vehicles.h"
	#include "LINE.H"
	#include "Text.h"
	#include "gameloop.h"
	#include "Map Screen Helicopter.h"
	#include "PreBattle Interface.h"
	#include "WordWrap.h"
	#include "GameSettings.h"
	#include "Campaign Types.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "strategic.h"
	#include "Keys.h"
	#include "Soldier macros.h"
	#include "Militia Control.h"
	#include "Random.h"
	#include "Cursor Control.h"
	#include "renderworld.h"
	#include "Strategic Mines.h"
	#include "strategic.h"
	#include "Air Raid.h"
	#include "Queen Command.h"
	#include "Render Fun.h"
#endif

#include "connect.h"

	#ifdef JA2UB

#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#include "ub_config.h"
#include "Ja25Update.h"


#include "MapScreen Quotes.h"
//******  Global Variables  *****************************************

UINT16	gusWaitingQuote=MILO_NO_QUOTE;
UINT16	gusJerryDialogueDelay = 0;



//*******  Function Prototypes ***************************************

BOOLEAN Internal_JerryMeloCharacterDialogue( UINT16 usQuoteNum );
BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained();
BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained2ndTime();
BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasComplained2ndTime();
BOOLEAN DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet();
//BOOLEAN DoesPlayerHaveExactly6MercsOrLessThen6MercsAndJerryHasDoubleChecked();
BOOLEAN DoesPlayerHaveLessThen6MercsAndJerryHasDoubleChecked();
BOOLEAN DoesPlayerHaveExactly6MercsAndNotSaidQuoteBefore();
BOOLEAN HasJerryAlreadySaidTheMapScreenIntroSequence();
BOOLEAN HasJerrySaidTheLetsGoToTraconaQuote();


//ppp

//*******  Functions **************************************************


//void JerryMiloTalk( UINT16 usQuoteNum, UINT32 uiDelay )
void JerryMiloTalk( UINT16 usQuoteNum )
{
//	gusWaitingQuote = usQuoteNum;

	// have skyrider talk to player
	Internal_JerryMeloCharacterDialogue( usQuoteNum );

	//set that jerry has said the quote
	JerryMiloSaidQuote( usQuoteNum );

	return;
}

void JerryMiloDelayedTalk( UINT16 usQuoteNum, UINT32 uiDelay )
{
	gusWaitingQuote = usQuoteNum;
	gusJerryDialogueDelay = uiDelay;

	return;
}


void HandleJerryMiloQuotes( BOOLEAN fReset )
{
    
	static UINT32 uiLastCount;
	UINT32	uiCurrentTime = GetJA2Clock();

    
	if( fReset )
	{
		uiLastCount = 0;
		gusWaitingQuote = MILO_NO_QUOTE;
		return;
	}
	
//	if ( gGameUBOptions.InGameHeliCrash == TRUE )
//	{
	
 //   return;//AA
	if( gusWaitingQuote == MILO_NO_QUOTE )
	{
		return;
	}

	if( uiLastCount == 0 )
	{
		uiLastCount = GetJA2Clock();
		return;
	}

	if( ( uiCurrentTime - uiLastCount ) > gusJerryDialogueDelay )
	{
		JerryMiloTalk( gusWaitingQuote );

		//if the quote waiting to be said was the opening quote
		if( gusWaitingQuote == MILO_QUOTE__OPENING_GREETING_PART_1 )
		{
			//say the follow up part
			if( DoesPlayerHaveExactly6MercsAndNotSaidQuoteBefore() )
			{
				JerryMiloTalk( MILO_QUOTE__ALREADY_HAS_6_MERCS );
			}
			else
			{
				JerryMiloTalk( MILO_QUOTE__OPENING_GREETING_PART_2 );
			}
		}

		gusWaitingQuote = MILO_NO_QUOTE;

		uiLastCount = uiCurrentTime;
	}
	
//	}
}

BOOLEAN Internal_JerryMeloCharacterDialogue( UINT16 usQuoteNum )
{
	BOOLEAN fRetVal = FALSE;
	fTeamPanelDirty = TRUE;

	fRetVal = CharacterDialogue( 76, usQuoteNum, uiExternalStaticNPCFacesUB[ JERRY_MELO_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );

	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_JERRY_MILO, usQuoteNum, 0, 0, uiExternalStaticNPCFacesUB[ JERRY_MELO_FACE ], DIALOGUE_EXTERNAL_NPC_UI );

	return( fRetVal );
}

BOOLEAN HasJerryMiloSaidQuoteBefore( UINT32 uiQuoteID )
{
	//has the quote been said
	return( ( gJa25SaveStruct.uiJerryMiloQuotesFlags & ( 1 << uiQuoteID ) ) != 0 );
}


void JerryMiloSaidQuote( UINT32 uiQuoteID )
{
	gJa25SaveStruct.uiJerryMiloQuotesFlags |= ( 1 << uiQuoteID );
}


BOOLEAN WillJerryMiloAllowThePlayerToCompressTimeAtBeginingOfGame()
{

if ( gGameUBOptions.JerryQuotes == TRUE )
{
	//to speed things up, has jerry finished the intro speech?
	if( HasJerryAlreadySaidTheMapScreenIntroSequence() )
		return( TRUE );


	//if the player has more then 6merc and Jerry hasnt complained the first time
	if( DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained() )
	{
		return( FALSE );
	}

	//if the player has over 6 mercs and jerry HAS NOT complained the 2nd time
	if( DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained2ndTime() )
	{
		return( FALSE );
	}

	//if the player has over 6 mercs and jerry HAS complained the 2nd time
	if( DoesPlayerHaveMoreThen6MercsAndJerryHasComplained2ndTime() )
	{
		return( FALSE );
	}

	//if the player has less then 6 mercs, and hasnt double checked yet
	if( DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet() )
	{
		return( FALSE );
	}

/*
	//if the player has exactly 6 mercs OR the player has less then 6 AND has double checked
	if( DoesPlayerHaveExactly6MercsOrLessThen6MercsAndJerryHasDoubleChecked() )
	{
		return( FALSE );
	}
*/

	//if the player has exactly 6 mercs OR the player has less then 6 AND has double checked
	if( DoesPlayerHaveLessThen6MercsAndJerryHasDoubleChecked() )
	{
		return( FALSE );
	}

	//if the player has exactly 6 mercs OR the player has less then 6 AND has double checked
	if( DoesPlayerHaveExactly6MercsAndNotSaidQuoteBefore() )
	{
		return( FALSE );
	}


	if( !HasJerrySaidTheLetsGoToTraconaQuote() )
	{
		return( FALSE );
	}
}
	return( TRUE );
}

BOOLEAN HasJerrySaidTheLetsGoToTraconaQuote()
{
	return( HasJerryMiloSaidQuoteBefore( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE ) );
}

void HaveJerrySayWhyPlayerCantTimeCompressAtBeginningOfGame()
{
	if( !gfFirstTimeInGameHeliCrash)
	{
		INT32 i=0;
	}

	/*
	if (gGameOptions.ubSquadSize > 6)
	{		
		if( DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS );
			return;
		}
		//else if the player has less then 6 mercs, and hasnt double checked yet
		else if( DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS );
			return;
		}
		//if the player has less then 6 AND has double checked
		else if( DoesPlayerHaveLessThen6MercsAndJerryHasDoubleChecked() )
		{
			JerryMiloTalk( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE );
			return;
		}
		else
		{
			JerryMiloTalk( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE );
			return;
		}
	}
	else
	*/
	{
		//if the player has more then 6merc and Jerry hasnt complained the first time
		if( DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS );
			return;
		}

		//else if the player has over 6 mercs and jerry HAS NOT complained the 2nd time
		else if( DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained2ndTime() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS_2ND_TIME );
			return;
		}

		//else if the player has over 6 mercs and jerry HAS complained the 2nd time
		else if( DoesPlayerHaveMoreThen6MercsAndJerryHasComplained2ndTime() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS_2ND_TIME );
			return;
		}

		//else if the player has less then 6 mercs, and hasnt double checked yet
		else if( DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet() )
		{
			JerryMiloTalk( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS );
			return;
		}

		//if the player has less then 6 AND has double checked
		else if( DoesPlayerHaveLessThen6MercsAndJerryHasDoubleChecked() )
		{
			JerryMiloTalk( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE );
			return;
		}

		else if( !HasJerrySaidTheLetsGoToTraconaQuote() )
		{
			JerryMiloTalk( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE );
			return;
		}
	}

	//shoud satisfy condition above
	Assert( 0 );
}


BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has over 6 mercs, and jerry hasnt complained about that yet
	if( ubNumMercsOnTeam > 6 && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasntComplained2ndTime()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has over 6 mercs and jerry HAS NOT complained the 2nd time
	if( ubNumMercsOnTeam > 6 && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS_2ND_TIME ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN DoesPlayerHaveMoreThen6MercsAndJerryHasComplained2ndTime()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has over 6 mercs and jerry HAS complained the 2nd time
	if( ubNumMercsOnTeam > 6 && HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_MORE_THEN_6_MERCS_2ND_TIME ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN DoesPlayerHaveLessThen6MercsAndJerryHasNotDoubleCheckedYet()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has less then 6 mercs, and hasnt double checked yet
	if( ubNumMercsOnTeam < 6 && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN DoesPlayerHaveExactly6MercsAndNotSaidQuoteBefore()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has exactly 6 mercs OR the player has less then 6 AND has double checked
	if( ubNumMercsOnTeam == 6 && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__ALREADY_HAS_6_MERCS ) && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS ) )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN DoesPlayerHaveLessThen6MercsAndJerryHasDoubleChecked()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has exactly 6 mercs OR the player has less then 6 AND has double checked
	if( ubNumMercsOnTeam < 6 && HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_LESS_THEN_6_MERCS ) )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN DoesPlayerHaveNoMercsHiredAndJerryHasntSaidQuoteYet()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//if the player has less then 6 mercs, and hasnt double checked yet
	//if( ubNumMercsOnTeam == 0 && !HasJerryMiloSaidQuoteBefore( MILO_QUOTE__PLAYER_HAS_NO_MERCS ) )
    if( ubNumMercsOnTeam == 0 )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN DidPlayerInitiallyHaveLessThen6MercsAndNowHaveExactly6AndHasntSaidFullLoadQuote()
{
	UINT8	ubNumMercsOnTeam = NumberOfMercsOnPlayerTeam();

	//If we have been to the sector, make sure we dont say it
	
	//if( SectorInfo[ SEC_H7].uiFlags & SF_HAS_ENTERED_TACTICAL )
	if( SectorInfo[ (UINT8)SECTOR(  gGameExternalOptions.ubDefaultArrivalSectorX,  gGameExternalOptions.ubDefaultArrivalSectorY ) ].uiFlags & SF_HAS_ENTERED_TACTICAL )
	{
		return( FALSE );
	}

	//if the player has 6 mercs, and has said the regular opening line ( not the line with 6 mercs )
	if( ubNumMercsOnTeam == 6 && 
			HasJerryMiloSaidQuoteBefore( MILO_QUOTE__OPENING_GREETING_PART_2 ) && 
			!HasJerryMiloSaidQuoteBefore( MILO_QUOTE__ALREADY_HAS_6_MERCS ) )
	{
		return( TRUE );
	}

	return( FALSE );
}


void InitJerryQuotes()
{
	//Clear out the quote flags 
	gJa25SaveStruct.uiJerryMiloQuotesFlags = 0;
}

BOOLEAN HasJerryAlreadySaidTheMapScreenIntroSequence()
{
   // return TRUE; //AA
	if( HasJerryMiloSaidQuoteBefore( MILO_QUOTE__HEADED_TO_TRACONA_QUOTE ) || gGameUBOptions.InGameHeliCrash == FALSE )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

#endif
