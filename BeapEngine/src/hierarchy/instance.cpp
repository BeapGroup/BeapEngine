#include "instance.h"

void instance::add_to_tree(node<instance*>* parent) {
	node<instance*>* newnode = new node<instance*>(this);
	parent->add_child(newnode);
	manu = newnode;
}