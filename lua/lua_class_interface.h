#ifndef _LUA_CLASS_INTERFACE_H_
#define _LUA_CLASS_INTERFACE_H_

//#include "Lua Interpreter.h"
#include "lua_state.h"

#include <string>
#include <list>

#define DECLARE_LUA_CFUNCTION(name)				static int name(lua_State* L)
#define DEFINE_LUA_CFUNCTION(class_name,name)	int class_name::name(lua_State* L)

class LuaInterface
{
public:
	struct SInterfaceData
	{
		typedef std::list<LuaAttrib*>	tAttribs;
		typedef std::list<std::string>	tClassInterface;
		tAttribs						Attribs;
		tClassInterface					ClassHierarchy;
	};

	static const char* luaClassName;
public:
	LuaInterface();
	virtual ~LuaInterface();

	virtual void			luaPushObject( LuaState L,  const char* luaName = NULL );

protected:
	static void				luaExportInterface(LuaState L, LuaInterface::SInterfaceData& iface);
	static SInterfaceData*	luaGetExportedInterface( SInterfaceData* iface=NULL);

	virtual SInterfaceData*	luaGetInterface();

	void					luaPushObject( LuaState L, const char* sClassName, void *Ptr );
private:
	void					luaCreateObject( LuaState L, void *Ptr );
	void*					luaObject;
	
	static LuaAttrib*		luaCommonGetSet( LuaState L );
	static void*			luaIsOfType(LuaState L, int index, const char* sClassName);

	// "meta"-interface
	//static int				luaGetObject( lua_State *L);
	//static int				luaSetObject( lua_State *L);
	DECLARE_LUA_CFUNCTION( luaGetObject );
	DECLARE_LUA_CFUNCTION( luaSetObject );
	
	static luaL_Reg			s_ClassList[];
public:
	/////////////////////////////////////////////////////////////////////////
	template<typename T>
	static void luaExportClass( LuaState L )
	{
		LuaInterface::SInterfaceData* iface = T::luaGetExportedInterface();
		LuaInterface::luaExportInterface(L, *iface);
	}

	template<typename T>
	static T* luaGetTypedUserData(LuaState L, int index)
	{
		void *ptr = LuaInterface::luaIsOfType(L, index, T::luaClassName);
		if( !ptr )
		{
			luaL_typerror(L(), index, T::luaClassName);
		}

		T** ppNode = (T**)(ptr);
		if(ppNode)
		{
			return *ppNode;
		}
		return NULL;
	}
};

class LuaUserObject
{
public:
	LuaUserObject(LuaInterface* luaObject=NULL) : _luaObject(luaObject) {};
	void PushObject(LuaState state, const char* luaName = NULL)
	{
		if(_luaObject) _luaObject->luaPushObject(state, luaName);
	}
private:
	LuaInterface* _luaObject;
};

#ifndef DECLARE_LUA_INTERFACE
#define DECLARE_LUA_INTERFACE(BaseClass) \
	private: \
		typedef BaseClass tSuperClass; \
	public: \
		static	const char*		luaClassName; \
		static	SInterfaceData*	luaGetExportedInterface(SInterfaceData* iface=NULL); \
	protected: \
		virtual SInterfaceData*	luaGetInterface(); \
	private: \
		static	SInterfaceData	luaInterface; 
#endif // DECLARE_LUA_INTERFACE

#ifndef IMPLEMENT_LUA_INTERFACE
#define IMPLEMENT_LUA_INTERFACE(className, classLuaName, AttribArray) \
	const char* className::luaClassName = classLuaName; \
	LuaInterface::SInterfaceData className::luaInterface; \
	LuaInterface::SInterfaceData* className::luaGetInterface() \
	{ \
		return &(className::luaInterface); \
	} \
	LuaInterface::SInterfaceData* className::luaGetExportedInterface(LuaInterface::SInterfaceData* iface) \
	{ \
		if(!iface) iface = &(className::luaInterface); \
		iface->ClassHierarchy.push_back(className::luaClassName); \
		iface->Attribs.push_back(AttribArray); \
		return tSuperClass::luaGetExportedInterface( iface ); \
	}
#endif // IMPLEMENT_LUA_INTERFACE

#endif // _LUA_CLASS_INTERFACE_H_

