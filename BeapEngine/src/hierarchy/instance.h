#pragma once

#include <string>
#include <algorithm>
#include "../math/tree.h"

namespace beap {

	class component {
	public:
		virtual std::string component_name() const = 0;
		virtual ~component() = 0;
	};
	class flag_component : public component {
	public:
		std::string component_name() const override { return "flag"; }
	};

	class instance {
	public:
		std::string name;
		node<instance*>* manu;

		std::vector<component> components;

		//virtual instance clone() = 0;
		//virtual std::string serialize() = 0;

		void add_to_tree(node<instance*>* parent);
		std::vector<node<instance*>*> getchildren() const;
		bool has_component(std::string const& cname);
		virtual unsigned int get_explorer_icon() const { return 0; }
		void remove();

		component* getcomponent(std::string_view const& cname);

		instance() = default;
		explicit instance(std::string const& instname) : name(instname) {}
		instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
	};

}


