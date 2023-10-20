#include "Script.h"

namespace beap {

	void Script::Update(GLFWwindow* w, float dt) {
		//Call update(parent, dt) lua function
		lua_getglobal(LocalLua.L, "update");
		if (lua_isfunction(LocalLua.L, -1)) {
			lua_pushlightuserdata(LocalLua.L, GetParent());
			lua_pushnumber(LocalLua.L, dt);
			lua_pcall(LocalLua.L, 2, 0, 0);
		}
		else {
			std::cout << "[luadebug] Failed to detect update() in a " << name << " script" << std::endl;
		}

	}

}