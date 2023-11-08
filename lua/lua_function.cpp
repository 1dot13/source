#include "DEBUG.H"
#include "lua_function.h"

template<>
LuaFunction& LuaFunction::Param<std::string>(std::string const& par)
{
	if(m_is_function || is_function())
	{
		lua_pushstring(_L, par.c_str());
		_counter++;
	}
	return *this;
}

template<>
LuaFunction& LuaFunction::Param<double>(double const& par)
{
	if(m_is_function || is_function())
	{
		lua_pushnumber(_L, par);
		_counter++;
	}
	return *this;
}

template<>
LuaFunction& LuaFunction::Param<int>(int const& par)
{
	if(m_is_function || is_function())
	{
		lua_pushinteger(_L, par);
		_counter++;
	}
	return *this;
}

template<>
LuaFunction& LuaFunction::Param<unsigned int>(unsigned int const& par)
{
	if(m_is_function || is_function())
	{
		lua_pushnumber(_L, (double)par);
		_counter++;
	}
	return *this;
}

template<>
LuaFunction& LuaFunction::Param<bool>(bool const& par)
{
	if(m_is_function || is_function())
	{
		lua_pushboolean(_L, par);
		_counter++;
	}
	return *this;
}
