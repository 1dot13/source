#ifndef _UNDERGROUNDINIT_H
#define _UNDERGROUNDINIT_H

#include "Campaign Types.h"
#include "Types.h"
//#include "Strategic Mines LUA.h"

#include "lua_state.h"

//BOOLEAN LetLuaBuildUndergroundSectorInfoList();
BOOLEAN locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z);

#include <vfs/Tools/vfs_log.h>
class LuaLogger
{
	bool m_active;
	vfs::Log& m_log;

public:
	enum LoggingState
	{
		OFF,
		ON,
	};

	LuaLogger(const vfs::Path filename, bool enable = true, bool append = false, vfs::Log::EFlushMode flushMode = vfs::Log::FLUSH_ON_ENDL)
		: m_log(*vfs::Log::create(filename, append, flushMode)), m_active(enable) {}
	~LuaLogger() { /*Flush();*/ }
	void ToggleLogging(bool enable) { m_active = enable; }
	void Flush() { m_log.flush(); }

	template <typename T> LuaLogger& operator<<(const T& message) { if (m_active) m_log << message; return *this; }
	template <> LuaLogger& operator<<(const vfs::Log::_endl& endl) { if (m_active) m_log << vfs::Log::endl; return *this; }
	template <> LuaLogger& operator<<(const LoggingState& state) { m_active = state == ON; return *this; }
	
};

class LuaUnderground
{
public:
	void LoadScript(const char * langPrefix);

	BOOLEAN InitializeSectorList();
	void GetLoadscreen(INT16 x, INT16 y, INT16 z, std::string& buffer, std::string& format);
	void GetSectorName(INT16 x, INT16 y, INT16 z, const UNDERGROUND_SECTORINFO* pSector, STR16 buffer, size_t bufSizeInWChar, BOOLEAN fDetailed);

	LuaUnderground();
	virtual ~LuaUnderground();
private:
	LuaState GetLuaState()
	{
		return LuaState::GET(lua::LUA_STATE_STRATEGIC_MINES_AND_UNDERGROUND);
	}
	void Reset();
	bool RunScript(LuaState& L, const char * filename);
	
	LuaLogger m_log;
};

extern LuaUnderground g_luaUnderground;

#endif
