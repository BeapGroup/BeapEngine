#include "instance.h"

namespace beap {
	void Instance::AddToTree(Node<Instance*>* parent) {
		auto* newnode = new Node<Instance*>(this);
		parent->AddChild(newnode);
		in_tree = newnode;
	}

	std::vector<Node<Instance*>*> Instance::GetChildren() const {
		return in_tree->Children;
	}

	void Instance::Remove() {
		in_tree->RemoveParent();
		delete in_tree;
	}

	Node<Instance*>* Instance::CreateHierarchy(Instance* root) {
		auto nodeptr = new Node<Instance*>(root);
		root->in_tree = nodeptr;
		return nodeptr;
	}

	Instance* Instance::GetParent() const {
		return in_tree->Parent.value()->Contents;
	}

	void Instance::GetLuaTable(lua_State* L) {
		lua_createtable(L, 0, 2);

		lua_pushstring(L, name.c_str());
		lua_setfield(L, -2, "name");

		lua_pushstring(L, std::string(InstanceType()).c_str());
		lua_setfield(L, -2, "type");
	}
}

