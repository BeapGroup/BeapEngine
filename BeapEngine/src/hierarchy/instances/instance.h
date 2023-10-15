#pragma once

#include <string>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>

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
		virtual unsigned int get_explorer_icon() const { return 0; }
		void remove();

		instance() = default;
		explicit instance(std::string const& instname) : name(instname) {}
		instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
	};

}


