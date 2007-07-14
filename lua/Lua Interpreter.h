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
int EvalLua (const wchar_t* buff) ; // The return value is whether to clear the input line, not whether the call succeeded.
void ShutdownLua( );

