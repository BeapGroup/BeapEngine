#include "beaplua.h"

namespace beap::lua {

	int LuaInterpreter::ExecuteString(std::string const& command) {
		return luaL_dostring(L, command.c_str());
	}

	float LuaInterpreter::GetNumber(std::string const& varname) {
		lua_getglobal(L, varname.c_str());
		if (lua_isnumber(L, -1)) {
			return (float)lua_tonumber(L, -1);
		}
	}

	std::string LuaInterpreter::GetString(std::string const& varname) {
		lua_getglobal(L, varname.c_str());
		if (lua_isstring(L, -1)) {
			return lua_tostring(L, -1);
		}
	}

	void LuaInterpreter::ReadFromStack() {
		std::cout << lua_tostring(L, -1) << std::endl;
	}

	LuaInterpreter::LuaInterpreter(bool provide_libs, bool incl_env) {
		if (provide_libs) {
			luaL_openlibs(L);
		}
		if (incl_env) {
			RegisterCppFunctions();
			luaL_dofile(L, "resources/scripts/main.lua");
		}
	}

	void LuaInterpreter::RegisterCppFunctions() {
		//functions are forced to be in a global scope
		//BeapEngine.FuncName could be achieved through lua_setfuncs
		//but its behavior is fucking hellish
		lua_register(L, "GetInstance", LuaInterpreter::lw_GetInstance);
		lua_register(L, "GetParent", LuaInterpreter::lw_GetParent);
		lua_register(L, "FindChild", LuaInterpreter::lw_FindChild);
	}
}


