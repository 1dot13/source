#ifndef STRATEGIC_MINES_LUA_H
#define STRATEGIC_MINES_LUA_H

#include "Types.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "UndergroundInit.h"

//#include <vfs/Tools/vfs_log.h>
//class LuaLogger
//{
//	bool m_active;
//	vfs::Log& m_log;
//
//public:
//	enum LoggingState
//	{
//		OFF,
//		ON,
//	};
//
//	LuaLogger(const vfs::Path filename, bool enable = true, bool append = false, vfs::Log::EFlushMode flushMode = vfs::Log::FLUSH_ON_ENDL)
//		: m_log(*vfs::Log::create(filename, append, flushMode)), m_active(enable) {}
//	~LuaLogger() { Flush(); }
//	void ToggleLogging(bool enable) { m_active = enable; }
//	void Flush() { m_log.flush(); }
//
//	template <typename T> LuaLogger& operator<<(const T& message) { if (m_active) m_log << message; return *this; }
//	template <> LuaLogger& operator<<(const vfs::Log::_endl& endl) { if (m_active) m_log << vfs::Log::endl; return *this; }
//	template <> LuaLogger& operator<<(const LoggingState& state) { m_active = state == ON; return *this; }
//	
//};

class LuaMines
{
public:
	void LoadScript();
	void InitializeMines();
	BOOLEAN InitializeHeadMiners(UINT8 firstMineID);

	BOOLEAN Save(HWFILE hFile);
	BOOLEAN Load(HWFILE hFile);

	LuaMines();
	virtual ~LuaMines();

private:
	void Reset();
	void InitMinerFaces();

	int m_initMinesFuncID;
	int m_initMinersFuncID;
	lua_State * m_L;

	LuaLogger m_log;
};

extern LuaMines g_luaMines;


#endif
