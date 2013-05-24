#ifndef _GAME_VERSION_H_
#define _GAME_VERSION_H_


#ifdef __cplusplus
extern "C" {
#endif


//
//	Keeps track of the game version
//

extern	CHAR16		zVersionLabel[256];
extern	CHAR8		czVersionNumber[16];
extern	CHAR16		zTrackingNumber[16];


//ADB:	I needed these here so I moved them, and why put them in *.cpp anyways?
//
//		Keeps track of the saved game version.	Increment the saved game version whenever 
//	you will invalidate the saved game file

#define			NO_VEHICLE_SAVE									144 // Flugente: vehicle data isn't saved anymore, as it has long been externalised
#define			SOLDIER_PROFILES								143	// Flugente: profiles for enemy and militia (possibly any non-NPC/RPC character)
#define			DYNAMIC_FLASHLIGHTS								142	// Flugente: had to add variables to LIGHTEFFECT-struct
#define			ENLARGED_OPINIONS								141	// Flugente: increased numberof merc opinions to 255, thus need to change savegame reading
#define			MODULARIZED_AI									140	// Flugente: new savegame version due to modularized AI
#define			MILITIA_EQUIPMENT								139	// Flugente: militia can equip from sector inventory
#define			MULTITURN_ACTIONS								138	// Flugente: multiturn actions
#define			EXTERNALISED_MERCHANTS							137	// Flugente: increased the number of merchants and externalised their data
#define			PRISON_SYSTEM									136	// Flugente: added var to the sector structure. Shouldn't break compatibility, but let's be safe
#define			DUAL_BURST_ADDED								135
#define			MORE_ROOMS										134 // changed room numbers from 8 to 16 bit by DBrot
#define			FOOD_CHANGES									133	// changes for food system
#define			ZOMBIE_CHANGES									132	// Included some changes for Zombies
#define			JA25_UB_INTEGRATION								131	// Before this, the JA25 Unfinished Business code was not integrated (see "Builddefines.h" JA2UB)
#define			IMPROVED_INTERRUPT_SYSTEM						130	
#define			ENCYCLOPEDIA_SAVEGAME_CHANGE					129	// Added encyclopedia
#define			HIDDENTOWN_DATATYPE_CHANGE						128
#define			NEW_EMAIL_SAVE_GAME								127
#define			NEW_SAVE_GAME_GENERAL_SAVE_INFO_DATA			126
#define			QUESTS_DATATYPE_CHANGE							125	// Before this, we had a maximum number of 30 quests (JA2 default) (by Jazz)
#define			VEHICLES_DATATYPE_CHANGE						124	// Before this, vehicles were not externalized (by Jazz)
#define			FIXED_MERC_NOT_AVAILABLE_ON_MERC_WEBSITE		123	// Before this, only Bubba was available on MERC website
#define			FIXED_CREPITUS_IN_REALISTIC_GAME_MODE			122	// Before this, there was an initializing problem in GameInit.lua on startup, in which we could meet crepitus in Tixa underground map
#define			FIXED_NPC_QUOTE_BUG								121	// Before this, we need to reload all npc quote information because it was not being saved and loaded correctly.
#define			MOVED_GENERAL_INFO								120	// Before this, general game information was saved much later in the save game process.
#define			NCTH_AUTOFIRE_UPDATE							119	// Before this, we didn't track the original autofire offsets
#define			BR_EMAIL_DATA_CHANGE							118	// Before this, BR emails caused CTD
#define			NCTH_DATATYPE_CHANGE							117	// Before this, we did not have HAM 4.0 NCTH
#define			NEW_GENERAL_SAVE_INFO_DATA						116	// Before this, we did not have HAM 4.0 general info
#define			IMP_TRAIT_EXPERT_ITEMS_CHANGE					115	// Before this, we did not have additional expert items defined in IMPItemChoices.xml
#define			LUA_SAVEGAME_CHANGE								114	// Before this, we did not have additional lua script files (e.g: Intro.lua, ...)
#define			STOMP12_SAVEGAME_DATATYPE_CHANGE2				113	// Before this, STOMP did not allow to have up to 30 traits
#define			NAS_SAVEGAME_DATATYPE_CHANGE					112	// Before this, NAS settings (ON/OFF) was not saved to the savegame
#define			STOMP12_SAVEGAME_DATATYPE_CHANGE				111	// Before this, STOMP v1.2 was not included in the source
#define			BUGFIX_NPC_DATA_FOR_BIG_MAPS					110 // Before this, we had a bug in the NPC Data for Big Maps (this fix was introduced in the Closed Beta)
#define			STRATEGIC_MINES_DATATYPE_CHANGE					109 // Before this, we did not have strategic mines initialization via lua script
#define			HIDE_NPC_NAMES_FOR_UNKNOWN_PERSONS				108	// Before this, we did not have hiding of npc names for unknown persons included (by jazz)
#define			BIG_MAPS_PROJECT_SAVEGAME_DATATYPE_CHANGE		107	// Before this, we did not have big maps project 
#define			AIRPORT_SAVEGAME_DATATYPE_CHANGE				106	// Before this, we did not have airport externalized
#define			AP100_SAVEGAME_DATATYPE_CHANGE					105	// Before this, we didn't have the 100AP structure changes
#define			NIV_SAVEGAME_DATATYPE_CHANGE					102	// Before this, we used the old structure system

#define			SAVE_GAME_VERSION								NO_VEHICLE_SAVE

//#define RUSSIANGOLD
#ifdef __cplusplus
}
#endif


#endif
