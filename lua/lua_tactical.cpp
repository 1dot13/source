#include "Lua Interpreter.h"
#include "lwstring.h"

#include "Overhead.h"
#include "Isometric Utils.h"
#include "soldier tile.h"
#include "Soldier Profile.h"
#include "Soldier Functions.h"
#include "ai.h"

#define SOLDIER_CLASS "ja2_SoldierClass"

static int LuaGetSoldierName( lua_State *L )
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

static int LuaGetSoldierFullName( lua_State *L )
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

static int LuaGetSoldierGrid( lua_State *L )
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

static int LuaSetSoldierGrid( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newgrid > 0 && newgrid < MAX_MAP_POS, 2, "The grid number must be on map" );
	TeleportSoldier( pSoldier, (INT16) newgrid, TRUE);
	return 0;
}

static int LuaGetSoldierLevel( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSoldier->pathing.bLevel);
	}
	return 1;
}

static int LuaSetSoldierLevel( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newlevel = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newlevel >= 0 && newlevel <= 1, 2, "The level must be 1 or 0" );
	HandlePlacingRoofMarker( pSoldier, pSoldier->sGridNo, (BOOLEAN) newlevel, TRUE );
	pSoldier->SetSoldierHeight((FLOAT)50*newlevel );
	HandlePlacingRoofMarker( pSoldier, pSoldier->sGridNo, (BOOLEAN) newlevel, TRUE );
	return 0;
}

static int LuaGetSoldierAPs( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	if (!pSoldier)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSoldier->bActionPoints);
	}
	return 1;
}

static int LuaSetSoldierAPs( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) lua_touserdata( L, 1 );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newaps = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newaps > 0 && newaps < 256, 2, "The grid number must be on screen!" );
	pSoldier->bActionPoints = (INT8) newaps;
	return 0;
}

static int LuaSoldierWalkTo( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) luaL_checkudata( L, 1, SOLDIER_CLASS );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 2);
	luaL_argcheck( L, newgrid > 0 && newgrid < MAX_MAP_POS, 2, "The grid number must be on screen!" );
	pSoldier->aiData.bAction = AI_ACTION_WALK;
	pSoldier->aiData.usActionData = (INT16) newgrid;
	pSoldier->pathing.bPathStored = FALSE;
	pSoldier->aiData.bActionInProgress = ExecuteAction( pSoldier);
	return 0;
}

static int LuaSoldierRunTo( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) luaL_checkudata( L, 1, SOLDIER_CLASS );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newgrid = luaL_checkinteger( L, 2);
	luaL_argcheck( L, newgrid > 0 && newgrid < MAX_MAP_POS, 2, "The grid number must be on screen!" );
	pSoldier->aiData.bAction = AI_ACTION_RUN;
	pSoldier->aiData.usActionData = (INT16) newgrid;
	pSoldier->pathing.bPathStored = FALSE;
	pSoldier->aiData.bActionInProgress = ExecuteAction( pSoldier);
	return 0;
}

static int LuaSoldierChangeStance( lua_State *L )
{
	SOLDIERTYPE **ppSoldier = (SOLDIERTYPE**) luaL_checkudata( L, 1, SOLDIER_CLASS );
	SOLDIERTYPE *pSoldier = *ppSoldier;
	int newstance = luaL_checkinteger( L, 2);
	pSoldier->ChangeSoldierStance( (UINT8) newstance);
	return 0;
}

static LuaAttrib Soldier[] = {
	{ "name", LuaGetSoldierName, },
	{ "fullname", LuaGetSoldierFullName, },
	{ "grid", LuaGetSoldierGrid, LuaSetSoldierGrid },
	{ "level", LuaGetSoldierLevel, LuaSetSoldierLevel },
	{ "APs", LuaGetSoldierAPs, LuaSetSoldierAPs },
	{ "walkto", NULL, NULL, LuaSoldierWalkTo },
	{ "runto", NULL, NULL, LuaSoldierRunTo },
	{ "changestance", NULL, NULL, LuaSoldierChangeStance },
	{ NULL, },
};


void LuaTacticalSetup(lua_State *L)
{
	// Create a soldier type
	CreateLuaClass( L, SOLDIER_CLASS, Soldier );
	lua_setglobal( L, SOLDIER_CLASS); // We also want this class to be known to the script

	// Create a soldier list type and create its object
	lua_createtable( L, TOTAL_SOLDIERS, 0 );
	for ( int sold = 0; sold < TOTAL_SOLDIERS; ++sold )
	{
		lua_pushinteger( L, sold);
		if (MercPtrs[ sold ] )
		{
			NewLuaObject( L, SOLDIER_CLASS, MercPtrs[ sold ] );
		}
		else
		{
			lua_pushnil( L );
		}
		lua_settable( L, -3);
	}

	// Give this new object a name
	lua_setglobal( L, "Soldiers" );
}
