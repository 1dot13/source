#ifndef __DYNAMICDIALOGUE_H
#define __DYNAMICDIALOGUE_H

/**
* @file
* @author Flugente (bears-pit.com)
* @brief This file contains declarations of classes and functions used for dynamic dialogue
*/

#include "Types.h"
#include "Soldier Control.h"

// Flugente: calculate A's opinion of B
INT8	SoldierRelation( SOLDIERTYPE* pSoldierA, SOLDIERTYPE* pSoldierB );
INT8	SoldierRelation( UINT8 usProfileA, UINT8 usProfileB );

// Flugente: dynamic opinions
enum
{
	OPINIONEVENT_FRIENDLYFIRE,
	OPINIONEVENT_SNITCHSOLDMEOUT,
	OPINIONEVENT_SNITCHINTERFERENCE,
	OPINIONEVENT_FRIENDSWITHHATED,

	OPINIONEVENT_CONTRACTEXTENSION,
	OPINIONEVENT_ORDEREDRETREAT,
	OPINIONEVENT_CIVKILLER,
	OPINIONEVENT_SLOWSUSDOWN,

	OPINIONEVENT_NOSHARINGFOOD,
	OPINIONEVENT_ANNOYINGDISABILITY,
	OPINIONEVENT_ADDICT,
	OPINIONEVENT_THIEF,

	OPINIONEVENT_WORSTCOMMANDEREVER,
	OPINIONEVENT_RICHGUY,
	OPINIONEVENT_BETTERGEAR,
	OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS,

	OPINIONEVENT_BANDAGED,
	OPINIONEVENT_DRINKBUDDIES_GOOD,
	OPINIONEVENT_DRINKBUDDIES_SUPER,
	OPINIONEVENT_DRINKBUDDIES_BAD,

	OPINIONEVENT_DRINKBUDDIES_WORSE,
	// events from dialogue
	OPINIONEVENT_AGAINST_US,		// other guy was against us in dialogue
	OPINIONEVENT_FOR_US,			// other guy was for us in dialogue	
	OPINIONEVENT_AGAINST_ENEMY,		// other guy was against our dialogue enemy in dialogue

	OPINIONEVENT_FOR_ENEMY,			// other guy was for our dialogue enemy in dialogue
	OPINIONEVENT_SOLVECONFLICT_REASON_GOOD,		// other guy attempted to solve the conflict reasonably, and we thought that was good
	OPINIONEVENT_SOLVECONFLICT_REASON_BAD,		// other guy attempted to solve the conflict reasonably, and we thought that was bad
	OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_GOOD,		// other guy attempted to solve the conflict agressively, and we thought that was good

	OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD,		// other guy attempted to solve the conflict agressively, and we thought that was bad
	OPINIONEVENT_DISEASE_DISGUSTING,	// we have a disease, which others might find disgusting
	OPINIONEVENT_DISEASE_TREATMENT,		// someone treated our diseases
	OPINIONEVENT_BRUTAL_GOOD,		// other guy killed someone in a brutal fashion, and we like that

	OPINIONEVENT_BRUTAL_BAD,		// other guy killed someone in a brutal fashion, and we dislike that
	OPINIONEVENT_TEACHER,			// other guy taught us and we are grateful
	OPINIONEVENT_BESTCOMMANDEREVER,	// we won a battle through what must be magnificent leadership
	OPINIONEVENT_BATTLE_SAVIOUR,	// other guy saved us in a firefight

	OPINIONEVENT_FRAGTHIEF,			// other guy stole our kill
	OPINIONEVENT_BATTLE_ASSIST,		// other guy assisted us in battle
	OPINIONEVENT_BATTLE_TOOK_PRISONER,	// other guy chose to take prisoners instead of killing the enemy
	OPINIONEVENT_CIV_ATTACKER,		// other guy attacks a non-hostile civilian

	OPINIONEVENT_MAX
};

typedef struct DynamicOpinionEvent
{
	UINT8 ubEventId;
	INT8	sOpinionModifier;
	BOOLEAN fSameSector;
	BOOLEAN fSameAssignment;
	BOOLEAN fAwake;
	BOOLEAN fSquadsOnly;
} DynamicOpinionEvent;

extern DynamicOpinionEvent gDynamicOpinionEvent[OPINIONEVENT_MAX];

// -------- added by Flugente: flags for opinions --------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
// if someone influences our opinion, STAGE1 gets set. Every day, stages move up one step, until we 'forget' them
// these flags describe what the other guy did to us....
// flagmask 1
#define OPINIONFLAG_STAGE1_FRIENDLYFIRE								0x00000001	//1			// fired on us
#define OPINIONFLAG_STAGE2_FRIENDLYFIRE								0x00000002	//2
#define OPINIONFLAG_STAGE3_FRIENDLYFIRE								0x00000004	//4
#define OPINIONFLAG_STAGE4_FRIENDLYFIRE								0x00000008	//8

#define OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT							0x00000010	//16		// has been snitching to the player about us
#define OPINIONFLAG_STAGE2_SNITCHSOLDMEOUT							0x00000020	//32
#define OPINIONFLAG_STAGE3_SNITCHSOLDMEOUT   						0x00000040	//64
#define OPINIONFLAG_STAGE4_SNITCHSOLDMEOUT							0x00000080	//128

#define OPINIONFLAG_STAGE1_INTERFERENCE								0x00000100	//256		// is interfering with us (stopped us from taking drugs or stealing)
#define OPINIONFLAG_STAGE2_INTERFERENCE								0x00000200	//512
#define OPINIONFLAG_STAGE3_INTERFERENCE								0x00000400	//1024
#define OPINIONFLAG_STAGE4_INTERFERENCE								0x00000800	//2048

#define OPINIONFLAG_STAGE1_FRIENDSWITHHATED							0x00001000	//4096		// is friends with someone we hate
#define OPINIONFLAG_STAGE2_FRIENDSWITHHATED							0x00002000	//8192
#define OPINIONFLAG_STAGE3_FRIENDSWITHHATED							0x00004000	//16384
#define OPINIONFLAG_STAGE4_FRIENDSWITHHATED							0x00008000	//32768

#define OPINIONFLAG_STAGE1_CONTRACTEXTENSION						0x00010000	//65536		// got his contract extended before we did, even though we had less time left
#define OPINIONFLAG_STAGE2_CONTRACTEXTENSION						0x00020000	//131072
#define OPINIONFLAG_STAGE3_CONTRACTEXTENSION						0x00040000	//262144
#define OPINIONFLAG_STAGE4_CONTRACTEXTENSION						0x00080000	//524288

#define OPINIONFLAG_STAGE1_ORDEREDRETREAT							0x00100000	//1048576	// was commander (most senior merc around) and ordered retreat
#define OPINIONFLAG_STAGE2_ORDEREDRETREAT							0x00200000	//2097152
#define OPINIONFLAG_STAGE3_ORDEREDRETREAT							0x00400000	//4194304
#define OPINIONFLAG_STAGE4_ORDEREDRETREAT							0x00800000	//8388608

#define OPINIONFLAG_STAGE1_CIVKILLER								0x01000000	//16777216	// killed an unarmed civilian
#define OPINIONFLAG_STAGE2_CIVKILLER								0x02000000	//33554432
#define OPINIONFLAG_STAGE3_CIVKILLER								0x04000000	//67108864
#define OPINIONFLAG_STAGE4_CIVKILLER								0x08000000	//134217728

#define OPINIONFLAG_STAGE1_SLOWSUSDOWN								0x10000000	//268435456	// is overloaded and thus slows us down
#define OPINIONFLAG_STAGE2_SLOWSUSDOWN								0x20000000	//536870912
#define OPINIONFLAG_STAGE3_SLOWSUSDOWN								0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_SLOWSUSDOWN								0x80000000	//2147483648

// flagmask 2
#define OPINIONFLAG_STAGE1_NOSHARINGFOOD							0x00000001	//1			// had water/food when we needed it. So he did not share!
#define OPINIONFLAG_STAGE2_NOSHARINGFOOD							0x00000002	//2
#define OPINIONFLAG_STAGE3_NOSHARINGFOOD							0x00000004	//4
#define OPINIONFLAG_STAGE4_NOSHARINGFOOD							0x00000008	//8

#define OPINIONFLAG_STAGE1_ANNOYINGDISABILITY						0x00000010	//16		// disability kicked in at a bad time, putting the team at risk (forgeful, nervous, psycho...)
#define OPINIONFLAG_STAGE2_ANNOYINGDISABILITY						0x00000020	//32
#define OPINIONFLAG_STAGE3_ANNOYINGDISABILITY  						0x00000040	//64
#define OPINIONFLAG_STAGE4_ANNOYINGDISABILITY						0x00000080	//128

#define OPINIONFLAG_STAGE1_ADDICT									0x00000100	//256		// is taking drugs 
#define OPINIONFLAG_STAGE2_ADDICT									0x00000200	//512
#define OPINIONFLAG_STAGE3_ADDICT									0x00000400	//1024
#define OPINIONFLAG_STAGE4_ADDICT									0x00000800	//2048

#define OPINIONFLAG_STAGE1_THIEF									0x00001000	//4096		// is stealing items
#define OPINIONFLAG_STAGE2_THIEF									0x00002000	//8192
#define OPINIONFLAG_STAGE3_THIEF									0x00004000	//16384
#define OPINIONFLAG_STAGE4_THIEF									0x00008000	//32768

#define OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER						0x00010000	//65536		// was commander (most senior merc around) when a lot of our people (including militia) died
#define OPINIONFLAG_STAGE2_WORSTCOMMANDEREVER						0x00020000	//131072
#define OPINIONFLAG_STAGE3_WORSTCOMMANDEREVER						0x00040000	//262144
#define OPINIONFLAG_STAGE4_WORSTCOMMANDEREVER						0x00080000	//524288

#define OPINIONFLAG_STAGE1_RICHGUY									0x00100000	//1048576	// gets paid a lot more than we do
#define OPINIONFLAG_STAGE2_RICHGUY									0x00200000	//2097152
#define OPINIONFLAG_STAGE3_RICHGUY									0x00400000	//4194304
#define OPINIONFLAG_STAGE4_RICHGUY									0x00800000	//8388608

#define OPINIONFLAG_STAGE1_BETTERGEAR								0x01000000	//16777216	// has a lot better gear than we do
#define OPINIONFLAG_STAGE2_BETTERGEAR								0x02000000	//33554432
#define OPINIONFLAG_STAGE3_BETTERGEAR								0x04000000	//67108864
#define OPINIONFLAG_STAGE4_BETTERGEAR								0x08000000	//134217728

#define OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS				0x10000000	//268435456	// used our body to rest his gun on
#define OPINIONFLAG_STAGE2_YOUMOUNTEDAGUNONMYBREASTS				0x20000000	//536870912
#define OPINIONFLAG_STAGE3_YOUMOUNTEDAGUNONMYBREASTS				0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_YOUMOUNTEDAGUNONMYBREASTS				0x80000000	//2147483648

// flagmask 3
#define OPINIONFLAG_STAGE1_BANDAGED									0x00000001	//1			// bandaged our wounds
#define OPINIONFLAG_STAGE2_BANDAGED									0x00000002	//2
#define OPINIONFLAG_STAGE3_BANDAGED									0x00000004	//4
#define OPINIONFLAG_STAGE4_BANDAGED									0x00000008	//8

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD						0x00000010	//16		// had a drink with him, he's okay.
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_GOOD						0x00000020	//32
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_GOOD  						0x00000040	//64
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_GOOD						0x00000080	//128

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER						0x00000100	//256		// had a drink with him, he's super okay
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_SUPER						0x00000200	//512
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_SUPER						0x00000400	//1024
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_SUPER						0x00000800	//2048

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD							0x00001000	//4096		// had a drink with him, don't like him
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_BAD							0x00002000	//8192
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_BAD							0x00004000	//16384
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_BAD							0x00008000	//32768

#define OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE						0x00010000	//65536		// had a drink with him, and I REALLY don't like him now
#define OPINIONFLAG_STAGE2_DRINKBUDDIES_WORSE						0x00020000	//131072
#define OPINIONFLAG_STAGE3_DRINKBUDDIES_WORSE						0x00040000	//262144
#define OPINIONFLAG_STAGE4_DRINKBUDDIES_WORSE						0x00080000	//524288

#define OPINIONFLAG_STAGE1_AGAINST_US								0x00100000	//1048576	// was against us in dialogue
#define OPINIONFLAG_STAGE2_AGAINST_US								0x00200000	//2097152
#define OPINIONFLAG_STAGE3_AGAINST_US								0x00400000	//4194304
#define OPINIONFLAG_STAGE4_AGAINST_US								0x00800000	//8388608

#define OPINIONFLAG_STAGE1_FOR_US									0x01000000	//16777216	// other guy was for us in dialogue
#define OPINIONFLAG_STAGE2_FOR_US									0x02000000	//33554432
#define OPINIONFLAG_STAGE3_FOR_US									0x04000000	//67108864
#define OPINIONFLAG_STAGE4_FOR_US									0x08000000	//134217728

#define OPINIONFLAG_STAGE1_AGAINST_ENEMY							0x10000000	//268435456	// was against our dialogue enemy in dialogue
#define OPINIONFLAG_STAGE2_AGAINST_ENEMY							0x20000000	//536870912
#define OPINIONFLAG_STAGE3_AGAINST_ENEMY							0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_AGAINST_ENEMY							0x80000000	//2147483648

// flagmask 4
#define OPINIONFLAG_STAGE1_FOR_ENEMY								0x00000001	//1			// was for our dialogue enemy in dialogue
#define OPINIONFLAG_STAGE2_FOR_ENEMY								0x00000002	//2
#define OPINIONFLAG_STAGE3_FOR_ENEMY								0x00000004	//4
#define OPINIONFLAG_STAGE4_FOR_ENEMY								0x00000008	//8

#define OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_GOOD				0x00000010	//16		// attempted to solve the conflict reasonably, and we thought that was good
#define OPINIONFLAG_STAGE2_SOLVECONFLICT_REASON_GOOD				0x00000020	//32
#define OPINIONFLAG_STAGE3_SOLVECONFLICT_REASON_GOOD  				0x00000040	//64
#define OPINIONFLAG_STAGE4_SOLVECONFLICT_REASON_GOOD				0x00000080	//128

#define OPINIONFLAG_STAGE1_SOLVECONFLICT_REASON_BAD					0x00000100	//256		// attempted to solve the conflict reasonably, and we thought that was bad
#define OPINIONFLAG_STAGE2_SOLVECONFLICT_REASON_BAD					0x00000200	//512
#define OPINIONFLAG_STAGE3_SOLVECONFLICT_REASON_BAD					0x00000400	//1024
#define OPINIONFLAG_STAGE4_SOLVECONFLICT_REASON_BAD					0x00000800	//2048

#define OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_GOOD			0x00001000	//4096		// attempted to solve the conflict agressively, and we thought that was good
#define OPINIONFLAG_STAGE2_SOLVECONFLICT_AGGRESSIVE_GOOD			0x00002000	//8192
#define OPINIONFLAG_STAGE3_SOLVECONFLICT_AGGRESSIVE_GOOD			0x00004000	//16384
#define OPINIONFLAG_STAGE4_SOLVECONFLICT_AGGRESSIVE_GOOD			0x00008000	//32768

#define OPINIONFLAG_STAGE1_SOLVECONFLICT_AGGRESSIVE_BAD				0x00010000	//65536		// attempted to solve the conflict agressively, and we thought that was bad
#define OPINIONFLAG_STAGE2_SOLVECONFLICT_AGGRESSIVE_BAD				0x00020000	//131072
#define OPINIONFLAG_STAGE3_SOLVECONFLICT_AGGRESSIVE_BAD				0x00040000	//262144
#define OPINIONFLAG_STAGE4_SOLVECONFLICT_AGGRESSIVE_BAD				0x00080000	//524288

#define OPINIONFLAG_STAGE1_DISEASE_DISGUSTING						0x00100000	//1048576	// is diseased, which we find disgusting
#define OPINIONFLAG_STAGE2_DISEASE_DISGUSTING						0x00200000	//2097152
#define OPINIONFLAG_STAGE3_DISEASE_DISGUSTING						0x00400000	//4194304
#define OPINIONFLAG_STAGE4_DISEASE_DISGUSTING						0x00800000	//8388608

#define OPINIONFLAG_STAGE1_DISEASE_TREATMENT						0x01000000	//16777216	// treated our disease, which we like
#define OPINIONFLAG_STAGE2_DISEASE_TREATMENT						0x02000000	//33554432
#define OPINIONFLAG_STAGE3_DISEASE_TREATMENT						0x04000000	//67108864
#define OPINIONFLAG_STAGE4_DISEASE_TREATMENT						0x08000000	//134217728

#define OPINIONFLAG_STAGE1_BRUTAL_GOOD								0x10000000	//268435456	// other guy killed someone in a brutal fashion, and we like that
#define OPINIONFLAG_STAGE2_BRUTAL_GOOD								0x20000000	//536870912
#define OPINIONFLAG_STAGE3_BRUTAL_GOOD								0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_BRUTAL_GOOD								0x80000000	//2147483648

// flagmask 5
#define OPINIONFLAG_STAGE1_BRUTAL_BAD								0x00000001	//1			// other guy killed someone in a brutal fashion, and we dislike that
#define OPINIONFLAG_STAGE2_BRUTAL_BAD								0x00000002	//2
#define OPINIONFLAG_STAGE3_BRUTAL_BAD								0x00000004	//4
#define OPINIONFLAG_STAGE4_BRUTAL_BAD								0x00000008	//8

#define OPINIONFLAG_STAGE1_TEACHER									0x00000010	//16		// other guy taught us and we are grateful
#define OPINIONFLAG_STAGE2_TEACHER									0x00000020	//32
#define OPINIONFLAG_STAGE3_TEACHER  								0x00000040	//64
#define OPINIONFLAG_STAGE4_TEACHER									0x00000080	//128

#define OPINIONFLAG_STAGE1_BESTCOMMANDEREVER						0x00000100	//256		// this guy lead our team to a magnificent victory
#define OPINIONFLAG_STAGE2_BESTCOMMANDEREVER						0x00000200	//512
#define OPINIONFLAG_STAGE3_BESTCOMMANDEREVER						0x00000400	//1024
#define OPINIONFLAG_STAGE4_BESTCOMMANDEREVER						0x00000800	//2048

#define OPINIONFLAG_STAGE1_BATTLE_SAVIOUR							0x00001000	//4096		// other guy saved us in a firefight
#define OPINIONFLAG_STAGE2_BATTLE_SAVIOUR							0x00002000	//8192
#define OPINIONFLAG_STAGE3_BATTLE_SAVIOUR							0x00004000	//16384
#define OPINIONFLAG_STAGE4_BATTLE_SAVIOUR							0x00008000	//32768

#define OPINIONFLAG_STAGE1_FRAGTHIEF								0x00010000	//65536		// other guy stole our kill
#define OPINIONFLAG_STAGE2_FRAGTHIEF								0x00020000	//131072
#define OPINIONFLAG_STAGE3_FRAGTHIEF								0x00040000	//262144
#define OPINIONFLAG_STAGE4_FRAGTHIEF								0x00080000	//524288

#define OPINIONFLAG_STAGE1_BATTLE_ASSIST							0x00100000	//1048576	// other guy assisted us in battle
#define OPINIONFLAG_STAGE2_BATTLE_ASSIST							0x00200000	//2097152
#define OPINIONFLAG_STAGE3_BATTLE_ASSIST							0x00400000	//4194304
#define OPINIONFLAG_STAGE4_BATTLE_ASSIST							0x00800000	//8388608

#define OPINIONFLAG_STAGE1_BATTLE_TOOK_PRISONER						0x01000000	//16777216	// other guy chose to take prisoners instead of killing the enemy
#define OPINIONFLAG_STAGE2_BATTLE_TOOK_PRISONER						0x02000000	//33554432
#define OPINIONFLAG_STAGE3_BATTLE_TOOK_PRISONER						0x04000000	//67108864
#define OPINIONFLAG_STAGE4_BATTLE_TOOK_PRISONER						0x08000000	//134217728

#define OPINIONFLAG_STAGE1_CIV_ATTACKER								0x10000000	//268435456	// other guy attacks a non-hostile civilian
#define OPINIONFLAG_STAGE2_CIV_ATTACKER								0x20000000	//536870912
#define OPINIONFLAG_STAGE3_CIV_ATTACKER								0x40000000	//1073741824
#define OPINIONFLAG_STAGE4_CIV_ATTACKER								0x80000000	//2147483648

#define OPINIONFLAG_STAGE1_ALL										0x11111111				// flags of all first stages, used when rolling over
#define OPINIONFLAG_STAGE4_ALL										0x88888888				// flags of all final stages, used when rolling over
// ----------------------------------------------------------------


// we have several 'roles' in a dialogue for now:
// victim - a merc initiating dialogue because an event has happened to him (can also be a positive thing)
// cause - the merc that caused the event through his behaviour
// interjector - an person that is neither victim nor cause and attemtps to solve the situation, the player can control his/her actions
// speaker - whoever speaks the current line, for no one of the above
typedef enum
{
	// opinion event has occured and a merc points that out to ...
	DOST_VICTIM_INITIATE,							// victim begins dialogue

	DOST_VICTIM_TO_INTERJECTOR_DENY,				// victim denies what interjector said
	DOST_VICTIM_TO_INTERJECTOR_AGREE,				// victim agrees what interjector said

	DOST_CAUSE_TO_VICTIM_DENY,						// cause denies victim's accusation
	DOST_CAUSE_TO_VICTIM_AGREE,						// cause accepts responsibility to victim

	DOST_CAUSE_TO_INTERJECTOR_DENY,					// cause denies what interjector said
	DOST_CAUSE_TO_INTERJECTOR_AGREE,				// cause agrees what interjector said

	DOST_INTERJECTOR_DIALOGUESELECTION,				// special: creates the interjector dialogue selection

	DOST_INTERJECTOR_TO_VICTIM_DENY,				// interjector denies victim's accusation
	DOST_INTERJECTOR_TO_VICTIM_AGREE,				// interjectorinterjector agrees to victim that cause is to blame

	DOST_INTERJECTOR_TO_CAUSE_DENY,					// interjector denies victim denial
	DOST_INTERJECTOR_TO_CAUSE_AGREE,				// interjector agrees that victim is not to blame

	DOST_INTERJECTOR_SOLVE_REASON,					// interjector tries to solve the situation peacefully
	DOST_INTERJECTOR_SOLVE_AGGRESSIVE,				// interjector tries to solve the situation aggressively

	DOST_INTERJECTOR_NOTHING,						// interjector decides to say nothing

	DOST_SIDEWITH_VICTIM,							// another person sides with victim
	DOST_SIDEWITH_CAUSE,							// another person sides with cause

	DOST_MAX,

} DynamicOpinionSpeechType;

// the different choices we can take when selecting what approach to use in dialogue
enum
{
	DOST_CHOICE_NOTHING = 0,
	DOST_CHOICE_SIDEWITH_VICTIM,
	DOST_CHOICE_SIDEWITH_CAUSE,
	DOST_CHOICE_REASON,
	DOST_CHOICE_AGGRESSIVE,

	DOST_CHOICE_MAX
};

enum
{
	DOST_POSITION_LEFT,
	DOST_POSITION_RIGHT,
	DOST_POSITION_MIDDLE,
};

enum
{
	DOST_PRIORITY_LOW,		// can be handled hours later if needed
	DOST_PRIORITY_NORMAL,	// normal priority
	DOST_PRIORITY_HIGH,		// urgent, handle as soon as possible

	DOST_PRIORITY_MAX
};

typedef struct OpinionEvent
{
	OpinionEvent( ) :
	ubEventId( 0 ),
	ubProfileComplainant( NO_PROFILE ),
	ubProfileCause( NO_PROFILE ),
	usSector( 0 )
	{}

	UINT8 ubEventId;				// the dynamic opinion event this is all about
	UINT8 ubProfileComplainant;		// the merc who started this dialogue by complaining
	UINT8 ubProfileCause;			// the merc who caused the event
	UINT8 usSector;					// sector of incident
} OpinionEvent;

#define DOST_INTERJECTORS_MAX	4

typedef struct DynamicOpinionSpeechEvent_EventData
{
	DynamicOpinionSpeechEvent_EventData( ) :
	event( ),
	usId( 0 ),
	usPriority( 0 ),
	ubProfileINTERJECTOR( NO_PROFILE )
	{}

	OpinionEvent event;				// the event that this is all about

	UINT16 usId;					// id that signifies wether dialogue belongs to this event or another one
	UINT8 usPriority;				// priority of event, used when determining which event will be handled next

	// other mercs an interfere in the dialogue
	UINT8 ubProfileINTERJECTOR;		// if an INTERJECTOR tries to solve the situation, it'll be this guy

	// several other mercs can agree with either cause or complainant. Most of the time, these guys will have similar events at the time, which will simply be merged
	// the goal is to have less dialogue events while making them somewhat deeper
	UINT8 ubProfileSideCause[DOST_INTERJECTORS_MAX];
	UINT8 ubProfileSideComplainant[DOST_INTERJECTORS_MAX];

} DynamicOpinionSpeechEvent_EventData;

typedef struct DynamicOpinionSpeechEvent
{
	DynamicOpinionSpeechEvent( ) :
	data( ),
	usStarttime( 0 ),
	ubEventType( DOST_INTERJECTOR_NOTHING ),
	usQueueNumber( 0 ),
	usSpeaker( NO_PROFILE ),
	usSide( DOST_POSITION_LEFT ),
	usNumonside( 0 )
	{}

	DynamicOpinionSpeechEvent_EventData data;

	UINT32 usStarttime;				// time this dialogue should be displayed

	UINT8 ubEventType;				// kind of speech event (from DynamicOpinionSpeechType)
	UINT8 usQueueNumber;			// number of this dialogue bit in the event's queue
	UINT8 usSpeaker;				// the merc who says this
	UINT8 usSide;					// side with ubProfileComplainant (DOST_POSITION_LEFT), ubProfileCause (DOST_POSITION_RIGHT) or stay neutral (DOST_POSITION_MIDDLE)

	UINT8 usNumonside;				// this argument is the x'th on this side
	UINT8 usFiller[3];				// to appease the petty gods of padding AGAIN
} DynamicOpinionSpeechEvent;



void CleanDynamicOpinionSpeechEvents( );
void AddDynamicOpinionEvent_New( OpinionEvent aEvent );
void AddDynamicOpinionEvent_Continue( DynamicOpinionSpeechEvent aEvent );
void CreateSpeechEventsFromDynamicOpinionEvent( DynamicOpinionSpeechEvent aEvent );
void HandleDynamicOpinionSpeechEvents( );
void StartDynamicOpinionDialogue( );
void PerformPossibleDynamicOpinionEventMerges( );

BOOLEAN DynamicOpinionTacticalCharacterDialogue( DynamicOpinionSpeechEvent& aEvent );

INT8 GetSidePosition( UINT8 aDostPosition );

BOOLEAN DynamicOpinionSpeechInProgress( );
void SetfDynamicOpinionSpeechInProgress( BOOLEAN aVal );

BOOLEAN SaveDynamicDialogue( HWFILE hwFile );
BOOLEAN LoadDynamicDialogue( HWFILE hwFile );


//  modify usProfileA's opinion of usProfileB because of usEvent
void AddOpinionEvent( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent, BOOLEAN fStartDialogue = TRUE );

// get usProfileA's opinion of usProfileB on one of the last 4 days
INT8 GetDynamicOpinionDay( UINT8 usProfileA, UINT8 usProfileB, UINT8 usDay );

// get usProfileA's opinion of usProfileB concerning usEvent
INT8 GetDynamicOpinion( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent );

// daily rollover of opinions
void HandleDynamicOpinionsDailyRefresh( );

// a day has passed, 'age' opinions 
void RolloverDynamicOpinions( UINT8 usProfileA );

// check wether other people are friends with someone else we hate. All persons must be in Arulco
void CheckForFriendsofHated( SOLDIERTYPE* pSoldier );

void HandleDynamicOpinionOnContractExtension( UINT8 ubCode, UINT8 usProfile );

// depending on what happened in the battle, our mercs might complain or celebrate
void HandleDynamicOpinionBattleFinished( BOOLEAN fBattleWon );
void HandleDynamicOpinionRetreat( );
void HandleDynamicOpinionTeamDrinking( SOLDIERTYPE* pSoldier );
void HandleDynamicOpinionTeaching( SOLDIERTYPE* pSoldier, UINT8 ubStat );

// some events require a 'leader' -  merc the team will regard as the one being in charge, and subsequently being praised or damned for the way things develop
UINT32 GetSoldierLeaderRating( SOLDIERTYPE* pSoldier );

UINT16 GetBestMercLeaderInSector( INT16 sX, INT16 sY, INT8 sZ );

// get id of a random merc in a sector, provided one exists
UINT8 GetRandomMercInSectorNotInList( INT16 sX, INT16 sY, INT8 sZ, std::vector<UINT8> aTaboo, BOOLEAN fImpOnly );

UINT8 GetFittingInterjectorProfile( UINT8 usEvent, UINT8 usProfileVictim, UINT8 usProfileCause );

UINT8 HighestInventoryCoolness( SOLDIERTYPE* pSoldier );

void HandleDynamicOpinionChange( SOLDIERTYPE* pSoldier, UINT8 usEvent, BOOLEAN fOffender, BOOLEAN fStartDialogue );

#endif
