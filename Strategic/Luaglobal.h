#ifndef _LUAGLOBAL_H
#define _LUAGLOBAL_H

#include "Types.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// --------------------------
// WANNE: Should we enable lua scripting for different actions, or should we use "hardcoded" stuff instead?

#define LUA_QUESTS							// Quests.lua
#define LUA_INTERFACE_DIALOGUE				// InterfaceDialogue.lua
#define LUA_EXPLOSION_CONTROL				// ExplosionControl.lua
#define LUA_HANDLE_QUEST_CODE_ON_SECTOR		// Strategicmap.lua
#define LUA_GAME_INIT_NEW_GAME				// GameInit.lua	(Function: InitNewGame)
#define LUA_GAME_INIT_NPCS					// GameInit.lua (Function: InitNPCs)
#define LUA_INTRO							// Intro.lua
#define LUA_HOURLY_QUEST_UPDATE				// HourlyUpdate.lua
#define LUA_STRATEGY_EVENT_HANDLER			// StrategicEventHandler.lua
#define LUA_STRATEGY_TOWN_LOYALTY			// StrategicTownLoyalty.lua
#define LUA_OVERHEAD						// Overhead.lua
// --------------------------

void  IniGlobalGameSetting(lua_State *L);

#endif
