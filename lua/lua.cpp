#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Lua Interpreter.h"
#include "lwstring.h"

#include "Overhead.h"
#include "Isometric Utils.h"
#include "soldier tile.h"
#include "Soldier Profile.h"
#include "ai.h"

lua_State *L;

#define ARRAY_INDEX " idx"  // The space is intentional to make this an out-of-band field

#define SOLDIER_CLASS "ja2_SoldierClass"

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

	luaL_newmetatable( L, ClassName );
	for (i = ClassList; i->name; i++)
	{
		lua_pushstring( L, i->name);
		lua_pushlightuserdata( L, Attribs);
		lua_pushcclosure( L, i->func, 1);
		lua_rawset( L, -3);
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






static int LuaGetMercPtr( lua_State *L)
{
	int v = lua_tointeger( L, 2 );
	luaL_argcheck( L, (v >= 0 && v < TOTAL_SOLDIERS), 2, "The soldier index is out of range");
	if (MercPtrs[ v ] )
	{
		NewLuaObject( L, "ja2_SoldierClass", MercPtrs[ v ] );
	}
	else
	{
		lua_pushnil( L );
	}
	
	return 1;
}

static luaL_Reg SoldierList[] = {
	{ "__index", LuaGetMercPtr, },
	{ NULL, },
};




int LuaGetSoldierName( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		luaWS_newstr( L, pSoldier->name);
	}
	return 1;
}

int LuaGetSoldierFullName( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		if (pSoldier->ubProfile < NUM_PROFILES)
		{
			luaWS_newstr( L, gMercProfiles[pSoldier->ubProfile].zName );
		}
		else
		{
			return LuaGetSoldierName( L);
		}
	}
	return 1;
}

int LuaGetSoldierGrid( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSoldier->sGridNo);
	}
	return 1;
}

int LuaSetSoldierGrid( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newgrid > 0 && newgrid <= NOWHERE, 2, "The grid number must be on screen!" );
	TeleportSoldier( pSoldier, newgrid, TRUE);
	return 0;
}

int LuaSoldierWalkTo( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) luaL_checkudata( L, 1, SOLDIER_CLASS );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 2);
	luaL_argcheck( L, newgrid > 0 && newgrid <= NOWHERE, 2, "The grid number must be on screen!" );
	pSoldier->bAction = AI_ACTION_WALK;
	pSoldier->usActionData = newgrid;
	pSoldier->bPathStored = FALSE;
	pSoldier->bActionInProgress = ExecuteAction( pSoldier);
	return 0;
}

int LuaSoldierRunTo( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) luaL_checkudata( L, 1, SOLDIER_CLASS );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 2);
	luaL_argcheck( L, newgrid > 0 && newgrid <= NOWHERE, 2, "The grid number must be on screen!" );
	pSoldier->bAction = AI_ACTION_RUN;
	pSoldier->usActionData = newgrid;
	pSoldier->bPathStored = FALSE;
	pSoldier->bActionInProgress = ExecuteAction( pSoldier);
	return 0;
}

LuaAttrib Soldier[] = {
	{ "name", LuaGetSoldierName, },
	{ "fullname", LuaGetSoldierFullName, },
	{ "grid", LuaGetSoldierGrid, LuaSetSoldierGrid },
	{ "walkto", NULL, NULL, LuaSoldierWalkTo },
	{ "runto", NULL, NULL, LuaSoldierRunTo },
	{ NULL, },
};



extern luaL_Reg WStringMethods[];

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

	// Create a soldier type
	CreateLuaClass( L, SOLDIER_CLASS, Soldier );
	lua_setglobal( L, SOLDIER_CLASS); // We also want this class to be known to the script

	// Create a soldier list type and create its object
	CreateLuaType( L, "ja2_SoldierListClass", SoldierList);
	CreateLuaObject( L, NULL );
	// Give this new object a name
	lua_setglobal( L, "Soldiers" );
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
			lua_pop(L, 1);  /* pop error message from the stack */
			return FALSE;
		}
		cout << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);  /* pop error message from the stack */
		return TRUE;
	}

	return TRUE;
}

void ShutdownLua( ) {
	lua_close(L);
}
