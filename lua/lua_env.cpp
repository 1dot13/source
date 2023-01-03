#include "Lua Interpreter.h"
#include "environment.h"

#define ENVIRONMENT_CLASS "ja2_EnvironmentClass"

static int LuaGetRainVal( lua_State *L )
{
	lua_pushinteger( L, GetWeatherInCurrentSector() );
	return 1;
}

static int LuaSetRainVal( lua_State *L )
{
	int newweather = luaL_checkinteger( L, 3);
	int sector = luaL_checkinteger( L, 4 );
	luaL_argcheck( L, newweather >= 0 && newweather < WEATHER_FORECAST_MAX, 2, "The rain must be between 0 and 5" );
	luaL_argcheck( L, sector >= 0 && sector <= 255, 2, "The secor must be between 0 and 255" );

	ChangeWeather( sector, newweather );

	return 0;
}

static int LuaGetAmbient( lua_State *L )
{
	lua_pushinteger( L, gubEnvLightValue);
	return 1;
}

static int LuaSetAmbient( lua_State *L )
{
	int newambient = luaL_checkinteger( L, 3);
	luaL_argcheck( L, newambient >= 0 && newambient <= 255, 2, "The ambience must be between 0 and 255" );
	gubEnvLightValue = (UINT8)newambient;
	if( !gfBasement && !gfCaves )
		gfDoLighting		 = TRUE;
	return 0;
}

static int LuaSetNightLights( lua_State *L )
{
	int newprime = luaL_checkinteger( L, 1);
	if (newprime)
	{
		TurnOnNightLights();
	}
	else
	{
		TurnOffNightLights();
	}
	return 0;
}

static int LuaSetPrimeLights( lua_State *L )
{
	int newprime = luaL_checkinteger( L, 1);
	if (newprime)
	{
		TurnOnPrimeLights();
	}
	else
	{
		TurnOffPrimeLights();
	}
	return 0;
}

static LuaAttrib Environment[] = {
	{ "rain", LuaGetRainVal, LuaSetRainVal },
	{ "ambient", LuaGetAmbient, LuaSetAmbient },
	{ "nightlights", NULL, NULL, LuaSetNightLights },
	{ "primelights", NULL, NULL, LuaSetPrimeLights },
	{ NULL, },
};


void LuaEnvironmentSetup(lua_State *L)
{
	// Create the environment object
	lua_newtable( L);
	CreateLuaClass( L, ENVIRONMENT_CLASS, Environment );
	lua_setmetatable( L, -2);
	lua_setglobal( L, "Environment"); // We also want this class to be known to the script
}
