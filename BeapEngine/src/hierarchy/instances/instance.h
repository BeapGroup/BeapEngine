#pragma once

#define GLFW_INCLUDE_NONE

#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "../../math/tree.h"

namespace beap {

	class instance {
	public:
		std::string name;
		node<instance*>* in_tree;

		static node<instance*>* create_hierarchy(instance* root);

		void add_to_tree(node<instance*>* parent);
		std::vector<node<instance*>*> getchildren() const;
		instance* getparent() const;
		void remove();

		virtual unsigned int get_explorer_icon() const { return 0; }
		virtual std::string_view instance_type() const { return "instance"; }
		virtual void update(GLFWwindow* w, float dt) { return; }

		instance() = default;
		explicit instance(std::string const& instname) : name(instname) {}
		instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
	};

}


