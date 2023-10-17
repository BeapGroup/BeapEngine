#pragma once

#include <glm/glm.hpp>
#include "instance.h"

namespace beap {

	class GameObject : public Instance {
	public:
		glm::f32vec3 Position;
		glm::f32vec3 Rotation = glm::f32vec3(0);
		glm::f32vec3 Scale = glm::u32vec3(1);

		std::string_view InstanceType() const override { return "gameObject"; }
		void ResetTransform();
		void CopyTransform(GameObject const* gbj);

		virtual void Render() = 0;
		virtual void Move(glm::f32vec3 shift);
		virtual void SetPosition(glm::f32vec3 pos);
		virtual void SetRotation(glm::f32vec3 euler);
		virtual void SetScale(glm::f32vec3 scalar);

		GameObject() = default;
		GameObject(glm::u32vec3 pos, glm::f32vec3 rot, glm::u32vec3 scl)
			: Position(pos), Rotation(rot), Scale(scl) {}
		GameObject(glm::u32vec3 pos, const std::string& instname) 
			: Instance(instname), Position(pos) {}
		GameObject(glm::u32vec3 pos, const std::string& instname, Node<Instance*>* parent)
			: Instance(parent, instname), Position(pos) {}
	};
}