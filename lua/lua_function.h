#ifndef _LUA_FUNCTION_H_
#define _LUA_FUNCTION_H_

#include <lua_state.h>
#include <lua_table.h>

#include <stack>

class LuaFunction
{
public:
	LuaFunction(LuaState L, const char* function_name)
		: _function_name(function_name), _L(L()), _counter(0), m_is_function(false)
	{
		lua_getglobal(_L, function_name);
	}

	template<typename T>
	LuaFunction& Param(T const& par);

	template<typename TI, typename TV>
	LuaFunction& TParam(TI const index, TV const& par)
	{
		if(m_is_function || is_function(-2))
		{
			if(!_tables.empty())
			{
				LuaTable& tab = *_tables.top();
				tab.setValue(index, par);
			}
		}
		return *this;
	}

	LuaFunction& TableOpen()
	{
		_tables.push(new LuaTable(_L));
		return *this;
	}
	LuaFunction& TableClose()
	{
		if(!_tables.empty())
		{
			LuaTable* tab = _tables.top();
			_tables.pop();
			if(tab) delete tab;
			_counter++;
		}
		return *this;
	}

	bool Call(int num_ret)
	{
		if(m_is_function || is_function())
		{
			if(_tables.empty() && num_ret >= 0)
			{
				if( lua_pcall(_L, _counter, num_ret, 0) != 0)
				{
					std::string err = lua_tostring(_L, -1);

					std::wstring wstr;
					convert_string(_function_name, wstr);

					std::wstringstream wss;
					wss << L"Execution of lua function '" << wstr << L"' failed : ";

					convert_string(err, wstr);
					wss << wstr;

					SGP_THROW(wss.str().c_str());
				}
				return true;
			}
		}
		return false;
	}

private:
	bool is_function(int pos=-1)
	{
		return m_is_function = lua_isfunction(_L, pos);
	}
private:
	std::string				_function_name;
	lua_State*				_L;
	std::stack<LuaTable*>	_tables;
	int						_counter;
	bool					m_is_function;
};


#endif // _LUA_FUNCTION_H_
