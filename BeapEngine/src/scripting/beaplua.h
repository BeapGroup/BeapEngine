#pragma once
#include <string>
#include <iostream>

// Include Lua
extern "C" {
#include "../thirdparty/lua/include/lua.h"
#include "../thirdparty/lua/include/lauxlib.h"
#include "../thirdparty/lua/include/lualib.h"
}

namespace beap::lua {

	class LuaInterpreter {
	public:
		lua_State* L = luaL_newstate();

		int ExecuteString(std::string const& command);
		float GetNumber(std::string const& varname);
		std::string GetString(std::string const& varname);

		void ReadFromStack();
		void RegisterFunctions();

		LuaInterpreter() = default;
		LuaInterpreter(bool provide_libs, bool incl_env);

	private:

	};
}
