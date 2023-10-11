#include "instance.h"

void instance::add_to_tree(node<instance*>* parent) {
	auto* newnode = new node<instance*>(this);
	parent->add_child(newnode);
	manu = newnode;
}

std::vector<node<instance*>*> instance::getchildren() const {
	return manu->children;
}