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
#define			SAVE_GAME_VERSION								117		//114	//113	//112	//111	//110	//109	//108	//107	//106	//105	//104	//103	//102	//101	//100 // 99

#define			CURRENT_SAVEGAME_DATATYPE_VERSION				116

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


//#define RUSSIANGOLD
#ifdef __cplusplus
}
#endif


#endif
