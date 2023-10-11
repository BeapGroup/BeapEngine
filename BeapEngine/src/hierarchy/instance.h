#pragma once

#include <string>
#include <algorithm>
#include "../math/tree.h"

class component {
public:
	virtual std::string component_name() = 0;
};
class flag_component : public component {
public:
	std::string component_name() override { return "flag"; }
};

class instance {
public:
	std::string name;
	node<instance*>* manu;
	unsigned int explorer_icon;

	std::vector<component> components;

	//virtual instance clone() = 0;
	//virtual std::string serialize() = 0;

	void add_to_tree(node<instance*>* parent);
	std::vector<node<instance*>*> getchildren() const;

	bool has_component(std::string const& cname) {
		return std::any_of(components.begin(), components.end(), 
			[&cname](component& c) {
				return c.component_name() == cname;
			});
	}

	instance() = default;
	explicit instance(std::string const& instname) : name(instname) {}
	instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
};

