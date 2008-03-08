/*
** $Id: lstring.h,v 1.43 2005/04/25 19:24:10 roberto Exp $
** String table (keep all strings handled by Lua)
** See Copyright Notice in lua.h
*/

#ifndef lwstring_h
#define lwstring_h

#include <wchar.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "Types.h"

typedef struct {
	int len;
	CHAR16 data[1];
} TWString;

#define sizewstring(s)	(sizeof(int) + (s) * sizeof(CHAR16) + sizeof(CHAR16))

#define luaWS_newstr(L, s)	(luaWS_newlstr(L, s, wcslen(s)))

void luaWS_newlstr (lua_State *L, const CHAR16 *str, size_t l);


#endif
