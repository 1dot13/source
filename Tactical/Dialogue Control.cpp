#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "PreBattle Interface.h"
#else
	#include "sgp.h"
	//#include "soldier control.h"
	#include "Encrypted File.h"
	#include "faces.h"
	#include "wcheck.h"
	#include "gap.h"
	#include "overhead.h"
	#include "Sound Control.h"
	#include "dialogue control.h"
	#include "Message.h"
	#include "render dirty.h"
	#include "soldier profile.h"
	#include "wordwrap.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "AimMembers.h"
	#include "mercs.h"
	#include "interface dialogue.h"
	#include "merctextbox.h"
	#include "renderworld.h"
	#include "soldier macros.h"
	#include "squads.h"
	#include "screenids.h"
	#include "interface utils.h"
	#include "strategicmap.h"
	#include "PreBattle Interface.h"
	#include "Game Clock.h"
	#include "Quests.h"
	#include "cursors.h"
	#include "gamescreen.h"
	#include "Random.h"
	#include "Map Screen Helicopter.h"
	#include	"GameSettings.h"
	#include "ShopKeeper Interface.h"
	#include "Map Screen Interface.h"
	#include "text.h"
	#include "Merc Contract.h"
	#include "Town Militia.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "Interface Control.h"
	#include "finances.h"
	#include "civ quotes.h"
	#include "Map Screen Interface Map.h"
	#include "opplist.h"
	#include "ai.h"
	#include "worldman.h"
	#include "Map Screen Interface Bottom.h"
	#include "Campaign.h"
	#include "end game.h"
	#include "los.h"
	#include "qarray.h"
	#include "Soldier Profile.h"
#endif
#include <vector>
#include "Auto Resolve.h"

#include "connect.h"
#ifdef JA2UB
#include "Intro.h"
#include "MapScreen Quotes.h"
#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#include "Animation Control.h"
#endif

#include "ub_config.h"

#include "history.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define		DIALOGUESIZE					480
#define	QUOTE_MESSAGE_SIZE		520

#define		TALK_PANEL_FACE_X				6
#define		TALK_PANEL_FACE_Y				9
#define		TALK_PANEL_NAME_X				5
#define		TALK_PANEL_NAME_Y				114
#define		TALK_PANEL_NAME_WIDTH		92
#define		TALK_PANEL_NAME_HEIGHT	15
#define		TALK_PANEL_MENU_STARTY	8
#define		TALK_PANEL_MENU_HEIGHT	24
#define		TALK_MENU_WIDTH					96
#define		TALK_MENU_HEIGHT				16

#define		DIALOGUE_DEFAULT_SUBTITLE_WIDTH		200
#define		TEXT_DELAY_MODIFIER			60

typedef struct
{
	UINT16	usQuoteNum;
	UINT8		ubCharacterNum;
	INT8		bUIHandlerID;
	INT32		iFaceIndex;
	INT32		iTimeStamp;
	UINT32	uiSpecialEventFlag;
	UINT32	uiSpecialEventData;
	UINT32	uiSpecialEventData2;
	UINT32	uiSpecialEventData3;
	UINT32	uiSpecialEventData4;
	BOOLEAN	fFromSoldier;
	BOOLEAN	fDelayed;
	BOOLEAN fPauseTime;
} DIALOGUE_Q_STRUCT, *DIALOGUE_Q_STRUCT_PTR;

extern INT32 giMapInvPrev;
extern INT32 giMapInvNext;
extern BOOLEAN gfSKIScreenExit;
extern SOLDIERTYPE *pProcessingSoldier;
extern BOOLEAN fProcessingAMerc;
extern UINT32 guiPendingScreen;
extern BOOLEAN fReDrawFace;
extern BOOLEAN gfWaitingForTriggerTimer;


BOOLEAN fExternFacesLoaded = FALSE;


// head miners' external profile ids have been moved to lua script
std::vector<UINT32> uiExternalStaticNPCFaces;
//UINT32 uiExternalStaticNPCFaces[ NUMBER_OF_EXTERNAL_NPC_FACES ];
//UINT32 uiExternalFaceProfileIds[ NUMBER_OF_EXTERNAL_NPC_FACES ]=
//{
//	97,
//	106,
//	148,
//	156,
//	157,
//	158,
//};
#ifdef JA2UB
UINT32 uiExternalStaticNPCFacesUB[ NUMBER_OF_EXTERNAL_NPC_FACES ];
UINT32 uiExternalFaceProfileIdsUB[ NUMBER_OF_EXTERNAL_NPC_FACES ]=
{
	97,
	106,
	148,
	156,
	157,
	158,
	76,					//Jerry Melo
};


BOOLEAN AreAllTheMercsFinishedSayingThereInitialHeliCrashQuotes();
void		InitJerriesSpeechCallBack();
void		HandlePlayerClosingMorrisNoteDisplayedOnScreen();
#endif

UINT8	gubMercValidPrecedentQuoteID[ NUMBER_VALID_MERC_PRECEDENT_QUOTES ] =
					{ 80, 81, 82, 83, 86, 87, 88, 95, 97, 99, 100, 101, 102 };


extern INT32 iInterfaceDialogueBox;
extern BOOLEAN	gfRerenderInterfaceFromHelpText;
extern UINT32 guiSKI_TransactionButton;

UINT16	gusStopTimeQuoteList[ ] =
{
	QUOTE_BOOBYTRAP_ITEM,
	QUOTE_SUSPICIOUS_GROUND
};

UINT8							gubNumStopTimeQuotes = 2;


// QUEUE UP DIALOG!
#define		INITIAL_Q_SIZE				10
HQUEUE		ghDialogueQ						= NULL;
FACETYPE	*gpCurrentTalkingFace	= NULL;
UINT8			gubCurrentTalkingID	= NO_PROFILE;
INT8			gbUIHandlerID;

INT32				giNPCReferenceCount = 0;
INT32				giNPCSpecialReferenceCount = 0;

INT16		gsExternPanelXPosition	 = DEFAULT_EXTERN_PANEL_X_POS;
INT16		gsExternPanelYPosition	 = DEFAULT_EXTERN_PANEL_Y_POS;

BOOLEAN			gfDialogueQueuePaused = FALSE;
UINT16			gusSubtitleBoxWidth;
UINT16			gusSubtitleBoxHeight;
INT32				giTextBoxOverlay = -1;
BOOLEAN			gfFacePanelActive = FALSE;
UINT32			guiScreenIDUsedWhenUICreated;
CHAR16					gzQuoteStr[ QUOTE_MESSAGE_SIZE ];
CHAR8					gzSoundString[164];
MOUSE_REGION	gTextBoxMouseRegion;
MOUSE_REGION	gFacePopupMouseRegion;
BOOLEAN				gfUseAlternateDialogueFile = FALSE;

// set the top position value for merc dialogue pop up boxes
INT16 gsTopPosition = 20;


INT32 iDialogueBox = -1;
void RenderSubtitleBoxOverlay( VIDEO_OVERLAY *pBlitter );
void RenderFaceOverlay( VIDEO_OVERLAY *pBlitter );


extern BOOLEAN ContinueDialogue(SOLDIERTYPE *pSoldier, BOOLEAN fDone );
extern void HandlePendingInitConv( );
extern BOOLEAN WillMercRenew( SOLDIERTYPE *pSoldier, BOOLEAN fSayQuote );
extern void DrawFace( INT16 sCharNumber );
extern void LuaHandleReplaceQuote( UINT8 ubProfile, UINT16 usQuoteNum );

// the next said quote will pause time
BOOLEAN fPausedTimeDuringQuote = FALSE;
BOOLEAN fWasPausedDuringDialogue = FALSE;
extern BOOLEAN gfLockPauseState;

INT8	gubLogForMeTooBleeds = FALSE;


// has the text region been created?
BOOLEAN fTextBoxMouseRegionCreated = FALSE;
BOOLEAN	fExternFaceBoxRegionCreated = FALSE;

// due to last quote system?
BOOLEAN fDialogueBoxDueToLastMessage = FALSE;

// last quote timers
UINT32 guiDialogueLastQuoteTime = 0;
UINT32 guiDialogueLastQuoteDelay = 0;

void CheckForStopTimeQuotes( UINT16 usQuoteNum );

void TextOverlayClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
void FaceOverlayClickCallback( MOUSE_REGION * pRegion, INT32 iReason );


// Handler functions for tactical ui diaplay
void HandleTacticalTextUI( INT32 iFaceIndex, SOLDIERTYPE *pSoldier, STR16 zQuoteStr );
void HandleTacticalNPCTextUI( UINT8 ubCharacterNum, STR16 zQuoteStr );
void HandleTacticalSpeechUI( UINT8 ubCharacterNum, INT32 iFaceIndex );

void DisplayTextForExternalNPC(	UINT8 ubCharacterNum, STR16 zQuoteStr );
void CreateTalkingUI( INT8 bUIHandlerID, INT32 iFaceIndex, UINT8 ubCharacterNum, SOLDIERTYPE *pSoldier, STR16 zQuoteStr );

#ifdef JA2UB
BOOLEAN AreAllTheMercsFinishedSayingThereInitialHeliCrashQuotes();
void		InitJerriesSpeechCallBack();
void		HandlePlayerClosingMorrisNoteDisplayedOnScreen();
#endif

void HandleExternNPCSpeechFace( INT32 iIndex );



extern BOOLEAN ContinueDialogue(SOLDIERTYPE *pSoldier, BOOLEAN fDone );
extern	BOOLEAN		DoSkiMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback );



void UnPauseGameDuringNextQuote( void )
{
	fPausedTimeDuringQuote = FALSE;

	return;
}


void PauseTimeDuringNextQuote( void )
{
	fPausedTimeDuringQuote = TRUE;

	return;
}

BOOLEAN DialogueActive( )
{
	if ( gpCurrentTalkingFace != NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN InitalizeDialogueControl()
{
	ghDialogueQ = CreateQueue( INITIAL_Q_SIZE, sizeof( DIALOGUE_Q_STRUCT_PTR ) );

	// Initalize subtitle popup box
	//

	giNPCReferenceCount = 0;


	if ( ghDialogueQ == NULL )
	{
		return( FALSE );
	}
	else
	{
		return( TRUE );
	}
}

void ShutdownDialogueControl()
{
	if( ghDialogueQ != NULL )
	{
		// Empty
		EmptyDialogueQueue( );

		// Delete
		DeleteQueue( ghDialogueQ );
		ghDialogueQ=NULL;
	}

	// shutdown external static NPC faces
	ShutdownStaticExternalNPCFaces( );

	// gte rid of portraits for cars
	UnLoadCarPortraits( );
	//
}

void InitalizeStaticExternalNPCFaces( void )
{
	// go and grab all external NPC faces that are needed for the game who won't exist as soldiertypes

	if( fExternFacesLoaded )
		return;
	
	//
	// Code for loading miners' faces has been moved to LuaMines::InitMinerFaces ...
	//
#ifdef JA2UB
	for( INT32 iCounter = 0; iCounter < NUMBER_OF_EXTERNAL_NPC_FACES; ++iCounter )
	{
		uiExternalStaticNPCFacesUB[ iCounter ] = ( UINT32 )InitFace( ( UINT8 )( uiExternalFaceProfileIdsUB[ iCounter ] ), NOBODY, FACE_FORCE_SMALL );
	}
#endif

	// ... put Skyrider's face at index 0
	uiExternalStaticNPCFaces.push_back(( UINT32 )InitFace( ( UINT8 )( SKYRIDER ), NOBODY, FACE_FORCE_SMALL ));
	// anv: and put Waldo too
	uiExternalStaticNPCFaces.push_back(( UINT32 )InitFace( ( UINT8 )( WALDO ), NOBODY, FACE_FORCE_SMALL ));

	fExternFacesLoaded = TRUE;
}

void ShutdownStaticExternalNPCFaces( void )
{
	if ( !fExternFacesLoaded )
		return;

#ifdef JA2UB
	for( INT32 iCounter = 0; iCounter < NUMBER_OF_EXTERNAL_NPC_FACES; ++iCounter )
	{
		DeleteFace( uiExternalStaticNPCFacesUB[ iCounter ] );
	}
#endif

	for (std::vector<UINT32>::iterator face = uiExternalStaticNPCFaces.begin();
		face != uiExternalStaticNPCFaces.end(); ++face)
	{
		DeleteFace(*face);
	}

	uiExternalStaticNPCFaces.clear();

	fExternFacesLoaded = FALSE;
}


void EmptyDialogueQueue( )
{
	// If we have anything left in the queue, remove!
	if( ghDialogueQ != NULL )
	{
/*
DEF:	commented out because the Queue system ?? uses a contiguous memory block ??? for the queue
	so you cant delete a single node.	The DeleteQueue, below, will free the entire memory block

		numDialogueItems = QueueSize( ghDialogueQ );

		for ( cnt = numDialogueItems-1; cnt >= 0; cnt-- )
		{
			if ( PeekQueue( ghDialogueQ, &QItem ) )
			{
					MemFree( QItem );
			}
		}
*/

		// Delete list
		DeleteQueue( ghDialogueQ );
		ghDialogueQ=NULL;

		// Recreate list
		ghDialogueQ = CreateQueue( INITIAL_Q_SIZE, sizeof( DIALOGUE_Q_STRUCT_PTR ) );

	}

	gfWaitingForTriggerTimer = FALSE;
}


BOOLEAN DialogueQueueIsEmpty( )
{
	if( ghDialogueQ != NULL )
	{
		INT32 numDialogueItems = QueueSize( ghDialogueQ );

		if ( numDialogueItems == 0 )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


BOOLEAN	DialogueQueueIsEmptyOrSomebodyTalkingNow( )
{
	if ( gpCurrentTalkingFace != NULL )
	{
		return( FALSE );
	}

	if ( !DialogueQueueIsEmpty( ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

void DialogueAdvanceSpeech( )
{
	// Shut them up!
	InternalShutupaYoFace( gpCurrentTalkingFace->iID, FALSE );
}


void StopAnyCurrentlyTalkingSpeech( )
{
	// ATE; Make sure guys stop talking....
	if ( gpCurrentTalkingFace != NULL )
	{
		InternalShutupaYoFace( gpCurrentTalkingFace->iID, TRUE );
	}
#ifdef JA2UB	
	RemoveJerryMiloBrokenLaptopOverlay();
#endif
}


// ATE: Handle changes like when face goes from
// 'external' to on the team panel...
void HandleDialogueUIAdjustments( )
{
	SOLDIERTYPE							*pSoldier;

	// OK, check if we are still taking
	if ( gpCurrentTalkingFace != NULL )
	{
		if ( gpCurrentTalkingFace->fTalking )
		{
			// ATE: Check for change in state for the guy currently talking on 'external' panel....
			if ( gfFacePanelActive )
			{
				pSoldier = FindSoldierByProfileID( gubCurrentTalkingID, FALSE );

				if ( pSoldier )
				{
					if ( 0 )
					{
						// A change in plans here...
						// We now talk through the interface panel...
						if ( gpCurrentTalkingFace->iVideoOverlay != -1 )
						{
							RemoveVideoOverlay( gpCurrentTalkingFace->iVideoOverlay );
							gpCurrentTalkingFace->iVideoOverlay = -1;
						}
						gfFacePanelActive = FALSE;

						RemoveVideoOverlay( giTextBoxOverlay );
						giTextBoxOverlay = -1;

						if ( fTextBoxMouseRegionCreated )
						{
							MSYS_RemoveRegion( &gTextBoxMouseRegion );
							fTextBoxMouseRegionCreated = FALSE;
						}


						// Setup UI again!
						CreateTalkingUI( gbUIHandlerID, pSoldier->iFaceIndex, pSoldier->ubProfile, pSoldier, gzQuoteStr );
					}
				}
			}
		}
	}
}



void HandleDialogue( )
{
	INT32 iQSize;
	DIALOGUE_Q_STRUCT				*QItem;
	static BOOLEAN					fOldEngagedInConvFlagOn = FALSE;
	BOOLEAN fDoneTalking = FALSE;
	SOLDIERTYPE *pSoldier = NULL;
	CHAR16	zText[ 512 ];
	CHAR16	zMoney[128];

	// we don't want to just delay action of some events, we want to pause the whole queue, regardless of the event
	if( gfDialogueQueuePaused )
	{
		return;
	}

	iQSize = QueueSize( ghDialogueQ );

	if ( iQSize == 0 && gpCurrentTalkingFace == NULL )
	{
		HandlePendingInitConv( );
	}

	HandleCivQuote( );

	// Alrighty, check for a change in state, do stuff appropriately....
	// Turned on
	if ( fOldEngagedInConvFlagOn == FALSE && ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
	{
		// OK, we have just entered...
		fOldEngagedInConvFlagOn = TRUE;

		// pause game..
		PauseGame();
		LockPauseState( 14 );
	}
	else if ( fOldEngagedInConvFlagOn == TRUE && !( gTacticalStatus.uiFlags & ENGAGED_IN_CONV ) )
	{
		// OK, we left...
		fOldEngagedInConvFlagOn = FALSE;

		// Unpause game..
		UnLockPauseState();
		UnPauseGame();

		// if we're exiting boxing with the UI lock set then DON'T OVERRIDE THIS!
		if ( !(gTacticalStatus.bBoxingState == WON_ROUND || gTacticalStatus.bBoxingState == LOST_ROUND || gTacticalStatus.bBoxingState == DISQUALIFIED) && !( gTacticalStatus.uiFlags & IGNORE_ENGAGED_IN_CONV_UI_UNLOCK ) )
		{
			guiPendingOverrideEvent = LU_ENDUILOCK;
			HandleTacticalUI( );

			// ATE: If this is NOT the player's turn.. engage AI UI lock!
			if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum )
			{
				// Setup locked UI
				guiPendingOverrideEvent = LU_BEGINUILOCK;
				HandleTacticalUI( );
			}
		}

		gTacticalStatus.uiFlags &= ( ~IGNORE_ENGAGED_IN_CONV_UI_UNLOCK );
	}


	if ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )
	{
		// Are we in here because of the dialogue system up?
		if ( !gfInTalkPanel )
		{
			// ATE: NOT if we have a message box pending....
			if ( guiPendingScreen != MSG_BOX_SCREEN && guiCurrentScreen != MSG_BOX_SCREEN	)
			{
				// No, so we should lock the UI!
				guiPendingOverrideEvent = LU_BEGINUILOCK;
				HandleTacticalUI( );
			}
		}
	}

	// OK, check if we are still talking
	if ( gpCurrentTalkingFace != NULL )
	{
		if ( gpCurrentTalkingFace->fTalking )
		{
			// ATE: OK, MANAGE THE DISPLAY OF OUR CURRENTLY ACTIVE FACE IF WE / IT CHANGES STATUS
			// THINGS THAT CAN CHANGE STATUS:
			//		CHANGE TO MAPSCREEN
			//		CHANGE TO GAMESCREEN
			//		CHANGE IN MERC STATUS TO BE IN A SQUAD
			//	CHANGE FROM TEAM TO INV INTERFACE

			// Where are we and where did this face once exist?
			if ( guiScreenIDUsedWhenUICreated == GAME_SCREEN && guiCurrentScreen == MAP_SCREEN )
			{
				// GO FROM GAMESCREEN TO MAPSCREEN
				// REMOVE OLD UI
				// Set face inactive!
				//gpCurrentTalkingFace->fCanHandleInactiveNow = TRUE;
				//SetAutoFaceInActive( gpCurrentTalkingFace->iID );
				//gfFacePanelActive = FALSE;

				// delete face panel if there is one!
				if ( gfFacePanelActive )
				{
					// Set face inactive!
					if ( gpCurrentTalkingFace->iVideoOverlay != -1 )
					{
						RemoveVideoOverlay( gpCurrentTalkingFace->iVideoOverlay );
						gpCurrentTalkingFace->iVideoOverlay = -1;
					}

					if ( fExternFaceBoxRegionCreated )
					{
						fExternFaceBoxRegionCreated = FALSE;
						MSYS_RemoveRegion(&(gFacePopupMouseRegion) );
					}

					// Set face inactive....
					gpCurrentTalkingFace->fCanHandleInactiveNow = TRUE;
					SetAutoFaceInActive( gpCurrentTalkingFace->iID );
					HandleTacticalSpeechUI( gubCurrentTalkingID, gpCurrentTalkingFace->iID );

			// ATE: Force mapscreen to set face active again.....
			fReDrawFace = TRUE;
				DrawFace( bSelectedInfoChar );

					gfFacePanelActive = FALSE;
				}

				guiScreenIDUsedWhenUICreated = guiCurrentScreen;
			}
			else if ( guiScreenIDUsedWhenUICreated == MAP_SCREEN && guiCurrentScreen == GAME_SCREEN )
			{
				HandleTacticalSpeechUI( gubCurrentTalkingID, gpCurrentTalkingFace->iID );
				guiScreenIDUsedWhenUICreated = guiCurrentScreen;
			}
			return;
		}
		else
		{
			// Check special flags
			// If we are done, check special face flag for trigger NPC!
			if ( gpCurrentTalkingFace->uiFlags & FACE_PCTRIGGER_NPC )
			{
				// Decrement refrence count...
				giNPCReferenceCount--;

				TriggerNPCRecord( (UINT8)gpCurrentTalkingFace->uiUserData1, (UINT8)gpCurrentTalkingFace->uiUserData2 );
				//Reset flag!
				gpCurrentTalkingFace->uiFlags &= (~FACE_PCTRIGGER_NPC );
			}

			if ( gpCurrentTalkingFace->uiFlags & FACE_MODAL )
			{
			gpCurrentTalkingFace->uiFlags &= (~FACE_MODAL );

				EndModalTactical( );

				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Ending Modal Tactical Quote." );

			}

			if ( gpCurrentTalkingFace->uiFlags & FACE_TRIGGER_PREBATTLE_INT )
			{
				UnLockPauseState();
				InitPreBattleInterface( (GROUP*)gpCurrentTalkingFace->uiUserData1, TRUE );
				//Reset flag!
				gpCurrentTalkingFace->uiFlags &= (~FACE_TRIGGER_PREBATTLE_INT );
			}

			gpCurrentTalkingFace = NULL;
			gubCurrentTalkingID	= NO_PROFILE;
			gTacticalStatus.ubLastQuoteProfileNUm = NO_PROFILE;
			fDoneTalking = TRUE;
		}
	}

	if( ( fDoneTalking ) && ( fWasPausedDuringDialogue ) )
	{
		fWasPausedDuringDialogue = FALSE;

		// unlock pause state
		UnLockPauseState();
		UnPauseGame();

	}
	
#ifdef JA2UB	
	if ( iQSize == 0 )
	{
		if( gfMorrisShouldSayHi )
		{
			SOLDIERTYPE * pMorris;
			SOLDIERTYPE * pSoldier;
			INT16	sPlayerGridNo;
			UINT8	ubPlayerID;
			UINT8	ubQualifiedSoldierIDArray[ NUM_MERCS_WITH_NEW_QUOTES ];
			UINT8	ubNumQualifiedMercs=0;
			UINT8	ubCnt=0;

			if( !( gMercProfiles[ MORRIS_UB ].ubMiscFlags2 & PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE ) )
			{
				pMorris = FindSoldierByProfileID( MORRIS_UB, FALSE );
				if ( pMorris && pMorris->stats.bLife >= OKLIFE )
				{
					sPlayerGridNo = ClosestPC( pMorris, NULL );
					if (sPlayerGridNo != NOWHERE )
					{
						ubPlayerID = WhoIsThere2( sPlayerGridNo, 0 );
						if (ubPlayerID != NOBODY)
						{
							InitiateConversation( pMorris, MercPtrs[ ubPlayerID ], NPC_INITIAL_QUOTE, 0 );
							gMercProfiles[ pMorris->ubProfile ].ubMiscFlags2 |= PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE;
						}
					}
				}
			}
			else
			{
				//Get the # of qualified mercs
				ubNumQualifiedMercs = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( ubQualifiedSoldierIDArray, NULL );

				//if there is some qualified mercs
				if( ubNumQualifiedMercs != 0 )
				{
					//loop through all the mercs
					for( ubCnt=0; ubCnt<ubNumQualifiedMercs; ++ubCnt )
					{
						pSoldier = MercPtrs[ ubQualifiedSoldierIDArray[ ubCnt ] ];

						TacticalCharacterDialogue( pSoldier, QUOTE_JOINING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM );
						pSoldier->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_EXT_MORRIS;
					}
				}

				//Morris should say a new quote
//				CharacterDialogue( MORRIS, QUOTE_ATTACKED_BY_MULTIPLE_CREATURES, gTalkPanel.iFaceIndex, DIALOGUE_NPC_UI, FALSE, FALSE );

				TriggerNPCRecord( MORRIS_UB, 2 );

				gfMorrisShouldSayHi = FALSE;
			}
		}
		
		return;
	}
#else		//Ja25: no mike

	if ( iQSize == 0 )
	{
		if ( gfMikeShouldSayHi == TRUE )
		{
			SOLDIERTYPE * pMike;
			INT32	sPlayerGridNo;
			UINT8	ubPlayerID;

			pMike = FindSoldierByProfileID( MIKE, FALSE );
			if ( pMike )
			{
				sPlayerGridNo = ClosestPC( pMike, NULL );				
				if (!TileIsOutOfBounds(sPlayerGridNo))
				{
					ubPlayerID = WhoIsThere2( sPlayerGridNo, 0 );
					if (ubPlayerID != NOBODY)
					{
						InitiateConversation( pMike, MercPtrs[ ubPlayerID ], NPC_INITIAL_QUOTE, 0 );
						gMercProfiles[ pMike->ubProfile ].ubMiscFlags2 |= PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE;
						// JA2Gold: special hack value of 2 to prevent dialogue from coming up more than once
						gfMikeShouldSayHi = 2;
					}
				}
			}
		}

		return;
	}
#endif
	// ATE: Remove any civ quotes....
	// ShutDownQuoteBoxIfActive( TRUE );

	// If here, pick current one from queue and play

	// Get new one
	RemfromQueue( ghDialogueQ, &QItem );

	// If we are in auto bandage, ignore any quotes!
	if ( gTacticalStatus.fAutoBandageMode )
	{
		if( QItem->fPauseTime )
		{
			UnLockPauseState();
			UnPauseGame();
		}

		// Delete memory
		MemFree( QItem );
		return;
	}

	// Check time delay

	// Alrighty, check if this one is to be delayed until we gain control.
	// If so, place it back in!
	if ( QItem->fDelayed )
	{
		// Are we not in our turn and not interrupted
		if ( gTacticalStatus.ubCurrentTeam != gbPlayerNum )
		{
			//Place back in!
			// Add to queue
			ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

			return;
		}
	}

	// ATE: OK: If a battle sound, and delay value was given, set time stamp
	// now...
	if ( QItem->uiSpecialEventFlag == DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND )
	{
		if ( QItem->uiSpecialEventData2 != 0 )
		{
			if ( ( GetJA2Clock( ) - QItem->iTimeStamp ) < QItem->uiSpecialEventData2 )
			{
				//Place back in!
				// Add to queue
				ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

				return;
			}
		}
	}

	// Try to find soldier...
	pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, TRUE );

	if ( pSoldier != NULL )
	{
		if ( SoundIsPlaying( pSoldier->uiBattleSoundID ) )
		{
			//Place back in!
			// Add to queue
			ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

			return;
		}
	}

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) && ( QItem->uiSpecialEventFlag == 0 ) )
	{
		QItem->fPauseTime = TRUE;
	}

	if( QItem->fPauseTime )
	{
		if( GamePaused( ) == FALSE )
		{
			PauseGame();
			LockPauseState( 15 );
			fWasPausedDuringDialogue = TRUE;
		}
	}

	// Now play first item in queue
	// If it's not a 'special' dialogue event, continue
	if ( QItem->uiSpecialEventFlag == 0 )
	{
		if( pSoldier )
		{
			// wake grunt up to say
			if( pSoldier->flags.fMercAsleep )
			{
				pSoldier->flags.fMercAsleep = FALSE;

				// refresh map screen
				fCharacterInfoPanelDirty = TRUE;
				fTeamPanelDirty = TRUE;

				// allow them to go back to sleep
				TacticalCharacterDialogueWithSpecialEvent( pSoldier, QItem->usQuoteNum, DIALOGUE_SPECIAL_EVENT_SLEEP, 1,0 );
			}
		}

		gTacticalStatus.ubLastQuoteSaid = (UINT8)QItem->usQuoteNum;
		gTacticalStatus.ubLastQuoteProfileNUm = (UINT8)QItem->ubCharacterNum;
				
		// Flugente: only set up face if we can access correctly
		if ( QItem->iFaceIndex >= 0 )
		{
			// Setup face pointer
			gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
		}

		gubCurrentTalkingID	= QItem->ubCharacterNum;
#ifdef JA2UB		
		//Ja25: test
		if( QItem->ubCharacterNum == MORRIS_UB ) //MORRIS
		{
			if( QItem->usQuoteNum == 0 )
			{
			}
		}
#endif
		ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->fFromSoldier );

	}
	else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SKIP_A_FRAME )
	{

	}
	else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE )
	{
		// locking or unlocking?
		if( QItem->uiSpecialEventData )
		{
			switch( QItem->uiSpecialEventData2 )
			{
				case( MAP_SCREEN ):
					fLockOutMapScreenInterface = TRUE;
				break;
			}
		}
		else
		{
			switch( QItem->uiSpecialEventData2 )
			{
				case( MAP_SCREEN ):
					fLockOutMapScreenInterface = FALSE;
				break;
			}
		}
	}
	else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_REMOVE_EPC )
	{
		gMercProfiles[ (UINT8) QItem->uiSpecialEventData ].ubMiscFlags &= ~PROFILE_MISC_FLAG_FORCENPCQUOTE;
		UnRecruitEPC( (UINT8) QItem->uiSpecialEventData );
		ReBuildCharactersList();
	}
	else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CONTRACT_WANTS_TO_RENEW )
	{
		HandleMercIsWillingToRenew( (UINT8)QItem->uiSpecialEventData );
	}
	else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CONTRACT_NOGO_TO_RENEW )
	{
		HandleMercIsNotWillingToRenew( (UINT8)QItem->uiSpecialEventData );
	}
	else
	{
		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_USE_ALTERNATE_FILES )
		{
			gfUseAlternateDialogueFile = TRUE;
			
			// Flugente: only set up face if we can access correctly
			if ( QItem->iFaceIndex >= 0 )
			{
				// Setup face pointer
				gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			}

			gubCurrentTalkingID	= QItem->ubCharacterNum;

			ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->fFromSoldier );

			gfUseAlternateDialogueFile = FALSE;

		}
		// We could have a special flag, but dialogue as well
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_PCTRIGGERNPC )
		{
			// Flugente: only set up face if we can access correctly
			if ( QItem->iFaceIndex >= 0 )
			{
				// Setup face pointer
				gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			}

			gubCurrentTalkingID	= QItem->ubCharacterNum;

			ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->fFromSoldier );

			// Setup face with data!
			gpCurrentTalkingFace->uiFlags				|= FACE_PCTRIGGER_NPC;
			gpCurrentTalkingFace->uiUserData1		= QItem->uiSpecialEventData;
			gpCurrentTalkingFace->uiUserData2		= QItem->uiSpecialEventData2;

		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SHOW_CONTRACT_MENU )
		{
			// Setup face pointer
			// ATE: THis is working with MARK'S STUFF :(
			// Need this stuff so that bSelectedInfoChar is set...
			SetInfoChar( pSoldier->ubID );

			fShowContractMenu = TRUE;
			RebuildContractBoxForMerc( pSoldier );
			bSelectedContractChar = bSelectedInfoChar;
			pProcessingSoldier = pSoldier;
			fProcessingAMerc = TRUE;
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND )
		{
			// grab soldier ptr from profile ID
			pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, FALSE );

			// Do battle snounds......
			if ( pSoldier )
			{
				pSoldier->InternalDoMercBattleSound(	(UINT8)QItem->uiSpecialEventData, 0 );
			}
		}

		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SIGNAL_ITEM_LOCATOR_START )
		{
			// Turn off item lock for locators...
			gTacticalStatus.fLockItemLocators = FALSE;

			// Slide to location!
			SlideToLocation( 0,  QItem->uiSpecialEventData );

			// Flugente: only set up face if we can access correctly
			if ( QItem->iFaceIndex >= 0 )
			{
				// Setup face pointer
				gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			}

			gubCurrentTalkingID	= QItem->ubCharacterNum;

			ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->fFromSoldier );
		}

		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_ENABLE_AI )
		{
			//OK, allow AI to work now....
			UnPauseAI();
		}

		if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_TRIGGERPREBATTLEINTERFACE )
		{
			UnLockPauseState();

			// Flugente: what hideous idiocy is this? We cast a UINT32 as GROUP*? This is likely to return garbage.
			// Why the hell not use the id instead?
			InitPreBattleInterface( (GROUP*)QItem->uiSpecialEventData, TRUE );

			/*GROUP* pGroup = GetGroup( (UINT8)QItem->uiSpecialEventData );
			if ( pGroup )
				InitPreBattleInterface( pGroup, TRUE );*/
		}
		if( QItem->uiSpecialEventFlag & DIALOGUE_ADD_EVENT_FOR_SOLDIER_UPDATE_BOX )
		{
			INT32 iReason = 0;
			SOLDIERTYPE *pUpdateSoldier = NULL;

			iReason = QItem->uiSpecialEventData;

			switch( iReason )
			{
				case( UPDATE_BOX_REASON_ADDSOLDIER ):
					pUpdateSoldier = &Menptr[ QItem->uiSpecialEventData2 ];
					if( pUpdateSoldier->bActive == TRUE )
					{
						AddSoldierToUpdateBox( pUpdateSoldier );
					}
				break;
				case( UPDATE_BOX_REASON_SET_REASON ):
					SetSoldierUpdateBoxReason( QItem->uiSpecialEventData2 );
				break;
				case( UPDATE_BOX_REASON_SHOW_BOX ):
					ShowUpdateBox( );
				break;
			}
		}
		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_BEGINPREBATTLEINTERFACE )
		{
			// Flugente: only set up face if we can access correctly
			if ( QItem->iFaceIndex >= 0 )
			{
				// Setup face pointer
				gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			}

			gubCurrentTalkingID	= QItem->ubCharacterNum;

			ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->fFromSoldier );

			// Setup face with data!
			gpCurrentTalkingFace->uiFlags				|= FACE_TRIGGER_PREBATTLE_INT;
			gpCurrentTalkingFace->uiUserData1		= QItem->uiSpecialEventData;
			gpCurrentTalkingFace->uiUserData2		= QItem->uiSpecialEventData2;
		}

		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SHOPKEEPER )
		{
			if( QItem->uiSpecialEventData < 3 )
			{
				// post a notice if the player wants to withdraw money from thier account to cover the difference?
				swprintf( zMoney, L"%d", QItem->uiSpecialEventData2 );
				InsertCommasForDollarFigure( zMoney );
				InsertDollarSignInToString( zMoney );
			}
			else if ( QItem->uiSpecialEventData > 7 )
			{
				// post a notice if the player wants to withdraw money from thier account to cover the difference?
				swprintf( zMoney, L"%d", QItem->uiSpecialEventData2 );
				InsertCommasForDollarFigure( zMoney );
			}

			switch( QItem->uiSpecialEventData )
			{
				case( 0 ):
						swprintf( zText, SkiMessageBoxText[ SKI_SHORT_FUNDS_TEXT ], zMoney );

						//popup a message stating the player doesnt have enough money
						DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, MSG_BOX_FLAG_OK, ConfirmDontHaveEnoughForTheDealerMessageBoxCallBack );
				break;
				case( 1 ):
						//if the player is trading items
						swprintf( zText, SkiMessageBoxText[ SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_DIFFERENCE ], zMoney );

						//ask them if we should deduct money out the players account to cover the difference
						DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmToDeductMoneyFromPlayersAccountMessageBoxCallBack );

				break;
				case( 2 ):
						swprintf( zText, SkiMessageBoxText[ SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_COST ], zMoney );

						//ask them if we should deduct money out the players account to cover the difference
						DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmToDeductMoneyFromPlayersAccountMessageBoxCallBack );
				break;
				case( 3 ):
					// this means a dialogue event is in progress
					giShopKeepDialogueEventinProgress = QItem->uiSpecialEventData2;
				break;
				case( 4 ):
					// this means a dialogue event has ended
					giShopKeepDialogueEventinProgress = -1;
				break;
				case( 5 ):
					// this means a dialogue event has ended
					gfSKIScreenExit = TRUE;
				break;

				case( 6 ):
					if( guiCurrentScreen == SHOPKEEPER_SCREEN )
					{
						DisableButton( guiSKI_TransactionButton );
					}
				break;
				case( 7 ):
					if( guiCurrentScreen == SHOPKEEPER_SCREEN )
					{
						EnableButton( guiSKI_TransactionButton );
					}
				break;

				case 8:
					//if the player is trading items
					swprintf( zText, SkiMessageBoxText[SKI_QUESTION_TO_DEDUCT_INTEL_FROM_PLAYERS_ACCOUNT_TO_COVER_DIFFERENCE], zMoney );

					//ask them if we should deduct money out the players account to cover the difference
					DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmToDeductIntelFromPlayersAccountMessageBoxCallBack );

					break;

				case 9:
					swprintf( zText, SkiMessageBoxText[SKI_QUESTION_TO_DEDUCT_INTEL_FROM_PLAYERS_ACCOUNT_TO_COVER_COST], zMoney );

					//ask them if we should deduct money out the players account to cover the difference
					DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmToDeductIntelFromPlayersAccountMessageBoxCallBack );
					break;
			}

		}

		if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_EXIT_MAP_SCREEN )
		{
			// select sector
			ChangeSelectedMapSector( ( INT16 )QItem->uiSpecialEventData, ( INT16 )QItem->uiSpecialEventData2, ( INT8 )QItem->uiSpecialEventData3 );
			RequestTriggerExitFromMapscreen( MAP_EXIT_TO_TACTICAL );
		}
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_DISPLAY_STAT_CHANGE )
		{
			// grab soldier ptr from profile ID
			pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, FALSE );

			if ( pSoldier )
			{
				CHAR16 wTempString[ 128 ];

				// tell player about stat increase
				BuildStatChangeString( wTempString, pSoldier->name, ( BOOLEAN ) QItem->uiSpecialEventData, ( INT16 ) QItem->uiSpecialEventData2, ( UINT8 ) QItem->uiSpecialEventData3 );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, wTempString );
			}
		}
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_UNSET_ARRIVES_FLAG )
		{
			gTacticalStatus.bMercArrivingQuoteBeingUsed = FALSE;
		}

		/*
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_DISPLAY_INVASION_MESSAGE )
		{
			HandlePlayerNotifyInvasionByEnemyForces( (INT16)(QItem->uiSpecialEventData % MAP_WORLD_X), (INT16)(QItem->uiSpecialEventData / MAP_WORLD_X), 0, NULL );
		}
		*/
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT )
		{
			// Setup face pointer
			gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			gubCurrentTalkingID	= QItem->ubCharacterNum;

			// handle the monologue event
			HandleSkyRiderMonologueEvent( QItem->uiSpecialEventData, QItem->uiSpecialEventData2 );
		}

		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_MINESECTOREVENT )
		{
			// Flugente: only set up face if we can access correctly
			if ( QItem->iFaceIndex >= 0 )
			{
				// Setup face pointer
				gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
			}

			gubCurrentTalkingID	= QItem->ubCharacterNum;

			// set up the mine highlgith events
			SetUpAnimationOfMineSectors( QItem->uiSpecialEventData );
		}

		//Switch on our special events
		if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_GIVE_ITEM )
		{
			if ( QItem->bUIHandlerID == DIALOGUE_NPC_UI )
			{
				HandleNPCItemGiven( (UINT8)QItem->uiSpecialEventData, (OBJECTTYPE*)QItem->uiSpecialEventData2, (INT8)QItem->uiSpecialEventData3 );
			}
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_TRIGGER_NPC )
		{
			if ( QItem->bUIHandlerID == DIALOGUE_NPC_UI )
			{
				HandleNPCTriggerNPC( (UINT8)QItem->uiSpecialEventData, (UINT8)QItem->uiSpecialEventData2, (BOOLEAN)QItem->uiSpecialEventData3, (UINT8)QItem->uiSpecialEventData4 );
			}
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_GOTO_GRIDNO )
		{
			if ( QItem->bUIHandlerID == DIALOGUE_NPC_UI )
			{
				HandleNPCGotoGridNo( (UINT8)QItem->uiSpecialEventData, (UINT16)QItem->uiSpecialEventData2, (UINT8)QItem->uiSpecialEventData3 );//dnl!!!
			}
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_DO_ACTION )
		{
			if ( QItem->bUIHandlerID == DIALOGUE_NPC_UI )
			{
				HandleNPCDoAction( (UINT8)QItem->uiSpecialEventData, (UINT16)QItem->uiSpecialEventData2, (UINT8)QItem->uiSpecialEventData3 );
			}
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CLOSE_PANEL )
		{
			if ( QItem->bUIHandlerID == DIALOGUE_NPC_UI )
			{
				HandleNPCClosePanel( );
			}
		}
		else if ( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SHOW_UPDATE_MENU )
		{
			SetUpdateBoxFlag( TRUE );
		}
#ifdef JA2UB
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_JERRY_MILO )
		{
			//switch on the quote that is being spoken
			switch( QItem->uiSpecialEventData )
			{				
				case JM_SE__SHOW_RADIO_LOCATOR:
					HandleShowingRadioLocatorsInMorrisArea();
					break;

				case JM_SE__HEADING_TO_TRACONA:

					// play the heli crash video
					SetIntroType( INTRO_HELI_CRASH );

					//Go to the intro screen
					RequestTriggerExitFromMapscreen( MAP_EXIT_TO_INTRO_SCREEN );
					break;


				default:
					//nothing
					break;
			}
		}
#else //Ja25: removed the flag, no militia
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CONTINUE_TRAINING_MILITIA )
		{
			// grab soldier ptr from profile ID
			pSoldier = FindSoldierByProfileID( ( UINT8 )( QItem->uiSpecialEventData ), FALSE );

			// if soldier valid...
			if( pSoldier != NULL )
			{
				HandleInterfaceMessageForContinuingTrainingMilitia( pSoldier );
			}
		}
#endif
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_ENTER_MAPSCREEN )
		{
			if( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
				gfEnteringMapScreen = TRUE;
				fEnterMapDueToContract = TRUE;
			}
		}
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING )
		{
			// grab soldier ptr from profile ID
			pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, FALSE );

			// if soldier valid...
			if( pSoldier != NULL )
			{
				// .. remove the fired soldier again
				BeginStrategicRemoveMerc( pSoldier, (UINT8)QItem->uiSpecialEventData );
			}
		}
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING_NO_ASK_EQUIP )
		{
			// grab soldier ptr from profile ID
			pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, FALSE );

			// if soldier valid...
			if( pSoldier != NULL )
			{
				// .. remove the fired soldier again
				StrategicRemoveMerc( pSoldier );
			}
		}
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE )
		{
			// anv: handle snitch event
			if( QItem->uiSpecialEventData & MULTIPURPOSE_SPECIAL_EVENT_SNITCH_DIALOGUE )
			{
				gTacticalStatus.ubLastQuoteSaid = (UINT8)QItem->usQuoteNum;
				gTacticalStatus.ubLastQuoteProfileNUm = (UINT8)QItem->ubCharacterNum;
				
				// Flugente: only set up face if we can access correctly
				if ( QItem->iFaceIndex >= 0 )
				{
					// Setup face pointer
					gpCurrentTalkingFace = &gFacesData[ QItem->iFaceIndex ];
				}

				gubCurrentTalkingID	= QItem->iFaceIndex;

				//ExecuteCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, DIALOGUE_TACTICAL_UI, TRUE );
				ExecuteSnitchCharacterDialogue( QItem->ubCharacterNum, QItem->usQuoteNum, QItem->iFaceIndex, QItem->bUIHandlerID, QItem->uiSpecialEventData2, QItem->uiSpecialEventData3, QItem->uiSpecialEventData4 );
			}
			// Flugente: additional dialogue
			else if ( QItem->uiSpecialEventData & MULTIPURPOSE_SPECIAL_EVENT_ADDITIONAL_DIALOGUE )
			{
				// Flugente: only set up face if we can access correctly
				if ( QItem->iFaceIndex >= 0 )
				{
					// Setup face pointer
					gpCurrentTalkingFace = &gFacesData[QItem->iFaceIndex];
				}

				gubCurrentTalkingID = QItem->iFaceIndex;

				extern BOOLEAN ExecuteAdditionalCharacterDialogue( UINT8 ubProfile, INT32 iFaceIndex, UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3 );

				ExecuteAdditionalCharacterDialogue( QItem->ubCharacterNum, QItem->iFaceIndex, QItem->usQuoteNum, QItem->uiSpecialEventData2, QItem->uiSpecialEventData3, QItem->uiSpecialEventData4 );
			}

#ifdef JA2UB
			//JA25 UB
			if ( QItem->uiSpecialEventData & MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING )
			{
				HandleEveryoneDoneTheirEndGameQuotes();
			}
			else
			{
				// grab soldier ptr from profile ID
				pSoldier = FindSoldierByProfileID( (UINT8)QItem->uiSpecialEventData, FALSE );

				// FindSoldier was returning a lot of nullptrs which would crash the game very quickly after Jerry gets up. This check is here to circumvent that.
				if (pSoldier != nullptr)
				{
					// Now, wake these sluts up and have them say quote...
					pSoldier->fIgnoreGetupFromCollapseCheck = FALSE;

					//Get the soldier up
					pSoldier->bCollapsed = FALSE;
					pSoldier->ChangeSoldierStance( ANIM_STAND );

					//if the soldier is Jerry
					if (FindSoldierByProfileID( JERRY_MILO_UB, FALSE ) == pSoldier) //JERRY
					{
						//Play the sound of the Antena breaking
						PlayJA2SampleFromFile( "SOUNDS\\Metal Antenna Crunch.wav", RATE_11025, HIGHVOLUME, 1, MIDDLE );
					}

					//Turn off the flag saying we are doing the initial heli crash
					gfFirstTimeInGameHeliCrash = FALSE;

					//if all the mercs are done their talk
					if (AreAllTheMercsFinishedSayingThereInitialHeliCrashQuotes())
					{
						//Trigger Jerry Milo's script record 10 ( call action 301 )
						//AA 
						//if ( gGameUBOptions.InGameHeliCrash == TRUE )
						if (gGameUBOptions.JerryQuotes == TRUE)
							DelayedMercQuote( JERRY_MILO_UB, 0xffff, 4 ); //JERRY

						//End the ui Lock
						guiPendingOverrideEvent = LU_ENDUILOCK;
					}
				}
			}
		}
#else //Ja25 No queen
			if ( QItem->uiSpecialEventData & MULTIPURPOSE_SPECIAL_EVENT_DONE_KILLING_DEIDRANNA )
			{
				HandleDoneLastKilledQueenQuote( );
			}
			else if ( QItem->uiSpecialEventData & MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING )
			{
				HandleDoneLastEndGameQuote( );
			}
		}
#endif
		else if( QItem->uiSpecialEventFlag & DIALOGUE_SPECIAL_EVENT_SLEEP )
		{
			// no soldier, leave now
			if( pSoldier == NULL )
			{
				return;
			}

			// wake merc up or put them back down?
			if( QItem->uiSpecialEventData == 1 )
			{
				pSoldier->flags.fMercAsleep = TRUE;
			}
			else
			{
				pSoldier->flags.fMercAsleep = FALSE;
			}

			// refresh map screen
			fCharacterInfoPanelDirty = TRUE;
			fTeamPanelDirty = TRUE;
		}
	}

	// grab soldier ptr from profile ID
	pSoldier = FindSoldierByProfileID( QItem->ubCharacterNum, FALSE );

	if ( pSoldier && pSoldier->bTeam == gbPlayerNum )
	{
		CheckForStopTimeQuotes( QItem->usQuoteNum );
	}

	if( QItem->fPauseTime )
	{
		fWasPausedDuringDialogue = TRUE;
	}

	// Delete memory
	MemFree( QItem );
}

BOOLEAN GetDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, UINT32 iDataSize, STR16 zDialogueText, UINT32 *puiSoundID, CHAR8 *zSoundString );

BOOLEAN GetSnitchDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, UINT32 iDataSize, STR16 zDialogueText, UINT32 *puiSound1ID, UINT32 *puiSound2ID, UINT32 *puiSound3ID, CHAR8 zSoundFiles[][64], UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile );

BOOLEAN DelayedTacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum )
{
	if ( pSoldier->ubProfile == NO_PROFILE )
		return( FALSE );

	if (pSoldier->stats.bLife < CONSCIOUSNESS )
		return( FALSE );

	if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		return FALSE;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		return( FALSE );

	if ( (AM_A_ROBOT( pSoldier )) )
		return( FALSE );

	if (pSoldier->stats.bLife < OKLIFE && usQuoteNum != QUOTE_SERIOUSLY_WOUNDED )
		return( FALSE );

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
		return( FALSE );

	return( CharacterDialogue( pSoldier->ubProfile, usQuoteNum, pSoldier->iFaceIndex, DIALOGUE_TACTICAL_UI, TRUE, TRUE ) );
}


BOOLEAN TacticalCharacterDialogueWithSpecialEvent( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2 )
{
	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return( FALSE );
	}

	if ( uiFlag != DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND && uiData1 != BATTLE_SOUND_DIE1 )
	{
		if (pSoldier->stats.bLife < CONSCIOUSNESS )
			return( FALSE );

		if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
			return FALSE;

		if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
			return( FALSE );
	}

	return( CharacterDialogueWithSpecialEvent( pSoldier->ubProfile, usQuoteNum, pSoldier->iFaceIndex, DIALOGUE_TACTICAL_UI, TRUE, FALSE, uiFlag, uiData1, uiData2 ) );
}

BOOLEAN TacticalCharacterDialogueWithSpecialEventEx( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 )
{
	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return( FALSE );
	}

	if ( uiFlag != DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND && uiData1 != BATTLE_SOUND_DIE1 )
	{
		if (pSoldier->stats.bLife < CONSCIOUSNESS )
			return( FALSE );

		if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
			return FALSE;

		if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
			return( FALSE );

		if ( (AM_A_ROBOT( pSoldier )) )
		{
			return( FALSE );
		}

		if (pSoldier->stats.bLife < OKLIFE && usQuoteNum != QUOTE_SERIOUSLY_WOUNDED )
		return( FALSE );

		if( pSoldier->bAssignment == ASSIGNMENT_POW )
		{
			return( FALSE );
		}

	}

	return( CharacterDialogueWithSpecialEventEx( pSoldier->ubProfile, usQuoteNum, pSoldier->iFaceIndex, DIALOGUE_TACTICAL_UI, TRUE, FALSE, uiFlag, uiData1, uiData2, uiData3 ) );
}


BOOLEAN TacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum )
{
// Haydent
	if(is_client)
	{
		return(FALSE); //somewhere amongst all this it causes a puase of merc movement while making the quote which throws out the movement sync between  clients... : hayden.
	}
	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return( FALSE );
	}

#ifdef JA2UB
//Ja25 no meanwhiles
#else
	if ( AreInMeanwhile( ) )
	{
		return( FALSE );
	}
#endif
	if (pSoldier->stats.bLife < CONSCIOUSNESS )
		return( FALSE );

	if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		return FALSE;

	if (pSoldier->stats.bLife < OKLIFE && usQuoteNum != QUOTE_SERIOUSLY_WOUNDED )
		return( FALSE );

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		return( FALSE );

	if ( (AM_A_ROBOT( pSoldier )) )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return( FALSE );
	}

	// OK, let's check if this is the exact one we just played, if so, skip.
	if ( pSoldier->ubProfile == gTacticalStatus.ubLastQuoteProfileNUm &&
			usQuoteNum == gTacticalStatus.ubLastQuoteSaid )
	{
		return( FALSE );
	}


	// If we are a robot, play the controller's quote!
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		if ( pSoldier->CanRobotBeControlled( ) )
		{
			return( TacticalCharacterDialogue( MercPtrs[ pSoldier->ubRobotRemoteHolderID ], usQuoteNum ) );
		}
		else
		{
			return( FALSE );
		}
	}

	if ( AM_AN_EPC( pSoldier ) && !(gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG_FORCENPCQUOTE) )
		return( FALSE );

	// Check for logging of me too bleeds...
	if ( usQuoteNum == QUOTE_STARTING_TO_BLEED )
	{
		if ( gubLogForMeTooBleeds )
		{
			// If we are greater than one...
			if ( gubLogForMeTooBleeds > 1 )
			{
				//Replace with me too....
				usQuoteNum = QUOTE_ME_TOO;
			}

			++gubLogForMeTooBleeds;
		}
	}

	// Flugente: if we don't have a face index, then don't do a dialogue here...
	if ( pSoldier->iFaceIndex <= -1 )
		return( FALSE );

	return( CharacterDialogue( pSoldier->ubProfile, usQuoteNum, pSoldier->iFaceIndex, DIALOGUE_TACTICAL_UI, TRUE, FALSE ) );
}

BOOLEAN SnitchTacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT8 ubEventType, UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile )
{
	// Haydent
	if(is_client)
	{
		return(FALSE); //somewhere amongst all this it causes a puase of merc movement while making the quote which throws out the movement sync between  clients... : hayden.
	}
	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return( FALSE );
	}

#if (defined JA2UB) 
	//Ja25 no meanwhiles
#else
	if ( AreInMeanwhile( ) )
	{
		return( FALSE );
	}
#endif
	if (pSoldier->stats.bLife < CONSCIOUSNESS )
		return( FALSE );

	if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		return FALSE;

	if (pSoldier->stats.bLife < OKLIFE && usQuoteNum != QUOTE_SERIOUSLY_WOUNDED )
		return( FALSE );

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		return( FALSE );

	if ( (AM_A_ROBOT( pSoldier )) )
	{
		return( FALSE );
	}

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		return( FALSE );
	}

	// OK, let's check if this is the exact one we just played, if so, skip.
	//if ( pSoldier->ubProfile == gTacticalStatus.ubLastQuoteProfileNUm &&
	//	usQuoteNum == gTacticalStatus.ubLastQuoteSaid )
	//{
	//	return( FALSE );
	//}

	if ( AM_AN_EPC( pSoldier ) && !(gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG_FORCENPCQUOTE) )
		return( FALSE );

	return( SnitchCharacterDialogue( pSoldier->ubProfile, usQuoteNum, pSoldier->iFaceIndex, 
		DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE, MULTIPURPOSE_SPECIAL_EVENT_SNITCH_DIALOGUE,
		ubTargetProfile, ubTargetProfile, ubSecondaryTargetProfile,
		DIALOGUE_TACTICAL_UI, TRUE, FALSE ) );
}




// Flugente: additional dialogue
// We call this function from several places. It uses the dialogue functions, but calls a Lua script to know whether something, and what, should be said
// This allows us to call a lot of very specific dialogue for individual mercs without the need to define what exactly should be said in the code -
// a modder can simply control in Lua what should happen
BOOLEAN AdditionalTacticalCharacterDialogue_CallsLua( SOLDIERTYPE *pSoldier, UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3 )
{
	// Haydent
	if ( is_client )
		return( FALSE ); //somewhere amongst all this it causes a puase of merc movement while making the quote which throws out the movement sync between  clients... : hayden.

	if ( pSoldier->ubProfile == NO_PROFILE )
		return( FALSE );

#if (defined JA2UB) 
	//Ja25 no meanwhiles
#else
	if ( AreInMeanwhile() )
		return( FALSE );
#endif
	
	if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
		return FALSE;

	if ( pSoldier->stats.bLife < OKLIFE )
		return( FALSE );

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
		return( FALSE );

	if ( ( AM_A_ROBOT( pSoldier ) ) )
		return( FALSE );

	if ( pSoldier->bAssignment == ASSIGNMENT_POW )
		return( FALSE );
	
	if ( AM_AN_EPC( pSoldier ) && !( gMercProfiles[pSoldier->ubProfile].ubMiscFlags & PROFILE_MISC_FLAG_FORCENPCQUOTE ) )
		return( FALSE );

	// we actually use the snitch function here, and simply deviate later on due to marking this with MULTIPURPOSE_SPECIAL_EVENT_ADDITIONAL_DIALOGUE
	return( SnitchCharacterDialogue( pSoldier->ubProfile, usEventNr, pSoldier->iFaceIndex,
		DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE, MULTIPURPOSE_SPECIAL_EVENT_ADDITIONAL_DIALOGUE,
		aData1, aData2, aData3,
		DIALOGUE_TACTICAL_UI, TRUE, FALSE ) );
}

// call event for all mercs in a sector
void AdditionalTacticalCharacterDialogue_AllInCurrentSector( UINT8 ausIgnoreProfile,
	UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3, INT32 aAroundGridno, INT32 aRadius )
{
	return AdditionalTacticalCharacterDialogue_AllInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, ausIgnoreProfile,
		usEventNr, aData1, aData2, aData3, aAroundGridno, aRadius );
}

void AdditionalTacticalCharacterDialogue_AllInSector(INT16 aSectorX, INT16 aSectorY, INT8 aSectorZ, UINT8 ausIgnoreProfile, 
	UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3, INT32 aAroundGridno, INT32 aRadius )
{
	SOLDIERTYPE* pSoldier;
	int cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt, pSoldier++ )
	{
		if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->bActive &&
			pSoldier->ubProfile != ausIgnoreProfile &&
			pSoldier->sSectorX == aSectorX && pSoldier->sSectorY == aSectorY && pSoldier->bSectorZ == aSectorZ &&
			pSoldier->bAssignment != ASSIGNMENT_POW && pSoldier->bAssignment != IN_TRANSIT &&
			(aAroundGridno == NOWHERE || PythSpacesAway( pSoldier->sGridNo, aAroundGridno ) <= aRadius ) &&
			!pSoldier->flags.fBetweenSectors )
		{
			AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, usEventNr, aData1, aData2, aData3 );
		}
	}
}

void AdditionalTacticalCharacterDialogue_AllInSectorRadiusCall( UINT8 ausIgnoreProfile,
	UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3, INT32 aAroundGridno, INT32 aRadius )
{
	return AdditionalTacticalCharacterDialogue_AllInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, ausIgnoreProfile,
		usEventNr, aData1, aData2, aData3, aAroundGridno, aRadius );
}

// This function takes a profile num, quote num, faceindex and a UI hander ID.
// What it does is queues up the dialog to be ultimately loaded/displayed
//				FACEINDEX
//						The face index is an index into an ACTIVE face. The face is considered to
//						be active, and if it's not, either that has to be handled by the UI handler
//						ir nothing will show.	What this function does is set the face to talking,
//						and the face sprite system should handle the rest.
//				bUIHandlerID
//						Because this could be used in any place, the UI handleID is used to differentiate
//						places in the game. For example, specific things happen in the tactical engine
//						that may not be the place where in the AIM contract screen uses.....

// NB;				The queued system is not yet implemented, but will be transpatent to the caller....


BOOLEAN CharacterDialogueWithSpecialEvent( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2 )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->ubCharacterNum = ubCharacterNum;
	QItem->usQuoteNum			= usQuoteNum;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID		= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );
	QItem->fFromSoldier		= fFromSoldier;
	QItem->fDelayed				= fDelayed;

	// Set flag for special event
	QItem->uiSpecialEventFlag		= uiFlag;
	QItem->uiSpecialEventData		= uiData1;
	QItem->uiSpecialEventData2	= uiData2;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

	// sevenfm: disable FF for dialogues
	if (IsFastForwardMode())
	{
		SetFastForwardMode(FALSE);
	}

	if ( uiFlag & DIALOGUE_SPECIAL_EVENT_PCTRIGGERNPC )
	{
		// Increment refrence count...
		giNPCReferenceCount++;
	}

	return( TRUE );
}

BOOLEAN CharacterDialogueWithSpecialEventEx( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->ubCharacterNum = ubCharacterNum;
	QItem->usQuoteNum			= usQuoteNum;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID		= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );
	QItem->fFromSoldier		= fFromSoldier;
	QItem->fDelayed				= fDelayed;

	// Set flag for special event
	QItem->uiSpecialEventFlag		= uiFlag;
	QItem->uiSpecialEventData		= uiData1;
	QItem->uiSpecialEventData2	= uiData2;
	QItem->uiSpecialEventData3	= uiData3;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

	if ( uiFlag & DIALOGUE_SPECIAL_EVENT_PCTRIGGERNPC )
	{
		// Increment refrence count...
		giNPCReferenceCount++;
	}

	return( TRUE );
}

BOOLEAN CharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->ubCharacterNum = ubCharacterNum;
	QItem->usQuoteNum			= usQuoteNum;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID		= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );
	QItem->fFromSoldier		= fFromSoldier;
	QItem->fDelayed				= fDelayed;

	// check if pause already locked, if so, then don't mess with it
	if( gfLockPauseState == FALSE )
	{
		QItem->fPauseTime	 = fPausedTimeDuringQuote;
	}

	fPausedTimeDuringQuote = FALSE;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

	// Flugente: additional dialogue
	// in the dialogue screen, only the merc doing the talking may answer
	if ( bUIHandlerID == DIALOGUE_NPC_UI )
	{
		if ( gusSelectedSoldier != NOBODY )
		{
			AdditionalTacticalCharacterDialogue_CallsLua( MercPtrs[gusSelectedSoldier], ADE_DIALOGUE_REACTION, ubCharacterNum, usQuoteNum, ( gMercProfiles[ubCharacterNum].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) ? 1 : 0 );
		}
	}
	// if team members talk, anyone may answer
	else
	{
		// other team members might react to this line
		AdditionalTacticalCharacterDialogue_AllInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, ubCharacterNum, ADE_DIALOGUE_REACTION, ubCharacterNum, usQuoteNum, ( gMercProfiles[ubCharacterNum].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) ? 1 : 0 );
	}

	// sevenfm: disable FF for dialogues
	if (IsFastForwardMode())
	{
		SetFastForwardMode(FALSE);
	}

	return( TRUE );
}

BOOLEAN SnitchCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex,
	UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, 
	UINT32 uiSpecialEventData3, UINT32 uiSpecialEventData4, 
	UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->ubCharacterNum		= ubCharacterNum;
	QItem->usQuoteNum			= usQuoteNum;
	QItem->uiSpecialEventFlag	= uiSpecialEventFlag;
	QItem->uiSpecialEventData	= uiSpecialEventData1;
	QItem->uiSpecialEventData2	= uiSpecialEventData2;
	QItem->uiSpecialEventData3	= uiSpecialEventData3;
	QItem->uiSpecialEventData4	= uiSpecialEventData4;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID			= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );
	QItem->fFromSoldier			= fFromSoldier;
	QItem->fDelayed				= fDelayed;

	// check if pause already locked, if so, then don't mess with it
	if( gfLockPauseState == FALSE )
	{
		QItem->fPauseTime	 = fPausedTimeDuringQuote;
	}

	fPausedTimeDuringQuote = FALSE;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );

	return( TRUE );
}


BOOLEAN SpecialCharacterDialogueEvent( UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, UINT32 uiSpecialEventData3, INT32 iFaceIndex, UINT8 bUIHandlerID )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->uiSpecialEventFlag		= uiSpecialEventFlag;
	QItem->uiSpecialEventData		= uiSpecialEventData1;
	QItem->uiSpecialEventData2	= uiSpecialEventData2;
	QItem->uiSpecialEventData3	= uiSpecialEventData3;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID		= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );

	// if paused state not already locked
	if( gfLockPauseState == FALSE )
	{
		QItem->fPauseTime	 = fPausedTimeDuringQuote;
	}

	fPausedTimeDuringQuote = FALSE;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );


	return( TRUE );
}

BOOLEAN SpecialCharacterDialogueEventWithExtraParam( UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, UINT32 uiSpecialEventData3, UINT32 uiSpecialEventData4, INT32 iFaceIndex, UINT8 bUIHandlerID )
{
	DIALOGUE_Q_STRUCT				*QItem;

	// Allocate new item
	QItem = (DIALOGUE_Q_STRUCT *) MemAlloc( sizeof( DIALOGUE_Q_STRUCT ) );
	memset( QItem, 0, sizeof( DIALOGUE_Q_STRUCT ) );

	QItem->uiSpecialEventFlag		= uiSpecialEventFlag;
	QItem->uiSpecialEventData		= uiSpecialEventData1;
	QItem->uiSpecialEventData2	= uiSpecialEventData2;
	QItem->uiSpecialEventData3	= uiSpecialEventData3;
	QItem->uiSpecialEventData4	= uiSpecialEventData4;
	QItem->iFaceIndex			= iFaceIndex;
	QItem->bUIHandlerID		= bUIHandlerID;
	QItem->iTimeStamp			= GetJA2Clock( );

	// if paused state not already locked
	if( gfLockPauseState == FALSE )
	{
		QItem->fPauseTime	 = fPausedTimeDuringQuote;
	}

	fPausedTimeDuringQuote = FALSE;

	// Add to queue
	ghDialogueQ = AddtoQueue( ghDialogueQ, &QItem );


	return( TRUE );
}

BOOLEAN ExecuteCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier )
{
	UINT32	uiSoundID;
	SOLDIERTYPE *pSoldier;

	// Check if we are dead now or not....( if from a soldier... )

	// Try to find soldier...
	pSoldier = FindSoldierByProfileID( ubCharacterNum, TRUE );

	if ( pSoldier != NULL )
	{
		// Check vital stats
		if (pSoldier->stats.bLife < CONSCIOUSNESS )
		{
			return( FALSE );
		}

		if ( pSoldier->usSkillCooldown[SOLDIER_COOLDOWN_CRYO] )
			return FALSE;

		if ( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED )
			return( FALSE );

		if ( (AM_A_ROBOT( pSoldier )) )
		{
			return( FALSE );
		}

		if (pSoldier->stats.bLife < OKLIFE && usQuoteNum != QUOTE_SERIOUSLY_WOUNDED )
		{
			return( FALSE );
		}

		if( pSoldier->bAssignment == ASSIGNMENT_POW )
		{
			return( FALSE );
		}

		// sleeping guys don't talk.. go to standby to talk
		if( pSoldier->flags.fMercAsleep == TRUE )
		{
			// check if the soldier was compaining about lack of sleep and was alseep, if so, leave them alone
			if( ( usQuoteNum == QUOTE_NEED_SLEEP ) || ( usQuoteNum == QUOTE_OUT_OF_BREATH ) )
			{
				// leave them alone
				return ( TRUE );
			}

			// may want to wake up any character that has VERY important dialogue to say
			// MC to flesh out

		}

		// now being used in a different way...
		/*
		if ( ( (usQuoteNum == QUOTE_PERSONALITY_TRAIT &&
					(gMercProfiles[ubCharacterNum].bDisability == FORGETFUL ||
					gMercProfiles[ubCharacterNum].bDisability == CLAUSTROPHOBIC ||
					gMercProfiles[ubCharacterNum].bDisability == NERVOUS ||
					gMercProfiles[ubCharacterNum].bDisability == NONSWIMMER ||
					gMercProfiles[ubCharacterNum].bDisability == FEAR_OF_INSECTS))
					//usQuoteNum == QUOTE_STARTING_TO_WHINE ||
#ifdef JA2BETAVERSION
					|| usQuoteNum == QUOTE_WHINE_EQUIPMENT) && (guiCurrentScreen != QUEST_DEBUG_SCREEN) )
#else
			) )
#endif

		{
			// This quote might spawn another quote from someone
			iLoop = 0;
			for ( pTeamSoldier = MercPtrs[ iLoop ]; iLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; iLoop++,pTeamSoldier++ )
			{
				if ( (pTeamSoldier->ubProfile != ubCharacterNum) && (OK_INSECTOR_MERC( pTeamSoldier )) && (SpacesAway( pSoldier->sGridNo, pTeamSoldier->sGridNo ) < 5) )
				{
					// if this merc disliked the whining character sufficiently and hasn't already retorted
					if ( gMercProfiles[ pTeamSoldier->ubProfile ].bMercOpinion[ ubCharacterNum ] < -2 && !( pTeamSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_ANNOYING_MERC ) )
					{
						// make a comment!
						TacticalCharacterDialogue( pTeamSoldier, QUOTE_ANNOYING_PC );
						pTeamSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_ANNOYING_MERC;
						break;
					}
				}
			}
		}
		*/
	}
	else
	{
		// If from a soldier, and he does not exist anymore, donot play!
		if ( fFromSoldier )
		{
			return( FALSE );
		}
	}

	// anv: have to do it now for single sounds
	subsequentsounds.ubMaxSndCounter = 0;

	// Check face index
	CHECKF( iFaceIndex != -1 );
	
	if ( !GetDialogue( ubCharacterNum, usQuoteNum, DIALOGUESIZE, gzQuoteStr, &uiSoundID, gzSoundString ) )
	{
		return( FALSE );
	}

	// Flugente: hijack the quote for possible replacement by additional dialogue
	// only do so if recruited (npc dialogue is already unique)
	if ( gMercProfiles[ubCharacterNum].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
		LuaHandleReplaceQuote( ubCharacterNum, usQuoteNum );

	// sevenfm: stop high speed timer for any talking face
	if (IsFastForwardMode())
	{
		SetFastForwardMode(FALSE);
	}

	// start "talking" system (portrait animation and start wav sample)
	SetFaceTalking( iFaceIndex, gzSoundString, gzQuoteStr, RATE_11025, 30, 1, MIDDLEPAN );

	// pSoldier can be null here... ( if NOT from an alive soldier )
	CreateTalkingUI( bUIHandlerID, iFaceIndex, ubCharacterNum, pSoldier, gzQuoteStr );

	// Set global handler ID value, used when face desides it's done...
	gbUIHandlerID = bUIHandlerID;

	guiScreenIDUsedWhenUICreated = guiCurrentScreen;

	return( TRUE );
}

BOOLEAN ExecuteSnitchCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, UINT8 ubSnitchEventType, UINT8 ubSnitchTargetID, UINT8 ubSecondarySnitchTargetID )
{
	UINT32 uiSound1ID;
	UINT32 uiSound2ID;
	UINT32 uiSound3ID;
	//if( bUIHandlerID == DIALOGUE_SNITCH_UI )
	{
		CHAR8 zSoundFiles[10][64];

		SOLDIERTYPE *pSoldier;
		pSoldier = FindSoldierByProfileID( ubCharacterNum, TRUE );
		if ( !GetSnitchDialogue( ubCharacterNum,
			usQuoteNum, DIALOGUESIZE, gzQuoteStr, &uiSound1ID, &uiSound2ID, &uiSound3ID, zSoundFiles, ubSnitchTargetID, ubSecondarySnitchTargetID ) )
		{
			return( FALSE );
		}
		//SetFaceTalkingMultipleSounds( iFaceIndex, zSoundString1, zSoundString2, zSoundString3, gzQuoteStr, RATE_11025, 30, 1, MIDDLEPAN );
		//SetFaceTalking( iFaceIndex, zSoundString1, gzQuoteStr, RATE_11025, 30, 1, MIDDLEPAN );
		SetFaceTalkingMultipleSounds( iFaceIndex, zSoundFiles, 3, gzQuoteStr, RATE_11025, 30, 1, MIDDLEPAN );
		CreateTalkingUI( DIALOGUE_TACTICAL_UI, iFaceIndex, ubCharacterNum, pSoldier, gzQuoteStr );

		// Set global handleer ID value, used when face desides it's done...
		gbUIHandlerID = DIALOGUE_TACTICAL_UI;

		guiScreenIDUsedWhenUICreated = guiCurrentScreen;
	}

	return( TRUE );
}

// Flugente: additional dialogue
extern void LuaHandleAdditionalDialogue( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubProfile, INT32 iFaceIndex, UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3 );

BOOLEAN ExecuteAdditionalCharacterDialogue( UINT8 ubProfile, INT32 iFaceIndex, UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3 )
{
	SOLDIERTYPE *pSoldier = FindSoldierByProfileID( ubProfile, TRUE );

	if ( !pSoldier )
		return FALSE;

	// call Lua script on whether we can play something here, and get text and sound file
	LuaHandleAdditionalDialogue( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, ubProfile, iFaceIndex, usEventNr, aData1, aData2, aData3 );
	
	return( TRUE );
}

void SetQuoteStr( STR16 aStr )
{
	//Copy the original string over to the temp string
	wcscpy( gzQuoteStr, aStr );
}

void SetSoundString( const char* aStr )
{
	//Copy the original string over to the temp string
	strcpy( gzSoundString, aStr );
}

BOOLEAN LuaCallsToDoDialogueStuff( UINT8 ubProfile, INT32 iFaceIndex, const char* azSoundString )
{
	SOLDIERTYPE *pSoldier = FindSoldierByProfileID( ubProfile, TRUE );

	if ( !pSoldier )
		return FALSE;

	// in order to stop modders from crashing the game by calling this function a million times, we can only say one thing at a time
	if ( IsFaceTalking( iFaceIndex ) )
		return FALSE;

	SetFaceTalking( iFaceIndex, (char*)azSoundString, gzQuoteStr, RATE_11025, 30, 1, MIDDLEPAN );
	CreateTalkingUI( DIALOGUE_TACTICAL_UI, iFaceIndex, ubProfile, pSoldier, gzQuoteStr );

	// Set global handleer ID value, used when face desides it's done...
	gbUIHandlerID = DIALOGUE_TACTICAL_UI;

	guiScreenIDUsedWhenUICreated = guiCurrentScreen;

	return TRUE;
}

// Flugente: play sound file like normal dialogue or, if someone else is already talking, just play the sound
void SpecialDialogue( SOLDIERTYPE* pSoldier, STR8 azSoundString, STR16 azTextString )
{
	// if possible, set up a proper dialogue. Otherwise just play the file
	if ( !DialogueActive() && pSoldier->ubProfile != NO_PROFILE )
	{
		gpCurrentTalkingFace = &gFacesData[pSoldier->iFaceIndex];
		gubCurrentTalkingID = pSoldier->ubID;

		SetQuoteStr( azTextString );

		LuaCallsToDoDialogueStuff( pSoldier->ubProfile, pSoldier->iFaceIndex, azSoundString );
	}
	else
	{
		PlayJA2SampleFromFile( azSoundString, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
	}
}

void CreateTalkingUI( INT8 bUIHandlerID, INT32 iFaceIndex, UINT8 ubCharacterNum, SOLDIERTYPE *pSoldier, STR16 zQuoteStr )
{
	// Show text, if on
	if ( gGameSettings.fOptions[ TOPTION_SUBTITLES ] || !gFacesData[ iFaceIndex ].fValidSpeech )
	{
		switch( bUIHandlerID )
		{
			case DIALOGUE_TACTICAL_UI:

				HandleTacticalTextUI( iFaceIndex, pSoldier, zQuoteStr );
				break;

			case DIALOGUE_NPC_UI:

				HandleTacticalNPCTextUI( ubCharacterNum, zQuoteStr );
				break;

			case DIALOGUE_CONTACTPAGE_UI:
				DisplayTextForMercFaceVideoPopUp( zQuoteStr );
				break;

			case DIALOGUE_SPECK_CONTACT_PAGE_UI:
				DisplayTextForSpeckVideoPopUp( zQuoteStr );
				break;
			case DIALOGUE_EXTERNAL_NPC_UI:

				DisplayTextForExternalNPC( ubCharacterNum, zQuoteStr );
				break;

			case DIALOGUE_SHOPKEEPER_UI:
				InitShopKeeperSubTitledText( zQuoteStr );
				break;
		}
	}

	if ( gGameSettings.fOptions[ TOPTION_SPEECH ] )
	{
		switch( bUIHandlerID )
		{
			case DIALOGUE_TACTICAL_UI:

				HandleTacticalSpeechUI( ubCharacterNum, iFaceIndex );
				break;

			case DIALOGUE_CONTACTPAGE_UI:
				break;

			case DIALOGUE_SPECK_CONTACT_PAGE_UI:
				break;
			case DIALOGUE_EXTERNAL_NPC_UI:
				HandleExternNPCSpeechFace( iFaceIndex );
				break;
		}
	}
}


CHAR8 *GetDialogueDataFilename( UINT8 ubCharacterNum, UINT16 usQuoteNum, BOOLEAN fWavFile )
{
	static CHAR8 zFileName[164];
	static CHAR8 zFileNameHelper[164];
	UINT32		ubFileNumID;
	BOOLEAN isBartenderSantos = FALSE;

	// Flugente: For the voice set itself, use this number
	UINT32 usVoiceSet = gMercProfiles[ubCharacterNum].usVoiceIndex;

	// Are we an NPC OR an RPC that has not been recruited?
	// ATE: Did the || clause here to allow ANY RPC that talks while the talking menu is up to use an npc quote file
	if ( gfUseAlternateDialogueFile )
	{
		if ( fWavFile )
		{
			sprintf( zFileNameHelper, "NPC_SPEECH\\d_%03d_%03d", usVoiceSet, usQuoteNum );
		}
		else
		{
			// assume EDT files are in EDT directory on HARD DRIVE
			sprintf( zFileName, "NPCDATA\\d_%03d.EDT", usVoiceSet );
		}
	}
	else if ( ( gMercProfiles[ubCharacterNum].Type == PROFILETYPE_RPC || gMercProfiles[ubCharacterNum].Type == PROFILETYPE_NPC ) &&
			( !( gMercProfiles[ ubCharacterNum ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
			|| ProfileCurrentlyTalkingInDialoguePanel( ubCharacterNum )
			|| (gMercProfiles[ ubCharacterNum ].ubMiscFlags & PROFILE_MISC_FLAG_FORCENPCQUOTE) )
			)
	{
		ubFileNumID = usVoiceSet;
		
		if ( ubCharacterNum == HERVE )
		{
			isBartenderSantos = TRUE;
			ubFileNumID = HERVE;	
		}
		else if ( ubCharacterNum == PETER || ubCharacterNum == ALBERTO || ubCharacterNum == CARLO )
		{
			isBartenderSantos = TRUE;
			sprintf( (char *)zFileName,"NPCDATA\\%03d.EDT", ubCharacterNum );
			if ( !FileExists( zFileName ) )
				ubFileNumID = HERVE;
			else
				ubFileNumID = ubCharacterNum;
		}

#ifdef JA2UB
		if ( ubCharacterNum == MANUEL_UB ||
			ubCharacterNum == BIGGENS_UB ||
			ubCharacterNum == JOHN_K_UB ||
			ubCharacterNum == TEX_UB ||
			ubCharacterNum == GASTON_UB ||
			ubCharacterNum == STOGIE_UB ||
			ubCharacterNum == JERRY_MILO_UB ||
			ubCharacterNum == PGMALE4_UB ||
			ubCharacterNum == BETTY_UB ||
			ubCharacterNum == RAUL_UB ||
			ubCharacterNum == MORRIS_UB ||
			ubCharacterNum == RUDY_UB )
		{
			sprintf( (char *)zFileName,"NPCDATA\\%03d.EDT", ubCharacterNum );
			ubFileNumID = ubCharacterNum;
		}
#endif

		// If we are character #155, check fact!
		if ( ubCharacterNum == MANNY && !gubFact[ FACT_MANNY_IS_BARTENDER ] )
		{
			ubFileNumID = MANNY;
		}

		if ( fWavFile )
		{
			// Lesh: patch to allow playback ogg speech files
			sprintf( zFileNameHelper, "NPC_SPEECH\\%03d_%03d", ubFileNumID, usQuoteNum );

			// WANNE: We do not have any speech files for the other (!= HERVE) santos bartenders, take HERVE speech files
			if ( isBartenderSantos && HERVE != ubFileNumID && !SoundFileExists( zFileNameHelper, zFileName ) )
			{
				ubFileNumID = HERVE;
				sprintf( zFileNameHelper, "NPC_SPEECH\\%03d_%03d", ubFileNumID, usQuoteNum );
			}
		}
		else
		{
			// assume EDT files are in EDT directory on HARD DRIVE
			sprintf( zFileName,"NPCDATA\\%03d.EDT", ubFileNumID );
			
			// WANNE: We do not have any edt files for the other (!= HERVE) santos bartenders, take HERVE speech files
			if (isBartenderSantos && HERVE != ubFileNumID && !FileExists(zFileName))
			{
				sprintf( zFileName,"NPCDATA\\%03d.EDT", HERVE );
			}
		}
	}
	else
	{
		if ( fWavFile )
		{
#ifdef RUSSIAN
				if ( ( gMercProfiles[ubCharacterNum].Type == PROFILETYPE_RPC ||
					gMercProfiles[ubCharacterNum].Type == PROFILETYPE_NPC ||
					gMercProfiles[ubCharacterNum].Type == PROFILETYPE_VEHICLE ) && gMercProfiles[ ubCharacterNum ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )	
				{
					//inshy: fix for UB-1.13 version only					sprintf( zFileName,"SPEECH\\r_%03d_%03d.ogg",ubCharacterNum,usQuoteNum );
					
#ifdef JA2UB
					//inshy: fix for UB-1.13 version only
					sprintf( zFileNameHelper,"SPEECH\\%03d_%03d",usVoiceSet,usQuoteNum );
#else
					sprintf( zFileNameHelper,"SPEECH\\r_%03d_%03d",usVoiceSet,usQuoteNum );

					//<SB> Also check for Russian Gold sound files (identical to international ones)
					if ( !SoundFileExists( zFileNameHelper, zFileName ) )
						sprintf( zFileNameHelper, "SPEECH\\%03d_%03d", usVoiceSet, usQuoteNum );
#endif
				}
				else
#endif
			{
				// build name of wav file (characternum + quotenum)
				sprintf( zFileNameHelper, "SPEECH\\%03d_%03d", usVoiceSet, usQuoteNum );
			}
		}
		else
		{
			// assume EDT files are in EDT directory on HARD DRIVE
			sprintf( zFileName, "MERCEDT\\%03d.EDT", usVoiceSet );
		}
	}

	if ( fWavFile )
		SoundFileExists( zFileNameHelper, zFileName );

	return( zFileName );
}

CHAR8 *GetSnitchDialogueDataFilename( UINT8 ubCharacterNum, UINT16 usQuoteNum, BOOLEAN fWavFile, BOOLEAN fName )
{
	static CHAR8 zFileName[164];
	static CHAR8 zFilename_Used[164];

	// Flugente: For the voice set itself, use this number
	UINT32 usVoiceSet = gMercProfiles[ubCharacterNum].usVoiceIndex;

	if ( fWavFile )
	{
		// build name of wav file (characternum + quotenum)
		if ( fName )
		{
			sprintf( zFileName, "SPEECH\\SNITCH\\NAMES\\%03d_%03d", usVoiceSet, usQuoteNum );
		}
		else
		{
			sprintf( zFileName, "SPEECH\\SNITCH\\%03d_%03d", usVoiceSet, usQuoteNum );
		}

		SoundFileExists( zFileName, zFilename_Used );
	}
	else
	{
		// assume EDT files are in EDT directory on HARD DRIVE
		if(fName)
			sprintf( zFilename_Used, "MERCEDT\\SNITCH\\NAMES\\%03d.EDT", usVoiceSet );
		else
			sprintf( zFilename_Used, "MERCEDT\\SNITCH\\%03d.EDT", usVoiceSet );
	}

	return( zFilename_Used );
}

// Used to see if the dialog text file exists
BOOLEAN DialogueDataFileExistsForProfile( UINT8 ubCharacterNum, UINT16 usQuoteNum, BOOLEAN fWavFile, STR8 *ppStr )
{
	STR8 pFilename;

	pFilename = GetDialogueDataFilename( ubCharacterNum, usQuoteNum, fWavFile );

	if ( ppStr )
	{
		(*ppStr ) = pFilename;
	}

	return( FileExists( pFilename ) );
}

BOOLEAN SnitchDialogueDataFileExistsForProfile( UINT8 ubCharacterNum, UINT16 usQuoteNum, BOOLEAN fWavFile, STR8 *ppStr, BOOLEAN fName )
{
	STR8 pFilename;

	pFilename = GetSnitchDialogueDataFilename( ubCharacterNum, usQuoteNum, fWavFile, fName );

	if ( ppStr )
	{
		(*ppStr ) = pFilename;
	}

	return( FileExists( pFilename ) );
}

BOOLEAN GetDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, UINT32 iDataSize, STR16 zDialogueText, UINT32 *puiSoundID, CHAR8 *zSoundString )
{
	STR8 pFilename;
	BOOLEAN fTextAvailable = FALSE;

	// first things first	- grab the text (if player has SUBTITLE PREFERENCE ON)
	//if ( gGameSettings.fOptions[ TOPTION_SUBTITLES ] )
	{
		if ( DialogueDataFileExistsForProfile( ubCharacterNum, 0, FALSE, &pFilename ) )
		{
			LoadEncryptedDataFromFile( pFilename, zDialogueText, usQuoteNum * iDataSize, iDataSize );
			if(zDialogueText[0] == 0)
			{
				swprintf( zDialogueText, L"I have no text in the EDT file ( %d ) %S", usQuoteNum, pFilename );

#ifndef JA2BETAVERSION
				return( FALSE );
#endif
			}
			else
				fTextAvailable = TRUE;
		}
		else
		{
			swprintf( zDialogueText, L"I have no text in the file ( %d ) %S", usQuoteNum , pFilename );

#ifndef JA2BETAVERSION
			return( FALSE );
#endif
		}
	}


	// CHECK IF THE FILE EXISTS, IF NOT, USE DEFAULT!
	pFilename = GetDialogueDataFilename( ubCharacterNum, usQuoteNum, TRUE );

	// Copy
	strcpy( zSoundString, pFilename );

	// Double check it exists....

//#ifndef JA2TESTVERSION

/*
	if ( !FileExists( pFilename ) )
	{
		CHAR8 sString[512];

		sprintf( sString, "ERROR: Missing file for character # %d, quote # %d", ubCharacterNum, usQuoteNum );
		ShowCursor(TRUE);
		ShowCursor(TRUE);
		ShutdownWithErrorBox( sString );
	}
*/

//#endif

	*puiSoundID = NO_SAMPLE;

	// get speech if applicable
	if ( gGameSettings.fOptions[ TOPTION_SPEECH ] )
	{
		// Load it into memory!
		*puiSoundID = SoundLoadSample(	pFilename );
	}

	return (*puiSoundID != NO_SAMPLE) || fTextAvailable;
}

BOOLEAN ReplaceTextWithOtherText( CHAR16 *pFinishedString, CHAR16 compare[32], CHAR16 replace[32] )
{
	CHAR16		pTempString[320];
	INT32		iLength = 0;
	INT32		iCurLocInSourceString = 0;
	INT32		iLengthOfSourceString = wcslen( pFinishedString );		//Get the length of the source string
	CHAR16		*pNickString = NULL;

	CHAR16		*pSubString = NULL;
	BOOLEAN		fReplacingMercName = TRUE;

	CHAR16	sSearchString[32];

	//Copy the original string over to the temp string
	wcscpy( pTempString, pFinishedString );

	//Null out the string
	pFinishedString[0] = L'\0';

	//Keep looping through to replace all references to the keyword
	while ( iCurLocInSourceString < iLengthOfSourceString )
	{
		iLength = 0;
		pSubString = NULL;

		//Find out if the $MERCNICK$ is in the string
		pNickString = wcsstr( &pTempString[iCurLocInSourceString], compare );

		if ( pNickString != NULL )
		{
			fReplacingMercName = TRUE;
			pSubString = pNickString;
			wcscpy( sSearchString, compare );
		}
		else
		{
			pSubString = NULL;
		}

		// if there is a substring
		if ( pSubString != NULL )
		{
			iLength = pSubString - &pTempString[iCurLocInSourceString];

			//Copy the part of the source string up to the keyword
			wcsncat( pFinishedString, &pTempString[iCurLocInSourceString], iLength );

			//increment the source string counter by how far in the keyword is and by the length of the keyword
			iCurLocInSourceString += iLength + wcslen( sSearchString );

			if ( fReplacingMercName )
			{
				//add the mercs name to the string
				wcscat( pFinishedString, replace );
			}
		}
		else
		{
			//add the rest of the string
			wcscat( pFinishedString, &pTempString[iCurLocInSourceString] );

			iCurLocInSourceString += wcslen( &pTempString[iCurLocInSourceString] );
		}
	}

	return(TRUE);
}

// anv: special version of GetDialogue, message is put together from parts
BOOLEAN GetSnitchDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, UINT32 iDataSize, STR16 zDialogueText, UINT32 *puiSound1ID, UINT32 *puiSound2ID, UINT32 *puiSound3ID, CHAR8 zSoundFiles[][64], UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile )
{
	STR8 pFilename1;
	//STR8 pFilename2;
	//STR8 pFilename3;
	//CHAR16	zDialogueTextTemp[ QUOTE_MESSAGE_SIZE ];
	BOOLEAN fTextAvailable = FALSE;

	// first things first	- grab the text (if player has SUBTITLE PREFERENCE ON)
	//if ( gGameSettings.fOptions[ TOPTION_SUBTITLES ] )
	{

		if ( SnitchDialogueDataFileExistsForProfile( ubCharacterNum, 0, FALSE, &pFilename1, FALSE ) )
			 //SnitchDialogueDataFileExistsForProfile( ubCharacterNum, 0, FALSE, &pFilename, TRUE ) && 
			 //SnitchDialogueDataFileExistsForProfile( ubCharacterNum, 0, FALSE, &pFilename, TRUE ) && )
		{

			LoadEncryptedDataFromFile( pFilename1, zDialogueText, usQuoteNum * iDataSize, iDataSize );

			// Flugente: replaced SnitchDialogueReplaceMercNicksWithProperData with ReplaceTextWithOtherText - less complicated an can be used for other things than merc names
			ReplaceTextWithOtherText( zDialogueText, L"$MERCNICK$", gMercProfiles[ubTargetProfile].zNickname );
			ReplaceTextWithOtherText( zDialogueText, L"$MERCNICK2$", gMercProfiles[ubSecondaryTargetProfile].zNickname );

			if(zDialogueText[0] == 0)
			{
				swprintf( zDialogueText, L"I have no text in the EDT file ( %d ) %S", usQuoteNum, pFilename1 );

#ifndef JA2BETAVERSION
				return( FALSE );
#endif
			}
			else
				fTextAvailable = TRUE;
		}
		else
		{
			swprintf( zDialogueText, L"I have no text in the file ( %d ) %S", usQuoteNum , pFilename1 );

#ifndef JA2BETAVERSION
			return( FALSE );
#endif
		}
	}


	// CHECK IF THE FILE EXISTS, IF NOT, USE DEFAULT!
	//pFilename1 = GetSnitchDialogueDataFilename( ubCharacterNum, usQuoteNum, TRUE, FALSE );
	//pFilename2 = GetSnitchDialogueDataFilename( ubCharacterNum, ubSnitchTargetID, TRUE, TRUE );
	//pFilename3 = GetSnitchDialogueDataFilename( ubCharacterNum, ubSecondarySnitchTargetID, TRUE, TRUE );

	// Copy
	strcpy( zSoundFiles[0], GetSnitchDialogueDataFilename( ubCharacterNum, ubTargetProfile, TRUE, TRUE ) );
	strcpy( zSoundFiles[1], GetSnitchDialogueDataFilename( ubCharacterNum, usQuoteNum, TRUE, FALSE ) );
	strcpy( zSoundFiles[2], GetSnitchDialogueDataFilename( ubCharacterNum, ubSecondaryTargetProfile, TRUE, TRUE ) );

	*puiSound1ID = NO_SAMPLE;
	*puiSound2ID = NO_SAMPLE;
	*puiSound3ID = NO_SAMPLE;

	// get speech if applicable
	if ( gGameSettings.fOptions[ TOPTION_SPEECH ] )
	{
		// Load it into memory!
		*puiSound1ID = SoundLoadSample(	zSoundFiles[0] );
		*puiSound2ID = SoundLoadSample(	zSoundFiles[0] );
		*puiSound3ID = SoundLoadSample(	zSoundFiles[0] );
	}

	return (*puiSound2ID != NO_SAMPLE) || fTextAvailable;
}

// Handlers for tactical UI stuff
void HandleTacticalNPCTextUI( UINT8 ubCharacterNum, STR16 zQuoteStr )
{
	CHAR16									zText[ QUOTE_MESSAGE_SIZE ];

	// Setup dialogue text box
	if ( guiCurrentScreen != MAP_SCREEN )
	{
		gTalkPanel.fRenderSubTitlesNow = TRUE;
		gTalkPanel.fSetupSubTitles = TRUE;
	}

	// post message to mapscreen message system
	swprintf( gTalkPanel.zQuoteStr, L"\"%s\"", zQuoteStr );
	swprintf( zText, L"%s: \"%s\"", gMercProfiles[ ubCharacterNum ].zNickname, zQuoteStr );
	MapScreenMessage( FONT_MCOLOR_WHITE, MSG_DIALOG, L"%s",	zText );
}


// Handlers for tactical UI stuff
void DisplayTextForExternalNPC(	UINT8 ubCharacterNum, STR16 zQuoteStr )
{
	CHAR16									zText[ QUOTE_MESSAGE_SIZE ];
	INT16									sLeft;

	// Setup dialogue text box
	if ( guiCurrentScreen != MAP_SCREEN )
	{
		gTalkPanel.fRenderSubTitlesNow = TRUE;
		gTalkPanel.fSetupSubTitles = TRUE;
	}

	// post message to mapscreen message system
	swprintf( gTalkPanel.zQuoteStr, L"\"%s\"", zQuoteStr );
	swprintf( zText, L"%s: \"%s\"", gMercProfiles[ ubCharacterNum ].zNickname, zQuoteStr );
	MapScreenMessage( FONT_MCOLOR_WHITE, MSG_DIALOG, L"%s",	zText );

	if ( guiCurrentScreen == MAP_SCREEN )
	{
		sLeft			= ( gsExternPanelXPosition + 97 + xResOffset );
		gsTopPosition = gsExternPanelYPosition + yResOffset;
	}
	else
	{
		sLeft			= ( 110 + xResOffset );
	}

	ExecuteTacticalTextBox( sLeft, gTalkPanel.zQuoteStr );

	return;

}


void HandleTacticalTextUI( INT32 iFaceIndex, SOLDIERTYPE *pSoldier, STR16 zQuoteStr )
{
	CHAR16									zText[ QUOTE_MESSAGE_SIZE ];
	INT16									sLeft = 0;

	//BUild text
	// How do we do this with defines?
	//swprintf( zText, L"\xb4\xa2 %s: \xb5 \"%s\"", gMercProfiles[ ubCharacterNum ].zNickname, zQuoteStr );
	swprintf( zText, L"\"%s\"", zQuoteStr );

	// TODO.RW: Wenn wir in tactical sind, dann normal. In strategy den offset dazurechnen!
	if ( guiCurrentScreen == GAME_SCREEN )
	{
		sLeft = 110;
	}
	else
	{
		sLeft = 110 + xResOffset;
		gsTopPosition += yResOffset;
	}	

	//previous version
	//sLeft = 110;

	ExecuteTacticalTextBox( sLeft, zText );
}


void ExecuteTacticalTextBoxForLastQuote( INT16 sLeftPosition, STR16 pString )
{
	UINT32 uiDelay = FindDelayForString( pString );

	fDialogueBoxDueToLastMessage = TRUE;

	guiDialogueLastQuoteTime = GetJA2Clock();

	guiDialogueLastQuoteDelay = ( ( uiDelay < FINAL_TALKING_DURATION ) ?	FINAL_TALKING_DURATION : uiDelay );

	// now execute box
	ExecuteTacticalTextBox(sLeftPosition, pString );
}

void ExecuteTacticalTextBox( INT16 sLeftPosition, STR16 pString )
{
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;

	// check if mouse region created, if so, do not recreate

	if( fTextBoxMouseRegionCreated == TRUE )
	{
		return;
	}

	memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	//never use it anymore
	//SET_USE_WINFONTS( TRUE );
	//SET_WINFONT( giSubTitleWinFont );
	// Prepare text box
	iDialogueBox = PrepareMercPopupBox( iDialogueBox , BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, pString, DIALOGUE_DEFAULT_SUBTITLE_WIDTH, 0, 0, 0, &gusSubtitleBoxWidth, &gusSubtitleBoxHeight );
	//SET_USE_WINFONTS( FALSE );


	VideoOverlayDesc.sLeft			= sLeftPosition;
	VideoOverlayDesc.sTop				= gsTopPosition;
	VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusSubtitleBoxWidth;
	VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusSubtitleBoxHeight;
	VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
	VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
	VideoOverlayDesc.BltCallback = RenderSubtitleBoxOverlay;

	giTextBoxOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );

	gsTopPosition = 20;

	//Define main region
	MSYS_DefineRegion( &gTextBoxMouseRegion,	VideoOverlayDesc.sLeft, VideoOverlayDesc.sTop,	VideoOverlayDesc.sRight, VideoOverlayDesc.sBottom, MSYS_PRIORITY_HIGHEST,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, TextOverlayClickCallback );
	// Add region
	MSYS_AddRegion(&(gTextBoxMouseRegion) );

	fTextBoxMouseRegionCreated = TRUE;
}


void HandleExternNPCSpeechFace( INT32 iIndex )
{
	INT32 iFaceIndex;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	INT32									iFaceOverlay;


	// grab face index
	iFaceIndex = iIndex;

	// Enable it!
	SetAutoFaceActive( FACE_AUTO_DISPLAY_BUFFER, FACE_AUTO_RESTORE_BUFFER, iFaceIndex , 0, 0 );

	// Set flag to say WE control when to set inactive!
	gFacesData[ iFaceIndex ].uiFlags |= FACE_INACTIVE_HANDLED_ELSEWHERE;

	// WDS - bug fix: VideoOverlayDesc must be initialized! - 07/16/2007
	memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	if ( guiCurrentScreen != MAP_SCREEN )
	{
		// Setup video overlay!
		VideoOverlayDesc.sLeft			= 10 + xResOffset;
		VideoOverlayDesc.sTop				= 20;
		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + 99;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + 98;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderFaceOverlay;
	}
	else
	{
		// Setup video overlay!

		VideoOverlayDesc.sLeft			= gsExternPanelXPosition + xResOffset;
		VideoOverlayDesc.sTop				= gsExternPanelYPosition + yResOffset;

		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + 99;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + 98;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderFaceOverlay;
	}

	iFaceOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );
	gpCurrentTalkingFace->iVideoOverlay = iFaceOverlay;

	RenderAutoFace( iFaceIndex );

	// ATE: Create mouse region.......
	if ( !fExternFaceBoxRegionCreated )
	{
		fExternFaceBoxRegionCreated = TRUE;

		//Define main region
		MSYS_DefineRegion( &gFacePopupMouseRegion,	VideoOverlayDesc.sLeft, VideoOverlayDesc.sTop,	VideoOverlayDesc.sRight, VideoOverlayDesc.sBottom, MSYS_PRIORITY_HIGHEST,
							CURSOR_NORMAL, MSYS_NO_CALLBACK, FaceOverlayClickCallback );
		// Add region
		MSYS_AddRegion(&(gFacePopupMouseRegion) );

	}

	gfFacePanelActive = TRUE;

	return;
}


void HandleTacticalSpeechUI( UINT8 ubCharacterNum, INT32 iFaceIndex	)
{
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	INT32									iFaceOverlay;
	SOLDIERTYPE						*pSoldier;
	BOOLEAN								fDoExternPanel = FALSE;

	memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	// Get soldier pointer, if there is one...
	// Try to find soldier...
	pSoldier = FindSoldierByProfileID( ubCharacterNum, FALSE );

	// PLEASE NOTE:	pSoldier may legally be NULL (e.g. Skyrider) !!!

	if ( pSoldier == NULL )
	{
		fDoExternPanel = TRUE;
	}
	else
	{
		// If we are not an active face!
		if ( guiCurrentScreen != MAP_SCREEN )
		{
			fDoExternPanel = TRUE;
		}
	}

	if ( fDoExternPanel )
	{
		// Enable it!
		SetAutoFaceActive( FACE_AUTO_DISPLAY_BUFFER, FACE_AUTO_RESTORE_BUFFER, iFaceIndex , 0, 0 );

		// Set flag to say WE control when to set inactive!
		gFacesData[ iFaceIndex ].uiFlags |= ( FACE_INACTIVE_HANDLED_ELSEWHERE | FACE_MAKEACTIVE_ONCE_DONE );

		// IF we are in tactical and this soldier is on the current squad
		if ( ( guiCurrentScreen == GAME_SCREEN ) && ( pSoldier != NULL ) && ( pSoldier->bAssignment == iCurrentTacticalSquad ) )
		{
			// Make the interface panel dirty..
			// This will dirty the panel next frame...
			gfRerenderInterfaceFromHelpText = TRUE;
		}

		// Setup video overlay!
		VideoOverlayDesc.sLeft = 10;	
		VideoOverlayDesc.sTop = 20;
		VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + 99;
		VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + 98;
		VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
		VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
		VideoOverlayDesc.BltCallback = RenderFaceOverlay;

		iFaceOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );
		gpCurrentTalkingFace->iVideoOverlay = iFaceOverlay;

		RenderAutoFace( iFaceIndex );

		// ATE: Create mouse region.......
		if ( !fExternFaceBoxRegionCreated )
		{
			fExternFaceBoxRegionCreated = TRUE;

			//Define main region
			MSYS_DefineRegion( &gFacePopupMouseRegion,	VideoOverlayDesc.sLeft, VideoOverlayDesc.sTop,	VideoOverlayDesc.sRight, VideoOverlayDesc.sBottom, MSYS_PRIORITY_HIGHEST,
								CURSOR_NORMAL, MSYS_NO_CALLBACK, FaceOverlayClickCallback );
			// Add region
			MSYS_AddRegion(&(gFacePopupMouseRegion) );
		}

		gfFacePanelActive = TRUE;

	}
	else if ( guiCurrentScreen == MAP_SCREEN )
	{
		// Are we in mapscreen?
		// If so, set current guy active to talk.....
		if ( pSoldier != NULL )
		{
			ContinueDialogue( pSoldier, FALSE );
		}
	}

}


void HandleDialogueEnd( FACETYPE *pFace )
{
	if ( gGameSettings.fOptions[ TOPTION_SPEECH ] )
	{

		if ( pFace != gpCurrentTalkingFace )
		{
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"HandleDialogueEnd() face mismatch." );
			return;
		}

		if ( pFace->fTalking )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"HandleDialogueEnd() face still talking." );
			return;
		}

		switch( gbUIHandlerID )
		{
			case DIALOGUE_TACTICAL_UI:

			if ( gfFacePanelActive )
			{
				// Set face inactive!
				pFace->fCanHandleInactiveNow = TRUE;
				SetAutoFaceInActive( pFace->iID );
				gfFacePanelActive = FALSE;

				if ( fExternFaceBoxRegionCreated )
				{
					fExternFaceBoxRegionCreated = FALSE;
					MSYS_RemoveRegion(&(gFacePopupMouseRegion) );
				}
			}
			break;
			case DIALOGUE_NPC_UI:
			break;
			case DIALOGUE_EXTERNAL_NPC_UI:
				pFace->fCanHandleInactiveNow = TRUE;
				SetAutoFaceInActive( pFace->iID );
				gfFacePanelActive = FALSE;

				if ( fExternFaceBoxRegionCreated )
				{
					fExternFaceBoxRegionCreated = FALSE;
					MSYS_RemoveRegion(&(gFacePopupMouseRegion) );
				}

			break;
		}
	}


	if ( gGameSettings.fOptions[ TOPTION_SUBTITLES ] || !pFace->fValidSpeech )
	{
		switch( gbUIHandlerID )
		{
			case DIALOGUE_TACTICAL_UI:
			case DIALOGUE_EXTERNAL_NPC_UI:
				// Remove if created
				// 0verhaul:	Both of these events are more or less independent.	Most often the gTextBoxOverlay fails to init.
				// Therefore I am removing the dependency on giTextBoxOverlay before it decides to destroy the popup box region.
				// This should prevent blockage of buttons and also clicks on the tactical map.
				if ( giTextBoxOverlay != -1 )
				{
					RemoveVideoOverlay( giTextBoxOverlay );
					giTextBoxOverlay = -1;
				}

				if ( fTextBoxMouseRegionCreated )
				{
					RemoveMercPopupBoxFromIndex( iDialogueBox );

					// reset box id
					iDialogueBox = -1;
					MSYS_RemoveRegion( &gTextBoxMouseRegion );
					fTextBoxMouseRegionCreated = FALSE;
				}
				// anv: show repair prompt afer Waldo offered repairs
				if ( gbUIHandlerID == DIALOGUE_EXTERNAL_NPC_UI && gTacticalStatus.ubLastQuoteProfileNUm == WALDO && 
					( gTacticalStatus.ubLastQuoteSaid == WALDO_REPAIR_PROPOSITION || gTacticalStatus.ubLastQuoteSaid == WALDO_SERIOUS_REPAIR_PROPOSITION ) )
				{
					CHAR16	sHelicopterRepairPromptText[ 320 ];
					if( CheckFact(FACT_HELI_DAMAGED_CAN_START_REPAIR, 0) == TRUE )
					{	
						swprintf( sHelicopterRepairPromptText, pHelicopterRepairRefuelStrings[ STR_HELI_RR_REPAIR_PROMPT ], gMercProfiles[ WALDO ].zNickname, CalculateHelicopterRepairCost( FALSE ), gHelicopterSettings.ubHelicopterBasicRepairTime );
					}
					else
					{
						swprintf( sHelicopterRepairPromptText, pHelicopterRepairRefuelStrings[ STR_HELI_RR_REPAIR_PROMPT ], gMercProfiles[ WALDO ].zNickname, CalculateHelicopterRepairCost( TRUE ),  gHelicopterSettings.ubHelicopterSeriousRepairTime );
					}
					DoMessageBox( MSG_BOX_BASIC_STYLE, sHelicopterRepairPromptText, guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, OfferHelicopterRepairBoxCallBack, NULL );
				}

				// anv: after merc finishes his quote, we want enemy to answer
				if ( gbUIHandlerID == DIALOGUE_TACTICAL_UI )
				{
					SOLDIERTYPE *pSoldier = FindSoldierByProfileID( gTacticalStatus.ubLastQuoteProfileNUm, FALSE );
					if ( pSoldier )
					{
						UINT8 ubSeenEnemies[ MAX_NUM_SOLDIERS ];
						UINT8 ubSeenEnemiesCnt = 0;
						switch( gTacticalStatus.ubLastQuoteSaid )
						{					
							case QUOTE_CLOSE_CALL:					
							case QUOTE_UNDER_HEAVY_FIRE:
							case QUOTE_TAKEN_A_BREATING:
								if( pSoldier->ubPreviousAttackerID != NOBODY && !( MercPtrs[pSoldier->ubPreviousAttackerID]->bDeafenedCounter > 0 ) )
									PossiblyStartEnemyTaunt( MercPtrs[pSoldier->ubPreviousAttackerID], TAUNT_RIPOSTE, gTacticalStatus.ubLastQuoteProfileNUm );
								break;
							default:
								// select random enemy, who we see, who sees us and isn't deaf
								for(UINT8 cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID ; cnt++ )
								{
									if( MercPtrs[cnt] != NULL && MercPtrs[cnt]->aiData.bOppList[pSoldier->ubID] == SEEN_CURRENTLY 
										&& MercPtrs[pSoldier->ubID]->aiData.bOppList[cnt] == SEEN_CURRENTLY && !( MercPtrs[cnt]->bDeafenedCounter > 0 ) )
									{
										ubSeenEnemies[ubSeenEnemiesCnt] = cnt; 
										ubSeenEnemiesCnt++;
									}
								}
								if( ubSeenEnemiesCnt > 0 )
									PossiblyStartEnemyTaunt( MercPtrs[ubSeenEnemies[ Random(ubSeenEnemiesCnt) ]], TAUNT_RIPOSTE, gTacticalStatus.ubLastQuoteProfileNUm );
								}
								break;
						}
					}
				break;

			case DIALOGUE_NPC_UI:

				// anv: "meet" Waldo when speaking with him first time
				if ( gTacticalStatus.ubLastQuoteProfileNUm == WALDO && CheckFact( FACT_WALDO_MET, 0 ) == FALSE && gTacticalStatus.ubLastQuoteSaid == 0 )
				{
					AddHistoryToPlayersLog( HISTORY_WALDO, 0, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );
					SetFactTrue( FACT_WALDO_MET );
				}

				// Remove region
				if ( gTalkPanel.fTextRegionOn )
				{
					MSYS_RemoveRegion(&(gTalkPanel.TextRegion) );
					gTalkPanel.fTextRegionOn = FALSE;

				}

				// WANNE: This check is used, to prevent scrolling in small maps (e.g: Rebel Basement) in higher resolution (1024x768) [2007-05-14]
				gfDialogControl = TRUE;

				SetRenderFlags( RENDER_FLAG_FULL );

				gTalkPanel.fRenderSubTitlesNow = FALSE;

				// Delete subtitle box
				gTalkPanel.fDirtyLevel = DIRTYLEVEL2;
				RemoveMercPopupBoxFromIndex( iInterfaceDialogueBox );
				iInterfaceDialogueBox = -1;
				break;

			case DIALOGUE_CONTACTPAGE_UI:
				break;

			case DIALOGUE_SPECK_CONTACT_PAGE_UI:
				break;



		}
	}

	TurnOffSectorLocator();

	gsExternPanelXPosition	 = DEFAULT_EXTERN_PANEL_X_POS;
	gsExternPanelYPosition	 = DEFAULT_EXTERN_PANEL_Y_POS;

}



void RenderFaceOverlay( VIDEO_OVERLAY *pBlitter )
{
	UINT32 uiDestPitchBYTES, uiSrcPitchBYTES;
	UINT8	*pDestBuf, *pSrcBuf;
	INT16 sFontX, sFontY;
	SOLDIERTYPE *pSoldier;
	CHAR16					zTownIDString[50];


	if ( gpCurrentTalkingFace == NULL )
	{
		return;
	}

	if ( gfFacePanelActive )
	{
		pSoldier = FindSoldierByProfileID( gpCurrentTalkingFace->ubCharacterNum, FALSE );


		// a living soldier?..or external NPC?..choose panel based on this
		if( pSoldier )
		{
			BltVideoObjectFromIndex( pBlitter->uiDestBuff, guiCOMPANEL, 0, pBlitter->sX, pBlitter->sY, VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			BltVideoObjectFromIndex( pBlitter->uiDestBuff, guiCOMPANELB, 0, pBlitter->sX, pBlitter->sY, VO_BLT_SRCTRANSPARENCY, NULL );
		}

		// Display name, location ( if not current )
		SetFont( BLOCKFONT2 );
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_LTGRAY );

		if ( pSoldier )
		{
		//reset the font dest buffer
		SetFontDestBuffer( pBlitter->uiDestBuff, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT, FALSE);

			VarFindFontCenterCoordinates( (INT16)( pBlitter->sX + 12 ), (INT16)( pBlitter->sY + 55 ), 73, 9, BLOCKFONT2, &sFontX, &sFontY, L"%s", pSoldier->name );
			mprintf( sFontX, sFontY, L"%s", pSoldier->name );

			// What sector are we in, ( and is it the same as ours? )
			if ( pSoldier->sSectorX != gWorldSectorX || pSoldier->sSectorY != gWorldSectorY || pSoldier->bSectorZ != gbWorldSectorZ || pSoldier->flags.fBetweenSectors )
			{
				GetSectorIDString( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, zTownIDString, FALSE );

				ReduceStringLength( zTownIDString, 64 , BLOCKFONT2 );

				VarFindFontCenterCoordinates( (INT16)( pBlitter->sX + 12 ), (INT16)( pBlitter->sY + 68 ), 73, 9, BLOCKFONT2, &sFontX, &sFontY, L"%s", zTownIDString );
				mprintf( sFontX, sFontY, L"%s", zTownIDString );
			}

		//reset the font dest buffer
		SetFontDestBuffer(FRAME_BUFFER, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT, FALSE);

		// Display bars
		DrawLifeUIBarEx( pSoldier, (INT16)( pBlitter->sX + 69 ), (INT16)( pBlitter->sY + 47 ), 3, 42, FALSE, pBlitter->uiDestBuff );
		DrawBreathUIBarEx( pSoldier, (INT16)( pBlitter->sX + 75 ), (INT16)( pBlitter->sY + 47 ), 3, 42, FALSE, pBlitter->uiDestBuff );
		DrawMoraleUIBarEx( pSoldier, (INT16)( pBlitter->sX + 81 ), (INT16)( pBlitter->sY + 47 ), 3, 42, FALSE, pBlitter->uiDestBuff );

		}
		else
		{
			VarFindFontCenterCoordinates( (INT16)( pBlitter->sX + 9 ), (INT16)( pBlitter->sY + 55 ), 73, 9, BLOCKFONT2, &sFontX, &sFontY, L"%s", gMercProfiles[ gpCurrentTalkingFace->ubCharacterNum ].zNickname );
			mprintf( sFontX, sFontY, L"%s", gMercProfiles[ gpCurrentTalkingFace->ubCharacterNum ].zNickname );
		}

		//RenderAutoFace( gpCurrentTalkingFace->iID );
		//BlinkAutoFace( gpCurrentTalkingFace->iID );
		//MouthAutoFace( gpCurrentTalkingFace->iID );


		pDestBuf = LockVideoSurface( pBlitter->uiDestBuff, &uiDestPitchBYTES);
		pSrcBuf = LockVideoSurface( gpCurrentTalkingFace->uiAutoDisplayBuffer, &uiSrcPitchBYTES);

		Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
					(UINT16 *)pSrcBuf, uiSrcPitchBYTES,
					(INT16)( pBlitter->sX + 14 ), (INT16)( pBlitter->sY + 6 ),
					0 , 0,
					gpCurrentTalkingFace->usFaceWidth, gpCurrentTalkingFace->usFaceHeight );

		UnLockVideoSurface( pBlitter->uiDestBuff );
		UnLockVideoSurface( gpCurrentTalkingFace->uiAutoDisplayBuffer );

		InvalidateRegion( pBlitter->sX, pBlitter->sY, pBlitter->sX + 99, pBlitter->sY + 98 );
	}
}


void RenderSubtitleBoxOverlay( VIDEO_OVERLAY *pBlitter )
{
	if ( giTextBoxOverlay != -1 )
	{
		RenderMercPopUpBoxFromIndex( iDialogueBox, pBlitter->sX, pBlitter->sY,	pBlitter->uiDestBuff );

		InvalidateRegion( pBlitter->sX, pBlitter->sY, pBlitter->sX + gusSubtitleBoxWidth, pBlitter->sY + gusSubtitleBoxHeight );
	}
}


void SayQuoteFromAnyBodyInSector( UINT16 usQuoteNum )
{
    // WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS, 0);
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		if ( OK_INSECTOR_MERC( pTeamSoldier ) && !AM_AN_EPC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !(AM_A_ROBOT( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep )
		{
			if ( gTacticalStatus.bNumFoughtInBattle[ ENEMY_TEAM ] == 0 )
			{
			
#ifdef JA2UB
//Ja25 No Ira, Miguel etc.
#else
				// quotes referring to Deidranna's men so we skip quote if there were no army guys fought
				if ( (usQuoteNum == QUOTE_SECTOR_SAFE) && (pTeamSoldier->ubProfile == IRA || pTeamSoldier->ubProfile == MIGUEL || pTeamSoldier->ubProfile == SHANK ) )
				{

					continue;
				}
				if ( (usQuoteNum == QUOTE_ENEMY_PRESENCE ) && (pTeamSoldier->ubProfile == IRA || pTeamSoldier->ubProfile == DIMITRI || pTeamSoldier->ubProfile == DYNAMO || pTeamSoldier->ubProfile == SHANK ) )
				{
					continue;
				}
#endif
			}

			ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
			ubNumMercs++;
		}
	}

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		// If we are air raid, AND red exists somewhere...
		if ( usQuoteNum == QUOTE_AIR_RAID )
		{
			for ( cnt = 0; cnt < ubNumMercs; cnt++ )
			{
				if ( ubMercsInSector[ cnt ] == 11 )
				{
					ubChosenMerc = (UINT8)cnt;
					break;
				}
			}
		}


		TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], usQuoteNum );
	}

}


void SayQuoteFromAnyBodyInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT16 usQuoteNum )
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT16>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
	UINT16	ubNumMercs = 0;
	UINT16	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive )
		{
			// Add guy if he's a candidate...
			if( pTeamSoldier->sSectorX == sSectorX && pTeamSoldier->sSectorY == sSectorY && pTeamSoldier->bSectorZ == bSectorZ	&& !AM_AN_EPC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !(AM_A_ROBOT( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep )
			{
				ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
				++ubNumMercs;
			}
		}
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("SayQuoteFromAnyBodyInThisSector: num mercs = %d",ubNumMercs));

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT16)Random( ubNumMercs );
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("SayQuoteFromAnyBodyInThisSector: chosen merc = %d",ubChosenMerc));

		//// If we are air raid, AND red exists somewhere...
		//if ( usQuoteNum == QUOTE_AIR_RAID )
		//{
		//	for ( cnt = 0; cnt < ubNumMercs; cnt++ )
		//	{
		//		if ( ubMercsInSector[ cnt ] == 11 )
		//		{
		//			ubChosenMerc = (UINT8)cnt;
		//			break;
		//		}
		//	}
		//}

		TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], usQuoteNum );
	}
}

void SayQuoteFromNearbyMercInSector( INT32 sGridNo, INT8 bDistance, UINT16 usQuoteNum )
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0 );
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		if ( OK_INSECTOR_MERC( pTeamSoldier ) && PythSpacesAway( sGridNo, pTeamSoldier->sGridNo ) < bDistance && !AM_AN_EPC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !(AM_A_ROBOT( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep &&
			SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo, 0, 0, TRUE ) )
		{
			if ( usQuoteNum == 66 && (INT16) Random( 100 ) > EffectiveWisdom( pTeamSoldier ) )
			{
				continue;
			}
			ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
			ubNumMercs++;
		}
	}

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		if (usQuoteNum == 66)
		{
			SetFactTrue( FACT_PLAYER_FOUND_ITEMS_MISSING );
		}
		TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], usQuoteNum );

	}

}

void SayQuote58FromNearbyMercInSector( INT32 sGridNo, INT8 bDistance, UINT16 usQuoteNum, INT8 bSex )
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8> ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++cnt,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		if ( OK_INSECTOR_MERC( pTeamSoldier ) && PythSpacesAway( sGridNo, pTeamSoldier->sGridNo ) < bDistance && !AM_AN_EPC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !(AM_A_ROBOT( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep &&
			SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo, 0, 0, TRUE ) )
		{
			// ATE: This is to check gedner for this quote...
			if ( QuoteExp[ pTeamSoldier->ubProfile ].QuoteExpGenderCode == 0 && bSex == FEMALE )
			{
				continue;
			}

			if ( QuoteExp[ pTeamSoldier->ubProfile ].QuoteExpGenderCode == 1 && bSex == MALE )
			{
				continue;
			}

			ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
			++ubNumMercs;
		}
	}

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );
		TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], usQuoteNum );
	}
}


void TextOverlayClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	static BOOLEAN fLButtonDown = FALSE;

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLButtonDown = TRUE;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLButtonDown )
	{
		if(	gpCurrentTalkingFace != NULL )
		{
			InternalShutupaYoFace( gpCurrentTalkingFace->iID, FALSE );

			// Did we succeed in shutting them up?
			if ( !gpCurrentTalkingFace->fTalking )
			{
				// shut down last quote box
				ShutDownLastQuoteTacticalTextBox( );
			}
		}
#ifdef JA2UB		
		//JA25 UB
		//if we are in the heli crash sequence
		else if( gJa25SaveStruct.fJerryBreakingLaptopOccuring )
		{
			InitJerriesSpeechCallBack();
		}

		//else the Commander morris note is being displayed
		else if( gJa25SaveStruct.ubDisplayCommanderMorrisNote != DMN__NOT_TO_DISPLAY_IT ||
						 gJa25SaveStruct.ubDisplayCommanderMorrisNote != DMN__FINISHED )
		{
			HandlePlayerClosingMorrisNoteDisplayedOnScreen();
		}
#endif
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}


void FaceOverlayClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	static BOOLEAN fLButtonDown = FALSE;

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLButtonDown = TRUE;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLButtonDown )
	{
		if(	gpCurrentTalkingFace != NULL )
		{
			InternalShutupaYoFace( gpCurrentTalkingFace->iID, FALSE );
		}

	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}

void ShutDownLastQuoteTacticalTextBox( void )
{
	if( fDialogueBoxDueToLastMessage )
	{
		RemoveVideoOverlay( giTextBoxOverlay );
		giTextBoxOverlay = -1;

		if ( fTextBoxMouseRegionCreated )
		{
			MSYS_RemoveRegion( &gTextBoxMouseRegion );
			fTextBoxMouseRegionCreated = FALSE;
		}

		fDialogueBoxDueToLastMessage = FALSE;
	}
}

UINT32 FindDelayForString( STR16 sString )
{
	return( wcslen( sString ) * TEXT_DELAY_MODIFIER );
}

void BeginLoggingForBleedMeToos( BOOLEAN fStart )
{
	gubLogForMeTooBleeds = fStart;
}


void SetEngagedInConvFromPCAction( SOLDIERTYPE *pSoldier )
{
	// OK, If a good give, set engaged in conv...
	gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
	gTacticalStatus.ubEngagedInConvFromActionMercID = pSoldier->ubID;
}

void UnSetEngagedInConvFromPCAction( SOLDIERTYPE *pSoldier )
{
	if ( gTacticalStatus.ubEngagedInConvFromActionMercID == pSoldier->ubID )
	{
		// OK, If a good give, set engaged in conv...
		gTacticalStatus.uiFlags &= ( ~ENGAGED_IN_CONV );
	}
}


BOOLEAN IsStopTimeQuote( UINT16 usQuoteNum )
{
	INT32 cnt;

	for ( cnt = 0; cnt < gubNumStopTimeQuotes; cnt++ )
	{
		if ( gusStopTimeQuoteList[ cnt ] == usQuoteNum )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void CheckForStopTimeQuotes( UINT16 usQuoteNum )
{
	if ( IsStopTimeQuote( usQuoteNum ) )
	{
		// Stop Time, game
		EnterModalTactical( TACTICAL_MODAL_NOMOUSE );

		gpCurrentTalkingFace->uiFlags		|= FACE_MODAL;

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Starting Modal Tactical Quote." );

	}
}

void SetStopTimeQuoteCallback( MODAL_HOOK pCallBack )
{
	gModalDoneCallback = pCallBack;
}


BOOLEAN IsMercSayingDialogue( UINT8 ubProfileID )
{
	if ( gpCurrentTalkingFace != NULL && gubCurrentTalkingID == ubProfileID )
	{
		return( TRUE );
	}
	return( FALSE );
}





BOOLEAN ShouldMercSayPrecedentToRepeatOneSelf( UINT8 ubMercID, UINT32 uiQuoteID )
{
	UINT8	ubQuoteBit=0;

	//If the quote is not in the array
	if( !IsQuoteInPrecedentArray( uiQuoteID ) )
	{
		return( FALSE );
	}

	ubQuoteBit = GetQuoteBitNumberFromQuoteID( uiQuoteID );
	if( ubQuoteBit == 0 )
		return( FALSE );

	if( GetMercPrecedentQuoteBitStatus( ubMercID, ubQuoteBit ) )
	{
		return( TRUE );
	}
	else
	{
		SetMercPrecedentQuoteBitStatus( ubMercID, ubQuoteBit );
	}

	return( FALSE );
}



BOOLEAN GetMercPrecedentQuoteBitStatus( UINT8 ubMercID, UINT8 ubQuoteBit )
{
	if( gMercProfiles[ ubMercID ].uiPrecedentQuoteSaid & ( 1 << ( ubQuoteBit - 1 ) ) )
		return( TRUE );
	else
		return( FALSE );
}

BOOLEAN SetMercPrecedentQuoteBitStatus( UINT8 ubMercID, UINT8 ubBitToSet )
{
	//Set the bit
	gMercProfiles[ ubMercID ].uiPrecedentQuoteSaid |= 1 << ( ubBitToSet - 1 );

	return( TRUE );
}



BOOLEAN IsQuoteInPrecedentArray( UINT32 uiQuoteID )
{
	UINT8	ubCnt;

	//If the quote id is above or below the ones in the array
	if( uiQuoteID < gubMercValidPrecedentQuoteID[ 0 ] ||
			uiQuoteID > gubMercValidPrecedentQuoteID[ NUMBER_VALID_MERC_PRECEDENT_QUOTES-1 ] )
	{
		return( FALSE );
	}


	//loop through all the quotes
	for( ubCnt=0; ubCnt<NUMBER_VALID_MERC_PRECEDENT_QUOTES;ubCnt++)
	{
		if( gubMercValidPrecedentQuoteID[ ubCnt ] == uiQuoteID )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}



UINT8	GetQuoteBitNumberFromQuoteID( UINT32 uiQuoteID )
{
	UINT8 ubCnt;

	//loop through all the quotes
	for( ubCnt=0; ubCnt<NUMBER_VALID_MERC_PRECEDENT_QUOTES;ubCnt++)
	{
		if( gubMercValidPrecedentQuoteID[ ubCnt ] == uiQuoteID )
		{
			return( ubCnt );
		}
	}

	return( 0 );
}

void HandleShutDownOfMapScreenWhileExternfaceIsTalking( void )
{

	if ( ( fExternFaceBoxRegionCreated ) && ( gpCurrentTalkingFace) )
	{
		RemoveVideoOverlay( gpCurrentTalkingFace->iVideoOverlay );
		gpCurrentTalkingFace->iVideoOverlay = -1;
	}

}


void HandleImportantMercQuote( SOLDIERTYPE * pSoldier, UINT16 usQuoteNumber )
{
	// wake merc up for THIS quote
	if( pSoldier->flags.fMercAsleep )
	{
		TacticalCharacterDialogueWithSpecialEvent( pSoldier, usQuoteNumber, DIALOGUE_SPECIAL_EVENT_SLEEP, 0,0 );
		TacticalCharacterDialogue( pSoldier, usQuoteNumber );
		TacticalCharacterDialogueWithSpecialEvent( pSoldier, usQuoteNumber, DIALOGUE_SPECIAL_EVENT_SLEEP, 1,0 );
	}
	else
	{
		TacticalCharacterDialogue( pSoldier, usQuoteNumber );
	}
}


// handle pausing of the dialogue queue
void PauseDialogueQueue( void )
{
	gfDialogueQueuePaused = TRUE;
	return;
}

// unpause the dialogue queue
void UnPauseDialogueQueue( void )
{
	gfDialogueQueuePaused = FALSE;
	return;
}


void SetExternMapscreenSpeechPanelXY( INT16 sXPos, INT16 sYPos )
{
	gsExternPanelXPosition	 = sXPos;
	gsExternPanelYPosition	 = sYPos;
}


#ifdef JA2UB
//JA25 UB
BOOLEAN AreAllTheMercsFinishedSayingThereInitialHeliCrashQuotes()
{
	INT32                   cnt;
	SOLDIERTYPE             *pSoldier;

	// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	// look for all mercs on the same team, 
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{       
		//if the merc is alive, in sector, etc...
		if ( OK_CONTROLLABLE_MERC( pSoldier )  )
		{
			//if the merc is still not done the initial speech, and still prone
			if( pSoldier->fIgnoreGetupFromCollapseCheck )
			{
				//we arent done
				return( FALSE );
			}
		}
	}

	//See if Jerry is still waiting to get up
	pSoldier = FindSoldierByProfileID( JERRY_MILO_UB, FALSE );
	if( pSoldier )
	{
		//if the merc is still not done the initial speech, and still prone
		if( pSoldier->fIgnoreGetupFromCollapseCheck )
		{
			//we arent done
			return( FALSE );
		}
	}

	//all mercs on the team are done
	return( TRUE );
}

void InitJerriesSpeechCallBack()
{
	//Trigger Jerry Milo's script record 10 ( call action 301 )
	TriggerNPCRecord( JERRY_MILO_UB, 10 );

	//Clear the overlay
	RemoveJerryMiloBrokenLaptopOverlay();
}

void RemoveJerryMiloBrokenLaptopOverlay()
{
	//if the overlay is up
	if( gJa25SaveStruct.fJerryBreakingLaptopOccuring )
	{
		gJa25SaveStruct.fJerryBreakingLaptopOccuring = FALSE;

		RemoveVideoOverlay( giTextBoxOverlay );
		giTextBoxOverlay = -1;

		if( fTextBoxMouseRegionCreated )
		{
			MSYS_RemoveRegion( &gTextBoxMouseRegion );
			fTextBoxMouseRegionCreated = FALSE; 
		}
	}
}

void HandlePlayerClosingMorrisNoteDisplayedOnScreen()
{
	RemoveVideoOverlay( giTextBoxOverlay );
	giTextBoxOverlay = -1;

	if ( fTextBoxMouseRegionCreated )
	{
		MSYS_RemoveRegion( &gTextBoxMouseRegion );
		fTextBoxMouseRegionCreated = FALSE; 
	}

	if( gJa25SaveStruct.ubDisplayCommanderMorrisNote == DMN__DISPLAY_PART_2 )
	{
		gJa25SaveStruct.ubDisplayCommanderMorrisNote = DMN__FINISHED;

		HandleShowingRadioLocatorsInMorrisArea();
	}
	else
	{
		DelayedMercQuote( (UINT16)(gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote), DQ__MORRIS_NOTE_DISPLAY_NOTE_1, GetWorldTotalSeconds() + 1 );
	}
}
#endif
