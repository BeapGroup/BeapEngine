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

	instance* instance::getparent() const {
		return in_tree->parent.value()->data;
	}
}

