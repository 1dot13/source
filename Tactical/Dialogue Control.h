#ifndef _DIALOG_CONTROL_H
#define _DIALOG_CONTROL_H

#include "faces.h"
#include "gamescreen.h"
#include "soldier profile type.h"
#include <vector>

// An enumeration for dialog quotes
enum DialogQuoteIDs
{
	// 0
	QUOTE_SEE_ENEMY = 0,						// used when we spot an enemy
	QUOTE_SEE_ENEMY_VARIATION,					// used when we spot an enemy in this sector for the first time
	QUOTE_IN_TROUBLE_SLASH_IN_BATTLE,			// used when we spot an enemy and they significantly outnumber us
	QUOTE_SEE_CREATURE,							// used when we spot a bug
	QUOTE_FIRSTTIME_GAME_SEE_CREATURE,			// used when we spot a bug for the first time
	QUOTE_TRACES_OF_CREATURE_ATTACK,			// randomly played during the bug quest in the tunnels
	QUOTE_HEARD_SOMETHING,						// we heard a suspicious sound
	QUOTE_SMELLED_CREATURE,						// randomly played during the bug quest in the tunnels
	QUOTE_WEARY_SLASH_SUSPUCIOUS,				// we are weary, because some non-militia NPC has a rocketlauncher or cannon
	QUOTE_WORRIED_ABOUT_CREATURE_PRESENCE,		// randomly played during the bug quest in the tunnels

	//10
	QUOTE_ATTACKED_BY_MULTIPLE_CREATURES,		// used when we spot several bugs
	QUOTE_SPOTTED_SOMETHING_ONE,				// spotted an item
	QUOTE_SPOTTED_SOMETHING_TWO,				// spotted an item
	QUOTE_OUT_OF_AMMO,							// gun is empty
	QUOTE_SERIOUSLY_WOUNDED,					// lose life from bleeding below 15 HP
	QUOTE_BUDDY_1_KILLED,						// buddy 1 died
	QUOTE_BUDDY_2_KILLED,						// buddy 2 died
	QUOTE_LEARNED_TO_LIKE_MERC_KILLED,			// learn to like died
	QUOTE_FORGETFULL_SLASH_CONFUSED,			// currently not used
	QUOTE_JAMMED_GUN,							// gun is jammed

	//20
	QUOTE_UNDER_HEAVY_FIRE,						// heavily suppressed
	QUOTE_TAKEN_A_BREATING,						// took a lot of hits this turn
	QUOTE_CLOSE_CALL,							// almost hit by a projectile
	QUOTE_NO_LINE_OF_FIRE,						// really bad chance to hit target
	QUOTE_STARTING_TO_BLEED,					// we are bleeding
	QUOTE_NEED_SLEEP,							// going to sleep
	QUOTE_OUT_OF_BREATH,						// very low on breath points
	QUOTE_KILLED_AN_ENEMY,						// killed an enemy
	QUOTE_KILLED_A_CREATURE,					// killed an bug
	QUOTE_HATED_1,								// complain about hated 1

	//30
	QUOTE_HATED_2,								// complain about hated 2
	QUOTE_LEARNED_TO_HATE_MERC,					// complain about learn to hate
	QUOTE_AIM_KILLED_MIKE,						// AIM: killed Mike
	QUOTE_MERC_QUIT_LEARN_TO_HATE = QUOTE_AIM_KILLED_MIKE,	// MERC: quit because of guy we learned to hate
	QUOTE_HEADSHOT,								// complain about head explosion/corpse being disgusting
	QUOTE_PERSONALITY_TRAIT,					// disability kicks in (forgetfulness, afraid of heights etc.)
	QUOTE_ASSIGNMENT_COMPLETE,					// assignment is complete, so we tell the player
	QUOTE_REFUSING_ORDER,						// refuse to do action to target
	QUOTE_KILLING_DEIDRANNA,					// killed (or witnessed death of) Deidranna
	QUOTE_KILLING_QUEEN,						// killed (or witnessed death of) bug queen
	QUOTE_ANNOYING_PC,							// we dislike the NPC we just talked to

	//40
	QUOTE_STARTING_TO_WHINE,					// complain: low morale
	QUOTE_NEGATIVE_COMPANY,						// randomly played when we talk to a merc
	QUOTE_AIR_RAID,								// notify player of air raid / enemy helicopter spotted
	QUOTE_WHINE_EQUIPMENT,						// complain about quality of equipment
	QUOTE_SOCIAL_TRAIT,							// randomly played when we talk to a merc
	QUOTE_PASSING_DISLIKE,						// randomly played when we talk to a merc
	QUOTE_EXPERIENCE_GAIN,						// gained stats
	QUOTE_REFUSE_TO_EATFOOD,					// refuse to eat food
	QUOTE_REFUSE_TO_SMOKE,						// refuse to smoke
	QUOTE_HATED_1_ARRIVES,						// AIM: hated 1 arrives
	QUOTE_MERC_QUIT_HATED_1 = QUOTE_HATED_1_ARRIVES,	// MERC/NPC: quit because of hated 1

	//50
	QUOTE_HATED_2_ARRIVES,						// AIM: hated 2 arrives
	QUOTE_MERC_QUIT_HATED_2 = QUOTE_HATED_2_ARRIVES,	// MERC/NPC: quit because of hated 2
	QUOTE_BUDDY_1_GOOD,							// compliment buddy 1
	QUOTE_BUDDY_2_GOOD,							// compliment buddy 2
	QUOTE_LEARNED_TO_LIKE_WITNESSED,			// compliment learn to like
	QUOTE_DELAY_CONTRACT_RENEWAL,				// currently unused
	QUOTE_NOT_GETTING_PAID = QUOTE_DELAY_CONTRACT_RENEWAL,	// MERC: complain about not being paid
	QUOTE_AIM_SEEN_MIKE,						// AIM/MERC: see Mike
	QUOTE_PC_DROPPED_OMERTA = QUOTE_AIM_SEEN_MIKE,		// IMP: airdop in Omerta
	QUOTE_NOT_GETTING_PAID_RPC = QUOTE_PC_DROPPED_OMERTA,	// RPC: complain about not being paid
	QUOTE_BLINDED,								// an attack has blinded us
	QUOTE_DEFINITE_CANT_DO,						// we are hopeless at this skill check
	QUOTE_LISTEN_LIKABLE_PERSON,				// we like the NPC we just talked to
	QUOTE_ENEMY_PRESENCE,						// prior to combat, notify us of enemy presence

	//60
	QUOTE_WARNING_OUTSTANDING_ENEMY_AFTER_RT,	// realtime has begun, but hostiles are still around
	QUOTE_FOUND_SOMETHING_SPECIAL,				// we picked up an item we think is cool (once per day)
	QUOTE_SATISFACTION_WITH_GUN_AFTER_KILL,		// we are happy with the gun we are currently using (once per day)
	QUOTE_SPOTTED_JOEY,							// we spotted Joey during his quest
	QUOTE_RESPONSE_TO_MIGUEL_SLASH_QUOTE_MERC_OR_RPC_LETGO,		// AIM: answer to Miguel speech, MERC/RPC: departing comment upon contract end
	QUOTE_SECTOR_SAFE,							// we won the battle
	QUOTE_STUFF_MISSING_DRASSEN,				// notice shipment in Drassen was stolen
	QUOTE_KILLED_FACTORY_MANAGER,				// killed Doreen
	QUOTE_SPOTTED_BLOODCAT,						// used when we spot a bloodcat
	QUOTE_END_GAME_COMMENT,						// end speech after game is won
	
	//70
	QUOTE_ENEMY_RETREATED,						// we won the battle due to enemy retreat
	QUOTE_GOING_TO_AUTO_SLEEP,					// currently not used
	QUOTE_WORK_UP_AND_RETURNING_TO_ASSIGNMENT,	// woke up from auto sleep, going back to work
	QUOTE_ME_TOO,								// me too quote, in agreement with whatever the merc previous said
	QUOTE_USELESS_ITEM,							// tried working the item in hand, and it's broken
	QUOTE_BOOBYTRAP_ITEM,						// item/door/etc. is boobytrapped
	QUOTE_SUSPICIOUS_GROUND,					// found mine etc. on floor tile
	QUOTE_DROWNING,								// currently not used, intended upon drowning
	QUOTE_MERC_REACHED_DESTINATION,				// strategic travel: arrived at destination
	QUOTE_JA2UB_LAPTOP_FIXED,					// JA2UB: played when the laptop is fixed

	//80
	QUOTE_REPUTATION_REFUSAL, //80								// AIM: refuse to be hired due to bad player reputation
	QUOTE_NON_AIM_BUDDY_3_KILLED = QUOTE_REPUTATION_REFUSAL,	// not AIM: buddy 3 died
	QUOTE_DEATH_RATE_REFUSAL, //= 81,							// AIM: refuse to be hired due to high death rate
	QUOTE_NON_AIM_BUDDY_4_KILLED = QUOTE_DEATH_RATE_REFUSAL,	// not AIM: buddy 4 died
	QUOTE_LAME_REFUSAL, //= 82,									// AIM: refuse to be hired due not being in the mood (hangover etc.)
	QUOTE_NON_AIM_BUDDY_5_KILLED = QUOTE_LAME_REFUSAL,			// not AIM: buddy 5 died
	QUOTE_WONT_RENEW_CONTRACT_LAME_REFUSAL,	//=83,				// AIM: refuse to be renew contract due to already having signed elsewhere
	QUOTE_NON_AIM_HATED_3 = QUOTE_WONT_RENEW_CONTRACT_LAME_REFUSAL,		// not AIM: complain about hated 3
	QUOTE_ANSWERING_MACHINE_MSG, //= 84,								// AIM: message that plays on the answering machine
	QUOTE_NON_AIM_HATED_4 = QUOTE_ANSWERING_MACHINE_MSG,				// not AIM: complain about hated 4
	QUOTE_DEPARTING_COMMENT_CONTRACT_NOT_RENEWED_OR_48_OR_MORE, //=85,	// AIM: warn that contract is >= 48 hours, would like to renew
	QUOTE_NON_AIM_HATED_5 = QUOTE_DEPARTING_COMMENT_CONTRACT_NOT_RENEWED_OR_48_OR_MORE,		// not AIM: complain about hated 5
	QUOTE_HATED_1_ON_TEAM,// = 86,								// AIM: played when we refuse to be hired because hated 1 is present and bHatedTime < 24 (else play QUOTE_HATED_1_ON_TEAM_LONGTIMETOHATE)
	QUOTE_NON_AIM_BUDDY_3_GOOD = QUOTE_HATED_1_ON_TEAM,			// not AIM: compliment buddy 3
	QUOTE_HATED_2_ON_TEAM,// = 87,								// AIM: played when we refuse to be hired because hated 2 is present and bHatedTime < 24 (else play QUOTE_HATED_2_ON_TEAM_LONGTIMETOHATE)
	QUOTE_NON_AIM_BUDDY_4_GOOD = QUOTE_HATED_2_ON_TEAM,			// not AIM: compliment buddy 4
	QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM,// = 88,					// AIM: played when we refuse to be hired because learn to hate is present
	QUOTE_NON_AIM_BUDDY_5_GOOD = QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM,		// not AIM: compliment buddy 5
	QUOTE_CONTRACTS_OVER,// = 89,								// tell player that contract is over
	QUOTE_MERC_QUIT_HATED_3 = QUOTE_CONTRACTS_OVER,				// MERC/RPC: quit because of hated 3
	
	//90
	QUOTE_ACCEPT_CONTRACT_RENEWAL, // =90,						// AIM: renew contract
	QUOTE_MERC_QUIT_HATED_4 = QUOTE_ACCEPT_CONTRACT_RENEWAL,	// MERC/RPC: quit because of hated 4
	QUOTE_CONTRACT_ACCEPTANCE,// =91,							// AIM: agree to contract
	QUOTE_MERC_QUIT_HATED_5 = QUOTE_CONTRACT_ACCEPTANCE,		// MERC/RPC: quit because of hated 5
	QUOTE_JOINING_CAUSE_BUDDY_1_ON_TEAM,// = 92,				// agree to contract because buddy 1 is here
	QUOTE_JOINING_CAUSE_BUDDY_2_ON_TEAM,// = 93,				// agree to contract because buddy 2 is here
	QUOTE_JOINING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM,// = 94,	// agree to contract because learn to like is here
	QUOTE_REFUSAL_RENEW_DUE_TO_MORALE,// = 95,					// AIM: refuse to be renew due to bad player reputation or morale
	QUOTE_PRECEDENT_TO_REPEATING_ONESELF,// = 96,				// say that you're repeating yourself
	QUOTE_REFUSAL_TO_JOIN_LACK_OF_FUNDS,// = 97,				// player can't afford hiring us
	QUOTE_DEPART_COMMET_CONTRACT_NOT_RENEWED_OR_TERMINATED_UNDER_48,// = 98,	// AIM: warn that contract is < 48 hours, would like to renew
	QUOTE_DEATH_RATE_RENEWAL,									// AIM: refuse to be renew due to high death rate

	//100
	QUOTE_HATED_1_ON_TEAM_WONT_RENEW,			// refuse to renew contract because hated 1 is here
	QUOTE_HATED_2_ON_TEAM_WONT_RENEW,			// refuse to renew contract because hated 2 is here
	QUOTE_LEARNED_TO_HATE_ON_TEAM_WONT_RENEW,	// refuse to renew contract because learn to hate is here
	QUOTE_RENEWING_CAUSE_BUDDY_1_ON_TEAM,		// renew contract only because buddy 1 is around
	QUOTE_RENEWING_CAUSE_BUDDY_2_ON_TEAM,		// renew contract only because buddy 2 is around
	QUOTE_RENEWING_CAUSE_LEARNED_TO_LIKE_BUDDY_ON_TEAM,		// renew contract only because learn to like is around
	QUOTE_PRECEDENT_TO_REPEATING_ONESELF_RENEW,	// repeat yourself during renewal process
	QUOTE_RENEW_REFUSAL_DUE_TO_LACK_OF_FUNDS,	// JA2UB: kill bloodcat quest fulfilled
	QUOTE_GREETING,								// AIM hiring screen: greet the player
	QUOTE_SMALL_TALK,							// AIM hiring screen: player says nothing, so do a bit of small talk

	//110
	QUOTE_IMPATIENT_QUOTE,						// AIM hiring screen: player says nothing, so get impatient / easter egg: female mercs complain about player trying to abuse the 'strip' command
	QUOTE_LENGTH_OF_CONTRACT,					// AIM hiring screen: ask player what contract terms they want
	QUOTE_COMMENT_BEFORE_HANG_UP,				// AIM hiring screen: player says nothing, we are fed up so end the call
	QUOTE_HATED_1_ON_TEAM_LONGTIMETOHATE,		// AIM: played when we refuse to be hired because hated 1 is present and bHatedTime >= 24 (else play QUOTE_HATED_1_ON_TEAM)
	QUOTE_HATED_2_ON_TEAM_LONGTIMETOHATE,		// AIM: played when we refuse to be hired because hated 2 is present and bHatedTime >= 24 (else play QUOTE_HATED_2_ON_TEAM)
	QUOTE_MERC_LEAVING_ALSUCO_SOON,				// tell player that contract will be over soon and we don't want to renew
	QUOTE_MERC_GONE_UP_IN_PRICE,				// during contract renegotiation, inform the player of increased salary
#ifdef JA2UB
	QUOTE_ENTER_SECTOR_WITH_FAN_1,				// special JA2UB dialogue about fan
	QUOTE_ENTER_SECTOR_WITH_FAN_2,				// special JA2UB dialogue about fan
#endif

	QUOTE_AIM_BUDDY_3_KILLED = 119,				// AIM: buddy 3 died

	//120
	QUOTE_AIM_BUDDY_4_KILLED,					// AIM: buddy 4 died
	QUOTE_AIM_BUDDY_5_KILLED,					// AIM: buddy 5 died
	QUOTE_AIM_HATED_3,							// AIM: complain about hated 3
	QUOTE_AIM_HATED_4,							// AIM: complain about hated 4
	QUOTE_AIM_HATED_5,							// AIM: complain about hated 5
	QUOTE_HATED_3_ARRIVES,						// AIM: hated 3 arrives
	QUOTE_HATED_4_ARRIVES,						// AIM: hated 4 arrives
	QUOTE_HATED_5_ARRIVES,						// AIM: hated 5 arrives
	QUOTE_AIM_BUDDY_3_GOOD,						// AIM: compliment buddy 3
	QUOTE_AIM_BUDDY_4_GOOD,						// AIM: compliment buddy 4
	
	//130
	QUOTE_AIM_BUDDY_5_GOOD,						// AIM: compliment buddy 5
	QUOTE_HATED_3_ON_TEAM,						// AIM: played when we refuse to be hired because hated 3 is present and bHatedTime < 24 ( else play QUOTE_HATED_3_ON_TEAM_LONGTIMETOHATE )
	QUOTE_HATED_4_ON_TEAM,						// AIM: played when we refuse to be hired because hated 4 is present and bHatedTime < 24 ( else play QUOTE_HATED_4_ON_TEAM_LONGTIMETOHATE )
	QUOTE_HATED_5_ON_TEAM,						// AIM: played when we refuse to be hired because hated 5 is present and bHatedTime < 24 ( else play QUOTE_HATED_5_ON_TEAM_LONGTIMETOHATE )
	QUOTE_JOINING_CAUSE_BUDDY_3_ON_TEAM,		// agree to contract because buddy 3 is here
	QUOTE_JOINING_CAUSE_BUDDY_4_ON_TEAM,		// agree to contract because buddy 4 is here
	QUOTE_JOINING_CAUSE_BUDDY_5_ON_TEAM,		// agree to contract because buddy 5 is here
	QUOTE_HATED_3_ON_TEAM_WONT_RENEW,			// refuse to renew contract because hated 3 is here
	QUOTE_HATED_4_ON_TEAM_WONT_RENEW,			// refuse to renew contract because hated 4 is here
	QUOTE_HATED_5_ON_TEAM_WONT_RENEW,			// refuse to renew contract because hated 5 is here

	//140
	QUOTE_RENEWING_CAUSE_BUDDY_3_ON_TEAM,		// renew contract only because buddy 3 is around
	QUOTE_RENEWING_CAUSE_BUDDY_4_ON_TEAM,		// renew contract only because buddy 4 is around
	QUOTE_RENEWING_CAUSE_BUDDY_5_ON_TEAM,		// renew contract only because buddy 5 is around
	QUOTE_HATED_3_ON_TEAM_LONGTIMETOHATE,		// AIM: played when we refuse to be hired because hated 3 is present and bHatedTime >= 24 (else play QUOTE_HATED_3_ON_TEAM)
	QUOTE_HATED_4_ON_TEAM_LONGTIMETOHATE,		// AIM: played when we refuse to be hired because hated 4 is present and bHatedTime >= 24 (else play QUOTE_HATED_4_ON_TEAM)
	QUOTE_HATED_5_ON_TEAM_LONGTIMETOHATE,		// AIM: played when we refuse to be hired because hated 5 is present and bHatedTime >= 24 (else play QUOTE_HATED_5_ON_TEAM)
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
	ADE_MERC_ARRIVES,						// a mercenary arrives in Arulco. aData1 is the profile of the arriving merc, aData2 is 1 if the player hasn't made contact with the rebels yet
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
	ADE_SKILL_RESULT,						// we used a skill, and comment on the result, aData1 is of Skill, aData2 indicates whether it was a success (1) or failure (0)
	ADE_GRENADEWARNING,						// a delayed enemy grenade was dropped nearby
	ADE_CONSUMEITEM,						// we consumed an item, aData1 is the item index
	ADE_NPC_DEATH,							// someone died
	ADE_WITNESS_GOOD,						// similar to comment upon buddy doing good.
	ADE_BANDAGE_PERFORM_BEGIN,				// we bandage someone, aData1 is the profile of person
	ADE_BANDAGE_PERFORM_END,				// we finished bandaging someone, aData1 is the profile of person
	ADE_BANDAGE_RECEIVE_BEGIN,				// someone bandages us, aData1 is the profile of person
	ADE_BANDAGE_RECEIVE_END,				// someone finished bandaging us, aData1 is the profile of person
	ADE_ZOMBIESRISE,						// a new wave of zombies is spawned from corpses
	ADE_SEXUALHARASSMENT,					// the player wants to take the 'strip' command further than we are comfortable with
	ADE_LOCKBOMB,							// merc uses a shaped charge to blow the locks of a door, aData1 indicates whether it was a success (1) or failure (0)
	ADE_SNIPERWARNING,						// merc warns of sniper presence in this sector
	ADE_BOMB_HAS_BEEN_PLANTED,				// we planted a bomb/mine/etc., aData1 is the item index, aData2 indicates whether it was a success (1) or failure (0)
};

// We call this function from several places. It uses the dialogue functions, but calls a Lua script to know whether something, and what, should be said
// This allows us to call a lot of very specific dialogue for individual mercs without the need to define what exactly should be said in the code -
// a modder can simply control in Lua what should happen
BOOLEAN AdditionalTacticalCharacterDialogue_CallsLua( SOLDIERTYPE *pSoldier, UINT16 usEventNr, UINT32 aData1 = 0, UINT32 aData2 = 0, UINT32 aData3 = 0 );

// call event for all mercs in a sector
void AdditionalTacticalCharacterDialogue_AllInCurrentSector( UINT8 ausIgnoreProfile,
	UINT16 usEventNr, UINT32 aData1 = 0, UINT32 aData2 = 0, UINT32 aData3 = 0, INT32 aAroundGridno = -1, INT32 aRadius = 0 );

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