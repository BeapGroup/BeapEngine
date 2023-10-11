#pragma once
#include <optional>
#include <vector>
#include <functional>

namespace beap {

	template <typename T>
	struct node {

		T data;
		std::optional<node<T>*> parent;
		std::vector<node<T>*> children;

		bool has_children() {
			return children.size() > 0;
		}

		void add_child(node<T>* child) {
			child->parent = this;
			children.push_back(child);
		}
		void remove_child(node<T>* child) {
			child->parent.reset();
			auto i = std::find(children.begin(), children.end(), child);
			if (i != children.end()) {
				children.erase(i);
			}
		}
		void set_parent(node<T>* newparent) {
			newparent->add_child(this);
			parent = newparent;
		}
		void remove_parent() {
			if (parent.has_value()) {
				parent.value()->remove_child(this);
				parent.reset();
			}
		}

		void traverse(std::function<void(node<T>*)> action) {
			action(this);
			if (has_children()) {
				for (auto child : children) { child->traverse(action); }
			}
		}

		node() = default;
		explicit node(T value) : data(value) {}

		bool operator==(const node& other) const
		{
			return data == other.data;
		}
	};

}

