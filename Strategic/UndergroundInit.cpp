#include <iostream>
#include <string>
#include <sstream>

#include "UndergroundInit.h"
#include "Campaign Init.h"
#include "Campaign Types.h"
#include "FileMan.h"
#include "GameSettings.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


extern UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );

static int l_addSector(lua_State *L);
static int lh_getIntegerFromTable(lua_State *L, char * fieldname);
static std::string lh_getStringFromTable(lua_State *L, char * fieldname);
static std::wstring lh_getWStringFromTable(lua_State *L, char * fieldname);
static bool locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z);

using namespace std;


BOOLEAN LetLuaBuildUndergroundSectorInfoList()
{
	char * filename = "scripts\\initunderground.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);
	
	lua_register(L, "addSector", l_addSector);

	lua_pushinteger(L, gGameOptions.ubDifficultyLevel);
	lua_setglobal(L, "difficultyLevel");

	lua_pushinteger(L, gGameOptions.ubGameStyle);
	lua_setglobal(L, "gameStyle");
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	lua_close(L);

	delete[] buffer;

	return true;
}


static int l_addSector(lua_State *L)
{
	UNDERGROUND_SECTORINFO* curr = NULL;

	std::string sector_id = lh_getStringFromTable(L, "location");
	
	UINT8 x, y, z;
	if (locationStringToCoordinates(sector_id, &x, &y, &z))
	{
		curr = NewUndergroundNode(x, y, z);

		curr->ubNumAdmins = lh_getIntegerFromTable(L, "numAdmins");
		curr->ubNumTroops = lh_getIntegerFromTable(L, "numTroops");
		curr->ubNumElites = lh_getIntegerFromTable(L, "numElites");

		curr->ubCreatureHabitat = lh_getIntegerFromTable(L, "creatureHabitat");
		curr->ubNumCreatures = lh_getIntegerFromTable(L, "numCreatures");

		curr->ubNumBloodcats = lh_getIntegerFromTable(L, "numBloodcats");
	}
	
	return 0;	// number of results
}


static int lh_getIntegerFromTable(lua_State *L, char * fieldname)
{
	lua_getfield(L, -1, fieldname);
	int i = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return i;
}


static std::string lh_getStringFromTable(lua_State *L, char * fieldname)
{
	lua_getfield(L, -1, fieldname);
	string s = lua_tostring(L, -1);
	lua_pop(L, 1);
	return s;
}


// extract x,y,z from strings like "D13-1"
static bool locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z)
{
	int length = loc.length();
	
	// gather row
	if (length < 4 || length > 5)
		return false;
	
	char row = loc[0];
	if (row >= 'A' && row <= 'P')
		*y = row - 'A' + 1;
	else if (row >= 'a' && row <= 'p')
		*y = row - 'a' + 1;
	else
		return false;
	
	// gather column
	loc = loc.substr(1);
	stringstream ss = stringstream();
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
	int col;
	ss >> col;
	if (col >= 1 && col <= 16)
		*x = col;
	else
		return false;
	
	// gather level
	char lvl = loc[loc.length() - 1];
	if (lvl >= '1' && lvl <= '3')
		*z = lvl - '0';
	else
		return false;
	
	return true;
}
