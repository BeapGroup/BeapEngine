#include "hierarchy.h"

void instance::add_to_tree(haoma_node parent) {
	haoma_node newnode = new node<instance*>(this);
	parent->add_child(newnode);
	manu = newnode;
}