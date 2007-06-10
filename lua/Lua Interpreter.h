extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Types.h"

typedef struct {
	STR8 name;
	lua_CFunction get;
	lua_CFunction set;
	lua_CFunction call;
} LuaAttrib;

void InitializeLua( );
void EvalLua (STR8 buff) ;
void ShutdownLua( );

