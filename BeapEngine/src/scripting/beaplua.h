#pragma once
#include <string>
#include <iostream>

// Include Lua
extern "C" {
#include "../thirdparty/lua/include/lua.h"
#include "../thirdparty/lua/include/lauxlib.h"
#include "../thirdparty/lua/include/lualib.h"
}
#include "../hierarchy/instances/instance.h"

namespace beap::lua {

	class LuaInterpreter {
	public:
		lua_State* L = luaL_newstate();

		int ExecuteString(std::string const& command);
		float GetNumber(std::string const& varname);
		std::string GetString(std::string const& varname);

		void ReadFromStack();
		void RegisterCppFunctions();

		LuaInterpreter() = default;
		LuaInterpreter(bool provide_libs, bool incl_env);

	private:

		//lw stands for lua wrapper
		static int lw_GetInstance(lua_State* L) {
			auto inst = (Instance*)(lua_touserdata(L, 1));
			//create a table
			//pushes the table on the top of the stack, so its safe to return
			inst->GetLuaTable(L); 

			return 1; //amount of values returned
		}

		static int lw_GetParent(lua_State* L) {
			auto inst = (Instance*)(lua_touserdata(L, 1));
			lua_pushlightuserdata(L, inst->GetParent());
			return 1;
		}

		static int lw_FindChild(lua_State* L) {
			std::string str = lua_tostring(L, -1);
			auto inst = (Instance*)(lua_touserdata(L, -2));

			for (auto& node : inst->GetChildren()) {
				if (node->Contents->name == str) {
					lua_pushlightuserdata(L, node->Contents);
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}
	};
}
