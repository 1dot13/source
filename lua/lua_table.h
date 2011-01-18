#ifndef _LUA_TABLE_H_
#define _LUA_TABLE_H_

#include "lua/lua_state.h"

class LuaTable
{
public:
	/**
	 * Create new table 
	 */
	LuaTable(LuaState L);

	/**
	 * Bind to table at stack offset 'st_off'
	 */
	LuaTable(LuaState L, int st_off);

	/**
	 * Bind to table with global name 'global'
	 */
	LuaTable(LuaState L, const char* global);

	/**
	 * Bind to table value 'index' of table 'table'
	 */
	LuaTable(LuaTable& table, const char* index);

	/**
	 * Bind to table value 'index' of table 'table'
	 */ 
	LuaTable(LuaTable& table, int index);

	////////////////////////////////////////////////////////////////////////

	/**
	 * get value at given index; default implementation for standard types exists
	 * @return true if value at index exists and has required type, false otherwise
	 */
	template<typename T>
	bool getValue(const char* index, T& value);

	/**
	 * get value at given index; default implementation for standard types exists
	 * @return true if value at index exists and has required type, false otherwise
	 */
	template<typename T>
	bool getValue(int index, T& value);

	/**
	 * If value has not standard type, then the default implementation expects it to be user data.
	 * The userdata object itself must inherit from the 'LuaInterface' class.
	 * @return object if it has correct type, NULL otherwise
	 */
	template<typename T>
	T* getValue(const char* index)
	{
		if(is_valid())
		{
			lua_getfield(_L(), _off, index);
			T* value = LuaInterface::luaGetTypedUserData<T>(_L, -1);
			return value;
		}
		return NULL;
	}

	/**
	 * set value at given index 
	 * @return false if table object is invalid, true otherwise
	 */
	template<typename T>
	bool setValue(const char* index, T const& value);

	/**
	 * set value at given index 
	 * @return false if table object is invalid, true otherwise
	 */
	template<typename T>
	bool setValue(int index, T const& value);

	////////////////////////////////////////////////////////////////////////

	/**
	 * is it a table at offset '_off'
	 */ 
	bool is_valid();

	/**
	 * returns size of table as determined by the "#" operator
	 */
	int length();

private:
	LuaState	_L;
	int			_off;
};

/////////////////////
// default implementation for 'getValue(const char*, xxx)'
template<> bool LuaTable::getValue<std::string> (const char* index, std::string& value);
template<> bool LuaTable::getValue<double>      (const char* index, double& value);
template<> bool LuaTable::getValue<int>         (const char* index, int& value);
template<> bool LuaTable::getValue<unsigned int>(const char* index, unsigned int& value);
template<> bool LuaTable::getValue<bool>        (const char* index, bool& value);

// default implementation for 'getValue(int, xxx)'
template<> bool LuaTable::getValue<std::string> (int index, std::string& value);
template<> bool LuaTable::getValue<double>      (int index, double& value);
template<> bool LuaTable::getValue<int>         (int index, int& value);
template<> bool LuaTable::getValue<unsigned int>(int index, unsigned int& value);
template<> bool LuaTable::getValue<bool>        (int index, bool& value);

/////////////////////
// default implementation for 'setValue(const char*, xxx)'
template<> bool LuaTable::setValue<std::string> (const char* index, std::string const& value);
template<> bool LuaTable::setValue<double>      (const char* index, double const& value);
template<> bool LuaTable::setValue<int>         (const char* index, int const& value);
template<> bool LuaTable::setValue<unsigned int>(const char* index, unsigned int const& value);
template<> bool LuaTable::setValue<bool>        (const char* index, bool const& value);

// default implementation for 'setValue(int*, xxx)'
template<> bool LuaTable::setValue<std::string> (int index, std::string const& value);
template<> bool LuaTable::setValue<double>      (int index, double const& value);
template<> bool LuaTable::setValue<int>         (int index, int const& value);
template<> bool LuaTable::setValue<unsigned int>(int index, unsigned int const& value);
template<> bool LuaTable::setValue<bool>        (int index, bool const& value);

#endif // _LUA_TABLE_H_
