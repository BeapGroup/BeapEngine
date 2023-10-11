#include "instance.h"

namespace beap {
	void instance::add_to_tree(node<instance*>* parent) {
		auto* newnode = new node<instance*>(this);
		parent->add_child(newnode);
		manu = newnode;
	}

	std::vector<node<instance*>*> instance::getchildren() const {
		return manu->children;
	}

	bool instance::has_component(std::string const& cname) {

		return std::any_of(components.begin(), components.end(),
			[&cname](component const& c) {
				return c.component_name() == cname;
			});

	}

	component* instance::getcomponent(std::string_view const& cname) {
		for (auto& c : components) {
			if (c.component_name() == cname) {
				return &c;
			}
		}
	}

	void instance::remove() {
		manu->remove_parent();
		delete manu;
	}
}

