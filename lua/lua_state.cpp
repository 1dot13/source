#include "lua/lua_state.h"
#include <Debug.h>
#include "sgp_logger.h"

#include <vfs/Core/vfs_string.h>

LuaState::LuaState(lua_State* state, bool own) : _L(state), _own(own), _registry(state)
{
}

LuaState::LuaState() : _L(NULL), _own(true), _registry(NULL)
{
	//_L = lua_open();
	//_registry._L = _L;
}

LuaState::~LuaState()
{
}

void LuaState::init(bool init_libs)
{
	if(!_L)
	{
		_L = lua_open();
		if(init_libs)
		{
			luaL_openlibs(_L);
		}
		_registry._L = _L;
	}
}

void LuaState::close()
{
	if(_L && _own)
	{
		lua_close(_L);
	}
}

#include <vfs/Core/vfs_file_raii.h>
bool LuaState::EvalFile(const char* filename)
{
	try
	{
		vfs::COpenReadFile rfile(filename);
		vfs::UInt32 size = rfile.file().getSize();
		std::vector<vfs::Byte> buffer(size+1);
		rfile.file().read(&buffer[0], size);
		buffer[size] = 0;
		return this->EvalString((char*)&buffer[0], size, filename);
	}
	catch(std::exception &ex)
	{
		SGP_ERROR(ex.what());
	}
	return false;
}

bool LuaState::CallFunction(const char* function_name)
{
	if(function_name)
	{
		// call function
		lua_getglobal(_L, function_name);
		if(lua_isfunction(_L, -1))
		{
			if( lua_pcall(_L, 0, 1, 0) != 0 )
			{
				std::string err = lua_tostring(_L, -1);
				std::wstringstream wss;
				wss << L"Execution of lua function '" << vfs::String::as_utf16(function_name) << L"' failed : ";
				wss << vfs::String::as_utf16(err);
				SGP_THROW(wss.str().c_str());
			}
			return true;
		}
	}
	return false;
}


bool LuaState::EvalString(const wchar_t *lua_str, size_t length, const char* id_str)
{
	std::string s = vfs::String::as_utf8(lua_str, length);

	return this->EvalString(s.c_str(), s.length(), id_str);
}

bool LuaState::EvalString(const char *lua_str, size_t length, const char* id_str)
{
	int error;

	if(length == 0)
	{
		length = strlen(lua_str);
	}
	error = luaL_loadbuffer(_L, lua_str, length, id_str) || 
		lua_pcall(_L, 0, 0, 0);

	if (error)
	{
		const char *error = lua_tostring(_L, -1);
		int len = strlen( error);
		if (len >= 7 && !strcmp( error + len - 7, "'<eof>'"))
		{
			lua_pop(_L, 1);	/* pop error message from the stack */
			return false;
		}
		std::string s = lua_tostring(_L, -1);
		SGP_THROW(s);
		//printf( "%s\n", lua_tostring(L, -1));
		//lua_pop(L, 1);	/* pop error message from the stack */
		//return true;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////

LuaState::Registry::Registry(lua_State* L) : _L(L)
{
}

void LuaState::Registry::CreateCallbackTable()
{
	lua_getfield(_L, LUA_REGISTRYINDEX, "__CallBackRegistry");
	if(lua_isnil(_L,-1))
	{
		lua_pop(_L,1); // pop 'nil' from stack
		lua_newtable(_L);
		lua_setfield(_L,LUA_REGISTRYINDEX,"__CallBackRegistry"); // 'setfield' removes value from stack
		return;
	}
	else if(lua_istable(_L,-1))
	{
		lua_pop(_L,1);
		return;
	}
	SGP_THROW(L"Could not create callback registy table");
}
void LuaState::Registry::GetCallbackTable()
{
	lua_getfield(_L, LUA_REGISTRYINDEX, "__CallBackRegistry");
	if(!lua_istable(_L,-1))
	{
		SGP_THROW("CallBack registry table does not exist");
	}
}

void LuaState::Registry::SetCallback(const char* callback_name)
{
	// function is at (L, -2)
	// user data value is at (L, -1)
	this->GetCallbackTable();
	lua_pushvalue(_L, -2);
	lua_rawget(_L, -2);
	if(!lua_istable(_L, -1))
	{
		lua_pop(_L, 1); // remove 'nil'
		// set new table
		lua_pushvalue(_L, -2);
		lua_newtable(_L);
		lua_rawset(_L, -3);
		// get table
		lua_pushvalue(_L, -2);
		lua_rawget(_L, -2);
	}
	if(!lua_istable(_L,-1))
	{
		SGP_THROW("Could not create callback");
	}
	lua_pushvalue(_L, -4);
	lua_setfield(_L, -2, callback_name);
	lua_pop(_L, 4);
}
void LuaState::Registry::GetCallback(const char* callback_name)
{
	// user data is at (L, -1)
	this->GetCallbackTable();
	lua_pushvalue(_L, -2);
	lua_rawget(_L, -2);
	if(lua_istable(_L, -1))
	{
		lua_getfield(_L, -1, callback_name);
		if(lua_isfunction(_L, -1))
		{
			lua_insert(_L, -4);
			lua_pop(_L, 3);
			if(!lua_isfunction(_L,-1))
			{
				SGP_THROW("messed up stack");
			}
			return;
		}
		// no callback defined
		lua_pop(_L, 4);
	}
	// no table created
	lua_pop(_L, 3); // also pops user data
	return;
}


void LuaState::Registry::CreateUserDataTable()
{
	lua_getfield(_L, LUA_REGISTRYINDEX, "__UserData");
	if(lua_isnil(_L,-1))
	{
		lua_pop(_L,1); // pop 'nil' from stack
		lua_newtable(_L);
		lua_setfield(_L,LUA_REGISTRYINDEX,"__UserData"); // 'setfield' removes value from stack
		return;
	}
	else if(lua_istable(_L,-1))
	{
		lua_pop(_L,1);
		return;
	}
	SGP_THROW(L"Could not create User Data registy table");
}

void LuaState::Registry::GetUserDataTable()
{
	lua_getfield(_L, LUA_REGISTRYINDEX, "__UserData");
	if(!lua_istable(_L,-1))
	{
		SGP_THROW("User Data registry table does not exist");
	}
}


/************************************************************************************/

#include <map>

class LuaStateManager
{
public:
	~LuaStateManager() {
		_states.clear();
	};

	static LuaStateManager& instance() {
		static LuaStateManager lsm;
		return lsm;
	};

	LuaState& GetState(lua::State eState) {
		return _states[eState];
	}

private:
	LuaStateManager() {};
	static std::map<lua::State, LuaState> _states;
};

std::map<lua::State, LuaState> LuaStateManager::_states;


void LuaState::INIT(lua::State state_id, bool init_libs)
{
	LuaStateManager::instance().GetState(state_id).init(init_libs);
}

LuaState LuaState::INIT(bool init_libs)
{
	LuaState L;
	L.init(init_libs);
	return L;
}

void LuaState::CLOSE(lua::State state_id)
{
	LuaStateManager::instance().GetState(state_id).close();
}

void LuaState::CLOSE(LuaState state)
{
	state.close();
}

LuaState LuaState::GET(lua::State state_id)
{
	return LuaStateManager::instance().GetState(state_id);
}





