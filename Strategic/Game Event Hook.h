#ifndef __GAME_EVENT_HOOK_H
#define __GAME_EVENT_HOOK_H

#include "types.h"

enum
{
	EVENT_CHANGELIGHTVAL = 1,
	EVENT_WEATHERSTART,
	EVENT_WEATHEREND,
	EVENT_CHECKFORQUESTS,
	EVENT_AMBIENT,
	EVENT_AIM_RESET_MERC_ANNOYANCE,
	EVENT_BOBBYRAY_PURCHASE,	// WANNE: Unused (old Bobby Ray Shipment EVENT)
	EVENT_DAILY_UPDATE_BOBBY_RAY_INVENTORY,
	EVENT_UPDATE_BOBBY_RAY_INVENTORY,	
	EVENT_DAILY_UPDATE_OF_MERC_SITE,	
	EVENT_DAY3_ADD_EMAIL_FROM_SPECK,
	EVENT_DELAYED_HIRING_OF_MERC,
	EVENT_HANDLE_INSURED_MERCS,
	EVENT_PAY_LIFE_INSURANCE_FOR_DEAD_MERC,
	EVENT_MERC_DAILY_UPDATE,
	EVENT_MERC_ABOUT_TO_LEAVE_COMMENT,
	EVENT_MERC_CONTRACT_OVER,
	EVENT_GROUP_ARRIVAL,
	EVENT_DAY2_ADD_EMAIL_FROM_IMP,
	EVENT_MERC_COMPLAIN_EQUIPMENT,
	EVENT_HOURLY_UPDATE,
	EVENT_HANDLE_MINE_INCOME,
	EVENT_SETUP_MINE_INCOME,
	EVENT_QUEUED_BATTLE,
	EVENT_LEAVING_MERC_ARRIVE_IN_DRASSEN,	// unused
	EVENT_LEAVING_MERC_ARRIVE_IN_OMERTA,	// unused
	EVENT_SET_BY_NPC_SYSTEM,
	EVENT_SECOND_AIRPORT_ATTENDANT_ARRIVED,
	EVENT_HELICOPTER_HOVER_TOO_LONG,
	EVENT_HELICOPTER_HOVER_WAY_TOO_LONG,
	EVENT_HELICOPTER_DONE_REFUELING,
	EVENT_MERC_LEAVE_EQUIP_IN_OMERTA,
	EVENT_MERC_LEAVE_EQUIP_IN_DRASSEN,
	EVENT_DAILY_EARLY_MORNING_EVENTS,
	EVENT_GROUP_ABOUT_TO_ARRIVE,
	EVENT_PROCESS_TACTICAL_SCHEDULE,
	EVENT_BEGINRAINSTORM,
	EVENT_ENDRAINSTORM,
	EVENT_HANDLE_TOWN_OPINION,
	EVENT_SETUP_TOWN_OPINION,
	EVENT_MAKE_CIV_GROUP_HOSTILE_ON_NEXT_SECTOR_ENTRANCE,
	EVENT_BEGIN_AIR_RAID,
	EVENT_TOWN_LOYALTY_UPDATE,		/* Delayed loyalty effects elimininated.	Sep.12/98.	ARM */
	EVENT_MEANWHILE,
	EVENT_BEGIN_CREATURE_QUEST,
	EVENT_CREATURE_SPREAD,
	EVENT_DECAY_CREATURES,
	EVENT_CREATURE_NIGHT_PLANNING,
	EVENT_CREATURE_ATTACK,
	EVENT_EVALUATE_QUEEN_SITUATION,
	EVENT_CHECK_ENEMY_CONTROLLED_SECTOR,
	EVENT_TURN_ON_NIGHT_LIGHTS,
	EVENT_TURN_OFF_NIGHT_LIGHTS,
	EVENT_TURN_ON_PRIME_LIGHTS,
	EVENT_TURN_OFF_PRIME_LIGHTS,
	EVENT_MERC_ABOUT_TO_LEAVE,
	EVENT_INTERRUPT_TIME,
	EVENT_ENRICO_MAIL,
	EVENT_INSURANCE_INVESTIGATION_STARTED,
	EVENT_INSURANCE_INVESTIGATION_OVER,
	EVENT_MINUTE_UPDATE,
	EVENT_TEMPERATURE_UPDATE,
	EVENT_KEITH_GOING_OUT_OF_BUSINESS,
	EVENT_MERC_SITE_BACK_ONLINE,
	EVENT_INVESTIGATE_SECTOR,
	EVENT_CHECK_IF_MINE_CLEARED,
	EVENT_REMOVE_ASSASSIN,
	EVENT_BANDAGE_BLEEDING_MERCS,
	EVENT_SHOW_UPDATE_MENU,
	EVENT_SET_MENU_REASON,
	EVENT_ADDSOLDIER_TO_UPDATE_BOX,
	EVENT_BEGIN_CONTRACT_RENEWAL_SEQUENCE,
	EVENT_RPC_WHINE_ABOUT_PAY,
	EVENT_HAVENT_MADE_IMP_CHARACTER_EMAIL,
	EVENT_RAINSTORM,
	EVENT_QUARTER_HOUR_UPDATE,
	EVENT_MERC_MERC_WENT_UP_LEVEL_EMAIL_DELAY,
	EVENT_MERC_SITE_NEW_MERC_AVAILABLE,
	EVENT_POSTAL_SERVICE_SHIPMENT,
//	EVENT_CHECK_FOR_AIR_RAID,
#ifdef JA2UB
	//Ja25:
	EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE,
	EVENT_SAY_DELAYED_MERC_QUOTE,

	EVENT_DELAY_SOMEONE_IN_SECTOR_MSGBOX,
	EVENT_SECTOR_H8_DONT_WAIT_IN_SECTOR,
	EVENT_SEND_ENRICO_UNDERSTANDING_EMAIL,
#endif
#ifdef CRIPPLED_VERSION
	EVENT_CRIPPLED_VERSION_END_GAME_CHECK,
#endif
	/*
	HEY, YOU GUYS AREN'T DOING THIS!!!!!!	(see below)



	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!! IMPORTANT NOTE																		 !!
	!! FOR ALL NEW EVENTS:	For text debug support, make sure you add the text version of the !!
	!! new event into the gEventName[] at the top of Game Events.c.							!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/
	
	EVENT_HELICOPTER_HOVER_FOR_A_MINUTE,
	EVENT_HELICOPTER_REFUEL_FOR_A_MINUTE,

	EVENT_MILITIA_MOVEMENT_ORDER,				// Flugente: militia was ordered to move, at this time they should arrive
	EVENT_PMC_EMAIL,							// Flugente: pmc will send us an introduction email
	EVENT_PMC_REINFORCEMENT_ARRIVAL,			// Flugente: pmc hired militia arrives

	EVENT_KINGPIN_BOUNTY_INITIAL,				// Flugente: a new quest by Kingpin: kill the DaSilvas while fighting off the competition
	EVENT_KINGPIN_BOUNTY_END_KILLEDTHEM,
	EVENT_KINGPIN_BOUNTY_END_TIME_PASSED,

	EVENT_ASD_UPDATE,							// Flugente: the queen's special division decides it's next action
	EVENT_ASD_PURCHASE_FUEL,
	EVENT_ASD_PURCHASE_JEEP,
	EVENT_ASD_PURCHASE_TANK,
	EVENT_ASD_PURCHASE_HELI,
	EVENT_ASD_PURCHASE_ROBOT,

	EVENT_ENEMY_HELI_UPDATE,					// Flugente: move enemy helicopter to another sector
	EVENT_ENEMY_HELI_REPAIR,
	EVENT_ENEMY_HELI_REFUEL,

	EVENT_SAMSITE_REPAIRED,						// Flugente: have a SAM site be fully repaired

	EVENT_MILITIAROSTER_EMAIL,					// Flugente: we get an email giving us the bookmark to the militia roster webpage
	
	EVENT_WEATHER_NORMAL,						// Flugente: localized weather
	EVENT_WEATHER_SHOWERS,
	EVENT_WEATHER_THUNDERSHOWERS,
	EVENT_WEATHER_SANDSTORM,
	EVENT_WEATHER_SNOW,

	EVENT_INTEL_ENRICO_EMAIL,					// Flugente: Enrico sends us an email introduction to intel
	EVENT_INTEL_PHOTOFACT_VERIFY,				// verification of a photo fact

	EVENT_DAILY_RAID_EVENTS,					// Flugente: daily planning of raids on player sectors
	EVENT_BLOODCAT_ATTACK,						// Flugente: like a creature attack, but with cats
	EVENT_ZOMBIE_ATTACK,						// Flugente: like a creature attack, but with zombies
	EVENT_BANDIT_ATTACK,						// Flugente: like a creature attack, but with bandits
	EVENT_ARMY_FINISH_TRAINING,					// Another bunch of Queen Army recruits is about to finish training

	EVENT_MINIEVENT,

	NUMBER_OF_EVENT_TYPES_PLUS_ONE,
	NUMBER_OF_EVENT_TYPES						= NUMBER_OF_EVENT_TYPES_PLUS_ONE - 1
};

// This value is added to the param value for NPC-system-created events which are based on an
// action rather than a fact:
#define NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS 10000

//One Time Events
//These events are scheduled to happen, and when their time is up, they are processed 
//once, and deleted.	The first prototype requires the actual world time in minutes 
//included the current day, and the additional ones are wrappers which calculate the 
//actual world time for you.
BOOLEAN AddStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam );
BOOLEAN AddSameDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam );
BOOLEAN AddFutureDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam, UINT32 uiNumDaysFromPresent );
//Same but uses seconds instead of minutes.
BOOLEAN AddStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam );
BOOLEAN AddSameDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam );
BOOLEAN AddFutureDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam, UINT32 uiNumDaysFromPresent );

//Ranged Events
//In certain cases, you may wish to create an event that has a start time and an end 
//time.	If this is used, then the execution of the event will happen once at the 
//beginning of the event and once at the expiration time of the event.	A flag, 
//pEvent->fEndEvent is set for the second call.	It is up to you process it 
//separately.	An example use of ranged events would be for ambient sounds.	The 
//start time would be called to define the sounds for use within the game.	The end 
//time would be used to remove the sound from the system.
BOOLEAN AddRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam );
BOOLEAN AddSameDayRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam);
BOOLEAN AddFutureDayRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam, UINT32 uiNumDaysFromPresent );
BOOLEAN AddRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam );
BOOLEAN AddSameDayRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam);
BOOLEAN AddFutureDayRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam, UINT32 uiNumDaysFromPresent );

//EveryDay Events
//A second special case event, this event will get processed automatically at the 
//same time every day.	Time of day lighting is a perfect example.	Other events 
//such as checking for quests, can also be automated using this system.	NOTE:	
//Only specify the time of day in minutes (don't add the actual day to this value)
BOOLEAN AddEveryDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiParam );
BOOLEAN AddEveryDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiParam );

//Periodic Events
//Event will get processed automatically once every X minutes.	The period is added to the current time for
//the time stamp.	
BOOLEAN AddPeriodStrategicEvent( UINT8 ubCallbackID, UINT32 uiOnceEveryXMinutes, UINT32 uiParam );
BOOLEAN AddPeriodStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiOnceEveryXSeconds, UINT32 uiParam );
//Hybrids that aren't based from the current time.	They are offsetted from the current time first.
BOOLEAN AddPeriodStrategicEventWithOffset( UINT8 ubCallbackID, UINT32 uiOnceEveryXMinutes, UINT32 uiOffsetFromCurrent, UINT32 uiParam );
BOOLEAN AddPeriodStrategicEventUsingSecondsWithOffset( UINT8 ubCallbackID, UINT32 uiOnceEveryXSeconds, UINT32 uiOffsetFromCurrent, UINT32 uiParam );

//Searches for and removes the first event matching the supplied information.	There may very well be a need
//for more specific event removal, so let me know (Kris), of any support needs.	Function returns FALSE if
//no events were found.
BOOLEAN DeleteStrategicEvent( UINT8 ubCallbackID, UINT32 uiParam );
void DeleteAllStrategicEventsOfType( UINT8 ubCallbackID );
void DeleteAllStrategicEvents();

// Flugente: return vector of all events of type ubCallbackID with time and param
std::vector< std::pair<UINT32, UINT32> > GetAllStrategicEventsOfType( UINT8 ubCallbackID );

#endif