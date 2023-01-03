#include "lua/lua_table.h"

/*
 *  Table constructors
 */

LuaTable::LuaTable(LuaState L)
: _L(L)
{
	lua_newtable(_L());
	_off = -1;
}

LuaTable::LuaTable(LuaState L, int st_off)
: _L(L), _off(st_off)
{};

LuaTable::LuaTable(LuaState L, const char* global)
: _L(L)
{
	lua_getglobal(_L(), global);
	_off = -1;
};

LuaTable::LuaTable(LuaTable& table, const char* index)
: _off(-1)
{
	_L = table._L;
	lua_State* L = _L();
	lua_getfield(L, table._off, index);
	if(!is_valid())
	{
		lua_pop(L,1);
	}
}

LuaTable::LuaTable(LuaTable& table, int index)
: _off(-1)
{
	_L = table._L;
	lua_State* L = _L();
	lua_pushinteger(L, index);
	lua_gettable(L, table._off < 0 ? table._off-1 : table._off);
	if(!is_valid())
	{
		lua_pop(L,1);
	}
}

/////////////////////////////////////////////////////////////////////////////////

template<>
bool LuaTable::getValue<std::string>(const char* index, std::string& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_getfield(L, _off, index);
		if(lua_isstring(L, -1))
		{
			value = lua_tostring(L,-1);
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

template<>
bool LuaTable::getValue<double>(const char* index, double& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_getfield(L, _off, index);
		if(lua_isnumber(L, -1))
		{
			value = lua_tonumber(L,-1);
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

template<>
bool LuaTable::getValue<int>(const char* index, int& value)
{
	double tmp;
	if( getValue<double>(index,tmp) )
	{
		value = (int)tmp;
		return true;
	}
	return false;
}
template<>
bool LuaTable::getValue<unsigned int>(const char* index, unsigned int& value)
{
	double tmp;
	if( getValue<double>(index,tmp) )
	{
		value = (unsigned int)tmp;
		return true;
	}
	return false;
}

template<>
bool LuaTable::getValue<bool>(const char* index, bool& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_getfield(L, _off, index);
		if(lua_isboolean(L, -1))
		{
			value = lua_toboolean(L,-1) != 0;
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

/////

template<>
bool LuaTable::getValue<std::string>(int index, std::string& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_pushinteger(L, index);
		lua_gettable(L, _off < 0 ? _off-1 : _off);
		if(lua_isstring(L, -1))
		{
			value = lua_tostring(L,-1);
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

template<>
bool LuaTable::getValue<double>(int index, double& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_pushinteger(L, index);
		lua_gettable(L, _off < 0 ? _off-1 : _off);
		if(lua_isnumber(L, -1))
		{
			value = lua_tonumber(L,-1);
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

template<>
bool LuaTable::getValue<int>(int index, int& value)
{
	double tmp;
	if( getValue<double>(index,tmp) )
	{
		value = (int)tmp;
		return true;
	}
	return false;
}
template<>
bool LuaTable::getValue<unsigned int>(int index, unsigned int& value)
{
	double tmp;
	if( getValue<double>(index,tmp) )
	{
		value = (unsigned int)tmp;
		return true;
	}
	return false;
}

template<>
bool LuaTable::getValue<bool>(int index, bool& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		lua_pushinteger(L, index);
		lua_gettable(L, _off < 0 ? _off-1 : _off);
		if(lua_isboolean(L, -1))
		{
			value = lua_toboolean(L,-1) != 0;
			lua_pop(L,1);
			return true;
		}
		lua_pop(L,1);
	}
	return false;
}

/////////////////////////////////

template<>
bool LuaTable::setValue<std::string>(const char* index, std::string const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushstring(L, index);
		lua_pushstring(L, value.c_str());
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

template<>
bool LuaTable::setValue<double>(const char* index, double const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushstring(L, index);
		lua_pushnumber(L, value);
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

template<>
bool LuaTable::setValue<int>(const char* index, int const& value)
{
	return setValue<double>(index, (double)value);
}
template<>
bool LuaTable::setValue<unsigned int>(const char* index, unsigned int const& value)
{
	return setValue<double>(index, (double)value);
}

template<>
bool LuaTable::setValue<bool>(const char* index, bool const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushstring(L, index);
		lua_pushboolean(L, value);
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

/////////////////////////////////////

template<> 
bool LuaTable::setValue<std::string>(int index, std::string const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushinteger(L, index);
		lua_pushstring(L, value.c_str());
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

template<>
bool LuaTable::setValue<double>(int index, double const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushinteger(L, index);
		lua_pushnumber(L, value);
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

template<>
bool LuaTable::setValue<int>(int index, int const& value)
{
	return setValue<double>(index, (double)value);
}
template<>
bool LuaTable::setValue<unsigned int>(int index, unsigned int const& value)
{
	return setValue<double>(index, (double)value);
}

template<>
bool LuaTable::setValue<bool>(int index, bool const& value)
{
	if(is_valid())
	{
		lua_State* L = _L();
		
		lua_pushinteger(L, index);
		lua_pushboolean(L, value);
		lua_rawset(L, _off < 0 ? _off-2 : _off);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////

int LuaTable::length()
{
	return (int)lua_objlen(_L(), _off);
}

bool LuaTable::is_valid()
{
	if(lua_istable(_L(), _off))
	{
		return true;
	}
	return false;
}
