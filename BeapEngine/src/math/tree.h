#pragma once
#include <optional>
#include <vector>
#include <functional>

namespace beap {

	template <typename T>
	struct Node {

		T Contents;
		std::optional<Node<T>*> Parent;
		std::vector<Node<T>*> Children;

		bool HasChildren() {
			return Children.size() > 0;
		}

		void AddChild(Node<T>* child) {
			child->Parent = this;
			Children.push_back(child);
		}
		void RemoveChild(Node<T>* child) {
			child->Parent.reset();
			auto i = std::find(Children.begin(), Children.end(), child);
			if (i != Children.end()) {
				Children.erase(i);
			}
		}
		void SetParent(Node<T>* newparent) {
			newparent->AddChild(this);
			Parent = newparent;
		}
		void RemoveParent() {
			if (Parent.has_value()) {
				Parent.value()->RemoveChild(this);
				Parent.reset();
			}
		}

		void Traverse(std::function<void(Node<T>*)> action) {
			action(this);
			if (HasChildren()) {
				for (auto child : Children) { child->Traverse(action); }
			}
		}

		Node() = default;
		explicit Node(T value) : Contents(value) {}

		bool operator==(const Node& other) const
		{
			return Contents == other.Contents;
		}
	};

}

