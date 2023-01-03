#include <vector>
#include <cctype>
#include <sstream>
#include <string>

#include "DEBUG.H"
#include "Dialogue Control.h"
#include "FileMan.h"
#include "GameSettings.h"
#include "Soldier Profile.h"
#include "strategic.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Mines LUA.h"
#include "UndergroundInit.h"
#include <vfs/Core/vfs_debug.h>	// for CBasicException

#include "connect.h"

using namespace std;

static bool locationStrToCoords(string str, INT16* x, INT16* y);


// global
LuaMines g_luaMines;


/////////////////////////////////////////
////////////////////////////////////////

LuaMines::LuaMines()
	: m_log(L"initmines.log", false)
{
	m_L = lua_open();
	luaL_openlibs(m_L);
}


void LuaMines::LoadScript()
{
	char * filename = "scripts\\initmines.lua";
	UINT32 size, bytesRead;


	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		m_log << msg << vfs::Log::endl;
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	vector<char> buffer = vector<char>(size+1);
	buffer[size] = 0;
	FileRead(file, &buffer[0], size, &bytesRead);
	FileClose(file);


	if (luaL_dostring(m_L, &buffer[0]))
	{
		string error("lua parsing error: ");
		error.append(lua_tostring(m_L, -1));
		m_log << error << vfs::Log::endl;
		lua_pop(m_L, 1);
		SGP_THROW(error);
	}
	else
	{
		lua_getglobal(m_L, "InitializeMines");
		if (!lua_isfunction(m_L, -1))
		{
			string error("missing LUA function 'InitializeMines'");
			m_log << error << vfs::Log::endl;
			lua_pop(m_L, 1);
			SGP_THROW(error);
		}
		else
			m_initMinesFuncID = luaL_ref(m_L, LUA_REGISTRYINDEX);

		lua_getglobal(m_L, "InitializeHeadMiners");
		if (!lua_isfunction(m_L, -1))
		{
			string error("missing LUA function 'InitializeHeadMiners'");
			m_log << error << vfs::Log::endl;
			lua_pop(m_L, 1);
			SGP_THROW(error);
		}
		else
			m_initMinersFuncID = luaL_ref(m_L, LUA_REGISTRYINDEX);


		lua_getglobal(m_L, "logging");
		if (!lua_isnil(m_L, -1) && lua_isboolean(m_L, -1) && lua_toboolean(m_L, -1))
			m_log.ToggleLogging(true);
		lua_pop(m_L, 1);
	}
}


void LuaMines::Reset()
{
	gMineStatus.clear();
	gHeadMinerData.clear();

	// just to be sure
	ShutdownStaticExternalNPCFaces();
	InitalizeStaticExternalNPCFaces();
}


void LuaMines::InitMinerFaces()
{
	if (gHeadMinerData.size() > 0)
	{
		for (vector<HEAD_MINER_TYPE>::iterator miner = gHeadMinerData.begin();
			miner != gHeadMinerData.end(); ++miner)
		{
			// load external face
			UINT32 extFaceIndex = static_cast<UINT32>(
				InitFace(static_cast<UINT8>(miner->usProfileId), NOBODY, FACE_FORCE_SMALL) );
			if (extFaceIndex != -1)
			{
				miner->ubExternalFaceIndex = uiExternalStaticNPCFaces.size();
				uiExternalStaticNPCFaces.push_back(extFaceIndex);
			}
			else
				miner->ubExternalFaceIndex = -1;
		}
	}
}


void LuaMines::InitializeMines()
{
	Reset();

	m_log << "Calling 'InitializeMines'" << vfs::Log::endl;
	
	// WANNE: Init mines in a multiplayer game, only if we already received the server game difficulty level (ubDifficultyLevel > 0)!!
	// set global, maybe pass as parameter instead?
	if (!is_networked || gGameOptions.ubDifficultyLevel > 0)
	{
		AssertT(gGameOptions.ubDifficultyLevel > 0);
			lua_pushinteger(m_L, gGameOptions.ubDifficultyLevel);

		lua_setglobal(m_L, "newDIFFICULTY_LEVEL");
		m_log << "global newDIFFICULTY_LEVEL=" << static_cast<int>(gGameOptions.ubDifficultyLevel) << vfs::Log::endl;

		// quick fix for ini override option
		lua_pushinteger(m_L, gGameExternalOptions.bWhichMineRunsOut);
		lua_setglobal(m_L, "iniWHICH_MINE_SHUTS_DOWN");
		m_log << "global iniWHICH_MINE_SHUTS_DOWN=" << static_cast<int>(gGameExternalOptions.bWhichMineRunsOut) << vfs::Log::endl;


		// load and call lua function
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_initMinesFuncID);
		if (lua_pcall(m_L, 0, 1, 0) != 0)
		{
			string error(lua_tostring(m_L, -1));
			lua_pop(m_L, 1);
			SGP_THROW(error);
		}


		// The script is supposed to return an array of tables.
		// Iterate over each table.
		int currentTable = 1;	// first table, lua indexing is 1-based
		while (1)
		{
			MINE_STATUS_TYPE mineStatus = MINE_STATUS_TYPE();

			// load table
			lua_pushinteger(m_L, currentTable);
			lua_gettable(m_L, -2);
	        
			if (lua_isnil(m_L, -1))
				// done, no more tables
				break;


			lua_getfield(m_L, -1, "Location");
			string loc = lua_tostring(m_L, -1);
			lua_pop(m_L, 1);
	        
			lua_getfield(m_L, -1, "Type");
			mineStatus.ubMineType = lua_tointeger(m_L, -1);
			lua_pop(m_L, 1);
	        
			lua_getfield(m_L, -1, "MaxRemovalRate");
			mineStatus.uiMaxRemovalRate = lua_tointeger(m_L, -1);
			lua_pop(m_L, 1);
	        
			lua_getfield(m_L, -1, "RemainingOreSupply");
			mineStatus.uiRemainingOreSupply = lua_tointeger(m_L, -1);
			lua_pop(m_L, 1);

			lua_getfield(m_L, -1, "Infectible");
			mineStatus.fInfectible = lua_tointeger(m_L, -1) != 0;
			lua_pop(m_L, 1);


			lua_getfield(m_L, -1, "AssociatedUnderground");
			if (!lua_isnil(m_L, -1))
			{
				// again, this field is a table/array
				int currentSubTable = 1; // first lua index
				while (1)
				{
					// get next array element
					lua_pushinteger(m_L, currentSubTable);
					lua_gettable(m_L, -2);
					if (lua_isnil(m_L, -1))
						// done
						break;
					string str(lua_tostring(m_L, -1));

					AssociatedMineSector sector;
					if (locationStringToCoordinates(str, &sector.x, &sector.y, &sector.z))
					{
						sector.mineID = gMineStatus.size();
						associatedMineSectors.push_back(sector);
					}

					lua_pop(m_L, 1);
					currentSubTable++;
				}
				lua_pop(m_L, 1);	// pop nil
			}
			lua_pop(m_L, 1);	// pop field or nil value


			if (locationStrToCoords(loc, &mineStatus.sSectorX, &mineStatus.sSectorY))
			{
				// valid location, add new mine
				mineStatus.fEmpty = mineStatus.uiMaxRemovalRate == 0;
				mineStatus.fRunningOut = FALSE;
				mineStatus.fWarnedOfRunningOut = FALSE;
				mineStatus.fShutDown = FALSE;
				mineStatus.fPrevInvadedByMonsters = FALSE;
				mineStatus.fSpokeToHeadMiner = FALSE;
				mineStatus.fMineHasProducedForPlayer = FALSE;
				mineStatus.fQueenRetookProducingMine = FALSE;
				mineStatus.fShutDownIsPermanent = FALSE;

				// this was usually set to 0 except for the mine that is supposed to
				// be running out (in which case the calculation below applies)
				// TODO: see if this works for all mines
				mineStatus.uiOreRunningOutPoint = mineStatus.uiRemainingOreSupply / 4;
				
				// gather associated town id from sector coordinates
				int index = CALCULATE_STRATEGIC_INDEX(mineStatus.sSectorX, mineStatus.sSectorY);
				mineStatus.bAssociatedTown = StrategicMap[index].bNameId;

				gMineStatus.push_back(mineStatus);

				// log
				m_log << "create mine" << vfs::Log::endl;
				m_log << "\tSector: " << string(1, 'A'-1+mineStatus.sSectorY) << mineStatus.sSectorX << vfs::Log::endl;
				m_log << "\tType: " << (mineStatus.ubMineType ? "gold" : "silver") << vfs::Log::endl;
				m_log << "\tMaxRemovalRate: " << mineStatus.uiMaxRemovalRate << vfs::Log::endl;
				m_log << "\tRemainingOreSupply: " << mineStatus.uiRemainingOreSupply << vfs::Log::endl;
				m_log << "\tOreRunningOutPoint: " << mineStatus.uiOreRunningOutPoint << vfs::Log::endl;
				m_log << "\tInfectible: " << (mineStatus.fInfectible ? 1 : 0 ) << vfs::Log::endl;
			}
			else
			{
				m_log << "WARNING: not creating mine #" << currentTable << " due to bad location string ("
					<< loc << ")" << vfs::Log::endl;
			}

			lua_pop(m_L, 1);  // pop this table
			currentTable++;
		}

		// update global
		MAX_NUMBER_OF_MINES = gMineStatus.size();


		lua_pop(m_L, 1);	// pop nil value


		lua_pop(m_L, 1);	// pop return value
	}
}


BOOLEAN LuaMines::InitializeHeadMiners(UINT8 firstMineID)
{
	m_log << vfs::Log::endl;
	m_log << "Calling 'InitializeHeadMiners'" << vfs::Log::endl;

	lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_initMinersFuncID);

	m_log << "parameter 1 ('mines')" << vfs::Log::endl;
	// push parameters
	lua_newtable(m_L);
	for (size_t i = 0; i < gMineStatus.size(); i++)
	{
		int luaIndex = i + 1;
		lua_pushnumber(m_L, luaIndex);

		m_log << "\tindex #" << luaIndex << ": ";

		lua_newtable(m_L);
		{
			MINE_STATUS_TYPE& mine = gMineStatus[i];
			lua_pushstring(m_L, "MaxRemovalRate");
			lua_pushinteger(m_L, mine.uiMaxRemovalRate);

			m_log << "\t\tMaxRemovalRate:" << mine.uiMaxRemovalRate << vfs::Log::endl;

			lua_settable(m_L, -3);
		}
		lua_settable(m_L, -3);
	}

	// add 1 due to lua indexing starting at 1
	int luaFirstMine = firstMineID + 1;
	lua_pushnumber(m_L, luaFirstMine);
	m_log << "parameter 2 ('currentMine'): " << luaFirstMine << vfs::Log::endl;

	if (lua_pcall(m_L, 2, 1, 0) != 0)
	{
		string error(lua_tostring(m_L, -1));
		lua_pop(m_L, 1);
		SGP_THROW(error);
	}

	if (lua_isnil(m_L, -1))
	{
		// lua script did not initialize head miners yet
		lua_pop(m_L, 1); // pop return value (nil)
		m_log << "script returned nil" << vfs::Log::endl;
		return FALSE;
	}

	// The script is supposed to return an array of tables.
	// Iterate over each element.
	int currentTable = 1;
	while (1)
	{
		HEAD_MINER_TYPE headMiner;

		// get this element of result array
		lua_pushinteger(m_L, currentTable);
        lua_gettable(m_L, -2);
    
	    if (lua_isnil(m_L, -1))
			// done, no more elements in result
		    break;


		lua_getfield(m_L, -1, "Profile");
		headMiner.usProfileId = lua_tointeger(m_L, -1);
		lua_pop(m_L, 1);


		lua_getfield(m_L, -1, "Quotes");
		INT8 quotes[4] = { -1, -1, -1, -1 };
		for (int quoteIndex = 0; quoteIndex < 4; quoteIndex++)
		{
			lua_pushinteger(m_L, quoteIndex+1);	// 1-based lua index
			lua_gettable(m_L, -2);

			if (lua_isnil(m_L, -1))
			{
				// not enough quote ids, issue warning?
				lua_pop(m_L, 1);	// pop nil
				break;
			}
			quotes[quoteIndex] = lua_tointeger(m_L, -1);
			lua_pop(m_L, 1);
		}
		memcpy(headMiner.bQuoteNum, quotes, 4);
		lua_pop(m_L, 1);	// pop quotes array


		int mineId = -1;
		lua_getfield(m_L, -1, "MineID");
		if (!lua_isnil(m_L, -1))
			mineId = lua_tointeger(m_L, -1) - 1;	// lua is 1-based, convert to 0-based
		lua_pop(m_L, 1);


		if (mineId > -1 && mineId < int(gMineStatus.size()))
		{
			MINE_STATUS_TYPE* mine = &gMineStatus[mineId];
			MERCPROFILESTRUCT* minerProfile = &gMercProfiles[headMiner.usProfileId];

			// place miner
			minerProfile->bTown = mine->bAssociatedTown;
			minerProfile->sSectorX = mine->sSectorX;
			minerProfile->sSectorY = mine->sSectorY;
			minerProfile->bSectorZ = 0;

			gHeadMinerData.push_back(headMiner);

			// log
			m_log << "create miner" << vfs::Log::endl;
			m_log << "\tProfileID: " << headMiner.usProfileId << vfs::Log::endl;
			m_log << "\tQuotes:";
			for (int i = 0; i < 4; i++)
				m_log << " " << headMiner.bQuoteNum[i];
			m_log << vfs::Log::endl;
			m_log << "\tMineID: " << mineId << vfs::Log::endl;
			m_log << "\tAssociated Town: " << minerProfile->bTown << vfs::Log::endl;
			m_log << "\tSector: " << string(1, 'A'-1+minerProfile->sSectorY) << minerProfile->sSectorX << vfs::Log::endl;
		}
		else
		{
			m_log << "WARNING: not creating miner #" << currentTable << " due to illegal mine id (" << mineId << ")" << vfs::Log::endl;
		}

		lua_pop(m_L, 1);	// pop current element of result table
		currentTable++;
	}
	
	lua_pop(m_L, 1);	// pop nil value

	lua_pop(m_L, 1);	// pop return value

	InitMinerFaces();

	// update global
	NUM_HEAD_MINERS = gHeadMinerData.size();

	return TRUE;
}

LuaMines::~LuaMines()
{
	lua_close(m_L);
}

BOOLEAN LuaMines::Load(HWFILE hFile)
{
	Reset();

	UINT32 uiNumBytesRead;
	INT32 numMines;
	MINE_STATUS_TYPE mineStatus;

	gMineStatus.clear();
	FileRead(hFile, &numMines, sizeof(INT32), &uiNumBytesRead);
	if (uiNumBytesRead != sizeof(INT32))
		return FALSE;

	for (int i = 0; i < numMines; i++)
	{
		FileRead(hFile, &mineStatus, sizeof(MINE_STATUS_TYPE), &uiNumBytesRead);
		if (uiNumBytesRead != sizeof(MINE_STATUS_TYPE))
			return FALSE;
		gMineStatus.push_back(mineStatus);
	}

	MAX_NUMBER_OF_MINES = gMineStatus.size();


	INT32 numSectors;
	AssociatedMineSector sector;
	associatedMineSectors.clear();
	FileRead(hFile, &numSectors, sizeof(INT32), &uiNumBytesRead);
	if (uiNumBytesRead != sizeof(INT32))
		return FALSE;
	for (int i = 0; i < numSectors; i++)
	{
		FileRead(hFile, &sector, sizeof(AssociatedMineSector), &uiNumBytesRead);
		if (uiNumBytesRead != sizeof(AssociatedMineSector))
			return FALSE;
		associatedMineSectors.push_back(sector);
	}


	INT32 numHeadMiners;
	HEAD_MINER_TYPE miner;
	gHeadMinerData.clear();
	FileRead(hFile, &numHeadMiners, sizeof(INT32), &uiNumBytesRead);
	if (uiNumBytesRead != sizeof(INT32))
		return FALSE;
	for (int i = 0; i < numHeadMiners; i++)
	{
		FileRead(hFile, &miner, sizeof(HEAD_MINER_TYPE), &uiNumBytesRead);
		if (uiNumBytesRead != sizeof(HEAD_MINER_TYPE))
			return FALSE;
		gHeadMinerData.push_back(miner);
	}
	NUM_HEAD_MINERS = gHeadMinerData.size();

	InitMinerFaces();

	return TRUE;
}

BOOLEAN LuaMines::Save(HWFILE hFile)
{
	UINT32 uiNumBytesWritten;
	INT32 numMines = gMineStatus.size();

	FileWrite(hFile, &numMines, sizeof(INT32), &uiNumBytesWritten);
	if (uiNumBytesWritten != sizeof(INT32))
		return FALSE;

	for (std::vector<MINE_STATUS_TYPE>::iterator it = gMineStatus.begin();
		it != gMineStatus.end(); ++it)
	{
		FileWrite(hFile, &*it, sizeof(MINE_STATUS_TYPE), &uiNumBytesWritten);
		if (uiNumBytesWritten != sizeof(MINE_STATUS_TYPE))
			return FALSE;
	}


	INT32 numSectors = associatedMineSectors.size();
	FileWrite(hFile, &numSectors, sizeof(INT32), &uiNumBytesWritten);
	if (uiNumBytesWritten != sizeof(INT32))
		return FALSE;

	for (std::vector<AssociatedMineSector>::iterator it = associatedMineSectors.begin();
		it != associatedMineSectors.end(); ++it)
	{
		FileWrite(hFile, &*it, sizeof(AssociatedMineSector), &uiNumBytesWritten);
		if (uiNumBytesWritten != sizeof(AssociatedMineSector))
			return FALSE;
	}


	INT32 numMiners = gHeadMinerData.size();
	FileWrite(hFile, &numMiners, sizeof(INT32), &uiNumBytesWritten);
	if (uiNumBytesWritten != sizeof(INT32))
		return FALSE;

	for (std::vector<HEAD_MINER_TYPE>::iterator it = gHeadMinerData.begin();
		it != gHeadMinerData.end(); ++it)
	{
		FileWrite(hFile, &*it, sizeof(HEAD_MINER_TYPE), &uiNumBytesWritten);
		if (uiNumBytesWritten != sizeof(HEAD_MINER_TYPE))
			return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////
//////////////////////////////////////////////

static bool locationStrToCoords(string str, INT16* x, INT16* y)
{
	if (str.length() < 2 || str.length() > 3)
		return false;

	char yy = tolower(str[0]);
	if (yy < 'a' || yy > 'p')
		return false;
	*y = yy - 'a' + 1;
	
	str = str.substr(1);
	stringstream ss;
	ss << str;
	ss >> *x;
	if (*x < 1 || *x > 16)
		return false;

	return true;
}
