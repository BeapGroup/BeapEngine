#pragma once

#include <string>
#include "../math/tree.h"

using haoma_node = node<instance*>*;

class instance {
public:
	std::string name;
	haoma_node manu;
	unsigned int explorer_icon;

	virtual instance clone() = 0;
	virtual std::string serialize() = 0;

	void add_to_tree(haoma_node parent);

	instance() = default;
	explicit instance(std::string const& instname) : name(instname) {}
	instance(haoma_node parent, std::string _name) : name(_name) { add_to_tree(parent); }
};