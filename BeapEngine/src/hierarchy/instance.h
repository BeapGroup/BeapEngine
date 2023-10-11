#pragma once

#include <string>
#include "../math/tree.h"

class instance {
public:
	std::string name;
	node<instance*>* manu;
	unsigned int explorer_icon;

	virtual instance clone() = 0;
	virtual std::string serialize() = 0;

	void add_to_tree(node<instance*>* parent);

	instance() = default;
	explicit instance(std::string const& instname) : name(instname) {}
	instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
};
