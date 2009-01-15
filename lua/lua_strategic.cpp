#include "Lua Interpreter.h"
#include "Campaign Types.h"
#include "Strategic Movement.h"

#define SECTOR_CLASS "ja2_SectorClass"
#define MOBILEGROUP_CLASS "ja2_MobileGroup"

static int LuaGetSectorNumAdmins( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	if (!pSector)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSector->ubNumAdmins);
	}
	return 1;
}

static int LuaSetSectorNumAdmins( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	int newnum = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newnum >= 0 && newnum < 256, 2, "The number of soldiers must be between 0 and 255" );
	pSector->ubNumAdmins = (UINT8) newnum;
	return 0;
}

static int LuaGetSectorNumTroops( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	if (!pSector)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSector->ubNumTroops);
	}
	return 1;
}

static int LuaSetSectorNumTroops( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	int newnum = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newnum >= 0 && newnum < 256, 2, "The number of soldiers must be between 0 and 255" );
	pSector->ubNumTroops = (UINT8) newnum;
	return 0;
}

static int LuaGetSectorNumElites( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	if (!pSector)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSector->ubNumElites);
	}
	return 1;
}

static int LuaSetSectorNumElites( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	int newnum = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newnum >= 0 && newnum < 256, 2, "The number of soldiers must be between 0 and 255" );
	pSector->ubNumElites = (UINT8) newnum;
	return 0;
}

static int LuaGetSectorNumCreatures( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	if (!pSector)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pSector->ubNumCreatures);
	}
	return 1;
}

static int LuaSetSectorNumCreatures( lua_State *L )
{
	SECTORINFO **ppSector = (SECTORINFO**) lua_touserdata( L, 1 );
	SECTORINFO *pSector = *ppSector;
	int newnum = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newnum >= 0 && newnum < 256, 2, "The number of soldiers must be between 0 and 255" );
	pSector->ubNumCreatures = (UINT8) newnum;
	return 0;
}


LuaAttrib Sector[] = {
	{ "numadmins", LuaGetSectorNumAdmins, LuaSetSectorNumAdmins },
	{ "numtroops", LuaGetSectorNumTroops, LuaSetSectorNumTroops },
	{ "numelites", LuaGetSectorNumElites, LuaSetSectorNumElites },
	{ "numcreatures", LuaGetSectorNumCreatures, LuaSetSectorNumCreatures },
	{ NULL, },
};


static int LuaGetGroupList( lua_State *L )
{
	GROUP *pGroup = gpGroupList;
	int idx = 0;

	lua_newtable( L);

	//Count the number of active groups
	while( pGroup )
	{
		lua_pushinteger( L, pGroup->ubGroupID);
		lua_rawseti( L, -2, idx++);
		pGroup = pGroup->next;
	}

	return 1;
}

static int LuaGetGroup( lua_State *L )
{
	int grp = luaL_checkinteger( L, 1);

	GROUP *pGroup = GetGroup( (UINT8) grp);
	
	if (pGroup)
	{
		NewLuaObject( L, MOBILEGROUP_CLASS, pGroup);
	}
	else
	{
		lua_pushnil( L );
	}

	return 1;
}

static int LuaGroupCreate( lua_State *L )
{
	UINT32 Sector = luaL_checkinteger( L, 1);
	UINT8 NumAdmins = (UINT8) luaL_checkinteger( L, 2);
	UINT8 NumTroops = (UINT8) luaL_checkinteger( L, 3);
	UINT8 NumElites = (UINT8) luaL_checkinteger( L, 4);

	GROUP *pGroup = CreateNewEnemyGroupDepartingFromSector( Sector, NumAdmins, NumTroops, NumElites );

	if (pGroup)
	{
		NewLuaObject( L, MOBILEGROUP_CLASS, pGroup);
	}
	else
	{
		lua_pushnil( L );
	}

	return 1;
}

static int LuaGetGroupID( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->ubGroupID);
	}
	return 1;
}

static int LuaGetGroupX( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->ubSectorX);
	}
	return 1;
}

static int LuaGetGroupY( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->ubSectorY);
	}
	return 1;
}

static int LuaGetGroupZ( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->ubSectorZ);
	}
	return 1;
}

static int LuaGetGroupWaypoints( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;

	if (!pGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		WAYPOINT *waypoint = pGroup->pWaypoints;
		int idx = 0;

		lua_newtable(L);
		while (waypoint)
		{
			lua_newtable(L);
			lua_pushstring(L, "x");
			lua_pushinteger(L, waypoint->x);
			lua_rawset(L, -3);
			lua_pushstring(L, "y");
			lua_pushinteger(L, waypoint->y);
			lua_rawset(L, -3);

			lua_rawseti(L, -2, idx++);

			waypoint = waypoint->next;
		}
	}
	return 1;
}

static int LuaGetGroupNumAdmins( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup || !pGroup->pEnemyGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->pEnemyGroup->ubNumAdmins);
	}
	return 1;
}

static int LuaGetGroupNumTroops( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup || !pGroup->pEnemyGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->pEnemyGroup->ubNumTroops);
	}
	return 1;
}

static int LuaGetGroupNumElites( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup || !pGroup->pEnemyGroup)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushinteger( L, pGroup->pEnemyGroup->ubNumElites);
	}
	return 1;
}

static int LuaGroupAddWaypoint( lua_State *L )
{
	GROUP **ppGroup = (GROUP**) lua_touserdata( L, 1 );
	GROUP *pGroup = *ppGroup;
	if (!pGroup)
	{
		lua_pushinteger( L, 0 );
	}
	else
	{
		int x = lua_tointeger(L, 2);
		int y = lua_tointeger(L, 3);

		lua_pushinteger( L, AddWaypointToPGroup( pGroup, (UINT8) x, (UINT8) y) );
	}
	return 1;
}

static LuaAttrib MobileGroup[] = {
	{ "grouplist", NULL, NULL, NULL, LuaGetGroupList },
	{ "get", NULL, NULL, NULL, LuaGetGroup },
	{ "new", NULL, NULL, NULL, LuaGroupCreate },
	
	{ "ID", LuaGetGroupID },
	{ "X", LuaGetGroupX },
	{ "Y", LuaGetGroupY },
	{ "Z", LuaGetGroupZ },
	{ "waypoints", LuaGetGroupWaypoints },
	{ "numadmins", LuaGetGroupNumAdmins },
	{ "numtroops", LuaGetGroupNumTroops },
	{ "numelites", LuaGetGroupNumElites },
	{ "addwaypoint", NULL, NULL, LuaGroupAddWaypoint },
	{ NULL, },
};

void LuaStrategicSetup(lua_State *L)
{
	// Create a sector type
	CreateLuaClass( L, SECTOR_CLASS, Sector );
	//lua_setglobal( L, SECTOR_CLASS); // We also want this class to be known to the script

	// Create a sector list type and create its object
	lua_createtable (L, 256, 0);
	for (int sect=0; sect < 256; sect++)
	{
		lua_pushinteger( L, sect);
		NewLuaObject( L, SECTOR_CLASS, &SectorInfo[sect]);
		lua_settable( L, -3);
	}

	// Give this new object a name
	lua_setglobal( L, "Sectors" );

	// Create a mobile group type
	CreateLuaClass( L, MOBILEGROUP_CLASS, MobileGroup );
	lua_setglobal( L, "MobileGroups"); // We also want this class to be known to the script
}
