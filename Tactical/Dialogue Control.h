#ifndef _DIALOG_CONTROL_H
#define _DIALOG_CONTROL_H

#include "faces.h"
#include "gamescreen.h"
#include "soldier profile type.h"
#include <vector>

typedef struct
{
	UINT16		uiIndex; // add
	BOOLEAN EnabledSound;

} SOUND_PROFILE_VALUES;

extern SOUND_PROFILE_VALUES gSoundProfileValue[NUM_PROFILES];

// An enumeration for dialog quotes
enum DialogQuoteIDs
{
	// 0
	QUOTE_SEE_ENEMY = 0,
	QUOTE_SEE_ENEMY_VARIATION,
	QUOTE_IN_TROUBLE_SLASH_IN_BATTLE,
	QUOTE_SEE_CREATURE,
	QUOTE_FIRSTTIME_GAME_SEE_CREATURE,
	QUOTE_TRACES_OF_CREATURE_ATTACK,
	QUOTE_HEARD_SOMETHING,
	QUOTE_SMELLED_CREATURE,
	QUOTE_WEARY_SLASH_SUSPUCIOUS,
	QUOTE_WORRIED_ABOUT_CREATURE_PRESENCE,

	//10
	QUOTE_ATTACKED_BY_MULTIPLE_CREATURES,
	QUOTE_SPOTTED_SOMETHING_ONE,
	QUOTE_SPOTTED_SOMETHING_TWO,
	QUOTE_OUT_OF_AMMO,
	QUOTE_SERIOUSLY_WOUNDED,
	QUOTE_BUDDY_1_KILLED,
	QUOTE_BUDDY_2_KILLED,
	QUOTE_LEARNED_TO_LIKE_MERC_KILLED,
	QUOTE_FORGETFULL_SLASH_CONFUSED,
	QUOTE_JAMMED_GUN,

	//20
	QUOTE_UNDER_HEAVY_FIRE,
	QUOTE_TAKEN_A_BREATING,
	QUOTE_CLOSE_CALL,
	QUOTE_NO_LINE_OF_FIRE,
	QUOTE_STARTING_TO_BLEED,
	QUOTE_NEED_SLEEP,
	QUOTE_OUT_OF_BREATH,
	QUOTE_KILLED_AN_ENEMY,
	QUOTE_KILLED_A_CREATURE,
	QUOTE_HATED_1,

	//30
	QUOTE_HATED_2,
	QUOTE_LEARNED_TO_HATE_MERC,
	QUOTE_AIM_KILLED_MIKE,
	QUOTE_MERC_QUIT_LEARN_TO_HATE = QUOTE_AIM_KILLED_MIKE,
	QUOTE_HEADSHOT,
	QUOTE_PERSONALITY_TRAIT,
	QUOTE_ASSIGNMENT_COMPLETE,
	QUOTE_REFUSING_ORDER,
	QUOTE_KILLING_DEIDRANNA,
	QUOTE_KILLING_QUEEN,
	QUOTE_ANNOYING_PC,

	//40
	QUOTE_STARTING_TO_WHINE,
	QUOTE_NEGATIVE_COMPANY,
	QUOTE_AIR_RAID,
	QUOTE_WHINE_EQUIPMENT,
	QUOTE_SOCIAL_TRAIT,
	QUOTE_PASSING_DISLIKE,
	QUOTE_EXPERIENCE_GAIN,
	QUOTE_REFUSE_TO_EATFOOD,
	QUOTE_REFUSE_TO_SMOKE,
	QUOTE_HATED_1_ARRIVES,
	QUOTE_MERC_QUIT_HATED_1 = QUOTE_HATED_1_ARRIVES,

	//50
	QUOTE_HATED_2_ARRIVES,
	QUOTE_MERC_QUIT_HATED_2 = QUOTE_HATED_2_ARRIVES,
	QUOTE_BUDDY_1_GOOD,
	QUOTE_BUDDY_2_GOOD,
	QUOTE_LEARNED_TO_LIKE_WITNESSED,
	QUOTE_DELAY_CONTRACT_RENEWAL,
	QUOTE_NOT_GETTING_PAID = QUOTE_DELAY_CONTRACT_RENEWAL,
	QUOTE_AIM_SEEN_MIKE,
	QUOTE_PC_DROPPED_OMERTA = QUOTE_AIM_SEEN_MIKE,
	QUOTE_BLINDED,
	QUOTE_DEFINITE_CANT_DO,
	QUOTE_LISTEN_LIKABLE_PERSON,
	QUOTE_ENEMY_PRESENCE,

	//60
	QUOTE_WARNING_OUTSTANDING_ENEMY_AFTER_RT,
	QUOTE_FOUND_SOMETHING_SPECIAL,
	QUOTE_SATISFACTION_WITH_GUN_AFTER_KILL,
	QUOTE_SPOTTED_JOEY,
	QUOTE_RESPONSE_TO_MIGUEL_SLASH_QUOTE_MERC_OR_RPC_LETGO,
	QUOTE_SECTOR_SAFE,
	QUOTE_STUFF_MISSING_DRASSEN,
	QUOTE_KILLED_FACTORY_MANAGER,
	QUOTE_SPOTTED_BLOODCAT,
	QUOTE_END_GAME_COMMENT,
	
	//70
	QUOTE_ENEMY_RETREATED,
	QUOTE_GOING_TO_AUTO_SLEEP,
	QUOTE_WORK_UP_AND_RETURNING_TO_ASSIGNMENT, // woke up from auto sleep, going back to work
	QUOTE_ME_TOO, // me too quote, in agreement with whatever the merc previous said
	QUOTE_USELESS_ITEM,
	QUOTE_BOOBYTRAP_ITEM,
	QUOTE_SUSPICIOUS_GROUND,
	QUOTE_DROWNING,
	QUOTE_MERC_REACHED_DESTINATION,
	QUOTE_JA2UB_LAPTOP_FIXED,			// JA2UB: played when the laptop is fixed

	//80
	QUOTE_REPUTATION_REFUSAL, //80
	QUOTE_NON_AIM_BUDDY_3_KILLED = QUOTE_REPUTATION_REFUSAL,
	QUOTE_DEATH_RATE_REFUSAL, //= 81,
	QUOTE_NON_AIM_BUDDY_4_KILLED = QUOTE_DEATH_RATE_REFUSAL,
	QUOTE_LAME_REFUSAL, //= 82,
	QUOTE_NON_AIM_BUDDY_5_KILLED = QUOTE_LAME_REFUSAL,	
	QUOTE_WONT_RENEW_CONTRACT_LAME_REFUSAL,	//=83,
	QUOTE_NON_AIM_HATED_3 = QUOTE_WONT_RENEW_CONTRACT_LAME_REFUSAL,
	QUOTE_ANSWERING_MACHINE_MSG, //= 84,
	QUOTE_NON_AIM_HATED_4 = QUOTE_ANSWERING_MACHINE_MSG,
	QUOTE_DEPARTING_COMMENT_CONTRACT_NOT_RENEWED_OR_48_OR_MORE, //=85,
	QUOTE_NON_AIM_HATED_5 = QUOTE_DEPARTING_COMMENT_CONTRACT_NOT_RENEWED_OR_48_OR_MORE,
	QUOTE_HATED_1_ON_TEAM,// = 86,
	QUOTE_NON_AIM_BUDDY_3_GOOD = QUOTE_HATED_1_ON_TEAM,
	QUOTE_HATED_2_ON_TEAM,// = 87,
	QUOTE_NON_AIM_BUDDY_4_GOOD = QUOTE_HATED_2_ON_TEAM,
	QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM,// = 88,
	QUOTE_NON_AIM_BUDDY_5_GOOD = QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM,
	QUOTE_CONTRACTS_OVER,// = 89,
	QUOTE_MERC_QUIT_HATED_3 = QUOTE_CONTRACTS_OVER,
	
	//90
	QUOTE_ACCEPT_CONTRACT_RENEWAL, // =90,
	QUOTE_MERC_QUIT_HATED_4 = QUOTE_ACCEPT_CONTRACT_RENEWAL,
	QUOTE_CONTRACT_ACCEPTANCE,// =91,
	QUOTE_MERC_QUIT_HATED_5 = QUOTE_CONTRACT_ACCEPTANCE,
	QUOTE_JOINING_CAUSE_BUDDY_1_ON_TEAM,// = 92,
	QUOTE_JOINING_CAUSE_BUDDY_2_ON_TEAM,// = 93,
	QUOTE_JOINING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM,// = 94,
	QUOTE_REFUSAL_RENEW_DUE_TO_MORALE,// = 95,
	QUOTE_PRECEDENT_TO_REPEATING_ONESELF,// = 96,
	QUOTE_REFUSAL_TO_JOIN_LACK_OF_FUNDS,// = 97,
	QUOTE_DEPART_COMMET_CONTRACT_NOT_RENEWED_OR_TERMINATED_UNDER_48,// = 98,
	QUOTE_DEATH_RATE_RENEWAL,

	//100
	QUOTE_HATED_1_ON_TEAM_WONT_RENEW,
	QUOTE_HATED_2_ON_TEAM_WONT_RENEW,
	QUOTE_LEARNED_TO_HATE_ON_TEAM_WONT_RENEW,
	QUOTE_RENEWING_CAUSE_BUDDY_1_ON_TEAM,
	QUOTE_RENEWING_CAUSE_BUDDY_2_ON_TEAM,
	QUOTE_RENEWING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM,
	QUOTE_PRECEDENT_TO_REPEATING_ONESELF_RENEW,
	QUOTE_RENEW_REFUSAL_DUE_TO_LACK_OF_FUNDS,
	QUOTE_GREETING,
	QUOTE_SMALL_TALK,

	//110
	QUOTE_IMPATIENT_QUOTE,					
	QUOTE_LENGTH_OF_CONTRACT,
	QUOTE_COMMENT_BEFORE_HANG_UP,
	QUOTE_HATED_1_ON_TEAM_LONGTIMETOHATE,		// played when we refuse to be hired because hated person is present and hatetime >= 24 (else play QUOTE_HATED_1_ON_TEAM)
	QUOTE_HATED_2_ON_TEAM_LONGTIMETOHATE,
	QUOTE_MERC_LEAVING_ALSUCO_SOON,
	QUOTE_MERC_GONE_UP_IN_PRICE,
#ifdef JA2UB
	QUOTE_ENTER_SECTOR_WITH_FAN_1,
	QUOTE_ENTER_SECTOR_WITH_FAN_2,
#endif

	QUOTE_AIM_BUDDY_3_KILLED = 119,

	//120
	QUOTE_AIM_BUDDY_4_KILLED,
	QUOTE_AIM_BUDDY_5_KILLED,
	QUOTE_AIM_HATED_3,
	QUOTE_AIM_HATED_4,
	QUOTE_AIM_HATED_5,
	QUOTE_HATED_3_ARRIVES,
	QUOTE_HATED_4_ARRIVES,
	QUOTE_HATED_5_ARRIVES,
	QUOTE_AIM_BUDDY_3_GOOD,
	QUOTE_AIM_BUDDY_4_GOOD,
	
	//130
	QUOTE_AIM_BUDDY_5_GOOD,
	QUOTE_HATED_3_ON_TEAM,
	QUOTE_HATED_4_ON_TEAM,
	QUOTE_HATED_5_ON_TEAM,
	QUOTE_JOINING_CAUSE_BUDDY_3_ON_TEAM,
	QUOTE_JOINING_CAUSE_BUDDY_4_ON_TEAM,
	QUOTE_JOINING_CAUSE_BUDDY_5_ON_TEAM,
	QUOTE_HATED_3_ON_TEAM_WONT_RENEW,
	QUOTE_HATED_4_ON_TEAM_WONT_RENEW,
	QUOTE_HATED_5_ON_TEAM_WONT_RENEW,

	//140
	QUOTE_RENEWING_CAUSE_BUDDY_3_ON_TEAM,
	QUOTE_RENEWING_CAUSE_BUDDY_4_ON_TEAM,
	QUOTE_RENEWING_CAUSE_BUDDY_5_ON_TEAM,
	QUOTE_HATED_3_ON_TEAM_LONGTIMETOHATE,
	QUOTE_HATED_4_ON_TEAM_LONGTIMETOHATE,
	QUOTE_HATED_5_ON_TEAM_LONGTIMETOHATE,
};


#define			DEFAULT_EXTERN_PANEL_X_POS		320
#define			DEFAULT_EXTERN_PANEL_Y_POS		40


#define			DIALOGUE_TACTICAL_UI						1
#define			DIALOGUE_CONTACTPAGE_UI					2
#define			DIALOGUE_NPC_UI									3
#define			DIALOGUE_SPECK_CONTACT_PAGE_UI	4
#define	 DIALOGUE_EXTERNAL_NPC_UI				5
#define			DIALOGUE_SHOPKEEPER_UI					6
#define			DIALOGUE_SNITCH_UI					7	// anv: special handle for snitch dialogue

#define			DIALOGUE_SPECIAL_EVENT_GIVE_ITEM									0x00000001
#define			DIALOGUE_SPECIAL_EVENT_TRIGGER_NPC								0x00000002
#define			DIALOGUE_SPECIAL_EVENT_GOTO_GRIDNO								0x00000004
#define			DIALOGUE_SPECIAL_EVENT_DO_ACTION									0x00000008
#define			DIALOGUE_SPECIAL_EVENT_CLOSE_PANEL								0x00000010
#define	 DIALOGUE_SPECIAL_EVENT_PCTRIGGERNPC								0x00000020
#define	 DIALOGUE_SPECIAL_EVENT_BEGINPREBATTLEINTERFACE		0x00000040
#define			DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT			0x00000080
#define	 DIALOGUE_SPECIAL_EVENT_SHOW_CONTRACT_MENU		 0x00000100
#define			DIALOGUE_SPECIAL_EVENT_MINESECTOREVENT						0x00000200	
#define			DIALOGUE_SPECIAL_EVENT_SHOW_UPDATE_MENU			0x00000400
#define			DIALOGUE_SPECIAL_EVENT_ENABLE_AI									0x00000800
#define			DIALOGUE_SPECIAL_EVENT_USE_ALTERNATE_FILES				0x00001000

#ifdef JA2UB
#define			DIALOGUE_SPECIAL_EVENT_JERRY_MILO								  0x00002000
#else
#define			DIALOGUE_SPECIAL_EVENT_CONTINUE_TRAINING_MILITIA	0x00002000
#endif

#define			DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING						0x00004000
#define			DIALOGUE_SPECIAL_EVENT_MULTIPURPOSE								0x00008000
#define			DIALOGUE_SPECIAL_EVENT_SLEEP											0x00010000
#define			DIALOGUE_SPECIAL_EVENT_DO_BATTLE_SND							0x00020000
#define			DIALOGUE_SPECIAL_EVENT_SIGNAL_ITEM_LOCATOR_START	0x00040000
#define			DIALOGUE_SPECIAL_EVENT_SHOPKEEPER									0x00080000
#define			DIALOGUE_SPECIAL_EVENT_SKIP_A_FRAME								0x00100000
#define			DIALOGUE_SPECIAL_EVENT_EXIT_MAP_SCREEN						0x00200000
#define			DIALOGUE_SPECIAL_EVENT_DISPLAY_STAT_CHANGE				0x00400000
#define			DIALOGUE_SPECIAL_EVENT_UNSET_ARRIVES_FLAG					0x00800000
#define			DIALOGUE_SPECIAL_EVENT_TRIGGERPREBATTLEINTERFACE	0x01000000
#define			DIALOGUE_ADD_EVENT_FOR_SOLDIER_UPDATE_BOX					0x02000000
#define			DIALOGUE_SPECIAL_EVENT_ENTER_MAPSCREEN						0x04000000
#define			DIALOGUE_SPECIAL_EVENT_LOCK_INTERFACE							0x08000000
#define			DIALOGUE_SPECIAL_EVENT_REMOVE_EPC									0x10000000
#define			DIALOGUE_SPECIAL_EVENT_CONTRACT_WANTS_TO_RENEW		0x20000000
#define			DIALOGUE_SPECIAL_EVENT_CONTRACT_NOGO_TO_RENEW			0x40000000
#define			DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING_NO_ASK_EQUIP	0x80000000

#ifdef JA2UB
#define			MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING	0x20000000
#define			MULTIPURPOSE_SPECIAL_EVENT_DONE_KILLING_DEIDRANNA			0x10000000
#else
#define			MULTIPURPOSE_SPECIAL_EVENT_DONE_KILLING_DEIDRANNA			0x00000001
#define			MULTIPURPOSE_SPECIAL_EVENT_TEAM_MEMBERS_DONE_TALKING	0x00000002
#endif

#define			MULTIPURPOSE_SPECIAL_EVENT_SNITCH_DIALOGUE				0x00000004
#define			MULTIPURPOSE_SPECIAL_EVENT_ADDITIONAL_DIALOGUE			0x00000008

#ifdef JA2UB
enum{
     JERRY_MELO_FACE = 6,
     NUMBER_OF_EXTERNAL_NPC_FACES,
};
#endif

enum{
	SKYRIDER_EXTERNAL_FACE =0,
	WALDO_EXTERNAL_FACE,
	//MINER_FRED_EXTERNAL_FACE,
	//MINER_MATT_EXTERNAL_FACE,
	//MINER_OSWALD_EXTERNAL_FACE,
	//MINER_CALVIN_EXTERNAL_FACE,
	//MINER_CARL_EXTERNAL_FACE,
	//NUMBER_OF_EXTERNAL_NPC_FACES,
};


// soldier up-date box reasons
enum{
	UPDATE_BOX_REASON_ADDSOLDIER =0,
	UPDATE_BOX_REASON_SET_REASON,
	UPDATE_BOX_REASON_SHOW_BOX,
};

#ifdef JA2UB
extern UINT32 uiExternalStaticNPCFacesUB[ ];
extern UINT32 uiExternalFaceProfileIdsUB[ ];
#endif

//extern UINT32 uiExternalStaticNPCFaces[ ];
extern std::vector<UINT32> uiExternalStaticNPCFaces;
//extern UINT32 uiExternalFaceProfileIds[ ];

// Functions for handling dialogue Q
BOOLEAN InitalizeDialogueControl();
void ShutdownDialogueControl();
void EmptyDialogueQueue( );
void HandleDialogue( );
void HandleImportantMercQuote( SOLDIERTYPE * pSoldier, UINT16 usQuoteNumber );



// Send in a profile number to see if text dialog exists for this guy....
BOOLEAN DialogueDataFileExistsForProfile( UINT8 ubCharacterNum, UINT16 usQuoteNum, BOOLEAN fWavFile, STR8 *ppStr );

// Do special event as well as dialogue!
BOOLEAN CharacterDialogueWithSpecialEvent( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2 );

// Do special event as well as dialogue!
BOOLEAN CharacterDialogueWithSpecialEventEx( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 );

// A higher level function used for tactical quotes
BOOLEAN TacticalCharacterDialogueWithSpecialEvent( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2 );

// A higher level function used for tactical quotes
BOOLEAN TacticalCharacterDialogueWithSpecialEventEx( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT32 uiFlag, UINT32 uiData1, UINT32 uiData2, UINT32 uiData3 );

// A higher level function used for tactical quotes
BOOLEAN TacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum );

BOOLEAN SnitchTacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum, UINT8 ubEventType, UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile );

// Flugente: additional dialogue
enum AdditionalDialogEvents
{
	ADE_DISCOVER_ROOM = 0,					// merc is the 1st one to explore this room
	ADE_DIALOGUE_REACTION,					// someone else used a line, and we might answer to that
	ADE_OMERTA_ENTRY,						// similar to an IMP, we can comment upon the initial Omerta landing
	ADE_SECTOR_COMMENTARY,					// similar to the rebel's quotes, this is called upon visiting a sector for the first time
	ADE_MERCHANT_CHAT,						// we greet a merchant (used with non-profile merchants, in order to be a bit more lively)
	ADE_DIALOGUE_NPC_FRIENDLY,				// in a conversation, we can say something when clicking on a button
	ADE_DIALOGUE_NPC_DIRECT,
	ADE_DIALOGUE_NPC_THREATEN,
	ADE_DIALOGUE_NPC_RECRUIT,
	ADE_DIALOGUE_NPC_REPEAT,
	ADE_DIALOGUE_RPC_RECRUIT_SUCCESS,		// we've successfully recruited an RPC, and might comment on that
	ADE_SEX,								// does exactly what you think it does
	ADE_WEATHERCHANGE,						// the weather has changed, and we can warn the player about that
	ADE_SKILL_RESULT,						// we used a skill, and comment on the result
	ADE_GRENADEWARNING,						// a delayed enemy grenade was dropped nearby
	ADE_CONSUMEITEM,						// we applied an item to us
	ADE_NPC_DEATH,							// someone died
};

// We call this function from several places. It uses the dialogue functions, but calls a Lua script to know whether something, and what, should be said
// This allows us to call a lot of very specific dialogue for individual mercs without the need to define what exactly should be said in the code -
// a modder can simply control in Lua what should happen
BOOLEAN AdditionalTacticalCharacterDialogue_CallsLua( SOLDIERTYPE *pSoldier, UINT16 usEventNr, UINT32 aData1 = 0, UINT32 aData2 = 0, UINT32 aData3 = 0 );

// call event for all mercs in a sector
void AdditionalTacticalCharacterDialogue_AllInSector( INT16 aSectorX, INT16 aSectorY, INT8 aSectorZ, UINT8 ausIgnoreProfile, 
	UINT16 usEventNr, UINT32 aData1 = 0, UINT32 aData2 = 0, UINT32 aData3 = 0, INT32 aAroundGridno = -1, INT32 aRadius = 0 );

void AdditionalTacticalCharacterDialogue_AllInSectorRadiusCall( UINT8 ausIgnoreProfile,	UINT16 usEventNr, 
	UINT32 aData1 = 0, UINT32 aData2 = 0, UINT32 aData3 = 0, INT32 aAroundGridno = -1, INT32 aRadius = 0 );

// Flugente: play sound file like normal dialogue or, if someone else is already talking, just play the sound
void SpecialDialogue( SOLDIERTYPE* pSoldier, STR8 azSoundString, STR16 azTextString );

// Flugente: replace text with other text
BOOLEAN ReplaceTextWithOtherText( CHAR16 *pFinishedString, CHAR16 compare[32], CHAR16 replace[32] );

// A higher level function used for tactical quotes
BOOLEAN DelayedTacticalCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum );

// A more general purpose function for processing quotes
BOOLEAN CharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed );

BOOLEAN SnitchCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex,
	UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, 
	UINT32 uiSpecialEventData3, UINT32 uiSpecialEventData4, 
	UINT8 bUIHandlerID, BOOLEAN fFromSoldier, BOOLEAN fDelayed );

// A special event can be setup which can be queued with other speech
BOOLEAN SpecialCharacterDialogueEvent( UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, UINT32 uiSpecialEventData3, INT32 iFaceIndex, UINT8 bUIHandlerID );

// Same as above, for triggers, with extra param to hold approach value
BOOLEAN SpecialCharacterDialogueEventWithExtraParam( UINT32 uiSpecialEventFlag, UINT32 uiSpecialEventData1, UINT32 uiSpecialEventData2, UINT32 uiSpecialEventData3, UINT32 uiSpecialEventData4, INT32 iFaceIndex, UINT8 bUIHandlerID );

// execute specific character dialogue
BOOLEAN ExecuteCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, BOOLEAN fSoldier );

BOOLEAN ExecuteSnitchCharacterDialogue( UINT8 ubCharacterNum, UINT16 usQuoteNum, INT32 iFaceIndex, UINT8 bUIHandlerID, UINT8 ubSnitchEventType, UINT8 ubSnitchTargetID, UINT8 ubSecondarySnitchTargetID );

// Called when a face stops talking...
void HandleDialogueEnd( FACETYPE *pFace );

// shut down last quotetext box
void ShutDownLastQuoteTacticalTextBox( void );

// Called to advance speech
// Used for option when no speech sound file
void DialogueAdvanceSpeech( );

BOOLEAN DialogueQueueIsEmpty( );
BOOLEAN	DialogueQueueIsEmptyOrSomebodyTalkingNow( );

// Adjust the face, etc when switching from panel to extern panel...
void HandleDialogueUIAdjustments( );

// pause game time during 
void PauseTimeDuringNextQuote( );
void UnPauseGameDuringNextQuote( void );


// set up and shutdown static external NPC faces
void InitalizeStaticExternalNPCFaces( void );
void ShutdownStaticExternalNPCFaces( void );


void SayQuoteFromAnyBodyInSector( UINT16 usQuoteNum );
void SayQuoteFromAnyBodyInThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT16 usQuoteNum );
void SayQuoteFromNearbyMercInSector( INT32 sGridNo, INT8 bDistance, UINT16 usQuoteNum );
void SayQuote58FromNearbyMercInSector( INT32 sGridNo, INT8 bDistance, UINT16 usQuoteNum, INT8 bSex );


void ExecuteTacticalTextBox( INT16 sLeftPosition, STR16 pString );

void ExecuteTacticalTextBoxForLastQuote( INT16 sLeftPosition, STR16 pString );

UINT32 FindDelayForString( STR16 sString );

void BeginLoggingForBleedMeToos( BOOLEAN fStart );

void UnSetEngagedInConvFromPCAction( SOLDIERTYPE *pSoldier );
void SetEngagedInConvFromPCAction( SOLDIERTYPE *pSoldier );


extern UINT32 guiDialogueLastQuoteTime;
extern UINT32 guiDialogueLastQuoteDelay;

void SetStopTimeQuoteCallback( MODAL_HOOK pCallBack );

BOOLEAN DialogueActive( );

extern INT32 giNPCReferenceCount;
extern INT32 giNPCSpecialReferenceCount;


#define	NUMBER_VALID_MERC_PRECEDENT_QUOTES	13

extern	UINT8	gubMercValidPrecedentQuoteID[ NUMBER_VALID_MERC_PRECEDENT_QUOTES ];

BOOLEAN ShouldMercSayPrecedentToRepeatOneSelf( UINT8 ubMercID, UINT32 uiQuoteID );
BOOLEAN GetMercPrecedentQuoteBitStatus( UINT8 ubMercID, UINT8 ubQuoteBit );
BOOLEAN SetMercPrecedentQuoteBitStatus( UINT8 ubMercID, UINT8 ubBitToSet );
BOOLEAN IsQuoteInPrecedentArray( UINT32 uiQuoteID );
UINT8	GetQuoteBitNumberFromQuoteID( UINT32 uiQuoteID );
void HandleShutDownOfMapScreenWhileExternfaceIsTalking( void );

void StopAnyCurrentlyTalkingSpeech( );

// handle pausing of the dialogue queue
void PauseDialogueQueue( void );

// unpause the dialogue queue
void UnPauseDialogueQueue( void );

void SetExternMapscreenSpeechPanelXY( INT16 sXPos, INT16 sYPos );

#ifdef JA2UB
void RemoveJerryMiloBrokenLaptopOverlay();
#endif

#endif