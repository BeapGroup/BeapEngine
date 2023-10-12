#include "instance.h"

namespace beap {
	void instance::add_to_tree(node<instance*>* parent) {
		auto* newnode = new node<instance*>(this);
		parent->add_child(newnode);
		in_tree = newnode;
	}

	std::vector<node<instance*>*> instance::getchildren() const {
		return in_tree->children;
	}

	bool instance::has_component(std::string const& cname) {

		return std::any_of(components.begin(), components.end(),
			[&cname](component const* c) {
				return c->component_name() == cname;
			});

	}

	component* instance::getcomponent(std::string_view const& cname) {
		for (auto& c : components) {
			if (c->component_name() == cname) {
				return c;
			}
		}
	}

	void instance::remove() {
		in_tree->remove_parent();
		for (auto c : components) {
			delete c;
		}
		delete in_tree;
	}

	node<instance*>* instance::create_hierarchy(instance* root) {
		auto nodeptr = new node<instance*>(root);
		root->in_tree = nodeptr;
		return nodeptr;
	}

	void instance::attach(component* c) {
		components.emplace_back(c);
	}

	glm::vec3 transform_component::getpos(instance* base) {
		if (base->in_tree->parent.has_value()) {
			auto parent = base->in_tree->parent.value();
			if (parent->data->has_component("transform")) {
				return getpos(parent->data) + ((transform_component*)base->getcomponent("transform"))->position;
			}
			else { 
				return ((transform_component*)base->getcomponent("transform"))->position;
			}
		}
		else {
			return ((transform_component*)base->getcomponent("transform"))->position;
		}
	}

	glm::vec3 transform_component::getrot(instance* base) {
		if (base->in_tree->parent.has_value()) {
			auto parent = base->in_tree->parent.value();
			if (parent->data->has_component("transform")) {
				return getrot(parent->data) + ((transform_component*)base->getcomponent("transform"))->rotation;
			}
			else {
				return ((transform_component*)base->getcomponent("transform"))->rotation;
			}
		}
		else {
			return ((transform_component*)base->getcomponent("transform"))->rotation;
		}
	}
}

