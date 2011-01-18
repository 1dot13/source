#include "lua_class_interface.h"

#include <Debug.h>

#define ARRAY_INDEX " idx"	// The space is intentional to make this an out-of-band field

luaL_Reg LuaInterface::s_ClassList[] = {
	{ "__index",	LuaInterface::luaGetObject, },
	{ "__newindex",	LuaInterface::luaSetObject, },
	{ NULL, },
};

const char* LuaInterface::luaClassName = "ja2_LuaInterface";

LuaInterface::LuaInterface()
: luaObject(NULL)
{
}

LuaInterface::~LuaInterface()
{
}

LuaInterface::SInterfaceData* LuaInterface::luaGetInterface()
{
	return NULL;
}


void* LuaInterface::luaIsOfType(LuaState state, int index, const char* sClassName)
{
	lua_State* L = state();
	void *p = lua_touserdata(L, index);
	LuaInterface::SInterfaceData* iface = NULL;
	if( lua_getmetatable(L, index) ) /* does it have a metatable? */
	{
		lua_getfield(L, -1, "__ClassInterface");
		iface = (LuaInterface::SInterfaceData*) lua_touserdata(L, -1);
		if( !iface )
		{
			return NULL;
		}
		lua_pop(L,2);
	}
	else
	{
		return NULL;
	}
	LuaInterface::SInterfaceData::tClassInterface::iterator it = iface->ClassHierarchy.begin();
	for(; it != iface->ClassHierarchy.end(); ++it)
	{
		if( *it == sClassName )
		{
			return p;
		}
	}
	return NULL;
}

LuaInterface::SInterfaceData* LuaInterface::luaGetExportedInterface( SInterfaceData* iface )
{
	//iface.Attribs.push_back(s_attrClassList);
	//iface.ClassHierarchy.push_back(LuaInterface::luaClassName);
	return iface;
}

void LuaInterface::luaExportInterface(LuaState state, LuaInterface::SInterfaceData& iface )
{
	luaL_Reg *i;
	LuaAttrib *idx;
	lua_State* L = state();

	LuaInterface::SInterfaceData::tClassInterface::iterator it_class = iface.ClassHierarchy.begin();

	luaL_newmetatable( L, (*it_class).c_str() );
	for (i = s_ClassList; i->name; i++)
	{
		lua_pushstring(L, i->name);
		lua_pushlightuserdata(L, &iface);
		lua_pushcclosure(L, i->func, 1);
		lua_rawset(L, -3);
	}
	lua_pushlightuserdata(L, &iface);
	lua_setfield(L, -2, "__ClassInterface");

	LuaInterface::SInterfaceData::tAttribs::iterator it_attrib = iface.Attribs.begin();
	for(;it_attrib != iface.Attribs.end(); ++it_attrib)
	{
		for (idx=*it_attrib; idx->name; idx++)
		{
			if (idx->staticcall)
			{
				lua_pushstring(L, idx->name);
				lua_pushcfunction(L, idx->staticcall);
				lua_rawset(L, -3 );
			}
		}
	}
}

LuaAttrib* LuaInterface::luaCommonGetSet(LuaState state)
{
	const char *AttrToFind;
	typedef std::list<LuaAttrib*> tAttributes;
	lua_State* L = state();

	LuaInterface::SInterfaceData* iface = (LuaInterface::SInterfaceData*) lua_touserdata(L, lua_upvalueindex( 1));
	if(!iface)
	{
		return NULL;
	}
	tAttributes* Attributes = &iface->Attribs;

	if (lua_isnumber(L, 2) )
	{
		AttrToFind = ARRAY_INDEX;
	}
	else if (lua_isstring(L, 2) )
	{
		AttrToFind = lua_tostring(L, 2);
	}
	else
	{
		luaL_argcheck(L, 0, 2, "Invalid index to the table" );
		// The last function doesn't return, but gotta keep the compiler happy
		return NULL;
	}

	tAttributes::iterator it = Attributes->begin();
	for(;it != Attributes->end(); ++it)
	{
		LuaAttrib* attrib = (*it);
		for ( ; attrib->name; attrib++ )
		{
			if ( strcmp( attrib->name, AttrToFind) == 0 )
			{
				return attrib;
			}
		}
	}
	std::string err = "No such field : " + std::string(AttrToFind);
	luaL_error(L, err.c_str());
	return NULL;
}

//int LuaInterface::luaGetObject(lua_State* L)
DEFINE_LUA_CFUNCTION(LuaInterface, luaGetObject)
{
	LuaAttrib *Attributes;

	Attributes = LuaInterface::luaCommonGetSet(L);
	if (!Attributes)
	{
		lua_pushnil( L );
		return 1;
	}

	if (!Attributes->get)
	{
		if (!Attributes->call)
		{
			lua_pushnil( L );
		}
		else
		{
			lua_pushcfunction(L, Attributes->call);
		}
		return 1;
	}

	return Attributes->get( L );
}

//int LuaInterface::luaSetObject(lua_State* L)
DEFINE_LUA_CFUNCTION(LuaInterface, luaSetObject)
{
	LuaAttrib *Attributes;

	Attributes = LuaInterface::luaCommonGetSet(L);

	if (!Attributes)
	{
		lua_rawset( L, 1 );
		return 0;
	}

	if (!Attributes->set)
	{
		luaL_error( L, "The attribute is not writable" );
	}

	return Attributes->set(L);
}


void LuaInterface::luaCreateObject( LuaState state, void *Ptr )
{
	lua_State* L = state();
	// Create the object and initialize its pointer
	void **ud;
	ud = (void**)lua_newuserdata( L, sizeof( void*) );
	*ud = Ptr;
	// save user data
	state.registry().GetUserDataTable();
	lua_pushlightuserdata(L, this);
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);
	// Swap the table and metatable for stack arg purposes
	lua_insert( L, -2 );
	lua_setmetatable( L, -2 );
}

void LuaInterface::luaPushObject( LuaState state, const char* sClassName, void *Ptr )
{
	lua_State* L = state();
	state.registry().GetUserDataTable();
	lua_pushlightuserdata(L, Ptr);
	lua_rawget(L,-2);
	if(lua_isnil(L,-1))
	{
		lua_pop(L, 1);
		luaL_getmetatable( L, sClassName );
		if (lua_istable( L, -1 ) )
		{
			this->luaCreateObject( state, Ptr );
		}
	}
	if(!lua_isuserdata(L,-1))
	{
		SGP_THROW(L"Could not create User Data");
	}
	lua_insert(L, -2);
	lua_pop(L, 1);
}

void LuaInterface::luaPushObject( LuaState state, const char* luaName )
{
	LuaInterface::SInterfaceData* iface = this->luaGetInterface();
	this->luaPushObject(state, iface->ClassHierarchy.front().c_str(), this);
	if(luaName != NULL)
	{
		lua_setglobal( state(), luaName );
	}
}


