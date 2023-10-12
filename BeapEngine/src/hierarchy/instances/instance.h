#pragma once

#include <string>
#include <algorithm>
#include <glm/glm.hpp>

#include "../../math/tree.h"

namespace beap {
	
	class component;

	class instance {
	public:
		std::string name;
		node<instance*>* in_tree;
		std::vector<component*> components;

		static node<instance*>* create_hierarchy(instance* root);

		void add_to_tree(node<instance*>* parent);
		std::vector<node<instance*>*> getchildren() const;
		bool has_component(std::string const& cname);
		void attach(component* c);
		virtual unsigned int get_explorer_icon() const { return 0; }
		void remove();

		component* getcomponent(std::string_view const& cname);

		instance() = default;
		explicit instance(std::string const& instname) : name(instname) {}
		instance(node<instance*>* parent, std::string const& _name) : name(_name) { add_to_tree(parent); }
	};

	class component {
	public:
		virtual std::string component_name() const = 0;
		//virtual ~component() = 0;
	};

	class transform_component : public component {
	public:
		std::string component_name() const override { return "transform"; }

		glm::vec3 position;
		glm::vec3 rotation;

		static glm::vec3 getpos(instance* base);
		static glm::vec3 getrot(instance* base);


		transform_component(glm::vec3 pos) : position(pos) {}
		transform_component(glm::vec3 pos, glm::vec3 rot) : position(pos), rotation(rot) {}
	};

}


