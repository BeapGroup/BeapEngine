#include "gameobject.h"

namespace beap {

	void GameObject::ResetTransform() {
		Position = glm::u32vec3(0);
		Rotation = glm::f32vec3(0);
		Scale = glm::u32vec3(0);
	}

	void GameObject::CopyTransform(GameObject const* gbj) {
		Position = gbj->Position;
		Rotation = gbj->Rotation;
		Scale = gbj->Scale;
	}

	void GameObject::Move(glm::f32vec3 shift) {
		Position += shift;
	}

	void GameObject::SetPosition(glm::f32vec3 pos) {
		Position = pos;
	}

	void GameObject::SetRotation(glm::f32vec3 euler) {
		Rotation = euler;
	}

	void GameObject::SetScale(glm::f32vec3 scalar) {
		Scale = scalar;
	}

	void GameObject::GetLuaTable(lua_State* L) {
		Instance::GetLuaTable(L);

		lua_createtable(L, 0, 3);
		lua_pushnumber(L, Position.x);
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, Position.y);
		lua_setfield(L, -2, "y");
		lua_pushnumber(L, Position.z);
		lua_setfield(L, -2, "z");

		lua_setfield(L, -2, "position");

		lua_createtable(L, 0, 3);
		lua_pushnumber(L, Rotation.x);
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, Rotation.y);
		lua_setfield(L, -2, "y");
		lua_pushnumber(L, Rotation.z);
		lua_setfield(L, -2, "z");

		lua_setfield(L, -2, "rotation");

		lua_createtable(L, 0, 3);
		lua_pushnumber(L, Scale.x);
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, Scale.y);
		lua_setfield(L, -2, "y");
		lua_pushnumber(L, Scale.z);
		lua_setfield(L, -2, "z");

		lua_setfield(L, -2, "scale");
	}
}