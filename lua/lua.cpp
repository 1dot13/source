#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Lua Interpreter.h"
#include <Windows.h>
#include "MemMan.h"

lua_State *L;

#define ARRAY_INDEX " idx"	// The space is intentional to make this an out-of-band field

void CreateLuaType( lua_State *L, STR8 TypeName, luaL_Reg *LuaAccessors )
{
	luaL_Reg *idx;

	luaL_newmetatable( L, TypeName);
	for (idx = LuaAccessors; idx->name; idx++)
	{
		lua_pushstring( L, idx->name);
		lua_pushcfunction( L, idx->func);
		lua_rawset( L, -3 );
	}
}





static LuaAttrib* LuaCommonGetSet( lua_State *L )
{
	const CHAR8 *AttrToFind;
	LuaAttrib *Attributes;

	Attributes = (LuaAttrib*) lua_touserdata( L, lua_upvalueindex( 1));

	if (lua_isnumber( L, 2) )
	{
		AttrToFind = ARRAY_INDEX;
	}
	else if (lua_isstring( L, 2) )
	{
		AttrToFind = lua_tostring( L, 2);
	}
	else
	{
		luaL_argcheck( L, 0, 2, "Invalid index to the table" );
		// The last function doesn't return, but gotta keep the compiler happy
		return NULL;
	}

	for ( ; Attributes->name; Attributes++ )
	{
		if (!strcmp( Attributes->name, AttrToFind) )
		{
			return Attributes;
		}
	}

	return NULL;
}

static int LuaGetObject( lua_State *L)
{
	LuaAttrib *Attributes;

	Attributes = LuaCommonGetSet( L);
	if (!Attributes)
	{
		lua_pushnil( L );
		return 1;
	}

	if (!Attributes->get)
	{
		if (!Attributes->call)
		{
			lua_pushnil( L );
		}
		else
		{
			lua_pushcfunction( L, Attributes->call);
		}
		return 1;
	}

	return Attributes->get( L );
}

static int LuaSetObject( lua_State *L)
{
	LuaAttrib *Attributes;

	Attributes = LuaCommonGetSet( L);

	if (!Attributes)
	{
		lua_rawset( L, 1 );
		return 0;
	}

	if (!Attributes->set)
	{
		luaL_error( L, "The attribute is not writable" );
	}

	return Attributes->set( L);
}


luaL_Reg ClassList[] = {
	{ "__index", LuaGetObject, },
	{ "__newindex", LuaSetObject, },
	{ NULL, },
};

void CreateLuaClass( lua_State *L, STR8 ClassName, LuaAttrib *Attribs )
{
	luaL_Reg *i;
	LuaAttrib *idx;

	luaL_newmetatable( L, ClassName );
	for (i = ClassList; i->name; i++)
	{
		lua_pushstring( L, i->name);
		lua_pushlightuserdata( L, Attribs);
		lua_pushcclosure( L, i->func, 1);
		lua_rawset( L, -3);
	}

	for (idx=Attribs; idx->name; idx++)
	{
		if (idx->staticcall)
		{
			lua_pushstring( L, idx->name);
			lua_pushcfunction( L, idx->staticcall);
			lua_rawset( L, -3 );
		}
	}
}







void CreateLuaObject( lua_State *L, void *Ptr )
{
	void **ud;

	// Create the object and initialize its pointer
	ud = (void**)lua_newuserdata( L, sizeof( void*) );
	*ud = Ptr;
	// Swap the table and metatable for stack arg purposes
	lua_insert( L, -2 );
	lua_setmetatable( L, -2 );
}





void NewLuaObject( lua_State *L, STR8 ClsName, void *Ptr )
{
	luaL_getmetatable( L, ClsName );
	if (lua_istable( L, -1 ) )
	{
		CreateLuaObject( L, Ptr );
	}
}








void InitializeLua( )
{
	L = lua_open();
	luaL_openlibs(L);

	// Create the accessor metatable
//	CreateLuaType( L, ACCESSOR_TABLE, LuaAccessors);
//	lua_setglobal( L, ACCESSOR_TABLE); // We also want this class to be known to the script
//	lua_pop(L, 1);

	// Create a wide-character savvy string
	CreateLuaType( L, "wstring", WStringMethods);
	lua_setglobal( L, "wstring"); // We also want this class to be known to the script

	// Set up the game info
	LuaTacticalSetup( L);
	LuaStrategicSetup( L);
	LuaEnvironmentSetup( L);
}

int EvalLua (const wchar_t* buff) {
	int error;
	int newlen;
	STR8 newstr = NULL;

	// Since we get wide chars coming in, we need to convert to UTF8
	newlen = WideCharToMultiByte( CP_UTF8, 0, buff, -1, newstr, 0, NULL, NULL);
	newstr = (STR8) MemAlloc( newlen);
	WideCharToMultiByte( CP_UTF8, 0, buff, -1, newstr, newlen, NULL, NULL);

	error = luaL_loadbuffer(L, newstr, newlen-1, "line") ||
		lua_pcall(L, 0, 0, 0);

	MemFree( newstr);

	if (error) {
		const char *error = lua_tostring(L, -1);
		int len = strlen( error);
		if (len >= 7 && !strcmp( error + len - 7, "'<eof>'"))
		{
			lua_pop(L, 1);	/* pop error message from the stack */
			return FALSE;
		}
		printf( "%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);	/* pop error message from the stack */
		return TRUE;
	}

	return TRUE;
}

void ShutdownLua( ) {
	lua_close(L);
}
