#pragma once
#include <string>
#include <iostream>

// Include Lua
extern "C" {
#include "../thirdparty/lua/include/lua.h"
#include "../thirdparty/lua/include/lauxlib.h"
#include "../thirdparty/lua/include/lualib.h"
}

//#ifdef _WIN32
//#pragma comment(lib, "../thirdparty/lua/lua54.lib")
//#endif

namespace beap::lua {

	class LuaInterpreter {
	public:
		lua_State* L = luaL_newstate();

		int ExecuteString(std::string const& command) {
			return luaL_dostring(L, command.c_str());
		}

		float GetNumber(std::string const& varname) {
			lua_getglobal(L, varname.c_str());
			if (lua_isnumber(L, -1)) {
				return (float)lua_tonumber(L, -1);
			}
		}

		void CheckErrors(int code) {
			if (code != LUA_OK) {
				std::cout << lua_tostring(L, -1) << std::endl;
			}
		}

		LuaInterpreter() = default;
		explicit LuaInterpreter(bool provide_libs, bool incl_env) {
			if (provide_libs) {
				luaL_openlibs(L);
			}
			if (incl_env) {
				luaL_dofile(L, "resources/scripts/main.lua");
			}
		}
	};
}
