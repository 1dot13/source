#include "builddefines.h"
#include <Windows.h>

#include "MemMan.h"

#include "lwstring.h"

void luaWS_newlstr (lua_State *L, const CHAR16 *str, size_t l)
{
  TWString *ts;

  // Create and initialize this data
  int size = sizewstring( l);
  ts = (TWString*) lua_newuserdata( L, size );
  ts->len = l;
  memcpy( ts->data, str, size);

  // Make this data a wstring
  luaL_getmetatable(L, "wstring");
  lua_setmetatable(L, -2);
}

// Create a string from a UTF-8 representation
// Called from LUA code
int LuaWStringFromUTF8( lua_State *L )
{
	const CHAR8 *utf8 = luaL_checkstring( L, 1 );
	STR16 str = NULL;
	int len;

	// Find out how long this needs to be
	len = MultiByteToWideChar( CP_UTF8, 0, utf8, -1, str, 0);
	str = (STR16) MemAlloc( len * sizeof(CHAR16));

	// Get the string
	MultiByteToWideChar( CP_UTF8, 0, utf8, -1, str, len);

	luaWS_newlstr( L, str, len - 1);

	MemFree( str);
	return 1;
}

int LuaWStringByte( lua_State* L )
{
	int start;
	int stop;
	int idx;
	TWString *tw;

	tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	start = luaL_optint( L, 2, 1 );
	stop = luaL_optint( L, 3, 1 );
	
	// This function in the char strings will return nothing if the params are out of bounds.
	if (start < 1 || start >= tw->len ||
		stop < start)
	{
		return 0;
	}
	if (stop >= tw->len)
	{
		stop = tw->len;
	}

	// Make these 0-based
	start--;

	for (idx = start; idx < stop; idx++)
	{
		lua_pushinteger( L, tw->data[ idx] );
	}

	return stop - start;
}

static int LuaWStringChar( lua_State *L )
{
	int len = lua_gettop( L) + 1;
	STR16 str = (STR16) MemAlloc( len * sizeof( CHAR16) );
	int idx;
	
	len--;
	for( idx=0; idx<len; idx++)
	{
		str[ idx ] = (CHAR16) luaL_checkint( L, idx + 1 );
	}
	str[ idx ] = 0;

	luaWS_newlstr( L, str, len);

	MemFree( str);
	return 1;
}

static int LuaWStringFind( lua_State *L )
{
	return 0;
}

static int LuaWStringFormat( lua_State *L )
{
	return 0;
}

static int LuaWStringGMatch( lua_State *L )
{
	return 0;
}

static int LuaWStringGSub( lua_State *L )
{
	return 0;
}

static int LuaWStringLen( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	lua_pushinteger( L, tw->len );
	return 1;
}

static int LuaWStringLower( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	int len = tw->len + 1;
	STR16 str = (STR16) MemAlloc( len * sizeof( CHAR16) );
	int idx;
	
	for( idx=0; idx<len-1; idx++)
	{
		str[ idx ] = (CHAR16) towlower( tw->data[ idx ] );
	}
	str[ idx ] = 0;

	luaWS_newlstr( L, str, len-1);

	MemFree( str);
	return 1;
}

static int LuaWStringMatch( lua_State *L )
{
	return 0;
}

static int LuaWStringRep( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	int num = (luaL_checkint( L, 2) >= 0) ? lua_tointeger( L, 2) : 0;
	int len = tw->len * num + 1;
	STR16 str = (STR16) MemAlloc( len * sizeof( CHAR16) );
	int idx;

	if (num > 0)
	{
		wcscpy( str, tw->data);
	}
	for( idx=1; idx<num; idx++)
	{
		wcscat( str, tw->data);
	}

	luaWS_newlstr( L, str, len-1);

	MemFree( str);
	return 1;
}

static int LuaWStringReverse( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	int len = tw->len + 1;
	STR16 str = (STR16) MemAlloc( len * sizeof( CHAR16) );
	int idx;
	
	for( idx=0; idx<len-1; idx++)
	{
		str[ idx ] = tw->data[ tw->len - idx - 1 ];
	}
	str[ idx ] = 0;

	luaWS_newlstr( L, str, len-1);

	MemFree( str);
	return 1;
}

static int LuaWStringSub( lua_State *L )
{
	return 0;
}

static int LuaWStringUpper( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	int len = tw->len + 1;
	STR16 str = (STR16) MemAlloc( len * sizeof( CHAR16) );
	int idx;
	
	for( idx=0; idx<len-1; idx++)
	{
		str[ idx ] = (CHAR16) towupper( tw->data[ idx ] );
	}
	str[ idx ] = 0;

	luaWS_newlstr( L, str, len-1);

	MemFree( str);
	return 1;
}

static int LuaWStringToString( lua_State *L )
{
	TWString *tw = (TWString*) luaL_checkudata( L, 1, "wstring" );
	int newlen;
	STR8 newstr = NULL;

	newlen = WideCharToMultiByte( CP_UTF8, 0, tw->data, -1, newstr, 0, NULL, NULL);
	newstr = (STR8) MemAlloc( newlen);
	WideCharToMultiByte( CP_UTF8, 0, tw->data, -1, newstr, newlen, NULL, NULL);
	lua_pushstring( L, newstr);
	MemFree( newstr);
	return 1;
}

static int LuaWStringIndex( lua_State *L )
{
	luaL_checkudata( L, 1, "wstring" );
	const CHAR8 *idx = luaL_checkstring( L, 2 );
	
	// Disqualify metafunctions
	if ( idx[0] == '_' && idx[1] == '_' )
	{
		lua_pushnil( L );
		return 1;
	}

	lua_getmetatable( L, 1 );
	lua_insert( L, 2 );
	lua_rawget( L, 2 );
	return 1;
}

luaL_Reg WStringMethods[] = {
    // Constructors
	{ "fromUTF8", LuaWStringFromUTF8, },

	// String-specific
	{ "byte", LuaWStringByte, },
	{ "char", LuaWStringChar, },
	{ "find", LuaWStringFind, },
	{ "format", LuaWStringFormat, },
	{ "gmatch", LuaWStringGMatch, },
	{ "gsub", LuaWStringGSub, },
	{ "len", LuaWStringLen, },
	{ "lower", LuaWStringLower, },
	{ "match", LuaWStringMatch, },
	{ "rep", LuaWStringRep, },
	{ "reverse", LuaWStringReverse, },
	{ "sub", LuaWStringSub, },
	{ "upper", LuaWStringUpper, },

	// Special functions
	{ "__tostring", LuaWStringToString, },
	{ "__index", LuaWStringIndex, },
	{ NULL, },
};
