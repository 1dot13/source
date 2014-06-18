#include <iostream>
#include <string>
#include <sstream>

#include "UndergroundInit.h"
#include "FileMan.h"
#include "GameSettings.h"
#include "Game Clock.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lua_state.h"
#include "lua_table.h"
#include "lua_function.h"
#include "connect.h"

// externals
UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );
extern BOOLEAN gfGettingNameFromSaveLoadScreen;

// helper functions
static std::string XYZtoString(INT16 x, INT16 y, INT16 z);

// keystroke savers
#define LF vfs::Log::endl
#define ON LuaLogger::ON
#define OFF LuaLogger::OFF

// global
LuaUnderground g_luaUnderground;


template <>
bool LuaTable::getValue<UINT8>(const char * index, UINT8& value)
{
	int dummy;

	bool b = getValue(index, dummy);
	if (b)
	{
		value = static_cast<UINT8>(dummy);
		return true;
	}

	return false;
}

// Calls into Lua script to let build underground sector list.
BOOLEAN LuaUnderground::InitializeSectorList()
{
	// WANNE: Skip initialization in a multiplayer game, because some variables are not yet set
	// No problem: Underground sectors are not currently used in a multiplayer game
	if ( !is_networked || gGameOptions.ubDifficultyLevel > 0)
	{
		LuaState L = GetLuaState();

		LuaFunction initsectorlist_func(L, "BuildUndergroundSectorList");
		
		initsectorlist_func
			.TableOpen()
			.TParam("difficultyLevel", int(gGameOptions.ubDifficultyLevel))
			.TParam("gameStyle", int(gGameOptions.ubGameStyle))
			.TableClose();		
		
		SGP_THROW_IFFALSE(initsectorlist_func.Call(1), "call to lua function BuildUndergroundSectorList failed");
		
		LuaTable result(L, -1);
		if (result.is_valid())
		{
			int len = result.length();
			for (int i = 1; i <= result.length(); ++i)
			{
				LuaTable sectorData(result, i);
				if (sectorData.is_valid())
				{
					std::string location;
					sectorData.getValue("location", location);

					UINT8 x, y, z;
					if (locationStringToCoordinates(location, &x, &y, &z))
					{
						UNDERGROUND_SECTORINFO* s = NewUndergroundNode(x, y, z);

						sectorData.getValue("numAdmins", s->ubNumAdmins);
						sectorData.getValue("numTroops", s->ubNumTroops);
						sectorData.getValue("numElites", s->ubNumElites);
						sectorData.getValue("numTanks", s->ubNumTanks);

						sectorData.getValue("numBloodcats", s->ubNumBloodcats);
						sectorData.getValue("numCreatures", s->ubNumCreatures);
						sectorData.getValue("creatureHabitat", s->ubCreatureHabitat);

						sectorData.getValue("music", s->ubMusicMode);
						if (s->ubMusicMode < CM_COMPAT || CM_ALWAYS < s->ubMusicMode)
							s->ubMusicMode = CM_COMPAT;

						// logging only
						{
							m_log << "adding sector: " << location << LF;
							m_log << "\t- Enemy garrison" << LF;
							m_log << "\t\tAdmins: " << static_cast<int>(s->ubNumAdmins) << LF;
							m_log << "\t\tTroops: " << static_cast<int>(s->ubNumTroops) << LF;
							m_log << "\t\tElites: " << static_cast<int>(s->ubNumElites) << LF;
							m_log << "\t\tTanks: " << static_cast<int>(s->ubNumTanks) << LF;
							m_log << "\t- Creature population" << LF;
							m_log << "\t\tBloodcats: " << static_cast<int>(s->ubNumBloodcats) << LF;
							m_log << "\t\tCrepitus: " << static_cast<int>(s->ubNumCreatures) << LF;
							m_log << "\t\t- Habitat: " << static_cast<int>(s->ubCreatureHabitat) << LF;
							m_log << "\t\t- Music mode: ";
							switch (s->ubMusicMode)
							{
								case CM_COMPAT: m_log << "compat" << LF; break;
								case CM_AUTO: m_log << "auto" << LF; break;
								case CM_NEVER: m_log << "never" << LF; break;
								case CM_ALWAYS: m_log << "always" << LF; break;
							}
							m_log << LF;
						}
					}
					else
						m_log << "WARNING: ignoring invalid location ID: '" << location << "'" << LF;

					m_log.Flush();				
				}

				lua_pop(L(), 1);  // sectorData
			}
		}
		lua_pop(L(), 1); // result
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


// Calls into Lua script to get the loadscreen for a given sector.
void LuaUnderground::GetLoadscreen(INT16 x, INT16 y, INT16 z, std::string& loadscreen, std::string& format)
{
	const std::string strSector = XYZtoString(x, y, z);

	LuaState L = GetLuaState();
	LuaFunction getLoadscreen_func(L, "GetLoadscreen");

	getLoadscreen_func
		.Param(strSector)
		.Param(int(GetWorldMinutesInDay()));

	SGP_THROW_IFFALSE(getLoadscreen_func.Call(2), "call to lua function GetLoadscreen failed");


	{
		if (lua_isstring(L(), -1))
		{
			format.append(lua_tostring(L(), -1));
		}
		else
			m_log << "ERROR: GetLoadscreen('" << strSector << "'): return value #2 is not a valid string object" << LF;
		lua_pop(L(), 1);

		if (lua_isstring(L(), -1))
		{
			loadscreen.append(lua_tostring(L(), -1));
		}
		else
			m_log << "ERROR: GetLoadscreen('" << strSector << "'): return value #1 is not a valid string object" << LF;
		lua_pop(L(), 1);
	}
	
	return;
}


// Calls into Lua script in order to request a sector name for a given sector.
// pSector can be null, e.g. when saveloadscreen requests sector names;
// in this case we didn't initialize the sector list nor have we loaded it from savegame yet.
void LuaUnderground::GetSectorName(INT16 x, INT16 y, INT16 z, const UNDERGROUND_SECTORINFO* pSector, STR16 buffer, ::size_t bufSizeInWChar, BOOLEAN fDetailed)
{
	const std::string coords = XYZtoString(x, y, z);
	LuaState L = GetLuaState();

	LuaFunction getSectorName_func(L, "GetSectorName");
	
	getSectorName_func
		.Param(coords)
		.TableOpen()
			.TParam("visited", gfGettingNameFromSaveLoadScreen || (pSector && pSector->fVisited))
			.TParam("creaturesPresent", pSector && pSector->ubNumCreatures > 0)
			.TParam("detailed", !!fDetailed) // conversion to C++ bool
		.TableClose();

	SGP_THROW_IFFALSE(getSectorName_func.Call(1), "call to lua func GetSectorName failed");


	// TODO: check if this still makes a whole lot of sense now
	{
		memset(buffer, 0, bufSizeInWChar * sizeof(CHAR16));

		if (lua_isstring(L(), -1))
		{
			const char * utf8 = lua_tostring(L(), -1);
			int result = MultiByteToWideChar(CP_UTF8, 0, lua_tostring(L(), -1), -1, buffer, bufSizeInWChar-1);
			if (result == 0)
			{
				// Houston, we have a problem...
				buffer[bufSizeInWChar-1] = L'\0';

				DWORD errorcode = GetLastError();
				if (errorcode == ERROR_INSUFFICIENT_BUFFER)
				{
					// quick note on logging:
					// we explicitly turn logging off once we encounter an error
					// since this function gets called a lot (every frame or so)
					m_log << "'" << utf8 << "': wchar representation too long" << LF << OFF;
				}
				else
				{
					m_log << "'" << utf8 << "': illegal unicode data" << LF << OFF;;
				}
			}
		}
		else
			m_log << "ERROR: requesting sector name for '" << coords << "' failed, no valid string object has been returned" << LF << OFF;

		lua_pop(L(), 1);	// return value
	}

	return;
}


// Parse the scripts, set everything up.
void LuaUnderground::LoadScript(const char * langPrefix)
{
	LuaState L = GetLuaState();

	// #1: builds strategic underground layout
	char * initunderground = "scripts\\initunderground.lua";

	// #2: provides sector names; this is localized, build actual filename at runtime
	char * sectornamesfile = "undergroundsectornames.lua";
	std::string sectornames("scripts\\");
	if (langPrefix != NULL)
		sectornames.append(langPrefix);
	sectornames.append(sectornamesfile);

	// we don't use L.EvalFile because the lua parser dislikes UTF-8 BOMs (gonna strip them first)
	RunScript(L, sectornames.c_str());
	RunScript(L, initunderground);
	
	// TODO: see if we can make this better
	{
		lua_getglobal(L(), "logging");
		if (lua_isboolean(L(), -1) && lua_toboolean(L(), -1))
			m_log << ON;
		lua_pop(L(), -1);
	}

	return;
}



// Loads a Lua script and let the interpreter process it.
// Returns true if successful.
// Throws an exception if file not found.
bool LuaUnderground::RunScript(LuaState& L, const char * szFileName)
{
	using namespace std;

	// stupid file opening functions don't use const parameters...
	vector<char> filenamebuffer(strlen(szFileName)+1);
	strcpy(&filenamebuffer[0], szFileName);
	char * filename = &filenamebuffer[0];


	UINT32 size, bytesRead;
	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);

		m_log << msg << LF;
		m_log.Flush();

		//throw sgp::Exception(msg.c_str(), __LINE__, _FUNCTION_FORMAT_, __FILE__);
		SGP_THROW(msg);		
	}

	size = FileSize(filename);
	vector<char> buffer = vector<char>(size);
	FileRead(file, &buffer[0], size, &bytesRead);
	FileClose(file);

	// simple UTF8 BOM check:
	// replace with blanks if necessary since Lua HATES byte order marks
	const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
	if (memcmp(&buffer[0], bom, 3) == 0)
	{
		buffer[0] = buffer[1] = buffer[2] = ' ';
	}

	return L.EvalString(&buffer[0], buffer.size(), szFileName);
}


LuaUnderground::LuaUnderground()
	: m_log("initunderground.log", false, true, vfs::Log::FLUSH_BUFFER)
{
}


LuaUnderground::~LuaUnderground()
{
	// WANNE: Disabled, because it leads to an unhandled exception when shutting down ja2 1.13
	/*m_log.Flush();*/
}

// extract x,y,z from strings like "D13-1"
BOOLEAN locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z)
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
	std::stringstream ss;
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

static std::string XYZtoString(INT16 x, INT16 y, INT16 z)
{
	using namespace std;

	stringstream ss;
	ss << static_cast<char>(y+'A'-1) << x << '-' << z;
	string tmp;
	ss >> tmp;
	return tmp;
}
