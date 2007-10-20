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
	lua_CFunction staticcall;
} LuaAttrib;

void InitializeLua( );
int EvalLua (const wchar_t* buff) ; // The return value is whether to clear the input line, not whether the call succeeded.
void ShutdownLua( );

extern luaL_Reg WStringMethods[];
extern LuaAttrib Environment[];
extern LuaAttrib Soldier[];
extern LuaAttrib Sector[];

void CreateLuaClass( lua_State *L, STR8 ClassName, LuaAttrib *Attribs );
void NewLuaObject( lua_State *L, STR8 ClsName, void *Ptr );

void LuaStrategicSetup(lua_State *L);
void LuaTacticalSetup(lua_State *L);
void LuaEnvironmentSetup(lua_State *L);
