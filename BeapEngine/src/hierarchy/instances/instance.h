#pragma once

#define GLFW_INCLUDE_NONE

#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "../../math/tree.h"

extern "C" {
#include "../../thirdparty/lua/include/lua.h"
#include "../../thirdparty/lua/include/lauxlib.h"
#include "../../thirdparty/lua/include/lualib.h"
}


namespace beap {

	class Instance {
	public:
		std::string name;
		Node<Instance*>* in_tree;

		static Node<Instance*>* CreateHierarchy(Instance* root);

		void AddToTree(Node<Instance*>* parent);
		std::vector<Node<Instance*>*> GetChildren() const;
		Instance* GetParent() const;
		void Remove();

		virtual unsigned int GetExplorerIcon() const { return 0; }
		virtual std::string_view InstanceType() const { return "instance"; }
		virtual void Update(GLFWwindow* w, float dt) { return; }
		virtual void GetLuaTable(lua_State* L);

		Instance() = default;
		explicit Instance(std::string const& instname) : name(instname) {}
		Instance(Node<Instance*>* parent, std::string const& _name) : name(_name) { AddToTree(parent); }
	};

}


