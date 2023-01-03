#ifndef _LUA_STATE_H_
#define _LUA_STATE_H_

#include <Lua Interpreter.h>

namespace lua
{
	enum State
	{
		LUA_STATE,
		LUA_STATE_STRUCTURE,
		LUA_STATE_GUI,

		LUA_STATE_STRATEGIC_MINES_AND_UNDERGROUND,
	};
};

class LuaState
{
	class Registry
	{
	public:
		void CreateCallbackTable();
		void GetCallbackTable();

		void SetCallback(const char* callback_name);
		void GetCallback(const char* callback_name);

		void CreateUserDataTable();
		void GetUserDataTable();
	private:
		friend class LuaState;
		Registry(lua_State* L);
		lua_State* _L;
	};
public:
	/**
	 * takes over raw lua_State
	 * @param state pointer to lua_State
	 * @param own takes over raw state when true (= closes it)
	 */
	LuaState(lua_State* state, bool own=false);

	/**
	 * default ctor, does nothing other than allowing uninitialized variables
	 */
	LuaState();

	/**
	 * default dtor
	 */
	~LuaState();

	/**
	 * initialize longterm lua state by its ID
	 */
	static void			INIT(lua::State state_id, bool init_libs);
	
	/**
	 * initializes and returns shortterm lua state
	 */
	static LuaState		INIT(bool init_libs=false);

	/**
	 * close longterm lua state
	 */
	static void			CLOSE(lua::State state_id);

	/**
	 * close shortterm lua state
	 */
	static void			CLOSE(LuaState state);

	/**
	 * get lua state after it was initialized (right before or in some other place)
	 */
	static LuaState		GET(lua::State state_id);

	/**
	 * returns raw lua state
	 */
	inline lua_State* operator()() { return _L; }

	/**
	 * return registry object associated with this lua state
	 */
	inline Registry& registry() { return _registry; }

	/**
	 * loads and evaluates lua file. catches and logs error if file doesn't exist or could not be read.
	 * @return true if file could be loaded and evaluated, false otherwise. 
	 */
	bool EvalFile(const char* filename);

	/**
	 * evaluate string
	 */ 
	bool EvalString(const char* lua_str, size_t length, const char* id_str);
	/**
	 * evaluate string
	 */ 
	bool EvalString(const wchar_t* lua_str, size_t length, const char* id_str);

	/**
	 * deprecated (i guess), use LuaFunction instead
	 */
	bool CallFunction(const char* function_name);
private:
	/**
	 * creates lua state and initializes additional libraries
	 * @param load_libs opens additional libraries if true
	 */
	void init(bool load_libs);

	/**
	 * closes lua state
	 */
	void close();
private:
	lua_State*		_L;
	bool			_own;
	Registry		_registry;
};

class LuaScopeState
{
public:
	LuaState L;

	LuaScopeState(bool init_libs) : L(LuaState::INIT(init_libs))
	{
	}

	~LuaScopeState()
	{
		LuaState::CLOSE(L);
	}
};

#endif // _LUA_STATE_H_
